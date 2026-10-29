// CMFCRibbonBaseElement — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-22_impl.cpp, global_mfc-feature-23_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonBaseElementSupport.h"
#include "detail/MfcFeature23ImplSupport.h"
#include "detail/MfccoreSupport.h"

class CMFCRibbonQuickAccessToolBar;
class CMFCRibbonCmdUI;

extern "C" void MS_ABI impl__Add_CMFCRibbonQuickAccessToolBar__IEAAXPEAVCMFCRibbonBaseElement___Z(
    CMFCRibbonQuickAccessToolBar* pThis, CMFCRibbonBaseElement* pElement);

// Symbol: ?SetToolTipText@CMFCRibbonBaseElement@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetToolTipText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(CMFCRibbonBaseElement* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonToolTips[RibbonElementID(pThis)] = (lpszText != nullptr) ? lpszText : L"";
}
// Symbol: ?GetToolTipText@CMFCRibbonBaseElement@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetToolTipText_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonBaseElement* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonToolTips.find(RibbonElementID(pThis));
    if (it != g_ribbonToolTips.end()) {
        BuildCStringResult(pRet, it->second);
        return;
    }
    CString text = pThis->GetText();
    BuildCStringResult(pRet, (const wchar_t*)text);
}
// Symbol: ?SetDescription@CMFCRibbonBaseElement@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetDescription_CMFCRibbonBaseElement__UEAAXPEB_W_Z(CMFCRibbonBaseElement* pThis, const wchar_t* lpszText) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    g_ribbonDescriptions[RibbonElementID(pThis)] = (lpszText != nullptr) ? lpszText : L"";
}
// Symbol: ?GetDescription@CMFCRibbonBaseElement@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDescription_CMFCRibbonBaseElement__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pRet, const CMFCRibbonBaseElement* pThis) {
    if (!pThis) {
        BuildCStringResult(pRet, L"");
        return;
    }
    std::lock_guard<std::mutex> lock(g_ribbonMutex);
    auto it = g_ribbonDescriptions.find(RibbonElementID(pThis));
    if (it != g_ribbonDescriptions.end()) {
        BuildCStringResult(pRet, it->second);
        return;
    }
    CString text = pThis->GetText();
    BuildCStringResult(pRet, (const wchar_t*)text);
}
// CMFCRibbonBaseElement::AddToKeyList(CArray<CMFCRibbonKeyTip*,
// CMFCRibbonKeyTip*>&) -- retail RVA 0x119b0 (mfc140u) attempts to allocate
// 0x108 bytes, constructs a CMFCRibbonKeyTip (constructor RVA 0x10e010,
// mfc140u) on success, and appends the resulting pointer. If the key-tip string
// at +0xb0 is nonempty and the virtual at vtable+0x280 returns true, it also
// attempts to allocate, construct, and append a second key tip. The type/state
// are not modeled.
// TODO(clean-room): transcribed partially -- key-tip objects are not modeled.
// Symbol: ?AddToKeyList@CMFCRibbonBaseElement@@UEAAXAEAV?$CArray@PEAVCMFCRibbonKeyTip@@PEAV1@@@@Z
extern "C" void MS_ABI impl__AddToKeyList_CMFCRibbonBaseElement__UEAAXAEAV__CArray_PEAVCMFCRibbonKeyTip__PEAV1____Z(
    CMFCRibbonBaseElement* pThis, CArray<CMFCRibbonKeyTip*, CMFCRibbonKeyTip*>& /*lstKeyTips*/)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::AddToListBox(CMFCRibbonCommandsListBox_MfcFeature22Impl*, int) --
