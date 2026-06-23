// Logic test for global_mfc_dialogs_ctrls_rtti.cpp (run under wine/host).
#include "../phase4/src/global_mfc_dialogs_ctrls_rtti.cpp"
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
CRuntimeClass CDialogEx::classCDialogEx{};
CRuntimeClass CEdit::classCEdit{};
CRuntimeClass CMiniFrameWnd::classCMiniFrameWnd{};
CRuntimeClass CPane::classCPane{};
CRuntimeClass CPropertyPage::classCPropertyPage{};
CRuntimeClass CPropertySheet::classCPropertySheet{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);
              const char* name; int size; unsigned schema; CRuntimeClass* base; };

int main(){
    Case cases[] = {
        { impl__GetThisClass_CMFCCaptionBar__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCCaptionBar__UEBAPEAUCRuntimeClass__XZ, "CMFCCaptionBar", 1720, 0xFFFF, &CPane::classCPane },
        { impl__GetThisClass_CMFCColorPropertySheet__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCColorPropertySheet__UEBAPEAUCRuntimeClass__XZ, "CMFCColorPropertySheet", 408, 0xFFFF, &CPropertySheet::classCPropertySheet },
        { impl__GetThisClass_CMFCCustomColorsPropertyPage__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCCustomColorsPropertyPage__UEBAPEAUCRuntimeClass__XZ, "CMFCCustomColorsPropertyPage", 1072, 0xFFFF, &CPropertyPage::classCPropertyPage },
        { impl__GetThisClass_CMFCDesktopAlertDialog__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDesktopAlertDialog__UEBAPEAUCRuntimeClass__XZ, "CMFCDesktopAlertDialog", 3776, 0xFFFF, &CDialogEx::classCDialogEx },
        { impl__GetThisClass_CMFCDesktopAlertWnd__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDesktopAlertWnd__UEBAPEAUCRuntimeClass__XZ, "CMFCDesktopAlertWnd", 6160, 0xFFFF, &CWnd::classCWnd },
        { impl__GetThisClass_CMFCDropDownFrame__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDropDownFrame__UEBAPEAUCRuntimeClass__XZ, "CMFCDropDownFrame", 5488, 0x80000001, &CMiniFrameWnd::classCMiniFrameWnd },
        { impl__GetThisClass_CMFCDynamicLayout__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCDynamicLayout__UEBAPEAUCRuntimeClass__XZ, "CMFCDynamicLayout", 80, 0xFFFF, &CObject::classCObject },
        { impl__GetThisClass_CMFCEditBrowseCtrl__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCEditBrowseCtrl__UEBAPEAUCRuntimeClass__XZ, "CMFCEditBrowseCtrl", 336, 0xFFFF, &CEdit::classCEdit },
        { impl__GetThisClass_CMFCMaskedEdit__SAPEAUCRuntimeClass__XZ, impl__GetRuntimeClass_CMFCMaskedEdit__UEBAPEAUCRuntimeClass__XZ, "CMFCMaskedEdit", 296, 0xFFFF, &CEdit::classCEdit },
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
