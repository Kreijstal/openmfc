// CMFCButton — OpenMFC implementation.
// Sources: global_cmfcbutton.cpp, global_mfc-feature-34_impl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CMFCButtonSupport.h"
#include "detail/MfcFeature34ImplSupport.h"
#include "detail/MfccoreSupport.h"

// OnSetCursor(CWnd*, UINT, UINT): when a custom cursor was installed, sets it
// while the pointer is over the button client area; otherwise defers to the
// base dispatch.  Transcribed from retail RVA 0x1e830.
// Symbol: ?OnSetCursor@CMFCButton@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CMFCButton__IEAAHPEAVCWnd__II_Z(
    void* pThis, void* /*pWnd*/, unsigned int /*nHitTest*/, unsigned int /*message*/)
{
    if (pThis == nullptr) return 0;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->m_hCursor != nullptr) {
        RECT rc;
        ::GetClientRect(s->m_hWnd, &rc);
        POINT pt;
        ::GetCursorPos(&pt);
        ::ScreenToClient(s->m_hWnd, &pt);
        if (::PtInRect(&rc, pt)) {
            ::SetCursor(s->m_hCursor);
            return 1;
        }
    }
    // retail tail-calls the CWnd base dispatch here
    return 0;
}
// OnSetFont(CFont*, BOOL): caches the CFont::m_hObject handle (offset 8) into
// the 0xb00 slot and redraws on request.  Retail RVA 0x1eb20.
// Symbol: ?OnSetFont@CMFCButton@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCButton__IEAAXPEAVCFont__H_Z(
    void* pThis, void* pFont, int bRedraw)
{
    if (pThis == nullptr) return;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    s->pMenuFont = (pFont != nullptr) ? *reinterpret_cast<void**>(reinterpret_cast<char*>(pFont) + 8)
                                      : nullptr;
    if (bRedraw != 0 && s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// OnSetImage(WPARAM, LPARAM): routes the bitmap (wParam==0) or icon
// (wParam==1) handle in lParam into the matching SetImageInternal overload.
// Retail RVA 0x1f410.
// Symbol: ?OnSetImage@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnSetImage_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long lParam)
{
    if (pThis == nullptr) return 0;
    if (wParam == 0) {
        impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
            pThis, reinterpret_cast<void*>(lParam), 0, 0, 1, 0, 0);
    } else if (wParam == 1) {
        impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
            pThis, reinterpret_cast<void*>(lParam), 0, 0, 0, 0, 0);
    }
    return 0;
}
// OnSize(UINT, int, int): copies the focus-draw flag pair, then defers to the
// base dispatch.  Retail RVA 0x1ec10.
// Symbol: ?OnSize@CMFCButton@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCButton__IEAAXIHH_Z(
    void* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/)
{
    if (pThis == nullptr) return;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    s->m_bDrawFocusB = s->m_bDrawFocusA;
    // retail tail-calls the base OnSize dispatch here
}
// OnTimer(UINT_PTR): while the autorepeat timer fires over a clicked/hovered
// radio-style button, repeats the BN_CLICKED-style WM_COMMAND to the parent.
// Retail RVA 0x1f0e0.
// Symbol: ?OnTimer@CMFCButton@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCButton__IEAAX_K_Z(
    void* pThis, unsigned long long nIDEvent)
{
    if (pThis == nullptr) return;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (nIDEvent == 0xec0d && s->m_bHover != 0 && s->m_bClickStarted != 0) {
        SendButtonCommandToParent(pThis);
    }
    // retail tail-calls the CWnd base dispatch here
}
// OnUpdateToolTips(WPARAM, LPARAM): relays the mouse state to the tooltip
// helper.  The retail body (RVA 0x1f4f0) drives the tooltip-manager global
// (0x1803b25e8) which is not modeled; the guard checks are kept.
// Symbol: ?OnUpdateToolTips@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnUpdateToolTips_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->pHelperWnd != nullptr) {
        HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
        if (hwndHelper != nullptr && (wParam & 0x40) != 0) {
            // TODO(clean-room): tooltip-manager update not modeled.
        }
    }
    return 0;
}
// PreCreateWindow(CREATESTRUCTW&): decodes the button style and forces the
// BS_OWNERDRAW bit (style = (style & ~BS_PUSHBUTTON) | BS_OWNERDRAW).
// Retail RVA 0x1cd30.
// Symbol: ?PreCreateWindow@CMFCButton@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCButton__MEAAHAEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    if (pThis == nullptr || lpCreateStruct == nullptr) return FALSE;
    impl__InitStyle_CMFCButton__IEAAXK_Z(pThis, lpCreateStruct->style);
    lpCreateStruct->style = (lpCreateStruct->style & ~1u) | 0xA;
    // retail tail-calls the base PreCreateWindow, which returns TRUE
    return TRUE;
}
// PreSubclassWindow(): decodes the live window style, then forces
// BS_OWNERDRAW via ModifyStyle(1, 0xB, 0).  Retail RVA 0x1cd00.
// Symbol: ?PreSubclassWindow@CMFCButton@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCButton__MEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->m_hWnd == nullptr) return;
    LONG_PTR style = ::GetWindowLongPtrW(s->m_hWnd, GWL_STYLE);
    impl__InitStyle_CMFCButton__IEAAXK_Z(pThis, static_cast<unsigned long>(style));
    impl__ModifyStyle_CWnd__SAHPEAUHWND____KKI_Z(s->m_hWnd, 1, 0xB, 0);
}
// PreTranslateMessage(MSG*): relays mouse messages to the tooltip helper,
// turns VK_RETURN into a WM_COMMAND click, and handles the radio-group
// keyboard navigation (VK_SPACE toggles, arrow keys move the check).
// Transcribed from retail RVA 0x1e560.
// Symbol: ?PreTranslateMessage@CMFCButton@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCButton__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg)
{
    if (pThis == nullptr || pMsg == nullptr) return 0;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);

    // (a) Relay mouse messages to the tooltip helper.
    if (s->pHelperWnd != nullptr) {
        HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
        if (hwndHelper != nullptr &&
            pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST) {
            ::SendMessageW(hwndHelper, 0x407 /* TTM_RELAYEVENT */, 0,
                           reinterpret_cast<LPARAM>(pMsg));
        }
    }

    if (pMsg->message == WM_KEYDOWN) {
        // (b) VK_RETURN behaves like a click.  Retail gates this on the global
        // animation pointer (0x1803be288) being null; that global is not
        // modeled, so the path is always taken.
        if (pMsg->wParam == VK_RETURN) {
            HWND hParent = ::GetParent(s->m_hWnd);
            void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
            if (pParent != nullptr) {
                SendButtonCommandToParent(pThis);
                return 1;
            }
        }

        // (c) Auto-toggle keyboard navigation.
        if (s->m_bAutoToggle != 0) {
            HWND hParent = ::GetParent(s->m_hWnd);
            void* pParent = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);
            if (pParent != nullptr) {
                switch (pMsg->wParam) {
                case VK_SPACE:
                    if (s->m_bRadioStyle != 0) {
                        s->m_bChecked = (s->m_bChecked == 0) ? 1 : 0;
                        if (s->m_hWnd != nullptr) {
                            ::RedrawWindow(s->m_hWnd, nullptr, nullptr,
                                           RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
                        }
                        SendButtonCommandToParent(pThis);
                        return 1;
                    }
                    break;
                case VK_LEFT:
                case VK_UP:
                    if (impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(pThis, 0) != 0) {
                        return 1;
                    }
                    break;
                case VK_RIGHT:
                case VK_DOWN:
                    if (impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(pThis, 1) != 0) {
                        return 1;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    // retail tail-calls the base PreTranslateMessage here
    return 0;
}
// SelectFont(CDC*): validates the cached menu font and selects it into the
// DC (or the stock DEFAULT_GUI_FONT when none is set).  The retail body
// (RVA 0x1eaa0) returns the previously selected CFont* through the CDC
// SelectObject virtual; the previous font is not tracked here.
// Symbol: ?SelectFont@CMFCButton@@MEAAPEAVCFont@@PEAVCDC@@@Z
extern "C" void* MS_ABI impl__SelectFont_CMFCButton__MEAAPEAVCFont__PEAVCDC___Z(
    void* pThis, void* pDC)
{
    if (pThis == nullptr) return nullptr;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->pMenuFont != nullptr &&
        ::GetObjectType(reinterpret_cast<HGDIOBJ>(s->pMenuFont)) != OBJ_FONT) {
        s->pMenuFont = nullptr;
    }
    HFONT hFont = reinterpret_cast<HFONT>(s->pMenuFont);
    HDC hdc = hdc_of_Cmfcbutton(pDC);
    if (hdc != nullptr) {
        if (hFont == nullptr) {
            hFont = reinterpret_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
        }
        ::SelectObject(hdc, hFont);
    }
    // TODO(clean-room): previously selected CFont* not returned.
    return nullptr;
}
// SetAutorepeatMode(int): stores the autorepeat timer interval.  Retail RVA
// 0x1f0d0 (single store into m_nTimerID at 0x144).
// Symbol: ?SetAutorepeatMode@CMFCButton@@QEAAXH@Z
extern "C" void MS_ABI impl__SetAutorepeatMode_CMFCButton__QEAAXH_Z(
    void* pThis, int nMode)
{
    if (pThis == nullptr) return;
    reinterpret_cast<S_Cmfcbutton*>(pThis)->m_nTimerID = nMode;
}
// SetImageInternal(UINT, UINT, int, UINT): resource-ID image setter.  Retail
// (RVA 0x1dd00) loads three icons from the module resources through an
// internal helper and forwards them to the HBITMAP overload; the resource
// loads are not modeled, so the primary handle slot is cleared instead.
// Symbol: ?SetImageInternal@CMFCButton@@IEAAXIIHI@Z
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(
    void* pThis, unsigned int nID, unsigned int /*nImage*/,
    int /*bChecked*/, unsigned int /*nCheckedImage*/)
{
    if (pThis == nullptr) return;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (nID == 0) return;
    // TODO(clean-room): icon-from-resource loads (internal 0x18001dc10) and
    // image-object setup not modeled.
    ImageHandleSlot(s) = nullptr;
}
// SetImageInternal(HBITMAP, int, HBITMAP, int, int, HBITMAP): bitmap image
// setter.  Retail (RVA 0x1da00) calls an InitStyle-like helper and drives the
// image objects; the primary bitmap is kept in the first image object's
// handle slot (0x1f8) which OnGetImage reads back.
// Symbol: ?SetImageInternal@CMFCButton@@IEAAXPEAUHBITMAP__@@H0HH0@Z
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
    void* pThis, void* hBitmap, int /*bAutoDestroy*/, void* /*hBitmapHot*/,
    int /*bAutoDestroyHot*/, int /*bChecked*/, void* /*hBitmapChecked*/)
{
    if (pThis == nullptr) return;
    if (hBitmap != nullptr) {
        ImageHandleSlot(reinterpret_cast<S_Cmfcbutton*>(pThis)) = hBitmap;
        // TODO(clean-room): hot/checked image objects not modeled.
    }
}
// SetImageInternal(HICON, int, HICON, int, HICON, int): icon image setter.
// Retail (RVA 0x1d6f0) resolves the icon dimensions via GetIconInfo/GetObject
// and drives the image objects; the observable subset kept here is the icon
// handle slot plus the m_sizeImage (0x148/0x14c) dimensions.
// Symbol: ?SetImageInternal@CMFCButton@@IEAAXPEAUHICON__@@H0H0H@Z
extern "C" void MS_ABI impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
    void* pThis, void* hIcon, int /*bAutoDestroy*/, void* /*hIconHot*/,
    int /*bChecked*/, void* /*hIconChecked*/, int /*bAutoDestroyHot*/)
{
    if (pThis == nullptr) return;
    if (hIcon != nullptr) {
        S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
        ImageHandleSlot(s) = hIcon;
        ICONINFO ii;
        std::memset(&ii, 0, sizeof(ii));
        if (::GetIconInfo(reinterpret_cast<HICON>(hIcon), &ii)) {
            BITMAP bm;
            std::memset(&bm, 0, sizeof(bm));
            if (::GetObjectW(ii.hbmColor, sizeof(bm), &bm) != 0) {
                s->m_sizeImageCX = bm.bmWidth;
                s->m_sizeImageCY = bm.bmHeight;
            }
            if (ii.hbmColor != nullptr) ::DeleteObject(ii.hbmColor);
            if (ii.hbmMask != nullptr) ::DeleteObject(ii.hbmMask);
        }
        // TODO(clean-room): hot/checked image objects not modeled.
    }
}
// SetImage(UINT, UINT, UINT) / SetCheckedImage(UINT, UINT, UINT): route into
// the UINT SetImageInternal overload with bChecked = 0 / 1.  Retail RVAs
// 0x1d650 / 0x1d6d0.
// Symbol: ?SetImage@CMFCButton@@QEAAXIII@Z
extern "C" void MS_ABI impl__SetImage_CMFCButton__QEAAXIII_Z(
    void* pThis, unsigned int nID, unsigned int nImage, unsigned int nCheckedImage)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(pThis, nID, nImage, 0, nCheckedImage);
}
// Symbol: ?SetCheckedImage@CMFCButton@@QEAAXIII@Z
extern "C" void MS_ABI impl__SetCheckedImage_CMFCButton__QEAAXIII_Z(
    void* pThis, unsigned int nID, unsigned int nImage, unsigned int nCheckedImage)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXIIHI_Z(pThis, nID, nImage, 1, nCheckedImage);
}
// SetImage(HBITMAP, int, HBITMAP, int, HBITMAP) / checked variant: route into
// the HBITMAP SetImageInternal overload with bChecked = 0 / 1.  Retail RVAs
// 0x1d620 / 0x1d6a0.
// Symbol: ?SetImage@CMFCButton@@QEAAXPEAUHBITMAP__@@H0H0@Z
extern "C" void MS_ABI impl__SetImage_CMFCButton__QEAAXPEAUHBITMAP____H0H0_Z(
    void* pThis, void* hBitmap, int bAutoDestroy, void* hBitmapHot,
    int bAutoDestroyHot, void* hBitmapChecked)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
        pThis, hBitmap, bAutoDestroy, hBitmapHot, bAutoDestroyHot, 0, hBitmapChecked);
}
// Symbol: ?SetCheckedImage@CMFCButton@@QEAAXPEAUHBITMAP__@@H0H0@Z
extern "C" void MS_ABI impl__SetCheckedImage_CMFCButton__QEAAXPEAUHBITMAP____H0H0_Z(
    void* pThis, void* hBitmap, int bAutoDestroy, void* hBitmapHot,
    int bAutoDestroyHot, void* hBitmapChecked)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHBITMAP____H0HH0_Z(
        pThis, hBitmap, bAutoDestroy, hBitmapHot, bAutoDestroyHot, 1, hBitmapChecked);
}
// SetImage(HICON, int, HICON, int, HICON) / checked variant: route into the
// HICON SetImageInternal overload with bChecked = 0 / 1.  Retail RVAs
// 0x1d5f0 / 0x1d670.  Note the retail parameter order is
// (hIcon, bAutoDestroy, hIconHot, hIconChecked, bAutoDestroyHot).
// Symbol: ?SetImage@CMFCButton@@QEAAXPEAUHICON__@@H00H@Z
extern "C" void MS_ABI impl__SetImage_CMFCButton__QEAAXPEAUHICON____H00H_Z(
    void* pThis, void* hIcon, int bAutoDestroy, void* hIconHot,
    void* hIconChecked, int bAutoDestroyHot)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
        pThis, hIcon, bAutoDestroy, hIconHot, 0, hIconChecked, bAutoDestroyHot);
}
// Symbol: ?SetCheckedImage@CMFCButton@@QEAAXPEAUHICON__@@H00H@Z
extern "C" void MS_ABI impl__SetCheckedImage_CMFCButton__QEAAXPEAUHICON____H00H_Z(
    void* pThis, void* hIcon, int bAutoDestroy, void* hIconHot,
    void* hIconChecked, int bAutoDestroyHot)
{
    if (pThis == nullptr) return;
    impl__SetImageInternal_CMFCButton__IEAAXPEAUHICON____H0H0H_Z(
        pThis, hIcon, bAutoDestroy, hIconHot, 1, hIconChecked, bAutoDestroyHot);
}
// SetMouseCursor(HICON): installs a custom cursor.  Retail RVA 0x1e7d0
// (single store into m_hCursor at 0xb08).
// Symbol: ?SetMouseCursor@CMFCButton@@QEAAXPEAUHICON__@@@Z
extern "C" void MS_ABI impl__SetMouseCursor_CMFCButton__QEAAXPEAUHICON_____Z(
    void* pThis, void* hCursor)
{
    if (pThis == nullptr) return;
    reinterpret_cast<S_Cmfcbutton*>(pThis)->m_hCursor = reinterpret_cast<HICON>(hCursor);
}
// SetMouseCursorHand(): installs the standard hand cursor.  Retail
// (RVA 0x1e7e0) takes it from a cached visual-manager cursor; LoadCursorW
// with IDC_HAND is the public equivalent.
// Symbol: ?SetMouseCursorHand@CMFCButton@@QEAAXXZ
extern "C" void MS_ABI impl__SetMouseCursorHand_CMFCButton__QEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): uses LoadCursorW(IDC_HAND) instead of the cached
    // visual-manager cursor (0x18006c790).
    reinterpret_cast<S_Cmfcbutton*>(pThis)->m_hCursor =
        ::LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_HAND));
}
// SetStdImage(CMenuImages::IMAGES_IDS, IMAGE_STATE, IMAGE_STATE): stores the
// standard-image id/state triplet.  Retail (RVA 0x1dd80) also fills the
// 8-byte m_sizeImage slot from CMenuImages::GetImage (internal 0x18008f600)
// and a vtable-side-effect call; the image lookup is not modeled, so the size
// slot is cleared.
// Symbol: ?SetStdImage@CMFCButton@@QEAAXW4IMAGES_IDS@CMenuImages@@W4IMAGE_STATE@3@0@Z
extern "C" void MS_ABI impl__SetStdImage_CMFCButton__QEAAXW4IMAGES_IDS_CMenuImages__W4IMAGE_STATE_3_0_Z(
    void* pThis, int nImageId, int nState, int nStateHot)
{
    if (pThis == nullptr) return;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    // TODO(clean-room): CMenuImages::GetImage lookup not modeled.
    s->m_sizeImageCX = 0;
    s->m_sizeImageCY = 0;
    s->m_nImageIndex = nImageId;          // 0xae8
    s->m_nCheckedImageIndex = nStateHot;  // 0xaec
    s->m_nState = nState;                 // 0xaf0
}
// SizeToContent(BOOL): computes the size the button needs for its text and
// image plus the fixed margins (GetImageHorzMargin = 10, GetVertMargin = 5),
// and resizes the window when bCalcOnly is FALSE.  The retail body
// (RVA 0x1e280) measures the text through internal font/CString helpers; a
// DrawTextW DT_CALCRECT pass is the equivalent.  CSize is returned through
// the hidden return slot (this in %rcx, slot in %rdx).
// Symbol: ?SizeToContent@CMFCButton@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly)
{
    if (!pRet) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->m_hWnd == nullptr) return pRet;

    // Text extent using the cached menu font (or the stock default GUI font).
    int textW = 0;
    int textH = 0;
    wchar_t buf[1024];
    const int nLen = ::GetWindowTextW(s->m_hWnd, buf, 1024);
    HDC hdc = ::GetDC(s->m_hWnd);
    if (hdc != nullptr) {
        HFONT hFont = reinterpret_cast<HFONT>(s->pMenuFont);
        if (hFont == nullptr) {
            hFont = reinterpret_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
        }
        HGDIOBJ hOld = ::SelectObject(hdc, hFont);
        RECT rc = {0, 0, 0, 0};
        if (nLen > 0) {
            ::DrawTextW(hdc, buf, nLen, &rc, DT_CALCRECT | DT_SINGLELINE);
            textW = rc.right - rc.left;
            textH = rc.bottom - rc.top;
        }
        ::SelectObject(hdc, hOld);
        ::ReleaseDC(s->m_hWnd, hdc);
    }

    const int iconW = s->m_sizeImageCX;
    const int iconH = s->m_sizeImageCY;
    const int marginX = 10; // GetImageHorzMargin
    const int marginY = 5;  // GetVertMargin
    if (s->m_bImageIsTiled != 0) {
        // Tiled-image layout: icon sits beside the text.
        pRet->cx = iconW + marginX + textW + (textW > 0 ? marginX : 0);
        pRet->cy = 2 * marginY + (textH > iconH ? textH : iconH);
    } else {
        pRet->cx = marginX + (textW > iconW ? textW : iconW) + (textW > 0 ? marginX : 0);
        pRet->cy = iconH + marginY + textH + (iconH > 0 ? marginY : 0) +
                   (textH > 0 ? marginY : 0);
    }

    if (bCalcOnly == 0) {
        ::SetWindowPos(s->m_hWnd, nullptr, 0, 0, pRet->cx, pRet->cy,
                       SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
    }
    return pRet;
}
// UncheckRadioButtonsInGroup(): unchecks every checked CMFCButton radio
// sibling in the WS_GROUP, removing WS_TABSTOP from them, then restores
// WS_TABSTOP on `this`.  Transcribed from retail RVA 0x1efb0 (same walk as
// the private helper in the feature-34 batch, reimplemented here).
// Symbol: ?UncheckRadioButtonsInGroup@CMFCButton@@IEAAXXZ
extern "C" void MS_ABI impl__UncheckRadioButtonsInGroup_CMFCButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    UncheckRadioButtonsInGroupImpl_Cmfcbutton(reinterpret_cast<S_Cmfcbutton*>(pThis));
}
// get_accState(tagVARIANT, VARIANT*): MSAA state for the child variant.
// Retail (RVA 0x1fb00) resolves the child index through an internal
// VariantChangeType helper that fills *pvarVal, then ORs the button state
// bits into pvarVal->lVal: STATE_SYSTEM_FOCUSED (0x80) while a click is in
// progress, and for a checked button STATE_SYSTEM_SELECTED (0x2) when
// m_bCheckStyle is set, else STATE_SYSTEM_CHECKED (0x10).
// Symbol: ?get_accState@CMFCButton@@UEAAJUtagVARIANT@@PEAU2@@Z
extern "C" long MS_ABI impl__get_accState_CMFCButton__UEAAJUtagVARIANT__PEAU2__Z(
    void* pThis, const VARIANT& varChild, VARIANT* pvarVal)
{
    if (pvarVal == nullptr) return E_INVALIDARG;
    if (varChild.vt != VT_I4) {
        // TODO(clean-room): retail converts the child variant through an
        // internal VariantChangeType helper (0x180290620); only VT_I4 is
        // modeled.
        return E_INVALIDARG;
    }
    pvarVal->vt = VT_I4;
    pvarVal->lVal = varChild.lVal;
    if (pThis == nullptr) return S_OK;
    S_Cmfcbutton* s = reinterpret_cast<S_Cmfcbutton*>(pThis);
    if (s->m_bClickStarted != 0) pvarVal->lVal |= 0x80;
    if (s->m_bChecked != 0) {
        if (s->m_bCheckStyle != 0) pvarVal->lVal |= 0x2;   // STATE_SYSTEM_SELECTED
        else pvarVal->lVal |= 0x10;                        // STATE_SYSTEM_CHECKED
    }
    return S_OK;
}
// OnEraseBkgnd always reports the background as erased (the button paints
// everything itself through the draw virtuals).
// Symbol: ?OnEraseBkgnd@CMFCButton@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCButton__IEAAHPEAVCDC___Z(
    void* /*pThis*/, void* /*pDC*/)
{
    return 1;
}
// OnGetFont returns the font handle stored in the CString slot at 0xb00.
// Symbol: ?OnGetFont@CMFCButton@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CMFCButton__IEAAPEAUHFONT____XZ(
    void* pThis)
{
    if (pThis == nullptr) return nullptr;
    return reinterpret_cast<S_MfcFeature34Impl*>(pThis)->pMenuFont;
}
// GetImageHorzMargin: fixed 10-pixel horizontal margin.
// Symbol: ?GetImageHorzMargin@CMFCButton@@MEBAHXZ
extern "C" int MS_ABI impl__GetImageHorzMargin_CMFCButton__MEBAHXZ(
    void* /*pThis*/)
{
    return 10;
}
// GetVertMargin: fixed 5-pixel vertical margin.
// Symbol: ?GetVertMargin@CMFCButton@@MEBAHXZ
extern "C" int MS_ABI impl__GetVertMargin_CMFCButton__MEBAHXZ(
    void* /*pThis*/)
{
    return 5;
}
// OnGetCheck returns the check state only for radio/check buttons.
// Symbol: ?OnGetCheck@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnGetCheck_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long /*wParam*/, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    if (s->m_bRadioStyle == 0 && s->m_bCheckStyle == 0) return 0;
    return s->m_bChecked != 0 ? 1 : 0;
}
// EnableWindowsTheming(int): static member, stores into the class static
// m_bWinXPTheme (defined in feature_static_data.cpp).
// Symbol: ?EnableWindowsTheming@CMFCButton@@SAXH@Z
extern "C" void MS_ABI impl__EnableWindowsTheming_CMFCButton__SAXH_Z(
    int bEnable)
{
    impl__m_bWinXPTheme_CMFCButton__1HA = bEnable;
}
// InitStyle(UINT): decodes the button style bits into the radio/check/toggle
// flags and the flat-style, and lazily probes whether WinXP theming is
// available (once).
// Symbol: ?InitStyle@CMFCButton@@IEAAXK@Z
extern "C" void MS_ABI impl__InitStyle_CMFCButton__IEAAXK_Z(
    void* pThis, unsigned long dwStyle)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);

    switch (dwStyle & 0xf) {
    case 0x2: // BS_CHECKBOX
        s->m_bRadioStyle = 1;
        break;
    case 0x3: // BS_AUTOCHECKBOX
        s->m_bAutoToggle = 1;
        break;
    case 0x4: // BS_RADIOBUTTON
        s->m_bAutoToggle = 1;
        s->m_bCheckStyle = 1;
        break;
    case 0x9: // BS_AUTORADIOBUTTON
        s->m_bAutoToggle = 1;
        s->m_bCheckStyle = 1;
        break;
    default:
        break;
    }

    if (s->m_bRadioStyle != 0 || s->m_bCheckStyle != 0) {
        switch (dwStyle & 0x300) {
        case 0x100: s->m_nFlatStyle = 0; break; // BS_LEFT
        case 0x200: s->m_nFlatStyle = 1; break; // BS_RIGHT
        case 0x300: s->m_nFlatStyle = 2; break; // BS_CENTER
        default: break;
        }
    }

    // One-time WinXP-theme availability probe (retail RVA 0x1cdf7 loads
    // "uxtheme"-style support via an internal helper; GetProcAddress probe
    // is the equivalent).
    // TODO(clean-room): approximated by probing IsAppThemed directly.
    if (impl__m_bWinXPThemeWasChecked_CMFCButton__1HA == 0) {
        if (impl__m_bWinXPTheme_CMFCButton__1HA == 0) {
            int themed = 0;
            HMODULE hUx = ::LoadLibraryW(L"uxtheme.dll");
            if (hUx != nullptr) {
                FARPROC pIsAppThemed = ::GetProcAddress(hUx, "IsAppThemed");
                if (pIsAppThemed != nullptr) {
                    typedef BOOL(WINAPI *IsAppThemedFn)();
                    themed = (reinterpret_cast<IsAppThemedFn>(pIsAppThemed)() != FALSE) ? 1 : 0;
                }
            }
            impl__m_bWinXPTheme_CMFCButton__1HA = themed;
        }
        impl__m_bWinXPThemeWasChecked_CMFCButton__1HA = 1;
    }
}
// EnableMenuFont(BOOL, BOOL): stores the shared menu font (or NULL when
// disabled) into the 0xb00 slot and redraws when requested.
// Symbol: ?EnableMenuFont@CMFCButton@@QEAAXHH@Z
extern "C" void MS_ABI impl__EnableMenuFont_CMFCButton__QEAAXHH_Z(
    void* pThis, int bEnable, int bRedraw)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    if (bEnable != 0) {
        // Retail stores the process-global MFC menu font (0x1803c17d0).
        // DEFAULT_GUI_FONT is the closest public equivalent.
        // TODO(clean-room): uses DEFAULT_GUI_FONT instead of the MFC global.
        s->pMenuFont = reinterpret_cast<void*>(::GetStockObject(DEFAULT_GUI_FONT));
    } else {
        s->pMenuFont = nullptr;
    }
    if (bRedraw != 0) {
        HWND hwnd = s->m_hWnd;
        if (hwnd != nullptr) {
            ::InvalidateRect(hwnd, nullptr, TRUE);
            ::UpdateWindow(hwnd);
        }
    }
}
// OnSetCheck(WPARAM, LPARAM): mirrors OnGetCheck; updates m_bChecked when the
// requested state differs from the current one and unchecks the radio group.
// Symbol: ?OnSetCheck@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnSetCheck_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    if (s->m_bRadioStyle == 0 && s->m_bCheckStyle == 0) return 0;

    const int bNewCheck = (wParam != 0) ? 1 : 0;
    if ((bNewCheck == 0) == (s->m_bChecked == 0)) {
        return 0; // state unchanged
    }
    s->m_bChecked = bNewCheck;
    if (s->m_bCheckStyle != 0) {
        UncheckRadioButtonsInGroupImpl_MfcFeature34Impl(s);
    }
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
    return 0;
}
// OnGetImage(WPARAM, LPARAM): wParam==0 returns the first image object's
// bitmap cell (0x1f8); wParam==1 returns the icon from the image object.
// Symbol: ?OnGetImage@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnGetImage_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    if (wParam == 0) {
        // m_Images[0] + 0xa0 holds the bitmap handle.
        return static_cast<long long>(reinterpret_cast<long long>(
            *reinterpret_cast<void**>(reinterpret_cast<char*>(pThis) + 0x1f8)));
    }
    if (wParam == 1) {
        // Retail: m_Images[0].GetImage(0) -> HICON.  Image objects are not
        // modeled in this codebase.
        // TODO(clean-room): image-object GetImage not modeled.
        return 0;
    }
    return 0;
}
// CleanUp(): virtual cleanup -- resets the icon, the image indices and the six
// 408-byte image objects.
// Symbol: ?CleanUp@CMFCButton@@UEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCButton__UEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    s->m_hIcon = nullptr;
    s->m_nImageIndex = -1;
    s->m_nCheckedImageIndex = -1;
    // Retail runs the image-object reset ctor (0x18016f690) on all six.
    // TODO(clean-room): image objects are zeroed instead of reconstructed.
    for (int i = 0; i < 6; ++i) {
        std::memset(s->m_Images[i], 0, 0x198);
    }
}
// ClearImages(BOOL): like CleanUp but clears the first three image objects
// (or the last three when bClearImages == FALSE) plus the icon state.
// Symbol: ?ClearImages@CMFCButton@@IEAAXH@Z
extern "C" void MS_ABI impl__ClearImages_CMFCButton__IEAAXH_Z(
    void* pThis, int bClearImages)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    s->m_hIcon = nullptr;
    s->m_nImageIndex = -1;
    s->m_nCheckedImageIndex = -1;
    // TODO(clean-room): image objects are zeroed instead of reconstructed.
    const int first = (bClearImages != 0) ? 0 : 3;
    for (int i = 0; i < 3; ++i) {
        std::memset(s->m_Images[first + i], 0, 0x198);
    }
}
// OnMouseMove(UINT, CPoint): hot-tracking -- highlights the button while the
// pointer is inside and the left button is down, captures the mouse, and
// clears the states when the pointer leaves.
// Symbol: ?OnMouseMove@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    s->m_bHighlighted = 0;

    const bool bLeftDown = (nFlags & MK_LBUTTON) != 0;
    const bool bNeedTracking =
        bLeftDown || s->m_nStyle != 0 || impl__m_bWinXPTheme_CMFCButton__1HA != 0 ||
        s->m_bTransparent != 0;
    if (!bNeedTracking) {
        return; // retail tail-calls the CWnd base dispatch here
    }
    HWND hwnd = s->m_hWnd;
    if (hwnd == nullptr) return;

    RECT clientRect;
    std::memset(&clientRect, 0, sizeof(clientRect));
    ::GetClientRect(hwnd, &clientRect);

    POINT ptScreen = point;
    ::ClientToScreen(hwnd, &ptScreen);

    bool changed = false;
    if (::PtInRect(&clientRect, point)) {
        HWND hwndUnder = ::WindowFromPoint(ptScreen);
        void* pUnder = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndUnder);
        HWND hwndOf = (pUnder != nullptr)
                          ? *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pUnder) + 0x40)
                          : nullptr;
        if (hwndOf != hwnd) {
            goto not_over_button;
        }
        s->m_bHighlighted = 1;
        if (s->m_bClickStarted == 0) {
            s->m_bClickStarted = 1;
            changed = true;
        }
        if (bLeftDown) {
            if (s->m_bHover == 0 && s->m_bPress != 0) {
                s->m_bHover = 1;
                changed = true;
            }
        }
        if (s->m_bCapture == 0) {
            ::SetCapture(hwnd);
            s->m_bCapture = 1;
            changed = true;
        }
    } else {
not_over_button:
        if (bLeftDown) {
            if (s->m_bHover != 0) {
                s->m_bHover = 0;
                changed = true;
            }
        } else {
            if (s->m_bClickStarted != 0) {
                s->m_bClickStarted = 0;
                changed = true;
            }
        }
        if (s->m_bCapture != 0 && !bLeftDown) {
            ::ReleaseCapture();
            s->m_bCapture = 0;
        }
    }
    if (changed) {
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
    }
    // retail tail-calls the CWnd base dispatch here
}
// OnLButtonDown(UINT, CPoint): for multiline buttons forwards the press to the
// parent as WM_COMMAND; otherwise records pressed state, captures the mouse and
// starts the autorepeat timer.
// Symbol: ?OnLButtonDown@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    HWND hwnd = s->m_hWnd;
    if (hwnd == nullptr) return;

    if (s->m_bMultiline != 0) {
        HWND hParent = ::GetParent(hwnd);
        if (hParent != nullptr) {
            WORD id = static_cast<WORD>(::GetDlgCtrlID(hwnd));
            ::SendMessageW(hParent, WM_COMMAND, MAKEWPARAM(id, 0), reinterpret_cast<LPARAM>(hwnd));
        }
    } else {
        s->m_bHover = 1;
        s->m_bPress = 1;
        s->m_bClickStarted = 1;
        if (s->m_bCapture == 0) {
            ::SetCapture(hwnd);
            s->m_bCapture = 1;
        }
        ::InvalidateRect(hwnd, nullptr, TRUE);
        ::UpdateWindow(hwnd);
        if (s->m_nTimerID > 0) {
            ::SetTimer(hwnd, 0xec0d, static_cast<UINT>(s->m_nTimerID), nullptr);
        }
    }
    // retail tail-calls the CWnd base dispatch here
}
// OnLButtonUp(UINT, CPoint): completes the click -- toggles radio/check state,
// sends BN_CLICKED, releases the capture, kills the autorepeat timer and pops
// the tooltip.
// Symbol: ?OnLButtonUp@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    HWND hwnd = s->m_hWnd;
    if (hwnd == nullptr) return;

    const bool bClickedInside =
        s->m_bHover != 0 && s->m_bPress != 0 && s->m_bClickStarted != 0;
    s->m_bHover = 0;
    s->m_bPress = 0;
    s->m_bClickStarted = 0;

    if (bClickedInside) {
        if (s->m_bAutoToggle != 0) {
            if (s->m_bRadioStyle != 0) {
                s->m_bChecked = (s->m_bChecked == 0) ? 1 : 0; // toggle
            } else if (s->m_bCheckStyle != 0 && s->m_bChecked == 0) {
                s->m_bChecked = 1;
                UncheckRadioButtonsInGroupImpl_MfcFeature34Impl(s);
            }
        }
    }

    if (s->m_bPushed != 0) {
        s->m_bPushed = 0;
        HWND hParent = ::GetParent(hwnd);
        if (hParent != nullptr) {
            WORD id = static_cast<WORD>(::GetDlgCtrlID(hwnd));
            ::SendMessageW(hParent, WM_COMMAND, MAKEWPARAM(id, BN_CLICKED),
                           reinterpret_cast<LPARAM>(hwnd));
        }
    }

    if (::IsWindow(hwnd)) {
        ::RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
        // retail tail-calls the CWnd base dispatch here
        if (::IsWindow(hwnd)) {
            if (s->m_bCapture != 0) {
                ::ReleaseCapture();
                s->m_bCapture = 0;
            }
            if (s->m_nTimerID > 0) {
                ::KillTimer(hwnd, 0xec0d);
            }
            if (s->pHelperWnd != nullptr) {
                HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
                if (hwndHelper != nullptr) {
                    ::SendMessageW(hwndHelper, 0x41c, 0, 0); // TTM_POP
                }
            }
        }
    }
}
// OnLButtonDblClk(UINT, CPoint): treats a double-click like a press (sets the
// pressed/hover/click states, redraws) and marks the button as pushed.
// Symbol: ?OnLButtonDblClk@CMFCButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    s->m_bHover = 1;
    s->m_bPress = 1;
    s->m_bClickStarted = 1;
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
    // retail tail-calls the CWnd base dispatch here
    s->m_bPushed = 1;
}
// OnCancelMode(): releases the capture (if held), resets the pressed/hover
// states, redraws and kills the autorepeat timer.
// Symbol: ?OnCancelMode@CMFCButton@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    // retail first tail-calls the CWnd base dispatch
    if (s->m_bCapture != 0) {
        ::ReleaseCapture();
        s->m_bClickStarted = 0;
        s->m_bCapture = 0;
        s->m_bHover = 0;
        s->m_bPress = 0;
        s->m_bHighlighted = 0;
        if (s->m_hWnd != nullptr) {
            ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
            ::UpdateWindow(s->m_hWnd);
        }
    }
    if (s->m_nTimerID >= 0) {
        ::KillTimer(s->m_hWnd, 0xec0d);
    }
}
// OnKillFocus(CWnd*): releases the capture, resets the pressed/hover states and
// redraws.
// Symbol: ?OnKillFocus@CMFCButton@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCButton__IEAAXPEAVCWnd___Z(
    void* pThis, void* /*pNewWnd*/)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    // retail first tail-calls the CWnd base dispatch
    if (s->m_bCapture != 0) {
        ::ReleaseCapture();
        s->m_bCapture = 0;
    }
    s->m_bHover = 0;
    s->m_bPress = 0;
    s->m_bClickStarted = 0;
    s->m_bHighlighted = 0;
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// OnEnable(BOOL): when disabling, resets the pressed states and capture; always
// redraws.
// Symbol: ?OnEnable@CMFCButton@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CMFCButton__IEAAXH_Z(
    void* pThis, int bEnable)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    if (bEnable == 0) {
        s->m_bHover = 0;
        s->m_bPress = 0;
        s->m_bClickStarted = 0;
        if (s->m_bCapture != 0) {
            ::ReleaseCapture();
            s->m_bCapture = 0;
        }
    }
    if (s->m_hWnd != nullptr) {
        ::RedrawWindow(s->m_hWnd, nullptr, nullptr,
                       RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
    // retail tail-calls the CWnd base dispatch here
}
// CheckNextPrevRadioButton(BOOL): walks the radio group with
// GetNextDlgGroupItem, finds the next visible/enabled check-style sibling,
// checks it and notifies the parent.
// Symbol: ?CheckNextPrevRadioButton@CMFCButton@@IEAAHH@Z
extern "C" int MS_ABI impl__CheckNextPrevRadioButton_CMFCButton__IEAAHH_Z(
    void* pThis, int bNext)
{
    if (pThis == nullptr) return 0;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    if (s->m_bCheckStyle == 0) return 0;
    HWND hwndThis = s->m_hWnd;
    if (hwndThis == nullptr) return 0;
    HWND hParent = ::GetParent(hwndThis);

    HWND hwndCurrent = hwndThis;
    for (;;) {
        HWND hwndNext = ::GetNextDlgGroupItem(hParent, hwndCurrent,
                                              (bNext != 0) ? FALSE : TRUE);
        if (hwndNext == nullptr || hwndNext == hwndThis) return 0;
        void* pNext = impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(hwndNext);
        if (pNext == pThis) return 0;
        if (pNext != nullptr &&
            *reinterpret_cast<int*>(reinterpret_cast<char*>(pNext) + 0x130) != 0) {
            // A check-style CMFCButton sibling; require it to be visible and
            // enabled (retail: (style & 0x18000000) == WS_VISIBLE).
            LONG_PTR style = ::GetWindowLongPtrW(hwndNext, GWL_STYLE);
            if ((style & 0x18000000L) == WS_VISIBLE) {
                if (*reinterpret_cast<int*>(reinterpret_cast<char*>(pNext) + 0x128) != 0) {
                    return 0; // already checked
                }
                ::SendMessageW(hwndNext, BM_SETCHECK, BST_CHECKED, 1);
                WORD id = static_cast<WORD>(::GetWindowLongPtrW(hwndNext, GWL_ID));
                // Retail resolves the parent CWnd via FromHandlePermanent and
                // sends WM_COMMAND to its m_hWnd (== hParent).
                ::SendMessageW(hParent, WM_COMMAND, MAKEWPARAM(id, BN_CLICKED),
                               reinterpret_cast<LPARAM>(hwndNext));
                return 1;
            }
        }
        hwndCurrent = hwndNext;
    }
}
// OnDrawFocusRect(CDC*, const CRect&): deflates the rect, fills it with the
// focus color (unless themed/transparent) and draws the focus rectangle.
// Symbol: ?OnDrawFocusRect@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rect)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    HDC hdc = hdc_of_MfcFeature34Impl(pDC);
    if (hdc == nullptr) return;

    RECT r = *rect;
    ::InflateRect(&r, -1, -1);

    COLORREF clrFocus = (s->m_nFocusColor != -1)
                            ? static_cast<COLORREF>(s->m_nFocusColor)
                            : static_cast<COLORREF>(::GetSysColor(COLOR_BTNFACE));
    // TODO(clean-room): retail reads the color from the global state singleton
    // (0x1803c1648) when m_nFocusColor == -1.

    const bool bThemedTransparent =
        impl__m_bWinXPTheme_CMFCButton__1HA != 0 && s->m_bTransparent != 0;
    if (!bThemedTransparent) {
        ::InflateRect(&r, -1, -1);
        HBRUSH hbr = ::CreateSolidBrush(clrFocus);
        ::FillRect(hdc, &r, hbr);
        ::DeleteObject(hbr);
    }
    ::DrawFocusRect(hdc, &r);
}
// OnDrawParentBackground(CDC*, CRect): fills the rect with the parent/button
// face background.  Retail forwards to the global visual-manager helper; that
// global is not modeled, so the button-face brush is used instead.
// Symbol: ?OnDrawParentBackground@CMFCButton@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void MS_ABI impl__OnDrawParentBackground_CMFCButton__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, void* pDC, RECT rect)
{
    (void)pThis;
    if (pDC == nullptr) return;
    HDC hdc = hdc_of_MfcFeature34Impl(pDC);
    if (hdc == nullptr) return;
    // TODO(clean-room): global visual-manager OnDrawParentBackground not modeled.
    ::FillRect(hdc, &rect, ::GetSysColorBrush(COLOR_BTNFACE));
}
// OnFillBackground(CDC*, const CRect&): fills the button face; when checked,
// draws a highlight frame around the interior.
// Symbol: ?OnFillBackground@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    HDC hdc = hdc_of_MfcFeature34Impl(pDC);
    if (hdc == nullptr) return;

    if (s->m_bMenuFont != 0) {
        // Retail forwards to the global visual-manager parent-background
        // helper (0x18006bbd0); not modeled here.
        // TODO(clean-room): menu-font parent background not modeled.
        ::FillRect(hdc, rectClient, ::GetSysColorBrush(COLOR_BTNFACE));
    } else {
        // Retail fills with a cached brush from the global state; button face
        // is the closest public approximation.
        // TODO(clean-room): uses COLOR_BTNFACE instead of the global color.
        ::FillRect(hdc, rectClient, ::GetSysColorBrush(COLOR_BTNFACE));
    }

    if (s->m_bChecked != 0 && s->m_bDrawText != 0) {
        if (!(s->m_bHover != 0 && s->m_bClickStarted != 0)) {
            // Retail draws a highlight frame via 0x180056900; approximate with
            // a one-pixel highlight border.
            // TODO(clean-room): highlight frame approximated with FrameRect.
            RECT frame = *rectClient;
            ::InflateRect(&frame, -1, -1);
            ::FrameRect(hdc, &frame, ::GetSysColorBrush(COLOR_HIGHLIGHT));
        }
    }
}
// DrawBorder(CDC*, CRect&, UINT): draws the border and deflates rectClient by
// the space the border consumed.  Retail fills the face with global state
// colors; approximated with standard edge drawing.
// Symbol: ?DrawBorder@CMFCButton@@IEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__DrawBorder_CMFCButton__IEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, unsigned int uiState)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    HDC hdc = hdc_of_MfcFeature34Impl(pDC);
    if (hdc == nullptr) return;

    if (s->m_nStyle == 3) {
        // Borderless style: retail only deflates the rect.
        ::InflateRect(rectClient, -2, -2);
        return;
    }

    const bool bPressed = (s->m_bClickStarted != 0) || ((uiState & 0x1) != 0);
    const bool bHot = s->m_bHover != 0;

    // TODO(clean-room): retail fills the face with global-state colors and uses
    // finer pressed/hover distinctions; DrawEdge approximates the look.
    if (s->m_bChecked != 0) {
        ::DrawEdge(hdc, rectClient, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    } else if (bPressed) {
        ::DrawEdge(hdc, rectClient, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    } else if (bHot) {
        ::DrawEdge(hdc, rectClient, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    } else {
        ::DrawEdge(hdc, rectClient, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
        ::InflateRect(rectClient, -1, -1);
    }
}
// OnDrawBorder(CDC*, CRect&, UINT): retail is a direct jump to DrawBorder
// (RVA 0x1ec30 -> 0x1f160); forwarded to the same implementation.
// Symbol: ?OnDrawBorder@CMFCButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, unsigned int uiState)
{
    impl__DrawBorder_CMFCButton__IEAAXPEAVCDC__AEAVCRect__I_Z(
        pThis, pDC, rectClient, uiState);
}
// OnDrawText(CDC*, const CRect&, const CString&, UINT, UINT): draws the button
// text centred with the supplied format.  (uiState is unused in the retail
// body; the mangled name ends `II` = two UINT params.)
// Symbol: ?OnDrawText@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@II@Z
extern "C" void MS_ABI impl__OnDrawText_CMFCButton__MEAAXPEAVCDC__AEBVCRect__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__II_Z(
    void* pThis, void* pDC, const RECT* rect, const void* strText,
    unsigned int uiFormat, unsigned int /*uiState*/)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr || strText == nullptr) return;
    HDC hdc = hdc_of_MfcFeature34Impl(pDC);
    if (hdc == nullptr) return;
    const wchar_t* psz = *reinterpret_cast<const wchar_t* const*>(strText);
    if (psz == nullptr) return;
    // ATL CStringData: nDataLength precedes the buffer by 16 bytes.
    const int nLen = *reinterpret_cast<const int*>(
        reinterpret_cast<const char*>(psz) - 0x10);
    RECT r = *rect;
    ::DrawTextW(hdc, psz, nLen, &r, uiFormat);
}
// OnDraw(CDC*, const CRect&, UINT): main draw -- fills the background and draws
// the window text.  The image/icon and hot-text rendering paths of the retail
// body (RVA 0x1cf60) require the unmodeled image objects and visual-manager
// globals, so they are not reproduced.
// Symbol: ?OnDraw@CMFCButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    HDC hdc = hdc_of_MfcFeature34Impl(pDC);
    if (hdc == nullptr) return;

    impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
        pThis, pDC, rect);

    if (s->m_bDrawText != 0) {
        wchar_t buf[1024];
        int nLen = ::GetWindowTextW(s->m_hWnd, buf, 1024);
        if (nLen > 0) {
            RECT r = *rect;
            UINT format = DT_SINGLELINE | DT_CENTER | DT_VCENTER;
            if (s->m_nFlatStyle == 1) format = DT_SINGLELINE | DT_LEFT | DT_VCENTER;
            else if (s->m_nFlatStyle == 2) format = DT_SINGLELINE | DT_RIGHT | DT_VCENTER;
            // TODO(clean-room): image/icon + hot-text color rendering not modeled.
            ::SetTextColor(hdc, ::GetSysColor(COLOR_BTNTEXT));
            ::DrawTextW(hdc, buf, nLen, &r, format);
        }
    }
}
// DrawItem(LPDRAWITEMSTRUCT): owner-draw entry point -- validates the control
// type, then invokes the drawing virtuals in the same order as retail
// (OnDraw, OnDrawBorder, OnFillBackground, OnDrawFocusRect).
// Symbol: ?DrawItem@CMFCButton@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CMFCButton__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDIS)
{
    if (pThis == nullptr) return;
    if (lpDIS == nullptr || lpDIS->CtlType != ODT_BUTTON) {
        // Retail asserts (0x180227720) on a bad DRAWITEMSTRUCT.
        return;
    }
    // CDC::FromHandle equivalent: a minimal CDC head wrapping the HDC.
    struct CDCShim {
        void* vfptr;
        HDC   hDC;
        HDC   hAttribDC;
    } dc = { nullptr, lpDIS->hDC, lpDIS->hDC };
    void* pDC = &dc;

    RECT rect = lpDIS->rcItem;
    const unsigned int itemState = static_cast<unsigned int>(lpDIS->itemState);

    impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(pThis, pDC, &rect, itemState);
    impl__OnDrawBorder_CMFCButton__MEAAXPEAVCDC__AEAVCRect__I_Z(pThis, pDC, &rect, itemState);
    impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(pThis, pDC, &rect);

    if ((itemState & ODS_FOCUS) != 0) {
        S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
        if (s->m_bDrawFocus != 0) {
            impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
                pThis, pDC, &rect);
        }
    }
}
// OnInitControl(WPARAM, LPARAM): retail parses the "MFCButton_Style",
// "MFCButton_Autosize", "MFCButton_Tooltip" and "MFCButton_FullTextTool"
// window properties and the runtime font to configure the button.  Those
// property/registry lookups and the font helper are not modeled.
// Symbol: ?OnInitControl@CMFCButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnInitControl_CMFCButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long /*wParam*/, long long /*lParam*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): property/style parsing not modeled.
    return 0;
}
// OnDestroy(): cleans up the tooltip helper object at 0xaf8.
// Symbol: ?OnDestroy@CMFCButton@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    S_MfcFeature34Impl* s = reinterpret_cast<S_MfcFeature34Impl*>(pThis);
    if (s->pHelperWnd != nullptr) {
        HWND hwndHelper = *reinterpret_cast<HWND*>(reinterpret_cast<char*>(s->pHelperWnd) + 0x40);
        if (hwndHelper != nullptr) {
            ::DestroyWindow(hwndHelper);
        }
        // Retail frees the helper object through 0x1801824a0; null it out.
        // TODO(clean-room): helper object is not deallocated.
        s->pHelperWnd = nullptr;
    }
    // retail tail-calls the CWnd base OnDestroy handler here
}
CMFCButton::CMFCButton() {
    memset(_mfcbutton_padding, 0, sizeof(_mfcbutton_padding));
}
CMFCButton::~CMFCButton() {
    RemoveMFCButtonState(this);
}
void CMFCButton::SetImage(HICON hIcon, BOOL bAutoDestroy, HICON hIconHot, BOOL) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    if (state.ownsIcon && state.icon && state.icon != hIcon) ::DestroyIcon(state.icon);
    if (state.ownsHotIcon && state.iconHot && state.iconHot != hIconHot) ::DestroyIcon(state.iconHot);
    state.icon = hIcon;
    state.iconHot = hIconHot;
    state.ownsIcon = bAutoDestroy != FALSE;
    state.ownsHotIcon = bAutoDestroy != FALSE && hIconHot != nullptr;
    state.ownerDraw = (hIcon != nullptr || hIconHot != nullptr);
    if (GetSafeHwnd()) Invalidate(FALSE);
}
void CMFCButton::SetFaceColor(COLORREF crFace, BOOL bRedraw) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    state.faceColor = crFace;
    state.ownerDraw = true;
    if (bRedraw && GetSafeHwnd()) Invalidate(FALSE);
}
void CMFCButton::SetTextColor(COLORREF crText) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    state.textColor = crText;
    state.ownerDraw = true;
    if (GetSafeHwnd()) Invalidate(FALSE);
}
void CMFCButton::SetTextHotColor(COLORREF crTextHot) {
    MFCButtonState& state = EnsureMFCButtonState(this);
    state.textHotColor = crTextHot;
    state.ownerDraw = true;
    if (GetSafeHwnd()) Invalidate(FALSE);
}
BOOL CMFCButton::IsOwnerDraw() const {
    auto it = g_mfcButtonStates.find(this);
    return it != g_mfcButtonStates.end() && it->second.ownerDraw ? TRUE : FALSE;
}
void CMFCButton::EnableFullTextTooltip(BOOL bEnable) {
    EnsureMFCButtonState(this).fullTextTooltip = bEnable != FALSE;
}
void CMFCButton::SetTooltip(const wchar_t* lpszToolTipText) {
    EnsureMFCButtonState(this).tooltip = lpszToolTipText ? lpszToolTipText : L"";
}
