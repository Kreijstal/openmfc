// CMFCColorButton — OpenMFC implementation.
// Sources: global_cmfccolorbutton.cpp, manual_small_stub_implementations.cpp

#include "detail/CMFCColorButtonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

// EnableAutomaticButton(LPCTSTR lpszLabel, COLORREF clr, BOOL bShowBorder):
// stores the automatic-button label (CString at 0xbb0, a null label defaults
// to the empty string in retail) and clr (0xb2c).  Transcribed from retail RVA
// 0x28d00; the retail bShowBorder gate only chooses between the null-label
// fallback (L"") and the label itself, so the effective result is
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
// 0x28d70; the bShowBorder gate only chooses the null-label fallback.
// Symbol: ?EnableOtherButton@CMFCColorButton@@QEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__EnableOtherButton_CMFCColorButton__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* lpszLabel, int bAuto, int /*bShowBorder*/)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    AssignCStr_Cmfccolorbutton(&s->m_strOtherLabel, lpszLabel ? lpszLabel : L"");  // 0xba0
    s->m_bOtherButton = bAuto;                                     // 0xbb8
}
// IsDrawXPTheme(): retail (RVA 0x29160) gates on the module flag 0x1803be1a8
// and dispatches the visual-manager IsDrawXPTheme-style virtual
// (0x180009774, vtable+0x68) through 0x1802c7b30.  Neither the flag nor the
// singleton is modeled, so 0 (the non-themed result) is returned.  The
// callers (OnFillBackground / OnMouseMove / OnDrawBorder) then take their
// non-themed branches, which is the behaviour of a non-themed system.
// Symbol: ?IsDrawXPTheme@CMFCColorButton@@IEBAHXZ
extern "C" int MS_ABI impl__IsDrawXPTheme_CMFCColorButton__IEBAHXZ(
    void* /*pThis*/)
{
    // TODO(clean-room): transcribed partially -- module flag 0x1803be1a8 and
    // visual-manager virtual dispatch not modeled.
    return 0;
}
// SetColor(COLORREF clr): stores the color at 0xb28 and, when the control has
// a window, repaints it.  Transcribed from retail RVA 0x28f50.
// Symbol: ?SetColor@CMFCColorButton@@QEAAXK@Z
extern "C" void MS_ABI impl__SetColor_CMFCColorButton__QEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    s->m_nColor = static_cast<int>(clr);                    // 0xb28
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);         // 0x1802c7128
        ::UpdateWindow(s->m_hWnd);                          // 0x1802c7300
    }
}
// OnGetDlgCode(): retail is `mov $0x1,%eax; ret` (RVA 0x3a60).  Transcribed
// verbatim.
// Symbol: ?OnGetDlgCode@CMFCColorButton@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CMFCColorButton__IEAAIXZ(
    void* /*pThis*/)
{
    return 1;
}
// OnSysColorChange(): runs the CWnd base handler, rebuilds the button's
// palette and repaints.  Transcribed from retail RVA 0x29120.
// Symbol: ?OnSysColorChange@CMFCColorButton@@IEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCColorButton__IEAAXXZ(
    void* pThis)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    impl__OnSysColorChange_CWnd__IEAAXXZ(reinterpret_cast<CWnd*>(pThis));
    impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(pThis, nullptr);
    if (s->m_hWnd != nullptr) {
        ::InvalidateRect(s->m_hWnd, nullptr, TRUE);         // 0x1802c7128
        ::UpdateWindow(s->m_hWnd);                          // 0x1802c7300
    }
}
// OnMouseMove(UINT, CPoint): while XP-theme drawing is active the retail
// forces m_nStyle to 2 (pushed) around the CMFCButton base handler and
// restores it afterwards.  Transcribed from retail RVA 0x28cb0 (IsDrawXPTheme
// is conservatively 0, so the style is never forced in this build).
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
// OnLButtonDown(UINT, CPoint): retail (RVA 0x28c80) calls CWnd::SetFocus
// (0x1802a9b60) then dispatches the vslot+0x348 virtual -- the same handler
// OnKeyDown reaches, i.e. the color popup.  The popup dispatch is not modeled,
// so the conservative OnShowColorPopup below is used.
// Symbol: ?OnLButtonDown@CMFCColorButton@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCColorButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int /*nFlags*/, POINT /*point*/)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    if (s->m_hWnd != nullptr) {
        ::SetFocus(s->m_hWnd);   // retail: CWnd::SetFocus
    }
    // TODO(clean-room): transcribed partially -- the vslot+0x348 dispatch
    // (0x1802c7b30) is routed to the conservative OnShowColorPopup.
    impl__OnShowColorPopup_CMFCColorButton__MEAAXXZ(pThis);
}
// OnKeyDown(UINT, UINT, UINT): retail (RVA 0x28c60) only handles keys with
// (nChar - 0x20) & 0xFFFFFFF7 == 0 (VK_SPACE 0x20 and friends); everything
// else goes to CWnd::Default (0x18028ac80).  The handled keys dispatch the
// vslot+0x348 virtual (the color popup), which is routed to the conservative
// OnShowColorPopup.
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
    // TODO(clean-room): transcribed partially -- the vslot+0x348 dispatch is
    // routed to the conservative OnShowColorPopup.
    impl__OnShowColorPopup_CMFCColorButton__MEAAXXZ(pThis);
}
// OnShowColorPopup(): when a popup is already active the retail closes it by
// sending WM_CLOSE to the popup window and clears 0xb98 (RVA 0x28a09..0x28a31).
// The fresh-popup branch (0x28a36..0x28c39) allocates a 0x2ed8-byte
// CMFCColorPopup, initialises it from the CArray at 0xb30 and the labels, and
// positions it through virtuals -- that machinery is not modeled.
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
        if (hwndPopup != nullptr) {
            ::SendMessageW(hwndPopup, WM_CLOSE, 0, 0);      // 0x1802c7120
        }
        s->m_pColorPopup = nullptr;
        return;
    }
    // Retail fills the color CArray from the system palette when empty
    // (0x1800265f0), then creates and shows the popup.  Only the InitColors
    // side effect is reproduced.
    // TODO(clean-room): transcribed partially -- the 0x2ed8-byte CMFCColorPopup
    // creation (ctor 0x1800b4cb0, vtable 0x1802e13d0, Create 0x180024730) and
    // its positioning virtuals (vslot+0x430 / vslot+0x3a0 / 0x1800ba8a0) are
    // not modeled.
    CColorBtn* b = reinterpret_cast<CColorBtn*>(pThis);
    void* pArray = reinterpret_cast<char*>(pThis) + 0xb30;
    if (*reinterpret_cast<long long*>(reinterpret_cast<char*>(pArray) + 0x10) == 0) {
        (void)impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
            nullptr, pArray);
    }
    (void)b; // keep b referenced for clarity if extended later
}
// OnDraw(CDC*, const CRect&, UINT): retail (RVA 0x28360) ensures the palette
// holder at 0xbc0 (RebuildPalette when null), selects/realises the palette
// into the DC, shrinks the rect by 8 + CMenuImages::Size().cx, draws the
// automatic label (when m_nColor == -1) and the color cell -- a 3-D border
// via CDC::Draw3dRect with unmodeled visual-manager globals (0x1803c1650 /
// 0x1803c165c) and the cell fill.  The label text and the palette-index
// conversion (global 0x1803c18a8 == 8) are not modeled; the cell is filled
// with the current (or automatic) colour.
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
    if (s->m_pPaletteObj != nullptr && s->m_pPaletteObj->m_hPal != nullptr) {
        ::SelectPalette(hdc, s->m_pPaletteObj->m_hPal, FALSE);   // 0x1802c60e8
        ::RealizePalette(hdc);                                   // 0x1802c6238
    }

    // Cell rect: right shrunk by 8 + CMenuImages::Size().cx (default 9).
    // TODO(clean-room): CMenuImages::Size (0x18008f600) is DPI-scaled when the
    // theme global 0x1803be258 is set; the constant 9 is the non-themed size.
    RECT r = *rect;
    r.right -= (8 + 9);

    // 3-D border + cell fill (retail: CDC::Draw3dRect with the unmodeled
    // globals 0x1803c1650/0x1803c165c; DrawEdge is the public equivalent).
    RECT border = r;
    ::InflateRect(&border, -2, -2);
    ::DrawEdge(hdc, &border, BDR_SUNKENOUTER, BF_RECT);
    RECT cell = border;
    ::InflateRect(&cell, -1, -1);

    if (s->m_nColor != -1 && (uiState & ODS_DISABLED) == 0) {
        HBRUSH br = ::CreateSolidBrush(static_cast<COLORREF>(s->m_nColor));
        ::FillRect(hdc, &cell, br);                             // 0x1802c7208
        ::DeleteObject(br);
    } else {
        ::FillRect(hdc, &cell, ::GetSysColorBrush(COLOR_BTNFACE));
    }

    // TODO(clean-room): transcribed partially -- the automatic-label text
    // (m_nColor == -1 && label at 0xbb0 non-empty, drawn via the CDC
    // DrawText virtual with the auto colour 0x1803c1654), the palette-index
    // conversion (GetNearestPaletteIndex 0x1802c6230 + bit 0x18) and the
    // themed focus-rect pass are not modeled.
}
// OnFillBackground(CDC*, const CRect&): non-themed (IsDrawXPTheme == 0) routes
// to CMFCButton::OnFillBackground; themed fills with the global brush at
// 0x1803c1730 (approximated with the button-face brush).  Transcribed from
// retail RVA 0x282e0.
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
    // TODO(clean-room): the global brush at 0x1803c1730 (lazy-init
    // 0x18006a790) is approximated with the button-face brush.
    ::FillRect(hdc, rectClient, ::GetSysColorBrush(COLOR_BTNFACE));
}
// OnDrawBorder(CDC*, CRect&, UINT): non-themed retail (RVA 0x28830) draws a
// sunken 3-D border (inflated -1), and when m_nStyle && m_bClickStarted draws
// a second pressed border using the unmodeled globals 0x1803c164c/0x1803c1660.
// The themed branch dispatches the visual-manager DrawButtonBorder
// (0x180009774 / vslot+0x3a8).  The visual-manager path is not modeled; the
// standard border is drawn through DrawEdge.
// Symbol: ?OnDrawBorder@CMFCColorButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI impl__OnDrawBorder_CMFCColorButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rect, unsigned int /*uiState*/)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    HDC hdc = hdc_of_Cmfccolorbutton(pDC);
    if (hdc == nullptr) return;

    // TODO(clean-room): transcribed partially -- the themed visual-manager
    // dispatch (0x180009774 / vslot+0x3a8) and the border colours
    // (0x1803c1650/0x1803c165c/0x1803c164c/0x1803c1660) are not modeled.
    RECT r = *rect;
    ::DrawEdge(hdc, &r, BDR_RAISEDINNER, BF_RECT | BF_ADJUST);
    if (s->m_nStyle != 0 && s->m_bClickStarted != 0) {
        ::DrawEdge(hdc, &r, BDR_SUNKENOUTER, BF_RECT | BF_ADJUST);
    }
}
// OnDrawFocusRect(CDC*, const CRect&): shrinks the right edge by
// 8 + CMenuImages::Size().cx (the colour-cell gutter) and forwards to
// CMFCButton::OnDrawFocusRect.  Transcribed from retail RVA 0x28980.
// Symbol: ?OnDrawFocusRect@CMFCColorButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawFocusRect_CMFCColorButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rect)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;
    // TODO(clean-room): CMenuImages::Size (0x18008f600) default 9 is used.
    RECT r = *rect;
    r.right -= (8 + 9);
    impl__OnDrawFocusRect_CMFCButton__MEAAXPEAVCDC__AEBVCRect___Z(
        pThis, pDC, &r);
}
// OnInitControl(unsigned __int64, __int64): retail (RVA 0x291a0) reads the
// per-button "MFCCColorButton_EnableOtherButton" / "..._EnableAutomaticButton"
// profile entries (keys at 0x18033dab0 / 0x18033db10 through the CWinApp
// GetProfileString machinery at 0x1803b25e8 / 0x180034080) and, when set,
// calls EnableOtherButton(this, "Other", 1, flag) / EnableAutomaticButton
// (this, "Automatic", 0, flag).  The profile plumbing and the ATL CString
// machinery are not modeled, so the unset-profile path (no calls) is taken.
// Symbol: ?OnInitControl@CMFCColorButton@@IEAA_J_K_J@Z
extern "C" long long MS_ABI impl__OnInitControl_CMFCColorButton__IEAA_J_K_J_Z(
    void* pThis, unsigned long long /*pApp*/, long long /*nCommandId*/)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): transcribed partially -- the CWinApp GetProfileString
    // reads (0x1803b25e8 / 0x180034080 / 0x1801409a0, keys 0x18033dab0 /
    // 0x18033db10) and the conditional EnableOtherButton / EnableAutomaticButton
    // calls (labels "Other" 0x18033daf8 / "Automatic" 0x18033db60) are not
    // modeled; the unset-profile defaults are used.
    return 0;
}
// RebuildPalette(CPalette*): builds a system (pPalette == null) or
// caller-palette (pPalette != null) LOGPALETTE, CreatePalette's it and stores
// the handle in the 0x10-byte holder at 0xbc0, freeing the previous holder.
// Transcribed from retail RVA 0x29000 (the holder vtable delete is replaced
// by DeleteObject + free; foreign blocks -- vtable != 0 -- are not freed).
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
            ::GetSystemPaletteEntries(hdc, 0, 0x100, pLog->palPalEntry);  // 0x1802c6228
        } else if (pLog != nullptr) {
            pLog->palVersion = 0x300;
            pLog->palNumEntries = 0x100;
        }
    } else {
        WORD wCount = 0;
        void* hPalObj = *reinterpret_cast<void**>(
            reinterpret_cast<char*>(pPalette) + 8);      // CPalette::m_hObject
        if (hPalObj != nullptr &&
            ::GetObjectW(hPalObj, sizeof(WORD), &wCount) != 0) {  // 0x1802c6290
            nCount = wCount;
            pLog = static_cast<LOGPALETTE*>(std::malloc(8 + static_cast<size_t>(nCount) * 4));
            if (pLog != nullptr) {
                pLog->palVersion = 0x300;
                pLog->palNumEntries = static_cast<WORD>(nCount);
                ::GetPaletteEntries(reinterpret_cast<HPALETTE>(hPalObj), 0,
                                    static_cast<UINT>(nCount),
                                    pLog->palPalEntry);          // 0x1802c6248
            }
        }
    }

    if (pLog != nullptr) {
        HPALETTE hPal = ::CreatePalette(pLog);                   // 0x1802c6240
        if (hPal != nullptr) {
            holder->m_hPal = hPal;   // retail: CGdiObject::Attach (0x1802a3ed0)
        }
        std::free(pLog);                                         // 0x1802c74e8
    }
    if (hdc != nullptr) ::ReleaseDC(s->m_hWnd, hdc);
    (void)nCount;
}
// SetPalette(CPalette*): releases the color CArray at 0xb30 when non-empty,
// sizes the internal buffer, fills the array through CMFCColorBar::InitColors
// and rebuilds the button palette.  Transcribed from retail RVA 0x28e70; the
// CArray layout is m_pData@+0x8 / m_nSize@+0x10 / m_nMaxSize@+0x18 and the
// existing conservative InitColors leaves the array empty.
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
        // CArray::SetSize(arr, 0): release the buffer (0x180015034).
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

    // Retail: InitColors(&0xb30, pPalette) then RebuildPalette(this, pPalette).
    // TODO(clean-room): InitColors (0x1800265f0) is the conservative
    // implementation in global_cmfccolorbar.cpp and leaves the array empty.
    (void)impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
        pPalette, pArray);
    impl__RebuildPalette_CMFCColorButton__IEAAXPEAVCPalette___Z(pThis, pPalette);
}
// SetDocumentColors(LPCTSTR, CList<COLORREF,COLORREF>&): clears the member
// list at 0xb58, stores the label (CString at 0xba8, null -> L"") and, when
// the label is non-empty, copies the caller's colors into the member list.
// Transcribed from retail RVA 0x28de0; the list copy requires the retail CList
// layout (head@+0x8, count@+0x18, nodes {next@0x0, data@0x10}) and is kept
// out because the stub constructor leaves the member list uninitialized.
// Symbol: ?SetDocumentColors@CMFCColorButton@@QEAAXPEB_WAEAV?$CList@KK@@@Z
extern "C" void MS_ABI impl__SetDocumentColors_CMFCColorButton__QEAAXPEB_WAEAV__CList_KK___Z(
    void* pThis, const wchar_t* lpszLabel, void* /*lstColors*/)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    // TODO(clean-room): retail RemoveAll's the member list at 0xb58
    // (0x180008350) and, when the label is non-empty, walks the caller's
    // CList (head@+0x8; nodes {next@0x0, data@0x10}) appending each colour
    // through 0x180027d84; the member list is not touched because the stub
    // constructor does not initialise it.
    AssignCStr_Cmfccolorbutton(&s->m_strDocLabel, lpszLabel ? lpszLabel : L"");  // 0xba8
}
// SizeToContent(BOOL): runs the CMFCButton base (retail passes bCalcOnly=0),
// adds CMenuImages::Size().cx to the width and resizes the window unless
// bCalcOnly.  Transcribed from retail RVA 0x28270 (SetWindowPos flags 0x16 =
// SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE, x/y = -1).
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
    // TODO(clean-room): CMenuImages::Size().cx (0x18008f600) default 9 is used.
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
// retail RVA 0x28f90 (CWnd::FromHandle on the parent, CWnd::GetDlgCtrlID,
// SendMessageW 0x111).
// Symbol: ?UpdateColor@CMFCColorButton@@MEAAXK@Z
extern "C" void MS_ABI impl__UpdateColor_CMFCColorButton__MEAAXK_Z(
    void* pThis, unsigned long clr)
{
    if (pThis == nullptr) return;
    CColorBtn* s = reinterpret_cast<CColorBtn*>(pThis);
    impl__SetColor_CMFCColorButton__QEAAXK_Z(pThis, clr);
    if (s->m_hWnd == nullptr) return;
    HWND hwndParent = ::GetParent(s->m_hWnd);                    // 0x1802c72d8
    if (hwndParent == nullptr) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hwndParent);
    if (pParent == nullptr || pParent->m_hWnd == nullptr) return;
    WORD wId = static_cast<WORD>(::GetDlgCtrlID(s->m_hWnd));
    ::SendMessageW(pParent->m_hWnd, WM_COMMAND,                   // 0x1802c7120
                   static_cast<WPARAM>(wId),                      // id (BN_CLICKED == 0)
                   reinterpret_cast<LPARAM>(s->m_hWnd));
}
// Symbol: ??0CMFCColorButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCColorButton__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??1CMFCColorButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}
