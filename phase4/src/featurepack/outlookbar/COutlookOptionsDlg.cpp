// COutlookOptionsDlg — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>

// ===========================================================================
// COutlookOptionsDlg -- the "Navigation Pane Options" dialog shown by
// CMFCOutlookBarTabCtrl::OnShowOptions.  It derives CDialog and is not declared
// in the public SDK headers (afxoutlookbartabctrl.h:78 only names it as a
// friend), nor anywhere in include/openmfc, so `this` is taken as void* and the
// layout is pinned in-file below.
//
// Every implemented body was transcribed from the retail disassembly (the
// method described in the header of core/ole/COleControl.cpp); the constructor
// and OnReset are still stubs (see each).  All RVAs below are mfc140u
// function ENTRIES.  The exports were resolved through their ordinal and
// mfc140u.dll's export address table (six of them -- OnInitDialog, OnMoveDown,
// OnMoveUp, OnOK, OnReset, OnSelchange -- have no entry in
// mfc140u_rva_symbols.json); the non-exported scalar deleting destructor is
// vftable slot 1.  The bodies were read in mfc140.dll, whose code is
// byte-identical, and the vftable slot numbers below come from its
// COutlookOptionsDlg vftable:
//
//   ??0COutlookOptionsDlg (ctor)       0x9e850     OnDblclkList   0x9eab0
//   scalar deleting dtor (vslot 1)     0x9e8e0     OnMoveDown     0x9eb10
//   DoDataExchange       (vslot 66)    0x9e9a0     OnMoveUp       0x9eb20
//   OnSelchange                        0x9ea20     OnInitDialog   0x9eb30 (vslot 96)
//   OnOK                 (vslot 98)    0x9ed10     OnReset        0x9ee60
//   MoveItem                           0x9f0c0
//
// Layout, from the constructor (0x9e850, mfc140u):
//     CDialog::CDialog(IDD_AFXBARRES_OUTLOOKBAR_OPTIONS /*17004*/, &parent);
//     +0x130, +0x218, +0x300: CWnd::CWnd() then the CButton vftable
//     +0x3e8: CMFCToolBarsListCheckBox::CMFCToolBarsListCheckBox()
//     +0x500: the CMFCOutlookBarTabCtrl& parent (stored as a pointer)
// and the scalar deleting destructor (0x9e8e0) passes 0x508 as the size to its
// sized-delete path, destroying +0x3e8, +0x300, +0x218, +0x130 then CDialog.
// Which button is which follows from the handlers: OnSelchange enables +0x130
// when the selection is > 0 (move up) and +0x218 when it is < count-1 (move
// down); OnInitDialog hides +0x300 (reset); DoDataExchange binds them to
// IDC_AFXBARRES_MOVEUP 17025, IDC_AFXBARRES_MOVEDOWN 17026 and
// IDC_AFXBARRES_RESET 16613, and the list to IDC_AFXBARRES_LIST 16923.
//
// The parent's retail vtable calls were checked against the
// CMFCOutlookBarTabCtrl vftable (the one its constructor stores): slot 108
// (+0x360) ShowTab, slot 113 (+0x388) GetTabLabel, slot 138 (+0x450)
// GetTabByID and slot 161 (+0x508) IsTabVisible all hold the CMFCBaseTabCtrl
// implementations -- CMFCOutlookBarTabCtrl overrides none of them
// (afxoutlookbartabctrl.h).
//
// Structural deviations, applied uniformly:
//
//  (1) Devirtualised calls.  OpenMFC's CMFCBaseTabCtrl C++ vtable does not
//      have the retail slot numbering, so the four parent virtuals above are
//      called through their exported CMFCBaseTabCtrl thunks, and the thread's
//      GetMainWnd (retail: CWinThread vslot 31) through the CWinThread thunk.
//      An override in a client-derived tab control or thread is bypassed.
//
//  (2) The parent's tab count.  Retail reads CMFCBaseTabCtrl::m_iTabsNum at
//      parent+0x150 (the inline GetTabsNum()).  That read is kept, but OpenMFC
//      does not maintain +0x150: its tabs live in the side table behind
//      CMFCBaseTabCtrl::GetTabsCount(), and +0x150 falls inside the opaque
//      padding of OpenMFC's CMFCBaseTabCtrl (afxmfc.h:1086), which its C++
//      constructor zeroes and which models no m_iTabsNum.  So on such a tab
//      control OnInitDialog lists no tabs.  (GetTabsCount is a C++ member with
//      no impl__ thunk; calling it from here fails checkfile's link audit.)
//
//  (3) CArray<int,int>.  OnOK builds a local CArray<int,int> in the retail x64
//      layout (vfptr, m_pData, then INT_PTR size/max/grow) and passes it to
//      CMFCBaseTabCtrl::SetTabsOrder, whose retail body reads m_nSize at +0x10.
//      The vfptr is left NULL (OpenMFC has no MSVC CArray<int,int> vftable) and
//      the storage uses malloc/free (retail's destructor frees with CRT free).
//
//  (4) NULL guards.  Retail dereferences m_parent (+0x500) without testing
//      it; here the parent-dependent calls are skipped when it is NULL, and
//      every export returns early on a NULL `this`.
//
// NOT REACHABLE TODAY (outside this file): the exported constructor above is
// still a stub that leaves the object uninitialised, OpenMFC's OnShowOptions
// never creates this dialog, CWnd::UpdateData never calls DoDataExchange and
// OpenMFC's DDX_Control (core/runtime/DdxExchange.cpp) only looks the control
// up -- it does not attach it -- so m_wndList / the buttons keep a NULL m_hWnd
// in an OpenMFC-run dialog, and every SendMessage below then goes to a NULL
// window and returns 0.  In addition, OpenMFC's COutlookOptionsDlg message map
// (detail/Other12MsgmapSupport.cpp) has no entries, so none of the handlers
// below is reached through message dispatch.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each signature was checked against the definition
// named on its line.
// ---------------------------------------------------------------------------
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                           // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* pThis);                                                   // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);       // core/runtime/DdxExchange.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                     // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                                   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                                  // core/window/CWnd.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);                                    // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ();                        // featurepack/outlookbar/RuntimeClasses.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                                             // core/app/Globals.cpp
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);                             // core/app/CWinThread.cpp
extern "C" void  MS_ABI impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CListBox* pThis, int nIndex, CString* rString);   // core/controls/CListBox.cpp
extern "C" int   MS_ABI impl__GetCheck_CCheckListBox__QEAAHH_Z(CCheckListBox* pThis, int nIndex);                    // core/controls/CCheckListBox.cpp
extern "C" void  MS_ABI impl__SetCheck_CCheckListBox__QEAAXHH_Z(CCheckListBox* pThis, int nIndex, int nCheck);        // core/controls/CCheckListBox.cpp
extern "C" int   MS_ABI impl__GetTabLabel_CMFCBaseTabCtrl__UEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CMFCBaseTabCtrl* pThis, int iTab, CString& strLabel);   // featurepack/tabs/CMFCBaseTabCtrl.cpp
extern "C" int   MS_ABI impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(const CMFCBaseTabCtrl* pThis, int iTab);         // featurepack/tabs/CMFCBaseTabCtrl.cpp
extern "C" int   MS_ABI impl__ShowTab_CMFCBaseTabCtrl__UEAAHHHHH_Z(CMFCBaseTabCtrl* pThis, int iTab, int bShow, int bRecalcLayout, int bActivate);   // featurepack/tabs/CMFCBaseTabCtrl.cpp
extern "C" int   MS_ABI impl__SetTabsOrder_CMFCBaseTabCtrl__QEAAHAEBV__CArray_HH___Z(CMFCBaseTabCtrl* pThis, const CArray<int, int>& arOrder);   // featurepack/tabs/CMFCBaseTabCtrl.cpp

