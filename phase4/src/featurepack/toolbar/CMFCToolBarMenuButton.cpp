// CMFCToolBarMenuButton — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Retail layout used by the transcribed bodies below.
//
// Retail CMFCToolBarMenuButton (afxtoolbarmenubutton.h, on-host SDK 14.51) is the
// 0x88-byte CMFCToolBarButton followed by
//     CObList m_listCommands  @0x88 (0x38 bytes: vptr, m_pNodeHead @0x90,
//                                    m_pNodeTail @0x98, m_nCount @0xa0, ...)
//     CMFCPopupMenu* m_pPopupMenu @0xc0
//     BOOL m_bDrawDownArrow @0xc8, m_bMenuMode @0xcc, m_bDefault @0xd0,
//          m_bClickedOnMenu @0xd4, m_bHorz @0xd8, m_bToBeClosed @0xdc,
//          m_bMenuOnly @0xe0, m_bIsRadio @0xe4, m_bMenuPaletteMode @0xe8,
//          m_bQuickCustomMode @0xec, m_bShowAtRightSide @0xf0
//     UINT m_uiTearOffBarID @0xf4, int m_nPaletteRows @0xf8
//     CWnd* m_pWndMessage @0x100, CRect m_rectArrow @0x108, CRect m_rectButton @0x118
// for a total of 0x128 (296) bytes.  Every offset named below is one the retail
// bodies in this file actually touch (cited at each use).
//
// OpenMFC's afxmfc.h declares `HMENU m_hMenu` at 0x88 followed by `char _pad[152]`,
// so m_hMenu overlays the CObList vptr and the list is never constructed by
// OpenMFC's C++ constructors (defined further down in this file).  Everything from
// 0x90 on lies inside _pad, which those constructors zero (memset in the default
// and copy constructors) -- so m_listCommands reads as an empty list (head/count 0)
// and every BOOL/UINT above reads 0 until something stores to it.  The bodies
// below read and write the retail offsets directly; they never touch 0x88.
// ---------------------------------------------------------------------------
namespace {
constexpr std::size_t kMbListHead         = 0x90;   // m_listCommands.m_pNodeHead
constexpr std::size_t kMbListCount        = 0xa0;   // m_listCommands.m_nCount (INT_PTR)
constexpr std::size_t kMbPopupMenu        = 0xc0;   // CMFCPopupMenu* m_pPopupMenu
constexpr std::size_t kMbDrawDownArrow    = 0xc8;   // BOOL m_bDrawDownArrow
constexpr std::size_t kMbMenuMode         = 0xcc;   // BOOL m_bMenuMode
constexpr std::size_t kMbToBeClosed       = 0xdc;   // BOOL m_bToBeClosed
constexpr std::size_t kMbIsRadio          = 0xe4;   // BOOL m_bIsRadio
constexpr std::size_t kMbMenuPaletteMode  = 0xe8;   // BOOL m_bMenuPaletteMode
constexpr std::size_t kMbTearOffBarID     = 0xf4;   // UINT m_uiTearOffBarID
constexpr std::size_t kMbPaletteRows      = 0xf8;   // int  m_nPaletteRows

// CObList::CNode is { CNode* pNext; CNode* pPrev; CObject* data; }.
constexpr std::size_t kNodeNext = 0x00;
constexpr std::size_t kNodeData = 0x10;

// CWnd::m_hWnd (also the popup menu's HWND).
constexpr std::size_t kWndHWnd = 0x40;
// CMFCMenuBar::m_bForceDownArrows (see featurepack/menu/CMFCMenuBar.cpp, which
// pins the same offset; inside OpenMFC's 0x13b0-byte CMFCMenuBar).
constexpr std::size_t kMenuBarForceDownArrows = 0x139c;
// CMFCVisualManager::m_nMenuShadowDepth (read by OnCancelMode at +0xcc).
constexpr std::size_t kVmMenuShadowDepth = 0xcc;
// AFX_GLOBAL_DATA: the one-time init gate at +0 and m_sizeSmallIcon at +0x1a0/+0x1a4
// (pinned in core/runtime/AFX_GLOBAL_DATA.cpp).
constexpr std::size_t kGdInitGate        = 0x000;
constexpr std::size_t kGdSizeSmallIconCx = 0x1a0;
constexpr std::size_t kGdSizeSmallIconCy = 0x1a4;

static_assert(sizeof(CMFCToolBarMenuButton) == 0x128, "retail CMFCToolBarMenuButton is 296 bytes");
static_assert(offsetof(CMFCToolBarMenuButton, _pad) == 0x90,
              "every retail member this file touches lies inside OpenMFC's zeroed _pad");
static_assert(kMbPaletteRows + sizeof(int) <= sizeof(CMFCToolBarMenuButton), "offsets inside the object");
static_assert(offsetof(CMFCToolBarButton, m_bText) == 0x0c, "CMFCToolBarButton::m_bText @0x0c");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == 0x10, "CMFCToolBarButton::m_bImage @0x10");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "CMFCToolBarButton::m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "CMFCToolBarButton::m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "CMFCToolBarButton::m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "CMFCToolBarButton::m_pWndParent @0x80");
static_assert(offsetof(CWnd, m_hWnd) == kWndHWnd, "CWnd::m_hWnd @0x40");
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC @0x08");
static_assert(offsetof(CAccessibilityData, m_strAccHelp) == 0x20 &&
              offsetof(CAccessibilityData, m_strAccDefAction) == 0x28 &&
              offsetof(CAccessibilityData, m_nAccRole) == 0x30 &&
              offsetof(CAccessibilityData, m_bAccState) == 0x34, "retail CAccessibilityData offsets");

template <class T> inline T& MbAt(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}
template <class T> inline T MbGet(const void* p, std::size_t off) {
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(p) + off, sizeof v);
    return v;
}
} // namespace

