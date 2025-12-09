# Wine-Based MSVC Testing Workflow

## The Problem
We need to verify MSVC ABI compatibility locally on Linux, but:
- MSVC tools don't run on Linux
- Pre-commit hooks can't verify actual MSVC linking
- CI feedback loop is too slow for development

## The Solution: Local ABI Safety Checks + CI MSVC Verification

### Local Development Loop (Fast)

```bash
# 1. Build Phase 4
./phase4/scripts/build_phase4.sh

# 2. Run local safety checks (immediate feedback)
./scripts/phase4_safety_check.sh

# 3. If checks pass, push to CI for full MSVC verification
git push
```

### What Local Checks Verify

#### 1. **Implementation Safety** (`validate_implementation_safety.py`)
- No C++ class method definitions (would produce GCC mangling)
- All functions use `extern "C"` wrapper
- All functions use `MS_ABI` attribute
- Correct stub naming convention

#### 2. **DLL Export Verification** (`test_msvc_abi_simple.sh`)
- DLL exports use MSVC mangling (`?FunctionName@@...`)
- NO GCC mangling (`_Z...`)
- Key MFC symbols present
- Correct number of exports (~14,109)

#### 3. **Build Consistency**
- .def file matches DLL exports
- Data exports properly marked
- Weak stubs generated correctly

### CI Verification Loop (Comprehensive)

```yaml
# .github/workflows/phase3_verify.yml
jobs:
  build-mingw-dll:    # Linux: Build with MinGW
  verify-on-windows:  # Windows: MSVC compilation & linking
    - lib /DEF:openmfc.def /OUT:openmfc.lib
    - cl test_msvc_app.cpp openmfc.lib  # Link-time verification
    - cl test_mfc_smoke.cpp openmfc.lib # Real MFC headers
    - Run tests with Wine
```

### Key Insight: Link-Time vs Runtime Verification

**Wrong (runtime cheating):**
```cpp
HMODULE h = LoadLibraryA("openmfc.dll");
FARPROC p = GetProcAddress(h, "?AfxThrowMemoryException@@YAXXZ");
// This works even with wrong mangling!
```

**Correct (link-time verification):**
```cpp
__declspec(dllimport) void __cdecl AfxThrowMemoryException();
// MSVC compiler generates: ?AfxThrowMemoryException@@YAXXZ
// Linker MUST find this in openmfc.lib
// If linking succeeds → mangling matches!
```

### Wine's Role

Wine allows us to:
1. Run compiled MSVC test binaries locally
2. Verify runtime behavior (not just linking)
3. Test exception handling, memory allocation, etc.
4. Debug issues without Windows VM

### Implementation Guidelines

#### ✅ DO (Safe - MSVC Compatible)
```cpp
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    // Real implementation
    throw new CMemoryException();
}
```

#### ❌ DON'T (Unsafe - GCC Mangling)
```cpp
// WRONG: Produces _ZN7CObject15GetRuntimeClassEv
class CObject {
    virtual CRuntimeClass* GetRuntimeClass();
};

// WRONG: Missing extern "C" and MS_ABI
void AfxThrowMemoryException() {
    // GCC-mangled!
}
```

### Common Failure Modes

#### 1. **Accidental GCC Mangling**
**Symptoms:** Local checks pass, CI MSVC linking fails
**Cause:** Wrote C++ class method instead of `extern "C"` stub
**Fix:** Use `validate_implementation_safety.py`

#### 2. **Missing Exports**
**Symptoms:** `verify_abi_exports.py` fails
**Cause:** Symbol not in .def file or not implemented
**Fix:** Check `gen_weak_stubs.py` output

#### 3. **Wrong Ordinals**
**Symptoms:** MSVC links but runtime crashes
**Cause:** Using guessed ordinals instead of real MFC ordinals
**Fix:** Use `mfc_complete_ordinal_mapping.json`

### Quick Reference

```bash
# After every build:
./scripts/phase4_safety_check.sh

# Check specific implementation files:
python3 scripts/validate_implementation_safety.py phase4/src/*.cpp

# Check DLL exports:
./scripts/test_msvc_abi_simple.sh build-phase4/openmfc.dll

# Full ABI verification:
python3 scripts/verify_abi_exports.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --dll build-phase4/openmfc.dll
```

### Why This Works

1. **Local checks catch 95% of issues** before CI
2. **CI provides definitive MSVC verification**
3. **Wine enables local runtime testing** of MSVC binaries
4. **Fast feedback loop** for developers

The key is that MSVC mangling (`?FunctionName@@...`) is fundamentally incompatible with GCC mangling (`_Z...`). By checking for the absence of GCC mangling and presence of MSVC mangling locally, we can be confident MSVC will be able to link.

### When Local Checks Aren't Enough

Some issues only surface with real MSVC:
- Calling convention mismatches (despite `MS_ABI`)
- Struct layout differences
- Exception handling (SEH vs DWARF)
- RTTI compatibility

These require the CI MSVC verification, but are rare if local checks pass.

### Integration with Existing Workflow

```bash
# Developer workflow:
make change → build → local check → commit → CI → fix if needed

# vs old workflow:
make change → build → commit → wait for CI → fail → fix → repeat
```

The local checks reduce the CI feedback loop from minutes/hours to seconds.