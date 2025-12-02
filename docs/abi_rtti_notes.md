# RTTI Notes (Phase 0A)

Artifacts: `/tmp/phase0a_msvc_artifacts/symbols_objects.txt`

Observed COL (`??_R4`) examples:
- `??_R4CStage1_Simple@@6B@`
- `??_R4CStage4_Multi@@6BCStage4_A@@@` and `??_R4CStage4_Multi@@6BCStage4_B@@@`
- `??_R4CStage6_Modern@@6B@`
- `??_R4CCovariantDerived@@6B@`
- `??_R4IPure@@6B@`

Observed type descriptors (`??_R0`):
- `??_R0?AVCStage1_Simple@@@8`
- `??_R0?AVCStage4_Multi@@@8`
- `??_R0?AVCStage6_Modern@@@8`
- `??_R0?AVCCovariantDerived@@@8`
- `??_R0?AVIPure@@@8`

Layout log shows vftable meta entries (slot -1) that correspond to these COLs. RTTI is present even for the pure virtual interface (`IPure`) and MI class (`CStage4_Multi`).
