# Phase 0A / 0A.5 Results and Verdict

## MSVC ABI Findings (Phase 0A)
- Layout: `CStage1_Simple` size 16 (vptr 0, member 8); `CStage4_Multi` size 40 with secondary base at +16.
- RVO: 8-byte POD returns in `rax`; POD 16/24 and 8-byte non-POD use hidden pointer (`rcx`, `rax=rcx`).
- Mangling catalog: see `docs/msvc_symbols.csv` (generated from exports).
- RTTI: present in object-level symbols (`symbols_objects.txt`), e.g., `??_R4CStage1_Simple@@6B@`; `_purecall` present for pure virtual interface.
- Covariant: `CCovariantDerived::Clone` returns derived pointer in same slot; no thunk in single inheritance.
- MI adjustor: `CStage4_Multi::FuncB` adjustor +16 (secondary base offset).

## MinGW Capability (Phase 0A.5)
- Symbol export: `.def` mapping works; MSVC host resolves decorated name and calls into MinGW DLL.
- Heap interop: MinGW DLL imports `msvcrt.dll`; host frees using `msvcrt` `free` to avoid CRT mismatch crash.
- Exception sanity: DLL throws/catches internally, returns 42; host sees success.
- Dependencies: `KERNEL32.dll`, `msvcrt.dll`; built with `-static-libgcc -static-libstdc++`.

## Risks / Workarounds
- Heap ownership must stay within the same CRT; export allocator/free or match CRTs.
- RTTI/purecall confirmed; if richer RTTI needed, rely on object-level dumps.
- No covariant MI thunk yet; add if required for MI coverage.

## Verdict (provisional)
- MSVC ABI data collected; MinGW basic interop validated with caveats on heap.
- Proceed to Phase 0B if comfortable with CRT ownership rules, or add targeted tests (MI covariant, negative heap case) before final GREEN.

## Risk Register (snapshot)

| Risk | Severity | Probability | Mitigation |
|------|----------|-------------|------------|
| Heap mismatch across CRTs | Med | 40% | Keep ownership within one CRT; export alloc/free. |
| MI covariant gaps | Low | 20% | Add MI covariant probe before Phase 0B if needed. |
| RTTI capture completeness | Low | 20% | Use object-level dumps; add flags/tests if missing. |
