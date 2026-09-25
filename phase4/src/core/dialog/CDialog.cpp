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

extern "C" IMAGE_DOS_HEADER __ImageBase;   // this DLL's own image base (linker-provided)

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
// OpenMFC's declaration (include/openmfc/afxwin.h:1053) differs in the first
// two members: it declares m_lpszTemplateName FIRST (+0xe8) and m_nIDHelp
// second (+0xf0), followed by char _dialog_padding[56] (+0xf4..+0x12c) and
// 4 bytes of tail padding.  The OpenMFC constructors in this file and in
// detail/DlgcoreSupport.cpp write those two through the declared names, so the
// bodies below also use the declared names for them (a retail-offset read of
// +0xe8 as m_nIDHelp would return the low half of the template-name pointer).
// Every other retail member lies at or above +0xf8, in bytes OpenMFC's
// CDialog does not declare (see the tail-padding caveat below), and is
// reached through S_CDialogRetailTail at its exact retail offset (the same
// approach core/dialog/CPrintDialogEx.cpp takes for +0x110/+0x118).
// Swapping the two declarations in the header would put both at their retail
// offsets without changing sizeof(CDialog) (reported as a headerRequest).
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

// The OpenMFC side, pinned.  _dialog_padding runs +0xf4..+0x12c; +0x12c..+0x130
// is CDialog's TAIL padding.  So the view touches only padding bytes of an
// OpenMFC CDialog, but the high half of m_pOccDialogInfo (+0x12c..+0x12f) is
// tail padding, which g++ (Itanium ABI) lets a derived class reuse: measured
// under this file's flags, OpenMFC's own CDialogEx places m_clrBackground at
// +0x12c.  For a client-compiled (MSVC) object there is no such overlap --
// MSVC never reuses base tail padding, and the retail CDialogEx members start
// at +0x130 -- which is the object these exports are written for.  On a
// g++-constructed OpenMFC CDialogEx, SetOccDialogInfo or a later Initialize
// would overwrite m_clrBackground.  The header swap described above would make
// the declared members end exactly at +0x130 and close this too (headerRequest).
struct CDialogPaddingProbe : CDialog {
    using CDialog::_dialog_padding;
};
static_assert(sizeof(CDialog) == 0x130, "OpenMFC CDialog is the retail size");
static_assert(offsetof(CDialog, m_lpszTemplateName) == 0xe8, "OpenMFC m_lpszTemplateName (retail: m_nIDHelp)");
static_assert(offsetof(CDialog, m_nIDHelp) == 0xf0, "OpenMFC m_nIDHelp (retail: m_lpszTemplateName)");
static_assert(offsetof(CDialogPaddingProbe, _dialog_padding) == 0xf4 &&
              offsetof(CDialogPaddingProbe, _dialog_padding) + sizeof(CDialogPaddingProbe::_dialog_padding) == 0x12c,
              "OpenMFC CDialog::_dialog_padding is +0xf4..+0x12c; +0x12c..+0x130 is tail padding");
static_assert(offsetof(CDialogExAccess, m_clrBackground) == 0x12c,
              "g++ reuses CDialog's tail padding for CDialogEx::m_clrBackground (see note above)");

inline S_CDialogRetailTail* Tail(CDialog* p) { return reinterpret_cast<S_CDialogRetailTail*>(p); }

// OpenMFC keeps the indirect template in a side table (InitModalIndirect in this
// file stores it there, DoModal in detail/DlgcoreSupport.cpp reads it from
// there) instead of retail's m_lpDialogTemplate (+0x100), so that table is what
// stands in for the member when a body below reads it.
inline const DLGTEMPLATE* IndirectTemplateOf(CDialog* p) {
    auto it = g_dlgIndirectTemplates.find(p);
    return it != g_dlgIndirectTemplates.end() ? it->second : nullptr;
}

