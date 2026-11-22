// CDockingManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"
#include "detail/CSmartDockingInfoSupport.h"
#include "detail/FeaturePackDockSite.h"
#include "detail/CMFCAutoHideButtonSupport.h"
#include "detail/ClassicDockingSupport.h"
#include "openmfc/afxole.h"

#include <cstdlib>
#include <cstring>
#include <memory>

static_assert(sizeof(CDockingManager) == 0x90,
              "preserve the existing compact CDockingManager allocation");

extern "C" void* MS_ABI impl___0CSmartDockingInfo__QEAA_XZ(void* pThis);
extern "C" std::int32_t impl__m_bSDParamsModified_CDockingManager__1HA;
extern "C" std::uint32_t impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A;
extern "C" int impl__m_nDockSensitivity_CDockingManager__2HA;
extern "C" unsigned long MS_ABI impl__GetCurrentAlignment_CBasePane__UEBAKXZ(const CBasePane*);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd*);
extern "C" void* impl__m_pAutoHideToolbarRTC_CDockingManager__2PEAUCRuntimeClass__EA;
extern "C" BOOL MS_ABI impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(CWnd*);
extern "C" BOOL MS_ABI impl__Create_CMFCAutoHideBar__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
    CMFCAutoHideBar*, const wchar_t*, DWORD, const RECT&, CWnd*, UINT, DWORD, CCreateContext*);
extern "C" void MS_ABI impl__DockPane_CAutoHideDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT___Z(
    void*, CPane*, int, const RECT*);
extern "C" void MS_ABI impl__RepositionPanes_CAutoHideDockSite__UEAAXAEAVCRect___Z(void*, CRect*);
extern "C" void MS_ABI impl__Slide_CDockablePane__UEAAXHH_Z(CDockablePane*, BOOL, BOOL);
extern "C" void* MS_ABI impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(const CDockablePane*);
extern "C" void MS_ABI impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(
    CDockingManager*, CPaneDivider*, BOOL);

extern "C" void* MS_ABI impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(void*, unsigned int);
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd*);
extern "C" BOOL MS_ABI impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
    const void*, long long, CBasePane*, int, const CDockingManager*, BOOL, DWORD*, DWORD, const RECT*);

namespace {
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(
    const CDockingManager* manager, CPoint point, const CPaneFrameWnd* ignore, int multiOnly);
extern "C" int MS_ABI impl__GetDockingStatus_CDockablePane__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(
    CDockablePane* pane, void* point, int sensitivity);

struct DockSiteInfoShadow {
    DWORD alignment;
    CRuntimeClass* dockSiteRuntimeClass;
};
static_assert(offsetof(DockSiteInfoShadow, alignment) == 0x00, "AFX_DOCKSITE_INFO alignment");
static_assert(offsetof(DockSiteInfoShadow, dockSiteRuntimeClass) == 0x08, "AFX_DOCKSITE_INFO runtime class");
static_assert(sizeof(DockSiteInfoShadow) == 0x10, "AFX_DOCKSITE_INFO size");

template <typename T>
bool InsertManagerStateEntry(T**& entries, std::size_t& count, std::size_t& capacity,
                             std::size_t index, T* value) {
    if (index > count) index = count;
    if (count == capacity) {
        const std::size_t newCapacity = capacity == 0 ? 4 : capacity * 2;
        T** grown = static_cast<T**>(std::realloc(entries, newCapacity * sizeof(T*)));
        if (!grown) return false;
        entries = grown;
        capacity = newCapacity;
    }
    if (index < count) {
        std::memmove(entries + index + 1, entries + index, (count - index) * sizeof(T*));
    }
    entries[index] = value;
    ++count;
    return true;
}

CDockSite* CreateDockSiteForManager(CDockingManager* manager, const DockSiteInfoShadow& info,
                                     std::size_t position) {
    auto& state = EnsureDockingState(manager);
    if (!state.parentFrame || !info.dockSiteRuntimeClass ||
        !info.dockSiteRuntimeClass->IsDerivedFrom(RUNTIME_CLASS(CDockSite))) return nullptr;

    std::unique_ptr<CDockSite> site(static_cast<CDockSite*>(info.dockSiteRuntimeClass->CreateObject()));
    if (!site || !site->Create(info.alignment, RECT{}, state.parentFrame, 0, nullptr))
        return nullptr;

    const bool autoHide = site->IsKindOf(RUNTIME_CLASS(CAutoHideDockSite));
    site->SetDockingManager(manager);
    state.dockSites.insert(state.dockSites.begin() + std::min(position, state.dockSites.size()),
                           {site.get(), info.alignment & CBRS_ALIGN_ANY, autoHide});
    return site.release();
}

constexpr DWORD dockingEdges[]{CBRS_ALIGN_TOP, CBRS_ALIGN_BOTTOM, CBRS_ALIGN_LEFT, CBRS_ALIGN_RIGHT};

void LayoutDockSites(CDockingManager& manager, const DockingManagerState& state, const RECT& available) {
    if (state.dockSites.empty()) return;
    HDWP layout = ::BeginDeferWindowPos(static_cast<int>(state.dockSites.size()));
    for (const auto& entry : state.dockSites) {
        if (!layout || !entry.site->m_hWnd) continue;
        const bool occupied = !entry.site->m_lstDockBarRows.IsEmpty();
        if (occupied && !entry.autoHide) continue;
        RECT bounds = EmptyDockSiteBounds(available, entry.alignment);
        if (occupied) {
            RECT current{};
            ::GetWindowRect(entry.site->m_hWnd, &current);
            switch (entry.alignment) {
            case CBRS_ALIGN_LEFT: bounds.right += current.right - current.left; break;
            case CBRS_ALIGN_RIGHT: bounds.left -= current.right - current.left; break;
            case CBRS_ALIGN_TOP: bounds.bottom += current.bottom - current.top; break;
            case CBRS_ALIGN_BOTTOM: bounds.top -= current.bottom - current.top; break;
            }
        }
        layout = ::DeferWindowPos(layout, entry.site->m_hWnd, nullptr, bounds.left, bounds.top,
            bounds.right - bounds.left, bounds.bottom - bounds.top, SWP_NOZORDER | SWP_NOACTIVATE);
    }
    if (layout) ::EndDeferWindowPos(layout);
    for (const auto& entry : state.dockSites) {
        if (entry.autoHide && !entry.site->m_lstDockBarRows.IsEmpty())
            impl__RepositionPanes_CAutoHideDockSite__UEAAXAEAVCRect___Z(entry.site, nullptr);
    }
    for (const auto& bar : state.autoHideBars) {
        for (auto pos = bar->m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
            const auto* button = static_cast<const CMFCAutoHideButton*>(bar->m_lstAutoHideButtons.GetNext(pos));
            const auto* pane = button->m_pAutoHideWindow;
            if (!pane || !::IsWindowVisible(pane->m_hWnd)) continue;
            auto* divider = static_cast<CPaneDivider*>(
                impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pane));
            impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(&manager, divider, TRUE);
        }
    }
}
} // namespace

// Symbol: ?AddMiniFrame@CDockingManager@@UEAAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__AddMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(CDockingManager* pThis, void* pFrame) {
    if (!pThis || !pFrame) return FALSE;
    auto& frames = EnsureDockingState(pThis).miniFrames;
    if (std::find(frames.begin(), frames.end(), pFrame) == frames.end()) {
        frames.push_back(pFrame);
    }
    return TRUE;
}
// Symbol: ?AddPane@CDockingManager@@QEAAHPEAVCBasePane@@HHH@Z
extern "C" int MS_ABI impl__AddPane_CDockingManager__QEAAHPEAVCBasePane__HHH_Z(
    CDockingManager* pThis, CBasePane* pPane, int, int, int) {
    if (!pThis || !pPane) return FALSE;
    AddDockingPane(pThis, pPane);
    return TRUE;
}
// Symbol: ?AdjustDockingLayout@CDockingManager@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__AdjustDockingLayout_CDockingManager__UEAAXPEAX_Z(CDockingManager* pThis, void*) {
    if (pThis) pThis->RecalcLayout();
}
// Symbol: ?AdjustPaneFrames@CDockingManager@@UEAAXXZ
extern "C" void MS_ABI impl__AdjustPaneFrames_CDockingManager__UEAAXXZ(CDockingManager* pThis) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->miniFrames.empty()) return;

    int index = 0;
    for (void* pFrame : state->miniFrames) {
        if (!pFrame) continue;
        CWnd* frame = static_cast<CWnd*>(pFrame);
        HWND hWnd = frame ? frame->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;

        RECT currentRect{};
        ::GetWindowRect(hWnd, &currentRect);
        CRect rect(currentRect);
        if (!IsDrawableRect(rect)) {
            rect = DefaultMiniFrameRect(index++);
        }

        frame->SetWindowPos(nullptr,
                            rect.left, rect.top,
                            std::max(rect.Width(), 80),
                            std::max(rect.Height(), 60),
                            SWP_NOZORDER | SWP_NOACTIVATE);
    }
}
// Symbol: ?AdjustRectToClientArea@CDockingManager@@UEAAHAEAVCRect@@K@Z
extern "C" int MS_ABI impl__AdjustRectToClientArea_CDockingManager__UEAAHAEAVCRect__K_Z(CDockingManager* pThis, CRect* pRect, unsigned long) {
    if (!pThis || !pRect) return FALSE;

    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->panes.empty()) return FALSE;

    CWnd* parentWnd = nullptr;
    for (CBasePane* pane : state->panes) {
        if (!pane || !pane->GetSafeHwnd()) continue;
        if (CWnd* parent = pane->GetParent()) {
            if (parent->GetSafeHwnd()) {
                parentWnd = parent;
                break;
            }
        }
    }

    if (!parentWnd) return FALSE;

    CRect original = *pRect;
    POINT topLeft = original.TopLeft();
    POINT bottomRight = original.BottomRight();
    ::ScreenToClient(parentWnd->GetSafeHwnd(), &topLeft);
    ::ScreenToClient(parentWnd->GetSafeHwnd(), &bottomRight);
    original = CRect(topLeft, bottomRight);
    RECT client{};
    parentWnd->GetClientRect(&client);
    CRect clientRect(client);
    original.IntersectRect(original, clientRect);
    if (original.IsRectEmpty()) return FALSE;
    *pRect = original;
    return TRUE;
}
CAutoHideDockSite* openmfc::detail::mfccore::FindAutoHideDockSite(const CDockingManager& manager, DWORD alignment) {
    const auto* state = FindDockingState(&manager);
    if (!state) return nullptr;
    for (auto it = state->dockSites.rbegin(); it != state->dockSites.rend(); ++it) {
        if (it->autoHide && it->alignment == alignment && it->site->m_hWnd)
            return static_cast<CAutoHideDockSite*>(it->site);
    }
    return nullptr;
}

