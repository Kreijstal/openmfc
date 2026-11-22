// Exercise OpenMFC's frame record pipeline with a retail frame and control bars.
#define OPENMFC_APPCORE_IMPL
#include "detail/DockStateSupport.h"
#include "detail/DockContextSupport.h"
#include "detail/AppProfile.h"
#include "detail/ClassicDockingSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include "openmfc/afxstat_.h"
#include "openmfc/afxole.h"
#include <cstdio>
#include <cstdlib>

namespace {
HMODULE retail;
template<class T> T entry(WORD ordinal) {
    auto* address = GetProcAddress(retail, MAKEINTRESOURCEA(ordinal));
    if (!address) { std::fprintf(stderr, "Missing ordinal %u\n", unsigned(ordinal)); std::abort(); }
    return reinterpret_cast<T>(address);
}
int failures;
void check(bool condition, const char* label) {
    if (!condition) { std::fprintf(stderr, "FAIL: %s\n", label); ++failures; }
}
}

extern "C" void impl__GetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo___Z(CControlBar* bar, void* info) {
    entry<void(*)(CControlBar*, void*)>(4926)(bar, info);
}
extern "C" void impl__SetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo__PEAVCFrameWnd___Z(
    CControlBar* bar, void* info, CFrameWnd* frame) {
    entry<void(*)(CControlBar*, void*, CFrameWnd*)>(12954)(bar, info, frame);
}
// Supply retail window creation and dock-bar operations. The frame's routing
// and the allocation and lifetime of its control-bar contexts are production code.
extern "C" CRuntimeClass* impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ() {
    return entry<CRuntimeClass*(*)()>(6824)();
}
extern "C" CRuntimeClass* impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ() {
    return entry<CRuntimeClass*(*)()>(6854)();
}
extern "C" CRuntimeClass* impl__GetThisClass_CMiniDockFrameWnd__SAPEAUCRuntimeClass__XZ() {
    return entry<CRuntimeClass*(*)()>(7056)();
}
extern "C" int impl__Create_CDockBar__UEAAHPEAVCWnd__KI_Z(void* bar, CWnd* owner, DWORD style, UINT id) {
    return entry<int(*)(void*, CWnd*, DWORD, UINT)>(3075)(bar, owner, style, id);
}
extern "C" void impl__DockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(
    void* dock, CControlBar* bar, const RECT* rect) {
    entry<void(*)(void*, CControlBar*, const RECT*)>(3878)(dock, bar, rect);
}
extern "C" void impl__ReDockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(
    void* dock, CControlBar* bar, const RECT* rect) {
    entry<void(*)(void*, CControlBar*, const RECT*)>(12180)(dock, bar, rect);
}
extern "C" int impl__RemoveControlBar_CDockBar__QEAAHPEAVCControlBar__HH_Z(
    void* dock, CControlBar* bar, int exclude, int placeholder) {
    return entry<int(*)(void*, CControlBar*, int, int)>(12342)(dock, bar, exclude, placeholder);
}
extern "C" int impl__GetDockedCount_CDockBar__QEBAHXZ(const void* dock) {
    return entry<int(*)(const void*)>(5250)(dock);
}
extern "C" int impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(const void* dock) {
    return entry<int(*)(const void*)>(5251)(dock);
}
extern "C" void impl__EnableDocking_CControlBar__QEAAXK_Z(CControlBar*, DWORD);
extern "C" void impl__StartDrag_CDockContext__UEAAXVCPoint___Z(void*, long long) { std::abort(); }
extern "C" void impl__StartResize_CDockContext__UEAAXHVCPoint___Z(void*, int, long long) { std::abort(); }
extern "C" void impl__ToggleDocking_CDockContext__UEAAXXZ(void*) { std::abort(); }
namespace openmfc::detail::dock {
CControlBar* NewDockBar() {
    void* storage = std::malloc(0x190);
    return storage ? entry<CControlBar*(*)(void*, BOOL)>(459)(storage, FALSE) : nullptr;
}
bool IsFloatingBar(const void* bar) {
    return bar && *reinterpret_cast<const BOOL*>(static_cast<const BYTE*>(bar) + 0x148);
}
CMiniDockFrameWnd* CreateFloatingFrame(CFrameWnd* frame, DWORD style, CRuntimeClass*) {
    return entry<CMiniDockFrameWnd*(*)(CFrameWnd*, DWORD)>(3280)(frame, style);
}
CFrameWnd* FrameWindow(CMiniDockFrameWnd* frame) { return reinterpret_cast<CFrameWnd*>(frame); }
CControlBar* FloatingDockBar(CMiniDockFrameWnd* frame) {
    return reinterpret_cast<CControlBar*>(reinterpret_cast<BYTE*>(frame) + 0x1f0);
}
}
CFrameWnd* CWnd::GetParentFrame() const { return entry<CFrameWnd*(*)(const CWnd*)>(6074)(this); }
void* CWnd::FromHandlePermanent(HWND handle) { return entry<CWnd*(*)(HWND)>(4730)(handle); }

