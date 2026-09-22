// CMFCWindowsManagerDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <cwchar>

// ===========================================================================
// CMFCWindowsManagerDialog -- the feature-pack "Window > Windows..." dialog
// (afxwindowsmanagerdialog.h:35 in the 14.51 SDK on this host; derives CDialog).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  All RVAs
// and absolute addresses in this file are mfc140u.  Ten of these eighteen
// exports (OnHelpInfo, OnInitDialog, OnMinimize, OnSave, OnSelchangeList,
// OnTilehorz, OnTilevert, OnWindowHelp, SelActive, UpdateButtons) have no
// entry in mfc140u_rva_symbols.json; every RVA below was obtained by
// resolving the export's ordinal (mfc_complete_ordinal_mapping.json) through
// mfc140u.dll's export address table, and the handler RVAs were confirmed a
// second time against the class message map at 0x1803207f8 (returned by
// GetMessageMap, 0x1c9a40), whose base map is CDialog's:
//
//   WM_DRAWITEM                           OnDrawItem       0x1ca740
//   WM_HELPINFO                           OnHelpInfo       0x1caa40
//   BN_CLICKED   16920 (ACTIVATE)         OnActivate       0x1c9b50
//   BN_CLICKED   16921 (SAVE)             OnSave           0x1c9cd0
//   BN_CLICKED   16922 (CLOSE)            OnClose          0x1c9df0
//   BN_CLICKED   16926 (CASCADE)          OnCascade        0x1c9ee0
//   BN_CLICKED   16924 (TILEHORZ)         OnTilehorz       0x1c9ef0
//   BN_CLICKED   16928 (TILEVERT)         OnTilevert       0x1c9f00
//   BN_CLICKED   16925 (MINIMIZE)         OnMinimize       0x1c9f10
//   BN_CLICKED   0xe146 (ID_HELP)         OnWindowHelp     0x1ca9f0
//   LBN_SELCHANGE 16923 (LIST)            OnSelchangeList  0x1ca110
//   LBN_DBLCLK    16923 (LIST)            OnActivate       0x1c9b50
// The class vftable is 0x1803209b8 (stored by the constructor, 0x1c98a0):
// slot 66 (+0x210) is DoDataExchange 0x1c9a20 and slot 96 (+0x300) is
// OnInitDialog 0x1c9fe0.  The non-virtual helpers are MDIMessage 0x1c9a50,
// UpdateButtons 0x1ca120, SelActive 0x1ca330 and FillWindowList 0x1ca420.
//
// NOT WIRED IN THIS TREE (outside this file, reported as headerRequests):
// OpenMFC's message map for this class (classCMFCWindowsManagerDialog_msgmap
// in detail/Mfc08MsgmapSupport.cpp) has no entries, so none of the handlers
// above is reached by message dispatch today; and OpenMFC's DDX_Control
// (core/runtime/DdxExchange.cpp) looks the control up but does not attach it
// to the CWnd it is handed, so m_wndList.m_hWnd is only non-NULL when
// something else attaches it.
//
// Layout.  The class is not declared in OpenMFC's public headers, so this file
// is the only place its layout lives.  S_WndMgrDlg pins it: the constructor
// builds CDialog at +0 and then the members at the offsets below, and the
// scalar deleting destructor (vftable slot 1, 0x1c9990) loads 0x298 as the
// size argument of its sized-delete path (`mov $0x298,%edx` at 0x1c99f4,
// before the call at 0x1c99f9 inside that destructor).  The
// member names are those of afxwindowsmanagerdialog.h, whose declaration order
// matches the offsets.  No CRuntimeClass exists for this class (the header has
// no DECLARE_DYNAMIC; its slot 0 is CDialog::GetRuntimeClass).
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftable 0x1803209b8 at +0x00, the
//      CListBox vftable 0x1802effe8 into m_wndList after ??0CWnd@@QEAA@XZ, and
//      the CList<HWND,HWND> vftable 0x1802e9248 at +0x00 of both lists.
//      OpenMFC has no MSVC-layout vtable for any of them, so +0x00 keeps
//      whatever the CDialog constructor thunk leaves there (it installs none),
//      m_wndList is built with the exported CWnd constructor alone (as
//      CMFCKeyMapDialog.cpp does for its plain controls; that thunk placement-
//      constructs OpenMFC's C++ CWnd, so m_wndList's +0x00 holds OpenMFC's own
//      CWnd vptr, not an MSVC CListBox vftable), and the lists keep a
//      NULL vfptr (as CMFCToolBarsCustomizeDialog.cpp does for its CMap).
//      Consequently the retail bodies' virtual calls are devirtualised.  (None
//      of them is on `this`: retail calls FillWindowList / SelActive /
//      UpdateButtons / MDIMessage, CDialog::OnInitDialog, CDialog::EndDialog
//      and CWnd::OnDrawItem directly, and so does this file.)  The calls on
//      other objects go to the exported thunks they resolve to for the base
//      class (CWinThread::GetMainWnd, CFrameWnd::GetActiveDocument,
//      CDocument::DoFileSave, CDC::SetTextColor) -- or, for CDC::DrawText,
//      whose base body 0x1e6ce0 is not exported, straight to the ::DrawTextW
//      it wraps -- so an override in a client-derived class is bypassed.
//      Two of those virtuals, CMDIChildWndEx::CanShowOnWindowsList (slot 124,
//      +0x3e0) and IsReadOnly (slot 125, +0x3e8), are inline in
//      afxmdichildwndex.h and not exported at all; their retail default bodies
//      are the shared `return 1` (0x3a60) and `return 0` (0x71e0) that fill
//      those two slots of the CMDIChildWndEx vftable 0x2ee018, and those
//      constants are used here.  A client MDI child that overrides either one
//      is therefore always listed / never read-only here.
//
//  (2) The two CList<HWND,HWND> members are file-local replicas of the
//      afxtempl.h layout (below), with the node operations retail instantiates
//      transcribed: AddTail (0x7908) and RemoveAll (0x8350); Find is inlined
//      into its callers.  Blocks come from the exported CPlex::Create and are
//      released through the exported CPlex::FreeDataChain, so an MSVC client
//      whose inlined ~CMFCWindowsManagerDialog runs RemoveAll through the
//      FreeDataChain export frees them with the matching deallocator.
//
//  (3) NULL guards.  Retail dereferences m_pMDIFrame, the GetDlgItem results
//      it passes to ShowWindow / EnableWindow, the CWnd::FromHandle result of
//      GetParent() in OnActivate and the thread's main window in OnHelpInfo /
//      OnWindowHelp without testing them.  Each of those is tested here and
//      the dependent call skipped when it is NULL, where retail would fault.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each signature was checked against the definition
// named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd);     // detail/DlgcoreSupport.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                          // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult);                               // detail/DlgcoreSupport.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                                          // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);      // core/runtime/DdxExchange.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                    // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);                            // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);                                  // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                                 // core/window/CWnd.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);                                   // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__OnDrawItem_CWnd__IEAAXHPEAUtagDRAWITEMSTRUCT___Z(CWnd* pThis, int nIDCtl, DRAWITEMSTRUCT* lpDrawItemStruct);   // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ();                            // core/frame/CFrameWnd.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ();                       // core/frame/CMDIChildWndEx.cpp
extern "C" CDocument* MS_ABI impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(CFrameWnd* pThis);            // core/frame/Thunks.cpp
extern "C" int   MS_ABI impl__DoFileSave_CDocument__UEAAHXZ(CDocument* pThis);                                       // core/doc/CDocument.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();                                                     // detail/RegcoreSupport.cpp (retail: AfxGetModuleState()->m_pCurrentWinApp)
extern "C" void  MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);                                // core/runtime/CCmdTarget.cpp
extern "C" void  MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);                                  // core/runtime/CCmdTarget.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                                            // core/app/Globals.cpp
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);                            // core/app/CWinThread.cpp
extern "C" CDC*  MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);                                        // core/gdi/CDC.cpp
extern "C" int   MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);                                      // core/gdi/CDC.cpp
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);                 // core/gdi/CDC.cpp
extern "C" void  MS_ABI impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CListBox* pThis, int nIndex, CString* rString);   // core/controls/CListBox.cpp
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(CPlex** ppHead, unsigned __int64 nMax, unsigned __int64 cbElement);   // core/collections/CPlex.cpp
extern "C" void  MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* self);                                             // core/collections/CPlex.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                                  // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowResourceException__YAXXZ();                                                   // detail/MfcExceptionsSupport.cpp