// ---------------------------------------------------------------------------
// Cross-file thunks (C++ methods exist only as these; see BRIEFING section 1).
// Parameter lists are the ones the mangled names describe.
// ---------------------------------------------------------------------------
//   core/runtime/CObject.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/runtime/CRuntimeClass.cpp
extern "C" int MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(const CRuntimeClass* pThis, const CRuntimeClass* pBaseClass);
//   core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
//   RTTI getters (featurepack/menu/RuntimeClasses.cpp, featurepack/toolbar/RuntimeClasses.cpp,
//   featurepack/ribbon/RuntimeClasses.cpp, featurepack/visualmanager/*)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCRibbonGalleryMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2007__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ();
//   featurepack/toolbar/CMFCToolBarButton.cpp -- the base-class bodies retail calls
//   non-virtually.  (Several are still stubs there; the lists below are the retail
//   ones, which is harmless across translation units for extern "C" symbols.)
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(CMFCToolBarButton* pThis, CWnd* pWndParent);
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBarButton* pThis, CDC* pDC, const CRect* pRect, int bSelected);
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCToolBarButton* pThis, CWnd* pParent, CAccessibilityData* pData);
//   featurepack/toolbar/CMFCToolBar.cpp / featurepack/controls/CMFCReBar.cpp:
//   static CSize getters; the hidden CSize slot arrives in RCX and comes back in RAX.
extern "C" CSize* MS_ABI impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(CSize* pRet);
extern "C" CSize* MS_ABI impl__Size_CMenuImages__SA_AVCSize__XZ(CSize* pRet);
//   featurepack/menu/CMFCPopupMenu.cpp
extern "C" void MS_ABI impl__CollapseSubmenus_CMFCPopupMenu__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SaveState_CMFCPopupMenu__UEAAXXZ(void* pThis);
//   featurepack/visualmanager/Thunks.cpp (dispatches on the C++ manager object)
extern "C" void MS_ABI impl__OnDrawMenuArrowOnCustomizeList_CMFCVisualManager__UEAAXPEAVCDC__VCRect__H_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bSelected);
//   core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
//   core/runtime/StaticData.cpp: ?m_pRTIDefault@CMFCVisualManager@@1PEAUCRuntimeClass@@EA
//   (?m_pVisManager@CMFCVisualManager@@1PEAV1@EA is declared by detail/MfccoreSupport.h)
extern "C" void* impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA;

// Exports of this file that other bodies here call directly.  Retail calls
// GetImageRect non-virtually from SetRadio (direct call to 0x175ea0, mfc140u), but
// dispatches the per-command ResetImageToDefault VIRTUALLY (vslot +0x130); the
// direct recursive call below is a documented deviation -- see ResetImageToDefault.
extern "C" void MS_ABI impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(CMFCToolBarMenuButton* pThis, CRect* pRectImage);
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarMenuButton__UEAAXXZ(CMFCToolBarMenuButton* pThis);

