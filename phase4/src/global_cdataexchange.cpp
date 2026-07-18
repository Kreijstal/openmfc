// OpenMFC real implementation of CDataExchange.
//
// CDataExchange is the non-polymorphic context object threaded through every
// DDX_/DDV_ routine during CWnd::UpdateData. It owns no vtable; it simply
// tracks the dialog window, the save/validate direction, and bookkeeping for
// the last control touched (used by Fail() to restore focus/selection before
// aborting the exchange).
//
// Layout (cl.exe /d1reportSingleClassLayout) — class CDataExchange size(24):
//    0 | BOOL  m_bSaveAndValidate
//    4 | <alignment padding>
//    8 | CWnd* m_pDlgWnd
//   16 | UINT  m_idLastControl
//   20 | BOOL  m_bEditLastControl
//
// Exports (all aliased in the .def to the impl_ symbols below):
//   ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z            (ctor)
//   ?Fail@CDataExchange@@QEAAXXZ
//   ?PrepareCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
//   ?PrepareEditCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
//   ?PrepareOleCtrl@CDataExchange@@QEAAPEAVCOleControlSite@@H@Z

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view of CDataExchange (size 24).
struct S {
    BOOL  m_bSaveAndValidate;  // 0
    char  _pad0[4];            // 4  (alignment before the pointer)
    void* m_pDlgWnd;           // 8  (CWnd*)
    UINT  m_idLastControl;     // 16
    BOOL  m_bEditLastControl;  // 20
};

static_assert(sizeof(S) == 24, "CDataExchange must be 24 bytes");
static_assert(offsetof(S, m_bSaveAndValidate) == 0, "m_bSaveAndValidate offset");
static_assert(offsetof(S, m_pDlgWnd) == 8, "m_pDlgWnd offset");
static_assert(offsetof(S, m_idLastControl) == 16, "m_idLastControl offset");
static_assert(offsetof(S, m_bEditLastControl) == 20, "m_bEditLastControl offset");

// CWnd::m_hWnd lives at offset 64 in the MFC x64 layout. Recover the raw
// window handle of the dialog container so we can drive the real ::GetDlgItem /
// focus APIs exactly as MFC does internally.
inline HWND DlgHwnd(const S* self)
{
    if (self->m_pDlgWnd == nullptr)
        return nullptr;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(self->m_pDlgWnd) + 64);
}

// Placeholder for MFC's CUserException. The real DLL calls
// AfxThrowUserException() (throw new CUserException) to unwind out of the DDX
// pass; that exception type / hierarchy is not available self-contained here,
// so Fail() throws this local marker to preserve the non-returning, stack-
// unwinding contract.
struct UserExceptionAbort {};

} // namespace

// Symbol: ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z
// CDataExchange(CWnd* pDlgWnd, BOOL bSaveAndValidate)
extern "C" void* MS_ABI
impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(void* pThis, void* pDlgWnd, BOOL bSaveAndValidate)
{
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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
    S* self = reinterpret_cast<S*>(pThis);
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
    throw UserExceptionAbort();
}
