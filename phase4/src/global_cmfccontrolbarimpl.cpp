// OpenMFC real implementation of CMFCControlBarImpl.
//
// Layout (harvested via cl.exe /d1reportSingleClassLayout):
//   class CMFCControlBarImpl  size(16):
//     0  | {vfptr}
//     8  | m_pBar        (CPane*)
//
// Polymorphic: single vtable slot = virtual destructor (vector deleting dtor).
//
// CMFCControlBarImpl is the helper that CMFCToolBar / CMFCReBar delegate to for
// non-client (gripper) sizing and painting. All observable behaviour flows
// through its owning CPane (m_pBar), whose HWND lives at CWnd::m_hWnd (offset 64).

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

#include <cstddef>

namespace {

// Layout-faithful view of CMFCControlBarImpl.
struct S {
    void* vfptr;   // 0
    void* m_pBar;  // 8  -> CPane*
};
static_assert(sizeof(S) == 16, "CMFCControlBarImpl must be 16 bytes");
static_assert(offsetof(S, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S, m_pBar) == 8, "m_pBar at 8");

// CWnd::m_hWnd lives at offset 64 inside any CWnd-derived object (CPane derives
// from CBasePane -> CPane -> ... -> CWnd). We read it to reach the real window.
static HWND BarHwnd(void* pBar) {
    if (!pBar) return nullptr;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pBar) + 64);
}

// Default gripper thickness used by the MFC docking/rebar non-client frame.
static const int kGripperSize = 12;

} // namespace

// -------------------------------------------------------------------------
// Constructor: CMFCControlBarImpl::CMFCControlBarImpl(CPane* pBar)
// -------------------------------------------------------------------------
// Symbol: ??0CMFCControlBarImpl@@QEAA@PEAVCPane@@@Z
extern "C" void* MS_ABI impl___0CMFCControlBarImpl__QEAA_PEAVCPane___Z(void* pThis, void* pBar);

// -------------------------------------------------------------------------
// Destructor: CMFCControlBarImpl::~CMFCControlBarImpl()   (virtual)
// -------------------------------------------------------------------------
// Symbol: ??1CMFCControlBarImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlBarImpl__UEAA_XZ(void* pThis);

// -------------------------------------------------------------------------
// vtable  (slot 0 = vector deleting destructor)
// -------------------------------------------------------------------------
namespace {
void* MS_ABI vdtor(void* p, unsigned flags) {
    impl___1CMFCControlBarImpl__UEAA_XZ(p);
    if (flags & 1) ::operator delete(p);
    return p;
}
void* const g_CMFCControlBarImpl_vtbl[1] = {
    reinterpret_cast<void*>(&vdtor),
};
} // namespace

// Symbol: ??0CMFCControlBarImpl@@QEAA@PEAVCPane@@@Z
extern "C" void* MS_ABI impl___0CMFCControlBarImpl__QEAA_PEAVCPane___Z(void* pThis, void* pBar) {
    S* self = reinterpret_cast<S*>(pThis);
    self->vfptr  = reinterpret_cast<void*>(const_cast<void**>(&g_CMFCControlBarImpl_vtbl[0]));
    self->m_pBar = pBar;
    return pThis;
}

// Symbol: ??1CMFCControlBarImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlBarImpl__UEAA_XZ(void* pThis) {
    // Owns no heap resources; m_pBar is a back-pointer, not owned.
    S* self = reinterpret_cast<S*>(pThis);
    self->m_pBar = nullptr;
}

// -------------------------------------------------------------------------
// void CMFCControlBarImpl::GetGripperRect(CRect& rectGripper, BOOL bClientCoord)
// -------------------------------------------------------------------------
// Computes the rectangle occupied by the drag gripper. For a horizontally laid
// out bar the gripper is a thin strip down the left edge; for a vertically laid
// out bar it is a strip along the top edge. Coordinates are returned in client
// space of the owning bar (default) or in the bar's window space.
// Symbol: ?GetGripperRect@CMFCControlBarImpl@@QEAAXAEAVCRect@@H@Z
extern "C" void MS_ABI impl__GetGripperRect_CMFCControlBarImpl__QEAAXAEAVCRect__H_Z(
        void* pThis, RECT* rectGripper, int bClientCoord) {
    S* self = reinterpret_cast<S*>(pThis);
    if (!rectGripper) return;

    RECT rc = {0, 0, 0, 0};
    HWND h = BarHwnd(self->m_pBar);
    if (h) {
        ::GetClientRect(h, &rc);
    }

    // Horizontal bar: wider than tall -> gripper is a left-edge column.
    // Vertical bar: taller than wide -> gripper is a top-edge row.
    const bool horizontal = (rc.right - rc.left) >= (rc.bottom - rc.top);

    RECT g = rc;
    if (horizontal) {
        g.right = g.left + kGripperSize;
    } else {
        g.bottom = g.top + kGripperSize;
    }

    if (!bClientCoord && h) {
        // Translate the gripper (client-relative) into window coordinates by
        // adding the non-client offset of the client area.
        RECT wr = {0, 0, 0, 0};
        POINT cliOrg = {0, 0};
        ::GetWindowRect(h, &wr);
        ::ClientToScreen(h, &cliOrg);
        const int dx = cliOrg.x - wr.left;
        const int dy = cliOrg.y - wr.top;
        ::OffsetRect(&g, dx, dy);
    }

    *rectGripper = g;
}