// This file's own thunks that earlier bodies call.
extern "C" void MS_ABI impl__OnSelchange_COutlookOptionsDlg__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__MoveItem_COutlookOptionsDlg__IEAAXH_Z(void* pThis, int bMoveUp);

namespace {

// ---------------------------------------------------------------------------
// The retail object (0x508 bytes), offsets from the constructor (0x9e850).
// ---------------------------------------------------------------------------
struct S_OutlookOptionsDlg {
    alignas(8) unsigned char m_base[0x130];         // +0x000 CDialog (OpenMFC sizeof(CDialog) == 0x130, asserted below)
    alignas(8) unsigned char m_btnMoveUp[0xe8];     // +0x130 CButton (IDC_AFXBARRES_MOVEUP)
    alignas(8) unsigned char m_btnMoveDown[0xe8];   // +0x218 CButton (IDC_AFXBARRES_MOVEDOWN)
    alignas(8) unsigned char m_btnReset[0xe8];      // +0x300 CButton (IDC_AFXBARRES_RESET)
    alignas(8) unsigned char m_wndList[0x118];      // +0x3e8 CMFCToolBarsListCheckBox (IDC_AFXBARRES_LIST)
    CMFCBaseTabCtrl*         m_pParent;             // +0x500 CMFCOutlookBarTabCtrl& m_parent
};
static_assert(offsetof(S_OutlookOptionsDlg, m_btnMoveUp) == 0x130, "ctor: lea 0x130(%rsi),%rbx; CWnd ctor");
static_assert(offsetof(S_OutlookOptionsDlg, m_btnMoveDown) == 0x218, "ctor: lea 0x218(%rsi),%rbx; CWnd ctor");
static_assert(offsetof(S_OutlookOptionsDlg, m_btnReset) == 0x300, "ctor: lea 0x300(%rsi),%rbx; CWnd ctor");
static_assert(offsetof(S_OutlookOptionsDlg, m_wndList) == 0x3e8, "ctor: lea 0x3e8(%rsi),%rcx; CMFCToolBarsListCheckBox ctor");
static_assert(offsetof(S_OutlookOptionsDlg, m_pParent) == 0x500, "ctor: mov %rdi,0x500(%rsi)");
static_assert(sizeof(S_OutlookOptionsDlg) == 0x508, "scalar deleting dtor 0x9e8e0: mov $0x508,%edx");
static_assert(sizeof(CDialog) == 0x130, "OpenMFC CDialog fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8, "retail CButton is a bare CWnd (0xe8)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_wndList.m_hWnd at 0x428 == 0x3e8 + 0x40 and the parent's at +0x40");
// Deviation (2): the retail parent+0x150 read stays inside OpenMFC's object.
static_assert(sizeof(CMFCBaseTabCtrl) >= 0x154, "parent+0x150 (m_iTabsNum) lies inside OpenMFC's CMFCBaseTabCtrl");

// Resource IDs (afxribbonres.h, 14.51 SDK).
constexpr int kIdcMoveUp   = 17025;   // IDC_AFXBARRES_MOVEUP   (0x4281)
constexpr int kIdcMoveDown = 17026;   // IDC_AFXBARRES_MOVEDOWN (0x4282)
constexpr int kIdcList     = 16923;   // IDC_AFXBARRES_LIST     (0x421b)
constexpr int kIdcReset    = 16613;   // IDC_AFXBARRES_RESET    (0x40e5)
constexpr std::size_t kTabsNumOffset = 0x150;   // CMFCBaseTabCtrl::m_iTabsNum (retail)

inline S_OutlookOptionsDlg* D(void* p) { return static_cast<S_OutlookOptionsDlg*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline CDialog* Dlg(void* p) { return static_cast<CDialog*>(p); }
inline CWnd* SubWnd(unsigned char* p) { return reinterpret_cast<CWnd*>(p); }
inline CCheckListBox* List(S_OutlookOptionsDlg* d) { return reinterpret_cast<CCheckListBox*>(d->m_wndList); }
inline HWND ListHwnd(S_OutlookOptionsDlg* d) { return reinterpret_cast<CWnd*>(d->m_wndList)->m_hWnd; }
inline LRESULT ListSend(S_OutlookOptionsDlg* d, UINT msg, WPARAM wParam = 0, LPARAM lParam = 0) {
    return ::SendMessage(ListHwnd(d), msg, wParam, lParam);
}
// Retail reads m_parent.m_iTabsNum afresh on every loop test; deviation (2).
inline int ParentTabsNum(const S_OutlookOptionsDlg* d) {
    return *reinterpret_cast<const int*>(reinterpret_cast<const unsigned char*>(d->m_pParent) + kTabsNumOffset);
}
inline HWND ParentHwnd(const S_OutlookOptionsDlg* d) { return reinterpret_cast<const CWnd*>(d->m_pParent)->m_hWnd; }

// CArray<int,int> in the retail x64 layout; deviation (3).  Offsets read from
// OnOK's frame: vfptr at arr+0, m_pData +8, m_nSize +0x10, m_nMaxSize +0x18,
// m_nGrowBy +0x20 (all zero-initialised).
struct S_IntArray {
    void*          vfptr;      // +0x00 retail CArray<int,int> vftable; NULL here
    int*           m_pData;    // +0x08
    std::ptrdiff_t m_nSize;    // +0x10
    std::ptrdiff_t m_nMaxSize; // +0x18
    std::ptrdiff_t m_nGrowBy;  // +0x20
};
static_assert(sizeof(S_IntArray) == 0x28, "retail CArray<int,int> is 0x28 bytes");

// CArray<int,int>::Add(v) as OnOK inlines it: SetSize(m_nSize + 1) through the
// non-exported SetSize instantiation (0x15034, mfc140u) -- whose growth rule,
// with m_nGrowBy 0, is clamp(m_nSize / 8, 4, 1024), the transcription in
// featurepack/taskspane/CMFCTasksPane.cpp -- then m_pData[old size] = v.
// Retail throws on a negative size; the array here only ever grows from 0.
void IntArrayAdd(S_IntArray& a, int v) {
    const std::ptrdiff_t n = a.m_nSize;
    if (n + 1 > a.m_nMaxSize) {
        std::ptrdiff_t newMax;
        if (a.m_pData == nullptr) {
            newMax = (a.m_nGrowBy > n + 1) ? a.m_nGrowBy : n + 1;
        } else {
            std::ptrdiff_t grow = a.m_nGrowBy;
            if (grow == 0) {
                grow = n / 8;
                if (grow > 1024) grow = 1024;
                if (grow < 4) grow = 4;
            }
            newMax = a.m_nMaxSize + grow;
            if (n + 1 >= newMax) newMax = n + 1;
        }
        int* p = static_cast<int*>(std::malloc(static_cast<std::size_t>(newMax) * sizeof(int)));
        if (p == nullptr) return;
        std::memset(p, 0, static_cast<std::size_t>(newMax) * sizeof(int));
        if (a.m_pData != nullptr) {
            std::memcpy(p, a.m_pData, static_cast<std::size_t>(n) * sizeof(int));
            std::free(a.m_pData);
        }
        a.m_pData = p;
        a.m_nMaxSize = newMax;
    }
    a.m_nSize = n + 1;
    a.m_pData[n] = v;
}

// AfxGetMainWnd() as retail inlines it: AfxGetModuleThreadState()->
// m_pCurrentWinThread, then its GetMainWnd (vslot 31, +0xf8); deviation (1).
CWnd* MainWnd() {
    CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
    return pThread != nullptr ? impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread) : nullptr;
}

} // namespace

// Not on this change's assignment and unchanged: still a stub that leaves the
// object uninitialised (retail 0x9e850, mfc140u, is summarised in the file
// header).
// Symbol: ??0COutlookOptionsDlg@@QEAA@AEAVCMFCOutlookBarTabCtrl@@@Z
extern "C" void* MS_ABI impl___0COutlookOptionsDlg__QEAA_AEAVCMFCOutlookBarTabCtrl___Z(void* pThis, void* pTabCtrl) {
    (void)pTabCtrl;
    return pThis;
}

// Retail (RVA 0x9e9a0, mfc140u -- vslot 66), transcribed:
//     DDX_Control(pDX, IDC_AFXBARRES_MOVEUP,   m_btnMoveUp);     // +0x130
//     DDX_Control(pDX, IDC_AFXBARRES_MOVEDOWN, m_btnMoveDown);   // +0x218
//     DDX_Control(pDX, IDC_AFXBARRES_LIST,     m_wndList);       // +0x3e8
//     DDX_Control(pDX, IDC_AFXBARRES_RESET,    m_btnReset);      // +0x300 (tail call)
// No base-class DoDataExchange is called.  In OpenMFC this is never reached
// (UpdateData does not call DoDataExchange) and DDX_Control does not attach
// the control, so the members' m_hWnd stay NULL -- see the file header.
// Symbol: ?DoDataExchange@COutlookOptionsDlg@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_COutlookOptionsDlg__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_OutlookOptionsDlg* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMoveUp, d->m_btnMoveUp);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMoveDown, d->m_btnMoveDown);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcList, d->m_wndList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcReset, d->m_btnReset);
}

