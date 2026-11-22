// CMFCDragFrameImpl — standard Feature Pack docking previews.

#define OPENMFC_APPCORE_IMPL

#include "detail/DragFrameSupport.h"
#include "detail/MfccoreSupport.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <unordered_map>

extern "C" SIZE impl__m_sizeDragSensitivity_CDockablePane__1VCSize__A;
extern "C" int impl__m_bTabsAlwaysTop_CTabbedPane__2HA;
extern "C" void MS_ABI impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
    CDockingManager*, CWnd*, CPoint, CRect*, BOOL*, CDockablePane**);

namespace {
struct DragFrameLayout {
    void* vfptr;
    POINT hot;
    RECT floating;
    RECT expected;
    BOOL dockToTab;
    BOOL started;
    int insertedTab;
    CDockablePane* finalTarget;
    CDockablePane* oldTarget;
    int thickness;
    CWnd* window;
    CDockingManager* manager;
    CDockablePane* target;
    CWnd* dummy;
    BOOL tabOutline;
};

static_assert(sizeof(DragFrameLayout) == sizeof(CMFCDragFrameImpl));
static_assert(offsetof(DragFrameLayout, hot) == 0x08);
static_assert(offsetof(DragFrameLayout, floating) == 0x10);
static_assert(offsetof(DragFrameLayout, expected) == 0x20);
static_assert(offsetof(DragFrameLayout, dockToTab) == 0x30);
static_assert(offsetof(DragFrameLayout, started) == 0x34);
static_assert(offsetof(DragFrameLayout, insertedTab) == 0x38);
static_assert(offsetof(DragFrameLayout, finalTarget) == 0x40);
static_assert(offsetof(DragFrameLayout, oldTarget) == 0x48);
static_assert(offsetof(DragFrameLayout, thickness) == 0x50);
static_assert(offsetof(DragFrameLayout, window) == 0x58);
static_assert(offsetof(DragFrameLayout, manager) == 0x60);
static_assert(offsetof(DragFrameLayout, target) == 0x68);
static_assert(offsetof(DragFrameLayout, dummy) == 0x70);
static_assert(offsetof(DragFrameLayout, tabOutline) == 0x78);

constexpr wchar_t kOutlineClass[] = L"OpenMFC.DockDragOutline";
constexpr int kFloatingThickness = 4;
constexpr int kDockingThickness = 3;
thread_local std::unordered_map<const DragFrameLayout*, HWND> dragOutlines;

LRESULT CALLBACK DragOutlineProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCHITTEST) return HTTRANSPARENT;
    if (msg == WM_MOUSEACTIVATE) return MA_NOACTIVATE;
    if (msg == WM_ERASEBKGND) return TRUE;
    if (msg == WM_PAINT) {
        PAINTSTRUCT paint{};
        HDC dc = ::BeginPaint(window, &paint);
        ::FillRect(dc, &paint.rcPaint, ::GetSysColorBrush(COLOR_HIGHLIGHT));
        ::EndPaint(window, &paint);
        return 0;
    }
    return ::DefWindowProcW(window, msg, wParam, lParam);
}

void HideDragOutline(const DragFrameLayout& drag) {
    auto entry = dragOutlines.extract(&drag);
    if (!entry.empty() && ::GetWindowLongPtrW(entry.mapped(), GWLP_USERDATA) == reinterpret_cast<LONG_PTR>(&drag))
        ::DestroyWindow(entry.mapped());
}

HRGN OutlineRegion(const RECT& bounds, int thickness) {
    HRGN region = ::CreateRectRgnIndirect(&bounds);
    if (!region) return nullptr;
    RECT center = bounds;
    ::InflateRect(&center, -thickness, -thickness);
    if (!::IsRectEmpty(&center)) {
        if (HRGN inner = ::CreateRectRgnIndirect(&center)) {
            ::CombineRgn(region, region, inner, RGN_DIFF);
            ::DeleteObject(inner);
        }
    }
    return region;
}

