#pragma once

#include "openmfc/afxwin.h"
#include <memory>
#include <type_traits>

namespace openmfc::detail::olecore {
// Merged menus borrow their source popups until both sides have unmerged them.
using MenuResource = std::shared_ptr<std::remove_pointer_t<HMENU>>;
MenuResource GetOleMenuResource(const CDocument* document, bool server);
}

extern "C" HMENU __cdecl impl__AfxMergeMenus__YAPEAUHMENU____PEAU1_0PEAJHH_Z(
    HMENU shared, HMENU source, LONG* widths, int group, BOOL mergeHelp);
extern "C" void __cdecl impl__AfxUnmergeMenus__YAXPEAUHMENU____00_Z(
    HMENU shared, HMENU source, HMENU mergedHelp);
