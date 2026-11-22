#define OPENMFC_APPCORE_IMPL
#include "detail/RegcoreSupport.h"
#include "detail/WindowMessageSupport.h"
#include "detail/WindowModalSupport.h"

extern "C" BOOL MS_ABI impl__AfxHelpEnabled__YAHXZ();

BOOL CDialog::OnInitDialog() {
    if(!(m_lpDialogInit ? ExecuteDlgInit(m_lpDialogInit) : ExecuteDlgInit(m_lpszTemplateName))) {EndDialog(-1);return FALSE;}
    LoadDynamicLayoutResource(m_lpszTemplateName);
    if(!UpdateData(FALSE)) {EndDialog(-1);return FALSE;}
    HWND help=::GetDlgItem(m_hWnd,0xe146);
    if(help)::ShowWindow(help,impl__AfxHelpEnabled__YAHXZ() ? SW_SHOW : SW_HIDE);
    return TRUE;
}
void CDialog::OnOK(){if(UpdateData(TRUE))EndDialog(IDOK);}
void CDialog::OnCancel(){EndDialog(IDCANCEL);}
void CDialog::EndDialog(int result) {
    m_bClosedByEndDialog=TRUE;
    if(m_nFlags & (openmfc::detail::wfModalLoop | openmfc::detail::wfContinueModal)) {
        using End=void (MS_ABI*)(CWnd*,int);
        openmfc::detail::WindowSlot<End>(this,37)(this,result);
    }
    ::EndDialog(m_hWnd,result);
}
// Symbol: ?OnInitDialog@CDialog@@UEAAHXZ
extern "C" BOOL MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* self){return self->CDialog::OnInitDialog();}
// Symbol: ?OnOK@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* self){self->CDialog::OnOK();}
// Symbol: ?OnCancel@CDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnCancel_CDialog__MEAAXXZ(CDialog* self){self->CDialog::OnCancel();}
// Symbol: ?EndDialog@CDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* self,int result){self->EndDialog(result);}

// Symbol: ?OnSetFont@CDialog@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(CDialog* self,CFont* font,BOOL) {
    using SetFont=void (MS_ABI*)(CDialog*,CFont*);
    openmfc::detail::WindowSlot<SetFont>(self,97)(self,font);self->Default();
}

// Symbol: ?HandleInitDialog@CDialog@@IEAA_J_K_J@Z
extern "C" INT_PTR MS_ABI impl__HandleInitDialog_CDialog__IEAA_J_K_J_Z(CDialog* dialog, WPARAM, LPARAM) {
    if (!dialog) return TRUE;
    using namespace openmfc::detail;
    dialog->PreInitDialog();
    auto* manager = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_pOccManager;
    if (manager && dialog->m_pOccDialogInfo) {
        using CreateControls = BOOL (MS_ABI*)(COccManager*, CWnd*, const void*, _AFX_OCC_DIALOG_INFO*);
        const auto* init = dialog->m_lpDialogInit
            ? dialog->m_lpDialogInit : static_cast<const void*>(dialog->m_lpszTemplateName);
        const auto slot = dialog->m_lpDialogInit ? 8 : 9;
        if (!WindowSlot<CreateControls>(manager, slot)(manager, dialog, init, dialog->m_pOccDialogInfo)) {
            dialog->EndDialog(-1);
            return FALSE;
        }
    }

    const auto result = dialog->Default();
    constexpr UINT oleControlContainer = 0x100;
    if (result && (dialog->m_nFlags & oleControlContainer)) {
        if (dialog->m_pCtrlCont) {
            // COleControlContainer::m_pSiteFocus in the release x64 layout.
            *reinterpret_cast<COleControlSite**>(reinterpret_cast<BYTE*>(dialog->m_pCtrlCont) + 0xd8) = nullptr;
        }
        if (auto* first = CWnd::FromHandle(::GetNextDlgTabItem(dialog->m_hWnd, nullptr, FALSE))) {
            first->SetFocus();
            return FALSE;
        }
    }
    return result;
}

