#pragma once
// Shared internals of the former global_csmartdockinginfo.cpp translation unit.
// Definitions live in detail/CSmartDockingInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace csmartdockinginfo {} } }
using namespace openmfc::detail::csmartdockinginfo;
// CSmartDockingInfo — real OpenMFC implementation.
//
// A CObject-derived value/settings class describing the smart-docking marker
// appearance used by the MFC docking framework (CMFCVisualManager). It carries
// bitmap resource IDs, tone/shading colors and geometry for the docking markers.
//
// Layout (cl.exe /d1reportSingleClassLayout), size(96):
//   0  {vfptr}                         (CObject base)
//   8  CSize    m_sizeTotal            (cx@8, cy@12)
//  16  int      m_nCentralGroupOffset
//  20  COLORREF m_clrTransparent
//  24  COLORREF m_clrToneSrc
//  28  COLORREF m_clrToneDest
//  32  COLORREF m_clrBaseBackground
//  36  COLORREF m_clrBaseBorder
//  40  UINT     m_uiMarkerBmpResID[5]
//  60  UINT     m_uiMarkerLightBmpResID[5]
//  80  UINT     m_uiBaseBmpResID
//  84  BOOL     m_bUseThemeColorInShading
//  88  BOOL     m_bIsAlphaMarkers
//  92  (tail padding to 8-byte alignment -> 96)

#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#define AFX_SD_MARKERS_NUM 5


namespace openmfc { namespace detail { namespace csmartdockinginfo {
struct S_Csmartdockinginfo {
    void*    vfptr;                                        // 0
    LONG     cx;                                           // 8  (m_sizeTotal.cx)
    LONG     cy;                                           // 12 (m_sizeTotal.cy)
    int      m_nCentralGroupOffset;                        // 16
    COLORREF m_clrTransparent;                             // 20
    COLORREF m_clrToneSrc;                                 // 24
    COLORREF m_clrToneDest;                                // 28
    COLORREF m_clrBaseBackground;                          // 32
    COLORREF m_clrBaseBorder;                              // 36
    UINT     m_uiMarkerBmpResID[AFX_SD_MARKERS_NUM];       // 40
    UINT     m_uiMarkerLightBmpResID[AFX_SD_MARKERS_NUM];  // 60
    UINT     m_uiBaseBmpResID;                             // 80
    BOOL     m_bUseThemeColorInShading;                    // 84
    BOOL     m_bIsAlphaMarkers;                            // 88
};
} } }


// ---- vtable slot wrappers -------------------------------------------------
// CSmartDockingInfo is not DYNAMIC, so its vtable entries are the inherited
// CObject implementations (GetRuntimeClass, Serialize, AssertValid, Dump).

// slot 0: CObject::GetRuntimeClass (not overridden) — delegate to the real
// exported CObject impl so the descriptor pointer is identical to the base
// class's, without referencing the '?'-mangled data symbol directly (mingw
// cannot emit a valid .refptr for such a name; a link-time undefined ref
// results). Same idiom as global_cmfcacceleratorkey.cpp.
extern "C" void* MS_ABI
impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);

namespace openmfc { namespace detail { namespace csmartdockinginfo {
void* MS_ABI sd_GetRuntimeClass(void* pThis);
} } }

// slot 1: vector deleting destructor. CSmartDockingInfo owns no resources.
namespace openmfc { namespace detail { namespace csmartdockinginfo {
void* MS_ABI sd_VectorDeletingDtor(void* p, unsigned int flags);
} } }

// slot 2: CObject::Serialize — delegate to the base implementation.
namespace openmfc { namespace detail { namespace csmartdockinginfo {
void MS_ABI sd_Serialize(void* pThis, void* pAr);
} } }

// slot 3: CObject::AssertValid — base implementation.
namespace openmfc { namespace detail { namespace csmartdockinginfo {
void MS_ABI sd_AssertValid(void* pThis);
} } }

// slot 4: CObject::Dump — base implementation.
namespace openmfc { namespace detail { namespace csmartdockinginfo {
void MS_ABI sd_Dump(void* pThis, void* /*dc*/);
} } }

namespace openmfc { namespace detail { namespace csmartdockinginfo {
extern void* const g_CSmartDockingInfo_vtbl[5];
} } }


// ===========================================================================
// Exports
// ===========================================================================


