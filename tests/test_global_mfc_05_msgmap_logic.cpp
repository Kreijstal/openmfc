// Logic test for global_mfc_05_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_05_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CBaseTabbedPane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CBaseTabbedPane__KAPEBUAFX_MSGMAP__XZ() { return &s_CBaseTabbedPane_map; }
static const AFX_MSGMAP s_CListCtrl_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CListCtrl__KAPEBUAFX_MSGMAP__XZ() { return &s_CListCtrl_map; }
static const AFX_MSGMAP s_CMFCButton_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCButton__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCButton_map; }
static const AFX_MSGMAP s_CMFCToolBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCToolBar_map; }
static const AFX_MSGMAP s_CPropertyPage_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertyPage__KAPEBUAFX_MSGMAP__XZ() { return &s_CPropertyPage_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCLinkCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCLinkCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCLinkCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCLinkCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCLinkCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCLinkCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCListCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCListCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCListCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCListCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCListCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCListCtrl base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCMaskedEdit__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCMaskedEdit__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCMaskedEdit GetMessageMap non-null");
        check(gm == gt, "CMFCMaskedEdit GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCMaskedEdit lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCMaskedEdit base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCMenuBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCMenuBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCMenuBar GetMessageMap non-null");
        check(gm == gt, "CMFCMenuBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCMenuBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCMenuBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCMenuButton__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCMenuButton__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCMenuButton GetMessageMap non-null");
        check(gm == gt, "CMFCMenuButton GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCMenuButton lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCMenuButton base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCMousePropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCMousePropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCMousePropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCMousePropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCMousePropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCMousePropertyPage base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCOutlookBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCOutlookBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCOutlookBar GetMessageMap non-null");
        check(gm == gt, "CMFCOutlookBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCOutlookBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCOutlookBar base chain set");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCOutlookBarPane__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCOutlookBarPane__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCOutlookBarPane GetMessageMap non-null");
        check(gm == gt, "CMFCOutlookBarPane GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCOutlookBarPane lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCOutlookBarPane base chain set");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
