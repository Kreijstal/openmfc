// CMFCTasksPane — OpenMFC implementation.
// Sources: ctrlcore.cpp, global_mfc-feature-20_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CtrlcoreSupport.h"
#include "detail/CMFCTasksPaneSupport.h"
#include "detail/MfccoreSupport.h"
#include "detail/CMFCTasksPaneTaskGroupSupport.h"     // S_Cmfctaskspanetaskgroup (retail group record)
#include "detail/CMFCTasksPaneTaskSupport.h"          // S_Cmfctaskspanetask (retail task record)
#include "detail/CMFCTasksPanePropertyPageSupport.h"  // S_Cmfctaskspanepropertypage (retail page record)

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <new>   // placement new only
#include <commctrl.h>   // ImageList_* (comctl32 is on the phase4 link line)

// ---------------------------------------------------------------------------
// Retail object model used by the bodies below (all offsets read from the
// mfc140u disassembly and pinned by the shadow structs named above):
//
//   CMFCTasksPane (S_MfcFeature20Impl, detail/CMFCTasksPaneSupport.h)
//     +0x608 CObList m_lstTasksPanes   (pages;  head +0x610, count +0x620)
//     +0x640 CObList m_lstTaskGroups   (groups; head +0x648, count +0x658)
//     +0x778 CArray<int,int> m_arrHistoryStack (m_pData +0x780, m_nSize +0x788,
//            m_nMaxSize +0x790, m_nGrowBy +0x798)
//   CMFCTasksPaneTaskGroup (S_Cmfctaskspanetaskgroup): m_pPage +8, m_lstTasks
//     +0x18 (head +0x20, count +0x30), m_bIsCollapsed +0x58, m_rect +0x5c
//   CMFCTasksPaneTask (S_Cmfctaskspanetask): m_pGroup +8, m_strName +0x10,
//     m_rect +0x18, m_uiCommandID +0x30, m_hwndTask +0x40
//   CMFCTasksPanePropertyPage (S_Cmfctaskspanepropertypage): m_strName +8,
//     m_pTaskPane +0x10
//
// Retail walks the CObLists inline (head pointer, node->pNext, node->data).
// OpenMFC's CObList keeps its live contents in a side table keyed by the
// list's address (include/openmfc/afx.h, OPENMFC_DECLARE_LIST_WRAPPER), so the
// in-object head/count words are NOT the working representation.  Every walk
// below therefore goes through the exported FindIndex thunk and the node view
// it hands back (the same pattern featurepack/visualmanager/CMFCVisualManager.cpp
// and docking/CDockingPanesRow.cpp use), and every mutation goes through the
// AddTail / RemoveAt / RemoveHead thunks.  The retail counts at
// +0x620 / +0x658 / +0x30 are replaced by walking the list.  That is a storage
// deviation only; the order of operations is retail's.
//
// KNOWN GAP (not fixable in this file): OpenMFC's CMFCTasksPane is a 552-byte
// CBasePane subclass (measured with the phase4 mingw flags) whose constructor
// never touches the retail members from +0x4E0 up, and the retail object is
// 0x1B18 bytes (the deleting destructor at mfc140u RVA 0x144390 frees with
// size 0x1B18; +0x7B8 is a full CMFCTasksPaneToolBar, not a 0x48-byte blob).
// So m_iActivePage, m_arrHistoryStack, m_wndToolBar etc. are uninitialised
// storage until the ctor thunk (taskspane/Thunks.cpp) value-initialises that
// range and CreateObject (DynCreateFactories.cpp) allocates the retail size.
// The bodies below are written against the retail layout exactly as the
// pre-existing OnPress* / OnGetFont / OnSetFont handlers already were, and
// read the history array only through retail's own bounds checks.
//
// SECOND GAP (review note, not fixable here without porting older exports):
// the exported AddGroup / AddTask / RemoveAllTasks / SetCaption / SetGroupName
// thunks in this file forward to the C++ methods defined below them, and those
// keep their groups and tasks in OpenMFC's own side table
// (mfccore::TasksPaneState, detail/MfccoreSupport.h); they never touch
// m_lstTaskGroups / m_lstTasksPanes.  The retail-layout bodies only see what
// reached the CObLists at +0x640 / +0x608 (pages from AddPage, tasks from
// AddWindow), so a group created through the exported AddGroup is invisible to
// GetTaskGroup / RemoveGroup / CollapseGroup / GetTaskLocation / AddWindow
// until AddGroup and AddTask are ported to the retail list.
// ---------------------------------------------------------------------------

// Exported thunks used below (BRIEFING S1: no C++ method symbols exist inside
// this DLL).  Each declaration was checked against its definition:
//   core/collections/CObList.cpp        FindIndex / AddTail / RemoveAt / RemoveHead
//                                       (RemoveAt takes POSITION* in OpenMFC; called
//                                       with &pos exactly as docking/CDockingPanesRow.cpp does)
//   core/collections/CStringList.cpp    AddTail(const CString&) / RemoveAll
//   core/runtime/CObject.cpp            GetRuntimeClass (declared in
//                                       detail/CMFCTasksPaneTaskGroupSupport.h)
//   core/window/CWnd.cpp                FromHandle, FromHandlePermanent,
//                                       DestroyWindow, ShowWindow
//   core/window/Thunks.cpp              GetCurrentMessage, GetTopLevelFrame,
//                                       SetFocus, SetScrollPos, GetStyle
//   core/gdi/CGdiObject.cpp             CGdiObject Attach / DeleteObject / FromHandle
//   core/app/CRecentFileList.cpp        GetDisplayName@CRecentFileList
//   detail/MfcExceptionsSupport.cpp     AfxThrowInvalidArgException (a real throw)
//   featurepack/docking/CDockablePane.cpp  OnCancelMode@CDockablePane
//   featurepack/docking/CPane.cpp       CreateDefaultMiniframe@CPane (stub today)
//   featurepack/docking/RuntimeClasses.cpp / taskspane/RuntimeClasses.cpp
//                                       GetThisClass@CMultiPaneFrameWnd /
//                                       GetThisClass@CMFCTasksPaneFrameWnd
//   featurepack/menu/StaticData.cpp     m_nLastCommandIndex@CMFCPopupMenuBar
//   featurepack/CMFC_misc_stubs.cpp     g_pTopLevelFrame (exported data)
//   featurepack/taskspane/CMFCTasksPaneToolBar.cpp  UpdateButtons (stub today,
//                                       placeholder list without `this`)
//   featurepack/taskspane/CMFCTasksPaneTask.cpp / CMFCTasksPanePropertyPage.cpp
//                                       SetACCData overrides (vtable slot 5)
class CRecentFileList;   // only ever passed through (core/app/CRecentFileList.cpp keeps its state in a side table)
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(
    const CObList* pThis, long long nIndex);
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(
    CObList* pThis, CObject* value);
extern "C" void MS_ABI impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(CObList* pThis, CObList::POSITION* pPos);
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
extern "C" CStringList::POSITION MS_ABI
    impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        CStringList* pThis, const CString& value);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" const MSG* MS_ABI impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
extern "C" int MS_ABI impl__ShowWindow_CWnd__QEAAHH_Z(CWnd* pThis, int nCmdShow);
extern "C" int MS_ABI impl__SetScrollPos_CWnd__QEAAHHHH_Z(CWnd* pThis, int nBar, int nPos, int bRedraw);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__Attach_CGdiObject__QEAAHPEAX_Z(CGdiObject* pThis, HGDIOBJ hObject);
extern "C" int MS_ABI impl__DeleteObject_CGdiObject__QEAAHXZ(CGdiObject* pThis);
extern "C" CGdiObject* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject);
extern "C" void MS_ABI impl__OnCancelMode_CDockablePane__IEAAXXZ(CDockablePane* pThis);
extern "C" void* MS_ABI impl__CreateDefaultMiniframe_CPane__UEAAPEAVCPaneFrameWnd__VCRect___Z(
    CPane* pThis, const RECT* pRectInitial);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" std::int32_t impl__m_nLastCommandIndex_CMFCPopupMenuBar__1HA;
extern "C" void* impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;
extern "C" void MS_ABI impl__UpdateButtons_CMFCTasksPaneToolBar__QEAAXXZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPaneTask__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData);
extern "C" int MS_ABI impl__SetACCData_CMFCTasksPanePropertyPage__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, void* pWindow, void* pData);
// Sibling exports defined further down in THIS file (retail-correct lists; the
// placeholder definitions below were rewritten to match).
extern "C" void MS_ABI impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis);
extern "C" int MS_ABI impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(CMFCTasksPane* pThis, int bCalcHeightOnly);
extern "C" void MS_ABI impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis);
extern "C" int MS_ABI impl__CreateNavigationToolbar_CMFCTasksPane__IEAAHXZ(CMFCTasksPane* pThis);
extern "C" void MS_ABI impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(CMFCTasksPane* pThis, int nNewPageHistIdx, int nOldPageHistIdx);
extern "C" void* MS_ABI impl__GetTaskGroup_CMFCTasksPane__QEBAPEAVCMFCTasksPaneTaskGroup__H_Z(const CMFCTasksPane* pThis, int nGroup);
extern "C" void MS_ABI impl__UpdateCaption_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis);
extern "C" void MS_ABI impl__SaveHistory_CMFCTasksPane__IEAAXH_Z(CMFCTasksPane* pThis, int nPage);
extern "C" void MS_ABI impl__RemoveAll_CStringList__QEAAXXZ(CStringList* pThis);
// Added by review (definitions: this file / docking/CDockablePane.cpp /
// core/window/CMenu.cpp / core/window/Thunks.cpp / docking/CBasePane.cpp).
extern "C" void MS_ABI impl__SetActivePage_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nPage);
extern "C" void MS_ABI impl__CreateFonts_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis);
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCTasksPane__QEBAPEAUHMENU____XZ(const CMFCTasksPane* pThis);
extern "C" void MS_ABI impl__OnPressOtherButton_CMFCTasksPane__UEAAXPEAVCMFCCaptionMenuButton__PEAVCWnd___Z(
    CMFCTasksPane* pThis, void* pbtn, CWnd* pWndOwner);
extern "C" void* MS_ABI impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(
    const CDockablePane* pThis, unsigned int nHit);
extern "C" int MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu* pThis);
extern "C" int MS_ABI impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(CMenu* pThis, HMENU hMenu);
extern "C" void MS_ABI impl__OnSize_CBasePane__IEAAXIHH_Z(CBasePane* pThis, unsigned int nType, int cx, int cy);
extern "C" int MS_ABI impl__GetDisplayName_CRecentFileList__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEB_WHH_Z(
    const CRecentFileList* pThis, CString* stringOut, int index, const wchar_t* curDir, int nCurDir, int bAtLeastName);
extern "C" int MS_ABI impl__GetMRUFileName_CMFCTasksPane__MEAAHPEAVCRecentFileList__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCTasksPane* pThis, CRecentFileList* pRecentFileList, int nIndex, CString& strName);
// Added by the setter/layout batch.  Every one of these is defined either
// further down in THIS file (the GetTaskLocation overloads, ShowTask, the
// three-argument SetWindowHeight, the HIMAGELIST SetIconsList) or in
// featurepack/docking/CDockablePane.cpp / core/runtime/Globals.cpp; the
// parameter lists are the ones the mangled names describe, and the
// definitions below were rewritten to match where a placeholder list
// disagreed.
extern "C" int MS_ABI impl__GetTaskLocation_CMFCTasksPane__QEBAHIAEAH0_Z(
    const CMFCTasksPane* pThis, unsigned int uiCommandID, int* pnGroup, int* pnTask);
extern "C" int MS_ABI impl__GetTaskLocation_CMFCTasksPane__QEBAHPEAUHWND____AEAH1_Z(
    const CMFCTasksPane* pThis, HWND hwndTask, int* pnGroup, int* pnTask);
extern "C" int MS_ABI impl__ShowTask_CMFCTasksPane__QEAAHHHHH_Z(
    CMFCTasksPane* pThis, int nGroup, int nTask, int bShow, int bRedraw);
extern "C" int MS_ABI impl__SetWindowHeight_CMFCTasksPane__QEAAHHPEAUHWND____H_Z(
    CMFCTasksPane* pThis, int nGroup, HWND hwndTask, int nWndHeight);
extern "C" void MS_ABI impl__SetIconsList_CMFCTasksPane__QEAAXPEAU_IMAGELIST___Z(
    CMFCTasksPane* pThis, HIMAGELIST hIcons);
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(CDockablePane* pThis);
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);   // core/runtime/Globals.cpp

namespace {

typedef S_MfcFeature20Impl TP;                       // retail CMFCTasksPane view
typedef S_Cmfctaskspanetaskgroup TPGroup;            // retail CMFCTasksPaneTaskGroup
typedef S_Cmfctaskspanetask TPTask;                  // retail CMFCTasksPaneTask
typedef S_Cmfctaskspanepropertypage TPPage;          // retail CMFCTasksPanePropertyPage

static_assert(offsetof(TP, m_lstTasksPanes)  == 0x608, "m_lstTasksPanes @0x608");
static_assert(offsetof(TP, m_lstTaskGroups)  == 0x640, "m_lstTaskGroups @0x640");
static_assert(offsetof(TP, m_arrHistoryVfptr) == 0x778, "m_arrHistoryStack @0x778");
static_assert(offsetof(TP, m_nHistoryMaxSize) == 0x790, "m_arrHistoryStack.m_nMaxSize @0x790");
static_assert(offsetof(TP, m_nHistoryGrowBy)  == 0x798, "m_arrHistoryStack.m_nGrowBy @0x798");
static_assert(offsetof(TP, m_wndToolBar)     == 0x7B8, "m_wndToolBar @0x7B8");
static_assert(offsetof(TP, m_wndScrollVert)  == 0x680, "m_wndScrollVert @0x680");
static_assert(offsetof(TP, m_fonts)          == 0x558, "m_fontBold @0x558");
static_assert(offsetof(TPGroup, m_lstTasks)  == 0x18, "CMFCTasksPaneTaskGroup::m_lstTasks @0x18");
static_assert(offsetof(TPGroup, m_bIsCollapsed) == 0x58, "CMFCTasksPaneTaskGroup::m_bIsCollapsed @0x58");
static_assert(offsetof(TPGroup, m_rect)      == 0x5c, "CMFCTasksPaneTaskGroup::m_rect @0x5c");
static_assert(offsetof(TPTask, m_strName)    == 0x10, "CMFCTasksPaneTask::m_strName @0x10");
static_assert(offsetof(TPTask, m_uiCommandID) == 0x30, "CMFCTasksPaneTask::m_uiCommandID @0x30");
static_assert(offsetof(TPTask, m_hwndTask)   == 0x40, "CMFCTasksPaneTask::m_hwndTask @0x40");
static_assert(offsetof(TPPage, m_strName)    == 0x8,  "CMFCTasksPanePropertyPage::m_strName @8");
static_assert(sizeof(TPTask) == 0x68 && sizeof(TPPage) == 0x18 && sizeof(TPGroup) == 0x98,
              "retail record sizes (operator new sizes read at 0x146c8c / 0x145a58 / 0x145f62)");
// Added by review: offsets the setter/layout batch reads that the shadow
// headers' own static_asserts do not pin (values read from the mfc140
// disassembly cited at each body below).
static_assert(offsetof(TP, m_bMenuBtnPressed) == 0x4F8, "m_bMenuBtnPressed @0x4F8 (StopCaptionButtonsTracking)");
static_assert(offsetof(TP, m_hFont)          == 0x550, "m_hFont @0x550 (SetFont)");
static_assert(offsetof(TP, m_sizeIcon)       == 0x588, "m_sizeIcon @0x588 (SetIconsList)");
static_assert(offsetof(TP, m_lstIcons)       == 0x768, "m_lstIcons @0x768, m_hImageList at +8 (SetIconsList)");
static_assert(offsetof(TP, m_rectTasks)      == 0x5D8, "m_rectTasks @0x5D8 (TaskHitTest)");
static_assert(offsetof(TPTask, m_nWindowHeight) == 0x2c, "CMFCTasksPaneTask::m_nWindowHeight @0x2c");
static_assert(offsetof(TPTask, m_bVisible)   == 0x4c, "CMFCTasksPaneTask::m_bVisible @0x4c");
static_assert(offsetof(TPTask, m_clrText)    == 0x5c && offsetof(TPTask, m_clrTextHot) == 0x60,
              "CMFCTasksPaneTask::m_clrText/m_clrTextHot @0x5c/0x60");
static_assert(offsetof(TPGroup, m_clrText)   == 0x90 && offsetof(TPGroup, m_clrTextHot) == 0x94,
              "CMFCTasksPaneTaskGroup::m_clrText/m_clrTextHot @0x90/0x94");
// CWnd::m_hWndOwner (retail CWnd +0xa0; see ShowCommandMessageString).
constexpr size_t kCWndOwnerOffset = 0xa0;
static_assert(offsetof(CWnd, _cwnd_padding2) <= kCWndOwnerOffset &&
              kCWndOwnerOffset + sizeof(HWND) <= offsetof(CWnd, _cwnd_padding2) + sizeof(CWnd::_cwnd_padding2),
              "m_hWndOwner slot must lie inside CWnd's zero-filled padding");

inline TP* View(CMFCTasksPane* p)             { return reinterpret_cast<TP*>(p); }
inline const TP* View(const CMFCTasksPane* p) { return reinterpret_cast<const TP*>(p); }
inline CObList* GroupList(const CMFCTasksPane* p) {
    return reinterpret_cast<CObList*>(const_cast<char*>(View(p)->m_lstTaskGroups));
}
inline CObList* PageList(const CMFCTasksPane* p) {
    return reinterpret_cast<CObList*>(const_cast<char*>(View(p)->m_lstTasksPanes));
}
inline CObList* TaskList(const TPGroup* g) {
    return reinterpret_cast<CObList*>(const_cast<char*>(g->m_lstTasks));
}

// Mirror of CList<CObject*,CObject*>::CNode -- what a CObList::POSITION points
// at (pNext / pPrev / data), identical to retail's node layout.
struct TpNode {
    TpNode*  pNext;
    TpNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

inline TpNode* NodeFromPos(CObList::POSITION pos) {
    TpNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}
inline CObList::POSITION PosFromNode(TpNode* node) {
    CObList::POSITION pos;
    std::memcpy(&pos, &node, sizeof(node));
    return pos;
}
// Retail: `if (n >= m_nCount || n < 0) ...; node = head; while (n--) node = node->pNext;`
// The FindIndex thunk performs the same bounds test and walk and returns NULL
// when the index is out of range.
inline TpNode* NodeAt(const CObList* list, long long n) {
    if (n < 0) return nullptr;
    return NodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(list, n));
}
inline TpNode* HeadNode(const CObList* list) { return NodeAt(list, 0); }
// Replaces retail's direct read of CObList::m_nCount (see the note above).
inline long long ListCount(const CObList* list) {
    long long n = 0;
    for (TpNode* p = HeadNode(list); p != nullptr; p = p->pNext) ++n;
    return n;
}
inline void ListRemove(CObList* list, TpNode* node) {
    CObList::POSITION pos = PosFromNode(node);
    impl__RemoveAt_CObList__QEAAXPEAU__POSITION___Z(list, &pos);
}

// `delete pObj` in retail is a call through the object's vtable slot 1 (the
// MSVC vector-deleting destructor) with flags = 1.  Reproduced as such so that
// objects constructed by the client with its own vtable are destroyed by the
// client's destructor, and objects constructed here by ours.
typedef void* (MS_ABI* TpDeletingDtor)(void* pThis, unsigned int flags);
inline void DeleteViaVtable(void* pObj) {
    if (pObj == nullptr) return;
    void** vtbl = *reinterpret_cast<void***>(pObj);
    reinterpret_cast<TpDeletingDtor>(vtbl[1])(pObj, 1u);
}

// Retail's refresh tail, shared by CollapseGroup/RemoveGroup/RemoveTask/...:
//   AdjustScroll();                       // 0x148200 (mfc140u)
//   ReposTasks(FALSE);                    // vtable +0x7d0 = ReposTasks (slot 250;
//                                         //   the CMFCTasksPane vftable at
//                                         //   0x1803135b8 holds 0x144840 there,
//                                         //   whose body is the ReposTasks
//                                         //   export; called with %edx = 0)
//   RedrawWindow(NULL, NULL, flags);      // USER32!RedrawWindow via m_hWnd
// Deviation: the virtual is called as this class's own export (OpenMFC does
// not reproduce the retail vtable), so a derived override is not honoured.
// Both siblings are still stubs in this file.
inline void RetailRefresh(CMFCTasksPane* pThis, unsigned int rdwFlags) {
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
    ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, rdwFlags);
}

// Retail's `if (bCollapse) { if (m_bIsCollapsed) skip; } else { if (!m_bIsCollapsed) skip; }
// m_bIsCollapsed = bCollapse;` (0x1463fc..0x14640b and the same shape inside
// both CollapseAllGroups loops).
inline void ApplyCollapse(TPGroup* g, int bCollapse) {
    if (bCollapse != 0) {
        if (g->m_bIsCollapsed != 0) return;
    } else {
        if (g->m_bIsCollapsed == 0) return;
    }
    g->m_bIsCollapsed = bCollapse;
}

// --- Local MSVC-layout vtables for the records this file allocates ----------
// Retail allocates tasks (AddWindow, AddMRUFilesList) and pages (AddPage) with
// operator new and inline constructors; neither class exports a constructor,
// so the vtables are authored here.  Their retail shape (page vftable at
// mfc140u 0x180314cb8, task vftable at 0x180314c48) is six slots:
//   0 CObject::GetRuntimeClass   (neither class has DECLARE_DYNAMIC; slot 0 of
//                                 both retail vftables is CObject's, 0x37a0)
//   1 deleting destructor
//   2 Serialize / 3 AssertValid / 4 Dump  (retail: the shared empty body 0x27d0)
//   5 SetACCData                  (the exported override)
void MS_ABI TpEmptyVirtual(void*) {}
void MS_ABI TpEmptyVirtual1(void*, void*) {}

void* MS_ABI TpTaskDeletingDtor(void* p, unsigned int flags);
void* MS_ABI TpPageDeletingDtor(void* p, unsigned int flags);

void* const g_TpTaskVtbl[6] = {
    reinterpret_cast<void*>(&impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),
    reinterpret_cast<void*>(&TpTaskDeletingDtor),
    reinterpret_cast<void*>(&TpEmptyVirtual1),   // Serialize(CArchive&)
    reinterpret_cast<void*>(&TpEmptyVirtual),    // AssertValid()
    reinterpret_cast<void*>(&TpEmptyVirtual1),   // Dump(CDumpContext&)
    reinterpret_cast<void*>(&impl__SetACCData_CMFCTasksPaneTask__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z),
};
void* const g_TpPageVtbl[6] = {
    reinterpret_cast<void*>(&impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ),
    reinterpret_cast<void*>(&TpPageDeletingDtor),
    reinterpret_cast<void*>(&TpEmptyVirtual1),
    reinterpret_cast<void*>(&TpEmptyVirtual),
    reinterpret_cast<void*>(&TpEmptyVirtual1),
    reinterpret_cast<void*>(&impl__SetACCData_CMFCTasksPanePropertyPage__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z),
};

