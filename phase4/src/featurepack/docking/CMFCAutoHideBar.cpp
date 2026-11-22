// CMFCAutoHideBar — OpenMFC implementation.
#define OPENMFC_APPCORE_IMPL
#include "detail/CMFCAutoHideButtonSupport.h"
#include "detail/Mfc02MsgmapSupport.h"
#include "detail/DockingState.h"
#include "detail/FeaturePackDockSite.h"

#include <algorithm>
#include <memory>
#include <new>
#include <vector>

static_assert(sizeof(CMFCAutoHideBar) == 0x458);
static_assert(offsetof(CMFCAutoHideBar, m_lstAutoHideButtons) == 0x3f8);
static_assert(offsetof(CMFCAutoHideBar, m_nDisplayAHWndTimerID) == 0x448);

extern "C" BOOL AFXAPI impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CWnd*, const wchar_t*, DWORD, const RECT&, CWnd*, UINT, CCreateContext*);
extern "C" void AFXAPI impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(CWnd*, UINT, CPoint);
extern "C" BOOL AFXAPI impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(CWnd*);
extern "C" void AFXAPI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd*);
extern "C" void AFXAPI impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(void*, CPane*, int);
extern "C" void AFXAPI impl__RepositionPanes_CAutoHideDockSite__UEAAXAEAVCRect___Z(void*, CRect*);
extern "C" void AFXAPI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int impl__m_nShowAHWndDelay_CMFCAutoHideBar__2HA;
extern "C" void* impl__m_pAutoHideButtonRTS_CMFCAutoHideBar__2PEAUCRuntimeClass__EA;

namespace {
constexpr UINT_PTR hoverTimer = 0xec00;

void CancelHover(CMFCAutoHideBar& bar) {
    if (bar.m_nDisplayAHWndTimerID)
        ::KillTimer(bar.m_hWnd, bar.m_nDisplayAHWndTimerID);
    bar.m_nDisplayAHWndTimerID = 0;
    bar.m_bReadyToDisplayAHWnd = FALSE;
}
}

const AFX_MSGMAP* AFXAPI CMFCAutoHideBar::GetThisMessageMap() {
    return &classCMFCAutoHideBar_msgmap;
}

const AFX_MSGMAP* CMFCAutoHideBar::GetMessageMap() const {
    return GetThisMessageMap();
}

CMFCAutoHideBar::~CMFCAutoHideBar() {
    if (m_hWnd) DestroyWindow();
    CleanUpAutoHideButtons();
}

CMFCAutoHideButton* CMFCAutoHideBar::ButtonFromAutoHideWindow(CDockablePane* pane) const {
    if (!pane) return nullptr;
    for (auto pos = m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
        auto* button = static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.GetNext(pos));
        if (button->m_pAutoHideWindow == pane) return button;
    }
    return nullptr;
}

CMFCAutoHideButton* CMFCAutoHideBar::ButtonFromPoint(CPoint point) const {
    for (auto pos = m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
        auto* button = static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.GetNext(pos));
        if (button->IsVisible() && ::PtInRect(button->m_rect, point)) return button;
    }
    return nullptr;
}

CMFCAutoHideButton* CMFCAutoHideBar::AddAutoHideWindow(CDockablePane* pane, DWORD alignment) {
    if (!pane) return nullptr;
    if (auto* existing = ButtonFromAutoHideWindow(pane)) return existing;
    {
        std::lock_guard<std::mutex> lock(openmfc::docking_state::PaneCoreStateMutex());
        const auto& states = openmfc::docking_state::PaneCoreStates();
        const auto found = states.find(pane);
        if (found != states.end() && found->second.autoHideButton) return nullptr;
    }

    auto* type = static_cast<CRuntimeClass*>(impl__m_pAutoHideButtonRTS_CMFCAutoHideBar__2PEAUCRuntimeClass__EA);
    if (!type) type = CMFCAutoHideButton::GetThisClass();
    if (!type->IsDerivedFrom(RUNTIME_CLASS(CMFCAutoHideButton))) return nullptr;
    std::unique_ptr<CMFCAutoHideButton> button(static_cast<CMFCAutoHideButton*>(type->CreateObject()));
    if (!button || !button->Create(this, pane, alignment)) return nullptr;
    m_lstAutoHideButtons.AddTail(button.get());
    auto* added = button.release();
    LayoutButtons();
    if (m_hWnd) InvalidateRect(nullptr, FALSE);
    return added;
}

