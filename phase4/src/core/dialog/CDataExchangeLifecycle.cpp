// Native 24-byte CDataExchange tracking and failure behavior.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <new>
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
extern "C" {
void MS_ABI impl__AfxThrowNotSupportedException__YAXXZ();
void MS_ABI impl__AfxThrowUserException__YAXXZ();
}
namespace {
COleControlSite* FindSite(COleControlContainer* container,UINT id) {
    if(!container)return nullptr;
    auto* table=*reinterpret_cast<void***>(container);
    using Find=COleControlSite* (MS_ABI*)(const void*,UINT);
    return reinterpret_cast<Find>(table[23])(container,id);
}
}
CDataExchange::CDataExchange(CWnd* window,BOOL save) : m_bSaveAndValidate(save),m_pDlgWnd(window),m_idLastControl(0),m_bEditLastControl(FALSE) {}
HWND CDataExchange::PrepareCtrl(int id) {
    HWND window=::GetDlgItem(m_pDlgWnd->m_hWnd,id);
    if(!window && !FindSite(m_pDlgWnd->m_pCtrlCont,static_cast<UINT>(id))) {
        impl__AfxThrowNotSupportedException__YAXXZ();return nullptr;
    }
    m_idLastControl=static_cast<UINT>(id);m_bEditLastControl=FALSE;return window;
}
HWND CDataExchange::PrepareEditCtrl(int id) { HWND window=PrepareCtrl(id);m_bEditLastControl=TRUE;return window; }
COleControlSite* CDataExchange::PrepareOleCtrl(int id) {
    auto* site=FindSite(m_pDlgWnd->m_pCtrlCont,static_cast<UINT>(id));
    if(!site) { impl__AfxThrowNotSupportedException__YAXXZ();return nullptr; }
    m_idLastControl=static_cast<UINT>(id);m_bEditLastControl=FALSE;return site;
}
void CDataExchange::Fail() {
    if(m_bSaveAndValidate && m_idLastControl) {
        HWND control=::GetDlgItem(m_pDlgWnd->m_hWnd,m_idLastControl);
        if(control) {
            ::SetFocus(control);
            if(m_bEditLastControl)::SendMessageW(control,EM_SETSEL,0,-1);
        }
    }
    impl__AfxThrowUserException__YAXXZ();
}
// Symbol: ??0CDataExchange@@QEAA@PEAVCWnd@@H@Z
extern "C" void* MS_ABI impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(void* storage,void* window,BOOL save) { return new(storage) CDataExchange(static_cast<CWnd*>(window),save); }
// Symbol: ?PrepareCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
extern "C" HWND MS_ABI impl__PrepareCtrl_CDataExchange__QEAAPEAUHWND____H_Z(CDataExchange* self,int id) { return self->PrepareCtrl(id); }
// Symbol: ?PrepareEditCtrl@CDataExchange@@QEAAPEAUHWND__@@H@Z
extern "C" HWND MS_ABI impl__PrepareEditCtrl_CDataExchange__QEAAPEAUHWND____H_Z(CDataExchange* self,int id) { return self->PrepareEditCtrl(id); }
// Symbol: ?Fail@CDataExchange@@QEAAXXZ
extern "C" void MS_ABI impl__Fail_CDataExchange__QEAAXXZ(CDataExchange* self) { self->Fail(); }
// Symbol: ?PrepareOleCtrl@CDataExchange@@QEAAPEAVCOleControlSite@@H@Z
extern "C" COleControlSite* MS_ABI impl__PrepareOleCtrl_CDataExchange__QEAAPEAVCOleControlSite__H_Z(CDataExchange* self,int id) { return self->PrepareOleCtrl(id); }
