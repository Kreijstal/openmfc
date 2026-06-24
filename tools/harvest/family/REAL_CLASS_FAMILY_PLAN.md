# Real Class Family — Preparation (CMFCToolBarButton family)

Foundation for moving OpenMFC stubs from *ABI-shaped padding shells* to *real class
bodies with genuine, differentially-verified behavior*. First family: the
`CMFCToolBarButton` button hierarchy — the data substrate the whole toolbar /
`SaveState` story rests on, and (unlike GUI handlers) **headlessly verifiable** via a
Serialize / CopyFrom / CompareWith round-trip against real `mfc140u` under wine.

## Why this family first
- Bounded, fully-documented public SDK header (`atlmfc/include/afxtoolbarbutton.h`, 249
  lines) → member layout is **ground truth, not reverse-engineered**.
- `DECLARE_SERIAL` → real `CreateObject` factory + `Serialize` are exercisable with no
  window/DC/message pump, so the differential harness verifies them on this Linux host.
- It is the base of the in-repo derived buttons (ComboBox/EditBox/Menu) and the element
  type toolbars persist — real bodies here unblock the toolbar `SaveState` direction.

## Ground truth already harvested (verified, not assumed)
Tooling committed under `tools/harvest/family/` (all reproducible on this host):
- `build_family_probe.sh` — clang-cl + lld-link + wine recipe; links the **real**
  `mfc140u.lib`, runs against the **real** redist DLLs. Creates exact-case header/lib
  symlinks (Linux is case-sensitive; MFC `#include`s are mixed-case).
- `cmfctoolbarbutton_layout_probe.cpp` — `sizeof`/`offsetof` of every member.
- `cmfctoolbarbutton_golden_probe.cpp` — RTTI schema, ctor field values, SetImage,
  CopyFrom/CompareWith, and a full Serialize round-trip with hex dump.
- `cmfctoolbarbutton_golden.json` — the captured ground truth (the verification target).

Key facts (real mfc140u 14.51.36231):
- `sizeof CMFCToolBarButton = 136`; exact member offsets pinned (see golden.json).
  Current repo stub is **64 bytes with wrong member order** and wrongly `DECLARE_DYNAMIC`.
- RTTI: schema `0x80000001` (VERSIONABLE_SCHEMA|1), objsize 136, `CreateObject` non-null
  → real `DECLARE_SERIAL(CMFCToolBarButton, 1)`.
- Default ctor: `m_iImage=-1, m_bImage=1, m_bVisible=1, m_bHorz=1`, all else 0/empty.
- Serialize (36-byte baseline): `m_nID, m_nStyle, m_iImage, m_strText`(CArchive UTF-16),
  `m_bUserButton, <reserved 0>, m_bText, m_bImage, m_bVisible`. 11 members are NOT
  serialized (confirmed by toggling). Byte-exact golden captured.
- Family sizes (for re-padding derived stubs so the ABI build stays exact):
  `CMFCToolBarMenuButton 296, CMFCToolBarComboBoxButton 336, CMFCToolBarEditBoxButton 176`.

## Export surface (30 exact `@CMFCToolBarButton@@` exports), classified
- **Already real (metadata campaigns):** `GetThisClass`, `GetRuntimeClass`. The descriptor
  must be upgraded DYNAMIC→SERIAL (schema 0x80000001) and given a real `CreateObject`.
- **Now implementable (real body) + headlessly verifiable:** both ctors, `Initialize`,
  `Serialize`, `CopyFrom`, `CompareWith`, `SetImage`, `ResetImageToDefault`, `CreateObject`,
  the statics (`m_cFormat`, `m_strClipboardFormatName`, `m_bWrapText`, `m_bUpdateImages`,
  `m_lstProtectedCommands`), `GetClipboardFormat`, `SetClipboardFormatName`,
  `Set/GetProtectedCommands`, `IsFirstInGroup`/`IsLastInGroup` (need parent ctx — partial).
- **Honest minimal (GUI/OLE, not the verification target):** `OnDraw`, `OnCalculateSize`,
  `OnDrawOnCustomizeList`, `FillInterior`, `OnChangeParentWnd`, `OnToolHitTest`,
  `PrepareDrag`, `CreateFromOleData`, `ExportToMenuButton`, `SetACCData`, `GetAccCount`.
  Keep faithful signatures/return-shape; do not fabricate pixel behavior.

