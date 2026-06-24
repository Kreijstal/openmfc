// Logic test for global_frame_01_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_frame_01_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CFrameWnd_map; }
static const AFX_MSGMAP s_CMDIChildWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIChildWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CMDIChildWnd_map; }
static const AFX_MSGMAP s_CMDIFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDIFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CMDIFrameWnd_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CControlFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CControlFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CControlFrameWnd GetMessageMap non-null");
        check(gm == gt, "CControlFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CControlFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CControlFrameWnd base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CFrameWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CFrameWndEx GetMessageMap non-null");
        check(gm == gt, "CFrameWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CFrameWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CFrameWndEx base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMDIChildWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMDIChildWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMDIChildWndEx GetMessageMap non-null");
        check(gm == gt, "CMDIChildWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMDIChildWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMDIChildWndEx base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMDIClientAreaWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMDIClientAreaWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMDIClientAreaWnd GetMessageMap non-null");
        check(gm == gt, "CMDIClientAreaWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMDIClientAreaWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMDIClientAreaWnd base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMDIFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMDIFrameWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMDIFrameWndEx GetMessageMap non-null");
        check(gm == gt, "CMDIFrameWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMDIFrameWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMDIFrameWndEx base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMDITabProxyWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMDITabProxyWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMDITabProxyWnd GetMessageMap non-null");
        check(gm == gt, "CMDITabProxyWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMDITabProxyWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMDITabProxyWnd base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMiniFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMiniFrameWnd GetMessageMap non-null");
        check(gm == gt, "CMiniFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMiniFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMiniFrameWnd base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
