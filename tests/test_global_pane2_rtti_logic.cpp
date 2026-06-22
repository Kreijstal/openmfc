// Behavioral test for the docking-pane RTTI getters (global_pane2_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass points at the correct (external) base descriptor.

#include "../phase4/src/global_pane2_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base CRuntimeClass statics referenced by the impl's chains and dragged in by
// afxmfc.h's class graph; defined here only so the standalone test TU links.
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CBasePane::classCBasePane{};
CRuntimeClass CPane::classCPane{};
CRuntimeClass CDockablePane::classCDockablePane{};

static int g_fail = 0;
static void check(bool c, const char* what) {
    std::printf("%s: %s\n", c ? "PASS" : "FAIL", what);
    if (!c) ++g_fail;
}

struct Case {
    CRuntimeClass* (MS_ABI *getThis)();
    CRuntimeClass* (MS_ABI *getRC)(const void*);
    const char*    name;
    int            size;
    CRuntimeClass* base;
};

int main() {
    Case cases[] = {
        { impl__GetThisClass_CPaneContainer__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CPaneContainer__UEBAPEAUCRuntimeClass__XZ,
          "CPaneContainer", 248, &CObject::classCObject },
        { impl__GetThisClass_CPaneContainerManager__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CPaneContainerManager__UEBAPEAUCRuntimeClass__XZ,
          "CPaneContainerManager", 160, &CObject::classCObject },
        { impl__GetThisClass_CPaneDivider__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CPaneDivider__UEBAPEAUCRuntimeClass__XZ,
          "CPaneDivider", 536, &CBasePane::classCBasePane },
        { impl__GetThisClass_CPaneDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CPaneDialog__UEBAPEAUCRuntimeClass__XZ,
          "CPaneDialog", 1264, &CDockablePane::classCDockablePane },
        { impl__GetThisClass_CPaneFrameWnd__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ,
          "CPaneFrameWnd", 600, &CWnd::classCWnd },
    };

    for (const Case& c : cases) {
        CRuntimeClass* rc = c.getThis();
        char buf[112];
        std::snprintf(buf, sizeof(buf), "%s: GetThisClass non-null", c.name);
        check(rc != nullptr, buf);
        if (!rc) continue;
        std::snprintf(buf, sizeof(buf), "%s: class name", c.name);
        check(rc->m_lpszClassName && std::strcmp(rc->m_lpszClassName, c.name) == 0, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_nObjectSize == %d", c.name, c.size);
        check(rc->m_nObjectSize == c.size, buf);
        std::snprintf(buf, sizeof(buf), "%s: schema 0xFFFF (DYNAMIC, no factory)", c.name);
        check(rc->m_wSchema == 0xFFFF && rc->m_pfnCreateObject == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: GetRuntimeClass == GetThisClass", c.name);
        check(c.getRC(nullptr) == rc, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_pBaseClass is the right base", c.name);
        check(rc->m_pBaseClass == c.base, buf);
    }

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