void openmfc::detail::mfccore::PruneAutoHideBars(CDockingManager& manager) {
    auto& bars = EnsureDockingState(&manager).autoHideBars;
    bars.erase(std::remove_if(bars.begin(), bars.end(), [](const auto& bar) {
        return !bar->m_hWnd;
    }), bars.end());
}

CMFCAutoHideBar* openmfc::detail::mfccore::AttachAutoHideBar(
    CDockingManager& manager, CAutoHideDockSite& site, CDockablePane& pane, CMFCAutoHideBar* bar, DWORD alignment) {
    auto& state = EnsureDockingState(&manager);
    auto* frame = state.parentFrame;
    if (!frame || !frame->m_hWnd) return nullptr;
    if (bar) {
        if (::GetParent(bar->m_hWnd) != site.m_hWnd || site.m_lstControlBars.Find(bar) == nullptr ||
            !bar->AddAutoHideWindow(&pane, alignment)) return nullptr;
        bar->UpdateVisibleState();
        return bar;
    }

    PruneAutoHideBars(manager);
    if (!impl__AfxIsExtendedFrameClass__YAHPEAVCWnd___Z(frame)) return nullptr;
    auto* type = static_cast<CRuntimeClass*>(impl__m_pAutoHideToolbarRTC_CDockingManager__2PEAUCRuntimeClass__EA);
    if (!type) type = CMFCAutoHideBar::GetThisClass();
    if (!type->IsDerivedFrom(RUNTIME_CLASS(CMFCAutoHideBar))) return nullptr;
    std::unique_ptr<CMFCAutoHideBar> toolbar(static_cast<CMFCAutoHideBar*>(type->CreateObject()));
    if (!toolbar || !impl__Create_CMFCAutoHideBar__UEAAHPEB_WKAEBUtagRECT__PEAVCWnd__IKPEAUCCreateContext___Z(
            toolbar.get(), nullptr, WS_CHILD | WS_CLIPSIBLINGS, RECT{0, 0, 1, 1}, frame, 1, 0, nullptr))
        return nullptr;
    const HWND window = toolbar->m_hWnd;
    if (!::SetParent(window, site.m_hWnd)) return nullptr;
    toolbar->m_dwStyle = alignment;
    toolbar->SetPaneAlignment(alignment);
    toolbar->m_pDockSite = reinterpret_cast<CDockSite*>(frame);
    toolbar->m_bFirstInGroup = toolbar->m_bLastInGroup = TRUE;
    toolbar->SetActiveInGroup(TRUE);
    if (!toolbar->AddAutoHideWindow(&pane, alignment)) return nullptr;
    const CSize size = toolbar->LayoutButtons();
    ::SetWindowPos(window, nullptr, 0, 0, size.cx, size.cy, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
    toolbar->m_bManagerOwned = true;
    impl__DockPane_CAutoHideDockSite__UEAAXPEAVCPane__W4AFX_DOCK_METHOD__PEBUtagRECT___Z(
        &site, toolbar.get(), 4 /* DM_RECT */, nullptr);
    if (!toolbar->m_pDockBarRow) return nullptr;
    bar = toolbar.get();
    state.autoHideBars.push_back(std::move(toolbar));
    return bar;
}

// Symbol: ?AlignAutoHidePane@CDockingManager@@QEAAXPEAVCPaneDivider@@H@Z
extern "C" void MS_ABI impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(
    CDockingManager* manager, CPaneDivider* divider, BOOL positionAtEdge) {
    const auto* state = FindDockingState(manager);
    if (!state || !state->parentFrame || !divider || !divider->m_hWnd) return;
    auto* pane = static_cast<const CDockablePane*>(divider->GetFirstPane());
    if (!pane || !pane->m_hWnd || !pane->IsAutoHideMode()) return;
    const DWORD alignment = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(divider);
    const auto* site = FindAutoHideDockSite(*manager, alignment);
    if (!site) return;

    const HWND parent = state->parentFrame->m_hWnd;
    RECT bounds = state->clientAreaBounds;
    if (::IsRectEmpty(&bounds)) ::GetClientRect(parent, &bounds);
    RECT edge{}, paneRect{}, dividerRect{};
    ::GetWindowRect(site->m_hWnd, &edge);
    ::GetWindowRect(pane->m_hWnd, &paneRect);
    ::MapWindowPoints(nullptr, parent, reinterpret_cast<POINT*>(&edge), 2);
    ::MapWindowPoints(nullptr, parent, reinterpret_cast<POINT*>(&paneRect), 2);
    const LONG width = paneRect.right - paneRect.left;
    const LONG height = paneRect.bottom - paneRect.top;
    const int thickness = divider->m_dividerData.nWidth;

    if (alignment & CBRS_ORIENT_HORZ) {
        paneRect.left = bounds.left;
        paneRect.right = bounds.right;
        if (positionAtEdge && alignment == CBRS_ALIGN_TOP) {
            paneRect.top = edge.bottom;
            paneRect.bottom = paneRect.top + height;
        } else if (positionAtEdge) {
            paneRect.bottom = edge.top;
            paneRect.top = paneRect.bottom - height;
        }
        const LONG y = alignment == CBRS_ALIGN_TOP ? paneRect.bottom : paneRect.top - thickness;
        dividerRect = {paneRect.left, y, paneRect.right, y + thickness};
    } else {
        paneRect.top = bounds.top;
        paneRect.bottom = bounds.bottom;
        if (positionAtEdge && alignment == CBRS_ALIGN_LEFT) {
            paneRect.left = edge.right;
            paneRect.right = paneRect.left + width;
        } else if (positionAtEdge) {
            paneRect.right = edge.left;
            paneRect.left = paneRect.right - width;
        }
        const LONG x = alignment == CBRS_ALIGN_LEFT ? paneRect.right : paneRect.left - thickness;
        dividerRect = {x, paneRect.top, x + thickness, paneRect.bottom};
    }
    constexpr UINT flags = SWP_NOACTIVATE | SWP_NOZORDER;
    ::SetWindowPos(pane->m_hWnd, nullptr, paneRect.left, paneRect.top,
        paneRect.right - paneRect.left, paneRect.bottom - paneRect.top, flags);
    ::SetWindowPos(divider->m_hWnd, nullptr, dividerRect.left, dividerRect.top,
        dividerRect.right - dividerRect.left, dividerRect.bottom - dividerRect.top, flags);
}

// Symbol: ?AutoHidePane@CDockingManager@@QEAAPEAVCMFCAutoHideBar@@PEAVCDockablePane@@PEAV2@@Z
extern "C" CMFCAutoHideBar* MS_ABI impl__AutoHidePane_CDockingManager__QEAAPEAVCMFCAutoHideBar__PEAVCDockablePane__PEAV2__Z(
    CDockingManager* manager, CDockablePane* pane, CMFCAutoHideBar* bar) {
    if (!manager || !pane) return nullptr;
    const DWORD alignment = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pane);
    return pane->BeginAutoHide(*manager, alignment, bar, FALSE);
}
// Symbol: ?BringBarsToTop@CDockingManager@@QEAAXKH@Z
extern "C" void MS_ABI impl__BringBarsToTop_CDockingManager__QEAAXKH_Z(
    CDockingManager* pThis, unsigned long nFlags, int bActivate) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    UINT flags = SWP_NOMOVE | SWP_NOSIZE | (bActivate ? 0 : SWP_NOACTIVATE);
    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        HWND hWnd = pane->GetSafeHwnd();
        if (!hWnd) continue;
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, flags);
        pane->InvalidateRect(nullptr, FALSE);
    }

    for (void* pFrame : state->miniFrames) {
        if (!pFrame) continue;
        CWnd* frame = static_cast<CWnd*>(pFrame);
        HWND hWnd = frame ? frame->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, flags);
        ::InvalidateRect(hWnd, nullptr, FALSE);
    }
    (void)nFlags;
}
// Symbol: ?BuildPanesMenu@CDockingManager@@QEAAXAEAVCMenu@@H@Z
extern "C" void MS_ABI impl__BuildPanesMenu_CDockingManager__QEAAXAEAVCMenu__H_Z(
    CDockingManager* pThis, CMenu* pMenu, int nIDBase) {
    if (!pThis || !pMenu) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    HMENU hMenu = pMenu->GetSafeHmenu();
    if (!hMenu) {
        if (!pMenu->CreatePopupMenu()) return;
        hMenu = pMenu->GetSafeHmenu();
    }
    if (!hMenu) return;

    while (::GetMenuItemCount(hMenu) > 0) {
        ::RemoveMenu(hMenu, 0, MF_BYPOSITION);
    }

    int idx = 0;
    const UINT baseId = static_cast<UINT>(nIDBase == 0 ? 5000 : nIDBase);
    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        UINT cmd = PaneCommandID(pane);
        if (cmd == 0) cmd = baseId + idx;
        ++idx;

        CString label;
        const int len = pane->GetWindowTextLengthW();
        if (len > 0) {
            std::wstring tmp(static_cast<size_t>(len + 1), L'\0');
            pane->GetWindowTextW(&tmp[0], len + 1);
            label = tmp.c_str();
        } else {
            label = L"Pane";
        }
        pMenu->AppendMenu(MF_STRING | MF_ENABLED, cmd, (const wchar_t*)label);
    }

    if (idx == 0) {
        pMenu->AppendMenu(MF_STRING | MF_ENABLED | MF_GRAYED, baseId, L"(No panes)");
    }
}
// Symbol: ?CalcExpectedDockedRect@CDockingManager@@QEAAXPEAVCWnd@@VCPoint@@AEAVCRect@@AEAHPEAPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    CDockingManager* manager, CWnd* dragged, CPoint point, CRect* expected, BOOL* drawTab, CDockablePane** target) {
    if (drawTab) *drawTab = FALSE;
    if (target) *target = nullptr;
    if (!expected) return;
    expected->SetRectEmpty();
    if (!manager || !dragged || ::GetKeyState(VK_CONTROL) < 0) return;
    CWnd* window = dragged;
    if (window->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd))) window = static_cast<CPaneFrameWnd*>(window)->GetPane();
    if (!window || !window->IsKindOf(RUNTIME_CLASS(CBasePane))) return;
    auto* source = static_cast<CBasePane*>(window);
    const auto drop = DockingTarget(manager, point, impl__m_nDockSensitivity_CDockingManager__2HA,
        source->m_dwEnabledAlignment, source, source);
    if (drop.change == NoDockingChange) return;

    RECT bounds{};
    if (drop.pane) {
        if (!::GetWindowRect(drop.pane->m_hWnd, &bounds)) return;
        if (target) *target = drop.pane;
        if (drop.change == DelayDockToTab) {
            *expected = bounds;
            if (drawTab) *drawTab = TRUE;
            return;
        }
    } else {
        const auto* state = FindDockingState(manager);
        if (!state || !state->parentFrame || !::GetClientRect(state->parentFrame->m_hWnd, &bounds)) return;
        ::MapWindowPoints(state->parentFrame->m_hWnd, nullptr, reinterpret_cast<POINT*>(&bounds), 2);
    }

    RECT moving{};
    if (!::GetWindowRect(source->m_hWnd, &moving)) return;
    const LONG width = (std::min)(moving.right - moving.left, bounds.right - bounds.left);
    const LONG height = (std::min)(moving.bottom - moving.top, bounds.bottom - bounds.top);
    switch (drop.alignment) {
    case CBRS_ALIGN_LEFT: bounds.right = bounds.left + width; break;
    case CBRS_ALIGN_RIGHT: bounds.left = bounds.right - width; break;
    case CBRS_ALIGN_TOP: bounds.bottom = bounds.top + height; break;
    case CBRS_ALIGN_BOTTOM: bounds.top = bounds.bottom - height; break;
    default: return;
    }
    *expected = bounds;
}
namespace {
long long PackedDockPoint(CPoint point) {
    long long packed;
    std::memcpy(&packed, &point, sizeof(packed));
    return packed;
}

const CPaneFrameWnd* ParentPaneFrame(const CBasePane* pane) {
    for (HWND window = pane ? ::GetParent(pane->m_hWnd) : nullptr;
         window; window = ::GetParent(window)) {
        auto* owner = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
        if (owner && owner->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
            return static_cast<CPaneFrameWnd*>(owner);
    }
    return nullptr;
}
}

DockingDrop openmfc::detail::mfccore::DockingTarget(const CDockingManager* manager, CPoint point, int sensitivity,
    DWORD allowedAlignment, const CBasePane* ignore, const CBasePane* source) {
    if (!manager || (source && !source->CanBeDocked())) return {};
    const int tolerance = sensitivity >= 0 ? (std::min)(sensitivity, 32767) : 0;
    const auto* ignoredFrame = ParentPaneFrame(ignore ? ignore : source);
    auto* frame = static_cast<CPaneFrameWnd*>(
        impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(manager, point, ignoredFrame, FALSE));
    if (frame) {
        RECT client{};
        ::GetClientRect(frame->m_hWnd, &client);
        ::MapWindowPoints(frame->m_hWnd, nullptr, reinterpret_cast<POINT*>(&client), 2);
        auto* window = frame->GetFirstVisiblePane();
        // A single floating pane can accept a tab through its caption.
        if (window && window != ignore && window != source && point.y < client.top &&
            window->IsKindOf(RUNTIME_CLASS(CDockablePane)) &&
            (!source || static_cast<CBasePane*>(window)->CanAcceptPane(source)))
            return {static_cast<CDockablePane*>(window), 0, DelayDockToTab};
    }

    auto* target = impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        manager, point, tolerance, false, RUNTIME_CLASS(CDockablePane), TRUE, ignore ? ignore : source);
    if (target && target != source && (!source || target->CanAcceptPane(source))) {
        auto* pane = static_cast<CDockablePane*>(target);
        if (!pane->IsAutoHideMode()) {
            const auto* paneFrame = ParentPaneFrame(pane);
            const bool acceptsSplit = !paneFrame || (source &&
                (source->m_dwStyle & CBRS_FLOAT_MULTI) && (pane->m_dwStyle & CBRS_FLOAT_MULTI));
            const DWORD alignment = (pane->GetPaneAlignment() | pane->m_dwStyle) & CBRS_ALIGN_ANY;
            if (acceptsSplit && (paneFrame || (allowedAlignment & alignment))) {
                const auto packed = PackedDockPoint(point);
                void* pointValue;
                std::memcpy(&pointValue, &packed, sizeof(pointValue));
                const int status = impl__GetDockingStatus_CDockablePane__UEAA_AW4AFX_CS_STATUS__VCPoint__H_Z(
                    pane, pointValue, sensitivity);
                if (status == DelayDockToTab) return {pane, 0, DelayDockToTab};
                DWORD side = 0;
                impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
                    nullptr, packed, pane, tolerance, manager, FALSE, &side, allowedAlignment, nullptr);
                if (status == DelayDock || side) return {pane, side, DelayDock};
            }
        }
    }

    DWORD side = 0;
    BOOL outer = FALSE;
    if (impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(manager, point, &side, &outer) &&
        (allowedAlignment & side)) return {nullptr, side, DelayDock, outer != FALSE};
    return {};
}