// Symbol: ?OnCommandHelp@CDialog@@IEAA_J_K_J@Z
extern "C" intptr_t MS_ABI impl__OnCommandHelp_CDialog__IEAA_J_K_J_Z(
    CDialog* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    if (lParam == 0) {
        if (pThis == nullptr || pThis->m_nIDHelp == 0) return 0;
        lParam = static_cast<LPARAM>(static_cast<DWORD>(pThis->m_nIDHelp + 0x20000u /* HID_BASE_RESOURCE */));
        if (lParam == 0) return 0;
    }
    CWinApp* pApp = AfxGetApp();
    using WinHelpInternalFn = void (MS_ABI*)(CWinApp*, ULONG_PTR, UINT);
    if (pApp != nullptr) {
        openmfc::detail::WindowSlot<WinHelpInternalFn>(pApp, 52)(
            pApp, static_cast<unsigned long long>(lParam), HELP_CONTEXT);
    }
    return TRUE;
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
    CPaintDC dc(pThis);
    auto* container = pThis->m_pCtrlCont;
    using PaintControls = BOOL (MS_ABI*)(COleControlContainer*, CDC*);
    if (!container || !openmfc::detail::WindowSlot<PaintControls>(container, 40)(container, &dc)) {
        pThis->Default();
    }
}

namespace {
struct DialogMessageHandlers : CDialog {
    LRESULT OnInitMessage(WPARAM w, LPARAM l) {
        return impl__HandleInitDialog_CDialog__IEAA_J_K_J_Z(this, w, l);
    }
    LRESULT OnCommandHelp(WPARAM w, LPARAM l) {
        return impl__OnCommandHelp_CDialog__IEAA_J_K_J_Z(this, w, l);
    }
    LRESULT OnHelpHitTest(WPARAM w, LPARAM l) {
        return impl__OnHelpHitTest_CDialog__IEAA_J_K_J_Z(this, w, l);
    }
    void OnFont(CFont* font, BOOL redraw) {
        impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(this, font, redraw);
    }
    void OnPaint() { impl__OnPaint_CDialog__IEAAXXZ(this); }
    BOOL OnQueryEndSession() { return impl__OnQueryEndSession_CDialog__IEAAHXZ(this); }
    void OnEndSession(UINT ending) { impl__OnEndSession_CDialog__IEAAXH_Z(this, ending); }
};
}

BEGIN_MESSAGE_MAP(CDialog, CWnd)
    ON_COMMAND(IDOK, &CDialog::OnOK)
    ON_COMMAND(IDCANCEL, &CDialog::OnCancel)
    ON_MESSAGE(0x365, &DialogMessageHandlers::OnCommandHelp)
    ON_MESSAGE(0x366, &DialogMessageHandlers::OnHelpHitTest)
    ON_MESSAGE(WM_INITDIALOG, &DialogMessageHandlers::OnInitMessage)
    OPENMFC_MAP_ENTRY(WM_SETFONT, 0, 0, 0, AfxSig_v_F_b, &DialogMessageHandlers::OnFont)
    OPENMFC_MAP_ENTRY(WM_PAINT, 0, 0, 0, AfxSig_v_v_v, &DialogMessageHandlers::OnPaint)
    OPENMFC_MAP_ENTRY(WM_QUERYENDSESSION, 0, 0, 0, AfxSig_b_v_v, &DialogMessageHandlers::OnQueryEndSession)
    OPENMFC_MAP_ENTRY(WM_ENDSESSION, 0, 0, 0, AfxSig_v_u_v, &DialogMessageHandlers::OnEndSession)
END_MESSAGE_MAP()

// Symbol: ?GetMessageMap@CDialog@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CDialog__MEBAPEBUAFX_MSGMAP__XZ(const CDialog*) {
    return CDialog::GetThisMessageMap();
}

// Symbol: ?GetThisMessageMap@CDialog@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() {
    return CDialog::GetThisMessageMap();
}
