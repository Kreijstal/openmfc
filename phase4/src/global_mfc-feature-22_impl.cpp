// CMFCRibbonBaseElement / CMFCRibbonButton exports implemented for the
// wave2_mfc-feature-22 batch (30 symbols: 13 button + 17 base element).
//
// Each body was decoded from the retail mfc140u export (ordinal -> RVA ->
// objdump).  The retail layout of both classes is much larger than the
// clean-room declarations in include/openmfc/afxmfc.h model:
//
//   CMFCRibbonBaseElement retail members used here include m_dwData (+0x110),
//   m_nID (+0x118), the tooltip/description strings (+0xa0/+0xa8/+0xb0), the
//   parent pointers (+0xe0/+0xf0), the "update tooltip info" flag (+0x174),
//   the tooltip-info-shortcut flag (+0x178), the popup pointer (+0x108) and
//   m_pOriginal (+0xe8).  OpenMFC declares only m_nID plus m_strText and
//   opaque padding, so everything except the ID is unreachable.  The small
//   ID-only functions (Find / FindByID / GetElementsByID / GetItemIDsList /
//   CanBeAddedToQuickAccessToolBar) are therefore transcribed faithfully
//   against GetID(), and everything else is a type-correct conservative
//   default with a TODO(clean-room) note.
//
//   CMFCRibbonButton retail members used here are entirely unmodeled: the
//   sub-item CArray (+0x248/+0x250/+0x258), the menu HMENU (+0x1d0) and menu
//   flags (+0x1f0/+0x21c/+0x1c4), the parent category/bar (+0xe0/+0xd8), the
//   m_pOriginal link (+0xe8), the geometry rects (+0x198/+0x1a8) and all the
//   mode flags (+0x144/+0x148/+0x13c/+0x154/+0x1f8/+0x1fc).  The clean-room
//   CMFCRibbonButton is a placeholder with only a 64-byte pad, so every
//   button mutator gets a type-correct no-op (or a NULL-checked safe default)
//   with a TODO(clean-room) note.  SetDescription is the exception: it only
//   forwards to the base element implementation (which OpenMFC keeps in the
//   ribbon_state side tables, see cbarcore.cpp), so it is implemented.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (scan of
// `// Symbol:` comments) drops the generated stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <cstdint>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Classes referenced by the exported signatures but not declared in the
// clean-room headers.  Only pointers/array references are used, so a forward
// declaration is sufficient.
class CMFCRibbonKeyTip;
class CMFCRibbonCommandsListBox;

// Defined in cbarcore.cpp (the ribbon_state side-table implementation of
// CMFCRibbonBaseElement::SetDescription); called by the button override below.
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(
    CMFCRibbonBaseElement* pThis, const wchar_t* lpszText);

//=============================================================================
// CMFCRibbonButton
//=============================================================================
// The button's retail members (sub-item CArray, menu handle/flags, parent
// links, geometry rects and mode flags) are not modeled by the clean-room
// declaration, so these bodies are conservative NULL-checked no-ops.  They
// keep the exports present and ABI-correct without corrupting state.

