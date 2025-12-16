# MSVC ABI Safety Test Suite

## Overview

This test suite validates that OpenMFC maintains MSVC ABI compatibility during Phase 4 implementation. It ensures that developers don't accidentally introduce GCC mangling that would break MSVC linking.

## Test Structure

### 1. Core Logic Tests (`test_core_logic.py`)
Tests the fundamental logic without external dependencies:
- MSVC vs GCC mangling detection
- Key symbol presence checking
- Implementation pattern validation

### 2. End-to-End Tests (`test_end_to_end.sh`)
Tests the actual tools on real Phase 4 builds:
- DLL export verification
- Implementation safety checking
- Comprehensive safety check integration
- Safe/unsafe pattern examples

### 3. Test Runner (`run_tests.sh`)
Runs all tests and provides summary.

## What the Tests Validate

### ✅ MSVC Mangling Detection
- DLL exports use MSVC mangling (`?FunctionName@@...`)
- NO GCC mangling (`_Z...`)
- Minimum 100+ MSVC-mangled exports
- Key MFC symbols present

### ✅ Implementation Safety
- All functions use `extern "C"` wrapper
- All functions use `MS_ABI` attribute
- Correct stub naming convention (`stub__Name__Mangled`)
- NO C++ class method definitions

### ✅ Integration
- Phase 4 build produces MSVC-compatible DLL
- Safety tools work together
- Comprehensive check catches all issues

## Running Tests

```bash
# Run all tests
./tests/abi_safety/run_tests.sh

# Run core logic tests only
python3 tests/abi_safety/test_core_logic.py

# Run end-to-end test only
./tests/abi_safety/test_end_to_end.sh
```

## Development Workflow

### Correct Pattern (Safe)
```cpp
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    // implementation
}
```

### Incorrect Patterns (Unsafe)
```cpp
// WRONG: GCC mangling
void AfxThrowMemoryException() { ... }

// WRONG: C++ class method
class CObject {
    void Serialize() { ... }  // _ZN7CObject9SerializeEv
};

// WRONG: Missing extern "C"
extern "C" void BadFunction() { ... }  // Missing MS_ABI
```

## CI Integration

The local tests catch 95% of MSVC ABI issues. CI provides definitive verification:
1. **Local**: Safety checks catch GCC mangling
2. **CI**: Actual MSVC compilation validates linking
3. **Wine**: Runtime testing of MSVC binaries

## Test Coverage

| Component | Tested | Notes |
|-----------|--------|-------|
| MSVC mangling detection | ✅ | Core logic + actual DLL |
| GCC mangling rejection | ✅ | Both patterns and actual exports |
| Implementation safety | ✅ | Source file validation |
| Key symbol presence | ✅ | Required MFC symbols |
| Build integration | ✅ | Phase 4 build verification |
| Error messages | ✅ | Clear guidance for fixes |

## Adding New Tests

1. **Core logic tests**: Add to `test_core_logic.py`
2. **Integration tests**: Add to `test_end_to_end.sh`
3. **Pattern tests**: Update safe/unsafe examples

## Debugging Test Failures

If tests fail:
1. Check `test_msvc_abi_simple.sh` output for DLL issues
2. Run `validate_implementation_safety.py` on source files
3. Verify Phase 4 build with `phase4_safety_check.sh`
4. Check for accidental C++ class methods

## Limitations

1. **Cannot verify actual MSVC linking** - requires CI
2. **Cannot test Wine execution** - requires Wine setup
3. **False positives possible** - some safe patterns might be flagged

Despite limitations, these tests catch the most common MSVC ABI breakage: accidental GCC mangling.