namespace {
// Retail's inlined AFX_GLOBAL_DATA gate: if (afxGlobalData.<+0> == 0) { Initialize(); <+0> = 1; }
inline void MbEnsureGlobalData() {
    unsigned char* gd = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    if (MbGet<int>(gd, kGdInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(gd);
        const int one = 1;
        std::memcpy(gd + kGdInitGate, &one, sizeof one);
    }
}

inline bool MbIsKindOf(const void* p, CRuntimeClass* pClass) {
    return p != nullptr && pClass != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(static_cast<const CObject*>(p), pClass) != 0;
}

// CMFCToolBarMenuButton::HasButton() is vtable slot 62 (+0x1f0; the harvested
// 65-slot vtable, tools/harvest/family).  Retail dispatches it through `this`'s
// vptr.  OpenMFC-constructed menu buttons carry the mingw (Itanium) vtable, which
// has no MSVC slot 62, so the call cannot be made by slot.  DEVIATION: the only
// stock override in the retail headers is CMFCRibbonGalleryMenuButton::HasButton()
// `{ return TRUE; }` (afxribbonpalettegallery.h:378); the base inline returns FALSE
// (afxtoolbarmenubutton.h:98).  Both are reproduced by runtime class; an override
// in a client-derived class is NOT honoured.
inline BOOL MbHasButton(const CMFCToolBarMenuButton* pThis) {
    return MbIsKindOf(pThis, impl__GetThisClass_CMFCRibbonGalleryMenuButton__SAPEAUCRuntimeClass__XZ()) ? TRUE : FALSE;
}

// CMFCVisualManager::GetMenuImageMargin() is visual-manager vtable slot 185
// (+0x5c8), an inline virtual that is not exported, so it cannot be called here.
// Its retail bodies are `{ return 2; }` in CMFCVisualManager (afxvisualmanager.h:513)
// and `{ return 3; }` in CMFCVisualManagerOfficeXP (afxvisualmanagerofficexp.h:93),
// and every other stock manager (Office2003/2007, VS2005/2008, Windows, Windows7)
// derives from OfficeXP without overriding it.  OpenMFC's descriptors for those
// classes do not chain through OfficeXP, so each is tested explicitly.
// DEVIATIONS: (1) an override in a client-derived manager is not honoured;
// (2) retail's inlined GetInstance() CREATES the manager when
// ?m_pVisManager@ is NULL (from ?m_pRTIDefault@, or a plain CMFCVisualManager);
// this helper does not create it but answers for the class that would be created.
inline BOOL MbIsXPFamily(const CRuntimeClass* pClass, const void* pObject) {
    CRuntimeClass* const family[] = {
        impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ(),
        impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ(),
        impl__GetThisClass_CMFCVisualManagerOffice2007__SAPEAUCRuntimeClass__XZ(),
        impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ(),
        impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ(),
        impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ(),
        impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ(),
    };
    for (CRuntimeClass* c : family) {
        if (c == nullptr) continue;
        if (pObject != nullptr) {
            if (MbIsKindOf(pObject, c)) return TRUE;
        } else if (pClass != nullptr &&
                   impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(pClass, c) != 0) {
            return TRUE;
        }
    }
    return FALSE;
}
inline int MbMenuImageMargin() {
    const void* pVM = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (pVM != nullptr) return MbIsXPFamily(nullptr, pVM) ? 3 : 2;
    const CRuntimeClass* pDefault =
        static_cast<const CRuntimeClass*>(impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA);
    return (pDefault != nullptr && MbIsXPFamily(pDefault, nullptr)) ? 3 : 2;
}
} // namespace


// mfccore.cpp carried a second copy of the constructor and Initialize; cbarcore.cpp
// linked first, so those were dead code. The cbarcore definitions are below.
// Symbol: ??0CMFCToolBarMenuButton@@QEAA@IPEAUHMENU__@@HPEB_WH@Z
// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXIPEAUHMENU____HPEB_WH_Z(CMFCToolBarMenuButton* pThis, unsigned int p0, HMENU p1, int p2, const wchar_t* p3, int p4) {
    pThis->Initialize(p0, p1, (BOOL)p2, p3, (BOOL)p4);
}
// Symbol: ??0CMFCToolBarMenuButton@@QEAA@AEBV0@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarMenuButton__QEAA_AEBV0__Z(void* pThis, const CMFCToolBarMenuButton* pSrc) {
    return new (pThis) CMFCToolBarMenuButton(*pSrc);
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXIPEAUHMENU__@@HPEB_WH@Z
// Symbol: ?Initialize@CMFCToolBarMenuButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarMenuButton__IEAAXXZ(CMFCToolBarMenuButton* pThis) {
    if (pThis) pThis->Initialize(0, nullptr, FALSE, nullptr, FALSE);
}
// Symbol: ?CreateFromMenu@CMFCToolBarMenuButton@@UEAAXPEAUHMENU__@@@Z
extern "C" void MS_ABI impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(CMFCToolBarMenuButton* pThis, HMENU hMenu) {
    if (pThis) pThis->CreateFromMenu(hMenu);
}
// Symbol: ?CreateMenu@CMFCToolBarMenuButton@@UEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCToolBarMenuButton__UEBAPEAUHMENU____XZ(const CMFCToolBarMenuButton* pThis) {
    return pThis ? pThis->CreateMenu() : nullptr;
}
// Symbol: ?CopyFrom@CMFCToolBarMenuButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc) {
    if (pThis && pSrc) pThis->CopyFrom(*pSrc);
}
// Symbol: ?CompareWith@CMFCToolBarMenuButton@@UEBAHAEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarMenuButton__UEBAHAEBVCMFCToolBarButton___Z(const CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pOther) {
    return (pThis && pOther) ? pThis->CompareWith(*pOther) : 0;
}
CMFCToolBarMenuButton::CMFCToolBarMenuButton() : m_hMenu(nullptr) { memset(_pad, 0, sizeof(_pad)); }
CMFCToolBarMenuButton::CMFCToolBarMenuButton(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton)
    : CMFCToolBarMenuButton() {
    Initialize(uiID, hMenu, bHasDropDownArrow, lpszText, bUserButton);
}
CMFCToolBarMenuButton::CMFCToolBarMenuButton(const CMFCToolBarMenuButton& src)
    : CMFCToolBarButton(src.m_nID, src.m_iImage, static_cast<const wchar_t*>(src.m_strText), src.m_bUserButton, src.m_bLocked),
      m_hMenu(src.m_hMenu) {
    memset(_pad, 0, sizeof(_pad));
    CopyMenuButtonState(this, &src);
}
CMFCToolBarMenuButton::~CMFCToolBarMenuButton() {
    g_menuButtonStates.erase(this);
}
void CMFCToolBarMenuButton::Initialize(UINT uiID, HMENU hMenu, BOOL bHasDropDownArrow, const wchar_t* lpszText, BOOL bUserButton) {
    m_nID = uiID;
    m_hMenu = hMenu;
    m_bUserButton = bUserButton;
    if (lpszText) m_strText = lpszText;
    MenuButtonState& state = g_menuButtonStates[this];
    state.hasDropDownArrow = bHasDropDownArrow != FALSE;
    state.userButton = bUserButton != FALSE;
}
void CMFCToolBarMenuButton::CreateFromMenu(HMENU hMenu) {
    m_hMenu = hMenu;
}
HMENU CMFCToolBarMenuButton::CreateMenu() const {
    return m_hMenu ? m_hMenu : ::CreatePopupMenu();
}
void CMFCToolBarMenuButton::CopyFrom(const CMFCToolBarButton& src) {
    m_nID = src.m_nID;
    m_iImage = src.m_iImage;
    m_strText = src.m_strText;
    m_bUserButton = src.m_bUserButton;
    m_bLocked = src.m_bLocked;
    if (src.IsKindOf(RUNTIME_CLASS(CMFCToolBarMenuButton))) {
        const CMFCToolBarMenuButton& menuSrc = static_cast<const CMFCToolBarMenuButton&>(src);
        m_hMenu = menuSrc.m_hMenu;
        CopyMenuButtonState(this, &menuSrc);
    }
}
int CMFCToolBarMenuButton::CompareWith(const CMFCToolBarButton& other) const {
    if (m_nID != other.m_nID) {
        return m_nID < other.m_nID ? -1 : 1;
    }
    return m_strText.Compare(other.m_strText);
}
// Retail RVA 0x1761c0 (mfc140u), fully transcribed:
//     if (!afxGlobalData.<+0>) { afxGlobalData.Initialize(); <+0> = 1; }
//     int cx = afxGlobalData.m_sizeSmallIcon.cx /*+0x1a0*/, cy = afxGlobalData.m_sizeSmallIcon.cy /*+0x1a4*/;
//     if (cx > rectImage.Width() || cy > rectImage.Height()) { cx = rectImage.Width(); cy = rectImage.Height(); }
//     int dx = max(0, (rectImage.Width() - cx) / 2), dy = max(0, (rectImage.Height() - cy) / 2);   // cmovns
//     ::DrawIconEx(pDC->GetSafeHdc(), rectImage.left + dx, rectImage.top + dy, hIcon, cx, cy, 0, NULL, DI_NORMAL);
// (The import slot resolves to USER32!DrawIconEx.)  `this` is not read.
// Symbol: ?DrawDocumentIcon@CMFCToolBarMenuButton@@IEAAXPEAVCDC@@AEBVCRect@@PEAUHICON__@@@Z
extern "C" void MS_ABI impl__DrawDocumentIcon_CMFCToolBarMenuButton__IEAAXPEAVCDC__AEBVCRect__PEAUHICON_____Z(
    CMFCToolBarMenuButton* pThis, CDC* pDC, const CRect* pRectImage, HICON hIcon)
{
    (void)pThis;
    if (pRectImage == nullptr) return;
    MbEnsureGlobalData();
    const unsigned char* gd = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    const int width = pRectImage->right - pRectImage->left;
    const int height = pRectImage->bottom - pRectImage->top;
    int cx = MbGet<int>(gd, kGdSizeSmallIconCx);
    int cy = MbGet<int>(gd, kGdSizeSmallIconCy);
    if (cx > width || cy > height) {
        cx = width;
        cy = height;
    }
    int dx = (width - cx) / 2;
    if (dx < 0) dx = 0;
    int dy = (height - cy) / 2;
    if (dy < 0) dy = 0;
    ::DrawIconEx(pDC != nullptr ? pDC->m_hDC : nullptr, pRectImage->left + dx, pRectImage->top + dy,
                 hIcon, cx, cy, 0, nullptr, DI_NORMAL);
}