// retail checks the ID range (+0x118), iterates the list-box entries and
// calls virtuals on the box.  CMFCRibbonCommandsListBox_MfcFeature22Impl is not declared and
// the box state is not modeled.
// TODO(clean-room): transcribed partially -- commands list-box state is not
// modeled.
// Symbol: ?AddToListBox@CMFCRibbonBaseElement@@UEAAHPEAVCMFCRibbonCommandsListBox@@H@Z
extern "C" int MS_ABI impl__AddToListBox_CMFCRibbonBaseElement__UEAAHPEAVCMFCRibbonCommandsListBox__H_Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonCommandsListBox_MfcFeature22Impl* /*pListBox*/, int /*nIndex*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// CMFCRibbonBaseElement::CanBeAddedToQuickAccessToolBar() const -- retail
// RVA 0x11120 (mfc140u) admits IDs in [1..0xe10f], [0xe120..0xefff], and
// [0xf1f0..0xffffffff] through its first range gate, then rejects signed IDs
// >= 0xff00 and [0xe210..0xe21f]. Thus 0, [0xe110..0xe11f], and
// [0xf000..0xf1ef] fail the range gate. Retail also rejects an ID matching
// the +0x40 field of the global pointer at RVA 0x3be3b0 (mfc140u). The range
// logic below matches the compare/branch sequence; that global comparison is
// omitted because its object is not modeled.
// TODO(clean-room): transcribed partially -- the global-object ID comparison
// at RVA 0x3be3b0 (mfc140u) is not modeled.
// Symbol: ?CanBeAddedToQuickAccessToolBar@CMFCRibbonBaseElement@@UEBAHXZ
extern "C" int MS_ABI impl__CanBeAddedToQuickAccessToolBar_CMFCRibbonBaseElement__UEBAHXZ(
    const CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return FALSE;

    const UINT id = pThis->GetID();

    // First stage admits [0xe120..0xefff], [1..0xe10f], and
    // [0xf1f0..0xffffffff]. Other IDs (including 0 and [0xe110..0xe11f]) fail.
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
// copies m_nID (+0x118), m_dwData (+0x110), scalar members at
// +0x124/+0x138/+0x15c/+0x160/+0x150/+0x164/+0x16c/+0x174/+0x178, three
// CStrings (+0xa0/+0xa8/+0xb0), two more CStrings (+0xb8/+0xc0), and parent
// pointers (+0xe0/+0xf0). This placeholder copies only the modeled command ID;
// OpenMFC also has m_strText, but its correspondence to these retail strings
// is not established here.
// TODO(clean-room): remaining retail state and string mapping are not modeled.
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
// CMFCRibbonBaseElement::GetDropDownImageWidth() const -- retail RVA 0x11a90
// (mfc140u) calls the internal helper at RVA 0x8f600 (mfc140u) with a CSize
// output buffer and returns its cx field. The helper uses internal global and
// settings state that this implementation does not model.
// TODO(clean-room): the helper's global/settings state is not modeled.
// Symbol: ?GetDropDownImageWidth@CMFCRibbonBaseElement@@MEBAHXZ
extern "C" int MS_ABI impl__GetDropDownImageWidth_CMFCRibbonBaseElement__MEBAHXZ(
    const CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCRibbonBaseElement::GetDroppedDown() -- retail calls IsDroppedDown at
// vtable byte offset +0x1d8 and returns this when it is non-zero. The slot is
// not modeled.
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
// CMFCRibbonBaseElement::SetKeys(const wchar_t*, const wchar_t*) -- retail
// measures both strings (wcslen) and stores them into the key CStrings at
// +0xa8/+0xb0.  Neither string member is modeled.
// TODO(clean-room): transcribed partially -- the key-tip CStrings are not
// modeled.
// Symbol: ?SetKeys@CMFCRibbonBaseElement@@UEAAXPEB_W0@Z
extern "C" void MS_ABI impl__SetKeys_CMFCRibbonBaseElement__UEAAXPEB_W0_Z(
    CMFCRibbonBaseElement* pThis, const wchar_t* /*lpszKeys*/, const wchar_t* /*lpszKeysExt*/)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::SetOriginal(CMFCRibbonBaseElement*) -- retail walks
