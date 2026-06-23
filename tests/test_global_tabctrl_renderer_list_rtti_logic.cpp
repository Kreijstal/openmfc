// Logic test for global_tabctrl_renderer_list_rtti.cpp (run under wine/host).
#include "../phase4/src/global_tabctrl_renderer_list_rtti.cpp"
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
CRuntimeClass CListCtrl::classCListCtrl{};
CRuntimeClass CPane::classCPane{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCBaseTabCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCBaseTabCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCBaseTabCtrl", 960, 0xFFFF, &CWnd::classCWnd },
        { impl__GetThisClass_CMFCOutlookBarTabCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCOutlookBarTabCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCOutlookBarTabCtrl", 11768, 0xFFFF, &classCMFCBaseTabCtrl },
        { impl__GetThisClass_CMFCTabCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCTabCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCTabCtrl", 15720, 0xFFFF, &classCMFCBaseTabCtrl },
        { impl__GetThisClass_CMFCControlRenderer__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCControlRenderer__UEBAPEAUCRuntimeClass__XZ, "CMFCControlRenderer", 512, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CMFCShadowRenderer__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCShadowRenderer__UEBAPEAUCRuntimeClass__XZ, "CMFCShadowRenderer", 512, 0xFFFF, &classCMFCControlRenderer },
        { impl__GetThisClass_CMFCListCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCListCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCListCtrl", 576, 0xFFFF, &CListCtrl::classCListCtrl },
        { impl__GetThisClass_CMFCShellListCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCShellListCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCShellListCtrl", 632, 0xFFFF, &classCMFCListCtrl },
        { impl__GetThisClass_CMFCAutoHideBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCAutoHideBar__UEBAPEAUCRuntimeClass__XZ, "CMFCAutoHideBar", 1112, 0xFFFF, &CPane::classCPane },
        { impl__GetThisClass_CMFCAutoHideButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCAutoHideButton__UEBAPEAUCRuntimeClass__XZ, "CMFCAutoHideButton", 64, 0xFFFF, &CObject::classCObject },
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
