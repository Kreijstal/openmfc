#pragma once
#include <windows.h>
namespace openmfc { namespace detail {
// Window, class-info and library wrappers share the module's cached activation
// context and preserve failed API last-error values.
struct WindowActivationState {
    BOOL cleanupStarted=FALSE;
    BOOL bypass=FALSE;
    BOOL unavailable=FALSE;
    HANDLE context=INVALID_HANDLE_VALUE;
};
extern WindowActivationState windowActivation;
BOOL InitializeWindowActivation();
BOOL ActivateWindowContext(ULONG_PTR*);
HWND ContextCreateWindow(CREATESTRUCTW);
BOOL ContextMenuClassInfo(WNDCLASSEXW*);
HMODULE ContextLoadLibrary(const wchar_t*,HANDLE,DWORD);
} }
