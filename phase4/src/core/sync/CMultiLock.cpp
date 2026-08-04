// CMultiLock — OpenMFC implementation.
// Sources: synccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/SynccoreSupport.h"

// Symbol: ??0CMultiLock@@QEAA@QEAPEAVCSyncObject@@KH@Z
extern "C" void* MS_ABI
impl___0CMultiLock__QEAA_QEAPEAVCSyncObject__KH_Z(
    void* pThis, CSyncObject** p0, unsigned long p1, int p2) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CMultiLock(p0, p1, p2);
}
CMultiLock::CMultiLock(CSyncObject* ppObjects[], DWORD dwCount, BOOL bInitialLock)
    : m_dwCount(dwCount) {
    m_pHandleArray = new HANDLE[dwCount];
    m_bLockedArray = new BOOL[dwCount];
    m_ppObjectArray = new CSyncObject*[dwCount];

    for (DWORD i = 0; i < dwCount; i++) {
        m_ppObjectArray[i] = ppObjects[i];
        m_pHandleArray[i] = ppObjects[i]->m_hObject;
        m_bLockedArray[i] = FALSE;
    }

    if (bInitialLock) {
        Lock();
    }
}
CMultiLock::~CMultiLock() {
    Unlock();
    delete[] m_pHandleArray;
    delete[] m_bLockedArray;
    delete[] m_ppObjectArray;
}
DWORD CMultiLock::Lock(DWORD dwTimeOut, BOOL bWaitForAll, DWORD dwWakeMask) {
    DWORD dwResult;

    if (dwWakeMask != 0) {
        dwResult = ::MsgWaitForMultipleObjects(m_dwCount, m_pHandleArray,
                                                bWaitForAll, dwTimeOut, dwWakeMask);
    } else {
        dwResult = ::WaitForMultipleObjects(m_dwCount, m_pHandleArray,
                                            bWaitForAll, dwTimeOut);
    }

    if (dwResult >= WAIT_OBJECT_0 && dwResult < WAIT_OBJECT_0 + m_dwCount) {
        if (bWaitForAll) {
            for (DWORD i = 0; i < m_dwCount; i++) {
                m_bLockedArray[i] = TRUE;
            }
        } else {
            m_bLockedArray[dwResult - WAIT_OBJECT_0] = TRUE;
        }
    } else if (dwResult >= WAIT_ABANDONED_0 && dwResult < WAIT_ABANDONED_0 + m_dwCount) {
        if (bWaitForAll) {
            for (DWORD i = 0; i < m_dwCount; i++) {
                m_bLockedArray[i] = TRUE;
            }
        } else {
            m_bLockedArray[dwResult - WAIT_ABANDONED_0] = TRUE;
        }
    }

    return dwResult;
}
BOOL CMultiLock::Unlock() {
    for (DWORD i = 0; i < m_dwCount; i++) {
        if (m_bLockedArray[i]) {
            m_ppObjectArray[i]->Unlock();
            m_bLockedArray[i] = FALSE;
        }
    }
    return TRUE;
}
BOOL CMultiLock::Unlock(LONG lCount, LPLONG lpPrevCount) {
    for (DWORD i = 0; i < m_dwCount; i++) {
        if (m_bLockedArray[i]) {
            m_ppObjectArray[i]->Unlock(lCount, lpPrevCount);
            m_bLockedArray[i] = FALSE;
        }
    }
    return TRUE;
}
BOOL CMultiLock::IsLocked(DWORD dwItem) const {
    if (dwItem < m_dwCount) {
        return m_bLockedArray[dwItem];
    }
    return FALSE;
}
