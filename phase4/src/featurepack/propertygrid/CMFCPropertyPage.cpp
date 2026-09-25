// CMFCPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// ===========================================================================
// CMFCPropertyPage -- the feature-pack property page (afxpropertypage.h,
// derives CPropertyPage).  Members, in declaration order: CDialogImpl m_Impl;
// CMFCPropertySheetCategoryInfo* m_pCategory; int m_nIcon; int m_nSelIconNum;
// HTREEITEM m_hTreeNode.
//
// Bodies below were transcribed from the retail disassembly (the method
// described in the header of core/ole/COleControl.cpp).  The disassembler
// reads mfc140.dll, the ANSI twin; function bodies are byte-identical in
// mfc140u.dll, only their addresses differ.  Each function below cites both:
//
//   function             mfc140 RVA   mfc140u RVA
//   ctor ()              0xcd3e0      0xccf10
//   ctor (UINT,UINT)     0xcd4a0      0xccfd0
//   ctor (LPCTSTR,UINT)  0xcd510 (PEBD)  0xcd040 (PEB_W)
//   dtor                 0xcd5b0      0xcd0e0
//   CommonInit           0xcd580      0xcd0b0
//   CreateObject         0xcd3a0      0xcced0
//   OnActivate           0xcd5e0      0xcd110
//   PreTranslateMessage  0xcd610      0xcd140 *
//   OnSetActive          0xcd650      0xcd180 *
//   OnInitDialog         0xcd6c0      0xcd1f0 *
//   OnEraseBkgnd         0xcd7d0      0xcd300 *
//   OnNcActivate         0x3edd0      0x3ef80 *
//   SetActiveMenu        0x3f240      0x3f3f0 *
//   OnCommand            0x3f250      0x3f400 *
// (*) mfc140u_rva_symbols.json has no entry under these names.  Their mfc140u
// RVAs are the ones mfc140u.dll's export address table gives for the
// symbols' ordinals (e.g. OnEraseBkgnd = ordinal 9769 -> 0xcd300); they agree
// with a byte search for the mfc140 bodies and with the constant
// mfc140->mfc140u displacement of the neighbouring resolved exports (-0x4d0
// in the 0xcd... block, +0x1b0 in the 0x3f... block).  The four 0xcd...
// entries are the only exports at their RVA.  The three 0x3f... entries are
// identical-code-folded with the same-named CDialogEx methods
// (?OnNcActivate / ?SetActiveMenu / ?OnCommand @CDialogEx share those export
// RVAs), which is why the map files 0x3f400 under
// ?OnCommand@CDialogEx@@MEAAH_K_J@Z.
//
// Layout.  The class is not declared in OpenMFC's public headers, so
// S_MFCPropertyPage below is the only place its retail layout lives.  Every
// offset is a store in the retail constructor (0xcd3e0 mfc140):
//     +0x000  CPropertyPage base      (vftable store at +0x00)
//     +0x158  m_Impl vfptr            (CDialogImpl vftable store)
//     +0x160  m_Impl.m_Dlg            (mov %rbx,0x160(%rbx) -- CWnd& to the page)
//     +0x168  m_pCategory             (= NULL)
//     +0x170  m_nIcon                 (= -1)
//     +0x174  m_nSelIconNum           (= -1)
//     +0x178  m_hTreeNode             (= NULL)
// sizeof == 0x180 is what CreateObject passes to operator new, and matches
// the 384 that featurepack/propertygrid/RuntimeClasses.cpp records.
// ??0CDialogImpl@@IEAA@AEAVCWnd@@@Z (0x3f380 mfc140) confirms the m_Impl
// half: it stores the vftable at +0 and its CWnd& argument at +8.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores the CMFCPropertyPage vftable at +0x00
//      and the CDialogImpl vftable at +0x158.  OpenMFC has no MSVC-layout
//      CMFCPropertyPage vftable, so +0x00 keeps what the CPropertyPage
//      constructor thunk installs.  m_Impl is built through the exported
//      CDialogImpl constructor thunk instead of inline (retail inlines that
//      constructor; retail's exported one, 0x3f380 mfc140, performs exactly
//      the same two stores), so +0x158 holds whatever vfptr that thunk
//      installs.
//
//  (2) m_Impl.  Every m_Impl call goes to the exported CDialogImpl thunk
//      defined in core/dialog/CDialogImpl.cpp, declared below with the
//      signature its mangled name describes; this file only forwards, and
//      what the CDialogImpl half of each handler (popup-menu tracking for
//      CMFCPropertySheet's list/tree looks) does is up to those bodies.  The
//      page constructors zero m_Impl's 0x10 bytes before calling the
//      CDialogImpl constructor thunk, so the member is never left holding
//      whatever the allocation held even if that thunk stores nothing (its
//      committed version at the time this was written was such a
//      placeholder).
//
//  (3) Virtual calls on the parent CMFCPropertySheet.  Retail dispatches
//      through the sheet's vftable (slots 97 and 100).  An OpenMFC sheet does
//      not carry retail's vftable, so these calls cannot be made through the
//      object; each site says what it does instead.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the mangled names; the file that
// defines each is named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                                        // detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_XZ(void* pThis);                                                  // core/dialog/Thunks.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_PEB_WIK_Z(void* pThis, const wchar_t* lpszTemplateName, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);                                         // core/dialog/RuntimeClasses.cpp
extern "C" int   MS_ABI impl__OnSetActive_CPropertyPage__UEAAHXZ(CPropertyPage* pThis);                               // core/dialog/Thunks.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                          // detail/DlgcoreSupport.cpp
extern "C" int   MS_ABI impl__GetPageIndex_CPropertySheet__QEAAHPEAVCPropertyPage___Z(CPropertySheet* pThis, CPropertyPage* pPage);   // core/dialog/Thunks.cpp

extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                                  // core/window/Thunks.cpp
extern "C" CWnd*   MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                   // core/window/CWnd.cpp
extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);   // core/window/CWnd.cpp
extern "C" int     MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);              // core/window/CWnd.cpp
extern "C" int     MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);                // core/window/CWnd.cpp
extern "C" void    MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);          // core/window/Thunks.cpp
extern "C" int     MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" int     MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ();                    // featurepack/propertygrid/RuntimeClasses.cpp
// featurepack/propertygrid/CMFCPropertySheet.cpp.  CRect by value is passed by
// hidden pointer to a caller-owned copy under the x64 MS ABI.
extern "C" void    MS_ABI impl__OnDrawPageHeader_CMFCPropertySheet__UEAAXPEAVCDC__HVCRect___Z(void* pThis, CDC* pDC, int nPage, CRect* pRectHeader);

// core/dialog/CDialogImpl.cpp -- see deviation (2).
extern "C" void* MS_ABI impl___0CDialogImpl__IEAA_AEAVCWnd___Z(void* pThis, void* pDlg);
extern "C" void  MS_ABI impl___1CDialogImpl__MEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__OnActivate_CDialogImpl__IEAAXIPEAVCWnd___Z(void* pThis, unsigned int nState, CWnd* pWndOther);
extern "C" int   MS_ABI impl__OnCommand_CDialogImpl__IEAAH_K_J_Z(void* pThis, WPARAM wParam, LPARAM lParam);
extern "C" void  MS_ABI impl__OnNcActivate_CDialogImpl__IEAAXAEAH_Z(void* pThis, int* pbActive);
extern "C" int   MS_ABI impl__PreTranslateMessage_CDialogImpl__IEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg);
extern "C" void  MS_ABI impl__SetActiveMenu_CDialogImpl__IEAAXPEAVCMFCPopupMenu___Z(void* pThis, void* pMenu);

