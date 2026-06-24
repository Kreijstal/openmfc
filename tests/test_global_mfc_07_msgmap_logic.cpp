// Logic test for global_mfc_07_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_07_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDockablePane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ() { return &s_CDockablePane_map; }
static const AFX_MSGMAP s_CMFCBaseTabCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCBaseTabCtrl_map; }
static const AFX_MSGMAP s_CMFCListCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCListCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCListCtrl_map; }
static const AFX_MSGMAP s_CMiniFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CMiniFrameWnd_map; }
static const AFX_MSGMAP s_CPane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ() { return &s_CPane_map; }
static const AFX_MSGMAP s_CPropertyPage_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertyPage_map; }
static const AFX_MSGMAP s_CTreeCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTreeCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CTreeCtrl_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCShadowWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCShadowWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCShadowWnd GetMessageMap non-null");
        check(gm == gt, "CMFCShadowWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCShadowWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCShadowWnd base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCShellListCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCShellListCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCShellListCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCShellListCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCShellListCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCShellListCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCShellTreeCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCShellTreeCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCShellTreeCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCShellTreeCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCShellTreeCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCShellTreeCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCSpinButtonCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCSpinButtonCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCSpinButtonCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCSpinButtonCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCSpinButtonCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCStandardColorsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCStandardColorsPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCStandardColorsPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCStandardColorsPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCStandardColorsPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCStandardColorsPropertyPage base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCStatusBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCStatusBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCStatusBar GetMessageMap non-null");
        check(gm == gt, "CMFCStatusBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCStatusBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCStatusBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCTabCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCTabCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCTabCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCTabCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCTabCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCTabCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCTasksPane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCTasksPane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCTasksPane GetMessageMap non-null");
        check(gm == gt, "CMFCTasksPane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCTasksPane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCTasksPane base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
