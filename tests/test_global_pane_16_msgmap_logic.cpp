// Logic test for global_pane_16_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_pane_16_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CBasePane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBasePane__KAPEBUAFX_MSGMAP__XZ() { return &s_CBasePane_map; }
static const AFX_MSGMAP s_CDockablePane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDockablePane__KAPEBUAFX_MSGMAP__XZ() { return &s_CDockablePane_map; }
static const AFX_MSGMAP s_CMiniFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CMiniFrameWnd_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMiniDockFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMiniDockFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMiniDockFrameWnd GetMessageMap non-null");
        check(gm == gt, "CMiniDockFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMiniDockFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMiniDockFrameWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMiniFrameWnd_map,
              "CMiniDockFrameWnd pfnGetBaseMap resolves to CMiniFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMultiPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMultiPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMultiPaneFrameWnd GetMessageMap non-null");
        check(gm == gt, "CMultiPaneFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMultiPaneFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMultiPaneFrameWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCPaneFrameWnd_msgmap,
              "CMultiPaneFrameWnd pfnGetBaseMap resolves to CPaneFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPane GetMessageMap non-null");
        check(gm == gt, "CPane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPane base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CBasePane_map,
              "CPane pfnGetBaseMap resolves to CBasePane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPaneDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPaneDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPaneDialog GetMessageMap non-null");
        check(gm == gt, "CPaneDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPaneDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPaneDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDockablePane_map,
              "CPaneDialog pfnGetBaseMap resolves to CDockablePane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPaneDivider__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPaneDivider__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPaneDivider GetMessageMap non-null");
        check(gm == gt, "CPaneDivider GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPaneDivider lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPaneDivider base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CBasePane_map,
              "CPaneDivider pfnGetBaseMap resolves to CBasePane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPaneFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPaneFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPaneFrameWnd GetMessageMap non-null");
        check(gm == gt, "CPaneFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPaneFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPaneFrameWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CPaneFrameWnd pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPaneTrackingWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPaneTrackingWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPaneTrackingWnd GetMessageMap non-null");
        check(gm == gt, "CPaneTrackingWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPaneTrackingWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPaneTrackingWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CPaneTrackingWnd pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CSmartDockingGroupGuidesWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CSmartDockingGroupGuidesWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CSmartDockingGroupGuidesWnd GetMessageMap non-null");
        check(gm == gt, "CSmartDockingGroupGuidesWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CSmartDockingGroupGuidesWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CSmartDockingGroupGuidesWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CSmartDockingGroupGuidesWnd pfnGetBaseMap resolves to CWnd map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
