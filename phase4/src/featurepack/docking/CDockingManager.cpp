// CDockingManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"
#include "detail/PaneContainerLayout.h"
#include "detail/MultiPaneFrameSupport.h"
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
extern "C" void MS_ABI impl__CalcPaneOffset_CDockingManager__IEAAXPEAVCAutoHideDockSite___Z(
    CDockingManager*, CAutoHideDockSite*);

extern "C" void* MS_ABI impl__FindPaneByID_CDockSite__QEAAPEAVCPane__I_Z(void*, unsigned int);
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd*);
extern "C" BOOL MS_ABI impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
    const void*, long long, CBasePane*, int, const CDockingManager*, BOOL, DWORD*, DWORD, const RECT*);

extern "C" CString* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
    CString*, const wchar_t*, const wchar_t*);
extern "C" void MS_ABI impl__Serialize_CDockingManager__UEAAXAEAVCArchive___Z(CDockingManager*, CArchive&);
extern "C" void MS_ABI impl__Serialize_CMFCBaseTabCtrl__UEAAXAEAVCArchive___Z(CMFCBaseTabCtrl*, CArchive&);
extern "C" void MS_ABI impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(CBaseTabbedPane*, int);
extern "C" void MS_ABI impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(void*, int, CWnd*);
extern "C" void MS_ABI impl__Serialize_CPaneDivider__UEAAXAEAVCArchive___Z(void*, CArchive&);
extern "C" void MS_ABI impl__SetDockState_CPaneFrameWnd__UEAAXPEAVCDockingManager___Z(void*, void*);
extern "C" void MS_ABI impl__SetDockState_CMultiPaneFrameWnd__UEAAXPEAVCDockingManager___Z(void*, void*);
extern "C" void* impl__m_pSliderRTC_CPaneDivider__2PEAUCRuntimeClass__EA;
extern "C" void* impl__m_pParentWndForSerialize_CPaneFrameWnd__1PEAVCFrameWnd__EA;
extern "C" int impl__m_bRestoringDockState_CDockingManager__2HA;
extern "C" int impl__m_bSavingState_CDockingManager__2HA;
extern "C" void* MS_ABI impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ(const CBasePane*);

namespace openmfc::detail::mfccore {
struct DockingProfileArchive {
    struct TabGroup {
        UINT id;
        std::vector<RestoredTab> tabs;
        int active;
    };
    struct AutoHidePane {
        UINT id;
        DWORD alignment;
        BOOL visible;
        RECT bounds;
        BOOL first;
        BOOL last;
        BOOL active;
    };
    std::vector<TabGroup> tabs;
    std::vector<std::unique_ptr<CPaneDivider>> edges;
    std::vector<std::unique_ptr<CPaneFrameWnd>> floating;
    std::vector<AutoHidePane> autoHide;
    std::vector<UINT> hiddenMdi;
    std::unordered_map<UINT, BOOL> visibility;
};
}

namespace {
constexpr wchar_t dockingProfileValue[] = L"DockingPaneAndPaneDividers";
constexpr DWORD maxDockingProfileBytes = 64 * 1024 * 1024;
constexpr DWORD visibilityRecord = 0x56464d4f; // OMFV

template<class T> class ScopedValue {
public:
    ScopedValue(T& value, T temporary) : slot(value), previous(std::exchange(value, temporary)) {}
    ~ScopedValue() { slot = previous; }
private:
    T& slot;
    T previous;
};

CString DockingProfilePath(const wchar_t* profile, UINT frameId) {
    alignas(CString) unsigned char storage[sizeof(CString)];
    auto* root = reinterpret_cast<CString*>(storage);
    impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
        root, L"DockingManagers", profile);
    CString path(std::move(*root));
    root->~CString();
    CString section;
    section.Format(L"%sDockingManager-%d", path.GetString(), static_cast<int>(frameId));
    return section;
}

void ApplyDockingLayout(CDockingManager& manager, DockingProfileArchive& restored);