// CMFCRibbonButton::OnLButtonDown(CPoint) -- retail hit-tests the menu-area
// rects (+0x198/+0x1a8), reads the mode flags (+0x148/+0x13c/+0x1ec) and
// dispatches to the base OnLButtonDown / virtual IsDisabled (vtable+0x280)
// and OnShowPopupMenu (vtable+0x288).  None of these members or vtable slots
// exist in the clean-room declaration.
// TODO(clean-room): transcribed partially -- button geometry/menu state is
// not modeled.
// Symbol: ?OnLButtonDown@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, CPoint /*pt*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::OnLButtonUp(CPoint) -- retail reads the flag words
// (+0x144/+0x148/+0x13c/+0x154), the rect at +0x1a8, and dispatches through
// vtable+0x4d8.  None of these exist in the clean-room declaration.
// TODO(clean-room): transcribed partially -- button interaction state is not
// modeled.
// Symbol: ?OnLButtonUp@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, CPoint /*pt*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::OnMouseMove(CPoint) -- retail checks virtual IsDisabled
// (vtable+0x280), the ID range (+0x118), the rects at +0x198/+0x1a8 and the
// flags at +0x1f8/+0x1fc.  The ID is modeled but nothing else is, and the
// body largely drives mouse-over highlight state, which is not modeled.
// TODO(clean-room): transcribed partially -- button hover state is not
// modeled.
// Symbol: ?OnMouseMove@CMFCRibbonButton@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCRibbonButton__MEAAXVCPoint___Z(
    CMFCRibbonButton* pThis, CPoint /*pt*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::OnShowPopupMenu() -- retail (RVA 0xe7550, 0x500+ bytes)
// builds the whole popup menu from the button's sub-items, sizes the menu
// window, tracks it, and unwinds the drop-down state.  The sub-item array
// (+0x248/+0x250/+0x258), menu state and geometry are not modeled.
// TODO(clean-room): transcribed partially -- popup-menu construction is not
// modeled.
// Symbol: ?OnShowPopupMenu@CMFCRibbonButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonButton__MEAAXXZ(
    CMFCRibbonButton* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonButton::RemoveAllSubItems() -- retail walks the sub-item CArray
// (+0x248/+0x250/+0x258), deletes each sub-item via its scalar-deleting
// destructor and calls CArray::SetSize(0).  The sub-item array is not
// modeled.
// TODO(clean-room): transcribed partially -- sub-items are not modeled.
// Symbol: ?RemoveAllSubItems@CMFCRibbonButton@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllSubItems_CMFCRibbonButton__QEAAXXZ(
    CMFCRibbonButton* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonButton::RemoveSubItem(int) -- retail validates nIndex against the
// sub-item count (+0x258), deletes the item and erases it from the CArray,
// returning TRUE on success and FALSE for an out-of-range index.  The
// sub-item array is not modeled, so nothing can be removed; FALSE is the
// truthful result.
// TODO(clean-room): transcribed partially -- sub-items are not modeled.
// Symbol: ?RemoveSubItem@CMFCRibbonButton@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveSubItem_CMFCRibbonButton__QEAAHH_Z(
    CMFCRibbonButton* pThis, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}

// CMFCRibbonButton::SetACCData(CWnd*, CAccessibilityData&) -- retail first
// delegates to CMFCRibbonBaseElement::SetACCData (0x180011200, which clears
// the block), then sets m_nAccRole from a role-string lookup and m_bAccState
// from a virtual result (vtable+0x4b8).  The role/state computation and the
// virtual slot are not modeled; the retail function always returns TRUE.
// TODO(clean-room): transcribed partially -- accessibility role/state data is
// not modeled.
// Symbol: ?SetACCData@CMFCRibbonButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonButton* pThis, CWnd* /*pWndParent*/, CAccessibilityData& /*data*/)
{
    if (!pThis) return FALSE;
    return TRUE;
}

// CMFCRibbonButton::SetDescription(const wchar_t*) -- retail forwards to
// CMFCRibbonBaseElement::SetDescription and, when the flag at +0x210 is set,
// zeroes the +0x180/+0x188 pointers.  The base forwarding is implemented
// (ribbon_state side table); the flag/pointer members are not modeled.
// TODO(clean-room): transcribed partially -- the +0x180/+0x188 pointer reset
// behind the +0x210 flag is not modeled.
// Symbol: ?SetDescription@CMFCRibbonButton@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonButton__UEAAXPEB_W_Z(
    CMFCRibbonButton* pThis, const wchar_t* lpszText)
{
    if (!pThis) return;
    impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(pThis, lpszText);
}