## Implementation plan (per increment, merge-if-green, same pipeline as RTTI/msgmap)
1. Replace the `CMFCToolBarButton` stub in `include/openmfc/afxmfc.h` with the **exact**
   SDK member layout (offsets pinned by the layout probe; `DECLARE_SERIAL`). Re-pad the 3
   derived button stubs to their harvested sizes (296/336/176).
2. Implement the verifiable-subset method bodies in a new `phase4/src/` TU (real ctor field
   init from golden; Serialize field order from golden; CopyFrom/CompareWith; statics).
   Provide the real `CreateObject` factory now that a body exists.
3. **Verify (MSVC-ABI-targeted; mingw is only a fast cross-check, not the gate):**
   - `build_phase4.sh` → NO ABI REGRESSIONS / 14,109 exact / 0 GCC-mangled (MSVC export surface).
   - **Authoritative on-host gate — drop-in differential vs real `mfc140u`**: LoadLibrary
     our built `openmfc.dll` under wine and call the exported ctors/methods on a raw buffer,
     reading fields at the REAL MSVC offsets; output must match real `mfc140u` field-for-field
     (and byte-exact for Serialize). This proves the **MSVC** layout/behavior — a mingw
     `sizeof`/`offsetof` match alone does NOT (mingw can diverge from MSVC via tail-padding
     reuse), so the mingw probe is only a quick sanity cross-check.
   - Windows CI (compile + MSVC link + wine runtime) is the final authoritative gate.
4. PR, watch Windows CI, squash-merge, update memory.

## Vtable order (harvested — required before behavioral virtual methods)
Virtual dispatch from a real client uses real `mfc140u`'s slot indices, so to override
`Serialize`/`CopyFrom`/`CompareWith`/etc. our header must declare the full virtual list in
the SAME order, producing the same 53-slot vtable. Harvested + verified in
`cmfctoolbarbutton_vtable.json` (probe: `cmfctoolbarbutton_vtable_probe.cpp`): every
exported virtual's slot was found by exact-address match in the real vtable (by ordinal),
and all 17 land exactly where SDK-header declaration order predicts. Key slots:
`[0] GetRuntimeClass, [1] dtor, [2] Serialize, [3] AssertValid, [4] Dump, [5] PrepareDrag,
[7] CopyFrom, [8] OnDraw, [9] OnCalculateSize, [12] OnChangeParentWnd, [13] ExportToMenuButton,
[29] OnDrawOnCustomizeList, [33] OnToolHitTest, [38] ResetImageToDefault, [39] CompareWith,
[46] IsFirstInGroup, [47] IsLastInGroup, [48] SetACCData, [49] GetAccCount, [50] SetImage`
(total 53). CObject overrides (~dtor/Serialize/AssertValid/Dump) fold into base slots 1-4;
CMFCToolBarButton's new virtuals start at slot 5. Increment-2 impl declares all 48 own
virtuals in this order (trivial ones inline from the SDK header; exported ones real), then
verifies OUR DLL's vtable slot-for-slot against this map before relying on virtual dispatch.

## Verification bar (what "faithful" means here, concretely)
The golden differential — our DLL must produce the **same 36/46-byte Serialize stream**,
same default-ctor field values, and same CompareWith verdicts as real `mfc140u`. This is a
true drop-in behavior check, not a self-consistent stub.

## Risks / stop conditions
- Layout cascade: growing the base shifts derived offsets — re-pad each derived to its
  harvested `sizeof`; re-run the ABI safety suite (must stay 14,109 exact).
- The reserved Serialize DWORD at offset +4-after-bUserButton is always 0 for base buttons
  (no settable field drives it) → reproduce as literal 0; revisit if a derived class needs it.
- Derived button bodies (ComboBox/EditBox/Menu) are a **separate increment** — they embed
  live controls (CComboBox/CEdit/CMenu) and are only partly headless; harvest + scope each
  before implementing. Do not bundle with the base PR.
- Any CI red or ambiguous harvest → stop, surface, do not merge.