int ReadLayoutCount(CArchive& archive) {
    int count = 0;
    archive >> count;
    if (count < 0 || count > 65535) AfxThrowArchiveException(CArchiveException::badIndex, nullptr);
    return count;
}
}

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
        if (!entry.autoHide) continue;
        auto* site = static_cast<CAutoHideDockSite*>(entry.site);
        site->m_nOffsetLeft = site->m_nOffsetRight = 0;
    }
    for (const auto& entry : state.dockSites) {
        if (entry.autoHide)
            impl__CalcPaneOffset_CDockingManager__IEAAXPEAVCAutoHideDockSite___Z(
                &manager, static_cast<CAutoHideDockSite*>(entry.site));
    }
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
    if (window->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
        window = static_cast<CPaneFrameWnd*>(window)->GetFirstVisiblePane();
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

bool openmfc::detail::mfccore::PaneFrameCaptionContains(const CPaneFrameWnd& frame, CPoint point) {
    RECT bounds{};
    POINT client{};
    return ::GetWindowRect(frame.m_hWnd, &bounds) && ::ClientToScreen(frame.m_hWnd, &client) &&
        ::PtInRect(&bounds, point) && point.y < client.y;
}

DockingDrop openmfc::detail::mfccore::DockingTarget(const CDockingManager* manager, CPoint point, int sensitivity,
    DWORD allowedAlignment, const CBasePane* ignore, const CBasePane* source) {
    if (!manager || (source && !source->CanBeDocked())) return {};
    const int tolerance = sensitivity >= 0 ? (std::min)(sensitivity, 32767) : 0;
    const auto* ignoredFrame = ParentPaneFrame(ignore ? ignore : source);
    auto* frame = static_cast<CPaneFrameWnd*>(
        impl__FrameFromPoint_CDockingManager__UEBAPEAVCPaneFrameWnd__VCPoint__PEAV2_H_Z(manager, point, ignoredFrame, FALSE));
    if (frame && PaneFrameCaptionContains(*frame, point)) {
        auto* window = frame->GetFirstVisiblePane();
        if (window && window != ignore && window != source &&
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

DockingDrop openmfc::detail::mfccore::PreDockTarget(const CDockingManager* manager, CPoint point,
    DWORD allowedAlignment, CDockablePane* target, int request) {
    if (!manager || (target && (!::IsWindow(target->m_hWnd) || target->IsAutoHideMode()))) return {};
    if (request == PreDockToTab)
        return target && target->CanBeAttached() ? DockingDrop{target, 0, DelayDockToTab} : DockingDrop{};
    if (request != PreDockRegular) return {};

    DWORD side = 0;
    const auto packed = PackedDockPoint(point);
    if (target) {
        if (impl__CheckAlignment_CGlobalUtils__QEBAHVCPoint__PEAVCBasePane__HPEBVCDockingManager__HAEAKKPEBUtagRECT___Z(
                nullptr, packed, target, impl__m_nDockSensitivity_CDockingManager__2HA, manager, FALSE,
                &side, allowedAlignment, nullptr)) return {target, side, DelayDock};
    } else {
        BOOL outer = FALSE;
        if (impl__IsPointNearDockSite_CDockingManager__QEBAHVCPoint__AEAKAEAH_Z(manager, point, &side, &outer) &&
            (allowedAlignment & side)) return {nullptr, side, DelayDock, outer != FALSE};
    }
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
namespace {
CBasePane* FindTabbedPaneById(CBasePane* pane, UINT id) {
    if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) return nullptr;
    auto* tabs = static_cast<CBaseTabbedPane*>(pane)->GetTabs();
    if (!tabs) return nullptr;
    for (int index = 0; index < tabs->GetTabsCount(); ++index) {
        auto* window = tabs->GetTabWnd(index);
        if (!window || !window->IsKindOf(RUNTIME_CLASS(CBasePane))) continue;
        auto* page = static_cast<CBasePane*>(window);
        if (PaneCommandID(page) == id) return page;
        if (auto* nested = FindTabbedPaneById(page, id)) return nested;
    }
    return nullptr;
}
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

    for (auto* pane : state->panes) {
        if (!bSearchMiniFrames && (state->hiddenPanes.count(pane) || state->floatingPanes.count(pane))) continue;
        if (auto* nested = FindTabbedPaneById(pane, nID)) return nested;
    }
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
extern "C" int MS_ABI impl__LoadState_CDockingManager__UEAAHPEB_WI_Z(
    CDockingManager* manager, const wchar_t* profile, UINT frameId) {
    if (!manager) return FALSE;
    const CString section = DockingProfilePath(profile, frameId);
    DWORD size = 0;
    if (::RegGetValueW(HKEY_CURRENT_USER, section, dockingProfileValue, RRF_RT_REG_BINARY,
            nullptr, nullptr, &size) != ERROR_SUCCESS || !size || size > maxDockingProfileBytes) return FALSE;
    std::vector<BYTE> bytes(size);
    if (::RegGetValueW(HKEY_CURRENT_USER, section, dockingProfileValue, RRF_RT_REG_BINARY,
            nullptr, bytes.data(), &size) != ERROR_SUCCESS || !size) return FALSE;

    auto& state = EnsureDockingState(manager);
    auto previous = state.pendingLayout;
    try {
        CMemFile file(bytes.data(), size);
        CArchive archive(&file, CArchive::load | CArchive::bNoFlushOnDelete);
        impl__Serialize_CDockingManager__UEAAXAEAVCArchive___Z(manager, archive);
        DWORD tag = 0;
        if (archive.Read(&tag, sizeof(tag)) == sizeof(tag) && tag == visibilityRecord) {
            auto& visibility = state.pendingLayout->visibility;
            for (int count = ReadLayoutCount(archive); count > 0; --count) {
                UINT id = 0;
                BOOL visible = FALSE;
                archive >> id >> visible;
                visibility[id] = visible;
            }
        }
        archive.Close();
        return TRUE;
    } catch (CException* error) {
        state.pendingLayout = std::move(previous);
        error->Delete();
        return FALSE;
    }
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
    if (frame && frame->IsKindOf(CMultiPaneFrameWnd::GetThisClass())) {
        const auto* multi = static_cast<const CMultiPaneFrameWnd*>(frame);
        for (auto* pane : openmfc::detail::ContainerPanes(multi->GetContainerManager())) {
            if (matches(pane, 0)) return pane;
        }
    } else if (frame) {
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
extern "C" int MS_ABI impl__SaveState_CDockingManager__UEAAHPEB_WI_Z(
    CDockingManager* manager, const wchar_t* profile, UINT frameId) {
    if (!manager) return FALSE;
    ScopedValue saving(impl__m_bSavingState_CDockingManager__2HA, TRUE);
    try {
        CMemFile file;
        CArchive archive(&file, CArchive::store | CArchive::bNoFlushOnDelete);
        impl__Serialize_CDockingManager__UEAAXAEAVCArchive___Z(manager, archive);
        const auto& state = EnsureDockingState(manager);
        std::unordered_map<UINT, BOOL> visibility;
        for (auto* pane : state.panes) {
            if (!pane || !pane->m_hWnd) continue;
            const UINT id = PaneCommandID(pane);
            if (id && id != UINT(-1)) visibility[id] = !state.hiddenPanes.count(pane) &&
                (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE) != 0;
        }
        // Native readers stop at the end of the layout. Plain panes use this
        // optional trailer to restore their visibility across processes.
        archive << visibilityRecord << static_cast<int>(visibility.size());
        for (const auto& [id, visible] : visibility) archive << id << visible;
        archive.Close();
        const auto size = file.GetLength();
        if (!size || size > maxDockingProfileBytes) return FALSE;
        std::unique_ptr<BYTE, decltype(&std::free)> bytes(file.Detach(), &std::free);
        const CString section = DockingProfilePath(profile, frameId);
        HKEY key = nullptr;
        if (::RegCreateKeyExW(HKEY_CURRENT_USER, section, 0, nullptr, 0, KEY_SET_VALUE,
                nullptr, &key, nullptr) != ERROR_SUCCESS) return FALSE;
        const auto status = ::RegSetValueExW(key, dockingProfileValue, 0, REG_BINARY,
            bytes.get(), static_cast<DWORD>(size));
        ::RegCloseKey(key);
        return status == ERROR_SUCCESS;
    } catch (CException* error) {
        error->Delete();
        return FALSE;
    }
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
        state.pendingLayout.reset();
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
        state.hiddenMDITabbedBars = nullptr;
        state.hiddenMDITabbedBarCount = state.hiddenMDITabbedBarCapacity = 0;
    }
    RemoveDockingState(this);
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
CPaneDivider* DockedPaneDivider(const CBasePane* pane) {
    if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CDockablePane))) return nullptr;
    auto* divider = static_cast<CPaneDivider*>(
        impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(static_cast<const CDockablePane*>(pane)));
    return divider && !divider->m_dividerData.bAutoHideMode ? divider : nullptr;
}

void ShowDockingWindows(DockingManagerState& state, CBasePane* pane, int command) {
    if (!pane->m_hWnd) return;
    if (state.floatingPanes.count(pane)) {
        ::ShowWindow(pane->m_hWnd, command);
        const HWND parent = ::GetParent(pane->m_hWnd);
        auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(parent));
        if (window && window->IsKindOf(CMultiPaneFrameWnd::GetThisClass())) {
            auto* multi = static_cast<CMultiPaneFrameWnd*>(window);
            ::ShowWindow(parent, multi->GetFirstVisiblePane() ? SW_SHOWNOACTIVATE : SW_HIDE);
            multi->AdjustLayout();
        } else {
            ::ShowWindow(parent, command);
        }
    } else if (auto* divider = DockedPaneDivider(pane)) {
        ::ShowWindow(pane->m_hWnd, command);
        using namespace openmfc::detail;
        const bool visible = DividerHasVisiblePanes(divider);
        ::ShowWindow(divider->m_hWnd, visible ? SW_SHOWNOACTIVATE : SW_HIDE);
        RefreshDividerPanes(divider);
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
constexpr UINT frameContentId = 0xe900; // AFX_IDW_PANE_FIRST
constexpr LONG frameClientMargin = 4;

struct EdgePlacement {
    HWND divider;
    RECT bounds;
    RECT sash;
};

bool IsVerticalEdge(DWORD side) {
    return side == CBRS_ALIGN_LEFT || side == CBRS_ALIGN_RIGHT;
}

LONG EdgeSpan(const RECT& rect, bool vertical) {
    return (std::max)(0L, vertical ? rect.right - rect.left : rect.bottom - rect.top);
}

std::vector<EdgePlacement> PlaceEdgePanes(const DockingManagerState& state, RECT& remaining) {
    std::vector<EdgePlacement> positions;
    if (state.edgePanes.empty() || !state.parentFrame) return positions;
    const HWND host = state.parentFrame->m_hWnd;
    for (bool outer : {true, false}) {
        for (const auto& dock : state.edgePanes) {
            if (dock.outer != outer || ::GetParent(dock.divider) != host) continue;
            auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(dock.divider));
            if (!window || !window->IsKindOf(RUNTIME_CLASS(CPaneDivider))) continue;
            auto* divider = static_cast<CPaneDivider*>(window);
            if (!openmfc::detail::DividerHasVisiblePanes(divider)) continue;

            const DWORD side = divider->GetPaneAlignment();
            const LONG span = EdgeSpan(remaining, IsVerticalEdge(side));
            const LONG sashSize = (std::clamp)(static_cast<LONG>(divider->m_dividerData.nWidth), 0L, span);
            const LONG thickness = (std::clamp)(dock.thickness, 0L, span - sashSize);
            RECT bounds = remaining, sash = remaining;
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
            positions.push_back({dock.divider, bounds, sash});
        }
    }
    return positions;
}

void ArrangeEdgePanes(const DockingManagerState& state, RECT& remaining, bool moveWindows = true) {
    auto positions = PlaceEdgePanes(state, remaining);
    if (!moveWindows) return;
    for (const auto& position : positions) {
        const RECT& rect = position.sash;
        ::SetWindowPos(position.divider, nullptr, rect.left, rect.top,
            EdgeSpan(rect, true), EdgeSpan(rect, false), SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
        auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(position.divider));
        if (window && window->IsKindOf(RUNTIME_CLASS(CPaneDivider)))
            openmfc::detail::ResizeDividerPanes(static_cast<CPaneDivider*>(window), position.bounds);
    }
}

RECT DockedFrameBounds(const DockingManagerState& state, const RECT& frameBounds) {
    RECT bounds = frameBounds;
    if (state.fitFrameClient) {
        state.parentFrame->RepositionBars(0, 0xffff, frameContentId, CWnd::reposQuery, &bounds, &frameBounds);
        const RECT& border = state.parentFrame->m_retailMiddle.m_rectBorder;
        bounds.left += border.left;
        bounds.top += border.top;
        bounds.right = (std::max)(bounds.left, bounds.right - border.right);
        bounds.bottom = (std::max)(bounds.top, bounds.bottom - border.bottom);
    }
    return bounds;
}

struct FrameEdgeResize {
    CDockingManager* manager = nullptr;
    DockingManagerState::EdgePane* edge = nullptr;
    LONG current = 0;
    LONG minDelta = 0;
    LONG maxDelta = 0;
    int growthDirection = 1; // Screen-independent client coordinates.
};

FrameEdgeResize GetFrameEdgeResize(const CPaneDivider& divider, LONG thickness, LONG minThickness) {
    if (!divider.m_hWnd || divider.m_dividerData.bAutoHideMode) return {};
    auto* frame = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(divider.m_hWnd)));
    auto* manager = DockingManagerForFrame(frame);
    if (!FindDockingState(manager)) return {};
    auto& state = EnsureDockingState(manager);
    const auto edge = std::find_if(state.edgePanes.begin(), state.edgePanes.end(), [&](const auto& slot) {
        return slot.divider == divider.m_hWnd;
    });
    if (edge == state.edgePanes.end()) return {};
    FrameEdgeResize resize{manager, &*edge};
    if (state.lockUpdate || !openmfc::detail::DividerHasVisiblePanes(&divider)) return resize;
    const DWORD side = divider.GetPaneAlignment();
    const bool vertical = IsVerticalEdge(side);
    resize.current = (std::max)(0L, thickness);
    // If the parent is already smaller than the requested minimum, keep the
    // current thickness reachable and allow growing again as space becomes available.
    const LONG minimum = (std::max)(1L, minThickness);
    resize.minDelta = (std::min)(0L, minimum - resize.current);
    resize.maxDelta = (std::max)(0L, EdgeSpan(state.clientAreaBounds, vertical) - frameClientMargin);
    if (side == CBRS_ALIGN_RIGHT || side == CBRS_ALIGN_BOTTOM) resize.growthDirection = -1;
    return resize;
}
}

void openmfc::detail::mfccore::RegisterFrameEdgeDivider(
    CDockingManager& manager, CPaneDivider& divider, LONG thickness, bool outer, const CBasePane* relative) {
    auto& edges = EnsureDockingState(&manager).edgePanes;
    auto* neighbor = DockedPaneDivider(relative);
    auto position = neighbor ? std::find_if(edges.begin(), edges.end(), [&](const auto& edge) {
        return edge.divider == neighbor->m_hWnd;
    }) : edges.end();
    edges.insert(position, {divider.m_hWnd, thickness, outer});
}

void openmfc::detail::mfccore::ForgetFrameEdgeDivider(const CPaneDivider& divider) {
    auto* frame = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(divider.m_hWnd)));
    auto* manager = DockingManagerForFrame(frame);
    if (!FindDockingState(manager)) return;
    auto& edges = EnsureDockingState(manager).edgePanes;
    edges.erase(std::remove_if(edges.begin(), edges.end(), [&](const auto& edge) {
        return edge.divider == divider.m_hWnd;
    }), edges.end());
}

