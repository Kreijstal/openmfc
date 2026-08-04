#pragma once
// Shared internals of the former ctrl_tooltip.cpp translation unit.
// Definitions live in detail/CToolTipCtrlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ctooltipctrl {} } }
using namespace openmfc::detail::ctooltipctrl;
// CToolTipCtrl exported wrappers.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

class CToolInfo;

namespace openmfc { namespace detail { namespace ctooltipctrl {
HWND ToolTipHwnd(const CToolTipCtrl* pThis);
} } }

namespace openmfc { namespace detail { namespace ctooltipctrl {
HWND ToolWindow(CWnd* pWnd);
} } }

namespace openmfc { namespace detail { namespace ctooltipctrl {
UINT ToolFlags(UINT_PTR nIDTool);
} } }

namespace openmfc { namespace detail { namespace ctooltipctrl {
UINT_PTR ToolId(CWnd* pWnd, UINT_PTR nIDTool);
} } }

namespace openmfc { namespace detail { namespace ctooltipctrl {
void FillToolInfo(TOOLINFOW* pInfo, CWnd* pWnd, UINT_PTR nIDTool);
} } }

namespace openmfc { namespace detail { namespace ctooltipctrl {
extern CRuntimeClass g_classCToolTipCtrl;
} } }