void ShowDragOutline(const DragFrameLayout& drag, const RECT& bounds, int thickness, bool tab) {
    if (::IsRectEmpty(&bounds) || thickness <= 0) {
        HideDragOutline(drag);
        return;
    }
    HWND& window = dragOutlines[&drag];
    if (!window || ::GetWindowLongPtrW(window, GWLP_USERDATA) != reinterpret_cast<LONG_PTR>(&drag)) {
        HMODULE instance = nullptr;
        if (!::GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                reinterpret_cast<LPCWSTR>(&DragOutlineProc), &instance)) {
            dragOutlines.erase(&drag);
            return;
        }
        const WNDCLASSW type{0, DragOutlineProc, 0, 0, instance, nullptr, nullptr, nullptr, nullptr, kOutlineClass};
        if (!::RegisterClassW(&type) && ::GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
            dragOutlines.erase(&drag);
            return;
        }
        HWND owner = drag.window ? drag.window->m_hWnd : nullptr;
        window = ::CreateWindowExW(WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE | WS_EX_LAYERED,
            kOutlineClass, L"", WS_POPUP, 0, 0, 0, 0, owner, nullptr, instance, nullptr);
        if (!window) {
            dragOutlines.erase(&drag);
            return;
        }
        ::SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&drag));
        ::SetLayeredWindowAttributes(window, 0, 210, LWA_ALPHA);
    }

    const LONG width = bounds.right - bounds.left;
    const LONG height = bounds.bottom - bounds.top;
    RECT body{0, 0, width, height};
    RECT lip{};
    if (tab && width >= 40 && height >= 36) {
        const LONG tabHeight = (std::min)(height / 3, 18L);
        if (impl__m_bTabsAlwaysTop_CTabbedPane__2HA) {
            body.top += tabHeight;
            lip = {10, 0, (std::min)(width - 4, 50L), tabHeight + thickness};
        } else {
            body.bottom -= tabHeight;
            lip = {10, height - tabHeight - thickness, (std::min)(width - 4, 50L), height};
        }
    }
    HRGN region = OutlineRegion(body, thickness);
    if (!region) {
        HideDragOutline(drag);
        return;
    }
    if (!::IsRectEmpty(&lip)) {
        if (HRGN tabRegion = OutlineRegion(lip, thickness)) {
            ::CombineRgn(region, region, tabRegion, RGN_OR);
            ::DeleteObject(tabRegion);
        }
    }
    if (!::SetWindowRgn(window, region, TRUE)) {
        ::DeleteObject(region);
        HideDragOutline(drag);
        return;
    }
    ::SetWindowPos(window, HWND_TOPMOST, bounds.left, bounds.top, width, height, SWP_NOACTIVATE | SWP_SHOWWINDOW);
    ::InvalidateRect(window, nullptr, FALSE);
    ::UpdateWindow(window);
}

void RemoveTemporaryTab(DragFrameLayout& drag) {
    std::unique_ptr<CWnd> dummy(drag.dummy);
    drag.dummy = nullptr;
    if (dummy && dummy->m_hWnd) {
        auto* parent = static_cast<CWnd*>(CWnd::FromHandlePermanent(::GetParent(dummy->m_hWnd)));
        if (parent && parent->IsKindOf(RUNTIME_CLASS(CMFCBaseTabCtrl))) {
            auto* tabs = static_cast<CMFCBaseTabCtrl*>(parent);
            for (int index = 0; index < tabs->GetTabsCount(); ++index) {
                if (tabs->GetTabWnd(index) == dummy.get()) {
                    tabs->RemoveTab(index);
                    break;
                }
            }
        }
        dummy->DestroyWindow();
    }
    drag.insertedTab = -1;
    drag.oldTarget = nullptr;
    if (drag.tabOutline) HideDragOutline(drag);
    drag.tabOutline = FALSE;
}

void DrawTabOutline(DragFrameLayout& drag, CDockablePane& target) {
    RECT bounds{};
    if (!::GetWindowRect(target.m_hWnd, &bounds)) return;
    ShowDragOutline(drag, bounds, kDockingThickness, true);
    drag.tabOutline = TRUE;
    drag.insertedTab = 1;
    drag.oldTarget = &target;
}

CWnd* DraggedPane(const DragFrameLayout& drag) {
    if (drag.window && drag.window->IsKindOf(RUNTIME_CLASS(CPaneFrameWnd)))
        return static_cast<CPaneFrameWnd*>(drag.window)->GetFirstVisiblePane();
    return drag.window;
}

bool PlaceTemporaryTab(DragFrameLayout& drag, CBaseTabbedPane& group) {
    auto* tabs = group.GetTabs();
    const auto* state = tabs ? openmfc::detail::mfccore::FindTabCtrlState(tabs) : nullptr;
    if (!state || !::IsWindow(tabs->m_hWnd) || state->strip.IsRectEmpty()) return false;
    auto dummy = std::make_unique<CWnd>();
    const RECT empty{};
    if (!dummy->Create(L"STATIC", L"", WS_CHILD, empty, tabs, static_cast<UINT>(-1))) return false;
    std::wstring caption;
    if (auto* pane = DraggedPane(drag); pane && pane->m_hWnd) {
        caption.resize(::GetWindowTextLengthW(pane->m_hWnd) + 1);
        caption.resize(::GetWindowTextW(pane->m_hWnd, caption.data(), static_cast<int>(caption.size())));
    }
    const int index = tabs->GetTabsCount();
    if (!tabs->InsertTab(dummy.get(), caption.c_str(), index, static_cast<UINT>(-1), FALSE)) {
        dummy->DestroyWindow();
        return false;
    }
    drag.dummy = dummy.release();
    drag.insertedTab = index;
    drag.oldTarget = &group;
    return true;
}