// Exported data objects, read the way retail reads them.
extern "C" int impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA;           // core/frame/StaticData.cpp (data 0x3b1afc)
extern "C" unsigned int impl__AFX_WM_WINDOW_HELP__3IA;                  // core/runtime/Globals.cpp  (data 0x3c2738)
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720]; // featurepack/CMFC_misc_stubs.cpp (data 0x3c1620)
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);   // core/runtime/AFX_GLOBAL_DATA.cpp (0x6a790)

// This file's own thunks that earlier bodies call (retail calls them directly;
// OnInitDialog / DoDataExchange are virtual but are never called from here).
extern "C" void MS_ABI impl__FillWindowList_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__MDIMessage_CMFCWindowsManagerDialog__IEAAXI_K_Z(void* pThis, unsigned int uMsg, unsigned __int64 flag);
extern "C" void MS_ABI impl__SelActive_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// CList<HWND,HWND> (afxtempl.h), deviation (2).  Node layout from AddTail
// (0x7908): pNext at +0 (also the free-list link), pPrev at +8, data at +0x10,
// and CPlex::Create is handed 0x18 as the element size.  The list object from
// the constructor stores (0x1c9901..0x1c9961): vfptr, then head +8, tail +0x10,
// count +0x18, free +0x20, blocks +0x28 zeroed and block size +0x30 = 10.
// ---------------------------------------------------------------------------
struct HwndNode {
    HwndNode* pNext;   // +0x00
    HwndNode* pPrev;   // +0x08
    HWND      data;    // +0x10
};
struct HwndList {
    void*     vfptr;         // +0x00  retail 0x1802e9248; NULL here (deviation 1)
    HwndNode* m_pNodeHead;   // +0x08
    HwndNode* m_pNodeTail;   // +0x10
    INT_PTR   m_nCount;      // +0x18
    HwndNode* m_pNodeFree;   // +0x20
    CPlex*    m_pBlocks;     // +0x28
    INT_PTR   m_nBlockSize;  // +0x30
};
static_assert(sizeof(HwndNode) == 0x18 && sizeof(HwndList) == 0x38, "retail CList<HWND,HWND> shape");

// ---------------------------------------------------------------------------
// The retail object (0x298 bytes); offsets from the constructor (0x1c98a0).
// ---------------------------------------------------------------------------
struct S_WndMgrDlg {
    alignas(8) unsigned char m_base[0x130];     // +0x000 CDialog (OpenMFC sizeof(CDialog) == 0x130, asserted below)
    alignas(8) unsigned char m_wndList[0xe8];   // +0x130 CListBox (DDX 16923 IDC_AFXBARRES_LIST)
    CMDIFrameWndEx* m_pMDIFrame;                // +0x218
    int             m_bHelpButton;              // +0x220 (const BOOL)
    int             m_bMDIActions;              // +0x224
    HwndList        m_lstCloseDisabled;         // +0x228
    HwndList        m_lstSaveDisabled;          // +0x260
};
static_assert(offsetof(S_WndMgrDlg, m_wndList) == 0x130, "ctor: lea 0x130(%r14),%rbx; CWnd ctor");
static_assert(offsetof(S_WndMgrDlg, m_pMDIFrame) == 0x218, "ctor: mov %rdi,0x218(%r14)");
static_assert(offsetof(S_WndMgrDlg, m_bHelpButton) == 0x220, "ctor: mov %esi,0x220(%r14)");
static_assert(offsetof(S_WndMgrDlg, m_bMDIActions) == 0x224, "ctor: movl $0x1,0x224(%r14)");
static_assert(offsetof(S_WndMgrDlg, m_lstCloseDisabled) == 0x228, "ctor: vfptr store at 0x228(%r14)");
static_assert(offsetof(S_WndMgrDlg, m_lstSaveDisabled) == 0x260, "ctor: vfptr store at 0x260(%r14)");
static_assert(sizeof(S_WndMgrDlg) == 0x298, "scalar deleting dtor 0x1c9990: mov $0x298,%edx");
static_assert(sizeof(CDialog) == 0x130, "OpenMFC CDialog fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CListBox) == 0xe8, "CListBox is a bare CWnd");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads the dialog / list HWNDs at +0x40");
static_assert(offsetof(CMDIFrameWnd, m_hWndMDIClient) == 0x1d8, "retail reads m_pMDIFrame->m_hWndMDIClient at +0x1d8");
static_assert(offsetof(CDC, m_hDC) == 0x8, "OnDrawItem: lea 0x8(%rax),%rsi (m_hDC)");
static_assert(offsetof(DRAWITEMSTRUCT, itemID) == 0x8 && offsetof(DRAWITEMSTRUCT, itemAction) == 0xc &&
              offsetof(DRAWITEMSTRUCT, itemState) == 0x10 && offsetof(DRAWITEMSTRUCT, hDC) == 0x20 &&
              offsetof(DRAWITEMSTRUCT, rcItem) == 0x28 && offsetof(DRAWITEMSTRUCT, itemData) == 0x38,
              "OnDrawItem reads DRAWITEMSTRUCT at these offsets");

// Resource IDs (afxribbonres.h, 14.51 SDK).
constexpr UINT kIdcActivate  = 16920;   // IDC_AFXBARRES_ACTIVATE  (0x4218)
constexpr UINT kIdcSave      = 16921;   // IDC_AFXBARRES_SAVE      (0x4219)
constexpr UINT kIdcClose     = 16922;   // IDC_AFXBARRES_CLOSE     (0x421a)
constexpr UINT kIdcList      = 16923;   // IDC_AFXBARRES_LIST      (0x421b)
constexpr UINT kIdcTileHorz  = 16924;   // IDC_AFXBARRES_TILEHORZ  (0x421c)
constexpr UINT kIdcMinimize  = 16925;   // IDC_AFXBARRES_MINIMIZE  (0x421d)
constexpr UINT kIdcCascade   = 16926;   // IDC_AFXBARRES_CASCADE   (0x421e)
constexpr UINT kIddWindows   = 16927;   // IDD_AFXBARRES_WINDOWS_DLG (0x421f)
constexpr UINT kIdcTileVert  = 16928;   // IDC_AFXBARRES_TILEVERT  (0x4220)
constexpr UINT kIdHelp       = 0xe146;  // ID_HELP

