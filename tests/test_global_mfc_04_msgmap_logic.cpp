// Logic test for global_mfc_04_msgmap.cpp (compiles+links; runs on Windows CI).
#include "../phase4/src/global_mfc_04_msgmap.cpp"
#include <cstdio>

// Out-of-batch base GetThisMessageMap getters: define sentinels so the
// standalone TU links (in the DLL these resolve to the real exports).
static const AFX_MSGMAP s_CDialogEx_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CDialogEx__KAPEBUAFX_MSGMAP__XZ() { return &s_CDialogEx_map; }
static const AFX_MSGMAP s_CMFCPopupMenu_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCPopupMenu__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCPopupMenu_map; }
static const AFX_MSGMAP s_CMFCToolBar_map = { nullptr, nullptr };
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMFCToolBar__KAPEBUAFX_MSGMAP__XZ() { return &s_CMFCToolBar_map; }
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
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCDropDownListBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCDropDownListBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCDropDownListBox GetMessageMap non-null");
        check(gm == gt, "CMFCDropDownListBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCDropDownListBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCDropDownListBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCPopupMenu_map,
              "CMFCDropDownListBox pfnGetBaseMap resolves to CMFCPopupMenu map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCDropDownToolBar__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCDropDownToolBar__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCDropDownToolBar GetMessageMap non-null");
        check(gm == gt, "CMFCDropDownToolBar GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCDropDownToolBar lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCDropDownToolBar base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CMFCToolBar_map,
              "CMFCDropDownToolBar pfnGetBaseMap resolves to CMFCToolBar map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCEditBrowseCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCEditBrowseCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCEditBrowseCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCEditBrowseCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCEditBrowseCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCEditBrowseCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCEditBrowseCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCFontComboBox__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCFontComboBox__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCFontComboBox GetMessageMap non-null");
        check(gm == gt, "CMFCFontComboBox GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCFontComboBox lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCFontComboBox base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCFontComboBox pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCHeaderCtrl__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCHeaderCtrl__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCHeaderCtrl GetMessageMap non-null");
        check(gm == gt, "CMFCHeaderCtrl GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCHeaderCtrl lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCHeaderCtrl base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCHeaderCtrl pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCImageEditorDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCImageEditorDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCImageEditorDialog GetMessageMap non-null");
        check(gm == gt, "CMFCImageEditorDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCImageEditorDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCImageEditorDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialogEx_map,
              "CMFCImageEditorDialog pfnGetBaseMap resolves to CDialogEx map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCImagePaintArea__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCImagePaintArea__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCImagePaintArea GetMessageMap non-null");
        check(gm == gt, "CMFCImagePaintArea GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCImagePaintArea lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCImagePaintArea base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CWnd_map,
              "CMFCImagePaintArea pfnGetBaseMap resolves to CWnd map");
    }
    {
        const AFX_MSGMAP* gm = impl__GetMessageMap_CMFCKeyMapDialog__MEBAPEBUAFX_MSGMAP__XZ(nullptr);
        const AFX_MSGMAP* gt = impl__GetThisMessageMap_CMFCKeyMapDialog__KAPEBUAFX_MSGMAP__XZ();
        check(gm != nullptr, "CMFCKeyMapDialog GetMessageMap non-null");
        check(gm == gt, "CMFCKeyMapDialog GetMessageMap==GetThisMessageMap");
        check(gm && gm->lpEntries != nullptr, "CMFCKeyMapDialog lpEntries non-null");
        check(gm && gm->pfnGetBaseMap != nullptr, "CMFCKeyMapDialog base chain set");
        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == &s_CDialogEx_map,
              "CMFCKeyMapDialog pfnGetBaseMap resolves to CDialogEx map");
    }
    printf("%s: %d checks failed\n", failures?"FAILED":"OK", failures);
    return failures ? 1 : 0;
}
