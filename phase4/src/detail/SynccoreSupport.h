#pragma once
// Shared internals of the former synccore.cpp translation unit.
// Definitions live in detail/SynccoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace synccore {} } }
using namespace openmfc::detail::synccore;
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












//=============================================================================
// CSyncObject implementation
//=============================================================================


namespace openmfc { namespace detail { namespace synccore {
extern "C" void* MS_ABI impl___0CSyncObject__QEAA_PEB_W_Z(void* pThis, const wchar_t* pstrName);
} } }





//=============================================================================
// CCriticalSection implementation
//=============================================================================






//=============================================================================
// CMutex implementation
//=============================================================================





//=============================================================================
// CSemaphore implementation
//=============================================================================






//=============================================================================
// CEvent implementation
//=============================================================================








//=============================================================================
// CSingleLock implementation
//=============================================================================







//=============================================================================
// CMultiLock implementation
//=============================================================================








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

namespace openmfc { namespace detail { namespace synccore {
unsigned int __stdcall AfxThreadEntry(void* pParam);
} } }

namespace openmfc { namespace detail { namespace synccore {
CWinThread* AfxBeginThread(AFX_THREADPROC pfnThreadProc, void* pParam,
                           int nPriority, UINT nStackSize,
                           DWORD dwCreateFlags,
                           LPSECURITY_ATTRIBUTES lpSecurityAttrs);
} } }

namespace openmfc { namespace detail { namespace synccore {
CWinThread* AfxBeginThread(CRuntimeClass* pThreadClass,
                           int nPriority, UINT nStackSize,
                           DWORD dwCreateFlags,
                           LPSECURITY_ATTRIBUTES lpSecurityAttrs);
} } }

void AfxEndThread(UINT nExitCode, BOOL bDelete);

// Note: AfxGetThread() is implemented in appcore.cpp
