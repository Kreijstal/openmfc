// CMFCAutoHideButton — OpenMFC implementation.
#define OPENMFC_APPCORE_IMPL
#include "detail/CMFCAutoHideButtonSupport.h"
#include "detail/DockingState.h"

#include <algorithm>
#include <cstring>
#include <new>
#include <string>

extern "C" void AFXAPI impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
    CDockablePane*, void*, void*);
extern "C" void AFXAPI impl__Slide_CDockablePane__UEAAXHH_Z(CDockablePane*, BOOL, BOOL);
extern "C" void AFXAPI impl__UnSetAutoHideMode_CDockablePane__UEAAXPEAV1__Z(CDockablePane*, CDockablePane*);
extern "C" void AFXAPI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void*);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[];
extern "C" int impl__m_bOverlappingTabs_CMFCAutoHideButton__2HA;
extern "C" int impl__m_nBorderSize_CMFCAutoHideButton__2HA;
extern "C" int impl__m_nMarginSize_CMFCAutoHideButton__2HA;
extern "C" int impl__m_nTextMargin_CMFCAutoHideButton__2HA;
extern "C" int impl__m_nTextSizeNoIcon_CMFCAutoHideButton__2HA;
extern "C" void AFXAPI impl__OnFillAutoHideButtonBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
    CMFCVisualManager*, CDC*, CRect, void*);
extern "C" COLORREF AFXAPI impl__GetAutoHideButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCAutoHideButton___Z(
    CMFCVisualManager*, void*);

IMPLEMENT_DYNCREATE(CMFCAutoHideButton, CObject)

namespace {
std::wstring Title(const CDockablePane* pane) {
    if (!pane || !pane->m_hWnd) return {};
    std::wstring title(::GetWindowTextLengthW(pane->m_hWnd) + 1, L'\0');
    title.resize(::GetWindowTextW(pane->m_hWnd, title.data(), static_cast<int>(title.size())));
    return title;
}

HICON PaneIcon(const CDockablePane* pane) {
    if (!pane || !pane->m_hWnd) return nullptr;
    return reinterpret_cast<HICON>(::SendMessageW(pane->m_hWnd, WM_GETICON, ICON_SMALL, 0));
}

HFONT TabFont(BOOL horizontal) {
    auto* data = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    int initialized = 0;
    std::memcpy(&initialized, data, sizeof(initialized));
    if (!initialized) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(data);
        initialized = 1;
        std::memcpy(data, &initialized, sizeof(initialized));
    }
    // Native fontRegular and fontVert are CFont objects with handles at +8.
    HFONT font = nullptr;
    std::memcpy(&font, data + (horizontal ? 0x1b0 : 0x210), sizeof(font));
    return font ? font : static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
}
}

BOOL CMFCAutoHideButton::Create(CMFCAutoHideBar* bar, CDockablePane* pane, DWORD alignment) {
    if (!bar || !pane) return FALSE;
    m_pParentBar = bar;
    m_dwAlignment = alignment;
    ReplacePane(pane);
    const CSize size = GetSize();
    m_rect.SetRect(0, 0, size.cx, size.cy);
    m_bVisible = TRUE;
    return TRUE;
}

CMFCAutoHideButton::~CMFCAutoHideButton() {
    ReplacePane(nullptr);
}

void CMFCAutoHideButton::ReplacePane(CDockablePane* pane) {
    bool ownsPrevious = false;
    if (m_pAutoHideWindow && m_pAutoHideWindow != pane) {
        std::lock_guard<std::mutex> lock(openmfc::docking_state::PaneCoreStateMutex());
        const auto& states = openmfc::docking_state::PaneCoreStates();
        const auto found = states.find(m_pAutoHideWindow);
        ownsPrevious = found != states.end() && found->second.autoHideButton == this;
    }
    if (ownsPrevious)
        impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
            m_pAutoHideWindow, nullptr, nullptr);
    m_pAutoHideWindow = pane;
    if (pane)
        impl__SetAutoHideParents_CDockablePane__QEAAXPEAVCMFCAutoHideBar__PEAVCMFCAutoHideButton___Z(
            pane, m_pParentBar, this);
}

