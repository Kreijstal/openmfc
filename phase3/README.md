# Phase 3: Hollow Stub DLL & MSVC Link Verification

Phase 3 builds a hollow stub DLL with all 14,109 MFC exports using MinGW, then verifies MSVC can link against it.

## What This Phase Tests

- MinGW builds a DLL that exports MSVC-mangled names (via .def file aliasing)
- MSVC's `lib.exe` generates an import library from the .def file
- MSVC's linker resolves `__declspec(dllimport)` references at **link time**
- The mangled names MSVC generates match what's in the .def file

## Anti-Pattern: What NOT To Do

### Bad: Runtime Checks with LoadLibrary (Cheating)

```cpp
// WRONG: This is a runtime check, not a link-time check
HMODULE h = LoadLibraryA("openmfc.dll");
FARPROC p = GetProcAddress(h, "?AfxThrowMemoryException@@YAXXZ");
if (p) { /* "success" */ }
```

**Why this is wrong:**
- `LoadLibrary`/`GetProcAddress` work at **runtime**, not link time
- This doesn't prove MSVC can link against the DLL like it would with real MFC
- You could export any garbage symbol and this test would "pass"

### Bad: C-style Declarations

```cpp
// WRONG: Produces unmangled C symbol, not MSVC C++ mangled name
extern "C" __declspec(dllimport) void AfxThrowMemoryException();
```

**Why this is wrong:**
- `extern "C"` produces `AfxThrowMemoryException` (no mangling)
- Real MFC exports `?AfxThrowMemoryException@@YAXXZ` (MSVC C++ mangling)
- The linker would look for the wrong symbol

### Correct: C++ Declarations with dllimport

```cpp
// CORRECT: C++ linkage produces MSVC-mangled name
__declspec(dllimport) void __cdecl AfxThrowMemoryException();

int main() {
    AfxThrowMemoryException();  // Linker must resolve ?AfxThrowMemoryException@@YAXXZ
}
```

**Why this works:**
- MSVC compiles `void __cdecl AfxThrowMemoryException()` → `?AfxThrowMemoryException@@YAXXZ`
- The linker looks for this symbol in `openmfc.lib`
- If `cl test.cpp openmfc.lib` succeeds, the symbols match
- This is a **link-time** check, not runtime

## How It Works

```
┌─────────────────────────────────────────────────────────────────┐
│                    Phase 3 Architecture                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                   │
│  1. MinGW builds DLL with .def aliasing:                         │
│                                                                   │
│     openmfc.def:                                                  │
│       ?AfxThrowMemoryException@@YAXXZ=stub__AfxThrow... @2350    │
│                                                                   │
│     MinGW compiles: stub__AfxThrow...() { print "Not Impl"; }    │
│     .def maps:      stub__AfxThrow... → ?AfxThrowMemoryException │
│                                                                   │
│  2. DLL exports MSVC-mangled names:                              │
│                                                                   │
│     $ objdump -p openmfc.dll | grep AfxThrow                     │
│     [2094] +base[2350] ?AfxThrowMemoryException@@YAXXZ           │
│                                                                   │
│  3. MSVC generates import library:                               │
│                                                                   │
│     lib /DEF:openmfc.def /OUT:openmfc.lib /MACHINE:X64           │
│                                                                   │
│  4. MSVC links test app:                                         │
│                                                                   │
│     cl test.cpp openmfc.lib  ← LINK-TIME verification            │
│                                                                   │
│     Compiler: void AfxThrow...() → ?AfxThrowMemoryException@@... │
│     Linker:   finds ?AfxThrowMemoryException@@... in openmfc.lib │
│     Success:  symbols match, exe is created                      │
│                                                                   │
└─────────────────────────────────────────────────────────────────┘
```

## Build

```bash
# Build on Linux with MinGW cross-compiler
./phase3/scripts/build_phase3.sh

# Output:
#   build-phase3/openmfc.dll  - Hollow stub DLL (exports MSVC-mangled names)
#   build-phase3/openmfc.def  - Export definitions
```

## Verify Exports Locally

```bash
# Check that MSVC-mangled names are exported
x86_64-w64-mingw32-objdump -p build-phase3/openmfc.dll | grep "?AfxThrow"
```

## CI Workflow

The GitHub Actions workflow:

1. **Ubuntu job**: Builds openmfc.dll with MinGW
2. **Windows job**:
   - Generates import library: `lib /DEF:openmfc.def /OUT:openmfc.lib`
   - Compiles test app with MSVC: `cl test_msvc_app.cpp openmfc.lib`
   - If linking succeeds → test passes
   - Runs test to verify stubs are callable at runtime

## Key Insight

The **real** test is whether `cl.exe` can link. If this command succeeds:

```cmd
cl /nologo /EHsc /MD test_msvc_app.cpp openmfc.lib /Fe:test_app.exe
```

Then MSVC's linker found all the mangled symbols it needed in `openmfc.lib`, which means our .def file exports the correct MSVC-mangled names.

The runtime execution is just a bonus to verify the stubs are actually callable.
