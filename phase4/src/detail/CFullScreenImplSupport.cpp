#include "CFullScreenImplSupport.h"

static_assert(sizeof(S_Cfullscreenimpl) == 80, "CFullScreenImpl must be 80 bytes");
static_assert(offsetof(S_Cfullscreenimpl, m_rectFullScreenWindow) == 8,  "m_rectFullScreenWindow @8");
static_assert(offsetof(S_Cfullscreenimpl, m_bFullScreen)          == 32, "m_bFullScreen @32");
static_assert(offsetof(S_Cfullscreenimpl, m_pImpl)                == 56, "m_pImpl @56");
static_assert(offsetof(S_Cfullscreenimpl, m_strRegSection)        == 72, "m_strRegSection @72");
namespace openmfc { namespace detail { namespace cfullscreenimpl {
HWND FrameHwnd(void* pFrame)
{
    if (!pFrame) return NULL;
    return *reinterpret_cast<HWND*>(reinterpret_cast<char*>(pFrame) + 64);
}
LONG RcW(const RECT& r) { return r.right  - r.left; }
LONG RcH(const RECT& r) { return r.bottom - r.top;  }
bool IsRectMatch(RECT child, RECT whole)
{
    return child.left <= whole.left && child.top <= whole.top &&
           child.right >= whole.right && child.bottom >= whole.bottom;
}
void* MS_ABI vdtor_Cfullscreenimpl(void* p, unsigned f)
{
    impl___1CFullScreenImpl__UEAA_XZ(p);
    if (f & 1) ::operator delete(p);
    return p;
}

std::unordered_map<void*, FullScreenPaneState> g_fullScreenState;
BOOL CALLBACK HidePanesEnumProc(HWND child, LPARAM lp)
{
    auto* ctx = reinterpret_cast<HidePanesContext*>(lp);
    if (!ctx || !ctx->state || !ctx->frame || child == ctx->frame || !::IsWindow(child)) {
        return TRUE;
    }

    if (!::IsWindowVisible(child)) return TRUE;

    wchar_t cls[64] = {};
    int clsLen = ::GetClassNameW(child, cls, static_cast<int>(sizeof(cls) / sizeof(cls[0])));
    if (clsLen > 0 && ::wcscmp(cls, L"MDIClient") == 0) return TRUE;

    RECT childRect{};
    if (!::GetWindowRect(child, &childRect)) return TRUE;

    RECT client{};
    if (!::GetClientRect(ctx->frame, &client)) return TRUE;
    POINT ptClientMin{client.left, client.top};
    POINT ptClientMax{client.right, client.bottom};
    ::ClientToScreen(ctx->frame, &ptClientMin);
    ::ClientToScreen(ctx->frame, &ptClientMax);
    RECT clientScreen{ptClientMin.x, ptClientMin.y, ptClientMax.x, ptClientMax.y};
    if (IsRectMatch(childRect, clientScreen)) return TRUE;

    ctx->state->hiddenBars.push_back(child);
    ::ShowWindow(child, SW_HIDE);
    return TRUE;
}
} } }  // namespace openmfc::detail::cfullscreenimpl
