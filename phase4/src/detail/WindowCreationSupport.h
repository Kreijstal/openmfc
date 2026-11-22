#pragma once
#include "detail/WindowMessageSupport.h"
LRESULT CALLBACK AfxWndProc(HWND,UINT,WPARAM,LPARAM);
namespace openmfc { namespace detail {
class WindowCreationOwner {
public:
    explicit WindowCreationOwner(CWnd* window);
    ~WindowCreationOwner();
    WindowCreationOwner(const WindowCreationOwner&) = delete;
    WindowCreationOwner& operator=(const WindowCreationOwner&) = delete;
    CWnd* Release();
    static void OnDestroy(CWnd* window);

private:
    CWnd* window_;
    WindowCreationOwner* previous_;
    static thread_local WindowCreationOwner* current_;
};

LRESULT CALLBACK WindowCreationHook(int,WPARAM,LPARAM);
WNDPROC ModuleWindowProcedure();
extern ATOM menuWindowAtom;
LRESULT CALLBACK ActivationWindowProcedure(HWND,UINT,WPARAM,LPARAM);
} }
