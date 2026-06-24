// Logic test for global_toolbar_23_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_toolbar_23_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CPropertyPage_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertyPage_map; }

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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsOptionsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsOptionsPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsOptionsPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsOptionsPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsOptionsPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsOptionsPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCToolBarsOptionsPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsToolsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsToolsPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsToolsPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsToolsPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsToolsPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsToolsPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCToolBarsToolsPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
