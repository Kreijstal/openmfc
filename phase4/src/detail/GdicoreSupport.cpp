#include "GdicoreSupport.h"

IMPLEMENT_DYNAMIC(CGdiObject, CObject)
IMPLEMENT_DYNAMIC(CPen, CGdiObject)
IMPLEMENT_DYNAMIC(CBrush, CGdiObject)
IMPLEMENT_DYNAMIC(CFont, CGdiObject)
IMPLEMENT_DYNAMIC(CBitmap, CGdiObject)
IMPLEMENT_DYNAMIC(CPalette, CGdiObject)
IMPLEMENT_DYNAMIC(CRgn, CGdiObject)
IMPLEMENT_DYNAMIC(CDC, CObject)
IMPLEMENT_DYNAMIC(CClientDC, CDC)
IMPLEMENT_DYNAMIC(CPaintDC, CDC)
IMPLEMENT_DYNAMIC(CWindowDC, CDC)
IMPLEMENT_DYNAMIC(CMetaFileDC, CDC)
IMPLEMENT_DYNAMIC(CRenderTarget, CObject)
IMPLEMENT_DYNAMIC(CDCRenderTarget, CRenderTarget)
IMPLEMENT_DYNAMIC(CAnimationVariable, CObject)
IMPLEMENT_DYNCREATE(CImageList, CObject)
namespace openmfc { namespace detail { namespace gdicore {
thread_local std::unordered_map<HGDIOBJ, CGdiObject*> g_tempGdiMap;
thread_local std::unordered_map<HDC, CDC*> g_tempDCMap;
CGdiObject* GetTempGdiObject(HGDIOBJ hObj) {
    if (!hObj) return nullptr;

    auto it = g_tempGdiMap.find(hObj);
    if (it != g_tempGdiMap.end()) {
        return it->second;
    }

    // Create a temporary wrapper for the old object
    auto* wrapper = new CGdiObject();
    wrapper->m_hObject = hObj;
    g_tempGdiMap.emplace(hObj, wrapper);
    return wrapper;
}
void DeleteTempGdiMap() {
    for (auto& [_, obj] : g_tempGdiMap) {
        // Null out the handle so the destructor doesn't try to delete it
        // (we don't own these GDI objects)
        obj->m_hObject = nullptr;
        delete obj;
    }
    g_tempGdiMap.clear();
}
CDC* GetTempDC(HDC hDC) {
    if (!hDC) return nullptr;

    auto it = g_tempDCMap.find(hDC);
    if (it != g_tempDCMap.end()) {
        return it->second;
    }

    auto* wrapper = new CDC();
    wrapper->m_hDC = hDC;
    wrapper->m_hAttribDC = hDC;
    g_tempDCMap.emplace(hDC, wrapper);
    return wrapper;
}
void DeleteTempDCMap() {
    for (auto& [_, dc] : g_tempDCMap) {
        dc->m_hDC = nullptr;
        dc->m_hAttribDC = nullptr;
        delete dc;
    }
    g_tempDCMap.clear();
}
std::unordered_map<const CRenderTarget*, CRenderTargetState> g_renderTargetState;
std::unordered_map<const CAnimationVariable*, CAnimationVariableState> g_animationVariableState;
std::mutex g_wave2StateMutex;
extern "C" int MS_ABI impl__CreateCompatibleDC_CDC__QEAAHPEAV1__Z(CDC* pThis, CDC* pDC) {
    if (!pThis) return FALSE;

    HDC hDC = pDC ? pDC->m_hDC : nullptr;
    pThis->m_hDC = ::CreateCompatibleDC(hDC);
    pThis->m_hAttribDC = pThis->m_hDC;
    return pThis->m_hDC != nullptr;
}
extern "C" int MS_ABI impl__Rectangle_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Rectangle(pThis->m_hDC, x1, y1, x2, y2);
}
extern "C" int MS_ABI impl__Ellipse_CDC__QEAAHHHHH_Z(CDC* pThis, int x1, int y1, int x2, int y2) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    return ::Ellipse(pThis->m_hDC, x1, y1, x2, y2);
}
extern "C" int MS_ABI impl__TextOutW_CDC__QEAAHHPEB_WH_Z(CDC* pThis, int x, int y, const wchar_t* lpszString, int nCount) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    if (nCount < 0 && lpszString) {
        nCount = (int)wcslen(lpszString);
    }
    return ::TextOutW(pThis->m_hDC, x, y, lpszString, nCount);
}
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetBkColor(pThis->m_hDC, crColor);
}
extern "C" unsigned long MS_ABI impl__GetBkColor_CDC__QEBAKXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::GetBkColor(pThis->m_hDC);
}
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long crColor) {
    if (!pThis || !pThis->m_hDC) return CLR_INVALID;
    return ::SetTextColor(pThis->m_hDC, crColor);
}
extern "C" int MS_ABI impl__GetTextColor_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return (int)::GetTextColor(pThis->m_hDC);
}
extern "C" int MS_ABI impl__GetBkMode_CDC__QEBAHXZ(const CDC* pThis) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::GetBkMode(pThis->m_hDC);
}
extern "C" int MS_ABI impl__BitBlt_CDC__QEAAHHHHHHPEAV1_HHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::BitBlt(pThis->m_hDC, x, y, nWidth, nHeight, hSrcDC, xSrc, ySrc, dwRop);
}
extern "C" int MS_ABI impl__StretchBlt_CDC__QEAAHHHHHHPEAV1_HHHHK_Z(
    CDC* pThis, int x, int y, int nWidth, int nHeight,
    CDC* pSrcDC, int xSrc, int ySrc, int nSrcWidth, int nSrcHeight, unsigned long dwRop) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HDC hSrcDC = pSrcDC ? pSrcDC->m_hDC : nullptr;
    return ::StretchBlt(pThis->m_hDC, x, y, nWidth, nHeight,
                        hSrcDC, xSrc, ySrc, nSrcWidth, nSrcHeight, dwRop);
}
extern "C" int MS_ABI impl__FillRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FillRect(pThis->m_hDC, lpRect, hBrush);
}
extern "C" int MS_ABI impl__FrameRect_CDC__QEAAHPEBUtagRECT__PEAVCBrush___Z(
    CDC* pThis, const RECT* lpRect, CBrush* pBrush) {
    if (!pThis || !pThis->m_hDC || !lpRect) return FALSE;
    HBRUSH hBrush = pBrush ? (HBRUSH)pBrush->GetSafeHandle() : nullptr;
    return ::FrameRect(pThis->m_hDC, lpRect, hBrush);
}
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat) {
    if (!pThis || !pThis->m_hDC) return 0;
    return ::DrawTextW(pThis->m_hDC, lpszString, nCount, lpRect, nFormat);
}
extern "C" CGdiObject* MS_ABI impl__SelectObject_CDC__QEAAPEAVCGdiObject__PEAV2__Z(
    CDC* pThis, CGdiObject* pObject) {
    if (!pThis || !pThis->m_hDC || !pObject) return nullptr;
    HGDIOBJ hOld = ::SelectObject(pThis->m_hDC, pObject->GetSafeHandle());
    // Return a temporary wrapper for the old object so callers can restore it
    return GetTempGdiObject(hOld);
}
extern "C" int MS_ABI impl__SelectStockObject_CDC__QEAAHH_Z(CDC* pThis, int nIndex) {
    if (!pThis || !pThis->m_hDC) return FALSE;
    HGDIOBJ hObj = ::GetStockObject(nIndex);
    return ::SelectObject(pThis->m_hDC, hObj) != nullptr;
}
extern "C" CPen* MS_ABI impl___0CPen__QEAA_XZ(CPen* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}
extern "C" int MS_ABI impl__CreatePen_CPen__QEAAHHHI_Z(CPen* pThis, int nPenStyle, int nWidth, unsigned int crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreatePen(nPenStyle, nWidth, crColor);
    return pThis->m_hObject != nullptr;
}
extern "C" CBrush* MS_ABI impl___0CBrush__QEAA_XZ(CBrush* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}
extern "C" int MS_ABI impl__CreateSolidBrush_CBrush__QEAAHK_Z(CBrush* pThis, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateSolidBrush(crColor);
    return pThis->m_hObject != nullptr;
}
extern "C" int MS_ABI impl__CreateHatchBrush_CBrush__QEAAHHK_Z(CBrush* pThis, int nIndex, unsigned long crColor) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateHatchBrush(nIndex, crColor);
    return pThis->m_hObject != nullptr;
}
extern "C" CFont* MS_ABI impl___0CFont__QEAA_XZ(CFont* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}
extern "C" int MS_ABI impl__CreateFontIndirectW_CFont__QEAAHPEBUtagLOGFONTW___Z(CFont* pThis, const LOGFONTW* lpLogFont) {
    if (!pThis || !lpLogFont) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateFontIndirectW(lpLogFont);
    return pThis->m_hObject != nullptr;
}
extern "C" CBitmap* MS_ABI impl___0CBitmap__QEAA_XZ(CBitmap* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hObject = nullptr;
    return pThis;
}
extern "C" int MS_ABI impl__CreateCompatibleBitmap_CBitmap__QEAAHPEAVCDC__HH_Z(
    CBitmap* pThis, CDC* pDC, int nWidth, int nHeight) {
    if (!pThis || !pDC || !pDC->m_hDC) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    pThis->m_hObject = ::CreateCompatibleBitmap(pDC->m_hDC, nWidth, nHeight);
    return pThis->m_hObject != nullptr;
}
extern "C" int MS_ABI impl__LoadBitmapW_CBitmap__QEAAHPEB_W_Z(CBitmap* pThis, const wchar_t* lpszResourceName) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hObject = ::LoadBitmapW(hInst, lpszResourceName);
    return pThis->m_hObject != nullptr;
}
extern "C" int MS_ABI impl__LoadBitmapW_CBitmap__QEAAHI_Z(CBitmap* pThis, UINT nIDResource) {
    if (!pThis) return FALSE;
    if (pThis->m_hObject) {
        ::DeleteObject(pThis->m_hObject);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hObject = ::LoadBitmapW(hInst, MAKEINTRESOURCEW(nIDResource));
    return pThis->m_hObject != nullptr;
}
thread_local std::unordered_map<const CMFCToolBarImages*, ToolBarImagesState> g_toolbarImagesState;
ToolBarImagesState& EnsureToolBarImagesState(const CMFCToolBarImages* pImages) {
    return g_toolbarImagesState[pImages];
}
const ToolBarImagesState* FindToolBarImagesState(const CMFCToolBarImages* pImages) {
    auto it = g_toolbarImagesState.find(pImages);
    return (it != g_toolbarImagesState.end()) ? &it->second : nullptr;
}
void ClearToolBarImagesState(CMFCToolBarImages* pImages, BOOL bDestroyIcons) {
    if (!pImages) return;
    auto it = g_toolbarImagesState.find(pImages);
    if (it == g_toolbarImagesState.end()) return;
    if (bDestroyIcons) {
        for (HICON hIcon : it->second.ownedIcons) {
            if (hIcon) ::DestroyIcon(hIcon);
        }
    }
    it->second.ownedIcons.clear();
    it->second.count = 0;
    it->second.isValid = FALSE;
}
void RemoveToolBarImagesState(CMFCToolBarImages* pImages) {
    if (!pImages) return;
    auto it = g_toolbarImagesState.find(pImages);
    if (it == g_toolbarImagesState.end()) return;
    for (HICON hIcon : it->second.ownedIcons) {
        if (hIcon) ::DestroyIcon(hIcon);
    }
    g_toolbarImagesState.erase(it);
}
CD2DPointF* InitCD2DPointF(CD2DPointF* pThis, float xValue, float yValue) {
    if (!pThis) return nullptr;
    pThis->x = xValue;
    pThis->y = yValue;
    return pThis;
}
CD2DSizeF* InitCD2DSizeF(CD2DSizeF* pThis, float widthValue, float heightValue) {
    if (!pThis) return nullptr;
    pThis->width = widthValue;
    pThis->height = heightValue;
    return pThis;
}
CD2DRectF* InitCD2DRectF(CD2DRectF* pThis, float leftValue, float topValue, float rightValue, float bottomValue) {
    if (!pThis) return nullptr;
    pThis->left = leftValue;
    pThis->top = topValue;
    pThis->right = rightValue;
    pThis->bottom = bottomValue;
    return pThis;
}
thread_local std::unordered_map<HIMAGELIST, CImageList*> g_tempImageListMap;
CImageList* GetTempImageList(HIMAGELIST hImageList) {
    if (!hImageList) return nullptr;
    auto it = g_tempImageListMap.find(hImageList);
    if (it != g_tempImageListMap.end()) return it->second;
    auto* pIL = new CImageList();
    pIL->m_hImageList = hImageList;
    g_tempImageListMap.emplace(hImageList, pIL);
    return pIL;
}
} } }  // namespace openmfc::detail::gdicore
