#pragma once
extern "C" {
CScrollBar* MS_ABI impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z(const CWnd*,int);
DWORD MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd*);
void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd*,RECT*);
int MS_ABI impl__GetScrollPos_CWnd__QEBAHH_Z(const CWnd*,int);
int MS_ABI impl__SetScrollPos_CWnd__QEAAHHHH_Z(CWnd*,int,int,int);
void MS_ABI impl__GetScrollRange_CWnd__QEBAXHPEAH0_Z(const CWnd*,int,int*,int*);
BOOL MS_ABI impl__GetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__I_Z(CWnd*,int,SCROLLINFO*,UINT);
int MS_ABI impl__GetScrollLimit_CWnd__QEAAHH_Z(CWnd*,int);
void MS_ABI impl__ScrollWindow_CWnd__QEAAXHHPEBUtagRECT__0_Z(CWnd*,int,int,const RECT*,const RECT*);
BOOL MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd*,LONGLONG*);
BOOL MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd*);
}
namespace window_scrolling_test {
using Event=std::array<INT64,8>;
struct API {
    decltype(&impl__GetScrollPos_CWnd__QEBAHH_Z) position;
    decltype(&impl__SetScrollPos_CWnd__QEAAHHHH_Z) setPosition;
    decltype(&impl__GetScrollRange_CWnd__QEBAXHPEAH0_Z) range;
    decltype(&impl__GetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__I_Z) info;
    decltype(&impl__GetScrollLimit_CWnd__QEAAHH_Z) limit;
    decltype(&impl__ScrollWindow_CWnd__QEAAXHHPEBUtagRECT__0_Z) scroll;
    decltype(&impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z) bar;
    decltype(&impl__GetStyle_CWnd__QEBAKXZ) style;
    decltype(&impl__IsWindowEnabled_CWnd__QEBAHXZ) enabled;
    decltype(&impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z) coordinates;
    decltype(&impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z) notify;
    _AFX_THREAD_STATE* (MS_ABI* state)();
};
static std::vector<Event> events;
static CWnd* self;
static CWnd* control;
static void* container;
static bool capture,hasControl,visible,shortRect,hasNext,mutateNext;
static int mutation,failure,coordinateCount,maximum;
static UINT page;
static BOOL infoResult,enabledResult,notifyResult;
static DWORD styleValue;
static LONGLONG* notifyOutput;
static void event(INT64 n,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0) {
    events.push_back({n,a,b,c,d,e,f,g});if(failure==n)throw 91;
}
static INT64 handle(HWND h) {return reinterpret_cast<INT_PTR>(h);}
static HWND hwnd(INT_PTR h) {return reinterpret_cast<HWND>(h);}
static CScrollBar* MS_ABI getBar(const CWnd* window,int axis) {
    event(1,axis,window==self);
    if(mutation==1)self->m_hWnd=hwnd(12);
    if(mutation==2)control->m_hWnd=hwnd(22);
    return hasControl?reinterpret_cast<CScrollBar*>(control):nullptr;
}
static DWORD MS_ABI siteStyle(void*) {event(2);return styleValue;}
static DWORD MS_ABI siteExStyle(void*) {event(3);return styleValue;}
static BOOL MS_ABI siteEnabled(void*) {event(4);return enabledResult;}
static void MS_ABI scrollContainer(void* site,int x,int y) {event(5,site==container,x,y);}
static BOOL MS_ABI childNotify(CWnd* window,UINT message,ULONGLONG w,LONGLONG l,LONGLONG* result) {
    event(6,window==self,message,static_cast<INT64>(w),l,result==nullptr,result==notifyOutput);
    if(result)*result=-0x123456789LL;
    return notifyResult;
}
static decltype(&::GetScrollPos) realPosition;
static decltype(&::SetScrollPos) realSetPosition;
static decltype(&::GetScrollRange) realRange;
static decltype(&::GetScrollInfo) realInfo;
static decltype(&::IsWindowVisible) realVisible;
static decltype(&::GetWindow) realWindow;
static decltype(&::GetWindowRect) realRectangle;
static decltype(&::ScreenToClient) realCoordinates;
static decltype(&::SetWindowPos) realSetWindow;
static decltype(&::ScrollWindow) realScroll;
static decltype(&::GetWindowLongW) realStyle;
static decltype(&::IsWindowEnabled) realEnabled;
static int WINAPI position(HWND h,int bar) {
    if(!capture)return realPosition(h,bar);event(10,handle(h),bar);return -0x1234567;
}
static int WINAPI setPosition(HWND h,int bar,int value,BOOL redraw) {
    if(!capture)return realSetPosition(h,bar,value,redraw);event(11,handle(h),bar,value,redraw);return -900;
}
static BOOL WINAPI range(HWND h,int bar,int* low,int* high) {
    if(!capture)return realRange(h,bar,low,high);event(12,handle(h),bar,low!=nullptr,high!=nullptr,low==high);
    if(low)*low=-17;if(high)*high=maximum;return FALSE;
}
static BOOL WINAPI info(HWND h,int bar,SCROLLINFO* value) {
    if(!capture)return realInfo(h,bar,value);event(13,handle(h),bar,value->cbSize,value->fMask);
    value->nPage=page;return infoResult;
}
static BOOL WINAPI isVisible(HWND h) {
    if(!capture)return realVisible(h);event(14,handle(h));if(mutation==3)self->m_hWnd=hwnd(12);return visible?69:FALSE;
}
static HWND WINAPI window(HWND h,UINT command) {
    if(!capture)return realWindow(h,command);event(15,handle(h),command);
    if(command==GW_CHILD)return hwnd(101);
    return h==hwnd(101) && hasNext?hwnd(102):nullptr;
}
static BOOL WINAPI rectangle(HWND h,RECT* r) {
    if(!capture)return realRectangle(h,r);event(16,handle(h));r->left=INT_MAX;r->top=-7;
    if(!shortRect){r->right=51;r->bottom=61;}return FALSE;
}
static BOOL WINAPI coordinates(HWND h,POINT* p) {
    if(!capture)return realCoordinates(h,p);event(17,handle(h),p->x,p->y);
    p->x=LONG(ULONG(p->x)+100u);p->y=LONG(ULONG(p->y)+200u);
    if(mutation==4 && ++coordinateCount==1)self->m_hWnd=hwnd(12);return FALSE;
}
static BOOL WINAPI setWindow(HWND h,HWND after,int x,int y,int width,int height,UINT flags) {
    if(!capture)return realSetWindow(h,after,x,y,width,height,flags);event(18,handle(h),handle(after),x,y,width,height,flags);
    if(mutateNext)hasNext=false;if(mutation==5)self->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(container);return FALSE;
}
static BOOL WINAPI scroll(HWND h,int x,int y,const RECT* r,const RECT* clip) {
    if(!capture)return realScroll(h,x,y,r,clip);event(19,handle(h),x,y,r!=nullptr,clip!=nullptr,r?r->left:0,clip?clip->top:0);
    if(mutation==5)self->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(container);return FALSE;
}
static LONG WINAPI style(HWND h,int which) {
    if(!capture)return realStyle(h,which);event(20,handle(h),which);return static_cast<LONG>(styleValue);
}
static BOOL WINAPI enabled(HWND h) {
    if(!capture)return realEnabled(h);event(21,handle(h));return enabledResult;
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate window scrolling import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetScrollPos",position,realPosition);HOOK("SetScrollPos",setPosition,realSetPosition);HOOK("GetScrollRange",range,realRange);HOOK("GetScrollInfo",info,realInfo);
        HOOK("IsWindowVisible",isVisible,realVisible);HOOK("GetWindow",window,realWindow);HOOK("GetWindowRect",rectangle,realRectangle);
        HOOK("ScreenToClient",coordinates,realCoordinates);HOOK("SetWindowPos",setWindow,realSetWindow);HOOK("ScrollWindow",scroll,realScroll);
        HOOK("GetWindowLongW",style,realStyle);HOOK("IsWindowEnabled",enabled,realEnabled);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    alignas(8) BYTE bytes[240]{},barBytes[240]{};
    void* table[91]{};void* siteTable[44]{};void* containerTable[26]{};
    void** site=siteTable;void** cont=containerTable;
    bool boundaries=true;
    Fixture() {
        self=reinterpret_cast<CWnd*>(bytes);control=reinterpret_cast<CWnd*>(barBytes);container=&cont;
        *reinterpret_cast<void***>(self)=table;table[32]=reinterpret_cast<void*>(&getBar);table[75]=reinterpret_cast<void*>(&childNotify);
        siteTable[32]=reinterpret_cast<void*>(&siteStyle);siteTable[33]=reinterpret_cast<void*>(&siteExStyle);siteTable[43]=reinterpret_cast<void*>(&siteEnabled);
        containerTable[25]=reinterpret_cast<void*>(&scrollContainer);memset(bytes+232,0xa5,8);memset(barBytes+232,0xa5,8);reset();capture=true;
    }
    void reset() {
        events.clear();hasControl=false;mutation=failure=coordinateCount=0;maximum=999;page=7;infoResult=69;enabledResult=69;notifyResult=-1;styleValue=0;
        visible=false;shortRect=false;hasNext=true;mutateNext=false;self->m_hWnd=hwnd(11);control->m_hWnd=hwnd(21);self->m_pCtrlSite=nullptr;self->m_pCtrlCont=nullptr;
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        out.insert(out.end(),events.begin(),events.end());out.push_back({90,result,caught,0,0,0,0,0});
        boundaries=boundaries && *reinterpret_cast<UINT64*>(bytes+232)==0xa5a5a5a5a5a5a5a5ULL && *reinterpret_cast<UINT64*>(barBytes+232)==0xa5a5a5a5a5a5a5a5ULL;
    }
    ~Fixture(){capture=false;failure=0;check(boundaries,"scroll helpers preserve native CWnd object boundaries");}
};
template<class F> static int invoke(F action){try{action();return 0;}catch(int value){return value;}}
static std::vector<Event> routing(const API& api) {
    Fixture f;std::vector<Event> out;
    for(int operation=0;operation<4;++operation)for(int bar:{-1,SB_HORZ,SB_VERT,SB_CTL,INT_MAX})for(bool attached:{false,true})for(int mode:{0,1,2})for(BOOL redraw:{FALSE,BOOL(69),BOOL(-1)}) {
        f.reset();hasControl=attached;mutation=mode;int result=17,low=-31,high=-33;SCROLLINFO value{999,0xabc,123,456,789,12,34};
        int caught=invoke([&]{switch(operation){case 0:result=api.position(self,bar);break;case 1:result=api.setPosition(self,bar,INT_MIN,redraw);break;case 2:api.range(self,bar,&low,&high);break;case 3:result=api.info(self,bar,&value,0xf0000002u);break;}});
        f.save(out,result,caught);out.push_back({91,low,high,value.cbSize,value.fMask,value.nMin,value.nMax,value.nPage});
    }
    for(int bar:{0,1,2})for(int pointers:{0,1,2,3}) {
        f.reset();int low=33;api.range(self,bar,pointers&1?&low:nullptr,pointers&2?&low:nullptr);f.save(out,low);
    }
    for(int op=0;op<4;++op)for(int at:{1,10,11,12,13}) {
        f.reset();failure=at;SCROLLINFO value{};int low=0,high=0,result=17;
        int caught=invoke([&]{if(op==0)result=api.position(self,0);if(op==1)result=api.setPosition(self,0,5,69);if(op==2)api.range(self,0,&low,&high);if(op==3)result=api.info(self,0,&value,2);});f.save(out,result,caught);
    }
    check(api.bar(nullptr,INT_MIN)==nullptr && api.bar(self,0)==nullptr,"base CWnd supplies no scrollbar control");return out;
}
static std::vector<Event> limits(const API& api) {
    Fixture f;std::vector<Event> out;
    for(int max:{INT_MIN,-7,0,999,INT_MAX})for(UINT count:{0u,1u,2u,0x80000000u,UINT_MAX})for(BOOL success:{FALSE,BOOL(69),BOOL(-1)})for(bool attached:{false,true})for(int bar:{0,1,2}) {
        f.reset();maximum=max;page=count;infoResult=success;hasControl=attached;f.save(out,api.limit(self,bar));
    }
    for(int at:{1,12,13}) {f.reset();failure=at;int result=17,caught=invoke([&]{result=api.limit(self,0);});f.save(out,result,caught);}return out;
}
static std::vector<Event> states(const API& api) {
    Fixture f;std::vector<Event> out;
    for(bool ole:{false,true})for(bool nullHandle:{false,true})for(DWORD bits:{DWORD(0),DWORD(WS_HSCROLL|WS_VSCROLL),DWORD(WS_EX_LAYOUTRTL),DWORD(-1)})for(BOOL on:{FALSE,BOOL(69),BOOL(-1)}) {
        f.reset();self->m_pCtrlSite=ole?reinterpret_cast<COleControlSite*>(&f.site):nullptr;if(nullHandle)self->m_hWnd=nullptr;styleValue=bits;enabledResult=on;
        f.save(out,api.style(self));events.clear();f.save(out,api.enabled(self));
    }
    for(bool ole:{false,true})for(int at:{0,2,3,4,17,20,21}) {
        f.reset();styleValue=WS_EX_LAYOUTRTL;failure=at;mutation=4;self->m_pCtrlSite=ole?reinterpret_cast<COleControlSite*>(&f.site):nullptr;RECT r{3,5,41,51};
        int caught=invoke([&]{api.coordinates(self,&r);});f.save(out,0,caught);out.push_back({91,r.left,r.top,r.right,r.bottom,0,0,0});
    }
    return out;
}
static std::vector<Event> scrolling(const API& api) {
    Fixture f;std::vector<Event> out;RECT r{13,17,27,31},clip{41,43,51,57};
    for(bool shown:{false,true})for(int rectangles:{0,1,2,3})for(bool ole:{false,true})for(int mode:{0,3,4,5})for(bool abbreviated:{false,true})for(bool removeNext:{false,true}) {
        f.reset();visible=shown;mutation=mode;shortRect=abbreviated;mutateNext=removeNext;styleValue=WS_EX_LAYOUTRTL;
        self->m_pCtrlSite=ole?reinterpret_cast<COleControlSite*>(&f.site):nullptr;
        if(mode!=5)self->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(container);
        api.scroll(self,INT_MAX,INT_MIN,rectangles&1?&r:nullptr,rectangles&2?&clip:nullptr);f.save(out,0);
    }
    for(bool shown:{false,true})for(int at:{0,3,5,14,15,16,17,18,19,20}) {
        f.reset();visible=shown;failure=at;self->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(container);
        int caught=invoke([&]{api.scroll(self,-11,23,nullptr,nullptr);});f.save(out,0,caught);
    }
    return out;
}
static std::vector<Event> notifications(const API& api) {
    Fixture f;std::vector<Event> out;auto* state=api.state();MSG saved=state->m_lastSentMsg;
    for(BOOL result:{FALSE,BOOL(69),BOOL(-1)})for(bool output:{false,true})for(int at:{0,6}) {
        f.reset();failure=at;notifyResult=result;LONGLONG value=0x3456789abcdefLL;notifyOutput=&value;
        memset(&state->m_lastSentMsg,0xa5,sizeof(MSG));state->m_lastSentMsg.message=0xfedcba98;state->m_lastSentMsg.wParam=0xfedcba9876543210ULL;state->m_lastSentMsg.lParam=-0x123456789abcdefLL;
        MSG before=state->m_lastSentMsg;int actual=17,caught=invoke([&]{actual=api.notify(self,output?&value:nullptr);});f.save(out,actual,caught);out.push_back({91,value,memcmp(&before,&state->m_lastSentMsg,sizeof(MSG))==0,0,0,0,0,0});
    }
    state->m_lastSentMsg=saved;return out;
}
static void expectations(const API& api) {
    Fixture f;
    mutation=1;
    api.position(self,SB_HORZ);
    check(events.size()==2 && events.back()[0]==10 && events.back()[1]==12,
        "scroll positions reload the window handle after virtual control lookup");
    f.reset();mutation=1;SCROLLINFO value{};
    api.info(self,SB_HORZ,&value,SIF_PAGE);
    check(events.size()==2 && events.back()[0]==13 && events.back()[1]==11 && value.cbSize==sizeof(value) && value.fMask==SIF_PAGE,
        "scroll info retains its original window handle and initializes the native info header");
    f.reset();hasControl=true;api.info(self,SB_CTL,&value,SIF_POS);
    check(events.size()==1 && events[0][0]==13 && events[0][1]==11 && events[0][2]==SB_CTL,
        "control scroll info skips virtual scrollbar lookup");
    f.reset();hasControl=true;api.setPosition(self,SB_VERT,37,-1);
    check(events.size()==2 && events.back()[1]==21 && events.back()[2]==SB_CTL && events.back()[3]==37 && events.back()[4]==-1,
        "scroll position setters use the returned control and preserve full redraw BOOL values");
    f.reset();maximum=-7;page=20;
    check(api.limit(self,SB_VERT)==-26,"scroll limits retain negative results instead of clamping them");
    f.reset();maximum=37;infoResult=FALSE;
    check(api.limit(self,SB_HORZ)==37,"failed page queries retain the separately queried scroll maximum");
    f.reset();visible=true;self->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(container);RECT clip{1,2,3,4};
    api.scroll(self,3,-5,nullptr,&clip);
    check(events.size()==3 && events[1][0]==19 && events.back()[0]==5,
        "clip-only scrolling still notifies the OLE control container");
    f.reset();self->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(container);
    api.scroll(self,3,-5,&clip,nullptr);
    check(events.size()==2 && events.back()[0]==19,"an explicit scroll rectangle suppresses the OLE container callback");
    f.reset();auto* state=api.state();MSG saved=state->m_lastSentMsg;
    state->m_lastSentMsg.message=WM_HSCROLL;state->m_lastSentMsg.wParam=123;state->m_lastSentMsg.lParam=-456;
    notifyOutput=nullptr;notifyResult=69;
    check(api.notify(self,nullptr)==69 && events.size()==1 && events[0][0]==6 && events[0][5]==1,
        "child reflection forwards null result storage and preserves noncanonical TRUE values");
    state->m_lastSentMsg=saved;
}
static void realWindows(const API& api) {
    Fixture f;capture=false;HWND parent=CreateWindowExW(0,L"STATIC",L"scroll parent",WS_OVERLAPPED,50,60,300,250,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HWND child=CreateWindowExW(0,L"STATIC",L"scroll child",WS_CHILD,13,17,41,43,parent,nullptr,GetModuleHandleW(nullptr),nullptr);
    check(parent && child,"create real hidden scroll window and child");self->m_hWnd=parent;
    // Restore a plain base scrollbar lookup for the real Win32 smoke check.
    f.table[32]=reinterpret_cast<void*>(api.bar);
    RECT before{},after{};GetWindowRect(child,&before);api.scroll(self,7,-9,nullptr,nullptr);GetWindowRect(child,&after);
    check(after.left==before.left+7 && after.top==before.top-9 && after.right-after.left==41,"hidden scrolling moves actual child windows without resizing them");
    ::SetScrollRange(parent,SB_HORZ,-5,200,FALSE);::SetScrollPos(parent,SB_HORZ,42,FALSE);
    check(api.position(self,SB_HORZ)==42,"base scrollbar lookup preserves actual window scrollbar positions");
    RECT r{0,0,80,70},expected=r;MapWindowPoints(nullptr,parent,reinterpret_cast<POINT*>(&expected),2);api.coordinates(self,&r);
    check(EqualRect(&r,&expected)!=FALSE,"screen rectangle conversion matches actual window coordinates");DestroyWindow(child);DestroyWindow(parent);
}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
        printf("window scroll mismatch %zu local:",i);for(auto v:expected[i])printf(" %lld",v);printf(" native:");for(auto v:actual[i])printf(" %lld",v);puts("");break;
    }
    check(actual==expected,label);
}
}
static void windowScrolling() {
    using namespace window_scrolling_test;
    API local{&impl__GetScrollPos_CWnd__QEBAHH_Z,&impl__SetScrollPos_CWnd__QEAAHHHH_Z,&impl__GetScrollRange_CWnd__QEBAXHPEAH0_Z,
        &impl__GetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__I_Z,&impl__GetScrollLimit_CWnd__QEAAHH_Z,&impl__ScrollWindow_CWnd__QEAAXHHPEBUtagRECT__0_Z,
        &impl__GetScrollBarCtrl_CWnd__UEBAPEAVCScrollBar__H_Z,&impl__GetStyle_CWnd__QEBAKXZ,&impl__IsWindowEnabled_CWnd__QEBAHXZ,
        &impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z,&impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z,&AfxGetThreadState};
    Imports imports(GetModuleHandleW(nullptr),true);expectations(local);realWindows(local);
    auto routed=routing(local),ranges=limits(local),status=states(local),movement=scrolling(local),reflected=notifications(local);
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_WINDOW_SCROLL_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for window scroll comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"window scroll oracle uses the verified MFC image build");
            if(pinned) {
                Imports nativeImports(native,false);API api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(position,0x28e710);RVA(setPosition,0x28e6c0);RVA(range,0x28e7d0);RVA(info,0x28e8f0);RVA(limit,0x28e950);RVA(scroll,0x28e9d0);
                RVA(bar,0x71e0);RVA(style,0x2a9690);RVA(enabled,0x2a9b00);RVA(coordinates,0x2a32b0);RVA(notify,0x28ef80);RVA(state,0x1332a0);
#undef RVA
                expectations(api);realWindows(api);
                compare(routing(api),routed,"scrollbar control dispatch, handle snapshots, output pointers and callback failures match native MFC");
                compare(limits(api),ranges,"scroll limits preserve native range fallback, unsigned pages and integer wrapping");
                compare(states(api),status,"OLE style/enabled callbacks and RTL rectangle conversion match native MFC");
                compare(scrolling(api),movement,"hidden child scrolling, live sibling iteration, OLE callbacks and interrupted operations match native MFC");
                compare(notifications(api),reflected,"last-message reflection preserves caller pointers, full BOOL values and thread message state");
            }
            FreeLibrary(native);
        }
    }
}
