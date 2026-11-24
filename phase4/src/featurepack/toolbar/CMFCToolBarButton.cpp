// CMFCToolBarButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfctoolbarbutton_msvtbl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCToolBarButtonSupport.h"
#include "detail/MfccoreSupport.h"

#include <string>

static_assert(sizeof(CMFCToolBarButton) == 0x88, "retail CMFCToolBarButton size");
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "m_bUserButton @0x08");
static_assert(offsetof(CMFCToolBarButton, m_bText) == 0x0c, "m_bText @0x0c");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == 0x10, "m_bImage @0x10");
static_assert(offsetof(CMFCToolBarButton, m_bWrap) == 0x14, "m_bWrap @0x14");
static_assert(offsetof(CMFCToolBarButton, m_bWholeText) == 0x18, "m_bWholeText @0x18");
static_assert(offsetof(CMFCToolBarButton, m_bTextBelow) == 0x1c, "m_bTextBelow @0x1c");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_dwdItemData) == 0x30, "m_dwdItemData @0x30");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == 0x48, "m_iImage @0x48");
static_assert(offsetof(CMFCToolBarButton, m_iUserImage) == 0x4c, "m_iUserImage @0x4c");
static_assert(offsetof(CMFCToolBarButton, m_bLocked) == 0x50, "m_bLocked @0x50");
static_assert(offsetof(CMFCToolBarButton, m_bDisableFill) == 0x58, "m_bDisableFill @0x58");
static_assert(offsetof(CMFCToolBarButton, m_bExtraSize) == 0x5c, "m_bExtraSize @0x5c");
static_assert(offsetof(CMFCToolBarButton, m_bHorz) == 0x60, "m_bHorz @0x60");
static_assert(offsetof(CMFCToolBarButton, m_bVisible) == 0x64, "m_bVisible @0x64");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_sizeText) == 0x78, "m_sizeText @0x78");
static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "m_pWndParent @0x80");

// Definitions are in core/runtime/CObjectTypeChecks.cpp and
// featurepack/toolbar/RuntimeClasses.cpp, respectively.
extern "C" BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();

// Retail exports a CString object at mfc140u data RVA 0x3c32a0; afxstr.h models
// CString with its ABI-compatible single data pointer.
static_assert(sizeof(CString) == sizeof(void*), "CString export object is one pointer");
// Symbol: ?m_strClipboardFormatName@CMFCToolBarButton@@2V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@A
extern "C" CString MS_ABI impl__m_strClipboardFormatName_CMFCToolBarButton__2V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = {};

// Symbol: ?SetClipboardFormatName@CMFCToolBarButton@@SAXPEB_W@Z
void CMFCToolBarButton::SetClipboardFormatName(const wchar_t* lpszName) {
    if (lpszName && *lpszName) {
        m_strClipboardFormatName = lpszName;
        impl__m_strClipboardFormatName_CMFCToolBarButton__2V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A = lpszName;
    } else {
        m_strClipboardFormatName.Empty();
        impl__m_strClipboardFormatName_CMFCToolBarButton__2V__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__A.Empty();
    }
}
CString CMFCToolBarButton::m_strClipboardFormatName;
BOOL CMFCToolBarButton::m_bWrapText = TRUE;
BOOL CMFCToolBarButton::m_bUpdateImages = FALSE;
// ---- named exports: real bodies (flip the weak null stubs to real behavior) ----
// Symbol: ?Serialize@CMFCToolBarButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    tbb_Serialize((CMFCToolBarButton*)pThis, ar);
}
// Symbol: ?CopyFrom@CMFCToolBarButton@@UEAAXAEBV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* src) {
    tbb_CopyFrom((CMFCToolBarButton*)pThis, src);
}
// Symbol: ?CompareWith@CMFCToolBarButton@@UEBAHAEBV1@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarButton__UEBAHAEBV1__Z(const void* pThis, const CMFCToolBarButton* other) {
    return tbb_CompareWith((const CMFCToolBarButton*)pThis, other);
}
// Symbol: ?SetImage@CMFCToolBarButton@@UEAAXH@Z
extern "C" void MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage) {
    tbb_SetImage((CMFCToolBarButton*)pThis, iImage);
}
// Symbol: ?ResetImageToDefault@CMFCToolBarButton@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(void*) {
    /* base class: no-op (harvested from real mfc140u) */
}
// Stub: mfc140u data RVA 0x3b20c8 needs a 0x38-byte retail CList object; the local CList storage is not ABI-compatible.
// Symbol: ?m_lstProtectedCommands@CMFCToolBarButton@@1V?$CList@II@@A
extern "C" void* MS_ABI impl__m_lstProtectedCommands_CMFCToolBarButton__1V__CList_II__A = {};