// Retail (RVA 0x9f0c0, mfc140u), transcribed:
//     int nSel = m_wndList.GetCurSel();                           // LB_GETCURSEL
//     CString str;  m_wndList.GetText(nSel, str);
//     DWORD_PTR dwData = m_wndList.GetItemData(nSel);             // LB_GETITEMDATA
//     int nCheck = m_wndList.GetCheck(nSel);
//     m_wndList.DeleteString(nSel);                               // LB_DELETESTRING
//     int nNew = m_wndList.InsertString(nSel + (bMoveUp ? -1 : 1), str);   // LB_INSERTSTRING
//     m_wndList.SetItemData(nNew, dwData);                        // LB_SETITEMDATA
//     m_wndList.SetCheck(nNew, nCheck);
//     m_wndList.SetCurSel(nNew);                                  // LB_SETCURSEL
//     OnSelchange();
// The list messages are raw SendMessageW calls on m_wndList.m_hWnd (+0x428);
// nSel is not range-checked (the buttons are disabled at the ends by
// OnSelchange).  The step is computed as `neg; sbb; and $-2; inc`.
// Symbol: ?MoveItem@COutlookOptionsDlg@@IEAAXH@Z
extern "C" void MS_ABI impl__MoveItem_COutlookOptionsDlg__IEAAXH_Z(void* pThis, int bMoveUp) {
    if (pThis == nullptr) return;
    S_OutlookOptionsDlg* d = D(pThis);
    const int nSel = static_cast<int>(ListSend(d, LB_GETCURSEL));
    CString str;
    impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        reinterpret_cast<const CListBox*>(d->m_wndList), nSel, &str);
    const LRESULT dwData = ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(nSel)));
    const int nCheck = impl__GetCheck_CCheckListBox__QEAAHH_Z(List(d), nSel);
    ListSend(d, LB_DELETESTRING, static_cast<WPARAM>(static_cast<unsigned int>(nSel)));   // retail: mov %ebx,%r8d (zero-extended)
    const int nInsertAt = nSel + (bMoveUp ? -1 : 1);
    const int nNew = static_cast<int>(ListSend(d, LB_INSERTSTRING, static_cast<WPARAM>(static_cast<INT_PTR>(nInsertAt)),
                                               reinterpret_cast<LPARAM>(static_cast<LPCTSTR>(str))));
    ListSend(d, LB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(nNew)), dwData);
    impl__SetCheck_CCheckListBox__QEAAXHH_Z(List(d), nNew, nCheck);
    ListSend(d, LB_SETCURSEL, static_cast<WPARAM>(static_cast<INT_PTR>(nNew)));
    impl__OnSelchange_COutlookOptionsDlg__IEAAXXZ(pThis);
}