namespace {

struct S_MFCPropertyPage {
    alignas(8) unsigned char m_base[0x158];   // +0x000 CPropertyPage
    void*     m_Impl_vfptr;                   // +0x158 CDialogImpl vftable (deviation (1))
    void*     m_Impl_m_Dlg;                   // +0x160 CDialogImpl::m_Dlg (CWnd&)
    void*     m_pCategory;                    // +0x168 CMFCPropertySheetCategoryInfo*
    int       m_nIcon;                        // +0x170
    int       m_nSelIconNum;                  // +0x174
    HTREEITEM m_hTreeNode;                    // +0x178
};
static_assert(offsetof(S_MFCPropertyPage, m_Impl_vfptr) == 0x158, "retail: m_Impl at +0x158");
static_assert(offsetof(S_MFCPropertyPage, m_Impl_m_Dlg) == 0x160, "retail: m_Impl.m_Dlg at +0x160");
static_assert(offsetof(S_MFCPropertyPage, m_pCategory) == 0x168, "retail: m_pCategory at +0x168");
static_assert(offsetof(S_MFCPropertyPage, m_nIcon) == 0x170, "retail: m_nIcon at +0x170");
static_assert(offsetof(S_MFCPropertyPage, m_nSelIconNum) == 0x174, "retail: m_nSelIconNum at +0x174");
static_assert(offsetof(S_MFCPropertyPage, m_hTreeNode) == 0x178, "retail: m_hTreeNode at +0x178");
static_assert(sizeof(S_MFCPropertyPage) == 0x180, "retail CreateObject: operator new(0x180)");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");

inline S_MFCPropertyPage* D(void* p) { return static_cast<S_MFCPropertyPage*>(p); }
inline void* Impl(void* p) { return &D(p)->m_Impl_vfptr; }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(static_cast<CPropertyPage*>(p)); }

// Retail offsets inside CMFCPropertySheet (afxpropertysheet.h), read raw from
// the parent sheet by OnInitDialog / OnEraseBkgnd:
//   +0x190   PropSheetLook m_look   -- SetLook (0xce930 mfc140) stores its first
//                                      argument there
//   +0x71d8  int m_nHeaderHeight    -- EnablePageHeader (0xd08c0 mfc140) stores
//                                      its argument there
// OpenMFC's CMFCPropertySheet (featurepack/propertygrid/CMFCPropertySheet.cpp)
// keeps its look and page-header state in a side table instead and zero-fills
// its 29232-byte object on construction, so on an OpenMFC sheet both fields
// read 0 today and the header code paths below do not run.  The reads stay in
// bounds: 0x71d8 + 4 < 29232.
constexpr std::size_t kSheetOffLook         = 0x190;
constexpr std::size_t kSheetOffHeaderHeight = 0x71d8;
constexpr int kLookOutlookBar = 1;   // PropSheetLook_OutlookBar
constexpr int kLookList       = 4;   // PropSheetLook_List

inline int SheetInt(const CWnd* pSheet, std::size_t off) {
    int v;
    std::memcpy(&v, reinterpret_cast<const unsigned char*>(pSheet) + off, sizeof v);
    return v;
}

// DYNAMIC_DOWNCAST(CMFCPropertySheet, CWnd::FromHandle(::GetParent(m_hWnd))),
// the lookup OnInitDialog and OnEraseBkgnd both open with:
//     mov 0x40(%rcx),%rcx ; call GetParent (IAT) ; call CWnd::FromHandle ;
//     test/je ; IsKindOf(pWnd, &classCMFCPropertySheet) ; test/je
inline CWnd* ParentMFCSheet(void* pThis) {
    CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Wnd(pThis)->m_hWnd));
    if (pWnd == nullptr) return nullptr;
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pWnd, impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return pWnd;
}

