#pragma once
// Shared internals of the former global_cfullscreenimpl.cpp translation unit.
// Definitions live in detail/CFullScreenImplSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cfullscreenimpl {} } }
using namespace openmfc::detail::cfullscreenimpl;
// OpenMFC — CFullScreenImpl real implementation
//
// Layout (harvested, cl.exe /d1reportSingleClassLayout, size(80)):
//   0  {vfptr}
//   8  CRect  m_rectFullScreenWindow
//  24  CMFCToolBar* m_pwndFullScreenBar
//  32  BOOL   m_bFullScreen
//  36  BOOL   m_bShowMenu
//  40  CRect  m_rectFramePrev
//  56  CFrameImpl*  m_pImpl
//  64  UINT   m_uiFullScreenID
//  68  BOOL   m_bTabsArea
//  72  CString m_strRegSection  (single ATL data pointer)
//
#include <windows.h>
#include <cstddef>
#include <unordered_map>
#include <vector>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// Layout-faithful mirror of CFullScreenImpl.
namespace openmfc { namespace detail { namespace cfullscreenimpl {
struct S_Cfullscreenimpl
{
    void*    vfptr;                    // 0
    RECT     m_rectFullScreenWindow;   // 8
    void*    m_pwndFullScreenBar;      // 24
    int      m_bFullScreen;            // 32
    int      m_bShowMenu;              // 36
    RECT     m_rectFramePrev;          // 40
    void*    m_pImpl;                  // 56  CFrameImpl*
    unsigned m_uiFullScreenID;         // 64
    int      m_bTabsArea;              // 68
    void*    m_strRegSection;          // 72  CString data ptr
};
} } }


// CWnd::m_hWnd lives at offset 64 for every CWnd-derived object (CFrameWnd).
namespace openmfc { namespace detail { namespace cfullscreenimpl {
HWND FrameHwnd(void* pFrame);
} } }

namespace openmfc { namespace detail { namespace cfullscreenimpl {
LONG RcW(const RECT& r);
} } }
namespace openmfc { namespace detail { namespace cfullscreenimpl {
LONG RcH(const RECT& r);
} } }
namespace openmfc { namespace detail { namespace cfullscreenimpl {
bool IsRectMatch(RECT child, RECT whole);
} } }

// Forward decls of the exported thunks used from the vtable / cross-calls.
extern "C" void MS_ABI impl___1CFullScreenImpl__UEAA_XZ(void* pThis);

// The single vtable slot MSVC emits for this class is the
// vector-deleting-destructor. It runs the real destructor and, when the
// low bit of the flags is set, frees the storage.
namespace openmfc { namespace detail { namespace cfullscreenimpl {
void* MS_ABI vdtor_Cfullscreenimpl(void* p, unsigned f);
} } }

namespace openmfc { namespace detail { namespace cfullscreenimpl {
extern void* const g_CFullScreenImpl_vtbl[1];
} } }

namespace openmfc { namespace detail { namespace cfullscreenimpl {
struct FullScreenPaneState
{
    HMENU menu = nullptr;
    bool menuStored = false;
    std::vector<HWND> hiddenBars;
};
} } }

namespace openmfc { namespace detail { namespace cfullscreenimpl {
extern std::unordered_map<void*, FullScreenPaneState> g_fullScreenState;
} } }

namespace openmfc { namespace detail { namespace cfullscreenimpl {
struct HidePanesContext
{
    HWND frame;
    FullScreenPaneState* state;
};
} } }

namespace openmfc { namespace detail { namespace cfullscreenimpl {
BOOL CALLBACK HidePanesEnumProc(HWND child, LPARAM lp);
} } }


// ---------------------------------------------------------------------------







