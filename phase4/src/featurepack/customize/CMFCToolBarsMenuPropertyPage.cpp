// CMFCToolBarsMenuPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <new>

// ===========================================================================
// CMFCToolBarsMenuPropertyPage -- the "Menu" page of the feature-pack
// Customize dialog (afxtoolbarsmenupropertypage.h:34 in the 14.51 SDK on this
// host; derives CPropertyPage, DECLARE_DYNCREATE).
//
// The bodies below were transcribed from the retail disassembly (the method
// described in the header of core/ole/COleControl.cpp).  Function bodies are
// byte-identical in mfc140.dll and mfc140u.dll; every RVA and absolute
// address quoted in this file is mfc140u.  The function entries are listed
// below.  Seven of them (OnInitDialog, OnSelchangeMenuList, SelectMenu,
// SaveMenu, OnResetMenu, OnResetFrameMenu, OnSelchangeContextMenuList) have
// no entry in mfc140u_rva_symbols.json; those were resolved from the
// export's ordinal (mfc_complete_ordinal_mapping.json) through mfc140u.dll's
// export address table, and all thirteen were re-checked that way:
//
//   ??0 (ctor)                  0x17d030     OnDestroy                   0x17df70
//   ??1 (dtor)                  0x17d2b0     OnSelchangeContextMenuList  0x17e0a0
//   CreateObject                0x17cff0     CloseContextMenu            0x17e260
//   DoDataExchange              0x17d3c0     SaveMenu                    0x17e2d0
//   OnInitDialog                0x17d520     OnResetMenu                 0x17e360
//   OnSelchangeMenuList         0x17dd40     OnResetFrameMenu            0x17e530
//   SelectMenu                  0x17e880
//
// Layout.  The class is not declared in OpenMFC's public headers, so this
// file is the only place its layout lives.  S_MenuPage pins it: the ctor
// builds CPropertyPage at +0 and the members at the offsets below, and
// CreateObject allocates 0xac0 bytes (`mov $0xac0,%ecx` before its operator
// new call).  The member names are those of afxtoolbarsmenupropertypage.h,
// whose declaration order matches the offsets.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftable 0x18031a748 at +0x00 and
//      the CStatic (0x1802e1ca8), CButton (0x1802df1b0) and CComboBox
//      (0x1802e8ea0) vftables into the controls after ??0CWnd@@QEAA@XZ.
//      OpenMFC has no MSVC-layout vtable for any of them, so +0x00 keeps what
//      the exported CPropertyPage constructor thunk leaves there (OpenMFC's
//      own C++ CPropertyPage vptr) and the controls are built with the
//      exported CWnd constructor alone, as CMFCKeyMapDialog.cpp and
//      CMFCWindowsManagerDialog.cpp do.  Consequences:
//        * the OnInitDialog / DoDataExchange thunks here are not reached
//          through the vtable, and the afx_msg handlers (OnDestroy,
//          OnSelchange*, OnReset*) are not reached through message dispatch
//          either: the C++ vptr is CPropertyPage's, and this class's own
//          message map (detail/Toolbar22MsgmapSupport.cpp) has no entries;
//        * the UpdateData calls below go to the exported CWnd::UpdateData
//          thunk, whose OpenMFC body (CWnd::UpdateData, core/window/CWnd.cpp)
//          never calls DoDataExchange at all -- it only walks the child
//          windows through UpdateDialogControls, re-setting each child's text
//          when bSaveAndValidate is TRUE -- so the DDX transfers retail
//          performs there do not happen today;
//        * virtual calls on other objects are devirtualised to the exported
//          base-class thunks, bypassing any client override:
//          CMFCMenuBar::OnChangeHot (slot 238, +0x770 = 0x8d580) and
//          CreateFromMenu (slot 271, +0x878 = 0x8af20), both read from the
//          CMFCMenuBar vftable 0x1802eef18; CDocTemplate::GetDocString
//          (slot 27, +0xd8); CWinThread::GetMainWnd (slot 31, +0xf8); and
//          CMFCPopupMenu::GetMenuBar (slot 116, +0x3a0), whose base body at
//          0x34cf0 is `lea 0x230(%rcx),%rax; ret` and is inlined here as
//          `pMenu + 0x230`.
//
//  (2) CMFCMenuBar::m_pMenuPage (+0x13b0) lies OUTSIDE OpenMFC's 0x13b0-byte
//      CMFCMenuBar (see featurepack/menu/CMFCMenuBar.cpp).  The one store to
//      it here (OnDestroy) is compiled only when sizeof(CMFCMenuBar) covers
//      it, the same guard that file's SetMenuButtonRTC uses.
//
//  (3) CMultiDocTemplate::m_hMenuShared (retail +0xf0) sits in OpenMFC's
//      unnamed _multidoctemplate_padding, which no OpenMFC body writes or
//      zeroes (CMFCMenuBar.cpp documents this).  OnSelchangeMenuList reads it
//      at the retail offset and validates it with ::IsMenu, as that file's
//      ResetImages does, treating a non-menu as NULL.
//
//      CDocTemplate::m_nIDResource is the opposite case: retail reads +0x98,
//      OpenMFC's own CDocTemplate (which built every template this page can
//      see) keeps it at +0x38, so the C++ member is read.
//
//  (4) CMFCPopupMenu.  m_pContextMenu is read with the retail layout (m_hWnd
//      +0x40, embedded m_wndMenuBar +0x230, m_hMenu +0x1658), the convention
//      featurepack/menu/CMFCPopupMenu.cpp follows.  Nothing in OpenMFC sets
//      m_pContextMenu today: the only retail writer is
//      OnSelchangeContextMenuList, which stays a stub (see there).
//
//  (5) NULL guards on `this`, and the retail-unchecked m_pContextMenu->
//      GetMenuBar() result is used as retail does (it cannot be NULL for a
//      non-NULL menu).  Retail tests no `this`.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each signature was checked against the definition
// named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);          // core/dialog/RuntimeClasses.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                             // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis);                       // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                          // core/controls/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CComboBox__UEAA_XZ(void* pThis);                        // core/controls/CtorDtorPlacement.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                          // detail/MemcoreSupport.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);       // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_CBString__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_CBIndex__YAXPEAVCDataExchange__HAEAH_Z(void* pDX, int nIDC, int* pv);         // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(void* pDX, int nIDC, int* pv);           // core/runtime/DdxExchange.cpp

extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                        // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                                // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);                                                 // core/window/Thunks.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                                           // core/app/Globals.cpp
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);                          // core/app/CWinThread.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                                 // detail/MfcExceptionsSupport.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszResource, const wchar_t* lpszType);   // core/runtime/Globals.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);                // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" int   MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4DocStringIndex_1__Z(const CDocTemplate* pThis, CString* rString, int nID);   // core/doc/CDocTemplate.cpp

extern "C" void  MS_ABI impl__OnChangeHot_CMFCMenuBar__UEAAXH_Z(CMFCMenuBar* pThis, int iHot);                      // featurepack/menu/CMFCMenuBar.cpp
extern "C" void  MS_ABI impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(CMFCMenuBar* pThis, HMENU hMenu, int bDefaultMenu, int bForceUpdate);   // featurepack/menu/CMFCMenuBar.cpp
extern "C" int   MS_ABI impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU hMenu, CMFCToolBar* pBar);   // core/window/CMenuHash.cpp

// Exported data objects, read and written the way retail does.
extern "C" unsigned char impl__afxMenuHash__3VCMenuHash__A[128];                        // featurepack/CMFC_misc_stubs.cpp (data 0x3b1d10)
extern "C" std::uint32_t impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A;    // featurepack/menu/StaticData.cpp (data 0x3be290)
extern "C" std::int32_t  impl__m_bMenuShadows_CMFCMenuBar__1HA;                         // featurepack/menu/StaticData.cpp
extern "C" POINT impl__m_ptMenuLastPos_CMFCToolBarsMenuPropertyPage__1VCPoint__A;       // defined at the end of this file

// This file's own thunks that other bodies here call (retail calls them
// directly, not through the vtable).
extern "C" void* MS_ABI impl___0CMFCToolBarsMenuPropertyPage__QEAA_PEAVCFrameWnd__H_Z(void* pThis, CFrameWnd* pParentFrame, int bAutoSet);
extern "C" void MS_ABI impl__SaveMenu_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnSelchangeMenuList_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// The retail object (0xac0 bytes); offsets from the constructor (0x17d030).
// ---------------------------------------------------------------------------
struct S_MenuPage {
    alignas(8) unsigned char m_base[0x158];                    // +0x000 CPropertyPage (OpenMFC sizeof == 0x158, asserted below)
    alignas(8) unsigned char m_wndMenuAnimationsLabel[0xe8];   // +0x158 CStatic
    alignas(8) unsigned char m_wndContextHint[0xe8];           // +0x240 CStatic
    alignas(8) unsigned char m_wndContextMenuCaption[0xe8];    // +0x328 CStatic
    alignas(8) unsigned char m_wndIcon[0xe8];                  // +0x410 CStatic
    alignas(8) unsigned char m_wndMenuShadows[0xe8];           // +0x4f8 CButton
    alignas(8) unsigned char m_wndContextFrame[0xe8];          // +0x5e0 CButton
    alignas(8) unsigned char m_wndResetMenuButton[0xe8];       // +0x6c8 CButton
    alignas(8) unsigned char m_wndMenuAnimations[0xe8];        // +0x7b0 CComboBox
    alignas(8) unsigned char m_wndContextMenus[0xe8];          // +0x898 CComboBox
    alignas(8) unsigned char m_wndMenuesList[0xe8];            // +0x980 CComboBox
    alignas(8) unsigned char m_strMenuDescr[8];                // +0xa68 CString
    alignas(8) unsigned char m_strContextMenuName[8];          // +0xa70 CString
    alignas(8) unsigned char m_strMenuName[8];                 // +0xa78 CString
    int            m_iMenuAnimationType;                       // +0xa80
    int            m_bMenuShadows;                             // +0xa84
    CMFCMenuBar*   m_pMenuBar;                                 // +0xa88
    void*          m_pContextMenu;                             // +0xa90 CMFCPopupMenu* (retail layout, deviation 4)
    CFrameWnd*     m_pParentFrame;                             // +0xa98
    HMENU          m_hmenuCurr;                                // +0xaa0
    HMENU          m_hmenuSelected;                            // +0xaa8
    UINT           m_uiContextMenuResId;                       // +0xab0
    int            m_bIsDefaultMDIMenu;                        // +0xab4
    int            m_bAutoSet;                                 // +0xab8
};
static_assert(offsetof(S_MenuPage, m_wndMenuAnimationsLabel) == 0x158, "ctor: lea 0x158(%r15),%rbx; CWnd ctor");
static_assert(offsetof(S_MenuPage, m_wndContextHint) == 0x240, "ctor: lea 0x240(%r15)");
static_assert(offsetof(S_MenuPage, m_wndContextMenuCaption) == 0x328, "ctor: lea 0x328(%r15)");
static_assert(offsetof(S_MenuPage, m_wndIcon) == 0x410, "ctor: lea 0x410(%r15)");
static_assert(offsetof(S_MenuPage, m_wndMenuShadows) == 0x4f8, "ctor: lea 0x4f8(%r15), then CButton vftable");
static_assert(offsetof(S_MenuPage, m_wndContextFrame) == 0x5e0, "ctor: lea 0x5e0(%r15)");
static_assert(offsetof(S_MenuPage, m_wndResetMenuButton) == 0x6c8, "ctor: lea 0x6c8(%r15)");
static_assert(offsetof(S_MenuPage, m_wndMenuAnimations) == 0x7b0, "ctor: lea 0x7b0(%r15), then CComboBox vftable");
static_assert(offsetof(S_MenuPage, m_wndContextMenus) == 0x898, "ctor: lea 0x898(%r15)");
static_assert(offsetof(S_MenuPage, m_wndMenuesList) == 0x980, "ctor: lea 0x980(%r15)");
static_assert(offsetof(S_MenuPage, m_strMenuDescr) == 0xa68, "ctor: lea 0xa68(%r15),%rsi (nil CString)");
static_assert(offsetof(S_MenuPage, m_strContextMenuName) == 0xa70, "ctor: lea 0xa70(%r15),%rdi");
static_assert(offsetof(S_MenuPage, m_strMenuName) == 0xa78, "ctor: lea 0xa78(%r15),%rbx");
static_assert(offsetof(S_MenuPage, m_iMenuAnimationType) == 0xa80, "ctor: mov %eax,0xa80(%r15)");
static_assert(offsetof(S_MenuPage, m_bMenuShadows) == 0xa84, "ctor: mov %eax,0xa84(%r15)");
static_assert(offsetof(S_MenuPage, m_pMenuBar) == 0xa88, "ctor: mov %rax,0xa88(%r15)");
static_assert(offsetof(S_MenuPage, m_pContextMenu) == 0xa90, "ctor: mov %rax,0xa90(%r15)");
static_assert(offsetof(S_MenuPage, m_pParentFrame) == 0xa98, "ctor: mov %rbp,0xa98(%r15)");
static_assert(offsetof(S_MenuPage, m_hmenuCurr) == 0xaa0, "ctor: mov %rax,0xaa0(%r15)");
static_assert(offsetof(S_MenuPage, m_hmenuSelected) == 0xaa8, "ctor: mov %rax,0xaa8(%r15)");
static_assert(offsetof(S_MenuPage, m_uiContextMenuResId) == 0xab0, "ctor: 8-byte store at 0xab0 (with m_bIsDefaultMDIMenu)");
static_assert(offsetof(S_MenuPage, m_bIsDefaultMDIMenu) == 0xab4, "afx header order: after m_uiContextMenuResId");
static_assert(offsetof(S_MenuPage, m_bAutoSet) == 0xab8, "ctor: mov %r14d,0xab8(%r15)");
static_assert(sizeof(S_MenuPage) == 0xac0, "CreateObject: mov $0xac0,%ecx");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CStatic) == 0xe8 && sizeof(CButton) == 0xe8 && sizeof(CComboBox) == 0xe8,
              "the plain controls are bare CWnds");