// The member initialisation every constructor ends with -- the same four
// stores CommonInit performs (see its transcription below).
inline void InitMembers(void* pThis) {
    S_MFCPropertyPage* d = D(pThis);
    d->m_pCategory = nullptr;
    d->m_nIcon = -1;
    d->m_nSelIconNum = -1;
    d->m_hTreeNode = nullptr;
}

// Retail constructs m_Impl inline: CDialogImpl vftable at +0x158, `this` at
// +0x160.  Here: zero the member (deviation (2)), then the exported
// CDialogImpl constructor thunk with m_Dlg = the page (deviation (1)).
inline void ConstructImpl(void* pThis) {
    std::memset(Impl(pThis), 0, 0x10);
    impl___0CDialogImpl__IEAA_AEAVCWnd___Z(Impl(pThis), Wnd(pThis));
}

} // namespace

// Retail ctor () (0xcd3e0 mfc140 / 0xccf10 mfc140u), fully transcribed:
//     CPropertyPage::CPropertyPage();                     // call ??0CPropertyPage@@QEAA@XZ
//     <store CMFCPropertyPage vftable at +0x00>
//     m_Impl: vftable at +0x158, m_Dlg = this at +0x160    // inlined CDialogImpl(*this)
//     m_nIcon = m_nSelIconNum = -1;  m_pCategory = NULL;  m_hTreeNode = NULL;
// Deviations (1) and (2) apply to the two vftable stores / m_Impl.
// This export was not on this file's work list (its previous body only
// returned pThis), but CreateObject and every handler depend on the members
// being initialised, so it is implemented with them; likewise the other two
// constructors.
// Symbol: ??0CMFCPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCPropertyPage__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    impl___0CPropertyPage__QEAA_XZ(pThis);
    ConstructImpl(pThis);
    InitMembers(pThis);
    return pThis;
}

// Retail ctor (UINT, UINT) (0xcd4a0 mfc140 / 0xccfd0 mfc140u): identical to
// the default constructor except the base call, which is
//     CPropertyPage::CPropertyPage(nIDTemplate, nIDCaption, 0x68);   // ??0CPropertyPage@@QEAA@IIK@Z
// with the two arguments passed through untouched (mov $0x68,%r9d).
// Symbol: ??0CMFCPropertyPage@@QEAA@II@Z
extern "C" void* MS_ABI impl___0CMFCPropertyPage__QEAA_II_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption) {
    if (pThis == nullptr) return nullptr;
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, nIDTemplate, nIDCaption, 0x68);
    ConstructImpl(pThis);
    InitMembers(pThis);
    return pThis;
}

// Retail ctor (LPCTSTR, UINT) (0xcd040 mfc140u; the mfc140 twin is the PEBD
// overload at 0xcd510): identical to the default constructor except the base
// call, which is
//     CPropertyPage::CPropertyPage(lpszTemplateName, nIDCaption, 0x68);   // ??0CPropertyPage@@QEAA@PEB_WIK@Z
// Symbol: ??0CMFCPropertyPage@@QEAA@PEB_WI@Z
extern "C" void* MS_ABI impl___0CMFCPropertyPage__QEAA_PEB_WI_Z(void* pThis, const wchar_t* lpszTemplateName, unsigned int nIDCaption) {
    if (pThis == nullptr) return nullptr;
    impl___0CPropertyPage__QEAA_PEB_WIK_Z(pThis, lpszTemplateName, nIDCaption, 0x68);
    ConstructImpl(pThis);
    InitMembers(pThis);
    return pThis;
}