// Retail RVA 0x174180 (mfc140u): the popup-menu item painter (check/radio marks,
// image or document icon, text + accelerator, sub-menu arrow), almost entirely
// visual-manager virtual calls (GetInstance()->..., non-exported slots) plus
// CMFCToolBarImages / CMenuImages drawing.  Not transcribed: the visual manager
// has no MSVC vtable in OpenMFC and the item text/accelerator state is not modelled.
// Symbol: ?DrawMenuItem@CMFCToolBarMenuButton@@IEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHH@Z
extern "C" void MS_ABI impl__DrawMenuItem_CMFCToolBarMenuButton__IEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHH_Z(
    CMFCToolBarMenuButton* pThis, CDC* pDC, const CRect* pRect, CMFCToolBarImages* pImages,
    int bCustomizeMode, int bHighlight, int bGrayDisabledButtons, int bContentOnly)
{
    (void)pThis; (void)pDC; (void)pRect; (void)pImages;
    (void)bCustomizeMode; (void)bHighlight; (void)bGrayDisabledButtons; (void)bContentOnly;
}

// Retail RVA 0x175ea0 (mfc140u), fully transcribed:
//     rectImage = m_rect;                                                     // +0x68
//     rectImage.left += CMFCVisualManager::GetInstance()->GetMenuImageMargin();   // vslot +0x5c8
//     rectImage.right = rectImage.left + CMFCVisualManager::GetInstance()->GetMenuImageMargin()
//                       + CMFCToolBar::GetMenuImageSize().cx;
// DEVIATION: GetMenuImageMargin() is resolved by MbMenuImageMargin() (see its
// comment above) rather than through the visual manager's vtable.
// Symbol: ?GetImageRect@CMFCToolBarMenuButton@@QEAAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(CMFCToolBarMenuButton* pThis, CRect* pRectImage)
{
    if (pThis == nullptr || pRectImage == nullptr) return;
    *pRectImage = pThis->m_rect;
    pRectImage->left += MbMenuImageMargin();
    const int nMargin = MbMenuImageMargin();
    CSize sizeMenuImage(0, 0);
    impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(&sizeMenuImage);
    pRectImage->right = pRectImage->left + nMargin + sizeMenuImage.cx;
}

// Retail RVA 0x175dc0 (mfc140u), fully transcribed (static, __stdcall == x64 ABI):
//     xOffsetLeft  = CMFCToolBar::GetMenuImageSize().cx / 2 + AFX_TEXT_MARGIN;   // signed /2, +3
//     xOffsetRight = CMenuImages::Size().cx;
// Symbol: ?GetTextHorzOffsets@CMFCToolBarMenuButton@@KAXAEAH0@Z
extern "C" void MS_ABI impl__GetTextHorzOffsets_CMFCToolBarMenuButton__KAXAEAH0_Z(int* pxOffsetLeft, int* pxOffsetRight)
{
    CSize sizeMenuImage(0, 0);
    impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(&sizeMenuImage);
    if (pxOffsetLeft != nullptr) *pxOffsetLeft = sizeMenuImage.cx / 2 + 3;
    CSize sizeMenuImages(0, 0);
    impl__Size_CMenuImages__SA_AVCSize__XZ(&sizeMenuImages);
    if (pxOffsetRight != nullptr) *pxOffsetRight = sizeMenuImages.cx;
}

// Retail RVA 0x175d80 (mfc140u), fully transcribed:
//     if (m_pPopupMenu != NULL) {                                 // +0xc0
//         m_pPopupMenu->CollapseSubmenus();                       // non-virtual call
//         ::SendMessage(m_pPopupMenu->m_hWnd, WM_CLOSE, 0, 0);    // +0x40; import slot = USER32!SendMessage
//     }
//     return TRUE;
// Symbol: ?OnBeforeDrag@CMFCToolBarMenuButton@@UEBAHXZ
extern "C" int MS_ABI impl__OnBeforeDrag_CMFCToolBarMenuButton__UEBAHXZ(const CMFCToolBarMenuButton* pThis)
{
    if (pThis == nullptr) return TRUE;
    void* pPopup = MbGet<void*>(pThis, kMbPopupMenu);
    if (pPopup != nullptr) {
        impl__CollapseSubmenus_CMFCPopupMenu__IEAAXXZ(pPopup);
        pPopup = MbGet<void*>(pThis, kMbPopupMenu);   // retail reloads m_pPopupMenu after the call
        ::SendMessage(MbGet<HWND>(pPopup, kWndHWnd), WM_CLOSE, 0, 0);
    }
    return TRUE;
}