// Symbol: ?DeterminePaneAndStatus@CDockingManager@@UEAA?AW4AFX_CS_STATUS@@VCPoint@@HKPEAPEAVCBasePane@@PEBV4@2@Z
extern "C" int MS_ABI impl__DeterminePaneAndStatus_CDockingManager__UEAA_AW4AFX_CS_STATUS__VCPoint__HKPEAPEAVCBasePane__PEBV4_2_Z(
    CDockingManager* manager, CPoint point, int sensitivity, DWORD allowedAlignment, CBasePane** target,
    const CBasePane* ignore, const CBasePane* source) {
    if (!target) return NoDockingChange;
    const auto result = DockingTarget(manager, point, sensitivity, allowedAlignment, ignore, source);
    *target = result.pane;
    return result.change;
}
// Symbol: ?EnableAutoHidePanes@CDockingManager@@QEAAHK@Z
extern "C" BOOL MS_ABI impl__EnableAutoHidePanes_CDockingManager__QEAAHK_Z(CDockingManager* manager, DWORD sides) {
    if (!manager) return FALSE;
    auto& state = EnsureDockingState(manager);
    for (DWORD side : dockingEdges) {
        if (!(sides & side) || (state.enabledAutoHideAlignment & side)) continue;
        if (!(state.enabledAlignment & side) && !manager->EnableDocking(side)) return FALSE;
        const DockSiteInfoShadow info{side, RUNTIME_CLASS(CAutoHideDockSite)};
        if (!CreateDockSiteForManager(manager, info, state.dockSites.size())) return FALSE;
        state.enabledAutoHideAlignment |= side;
    }
    manager->RecalcLayout();
    return TRUE;
}
// Symbol: ?EnablePaneContextMenu@CDockingManager@@QEAAXHIAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__EnablePaneContextMenu_CDockingManager__QEAAXHIAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CDockingManager* pThis, int bEnable, unsigned int uiCustomizeCmd, const CString* strCustomizeText, int) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    state.paneContextMenu = bEnable != FALSE;
    state.paneContextMenuID = uiCustomizeCmd;
    state.paneContextMenuName = strCustomizeText ? *strCustomizeText : CString();
}
// Symbol: ?FindPaneByID@CDockingManager@@UEAAPEAVCBasePane@@IH@Z
extern "C" CBasePane* MS_ABI impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
    CDockingManager* pThis, unsigned int nID, int bSearchMiniFrames) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    for (CBasePane* pane : state->panes) {
        if (!bSearchMiniFrames && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) {
            continue;
        }
        if (PaneCommandID(pane) == nID) return pane;
    }

    if (!bSearchMiniFrames) return nullptr;
    return nullptr;
}
// Symbol: ?FixupVirtualRects@CDockingManager@@UEAAXXZ
extern "C" void MS_ABI impl__FixupVirtualRects_CDockingManager__UEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    int idx = 0;
    for (CBasePane* pane : state->panes) {
        if (!pane || !pane->GetSafeHwnd()) continue;
        CRect rect{};
        {
            std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
            PaneCoreState& paneState = g_paneCoreState_Mfccore[pane];
            rect = paneState.recentRect;
            paneState.visible = TRUE;
        }

        if (!IsDrawableRect(rect)) rect = DefaultMiniFrameRect(idx++);
        if (rect.Width() <= 0 || rect.Height() <= 0) {
            rect.right = rect.left + 240;
            rect.bottom = rect.top + 180;
        }

        HWND hWnd = pane->GetSafeHwnd();
        if (hWnd) {
            pane->SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        }
    }
}
// Symbol: ?FloatPane@CDockingManager@@QEAAXPEAVCBasePane@@VCPoint@@K@Z
extern "C" void MS_ABI impl__FloatPane_CDockingManager__QEAAXPEAVCBasePane__VCPoint__K_Z(
    CDockingManager* pThis, CBasePane* pPane, CPoint ptOffset, unsigned long dwAlignment) {
    if (pThis) pThis->FloatPane(pPane, ptOffset, dwAlignment);
}
// Symbol: ?FrameFromPoint@CDockingManager@@UEBAPEAVCPaneFrameWnd@@VCPoint@@PEAV2@H@Z
extern "C" void* MS_ABI impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(
    const CDockingManager* manager, CPoint point, const CPaneFrameWnd* ignore, int multiOnly) {
    const auto* state = FindDockingState(manager);
    if (!state) return nullptr;
    const auto* multiType = multiOnly ? impl__GetThisClass_CMultiPaneFrameWnd__SAPEAUCRuntimeClass__XZ() : nullptr;
    CPaneFrameWnd* bestFrame = nullptr;
    int bestRank = INT_MAX;
    for (void* value : state->miniFrames) {
        auto* frame = static_cast<CPaneFrameWnd*>(value);
        if (!frame || frame == ignore || !::IsWindowVisible(frame->m_hWnd)) continue;
        if (multiOnly && (!multiType || !frame->IsKindOf(multiType))) continue;
        RECT rect{};
        if (!::GetWindowRect(frame->m_hWnd, &rect) || !::PtInRect(&rect, point)) continue;
        const int rank = MiniFrameZRank(frame->m_hWnd);
        if (!bestFrame || rank < bestRank) {
            bestRank = rank;
            bestFrame = frame;
        }
    }
    return bestFrame;
}
// Symbol: ?GetPaneList@CDockingManager@@QEAAXAEAVCObList@@HPEAUCRuntimeClass@@H@Z
extern "C" void MS_ABI impl__GetPaneList_CDockingManager__QEAAXAEAVCObList__HPEAUCRuntimeClass__H_Z(
    CDockingManager* pThis, CObList* pList, int bIncludeHidden, CRuntimeClass* pRTCFilter, int) {
    if (!pList) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    for (CBasePane* pane : state->panes) {
        if (!pane) continue;
        if (!bIncludeHidden && state->hiddenPanes.find(pane) != state->hiddenPanes.end()) continue;
        if (pRTCFilter && !pane->IsKindOf(pRTCFilter)) continue;
        pList->AddTail(static_cast<CObject*>(pane));
    }
}
// Symbol: ?HideAutoHidePanes@CDockingManager@@QEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__HideAutoHidePanes_CDockingManager__QEAAXPEAVCDockablePane__H_Z(
    CDockingManager* manager, CDockablePane* exclude, BOOL immediately) {
    const auto* state = FindDockingState(manager);
    if (!state) return;
    std::vector<HWND> windows;
    for (const auto& bar : state->autoHideBars) {
        for (auto pos = bar->m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
            const auto* button = static_cast<const CMFCAutoHideButton*>(bar->m_lstAutoHideButtons.GetNext(pos));
            const auto* pane = button->m_pAutoHideWindow;
            if (pane && pane != exclude && ::IsWindowVisible(pane->m_hWnd)) windows.push_back(pane->m_hWnd);
        }
    }
    for (HWND window : windows) {
        auto* pane = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
        if (pane && pane->IsKindOf(RUNTIME_CLASS(CDockablePane)))
            impl__Slide_CDockablePane__UEAAXHH_Z(static_cast<CDockablePane*>(pane), FALSE, !immediately);
    }
}
// Symbol: ?InsertPane@CDockingManager@@QEAAHPEAVCBasePane@@0H@Z
extern "C" int MS_ABI impl__InsertPane_CDockingManager__QEAAHPEAVCBasePane__0H_Z(
    CDockingManager* manager, CBasePane* pane, CBasePane* target, int after) {
    if (!manager || !pane || !target) return FALSE;
    auto& panes = EnsureDockingState(manager).panes;
    if (ContainsPtr(panes, pane)) return FALSE;
    const auto position = std::find(panes.begin(), panes.end(), target);
    if (position == panes.end()) return FALSE;
    panes.insert(after ? std::next(position) : position, pane);
    return TRUE;
}
// Symbol: ?LoadState@CDockingManager@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__LoadState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t* lpszProfileName, unsigned int) {
    if (!pThis) return FALSE;
    DockingManagerState& state = EnsureDockingState(pThis);
    std::lock_guard<std::mutex> lock(g_dockingProfileMutex);
    DockingManagerSnapshot* snapshot = FindDockingProfileSnapshot(pThis, lpszProfileName);
    if (!snapshot) return TRUE;

    const std::size_t kCap = snapshot->ids.size();
    std::vector<CBasePane*> ordered;
    std::unordered_set<CBasePane*> restored;
    ordered.reserve(state.panes.size());

    for (std::size_t i = 0; i < kCap; ++i) {
        if (i >= snapshot->rects.size() || i >= snapshot->hidden.size() || i >= snapshot->floating.size()) break;
        CBasePane* pane = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(
            pThis, snapshot->ids[i], TRUE);
        if (!pane) continue;
        if (restored.find(pane) != restored.end()) continue;

        if (snapshot->hidden[i]) state.hiddenPanes.insert(pane); else state.hiddenPanes.erase(pane);
        if (snapshot->floating[i]) state.floatingPanes.insert(pane); else state.floatingPanes.erase(pane);

        std::lock_guard<std::mutex> paneLock(g_paneCoreStateMutex_Mfccore);
        g_paneCoreState_Mfccore[pane].recentRect = snapshot->rects[i];
        g_paneCoreState_Mfccore[pane].visible = !snapshot->hidden[i];

        ordered.push_back(pane);
        restored.insert(pane);
    }

    for (CBasePane* pane : state.panes) {
        if (pane && restored.find(pane) == restored.end()) ordered.push_back(pane);
    }
    if (!ordered.empty()) state.panes = ordered;

    pThis->RecalcLayout();
    return TRUE;
}
// Symbol: ?LockUpdate@CDockingManager@@QEAAXH@Z
extern "C" void MS_ABI impl__LockUpdate_CDockingManager__QEAAXH_Z(CDockingManager* pThis, int bLock) {
    if (pThis) EnsureDockingState(pThis).lockUpdate = bLock != FALSE;
}
// Symbol: ?OnActivateFrame@CDockingManager@@UEAAXH@Z
extern "C" void MS_ABI impl__OnActivateFrame_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int bActivate) {
    if (!pThis || !bActivate) return;
    pThis->RecalcLayout();
    impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(pThis);
}
// Symbol: ?OnClosePopupMenu@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__OnClosePopupMenu_CDockingManager__QEAAXXZ(CDockingManager*) {
    if (CMFCPopupMenu* popup = CMFCPopupMenu::GetSafeActivePopupMenu()) {
        popup->CloseMenu(TRUE);
    }
}
// Symbol: ?OnMoveMiniFrame@CDockingManager@@UEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__OnMoveMiniFrame_CDockingManager__UEAAHPEAVCWnd___Z(CDockingManager* pThis, CWnd* pFrame) {
    if (!pThis || !pFrame) return FALSE;

    auto& frames = EnsureDockingState(pThis).miniFrames;
    auto it = std::find(frames.begin(), frames.end(), pFrame);
    if (it == frames.end()) {
        frames.push_back(pFrame);
    }

    HWND hWnd = pFrame->GetSafeHwnd();
    if (!hWnd) return TRUE;

    RECT rect{};
    ::GetWindowRect(hWnd, &rect);
    if (!IsDrawableRect(CRect(rect))) return FALSE;

    pFrame->InvalidateRect(nullptr, FALSE);
    return TRUE;
}
// Symbol: ?OnPaneContextMenu@CDockingManager@@QEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnPaneContextMenu_CDockingManager__QEAAXVCPoint___Z(CDockingManager* pThis, CPoint pt) {
    if (!pThis) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state || state->panes.empty()) return;

    CMenu menu;
    if (!menu.CreatePopupMenu()) return;
    impl__BuildPanesMenu_CDockingManager__QEAAXAEAVCMenu__H_Z(pThis, &menu, 5000);

    HMENU hMenu = menu.GetSafeHmenu();
    if (!hMenu) return;
    if (::GetMenuItemCount(hMenu) <= 0) return;
    ::TrackPopupMenu(hMenu, TPM_LEFTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN,
                     pt.x, pt.y, 0, nullptr, nullptr);
}
// Symbol: ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@H_NPEAUCRuntimeClass@@HPEBV2@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
    const CDockingManager* manager, CPoint point, int sensitivity, bool exact,
    CRuntimeClass* filter, BOOL checkVisibility, const CBasePane* ignore) {
    const auto* state = FindDockingState(manager);
    if (!state) return nullptr;
    const auto* ignoredFrame = ParentPaneFrame(ignore);
    const auto matches = [point, filter, checkVisibility, ignore, &state](CWnd* candidate, int margin) {
        if (!candidate || candidate == ignore || !candidate->m_hWnd ||
            !candidate->IsKindOf(RUNTIME_CLASS(CBasePane)) || (filter && !candidate->IsKindOf(filter)))
            return false;
        if (ignore && ::IsChild(ignore->m_hWnd, candidate->m_hWnd)) return false;
        if (checkVisibility && (!::IsWindowVisible(candidate->m_hWnd) ||
            state->hiddenPanes.count(static_cast<CBasePane*>(candidate)))) return false;
        RECT bounds{};
        if (!::GetWindowRect(candidate->m_hWnd, &bounds)) return false;
        if (margin) ::InflateRect(&bounds, margin, margin);
        return ::PtInRect(&bounds, point) != FALSE;
    };
    auto* frame = static_cast<CPaneFrameWnd*>(
        impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(manager, point, ignoredFrame, FALSE));
    if (frame) {
        auto* pane = frame->GetPane();
        if (matches(pane, 0)) return static_cast<CBasePane*>(pane);
    }
    const int margin = exact || sensitivity < 0 ? 0 : (std::min)(sensitivity, 32767);
    for (auto* pane : state->panes) {
        if (!state->floatingPanes.count(pane) && matches(pane, margin)) return pane;
    }
    for (const auto& entry : state->dockSites) {
        if (!entry.autoHide && matches(entry.site, margin)) return entry.site;
    }
    return nullptr;
}

