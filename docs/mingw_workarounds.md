# MinGW Capability Findings (Phase 0A.5)

## CRT / Heap
- MinGW DLL (`mingw_dll.dll`) imports `msvcrt.dll` (objdump import table).
- Host uses `msvcrt.dll`’s `free` to match DLL CRT; cross-CRT free with the host’s default CRT previously crashed (exit -1073740940).
- Recommendation: always free allocations with the same CRT that allocated them (export a free or expose allocator APIs).

## Symbol Export
- `.def` mapping works: `Shim_TestFunc` exported as `?GetValue@CTest@@UEAAPEAXH@Z`; MSVC host resolves and calls it successfully.

## Dependencies
- Imports: `KERNEL32.dll`, `msvcrt.dll`; built with `-static-libgcc -static-libstdc++`, so no extra GCC runtime DLLs required in this build.

## Open Items
- If UCRT alignment is required, adjust toolchain/flags and re-verify heap interop.
- Add a host-side negative test that frees with a mismatched CRT to document failure mode, if desired.
