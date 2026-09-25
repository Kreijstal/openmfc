// CMFCToolBarsOptionsPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

// ===========================================================================
// CMFCToolBarsOptionsPropertyPage -- the "Options" page of the feature-pack
// Customize dialog (afxtoolbarsoptionspropertypage.h:28 in the 14.51 SDK on
// this host; derives CPropertyPage, DECLARE_DYNCREATE).
//
// The bodies below were transcribed from the retail disassembly (the method
// described in the header of core/ole/COleControl.cpp).  Function bodies are
// byte-identical in mfc140.dll and mfc140u.dll; every RVA and absolute
// address quoted in this file is mfc140u.  Only ??0, ??1, CreateObject and
// DoDataExchange have a row in mfc140u_rva_symbols.json; all eleven were
// resolved from the export's ordinal (mfc_complete_ordinal_mapping.json)
// through mfc140u.dll's export address table:
//
//   ??0 (ctor)         0x17e990     OShowTooltipsWithKeys     0x17edf0
//   ??1 (dtor)         0x17eb00     OnShowTooltips            0x17ee20
//   CreateObject       0x17e950     OnResetUsageData          0x17ee60
//   DoDataExchange     0x17eb80     OnShowRecentlyUsedMenus   0x17eea0
//   OnInitDialog       0x17eca0     OnShowMenusDelay          0x17eee0
//                                   OnLargeIcons              0x17ef10
//
// Layout.  The class is not declared in OpenMFC's public headers (grep of
// include/ finds nothing), so this file is the only place its layout lives.
// S_OptionsPage pins it: the ctor builds CPropertyPage at +0 and the members
// at the offsets below, and CreateObject allocates 0x7c8 bytes
// (`mov $0x7c8,%ecx` before its operator new call).  The member names are
// those of afxtoolbarsoptionspropertypage.h, whose declaration order matches
// the offsets and the CButton / CStatic vftable each control receives in the
// ctor -- with one wrinkle: DoDataExchange binds +0x410 (declared
// m_wndRuMenusLine) to IDC_AFX_RU_MENUS_TITLE and +0x4f8 (declared
// m_wndRuMenusTitle) to IDC_AFX_RU_MENUS_LINE.  Both are CStatics that every
// body here treats identically, so the bindings are transcribed by offset as
// retail has them.
//
// Structural deviations, applied uniformly (the same ones
// CMFCToolBarsMenuPropertyPage.cpp documents).  (1)-(3) are named again at the
// functions they affect; (4) is not repeated per function, so a body below
// described as "fully transcribed" still carries the (4) guard:
//
//  (1) vtable pointers.  Retail stores its vftable 0x18031abe8 at +0x00 and
//      the CButton (0x1802df1b0) / CStatic (0x1802e1ca8) vftables into the
//      controls after ??0CWnd@@QEAA@XZ.  OpenMFC has no MSVC-layout vtable for
//      any of them, so +0x00 keeps what the exported CPropertyPage constructor
//      thunk leaves there (OpenMFC's own C++ CPropertyPage vptr) and the
//      controls are built with the exported CWnd constructor alone.
//      Consequences:
//        * DoDataExchange / OnInitDialog here are not reached through the
//          vtable, and the afx_msg handlers are not reached through message
//          dispatch either: the map the exported GetMessageMap /
//          GetThisMessageMap return (featurepack/customize/MessageMaps.cpp
//          -> classCMFCToolBarsOptionsPropertyPage_msgmap in
//          detail/Toolbar23MsgmapSupport.cpp) has no entries of its own;
//        * the UpdateData(TRUE) calls below go to the exported
//          CWnd::UpdateData thunk, whose OpenMFC body (CWnd::UpdateData,
//          core/window/CWnd.cpp) never calls DoDataExchange -- it only walks
//          the child windows through UpdateDialogControls -- so the check-box
//          states are NOT transferred into m_bShowTooltips..m_bLargeIcons
//          today, and each handler publishes the member's current value.
//
//  (2) CDialog::OnInitDialog.  Retail calls ?OnInitDialog@CDialog@@UEAAHXZ
//      directly; this file calls the same export's thunk, whose OpenMFC body
//      (detail/DlgcoreSupport.cpp) is a stub returning TRUE.
//
//  (3) CMFCToolBar::m_UsageCount.Reset().  Retail inlines Reset (RemoveAll on
//      the CMap at +0x8 via the call to 0x18001ba40, then a dword 0 stored at
//      +0x40 -- the same two operations ?Reset@CMFCCmdUsageCount@@QEAAXXZ,
//      RVA 0x233a0 (mfc140u), performs on `this`; the inlined copy addresses
//      the static RIP-relatively, so the bytes differ).  This file calls that export's
//      thunk instead; OpenMFC's body (featurepack/customize/
//      CMFCCmdUsageCount.cpp) zeroes the total but does not empty the map,
//      which OpenMFC does not model.
//
//  (4) NULL guards on `this`.  Every body that reads `this` (all but
//      CreateObject and OnResetUsageData) returns early on NULL -- nullptr
//      from the ctor, FALSE from OnInitDialog.  Retail tests no `this`.
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
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                          // detail/MemcoreSupport.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(void* pDX, int nIDC, int* pv);           // core/runtime/DdxExchange.cpp

extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                        // detail/DlgcoreSupport.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                        // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                                // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                                 // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ();        // featurepack/customize/RuntimeClasses.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                                 // detail/MfcExceptionsSupport.cpp
extern "C" int   MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType, UINT nIDHelp);                    // core/collections/Globals.cpp
extern "C" void  MS_ABI impl__SetLargeIcons_CMFCToolBar__SAXH_Z(int bLargeIcons);                                   // featurepack/toolbar/CMFCToolBar.cpp
extern "C" void  MS_ABI impl__Reset_CMFCCmdUsageCount__QEAAXXZ(void* pThis);                                        // featurepack/customize/CMFCCmdUsageCount.cpp

// Exported data objects, read and written the way retail does.
extern "C" std::int32_t impl__m_bShowTooltips_CMFCToolBar__1HA;                  // featurepack/toolbar/StaticData.cpp
extern "C" std::int32_t impl__m_bShowShortcutKeys_CMFCToolBar__1HA;              // featurepack/toolbar/StaticData.cpp
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;                    // featurepack/toolbar/StaticData.cpp
extern "C" std::int32_t impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;             // featurepack/menu/StaticData.cpp
extern "C" std::int32_t impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA;             // featurepack/menu/StaticData.cpp
// CMFCToolBar::m_UsageCount (CMFCCmdUsageCount, 0x1803b1fd0) and
// CMFCToolBar::m_lstBasicCommands (CList<UINT,UINT>, 0x1803b2020), both
// defined with the retail layout in featurepack/toolbar/CMFCToolBar.cpp
// (UIntList there pins the CList: m_nCount is the INT_PTR at +0x18).
extern "C" unsigned char impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A[80];
extern "C" unsigned char impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A[56];

// This file's own ctor, called by CreateObject.
extern "C" void* MS_ABI impl___0CMFCToolBarsOptionsPropertyPage__QEAA_H_Z(void* pThis, int bIsMenuBarExist);

