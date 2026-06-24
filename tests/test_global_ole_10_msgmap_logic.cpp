// Logic test for global_ole_10_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_ole_10_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CControlBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CControlBar_map; }
static const AFX_MSGMAP s_CDocument_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDocument__KAPEBUAFX_MSGMAP__XZ() { return &s_CDocument_map; }

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
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleResizeBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleResizeBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleResizeBar GetMessageMap non-null");
        check(gm == gt, "COleResizeBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleResizeBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleResizeBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CControlBar_map,
              "COleResizeBar pfnGetBaseMap resolves to CControlBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleServerDoc__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleServerDoc__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleServerDoc GetMessageMap non-null");
        check(gm == gt, "COleServerDoc GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleServerDoc lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleServerDoc base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDocument_map,
              "COleServerDoc pfnGetBaseMap resolves to CDocument map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleServerDocEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleServerDocEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleServerDocEx GetMessageMap non-null");
        check(gm == gt, "COleServerDocEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleServerDocEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleServerDocEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCOleServerDoc_msgmap,
              "COleServerDocEx pfnGetBaseMap resolves to COleServerDoc map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
