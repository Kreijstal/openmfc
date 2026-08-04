#pragma once
// Shared internals of the former global_cmemdc.cpp translation unit.
// Definitions live in detail/CMemDCSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmemdc {} } }
using namespace openmfc::detail::cmemdc;
// OpenMFC — CMemDC real implementation
//
// CMemDC (afxwin.h, feature-pack flavour) is a lightweight flicker-free double-buffering
// helper. Given a target CDC (typically the paint DC of a control) it creates an off-screen
// memory device context + a compatible bitmap sized to the target rectangle, redirects all
// drawing there via GetDC(), and on destruction blits the finished buffer back to the real
// device context in a single BitBlt. On Vista+ it first tries the DWM buffered-paint API
// (BeginBufferedPaint) and, only if that is unavailable, falls back to the classic memory-DC
// path (CreateCompatibleDC + CreateCompatibleBitmap + SelectObject).
//
// Harvested layout (cl.exe /d1reportSingleClassLayout): size(104)
//    0  {vfptr}            CObject vtable pointer (deleting-dtor slot only)
//    8  m_dc               CDC&  -> stored as CDC*
//   16  m_bMemDC           BOOL  (memory-DC path succeeded)
//   24  m_hBufferedPaint   HANDLE (HPAINTBUFFER; NULL unless buffered-paint path used)
//   32  m_dcMem            CDC   embedded (32 bytes): sub-vfptr@32, m_hDC@40, m_hAttribDC@48
//   64  m_bmp              CBitmap embedded (16 bytes): sub-vfptr@64, m_hObject@72
//   80  m_pOldBmp          CBitmap*  (object deselected out of m_dcMem)
//   88  m_rect             CRect/RECT (16 bytes)
//
// The embedded m_dcMem / m_bmp carry the created GDI handles at their real member offsets so
// that a client calling GetDC() (an inline accessor that returns &m_dcMem) obtains a CDC whose
// m_hDC is the live off-screen DC — every non-virtual CDC drawing thunk in this DLL dispatches
// off that m_hDC. This file is self-contained: no project headers, no CDC/CBitmap vtable
// dependency; the buffer round-trip and cleanup are performed with raw GDI here.

#include <windows.h>
#include <uxtheme.h>
#include <cstddef>
// Buffered-paint APIs come from the uxtheme import library (-luxtheme), matching
// the real MFC CMemDC which links uxtheme directly.

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Embedded CDC view — only the fields we must populate matter; padded to the real size(32).
namespace openmfc { namespace detail { namespace cmemdc {
struct CDC_S {
    void* vfptr;        // +0  (relative to sub-object)
    HDC   m_hDC;        // +8
    HDC   m_hAttribDC;  // +16
    int   m_bPrinting;  // +24
    int   _pad;         // +28  -> total 32
};
} } }

// Embedded CBitmap view (CGdiObject) — padded to the real size(16).
namespace openmfc { namespace detail { namespace cmemdc {
struct CBitmap_S {
    void*   vfptr;      // +0
    HGDIOBJ m_hObject;  // +8  -> total 16
};
} } }

// Layout-faithful view of CMemDC, harvested size(104).
namespace openmfc { namespace detail { namespace cmemdc {
struct S_Cmemdc {
    void*     vfptr;            //  0
    void*     m_dc;             //  8  CDC&
    BOOL      m_bMemDC;         // 16
    int       _pad0;            // 20 (alignment for the HANDLE that follows)
    HANDLE    m_hBufferedPaint; // 24
    CDC_S     m_dcMem;          // 32
    CBitmap_S m_bmp;            // 64
    void*     m_pOldBmp;        // 80 CBitmap*
    RECT      m_rect;           // 88
};
} } }


// CWnd::m_hWnd lives at offset 64. CDC::m_hDC lives at offset 8.
namespace openmfc { namespace detail { namespace cmemdc {
inline HDC dc_hdc(void* pCDC) {
    return pCDC ? reinterpret_cast<CDC_S*>(pCDC)->m_hDC : nullptr;
}
} } }
namespace openmfc { namespace detail { namespace cmemdc {
inline int rect_w(const RECT& r) { return r.right - r.left; }
} } }
namespace openmfc { namespace detail { namespace cmemdc {
inline int rect_h(const RECT& r) { return r.bottom - r.top; }
} } }




// Shared construction body: with m_dc / m_rect already set and members zeroed, establish the
// off-screen surface following the real CMemDC ctor logic. Leaves GetDC() returning a live DC.
// CMemDC::m_bUseMemoryDC — the static opt-out flag, defined with the class.
extern "C" int impl__m_bUseMemoryDC_CMemDC__2HA;

namespace openmfc { namespace detail { namespace cmemdc {
void ConstructBuffer(S_Cmemdc* self);
} } }

// Full ~CMemDC destruction body (also invoked from the deleting-dtor vtable slot). Flushes the
// off-screen surface back to the target DC and releases every GDI resource we created,
// including the embedded m_dcMem / m_bmp sub-objects.
namespace openmfc { namespace detail { namespace cmemdc {
void DestructBody(S_Cmemdc* self);
} } }

// Vector-deleting-destructor (sole vtable slot, per harvested vtable order).
namespace openmfc { namespace detail { namespace cmemdc {
void* MS_ABI vdtor_Cmemdc(void* p, unsigned f);
} } }

namespace openmfc { namespace detail { namespace cmemdc {
extern void* const g_CMemDC_vtbl[1];
} } }

// Zero the whole object, install the vptr, and record the target DC.
namespace openmfc { namespace detail { namespace cmemdc {
inline void InitCommon(S_Cmemdc* self, void* pDC) {
    self->vfptr = (void*)&g_CMemDC_vtbl[0];
    self->m_dc = pDC;
    self->m_bMemDC = FALSE;
    self->_pad0 = 0;
    self->m_hBufferedPaint = nullptr;
    self->m_dcMem.vfptr = nullptr;
    self->m_dcMem.m_hDC = nullptr;
    self->m_dcMem.m_hAttribDC = nullptr;
    self->m_dcMem.m_bPrinting = 0;
    self->m_dcMem._pad = 0;
    self->m_bmp.vfptr = nullptr;
    self->m_bmp.m_hObject = nullptr;
    self->m_pOldBmp = nullptr;
}
} } }


extern "C" {


// CMemDC(CDC& dc, const CRect& rect): buffer against the caller-supplied rectangle.
// CRect passed by const-reference arrives as a pointer.
// Symbol: ??0CMemDC@@QEAA@AEAVCDC@@AEBVCRect@@@Z
void* MS_ABI impl___0CMemDC__QEAA_AEAVCDC__AEBVCRect___Z(void* pThis, void* pDC, const RECT* pRect);

// CMemDC(CDC& dc, CWnd* pWnd): buffer against pWnd's client rectangle.
// Symbol: ??0CMemDC@@QEAA@AEAVCDC@@PEAVCWnd@@@Z
void* MS_ABI impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(void* pThis, void* pDC, void* pWnd);

// virtual ~CMemDC(): flush the buffer to the target DC and release all GDI resources.
// Symbol: ??1CMemDC@@UEAA@XZ
void MS_ABI impl___1CMemDC__UEAA_XZ(void* pThis);

}
