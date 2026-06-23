// Logic test for global_frames_docs_preview_rtti.cpp (run under wine/host).
#include "../phase4/src/global_frames_docs_preview_rtti.cpp"
#include <cstdio>
#include <cstring>

// out-of-set / framework base descriptors, defined here only so the
// standalone test TU links. AUTO-RESOLVED region (fix_test_bases.sh):
// >>> BASE-STATICS
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CDialog::classCDialog{};
CRuntimeClass CDC::classCDC{};
CRuntimeClass CFormView::classCFormView{};
CRuntimeClass CFrameWnd::classCFrameWnd{};
CRuntimeClass COleClientItem::classCOleClientItem{};
CRuntimeClass COleServerDoc::classCOleServerDoc{};
CRuntimeClass CPaneFrameWnd::classCPaneFrameWnd{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMultiPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ, "CMultiPaneFrameWnd", 768, 0x80000002, &CPaneFrameWnd::classCPaneFrameWnd },
        { impl__GetThisClass_COleCntrFrameWndEx__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_COleCntrFrameWndEx__UEBAPEAUCRuntimeClass__XZ, "COleCntrFrameWndEx", 1352, 0xFFFF, &CFrameWnd::classCFrameWnd },
        { impl__GetThisClass_COleDBRecordView__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_COleDBRecordView__UEBAPEAUCRuntimeClass__XZ, "COleDBRecordView", 360, 0xFFFF, &CFormView::classCFormView },
        { impl__GetThisClass_COlePropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_COlePropertyPage__UEBAPEAUCRuntimeClass__XZ, "COlePropertyPage", 512, 0xFFFF, &CDialog::classCDialog },
        { impl__GetThisClass_CPreviewDC__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CPreviewDC__UEBAPEAUCRuntimeClass__XZ, "CPreviewDC", 88, 0xFFFF, &CDC::classCDC },
        { impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CRichEditCntrItem__UEBAPEAUCRuntimeClass__XZ, "CRichEditCntrItem", 256, 0x00000000, &COleClientItem::classCOleClientItem },
        { impl__GetThisClass_CRichEditDoc__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CRichEditDoc__UEBAPEAUCRuntimeClass__XZ, "CRichEditDoc", 672, 0xFFFF, &COleServerDoc::classCOleServerDoc },
        { impl__GetThisClass_CSmartDockingGroupGuidesManager__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CSmartDockingGroupGuidesManager__UEBAPEAUCRuntimeClass__XZ, "CSmartDockingGroupGuidesManager", 6680, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CSmartDockingStandaloneGuide__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CSmartDockingStandaloneGuide__UEBAPEAUCRuntimeClass__XZ, "CSmartDockingStandaloneGuide", 1128, 0xFFFF, &CObject::classCObject },
    };
    for (const Case& c : cases) {
        CRuntimeClass* r = c.gt();
        check(r != nullptr, c.name);
        check(r && strcmp(r->m_lpszClassName, c.name)==0, c.name);
        check(r && r->m_nObjectSize == c.size, c.name);
        check(r && r->m_wSchema == c.schema, c.name);
        check(r && r->m_pfnCreateObject == nullptr, c.name);
        check(r && r->m_pfnGetBaseClass == nullptr, c.name);
        check(c.grc(nullptr) == r, c.name);   // no self-dispatch
        check(r && r->m_pBaseClass == c.base, c.name);
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