// Retail virtual slots reached from the bodies below, each read off the retail
// call site (byte offset / 8).  They index an MSVC-layout vftable: the objects
// involved are client objects (the application, frames, dialogs a client
// derived and constructed) whose vptr is the client compiler's MSVC table --
// the same assumption core/controlbar/CStatusBar.cpp and
// featurepack/menu/CMFCPopupMenu.cpp make for slot 5.  An object that carries
// g++'s own vtable (e.g. the plain temporary CWnd wrapper FromHandle creates
// for an HWND with no CWnd, detail/WincoreSupport.cpp OpenMfcAttachCWnd) has a
// different slot order: measured under this file's flags (-fdump-lang-class),
// g++ slot 5 of CWnd is CObject::Dump and CCmdTarget::OnCmdMsg is g++ slot 7;
// DispatchOnCmdMsg below routes such objects through the g++ virtual.
// CCmdTarget::OnCmdMsg: `mov 0x28(%rcx),%rax` at 0x207df1 and `mov 0x28(%rax),%rax`
// at 0x207e20, both inside CDialog::OnCmdMsg (entry 0x207d80, mfc140u).
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

// True when p lies inside this DLL's own image -- i.e. a vptr that is one of
// g++'s vtables, not an MSVC client's (the same test
// featurepack/customize/CMFCToolBarsKeyboardPropertyPage.cpp uses).
bool PointsIntoThisImage(const void* p) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* q = static_cast<const unsigned char*>(p);
    return q >= base && q < base + nt->OptionalHeader.SizeOfImage;
}