// CMFCTasksPaneTask::~CMFCTasksPaneTask -- retail body at mfc140u 0x1431cc:
//   if (m_hwndTask != NULL && m_bAutoDestroyWindow) {
//       CWnd* pWnd = CWnd::FromHandlePermanent(m_hwndTask);      // 0x28adc0
//       if (pWnd != NULL) { pWnd->DestroyWindow(); delete pWnd; } // vslots 0xd0 / 0x8
//       else ::DestroyWindow(m_hwndTask);
//   }
//   m_pGroup = NULL;  m_strName.~CString();
// Deviation: `delete pWnd` is NOT reproduced.  The permanent wrapper may have
// been constructed by the client with an MSVC-layout vtable OpenMFC cannot
// index, so the wrapper object is leaked rather than destroyed through the
// wrong slot.  DestroyWindow goes through the exported thunk instead of the
// vtable for the same reason.
void* MS_ABI TpTaskDeletingDtor(void* p, unsigned int flags) {
    TPTask* t = static_cast<TPTask*>(p);
    *reinterpret_cast<void* const**>(p) = g_TpTaskVtbl;
    if (t->m_hwndTask != nullptr && t->m_bAutoDestroyWindow != 0) {
        CWnd* pWnd = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(t->m_hwndTask);
        if (pWnd != nullptr) {
            impl__DestroyWindow_CWnd__UEAAHXZ(pWnd);
            // TODO(clean-room): retail `delete pWnd` (vslot 1) not reproduced.
        } else {
            ::DestroyWindow(t->m_hwndTask);
        }
    }
    t->m_pGroup = nullptr;
    reinterpret_cast<CString*>(&t->m_strName)->~CString();
    if (flags & 1u) std::free(p);
    return p;
}

// CMFCTasksPanePropertyPage::~CMFCTasksPanePropertyPage (afxtaskspane.h,
// inline): `m_pTaskPane = NULL;` then the m_strName CString destructor.
void* MS_ABI TpPageDeletingDtor(void* p, unsigned int flags) {
    TPPage* pg = static_cast<TPPage*>(p);
    *reinterpret_cast<void* const**>(p) = g_TpPageVtbl;
    pg->m_pTaskPane = nullptr;
    reinterpret_cast<CString*>(&pg->m_strName)->~CString();
    if (flags & 1u) std::free(p);
    return p;
}

// CMFCTasksPaneTask::CMFCTasksPaneTask(CMFCTasksPaneTaskGroup*, LPCTSTR, int
// nIcon, UINT uiCommandID, DWORD_PTR dwUserData, HWND hwndTask, BOOL
// bAutoDestroyWindow, int nWindowHeight) -- the inline constructor retail
// instantiates at mfc140u 0x1430c4 (called from AddWindow 0x146c40 and
// AddMRUFilesList 0x147c20):
//   vfptr = task vftable; m_strName = lpszName ? lpszName : L"";
//   m_rect = {0}; m_pGroup = pGroup; m_nIcon = nIcon; m_uiCommandID;
//   m_dwUserData; m_hwndTask; m_bAutoDestroyWindow; m_nWindowHeight;
//   m_bVisible = TRUE; m_bEnabled = TRUE; m_bIsSeparator = (lpszName == NULL);
//   m_clrText = m_clrTextHot = (COLORREF)-1; m_bIsBold = FALSE;
//   SetRectEmpty(&m_rect);
// Returns NULL when the allocation fails (retail then AddTail's NULL).
// Storage: retail allocates through MFC's exported operator new (malloc-backed)
// and its deleting destructors call CRT free (e.g. 0x1443b3); malloc/free here.
TPTask* NewTask(TPGroup* pGroup, const wchar_t* lpszName, int nIcon, unsigned int uiCommandID,
                unsigned __int64 dwUserData, HWND hwndTask, int bAutoDestroyWindow, int nWindowHeight) {
    TPTask* t = static_cast<TPTask*>(std::malloc(sizeof(TPTask)));
    if (t == nullptr) return nullptr;
    std::memset(t, 0, sizeof(TPTask));
    *reinterpret_cast<void* const**>(t) = g_TpTaskVtbl;
    new (&t->m_strName) CString(lpszName != nullptr ? lpszName : L"");
    t->m_pGroup             = pGroup;
    t->m_nIcon              = nIcon;
    t->m_nWindowHeight      = nWindowHeight;
    t->m_uiCommandID        = uiCommandID;
    t->m_dwUserData         = static_cast<DWORD_PTR>(dwUserData);
    t->m_hwndTask           = hwndTask;
    t->m_bAutoDestroyWindow = bAutoDestroyWindow;
    t->m_bVisible           = TRUE;
    t->m_bEnabled           = TRUE;
    t->m_bIsSeparator       = (lpszName == nullptr) ? TRUE : FALSE;
    t->m_bIsBold            = FALSE;
    t->m_clrText            = static_cast<COLORREF>(-1);
    t->m_clrTextHot         = static_cast<COLORREF>(-1);
    ::SetRectEmpty(&t->m_rect);
    return t;
}

// CArray<int,int>::SetSize(nNewSize, -1) as instantiated in mfc140u at
// 0x15034 (the m_arrHistoryStack helper every page routine calls), transcribed:
//   if (n < 0) AfxThrowInvalidArgException();
//   if (n == 0) { if (m_pData) { free(m_pData); m_pData = NULL; } m_nSize = m_nMaxSize = 0; }
//   else if (m_pData == NULL) { max = max(n, m_nGrowBy); m_pData = new(max*4) zeroed; m_nSize = n; m_nMaxSize = max; }
//   else if (n <= m_nMaxSize) { zero the tail past m_nSize; m_nSize = n; }
//   else { grow = m_nGrowBy ? m_nGrowBy : clamp(m_nSize/8, 4, 1024);
//          newMax = m_nMaxSize + grow; if (n >= newMax) newMax = n;
//          p = new(newMax*4); memcpy old m_nSize ints; zero the rest; free(m_pData);
//          m_pData = p; m_nSize = n; m_nMaxSize = newMax; }
// Retail pairs MFC's operator new with CRT free; here malloc/free are paired.
void HistorySetSize(TP* self, long long n) {
    if (n < 0) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    int*& data = *reinterpret_cast<int**>(reinterpret_cast<char*>(self) + offsetof(TP, m_pHistoryData));   // m_pData (non-const in retail)
    if (n == 0) {
        if (data != nullptr) { std::free(data); data = nullptr; }
        self->m_nHistoryMaxSize = 0;
        self->m_nHistorySize = 0;
        return;
    }
    if (data == nullptr) {
        long long mx = self->m_nHistoryGrowBy;
        if (n > mx) mx = n;
        data = static_cast<int*>(std::malloc(static_cast<size_t>(mx) * sizeof(int)));
        if (data != nullptr) std::memset(data, 0, static_cast<size_t>(mx) * sizeof(int));
        self->m_nHistorySize = n;
        self->m_nHistoryMaxSize = mx;
        return;
    }
    if (n <= self->m_nHistoryMaxSize) {
        if (n > self->m_nHistorySize) {
            std::memset(data + self->m_nHistorySize, 0,
                        static_cast<size_t>(n - self->m_nHistorySize) * sizeof(int));
        }
        self->m_nHistorySize = n;
        return;
    }
    long long grow = self->m_nHistoryGrowBy;
    if (grow == 0) {
        grow = self->m_nHistorySize / 8;
        if (grow > 1024) grow = 1024;
        if (grow < 4) grow = 4;
    }
    long long newMax = self->m_nHistoryMaxSize + grow;
    if (n >= newMax) newMax = n;
    int* p = static_cast<int*>(std::malloc(static_cast<size_t>(newMax) * sizeof(int)));
    if (p == nullptr) return;
    std::memcpy(p, data, static_cast<size_t>(self->m_nHistorySize) * sizeof(int));
    std::memset(p + self->m_nHistorySize, 0,
                static_cast<size_t>(n - self->m_nHistorySize) * sizeof(int));
    std::free(data);
    data = p;
    self->m_nHistorySize = n;
    self->m_nHistoryMaxSize = newMax;
}
// CArray<int,int>::Add(v): SetSize(m_nSize + 1); m_pData[old] = v  (retail
// inlines it as `n = m_nSize; if (n < 0) throw; SetSize(n + 1); m_pData[n] = v`).
void HistoryAdd(TP* self, int v) {
    long long n = self->m_nHistorySize;
    if (n < 0) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
    HistorySetSize(self, n + 1);
    if (self->m_pHistoryData != nullptr && self->m_nHistorySize > n) {
        const_cast<int*>(self->m_pHistoryData)[n] = v;
    }
}

// CArray<int,int>::RemoveAt(nIndex, nCount) as instantiated in mfc140u at
// 0x1b880 (called from SaveHistory 0x1488c0):
//   ENSURE(nIndex >= 0 && nCount >= 0 && nIndex + nCount <= m_nSize);
//   if (m_nSize - (nIndex + nCount) > 0)
//       memmove(&m_pData[nIndex], &m_pData[nIndex + nCount], moveCount * 4);
//   m_nSize -= nCount;
void HistoryRemoveAt(TP* self, long long nIndex, long long nCount) {
    if (nIndex < 0 || nCount < 0 || nIndex + nCount > self->m_nHistorySize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    const long long moveCount = self->m_nHistorySize - (nIndex + nCount);
    if (moveCount > 0 && self->m_pHistoryData != nullptr) {
        int* data = const_cast<int*>(self->m_pHistoryData);
        std::memmove(data + nIndex, data + nIndex + nCount, static_cast<size_t>(moveCount) * sizeof(int));
    }
    self->m_nHistorySize -= nCount;
}

// Reads m_arrHistoryStack[idx] with retail's own `idx >= 0 && idx < m_nSize`
// guard (every reader below throws AfxThrowInvalidArgException on failure,
// exactly like the inlined CArray::GetAt/operator[] ENSURE does).
inline bool HistoryAt(const TP* self, long long idx, int& out) {
    if (idx < 0 || idx >= self->m_nHistorySize || self->m_pHistoryData == nullptr) return false;
    out = self->m_pHistoryData[idx];
    return true;
}

} // namespace

// Symbol: ?AddGroup@CMFCTasksPane@@QEAAHHPEB_WHHPEAUHICON__@@@Z
extern "C" int MS_ABI impl__AddGroup_CMFCTasksPane__QEAAHHPEB_WHHPEAUHICON_____Z(
    CMFCTasksPane* pThis, int nGroup, const wchar_t* lpszName, int bBottomHasGripper, int bSpecial, void* hIcon) {
    return pThis ? pThis->AddGroup(nGroup, lpszName, bBottomHasGripper ? TRUE : FALSE, bSpecial ? TRUE : FALSE,
                                     reinterpret_cast<HICON>(hIcon))
                 : -1;
}
// Symbol: ?RemoveAllTasks@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RemoveAllTasks_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nGroup) {
    if (pThis) pThis->RemoveAllTasks(nGroup);
}
// Symbol: ?SetCaption@CMFCTasksPane@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetCaption_CMFCTasksPane__QEAAXPEB_W_Z(CMFCTasksPane* pThis, const wchar_t* lpszCaption) {
    if (pThis) pThis->SetCaption(lpszCaption);
}
// CMFCTasksPane::OnPressBackButton -- transcribed from mfc140u RVA 0x148d60:
//   if (m_iActivePage > 0) { nOld = m_iActivePage; m_iActivePage = nOld - 1;
//                            ChangeActivePage(m_iActivePage, nOld); }   // 0x148990
// (Review: the earlier body approximated the ChangeActivePage call with a
// pane-wide redraw; ChangeActivePage is implemented below, so it is called.)
// Symbol: ?OnPressBackButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    TP* self = View(pThis);
    if (self->m_iActivePage > 0) {
        const int nOld = self->m_iActivePage;
        self->m_iActivePage = nOld - 1;
        impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
    }
}
// CMFCTasksPane::OnPressForwardButton -- transcribed from mfc140u RVA 0x148d90:
//   if (m_iActivePage < m_arrHistoryStack.m_nSize - 1) {         // +0x504 / +0x788
//       nOld = m_iActivePage; m_iActivePage = nOld + 1;
//       ChangeActivePage(m_iActivePage, nOld); }                  // 0x148990
// Symbol: ?OnPressForwardButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    TP* self = View(pThis);
    if (static_cast<__int64>(self->m_iActivePage) < self->m_nHistorySize - 1) {
        const int nOld = self->m_iActivePage;
        self->m_iActivePage = nOld + 1;
        impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
    }
}
// CMFCTasksPane::OnPressHomeButton -- transcribed from mfc140u RVA 0x148dc0:
//   ENSURE(0 <= m_iActivePage < m_arrHistoryStack.m_nSize)  else AfxThrowInvalidArgException
//   if (m_arrHistoryStack[m_iActivePage] != 0) SetActivePage(0);   // 0x148b00
// (0x148b00 is the exported ?SetActivePage@CMFCTasksPane@@QEAAXH@Z -- its
// ANSI twin 0x147170 carries the name; the u map does not.  The earlier body
// wrote m_iActivePage = 0 directly, which is not what SetActivePage does.)
// Symbol: ?OnPressHomeButton@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnPressHomeButton_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    TP* self = View(pThis);
    int nCur = 0;
    if (!HistoryAt(self, self->m_iActivePage, nCur)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (nCur != 0) {
        impl__SetActivePage_CMFCTasksPane__QEAAXH_Z(pThis, 0);
    }
}
// CMFCTasksPane::OnPressButtons(UINT nHit) -- transcribed from mfc140u RVA
// 0x148ce0:
//   switch (nHit) {
//   case 0x17: OnPressBackButton();    return;   // vtable +0x750
//   case 0x18: OnPressForwardButton(); return;   // vtable +0x758
//   case 0x19:
//       CMFCCaptionMenuButton* pBtn = FindButtonByHit(0x19);   // 0x453c0 =
//                               // ?FindButtonByHit@CDockablePane@@IEBAPEAVCMFCCaptionButton@@I@Z
//       if (pBtn == NULL) return;
//       m_bMenuBtnPressed = TRUE;                             // +0x4f8
//       OnPressOtherButton(pBtn, this);                       // vtable +0x768
//       m_bMenuBtnPressed = FALSE;
//   }
// (Review: an earlier comment named 0x453c0 "AfxFindResourceHandle"; it is
// CDockablePane::FindButtonByHit, whose OpenMFC body is a stub returning NULL
// because m_arrButtons has no storage -- see docking/CDockablePane.cpp -- so
// the 0x19 branch currently ends at the NULL test.)  The two virtuals are
// called as this class's own exports.
// Symbol: ?OnPressButtons@CMFCTasksPane@@MEAAXI@Z
extern "C" void MS_ABI impl__OnPressButtons_CMFCTasksPane__MEAAXI_Z(
    CMFCTasksPane* pThis, unsigned int nHit)
{
    if (!pThis) return;
    switch (nHit) {
    case 0x17:
        impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(pThis);
        break;
    case 0x18:
        impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(pThis);
        break;
    case 0x19: {
        void* pBtn = impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(
            reinterpret_cast<const CDockablePane*>(pThis), 0x19u);
        if (pBtn == nullptr) break;
        TP* self = View(pThis);
        self->m_bMenuBtnPressed = TRUE;
        impl__OnPressOtherButton_CMFCTasksPane__UEAAXPEAVCMFCCaptionMenuButton__PEAVCWnd___Z(pThis, pBtn, pThis);
        self->m_bMenuBtnPressed = FALSE;
        break;
    }
    default:
        break;
    }
}
// CMFCTasksPane::OnForward -- transcribed from mfc140u RVA 0x14b1e0:
//   if (m_bHistoryMenuButtons) {                                    // +0x4e8
//       int n = CMFCPopupMenuBar::m_nLastCommandIndex;              // 0x3b1b3c (exported static)
//       if (n >= 0 && n < (int)m_arrHistoryStack.m_nSize - m_iActivePage - 1) {
//           int nOld = m_iActivePage;
//           m_iActivePage = nOld + n + 1;
//           ChangeActivePage(m_iActivePage, nOld);                  // tail jmp 0x148990
//           return;
//       }
//   }
//   OnPressForwardButton();                                         // tail jmp via vtable +0x758
// (Review: an earlier comment called 0x1803b1b3c "internal MFC state"; it is
// the exported ?m_nLastCommandIndex@CMFCPopupMenuBar@@1HA, defined in
// featurepack/menu/StaticData.cpp, so the history branch is reproduced.)
// Deviation: the virtual is called as this class's own export.
// Symbol: ?OnForward@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnForward_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    TP* self = View(pThis);
    if (self->m_bHistoryMenuButtons != 0) {
        const int n = impl__m_nLastCommandIndex_CMFCPopupMenuBar__1HA;
        if (n >= 0 && n < static_cast<int>(self->m_nHistorySize) - self->m_iActivePage - 1) {
            const int nOld = self->m_iActivePage;
            self->m_iActivePage = nOld + n + 1;
            impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
            return;
        }
    }
    impl__OnPressForwardButton_CMFCTasksPane__UEAAXXZ(pThis);
}
// CMFCTasksPane::OnHome -- retail (RVA 0x14b230) is a pure tail call to
// vtable slot 0x760, which points at OnPressHomeButton; reproduced directly.
// Symbol: ?OnHome@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnHome_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    impl__OnPressHomeButton_CMFCTasksPane__UEAAXXZ(pThis);
}
// CMFCTasksPane::OnOther -- transcribed from mfc140u RVA 0x14b270, whose whole
// body is `SetActivePage(CMFCPopupMenuBar::m_nLastCommandIndex)` (a tail jump
// to 0x148b00 = the exported SetActivePage with %edx loaded from the exported
// static at 0x3b1b3c).  (Review: the earlier comment called both "internal".)
// Symbol: ?OnOther@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnOther_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
    impl__SetActivePage_CMFCTasksPane__QEAAXH_Z(pThis, impl__m_nLastCommandIndex_CMFCPopupMenuBar__1HA);
}
// CMFCTasksPane::OnClose -- retail (RVA 0x14b250) is a pure tail call to
// vtable slot 0x5c8 (a close/visibility virtual not exported by OpenMFC).
// TODO(clean-room): partially transcribed -- vtable slot 0x5c8 is not
// modeled.
// Symbol: ?OnClose@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnEraseBkgnd(CDC*) -- retail (RVA 0x3a60) is literally
// `mov $0x1,%eax; ret` (always erase).
// Symbol: ?OnEraseBkgnd@CMFCTasksPane@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCTasksPane__IEAAHPEAVCDC___Z(
    CMFCTasksPane* pThis, CDC* /*pDC*/)
{
    if (!pThis) return FALSE;
    return TRUE;
}
// CMFCTasksPane::OnGetFont() -- retail (RVA 0x146ea0) returns m_hFont (0x550)
// when set, otherwise GetStockObject(DEFAULT_GUI_FONT) (0x11 via IAT
// 0x1802c6250).
// Symbol: ?OnGetFont@CMFCTasksPane@@IEAAPEAUHFONT__@@XZ
extern "C" HFONT MS_ABI impl__OnGetFont_CMFCTasksPane__IEAAPEAUHFONT____XZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return nullptr;
    S_MfcFeature20Impl* self = reinterpret_cast<S_MfcFeature20Impl*>(pThis);
    if (self->m_hFont != nullptr) {
        return self->m_hFont;
    }
    return static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
}
// CMFCTasksPane::OnSetFont(CFont* pFont, BOOL) -- transcribed from mfc140u
// RVA 0x146e50:
//   m_hFont = pFont ? pFont->m_hObject : NULL;      // +0x550, CGdiObject +8
//   CreateFonts();                                  // 0x146ec0 (this file)
//   AdjustScroll();                                 // 0x148200
//   ReposTasks(FALSE);                              // tail jmp via vtable +0x7d0
// (Review: an earlier comment called slot +0x7d0 "the virtual RecalcLayout";
// the CMFCTasksPane vftable holds ReposTasks there -- see RetailRefresh.)
// bRedraw is not read.
// Symbol: ?OnSetFont@CMFCTasksPane@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCTasksPane__IEAAXPEAVCFont__H_Z(
    CMFCTasksPane* pThis, CFont* pFont, int /*bRedraw*/)
{
    if (!pThis) return;
    TP* self = View(pThis);
    self->m_hFont = pFont
        ? *reinterpret_cast<HFONT*>(reinterpret_cast<char*>(pFont) + 8)
        : nullptr;
    impl__CreateFonts_CMFCTasksPane__IEAAXXZ(pThis);
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
}
// CMFCTasksPane::OnSetText(const wchar_t*) -- retail (RVA 0x14bb00) first
// calls the base CWnd::OnSetText (0x18028ac80) and propagates its return
// value, then copies the text into m_strCaption (0x678, CString). The base
// handler is not exported, so it is approximated with SetWindowTextW on
// m_hWnd. m_strCaption is left untouched: writing an alien heap pointer into
// the retail CString slot would be freed by the retail CString destructor
// with the wrong allocator.
// TODO(clean-room): partially transcribed -- m_strCaption is not updated.
// Symbol: ?OnSetText@CMFCTasksPane@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMFCTasksPane__IEAAHPEB_W_Z(
    CMFCTasksPane* pThis, const wchar_t* lpszText)
{
    if (!pThis) return FALSE;
    if (pThis->m_hWnd != nullptr) {
        return ::SetWindowTextW(pThis->m_hWnd, lpszText ? lpszText : L"");
    }
    return FALSE;
}
// CMFCTasksPane::OnSetAccData(int) -- retail (RVA 0x14bb50) screen-to-client
// converts the packed hit point, hit-tests a task or group caption and calls
// the target object's SetACCData virtual (vtable slot 0x28) to fill
// pThis->m_AccData (0x150). Before that it always resets the accessibility
// block: the six CStrings are emptied and the role/state/hit/rect/point
// numeric fields (0x150+0x30..+0x53) are zeroed. The task/group list walking
// needs object layout OpenMFC does not model; the accessibility reset and the
// TRUE result are reproduced faithfully.
// TODO(clean-room): partially transcribed -- the task/group SetACCData
// population is not reproduced.
// Symbol: ?OnSetAccData@CMFCTasksPane@@UEAAHJ@Z
extern "C" int MS_ABI impl__OnSetAccData_CMFCTasksPane__UEAAHJ_Z(
    CMFCTasksPane* pThis, int /*nAccData*/)
{
    if (!pThis) return FALSE;

    CAccessibilityData* acc = &pThis->m_AccData;
    acc->m_strAccName.Empty();
    acc->m_strAccValue.Empty();
    acc->m_strDescription.Empty();
    acc->m_strAccKeys.Empty();
    acc->m_strAccHelp.Empty();
    acc->m_strAccDefAction.Empty();
    acc->m_nAccRole = 0;
    acc->m_bAccState = 0;
    acc->m_nAccHit = 0;
    acc->m_rectAccLocation.SetRectEmpty();
    acc->m_ptAccHit = CPoint(0, 0);

    return TRUE;
}
// CMFCTasksPane::OnLButtonDown(unsigned int, CPoint) -- retail (RVA 0x147af0)
// tracks caption buttons, starts the scroll-button timers and performs
// hit-testing against the task/group geometry. The hit-test internals
// (0x147070) and the unmodeled group objects are required.
// TODO(clean-room): partially transcribed -- hit-testing and button tracking
// are not modeled.
// Symbol: ?OnLButtonDown@CMFCTasksPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCTasksPane__IEAAXIVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnLButtonUp(unsigned int, CPoint) -- retail (RVA 0x1476f0)
// releases mouse capture, dispatches clicks on group captions / tasks and
// sends WM_COMMAND / starts the animation timer (0xec0b). Requires the
// task/group objects and the command routing OpenMFC does not model.
// TODO(clean-room): partially transcribed -- click dispatch and animation
// timer are not modeled.
// Symbol: ?OnLButtonUp@CMFCTasksPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCTasksPane__IEAAXIVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnMouseMove(unsigned int, CPoint) -- retail (RVA 0x1472f0)
// updates the hot task / group caption highlight and changes the cursor.
// Requires the hit-test helper (0x147070) and the task/group geometry.
// TODO(clean-room): partially transcribed -- hot-tracking is not modeled.
// Symbol: ?OnMouseMove@CMFCTasksPane@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCTasksPane__IEAAXIVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnMouseWheel(unsigned int, short, CPoint) -- retail (RVA
// 0x149b90) returns 1 when the internal animation state (0x1803be288) is
// active, otherwise scrolls the vertical scroll-bar through the DoScroll
// helper (0x147f20) using m_nVertScrollTotal (0x514) and the scroll sub-window
// (0x680), then redraws and falls back to the base handler (0x18028ac80).
// Both the animation global and the scroll sub-window are unmodeled.
// TODO(clean-room): partially transcribed -- vertical scrolling is not
// modeled.
// Symbol: ?OnMouseWheel@CMFCTasksPane@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CMFCTasksPane__IEAAHIFVCPoint___Z(
    CMFCTasksPane* pThis, unsigned int /*nFlags*/, short /*zDelta*/, CPoint /*point*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCTasksPane::OnSetCursor(CWnd*, unsigned int, unsigned int) -- retail
// (RVA 0x147220) GetCursorPos/ScreenToClient hit-tests the task under the
// pointer, and when a clickable task or a collapsible group caption is hit
// sets the visual-manager cursor (visual-manager virtuals + global 0x1803c1620),
// otherwise defers to the base handler (0x18028ac80).
// TODO(clean-room): partially transcribed -- visual-manager cursor lookup is
// not modeled.
// Symbol: ?OnSetCursor@CMFCTasksPane@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCTasksPane__IEAAHPEAVCWnd__II_Z(
    CMFCTasksPane* pThis, CWnd* /*pWnd*/, unsigned int /*nHitTest*/,
    unsigned int /*message*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCTasksPane::OnTrackCaptionButtons(CPoint) -- retail (RVA 0x1498d0)
// hit-tests the caption buttons and toggles their pressed state.
// TODO(clean-room): partially transcribed -- caption-button hit-testing is
// not modeled.
// Symbol: ?OnTrackCaptionButtons@CMFCTasksPane@@MEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CMFCTasksPane__MEAAXVCPoint___Z(
    CMFCTasksPane* pThis, CPoint /*point*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnDrawTasks(CDC*, CRect) -- retail (RVA 0x145360) renders
// the task groups through the CMFCVisualManager virtuals (slots 0x2d8/0x2e0/
// 0x2f0/0x2f8/0x300/0x308, visual-manager accessor at 0x180009774) walking
// the group/task lists at 0x640/0x608.
// TODO(clean-room): partially transcribed -- task rendering is not modeled.
// Symbol: ?OnDrawTasks@CMFCTasksPane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawTasks_CMFCTasksPane__MEAAXPEAVCDC__VCRect___Z(
    CMFCTasksPane* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnFillBackground(CDC*, CRect) -- retail (RVA 0x1457d0)
// paints the pane background through the visual manager.
// TODO(clean-room): partially transcribed -- background rendering is not
// modeled.
// Symbol: ?OnFillBackground@CMFCTasksPane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCTasksPane__MEAAXPEAVCDC__VCRect___Z(
    CMFCTasksPane* pThis, CDC* /*pDC*/, CRect /*rect*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnPaint() -- retail (RVA 0x148620) constructs a CPaintDC,
// calls the draw virtual (vtable slot 0x4d8) and destroys it. No-op here:
// triggering a redraw from inside a paint cycle would repaint forever.
// TODO(clean-room): partially transcribed -- painting is not modeled.
// Symbol: ?OnPaint@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnCreate(CREATESTRUCTW*) -- retail (RVA 0x144530) registers
// the navigation messages (resource strings 0x427b/0x427c/0x427d/0x4280,
// RegisterWindowMessage via 0x18000db70), creates the scroll-bar sub-window
// (0x680), the toolbar (0x7B8), the history stack and the font objects, and
// returns 0 on success after the base OnCreate (0x1800403a0).
// TODO(clean-room): partially transcribed -- window/control creation is not
// modeled.
// Symbol: ?OnCreate@CMFCTasksPane@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCTasksPane__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCTasksPane* pThis, void* /*lpCreateStruct*/)
{
    if (!pThis) return -1;
    return 0;
}
// CMFCTasksPane::OnDestroy() -- retail (RVA 0x1487b0) deletes every group in
// m_lstTaskGroups (0x640, count 0x658) and then tail-calls the base
// CWnd::OnDestroy (0x180044fb0). The groups are CObject-derived objects
// OpenMFC never allocates, so they must not be deleted here.
// TODO(clean-room): partially transcribed -- group deletion is not modeled.
// Symbol: ?OnDestroy@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCTasksPane__IEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnNeedTipText(unsigned int, NMHDR*, __int64*) -- retail
// (RVA 0x149910) verifies the tooltip window, loads the tooltip strings
// (0x427c/0x427d/0x4280), registers a navigation message and writes the text
// pointer into pNMHDR->pszText (NMHDR+0x18). Requires the tooltip sub-window
// (m_pToolTip at 0x4B8) and the string-loader (0x1802aee00).
// TODO(clean-room): partially transcribed -- tooltip wiring is not modeled.
// Symbol: ?OnNeedTipText@CMFCTasksPane@@IEAAHIPEAUtagNMHDR@@PEA_J@Z
extern "C" int MS_ABI impl__OnNeedTipText_CMFCTasksPane__IEAAHIPEAUtagNMHDR__PEA_J_Z(
    CMFCTasksPane* pThis, unsigned int /*id*/, NMHDR* /*pNMHDR*/,
    __int64* /*pResult*/)
{
    if (!pThis) return 0;
    return 0;
}
// CMFCTasksPane::OnOK() -- retail (RVA 0x1499f0) walks the accessibility
// window hierarchy and posts WM_COMMAND to activate the focused task.
// TODO(clean-room): partially transcribed -- accessibility window traversal is
// not modeled.
// Symbol: ?OnOK@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnOK_CMFCTasksPane__UEAAXXZ(
    CMFCTasksPane* pThis)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnPressOtherButton(CMFCCaptionMenuButton*, CWnd*) -- retail
