// Logic test for global_mfc_toolbars_rtti.cpp (run under wine/host).
#include "../phase4/src/global_mfc_toolbars_rtti.cpp"
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
CRuntimeClass CPane::classCPane{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCBaseToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCBaseToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCBaseToolBar", 1016, 0xFFFF, &CPane::classCPane },
        { impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBar", 4944, 0x80000001, &classCMFCBaseToolBar },
        { impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPopupMenuBar__UEBAPEAUCRuntimeClass__XZ, "CMFCPopupMenuBar", 5152, 0x00000001, &classCMFCToolBar },
        { impl__GetThisClass_CMFCColorBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCColorBar__UEBAPEAUCRuntimeClass__XZ, "CMFCColorBar", 5384, 0x00000001, &classCMFCPopupMenuBar },
        { impl__GetThisClass_CMFCDropDownToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDropDownToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCDropDownToolBar", 4944, 0x00000001, &classCMFCToolBar },
        { impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCMenuBar__UEBAPEAUCRuntimeClass__XZ, "CMFCMenuBar", 5056, 0x80000001, &classCMFCToolBar },
        { impl__GetThisClass_CMFCOutlookBarPane__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCOutlookBarPane__UEBAPEAUCRuntimeClass__XZ, "CMFCOutlookBarPane", 11112, 0x00000001, &classCMFCToolBar },
        { impl__GetThisClass_CMFCOutlookBarToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCOutlookBarToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCOutlookBarToolBar", 5008, 0xFFFF, &classCMFCToolBar },
        { impl__GetThisClass_CMFCPrintPreviewToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPrintPreviewToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCPrintPreviewToolBar", 4944, 0xFFFF, &classCMFCToolBar },
        { impl__GetThisClass_CMFCRibbonPanelMenuBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonPanelMenuBar__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonPanelMenuBar", 5520, 0xFFFF, &classCMFCPopupMenuBar },
        { impl__GetThisClass_CMFCTasksPaneToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCTasksPaneToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCTasksPaneToolBar", 4960, 0x00000001, &classCMFCToolBar },
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
