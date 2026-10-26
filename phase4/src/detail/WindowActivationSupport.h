#pragma once
#include <windows.h>
namespace openmfc { namespace detail {
// Private isolation-aware state. Window and class-info wrappers share the
// module's cached activation context and preserve failed API last-error values.
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
} }
