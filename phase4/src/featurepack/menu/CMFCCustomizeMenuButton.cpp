// CMFCCustomizeMenuButton — OpenMFC implementation.
// Sources: global_cmfccustomizemenubutton.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCustomizeMenuButtonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

namespace {
// This class is not declared in include/openmfc. Retail afxcustomizemenubutton.h
// lines 86-92 place these seven fields after the 0x128-byte base; retail
// OnDraw at RVA 0x36c20 (mfc140u) reads the offsets pinned below, and
// CreateObject at RVA 0x36210 (mfc140u) allocates 0x148 bytes.
struct CMFCCustomizeMenuButtonShadow {
    unsigned char base[0x128];
    UINT m_uiIndex;
    int m_bShow;
    int bSeparator;
    int m_bExist;
    int m_bAddSpr;
    int m_bIsEnabled;
    int m_bBrothersBtn;
    unsigned char tailPad[4];
};
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, m_uiIndex) == 0x128, "retail customize button m_uiIndex");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, m_bShow) == 0x12c, "retail customize button m_bShow");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, bSeparator) == 0x130, "retail customize button bSeparator");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, m_bExist) == 0x134, "retail customize button m_bExist");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, m_bAddSpr) == 0x138, "retail customize button m_bAddSpr");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, m_bIsEnabled) == 0x13c, "retail customize button m_bIsEnabled");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, m_bBrothersBtn) == 0x140, "retail customize button m_bBrothersBtn");
static_assert(offsetof(CMFCCustomizeMenuButtonShadow, tailPad) == 0x144, "retail customize button tail padding");
static_assert(sizeof(CMFCCustomizeMenuButtonShadow) == 0x148, "retail CMFCCustomizeMenuButton size");

// Retail CMap<UINT, UINT, int, int> storage. The same 0x38-byte layout and
// 0x18-byte associations are pinned in CCommandManager.cpp; its retail
// GetAssocAt helper is also the helper called by IsCommandExist.
struct CustomizePresentAssoc {
    UINT key;
    int value;
    CustomizePresentAssoc* pNext;
    UINT nHashValue;
};
struct CustomizePresentMap {
    void* vfptr;
    CustomizePresentAssoc** pHashTable;
    UINT hashTableSize;
    UINT _pad14;
    INT_PTR count;
    CustomizePresentAssoc* pFreeList;
    void* pBlocks;
    INT_PTR blockSize;
};
static_assert(sizeof(CustomizePresentAssoc) == 0x18, "retail CMap association size");
static_assert(offsetof(CustomizePresentAssoc, pNext) == 0x08 &&
              offsetof(CustomizePresentAssoc, nHashValue) == 0x10, "retail CMap association offsets");
static_assert(sizeof(CustomizePresentMap) == 0x38, "retail CMap<UINT,UINT,int,int> size");
static_assert(offsetof(CustomizePresentMap, pHashTable) == 0x08 &&
              offsetof(CustomizePresentMap, hashTableSize) == 0x10 &&
              offsetof(CustomizePresentMap, count) == 0x18 &&
              offsetof(CustomizePresentMap, pFreeList) == 0x20 &&
              offsetof(CustomizePresentMap, pBlocks) == 0x28 &&
              offsetof(CustomizePresentMap, blockSize) == 0x30, "retail CMap header offsets");
} // namespace

extern "C" CustomizePresentMap impl__m_mapPresentIDs_CMFCCustomizeMenuButton__2V__CMap_IIHH__A;
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, unsigned int uiID, HMENU hMenu, int image, const wchar_t* text, int userButton);
extern "C" void MS_ABI impl___1CMFCToolBarMenuButton__UEAA_XZ(void* pThis);
extern "C" void* impl__m_pWndToolBar_CMFCCustomizeMenuButton__1PEAVCMFCToolBar__EA;
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
    const CMFCToolBar* pThis, int nIndex);
extern "C" int MS_ABI impl__IsUserDefined_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);

