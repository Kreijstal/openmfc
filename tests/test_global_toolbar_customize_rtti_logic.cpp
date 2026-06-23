// Logic test for global_toolbar_customize_rtti.cpp (run under wine/host).
#include "../phase4/src/global_toolbar_customize_rtti.cpp"
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
CRuntimeClass CDockablePane::classCDockablePane{};
CRuntimeClass CPaneFrameWnd::classCPaneFrameWnd{};
CRuntimeClass CPropertyPage::classCPropertyPage{};
CRuntimeClass CPropertySheet::classCPropertySheet{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCTasksPane__UEBAPEAUCRuntimeClass__XZ, "CMFCTasksPane", 6936, 0x80000001, &CDockablePane::classCDockablePane },
        { impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCTasksPaneFrameWnd__UEBAPEAUCRuntimeClass__XZ, "CMFCTasksPaneFrameWnd", 608, 0x80000002, &CPaneFrameWnd::classCPaneFrameWnd },
        { impl__GetThisClass_CMFCToolBarsCommandsPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarsCommandsPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarsCommandsPropertyPage", 848, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarsCustomizeDialog__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarsCustomizeDialog", 3512, 0xFFFF, &CPropertySheet::classCPropertySheet },
        { impl__GetThisClass_CMFCToolBarsKeyboardPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarsKeyboardPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarsKeyboardPropertyPage", 2552, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCToolBarsListPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarsListPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarsListPropertyPage", 1816, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCToolBarsMenuPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarsMenuPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarsMenuPropertyPage", 2752, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCToolBarsOptionsPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarsOptionsPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarsOptionsPropertyPage", 1992, 0xFFFF, &CPropertyPage::classCPropertyPage },
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
