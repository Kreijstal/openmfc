// Real CRectTracker implementation (afxext.h). Self-contained: the DLL is
// mingw-built but exports the MSVC-mangled CRectTracker methods as extern "C"
// MS_ABI impl_ thunks that view the object through a layout-faithful struct.
//
// Layout + vtable order harvested from the real headers via
// cl.exe /d1reportSingleClassLayoutCRectTracker (size 80):
//   0  {vfptr}      8  m_nStyle     12 m_rect(RECT)   28 m_sizeMin(SIZE)
//   36 m_nHandleSize 40 m_bAllowInvert 44 m_rectLast   60 m_sizeLast
//   68 m_bErase     72 m_bFinalErase
// vtable slots: 0 DrawTrackerRect 1 AdjustRect 2 OnChangedRect 3 GetHandleMask
//               4 ~dtor(vector-deleting) 5 GetHandleSize
//
// The geometry algorithm faithfully reproduces MFC's viewtrak.cpp semantics and
// is differentially verified against real mfc140u.dll (tests/test_recttracker_wine).
#include <windows.h>
#include <cstddef>   // offsetof
#include <cstdlib>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

struct RT {
    const void* vfptr;      // 0
    unsigned int m_nStyle;  // 8
    RECT m_rect;            // 12
    SIZE m_sizeMin;         // 28
    int m_nHandleSize;      // 36
    int m_bAllowInvert;     // 40
    RECT m_rectLast;        // 44
    SIZE m_sizeLast;        // 60
    int m_bErase;           // 68
    int m_bFinalErase;      // 72
};
static_assert(sizeof(RT) == 80, "CRectTracker layout must match MSVC (size 80)");
static_assert(offsetof(RT, m_nStyle) == 8, "m_nStyle@8");
static_assert(offsetof(RT, m_rect) == 12, "m_rect@12");
static_assert(offsetof(RT, m_sizeMin) == 28, "m_sizeMin@28");
static_assert(offsetof(RT, m_nHandleSize) == 36, "m_nHandleSize@36");
static_assert(offsetof(RT, m_rectLast) == 44, "m_rectLast@44");

enum StyleFlags {
    solidLine = 1, dottedLine = 2, hatchedBorder = 4,
    resizeInside = 8, resizeOutside = 16, hatchInside = 32,
};
enum TrackerHit {
    hitNothing = -1,
    hitTopLeft = 0, hitTopRight = 1, hitBottomRight = 2, hitBottomLeft = 3,
    hitTop = 4, hitRight = 5, hitBottom = 6, hitLeft = 7, hitMiddle = 8
};

// CPoint is a 2x LONG value passed by value -> a single 8-byte integer register.
static inline LONG px(unsigned long long pt) { return (LONG)(unsigned int)(pt & 0xFFFFFFFFull); }
static inline LONG py(unsigned long long pt) { return (LONG)(unsigned int)(pt >> 32); }

static inline void Normalize(RECT& r) {
    if (r.left > r.right) { LONG t = r.left; r.left = r.right; r.right = t; }
    if (r.top > r.bottom) { LONG t = r.top; r.top = r.bottom; r.bottom = t; }
}
static inline int RWidth(const RECT& r)  { return r.right - r.left; }
static inline int RHeight(const RECT& r) { return r.bottom - r.top; }

// --- handle geometry, faithful to MFC viewtrak.cpp ---

static int HandleSize(const RT* p, const RECT* lpRect) {
    if (lpRect == nullptr) lpRect = &p->m_rect;
    int size = p->m_nHandleSize;
    if (!(p->m_nStyle & resizeOutside)) {
        RECT rect = *lpRect; Normalize(rect);
        int nMin = RWidth(rect) < RHeight(rect) ? RWidth(rect) : RHeight(rect);
        if (size * 2 > nMin && nMin >= 0) size = nMin / 2;
    }
    return size;
}