// afxGlobalData members OnDrawItem reads (core/runtime/AFX_GLOBAL_DATA.cpp
// pins the same offsets): brHilite / brWindow are CBrush objects at +0xb8 /
// +0x108 whose m_hObject sits 8 bytes in (retail loads 0x3c16e0 / 0x3c1730 ==
// blob + 0xc0 / + 0x110); clrTextHilite +0x4c, clrWindowText +0x7c.
constexpr std::size_t kGdBrHiliteHandle   = 0xc0;
constexpr std::size_t kGdBrWindowHandle   = 0x110;
constexpr std::size_t kGdClrTextHilite    = 0x4c;
constexpr std::size_t kGdClrWindowText    = 0x7c;

inline S_WndMgrDlg* D(void* p) { return static_cast<S_WndMgrDlg*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline CDialog* Dlg(void* p) { return static_cast<CDialog*>(p); }
inline HWND ListHwnd(const S_WndMgrDlg* d) { return reinterpret_cast<const CWnd*>(d->m_wndList)->m_hWnd; }
inline const CListBox* ListBox(const S_WndMgrDlg* d) { return reinterpret_cast<const CListBox*>(d->m_wndList); }
inline LRESULT ListSend(const S_WndMgrDlg* d, UINT msg, WPARAM wParam = 0, LPARAM lParam = 0) {
    return ::SendMessage(ListHwnd(d), msg, wParam, lParam);
}
// m_pMDIFrame->m_hWndMDIClient (+0x1d8); deviation (3) for a NULL frame.
inline HWND MdiClient(const S_WndMgrDlg* d) {
    return d->m_pMDIFrame != nullptr ? reinterpret_cast<const CMDIFrameWnd*>(d->m_pMDIFrame)->m_hWndMDIClient : nullptr;
}
inline CWnd* DlgItem(void* pThis, UINT nID) { return impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(Wnd(pThis), static_cast<int>(nID)); }

// CList::Find(value) as inlined into OnSave (0x1c9d3c) and UpdateButtons
// (0x1ca1be / 0x1ca1ee): walk from m_pNodeHead comparing node->data.  (The
// inlined ENSURE on &node->data, which cannot be NULL, is not reproduced.)
bool ListContains(const HwndList& l, HWND h) {
    for (const HwndNode* p = l.m_pNodeHead; p != nullptr; p = p->pNext) {
        if (p->data == h) return true;
    }
    return false;
}

// CList::RemoveAll (0x8350): zero count, free list, tail and head, free the
// block chain, zero m_pBlocks.
void ListRemoveAll(HwndList& l) {
    l.m_nCount = 0;
    l.m_pNodeFree = nullptr;
    l.m_pNodeTail = nullptr;
    l.m_pNodeHead = nullptr;
    if (l.m_pBlocks != nullptr) impl__FreeDataChain_CPlex__QEAAXXZ(l.m_pBlocks);
    l.m_pBlocks = nullptr;
}

// CList::AddTail (0x7908), NewNode inlined: when the free list is empty,
// CPlex::Create(m_pBlocks, m_nBlockSize, 0x18) and push the new block's
// nodes on it last-to-first; pop a node, pPrev = old tail, pNext = NULL,
// ++m_nCount, data = h; link it after the old tail (or as head); tail = node.
// A NULL free list after the refill takes retail's AfxThrowInvalidArgException
// path (0x1800079ae).
void ListAddTail(HwndList& l, HWND h) {
    HwndNode* pOldTail = l.m_pNodeTail;
    if (l.m_pNodeFree == nullptr) {
        CPlex* pBlock = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
            &l.m_pBlocks, static_cast<unsigned __int64>(l.m_nBlockSize), sizeof(HwndNode));
        if (pBlock != nullptr) {
            // CPlex::data() == this + 1: nodes start 8 bytes into the block
            // (retail addresses the last node as block + 8 + 0x18 * (n - 1)).
            HwndNode* pNode = reinterpret_cast<HwndNode*>(reinterpret_cast<unsigned char*>(pBlock) + 8) + l.m_nBlockSize - 1;
            for (INT_PTR i = l.m_nBlockSize - 1; i >= 0; i--, pNode--) {
                pNode->pNext = l.m_pNodeFree;
                l.m_pNodeFree = pNode;
            }
        }
    }
    HwndNode* pNew = l.m_pNodeFree;
    if (pNew == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    l.m_pNodeFree = pNew->pNext;
    pNew->pPrev = pOldTail;
    pNew->pNext = nullptr;
    l.m_nCount++;
    pNew->data = h;
    if (l.m_pNodeTail != nullptr) {
        l.m_pNodeTail->pNext = pNew;
    } else {
        l.m_pNodeHead = pNew;
    }
    l.m_pNodeTail = pNew;
}

// afxGlobalData accessor prologue, as inlined into OnDrawItem: if the
// m_bInitialized word (+0) is 0, Initialize() the blob and set it to 1.
inline void EnsureGlobalData() {
    int bInit;
    std::memcpy(&bInit, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof bInit);
    if (bInit == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
}
template <typename T>
inline T GlobalData(std::size_t off) {
    T v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}

// AfxGetModuleThreadState()->m_pCurrentWinThread (+0x8), then its virtual
// GetMainWnd (CWinThread vftable 0x3318f0, slot 31 +0xf8 == 0x274960),
// devirtualised (deviation 1).  NULL when there is no current thread, as in
// retail.
CWnd* MainWnd() {
    CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
    return pThread != nullptr ? impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread) : nullptr;
}

} // namespace

// Retail (RVA 0x1c98a0, mfc140u), transcribed:
//     CDialog::CDialog(IDD_AFXBARRES_WINDOWS_DLG /*0x421f*/, pMDIFrame);   // 0x208720
//     vfptr = 0x1803209b8;
//     m_wndList: CWnd::CWnd() (0x28a700) + CListBox vftable 0x1802effe8
//     m_pMDIFrame = pMDIFrame;  m_bHelpButton = bHelpButton;
//     m_lstCloseDisabled, m_lstSaveDisabled: vftable 0x1802e9248, head/tail/
//         count/free/blocks = 0, block size = 10
//     m_bMDIActions = TRUE;
// DEVIATION: the vftable stores are deviation (1).
// Symbol: ??0CMFCWindowsManagerDialog@@QEAA@PEAVCMDIFrameWndEx@@H@Z
extern "C" void* MS_ABI impl___0CMFCWindowsManagerDialog__QEAA_PEAVCMDIFrameWndEx__H_Z(
    void* pThis, CMDIFrameWndEx* pMDIFrame, int bHelpButton) {
    if (pThis == nullptr) return nullptr;
    S_WndMgrDlg* d = D(pThis);
    impl___0CDialog__QEAA_IPEAVCWnd___Z(Dlg(pThis), kIddWindows, reinterpret_cast<CWnd*>(pMDIFrame));
    impl___0CWnd__QEAA_XZ(d->m_wndList);
    d->m_pMDIFrame = pMDIFrame;
    d->m_bHelpButton = bHelpButton;
    for (HwndList* l : {&d->m_lstCloseDisabled, &d->m_lstSaveDisabled}) {
        l->vfptr = nullptr;
        l->m_pNodeHead = nullptr;
        l->m_pNodeTail = nullptr;
        l->m_nCount = 0;
        l->m_pNodeFree = nullptr;
        l->m_pBlocks = nullptr;
        l->m_nBlockSize = 10;
    }
    d->m_bMDIActions = TRUE;
    return pThis;
}

