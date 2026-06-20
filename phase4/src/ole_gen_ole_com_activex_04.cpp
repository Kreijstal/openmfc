#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Forward-declared AFX map types: the repo headers only forward-declare these,
// so returning nullptr (the documented "no map" sentinel) is the safe choice
// matching docview.cpp's interface-map handling.
struct AFX_INTERFACEMAP;
struct AFX_OLECMDMAP;

namespace {
constexpr unsigned int kNoSerializationSchema = 0xFFFF;

// Local CRuntimeClass descriptors for OLE classes that have no concrete
// declaration in the repo headers. These mirror the layout MFC uses so the
// RTTI accessors return a stable, valid pointer instead of the weak-stub
// default. Object size is left 0 (factory disabled) because the true layout
// is unknown; the base is chained to CObject which IsDerivedFrom can walk.
CRuntimeClass g_rcCOleDocObjectItem = {
    "COleDocObjectItem", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleDocument = {
    "COleDocument", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleException = {
    "COleException", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleIPFrameWnd = {
    "COleIPFrameWnd", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleIPFrameWndEx = {
    "COleIPFrameWndEx", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleInsertDialog = {
    "COleInsertDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleLinkingDoc = {
    "COleLinkingDoc", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleLinksDialog = {
    "COleLinksDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleObjectFactory = {
    "COleObjectFactory", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOlePasteSpecialDialog = {
    "COlePasteSpecialDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOlePropertiesDialog = {
    "COlePropertiesDialog", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};

// Empty message map shared by the OLE frame classes below. An empty map is the
// faithful "no handlers in this class" representation (MFC chains to base via
// pfnGetBaseMap, here left null).
const AFX_MSGMAP_ENTRY g_oleEmptyEntries[] = {
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0}};
const AFX_MSGMAP g_oleEmptyMap = {nullptr, g_oleEmptyEntries};
}  // namespace

// ---------------------------------------------------------------------------
// COleDocObjectItem
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleDocObjectItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleDocObjectItem;
}
// Symbol: ?GetRuntimeClass@COleDocObjectItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDocObjectItem__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleDocObjectItem;
}
// Symbol: ?GetCommandMap@COleDocObjectItem@@MEBAPEBUAFX_OLECMDMAP@@XZ
extern "C" const AFX_OLECMDMAP* MS_ABI impl__GetCommandMap_COleDocObjectItem__MEBAPEBUAFX_OLECMDMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisCommandMap@COleDocObjectItem@@KAPEBUAFX_OLECMDMAP@@XZ
extern "C" const AFX_OLECMDMAP* MS_ABI impl__GetThisCommandMap_COleDocObjectItem__KAPEBUAFX_OLECMDMAP__XZ() {
    return nullptr;
}
// Symbol: ?GetInterfaceMap@COleDocObjectItem@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleDocObjectItem__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleDocObjectItem@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleDocObjectItem__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COleDocument
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleDocument@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleDocument;
}
// Symbol: ?GetRuntimeClass@COleDocument@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleDocument__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleDocument;
}

// ---------------------------------------------------------------------------
// COleException
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleException;
}
// Symbol: ?GetRuntimeClass@COleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleException;
}

// ---------------------------------------------------------------------------
// COleIPFrameWnd
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleIPFrameWnd@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleIPFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleIPFrameWnd;
}
// Symbol: ?GetRuntimeClass@COleIPFrameWnd@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleIPFrameWnd__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleIPFrameWnd;
}
// Symbol: ?GetMessageMap@COleIPFrameWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_COleIPFrameWnd__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return &g_oleEmptyMap;
}
// Symbol: ?GetThisMessageMap@COleIPFrameWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_COleIPFrameWnd__KAPEBUAFX_MSGMAP__XZ() {
    return &g_oleEmptyMap;
}