// Retail dtor (0xcd5b0 mfc140 / 0xcd0e0 mfc140u), fully transcribed:
//     <store CMFCPropertyPage vftable at +0x00>
//     <store CDialogImpl vftable at +0x158>          // inlined ~CDialogImpl, whose
//                                                    // exported body (0x3f3d0 mfc140)
//                                                    // is that one store
//     CPropertyPage::~CPropertyPage();               // tail jump ??1CPropertyPage@@UEAA@XZ
// The vftable stores are deviation (1); m_Impl is torn down through the
// exported CDialogImpl destructor thunk, deviation (2).
// Symbol: ??1CMFCPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    impl___1CDialogImpl__MEAA_XZ(Impl(pThis));
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (0xcd580 mfc140 / 0xcd0b0 mfc140u), fully transcribed:
//     m_pCategory = NULL;  m_nIcon = -1;  m_nSelIconNum = -1;  m_hTreeNode = NULL;
// Symbol: ?CommonInit@CMFCPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__CommonInit_CMFCPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    InitMembers(pThis);
}

// Retail (0xcd3a0 mfc140 / 0xcced0 mfc140u), fully transcribed:
//     void* p = operator new(0x180);                      // ??2@YAPEAX_K@Z
//     return p ? new(p) CMFCPropertyPage() : NULL;        // 0xcd3e0 mfc140
// Symbol: ?CreateObject@CMFCPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_MFCPropertyPage));
    if (p == nullptr) return nullptr;
    return impl___0CMFCPropertyPage__QEAA_XZ(p);
}

// Retail (0xcd5e0 mfc140 / 0xcd110 mfc140u), fully transcribed:
//     m_Impl.OnActivate(nState, pWndOther);    // ?OnActivate@CDialogImpl@@IEAAXIPEAVCWnd@@@Z
//                                              // (rdx/r8 passed through; bMinimized is not)
//     Default();                               // tail jump ?Default@CWnd@@IEAA_JXZ
// m_Impl.OnActivate goes to the exported CDialogImpl thunk, deviation (2).
// Symbol: ?OnActivate@CMFCPropertyPage@@IEAAXIPEAVCWnd@@H@Z
extern "C" void MS_ABI impl__OnActivate_CMFCPropertyPage__IEAAXIPEAVCWnd__H_Z(void* pThis, unsigned int nState, CWnd* pWndOther, int bMinimized) {
    (void)bMinimized;   // retail does not forward it either
    if (pThis == nullptr) return;
    impl__OnActivate_CDialogImpl__IEAAXIPEAVCWnd___Z(Impl(pThis), nState, pWndOther);
    impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
}

// Retail (0x3f250 mfc140 / 0x3f400 mfc140u), fully transcribed:
//     if (m_Impl.OnCommand(wParam, lParam))    // ?OnCommand@CDialogImpl@@IEAAH_K_J@Z
//         return TRUE;
//     return CWnd::OnCommand(wParam, lParam);  // ?OnCommand@CWnd@@MEAAH_K_J@Z
// m_Impl.OnCommand goes to the exported CDialogImpl thunk, deviation (2).
// Symbol: ?OnCommand@CMFCPropertyPage@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCPropertyPage__MEAAH_K_J_Z(void* pThis, WPARAM wParam, LPARAM lParam) {
    if (pThis == nullptr) return FALSE;
    if (impl__OnCommand_CDialogImpl__IEAAH_K_J_Z(Impl(pThis), wParam, lParam)) {
        return TRUE;
    }
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(Wnd(pThis), wParam, lParam);
}