// -------------------------------------------------------------------------
// void CMFCControlBarImpl::CalcNcSize(NCCALCSIZE_PARAMS* lpncsp)
// -------------------------------------------------------------------------
// Reserves the gripper strip out of the client area during WM_NCCALCSIZE.
// rgrc[0] is the proposed client rect; we shrink it by the gripper thickness on
// the appropriate edge, matching MFC's non-client frame accounting.
// Symbol: ?CalcNcSize@CMFCControlBarImpl@@IEAAXPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__CalcNcSize_CMFCControlBarImpl__IEAAXPEAUtagNCCALCSIZE_PARAMS___Z(
        void* pThis, NCCALCSIZE_PARAMS* lpncsp) {
    (void)pThis;
    if (!lpncsp) return;

    RECT& client = lpncsp->rgrc[0];
    const bool horizontal = (client.right - client.left) >= (client.bottom - client.top);
    if (horizontal) {
        client.left += kGripperSize;
        if (client.left > client.right) client.left = client.right;
    } else {
        client.top += kGripperSize;
        if (client.top > client.bottom) client.top = client.bottom;
    }
}

// -------------------------------------------------------------------------
// void CMFCControlBarImpl::DrawNcArea()
// -------------------------------------------------------------------------
// Paints the non-client gripper area. Acquires a window DC for the owning bar,
// computes the gripper rectangle in window coordinates and fills it with the
// 3D-face brush as a lightweight faithful rendering of the gripper background.
// The detailed gripper glyph painting is delegated to the visual manager in the
// full framework; here we render the background band so the area is valid.
// Symbol: ?DrawNcArea@CMFCControlBarImpl@@IEAAXXZ
extern "C" void MS_ABI impl__DrawNcArea_CMFCControlBarImpl__IEAAXXZ(void* pThis) {
    S* self = reinterpret_cast<S*>(pThis);
    HWND h = BarHwnd(self->m_pBar);
    if (!h) return;

    HDC hdc = ::GetWindowDC(h);
    if (!hdc) return;

    RECT g;
    impl__GetGripperRect_CMFCControlBarImpl__QEAAXAEAVCRect__H_Z(pThis, &g, FALSE);
    HBRUSH br = ::GetSysColorBrush(COLOR_3DFACE);
    if (br) {
        ::FillRect(hdc, &g, br);
    }
    ::ReleaseDC(h, hdc);
}

// -------------------------------------------------------------------------
// BOOL CMFCControlBarImpl::GetBackgroundFromParent(CDC* pDC)
// -------------------------------------------------------------------------
// Attempts to paint the bar's background by asking the parent window to render
// its own background into pDC (WM_ERASEBKGND against the parent), which is how
// MFC lets rebar/toolbar children inherit a themed parent background. Returns
// TRUE if a parent exists and the erase was forwarded, FALSE otherwise.
// CDC::m_hDC lives at offset 8 inside CDC.
// Symbol: ?GetBackgroundFromParent@CMFCControlBarImpl@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__GetBackgroundFromParent_CMFCControlBarImpl__IEAAHPEAVCDC___Z(
        void* pThis, void* pDC) {
    S* self = reinterpret_cast<S*>(pThis);
    if (!pDC) return FALSE;

    HWND h = BarHwnd(self->m_pBar);
    if (!h) return FALSE;
    HWND hParent = ::GetParent(h);
    if (!hParent) return FALSE;

    HDC hdc = *reinterpret_cast<HDC*>(reinterpret_cast<char*>(pDC) + 8);
    if (!hdc) return FALSE;

    // Align the DC origin so the parent paints the region behind this bar, then
    // forward WM_ERASEBKGND to the parent, mirroring MFC's inherited-background
    // technique. Restore the origin afterwards.
    RECT rcBar = {0, 0, 0, 0};
    ::GetWindowRect(h, &rcBar);
    POINT ptParent = {rcBar.left, rcBar.top};
    ::ScreenToClient(hParent, &ptParent);

    POINT oldOrg = {0, 0};
    ::SetWindowOrgEx(hdc, ptParent.x, ptParent.y, &oldOrg);
    LRESULT lr = ::SendMessage(hParent, WM_ERASEBKGND, reinterpret_cast<WPARAM>(hdc), 0);
    ::SetWindowOrgEx(hdc, oldOrg.x, oldOrg.y, nullptr);

    return lr ? TRUE : FALSE;
}