// Retail (RVA 0x1c9a20, mfc140u), fully transcribed: a single tail jump to
// DDX_Control (0x293870) binding m_wndList (+0x130) to IDC_AFXBARRES_LIST
// (16923).  No CDialog::DoDataExchange call.
// Symbol: ?DoDataExchange@CMFCWindowsManagerDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCWindowsManagerDialog__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, static_cast<int>(kIdcList), D(pThis)->m_wndList);
}

// Retail (RVA 0x1ca420, mfc140u), transcribed:
//     m_wndList.SetRedraw(FALSE);                              // WM_SETREDRAW on m_wndList.m_hWnd
//     m_wndList.ResetContent();                                // LB_RESETCONTENT
//     int cxExtent = 0;
//     CClientDC dcList(&m_wndList);                            // 0x2a3b20: ::GetDC(list)
//     CFont* pOldFont = dcList.SelectObject(                   // 0x2a2730
//         CFont::FromHandle((HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0)));   // the DIALOG's font
//     m_bMDIActions = TRUE;
//     m_lstCloseDisabled.RemoveAll();  m_lstSaveDisabled.RemoveAll();          // 0x8350
//     for (HWND hwndT = ::GetWindow(m_pMDIFrame->m_hWndMDIClient, GW_CHILD);
//          hwndT != NULL; hwndT = ::GetWindow(hwndT, GW_HWNDNEXT)) {
//         CMDIChildWndEx* pChild = DYNAMIC_DOWNCAST(CMDIChildWndEx, CWnd::FromHandle(hwndT));
//         if (pChild == NULL || !pChild->CanShowOnWindowsList()) continue;   // vslot 124
//         TCHAR szWndTitle[256];
//         ::GetWindowText(hwndT, szWndTitle, 256);
//         int index = m_wndList.AddString(szWndTitle);          // LB_ADDSTRING
//         CString str(szWndTitle);
//         cxExtent = max(cxExtent, dcList.GetTextExtent(str).cx);   // ::GetTextExtentPoint32W on m_hAttribDC
//         m_wndList.SetItemData(index, (DWORD_PTR)hwndT);       // LB_SETITEMDATA
//         if (pChild->IsReadOnly()) m_lstSaveDisabled.AddTail(hwndT);   // vslot 125
//         if (::GetWindowLong(hwndT, GWL_STYLE) & WS_SYSMENU) {  // `bt $0x13`
//             HMENU hSysMenu = ::GetSystemMenu(hwndT, FALSE);
//             if (hSysMenu == NULL) m_bMDIActions = FALSE;
//             else {
//                 MENUITEMINFO mii; memset(&mii, 0, 0x50);  mii.cbSize = 0x50;  mii.fMask = MIIM_STATE;
//                 if (!::GetMenuItemInfo(hSysMenu, SC_CLOSE, FALSE, &mii)
//                     || (mii.fState & (MFS_GRAYED | MFS_DISABLED)))     // `testb $0x3`
//                     m_lstCloseDisabled.AddTail(hwndT);
//             }
//         } else m_bMDIActions = FALSE;
//     }
//     m_wndList.SetHorizontalExtent(cxExtent + ::GetSystemMetrics(SM_CXHSCROLL) + 30);   // LB_SETHORIZONTALEXTENT
//     dcList.SelectObject(pOldFont);
//     m_wndList.SetRedraw(TRUE);
//     // ~CClientDC (0x2a3be0): ::ReleaseDC(list, m_hDC)
// DEVIATIONS: CanShowOnWindowsList / IsReadOnly are the inline defaults TRUE /
// FALSE (deviation 1).  The CClientDC / CDC::SelectObject / CFont::FromHandle
// wrappers are replaced by the GDI calls they reduce to for a client DC
// (whose m_hDC and m_hAttribDC are the same handle): ::GetDC(list),
// ::SelectObject, ::GetTextExtentPoint32W, ::ReleaseDC -- without the
// temporary handle-map entries; a NULL DC still throws, as CClientDC does.
// Retail's CClientDC destructor would also release the DC if AddTail threw
// out of the loop; this body does not.  NULL m_pMDIFrame: deviation (3), the
// child walk is skipped.
// Symbol: ?FillWindowList@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__FillWindowList_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    const HWND hList = ListHwnd(d);
    ::SendMessage(hList, WM_SETREDRAW, FALSE, 0);
    ::SendMessage(hList, LB_RESETCONTENT, 0, 0);
    int cxExtent = 0;

    // CClientDC::CClientDC (0x2a3b20): ::GetDC, then CDC::Attach; a NULL DC
    // makes Attach fail and the constructor calls AfxThrowResourceException
    // (the call at 0x2a3b7d) -- after the two list messages above were sent.
    HDC hdcList = ::GetDC(hList);
    if (hdcList == nullptr) {
        impl__AfxThrowResourceException__YAXXZ();
        return;
    }
    HFONT hFont = reinterpret_cast<HFONT>(::SendMessage(Wnd(pThis)->m_hWnd, WM_GETFONT, 0, 0));
    HGDIOBJ hOldFont = ::SelectObject(hdcList, hFont);

    d->m_bMDIActions = TRUE;
    ListRemoveAll(d->m_lstCloseDisabled);
    ListRemoveAll(d->m_lstSaveDisabled);

    const HWND hMDIClient = MdiClient(d);
    for (HWND hwndT = hMDIClient != nullptr ? ::GetWindow(hMDIClient, GW_CHILD) : nullptr;
         hwndT != nullptr; hwndT = ::GetWindow(hwndT, GW_HWNDNEXT)) {
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwndT);
        if (pWnd == nullptr ||
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, impl__GetThisClass_CMDIChildWndEx__SAPEAUCRuntimeClass__XZ())) {
            continue;
        }
        const BOOL bCanShowOnWindowsList = TRUE;   // CMDIChildWndEx vslot 124 default (0x3a60), deviation (1)
        if (!bCanShowOnWindowsList) continue;

        wchar_t szWndTitle[256];
        ::GetWindowTextW(hwndT, szWndTitle, 256);
        const int index = static_cast<int>(::SendMessage(hList, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(szWndTitle)));
        SIZE sz = {0, 0};
        ::GetTextExtentPoint32W(hdcList, szWndTitle, static_cast<int>(std::wcslen(szWndTitle)), &sz);
        if (sz.cx > cxExtent) cxExtent = sz.cx;
        ::SendMessage(hList, LB_SETITEMDATA, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(hwndT));

        const BOOL bIsReadOnly = FALSE;            // CMDIChildWndEx vslot 125 default (0x71e0), deviation (1)
        if (bIsReadOnly) ListAddTail(d->m_lstSaveDisabled, hwndT);

        if ((static_cast<DWORD>(::GetWindowLongW(hwndT, GWL_STYLE)) & WS_SYSMENU) == 0) {
            d->m_bMDIActions = FALSE;
            continue;
        }
        HMENU hSysMenu = ::GetSystemMenu(hwndT, FALSE);
        if (hSysMenu == nullptr) {
            d->m_bMDIActions = FALSE;
            continue;
        }
        MENUITEMINFOW mii;
        std::memset(&mii, 0, sizeof mii);
        mii.cbSize = sizeof mii;
        mii.fMask = MIIM_STATE;
        if (!::GetMenuItemInfoW(hSysMenu, SC_CLOSE, FALSE, &mii) || (mii.fState & (MFS_GRAYED | MFS_DISABLED)) != 0) {
            ListAddTail(d->m_lstCloseDisabled, hwndT);
        }
    }

    ::SendMessage(hList, LB_SETHORIZONTALEXTENT,
                  static_cast<WPARAM>(cxExtent + ::GetSystemMetrics(SM_CXHSCROLL) + 30), 0);
    ::SelectObject(hdcList, hOldFont);
    ::SendMessage(hList, WM_SETREDRAW, TRUE, 0);
    ::ReleaseDC(hList, hdcList);
}
static_assert(sizeof(MENUITEMINFOW) == 0x50, "FillWindowList: memset(&mii, 0, 0x50); mii.cbSize = 0x50");
static_assert(SM_CXHSCROLL == 0x15 && WM_GETFONT == 0x31, "FillWindowList: GetSystemMetrics(0x15); SendMessage(m_hWnd, 0x31)");

