// Logic test for global_other_11_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_other_11_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialog_map; }
static const AFX_MSGMAP s_CFormView_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return &s_CFormView_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CCmdTarget__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CCmdTarget__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CCmdTarget GetMessageMap non-null");
        check(gm == gt, "CCmdTarget GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CCmdTarget lpEntries non-null");
        check(gm && gm->pfnGetBaseMap == nullptr, "CCmdTarget root has null base");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CCommonDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CCommonDialog GetMessageMap non-null");
        check(gm == gt, "CCommonDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CCommonDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CCommonDialog base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CControlBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CControlBar GetMessageMap non-null");
        check(gm == gt, "CControlBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CControlBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CControlBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CCtrlView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CCtrlView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CCtrlView GetMessageMap non-null");
        check(gm == gt, "CCtrlView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CCtrlView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CCtrlView base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDHtmlDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDHtmlDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDHtmlDialog GetMessageMap non-null");
        check(gm == gt, "CDHtmlDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDHtmlDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDHtmlDialog base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDialogBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDialogBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDialogBar GetMessageMap non-null");
        check(gm == gt, "CDialogBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDialogBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDialogBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CDocObjectServer__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CDocObjectServer__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CDocObjectServer GetMessageMap non-null");
        check(gm == gt, "CDocObjectServer GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CDocObjectServer lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CDocObjectServer base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CHtmlEditView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CHtmlEditView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CHtmlEditView GetMessageMap non-null");
        check(gm == gt, "CHtmlEditView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CHtmlEditView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CHtmlEditView base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
