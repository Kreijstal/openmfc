#pragma once
// Shared internals of the former ctrl_checklist.cpp translation unit.
// Definitions live in detail/CCheckListBoxSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cchecklistbox {} } }
using namespace openmfc::detail::cchecklistbox;
// CCheckListBox implementation
//
// This file is intentionally isolated so checklist work can be owned and
// reviewed independently from the broader control-core implementation.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include <windows.h>
#include <oleauto.h>
#include <oleacc.h>
#include <unordered_map>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace openmfc { namespace detail { namespace cchecklistbox {
using ItemStateMap = std::unordered_map<int, int>;
} } }
namespace openmfc { namespace detail { namespace cchecklistbox {
extern std::unordered_map<const CCheckListBox*, ItemStateMap> g_checkStates;
} } }
namespace openmfc { namespace detail { namespace cchecklistbox {
extern std::unordered_map<const CCheckListBox*, ItemStateMap> g_itemEnabledStates;
} } }
namespace openmfc { namespace detail { namespace cchecklistbox {
extern std::unordered_map<const CCheckListBox*, unsigned int> g_checkStyles;
} } }

namespace openmfc { namespace detail { namespace cchecklistbox {
int ListBoxItemFromPoint_CtrlChecklist(HWND hWnd, CPoint pt);
} } }

namespace openmfc { namespace detail { namespace cchecklistbox {
CRect CheckListBoxCheckRect(const RECT& itemRect);
} } }

namespace openmfc { namespace detail { namespace cchecklistbox {
int CheckListBoxMinimumItemHeight(HWND hWnd);
} } }

namespace openmfc { namespace detail { namespace cchecklistbox {
int CheckListBoxVariantIndex(const VARIANT& varChild);
} } }

namespace openmfc { namespace detail { namespace cchecklistbox {
LRESULT CheckListBoxSend(CCheckListBox* pThis, UINT message, WPARAM wParam, LPARAM lParam);
} } }


















































