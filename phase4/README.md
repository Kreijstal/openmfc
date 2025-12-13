# Phase 4: Implementation Loop

Phase 4 is where real MFC functionality gets implemented. The build system generates stubs for all 14,109 exports, and you override specific stubs with real implementations.

## Implementation Checklist

### Exception Throwing (`AfxThrowXxxException`)

| Function | Status | Notes |
|----------|--------|-------|
| `AfxThrowMemoryException` | ✅ Done | Full C++ exception, catchable by MSVC |
| `AfxThrowNotSupportedException` | ❌ Stub | |
| `AfxThrowArchiveException` | ❌ Stub | |
| `AfxThrowFileException` | ❌ Stub | |
| `AfxThrowInvalidArgException` | ❌ Stub | |
| `AfxThrowOleDispatchException` | ❌ Stub | |
| `AfxThrowOleException` | ❌ Stub | |
| `AfxThrowResourceException` | ❌ Stub | |
| `AfxThrowUserException` | ❌ Stub | |
| `AfxThrowDaoException` | ❌ Stub | |
| `AfxThrowDBException` | ❌ Stub | |
| `AfxThrowInternetException` | ❌ Stub | |

### Exception Classes RTTI

For exceptions to be catchable by type, we need MSVC-compatible RTTI structures:

| Type | TypeDescriptor | CatchableType | Status |
|------|----------------|---------------|--------|
| `CMemoryException*` | `.PEAVCMemoryException@@` | ✅ | Pointer type |
| `CException*` | `.PEAVCException@@` | ✅ | Base class |
| `CObject*` | `.PEAVCObject@@` | ✅ | Root base |
| Other exception types | - | ❌ | Not yet |

### Version/Info Functions

| Function | Status | Notes |
|----------|--------|-------|
| `AfxGetDllVersion` | ✅ Done | Returns 0x0E00 (MFC 14.0) |

### Core Classes

| Class | Status | Notes |
|-------|--------|-------|
| `CObject` | ✅ Done | Base class with RTTI (GetRuntimeClass, IsKindOf) |
| `CRuntimeClass` | ✅ Done | CreateObject, FromName, IsDerivedFrom |
| `CString` | ❌ Stub | String handling |
| `CException` | ❌ Stub | Exception base |
| `CWnd` | ❌ Stub | Window base |
| `CWinApp` | ❌ Stub | Application class |

## Test Results

Tests run on Windows with MSVC-compiled test binaries:

| Test | Status | Description |
|------|--------|-------------|
| `test_exception_simple` | ✅ Pass | `catch(...)` catches exception |
| `test_exception_typed` | ✅ Pass | `catch(CMemoryException*)` works |
| `test_exception_mfc` | ✅ Pass | Real MFC headers (`<afx.h>`) |
| `test_version` | ✅ Pass | `AfxGetDllVersion()` returns 0x0E00 |
| `test_cobject_rtti` | 🟡 Pending | CObject RTTI methods |
| `test_openmfc_suite` | 🟡 Pending | **Comprehensive test** (all features) |

## Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                         Phase 4 Build Flow                          │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  mfc_complete_ordinal_mapping.json (14,109 symbols)                 │
│         │                                                           │
│         ▼                                                           │
│  ┌─────────────────┐                                                │
│  │ gen_weak_stubs.py│                                               │
│  └────────┬────────┘                                                │
│           │                                                         │
│           ├──────────────────────┐                                  │
│           ▼                      ▼                                  │
│  ┌─────────────────┐    ┌─────────────────┐                        │
│  │  weak_stubs.cpp │    │  openmfc.def    │                        │
│  │                 │    │                 │                        │
│  │ stub_Foo()      │    │ ?Foo@@XZ=stub_Foo @256                   │
│  │ stub_Bar()      │    │ ?Bar@@XZ=stub_Bar @257                   │
│  │ ...14k stubs    │    │ ...14k exports                           │
│  └────────┬────────┘    └────────┬────────┘                        │
│           │                      │                                  │
│           ▼                      ▼                                  │
│  ┌─────────────────────────────────────────────────────┐           │
│  │            x86_64-w64-mingw32-g++                   │           │
│  │                                                     │           │
│  │  Links: implementations.o + weak_stubs.o            │           │
│  │  (First definition wins - implementations override) │           │
│  └────────────────────────┬────────────────────────────┘           │
│                           │                                         │
│                           ▼                                         │
│                  ┌─────────────────┐                               │
│                  │  openmfc.dll    │                               │
│                  │                 │                               │
│                  │ Real: Exceptions│                               │
│                  │ Real: Version   │                               │
│                  │ Stub: others    │                               │
│                  └─────────────────┘                               │
└─────────────────────────────────────────────────────────────────────┘
```

## Key Technical Details

### Exception Throwing (MSVC ABI Compatibility)

For MSVC-compiled code to catch exceptions thrown by OpenMFC:

1. **Pointer Type Names**: Use `.PEAVClassName@@` (not `.?AVClassName@@`)
   - `P` = pointer, `E` = __ptr64, `AV` = class type

2. **ThrowInfo Structure** (16 bytes on x64):
   ```cpp
   struct ThrowInfo {
       uint32_t attributes;           // 0
       int32_t pmfnUnwind;            // RVA to destructor (0 if none)
       int32_t pForwardCompat;        // 0
       int32_t pCatchableTypeArray;   // RVA to CatchableTypeArray
   };
   ```

3. **CatchableType Structure** (28 bytes):
   ```cpp
   struct CatchableType {
       uint32_t properties;           // 1 for pointer types
       int32_t pType;                 // RVA to TypeDescriptor
       int32_t mdisp, pdisp, vdisp;   // Displacement (0, -1, 0)
       int32_t sizeOrOffset;          // 8 for pointer on x64
       int32_t copyFunction;          // 0 (use memcpy)
   };
   ```

4. **TypeDescriptor** (variable size):
   ```cpp
   struct TypeDescriptor {
       const void* pVFTable;  // type_info vftable (8 bytes)
       void* spare;           // 0 (8 bytes)
       char name[];           // ".PEAVCMemoryException@@"
   };
   ```

5. **RVAs**: All pointers in exception structures are 32-bit RVAs relative to image base

### Why extern "C" Instead of C++ Classes?

GCC and MSVC have incompatible C++ ABIs:

```
GCC mangles:  _ZN7CObject15GetRuntimeClassEv
MSVC mangles: ?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ
```

Solution: Use `extern "C"` + `.def` file mapping:

```cpp
// Produces C symbol, .def maps to MSVC name
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    // Implementation
}
```

## Directory Structure

```
phase4/
├── README.md           # This file
├── include/            # Headers (if needed)
├── src/
│   ├── mfc_exceptions.cpp   # Exception throwing implementation
│   ├── version_impl.cpp     # AfxGetDllVersion
│   └── cobject_impl.cpp     # CObject and CRuntimeClass implementation
├── scripts/
│   └── build_phase4.sh      # Build script
└── tests/
    ├── test_openmfc_suite.cpp     # ** Comprehensive test suite **
    ├── test_exception_simple.cpp  # catch(...) test
    ├── test_exception_typed.cpp   # catch(CMemoryException*) test
    ├── test_exception_mfc.cpp     # Real MFC headers test
    ├── test_cobject_rtti.cpp      # CObject RTTI test
    └── test_version.cpp           # Version function test
```

## Build

```bash
# Build Phase 4
./phase4/scripts/build_phase4.sh

# Output:
#   build-phase4/openmfc.dll    - The DLL
#   build-phase4/libopenmfc.a   - MinGW import library
#   build-phase4/openmfc.def    - Export definitions
```

## Checking Implementation Progress

Use `scripts/check_implementation_status.py` to analyze which symbols are implemented vs stubbed:

```bash
# Summary by category
python3 scripts/check_implementation_status.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --summary

# Group by class name (top 30 classes)
python3 scripts/check_implementation_status.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --by-class

# List all symbols for a specific class
python3 scripts/check_implementation_status.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --by-class --filter-class CWnd

# With DLL analysis (shows weak vs strong symbols)
python3 scripts/check_implementation_status.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --obj build-phase4/weak_stubs.o \
    --impl-objs build-phase4/mfc_exceptions.o build-phase4/version_impl.o
```

The script uses `x86_64-w64-mingw32-nm` to identify:
- **Weak symbols**: Stubs that haven't been implemented yet
- **Strong symbols**: Real implementations that override stubs

## Testing with MSVC

The CI workflow:

1. Builds DLL with MinGW on Linux
2. Downloads DLL on Windows runner
3. Creates import library: `lib /DEF:openmfc.def /OUT:openmfc.lib`
4. Builds test apps with MSVC (including real MFC headers)
5. Runs tests and verifies exception catching works
