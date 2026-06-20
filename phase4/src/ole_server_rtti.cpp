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
// matching docview.cpp / ole_gen_ole_com_activex_04.cpp interface-map handling.
struct AFX_INTERFACEMAP;

namespace {
constexpr unsigned int kNoSerializationSchema = 0xFFFF;

// Local CRuntimeClass descriptors for OLE classes that have no concrete
// declaration in the repo headers. These mirror the layout MFC uses so the
// RTTI accessors return a stable, valid pointer instead of the weak-stub
// default. Object size is left 0 (factory disabled) because the true layout
// is unknown; the base is chained to CObject which IsDerivedFrom can walk.
CRuntimeClass g_rcCOlePropertyPage = {
    "COlePropertyPage", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleResizeBar = {
    "COleResizeBar", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleServerDoc = {
    "COleServerDoc", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleServerDocEx = {
    "COleServerDocEx", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleServerItem = {
    "COleServerItem", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};
CRuntimeClass g_rcCOleStreamFile = {
    "COleStreamFile", 0, kNoSerializationSchema, nullptr, nullptr,
    &CObject::classCObject, nullptr};

// Empty message map shared by the OLE classes below. An empty map is the
// faithful "no handlers in this class" representation (MFC chains to base via
// pfnGetBaseMap, here left null).
const AFX_MSGMAP_ENTRY g_oleEmptyEntries[] = {
    {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0}};
const AFX_MSGMAP g_oleEmptyMap = {nullptr, g_oleEmptyEntries};
}  // namespace

// ---------------------------------------------------------------------------
// COlePropertyPage
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COlePropertyPage@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COlePropertyPage__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOlePropertyPage;
}
// Symbol: ?GetRuntimeClass@COlePropertyPage@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COlePropertyPage__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOlePropertyPage;
}
// Symbol: ?GetMessageMap@COlePropertyPage@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_COlePropertyPage__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return &g_oleEmptyMap;
}
// Symbol: ?GetThisMessageMap@COlePropertyPage@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_COlePropertyPage__KAPEBUAFX_MSGMAP__XZ() {
    return &g_oleEmptyMap;
}
// Symbol: ?GetInterfaceMap@COlePropertyPage@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COlePropertyPage__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COlePropertyPage@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COlePropertyPage__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COleResizeBar
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleResizeBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleResizeBar__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleResizeBar;
}
// Symbol: ?GetRuntimeClass@COleResizeBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleResizeBar__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleResizeBar;
}
// Symbol: ?GetMessageMap@COleResizeBar@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_COleResizeBar__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return &g_oleEmptyMap;
}
// Symbol: ?GetThisMessageMap@COleResizeBar@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_COleResizeBar__KAPEBUAFX_MSGMAP__XZ() {
    return &g_oleEmptyMap;
}

// ---------------------------------------------------------------------------
// COleServerDoc
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleServerDoc@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleServerDoc;
}
// Symbol: ?GetRuntimeClass@COleServerDoc@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleServerDoc__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleServerDoc;
}
// Symbol: ?GetMessageMap@COleServerDoc@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_COleServerDoc__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return &g_oleEmptyMap;
}
// Symbol: ?GetThisMessageMap@COleServerDoc@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_COleServerDoc__KAPEBUAFX_MSGMAP__XZ() {
    return &g_oleEmptyMap;
}
// Symbol: ?GetInterfaceMap@COleServerDoc@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleServerDoc__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleServerDoc@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleServerDoc__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COleServerDocEx
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleServerDocEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleServerDocEx;
}
// Symbol: ?GetRuntimeClass@COleServerDocEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleServerDocEx__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleServerDocEx;
}
// Symbol: ?GetMessageMap@COleServerDocEx@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_COleServerDocEx__MEBAPEBUAFX_MSGMAP__XZ(const void*) {
    return &g_oleEmptyMap;
}
// Symbol: ?GetThisMessageMap@COleServerDocEx@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_COleServerDocEx__KAPEBUAFX_MSGMAP__XZ() {
    return &g_oleEmptyMap;
}

// ---------------------------------------------------------------------------
// COleServerItem
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleServerItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleServerItem;
}
// Symbol: ?GetRuntimeClass@COleServerItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleServerItem__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleServerItem;
}
// Symbol: ?GetInterfaceMap@COleServerItem@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_COleServerItem__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) {
    return nullptr;
}
// Symbol: ?GetThisInterfaceMap@COleServerItem@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_COleServerItem__KAPEBUAFX_INTERFACEMAP__XZ() {
    return nullptr;
}

// ---------------------------------------------------------------------------
// COleStreamFile
// ---------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleStreamFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ() {
    return &g_rcCOleStreamFile;
}
// Symbol: ?GetRuntimeClass@COleStreamFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &g_rcCOleStreamFile;
}
