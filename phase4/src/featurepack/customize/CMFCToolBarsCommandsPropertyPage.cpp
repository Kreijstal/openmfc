// CMFCToolBarsCommandsPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
// FindListStorage: the header-only reader of the side table in which
// OpenMFC's CObList keeps its nodes (deviation (4)).
#include "detail/FilecoreSupport.h"

#include <cstddef>
#include <cstring>
#include <cwchar>
#include <new>

// ===========================================================================
// CMFCToolBarsCommandsPropertyPage -- the "Commands" page of the feature-pack
// Customize sheet (afxtoolbarslistpropertypage.h, derives CPropertyPage).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  All RVAs
// and absolute addresses in this file are mfc140u.  The ctor, dtor,
// CreateObject, DoDataExchange and OnChangeSelButton have entries in
// mfc140u_rva_symbols.json; the other exports' RVAs were resolved from their
// ordinals (mfc_complete_ordinal_mapping.json) through mfc140u.dll's export
// address table:
//
//   ctor 0x17b6d0   dtor 0x17b810   CreateObject 0x17b650   DoDataExchange 0x17b8d0
//   OnSelchangeCategory 0x17b940    OnSelchangeUserTools 0x17bcf0
//   OnInitDialog 0x17bd50           OnChangeSelButton 0x17bde0
//   SetUserCategory 0x17be90        SetAllCategory 0x17bed0
//
// Import slots named below were resolved with iatu.py against mfc140u.dll:
// 0x1802c7120 = USER32!SendMessageW, 0x1802c72d8 = USER32!GetParent,
// 0x1802c7748 = ucrt wcslen, 0x1802c7770 = ucrt wcscmp.  0x1802c7b30 is not
// an import; it is the indirect-call target every virtual call goes through.
//
// The class is NOT declared in OpenMFC's public headers, so S_CmdsPage below
// is the only place its layout lives.  It is pinned by the ctor (0x17b6d0),
// whose stores are listed on the struct, by the dtor (0x17b810), and by
// CreateObject (0x17b650), which passes 0x350 to operator new (the harvested
// descriptor in featurepack/customize/RuntimeClasses.cpp agrees: 848).  The
// member names are afxtoolbarslistpropertypage.h's (14.51 SDK), whose
// declaration order matches the offsets.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftable 0x18031a298 at +0x00 (ctor
//      and dtor), the CListBox vftable into m_wndCategory after
//      ??0CWnd@@QEAA@XZ, and the CMFCToolBarsCommandsListBox vftable
//      0x180318cc8 into m_wndTools.  OpenMFC has no MSVC-layout vtable for
//      this class: +0x00 keeps what the CPropertyPage ctor thunk installs and
//      the list-box members are built with the exported CWnd ctor alone, as in
//      the sibling Keyboard / Toolbars pages.  Related consequence: UpdateData
//      goes to the exported CWnd::UpdateData thunk (core/window/Thunks.cpp ->
//      CWnd::UpdateData in core/window/CWnd.cpp), which does not call this
//      class's DoDataExchange, and OpenMFC's CDialog::OnInitDialog thunk
//      (detail/DlgcoreSupport.cpp) only returns TRUE, whereas retail's
//      (0x208d10) runs UpdateData(FALSE) (the call at 0x208d53).  So no
//      OpenMFC path runs DoDataExchange today: not only is the DDX_Text
//      transfer of m_strButtonDescription skipped, the two DDX_Control
//      calls that attach m_wndCategory / m_wndTools to their dialog items do
//      not run either, and those members' m_hWnd (+0x198 / +0x280) stay NULL
//      unless a caller invokes the exported DoDataExchange thunk itself.
//      Every SendMessage below then targets a NULL HWND and fails.
//
//  (2) m_wndTools (CMFCToolBarsCommandsListBox).  Retail calls its exported
//      ctor (0x176720): CWnd::CWnd(), its vftable, then m_sizeButton (+0xe8)
//      = CSize(0, 0) as one 8-byte zero store.  OpenMFC's ctor thunk for that
//      class (featurepack/customize/CMFCToolBarsCommandsListBox.cpp) is still
//      a placeholder that only returns pThis, so the member is built here the
//      way 0x176720 builds it (less the vftable, deviation (1)).  The retail
//      dtor inlines that class's destructor (vftable reset, then
//      ??1CListBox@@ 0x293f30), so the CListBox dtor thunk is what runs here.
//
//  (3) Virtual GetMessageString on the parent frame (vslot 95, +0x2f8) is
//      devirtualised to the exported CFrameWnd thunk, so an override in a
//      client frame class is not reached -- the same treatment as
//      CMFCToolBarsKeyboardPropertyPage.cpp's OnSelchangeCommandsList.
//
//  (4) OpenMFC's CObList keeps its nodes in a side table, so the retail walk
//      of a category list (m_pNodeHead +0x08, node data +0x10, pNext +0x00)
//      becomes a FindListStorage walk over the same object, as in the Keyboard
//      page.  A NULL list, which retail dereferences, walks as empty.
//
//  (5) The parent sheet.  Retail reaches its CMFCToolBarsCustomizeDialog as
//      DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent()) (::GetParent
//      + CWnd::FromHandle 0x28ad70 + CObject::IsKindOf 0x234cf0 against the
//      descriptor 0x180319428) under ENSURE.  The same test is made here
//      through the thunks, and a failure throws through
//      AfxThrowInvalidArgException exactly as retail's 0x227720 does.  (The
//      Keyboard page's header notes that a customize dialog OpenMFC itself
//      built reports CPropertySheet from its vtable, so there the ENSURE fires.)
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the mangled names; the file that
// defines each is named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                             // detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);                               // core/dialog/RuntimeClasses.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                 // detail/DlgcoreSupport.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                                 // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis);                                         // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CListBox* pThis, int nIndex, CString* rString);                                                  // core/controls/CListBox.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                        // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);                  // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                  // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CFrameWnd* pThis, unsigned int nID, CString* rMessage);   // core/frame/CFrameWnd.cpp

extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ();        // featurepack/customize/RuntimeClasses.cpp
extern "C" void  MS_ABI impl__FillCategoriesListBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCListBox__H_Z(const void* pThis, CWnd* pWndCategory, int bAddEmpty);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
extern "C" int   MS_ABI impl__GetCountInCategory_CMFCToolBarsCustomizeDialog__QEBAHPEB_WAEBVCObList___Z(const void* pThis, const wchar_t* lpszItemName, const CObList* pListCommands);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp

extern "C" void* MS_ABI impl__GetCmdMgr__YAPEAVCCommandManager__XZ();                                          // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__GetCmdImage_CCommandManager__QEBAHIH_Z(const void* pThis, unsigned int uiCmd, int bUserImage);   // featurepack/customize/CCommandManager.cpp

extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                                              // detail/RegcoreSupport.cpp
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);                        // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);                          // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                            // detail/MfcExceptionsSupport.cpp

// This file's own thunks that earlier bodies call.
extern "C" void* MS_ABI impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__OnChangeSelButton_CMFCToolBarsCommandsPropertyPage__QEAAXPEAVCMFCToolBarButton___Z(void* pThis, CMFCToolBarButton* pButton);
extern "C" void  MS_ABI impl__OnSelchangeCategory_CMFCToolBarsCommandsPropertyPage__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Retail layout (mfc140u).  Offsets from the ctor (0x17b6d0):
// CPropertyPage at +0 (??0CPropertyPage@@QEAA@IIK@Z 0x215910), ??0CWnd@@
// (0x28a700) on +0x158, ??0CMFCToolBarsCommandsListBox@@ (0x176720) on
// +0x240, nil strings at +0x330 / +0x340 / +0x348, then SetString(L"", 0)
// (0x2e30) on +0x330.  +0x338 is not written by the ctor; OnChangeSelButton
// (0x17bde0) stores its argument there.  The dtor (0x17b810) releases the
// three strings in reverse order, then ??1CListBox@@ (0x293f30) on +0x240 and
// +0x158, then ??1CPropertyPage@@ (0x215f40).
// ---------------------------------------------------------------------------
struct S_CmdsListBox {
    alignas(8) unsigned char m_base[0xe8];     // +0x000 CListBox (a bare CWnd here, deviation (1))
    SIZE          m_sizeButton;                // +0x0e8 `movq $0x0,0xe8(%rbx)` in 0x176720
};
static_assert(offsetof(S_CmdsListBox, m_sizeButton) == 0xe8, "0x176720: movq $0x0,0xe8(%rbx)");
static_assert(sizeof(S_CmdsListBox) == 0xf0, "m_wndTools spans +0x240..+0x330");