// (RVA 0x148e00) attaches m_menuOther (0x5E8) to the caption button, pops the
// menu, and navigates to the last history index when the button's flag
// (pbtn+0x38) is set. Needs the CMFCCaptionMenuButton layout and the internal
// navigation global.
// TODO(clean-room): partially transcribed -- menu popup is not modeled.
// Symbol: ?OnPressOtherButton@CMFCTasksPane@@UEAAXPEAVCMFCCaptionMenuButton@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnPressOtherButton_CMFCTasksPane__UEAAXPEAVCMFCCaptionMenuButton__PEAVCWnd___Z(
    CMFCTasksPane* pThis, void* /*pbtn*/, CWnd* /*pWndOwner*/)
{
    if (!pThis) return;
}
// CMFCTasksPane::OnSettingChange(UINT, LPCTSTR) -- retail mfc140u RVA 0x146ff0:
//   CWnd::OnSettingChange(uFlags, lpszSection);     // 0x28f380
//   AdjustScroll();                                 // 0x148200
//   ReposTasks(FALSE);                              // tail jmp via vtable +0x7d0
// Deviation: the base call is NOT reproduced.  OpenMFC's exported
// ?OnSettingChange@CWnd@@IEAAXIPEB_W@Z (core/window/Thunks.cpp) forwards to
// CWnd::OnSettingChange in core/window/CWnd.cpp, whose body SendMessage's
// WM_SETTINGCHANGE back to m_hWnd -- calling it from this handler would loop.
// (Review: an earlier comment called slot +0x7d0 "the virtual RecalcLayout";
// it holds ReposTasks.)
// Symbol: ?OnSettingChange@CMFCTasksPane@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCTasksPane__IEAAXIPEB_W_Z(
    CMFCTasksPane* pThis, unsigned int /*uFlags*/, const wchar_t* /*lpszSection*/)
{
    if (!pThis) return;
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
}
// CMFCTasksPane::OnSize(UINT nType, int cx, int cy) -- transcribed from
// mfc140u RVA 0x1447f0:
//   CBasePane::OnSize(nType, cx, cy);   // 0xc2a0 (mfc140u) = ?OnSize@CBasePane@@IEAAXIHH@Z
//                                       // (ANSI 0xc320; the earlier comment
//                                       // called it CWnd::OnSize -- it is CBasePane's)
//   AdjustScroll();                     // 0x148200
//   ReposTasks(FALSE);                  // vtable +0x7d0 (not RecalcLayout)
//   RedrawWindow(NULL, NULL, 0x105);    // tail jmp USER32!RedrawWindow
// The CBasePane::OnSize export is still a placeholder stub in
// docking/CBasePane.cpp (its definition omits `this`); it is called with the
// retail-correct list so the chain is complete once that body exists.
// Symbol: ?OnSize@CMFCTasksPane@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCTasksPane__IEAAXIHH_Z(
    CMFCTasksPane* pThis, unsigned int nType, int cx, int cy)
{
    if (!pThis) return;
    impl__OnSize_CBasePane__IEAAXIHH_Z(pThis, nType, cx, cy);
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}
// CMFCTasksPane::OnTimer(unsigned __int64) -- retail (RVA 0x14a1a0) handles
// the scroll-button timers (0xec0b/0xec0c): GetCursorPos/ScreenToClient/
// PtInRect against m_rectScrollUp (0x5B8) and m_rectScrollDn (0x5C8) update
// m_nVertScrollOffset (0x510), plus a scroll-animation path. Requires the
// scroll sub-window and the animation state.
// TODO(clean-room): partially transcribed -- scroll-button timers are not
// modeled.
// Symbol: ?OnTimer@CMFCTasksPane@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCTasksPane__IEAAX_K_Z(
    CMFCTasksPane* pThis, unsigned __int64 /*nIDEvent*/)
{
    if (!pThis) return;
}
// Symbol: ?OnUpdateBack@CMFCTasksPane@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateBack_CMFCTasksPane__IEAAXPEAVCCmdUI___Z(CMFCTasksPane*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateClose@CMFCTasksPane@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateClose_CMFCTasksPane__IEAAXPEAVCCmdUI___Z(CMFCTasksPane*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateForward@CMFCTasksPane@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateForward_CMFCTasksPane__IEAAXPEAVCCmdUI___Z(CMFCTasksPane*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
CMFCTasksPane::CMFCTasksPane() {
    memset(_taskspane_padding, 0, sizeof(_taskspane_padding));
    EnsureTasksPaneState(this);
}
CMFCTasksPane::~CMFCTasksPane() { g_tasksPaneStates.erase(this); }
BOOL CMFCTasksPane::Create(DWORD, const RECT&, CWnd*, UINT) {
    EnsureTasksPaneState(this);
    return TRUE;
}
int CMFCTasksPane::AddTask(int nGroup, const wchar_t* lpszName, int nIcon, unsigned int uiCmdID, unsigned __int64 dwUserData) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState& group = EnsureTasksPaneGroup(state, nGroup);

    TasksPaneGroupState::TaskEntry task;
    task.label = lpszName ? lpszName : L"";
    task.icon = nIcon;
    task.commandID = uiCmdID;
    task.userData = dwUserData;
    group.tasks.push_back(std::move(task));
    return static_cast<int>(group.tasks.size() - 1);
}
void CMFCTasksPane::AddTask(int nGroup, CMFCTasksPaneTask* pTask) {
    if (!pTask) return;
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState& group = EnsureTasksPaneGroup(state, nGroup);
    auto it = std::find_if(group.tasks.begin(), group.tasks.end(),
        [pTask](const TasksPaneGroupState::TaskEntry& task) { return task.task == pTask; });
    if (it == group.tasks.end()) {
        TasksPaneGroupState::TaskEntry task;
        task.task = pTask;
        group.tasks.push_back(std::move(task));
    }
}
void CMFCTasksPane::RemoveAllTasks() {
    TasksPaneState& state = EnsureTasksPaneState(this);
    for (auto& group : state.groups) {
        group.tasks.clear();
    }
}
void CMFCTasksPane::RemoveAllTasks(int nGroup) {
    if (nGroup < 0) {
        RemoveAllTasks();
        return;
    }

    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup >= static_cast<int>(state.groups.size())) return;
    state.groups[static_cast<size_t>(nGroup)].tasks.clear();
}
void CMFCTasksPane::SetCaption(int nGroup, const wchar_t* lpszCaption) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup < 0) {
        state.caption = lpszCaption ? lpszCaption : L"";
        return;
    }
    if (nGroup >= static_cast<int>(state.groups.size())) return;
    state.groups[static_cast<size_t>(nGroup)].name = lpszCaption ? lpszCaption : L"";
}
void CMFCTasksPane::SetCaption(const wchar_t* lpszCaption) { SetCaption(-1, lpszCaption); }
BOOL CMFCTasksPane::SetGroupName(int nGroup, const wchar_t* lpszName) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    if (nGroup < 0 || nGroup >= static_cast<int>(state.groups.size())) return FALSE;
    state.groups[static_cast<size_t>(nGroup)].name = lpszName ? lpszName : L"";
    return TRUE;
}
int CMFCTasksPane::AddGroup(const wchar_t* lpszName, BOOL bBottomHasGripper, BOOL bSpecial, int nIcon) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState group;
    group.name = lpszName ? lpszName : L"";
    group.hasGripper = bBottomHasGripper ? TRUE : FALSE;
    group.isSpecial = bSpecial ? TRUE : FALSE;
    group.icon = nIcon;
    state.groups.push_back(std::move(group));
    return static_cast<int>(state.groups.size() - 1);
}
int CMFCTasksPane::AddGroup(int nGroup, const wchar_t* lpszName, BOOL bBottomHasGripper, BOOL bSpecial, HICON hIcon) {
    TasksPaneState& state = EnsureTasksPaneState(this);
    TasksPaneGroupState group;
    group.name = lpszName ? lpszName : L"";
    group.hasGripper = bBottomHasGripper ? TRUE : FALSE;
    group.isSpecial = bSpecial ? TRUE : FALSE;
    group.hIcon = hIcon;

    if (nGroup >= 0 && nGroup <= static_cast<int>(state.groups.size())) {
        state.groups.insert(state.groups.begin() + nGroup, std::move(group));
        return nGroup;
    }

    state.groups.push_back(std::move(group));
    return static_cast<int>(state.groups.size() - 1);
}
// STUB.  CMFCTasksPane::AddMRUFilesList(int nGroup, int nMaxFiles) -- retail
// mfc140u RVA 0x147c20, decoded:
//   pGroup = FindIndex(nGroup)->data (out of range -> return -1);
//   remove every task of pGroup whose m_uiCommandID is in
//     [ID_FILE_MRU_FILE1 = 0xE110, +0xF], remembering the first such node;
//   CRecentFileList* pMRU = AfxGetModuleState()->m_pCurrentWinApp->m_pRecentFileList;   // +0x8 / +0x118
//   if (pMRU == NULL) return pGroup->m_lstTasks.m_nCount - 1;
//   n = min(nMaxFiles, pMRU->m_nSize);
//   for (i = 0; i < n; i++) if (GetMRUFileName(pMRU, i, strName))    // vtable +0x7b8
//       new task(pGroup, strName, -1, 0xE110 + i, 0, NULL, 0, 0) inserted before the
//       remembered node (InsertBefore) or AddTail'd;
//   AdjustScroll(); ReposTasks(FALSE); return pGroup->m_lstTasks.m_nCount - 1;
// Blocked: OpenMFC's AFX_MODULE_STATE (detail/RegcoreSupport.h) and CWinApp
// carry no retail-offset m_pCurrentWinApp / m_pRecentFileList, and
// include/openmfc/afxwin.h declares no CWinApp::m_pRecentFileList at all, so
// the recent-file list cannot be reached.  Signature corrected to carry `this`.
// Symbol: ?AddMRUFilesList@CMFCTasksPane@@QEAAHHH@Z
extern "C" int MS_ABI impl__AddMRUFilesList_CMFCTasksPane__QEAAHHH_Z(CMFCTasksPane* pThis, int nGroup, int nMaxFiles) {
    (void)pThis; (void)nGroup; (void)nMaxFiles;
    return -1;
}

// CMFCTasksPane::AddPage(LPCTSTR lpszPageLabel) -- transcribed from mfc140u
// RVA 0x145a40:
//   if (lpszPageLabel == NULL) AfxThrowInvalidArgException();    // 0x227720
//   pPage = new CMFCTasksPanePropertyPage(lpszPageLabel, this);   // 0x18 bytes:
//       vfptr = 0x180314cb8, m_strName = lpszPageLabel, m_pTaskPane = this
//   m_lstTasksPanes.AddTail(pPage);                               // 0x231e70 (NULL on alloc failure)
//   RebuildMenu();                                                // 0x148810
//   return m_lstTasksPanes.m_nCount - 1;
// Symbol: ?AddPage@CMFCTasksPane@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__AddPage_CMFCTasksPane__QEAAHPEB_W_Z(CMFCTasksPane* pThis, const wchar_t* lpszPageLabel)
{
    if (pThis == nullptr) return -1;
    if (lpszPageLabel == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    TPPage* pPage = static_cast<TPPage*>(std::malloc(sizeof(TPPage)));
    if (pPage != nullptr) {
        *reinterpret_cast<void* const**>(pPage) = g_TpPageVtbl;
        new (&pPage->m_strName) CString(lpszPageLabel);
        pPage->m_pTaskPane = pThis;
    }
    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(PageList(pThis), reinterpret_cast<CObject*>(pPage));
    impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(pThis);
    return static_cast<int>(ListCount(PageList(pThis))) - 1;
}

// CMFCTasksPane::AddWindow(int nGroup, HWND hwndTask, int nWindowHeight, BOOL
// bAutoDestroyWindow, DWORD_PTR dwUserData) -- transcribed from mfc140u RVA
// 0x146c40:
//   if (nGroup >= m_lstTaskGroups.m_nCount || nGroup < 0) return -1;
//   pGroup = FindIndex(nGroup)->data; (NULL node -> return -1)
//   pTask = new CMFCTasksPaneTask(pGroup, L"" /*0x18033d19c*/, -1, 0,
//                                 dwUserData, hwndTask, bAutoDestroyWindow,
//                                 nWindowHeight);                 // 0x1430c4
//   pGroup->m_lstTasks.AddTail(pTask);                            // AddTail even when new failed
//   AdjustScroll(); ReposTasks(FALSE);
//   return pGroup->m_lstTasks.m_nCount - 1;
// Symbol: ?AddWindow@CMFCTasksPane@@QEAAHHPEAUHWND__@@HH_K@Z
extern "C" int MS_ABI impl__AddWindow_CMFCTasksPane__QEAAHHPEAUHWND____HH_K_Z(
    CMFCTasksPane* pThis, int nGroup, HWND hwndTask, int nWindowHeight, int bAutoDestroyWindow,
    unsigned __int64 dwUserData)
{
    if (pThis == nullptr) return -1;
    TpNode* gnode = NodeAt(GroupList(pThis), nGroup);
    if (gnode == nullptr) return -1;
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(gnode->data);
    TPTask* pTask = NewTask(pGroup, L"", -1, 0u, dwUserData, hwndTask, bAutoDestroyWindow, nWindowHeight);
    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(TaskList(pGroup), reinterpret_cast<CObject*>(pTask));
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
    return static_cast<int>(ListCount(TaskList(pGroup))) - 1;
}

// STUB.  CMFCTasksPane::AdjustScroll() -- retail mfc140u RVA 0x148200.  It
// lays out the navigation toolbar (m_wndToolBar +0x7b8: virtuals at +0x4d0 /
// +0x480 of the CMFCToolBar vtable, GetParentTabbedPane, IsKindOf
// CMFCTasksPaneFrameWnd), computes m_rectToolbar / m_rectTasks /
// m_rectScrollUp / m_rectScrollDn, calls SetScrollSizes (0x148000),
// EnableWindow / MoveWindow on the vertical CScrollBar sub-window (+0x680),
// kills the animation timer 0xEC0C and invalidates the changed rectangles.
// Blocked: the toolbar and scroll-bar sub-objects are never constructed by
// OpenMFC's CMFCTasksPane and CMFCTasksPaneToolBar is entirely unimplemented.
// Signature corrected to carry `this`.
// Symbol: ?AdjustScroll@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis) {
    (void)pThis;
}

// CMFCTasksPane::ChangeActivePage(int nNewPageHistIdx, int nOldPageHistIdx) --
// transcribed from mfc140u RVA 0x148990:
//   ENSURE(0 <= nNew < m_nSize && 0 <= nOld < m_nSize)  else AfxThrowInvalidArgException
//   int nOldPage = m_arrHistoryStack[nOld];
//   if (m_arrHistoryStack[nNew] == nOldPage) return;
//   if (m_hWnd == NULL) { OnActivePageChanged(); RebuildMenu(); return; }   // vtable +0x7b0 (retail body: `ret`), 0x148810
//   ENSURE(0 <= nOldPage < m_lstTasksPanes.m_nCount); pOldPage = FindIndex(nOldPage)->data;
//   for (g = groups.head; g; g = g->pNext)                          // hide the old page's windows
//       if (g->data->m_pPage == pOldPage)
//           for (t = g->data->m_lstTasks.head; t; t = t->pNext)
//               if (t->data->m_hwndTask) CWnd::FromHandle(t->data->m_hwndTask)->ShowWindow(SW_HIDE);
//   UpdateCaption();                                                // 0x14b760
//   OnActivePageChanged();                                          // vtable +0x7b0
//   RebuildMenu();                                                  // 0x148810
//   m_nVertScrollOffset = 0;                                        // +0x510
//   AdjustScroll(); ReposTasks(FALSE);
//   ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd);
// Deviation: the OnActivePageChanged virtual (an empty inline in
// afxtaskspane.h; CMFCTasksPane's slot holds the shared `ret` at 0x27d0) is
// not dispatched, so a derived override is not honoured.  UpdateCaption and
// RebuildMenu are still stubs in this file.
// Symbol: ?ChangeActivePage@CMFCTasksPane@@IEAAXHH@Z
extern "C" void MS_ABI impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(CMFCTasksPane* pThis, int nNewPageHistIdx, int nOldPageHistIdx)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    int nNewPage = 0, nOldPage = 0;
    if (!HistoryAt(self, nNewPageHistIdx, nNewPage) || !HistoryAt(self, nOldPageHistIdx, nOldPage)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (nNewPage == nOldPage) return;
    if (pThis->m_hWnd == nullptr) {
        impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(pThis);
        return;
    }
    TpNode* pageNode = NodeAt(PageList(pThis), nOldPage);
    if (pageNode == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    const CObject* pOldPage = pageNode->data;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        const TPGroup* pGroup = reinterpret_cast<const TPGroup*>(g->data);
        if (pGroup->m_pPage != pOldPage) continue;
        for (TpNode* t = HeadNode(TaskList(pGroup)); t != nullptr; t = t->pNext) {
            const TPTask* pTask = reinterpret_cast<const TPTask*>(t->data);
            if (pTask->m_hwndTask == nullptr) continue;
            CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(pTask->m_hwndTask);
            if (pWnd != nullptr) impl__ShowWindow_CWnd__QEAAHH_Z(pWnd, SW_HIDE);
        }
    }
    impl__UpdateCaption_CMFCTasksPane__IEAAXXZ(pThis);
    impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(pThis);
    self->m_nVertScrollOffset = 0;
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
    ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    ::UpdateWindow(pThis->m_hWnd);
}