void PlaceTabPreview(DragFrameLayout& drag, CDockablePane& target) {
    if (drag.oldTarget == &target && drag.dummy && drag.dummy->m_hWnd) return;
    RemoveTemporaryTab(drag);
    HideDragOutline(drag);
    drag.target = &target;
    if (target.IsKindOf(RUNTIME_CLASS(CBaseTabbedPane)) && PlaceTemporaryTab(drag, static_cast<CBaseTabbedPane&>(target)))
        return;
    DrawTabOutline(drag, target);
}

void ClearPreview(DragFrameLayout& drag) {
    RemoveTemporaryTab(drag);
    HideDragOutline(drag);
}
}

void openmfc::detail::BeginDockDrag(CMFCDragFrameImpl& frame, CWnd& window, CDockingManager* manager, POINT anchor) {
    impl__ResetState_CMFCDragFrameImpl__QEAAXXZ(&frame);
    auto& drag = reinterpret_cast<DragFrameLayout&>(frame);
    drag.window = &window;
    drag.manager = manager;
    drag.hot = anchor;
}

bool openmfc::detail::DockDragBounds(const CMFCDragFrameImpl& frame, RECT& bounds) {
    const auto& drag = reinterpret_cast<const DragFrameLayout&>(frame);
    bounds = drag.floating;
    return drag.started && !::IsRectEmpty(&bounds);
}

void openmfc::detail::UpdateDockDragPreview(CMFCDragFrameImpl& frame, bool showFloatingOutline, bool force) {
    auto& drag = reinterpret_cast<DragFrameLayout&>(frame);
    drag.finalTarget = nullptr;
    if (!drag.window || !drag.manager || !::IsWindow(drag.window->m_hWnd)) return;
    POINT cursor{};
    if (!::GetCursorPos(&cursor)) return;
    if (drag.hot.x == -1 && drag.hot.y == -1) drag.hot = cursor;
    const LONG dx = cursor.x - drag.hot.x;
    const LONG dy = cursor.y - drag.hot.y;
    const auto& sensitivity = impl__m_sizeDragSensitivity_CDockablePane__1VCSize__A;
    if (!force && !drag.started && std::abs(dx) < sensitivity.cx && std::abs(dy) < sensitivity.cy) return;

    if (::IsRectEmpty(&drag.floating)) {
        if (!::GetWindowRect(drag.window->m_hWnd, &drag.floating)) return;
        if (!::PtInRect(&drag.floating, drag.hot)) {
            const LONG width = drag.floating.right - drag.floating.left;
            ::OffsetRect(&drag.floating, drag.hot.x - drag.floating.left - width / 2,
                drag.hot.y - drag.floating.top - 5);
        }
    }
    ::OffsetRect(&drag.floating, dx, dy);
    drag.hot = cursor;
    drag.started = TRUE;

    CRect bounds;
    BOOL tab = FALSE;
    CDockablePane* target = nullptr;
    impl__CalcExpectedDockedRect_CDockingManager__QEAAXPEAVCWnd__VCPoint__AEAVCRect__AEAHPEAPEAVCDockablePane___Z(
        drag.manager, drag.window, CPoint(cursor), &bounds, &tab, &target);
    drag.expected = *static_cast<const RECT*>(bounds);
    drag.target = target;
    auto* pane = DraggedPane(drag);
    if (tab && target && pane && pane->IsKindOf(RUNTIME_CLASS(CBasePane)) &&
        static_cast<CBasePane*>(pane)->CanBeAttached() && target->CanBeAttached()) {
        PlaceTabPreview(drag, *target);
    } else {
        RemoveTemporaryTab(drag);
        if (!bounds.IsRectEmpty()) {
            drag.thickness = kDockingThickness;
            ShowDragOutline(drag, drag.expected, drag.thickness, false);
        } else if (showFloatingOutline) {
            drag.thickness = kFloatingThickness;
            ShowDragOutline(drag, drag.floating, drag.thickness, false);
        } else {
            HideDragOutline(drag);
        }
    }
}

// Symbol: ??0CMFCDragFrameImpl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDragFrameImpl__QEAA_XZ(void* storage) {
    if (!storage) return nullptr;
    auto& drag = *static_cast<DragFrameLayout*>(storage);
    // This compact type dispatches its exports directly.
    constexpr size_t fields = offsetof(DragFrameLayout, hot);
    std::memset(static_cast<unsigned char*>(storage) + fields, 0, sizeof(drag) - fields);
    drag.insertedTab = -1;
    drag.thickness = kFloatingThickness;
    return storage;
}

