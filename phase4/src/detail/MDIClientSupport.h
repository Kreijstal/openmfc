#pragma once

#include "openmfc/afxmfc.h"

namespace openmfc::detail::mdi {
CMDIFrameWnd*& DocumentFrameOverride();
void* CreateClientArea();
void DeleteClientArea(void* area);
bool AttachClientArea(void* area, HWND window);
int TabMode(void* area);
CMFCTabCtrl& Tabs(void* area);
const CObList& Groups(void* area);
void SetTabsVisible(void* area, bool visible);
}
