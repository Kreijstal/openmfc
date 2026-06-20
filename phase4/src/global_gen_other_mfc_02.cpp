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
// is unknown; the base class is chained to CObject so IsKindOf walks succeed.
CRuntimeClass g_rcCControlBar = {
    "CControlBar", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCCtrlView = {
    "CCtrlView", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DGeometry = {
    "CD2DGeometry", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DLayer = {
    "CD2DLayer", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DMesh = {
    "CD2DMesh", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DPathGeometry = {
    "CD2DPathGeometry", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DResource = {
    "CD2DResource", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DTextFormat = {
    "CD2DTextFormat", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCD2DTextLayout = {
    "CD2DTextLayout", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCDHtmlDialog = {
    "CDHtmlDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
}  // namespace

// ---------------------------------------------------------------------------
// CControlBar
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CControlBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCControlBar;
}
// Symbol: ?GetRuntimeClass@CControlBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CControlBar__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCControlBar;
}

// ---------------------------------------------------------------------------
// CCtrlView
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CCtrlView@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CCtrlView__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCCtrlView;
}
// Symbol: ?GetRuntimeClass@CCtrlView@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCtrlView__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCCtrlView;
}

// ---------------------------------------------------------------------------
// CD2DGeometry
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DGeometry@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DGeometry__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DGeometry;
}
// Symbol: ?GetRuntimeClass@CD2DGeometry@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DGeometry__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DGeometry;
}

// ---------------------------------------------------------------------------
// CD2DLayer
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DLayer@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DLayer__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DLayer;
}
// Symbol: ?GetRuntimeClass@CD2DLayer@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DLayer__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DLayer;
}

// ---------------------------------------------------------------------------
// CD2DMesh
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DMesh@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DMesh__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DMesh;
}
// Symbol: ?GetRuntimeClass@CD2DMesh@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DMesh__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DMesh;
}

// ---------------------------------------------------------------------------
// CD2DPathGeometry
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DPathGeometry@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DPathGeometry__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DPathGeometry;
}
// Symbol: ?GetRuntimeClass@CD2DPathGeometry@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DPathGeometry__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DPathGeometry;
}

// ---------------------------------------------------------------------------
// CD2DResource
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DResource@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DResource;
}
// Symbol: ?GetRuntimeClass@CD2DResource@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DResource__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DResource;
}

// ---------------------------------------------------------------------------
// CD2DTextFormat
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DTextFormat@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DTextFormat;
}
// Symbol: ?GetRuntimeClass@CD2DTextFormat@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DTextFormat__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DTextFormat;
}

// ---------------------------------------------------------------------------
// CD2DTextLayout
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CD2DTextLayout@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CD2DTextLayout__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCD2DTextLayout;
}
// Symbol: ?GetRuntimeClass@CD2DTextLayout@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CD2DTextLayout__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCD2DTextLayout;
}

// ---------------------------------------------------------------------------
// CDHtmlDialog
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@CDHtmlDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDHtmlDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCDHtmlDialog;
}
// Symbol: ?GetRuntimeClass@CDHtmlDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDHtmlDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCDHtmlDialog;
}
