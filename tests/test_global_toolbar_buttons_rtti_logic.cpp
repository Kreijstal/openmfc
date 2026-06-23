// Logic test for global_toolbar_buttons_rtti.cpp (run under wine/host).
#include "../phase4/src/global_toolbar_buttons_rtti.cpp"
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
        { impl__GetThisClass_CMFCToolBarButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarButton", 136, 0x80000001, &CObject::classCObject },
        { impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarComboBoxButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarComboBoxButton", 336, 0x00000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CHelpComboBoxButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CHelpComboBoxButton__UEBAPEAUCRuntimeClass__XZ, "CHelpComboBoxButton", 344, 0x80000001, &classCMFCToolBarComboBoxButton },
        { impl__GetThisClass_CMFCToolBarMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarMenuButton", 296, 0x80000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCColorMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCColorMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCColorMenuButton", 416, 0x80000001, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CMFCCustomizeButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCCustomizeButton__UEBAPEAUCRuntimeClass__XZ, "CMFCCustomizeButton", 408, 0x80000001, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CMFCCustomizeMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCCustomizeMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCCustomizeMenuButton", 328, 0xFFFF, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CMFCDropDownToolbarButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDropDownToolbarButton__UEBAPEAUCRuntimeClass__XZ, "CMFCDropDownToolbarButton", 176, 0x80000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCOutlookBarPaneButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCOutlookBarPaneButton__UEBAPEAUCRuntimeClass__XZ, "CMFCOutlookBarPaneButton", 160, 0x00000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCRibbonGalleryMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonGalleryMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonGalleryMenuButton", 1568, 0xFFFF, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CMFCShowAllButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCShowAllButton__UEBAPEAUCRuntimeClass__XZ, "CMFCShowAllButton", 296, 0xFFFF, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CMFCToolBarColorButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarColorButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarColorButton", 176, 0x00000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCToolBarDateTimeCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarDateTimeCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarDateTimeCtrl", 168, 0x00000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCToolBarEditBoxButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarEditBoxButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarEditBoxButton", 176, 0x00000001, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCToolBarFontComboBox__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarFontComboBox__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarFontComboBox", 352, 0x00000001, &classCMFCToolBarComboBoxButton },
        { impl__GetThisClass_CMFCToolBarFontSizeComboBox__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarFontSizeComboBox__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarFontSizeComboBox", 344, 0x00000001, &classCMFCToolBarComboBoxButton },
        { impl__GetThisClass_CMFCToolBarMenuButtonsButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarMenuButtonsButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarMenuButtonsButton", 144, 0xFFFF, &classCMFCToolBarButton },
        { impl__GetThisClass_CMFCToolBarSpinEditBoxButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarSpinEditBoxButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarSpinEditBoxButton", 440, 0x00000001, &classCMFCToolBarEditBoxButton },
        { impl__GetThisClass_CMFCToolBarSystemMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCToolBarSystemMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCToolBarSystemMenuButton", 312, 0x80000001, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_COutlookCustomizeButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_COutlookCustomizeButton__UEBAPEAUCRuntimeClass__XZ, "COutlookCustomizeButton", 408, 0xFFFF, &classCMFCCustomizeButton },
        { impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CTasksPaneHistoryButton__UEBAPEAUCRuntimeClass__XZ, "CTasksPaneHistoryButton", 360, 0x00000001, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CTasksPaneMenuButton__UEBAPEAUCRuntimeClass__XZ, "CTasksPaneMenuButton", 304, 0x00000001, &classCMFCToolBarMenuButton },
        { impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CTasksPaneNavigateButton__UEBAPEAUCRuntimeClass__XZ, "CTasksPaneNavigateButton", 136, 0x00000001, &classCMFCToolBarButton },
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
