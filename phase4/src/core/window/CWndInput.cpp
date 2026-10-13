// Native input dispatch, decoded from mfc140.dll 0x28ac20..0x28b2bf.
#define OPENMFC_APPCORE_IMPL
#include "detail/WindowInputSupport.h"
#include <cstdlib>
#include <cstring>
#include <memory>
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
static_assert(offsetof(CWnd,m_ptGestureFrom)==0x74,"native gesture origin");
static_assert(offsetof(CWnd,m_ulGestureArg)==0x80,"native gesture argument");
static_assert(offsetof(CWnd,m_pCurrentGestureInfo)==0x90,"native gesture buffer");
namespace {
LRESULT Default(CWnd* window) {
    const MSG* message=CWnd::GetCurrentMessage();
    using Procedure=LRESULT (MS_ABI*)(void*,UINT,WPARAM,LPARAM);
    return openmfc::detail::WindowSlot<Procedure>(window,73)(window,message->message,message->wParam,message->lParam);
}
struct Free { void operator()(void* memory) const { std::free(memory); } };
}
// Symbol: ?OnTouchInputs@CWnd@@MEAAHIPEAUtagTOUCHINPUT@@@Z
extern "C" BOOL MS_ABI impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(CWnd* window,UINT count,TOUCHINPUT* inputs) {
    using Handler=BOOL (MS_ABI*)(void*,CPoint,int,int,TOUCHINPUT*);
    for(UINT index=0;index<count;++index) {
        POINT point{inputs[index].x/100,inputs[index].y/100};
        ScreenToClient(window->m_hWnd,&point);
        if(!openmfc::detail::WindowSlot<Handler>(window,77)(window,CPoint(point.x,point.y),index,count,&inputs[index]))return FALSE;
    }
    return TRUE;
}
// Symbol: ?OnTouchMessage@CWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnTouchMessage_CWnd__IEAA_J_K_J_Z(CWnd* window,WPARAM wparam,LPARAM lparam) {
    UINT count=LOWORD(wparam);
    if(!count)return Default(window);
    std::unique_ptr<void,Free> storage(std::malloc(count*sizeof(TOUCHINPUT)));
    if(!storage) { impl__AfxThrowMemoryException__YAXXZ();return Default(window); }
    auto* inputs=static_cast<TOUCHINPUT*>(storage.get());
    HTOUCHINPUT handle=reinterpret_cast<HTOUCHINPUT>(lparam);
    if(!GetTouchInputInfo(handle,count,inputs,sizeof(TOUCHINPUT)))return Default(window);
    using Handler=BOOL (MS_ABI*)(void*,UINT,TOUCHINPUT*);
    BOOL handled=openmfc::detail::WindowSlot<Handler>(window,76)(window,count,inputs);
    storage.reset();CloseTouchInputHandle(handle);
    return handled ? 0 : Default(window);
}
// Symbol: ?OnTabletQuerySystemGestureStatus@CWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnTabletQuerySystemGestureStatus_CWnd__IEAA_J_K_J_Z(CWnd* window,WPARAM,LPARAM lparam) {
    POINT point{static_cast<short>(LOWORD(lparam)),static_cast<short>(HIWORD(lparam))};
    ScreenToClient(window->m_hWnd,&point);
    using Status=ULONG (MS_ABI*)(void*,CPoint);
    return openmfc::detail::WindowSlot<Status>(window,78)(window,CPoint(point.x,point.y));
}
// Symbol: ?OnGesture@CWnd@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnGesture_CWnd__IEAA_J_K_J_Z(CWnd* window,WPARAM,LPARAM lparam) {
    if(!window->m_pCurrentGestureInfo) {
        window->m_pCurrentGestureInfo=static_cast<GESTUREINFO*>(std::malloc(sizeof(GESTUREINFO)));
        if(!window->m_pCurrentGestureInfo) { impl__AfxThrowMemoryException__YAXXZ();return Default(window); }
    }
    auto* info=window->m_pCurrentGestureInfo;
    std::memset(info,0,sizeof(*info));info->cbSize=sizeof(*info);
    HGESTUREINFO handle=reinterpret_cast<HGESTUREINFO>(lparam);
    if(!GetGestureInfo(handle,info) || info->hwndTarget!=window->m_hWnd) {
        std::memset(info,0,sizeof(*info));return Default(window);
    }
    POINT point{info->ptsLocation.x,info->ptsLocation.y};ScreenToClient(window->m_hWnd,&point);
    if(info->dwID==GID_BEGIN) {
        window->m_ptGestureFrom=point;window->m_ulGestureArg=info->ullArguments;return Default(window);
    }
    if(info->dwID==GID_END) {
        window->m_ptGestureFrom={-1,-1};window->m_ulGestureArg=0;
        std::memset(info,0,sizeof(*info));return Default(window);
    }
    BOOL handled=FALSE;CPoint current(point.x,point.y);
    using Scalar=BOOL (MS_ABI*)(void*,CPoint,LONG);
    switch(info->dwID) {
    case GID_ZOOM: {
        LONG delta=static_cast<LONG>(static_cast<DWORD>(info->ullArguments)-static_cast<DWORD>(window->m_ulGestureArg));
        handled=openmfc::detail::WindowSlot<Scalar>(window,79)(window,current,delta);break;
    }
    case GID_PAN: {
        using Pan=BOOL (MS_ABI*)(void*,CPoint,CPoint);
        handled=openmfc::detail::WindowSlot<Pan>(window,80)(window,CPoint(window->m_ptGestureFrom.x,window->m_ptGestureFrom.y),current);break;
    }
    case GID_ROTATE: {
        using Rotate=BOOL (MS_ABI*)(void*,CPoint,double);
        double angle=static_cast<double>(info->ullArguments)/65535.0*4.0*3.14159265-6.2831853;
        handled=openmfc::detail::WindowSlot<Rotate>(window,81)(window,current,angle);break;
    }
    case GID_TWOFINGERTAP: {
        using Tap=BOOL (MS_ABI*)(void*,CPoint);
        handled=openmfc::detail::WindowSlot<Tap>(window,82)(window,current);break;
    }
    case GID_PRESSANDTAP:
        handled=openmfc::detail::WindowSlot<Scalar>(window,83)(window,current,static_cast<LONG>(info->ullArguments));break;
    }
    if(handled)CloseGestureInfoHandle(handle);
    window->m_ptGestureFrom=point;window->m_ulGestureArg=info->ullArguments;
    return handled ? 0 : Default(window);
}
LRESULT CWnd::OnGesture(WPARAM wparam,LPARAM lparam) { return impl__OnGesture_CWnd__IEAA_J_K_J_Z(this,wparam,lparam); }
LRESULT CWnd::OnTouchMessage(ULONGLONG wparam,LONGLONG lparam) { return impl__OnTouchMessage_CWnd__IEAA_J_K_J_Z(this,wparam,lparam); }
LRESULT CWnd::OnTabletQuerySystemGestureStatus(ULONGLONG wparam,LONGLONG lparam) { return impl__OnTabletQuerySystemGestureStatus_CWnd__IEAA_J_K_J_Z(this,wparam,lparam); }