// Stub: retail mfc140u RVA 0x15c790 performs OLE clipboard retrieval and format decoding, which is not modeled here.
// Symbol: ?CreateFromOleData@CMFCToolBarButton@@SAPEAV1@PEAVCOleDataObject@@@Z
extern "C" CMFCToolBarButton* MS_ABI impl__CreateFromOleData_CMFCToolBarButton__SAPEAV1_PEAVCOleDataObject___Z(
    COleDataObject* pDataObject) {
    (void)pDataObject;
    return nullptr;
}

// Stub: retail mfc140u RVA 0x15e140 can edit the destination text after command/string lookup; that path is not transcribed.
// Symbol: ?ExportToMenuButton@CMFCToolBarButton@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" BOOL MS_ABI impl__ExportToMenuButton_CMFCToolBarButton__UEBAHAEAVCMFCToolBarMenuButton___Z(
    const void* pThis, CMFCToolBarMenuButton* pMenuButton) {
    (void)pThis;
    (void)pMenuButton;
    return FALSE;
}

extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
extern "C" int impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" int impl__m_bAltCustomizeMode_CMFCToolBar__1HA;
extern "C" void* impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
extern "C" void* MS_ABI impl__FindTool_CUserToolsManager__QEBAPEAVCUserTool__I_Z(const void*, UINT);
extern "C" void MS_ABI impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(void*, CDC*, const CRect*);
extern "C" void MS_ABI impl__OnFillButtonInterior_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1__Z(
    CMFCVisualManager*, CDC*, CMFCToolBarButton*, CRect, int);

