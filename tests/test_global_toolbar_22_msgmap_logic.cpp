// Logic test for global_toolbar_22_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_toolbar_22_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CCheckListBox_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCheckListBox__KAPEBUAFX_MSGMAP__XZ() { return &s_CCheckListBox_map; }
static const AFX_MSGMAP s_CDialog_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialog__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialog_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarNameDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarNameDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarNameDialog GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarNameDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarNameDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarNameDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialog_map,
              "CMFCToolBarNameDialog pfnGetBaseMap resolves to CDialog map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsCommandsListBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsCommandsListBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsCommandsListBox GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsCommandsListBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsCommandsListBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsCommandsListBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCToolBarsCommandsListBox pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsCommandsPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsCommandsPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsCommandsPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsCommandsPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsCommandsPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsCommandsPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCToolBarsCommandsPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsCustomizeDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsCustomizeDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsCustomizeDialog GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsCustomizeDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsCustomizeDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsCustomizeDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertySheet_map,
              "CMFCToolBarsCustomizeDialog pfnGetBaseMap resolves to CPropertySheet map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsKeyboardPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsKeyboardPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsKeyboardPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsKeyboardPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsKeyboardPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsKeyboardPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCToolBarsKeyboardPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsListCheckBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsListCheckBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsListCheckBox GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsListCheckBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsListCheckBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsListCheckBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CCheckListBox_map,
              "CMFCToolBarsListCheckBox pfnGetBaseMap resolves to CCheckListBox map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsListPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsListPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsListPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsListPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsListPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsListPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCToolBarsListPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCToolBarsMenuPropertyPage__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCToolBarsMenuPropertyPage__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCToolBarsMenuPropertyPage GetMessageMap non-null");
        check(gm == gt, "CMFCToolBarsMenuPropertyPage GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCToolBarsMenuPropertyPage lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCToolBarsMenuPropertyPage base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CPropertyPage_map,
              "CMFCToolBarsMenuPropertyPage pfnGetBaseMap resolves to CPropertyPage map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
