// Behavioral test for COleException / COleDispatchException RTTI descriptors.
//
// We #include the impl .cpp directly so we drive the extern "C" impl_ thunks
// without linking the DLL. The descriptors are file-scope (anonymous namespace);
// we reach them only through the thunks, exactly as a caller would.

#include <cstdio>
#include <cstring>

// Pull in the impl under test (defines the thunks + the two descriptors).
#include "../phase4/src/ole_oleexception_rtti.cpp"

// The impl references &CException::classCException, defined in mfc_exceptions.cpp
// (not linked here). afxwin.h inline code also drags in a few other
// DECLARE_DYNAMIC descriptors; provide standalone definitions so the test links.
// CException chains to CObject::classCObject (header-inline) so the base-chain
// assertions exercise the real linkage the impl depends on.
CRuntimeClass CException::classCException = {
    "CException", sizeof(CException), 0xFFFF, nullptr, nullptr,
    &CObject::classCObject, nullptr
};
CRuntimeClass CCmdTarget::classCCmdTarget = {
    "CCmdTarget", sizeof(CCmdTarget), 0xFFFF, nullptr, nullptr,
    &CObject::classCObject, nullptr
};
CRuntimeClass CWinThread::classCWinThread = {
    "CWinThread", sizeof(CWinThread), 0xFFFF, nullptr, nullptr,
    &CCmdTarget::classCCmdTarget, nullptr
};
CRuntimeClass CWinApp::classCWinApp = {
    "CWinApp", sizeof(CWinApp), 0xFFFF, nullptr, nullptr,
    &CWinThread::classCWinThread, nullptr
};
CRuntimeClass CWnd::classCWnd = {
    "CWnd", sizeof(CWnd), 0xFFFF, nullptr, nullptr,
    &CCmdTarget::classCCmdTarget, nullptr
};

// The m_nObjectSize literals baked into the descriptors must equal the repo's own
// class sizes; pin them so the descriptor can't silently drift from the headers.
static_assert(sizeof(COleException) > 0, "COleException complete");
static_assert(sizeof(COleDispatchException) > 0, "COleDispatchException complete");

// Thunk prototypes (extern "C", MS_ABI matches the impl).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(const void*);

static int g_failures = 0;
static void check(bool cond, const char* what) {
    printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

static bool chain_reaches(const CRuntimeClass* start, const CRuntimeClass* target) {
    for (const CRuntimeClass* p = start; p; p = p->m_pfnGetBaseClass ? p->m_pfnGetBaseClass() : p->m_pBaseClass)
        if (p == target) return true;
    return false;
}

int main() {
    int dummy = 0;
    const void* pThis = &dummy;

    CRuntimeClass* oe   = impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* oeR  = impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(pThis);
    CRuntimeClass* ode  = impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* odeR = impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(pThis);

    // --- COleException ---
    check(oe != nullptr, "COleException GetThisClass non-null");
    check(oe->m_lpszClassName && strcmp(oe->m_lpszClassName, "COleException") == 0,
          "COleException name");
    check(oe->m_nObjectSize == (int)sizeof(COleException), "COleException m_nObjectSize == sizeof");
    check(oe->m_wSchema == 0xFFFFu, "COleException schema 0xFFFF");
    check(oe->m_pfnCreateObject == nullptr, "COleException not DYNCREATE");
    check(oe->m_pBaseClass == &CException::classCException, "COleException base == CException");
    check(chain_reaches(oe, &CException::classCException), "COleException chain reaches CException");
    check(chain_reaches(oe, &CObject::classCObject), "COleException chain reaches CObject");
    check(oeR == oe, "COleException GetRuntimeClass == GetThisClass");

    // --- COleDispatchException ---
    check(ode != nullptr, "COleDispatchException GetThisClass non-null");
    check(ode->m_lpszClassName && strcmp(ode->m_lpszClassName, "COleDispatchException") == 0,
          "COleDispatchException name");
    check(ode->m_nObjectSize == (int)sizeof(COleDispatchException),
          "COleDispatchException m_nObjectSize == sizeof");
    check(ode->m_wSchema == 0xFFFFu, "COleDispatchException schema 0xFFFF");
    check(ode->m_pfnCreateObject == nullptr, "COleDispatchException not DYNCREATE");
    check(ode->m_pBaseClass == oe, "COleDispatchException base == COleException");
    check(chain_reaches(ode, oe), "COleDispatchException chain reaches COleException");
    check(chain_reaches(ode, &CException::classCException), "COleDispatchException chain reaches CException");
    check(chain_reaches(ode, &CObject::classCObject), "COleDispatchException chain reaches CObject");
    check(odeR == ode, "COleDispatchException GetRuntimeClass == GetThisClass");

    // The two descriptors must be distinct objects.
    check(oe != ode, "COleException and COleDispatchException are distinct descriptors");

    printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