namespace {

// ---------------------------------------------------------------------------
// The retail object (0x7c8 bytes); offsets from the constructor (0x17e990).
// ---------------------------------------------------------------------------
struct S_OptionsPage {
    alignas(8) unsigned char m_base[0x158];                 // +0x000 CPropertyPage (OpenMFC sizeof == 0x158, asserted below)
    alignas(8) unsigned char m_wndLargeIcons[0xe8];         // +0x158 CButton
    alignas(8) unsigned char m_wndRUMenus[0xe8];            // +0x240 CButton
    alignas(8) unsigned char m_wndResetUsageBtn[0xe8];      // +0x328 CButton
    alignas(8) unsigned char m_wndRuMenusLine[0xe8];        // +0x410 CStatic (DDX-bound to IDC_AFX_RU_MENUS_TITLE)
    alignas(8) unsigned char m_wndRuMenusTitle[0xe8];       // +0x4f8 CStatic (DDX-bound to IDC_AFX_RU_MENUS_LINE)
    alignas(8) unsigned char m_wndShowAllMenusDelay[0xe8];  // +0x5e0 CButton
    alignas(8) unsigned char m_wndShowShortcutKeys[0xe8];   // +0x6c8 CButton
    int m_bShowTooltips;                                    // +0x7b0
    int m_bShowShortcutKeys;                                // +0x7b4
    int m_bRecentlyUsedMenus;                               // +0x7b8
    int m_bShowAllMenusDelay;                               // +0x7bc
    int m_bLargeIcons;                                      // +0x7c0
    int m_bIsMenuBarExist;                                  // +0x7c4
};
static_assert(offsetof(S_OptionsPage, m_wndLargeIcons) == 0x158, "ctor: lea 0x158(%rsi); CWnd ctor; CButton vftable");
static_assert(offsetof(S_OptionsPage, m_wndRUMenus) == 0x240, "ctor: lea 0x240(%rsi); CButton vftable");
static_assert(offsetof(S_OptionsPage, m_wndResetUsageBtn) == 0x328, "ctor: lea 0x328(%rsi); CButton vftable");
static_assert(offsetof(S_OptionsPage, m_wndRuMenusLine) == 0x410, "ctor: lea 0x410(%rsi); CStatic vftable");
static_assert(offsetof(S_OptionsPage, m_wndRuMenusTitle) == 0x4f8, "ctor: lea 0x4f8(%rsi); CStatic vftable");
static_assert(offsetof(S_OptionsPage, m_wndShowAllMenusDelay) == 0x5e0, "ctor: lea 0x5e0(%rsi); CButton vftable");
static_assert(offsetof(S_OptionsPage, m_wndShowShortcutKeys) == 0x6c8, "ctor: lea 0x6c8(%rsi); CButton vftable");
static_assert(offsetof(S_OptionsPage, m_bShowTooltips) == 0x7b0, "ctor: mov %eax,0x7b0(%rsi) <- CMFCToolBar::m_bShowTooltips");
static_assert(offsetof(S_OptionsPage, m_bShowShortcutKeys) == 0x7b4, "ctor: mov %eax,0x7b4(%rsi) <- CMFCToolBar::m_bShowShortcutKeys");
static_assert(offsetof(S_OptionsPage, m_bRecentlyUsedMenus) == 0x7b8, "ctor: mov %eax,0x7b8(%rsi) <- CMFCMenuBar::m_bRecentlyUsedMenus");
static_assert(offsetof(S_OptionsPage, m_bShowAllMenusDelay) == 0x7bc, "ctor: mov %eax,0x7bc(%rsi) <- CMFCMenuBar::m_bShowAllMenusDelay");
static_assert(offsetof(S_OptionsPage, m_bLargeIcons) == 0x7c0, "ctor: mov %eax,0x7c0(%rsi) <- CMFCToolBar::m_bLargeIcons");
static_assert(offsetof(S_OptionsPage, m_bIsMenuBarExist) == 0x7c4, "ctor: mov %edi,0x7c4(%rsi) <- bIsMenuBarExist");
static_assert(sizeof(S_OptionsPage) == 0x7c8, "CreateObject: mov $0x7c8,%ecx");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CStatic) == 0xe8 && sizeof(CButton) == 0xe8,
              "the plain controls are bare CWnds");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads this page's HWND at +0x40 (OnInitDialog: mov 0x40(%rdi),%rcx)");