namespace {
void TestDockTarget(CFrameWnd* frame, CControlBar* top) {
    using namespace openmfc::detail::dock;
    const auto* dock = reinterpret_cast<CDockBar*>(top);
    ::SetWindowPos(top->m_hWnd, nullptr, 0, 0, 160, 0, SWP_NOZORDER | SWP_NOACTIVATE);
    RECT bounds{};
    ::GetWindowRect(top->m_hWnd, &bounds);
    const CRect inside(bounds.left, bounds.top, bounds.left + 12, bounds.top + 12);
    const CRect outside(bounds.left, bounds.top + 12, bounds.left + 12, bounds.top + 24);
    auto native = entry<DWORD(*)(CFrameWnd*, CRect, DWORD, CDockBar**)>(2667);
    auto compare = [&](CRect rect, DWORD style, DWORD expected, const char* label) {
        CDockBar* nativeTarget = reinterpret_cast<CDockBar*>(top);
        CDockBar* sourceTarget = nativeTarget;
        const DWORD nativeStyle = native(frame, rect, style, &nativeTarget);
        const DWORD sourceStyle = frame->CanDock(rect, style, &sourceTarget);
        check(nativeStyle == expected && sourceStyle == nativeStyle &&
              sourceTarget == nativeTarget && sourceTarget == (expected ? dock : nullptr), label);
    };
    compare(inside, alignTop, alignTop, "hit the zero-height top dock target");
    compare(outside, alignTop, 0, "miss a dock target outside the drag bounds");
    compare(inside, alignBottom, 0, "exclude a dock target on the wrong side");
    ::ShowWindow(top->m_hWnd, SW_HIDE);
    compare(inside, alignTop, 0, "exclude a hidden dock target");
    ::ShowWindow(top->m_hWnd, SW_SHOWNA);
}

void TestFloating(CFrameWnd* frame, CControlBar* bar) {
    using namespace openmfc::detail::dock;
    const HWND handle = bar->m_hWnd;
    bar->m_hWnd = nullptr;
    check(entry<BOOL(*)(CWnd*, HWND)>(13956)(bar, handle), "subclass the toolbar window");
    bar->m_pDockSite = frame;
    bar->m_dwStyle = alignTop;
    frame->EnableDocking(alignTop);
    auto* top = frame->GetControlBar(topBarId);
    check(top && ::IsWindow(top->m_hWnd), "enable the top dock bar");
    if (!top) return;
    TestDockTarget(frame, top);

    impl__EnableDocking_CControlBar__QEAAXK_Z(bar, alignTop);
    auto* context = static_cast<DockContext*>(bar->m_pDockContext);
    check(context && context->m_pBar == bar && context->m_pDockSite == frame &&
          context->m_dwMRUFloatStyle == alignTop && bar->m_hWndOwner == frame->m_hWnd,
          "initialize the dock context and owner");
    impl__EnableDocking_CControlBar__QEAAXK_Z(bar, alignTop | alignBottom);
    check(context == bar->m_pDockContext && bar->m_dwDockStyle == (alignTop | alignBottom),
          "reuse the dock context when allowed sides change");

    frame->DockControlBar(bar, topBarId, nullptr);
    check(bar->m_pDockBar == top && ::GetParent(handle) == top->m_hWnd, "dock the toolbar");
    frame->FloatControlBar(bar, CPoint(245, 140), alignTop);
    auto* dock = static_cast<CControlBar*>(bar->m_pDockBar);
    check(dock && dock != top && IsFloatingBar(dock), "float into a distinct dock bar");
    if (!dock || dock == top || !IsFloatingBar(dock)) return;
    auto* floating = dock->GetParentFrame();
    check(floating && ::GetParent(handle) == dock->m_hWnd && ::IsWindowVisible(floating->m_hWnd),
          "float reparents the visible toolbar");
    if (!floating) return;
    const HWND originalPopup = floating->m_hWnd;
    frame->FloatControlBar(bar, CPoint(280, 170), alignTop);
    RECT popupRect{};
    ::GetWindowRect(originalPopup, &popupRect);
    check(bar->m_pDockBar == dock && popupRect.left == 280 && popupRect.top == 170,
          "repeat float reuses and moves the popup");

    frame->SaveBarState(L"Floating layout");
    const HWND topHandle = top->m_hWnd;
    frame->ReDockControlBar(bar, reinterpret_cast<CDockBar*>(top), nullptr);
    check(bar->m_pDockBar == top && ::GetParent(handle) == topHandle &&
          !::IsWindow(originalPopup), "redock closes the empty popup");

    frame->LoadBarState(L"Floating layout");
    auto* restoredDock = static_cast<CControlBar*>(bar->m_pDockBar);
    check(restoredDock && restoredDock != top && IsFloatingBar(restoredDock) &&
          ::GetParent(handle) == restoredDock->m_hWnd, "restore floating membership from the profile");
    HWND restoredPopup = nullptr;
    if (restoredDock && restoredDock != top) {
        if (auto* popup = restoredDock->GetParentFrame()) restoredPopup = popup->m_hWnd;
    }
    frame->DestroyDockBars();
    check(!::IsWindow(topHandle) && !::IsWindow(restoredPopup) && !::IsWindow(handle),
          "destroy dock and floating windows");
    check(openmfc::detail::frame::ControlBars(frame).IsEmpty(), "destroy unregisters the toolbar and dock bars");
}
}