// Symbol: ?PaneFromPoint@CDockingManager@@UEBAPEAVCBasePane@@VCPoint@@HAEAKPEAUCRuntimeClass@@PEBV2@@Z
extern "C" CBasePane* MS_ABI impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__HAEAKPEAUCRuntimeClass__PEBV2__Z(
    const CDockingManager* manager, CPoint point, int sensitivity, DWORD* alignment,
    CRuntimeClass* filter, const CBasePane* ignore) {
    if (!alignment) return nullptr;
    *alignment = 0;
    auto* pane = impl__PaneFromPoint_CDockingManager__UEBAPEAVCBasePane__VCPoint__H_NPEAUCRuntimeClass__HPEBV2__Z(
        manager, point, sensitivity, true, filter, FALSE, ignore);
    if (!pane || !impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
        nullptr, PackedDockPoint(point), pane, (std::clamp)(sensitivity, 0, 32767), manager,
        FALSE, alignment, CBRS_ALIGN_ANY, nullptr)) return nullptr;
    return pane;
}
// Symbol: ?ProcessPaneContextMenuCommand@CDockingManager@@QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__ProcessPaneContextMenuCommand_CDockingManager__QEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CDockingManager* pThis, unsigned int nID, int, void*, void*) {
    if (!pThis || nID == 0) return FALSE;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return FALSE;

    CBasePane* target = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(pThis, nID, TRUE);
    if (!target) return FALSE;

    if (state->hiddenPanes.find(target) == state->hiddenPanes.end()) {
        pThis->HidePane(target);
    } else {
        pThis->ShowPane(target, FALSE);
    }

    return TRUE;
}
// Symbol: ?RecalcLayout@CDockingManager@@UEAAXH@Z
extern "C" void MS_ABI impl__RecalcLayout_CDockingManager__UEAAXH_Z(CDockingManager* pThis, int) {
    if (pThis) pThis->RecalcLayout();
}
// Symbol: ?RedrawAllMiniFrames@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__RedrawAllMiniFrames_CDockingManager__QEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return;

    for (void* pFrame : state->miniFrames) {
        if (!pFrame) continue;
        CWnd* frame = static_cast<CWnd*>(pFrame);
        HWND hWnd = frame ? frame->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        ::InvalidateRect(hWnd, nullptr, TRUE);
        ::UpdateWindow(hWnd);
    }
}
// Symbol: ?ReleaseEmptyPaneContainers@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__ReleaseEmptyPaneContainers_CDockingManager__QEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    std::vector<CBasePane*> oldPanes = state.panes;
    state.panes.erase(std::remove_if(state.panes.begin(), state.panes.end(),
                                    [](CBasePane* pane) { return pane == nullptr || !pane->GetSafeHwnd(); }),
                     state.panes.end());

    const auto isAlive = [&state](CBasePane* pane) {
        return std::find(state.panes.begin(), state.panes.end(), pane) != state.panes.end();
    };
    for (CBasePane* pane : oldPanes) {
        if (!pane || isAlive(pane)) continue;
        state.hiddenPanes.erase(pane);
        state.floatingPanes.erase(pane);
        state.edgePanes.erase(pane);
        g_paneCoreState_Mfccore.erase(pane);
    }

    state.miniFrames.erase(std::remove_if(state.miniFrames.begin(), state.miniFrames.end(),
                                          [](void* frame) {
                                              if (!frame) return true;
                                              CWnd* wnd = static_cast<CWnd*>(frame);
                                              return wnd->GetSafeHwnd() == nullptr;
                                          }),
                           state.miniFrames.end());
}
// Symbol: ?RemoveHiddenMDITabbedBar@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemoveHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(CDockingManager* pThis, CDockablePane* pPane) {
    RemoveDockingPane(pThis, pPane);
}
// Symbol: ?RemoveMiniFrame@CDockingManager@@UEAAHPEAVCPaneFrameWnd@@@Z
extern "C" int MS_ABI impl__RemoveMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(CDockingManager* pThis, void* pFrame) {
    if (!pThis || !pFrame) return FALSE;
    auto& frames = EnsureDockingState(pThis).miniFrames;
    auto oldSize = frames.size();
    frames.erase(std::remove(frames.begin(), frames.end(), pFrame), frames.end());
    return frames.size() != oldSize;
}
// Symbol: ?RemovePaneFromDockManager@CDockingManager@@QEAAXPEAVCBasePane@@HHH0@Z
extern "C" void MS_ABI impl__RemovePaneFromDockManager_CDockingManager__QEAAXPEAVCBasePane__HHH0_Z(
    CDockingManager* pThis, CBasePane* pPane, int, int, int, CBasePane*) {
    RemoveDockingPane(pThis, pPane);
}
// Symbol: ?ReplacePane@CDockingManager@@QEAAHPEAVCDockablePane@@0@Z
extern "C" int MS_ABI impl__ReplacePane_CDockingManager__QEAAHPEAVCDockablePane__0_Z(
    CDockingManager* pThis, CDockablePane* pOldPane, CDockablePane* pNewPane) {
    if (!pThis || !pOldPane || !pNewPane) return FALSE;
    DockingManagerState& state = EnsureDockingState(pThis);
    auto it = std::find(state.panes.begin(), state.panes.end(), static_cast<CBasePane*>(pOldPane));
    if (it == state.panes.end()) return FALSE;
    *it = pNewPane;
    if (auto edge = state.edgePanes.extract(pOldPane); !edge.empty()) {
        edge.key() = pNewPane;
        state.edgePanes.insert(std::move(edge));
    }
    state.hiddenPanes.erase(pOldPane);
    state.floatingPanes.erase(pOldPane);
    return TRUE;
}
// Symbol: ?ResortMiniFramesForZOrder@CDockingManager@@QEAAXXZ
extern "C" void MS_ABI impl__ResortMiniFramesForZOrder_CDockingManager__QEAAXXZ(CDockingManager* pThis) {
    if (!pThis) return;
    DockingManagerState& state = EnsureDockingState(pThis);
    if (state.miniFrames.size() < 2) return;

    std::sort(state.miniFrames.begin(), state.miniFrames.end(),
              [](void* lhs, void* rhs) {
                  HWND left = lhs ? static_cast<CWnd*>(lhs)->GetSafeHwnd() : nullptr;
                  HWND right = rhs ? static_cast<CWnd*>(rhs)->GetSafeHwnd() : nullptr;
                  return MiniFrameZRank(left) < MiniFrameZRank(right);
              });

    for (void* frame : state.miniFrames) {
        if (!frame) continue;
        CWnd* wnd = static_cast<CWnd*>(frame);
        HWND hWnd = wnd ? wnd->GetSafeHwnd() : nullptr;
        if (!hWnd) continue;
        ::SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    }
}
// Symbol: ?SaveState@CDockingManager@@UEAAHPEB_WI@Z
extern "C" int MS_ABI impl__SaveState_CDockingManager__UEAAHPEB_WI_Z(CDockingManager* pThis, const wchar_t* lpszProfileName, unsigned int) {
    if (!pThis) return FALSE;
    const DockingManagerState& state = EnsureDockingState(pThis);
    DockingManagerSnapshot snapshot;

    std::unordered_set<UINT> usedIds;
    for (CBasePane* pane : state.panes) {
        if (!pane) continue;
        UINT id = PaneCommandID(pane);
        if (id == 0 || usedIds.find(id) != usedIds.end()) continue;
        std::lock_guard<std::mutex> paneLock(g_paneCoreStateMutex_Mfccore);
        const auto paneStateIt = g_paneCoreState_Mfccore.find(pane);

        CRect rect = paneStateIt == g_paneCoreState_Mfccore.end() ? CRect() : paneStateIt->second.recentRect;
        if (rect.Width() <= 0 || rect.Height() <= 0) {
            HWND hWnd = pane->GetSafeHwnd();
            if (hWnd) {
                RECT rc{};
                pane->GetWindowRect(&rc);
                rect = rc;
            }
        }

        snapshot.ids.push_back(id);
        snapshot.rects.push_back(rect);
        snapshot.hidden.push_back(state.hiddenPanes.find(pane) != state.hiddenPanes.end() ? TRUE : FALSE);
        snapshot.floating.push_back(state.floatingPanes.find(pane) != state.floatingPanes.end() ? TRUE : FALSE);
        usedIds.insert(id);
    }

    const std::wstring profile = DockingProfileName(lpszProfileName);
    std::lock_guard<std::mutex> lock(g_dockingProfileMutex);
    g_dockingProfileStates[pThis][profile] = std::move(snapshot);
    return TRUE;
}
// Symbol: ?SendMessageToMiniFrames@CDockingManager@@QEAAHI_K_J@Z
extern "C" int MS_ABI impl__SendMessageToMiniFrames_CDockingManager__QEAAHI_K_J_Z(
    CDockingManager* pThis, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return FALSE;

    int sent = 0;
    for (void* pFrame : state->miniFrames) {
        CWnd* frame = static_cast<CWnd*>(pFrame);
        if (!frame) continue;
        HWND hWnd = frame->GetSafeHwnd();
        if (!hWnd) continue;
        ::SendMessageW(hWnd, message, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
        ++sent;
    }
    return sent > 0 ? TRUE : FALSE;
}
CDockingManager::CDockingManager()
    : m_pParentWnd(nullptr) {
    memset(_dockingmanager_padding, 0, sizeof(_dockingmanager_padding));
    EnsureDockingState(this).parentFrame = nullptr;
}
CDockingManager::CDockingManager(CFrameWnd* pParentFrameWnd)
    : m_pParentWnd(pParentFrameWnd) {
    memset(_dockingmanager_padding, 0, sizeof(_dockingmanager_padding));
    EnsureDockingState(this).parentFrame = pParentFrameWnd;
}
CDockingManager::~CDockingManager() {
    if (const DockingManagerState* found = FindDockingState(this)) {
        DockingManagerState& state = const_cast<DockingManagerState&>(*found);
        while (!state.miniFrames.empty()) {
            auto* frame = static_cast<CWnd*>(state.miniFrames.back());
            state.miniFrames.pop_back();
            if (frame && frame->m_hWnd) frame->DestroyWindow();
        }
        while (!state.autoHideBars.empty()) {
            auto bar = std::move(state.autoHideBars.back());
            state.autoHideBars.pop_back();
            bar.reset();
        }
        while (!state.dockSites.empty()) {
            auto* site = state.dockSites.back().site;
            state.dockSites.pop_back();
            delete site;
        }
        std::free(state.hiddenMDITabbedBars);
        std::free(state.printPreviewHiddenBars);
        state.hiddenMDITabbedBars = nullptr;
        state.printPreviewHiddenBars = nullptr;
        state.hiddenMDITabbedBarCount = state.hiddenMDITabbedBarCapacity = 0;
        state.printPreviewHiddenBarCount = state.printPreviewHiddenBarCapacity = 0;
    }
    RemoveDockingState(this);
    std::lock_guard<std::mutex> lock(g_dockingProfileMutex);
    g_dockingProfileStates.erase(this);
}
void CDockingManager::DockPane(CBasePane* pBar, UINT, LPCRECT) {
    AddDockingPane(this, pBar);
    DockingManagerState& state = EnsureDockingState(this);
    state.hiddenPanes.erase(pBar);
    state.floatingPanes.erase(pBar);
}
BOOL CDockingManager::DockPaneLeftOf(CBasePane* pBarToDock, CBasePane* pBar) {
    if (!pBarToDock) return FALSE;

    DockingManagerState& state = EnsureDockingState(this);
    state.panes.erase(std::remove(state.panes.begin(), state.panes.end(), pBarToDock), state.panes.end());

    auto it = std::find(state.panes.begin(), state.panes.end(), pBar);
    if (it != state.panes.end()) {
        state.panes.insert(it, pBarToDock);
    } else {
        state.panes.push_back(pBarToDock);
    }

    state.hiddenPanes.erase(pBarToDock);
    state.floatingPanes.erase(pBarToDock);
    return TRUE;
}
BOOL CDockingManager::EnableDocking(DWORD dwDockStyle) {
    auto& state = EnsureDockingState(this);
    for (DWORD side : dockingEdges) {
        if (!(dwDockStyle & side) || (state.enabledAlignment & side)) continue;
        const DockSiteInfoShadow info{side, RUNTIME_CLASS(CDockSite)};
        if (!CreateDockSiteForManager(this, info, state.dockSites.size())) return FALSE;
        state.enabledAlignment |= side;
    }
    RecalcLayout();
    return TRUE;
}

void openmfc::detail::mfccore::ForgetDockSite(CDockingManager* manager, CDockSite* site) {
    const auto* found = FindDockingState(manager);
    if (!found) return;
    auto& state = const_cast<DockingManagerState&>(*found);
    const auto entry = std::find_if(state.dockSites.begin(), state.dockSites.end(),
        [site](const auto& value) { return value.site == site; });
    if (entry == state.dockSites.end()) return;
    const auto removed = *entry;
    state.dockSites.erase(entry);
    const bool hasReplacement = std::any_of(state.dockSites.begin(), state.dockSites.end(),
        [removed](const auto& value) {
            return value.alignment == removed.alignment && value.autoHide == removed.autoHide;
        });
    if (!hasReplacement) {
        auto& enabled = removed.autoHide ? state.enabledAutoHideAlignment : state.enabledAlignment;
        enabled &= ~removed.alignment;
    }
}
void CDockingManager::FloatPane(CBasePane* pane, CPoint offset, DWORD) {
    if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CPane))) return;
    RECT bounds{};
    if (!::GetWindowRect(pane->m_hWnd, &bounds)) return;
    ::OffsetRect(&bounds, offset.x, offset.y);
    static_cast<CPane*>(pane)->FloatPaneWindow(bounds, 5 /* DM_STANDARD */, true, this);
}
namespace {
void ShowDockingWindows(DockingManagerState& state, CBasePane* pane, int command) {
    if (!pane->m_hWnd) return;
    if (state.floatingPanes.count(pane)) {
        ::ShowWindow(pane->m_hWnd, command);
        ::ShowWindow(::GetParent(pane->m_hWnd), command);
    } else if (const auto edge = state.edgePanes.find(pane); edge != state.edgePanes.end()) {
        ::ShowWindow(pane->m_hWnd, command);
        ::ShowWindow(edge->second.divider, command);
    }
}
}

