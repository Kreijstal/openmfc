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
CRuntimeClass g_rcCAnimateCtrl = {
    "CAnimateCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAnimationBaseObject = {
    "CAnimationBaseObject", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAnimationColor = {
    "CAnimationColor", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAnimationPoint = {
    "CAnimationPoint", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAnimationRect = {
    "CAnimationRect", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAnimationSize = {
    "CAnimationSize", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAnimationValue = {
    "CAnimationValue", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAsyncMonikerFile = {
    "CAsyncMonikerFile", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCAutoHideDockSite = {
    "CAutoHideDockSite", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCBaseKeyFrame = {
    "CBaseKeyFrame", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCBaseTransition = {
    "CBaseTransition", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCCachedDataPathProperty = {
    "CCachedDataPathProperty", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCComboBoxEx = {
    "CComboBoxEx", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCCommonDialog = {
    "CCommonDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ---------------------------------------------------------------------------
// CAnimateCtrl
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimateCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimateCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimateCtrl;
}
// Symbol: ?GetRuntimeClass@CAnimateCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimateCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimateCtrl;
}

// ---------------------------------------------------------------------------
// CAnimationBaseObject
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimationBaseObject@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationBaseObject__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimationBaseObject;
}
// Symbol: ?GetRuntimeClass@CAnimationBaseObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationBaseObject__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimationBaseObject;
}

// ---------------------------------------------------------------------------
// CAnimationColor
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimationColor@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationColor__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimationColor;
}
// Symbol: ?GetRuntimeClass@CAnimationColor@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationColor__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimationColor;
}

// ---------------------------------------------------------------------------
// CAnimationPoint
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimationPoint@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationPoint__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimationPoint;
}
// Symbol: ?GetRuntimeClass@CAnimationPoint@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationPoint__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimationPoint;
}

// ---------------------------------------------------------------------------
// CAnimationRect
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimationRect@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationRect__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimationRect;
}
// Symbol: ?GetRuntimeClass@CAnimationRect@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationRect__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimationRect;
}

// ---------------------------------------------------------------------------
// CAnimationSize
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimationSize@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationSize__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimationSize;
}
// Symbol: ?GetRuntimeClass@CAnimationSize@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationSize__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimationSize;
}

// ---------------------------------------------------------------------------
// CAnimationValue
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAnimationValue@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimationValue__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAnimationValue;
}
// Symbol: ?GetRuntimeClass@CAnimationValue@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimationValue__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAnimationValue;
}

// ---------------------------------------------------------------------------
// CAsyncMonikerFile
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAsyncMonikerFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAsyncMonikerFile__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAsyncMonikerFile;
}
// Symbol: ?GetRuntimeClass@CAsyncMonikerFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAsyncMonikerFile__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAsyncMonikerFile;
}

// ---------------------------------------------------------------------------
// CAutoHideDockSite  (CWnd-derived; RTTI + message-map accessors)
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CAutoHideDockSite@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAutoHideDockSite__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCAutoHideDockSite;
}
// Symbol: ?GetRuntimeClass@CAutoHideDockSite@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAutoHideDockSite__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCAutoHideDockSite;
}
// Symbol: ?CreateObject@CAutoHideDockSite@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CAutoHideDockSite__SAPEAVCObject__XZ() {
    // No concrete CAutoHideDockSite layout available in this phase; factory disabled.
    return nullptr;
}
// Symbol: ?GetThisMessageMap@CAutoHideDockSite@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CAutoHideDockSite__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CAutoHideDockSite@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CAutoHideDockSite__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}

// ---------------------------------------------------------------------------
// CBaseKeyFrame
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CBaseKeyFrame@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseKeyFrame__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCBaseKeyFrame;
}
// Symbol: ?GetRuntimeClass@CBaseKeyFrame@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBaseKeyFrame__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCBaseKeyFrame;
}

// ---------------------------------------------------------------------------
// CBaseTransition
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CBaseTransition@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBaseTransition__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCBaseTransition;
}
// Symbol: ?GetRuntimeClass@CBaseTransition@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBaseTransition__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCBaseTransition;
}

// ---------------------------------------------------------------------------
// CCachedDataPathProperty
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CCachedDataPathProperty@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCachedDataPathProperty__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCCachedDataPathProperty;
}
// Symbol: ?GetRuntimeClass@CCachedDataPathProperty@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCachedDataPathProperty__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCCachedDataPathProperty;
}

// ---------------------------------------------------------------------------
// CComboBoxEx
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CComboBoxEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CComboBoxEx__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCComboBoxEx;
}
// Symbol: ?GetRuntimeClass@CComboBoxEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CComboBoxEx__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCComboBoxEx;
}

// ---------------------------------------------------------------------------
// CCommonDialog  (CDialog/CWnd-derived; RTTI + message-map accessors)
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CCommonDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCommonDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCCommonDialog;
}
// Symbol: ?GetRuntimeClass@CCommonDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCommonDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCCommonDialog;
}
// Symbol: ?GetThisMessageMap@CCommonDialog@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CCommonDialog__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}
// Symbol: ?GetMessageMap@CCommonDialog@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CCommonDialog__MEBAPEBUAFX_MSGMAP__XZ(const void* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