// CMFCToolBarsCustomizeDialog::m_uiFlags: retail reads +0x284
// (`testb $0x80,0x284(%rbx)`); CMFCToolBarsCustomizeDialog.cpp's S_CustDlg
// pins the same offset (static_assert "m_uiFlags" there).
constexpr int  kOffCustDlgFlags          = 0x284;
constexpr UINT kCustomizeNoLargeIcons    = 0x80;     // AFX_CUSTOMIZE_NO_LARGE_ICONS
// CList<UINT,UINT>::m_nCount (INT_PTR): retail `cmpq $0x0,0x1803b2038`,
// i.e. m_lstBasicCommands (0x1803b2020) + 0x18.
constexpr int  kOffListCount             = 0x18;

// Resource ids (afxribbonres.h in the 14.51 SDK on this host).
constexpr UINT kIddPropPage6             = 0x3f09;   // IDD_AFXBARRES_PROPPAGE6 (16137)
constexpr int  kIdcRuMenusTitle          = 0x409a;   // IDC_AFX_RU_MENUS_TITLE (16538)
constexpr int  kIdcRuMenusLine           = 0x409b;   // IDC_AFX_RU_MENUS_LINE (16539)
constexpr int  kIdcShowTooltips          = 0x410b;   // IDC_AFXBARRES_SHOW_TOOLTIPS (16651)
constexpr int  kIdcShowTooltipsWithKeys  = 0x410c;   // IDC_AFXBARRES_SHOW_TOOLTIPS_WITH_KEYS (16652)
constexpr int  kIdcLargeIcons            = 0x410d;   // IDC_AFXBARRES_LARGE_ICONS (16653)
constexpr int  kIdcShowRecentlyUsedMenus = 0x410f;   // IDC_AFXBARRES_SHOW_RECENTLY_USED_MENUS (16655)
constexpr int  kIdcShowMenusDelay        = 0x4110;   // IDC_AFXBARRES_SHOW_MENUS_DELAY (16656)
constexpr int  kIdcResetUsageData        = 0x4111;   // IDC_AFXBARRES_RESET_USAGE_DATA (16657)
constexpr UINT kIdsResetUsageWarning     = 0x3f74;   // IDS_AFXBARRES_RESET_USAGE_WARNING (16244)

inline S_OptionsPage* D(void* p) { return static_cast<S_OptionsPage*>(p); }
inline CWnd* Self(void* p) { return static_cast<CWnd*>(p); }
inline CWnd* Wnd(unsigned char* storage) { return reinterpret_cast<CWnd*>(storage); }

template <typename T>
inline T& At(void* p, int off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off); }

// ctrl.ShowWindow(SW_HIDE); ctrl.EnableWindow(FALSE); -- the pair retail
// emits for each control it removes (`xor %edx,%edx` before both calls).
inline void HideAndDisable(unsigned char* storage) {
    impl__ShowWindow_CWnd__QEAAHH_Z(Wnd(storage), SW_HIDE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(storage), FALSE);
}

} // namespace

// Retail (RVA 0x17e990, mfc140u), transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE6 /*0x3f09*/, 0,
//                                  /*dwSize*/ 0x68);
//     vfptr = 0x18031abe8;
//     seven controls: CWnd::CWnd() then the CButton (+0x158/+0x240/+0x328/
//         +0x5e0/+0x6c8) or CStatic (+0x410/+0x4f8) vftable;
//     m_bIsMenuBarExist     = bIsMenuBarExist;
//     m_bShowTooltips       = CMFCToolBar::m_bShowTooltips;
//     m_bShowShortcutKeys   = CMFCToolBar::m_bShowShortcutKeys;
//     m_bRecentlyUsedMenus  = CMFCMenuBar::m_bRecentlyUsedMenus;
//     m_bShowAllMenusDelay  = CMFCMenuBar::m_bShowAllMenusDelay;
//     m_bLargeIcons         = CMFCToolBar::m_bLargeIcons;
// DEVIATION: the vftable stores are deviation (1); `this` guard (4).
// Symbol: ??0CMFCToolBarsOptionsPropertyPage@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsOptionsPropertyPage__QEAA_H_Z(void* pThis, int bIsMenuBarExist) {
    if (pThis == nullptr) return nullptr;
    S_OptionsPage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage6, 0, 0x68);
    for (unsigned char* w : {d->m_wndLargeIcons, d->m_wndRUMenus, d->m_wndResetUsageBtn, d->m_wndRuMenusLine,
                             d->m_wndRuMenusTitle, d->m_wndShowAllMenusDelay, d->m_wndShowShortcutKeys}) {
        impl___0CWnd__QEAA_XZ(w);
    }
    d->m_bIsMenuBarExist    = bIsMenuBarExist;
    d->m_bShowTooltips      = impl__m_bShowTooltips_CMFCToolBar__1HA;
    d->m_bShowShortcutKeys  = impl__m_bShowShortcutKeys_CMFCToolBar__1HA;
    d->m_bRecentlyUsedMenus = impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA;
    d->m_bShowAllMenusDelay = impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA;
    d->m_bLargeIcons        = impl__m_bLargeIcons_CMFCToolBar__1HA;
    return pThis;
}

