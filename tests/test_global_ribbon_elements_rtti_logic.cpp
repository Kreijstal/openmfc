// Logic test for global_ribbon_elements_rtti.cpp (run under wine/host).
#include "../phase4/src/global_ribbon_elements_rtti.cpp"
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
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCBaseAccessibleObject__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCBaseAccessibleObject__UEBAPEAUCRuntimeClass__XZ, "CMFCBaseAccessibleObject", 160, 0xFFFF, &CCmdTarget::classCCmdTarget },
        { impl__GetThisClass_CMFCRibbonBaseElement__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonBaseElement__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonBaseElement", 384, 0xFFFF, &classCMFCBaseAccessibleObject },
        { impl__GetThisClass_CMFCRibbonButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonButton", 624, 0xFFFF, &classCMFCRibbonBaseElement },
        { impl__GetThisClass_CMFCRibbonApplicationButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonApplicationButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonApplicationButton", 1440, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonButtonsGroup__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonButtonsGroup__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonButtonsGroup", 1648, 0xFFFF, &classCMFCRibbonBaseElement },
        { impl__GetThisClass_CMFCRibbonCaptionButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonCaptionButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonCaptionButton", 632, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonCheckBox__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonCheckBox__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonCheckBox", 624, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonGallery__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonGallery__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonGallery", 1272, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonColorButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonColorButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonColorButton", 1496, 0xFFFF, &classCMFCRibbonGallery },
        { impl__GetThisClass_CMFCRibbonColorMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonColorMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonColorMenuButton", 640, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonEdit__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonEdit__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonEdit", 688, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonComboBox__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonComboBox__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonComboBox", 816, 0xFFFF, &classCMFCRibbonEdit },
        { impl__GetThisClass_CMFCRibbonContextCaption__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonContextCaption__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonContextCaption", 640, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonDefaultPanelButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonDefaultPanelButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonDefaultPanelButton", 640, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonFontComboBox__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonFontComboBox__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonFontComboBox", 840, 0xFFFF, &classCMFCRibbonComboBox },
        { impl__GetThisClass_CMFCRibbonGalleryIcon__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonGalleryIcon__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonGalleryIcon", 656, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonLabel__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonLabel__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonLabel", 624, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonLaunchButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonLaunchButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonLaunchButton", 632, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonLinkCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonLinkCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonLinkCtrl", 632, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonMainPanelButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonMainPanelButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonMainPanelButton", 624, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonProgressBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonProgressBar__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonProgressBar", 408, 0xFFFF, &classCMFCRibbonBaseElement },
        { impl__GetThisClass_CMFCRibbonQuickAccessCustomizeButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonQuickAccessCustomizeButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonQuickAccessCustomizeButton", 672, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonQuickAccessToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonQuickAccessToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonQuickAccessToolBar", 1744, 0xFFFF, &classCMFCRibbonButtonsGroup },
        { impl__GetThisClass_CMFCRibbonRecentFilesList__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonRecentFilesList__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonRecentFilesList", 1648, 0xFFFF, &classCMFCRibbonButtonsGroup },
        { impl__GetThisClass_CMFCRibbonSeparator__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonSeparator__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonSeparator", 392, 0xFFFF, &classCMFCRibbonBaseElement },
        { impl__GetThisClass_CMFCRibbonSlider__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonSlider__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonSlider", 480, 0xFFFF, &classCMFCRibbonBaseElement },
        { impl__GetThisClass_CMFCRibbonStatusBarCustomizeButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonStatusBarCustomizeButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonStatusBarCustomizeButton", 624, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonStatusBarPane__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonStatusBarPane__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonStatusBarPane", 1072, 0xFFFF, &classCMFCRibbonButton },
        { impl__GetThisClass_CMFCRibbonTab__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonTab__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonTab", 400, 0xFFFF, &classCMFCRibbonBaseElement },
        { impl__GetThisClass_CMFCRibbonTabsGroup__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonTabsGroup__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonTabsGroup", 1648, 0xFFFF, &classCMFCRibbonButtonsGroup },
        { impl__GetThisClass_CMFCRibbonUndoButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonUndoButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonUndoButton", 1352, 0xFFFF, &classCMFCRibbonGallery },
        { impl__GetThisClass_CRibbonUndoLabel__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CRibbonUndoLabel__UEBAPEAUCRuntimeClass__XZ, "CRibbonUndoLabel", 624, 0xFFFF, &classCMFCRibbonButton },
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