static_assert(sizeof(CString) == 8, "one CStringT data pointer");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads control / popup HWNDs at +0x40");
// NOTE: retail reads CDocTemplate::m_nIDResource at +0x98; OpenMFC's
// CDocTemplate keeps it at +0x38.  The templates reaching this page are built
// by OpenMFC's exported constructors, so the C++ member is read (see
// OnSelchangeMenuList).

// Retail CMFCMenuBar members (featurepack/menu/CMFCMenuBar.cpp pins them).
constexpr int kOffMenuBarHMenu        = 0x1360;   // HMENU m_hMenu
constexpr int kOffMenuBarDefaultMenu  = 0x1368;   // HMENU m_hDefaultMenu
constexpr int kOffMenuBarMenuPage     = 0x13b0;   // CMFCToolBarsMenuPropertyPage* m_pMenuPage (deviation 2)
// Retail CMultiDocTemplate::m_hMenuShared (deviation 3).
constexpr int kOffTemplateMenuShared  = 0xf0;
// Retail CMFCPopupMenu (afxpopupmenu.h order; deviation 4).
constexpr int kOffPopupHWnd           = 0x40;     // CWnd::m_hWnd
constexpr int kOffPopupMenuBar        = 0x230;    // CMFCPopupMenuBar m_wndMenuBar (GetMenuBar body 0x34cf0)
constexpr int kOffPopupHMenu          = 0x1658;   // HMENU m_hMenu

// Resource ids (afxribbonres.h in the 14.51 SDK on this host).
constexpr UINT kIddPropPage3                 = 0x3f06;   // IDD_AFXBARRES_PROPPAGE3
constexpr int  kIdcMenuDescription           = 0x407a;   // IDC_AFXBARRES_MENU_DESCRIPTION
constexpr int  kIdcTemplIcon                 = 0x407b;   // IDC_AFXBARRES_TEMPL_ICON
constexpr int  kIdcContextMenuCaption        = 0x407c;   // IDC_AFXBARRES_CONTEXT_MENU_CAPTION
constexpr int  kIdcContextHint               = 0x4087;   // IDC_AFXBARRES_CONTEXT_HINT
constexpr int  kIdcContextFrame              = 0x4088;   // IDC_AFXBARRES_CONTEXT_FRAME
constexpr int  kIdcMenuAnimationLabel        = 0x409f;   // IDC_AFXBARRES_MENU_ANIMATION_LABEL
constexpr int  kIdcMenuList                  = 0x40ed;   // IDC_AFXBARRES_MENU_LIST
constexpr int  kIdcContextMenuList           = 0x40ee;   // IDC_AFXBARRES_CONTEXT_MENU_LIST
constexpr int  kIdcResetMenu                 = 0x40ef;   // IDC_AFXBARRES_RESET_MENU
constexpr int  kIdcMenuAnimation             = 0x40f1;   // IDC_AFXBARRES_MENU_ANIMATION
constexpr int  kIdcMenuShadows               = 0x40f2;   // IDC_AFXBARRES_MENU_SHADOWS
constexpr UINT kIdsDefaultMenuDescr          = 0x3eeb;   // IDS_AFXBARRES_DEFAULT_MENU_DESCR
constexpr std::uint32_t kSystemDefaultAnimation = 999;   // CMFCPopupMenu::SYSTEM_DEFAULT_ANIMATION (`mov $0x3e7,%ecx`)
constexpr UINT kStmSetIcon                   = 0x170;    // STM_SETICON

inline S_MenuPage* D(void* p) { return static_cast<S_MenuPage*>(p); }
inline CWnd* Self(void* p) { return static_cast<CWnd*>(p); }
inline CWnd* Wnd(unsigned char* storage) { return reinterpret_cast<CWnd*>(storage); }
inline HWND Hwnd(unsigned char* storage) { return Wnd(storage)->m_hWnd; }
inline CString* Str(unsigned char* storage) { return reinterpret_cast<CString*>(storage); }

template <typename T>
inline T& At(void* p, int off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off); }

inline CMenuHash* MenuHash() { return reinterpret_cast<CMenuHash*>(impl__afxMenuHash__3VCMenuHash__A); }
inline CMFCToolBar* AsToolBar(CMFCMenuBar* p) { return reinterpret_cast<CMFCToolBar*>(p); }