struct S_CmdsPage {
    alignas(8) unsigned char m_base[0x158];    // +0x000 CPropertyPage (OpenMFC sizeof == 0x158, asserted below)
    alignas(8) unsigned char m_wndCategory[0xe8];   // +0x158 CListBox, IDC_AFXBARRES_CATEGORY    16601
    S_CmdsListBox      m_wndTools;             // +0x240 CMFCToolBarsCommandsListBox, IDC_AFXBARRES_USER_TOOLS 16602
    CString            m_strButtonDescription; // +0x330 (DDX_Text IDC_AFXBARRES_BUTTON_DESCR 16603)
    CMFCToolBarButton* m_pSelButton;           // +0x338
    CString            m_strUserCategory;      // +0x340 (SetUserCategory 0x17be90)
    CString            m_strAllCategory;       // +0x348 (SetAllCategory 0x17bed0)
};
static_assert(offsetof(S_CmdsPage, m_wndCategory) == 0x158, "ctor: CWnd ctor on this+0x158");
static_assert(offsetof(S_CmdsPage, m_wndTools) == 0x240, "ctor: CMFCToolBarsCommandsListBox ctor on this+0x240");
static_assert(offsetof(S_CmdsPage, m_strButtonDescription) == 0x330, "ctor: nil string at 0x330");
static_assert(offsetof(S_CmdsPage, m_pSelButton) == 0x338, "OnChangeSelButton: mov %rbx,0x338(%rdi)");
static_assert(offsetof(S_CmdsPage, m_strUserCategory) == 0x340, "ctor: nil string at 0x340");
static_assert(offsetof(S_CmdsPage, m_strAllCategory) == 0x348, "ctor: nil string at 0x348");
static_assert(sizeof(S_CmdsPage) == 0x350, "CreateObject 0x17b650: operator new(0x350)");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CListBox) == 0xe8, "plain list box is a bare CWnd");
static_assert(sizeof(CString) == 8, "retail CString is one pointer");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads the control HWNDs at member+0x40 (0x198, 0x280)");
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "OnSelchangeCategory: mov %eax,0x8(%r13)");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "0x24(button)");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "0x38(button)");
static_assert(offsetof(CMFCToolBarButton, m_strTextCustom) == 0x40, "0x40(button)");

// Resource IDs (afxribbonres.h, 14.51 SDK).
constexpr unsigned int kIddPropPage1   = 16104;   // 0x3ee8 IDD_AFXBARRES_PROPPAGE1
constexpr int          kIdcCategory    = 16601;   // 0x40d9 IDC_AFXBARRES_CATEGORY
constexpr int          kIdcUserTools   = 16602;   // 0x40da IDC_AFXBARRES_USER_TOOLS
constexpr int          kIdcButtonDescr = 16603;   // 0x40db IDC_AFXBARRES_BUTTON_DESCR

// The retail ctor passes sizeof(PROPSHEETPAGE) = 0x68 as CPropertyPage's dwSize.
constexpr unsigned long kPropSheetPageSize = 0x68;

