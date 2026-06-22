// Behavioral test for the OLE in-place frame / OLE dialog RTTI getters
// (global_oleframe_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size / schema (all DECLARE_DYNAMIC or
// DECLARE_DYNCREATE, so 0xFFFF, no factory), that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass equals the descriptor the base class's own GetThisClass returns
// (COleDialog / CFrameWnd members plus the in-file COleIPFrameWnd /
// COleDocIPFrameWnd chains), including a COleDocIPFrameWndEx IsDerivedFrom walk.

#include "../phase4/src/global_oleframe_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base CRuntimeClass statics referenced by the impl's chains and dragged in by
// afxole.h's class graph; defined here only so the standalone test TU links.
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CFrameWnd::classCFrameWnd{};
CRuntimeClass CDialog::classCDialog{};
CRuntimeClass COleDialog::classCOleDialog{};
CRuntimeClass CDocument::classCDocument{};

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
        { impl__GetThisClass_COleInsertDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleInsertDialog__UEBAPEAUCRuntimeClass__XZ,
          "COleInsertDialog", 1008, &COleDialog::classCOleDialog },
        { impl__GetThisClass_COleLinksDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleLinksDialog__UEBAPEAUCRuntimeClass__XZ,
          "COleLinksDialog", 432, &COleDialog::classCOleDialog },
        { impl__GetThisClass_COlePasteSpecialDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COlePasteSpecialDialog__UEBAPEAUCRuntimeClass__XZ,
          "COlePasteSpecialDialog", 480, &COleDialog::classCOleDialog },
        { impl__GetThisClass_COlePropertiesDialog__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COlePropertiesDialog__UEBAPEAUCRuntimeClass__XZ,
          "COlePropertiesDialog", 720, &COleDialog::classCOleDialog },
        { impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleIPFrameWnd__UEBAPEAUCRuntimeClass__XZ,
          "COleIPFrameWnd", 640, &CFrameWnd::classCFrameWnd },
        { impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleIPFrameWndEx__UEBAPEAUCRuntimeClass__XZ,
          "COleIPFrameWndEx", 1936, impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleDocIPFrameWnd__UEBAPEAUCRuntimeClass__XZ,
          "COleDocIPFrameWnd", 640, impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleDocIPFrameWndEx__UEBAPEAUCRuntimeClass__XZ,
          "COleDocIPFrameWndEx", 1936, impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ() },
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
        std::snprintf(buf, sizeof(buf), "%s: schema 0xFFFF, no factory", c.name);
        check(rc->m_wSchema == 0xFFFF && rc->m_pfnCreateObject == nullptr, buf);
        std::snprintf(buf, sizeof(buf), "%s: GetRuntimeClass == GetThisClass", c.name);
        check(c.getRC(nullptr) == rc, buf);
        std::snprintf(buf, sizeof(buf), "%s: m_pBaseClass chains correctly", c.name);
        check(rc->m_pBaseClass == c.base, buf);
    }

    // COleDocIPFrameWndEx -> COleDocIPFrameWnd -> COleIPFrameWnd -> CFrameWnd:
    // IsDerivedFrom walks the in-file links onto the external CFrameWnd member.
    CRuntimeClass* ex = impl__GetThisClass_COleDocIPFrameWndEx__SAPEAUCRuntimeClass__XZ();
    check(ex->IsDerivedFrom(impl__GetThisClass_COleDocIPFrameWnd__SAPEAUCRuntimeClass__XZ()),
          "COleDocIPFrameWndEx IsDerivedFrom COleDocIPFrameWnd");
    check(ex->IsDerivedFrom(impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ()),
          "COleDocIPFrameWndEx IsDerivedFrom COleIPFrameWnd");
    check(ex->IsDerivedFrom(&CFrameWnd::classCFrameWnd),
          "COleDocIPFrameWndEx IsDerivedFrom CFrameWnd");
    check(!ex->IsDerivedFrom(impl__GetThisClass_COleInsertDialog__SAPEAUCRuntimeClass__XZ()),
          "COleDocIPFrameWndEx is NOT derived from COleInsertDialog");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
