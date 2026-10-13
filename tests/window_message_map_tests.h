#pragma once
#include "detail/CommandMessageDispatch.h"
extern "C" int MS_ABI impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(CWnd*,UINT,WPARAM,LPARAM,LRESULT*);
extern "C" CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC);
extern "C" void MS_ABI impl__AfxResetMsgCache__YAXXZ();
extern "C" CPoint* MS_ABI impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(CDC*,CPoint*,int,int);
namespace window_map_tests {
extern "C" BOOL (WINAPI* __imp_DeleteDC)(HDC);
static auto nativeDeleteDC=__imp_DeleteDC;
static int dcDeletes=0;
static HDC deletedDC=nullptr;
static BOOL dcDeleted=FALSE;
static BOOL WINAPI DeleteOwnedDC(HDC dc) { ++dcDeletes;deletedDC=dc;return dcDeleted=nativeDeleteDC(dc); }
static UINT registered=0;
static CWnd* wrapped=nullptr;
static CWnd* Lookup(HWND handle) { return wrapped && wrapped->m_hWnd==handle ? wrapped : nullptr; }
class Base : public CWnd {
    DECLARE_MESSAGE_MAP()
public:
    int calls=0,kind=0,resizeCalls=0;
    UINT a=0,b=0,c=0,d=0;
    UINT_PTR pointer=0;
    CPoint point;
    CWnd* window=nullptr;
    BOOL accept=TRUE;
    LRESULT reply=-73;
    CDC* colorDC=nullptr;
    HWND colored=nullptr;
    CMenu* menu=nullptr;
    void ResizeDynamicLayout() override { ++resizeCalls; }
    BOOL OnCommand(WPARAM value,LPARAM detail) override { ++calls;pointer=value;reply=detail;return accept; }
    BOOL OnNotify(WPARAM value,LPARAM detail,LRESULT* result) override { ++calls;pointer=value;window=reinterpret_cast<CWnd*>(detail);*result=reply;return accept; }
    void OnMove(int x,int y) { ++calls;kind=1;point=CPoint(x,y); }
    void OnSize(UINT type,int width,int height) { ++calls;kind=2;a=type;b=width;c=height; }
    void OnTimer(UINT_PTR id) { ++calls;kind=3;pointer=id; }
    void OnPaint() { ++calls;kind=4; }
    void OnMouseMove(UINT flags,CPoint where) { ++calls;kind=5;a=flags;point=where; }
    BOOL OnMouseWheel(UINT flags,short delta,CPoint where) { ++calls;kind=6;a=flags;b=delta;point=where;return accept; }
    void OnXButtonDown(UINT flags,UINT button,CPoint where) { ++calls;kind=7;a=flags;b=button;point=where; }
    void OnRawInput(UINT code,HRAWINPUT handle) { ++calls;kind=8;a=code;pointer=reinterpret_cast<UINT_PTR>(handle); }
    void OnAppCommand(CWnd* target,UINT command,UINT device,UINT key) { ++calls;kind=9;window=target;a=command;b=device;c=key; }
    void OnContextMenu(CWnd* target,CPoint where) { ++calls;kind=10;window=target;point=where; }
    void OnKeyDown(UINT key,UINT repeat,UINT flags) { ++calls;kind=11;a=key;b=repeat;c=flags; }
    LRESULT OnNcHitTest(CPoint where) { ++calls;kind=12;point=where;return reply; }
    void OnSizing(UINT edge,RECT* rect) { ++calls;kind=13;a=edge;rect->left=-123; }
    HBRUSH OnCtlColor(CDC* dc,CWnd* child,UINT type) {
        ++calls;kind=14;colorDC=dc;colored=child->m_hWnd;a=type;
        check(dc->m_hAttribDC==nullptr && !dc->m_bPrinting && dc->GetRuntimeClass()==CDC::GetThisClass(),
            "control-color handlers receive a native temporary CDC with a borrowed output handle");
        check(child->GetRuntimeClass()==CWnd::GetThisClass(),"unmapped color controls use a native temporary CWnd");
        check(g_messageCacheLockDepth==0,"message callbacks run after releasing the native global cache lock");
        dc->SetBkColor(RGB(11,22,33));::SetPixel(dc->GetSafeHdc(),0,0,RGB(7,8,9));
        return reinterpret_cast<HBRUSH>(UINT_PTR(0x123456789));
    }
    LRESULT OnCustom(WPARAM value,LPARAM detail) { ++calls;kind=15;pointer=value;reply=detail;return detail; }
    void Command() { ++calls;kind=16; }
    void Range(UINT id) { ++calls;kind=17;a=id; }
    BOOL Extended(UINT id) { ++calls;kind=18;a=id;return accept; }
    void Notify(NMHDR* header,LRESULT* result) { ++calls;kind=19;pointer=header->idFrom;*result=reply; }
    void Update(CCmdUI* ui) { ++calls;kind=20;*reinterpret_cast<BOOL*>(reinterpret_cast<char*>(ui)+0x2c)=!accept; }
};
BEGIN_MESSAGE_MAP(Base,CWnd)
    ON_WM_MOVE()
    ON_WM_SIZE()
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEWHEEL()
    ON_WM_XBUTTONDOWN()
    ON_WM_INPUT()
    ON_WM_APPCOMMAND()
    ON_WM_CONTEXTMENU()
    ON_WM_KEYDOWN()
    ON_WM_NCHITTEST()
    ON_WM_SIZING()
    ON_WM_CTLCOLOR()
    ON_MESSAGE(WM_APP+31,&Base::OnCustom)
    ON_REGISTERED_MESSAGE(registered,&Base::OnCustom)
    ON_COMMAND(41,&Base::Command)
    ON_COMMAND_RANGE(42,44,&Base::Range)
    ON_COMMAND_EX(45,&Base::Extended)
    ON_NOTIFY(7,46,&Base::Notify)
    ON_UPDATE_COMMAND_UI(47,&Base::Update)
END_MESSAGE_MAP()
class Derived : public Base {
    DECLARE_MESSAGE_MAP()
public:
    void OnPaint() { ++calls;kind=40; }
    const AFX_MSGMAP* Map() { return GetMessageMap(); }
};
BEGIN_MESSAGE_MAP(Derived,Base)
    ON_WM_PAINT()
END_MESSAGE_MAP()
static BOOL dispatch(CWnd& window,UINT message,WPARAM w,LPARAM l,LRESULT& result) {
    return impl__OnWndMsg_CWnd__MEAAHI_K_JPEA_J_Z(&window,message,w,l,&result);
}
static void macrosAndMessages() {
    static_assert(AfxSig_v_v_v==19 && AfxSig_l_w_l==14 && AfxSig_v_u_ii==26 && AfxSigCmd_v==58 && AfxSig_u_u_l==94,"native SDK signature numbers");
    Derived target;LRESULT result=999;
    check(dispatch(target,WM_PAINT,0,0,result) && target.kind==40 && result==0,"derived message maps take precedence over inherited handlers");
    check(dispatch(target,WM_MOVE,0,MAKELPARAM(-32768,-17),result) && target.kind==1 && target.point.x==-32768 && target.point.y==-17,
        "inherited WM_MOVE decodes signed coordinates");
    check(dispatch(target,WM_SIZE,SIZE_MAXIMIZED,MAKELPARAM(640,480),result) && target.kind==2 && target.a==SIZE_MAXIMIZED &&
        target.b==640 && target.c==480 && target.resizeCalls==1,"WM_SIZE performs native resize preprocessing before the inherited map handler");
    check(target.OnWndMsg(WM_TIMER,UINT_PTR(0x123456789abcdef0),0,&result) && target.kind==3 && target.pointer==UINT_PTR(0x123456789abcdef0),
        "public OnWndMsg uses the production map engine and preserves 64-bit timer IDs");
    check(dispatch(target,WM_MOUSEMOVE,MK_SHIFT,MAKELPARAM(-200,-300),result) && target.a==MK_SHIFT && target.point.x==-200 && target.point.y==-300,
        "mouse maps preserve flags and signed CPoint arguments");
    target.accept=FALSE;result=999;
    check(!dispatch(target,WM_MOUSEWHEEL,MAKEWPARAM(MK_CONTROL,-120),MAKELPARAM(-20,40),result) && result==999 && target.b==UINT(-120),
        "rejected wheel handlers preserve the caller result and permit default processing");
    target.accept=TRUE;
    check(dispatch(target,WM_MOUSEWHEEL,MAKEWPARAM(MK_CONTROL,120),MAKELPARAM(-20,40),result) && result==TRUE && target.a==MK_CONTROL && target.point.x==-20,
        "accepted wheel handlers return their native BOOL result");
    check(dispatch(target,WM_XBUTTONDOWN,MAKEWPARAM(MK_SHIFT,XBUTTON2),MAKELPARAM(-1,-2),result) && result==TRUE && target.b==XBUTTON2,
        "X-button handlers decode the button field and force the native handled result");
    check(dispatch(target,WM_INPUT,0x1234,LPARAM(0x123456789),result) && target.a==0x34 && target.pointer==0x123456789,
        "raw-input handlers use the native low-byte input code and full-width handle");
    CWnd child;child.m_hWnd=CreateWindowExW(0,L"STATIC",L"",WS_POPUP,0,0,40,40,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    wrapped=&child;g_messageWindowLookup=Lookup;
    check(dispatch(target,WM_CONTEXTMENU,reinterpret_cast<WPARAM>(child.m_hWnd),MAKELPARAM(-1,-1),result) && target.window==&child && target.point.x==-1,
        "window arguments resolve through the framework handle map");
    check(dispatch(target,WM_APPCOMMAND,reinterpret_cast<WPARAM>(child.m_hWnd),MAKELPARAM(0x2222,0x8123),result) &&
        result==TRUE && target.window==&child && target.a==0x123 && target.b==0x8000 && target.c==0x2222,
        "application commands decode command, device and key-state bitfields");
    g_messageWindowLookup=nullptr;wrapped=nullptr;
    check(dispatch(target,WM_KEYDOWN,'A',MAKELPARAM(3,0xdead),result) && target.a=='A' && target.b==3 && target.c==0xdead,
        "keyboard maps split repeat counts and flags without sign extension");
    target.reply=-73;
    check(dispatch(target,WM_NCHITTEST,0,MAKELPARAM(-50,60),result) && result==-73 && target.point.x==-50,
        "hit-test maps retain signed 64-bit LRESULT returns");
    RECT rect{};
    check(dispatch(target,WM_SIZING,WMSZ_LEFT,reinterpret_cast<LPARAM>(&rect),result) && result==TRUE && rect.left==-123,
        "sizing maps expose mutable rectangles and force a handled result");
    check(dispatch(target,WM_APP+31,UINT_PTR(0xfedcba9876543210),-900,result) && result==-900 && target.pointer==UINT_PTR(0xfedcba9876543210),
        "ON_MESSAGE preserves full-width WPARAM and LPARAM");
    registered=RegisterWindowMessageW(L"OpenMFC native message-map test");
    check(registered>=0xc000 && dispatch(target,registered,17,-18,result) && result==-18,
        "registered message maps dereference the native signature pointer");
    result=999;int previous=target.calls;
    check(!dispatch(target,WM_APP+99,1,2,result) && result==999 && target.calls==previous,"unmapped messages preserve result storage and do not call handlers");
    target.accept=TRUE;
    check(dispatch(target,WM_COMMAND,UINT_PTR(0x123456789),-61,result) && result==TRUE && target.pointer==0x123456789 && target.reply==-61,
        "WM_COMMAND reaches the native OnCommand virtual instead of bypassing overrides");
    target.accept=FALSE;result=999;
    check(!dispatch(target,WM_COMMAND,1,2,result) && result==999,"unhandled command overrides preserve result storage");
    NMHDR header{child.m_hWnd,UINT_PTR(0x123456789),7};target.accept=TRUE;target.reply=-900;
    check(dispatch(target,WM_NOTIFY,17,reinterpret_cast<LPARAM>(&header),result) && result==-900,
        "WM_NOTIFY reaches the native OnNotify virtual and returns its full-width result");
    header.hwndFrom=nullptr;previous=target.calls;result=999;
    check(!dispatch(target,WM_NOTIFY,17,reinterpret_cast<LPARAM>(&header),result) && target.calls==previous && result==999,
        "notifications without a sender bypass the handler");
}
class Raw : public CWnd {
public:
    static int traversals;
    static const AFX_MSGMAP* AFXAPI Parent() { ++traversals;return CWnd::GetThisMessageMap(); }
    AFX_MSGMAP_ENTRY entries[2]{};
    AFX_MSGMAP map{Parent,entries};
    CPoint point;UINT a=0,b=0;UINT_PTR handle=0;int calls=0;LRESULT reply=0;
    const AFX_MSGMAP* GetMessageMap() const override { return &map; }
    template<class Fn> void Set(UINT_PTR signature,Fn fn) {
        entries[0]={WM_APP+51,0,0,0,signature,nullptr};std::memcpy(&entries[0].pfn,&fn,sizeof(fn));
    }
    static BOOL MS_ABI Pair(Raw* self,int first,int second) { ++self->calls;self->a=first;self->b=second;return -3; }
    static LRESULT MS_ABI Empty(Raw* self) { ++self->calls;return self->reply; }
    static void MS_ABI MenuPair(Raw* self,CMenu* menu,int first,int second) { self->handle=reinterpret_cast<UINT_PTR>(menu);self->a=first;self->b=second; }
    static UINT MS_ABI Full(Raw* self,UINT first,LPARAM second) { self->a=first;self->handle=second;return 0xfedcba98; }
};
int Raw::traversals=0;
static void rawTablesAndCommands() {
    impl__AfxResetMsgCache__YAXXZ();
    Raw target;LRESULT result=999;
    target.Set(91,Raw::Pair); // AfxSig_b_v_ii, SDK numeric value.
    check(dispatch(target,WM_APP+51,0,MAKELPARAM(-4,-5),result) && result==-3 && target.a==UINT(-5) && target.b==UINT(-4),
        "SDK-authored tables retain native swapped signed coordinate arguments and BOOL sign extension");
    target.Set(70,Raw::Empty);target.reply=71;result=999;
    check(!dispatch(target,WM_APP+51,0,0,result) && result==999,"native AfxSig_l rejects nonzero returns without overwriting the result");
    target.reply=0;
    check(dispatch(target,WM_APP+51,0,0,result) && result==0,"native AfxSig_l handles a zero result");
    struct MenuModel { void* table;HMENU handle; } menu{nullptr,CreateMenu()};
    g_messageMenu=reinterpret_cast<CMenu*>(&menu);
    target.Set(35,Raw::MenuPair);
    check(dispatch(target,WM_APP+51,reinterpret_cast<WPARAM>(menu.handle),MAKELPARAM(-6,-7),result) &&
        target.handle==reinterpret_cast<UINT_PTR>(&menu) && target.a==UINT(-6) && target.b==UINT(-7),
        "menu-message arguments use native wrappers and signed word fields");
    DestroyMenu(menu.handle);g_messageMenu=nullptr;
    target.Set(94,Raw::Full);
    check(dispatch(target,WM_APP+51,0x12345678,LPARAM(0x123456789abcdef0),result) &&
        result==LRESULT(0xfedcba98) && target.handle==UINT_PTR(0x123456789abcdef0),"unsigned callback returns zero-extend while LPARAM retains all 64 bits");
    Raw::traversals=0;
    dispatch(target,WM_APP+52,0,0,result);dispatch(target,WM_APP+52,0,0,result);
    check(Raw::traversals==1,"native message caching retains misses and avoids repeated base-map traversal");
    impl__AfxResetMsgCache__YAXXZ();dispatch(target,WM_APP+52,0,0,result);
    check(Raw::traversals==2,"AfxResetMsgCache invalidates the production lookup cache");
    Derived command;
    auto* entries=command.Map()->pfnGetBaseMap()->lpEntries;
    for(auto* entry=entries;entry->nSig;++entry) {
        if(entry->nMessage!=WM_COMMAND && entry->nMessage!=WM_NOTIFY)continue;
        void* method=nullptr;memcpy(&method,&entry->pfn,8);command.calls=0;command.accept=TRUE;command.reply=-55;
        AFX_CMDHANDLERINFO info{};
        check(openmfc::detail::DispatchCommandMessage(&command,entry->nID,method,nullptr,entry->nSig,&info) &&
            info.pTarget==&command && !memcmp(&info.pmf,&entry->pfn,8) && command.calls==0,
            "command-handler queries report the native member pointer without invoking it");
        alignas(void*) unsigned char ui[72]{};NMHDR header{nullptr,1234,7};LRESULT notification=0;
        struct Notify { LRESULT* result;NMHDR* header; } notify{&notification,&header};
        void* extra=entry->nSig==AfxSigCmdUI ? static_cast<void*>(ui) : entry->nMessage==WM_NOTIFY ? static_cast<void*>(&notify) : nullptr;
        check(openmfc::detail::DispatchCommandMessage(&command,entry->nID,method,extra,entry->nSig,nullptr) && command.calls==1,
            "public command and notification macros use the production native command decoder");
        if(entry->nSig==AfxSigCmdUI) {
            command.accept=FALSE;
            check(!openmfc::detail::DispatchCommandMessage(&command,entry->nID,method,ui,entry->nSig,nullptr) &&
                !*reinterpret_cast<BOOL*>(ui+0x2c),"update-command continuation uses and resets the native BOOL at offset 0x2c");
        }
        if(entry->nMessage==WM_NOTIFY)check(notification==-55 && command.pointer==1234,"notification callbacks receive native header/result pointers");
    }
    impl__AfxResetMsgCache__YAXXZ();
}
static void colorAndDC() {
    Base window;
    HWND child=CreateWindowExW(0,L"STATIC",L"",WS_POPUP,0,0,40,40,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HDC handle=CreateCompatibleDC(nullptr);HBITMAP bitmap=CreateBitmap(2,2,1,32,nullptr);HGDIOBJ previous=::SelectObject(handle,bitmap);
    auto* temporary=impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(handle);
    LRESULT result=0;
    check(dispatch(window,WM_CTLCOLORSTATIC,reinterpret_cast<WPARAM>(handle),reinterpret_cast<LPARAM>(child),result) &&
        result==LRESULT(0x123456789) && window.a==CTLCOLOR_STATIC && window.colored==child && window.colorDC!=temporary,
        "ordinary Windows control-color messages reach ON_WM_CTLCOLOR with temporary native wrappers");
    check(GetObjectType(handle)==OBJ_MEMDC && GetBkColor(handle)==RGB(11,22,33),"borrowed control-color DCs remain usable after callback cleanup");
    using Color=COLORREF (MS_ABI*)(void*,COLORREF);
    using Saved=int (MS_ABI*)(void*);
    auto* table=*reinterpret_cast<void***>(temporary);
    auto color=reinterpret_cast<Color>(table[13]);
    check(color(temporary,RGB(44,55,66))==RGB(11,22,33) && GetBkColor(handle)==RGB(44,55,66),
        "native CDC slot 13 reaches SetBkColor on a real borrowed DC");
    check(reinterpret_cast<Saved>(table[9])(temporary)>0,"native CDC slot 9 preserves SaveDC's shared-handle return value");
    temporary->RestoreDC(-1);
    CPoint old(99,100);POINT current{};
    check(impl__SetViewportOrg_CDC__UEAA_AVCPoint__HH_Z(temporary,&old,3,4)==&old && old.x==0 && old.y==0 &&
        GetViewportOrgEx(handle,&current) && current.x==3 && current.y==4,
        "CDC point-return exports place the hidden return buffer after this, as required by the native member ABI");
    ::SelectObject(handle,previous);DeleteObject(bitmap);
    DeleteTempDCMap();
    check(GetObjectType(handle)==OBJ_MEMDC,"temporary DC map cleanup preserves its borrowed Windows handle");
    DeleteDC(handle);DestroyWindow(child);
    HDC owned=CreateCompatibleDC(nullptr);
    {
        window_input_tests::Import deletion(__imp_DeleteDC,DeleteOwnedDC);
        dcDeletes=0;
        { CDC owner;owner.SetOutputDC(owned); }
        check(dcDeletes==1 && deletedDC==owned && dcDeleted,"native CDC destruction successfully deletes its owned output DC once");
    }
}
}
static void windowMessageMaps() {
    window_map_tests::macrosAndMessages();window_map_tests::rawTablesAndCommands();window_map_tests::colorAndDC();
}
