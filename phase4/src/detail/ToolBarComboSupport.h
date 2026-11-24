#pragma once

#include "openmfc/afxmfc.h"

namespace openmfc::detail {

bool AttachToolBarComboEditor(CEdit& edit);
CComboBox* CreateToolBarFontWindow(CWnd* parent, const RECT& bounds, DWORD style, UINT id);
bool DestroyToolBarFontWindow(CWnd* window);
void RefreshToolBarComboItems(CMFCToolBarComboBoxButton& button);
void ReleaseToolBarComboWindows(CMFCToolBarComboBoxButton& button);
bool DrawToolBarFontComboItem(CMFCToolBarComboBoxButton& button, DRAWITEMSTRUCT& item);
bool IsFlatToolBarCombo(const CMFCToolBarComboBoxButton& button);
bool HitToolBarComboArrow(const CMFCToolBarComboBoxButton& button, POINT point);
bool HandleToolBarComboCommand(CMFCToolBarComboBoxButton& button, HWND source, int code);

}  // namespace openmfc::detail
