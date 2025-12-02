# RTTI Harvest Notes (Phase 0A – Stage 11)

Harvest run: workflow `phase0a_harvest.yml`, run ID 19873293828  
Artifacts inspected: `/tmp/phase0a_artifacts_rtti/phase0a_msvc_artifacts/{rdata_dump.txt,relocations.txt,symbols.txt,layout.log}`

## Key RVAs (from exports/relocations)
- CRttiBase vftable: RVA `0x8A10`
- CRttiDerived vftable: RVA `0x8A28`
- CRttiBase COL (Complete Object Locator): RVA `0x8009878`  (`??_R4CRttiBase@@6B@`)
- CRttiDerived COL: RVA `0x8009900` (`??_R4CRttiDerived@@6B@`)

Relocations show the COL pointers placed at vptr[-1] for each vtable:
- At relocation RVA `0xA08`: DIR64 -> `0x180009878` (CRttiBase COL)
- At relocation RVA `0xA20`: DIR64 -> `0x180009900` (CRttiDerived COL)

## .rdata excerpts (objdump -s --section .rdata)
```
180008a00: 00000000 00000000 78980080 01000000   ; padding, then COL (CRttiBase) at 0x180009878
180008a10: db110080 01000000 00000000 00000000   ; vtable@8A10: [-1] = COL, slot0 = dtor
180008a20: 00990080 01000000 13110080 01000000   ; vtable@8A28: [-1] = COL (CRttiDerived), slot0/1 follow
180008a30: 00000000 00000000 90990080 01000000   ; additional slots for derived
```
Interpretation:
- At `0x180008A10` (CRttiBase vptr): vptr[-1] = `0x180009878` (COL), vptr[0] = scalar deleting dtor, vptr[1] = GetRuntimeClass.
- At `0x180008A28` (CRttiDerived vptr): vptr[-1] = `0x180009900` (COL), subsequent entries are destructor / GetRuntimeClass for the derived class.

## Complete Object Locator fields (decoded from rdata)
Each COL is 6 dwords (x64):
1. signature
2. offset
3. cdOffset
4. pTypeDescriptor (RVA)
5. pClassHierarchyDescriptor (RVA)
6. pObjectBase (RVA, often self)

- CRttiBase COL @ RVA `0x9878`:
  - (1, 0, 0, 0xD1A8, 0x98A8, 0x9868)
  - TypeDescriptor RVA: `0xD1A8`
  - ClassHierarchyDescriptor RVA: `0x98A8`
  - ObjectBase RVA: `0x9868`

- CRttiDerived COL @ RVA `0x9900`:
  - (1, 0, 0, 0xD1D0, 0x9920, 0x98F0)
  - TypeDescriptor RVA: `0xD1D0`
  - ClassHierarchyDescriptor RVA: `0x9920`
  - ObjectBase RVA: `0x98F0`

## What remains to decode
- Parse the COL structures at `0x180009878` and `0x180009900`:
  - Fields: signature, offset, cdOffset, pTypeDescriptor (RVA), pClassHierarchyDescriptor (RVA).
- Follow pTypeDescriptor to capture mangled type names (`.??_R0...`, likely under `.rdata` near `type_info` region).
- Follow pClassHierarchyDescriptor to decode attributes, number of bases, and BaseClassArray -> BaseClassDescriptors (PMD layout).

## Next steps
1) Write a small parser (Python) to:
   - Read `rdata_dump.txt` and extract 64-bit values at the COL RVAs above.
   - Map the referenced RVAs back to `.rdata` offsets for TypeDescriptor and ClassHierarchyDescriptor.
2) Document the concrete struct layouts (offset-by-offset) for CRttiBase and CRttiDerived, including PMD values.
3) Feed these into the Phase 0B shim to emit a correct RTTI COL and TD for `CObject` so `dynamic_cast` from MSVC succeeds without crashing.