// Retail RVA 0x173560 (mfc140u): stores bHorz to m_bHorz (+0xd8) and returns
// CSize(0,0) when the button is hidden (!m_bVisible, +0x64).  Otherwise, in menu
// mode, it strips the "\t..." suffix from m_strText and re-appends "\t" plus the
// accelerator found by CKeyboardManager::FindDefaultAccelerator; calls
// CMFCToolBarButton::OnCalculateSize; defers to a CMFCDropDownListBox that owns a
// CMFCPopupMenuBar parent (owner vslot +0x440, bar vslot +0x6b8); and otherwise
// widens the result by arrow / image extents, including the visual manager's
// GetMenuImageMargin (vslot +0x5c8).  Not transcribed: those virtual calls and the
// keyboard-manager / frame lookups are not available here.  STUB: returns
// CSize(0,0) through the hidden return slot (RDX, returned in RAX) so a caller
// never dereferences a null result; m_bHorz is not stored.
// Symbol: ?OnCalculateSize@CMFCToolBarMenuButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarMenuButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarMenuButton* pThis, SIZE* pRet, CDC* pDC, const CSize* pSizeDefault, int bHorz)
{
    (void)pThis; (void)pDC; (void)pSizeDefault; (void)bHorz;
    if (pRet != nullptr) {
        pRet->cx = 0;
        pRet->cy = 0;
    }
    return pRet;
}

// Retail RVA 0x175820 (mfc140u):
//     if (m_pPopupMenu != NULL && ::IsWindow(m_pPopupMenu->m_hWnd)) {
//         if (m_pPopupMenu->InCommand()) return;                          // popup vslot +0x3a8 (a)
//         CMFCPopupMenuBar* pBar = m_pPopupMenu->GetMenuBar();            // popup vslot +0x3a0 (a)
//         for (int i = 0; i < pBar->m_Buttons.GetCount() /*+0x11a0*/; i++)
//             if (CMFCToolBarButton* p = pBar->GetButton(i)) p->OnCancelMode();   // button vslot +0xc0
//         m_pPopupMenu->SaveState();                                      // popup vslot +0x3d0
//         m_pPopupMenu->m_bAutoDestroyParent = FALSE;                     // popup +0x1f0 (first CMFCPopupMenu member)
//         m_pPopupMenu->CloseMenu(FALSE);
//     }
//     m_pPopupMenu = NULL;                                                // +0xc0
//     if (m_pWndParent != NULL && ::IsWindow(m_pWndParent->m_hWnd)) {     // +0x80
//         CRect rect = m_rect;                                            // +0x68
//         int d = CMFCVisualManager::GetInstance()->m_nMenuShadowDepth;   // manager +0xcc
//         ::InflateRect(rect, d, d);
//         ::InvalidateRect(m_pWndParent->m_hWnd, rect, TRUE);
//         ::UpdateWindow(m_pWndParent->m_hWnd);
//     }
//     m_bToBeClosed = FALSE;                                              // +0xdc
// (Import slots resolve to USER32!IsWindow / InflateRect / InvalidateRect / UpdateWindow.
// (a) In the retail CMFCPopupMenu vftable (mfc140u 0x1802f85b0) slot +0x3a0 is
// `lea 0x230(%rcx),%rax; ret` (&m_wndMenuBar) and slot +0x3a8 calls it and returns
// bar->+0x1384 or FALSE -- the inline GetMenuBar()/InCommand() of afxpopupmenu.h.)
// DEVIATIONS: (1) the live-popup branch is not transcribed -- InCommand() and
// GetMenuBar() are non-exported CMFCPopupMenu virtuals and OpenMFC's popup menus
// carry no MSVC vtable -- so when a live popup is attached this body returns with
// nothing changed, which is exactly retail's InCommand() == TRUE outcome.  OpenMFC's
// own OpenPopupMenu is a stub, so m_pPopupMenu is only ever set by retail-layout
// callers.  (2) The shadow depth is read from ?m_pVisManager@ at the retail offset
// (0 when no manager exists; retail would create one).  OpenMFC's CMFCVisualManager
// keeps its members in a padding block that its constructor zero-fills and that
// nothing in OpenMFC stores m_nMenuShadowDepth into, so for OpenMFC managers the
// invalidated rectangle is m_rect un-inflated (retail inflates by the depth).
// Symbol: ?OnCancelMode@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCToolBarMenuButton__UEAAXXZ(CMFCToolBarMenuButton* pThis)
{
    if (pThis == nullptr) return;
    void* pPopup = MbGet<void*>(pThis, kMbPopupMenu);
    if (pPopup != nullptr && ::IsWindow(MbGet<HWND>(pPopup, kWndHWnd))) {
        return;   // DEVIATION (1) above
    }
    MbAt<void*>(pThis, kMbPopupMenu) = nullptr;
    CWnd* pParent = pThis->m_pWndParent;
    if (pParent != nullptr && ::IsWindow(pParent->m_hWnd)) {
        CRect rect = pThis->m_rect;
        const void* pVM = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
        const int nShadow = pVM != nullptr ? MbGet<int>(pVM, kVmMenuShadowDepth) : 0;
        ::InflateRect(rect, nShadow, nShadow);
        ::InvalidateRect(pParent->m_hWnd, rect, TRUE);
        ::UpdateWindow(pParent->m_hWnd);
    }
    MbAt<BOOL>(pThis, kMbToBeClosed) = FALSE;
}