bool openmfc::detail::mfccore::FrameEdgeDividerOffsets(
    const CPaneDivider& divider, LONG thickness, LONG minThickness, int& low, int& high) {
    const auto resize = GetFrameEdgeResize(divider, thickness, minThickness);
    if (!resize.edge) return false;
    low = resize.growthDirection > 0 ? resize.minDelta : -resize.maxDelta;
    high = resize.growthDirection > 0 ? resize.maxDelta : -resize.minDelta;
    return true;
}

bool openmfc::detail::mfccore::MoveFrameEdgeDivider(
    CPaneDivider& divider, LONG thickness, LONG minThickness, int offset) {
    const auto resize = GetFrameEdgeResize(divider, thickness, minThickness);
    if (!resize.edge) return false;
    const LONG delta = static_cast<LONG>((std::clamp)(static_cast<long long>(offset) * resize.growthDirection,
        static_cast<long long>(resize.minDelta), static_cast<long long>(resize.maxDelta)));
    if (delta) {
        resize.edge->thickness = resize.current + delta;
        resize.manager->RecalcLayout();
    }
    return true;
}

void openmfc::detail::mfccore::AdjustDockedFrameClient(const CDockingManager* manager) {
    const auto* state = FindDockingState(manager);
    if (!state || state->lockUpdate || state->inPlaceBounds || !state->fitFrameClient ||
        !state->parentFrame) return;
    auto* frame = state->parentFrame;
    const HWND host = frame->m_hWnd;
    HWND content = ::GetDlgItem(host, frameContentId);
    if (frame->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx))) {
        const HWND mdi = static_cast<CMDIFrameWnd*>(frame)->m_hWndMDIClient;
        if (::IsWindow(mdi)) content = mdi;
    }
    if (!content || ::GetParent(content) != host) return;
    RECT bounds = state->clientAreaBounds;
    if (auto* window = static_cast<CWnd*>(CWnd::FromHandlePermanent(content)))
        window->CalcWindowRect(&bounds, 0);
    const LONG style = ::GetWindowLongW(content, GWL_STYLE);
    if (!(style & WS_CLIPSIBLINGS))
        ::SetWindowLongW(content, GWL_STYLE, style | WS_CLIPSIBLINGS);
    ::SetWindowPos(content, HWND_BOTTOM, bounds.left, bounds.top, EdgeSpan(bounds, true),
        EdgeSpan(bounds, false), SWP_NOACTIVATE);
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
        state.fitFrameClient = state.fitFrameClient || !state.edgePanes.empty();
        client = DockedFrameBounds(state, frameBounds);
        ArrangeEdgePanes(state, client);
        state.clientAreaBounds = client;
        LayoutDockSites(*this, state, frameBounds);
        AdjustDockedFrameClient(this);
    }
    const auto canTile = [&state](CBasePane* pane) {
        if (!IsPaneVisibleForDocking(state, pane) || state.floatingPanes.count(pane) ||
            DockedPaneDivider(pane)) return false;
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
        ArrangeEdgePanes(state, remaining, moveWindows);
        for (auto* pane : state.panes) {
            if (!IsPaneVisibleForDocking(state, pane) || state.floatingPanes.count(pane) ||
                DockedPaneDivider(pane)) continue;
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
    if (auto restored = std::exchange(state.pendingLayout, {})) {
        ApplyDockingLayout(*this, *restored);
        RecalcLayout();
        return;
    }
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
// mfc140u 0x4a100. Each corner is visited from one of its two sides.
extern "C" void MS_ABI impl__CalcPaneOffset_CDockingManager__IEAAXPEAVCAutoHideDockSite___Z(
    CDockingManager* manager, CAutoHideDockSite* site) {
    const auto* state = FindDockingState(manager);
    RECT bounds{};
    if (!state || !site || !::GetWindowRect(site->m_hWnd, &bounds)) return;
    const DWORD side = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(site);
    const LONG width = bounds.right - bounds.left;
    const LONG height = bounds.bottom - bounds.top;
    for (const auto& entry : state->dockSites) {
        if (!entry.autoHide || entry.site == site) continue;
        auto* adjacent = static_cast<CAutoHideDockSite*>(entry.site);
        const DWORD adjacentSide = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(adjacent);
        if (((side & CBRS_ORIENT_HORZ) != 0) == ((adjacentSide & CBRS_ORIENT_HORZ) != 0)) continue;
        RECT rect{};
        if (!::GetWindowRect(adjacent->m_hWnd, &rect) || ::IsRectEmpty(&rect)) continue;
        if ((side & CBRS_ALIGN_LEFT) && (adjacentSide & CBRS_ALIGN_TOP)) {
            if (bounds.top == rect.bottom) adjacent->m_nOffsetLeft = width;
            if (bounds.right == rect.left) site->m_nOffsetLeft = rect.bottom - rect.top;
        } else if ((side & CBRS_ALIGN_TOP) && (adjacentSide & CBRS_ALIGN_RIGHT)) {
            if (bounds.right == rect.left) adjacent->m_nOffsetLeft = height;
            if (bounds.bottom == rect.top) site->m_nOffsetRight = rect.right - rect.left;
        } else if ((side & CBRS_ALIGN_RIGHT) && (adjacentSide & CBRS_ALIGN_BOTTOM)) {
            if (bounds.bottom == rect.top) adjacent->m_nOffsetRight = width;
            if (bounds.left == rect.right) site->m_nOffsetRight = width;
        } else if ((side & CBRS_ALIGN_LEFT) && (adjacentSide & CBRS_ALIGN_BOTTOM)) {
            if (bounds.bottom == rect.top) adjacent->m_nOffsetLeft = width;
            if (bounds.right == rect.left) site->m_nOffsetRight = rect.bottom - rect.top;
        }
    }
}

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

namespace {
extern "C" int MS_ABI impl__IsVisible_CBasePane__UEBAHXZ(const CBasePane*);
extern "C" void MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane*, int, int, int);
extern "C" void MS_ABI impl__ShowPane_CDockablePane__UEAAXHHH_Z(CDockablePane*, int, int, int);

void SetPreviewPaneVisibility(CBasePane* pane, BOOL visible) {
    if (pane->IsKindOf(RUNTIME_CLASS(CDockablePane))) {
        auto* dockable = static_cast<CDockablePane*>(pane);
        impl__ShowPane_CDockablePane__UEAAXHHH_Z(dockable, visible, TRUE, FALSE);
        if (dockable->IsAutoHideMode()) {
            auto* divider = static_cast<CPaneDivider*>(
                impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(dockable));
            if (divider) ::ShowWindow(divider->m_hWnd, visible ? SW_SHOWNOACTIVATE : SW_HIDE);
        }
    } else {
        impl__ShowPane_CBasePane__UEAAXHHH_Z(pane, visible, TRUE, FALSE);
    }
}
}

// Symbol: ?HideForPrintPreview@CDockingManager@@IEAAXAEBVCObList@@@Z
// mfc140u 0x4cfb0. Remember each visible eligible pane and each visible mini-frame.
extern "C" void MS_ABI impl__HideForPrintPreview_CDockingManager__IEAAXAEBVCObList___Z(
    CDockingManager* manager, const CObList* panes) {
    if (!manager || !panes || panes->IsEmpty()) return;
    auto& state = EnsureDockingState(manager);
    auto& hidden = state.printPreviewHiddenBars;
    std::vector<HWND> candidates;
    for (auto position = panes->GetHeadPosition(); position != nullptr;) {
        auto* object = panes->GetNext(position);
        if (object && object->IsKindOf(RUNTIME_CLASS(CBasePane)))
            candidates.push_back(static_cast<CBasePane*>(object)->m_hWnd);
    }
    for (HWND window : candidates) {
        auto* object = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
        if (!object || !object->IsKindOf(RUNTIME_CLASS(CBasePane)) ||
            object->IsKindOf(RUNTIME_CLASS(CMFCRibbonBar))) continue;
        auto* pane = static_cast<CBasePane*>(object);
        if (state.floatingPanes.count(pane) || !impl__IsVisible_CBasePane__UEBAHXZ(pane) ||
            std::find(hidden.begin(), hidden.end(), window) != hidden.end()) continue;
        hidden.push_back(window);
        SetPreviewPaneVisibility(pane, FALSE);
    }
    std::vector<HWND> frames;
    for (void* value : state.miniFrames) {
        if (auto* frame = static_cast<CWnd*>(value); frame && ::IsWindowVisible(frame->m_hWnd))
            frames.push_back(frame->m_hWnd);
    }
    for (HWND window : frames) {
        if (!::IsWindowVisible(window)) continue;
        hidden.push_back(window);
        ::ShowWindow(window, SW_HIDE);
    }
}

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

namespace {
CDockablePane* LayoutPane(CDockingManager& manager, UINT id) {
    if (!id || id == UINT(-1)) return nullptr;
    auto* pane = impl__FindPaneByID_CDockingManager__UEAAPEAVCBasePane__IH_Z(&manager, id, TRUE);
    return pane && pane->IsKindOf(RUNTIME_CLASS(CDockablePane)) ? static_cast<CDockablePane*>(pane) : nullptr;
}

std::vector<DockingProfileArchive::AutoHidePane> AutoHideRecords(const DockingManagerState& state) {
    std::vector<DockingProfileArchive::AutoHidePane> records;
    for (const auto& bar : state.autoHideBars) {
        for (auto pos = bar->m_lstAutoHideButtons.GetHeadPosition(); pos != nullptr;) {
            const auto* button = static_cast<const CMFCAutoHideButton*>(bar->m_lstAutoHideButtons.GetNext(pos));
            const auto* pane = button->m_pAutoHideWindow;
            if (!pane || !pane->m_hWnd || !pane->IsAutoHideMode()) continue;
            RECT bounds{};
            ::GetWindowRect(pane->m_hWnd, &bounds);
            if (::IsRectEmpty(&bounds)) bounds = *static_cast<const RECT*>(pane->m_rectSavedDockedRect);
            records.push_back({PaneCommandID(pane), bar->GetPaneAlignment(), ::IsWindowVisible(pane->m_hWnd),
                bounds, bar->m_bFirstInGroup, bar->m_bLastInGroup, bar->m_bActiveInGroup});
        }
    }
    return records;
}

int TabbedPaneReference(CDockablePane& pane) {
    if (!pane.IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) return 0;
    const auto* tabs = FindTabCtrlState(static_cast<CBaseTabbedPane&>(pane).GetTabs());
    if (!tabs) return 0;
    for (const auto& tab : tabs->tabs) {
        if (!tab.window || !tab.window->m_hWnd || tab.temporary) continue;
        const int id = ::GetDlgCtrlID(tab.window->m_hWnd);
        if (id && id != -1) return id;
    }
    return 0;
}

void StoreDockingLayout(const DockingManagerState& state, CArchive& archive) {
    std::vector<std::pair<UINT, CMFCBaseTabCtrl*>> tabGroups;
    for (auto* pane : state.panes) {
        if (!pane || !pane->m_hWnd || !pane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)) ||
            state.floatingPanes.count(pane) || DockedPaneDivider(pane)) continue;
        const UINT id = PaneCommandID(pane);
        auto* tabs = static_cast<CBaseTabbedPane*>(pane)->GetTabs();
        if (id && id != UINT(-1) && tabs) tabGroups.emplace_back(id, tabs);
    }
    archive << static_cast<int>(tabGroups.size());
    for (const auto& [id, tabs] : tabGroups) {
        archive << id;
        impl__Serialize_CMFCBaseTabCtrl__UEAAXAEAVCArchive___Z(tabs, archive);
    }

    struct EdgeRecord {
        CPaneDivider* divider;
        std::vector<std::pair<int, int>> members;
    };
    std::vector<EdgeRecord> edges;
    int count = 0;
    for (bool outer : {true, false}) {
        for (const auto& edge : state.edgePanes) {
            if (edge.outer != outer) continue;
            auto* divider = static_cast<CPaneDivider*>(CWnd::FromHandlePermanent(edge.divider));
            if (!divider || !divider->m_dividerData.bDefaultDivider || divider->m_dividerData.bAutoHideMode) continue;
            EdgeRecord record{divider, {}};
            for (auto* pane : openmfc::detail::ContainerPanes(divider->m_dividerData.pContainerManager)) {
                if (!pane || !pane->m_hWnd) continue;
                const int id = static_cast<int>(PaneCommandID(pane));
                const int sibling = id == -1 ? TabbedPaneReference(*pane) : 0;
                if (id && (id != -1 || sibling)) record.members.emplace_back(id, sibling);
            }
            if (record.members.empty()) continue;
            count += static_cast<int>(record.members.size()) + 1;
            edges.push_back(std::move(record));
        }
    }
    // Retail walks the control-bar order backwards and inserts at the head.
    archive << count;
    for (auto edge = edges.rbegin(); edge != edges.rend(); ++edge) {
        archive << 0;
        impl__Serialize_CPaneDivider__UEAAXAEAVCArchive___Z(edge->divider, archive);
        for (auto member = edge->members.rbegin(); member != edge->members.rend(); ++member) {
            archive << 1 << member->first;
            if (member->first == -1) archive << member->second;
        }
    }

    std::vector<CPaneFrameWnd*> floating;
    for (void* value : state.miniFrames) {
        auto* frame = static_cast<CPaneFrameWnd*>(value);
        if (frame && frame->m_hWnd && frame->GetPane()) floating.push_back(frame);
    }
    archive << static_cast<int>(floating.size());
    for (auto* frame : floating) archive.WriteObject(frame);

    const auto autoHide = AutoHideRecords(state);
    archive << static_cast<int>(autoHide.size());
    for (const auto& record : autoHide) {
        archive << record.id << record.alignment << record.visible;
        archive.Write(&record.bounds, sizeof(record.bounds));
        archive << record.first << record.last << record.active;
    }
    archive << static_cast<int>(state.hiddenMDITabbedBarCount);
    for (std::size_t i = 0; i < state.hiddenMDITabbedBarCount; ++i) {
        const HWND window = reinterpret_cast<HWND>(state.hiddenMDITabbedBars[i]);
        archive << (::IsWindow(window) ? ::GetDlgCtrlID(window) : -1);
    }
}

