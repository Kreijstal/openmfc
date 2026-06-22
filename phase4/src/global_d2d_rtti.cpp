// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for the CD2D* Direct2D
// resource wrapper family (28 exports, 14 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory), following the repo's own IMPLEMENT_DYNAMIC convention
// (m_pfnGetBaseClass null, m_pBaseClass set). The CD2D* tree chains within itself
// (e.g. CD2DSolidColorBrush -> CD2DBrush -> CD2DResource), so m_pBaseClass points
// at the in-file descriptor that the base class's own GetThisClass returns; the
// root CD2DResource chains to CObject's real descriptor. This reproduces the
// retail RUNTIME_CLASS graph that IsKindOf/IsDerivedFrom walk. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout.
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CObject + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
// BaseDesc is the address of the base class's CRuntimeClass descriptor — either
// CObject's real member or an earlier in-file CD2D* descriptor.
#define D2D_DESC(Cls, Size, BaseDesc) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, (BaseDesc), nullptr }

// Declared parents-first so each derived descriptor can take the address of its
// already-defined base descriptor.
D2D_DESC(CD2DResource,            24,  &CObject::classCObject);
D2D_DESC(CD2DBrush,               40,  &classCD2DResource);
D2D_DESC(CD2DGeometry,            32,  &classCD2DResource);
D2D_DESC(CD2DBitmap,              80,  &classCD2DResource);
D2D_DESC(CD2DLayer,               32,  &classCD2DResource);
D2D_DESC(CD2DMesh,                32,  &classCD2DResource);
D2D_DESC(CD2DTextFormat,          32,  &classCD2DResource);
D2D_DESC(CD2DTextLayout,          32,  &classCD2DResource);
D2D_DESC(CD2DSolidColorBrush,     64,  &classCD2DBrush);
D2D_DESC(CD2DGradientBrush,       96,  &classCD2DBrush);
D2D_DESC(CD2DBitmapBrush,         64,  &classCD2DBrush);
D2D_DESC(CD2DPathGeometry,        40,  &classCD2DGeometry);
D2D_DESC(CD2DLinearGradientBrush, 120, &classCD2DGradientBrush);
D2D_DESC(CD2DRadialGradientBrush, 128, &classCD2DGradientBrush);
#undef D2D_DESC
} // namespace

// Two exported getters per class. Defined via a macro because each pair is
// identical bar the class name; the literal "// Symbol:" marker lines remain
// in the source (build_phase4.sh greps them to auto-exclude the weak stubs).
#define D2D_GETTERS(Cls, GetThisSym, GetRtSym) \
    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \
    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }

// Symbol: ?GetThisClass@CD2DResource@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DResource@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DResource,
            impl__GetThisClass_CD2DResource__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DResource__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DBrush@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DBrush@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DBrush,
            impl__GetThisClass_CD2DBrush__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DBrush__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DGeometry@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DGeometry@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DGeometry,
            impl__GetThisClass_CD2DGeometry__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DGeometry__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DBitmap@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DBitmap@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DBitmap,
            impl__GetThisClass_CD2DBitmap__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DBitmap__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DLayer@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DLayer@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DLayer,
            impl__GetThisClass_CD2DLayer__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DLayer__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DMesh@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DMesh@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DMesh,
            impl__GetThisClass_CD2DMesh__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DMesh__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DTextFormat@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DTextFormat@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DTextFormat,
            impl__GetThisClass_CD2DTextFormat__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DTextFormat__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DTextLayout@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DTextLayout@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DTextLayout,
            impl__GetThisClass_CD2DTextLayout__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DTextLayout__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DSolidColorBrush@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DSolidColorBrush@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DSolidColorBrush,
            impl__GetThisClass_CD2DSolidColorBrush__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DSolidColorBrush__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DGradientBrush@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DGradientBrush@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DGradientBrush,
            impl__GetThisClass_CD2DGradientBrush__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DGradientBrush__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DBitmapBrush@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DBitmapBrush@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DBitmapBrush,
            impl__GetThisClass_CD2DBitmapBrush__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DBitmapBrush__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DPathGeometry@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DPathGeometry@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DPathGeometry,
            impl__GetThisClass_CD2DPathGeometry__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DPathGeometry__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DLinearGradientBrush@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DLinearGradientBrush@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DLinearGradientBrush,
            impl__GetThisClass_CD2DLinearGradientBrush__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DLinearGradientBrush__UEBAPEAUCRuntimeClass__XZ)

// Symbol: ?GetThisClass@CD2DRadialGradientBrush@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetRuntimeClass@CD2DRadialGradientBrush@@UEBAPEAUCRuntimeClass@@XZ
D2D_GETTERS(CD2DRadialGradientBrush,
            impl__GetThisClass_CD2DRadialGradientBrush__SAPEAUCRuntimeClass__XZ,
            impl__GetRuntimeClass_CD2DRadialGradientBrush__UEBAPEAUCRuntimeClass__XZ)

#undef D2D_GETTERS
