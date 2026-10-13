// Native thread dispatch order includes virtual GetMainWnd at slot 31.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
extern "C" {
void MS_ABI impl__Delete_CWinThread__UEAAXXZ(CWinThread*);
BOOL MS_ABI impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(CWinThread*,int,MSG*);
LRESULT MS_ABI impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(CWinThread*,CException*,const MSG*);
CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND);
}
CWnd* CWinThread::GetMainWnd() {
    if(m_pActiveWnd)return m_pActiveWnd;
    if(m_pMainWnd)return m_pMainWnd;
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetActiveWindow());
}
void CWinThread::Delete(){impl__Delete_CWinThread__UEAAXXZ(this);}
BOOL CWinThread::ProcessMessageFilter(int code,MSG* message){return impl__ProcessMessageFilter_CWinThread__UEAAHHPEAUtagMSG___Z(this,code,message);}
LRESULT CWinThread::ProcessWndProcException(CException* error,const MSG* message){return impl__ProcessWndProcException_CWinThread__UEAA_JPEAVCException__PEBUtagMSG___Z(this,error,message);}
// Symbol: ?GetMainWnd@CWinThread@@UEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* self){return self->CWinThread::GetMainWnd();}
static_assert(offsetof(CWinThread,m_pMainWnd)==64 && offsetof(CWinThread,m_pActiveWnd)==72,"native main/active window prefix");