namespace {
constexpr UINT kPaintChecked = UINT(TBSTATE_CHECKED) << 16;
constexpr UINT kPaintPressed = UINT(TBSTATE_PRESSED) << 16;
constexpr UINT kPaintDisabled = UINT(TBSTATE_ENABLED) << 16;
constexpr UINT kPaintIndeterminate = UINT(TBSTATE_INDETERMINATE) << 16;

CMFCVisualManager* ToolbarPaintManager() {
    return static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
}

CMFCVisualManager::AFX_BUTTON_STATE ToolbarPaintState(const CMFCToolBarButton& button, BOOL hot, BOOL customize) {
    if (!customize) {
        if (hot) return CMFCVisualManager::ButtonsIsHighlighted;
        if (button.m_nStyle & (kPaintChecked | kPaintPressed)) return CMFCVisualManager::ButtonsIsPressed;
    }
    return CMFCVisualManager::ButtonsIsRegular;
}

struct ToolbarLabel {
    std::wstring text;
    size_t mnemonic = std::wstring::npos;
};

ToolbarLabel ParseToolbarLabel(const CMFCToolBarButton& button) {
    ToolbarLabel label;
    const auto* text = static_cast<const wchar_t*>(button.m_strText);
    for (size_t i = 0; text && text[i]; ++i) {
        if (text[i] == L'&' && !(button.m_nStyle & TBSTYLE_NOPREFIX)) {
            if (!text[i + 1]) break;
            if (text[i + 1] != L'&' && label.mnemonic == std::wstring::npos)
                label.mnemonic = label.text.size();
            ++i;
        }
        label.text.push_back(text[i]);
    }
    return label;
}

SIZE ToolbarTextExtent(HDC dc, const std::wstring& label) {
    SIZE size{};
    if (dc && !label.empty())
        ::GetTextExtentPoint32W(dc, label.data(), static_cast<int>(label.size()), &size);
    return size;
}

void PaintVerticalToolbarText(HDC dc, const CRect& bounds, const CMFCToolBarButton& button, BOOL noCues) {
    const auto label = ParseToolbarLabel(button);
    if (label.text.empty()) return;
    LOGFONTW logFont{};
    if (!::GetObjectW(::GetCurrentObject(dc, OBJ_FONT), sizeof(logFont), &logFont)) return;
    const SIZE textSize = ToolbarTextExtent(dc, label.text);
    const int x = bounds.left + (bounds.Width() - textSize.cy) / 2;
    const int y = bounds.bottom - (std::max)(0, (bounds.Height() - static_cast<int>(textSize.cx)) / 2);
    SIZE prefix{}, mnemonic{};
    const bool underline = !noCues && label.mnemonic < label.text.size();
    if (underline) {
        ::GetTextExtentPoint32W(dc, label.text.data(), static_cast<int>(label.mnemonic), &prefix);
        ::GetTextExtentPoint32W(dc, label.text.data() + label.mnemonic, 1, &mnemonic);
    }
    logFont.lfEscapement = logFont.lfOrientation = 900;
    const HFONT font = ::CreateFontIndirectW(&logFont);
    if (!font) return;
    const auto old = ::SelectObject(dc, font);
    if (old) {
        ::SetTextAlign(dc, TA_LEFT | TA_TOP | TA_NOUPDATECP);
        ::TextOutW(dc, x, y, label.text.data(), static_cast<int>(label.text.size()));
        if (underline) {
            const int lineX = x + textSize.cy - 1;
            const int lineY = y - prefix.cx;
            const HPEN pen = ::CreatePen(PS_SOLID, 1, ::GetTextColor(dc));
            if (pen) {
                const auto oldPen = ::SelectObject(dc, pen);
                if (oldPen) {
                    ::MoveToEx(dc, lineX, lineY, nullptr);
                    ::LineTo(dc, lineX, lineY - mnemonic.cx);
                    ::SelectObject(dc, oldPen);
                }
                ::DeleteObject(pen);
            }
        }
        ::SelectObject(dc, old);
    }
    ::DeleteObject(font);
}
} // namespace

// Symbol: ?FillInterior@CMFCToolBarButton@@IEAAXPEAVCDC@@AEBVCRect@@HH@Z
extern "C" void MS_ABI impl__FillInterior_CMFCToolBarButton__IEAAXPEAVCDC__AEBVCRect__HH_Z(
    void* self, CDC* dc, const CRect* bounds, BOOL hot, BOOL)
{
    auto* button = static_cast<CMFCToolBarButton*>(self);
    if (!button || !dc || !dc->GetSafeHdc() || !bounds || bounds->IsRectEmpty() || button->m_bDisableFill)
        return;
    const bool customize = impl__m_bCustomizeMode_CMFCToolBar__1HA &&
        !impl__m_bAltCustomizeMode_CMFCToolBar__1HA && !button->m_bLocked;
    const auto state = ToolbarPaintState(*button, hot, customize);
    if (auto* visual = ToolbarPaintManager()) {
        impl__OnFillButtonInterior_CMFCVisualManager__UEAAXPEAVCDC__PEAVCMFCToolBarButton__VCRect__W4AFX_BUTTON_STATE_1__Z(
            visual, dc, button, *bounds, state);
    } else if (!customize && (button->m_nStyle & (kPaintChecked | kPaintIndeterminate))) {
        ::FillRect(dc->GetSafeHdc(), *bounds, ::GetSysColorBrush(COLOR_BTNFACE));
    }
}

// Retail mfc140u RVA 0x15eab0 returns zero unless the button is visible and
// has a nonempty rect, then returns the inverse of style bit 0.
// Symbol: ?GetAccCount@CMFCToolBarButton@@UEAAHXZ
extern "C" int MS_ABI impl__GetAccCount_CMFCToolBarButton__UEAAHXZ(void* pThis) {
    const auto* pButton = static_cast<const CMFCToolBarButton*>(pThis);
    if (!pButton->m_bVisible || ::IsRectEmpty(static_cast<const RECT*>(pButton->m_rect))) return 0;
    return (pButton->m_nStyle & 1u) == 0 ? 1 : 0;
}

