// synccore.cpp - Synchronization classes implementation
//
// This file implements:
// - CSyncObject (base class)
// - CCriticalSection
// - CMutex
// - CSemaphore
// - CEvent
// - CSingleLock
// - CMultiLock
// - AfxBeginThread / AfxEndThread

// Skip inline implementations from afxwin.h (defined in appcore.cpp)
#define OPENMFC_APPCORE_IMPL

#include <openmfc/afxwin.h>
#include <windows.h>
#include <process.h>

// MS ABI for x64 Windows
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

//=============================================================================
// Runtime class implementations
//=============================================================================

IMPLEMENT_DYNAMIC(CSyncObject, CObject)
IMPLEMENT_DYNAMIC(CCriticalSection, CSyncObject)
IMPLEMENT_DYNAMIC(CMutex, CSyncObject)
IMPLEMENT_DYNAMIC(CSemaphore, CSyncObject)
IMPLEMENT_DYNAMIC(CEvent, CSyncObject)

//=============================================================================
// CSyncObject implementation
//=============================================================================

CSyncObject::CSyncObject(const wchar_t* pstrName)
    : m_hObject(nullptr) {
    if (pstrName != nullptr) {
        m_strName = pstrName;
    }
}

CSyncObject::~CSyncObject() {
    if (m_hObject != nullptr) {
        ::CloseHandle(m_hObject);
        m_hObject = nullptr;
    }
}

BOOL CSyncObject::Lock(DWORD dwTimeout) {
    if (m_hObject == nullptr) {
        return FALSE;
    }
    DWORD dwRet = ::WaitForSingleObject(m_hObject, dwTimeout);
    return (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_ABANDONED);
}

BOOL CSyncObject::Unlock(LONG /* lCount */, LPLONG /* lpPrevCount */) {
    // Default implementation - derived classes override
    return TRUE;
}

//=============================================================================
// CCriticalSection implementation
//=============================================================================

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

//=============================================================================
// CMutex implementation
//=============================================================================

CMutex::CMutex(BOOL bInitiallyOwn, const wchar_t* lpszName,
               LPSECURITY_ATTRIBUTES lpsaAttribute)
    : CSyncObject(lpszName) {
    m_hObject = ::CreateMutexW(lpsaAttribute, bInitiallyOwn, lpszName);
}

CMutex::~CMutex() {
    // Base class destructor closes handle
}

BOOL CMutex::Unlock() {
    return ::ReleaseMutex(m_hObject);
}

//=============================================================================
// CSemaphore implementation
//=============================================================================

CSemaphore::CSemaphore(LONG lInitialCount, LONG lMaxCount,
                       const wchar_t* pstrName,
                       LPSECURITY_ATTRIBUTES lpsaAttributes)
    : CSyncObject(pstrName) {
    m_hObject = ::CreateSemaphoreW(lpsaAttributes, lInitialCount, lMaxCount, pstrName);
}

CSemaphore::~CSemaphore() {
    // Base class destructor closes handle
}

BOOL CSemaphore::Unlock() {
    return Unlock(1, nullptr);
}

BOOL CSemaphore::Unlock(LONG lCount, LPLONG lpPrevCount) {
    return ::ReleaseSemaphore(m_hObject, lCount, lpPrevCount);
}

//=============================================================================
// CEvent implementation
//=============================================================================

CEvent::CEvent(BOOL bInitiallyOwn, BOOL bManualReset,
               const wchar_t* lpszName,
               LPSECURITY_ATTRIBUTES lpsaAttribute)
    : CSyncObject(lpszName) {
    m_hObject = ::CreateEventW(lpsaAttribute, bManualReset, bInitiallyOwn, lpszName);
}

CEvent::~CEvent() {
    // Base class destructor closes handle
}

BOOL CEvent::SetEvent() {
    return ::SetEvent(m_hObject);
}

BOOL CEvent::PulseEvent() {
    return ::PulseEvent(m_hObject);
}

BOOL CEvent::ResetEvent() {
    return ::ResetEvent(m_hObject);
}

BOOL CEvent::Unlock() {
    return TRUE;  // Events don't need unlock
}

//=============================================================================
// CSingleLock implementation
//=============================================================================

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

//=============================================================================
// CMultiLock implementation
//=============================================================================

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

//=============================================================================
// Thread functions
//=============================================================================

