// Behavioral test for CLongBinary RTTI getters.
// Verifies the descriptor identity, fields, and CObject base chain. Includes the
// real afxdao.h CLongBinary to static_assert the descriptor's size literal.

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdao.h"
#include <cstdio>
#include <cstring>

// afxwin.h inline code drags in these DECLARE_DYNAMIC descriptors; supply them.
CRuntimeClass CCmdTarget::classCCmdTarget={"CCmdTarget",sizeof(CCmdTarget),0xFFFF,nullptr,nullptr,&CObject::classCObject,nullptr};
CRuntimeClass CWinThread::classCWinThread={"CWinThread",sizeof(CWinThread),0xFFFF,nullptr,nullptr,&CCmdTarget::classCCmdTarget,nullptr};
CRuntimeClass CWinApp::classCWinApp={"CWinApp",sizeof(CWinApp),0xFFFF,nullptr,nullptr,&CWinThread::classCWinThread,nullptr};
CRuntimeClass CWnd::classCWnd={"CWnd",sizeof(CWnd),0xFFFF,nullptr,nullptr,&CCmdTarget::classCCmdTarget,nullptr};

#include "../phase4/src/global_clongbinary_rtti.cpp"

static_assert(sizeof(CLongBinary) == 24, "descriptor size literal (24) must match real CLongBinary");

extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(const void*);

static int g_failures = 0;
static void check(bool cond, const char* what) {
    std::printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

int main() {
    CRuntimeClass* rc = impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ();
    check(rc != nullptr, "GetThisClass non-null");
    check(std::strcmp(rc->m_lpszClassName, "CLongBinary") == 0, "class name is CLongBinary");
    check(rc->m_nObjectSize == (int)sizeof(CLongBinary), "m_nObjectSize == sizeof(CLongBinary)");
    check(rc->m_wSchema == 0xFFFF, "schema 0xFFFF (not serializable)");
    check(rc->m_pfnCreateObject == nullptr, "no DYNCREATE factory");
    check(rc->m_pBaseClass == &CObject::classCObject, "base chains to CObject");

    int dummy = 0;
    CRuntimeClass* rrc = impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(&dummy);
    check(rrc == rc, "GetRuntimeClass == GetThisClass");

    // IsKindOf-style walk: CLongBinary derives from CObject, not vice versa.
    bool reachesObject = false;
    for (const CRuntimeClass* p = rc; p; p = p->m_pBaseClass)
        if (p == &CObject::classCObject) { reachesObject = true; break; }
    check(reachesObject, "base chain reaches CObject");

    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
