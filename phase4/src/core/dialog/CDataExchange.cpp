// CDataExchange — OpenMFC implementation.
// Sources: global_cdataexchange.cpp

#include "detail/CDataExchangeSupport.h"

// Symbol: ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z
// CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate)
extern "C" void* MS_ABI
impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(void* pThis, void* pDlgWnd, BOOL bSaveAndValidate)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    self->m_bSaveAndValidate = bSaveAndValidate;
    self->m_pDlgWnd = pDlgWnd;
    self->m_idLastControl = 0;
    self->m_bEditLastControl = FALSE;
    return pThis;
}
// Symbol: ?PrepareCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
// HWND PrepareCtrl(int nIDC)
// Resolve the control window from the dialog, record it as the last-touched
// (non-edit) control, and hand back its HWND for the DDX routine.
extern "C" HWND MS_ABI
impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), nIDC);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = FALSE;  // not an edit item unless PrepareEditCtrl
    return hWndCtrl;
}
// Symbol: ?PrepareEditCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
// HWND PrepareEditCtrl(int nIDC)
// Same as PrepareCtrl but flags the control as an edit item so that a later
// Fail() knows to select all its text.
extern "C" HWND MS_ABI
impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), nIDC);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = TRUE;
    return hWndCtrl;
}
// Symbol: ?PrepareOleCtrl@CDataExchange@@QEAAPEAVCOleControlSite@@H@Z
// COleControlSite* PrepareOleCtrl(int nIDC)
// Record the control id (non-edit) exactly like PrepareCtrl. The real routine
// then looks the id up in the dialog's OLE control container site map; when the
// dialog hosts no OLE control container (the common case) MFC returns NULL, so
// that is the faithful result here. The bookkeeping side-effects are real.
extern "C" void* MS_ABI
impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(void* pThis, int nIDC)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    self->m_idLastControl = static_cast<UINT>(nIDC);
    self->m_bEditLastControl = FALSE;
    return nullptr;
}
// Symbol: ?Fail@CDataExchange@@QEAAXXZ
// void Fail()
// Restore focus to the last control involved in the exchange (selecting all of
// its text if it was an edit control) and then abort the DDX pass by throwing.
extern "C" void MS_ABI
impl__Fail_CDataExchange__QEAAXXZ(void* pThis)
{
    S_Cdataexchange* self = reinterpret_cast<S_Cdataexchange*>(pThis);
    if (self->m_idLastControl != 0) {
        HWND hWndCtrl = ::GetDlgItem(DlgHwnd(self), self->m_idLastControl);
        if (hWndCtrl != nullptr) {
            ::SetFocus(hWndCtrl);
            if (self->m_bEditLastControl) {
                // Select the entire contents of the offending edit control.
                ::SendMessageW(hWndCtrl, EM_SETSEL, 0, static_cast<LPARAM>(-1));
            }
        }
    }
    // AfxThrowUserException(): never returns normally.
    impl__AfxThrowUserException__YAXXZ();
}