inline S_CmdsPage* D(void* p) { return static_cast<S_CmdsPage*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline HWND HwndOf(const void* pWnd) { return static_cast<const CWnd*>(pWnd)->m_hWnd; }   // member +0x40

// ENSURE(x): retail throws through ?AfxThrowInvalidArgException@@ (0x227720).
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent()) -- deviation (5).
void* ParentCustomizeDialog(void* pThis) {
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Wnd(pThis)->m_hWnd));
    if (pParent == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pParent;
}

// The category list box carries CObList* item data (FillCategoriesListBox);
// deviation (4).
typedef CList<CObject*, CObject*> ObListData;
inline const ObListData* CategoryButtons(const CObList* p) {
    return p ? openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(p) : nullptr;
}

// CWaitCursor as retail inlines it: the ctor is
// AfxGetModuleState()->m_pCurrentWinApp->BeginWaitCursor() (0x133930, +0x8,
// 0x1de7b0); the dtor, the unexported 0x7687c, is the same with
// EndWaitCursor (0x1de7e0).  Both thunks tolerate a NULL app.
struct WaitCursor {
    WaitCursor() { impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(impl__AfxGetApp__YAPEAVCWinApp__XZ()); }
    ~WaitCursor() { impl__EndWaitCursor_CCmdTarget__QEAAXXZ(impl__AfxGetApp__YAPEAVCWinApp__XZ()); }
    WaitCursor(const WaitCursor&) = delete;
    WaitCursor& operator=(const WaitCursor&) = delete;
};

// CStringT's `str1 > psz2` as the retail loop inlines it: wcscmp (import
// 0x1802c7770), result normalised to -1/0/1, `> 0`.
inline bool StringGreater(const CString& a, const CString& b) {
    return std::wcscmp(a.GetString(), b.GetString()) > 0;
}

} // namespace

// Retail (RVA 0x17b6d0, mfc140u), transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE1 /*0x3ee8*/, 0, 0x68);   // 0x215910
//     vfptr = 0x18031a298;
//     m_wndCategory: CWnd::CWnd() (0x28a700) + CListBox vftable
//     m_wndTools: CMFCToolBarsCommandsListBox::CMFCToolBarsCommandsListBox()      // 0x176720
//     m_strButtonDescription, m_strUserCategory, m_strAllCategory: nil string
//     m_strButtonDescription = L"";   // SetString(0x18033d19c, wcslen) 0x2e30
//     (m_pSelButton, +0x338, is not initialised)
// DEVIATIONS: the vftable stores are deviation (1); m_wndTools is built
// inline (deviation (2)).  Assigning L"" to a nil string leaves it empty, so
// the three CStrings are simply default-constructed.  m_pSelButton is set to
// NULL here although retail leaves it uninitialised.  In the retail code range
// 0x17b650..0x17bf0f (this class's exports, plus CMFCToolBarsListPropertyPage's
// CreateObject 0x17b690 / GetRuntimeClass 0x17b6c0) the only access to +0x338
// is OnChangeSelButton's store; readers elsewhere in the image were not
// searched for.  This export was not on this file's work list (its previous
// body only returned pThis), but CMFCToolBarsCustomizeDialog.cpp constructs the
// page through it and the destructor below releases the CStrings it builds,
// so it is implemented with the rest.
// Symbol: ??0CMFCToolBarsCommandsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    S_CmdsPage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage1, 0, kPropSheetPageSize);
    impl___0CWnd__QEAA_XZ(d->m_wndCategory);
    impl___0CWnd__QEAA_XZ(d->m_wndTools.m_base);   // deviation (2)
    d->m_wndTools.m_sizeButton.cx = 0;
    d->m_wndTools.m_sizeButton.cy = 0;
    new (&d->m_strButtonDescription) CString();
    new (&d->m_strUserCategory) CString();
    new (&d->m_strAllCategory) CString();
    d->m_pSelButton = nullptr;
    return pThis;
}

