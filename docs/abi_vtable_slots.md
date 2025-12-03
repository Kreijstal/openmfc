# ABI VTable Slots (Phase 0B)

Status: MSVC reference observed via current shim build (release-style). Update this file if future dumps show additional slots.

## CObject
- 0: Scalar deleting destructor
- 1: GetRuntimeClass
- 2: Serialize
- 3: AssertValid — not present in current mfc140u.dll observation (release); omitted from shim vtable
- 4: Dump — not present in current mfc140u.dll observation (release); omitted from shim vtable

If future MFC exports show AssertValid/Dump, add matching thunks/exports to `phase0b/src/object.s` and `phase0b/def/mfc140u.def`.