// CMFCTasksPane::CollapseAllGroups(BOOL bCollapse) -- transcribed from mfc140u
// RVA 0x146450:
//   for (g = m_lstTaskGroups.head; g; g = g->pNext)
//       if (bCollapse ? !g->data->m_bIsCollapsed : g->data->m_bIsCollapsed)
//           g->data->m_bIsCollapsed = bCollapse;
//   AdjustScroll(); ReposTasks(FALSE);
//   tail: RedrawWindow(NULL, NULL, 0x105)      // unconditional
// Symbol: ?CollapseAllGroups@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__CollapseAllGroups_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int bCollapse)
{
    if (pThis == nullptr) return;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        ApplyCollapse(reinterpret_cast<TPGroup*>(g->data), bCollapse);
    }
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// CMFCTasksPane::CollapseAllGroups(int nPage, BOOL bCollapse) -- transcribed
// from mfc140u RVA 0x1464c0:
//   if (nPage >= m_lstTasksPanes.m_nCount || nPage < 0) return;
//   pPage = FindIndex(nPage)->data; (NULL node -> return)
//   for (g = m_lstTaskGroups.head; g; g = g->pNext)
//       if (g->data->m_pPage == pPage &&                          // +0x8
//           (bCollapse ? !g->data->m_bIsCollapsed : g->data->m_bIsCollapsed))
//           g->data->m_bIsCollapsed = bCollapse;
//   AdjustScroll(); ReposTasks(FALSE); RedrawWindow(NULL, NULL, 0x105);
// Symbol: ?CollapseAllGroups@CMFCTasksPane@@QEAAXHH@Z
extern "C" void MS_ABI impl__CollapseAllGroups_CMFCTasksPane__QEAAXHH_Z(CMFCTasksPane* pThis, int nPage, int bCollapse)
{
    if (pThis == nullptr) return;
    TpNode* pageNode = NodeAt(PageList(pThis), nPage);
    if (pageNode == nullptr) return;
    const CObject* pPage = pageNode->data;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        TPGroup* pGroup = reinterpret_cast<TPGroup*>(g->data);
        if (pGroup->m_pPage != pPage) continue;
        ApplyCollapse(pGroup, bCollapse);
    }
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// CMFCTasksPane::CollapseGroup(CMFCTasksPaneTaskGroup* pGroup, BOOL bCollapse)
// -- transcribed from mfc140u RVA 0x1463f0:
//   if (bCollapse ? !pGroup->m_bIsCollapsed : pGroup->m_bIsCollapsed) {
//       pGroup->m_bIsCollapsed = bCollapse;                       // +0x58
//       AdjustScroll(); ReposTasks(FALSE); RedrawWindow(NULL, NULL, 0x105);
//   }
//   return TRUE;
// Symbol: ?CollapseGroup@CMFCTasksPane@@QEAAHPEAVCMFCTasksPaneTaskGroup@@H@Z
extern "C" int MS_ABI impl__CollapseGroup_CMFCTasksPane__QEAAHPEAVCMFCTasksPaneTaskGroup__H_Z(
    CMFCTasksPane* pThis, void* pGroup, int bCollapse)
{
    if (pThis == nullptr || pGroup == nullptr) return FALSE;
    TPGroup* g = static_cast<TPGroup*>(pGroup);
    const bool bSet = (bCollapse != 0) ? (g->m_bIsCollapsed == 0) : (g->m_bIsCollapsed != 0);
    if (bSet) {
        g->m_bIsCollapsed = bCollapse;
        RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    return TRUE;
}

// CMFCTasksPane::CreateDefaultMiniframe(CRect rectInitial) -- transcribed from
// mfc140u RVA 0x149870:
//   m_pMiniFrameRTC = (GetStyle() & CBRS_FLOAT_MULTI /*0x40*/)          // CPane +0x230
//       ? RUNTIME_CLASS(CMultiPaneFrameWnd)        // 0x1803b12f8, class name read from the descriptor
//       : RUNTIME_CLASS(CMFCTasksPaneFrameWnd);    // 0x1803b15c8
//   return CPane::CreateDefaultMiniframe(rectInitial);                 // 0xa1440
// The base export is still a stub in docking/CPane.cpp (returns NULL).
// Symbol: ?CreateDefaultMiniframe@CMFCTasksPane@@UEAAPEAVCPaneFrameWnd@@VCRect@@@Z
extern "C" void* MS_ABI impl__CreateDefaultMiniframe_CMFCTasksPane__UEAAPEAVCPaneFrameWnd__VCRect___Z(
    CMFCTasksPane* pThis, const RECT* pRectInitial)
{
    if (pThis == nullptr) return nullptr;
    static_assert(offsetof(CPane, m_pMiniFrameRTC) == 0x230, "CPane::m_pMiniFrameRTC @0x230");
    CPane* pPane = reinterpret_cast<CPane*>(pThis);
    const unsigned long style = impl__GetStyle_CWnd__QEBAKXZ(pThis);
    pPane->m_pMiniFrameRTC = (style & 0x40UL /*CBRS_FLOAT_MULTI*/)
        ? impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ()
        : impl__GetThisClass_CMFCTasksPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
    return impl__CreateDefaultMiniframe_CPane__UEAAPEAVCPaneFrameWnd__VCRect___Z(pPane, pRectInitial);
}

// CMFCTasksPane::CreateFonts() -- transcribed from mfc140u RVA 0x146ec0:
//   if (m_fontBold.m_hObject)          m_fontBold.DeleteObject();          // +0x558 (handle +0x560)
//   if (m_fontBoldUnderline.m_hObject) m_fontBoldUnderline.DeleteObject(); // +0x568
//   if (m_fontUnderline.m_hObject)     m_fontUnderline.DeleteObject();     // +0x578
//   HFONT h = m_hFont ? m_hFont : (HFONT)::GetStockObject(DEFAULT_GUI_FONT); // +0x550
//   CFont* pFont = CFont::FromHandle(h);                          // CGdiObject::FromHandle 0x2a3ea0
//   LOGFONT lf; memset(&lf, 0, sizeof lf); ::GetObject(pFont->m_hObject, sizeof lf, &lf);
//   lf.lfWeight = FW_BOLD;      m_fontBold.Attach(::CreateFontIndirect(&lf));
//   lf.lfUnderline = TRUE;      m_fontBoldUnderline.Attach(::CreateFontIndirect(&lf));
//   lf.lfWeight = FW_NORMAL; lf.lfUnderline = TRUE;
//                               m_fontUnderline.Attach(::CreateFontIndirect(&lf));
// The three CFont sub-objects are addressed as CGdiObject (m_hObject at +8).
// Symbol: ?CreateFonts@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__CreateFonts_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    static_assert(offsetof(CGdiObject, m_hObject) == 8, "CGdiObject::m_hObject @8");
    CGdiObject* pBold          = reinterpret_cast<CGdiObject*>(self->m_fonts + 0x00);   // +0x558
    CGdiObject* pBoldUnderline = reinterpret_cast<CGdiObject*>(self->m_fonts + 0x10);   // +0x568
    CGdiObject* pUnderline     = reinterpret_cast<CGdiObject*>(self->m_fonts + 0x20);   // +0x578
    if (pBold->m_hObject != nullptr)          impl__DeleteObject_CGdiObject__QEAAHXZ(pBold);
    if (pBoldUnderline->m_hObject != nullptr) impl__DeleteObject_CGdiObject__QEAAHXZ(pBoldUnderline);
    if (pUnderline->m_hObject != nullptr)     impl__DeleteObject_CGdiObject__QEAAHXZ(pUnderline);

    HGDIOBJ hFont = self->m_hFont;
    if (hFont == nullptr) hFont = ::GetStockObject(DEFAULT_GUI_FONT);
    CGdiObject* pFont = impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(hFont);
    LOGFONTW lf;
    std::memset(&lf, 0, sizeof(lf));
    ::GetObjectW(pFont != nullptr ? pFont->m_hObject : hFont, sizeof(lf), &lf);

    lf.lfWeight = FW_BOLD;
    impl__Attach_CGdiObject__QEAAHPEAX_Z(pBold, ::CreateFontIndirectW(&lf));
    lf.lfUnderline = TRUE;
    impl__Attach_CGdiObject__QEAAHPEAX_Z(pBoldUnderline, ::CreateFontIndirectW(&lf));
    lf.lfWeight = FW_NORMAL;
    lf.lfUnderline = TRUE;
    impl__Attach_CGdiObject__QEAAHPEAX_Z(pUnderline, ::CreateFontIndirectW(&lf));
}

// CMFCTasksPane::CreateMenu() const -- transcribed from mfc140u RVA 0x14b340:
//   CMenu menu;                                   // local, vftable 0x1802e2f58
//   menu.Attach(::CreatePopupMenu());             // 0x2a8100 = ?Attach@CMenu@@QEAAHPEAUHMENU__@@@Z
//   for (p = m_lstTasksPanes.head; p; p = p->pNext)
//       ::AppendMenuW(menu.m_hMenu, MF_STRING, 0x4280, p->data->m_strName);   // page +0x8
//   HMENU hMenu = menu.Detach();                  // inlined: AfxGetModuleThreadState()
//                                                 // (0x133a20)->m_pmapHMENU->RemoveKey (0x232ec0)
//   if (hMenu != NULL) {
//       ENSURE(0 <= m_iActivePage < m_arrHistoryStack.m_nSize)  else AfxThrowInvalidArgException
//       ::CheckMenuItem(hMenu, m_arrHistoryStack[m_iActivePage], MF_BYPOSITION | MF_CHECKED /*0x408*/);
//   }
//   return hMenu;                                 // the local CMenu is empty by now
// (Review: the previous stub note claimed this depends on CMFCToolBarImages /
// AFX_GLOBAL_DATA; the body references neither.  IAT slots on the u image:
// 0x2c6cc0 CreatePopupMenu, 0x2c6cb8 AppendMenuW, 0x2c6da0 CheckMenuItem.)
// The Attach/Detach pair on the local CMenu leaves the handle map exactly as
// it found it, so the HMENU is used bare here; 0x4280 is the item id retail
// gives every page entry (the ID_AFXBARRES_TASKPANE_* range, see OnCreate).
// Symbol: ?CreateMenu@CMFCTasksPane@@QEBAPEAUHMENU__@@XZ
extern "C" HMENU MS_ABI impl__CreateMenu_CMFCTasksPane__QEBAPEAUHMENU____XZ(const CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return nullptr;
    const TP* self = View(pThis);
    HMENU hMenu = ::CreatePopupMenu();
    for (TpNode* p = HeadNode(PageList(pThis)); p != nullptr; p = p->pNext) {
        const TPPage* pPage = reinterpret_cast<const TPPage*>(p->data);
        ::AppendMenuW(hMenu, MF_STRING, 0x4280u,
                      static_cast<const wchar_t*>(*reinterpret_cast<const CString*>(&pPage->m_strName)));
    }
    if (hMenu != nullptr) {
        int nPage = 0;
        if (!HistoryAt(self, self->m_iActivePage, nPage)) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return hMenu;
        }
        ::CheckMenuItem(hMenu, static_cast<UINT>(nPage), MF_BYPOSITION | MF_CHECKED);
    }
    return hMenu;
}

// STUB.  CMFCTasksPane::CreateNavigationToolbar() -- retail mfc140u RVA
// 0x14a930.  Creates m_wndToolBar (+0x7b8, a CMFCTasksPaneToolBar) with the
// back/forward/home/other buttons (four CMFCToolBarButton ctors at mfc140u
// 0x15bf40, strings loaded via AfxFindStringResourceHandle 0x2aee00 +
// CStringT::LoadString 0xdb70).
// Blocked: CMFCTasksPaneToolBar is unimplemented and the sub-object is never
// constructed by OpenMFC's CMFCTasksPane.  Signature corrected to carry `this`.
// Symbol: ?CreateNavigationToolbar@CMFCTasksPane@@IEAAHXZ
extern "C" int MS_ABI impl__CreateNavigationToolbar_CMFCTasksPane__IEAAHXZ(CMFCTasksPane* pThis) {
    (void)pThis;
    return FALSE;
}

// STUB.  CMFCTasksPane::DoPaint(CDC* pDC) -- retail mfc140u RVA 0x148690:
//   CMemDC memDC(*pDC, this);  CDC* pPaint = memDC.m_bMemDC ? &memDC.m_dcMem : pDC;  // 0x69f50 = ??0CMemDC@@QEAA@AEAVCDC@@PEAVCWnd@@@Z
//   GetClientRect(rectClient); rectBorder = rectClient; SetRectEmpty(&rectBorder);
//   if (IsToolBox()) { rectBorder = rectClient; InflateRect(&rectClient, -1, -1); }   // vtable +0x798
//   OnDrawTasks(pPaint, rectClient);                                  // vtable +0x7a8 (0x145360);
//                                                                     // OnDrawTasks itself calls
//                                                                     // OnFillBackground (+0x7a0) --
//                                                                     // DoPaint does NOT
//   if (!IsRectEmpty(&rectBorder))
//       CMFCVisualManager::GetInstance()->{vtable +0x310}(pPaint, rectBorder);   // 0x9774
//   memDC.~CMemDC();                                                  // 0x6a380 = ??1CMemDC@@UEAA@XZ
// (Review: an earlier version of this note listed an OnFillBackground call in
// DoPaint and said the CMemDC exports were unimplemented; neither is true --
// the retail body has one virtual draw call, and both CMemDC exports are
// implemented in detail/CMemDCSupport.cpp.)  Left a stub because OnDrawTasks
// (this file) is a stub: a transcription would construct a CMemDC around a
// no-op and then blit that buffer over the pane.  Signature corrected to
// carry `this`.
// Symbol: ?DoPaint@CMFCTasksPane@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCTasksPane__MEAAXPEAVCDC___Z(CMFCTasksPane* pThis, CDC* pDC) {
    (void)pThis; (void)pDC;
}

// STUB.  CMFCTasksPane::DrawCaption(CDC* pDC, CRect rectCaption) -- retail
// mfc140u RVA 0x148e60:
//   for each CMFCCaptionButton* in CDockablePane::m_arrButtons (+0x488;
//   m_pData +0x490, m_nSize +0x498):
//     if (m_nHit /*+0x20*/ in {0x17,0x18,0x19}) m_bHidden /*+0x10*/ =
//         !(m_bHidden == 0 && m_lstTasksPanes.m_nCount > 1 && !m_bUseNavigationToolbar);
//     if (m_nHit == 0x17) m_bEnabled /*+0x14*/ = m_iActivePage > 0;
//     if (m_nHit == 0x18) m_bEnabled = m_iActivePage < m_arrHistoryStack.m_nSize - 1;
//   CDockablePane::UpdateTooltips();                                 // 0x405f0
//   CDockablePane::DrawCaption(pDC, rectCaption);                    // 0x446a0
// Blocked: m_arrButtons and m_pToolTip sit above the storage OpenMFC's
// CDockablePane provides (see docking/CDockablePane.cpp), and both callees
// are stubs there.  Signature corrected: `this`, CDC*, and the by-value CRect
// (16 bytes -> passed by hidden pointer).
// Symbol: ?DrawCaption@CMFCTasksPane@@MEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__DrawCaption_CMFCTasksPane__MEAAXPEAVCDC__VCRect___Z(CMFCTasksPane* pThis, CDC* pDC, const RECT* pRectCaption) {
    (void)pThis; (void)pDC; (void)pRectCaption;
}

// CMFCTasksPane::EnableHistoryMenuButtons(BOOL bEnable) -- transcribed from
// mfc140u RVA 0x14ba60:
//   if (m_bHistoryMenuButtons == bEnable) return;                   // +0x4e8
//   if (m_wndToolBar.m_hWnd != NULL) {                              // +0x7b8, HWND at +0x40
//       m_wndToolBar.DestroyWindow();                               // CWnd vtable +0xd0
//       m_bHistoryMenuButtons = bEnable;
//       CreateNavigationToolbar();                                  // 0x14a930
//       m_wndToolBar.UpdateButtons();                               // 0x143d80
//   } else {
//       m_bHistoryMenuButtons = bEnable;
//   }
// DestroyWindow goes through the exported CWnd thunk; CreateNavigationToolbar
// (this file) and CMFCTasksPaneToolBar::UpdateButtons (CMFCTasksPaneToolBar.cpp,
// still a stub whose placeholder list omits `this`; harmless under the MS x64
// ABI because the body reads nothing) are called as exports.
// Symbol: ?EnableHistoryMenuButtons@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableHistoryMenuButtons_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int bEnable)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    if (self->m_bHistoryMenuButtons == bEnable) return;
    CWnd* pToolBar = reinterpret_cast<CWnd*>(self->m_wndToolBar);
    if (pToolBar->m_hWnd != nullptr) {
        impl__DestroyWindow_CWnd__UEAAHXZ(pToolBar);
        self->m_bHistoryMenuButtons = bEnable;
        impl__CreateNavigationToolbar_CMFCTasksPane__IEAAHXZ(pThis);
        impl__UpdateButtons_CMFCTasksPaneToolBar__QEAAXXZ();
    } else {
        self->m_bHistoryMenuButtons = bEnable;
    }
}

// STUB.  CMFCTasksPane::EnableNavigationToolbar(BOOL bEnable, UINT
// uiToolbarBmpRes, CSize sizeToolbarImage, CSize sizeToolbarButton) -- retail
// mfc140u RVA 0x14b460.  Stores the four arguments (m_bUseNavigationToolbar
// +0x4e4, m_uiToolbarBmpRes +0x54c, m_sizeToolbarImage +0x590,
// m_sizeToolbarButton +0x598), destroys and recreates the navigation toolbar
// (CMFCToolBarImages::Clear, CreateNavigationToolbar, UpdateButtons,
// UpdateCaption, AdjustScroll) and redraws.  Blocked for the same reason as
// CreateNavigationToolbar.  Signature corrected: `this`, BOOL, UINT and the two
// by-value CSizes (8 bytes each -> one register each).
// Symbol: ?EnableNavigationToolbar@CMFCTasksPane@@QEAAXHIVCSize@@0@Z
extern "C" void MS_ABI impl__EnableNavigationToolbar_CMFCTasksPane__QEAAXHIVCSize__0_Z(
    CMFCTasksPane* pThis, int bEnable, unsigned int uiToolbarBmpRes, long long sizeToolbarImage, long long sizeToolbarButton) {
    (void)pThis; (void)bEnable; (void)uiToolbarBmpRes; (void)sizeToolbarImage; (void)sizeToolbarButton;
}

// CMFCTasksPane::GetGroupLocation(CMFCTasksPaneTaskGroup* pGroup, int& nGroup)
// const -- transcribed from mfc140u RVA 0x1465a0:
//   i = 0; for (node = head; node; node = node->pNext, ++i)
//       if (node->data == pGroup) { nGroup = i; return TRUE; }
//   return FALSE;                       // nGroup untouched on failure
// Symbol: ?GetGroupLocation@CMFCTasksPane@@QEBAHPEAVCMFCTasksPaneTaskGroup@@AEAH@Z
extern "C" int MS_ABI impl__GetGroupLocation_CMFCTasksPane__QEBAHPEAVCMFCTasksPaneTaskGroup__AEAH_Z(
    const CMFCTasksPane* pThis, void* pGroup, int* pnGroup)
{
    if (pThis == nullptr || pnGroup == nullptr) return FALSE;
    int i = 0;
    for (TpNode* node = HeadNode(GroupList(pThis)); node != nullptr; node = node->pNext, ++i) {
        if (node->data == pGroup) {
            *pnGroup = i;
            return TRUE;
        }
    }
    return FALSE;
}

