#define OPENMFC_APPCORE_IMPL
#include "detail/ClassicDockingSupport.h"
#include "detail/DockStateSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include "openmfc/afxole.h"
#include <memory>

extern "C" void impl__GetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo___Z(CControlBar*, void*);
extern "C" void impl__SetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo__PEAVCFrameWnd___Z(
    CControlBar*, void*, CFrameWnd*);

using namespace openmfc::detail::dock;
using openmfc::detail::frame::ControlBars;

CControlBar* CFrameWnd::GetControlBar(unsigned int nID) {
    for (auto* node = ControlBars(this).m_pNodeHead; node; node = node->pNext) {
        auto* bar = static_cast<CControlBar*>(node->data);
        if (bar && bar->m_hWnd && static_cast<UINT>(::GetDlgCtrlID(bar->m_hWnd)) == nID)
            return bar;
    }
    return nullptr;
}

void CFrameWnd::GetDockState(CDockState& state) const {
    impl__Clear_CDockState__QEAAXXZ(&state);
    for (auto* node = ControlBars(this).m_pNodeHead; node; node = node->pNext) {
        auto* bar = static_cast<CControlBar*>(node->data);
        if (!bar) continue;
        std::unique_ptr<S_Ccontrolbarinfo, decltype(&DeleteBarInfo)> info(NewBarInfo(), &DeleteBarInfo);
        if (!info) AfxThrowMemoryException();
        impl__GetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo___Z(bar, info.get());
        Bars(state).Add(info.get());
        info.release();
    }
}

void CFrameWnd::SaveBarState(const wchar_t* profile) const {
    ScopedDockState state;
    GetDockState(state.Get());
    impl__SaveState_CDockState__QEAAXPEB_W_Z(&state.Get(), profile);
}

void CFrameWnd::LoadBarState(const wchar_t* profile) {
    ScopedDockState state;
    impl__LoadState_CDockState__QEAAXPEB_W_Z(&state.Get(), profile);
    SetDockState(state.Get());
}

CMiniDockFrameWnd* CFrameWnd::CreateFloatingFrame(DWORD style) {
    return m_hWnd ? openmfc::detail::dock::CreateFloatingFrame(this, style, m_retailTail.m_pFloatingFrameClass) : nullptr;
}

void CFrameWnd::SetDockState(const CDockState& state) {
    const auto& bars = Bars(state);
    for (INT_PTR i = 0; i < bars.GetCount(); ++i) {
        auto* info = static_cast<S_Ccontrolbarinfo*>(bars[i]);
        if (!info) continue;
        CControlBar* bar = nullptr;
        if (info->m_bFloating) {
            auto* floating = CreateFloatingFrame(info->m_bHorz ? alignTop : alignLeft);
            if (floating) {
                auto* window = FrameWindow(floating);
                const POINT point = info->m_pointPos;
                RECT position{point.x, point.y, point.x + 10, point.y + 10};
                window->CalcWindowRect(&position);
                ::SetWindowPos(window->m_hWnd, nullptr, position.left, position.top, 0, 0,
                               SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
                bar = FloatingDockBar(floating);
            }
        } else {
            bar = GetControlBar(info->m_nBarID);
            if (bar && IsFloatingBar(bar->m_pDockBar))
                DockControlBar(bar, 0u, nullptr);
        }
        info->m_pBar = bar;
        if (bar) bar->m_nMRUWidth = info->m_nMRUWidth;
    }

    for (INT_PTR i = 0; i < bars.GetCount(); ++i) {
        auto* info = static_cast<S_Ccontrolbarinfo*>(bars[i]);
        if (info && info->m_pBar) {
            impl__SetBarInfo_CControlBar__QEAAXPEAVCControlBarInfo__PEAVCFrameWnd___Z(
                static_cast<CControlBar*>(info->m_pBar), info, this);
        }
    }

    for (INT_PTR i = 0; i < bars.GetCount(); ++i) {
        auto* info = static_cast<S_Ccontrolbarinfo*>(bars[i]);
        if (!info || !info->m_pBar || !info->m_bFloating ||
            impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(info->m_pBar) <= 0) continue;
        auto* bar = static_cast<CControlBar*>(info->m_pBar);
        auto* floating = bar->GetParentFrame();
        if (floating) {
            floating->RecalcLayout(TRUE);
            ::ShowWindow(floating->m_hWnd, SW_SHOWNA);
        }
    }
    m_retailTail.m_dwBorderFlags |= 0x0c;
}