void LoadDockingLayout(CDockingManager& manager, CArchive& archive) {
    auto& state = EnsureDockingState(&manager);
    auto* owner = state.parentFrame;
    if (!owner || !owner->m_hWnd) AfxThrowArchiveException(CArchiveException::generic, nullptr);
    auto draft = std::make_shared<DockingProfileArchive>();
    for (int count = ReadLayoutCount(archive); count > 0; --count) {
        UINT id = 0;
        archive >> id;
        CMFCBaseTabCtrl tabs;
        impl__Serialize_CMFCBaseTabCtrl__UEAAXAEAVCArchive___Z(&tabs, archive);
        auto& loaded = EnsureTabCtrlState(&tabs);
        draft->tabs.push_back({id, std::move(*loaded.restoredTabs), loaded.restoredActiveTab});
    }
    for (int count = ReadLayoutCount(archive); count > 0; --count) {
        int kind = 0;
        archive >> kind;
        if (kind == 1) {
            // The native order also names the leaves. Loaded divider trees
            // already resolve these references through their membership list.
            int id = 0;
            archive >> id;
            if (id == -1) archive >> id;
            continue;
        }
        if (kind) AfxThrowArchiveException(CArchiveException::badFormat, nullptr);
        auto* type = static_cast<CRuntimeClass*>(impl__m_pSliderRTC_CPaneDivider__2PEAUCRuntimeClass__EA);
        if (!type) type = RUNTIME_CLASS(CPaneDivider);
        std::unique_ptr<CObject> object(type->CreateObject());
        if (!object || !object->IsKindOf(RUNTIME_CLASS(CPaneDivider)))
            AfxThrowArchiveException(CArchiveException::badClass, nullptr);
        std::unique_ptr<CPaneDivider> divider(static_cast<CPaneDivider*>(object.release()));
        impl__Init_CPaneDivider__QEAAXHPEAVCWnd___Z(divider.get(), TRUE, owner);
        impl__Serialize_CPaneDivider__UEAAXAEAVCArchive___Z(divider.get(), archive);
        if (!divider->m_hWnd || !divider->m_dividerData.bDefaultDivider)
            AfxThrowArchiveException(CArchiveException::badFormat, nullptr);
        divider->SetPaneAlignment(divider->m_dwStyle & CBRS_ALIGN_ANY);
        divider->m_dividerData.destroyWithWindow = false;
        ::ShowWindow(divider->m_hWnd, SW_HIDE);
        draft->edges.push_back(std::move(divider));
    }
    std::reverse(draft->edges.begin(), draft->edges.end());

    ScopedValue parent(impl__m_pParentWndForSerialize_CPaneFrameWnd__1PEAVCFrameWnd__EA, static_cast<void*>(owner));
    for (int count = ReadLayoutCount(archive); count > 0; --count) {
        auto* frame = static_cast<CPaneFrameWnd*>(archive.ReadObject(RUNTIME_CLASS(CPaneFrameWnd)));
        if (!frame || !frame->m_hWnd ||
            std::any_of(draft->floating.begin(), draft->floating.end(), [frame](const auto& item) { return item.get() == frame; }))
            AfxThrowArchiveException(CArchiveException::badClass, nullptr);
        std::unique_ptr<CPaneFrameWnd> item(frame);
        auto& frames = state.miniFrames;
        frames.erase(std::remove(frames.begin(), frames.end(), frame), frames.end());
        draft->floating.push_back(std::move(item));
    }
    for (int count = ReadLayoutCount(archive); count > 0; --count) {
        DockingProfileArchive::AutoHidePane record{};
        archive >> record.id >> record.alignment >> record.visible;
        archive.EnsureRead(&record.bounds, sizeof(record.bounds));
        archive >> record.first >> record.last >> record.active;
        draft->autoHide.push_back(record);
    }
    for (int count = ReadLayoutCount(archive); count > 0; --count) {
        UINT id = 0;
        archive >> id;
        draft->hiddenMdi.push_back(id);
    }
    state.pendingLayout = std::move(draft);
}

