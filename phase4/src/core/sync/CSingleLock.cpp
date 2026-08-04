// CSingleLock — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ??0CSingleLock@@QEAA@PEAVCSyncObject@@H@Z
extern "C" void* MS_ABI
impl___0CSingleLock__QEAA_PEAVCSyncObject__H_Z(
    void* pThis, CSyncObject* p0, int p1) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CSingleLock(p0, p1);
}
CSingleLock::CSingleLock(CSyncObject* pObject, BOOL bInitialLock)
    : m_pObject(pObject), m_hObject(nullptr), m_bAcquired(FALSE) {
    if (m_pObject != nullptr) {
        m_hObject = m_pObject->m_hObject;
    }
    if (bInitialLock) {
        Lock();
    }
}
CSingleLock::~CSingleLock() {
    if (m_bAcquired) {
        Unlock();
    }
}
BOOL CSingleLock::Lock(DWORD dwTimeOut) {
    if (m_pObject == nullptr) {
        return FALSE;
    }
    m_bAcquired = m_pObject->Lock(dwTimeOut);
    return m_bAcquired;
}
BOOL CSingleLock::Unlock() {
    if (m_pObject == nullptr || !m_bAcquired) {
        return FALSE;
    }
    BOOL bResult = m_pObject->Unlock();
    if (bResult) {
        m_bAcquired = FALSE;
    }
    return bResult;
}
BOOL CSingleLock::Unlock(LONG lCount, LPLONG lpPrevCount) {
    if (m_pObject == nullptr || !m_bAcquired) {
        return FALSE;
    }
    BOOL bResult = m_pObject->Unlock(lCount, lpPrevCount);
    if (bResult) {
        m_bAcquired = FALSE;
    }
    return bResult;
}