// Retail (RVA 0x17eb00, mfc140u), transcribed: re-install the class vftable
// (deviation 1), then ~CButton on +0x6c8/+0x5e0, ~CStatic on +0x4f8/+0x410,
// ~CButton on +0x328/+0x240/+0x158, and a tail jump to ~CPropertyPage.
// Symbol: ??1CMFCToolBarsOptionsPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsOptionsPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_OptionsPage* d = D(pThis);
    impl___1CButton__UEAA_XZ(d->m_wndShowShortcutKeys);
    impl___1CButton__UEAA_XZ(d->m_wndShowAllMenusDelay);
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndRuMenusTitle));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndRuMenusLine));
    impl___1CButton__UEAA_XZ(d->m_wndResetUsageBtn);
    impl___1CButton__UEAA_XZ(d->m_wndRUMenus);
    impl___1CButton__UEAA_XZ(d->m_wndLargeIcons);
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (RVA 0x17e950, mfc140u): `new` 0x7c8 bytes (operator new) and, when
// that is non-NULL, run the constructor with bIsMenuBarExist = FALSE
// (`xor %edx,%edx`).
// Symbol: ?CreateObject@CMFCToolBarsOptionsPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarsOptionsPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_OptionsPage));
    if (p != nullptr) impl___0CMFCToolBarsOptionsPropertyPage__QEAA_H_Z(p, FALSE);
    return p;
}

// Retail (RVA 0x17eb80, mfc140u), fully transcribed -- no CPropertyPage::
// DoDataExchange call, the DDX calls in this order, the last a tail jump:
//     DDX_Control(pDX, LARGE_ICONS               16653, +0x158 m_wndLargeIcons);
//     DDX_Control(pDX, SHOW_RECENTLY_USED_MENUS  16655, +0x240 m_wndRUMenus);
//     DDX_Control(pDX, RESET_USAGE_DATA          16657, +0x328 m_wndResetUsageBtn);
//     DDX_Control(pDX, IDC_AFX_RU_MENUS_TITLE    16538, +0x410);
//     DDX_Control(pDX, IDC_AFX_RU_MENUS_LINE     16539, +0x4f8);
//     DDX_Control(pDX, SHOW_MENUS_DELAY          16656, +0x5e0 m_wndShowAllMenusDelay);
//     DDX_Control(pDX, SHOW_TOOLTIPS_WITH_KEYS   16652, +0x6c8 m_wndShowShortcutKeys);
//     DDX_Check  (pDX, SHOW_TOOLTIPS             16651, m_bShowTooltips);
//     DDX_Check  (pDX, SHOW_TOOLTIPS_WITH_KEYS   16652, m_bShowShortcutKeys);
//     DDX_Check  (pDX, SHOW_RECENTLY_USED_MENUS  16655, m_bRecentlyUsedMenus);
//     DDX_Check  (pDX, SHOW_MENUS_DELAY          16656, m_bShowAllMenusDelay);
//     DDX_Check  (pDX, LARGE_ICONS               16653, m_bLargeIcons);
// (Not reached through UpdateData today -- deviation 1.)
// Symbol: ?DoDataExchange@CMFCToolBarsOptionsPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarsOptionsPropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_OptionsPage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcLargeIcons, d->m_wndLargeIcons);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcShowRecentlyUsedMenus, d->m_wndRUMenus);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcResetUsageData, d->m_wndResetUsageBtn);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcRuMenusTitle, d->m_wndRuMenusLine);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcRuMenusLine, d->m_wndRuMenusTitle);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcShowMenusDelay, d->m_wndShowAllMenusDelay);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcShowTooltipsWithKeys, d->m_wndShowShortcutKeys);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcShowTooltips, &d->m_bShowTooltips);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcShowTooltipsWithKeys, &d->m_bShowShortcutKeys);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcShowRecentlyUsedMenus, &d->m_bRecentlyUsedMenus);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcShowMenusDelay, &d->m_bShowAllMenusDelay);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcLargeIcons, &d->m_bLargeIcons);
}