// CopyFrom(const CMFCToolBarButton& src): base copy plus seven 32-bit derived
// fields. The field stores match retail RVA 0x363c0 (mfc140u); the null guard
// is defensive and is not present in retail.
// Symbol: ?CopyFrom@CMFCCustomizeMenuButton@@MEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCCustomizeMenuButton__MEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton& src)
{
    if (pThis == nullptr) return;
    impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
        reinterpret_cast<CMFCToolBarMenuButton*>(pThis),
        reinterpret_cast<const CMFCToolBarButton*>(&src));
    auto* dst = reinterpret_cast<CMFCCustomizeMenuButtonShadow*>(pThis);
    const auto* srcButton = reinterpret_cast<const CMFCCustomizeMenuButtonShadow*>(&src);
    dst->m_uiIndex = srcButton->m_uiIndex;
    dst->m_bShow = srcButton->m_bShow;
    dst->bSeparator = srcButton->bSeparator;
    dst->m_bExist = srcButton->m_bExist;
    dst->m_bAddSpr = srcButton->m_bAddSpr;
    dst->m_bIsEnabled = srcButton->m_bIsEnabled;
    dst->m_bBrothersBtn = srcButton->m_bBrothersBtn;
}
// Retail RVA 0x36210 (mfc140u) allocates 0x148 bytes and, when allocation
// succeeds, enters the default constructor at RVA 0x36250 (mfc140u). The local
// constructor now initializes the base but cannot install the unavailable
// derived vftable, so this factory still cannot produce a valid derived object.
// Symbol: ?CreateObject@CMFCCustomizeMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCCustomizeMenuButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): leave stubbed until the derived-vftable installation
    // can be represented; the local constructor only initializes the base.
    return nullptr;
}
// DrawCheckBox(CDC* pDC, const CRect& rect, int nCheck): draws the checked
// state of the "customize" popup item. Retail RVA 0x37160 (mfc140u) calls
// CMFCToolBarButton::FillInterior at RVA 0x15e300 (mfc140u) on its shown path,
// then uses a visual-manager checkbox helper; those drawing dependencies are
// not modeled, so the body is conservative.
// Symbol: ?DrawCheckBox@CMFCCustomizeMenuButton@@MEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawCheckBox_CMFCCustomizeMenuButton__MEAAXPEAVCDC__AEBVCRect__H_Z(
    void* pThis, void* /*pDC*/, const CRect* /*rect*/, int /*nCheck*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): not transcribed -- depends on FillInterior at RVA
    // 0x15e300 (mfc140u) and the visual-manager checkbox drawing; not modeled.
}
// Retail RVA 0x37300 (mfc140u) calls CMap::Lookup on m_mapPresentIDs and
// returns its BOOL result. Retail CMap::Lookup at RVA 0x2f610 calls its
// GetAssocAt helper at RVA 0x236e0, whose hash calculation uses ldiv; iatu.py
// resolves the helper's call at RVA 0x236fb to UCRT ldiv. The arithmetic below
// reproduces that hash and searches the retail-layout association chain.
// Symbol: ?IsCommandExist@CMFCCustomizeMenuButton@@SAHI@Z
extern "C" int MS_ABI impl__IsCommandExist_CMFCCustomizeMenuButton__SAHI_Z(
    unsigned int nID)
{
    const CustomizePresentMap& map = impl__m_mapPresentIDs_CMFCCustomizeMenuButton__2V__CMap_IIHH__A;
    if (map.pHashTable == nullptr) return 0;

    // GetAssocAt calls ldiv((LONG)nID, 0x1f31d), then computes
    // 0x41a7 * remainder - 0xb14 * quotient with 32-bit wraparound.
    std::int32_t signedID;
    std::memcpy(&signedID, &nID, sizeof signedID);
    const std::int32_t quotient = signedID / 0x1f31d;
    const std::int32_t remainder = signedID % 0x1f31d;
    std::uint32_t hash = 0x41a7u * static_cast<std::uint32_t>(remainder) -
                         0xb14u * static_cast<std::uint32_t>(quotient);
    if ((hash & 0x80000000u) != 0) hash += 0x7fffffffu;

    const UINT bucket = hash % map.hashTableSize;
    for (CustomizePresentAssoc* assoc = map.pHashTable[bucket]; assoc != nullptr; assoc = assoc->pNext) {
        if (assoc->nHashValue == hash && assoc->key == nID) return 1;
    }
    return 0;
}
// OnCalculateSize(CDC* pDC, CSize sizeButton, BOOL bHorz): returns the button
// size. Retail RVA 0x36440 (mfc140u) delegates to
// CMFCToolBarMenuButton::OnCalculateSize at RVA 0x173560 (mfc140u) and folds
// in the caption-string width. The hidden return
// slot is the second parameter (after `this`), matching the retail register
// usage.  Conservative: returns {0, 0}.
// Symbol: ?OnCalculateSize@CMFCCustomizeMenuButton@@MEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCCustomizeMenuButton__MEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, void* /*pDC*/, const CSize* /*sizeButton*/, int /*bHorz*/)
{
    if (pThis == nullptr || pRet == nullptr) return pRet;
    // TODO(clean-room): not transcribed -- retail delegates to the base
    // CMFCToolBarMenuButton::OnCalculateSize at RVA 0x173560 (mfc140u) and
    // adds caption width (CString at +0x38); those details are not modeled.
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}
// OnClickMenuItem(): invoked when the "customize" menu item is clicked. Retail
// RVA 0x36850 (mfc140u) has distinct branches using the toolbar, global
// customize state, and m_bBrothersBtn; most paths return TRUE, but the
// m_bBrothersBtn branch can return FALSE. This pre-existing body only preserves
// the common TRUE result and does not perform those side effects.
// Symbol: ?OnClickMenuItem@CMFCCustomizeMenuButton@@MEAAHXZ
extern "C" int MS_ABI impl__OnClickMenuItem_CMFCCustomizeMenuButton__MEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): not transcribed -- depends on the toolbar pointer at
    // +0x80, ButtonToIndex at RVA 0x14f000 and GetButton at RVA 0x14fe00
    // (both mfc140u), plus global customize state; not modeled.
    return 1;
}
// Retail RVA 0x36250 (mfc140u) calls the menu-button default constructor and
// then installs a derived vftable that is not modeled here. The base constructor
// is still invoked so its storage is initialized; the base vftable remains.
// Symbol: ??0CMFCCustomizeMenuButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCCustomizeMenuButton__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    return impl___0CMFCToolBarMenuButton__QEAA_XZ(pThis);
}
// Retail RVA 0x362e0 (mfc140u) calls the matching base overload and initializes
// the derived fields at +0x128, +0x130, +0x138, and +0x13c/+0x140. The derived
// vftable store is omitted because no matching vftable is modeled.
// Symbol: ??0CMFCCustomizeMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
extern "C" void* MS_ABI impl___0CMFCCustomizeMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
    void* pThis, unsigned int uiID, HMENU hMenu, int image,
    const wchar_t* text, int userButton) {
    if (pThis == nullptr) return pThis;
    impl___0CMFCToolBarMenuButton__QEAA_IPEAUHMENU____HPEB_WH_Z(
        pThis, uiID, hMenu, image, text, userButton);
    auto* button = reinterpret_cast<CMFCCustomizeMenuButtonShadow*>(pThis);
    button->m_uiIndex = 0xffffffffu;
    button->bSeparator = 0;
    button->m_bAddSpr = 0;
    button->m_bIsEnabled = 1;
    button->m_bBrothersBtn = 0;
    return pThis;
}
// Retail RVA 0x362d0 (mfc140u) writes the derived vftable then tail-jumps to
// the CMFCToolBarMenuButton destructor at RVA 0x172bf0 (mfc140u). The base
// destructor immediately replaces the vftable, so this omits that transient
// derived-vftable store (OpenMFC has no such vftable), adds a defensive null
// guard (retail assumes a valid this), and delegates cleanup to the verified
// base destructor thunk.
// Symbol: ??1CMFCCustomizeMenuButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCCustomizeMenuButton__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    impl___1CMFCToolBarMenuButton__UEAA_XZ(pThis);
}

