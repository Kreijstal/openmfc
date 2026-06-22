// Behavioral test for the docking-state / tabbed-pane RTTI getters
// (global_docktab_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass points at the correct base descriptor (external members and the
// in-file CBaseTabbedPane), including a CTabbedPane IsDerivedFrom walk.

#include "../phase4/src/global_docktab_rtti.cpp"

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
CRuntimeClass CMiniFrameWnd::classCMiniFrameWnd{};

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
        { impl__GetThisClass_CDockState__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDockState__UEBAPEAUCRuntimeClass__XZ,
          "CDockState", 96, &CObject::classCObject },
        { impl__GetThisClass_CDockingPanesRow__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDockingPanesRow__UEBAPEAUCRuntimeClass__XZ,
          "CDockingPanesRow", 112, &CObject::classCObject },
        { impl__GetThisClass_CDockablePaneAdapter__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDockablePaneAdapter__UEBAPEAUCRuntimeClass__XZ,
          "CDockablePaneAdapter", 1280, &CDockablePane::classCDockablePane },
        { impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CBaseTabbedPane__UEBAPEAUCRuntimeClass__XZ,
          "CBaseTabbedPane", 1304, &CDockablePane::classCDockablePane },
        { impl__GetThisClass_CTabbedPane__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CTabbedPane__UEBAPEAUCRuntimeClass__XZ,
          "CTabbedPane", 1304, impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CMiniDockFrameWnd__UEBAPEAUCRuntimeClass__XZ,
          "CMiniDockFrameWnd", 896, &CMiniFrameWnd::classCMiniFrameWnd },
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

    // CTabbedPane -> CBaseTabbedPane -> CDockablePane: IsDerivedFrom walks the
    // in-file link onto the external CDockablePane member.
    CRuntimeClass* tab = impl__GetThisClass_CTabbedPane__SAPEAUCRuntimeClass__XZ();
    check(tab->IsDerivedFrom(impl__GetThisClass_CBaseTabbedPane__SAPEAUCRuntimeClass__XZ()),
          "CTabbedPane IsDerivedFrom CBaseTabbedPane");
    check(tab->IsDerivedFrom(&CDockablePane::classCDockablePane),
          "CTabbedPane IsDerivedFrom CDockablePane");
    check(!tab->IsDerivedFrom(impl__GetThisClass_CDockState__SAPEAUCRuntimeClass__XZ()),
          "CTabbedPane is NOT derived from CDockState");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
