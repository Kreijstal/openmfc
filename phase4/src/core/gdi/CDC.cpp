// CDC — OpenMFC implementation.
// Sources: gdicore.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// CDC default constructor
// Symbol: ??0CDC@@QEAA@XZ
// Ordinal: 438
extern "C" CDC* MS_ABI impl___0CDC__QEAA_XZ(CDC* pThis) {
    if (!pThis) return nullptr;

    // Initialize members
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return pThis;
}
// CDC destructor
// Symbol: ??1CDC@@UEAA@XZ
// Ordinal: 1086
//
// NOTE: Base CDC destructor does NOT delete the DC. Derived classes (CClientDC, CPaintDC,
// CWindowDC) handle releasing their own DCs appropriately. If a raw CDC owns a DC (e.g.,
// from CreateCompatibleDC), the caller must explicitly call DeleteDC() before destruction.
// This matches real MFC behavior where CDC is often used as a wrapper for DCs it doesn't own.
extern "C" void MS_ABI impl___1CDC__UEAA_XZ(CDC* pThis) {
    if (pThis) {
        // Don't delete - caller is responsible for DC lifetime
        pThis->m_hDC = nullptr;
        pThis->m_hAttribDC = nullptr;
    }
}
// CDC::DeleteDC
// Symbol: ?DeleteDC@CDC@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteDC_CDC__QEAAHXZ(CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return FALSE;

    int result = ::DeleteDC(pThis->m_hDC);
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return result;
}
// CDC::SaveDC
// Symbol: ?SaveDC@CDC@@QEAAHXZ
extern "C" int MS_ABI impl__SaveDC_CDC__QEAAHXZ(CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SaveDC(pThis->m_hDC);
}
// CDC::RestoreDC
// Symbol: ?RestoreDC@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__RestoreDC_CDC__QEAAHH_Z(CDC* pThis, int nSavedDC) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::RestoreDC(pThis->m_hDC, nSavedDC);
}
// CDC::MoveTo (returns CPoint)
// Symbol: ?MoveTo@CDC@@QEAA?AVCPoint@@HH@Z
// Ordinal: 8501
extern "C" CPoint MS_ABI impl__MoveTo_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
    CPoint pt(0, 0);
    if (pThis && pThis->m_hDC) {
        POINT oldPt;
        ::MoveToEx(pThis->m_hDC, x, y, &oldPt);
        pt.x = oldPt.x;
        pt.y = oldPt.y;
    }
    return pt;
}
// CDC::LineTo
// Symbol: ?LineTo@CDC@@QEAAHHH@Z
// Ordinal: 8063
extern "C" int MS_ABI impl__LineTo_CDC__QEAAHHH_Z(CDC* pThis, int x, int y) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::LineTo(pThis->m_hDC, x, y);
}
// CDC::SetBkMode
// Symbol: ?SetBkMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetBkMode(pThis->m_hDC, nBkMode);
}
// CDC::PatBlt
// Symbol: ?PatBlt@CDC@@QEAAHHHHHK@Z
extern "C" int MS_ABI impl__PatBlt_CDC__QEAAHHHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::PatBlt(pThis->m_hDC, x, y, nWidth, nHeight, dwRop);
}
// CDC::SelectClipRgn
// Symbol: ?SelectClipRgn@CDC@@QEAAHPEAVCRgn@@@Z
extern "C" int MS_ABI impl__SelectClipRgn_CDC__QEAAHPEAVCRgn___Z(CDC* pThis, CRgn* pRgn) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    HRGN hRgn = pRgn ? (HRGN)pRgn->GetSafeHandle() : nullptr;
    return ::SelectClipRgn(pThis->m_hDC, hRgn);
}
// CDC::SelectClipRgn (mode)
// Symbol: ?SelectClipRgn@CDC@@QEAAHPEAVCRgn@@H@Z
extern "C" int MS_ABI impl__SelectClipRgn_CDC__QEAAHPEAVCRgn__H_Z(CDC* pThis, CRgn* pRgn, int nMode) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    HRGN hRgn = pRgn ? (HRGN)pRgn->GetSafeHandle() : nullptr;
    return ::ExtSelectClipRgn(pThis->m_hDC, hRgn, nMode);
}
// CDC::ExcludeClipRect
// Symbol: ?ExcludeClipRect@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__ExcludeClipRect_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    return ::ExcludeClipRect(pThis->m_hDC, x1, y1, x2, y2);
}
// CDC::ExcludeClipRect (RECT)
// Symbol: ?ExcludeClipRect@CDC@@QEAAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__ExcludeClipRect_CDC__QEAAHPEBUtagRECT___Z(CDC* pThis, const RECT* lpRect) {
    if (!pThis || !pThis->m_hDC || !lpRect) return ERROR;
    return ::ExcludeClipRect(pThis->m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
// CDC::IntersectClipRect
// Symbol: ?IntersectClipRect@CDC@@QEAAHHHHH@Z
extern "C" int MS_ABI impl__IntersectClipRect_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    return ::IntersectClipRect(pThis->m_hDC, x1, y1, x2, y2);
}
// CDC::IntersectClipRect (RECT)
// Symbol: ?IntersectClipRect@CDC@@QEAAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__IntersectClipRect_CDC__QEAAHPEBUtagRECT___Z(CDC* pThis, const RECT* lpRect) {
    if (!pThis || !pThis->m_hDC || !lpRect) return ERROR;
    return ::IntersectClipRect(pThis->m_hDC, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
// CDC::OffsetClipRgn
// Symbol: ?OffsetClipRgn@CDC@@QEAAHHH@Z
extern "C" int MS_ABI impl__OffsetClipRgn_CDC__QEAAHHH_Z(CDC* pThis, int x, int y) {
    if (!pThis || !pThis->m_hDC) return ERROR;
    return ::OffsetClipRgn(pThis->m_hDC, x, y);
}
// CDC::OffsetClipRgn (SIZE)
// Symbol: ?OffsetClipRgn@CDC@@QEAAHUtagSIZE@@@Z
extern "C" int MS_ABI impl__OffsetClipRgn_CDC__QEAAHUtagSIZE___Z(CDC* pThis, SIZE size) {
    return impl__OffsetClipRgn_CDC__QEAAHHH_Z(pThis, size.cx, size.cy);
}
// CDC::FillSolidRect
// Symbol: ?FillSolidRect@CDC@@QEAAXHHHHK@Z
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXHHHHK_Z(
    CDC* pThis, int x, int y, int cx, int cy, unsigned long clr) {
    if (!pThis || !pThis->m_hDC) return;
    RECT rc = {x, y, x + cx, y + cy};
    ::SetBkColor(pThis->m_hDC, clr);
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rc, nullptr, 0, nullptr);
}
// CDC::FillSolidRect (RECT)
// Symbol: ?FillSolidRect@CDC@@QEAAXPEBUtagRECT@@K@Z
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clr) {
    if (!lpRect) return;
    impl__FillSolidRect_CDC__QEAAXHHHHK_Z(
        pThis,
        lpRect->left,
        lpRect->top,
        lpRect->right - lpRect->left,
        lpRect->bottom - lpRect->top,
        clr);
}
// CDC::Attach
// Symbol: ?Attach@CDC@@QEAAHPEAUHDC__@@@Z
extern "C" int MS_ABI impl__Attach_CDC__QEAAHPEAUHDC_____Z(CDC* pThis, HDC hDC) {
    if (!pThis) return FALSE;
    pThis->m_hDC = hDC;
    pThis->m_hAttribDC = hDC;
    return TRUE;
}
// CDC::Detach
// Symbol: ?Detach@CDC@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__Detach_CDC__QEAAPEAUHDC____XZ(CDC* pThis) {
    if (!pThis) return nullptr;
    HDC hDC = pThis->m_hDC;
    pThis->m_hDC = nullptr;
    pThis->m_hAttribDC = nullptr;
    return hDC;
}
// CDC::FromHandle
// Symbol: ?FromHandle@CDC@@SAPEAV1@PEAUHDC__@@@Z
extern "C" CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC) {
    return GetTempDC(hDC);
}
// CDC::DeleteTempMap
// Symbol: ?DeleteTempMap@CDC@@SAXXZ
extern "C" void MS_ABI impl__DeleteTempMap_CDC__SAXXZ() {
    DeleteTempDCMap();
}
// CDC::DPtoLP (SIZE)
// Symbol: ?DPtoLP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__DPtoLP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    POINT pt = {lpSize->cx, lpSize->cy};
    ::DPtoLP(pThis->m_hDC, &pt, 1);
    lpSize->cx = pt.x;
    lpSize->cy = pt.y;
}
// CDC::LPtoDP (SIZE)
// Symbol: ?LPtoDP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__LPtoDP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    POINT pt = {lpSize->cx, lpSize->cy};
    ::LPtoDP(pThis->m_hDC, &pt, 1);
    lpSize->cx = pt.x;
    lpSize->cy = pt.y;
}
// CDC::SaveDC (virtual override, UEAA)
// Symbol: ?SaveDC@CDC@@UEAAHXZ
extern "C" int MS_ABI impl__SaveDC_CDC__UEAAHXZ(CDC* pThis) {
    return impl__SaveDC_CDC__QEAAHXZ(pThis);
}
// CDC::RestoreDC (virtual override, UEAA)
// Symbol: ?RestoreDC@CDC@@UEAAHH@Z
extern "C" int MS_ABI impl__RestoreDC_CDC__UEAAHH_Z(CDC* pThis, int nSavedDC) {
    return impl__RestoreDC_CDC__QEAAHH_Z(pThis, nSavedDC);
}
// CDC::SetBkColor (virtual override, UEAA)
// Symbol: ?SetBkColor@CDC@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor) {
    return impl__SetBkColor_CDC__QEAAKK_Z(pThis, crColor);
}
// CDC::SetTextColor (virtual override, UEAA)
// Symbol: ?SetTextColor@CDC@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor) {
    return impl__SetTextColor_CDC__QEAAKK_Z(pThis, crColor);
}
// CDC::SetMapMode (virtual, UEAA)
// Symbol: ?SetMapMode@CDC@@UEAAHH@Z
extern "C" int MS_ABI impl__SetMapMode_CDC__UEAAHH_Z(CDC* pThis, int nMapMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetMapMode(pThis->m_hDC, nMapMode);
}
// CDC::SetViewportOrg (virtual, UEAA)
// Symbol: ?SetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetViewportOrgEx(pThis->m_hDC, &old);
    ::SetViewportOrgEx(pThis->m_hDC, x, y, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}
