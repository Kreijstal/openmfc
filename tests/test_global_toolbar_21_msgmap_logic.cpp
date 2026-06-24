// Logic test for global_toolbar_21_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_toolbar_21_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CCmdTarget_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ() { return &s_CCmdTarget_map; }
static const AFX_MSGMAP s_CDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialog_map; }
static const AFX_MSGMAP s_CMFCBaseToolBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCBaseToolBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCBaseToolBar_map; }
static const AFX_MSGMAP s_CMFCEditBrowseCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCEditBrowseCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCEditBrowseCtrl_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCBaseToolBar_map,
              "CMFCToolBar pfnGetBaseMap resolves to CMFCBaseToolBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarButtonCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarButtonCustomizeDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarButtonCustomizeDialog GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarButtonCustomizeDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarButtonCustomizeDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarButtonCustomizeDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialog_map,
              "CMFCToolBarButtonCustomizeDialog pfnGetBaseMap resolves to CDialog map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarButtonsListButton__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarButtonsListButton__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarButtonsListButton GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarButtonsListButton GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarButtonsListButton lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarButtonsListButton base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCToolBarButtonsListButton pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarComboBoxEdit__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarComboBoxEdit__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarComboBoxEdit GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarComboBoxEdit GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarComboBoxEdit lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarComboBoxEdit base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCToolBarComboBoxEdit pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarDateTimeCtrlImpl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarDateTimeCtrlImpl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarDateTimeCtrlImpl GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarDateTimeCtrlImpl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarDateTimeCtrlImpl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarDateTimeCtrlImpl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCToolBarDateTimeCtrlImpl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarDropSource__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarDropSource__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarDropSource GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarDropSource GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarDropSource lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarDropSource base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CCmdTarget_map,
              "CMFCToolBarDropSource pfnGetBaseMap resolves to CCmdTarget map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarDropTarget__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarDropTarget__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarDropTarget GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarDropTarget GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarDropTarget lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarDropTarget base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CCmdTarget_map,
              "CMFCToolBarDropTarget pfnGetBaseMap resolves to CCmdTarget map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarEditCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarEditCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarEditCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarEditCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarEditCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarEditCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCEditBrowseCtrl_map,
              "CMFCToolBarEditCtrl pfnGetBaseMap resolves to CMFCEditBrowseCtrl map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