void CDockingManager::HidePane(CBasePane* pane) {
    if (!pane) return;
    AddDockingPane(this, pane);
    auto& state = EnsureDockingState(this);
    state.hiddenPanes.insert(pane);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        g_paneCoreState_Mfccore[pane].visible = FALSE;
    }
    ShowDockingWindows(state, pane, SW_HIDE);
}

void CDockingManager::ShowPane(CBasePane* pane, BOOL) {
    if (!pane) return;
    AddDockingPane(this, pane);
    auto& state = EnsureDockingState(this);
    state.hiddenPanes.erase(pane);
    {
        std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
        g_paneCoreState_Mfccore[pane].visible = TRUE;
    }
    ShowDockingWindows(state, pane, SW_SHOWNOACTIVATE);
}

namespace {
void ArrangeEdgePanes(DockingManagerState& state, RECT& remaining) {
    if (state.edgePanes.empty() || !state.parentFrame) return;
    const HWND host = state.parentFrame->m_hWnd;
    struct Placement { HWND pane; HWND divider; RECT bounds; RECT sash; };
    std::vector<Placement> positions;
    for (bool outer : {true, false}) {
        for (auto* pane : state.panes) {
            const auto it = state.edgePanes.find(pane);
            if (it == state.edgePanes.end() || it->second.outer != outer) continue;
            const auto& dock = it->second;
            if (!IsPaneVisibleForDocking(state, pane) || state.floatingPanes.count(pane) ||
                !pane->m_hWnd || ::GetParent(pane->m_hWnd) != host) continue;

            RECT sash{};
            ::GetWindowRect(dock.divider, &sash);
            const DWORD side = pane->GetPaneAlignment();
            const bool vertical = side == CBRS_ALIGN_LEFT || side == CBRS_ALIGN_RIGHT;
            const LONG span = vertical ? remaining.right - remaining.left : remaining.bottom - remaining.top;
            const LONG sashSize = (std::clamp)(vertical ? sash.right - sash.left : sash.bottom - sash.top, 0L, span);
            const LONG thickness = (std::min)(dock.thickness, span - sashSize);
            RECT bounds = remaining;
            sash = remaining;
            switch (side) {
            case CBRS_ALIGN_LEFT:
                bounds.right = sash.left = remaining.left + thickness;
                sash.right = remaining.left = sash.left + sashSize;
                break;
            case CBRS_ALIGN_RIGHT:
                bounds.left = sash.right = remaining.right - thickness;
                sash.left = remaining.right = sash.right - sashSize;
                break;
            case CBRS_ALIGN_TOP:
                bounds.bottom = sash.top = remaining.top + thickness;
                sash.bottom = remaining.top = sash.top + sashSize;
                break;
            case CBRS_ALIGN_BOTTOM:
                bounds.top = sash.bottom = remaining.bottom - thickness;
                sash.top = remaining.bottom = sash.bottom - sashSize;
                break;
            default: continue;
            }
            positions.push_back({pane->m_hWnd, dock.divider, bounds, sash});
        }
    }
    for (const auto& position : positions) {
        const auto place = [](HWND window, const RECT& rect) {
            ::SetWindowPos(window, nullptr, rect.left, rect.top, (std::max)(0L, rect.right - rect.left),
                (std::max)(0L, rect.bottom - rect.top), SWP_NOZORDER | SWP_NOACTIVATE);
        };
        place(position.pane, position.bounds);
        place(position.divider, position.sash);
    }
}
}