// the m_pOriginal chain (+0xe8) to the last non-null link and stores it into
// +0xe8.  The original-link member is not modeled.
// TODO(clean-room): transcribed partially -- m_pOriginal is not modeled.
// Symbol: ?SetOriginal@CMFCRibbonBaseElement@@UEAAXPEAV1@@Z
extern "C" void MS_ABI impl__SetOriginal_CMFCRibbonBaseElement__UEAAXPEAV1__Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonBaseElement* /*pOriginal*/)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::SetParentCategory(CMFCRibbonCategory*) -- retail
// stores the category pointer at +0xe0.  The parent-category member is not
// modeled.
// TODO(clean-room): transcribed partially -- the parent category is not
// modeled.
// Symbol: ?SetParentCategory@CMFCRibbonBaseElement@@UEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__SetParentCategory_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonCategory___Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonCategory* /*pCategory*/)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::SetParentMenu(CMFCRibbonPanelMenuBar*) -- retail
// stores the panel-menu-bar pointer at +0xf8.  The parent-menu member is not
// modeled.
// TODO(clean-room): transcribed partially -- the parent panel menu bar is not
// modeled.
// Symbol: ?SetParentMenu@CMFCRibbonBaseElement@@UEAAXPEAVCMFCRibbonPanelMenuBar@@@Z
extern "C" void MS_ABI impl__SetParentMenu_CMFCRibbonBaseElement__UEAAXPEAVCMFCRibbonPanelMenuBar___Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonPanelMenuBar* /*pParentMenu*/)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::SetTextAlwaysOnRight(int) -- retail stores the
// argument into the +0x124 flag.  The flag is not modeled.
// TODO(clean-room): transcribed partially -- the +0x124 text-alignment flag
// is not modeled.
// Symbol: ?SetTextAlwaysOnRight@CMFCRibbonBaseElement@@UEAAXH@Z
extern "C" void MS_ABI impl__SetTextAlwaysOnRight_CMFCRibbonBaseElement__UEAAXH_Z(
    CMFCRibbonBaseElement* pThis, int /*bSet*/)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::StretchToWholeRow(CDC*, int) -- retail calls the
