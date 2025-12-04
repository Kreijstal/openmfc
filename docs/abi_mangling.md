# ABI Mangling Notes (Phase 0)

Collected patterns confirmed while making MSVC consume a MinGW-built DLL:

- **Manual vtables**: Declaring vtable arrays (and RTTI blobs) in assembly is viable; ensure vptr[-1] points at the COL and slots align with MSVC expectations.
- **`__attribute__((ms_abi))`**: Use on freestanding thunks when emitting from MinGW/Clang so call/return registers match MSVC.
- **`__asm__` labels**: Pin symbol names to MSVC-decorated forms (e.g., `??_7Class@@6B@`) when the compiler would otherwise choose a different spelling.
- **Import libs from DEF**: Deriving a `.def` from a MinGW DLL (`dumpbin /exports`) and feeding it to `lib.exe /def:...` produces a working `.lib` for MSVC consumers.
- **Factory entry points**: Export C factories (`CreateRef`/`DestroyRef`) to hand MSVC a valid object pointer; the host can then call virtual methods across the DLL boundary safely.
