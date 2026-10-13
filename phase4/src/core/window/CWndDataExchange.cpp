#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxstat_.h"
#include "detail/WindowMessageSupport.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" int MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT,UINT,UINT);
namespace {
struct NotificationLockout {
    _AFX_THREAD_STATE* state;
    HWND previous;
    explicit NotificationLockout(HWND window) : state(AfxGetThreadState()),previous(state->m_hLockoutNotifyWindow) { state->m_hLockoutNotifyWindow=window; }
    ~NotificationLockout() { state->m_hLockoutNotifyWindow=previous; }
};
}
int CWnd::UpdateData(int save) {
    CDataExchange exchange(this,save);NotificationLockout lockout(m_hWnd);
    try {
        using Exchange=void (MS_ABI*)(void*,CDataExchange*);
        openmfc::detail::WindowSlot<Exchange>(this,66)(this,&exchange);
        return TRUE;
    } catch(CUserException* error) {
        error->Delete();return FALSE;
    } catch(CException* error) {
        impl__AfxMessageBox__YAHIII_Z(0xf108,MB_ICONSTOP,0);
        error->Delete();return FALSE;
    }
}
// Symbol: ?UpdateData@CWnd@@QEAAHH@Z
extern "C" int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* self,int save) { return self->UpdateData(save); }