// Retail RVA 0x36c20 (mfc140u) is a long drawing routine with multiple helper
// and indirect calls. Its visual-manager and menu drawing dependencies are not
// modeled sufficiently to transcribe safely, so this remains an explicit stub.
// Symbol: ?OnDraw@CMFCCustomizeMenuButton@@MEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCustomizeMenuButton__MEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, void* pDC, const CRect* pRect, void* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons)
{
    (void)pThis;
    (void)pDC;
    (void)pRect;
    (void)pImages;
    (void)bHorz;
    (void)bCustomizeMode;
    (void)bHighlight;
    (void)bDrawBorder;
    (void)bGrayDisabledButtons;
}

// The Unicode SearchCommandText export has no resolved RVA. Its analogous
// ANSI-twin function at RVA 0x370d0 (mfc140) enumerates menu items and recurses
// into submenus; the CString result construction is not modeled here.
// Symbol: ?SearchCommandText@CMFCCustomizeMenuButton@@IEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCMenu@@I@Z
extern "C" void* MS_ABI impl__SearchCommandText_CMFCCustomizeMenuButton__IEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCMenu__I_Z(
    void* pRet, void* pThis, void* pMenu, unsigned int commandID)
{
    (void)pThis;
    (void)pMenu;
    (void)commandID;
    return pRet;
}

