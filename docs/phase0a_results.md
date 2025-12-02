# Phase 0A / 0A.5 Results and Verdict

## MSVC ABI Findings (Phase 0A)
- Layout: `CStage1_Simple` size 16 (vptr 0, member 8); `CStage4_Multi` size 40 with secondary base at +16 (per `layout.log`).
- RVO: 8-byte POD returns in `rax`; POD 16/24 and 8-byte non-POD use hidden pointer (`rcx`, `rax=rcx`) — see `disassembly.txt`.
- Mangling catalog: `docs/msvc_symbols.csv` (~90 rows) generated from `exports.txt`.
- RTTI: present in `symbols_objects.txt` (`??_R4*`, `??_R0*`), `_purecall` imported for pure virtual interface.
- Covariant: `CCovariantDerived::Clone` returns derived pointer in same slot; no thunk in single inheritance.
- MI adjustor: `CStage4_Multi::FuncB` adjustor +16 (secondary base offset).
- Calling convention probe: 5th/6th args read from `[rsp+28h]/[rsp+30h]` (`SumFive`/`CCStdCall`/`CCThiscall`).

## MinGW Capability (Phase 0A.5)
- Symbol export: `.def` mapping works; MSVC host resolves decorated name and calls into MinGW DLL.
- Heap interop: MinGW DLL imports `msvcrt.dll`; host now uses DLL exports `Shim_Malloc` + `Shim_Free` to keep allocation/free in the same CRT. Cross-CRT free is forbidden (previously crashed); optional forbidden test is guarded by `ENABLE_FORBIDDEN_CROSS_FREE=1`.
- Exception sanity: DLL throws/catches internally, returns 42; host sees success.
- Dependencies: `KERNEL32.dll`, `msvcrt.dll`; built with `-static-libgcc -static-libstdc++`.

## Risks / Workarounds
- Heap ownership must stay within the same CRT; export alloc/free or align CRTs explicitly.
- No covariant MI thunk yet; add if required for MI coverage.
- Calling convention observation used trivial bodies; heavier probes could confirm shadow-space emission if needed.

## Verdict
- Status: **YELLOW (conditional pass)** — heap test passes only when both sides use `msvcrt` (per CI run 19851008868: `[Pass?] Freed memory (No crash detected yet)`).
- Proceed to Phase 0B with the CRT-ownership rule enforced; probability of success ~75%.
- Decision authority: OpenMFC maintainer (acting) — approves proceeding with the constraint above.

## Risk Register (snapshot)

| Risk | Severity | Probability | Mitigation |
|------|----------|-------------|------------|
| Heap mismatch across CRTs | Med | 40% | Keep ownership within one CRT; export alloc/free; align CRTs only if controlled. |
| MI covariant gaps | Low | 20% | Add MI covariant probe before shipping any MI-dependent surface. |
| RTTI capture completeness | Low | 20% | Use object-level dumps (`symbols_objects.txt`); add focused builds if gaps appear. |
| Calling convention detail gaps | Med | 30% | Add non-trivial probe with real calls to force shadow space if future evidence needed. |

## Go / No-Go (final)
- All required artifacts present (DLL, PDB, exports, symbols, disassembly, layout, README, host).
- Datasheet and symbol catalog populated; knowledge tests answered.
- CI workflows green (harvest + MinGW verify).
- Proceed to Phase 0B with the documented caveats above.