void CDockingManager::RecalcLayout() {
    DockingManagerState& state = EnsureDockingState(this);
    if (state.lockUpdate) return;
    if (state.inPlaceBounds) {
        LayoutInPlacePanes(this, *state.inPlaceBounds, true);
        return;
    }

    RECT client{0, 0, 0, 0};
    HWND parent = m_pParentWnd ? m_pParentWnd->GetSafeHwnd() : nullptr;
    if (parent) {
        ::GetClientRect(parent, &client);
        const RECT frameBounds = client;
        ArrangeEdgePanes(state, client);
        state.clientAreaBounds = client;
        LayoutDockSites(*this, state, frameBounds);
    }
    const auto canTile = [&state](CBasePane* pane) {
        if (!IsPaneVisibleForDocking(state, pane) || state.floatingPanes.count(pane) ||
            state.edgePanes.count(pane)) return false;
        auto* parent = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(pane->m_hWnd)));
        return !parent || !parent->IsKindOf(RUNTIME_CLASS(CDockSite));
    };
    const int visibleCount = static_cast<int>(std::count_if(state.panes.begin(), state.panes.end(), canTile));
    if (visibleCount == 0) return;

    int index = 0;
    const int width = (client.right > client.left) ? (client.right - client.left) / visibleCount : 0;
    for (CBasePane* pane : state.panes) {
        if (!canTile(pane)) continue;
        CRect rect(client.left + index * width, client.top,
                   index == visibleCount - 1 ? client.right : client.left + (index + 1) * width,
                   client.bottom);
        {
            std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
            g_paneCoreState_Mfccore[pane].recentRect = rect;
            g_paneCoreState_Mfccore[pane].visible = TRUE;
        }
        if (pane->GetSafeHwnd() && rect.Width() > 0 && rect.Height() > 0) {
            pane->SetWindowPos(nullptr, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE, nullptr);
        } else {
            pane->RecalcLayout();
        }
        ++index;
    }
}

