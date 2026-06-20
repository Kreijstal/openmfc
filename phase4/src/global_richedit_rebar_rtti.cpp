#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// This shard implements the MFC RTTI accessors (GetThisClass /
// GetRuntimeClass / CreateObject) for a set of classes whose true memory
// layout is not faithfully modelled by the repo headers.
//
// Each class gets a local CRuntimeClass descriptor that mirrors MFC's layout.
// The class name and base-class link (CObject) are correct, which is what
// callers of GetRuntimeClass()/IsKindOf typically rely on. Object size is
// left 0 (factory disabled) because the true layout is unknown, so
// CreateObject returns nullptr for those classes.
//
// Only RTTI accessors are implemented here; behaviour-bearing members of
// these classes are left as weak stubs because their internal state / memory
// layout cannot be determined safely.
// ---------------------------------------------------------------------------

namespace {
constexpr unsigned int kNoSerializationSchema = 0xFFFF;

CRuntimeClass g_runtimeClassCPtrList = {
    "CPtrList", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCReBar = {
    "CReBar", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCReBarCtrl = {
    "CReBarCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCResourceException = {
    "CResourceException", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCRibbonUndoLabel = {
    "CRibbonUndoLabel", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCRichEditCntrItem = {
    "CRichEditCntrItem", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCRichEditCtrl = {
    "CRichEditCtrl", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCRichEditDoc = {
    "CRichEditDoc", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_runtimeClassCSettingsStore = {
    "CSettingsStore", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ===========================================================================
// CPtrList
// ===========================================================================

// Symbol: ?GetThisClass@CPtrList@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPtrList__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCPtrList;
}

// Symbol: ?GetRuntimeClass@CPtrList@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPtrList__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCPtrList;
}

// ===========================================================================
// CReBar
// ===========================================================================

// Symbol: ?GetThisClass@CReBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBar__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCReBar;
}

// Symbol: ?GetRuntimeClass@CReBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CReBar__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCReBar;
}

// ===========================================================================
// CReBarCtrl
// ===========================================================================

// Symbol: ?GetThisClass@CReBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCReBarCtrl;
}

// Symbol: ?GetRuntimeClass@CReBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CReBarCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCReBarCtrl;
}

// ===========================================================================
// CResourceException
// ===========================================================================

// Symbol: ?GetThisClass@CResourceException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCResourceException;
}

// Symbol: ?GetRuntimeClass@CResourceException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCResourceException;
}

// ===========================================================================
// CRibbonUndoLabel
// ===========================================================================

// Symbol: ?GetThisClass@CRibbonUndoLabel@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRibbonUndoLabel__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCRibbonUndoLabel;
}

// Symbol: ?GetRuntimeClass@CRibbonUndoLabel@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRibbonUndoLabel__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCRibbonUndoLabel;
}

// Symbol: ?CreateObject@CRibbonUndoLabel@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRibbonUndoLabel__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CRichEditCntrItem
// ===========================================================================

// Symbol: ?GetThisClass@CRichEditCntrItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditCntrItem__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCRichEditCntrItem;
}

// Symbol: ?GetRuntimeClass@CRichEditCntrItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRichEditCntrItem__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCRichEditCntrItem;
}

// Symbol: ?CreateObject@CRichEditCntrItem@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CRichEditCntrItem__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}

// ===========================================================================
// CRichEditCtrl
// ===========================================================================

// Symbol: ?GetThisClass@CRichEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCRichEditCtrl;
}

// Symbol: ?GetRuntimeClass@CRichEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRichEditCtrl__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCRichEditCtrl;
}

// ===========================================================================
// CRichEditDoc
// ===========================================================================

// Symbol: ?GetThisClass@CRichEditDoc@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditDoc__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCRichEditDoc;
}

// Symbol: ?GetRuntimeClass@CRichEditDoc@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRichEditDoc__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCRichEditDoc;
}

// ===========================================================================
// CSettingsStore
// ===========================================================================

// Symbol: ?GetThisClass@CSettingsStore@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSettingsStore__SAPEAUCRuntimeClass__XZ() {
    return &g_runtimeClassCSettingsStore;
}

// Symbol: ?GetRuntimeClass@CSettingsStore@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSettingsStore__UEBAPEAUCRuntimeClass__XZ(
    const void*) {
    return &g_runtimeClassCSettingsStore;
}

// Symbol: ?CreateObject@CSettingsStore@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CSettingsStore__SAPEAVCObject__XZ() {
    // No concrete layout available; factory disabled.
    return nullptr;
}
