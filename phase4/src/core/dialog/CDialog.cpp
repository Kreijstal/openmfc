// CDialog — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"

#include <cstddef>

// ---- sibling impl__ exports called by the bodies below ----------------------
// Each matches a definition that exists today; the defining file is named.
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo); // core/runtime/CCmdTarget.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);     // core/window/CWnd.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();            // core/app/Globals.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);                            // core/runtime/Globals.cpp
extern "C" int MS_ABI impl__AfxUnhookWindowCreate__YAHXZ();                          // featurepack/CMFC_misc_stubs.cpp
extern "C" HWND__* MS_ABI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd* pThis);        // core/window/Thunks.cpp
extern "C" void MS_ABI impl__DoEnableModeless_CWinApp__SAXH_Z(int bEnable);          // core/app/CWinApp.cpp

// OpenMFC's stand-in for AfxGetModuleState()->m_pCurrentWinApp: the plain global
// the CWinApp constructor export stores `this` into (core/app/CWinApp.cpp; defined
// in detail/CWinAppSupport.cpp, declared the same way in detail/CWinAppSupport.h).
// AfxGetApp()/impl__AfxGetApp are NOT used for it: in this build they return
// AfxGetThread(), which on a thread with its own CWinThread is that thread object,
// not the application.
extern CWinApp* g_pApp;