// Retail (RVA 0x17edf0, mfc140u), fully transcribed:
//     UpdateData(TRUE);
//     CMFCToolBar::m_bShowShortcutKeys = m_bShowShortcutKeys;
// (UpdateData transfers nothing today -- deviation 1.)
// Symbol: ?OShowTooltipsWithKeys@CMFCToolBarsOptionsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OShowTooltipsWithKeys_CMFCToolBarsOptionsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    impl__m_bShowShortcutKeys_CMFCToolBar__1HA = D(pThis)->m_bShowShortcutKeys;
}

// Retail (RVA 0x17eca0, mfc140u), fully transcribed:
//     CDialog::OnInitDialog();                               // direct call, result ignored
//     m_wndShowShortcutKeys.EnableWindow(m_bShowTooltips);
//     m_wndShowAllMenusDelay.EnableWindow(m_bRecentlyUsedMenus);
//     if (CMFCToolBar::m_lstBasicCommands.IsEmpty() || !m_bIsMenuBarExist) {
//         // each: ShowWindow(SW_HIDE) then EnableWindow(FALSE)
//         m_wndRUMenus (+0x240); m_wndResetUsageBtn (+0x328); +0x410; +0x4f8;
//         m_wndShowAllMenusDelay (+0x5e0);
//     }
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(
//         CMFCToolBarsCustomizeDialog,
//         CWnd::FromHandle(::GetParent(m_hWnd)));           // IAT slot resolved: USER32!GetParent
//     ENSURE(pWndParent != NULL);          // NULL or !IsKindOf -> AfxThrowInvalidArgException
//     if (pWndParent->m_uiFlags & AFX_CUSTOMIZE_NO_LARGE_ICONS) {
//         m_wndLargeIcons.ShowWindow(SW_HIDE);
//         m_wndLargeIcons.EnableWindow(FALSE);
//         m_bLargeIcons = FALSE;
//     }
//     return TRUE;
// DEVIATIONS: (2) for the CDialog::OnInitDialog body; (4).
// Symbol: ?OnInitDialog@CMFCToolBarsOptionsPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsOptionsPropertyPage__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_OptionsPage* d = D(pThis);
    impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(Self(pThis)));

    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndShowShortcutKeys), d->m_bShowTooltips);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndShowAllMenusDelay), d->m_bRecentlyUsedMenus);

    const INT_PTR nBasicCommands = At<INT_PTR>(impl__m_lstBasicCommands_CMFCToolBar__1V__CList_II__A, kOffListCount);
    if (nBasicCommands == 0 || !d->m_bIsMenuBarExist) {
        HideAndDisable(d->m_wndRUMenus);
        HideAndDisable(d->m_wndResetUsageBtn);
        HideAndDisable(d->m_wndRuMenusLine);
        HideAndDisable(d->m_wndRuMenusTitle);
        HideAndDisable(d->m_wndShowAllMenusDelay);
    }

    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Self(pThis)->m_hWnd));
    if (pParent == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ())) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // not reached: the throw does not return
    }

    if (At<UINT>(pParent, kOffCustDlgFlags) & kCustomizeNoLargeIcons) {
        HideAndDisable(d->m_wndLargeIcons);
        d->m_bLargeIcons = FALSE;
    }
    return TRUE;
}