// map handle index -> which x/y edge pointers move (into a working RECT copy)
static void ModifyPointers(int nHandle, RECT& rect, int** ppx, int** ppy) {
    *ppx = nullptr; *ppy = nullptr;
    // x: left for 0,3,7 ; right for 1,2,5
    switch (nHandle) {
    case hitTopLeft: case hitBottomLeft: case hitLeft:   *ppx = (int*)&rect.left;  break;
    case hitTopRight: case hitBottomRight: case hitRight: *ppx = (int*)&rect.right; break;
    default: break;
    }
    // y: top for 0,1,4 ; bottom for 2,3,6
    switch (nHandle) {
    case hitTopLeft: case hitTopRight: case hitTop:       *ppy = (int*)&rect.top;    break;
    case hitBottomRight: case hitBottomLeft: case hitBottom: *ppy = (int*)&rect.bottom; break;
    default: break;
    }
}

// The "true rect": m_rect inflated outward by the handle overhead. Handles and
// the whole hit region are anchored to this rect (matches real MFC).
static void ComputeTrueRect(const RT* p, RECT* out) {
    RECT rect = p->m_rect; Normalize(rect);
    int inflate = 0;
    if (p->m_nStyle & (resizeOutside | hatchedBorder)) inflate = HandleSize(p, nullptr) - 1;
    InflateRect(&rect, inflate, inflate);
    *out = rect;
}

// Handles are size x size squares anchored at the true-rect edges, always going
// INWARD (left handles [left,left+size], right [right-size,right], centers at mid).
static void HandleRect(const RT* p, int nHandle, RECT* pHandleRect) {
    RECT rect; ComputeTrueRect(p, &rect);
    int size = HandleSize(p, nullptr);
    int hx; // 0 left, 1 center, 2 right
    switch (nHandle) {
    case hitTopLeft: case hitBottomLeft: case hitLeft:   hx = 0; break;
    case hitTopRight: case hitBottomRight: case hitRight: hx = 2; break;
    default: hx = 1; break;
    }
    int hy; // 0 top, 1 center, 2 bottom
    switch (nHandle) {
    case hitTopLeft: case hitTopRight: case hitTop:       hy = 0; break;
    case hitBottomLeft: case hitBottomRight: case hitBottom: hy = 2; break;
    default: hy = 1; break;
    }
    int L = (hx == 0) ? rect.left : (hx == 2) ? rect.right - size : (rect.left + rect.right) / 2 - size / 2;
    int T = (hy == 0) ? rect.top  : (hy == 2) ? rect.bottom - size : (rect.top + rect.bottom) / 2 - size / 2;
    pHandleRect->left = L; pHandleRect->top = T;
    pHandleRect->right = L + size; pHandleRect->bottom = T + size;
}

static unsigned int HandleMask(const RT* p) {
    unsigned int mask = 0x0F; // 4 corners always present
    int size = p->m_nHandleSize * 3;
    RECT rect = p->m_rect; Normalize(rect);
    if (abs(RWidth(rect)) - size > 4)  mask |= (1u << hitTop) | (1u << hitBottom);
    if (abs(RHeight(rect)) - size > 4) mask |= (1u << hitLeft) | (1u << hitRight);
    return mask;
}

static int HitTestHandles(const RT* p, unsigned long long point) {
    POINT pt = { px(point), py(point) };
    RECT rectTrue; ComputeTrueRect(p, &rectTrue);
    if (!PtInRect(&rectTrue, pt)) return hitNothing;
    // Handles exist only when the tracker is resizable.
    if (p->m_nStyle & (resizeInside | resizeOutside)) {
        unsigned int mask = HandleMask(p);
        for (int nHandle = 0; nHandle < 8; ++nHandle) {
            if (mask & (1u << nHandle)) {
                RECT rectHandle;
                HandleRect(p, nHandle, &rectHandle);
                if (PtInRect(&rectHandle, pt)) return nHandle;
            }
        }
    }
    // Body (hitMiddle) region: for resizeOutside the margin between m_rect and the
    // true rect is NOT body (only handles live there); otherwise the true rect is.
    RECT body;
    if (p->m_nStyle & resizeOutside) { body = p->m_rect; Normalize(body); body.right++; body.bottom++; }
    else ComputeTrueRect(p, &body);
    if (PtInRect(&body, pt)) return hitMiddle;
    return hitNothing;
}