// CMFCTasksPane::GetMRUFileName(CRecentFileList* pRecentFileList, int nIndex,
// CString& strName) -- transcribed from mfc140u RVA 0x147e20:
//   if (pRecentFileList == NULL || nIndex < 0 || nIndex >= pRecentFileList->m_nSize)
//       AfxThrowInvalidArgException();                             // m_nSize at +0x8
//   if (pRecentFileList->m_arrNames[nIndex].IsEmpty()) return FALSE;   // m_arrNames at +0x10
//   TCHAR buf[513]; memset(buf, 0, sizeof buf);
//   if (::GetFileTitle(m_arrNames[nIndex], buf, 512) != 0) return FALSE;   // 0x14be14 = COMDLG32
//                                           // delay-load stub (LoadLibrary "Comdlg32" + GetProcAddress)
//   strName = buf; return TRUE;
// Deviation (storage only): OpenMFC's CRecentFileList keeps its entries in a
// side table (core/app/CRecentFileList.cpp), not in a retail m_arrNames array,
// so the full path is fetched through the exported GetDisplayName thunk, whose
// OpenMFC body returns the stored path verbatim when its last argument is
// non-zero and FALSE for an out-of-range index.
// Symbol: ?GetMRUFileName@CMFCTasksPane@@MEAAHPEAVCRecentFileList@@HAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__GetMRUFileName_CMFCTasksPane__MEAAHPEAVCRecentFileList__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CMFCTasksPane* pThis, CRecentFileList* pRecentFileList, int nIndex, CString& strName)
{
    (void)pThis;
    if (pRecentFileList == nullptr || nIndex < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    CString strPath;
    if (!impl__GetDisplayName_CRecentFileList__UEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEB_WHH_Z(
            pRecentFileList, &strPath, nIndex, L"", 0, TRUE)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    if (strPath.IsEmpty()) return FALSE;
    wchar_t buf[513];
    std::memset(buf, 0, sizeof(buf));
    if (::GetFileTitleW(static_cast<const wchar_t*>(strPath), buf, 512) != 0) return FALSE;
    strName = buf;
    return TRUE;
}

// CMFCTasksPane::GetNextPages(CStringList& lstNextPages) const -- transcribed
// from mfc140u RVA 0x14b9c0:
//   lstNextPages.RemoveAll();                                       // 0x232300
//   int n = m_arrHistoryStack.m_nSize - m_iActivePage - 1;          // +0x788 / +0x504
//   for (int i = 0; i < n; i++) {
//       idx = m_iActivePage + i + 1;         ENSURE(0 <= idx < m_nSize)  else AfxThrowInvalidArgException
//       nPage = m_arrHistoryStack[idx];      ENSURE(0 <= nPage < m_lstTasksPanes.m_nCount)
//       pPage = m_lstTasksPanes.FindIndex(nPage)->data;  (NULL -> throw)
//       lstNextPages.AddTail(pPage->m_strName);                     // 0x2325f0, page +0x8
//   }
// Symbol: ?GetNextPages@CMFCTasksPane@@QEBAXAEAVCStringList@@@Z
extern "C" void MS_ABI impl__GetNextPages_CMFCTasksPane__QEBAXAEAVCStringList___Z(const CMFCTasksPane* pThis, CStringList* pList)
{
    if (pThis == nullptr || pList == nullptr) return;
    const TP* self = View(pThis);
    impl__RemoveAll_CStringList__QEAAXXZ(pList);
    const int n = static_cast<int>(self->m_nHistorySize) - self->m_iActivePage - 1;
    for (int i = 0; i < n; ++i) {
        int nPage = 0;
        if (!HistoryAt(self, static_cast<long long>(self->m_iActivePage) + i + 1, nPage)) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        TpNode* node = NodeAt(PageList(pThis), nPage);
        if (node == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        const TPPage* pPage = reinterpret_cast<const TPPage*>(node->data);
        impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pList, *reinterpret_cast<const CString*>(&pPage->m_strName));
    }
}

// CMFCTasksPane::GetPageByGroup(int nGroup, int& nPage) const -- transcribed
// from mfc140u RVA 0x148c90:
//   pGroup = GetTaskGroup(nGroup);                                 // 0x146570
//   i = 0; for (p = m_lstTasksPanes.head; p; p = p->pNext, ++i)
//       if (p->data == pGroup->m_pPage) { nPage = i; return TRUE; }
//   return FALSE;
// Deviation: retail reads pGroup->m_pPage without a NULL test (only reached
// when the page list is non-empty); a NULL group returns FALSE here.
// Symbol: ?GetPageByGroup@CMFCTasksPane@@QEBAHHAEAH@Z
extern "C" int MS_ABI impl__GetPageByGroup_CMFCTasksPane__QEBAHHAEAH_Z(
    const CMFCTasksPane* pThis, int nGroup, int* pnPage)
{
    if (pThis == nullptr || pnPage == nullptr) return FALSE;
    const TPGroup* pGroup = static_cast<const TPGroup*>(
        impl__GetTaskGroup_CMFCTasksPane__QEBAPEAVCMFCTasksPaneTaskGroup__H_Z(pThis, nGroup));
    if (pGroup == nullptr) return FALSE;
    int i = 0;
    for (TpNode* p = HeadNode(PageList(pThis)); p != nullptr; p = p->pNext, ++i) {
        if (p->data == pGroup->m_pPage) {
            *pnPage = i;
            return TRUE;
        }
    }
    return FALSE;
}

// CMFCTasksPane::GetPreviousPages(CStringList& lstPrevPages) const --
// transcribed from mfc140u RVA 0x14b930:
//   lstPrevPages.RemoveAll();
//   for (int i = 0; i < m_iActivePage; i++) {
//       idx = m_iActivePage - i - 1;         ENSURE(0 <= idx < m_nSize)
//       nPage = m_arrHistoryStack[idx];      ENSURE(0 <= nPage < m_lstTasksPanes.m_nCount)
//       lstPrevPages.AddTail(FindIndex(nPage)->data->m_strName);
//   }
// Symbol: ?GetPreviousPages@CMFCTasksPane@@QEBAXAEAVCStringList@@@Z
extern "C" void MS_ABI impl__GetPreviousPages_CMFCTasksPane__QEBAXAEAVCStringList___Z(const CMFCTasksPane* pThis, CStringList* pList)
{
    if (pThis == nullptr || pList == nullptr) return;
    const TP* self = View(pThis);
    impl__RemoveAll_CStringList__QEAAXXZ(pList);
    const int n = self->m_iActivePage;
    for (int i = 0; i < n; ++i) {
        int nPage = 0;
        if (!HistoryAt(self, static_cast<long long>(self->m_iActivePage) - i - 1, nPage)) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        TpNode* node = NodeAt(PageList(pThis), nPage);
        if (node == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        const TPPage* pPage = reinterpret_cast<const TPPage*>(node->data);
        impl__AddTail_CStringList__QEAAPEAU__POSITION__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pList, *reinterpret_cast<const CString*>(&pPage->m_strName));
    }
}

// CMFCTasksPane::GetScrollBarCtrl(int nBar) const -- transcribed from mfc140u
// RVA 0x147f00:
//   if (nBar == SB_HORZ) return NULL;
//   return m_wndScrollVert.m_hWnd != NULL ? &m_wndScrollVert : NULL;   // +0x680, HWND at +0x40
// Symbol: ?GetScrollBarCtrl@CMFCTasksPane@@UEBAPEAVCScrollBar@@H@Z
extern "C" void* MS_ABI impl__GetScrollBarCtrl_CMFCTasksPane__UEBAPEAVCScrollBar__H_Z(const CMFCTasksPane* pThis, int nBar)
{
    if (pThis == nullptr) return nullptr;
    if (nBar == SB_HORZ) return nullptr;
    CWnd* pScroll = reinterpret_cast<CWnd*>(const_cast<char*>(View(pThis)->m_wndScrollVert));
    return pScroll->m_hWnd != nullptr ? pScroll : nullptr;
}

// CMFCTasksPane::GetTask(int nGroup, int nTask) const -- transcribed from
// mfc140u RVA 0x146ba0:
//   CMFCTasksPaneTaskGroup* pGroup = GetTaskGroup(nGroup);        // 0x146570
//   if (nTask >= pGroup->m_lstTasks.m_nCount || nTask < 0) return NULL;
//   node = pGroup->m_lstTasks.m_pNodeHead; while (nTask--) node = node->pNext;
//   return node ? node->data : NULL;
// Deviation: retail dereferences pGroup unconditionally (it faults on a bad
// nGroup); a NULL group returns NULL here.
// Symbol: ?GetTask@CMFCTasksPane@@QEBAPEAVCMFCTasksPaneTask@@HH@Z
extern "C" void* MS_ABI impl__GetTask_CMFCTasksPane__QEBAPEAVCMFCTasksPaneTask__HH_Z(
    const CMFCTasksPane* pThis, int nGroup, int nTask)
{
    if (pThis == nullptr) return nullptr;
    TPGroup* pGroup = static_cast<TPGroup*>(
        impl__GetTaskGroup_CMFCTasksPane__QEBAPEAVCMFCTasksPaneTaskGroup__H_Z(pThis, nGroup));
    if (pGroup == nullptr) return nullptr;
    TpNode* node = NodeAt(TaskList(pGroup), nTask);
    return node != nullptr ? node->data : nullptr;
}

// CMFCTasksPane::GetTaskGroup(int nGroup) const -- transcribed from mfc140u
// RVA 0x146570:
//   if (nGroup >= m_lstTaskGroups.m_nCount || nGroup < 0) return NULL;
//   node = m_lstTaskGroups.m_pNodeHead; while (nGroup--) node = node->pNext;
//   return node ? node->data : NULL;
// (bounds test + walk = the FindIndex thunk; see the storage note at the top.)
// Symbol: ?GetTaskGroup@CMFCTasksPane@@QEBAPEAVCMFCTasksPaneTaskGroup@@H@Z
extern "C" void* MS_ABI impl__GetTaskGroup_CMFCTasksPane__QEBAPEAVCMFCTasksPaneTaskGroup__H_Z(
    const CMFCTasksPane* pThis, int nGroup)
{
    if (pThis == nullptr) return nullptr;
    TpNode* node = NodeAt(GroupList(pThis), nGroup);
    return node != nullptr ? node->data : nullptr;
}

// CMFCTasksPane::GetTaskLocation(UINT uiCommandID, int& nGroup, int& nTask)
// const -- transcribed from mfc140u RVA 0x146ac0:
//   nGroup = 0;
//   for (g = groups.head; g; g = g->pNext, ++nGroup) {
//       nTask = 0;
//       for (t = g->data->m_lstTasks.head; t; t = t->pNext) {
//           if (t->data->m_uiCommandID == uiCommandID) return TRUE;   // +0x30
//           nTask = ++i;
//       }
//   }
//   nGroup = -1; nTask = -1; return FALSE;
// Symbol: ?GetTaskLocation@CMFCTasksPane@@QEBAHIAEAH0@Z
extern "C" int MS_ABI impl__GetTaskLocation_CMFCTasksPane__QEBAHIAEAH0_Z(
    const CMFCTasksPane* pThis, unsigned int uiCommandID, int* pnGroup, int* pnTask)
{
    if (pThis == nullptr || pnGroup == nullptr || pnTask == nullptr) return FALSE;
    *pnGroup = 0;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        const TPGroup* pGroup = reinterpret_cast<const TPGroup*>(g->data);
        *pnTask = 0;
        int i = 0;
        for (TpNode* t = HeadNode(TaskList(pGroup)); t != nullptr; t = t->pNext) {
            const TPTask* pTask = reinterpret_cast<const TPTask*>(t->data);
            if (pTask->m_uiCommandID == uiCommandID) return TRUE;
            *pnTask = ++i;
        }
        ++*pnGroup;
    }
    *pnGroup = -1;
    *pnTask = -1;
    return FALSE;
}

// CMFCTasksPane::GetTaskLocation(HWND hwndTask, int& nGroup, int& nTask) const
// -- transcribed from mfc140u RVA 0x146b30; identical to the command-id
// overload above except that the match is `t->data->m_hwndTask == hwndTask`
// (+0x40).
// Symbol: ?GetTaskLocation@CMFCTasksPane@@QEBAHPEAUHWND__@@AEAH1@Z
extern "C" int MS_ABI impl__GetTaskLocation_CMFCTasksPane__QEBAHPEAUHWND____AEAH1_Z(
    const CMFCTasksPane* pThis, HWND hwndTask, int* pnGroup, int* pnTask)
{
    if (pThis == nullptr || pnGroup == nullptr || pnTask == nullptr) return FALSE;
    *pnGroup = 0;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        const TPGroup* pGroup = reinterpret_cast<const TPGroup*>(g->data);
        *pnTask = 0;
        int i = 0;
        for (TpNode* t = HeadNode(TaskList(pGroup)); t != nullptr; t = t->pNext) {
            const TPTask* pTask = reinterpret_cast<const TPTask*>(t->data);
            if (pTask->m_hwndTask == hwndTask) return TRUE;
            *pnTask = ++i;
        }
        ++*pnGroup;
    }
    *pnGroup = -1;
    *pnTask = -1;
    return FALSE;
}

// CMFCTasksPane::GetTaskLocation(CMFCTasksPaneTask* pTask, int& nGroup, int&
// nTask) const -- transcribed from mfc140u RVA 0x146be0:
//   nGroup = -1; nTask = -1;
//   i = 0; for (g = groups.head; g; g = g->pNext, ++i) {
//       if (g->data == pTask->m_pGroup) {                          // +0x8
//           j = 0; for (t = g->data->m_lstTasks.head; t; t = t->pNext, ++j)
//               if (t->data == pTask) { nGroup = i; nTask = j; return TRUE; }
//           return FALSE;
//       }
//   }
//   return FALSE;
// Deviation: retail reads pTask->m_pGroup unconditionally; NULL returns FALSE.
// Symbol: ?GetTaskLocation@CMFCTasksPane@@QEBAHPEAVCMFCTasksPaneTask@@AEAH1@Z
extern "C" int MS_ABI impl__GetTaskLocation_CMFCTasksPane__QEBAHPEAVCMFCTasksPaneTask__AEAH1_Z(
    const CMFCTasksPane* pThis, void* pTask, int* pnGroup, int* pnTask)
{
    if (pThis == nullptr || pnGroup == nullptr || pnTask == nullptr) return FALSE;
    *pnGroup = -1;
    *pnTask = -1;
    if (pTask == nullptr) return FALSE;
    const void* pOwner = static_cast<const TPTask*>(pTask)->m_pGroup;
    int i = 0;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext, ++i) {
        if (g->data != pOwner) continue;
        const TPGroup* pGroup = reinterpret_cast<const TPGroup*>(g->data);
        int j = 0;
        for (TpNode* t = HeadNode(TaskList(pGroup)); t != nullptr; t = t->pNext, ++j) {
            if (t->data == pTask) {
                *pnGroup = i;
                *pnTask = j;
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;
}

// CMFCTasksPane::GetTasksGroupBorders() const -- transcribed from mfc140u RVA
// 0x14bad0:
//   int b = (IsToolBox() == FALSE);     // vtable +0x798; CMFCTasksPane's slot
//                                       // holds 0x71e0 = `xor eax,eax; ret`
//   *pRet = CSize(b, b); return pRet;   // CSize is returned via hidden pointer
// Deviation: IsToolBox is an inline virtual with no export, so it cannot be
// dispatched here; the CMFCTasksPane body (FALSE) is assumed, i.e. (1, 1).
// Symbol: ?GetTasksGroupBorders@CMFCTasksPane@@MEBA?AVCSize@@XZ
extern "C" SIZE* MS_ABI impl__GetTasksGroupBorders_CMFCTasksPane__MEBA_AVCSize__XZ(const CMFCTasksPane* pThis, SIZE* pRet)
{
    if (pRet == nullptr) return nullptr;
    (void)pThis;
    pRet->cx = 1;
    pRet->cy = 1;
    return pRet;
}

// CMFCTasksPane::GroupCaptionHitTest(CPoint point) const -- transcribed from
// mfc140u RVA 0x147160:
//   if (!m_bCanCollapse) return NULL;                               // +0x4e0
//   if (!::PtInRect(&m_rectTasks, point)) return NULL;              // +0x5d8
//   nPage = m_arrHistoryStack[m_iActivePage];   ENSURE(bounds)  else AfxThrowInvalidArgException
//   pPage = m_lstTasksPanes.FindIndex(nPage)->data;  ENSURE(bounds / non-NULL)
//   for (g = m_lstTaskGroups.head; g; g = g->pNext)
//       if (g->data->m_pPage == pPage && ::PtInRect(&g->data->m_rect, point))   // +0x8 / +0x5c
//           return g->data;
//   return NULL;
// The by-value CPoint arrives packed in one register (x low, y high).
// Symbol: ?GroupCaptionHitTest@CMFCTasksPane@@MEBAPEAVCMFCTasksPaneTaskGroup@@VCPoint@@@Z
extern "C" void* MS_ABI impl__GroupCaptionHitTest_CMFCTasksPane__MEBAPEAVCMFCTasksPaneTaskGroup__VCPoint___Z(
    const CMFCTasksPane* pThis, long long packedPoint)
{
    if (pThis == nullptr) return nullptr;
    const TP* self = View(pThis);
    if (self->m_bCanCollapse == 0) return nullptr;
    POINT pt;
    pt.x = static_cast<LONG>(packedPoint & 0xffffffffLL);
    pt.y = static_cast<LONG>((packedPoint >> 32) & 0xffffffffLL);
    if (!::PtInRect(&self->m_rectTasks, pt)) return nullptr;
    int nPage = 0;
    if (!HistoryAt(self, self->m_iActivePage, nPage)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    TpNode* pageNode = NodeAt(PageList(pThis), nPage);
    if (pageNode == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    const CObject* pPage = pageNode->data;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        TPGroup* pGroup = reinterpret_cast<TPGroup*>(g->data);
        if (pGroup->m_pPage != pPage) continue;
        if (::PtInRect(&pGroup->m_rect, pt)) return pGroup;
    }
    return nullptr;
}

// STUB.  CMFCTasksPane::LoadState(LPCTSTR lpszProfileName, int nIndex, UINT
// uiID) -- retail mfc140u RVA 0x149ec0:
//   CPane::LoadState(lpszProfileName, nIndex, uiID);                 // 0xa1ca0
//   strProfile = AFXGetRegPath(strTasksPaneProfile, lpszProfileName);
//   if (nIndex == -1) nIndex = GetDlgCtrlID();
//   strSection.Format(L"%s-%d", strProfile, nIndex);
//   CSettingsStoreSP regSP; CSettingsStore& reg = regSP.Create(FALSE, TRUE);
//   if (reg.Open(strSection)) { read the "Settings" blob into a CMemFile,
//       CArchive::operator>> this (Serialize), free the blob }
//   return TRUE;
// Blocked: CSettingsStoreSP / AFXGetRegPath and this class's Serialize (a
// second-batch symbol) are not available here.  Signature corrected to carry
// `this`.
// Symbol: ?LoadState@CMFCTasksPane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__LoadState_CMFCTasksPane__UEAAHPEB_WHI_Z(CMFCTasksPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return FALSE;
}

// CMFCTasksPane::OnBack() -- transcribed from mfc140u RVA 0x14b190:
//   if (m_bHistoryMenuButtons) {                                    // +0x4e8
//       int n = CMFCPopupMenuBar::m_nLastCommandIndex;              // exported static, 0x3b1b3c
//       if (n >= 0 && n < m_iActivePage) {                          // +0x504
//           int nOld = m_iActivePage;
//           m_iActivePage = nOld - n - 1;
//           ChangeActivePage(m_iActivePage, nOld);                  // tail jmp 0x148990
//           return;
//       }
//   }
//   OnPressBackButton();                                            // tail jmp via vtable +0x750
// Deviation: the virtual is called as this class's own export.
// Symbol: ?OnBack@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnBack_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    if (self->m_bHistoryMenuButtons != 0) {
        const int n = impl__m_nLastCommandIndex_CMFCPopupMenuBar__1HA;
        if (n >= 0 && n < self->m_iActivePage) {
            const int nOld = self->m_iActivePage;
            self->m_iActivePage = nOld - n - 1;
            impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
            return;
        }
    }
    impl__OnPressBackButton_CMFCTasksPane__UEAAXXZ(pThis);
}

// CMFCTasksPane::OnCancel() -- transcribed from mfc140u RVA 0x149aa0:
//   if (IsToolBox()) return;                                  // vtable +0x798 (FALSE for this class)
//   CWnd* pFocus = CWnd::FromHandle(::GetFocus());
//   if (pFocus != NULL && ::IsChild(m_hWnd, pFocus->m_hWnd) && pFocus != this) {
//       for (;;) {
//           if (CWnd::FromHandle(::GetParent(pFocus->m_hWnd)) == this) {
//               const MSG* pMsg = CWnd::GetCurrentMessage();          // 0x28ac10
//               ::SendMessage(pFocus->m_hWnd, WM_COMMAND, pMsg->wParam, pMsg->lParam);
//               break;
//           }
//           pFocus = CWnd::FromHandle(::GetParent(pFocus->m_hWnd));
//           if (pFocus == this) break;
//       }
//   }
//   CFrameWnd* pFrame = g_pTopLevelFrame;                      // 0x3be1b8
//   if (pFrame == NULL) pFrame = GetTopLevelFrame();           // 0x28e490
//   if (::IsWindow(pFrame ? pFrame->m_hWnd : NULL)) pFrame->SetFocus();   // 0x2a9b60 = CWnd::SetFocus
// Deviation: IsToolBox cannot be dispatched (see GetTasksGroupBorders); the
// CMFCTasksPane value FALSE is assumed.  Two guards are added: a NULL pMsg
// test, and the walk stops when FromHandle(GetParent()) yields NULL (retail
// would dereference it on the next iteration).
// Symbol: ?OnCancel@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancel_CMFCTasksPane__UEAAXXZ(CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return;
    CWnd* pFocus = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
    if (pFocus != nullptr && ::IsChild(pThis->m_hWnd, pFocus->m_hWnd) && pFocus != pThis) {
        for (;;) {
            CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pFocus->m_hWnd));
            if (pParent == pThis) {
                const MSG* pMsg = impl__GetCurrentMessage_CWnd__KAPEBUtagMSG__XZ();
                if (pMsg != nullptr) {
                    ::SendMessage(pFocus->m_hWnd, WM_COMMAND, pMsg->wParam, pMsg->lParam);
                }
                break;
            }
            pFocus = pParent;
            if (pFocus == pThis || pFocus == nullptr) break;
        }
    }
    CFrameWnd* pFrame = static_cast<CFrameWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
    if (pFrame == nullptr) {
        pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    }
    HWND hFrame = pFrame != nullptr ? pFrame->m_hWnd : nullptr;
    if (::IsWindow(hFrame)) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(pFrame);
    }
}

