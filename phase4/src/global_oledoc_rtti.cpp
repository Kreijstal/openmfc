// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for the OLE document /
// server-item family (16 exports, 8 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set). The document chain
// (COleDocument -> COleLinkingDoc -> COleServerDoc -> COleServerDocEx) and the
// server-item chain (COleServerItem -> CDocObjectServerItem) chain within this
// file; the chain roots point at the existing real base descriptors that the DLL
// already defines (CDocument::classCDocument in docview.cpp,
// CDocItem::classCDocItem / COleClientItem::classCOleClientItem in olecore.cpp,
// CCmdTarget::classCCmdTarget in appcore.cpp). This reproduces the retail
// RUNTIME_CLASS graph IsKindOf/IsDerivedFrom walk. m_nObjectSize is the real
// MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout. GetRuntimeClass
// returns the static descriptor directly (never pThis->GetRuntimeClass(), which
// would recurse through this very export in a drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxole.h"   // CDocument/CDocItem/COleClientItem/CCmdTarget + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define OLED_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// Parents-first so each derived descriptor can take its base's address.
OLED_DESC(COleDocument,         488, &CDocument::classCDocument);
OLED_DESC(COleLinkingDoc,       560, &classCOleDocument);
OLED_DESC(COleServerDoc,        664, &classCOleLinkingDoc);
OLED_DESC(COleServerDocEx,      664, &classCOleServerDoc);
OLED_DESC(COleServerItem,       224, &CDocItem::classCDocItem);
OLED_DESC(CDocObjectServerItem, 224, &classCOleServerItem);
OLED_DESC(COleObjectFactory,    136, &CCmdTarget::classCCmdTarget);
OLED_DESC(COleDocObjectItem,    280, &COleClientItem::classCOleClientItem);
#undef OLED_DESC
} // namespace

// Two exported getters per class via a macro; the literal "// Symbol:" marker
// lines stay in the source so build_phase4.sh's grep auto-excludes the weak stubs.
#define OLED_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@COleDocument@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDocument@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleDocument,
             impl__GetThisClass_COleDocument__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleDocument__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleLinkingDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleLinkingDoc@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleLinkingDoc,
             impl__GetThisClass_COleLinkingDoc__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleLinkingDoc__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleServerDoc@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleServerDoc@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleServerDoc,
             impl__GetThisClass_COleServerDoc__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleServerDoc__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleServerDocEx@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleServerDocEx@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleServerDocEx,
             impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleServerDocEx__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleServerItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleServerItem@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleServerItem,
             impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleServerItem__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CDocObjectServerItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CDocObjectServerItem@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(CDocObjectServerItem,
             impl__GetThisClass_CDocObjectServerItem__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_CDocObjectServerItem__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleObjectFactory@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleObjectFactory@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleObjectFactory,
             impl__GetThisClass_COleObjectFactory__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleObjectFactory__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@COleDocObjectItem@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@COleDocObjectItem@@UEBAPEAUCRuntimeClass@@XZ
OLED_GETTERS(COleDocObjectItem,
             impl__GetThisClass_COleDocObjectItem__SAPEAUCRuntimeClass__XZ,
             impl__GetRuntimeClass_COleDocObjectItem__UEBAPEAUCRuntimeClass__XZ)

#undef OLED_GETTERS