// Retail RVA 0x173c30 (mfc140u), fully transcribed:
//     CMFCToolBarButton::OnChangeParentWnd(pWndParent);
//     if (pWndParent != NULL) {
//         if (pWndParent->IsKindOf(RUNTIME_CLASS(CMFCMenuBar))) {
//             m_bDrawDownArrow = (m_nID != 0 && !m_listCommands.IsEmpty())
//                                || ((CMFCMenuBar*)pWndParent)->m_bForceDownArrows;   // +0x139c
//             m_bText = TRUE;  m_bImage = FALSE;
//         } else {
//             m_bDrawDownArrow = (m_nID == 0 || !m_listCommands.IsEmpty());
//         }
//         if (pWndParent->IsKindOf(RUNTIME_CLASS(CMFCPopupMenuBar))) {
//             m_bMenuMode = TRUE;  m_bText = TRUE;  m_bImage = FALSE;
//             m_bDrawDownArrow = (m_nID == 0 || !m_listCommands.IsEmpty()) || HasButton();   // vslot +0x1f0
//         } else {
//             m_bMenuMode = FALSE;
//         }
//     }
// DEVIATION: HasButton() is MbHasButton() (see its comment above).
// Symbol: ?OnChangeParentWnd@CMFCToolBarMenuButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarMenuButton__UEAAXPEAVCWnd___Z(CMFCToolBarMenuButton* pThis, CWnd* pWndParent)
{
    if (pThis == nullptr) return;
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(pThis, pWndParent);
    if (pWndParent == nullptr) return;

    const bool bHasCommands = MbGet<INT_PTR>(pThis, kMbListCount) != 0;
    if (MbIsKindOf(pWndParent, impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ())) {
        MbAt<BOOL>(pThis, kMbDrawDownArrow) =
            ((pThis->m_nID != 0 && bHasCommands) || MbGet<BOOL>(pWndParent, kMenuBarForceDownArrows) != 0) ? TRUE : FALSE;
        pThis->m_bText = TRUE;
        pThis->m_bImage = FALSE;
    } else {
        MbAt<BOOL>(pThis, kMbDrawDownArrow) = (pThis->m_nID == 0 || bHasCommands) ? TRUE : FALSE;
    }

    if (MbIsKindOf(pWndParent, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ())) {
        MbAt<BOOL>(pThis, kMbMenuMode) = TRUE;
        pThis->m_bText = TRUE;
        pThis->m_bImage = FALSE;
        MbAt<BOOL>(pThis, kMbDrawDownArrow) =
            (pThis->m_nID == 0 || bHasCommands || MbHasButton(pThis)) ? TRUE : FALSE;
    } else {
        MbAt<BOOL>(pThis, kMbMenuMode) = FALSE;
    }
}

// Retail RVA 0x173990 (mfc140u): hit-tests m_rectArrow (+0x108) and m_rectButton
// (+0x118) with GetCursorPos + ScreenToClient + PtInRect, dispatches HasButton()
// and OpenPopupMenu() through `this`'s vtable (+0x1f0 / +0x1a8), destroys an
// existing popup through its vtable (+0xd0, DestroyWindow), and drives the owning
// bar (CMFCPopupMenuBar::StartPopupMenuTimer, CMFCToolBar::SetHot /
// GetDroppedDownMenu).  Not transcribed: it depends on popup-menu creation
// (OpenPopupMenu, itself a stub here) and on MSVC-vtable dispatch that
// OpenMFC-constructed buttons and popups do not support.
// Symbol: ?OnClick@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCToolBarMenuButton__UEAAHPEAVCWnd__H_Z(CMFCToolBarMenuButton* pThis, CWnd* pWnd, int bDelay)
{
    (void)pThis; (void)pWnd; (void)bDelay;
    return FALSE;
}

// Retail RVA 0x173010 (mfc140u): empties m_rectArrow / m_rectButton (SetRectEmpty
// on +0x108 / +0x118), then in menu mode (m_bMenuMode, +0xcc) forwards to
// DrawMenuItem; otherwise draws the button, arrow and highlight with the visual
// manager (GetInstance() is its next call).  Not transcribed (DrawMenuItem is a stub and the
// visual-manager slots are not dispatchable here).
// Symbol: ?OnDraw@CMFCToolBarMenuButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarMenuButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarMenuButton* pThis, CDC* pDC, const CRect* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons)
{
    (void)pThis; (void)pDC; (void)pRect; (void)pImages; (void)bHorz;
    (void)bCustomizeMode; (void)bHighlight; (void)bDrawBorder; (void)bGrayDisabledButtons;
}

// Retail RVA 0x175d00 (mfc140u), fully transcribed:
//     CMFCToolBarButton::OnDrawOnCustomizeList(pDC, rect, bSelected);   // result discarded
//     if (m_nID == 0 || !m_listCommands.IsEmpty() || HasButton())       // HasButton = vslot +0x1f0
//         CMFCVisualManager::GetInstance()->OnDrawMenuArrowOnCustomizeList(pDC, rect, bSelected);   // vslot +0x288
//     return rect.Width();
// DEVIATIONS: HasButton() is MbHasButton() (see above); the manager is the
// current ?m_pVisManager@ called through its exported thunk, and a NULL manager
// skips the arrow (retail's GetInstance() would create one), as in
// CMFCToolBarEditBoxButton.cpp.
// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarMenuButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarMenuButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBarMenuButton* pThis, CDC* pDC, const CRect* pRect, int bSelected)
{
    if (pThis == nullptr || pRect == nullptr) return 0;
    impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(pThis, pDC, pRect, bSelected);
    if (pThis->m_nID == 0 || MbGet<INT_PTR>(pThis, kMbListCount) != 0 || MbHasButton(pThis)) {
        if (CMFCVisualManager* pVM = static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA)) {
            impl__OnDrawMenuArrowOnCustomizeList_CMFCVisualManager__UEAAXPEAVCDC__VCRect__H_Z(pVM, pDC, CRect(*pRect), bSelected);
        }
    }
    return pRect->right - pRect->left;
}

