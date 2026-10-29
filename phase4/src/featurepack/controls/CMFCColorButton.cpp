// CMFCColorButton — OpenMFC implementation.
// Sources: global_cmfccolorbutton.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCColorButtonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// CMFCButton::m_bWinXPTheme and CMFCVisualManager::m_pVisManager are the
// globals read by retail IsDrawXPTheme. Their OpenMFC definitions are in
// featurepack/controls/StaticData.cpp and core/runtime/StaticData.cpp.
extern "C" std::int32_t impl__m_bWinXPTheme_CMFCButton__1HA;
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;

// Internal helper thunks used by OnInitControl; their definitions are in the
// referenced featurepack translation units.
extern "C" int MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const char* pszUTF8, CString* pStrResult, int cbUTF8);
extern "C" void MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer);
extern "C" void MS_ABI impl___1CTagManager__UEAA_XZ(void* self);
extern "C" int MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(
    void* tagManager, const wchar_t* property, int* value);
extern "C" int MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* self, const wchar_t* tag, CString* value, int trim);
extern "C" CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);

// EnableAutomaticButton(LPCTSTR lpszLabel, COLORREF clr, BOOL bShowBorder):
// stores the automatic-button label (CString at 0xbb0, a null label defaults
// to the empty string in retail) and clr (0xb2c).  Transcribed from retail RVA
// 0x28d00 (mfc140u); the retail bShowBorder gate only chooses between the
// null-label fallback (L"") and the label itself, so the effective result is
// unconditional.
// Symbol: ?EnableAutomaticButton@CMFCColorButton@@QEAAXPEB_WKH@Z
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCColorButton__QEAAXPEB_WKH_Z(
    void* pThis, const wchar_t* lpszLabel, unsigned long clr, int /*bShowBorder*/)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    AssignCStr_Cmfccolorbutton(&s->m_strAutoLabel, lpszLabel ? lpszLabel : L"");  // 0xbb0
    s->m_clrAutomaticButton = static_cast<int>(clr);              // 0xb2c
}
// EnableOtherButton(LPCTSTR lpszLabel, BOOL bAuto, BOOL bShowBorder): stores
// the other-button label (CString at 0xba0, a null label defaults to the
// empty string in retail) and bAuto (0xbb8).  Transcribed from retail RVA
// 0x28d70 (mfc140u); the bShowBorder gate only chooses the null-label fallback.
// Symbol: ?EnableOtherButton@CMFCColorButton@@QEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__EnableOtherButton_CMFCColorButton__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* lpszLabel, int bAuto, int /*bShowBorder*/)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    AssignCStr_Cmfccolorbutton(&s->m_strOtherLabel, lpszLabel ? lpszLabel : L"");  // 0xba0
    s->m_bOtherButton = bAuto;                                     // 0xbb8
}
// IsDrawXPTheme(): retail RVA 0x29160 (mfc140u) returns FALSE when
// CMFCButton::m_bWinXPTheme is zero; otherwise it obtains the visual manager
// and returns the result of its virtual call at vtable byte offset 0x68
// (slot 13). OpenMFC has a C++ GetInstance definition that creates a manager,
// but direct use adds an unaudited C++ undefined symbol; there is no impl__
// thunk for it. To keep the per-file link audit clean, use the existing global
// manager only and return FALSE when it is still null (a retail deviation).
// Symbol: ?IsDrawXPTheme@CMFCColorButton@@IEBAHXZ
extern "C" int MS_ABI impl__IsDrawXPTheme_CMFCColorButton__IEBAHXZ(
    void* /*pThis*/)
{
    if (impl__m_bWinXPTheme_CMFCButton__1HA == 0) return 0;
    void* pManager = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (pManager == nullptr) return 0;

    void** vtable = *reinterpret_cast<void***>(pManager);
    using IsDrawXPThemeFn = int (MS_ABI *)(void*);
    IsDrawXPThemeFn isDrawXPTheme = reinterpret_cast<IsDrawXPThemeFn>(vtable[13]);
    return isDrawXPTheme(pManager) != 0;
}
// SetColor(COLORREF clr): stores the color at 0xb28 and, when the control has
// a window, repaints it.  Transcribed from retail RVA 0x28f50 (mfc140u).
// Symbol: ?SetColor@CMFCColorButton@@QEAAXK@Z
extern "C" void MS_ABI impl__SetColor_CMFCColorButton__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    s->m_nColor = static_cast<int>(clr);                    // 0xb28
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// Retail's CMFCColorButton message map routes WM_GETDLGCODE to the shared
// body returning 1 at RVA 0x3a60 (mfc140u); the symbol map names that folded body
// as accDoDefaultAction on another class.  Return DLGC_WANTARROWS (1).
// Symbol: ?OnGetDlgCode@CMFCColorButton@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CMFCColorButton__IEAAIXZ(
    void* /*pThis*/)
{
    return 1;
}
// OnSysColorChange(): runs the CWnd base handler, rebuilds the button's
// palette and repaints.  Transcribed from retail RVA 0x29120 (mfc140u).
// Symbol: ?OnSysColorChange@CMFCColorButton@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCColorButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    impl__OnSysColorChange_CWnd__IEAAXXZ(reinterpret_cast<CWnd*>(pThis));
    impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(pThis, nullptr);
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(s->m_hWnd);
    }
}
// OnMouseMove(UINT, CPoint): while XP-theme drawing is active the retail
// forces m_nStyle to 2 (pushed) around the CMFCButton base handler and
// restores it afterwards.  Transcribed from retail RVA 0x28cb0 (mfc140u).
// The style is forced only when the local IsDrawXPTheme returns nonzero; its
// null-manager fallback differs from retail's manager-creating helper.
// Symbol: ?OnMouseMove@CMFCColorButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCColorButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, POINT point)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    const int nStyleSaved = s->m_nStyle;                    // 0xe8
    if (impl__IsDrawXPTheme_CMFCColorButton__IEBAHXZ(pThis) != 0) {
        s->m_nStyle = 2;
    }
    impl__OnMouseMove_CMFCButton__IEAAXIVCPoint___Z(pThis, nFlags, point);
    s->m_nStyle = nStyleSaved;
}
// OnLButtonDown(UINT, CPoint): retail RVA 0x28c80 (mfc140u) calls
// CWnd::SetFocus then dispatches the vslot+0x348 virtual (the color-popup
// handler).  This implementation substitutes a direct call to the local
// OnShowColorPopup thunk, so derived-class virtual overrides are not dispatched.
// Symbol: ?OnLButtonDown@CMFCColorButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCColorButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(reinterpret_cast<CWnd*>(pThis));
    // TODO(clean-room): the vslot+0x348 virtual dispatch is replaced with a
    // direct call to the local OnShowColorPopup implementation.
    impl__OnShowColorPopup_CMFCColorButton__MEAAXXZ(pThis);
}
// OnKeyDown(UINT, UINT, UINT): retail RVA 0x28c60 (mfc140u) only handles
// keys with (nChar - 0x20) & 0xFFFFFFF7 == 0 (VK_SPACE 0x20 and friends);
// everything else goes to CWnd::Default.  The handled keys dispatch the
// vslot+0x348 virtual; this implementation substitutes a direct call to the
// local OnShowColorPopup thunk.
// Symbol: ?OnKeyDown@CMFCColorButton@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCColorButton__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int /*nRepCnt*/,
    unsigned int /*nFlags*/)
{
    if (pThis == nullptr) return;
    if (((nChar - 0x20u) & 0xfffffff7u) != 0) {
        (void)impl__Default_CWnd__IEAA_JXZ(reinterpret_cast<CWnd*>(pThis));
        return;
    }
    // TODO(clean-room): the vslot+0x348 virtual dispatch is replaced with a
    // direct call to the local OnShowColorPopup implementation.
    impl__OnShowColorPopup_CMFCColorButton__MEAAXXZ(pThis);
}
// OnShowColorPopup(): retail closes an already-active popup with WM_CLOSE
// and clears m_pColorPopup at 0xb98.  Otherwise it initializes the color array
// when empty, allocates and configures a CMFCColorPopup, then positions it via
// virtual calls; the latter popup-creation and positioning machinery is not modeled.
// Symbol: ?OnShowColorPopup@CMFCColorButton@@MEAAXXZ
extern "C" void MS_ABI impl__OnShowColorPopup_CMFCColorButton__MEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    void* pPopup = s->m_pColorPopup;                        // 0xb98
    if (pPopup != nullptr) {
        HWND hwndPopup = *reinterpret_cast<HWND*>(
            reinterpret_cast<char*>(pPopup) + 0x40);        // popup CWnd::m_hWnd
        ::SendMessage(hwndPopup, WM_CLOSE, 0, 0);
        s->m_pColorPopup = nullptr;
        return;
    }
    // Retail fills the color CArray from the system palette when empty, then
    // creates and shows the popup.  Only the InitColors side effect is reproduced.
    // TODO(clean-room): popup allocation, initialization, and positioning are
    // not modeled.
    void* pArray = reinterpret_cast<char*>(pThis) + 0xb30;
    if (*reinterpret_cast<long long*>(reinterpret_cast<char*>(pArray) + 0x10) == 0) {
        (void)impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
            nullptr, pArray);
    }
}
// OnDraw(CDC*, const CRect&, UINT): retail RVA 0x28360 (mfc140u) ensures the
// palette holder at 0xbc0 (RebuildPalette when null), selects/realises the
// palette into the DC, shrinks the rect by 8 + CMenuImages::Size().cx, draws
// the automatic label when m_nColor == -1 and its label is non-empty, and fills
// the cell with the current or automatic color unless disabled.  The local gutter size and 3-D border
// are approximations; automatic-label drawing and palette-index conversion
// are not modeled.
// Symbol: ?OnDraw@CMFCColorButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI impl__OnDraw_CMFCColorButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    HDC hdc = hdc_of_Cmfccolorbutton(pDC);
    if (hdc == nullptr) return;

    if (s->m_pPaletteObj == nullptr) {
        impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(pThis, nullptr);
    }
    HPALETTE hOldPalette = nullptr;
    if (s->m_pPaletteObj != nullptr && s->m_pPaletteObj->m_hPal != nullptr) {
        hOldPalette = ::SelectPalette(hdc, s->m_pPaletteObj->m_hPal, FALSE);
        ::RealizePalette(hdc);
    }

    // Cell rect: right shrunk by 8 + CMenuImages::Size().cx (approximated as 9).
    RECT r = *rect;
    r.right -= (8 + 9);

    // DrawEdge approximates retail's 3-D border.
    RECT border = r;
    ::InflateRect(&border, -2, -2);
    ::DrawEdge(hdc, &border, BDR_SUNKENOUTER, BF_RECT);
    RECT cell = border;
    ::InflateRect(&cell, -1, -1);

    if ((uiState & ODS_DISABLED) == 0) {
        const COLORREF color = static_cast<COLORREF>(
            s->m_nColor == -1 ? s->m_clrAutomaticButton : s->m_nColor);
        HBRUSH br = ::CreateSolidBrush(color);
        if (br != nullptr) {
            ::FillRect(hdc, &cell, br);
            ::DeleteObject(br);
        }
    }
    if (hOldPalette != nullptr) {
        ::SelectPalette(hdc, hOldPalette, FALSE);
    }

    // TODO(clean-room): automatic-label text, palette-index conversion, and
    // the themed focus-rectangle pass are not modeled.
}
// OnFillBackground(CDC*, const CRect&): non-themed (IsDrawXPTheme == 0) routes
// to CMFCButton::OnFillBackground; the themed global brush is approximated
// with the button-face brush.  A null manager takes the base path here because
// the local IsDrawXPTheme cannot call retail's lazy-creating helper.  Transcribed
// from retail RVA 0x282e0 (mfc140u).
// Symbol: ?OnFillBackground@CMFCColorButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnFillBackground_CMFCColorButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    if (impl__IsDrawXPTheme_CMFCColorButton__IEBAHXZ(pThis) == 0) {
        impl__OnFillBackground_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
            pThis, pDC, rectClient);
        return;
    }
    HDC hdc = hdc_of_Cmfccolorbutton(pDC);
    if (hdc == nullptr) return;
    // TODO(clean-room): the retail themed brush and its lazy initialization
    // are not modeled; the button-face brush is used instead.
    ::FillRect(hdc, rectClient, ::GetSysColorBrush(COLOR_BTNFACE));
}
// OnDrawBorder(CDC*, CRect&, UINT): retail RVA 0x28830 (mfc140u) draws a
// 3-D border and, when m_nStyle && m_bClickStarted, a second pressed border.
// The themed branch dispatches the visual-manager DrawButtonBorder virtual;
// that path and its retail colors are not modeled, so DrawEdge approximates it.
// Symbol: ?OnDrawBorder@CMFCColorButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCColorButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rect, unsigned int /*uiState*/)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    HDC hdc = hdc_of_Cmfccolorbutton(pDC);
    if (hdc == nullptr) return;

    // TODO(clean-room): the themed visual-manager dispatch and retail border
    // colors are not modeled.
    RECT r = *rect;
    ::DrawEdge(hdc, &r, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
    if (s->m_nStyle != 0 && s->m_bClickStarted != 0) {
        ::DrawEdge(hdc, &r, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
    }
}
// OnDrawFocusRect(CDC*, const CRect&): shrinks the right edge by
// 8 + CMenuImages::Size().cx (the colour-cell gutter) and forwards to
// CMFCButton::OnDrawFocusRect.  Transcribed from retail RVA 0x28980 (mfc140u).
// Symbol: ?OnDrawFocusRect@CMFCColorButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCColorButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rect)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    // TODO(clean-room): use of the CMenuImages::Size() API is approximated as 9;
    // retail has separate theme/DPI-dependent size paths.
    RECT r = *rect;
    r.right -= (8 + 9);
    impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
        pThis, pDC, &r);
}
// OnInitControl(unsigned __int64 wParam, __int64 lParam): retail entry RVA
// 0x291a0 (mfc140u) converts the UTF-8 init string to CString, reads the
// MFCColorButton_EnableOtherButton / EnableAutomaticButton / ColumnsCount
// properties through CTagManager, and returns 0.
// Symbol: ?OnInitControl@CMFCColorButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnInitControl_CMFCColorButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long wParam, long long lParam)
{
    if (pThis == nullptr) return 0;

    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));

    alignas(16) unsigned char tagManagerStorage[64] = {};
    impl___0CTagManager__QEAA_PEB_W_Z(tagManagerStorage, strDst.GetString());

    int bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(
            tagManagerStorage, L"MFCColorButton_EnableOtherButton", &bValue)) {
        impl__EnableOtherButton_CMFCColorButton__QEAAXPEB_WHH_Z(
            pThis, bValue ? L"Other" : nullptr, 1, bValue);
    }
    bValue = FALSE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(
            tagManagerStorage, L"MFCColorButton_EnableAutomaticButton", &bValue)) {
        impl__EnableAutomaticButton_CMFCColorButton__QEAAXPEB_WKH_Z(
            pThis, bValue ? L"Automatic" : nullptr, 0, bValue);
    }

    CString strColumns;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            tagManagerStorage, L"MFCColorButton_ColumnsCount", &strColumns, FALSE) &&
        !strColumns.IsEmpty()) {
        const int nColumns = ::_wtoi(strColumns.GetString());
        if (nColumns > 0) {
            reinterpret_cast<CColorBtn*>(pThis)->m_nPopupArg = nColumns;  // 0xb90
        }
    }

    impl___1CTagManager__UEAA_XZ(tagManagerStorage);
    return 0;
}
// RebuildPalette(CPalette*): builds a system (pPalette == null) or
// caller-palette (pPalette != null) LOGPALETTE, CreatePalette's it and stores
// the handle in the 0x10-byte holder at 0xbc0, freeing the previous holder.
// Transcribed from retail RVA 0x29000 (mfc140u); the holder vtable delete is
// replaced by DeleteObject + free, and foreign blocks (vtable != 0) are not freed.
// Symbol: ?RebuildPalette@CMFCColorButton@@IEAAXPEAVCPalette@@@Z
extern "C" void MS_ABI impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);

    // Free the previous holder (retail: vtable delete on 0xbc0).
    CPalHolder_Cmfccolorbutton* old = s->m_pPaletteObj;
    if (old != nullptr && old->vtable == nullptr) {
        if (old->m_hPal != nullptr) ::DeleteObject(old->m_hPal);
        std::free(old);
    }

    CPalHolder_Cmfccolorbutton* holder = static_cast<CPalHolder_Cmfccolorbutton*>(std::calloc(1, sizeof(CPalHolder_Cmfccolorbutton)));
    s->m_pPaletteObj = holder;
    if (holder == nullptr) return;

    HDC hdc = (s->m_hWnd != nullptr) ? ::GetDC(s->m_hWnd) : nullptr;  // CClientDC
    LOGPALETTE* pLog = nullptr;
    int nCount = 0;
    if (pPalette == nullptr) {
        nCount = 0x100;
        pLog = static_cast<LOGPALETTE*>(std::malloc(0x408));
        if (pLog != nullptr && hdc != nullptr) {
            pLog->palVersion = 0x300;
            pLog->palNumEntries = 0x100;
            ::GetSystemPaletteEntries(hdc, 0, 0x100, pLog->palPalEntry);
        } else if (pLog != nullptr) {
            pLog->palVersion = 0x300;
            pLog->palNumEntries = 0x100;
        }
    } else {
        WORD wCount = 0;
        void* hPalObj = *reinterpret_cast<void**>(
            reinterpret_cast<char*>(pPalette) + 8);      // CPalette::m_hObject
        if (hPalObj != nullptr &&
            ::GetObject(hPalObj, sizeof(WORD), &wCount) != 0) {
            nCount = wCount;
            pLog = static_cast<LOGPALETTE*>(std::malloc(8 + static_cast<size_t>(nCount) * 4));
            if (pLog != nullptr) {
                pLog->palVersion = 0x300;
                pLog->palNumEntries = static_cast<WORD>(nCount);
                ::GetPaletteEntries(reinterpret_cast<HPALETTE>(hPalObj), 0,
                                    static_cast<UINT>(nCount),
                                    pLog->palPalEntry);
            }
        }
    }

    if (pLog != nullptr) {
        HPALETTE hPal = ::CreatePalette(pLog);
        if (hPal != nullptr) {
            holder->m_hPal = hPal;   // retail attaches the created palette handle.
        }
        std::free(pLog);
    }
    if (hdc != nullptr) ::ReleaseDC(s->m_hWnd, hdc);
    (void)nCount;
}
// SetPalette(CPalette*): releases the color CArray at 0xb30 when non-empty,
// sizes the internal buffer, fills the array through CMFCColorBar::InitColors
// and rebuilds the button palette.  Transcribed from retail RVA 0x28e70
// (mfc140u); the CArray layout is m_pData@+0x8 / m_nSize@+0x10 /
// m_nMaxSize@+0x18.  The local InitColors implementation fills the array.
// Symbol: ?SetPalette@CMFCColorButton@@QEAAXPEAVCPalette@@@Z
extern "C" void MS_ABI impl__SetPalette_CMFCColorButton__QEAAXPEAVCPalette___Z(
    void* pThis, void* pPalette)
{
    if (pThis == nullptr) return;
    void* pArray = reinterpret_cast<char*>(pThis) + 0xb30;

    unsigned int* pData = *reinterpret_cast<unsigned int**>(
        reinterpret_cast<char*>(pArray) + 0x8);
    long long nSize = *reinterpret_cast<long long*>(
        reinterpret_cast<char*>(pArray) + 0x10);
    long long nMaxSize = *reinterpret_cast<long long*>(
        reinterpret_cast<char*>(pArray) + 0x18);

    if (nSize != 0) {
        // CArray::SetSize(arr, 0): release the buffer.
        if (pData != nullptr) std::free(pData);
        pData = nullptr;
        nSize = 0;
        nMaxSize = 0;
    }
    if (nSize != nMaxSize) {
        // Grow/shrink the buffer to m_nSize entries.
        unsigned int* pNew = nullptr;
        if (nSize > 0) {
            pNew = static_cast<unsigned int*>(
                std::malloc(static_cast<size_t>(nSize) * 4));
            if (pNew != nullptr && pData != nullptr) {
                std::memcpy(pNew, pData, static_cast<size_t>(nSize) * 4);
            }
        }
        if (pData != nullptr) std::free(pData);
        pData = pNew;
        nMaxSize = nSize;
    }
    *reinterpret_cast<unsigned int**>(reinterpret_cast<char*>(pArray) + 0x8) = pData;
    *reinterpret_cast<long long*>(reinterpret_cast<char*>(pArray) + 0x10) = nSize;
    *reinterpret_cast<long long*>(reinterpret_cast<char*>(pArray) + 0x18) = nMaxSize;

    // Retail calls InitColors(&0xb30, pPalette) then RebuildPalette(this, pPalette).
    // The local InitColors implementation fills the array from the supplied
    // palette, or from the system default palette when pPalette is null.
    (void)impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
        pPalette, pArray);
    impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(pThis, pPalette);
}
// Retail SetDocumentColors clears the member list at 0xb58, stores the label
// (CString at 0xba8, null -> L""), and copies the caller's colors when the label
// is non-empty.  Transcribed from retail RVA 0x28de0 (mfc140u); local list
// operations are omitted because the stub constructor leaves the member list
// uninitialized.  The caller-provided label is still stored below.
// Symbol: ?SetDocumentColors@CMFCColorButton@@QEAAXPEB_WAEAV?$CList@KK@@@Z
extern "C" void MS_ABI impl__SetDocumentColors_CMFCColorButton__QEAAXPEB_WAEAV__CList_KK___Z(
    void* pThis, const wchar_t* lpszLabel, void* /*lstColors*/)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    // TODO(clean-room): retail RemoveAll's the member list at 0xb58 and, when
    // the label is non-empty, walks the caller's CList (head@+0x8; nodes
    // {next@0x0, data@0x10}) appending each colour.  The member list is not
    // touched because the stub constructor does not initialize it.
    AssignCStr_Cmfccolorbutton(&s->m_strDocLabel, lpszLabel ? lpszLabel : L"");  // 0xba8
}
// SizeToContent(BOOL): runs the CMFCButton base (retail passes bCalcOnly=0),
// approximates CMenuImages::Size().cx as 9 when adding to the width, and resizes unless
// bCalcOnly.  Transcribed from retail RVA 0x28270 (mfc140u; SetWindowPos
// flags 0x16 = SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE, x/y = -1).
// Symbol: ?SizeToContent@CMFCColorButton@@UEAA?AVCSize@@H@Z
extern "C" CSize* MS_ABI impl__SizeToContent_CMFCColorButton__UEAA_AVCSize__H_Z(
    void* pThis, CSize* pRet, int bCalcOnly)
{
    if (pRet == nullptr) return pRet;
    if (pThis == nullptr) {
        pRet->cx = 0;
        pRet->cy = 0;
        return pRet;
    }
    impl__SizeToContent_CMFCButton__UEAA_AVCSize__H_Z(pThis, pRet, 0);
    // TODO(clean-room): CMenuImages::Size().cx is approximated as 9; retail has
    // separate theme/DPI-dependent size paths.
    pRet->cx += 9;
    if (bCalcOnly == 0) {
        impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(
            reinterpret_cast<CWnd*>(pThis), nullptr, -1, -1,
            pRet->cx, pRet->cy, 0x16);
    }
    return pRet;
}
// UpdateColor(COLORREF clr): stores the color (SetColor) and notifies the
// parent dialog with a WM_COMMAND/BN_CLICKED message.  Transcribed from
// retail RVA 0x28f90 (mfc140u) uses CWnd::FromHandle on the parent,
// CWnd::GetDlgCtrlID, and sends WM_COMMAND (BN_CLICKED).
// Symbol: ?UpdateColor@CMFCColorButton@@MEAAXK@Z
extern "C" void MS_ABI impl__UpdateColor_CMFCColorButton__MEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    impl__SetColor_CMFCColorButton__QEAAXK_Z(pThis, clr);
    if (s->m_hWnd == nullptr) return;
    HWND hwndParent = ::GetParent(s->m_hWnd);
    if (hwndParent == nullptr) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwndParent);
    if (pParent == nullptr || pParent->m_hWnd == nullptr) return;
    WORD wId = static_cast<WORD>(::GetDlgCtrlID(s->m_hWnd));
    ::SendMessage(pParent->m_hWnd, WM_COMMAND,
                  static_cast<WPARAM>(wId),
                  reinterpret_cast<LPARAM>(s->m_hWnd));
}
// Symbol: ??0CMFCColorButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// Retail RVA 0x28180 (mfc140u) destroys the palette holder, releases the
// three CString members, destroys the CList and CArray members, then calls
// CMFCButton::~CMFCButton. The local CMFCColorButton constructor thunk above
// currently returns without constructing any of those subobjects, so their
// fields cannot be safely treated as initialized here. Leave this as an
// explicit stub rather than dereferencing or releasing potentially invalid
// object state; faithful cleanup requires first implementing that constructor
// and modeling the retail-owned palette/list/array teardown.
// Symbol: ??1CMFCColorButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorButton__UEAA_XZ(void* /*pThis*/) {
    // TODO(clean-room): unsafe until this class's subobjects are initialized.
}
