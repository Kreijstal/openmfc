// Native release x64 CWinThread construction and owned-handle teardown.
#define OPENMFC_APPCORE_IMPL
#include "detail/ModuleThreadStateSupport.h"
#include <new>

CWinThread::CWinThread() : m_pThreadParams(nullptr),m_pfnThreadProc(nullptr) {CommonConstruct();}
CWinThread::CWinThread(unsigned int (AFXAPI* callback)(void*),void* argument)
    {m_pfnThreadProc=callback;m_pThreadParams=argument;CommonConstruct();}
void CWinThread::CommonConstruct() {
    m_pMainWnd=nullptr;m_pActiveWnd=nullptr;m_hThread=nullptr;m_nThreadID=0;
    auto* state=AfxGetThreadState();
    state->m_msgCur.message=0;state->m_nMsgLast=0;
    ::GetCursorPos(reinterpret_cast<POINT*>(&state->m_ptCursorLast));
    m_bAutoDelete=TRUE;m_pMessageFilter=nullptr;m_lpfnOleTermOrFreeLib=nullptr;
}
CWinThread::~CWinThread() {
    if(m_hThread)::CloseHandle(m_hThread);
    auto* state=impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ();
    if(state->m_pCurrentWinThread==this)state->m_pCurrentWinThread=nullptr;
}
// Symbol: ??0CWinThread@@QEAA@XZ
extern "C" CWinThread* MS_ABI impl___0CWinThread__QEAA_XZ(CWinThread* self) {return ::new(self) CWinThread;}
// Symbol: ??0CWinThread@@QEAA@P6AIPEAX@Z0@Z
extern "C" CWinThread* MS_ABI impl___0CWinThread__QEAA_P6AIPEAX_Z0_Z(
    CWinThread* self,unsigned int (AFXAPI* callback)(void*),void* argument) {return ::new(self) CWinThread(callback,argument);}
// Symbol: ??1CWinThread@@UEAA@XZ
extern "C" void MS_ABI impl___1CWinThread__UEAA_XZ(CWinThread* self) {self->CWinThread::~CWinThread();}
// Symbol: ?CommonConstruct@CWinThread@@QEAAXXZ
extern "C" void MS_ABI impl__CommonConstruct_CWinThread__QEAAXXZ(CWinThread* self) {self->CommonConstruct();}
// Symbol: ?Delete@CWinThread@@UEAAXXZ
extern "C" void MS_ABI impl__Delete_CWinThread__UEAAXXZ(CWinThread* self) {
    if(self->m_bAutoDelete) {
        using Delete=void* (MS_ABI*)(void*,unsigned);
        reinterpret_cast<Delete>((*reinterpret_cast<void***>(self))[1])(self,1);
    }
}
static_assert(sizeof(CWinThread)==0x88,"native CWinThread storage");
static_assert(offsetof(CWinThread,m_pThreadParams)==0x68 && offsetof(CWinThread,m_pfnThreadProc)==0x70 &&
    offsetof(CWinThread,m_lpfnOleTermOrFreeLib)==0x78 && offsetof(CWinThread,m_pMessageFilter)==0x80,"native worker services");
static_assert(sizeof(CWinApp)==0x178 && offsetof(CWinApp,m_pszAppName)==0xa0 &&
    offsetof(CWinApp,m_pszRegistryKey)==0xb0 && offsetof(CWinApp,m_hInstance)==0x88,"native application data prefix");
