// Logic test for global_ribbon_20_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_ribbon_20_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CMFCRibbonBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCRibbonBar_map; }
static const AFX_MSGMAP s_CMFCSpinButtonCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCSpinButtonCtrl_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonRichEditCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonRichEditCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonRichEditCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonRichEditCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonRichEditCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonRichEditCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCRibbonRichEditCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonSpinButtonCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonSpinButtonCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonSpinButtonCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonSpinButtonCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonSpinButtonCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonSpinButtonCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCSpinButtonCtrl_map,
              "CMFCRibbonSpinButtonCtrl pfnGetBaseMap resolves to CMFCSpinButtonCtrl map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonStatusBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonStatusBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonStatusBar GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonStatusBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonStatusBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonStatusBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCRibbonBar_map,
              "CMFCRibbonStatusBar pfnGetBaseMap resolves to CMFCRibbonBar map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