CSize CMFCAutoHideButton::GetTextSize() const {
    if (!m_pParentBar || !m_pAutoHideWindow) return CSize(0, 0);
    const auto title = Title(m_pAutoHideWindow);
    const HWND window = m_pParentBar->m_hWnd;
    const HDC dc = title.empty() ? nullptr : ::GetDC(window);
    SIZE measured{};
    if (dc) {
        const auto oldFont = ::SelectObject(dc, TabFont(IsHorizontal()));
        if (::GetTextExtentPoint32W(dc, title.data(), static_cast<int>(title.size()), &measured)) {
            measured.cx += impl__m_nMarginSize_CMFCAutoHideButton__2HA;
            measured.cy += impl__m_nMarginSize_CMFCAutoHideButton__2HA;
        }
        if (oldFont) ::SelectObject(dc, oldFont);
        ::ReleaseDC(window, dc);
    }
    if (!m_pParentBar->m_bActiveInGroup && impl__m_bOverlappingTabs_CMFCAutoHideButton__2HA)
        measured.cx = 0;
    return IsHorizontal() ? CSize(measured.cx, measured.cy) : CSize(measured.cy, measured.cx);
}

CSize CMFCAutoHideButton::GetSize() const {
    const int margin = impl__m_nMarginSize_CMFCAutoHideButton__2HA;
    const int padding = margin + 2 * impl__m_nBorderSize_CMFCAutoHideButton__2HA;
    if (!m_pAutoHideWindow) return CSize(padding, padding);
    const CSize text = GetTextSize();
    const HICON icon = PaneIcon(m_pAutoHideWindow);
    const int iconWidth = icon ? ::GetSystemMetrics(SM_CXSMICON) : 0;
    const int iconHeight = icon ? ::GetSystemMetrics(SM_CYSMICON) : 0;
    const int length = IsHorizontal() ? text.cx : text.cy;
    const int gap = !icon ? impl__m_nTextSizeNoIcon_CMFCAutoHideButton__2HA + margin
        : length > 0 ? (IsHorizontal() ? iconWidth : iconHeight) + impl__m_nTextMargin_CMFCAutoHideButton__2HA
                     : margin + 1;
    if (IsHorizontal())
        return CSize(padding + length + iconWidth + gap, padding + (std::max)(text.cy, iconHeight));
    return CSize(padding + (std::max)(text.cx, iconWidth), padding + length + iconHeight + gap);
}

void CMFCAutoHideButton::ShowButton(BOOL show) { m_bVisible = show; }
void CMFCAutoHideButton::HighlightButton(BOOL highlight) { m_bHighlighted = highlight; }

void CMFCAutoHideButton::Move(int offset) {
    ::OffsetRect(m_rect, IsHorizontal() ? offset : 0, IsHorizontal() ? 0 : offset);
}

void CMFCAutoHideButton::ShowAttachedWindow(BOOL show) {
    if (m_pAutoHideWindow && !!::IsWindowVisible(m_pAutoHideWindow->m_hWnd) != !!show)
        impl__Slide_CDockablePane__UEAAXHH_Z(m_pAutoHideWindow, show, TRUE);
    if (m_pParentBar) m_pParentBar->SetActiveInGroup(show);
}

void CMFCAutoHideButton::UnSetAutoHideMode(CDockablePane* firstInGroup) {
    if (m_pAutoHideWindow)
        impl__UnSetAutoHideMode_CDockablePane__UEAAXPEAV1__Z(m_pAutoHideWindow, firstInGroup);
}

void CMFCAutoHideButton::OnFillBackground(CDC* dc, CRect rect) {
    impl__OnFillAutoHideButtonBackground_CMFCVisualManager__UEAAXPEAVCDC__VCRect__PEAVCMFCAutoHideButton___Z(
        CMFCVisualManager::GetInstance(), dc, rect, this);
}

void CMFCAutoHideButton::OnDrawBorder(CDC* dc, CRect bounds, CRect) {
    if (dc && dc->GetSafeHdc())
        ::FrameRect(dc->GetSafeHdc(), bounds, ::GetSysColorBrush(m_bHighlighted ? COLOR_HIGHLIGHT : COLOR_BTNSHADOW));
}