namespace {

// ---------------------------------------------------------------------------
// Retail CDialog data layout (afxwin.h:3332, _AFXDLL x64), read off the retail
// constructor ??0CDialog@@QEAA@XZ (RVA 0x207e50 (mfc140u)) and
// CDialog::Initialize (RVA 0x207ee0 (mfc140u)), which store every member:
//   +0xe8  UINT            m_nIDHelp             (dword store)
//   +0xf0  LPCTSTR         m_lpszTemplateName
//   +0xf8  HGLOBAL         m_hDialogTemplate
//   +0x100 LPCDLGTEMPLATE  m_lpDialogTemplate
//   +0x108 void*           m_lpDialogInit
//   +0x110 CWnd*           m_pParentWnd
//   +0x118 HWND            m_hWndTop
//   +0x120 BOOL            m_bClosedByEndDialog  (dword store)
//   +0x128 _AFX_OCC_DIALOG_INFO* m_pOccDialogInfo
//   sizeof 0x130 (CPrintDialogEx's first own member sits at +0x130).
//
struct S_CDialogRetailTail {
    unsigned char          m_head[0xf8];          // CWnd + OpenMFC's m_lpszTemplateName/m_nIDHelp
    HGLOBAL                m_hDialogTemplate;     // +0xf8
    const DLGTEMPLATE*     m_lpDialogTemplate;    // +0x100
    void*                  m_lpDialogInit;        // +0x108
    CWnd*                  m_pParentWnd;          // +0x110
    HWND                   m_hWndTop;             // +0x118
    BOOL                   m_bClosedByEndDialog;  // +0x120
    _AFX_OCC_DIALOG_INFO*  m_pOccDialogInfo;      // +0x128
};
static_assert(offsetof(S_CDialogRetailTail, m_hDialogTemplate) == 0xf8, "Initialize: mov %rax,0xf8(%rcx)");
static_assert(offsetof(S_CDialogRetailTail, m_lpDialogTemplate) == 0x100, "Initialize: mov %rax,0x100(%rcx)");
static_assert(offsetof(S_CDialogRetailTail, m_lpDialogInit) == 0x108, "Initialize: mov %rax,0x108(%rcx)");
static_assert(offsetof(S_CDialogRetailTail, m_pParentWnd) == 0x110, "ctor(UINT,CWnd*): mov %rbx,0x110(%rsi)");
static_assert(offsetof(S_CDialogRetailTail, m_hWndTop) == 0x118, "PostModal: mov 0x118(%rbx),%rcx");
static_assert(offsetof(S_CDialogRetailTail, m_bClosedByEndDialog) == 0x120, "Initialize: mov %eax,0x120(%rcx)");
static_assert(offsetof(S_CDialogRetailTail, m_pOccDialogInfo) == 0x128, "SetOccDialogInfo: mov %rdx,0x128(%rcx)");
static_assert(sizeof(S_CDialogRetailTail) == 0x130, "retail sizeof(CDialog)");

static_assert(sizeof(CDialog)==0x130 && offsetof(CDialog,m_nIDHelp)==0xe8 && offsetof(CDialog,m_lpszTemplateName)==0xf0,"native dialog prefix");
static_assert(offsetof(CDialogExAccess,m_clrBackground)==0x130,"extended dialog follows the complete native base");

inline S_CDialogRetailTail* Tail(CDialog* p) { return reinterpret_cast<S_CDialogRetailTail*>(p); }

// OpenMFC keeps the indirect template in a side table (InitModalIndirect in this
// file stores it there, DoModal in detail/DlgcoreSupport.cpp reads it from
// there) instead of retail's m_lpDialogTemplate (+0x100), so that table is what
// stands in for the member when a body below reads it.
inline const DLGTEMPLATE* IndirectTemplateOf(CDialog* p) {return p->m_lpDialogTemplate ? p->m_lpDialogTemplate : static_cast<const DLGTEMPLATE*>(::LockResource(p->m_hDialogTemplate));}

// Native command routing uses CCmdTarget virtual slot 5.
constexpr std::size_t kVs_OnCmdMsg = 5;
// CWinApp::WinHelpInternal: `mov 0x1a0(%rdx),%rax` at 0x208efc inside
// CDialog::OnCommandHelp (entry 0x208ec0, mfc140u).  Slot 52 of the mfc140u
// CWinApp vftable (RVA 0x320d58, installed by ??0CWinApp@@QEAA@PEB_W@Z) holds
// 0x1cc990, which is the export ?WinHelpInternal@CWinApp@@UEAAX_KI@Z
// (ordinal 14210) -- all mfc140u.
constexpr std::size_t kVs_WinApp_WinHelpInternal = 52;
using OnCmdMsgFn = int (MS_ABI*)(void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
using WinHelpInternalFn = void (MS_ABI*)(void* pThis, unsigned long long dwData, unsigned int nCmd);

template <typename Fn>
inline Fn VSlot(const void* pObj, std::size_t nSlot) {
    return reinterpret_cast<Fn>((*static_cast<void* const* const*>(pObj))[nSlot]);
}

int DispatchOnCmdMsg(CCmdTarget* target,UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) {
    return VSlot<OnCmdMsgFn>(target,kVs_OnCmdMsg)(target,id,code,extra,info);
}

} // namespace

// CDialog::CheckAutoCenter -- transcribed from RVA 0x208df0 (mfc140u; found
// through ordinal 2697 -- mfc_complete_ordinal_mapping.json, mfc140u table --
// in the export address table).  Retail:
//     const DLGTEMPLATE* lpTemplate = m_lpDialogTemplate;          // +0x100
//     HGLOBAL hTemplate = m_hDialogTemplate;                        // +0xf8
//     if (m_lpszTemplateName != NULL) {                             // +0xf0
//         HINSTANCE hInst = AfxFindResourceHandle(m_lpszTemplateName, RT_DIALOG);  // call 0x2aeb50
//         HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);  // FindResourceW
//         if (hResource != NULL)
//             hTemplate = ::LoadResource(hInst, hResource);
//     }
//     if (hTemplate != NULL)
//         lpTemplate = (DLGTEMPLATE*)::LockResource(hTemplate);
//     if (lpTemplate == NULL) return TRUE;
//     DLGTEMPLATEEX when the WORD at +2 is 0xFFFF: style at +0xc, x at +0x12, y at +0x14;
//     plain DLGTEMPLATE otherwise:                 style at +0x0, x at +0xa,  y at +0xc.
//     if (style & (DS_ABSALIGN|DS_CENTER|DS_CENTERMOUSE)) return FALSE;   // testl $0x1801
//     return (x == 0 && y == 0);                                          // 16-bit compares
// (import slots 0x1802c6588/0x1802c6580/0x1802c6550 of mfc140u resolve to
// FindResourceW, LoadResource, LockResource.)  Nothing is unlocked or freed.
// DEVIATION: m_lpDialogTemplate is read from OpenMFC's indirect-template side
// table (IndirectTemplateOf) and m_hDialogTemplate is taken as NULL -- OpenMFC
// never stores a separate template handle (InitModalIndirect(HGLOBAL) in this
// file files the handle in that same table and, like DoModal, uses it directly
// as the template pointer, which holds for GMEM_FIXED memory only), and the retail +0xf8/+0x100 slots are
// not initialised by the OpenMFC constructor exports, so they are not read.
// OpenMFC's m_lpszTemplateName is used by name (see the layout note above).
// The NULL-`this` guard is OpenMFC's; retail dereferences `this` unconditionally.
// (The pre-existing `Ordinal: 2701` line below disagrees with the mfc140u
// table in mfc_complete_ordinal_mapping.json, which gives 2697; its source is
// not recorded.)
// Symbol: ?CheckAutoCenter@CDialog@@UEAAHXZ
// Ordinal: 2701
extern "C" int MS_ABI impl__CheckAutoCenter_CDialog__UEAAHXZ(CDialog* pThis) {
    if (!pThis) return TRUE;
    const DLGTEMPLATE* lpTemplate = IndirectTemplateOf(pThis);
    HGLOBAL hTemplate = nullptr;
    if (pThis->m_lpszTemplateName != nullptr) {
        HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
            pThis->m_lpszTemplateName, RT_DIALOG);
        HRSRC hResource = ::FindResourceW(hInst, pThis->m_lpszTemplateName, RT_DIALOG);
        if (hResource != nullptr) {
            hTemplate = ::LoadResource(hInst, hResource);
        }
    }
    if (hTemplate != nullptr) {
        lpTemplate = static_cast<const DLGTEMPLATE*>(::LockResource(hTemplate));
    }
    if (lpTemplate == nullptr) return TRUE;