// CMFCRibbonButton::SetMenu(UINT, int, int) -- retail loads the menu resource
// through the app resource handle, delegates to the HMENU overload
// (0x1800e52c0) and sets the "menu was set from resource" flag (+0x1f0).
// Menu/resource state is not modeled.
// TODO(clean-room): transcribed partially -- menu resource state is not
// modeled.
// Symbol: ?SetMenu@CMFCRibbonButton@@QEAAXIHH@Z
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXIHH_Z(
    CMFCRibbonButton* pThis, unsigned int /*uiMenuResID*/, int /*x*/, int /*y*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::SetMenu(HMENU, int, int) -- retail (RVA 0xe52c0, ~0x3a0
// bytes) resets +0x21c/+0x1c4, releases a previously owned menu (+0x1d0 when
// +0x1f0 set), stores the new HMENU at +0x1d0, enumerates the menu items
// (GetMenuItemCount/GetMenuItemID/GetSubMenu) into the sub-item array at
// +0x1a0 and rebuilds ghost-menu state.  None of this state is modeled.
// TODO(clean-room): transcribed partially -- menu/sub-item state is not
// modeled.
// Symbol: ?SetMenu@CMFCRibbonButton@@QEAAXPEAUHMENU__@@HH@Z
extern "C" void MS_ABI impl__SetMenu_CMFCRibbonButton__QEAAXPEAUHMENU____HH_Z(
    CMFCRibbonButton* pThis, HMENU /*hMenu*/, int /*x*/, int /*y*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::SetOriginal(CMFCRibbonBaseElement*) -- retail walks the
// pOriginal chain (+0xe8) to the last non-null link, stores it at +0xe8, and
// when the source is a CMFCRibbonButton clones its sub-items into this
// button.  The pOriginal member and sub-item array are not modeled.
// TODO(clean-room): transcribed partially -- the original-link chain and
// sub-items are not modeled.
// Symbol: ?SetOriginal@CMFCRibbonButton@@MEAAXPEAVCMFCRibbonBaseElement@@@Z
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonButton__MEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonButton* pThis, CMFCRibbonBaseElement* /*pOriginal*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::SetParentCategory(CMFCRibbonCategory*) -- retail stores
// the category at +0xe0 and propagates it to every sub-item (array
// +0x250/+0x258) through the virtual SetParentCategory (vtable+0x2d8), also
// setting each sub-item's flag at +0x160.  None of this state is modeled.
// TODO(clean-room): transcribed partially -- category/sub-item state is not
// modeled.
// Symbol: ?SetParentCategory@CMFCRibbonButton@@UEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonButton__UEAAXPEAVCMFCRibbonCategory___Z(
    CMFCRibbonButton* pThis, CMFCRibbonCategory* /*pCategory*/)
{
    if (!pThis) return;
}

// CMFCRibbonButton::SetParentRibbonBar(CMFCRibbonBar*) -- retail stores the
// bar at +0xd8 and propagates it to every sub-item (array +0x250/+0x258)
// through the virtual SetParentRibbonBar (vtable+0x278).  None of this state
// is modeled.
// TODO(clean-room): transcribed partially -- ribbon-bar/sub-item state is not
// modeled.
// Symbol: ?SetParentRibbonBar@CMFCRibbonButton@@MEAAXPEAVCMFCRibbonBar@@@Z
extern "C" void MS_ABI impl__SetParentRibbonBar_CMFCRibbonButton__MEAAXPEAVCMFCRibbonBar___Z(
    CMFCRibbonButton* pThis, CMFCRibbonBar* /*pBar*/)
{
    if (!pThis) return;
}

//=============================================================================
// CMFCRibbonBaseElement
//=============================================================================
// The base element's only modeled state is the command ID (GetID/SetID), so
// the ID-based queries below are transcribed faithfully; everything else is a
// type-correct conservative default.