// ENSURE(x): retail throws through ?AfxThrowInvalidArgException@@.
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// CStringT::LoadString(UINT) as retail inlines it (AfxFindStringResourceHandle,
// then the exported LoadString(HINSTANCE, UINT)), under ENSURE: a missing
// resource or a failed load throws.
bool EnsureLoadString(CString* str, UINT nID) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(str, hInst, nID)) {
        ThrowInvalidArg();
        return false;
    }
    return true;
}

} // namespace

// Retail (RVA 0x17d030, mfc140u), transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE3 /*0x3f06*/, 0,
//                                  /*dwSize*/ 0x68);                  // 0x215910
//     vfptr = 0x18031a748;
//     ten controls: CWnd::CWnd() (0x28a700) then the CStatic / CButton /
//         CComboBox vftable, in declaration order (+0x158 .. +0x980);
//     m_strMenuDescr / m_strContextMenuName / m_strMenuName: nil string, then
//         SetString("", 0) -- i.e. empty;
//     m_pParentFrame = pParentFrame;  m_bAutoSet = bAutoSet;
//     m_iMenuAnimationType = CMFCPopupMenu::m_AnimationType;
//     m_bMenuShadows = CMFCMenuBar::m_bMenuShadows;
//     m_pMenuBar = m_hmenuCurr = m_hmenuSelected = m_pContextMenu = NULL;
//     m_uiContextMenuResId = m_bIsDefaultMDIMenu = 0   (one 8-byte store)
// DEVIATION: the vftable stores are deviation (1).
// Symbol: ??0CMFCToolBarsMenuPropertyPage@@QEAA@PEAVCFrameWnd@@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsMenuPropertyPage__QEAA_PEAVCFrameWnd__H_Z(void* pThis, CFrameWnd* pParentFrame, int bAutoSet) {
    if (pThis == nullptr) return nullptr;
    S_MenuPage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage3, 0, 0x68);
    for (unsigned char* w : {d->m_wndMenuAnimationsLabel, d->m_wndContextHint, d->m_wndContextMenuCaption, d->m_wndIcon,
                             d->m_wndMenuShadows, d->m_wndContextFrame, d->m_wndResetMenuButton,
                             d->m_wndMenuAnimations, d->m_wndContextMenus, d->m_wndMenuesList}) {
        impl___0CWnd__QEAA_XZ(w);
    }
    new (d->m_strMenuDescr) CString();
    new (d->m_strContextMenuName) CString();
    new (d->m_strMenuName) CString();
    d->m_pParentFrame = pParentFrame;
    d->m_bAutoSet = bAutoSet;
    d->m_iMenuAnimationType = static_cast<int>(impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A);
    d->m_bMenuShadows = impl__m_bMenuShadows_CMFCMenuBar__1HA;
    d->m_pMenuBar = nullptr;
    d->m_hmenuCurr = nullptr;
    d->m_hmenuSelected = nullptr;
    d->m_pContextMenu = nullptr;
    d->m_uiContextMenuResId = 0;
    d->m_bIsDefaultMDIMenu = 0;
    return pThis;
}

// Retail (RVA 0x17d2b0, mfc140u), transcribed: re-install the class vftable
// (deviation 1), release the three CStrings in reverse order (+0xa78, +0xa70,
// +0xa68), then ~CComboBox on +0x980/+0x898/+0x7b0, ~CButton on
// +0x6c8/+0x5e0/+0x4f8, ~CStatic on +0x410/+0x328/+0x240/+0x158, and a tail
// jump to ~CPropertyPage.
// Symbol: ??1CMFCToolBarsMenuPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsMenuPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MenuPage* d = D(pThis);
    Str(d->m_strMenuName)->~CString();
    Str(d->m_strContextMenuName)->~CString();
    Str(d->m_strMenuDescr)->~CString();
    impl___1CComboBox__UEAA_XZ(d->m_wndMenuesList);
    impl___1CComboBox__UEAA_XZ(d->m_wndContextMenus);
    impl___1CComboBox__UEAA_XZ(d->m_wndMenuAnimations);
    impl___1CButton__UEAA_XZ(d->m_wndResetMenuButton);
    impl___1CButton__UEAA_XZ(d->m_wndContextFrame);
    impl___1CButton__UEAA_XZ(d->m_wndMenuShadows);
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndIcon));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndContextMenuCaption));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndContextHint));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndMenuAnimationsLabel));
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Symbol: ?CloseContextMenu@CMFCToolBarsMenuPropertyPage@@QEAAXPEAVCMFCPopupMenu@@@Z
// Retail (RVA 0x17e260, mfc140u), fully transcribed:
//     if (m_pContextMenu == NULL) return;
//     ENSURE(m_pContextMenu == pMenu);                    // else AfxThrowInvalidArgException
//     SaveMenu();
//     if (m_pContextMenu != NULL) {
//         m_pContextMenu = NULL;
//         ::SendMessage(m_wndContextMenus.m_hWnd, CB_SETCURSEL, 0, 0);
//         m_wndResetMenuButton.EnableWindow(FALSE);
//     }
//     m_uiContextMenuResId = 0;
extern "C" void MS_ABI impl__CloseContextMenu_CMFCToolBarsMenuPropertyPage__QEAAXPEAVCMFCPopupMenu___Z(void* pThis, void* pMenu) {
    if (pThis == nullptr) return;
    S_MenuPage* d = D(pThis);
    if (d->m_pContextMenu == nullptr) return;
    if (d->m_pContextMenu != pMenu) {
        ThrowInvalidArg();
        return;
    }
    impl__SaveMenu_CMFCToolBarsMenuPropertyPage__IEAAXXZ(pThis);
    if (d->m_pContextMenu != nullptr) {
        d->m_pContextMenu = nullptr;
        ::SendMessage(Hwnd(d->m_wndContextMenus), CB_SETCURSEL, 0, 0);
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndResetMenuButton), FALSE);
    }
    d->m_uiContextMenuResId = 0;
}