LONG MeasureStoredEdge(const CPaneDivider& divider, RECT& remaining, HWND owner) {
    RECT sash{};
    if (!::GetWindowRect(divider.m_hWnd, &sash)) return 0;
    ::MapWindowPoints(nullptr, owner, reinterpret_cast<POINT*>(&sash), 2);
    LONG thickness = 0;
    switch (divider.GetPaneAlignment()) {
    case CBRS_ALIGN_LEFT:
        thickness = sash.left - remaining.left;
        remaining.left = sash.right;
        break;
    case CBRS_ALIGN_RIGHT:
        thickness = remaining.right - sash.right;
        remaining.right = sash.left;
        break;
    case CBRS_ALIGN_TOP:
        thickness = sash.top - remaining.top;
        remaining.top = sash.bottom;
        break;
    case CBRS_ALIGN_BOTTOM:
        thickness = remaining.bottom - sash.bottom;
        remaining.bottom = sash.top;
        break;
    }
    return (std::max)(thickness, 1L);
}

bool RestoreDockedPane(CDockingManager& manager, CFrameWnd& owner, CPaneDivider& edge,
                       CDockablePane& pane, BOOL visible) {
    if (!pane.m_hWnd) return false;
    const HWND previous = ::GetParent(pane.m_hWnd);
    auto* group = static_cast<CBaseTabbedPane*>(impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ(&pane));
    if (previous != owner.m_hWnd) {
        ::SetLastError(ERROR_SUCCESS);
        if (!::SetParent(pane.m_hWnd, owner.m_hWnd) && ::GetLastError() != ERROR_SUCCESS) return false;
    }
    if (group) group->RemovePane(&pane);
    auto* parent = static_cast<CWnd*>(CWnd::FromHandlePermanent(previous));
    openmfc::detail::AdoptTransferredPane(pane, owner, parent, &edge);
    if (pane.IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) {
        auto* tabs = static_cast<CBaseTabbedPane*>(&pane);
        impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(tabs, TRUE);
        if (!tabs->GetTabs() || !tabs->GetTabs()->GetTabsCount()) {
            edge.RemovePane(tabs);
            tabs->DestroyWindow();
            return false;
        }
    }
    if (visible) manager.ShowPane(&pane); else manager.HidePane(&pane);
    return true;
}

