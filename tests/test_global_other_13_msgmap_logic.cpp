// Logic test for global_other_13_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_other_13_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CControlBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CControlBar_map; }
static const AFX_MSGMAP s_CVSListBoxBase_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ() { return &s_CVSListBoxBase_map; }
static const AFX_MSGMAP s_CView_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CView__KAPEBUAFX_MSGMAP__XZ() { return &s_CView_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CSplitButton__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CSplitButton__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CSplitButton GetMessageMap non-null");
        check(gm == gt, "CSplitButton GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CSplitButton lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CSplitButton base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CSplitButton pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CSplitterWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CSplitterWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CSplitterWnd GetMessageMap non-null");
        check(gm == gt, "CSplitterWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CSplitterWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CSplitterWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CSplitterWnd pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CSplitterWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CSplitterWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CSplitterWndEx GetMessageMap non-null");
        check(gm == gt, "CSplitterWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CSplitterWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CSplitterWndEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCSplitterWnd_msgmap,
              "CSplitterWndEx pfnGetBaseMap resolves to CSplitterWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CStatusBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CStatusBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CStatusBar GetMessageMap non-null");
        check(gm == gt, "CStatusBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CStatusBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CStatusBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CControlBar_map,
              "CStatusBar pfnGetBaseMap resolves to CControlBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CTabView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CTabView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CTabView GetMessageMap non-null");
        check(gm == gt, "CTabView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CTabView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CTabView base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CView_map,
              "CTabView pfnGetBaseMap resolves to CView map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CToolBar GetMessageMap non-null");
        check(gm == gt, "CToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CToolBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CControlBar_map,
              "CToolBar pfnGetBaseMap resolves to CControlBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CToolBarCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CToolBarCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CToolBarCtrl GetMessageMap non-null");
        check(gm == gt, "CToolBarCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CToolBarCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CToolBarCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CToolBarCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CVSListBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CVSListBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CVSListBox GetMessageMap non-null");
        check(gm == gt, "CVSListBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CVSListBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CVSListBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CVSListBoxBase_map,
              "CVSListBox pfnGetBaseMap resolves to CVSListBoxBase map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
