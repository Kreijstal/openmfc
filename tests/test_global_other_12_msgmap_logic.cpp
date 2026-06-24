// Logic test for global_other_12_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_other_12_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CCommonDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CCommonDialog_map; }
static const AFX_MSGMAP s_CControlBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CControlBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CControlBar_map; }
static const AFX_MSGMAP s_CCtrlView_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCtrlView__KAPEBUAFX_MSGMAP__XZ() { return &s_CCtrlView_map; }
static const AFX_MSGMAP s_CDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialog_map; }
static const AFX_MSGMAP s_CFormView_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return &s_CFormView_map; }
static const AFX_MSGMAP s_CScrollView_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CScrollView__KAPEBUAFX_MSGMAP__XZ() { return &s_CScrollView_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CHtmlView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CHtmlView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CHtmlView GetMessageMap non-null");
        check(gm == gt, "CHtmlView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CHtmlView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CHtmlView base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CFormView_map,
              "CHtmlView pfnGetBaseMap resolves to CFormView map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COutlookOptionsDlg__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COutlookOptionsDlg__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COutlookOptionsDlg GetMessageMap non-null");
        check(gm == gt, "COutlookOptionsDlg GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COutlookOptionsDlg lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COutlookOptionsDlg base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialog_map,
              "COutlookOptionsDlg pfnGetBaseMap resolves to CDialog map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPreviewView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPreviewView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPreviewView GetMessageMap non-null");
        check(gm == gt, "CPreviewView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPreviewView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPreviewView base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CScrollView_map,
              "CPreviewView pfnGetBaseMap resolves to CScrollView map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPreviewViewEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPreviewViewEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPreviewViewEx GetMessageMap non-null");
        check(gm == gt, "CPreviewViewEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPreviewViewEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPreviewViewEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCPreviewView_msgmap,
              "CPreviewViewEx pfnGetBaseMap resolves to CPreviewView map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CPrintDialogEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CPrintDialogEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CPrintDialogEx GetMessageMap non-null");
        check(gm == gt, "CPrintDialogEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CPrintDialogEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CPrintDialogEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CCommonDialog_map,
              "CPrintDialogEx pfnGetBaseMap resolves to CCommonDialog map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CReBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CReBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CReBar GetMessageMap non-null");
        check(gm == gt, "CReBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CReBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CReBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CControlBar_map,
              "CReBar pfnGetBaseMap resolves to CControlBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CRichEditView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CRichEditView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CRichEditView GetMessageMap non-null");
        check(gm == gt, "CRichEditView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CRichEditView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CRichEditView base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CCtrlView_map,
              "CRichEditView pfnGetBaseMap resolves to CCtrlView map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CScreenWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CScreenWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CScreenWnd GetMessageMap non-null");
        check(gm == gt, "CScreenWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CScreenWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CScreenWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CScreenWnd pfnGetBaseMap resolves to CWnd map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