// Retail (0xcd7d0 mfc140 / 0xcd300 mfc140u), fully transcribed:
//     BOOL bRes = (BOOL)Default();                            // ?Default@CWnd@@IEAA_JXZ
//     CMFCPropertySheet* pParent = <ParentMFCSheet(this)>;
//     if (pParent != NULL && pParent->m_nHeaderHeight > 0) {  // cmpl $0,0x71d8 ; jle
//         CRect rectClient;  ::GetClientRect(m_hWnd, &rectClient);   // zeroed first
//         int nRight = rectClient.right;
//         int look = pParent->m_look;                          // +0x190
//         if (look == OutlookBar || look == Tree || look == List) {  // (look-1)<=1 || look==4
//             CRect rectParent;  ::GetWindowRect(pParent->m_hWnd, &rectParent);   // zeroed first
//             ScreenToClient(&rectParent);                     // this page's CWnd::ScreenToClient
//             nRight = rectParent.right - ::GetSystemMetrics(7);   // SM_CXDLGFRAME
//         }
//         pParent->OnDrawPageHeader(pDC, pParent->GetPageIndex(this),    // vftable slot 100 (+0x320)
//             CRect(rectClient.left, rectClient.top, nRight,
//                   rectClient.top + pParent->m_nHeaderHeight));
//     }
//     return bRes;
// Import slots resolved with iat.py (mfc140): 0x1802c5300 GetParent,
// 0x1802c5358 GetClientRect, 0x1802c5370 GetWindowRect, 0x1802c4c50
// GetSystemMetrics.  Slot 100 was read from retail's CMFCPropertySheet
// vftable (the one ??1CMFCPropertySheet@@UEAA@XZ, 0xce970 mfc140, stores):
// it holds a bare `ret`, as does the mfc140u export of
// CMFCPropertySheet::OnDrawPageHeader (ordinal 9459, 0x27d0 mfc140u), so the
// retail base implementation draws nothing and only an override draws.
// DEVIATION (3): the virtual call is made to the exported
// CMFCPropertySheet::OnDrawPageHeader thunk; overrides are not reached, and
// OpenMFC's body of that thunk currently draws a caption where retail's does
// nothing.  The m_look / m_nHeaderHeight reads are raw retail offsets; on an
// OpenMFC sheet they read 0 (see kSheetOffHeaderHeight), so this branch does
// not run there today.
// Symbol: ?OnEraseBkgnd@CMFCPropertyPage@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCPropertyPage__IEAAHPEAVCDC___Z(void* pThis, CDC* pDC) {
    if (pThis == nullptr) return FALSE;
    const int bRes = static_cast<int>(impl__Default_CWnd__IEAA_JXZ(Wnd(pThis)));
    CWnd* pParent = ParentMFCSheet(pThis);
    if (pParent == nullptr) return bRes;
    const int nHeaderHeight = SheetInt(pParent, kSheetOffHeaderHeight);
    if (nHeaderHeight <= 0) return bRes;

    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(Wnd(pThis)->m_hWnd, &rectClient);
    int nRight = rectClient.right;
    const int look = SheetInt(pParent, kSheetOffLook);
    if (static_cast<unsigned int>(look - kLookOutlookBar) <= 1u || look == kLookList) {
        RECT rectParent = {0, 0, 0, 0};
        ::GetWindowRect(pParent->m_hWnd, &rectParent);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(Wnd(pThis), &rectParent);
        nRight = rectParent.right - ::GetSystemMetrics(SM_CXDLGFRAME);
    }
    const int nPage = impl__GetPageIndex_CPropertySheet__QEAAHPEAVCPropertyPage___Z(
        static_cast<CPropertySheet*>(pParent), static_cast<CPropertyPage*>(pThis));
    CRect rectHeader(rectClient.left, rectClient.top, nRight, rectClient.top + nHeaderHeight);
    impl__OnDrawPageHeader_CMFCPropertySheet__UEAAXPEAVCDC__HVCRect___Z(pParent, pDC, nPage, &rectHeader);   // deviation (3)
    return bRes;
}