// Retail (RVA 0x17b810, mfc140u), transcribed:
//     vfptr = 0x18031a298;
//     ~m_strAllCategory;  ~m_strUserCategory;  ~m_strButtonDescription;   // inline CStringData release
//     m_wndTools: vftable 0x180318cc8, then CListBox::~CListBox()        // 0x293f30
//     m_wndCategory.~CListBox();                                          // 0x293f30
//     CPropertyPage::~CPropertyPage();                                    // tail jump 0x215f40
// The vftable stores are deviation (1).
// Symbol: ??1CMFCToolBarsCommandsPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsCommandsPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CmdsPage* d = D(pThis);
    d->m_strAllCategory.~CString();
    d->m_strUserCategory.~CString();
    d->m_strButtonDescription.~CString();
    impl___1CListBox__UEAA_XZ(reinterpret_cast<CListBox*>(d->m_wndTools.m_base));
    impl___1CListBox__UEAA_XZ(reinterpret_cast<CListBox*>(d->m_wndCategory));
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (RVA 0x17b650, mfc140u), fully transcribed:
//     void* p = operator new(0x350);                                      // 0x27f0
//     return p ? new (p) CMFCToolBarsCommandsPropertyPage : NULL;         // ctor 0x17b6d0
// Symbol: ?CreateObject@CMFCToolBarsCommandsPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarsCommandsPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_CmdsPage));
    if (p == nullptr) return nullptr;
    return impl___0CMFCToolBarsCommandsPropertyPage__QEAA_XZ(p);
}

// Retail (RVA 0x17b8d0, mfc140u), fully transcribed:
//     DDX_Control(pDX, IDC_AFXBARRES_CATEGORY,     m_wndCategory);          // 0x293870, +0x158
//     DDX_Control(pDX, IDC_AFXBARRES_USER_TOOLS,   m_wndTools);             // 0x293870, +0x240
//     DDX_Text   (pDX, IDC_AFXBARRES_BUTTON_DESCR, m_strButtonDescription); // tail jump 0x209450, +0x330
// Symbol: ?DoDataExchange@CMFCToolBarsCommandsPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarsCommandsPropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_CmdsPage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCategory, d->m_wndCategory);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcUserTools, d->m_wndTools.m_base);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcButtonDescr, &d->m_strButtonDescription);
}

// Retail (RVA 0x17bde0, mfc140u), fully transcribed:
//     m_strButtonDescription = L"";                  // SetString(0x18033d19c, wcslen) 0x2e30
//     if (pButton != NULL) {
//         if (pButton->m_nID == 0)                   // cmpl $0x0,0x24(%rbx)
//             m_strButtonDescription = pButton->m_strText;   // CSimpleStringT::operator= 0xde30, +0x38
//         else {
//             CFrameWnd* pParent = GetParentFrame(); // 0x28e200
//             if (pParent != NULL && pParent->GetSafeHwnd() != NULL)   // cmpq $0x0,0x40(%rax)
//                 pParent->GetMessageString(pButton->m_nID, m_strButtonDescription);   // vslot 95 (+0x2f8)
//         }
//     }
//     m_pSelButton = pButton;                        // unconditional, +0x338
//     UpdateData(FALSE);                             // tail jump 0x2910d0
// GetMessageString is deviation (3); UpdateData is deviation (1).
// Symbol: ?OnChangeSelButton@CMFCToolBarsCommandsPropertyPage@@QEAAXPEAVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__OnChangeSelButton_CMFCToolBarsCommandsPropertyPage__QEAAXPEAVCMFCToolBarButton___Z(void* pThis, CMFCToolBarButton* pButton) {
    if (pThis == nullptr) return;
    S_CmdsPage* d = D(pThis);
    d->m_strButtonDescription.Empty();
    if (pButton != nullptr) {
        if (pButton->m_nID == 0) {
            d->m_strButtonDescription = pButton->m_strText;
        } else {
            CFrameWnd* pParent = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(Wnd(pThis));
            if (pParent != nullptr && pParent->m_hWnd != nullptr) {
                impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                    pParent, pButton->m_nID, &d->m_strButtonDescription);
            }
        }
    }
    d->m_pSelButton = pButton;
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);
}

