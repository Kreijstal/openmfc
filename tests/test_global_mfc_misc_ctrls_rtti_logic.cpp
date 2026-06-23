// Logic test for global_mfc_misc_ctrls_rtti.cpp (run under wine/host).
#include "../phase4/src/global_mfc_misc_ctrls_rtti.cpp"
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
CRuntimeClass CMFCVisualManagerVS2005::classCMFCVisualManagerVS2005{};
CRuntimeClass CPane::classCPane{};
CRuntimeClass CPropertyPage::classCPropertyPage{};
CRuntimeClass CTreeCtrl::classCTreeCtrl{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCMousePropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCMousePropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCMousePropertyPage", 1072, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCPreviewCtrlImpl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPreviewCtrlImpl__UEBAPEAUCRuntimeClass__XZ, "CMFCPreviewCtrlImpl", 264, 0xFFFF, &CWnd::classCWnd },
        { impl__GetThisClass_CMFCReBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCReBar__UEBAPEAUCRuntimeClass__XZ, "CMFCReBar", 1040, 0xFFFF, &CPane::classCPane },
        { impl__GetThisClass_CMFCShellTreeCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCShellTreeCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCShellTreeCtrl", 256, 0xFFFF, &CTreeCtrl::classCTreeCtrl },
        { impl__GetThisClass_CMFCStandardColorsPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCStandardColorsPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCStandardColorsPropertyPage", 1048, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCStatusBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCStatusBar__UEBAPEAUCRuntimeClass__XZ, "CMFCStatusBar", 1056, 0xFFFF, &CPane::classCPane },
        { impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCVisualManagerVS2008__UEBAPEAUCRuntimeClass__XZ, "CMFCVisualManagerVS2008", 824, 0xFFFF, &CMFCVisualManagerVS2005::classCMFCVisualManagerVS2005 },
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
