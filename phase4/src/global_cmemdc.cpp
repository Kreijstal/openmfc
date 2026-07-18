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

namespace {

// Embedded CDC view — only the fields we must populate matter; padded to the real size(32).
struct CDC_S {
    void* vfptr;        // +0  (relative to sub-object)
    HDC   m_hDC;        // +8
    HDC   m_hAttribDC;  // +16
    int   m_bPrinting;  // +24
    int   _pad;         // +28  -> total 32
};
static_assert(sizeof(CDC_S) == 32, "embedded CDC must be 32 bytes");

// Embedded CBitmap view (CGdiObject) — padded to the real size(16).
struct CBitmap_S {
    void*   vfptr;      // +0
    HGDIOBJ m_hObject;  // +8  -> total 16
};
static_assert(sizeof(CBitmap_S) == 16, "embedded CBitmap must be 16 bytes");

// Layout-faithful view of CMemDC, harvested size(104).
struct S {
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

static_assert(sizeof(S) == 104,                 "CMemDC size must match harvested layout");
static_assert(offsetof(S, m_dc) == 8,           "m_dc @8");
static_assert(offsetof(S, m_bMemDC) == 16,      "m_bMemDC @16");
static_assert(offsetof(S, m_hBufferedPaint) == 24, "m_hBufferedPaint @24");
static_assert(offsetof(S, m_dcMem) == 32,       "m_dcMem @32");
static_assert(offsetof(S, m_bmp) == 64,         "m_bmp @64");
static_assert(offsetof(S, m_pOldBmp) == 80,     "m_pOldBmp @80");
static_assert(offsetof(S, m_rect) == 88,        "m_rect @88");

// CWnd::m_hWnd lives at offset 64. CDC::m_hDC lives at offset 8.
inline HDC dc_hdc(void* pCDC) {
    return pCDC ? reinterpret_cast<CDC_S*>(pCDC)->m_hDC : nullptr;
}
inline int rect_w(const RECT& r) { return r.right - r.left; }
inline int rect_h(const RECT& r) { return r.bottom - r.top; }

} // namespace

// CMemDC::m_bUseMemoryDC — global gate (defaults TRUE) letting an app force straight-to-DC
// drawing by clearing it before constructing a CMemDC.  Exported as data.
// Symbol: ?m_bUseMemoryDC@CMemDC@@2HA
extern "C" int impl__m_bUseMemoryDC_CMemDC__2HA = TRUE;

namespace {

// Shared construction body: with m_dc / m_rect already set and members zeroed, establish the
// off-screen surface following the real CMemDC ctor logic. Leaves GetDC() returning a live DC.
void ConstructBuffer(S* self) {
    HDC hdcTarget = dc_hdc(self->m_dc);

    // Nothing to buffer against, or the app opted out: draw straight to the target DC.
    if (hdcTarget == nullptr || !impl__m_bUseMemoryDC_CMemDC__2HA) {
        return;
    }

    const int w = rect_w(self->m_rect);
    const int h = rect_h(self->m_rect);

    // Preferred path (Vista+ DWM): a compatible-bitmap buffered-paint session whose DC we hand
    // to the caller through m_dcMem. EndBufferedPaint(TRUE) later flushes it to the target.
    HDC hdcPaint = nullptr;
    HPAINTBUFFER hbp = ::BeginBufferedPaint(hdcTarget, &self->m_rect,
                                            BPBF_COMPATIBLEBITMAP, nullptr, &hdcPaint);
    if (hbp != nullptr && hdcPaint != nullptr) {
        self->m_hBufferedPaint = hbp;
        self->m_dcMem.m_hDC = hdcPaint;      // Attach — not owned by us, released via EndBufferedPaint
        self->m_dcMem.m_hAttribDC = hdcPaint;
        return;
    }
    if (hbp != nullptr) {
        ::EndBufferedPaint(hbp, FALSE);      // opened but no usable DC — abandon it
    }

    // Classic fallback: an owned memory DC + compatible bitmap selected into it.
    HDC hdcMem = ::CreateCompatibleDC(hdcTarget);
    if (hdcMem == nullptr) {
        return;                              // out of resources — GetDC() falls back to m_dc
    }
    self->m_bMemDC = TRUE;
    self->m_dcMem.m_hDC = hdcMem;
    self->m_dcMem.m_hAttribDC = hdcMem;

    HBITMAP hbmp = ::CreateCompatibleBitmap(hdcTarget, w, h);
    if (hbmp != nullptr) {
        self->m_bmp.m_hObject = hbmp;
        HGDIOBJ hOld = ::SelectObject(hdcMem, hbmp);
        // MFC stores the deselected CBitmap*; we round-trip the raw GDI handle through the
        // same slot so the destructor can restore it before deleting our bitmap.
        self->m_pOldBmp = hOld;
    }
}

// Full ~CMemDC destruction body (also invoked from the deleting-dtor vtable slot). Flushes the
// off-screen surface back to the target DC and releases every GDI resource we created,
// including the embedded m_dcMem / m_bmp sub-objects.
void DestructBody(S* self) {
    if (self->m_hBufferedPaint != nullptr) {
        // Buffered-paint path: TRUE => copy the buffer onto the target, then detach its DC.
        ::EndBufferedPaint(static_cast<HPAINTBUFFER>(self->m_hBufferedPaint), TRUE);
        self->m_hBufferedPaint = nullptr;
        self->m_dcMem.m_hDC = nullptr;       // Detach (buffer owned the DC)
        self->m_dcMem.m_hAttribDC = nullptr;
    } else if (self->m_bMemDC && self->m_dcMem.m_hDC != nullptr) {
        // Classic path: blit the finished buffer back to the real DC, then unwind selections.
        HDC hdcTarget = dc_hdc(self->m_dc);
        HDC hdcMem = self->m_dcMem.m_hDC;
        if (hdcTarget != nullptr) {
            ::BitBlt(hdcTarget, self->m_rect.left, self->m_rect.top,
                     rect_w(self->m_rect), rect_h(self->m_rect),
                     hdcMem, 0, 0, SRCCOPY);
        }
        if (self->m_pOldBmp != nullptr) {
            ::SelectObject(hdcMem, static_cast<HGDIOBJ>(self->m_pOldBmp));
            self->m_pOldBmp = nullptr;
        }
        if (self->m_bmp.m_hObject != nullptr) {
            ::DeleteObject(self->m_bmp.m_hObject);   // ~CBitmap
            self->m_bmp.m_hObject = nullptr;
        }
        ::DeleteDC(hdcMem);                          // ~CDC (owned memory DC)
        self->m_dcMem.m_hDC = nullptr;
        self->m_dcMem.m_hAttribDC = nullptr;
        self->m_bMemDC = FALSE;
    }
}

// Vector-deleting-destructor (sole vtable slot, per harvested vtable order).
void* MS_ABI vdtor(void* p, unsigned f) {
    DestructBody(reinterpret_cast<S*>(p));
    if (f & 1) ::operator delete(p);
    return p;
}

void* const g_CMemDC_vtbl[1] = {
    reinterpret_cast<void*>(&vdtor)
};

// Zero the whole object, install the vptr, and record the target DC.
inline void InitCommon(S* self, void* pDC) {
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

} // namespace

extern "C" {

// CMemDC(CDC& dc, const CRect& rect): buffer against the caller-supplied rectangle.
// CRect passed by const-reference arrives as a pointer.
// Symbol: ??0CMemDC@@QEAA@AEAVCDC@@AEBVCRect@@@Z
void* MS_ABI impl___0CMemDC__QEAA_AEAVCDC__AEBVCRect___Z(void* pThis, void* pDC, const RECT* pRect) {
    S* self = reinterpret_cast<S*>(pThis);
    InitCommon(self, pDC);
    if (pRect != nullptr) {
        self->m_rect = *pRect;
    } else {
        self->m_rect.left = self->m_rect.top = self->m_rect.right = self->m_rect.bottom = 0;
    }
    ConstructBuffer(self);
    return pThis;
}

// CMemDC(CDC& dc, CWnd* pWnd): buffer against pWnd's client rectangle.
// Symbol: ??0CMemDC@@QEAA@AEAVCDC@@PEAVCWnd@@@Z
void* MS_ABI impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(void* pThis, void* pDC, void* pWnd) {
    S* self = reinterpret_cast<S*>(pThis);
    InitCommon(self, pDC);
    RECT rc = { 0, 0, 0, 0 };
    if (pWnd != nullptr) {
        HWND hWnd = *reinterpret_cast<HWND*>(static_cast<char*>(pWnd) + 64);  // CWnd::m_hWnd @64
        if (::IsWindow(hWnd)) {
            ::GetClientRect(hWnd, &rc);
        }
    }
    self->m_rect = rc;
    ConstructBuffer(self);
    return pThis;
}

// virtual ~CMemDC(): flush the buffer to the target DC and release all GDI resources.
// Symbol: ??1CMemDC@@UEAA@XZ
void MS_ABI impl___1CMemDC__UEAA_XZ(void* pThis) {
    DestructBody(reinterpret_cast<S*>(pThis));
}

} // extern "C"