// Retail (RVA 0x1c9a50, mfc140u), transcribed:
//     CWaitCursor wait;      // AfxGetModuleState()->m_pCurrentWinApp->BeginWaitCursor() (0x1de7b0);
//                            // the epilogue tail-jumps to 0x7687c, which re-reads
//                            // AfxGetModuleState()->m_pCurrentWinApp and calls
//                            // EndWaitCursor (0x1de7e0) on it
//     int nItems = m_wndList.GetCount();                        // LB_GETCOUNT
//     if (nItems > 0) {
//         HWND hMDIClient = m_pMDIFrame->m_hWndMDIClient;
//         ::LockWindowUpdate(hMDIClient);
//         for (int i = nItems - 1; i >= 0; i--) {
//             HWND hWnd = (HWND)m_wndList.GetItemData(i);        // LB_GETITEMDATA
//             ::ShowWindow(hWnd, m_wndList.GetSel(i) > 0 ? SW_RESTORE : SW_MINIMIZE);   // LB_GETSEL; 9 : 6
//         }
//         ::SendMessage(hMDIClient, uMsg, flag, 0);
//         ::LockWindowUpdate(NULL);
//     }
// NULL m_pMDIFrame: deviation (3), nothing is done after the count.
// Symbol: ?MDIMessage@CMFCWindowsManagerDialog@@IEAAXI_K@Z
extern "C" void MS_ABI impl__MDIMessage_CMFCWindowsManagerDialog__IEAAXI_K_Z(void* pThis, unsigned int uMsg, unsigned __int64 flag) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
    const int nItems = static_cast<int>(ListSend(d, LB_GETCOUNT));
    if (nItems > 0 && d->m_pMDIFrame != nullptr) {
        // Keyed on the frame pointer only: a frame whose m_hWndMDIClient is
        // NULL still gets the ShowWindow loop, as in retail.
        const HWND hMDIClient = MdiClient(d);
        ::LockWindowUpdate(hMDIClient);
        for (int i = nItems - 1; i >= 0; i--) {
            HWND hWnd = reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(i)));
            const int nSel = static_cast<int>(ListSend(d, LB_GETSEL, static_cast<WPARAM>(i)));
            ::ShowWindow(hWnd, nSel > 0 ? SW_RESTORE : SW_MINIMIZE);
        }
        ::SendMessage(hMDIClient, uMsg, static_cast<WPARAM>(flag), 0);
        ::LockWindowUpdate(nullptr);
    }
    impl__EndWaitCursor_CCmdTarget__QEAAXXZ(impl__AfxGetApp__YAPEAVCWinApp__XZ());
}

// Retail (RVA 0x1c9b50, mfc140u), transcribed:
//     if (!CMDIFrameWndEx::m_bDisableSetRedraw)                 // data 0x3b1afc
//         GetParent()->SetRedraw(FALSE);                        // ::GetParent(m_hWnd) + CWnd::FromHandle
//     if (m_wndList.GetSelCount() == 1) {                       // LB_GETSELCOUNT
//         int index;
//         if (m_wndList.GetSelItems(1, &index) == 1) {          // LB_GETSELITEMS
//             HWND hWnd = (HWND)m_wndList.GetItemData(index);   // LB_GETITEMDATA
//             if (hWnd != (HWND)LB_ERR) {                       // `cmp $-1`
//                 WINDOWPLACEMENT wndpl;  wndpl.length = sizeof(WINDOWPLACEMENT);   // 0x2c
//                 ::GetWindowPlacement(hWnd, &wndpl);
//                 if (wndpl.showCmd == SW_SHOWMINIMIZED) ::ShowWindow(hWnd, SW_RESTORE);
//                 ::SendMessage(m_pMDIFrame->m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)hWnd, 0);
//                 EndDialog(IDOK);                              // 0x208b10
//             }
//         }
//     }
//     if (!CMDIFrameWndEx::m_bDisableSetRedraw) {
//         GetParent()->SetRedraw(TRUE);
//         GetParent()->RedrawWindow(NULL, NULL,                 // ::RedrawWindow
//             RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);   // 0x185
//     }
// The GetParent() round trip through CWnd::FromHandle only turns the parent
// HWND back into the same m_hWnd, so the HWND is used directly (no temporary
// CWnd); a NULL parent skips the call (deviation 3).  NULL m_pMDIFrame skips
// only the WM_MDIACTIVATE send (deviation 3).
// Symbol: ?OnActivate@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnActivate_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    if (impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA == 0) {
        if (HWND hParent = ::GetParent(Wnd(pThis)->m_hWnd)) ::SendMessage(hParent, WM_SETREDRAW, FALSE, 0);
    }
    if (ListSend(d, LB_GETSELCOUNT) == 1) {
        int index = 0;
        if (ListSend(d, LB_GETSELITEMS, 1, reinterpret_cast<LPARAM>(&index)) == 1) {
            HWND hWnd = reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(index)));
            if (hWnd != reinterpret_cast<HWND>(static_cast<LONG_PTR>(LB_ERR))) {
                WINDOWPLACEMENT wndpl;
                wndpl.length = sizeof(WINDOWPLACEMENT);
                ::GetWindowPlacement(hWnd, &wndpl);
                if (wndpl.showCmd == SW_SHOWMINIMIZED) ::ShowWindow(hWnd, SW_RESTORE);
                if (d->m_pMDIFrame != nullptr) {
                    ::SendMessage(MdiClient(d), WM_MDIACTIVATE, reinterpret_cast<WPARAM>(hWnd), 0);
                }
                impl__EndDialog_CDialog__QEAAXH_Z(Dlg(pThis), IDOK);
            }
        }
    }
    if (impl__m_bDisableSetRedraw_CMDIFrameWndEx__2HA == 0) {
        if (HWND hParent = ::GetParent(Wnd(pThis)->m_hWnd)) ::SendMessage(hParent, WM_SETREDRAW, TRUE, 0);
        if (HWND hParent = ::GetParent(Wnd(pThis)->m_hWnd)) {
            ::RedrawWindow(hParent, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);
        }
    }
}
static_assert((RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW) == 0x185, "OnActivate: mov $0x185,%r9d");
static_assert(sizeof(WINDOWPLACEMENT) == 0x2c && offsetof(WINDOWPLACEMENT, showCmd) == 8, "OnActivate: movl $0x2c; cmpl $0x2,0x30(%rsp)");