// Symbol: ??1CMFCDragFrameImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCDragFrameImpl__UEAA_XZ(void* storage) {
    if (storage) ClearPreview(*static_cast<DragFrameLayout*>(storage));
}

// Symbol: ?DrawDragFrame@CMFCDragFrameImpl@@IEAAXPEBUtagRECT@@0HHH@Z
extern "C" void MS_ABI impl__DrawDragFrame_CMFCDragFrameImpl__IEAAXPEBUtagRECT__0HHH_Z(
    void* storage, const RECT*, const RECT* next, int, int thickness, int) {
    if (!storage) return;
    auto& drag = *static_cast<DragFrameLayout*>(storage);
    if (next) ShowDragOutline(drag, *next, thickness, false);
    else HideDragOutline(drag);
}

// Symbol: ?DrawFrameTab@CMFCDragFrameImpl@@IEAAXPEAVCDockablePane@@H@Z
extern "C" void MS_ABI impl__DrawFrameTab_CMFCDragFrameImpl__IEAAXPEAVCDockablePane__H_Z(
    void* storage, CDockablePane* target, int erase) {
    if (!storage || !target) return;
    auto& drag = *static_cast<DragFrameLayout*>(storage);
    if (erase) RemoveTemporaryTab(drag);
    else DrawTabOutline(drag, *target);
}

// Symbol: ?EndDrawDragFrame@CMFCDragFrameImpl@@QEAAXH@Z
extern "C" void MS_ABI impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(void* storage, int clear) {
    if (!storage) return;
    auto& drag = *static_cast<DragFrameLayout*>(storage);
    drag.dockToTab = drag.insertedTab >= 0;
    ClearPreview(drag);
    if (clear) {
        ::SetRectEmpty(&drag.floating);
        ::SetRectEmpty(&drag.expected);
        drag.finalTarget = drag.target;
        drag.target = nullptr;
    }
    drag.started = FALSE;
}

// Symbol: ?Init@CMFCDragFrameImpl@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__Init_CMFCDragFrameImpl__QEAAXPEAVCWnd___Z(void* storage, CWnd* window) {
    if (!storage || !window) return;
    CGlobalUtils utils;
    POINT cursor{-1, -1};
    ::GetCursorPos(&cursor);
    openmfc::detail::BeginDockDrag(*static_cast<CMFCDragFrameImpl*>(storage), *window,
        utils.GetDockingManager(window), cursor);
}

// Symbol: ?MoveDragFrame@CMFCDragFrameImpl@@QEAAXH@Z
extern "C" void MS_ABI impl__MoveDragFrame_CMFCDragFrameImpl__QEAAXH_Z(void* storage, int force) {
    if (storage) openmfc::detail::UpdateDockDragPreview(*static_cast<CMFCDragFrameImpl*>(storage), true, force != FALSE);
}

// Symbol: ?PlaceTabPreDocking@CMFCDragFrameImpl@@QEAAXPEAVCBaseTabbedPane@@H@Z
extern "C" void MS_ABI impl__PlaceTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCBaseTabbedPane__H_Z(
    void* storage, CBaseTabbedPane* group, int) {
    if (storage && group) PlaceTabPreview(*static_cast<DragFrameLayout*>(storage), *group);
}

// Symbol: ?PlaceTabPreDocking@CMFCDragFrameImpl@@QEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__PlaceTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCWnd___Z(void* storage, CWnd* target) {
    if (storage && target && target->IsKindOf(RUNTIME_CLASS(CDockablePane)))
        PlaceTabPreview(*static_cast<DragFrameLayout*>(storage), static_cast<CDockablePane&>(*target));
}

// Symbol: ?RemoveTabPreDocking@CMFCDragFrameImpl@@QEAAXPEAVCDockablePane@@@Z
extern "C" void MS_ABI impl__RemoveTabPreDocking_CMFCDragFrameImpl__QEAAXPEAVCDockablePane___Z(void* storage, void*) {
    if (storage) RemoveTemporaryTab(*static_cast<DragFrameLayout*>(storage));
}

// Symbol: ?ResetState@CMFCDragFrameImpl@@QEAAXXZ
extern "C" void MS_ABI impl__ResetState_CMFCDragFrameImpl__QEAAXXZ(void* storage) {
    if (!storage) return;
    auto& drag = *static_cast<DragFrameLayout*>(storage);
    ClearPreview(drag);
    drag.hot = {-1, -1};
    ::SetRectEmpty(&drag.floating);
    ::SetRectEmpty(&drag.expected);
    drag.target = drag.finalTarget = nullptr;
    drag.started = drag.dockToTab = FALSE;
}