// The Unicode SetItemIndex RVA is unresolved, but the ANSI twin at RVA 0x36470
// (mfc140) shows these stores/branches. The shared non-string logic is
// implemented here; null toolbar/button checks are defensive deviations.
// Symbol: ?SetItemIndex@CMFCCustomizeMenuButton@@QEAAXIHH@Z
extern "C" void MS_ABI impl__SetItemIndex_CMFCCustomizeMenuButton__QEAAXIHH_Z(
    void* pThis, unsigned int uiIndex, int bExist, int bAddSeparator)
{
    if (pThis == nullptr) return;
    auto* button = reinterpret_cast<CMFCCustomizeMenuButtonShadow*>(pThis);
    button->m_uiIndex = uiIndex;
    button->m_bExist = bExist;
    button->m_bAddSpr = bAddSeparator;

    auto* toolbar = reinterpret_cast<CMFCToolBar*>(
        impl__m_pWndToolBar_CMFCCustomizeMenuButton__1PEAVCMFCToolBar__EA);
    if (uiIndex == 0x4278 || button->bSeparator != 0 || bExist == 0) {
        button->m_bShow = 0;
        if (uiIndex == 0x4278 && toolbar != nullptr &&
            impl__IsUserDefined_CMFCToolBar__QEBAHXZ(toolbar) != 0) {
            button->m_bIsEnabled = 0;
        }
        return;
    }

    if (toolbar != nullptr) {
        CMFCToolBarButton* toolbarButton =
            impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
                toolbar, static_cast<int>(uiIndex));
        if (toolbarButton != nullptr) button->m_bShow = toolbarButton->m_bVisible;
    }
}

// The Unicode RVA is unresolved. The ANSI twin at RVA 0x373b0 (mfc140) gets
// the toolbar's parent, redraws it with flags 0x505 when it has a window, then
// calls CMFCToolBar::RedrawCustomizeButton at RVA 0x159ed0 (mfc140). The
// corresponding local thunks are used below; null checks are defensive.
// Symbol: ?UpdateCustomizeButton@CMFCCustomizeMenuButton@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateCustomizeButton_CMFCCustomizeMenuButton__IEAAXXZ(void* pThis) {
    (void)pThis;
    auto* toolbar = reinterpret_cast<CMFCToolBar*>(
        impl__m_pWndToolBar_CMFCCustomizeMenuButton__1PEAVCMFCToolBar__EA);
    if (toolbar == nullptr) return;

    CWnd* parent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
        ::GetParent(toolbar->m_hWnd));
    if (parent != nullptr && parent->m_hWnd != nullptr) {
        CWnd* parentWindow = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
            ::GetParent(toolbar->m_hWnd));
        if (parentWindow != nullptr && parentWindow->m_hWnd != nullptr) {
            ::RedrawWindow(parentWindow->m_hWnd, nullptr, nullptr, 0x505);
        }
    }
    impl__RedrawCustomizeButton_CMFCToolBar__IEAAXXZ(toolbar);
}

// Retail m_mapPresentIDs is a 0x38-byte CMap<UINT,UINT,int,int> object. Its
// non-virtual header fields use defaults (hash size 17, block size 10), also
// observed in the retail CCommandManager constructor at RVA 0x2ed80 (mfc140u).
// OpenMFC has no matching CMap vftable, so vfptr remains null.
// Symbol: ?m_mapPresentIDs@CMFCCustomizeMenuButton@@2V?$CMap@IIHH@@A
extern "C" CustomizePresentMap impl__m_mapPresentIDs_CMFCCustomizeMenuButton__2V__CMap_IIHH__A = {
    nullptr, nullptr, 17, 0, 0, nullptr, nullptr, 10
};