    const unsigned char* p = reinterpret_cast<const unsigned char*>(lpTemplate);
    const bool bEx = *reinterpret_cast<const WORD*>(p + 2) == 0xFFFF;
    const DWORD dwStyle = *reinterpret_cast<const DWORD*>(p + (bEx ? 0xc : 0x0));
    const short x = *reinterpret_cast<const short*>(p + (bEx ? 0x12 : 0xa));
    const short y = *reinterpret_cast<const short*>(p + (bEx ? 0x14 : 0xc));
    if (dwStyle & (DS_ABSALIGN | DS_CENTER | DS_CENTERMOUSE)) return FALSE;
    return (x == 0 && y == 0) ? TRUE : FALSE;
}
// Symbol: ?PreTranslateMessage@CDialog@@UEAAHPEAUtagMSG@@@Z
// Ordinal: 11861
extern "C" int MS_ABI impl__PreTranslateMessage_CDialog__UEAAHPEAUtagMSG___Z(
    CDialog* pThis, MSG* pMsg)
{
    if (pThis && pThis->m_hWnd && pMsg) {
        // Handle dialog navigation keys
        if (IsDialogMessageW(pThis->m_hWnd, pMsg)) {
            return TRUE;
        }
    }
    return FALSE;
}
namespace {
BOOL CreateIndirectWindow(CDialog* self,const DLGTEMPLATE* data,CWnd* parent,HINSTANCE module) {
    if(!self || !data)return FALSE;
    if(!module)module=AfxGetInstanceHandle();
    if(!parent) {CWinThread* thread=AfxGetThread();if(thread)parent=thread->GetMainWnd();}
    HWND dialog=::CreateDialogIndirectParamW(module,data,parent ? parent->m_hWnd : nullptr,AfxDlgProc,reinterpret_cast<LPARAM>(self));
    if(!dialog)return FALSE;self->m_hWnd=dialog;g_dlgMap[dialog]=self;return TRUE;
}
}
// Symbol: ?CreateIndirect@CDialog@@IEAAHPEAXPEAVCWnd@@PEAUHINSTANCE__@@@Z
extern "C" BOOL MS_ABI impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(CDialog* self,HGLOBAL data,CWnd* parent,HINSTANCE module) {
    self->m_lpDialogInit=nullptr;
    return CreateIndirectWindow(self,static_cast<const DLGTEMPLATE*>(::LockResource(data)),parent,module);
}
// Symbol: ?CreateIndirect@CDialog@@IEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAXPEAUHINSTANCE__@@@Z
extern "C" BOOL MS_ABI impl__CreateIndirect_CDialog__IEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAXPEAUHINSTANCE_____Z(CDialog* self,const DLGTEMPLATE* data,CWnd* parent,void* init,HINSTANCE module) {
    self->m_lpDialogInit=init;return CreateIndirectWindow(self,data,parent,module);
}
// Symbol: ?CreateIndirect@CDialog@@UEAAHPEAXPEAVCWnd@@@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__UEAAHPEAXPEAVCWnd___Z(
    CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd) {
    return impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(
        pThis, lpDialogTemplate, pParentWnd, nullptr);
}
// Symbol: ?CreateIndirect@CDialog@@UEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAX@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__UEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(
    CDialog* pThis, const DLGTEMPLATE* lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit) {
    return impl__CreateIndirect_CDialog__IEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAXPEAUHINSTANCE_____Z(
        pThis, lpDialogTemplate, pParentWnd, lpDialogInit, nullptr);
}
// Symbol: ?GetMessageMap@CDialog@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDialog__MEBAPEBUAFX_MSGMAP__XZ(
    const CDialog* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialog__UEBAPEAUCRuntimeClass__XZ(
    const CDialog* pThis) {
    return CDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ() {
    return CDialog::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CDialog@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?HandleInitDialog@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__HandleInitDialog_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;
    return pThis ? impl__OnInitDialog_CDialog__UEAAHXZ(pThis) : TRUE;
}
// CDialog::OnCmdMsg -- transcribed from RVA 0x207d80 (mfc140u; named by the
// mfc140u symbol map and by ordinal 8901 in the export address table).  Retail:
//     if (CCmdTarget::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))   // direct call 0x1de460
//         return TRUE;
//     if ((nCode != CN_COMMAND && nCode != CN_UPDATE_COMMAND_UI)    // 0 / -1
//         || !(nID & 0x8000)                                        // bt $0xf,%edi
//         || nID >= 0xf000)
//         return FALSE;
//     CWnd* pOwner = CWnd::FromHandle(::GetParent(m_hWnd));         // import 0x1802c72d8 = GetParent; call 0x28ad70
//     if (pOwner != NULL && pOwner->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))  // vslot 5 (+0x28)
//         return TRUE;
//     CWinThread* pThread = AfxGetModuleThreadState()->m_pCurrentWinThread;  // call 0x133a20, +0x8
//     if (pThread != NULL && pThread->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) // vslot 5 (+0x28)
//         return TRUE;
//     return FALSE;
// The owner and current thread route through the same native command slot.
// Symbol: ?OnCmdMsg@CDialog@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CDialog__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CDialog* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    if (impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
            pThis, nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }
    if ((nCode != 0 /* CN_COMMAND */ && nCode != -1 /* CN_UPDATE_COMMAND_UI */) ||
        (nID & 0x8000) == 0 || nID >= 0xf000) {
        return FALSE;
    }
    CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pOwner != nullptr &&
        DispatchOnCmdMsg(pOwner, nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }
    CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
    if (pThread != nullptr &&
        DispatchOnCmdMsg(pThread, nID, nCode, pExtra, pHandlerInfo)) {
        return TRUE;
    }
    return FALSE;
}
// CDialog::OnCommandHelp -- transcribed from RVA 0x208ec0 (mfc140u; ordinal 8949).
// Retail:
//     if (lParam == 0) {
//         if (m_nIDHelp == 0) return 0;                  // +0xe8
//         lParam = (DWORD)(m_nIDHelp + HID_BASE_RESOURCE);  // add $0x20000; mov %eax,%ebx
//         if (lParam == 0) return 0;                     // the jne after the add (32-bit wrap only)
//     }
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;   // call 0x133930, +0x8
//     if (pApp != NULL)
//         pApp->WinHelpInternal(lParam, HELP_CONTEXT);   // vslot 52 (+0x1a0), r8d = 1
//     return TRUE;
// (Slot 52 is WinHelpInternal: see kVs_WinApp_WinHelpInternal at the top of
// the file -- the slot's entry is that export's own RVA.)
// m_nIDHelp is read by its OpenMFC name (see the layout note at the top of the
// file); g_pApp stands in for m_pCurrentWinApp; the application object is a
// client object and so carries an MSVC vftable (no CWinApp is constructed
// inside this DLL).  The NULL-`this` guard is OpenMFC's.
// Symbol: ?OnCommandHelp@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnCommandHelp_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    if (lParam == 0) {
        if (pThis == nullptr || pThis->m_nIDHelp == 0) return 0;
        lParam = static_cast<LPARAM>(static_cast<DWORD>(pThis->m_nIDHelp + 0x20000u /* HID_BASE_RESOURCE */));
        if (lParam == 0) return 0;
    }
    CWinApp* pApp = g_pApp;
    if (pApp != nullptr) {
        VSlot<WinHelpInternalFn>(pApp, kVs_WinApp_WinHelpInternal)(
            pApp, static_cast<unsigned long long>(lParam), HELP_CONTEXT);
    }
    return TRUE;
}
// Restart-manager session callbacks live in DialogMessageCallbacks.cpp.
// CDialog::OnHelpHitTest -- transcribed from RVA 0x208f20 (mfc140u; ordinal
// 10077).  Retail, in full:
//     return m_nIDHelp != 0 ? HID_BASE_RESOURCE + m_nIDHelp : 0;   // +0xe8, add $0x20000
// The sum is a 32-bit add (eax), zero-extended into the LRESULT.  m_nIDHelp is
// read by its OpenMFC name (see the layout note at the top of the file).  The
// NULL-`this` guard is OpenMFC's.
// Symbol: ?OnHelpHitTest@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnHelpHitTest_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;
    if (pThis == nullptr || pThis->m_nIDHelp == 0) return 0;
    return static_cast<intptr_t>(static_cast<DWORD>(pThis->m_nIDHelp + 0x20000u /* HID_BASE_RESOURCE */));
}
// Symbol: ?OnPaint@CDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CDialog__IEAAXXZ(CDialog* pThis) {
    if (!pThis || !pThis->m_hWnd) return;
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(pThis->m_hWnd, &ps);
    if (hdc) {
        EndPaint(pThis->m_hWnd, &ps);
    }
}
// CDialog::PostModal -- transcribed from RVA 0x208850 (mfc140u; the RVA has no
// entry in the symbol map and was found through ordinal 11757 in the export
// address table).  Retail:
//     AfxUnhookWindowCreate();                          // call 0x28b400
//     Detach();                                         // call 0x28ae50 (CWnd::Detach)
//     if (::IsWindow(m_hWndTop))                        // +0x118; import 0x1802c7138 = IsWindow
//         ::EnableWindow(m_hWndTop, TRUE);              // import 0x1802c7358 = EnableWindow
//     m_hWndTop = NULL;
//     if (AfxGetModuleState()->m_pCurrentWinApp != NULL)   // call 0x133930, +0x8
//         CWinApp::DoEnableModeless(TRUE);              // call 0x1ce9f0 (static; ecx = 1)
// Symbol: ?PostModal@CDialog@@IEAAXXZ
extern "C" void MS_ABI impl__PostModal_CDialog__IEAAXXZ(CDialog* pThis) {
    if (!pThis) return;
    impl__AfxUnhookWindowCreate__YAHXZ();
    impl__Detach_CWnd__QEAAPEAUHWND____XZ(pThis);
    S_CDialogRetailTail* t = Tail(pThis);
    if (::IsWindow(t->m_hWndTop)) {
        ::EnableWindow(t->m_hWndTop, TRUE);
    }
    t->m_hWndTop = nullptr;
    if (g_pApp != nullptr) {
        impl__DoEnableModeless_CWinApp__SAXH_Z(TRUE);
    }
}
// CDialog::PreInitDialog -- retail is an empty function.  Its export (ordinal
// 11813 in the mfc140u export address table) resolves to RVA 0x27d0 (mfc140u),
// a lone `ret $0x0` shared by identical-code folding (the symbol map names that
// RVA ?AddDockSite@CFrameWndEx@@QEAAXXZ).  So the empty body below IS the
// retail behaviour, not a placeholder.  Cross-check: slot 100 (+0x320) of the
// CDialog vftable at RVA 0x325658 (mfc140u; the table ??0CDialog@@QEAA@XZ,
// RVA 0x207e50 (mfc140u), stores into the vptr) also holds 0x27d0 (bytes
// c2 00 00), next to slot 96 = 0x208d10 (OnInitDialog) and slots 98/99 =
// OnOK/OnCancel -- PreInitDialog being the first virtual CDialog introduces
// after OnCancel in the retail afxwin.h.  The same `ret` is the virtual the
// CPrintDialogEx.cpp note reaches through slot 100.
// Symbol: ?PreModal@CDialog@@IEAAPEAUHWND__@@XZ
extern "C" HWND MS_ABI impl__PreModal_CDialog__IEAAPEAUHWND____XZ(CDialog* pThis) {
    CWnd* pParent = AfxGetMainWnd();
    HWND hWndParent = pParent ? pParent->m_hWnd : nullptr;
    return (pThis && pThis->m_hWnd) ? pThis->m_hWnd : hWndParent;
}