// Retail (RVA 0x17cff0, mfc140u): `new` 0xac0 bytes (operator new, 0x27f0)
// and, when that is non-NULL, run the constructor with (NULL, FALSE).
// Symbol: ?CreateObject@CMFCToolBarsMenuPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarsMenuPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_MenuPage));
    if (p != nullptr) impl___0CMFCToolBarsMenuPropertyPage__QEAA_PEAVCFrameWnd__H_Z(p, nullptr, FALSE);
    return p;
}

// Retail (RVA 0x17d3c0, mfc140u), fully transcribed -- no CPropertyPage::
// DoDataExchange call, the DDX calls in this order, the last a tail jump:
//     DDX_Control(pDX, 16543 MENU_ANIMATION_LABEL, m_wndMenuAnimationsLabel);
//     DDX_Control(pDX, 16625 MENU_ANIMATION,       m_wndMenuAnimations);
//     DDX_Control(pDX, 16626 MENU_SHADOWS,         m_wndMenuShadows);
//     DDX_Control(pDX, 16520 CONTEXT_FRAME,        m_wndContextFrame);
//     DDX_Control(pDX, 16519 CONTEXT_HINT,         m_wndContextHint);
//     DDX_Control(pDX, 16623 RESET_MENU,           m_wndResetMenuButton);
//     DDX_Control(pDX, 16508 CONTEXT_MENU_CAPTION, m_wndContextMenuCaption);
//     DDX_Control(pDX, 16622 CONTEXT_MENU_LIST,    m_wndContextMenus);
//     DDX_Control(pDX, 16507 TEMPL_ICON,           m_wndIcon);
//     DDX_Control(pDX, 16621 MENU_LIST,            m_wndMenuesList);
//     DDX_Text    (pDX, 16506 MENU_DESCRIPTION,    m_strMenuDescr);
//     DDX_CBString(pDX, 16622 CONTEXT_MENU_LIST,   m_strContextMenuName);
//     DDX_CBString(pDX, 16621 MENU_LIST,           m_strMenuName);
//     DDX_CBIndex (pDX, 16625 MENU_ANIMATION,      m_iMenuAnimationType);
//     DDX_Check   (pDX, 16626 MENU_SHADOWS,        m_bMenuShadows);
// (Not reached through UpdateData today -- deviation 1.)
// Symbol: ?DoDataExchange@CMFCToolBarsMenuPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarsMenuPropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_MenuPage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMenuAnimationLabel, d->m_wndMenuAnimationsLabel);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMenuAnimation, d->m_wndMenuAnimations);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMenuShadows, d->m_wndMenuShadows);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcContextFrame, d->m_wndContextFrame);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcContextHint, d->m_wndContextHint);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcResetMenu, d->m_wndResetMenuButton);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcContextMenuCaption, d->m_wndContextMenuCaption);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcContextMenuList, d->m_wndContextMenus);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcTemplIcon, d->m_wndIcon);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMenuList, d->m_wndMenuesList);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcMenuDescription, d->m_strMenuDescr);
    impl__DDX_CBString__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcContextMenuList, d->m_strContextMenuName);
    impl__DDX_CBString__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcMenuList, d->m_strMenuName);
    impl__DDX_CBIndex__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcMenuAnimation, &d->m_iMenuAnimationType);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcMenuShadows, &d->m_bMenuShadows);
}

// Retail (RVA 0x17df70, mfc140u), transcribed:
//     UpdateData(TRUE);
//     int nCount = ::SendMessage(m_wndMenuAnimations.m_hWnd, CB_GETCOUNT, 0, 0);
//     CMFCPopupMenu::m_AnimationType = (m_iMenuAnimationType == nCount - 1)
//         ? SYSTEM_DEFAULT_ANIMATION /*999*/ : m_iMenuAnimationType;
//     if (m_pMenuBar != NULL) {
//         m_pMenuBar->m_pMenuPage /*+0x13b0*/ = NULL;
//         if (m_hmenuSelected != NULL) {
//             m_pMenuBar->OnChangeHot(-1);                        // vslot +0x770
//             afxMenuHash.SaveMenuBar(m_hmenuSelected, m_pMenuBar);
//         }
//         if (m_hmenuCurr != NULL)
//             m_pMenuBar->CreateFromMenu(m_hmenuCurr, FALSE, FALSE);   // vslot +0x878
//     }
//     if (m_pContextMenu != NULL) {
//         ::SendMessage(m_pContextMenu->GetMenuBar()->m_hWnd, WM_CANCELMODE, 0, 0);
//         SaveMenu();
//         ::SendMessage(m_pContextMenu->m_hWnd, WM_CLOSE, 0, 0);
//     }
//     CMFCMenuBar::m_bMenuShadows = m_bMenuShadows;
//     CWnd::OnDestroy();                 // tail jump to 0x28b840 (afxdlgs.h declares no OnDestroy for CPropertyPage/CDialog)
// DEVIATIONS: (1) for the UpdateData transfer and the two devirtualised
// CMFCMenuBar calls; (2) for the m_pMenuPage store.
// Symbol: ?OnDestroy@CMFCToolBarsMenuPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MenuPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);

    const int nCount = static_cast<int>(::SendMessage(Hwnd(d->m_wndMenuAnimations), CB_GETCOUNT, 0, 0));
    impl__m_AnimationType_CMFCPopupMenu__1W4ANIMATION_TYPE_1_A =
        (d->m_iMenuAnimationType == nCount - 1) ? kSystemDefaultAnimation
                                                : static_cast<std::uint32_t>(d->m_iMenuAnimationType);

    if (d->m_pMenuBar != nullptr) {
        if constexpr (sizeof(CMFCMenuBar) >= kOffMenuBarMenuPage + static_cast<int>(sizeof(void*))) {
            At<void*>(d->m_pMenuBar, kOffMenuBarMenuPage) = nullptr;
        }
        if (d->m_hmenuSelected != nullptr) {
            impl__OnChangeHot_CMFCMenuBar__UEAAXH_Z(d->m_pMenuBar, -1);
            impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(MenuHash(), d->m_hmenuSelected, AsToolBar(d->m_pMenuBar));
        }
        if (d->m_hmenuCurr != nullptr) {
            impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(d->m_pMenuBar, d->m_hmenuCurr, FALSE, FALSE);
        }
    }

    if (d->m_pContextMenu != nullptr) {
        void* pBar = static_cast<unsigned char*>(d->m_pContextMenu) + kOffPopupMenuBar;
        ::SendMessage(At<HWND>(pBar, kOffPopupHWnd), WM_CANCELMODE, 0, 0);
        impl__SaveMenu_CMFCToolBarsMenuPropertyPage__IEAAXXZ(pThis);
        ::SendMessage(At<HWND>(d->m_pContextMenu, kOffPopupHWnd), WM_CLOSE, 0, 0);
    }

    impl__m_bMenuShadows_CMFCMenuBar__1HA = d->m_bMenuShadows;
    impl__OnDestroy_CWnd__IEAAXXZ(Self(pThis));
}