// Retail (RVA 0x9eab0, mfc140u), transcribed:
//     int nSel = m_wndList.GetCurSel();
//     if (nSel >= 0) m_wndList.SetCheck(nSel, !m_wndList.GetCheck(nSel));
// Symbol: ?OnDblclkList@COutlookOptionsDlg@@IEAAXXZ
extern "C" void MS_ABI impl__OnDblclkList_COutlookOptionsDlg__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_OutlookOptionsDlg* d = D(pThis);
    const int nSel = static_cast<int>(ListSend(d, LB_GETCURSEL));
    if (nSel < 0) return;
    const int nCheck = impl__GetCheck_CCheckListBox__QEAAHH_Z(List(d), nSel);
    impl__SetCheck_CCheckListBox__QEAAXHH_Z(List(d), nSel, nCheck == 0 ? 1 : 0);
}

// Retail (RVA 0x9eb30, mfc140u -- vslot 96), transcribed:
//     CDialog::OnInitDialog();                                    // called directly, result dropped
//     if (AfxGetMainWnd() != NULL && (AfxGetMainWnd()->GetExStyle() & WS_EX_LAYOUTRTL))
//         ModifyStyleEx(0, WS_EX_LAYOUTRTL);                      // nFlags 0
//     for (int i = 0; i < m_parent.m_iTabsNum /*+0x150*/; i++) {
//         CString strLabel;  m_parent.GetTabLabel(i, strLabel);   // vslot 113
//         int nIndex = m_wndList.AddString(strLabel);             // LB_ADDSTRING
//         m_wndList.SetItemData(nIndex, i);                       // LB_SETITEMDATA
//         m_wndList.SetCheck(nIndex, m_parent.IsTabVisible(i));   // vslot 161
//     }
//     m_wndList.SetCurSel(0);
//     OnSelchange();
//     CWnd* pParentBar = CWnd::FromHandle(::GetParent(m_parent.m_hWnd));
//     if (pParentBar == NULL || !pParentBar->IsKindOf(RUNTIME_CLASS(CMFCOutlookBar))) {
//         m_btnReset.EnableWindow(FALSE);
//         m_btnReset.ShowWindow(SW_HIDE);
//     }
//     return TRUE;
// Retail fetches the main window twice (once for the NULL test, once for
// GetExStyle); it is fetched once here.  Deviations (1) and (2) apply.
// Symbol: ?OnInitDialog@COutlookOptionsDlg@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_COutlookOptionsDlg__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_OutlookOptionsDlg* d = D(pThis);
    impl__OnInitDialog_CDialog__UEAAHXZ(Dlg(pThis));
    if (CWnd* pMainWnd = MainWnd()) {
        if ((impl__GetExStyle_CWnd__QEBAKXZ(pMainWnd) & WS_EX_LAYOUTRTL) != 0) {
            impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(Wnd(pThis), 0, WS_EX_LAYOUTRTL, 0);
        }
    }
    if (d->m_pParent != nullptr) {
        for (int i = 0; i < ParentTabsNum(d); i++) {
            CString strLabel;
            impl__GetTabLabel_CMFCBaseTabCtrl__UEBAHHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
                d->m_pParent, i, strLabel);
            const int nIndex = static_cast<int>(ListSend(d, LB_ADDSTRING, 0,
                                                         reinterpret_cast<LPARAM>(static_cast<LPCTSTR>(strLabel))));
            ListSend(d, LB_SETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(nIndex)),
                     static_cast<LPARAM>(static_cast<INT_PTR>(i)));
            impl__SetCheck_CCheckListBox__QEAAXHH_Z(List(d), nIndex,
                                                    impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(d->m_pParent, i));
        }
    }
    ListSend(d, LB_SETCURSEL, 0);
    impl__OnSelchange_COutlookOptionsDlg__IEAAXXZ(pThis);
    CWnd* pParentBar = (d->m_pParent != nullptr)
        ? impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(ParentHwnd(d)))
        : nullptr;
    if (pParentBar == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParentBar, impl__GetThisClass_CMFCOutlookBar__SAPEAUCRuntimeClass__XZ())) {
        impl__EnableWindow_CWnd__QEAAHH_Z(SubWnd(d->m_btnReset), FALSE);
        impl__ShowWindow_CWnd__QEAAHH_Z(SubWnd(d->m_btnReset), SW_HIDE);
    }
    return TRUE;
}
static_assert(WS_EX_LAYOUTRTL == 0x400000, "OnInitDialog: mov $0x400000,%r8d");