// CMFCTasksPane::OnCancelMode() -- transcribed from mfc140u RVA 0x1479a0:
//   CDockablePane::OnCancelMode();                                   // 0x45280
//   if (m_pHotTask != NULL || m_pClickedTask != NULL) {              // +0x5f8 / +0x600
//       CRect rc = m_pHotTask ? m_pHotTask->m_rect : CRect(0,0,0,0); // task +0x18
//       m_pHotTask = NULL; m_pClickedTask = NULL;
//       ::ReleaseCapture(); RedrawWindow(&rc, NULL, 0x105);
//   }
//   if (m_pHotGroupCaption != NULL || m_pClickedGroupCaption != NULL) {   // +0x7a0 / +0x7a8
//       CRect rc = m_pHotGroupCaption ? m_pHotGroupCaption->m_rect : CRect(0,0,0,0);  // group +0x5c
//       m_pHotGroupCaption = NULL; m_pClickedGroupCaption = NULL;
//       ::ReleaseCapture(); RedrawWindow(&rc, NULL, 0x105);
//   }
//   m_pClickedTask = NULL; m_pClickedGroupCaption = NULL;
// Symbol: ?OnCancelMode@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    impl__OnCancelMode_CDockablePane__IEAAXXZ(reinterpret_cast<CDockablePane*>(pThis));
    if (self->m_pHotTask != nullptr || self->m_pClickedTask != nullptr) {
        RECT rc = {0, 0, 0, 0};
        if (self->m_pHotTask != nullptr) rc = static_cast<const TPTask*>(self->m_pHotTask)->m_rect;
        self->m_pHotTask = nullptr;
        self->m_pClickedTask = nullptr;
        ::ReleaseCapture();
        ::RedrawWindow(pThis->m_hWnd, &rc, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    if (self->m_pHotGroupCaption != nullptr || self->m_pClickedGroupCaption != nullptr) {
        RECT rc = {0, 0, 0, 0};
        if (self->m_pHotGroupCaption != nullptr) rc = static_cast<const TPGroup*>(self->m_pHotGroupCaption)->m_rect;
        self->m_pHotGroupCaption = nullptr;
        self->m_pClickedGroupCaption = nullptr;
        ::ReleaseCapture();
        ::RedrawWindow(pThis->m_hWnd, &rc, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    self->m_pClickedTask = nullptr;
    self->m_pClickedGroupCaption = nullptr;
}

// CMFCTasksPane::OnClickTask(int nGroupNumber, int nTaskNumber, UINT
// uiCommandID, DWORD_PTR dwUserData) -- transcribed from mfc140u RVA 0x147aa0:
//   if (uiCommandID == 0) return;
//   HWND hOwner = m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd);   // CWnd +0xa0 / IAT GetParent
//   CWnd* pOwner = CWnd::FromHandle(hOwner);                          // 0x28ad70
//   ::PostMessage(pOwner->m_hWnd, WM_COMMAND, uiCommandID, 0);        // IAT slot 0x2c72b0 = PostMessageA in the
//                                                                     // ANSI twin (0x2c52d8) -> PostMessageW here
// nGroupNumber, nTaskNumber and dwUserData are not read.
// Deviation: OpenMFC's CWnd does not name m_hWndOwner (that range is padding),
// so ::GetParent(m_hWnd) is used unconditionally -- the same choice
// toolbar/CMFCToolBar.cpp and docking/CBasePane.cpp make.  The FromHandle
// round trip is kept.
// Symbol: ?OnClickTask@CMFCTasksPane@@UEAAXHHI_K@Z
extern "C" void MS_ABI impl__OnClickTask_CMFCTasksPane__UEAAXHHI_K_Z(
    CMFCTasksPane* pThis, int /*nGroupNumber*/, int /*nTaskNumber*/, unsigned int uiCommandID,
    unsigned __int64 /*dwUserData*/)
{
    if (pThis == nullptr) return;
    if (uiCommandID == 0) return;
    // TODO(clean-room): retail prefers CWnd::m_hWndOwner (CWnd + 0xa0).
    HWND hOwner = ::GetParent(pThis->m_hWnd);
    CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOwner);
    if (pOwner == nullptr) return;   // retail dereferences it (a NULL owner faults)
    ::PostMessage(pOwner->m_hWnd, WM_COMMAND, static_cast<WPARAM>(uiCommandID), 0);
}

// STUB.  CMFCTasksPane::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL
// bDisableIfNoHndler) -- retail mfc140u RVA 0x14a710 (slot 147 of the
// CMFCTasksPane vftable at 0x1803135b8; the ANSI twin's slot 147 is the named
// 0x148d80).  Builds a CCmdUI-derived state block on the stack (vftable
// 0x1803131d8, m_pOther = this, m_nIndexMax = group count), walks every task
// of every group and, for each task whose m_uiCommandID is a real command
// (non-zero, not 0xFFFF, outside the 0xF000..0xF1EF / >= 0xFF00 ranges), runs
// pTarget->OnCmdMsg(id, CN_UPDATE_COMMAND_UI, &cmdUI, NULL) (CCmdTarget
// vtable +0x28) and, when bDisableIfNoHndler is set and nothing handled it,
// disables the task through the block's Enable override; it ends with
// CDockablePane::OnUpdateCmdUI(pTarget, bDisableIfNoHndler) (0x45450).
// Blocked: needs that CCmdUI subclass with retail layout and its virtuals.
// Signature corrected to carry `this`.
// Symbol: ?OnUpdateCmdUI@CMFCTasksPane@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCTasksPane__UEAAXPEAVCFrameWnd__H_Z(CMFCTasksPane* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    (void)pThis; (void)pTarget; (void)bDisableIfNoHndler;
}

// CMFCTasksPane::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar*) --
// transcribed from mfc140u RVA 0x147f20 (ANSI twin 0x146590, located by byte
// match; the ANSI body's calls resolve to ?SetScrollPos@CWnd@@QEAAHHHH@Z and
// ?AdjustScroll@CMFCTasksPane@@IEAAXXZ):
//   int nOld = m_nVertScrollOffset, nNew;                        // +0x510
//   switch (nSBCode) {
//     SB_LINEUP:   nNew = nOld - 1;  SB_LINEDOWN: nNew = nOld + 1;
//     SB_PAGEUP:   nNew = nOld - m_nVertScrollPage;              // +0x518
//     SB_PAGEDOWN: nNew = nOld + m_nVertScrollPage;
//     SB_THUMBPOSITION / SB_THUMBTRACK: nNew = nPos;
//     SB_TOP: nNew = 0;  SB_BOTTOM: nNew = m_nVertScrollTotal;   // +0x514
//     default: return;
//   }
//   nNew = max(nNew, 0); nNew = min(nNew, m_nVertScrollTotal - m_nVertScrollPage + 1);
//   m_nVertScrollOffset = nNew;
//   if (nNew == nOld) return;
//   SetScrollPos(SB_VERT, nNew, TRUE); AdjustScroll(); ReposTasks(FALSE);
//   RedrawWindow(NULL, NULL, 0x105);
// The pScrollBar argument is never read.
// Symbol: ?OnVScroll@CMFCTasksPane@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CMFCTasksPane__IEAAXIIPEAVCScrollBar___Z(
    CMFCTasksPane* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* /*pScrollBar*/)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    const int nOld = self->m_nVertScrollOffset;
    int nNew;
    switch (nSBCode) {
    case SB_LINEUP:        nNew = nOld - 1; break;
    case SB_LINEDOWN:      nNew = nOld + 1; break;
    case SB_PAGEUP:        nNew = nOld - self->m_nVertScrollPage; break;
    case SB_PAGEDOWN:      nNew = nOld + self->m_nVertScrollPage; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:    nNew = static_cast<int>(nPos); break;
    case SB_TOP:           nNew = 0; break;
    case SB_BOTTOM:        nNew = self->m_nVertScrollTotal; break;
    default:               return;
    }
    if (nNew < 0) nNew = 0;
    const int nMax = self->m_nVertScrollTotal - self->m_nVertScrollPage + 1;
    if (nNew > nMax) nNew = nMax;
    self->m_nVertScrollOffset = nNew;
    if (nNew == nOld) return;
    impl__SetScrollPos_CWnd__QEAAHHHH_Z(pThis, SB_VERT, nNew, TRUE);
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// STUB.  CMFCTasksPane::PreTranslateMessage(MSG* pMsg) -- retail mfc140u RVA
// 0x149780 (ANSI twin 0x147df0, located by byte match).  Relays mouse
// messages to the tooltip control (m_pToolTip, CDockablePane +0x4b8), handles
// VK_ESCAPE / VK_RETURN / TAB navigation between task windows and falls back to
// CDockablePane::PreTranslateMessage.  Blocked: m_pToolTip has no storage in
// OpenMFC's CDockablePane and the accessibility/focus helpers it relies on
// are stubs.  Signature corrected to carry `this`.
// Symbol: ?PreTranslateMessage@CMFCTasksPane@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCTasksPane__UEAAHPEAUtagMSG___Z(CMFCTasksPane* pThis, MSG* pMsg) {
    (void)pThis; (void)pMsg;
    return FALSE;
}

// CMFCTasksPane::RebuildMenu() -- transcribed from mfc140u RVA 0x148810 (ANSI
// twin 0x146e80):
//   if (m_menuOther.m_hMenu) m_menuOther.DestroyMenu();              // +0x5e8 / +0x5f0, 0x2a8190
//   m_menuOther.Attach(CreateMenu());                                 // 0x2a8100 / 0x14b340
//   m_wndToolBar.UpdateButtons();                                     // 0x143d80, this = +0x7b8
//   if (pBtn = FindButtonByHit(0x17)) pBtn->m_bEnabled /*+0x14*/ = m_iActivePage > 0;        // 0x453c0
//   if (pBtn = FindButtonByHit(0x18)) pBtn->m_bEnabled = m_iActivePage < m_arrHistoryStack.m_nSize - 1;
// CreateMenu is implemented above.  CMFCTasksPaneToolBar::UpdateButtons and
// CDockablePane::FindButtonByHit are still stubs (the latter returns NULL, so
// the two enable-state writes are skipped); both are called as exports so the
// chain completes when they do.  m_menuOther is addressed as a CMenu (HMENU at +8).
// Symbol: ?RebuildMenu@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    static_assert(offsetof(CMenu, m_hMenu) == 8, "CMenu::m_hMenu @8");
    CMenu* pMenu = reinterpret_cast<CMenu*>(self->m_menuOther);
    if (pMenu->m_hMenu != nullptr) impl__DestroyMenu_CMenu__QEAAHXZ(pMenu);
    impl__Attach_CMenu__QEAAHPEAUHMENU_____Z(pMenu, impl__CreateMenu_CMFCTasksPane__QEBAPEAUHMENU____XZ(pThis));
    impl__UpdateButtons_CMFCTasksPaneToolBar__QEAAXXZ();
    struct CaptionButtonView { void* vfptr; void* p8; int m_bHidden; int m_bEnabled; int p18; int p1c; int m_nHit; };
    static_assert(offsetof(CaptionButtonView, m_bEnabled) == 0x14 && offsetof(CaptionButtonView, m_nHit) == 0x20,
                  "CMFCCaptionButton fields read by retail at +0x14 / +0x20");
    const CDockablePane* pDock = reinterpret_cast<const CDockablePane*>(pThis);
    CaptionButtonView* pBtn = static_cast<CaptionButtonView*>(
        impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(pDock, 0x17u));
    if (pBtn != nullptr) pBtn->m_bEnabled = (self->m_iActivePage > 0) ? 1 : 0;
    pBtn = static_cast<CaptionButtonView*>(
        impl__FindButtonByHit_CDockablePane__IEBAPEAVCMFCCaptionButton__I_Z(pDock, 0x18u));
    if (pBtn != nullptr) {
        pBtn->m_bEnabled = (static_cast<long long>(self->m_iActivePage) < self->m_nHistorySize - 1) ? 1 : 0;
    }
}

// CMFCTasksPane::RecalcLayout(BOOL bRedraw) -- transcribed from mfc140u RVA
// 0x14a4b0 (ANSI twin 0x148b20, located by byte match):
//   if (this == NULL) return;                    // retail tests %rcx itself
//   if (m_hWnd == NULL) return;
//   AdjustScroll();                              // 0x148200
//   ReposTasks(FALSE);                           // vtable +0x7d0
//   if (bRedraw) RedrawWindow(NULL, NULL, 0x185);   // RDW_INVALIDATE|RDW_ERASE|
//                                                   // RDW_ALLCHILDREN|RDW_UPDATENOW
// Symbol: ?RecalcLayout@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int bRedraw)
{
    if (pThis == nullptr) return;
    if (pThis->m_hWnd == nullptr) return;
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
    if (bRedraw != 0) {
        ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW);
    }
}

// CMFCTasksPane::RemoveAllGroups(int nPage) -- transcribed from mfc140u RVA
// 0x1461c0 (ANSI twin 0x144830, located by byte match):
//   if (nPage >= m_lstTasksPanes.m_nCount || nPage < 0) return;
//   pPage = FindIndex(nPage)->data; (NULL node -> return)
//   for (g = m_lstTaskGroups.head; g; ) {
//       pGroup = g->data; pos = g; g = g->pNext;
//       if (pGroup->m_pPage == pPage) { m_lstTaskGroups.RemoveAt(pos); delete pGroup; }
//   }
//   AdjustScroll(); ReposTasks(FALSE); RedrawWindow(NULL, NULL, 0x105);
// Symbol: ?RemoveAllGroups@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RemoveAllGroups_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nPage)
{
    if (pThis == nullptr) return;
    TpNode* pageNode = NodeAt(PageList(pThis), nPage);
    if (pageNode == nullptr) return;
    const CObject* pPage = pageNode->data;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; ) {
        TPGroup* pGroup = reinterpret_cast<TPGroup*>(g->data);
        TpNode* pos = g;
        g = g->pNext;
        if (pGroup->m_pPage != pPage) continue;
        ListRemove(GroupList(pThis), pos);
        DeleteViaVtable(pGroup);
    }
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// CMFCTasksPane::RemoveAllPages() -- transcribed from mfc140u RVA 0x145de0
// (ANSI twin 0x144450; the u map has no name for it, the body sits directly
// after RemovePage and matches the ANSI one instruction for instruction):
//   int nOld = m_iActivePage; m_iActivePage = 0; ChangeActivePage(0, nOld);
//   m_arrHistoryStack.SetSize(0); m_arrHistoryStack.Add(0);         // 0x15034 twice
//   pFirst = m_lstTasksPanes.m_pNodeHead->data;
//   for (g = groups.head; g; ) { pos = g; grp = g->data; g = g->pNext;
//       if (grp->m_pPage != NULL && grp->m_pPage != pFirst) { RemoveAt(pos); delete grp; } }
//   while (m_lstTasksPanes.m_nCount > 1) { p = m_lstTasksPanes.RemoveHead(); if (p) delete p; }   // 0x231fb0
//   tail jmp RebuildMenu();                                         // 0x148810
// Retail reads the page-list head inside the group loop (only when a group
// has a page); with no page the read would fault, so a NULL head yields a
// NULL pFirst here.
// Symbol: ?RemoveAllPages@CMFCTasksPane@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllPages_CMFCTasksPane__QEAAXXZ(CMFCTasksPane* pThis)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    const int nOld = self->m_iActivePage;
    self->m_iActivePage = 0;
    impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, 0, nOld);
    HistorySetSize(self, 0);
    HistoryAdd(self, 0);
    TpNode* firstNode = HeadNode(PageList(pThis));
    const CObject* pFirst = firstNode != nullptr ? firstNode->data : nullptr;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; ) {
        TpNode* pos = g;
        TPGroup* pGroup = reinterpret_cast<TPGroup*>(g->data);
        g = g->pNext;
        if (pGroup->m_pPage == nullptr || pGroup->m_pPage == pFirst) continue;
        ListRemove(GroupList(pThis), pos);
        DeleteViaVtable(pGroup);
    }
    while (ListCount(PageList(pThis)) > 1) {
        CObject* pPage = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(PageList(pThis));
        DeleteViaVtable(pPage);
    }
    impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(pThis);
}