void ApplyDockingLayout(CDockingManager& manager, DockingProfileArchive& restored) {
    auto& state = EnsureDockingState(&manager);
    auto* owner = state.parentFrame;
    if (!owner || !owner->m_hWnd) return;
    ScopedValue lockLayout(state.lockUpdate, true);
    ScopedValue restoring(impl__m_bRestoringDockState_CDockingManager__2HA, 0);
    for (const auto& record : AutoHideRecords(state)) {
        if (auto* pane = LayoutPane(manager, record.id); pane && pane->IsAutoHideMode())
            pane->SetAutoHideMode(FALSE, record.alignment, nullptr, FALSE);
    }
    for (auto* pane : state.panes) {
        if (!pane || !pane->m_hWnd) continue;
        const UINT id = PaneCommandID(pane);
        if (!id || id == UINT(-1)) continue;
        const auto saved = restored.visibility.find(id);
        pane->m_bRecentVisibleState = saved != restored.visibility.end() ? saved->second :
            (!state.hiddenPanes.count(pane) && (::GetWindowLongPtrW(pane->m_hWnd, GWL_STYLE) & WS_VISIBLE) != 0);
    }
    impl__m_bRestoringDockState_CDockingManager__2HA = TRUE;

    auto previousEdges = std::exchange(state.edgePanes, {});
    RECT frameBounds{};
    ::GetClientRect(owner->m_hWnd, &frameBounds);
    state.fitFrameClient = state.fitFrameClient || !restored.edges.empty();
    auto remaining = DockedFrameBounds(state, frameBounds);
    for (auto& entry : restored.edges) {
        auto* edge = entry.get();
        if (!edge->m_hWnd) continue;
        const LONG thickness = MeasureStoredEdge(*edge, remaining, owner->m_hWnd);
        const auto panes = openmfc::detail::ContainerPanes(edge->m_dividerData.pContainerManager);
        bool added = false;
        for (auto* pane : panes) {
            const HWND window = pane->m_hWnd;
            const BOOL visible = pane->m_bRecentVisibleState;
            if (RestoreDockedPane(manager, *owner, *edge, *pane, visible)) added = true;
            else if (::IsWindow(window)) edge->RemovePane(pane);
        }
        if (added) {
            RegisterFrameEdgeDivider(manager, *edge, thickness, false, nullptr);
            edge->m_dividerData.destroyWithWindow = true;
            entry.release();
        }
    }
    for (auto& record : restored.tabs) {
        auto* pane = LayoutPane(manager, record.id);
        if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) continue;
        auto* group = static_cast<CBaseTabbedPane*>(pane);
        if (auto* tabs = group->GetTabs()) {
            auto& values = EnsureTabCtrlState(tabs);
            values.restoredTabs = std::move(record.tabs);
            values.restoredActiveTab = record.active;
            impl__ApplyRestoredTabInfo_CBaseTabbedPane__UEAAXH_Z(group, TRUE);
        }
    }
    for (auto& item : restored.floating) {
        auto* frame = item.get();
        if (frame->IsKindOf(CMultiPaneFrameWnd::GetThisClass()))
            impl__SetDockState_CMultiPaneFrameWnd__UEAAXPEAVCDockingManager___Z(frame, &manager);
        else
            impl__SetDockState_CPaneFrameWnd__UEAAXPEAVCDockingManager___Z(frame, &manager);
        if (frame->m_hWnd) {
            impl__AddMiniFrame_CDockingManager__UEAAHPEAVCPaneFrameWnd___Z(&manager, frame);
            frame->AutoDestroyWithWindow();
            item.release();
        }
    }
    std::unordered_set<HWND> reservedEdges;
    for (auto* pane : state.panes) {
        if (!pane || !pane->IsKindOf(RUNTIME_CLASS(CPane))) continue;
        if (auto* edge = openmfc::detail::ReservedPaneDivider(*static_cast<CPane*>(pane)))
            reservedEdges.insert(edge->m_hWnd);
    }
    for (const auto& old : previousEdges) {
        auto* edge = static_cast<CPaneDivider*>(CWnd::FromHandlePermanent(old.divider));
        if (!edge) continue;
        for (auto* pane : openmfc::detail::ContainerPanes(edge->m_dividerData.pContainerManager)) {
            if (pane->IsKindOf(RUNTIME_CLASS(CBaseTabbedPane))) {
                auto* group = static_cast<CBaseTabbedPane*>(pane);
                if (group->GetTabs() && !group->GetTabs()->GetTabsCount()) group->DestroyWindow();
            }
        }
        if (reservedEdges.count(old.divider) ||
                !openmfc::detail::ContainerPanes(edge->m_dividerData.pContainerManager).empty())
            state.edgePanes.push_back(old);
        else
            edge->DestroyWindow();
    }
    CMFCAutoHideBar* groupBar = nullptr;
    for (const auto& record : restored.autoHide) {
        auto* pane = LayoutPane(manager, record.id);
        if (record.first) groupBar = nullptr;
        if (!pane || pane->IsAutoHideMode()) continue;
        if (auto* bar = pane->SetAutoHideMode(TRUE, record.alignment, groupBar, FALSE)) {
            bar->m_bFirstInGroup = record.first;
            bar->m_bLastInGroup = record.last;
            bar->m_bActiveInGroup = record.active;
            groupBar = record.last ? nullptr : bar;
            if (!::IsRectEmpty(&record.bounds) && !::IsRectEmpty(&frameBounds)) {
                const int width = static_cast<int>((std::min)(static_cast<long long>(record.bounds.right) - record.bounds.left,
                    static_cast<long long>(frameBounds.right)));
                const int height = static_cast<int>((std::min)(static_cast<long long>(record.bounds.bottom) - record.bounds.top,
                    static_cast<long long>(frameBounds.bottom)));
                ::SetWindowPos(pane->m_hWnd, nullptr, 0, 0, width, height,
                    SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
            }
            if (record.visible) {
                auto* divider = static_cast<CPaneDivider*>(
                    impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pane));
                impl__AlignAutoHidePane_CDockingManager__QEAAXPEAVCPaneDivider__H_Z(&manager, divider, TRUE);
                ::ShowWindow(pane->m_hWnd, SW_SHOWNOACTIVATE);
                if (divider) ::ShowWindow(divider->m_hWnd, SW_SHOWNOACTIVATE);
            }
        }
    }
    for (auto* pane : state.panes) {
        if (!pane || !pane->m_hWnd || state.floatingPanes.count(pane) ||
                impl__GetParentTabbedPane_CBasePane__QEBAPEAVCBaseTabbedPane__XZ(pane)) continue;
        if (pane->IsKindOf(RUNTIME_CLASS(CDockablePane)) && static_cast<CDockablePane*>(pane)->IsAutoHideMode()) continue;
        const auto saved = restored.visibility.find(PaneCommandID(pane));
        if (saved == restored.visibility.end()) continue;
        if (saved->second) manager.ShowPane(pane); else manager.HidePane(pane);
        if (!DockedPaneDivider(pane))
            ::ShowWindow(pane->m_hWnd, saved->second ? SW_SHOWNOACTIVATE : SW_HIDE);
    }
    for (std::size_t i = 0; i < state.hiddenMDITabbedBarCount; ++i) {
        const HWND window = reinterpret_cast<HWND>(state.hiddenMDITabbedBars[i]);
        auto* pane = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
        if (pane && pane->IsKindOf(RUNTIME_CLASS(CDockablePane)))
            static_cast<CDockablePane*>(pane)->m_bIsMDITabbed = FALSE;
    }
    state.hiddenMDITabbedBarCount = 0;
    for (UINT id : restored.hiddenMdi) {
        if (auto* pane = LayoutPane(manager, id)) {
            if (pane->m_bIsMDITabbed) continue;
            manager.HidePane(pane);
            pane->m_bIsMDITabbed = TRUE;
            impl__AddHiddenMDITabbedBar_CDockingManager__QEAAXPEAVCDockablePane___Z(&manager, pane);
        }
    }
}
}

