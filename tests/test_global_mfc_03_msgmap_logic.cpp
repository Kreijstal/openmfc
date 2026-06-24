// Logic test for global_mfc_03_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_03_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDialogEx_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialogEx_map; }
static const AFX_MSGMAP s_CMFCPopupMenu_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCPopupMenu_map; }
static const AFX_MSGMAP s_CMiniFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CMiniFrameWnd_map; }
static const AFX_MSGMAP s_CPropertyPage_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertyPage_map; }
static const AFX_MSGMAP s_CPropertySheet_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertySheet_map; }
static const AFX_MSGMAP s_CWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CWnd_map; }

// Framework CRuntimeClass statics dragged in by afxwin.h's class graph (not
// CObject, defined inline). AUTO-RESOLVED region (fix_test_bases.sh):
// >>> BASE-STATICS
CRuntimeClass CCmdTarget::classCCmdTarget{};
CRuntimeClass CWinThread::classCWinThread{};
CRuntimeClass CWinApp::classCWinApp{};
CRuntimeClass CWnd::classCWnd{};
CRuntimeClass CDialog::classCDialog{};
// <<< BASE-STATICS

static int failures = 0;
static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\n", m); failures++; } }

int main(){
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCColorDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCColorDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCColorDialog GetMessageMap non-null");
        check(gm == gt, "CMFCColorDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCColorDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCColorDialog base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCColorPickerCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCColorPickerCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCColorPickerCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCColorPickerCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCColorPickerCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCColorPickerCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCColorPopupMenu__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCColorPopupMenu__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCColorPopupMenu GetMessageMap non-null");
        check(gm == gt, "CMFCColorPopupMenu GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCColorPopupMenu lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCColorPopupMenu base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCColorPropertySheet__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCColorPropertySheet__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCColorPropertySheet GetMessageMap non-null");
        check(gm == gt, "CMFCColorPropertySheet GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCColorPropertySheet lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCColorPropertySheet base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCCustomColorsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCCustomColorsPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCCustomColorsPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCCustomColorsPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCCustomColorsPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCCustomColorsPropertyPage base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCDesktopAlertDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCDesktopAlertDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCDesktopAlertDialog GetMessageMap non-null");
        check(gm == gt, "CMFCDesktopAlertDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCDesktopAlertDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCDesktopAlertDialog base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCDesktopAlertWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCDesktopAlertWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCDesktopAlertWnd GetMessageMap non-null");
        check(gm == gt, "CMFCDesktopAlertWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCDesktopAlertWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCDesktopAlertWnd base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCDropDownFrame__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCDropDownFrame__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCDropDownFrame GetMessageMap non-null");
        check(gm == gt, "CMFCDropDownFrame GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCDropDownFrame lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCDropDownFrame base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
