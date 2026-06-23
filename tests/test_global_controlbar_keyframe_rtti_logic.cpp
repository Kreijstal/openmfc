// Logic test for global_controlbar_keyframe_rtti.cpp (run under wine/host).
#include "../phase4/src/global_controlbar_keyframe_rtti.cpp"
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
CRuntimeClass CScrollView::classCScrollView{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CControlBar__UEBAPEAUCRuntimeClass__XZ, "CControlBar", 328, 0xFFFF, &CWnd::classCWnd },
        { impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CDockBar__UEBAPEAUCRuntimeClass__XZ, "CDockBar", 400, 0xFFFF, &classCControlBar },
        { impl__GetThisClass_COleResizeBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_COleResizeBar__UEBAPEAUCRuntimeClass__XZ, "COleResizeBar", 408, 0xFFFF, &classCControlBar },
        { impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CReBar__UEBAPEAUCRuntimeClass__XZ, "CReBar", 336, 0xFFFF, &classCControlBar },
        { impl__GetThisClass_CStatusBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CStatusBar__UEBAPEAUCRuntimeClass__XZ, "CStatusBar", 336, 0xFFFF, &classCControlBar },
        { impl__GetThisClass_CBaseKeyFrame__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CBaseKeyFrame__UEBAPEAUCRuntimeClass__XZ, "CBaseKeyFrame", 24, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CKeyFrame__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CKeyFrame__UEBAPEAUCRuntimeClass__XZ, "CKeyFrame", 48, 0xFFFF, &classCBaseKeyFrame },
        { impl__GetThisClass_CPreviewView__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CPreviewView__UEBAPEAUCRuntimeClass__XZ, "CPreviewView", 536, 0xFFFF, &CScrollView::classCScrollView },
        { impl__GetThisClass_CPreviewViewEx__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CPreviewViewEx__UEBAPEAUCRuntimeClass__XZ, "CPreviewViewEx", 5536, 0xFFFF, &classCPreviewView },
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
