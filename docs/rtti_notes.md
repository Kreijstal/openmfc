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

## TypeDescriptors and names (observed)
- Type_info vftable RVA (shared): `0x8A40`
- Name strings:
  - CRttiBase: RVA `0xC3E8` contains `.?\u0041VCrttiBase@@` (null-terminated), followed by pointer to type_info vftable `0x180008A40`.
  - CRttiDerived: RVA `0xC410` contains `.?\u0041VCrttiDerived@@` (null-terminated), followed by pointer to type_info vftable `0x180008A40`.
  - The TypeDescriptor bodies at `0xD1A8` / `0xD1D0` show the vftable RVA (0xF88A) in their payload; names reside at the RAVs above.

## ClassHierarchyDescriptors and BaseClass arrays
- CRttiBase CHD @ `0x98A8`:
  - sig=0, attrs=0, numBases=1, BaseClassArray RVA=`0x98C0`.
  - BaseClassArray @ `0x98C0`: entries `[0x98F0, 0, 0, 0]`.
  - BaseClassDescriptor @ `0x98F0`: raw dwords `(0, 0, 0, 0, 1, 0, 0, 0xD1C0)` (needs mapping to TD/PMD/attrs).

- CRttiDerived CHD @ `0x9920`:
  - sig=0, attrs=0, numBases=2, BaseClassArray RVA=`0x9948`.
  - BaseClassArray @ `0x9948`: entries `[0x9940?, 0x98F0, 0, 0]` (first entry likely self, second is CRttiBase descriptor).
  - BaseClassDescriptor @ `0x9960`: raw dwords `(0xD1C0, 1, 0, 0xFFFFFFFF, 0, 64, 0x9920, 0)` (interpretation pending).

## Remaining decode work
- Map BaseClassDescriptor fields to the standard layout (pTypeDescriptor RVA, numContainedBases, PMD {mdisp,pdisp,vdisp}, attributes, pClassHierarchyDescriptor).
- Confirm the self-descriptor RVA for CRttiDerived in the BaseClassArray (0x9940 vs 0x9960).
- Validate whether the TD at `0xD1A8/0xD1D0` should be referenced directly or via the string RVAs (`0xC3E8/0xC410`) when recreating RTTI in the shim.

## Next steps
1) Write a small parser (Python) to:
   - Read `rdata_dump.txt` and extract 64-bit values at the COL RVAs above.
   - Map the referenced RVAs back to `.rdata` offsets for TypeDescriptor and ClassHierarchyDescriptor.
2) Document the concrete struct layouts (offset-by-offset) for CRttiBase and CRttiDerived, including PMD values.
3) Feed these into the Phase 0B shim to emit a correct RTTI COL and TD for `CObject` so `dynamic_cast` from MSVC succeeds without crashing.