// Retail (RVA 0x17d520, mfc140u) was disassembled but not transcribed.  It
// calls CDialog::OnInitDialog, then (while m_pMenuBar is NULL) walks the
// global toolbar list whose head pointer is at 0x1803b2098 (mfc140u), taking
// the first bar with CWnd::FromHandlePermanent(m_hWnd) != NULL and
// IsKindOf(RUNTIME_CLASS(CMFCMenuBar)) (no menu bar: it disables the menu
// controls, loads string 0x3e87 into m_strMenuDescr and skips to the
// context-menu part); stores `this` into that bar's m_pMenuPage (+0x13b0),
// sets m_hmenuCurr from the bar's m_hMenu and re-saves the bar in
// afxMenuHash; when m_bAutoSet is set, fills m_wndMenuesList from the
// application's CDocManager (vslots +0x30 / +0x38 on m_pDocManager, keeping
// templates that are IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)) with a
// non-NULL m_hMenuShared at +0xf0 and no earlier entry with the same
// m_nIDResource); fills the animation combo and, from
// afxContextMenuManager->GetMenuNames, the context-menu combo; adjusts the
// dialog controls; and returns TRUE.  The inputs are missing or laid out
// differently in this tree: m_pMenuPage lies outside OpenMFC's CMFCMenuBar
// (deviation 2), m_hMenuShared is never written (deviation 3),
// afxContextMenuManager has no OpenMFC counterpart global (the nearest
// substitute, CurrentContextMenuManager, is file-local to
// core/frame/CMDIFrameWndEx.cpp), and OpenMFC has no counterpart of the
// private all-toolbars CObList whose head retail reads directly (OpenMFC's
// CObList keeps its contents in a side table, and CMFCToolBar.cpp substitutes
// its toolbar side table for that list).  Left a stub (returns FALSE where
// retail returns TRUE; the placeholder parameter list was corrected to take
// `this`).
// Symbol: ?OnInitDialog@CMFCToolBarsMenuPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsMenuPropertyPage__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Retail (RVA 0x17e530, mfc140u) was disassembled but not transcribed.  It
// does UpdateData(TRUE), returns when m_pMenuBar is NULL or the menu list has
// no selection, and asks for confirmation (CString::Format with
// IDS_AFXBARRES_RESET_MENU_FMT and m_strMenuName, then AfxMessageBox with
// MB_YESNO|MB_ICONQUESTION; anything but IDYES returns).  For a template
// entry it WRITES ::LoadMenu(AfxFindResourceHandle(nID, RT_MENU), nID) into
// the template's m_hMenuShared (+0xf0), rebuilds the bar (CreateFromMenu,
// CMFCMenuBar::UpdateMDIChildrenMenus), sends the registered
// AFX_WM_RESETMENU to m_pParentFrame when it is set (a non-zero result calls
// m_pMenuBar->AdjustLayout(), bar vslot +0x428) and re-saves the bar in
// afxMenuHash, moving m_hmenuCurr to the new menu when it was the old one.
// For the default-menu entry (and only when m_pMenuBar's m_uiDefMenuResId at
// +0x1380 is non-zero) it reloads that menu, calls bar vslot +0x888 and
// CreateFromMenu(hMenu, TRUE, FALSE), and, when m_pParentFrame is set, sends
// the same message (same AdjustLayout rule) and WRITES the new menu into
// m_pParentFrame's +0xf0; it then re-saves the bar and, when
// m_bIsDefaultMDIMenu is set, makes it m_hmenuCurr.  Finally, when
// m_pParentFrame is set it calls the frame's vslot +0x360 with m_hmenuCurr;
// when there was an old menu it ENSUREs ::IsMenu on it before
// CMenuHash::RemoveMenu + ::DestroyMenu; and it ends with ::InvalidateRect +
// ::UpdateWindow on the bar.  In OpenMFC's layouts +0xf0 of CMultiDocTemplate is unused padding and
// +0xf0 of CFrameWnd is m_hAccelTable, so those two stores would corrupt live
// state.  Left a stub.
// Symbol: ?OnResetFrameMenu@CMFCToolBarsMenuPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetFrameMenu_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x17e360, mfc140u) was disassembled but not transcribed.  It
// returns when afxContextMenuManager is NULL, ENSUREs m_pContextMenu != NULL,
// asks for confirmation (CString::Format with IDS_AFXBARRES_RESET_MENU_FMT and
// m_strContextMenuName, AfxMessageBox MB_YESNO|MB_ICONQUESTION, anything but
// IDYES returns), looks the menu up with GetMenuByName (MessageBeep and return
// on NULL), passes ::GetSubMenu(hMenu, 0) to vslot +0x880 of the popup's
// embedded CMFCPopupMenuBar, sends the registered AFX_WM_RESETCONTEXTMENU
// (m_uiContextMenuResId, m_pContextMenu) to m_pParentFrame when it is set,
// calls OnSelchangeContextMenuList, then the popup's vslot +0x300 with TRUE
// and ::InvalidateRect on the embedded bar.  It needs the retail-layout
// context popup that only OnSelchangeContextMenuList creates (a stub, below),
// OpenMFC has no afxContextMenuManager global, and the embedded
// CMFCPopupMenuBar is not modeled.  Left a stub.
// Symbol: ?OnResetMenu@CMFCToolBarsMenuPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetMenu_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x17e0a0, mfc140u) was disassembled but not transcribed:
//     m_wndResetMenuButton.EnableWindow(FALSE);
//     if (afxContextMenuManager == NULL) return;
//     m_uiContextMenuResId = 0;
//     if (m_pContextMenu != NULL) { SaveMenu(); p = m_pContextMenu;
//         m_pContextMenu = NULL; ::SendMessage(p->m_hWnd, WM_CLOSE, 0, 0); }
//     if (::SendMessage(m_wndContextMenus.m_hWnd, CB_GETCURSEL, 0, 0) <= 0) return;
//     UpdateData(TRUE);
//     HMENU hMenu = afxContextMenuManager->GetMenuByName(m_strContextMenuName,
//                                                        &m_uiContextMenuResId);
//     if (hMenu == NULL) { ::MessageBeep((UINT)-1); return; }
//     HMENU hPopup = ::GetSubMenu(hMenu, 0);  if (hPopup == NULL) return;
//     m_pContextMenu = new CMFCPopupMenu(this, m_strContextMenuName);  // 0x19c8 bytes
//     ENSURE(m_pContextMenu != NULL);
//     m_pContextMenu->m_bAutoDestroy /*+0x1660*/ = FALSE;
//     if (!m_pContextMenu->Create(GetTopLevelFrame(), m_ptMenuLastPos.x,
//                                 m_ptMenuLastPos.y, hPopup, FALSE, FALSE))  // vslot +0x430
//         AfxMessageBox("Can't create context menu!");
//     else
//         m_wndResetMenuButton.EnableWindow(TRUE);
// Not implemented: the popup it builds cannot be built in this tree.
// OpenMFC's ??0CMFCPopupMenu@@IEAA@PEAVCMFCToolBarsMenuPropertyPage@@PEB_W@Z
// thunk (featurepack/menu/CMFCPopupMenu.cpp) has a parameter list that drops
// `this` and returns a fresh `new CMFCPopupMenu()` of OpenMFC's 0x238-byte
// clean-room layout instead of constructing the 0x19c8-byte retail object,
// and afxContextMenuManager has no OpenMFC counterpart global.  Left a stub.
// Symbol: ?OnSelchangeContextMenuList@CMFCToolBarsMenuPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeContextMenuList_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x17dd40, mfc140u), transcribed:
//     UpdateData(TRUE);
//     if (m_pMenuBar == NULL) return;
//     if (m_hmenuSelected != NULL) {
//         m_pMenuBar->OnChangeHot(-1);                                   // vslot +0x770
//         afxMenuHash.SaveMenuBar(m_hmenuSelected, m_pMenuBar);
//     }
//     int iSel = ::SendMessage(m_wndMenuesList.m_hWnd, CB_GETCURSEL, 0, 0);
//     if (iSel == CB_ERR) {
//         m_strMenuDescr = "";
//         ::SendMessage(m_wndIcon.m_hWnd, STM_SETICON, 0, 0);
//         UpdateData(FALSE);
//         m_hmenuSelected = NULL;
//         return;
//     }
//     CMultiDocTemplate* pTemplate =
//         (CMultiDocTemplate*)::SendMessage(<menu list>, CB_GETITEMDATA, iSel, 0);
//     HMENU hMenu; HICON hIcon;
//     if (pTemplate != NULL) {
//         pTemplate->GetDocString(m_strMenuDescr, CDocTemplate::fileNewName);  // vslot +0xd8, index 6
//         UINT nID = LOWORD(pTemplate->m_nIDResource);                       // +0x98
//         hIcon = ::LoadIcon(AfxFindResourceHandle(MAKEINTRESOURCE(nID), RT_GROUP_ICON),
//                            MAKEINTRESOURCE(nID));
//         if (hIcon == NULL) hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
//         hMenu = pTemplate->m_hMenuShared;                                  // +0xf0
//     } else {
//         ENSURE(m_strMenuDescr.LoadString(IDS_AFXBARRES_DEFAULT_MENU_DESCR));
//         hIcon = NULL;
//         CWnd* pMain = AfxGetMainWnd();                                     // inlined
//         if (pMain != NULL) hIcon = (HICON)::GetClassLongPtr(pMain->m_hWnd, GCLP_HICON);
//         hMenu = m_pMenuBar->m_hDefaultMenu;                                // +0x1368
//     }
//     ENSURE(hMenu != NULL);
//     m_pMenuBar->CreateFromMenu(hMenu, FALSE, FALSE);                       // vslot +0x878
//     ::SendMessage(m_wndIcon.m_hWnd, STM_SETICON, (WPARAM)hIcon, 0);
//     m_hmenuSelected = hMenu;
//     UpdateData(FALSE);
// The template branch also calls AfxGetModuleState() and discards the result
// (the call at 0x17de54, between GetDocString and the +0x98 load); that call
// has no effect here and is not reproduced.
// DEVIATIONS: (1) for UpdateData and the devirtualised calls; (3) for
// m_hMenuShared, which is only accepted when ::IsMenu says it is a menu.
// Symbol: ?OnSelchangeMenuList@CMFCToolBarsMenuPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeMenuList_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MenuPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    if (d->m_pMenuBar == nullptr) return;

    if (d->m_hmenuSelected != nullptr) {
        impl__OnChangeHot_CMFCMenuBar__UEAAXH_Z(d->m_pMenuBar, -1);
        impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(MenuHash(), d->m_hmenuSelected, AsToolBar(d->m_pMenuBar));
    }

    const HWND hList = Hwnd(d->m_wndMenuesList);
    const int iSel = static_cast<int>(::SendMessage(hList, CB_GETCURSEL, 0, 0));
    if (iSel == CB_ERR) {
        *Str(d->m_strMenuDescr) = L"";
        ::SendMessage(Hwnd(d->m_wndIcon), kStmSetIcon, 0, 0);
        impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), FALSE);
        d->m_hmenuSelected = nullptr;
        return;
    }

    CDocTemplate* pTemplate = reinterpret_cast<CDocTemplate*>(::SendMessage(hList, CB_GETITEMDATA, static_cast<WPARAM>(iSel), 0));
    HMENU hMenu = nullptr;
    HICON hIcon = nullptr;
    if (pTemplate != nullptr) {
        impl__GetDocString_CDocTemplate__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__W4DocStringIndex_1__Z(
            pTemplate, Str(d->m_strMenuDescr), 6 /* CDocTemplate::fileNewName: `mov $0x6,%r8d` */);
        const WORD nID = LOWORD(pTemplate->m_nIDResource);   // retail +0x98; OpenMFC member (deviation 3)
        HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(MAKEINTRESOURCEW(nID), RT_GROUP_ICON);
        hIcon = ::LoadIcon(hInst, MAKEINTRESOURCE(nID));
        if (hIcon == nullptr) hIcon = ::LoadIcon(nullptr, IDI_APPLICATION);
        hMenu = At<HMENU>(pTemplate, kOffTemplateMenuShared);
        if (hMenu != nullptr && !::IsMenu(hMenu)) hMenu = nullptr;   // deviation 3
    } else {
        if (!EnsureLoadString(Str(d->m_strMenuDescr), kIdsDefaultMenuDescr)) return;
        CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
        CWnd* pMain = pThread != nullptr ? impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread) : nullptr;
        if (pMain != nullptr) hIcon = reinterpret_cast<HICON>(::GetClassLongPtr(pMain->m_hWnd, GCLP_HICON));
        hMenu = At<HMENU>(d->m_pMenuBar, kOffMenuBarDefaultMenu);
    }
    if (hMenu == nullptr) {
        ThrowInvalidArg();
        return;
    }
    impl__CreateFromMenu_CMFCMenuBar__UEAAXPEAUHMENU____HH_Z(d->m_pMenuBar, hMenu, FALSE, FALSE);
    ::SendMessage(Hwnd(d->m_wndIcon), kStmSetIcon, reinterpret_cast<WPARAM>(hIcon), 0);
    d->m_hmenuSelected = hMenu;
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), FALSE);
}

