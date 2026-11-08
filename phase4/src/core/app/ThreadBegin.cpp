// Native public worker/UI creation keeps the initialized child suspended while
// the caller applies its priority, then resumes unless suspension was requested.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
extern "C" {
void* AFXAPI impl___2_YAPEAX_K_Z(size_t);
CObject* AFXAPI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass*);
void AFXAPI impl__AfxThrowMemoryException__YAXXZ();
void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
}
CWinThread* AfxBeginThread(AFX_THREADPROC function,void* argument,int priority,UINT stack,
    DWORD flags,SECURITY_ATTRIBUTES* security) {
    auto* storage=impl___2_YAPEAX_K_Z(sizeof(CWinThread));
    auto* thread=storage ? ::new(storage) CWinThread(function,argument) : nullptr;
    if(!thread->CreateThread(flags|CREATE_SUSPENDED,stack,security)) {thread->Delete();return nullptr;}
    ::SetThreadPriority(thread->m_hThread,priority);
    if(!(flags&CREATE_SUSPENDED))::ResumeThread(thread->m_hThread);
    return thread;
}
CWinThread* AfxBeginThread(CRuntimeClass* type,int priority,UINT stack,DWORD flags,SECURITY_ATTRIBUTES* security) {
    auto* thread=static_cast<CWinThread*>(impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(type));
    if(!thread)impl__AfxThrowMemoryException__YAXXZ();
    thread->m_pThreadParams=nullptr;
    if(!thread->CreateThread(flags|CREATE_SUSPENDED,stack,security)) {thread->Delete();return nullptr;}
    ::SetThreadPriority(thread->m_hThread,priority);
    if(!(flags&CREATE_SUSPENDED) && ::ResumeThread(thread->m_hThread)==DWORD(-1))
        impl__AfxThrowInvalidArgException__YAXXZ();
    return thread;
}
// Symbol: ?AfxBeginThread@@YAPEAVCWinThread@@P6AIPEAX@Z0HIKPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" CWinThread* AFXAPI impl__AfxBeginThread__YAPEAVCWinThread__P6AIPEAX_Z0HIKPEAU_SECURITY_ATTRIBUTES___Z(
    AFX_THREADPROC function,void* argument,int priority,UINT stack,DWORD flags,SECURITY_ATTRIBUTES* security) {
    return AfxBeginThread(function,argument,priority,stack,flags,security);
}
// Symbol: ?AfxBeginThread@@YAPEAVCWinThread@@PEAUCRuntimeClass@@HIKPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" CWinThread* AFXAPI impl__AfxBeginThread__YAPEAVCWinThread__PEAUCRuntimeClass__HIKPEAU_SECURITY_ATTRIBUTES___Z(
    CRuntimeClass* type,int priority,UINT stack,DWORD flags,SECURITY_ATTRIBUTES* security) {
    return AfxBeginThread(type,priority,stack,flags,security);
}
