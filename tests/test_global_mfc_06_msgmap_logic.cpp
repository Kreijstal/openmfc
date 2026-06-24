// Logic test for global_mfc_06_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_06_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDockablePaneAdapter_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePaneAdapter__KAPEBUAFX_MSGMAP__XZ() { return &s_CDockablePaneAdapter_map; }
static const AFX_MSGMAP s_CMFCBaseTabCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCBaseTabCtrl_map; }
static const AFX_MSGMAP s_CMFCToolBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCToolBar_map; }
static const AFX_MSGMAP s_CMiniFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CMiniFrameWnd_map; }
static const AFX_MSGMAP s_CPane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ() { return &s_CPane_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCOutlookBarPaneAdapter__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCOutlookBarPaneAdapter__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCOutlookBarPaneAdapter GetMessageMap non-null");
        check(gm == gt, "CMFCOutlookBarPaneAdapter GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCOutlookBarPaneAdapter lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCOutlookBarPaneAdapter base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCOutlookBarTabCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCOutlookBarTabCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCOutlookBarTabCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCOutlookBarTabCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCOutlookBarTabCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCOutlookBarTabCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCOutlookBarToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCOutlookBarToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCOutlookBarToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCOutlookBarToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCOutlookBarToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCOutlookBarToolBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPopupMenu__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPopupMenu GetMessageMap non-null");
        check(gm == gt, "CMFCPopupMenu GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPopupMenu lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPopupMenu base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPopupMenuBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPopupMenuBar GetMessageMap non-null");
        check(gm == gt, "CMFCPopupMenuBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPopupMenuBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPopupMenuBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPreviewCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPreviewCtrlImpl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPreviewCtrlImpl GetMessageMap non-null");
        check(gm == gt, "CMFCPreviewCtrlImpl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPreviewCtrlImpl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPreviewCtrlImpl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPrintPreviewToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPrintPreviewToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPrintPreviewToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCPrintPreviewToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPrintPreviewToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPrintPreviewToolBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCReBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCReBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCReBar GetMessageMap non-null");
        check(gm == gt, "CMFCReBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCReBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCReBar base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