// Symbol: ?Serialize@CDockingManager@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDockingManager__UEAAXAEAVCArchive___Z(
    CDockingManager* manager, CArchive& archive) {
    if (manager) manager->CDockingManager::Serialize(archive);
}

void CDockingManager::Serialize(CArchive& archive) {
    if (archive.IsStoring()) StoreDockingLayout(EnsureDockingState(this), archive);
    else LoadDockingLayout(*this, archive);
}

// Symbol: ?SetAutohideZOrder@CDockingManager@@QEAAXPEAVCDockablePane@@@Z
// mfc140u 0x497f0. The active pane sits behind its divider and above panes on its side.
extern "C" void MS_ABI impl__SetAutohideZOrder_CDockingManager__QEAAXPEAVCDockablePane___Z(
    CDockingManager* manager, CDockablePane* pane) {
    const auto* state = FindDockingState(manager);
    if (!state || !pane || !pane->m_hWnd) return;
    const HWND window = pane->m_hWnd;
    const HWND parent = ::GetParent(window);
    auto* divider = static_cast<CPaneDivider*>(
        impl__GetDefaultPaneDivider_CDockablePane__QEBAPEAVCPaneDivider__XZ(pane));
    const HWND dividerWindow = divider ? divider->m_hWnd : nullptr;
    const DWORD side = impl__GetCurrentAlignment_CBasePane__UEBAKXZ(pane);
    constexpr UINT flags = SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE;
    std::vector<HWND> adjacentPanes;
    for (auto it = state->panes.rbegin(); it != state->panes.rend(); ++it) {
        const auto* other = *it;
        if (other && other != pane && other->m_hWnd && ::GetParent(other->m_hWnd) == parent &&
            other->IsKindOf(RUNTIME_CLASS(CPane)) && impl__GetCurrentAlignment_CBasePane__UEBAKXZ(other) == side)
            adjacentPanes.push_back(other->m_hWnd);
    }
    const auto edges = state->edgePanes;
    for (auto it = edges.rbegin(); it != edges.rend(); ++it) {
        if (it->divider != dividerWindow && ::GetParent(it->divider) == parent)
            ::SetWindowPos(it->divider, HWND_BOTTOM, 0, 0, 0, 0, flags);
    }
    for (HWND other : adjacentPanes)
        ::SetWindowPos(other, window, 0, 0, 0, 0, flags);
    if (::IsWindow(window))
        ::SetWindowPos(window, dividerWindow ? dividerWindow : HWND_TOP, 0, 0, 0, 0, flags);
}

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
// mfc140u 0x4d080. OLE in-place frames own their control-bar visibility.
extern "C" void MS_ABI impl__SetPrintPreviewMode_CDockingManager__QEAAXHPEAUCPrintPreviewState___Z(
    CDockingManager* manager, int preview, CPrintPreviewState*) {
    if (!manager) return;
    auto& state = EnsureDockingState(manager);
    if (std::exchange(state.printPreviewMode, preview != FALSE) == (preview != FALSE)) return;

    const auto* frame = state.parentFrame;
    const void* notifyHook = nullptr;
    if (frame && frame->m_hWnd)
        std::memcpy(&notifyHook, frame->m_retailMiddle.m_pNotifyHook, sizeof(notifyHook));

    auto previous = std::exchange(state.printPreviewHiddenBars, {});
    if (notifyHook) return;
    if (preview) {
        CObList autoHide;
        for (const auto& bar : state.autoHideBars) autoHide.AddTail(bar.get());
        impl__HideForPrintPreview_CDockingManager__IEAAXAEBVCObList___Z(manager, &autoHide);
        CObList panes;
        for (const auto& site : state.dockSites) panes.AddTail(site.site);
        for (auto* pane : state.panes) if (pane) panes.AddTail(pane);
        impl__HideForPrintPreview_CDockingManager__IEAAXAEBVCObList___Z(manager, &panes);
        return;
    }
    for (HWND window : previous) {
        auto* object = static_cast<CWnd*>(CWnd::FromHandlePermanent(window));
        if (!object) continue;
        if (object->IsKindOf(RUNTIME_CLASS(CBasePane)))
            SetPreviewPaneVisibility(static_cast<CBasePane*>(object), TRUE);
        else if (object->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
            ::ShowWindow(window, SW_SHOWNOACTIVATE);
    }
}

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
// mfc140u 0x4d870. Consume only mini-frames marked for delayed visibility.
extern "C" void MS_ABI impl__ShowDelayShowMiniFrames_CDockingManager__UEAAXH_Z(
    CDockingManager* manager, int show) {
    const auto* state = FindDockingState(manager);
    if (!state) return;
    const auto frames = state->miniFrames;
    for (auto* value : frames) {
        const auto* current = FindDockingState(manager);
        if (!current || std::find(current->miniFrames.begin(), current->miniFrames.end(), value) == current->miniFrames.end())
            continue;
        auto* frame = static_cast<CWnd*>(value);
        if (frame && frame->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
            openmfc::detail::ApplyDelayedMiniFrameVisibility(*static_cast<CPaneFrameWnd*>(frame), show);
    }
}

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