BOOL CMFCAutoHideBar::RemoveAutoHideWindow(CDockablePane* pane) {
    auto* button = ButtonFromAutoHideWindow(pane);
    if (!button) return FALSE;
    CancelHover(*this);
    if (m_pHighlightedButton == button) m_pHighlightedButton = nullptr;
    if (m_pLastActiveButton == button) m_pLastActiveButton = nullptr;
    m_lstAutoHideButtons.RemoveAt(m_lstAutoHideButtons.Find(button));
    delete button;
    if (!m_bDestroying) {
        if (m_bManagerOwned && m_lstAutoHideButtons.IsEmpty() && m_hWnd) DestroyWindow();
        else UpdateVisibleState();
    }
    return TRUE;
}

BOOL CMFCAutoHideBar::ShowAutoHideWindow(CDockablePane* pane, BOOL show, BOOL) {
    auto* button = ButtonFromAutoHideWindow(pane);
    if (!button) return FALSE;
    if (!show && m_pHighlightedButton == button) {
        CancelHover(*this);
        button->HighlightButton(FALSE);
        m_pHighlightedButton = nullptr;
    }
    button->ShowButton(show);
    UpdateVisibleState();
    return TRUE;
}

CDockablePane* CMFCAutoHideBar::GetFirstAHWindow() const {
    return m_lstAutoHideButtons.IsEmpty() ? nullptr :
        static_cast<const CMFCAutoHideButton*>(m_lstAutoHideButtons.GetHead())->m_pAutoHideWindow;
}

int CMFCAutoHideBar::GetVisibleCount() const {
    int count = 0;
    for (auto pos = m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
        auto* button = static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.GetNext(pos));
        if (button->IsVisible()) ++count;
    }
    return count;
}

CSize CMFCAutoHideBar::LayoutButtons() {
    CSize bounds(0, 0);
    for (auto pos = m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
        auto* button = static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.GetNext(pos));
        if (!button->IsVisible()) {
            button->m_rect.SetRectEmpty();
            continue;
        }
        const CSize size = button->GetSize();
        if (button->IsHorizontal()) {
            button->m_rect.SetRect(bounds.cx, 0, bounds.cx + size.cx, size.cy);
            bounds.cx += size.cx;
            bounds.cy = (std::max)(bounds.cy, size.cy);
        } else {
            button->m_rect.SetRect(0, bounds.cy, size.cx, bounds.cy + size.cy);
            bounds.cx = (std::max)(bounds.cx, size.cx);
            bounds.cy += size.cy;
        }
    }
    return bounds;
}

void CMFCAutoHideBar::CleanUpAutoHideButtons() {
    CancelHover(*this);
    m_pLastActiveButton = nullptr;
    m_pHighlightedButton = nullptr;
    while (!m_lstAutoHideButtons.IsEmpty())
        delete static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.RemoveHead());
}

void CMFCAutoHideBar::SetActiveInGroup(BOOL active) {
    if (m_bActiveInGroup == active) return;
    m_bActiveInGroup = active;
    LayoutButtons();
    if (m_hWnd) InvalidateRect(nullptr, FALSE);
}

void CMFCAutoHideBar::UpdateVisibleState() {
    const CSize size = LayoutButtons();
    if (m_hWnd) {
        ::ShowWindow(m_hWnd, GetVisibleCount() ? SW_SHOWNOACTIVATE : SW_HIDE);
        if (m_bManagerOwned && m_pParentDockBar && size.cx > 0 && size.cy > 0) {
            ::SetWindowPos(m_hWnd, nullptr, 0, 0, size.cx, size.cy,
                SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            impl__RepositionPanes_CAutoHideDockSite__UEAAXAEAVCRect___Z(m_pParentDockBar, nullptr);
        }
        InvalidateRect(nullptr, FALSE);
    }
}

void CMFCAutoHideBar::UnSetAutoHideMode(CDockablePane* firstInGroup) {
    std::vector<CDockablePane*> panes;
    for (auto pos = m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;)
        panes.push_back(static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.GetNext(pos))->m_pAutoHideWindow);
    for (auto* pane : panes) {
        if (auto* button = ButtonFromAutoHideWindow(pane)) button->UnSetAutoHideMode(firstInGroup);
    }
}

void CMFCAutoHideBar::OnPaint() {
    CPaintDC dc(this);
    DoPaint(&dc);
}

int CMFCAutoHideBar::OnCreate(CREATESTRUCTW*) {
    m_bDestroying = false;
    return Default() == -1 ? -1 : 0;
}

