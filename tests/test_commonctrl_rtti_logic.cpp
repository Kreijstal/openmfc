// Behavioral test for the common-control RTTI getters (ctrl_commonctrl_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass points at the correct base descriptor. The base CRuntimeClass
// statics referenced by the impl are defined here so the TU links standalone.

#include "../phase4/src/ctrl_commonctrl_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base descriptors the impl chains to (defined in the real DLL by
// ctrlcore.cpp / cbarcore.cpp; provided here only for the standalone link so the
// m_pBaseClass pointer-identity checks have a target).
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CComboBox::classCComboBox{};
CRuntimeClass CEdit::classCEdit{};

static int g_fail = 0;
static void check(bool c, const char* what) {
    std::printf("%s: %s\n", c ? "PASS" : "FAIL", what);
    if (!c) ++g_fail;
}

// One class's getters, identified by its impl thunk addresses + expectations.
struct Case {
    CRuntimeClass* (MS_ABI *getThis)();
    CRuntimeClass* (MS_ABI *getRC)(const void*);
    const char*    name;
    int            size;
    CRuntimeClass* base;
};

int main() {
    Case cases[] = {
        { impl__GetThisClass_CAnimateCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CAnimateCtrl__UEBAPEAUCRuntimeClass__XZ, "CAnimateCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CHeaderCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CHeaderCtrl__UEBAPEAUCRuntimeClass__XZ, "CHeaderCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CHotKeyCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CHotKeyCtrl__UEBAPEAUCRuntimeClass__XZ, "CHotKeyCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CIPAddressCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CIPAddressCtrl__UEBAPEAUCRuntimeClass__XZ, "CIPAddressCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ, "CPagerCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CLinkCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CLinkCtrl__UEBAPEAUCRuntimeClass__XZ, "CLinkCtrl", 232, &CWnd::classCWnd },
        { impl__GetThisClass_CComboBoxEx__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CComboBoxEx__UEBAPEAUCRuntimeClass__XZ, "CComboBoxEx", 232, &CComboBox::classCComboBox },
        { impl__GetThisClass_CNetAddressCtrl__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CNetAddressCtrl__UEBAPEAUCRuntimeClass__XZ, "CNetAddressCtrl", 232, &CEdit::classCEdit },
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
