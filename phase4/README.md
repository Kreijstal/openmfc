# Phase 4: Implementation Loop

Phase 4 is where real MFC functionality gets implemented. The build system generates stubs for all 14,109 exports, and you override specific stubs with real implementations.

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
│                  │ Real: CString   │                               │
│                  │ Real: CObject   │                               │
│                  │ Stub: others    │                               │
│                  └─────────────────┘                               │
└─────────────────────────────────────────────────────────────────────┘
```

## Why Not Weak Symbols?

We initially tried using `__attribute__((weak))` to allow implementations to override stubs. However, PE/COFF (Windows executable format) handles weak symbols differently than ELF:

- On ELF (Linux): Weak symbols work as expected - strong definitions override weak ones
- On PE/COFF (Windows): Weak symbols become undefined references, causing linker errors

Instead, we use **link order** - the linker uses the first definition it finds. Link your implementations before `weak_stubs.o`.

## Why extern "C" Instead of C++ Classes?

**GCC and MSVC have incompatible C++ ABIs:**

```
GCC mangles:  _ZN7CObject15GetRuntimeClassEv
MSVC mangles: ?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ
```

These will **never match**. The solution:

1. All exports use `extern "C"` functions with stub names
2. The `.def` file maps stub names → MSVC-mangled export names
3. MSVC applications see correct symbols

```cpp
// WRONG - produces GCC-mangled symbol, won't work
class CObject {
    virtual CRuntimeClass* GetRuntimeClass();
};

// CORRECT - produces C symbol, .def maps to MSVC name
extern "C" void* MS_ABI stub__GetRuntimeClass_CObject__XZ() {
    return &CObject_classCObject;  // Return pointer to RTTI struct
}
```

## How to Implement a Function

### Step 1: Find the stub name

Use the implementation status checker:

```bash
python3 scripts/check_implementation_status.py \
    -m mfc_complete_ordinal_mapping.json \
    --by-class --filter-class CString
```

Or search the mapping directly:

```bash
grep "AfxThrowMemoryException" mfc_complete_ordinal_mapping.json
```

### Step 2: Write the implementation

Create a file in `phase4/src/` (or `src/mfc/`):

```cpp
// phase4/src/exceptions_impl.cpp

#include <cstdio>

// MS ABI calling convention for x64
#if defined(__GNUC__)
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Implementation of AfxThrowMemoryException
// Original: ?AfxThrowMemoryException@@YAXXZ
// Stub name: stub__AfxThrowMemoryException__YAXXZ
extern "C" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {
    // Real implementation here
    fprintf(stderr, "AfxThrowMemoryException called!\n");
    // In real impl: allocate and throw CMemoryException
}
```

### Step 3: Add to build

Edit `phase4/scripts/build_phase4.sh` to include your implementation:

```bash
IMPL_SOURCES=(
    "$ROOT/phase4/src/exceptions_impl.cpp"
    # Add more as implemented
)
```

### Step 4: Verify

```bash
./phase4/scripts/build_phase4.sh
```

The ABI verification will confirm all symbols are still present.

## Stub Name Convention

MSVC-mangled names are converted to valid C identifiers:

| Character | Replacement |
|-----------|-------------|
| `?` | `_` |
| `@` | `_` |
| Other special | `_` |

Examples:

| MSVC Symbol | Stub Name |
|-------------|-----------|
| `?AfxThrowMemoryException@@YAXXZ` | `stub__AfxThrowMemoryException__YAXXZ` |
| `??0CString@@QAE@XZ` | `stub___0CString__QAE_XZ` |
| `?GetLength@CString@@QBEHXZ` | `stub__GetLength_CString__QBEHXZ` |

## Data Exports

Some exports are data (static members, globals), not functions:

```cpp
// Data export: ?AFX_WM_DRAW2D@@3IA (global unsigned int)
unsigned int stub__AFX_WM_DRAW2D__3IA = 0;

// Data export: ?rectDefault@CFrameWnd@@2VCRect@@B (static const CRect)
void* stub__rectDefault_CFrameWnd__2VCRect__B = 0;
```

The `.def` file marks these with `DATA`:

```def
?AFX_WM_DRAW2D@@3IA=stub__AFX_WM_DRAW2D__3IA @2112 DATA
```

## Tools

### verify_abi_exports.py

Verifies the built DLL has all required symbols:

```bash
python3 scripts/verify_abi_exports.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --dll build-phase4/openmfc.dll
```

### check_implementation_status.py

Shows implementation progress:

```bash
# Summary by category
python3 scripts/check_implementation_status.py \
    -m mfc_complete_ordinal_mapping.json --summary

# Group by class
python3 scripts/check_implementation_status.py \
    -m mfc_complete_ordinal_mapping.json --by-class

# Filter to specific class
python3 scripts/check_implementation_status.py \
    -m mfc_complete_ordinal_mapping.json --by-class --filter-class CWnd
```

### gen_weak_stubs.py

Generates stubs and .def file:

```bash
python3 tools/gen_weak_stubs.py \
    --mapping mfc_complete_ordinal_mapping.json \
    --out-def build-phase4/openmfc.def \
    --out-stubs build-phase4/weak_stubs.cpp
```

## Symbol Categories

From the 14,109 exports:

| Category | Count | Description |
|----------|-------|-------------|
| Constructors | 765 | `??0ClassName@@...` |
| Destructors | 459 | `??1ClassName@@...` |
| Operators | 192 | `??BClassName@@...` (conversion), etc. |
| Virtual methods | 4,209 | `@@U...` or `@@V...` in signature |
| Static methods | 827 | `@@S...` in signature |
| Member functions | 3,731 | `@@Q...` or `@@A...` in signature |
| Global functions | 366 | `@@Y...` in signature |
| Data | 115 | `@@2...` or `@@3...` (static/global data) |
| Other | 3,445 | Templates, special members, etc. |

## Priority Classes for Implementation

Based on typical MFC application usage:

1. **CString** - String handling (heavily used)
2. **CObject** - RTTI base class
3. **CException** family - Exception handling
4. **CWnd** - Window base class
5. **CWinApp** - Application class
6. **CDialog** - Dialog windows
7. **CFrameWnd** - Frame windows
8. **CDC/CGdiObject** - GDI drawing

## Build

```bash
# Build Phase 4
./phase4/scripts/build_phase4.sh

# Output:
#   build-phase4/openmfc.dll    - The DLL
#   build-phase4/libopenmfc.a   - MinGW import library
#   build-phase4/openmfc.def    - Export definitions
```

## Testing with MSVC

The CI workflow will:

1. Download `openmfc.dll` and `openmfc.def`
2. Generate import library: `lib /DEF:openmfc.def /OUT:openmfc.lib`
3. Build test apps with MSVC
4. Verify linking and basic functionality