// Retail RVA 0x1759d0 (mfc140u; slot 53 of the retail vftable): builds the menu
// through CreateMenu() (vslot +0x1b8) and the popup through CreatePopupMenu()
// (vslot +0x1c0, result stored in m_pPopupMenu at +0xc0), destroying the HMENU
// with ::DestroyMenu, and positions it from the parent's window rect.  Not transcribed: OpenMFC's CMFCPopupMenu is not constructible
// at its retail size (afxmfc.h declares a 64-byte padding block against retail's
// 0x19c8), and the creation path runs through non-exported popup virtuals.
// Symbol: ?OpenPopupMenu@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OpenPopupMenu_CMFCToolBarMenuButton__UEAAHPEAVCWnd___Z(CMFCToolBarMenuButton* pThis, CWnd* pWnd)
{
    (void)pThis; (void)pWnd;
    return FALSE;
}

// Retail RVA 0x176000 (mfc140u; slot 38 of the retail vftable), fully transcribed:
//     CMFCToolBarButton::ResetImageToDefault();
//     for (POSITION pos = m_listCommands.GetHeadPosition(); pos != NULL;)       // head @0x90
//         ((CMFCToolBarButton*)m_listCommands.GetNext(pos))->ResetImageToDefault();   // vslot +0x130
// DEVIATION: the per-command call is made by runtime class instead of through
// the vtable (OpenMFC-constructed buttons do not all carry an MSVC vtable).  The
// only two ResetImageToDefault bodies in the retail headers are CMFCToolBarButton's
// and this one, so menu buttons recurse here and everything else gets the base
// body; an override in a client-derived class is not honoured.  With OpenMFC's
// constructors the list is never populated (see the layout note at the top).
// Symbol: ?ResetImageToDefault@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarMenuButton__UEAAXXZ(CMFCToolBarMenuButton* pThis)
{
    if (pThis == nullptr) return;
    impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(pThis);
    CRuntimeClass* pMenuButtonClass = impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ();
    for (void* pNode = MbGet<void*>(pThis, kMbListHead); pNode != nullptr;) {
        void* pButton = MbGet<void*>(pNode, kNodeData);
        pNode = MbGet<void*>(pNode, kNodeNext);
        if (MbIsKindOf(pButton, pMenuButtonClass)) {
            impl__ResetImageToDefault_CMFCToolBarMenuButton__UEAAXXZ(static_cast<CMFCToolBarMenuButton*>(pButton));
        } else {
            impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(pButton);
        }
    }
}

// Retail RVA 0x175e00 (mfc140u; not in mfc140u_rva_symbols.json, but the mfc140u
// export table resolves ?SaveBarState@CMFCToolBarMenuButton@@UEAAXXZ to this RVA;
// body identical to the ANSI 0x174460), fully transcribed:
//     if (m_pWndParent == NULL) return;                                                // +0x80
//     CWnd* p = CWnd::FromHandle(::GetParent(m_pWndParent->m_hWnd));
//     if (p == NULL || !p->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))) return;
//     for (;;) {                                           // climb to the top-level popup
//         CWnd* q = CWnd::FromHandle(::GetParent(p->m_hWnd));
//         if (q == NULL || !q->IsKindOf(RUNTIME_CLASS(CMFCPopupMenu))) break;
//         p = q;
//     }
//     ((CMFCPopupMenu*)p)->SaveState();                                                // popup vslot +0x3d0
// (Import slot resolves to USER32!GetParent; slot +0x3d0 of the retail CMFCPopupMenu
// vftable is ?SaveState@CMFCPopupMenu@@UEAAXXZ.)
// DEVIATION: SaveState() is called through its exported thunk rather than the
// vtable, so an override in a client-derived popup menu is not honoured.
// Symbol: ?SaveBarState@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__SaveBarState_CMFCToolBarMenuButton__UEAAXXZ(CMFCToolBarMenuButton* pThis)
{
    if (pThis == nullptr || pThis->m_pWndParent == nullptr) return;
    CRuntimeClass* pPopupClass = impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ();
    CWnd* pMenu = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_pWndParent->m_hWnd));
    if (!MbIsKindOf(pMenu, pPopupClass)) return;
    for (;;) {
        CWnd* pUp = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pMenu->m_hWnd));
        if (!MbIsKindOf(pUp, pPopupClass)) break;
        pMenu = pUp;
    }
    impl__SaveState_CMFCPopupMenu__UEAAXXZ(pMenu);
}

// Retail RVA 0x172dd0 (mfc140u; slot 2 of the retail vftable): CMFCToolBarButton::Serialize(ar), then on load
// deletes every command in m_listCommands (RemoveHead + deleting dtor), reads
// m_uiTearOffBarID and applies it through SetTearOff (vslot +0x200), then
// m_bMenuPaletteMode (+0xe8) and m_nPaletteRows (+0xf8); on store writes those
// three; finally m_listCommands.Serialize(ar) through the CObList vtable at +0x88.
// Not transcribed: OpenMFC's layout puts m_hMenu where the embedded CObList's
// vptr lives and never constructs the list, so the list half cannot run (see the
// layout note at the top of the file).
// Symbol: ?Serialize@CMFCToolBarMenuButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarMenuButton__UEAAXAEAVCArchive___Z(CMFCToolBarMenuButton* pThis, CArchive* pAr)
{
    (void)pThis; (void)pAr;
}