// Retail (0xcd6c0 mfc140 / 0xcd1f0 mfc140u), fully transcribed:
//     BOOL bRes = CDialog::OnInitDialog();                     // ?OnInitDialog@CDialog@@UEAAHXZ
//     CMFCPropertySheet* pParent = <ParentMFCSheet(this)>;
//     if (pParent != NULL && pParent->m_nHeaderHeight != 0) {  // test esi,esi ; je
//         int nHeaderHeight = pParent->m_nHeaderHeight;
//         for (CWnd* pChild = CWnd::FromHandle(::GetWindow(m_hWnd, GW_CHILD));
//              pChild != NULL;
//              pChild = CWnd::FromHandle(::GetWindow(pChild->m_hWnd, GW_HWNDNEXT))) {
//             CRect rect;  ::GetWindowRect(pChild->m_hWnd, &rect);   // zeroed first
//             ScreenToClient(&rect);                            // this page's
//             ::OffsetRect(&rect, 0, nHeaderHeight);
//             pChild->SetWindowPos(NULL, rect.left, rect.top, -1, -1,
//                                  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);   // 0x15
//         }
//     }
//     return bRes;
// Import slots resolved with iat.py (mfc140): 0x1802c5300 GetParent,
// 0x1802c4c88 GetWindow (edx = 5 GW_CHILD, then 2 GW_HWNDNEXT),
// 0x1802c5370 GetWindowRect, 0x1802c5318 OffsetRect.
// As in OnEraseBkgnd, m_nHeaderHeight is read at its raw retail offset and is
// 0 on an OpenMFC sheet today, so the loop does not run there.
// Symbol: ?OnInitDialog@CMFCPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCPropertyPage__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    const int bRes = impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(static_cast<CPropertyPage*>(pThis)));
    CWnd* pParent = ParentMFCSheet(pThis);
    if (pParent == nullptr) return bRes;
    const int nHeaderHeight = SheetInt(pParent, kSheetOffHeaderHeight);
    if (nHeaderHeight == 0) return bRes;

    for (CWnd* pChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(Wnd(pThis)->m_hWnd, GW_CHILD));
         pChild != nullptr;
         pChild = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetWindow(pChild->m_hWnd, GW_HWNDNEXT))) {
        RECT rect = {0, 0, 0, 0};
        ::GetWindowRect(pChild->m_hWnd, &rect);
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(Wnd(pThis), &rect);
        ::OffsetRect(&rect, 0, nHeaderHeight);
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(pChild, nullptr, rect.left, rect.top, -1, -1,
                                                   SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    return bRes;
}

// Retail (0x3edd0 mfc140 / 0x3ef80 mfc140u), fully transcribed:
//     m_Impl.OnNcActivate(bActive);            // ?OnNcActivate@CDialogImpl@@IEAAXAEAH@Z
//                                              // (BOOL&: may rewrite bActive)
//     return (BOOL)DefWindowProc(WM_NCACTIVATE, bActive, 0L);
// DefWindowProc is a virtual call through this page's vftable at +0x248
// (slot 73); retail's CMFCPropertyPage vftable holds
// ?DefWindowProcA@CWnd@@MEAA_JI_K_J@Z there in mfc140 (the ANSI twin of
// DefWindowProcW).  It is devirtualised here to the exported
// CWnd::DefWindowProc thunk: OpenMFC objects do not carry retail's vftable,
// so an override of DefWindowProc in a derived page is not reached.
// m_Impl.OnNcActivate goes to the exported CDialogImpl thunk, deviation (2);
// bActive is passed by address so that thunk can rewrite it, as retail's does.
// Symbol: ?OnNcActivate@CMFCPropertyPage@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CMFCPropertyPage__IEAAHH_Z(void* pThis, int bActive) {
    if (pThis == nullptr) return FALSE;
    impl__OnNcActivate_CDialogImpl__IEAAXAEAH_Z(Impl(pThis), &bActive);
    return static_cast<int>(impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
        Wnd(pThis), WM_NCACTIVATE, static_cast<WPARAM>(static_cast<LONG_PTR>(bActive)), 0));   // movslq: sign-extended
}

