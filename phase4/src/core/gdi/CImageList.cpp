// CImageList — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"

// Symbol: ??0CImageList@@QEAA@XZ
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* pThis) {
    if (!pThis) return nullptr;
    pThis->m_hImageList = nullptr;
    return pThis;
}
// Symbol: ??1CImageList@@UEAA@XZ
extern "C" void MS_ABI impl___1CImageList__UEAA_XZ(CImageList* pThis) {
    if (!pThis) return;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
        pThis->m_hImageList = nullptr;
    }
}
// Symbol: ?Create@CImageList@@QEAAHHHIHH@Z
// Create(cx, cy, flags, cInitial, cGrow)
extern "C" int MS_ABI impl__Create_CImageList__QEAAHHHIHH_Z(
    CImageList* pThis, int cx, int cy, unsigned int nFlags, int nInitial, int nGrow) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    pThis->m_hImageList = ::ImageList_Create(cx, cy, nFlags, nInitial, nGrow);
    return pThis->m_hImageList != nullptr;
}
// Symbol: ?Create@CImageList@@QEAAHIHHK@Z
// Create(nBitmapID, cx, nGrow, crMask) - load from resource ID
extern "C" int MS_ABI impl__Create_CImageList__QEAAHIHHK_Z(
    CImageList* pThis, unsigned int nBitmapID, int cx, int nGrow, unsigned long crMask) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hImageList = ::ImageList_LoadImageW(
        hInst, MAKEINTRESOURCEW(nBitmapID), cx, nGrow, crMask, IMAGE_BITMAP, LR_DEFAULTCOLOR);
    return pThis->m_hImageList != nullptr;
}
// Symbol: ?Create@CImageList@@QEAAHPEB_WHHK@Z
// Create(lpszBitmapID, cx, nGrow, crMask) - load from resource name
extern "C" int MS_ABI impl__Create_CImageList__QEAAHPEB_WHHK_Z(
    CImageList* pThis, const wchar_t* lpszBitmapID, int cx, int nGrow, unsigned long crMask) {
    if (!pThis || !lpszBitmapID) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    HINSTANCE hInst = AfxGetInstanceHandle();
    pThis->m_hImageList = ::ImageList_LoadImageW(
        hInst, lpszBitmapID, cx, nGrow, crMask, IMAGE_BITMAP, LR_DEFAULTCOLOR);
    return pThis->m_hImageList != nullptr;
}
// Symbol: ?Create@CImageList@@QEAAHPEAV1@@Z
// Create(pImageList) - copy constructor
extern "C" int MS_ABI impl__Create_CImageList__QEAAHPEAV1__Z(CImageList* pThis, CImageList* pImageList) {
    if (!pThis || !pImageList || !pImageList->m_hImageList) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    // Duplicate the image list
    pThis->m_hImageList = ::ImageList_Duplicate(pImageList->m_hImageList);
    return pThis->m_hImageList != nullptr;
}
// Symbol: ?Create@CImageList@@QEAAHAEAV1@H0HHH@Z
// Create(imagelist&, ...) - merge/create combined
extern "C" int MS_ABI impl__Create_CImageList__QEAAHAEAV1_H0HHH_Z(
    CImageList* pThis, CImageList* pImageList1, int nImage1, CImageList* pImageList2,
    int nImage2, int dx, int dy) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
        pThis->m_hImageList = nullptr;
    }
    HIMAGELIST h1 = pImageList1 ? pImageList1->m_hImageList : nullptr;
    HIMAGELIST h2 = pImageList2 ? pImageList2->m_hImageList : nullptr;
    if (!h1 && !h2) return FALSE;
    pThis->m_hImageList = ::ImageList_Merge(h1, nImage1, h2, nImage2, dx, dy);
    return pThis->m_hImageList != nullptr;
}
// Symbol: ?Attach@CImageList@@QEAAHPEAU_IMAGELIST@@@Z
extern "C" int MS_ABI impl__Attach_CImageList__QEAAHPEAU_IMAGELIST___Z(
    CImageList* pThis, HIMAGELIST hImageList) {
    if (!pThis) return FALSE;
    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
    }
    pThis->m_hImageList = hImageList;
    return TRUE;
}
// Symbol: ?Detach@CImageList@@QEAAPEAU_IMAGELIST@@XZ
extern "C" HIMAGELIST MS_ABI impl__Detach_CImageList__QEAAPEAU_IMAGELIST__XZ(CImageList* pThis) {
    if (!pThis) return nullptr;
    HIMAGELIST h = pThis->m_hImageList;
    pThis->m_hImageList = nullptr;
    return h;
}
// Symbol: ?DeleteImageList@CImageList@@QEAAHXZ
extern "C" int MS_ABI impl__DeleteImageList_CImageList__QEAAHXZ(CImageList* pThis) {
    if (!pThis || !pThis->m_hImageList) return FALSE;
    int result = ::ImageList_Destroy(pThis->m_hImageList);
    pThis->m_hImageList = nullptr;
    return result;
}
// Symbol: ?FromHandle@CImageList@@SAPEAV1@PEAU_IMAGELIST@@@Z
extern "C" CImageList* MS_ABI impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST hImageList) {
    return GetTempImageList(hImageList);
}
// Symbol: ?FromHandlePermanent@CImageList@@SAPEAV1@PEAU_IMAGELIST@@@Z
extern "C" CImageList* MS_ABI impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST hImageList) {
    return GetTempImageList(hImageList);
}
// Symbol: ?DeleteTempMap@CImageList@@SAXXZ
extern "C" void MS_ABI impl__DeleteTempMap_CImageList__SAXXZ() {
    for (auto& [_, pIL] : g_tempImageListMap) {
        // Don't destroy the image list - we don't own it
        pIL->m_hImageList = nullptr;
        delete pIL;
    }
    g_tempImageListMap.clear();
}
// Symbol: ?DrawIndirect@CImageList@@QEAAHPEAU_IMAGELISTDRAWPARAMS@@@Z
extern "C" int MS_ABI impl__DrawIndirect_CImageList__QEAAHPEAU_IMAGELISTDRAWPARAMS___Z(
    CImageList* pThis, IMAGELISTDRAWPARAMS* pimldp) {
    if (!pThis || !pThis->m_hImageList || !pimldp) return FALSE;
    pimldp->himl = pThis->m_hImageList;
    return ::ImageList_DrawIndirect(pimldp);
}
// Symbol: ?DrawIndirect@CImageList@@QEAAHPEAVCDC@@HUtagPOINT@@UtagSIZE@@1IKKKKKK@Z
extern "C" int MS_ABI impl__DrawIndirect_CImageList__QEAAHPEAVCDC__HUtagPOINT__UtagSIZE__1IKKKKKK_Z(
    CImageList* pThis, CDC* pDC, int nImage, POINT pt, SIZE sz,
    POINT ptOrigin, unsigned int fStyle, unsigned long rgbFg,
    unsigned long rgbBk, unsigned long rgbBlend1, unsigned long rgbBlend2,
    unsigned long fState, unsigned long Frame, unsigned long crEffect) {
    if (!pThis || !pThis->m_hImageList || !pDC || !pDC->m_hDC) return FALSE;
    IMAGELISTDRAWPARAMS ildp = {};
    ildp.cbSize = sizeof(ildp);
    ildp.himl = pThis->m_hImageList;
    ildp.i = nImage;
    ildp.hdcDst = pDC->m_hDC;
    ildp.x = pt.x;
    ildp.y = pt.y;
    ildp.cx = sz.cx;
    ildp.cy = sz.cy;
    ildp.xBitmap = ptOrigin.x;
    ildp.yBitmap = ptOrigin.y;
    ildp.rgbBk = rgbBk;
    ildp.rgbFg = rgbFg;
    ildp.fStyle = fStyle;
    ildp.dwRop = SRCCOPY;
    ildp.fState = fState;
    ildp.Frame = Frame;
    ildp.crEffect = crEffect;
    return ::ImageList_DrawIndirect(&ildp);
}
// Symbol: ?Read@CImageList@@QEAAHPEAVCArchive@@@Z
extern "C" int MS_ABI impl__Read_CImageList__QEAAHPEAVCArchive___Z(CImageList* pThis, CArchive* pAr) {
    if (!pThis || !pAr || !pAr->IsLoading()) return FALSE;

    void* serializedHandle = nullptr;
    int nImageCount = 0;
    int cx = 0;
    int cy = 0;
    unsigned int flags = 0;
    int grow = 0;
    unsigned long bgColor = (unsigned long)CLR_NONE;

    *pAr >> serializedHandle;
    *pAr >> nImageCount;
    *pAr >> cx;
    *pAr >> cy;
    *pAr >> flags;
    *pAr >> grow;
    *pAr >> bgColor;

    if (pThis->m_hImageList) {
        ::ImageList_Destroy(pThis->m_hImageList);
        pThis->m_hImageList = nullptr;
    }

    HIMAGELIST source = (HIMAGELIST)serializedHandle;
    if (source) {
        HIMAGELIST copy = ::ImageList_Duplicate(source);
        if (copy) {
            pThis->m_hImageList = copy;
            if (bgColor != (unsigned long)CLR_NONE) {
                ::ImageList_SetBkColor(pThis->m_hImageList, (COLORREF)bgColor);
            }
            return TRUE;
        }
    }

    if (cx > 0 && cy > 0) {
        if (flags == 0) flags = ILC_COLOR32;
        if (nImageCount < 0) nImageCount = 0;
        pThis->m_hImageList = ::ImageList_Create(cx, cy, flags, nImageCount, grow > 0 ? grow : 1);
        if (pThis->m_hImageList && bgColor != (unsigned long)CLR_NONE) {
            ::ImageList_SetBkColor(pThis->m_hImageList, (COLORREF)bgColor);
        }
        return TRUE;
    }

    if (!serializedHandle) {
        return TRUE;
    }

    return FALSE;
}
// Symbol: ?Write@CImageList@@QEAAHPEAVCArchive@@@Z
extern "C" int MS_ABI impl__Write_CImageList__QEAAHPEAVCArchive___Z(CImageList* pThis, CArchive* pAr) {
    if (!pThis || !pAr || !pAr->IsStoring()) return FALSE;

    void* serializedHandle = pThis->m_hImageList;
    int nImageCount = pThis ? pThis->GetImageCount() : 0;
    int cx = 0;
    int cy = 0;
    unsigned int flags = ILC_COLOR32;
    int grow = 1;
    unsigned long bgColor = (unsigned long)CLR_NONE;

    if (pThis->m_hImageList) {
        if (!::ImageList_GetIconSize(pThis->m_hImageList, &cx, &cy)) {
            cx = cy = 0;
        }
        bgColor = static_cast<unsigned long>(::ImageList_GetBkColor(pThis->m_hImageList));
    }

    *pAr << serializedHandle;
    *pAr << nImageCount;
    *pAr << cx;
    *pAr << cy;
    *pAr << flags;
    *pAr << grow;
    *pAr << bgColor;
    return TRUE;
}
// Symbol: ?GetRuntimeClass@CImageList@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CImageList__UEBAPEAUCRuntimeClass__XZ(
    const CImageList* pThis) {
    (void)pThis;
    return CImageList::GetThisClass();
}
// Symbol: ?GetThisClass@CImageList@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CImageList__SAPEAUCRuntimeClass__XZ() {
    return CImageList::GetThisClass();
}
// Symbol: ?CreateObject@CImageList@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CImageList__SAPEAVCObject__XZ() {
    return new CImageList();
}
CImageList::CImageList() {
    impl___0CImageList__QEAA_XZ(this);
}
CImageList::~CImageList() {
    impl___1CImageList__UEAA_XZ(this);
}
int CImageList::Create(int cx, int cy, UINT nFlags, int nInitial, int nGrow) {
    return impl__Create_CImageList__QEAAHHHIHH_Z(this, cx, cy, nFlags, nInitial, nGrow);
}
int CImageList::Create(UINT nBitmapID, int cx, int nGrow, COLORREF crMask) {
    return impl__Create_CImageList__QEAAHIHHK_Z(this, nBitmapID, cx, nGrow, crMask);
}
int CImageList::Create(const wchar_t* lpszBitmapID, int cx, int nGrow, COLORREF crMask) {
    return impl__Create_CImageList__QEAAHPEB_WHHK_Z(this, lpszBitmapID, cx, nGrow, crMask);
}
int CImageList::Create(CImageList* pImageList) {
    return impl__Create_CImageList__QEAAHPEAV1__Z(this, pImageList);
}
int CImageList::Attach(HIMAGELIST hImageList) {
    return impl__Attach_CImageList__QEAAHPEAU_IMAGELIST___Z(this, hImageList);
}
HIMAGELIST CImageList::Detach() {
    return impl__Detach_CImageList__QEAAPEAU_IMAGELIST__XZ(this);
}
int CImageList::DeleteImageList() {
    return impl__DeleteImageList_CImageList__QEAAHXZ(this);
}
CImageList* CImageList::FromHandle(HIMAGELIST hImageList) {
    return impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(hImageList);
}
CImageList* CImageList::FromHandlePermanent(HIMAGELIST hImageList) {
    return impl__FromHandlePermanent_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(hImageList);
}
void CImageList::DeleteTempMap() {
    impl__DeleteTempMap_CImageList__SAXXZ();
}
int CImageList::Add(HBITMAP hbmImage, HBITMAP hbmMask) {
    if (!m_hImageList || !hbmImage) return -1;
    return ::ImageList_Add(m_hImageList, hbmImage, hbmMask);
}
int CImageList::Add(HBITMAP hbmImage, COLORREF crMask) {
    if (!m_hImageList || !hbmImage) return -1;
    return ::ImageList_AddMasked(m_hImageList, hbmImage, crMask);
}
int CImageList::Add(HICON hIcon) {
    if (!m_hImageList || !hIcon) return -1;
    return ::ImageList_AddIcon(m_hImageList, hIcon);
}
int CImageList::Replace(int nImage, HBITMAP hbmImage, HBITMAP hbmMask) {
    if (!m_hImageList || !hbmImage) return FALSE;
    return ::ImageList_Replace(m_hImageList, nImage, hbmImage, hbmMask);
}
int CImageList::Replace(int nImage, HICON hIcon) {
    if (!m_hImageList || !hIcon) return -1;
    return ::ImageList_ReplaceIcon(m_hImageList, nImage, hIcon);
}
int CImageList::Remove(int nImage) {
    if (!m_hImageList) return FALSE;
    return ::ImageList_Remove(m_hImageList, nImage);
}
int CImageList::RemoveAll() {
    if (!m_hImageList) return FALSE;
    return ::ImageList_Remove(m_hImageList, -1);
}
HICON CImageList::ExtractIcon(int nImage) {
    if (!m_hImageList) return nullptr;
    return ::ImageList_GetIcon(m_hImageList, nImage, ILD_TRANSPARENT);
}
int CImageList::GetImageCount() const {
    if (!m_hImageList) return 0;
    return ::ImageList_GetImageCount(m_hImageList);
}
int CImageList::GetImageInfo(int nImage, IMAGEINFO* pImageInfo) const {
    if (!m_hImageList || !pImageInfo) return FALSE;
    return ::ImageList_GetImageInfo(m_hImageList, nImage, pImageInfo);
}
COLORREF CImageList::GetBkColor() const {
    if (!m_hImageList) return CLR_NONE;
    return ::ImageList_GetBkColor(m_hImageList);
}
COLORREF CImageList::SetBkColor(COLORREF cr) {
    if (!m_hImageList) return CLR_NONE;
    return ::ImageList_SetBkColor(m_hImageList, cr);
}
int CImageList::Draw(CDC* pDC, int nImage, POINT pt, UINT nStyle) {
    if (!m_hImageList || !pDC || !pDC->m_hDC) return FALSE;
    return ::ImageList_Draw(m_hImageList, nImage, pDC->m_hDC, pt.x, pt.y, nStyle);
}
int CImageList::DrawEx(CDC* pDC, int nImage, POINT pt, SIZE sz,
                       COLORREF clrBk, COLORREF clrFg, UINT nStyle) {
    if (!m_hImageList || !pDC || !pDC->m_hDC) return FALSE;
    return ::ImageList_DrawEx(m_hImageList, nImage, pDC->m_hDC,
                               pt.x, pt.y, sz.cx, sz.cy, clrBk, clrFg, nStyle);
}
int CImageList::DrawIndirect(IMAGELISTDRAWPARAMS* pimldp) {
    return impl__DrawIndirect_CImageList__QEAAHPEAU_IMAGELISTDRAWPARAMS___Z(this, pimldp);
}
int CImageList::DrawIndirect(CDC* pDC, int nImage, POINT pt, SIZE sz,
                              POINT ptOrigin, UINT fStyle, COLORREF rgbFg,
                              COLORREF rgbBk, COLORREF rgbBlend1, COLORREF rgbBlend2,
                              DWORD fState, DWORD Frame, COLORREF crEffect) {
    return impl__DrawIndirect_CImageList__QEAAHPEAVCDC__HUtagPOINT__UtagSIZE__1IKKKKKK_Z(
        this, pDC, nImage, pt, sz, ptOrigin, fStyle, rgbFg, rgbBk, rgbBlend1, rgbBlend2,
        fState, Frame, crEffect);
}
int CImageList::BeginDrag(int nImage, CPoint ptHotSpot) {
    if (!m_hImageList) return FALSE;
    return ::ImageList_BeginDrag(m_hImageList, nImage, ptHotSpot.x, ptHotSpot.y);
}
void CImageList::EndDrag() {
    ::ImageList_EndDrag();
}
int CImageList::DragMove(CPoint pt) {
    return ::ImageList_DragMove(pt.x, pt.y);
}
int CImageList::DragShowNolock(int bShow) {
    return ::ImageList_DragShowNolock(bShow);
}
CImageList* CImageList::GetDragImage(LPPOINT lpPoint, LPPOINT lpPointHotSpot) {
    HIMAGELIST h = ::ImageList_GetDragImage(lpPoint, lpPointHotSpot);
    return h ? GetTempImageList(h) : nullptr;
}
int CImageList::DragEnter(CWnd* pWndLock, CPoint point) {
    HWND hWnd = pWndLock ? pWndLock->GetSafeHwnd() : nullptr;
    return ::ImageList_DragEnter(hWnd, point.x, point.y);
}
int CImageList::DragLeave(CWnd* pWndLock) {
    HWND hWnd = pWndLock ? pWndLock->GetSafeHwnd() : nullptr;
    return ::ImageList_DragLeave(hWnd);
}
int CImageList::Read(CArchive* pArchive) {
    return impl__Read_CImageList__QEAAHPEAVCArchive___Z(this, pArchive);
}
int CImageList::Write(CArchive* pArchive) {
    return impl__Write_CImageList__QEAAHPEAVCArchive___Z(this, pArchive);
}
