#include "SynccoreSupport.h"

IMPLEMENT_DYNAMIC(CSyncObject, CObject)
IMPLEMENT_DYNAMIC(CCriticalSection, CSyncObject)
IMPLEMENT_DYNAMIC(CMutex, CSyncObject)
IMPLEMENT_DYNAMIC(CSemaphore, CSyncObject)
IMPLEMENT_DYNAMIC(CEvent, CSyncObject)
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
namespace openmfc { namespace detail { namespace synccore {
extern "C" void* MS_ABI impl___0CSyncObject__QEAA_PEB_W_Z(void* pThis, const wchar_t* pstrName) {
    if (!pThis) {
        return nullptr;
    }
    return new(pThis) CSyncObject(pstrName);
}
unsigned int __stdcall AfxThreadEntry(void* pParam) {
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
} } }  // namespace openmfc::detail::synccore