// Stub: retail mfc140u RVA 0x15c6c0 registers and caches the clipboard format through the static name and MFC global data.
// Symbol: ?GetClipboardFormat@CMFCToolBarButton@@SAGXZ
extern "C" unsigned short MS_ABI impl__GetClipboardFormat_CMFCToolBarButton__SAGXZ() {
    return 0;
}

// Retail mfc140u RVA 0x15c0c0 clears m_bUserButton/m_bText at +0x08, stores
// m_bImage=TRUE and m_bWrap=FALSE together at +0x10, and leaves m_bIsHidden untouched.
// Symbol: ?Initialize@CMFCToolBarButton@@IEAAXXZ
extern "C" void MS_ABI impl__Initialize_CMFCToolBarButton__IEAAXXZ(void* pThis) {
    auto* pButton = static_cast<CMFCToolBarButton*>(pThis);
    pButton->m_bImage = TRUE;
    pButton->m_bWrap = FALSE;
    pButton->m_nID = 0;
    pButton->m_iImage = -1;
    pButton->m_iUserImage = -1;
    pButton->m_bUserButton = FALSE;
    pButton->m_bText = FALSE;
    pButton->m_bTextBelow = FALSE;
    pButton->m_bLocked = FALSE;
    pButton->m_dwdItemData = 0;
    pButton->m_bWholeText = TRUE;
    ::SetRectEmpty(static_cast<RECT*>(pButton->m_rect));
    pButton->m_sizeText.cx = 0;
    pButton->m_sizeText.cy = 0;
    pButton->m_bDisableFill = FALSE;
    pButton->m_bHorz = TRUE;
    pButton->m_bVisible = TRUE;
    pButton->m_pWndParent = nullptr;
}

// Stub: retail mfc140u RVA 0x15e5d0 walks the owning toolbar's buttons and applies hidden/group-boundary rules, not reproduced here.
// Symbol: ?IsFirstInGroup@CMFCToolBarButton@@UEBAHXZ
extern "C" BOOL MS_ABI impl__IsFirstInGroup_CMFCToolBarButton__UEBAHXZ(const void* pThis) {
    (void)pThis;
    return FALSE;
}

// Stub: retail mfc140u RVA 0x15e680 walks the owning toolbar's trailing buttons and applies hidden/group-boundary rules, not reproduced here.
// Symbol: ?IsLastInGroup@CMFCToolBarButton@@UEBAHXZ
extern "C" BOOL MS_ABI impl__IsLastInGroup_CMFCToolBarButton__UEBAHXZ(const void* pThis) {
    (void)pThis;
    return FALSE;
}

SIZE CMFCToolBarButton::OnCalculateSize(CDC* dc, const CSize& defaultSize, BOOL horizontal) {
    m_sizeText = CSize(0, 0);
    if (!m_bVisible || m_bIsHidden) return {};
    SIZE size{defaultSize.cx, defaultSize.cy};
    if (m_nStyle & TBSTYLE_SEP) {
        if (horizontal) size.cx = m_iImage > 0 ? m_iImage : 8;
        else size.cy = 8;
        return size;
    }
    const bool hasImage = m_bImage && GetImage() >= 0;
    if (!hasImage) {
        if (horizontal) size.cx = 0;
        else size.cy = 0;
    }
    const HDC hdc = dc ? dc->GetSafeHdc() : nullptr;
    if (!hdc || m_strText.IsEmpty()) return size;
    if (m_bTextBelow && horizontal) {
        CRect bounds(0, 0, (std::max)(0, defaultSize.cx) * 3, defaultSize.cy);
        const UINT prefix = m_nStyle & TBSTYLE_NOPREFIX ? DT_NOPREFIX : 0;
        ::DrawTextW(hdc, m_strText, m_strText.GetLength(), bounds,
                    DT_CALCRECT | DT_CENTER | DT_WORDBREAK | prefix);
        m_sizeText = CSize(bounds.Width() + 6, bounds.Height());
        size.cx = (std::max)(size.cx, static_cast<LONG>(m_sizeText.cx)) + 12;
        size.cy += m_sizeText.cy + 1;
    } else if (m_bText) {
        const SIZE extent = ToolbarTextExtent(hdc, ParseToolbarLabel(*this).text);
        const LONG span = extent.cx + (hasImage ? 6 : 9);
        if (horizontal) size.cx += span;
        else size.cy += span;
    }
    return size;
}

