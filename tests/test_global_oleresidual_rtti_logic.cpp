// Behavioral test for the OLE/HTML residual RTTI getters
// (global_oleresidual_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size / real m_wSchema (DYNAMIC/DYNCREATE 0xFFFF;
// the two SERIAL classes carry real versions), that GetRuntimeClass returns the
// same static descriptor as GetThisClass (no self-dispatch recursion), the null
// m_pfnGetBaseClass/m_pNextClass shape, and that m_pBaseClass chains to the right
// base, including a CMultiPageDHtmlDialog IsDerivedFrom walk onto CDialog.

#include "../phase4/src/global_oleresidual_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base CRuntimeClass statics referenced by the impl's chains and dragged in by
// afxwin.h's class graph; defined here only so the standalone test TU links.
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CDialog::classCDialog{};

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
    unsigned       schema;
    CRuntimeClass* base;
};

int main() {
    Case cases[] = {
        { impl__GetThisClass_CDHtmlDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDHtmlDialog__UEBAPEAUCRuntimeClass__XZ,
          "CDHtmlDialog", 664, 0xFFFF, &CDialog::classCDialog },
        { impl__GetThisClass_CMultiPageDHtmlDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CMultiPageDHtmlDialog__UEBAPEAUCRuntimeClass__XZ,
          "CMultiPageDHtmlDialog", 672, 0xFFFF, impl__GetThisClass_CDHtmlDialog__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_CDocObjectServer__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDocObjectServer__UEBAPEAUCRuntimeClass__XZ,
          "CDocObjectServer", 144, 0xFFFF, &CCmdTarget::classCCmdTarget },
        { impl__GetThisClass_CDynLinkLibrary__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDynLinkLibrary__UEBAPEAUCRuntimeClass__XZ,
          "CDynLinkLibrary", 128, 0xFFFF, &CCmdTarget::classCCmdTarget },
        { impl__GetThisClass_COleControlModule__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleControlModule__UEBAPEAUCRuntimeClass__XZ,
          "COleControlModule", 376, 0xFFFF, &CWinApp::classCWinApp },
        { impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CSettingsStore__UEBAPEAUCRuntimeClass__XZ,
          "CSettingsStore", 56, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CMouseManager__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CMouseManager__UEBAPEAUCRuntimeClass__XZ,
          "CMouseManager", 176, 0x00000001, &CObject::classCObject },
        { impl__GetThisClass_CUserTool__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CUserTool__UEBAPEAUCRuntimeClass__XZ,
          "CUserTool", 56, 0x80000001, &CObject::classCObject },
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
        std::snprintf(buf, sizeof(buf), "%s: schema 0x%08X, no factory", c.name, c.schema);
        check(rc->m_wSchema == c.schema && rc->m_pfnCreateObject == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_pfnGetBaseClass and m_pNextClass null", c.name);
        check(rc->m_pfnGetBaseClass == nullptr && rc->m_pNextClass == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: GetRuntimeClass == GetThisClass", c.name);
        check(c.getRC(nullptr) == rc, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_pBaseClass chains correctly", c.name);
        check(rc->m_pBaseClass == c.base, buf);
    }

    // CMultiPageDHtmlDialog -> CDHtmlDialog -> CDialog: IsDerivedFrom walks the
    // in-file link onto the external CDialog member.
    CRuntimeClass* mp = impl__GetThisClass_CMultiPageDHtmlDialog__SAPEAUCRuntimeClass__XZ();
    check(mp->IsDerivedFrom(impl__GetThisClass_CDHtmlDialog__SAPEAUCRuntimeClass__XZ()),
          "CMultiPageDHtmlDialog IsDerivedFrom CDHtmlDialog");
    check(mp->IsDerivedFrom(&CDialog::classCDialog),
          "CMultiPageDHtmlDialog IsDerivedFrom CDialog");
    check(!mp->IsDerivedFrom(impl__GetThisClass_CDocObjectServer__SAPEAUCRuntimeClass__XZ()),
          "CMultiPageDHtmlDialog is NOT derived from CDocObjectServer");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
