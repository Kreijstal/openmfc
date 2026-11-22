#pragma once

#include "openmfc/afxmfc.h"

namespace openmfc::detail {
void BeginDockDrag(CMFCDragFrameImpl& drag, CWnd& window, CDockingManager* manager, POINT anchor);
void UpdateDockDragPreview(CMFCDragFrameImpl& drag, bool showFloatingOutline, bool force);
bool DockDragBounds(const CMFCDragFrameImpl& drag, RECT& bounds);
}

extern "C" void AFXAPI impl__EndDrawDragFrame_CMFCDragFrameImpl__QEAAXH_Z(void*, int);
extern "C" void AFXAPI impl__ResetState_CMFCDragFrameImpl__QEAAXXZ(void*);