// Symbol: ?OnCalculateSize@CMFCToolBarButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* self, SIZE* result, CDC* dc, const SIZE* defaultSize, BOOL horizontal)
{
    if (result) {
        *result = self && defaultSize ? static_cast<CMFCToolBarButton*>(self)->OnCalculateSize(
            dc, CSize(defaultSize->cx, defaultSize->cy), horizontal) : SIZE{};
    }
    return result;
}

// Retail mfc140u RVA 0x15e570 stores the parent and clears m_bExtraSize, then tests
// CMFCToolBar RTTI and calls virtual slot +0x6c8. No MSVC-layout CMFCToolBar
// vtable is defined in this tree, so this uses afxtoolbar.h:174's base TRUE result
// and deliberately does not dispatch derived overrides.
// Symbol: ?OnChangeParentWnd@CMFCToolBarButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(
    void* pThis, CWnd* pWndParent) {
    auto* pButton = static_cast<CMFCToolBarButton*>(pThis);
    pButton->m_bExtraSize = FALSE;
    pButton->m_pWndParent = pWndParent;
    if (pWndParent != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pWndParent, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ())) {
        pButton->m_bExtraSize = TRUE;
    }
}

void CMFCToolBarButton::OnDraw(CDC* dc, const CRect& bounds, CMFCToolBarImages* images,
                              BOOL horizontal, BOOL customize, BOOL hot, BOOL drawBorder, BOOL grayDisabled) {
    const HDC hdc = dc ? dc->GetSafeHdc() : nullptr;
    if (!hdc || !m_bVisible || m_bIsHidden || (m_nStyle & TBSTYLE_SEP) || bounds.IsRectEmpty()) return;
    const int saved = ::SaveDC(hdc);
    if (!saved) return;
    ::IntersectClipRect(hdc, bounds.left, bounds.top, bounds.right, bounds.bottom);

    const bool disabled = !customize && (m_nStyle & kPaintDisabled) != 0;
    const bool pressed = !customize && (m_nStyle & (kPaintPressed | kPaintChecked)) != 0;
    const bool below = horizontal && m_bTextBelow && !m_strText.IsEmpty();
    const bool label = !m_strText.IsEmpty() && (m_bText || below);
    const auto state = ToolbarPaintState(*this, hot, customize);
    impl__FillInterior_CMFCToolBarButton__IEAAXPEAVCDC__AEBVCRect__HH_Z(this, dc, &bounds, hot, FALSE);

    void* userTool = nullptr;
    if (m_bImage && impl__afxUserToolsManager__3PEAVCUserToolsManager__EA)
        userTool = impl__FindTool_CUserToolsManager__QEBAPEAVCUserTool__I_Z(
            impl__afxUserToolsManager__3PEAVCUserToolsManager__EA, m_nID);
    const int index = GetImage();
    const bool indexed = images && m_bImage && index >= 0 && index < images->GetCount();
    CSize image = indexed ? images->GetImageSize(TRUE) : CSize(0, 0);
    if (indexed && (image.cx <= 0 || image.cy <= 0)) image = images->GetImageSize();
    if (userTool && (image.cx <= 0 || image.cy <= 0))
        image = CSize(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));

    const UINT prefix = m_nStyle & TBSTYLE_NOPREFIX ? DT_NOPREFIX : 0;
    const BOOL hideCues = m_pWndParent && m_pWndParent->GetSafeHwnd() &&
        (::SendMessageW(m_pWndParent->GetSafeHwnd(), WM_QUERYUISTATE, 0, 0) & UISF_HIDEACCEL);
    const UINT cues = hideCues ? DT_HIDEPREFIX : 0;
    CRect text = bounds;
    text.DeflateRect(3, 2);
    if (below) {
        CRect measured(0, 0, text.Width(), 0);
        ::DrawTextW(hdc, m_strText, m_strText.GetLength(), measured,
                    DT_CALCRECT | DT_WORDBREAK | DT_CENTER | prefix);
        text.top = (std::max)(text.top, text.bottom - measured.Height());
    }

    if (userTool || indexed) {
        int x = bounds.left + (bounds.Width() - image.cx) / 2;
        int y = bounds.top + (bounds.Height() - image.cy) / 2;
        if (below) y = bounds.top + (std::max)(0, (text.top - bounds.top - image.cy) / 2);
        else if (label && horizontal) {
            x = text.left;
            text.left = x + image.cx + 3;
        } else if (label) {
            y = text.top;
            text.top = y + image.cy + 3;
        }
        if (pressed) { ++x; ++y; }
        if (userTool) {
            const CRect icon(x, y, x + image.cx, y + image.cy);
            impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(userTool, dc, &icon);
        } else {
            images->Draw(dc, x, y, index, FALSE, disabled && grayDisabled,
                         !customize && (m_nStyle & kPaintIndeterminate), FALSE, !hot);
        }
    }

    if (label && !text.IsRectEmpty()) {
        if (pressed) text.OffsetRect(1, 1);
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, ::GetSysColor(disabled ? COLOR_GRAYTEXT : COLOR_BTNTEXT));
        if (horizontal) {
            const UINT layout = below ? DT_CENTER | DT_WORDBREAK : DT_SINGLELINE | DT_VCENTER |
                (indexed || userTool ? DT_LEFT : DT_CENTER);
            ::DrawTextW(hdc, m_strText, m_strText.GetLength(), text, layout | prefix | cues | DT_END_ELLIPSIS);
        } else {
            PaintVerticalToolbarText(hdc, text, *this, hideCues);
        }
    }
    if (drawBorder && !disabled && state != CMFCVisualManager::ButtonsIsRegular) {
        if (auto* visual = ToolbarPaintManager()) {
            visual->CMFCVisualManager::OnDrawButtonBorder(dc, this, bounds, state);
        } else {
            RECT border{bounds.left, bounds.top, bounds.right, bounds.bottom};
            ::DrawEdge(hdc, &border, pressed ? BDR_SUNKENOUTER : BDR_RAISEDINNER, BF_RECT);
        }
    }
    ::RestoreDC(hdc, saved);
}