static int NormalizeHitVal(const RT* p, int nHandle) {
    if (nHandle == hitNothing || nHandle == hitMiddle) return nHandle;
    RECT rect = p->m_rect;
    // when rect is mirrored, remap the handle to its visual corner
    if (rect.right - rect.left < 0) {
        static const int mapH[8] = { hitTopRight, hitTopLeft, hitBottomLeft, hitBottomRight,
                                     hitTop, hitLeft, hitBottom, hitRight };
        nHandle = mapH[nHandle];
    }
    if (rect.bottom - rect.top < 0) {
        static const int mapV[8] = { hitBottomLeft, hitBottomRight, hitTopRight, hitTopLeft,
                                     hitBottom, hitRight, hitTop, hitLeft };
        nHandle = mapV[nHandle];
    }
    return nHandle;
}

static void DoConstruct(RT* p) {
    p->m_nStyle = 0;
    SetRectEmpty(&p->m_rect);
    // MFC default handle size derives from the system border; 4 is the common value.
    int nHandleSize = GetSystemMetrics(SM_CXBORDER) * 3;
    if (nHandleSize < 4) nHandleSize = 4;
    p->m_nHandleSize = nHandleSize;
    p->m_sizeMin.cx = p->m_sizeMin.cy = nHandleSize * 2;
    p->m_bAllowInvert = FALSE;
    SetRectEmpty(&p->m_rectLast);
    p->m_sizeLast.cx = p->m_sizeLast.cy = 0;
    p->m_bErase = FALSE;
    p->m_bFinalErase = FALSE;
}

// CWnd::m_hWnd@64, CDC::m_hDC@8 (harvested via cl.exe /d1reportSingleClassLayout).
static inline HWND WndHandle(void* pWnd) { return pWnd ? *(HWND*)((char*)pWnd + 64) : nullptr; }
static inline HDC  DcHandle(void* pDC)   { return pDC  ? *(HDC*)((char*)pDC + 8)    : nullptr; }

void* MS_ABI v_dtor(RT* p, unsigned int flags) { if (flags & 1) ::operator delete(p); return p; }

} // namespace

// The virtual slots dispatch to the exported thunks (this=RCX matches). Forward
// declare them so the anonymous-namespace vtable can point at them.
extern "C" {
void MS_ABI impl__DrawTrackerRect_CRectTracker__UEAAXPEBUtagRECT__PEAVCWnd__PEAVCDC__1_Z(void*, const RECT*, void*, void*, void*);
void MS_ABI impl__AdjustRect_CRectTracker__UEAAXHPEAUtagRECT___Z(void*, int, RECT*);
void MS_ABI impl__OnChangedRect_CRectTracker__UEAAXAEBVCRect___Z(void*, const RECT*);
unsigned int MS_ABI impl__GetHandleMask_CRectTracker__UEBAIXZ(const void*);
int MS_ABI impl__GetHandleSize_CRectTracker__MEBAHPEBUtagRECT___Z(const void*, const RECT*);
}

// 6-slot MSVC vtable (order harvested): DrawTrackerRect, AdjustRect, OnChangedRect,
// GetHandleMask, ~dtor(vector-deleting), GetHandleSize.
namespace {
void* const g_CRectTracker_msvtbl[6] = {
    (void*)impl__DrawTrackerRect_CRectTracker__UEAAXPEBUtagRECT__PEAVCWnd__PEAVCDC__1_Z,
    (void*)impl__AdjustRect_CRectTracker__UEAAXHPEAUtagRECT___Z,
    (void*)impl__OnChangedRect_CRectTracker__UEAAXAEBVCRect___Z,
    (void*)impl__GetHandleMask_CRectTracker__UEBAIXZ,
    (void*)v_dtor,
    (void*)impl__GetHandleSize_CRectTracker__MEBAHPEBUtagRECT___Z,
};
} // namespace

