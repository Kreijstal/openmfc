// Exercise OpenMFC's frame record pipeline with a retail frame and control bars.
#define OPENMFC_APPCORE_IMPL
#include "detail/DockStateSupport.h"
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
// Floating windows are outside this isolated native-client scenario.
extern "C" int impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(const void*) { std::abort(); }
namespace openmfc::detail::dock {
bool IsFloatingBar(const void* bar) { if (bar) std::abort(); return false; }
CMiniDockFrameWnd* CreateFloatingFrame(CFrameWnd*, DWORD) { std::abort(); }
CFrameWnd* FrameWindow(CMiniDockFrameWnd*) { std::abort(); }
CControlBar* FloatingDockBar(CMiniDockFrameWnd*) { std::abort(); }
}
void CFrameWnd::DockControlBar(CControlBar*, unsigned, const RECT*) { std::abort(); }
CFrameWnd* CWnd::GetParentFrame() const { std::abort(); }

int main() {
    using namespace openmfc::detail::dock;
    using openmfc::detail::frame::ControlBars;
    using openmfc::detail::frame::RegisterControlBar;
    using openmfc::detail::frame::UnregisterControlBar;
    retail = LoadLibraryW(L"mfc140u.dll");
    if (!retail) return 2;
    alignas(CFrameWnd) BYTE frameStorage[sizeof(CFrameWnd)]{};
    auto* frame = entry<CFrameWnd*(*)(void*)>(502)(frameStorage);
    frame->m_hWnd = CreateWindowExW(0, L"STATIC", L"frame", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                                   10, 10, 300, 200, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
    check(frame->m_hWnd != nullptr, "frame HWND");
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
    currentApp = previousApp;
    for (auto* bar : bars) {
        UnregisterControlBar(frame, bar);
        DestroyWindow(bar->m_hWnd);
        bar->m_hWnd = nullptr;
        destroy(bar);
    }
    check(ControlBars(frame).IsEmpty(), "release registrations");
    DestroyWindow(frame->m_hWnd);
    frame->m_hWnd = nullptr;
    entry<void(*)(CFrameWnd*)>(1129)(frame);
    std::printf("Frame dock records: %d failures\n", failures);
    return failures ? 1 : 0;
}
