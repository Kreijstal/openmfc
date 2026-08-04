#pragma once
// Shared internals of the former global_cmfccaptionbuttonex.cpp translation unit.
// Definitions live in detail/CMFCCaptionButtonExSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {} } }
using namespace openmfc::detail::cmfccaptionbuttonex;
// OpenMFC — real implementation of CMFCCaptionButtonEx
//
// class CMFCCaptionButtonEx  size(72):
//   0  | +--- (base CMFCCaptionButton : CObject)
//   0  | | {vfptr}
//   8  | | m_bPushed            (BOOL)
//  12  | | m_bFocused           (BOOL)
//  16  | | m_bHidden            (BOOL)
//  20  | | m_bEnabled           (BOOL)
//  24  | | m_bDroppedDown       (BOOL)
//  28  | | m_bLeftAlign         (BOOL)
//  32  | | m_nHit               (UINT)
//  36  | | m_clrForeground      (COLORREF)
//  40  | | CPoint m_ptOrg       (LONG x, LONG y)
//  48  | | m_bIsMiniFrameButton (BOOL)
//  52  | | <alignment padding>  (4)
//  56  | CRect m_rect           (LONG left,top,right,bottom)
//
#include <windows.h>
#include <cstddef>
#include "openmfc/afx.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

struct CRuntimeClass;

extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CObject__UEBAPEAUCRuntimeClass__XZ(const void* pThis);


namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
struct S_Cmfccaptionbuttonex {
    void*    vfptr;                // 0
    LONG     m_bPushed;           // 8
    LONG     m_bFocused;          // 12
    LONG     m_bHidden;           // 16
    LONG     m_bEnabled;          // 20
    LONG     m_bDroppedDown;      // 24
    LONG     m_bLeftAlign;        // 28
    UINT     m_nHit;             // 32
    COLORREF m_clrForeground;    // 36
    LONG     m_ptOrg_x;          // 40
    LONG     m_ptOrg_y;          // 44
    LONG     m_bIsMiniFrameButton;// 48
    LONG     m_pad;              // 52 (alignment member)
    RECT     m_rect;             // 56 (16 bytes)
};
} } }


namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
struct OpenMfcRuntimeClass_Cmfccaptionbuttonex {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnCreateObject;
    void* m_pfnGetBaseClass;
    OpenMfcRuntimeClass_Cmfccaptionbuttonex* m_pBaseClass;
    void* m_pClassContext;
};
} } }

namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
extern OpenMfcRuntimeClass_Cmfccaptionbuttonex g_CMFCCaptionButtonEx_rtti;
} } }

namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
OpenMfcRuntimeClass_Cmfccaptionbuttonex* GetBaseClass_Cmfccaptionbuttonex();
} } }


// Forward decls of the exported thunks so the vtable can reference them.
extern "C" void MS_ABI impl___1CMFCCaptionButtonEx__UEAA_XZ(void* pThis);


// --- vtable slot wrappers -------------------------------------------------

// slot 0: CObject::GetRuntimeClass — CMFCCaptionButton/Ex do not publish a
// runtime-class descriptor through this self-contained TU; use a local descriptor
// with CObject as base so RTTI walks to a real root instead of nullptr.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void* MS_ABI vt_GetRuntimeClass_Cmfccaptionbuttonex(void* /*pThis*/);
} } }

// slot 1: CMFCCaptionButtonEx::{dtor}  (vector-deleting-destructor wrapper).
// Runs the real destructor logic, then frees storage when the low flag bit
// is set (delete / delete[] semantics).
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void* MS_ABI vt_vdtor_Cmfccaptionbuttonex(void* p, unsigned flags);
} } }

// slot 2: CObject::Serialize — CObject's base implementation is a no-op.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void MS_ABI vt_Serialize_Cmfccaptionbuttonex(void* pThis, void* pAr);
} } }

// slot 3: CObject::AssertValid — no-op in release semantics.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void MS_ABI vt_AssertValid_Cmfccaptionbuttonex(void* pThis);
} } }

// slot 4: CObject::Dump — no-op.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void MS_ABI vt_Dump_Cmfccaptionbuttonex(void* pThis, void* /*dc*/);
} } }

// slot 5: CMFCCaptionButtonEx::GetRect — returns m_rect by value.
// A 16-byte struct return uses a hidden return-buffer pointer under the MS ABI.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void* MS_ABI vt_GetRect(void* pThis, void* retbuf);
} } }

// slot 6: CMFCCaptionButton::OnDraw — renders the button glyph. The full
// visual rendering depends on library-internal drawing managers; provide a
// faithful no-op for hidden buttons and otherwise leave the surface untouched.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
void MS_ABI vt_OnDraw(void* /*pThis*/, void* /*pDC*/, unsigned char /*bActive*/,
                      unsigned char /*bDarkBackground*/);
} } }

// slot 7: CMFCCaptionButton::GetIconID — maps the hit code to a marlett/glyph
// icon id. Return the hit code itself as the id (the identity mapping the
// framework uses when no special glyph override is registered).
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
UINT MS_ABI vt_GetIconID(void* pThis, unsigned char /*bHorz*/);
} } }

namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
extern void* const g_CMFCCaptionButtonEx_vtbl[8];
} } }



