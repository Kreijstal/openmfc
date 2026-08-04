// CMFCRibbonBaseElement — OpenMFC implementation.
// Sources: cbarcore.cpp, global_mfc-feature-22_impl.cpp, global_mfc-feature-23_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCRibbonBaseElementSupport.h"
#include "detail/MfcFeature23ImplSupport.h"
#include "detail/MfccoreSupport.h"

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
// retail rejects a NULL output pointer with E_INVALIDARG and returns S_FALSE
// for any child that is not CHILDID_SELF.  For CHILDID_SELF it computes the
// element rect (+0xc8) through the parent accessibility object; the rect and
// the parent are not modeled, so the outputs are zeroed and S_OK returned.
// TODO(clean-room): transcribed partially -- the element rect and the parent
// accessibility object are not modeled.
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
// returns 1 (S_FALSE) when the accessibility-mode flag at +0x17c is clear,
// which is the only state OpenMFC can produce; otherwise it walks the parent
// panel-menu-bar navigation (+0xf0).  The flag and the navigation state are
// not modeled.
// TODO(clean-room): transcribed partially -- accessibility navigation state
// is not modeled.
// Symbol: ?accNavigate@CMFCRibbonBaseElement@@UEAAJJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__accNavigate_CMFCRibbonBaseElement__UEAAJJUtagVARIANT__PEAU2__Z(
    CMFCRibbonBaseElement* pThis, long /*navDir*/, VARIANT /*varStart*/, VARIANT* pvarEnd)
{
    if (!pThis) return S_FALSE;
    if (!pvarEnd) return E_INVALIDARG;
    return S_FALSE;
}
// CMFCRibbonBaseElement::get_accParent(IDispatch**) -- retail rejects a NULL
// output with E_INVALIDARG, clears the output, and returns 1 (S_FALSE) when
// the accessibility-mode flag at +0x17c is clear (the only state OpenMFC can
// produce).  When set it resolves the parent bar's (+0xd8) accessibility
// object; the flag and the parent are not modeled.
// TODO(clean-room): transcribed partially -- the parent-bar accessibility
// object is not modeled.
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