// CMFCRibbonBaseElement::AddToKeyList(CArray<CMFCRibbonKeyTip*,
// CMFCRibbonKeyTip*>&) -- retail creates one or two CMFCRibbonKeyTip objects
// (0x18010e010, size 0x108, depending on a global "key tips enabled" flag at
// 0x1802c7b30) and appends them to the array.  CMFCRibbonKeyTip is not
// declared and key-tip state is not modeled.
// TODO(clean-room): transcribed partially -- key-tip objects are not modeled.
// Symbol: ?AddToKeyList@CMFCRibbonBaseElement@@UEAAXAEAV?$CArray@PEAVCMFCRibbonKeyTip@@PEAV1@@@@Z
extern "C" void MS_ABI impl__AddToKeyList_CMFCRibbonBaseElement__UEAAXAEAV__CArray_PEAVCMFCRibbonKeyTip__PEAV1____Z(
    CMFCRibbonBaseElement* pThis, CArray<CMFCRibbonKeyTip*, CMFCRibbonKeyTip*>& /*lstKeyTips*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::AddToListBox(CMFCRibbonCommandsListBox*, int) --
// retail checks the ID range (+0x118), iterates the list-box entries and
// calls virtuals on the box.  CMFCRibbonCommandsListBox is not declared and
// the box state is not modeled.
// TODO(clean-room): transcribed partially -- commands list-box state is not
// modeled.
// Symbol: ?AddToListBox@CMFCRibbonBaseElement@@UEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonBaseElement__UEAAHPEAVCMFCRibbonCommandsListBox__H_Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonCommandsListBox* /*pListBox*/, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}

// CMFCRibbonBaseElement::CanBeAddedToQuickAccessToolBar() const -- retail
// (RVA 0x11120) returns FALSE for the AFX internal command-ID ranges
// ([0xe120..0xf000], [1..0xe10f], [0xe210..0xe21f], >= 0xff00 signed) and for
// the IDs 0 / [0xe110..0xe11f] / [0xf001..0xf1ef], and additionally when the
// ID equals the command ID of a global object at 0x1803be3b0 (offset +0x40,
// an unmapped app-context global).  The ID-range logic is transcribed exactly
// from the retail compare/branch sequence; only the final global-object
// comparison is omitted because that global is not modeled.
// TODO(clean-room): transcribed partially -- the +0x1803be3b0 global
// app-context ID comparison is not modeled.
// Symbol: ?CanBeAddedToQuickAccessToolBar@CMFCRibbonBaseElement@@UEBAHXZ
extern "C" int MS_ABI impl__CanBeAddedToQuickAccessToolBar_CMFCRibbonBaseElement__UEBAHXZ(
    const CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return FALSE;

    const UINT id = pThis->GetID();

    // First stage: [0xe120..0xf000] and [1..0xe10f] jump straight to the
    // second stage; the remaining IDs below 0xf1f0 (including 0 and the
    // [0xe110..0xe11f] gap) return FALSE.
    const UINT a0 = id - 0xe120u;
    if (a0 > 0xedfu) {
        const UINT a1 = id - 1u;
        if (a1 > 0xe10eu) {
            const UINT a2 = id - 0xf1f0u;
            if (a2 > 0xffff0e0eu) {
                return FALSE;
            }
        }
    }

    // Second stage: >= 0xff00 (signed) and [0xe210..0xe21f] are internal
    // command IDs and are excluded.
    if (static_cast<int>(id) >= 0xff00) {
        return FALSE;
    }
    const UINT a3 = id - 0xe210u;
    if (a3 <= 0xfu) {
        return FALSE;
    }

    return TRUE;
}

// CMFCRibbonBaseElement::ClosePopupMenu() -- retail closes the popup tracked
// by the +0x108 pointer (virtual close chain) and zeroes +0x108/+0x170.
// The popup pointer and menu state are not modeled.
// TODO(clean-room): transcribed partially -- popup-menu state is not modeled.
// Symbol: ?ClosePopupMenu@CMFCRibbonBaseElement@@UEAAXXZ
extern "C" void MS_ABI impl__ClosePopupMenu_CMFCRibbonBaseElement__UEAAXXZ(
    CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::CopyFrom(const CMFCRibbonBaseElement&) -- retail
// copies m_nID (+0x118), m_dwData (+0x110), the scalar members at
// +0x124/+0x138/+0x15c/+0x160/+0x150/+0x164/+0x16c/+0x174/+0x178, three
// CStrings (+0xa0/+0xa8/+0xb0), two more CStrings (+0xb8/+0xc0) and the
// parent pointers (+0xe0/+0xf0).  Only m_nID is modeled, so only it can be
// copied.
// TODO(clean-room): transcribed partially -- the non-ID members are not
// modeled.
// Symbol: ?CopyFrom@CMFCRibbonBaseElement@@UEAAXAEBV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonBaseElement__UEAAXAEBV1__Z(
    CMFCRibbonBaseElement* pThis, const CMFCRibbonBaseElement& src)
{
    if (!pThis) return;
    pThis->SetID(src.GetID());
}

