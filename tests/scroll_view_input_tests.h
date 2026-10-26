#pragma once
extern "C" {
CPoint* MS_ABI impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ(const CScrollView*,CPoint*);
CSize* MS_ABI impl__GetWheelScrollDistance_CScrollView__UEAA_AVCSize__V2_HH_Z(CScrollView*,CSize*,CSize,BOOL,BOOL);
void MS_ABI impl__CheckScrollBars_CScrollView__QEBAXAEAH0_Z(const CScrollView*,BOOL*,BOOL*);
BOOL MS_ABI impl__OnScroll_CScrollView__UEAAHIIH_Z(CScrollView*,UINT,UINT,BOOL);
BOOL MS_ABI impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z(CScrollView*,CSize,BOOL);
void MS_ABI impl__OnHScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView*,UINT,UINT,CScrollBar*);
void MS_ABI impl__OnVScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z(CScrollView*,UINT,UINT,CScrollBar*);
BOOL MS_ABI impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView*,UINT,short,CPoint);
BOOL MS_ABI impl__OnMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView*,UINT,short,CPoint);
CScrollBar* MS_ABI impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(const CView*,int);
CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd*,int);
}
namespace openmfc { namespace detail { extern BOOL scrollViewWheelSettingsInitialized;extern UINT scrollViewWheelLines; } }
namespace scroll_view_input_test {
using Event=std::array<INT64,8>;
struct API {
    decltype(&impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ) device;
    decltype(&impl__GetWheelScrollDistance_CScrollView__UEAA_AVCSize__V2_HH_Z) distance;
    decltype(&impl__CheckScrollBars_CScrollView__QEBAXAEAH0_Z) checkBars;
    decltype(&impl__OnScroll_CScrollView__UEAAHIIH_Z) scroll;
    decltype(&impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z) scrollBy;
    decltype(&impl__OnHScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z) horizontal;
    decltype(&impl__OnVScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z) vertical;
    decltype(&impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z) wheel;
    decltype(&impl__OnMouseWheel_CScrollView__QEAAHIFVCPoint___Z) mouse;
    decltype(&impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z) bar;
    decltype(&impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z) item;
    _AFX_THREAD_STATE* (MS_ABI* state)();
    BOOL* cacheInitialized;UINT* cacheLines;
    splitter_command_test::API graph{};
};
static std::vector<Event> events;
static CScrollView* self;
static CWnd* controls[3];
static int availability[2],positions[2],maxima[2];static UINT pages[2];
static int mutation,failure,barCalls,paneID;
static DWORD styleBits;
static BOOL callbackResult,reflectResult,spiResult;
static UINT requestedLines;
static bool capture,graphMode,writeSettings;
static BOOL* observedVertical;
static const API* currentAPI;
static INT64 id(HWND h) {return graphMode?splitter_command_test::id(h):reinterpret_cast<INT_PTR>(h);}
static HWND hwnd(INT_PTR value) {return reinterpret_cast<HWND>(value);}
static void event(INT64 n,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0) {
    events.push_back({n,a,b,c,d,e,f,g});if(failure==n)throw 91;
}
static CScrollBar* MS_ABI bar(const CWnd*,int axis) {
    event(1,axis,observedVertical?*observedVertical:-1);
    if(mutation==1)styleBits=0;
    if(mutation==2 && ++barCalls==1)self->m_hWnd=hwnd(12);
    return availability[axis]?reinterpret_cast<CScrollBar*>(controls[axis]):nullptr;
}
static BOOL MS_ABI scrollBy(CScrollView*,CSize delta,BOOL perform) {
    event(2,delta.cx,delta.cy,perform);
    if(mutation==4)self->m_hWnd=hwnd(12);return callbackResult;
}
static BOOL MS_ABI scroll(CScrollView*,UINT code,UINT position,BOOL perform) {event(3,code,position,perform);return callbackResult;}
static BOOL MS_ABI reflect(CWnd* control,UINT message,ULONGLONG w,LONGLONG l,LONGLONG* result) {
    event(4,id(control->m_hWnd),message,static_cast<INT64>(w),l,result==nullptr);
    if(mutation==5)availability[0]=availability[1]=0;return reflectResult;
}
static DWORD MS_ABI siteStyle(void*) {event(5);return styleBits;}
static BOOL MS_ABI siteEnabled(void* site) {
    int axis=*reinterpret_cast<int*>(static_cast<char*>(site)+8);event(6,axis);return availability[axis]==2?69:FALSE;
}
static CWnd* MS_ABI item(void*,int id) {event(7,id);return controls[2];}
static decltype(&::GetWindowLongW) realStyle;
static decltype(&::IsWindowEnabled) realEnabled;
static decltype(&::GetScrollPos) realPosition;
static decltype(&::SetScrollPos) realSetPosition;
static decltype(&::GetScrollRange) realRange;
static decltype(&::GetScrollInfo) realInfo;
static decltype(&::IsWindowVisible) realVisible;
static decltype(&::ScrollWindow) realScroll;
static decltype(&::UpdateWindow) realUpdate;
static decltype(&::SystemParametersInfoW) realSettings;
static decltype(&::MulDiv) realMultiply;
static decltype(&::GetDlgCtrlID) realCtrlID;
static decltype(&::GetDlgItem) realItem;
static LONG WINAPI style(HWND h,int which) {if(!capture)return realStyle(h,which);event(10,id(h),which);return LONG(styleBits);}
static BOOL WINAPI enabled(HWND h) {
    if(!capture)return realEnabled(h);int axis=h==controls[0]->m_hWnd?0:1;event(11,id(h));return availability[axis]==2?-1:FALSE;
}
static int axis(HWND h,int bar) {if(bar==SB_CTL)return h==controls[0]->m_hWnd?0:1;return bar;}
static int WINAPI position(HWND h,int bar) {
    if(!capture)return realPosition(h,bar);event(12,id(h),bar);
    if(mutation==3){self->m_lineDev={INT_MAX,INT_MIN};self->m_pageDev={-5,7};}
    return positions[axis(h,bar)];
}
static int WINAPI setPosition(HWND h,int bar,int value,BOOL redraw) {
    if(!capture)return realSetPosition(h,bar,value,redraw);event(13,id(h),bar,value,redraw);int a=axis(h,bar),old=positions[a];positions[a]=value;return old;
}
static BOOL WINAPI range(HWND h,int bar,int* low,int* high) {
    if(!capture)return realRange(h,bar,low,high);event(14,id(h),bar);*low=-37;*high=maxima[axis(h,bar)];return FALSE;
}
static BOOL WINAPI info(HWND h,int bar,SCROLLINFO* value) {
    if(!capture)return realInfo(h,bar,value);event(15,id(h),bar,value->cbSize,value->fMask);value->nPage=pages[axis(h,bar)];return 69;
}
static BOOL WINAPI visible(HWND h) {if(!capture)return realVisible(h);event(16,id(h));return TRUE;}
static BOOL WINAPI scrollWindow(HWND h,int x,int y,const RECT* r,const RECT* clip) {
    if(!capture)return realScroll(h,x,y,r,clip);event(17,id(h),x,y,r!=nullptr,clip!=nullptr);return FALSE;
}
static BOOL WINAPI update(HWND h) {if(!capture)return realUpdate(h);event(18,id(h));return FALSE;}
static BOOL WINAPI settings(UINT action,UINT parameter,void* out,UINT flags) {
    if(!capture)return realSettings(action,parameter,out,flags);event(19,action,parameter,flags,*static_cast<UINT*>(out));
    if(mutation==6)currentAPI->wheel(self,0,120,CPoint(-7,13));
    if(writeSettings)*static_cast<UINT*>(out)=requestedLines;return spiResult;
}
static int WINAPI multiply(int value,int numerator,int denominator) {
    if(!capture)return realMultiply(value,numerator,denominator);event(20,value,numerator,denominator);return realMultiply(value,numerator,denominator);
}
static int WINAPI ctrlID(HWND h) {if(!capture)return realCtrlID(h);event(21,id(h));return paneID;}
static HWND WINAPI getItem(HWND h,int controlID) {
    if(!capture)return realItem(h,controlID);event(22,id(h),controlID);return controls[2]?controls[2]->m_hWnd:nullptr;
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate scroll view import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetWindowLongW",style,realStyle);HOOK("IsWindowEnabled",enabled,realEnabled);HOOK("GetScrollPos",position,realPosition);HOOK("SetScrollPos",setPosition,realSetPosition);
        HOOK("GetScrollRange",range,realRange);HOOK("GetScrollInfo",info,realInfo);HOOK("IsWindowVisible",visible,realVisible);HOOK("ScrollWindow",scrollWindow,realScroll);
        HOOK("UpdateWindow",update,realUpdate);HOOK("SystemParametersInfoW",settings,realSettings);HOOK("MulDiv",multiply,realMultiply);
        HOOK("GetDlgCtrlID",ctrlID,realCtrlID);HOOK("GetDlgItem",getItem,realItem);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
static void reset(const API& api) {
    events.clear();mutation=failure=barCalls=0;styleBits=WS_HSCROLL|WS_VSCROLL;callbackResult=69;reflectResult=FALSE;observedVertical=nullptr;
    availability[0]=availability[1]=0;positions[0]=7;positions[1]=11;maxima[0]=200;maxima[1]=300;pages[0]=5;pages[1]=7;
    spiResult=FALSE;requestedLines=3;writeSettings=true;*api.cacheInitialized=FALSE;*api.cacheLines=17;currentAPI=&api;
    self->m_lineDev={13,17};self->m_pageDev={71,83};self->m_pCtrlSite=nullptr;self->m_pCtrlCont=nullptr;
}
struct Fixture {
    alignas(8) BYTE bytes[312]{},controlBytes[3][240]{};
    void* table[112]{};void* controlTable[3][76]{};void* siteTable[44]{};
    struct Site{void** table;int axis;}sites[2]{};void* mainSiteTable[33]{};void** mainSite=mainSiteTable;
    const API& api;bool boundaries=true;BOOL savedInitialized;UINT savedLines;
    Fixture(const API& functions):api(functions),savedInitialized(*api.cacheInitialized),savedLines(*api.cacheLines) {
        graphMode=false;self=reinterpret_cast<CScrollView*>(bytes);*reinterpret_cast<void***>(self)=table;
        table[32]=reinterpret_cast<void*>(&bar);table[92]=reinterpret_cast<void*>(&scroll);table[93]=reinterpret_cast<void*>(&scrollBy);
        for(int i=0;i<3;++i){controls[i]=reinterpret_cast<CWnd*>(controlBytes[i]);*reinterpret_cast<void***>(controls[i])=controlTable[i];controls[i]->m_hWnd=hwnd(21+i);controlTable[i][75]=reinterpret_cast<void*>(&reflect);memset(controlBytes[i]+232,0xa5,8);}
        siteTable[43]=reinterpret_cast<void*>(&siteEnabled);for(int i=0;i<2;++i)sites[i]={siteTable,i};mainSiteTable[32]=reinterpret_cast<void*>(&siteStyle);
        memset(bytes+304,0xa5,8);reset(api);self->m_hWnd=hwnd(11);capture=true;
    }
    void resetCase(){reset(api);self->m_hWnd=hwnd(11);for(int i=0;i<2;++i)controls[i]->m_pCtrlSite=nullptr;}
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        out.insert(out.end(),events.begin(),events.end());out.push_back({90,result,caught,positions[0],positions[1],*api.cacheInitialized,*api.cacheLines,0});
        boundaries=boundaries && *reinterpret_cast<UINT64*>(bytes+304)==0xa5a5a5a5a5a5a5a5ULL;
        for(int i=0;i<3;++i)boundaries=boundaries && *reinterpret_cast<UINT64*>(controlBytes[i]+232)==0xa5a5a5a5a5a5a5a5ULL;
    }
    ~Fixture(){capture=false;failure=0;*api.cacheInitialized=savedInitialized;*api.cacheLines=savedLines;check(boundaries,"scroll view input preserves SDK object boundaries");}
};
template<class F>static int invoke(F action){try{action();return 0;}catch(int value){return value;}}
static std::vector<Event> availabilityCases(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(DWORD bits:{DWORD(0),DWORD(WS_HSCROLL),DWORD(WS_VSCROLL),DWORD(WS_HSCROLL|WS_VSCROLL)})for(int h:{0,1,2})for(int v:{0,1,2})for(bool ole:{false,true})for(int mode:{0,1,2})for(bool alias:{false,true}) {
        f.resetCase();styleBits=bits;availability[0]=h;availability[1]=v;mutation=mode;BOOL horizontal=37,vertical=41;observedVertical=&vertical;
        if(ole){self->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&f.mainSite);for(int a=0;a<2;++a)controls[a]->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&f.sites[a]);}
        api.checkBars(self,alias?&vertical:&horizontal,&vertical);f.save(out,0);out.push_back({91,horizontal,vertical,0,0,0,0,0});
    }
    for(int at:{1,5,6,10,11}){f.resetCase();failure=at;availability[0]=availability[1]=2;BOOL h=37,v=41;int caught=invoke([&]{api.checkBars(self,&h,&v);});f.save(out,0,caught);out.push_back({91,h,v,0,0,0,0,0});}
    return out;
}
static std::vector<Event> aggregates(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int x:{INT_MIN,-101,-9,0,9,101,INT_MAX})for(int y:{INT_MIN,-101,-9,0,9,101,INT_MAX})for(BOOL horizontal:{FALSE,BOOL(69),BOOL(-1)})for(BOOL vertical:{FALSE,BOOL(69),BOOL(-1)}) {
        alignas(8) UINT64 guarded[3]={0xabcdef0123456789ULL,0,0xabcdef0123456789ULL};auto* value=reinterpret_cast<CSize*>(guarded+1);
        auto* result=api.distance(self,value,CSize(x,y),horizontal,vertical);out.push_back({92,result==value,value->cx,value->cy,guarded[0]==0xabcdef0123456789ULL,guarded[2]==0xabcdef0123456789ULL,0,0});
    }
    for(bool attached:{false,true})for(int at:{0,1,12}){f.resetCase();availability[0]=availability[1]=attached?2:0;failure=at;CPoint point(-99,-101);CPoint* result=nullptr;int caught=invoke([&]{result=api.device(self,&point);});f.save(out,result==&point,caught);out.push_back({91,point.x,point.y,0,0,0,0,0});}
    return out;
}
static std::vector<Event> commands(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(UINT x:{0u,1u,2u,3u,4u,5u,6u,7u,8u,255u})for(UINT y:{0u,1u,2u,3u,4u,5u,6u,7u,8u,255u})for(UINT position:{0u,UINT(INT_MIN),UINT(INT_MAX),UINT_MAX})for(BOOL perform:{FALSE,BOOL(69),BOOL(-1)})for(BOOL reply:{FALSE,BOOL(69),BOOL(-1)}) {
        f.resetCase();callbackResult=reply;positions[0]=INT_MAX;positions[1]=INT_MIN;f.save(out,api.scroll(self,0x80000000u|(y<<8)|x,position,perform));
    }
    for(int mode:{0,2,3,4})for(int at:{0,1,2,12,18}) {f.resetCase();mutation=mode;failure=at;int result=17,caught=invoke([&]{result=api.scroll(self,0x0301,37,69);});f.save(out,result,caught);}return out;
}
static std::vector<Event> movement(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(DWORD bits:{DWORD(0),DWORD(WS_HSCROLL),DWORD(WS_VSCROLL),DWORD(WS_HSCROLL|WS_VSCROLL)})for(int h:{0,1,2})for(int v:{0,1,2})for(int start:{0,7,INT_MAX})for(int delta:{0,1,-1,1000,INT_MIN})for(int maximum:{-7,0,100})for(BOOL perform:{FALSE,BOOL(69),BOOL(-1)}) {
        f.resetCase();styleBits=bits;availability[0]=h;availability[1]=v;positions[0]=start;positions[1]=start;maxima[0]=maximum;maxima[1]=maximum;pages[0]=5;pages[1]=UINT_MAX;
        f.save(out,api.scrollBy(self,CSize(delta,delta),perform));
    }
    for(int mode:{0,1,2})for(int at:{0,1,6,10,11,12,13,14,15,16,17}) {
        f.resetCase();mutation=mode;failure=at;availability[0]=availability[1]=2;controls[0]->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&f.sites[0]);controls[1]->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&f.sites[1]);
        int result=17,caught=invoke([&]{result=api.scrollBy(self,CSize(17,-11),69);});f.save(out,result,caught);
    }
    return out;
}
static std::vector<Event> reflection(const API& api) {
    Fixture f(api);std::vector<Event> out;auto* state=api.state();MSG saved=state->m_lastSentMsg;
    state->m_lastSentMsg.message=WM_APP+17;state->m_lastSentMsg.wParam=0xfedcba9876543210ULL;state->m_lastSentMsg.lParam=-0x123456789abcdefLL;
    for(int axis:{0,1})for(int source:{-1,0,1,2})for(bool attached:{false,true})for(BOOL reflected:{FALSE,BOOL(69),BOOL(-1)})for(UINT code:{0u,5u,0x12345678u,UINT_MAX})for(int mode:{0,5}) {
        f.resetCase();availability[0]=availability[1]=attached?2:0;reflectResult=reflected;mutation=mode;CScrollBar* incoming=source<0?nullptr:reinterpret_cast<CScrollBar*>(controls[source]);
        (axis?api.vertical:api.horizontal)(self,code,0xfedcba98u,incoming);f.save(out,0);
    }
    for(int axis:{0,1})for(int at:{0,1,3,4}){f.resetCase();availability[0]=availability[1]=2;failure=at;int caught=invoke([&]{(axis?api.vertical:api.horizontal)(self,5,37,reinterpret_cast<CScrollBar*>(controls[axis]));});f.save(out,0,caught);}
    state->m_lastSentMsg=saved;return out;
}
static std::vector<Event> wheels(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(UINT lines:{0u,1u,3u,120u,0x80000000u,UINT_MAX})for(int delta:{SHRT_MIN,-240,-120,-60,-1,0,1,60,120,240,SHRT_MAX})for(int axes:{0,1,2,3})for(int dimensions:{0,1,2,3})for(BOOL reply:{FALSE,BOOL(69),BOOL(-1)}) {
        f.resetCase();styleBits=(axes&1?WS_HSCROLL:0)|(axes&2?WS_VSCROLL:0);requestedLines=lines;callbackResult=reply;
        if(dimensions==1){self->m_pageDev={-5,-7};self->m_lineDev={-11,-13};}
        if(dimensions==2){self->m_pageDev={INT_MAX,INT_MAX};self->m_lineDev={INT_MIN,INT_MAX};}
        if(dimensions==3){self->m_pageDev={0,0};self->m_lineDev={1,1};}
        f.save(out,api.wheel(self,MK_SHIFT|MK_CONTROL,short(delta),CPoint(-7,13)));
        // A changed system answer cannot invalidate this module's cached value.
        requestedLines=9;events.clear();f.save(out,api.wheel(self,0,short(delta),CPoint(3,5)));
    }
    for(bool write:{false,true})for(int mode:{0,4,6})for(int at:{0,1,2,10,18,19,20}) {
        f.resetCase();writeSettings=write;requestedLines=9;mutation=mode;failure=at;int result=17,caught=invoke([&]{result=api.wheel(self,0,120,CPoint(3,5));});f.save(out,result,caught);
    }
    return out;
}
static void expectations(const API& api) {
    Fixture f(api);BOOL h=17,v=19;availability[0]=1;availability[1]=0;styleBits=WS_HSCROLL;
    api.checkBars(self,&h,&v);check(h==TRUE && v==FALSE,"style bits still advertise an axis whose scrollbar control is disabled");
    f.resetCase();styleBits=WS_HSCROLL;availability[0]=1;positions[1]=0;
    check(api.scrollBy(self,CSize(7,0),FALSE)==FALSE,"disabled controls suppress scroll movement even when the window has the style bit");
    f.resetCase();check(api.scrollBy(self,CSize(7,9),FALSE)==TRUE && positions[0]==7 && positions[1]==11 && events.back()[0]==15,"scroll probing reports available movement without scrolling or changing positions");
    f.resetCase();api.scroll(self,0xff04,999,69);check(events[events.size()-2][0]==2 && events[events.size()-2][1]==0 && events[events.size()-2][2]==0,"thumb-position commands leave scroll positions unchanged");
    f.resetCase();requestedLines=3;api.wheel(self,0,240,CPoint());check(events[events.size()-2][0]==2 && events[events.size()-2][2]==-102,"negative wheel movement is not capped to the page height");
    f.resetCase();requestedLines=WHEEL_PAGESCROLL;api.wheel(self,0,0,CPoint());check(events[events.size()-2][2]==83,"page wheel mode treats a zero delta as forward page movement");
    f.resetCase();CSize size;check(api.distance(self,&size,CSize(-101,99),69,-1)==&size && size.cx==-10 && size.cy==9,"wheel distance uses native hidden-return ordering and signed tenth-size steps");
}
static std::vector<Event> lookup(const API& api) {
    // Reuse the actual Win32 graph and permanent handle-map fixture already
    // verified for splitter commands. Native FromHandle has an internal OLE
    // parent probe which the shared import scope filters by its return address.
    splitter_command_test::Imports parentImports(GetModuleHandleW(nullptr),true);
    std::unique_ptr<splitter_command_test::Imports> nativeParentImports;
    if(api.graph.type)nativeParentImports.reset(new splitter_command_test::Imports(reinterpret_cast<HMODULE>(reinterpret_cast<char*>(api.graph.type)-0x332860),false));
    splitter_command_test::Fixture f(api.graph);graphMode=true;self=reinterpret_cast<CScrollView*>(f.view());
    controls[0]=splitter_command_test::windows[0];controls[1]=splitter_command_test::windows[1];controls[2]=splitter_command_test::windows[2];
    f.table[3][32]=reinterpret_cast<void*>(&bar);f.table[3][93]=reinterpret_cast<void*>(&scrollBy);
    void* containerTable[32]{};containerTable[31]=reinterpret_cast<void*>(&item);void** container=containerTable;
    BOOL savedInitialized=*api.cacheInitialized;UINT savedLines=*api.cacheLines;std::vector<Event> out;bool lookupContract=true,mouseContract=true;
    auto save=[&](INT64 result,int caught=0){out.insert(out.end(),events.begin(),events.end());out.push_back({90,result,caught,*api.cacheInitialized,*api.cacheLines,0,0,0});for(auto e:splitter_command_test::events)out.push_back({93,e[0],e[1],e[2],e[3],e[4],e[5],0});};
    auto prepare=[&]{capture=false;f.reset();reset(api);self=reinterpret_cast<CScrollView*>(f.view());self->m_hWnd=splitter_command_test::handles[3];self->m_lineDev={13,17};self->m_pageDev={71,83};splitter_command_test::capture=true;capture=true;};
    for(int axis:{0,1,-1,2,INT_MIN})for(DWORD bits:{DWORD(0),DWORD(WS_HSCROLL),DWORD(WS_VSCROLL),DWORD(WS_HSCROLL|WS_VSCROLL)})for(int kind:{0,1,2})for(int id:{-1,0xe8ff,0xe900,0xe90f,0xe910,0xe97f,0xe9ff,0xea00})for(bool ole:{false,true}) {
        prepare();styleBits=bits;splitter_command_test::kind=kind;paneID=id;f.splitter()->m_pCtrlCont=ole?reinterpret_cast<COleControlContainer*>(&container):nullptr;
        CScrollBar* result=nullptr;int caught=invoke([&]{result=api.bar(f.view(),axis);});save(result==reinterpret_cast<CScrollBar*>(controls[2]),caught);
        bool expected=!(bits&(axis==0?WS_HSCROLL:WS_VSCROLL)) && kind!=2 && UINT(id)-0xe900u<=0xffu;
        lookupContract=lookupContract && !caught && ((result==reinterpret_cast<CScrollBar*>(controls[2]))==expected);
        // Even the parent can carry an OLE container with a null HWND.
        f.splitter()->m_pCtrlCont=nullptr;
    }
    for(UINT flags:{0u,UINT(MK_SHIFT),UINT(MK_CONTROL),UINT(MK_LBUTTON),0x80000000u})for(int kind:{0,1,2})for(BOOL reply:{FALSE,BOOL(69),BOOL(-1)}) {
        prepare();splitter_command_test::kind=kind;callbackResult=reply;int result=17,caught=invoke([&]{result=api.mouse(self,flags,-120,CPoint(-7,13));});save(result,caught);
        mouseContract=mouseContract && !caught && result==((kind==2 && !(flags&(MK_SHIFT|MK_CONTROL)))?reply:FALSE);
    }
    for(int at:{0,7,10,19,21,22}) {prepare();styleBits=0;paneID=0xe927;failure=at;CScrollBar* result=nullptr;int caught=invoke([&]{result=api.bar(f.view(),1);});save(result==reinterpret_cast<CScrollBar*>(controls[2]),caught);}
    for(bool ole:{false,true}) {prepare();f.splitter()->m_pCtrlCont=ole?reinterpret_cast<COleControlContainer*>(&container):nullptr;f.splitter()->m_hWnd=nullptr;CWnd* result=api.item(f.splitter(),-71);save(result==controls[2]);f.splitter()->m_hWnd=splitter_command_test::handles[1];f.splitter()->m_pCtrlCont=nullptr;}
    check(lookupContract,"splitter scrollbar lookup uses pane row/column IDs and respects built-in window scrollbar styles");
    check(mouseContract,"mouse wheel handlers reject modifiers and splitter parents before scrolling");
    capture=false;graphMode=false;*api.cacheInitialized=savedInitialized;*api.cacheLines=savedLines;return out;
}
static void realWindows(const API& api) {
    splitter_command_test::Imports parentImports(GetModuleHandleW(nullptr),true);
    std::unique_ptr<splitter_command_test::Imports> nativeParentImports;
    if(api.graph.type)nativeParentImports.reset(new splitter_command_test::Imports(reinterpret_cast<HMODULE>(reinterpret_cast<char*>(api.graph.type)-0x332860),false));
    splitter_command_test::Fixture f(api.graph);capture=false;splitter_command_test::capture=false;
    self=reinterpret_cast<CScrollView*>(f.view());f.table[3][32]=reinterpret_cast<void*>(api.bar);f.table[3][93]=reinterpret_cast<void*>(api.scrollBy);
    self->m_lineDev={3,5};self->m_pageDev={50,60};
    SetWindowLongPtrW(self->m_hWnd,GWL_STYLE,WS_CHILD|WS_HSCROLL|WS_VSCROLL);
    SCROLLINFO horizontal{sizeof(SCROLLINFO),SIF_RANGE|SIF_PAGE|SIF_POS,0,300,50,70,0};
    SCROLLINFO vertical{sizeof(SCROLLINFO),SIF_RANGE|SIF_PAGE|SIF_POS,0,400,60,80,0};
    SetScrollInfo(self->m_hWnd,SB_HORZ,&horizontal,FALSE);SetScrollInfo(self->m_hWnd,SB_VERT,&vertical,FALSE);
    HWND child=CreateWindowExW(0,L"STATIC",L"scroll input child",WS_CHILD,13,17,41,43,self->m_hWnd,nullptr,GetModuleHandleW(nullptr),nullptr);
    check(child!=nullptr,"create a real child for scroll view input integration");RECT before{},after{};GetWindowRect(child,&before);
    BOOL result=api.scrollBy(self,CSize(17,-11),FALSE);GetWindowRect(child,&after);
    check(result==TRUE && GetScrollPos(self->m_hWnd,SB_HORZ)==70 && GetScrollPos(self->m_hWnd,SB_VERT)==80 && EqualRect(&before,&after),
        "real scroll probing preserves window scrollbar positions and child geometry");
    result=api.scrollBy(self,CSize(17,-11),TRUE);GetWindowRect(child,&after);
    check(result==TRUE && GetScrollPos(self->m_hWnd,SB_HORZ)==87 && GetScrollPos(self->m_hWnd,SB_VERT)==69 && after.left==before.left-17 && after.top==before.top+11,
        "real scroll movement updates both scrollbar positions and hidden child coordinates");
    check(api.scrollBy(self,CSize(0,0),TRUE)==FALSE,"real scroll movement reports no change for a stationary view");
    check(api.scroll(self,0xff01,0,TRUE)==TRUE && GetScrollPos(self->m_hWnd,SB_HORZ)==90 && GetScrollPos(self->m_hWnd,SB_VERT)==69,
        "real packed horizontal commands dispatch through the scrolling virtual");
    BOOL savedInitialized=*api.cacheInitialized;UINT savedLines=*api.cacheLines;*api.cacheInitialized=TRUE;*api.cacheLines=3;
    check(api.wheel(self,0,-120,CPoint())==TRUE && GetScrollPos(self->m_hWnd,SB_HORZ)==90 && GetScrollPos(self->m_hWnd,SB_VERT)==84,
        "real wheel input dispatches through the scrolling virtual and updates the vertical scrollbar");
    *api.cacheInitialized=savedInitialized;*api.cacheLines=savedLines;DestroyWindow(child);
}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
        printf("scroll view mismatch %zu local:",i);for(auto v:expected[i])printf(" %lld",v);printf(" native:");for(auto v:actual[i])printf(" %lld",v);puts("");break;
    }
    check(actual==expected,label);
}
}
static void scrollViewInput() {
    using namespace scroll_view_input_test;
    API local{&impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ,&impl__GetWheelScrollDistance_CScrollView__UEAA_AVCSize__V2_HH_Z,&impl__CheckScrollBars_CScrollView__QEBAXAEAH0_Z,
        &impl__OnScroll_CScrollView__UEAAHIIH_Z,&impl__OnScrollBy_CScrollView__UEAAHVCSize__H_Z,&impl__OnHScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z,
        &impl__OnVScroll_CScrollView__QEAAXIIPEAVCScrollBar___Z,&impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z,&impl__OnMouseWheel_CScrollView__QEAAHIFVCPoint___Z,
        &impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z,&impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z,&AfxGetThreadState,&openmfc::detail::scrollViewWheelSettingsInitialized,&openmfc::detail::scrollViewWheelLines};
    Imports imports(GetModuleHandleW(nullptr),true);expectations(local);
    auto bars=availabilityCases(local),returns=aggregates(local),packed=commands(local),moving=movement(local),reflected=reflection(local),wheel=wheels(local),splitter=lookup(local);realWindows(local);
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_INPUT_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for scroll view input comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"scroll view input oracle uses the verified MFC image build");
            if(pinned) {
                Imports nativeImports(native,false);API api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(device,0x288510);RVA(distance,0x288610);RVA(checkBars,0x2895e0);RVA(scroll,0x2897c0);RVA(scrollBy,0x289900);RVA(horizontal,0x2894a0);RVA(vertical,0x289510);RVA(wheel,0x289680);RVA(mouse,0x289590);RVA(bar,0x277b30);RVA(item,0x2a9390);RVA(state,0x1332a0);
#undef RVA
                api.cacheInitialized=reinterpret_cast<BOOL*>(b+0x3be42c);api.cacheLines=reinterpret_cast<UINT*>(b+0x3c41d0);
                api.graph.construct=reinterpret_cast<decltype(api.graph.construct)>(b+0x28a700);api.graph.attach=reinterpret_cast<decltype(api.graph.attach)>(b+0x28adf0);api.graph.detach=reinterpret_cast<decltype(api.graph.detach)>(b+0x28ae50);api.graph.destroy=reinterpret_cast<decltype(api.graph.destroy)>(b+0x28b740);api.graph.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);
                splitter_command_test::nativeLookupReturn=b+0x2a9da9;expectations(api);realWindows(api);
                compare(availabilityCases(api),bars,"scrollbar availability, output aliasing, style snapshots and OLE enabled checks match native MFC");
                compare(aggregates(api),returns,"scroll view hidden aggregate returns and vertical-first device positions match native MFC");
                compare(commands(api),packed,"packed scroll commands, signed arithmetic and virtual callback results match native MFC");
                compare(movement(api),moving,"scroll clamping, disabled controls, probe mode and interrupted physical scrolling match native MFC");
                compare(reflection(api),reflected,"scrollbar reflection, sender identity and virtual command dispatch match native MFC");
                compare(wheels(api),wheel,"wheel settings caching, page mode, signed deltas and reentrant settings queries match native MFC");
                compare(lookup(api),splitter,"splitter scrollbar IDs, OLE child lookup and mouse-wheel parent filtering match native MFC");
                splitter_command_test::nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
