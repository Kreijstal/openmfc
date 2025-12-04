# Phase 0B: CObject Bootstrap

This directory bootstraps the first real OpenMFC surface: `CObject`.

Goals:
- Provide a clean-room `afx.h` with `CObject` and `CRuntimeClass`.
- Build a MinGW DLL (`openmfc.dll`) that exports the `CObject` surface.
- Build an MSVC host test that links/loads the DLL and exercises construction, RTTI (`GetRuntimeClass` / `IsKindOf`), and destruction.
- Harvest the real MSVC-decorated names via CI and use them in `def/mfc140u.def`.

Notes:
- `CObject` matches MFC semantics for Phase 0B: it is **not** dyncreate. `CRuntimeClass::m_pfnCreateObject` is `nullptr` and `CObject::CreateObject()` returns `nullptr`; tests allocate explicitly via `new`.
- `CRuntimeClass::m_nObjectSize` matches the harvested layout size (8 bytes, just the vptr).
- Heap ownership stays inside the DLL (our `operator new/delete` use the DLL CRT). Do not cross-free from MSVC unless explicitly validated.
- Export names must be generated (see `scripts/harvest_cobject_symbols.ps1`) and captured in `def/mfc140u.def`.
- CI workflow `.github/workflows/phase0b_harvest_cobject.yml` runs the harvest script on Windows, uploads `mfc140u.generated.def`, and stores logs under `phase0b/artifacts/harvest/`.
