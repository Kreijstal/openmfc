// Logic test for global_propgrid_18_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_propgrid_18_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CPropertyPage_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertyPage_map; }
static const AFX_MSGMAP s_CPropertySheet_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertySheet_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPropertyGridCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPropertyGridCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPropertyGridCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCPropertyGridCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPropertyGridCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPropertyGridCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCPropertyGridCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPropertyGridToolTipCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPropertyGridToolTipCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPropertyGridToolTipCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCPropertyGridToolTipCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPropertyGridToolTipCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPropertyGridToolTipCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCPropertyGridToolTipCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPropertySheet__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPropertySheet__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPropertySheet GetMessageMap non-null");
        check(gm == gt, "CMFCPropertySheet GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPropertySheet lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPropertySheet base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertySheet_map,
              "CMFCPropertySheet pfnGetBaseMap resolves to CPropertySheet map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCPropertySheetListBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCPropertySheetListBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCPropertySheetListBox GetMessageMap non-null");
        check(gm == gt, "CMFCPropertySheetListBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCPropertySheetListBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCPropertySheetListBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCPropertySheetListBox pfnGetBaseMap resolves to CWnd map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