// Retail (RVA 0x1c9ee0, mfc140u), fully transcribed: tail jump
// MDIMessage(WM_MDICASCADE /*0x227*/, 0).
// Symbol: ?OnCascade@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnCascade_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    impl__MDIMessage_CMFCWindowsManagerDialog__IEAAXI_K_Z(pThis, WM_MDICASCADE, 0);
}

// Retail (RVA 0x1c9df0, mfc140u), transcribed:
//     int nItems = m_wndList.GetCount();                        // LB_GETCOUNT
//     if (nItems > 0) {
//         HWND hMDIClient = m_pMDIFrame->m_hWndMDIClient;
//         m_wndList.SetRedraw(FALSE);
//         for (int i = nItems - 1; i >= 0; i--) {
//             if (m_wndList.GetSel(i) > 0) {                    // LB_GETSEL
//                 HWND hWnd = (HWND)m_wndList.GetItemData(i);   // LB_GETITEMDATA
//                 ::SendMessage(hWnd, WM_CLOSE, 0, 0);
//                 if (::GetParent(hWnd) == hMDIClient) break;   // the child survived the close
//             }
//         }
//         m_wndList.SetRedraw(TRUE);
//     }
//     FillWindowList();  SelActive();  UpdateButtons();         // the last a tail jump
// NULL m_pMDIFrame: deviation (3), hMDIClient is NULL (a child whose parent
// is NULL then also stops the loop).
// Symbol: ?OnClose@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    const int nItems = static_cast<int>(ListSend(d, LB_GETCOUNT));
    if (nItems > 0) {
        const HWND hMDIClient = MdiClient(d);
        ListSend(d, WM_SETREDRAW, FALSE);
        for (int i = nItems - 1; i >= 0; i--) {
            if (static_cast<int>(ListSend(d, LB_GETSEL, static_cast<WPARAM>(i))) > 0) {
                HWND hWnd = reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(i)));
                ::SendMessage(hWnd, WM_CLOSE, 0, 0);
                if (::GetParent(hWnd) == hMDIClient) break;
            }
        }
        ListSend(d, WM_SETREDRAW, TRUE);
    }
    impl__FillWindowList_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__SelActive_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x1ca740, mfc140u), transcribed:
//     if (nIDCtl != IDC_AFXBARRES_LIST)
//         CWnd::OnDrawItem(nIDCtl, lpDIS);                      // 0x28c040 -- and then FALLS THROUGH:
//                                                               // there is no return after this call
//     if (lpDIS->itemID == (UINT)-1) return;
//     BOOL bSel = lpDIS->itemState & ODS_SELECTED;
//     CBrush* pBrush = bSel ? &afxGlobalData.brHilite : &afxGlobalData.brWindow;
//     COLORREF clrText = bSel ? afxGlobalData.clrTextHilite : afxGlobalData.clrWindowText;
//         // (each afxGlobalData use carries the Initialize-on-first-use prologue)
//     CRect rect;  ::CopyRect(&rect, &lpDIS->rcItem);
//     CDC* pDC = CDC::FromHandle(lpDIS->hDC);                   // 0x2a2450
//     if (lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT))    // `testb $0x3,0xc`
//         ::FillRect(pDC->m_hDC, &rect, (HBRUSH)pBrush->m_hObject);
//     pDC->SetBkMode(TRANSPARENT);                              // 0x2a2860
//     pDC->SetTextColor(clrText);                               // vslot 14 (+0x70) == 0x2a2960
//     CString strText;  m_wndList.GetText(lpDIS->itemID, strText);   // 0x294030
//     CRect rectText = rect;  rectText.left += rect.Height() + 4;
//     pDC->DrawText(strText, strText.GetLength(), &rectText,    // vslot 28 (+0xe0) == 0x1e6ce0
//                   DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);       // 0x824
//     HICON hIcon = (HICON)::GetClassLongPtr((HWND)lpDIS->itemData, GCLP_HICONSM);   // -34
//     if (hIcon != NULL) {
//         CRect rectIcon = rect;  rectIcon.right = rect.left + rect.Height();
//         ::InflateRect(&rectIcon, -2, 0);
//         ::DrawIconEx(pDC->m_hDC, rectIcon.left, rectIcon.top, hIcon,
//                      rectIcon.Height(), rectIcon.Height(), 0, NULL, DI_NORMAL);
//     }
//     if (lpDIS->itemAction & ODA_FOCUS) ::DrawFocusRect(pDC->m_hDC, &rect);
// The two CDC virtuals are called through their base implementations
// (deviation 1).  Retail dispatches through the vftable of whatever
// CDC::FromHandle returns: normally a temporary plain CDC, but the permanent
// object when the HDC is already attached to one, in which case a derived
// override of SetTextColor / DrawText would run in retail and is bypassed
// here.  CDC::DrawText's body (0x1e6ce0) is `::DrawTextW(m_hDC, ...)`, called
// directly here as CMFCKeyMapDialog.cpp does.  A NULL lpDIS or a NULL pDC
// (FromHandle of a NULL HDC) returns early where retail would fault
// (deviation 3).
// Symbol: ?OnDrawItem@CMFCWindowsManagerDialog@@IEAAXHPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__OnDrawItem_CMFCWindowsManagerDialog__IEAAXHPEAUtagDRAWITEMSTRUCT___Z(void* pThis, int nIDCtl, DRAWITEMSTRUCT* lpDIS) {
    if (pThis == nullptr || lpDIS == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    if (nIDCtl != static_cast<int>(kIdcList)) {
        impl__OnDrawItem_CWnd__IEAAXHPEAUtagDRAWITEMSTRUCT___Z(Wnd(pThis), nIDCtl, lpDIS);
    }
    if (lpDIS->itemID == static_cast<UINT>(-1)) return;

    const bool bSel = (lpDIS->itemState & ODS_SELECTED) != 0;
    EnsureGlobalData();
    const HBRUSH hbr = GlobalData<HBRUSH>(bSel ? kGdBrHiliteHandle : kGdBrWindowHandle);
    const COLORREF clrText = GlobalData<COLORREF>(bSel ? kGdClrTextHilite : kGdClrWindowText);

    RECT rect;
    ::CopyRect(&rect, &lpDIS->rcItem);
    CDC* pDC = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(lpDIS->hDC);
    if (pDC == nullptr) return;
    if ((lpDIS->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)) != 0) {
        ::FillRect(pDC->m_hDC, &rect, hbr);
    }
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);

    CString strText;
    impl__GetText_CListBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        ListBox(d), static_cast<int>(lpDIS->itemID), &strText);
    RECT rectText = rect;
    rectText.left = rect.left + 4 + (rect.bottom - rect.top);
    ::DrawTextW(pDC->m_hDC, strText.GetString(), strText.GetLength(), &rectText,
                DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER);

    HICON hIcon = reinterpret_cast<HICON>(::GetClassLongPtrW(reinterpret_cast<HWND>(lpDIS->itemData), GCLP_HICONSM));
    if (hIcon != nullptr) {
        RECT rectIcon = rect;
        rectIcon.right = rect.left + (rect.bottom - rect.top);
        ::InflateRect(&rectIcon, -2, 0);
        const int cy = rectIcon.bottom - rectIcon.top;
        ::DrawIconEx(pDC->m_hDC, rectIcon.left, rectIcon.top, hIcon, cy, cy, 0, nullptr, DI_NORMAL);
    }
    if ((lpDIS->itemAction & ODA_FOCUS) != 0) {
        ::DrawFocusRect(pDC->m_hDC, &rect);
    }
}
static_assert((DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER) == 0x824, "OnDrawItem: movl $0x824,0x20(%rsp)");
static_assert(GCLP_HICONSM == -34, "OnDrawItem: mov $0xffffffde,%edx");

