# Phase 4 ABI Safety Workflow

## Overview

Phase 4 implements real MFC functionality while maintaining **exact ABI compatibility** with `mfc140u.dll`. The API is set in stone - we cannot change it. This workflow ensures OpenMFC remains a drop-in replacement.

## Key Principles

1. **API is immutable** - Must match real MFC exactly (14,109 exports at correct ordinals)
2. **MSVC mangling only** - No GCC mangling allowed (MSVC applications must link)
3. **Safe implementation patterns** - Use `extern "C"` with stub names
4. **Regression prevention** - Detect ABI breaks before they reach CI

## Developer Workflow

### 1. After Every Build
```bash
# Build Phase 4
./phase4/scripts/build_phase4.sh

# This automatically runs:
# - verify_exact_abi.py (exact ordinal matching)
# - phase4_safety_check.sh (comprehensive safety check)
```

### 2. Manual Safety Check
```bash
# Run comprehensive safety check
./scripts/phase4_safety_check.sh

# Individual checks:
./scripts/verify_exact_abi.py --mapping mfc_complete_ordinal_mapping.json --dll build-phase4/openmfc.dll
./scripts/test_msvc_abi_simple.sh build-phase4/openmfc.dll
./scripts/validate_implementation_safety.py --src-dir src/mfc --src-dir phase4/src
./scripts/check_abi_regression.sh
./scripts/test_dropin_replacement.sh
```

### 3. Before Committing
```bash
# Ensure all safety checks pass
./scripts/phase4_safety_check.sh

# Run tests
cd tests/abi_safety && ./run_tests.sh
```

## Safety Checks

### 1. Exact ABI Verification (`verify_exact_abi.py`)
- Verifies all 14,109 exports match real MFC
- Checks correct ordinals (e.g., `?AfxThrowMemoryException@@YAXXZ` @ 2350)
- Validates no missing or extra exports

### 2. MSVC Mangling Detection (`test_msvc_abi_simple.sh`)
- Ensures DLL uses **MSVC mangling** (`?FunctionName@@...`)
- Detects and rejects **GCC mangling** (`_Z...`)
- Requires minimum 100+ MSVC-mangled exports

### 3. Implementation Safety (`validate_implementation_safety.py`)
- Scans source files for unsafe patterns:
  - **Safe**: `extern "C" void MS_ABI stub__FunctionName__Mangled()`
  - **Unsafe**: C++ class methods, missing `extern "C"`, missing `MS_ABI`
- Prevents accidental GCC mangling at source level

### 4. Regression Detection (`check_abi_regression.sh`)
- Detects if ABI compatibility has been broken
- Compares against known-good baseline
- Provides clear error messages

### 5. Drop-in Replacement Test (`test_dropin_replacement.sh`)
- Tests if OpenMFC can actually replace `mfc140u.dll`
- Creates test MSVC application for linking verification

## CI Integration

The ABI safety checks run automatically in CI:

1. **On every push to phase4-* branches**: `phase4_abi_safety.yml`
2. **On pull requests to master**: `phase4_abi_safety.yml`
3. **Manual trigger**: GitHub Actions workflow dispatch

### CI Workflow Steps:
1. Build Phase 4 DLL with MinGW
2. Run all ABI safety checks
3. Create MSVC import library
4. Build and run MSVC test application
5. Verify linking works correctly

## Implementation Guidelines

### Safe Pattern (Required)
```cpp
// CORRECT: extern "C" with stub name
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    // Implementation here
    throw std::bad_alloc();
}
```

### Unsafe Patterns (Will Fail Safety Check)
```cpp
// WRONG: C++ class method (GCC mangling)
void AfxThrowMemoryException() {  // Will be mangled as _Z24AfxThrowMemoryExceptionv
    throw std::bad_alloc();
}

// WRONG: Missing extern "C"
void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {  // Still GCC mangled
    throw std::bad_alloc();
}

// WRONG: Missing MS_ABI (calling convention mismatch)
extern "C" void stub__AfxThrowMemoryException__YAXXZ() {  // Wrong calling convention
    throw std::bad_alloc();
}
```

## Troubleshooting

### Common Issues

1. **Missing exports**:
   ```
   Error: Missing export: ?AfxThrowMemoryException@@YAXXZ (ordinal 2350)
   ```
   **Fix**: Ensure symbol is in `mfc_complete_ordinal_mapping.json` and stub is generated.

2. **GCC mangling detected**:
   ```
   Error: Found GCC-mangled export: _Z24AfxThrowMemoryExceptionv
   ```
   **Fix**: Use `extern "C" void MS_ABI stub__FunctionName__Mangled()` pattern.

3. **Wrong ordinal**:
   ```
   Error: Wrong ordinal for ?AfxThrowMemoryException@@YAXXZ: expected 2350, got 2351
   ```
   **Fix**: Check `mfc_complete_ordinal_mapping.json` and regenerate stubs.

4. **Implementation safety violation**:
   ```
   Error: Unsafe pattern in src/mfc/exceptions.cpp:42
   ```
   **Fix**: Convert to safe `extern "C"` pattern.

### Debugging Tools

```bash
# View DLL exports
x86_64-w64-mingw32-objdump -p build-phase4/openmfc.dll | grep "^\s*\["

# Check specific symbol
x86_64-w64-mingw32-nm build-phase4/openmfc.dll | grep AfxThrowMemoryException

# Generate import library for testing
lib /DEF:build-phase4/openmfc.def /OUT:openmfc.lib /MACHINE:X64
```

## Adding New Implementations

1. **Add to excluded symbols** in `phase4/scripts/build_phase4.sh`:
   ```bash
   EXCLUDED_SYMBOLS="?AfxThrowMemoryException@@YAXXZ,..."
   ```

2. **Create implementation file** in `phase4/src/`:
   ```cpp
   // phase4/src/new_impl.cpp
   #include <openmfc/afx.h>
   
   extern "C" void MS_ABI stub__NewFunction__Mangled() {
       // Implementation
   }
   ```

3. **Add to build script**:
   ```bash
   IMPL_SOURCES=(
       "$ROOT/phase4/src/exceptions_impl.cpp"
       "$ROOT/phase4/src/new_impl.cpp"
   )
   ```

4. **Run safety checks**:
   ```bash
   ./phase4/scripts/build_phase4.sh
   ./scripts/phase4_safety_check.sh
   ```

## Emergency Procedures

If CI fails due to ABI regression:

1. **Don't force push** - This breaks the git history
2. **Revert the offending commit**:
   ```bash
   git revert <commit-hash>
   ```
3. **Fix the issue** and create a new commit
4. **Verify locally** before pushing:
   ```bash
   ./scripts/phase4_safety_check.sh
   ```

## Related Files

- `scripts/phase4_safety_check.sh` - Main safety check entry point
- `scripts/verify_exact_abi.py` - Core ABI compatibility check
- `scripts/validate_implementation_safety.py` - Source file safety
- `mfc_complete_ordinal_mapping.json` - Real MFC ABI definition
- `.github/workflows/phase4_abi_safety.yml` - CI workflow
- `tests/abi_safety/` - Test suite for safety checks