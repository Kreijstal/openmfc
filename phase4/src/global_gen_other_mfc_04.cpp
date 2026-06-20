#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// This shard implements the RTTI accessors (GetThisClass / GetRuntimeClass /
// CreateObject) and the message-map accessors for a set of control / view /
// frame classes.
//
// For classes that the repo headers declare faithfully (DECLARE_DYNAMIC /
// DECLARE_DYNCREATE + a matching IMPLEMENT_* in another translation unit) we
// forward to the real static accessor so the returned CRuntimeClass is the one
// MFC code already links against.
//
// For classes that have no concrete declaration in the repo headers we expose a
// local CRuntimeClass descriptor that mirrors MFC's layout. Object size is left
// 0 (factory disabled) because the true layout is unknown, and CreateObject
// returns nullptr for those.
// ---------------------------------------------------------------------------

namespace {
constexpr unsigned int kNoSerializationSchema = 0xFFFF;

CRuntimeClass g_runtimeClassCHeaderCtrl = {
    "CHeaderCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCHelpComboBoxButton = {
    "CHelpComboBoxButton", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCHotKeyCtrl = {
    "CHotKeyCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCHtmlEditDoc = {
    "CHtmlEditDoc", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCHtmlEditView = {
    "CHtmlEditView", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCHwndRenderTarget = {
    "CHwndRenderTarget", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCIPAddressCtrl = {
    "CIPAddressCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCInvalidArgException = {
    "CInvalidArgException", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCKeyFrame = {
    "CKeyFrame", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCLinkCtrl = {
    "CLinkCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCLongBinary = {
    "CLongBinary", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCMDITabProxyWnd = {
    "CMDITabProxyWnd", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCMiniDockFrameWnd = {
    "CMiniDockFrameWnd", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ===========================================================================
// CHeaderCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CHeaderCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHeaderCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCHeaderCtrl;
}

// Symbol: ?GetRuntimeClass@CHeaderCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHeaderCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCHeaderCtrl;
}

// ===========================================================================
// CHelpComboBoxButton  (CObject-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CHelpComboBoxButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHelpComboBoxButton__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCHelpComboBoxButton;
}

// Symbol: ?GetRuntimeClass@CHelpComboBoxButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHelpComboBoxButton__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCHelpComboBoxButton;
}

// Symbol: ?CreateObject@CHelpComboBoxButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHelpComboBoxButton__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CHotKeyCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CHotKeyCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHotKeyCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCHotKeyCtrl;
}

// Symbol: ?GetRuntimeClass@CHotKeyCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHotKeyCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCHotKeyCtrl;
}

// ===========================================================================
// CHtmlEditDoc  (CObject-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CHtmlEditDoc@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlEditDoc__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCHtmlEditDoc;
}

// Symbol: ?GetRuntimeClass@CHtmlEditDoc@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHtmlEditDoc__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCHtmlEditDoc;
}

// ===========================================================================
// CHtmlEditView  (CWnd/CView-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CHtmlEditView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlEditView__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCHtmlEditView;
}

// Symbol: ?GetRuntimeClass@CHtmlEditView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHtmlEditView__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCHtmlEditView;
}

// Symbol: ?CreateObject@CHtmlEditView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHtmlEditView__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CHtmlEditView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CHtmlEditView__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CHtmlEditView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CHtmlEditView__MEBAPEBUAFX_MSGMAP__XZ(
    const void*) {
    return CWnd::GetThisMessageMap();
}

// ===========================================================================
// CHtmlView  (CView-derived; declared with DECLARE_DYNCREATE in afxwin.h,
// IMPLEMENT_DYNCREATE lives in viewrich.cpp)
// ===========================================================================

// Symbol: ?GetThisClass@CHtmlView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHtmlView__SAPEAUCRuntimeClass__XZ() {
    return CHtmlView::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CHtmlView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHtmlView__UEBAPEAUCRuntimeClass__XZ(
    const CHtmlView* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CHtmlView::GetThisClass();
}

// Symbol: ?CreateObject@CHtmlView@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CHtmlView__SAPEAVCObject__XZ() {
    return CHtmlView::CreateObject();
}

// Symbol: ?GetThisMessageMap@CHtmlView@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CHtmlView__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CHtmlView@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CHtmlView__MEBAPEBUAFX_MSGMAP__XZ(
    const void*) {
    return CWnd::GetThisMessageMap();
}