// Retail (RVA 0x1caa40, mfc140u -- not in the RVA symbol map; the ordinal
// resolves here and the message map's WM_HELPINFO entry points here),
// transcribed:
//     AfxGetThread()->GetMainWnd()->SendMessage(AFX_WM_WINDOW_HELP, 0, (LPARAM)this);   // data 0x3c2738
//     return FALSE;
// (the thread comes from AfxGetModuleThreadState()->m_pCurrentWinThread and
// GetMainWnd is its vslot 31, see MainWnd()).  pHelpInfo is not read.  NULL
// main window: deviation (3), no message is sent.
// Symbol: ?OnHelpInfo@CMFCWindowsManagerDialog@@IEAAHPEAUtagHELPINFO@@@Z
extern "C" int MS_ABI impl__OnHelpInfo_CMFCWindowsManagerDialog__IEAAHPEAUtagHELPINFO___Z(void* pThis, HELPINFO* pHelpInfo) {
    (void)pHelpInfo;
    if (CWnd* pMainWnd = MainWnd()) {
        ::SendMessage(pMainWnd->m_hWnd, impl__AFX_WM_WINDOW_HELP__3IA, 0, reinterpret_cast<LPARAM>(pThis));
    }
    return FALSE;
}

// Retail (RVA 0x1c9fe0, mfc140u -- vftable slot 96 +0x300), transcribed:
//     CDialog::OnInitDialog();                                  // 0x208d10, called directly, result dropped
//     if (AfxGetThread() != NULL && AfxGetThread()->GetMainWnd() != NULL
//         && (AfxGetThread()->GetMainWnd()->GetExStyle() & WS_EX_LAYOUTRTL))   // 0x2a96c0; 0x400000
//         ModifyStyleEx(0, WS_EX_LAYOUTRTL);                    // 0x2a9740, nFlags 0
//     FillWindowList();
//     if (!m_bMDIActions) {
//         GetDlgItem(IDC_AFXBARRES_TILEHORZ)->ShowWindow(SW_HIDE);   // 0x2a9390 / 0x2a9ad0
//         GetDlgItem(IDC_AFXBARRES_TILEVERT)->ShowWindow(SW_HIDE);
//         GetDlgItem(IDC_AFXBARRES_CASCADE)->ShowWindow(SW_HIDE);
//         GetDlgItem(IDC_AFXBARRES_MINIMIZE)->ShowWindow(SW_HIDE);
//     }
//     SelActive();  UpdateButtons();
//     CWnd* pHelp = GetDlgItem(ID_HELP);
//     if (pHelp != NULL) pHelp->ShowWindow(m_bHelpButton ? SW_SHOW : SW_HIDE);   // `neg; sbb; and $5`
//     return TRUE;
// Retail fetches GetMainWnd twice (once for the NULL test, once for
// GetExStyle); it is fetched once here.  The four unchecked GetDlgItem
// results: deviation (3).
// Symbol: ?OnInitDialog@CMFCWindowsManagerDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCWindowsManagerDialog__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_WndMgrDlg* d = D(pThis);
    impl__OnInitDialog_CDialog__UEAAHXZ(Dlg(pThis));
    if (CWnd* pMainWnd = MainWnd()) {
        if ((impl__GetExStyle_CWnd__QEBAKXZ(pMainWnd) & WS_EX_LAYOUTRTL) != 0) {
            impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(Wnd(pThis), 0, WS_EX_LAYOUTRTL, 0);
        }
    }
    impl__FillWindowList_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    if (!d->m_bMDIActions) {
        for (UINT nID : {kIdcTileHorz, kIdcTileVert, kIdcCascade, kIdcMinimize}) {
            if (CWnd* pBtn = DlgItem(pThis, nID)) impl__ShowWindow_CWnd__QEAAHH_Z(pBtn, SW_HIDE);
        }
    }
    impl__SelActive_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    if (CWnd* pHelp = DlgItem(pThis, kIdHelp)) {
        impl__ShowWindow_CWnd__QEAAHH_Z(pHelp, d->m_bHelpButton ? SW_SHOW : SW_HIDE);
    }
    return TRUE;
}

// Retail (RVA 0x1c9f10, mfc140u), transcribed:
//     int nItems = m_wndList.GetCount();                        // LB_GETCOUNT
//     if (nItems > 0) {
//         m_wndList.SetRedraw(FALSE);
//         for (int i = nItems - 1; i >= 0; i--)
//             if (m_wndList.GetSel(i) > 0)                      // LB_GETSEL
//                 ::ShowWindow((HWND)m_wndList.GetItemData(i), SW_MINIMIZE);
//         m_wndList.SetRedraw(TRUE);
//     }
//     FillWindowList();  SelActive();  UpdateButtons();         // the last a tail jump
// Symbol: ?OnMinimize@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnMinimize_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    const int nItems = static_cast<int>(ListSend(d, LB_GETCOUNT));
    if (nItems > 0) {
        ListSend(d, WM_SETREDRAW, FALSE);
        for (int i = nItems - 1; i >= 0; i--) {
            if (static_cast<int>(ListSend(d, LB_GETSEL, static_cast<WPARAM>(i))) > 0) {
                ::ShowWindow(reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(i))), SW_MINIMIZE);
            }
        }
        ListSend(d, WM_SETREDRAW, TRUE);
    }
    impl__FillWindowList_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__SelActive_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x1c9cd0, mfc140u), transcribed:
//     int nItems = m_wndList.GetCount();                        // LB_GETCOUNT
//     for (int i = 0; i < nItems; i++) {                        // (skipped when nItems <= 0)
//         if (m_wndList.GetSel(i) <= 0) continue;               // LB_GETSEL
//         HWND hWnd = (HWND)m_wndList.GetItemData(i);           // LB_GETITEMDATA
//         if (m_lstSaveDisabled.Find(hWnd) != NULL) continue;   // inlined walk from +0x268
//         CFrameWnd* pFrame = DYNAMIC_DOWNCAST(CFrameWnd, CWnd::FromHandle(hWnd));   // 0x28ad70, IsKindOf 0x234cf0
//         if (pFrame == NULL) continue;
//         CDocument* pDoc = pFrame->GetActiveDocument();        // vslot 93 (+0x2e8)
//         if (pDoc != NULL) pDoc->DoFileSave();                 // vslot 59 (+0x1d8) == 0x21b190
//     }
//     FillWindowList();  SelActive();  UpdateButtons();         // the last a tail jump
// The two virtuals go through the CFrameWnd / CDocument base thunks
// (deviation 1).
// Symbol: ?OnSave@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSave_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    const int nItems = static_cast<int>(ListSend(d, LB_GETCOUNT));
    for (int i = 0; i < nItems; i++) {
        if (static_cast<int>(ListSend(d, LB_GETSEL, static_cast<WPARAM>(i))) <= 0) continue;
        HWND hWnd = reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(i)));
        if (ListContains(d->m_lstSaveDisabled, hWnd)) continue;
        CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd);
        if (pWnd == nullptr ||
            !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pWnd, impl__GetThisClass_CFrameWnd__SAPEAUCRuntimeClass__XZ())) {
            continue;
        }
        CDocument* pDoc = impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(static_cast<CFrameWnd*>(pWnd));
        if (pDoc != nullptr) {
            impl__DoFileSave_CDocument__UEAAHXZ(pDoc);
        }
    }
    impl__FillWindowList_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__SelActive_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
    impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x1ca110, mfc140u), fully transcribed: `jmp 0x1ca120`, i.e.