// Symbol: ?OnDraw@CMFCToolBarButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* self, CDC* dc, const CRect* bounds, CMFCToolBarImages* images,
    BOOL horizontal, BOOL customize, BOOL hot, BOOL drawBorder, BOOL grayDisabled)
{
    if (self && bounds)
        static_cast<CMFCToolBarButton*>(self)->OnDraw(dc, *bounds, images, horizontal, customize, hot,
                                                     drawBorder, grayDisabled);
}

extern "C" CSize* MS_ABI impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(CSize*);
extern "C" void* impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA;
extern "C" CMFCToolBarImages impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A;
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;

int CMFCToolBarButton::OnDrawOnCustomizeList(CDC* dc, const CRect& bounds, BOOL selected) {
    const HDC hdc = dc ? dc->GetSafeHdc() : nullptr;
    if (!hdc || bounds.IsRectEmpty()) return 0;
    const int saved = ::SaveDC(hdc);
    if (!saved) return 0;
    ::IntersectClipRect(hdc, bounds.left, bounds.top, bounds.right, bounds.bottom);
    ::FillRect(hdc, bounds, ::GetSysColorBrush(selected ? COLOR_HIGHLIGHT : COLOR_WINDOW));

    const int index = GetImage();
    auto* images = m_bUserButton ?
        static_cast<CMFCToolBarImages*>(impl__m_pUserImages_CMFCToolBar__1PEAVCMFCToolBarImages__EA) :
        &impl__m_MenuImages_CMFCToolBar__1VCMFCToolBarImages__A;
    if (!m_bUserButton && (index < 0 || index >= images->GetCount()))
        images = &impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;
    CSize cell;
    impl__GetMenuImageSize_CMFCToolBar__SA_AVCSize__XZ(&cell);
    const int width = cell.cx > 0 ? cell.cx : 16;
    const int height = cell.cy > 0 ? cell.cy : 15;
    const int imageSpace = width + 6;
    const int y = bounds.top + (std::max)(0, (bounds.Height() - height) / 2);
    const CRect glyph(bounds.left + 3, y, bounds.left + 3 + width, y + height);
    void* tool = nullptr;
    if (!m_bUserButton && impl__afxUserToolsManager__3PEAVCUserToolsManager__EA)
        tool = impl__FindTool_CUserToolsManager__QEBAPEAVCUserTool__I_Z(
            impl__afxUserToolsManager__3PEAVCUserToolsManager__EA, m_nID);
    if (tool) impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(tool, dc, &glyph);
    else if (images && index >= 0 && index < images->GetCount())
        images->DrawEx(dc, glyph, index, 3, 3);

    CRect label = bounds;
    label.left += imageSpace;
    int used = imageSpace;
    if (!m_strText.IsEmpty() && !label.IsRectEmpty()) {
        ::SetBkMode(hdc, TRANSPARENT);
        ::SetTextColor(hdc, ::GetSysColor(selected ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT));
        const UINT prefix = m_nStyle & TBSTYLE_NOPREFIX ? DT_NOPREFIX : 0;
        ::DrawTextW(hdc, m_strText, m_strText.GetLength(), label, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | prefix);
        used += (std::min)(label.Width(), static_cast<int>(ToolbarTextExtent(hdc, ParseToolbarLabel(*this).text).cx));
    }
    ::RestoreDC(hdc, saved);
    return used;
}