// virtual IsStretchToWholeRow (vtable+0x320), rejects when the flags at
// +0x12c/+0x130 are set, and on success adds the argument to the rect edge at
// +0xcc and stores the result into +0xd4, returning TRUE.  None of that state
// or the virtual slot is modeled.
// TODO(clean-room): transcribed partially -- element geometry and stretch
// state are not modeled.
// Symbol: ?StretchToWholeRow@CMFCRibbonBaseElement@@UEAAHPEAVCDC@@H@Z
extern "C" int MS_ABI impl__StretchToWholeRow_CMFCRibbonBaseElement__UEAAHPEAVCDC__H_Z(
    CMFCRibbonBaseElement* pThis, CDC* /*pDC*/, int /*nHighlighted*/)
{
    if (!pThis) return FALSE;
    return FALSE;
}
// CMFCRibbonBaseElement::UpdateTooltipInfo() -- retail, when the flag at
// +0x174 is set and the ID at +0x118 passes the command range guard, looks
// the tooltip up in the global tooltip-manager and rewrites the description
// CStrings at +0xb8/+0xc0.  The flag, the manager and the strings are not
// modeled.
// TODO(clean-room): transcribed partially -- tooltip state is not modeled.
// Symbol: ?UpdateTooltipInfo@CMFCRibbonBaseElement@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateTooltipInfo_CMFCRibbonBaseElement__UEAAXXZ(
    CMFCRibbonBaseElement* pThis)
{
    if (!pThis) return;
}
// CMFCRibbonBaseElement::accDoDefaultAction(VARIANT) -- retail invokes the
// virtual at vtable+0x2c8 (result discarded) and always returns S_OK.  The
// virtual is not modeled; S_OK is the retail return value on every path.
// Symbol: ?accDoDefaultAction@CMFCRibbonBaseElement@@UEAAJUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accDoDefaultAction_CMFCRibbonBaseElement__UEAAJUtagVARIANT___Z(
    CMFCRibbonBaseElement* pThis, VARIANT /*varChild*/)
{
    if (!pThis) return S_OK;
    return S_OK;
}
// CMFCRibbonBaseElement::accHitTest(long, long, VARIANT*) -- retail rejects a
// NULL output with E_INVALIDARG, then hit-tests through the parent bar at
// +0xd8 and its accessibility object (+0x40); without a parent it leaves the
// output VARIANT untouched and returns S_OK.  The parent state is not
// modeled, so the no-parent path is taken.
// TODO(clean-room): transcribed partially -- the parent-bar accessibility
// object is not modeled.
// Symbol: ?accHitTest@CMFCRibbonBaseElement@@UEAAJJJPEAUtagVARIANT@@@Z
extern "C" long MS_ABI impl__accHitTest_CMFCRibbonBaseElement__UEAAJJJPEAUtagVARIANT___Z(
    CMFCRibbonBaseElement* pThis, long /*x*/, long /*y*/, VARIANT* pVarOut)
{
    if (!pThis) return S_OK;
    if (!pVarOut) return E_INVALIDARG;
    return S_OK;
}
// CMFCRibbonBaseElement::accLocation(long*, long*, long*, long*, VARIANT) --
// retail rejects NULL output pointers with E_INVALIDARG and returns S_FALSE
// for non-VT_I4 children and negative child IDs. CHILDID_SELF uses the rect
// at +0xc8 and may transform it through the parent accessibility object;
// positive child IDs use a virtual child-rect path (vtable+0x160). This
// placeholder returns S_FALSE for every non-self child and zeroes the self
// outputs because neither geometry nor accessibility state is modeled.
// TODO(clean-room): child IDs and the element/parent accessibility geometry
// paths are not modeled; the child-ID behavior above is intentionally absent.
// Symbol: ?accLocation@CMFCRibbonBaseElement@@UEAAJPEAJ000UtagVARIANT@@@Z
extern "C" long MS_ABI impl__accLocation_CMFCRibbonBaseElement__UEAAJPEAJ000UtagVARIANT___Z(
    CMFCRibbonBaseElement* pThis, long* pxLeft, long* pyTop, long* pcxWidth,
    long* pcyHeight, VARIANT varChild)
{
    if (!pThis) return E_INVALIDARG;
    if (!pxLeft || !pyTop || !pcxWidth || !pcyHeight) return E_INVALIDARG;
    if (varChild.vt != 3 /*VT_I4*/) return S_FALSE;
    if (varChild.lVal != 0 /*CHILDID_SELF*/) return S_FALSE;
    *pxLeft = 0;
    *pyTop = 0;
    *pcxWidth = 0;
    *pcyHeight = 0;
    return S_OK;
}
// CMFCRibbonBaseElement::accNavigate(long, VARIANT, VARIANT*) -- retail
// returns S_FALSE when the accessibility-mode flag at +0x17c is clear;
// otherwise it validates the start VARIANT and walks parent panel-menu-bar
// navigation (+0xf0). The local placeholder always returns S_FALSE after its
// own pointer checks; the unmodeled flag is not known to be clear.
// TODO(clean-room): the enabled accessibility-navigation path is not modeled.
// Symbol: ?accNavigate@CMFCRibbonBaseElement@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonBaseElement__UEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonBaseElement* pThis, long /*navDir*/, VARIANT /*varStart*/, VARIANT* pvarEnd)
{
    if (!pThis) return S_FALSE;
    if (!pvarEnd) return E_INVALIDARG;
    return S_FALSE;
}
// CMFCRibbonBaseElement::get_accParent(IDispatch**) -- retail rejects a NULL
// output with E_INVALIDARG, clears the output, and returns S_FALSE when the
// accessibility-mode flag at +0x17c is clear. When set it resolves the parent
// bar's (+0xd8) accessibility object. The placeholder always takes the
// disabled-mode fallback; the unmodeled flag is not known to be clear.
// TODO(clean-room): the enabled parent-bar accessibility path is not modeled.
// Symbol: ?get_accParent@CMFCRibbonBaseElement@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__get_accParent_CMFCRibbonBaseElement__UEAAJPEAPEAUIDispatch___Z(
    CMFCRibbonBaseElement* pThis, IDispatch** ppdispParent)
{
    if (!pThis) return E_INVALIDARG;
    if (!ppdispParent) return E_INVALIDARG;
    *ppdispParent = nullptr;
    return S_FALSE;
}
CMFCRibbonBaseElement::CMFCRibbonBaseElement() : m_nID(0) {
    memset(_ribbonbaseelement_padding, 0, sizeof(_ribbonbaseelement_padding));
}
CMFCRibbonBaseElement::~CMFCRibbonBaseElement() {}
UINT CMFCRibbonBaseElement::GetID() const { return m_nID; }
void CMFCRibbonBaseElement::SetID(UINT nID) { m_nID = nID; }
CString CMFCRibbonBaseElement::GetText() const { return m_strText; }
void CMFCRibbonBaseElement::SetText(const wchar_t* lpszText) { if(lpszText) m_strText = lpszText; }
// Retail RVA 0x11690 (mfc140u) reads the key-tip strings at +0xa0/+0xa8,
// checks state at +0x138/+0xf8, then measures text through the supplied CDC.
// The class declaration does not model those fields. For this aggregate return,
// MSVC x64 passes the hidden CSize* after this in RDX; initialize a safe result.
// Symbol: ?GetKeyTipSize@CMFCRibbonBaseElement@@UEAA?AVCSize@@PEAVCDC@@@Z
extern "C" CSize* MS_ABI impl__GetKeyTipSize_CMFCRibbonBaseElement__UEAA_AVCSize__PEAVCDC___Z(
    CMFCRibbonBaseElement* /*pThis*/, CSize* pRet, CDC* /*pDC*/)
{
    if (pRet) {
        pRet->cx = 0;
        pRet->cy = 0;
    }
    return pRet;
}

