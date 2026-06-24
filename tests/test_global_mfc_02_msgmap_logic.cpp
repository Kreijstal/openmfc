// Logic test for global_mfc_02_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_02_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CMFCPopupMenuBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCPopupMenuBar_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCAcceleratorKeyAssignCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCAcceleratorKeyAssignCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCAcceleratorKeyAssignCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCAcceleratorKeyAssignCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCAcceleratorKeyAssignCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCAcceleratorKeyAssignCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCAcceleratorKeyAssignCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCAutoHideBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCAutoHideBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCAutoHideBar GetMessageMap non-null");
        check(gm == gt, "CMFCAutoHideBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCAutoHideBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCAutoHideBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPane_map,
              "CMFCAutoHideBar pfnGetBaseMap resolves to CPane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCBaseTabCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCBaseTabCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCBaseTabCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCBaseTabCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCBaseTabCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCBaseTabCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCBaseTabCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCBaseToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCBaseToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCBaseToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCBaseToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCBaseToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCBaseToolBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPane_map,
              "CMFCBaseToolBar pfnGetBaseMap resolves to CPane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCButton__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCButton GetMessageMap non-null");
        check(gm == gt, "CMFCButton GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCButton lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCButton base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCButton pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCCaptionBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCCaptionBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCCaptionBar GetMessageMap non-null");
        check(gm == gt, "CMFCCaptionBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCCaptionBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCCaptionBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPane_map,
              "CMFCCaptionBar pfnGetBaseMap resolves to CPane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCColorBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCColorBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCColorBar GetMessageMap non-null");
        check(gm == gt, "CMFCColorBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCColorBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCColorBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCPopupMenuBar_map,
              "CMFCColorBar pfnGetBaseMap resolves to CMFCPopupMenuBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCColorButton__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCColorButton__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCColorButton GetMessageMap non-null");
        check(gm == gt, "CMFCColorButton GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCColorButton lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCColorButton base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCMFCButton_msgmap,
              "CMFCColorButton pfnGetBaseMap resolves to CMFCButton map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