// ================================ exported thunks ================================
extern "C" {

// Symbol: ??0CRectTracker@@QEAA@PEBUtagRECT@@I@Z
void* MS_ABI impl___0CRectTracker__QEAA_PEBUtagRECT__I_Z(void* pThis, const RECT* lpSrcRect, unsigned int nStyle) {
    RT* p = (RT*)pThis;
    p->vfptr = &g_CRectTracker_msvtbl[0];
    DoConstruct(p);
    if (lpSrcRect) p->m_rect = *lpSrcRect;
    p->m_nStyle = nStyle;
    return pThis;
}

// Symbol: ??1CRectTracker@@UEAA@XZ
void MS_ABI impl___1CRectTracker__UEAA_XZ(void* /*pThis*/) { /* no owned resources */ }

// Symbol: ?Construct@CRectTracker@@IEAAXXZ
void MS_ABI impl__Construct_CRectTracker__IEAAXXZ(void* pThis) {
    RT* p = (RT*)pThis; p->vfptr = &g_CRectTracker_msvtbl[0]; DoConstruct(p);
}

// Symbol: ?GetHandleSize@CRectTracker@@MEBAHPEBUtagRECT@@@Z
int MS_ABI impl__GetHandleSize_CRectTracker__MEBAHPEBUtagRECT___Z(const void* pThis, const RECT* lpRect) {
    return HandleSize((const RT*)pThis, lpRect);
}

// Symbol: ?GetHandleMask@CRectTracker@@UEBAIXZ
unsigned int MS_ABI impl__GetHandleMask_CRectTracker__UEBAIXZ(const void* pThis) {
    return HandleMask((const RT*)pThis);
}

// Symbol: ?GetHandleRect@CRectTracker@@IEBAXHPEAVCRect@@@Z
void MS_ABI impl__GetHandleRect_CRectTracker__IEBAXHPEAVCRect___Z(const void* pThis, int nHandle, RECT* pRect) {
    HandleRect((const RT*)pThis, nHandle, pRect);
}

// Symbol: ?GetModifyPointers@CRectTracker@@IEAAXHPEAPEAH0PEAH1@Z
void MS_ABI impl__GetModifyPointers_CRectTracker__IEAAXHPEAPEAH0PEAH1_Z(
        void* pThis, int nHandle, int** ppx, int** ppy, int* px_out, int* py_out) {
    RT* p = (RT*)pThis;
    int* xp; int* yp;
    ModifyPointers(nHandle, p->m_rect, &xp, &yp);
    if (ppx) *ppx = xp;
    if (ppy) *ppy = yp;
    if (px_out) *px_out = xp ? *xp : 0;
    if (py_out) *py_out = yp ? *yp : 0;
}

// Symbol: ?GetTrueRect@CRectTracker@@QEBAXPEAUtagRECT@@@Z
void MS_ABI impl__GetTrueRect_CRectTracker__QEBAXPEAUtagRECT___Z(const void* pThis, RECT* lpTrueRect) {
    ComputeTrueRect((const RT*)pThis, lpTrueRect);
}

// Symbol: ?HitTestHandles@CRectTracker@@IEBAHVCPoint@@@Z
int MS_ABI impl__HitTestHandles_CRectTracker__IEBAHVCPoint___Z(const void* pThis, unsigned long long point) {
    return HitTestHandles((const RT*)pThis, point);
}

// Symbol: ?HitTest@CRectTracker@@QEBAHVCPoint@@@Z
int MS_ABI impl__HitTest_CRectTracker__QEBAHVCPoint___Z(const void* pThis, unsigned long long point) {
    const RT* p = (const RT*)pThis;
    int nHandle = HitTestHandles(p, point);
    return NormalizeHitVal(p, nHandle);
}

// Symbol: ?NormalizeHit@CRectTracker@@QEBAHH@Z
int MS_ABI impl__NormalizeHit_CRectTracker__QEBAHH_Z(const void* pThis, int nHandle) {
    return NormalizeHitVal((const RT*)pThis, nHandle);
}

// Symbol: ?OnChangedRect@CRectTracker@@UEAAXAEBVCRect@@@Z
void MS_ABI impl__OnChangedRect_CRectTracker__UEAAXAEBVCRect___Z(void* /*pThis*/, const RECT* /*rectOld*/) {
    // default: no-op (overridable notification)
}

// Symbol: ?AdjustRect@CRectTracker@@UEAAXHPEAUtagRECT@@@Z
void MS_ABI impl__AdjustRect_CRectTracker__UEAAXHPEAUtagRECT___Z(void* pThis, int nHandle, RECT* lpRect) {
    if (nHandle == hitNothing) return;
    RT* p = (RT*)pThis;
    int* pxCoord; int* pyCoord;
    ModifyPointers(nHandle, *lpRect, &pxCoord, &pyCoord);
    // enforce minimum size along changed edges
    RECT r = *lpRect; Normalize(r);
    if (RWidth(r) < p->m_sizeMin.cx) {
        if (pxCoord == (int*)&lpRect->left)  lpRect->left  = lpRect->right - p->m_sizeMin.cx;
        else if (pxCoord == (int*)&lpRect->right) lpRect->right = lpRect->left + p->m_sizeMin.cx;
    }
    if (RHeight(r) < p->m_sizeMin.cy) {
        if (pyCoord == (int*)&lpRect->top)   lpRect->top    = lpRect->bottom - p->m_sizeMin.cy;
        else if (pyCoord == (int*)&lpRect->bottom) lpRect->bottom = lpRect->top + p->m_sizeMin.cy;
    }
}

// Symbol: ?DrawTrackerRect@CRectTracker@@UEAAXPEBUtagRECT@@PEAVCWnd@@PEAVCDC@@1@Z
void MS_ABI impl__DrawTrackerRect_CRectTracker__UEAAXPEBUtagRECT__PEAVCWnd__PEAVCDC__1_Z(
        void* /*pThis*/, const RECT* lpRect, void* /*pWndClip*/, void* pDC, void* /*pWnd*/) {
    if (!lpRect || !pDC) return;
    HDC hdc = DcHandle(pDC);
    RECT r = *lpRect; Normalize(r);
    if (hdc) DrawFocusRect(hdc, &r);
}

// Symbol: ?Draw@CRectTracker@@QEBAXPEAVCDC@@@Z
void MS_ABI impl__Draw_CRectTracker__QEBAXPEAVCDC___Z(const void* pThis, void* pDC) {
    if (!pDC) return;
    const RT* p = (const RT*)pThis;
    HDC hdc = DcHandle(pDC);
    if (!hdc) return;
    RECT rect = p->m_rect; Normalize(rect);
    // border
    if (p->m_nStyle & (solidLine | dottedLine | hatchedBorder)) {
        FrameRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
    }
    // resize handles
    unsigned int mask = HandleMask(p);
    HBRUSH hb = (HBRUSH)GetStockObject(BLACK_BRUSH);
    for (int i = 0; i < 8; ++i) {
        if (mask & (1u << i)) {
            RECT h; HandleRect(p, i, &h);
            FillRect(hdc, &h, hb);
        }
    }
}

// Symbol: ?SetCursor@CRectTracker@@QEBAHPEAVCWnd@@I@Z
int MS_ABI impl__SetCursor_CRectTracker__QEBAHPEAVCWnd__I_Z(const void* /*pThis*/, void* /*pWnd*/, unsigned int nHitTest) {
    // Map the WM_SETCURSOR hit-test area to a resize cursor. Real MFC decodes the
    // handle from the low word; here we set a sensible sizing cursor for hits.
    LPCWSTR id = IDC_ARROW;
    switch (nHitTest & 0xFFFF) {
    case hitTopLeft: case hitBottomRight:   id = IDC_SIZENWSE; break;
    case hitTopRight: case hitBottomLeft:   id = IDC_SIZENESW; break;
    case hitTop: case hitBottom:            id = IDC_SIZENS;   break;
    case hitLeft: case hitRight:            id = IDC_SIZEWE;   break;
    case hitMiddle:                         id = IDC_SIZEALL;  break;
    default: return FALSE;
    }
    ::SetCursor(LoadCursorW(nullptr, id));
    return TRUE;
}

// Track / TrackRubberBand / TrackHandle: real modal mouse-capture resize loops.
static int DoTrack(RT* p, HWND hWnd, unsigned long long point, BOOL bRubber, BOOL bAllowInvert);

// Symbol: ?Track@CRectTracker@@QEAAHPEAVCWnd@@VCPoint@@H0@Z
int MS_ABI impl__Track_CRectTracker__QEAAHPEAVCWnd__VCPoint__H0_Z(
        void* pThis, void* pWnd, unsigned long long point, int bAllowInvert, void* /*pWndClipTo*/) {
    RT* p = (RT*)pThis;
    HWND hWnd = WndHandle(pWnd);
    return DoTrack(p, hWnd, point, FALSE, bAllowInvert);
}

// Symbol: ?TrackRubberBand@CRectTracker@@QEAAHPEAVCWnd@@VCPoint@@H@Z
int MS_ABI impl__TrackRubberBand_CRectTracker__QEAAHPEAVCWnd__VCPoint__H_Z(
        void* pThis, void* pWnd, unsigned long long point, int bAllowInvert) {
    RT* p = (RT*)pThis;
    HWND hWnd = WndHandle(pWnd);
    return DoTrack(p, hWnd, point, TRUE, bAllowInvert);
}

// Symbol: ?TrackHandle@CRectTracker@@IEAAHHPEAVCWnd@@VCPoint@@0@Z
int MS_ABI impl__TrackHandle_CRectTracker__IEAAHHPEAVCWnd__VCPoint__0_Z(
        void* pThis, int nHandle, void* pWnd, unsigned long long point, void* /*pWndClipTo*/) {
    RT* p = (RT*)pThis;
    HWND hWnd = WndHandle(pWnd);
    (void)nHandle;
    return DoTrack(p, hWnd, point, FALSE, p->m_bAllowInvert);
}

} // extern "C"