// Retail (RVA 0x17e2d0, mfc140u), fully transcribed:
//     if (m_pContextMenu == NULL) return;
//     CRect rect;  (zero-initialised)
//     ::GetWindowRect(m_pContextMenu->m_hWnd, &rect);
//     m_ptMenuLastPos = rect.TopLeft();                  // one 8-byte store
//     afxMenuHash.SaveMenuBar(m_pContextMenu->m_hMenu /*+0x1658*/,
//                             m_pContextMenu->GetMenuBar());   // vslot +0x3a0
// DEVIATIONS: (1) GetMenuBar is inlined as +0x230; (4) retail popup layout.
// Symbol: ?SaveMenu@CMFCToolBarsMenuPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__SaveMenu_CMFCToolBarsMenuPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MenuPage* d = D(pThis);
    if (d->m_pContextMenu == nullptr) return;
    RECT rect = {0, 0, 0, 0};
    ::GetWindowRect(At<HWND>(d->m_pContextMenu, kOffPopupHWnd), &rect);
    impl__m_ptMenuLastPos_CMFCToolBarsMenuPropertyPage__1VCPoint__A.x = rect.left;
    impl__m_ptMenuLastPos_CMFCToolBarsMenuPropertyPage__1VCPoint__A.y = rect.top;
    CMFCToolBar* pBar = reinterpret_cast<CMFCToolBar*>(static_cast<unsigned char*>(d->m_pContextMenu) + kOffPopupMenuBar);
    impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(MenuHash(), At<HMENU>(d->m_pContextMenu, kOffPopupHMenu), pBar);
}