// Retail RVA 0x176100 (mfc140u; slot 48 of the retail CMFCToolBarMenuButton
// vftable), fully transcribed:
//     if (!CMFCToolBarButton::SetACCData(pParent, data)) return FALSE;
//     data.m_nAccRole  = ROLE_SYSTEM_MENUITEM;                               // 0x0c
//     data.m_bAccState = STATE_SYSTEM_FOCUSED | STATE_SYSTEM_FOCUSABLE;      // 0x100004
//     if (m_nStyle & TBBS_CHECKED)  data.m_bAccState |= STATE_SYSTEM_CHECKED;      // 0x10000 -> 0x10
//     if (m_nStyle & TBBS_DISABLED) data.m_bAccState |= STATE_SYSTEM_UNAVAILABLE;  // 0x40000 -> 0x1
//     data.m_strAccHelp      = L"CMFCToolBarMenuButton";                     // SetString(s, wcslen(s))
//     data.m_strAccDefAction = m_bMenuMode ? L"Execute" : L"Open";           // +0xcc
//     return TRUE;
// (Strings read from mfc140u .rdata at 0x342dd8 / 0x33d4d8 / 0x340ce0.)
// Note that OpenMFC's base SetACCData is currently a stub returning FALSE, so
// this body returns FALSE until that is implemented -- as retail would.
// Symbol: ?SetACCData@CMFCToolBarMenuButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarMenuButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    CMFCToolBarMenuButton* pThis, CWnd* pParent, CAccessibilityData* pData)
{
    if (pThis == nullptr || pData == nullptr) return FALSE;
    if (!impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(pThis, pParent, pData)) return FALSE;
    pData->m_nAccRole = 0x0c;              // ROLE_SYSTEM_MENUITEM
    long state = 0x100004;                 // STATE_SYSTEM_FOCUSABLE | STATE_SYSTEM_FOCUSED
    if (pThis->m_nStyle & 0x10000) state = 0x100014;   // | STATE_SYSTEM_CHECKED
    if (pThis->m_nStyle & 0x40000) state |= 0x1;       // | STATE_SYSTEM_UNAVAILABLE
    pData->m_bAccState = state;
    pData->m_strAccHelp = L"CMFCToolBarMenuButton";
    pData->m_strAccDefAction = MbGet<BOOL>(pThis, kMbMenuMode) != 0 ? L"Execute" : L"Open";
    return TRUE;
}

// Retail RVA 0x175f70 (mfc140u; resolved through the mfc140u export table -- it is
// not in mfc140u_rva_symbols.json; body identical to the ANSI 0x1745d0), fully transcribed:  m_bMenuPaletteMode = bMenuPaletteMode;  m_nPaletteRows = nPaletteRows;
// Symbol: ?SetMenuPaletteMode@CMFCToolBarMenuButton@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetMenuPaletteMode_CMFCToolBarMenuButton__QEAAXHH_Z(CMFCToolBarMenuButton* pThis, int bMenuPaletteMode, int nPaletteRows)
{
    if (pThis == nullptr) return;
    MbAt<BOOL>(pThis, kMbMenuPaletteMode) = bMenuPaletteMode;
    MbAt<int>(pThis, kMbPaletteRows) = nPaletteRows;
}

// Retail RVA 0x175f80 (mfc140u; resolved through the mfc140u export table -- it is
// not in mfc140u_rva_symbols.json; body identical to the ANSI 0x1745e0, and its
// GetImageRect call targets 0x175ea0), fully transcribed:
//     m_bIsRadio = TRUE;                                        // +0xe4, unconditional
//     if (m_pWndParent != NULL) {
//         CRect rectImage;                                      // zeroed
//         GetImageRect(rectImage);                              // direct call
//         ::InvalidateRect(m_pWndParent->m_hWnd, rectImage, TRUE);
//         ::UpdateWindow(m_pWndParent->m_hWnd);
//     }
// Symbol: ?SetRadio@CMFCToolBarMenuButton@@UEAAXXZ
extern "C" void MS_ABI impl__SetRadio_CMFCToolBarMenuButton__UEAAXXZ(CMFCToolBarMenuButton* pThis)
{
    if (pThis == nullptr) return;
    MbAt<BOOL>(pThis, kMbIsRadio) = TRUE;
    if (pThis->m_pWndParent != nullptr) {
        CRect rectImage(0, 0, 0, 0);
        impl__GetImageRect_CMFCToolBarMenuButton__QEAAXAEAVCRect___Z(pThis, &rectImage);
        ::InvalidateRect(pThis->m_pWndParent->m_hWnd, rectImage, TRUE);
        ::UpdateWindow(pThis->m_pWndParent->m_hWnd);
    }
}

// Retail RVA 0x175f10 (mfc140u; slot 64 of the retail CMFCToolBarMenuButton
// vftable), transcribed:
//     if (m_uiTearOffBarID == uiBarID) return;                              // +0xf4
//     if (g_pTearOffMenuManager != NULL) {                                  // DLL-internal global
//         if (m_uiTearOffBarID != 0) g_pTearOffMenuManager->SetInUse(m_uiTearOffBarID, FALSE);
//         if (uiBarID != 0)          g_pTearOffMenuManager->SetInUse(uiBarID, TRUE);
//     }
//     m_uiTearOffBarID = uiBarID;
// DEVIATION: the tear-off manager global is not exported and OpenMFC never
// creates one (its CWinAppEx::EnableTearOffMenus only records a flag), so the
// manager branch is the retail NULL-manager path: only the ID is stored.
// Symbol: ?SetTearOff@CMFCToolBarMenuButton@@UEAAXI@Z
extern "C" void MS_ABI impl__SetTearOff_CMFCToolBarMenuButton__UEAAXI_Z(CMFCToolBarMenuButton* pThis, unsigned int uiBarID)
{
    if (pThis == nullptr) return;
    if (MbGet<UINT>(pThis, kMbTearOffBarID) == uiBarID) return;
    MbAt<UINT>(pThis, kMbTearOffBarID) = uiBarID;
}
