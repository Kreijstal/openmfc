// CRgn — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

CRgn::CRgn() : CGdiObject() {
}
CRgn::~CRgn() {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
        m_hObject = nullptr;
    }
}
int CRgn::CreateRectRgn(int x1, int y1, int x2, int y2) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateRectRgn(x1, y1, x2, y2);
    return m_hObject != nullptr;
}
int CRgn::CreateRectRgnIndirect(const RECT* lpRect) {
    if (!lpRect) return FALSE;
    return CreateRectRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
int CRgn::CreateEllipticRgn(int x1, int y1, int x2, int y2) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateEllipticRgn(x1, y1, x2, y2);
    return m_hObject != nullptr;
}
int CRgn::CreateEllipticRgnIndirect(const RECT* lpRect) {
    if (!lpRect) return FALSE;
    return CreateEllipticRgn(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
int CRgn::CreatePolygonRgn(const POINT* lpPoints, int nCount, int nMode) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreatePolygonRgn(lpPoints, nCount, nMode);
    return m_hObject != nullptr;
}
int CRgn::CreateRoundRectRgn(int x1, int y1, int x2, int y2, int x3, int y3) {
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::CreateRoundRectRgn(x1, y1, x2, y2, x3, y3);
    return m_hObject != nullptr;
}
int CRgn::CreateFromPath(CDC* pDC) {
    if (!pDC || !pDC->m_hDC) return FALSE;
    if (m_hObject) {
        ::DeleteObject(m_hObject);
    }
    m_hObject = ::PathToRegion(pDC->m_hDC);
    return m_hObject != nullptr;
}
void CRgn::SetRectRgn(int x1, int y1, int x2, int y2) {
    if (m_hObject) {
        ::SetRectRgn((HRGN)m_hObject, x1, y1, x2, y2);
    }
}
void CRgn::SetRectRgn(const RECT* lpRect) {
    if (lpRect && m_hObject) {
        ::SetRectRgn((HRGN)m_hObject, lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
    }
}
int CRgn::CombineRgn(CRgn* pRgn1, CRgn* pRgn2, int nCombineMode) {
    if (!m_hObject || !pRgn1 || !pRgn1->m_hObject) return ERROR;
    HRGN hRgn2 = pRgn2 ? (HRGN)pRgn2->m_hObject : nullptr;
    return ::CombineRgn((HRGN)m_hObject, (HRGN)pRgn1->m_hObject, hRgn2, nCombineMode);
}
int CRgn::CopyRgn(CRgn* pRgnSrc) {
    if (!m_hObject || !pRgnSrc || !pRgnSrc->m_hObject) return ERROR;
    return ::CombineRgn((HRGN)m_hObject, (HRGN)pRgnSrc->m_hObject, nullptr, RGN_COPY);
}
int CRgn::EqualRgn(CRgn* pRgn) const {
    if (!m_hObject || !pRgn || !pRgn->m_hObject) return FALSE;
    return ::EqualRgn((HRGN)m_hObject, (HRGN)pRgn->m_hObject);
}
int CRgn::OffsetRgn(int x, int y) {
    if (!m_hObject) return ERROR;
    return ::OffsetRgn((HRGN)m_hObject, x, y);
}
int CRgn::OffsetRgn(POINT point) {
    return OffsetRgn(point.x, point.y);
}
int CRgn::GetRgnBox(RECT* lpRect) const {
    if (!m_hObject || !lpRect) return ERROR;
    return ::GetRgnBox((HRGN)m_hObject, lpRect);
}
int CRgn::PtInRegion(int x, int y) const {
    if (!m_hObject) return FALSE;
    return ::PtInRegion((HRGN)m_hObject, x, y);
}
int CRgn::PtInRegion(POINT point) const {
    return PtInRegion(point.x, point.y);
}
int CRgn::RectInRegion(const RECT* lpRect) const {
    if (!m_hObject || !lpRect) return FALSE;
    return ::RectInRegion((HRGN)m_hObject, lpRect);
}
int CRgn::GetRegionData(RGNDATA* lpRgnData, int nDataSize) const {
    if (!m_hObject) return 0;
    return ::GetRegionData((HRGN)m_hObject, nDataSize, lpRgnData);
}