// CMFCRibbonBaseElement::EnableTooltipInfoShortcut(int) -- retail stores the
// value into the flag at +0x178.  The flag is not modeled.
// TODO(clean-room): transcribed partially -- the +0x178 tooltip-shortcut flag
// is not modeled.
// Symbol: ?EnableTooltipInfoShortcut@CMFCRibbonBaseElement@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableTooltipInfoShortcut_CMFCRibbonBaseElement__QEAAXH_Z(
    CMFCRibbonBaseElement* pThis, int /*bEnable*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::EnableUpdateTooltipInfo(int) -- retail, when the
// value differs from the flag at +0x174, stores it and calls the virtual
// OnUpdateToolTipInfo (vtable+0x368) if enabled.  The flag and the vtable
// slot are not modeled.
// TODO(clean-room): transcribed partially -- the +0x174 flag and its
// notification virtual are not modeled.
// Symbol: ?EnableUpdateTooltipInfo@CMFCRibbonBaseElement@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableUpdateTooltipInfo_CMFCRibbonBaseElement__QEAAXH_Z(
    CMFCRibbonBaseElement* pThis, int /*bEnable*/)
{
    if (!pThis) return;
}

// CMFCRibbonBaseElement::Find(const CMFCRibbonBaseElement*) -- retail:
// return this if the argument equals `this`, else nullptr.
// Symbol: ?Find@CMFCRibbonBaseElement@@UEAAPEAV1@PEBV1@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__Find_CMFCRibbonBaseElement__UEAAPEAV1_PEBV1__Z(
    CMFCRibbonBaseElement* pThis, const CMFCRibbonBaseElement* pElement)
{
    if (!pThis) return nullptr;
    return (pThis == pElement) ? pThis : nullptr;
}

// CMFCRibbonBaseElement::FindByData(unsigned __int64) -- retail compares the
// argument with m_dwData (+0x110) and returns this on equality.  m_dwData is
// not modeled, so no element can ever match.
// TODO(clean-room): transcribed partially -- m_dwData is not modeled.
// Symbol: ?FindByData@CMFCRibbonBaseElement@@UEAAPEAV1@_K@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByData_CMFCRibbonBaseElement__UEAAPEAV1__K_Z(
    CMFCRibbonBaseElement* pThis, unsigned long long /*dwData*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonBaseElement::FindByID(unsigned int) -- retail compares the
// argument with m_nID (+0x118) and returns this on equality.  m_nID is the
// modeled command ID, so this is transcribed faithfully.
// Symbol: ?FindByID@CMFCRibbonBaseElement@@UEAAPEAV1@I@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByID_CMFCRibbonBaseElement__UEAAPEAV1_I_Z(
    CMFCRibbonBaseElement* pThis, unsigned int nID)
{
    if (!pThis) return nullptr;
    return (pThis->GetID() == nID) ? pThis : nullptr;
}

// CMFCRibbonBaseElement::FindByOriginal(CMFCRibbonBaseElement*) -- retail
// compares the argument with m_pOriginal (+0xe8) and returns this on
// equality.  m_pOriginal is not modeled.
// TODO(clean-room): transcribed partially -- m_pOriginal is not modeled.
// Symbol: ?FindByOriginal@CMFCRibbonBaseElement@@UEAAPEAV1@PEAV1@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__FindByOriginal_CMFCRibbonBaseElement__UEAAPEAV1_PEAV1__Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonBaseElement* /*pOriginal*/)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonBaseElement::GetDropDownImageWidth() const -- retail lazily
// initializes a global "drop-down image width" integer (0x18008f600) and
// returns its value.  The global is not modeled, so the default width is not
// reachable; 0 is the safe empty value.
// TODO(clean-room): transcribed partially -- the global drop-down image width
// cache is not modeled.
// Symbol: ?GetDropDownImageWidth@CMFCRibbonBaseElement@@MEBAHXZ
extern "C" int MS_ABI impl__GetDropDownImageWidth_CMFCRibbonBaseElement__MEBAHXZ(
    const CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return 0;
    return 0;
}

