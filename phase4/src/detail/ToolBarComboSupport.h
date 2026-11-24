#pragma once

#include "openmfc/afxmfc.h"

namespace openmfc::detail {

bool AttachToolBarComboEditor(CEdit& edit);
bool IsFlatToolBarCombo(const CMFCToolBarComboBoxButton& button);
bool HitToolBarComboArrow(const CMFCToolBarComboBoxButton& button, POINT point);
bool HandleToolBarComboCommand(CMFCToolBarComboBoxButton& button, HWND source, int code);

}  // namespace openmfc::detail
