// Behavioral test for the four parameterless MFC exception RTTI descriptors.
//
// We #include the impl .cpp directly so we can call the impl_ thunks without
// linking the DLL. The impl's CRuntimeClass descriptors are file-scope (in an
// anonymous namespace), so we drive them only through the extern "C" thunks.

#include <cstdio>
#include <cstring>

// Pull in the impl under test (defines the extern "C" impl_ thunks + descriptors).
#include "../phase4/src/global_simple_exceptions_rtti.cpp"

// The impl references &CException::classCException, whose real definition lives in
// mfc_exceptions.cpp (not linked here). afxwin.h's inline code also drags in a few
// other DECLARE_DYNAMIC descriptors (CCmdTarget/CWinThread/CWinApp/CWnd). Provide
// standalone definitions so this test links without the DLL. CException's descriptor
// is chained to CObject::classCObject (header-inline) so the base-chain assertions
// exercise the real linkage relationship the impl depends on.
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

// Thunk prototypes (extern "C", MS_ABI matches the impl).
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(const void*);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(const void*);

static int g_failures = 0;

static void check(bool cond, const char* what) {
    if (cond) {
        printf("PASS: %s\n", what);
    } else {
        printf("FAIL: %s\n", what);
        ++g_failures;
    }
}

// Walk base chain via the same logic real MFC uses (prefer m_pfnGetBaseClass).
static bool chain_reaches(const CRuntimeClass* start, const CRuntimeClass* target) {
    const CRuntimeClass* p = start;
    while (p != nullptr) {
        if (p == target) return true;
        p = p->m_pfnGetBaseClass ? p->m_pfnGetBaseClass() : p->m_pBaseClass;
    }
    return false;
}

static void test_one(const char* expectedName,
                     CRuntimeClass* viaThis,
                     CRuntimeClass* viaRuntime) {
    char buf[128];

    snprintf(buf, sizeof(buf), "%s: GetThisClass non-null", expectedName);
    check(viaThis != nullptr, buf);
    if (!viaThis) return;

    snprintf(buf, sizeof(buf), "%s: name == \"%s\"", expectedName, expectedName);
    check(viaThis->m_lpszClassName != nullptr &&
          strcmp(viaThis->m_lpszClassName, expectedName) == 0, buf);

    snprintf(buf, sizeof(buf), "%s: m_nObjectSize == 16", expectedName);
    check(viaThis->m_nObjectSize == 16, buf);

    snprintf(buf, sizeof(buf), "%s: m_wSchema == 0xFFFF", expectedName);
    check(viaThis->m_wSchema == 0xFFFFu, buf);

    snprintf(buf, sizeof(buf), "%s: not DYNCREATE (no factory)", expectedName);
    check(viaThis->m_pfnCreateObject == nullptr, buf);

    snprintf(buf, sizeof(buf), "%s: m_pBaseClass == &CException::classCException", expectedName);
    check(viaThis->m_pBaseClass == &CException::classCException, buf);

    snprintf(buf, sizeof(buf), "%s: base chain reaches CException", expectedName);
    check(chain_reaches(viaThis, &CException::classCException), buf);

    snprintf(buf, sizeof(buf), "%s: base chain reaches CObject", expectedName);
    check(chain_reaches(viaThis, &CObject::classCObject), buf);

    snprintf(buf, sizeof(buf), "%s: GetRuntimeClass == GetThisClass", expectedName);
    check(viaRuntime == viaThis, buf);
}

int main() {
    // Sanity: the four descriptors must be distinct objects.
    CRuntimeClass* a = impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* b = impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* c = impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ();
    CRuntimeClass* d = impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ();
    check(a != b && a != c && a != d && b != c && b != d && c != d,
          "all four descriptors are distinct");

    // pThis is unused by the impl, but pass a real-ish pointer anyway.
    int dummy = 0;
    const void* pThis = &dummy;

    test_one("CInvalidArgException", a,
             impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(pThis));
    test_one("CNotSupportedException", b,
             impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(pThis));
    test_one("CResourceException", c,
             impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(pThis));
    test_one("CUserException", d,
             impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(pThis));

    if (g_failures == 0) {
        printf("RESULT: ALL PASS\n");
        return 0;
    }
    printf("RESULT: %d FAILURE(S)\n", g_failures);
    return 1;
}
