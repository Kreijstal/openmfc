#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

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
CRuntimeClass g_runtimeClassCToolBarCtrl = {
    "CToolBarCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCUserException = {
    "CUserException", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCUserTool = {
    "CUserTool", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCWordArray = {
    "CWordArray", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCWindowlessDC = {
    "CWindowlessDC", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ---------------------------------------------------------------------------
// CToolBar  (declared in afxole.h via DECLARE_DYNAMIC, runtime class lives in
// cbarcore.cpp). RTTI + message-map accessors.
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CToolBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBar__SAPEAUCRuntimeClass__XZ() {
    return CToolBar::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CToolBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBar__UEBAPEAUCRuntimeClass__XZ(
    const CToolBar* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CToolBar::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CToolBar@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolBar__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CToolBar@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CToolBar__MEBAPEBUAFX_MSGMAP__XZ(
    const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CToolBarCtrl  (CWnd-derived; no concrete declaration in headers).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CToolBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CToolBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCToolBarCtrl;
}

// Symbol: ?GetRuntimeClass@CToolBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CToolBarCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCToolBarCtrl;
}

// Symbol: ?GetThisMessageMap@CToolBarCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CToolBarCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CToolBarCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CToolBarCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CUserException  (CObject-derived; no concrete declaration in headers).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCUserException;
}

// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCUserException;
}

// ---------------------------------------------------------------------------
// CUserTool  (CObject-derived; no concrete declaration in headers).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CUserTool@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CUserTool__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCUserTool;
}

// Symbol: ?GetRuntimeClass@CUserTool@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CUserTool__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCUserTool;
}

// Symbol: ?CreateObject@CUserTool@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CUserTool__SAPEAVCObject__XZ() {
    // No concrete CUserTool layout available in this phase; factory disabled.
    return nullptr;
}

// ---------------------------------------------------------------------------
// CWordArray  (CObject-derived; no concrete declaration in headers).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CWordArray@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWordArray__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCWordArray;
}

// Symbol: ?GetRuntimeClass@CWordArray@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWordArray__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCWordArray;
}

// Symbol: ?CreateObject@CWordArray@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CWordArray__SAPEAVCObject__XZ() {
    // No concrete CWordArray layout available in this phase; factory disabled.
    return nullptr;
}

// ---------------------------------------------------------------------------
// CWindowlessDC  (no concrete declaration in headers).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CWindowlessDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWindowlessDC__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCWindowlessDC;
}

// Symbol: ?GetRuntimeClass@CWindowlessDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWindowlessDC__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCWindowlessDC;
}

// ---------------------------------------------------------------------------
// CVSListBox  (declared in afxmfc.h via DECLARE_DYNAMIC, runtime class lives
// in dlgcommon.cpp). RTTI + message-map accessors.
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CVSListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBox__SAPEAUCRuntimeClass__XZ() {
    return CVSListBox::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CVSListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBox__UEBAPEAUCRuntimeClass__XZ(
    const CVSListBox* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBox::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CVSListBox@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBox__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CVSListBox@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CVSListBox__MEBAPEBUAFX_MSGMAP__XZ(
    const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CVSListBoxBase  (declared in afxmfc.h via DECLARE_DYNAMIC).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CVSListBoxBase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxBase__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxBase::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CVSListBoxBase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxBase__UEBAPEAUCRuntimeClass__XZ(
    const CVSListBoxBase* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBoxBase::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CVSListBoxBase@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBoxBase__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CVSListBoxBase@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CVSListBoxBase__MEBAPEBUAFX_MSGMAP__XZ(
    const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CVSListBoxEditCtrl  (declared in afxmfc.h via DECLARE_DYNAMIC).
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@CVSListBoxEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxEditCtrl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CVSListBoxEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxEditCtrl__UEBAPEAUCRuntimeClass__XZ(
    const CVSListBoxEditCtrl* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBoxEditCtrl::GetThisClass();
}

// Symbol: ?GetThisMessageMap@CVSListBoxEditCtrl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CVSListBoxEditCtrl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CVSListBoxEditCtrl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CVSListBoxEditCtrl__MEBAPEBUAFX_MSGMAP__XZ(
    const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
