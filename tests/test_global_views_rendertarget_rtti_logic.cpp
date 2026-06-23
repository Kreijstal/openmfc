// Logic test for global_views_rendertarget_rtti.cpp (run under wine/host).
#include "../phase4/src/global_views_rendertarget_rtti.cpp"
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
CRuntimeClass CDocument::classCDocument{};
CRuntimeClass CFileFind::classCFileFind{};
CRuntimeClass CHtmlView::classCHtmlView{};
CRuntimeClass CRenderTarget::classCRenderTarget{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CSplitterWnd__UEBAPEAUCRuntimeClass__XZ, "CSplitterWnd", 384, 0xFFFF, &CWnd::classCWnd },
        { impl__GetThisClass_CSplitterWndEx__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CSplitterWndEx__UEBAPEAUCRuntimeClass__XZ, "CSplitterWndEx", 384, 0xFFFF, &classCSplitterWnd },
        { impl__GetThisClass_CBaseTransition__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CBaseTransition__UEBAPEAUCRuntimeClass__XZ, "CBaseTransition", 56, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CBitmapRenderTarget__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CBitmapRenderTarget__UEBAPEAUCRuntimeClass__XZ, "CBitmapRenderTarget", 88, 0xFFFF, &CRenderTarget::classCRenderTarget },
        { impl__GetThisClass_CGopherFileFind__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CGopherFileFind__UEBAPEAUCRuntimeClass__XZ, "CGopherFileFind", 72, 0xFFFF, &CFileFind::classCFileFind },
        { impl__GetThisClass_CHtmlEditDoc__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CHtmlEditDoc__UEBAPEAUCRuntimeClass__XZ, "CHtmlEditDoc", 384, 0xFFFF, &CDocument::classCDocument },
        { impl__GetThisClass_CHtmlEditView__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CHtmlEditView__UEBAPEAUCRuntimeClass__XZ, "CHtmlEditView", 592, 0xFFFF, &CHtmlView::classCHtmlView },
        { impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CHwndRenderTarget__UEBAPEAUCRuntimeClass__XZ, "CHwndRenderTarget", 88, 0xFFFF, &CRenderTarget::classCRenderTarget },
        { impl__GetThisClass_CMDITabProxyWnd__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMDITabProxyWnd__UEBAPEAUCRuntimeClass__XZ, "CMDITabProxyWnd", 240, 0xFFFF, &CWnd::classCWnd },
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