// Retail RVA 0x104d0 (mfc140u) follows +0xf8 / +0xe0 parent links and reads
// nested panel fields at +0x1560/+0x1580; those fields are not modeled here.
// Symbol: ?GetParentPanel@CMFCRibbonBaseElement@@UEBAPEAVCMFCRibbonPanel@@XZ
extern "C" CMFCRibbonPanel* MS_ABI impl__GetParentPanel_CMFCRibbonBaseElement__UEBAPEAVCMFCRibbonPanel__XZ(
    const CMFCRibbonBaseElement* /*pThis*/)
{
    return nullptr;
}

// Retail RVA 0x104a0 (mfc140u) reads parent pointers at +0xd8/+0xf8/+0xe0,
// including the parent object's +0x760 field; this layout is not modeled.
// Symbol: ?GetParentWnd@CMFCRibbonBaseElement@@UEBAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetParentWnd_CMFCRibbonBaseElement__UEBAPEAVCWnd__XZ(
    const CMFCRibbonBaseElement* /*pThis*/)
{
    return nullptr;
}

// Retail RVA 0x10280 (mfc140u) calls the virtual at vtable byte offset 0x1c0
// and returns this only when it reports true; that slot is absent from the model.
// Symbol: ?GetPressed@CMFCRibbonBaseElement@@UEAAPEAV1@XZ
extern "C" CMFCRibbonBaseElement* MS_ABI impl__GetPressed_CMFCRibbonBaseElement__UEAAPEAV1_XZ(
    CMFCRibbonBaseElement* /*pThis*/)
{
    return nullptr;
}