// Retail (RVA 0x17e880, mfc140u), fully transcribed (the count is re-read on
// every iteration):
//     for (int i = 0; i < ::SendMessage(m_wndMenuesList.m_hWnd, CB_GETCOUNT, 0, 0); i++) {
//         if ((CDocTemplate*)::SendMessage(<list>, CB_GETITEMDATA, i, 0) == pTemplate) {
//             if (!bSaveCurr) m_hmenuSelected = NULL;
//             if (m_pMenuBar != NULL) m_hmenuCurr = m_pMenuBar->m_hMenu;   // +0x1360
//             ::SendMessage(<list>, CB_SETCURSEL, i, 0);
//             OnSelchangeMenuList();
//             return TRUE;
//         }
//     }
//     return FALSE;
// Symbol: ?SelectMenu@CMFCToolBarsMenuPropertyPage@@QEAAHPEAVCDocTemplate@@H@Z
extern "C" int MS_ABI impl__SelectMenu_CMFCToolBarsMenuPropertyPage__QEAAHPEAVCDocTemplate__H_Z(void* pThis, CDocTemplate* pTemplate, int bSaveCurr) {
    if (pThis == nullptr) return FALSE;
    S_MenuPage* d = D(pThis);
    for (int i = 0; i < static_cast<int>(::SendMessage(Hwnd(d->m_wndMenuesList), CB_GETCOUNT, 0, 0)); ++i) {
        if (reinterpret_cast<CDocTemplate*>(::SendMessage(Hwnd(d->m_wndMenuesList), CB_GETITEMDATA, static_cast<WPARAM>(i), 0)) != pTemplate)
            continue;
        if (!bSaveCurr) d->m_hmenuSelected = nullptr;
        if (d->m_pMenuBar != nullptr) d->m_hmenuCurr = At<HMENU>(d->m_pMenuBar, kOffMenuBarHMenu);
        ::SendMessage(Hwnd(d->m_wndMenuesList), CB_SETCURSEL, static_cast<WPARAM>(i), 0);
        impl__OnSelchangeMenuList_CMFCToolBarsMenuPropertyPage__IEAAXXZ(pThis);
        return TRUE;
    }
    return FALSE;
}

// Static data export (a CPoint).  Retail's .data holds (100, 100) at this
// datum: bytes 64 00 00 00 64 00 00 00 at RVA 0x3b1f88 (mfc140u).  SaveMenu
// above is its only writer in this file; the retail OnSelchangeContextMenuList
// reads it as the popup position.
// Symbol: ?m_ptMenuLastPos@CMFCToolBarsMenuPropertyPage@@1VCPoint@@A
extern "C" POINT impl__m_ptMenuLastPos_CMFCToolBarsMenuPropertyPage__1VCPoint__A = {100, 100};