// CDC::SetViewportExt (virtual, UEAA)
// Symbol: ?SetViewportExt@CDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__SetViewportExt_CDC__UEAA_AVCSize__HH_Z(CDC* pThis, int cx, int cy) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetViewportExtEx(pThis->m_hDC, &old);
    ::SetViewportExtEx(pThis->m_hDC, cx, cy, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}
// CDC::ScaleViewportExt (virtual, UEAA)
// Symbol: ?ScaleViewportExt@CDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize MS_ABI impl__ScaleViewportExt_CDC__UEAA_AVCSize__HHHH_Z(
    CDC* pThis, int xNum, int xDenom, int yNum, int yDenom) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetViewportExtEx(pThis->m_hDC, &old);
    ::ScaleViewportExtEx(pThis->m_hDC, xNum, xDenom, yNum, yDenom, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}
// CDC::SetWindowExt (virtual, UEAA)
// Symbol: ?SetWindowExt@CDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize MS_ABI impl__SetWindowExt_CDC__UEAA_AVCSize__HH_Z(CDC* pThis, int cx, int cy) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetWindowExtEx(pThis->m_hDC, &old);
    ::SetWindowExtEx(pThis->m_hDC, cx, cy, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}
// CDC::SetWindowOrg
// Symbol: ?SetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__SetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int x, int y) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetWindowOrgEx(pThis->m_hDC, &old);
    ::SetWindowOrgEx(pThis->m_hDC, x, y, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}
