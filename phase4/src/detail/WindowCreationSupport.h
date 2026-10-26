#pragma once
#include "detail/WindowMessageSupport.h"
LRESULT CALLBACK AfxWndProc(HWND,UINT,WPARAM,LPARAM);
namespace openmfc { namespace detail {
LRESULT CALLBACK WindowCreationHook(int,WPARAM,LPARAM);
WNDPROC ModuleWindowProcedure();
extern ATOM menuWindowAtom;
LRESULT CALLBACK ActivationWindowProcedure(HWND,UINT,WPARAM,LPARAM);
} }
