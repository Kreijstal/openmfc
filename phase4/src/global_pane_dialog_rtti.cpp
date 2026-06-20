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
// CreateObject) for a set of classes from the other_mfc 05 shard.
//
// For classes that the repo headers declare faithfully (DECLARE_DYNAMIC +
// IMPLEMENT_* in another translation unit) we forward to the real static
// accessor so the returned CRuntimeClass is the one MFC code already links
// against.
//
// For classes that have no concrete declaration in the repo headers we expose a
// local CRuntimeClass descriptor that mirrors MFC's layout. Object size is left
// 0 (factory disabled) because the true layout is unknown, and CreateObject
// returns nullptr for those.
// ---------------------------------------------------------------------------

namespace {
constexpr unsigned int kNoSerializationSchema = 0xFFFF;

CRuntimeClass g_runtimeClassCMonikerFile = {
    "CMonikerFile", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCMonthCalCtrl = {
    "CMonthCalCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCMouseManager = {
    "CMouseManager", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCMultiPageDHtmlDialog = {
    "CMultiPageDHtmlDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCNetAddressCtrl = {
    "CNetAddressCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCNotSupportedException = {
    "CNotSupportedException", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCOutlookCustomizeButton = {
    "COutlookCustomizeButton", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCPagerCtrl = {
    "CPagerCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCPaneDialog = {
    "CPaneDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCPreviewViewEx = {
    "CPreviewViewEx", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ===========================================================================
// CMonikerFile  (CFile-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CMonikerFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMonikerFile__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCMonikerFile;
}

// Symbol: ?GetRuntimeClass@CMonikerFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMonikerFile__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCMonikerFile;
}

// ===========================================================================
// CMonthCalCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CMonthCalCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMonthCalCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCMonthCalCtrl;
}

// Symbol: ?GetRuntimeClass@CMonthCalCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMonthCalCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCMonthCalCtrl;
}

// ===========================================================================
// CMouseManager  (CObject-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CMouseManager@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMouseManager__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCMouseManager;
}

// Symbol: ?GetRuntimeClass@CMouseManager@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMouseManager__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCMouseManager;
}

// Symbol: ?CreateObject@CMouseManager@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMouseManager__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CMultiPageDHtmlDialog  (CDHtmlDialog-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CMultiPageDHtmlDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPageDHtmlDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCMultiPageDHtmlDialog;
}

// Symbol: ?GetRuntimeClass@CMultiPageDHtmlDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMultiPageDHtmlDialog__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCMultiPageDHtmlDialog;
}

// ===========================================================================
// CNetAddressCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CNetAddressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNetAddressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCNetAddressCtrl;
}

// Symbol: ?GetRuntimeClass@CNetAddressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CNetAddressCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCNetAddressCtrl;
}

// ===========================================================================
// CNotSupportedException  (CException-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCNotSupportedException;
}

// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCNotSupportedException;
}

// ===========================================================================
// COutlookCustomizeButton  (CObject-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@COutlookCustomizeButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COutlookCustomizeButton__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCOutlookCustomizeButton;
}

// Symbol: ?GetRuntimeClass@COutlookCustomizeButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COutlookCustomizeButton__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCOutlookCustomizeButton;
}

// Symbol: ?CreateObject@COutlookCustomizeButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_COutlookCustomizeButton__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CPagerCtrl  (CWnd-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCPagerCtrl;
}

// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCPagerCtrl;
}

// ===========================================================================
// CPaneDialog  (CDialog-derived; no concrete declaration in headers)
// ===========================================================================

// Symbol: ?GetThisClass@CPaneDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaneDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCPaneDialog;
}

// Symbol: ?GetRuntimeClass@CPaneDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPaneDialog__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCPaneDialog;
}

// Symbol: ?CreateObject@CPaneDialog@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPaneDialog__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CPreviewViewEx  (CPreviewView/CScrollView-derived; no concrete declaration)
// ===========================================================================

// Symbol: ?GetThisClass@CPreviewViewEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPreviewViewEx__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCPreviewViewEx;
}

// Symbol: ?GetRuntimeClass@CPreviewViewEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPreviewViewEx__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCPreviewViewEx;
}

// Symbol: ?CreateObject@CPreviewViewEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPreviewViewEx__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CPrintDialogEx  (CDialog-derived; declared with DECLARE_DYNAMIC in afxwin.h,
// IMPLEMENT_DYNAMIC lives in dlgcommon.cpp)
// ===========================================================================

// Symbol: ?GetThisClass@CPrintDialogEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPrintDialogEx__SAPEAUCRuntimeClass__XZ() {
    return CPrintDialogEx::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CPrintDialogEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPrintDialogEx__UEBAPEAUCRuntimeClass__XZ(
    const CPrintDialogEx* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CPrintDialogEx::GetThisClass();
}
