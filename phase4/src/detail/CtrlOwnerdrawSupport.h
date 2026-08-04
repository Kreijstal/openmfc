#pragma once
// Shared internals of the former ctrl_ownerdraw.cpp translation unit.
// Definitions live in detail/CtrlOwnerdrawSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ctrlownerdraw {} } }
using namespace openmfc::detail::ctrlownerdraw;
// Standard control owner-draw and runtime-class exported wrappers.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <commctrl.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__VKeyToItem_CListBox__UEAAHII_Z(CListBox* pThis, UINT nKey, UINT nIndex);

namespace openmfc { namespace detail { namespace ctrlownerdraw {
void FillOwnerDrawItem(DRAWITEMSTRUCT* pDraw);
} } }

namespace openmfc { namespace detail { namespace ctrlownerdraw {
int ListBoxItemFromPoint_CtrlOwnerdraw(HWND hWnd, CPoint pt, int* pOutside);
} } }

namespace openmfc { namespace detail { namespace ctrlownerdraw {
HWND TreeCtrlHwnd(const CTreeCtrl* pThis);
} } }

namespace openmfc { namespace detail { namespace ctrlownerdraw {
BOOL TreeCtrlGetItem(HWND hWnd, HTREEITEM hItem, UINT mask, TVITEMW* pItem);
} } }

namespace openmfc { namespace detail { namespace ctrlownerdraw {
BOOL TreeCtrlGetItemEx(HWND hWnd, HTREEITEM hItem, UINT mask, TVITEMEXW* pItem);
} } }









































