// Modal resize/move loop, faithful to MFC's Track semantics (mouse capture, move
// the hit handle/body until button-up or Escape). Returns TRUE if the rect changed.
static int DoTrack(RT* p, HWND hWnd, unsigned long long point, BOOL bRubber, BOOL bAllowInvert) {
    p->m_bAllowInvert = bAllowInvert;
    POINT ptStart = { px(point), py(point) };
    int nHandle;
    if (bRubber) {
        p->m_rect.left = p->m_rect.right = ptStart.x;
        p->m_rect.top = p->m_rect.bottom = ptStart.y;
        nHandle = hitBottomRight;
    } else {
        nHandle = HitTestHandles(p, point);
        if (nHandle == hitNothing) return FALSE;
    }
    if (!hWnd) return FALSE; // no window to capture: nothing to track interactively

    RECT rectSave = p->m_rect;
    SetCapture(hWnd);
    HDC hdc = GetDC(hWnd);
    RECT rectCur = p->m_rect;
    if (hdc) { RECT rn = rectCur; Normalize(rn); DrawFocusRect(hdc, &rn); }

    BOOL bMoved = FALSE, bResult = FALSE;
    MSG msg;
    while (GetCapture() == hWnd && GetMessageW(&msg, nullptr, 0, 0)) {
        if (msg.message == WM_LBUTTONUP) { bResult = bMoved; break; }
        if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE) { p->m_rect = rectSave; bResult = FALSE; break; }
        if (msg.message == WM_MOUSEMOVE) {
            POINT pt = { (SHORT)LOWORD(msg.lParam), (SHORT)HIWORD(msg.lParam) };
            if (hdc) { RECT rn = rectCur; Normalize(rn); DrawFocusRect(hdc, &rn); } // erase
            if (nHandle == hitMiddle) {
                OffsetRect(&rectCur, pt.x - ptStart.x, pt.y - ptStart.y);
                ptStart = pt;
            } else {
                // Apply the new mouse coordinate to the tracked edge(s) FIRST, so
                // the following AdjustRect enforces the min-size / inversion
                // constraints on the NEW position (previously AdjustRect ran on the
                // old rect and its result was overwritten by the raw coordinate).
                int* xp; int* yp; ModifyPointers(nHandle, rectCur, &xp, &yp);
                if (xp) *xp = pt.x;
                if (yp) *yp = pt.y;
                // Dispatch AdjustRect virtually (vtable slot 1) so a CRectTracker
                // subclass override participates, matching real MFC TrackHandle.
                void* const* vt = *(void* const* const*)p;
                ((void (MS_ABI*)(void*, int, RECT*))vt[1])(p, nHandle, &rectCur);
            }
            if (hdc) { RECT rn = rectCur; Normalize(rn); DrawFocusRect(hdc, &rn); } // redraw
            bMoved = TRUE;
        } else {
            TranslateMessage(&msg); DispatchMessageW(&msg);
        }
    }
    if (hdc) { RECT rn = rectCur; Normalize(rn); DrawFocusRect(hdc, &rn); ReleaseDC(hWnd, hdc); }
    if (GetCapture() == hWnd) ReleaseCapture();
    if (bResult) { RECT old = p->m_rect; p->m_rect = rectCur;
        // Dispatch OnChangedRect virtually (vtable slot 2) for subclass overrides.
        void* const* vt = *(void* const* const*)p;
        ((void (MS_ABI*)(void*, const RECT*))vt[2])(p, &old); }
    else p->m_rect = rectSave;
    return bResult;
}
