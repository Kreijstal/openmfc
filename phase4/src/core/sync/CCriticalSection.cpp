// CCriticalSection — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ?GetRuntimeClass@CCriticalSection@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_CCriticalSection__UEBAPEAUCRuntimeClass__XZ(
    const CCriticalSection* pThis) {
    return &CCriticalSection::classCCriticalSection;
}
// Symbol: ?GetThisClass@CCriticalSection@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_CCriticalSection__SAPEAUCRuntimeClass__XZ() {
    return &CCriticalSection::classCCriticalSection;
}
CCriticalSection::CCriticalSection() : CSyncObject(nullptr) {
    ::InitializeCriticalSection(&m_sect);
}
CCriticalSection::~CCriticalSection() {
    ::DeleteCriticalSection(&m_sect);
}
BOOL CCriticalSection::Lock() {
    ::EnterCriticalSection(&m_sect);
    return TRUE;
}
BOOL CCriticalSection::Lock(DWORD dwTimeout) {
    if (dwTimeout == INFINITE) {
        ::EnterCriticalSection(&m_sect);
        return TRUE;
    }
    // TryEnterCriticalSection with timeout simulation
    DWORD dwStart = ::GetTickCount();
    while (!::TryEnterCriticalSection(&m_sect)) {
        if (::GetTickCount() - dwStart >= dwTimeout) {
            return FALSE;
        }
        ::Sleep(1);
    }
    return TRUE;
}
BOOL CCriticalSection::Unlock() {
    ::LeaveCriticalSection(&m_sect);
    return TRUE;
}
