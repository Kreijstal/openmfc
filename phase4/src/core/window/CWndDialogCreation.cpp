#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"
#include "detail/WindowCreationSupport.h"
#include "detail/WindowActivationSupport.h"
#include "detail/WindowModalSupport.h"

extern "C" {
HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t*, const wchar_t*);
void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(CWnd*);
BOOL MS_ABI impl__AfxUnhookWindowCreate__YAHXZ();
}

// Symbol: ?AfxDlgProc@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" INT_PTR CALLBACK impl__AfxDlgProc__YA_JPEAUHWND____I_K_J_Z(
    HWND window, UINT message, WPARAM, LPARAM) {
    if (message != WM_INITDIALOG) return FALSE;
    auto* wrapper = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
    if (wrapper && wrapper->IsKindOf(CDialog::GetThisClass())) {
        return static_cast<CDialog*>(wrapper)->OnInitDialog();
    }
    return TRUE;
}

namespace {
const DLGTEMPLATE* PrepareOleTemplate(COccManager* manager, _AFX_OCC_DIALOG_INFO* info,
                                    const DLGTEMPLATE* dialog) {
    using Prepare = const DLGTEMPLATE* (MS_ABI*)(COccManager*, _AFX_OCC_DIALOG_INFO*, const DLGTEMPLATE*);
    return openmfc::detail::WindowSlot<Prepare>(manager, 5)(manager, info, dialog);
}

void FinishOleTemplate(COccManager* manager, _AFX_OCC_DIALOG_INFO* info, CWnd* window) {
    using Finish = void (MS_ABI*)(COccManager*, _AFX_OCC_DIALOG_INFO*);
    openmfc::detail::WindowSlot<Finish>(manager, 6)(manager, info);
    window->SetOccDialogInfo(nullptr);
}
}

int CWnd::CreateDlg(const wchar_t* name, void* parent) {
    if (!name || m_hWnd) return FALSE;
    const HINSTANCE module = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(name, RT_DIALOG);
    const HRSRC resource = ::FindResourceW(module, name, RT_DIALOG);
    if (!resource) return FALSE;
    const HGLOBAL data = ::LoadResource(module, resource);
    const auto* dialog = data ? static_cast<const DLGTEMPLATE*>(::LockResource(data)) : nullptr;
    return dialog && CreateDlgIndirect(dialog, parent, module);
}

int CWnd::CreateDlgIndirect(const DLGTEMPLATE* dialog, void* parent, HINSTANCE module) {
    if (!dialog || m_hWnd) return FALSE;
    auto* state = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if (!module) module = state->m_hCurrentResourceHandle;
    auto* owner = static_cast<CWnd*>(parent);
    COccManager* manager = state->m_pOccManager;

    INITCOMMONCONTROLSEX controls{sizeof(controls), ICC_WIN95_CLASSES | ICC_USEREX_CLASSES |
        ICC_DATE_CLASSES | ICC_COOL_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS |
        ICC_NATIVEFNTCTL_CLASS};
    ::InitCommonControlsEx(&controls);

    _AFX_OCC_DIALOG_INFO info{};
    if (manager && !SetOccDialogInfo(&info)) return FALSE;

    HWND window = nullptr;
    bool attached = false;
    try {
        if (manager) dialog = PrepareOleTemplate(manager, &info, dialog);
        if (dialog) {
            m_nModalResult = -1;
            m_nFlags |= openmfc::detail::wfContinueModal;
            impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(this);
            window = openmfc::detail::ContextCreateDialog(
                module, dialog, owner ? owner->m_hWnd : nullptr,
                impl__AfxDlgProc__YA_JPEAUHWND____I_K_J_Z);
            attached = impl__AfxUnhookWindowCreate__YAHXZ() != FALSE;
        }
    } catch (...) {
        if (AfxGetThreadState()->m_pWndInit == this) impl__AfxUnhookWindowCreate__YAHXZ();
        if (manager) FinishOleTemplate(manager, &info, this);
        throw;
    }
    if (manager) FinishOleTemplate(manager, &info, this);
    if (!dialog) return FALSE;

    if (!attached) {
        if (window) ::DestroyWindow(window);
        PostNcDestroy();
        return FALSE;
    }
    if (window && (m_nFlags & openmfc::detail::wfContinueModal) == 0) {
        ::DestroyWindow(window);
        return FALSE;
    }
    return window != nullptr;
}

int CWnd::CreateRunDlgIndirect(const DLGTEMPLATE* dialog, void* parent, HINSTANCE module) {
    using namespace openmfc::detail;
    if (!CreateDlgIndirect(dialog, parent, module)) return FALSE;

    if (m_nFlags & wfContinueModal) {
        DWORD flags = mlfShowOnIdle;
        if (GetStyle() & DS_NOIDLEMSG) flags |= mlfNoIdleMessage;
        RunModalLoop(flags);
    }
    if (m_hWnd) {
        SetWindowPos(nullptr, 0, 0, 0, 0,
                     SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
    }
    return TRUE;
}

// Symbol: ?CreateDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* window, const DLGTEMPLATE* dialog, CWnd* parent, HINSTANCE module) {
    return window && window->CreateDlgIndirect(dialog, parent, module);
}

// Symbol: ?CreateRunDlgIndirect@CWnd@@IEAAHPEBUDLGTEMPLATE@@PEAV1@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__CreateRunDlgIndirect_CWnd__IEAAHPEBUDLGTEMPLATE__PEAV1_PEAUHINSTANCE_____Z(
    CWnd* window, const DLGTEMPLATE* dialog, CWnd* parent, HINSTANCE module) {
    return window && window->CreateRunDlgIndirect(dialog, parent, module);
}