// Retail RVA 0x10140 (mfc140u) traverses the +0xd8/+0xe0/+0xf8 parent state;
// those pointers and the panel-menu traversal state are not modeled.
// Symbol: ?GetTopLevelRibbonBar@CMFCRibbonBaseElement@@QEBAPEAVCMFCRibbonBar@@XZ
extern "C" CMFCRibbonBar* MS_ABI impl__GetTopLevelRibbonBar_CMFCRibbonBaseElement__QEBAPEAVCMFCRibbonBar__XZ(
    const CMFCRibbonBaseElement* /*pThis*/)
{
    return nullptr;
}

// Retail RVA 0x11b70 (mfc140u) appends this to the array only if the CRect at
// +0xc8 is nonempty; the rectangle is not present in the declared class.
// Symbol: ?GetVisibleElements@CMFCRibbonBaseElement@@UEAAXAEAV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@@Z
extern "C" void MS_ABI impl__GetVisibleElements_CMFCRibbonBaseElement__UEAAXAEAV__CArray_PEAVCMFCRibbonBaseElement__PEAV1____Z(
    CMFCRibbonBaseElement* /*pThis*/,
    CArray<CMFCRibbonBaseElement*, CMFCRibbonBaseElement*>& /*arElements*/)
{
}

// The ANSI twin's body at RVA 0x10190 (mfc140) is `mov rcx, rax; ret`;
// the retail implementation simply returns this, independent of the CPoint.
// Symbol: ?HitTest@CMFCRibbonBaseElement@@UEAAPEAV1@VCPoint@@@Z
extern "C" CMFCRibbonBaseElement* MS_ABI impl__HitTest_CMFCRibbonBaseElement__UEAAPEAV1_VCPoint___Z(
    CMFCRibbonBaseElement* pThis, long long /*point*/)
{
    return pThis;
}

// Retail RVA 0x10520 (mfc140u) follows +0xf8 to +0x1560 and returns that
// object's +0xb8 mode value; this parent-menu state is not modeled.
// Symbol: ?IsMenuMode@CMFCRibbonBaseElement@@QEBAHXZ
extern "C" int MS_ABI impl__IsMenuMode_CMFCRibbonBaseElement__QEBAHXZ(
    const CMFCRibbonBaseElement* /*pThis*/)
{
    return 0;
}

// Retail RVA 0x10ab0 (mfc140u) walks +0xe8 original links, tests several
// flags, and notifies a parent window; those fields and the callback path are
// not modeled, so callers cannot be forwarded faithfully.
// Symbol: ?NotifyCommand@CMFCRibbonBaseElement@@QEAAHH@Z
extern "C" int MS_ABI impl__NotifyCommand_CMFCRibbonBaseElement__QEAAHH_Z(
    CMFCRibbonBaseElement* /*pThis*/, int /*bWithDelay*/)
{
    return 0;
}

// The exact decorated symbol was not resolved in either the mfc140u or ANSI
// symbol map, so its body could not be verified and this remains a stub.
// Symbol: ?NotifyControlCommand@CMFCRibbonBaseElement@@MEAAHHH_K_J@Z
extern "C" int MS_ABI impl__NotifyControlCommand_CMFCRibbonBaseElement__MEAAHHH_K_J_Z(
    CMFCRibbonBaseElement* /*pThis*/, int /*bAccelerator*/, int /*nNotifyCode*/,
    WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return 0;
}

// Retail RVA 0x11ab0 (mfc140u) obtains the top-level ribbon bar and sends a
// highlight notification through it; parent/bar notification state is absent.
// Symbol: ?NotifyHighlightListItem@CMFCRibbonBaseElement@@UEAAXH@Z
extern "C" void MS_ABI impl__NotifyHighlightListItem_CMFCRibbonBaseElement__UEAAXH_Z(
    CMFCRibbonBaseElement* /*pThis*/, int /*nIndex*/)
{
}

// Retail RVA 0x115e0 (mfc140u) tail-jumps to NotifyCommand(this, 1). The
// NotifyCommand path above depends on unmodeled ribbon state, so this remains
// a stub rather than forwarding into another stub.
// Symbol: ?OnAccDefaultAction@CMFCRibbonBaseElement@@UEAAXXZ
extern "C" void MS_ABI impl__OnAccDefaultAction_CMFCRibbonBaseElement__UEAAXXZ(
    CMFCRibbonBaseElement* /*pThis*/)
{
}