int main() {
    using namespace openmfc::detail::dock;
    using openmfc::detail::frame::ControlBars;
    using openmfc::detail::frame::RegisterControlBar;
    using openmfc::detail::frame::UnregisterControlBar;
    retail = LoadLibraryW(L"mfc140u.dll");
    if (!retail) return 2;
    if (!entry<BOOL(*)(HINSTANCE, HINSTANCE, LPWSTR, int)>(2369)(
            GetModuleHandleW(nullptr), nullptr, GetCommandLineW(), SW_SHOWNORMAL))
        return 3;
    alignas(CFrameWnd) BYTE frameStorage[sizeof(CFrameWnd)]{};
    auto* frame = entry<CFrameWnd*(*)(void*)>(502)(frameStorage);
    const HWND host = CreateWindowExW(0, L"STATIC", L"frame", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                      10, 10, 300, 200, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
    if (!host || !entry<BOOL(*)(CWnd*, HWND)>(2487)(frame, host)) return 4;
    alignas(CControlBar) BYTE barStorage[2][0x800]{};
    auto init = entry<CControlBar*(*)(void*)>(972);
    auto destroy = entry<void(*)(CControlBar*)>(1438);
    CControlBar* bars[] = {init(barStorage[0]), init(barStorage[1])};
    for (unsigned i=0; i<2; ++i) {
        bars[i]->m_hWnd = CreateWindowExW(0, L"STATIC", L"bar", WS_CHILD | (i ? 0 : WS_VISIBLE),
                                        10, 10, 80, 30, frame->m_hWnd,
                                        reinterpret_cast<HMENU>(UINT_PTR(900+i)), GetModuleHandleW(nullptr), nullptr);
        bars[i]->m_nMRUWidth = 77+42*i;
        RegisterControlBar(frame, bars[i]);
        RegisterControlBar(frame, bars[i]);
    }
    check(ControlBars(frame).GetCount() == 2 && frame->GetControlBar(900) == bars[0], "registered bar identity");
    ScopedDockState snapshot;
    frame->GetDockState(snapshot.Get());
    check(Bars(snapshot.Get()).GetCount() == 2, "capture both records");
    const auto* info = static_cast<const S_Ccontrolbarinfo*>(Bars(snapshot.Get())[1]);
    check(info->m_nBarID == 901 && info->m_nMRUWidth == 119 && !info->m_bVisible, "capture MRU and visibility");
    frame->GetDockState(snapshot.Get());
    check(Bars(snapshot.Get()).GetCount() == 2, "replace records");
    auto* module = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    alignas(CWinApp) BYTE appIdentity[sizeof(CWinApp)]{};
    auto* app = reinterpret_cast<CWinApp*>(appIdentity);
    auto& currentApp = *reinterpret_cast<CWinApp**>(reinterpret_cast<BYTE*>(module) + 8);
    auto* previousApp = currentApp;
    currentApp = app;
    frame->SaveBarState(L"Frame layout");
    check(impl__GetProfileIntW_CWinApp__UEAAIPEB_W0H_Z(app, L"Frame layout-Summary", L"Bars", 0) == 2,
          "profile contains both records");
    bars[0]->m_nMRUWidth = 900;
    bars[1]->m_nMRUWidth = 901;
    ShowWindow(bars[0]->m_hWnd, SW_HIDE);
    ShowWindow(bars[1]->m_hWnd, SW_SHOW);
    frame->LoadBarState(L"Frame layout");
    check(bars[0]->m_nMRUWidth == 77 && bars[1]->m_nMRUWidth == 119, "restore MRU width");
    check(IsWindowVisible(bars[0]->m_hWnd) && !IsWindowVisible(bars[1]->m_hWnd), "restore bar visibility by ID");
    UnregisterControlBar(frame, bars[1]);
    DestroyWindow(bars[1]->m_hWnd);
    bars[1]->m_hWnd = nullptr;
    destroy(bars[1]);
    TestFloating(frame, bars[0]);
    currentApp = previousApp;
    destroy(bars[0]);
    entry<HWND(*)(CWnd*)>(3841)(frame);
    DestroyWindow(host);
    entry<void(*)(CFrameWnd*)>(1129)(frame);
    std::printf("Frame dock records: %d failures\n", failures);
    return failures ? 1 : 0;
}
