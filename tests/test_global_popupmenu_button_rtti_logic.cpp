// Logic test for global_popupmenu_button_rtti.cpp (run under wine/host).
#include "../phase4/src/global_popupmenu_button_rtti.cpp"
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
CRuntimeClass CButton::classCButton{};
CRuntimeClass CMiniFrameWnd::classCMiniFrameWnd{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCPopupMenu__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCPopupMenu__UEBAPEAUCRuntimeClass__XZ, "CMFCPopupMenu", 6600, 0x80000001, &CMiniFrameWnd::classCMiniFrameWnd },
        { impl__GetThisClass_CMFCColorPopupMenu__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCColorPopupMenu__UEBAPEAUCRuntimeClass__XZ, "CMFCColorPopupMenu", 11992, 0xFFFF, &classCMFCPopupMenu },
        { impl__GetThisClass_CMFCDropDownListBox__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDropDownListBox__UEBAPEAUCRuntimeClass__XZ, "CMFCDropDownListBox", 6640, 0xFFFF, &classCMFCPopupMenu },
        { impl__GetThisClass_CMFCRibbonPanelMenu__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonPanelMenu__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonPanelMenu", 12128, 0xFFFF, &classCMFCPopupMenu },
        { impl__GetThisClass_CMFCRibbonMiniToolBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCRibbonMiniToolBar__UEBAPEAUCRuntimeClass__XZ, "CMFCRibbonMiniToolBar", 12144, 0xFFFF, &classCMFCRibbonPanelMenu },
        { impl__GetThisClass_CMFCButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCButton__UEBAPEAUCRuntimeClass__XZ, "CMFCButton", 2848, 0xFFFF, &CButton::classCButton },
        { impl__GetThisClass_CMFCColorButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCColorButton__UEBAPEAUCRuntimeClass__XZ, "CMFCColorButton", 3016, 0xFFFF, &classCMFCButton },
        { impl__GetThisClass_CMFCLinkCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCLinkCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCLinkCtrl", 2880, 0xFFFF, &classCMFCButton },
        { impl__GetThisClass_CMFCMenuButton__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCMenuButton__UEBAPEAUCRuntimeClass__XZ, "CMFCMenuButton", 2888, 0xFFFF, &classCMFCButton },
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