// Retail RVA 0x11180 (mfc140u) calls CMFCRibbonQuickAccessToolBar::Add at
// RVA 0x122fe0 (mfc140u), then returns TRUE. OpenMFC's Add thunk accepts both
// pointer arguments but is itself a no-op placeholder, so the call is retained
// while the actual QAT insertion remains unavailable.
// Symbol: ?OnAddToQAToolbar@CMFCRibbonBaseElement@@UEAAHAEAVCMFCRibbonQuickAccessToolBar@@@Z
extern "C" int MS_ABI impl__OnAddToQAToolbar_CMFCRibbonBaseElement__UEAAHAEAVCMFCRibbonQuickAccessToolBar___Z(
    CMFCRibbonBaseElement* pThis, CMFCRibbonQuickAccessToolBar& qat)
{
    impl__Add_CMFCRibbonQuickAccessToolBar__IEAAXPEAVCMFCRibbonBaseElement___Z(&qat, pThis);
    return TRUE;
}

// Retail RVA 0x10a80 (mfc140u) calls the +0x368 virtual only when the
// CString at +0xb8 is empty; neither that field nor the virtual slot is modeled.
// Symbol: ?OnAfterChangeRect@CMFCRibbonBaseElement@@UEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnAfterChangeRect_CMFCRibbonBaseElement__UEAAXPEAVCDC___Z(
    CMFCRibbonBaseElement* /*pThis*/, CDC* /*pDC*/)
{
}

// Retail RVA 0x115f0 (mfc140u) reads the key-tip CString at +0xa8 and builds
// drawing state from it; the key-tip state and its virtual drawing path are absent.
// Symbol: ?OnDrawKeyTip@CMFCRibbonBaseElement@@UEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__OnDrawKeyTip_CMFCRibbonBaseElement__UEAAXPEAVCDC__AEBVCRect__H_Z(
    CMFCRibbonBaseElement* /*pThis*/, CDC* /*pDC*/, const CRect& /*rect*/,
    int /*bIsMenu*/)
{
}

// Retail RVA 0x10f60 (mfc140u) uses element flags at +0x148/+0x168, the
// supplied rectangle, text and CDC drawing calls; those members are not modeled.
// Symbol: ?OnDrawOnList@CMFCRibbonBaseElement@@UEAAXPEAVCDC@@V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HVCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawOnList_CMFCRibbonBaseElement__UEAAXPEAVCDC__V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HVCRect__HH_Z(
    CMFCRibbonBaseElement* /*pThis*/, CDC* /*pDC*/, CString /*strText*/,
    int /*nTextOffset*/, CRect /*rect*/, int /*bIsSelected*/,
    int /*bHighlighted*/)
{
}

// The ANSI twin's body at RVA 0x11870 (mfc140) tests the rect at +0xc8,
// state at +0x148, and then follows ribbon/menu virtual callback paths. That
// state is not modeled, so this remains a stub.
// Symbol: ?OnKey@CMFCRibbonBaseElement@@UEAAHH@Z
extern "C" int MS_ABI impl__OnKey_CMFCRibbonBaseElement__UEAAHH_Z(
    CMFCRibbonBaseElement* /*pThis*/, int /*bIsMenuKey*/)
{
    return 0;
}

// The ANSI twin's body at RVA 0x10060 (mfc140) follows the +0xf8/+0xd8/+0xe0
// parent links and invokes virtual callbacks. Those links and slots are not
// modeled. CPoint is represented by its 8-byte MS_ABI register value.
// Symbol: ?OnLButtonDown@CMFCRibbonBaseElement@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCRibbonBaseElement__MEAAXVCPoint___Z(
    CMFCRibbonBaseElement* /*pThis*/, long long /*point*/)
{
}