// ===========================================================================
// CHwndRenderTarget  (no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CHwndRenderTarget@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHwndRenderTarget__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCHwndRenderTarget;
}

// Symbol: ?GetRuntimeClass@CHwndRenderTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHwndRenderTarget__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCHwndRenderTarget;
}

// ===========================================================================
// CIPAddressCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CIPAddressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CIPAddressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCIPAddressCtrl;
}

// Symbol: ?GetRuntimeClass@CIPAddressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CIPAddressCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCIPAddressCtrl;
}

// ===========================================================================
// CInvalidArgException  (CException-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCInvalidArgException;
}

// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCInvalidArgException;
}

// ===========================================================================
// CKeyFrame  (CObject-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CKeyFrame@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CKeyFrame__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCKeyFrame;
}

// Symbol: ?GetRuntimeClass@CKeyFrame@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CKeyFrame__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCKeyFrame;
}

// ===========================================================================
// CLinkCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CLinkCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLinkCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCLinkCtrl;
}

// Symbol: ?GetRuntimeClass@CLinkCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLinkCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCLinkCtrl;
}

// ===========================================================================
// CLongBinary  (CObject-derived; header has no DECLARE_DYNAMIC, so we provide a
// local descriptor with the correct class name rather than inheriting
// CObject's)
// ===========================================================================

// Symbol: ?GetThisClass@CLongBinary@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLongBinary__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCLongBinary;
}

// Symbol: ?GetRuntimeClass@CLongBinary@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLongBinary__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCLongBinary;
}

// ===========================================================================
// CMDITabProxyWnd  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CMDITabProxyWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMDITabProxyWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCMDITabProxyWnd;
}

// Symbol: ?GetRuntimeClass@CMDITabProxyWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMDITabProxyWnd__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCMDITabProxyWnd;
}

// Symbol: ?CreateObject@CMDITabProxyWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMDITabProxyWnd__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CMDITabProxyWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMDITabProxyWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CMDITabProxyWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMDITabProxyWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const void*) {
    return CWnd::GetThisMessageMap();
}

// ===========================================================================
// CMemoryException  (CException-derived; declared with DECLARE_DYNAMIC in
// afxwin.h, IMPLEMENT_DYNAMIC lives in appcore.cpp)
// ===========================================================================

// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(
    const CMemoryException* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CMemoryException::GetThisClass();
}

// ===========================================================================
// CMetaFileDC  (CDC-derived; declared with DECLARE_DYNAMIC in afxwin.h,
// IMPLEMENT_DYNAMIC lives in gdicore.cpp)
// ===========================================================================

// Symbol: ?GetThisClass@CMetaFileDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMetaFileDC__SAPEAUCRuntimeClass__XZ() {
    return CMetaFileDC::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMetaFileDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMetaFileDC__UEBAPEAUCRuntimeClass__XZ(
    const CMetaFileDC* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CMetaFileDC::GetThisClass();
}

// ===========================================================================
// CMiniDockFrameWnd  (CFrameWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CMiniDockFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCMiniDockFrameWnd;
}

// Symbol: ?GetRuntimeClass@CMiniDockFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMiniDockFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCMiniDockFrameWnd;
}

// Symbol: ?CreateObject@CMiniDockFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMiniDockFrameWnd__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CMiniDockFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniDockFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CMiniDockFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMiniDockFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const void*) {
    return CWnd::GetThisMessageMap();
}

// ===========================================================================
// CMiniFrameWnd  (CFrameWnd-derived; declared with DECLARE_DYNAMIC in
// afxmfc.h, IMPLEMENT_DYNAMIC lives in mfccore.cpp)
// ===========================================================================

// Symbol: ?GetThisClass@CMiniFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMiniFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return CMiniFrameWnd::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CMiniFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMiniFrameWnd__UEBAPEAUCRuntimeClass__XZ(
    const CMiniFrameWnd* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CMiniFrameWnd::GetThisClass();
}

// Symbol: ?CreateObject@CMiniFrameWnd@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMiniFrameWnd__SAPEAVCObject__XZ() {
    // Header declares CMiniFrameWnd via DECLARE_DYNAMIC (no factory); the real
    // CRuntimeClass has no CreateObject either, so we keep the factory disabled.
    return nullptr;
}

// Symbol: ?GetThisMessageMap@CMiniFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CMiniFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@CMiniFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CMiniFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(
    const void*) {
    return CWnd::GetThisMessageMap();
}
