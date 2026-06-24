// Logic test for global_mfc_08_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_08_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialog_map; }
static const AFX_MSGMAP s_CMFCToolBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCToolBar_map; }
static const AFX_MSGMAP s_CPaneFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CPaneFrameWnd_map; }
static const AFX_MSGMAP s_CToolTipCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolTipCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CToolTipCtrl_map; }

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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCTasksPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCTasksPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCTasksPaneFrameWnd GetMessageMap non-null");
        check(gm == gt, "CMFCTasksPaneFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCTasksPaneFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCTasksPaneFrameWnd base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCTasksPaneToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCTasksPaneToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCTasksPaneToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCTasksPaneToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCTasksPaneToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCTasksPaneToolBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolTipCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolTipCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCToolTipCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolTipCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolTipCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCWindowsManagerDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCWindowsManagerDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCWindowsManagerDialog GetMessageMap non-null");
        check(gm == gt, "CMFCWindowsManagerDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCWindowsManagerDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCWindowsManagerDialog base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