// Retail (0xcd650 mfc140 / 0xcd180 mfc140u):
//     CMFCPropertySheet* pParent = <ParentMFCSheet(this)>;
//     if (pParent != NULL)
//         pParent->OnActivatePage(this);       // vftable slot 97 (+0x308)
//     return CPropertyPage::OnSetActive();     // tail jump ?OnSetActive@CPropertyPage@@UEAAHXZ
// Slot 97 of retail's CMFCPropertySheet vftable is
// ?OnActivatePage@CMFCPropertySheet@@UEAAXPEAVCPropertyPage@@@Z (0xcff90
// mfc140), which synchronises the sheet's navigation controls.
// DEVIATION (3), and an omission: the OnActivatePage call is NOT made.  The
// only body reachable without retail's vftable is OpenMFC's exported thunk
// (featurepack/propertygrid/CMFCPropertySheet.cpp), and that body calls
// CPropertySheet::SetActivePage(pPage) -> PropSheet_SetCurSel, which is not
// what retail's OnActivatePage does: its direct callees are GetPageIndex,
// FindPageIndexInList, IsKindOf, ::SendMessage and ::PostMessage (import slots
// 0x1802c5378 / 0x1802c52d8, iat.py) plus three virtual calls, and
// CPropertySheet::SetActivePage, which is non-virtual, is not among them.
// Called from here, i.e. while the page is handling PSN_SETACTIVE, it would
// re-enter page activation.  The parent lookup is omitted with it because
// nothing else uses its result.
// Symbol: ?OnSetActive@CMFCPropertyPage@@UEAAHXZ
extern "C" int MS_ABI impl__OnSetActive_CMFCPropertyPage__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    return impl__OnSetActive_CPropertyPage__UEAAHXZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (0xcd610 mfc140 / 0xcd140 mfc140u):
//     if (m_Impl.PreTranslateMessage(pMsg))    // ?PreTranslateMessage@CDialogImpl@@IEAAHPEAUtagMSG@@@Z
//         return TRUE;
//     CWnd::PreTranslateMessage(pMsg);         // ?PreTranslateMessage@CWnd@@UEAAHPEAUtagMSG@@@Z
//     return FALSE;                            // xor %eax,%eax after the call
// Retail may drop the base call's result because its CWnd::PreTranslateMessage
// (0x28a080 mfc140) always returns 0: it calls an optional function pointer
// (loaded from +0x80 of the structure at this+0x38) and ends in
// xor %eax,%eax on every path.
// DEVIATION: the base result is returned here instead of discarded.  OpenMFC's
// CWnd::PreTranslateMessage thunk (core/window/CWnd.cpp ->
// CWnd_PreTranslateMessageCompat) calls ::IsDialogMessageW and returns TRUE
// when that consumed the message; discarding it would make the pump dispatch
// an already-processed message a second time.  Where the thunk returns FALSE
// the result is retail's.  m_Impl.PreTranslateMessage goes to the exported
// CDialogImpl thunk, deviation (2).
// Symbol: ?PreTranslateMessage@CMFCPropertyPage@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCPropertyPage__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr) return FALSE;
    if (impl__PreTranslateMessage_CDialogImpl__IEAAHPEAUtagMSG___Z(Impl(pThis), pMsg)) {
        return TRUE;
    }
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(Wnd(pThis), pMsg);
}

// Retail (0x3f240 mfc140 / 0x3f3f0 mfc140u), fully transcribed:
//     m_Impl.SetActiveMenu(pMenu);             // add $0x158,%rcx ; tail jump
//                                              // ?SetActiveMenu@CDialogImpl@@IEAAXPEAVCMFCPopupMenu@@@Z
// The pointer is forwarded opaquely; no CMFCPopupMenu layout is needed.  The
// callee is the exported CDialogImpl thunk, deviation (2).
// Symbol: ?SetActiveMenu@CMFCPropertyPage@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__SetActiveMenu_CMFCPropertyPage__IEAAXPEAVCMFCPopupMenu___Z(void* pThis, void* pMenu) {
    if (pThis == nullptr) return;
    impl__SetActiveMenu_CDialogImpl__IEAAXPEAVCMFCPopupMenu___Z(Impl(pThis), pMenu);
}
