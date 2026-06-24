// Logic test for global_pane_17_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_pane_17_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CBaseTabbedPane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ() { return &s_CBaseTabbedPane_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CSmartDockingHighlighterWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CSmartDockingHighlighterWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CSmartDockingHighlighterWnd GetMessageMap non-null");
        check(gm == gt, "CSmartDockingHighlighterWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CSmartDockingHighlighterWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CSmartDockingHighlighterWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CSmartDockingHighlighterWnd pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CSmartDockingStandaloneGuideWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CSmartDockingStandaloneGuideWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CSmartDockingStandaloneGuideWnd GetMessageMap non-null");
        check(gm == gt, "CSmartDockingStandaloneGuideWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CSmartDockingStandaloneGuideWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CSmartDockingStandaloneGuideWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CSmartDockingStandaloneGuideWnd pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CTabbedPane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CTabbedPane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CTabbedPane GetMessageMap non-null");
        check(gm == gt, "CTabbedPane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CTabbedPane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CTabbedPane base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CBaseTabbedPane_map,
              "CTabbedPane pfnGetBaseMap resolves to CBaseTabbedPane map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
