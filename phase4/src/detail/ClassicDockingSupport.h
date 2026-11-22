#pragma once

#include "openmfc/afxwin.h"

class CMiniDockFrameWnd;

extern "C" int impl__Create_CDockBar__UEAAHPEAVCWnd__KI_Z(void*, CWnd*, DWORD, UINT);
extern "C" void impl__DockControlBar_CDockBar__QEAAXPEAVCControlBar__PEBUtagRECT___Z(
    void*, CControlBar*, const RECT*);
extern "C" int impl__GetDockedVisibleCount_CDockBar__UEBAHXZ(const void*);
extern "C" CRuntimeClass* impl__GetThisClass_CDockBar__SAPEAUCRuntimeClass__XZ();

namespace openmfc::detail::dock {
constexpr UINT floatingBarId = 0xe81f;
constexpr UINT topBarId = 0xe81b;
constexpr UINT leftBarId = 0xe81c;
constexpr UINT rightBarId = 0xe81d;
constexpr UINT bottomBarId = 0xe81e;
constexpr DWORD alignLeft = 0x1000;
constexpr DWORD alignTop = 0x2000;
constexpr DWORD alignRight = 0x4000;
constexpr DWORD alignBottom = 0x8000;
constexpr DWORD borderLeft = 0x0100;
constexpr DWORD borderTop = 0x0200;
constexpr DWORD borderRight = 0x0400;
constexpr DWORD borderBottom = 0x0800;

CControlBar* NewDockBar();
bool IsFloatingBar(const void*);
CMiniDockFrameWnd* CreateFloatingFrame(CFrameWnd* owner, DWORD style);
CFrameWnd* FrameWindow(CMiniDockFrameWnd*);
CControlBar* FloatingDockBar(CMiniDockFrameWnd*);
}