// Thread-local storage for current CWinThread (defined in appcore.cpp)
extern __thread CWinThread* g_pCurrentThread;

// Internal thread start routine
struct AfxThreadParams {
    AFX_THREADPROC pfnThreadProc;
    void* pParam;
    CWinThread* pThread;
};

static unsigned int __stdcall AfxThreadEntry(void* pParam) {
    AfxThreadParams* pParams = static_cast<AfxThreadParams*>(pParam);

    g_pCurrentThread = pParams->pThread;

    UINT nResult = 0;
    if (pParams->pfnThreadProc != nullptr) {
        nResult = pParams->pfnThreadProc(pParams->pParam);
    } else if (pParams->pThread != nullptr) {
        // For CWinThread-derived classes, call InitInstance/Run/ExitInstance
        if (pParams->pThread->InitInstance()) {
            nResult = pParams->pThread->Run();
        } else {
            nResult = pParams->pThread->ExitInstance();
        }
    }

    // Clean up
    CWinThread* pThread = pParams->pThread;
    delete pParams;

    if (pThread != nullptr && pThread->m_bAutoDelete) {
        delete pThread;
    }

    return nResult;
}

CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, void* pParam,
                           int nPriority, UINT nStackSize,
                           DWORD dwCreateFlags,
                           LPSECURITY_ATTRIBUTES lpSecurityAttrs) {
    CWinThread* pThread = new CWinThread();
    pThread->m_bAutoDelete = TRUE;

    AfxThreadParams* pParams = new AfxThreadParams;
    pParams->pfnThreadProc = pfnThreadProc;
    pParams->pParam = pParam;
    pParams->pThread = pThread;

    unsigned int nThreadID;
    pThread->m_hThread = (HANDLE)_beginthreadex(
        lpSecurityAttrs,
        nStackSize,
        AfxThreadEntry,
        pParams,
        dwCreateFlags | CREATE_SUSPENDED,
        &nThreadID
    );

    if (pThread->m_hThread == nullptr) {
        delete pParams;
        delete pThread;
        return nullptr;
    }

    pThread->m_nThreadID = nThreadID;

    if (nPriority != 0) {
        ::SetThreadPriority(pThread->m_hThread, nPriority);
    }

    if (!(dwCreateFlags & CREATE_SUSPENDED)) {
        ::ResumeThread(pThread->m_hThread);
    }

    return pThread;
}

CWinThread* AfxBeginThread(CRuntimeClass* pThreadClass,
                           int nPriority, UINT nStackSize,
                           DWORD dwCreateFlags,
                           LPSECURITY_ATTRIBUTES lpSecurityAttrs) {
    if (pThreadClass == nullptr) {
        return nullptr;
    }

    CObject* pObject = pThreadClass->CreateObject();
    if (pObject == nullptr) {
        return nullptr;
    }

    CWinThread* pThread = dynamic_cast<CWinThread*>(pObject);
    if (pThread == nullptr) {
        delete pObject;
        return nullptr;
    }

    pThread->m_bAutoDelete = TRUE;

    AfxThreadParams* pParams = new AfxThreadParams;
    pParams->pfnThreadProc = nullptr;
    pParams->pParam = nullptr;
    pParams->pThread = pThread;

    unsigned int nThreadID;
    pThread->m_hThread = (HANDLE)_beginthreadex(
        lpSecurityAttrs,
        nStackSize,
        AfxThreadEntry,
        pParams,
        dwCreateFlags | CREATE_SUSPENDED,
        &nThreadID
    );

    if (pThread->m_hThread == nullptr) {
        delete pParams;
        delete pThread;
        return nullptr;
    }

    pThread->m_nThreadID = nThreadID;

    if (nPriority != 0) {
        ::SetThreadPriority(pThread->m_hThread, nPriority);
    }

    if (!(dwCreateFlags & CREATE_SUSPENDED)) {
        ::ResumeThread(pThread->m_hThread);
    }

    return pThread;
}

void AfxEndThread(UINT nExitCode, BOOL bDelete) {
    CWinThread* pThread = g_pCurrentThread;

    if (pThread != nullptr) {
        pThread->ExitInstance();
        if (bDelete && pThread->m_bAutoDelete) {
            delete pThread;
        }
    }

    g_pCurrentThread = nullptr;
    _endthreadex(nExitCode);
}

// Note: AfxGetThread() is implemented in appcore.cpp
