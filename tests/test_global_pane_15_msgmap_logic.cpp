// Logic test for global_pane_15_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_pane_15_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CControlBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CControlBar_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CAutoHideDockSite__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CAutoHideDockSite__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CAutoHideDockSite GetMessageMap non-null");
        check(gm == gt, "CAutoHideDockSite GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CAutoHideDockSite lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CAutoHideDockSite base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCDockSite_msgmap,
              "CAutoHideDockSite pfnGetBaseMap resolves to CDockSite map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CBasePane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CBasePane GetMessageMap non-null");
        check(gm == gt, "CBasePane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CBasePane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CBasePane base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CBasePane pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CBaseTabbedPane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CBaseTabbedPane GetMessageMap non-null");
        check(gm == gt, "CBaseTabbedPane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CBaseTabbedPane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CBaseTabbedPane base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCDockablePane_msgmap,
              "CBaseTabbedPane pfnGetBaseMap resolves to CDockablePane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDockBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDockBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDockBar GetMessageMap non-null");
        check(gm == gt, "CDockBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDockBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDockBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CControlBar_map,
              "CDockBar pfnGetBaseMap resolves to CControlBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDockSite__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDockSite__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDockSite GetMessageMap non-null");
        check(gm == gt, "CDockSite GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDockSite lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDockSite base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCBasePane_msgmap,
              "CDockSite pfnGetBaseMap resolves to CBasePane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDockablePane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDockablePane GetMessageMap non-null");
        check(gm == gt, "CDockablePane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDockablePane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDockablePane base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPane_map,
              "CDockablePane pfnGetBaseMap resolves to CPane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDockablePaneAdapter__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDockablePaneAdapter__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDockablePaneAdapter GetMessageMap non-null");
        check(gm == gt, "CDockablePaneAdapter GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDockablePaneAdapter lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDockablePaneAdapter base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCDockablePane_msgmap,
              "CDockablePaneAdapter pfnGetBaseMap resolves to CDockablePane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDummyDockablePane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDummyDockablePane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDummyDockablePane GetMessageMap non-null");
        check(gm == gt, "CDummyDockablePane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDummyDockablePane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDummyDockablePane base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCDockablePane_msgmap,
              "CDummyDockablePane pfnGetBaseMap resolves to CDockablePane map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
