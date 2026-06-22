// Behavioral test for the second common-control / view RTTI getters
// (global_commonctrl2_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass points at the correct base descriptor. The base CRuntimeClass
// statics referenced by the impl are defined here so the TU links standalone.

#include "../phase4/src/global_commonctrl2_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base descriptors the impl chains to (defined in the real DLL by the base
// classes' own RTTI; provided here only for the standalone link so the
// m_pBaseClass pointer-identity checks have a target).
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CButton::classCButton{};
CRuntimeClass CView::classCView{};

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
        { impl__GetThisClass_CMonthCalCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CMonthCalCtrl__UEBAPEAUCRuntimeClass__XZ,
          "CMonthCalCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CReBarCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CReBarCtrl__UEBAPEAUCRuntimeClass__XZ,
          "CReBarCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CStatusBarCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CStatusBarCtrl__UEBAPEAUCRuntimeClass__XZ,
          "CStatusBarCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CToolBarCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CToolBarCtrl__UEBAPEAUCRuntimeClass__XZ,
          "CToolBarCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CRichEditCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CRichEditCtrl__UEBAPEAUCRuntimeClass__XZ,
          "CRichEditCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CSplitButton__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CSplitButton__UEBAPEAUCRuntimeClass__XZ,
          "CSplitButton", 248, &CButton::classCButton },
        { impl__GetThisClass_CTabView__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CTabView__UEBAPEAUCRuntimeClass__XZ,
          "CTabView", 15880, &CView::classCView },
    };

    for (const Case& c : cases) {
        CRuntimeClass* rc = c.getThis();
        char buf[96];
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
