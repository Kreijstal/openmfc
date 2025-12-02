# SEH / EH Notes (Phase 0A)

Artifacts: `disassembly.txt`, `symbols_objects.txt`, MinGW harness logs.

- Most probe functions are leaf/trivial; no SEH frames are emitted.
- The helper `_M@YAXPEAX_K1P6AX0@Z@Z` (seen at `disassembly.txt` 21E0+) shows a standard SEH-style prologue:
  - saves non-volatiles (RBX, RSI, RDI, R14), `sub rsp,38h` (keeps 16-byte alignment).
- `_purecall` is imported for pure virtuals; RTTI is available to support EH metadata.
- MinGW capability test: `Shim_SafeException` throws/catches internally and returns 42; host observes success ⇒ no cross-frame corruption when exceptions stay inside the DLL (EH does not cross the boundary).
- Cross-boundary throws are intentionally avoided; if needed later, add a negative test that throws across DLL/EXE and expect failure.