void CMFCAutoHideBar::OnDestroy() {
    m_bDestroying = true;
    if (m_bManagerOwned) {
        UnSetAutoHideMode(nullptr);
        if (m_pParentDockBar) {
            auto* site = reinterpret_cast<CAutoHideDockSite*>(m_pParentDockBar);
            impl__RemovePane_CDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD___Z(site, this, 0);
            m_pParentDockBar = nullptr;
            if (site->m_lstControlBars.IsEmpty()) ::ShowWindow(site->m_hWnd, SW_HIDE);
            else impl__RepositionPanes_CAutoHideDockSite__UEAAXAEAVCRect___Z(site, nullptr);
        }
    }
    CleanUpAutoHideButtons();
    CWnd::OnDestroy();
}

void CMFCAutoHideBar::DoPaint(CDC* dc) {
    if (!dc || !dc->GetSafeHdc()) return;
    RECT client{};
    ::GetClientRect(m_hWnd, &client);
    ::FillRect(dc->GetSafeHdc(), &client, ::GetSysColorBrush(COLOR_BTNFACE));
    for (auto pos = m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
        auto* button = static_cast<CMFCAutoHideButton*>(m_lstAutoHideButtons.GetNext(pos));
        if (button->IsVisible()) button->OnDraw(dc);
    }
}

void CMFCAutoHideBar::OnLButtonDown(UINT flags, CPoint point) {
    if (auto* button = ButtonFromPoint(point)) {
        CDockablePane* pane = button->m_pAutoHideWindow;
        CancelHover(*this);
        m_pLastActiveButton = button;
        button->ShowAttachedWindow(TRUE);
        if (pane && ::IsWindowVisible(pane->m_hWnd)) ::SetFocus(pane->m_hWnd);
    } else {
        impl__OnLButtonDown_CPane__IEAAXIVCPoint___Z(this, flags, point);
    }
}

void CMFCAutoHideBar::OnMouseMove(UINT, CPoint point) {
    if (!m_bTracked) {
        TRACKMOUSEEVENT event{sizeof(event), TME_LEAVE, m_hWnd, 0};
        m_bTracked = ::TrackMouseEvent(&event);
    }
    auto* button = ButtonFromPoint(point);
    if (button == m_pHighlightedButton) return;
    CancelHover(*this);
    if (m_pHighlightedButton) m_pHighlightedButton->HighlightButton(FALSE);
    m_pHighlightedButton = button;
    if (button) {
        button->HighlightButton(TRUE);
        m_nDisplayAHWndTimerID = ::SetTimer(m_hWnd, hoverTimer,
            (std::max)(0, impl__m_nShowAHWndDelay_CMFCAutoHideBar__2HA), nullptr);
        m_bReadyToDisplayAHWnd = m_nDisplayAHWndTimerID != 0;
    }
    InvalidateRect(nullptr, FALSE);
}

void CMFCAutoHideBar::OnMouseLeave() {
    CancelHover(*this);
    if (m_pHighlightedButton) m_pHighlightedButton->HighlightButton(FALSE);
    m_pHighlightedButton = nullptr;
    m_bTracked = FALSE;
    if (m_hWnd) InvalidateRect(nullptr, FALSE);
}

void CMFCAutoHideBar::OnTimer(UINT_PTR timer) {
    if (timer != hoverTimer || timer != m_nDisplayAHWndTimerID) {
        Default();
        return;
    }
    CancelHover(*this);
    POINT cursor{};
    if (!::GetCursorPos(&cursor) || !::ScreenToClient(m_hWnd, &cursor)) return;
    if (auto* button = ButtonFromPoint(CPoint(cursor)); button && button == m_pHighlightedButton) {
        m_pLastActiveButton = button;
        button->ShowAttachedWindow(TRUE);
    }
}