// CMFCTasksPane::RemoveGroup(int nGroup) -- transcribed from mfc140u RVA
// 0x146120 (ANSI twin 0x144790; the u map has no name for it, the body was
// located by byte match):
//   if (nGroup >= m_lstTaskGroups.m_nCount || nGroup < 0) return;
//   node = FindIndex(nGroup); if (!node) return;
//   pGroup = node->data; m_lstTaskGroups.RemoveAt(node);          // 0x2320d0
//   if (pGroup) delete pGroup;                                    // vslot 1
//   AdjustScroll(); ReposTasks(FALSE); RedrawWindow(NULL, NULL, 0x105);
// Symbol: ?RemoveGroup@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RemoveGroup_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nGroup)
{
    if (pThis == nullptr) return;
    TpNode* node = NodeAt(GroupList(pThis), nGroup);
    if (node == nullptr) return;
    CObject* pGroup = node->data;
    ListRemove(GroupList(pThis), node);
    DeleteViaVtable(pGroup);
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// CMFCTasksPane::RemovePage(int nPage) -- transcribed from mfc140u RVA
// 0x145af0 (ANSI twin 0x144160, located by byte match):
//   if (nPage <= 0) return;                        // page 0 is never removed
//   ENSURE(nPage < m_lstTasksPanes.m_nCount); pPage = FindIndex(nPage)->data;
//   ENSURE(0 <= m_iActivePage < m_arrHistoryStack.m_nSize);
//   if (m_arrHistoryStack.m_nSize == 1) {
//       SaveHistory(0);                                              // 0x1488c0
//       nOld = m_iActivePage; m_iActivePage = m_nSize - 1; ChangeActivePage(m_iActivePage, nOld);
//   } else if (m_arrHistoryStack[m_iActivePage] == nPage) {
//       nOld = m_iActivePage;
//       m_iActivePage = (nOld >= m_nSize - 1) ? nOld - 1 : m_nSize - 1;
//       ChangeActivePage(m_iActivePage, nOld);
//   } else if (this != NULL && m_hWnd != NULL) {
//       RebuildMenu(); AdjustScroll(); ReposTasks(FALSE); RedrawWindow(NULL, NULL, 0x105);
//   }
//   for (g = groups.head; g; ) { pos = g; grp = g->data; g = g->pNext;
//       if (grp->m_pPage == pPage) { m_lstTaskGroups.RemoveAt(pos); delete grp; } }
//   ENSURE(nPage < m_lstTasksPanes.m_nCount); node = FindIndex(nPage); ENSURE(node);
//   m_lstTasksPanes.RemoveAt(node); if (pPage) delete pPage;
//   CArray<int,int> arrOld; arrOld.Copy(m_arrHistoryStack);         // 0xf0924
//   m_arrHistoryStack.SetSize(0);
//   int nPrev = nPage, nLast = -1, nActive = 0;
//   for (i = 0; i < arrOld.m_nSize; i++) {
//       v = arrOld[i];
//       if (v != nPrev) {
//           if (v < nPage)      { m_arrHistoryStack.Add(v);     nPrev = v; nLast++; }
//           else if (v > nPage) { m_arrHistoryStack.Add(v - 1); nPrev = v; nLast++; }
//           // v == nPage: dropped, nPrev unchanged
//       }
//       if (i == m_iActivePage) nActive = nLast;
//   }
//   m_iActivePage = nActive; RebuildMenu(); arrOld.~CArray();
// SaveHistory (a second-batch symbol) is still a stub in this file; its
// placeholder parameter list was corrected to carry `this`.
// Symbol: ?RemovePage@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__RemovePage_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nPage)
{
    if (pThis == nullptr) return;
    if (nPage <= 0) return;
    TP* self = View(pThis);
    TpNode* pageNode = NodeAt(PageList(pThis), nPage);
    if (pageNode == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (self->m_iActivePage < 0 || self->m_iActivePage >= self->m_nHistorySize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    CObject* pPage = pageNode->data;
    int nCur = 0;
    if (self->m_nHistorySize == 1) {
        impl__SaveHistory_CMFCTasksPane__IEAAXH_Z(pThis, 0);
        const int nOld = self->m_iActivePage;
        self->m_iActivePage = static_cast<int>(self->m_nHistorySize) - 1;
        impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
    } else if (HistoryAt(self, self->m_iActivePage, nCur) && nCur == nPage) {
        const int nOld = self->m_iActivePage;
        const int nLastIdx = static_cast<int>(self->m_nHistorySize) - 1;
        self->m_iActivePage = (nOld >= nLastIdx) ? nOld - 1 : nLastIdx;
        impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
    } else if (pThis->m_hWnd != nullptr) {
        impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(pThis);
        RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; ) {
        TpNode* pos = g;
        TPGroup* pGroup = reinterpret_cast<TPGroup*>(g->data);
        g = g->pNext;
        if (pGroup->m_pPage != pPage) continue;
        ListRemove(GroupList(pThis), pos);
        DeleteViaVtable(pGroup);
    }
    pageNode = NodeAt(PageList(pThis), nPage);
    if (pageNode == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    ListRemove(PageList(pThis), pageNode);
    DeleteViaVtable(pPage);

    // arrOld.Copy(m_arrHistoryStack)
    const long long nOldCount = self->m_nHistorySize;
    int* arrOld = nullptr;
    if (nOldCount > 0 && self->m_pHistoryData != nullptr) {
        arrOld = static_cast<int*>(std::malloc(static_cast<size_t>(nOldCount) * sizeof(int)));
        if (arrOld == nullptr) return;
        std::memcpy(arrOld, self->m_pHistoryData, static_cast<size_t>(nOldCount) * sizeof(int));
    }
    HistorySetSize(self, 0);
    int nPrev = nPage;
    int nLast = -1;
    int nActive = 0;
    for (long long i = 0; i < nOldCount && arrOld != nullptr; ++i) {
        const int v = arrOld[i];
        if (v != nPrev) {
            if (v < nPage) {
                HistoryAdd(self, v);
                nPrev = v;
                ++nLast;
            } else if (v > nPage) {
                HistoryAdd(self, v - 1);
                nPrev = v;
                ++nLast;
            }
        }
        if (i == self->m_iActivePage) nActive = nLast;
    }
    self->m_iActivePage = nActive;
    impl__RebuildMenu_CMFCTasksPane__IEAAXXZ(pThis);
    std::free(arrOld);
}

// CMFCTasksPane::RemoveTask(int nGroup, int nTask, BOOL bRedraw) -- transcribed
// from mfc140u RVA 0x146950 (ANSI twin 0x144fc0, located by byte match):
//   if (nGroup out of range) return FALSE;  pGroup = FindIndex(nGroup)->data;
//   if (nTask >= pGroup->m_lstTasks.m_nCount || nTask < 0) return FALSE;
//   node = FindIndex(nTask); if (!node) return FALSE;
//   if (node->data) delete node->data;                            // vslot 1
//   pGroup->m_lstTasks.RemoveAt(node);                            // 0x2320d0
//   AdjustScroll(); ReposTasks(FALSE);
//   if (bRedraw) RedrawWindow(NULL, NULL, 0x105);
//   return TRUE;
// Symbol: ?RemoveTask@CMFCTasksPane@@QEAAHHHH@Z
extern "C" int MS_ABI impl__RemoveTask_CMFCTasksPane__QEAAHHHH_Z(CMFCTasksPane* pThis, int nGroup, int nTask, int bRedraw)
{
    if (pThis == nullptr) return FALSE;
    TpNode* gnode = NodeAt(GroupList(pThis), nGroup);
    if (gnode == nullptr) return FALSE;
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(gnode->data);
    TpNode* tnode = NodeAt(TaskList(pGroup), nTask);
    if (tnode == nullptr) return FALSE;
    DeleteViaVtable(tnode->data);
    ListRemove(TaskList(pGroup), tnode);
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
    if (bRedraw != 0) {
        ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    return TRUE;
}

// STUB.  CMFCTasksPane::ReposTasks(BOOL bCalcHeightOnly) -- retail mfc140
// RVA 0x142eb0 (ANSI twin; the export has no entry in the mfc140u map and the
// byte search did not locate it, but the CMFCTasksPane vftable at mfc140u
// 0x1803135b8 holds 0x144840 at +0x7d0, the slot the refresh idiom calls with
// %edx = 0).  The ANSI body is 1,011 instructions: it takes a CClientDC,
// selects fonts through SetFont@CMFCTasksPane (a second-batch symbol) and
// CDC::SelectObject(CFont*), measures text with GetTextExtentPoint32 /
// GetTextMetrics, queries the visual manager 22 times through the inline
// GetInstance helper at 0x97f4 (see docking/CDockablePane.cpp) plus 15 guarded
// virtual calls, clips with CreateRectRgnIndirect / SelectClipRgn, positions
// task windows with CWnd::SetWindowPos / ShowWindow via CWnd::FromHandle, and
// ends with InvalidateRect / UpdateWindow.  Blocked: the visual-manager
// metrics, the fonts and the per-record rectangles it fills are not
// reproduced by OpenMFC.  Signature corrected to carry `this`.
// Symbol: ?ReposTasks@CMFCTasksPane@@MEAAHH@Z
extern "C" int MS_ABI impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(CMFCTasksPane* pThis, int bCalcHeightOnly) {
    (void)pThis; (void)bCalcHeightOnly;
    return 0;
}

// CMFCTasksPane::SaveHistory(int nPage) -- transcribed from mfc140u RVA
// 0x1488c0 (ANSI twin 0x146f30 carries the name; called from RemovePage,
// SetActivePage):
//   ENSURE(0 <= m_iActivePage < m_arrHistoryStack.m_nSize)  else AfxThrowInvalidArgException
//   if (m_arrHistoryStack[m_iActivePage] == nPage) return;
//   if (m_iActivePage < m_nSize - 1)
//       m_arrHistoryStack.RemoveAt(m_iActivePage + 1, m_nSize - m_iActivePage - 1);   // 0x1b880
//   if (m_nSize == m_nMaxHistory) {                                  // +0x548
//       m_arrHistoryStack.RemoveAt(0, 1);
//       if (m_iActivePage > 0) m_iActivePage--;
//   }
//   m_arrHistoryStack.Add(nPage);                                    // inline SetSize(n+1) 0x15034
// Symbol: ?SaveHistory@CMFCTasksPane@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveHistory_CMFCTasksPane__IEAAXH_Z(CMFCTasksPane* pThis, int nPage)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    int nCur = 0;
    if (!HistoryAt(self, self->m_iActivePage, nCur)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (nCur == nPage) return;
    const long long nActive = self->m_iActivePage;
    if (nActive < self->m_nHistorySize - 1) {
        HistoryRemoveAt(self, nActive + 1, self->m_nHistorySize - nActive - 1);
    }
    if (self->m_nHistorySize == static_cast<long long>(self->m_nMaxHistory)) {
        HistoryRemoveAt(self, 0, 1);
        if (self->m_iActivePage > 0) self->m_iActivePage--;
    }
    HistoryAdd(self, nPage);
}

// STUB.  CMFCTasksPane::SaveState(LPCTSTR lpszProfileName, int nIndex, UINT
// uiID) -- retail entry RVA 0x1482a0 (mfc140; the mfc140u map has no RVA for
// this export, and the two images share the body byte-for-byte):
//   strProfile = AFXGetRegPath("MFCTasksPanes", lpszProfileName);   // 0xd2540
//   if (nIndex == -1) nIndex = GetDlgCtrlID();                      // 0x2a78b0
//   strSection.Format(uiID == -1 ? "%TsMFCTasksPane-%d"
//                                : "%TsMFCTasksPane-%d%x",
//                     strProfile, nIndex [, uiID]);                 // 0xda80
//   CMemFile file(0x400);                                           // 0x229510
//   CArchive ar(&file, CArchive::store, 0x1000, NULL);              // 0x1cf500
//   this->Serialize(ar);                  // vtable +0x10, i.e. the Serialize below
//   ar.Flush() (0x1cfb90); ar.~CArchive() (0x1cf6b0);
//   UINT nLen = (UINT)file.GetLength();   // inlined: m_nFileSize, file +0x40
//   BYTE* pBuf = file.Detach();           // inlined: m_lpBuffer (+0x48) is taken,
//                                         // then m_lpBuffer / m_nFileSize /
//                                         // m_nBufferSize / m_nPosition zeroed
//   and:
//   if (pBuf != NULL) {
//       CSettingsStoreSP regSP;
//       CSettingsStore& reg = regSP.Create(FALSE, FALSE);           // 0x12b320
//       if (reg.<vslot 5>(strSection))            // the key-creating virtual
//           reg.<vslot 12>("Settings", pBuf, nLen);   // the blob-writing virtual
//       free(pBuf);                                                 // CRT free
//       reg's deleting destructor (vslot 1, flags = 1) runs;
//   }
//   file.~CMemFile();                                               // 0x229640
//   return CPane::SaveState(lpszProfileName, nIndex, uiID);         // 0xa24e0
// Blocked by this class's Serialize (below), which is a stub: the CMemFile
// would stay empty, so pBuf would be NULL, retail's `if (pBuf != NULL)` would
// never fire and the whole function would collapse to its
// `return CPane::SaveState(...)` tail -- reporting a successful save that
// stored nothing.  Everything else the body needs is exported by this tree:
// ?AFXGetRegPath@@ (featurepack/CMFC_misc_stubs.cpp),
// ?Create@CSettingsStoreSP@@ (core/app/CSettingsStoreSP.cpp),
// ?SaveState@CPane@@ (featurepack/docking/CPane.cpp) and ?GetDlgCtrlID@CWnd@@
// (core/window/Thunks.cpp); core/app/CSettingsStore.cpp also exports
// ?CreateKey@CSettingsStore@@UEAAHPEB_W@Z and
// ?Write@CSettingsStore@@UEAAHPEB_WPEAEI@Z, whose signatures are the ones the
// two virtual calls above use, though the slot numbers were not confirmed
// against OpenMFC's vtable.  So this becomes implementable as soon as
// Serialize is.  Signature corrected to carry `this`.
// Symbol: ?SaveState@CMFCTasksPane@@UEAAHPEB_WHI@Z
extern "C" int MS_ABI impl__SaveState_CMFCTasksPane__UEAAHPEB_WHI_Z(
    CMFCTasksPane* pThis, const wchar_t* lpszProfileName, int nIndex, unsigned int uiID) {
    (void)pThis; (void)lpszProfileName; (void)nIndex; (void)uiID;
    return FALSE;
}

// STUB.  CMFCTasksPane::Serialize(CArchive& ar) -- retail entry RVA 0x1475f0
// (mfc140; not in the mfc140u map, bodies are identical between the images):
//   CDockablePane::Serialize(ar);                                   // 0x45660
//   nine ints are read or written in this order -- m_nVertMargin (+0x524),
//   m_nHorzMargin (+0x528), m_nGroupVertOffset (+0x52c),
//   m_nGroupCaptionHeight (+0x530), m_nGroupCaptionHorzOffset (+0x534),
//   m_nGroupCaptionVertOffset (+0x538), m_nTasksHorzOffset (+0x53c),
//   m_nTasksIconHorzOffset (+0x540), m_nTasksIconVertOffset (+0x544) --
//   each as an inlined `ar >> n` / `ar << n` against the archive buffer
//   (m_lpBufCur +0x38, m_lpBufMax +0x40, FillBuffer 0x1cfc70 / Flush 0x1cfb90;
//   the load/store branch is `m_nMode & CArchive::load`, the byte at +0x20).
//   loading: then one more int -- the saved active page.  If it is negative or
//     >= m_lstTasksPanes.m_nCount it is forced to 0.  A local CStringArray is
//     Serialize'd in (0x1d3470) and, when its size equals the page count, each
//     page's m_strName (page +8) is assigned from it (0x0dee0).  Then
//     SetActivePage(nPage) (0x147170), m_nVertScrollOffset = 0 (+0x510),
//     AdjustScroll() (0x146870), then m_strCaption (+0x678) is read through
//     an internal archive-to-CString helper (0x1b58c) and UpdateCaption()
//     (0x149dd0) runs.
//   storing: m_arrHistoryStack[m_iActivePage] is written (with retail's
//     ENSURE on the index), a local CStringArray is filled with every page's
//     m_strName via SetAtGrow (0x1d30e0) and Serialize'd out, then
//     m_strCaption is written through the store-direction helper (0x1b8b0).
// Blocked: every archive access above is an inlined read/write of retail's
// own CArchive fields, and OpenMFC's CArchive (include/openmfc/afx.h) has a
// different layout -- m_nMode at +0x10, m_lpBufCur at +0x20 -- so none of
// those offsets can be reused.  Rebuilding the body out of OpenMFC's exported
// thunks gets most of the way -- ?Serialize@CDockablePane@@, the CArchive
// Read/Write thunks (core/runtime/Thunks.cpp) for the ten raw ints,
// ??0CStringArray@@ / ?Serialize@CStringArray@@ / ?SetAtGrow@CStringArray@@
// (core/collections/CStringArray.cpp) for the page names -- but there is no
// exported CArchive insertion or extraction for a CString itself (the tree has
// ??5@YAAEAVCArchive@@ overloads for the collection classes and for
// COleVariant only).  Retail's store-direction helper (0x1b8b0, not an export)
// is the inlined CString operator<<: AfxWriteStringLength (0x1cedf0) then
// CArchive::Write (0x1cfa20); the load helper (0x1b58c) is the matching
// operator>> built on AfxReadStringLength.  Both length helpers are exported
// by this tree but are stubs (core/runtime/CArchive.cpp: AfxReadStringLength
// returns 0, AfxWriteStringLength writes nothing), so m_strCaption could not
// be carried either way.  A
// Serialize that silently dropped the caption would write a stream whose
// layout is neither retail's nor readable by a future faithful LoadState, and
// both LoadState and SaveState above are stubs today, so nothing in this DLL
// produces or consumes the stream anyway.  Signature corrected to carry
// `this`.
// Symbol: ?Serialize@CMFCTasksPane@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCTasksPane__UEAAXAEAVCArchive___Z(
    CMFCTasksPane* pThis, CArchive* pAr) {
    (void)pThis; (void)pAr;
}

// CMFCTasksPane::SetActivePage(int nPage) -- transcribed from mfc140u RVA
// 0x148b00 (ANSI twin 0x147170 carries the name; the CMFCTasksPane callers
// OnPressHomeButton / OnOther / OnPressOtherButton tail-jump here):
//   ENSURE(0 <= m_iActivePage < m_arrHistoryStack.m_nSize)  else AfxThrowInvalidArgException
//   if (m_arrHistoryStack[m_iActivePage] == nPage) return;
//   SaveHistory(nPage);                                              // 0x1488c0
//   int nOld = m_iActivePage;                                        // read AFTER SaveHistory
//   m_iActivePage = m_arrHistoryStack.m_nSize - 1;
//   ChangeActivePage(m_iActivePage, nOld);                           // 0x148990
// Placeholder parameter list (`int p0`) replaced by the retail one.
// Symbol: ?SetActivePage@CMFCTasksPane@@QEAAXH@Z
extern "C" void MS_ABI impl__SetActivePage_CMFCTasksPane__QEAAXH_Z(CMFCTasksPane* pThis, int nPage)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    int nCur = 0;
    if (!HistoryAt(self, self->m_iActivePage, nCur)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (nCur == nPage) return;
    impl__SaveHistory_CMFCTasksPane__IEAAXH_Z(pThis, nPage);
    const int nOld = self->m_iActivePage;
    self->m_iActivePage = static_cast<int>(self->m_nHistorySize) - 1;
    impl__ChangeActivePage_CMFCTasksPane__IEAAXHH_Z(pThis, self->m_iActivePage, nOld);
}

// STUB.  CMFCTasksPane::SetCaptionButtons() -- retail entry RVA 0x1471d0
// (mfc140; no mfc140u RVA in the map, identical body):
//   CDockablePane::SetCaptionButtons();                              // 0x45f30
//   arr = m_arrButtons;  // CDockablePane, this+0x488; declared in
//                        // afxdockablepane.h as
//                        // CTypedPtrArray<CObArray, CMFCCaptionButton*>
//                        // (m_pData +0x490, m_nSize +0x498).  The SetAtGrow
//                        // call lands on 0x1d28a0, whose export name is
//                        // CPtrArray::SetAtGrow (CObArray::SetAtGrow has no
//                        // entry of its own in the RVA map; presumably the
//                        // two identical bodies were folded).
//   p = operator new(0x38) (0x2840);   // sizeof(CMFCCaptionButton)
//        vfptr = the CMFCCaptionButton vftable;
//        m_bPushed/m_bFocused/m_bHidden = FALSE  (+0x8/+0xc/+0x10),
//        m_bEnabled = TRUE (+0x14), m_bDroppedDown = FALSE (+0x18),
//        m_bLeftAlign = TRUE (+0x1c), m_nHit = 0x17 (+0x20),
//        m_clrForeground = (COLORREF)-1 (+0x24), m_ptOrg = (0,0) (+0x28),
//        m_bIsMiniFrameButton = FALSE (+0x30);
//   arr.SetAtGrow(arr.m_nSize, p);                                   // 0x1d28a0
//   the same again with m_nHit = 0x18;
//   p = operator new(0x48);            // sizeof(CMFCCaptionMenuButton)
//        same fields, except m_bLeftAlign = FALSE and m_nHit = 0x19, plus the
//        derived class's own trailing members zeroed;
//   arr.SetAtGrow(arr.m_nSize, p);                                   // tail jmp
// (the member offsets are those of the retail CMFCCaptionButton declaration in
// afxcaptionbutton.h, whose size 56 is the 0x38 allocated above; 0x17/0x18/
// 0x19 are the same hit codes OnPressButtons above dispatches on -- back,
// forward and the history menu button.)
// Blocked: OpenMFC's CDockablePane has no m_arrButtons storage at all --
// featurepack/docking/CDockablePane.cpp notes that offsets from +0x478 up are
// past the end of its 1144-byte object, which is also why
// CDockablePane::FindButtonByHit is a NULL-returning stub there.  There is
// nothing to append the three buttons to.  Signature corrected to carry `this`.
// Symbol: ?SetCaptionButtons@CMFCTasksPane@@MEAAXXZ
extern "C" void MS_ABI impl__SetCaptionButtons_CMFCTasksPane__MEAAXXZ(CMFCTasksPane* pThis) {
    (void)pThis;
}

// CMFCTasksPane::SetFont(CDC* pDC) -- transcribed from the retail body at
// RVA 0x1456a0 (mfc140; the mfc140u map has no RVA for this export and the
// bodies are byte-identical, so only the control flow is quoted):
//   HFONT h = m_hFont;                                        // +0x550
//   if (h == NULL) h = ::GetStockObject(DEFAULT_GUI_FONT);    // 0x11, GDI32
//   return ::SelectObject(pDC ? pDC->m_hDC : NULL, h);        // CDC +8, tail jmp
// Retail passes a NULL HDC straight to ::SelectObject when pDC is NULL; that
// is reproduced rather than short-circuited.  Placeholder parameter list
// (`void** p0`) replaced by the one the mangled name describes.
// Symbol: ?SetFont@CMFCTasksPane@@IEAAPEAUHFONT__@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__SetFont_CMFCTasksPane__IEAAPEAUHFONT____PEAVCDC___Z(
    CMFCTasksPane* pThis, CDC* pDC)
{
    if (pThis == nullptr) return nullptr;
    HFONT hFont = View(pThis)->m_hFont;
    if (hFont == nullptr) {
        hFont = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    }
    return ::SelectObject(pDC != nullptr ? pDC->m_hDC : nullptr, hFont);
}

// CMFCTasksPane::SetGroupTextColor(int nGroup, COLORREF color, COLORREF
// colorHot) -- transcribed from the retail body at RVA 0x1449e0 (mfc140):
//   if (nGroup >= m_lstTaskGroups.m_nCount || nGroup < 0) return FALSE;
//   node = m_lstTaskGroups.head; while (nGroup--) node = node->pNext;
//   if (node == NULL) return FALSE;
//   pGroup = node->data;
//   pGroup->m_clrText    (+0x90) = color;
//   pGroup->m_clrTextHot (+0x94) = colorHot;
//   ::InvalidateRect(m_hWnd, &pGroup->m_rect /*+0x5c*/, TRUE);
//   ::UpdateWindow(m_hWnd);
//   return TRUE;
// (bounds test + walk = the FindIndex thunk; see the storage note at the top.)
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?SetGroupTextColor@CMFCTasksPane@@QEAAHHKK@Z
extern "C" int MS_ABI impl__SetGroupTextColor_CMFCTasksPane__QEAAHHKK_Z(
    CMFCTasksPane* pThis, int nGroup, unsigned long color, unsigned long colorHot)
{
    if (pThis == nullptr) return FALSE;
    TpNode* node = NodeAt(GroupList(pThis), nGroup);
    if (node == nullptr) return FALSE;
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(node->data);
    pGroup->m_clrText    = static_cast<COLORREF>(color);
    pGroup->m_clrTextHot = static_cast<COLORREF>(colorHot);
    ::InvalidateRect(pThis->m_hWnd, &pGroup->m_rect, TRUE);
    ::UpdateWindow(pThis->m_hWnd);
    return TRUE;
}

// CMFCTasksPane::SetIconsList(UINT uiImageListResID, int cx, COLORREF
// clrTransparent) -- transcribed from the retail body at RVA 0x143f40
// (mfc140; the mfc140u map has no RVA for this export, and the bodies are
// byte-identical between the two images):
//   CBitmap bmp;
//   HINSTANCE h = AfxFindResourceHandle(MAKEINTRESOURCE((WORD)uiImageListResID),
//                                       RT_BITMAP);                  // 0x2aca40
//   if (!bmp.Attach(::LoadBitmap(h, MAKEINTRESOURCE((WORD)uiImageListResID))))
//       return FALSE;                                                // 0x2a1e10
//   CImageList il;
//   BITMAP bm; ::GetObject(bmp.m_hObject, sizeof(BITMAP), &bm);
//   UINT nMask = (clrTransparent != (COLORREF)-1) ? ILC_MASK : 0;
//   switch (bm.bmBitsPixel) {              // the flag value equals the depth
//   case 4:  f = ILC_COLOR4  | nMask; break;
//   case 8:  f = ILC_COLOR8  | nMask; break;
//   case 16: f = ILC_COLOR16 | nMask; break;
//   case 24: f = ILC_COLOR24 | nMask; break;
//   case 32: f = ILC_COLOR32 | ILC_MASK; break;   // mask forced on
//   default: f = ILC_COLOR4  | nMask; break; }
//   il.Create(cx, bm.bmHeight, f, 0, 0);                             // 0x294db0
//   if (bm.bmBitsPixel == 32 && clrTransparent == (COLORREF)-1)
//        ImageList_Add(il.m_hImageList, bmp.m_hObject, NULL);        // 0x14a574
//   else ImageList_AddMasked(il.m_hImageList, bmp.m_hObject, clrTransparent);
//                                                                    // 0x1c660
//   SetIconsList(il.m_hImageList);                                   // 0x143e90
//   /* il and bmp destructors: ImageList_Destroy then DeleteObject */
//   return TRUE;
// Retail returns TRUE even when CImageList::Create failed, and passes the
// resulting NULL handle on to the HIMAGELIST overload (which then clears the
// pane's icon list); that is reproduced.
// (0x14a574 / 0x1c660 are not exports: they are MFC's activation-context
// wrappers that resolve comctl32 by name -- the name strings they pass are
// "ImageList_Add" and "ImageList_AddMasked".)
// Deviations: the CBitmap / CImageList locals are replaced by the raw handles
// and the GDI32 / comctl32 entry points their members call.  CBitmap has no
// exported constructor (it is inline in the MFC headers); CImageList does
// (core/gdi/CImageList.cpp), but its Create/Attach would only add the
// temporary to the HIMAGELIST handle map for the lifetime of this call, which
// is not observable from outside.  The two ImageList_Add* calls are skipped
// when the list handle is NULL instead of being made with a NULL handle.
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?SetIconsList@CMFCTasksPane@@QEAAHIHK@Z
extern "C" int MS_ABI impl__SetIconsList_CMFCTasksPane__QEAAHIHK_Z(
    CMFCTasksPane* pThis, unsigned int uiImageListResID, int cx, unsigned long clrTransparent)
{
    if (pThis == nullptr) return FALSE;

    const wchar_t* lpszRes = MAKEINTRESOURCEW(static_cast<WORD>(uiImageListResID));
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszRes, RT_BITMAP);
    HBITMAP hBmp = ::LoadBitmapW(hInst, lpszRes);
    if (hBmp == nullptr) return FALSE;      // retail: CBitmap::Attach(NULL) fails

    BITMAP bm;
    std::memset(&bm, 0, sizeof(bm));
    ::GetObjectW(hBmp, static_cast<int>(sizeof(BITMAP)), &bm);

    const UINT nMask = (clrTransparent != static_cast<unsigned long>(-1)) ? ILC_MASK : 0u;
    UINT nFlags;
    switch (bm.bmBitsPixel) {
    case 4:  nFlags = ILC_COLOR4  | nMask;    break;
    case 8:  nFlags = ILC_COLOR8  | nMask;    break;
    case 16: nFlags = ILC_COLOR16 | nMask;    break;
    case 24: nFlags = ILC_COLOR24 | nMask;    break;
    case 32: nFlags = ILC_COLOR32 | ILC_MASK; break;
    default: nFlags = ILC_COLOR4  | nMask;    break;
    }

    HIMAGELIST hIL = ::ImageList_Create(cx, bm.bmHeight, nFlags, 0, 0);
    if (hIL != nullptr) {
        if (bm.bmBitsPixel == 32 && clrTransparent == static_cast<unsigned long>(-1)) {
            ::ImageList_Add(hIL, hBmp, nullptr);
        } else {
            ::ImageList_AddMasked(hIL, hBmp, static_cast<COLORREF>(clrTransparent));
        }
    }
    impl__SetIconsList_CMFCTasksPane__QEAAXPEAU_IMAGELIST___Z(pThis, hIL);
    if (hIL != nullptr) ::ImageList_Destroy(hIL);
    ::DeleteObject(hBmp);
    return TRUE;
}

// CMFCTasksPane::SetIconsList(HIMAGELIST hIcons) -- transcribed from the
// retail body at RVA 0x143e90 (mfc140; no mfc140u RVA in the map, identical
// body):
//   if (m_lstIcons.m_hImageList != NULL) m_lstIcons.DeleteImageList();  // 0x294d20
//                       // Detach()s the handle and ImageList_Destroy()s it
//   if (hIcons == NULL) { *(void**)&m_sizeIcon = 0; }   // both halves zeroed
//   else {
//       m_lstIcons.Attach(ImageList_Duplicate(
//           CImageList::FromHandle(hIcons)->m_hImageList));   // 0x294d50/0x29a7c4/0x294f00
//       ImageList_GetIconSize(hIcons, &m_sizeIcon.cx, &m_sizeIcon.cy);  // 0x6f1e0
//   }
//   AdjustScroll();                                                  // 0x146870
//   ReposTasks(FALSE);                                               // vtable +0x7d0
//   ::RedrawWindow(m_hWnd, NULL, NULL,
//                  RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);          // tail jmp
// m_lstIcons is the CImageList at +0x768; its m_hImageList is at +0x770.
// Deviations: the CImageList wrapper is replaced by the comctl32 entry points
// it calls (CImageList::FromHandle(h)->m_hImageList is h, so the round trip is
// elided).  Retail's Attach (0x294f00) also enters the handle into MFC's
// permanent HIMAGELIST map and DeleteImageList's Detach removes it; that map
// bookkeeping is not reproduced.  The refresh
// tail goes through this file's RetailRefresh helper, so a derived ReposTasks
// override is not honoured.  Per the KNOWN GAP at the top of this file,
// +0x770 is only meaningful once the ctor thunk initialises the retail member
// range; until then the handle destroyed first is whatever that slot holds.
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?SetIconsList@CMFCTasksPane@@QEAAXPEAU_IMAGELIST@@@Z
extern "C" void MS_ABI impl__SetIconsList_CMFCTasksPane__QEAAXPEAU_IMAGELIST___Z(
    CMFCTasksPane* pThis, HIMAGELIST hIcons)
{
    if (pThis == nullptr) return;
    TP* self = View(pThis);
    HIMAGELIST* phIL = reinterpret_cast<HIMAGELIST*>(self->m_lstIcons + 8);
    if (*phIL != nullptr) {
        ::ImageList_Destroy(*phIL);
        *phIL = nullptr;
    }
    if (hIcons == nullptr) {
        self->m_sizeIcon.cx = 0;
        self->m_sizeIcon.cy = 0;
    } else {
        *phIL = ::ImageList_Duplicate(hIcons);
        int cx = 0, cy = 0;
        ::ImageList_GetIconSize(hIcons, &cx, &cy);
        self->m_sizeIcon.cx = cx;
        self->m_sizeIcon.cy = cy;
    }
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// CMFCTasksPane::SetPageCaption(int nPageIdx, LPCTSTR lpszName) --
// transcribed from the retail body at RVA 0x147d70 (mfc140, whose export
// spells the parameter PEBD; mfc140u exports the PEB_W spelling of the same
// bytes):
//   if (lpszName == NULL)                                AfxThrowInvalidArgException();
//   if (nPageIdx >= m_lstTasksPanes.m_nCount || nPageIdx < 0)
//                                                        AfxThrowInvalidArgException();
//   node = m_lstTasksPanes.head; while (nPageIdx--) node = node->pNext;
//   if (node == NULL)                                    AfxThrowInvalidArgException();
//   node->data->m_strName.SetString(lpszName, strlen(lpszName));  // page +0x8
//   UpdateCaption();                                     // tail jmp 0x149dd0
// (bounds test + walk = the FindIndex thunk; see the storage note at the top.
// UpdateCaption is still a stub in this file, so the caption is not repainted.)
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?SetPageCaption@CMFCTasksPane@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetPageCaption_CMFCTasksPane__QEAAXHPEB_W_Z(
    CMFCTasksPane* pThis, int nPageIdx, const wchar_t* lpszName)
{
    if (pThis == nullptr) return;
    if (lpszName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    TpNode* node = NodeAt(PageList(pThis), nPageIdx);
    if (node == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    TPPage* pPage = reinterpret_cast<TPPage*>(node->data);
    *reinterpret_cast<CString*>(&pPage->m_strName) = lpszName;
    impl__UpdateCaption_CMFCTasksPane__IEAAXXZ(pThis);
}

// STUB.  CMFCTasksPane::SetScrollSizes() -- retail entry RVA 0x146670
// (mfc140) / 0x148000 (mfc140u, where AdjustScroll at 0x148200 calls it):
//   if (m_wndScrollVert.m_hWnd == NULL) return;           // +0x680, CWnd +0x40
//   if (m_nRowHeight /*+0x51c*/ == 0) {
//       m_nVertScrollTotal = m_nVertScrollPage = 0;       // +0x514 / +0x518
//       nPos = 0;
//   } else {
//       int h = m_rectTasks.bottom - m_rectTasks.top;     // +0x5e4 - +0x5dc
//       if (m_bUseScrollButtons /*+0x4ec*/)
//           h -= m_iScrollBtnHeight /*+0x50c*/ + 1;
//       if (m_bUseNavigationToolbar /*+0x4e4*/ || ForceShowNavToolbar())
//           // inlined: GetParentTabbedPane() != NULL (0xc9b0), or
//           // GetParentMiniFrame(TRUE) (vtable +0x460) is non-NULL and is NOT
//           // a CMFCTasksPaneFrameWnd (CObject::IsKindOf 0x233310 against the
//           // descriptor whose name string is "CMFCTasksPaneFrameWnd")
//           if (m_lstTasksPanes.m_nCount /*+0x620*/ > 1)
//               h += m_rectToolbar.bottom - m_rectToolbar.top;   // +0x5b4 - +0x5ac
//       m_nVertScrollPage = h / m_nRowHeight - 1;
//       int nRepos = ReposTasks(TRUE);                    // vtable +0x7d0
//       if (nRepos != 0 && nRepos > h) m_nVertScrollTotal = nRepos / m_nRowHeight - 1;
//       else { m_nVertScrollPage = m_nVertScrollOffset = 0;   // +0x518 / +0x510
//              m_nVertScrollTotal = 0; }
//   }
//   m_nVertScrollOffset is then set -- 0 on the m_nRowHeight == 0 path,
//   otherwise clamped into [0, total - page + 1] -- and when
//   m_bUseScrollButtons is FALSE a SCROLLINFO {nMin = 0, nMax = total,
//   nPage = page, nPos = offset, fMask = SIF_RANGE|SIF_PAGE|SIF_POS} goes to
//   SetScrollInfo(SB_VERT, &si, TRUE) (CWnd::SetScrollInfo, 0x28cd10); finally
//   ::EnableScrollBar(m_wndScrollVert.m_hWnd /*+0x6c0*/, SB_CTL,
//                     (m_bUseScrollButtons || m_nVertScrollTotal <= 0)
//                         ? ESB_DISABLE_BOTH : ESB_ENABLE_BOTH).
// Blocked: the scroll-bar sub-window at +0x680 is constructed by the retail
// constructor and created by retail OnCreate; OpenMFC's ctor thunk does not
// construct it and OnCreate above is a stub, so the early-return guard has no
// real window to test.  The one substantive input, the content height from
// ReposTasks(TRUE), comes from a stub that returns 0 (above), so a
// transcription would always take the `nRepos == 0` branch and zero the
// scroll state -- no information a caller could use.  Signature corrected to
// carry `this`.
// Symbol: ?SetScrollSizes@CMFCTasksPane@@IEAAXXZ
extern "C" void MS_ABI impl__SetScrollSizes_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis) {
    (void)pThis;
}

// CMFCTasksPane::SetTaskName(int nGroup, int nTask, LPCTSTR lpszTaskName) --
// transcribed from the retail body at RVA 0x144d30 (mfc140, PEBD spelling of
// the same bytes mfc140u exports as PEB_W):
//   if (nGroup >= m_lstTaskGroups.m_nCount || nGroup < 0) return FALSE;
//   gnode = walk m_lstTaskGroups nGroup;  if (gnode == NULL) return FALSE;
//   pGroup = gnode->data;
//   if (nTask >= pGroup->m_lstTasks.m_nCount || nTask < 0) return FALSE;
//   tnode = walk pGroup->m_lstTasks nTask; if (tnode == NULL) return FALSE;
//   pTask = tnode->data;
//   pTask->m_strName.SetString(lpszTaskName,
//                              lpszTaskName ? strlen(lpszTaskName) : 0);  // +0x10
//   if (pTask->m_bVisible /*+0x4c*/)
//       ::InvalidateRect(m_hWnd, &pTask->m_rect /*+0x18*/, TRUE);
//   return TRUE;
// (both bounds tests + walks are the FindIndex thunk; see the storage note at
// the top.)  Placeholder parameter list replaced by the one the mangled name
// describes.
// Symbol: ?SetTaskName@CMFCTasksPane@@QEAAHHHPEB_W@Z
extern "C" int MS_ABI impl__SetTaskName_CMFCTasksPane__QEAAHHHPEB_W_Z(
    CMFCTasksPane* pThis, int nGroup, int nTask, const wchar_t* lpszTaskName)
{
    if (pThis == nullptr) return FALSE;
    TpNode* gnode = NodeAt(GroupList(pThis), nGroup);
    if (gnode == nullptr) return FALSE;
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(gnode->data);
    TpNode* tnode = NodeAt(TaskList(pGroup), nTask);
    if (tnode == nullptr) return FALSE;
    TPTask* pTask = reinterpret_cast<TPTask*>(tnode->data);
    *reinterpret_cast<CString*>(&pTask->m_strName) = (lpszTaskName != nullptr ? lpszTaskName : L"");
    if (pTask->m_bVisible != 0) {
        ::InvalidateRect(pThis->m_hWnd, &pTask->m_rect, TRUE);
    }
    return TRUE;
}

// CMFCTasksPane::SetTaskTextColor(int nGroup, int nTask, COLORREF color,
// COLORREF colorHot) -- transcribed from the retail body at RVA 0x144e00
// (mfc140).  The group/task lookup is the same four bounds-checked walks as
// SetTaskName above, then:
//   pTask->m_clrText    (+0x5c) = color;
//   pTask->m_clrTextHot (+0x60) = colorHot;
//   if (pTask->m_bVisible /*+0x4c*/)
//       ::InvalidateRect(m_hWnd, &pTask->m_rect /*+0x18*/, TRUE);
//   return TRUE;                 // any failed lookup returns FALSE
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?SetTaskTextColor@CMFCTasksPane@@QEAAHHHKK@Z
extern "C" int MS_ABI impl__SetTaskTextColor_CMFCTasksPane__QEAAHHHKK_Z(
    CMFCTasksPane* pThis, int nGroup, int nTask, unsigned long color, unsigned long colorHot)
{
    if (pThis == nullptr) return FALSE;
    TpNode* gnode = NodeAt(GroupList(pThis), nGroup);
    if (gnode == nullptr) return FALSE;
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(gnode->data);
    TpNode* tnode = NodeAt(TaskList(pGroup), nTask);
    if (tnode == nullptr) return FALSE;
    TPTask* pTask = reinterpret_cast<TPTask*>(tnode->data);
    pTask->m_clrText    = static_cast<COLORREF>(color);
    pTask->m_clrTextHot = static_cast<COLORREF>(colorHot);
    if (pTask->m_bVisible != 0) {
        ::InvalidateRect(pThis->m_hWnd, &pTask->m_rect, TRUE);
    }
    return TRUE;
}

// CMFCTasksPane::SetWindowHeight(int nGroup, HWND hwndTask, int nWndHeight)
// -- transcribed from the retail body at RVA 0x145390 (mfc140):
//   if (nGroup >= m_lstTaskGroups.m_nCount || nGroup < 0) return -1;
//   gnode = walk m_lstTaskGroups nGroup;  if (gnode == NULL) return -1;
//   if (!::IsWindow(hwndTask))                       AfxThrowInvalidArgException();
//   pGroup = gnode->data;
//   for (t = pGroup->m_lstTasks.head; t; t = t->pNext)
//       if (t->data->m_hwndTask /*+0x40*/ == hwndTask) goto found;
//   return;                    // EAX still holds ::IsWindow's non-zero result
//   found:
//       t->data->m_nWindowHeight (+0x2c) = nWndHeight;
//       if (pGroup->m_bIsCollapsed /*+0x58*/ == 0) {
//           AdjustScroll();                                          // 0x146870
//           ReposTasks(FALSE);                                       // vtable +0x7d0
//           ::RedrawWindow(m_hWnd, NULL, NULL,
//                          RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
//       }
//       return TRUE;
// Note the three distinct results: -1 for a bad group index, TRUE when the
// window was found, and -- on the not-found path -- whatever ::IsWindow
// returned, which the code above has already proved non-zero; that is written
// as TRUE here.  Deviation: the refresh goes through this file's RetailRefresh
// helper, so a derived ReposTasks override is not honoured.  Placeholder
// parameter list replaced by the one the mangled name describes.
// Symbol: ?SetWindowHeight@CMFCTasksPane@@QEAAHHPEAUHWND__@@H@Z
extern "C" int MS_ABI impl__SetWindowHeight_CMFCTasksPane__QEAAHHPEAUHWND____H_Z(
    CMFCTasksPane* pThis, int nGroup, HWND hwndTask, int nWndHeight)
{
    if (pThis == nullptr) return -1;
    TpNode* gnode = NodeAt(GroupList(pThis), nGroup);
    if (gnode == nullptr) return -1;
    if (!::IsWindow(hwndTask)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;
    }
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(gnode->data);
    for (TpNode* t = HeadNode(TaskList(pGroup)); t != nullptr; t = t->pNext) {
        TPTask* pTask = reinterpret_cast<TPTask*>(t->data);
        if (pTask->m_hwndTask != hwndTask) continue;
        pTask->m_nWindowHeight = nWndHeight;
        if (pGroup->m_bIsCollapsed == 0) {
            RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        }
        return TRUE;
    }
    return TRUE;
}

// CMFCTasksPane::SetWindowHeight(HWND hwndTask, int nWndHeight) --
// transcribed from the retail body at RVA 0x145450 (mfc140):
//   if (!::IsWindow(hwndTask))                       AfxThrowInvalidArgException();
//   int nGroup, nTask;
//   if (!GetTaskLocation(hwndTask, nGroup, nTask)) return FALSE;     // 0x1451a0
//   return SetWindowHeight(nGroup, hwndTask, nWndHeight);            // 0x145390
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?SetWindowHeight@CMFCTasksPane@@QEAAHPEAUHWND__@@H@Z
extern "C" int MS_ABI impl__SetWindowHeight_CMFCTasksPane__QEAAHPEAUHWND____H_Z(
    CMFCTasksPane* pThis, HWND hwndTask, int nWndHeight)
{
    if (pThis == nullptr) return FALSE;
    if (!::IsWindow(hwndTask)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    int nGroup = 0, nTask = 0;
    if (!impl__GetTaskLocation_CMFCTasksPane__QEBAHPEAUHWND____AEAH1_Z(pThis, hwndTask, &nGroup, &nTask)) {
        return FALSE;
    }
    return impl__SetWindowHeight_CMFCTasksPane__QEAAHHPEAUHWND____H_Z(pThis, nGroup, hwndTask, nWndHeight);
}

// CMFCTasksPane::ShowCommandMessageString(UINT uiCmdId) -- transcribed from
// the retail body at RVA 0x148f40 (mfc140):
//   HWND hOwner = m_hWndOwner;                       // CWnd +0xa0
//   if (hOwner == NULL) hOwner = ::GetParent(m_hWnd);
//   UINT id = (uiCmdId == (UINT)-1) ? AFX_IDS_IDLEMESSAGE /*0xE001*/ : uiCmdId;
//   CWnd::FromHandle(hOwner)->SendMessage(WM_SETMESSAGESTRING /*0x362*/, id, 0);
//                                                    // 0x289180, then tail jmp
// OpenMFC's CWnd does not name m_hWndOwner: +0xa0 lies inside its
// _cwnd_padding2 block, which the CWnd constructor zero-fills.  MSVC clients
// inline CWnd::SetOwner (`m_hWndOwner = ...`) and so DO write that slot, so it
// is read here at the retail offset with the ::GetParent fallback -- the same
// treatment core/controlbar/CControlBar.cpp's OwnerHwnd gives it.  The
// CWnd::FromHandle round trip is elided because the wrapper it returns carries
// exactly that handle in m_hWnd, so the SendMessage target is unchanged --
// except when the owner handle is NULL, where retail dereferences FromHandle's
// NULL result and faults and this sends to a NULL window instead.
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?ShowCommandMessageString@CMFCTasksPane@@UEAAXI@Z
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCTasksPane__UEAAXI_Z(
    CMFCTasksPane* pThis, unsigned int uiCmdId)
{
    if (pThis == nullptr) return;
    const CWnd* pWnd = pThis;
    HWND hOwner = nullptr;
    std::memcpy(&hOwner, reinterpret_cast<const unsigned char*>(pWnd) + kCWndOwnerOffset, sizeof(hOwner));
    if (hOwner == nullptr) hOwner = ::GetParent(pThis->m_hWnd);
    const unsigned int id = (uiCmdId == static_cast<unsigned int>(-1)) ? 0xE001u : uiCmdId;
    ::SendMessage(hOwner, 0x362u /* WM_SETMESSAGESTRING */, static_cast<WPARAM>(id), 0);
}

// CMFCTasksPane::ShowTask(int nGroup, int nTask, BOOL bShow, BOOL bRedraw) --
// transcribed from the retail body at RVA 0x144e90 (mfc140).  The group/task
// lookup is the same four bounds-checked walks as SetTaskName above (any
// failure returns FALSE), then:
//   old = pTask->m_bVisible;                                        // +0x4c
//   if (bShow) { if (old) return TRUE; } else { if (!old) return TRUE; }
//   pTask->m_bVisible = bShow;
//   AdjustScroll();                                                 // 0x146870
//   ReposTasks(FALSE);                                              // vtable +0x7d0
//   if (bRedraw) ::RedrawWindow(m_hWnd, NULL, NULL,
//                               RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
//   return TRUE;
// i.e. nothing at all happens when the task is already in the requested
// state, and bRedraw gates only the RedrawWindow -- the scroll/reposition
// work runs either way.  Deviation: ReposTasks is called as this class's own
// export, so a derived override is not honoured; it is still a stub here.
// Placeholder parameter list replaced by the one the mangled name describes.
// Symbol: ?ShowTask@CMFCTasksPane@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__ShowTask_CMFCTasksPane__QEAAHHHHH_Z(
    CMFCTasksPane* pThis, int nGroup, int nTask, int bShow, int bRedraw)
{
    if (pThis == nullptr) return FALSE;
    TpNode* gnode = NodeAt(GroupList(pThis), nGroup);
    if (gnode == nullptr) return FALSE;
    TPGroup* pGroup = reinterpret_cast<TPGroup*>(gnode->data);
    TpNode* tnode = NodeAt(TaskList(pGroup), nTask);
    if (tnode == nullptr) return FALSE;
    TPTask* pTask = reinterpret_cast<TPTask*>(tnode->data);

    if (bShow != 0) {
        if (pTask->m_bVisible != 0) return TRUE;
    } else {
        if (pTask->m_bVisible == 0) return TRUE;
    }
    pTask->m_bVisible = bShow;
    impl__AdjustScroll_CMFCTasksPane__IEAAXXZ(pThis);
    impl__ReposTasks_CMFCTasksPane__MEAAHH_Z(pThis, FALSE);
    if (bRedraw != 0) {
        ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    return TRUE;
}

// CMFCTasksPane::ShowTaskByCmdId(UINT uiCommandID, BOOL bShow, BOOL bRedraw)
// -- transcribed from the retail body at RVA 0x144f60 (mfc140):
//   int nGroup, nTask;
//   if (!GetTaskLocation(uiCommandID, nGroup, nTask)) return FALSE;  // 0x145130
//   return ShowTask(nGroup, nTask, bShow, bRedraw);                  // 0x144e90
// (retail leaves GetTaskLocation's zero in EAX on the failure path, so the
// function returns FALSE there.)  Placeholder parameter list replaced by the
// one the mangled name describes.
// Symbol: ?ShowTaskByCmdId@CMFCTasksPane@@QEAAHIHH@Z
extern "C" int MS_ABI impl__ShowTaskByCmdId_CMFCTasksPane__QEAAHIHH_Z(
    CMFCTasksPane* pThis, unsigned int uiCommandID, int bShow, int bRedraw)
{
    if (pThis == nullptr) return FALSE;
    int nGroup = 0, nTask = 0;
    if (!impl__GetTaskLocation_CMFCTasksPane__QEBAHIAEAH0_Z(pThis, uiCommandID, &nGroup, &nTask)) {
        return FALSE;
    }
    return impl__ShowTask_CMFCTasksPane__QEAAHHHHH_Z(pThis, nGroup, nTask, bShow, bRedraw);
}

// CMFCTasksPane::StopCaptionButtonsTracking() -- transcribed from the retail
// body at RVA 0x147f60 (mfc140), which is the whole function:
//   if (m_bMenuBtnPressed /*+0x4f8*/ == 0)
//       CDockablePane::StopCaptionButtonsTracking();                 // 0x45030
// i.e. while the pane's own menu button is being held down the base class is
// deliberately NOT allowed to clear the caption-button tracking state.
// (featurepack/docking/CDockablePane.cpp's body is itself a stub -- that class
// has no m_arrButtons storage -- so the call currently does nothing, but the
// condition and the call site are retail's.)  Signature corrected to carry
// `this`.
// Symbol: ?StopCaptionButtonsTracking@CMFCTasksPane@@MEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CMFCTasksPane__MEAAXXZ(CMFCTasksPane* pThis) {
    if (pThis == nullptr) return;
    if (View(pThis)->m_bMenuBtnPressed == 0) {
        impl__StopCaptionButtonsTracking_CDockablePane__MEAAXXZ(reinterpret_cast<CDockablePane*>(pThis));
    }
}

// CMFCTasksPane::TaskHitTest(CPoint pt) const -- transcribed from the retail
// body at RVA 0x1456e0 (mfc140):
//   if (!::PtInRect(&m_rectTasks /*+0x5d8*/, pt)) return NULL;
//   nPage = m_arrHistoryStack[m_iActivePage];  ENSURE(bounds) else AfxThrowInvalidArgException
//   pPage = m_lstTasksPanes.FindIndex(nPage)->data;  ENSURE(bounds / non-NULL)
//   for (g = m_lstTaskGroups.head; g; g = g->pNext) {
//       if (g->data->m_pPage /*+0x8*/ != pPage) continue;
//       for (t = g->data->m_lstTasks.head; t; t = t->pNext)
//           if (t->data->m_bVisible /*+0x4c*/ &&
//               ::PtInRect(&t->data->m_rect /*+0x18*/, pt) &&
//               t->data->m_uiCommandID /*+0x30*/ != 0)
//               return t->data;
//   }
//   return NULL;
// Note the empty group list is short-circuited before the loop and also
// returns NULL, and that a task with command id 0 (a separator or a plain
// label) never wins the hit test even when the point is inside its rectangle.
// The by-value CPoint arrives packed in one register (x low, y high), exactly
// as in GroupCaptionHitTest above.  Placeholder parameter list replaced by the
// one the mangled name describes.
// Symbol: ?TaskHitTest@CMFCTasksPane@@IEBAPEAVCMFCTasksPaneTask@@VCPoint@@@Z
extern "C" void* MS_ABI impl__TaskHitTest_CMFCTasksPane__IEBAPEAVCMFCTasksPaneTask__VCPoint___Z(
    const CMFCTasksPane* pThis, long long packedPoint)
{
    if (pThis == nullptr) return nullptr;
    const TP* self = View(pThis);
    POINT pt;
    pt.x = static_cast<LONG>(packedPoint & 0xffffffffLL);
    pt.y = static_cast<LONG>((packedPoint >> 32) & 0xffffffffLL);
    if (!::PtInRect(&self->m_rectTasks, pt)) return nullptr;
    int nPage = 0;
    if (!HistoryAt(self, self->m_iActivePage, nPage)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    TpNode* pageNode = NodeAt(PageList(pThis), nPage);
    if (pageNode == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    const CObject* pPage = pageNode->data;
    for (TpNode* g = HeadNode(GroupList(pThis)); g != nullptr; g = g->pNext) {
        const TPGroup* pGroup = reinterpret_cast<const TPGroup*>(g->data);
        if (pGroup->m_pPage != pPage) continue;
        for (TpNode* t = HeadNode(TaskList(pGroup)); t != nullptr; t = t->pNext) {
            TPTask* pTask = reinterpret_cast<TPTask*>(t->data);
            if (pTask->m_bVisible == 0) continue;
            if (!::PtInRect(&pTask->m_rect, pt)) continue;
            if (pTask->m_uiCommandID == 0) continue;
            return pTask;
        }
    }
    return nullptr;
}

// CMFCTasksPane::Update() -- transcribed from the retail body at RVA 0x149f50
// (mfc140), which is the whole function:
//   UpdateCaption();                                                 // 0x149dd0
//   AdjustScroll();                                                  // 0x146870
//   ReposTasks(FALSE);                                               // vtable +0x7d0
//   ::RedrawWindow(m_hWnd, NULL, NULL,
//                  RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);          // tail jmp
// Deviation: the refresh tail goes through this file's RetailRefresh helper,
// so a derived ReposTasks override is not honoured; UpdateCaption, AdjustScroll
// and ReposTasks are all still stubs in this file, so today only the
// RedrawWindow has an observable effect.  Signature corrected to carry `this`.
// Symbol: ?Update@CMFCTasksPane@@UEAAXXZ
extern "C" void MS_ABI impl__Update_CMFCTasksPane__UEAAXXZ(CMFCTasksPane* pThis) {
    if (pThis == nullptr) return;
    impl__UpdateCaption_CMFCTasksPane__IEAAXXZ(pThis);
    RetailRefresh(pThis, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// Symbol: ?UpdateCaption@CMFCTasksPane@@IEAAXXZ
// Parameter list corrected to carry `this` (called from ChangeActivePage above); body still a stub.
extern "C" void MS_ABI impl__UpdateCaption_CMFCTasksPane__IEAAXXZ(CMFCTasksPane* pThis) { (void)pThis; }
