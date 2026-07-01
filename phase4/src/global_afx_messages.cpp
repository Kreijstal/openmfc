// Global residuals: registered AFX window-message exports.
//
// MFC exposes these as DATA symbols initialized from RegisterWindowMessage.
// The generated .def already aliases the MSVC DATA exports to the impl__ names.

#include <windows.h>

namespace {

unsigned int RegisterAfxMessage(const wchar_t* name)
{
    return ::RegisterWindowMessageW(name);
}

} // namespace

// Symbol: ?AFX_WM_AFTER_TASKBAR_ACTIVATE@@3IA
extern "C" unsigned int impl__AFX_WM_AFTER_TASKBAR_ACTIVATE__3IA =
    RegisterAfxMessage(L"AFX_WM_AFTER_TASKBAR_ACTIVATE");

// Symbol: ?AFX_WM_CHANGE_ACTIVE_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGE_ACTIVE_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGE_ACTIVE_TAB");

// Symbol: ?AFX_WM_CHANGE_CURRENT_FOLDER@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGE_CURRENT_FOLDER__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGE_CURRENT_FOLDER");

// Symbol: ?AFX_WM_CHANGEVISUALMANAGER@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGEVISUALMANAGER__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGEVISUALMANAGER");

// Symbol: ?AFX_WM_CHANGING_ACTIVE_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_CHANGING_ACTIVE_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_CHANGING_ACTIVE_TAB");

// Symbol: ?AFX_WM_CHECKEMPTYMINIFRAME@@3IA
extern "C" unsigned int impl__AFX_WM_CHECKEMPTYMINIFRAME__3IA =
    RegisterAfxMessage(L"AFX_WM_CHECKEMPTYMINIFRAME");

// Symbol: ?AFX_WM_CREATETOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_CREATETOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_CREATETOOLBAR");

// Symbol: ?AFX_WM_CUSTOMIZEHELP@@3IA
extern "C" unsigned int impl__AFX_WM_CUSTOMIZEHELP__3IA =
    RegisterAfxMessage(L"AFX_WM_CUSTOMIZEHELP");

// Symbol: ?AFX_WM_CUSTOMIZETOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_CUSTOMIZETOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_CUSTOMIZETOOLBAR");

// Symbol: ?AFX_WM_DELETETOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_DELETETOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_DELETETOOLBAR");

// Symbol: ?AFX_WM_DRAW2D@@3IA
extern "C" unsigned int impl__AFX_WM_DRAW2D__3IA =
    RegisterAfxMessage(L"AFX_WM_DRAW2D");

// Symbol: ?AFX_WM_GETDOCUMENTCOLORS@@3IA
extern "C" unsigned int impl__AFX_WM_GETDOCUMENTCOLORS__3IA =
    RegisterAfxMessage(L"AFX_WM_GETDOCUMENTCOLORS");

// Symbol: ?AFX_WM_GETDRAGBOUNDS@@3IA
extern "C" unsigned int impl__AFX_WM_GETDRAGBOUNDS__3IA =
    RegisterAfxMessage(L"AFX_WM_GETDRAGBOUNDS");

// Symbol: ?AFX_WM_ON_AFTER_SHELL_COMMAND@@3IA
extern "C" unsigned int impl__AFX_WM_ON_AFTER_SHELL_COMMAND__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_AFTER_SHELL_COMMAND");

// Symbol: ?AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU@@3IA
extern "C" unsigned int impl__AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_BEFORE_SHOW_RIBBON_ITEM_MENU");

// Symbol: ?AFX_WM_ON_CANCELTABMOVE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_CANCELTABMOVE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_CANCELTABMOVE");

// Symbol: ?AFX_WM_ON_CHANGE_RIBBON_CATEGORY@@3IA
extern "C" unsigned int impl__AFX_WM_ON_CHANGE_RIBBON_CATEGORY__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_CHANGE_RIBBON_CATEGORY");

// Symbol: ?AFX_WM_ON_CLOSEPOPUPWINDOW@@3IA
extern "C" unsigned int impl__AFX_WM_ON_CLOSEPOPUPWINDOW__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_CLOSEPOPUPWINDOW");

// Symbol: ?AFX_WM_ON_DRAGCOMPLETE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_DRAGCOMPLETE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_DRAGCOMPLETE");

// Symbol: ?AFX_WM_ON_GET_TAB_TOOLTIP@@3IA
extern "C" unsigned int impl__AFX_WM_ON_GET_TAB_TOOLTIP__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_GET_TAB_TOOLTIP");

// Symbol: ?AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM@@3IA
extern "C" unsigned int impl__AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_HIGHLIGHT_RIBBON_LIST_ITEM");