// Symbol: ?OnDrawOnCustomizeList@CMFCToolBarButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    void* self, CDC* dc, const CRect* bounds, BOOL selected)
{
    return self && bounds ?
        static_cast<CMFCToolBarButton*>(self)->OnDrawOnCustomizeList(dc, *bounds, selected) : 0;
}

// Stub: the mfc140u toolbar-button vtable points to RVA 0x15e070 (mfc140u); its parent/tool-tip virtual dispatch is not reproduced.
// Symbol: ?OnToolHitTest@CMFCToolBarButton@@UEAAHPEBVCWnd@@PEAUtagTOOLINFOW@@@Z
extern "C" int MS_ABI impl__OnToolHitTest_CMFCToolBarButton__UEAAHPEBVCWnd__PEAUtagTOOLINFOW___Z(
    void* pThis, const CWnd* pWnd, TOOLINFOW* pToolInfo) {
    (void)pThis;
    (void)pWnd;
    (void)pToolInfo;
    return 0;
}

// Stub: the mfc140u toolbar-button vtable points to RVA 0x15d900 (mfc140u); its OLE data-object publication path is not reproduced.
// Symbol: ?PrepareDrag@CMFCToolBarButton@@UEAAHAEAVCOleDataSource@@@Z
extern "C" int MS_ABI impl__PrepareDrag_CMFCToolBarButton__UEAAHAEAVCOleDataSource___Z(
    void* pThis, COleDataSource* pSource) {
    (void)pThis;
    (void)pSource;
    return 0;
}

// Stub: retail mfc140u RVA 0x15e730 builds accessibility data from the parent, command, strings, and image; that path is not reproduced.
// Symbol: ?SetACCData@CMFCToolBarButton@@UEAAHPEAVCWnd@@AEAVCAccessibilityData@@@Z
extern "C" int MS_ABI impl__SetACCData_CMFCToolBarButton__UEAAHPEAVCWnd__AEAVCAccessibilityData___Z(
    void* pThis, CWnd* pParent, CAccessibilityData* pData) {
    (void)pThis;
    (void)pParent;
    (void)pData;
    return 0;
}

// Stub: the mfc140u symbol map has no RVA for this export; its ANSI twin's body clears the global list and copies retail CList nodes (head +0x08, value +0x10, next +0x00). OpenMFC's CList layout/storage is incompatible.
// Symbol: ?SetProtectedCommands@CMFCToolBarButton@@SAXAEBV?$CList@II@@@Z
extern "C" void MS_ABI impl__SetProtectedCommands_CMFCToolBarButton__SAXAEBV__CList_II___Z(
    const CList<UINT, UINT>* pCommands) {
    (void)pCommands;
}
