// Behavioral test for the OLE document/server-item RTTI getters
// (global_oledoc_rtti.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks. Verifies
// each descriptor's name / object size, that GetRuntimeClass returns the same
// static descriptor as GetThisClass (no self-dispatch recursion), and that
// m_pBaseClass equals the descriptor the base class's own GetThisClass returns
// (the in-file chains plus the external CDocument / CDocItem / COleClientItem /
// CCmdTarget roots), so IsKindOf walks an unbroken graph.

#include "../phase4/src/global_oledoc_rtti.cpp"

#include <cstdio>
#include <cstring>

// Base CRuntimeClass statics referenced by the impl's chain roots and dragged in
// by afxole.h's class graph; defined here only so the standalone test TU links.
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CDocument::classCDocument{};
CRuntimeClass CDocItem::classCDocItem{};
CRuntimeClass COleClientItem::classCOleClientItem{};

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
    CRuntimeClass* base;   // the descriptor m_pBaseClass must equal
};

int main() {
    Case cases[] = {
        { impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleDocument__UEBAPEAUCRuntimeClass__XZ,
          "COleDocument", 488, &CDocument::classCDocument },
        { impl__GetThisClass_COleLinkingDoc__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleLinkingDoc__UEBAPEAUCRuntimeClass__XZ,
          "COleLinkingDoc", 560, impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleServerDoc__UEBAPEAUCRuntimeClass__XZ,
          "COleServerDoc", 664, impl__GetThisClass_COleLinkingDoc__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleServerDocEx__UEBAPEAUCRuntimeClass__XZ,
          "COleServerDocEx", 664, impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleServerItem__UEBAPEAUCRuntimeClass__XZ,
          "COleServerItem", 224, &CDocItem::classCDocItem },
        { impl__GetThisClass_CDocObjectServerItem__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_CDocObjectServerItem__UEBAPEAUCRuntimeClass__XZ,
          "CDocObjectServerItem", 224, impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ() },
        { impl__GetThisClass_COleObjectFactory__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleObjectFactory__UEBAPEAUCRuntimeClass__XZ,
          "COleObjectFactory", 136, &CCmdTarget::classCCmdTarget },
        { impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ,
          impl__GetRuntimeClass_COleDocObjectItem__UEBAPEAUCRuntimeClass__XZ,
          "COleDocObjectItem", 280, &COleClientItem::classCOleClientItem },
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
        std::snprintf(buf, sizeof(buf), "%s: m_pBaseClass chains correctly", c.name);
        check(rc->m_pBaseClass == c.base, buf);
    }

    // Full IsDerivedFrom walk down the document chain: COleServerDocEx should be
    // kind-of COleServerDoc, COleLinkingDoc, COleDocument and CDocument, but not
    // the server-item branch (COleServerItem).
    CRuntimeClass* sdx = impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ();
    check(sdx->IsDerivedFrom(impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ()),
          "COleServerDocEx IsDerivedFrom COleServerDoc");
    check(sdx->IsDerivedFrom(impl__GetThisClass_COleLinkingDoc__SAPEAUCRuntimeClass__XZ()),
          "COleServerDocEx IsDerivedFrom COleLinkingDoc");
    check(sdx->IsDerivedFrom(impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ()),
          "COleServerDocEx IsDerivedFrom COleDocument");
    check(sdx->IsDerivedFrom(&CDocument::classCDocument),
          "COleServerDocEx IsDerivedFrom CDocument");
    check(!sdx->IsDerivedFrom(impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ()),
          "COleServerDocEx is NOT derived from COleServerItem");

    std::printf("RESULT: %s (%d failures)\n", g_fail == 0 ? "ALL PASS" : "FAILED", g_fail);
    return g_fail == 0 ? 0 : 1;
}