// Retail (RVA 0x9eb10, mfc140u): `xor %edx,%edx; jmp MoveItem` -- MoveItem(FALSE).
// Symbol: ?OnMoveDown@COutlookOptionsDlg@@IEAAXXZ
extern "C" void MS_ABI impl__OnMoveDown_COutlookOptionsDlg__IEAAXXZ(void* pThis) {
    impl__MoveItem_COutlookOptionsDlg__IEAAXH_Z(pThis, FALSE);
}

// Retail (RVA 0x9eb20, mfc140u): `mov $1,%edx; jmp MoveItem` -- MoveItem(TRUE).
// Symbol: ?OnMoveUp@COutlookOptionsDlg@@IEAAXXZ
extern "C" void MS_ABI impl__OnMoveUp_COutlookOptionsDlg__IEAAXXZ(void* pThis) {
    impl__MoveItem_COutlookOptionsDlg__IEAAXH_Z(pThis, TRUE);
}

// Retail (RVA 0x9ed10, mfc140u -- vslot 98), transcribed:
//     CArray<int,int> arTabsOrder;
//     for (int i = 0; i < m_wndList.GetCount(); i++) {            // LB_GETCOUNT, re-sent each test
//         int iTab = (int)m_wndList.GetItemData(i);               // LB_GETITEMDATA
//         BOOL bCheck = m_wndList.GetCheck(i);
//         if (bCheck != m_parent.IsTabVisible(iTab))              // vslot 161
//             m_parent.ShowTab(iTab, bCheck, FALSE, FALSE);       // vslot 108
//         arTabsOrder.Add(iTab);
//     }
//     m_parent.SetTabsOrder(arTabsOrder);                         // result dropped
//     CDialog::OnOK();                                            // called directly
//     // ~CArray: free(m_pData)
// Deviations (1) and (3) apply.  OpenMFC's SetTabsOrder currently reads its
// argument through OpenMFC's own CArray declaration (afx.h:579, int-sized
// members without a vfptr), not the retail layout it is handed here; see the
// headerRequests of this change.
// Symbol: ?OnOK@COutlookOptionsDlg@@MEAAXXZ
extern "C" void MS_ABI impl__OnOK_COutlookOptionsDlg__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_OutlookOptionsDlg* d = D(pThis);
    S_IntArray arTabsOrder = {};
    for (int i = 0; i < static_cast<int>(ListSend(d, LB_GETCOUNT)); i++) {
        const int iTab = static_cast<int>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(i))));
        const int bCheck = impl__GetCheck_CCheckListBox__QEAAHH_Z(List(d), i);
        if (d->m_pParent != nullptr &&
            bCheck != impl__IsTabVisible_CMFCBaseTabCtrl__UEBAHH_Z(d->m_pParent, iTab)) {
            impl__ShowTab_CMFCBaseTabCtrl__UEAAHHHHH_Z(d->m_pParent, iTab, bCheck, FALSE, FALSE);
        }
        IntArrayAdd(arTabsOrder, iTab);
    }
    if (d->m_pParent != nullptr) {
        impl__SetTabsOrder_CMFCBaseTabCtrl__QEAAHAEBV__CArray_HH___Z(
            d->m_pParent, *reinterpret_cast<const CArray<int, int>*>(&arTabsOrder));
    }
    impl__OnOK_CDialog__MEAAXXZ(Dlg(pThis));
    if (arTabsOrder.m_pData != nullptr) std::free(arTabsOrder.m_pData);
}