RECT openmfc::detail::mfccore::LayoutInPlacePanes(CDockingManager* manager, const RECT& available,
                                                 bool moveWindows) {
    using namespace openmfc::detail::dock;
    auto& state = EnsureDockingState(manager);
    if (state.positioningInPlace || state.lockUpdate) return state.clientAreaBounds;
    state.inPlaceBounds = available;
    RECT remaining = available;
    const HWND parent = state.parentFrame ? state.parentFrame->m_hWnd : nullptr;
    HDWP layout = nullptr;
    if (parent && !::IsRectEmpty(&available)) {
        for (auto* pane : state.panes) {
            if (!IsPaneVisibleForDocking(state, pane) || state.floatingPanes.count(pane)) continue;
            const HWND window = pane->m_hWnd;
            if (!window || ::GetParent(window) != parent || !(::GetWindowLongW(window, GWL_STYLE) & WS_VISIBLE))
                continue;

            RECT current{};
            if (!::GetWindowRect(window, &current)) continue;
            DWORD alignment = pane->GetPaneAlignment() & alignAny;
            if (!alignment) alignment = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pane) & alignAny;
            const bool horizontal = alignment == alignTop || alignment == alignBottom;
            const LONG currentSize = horizontal ? current.bottom - current.top : current.right - current.left;
            auto& saved = state.inPlacePaneSizes[pane];
            if (saved.window != window || saved.alignment != alignment || saved.lastLayout != currentSize)
                saved = {window, alignment, currentSize, currentSize};
            const LONG availableSize = horizontal ? remaining.bottom - remaining.top : remaining.right - remaining.left;
            const LONG thickness = (std::clamp)(saved.preferred, 0L, availableSize);
            RECT position = remaining;
            switch (alignment) {
            case alignLeft: remaining.left += thickness; position.right = remaining.left; break;
            case alignTop: remaining.top += thickness; position.bottom = remaining.top; break;
            case alignRight: remaining.right -= thickness; position.left = remaining.right; break;
            case alignBottom: remaining.bottom -= thickness; position.top = remaining.bottom; break;
            default: continue;
            }
            if (moveWindows) {
                // Defer the physical moves until the registry walk has completed.
                if (!layout) layout = ::BeginDeferWindowPos(8);
                if (layout) layout = ::DeferWindowPos(layout, window, nullptr, position.left, position.top,
                    position.right - position.left, position.bottom - position.top, SWP_NOZORDER | SWP_NOACTIVATE);
                if (layout) saved.lastLayout = thickness;
            }
        }
    }
    state.clientAreaBounds = remaining;
    if (layout) {
        state.positioningInPlace = true;
        ::EndDeferWindowPos(layout);
        state.positioningInPlace = false;
    }
    if (moveWindows) LayoutDockSites(*manager, state, available);
    return remaining;
}
void CDockingManager::SetDockState() {
    DockingManagerState& state = EnsureDockingState(this);
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    for (CBasePane* pane : state.panes) {
        if (!pane) continue;
        PaneCoreState& paneState = g_paneCoreState_Mfccore[pane];
        paneState.visible = state.hiddenPanes.find(pane) == state.hiddenPanes.end();
        paneState.canFloat = paneState.canFloat || state.floatingPanes.find(pane) != state.floatingPanes.end();
        if (state.enabledAlignment != 0 && paneState.alignment == 0) {
            paneState.alignment = state.enabledAlignment;
        }
    }
}
// Symbol: ?m_SDParams@CDockingManager@@1VCSmartDockingInfo@@A
// Retail's exported static is a CSmartDockingInfo at RVA 0x3c15b0 (mfc140u), not a pointer.
// Seed its verified constructor defaults in static storage; the ctor thunk below installs
// the vtable and applies the same defaults before exported calls can observe the object.
extern "C" S_Csmartdockinginfo impl__m_SDParams_CDockingManager__1VCSmartDockingInfo__A = {
    nullptr, 93, 93, 5,
    0x00FF00FFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
    {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, 0, FALSE, FALSE
};

namespace {
struct DockingSmartParamsInitializer {
    DockingSmartParamsInitializer() {
        // Use the already-defined constructor thunk: retail's CSmartDockingInfo ctor
        // initializes a 93x93 marker size, offset 5, magenta transparency, and zero IDs.
        impl___0CSmartDockingInfo__QEAA_XZ(&impl__m_SDParams_CDockingManager__1VCSmartDockingInfo__A);
    }
};
DockingSmartParamsInitializer g_dockingSmartParamsInitializer;
}

// The following side-table implementations use defensive null checks; retail often
// dereferences these inputs. Side-array allocation failure also returns FALSE rather
// than reproducing an unmodeled allocation-exception path.
// Symbol: ?AddDockSite@CDockingManager@@QEAAHAEBUAFX_DOCKSITE_INFO@@PEAPEAVCDockSite@@@Z
extern "C" BOOL MS_ABI impl__AddDockSite_CDockingManager__QEAAHAEBUAFX_DOCKSITE_INFO__PEAPEAVCDockSite___Z(
    CDockingManager* manager, const DockSiteInfoShadow* info, CDockSite** result) {
    if (result) *result = nullptr;
    if (!manager || !info) return FALSE;
    auto* site = CreateDockSiteForManager(manager, *info, EnsureDockingState(manager).dockSites.size());
    if (result) *result = site;
    return site != nullptr;
}

// Symbol: ?AddHiddenMDITabbedBar@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
// Retail entry RVA 0x4ebc0 (mfc140u) appends pPane->m_hWnd (+0x40) to the
// manager's hidden-MDI-tabbed-bar list (+0x2d0). The side table stores that HWND
// bit-pattern in its pointer-sized hiddenMDITabbedBars entries.
extern "C" void MS_ABI impl__AddHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(
    CDockingManager* pThis, CDockablePane* pPane) {
    if (!pThis) return;
    HWND hWnd = pPane ? impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(
                            static_cast<const CWnd*>(pPane)) : nullptr;
    DockingManagerState& state = EnsureDockingState(pThis);
    InsertManagerStateEntry(state.hiddenMDITabbedBars,
                            state.hiddenMDITabbedBarCount,
                            state.hiddenMDITabbedBarCapacity,
                            state.hiddenMDITabbedBarCount,
                            reinterpret_cast<CBasePane*>(hWnd));
}

// Symbol: ?AlignByRect@CDockingManager@@IEAAXAEBVCRect@@AEAV2@KHH@Z
// Retail RVA 0x4a340 (mfc140u): apply the 0xF000 alignment case and the two
// size-adjustment flags to the target rectangle relative to the anchor rectangle.
// WARNING: This pre-existing approximation does not preserve the retail per-case
// coordinates: retail leaves one or both target vertical edges unchanged in cases
// where this body recomputes them from the anchor. Kept because this does not write
// outside either caller-provided CRect; it is not a faithful retail transcription.
extern "C" void MS_ABI impl__AlignByRect_CDockingManager__IEAAXAEBVCRect__AEAV2_KHH_Z(
    CDockingManager*, const CRect* pAnchor, CRect* pTarget, unsigned long dwAlignment,
    int bAdjustFirst, int bAdjustSecond) {
    const int anchorLeft = pAnchor->left;
    const int anchorTop = pAnchor->top;
    const int anchorRight = pAnchor->right;
    const int anchorBottom = pAnchor->bottom;
    const int targetWidth = pTarget->right - pTarget->left;
    const int targetHeight = pTarget->bottom - pTarget->top;
    const unsigned long alignment = dwAlignment & 0xf000;

    switch (alignment) {
    case 0x1000:
        pTarget->left = anchorLeft;
        pTarget->top = anchorTop;
        pTarget->right = anchorLeft + targetWidth;
        pTarget->bottom = anchorBottom;
        break;
    case 0x2000:
        pTarget->left = anchorLeft;
        pTarget->top = anchorTop;
        pTarget->right = anchorRight;
        pTarget->bottom = anchorTop + targetHeight;
        break;
    case 0x4000:
        pTarget->right = anchorRight;
        pTarget->bottom = anchorBottom;
        pTarget->left = anchorRight - targetWidth;
        pTarget->top = anchorTop;
        break;
    case 0x8000:
        pTarget->right = anchorRight;
        pTarget->bottom = anchorBottom;
        pTarget->left = anchorLeft;
        pTarget->top = anchorBottom - targetHeight;
        break;
    default:
        break;
    }

    if (bAdjustFirst) {
        const int widthDelta = (pAnchor->left - pAnchor->right) - pTarget->left + pTarget->right;
        if (widthDelta != 0) pTarget->right += widthDelta;

        const int heightDelta = (pAnchor->top - pAnchor->bottom) - pTarget->top + pTarget->bottom;
        if (heightDelta > 0 && bAdjustSecond) {
            if ((alignment & 0x2000) != 0) {
                pTarget->bottom -= heightDelta;
            } else if ((alignment & 0x8000) != 0) {
                pTarget->top += heightDelta;
            }
        }
    } else {
        const int heightDelta = (pAnchor->top - pAnchor->bottom) - pTarget->top + pTarget->bottom;
        if (heightDelta != 0) pTarget->bottom += heightDelta;

        const int widthDelta = (pTarget->right - pTarget->left) - (pAnchor->right - pAnchor->left);
        if (widthDelta > 0 && bAdjustSecond) {
            if ((alignment & 0x1000) != 0) {
                pTarget->right -= widthDelta;
            } else if ((alignment & 0x4000) != 0) {
                pTarget->left += widthDelta;
            }
        }
    }
}

// Symbol: ?CalcPaneOffset@CDockingManager@@IEAAXPEAVCAutoHideDockSite@@@Z
// STUB: Retail entry RVA 0x4a100 (mfc140u) compares neighboring auto-hide dock-site
// rectangles/orientations and updates their +0x220/+0x224 offsets. The manager's
// dockSites side array exists, but this TU has no typed accessor for the auto-hide
// site's offset fields and adjacent-site geometry.
extern "C" void MS_ABI impl__CalcPaneOffset_CDockingManager__IEAAXPEAVCAutoHideDockSite___Z(
    CDockingManager*, void*) {}

// Symbol: ?Create@CDockingManager@@QEAAHPEAVCFrameWnd@@@Z
// Retail RVA 0x473e0 (mfc140u) stores the frame at this+0x1b0 and returns TRUE.
// OpenMFC's m_pParentWnd is at this+sizeof(CObject), so store through that local
// member location rather than writing the incompatible retail offset.
extern "C" int MS_ABI impl__Create_CDockingManager__QEAAHPEAVCFrameWnd___Z(
    CDockingManager* pThis, CFrameWnd* pFrame) {
    if (!pThis) return FALSE;
    *reinterpret_cast<CFrameWnd**>(reinterpret_cast<unsigned char*>(pThis) + sizeof(CObject)) = pFrame;
    EnsureDockingState(pThis).parentFrame = pFrame;
    return TRUE;
}

// Symbol: ?FindDockSite@CDockingManager@@UEAAPEAVCDockSite@@KH@Z
// Retail entry RVA 0x481e0 (mfc140u) starts at the head when bOuter is set and
// walks forward; otherwise it starts at the tail and walks backward. It returns the
// first CDockSite whose alignment mask matches. dockSites preserves only the relative
// order of dock sites, not the complete m_lstControlBars sequence.
extern "C" void* MS_ABI impl__FindDockSite_CDockingManager__UEAAPEAVCDockSite__KH_Z(
    CDockingManager* pThis, unsigned long alignment, int bOuter) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!state) return nullptr;

    for (std::size_t step = 0; step < state->dockSites.size(); ++step) {
        const std::size_t index = bOuter ? step : state->dockSites.size() - step - 1;
        const auto& entry = state->dockSites[index];
        if (entry.alignment == (alignment & CBRS_ALIGN_ANY)) return entry.site;
    }
    return nullptr;
}