// pTarget->OnCmdMsg(...) for a target whose vtable may be either kind.  An
// MSVC-vtable (client) object is dispatched through retail slot 5.  A g++-vtable
// object -- the temporary CWnd wrapper FromHandle creates for an HWND with no
// permanent CWnd, or any window OpenMFC itself constructed -- would have
// CObject::Dump() at slot 5 (a no-arg empty function whose leftover RAX would
// be read back as the BOOL), so it is dispatched through g++'s own virtual
// instead: slot 7, CCmdTarget::OnCmdMsg, which walks OpenMFC's g++ message maps.
// (Limitation: OpenMFC's header gives CFrameWnd::OnCmdMsg an AFX_CMDHANDLERINFO*
// parameter, so it is a separate g++ virtual, not an override of slot 7 --
// measured: g++ frame vtables carry both -- and a g++-built frame owner is
// therefore routed without its frame-level view/document hops.)
int DispatchOnCmdMsg(CCmdTarget* pTarget, UINT nID, int nCode, void* pExtra,
                     AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (PointsIntoThisImage(*reinterpret_cast<void* const*>(pTarget))) {
        return pTarget->OnCmdMsg(nID, nCode, pExtra, static_cast<void*>(pHandlerInfo));
    }
    return VSlot<OnCmdMsgFn>(pTarget, kVs_OnCmdMsg)(pTarget, nID, nCode, pExtra, pHandlerInfo);
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
// CDialog::Initialize -- transcribed from RVA 0x207ee0 (mfc140u; ordinal 7695 in
// mfc_complete_ordinal_mapping.json's mfc140u table).  Retail zeroes every
// CDialog member and returns:
//     m_nIDHelp = 0 (+0xe8, dword); m_lpszTemplateName (+0xf0), m_hDialogTemplate,
//     m_lpDialogTemplate, m_lpDialogInit, m_pParentWnd, m_hWndTop (+0x118),
//     m_pOccDialogInfo (+0x128) = NULL; m_bClosedByEndDialog (+0x120, dword) = 0.
// The two members OpenMFC declares are cleared by name, the rest at their
// retail offsets (see the layout note at the top of this file).  Byte for
// byte this clears what retail clears except that, because of the swapped
// declarations, it also clears +0xec..+0xef (high half of OpenMFC's
// m_lpszTemplateName; padding in retail) and does NOT clear +0xf4..+0xf7
// (high half of retail's m_lpszTemplateName; the first bytes of OpenMFC's
// _dialog_padding, which nothing reads).
// OpenMFC additionally drops this dialog's entries from the two side tables
// that stand in for m_lpDialogTemplate and m_pParentWnd (g_dlgIndirectTemplates,
// g_dlgParents, detail/DlgcoreSupport.cpp), so those "members" read back as
// zero through the paths DoModal and CheckAutoCenter actually use.
// The NULL-`this` guard is OpenMFC's; retail has none.
// (The pre-existing `Ordinal: 7705` line below disagrees with that table's
// 7695; its source is not recorded.)
// Symbol: ?Initialize@CDialog@@QEAAXXZ
// Ordinal: 7705
extern "C" void MS_ABI impl__Initialize_CDialog__QEAAXXZ(CDialog* pThis) {
    if (!pThis) return;
    pThis->m_nIDHelp = 0;
    pThis->m_lpszTemplateName = nullptr;
    S_CDialogRetailTail* t = Tail(pThis);
    t->m_hDialogTemplate = nullptr;
    t->m_lpDialogTemplate = nullptr;
    t->m_lpDialogInit = nullptr;
    t->m_pParentWnd = nullptr;
    t->m_hWndTop = nullptr;
    t->m_pOccDialogInfo = nullptr;
    t->m_bClosedByEndDialog = FALSE;
    g_dlgIndirectTemplates.erase(pThis);
    g_dlgParents.erase(pThis);
}
// Symbol: ?CreateIndirect@CDialog@@IEAAHPEAXPEAVCWnd@@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(
    CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd, HINSTANCE hInst) {
    if (!pThis || !lpDialogTemplate) return FALSE;
    if (!hInst) hInst = AfxGetInstanceHandle();
    if (!hInst) hInst = GetModuleHandleW(nullptr);
    HWND hWndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    HWND hDlg = CreateDialogIndirectParamW(
        hInst,
        static_cast<LPCDLGTEMPLATEW>(lpDialogTemplate),
        hWndParent,
        AfxDlgProc,
        reinterpret_cast<LPARAM>(pThis));
    if (!hDlg) return FALSE;
    pThis->m_hWnd = hDlg;
    g_dlgIndirectTemplates.erase(pThis);
    g_dlgMap[hDlg] = pThis;
    return TRUE;
}
// Symbol: ?CreateIndirect@CDialog@@IEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAXPEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateIndirect_CDialog__IEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAXPEAUHINSTANCE_____Z(
    CDialog* pThis, const DLGTEMPLATE* lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit,
    HINSTANCE hInst) {
    (void)lpDialogInit;
    return impl__CreateIndirect_CDialog__IEAAHPEAXPEAVCWnd__PEAUHINSTANCE_____Z(
        pThis, const_cast<DLGTEMPLATE*>(lpDialogTemplate), pParentWnd, hInst);
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
// Symbol: ?GetOccDialogInfo@CDialog@@MEAAPEAU_AFX_OCC_DIALOG_INFO@@XZ
extern "C" _AFX_OCC_DIALOG_INFO* MS_ABI impl__GetOccDialogInfo_CDialog__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(
    CDialog* pThis) {
    return pThis ? pThis->GetOccDialogInfo() : nullptr;
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
// Symbol: ?InitModalIndirect@CDialog@@QEAAHPEAXPEAVCWnd@@@Z
extern "C" int MS_ABI impl__InitModalIndirect_CDialog__QEAAHPEAXPEAVCWnd___Z(
    CDialog* pThis, void* lpDialogTemplate, CWnd* pParentWnd) {
    if (!pThis || !lpDialogTemplate) return FALSE;
    pThis->m_lpszTemplateName = nullptr;
    g_dlgIndirectTemplates[pThis] = static_cast<const DLGTEMPLATE*>(lpDialogTemplate);
    g_dlgParents[pThis] = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    return TRUE;
}
// Symbol: ?InitModalIndirect@CDialog@@QEAAHPEBUDLGTEMPLATE@@PEAVCWnd@@PEAX@Z
extern "C" int MS_ABI impl__InitModalIndirect_CDialog__QEAAHPEBUDLGTEMPLATE__PEAVCWnd__PEAX_Z(
    CDialog* pThis, const DLGTEMPLATE* lpDialogTemplate, CWnd* pParentWnd, void* lpDialogInit) {
    (void)lpDialogInit;
    return impl__InitModalIndirect_CDialog__QEAAHPEAXPEAVCWnd___Z(
        pThis, const_cast<DLGTEMPLATE*>(lpDialogTemplate), pParentWnd);
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
// The base call goes through the CCmdTarget export thunk, which (like this
// export, reached from a client vftable) expects an MSVC-layout `this`.  The
// owner and thread are dispatched by DispatchOnCmdMsg: raw retail slot 5 for
// an MSVC-vtable object, g++'s own virtual for a g++-vtable one (DEVIATION in
// mechanism only -- the retail call is the same virtual OnCmdMsg).  That
// matters for the owner in particular: FromHandle returns a g++ temporary
// CWnd wrapper whenever the parent HWND has no permanent CWnd (a dialog owned
// by a non-MFC window), and slot 5 of that object is CObject::Dump.
// AfxGetThread() (core/app/Globals.cpp export) stands in for the module
// thread state's m_pCurrentWinThread.  The NULL-`this` guard is OpenMFC's.
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
// CDialog::OnEndSession -- NOT implemented; left a no-op.  Retail, RVA 0x208000
// (mfc140u; ordinal 9740), read in the byte-identical mfc140.dll copy at 0x2061b0:
//     if (!bEnding) return;
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;         // +0x8
//     if (pApp != NULL && pApp->m_pMainWnd == this                    // +0x40
//         && (AfxGetThreadState()->m_lastSentMsg.lParam & ENDSESSION_CLOSEAPP)) {  // testb $1,0xb0
//         CDataRecoveryHandler* pHandler = pApp->GetDataRecoveryHandler();   // vslot 65 (+0x208)
//         if (pHandler != NULL)
//             pHandler->SetShutdownByRestartManager(TRUE);                    // vslot 15 (+0x78)
//     }
// (vslot 65 is ?GetDataRecoveryHandler@CWinApp in the mfc140u CWinApp vftable;
// vslot 15 of the CDataRecoveryHandler vftable stores edx to +0x174, the field
// slot 14 returns -- the Get/SetShutdownByRestartManager pair by declaration
// order.)  The whole effect is gated on the lParam of the message being
// processed, which retail reads from the thread state's m_lastSentMsg:
// AfxGetThreadState() is the call at 0x208025 (u RVA 0x1332a0), and +0xb0 is
// m_lastSentMsg (+0x98) .lParam (+0x18) by the _AFX_THREAD_STATE declaration
// in the retail afxstat_.h (offsets computed from it, x64).
// OpenMFC's _AFX_THREAD_STATE (detail/CWinAppSupport.h) is a 0x28-byte struct
// with no m_lastSentMsg, and nothing records the last sent message, so the
// gate cannot be evaluated; with the ENDSESSION_CLOSEAPP bit clear retail also
// does nothing, which is what this body does.
// Symbol: ?OnEndSession@CDialog@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEndSession_CDialog__IEAAXH_Z(CDialog* pThis, BOOL bEnding) {
    (void)pThis;
    (void)bEnding;
}
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
// CDialog::OnQueryEndSession -- NOT implemented; left returning TRUE.  Retail,
// RVA 0x207fa0 (mfc140u; ordinal 10860), read in the byte-identical mfc140.dll
// copy at 0x206150:
//     CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;         // +0x8
//     if (pApp != NULL && pApp->m_pMainWnd == this                    // +0x40
//         && (AfxGetThreadState()->m_lastSentMsg.lParam & ENDSESSION_CLOSEAPP))  // testb $1,0xb0
//         return pApp->SupportsRestartManager();                      // vslot 56 (+0x1c0)
//     return TRUE;
// (vslot 56: by afxwin.h declaration order the first virtual after
// ApplicationRecoveryCallback, which is vslot 55 in the mfc140u CWinApp
// vftable; the slot's own body at RVA 0x1c9890 (mfc140u), not named in the
// symbol map, is `mov 0x14c(%rcx),%eax; and $1,%eax` -- consistent with
// SupportsRestartManager returning m_dwRestartManagerSupportFlags &
// AFX_RESTART_MANAGER_SUPPORT_RESTART.)  As in OnEndSession above, the result
// hinges on the lParam of the message being processed, read from the thread
// state's m_lastSentMsg, which OpenMFC's 0x28-byte _AFX_THREAD_STATE
// (detail/CWinAppSupport.h) does not have and nothing records.  TRUE is what
// retail returns whenever that ENDSESSION_CLOSEAPP bit is clear.
// Symbol: ?OnQueryEndSession@CDialog@@IEAAHXZ
extern "C" int MS_ABI impl__OnQueryEndSession_CDialog__IEAAHXZ(CDialog* pThis) {
    (void)pThis;
    return TRUE;
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
// m_hWndTop is used at its retail offset (inside OpenMFC's _dialog_padding; see
// the layout note at the top of the file) -- the same field
// core/dialog/CPrintDialogEx.cpp's PreModal fills.  OpenMFC's own
// CDialog::PreModal below does not store it, so after that PreModal the field
// is whatever the constructor left: zero for the C++ constructors in this file
// (they run Initialize), but NOT initialised by the constructor exports in
// detail/DlgcoreSupport.cpp (headerRequest).  g_pApp stands in for
// m_pCurrentWinApp.  The NULL-`this` guard is OpenMFC's.
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
// retail behaviour, not a placeholder.
// Symbol: ?PreInitDialog@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__PreInitDialog_CDialog__MEAAXXZ(CDialog* pThis) {
    (void)pThis;
}
// Symbol: ?PreModal@CDialog@@IEAAPEAUHWND__@@XZ
extern "C" HWND MS_ABI impl__PreModal_CDialog__IEAAPEAUHWND____XZ(CDialog* pThis) {
    CWnd* pParent = AfxGetMainWnd();
    HWND hWndParent = pParent ? pParent->m_hWnd : nullptr;
    return (pThis && pThis->m_hWnd) ? pThis->m_hWnd : hWndParent;
}
// CDialog::SetOccDialogInfo -- transcribed from RVA 0x208680 (mfc140u; ordinal
// 13397).  Retail, in full:
//     m_pOccDialogInfo = pOccDialogInfo;   // mov %rdx,0x128(%rcx)
//     return TRUE;
// m_pOccDialogInfo is written at its retail offset (inside OpenMFC's
// _dialog_padding and its tail padding; see the layout note and the
// tail-padding caveat at the top of the file).  NOTE: the
// GetOccDialogInfo export earlier in this file does not read this field back:
// it returns pThis->GetOccDialogInfo(), which in OpenMFC's header is the
// NON-virtual CWnd::GetOccDialogInfo (core/window/CWnd.cpp), reading the
// g_wndRuntimeStates side table.  Retail ?GetOccDialogInfo@CDialog (RVA 0x8760,
// mfc140u) is `mov 0x128(%rcx),%rax; ret`, but switching it to +0x128 must wait
// until the constructor exports in detail/DlgcoreSupport.cpp zero +0x128;
// today they leave it uninitialised, so it would return garbage for every
// client-constructed dialog.  The NULL-`this` guard is OpenMFC's.
// Symbol: ?SetOccDialogInfo@CDialog@@MEAAHPEAU_AFX_OCC_DIALOG_INFO@@@Z
extern "C" int MS_ABI impl__SetOccDialogInfo_CDialog__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(
    CDialog* pThis, _AFX_OCC_DIALOG_INFO* pDialogInfo) {
    if (pThis) {
        Tail(pThis)->m_pOccDialogInfo = pDialogInfo;
    }
    return TRUE;
}
// Symbol: ?OnSetFont@CDialog@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(
    CDialog* pThis, CFont* pFont, int bRedraw) {
    if (!pThis || !pThis->m_hWnd || !pFont) return;
    ::SendMessageW(pThis->m_hWnd, WM_SETFONT, (WPARAM)pFont->GetSafeHandle(), (LPARAM)bRedraw);
}
// Symbol: ?OnSetFont@CDialog@@UEAAXPEAVCFont@@@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__UEAAXPEAVCFont___Z(
    CDialog* pThis, CFont* pFont) {
    impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(pThis, pFont, TRUE);
}
// Symbol: ?DoModal@CDialog@@UEAA_JXZ
intptr_t CDialog::DoModal() {
    return impl__DoModal_CDialog__UEAA_JXZ(this);
}
// Symbol: ?Create@CDialog@@UEAAHPEB_WPEAVCWnd@@@Z
int CDialog::Create(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(this, lpszTemplateName, pParentWnd);
}
// CDialog::OnInitDialog -- NOT implemented; left returning TRUE.
// Two facts first: (1) the EXPORT ?OnInitDialog@CDialog@@UEAAHXZ is aliased to
// impl__OnInitDialog_CDialog__UEAAHXZ, which is defined in
// detail/DlgcoreSupport.cpp (a `return TRUE` body), not here; the C++ method
// below is what OpenMFC's own AfxDlgProc and g++-compiled subclasses reach.
// (2) Retail, RVA 0x208d10 (mfc140u; ordinal 10163), read in the byte-identical
// mfc140.dll copy at 0x206ec0, is:
//     BOOL bDlgInit = m_lpDialogInit != NULL                           // +0x108
//         ? ExecuteDlgInit(m_lpDialogInit)                             // 0x2936b0 (mfc140u)
//         : ExecuteDlgInit(m_lpszTemplateName);                        // 0x2913f0 (mfc140u), +0xf0
//     if (!bDlgInit) { EndDialog(-1); return FALSE; }                  // 0x208b10
//     LoadDynamicLayoutResource(m_lpszTemplateName);                   // 0x2925b0 (mfc140u); result ignored
//     if (!UpdateData(FALSE)) { EndDialog(-1); return FALSE; }         // 0x2910d0 (mfc140u)
//     CWnd* pHelpButton = GetDlgItem(ID_HELP);                         // 0xe146; 0x2a9390
//     if (pHelpButton != NULL)
//         pHelpButton->ShowWindow(AfxHelpEnabled() ? SW_SHOW : SW_HIDE);  // 0x208c70; 5 / 0; 0x2a9ad0
//     return TRUE;
// (Callees named by mfc140u export ordinal: 0x2936b0 = ?ExecuteDlgInit@CWnd@@QEAAHPEAX@Z
// (4437), 0x2913f0 = ?ExecuteDlgInit@CWnd@@QEAAHPEB_W@Z (4438), 0x2925b0 =
// ?LoadDynamicLayoutResource@CWnd@@IEAAHPEB_W@Z (14351), 0x2910d0 =
// ?UpdateData@CWnd@@QEAAHH@Z (14128), 0x2a9ad0 = ?ShowWindow@CWnd@@QEAAHH@Z (13864).)
// Every callee but EndDialog is an OpenMFC body that does not behave like
// retail, and transcribing the call sequence onto them would break dialogs
// that work today:
//   * CWnd::ExecuteDlgInit(LPCWSTR) (core/window/CWnd.cpp) returns FALSE for a
//     NULL name (retail: TRUE, nothing to run) -- every indirect dialog would
//     EndDialog(-1) -- and otherwise SetWindowTextW's the resource NAME onto the
//     dialog, dereferencing MAKEINTRESOURCE ids;
//   * CWnd::LoadDynamicLayoutResource builds a std::wstring from the name, which
//     faults on a MAKEINTRESOURCE id;
//   * CWnd::UpdateData runs no DoDataExchange;
//   * OpenMFC's AfxHelpEnabled tests the help-file path, while retail
//     (0x208c70) returns FALSE with no app, and otherwise asks the current
//     thread's main window (thread vslot 31, GetMainWnd) -- or, failing that,
//     the app -- whether it has an ID_HELP handler (OnCmdMsg slot 5 with a
//     handler-info probe), so Help buttons would be hidden where retail shows
//     them.
// Symbol: ?OnInitDialog@CDialog@@UEAAHXZ
int CDialog::OnInitDialog() {
    return TRUE;
}
// Symbol: ?OnOK@CDialog@@MEAAXXZ
void CDialog::OnOK() {
    EndDialog(IDOK);
}
// Symbol: ?OnCancel@CDialog@@MEAAXXZ
void CDialog::OnCancel() {
    EndDialog(IDCANCEL);
}
// Symbol: ?EndDialog@CDialog@@QEAAXH@Z
void CDialog::EndDialog(int nResult) {
    if (m_hWnd) {
        ::EndDialog(m_hWnd, nResult);
    }
}
// The C++ constructors (reached by g++-compiled subclasses; the exported
// constructors are the thunks in detail/DlgcoreSupport.cpp) zero every CDialog
// member through Initialize, as the retail constructors do inline
// (??0CDialog@@QEAA@XZ, RVA 0x207e50 (mfc140u), stores the same set as
// Initialize); the two that take a parent also store it at the retail
// m_pParentWnd offset, as ??0CDialog@@QEAA@IPEAVCWnd@@@Z (RVA 0x208720
// (mfc140u)) does with `mov %rbx,0x110(%rsi)`.  That +0x110 store is not read
// by anything in OpenMFC yet: DoModal (detail/DlgcoreSupport.cpp) takes the
// parent from g_dlgParents, which these C++ constructors do not fill (they
// never did), so a g++-constructed dialog still gets AfxGetMainWnd() as parent.
CDialog::CDialog() {
    m_hWnd = nullptr;
    impl__Initialize_CDialog__QEAAXXZ(this);
}
CDialog::CDialog(UINT nIDTemplate, CWnd* pParentWnd) {
    m_hWnd = nullptr;
    impl__Initialize_CDialog__QEAAXXZ(this);
    Tail(this)->m_pParentWnd = pParentWnd;
    m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    m_nIDHelp = nIDTemplate;
}
CDialog::CDialog(const wchar_t* lpszTemplateName, CWnd* pParentWnd) {
    m_hWnd = nullptr;
    impl__Initialize_CDialog__QEAAXXZ(this);
    Tail(this)->m_pParentWnd = pParentWnd;
    m_lpszTemplateName = lpszTemplateName;
    // Retail (0x208690): `cmp $0x10000,%rdi; jae` then `movzwl %di,%eax;
    // mov %eax,0xe8(%rsi)` -- a MAKEINTRESOURCE name also becomes the help ID.
    m_nIDHelp = IS_INTRESOURCE(lpszTemplateName)
        ? static_cast<unsigned int>(LOWORD(reinterpret_cast<ULONG_PTR>(lpszTemplateName)))
        : 0u;
}
CDialog::~CDialog() {
    if (m_hWnd) {
        g_dlgMap.erase(m_hWnd);
        m_hWnd = nullptr;
    }
    g_dlgIndirectTemplates.erase(this);
}
int CDialog::Create(UINT nIDTemplate, CWnd* pParentWnd) {
    return impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(this, nIDTemplate, pParentWnd);
}
void CDialog::OnSetFont(CWnd* /* pFont */) {
    // Default: do nothing
}
