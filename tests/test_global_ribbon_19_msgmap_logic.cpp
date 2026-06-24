// Logic test for global_ribbon_19_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_ribbon_19_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDialogEx_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialogEx_map; }
static const AFX_MSGMAP s_CMFCPopupMenu_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCPopupMenu_map; }
static const AFX_MSGMAP s_CMFCPopupMenuBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenuBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCPopupMenuBar_map; }
static const AFX_MSGMAP s_CMFCPropertyPage_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPropertyPage__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCPropertyPage_map; }
static const AFX_MSGMAP s_CPane_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPane__KAPEBUAFX_MSGMAP__XZ() { return &s_CPane_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonBar GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPane_map,
              "CMFCRibbonBar pfnGetBaseMap resolves to CPane map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonCommandsListBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonCommandsListBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonCommandsListBox GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonCommandsListBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonCommandsListBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonCommandsListBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCRibbonCommandsListBox pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonCustomizePropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonCustomizePropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonCustomizePropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonCustomizePropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonCustomizePropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonCustomizePropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCPropertyPage_map,
              "CMFCRibbonCustomizePropertyPage pfnGetBaseMap resolves to CMFCPropertyPage map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonKeyTip__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonKeyTip__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonKeyTip GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonKeyTip GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonKeyTip lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonKeyTip base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCRibbonKeyTip pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonKeyboardCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonKeyboardCustomizeDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonKeyboardCustomizeDialog GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonKeyboardCustomizeDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonKeyboardCustomizeDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonKeyboardCustomizeDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialogEx_map,
              "CMFCRibbonKeyboardCustomizeDialog pfnGetBaseMap resolves to CDialogEx map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonMiniToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonMiniToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonMiniToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonMiniToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonMiniToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonMiniToolBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &classCMFCRibbonPanelMenu_msgmap,
              "CMFCRibbonMiniToolBar pfnGetBaseMap resolves to CMFCRibbonPanelMenu map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonPanelMenu__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonPanelMenu__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonPanelMenu GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonPanelMenu GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonPanelMenu lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonPanelMenu base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCPopupMenu_map,
              "CMFCRibbonPanelMenu pfnGetBaseMap resolves to CMFCPopupMenu map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCRibbonPanelMenuBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCRibbonPanelMenuBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCRibbonPanelMenuBar GetMessageMap non-null");
        check(gm == gt, "CMFCRibbonPanelMenuBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCRibbonPanelMenuBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCRibbonPanelMenuBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCPopupMenuBar_map,
              "CMFCRibbonPanelMenuBar pfnGetBaseMap resolves to CMFCPopupMenuBar map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