// Retail (RVA 0x17bd50, mfc140u), fully transcribed:
//     CDialog::OnInitDialog();                       // direct call 0x208d10, result ignored
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(
//         CMFCToolBarsCustomizeDialog, CWnd::FromHandle(::GetParent(m_hWnd)));
//     ENSURE(pWndParent != NULL);                    // NULL or !IsKindOf -> 0x227720
//     pWndParent->FillCategoriesListBox(m_wndCategory, TRUE);   // 0x1794c0, +0x158
//     m_wndCategory.SetCurSel(0);                    // SendMessageW(+0x198, LB_SETCURSEL 0x186, 0, 0)
//     OnSelchangeCategory();                         // 0x17b940
//     return TRUE;
// DEVIATIONS: (5) for the parent test.  The base call goes to OpenMFC's
// CDialog::OnInitDialog thunk, which does not run retail's UpdateData(FALSE)
// (deviation (1)), so m_wndCategory is not attached by it.
// Symbol: ?OnInitDialog@CMFCToolBarsCommandsPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsCommandsPropertyPage__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_CmdsPage* d = D(pThis);
    impl__OnInitDialog_CDialog__UEAAHXZ(reinterpret_cast<CDialog*>(pThis));

    void* pWndParent = ParentCustomizeDialog(pThis);
    if (pWndParent == nullptr) {
        ThrowInvalidArg();
        return FALSE;   // not reached: the throw does not return
    }
    impl__FillCategoriesListBox_CMFCToolBarsCustomizeDialog__QEBAXAEAVCListBox__H_Z(pWndParent, Wnd(d->m_wndCategory), TRUE);
    ::SendMessage(HwndOf(d->m_wndCategory), LB_SETCURSEL, 0, 0);
    impl__OnSelchangeCategory_CMFCToolBarsCommandsPropertyPage__IEAAXXZ(pThis);
    return TRUE;
}