// The ANSI twin's body at RVA 0x11b30 (mfc140) obtains the top-level bar,
// checks +0x170, sets it, then posts a parent notification. The required ribbon
// and popup state is not modeled, so this remains a stub.
// Symbol: ?OnShowPopupMenu@CMFCRibbonBaseElement@@UEAAXXZ
extern "C" void MS_ABI impl__OnShowPopupMenu_CMFCRibbonBaseElement__UEAAXXZ(
    CMFCRibbonBaseElement* /*pThis*/)
{
}

// The ANSI twin's body at RVA 0x10120 (mfc140) reads the retail ID at +0x118,
// populates CMFCRibbonCmdUI, and invokes CCmdUI::DoUpdate. OpenMFC models the
// CCmdUI prefix, but the placeholder element vtable/state cannot safely service
// the virtual callbacks that DoUpdate may trigger, so this remains a stub.
// Symbol: ?OnUpdateCmdUI@CMFCRibbonBaseElement@@MEAAXPEAVCMFCRibbonCmdUI@@PEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCRibbonBaseElement__MEAAXPEAVCMFCRibbonCmdUI__PEAVCFrameWnd__H_Z(
    CMFCRibbonBaseElement* /*pThis*/, CMFCRibbonCmdUI* /*pCmdUI*/,
    CFrameWnd* /*pTarget*/, int /*bDisableIfNoHandler*/)
{
}

// The ANSI twin's body at RVA 0x10cd0 (mfc140) updates +0x154, calls a
// virtual, then follows +0xf8 to post a menu command. That state is not modeled.
// Symbol: ?PostMenuCommand@CMFCRibbonBaseElement@@QEAAXI@Z
extern "C" void MS_ABI impl__PostMenuCommand_CMFCRibbonBaseElement__QEAAXI_Z(
    CMFCRibbonBaseElement* /*pThis*/, unsigned int /*uiCmdId*/)
{
}

// The ANSI twin's body at RVA 0x105b0 (mfc140) tests the rect at +0xc8 and
// routes redraw through parent/menu state. That geometry and state are absent.
// Symbol: ?Redraw@CMFCRibbonBaseElement@@UEAAXXZ
extern "C" void MS_ABI impl__Redraw_CMFCRibbonBaseElement__UEAAXXZ(
    CMFCRibbonBaseElement* /*pThis*/)
{
}

// The exact decorated symbol was not resolved in either the mfc140u or ANSI
// symbol map, so its body could not be verified and this remains a stub.
// Symbol: ?ReplaceByID@CMFCRibbonBaseElement@@MEAAHIPEAV1@@Z
extern "C" int MS_ABI impl__ReplaceByID_CMFCRibbonBaseElement__MEAAHIPEAV1__Z(
    CMFCRibbonBaseElement* /*pThis*/, unsigned int /*uiCmdID*/,
    CMFCRibbonBaseElement* /*pElem*/)
{
    return 0;
}

// The ANSI twin's body at RVA 0x11240 (mfc140) initializes accessibility
// strings/flags, copies the rect at +0xc8, and queries multiple virtual states.
// CAccessibilityData is modeled, but the ribbon element strings, rect, and
// virtual state used to populate it are not.
// Symbol: ?SetACCData@CMFCRibbonBaseElement@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCRibbonBaseElement__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCRibbonBaseElement* /*pThis*/, CWnd* /*pParent*/,
    CAccessibilityData& /*data*/)
{
    return 0;
}

// The ANSI twin's body at RVA 0x101e0 (mfc140) writes popup state at +0x108,
// +0x154, +0x144 and +0x13c, then invokes virtual callbacks. Those fields and
// slots are not modeled.
// Symbol: ?SetDroppedDown@CMFCRibbonBaseElement@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__SetDroppedDown_CMFCRibbonBaseElement__IEAAXPEAVCMFCPopupMenu___Z(
    CMFCRibbonBaseElement* /*pThis*/, CMFCPopupMenu* /*pPopupMenu*/)
{
}
