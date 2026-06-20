#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxwin.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
constexpr unsigned int kNoSerializationSchema = 0xFFFF;

// Local CRuntimeClass descriptors for classes that have no concrete
// declaration in the repo headers. These mirror the layout MFC uses so the
// RTTI accessors return a stable, valid pointer rather than the weak-stub
// default. Object size is left 0 (factory disabled) because the true layout
// is unknown.
CRuntimeClass g_rcCSharedFile = {
    "CSharedFile", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCSimpleException = {
    "CSimpleException", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCSmartDockingGroupGuidesManager = {
    "CSmartDockingGroupGuidesManager", 0, kNoSerializationSchema, nullptr,
    nullptr, &CObject::classCObject, nullptr};
CRuntimeClass g_rcCSmartDockingStandaloneGuide = {
    "CSmartDockingStandaloneGuide", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCSplitButton = {
    "CSplitButton", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCSplitterWnd = {
    "CSplitterWnd", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCSplitterWndEx = {
    "CSplitterWndEx", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCStatusBar = {
    "CStatusBar", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCStatusBarCtrl = {
    "CStatusBarCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCTabView = {
    "CTabView", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCTasksPaneHistoryButton = {
    "CTasksPaneHistoryButton", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCTasksPaneMenuButton = {
    "CTasksPaneMenuButton", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCTasksPaneNavigateButton = {
    "CTasksPaneNavigateButton", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ---------------------------------------------------------------------------
// CSharedFile
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSharedFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSharedFile__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSharedFile;
}
// Symbol: ?GetRuntimeClass@CSharedFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSharedFile__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSharedFile;
}

// ---------------------------------------------------------------------------
// CSimpleException
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSimpleException;
}
// Symbol: ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSimpleException__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSimpleException;
}

// ---------------------------------------------------------------------------
// CSmartDockingGroupGuidesManager
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSmartDockingGroupGuidesManager@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSmartDockingGroupGuidesManager__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSmartDockingGroupGuidesManager;
}
// Symbol: ?GetRuntimeClass@CSmartDockingGroupGuidesManager@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSmartDockingGroupGuidesManager__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSmartDockingGroupGuidesManager;
}
// Symbol: ?CreateObject@CSmartDockingGroupGuidesManager@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CSmartDockingGroupGuidesManager__SAPEAVCObject__XZ() {
    // No concrete layout available in this phase; factory disabled.
    return nullptr;
}

// ---------------------------------------------------------------------------
// CSmartDockingGroupGuidesWnd
// ---------------------------------------------------------------------------
// Symbol: ?GetThisMessageMap@CSmartDockingGroupGuidesWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSmartDockingGroupGuidesWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CSmartDockingGroupGuidesWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSmartDockingGroupGuidesWnd__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CSmartDockingHighlighterWnd
// ---------------------------------------------------------------------------
// Symbol: ?GetThisMessageMap@CSmartDockingHighlighterWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSmartDockingHighlighterWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CSmartDockingHighlighterWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSmartDockingHighlighterWnd__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CSmartDockingStandaloneGuide
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSmartDockingStandaloneGuide@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSmartDockingStandaloneGuide__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSmartDockingStandaloneGuide;
}
// Symbol: ?GetRuntimeClass@CSmartDockingStandaloneGuide@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSmartDockingStandaloneGuide__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSmartDockingStandaloneGuide;
}
// Symbol: ?CreateObject@CSmartDockingStandaloneGuide@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CSmartDockingStandaloneGuide__SAPEAVCObject__XZ() {
    // No concrete layout available in this phase; factory disabled.
    return nullptr;
}

// ---------------------------------------------------------------------------
// CSmartDockingStandaloneGuideWnd
// ---------------------------------------------------------------------------
// Symbol: ?GetThisMessageMap@CSmartDockingStandaloneGuideWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSmartDockingStandaloneGuideWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CSmartDockingStandaloneGuideWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSmartDockingStandaloneGuideWnd__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CSplitButton
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSplitButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitButton__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSplitButton;
}
// Symbol: ?GetRuntimeClass@CSplitButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSplitButton;
}
// Symbol: ?GetThisMessageMap@CSplitButton@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSplitButton__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CSplitButton@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSplitButton__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CSplitterWnd
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSplitterWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSplitterWnd;
}
// Symbol: ?GetRuntimeClass@CSplitterWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitterWnd__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSplitterWnd;
}
// Symbol: ?GetThisMessageMap@CSplitterWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSplitterWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CSplitterWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSplitterWnd__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CSplitterWndEx
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CSplitterWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitterWndEx__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCSplitterWndEx;
}
// Symbol: ?GetRuntimeClass@CSplitterWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitterWndEx__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCSplitterWndEx;
}
// Symbol: ?GetThisMessageMap@CSplitterWndEx@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CSplitterWndEx__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CSplitterWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CSplitterWndEx__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CStatusBar
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CStatusBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatusBar__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCStatusBar;
}
// Symbol: ?GetRuntimeClass@CStatusBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStatusBar__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCStatusBar;
}
// Symbol: ?GetThisMessageMap@CStatusBar@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CStatusBar__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CStatusBar@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CStatusBar__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CStatusBarCtrl
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CStatusBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatusBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCStatusBarCtrl;
}
// Symbol: ?GetRuntimeClass@CStatusBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStatusBarCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCStatusBarCtrl;
}

// ---------------------------------------------------------------------------
// CTabView
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CTabView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTabView__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCTabView;
}
// Symbol: ?GetRuntimeClass@CTabView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTabView__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCTabView;
}
// Symbol: ?CreateObject@CTabView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTabView__SAPEAVCObject__XZ() {
    // No concrete layout available in this phase; factory disabled.
    return nullptr;
}
// Symbol: ?GetThisMessageMap@CTabView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CTabView__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CTabView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CTabView__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CTasksPaneHistoryButton
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CTasksPaneHistoryButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCTasksPaneHistoryButton;
}
// Symbol: ?GetRuntimeClass@CTasksPaneHistoryButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTasksPaneHistoryButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCTasksPaneHistoryButton;
}
// Symbol: ?CreateObject@CTasksPaneHistoryButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneHistoryButton__SAPEAVCObject__XZ() {
    // No concrete layout available in this phase; factory disabled.
    return nullptr;
}

// ---------------------------------------------------------------------------
// CTasksPaneMenuButton
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CTasksPaneMenuButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCTasksPaneMenuButton;
}
// Symbol: ?GetRuntimeClass@CTasksPaneMenuButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTasksPaneMenuButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCTasksPaneMenuButton;
}
// Symbol: ?CreateObject@CTasksPaneMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneMenuButton__SAPEAVCObject__XZ() {
    // No concrete layout available in this phase; factory disabled.
    return nullptr;
}

// ---------------------------------------------------------------------------
// CTasksPaneNavigateButton
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CTasksPaneNavigateButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCTasksPaneNavigateButton;
}
// Symbol: ?GetRuntimeClass@CTasksPaneNavigateButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTasksPaneNavigateButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCTasksPaneNavigateButton;
}
// Symbol: ?CreateObject@CTasksPaneNavigateButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CTasksPaneNavigateButton__SAPEAVCObject__XZ() {
    // No concrete layout available in this phase; factory disabled.
    return nullptr;
}