// Retail (RVA 0x17b940, mfc140u), fully transcribed:
//     UpdateData(TRUE);                              // 0x2910d0
//     int iSelIndex = m_wndCategory.GetCurSel();     // LB_GETCURSEL (0x188) on +0x198
//     if (iSelIndex == LB_ERR) return;
//     CWaitCursor wait;                              // AfxGetApp()->BeginWaitCursor() 0x1de7b0
//     m_wndTools.SetRedraw(FALSE);                   // WM_SETREDRAW (0xb), 0, on +0x280
//     m_wndTools.ResetContent();                     // LB_RESETCONTENT (0x184)
//     CString strCategory;
//     m_wndCategory.GetText(iSelIndex, strCategory); // 0x294030
//     BOOL bAllCommands = (strCategory == m_strAllCategory);   // wcscmp, +0x348
//     OnChangeSelButton(NULL);                       // 0x17bde0
//     CObList* pCategoryButtonsList = (CObList*)m_wndCategory.GetItemData(iSelIndex);   // LB_GETITEMDATA (0x199)
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(
//         CMFCToolBarsCustomizeDialog, CWnd::FromHandle(::GetParent(m_hWnd)));
//     ENSURE(pWndParent != NULL);
//     for (POSITION pos = pCategoryButtonsList->GetHeadPosition(); pos != NULL;) {   // head +0x08, unchecked
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)pCategoryButtonsList->GetNext(pos);   // data +0x10
//         ENSURE(pButton != NULL);
//         pButton->m_bUserButton = pButton->m_nID != (UINT)-1 &&
//             GetCmdMgr()->GetCmdImage(pButton->m_nID, FALSE) == -1;   // inlined: m_CommandIndex
//                                                    // (0x1803c1478 = GetCmdMgr() 0x1803c1470 + 8).Lookup 0x2f610
//         CString strText = pButton->m_strText;      // +0x38
//         if (!pButton->m_strTextCustom.IsEmpty() &&                                // +0x40
//             (bAllCommands || pWndParent->GetCountInCategory(strText, *pCategoryButtonsList) > 1))   // 0x179980
//             strText = pButton->m_strTextCustom;
//         int iIndex = -1;
//         if (bAllCommands) {                        // insert sorted
//             for (int i = 0; iIndex == -1 && i < m_wndTools.GetCount(); i++) {   // LB_GETCOUNT (0x18b)
//                 CString strCommand;
//                 m_wndTools.GetText(i, strCommand); // 0x294030 on +0x240
//                 if (strCommand > strText)          // wcscmp
//                     iIndex = m_wndTools.InsertString(i, strText);   // LB_INSERTSTRING (0x181)
//             }
//         }
//         if (iIndex == -1)
//             iIndex = m_wndTools.AddString(strText);                 // LB_ADDSTRING (0x180)
//         m_wndTools.SetItemData(iIndex, (DWORD_PTR)pButton);         // LB_SETITEMDATA (0x19a)
//     }
//     m_wndTools.SetRedraw(TRUE);                    // WM_SETREDRAW, 1
//     // ~strCategory, then ~CWaitCursor (0x7687c: AfxGetApp()->EndWaitCursor())
// DEVIATIONS: UpdateData is deviation (1); the list walk is deviation (4);
// the parent test is deviation (5).  CStringT's ATLENSURE on the PCXSTR
// operand of `==` and `>` (the AtlThrow(E_FAIL) branches at 0x17bcca /
// 0x17bcdb) cannot fire: a CString's buffer is never NULL.
// Symbol: ?OnSelchangeCategory@CMFCToolBarsCommandsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeCategory_CMFCToolBarsCommandsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CmdsPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), TRUE);

    const HWND hCategory = HwndOf(d->m_wndCategory);
    const HWND hTools = HwndOf(d->m_wndTools.m_base);
    const int iSelIndex = static_cast<int>(::SendMessage(hCategory, LB_GETCURSEL, 0, 0));
    if (iSelIndex == LB_ERR) return;

    WaitCursor wait;
    ::SendMessage(hTools, WM_SETREDRAW, FALSE, 0);
    ::SendMessage(hTools, LB_RESETCONTENT, 0, 0);

    CString strCategory;
    impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        reinterpret_cast<const CListBox*>(d->m_wndCategory), iSelIndex, &strCategory);
    const bool bAllCommands = std::wcscmp(strCategory.GetString(), d->m_strAllCategory.GetString()) == 0;

    impl__OnChangeSelButton_CMFCToolBarsCommandsPropertyPage__QEAAXPEAVCMFCToolBarButton___Z(pThis, nullptr);

    const CObList* pCategoryButtonsList = reinterpret_cast<const CObList*>(
        ::SendMessage(hCategory, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iSelIndex)), 0));

    void* pWndParent = ParentCustomizeDialog(pThis);
    if (pWndParent == nullptr) { ThrowInvalidArg(); return; }

    if (const ObListData* items = CategoryButtons(pCategoryButtonsList)) {
        ObListData::POSITION pos = items->GetHeadPosition();
        while (pos != ObListData::POSITION(nullptr)) {
            CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(items->GetNext(pos));
            if (pButton == nullptr) { ThrowInvalidArg(); return; }

            pButton->m_bUserButton =
                (pButton->m_nID != static_cast<UINT>(-1) &&
                 impl__GetCmdImage_CCommandManager__QEBAHIH_Z(impl__GetCmdMgr__YAPEAVCCommandManager__XZ(), pButton->m_nID, FALSE) == -1)
                    ? TRUE : FALSE;

            CString strText = pButton->m_strText;
            if (!pButton->m_strTextCustom.IsEmpty() &&
                (bAllCommands ||
                 impl__GetCountInCategory_CMFCToolBarsCustomizeDialog__QEBAHPEB_WAEBVCObList___Z(pWndParent, strText.GetString(), pCategoryButtonsList) > 1)) {
                strText = pButton->m_strTextCustom;
            }

            int iIndex = -1;
            if (bAllCommands) {
                for (int i = 0; iIndex == -1 && i < static_cast<int>(::SendMessage(hTools, LB_GETCOUNT, 0, 0)); i++) {
                    CString strCommand;
                    impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                        reinterpret_cast<const CListBox*>(d->m_wndTools.m_base), i, &strCommand);
                    if (StringGreater(strCommand, strText)) {
                        iIndex = static_cast<int>(::SendMessage(hTools, LB_INSERTSTRING,
                            static_cast<WPARAM>(static_cast<INT_PTR>(i)), reinterpret_cast<LPARAM>(strText.GetString())));
                    }
                }
            }
            if (iIndex == -1) {
                iIndex = static_cast<int>(::SendMessage(hTools, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(strText.GetString())));
            }
            ::SendMessage(hTools, LB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), reinterpret_cast<LPARAM>(pButton));
        }
    }

    ::SendMessage(hTools, WM_SETREDRAW, TRUE, 0);
}

