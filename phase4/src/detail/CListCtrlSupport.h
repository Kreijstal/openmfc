#pragma once
// Shared internals of the former ctrl_listctrl.cpp translation unit.
// Definitions live in detail/CListCtrlSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace clistctrl {} } }
using namespace openmfc::detail::clistctrl;
// CListCtrl exported wrappers
//
// Keep list-view exports in their own shard so future common-control work can
// be assigned by file without touching the shared control core.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

class CHeaderCtrl;

namespace openmfc { namespace detail { namespace clistctrl {
HWND ListCtrlHwnd(const CListCtrl* pThis);
} } }

namespace openmfc { namespace detail { namespace clistctrl {
BOOL ListCtrlGetItem(HWND hWnd, int nItem, UINT mask, LVITEMW* pItem);
} } }
