// ---------------------------------------------------------------------------
// COleIPFrameWndEx
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleIPFrameWndEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleIPFrameWndEx__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleIPFrameWndEx;
}
// Symbol: ?GetRuntimeClass@COleIPFrameWndEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleIPFrameWndEx__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleIPFrameWndEx;
}
// Symbol: ?GetMessageMap@COleIPFrameWndEx@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_COleIPFrameWndEx__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return &g_oleEmptyMap;
}
// Symbol: ?GetThisMessageMap@COleIPFrameWndEx@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_COleIPFrameWndEx__KAPEBUAFX_MSGMAP__XZ() {
    return &g_oleEmptyMap;
}

// ---------------------------------------------------------------------------
// COleInsertDialog
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleInsertDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleInsertDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleInsertDialog;
}
// Symbol: ?GetRuntimeClass@COleInsertDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleInsertDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleInsertDialog;
}

// ---------------------------------------------------------------------------
// COleLinkingDoc
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleLinkingDoc@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleLinkingDoc__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleLinkingDoc;
}
// Symbol: ?GetRuntimeClass@COleLinkingDoc@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleLinkingDoc__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleLinkingDoc;
}
// Symbol: ?GetInterfaceMap@COleLinkingDoc@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleLinkingDoc__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleLinkingDoc@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleLinkingDoc__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COleLinksDialog
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleLinksDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleLinksDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleLinksDialog;
}
// Symbol: ?GetRuntimeClass@COleLinksDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleLinksDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleLinksDialog;
}

// ---------------------------------------------------------------------------
// COleObjectFactory
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleObjectFactory@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleObjectFactory__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleObjectFactory;
}
// Symbol: ?GetRuntimeClass@COleObjectFactory@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleObjectFactory__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleObjectFactory;
}
// Symbol: ?GetInterfaceMap@COleObjectFactory@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleObjectFactory__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleObjectFactory@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleObjectFactory__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COlePasteSpecialDialog
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COlePasteSpecialDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COlePasteSpecialDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOlePasteSpecialDialog;
}
// Symbol: ?GetRuntimeClass@COlePasteSpecialDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COlePasteSpecialDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOlePasteSpecialDialog;
}

// ---------------------------------------------------------------------------
// COlePropertiesDialog
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COlePropertiesDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COlePropertiesDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOlePropertiesDialog;
}
// Symbol: ?GetRuntimeClass@COlePropertiesDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COlePropertiesDialog__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOlePropertiesDialog;
}

// ---------------------------------------------------------------------------
// COleDropSource
// ---------------------------------------------------------------------------
// Symbol: ?GetInterfaceMap@COleDropSource@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleDropSource__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleDropSource@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleDropSource__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}
// COleDropSource static drag-tracking defaults (well-known MFC constants).
extern "C" {
// Symbol: ?nDragDelay@COleDropSource@@1IA
unsigned int impl__nDragDelay_COleDropSource__1IA = 200;
// Symbol: ?nDragMinDist@COleDropSource@@1IA
unsigned int impl__nDragMinDist_COleDropSource__1IA = 2;
}

// ---------------------------------------------------------------------------
// COleDropTarget
// ---------------------------------------------------------------------------
// Symbol: ?GetInterfaceMap@COleDropTarget@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleDropTarget__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleDropTarget@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleDropTarget__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}
// COleDropTarget static auto-scroll defaults (well-known MFC constants).
extern "C" {
// Symbol: ?nScrollDelay@COleDropTarget@@1IA
unsigned int impl__nScrollDelay_COleDropTarget__1IA = 50;
// Symbol: ?nScrollInset@COleDropTarget@@1HA
int impl__nScrollInset_COleDropTarget__1HA = 11;
// Symbol: ?nScrollInterval@COleDropTarget@@1IA
unsigned int impl__nScrollInterval_COleDropTarget__1IA = 50;
}

// ---------------------------------------------------------------------------
// COleFrameHook
// ---------------------------------------------------------------------------
// Symbol: ?GetInterfaceMap@COleFrameHook@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleFrameHook__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleFrameHook@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleFrameHook__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COleMessageFilter
// ---------------------------------------------------------------------------
// Symbol: ?GetInterfaceMap@COleMessageFilter@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleMessageFilter__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleMessageFilter@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleMessageFilter__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}