ULONG CWnd::GetGestureStatus(CPoint) { return TABLET_DISABLE_PRESSANDHOLD; }
// Symbol: ?GetGestureStatus@CWnd@@MEAAKVCPoint@@@Z
extern "C" ULONG MS_ABI impl__GetGestureStatus_CWnd__MEAAKVCPoint___Z(CWnd*,CPoint) { return TABLET_DISABLE_PRESSANDHOLD; }

// Symbol: ?RegisterTouchWindow@CWnd@@QEAAHHK@Z
extern "C" BOOL MS_ABI impl__RegisterTouchWindow_CWnd__QEAAHHK_Z(CWnd* window,BOOL enable,DWORD flags) {
    window->m_bIsTouchWindowRegistered=FALSE;
    if(!enable)return ::UnregisterTouchWindow(window->m_hWnd);
    window->m_bIsTouchWindowRegistered=::RegisterTouchWindow(window->m_hWnd,flags);
    return window->m_bIsTouchWindowRegistered;
}
BOOL CWnd::RegisterTouchWindow(BOOL enable,DWORD flags) { return impl__RegisterTouchWindow_CWnd__QEAAHHK_Z(this,enable,flags); }
int CWnd::IsTouchWindow() const { return m_bIsTouchWindowRegistered; }
// Symbol: ?IsTouchWindow@CWnd@@QEBAHXZ
extern "C" BOOL MS_ABI impl__IsTouchWindow_CWnd__QEBAHXZ(const CWnd* window) { return window->m_bIsTouchWindowRegistered; }