// CDC::ScaleWindowExt (virtual, UEAA)
// Symbol: ?ScaleWindowExt@CDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize MS_ABI impl__ScaleWindowExt_CDC__UEAA_AVCSize__HHHH_Z(
    CDC* pThis, int xNum, int xDenom, int yNum, int yDenom) {
    CSize sz(0, 0);
    if (!pThis || !pThis->m_hDC) return sz;
    SIZE old;
    ::GetWindowExtEx(pThis->m_hDC, &old);
    ::ScaleWindowExtEx(pThis->m_hDC, xNum, xDenom, yNum, yDenom, nullptr);
    sz.cx = old.cx;
    sz.cy = old.cy;
    return sz;
}
// CDC::OffsetViewportOrg (virtual, UEAA)
// Symbol: ?OffsetViewportOrg@CDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__OffsetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC* pThis, int nWidth, int nHeight) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetViewportOrgEx(pThis->m_hDC, &old);
    ::OffsetViewportOrgEx(pThis->m_hDC, nWidth, nHeight, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}
// CDC::OffsetWindowOrg
// Symbol: ?OffsetWindowOrg@CDC@@QEAA?AVCPoint@@HH@Z
extern "C" CPoint MS_ABI impl__OffsetWindowOrg_CDC__QEAA_AVCPoint__HH_Z(CDC* pThis, int nWidth, int nHeight) {
    CPoint pt(0, 0);
    if (!pThis || !pThis->m_hDC) return pt;
    POINT old;
    ::GetWindowOrgEx(pThis->m_hDC, &old);
    ::OffsetWindowOrgEx(pThis->m_hDC, nWidth, nHeight, nullptr);
    pt.x = old.x;
    pt.y = old.y;
    return pt;
}
// CDC::SetPolyFillMode
// Symbol: ?SetPolyFillMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetPolyFillMode_CDC__QEAAHH_Z(CDC* pThis, int nPolyFillMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetPolyFillMode(pThis->m_hDC, nPolyFillMode);
}
// CDC::SetROP2
// Symbol: ?SetROP2@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetROP2_CDC__QEAAHH_Z(CDC* pThis, int nDrawMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetROP2(pThis->m_hDC, nDrawMode);
}
// CDC::SetStretchBltMode
// Symbol: ?SetStretchBltMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetStretchBltMode_CDC__QEAAHH_Z(CDC* pThis, int nStretchMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetStretchBltMode(pThis->m_hDC, nStretchMode);
}
// CDC::SetTextAlign
// Symbol: ?SetTextAlign@CDC@@QEAAII@Z
extern "C" unsigned int MS_ABI impl__SetTextAlign_CDC__QEAAII_Z(CDC* pThis, unsigned int nFlags) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::SetTextAlign(pThis->m_hDC, nFlags);
}
// CDC::SetTextCharacterExtra
// Symbol: ?SetTextCharacterExtra@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetTextCharacterExtra_CDC__QEAAHH_Z(CDC* pThis, int nCharExtra) {
    if (!pThis || !pThis->m_hDC) return 0x80000000;
    return ::SetTextCharacterExtra(pThis->m_hDC, nCharExtra);
}
// CDC::SetTextJustification
// Symbol: ?SetTextJustification@CDC@@QEAAHHH@Z
extern "C" int MS_ABI impl__SetTextJustification_CDC__QEAAHHH_Z(CDC* pThis, int nBreakExtra, int nBreakCount) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::SetTextJustification(pThis->m_hDC, nBreakExtra, nBreakCount);
}
// CDC::SetArcDirection
// Symbol: ?SetArcDirection@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetArcDirection_CDC__QEAAHH_Z(CDC* pThis, int nArcDirection) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetArcDirection(pThis->m_hDC, nArcDirection);
}
// CDC::SetGraphicsMode
// Symbol: ?SetGraphicsMode@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SetGraphicsMode_CDC__QEAAHH_Z(CDC* pThis, int nGraphicsMode) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::SetGraphicsMode(pThis->m_hDC, nGraphicsMode);
}
// CDC::GetLayout
// Symbol: ?GetLayout@CDC@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetLayout_CDC__QEBAKXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::GetLayout(pThis->m_hDC);
}
// CDC::SetLayout
// Symbol: ?SetLayout@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetLayout_CDC__QEAAKK_Z(CDC* pThis, unsigned long dwLayout) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::SetLayout(pThis->m_hDC, dwLayout);
}
// CDC::SetColorAdjustment
// Symbol: ?SetColorAdjustment@CDC@@QEAAHPEBUtagCOLORADJUSTMENT@@@Z
extern "C" int MS_ABI impl__SetColorAdjustment_CDC__QEAAHPEBUtagCOLORADJUSTMENT___Z(
    CDC* pThis, const COLORADJUSTMENT* lpColorAdjust) {
    if (!pThis || !pThis->m_hDC || !lpColorAdjust) return FALSE;
    return ::SetColorAdjustment(pThis->m_hDC, lpColorAdjust);
}
// CDC::SetWorldTransform
// Symbol: ?SetWorldTransform@CDC@@QEAAHPEBUtagXFORM@@@Z
extern "C" int MS_ABI impl__SetWorldTransform_CDC__QEAAHPEBUtagXFORM___Z(CDC* pThis, const XFORM* pXForm) {
    if (!pThis || !pThis->m_hDC || !pXForm) return FALSE;
    return ::SetWorldTransform(pThis->m_hDC, pXForm);
}
// CDC::ModifyWorldTransform
// Symbol: ?ModifyWorldTransform@CDC@@QEAAHPEBUtagXFORM@@K@Z
extern "C" int MS_ABI impl__ModifyWorldTransform_CDC__QEAAHPEBUtagXFORM__K_Z(
    CDC* pThis, const XFORM* pXForm, unsigned long iMode) {
    if (!pThis || !pThis->m_hDC || !pXForm) return FALSE;
    return ::ModifyWorldTransform(pThis->m_hDC, pXForm, iMode);
}
// CDC::SetMapperFlags
// Symbol: ?SetMapperFlags@CDC@@QEAAKK@Z
extern "C" unsigned long MS_ABI impl__SetMapperFlags_CDC__QEAAKK_Z(CDC* pThis, unsigned long dwFlag) {
    if (!pThis || !pThis->m_hDC) return GDI_ERROR;
    return ::SetMapperFlags(pThis->m_hDC, dwFlag);
}
// CDC::GetClipBox (virtual, UEBA)
// Symbol: ?GetClipBox@CDC@@UEBAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(const CDC* pThis, RECT* lpRect) {
    if (!pThis || !pThis->m_hDC || !lpRect) return ERROR;
    return ::GetClipBox(pThis->m_hDC, lpRect);
}
// CDC::SelectObject(CPen*)
// Symbol: ?SelectObject@CDC@@QEAAPEAVCPen@@PEAV2@@Z
extern "C" CPen* MS_ABI impl__SelectObject_CDC__QEAAPEAVCPen__PEAV2__Z(CDC* pThis, CPen* pPen) {
    if (!pThis || !pThis->m_hDC || !pPen) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pPen->GetSafeHandle());
    if (!hOld) return nullptr;
    // Return a temporary wrapper for the old object
    CGdiObject* pOld = GetTempGdiObject(hOld);
    return (CPen*)pOld;
}
// CDC::SelectObject(CBrush*)
// Symbol: ?SelectObject@CDC@@QEAAPEAVCBrush@@PEAV2@@Z
extern "C" CBrush* MS_ABI impl__SelectObject_CDC__QEAAPEAVCBrush__PEAV2__Z(CDC* pThis, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !pBrush) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pBrush->GetSafeHandle());
    if (!hOld) return nullptr;
    CGdiObject* pOld = GetTempGdiObject(hOld);
    return (CBrush*)pOld;
}
// CDC::SelectObject(CFont*) (virtual, UEAA)
// Symbol: ?SelectObject@CDC@@UEAAPEAVCFont@@PEAV2@@Z
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont) {
    if (!pThis || !pThis->m_hDC || !pFont) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pFont->GetSafeHandle());
    if (!hOld) return nullptr;
    CGdiObject* pOld = GetTempGdiObject(hOld);
    return (CFont*)pOld;
}
// CDC::SelectObject(CRgn*) - returns int
// Symbol: ?SelectObject@CDC@@QEAAHPEAVCRgn@@@Z
extern "C" int MS_ABI impl__SelectObject_CDC__QEAAHPEAVCRgn___Z(CDC* pThis, CRgn* pRgn) {
    if (!pThis || !pThis->m_hDC || !pRgn) return ERROR;
    HRGN hRgn = (HRGN)pRgn->GetSafeHandle();
    return ::SelectClipRgn(pThis->m_hDC, hRgn);
}
// CDC::SelectPalette
// Symbol: ?SelectPalette@CDC@@QEAAPEAVCPalette@@PEAV2@H@Z
extern "C" CPalette* MS_ABI impl__SelectPalette_CDC__QEAAPEAVCPalette__PEAV2_H_Z(
    CDC* pThis, CPalette* pPalette, int bForceBackground) {
    if (!pThis || !pThis->m_hDC || !pPalette) return nullptr;
    HPALETTE hOld = ::SelectPalette(pThis->m_hDC,
                                    (HPALETTE)pPalette->GetSafeHandle(),
                                    bForceBackground);
    if (!hOld) return nullptr;
    // Wrap in a temporary CGdiObject cast to CPalette*
    CGdiObject* pOld = GetTempGdiObject((HGDIOBJ)hOld);
    return (CPalette*)pOld;
}
// CDC::SelectStockObject (virtual, UEAA)
// Symbol: ?SelectStockObject@CDC@@UEAAPEAVCGdiObject@@H@Z
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC* pThis, int nIndex) {
    if (!pThis || !pThis->m_hDC) return nullptr;
    HGDIOBJ hNew = ::GetStockObject(nIndex);
    if (!hNew) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, hNew);
    return GetTempGdiObject(hOld);
}
// CDC::SelectGdiObject (static, KA qualified)
// Symbol: ?SelectGdiObject@CDC@@KAPEAVCGdiObject@@PEAUHDC__@@PEAX@Z
extern "C" CGdiObject* MS_ABI impl__SelectGdiObject_CDC__KAPEAVCGdiObject__PEAUHDC____PEAX_Z(
    HDC hDC, HGDIOBJ hObj) {
    if (!hDC || !hObj) return nullptr;
    HGDIOBJ hOld = ::SelectObject(hDC, hObj);
    return GetTempGdiObject(hOld);
}
// CDC::SelectClipPath
// Symbol: ?SelectClipPath@CDC@@QEAAHH@Z
extern "C" int MS_ABI impl__SelectClipPath_CDC__QEAAHH_Z(CDC* pThis, int nMode) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::SelectClipPath(pThis->m_hDC, nMode);
}
// CDC::SetAttribDC (virtual, UEAA)
// Symbol: ?SetAttribDC@CDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetAttribDC_CDC__UEAAXPEAUHDC_____Z(CDC* pThis, HDC hDC) {
    if (!pThis) return;
    pThis->m_hAttribDC = hDC;
}
// CDC::SetOutputDC (virtual, UEAA)
// Symbol: ?SetOutputDC@CDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetOutputDC_CDC__UEAAXPEAUHDC_____Z(CDC* pThis, HDC hDC) {
    if (!pThis) return;
    pThis->m_hDC = hDC;
}
// CDC::ReleaseAttribDC (virtual, UEAA)
// Symbol: ?ReleaseAttribDC@CDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseAttribDC_CDC__UEAAXXZ(CDC* pThis) {
    if (!pThis) return;
    pThis->m_hAttribDC = nullptr;
}
// CDC::ReleaseOutputDC (virtual, UEAA)
// Symbol: ?ReleaseOutputDC@CDC@@UEAAXXZ
extern "C" void MS_ABI impl__ReleaseOutputDC_CDC__UEAAXXZ(CDC* pThis) {
    if (!pThis) return;
    pThis->m_hDC = nullptr;
}
// CDC::ArcTo
// Symbol: ?ArcTo@CDC@@QEAAHHHHHHHHH@Z
extern "C" int MS_ABI impl__ArcTo_CDC__QEAAHHHHHHHHH_Z(
    CDC* pThis, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::ArcTo(pThis->m_hDC, x1, y1, x2, y2, x3, y3, x4, y4);
}
// CDC::PolylineTo
// Symbol: ?PolylineTo@CDC@@QEAAHPEBUtagPOINT@@H@Z
extern "C" int MS_ABI impl__PolylineTo_CDC__QEAAHPEBUtagPOINT__H_Z(
    CDC* pThis, const POINT* lpPoints, int nCount) {
    if (!pThis || !pThis->m_hDC || !lpPoints) return FALSE;
    return ::PolylineTo(pThis->m_hDC, lpPoints, (DWORD)nCount);
}
// CDC::PolyBezierTo
// Symbol: ?PolyBezierTo@CDC@@QEAAHPEBUtagPOINT@@H@Z
extern "C" int MS_ABI impl__PolyBezierTo_CDC__QEAAHPEBUtagPOINT__H_Z(
    CDC* pThis, const POINT* lpPoints, int nCount) {
    if (!pThis || !pThis->m_hDC || !lpPoints) return FALSE;
    return ::PolyBezierTo(pThis->m_hDC, lpPoints, (DWORD)nCount);
}
// CDC::PolyDraw
// Symbol: ?PolyDraw@CDC@@QEAAHPEBUtagPOINT@@PEBEH@Z
extern "C" int MS_ABI impl__PolyDraw_CDC__QEAAHPEBUtagPOINT__PEBEH_Z(
    CDC* pThis, const POINT* lpPoints, const BYTE* lpTypes, int nCount) {
    if (!pThis || !pThis->m_hDC || !lpPoints) return FALSE;
    return ::PolyDraw(pThis->m_hDC, lpPoints, lpTypes, nCount);
}
// CDC::PlayMetaFile
// Symbol: ?PlayMetaFile@CDC@@QEAAHPEAUHMETAFILE__@@@Z
extern "C" int MS_ABI impl__PlayMetaFile_CDC__QEAAHPEAUHMETAFILE_____Z(CDC* pThis, HMETAFILE hMF) {
    if (!pThis || !pThis->m_hDC || !hMF) return FALSE;
    return ::PlayMetaFile(pThis->m_hDC, hMF);
}
// CDC::StartDocW
// Symbol: ?StartDocW@CDC@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__StartDocW_CDC__QEAAHPEB_W_Z(CDC* pThis, const wchar_t* lpszDocName) {
    if (!pThis || !pThis->m_hDC) return SP_ERROR;
    DOCINFOW di = {};
    di.cbSize = sizeof(di);
    di.lpszDocName = lpszDocName;
    return ::StartDocW(pThis->m_hDC, &di);
}
// CDC::Draw3dRect (int coords)
// Symbol: ?Draw3dRect@CDC@@QEAAXHHHHKK@Z
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXHHHHKK_Z(
    CDC* pThis, int x, int y, int cx, int cy,
    unsigned long clrTopLeft, unsigned long clrBottomRight) {
    if (!pThis || !pThis->m_hDC) return;
    RECT rc = {x, y, x + cx, y + cy};
    // Top and left edges
    RECT rcEdge = {rc.left, rc.top, rc.right - 1, rc.top + 1};
    ::SetBkColor(pThis->m_hDC, clrTopLeft);
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
    rcEdge = {rc.left, rc.top, rc.left + 1, rc.bottom - 1};
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
    // Bottom and right edges
    rcEdge = {rc.left, rc.bottom - 1, rc.right, rc.bottom};
    ::SetBkColor(pThis->m_hDC, clrBottomRight);
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
    rcEdge = {rc.right - 1, rc.top, rc.right, rc.bottom};
    ::ExtTextOutW(pThis->m_hDC, 0, 0, ETO_OPAQUE, &rcEdge, nullptr, 0, nullptr);
}
// CDC::Draw3dRect (RECT)
// Symbol: ?Draw3dRect@CDC@@QEAAXPEBUtagRECT@@KK@Z
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect,
    unsigned long clrTopLeft, unsigned long clrBottomRight) {
    if (!lpRect) return;
    impl__Draw3dRect_CDC__QEAAXHHHHKK_Z(
        pThis,
        lpRect->left, lpRect->top,
        lpRect->right - lpRect->left,
        lpRect->bottom - lpRect->top,
        clrTopLeft, clrBottomRight);
}
// CDC::DrawDragRect
// Symbol: ?DrawDragRect@CDC@@QEAAXPEBUtagRECT@@UtagSIZE@@01PEAVCBrush@@2@Z
extern "C" void MS_ABI impl__DrawDragRect_CDC__QEAAXPEBUtagRECT__UtagSIZE__01PEAVCBrush__2_Z(
    CDC* pThis, const RECT* lpRect, SIZE size,
    const RECT* lpRectLast, SIZE sizeLast,
    CBrush* pBrush, CBrush* pBrushLast) {
    if (!pThis || !pThis->m_hDC) return;
    // Erase last rect by drawing with pBrushLast
    if (lpRectLast) {
        HBRUSH hBrush = pBrushLast ? (HBRUSH)pBrushLast->GetSafeHandle()
                                   : (HBRUSH)::GetStockObject(GRAY_BRUSH);
        // Draw 4 border rects for the "last" area
        RECT rc;
        rc = {lpRectLast->left, lpRectLast->top, lpRectLast->right, lpRectLast->top + sizeLast.cy};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRectLast->left, lpRectLast->bottom - sizeLast.cy, lpRectLast->right, lpRectLast->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRectLast->left, lpRectLast->top, lpRectLast->left + sizeLast.cx, lpRectLast->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRectLast->right - sizeLast.cx, lpRectLast->top, lpRectLast->right, lpRectLast->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
    }
    // Draw current rect
    if (lpRect) {
        HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle()
                               : (HBRUSH)::GetStockObject(BLACK_BRUSH);
        RECT rc;
        rc = {lpRect->left, lpRect->top, lpRect->right, lpRect->top + size.cy};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRect->left, lpRect->bottom - size.cy, lpRect->right, lpRect->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRect->left, lpRect->top, lpRect->left + size.cx, lpRect->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
        rc = {lpRect->right - size.cx, lpRect->top, lpRect->right, lpRect->bottom};
        ::FillRect(pThis->m_hDC, &rc, hBrush);
    }
}
// CDC::GetHalftoneBrush (static)
// Symbol: ?GetHalftoneBrush@CDC@@SAPEAVCBrush@@XZ
extern "C" CBrush* MS_ABI impl__GetHalftoneBrush_CDC__SAPEAVCBrush__XZ() {
    HGDIOBJ hBrush = ::GetStockObject(DKGRAY_BRUSH);
    return (CBrush*)GetTempGdiObject(hBrush);
}
// CDC::DPtoHIMETRIC
// Symbol: ?DPtoHIMETRIC@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__DPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    HDC hDC = pThis->m_hDC;
    lpSize->cx = MulDiv(lpSize->cx, 2540, ::GetDeviceCaps(hDC, LOGPIXELSX));
    lpSize->cy = MulDiv(lpSize->cy, 2540, ::GetDeviceCaps(hDC, LOGPIXELSY));
}
// CDC::LPtoHIMETRIC
// Symbol: ?LPtoHIMETRIC@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__LPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    // First convert LP to DP, then DP to HIMETRIC
    POINT pt = {lpSize->cx, lpSize->cy};
    ::LPtoDP(pThis->m_hDC, &pt, 1);
    SIZE sz = {pt.x, pt.y};
    impl__DPtoHIMETRIC_CDC__QEBAXPEAUtagSIZE___Z(pThis, &sz);
    lpSize->cx = sz.cx;
    lpSize->cy = sz.cy;
}
// CDC::HIMETRICtoDP
// Symbol: ?HIMETRICtoDP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__HIMETRICtoDP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    HDC hDC = pThis->m_hDC;
    lpSize->cx = MulDiv(lpSize->cx, ::GetDeviceCaps(hDC, LOGPIXELSX), 2540);
    lpSize->cy = MulDiv(lpSize->cy, ::GetDeviceCaps(hDC, LOGPIXELSY), 2540);
}
// CDC::HIMETRICtoLP
// Symbol: ?HIMETRICtoLP@CDC@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__HIMETRICtoLP_CDC__QEBAXPEAUtagSIZE___Z(const CDC* pThis, SIZE* lpSize) {
    if (!pThis || !pThis->m_hDC || !lpSize) return;
    SIZE sz = *lpSize;
    impl__HIMETRICtoDP_CDC__QEBAXPEAUtagSIZE___Z(pThis, &sz);
    POINT pt = {sz.cx, sz.cy};
    ::DPtoLP(pThis->m_hDC, &pt, 1);
    lpSize->cx = pt.x;
    lpSize->cy = pt.y;
}
// CDC::CreateObject (static) - runtime allocation
// Symbol: ?CreateObject@CDC@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CDC__SAPEAVCObject__XZ() {
    return new CDC();
}
CDC::CDC() {
    impl___0CDC__QEAA_XZ(this);
}
CDC::~CDC() {
    impl___1CDC__UEAA_XZ(this);
}
// Symbol: ?GetRuntimeClass@CDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetRuntimeClass_CDC__UEBAPEAUCRuntimeClass__XZ(void* pThis) {
    (void)pThis;
    return &CDC::classCDC;
}
// Symbol: ?GetThisClass@CDC@@SAPEAUCRuntimeClass@@XZ
extern "C" void* MS_ABI impl__GetThisClass_CDC__SAPEAUCRuntimeClass__XZ() {
    return &CDC::classCDC;
}
