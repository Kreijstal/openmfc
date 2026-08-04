#include "CMemDCSupport.h"

static_assert(sizeof(CDC_S) == 32, "embedded CDC must be 32 bytes");
static_assert(sizeof(CBitmap_S) == 16, "embedded CBitmap must be 16 bytes");
static_assert(sizeof(S_Cmemdc) == 104,                 "CMemDC size must match harvested layout");
static_assert(offsetof(S_Cmemdc, m_dc) == 8,           "m_dc @8");
static_assert(offsetof(S_Cmemdc, m_bMemDC) == 16,      "m_bMemDC @16");
static_assert(offsetof(S_Cmemdc, m_hBufferedPaint) == 24, "m_hBufferedPaint @24");
static_assert(offsetof(S_Cmemdc, m_dcMem) == 32,       "m_dcMem @32");
static_assert(offsetof(S_Cmemdc, m_bmp) == 64,         "m_bmp @64");
static_assert(offsetof(S_Cmemdc, m_pOldBmp) == 80,     "m_pOldBmp @80");
static_assert(offsetof(S_Cmemdc, m_rect) == 88,        "m_rect @88");
extern "C" {
void* MS_ABI impl___0CMemDC__QEAA_AEAVCDC__AEBVCRect___Z(void* pThis, void* pDC, const RECT* pRect) {
    S_Cmemdc* self = reinterpret_cast<S_Cmemdc*>(pThis);
    InitCommon(self, pDC);
    if (pRect != nullptr) {
        self->m_rect = *pRect;
    } else {
        self->m_rect.left = self->m_rect.top = self->m_rect.right = self->m_rect.bottom = 0;
    }
    ConstructBuffer(self);
    return pThis;
}
void* MS_ABI impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(void* pThis, void* pDC, void* pWnd) {
    S_Cmemdc* self = reinterpret_cast<S_Cmemdc*>(pThis);
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
void MS_ABI impl___1CMemDC__UEAA_XZ(void* pThis) {
    DestructBody(reinterpret_cast<S_Cmemdc*>(pThis));
}
}
namespace openmfc { namespace detail { namespace cmemdc {
void ConstructBuffer(S_Cmemdc* self) {
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
void DestructBody(S_Cmemdc* self) {
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
void* MS_ABI vdtor_Cmemdc(void* p, unsigned f) {
    DestructBody(reinterpret_cast<S_Cmemdc*>(p));
    if (f & 1) ::operator delete(p);
    return p;
}
} } }  // namespace openmfc::detail::cmemdc
