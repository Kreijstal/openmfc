#define OPENMFC_APPCORE_IMPL
#include "detail/ClassicDockingSupport.h"
#include "detail/DockContextSupport.h"
#include "detail/FrameLifetimeSupport.h"
#include <cstdlib>
#include <utility>

using openmfc::detail::dock::DockContext;
using openmfc::detail::dock::IsFloatingBar;
using openmfc::detail::frame::RegisterControlBar;
using openmfc::detail::frame::UnregisterControlBar;

IMPLEMENT_DYNAMIC(CControlBar, CWnd)

// Symbol: ?GetThisClass@CControlBar@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CControlBar__SAPEAUCRuntimeClass__XZ() {
    return CControlBar::GetThisClass();
}

// Symbol: ?GetRuntimeClass@CControlBar@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CControlBar__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return CControlBar::GetThisClass();
}

CControlBar::CControlBar()
    : m_pInPlaceOwner(nullptr), m_bAutoDelete(FALSE),
      m_cxLeftBorder(0), m_cxRightBorder(0), m_cyTopBorder(0), m_cyBottomBorder(0),
      m_cxDefaultGap(0), m_nMRUWidth(0), m_nCount(0), m_pData(nullptr),
      m_hReBarTheme(nullptr), m_nStateFlags(0), m_dwStyle(0), m_dwDockStyle(0),
      m_pDockSite(nullptr), m_pDockBar(nullptr), m_pDockContext(nullptr) {
}

CControlBar::~CControlBar() {
    m_pInPlaceOwner = nullptr;
    m_bAutoDelete = FALSE;
    CControlBar::DestroyWindow();
    if (m_pDockSite) {
        UnregisterControlBar(static_cast<CFrameWnd*>(m_pDockSite), this);
        m_pDockSite = nullptr;
    }
    if (auto* context = static_cast<DockContext*>(std::exchange(m_pDockContext, nullptr))) {
        using Delete = void* (*)(void*, UINT);
        auto* table = static_cast<void* const*>(context->vfptr);
        reinterpret_cast<Delete>(table[3])(context, 1);
        m_pDockBar = nullptr;
    }
    std::free(m_pData);
}

BOOL CControlBar::Create(CWnd* parent, DWORD style, UINT id) {
    if (!parent) return FALSE;
    m_dwStyle = style & 0xffff;
    RECT rect{};
    const DWORD windowStyle = (style & 0xffff0000) | WS_CHILD;
    if (!CWnd::Create(L"STATIC", nullptr, windowStyle, rect, parent, id, nullptr))
        return FALSE;
    if (parent->IsFrameWnd() && !m_pDockSite) {
        try {
            RegisterControlBar(static_cast<CFrameWnd*>(parent), this);
            m_pDockSite = parent;
        } catch (...) {
            CControlBar::DestroyWindow();
            throw;
        }
    }
    return TRUE;
}

BOOL CControlBar::DestroyWindow() {
    if (!m_hWnd) return CWnd::DestroyWindow();
    const bool isDockBar = IsKindOf(impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ());
    if (IsFloatingBar(isDockBar ? this : m_pDockBar)) {
        auto* frame = GetParentFrame();
        if (!frame) frame = static_cast<CFrameWnd*>(m_pDockSite);
        if (frame) return frame->DestroyWindow();
    }
    if (CWnd::FromHandlePermanent(m_hWnd) == this)
        return CWnd::DestroyWindow();

    if (!::DestroyWindow(m_hWnd)) return FALSE;
    m_hWnd = nullptr;
    return TRUE;
}

void CControlBar::PostNcDestroy() {
    if (m_pDockSite) {
        UnregisterControlBar(static_cast<CFrameWnd*>(m_pDockSite), this);
        m_pDockSite = nullptr;
    }
    if (m_bAutoDelete) delete this;
}

// Symbol: ?DestroyWindow@CControlBar@@UEAAHXZ
extern "C" BOOL impl__DestroyWindow_CControlBar__UEAAHXZ(CControlBar* bar) {
    return bar->CControlBar::DestroyWindow();
}

// Symbol: ?PostNcDestroy@CControlBar@@UEAAXXZ
extern "C" void impl__PostNcDestroy_CControlBar__UEAAXXZ(CControlBar* bar) {
    bar->CControlBar::PostNcDestroy();
}
