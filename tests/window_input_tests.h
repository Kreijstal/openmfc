#pragma once
#include "detail/WindowInputSupport.h"
#include <cmath>

// Replace only this executable's imported user32 entry points. Production
// dispatch still calls the real Windows API signatures and native virtuals.
extern "C" {
extern BOOL (WINAPI* __imp_GetGestureInfo)(HGESTUREINFO,PGESTUREINFO);
extern BOOL (WINAPI* __imp_CloseGestureInfoHandle)(HGESTUREINFO);
extern BOOL (WINAPI* __imp_GetTouchInputInfo)(HTOUCHINPUT,UINT,PTOUCHINPUT,int);
extern BOOL (WINAPI* __imp_CloseTouchInputHandle)(HTOUCHINPUT);
extern BOOL (WINAPI* __imp_RegisterTouchWindow)(HWND,ULONG);
extern BOOL (WINAPI* __imp_UnregisterTouchWindow)(HWND);
}
namespace window_input_tests {
template<class Fn> class Import {
    Fn& slot;Fn previous;DWORD protection=0;
public:
    Import(Fn& value,Fn replacement) : slot(value),previous(value) {
        if(!VirtualProtect(&slot,sizeof(slot),PAGE_READWRITE,&protection))std::abort();slot=replacement;
    }
    ~Import() { slot=previous;DWORD ignored;VirtualProtect(&slot,sizeof(slot),protection,&ignored); }
};
static GESTUREINFO gesture{};
static TOUCHINPUT touches[3]{};
static bool readGesture=true,readTouch=true;
static int gestureCloses=0,touchCloses=0,touchReads=0;
static UINT touchCount=0;
static BOOL registrationResult=TRUE;
static int registrations=0,unregistrations=0;
static DWORD registrationFlags=0;
static BOOL WINAPI Register(HWND,ULONG flags) { ++registrations;registrationFlags=flags;return registrationResult; }
static BOOL WINAPI Unregister(HWND) { ++unregistrations;return registrationResult; }
static BOOL WINAPI Gesture(HGESTUREINFO,PGESTUREINFO out) {
    check(out->cbSize==sizeof(GESTUREINFO),"gesture dispatch supplies the native GESTUREINFO size");
    if(!readGesture)return FALSE;*out=gesture;return TRUE;
}
static BOOL WINAPI CloseGesture(HGESTUREINFO) { ++gestureCloses;return TRUE; }
static BOOL WINAPI Touch(HTOUCHINPUT,UINT count,PTOUCHINPUT out,int size) {
    ++touchReads;touchCount=count;
    check(size==sizeof(TOUCHINPUT),"touch dispatch requests the native TOUCHINPUT stride");
    if(!readTouch)return FALSE;memcpy(out,touches,count*sizeof(TOUCHINPUT));return TRUE;
}
static BOOL WINAPI CloseTouch(HTOUCHINPUT) { ++touchCloses;return TRUE; }
class Window : public CWnd {
public:
    bool accept=true;
    int kind=0,calls=0,defaultCalls=0,reject=-1;
    CPoint first,last;
    LONG scalar=0;double angle=0;
    UINT index=0,total=0;
    TOUCHINPUT* input=nullptr;
    LRESULT DefWindowProcW(UINT message,WPARAM,LPARAM) override { ++defaultCalls;return -static_cast<LRESULT>(message); }
    BOOL OnTouchInput(CPoint point,int number,int count,TOUCHINPUT* value) override {
        ++calls;first=point;index=number;total=count;input=value;return number!=reject;
    }
    BOOL OnGestureZoom(CPoint point,LONG value) override { ++calls;kind=GID_ZOOM;first=point;scalar=value;return accept; }
    BOOL OnGesturePan(CPoint from,CPoint to) override { ++calls;kind=GID_PAN;first=from;last=to;return accept; }
    BOOL OnGestureRotate(CPoint point,double value) override { ++calls;kind=GID_ROTATE;first=point;angle=value;return accept; }
    BOOL OnGestureTwoFingerTap(CPoint point) override { ++calls;kind=GID_TWOFINGERTAP;first=point;return accept; }
    BOOL OnGesturePressAndTap(CPoint point,LONG value) override { ++calls;kind=GID_PRESSANDTAP;first=point;scalar=value;return accept; }
    ULONG GetGestureStatus(CPoint point) override { first=point;return 0xfedcba98; }
};
static LRESULT send(Window& window,UINT message,WPARAM wparam,LPARAM lparam) {
    return impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(&window,window.m_hWnd,message,wparam,lparam);
}
static void input() {
    Import gestureRead(__imp_GetGestureInfo,Gesture);
    Import gestureClose(__imp_CloseGestureInfoHandle,CloseGesture);
    Import touchRead(__imp_GetTouchInputInfo,Touch);
    Import touchClose(__imp_CloseTouchInputHandle,CloseTouch);
    Import registration(__imp_RegisterTouchWindow,Register);
    Import unregistration(__imp_UnregisterTouchWindow,Unregister);
    Window window;
    window.m_hWnd=CreateWindowExW(0,L"STATIC",L"",WS_OVERLAPPED,150,180,80,60,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    check(window.m_hWnd!=nullptr,"input callbacks use a real window for coordinate conversion");
    check(window.RegisterTouchWindow(TRUE,TWF_FINETOUCH) && window.IsTouchWindow() && registrations==1 && registrationFlags==TWF_FINETOUCH,
        "touch registration stores the native per-window BOOL and forwards registration flags");
    registrationResult=FALSE;
    check(!window.RegisterTouchWindow(TRUE,TWF_WANTPALM) && !window.IsTouchWindow() && registrations==2,
        "failed touch registration clears the previous native registration state");
    registrationResult=TRUE;window.RegisterTouchWindow(TRUE,0);registrationResult=FALSE;
    check(!window.RegisterTouchWindow(FALSE,0) && !window.IsTouchWindow() && unregistrations==1,
        "unregistering clears cached registration state even when the Windows call fails");
    registrationResult=TRUE;
    POINT origin{0,0};ClientToScreen(window.m_hWnd,&origin);
    for(UINT i=0;i<3;++i) { touches[i].x=(origin.x+10+static_cast<LONG>(i))*100;touches[i].y=(origin.y+20)*100;touches[i].dwID=100+i; }
    check(impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(&window,3,touches) && window.calls==3 &&
        window.first.x==12 && window.first.y==20 && window.index==2 && window.total==3 && window.input==&touches[2] && touches[2].x==(origin.x+12)*100,
        "native touch callbacks receive client coordinates, ordered indexes, total count and the unchanged original records");
    window.calls=0;window.reject=1;
    check(!impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(&window,3,touches) && window.calls==2 && window.index==1,
        "touch dispatch stops immediately when a client callback rejects an input");
    check(impl__OnTouchInputs_CWnd__MEAAHIPEAUtagTOUCHINPUT___Z(&window,0,nullptr),"empty touch batches are handled without invoking a callback");
    window.reject=-1;window.calls=0;
    check(send(window,WM_TOUCH,MAKEWPARAM(3,0xbeef),0x11)==0 && window.calls==3 && touchCount==3 && touchCloses==1,
        "WM_TOUCH reads the LOWORD contact count and closes a successfully consumed input handle");
    window.reject=0;
    check(send(window,WM_TOUCH,1,0x12)==-WM_TOUCH && touchCloses==2,
        "declined touch batches close their input handle and invoke the client's default procedure");
    readTouch=false;int before=touchCloses;
    check(send(window,WM_TOUCH,1,0x13)==-WM_TOUCH && touchCloses==before,
        "failed touch reads leave handle processing to the default window procedure");
    readTouch=true;before=touchReads;
    check(send(window,WM_TOUCH,MAKEWPARAM(0,7),0x14)==-WM_TOUCH && touchReads==before,
        "zero-contact touch messages skip the Windows input reader");
    check(send(window,WM_TABLET_QUERYSYSTEMGESTURESTATUS,0,MAKELPARAM(origin.x-4,origin.y+8))==0xfedcba98 &&
        window.first.x==-4 && window.first.y==8,
        "tablet gesture status decodes signed screen coordinates and preserves the ULONG return value");
    gesture={};gesture.cbSize=sizeof(gesture);gesture.hwndTarget=window.m_hWnd;
    gesture.ptsLocation={static_cast<SHORT>(origin.x+30),static_cast<SHORT>(origin.y+40)};
    gesture.dwID=GID_BEGIN;gesture.ullArguments=100;
    check(send(window,WM_GESTURE,0,0x21)==-WM_GESTURE && gestureCloses==0 && window.m_ptGestureFrom.x==30 && window.m_ulGestureArg==100,
        "gesture begin initializes native per-window state and retains default handle processing");
    auto* buffer=window.GetCurrentGestureInfo();gesture.dwID=GID_ZOOM;gesture.ullArguments=145;
    check(send(window,WM_GESTURE,0,0x22)==0 && window.kind==GID_ZOOM && window.scalar==45 && gestureCloses==1 &&
        buffer==window.GetCurrentGestureInfo() && buffer->dwID==GID_ZOOM && window.m_ulGestureArg==145,
        "zoom dispatch uses the native argument delta, reuses GESTUREINFO and closes handled gestures");
    gesture.dwID=GID_PAN;gesture.ptsLocation.x+=5;gesture.ptsLocation.y-=7;
    check(send(window,WM_GESTURE,0,0x23)==0 && window.kind==GID_PAN && window.first.x==30 && window.first.y==40 &&
        window.last.x==35 && window.last.y==33 && window.m_ptGestureFrom.x==35,
        "pan overrides receive previous and current client points before native state advances");
    gesture.dwID=GID_ROTATE;gesture.ullArguments=32768;
    check(send(window,WM_GESTURE,0,0x24)==0 && window.kind==GID_ROTATE &&
        std::abs(window.angle-(32768.0/65535.0*4.0*3.14159265-6.2831853))<1e-12,
        "rotation dispatch converts the encoded argument into the native double angle");
    gesture.dwID=GID_TWOFINGERTAP;
    check(send(window,WM_GESTURE,0,0x25)==0 && window.kind==GID_TWOFINGERTAP,"two-finger taps reach their native virtual override");
    gesture.dwID=GID_PRESSANDTAP;gesture.ullArguments=0x1fffffff8;
    check(send(window,WM_GESTURE,0,0x26)==0 && window.kind==GID_PRESSANDTAP && window.scalar==-8,
        "press-and-tap dispatch preserves the low DWORD as a signed native long");
    window.accept=false;gesture.dwID=GID_ZOOM;gesture.ullArguments=0x1fffffff0;
    before=gestureCloses;
    check(send(window,WM_GESTURE,0,0x27)==-WM_GESTURE && gestureCloses==before && window.scalar==-8 && window.m_ulGestureArg==gesture.ullArguments,
        "rejected gestures retain default handle ownership while updating native gesture history");
    window.accept=true;gesture.dwID=GID_END;
    check(send(window,WM_GESTURE,0,0x28)==-WM_GESTURE && window.m_ptGestureFrom.x==-1 && window.m_ptGestureFrom.y==-1 &&
        !window.m_ulGestureArg && !buffer->cbSize,"gesture end resets history and clears the retained native GESTUREINFO buffer");
    gesture.hwndTarget=nullptr;gesture.dwID=GID_ZOOM;before=window.calls;
    check(send(window,WM_GESTURE,0,0x29)==-WM_GESTURE && window.calls==before && !buffer->cbSize,
        "gesture records for a different target are cleared without invoking client callbacks");
    gesture.hwndTarget=window.m_hWnd;readGesture=false;
    check(send(window,WM_GESTURE,0,0x2a)==-WM_GESTURE && !buffer->cbSize,
        "failed gesture reads clear current gesture state and preserve default processing");
    readGesture=true;DestroyWindow(window.m_hWnd);window.m_hWnd=nullptr;
}
}
static void windowInput() { window_input_tests::input(); }
