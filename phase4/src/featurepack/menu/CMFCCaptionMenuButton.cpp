// CMFCCaptionMenuButton — OpenMFC implementation.
// Sources: global_cmfccaptionmenubutton.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCCaptionMenuButtonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CMFCCaptionMenuButton@@QEAA@IH@Z
extern "C" void* MS_ABI
impl___0CMFCCaptionMenuButton__QEAA_IH_Z(void* pThis, unsigned nHit, int bLeftAlign) {
    S_Cmfccaptionmenubutton* self = (S_Cmfccaptionmenubutton*)pThis;
    InitDefaults(self);
    self->m_nHit       = nHit;
    self->m_bLeftAlign = bLeftAlign;
    return pThis;
}
// Symbol: ??0CMFCCaptionMenuButton@@QEAA@XZ
extern "C" void* MS_ABI
impl___0CMFCCaptionMenuButton__QEAA_XZ(void* pThis) {
    InitDefaults((S_Cmfccaptionmenubutton*)pThis);
    return pThis;
}
// Symbol: ??1CMFCCaptionMenuButton@@UEAA@XZ
extern "C" void MS_ABI
impl___1CMFCCaptionMenuButton__UEAA_XZ(void* /*pThis*/) {
    // No owned resources (the tracked menu is owned by the caller).
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?OnDraw@CMFCCaptionMenuButton@@UEAAXPEAVCDC@@HHHH@Z
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?ShowMenu@CMFCCaptionMenuButton@@QEAAXPEAUHMENU__@@PEAVCWnd@@@Z
// Symbol: ?OnDraw@CMFCCaptionMenuButton@@UEAAXPEAVCDC@@HHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCaptionMenuButton__UEAAXPEAVCDC__HHHH_Z(
    void* pThis, void* pDC, int bActive, int /*bHorz*/, int /*bMaximized*/, int bDisabled) {
    auto* self = static_cast<CMFCCaptionMenuButtonLayout*>(pThis);
    HDC hdc = DCHandleForCDC(pDC);
    if (self == nullptr || hdc == nullptr) {
        return;
    }

    RECT rc = GetCaptionMenuRect(pThis);
    if (rc.right <= rc.left || rc.bottom <= rc.top) {
        return;
    }

    COLORREF bk = ::GetSysColor(self->m_bPushed ? COLOR_HIGHLIGHT
                              : (bActive ? COLOR_ACTIVECAPTION
                                         : COLOR_INACTIVECAPTION));
    HBRUSH hbr = ::CreateSolidBrush(bk);
    if (hbr) {
        ::FillRect(hdc, &rc, hbr);
        ::DeleteObject(hbr);
    }

    int cx = (rc.left + rc.right) / 2;
    int cy = (rc.top + rc.bottom) / 2;
    int r = 3;
    POINT tri[3] = {
        {cx - r, cy - 1},
        {cx + r, cy - 1},
        {cx, cy + r - 1},
    };
    COLORREF fg = ::GetSysColor(bDisabled ? COLOR_GRAYTEXT : COLOR_CAPTIONTEXT);
    HBRUSH fb = static_cast<HBRUSH>(::CreateSolidBrush(fg));
    HPEN fp = static_cast<HPEN>(::CreatePen(PS_SOLID, 1, fg));
    HGDIOBJ oldBrush = NULL;
    HGDIOBJ oldPen = NULL;
    if (fb) {
        oldBrush = ::SelectObject(hdc, fb);
    }
    if (fp) {
        oldPen = ::SelectObject(hdc, fp);
    }
    ::Polygon(hdc, tri, 3);
    if (oldBrush) {
        ::SelectObject(hdc, oldBrush);
        ::DeleteObject(fb);
    }
    if (oldPen) {
        ::SelectObject(hdc, oldPen);
        ::DeleteObject(fp);
    }
}
// Symbol: ?ShowMenu@CMFCCaptionMenuButton@@QEAAXPEAUHMENU__@@PEAVCWnd@@@Z
extern "C" void MS_ABI impl__ShowMenu_CMFCCaptionMenuButton__QEAAXPEAUHMENU____PEAVCWnd___Z(
    void* pThis, void* hMenu, void* pWindow) {
    auto* self = static_cast<CMFCCaptionMenuButtonLayout*>(pThis);
    HWND hWnd = WndHandleForObject(pWindow);

    if (self == nullptr) {
        return;
    }

    self->m_bPushed = TRUE;
    self->m_bMenuIsActive = TRUE;
    self->m_nMenuResult = 0;

    if (hMenu != nullptr) {
        HMENU hmenu = static_cast<HMENU>(hMenu);
        POINT pt {self->m_ptOrg.x, self->m_ptOrg.y};
        if (hWnd) {
            ::ClientToScreen(hWnd, &pt);
        }
        int cmd = ::TrackPopupMenu(
            hmenu,
            TPM_LEFTALIGN | TPM_TOPALIGN | TPM_LEFTBUTTON | TPM_RETURNCMD,
            pt.x, pt.y, 0, hWnd, NULL);
        self->m_nMenuResult = cmd;
        if (cmd != 0 && hWnd) {
            ::PostMessageW(hWnd, WM_COMMAND, static_cast<WPARAM>(cmd), 0);
        }
    }

    self->m_bMenuIsActive = FALSE;
    self->m_bPushed = FALSE;
}

// CMFCCaptionMenuButton's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
extern void* const g_CMFCCaptionMenuButton_vtbl[8] = {
    (void*)&slot_GetRuntimeClass_Cmfccaptionmenubutton,   // 0 CObject::GetRuntimeClass
    (void*)&slot_vecDelDtor_Cmfccaptionmenubutton,        // 1 CMFCCaptionMenuButton::{dtor}
    (void*)&slot_Serialize_Cmfccaptionmenubutton,         // 2 CObject::Serialize
    (void*)&slot_AssertValid_Cmfccaptionmenubutton,       // 3 CObject::AssertValid
    (void*)&slot_Dump_Cmfccaptionmenubutton,              // 4 CObject::Dump
    (void*)&slot_GetRect,           // 5 CMFCCaptionButton::GetRect
    (void*)&impl__OnDraw_CMFCCaptionMenuButton__UEAAXPEAVCDC__HHHH_Z, // 6 OnDraw
    (void*)&slot_GetIconID,         // 7 CMFCCaptionButton::GetIconID
};
} } }