// Retail (RVA 0x17bcf0, mfc140u), fully transcribed:
//     int iIndex = m_wndTools.GetCurSel();           // LB_GETCURSEL (0x188) on +0x280
//     CMFCToolBarButton* pButton = (iIndex == LB_ERR) ? NULL
//         : (CMFCToolBarButton*)m_wndTools.GetItemData(iIndex);   // LB_GETITEMDATA (0x199)
//     OnChangeSelButton(pButton);                    // tail jump 0x17bde0
// Symbol: ?OnSelchangeUserTools@CMFCToolBarsCommandsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeUserTools_CMFCToolBarsCommandsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CmdsPage* d = D(pThis);
    const HWND hTools = HwndOf(d->m_wndTools.m_base);
    const int iIndex = static_cast<int>(::SendMessage(hTools, LB_GETCURSEL, 0, 0));
    CMFCToolBarButton* pButton = nullptr;
    if (iIndex != LB_ERR) {
        pButton = reinterpret_cast<CMFCToolBarButton*>(
            ::SendMessage(hTools, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iIndex)), 0));
    }
    impl__OnChangeSelButton_CMFCToolBarsCommandsPropertyPage__QEAAXPEAVCMFCToolBarButton___Z(pThis, pButton);
}

// Retail (RVA 0x17bed0, mfc140u), fully transcribed:
//     ENSURE(lpszCategory != NULL);                  // 0x227720
//     m_strAllCategory = lpszCategory;               // SetString(psz, wcslen) tail jump 0x2e30, +0x348
// Symbol: ?SetAllCategory@CMFCToolBarsCommandsPropertyPage@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetAllCategory_CMFCToolBarsCommandsPropertyPage__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory) {
    if (pThis == nullptr) return;
    if (lpszCategory == nullptr) { ThrowInvalidArg(); return; }
    D(pThis)->m_strAllCategory = lpszCategory;
}

// Retail (RVA 0x17be90, mfc140u), fully transcribed:
//     ENSURE(lpszCategory != NULL);                  // 0x227720
//     m_strUserCategory = lpszCategory;              // SetString(psz, wcslen) tail jump 0x2e30, +0x340
// Symbol: ?SetUserCategory@CMFCToolBarsCommandsPropertyPage@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetUserCategory_CMFCToolBarsCommandsPropertyPage__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCategory) {
    if (pThis == nullptr) return;
    if (lpszCategory == nullptr) { ThrowInvalidArg(); return; }
    D(pThis)->m_strUserCategory = lpszCategory;
}