// CMFCRibbonBaseElement::GetDroppedDown() -- retail calls the virtual
// IsDroppedDown (vtable+0x1c0) and returns this when it is non-zero.  The
// vtable slot is not modeled.
// TODO(clean-room): transcribed partially -- drop-down state is not modeled.
// Symbol: ?GetDroppedDown@CMFCRibbonBaseElement@@UEAAPEAV1@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetDroppedDown_CMFCRibbonBaseElement__UEAAPEAV1_XZ(
    CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonBaseElement::GetElementsByID(unsigned int,
// CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>&) -- retail appends
// `this` to the array when m_nID (+0x118) equals uiCmd (CArray::SetSize(size+1)
// then store at pData[size], which is exactly CArray::Add).  Transcribed
// faithfully against the modeled command ID.
// Symbol: ?GetElementsByID@CMFCRibbonBaseElement@@UEAAXIAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetElementsByID_CMFCRibbonBaseElement__UEAAXIAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBaseElement* pThis, unsigned int uiCmd,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& arElements)
{
    if (!pThis) return;
    if (pThis->GetID() == uiCmd) {
        arElements.Add(pThis);
    }
}

// CMFCRibbonBaseElement::GetFocused() -- retail calls the virtual IsFocused
// (vtable+0x1b8) and returns this when focused.  The vtable slot is not
// modeled.
// TODO(clean-room): transcribed partially -- focus state is not modeled.
// Symbol: ?GetFocused@CMFCRibbonBaseElement@@UEAAPEAV1@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetFocused_CMFCRibbonBaseElement__UEAAPEAV1_XZ(
    CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonBaseElement::GetHighlighted() -- retail calls the virtual
// IsHighlighted (vtable+0x1b0) and returns this when highlighted.  The vtable
// slot is not modeled.
// TODO(clean-room): transcribed partially -- highlight state is not modeled.
// Symbol: ?GetHighlighted@CMFCRibbonBaseElement@@UEAAPEAV1@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetHighlighted_CMFCRibbonBaseElement__UEAAPEAV1_XZ(
    CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return nullptr;
    return nullptr;
}

// CMFCRibbonBaseElement::GetItemIDsList(CList<UINT, UINT>&) const -- retail
// adds m_nID (+0x118) to the list when it passes the range guard (skip only
// id == 0 and id == 0xffffffff) and is not already present (CList::Find, then
// AddTail).  Transcribed faithfully against the modeled command ID.
// Symbol: ?GetItemIDsList@CMFCRibbonBaseElement@@MEBAXAEAV?$CList@II@@@Z
extern "C" void MS_ABI impl__GetItemIDsList_CMFCRibbonBaseElement__MEBAXAEAV__CList_II___Z(
    const CMFCRibbonBaseElement* pThis, CList<unsigned int, unsigned int>& lstIDs)
{
    if (!pThis) return;

    const UINT id = pThis->GetID();

    // Retail guard: skip when (id - 1) unsigned > 0xfffffffd, i.e. only for
    // id == 0 and id == 0xffffffff.
    const UINT guard = id - 1u;
    if (guard > 0xfffffffd) {
        return;
    }

    CList<unsigned int, unsigned int>::POSITION pos = lstIDs.Find(id);
    if (pos.pNode == nullptr) {
        lstIDs.AddTail(id);
    }
}
