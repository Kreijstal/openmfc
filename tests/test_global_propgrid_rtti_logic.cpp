// Logic test for global_propgrid_rtti.cpp (run under wine/host).
#include "../phase4/src/global_propgrid_rtti.cpp"
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
CRuntimeClass CPropertyPage::classCPropertyPage{};
CRuntimeClass CPropertySheet::classCPropertySheet{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCPropertyGridProperty__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertyGridProperty__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertyGridProperty", 352, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CMFCPropertyGridColorProperty__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertyGridColorProperty__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertyGridColorProperty", 440, 0xFFFF, &classCMFCPropertyGridProperty },
        { impl__GetThisClass_CMFCPropertyGridFileProperty__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertyGridFileProperty__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertyGridFileProperty", 384, 0xFFFF, &classCMFCPropertyGridProperty },
        { impl__GetThisClass_CMFCPropertyGridFontProperty__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertyGridFontProperty__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertyGridFontProperty", 552, 0xFFFF, &classCMFCPropertyGridProperty },
        { impl__GetThisClass_CMFCPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertyPage", 384, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCRibbonCustomizePropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonCustomizePropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonCustomizePropertyPage", 7376, 0xFFFF, &classCMFCPropertyPage },
        { impl__GetThisClass_CMFCPropertySheet__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertySheet__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertySheet", 29232, 0xFFFF, &CPropertySheet::classCPropertySheet },
        { impl__GetThisClass_CMFCRibbonCustomizeDialog__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonCustomizeDialog__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonCustomizeDialog", 29240, 0xFFFF, &classCMFCPropertySheet },
        { impl__GetThisClass_CMFCPropertyGridCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPropertyGridCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCPropertyGridCtrl", 1720, 0xFFFF, &CWnd::classCWnd },
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