// Symbol: ?AFX_WM_ON_HSCROLL@@3IA
extern "C" unsigned int impl__AFX_WM_ON_HSCROLL__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_HSCROLL");

// Symbol: ?AFX_WM_ON_MOVE_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_ON_MOVE_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_MOVE_TAB");

// Symbol: ?AFX_WM_ON_MOVETABCOMPLETE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_MOVETABCOMPLETE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_MOVETABCOMPLETE");

// Symbol: ?AFX_WM_ON_MOVETOTABGROUP@@3IA
extern "C" unsigned int impl__AFX_WM_ON_MOVETOTABGROUP__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_MOVETOTABGROUP");

// Symbol: ?AFX_WM_ON_PRESS_CLOSE_BUTTON@@3IA
extern "C" unsigned int impl__AFX_WM_ON_PRESS_CLOSE_BUTTON__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_PRESS_CLOSE_BUTTON");

// Symbol: ?AFX_WM_ON_RENAME_TAB@@3IA
extern "C" unsigned int impl__AFX_WM_ON_RENAME_TAB__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_RENAME_TAB");

// Symbol: ?AFX_WM_ON_RIBBON_CUSTOMIZE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_RIBBON_CUSTOMIZE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_RIBBON_CUSTOMIZE");

// Symbol: ?AFX_WM_ON_TABGROUPMOUSEMOVE@@3IA
extern "C" unsigned int impl__AFX_WM_ON_TABGROUPMOUSEMOVE__3IA =
    RegisterAfxMessage(L"AFX_WM_ON_TABGROUPMOUSEMOVE");

// Symbol: ?AFX_WM_POSTRECALCLAYOUT@@3IA
extern "C" unsigned int impl__AFX_WM_POSTRECALCLAYOUT__3IA =
    RegisterAfxMessage(L"AFX_WM_POSTRECALCLAYOUT");

// Symbol: ?AFX_WM_POSTSETPREVIEWFRAME@@3IA
extern "C" unsigned int impl__AFX_WM_POSTSETPREVIEWFRAME__3IA =
    RegisterAfxMessage(L"AFX_WM_POSTSETPREVIEWFRAME");

// Symbol: ?AFX_WM_PROPERTY_CHANGED@@3IA
extern "C" unsigned int impl__AFX_WM_PROPERTY_CHANGED__3IA =
    RegisterAfxMessage(L"AFX_WM_PROPERTY_CHANGED");

// Symbol: ?AFX_WM_RECREATED2DRESOURCES@@3IA
extern "C" unsigned int impl__AFX_WM_RECREATED2DRESOURCES__3IA =
    RegisterAfxMessage(L"AFX_WM_RECREATED2DRESOURCES");

// Symbol: ?AFX_WM_RESETCONTEXTMENU@@3IA
extern "C" unsigned int impl__AFX_WM_RESETCONTEXTMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETCONTEXTMENU");

// Symbol: ?AFX_WM_RESETKEYBOARD@@3IA
extern "C" unsigned int impl__AFX_WM_RESETKEYBOARD__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETKEYBOARD");

// Symbol: ?AFX_WM_RESETMENU@@3IA
extern "C" unsigned int impl__AFX_WM_RESETMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETMENU");

// Symbol: ?AFX_WM_RESETRPROMPT@@3IA
extern "C" unsigned int impl__AFX_WM_RESETRPROMPT__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETRPROMPT");

// Symbol: ?AFX_WM_RESETTOOLBAR@@3IA
extern "C" unsigned int impl__AFX_WM_RESETTOOLBAR__3IA =
    RegisterAfxMessage(L"AFX_WM_RESETTOOLBAR");

// Symbol: ?AFX_WM_SHOWREGULARMENU@@3IA
extern "C" unsigned int impl__AFX_WM_SHOWREGULARMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_SHOWREGULARMENU");

// Symbol: ?AFX_WM_TOOLBARMENU@@3IA
extern "C" unsigned int impl__AFX_WM_TOOLBARMENU__3IA =
    RegisterAfxMessage(L"AFX_WM_TOOLBARMENU");

// Symbol: ?AFX_WM_UPDATETOOLTIPS@@3IA
extern "C" unsigned int impl__AFX_WM_UPDATETOOLTIPS__3IA =
    RegisterAfxMessage(L"AFX_WM_UPDATETOOLTIPS");

// Symbol: ?AFX_WM_WINDOW_HELP@@3IA
extern "C" unsigned int impl__AFX_WM_WINDOW_HELP__3IA =
    RegisterAfxMessage(L"AFX_WM_WINDOW_HELP");
