// Logic test for global_other_14_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_other_14_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CVSListBoxBase__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CVSListBoxBase GetMessageMap non-null");
        check(gm == gt, "CVSListBoxBase GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CVSListBoxBase lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CVSListBoxBase base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CVSListBoxEditCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CVSListBoxEditCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CVSListBoxEditCtrl GetMessageMap non-null");
        check(gm == gt, "CVSListBoxEditCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CVSListBoxEditCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CVSListBoxEditCtrl base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