// Symbol: ?FindDockSiteByPane@CDockingManager@@UEAAPEAVCDockSite@@PEAVCPane@@@Z
// Retail entry RVA 0x48500 (mfc140u) obtains the pane's control ID, scans the
// manager dock-site list, and asks each site to FindPaneByID before comparing pointers.
extern "C" void* MS_ABI impl__FindDockSiteByPane_CDockingManager__UEAAPEAVCDockSite__PEAVCPane___Z(
    CDockingManager* pThis, CPane* pPane) {
    const DockingManagerState* state = FindDockingState(pThis);
    if (!pThis || !pPane || !state) return nullptr;

    const unsigned int paneID = static_cast<unsigned int>(
        impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pPane)));
    for (const auto& entry : state->dockSites) {
        if (impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(entry.site, paneID) == pPane)
            return entry.site;
    }
    return nullptr;
}

// Symbol: ?HideForPrintPreview@CDockingManager@@IEAAXAEBVCObList@@@Z
// STUB: Retail entry RVA 0x4cfb0 (mfc140u) applies virtual type/visibility/eligibility
// checks to the supplied CObList. It separately scans manager mini-frames, validates
// their HWNDs, hides valid windows, and records eligible objects in
// m_lstBarsHiddenInPreview (+0x210). The side table has a saved-entry array, but not
// the input-list traversal and eligibility state.
extern "C" void MS_ABI impl__HideForPrintPreview_CDockingManager__IEAAXAEBVCObList___Z(
    CDockingManager*, const CObList*) {}

// Symbol: ?InsertDockSite@CDockingManager@@QEAAHAEBUAFX_DOCKSITE_INFO@@KPEAPEAVCDockSite@@@Z
extern "C" BOOL MS_ABI impl__InsertDockSite_CDockingManager__QEAAHAEBUAFX_DOCKSITE_INFO__KPEAPEAVCDockSite___Z(
    CDockingManager* manager, const DockSiteInfoShadow* info, DWORD afterAlignment, CDockSite** result) {
    if (result) *result = nullptr;
    if (!manager || !info) return FALSE;

    const auto& sites = EnsureDockingState(manager).dockSites;
    const auto match = std::find_if(sites.begin(), sites.end(), [afterAlignment](const auto& entry) {
        return entry.alignment == (afterAlignment & CBRS_ALIGN_ANY);
    });
    const auto position = match == sites.end() ? sites.size() : std::size_t(match - sites.begin() + 1);
    auto* site = CreateDockSiteForManager(manager, *info, position);
    if (result) *result = site;
    return site != nullptr;
}

// Symbol: ?IsPointNearDockSite@CDockingManager@@QEBAHVCPoint@@AEAKAEAH@Z
// Retail mfc140u 0x48310 checks the outer edge first, then the remaining client area.
extern "C" BOOL MS_ABI impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(
    const CDockingManager* manager, CPoint point, DWORD* alignment, BOOL* outerEdge) {
    DWORD foundAlignment = 0;
    if (alignment) *alignment = 0;
    if (outerEdge) *outerEdge = FALSE;

    const auto* state = FindDockingState(manager);
    if (!state || !state->parentFrame || !state->enabledAlignment) return FALSE;
    auto* frame = state->parentFrame;
    RECT client{};
    if (!::GetClientRect(frame->m_hWnd, &client)) return FALSE;

    RECT outer = state->inPlaceBounds.value_or(client);
    RECT inner = state->inPlaceBounds || !state->edgePanes.empty() ? state->clientAreaBounds : client;
    long long packedPoint;
    static_assert(sizeof(packedPoint) == sizeof(point));
    std::memcpy(&packedPoint, &point, sizeof(point));
    const auto checkEdge = [&](RECT& bounds, BOOL outside) {
        if (::IsRectEmpty(&bounds)) return FALSE;
        frame->ClientToScreen(&bounds);
        return impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
            nullptr, packedPoint, nullptr, impl__m_nDockSensitivity_CDockingManager__2HA,
            manager, outside, &foundAlignment, state->enabledAlignment, &bounds);
    };
    const BOOL outside = checkEdge(outer, TRUE);
    if (!outside && !checkEdge(inner, FALSE)) return FALSE;
    if (alignment) *alignment = foundAlignment;
    if (outerEdge) *outerEdge = outside;
    return TRUE;
}

// Symbol: ?Serialize@CDockingManager@@UEAAXAEAVCArchive@@@Z
// STUB: The ANSI twin's retail entry at RVA 0x4aa90 (mfc140) has a long archive traversal
// over retail dock-site and manager-list state. The side table tracks pane/frame pointers,
// but it is not the retail CObList layout/state required by this serialization routine.
extern "C" void MS_ABI impl__Serialize_CDockingManager__UEAAXAEAVCArchive___Z(
    CDockingManager* pThis, CArchive& archive) {
    (void)pThis;
    (void)archive;
}

// Symbol: ?SetAutohideZOrder@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
// STUB: The ANSI twin's retail entry RVA 0x49640 (mfc140) walks the manager control-bar
// list from its tail, filters pane/auto-hide-bar runtime classes, compares alignment
// and divider relationships, then dispatches virtual positioning operations. The local
// side table does not preserve the required complete membership/order and relationships.
extern "C" void MS_ABI impl__SetAutohideZOrder_CDockingManager__QEAAXPEAVCDockablePane___Z(
    CDockingManager*, CDockablePane*) {}

// Symbol: ?SetDockingMode@CDockingManager@@SAXW4AFX_DOCK_TYPE@@W4AFX_SMARTDOCK_THEME@@@Z
// Retail ANSI-twin entry RVA 0x4da50 (mfc140; --u does not resolve this name): store
// the mode; only mode 0x80 becomes 0x81 and stores the theme. The theme is stored in
// DockingSmartTheme() from detail/DockingState.h, the shared local shadow for m_SDTheme.
extern "C" void MS_ABI impl__SetDockingMode_CDockingManager__SAXW4AFX_DOCK_TYPE__W4AFX_SMARTDOCK_THEME___Z(
    int dockingMode, int theme) {
    impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A =
        static_cast<std::uint32_t>(dockingMode);
    if (dockingMode == 0x80) {
        impl__m_dockModeGlobal_CDockingManager__1W4AFX_DOCK_TYPE__A = 0x81;
        openmfc::docking_state::DockingSmartTheme() = theme;
    }
}

// Symbol: ?SetPrintPreviewMode@CDockingManager@@QEAAXHPEAUCPrintPreviewState@@@Z
// STUB: The ANSI twin's retail entry RVA 0x4ced0 (mfc140) toggles +0x208 and, when
// parent/OLE conditions permit, hides/restores eligible entries through manager lists.
// printPreviewMode and printPreviewHiddenBars are modeled, but the second manager list
// and parent-frame OLE-notify-hook state needed to reproduce the path are not.
extern "C" void MS_ABI impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(
    CDockingManager*, int, void*) {}

// Symbol: ?SetSmartDockingParams@CDockingManager@@SAXAEAVCSmartDockingInfo@@@Z
// Retail ANSI-twin entry RVA 0x4da70 (mfc140; --u does not resolve this name): copy
// the object fields only when all five marker IDs are zero or all are nonzero, then
// set m_bSDParamsModified. CopyTo at ANSI-twin entry RVA 0x4ebd0 copies the 84 bytes
// after the vfptr through m_bIsAlphaMarkers at offset 0x58, leaving tail padding.
extern "C" void MS_ABI impl__SetSmartDockingParams_CDockingManager__SAXAEAVCSmartDockingInfo___Z(
    void* pParams) {
    const auto* source = static_cast<const S_Csmartdockinginfo*>(pParams);
    unsigned int marked = 0;
    for (UINT id : source->m_uiMarkerBmpResID) {
        if (id != 0) ++marked;
    }
    if (marked != 0 && marked != AFX_SD_MARKERS_NUM) return;

    auto* destination = &impl__m_SDParams_CDockingManager__1VCSmartDockingInfo__A;
    constexpr std::size_t copiedBytes =
        offsetof(S_Csmartdockinginfo, m_bIsAlphaMarkers) + sizeof(BOOL) - sizeof(void*);
    if (source != destination) {
        std::memcpy(reinterpret_cast<unsigned char*>(destination) + sizeof(void*),
                    reinterpret_cast<const unsigned char*>(source) + sizeof(void*),
                    copiedBytes);
    }
    impl__m_bSDParamsModified_CDockingManager__1HA = TRUE;
}

// Symbol: ?ShowDelayShowMiniFrames@CDockingManager@@UEAAXH@Z
// The ANSI twin's retail entry RVA 0x4d6c0 (mfc140) scans mini-frames, tests
// m_bDelayShow (+0x108), calls USER32 IsWindow and (when valid) ShowWindow, then
// clears the flag. The flag is modeled in PaneFrameState, but its only accessor is a
// C++ helper in another TU; this file cannot call it without violating the thunk rule.
extern "C" void MS_ABI impl__ShowDelayShowMiniFrames_CDockingManager__UEAAXH_Z(
    CDockingManager*, int) {}

// Symbol: ?ShowPanes@CDockingManager@@UEAAHH@Z
// mfc140 0x4d490 saves the visible pane HWNDs so intentionally hidden panes stay hidden.
extern "C" int MS_ABI impl__ShowPanes_CDockingManager__UEAAHH_Z(CDockingManager* manager, int show) {
    if (!manager) return FALSE;
    auto& state = EnsureDockingState(manager);
    if (state.showPanesMode == !show) return FALSE;
    state.showPanesMode = !show;
    if (show) {
        auto hidden = std::move(state.panesHiddenForInPlace);
        for (HWND window : hidden) {
            auto* pane = static_cast<CBasePane*>(CWnd::FromHandlePermanent(window));
            if (::IsWindow(window) && ContainsPtr(state.panes, pane) && IsPaneVisibleForDocking(state, pane))
                ::ShowWindow(window, SW_SHOWNOACTIVATE);
        }
    } else {
        for (auto* pane : state.panes) {
            if (!IsPaneVisibleForDocking(state, pane) ||
                (!state.inPlaceBounds && !(pane->m_dwControlBarStyle & CBRS_HIDE_INPLACE))) continue;
            const HWND window = pane->m_hWnd;
            if (window && (::GetWindowLongW(window, GWL_STYLE) & WS_VISIBLE) &&
                !ContainsPtr(state.panesHiddenForInPlace, window)) state.panesHiddenForInPlace.push_back(window);
        }
        const auto hidden = state.panesHiddenForInPlace;
        for (HWND window : hidden) ::ShowWindow(window, SW_HIDE);
    }
    manager->RecalcLayout();
    return FALSE;
}
