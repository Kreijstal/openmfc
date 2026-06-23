// Logic test for global_ribbon_panels_rtti.cpp (run under wine/host).
#include "../phase4/src/global_ribbon_panels_rtti.cpp"
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
CRuntimeClass CMFCRibbonBar::classCMFCRibbonBar{};
CRuntimeClass CMFCRibbonPanel::classCMFCRibbonPanel{};
CRuntimeClass CRichEditCtrl::classCRichEditCtrl{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCRibbonCustomizeCategory__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonCustomizeCategory__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonCustomizeCategory", 72, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CMFCRibbonMainPanel__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonMainPanel__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonMainPanel", 1832, 0xFFFF, &CMFCRibbonPanel::classCMFCRibbonPanel },
        { impl__GetThisClass_CMFCRibbonRichEditCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonRichEditCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonRichEditCtrl", 264, 0xFFFF, &CRichEditCtrl::classCRichEditCtrl },
        { impl__GetThisClass_CMFCRibbonStatusBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonStatusBar__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonStatusBar", 8728, 0xFFFF, &CMFCRibbonBar::classCMFCRibbonBar },
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
