// CDialog — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"
#include "detail/WindowMessageSupport.h"

#include <cstddef>

// ---- sibling impl__ exports called by the bodies below ----------------------
// Each matches a definition that exists today; the defining file is named.
extern "C" int MS_ABI impl__OnCmdMsg_CCmdTarget__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CCmdTarget* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo); // core/runtime/CCmdTarget.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);     // core/window/CWnd.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();            // core/app/Globals.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);                            // core/runtime/Globals.cpp
extern "C" int MS_ABI impl__AfxUnhookWindowCreate__YAHXZ();
extern "C" void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(CWnd*);
extern "C" HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND, HWND*);
extern "C" HWND__* MS_ABI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd* pThis);        // core/window/Thunks.cpp
extern "C" void MS_ABI impl__DoEnableModeless_CWinApp__SAXH_Z(int bEnable);          // core/app/CWinApp.cpp

namespace {

// Release x64 CDialog members used by the dialog resource and modal paths.
static_assert(sizeof(CDialog) == 0x130);
static_assert(offsetof(CDialog, m_nIDHelp) == 0xe8);
static_assert(offsetof(CDialog, m_lpszTemplateName) == 0xf0);
static_assert(offsetof(CDialog, m_hDialogTemplate) == 0xf8);
static_assert(offsetof(CDialog, m_lpDialogTemplate) == 0x100);
static_assert(offsetof(CDialog, m_lpDialogInit) == 0x108);
static_assert(offsetof(CDialog, m_pParentWnd) == 0x110);
static_assert(offsetof(CDialog, m_hWndTop) == 0x118);
static_assert(offsetof(CDialog, m_bClosedByEndDialog) == 0x120);
static_assert(offsetof(CDialog, m_pOccDialogInfo) == 0x128);
static_assert(offsetof(CDialogExAccess, m_clrBackground) == 0x130);

const DLGTEMPLATE* IndirectTemplateOf(CDialog* dialog) {
    return dialog->m_hDialogTemplate
        ? static_cast<const DLGTEMPLATE*>(::LockResource(dialog->m_hDialogTemplate))
        : dialog->m_lpDialogTemplate;
}

const DLGTEMPLATE* LoadDialogTemplate(const wchar_t* name, HINSTANCE& module) {
    module = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, RT_DIALOG);
    const auto resource = ::FindResourceW(module, name, RT_DIALOG);
    const auto data = resource ? ::LoadResource(module, resource) : nullptr;
    return data ? static_cast<const DLGTEMPLATE*>(::LockResource(data)) : nullptr;
}

// Native command routing uses CCmdTarget virtual slot 5.
constexpr std::size_t kVs_OnCmdMsg = 5;
using OnCmdMsgFn = int (MS_ABI*)(void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);

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
    return self->CreateDlgIndirect(data, parent ? parent : AfxGetMainWnd(), module);
}
}
// Symbol: ?Create@CDialog@@UEAAHPEB_WPEAVCWnd@@@Z
extern "C" BOOL MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
    CDialog* self, const wchar_t* name, CWnd* parent) {
    if (!self || !name) return FALSE;
    self->m_lpszTemplateName = name;
    if (!self->m_nIDHelp && IS_INTRESOURCE(name)) {
        self->m_nIDHelp = LOWORD(reinterpret_cast<UINT_PTR>(name));
    }
    HINSTANCE module;
    const auto* data = LoadDialogTemplate(name, module);
    if (!data) return FALSE;
    self->m_lpDialogInit = nullptr;
    return CreateIndirectWindow(self, data, parent, module);
}

// Symbol: ?Create@CDialog@@UEAAHIPEAVCWnd@@@Z
extern "C" BOOL MS_ABI impl__Create_CDialog__UEAAHIPEAVCWnd___Z(
    CDialog* self, UINT id, CWnd* parent) {
    return impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(self, MAKEINTRESOURCEW(id), parent);
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
// Symbol: ?GetRuntimeClass@CDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDialog__UEBAPEAUCRuntimeClass__XZ(
    const CDialog* pThis) {
    return CDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ() {
    return CDialog::GetThisClass();
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
    if (::IsWindow(pThis->m_hWndTop)) {
        ::EnableWindow(pThis->m_hWndTop, TRUE);
    }
    pThis->m_hWndTop = nullptr;
    if (AfxGetApp() != nullptr) {
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
    if (!pThis) return nullptr;
    if (AfxGetApp()) impl__DoEnableModeless_CWinApp__SAXH_Z(FALSE);
    const HWND parent = pThis->m_pParentWnd ? pThis->m_pParentWnd->GetSafeHwnd() : nullptr;
    const HWND owner = impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(
        parent, &pThis->m_hWndTop);
    impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(pThis);
    return owner;
}

// Symbol: ?DoModal@CDialog@@UEAA_JXZ
extern "C" INT_PTR MS_ABI impl__DoModal_CDialog__UEAA_JXZ(CDialog* dialog) {
    if (!dialog || dialog->m_hWnd) return -1;

    HINSTANCE module = AfxGetResourceHandle();
    const DLGTEMPLATE* data = dialog->m_lpszTemplateName
        ? LoadDialogTemplate(dialog->m_lpszTemplateName, module) : IndirectTemplateOf(dialog);
    if (!data) return -1;

    HWND owner = nullptr;
    CWnd* disabledFrame = nullptr;
    bool restoreOwner = false;
    auto finish = [&] {
        if (disabledFrame) disabledFrame->EnableWindow(TRUE);
        if (restoreOwner) ::EnableWindow(owner, TRUE);
        if (owner && ::GetActiveWindow() == dialog->m_hWnd) ::SetActiveWindow(owner);
        dialog->DestroyWindow();
        impl__PostModal_CDialog__IEAAXXZ(dialog);
    };

    try {
        owner = impl__PreModal_CDialog__IEAAPEAUHWND____XZ(dialog);
        impl__AfxUnhookWindowCreate__YAHXZ();
        if (owner && owner != ::GetDesktopWindow() && ::IsWindowEnabled(owner)) {
            ::EnableWindow(owner, FALSE);
            restoreOwner = true;
            CWnd* frame = AfxGetMainWnd();
            if (frame && frame->IsFrameWnd() && frame->IsWindowEnabled()) {
                frame->EnableWindow(FALSE);
                disabledFrame = frame;
            }
        }

        auto* parent = CWnd::FromHandle(owner);
        impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(dialog);
        if (!dialog->CreateRunDlgIndirect(data, parent, module) && !dialog->m_bClosedByEndDialog) {
            dialog->CreateRunDlgIndirect(data, CWnd::FromHandle(owner), AfxGetInstanceHandle());
        }
    } catch (CException* error) {
        if (error) error->Delete();
    } catch (...) {
        dialog->m_bClosedByEndDialog = FALSE;
        finish();
        throw;
    }
    dialog->m_bClosedByEndDialog = FALSE;
    finish();
    return dialog->m_nModalResult;
}
