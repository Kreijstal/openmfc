// Logic test for global_ole_09_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_ole_09_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialog_map; }
static const AFX_MSGMAP s_CFormView_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ() { return &s_CFormView_map; }
static const AFX_MSGMAP s_CFrameWnd_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFrameWnd__KAPEBUAFX_MSGMAP__XZ() { return &s_CFrameWnd_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleCntrFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleCntrFrameWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleCntrFrameWndEx GetMessageMap non-null");
        check(gm == gt, "COleCntrFrameWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleCntrFrameWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleCntrFrameWndEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CFrameWnd_map,
              "COleCntrFrameWndEx pfnGetBaseMap resolves to CFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleControl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleControl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleControl GetMessageMap non-null");
        check(gm == gt, "COleControl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleControl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleControl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "COleControl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleDBRecordView__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleDBRecordView__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleDBRecordView GetMessageMap non-null");
        check(gm == gt, "COleDBRecordView GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleDBRecordView lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleDBRecordView base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CFormView_map,
              "COleDBRecordView pfnGetBaseMap resolves to CFormView map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleDocIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleDocIPFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleDocIPFrameWnd GetMessageMap non-null");
        check(gm == gt, "COleDocIPFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleDocIPFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleDocIPFrameWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCOleIPFrameWnd_msgmap,
              "COleDocIPFrameWnd pfnGetBaseMap resolves to COleIPFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleDocIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleDocIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleDocIPFrameWndEx GetMessageMap non-null");
        check(gm == gt, "COleDocIPFrameWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleDocIPFrameWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleDocIPFrameWndEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCOleDocIPFrameWnd_msgmap,
              "COleDocIPFrameWndEx pfnGetBaseMap resolves to COleDocIPFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleIPFrameWnd__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleIPFrameWnd GetMessageMap non-null");
        check(gm == gt, "COleIPFrameWnd GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleIPFrameWnd lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleIPFrameWnd base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CFrameWnd_map,
              "COleIPFrameWnd pfnGetBaseMap resolves to CFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COleIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COleIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COleIPFrameWndEx GetMessageMap non-null");
        check(gm == gt, "COleIPFrameWndEx GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COleIPFrameWndEx lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COleIPFrameWndEx base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCOleIPFrameWnd_msgmap,
              "COleIPFrameWndEx pfnGetBaseMap resolves to COleIPFrameWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_COlePropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_COlePropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "COlePropertyPage GetMessageMap non-null");
        check(gm == gt, "COlePropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "COlePropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "COlePropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialog_map,
              "COlePropertyPage pfnGetBaseMap resolves to CDialog map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