// UpdateButtons().
// Symbol: ?OnSelchangeList@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeList_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x1c9ef0, mfc140u), fully transcribed: tail jump
// MDIMessage(WM_MDITILE /*0x226*/, MDITILE_HORIZONTAL /*1*/).
// Symbol: ?OnTilehorz@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnTilehorz_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    impl__MDIMessage_CMFCWindowsManagerDialog__IEAAXI_K_Z(pThis, WM_MDITILE, MDITILE_HORIZONTAL);
}

// Retail (RVA 0x1c9f00, mfc140u), fully transcribed: tail jump
// MDIMessage(WM_MDITILE /*0x226*/, MDITILE_VERTICAL /*0*/).
// Symbol: ?OnTilevert@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnTilevert_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    impl__MDIMessage_CMFCWindowsManagerDialog__IEAAXI_K_Z(pThis, WM_MDITILE, MDITILE_VERTICAL);
}
static_assert(WM_MDICASCADE == 0x227 && WM_MDITILE == 0x226 && MDITILE_HORIZONTAL == 1 && MDITILE_VERTICAL == 0,
              "OnCascade / OnTilehorz / OnTilevert constants");

// Retail (RVA 0x1ca9f0, mfc140u -- the message map's ID_HELP entry),
// transcribed: the same send as OnHelpInfo, as a tail jump to ::SendMessageW
// (import slot 0x1802c7120):
//     AfxGetThread()->GetMainWnd()->SendMessage(AFX_WM_WINDOW_HELP, 0, (LPARAM)this);
// NULL main window: deviation (3).
// Symbol: ?OnWindowHelp@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnWindowHelp_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (CWnd* pMainWnd = MainWnd()) {
        ::SendMessage(pMainWnd->m_hWnd, impl__AFX_WM_WINDOW_HELP__3IA, 0, reinterpret_cast<LPARAM>(pThis));
    }
}

// Retail (RVA 0x1ca330, mfc140u), transcribed:
//     int nItems = m_wndList.GetCount();                        // LB_GETCOUNT
//     if (nItems > 0) {
//         m_wndList.SetRedraw(FALSE);
//         m_wndList.SelItemRange(FALSE, 0, nItems - 1);         // LB_SELITEMRANGEEX(nItems - 1, 0)
//         HWND hwndActive = (HWND)::SendMessage(m_pMDIFrame->m_hWndMDIClient, WM_MDIGETACTIVE, 0, 0);
//         for (int i = 0; i < nItems; i++) {
//             if ((HWND)m_wndList.GetItemData(i) == hwndActive) {   // LB_GETITEMDATA
//                 m_wndList.SetSel(i);                          // LB_SETSEL(TRUE, i)
//                 break;
//             }
//         }
//         m_wndList.SetRedraw(TRUE);
//     }
// NULL m_pMDIFrame: deviation (3), no WM_MDIGETACTIVE is sent and nothing is
// selected.
// Symbol: ?SelActive@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__SelActive_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    const int nItems = static_cast<int>(ListSend(d, LB_GETCOUNT));
    if (nItems <= 0) return;
    ListSend(d, WM_SETREDRAW, FALSE);
    ListSend(d, LB_SELITEMRANGEEX, static_cast<WPARAM>(nItems - 1), 0);
    if (d->m_pMDIFrame != nullptr) {
        HWND hwndActive = reinterpret_cast<HWND>(::SendMessage(MdiClient(d), WM_MDIGETACTIVE, 0, 0));
        for (int i = 0; i < nItems; i++) {
            if (reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(i))) == hwndActive) {
                ListSend(d, LB_SETSEL, TRUE, static_cast<LPARAM>(i));
                break;
            }
        }
    }
    ListSend(d, WM_SETREDRAW, TRUE);
}

// Retail (RVA 0x1ca120, mfc140u), transcribed:
//     int nSel = m_wndList.GetSelCount();                       // LB_GETSELCOUNT
//     BOOL bClose = nSel > 0;
//     BOOL bSave = FALSE;
//     for (int i = 0; bClose && i < m_wndList.GetCount(); i++) {   // LB_GETCOUNT re-sent every pass
//         if (m_wndList.GetSel(i) > 0) {                        // LB_GETSEL
//             HWND hWnd = (HWND)m_wndList.GetItemData(i);       // LB_GETITEMDATA
//             if (m_lstCloseDisabled.Find(hWnd) != NULL) bClose = FALSE;   // walk from +0x230
//             if (m_lstSaveDisabled.Find(hWnd) == NULL) bSave = TRUE;      // walk from +0x268
//         }
//     }
//     GetDlgItem(IDC_AFXBARRES_CLOSE)->EnableWindow(bClose);    // 0x2a9390 / 0x2a9b30
//     GetDlgItem(IDC_AFXBARRES_SAVE)->EnableWindow(bSave);
//     GetDlgItem(IDC_AFXBARRES_TILEHORZ)->EnableWindow(m_bMDIActions && nSel >= 2);
//     GetDlgItem(IDC_AFXBARRES_TILEVERT)->EnableWindow(m_bMDIActions && nSel >= 2);
//     GetDlgItem(IDC_AFXBARRES_CASCADE)->EnableWindow(m_bMDIActions && nSel >= 2);
//     GetDlgItem(IDC_AFXBARRES_MINIMIZE)->EnableWindow(m_bMDIActions && nSel > 0);
//     GetDlgItem(IDC_AFXBARRES_ACTIVATE)->EnableWindow(nSel == 1);   // tail jump
// The unchecked GetDlgItem results: deviation (3).
// Symbol: ?UpdateButtons@CMFCWindowsManagerDialog@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateButtons_CMFCWindowsManagerDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_WndMgrDlg* d = D(pThis);
    const int nSel = static_cast<int>(ListSend(d, LB_GETSELCOUNT));
    BOOL bClose = nSel > 0;
    BOOL bSave = FALSE;
    for (int i = 0; bClose && i < static_cast<int>(ListSend(d, LB_GETCOUNT)); i++) {
        if (static_cast<int>(ListSend(d, LB_GETSEL, static_cast<WPARAM>(i))) > 0) {
            HWND hWnd = reinterpret_cast<HWND>(ListSend(d, LB_GETITEMDATA, static_cast<WPARAM>(i)));
            if (ListContains(d->m_lstCloseDisabled, hWnd)) bClose = FALSE;
            if (!ListContains(d->m_lstSaveDisabled, hWnd)) bSave = TRUE;
        }
    }
    const BOOL bMulti = d->m_bMDIActions && nSel >= 2;
    const struct { UINT nID; BOOL bEnable; } items[] = {
        {kIdcClose, bClose},
        {kIdcSave, bSave},
        {kIdcTileHorz, bMulti},
        {kIdcTileVert, bMulti},
        {kIdcCascade, bMulti},
        {kIdcMinimize, d->m_bMDIActions && nSel > 0},
        {kIdcActivate, nSel == 1},
    };
    for (const auto& it : items) {
        if (CWnd* pBtn = DlgItem(pThis, it.nID)) impl__EnableWindow_CWnd__QEAAHH_Z(pBtn, it.bEnable);
    }
}