// Retail (RVA 0x9ee60, mfc140u), read but NOT transcribed:
//     CWnd* pWnd = CWnd::FromHandle(::GetParent(m_parent.m_hWnd));
//     if (pWnd == NULL || !pWnd->IsKindOf(RUNTIME_CLASS(CMFCOutlookBar))) return;
//     CMFCOutlookBar* pBar = (CMFCOutlookBar*)pWnd;
//     CArray<int,int> arTabsOrder;
//     for (int i = 0; i < pBar->GetDefaultTabsOrder().GetSize(); i++) {
//         // GetDefaultTabsOrder() is inline: FillDefaultTabsOrderArray() when
//         // m_arDefaultTabsOrder (+0x4f0: m_pData +0x4f8, m_nSize +0x500) is empty
//         int iTab = m_parent.GetTabByID(pBar->GetDefaultTabsOrder()[i]);   // vslot 138
//         if (iTab < 0) return;                                    // array freed, list untouched
//         arTabsOrder.Add(iTab);
//     }
//     m_wndList.ResetContent();                                    // LB_RESETCONTENT
//     for (int i = 0; i < arTabsOrder.GetSize(); i++) {
//         int iTab = arTabsOrder[i];
//         CString strLabel;  m_parent.GetTabLabel(iTab, strLabel); // vslot 113
//         int nIndex = m_wndList.AddString(strLabel);
//         m_wndList.SetItemData(nIndex, iTab);
//         m_wndList.SetCheck(nIndex, TRUE);
//     }
//     m_wndList.SetCurSel(0);
//     OnSelchange();
// Left a stub: CBaseTabbedPane::m_arDefaultTabsOrder (+0x4f0) is not modeled
// in OpenMFC -- its CBaseTabbedPane (afxmfc.h:1108) adds only 96 bytes of
// opaque padding to CDockablePane and measures 0x4d8 bytes in total, so
// +0x4f0..+0x508 lies past its end; no CMFCOutlookBar class is declared and
// its exported constructor (CMFCOutlookBar.cpp) is a stub -- and the
// FillDefaultTabsOrderArray thunk (featurepack/docking/CBaseTabbedPane.cpp) has
// a placeholder signature with no `this` and an empty body.
// Symbol: ?OnReset@COutlookOptionsDlg@@IEAAXXZ
extern "C" void MS_ABI impl__OnReset_COutlookOptionsDlg__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x9ea20, mfc140u), transcribed:
//     m_btnMoveUp.EnableWindow(m_wndList.GetCurSel() > 0);
//     int nSel = m_wndList.GetCurSel();                           // sent a second time
//     m_btnMoveDown.EnableWindow(nSel < m_wndList.GetCount() - 1);
// Symbol: ?OnSelchange@COutlookOptionsDlg@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchange_COutlookOptionsDlg__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_OutlookOptionsDlg* d = D(pThis);
    impl__EnableWindow_CWnd__QEAAHH_Z(SubWnd(d->m_btnMoveUp), static_cast<int>(ListSend(d, LB_GETCURSEL)) > 0);
    const int nSel = static_cast<int>(ListSend(d, LB_GETCURSEL));
    const int nCount = static_cast<int>(ListSend(d, LB_GETCOUNT));
    impl__EnableWindow_CWnd__QEAAHH_Z(SubWnd(d->m_btnMoveDown), nSel < nCount - 1);
}
