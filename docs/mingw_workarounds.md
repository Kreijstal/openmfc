# MinGW Capability Findings (Phase 0A.5)

## CRT / Heap
- MinGW DLL (`mingw_dll.dll`) imports `msvcrt.dll` (objdump import table).
- Host and DLL must use the same CRT for ownership: exports now include `Shim_Malloc` **and** `Shim_Free`; the host always calls `Shim_Free` (same CRT) for the matching free.
- Cross-CRT free with the host’s default CRT previously crashed (exit -1073740940). A forbidden test can be enabled via `ENABLE_FORBIDDEN_CROSS_FREE=1` to demonstrate/guard this; it is off by default to avoid intentional crashes in CI.
- Recommendation: never cross-free; always pair alloc/free inside the producing CRT or align CRTs explicitly if you choose to prove it.
- Exception sanity: internal throw/catch returns 42; no SEH/EH corruption observed when exceptions stay inside the DLL.

## Symbol Export
- `.def` mapping works: `Shim_TestFunc` exported as `?GetValue@CTest@@UEAAPEAXH@Z`; MSVC host resolves and calls it successfully.

## Dependencies
- Imports: `KERNEL32.dll`, `msvcrt.dll`; built with `-static-libgcc -static-libstdc++`, so no extra GCC runtime DLLs required in this build.

## Open Items
- If UCRT alignment is required, adjust toolchain/flags and re-verify heap interop.
- Add a host-side negative test that frees with a mismatched CRT to document failure mode, if desired.
- Capture RTTI/SEH patterns if needed by building a MinGW DLL with C++ classes and inspecting imports/metadata.