// Retail (RVA 0x17ef10, mfc140u), fully transcribed:
//     UpdateData(TRUE);
//     CMFCToolBar::SetLargeIcons(m_bLargeIcons);            // tail jump
// (UpdateData transfers nothing today -- deviation 1.)
// Symbol: ?OnLargeIcons@CMFCToolBarsOptionsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnLargeIcons_CMFCToolBarsOptionsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    impl__SetLargeIcons_CMFCToolBar__SAXH_Z(D(pThis)->m_bLargeIcons);
}

// Retail (RVA 0x17ee60, mfc140u), fully transcribed (it never reads `this`):
//     if (AfxMessageBox(IDS_AFXBARRES_RESET_USAGE_WARNING /*0x3f74*/,
//                       MB_YESNO /*4*/, (UINT)-1) == IDYES /*6*/)
//         CMFCToolBar::m_UsageCount.Reset();   // inlined: RemoveAll on the map
//                                              // at 0x1803b1fd8, then 0 -> 0x1803b2010
// DEVIATION: (3) -- Reset goes through the exported thunk.
// Symbol: ?OnResetUsageData@CMFCToolBarsOptionsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetUsageData_CMFCToolBarsOptionsPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;   // retail does not touch `this`
    if (impl__AfxMessageBox__YAHIII_Z(kIdsResetUsageWarning, MB_YESNO, static_cast<UINT>(-1)) == IDYES) {
        impl__Reset_CMFCCmdUsageCount__QEAAXXZ(impl__m_UsageCount_CMFCToolBar__1VCMFCCmdUsageCount__A);
    }
}

// Retail (RVA 0x17eee0, mfc140u), fully transcribed:
//     UpdateData(TRUE);
//     CMFCMenuBar::m_bShowAllMenusDelay = m_bShowAllMenusDelay;
// (UpdateData transfers nothing today -- deviation 1.)
// Symbol: ?OnShowMenusDelay@CMFCToolBarsOptionsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnShowMenusDelay_CMFCToolBarsOptionsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    impl__m_bShowAllMenusDelay_CMFCMenuBar__1HA = D(pThis)->m_bShowAllMenusDelay;
}

// Retail (RVA 0x17eea0, mfc140u), fully transcribed:
//     UpdateData(TRUE);
//     m_wndShowAllMenusDelay.EnableWindow(m_bRecentlyUsedMenus);
//     CMFCMenuBar::m_bRecentlyUsedMenus = m_bRecentlyUsedMenus;
// (UpdateData transfers nothing today -- deviation 1.)
// Symbol: ?OnShowRecentlyUsedMenus@CMFCToolBarsOptionsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnShowRecentlyUsedMenus_CMFCToolBarsOptionsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_OptionsPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndShowAllMenusDelay), d->m_bRecentlyUsedMenus);
    impl__m_bRecentlyUsedMenus_CMFCMenuBar__1HA = d->m_bRecentlyUsedMenus;
}

// Retail (RVA 0x17ee20, mfc140u), fully transcribed:
//     UpdateData(TRUE);
//     CMFCToolBar::m_bShowTooltips = m_bShowTooltips;
//     m_wndShowShortcutKeys.EnableWindow(m_bShowTooltips);  // tail jump
// (UpdateData transfers nothing today -- deviation 1.)
// Symbol: ?OnShowTooltips@CMFCToolBarsOptionsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnShowTooltips_CMFCToolBarsOptionsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_OptionsPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    impl__m_bShowTooltips_CMFCToolBar__1HA = d->m_bShowTooltips;
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndShowShortcutKeys), d->m_bShowTooltips);
}