void CMFCAutoHideButton::OnDraw(CDC* dc) {
    if (!dc || !dc->GetSafeHdc() || !m_bVisible || m_rect.IsRectEmpty()) return;
    OnFillBackground(dc, m_rect);
    OnDrawBorder(dc, m_rect, CRect(1, 1, 1, 1));
    const auto title = Title(m_pAutoHideWindow);
    const auto icon = PaneIcon(m_pAutoHideWindow);
    const int padding = impl__m_nMarginSize_CMFCAutoHideButton__2HA + impl__m_nBorderSize_CMFCAutoHideButton__2HA;
    CRect content = m_rect;
    content.DeflateRect(padding, padding);
    const HDC handle = dc->GetSafeHdc();
    const int saved = ::SaveDC(handle);
    if (!saved) return;
    ::IntersectClipRect(handle, content.left, content.top, content.right, content.bottom);
    ::SelectObject(handle, TabFont(IsHorizontal()));
    ::SetBkMode(handle, TRANSPARENT);
    ::SetTextColor(handle, impl__GetAutoHideButtonTextColor_CMFCVisualManager__UEAAKPEAVCMFCAutoHideButton___Z(
        CMFCVisualManager::GetInstance(), this));
    if (icon) {
        const int width = ::GetSystemMetrics(SM_CXSMICON);
        const int height = ::GetSystemMetrics(SM_CYSMICON);
        const int x = IsHorizontal() ? content.left : content.left + (content.Width() - width) / 2;
        const int y = IsHorizontal() ? content.top + (content.Height() - height) / 2 : content.bottom - height;
        ::DrawIconEx(handle, x, y, icon, width, height, 0, nullptr, DI_NORMAL);
        const int gap = impl__m_nTextMargin_CMFCAutoHideButton__2HA;
        if (IsHorizontal()) content.left += width + gap;
        else content.bottom -= height + gap;
    }
    if (!title.empty()) {
        const int length = static_cast<int>(title.size());
        if (IsHorizontal())
            ::DrawTextW(handle, title.data(), length, content, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
        else
            ::TextOutW(handle, content.left, content.bottom, title.data(), length);
    }
    ::RestoreDC(handle, saved);
}

// Symbol: ??0CMFCAutoHideButton@@QEAA@XZ
extern "C" void* AFXAPI impl___0CMFCAutoHideButton__QEAA_XZ(void* storage) {
    return storage ? ::new (storage) CMFCAutoHideButton : nullptr;
}
// Symbol: ??1CMFCAutoHideButton@@UEAA@XZ
extern "C" void AFXAPI impl___1CMFCAutoHideButton__UEAA_XZ(CMFCAutoHideButton* button) {
    if (button) button->CMFCAutoHideButton::~CMFCAutoHideButton();
}
// Symbol: ?CreateObject@CMFCAutoHideButton@@SAPEAVCObject@@XZ
extern "C" CObject* AFXAPI impl__CreateObject_CMFCAutoHideButton__SAPEAVCObject__XZ() {
    return CMFCAutoHideButton::CreateObject();
}
// Symbol: ?Create@CMFCAutoHideButton@@UEAAHPEAVCMFCAutoHideBar@@PEAVCDockablePane@@K@Z
extern "C" BOOL AFXAPI impl__Create_CMFCAutoHideButton__UEAAHPEAVCMFCAutoHideBar__PEAVCDockablePane__K_Z(
    CMFCAutoHideButton* button, CMFCAutoHideBar* bar, CDockablePane* pane, DWORD alignment) {
    return button && button->CMFCAutoHideButton::Create(bar, pane, alignment);
}
// Symbol: ?GetSize@CMFCAutoHideButton@@QEBA?AVCSize@@XZ
extern "C" CSize* AFXAPI impl__GetSize_CMFCAutoHideButton__QEBA_AVCSize__XZ(
    const CMFCAutoHideButton* button, CSize* result) {
    return result ? ::new (result) CSize(button ? button->GetSize() : CSize(0, 0)) : nullptr;
}
// Symbol: ?GetTextSize@CMFCAutoHideButton@@UEBA?AVCSize@@XZ
extern "C" CSize* AFXAPI impl__GetTextSize_CMFCAutoHideButton__UEBA_AVCSize__XZ(
    const CMFCAutoHideButton* button, CSize* result) {
    return result ? ::new (result) CSize(button ? button->CMFCAutoHideButton::GetTextSize() : CSize(0, 0)) : nullptr;
}
// Symbol: ?HighlightButton@CMFCAutoHideButton@@UEAAXH@Z
extern "C" void AFXAPI impl__HighlightButton_CMFCAutoHideButton__UEAAXH_Z(CMFCAutoHideButton* button, BOOL highlight) {
    if (button) button->CMFCAutoHideButton::HighlightButton(highlight);
}
// Symbol: ?IsHorizontal@CMFCAutoHideButton@@QEBAHXZ
extern "C" BOOL AFXAPI impl__IsHorizontal_CMFCAutoHideButton__QEBAHXZ(const CMFCAutoHideButton* button) {
    return button && button->IsHorizontal();
}
// Symbol: ?Move@CMFCAutoHideButton@@QEAAXH@Z
extern "C" void AFXAPI impl__Move_CMFCAutoHideButton__QEAAXH_Z(CMFCAutoHideButton* button, int offset) {
    if (button) button->Move(offset);
}
// Symbol: ?OnDraw@CMFCAutoHideButton@@UEAAXPEAVCDC@@@Z
extern "C" void AFXAPI impl__OnDraw_CMFCAutoHideButton__UEAAXPEAVCDC___Z(CMFCAutoHideButton* button, CDC* dc) {
    if (button) button->CMFCAutoHideButton::OnDraw(dc);
}
// Symbol: ?OnDrawBorder@CMFCAutoHideButton@@UEAAXPEAVCDC@@VCRect@@1@Z
extern "C" void AFXAPI impl__OnDrawBorder_CMFCAutoHideButton__UEAAXPEAVCDC__VCRect__1_Z(
    CMFCAutoHideButton* button, CDC* dc, CRect bounds, CRect border) {
    if (button) button->CMFCAutoHideButton::OnDrawBorder(dc, bounds, border);
}
// Symbol: ?OnFillBackground@CMFCAutoHideButton@@UEAAXPEAVCDC@@VCRect@@@Z
extern "C" void AFXAPI impl__OnFillBackground_CMFCAutoHideButton__UEAAXPEAVCDC__VCRect___Z(
    CMFCAutoHideButton* button, CDC* dc, CRect rect) {
    if (button) button->CMFCAutoHideButton::OnFillBackground(dc, rect);
}
// Symbol: ?ReplacePane@CMFCAutoHideButton@@QEAAXPEAVCDockablePane@@@Z
extern "C" void AFXAPI impl__ReplacePane_CMFCAutoHideButton__QEAAXPEAVCDockablePane___Z(
    CMFCAutoHideButton* button, CDockablePane* pane) {
    if (button) button->ReplacePane(pane);
}
// Symbol: ?ShowAttachedWindow@CMFCAutoHideButton@@QEAAXH@Z
extern "C" void AFXAPI impl__ShowAttachedWindow_CMFCAutoHideButton__QEAAXH_Z(CMFCAutoHideButton* button, BOOL show) {
    if (button) button->ShowAttachedWindow(show);
}
// Symbol: ?ShowButton@CMFCAutoHideButton@@UEAAXH@Z
extern "C" void AFXAPI impl__ShowButton_CMFCAutoHideButton__UEAAXH_Z(CMFCAutoHideButton* button, BOOL show) {
    if (button) button->CMFCAutoHideButton::ShowButton(show);
}
// Symbol: ?UnSetAutoHideMode@CMFCAutoHideButton@@UEAAXPEAVCDockablePane@@@Z
extern "C" void AFXAPI impl__UnSetAutoHideMode_CMFCAutoHideButton__UEAAXPEAVCDockablePane___Z(
    CMFCAutoHideButton* button, CDockablePane* firstInGroup) {
    if (button) button->CMFCAutoHideButton::UnSetAutoHideMode(firstInGroup);
}