// Symbol: ??0CMFCAutoHideBar@@QEAA@XZ
extern "C" void* AFXAPI impl___0CMFCAutoHideBar__QEAA_XZ(void* storage) {
    return storage ? ::new (storage) CMFCAutoHideBar : nullptr;
}
// Symbol: ??1CMFCAutoHideBar@@UEAA@XZ
extern "C" void AFXAPI impl___1CMFCAutoHideBar__UEAA_XZ(CMFCAutoHideBar* bar) {
    if (bar) bar->CMFCAutoHideBar::~CMFCAutoHideBar();
}
// Symbol: ?CreateObject@CMFCAutoHideBar@@SAPEAVCObject@@XZ
extern "C" CObject* AFXAPI impl__CreateObject_CMFCAutoHideBar__SAPEAVCObject__XZ() {
    return CMFCAutoHideBar::CreateObject();
}
// Symbol: ?AddAutoHideWindow@CMFCAutoHideBar@@QEAAPEAVCMFCAutoHideButton@@PEAVCDockablePane@@K@Z
extern "C" CMFCAutoHideButton* AFXAPI impl__AddAutoHideWindow_CMFCAutoHideBar__QEAAPEAVCMFCAutoHideButton__PEAVCDockablePane__K_Z(
    CMFCAutoHideBar* bar, CDockablePane* pane, DWORD alignment) {
    return bar ? bar->AddAutoHideWindow(pane, alignment) : nullptr;
}
// Symbol: ?ButtonFromAutoHideWindow@CMFCAutoHideBar@@IEAAPEAVCMFCAutoHideButton@@PEAVCDockablePane@@@Z
extern "C" CMFCAutoHideButton* AFXAPI impl__ButtonFromAutoHideWindow_CMFCAutoHideBar__IEAAPEAVCMFCAutoHideButton__PEAVCDockablePane___Z(
    CMFCAutoHideBar* bar, CDockablePane* pane) {
    return bar ? bar->ButtonFromAutoHideWindow(pane) : nullptr;
}
// Symbol: ?ButtonFromPoint@CMFCAutoHideBar@@IEAAPEAVCMFCAutoHideButton@@VCPoint@@@Z
extern "C" CMFCAutoHideButton* AFXAPI impl__ButtonFromPoint_CMFCAutoHideBar__IEAAPEAVCMFCAutoHideButton__VCPoint___Z(
    CMFCAutoHideBar* bar, CPoint point) {
    return bar ? bar->ButtonFromPoint(point) : nullptr;
}
// Symbol: ?GetFirstAHWindow@CMFCAutoHideBar@@QEAAPEAVCDockablePane@@XZ
extern "C" CDockablePane* AFXAPI impl__GetFirstAHWindow_CMFCAutoHideBar__QEAAPEAVCDockablePane__XZ(CMFCAutoHideBar* bar) {
    return bar ? bar->GetFirstAHWindow() : nullptr;
}
// Symbol: ?GetVisibleCount@CMFCAutoHideBar@@QEAAHXZ
extern "C" int AFXAPI impl__GetVisibleCount_CMFCAutoHideBar__QEAAHXZ(CMFCAutoHideBar* bar) {
    return bar ? bar->GetVisibleCount() : 0;
}
// Symbol: ?CalcFixedLayout@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* AFXAPI impl__CalcFixedLayout_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(
    CMFCAutoHideBar* bar, CSize* result, BOOL, BOOL) {
    if (!result) return nullptr;
    RECT rect{};
    if (bar) ::GetWindowRect(bar->m_hWnd, &rect);
    return ::new (result) CSize(rect.right - rect.left, rect.bottom - rect.top);
}
// Symbol: ?CleanUpAutoHideButtons@CMFCAutoHideBar@@IEAAXXZ
extern "C" void AFXAPI impl__CleanUpAutoHideButtons_CMFCAutoHideBar__IEAAXXZ(CMFCAutoHideBar* bar) {
    if (bar) bar->CleanUpAutoHideButtons();
}
// Symbol: ?Create@CMFCAutoHideBar@@UEAAHPEB_WKAEBUtagRECT@@PEAVCWnd@@IKPEAUCCreateContext@@@Z
extern "C" BOOL AFXAPI impl__Create_CMFCAutoHideBar__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CMFCAutoHideBar* bar, const wchar_t* windowClass, DWORD style, const RECT& rect,
    CWnd* parent, UINT id, DWORD controlBarStyle, CCreateContext* context) {
    if (!bar) return FALSE;
    if (!impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(parent)) impl__AfxThrowInvalidArgException__YAXXZ();
    bar->m_dwControlBarStyle = controlBarStyle;
    return impl__Create_CPane__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
        bar, windowClass, style, rect, parent, id, context);
}
// Symbol: ?DoPaint@CMFCAutoHideBar@@MEAAXPEAVCDC@@@Z
extern "C" void AFXAPI impl__DoPaint_CMFCAutoHideBar__MEAAXPEAVCDC___Z(CMFCAutoHideBar* bar, CDC* dc) {
    if (bar) bar->DoPaint(dc);
}
// Symbol: ?OnCreate@CMFCAutoHideBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int AFXAPI impl__OnCreate_CMFCAutoHideBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCAutoHideBar* bar, CREATESTRUCTW* create) {
    return bar ? bar->OnCreate(create) : -1;
}
// Symbol: ?OnLButtonDown@CMFCAutoHideBar@@IEAAXIVCPoint@@@Z
extern "C" void AFXAPI impl__OnLButtonDown_CMFCAutoHideBar__IEAAXIVCPoint___Z(CMFCAutoHideBar* bar, UINT flags, CPoint point) {
    if (bar) bar->OnLButtonDown(flags, point);
}
// Symbol: ?OnMouseLeave@CMFCAutoHideBar@@IEAAXXZ
extern "C" void AFXAPI impl__OnMouseLeave_CMFCAutoHideBar__IEAAXXZ(CMFCAutoHideBar* bar) {
    if (bar) bar->OnMouseLeave();
}
// Symbol: ?OnMouseMove@CMFCAutoHideBar@@IEAAXIVCPoint@@@Z
extern "C" void AFXAPI impl__OnMouseMove_CMFCAutoHideBar__IEAAXIVCPoint___Z(CMFCAutoHideBar* bar, UINT flags, CPoint point) {
    if (bar) bar->OnMouseMove(flags, point);
}
// Symbol: ?OnNcDestroy@CMFCAutoHideBar@@IEAAXXZ
extern "C" void AFXAPI impl__OnNcDestroy_CMFCAutoHideBar__IEAAXXZ(CMFCAutoHideBar* bar) {
    if (bar) {
        CancelHover(*bar);
        impl__OnNcDestroy_CWnd__IEAAXXZ(bar);
    }
}
// Symbol: ?OnTimer@CMFCAutoHideBar@@IEAAX_K@Z
extern "C" void AFXAPI impl__OnTimer_CMFCAutoHideBar__IEAAX_K_Z(CMFCAutoHideBar* bar, UINT_PTR timer) {
    if (bar) bar->OnTimer(timer);
}
// Symbol: ?RemoveAutoHideWindow@CMFCAutoHideBar@@QEAAHPEAVCDockablePane@@@Z
extern "C" BOOL AFXAPI impl__RemoveAutoHideWindow_CMFCAutoHideBar__QEAAHPEAVCDockablePane___Z(CMFCAutoHideBar* bar, CDockablePane* pane) {
    return bar && bar->RemoveAutoHideWindow(pane);
}
// Symbol: ?SetActiveInGroup@CMFCAutoHideBar@@UEAAXH@Z
extern "C" void AFXAPI impl__SetActiveInGroup_CMFCAutoHideBar__UEAAXH_Z(CMFCAutoHideBar* bar, BOOL active) {
    if (bar) bar->SetActiveInGroup(active);
}
// Symbol: ?ShowAutoHideWindow@CMFCAutoHideBar@@QEAAHPEAVCDockablePane@@HH@Z
extern "C" BOOL AFXAPI impl__ShowAutoHideWindow_CMFCAutoHideBar__QEAAHPEAVCDockablePane__HH_Z(
    CMFCAutoHideBar* bar, CDockablePane* pane, BOOL show, BOOL delay) {
    return bar && bar->ShowAutoHideWindow(pane, show, delay);
}
// Symbol: ?StretchPane@CMFCAutoHideBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* AFXAPI impl__StretchPane_CMFCAutoHideBar__UEAA_AVCSize__HH_Z(CMFCAutoHideBar* bar, CSize* result, int, BOOL) {
    return result ? ::new (result) CSize(bar ? bar->LayoutButtons() : CSize(0, 0)) : nullptr;
}
// Symbol: ?UnSetAutoHideMode@CMFCAutoHideBar@@QEAAXPEAVCDockablePane@@@Z
extern "C" void AFXAPI impl__UnSetAutoHideMode_CMFCAutoHideBar__QEAAXPEAVCDockablePane___Z(
    CMFCAutoHideBar* bar, CDockablePane* firstInGroup) {
    if (bar) bar->UnSetAutoHideMode(firstInGroup);
}
// Symbol: ?UpdateVisibleState@CMFCAutoHideBar@@QEAAXXZ
extern "C" void AFXAPI impl__UpdateVisibleState_CMFCAutoHideBar__QEAAXXZ(CMFCAutoHideBar* bar) {
    if (bar) bar->UpdateVisibleState();
}
