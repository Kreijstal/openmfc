#pragma once
#include "detail/AuxDataSupport.h"
extern "C" {
void MS_ABI impl__GetScrollBarSizes_CScrollView__IEAAXAEAVCSize___Z(CScrollView*,SIZE*);
BOOL MS_ABI impl__GetTrueClientSize_CScrollView__IEAAHAEAVCSize__0_Z(CScrollView*,SIZE*,SIZE*);
void MS_ABI impl__GetDeviceScrollSizes_CScrollView__QEBAXAEAHAEAUtagSIZE__11_Z(const CScrollView*,int*,SIZE*,SIZE*,SIZE*);
void MS_ABI impl__GetScrollBarState_CScrollView__IEAAXVCSize__AEAV2_1AEAVCPoint__H_Z(CScrollView*,CSize,CSize*,CSize*,CPoint*,BOOL);
void MS_ABI impl__ScrollToDevicePosition_CScrollView__IEAAXUtagPOINT___Z(CScrollView*,POINT);
void MS_ABI impl__UpdateBars_CScrollView__IEAAXXZ(CScrollView*);
void MS_ABI impl__SetScaleToFitSize_CScrollView__QEAAXUtagSIZE___Z(CScrollView*,SIZE);
void MS_ABI impl__OnSize_CScrollView__QEAAXIHH_Z(CScrollView*,UINT,int,int);
void MS_ABI impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z(CScrollView*,RECT*,UINT);
void MS_ABI impl__EnableScrollBarCtrl_CWnd__QEAAXHH_Z(CWnd*,int,BOOL);
int MS_ABI impl__SetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__H_Z(CWnd*,int,SCROLLINFO*,BOOL);
void MS_ABI impl__SetScrollRange_CWnd__QEAAXHHHH_Z(CWnd*,int,int,int,BOOL);
BOOL MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd*,BOOL);
}
namespace scroll_view_sizing_test {
using namespace scroll_view_input_test;
using openmfc::detail::AuxData;
struct API {
    scroll_view_input_test::API input;
    decltype(&impl__GetScrollBarSizes_CScrollView__IEAAXAEAVCSize___Z) barSizes;
    decltype(&impl__GetTrueClientSize_CScrollView__IEAAHAEAVCSize__0_Z) clientSize;
    decltype(&impl__GetDeviceScrollSizes_CScrollView__QEBAXAEAHAEAUtagSIZE__11_Z) deviceSizes;
    decltype(&impl__GetScrollBarState_CScrollView__IEAAXVCSize__AEAV2_1AEAVCPoint__H_Z) barState;
    decltype(&impl__ScrollToDevicePosition_CScrollView__IEAAXUtagPOINT___Z) devicePosition;
    decltype(&impl__UpdateBars_CScrollView__IEAAXXZ) updateBars;
    decltype(&impl__SetScaleToFitSize_CScrollView__QEAAXUtagSIZE___Z) scale;
    decltype(&impl__OnSize_CScrollView__QEAAXIHH_Z) size;
    decltype(&impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z) calculate;
    decltype(&impl__EnableScrollBarCtrl_CWnd__QEAAXHH_Z) enableBars;
    decltype(&impl__SetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__H_Z) setInfo;
    decltype(&impl__SetScrollRange_CWnd__QEAAXHHHH_Z) setRange;
    decltype(&impl__EnableWindow_CWnd__QEAAHH_Z) enable;
    AuxData* data;
    void (*metrics)();void (*colors)();void (MS_ABI* cleanup)(void*);
};
static const API* activeAPI;
static void* nativeLookupReturn;
static bool hasParent,partialClient,specialMetric;
static RECT clientRectangle,parentRectangle;
static int clientCalls,apiResult;
static LRESULT parentReply;
static DWORD extendedStyle;
static INT64 windowID(HWND h) {int i=int(splitter_command_test::id(h));return i>=0?100+i:id(h);}
static decltype(&::GetClientRect) realClient;
static decltype(&::GetParent) realParent;
static decltype(&::SendMessageW) realSend;
static decltype(&::SetScrollInfo) realSetInfo;
static decltype(&::SetScrollRange) realSetRange;
static decltype(&::ShowScrollBar) realShow;
static decltype(&::EnableWindow) realEnable;
static decltype(&::AdjustWindowRectEx) realAdjust;
static decltype(&::InvalidateRect) realInvalidate;
static decltype(&::GetWindowLongW) previousStyle;
static decltype(&::GetSystemMetrics) realMetric;
static decltype(&::GetDC) realDC;
static decltype(&::GetDeviceCaps) realCaps;
static decltype(&::ReleaseDC) realRelease;
static decltype(&::GetSysColor) realColor;
static decltype(&::GetSysColorBrush) realBrush;
static decltype(&::LoadCursorW) realCursor;
static decltype(&::DeleteObject) realDelete;
static BOOL WINAPI client(HWND h,RECT* r) {
    if(!capture)return realClient(h,r);event(31,windowID(h),r->left,r->top,r->right,r->bottom,self->m_bInsideUpdate);++clientCalls;
    if(partialClient)r->right=clientRectangle.right;else *r=clientRectangle;
    if(mutation==30)activeAPI->updateBars(self);
    if(mutation==32)self->m_hWnd=nullptr;
    return FALSE;
}
static HWND WINAPI parent(HWND h) {
    if(_ReturnAddress()==nativeLookupReturn)return nullptr;
    if(!capture)return realParent(h);event(32,windowID(h),self->m_bInsideUpdate);
    if(mutation==38)self->m_hWnd=hwnd(12);
    return hasParent?splitter_command_test::handles[1]:nullptr;
}
static LRESULT WINAPI send(HWND h,UINT message,WPARAM w,LPARAM l) {
    if(!capture)return realSend(h,message,w,l);event(33,windowID(h),message,static_cast<INT64>(w),self->m_bInsideUpdate);
    if(message==0x368)*reinterpret_cast<RECT*>(l)=parentRectangle;
    if(mutation==31)self->m_totalDev={57,61};
    if(mutation==30)activeAPI->updateBars(self);
    return parentReply;
}
static LRESULT MS_ABI defaultProc(CWnd*,UINT message,WPARAM w,LPARAM l) {
    event(34,message,static_cast<INT64>(w),l,self->m_bInsideUpdate,self->m_nMapMode);
    if(mutation==35){self->m_nMapMode=-1;self->m_totalLog={77,91};}
    return -0x123456789LL;
}
static int WINAPI setInfo(HWND h,int axis,SCROLLINFO* info,BOOL redraw) {
    if(!capture)return realSetInfo(h,axis,info,redraw);event(39,windowID(h),axis,info->cbSize,info->fMask,info->nMin,info->nMax,info->nPage);event(139,redraw,self->m_bInsideUpdate);
    if(mutation==36){info->fMask=0x1234;info->nMin=-17;self->m_totalDev.cy=777;}
    return apiResult;
}
static BOOL WINAPI setRange(HWND h,int axis,int low,int high,BOOL redraw) {
    if(!capture)return realSetRange(h,axis,low,high,redraw);event(38,windowID(h),axis,low,high,redraw);return FALSE;
}
static BOOL WINAPI show(HWND h,int axis,BOOL enable) {
    if(!capture)return realShow(h,axis,enable);event(40,windowID(h),axis,enable,self->m_bInsideUpdate);
    if(axis==0 || axis==1){DWORD bit=axis==0?WS_HSCROLL:WS_VSCROLL;if(enable)styleBits|=bit;else styleBits&=~bit;}
    return FALSE;
}
static BOOL WINAPI enable(HWND h,BOOL on) {if(!capture)return realEnable(h,on);event(41,windowID(h),on,self->m_bInsideUpdate);return apiResult;}
static BOOL MS_ABI siteEnable(void* site,BOOL on) {int a=*reinterpret_cast<int*>(static_cast<char*>(site)+8);event(42,a,on,self->m_bInsideUpdate);return apiResult;}
static BOOL MS_ABI mainEnable(void*,BOOL on) {event(42,2,on,self->m_bInsideUpdate);return apiResult;}
static DWORD MS_ABI siteExtended(void*) {event(43);return extendedStyle;}
static BOOL WINAPI adjust(RECT* r,DWORD style,BOOL menu,DWORD extended) {
    if(!capture)return realAdjust(r,style,menu,extended);event(44,style,menu,extended,r->left,r->top,r->right,r->bottom);
    r->left=LONG(ULONG(r->left)-3u);r->top=LONG(ULONG(r->top)-5u);r->right=LONG(ULONG(r->right)+7u);r->bottom=LONG(ULONG(r->bottom)+11u);
    if(mutation==39)self->m_nMapMode=-1;return FALSE;
}
static LONG WINAPI style(HWND h,int which) {
    if(capture && which==GWL_EXSTYLE){event(45,windowID(h),which);return LONG(extendedStyle);}return previousStyle(h,which);
}
static BOOL WINAPI invalidate(HWND h,const RECT* r,BOOL erase) {if(!capture)return realInvalidate(h,r,erase);event(46,windowID(h),r!=nullptr,erase,self->m_bInsideUpdate);return FALSE;}
static int WINAPI metric(int index) {
    if(!capture)return realMetric(index);event(60,index);
    if(index==SM_CXICON)return 31;if(index==SM_CYICON)return 33;if(index==SM_CXVSCROLL)return specialMetric?INT_MAX:17;return 19;
}
static HDC WINAPI dc(HWND h) {if(!capture)return realDC(h);event(61,windowID(h));return reinterpret_cast<HDC>(UINT_PTR(71));}
static int WINAPI caps(HDC h,int which) {if(!capture)return realCaps(h,which);event(which==LOGPIXELSX?62:63,INT64(UINT_PTR(h)),which);return which==LOGPIXELSX?123:145;}
static int WINAPI release(HWND h,HDC dc) {if(!capture)return realRelease(h,dc);event(64,windowID(h),INT64(UINT_PTR(dc)));return 0;}
static DWORD WINAPI color(int index) {if(!capture)return realColor(index);event(65,index);return 0x12340000u+index;}
static HBRUSH WINAPI brush(int index) {if(!capture)return realBrush(index);event(66,index);return reinterpret_cast<HBRUSH>(UINT_PTR(700+index));}
static HCURSOR WINAPI cursor(HINSTANCE instance,const wchar_t* name) {if(!capture)return realCursor(instance,name);event(67,INT64(UINT_PTR(instance)),INT64(UINT_PTR(name)));return reinterpret_cast<HCURSOR>(UINT_PTR(name));}
static BOOL WINAPI deleteObject(HGDIOBJ h) {if(!capture)return realDelete(h);event(68,INT64(UINT_PTR(h)));return FALSE;}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate scroll sizing import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetClientRect",client,realClient);HOOK("GetParent",parent,realParent);HOOK("SendMessageW",send,realSend);
        HOOK("SetScrollInfo",setInfo,realSetInfo);HOOK("SetScrollRange",setRange,realSetRange);HOOK("ShowScrollBar",show,realShow);HOOK("EnableWindow",enable,realEnable);
        HOOK("AdjustWindowRectEx",adjust,realAdjust);HOOK("InvalidateRect",invalidate,realInvalidate);HOOK("GetWindowLongW",style,previousStyle);
        HOOK("GetSystemMetrics",metric,realMetric);HOOK("GetDC",dc,realDC);HOOK("GetDeviceCaps",caps,realCaps);HOOK("ReleaseDC",release,realRelease);
        HOOK("GetSysColor",color,realColor);HOOK("GetSysColorBrush",brush,realBrush);HOOK("LoadCursorW",cursor,realCursor);HOOK("DeleteObject",deleteObject,realDelete);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    scroll_view_input_test::Fixture f;
    std::unique_ptr<splitter_command_test::Fixture> graph;
    const API& api;AuxData saved;
    void* siteTable[45]{};struct Site{void** table;int axis;}sites[2];
    void* mainSiteTable[45]{};void** mainSite=mainSiteTable;
    Fixture(const API& functions):f(functions.input),api(functions),saved(*api.data) {
        capture=false;graph.reset(new splitter_command_test::Fixture(api.input.graph));
        siteTable[43]=reinterpret_cast<void*>(&scroll_view_input_test::siteEnabled);siteTable[44]=reinterpret_cast<void*>(&siteEnable);
        for(int i=0;i<2;++i)sites[i]={siteTable,i};mainSiteTable[32]=reinterpret_cast<void*>(&scroll_view_input_test::siteStyle);mainSiteTable[33]=reinterpret_cast<void*>(&siteExtended);mainSiteTable[44]=reinterpret_cast<void*>(&mainEnable);
        f.table[73]=reinterpret_cast<void*>(&defaultProc);reset();capture=true;
    }
    void reset() {
        f.resetCase();f.table[32]=reinterpret_cast<void*>(&scroll_view_input_test::bar);activeAPI=&api;self->m_nMapMode=MM_TEXT;self->m_totalLog={100,110};self->m_totalDev={140,160};self->m_pageDev={50,60};self->m_lineDev={3,5};self->m_bInsideUpdate=FALSE;
        api.data->cxVScroll=17;api.data->cyHScroll=19;hasParent=partialClient=specialMetric=false;clientCalls=0;apiResult=0;parentReply=0;extendedStyle=WS_EX_CLIENTEDGE|WS_EX_LAYOUTRTL;
        clientRectangle={-3,5,101,103};parentRectangle={3,5,101,103};
    }
    void ole() {self->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&mainSite);for(int i=0;i<2;++i)controls[i]->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&sites[i]);}
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        f.save(out,result,caught);
        out.push_back({94,self->m_nMapMode,self->m_totalLog.cx,self->m_totalLog.cy,self->m_totalDev.cx,self->m_totalDev.cy,self->m_bInsideUpdate,styleBits});
    }
    ~Fixture(){capture=false;failure=0;*api.data=saved;graph.reset();}
};
static std::vector<Event> setters(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int operation:{0,1,2,3})for(int axis:{-1,0,1,2,3})for(int h:{0,2})for(int v:{0,2})for(BOOL enable:{FALSE,BOOL(69),BOOL(-1)})for(bool ole:{false,true})for(int mode:{0,2}) {
        f.reset();availability[0]=h;availability[1]=v;if(ole)f.ole();mutation=mode;apiResult=-7;
        // The input fixture's control callback accepts native H/V axes. For
        // invalid/SB_CTL axes use a null-control callback with full logging.
        if((axis!=0 && axis!=1) && (operation!=0 || axis!=3))f.f.table[32]=reinterpret_cast<void*>(+[](const CWnd*,int a)->CScrollBar* {event(1,a,-1);if(mutation==2)self->m_hWnd=hwnd(12);return nullptr;});
        else f.f.table[32]=reinterpret_cast<void*>(&scroll_view_input_test::bar);
        SCROLLINFO info{999,SIF_RANGE|SIF_PAGE,-37,123,17,41,43};int result=17;
        if(operation==0)api.enableBars(self,axis,enable);
        if(operation==1)result=api.setInfo(self,axis,&info,enable);
        if(operation==2)api.setRange(self,axis,INT_MIN,INT_MAX,enable);
        if(operation==3)result=api.enable(self,enable);
        f.save(out,result);out.push_back({95,info.cbSize,info.fMask,info.nMin,info.nMax,info.nPage,info.nPos,info.nTrackPos});
    }
    for(int operation:{0,1,2,3})for(int at:{0,1,38,39,40,41,42}) {f.reset();failure=at;SCROLLINFO info{0,SIF_PAGE};int result=17,caught=invoke([&]{if(operation==0)api.enableBars(self,3,-1);if(operation==1)result=api.setInfo(self,0,&info,69);if(operation==2)api.setRange(self,0,-3,7,-1);if(operation==3)result=api.enable(self,-1);});f.save(out,result,caught);}return out;
}
static std::vector<Event> dimensions(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(DWORD bits:{DWORD(0),DWORD(WS_BORDER),DWORD(WS_HSCROLL),DWORD(WS_VSCROLL),DWORD(WS_BORDER|WS_HSCROLL|WS_VSCROLL)})for(int h:{0,2})for(int v:{0,2})for(int metric:{0,1,17,INT_MIN,INT_MAX})for(bool alias:{false,true})for(bool ole:{false,true}) {
        f.reset();styleBits=bits;availability[0]=h;availability[1]=v;api.data->cxVScroll=metric;api.data->cyHScroll=metric;if(ole)f.ole();SIZE size{37,41},bars{43,47};
        BOOL result=api.clientSize(self,&size,alias?&size:&bars);f.save(out,result);out.push_back({95,size.cx,size.cy,bars.cx,bars.cy,0,0,0});
    }
    for(int mode:{0,1,2})for(int at:{0,1,5,6,10,11,31}) {f.reset();mutation=mode;failure=at;availability[0]=availability[1]=2;SIZE size{37,41},bars{43,47};int result=17,caught=invoke([&]{result=api.clientSize(self,&size,&bars);});f.save(out,result,caught);out.push_back({95,size.cx,size.cy,bars.cx,bars.cy,0,0,0});}
    for(int alias:{0,1,2,3}) {f.reset();self->m_nMapMode=MM_ANISOTROPIC;SIZE a{1,2},b{3,4},c{5,6};int mode=-1;
        api.deviceSizes(self,&mode,&a,alias&1?&a:&b,alias&2?&a:&c);f.save(out,mode);out.push_back({95,a.cx,a.cy,b.cx,b.cy,c.cx,c.cy,0});}
    return out;
}
static std::vector<Event> states(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int total:{-7,0,97,100,101,INT_MAX})for(int extent:{0,97,100,INT_MIN})for(BOOL inside:{FALSE,BOOL(69),BOOL(-1)})for(int aliases:{0,1,2,3,4})for(int at:{0,1,12}) {
        f.reset();self->m_totalDev={total,total};failure=at;CSize need(37,41),range(43,47);CPoint move(53,59);
        CSize* rangeOut=aliases==1 || aliases==4?&need:&range;CPoint* moveOut=aliases==2?reinterpret_cast<CPoint*>(&need):aliases>=3?reinterpret_cast<CPoint*>(rangeOut):&move;
        int caught=invoke([&]{api.barState(self,CSize(extent,extent),&need,rangeOut,moveOut,inside);});f.save(out,0,caught);out.push_back({95,need.cx,need.cy,range.cx,range.cy,move.x,move.y,0});
    }
    for(int x:{INT_MIN,-7,0,7,INT_MAX})for(int y:{INT_MIN,-11,0,11,INT_MAX})for(int at:{0,1,12,13,16,17}) {
        f.reset();failure=at;int caught=invoke([&]{api.devicePosition(self,POINT{x,y});});f.save(out,0,caught);
    }
    return out;
}
static std::vector<Event> updating(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int parent:{0,1,2,3})for(int extent:{-1,0,1,17,97,101})for(int total:{0,97,140})for(int h:{0,2})for(int v:{0,2})for(BOOL guard:{FALSE,BOOL(69)}) {
        f.reset();hasParent=parent!=0;parentReply=parent==1?0:parent==2?69:0x100000000LL;clientRectangle.right=clientRectangle.bottom=extent;
        self->m_totalDev={total,total};availability[0]=h;availability[1]=v;self->m_bInsideUpdate=guard;api.updateBars(self);f.save(out,0);
    }
    for(int mode:{0,1,2,30,31,36,38})for(int at:{0,1,10,12,13,16,17,31,32,33,39,40}) {
        f.reset();hasParent=true;parentReply=mode==31?69:0;failure=at;mutation=mode;int caught=invoke([&]{api.updateBars(self);});f.save(out,0,caught);
        failure=0;events.clear();api.updateBars(self);f.save(out,0); // Interrupted updates keep their guard set.
    }
    return out;
}
static std::vector<Event> scaling(const API& api) {
    Fixture f(api);std::vector<Event> out;MSG saved=api.input.state()->m_lastSentMsg;
    api.input.state()->m_lastSentMsg.message=WM_SIZE;api.input.state()->m_lastSentMsg.wParam=0xfedcba9876543210ULL;api.input.state()->m_lastSentMsg.lParam=-0x123456789abcdefLL;
    for(bool attached:{false,true})for(DWORD bits:{DWORD(0),DWORD(WS_HSCROLL|WS_VSCROLL)})for(int mode:{0,30,32,35})for(int operation:{0,1})for(int map:{-1,MM_TEXT}) {
        f.reset();styleBits=bits;mutation=mode;self->m_nMapMode=map;if(!attached)self->m_hWnd=nullptr;
        if(operation==0)api.scale(self,SIZE{77,-91});else api.size(self,0xfedcba98,123,-456);f.save(out,0);
    }
    for(int at:{1,10,13,31,32,34,39,40,46}) {f.reset();failure=at;self->m_nMapMode=-1;int caught=invoke([&]{api.size(self,SIZE_MAXIMIZED,123,456);});f.save(out,0,caught);}
    api.input.state()->m_lastSentMsg=saved;return out;
}
static std::vector<Event> rectangles(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(UINT kind:{0u,1u,2u,UINT_MAX})for(int map:{-1,MM_TEXT})for(int extent:{-1,37,INT_MAX})for(bool ole:{false,true})for(int mode:{0,1,2,39}) {
        f.reset();self->m_nMapMode=map;self->m_totalDev={extent,extent};mutation=mode;if(ole)f.ole();RECT r{3,5,41,43};api.calculate(self,&r,kind);f.save(out,0);out.push_back({95,r.left,r.top,r.right,r.bottom,0,0,0});
    }
    for(int at:{1,5,10,43,44,45}) {f.reset();failure=at;RECT r{3,5,41,43};int caught=invoke([&]{api.calculate(self,&r,1);});f.save(out,0,caught);out.push_back({95,r.left,r.top,r.right,r.bottom,0,0,0});}return out;
}
static std::vector<Event> cachedData(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int operation:{0,1,2})for(int at:{0,60,61,62,63,64,65,66,68})for(bool overflow:{false,true}) {
        if(operation==2 && at==68)continue; // Destructor termination is checked in a child process.
        f.reset();specialMetric=overflow;failure=at;memset(api.data,0xa5,sizeof(AuxData));if(operation==2)api.data->hbmMenuDot=reinterpret_cast<HBITMAP>(UINT_PTR(79));
        int caught=invoke([&]{if(operation==0)api.metrics();if(operation==1)api.colors();if(operation==2)api.cleanup(api.data);});f.save(out,0,caught);
        UINT64 bytes[15];memcpy(bytes,api.data,sizeof(bytes));for(int i=0;i<15;i+=3)out.push_back({96,INT64(bytes[i]),INT64(bytes[i+1]),INT64(bytes[i+2]),0,0,0,0});
    }
    return out;
}
static void expectations(const API& api) {
    Fixture f(api);SCROLLINFO info{0,SIF_RANGE|SIF_PAGE,-3,17,5,0,0};
    check(api.setInfo(self,0,&info,69)==TRUE && info.cbSize==sizeof(info),"native scroll-info wrappers report success when the previous Win32 position is zero");
    f.reset();availability[0]=2;api.enableBars(self,SB_BOTH,-1);
    check(events.size()==4 && events[1][0]==41 && events[1][2]==-1 && events[3][0]==40 && events[3][2]==SB_VERT,
        "both-axis scrollbar enabling visits horizontal then vertical controls and preserves BOOL arguments");
    f.reset();SIZE bars{};api.barSizes(self,&bars);check(bars.cx==17 && bars.cy==19,"scrollbar sizing uses cached AUX_DATA metrics");
    f.reset();styleBits|=WS_BORDER;api.barSizes(self,&bars);check(bars.cx==16 && bars.cy==18,"bordered views remove the cached scrollbar border allowance");
    f.reset();SIZE size{};BOOL enough=api.clientSize(self,&size,&bars);check(enough==TRUE && size.cx==118 && size.cy==122,"true client sizing restores space occupied by visible window scrollbars");
    f.reset();failure=31;int caught=invoke([&]{api.updateBars(self);});check(caught==91 && self->m_bInsideUpdate==TRUE,"interrupted scrollbar updates retain the native recursion guard");
    failure=0;events.clear();api.updateBars(self);check(events.empty(),"a retained scrollbar update guard suppresses reentry");
    f.reset();api.updateBars(self);check(self->m_bInsideUpdate==FALSE && positions[0]==7 && positions[1]==11,"successful scrollbar updates clear their recursion guard");
}
static void localCleanup(void*) {openmfc::detail::CleanupAuxData();}
static BOOL WINAPI throwingDelete(HGDIOBJ) {throw 91;}
static void termination(bool native) {
    wchar_t executable[2048];GetModuleFileNameW(nullptr,executable,2048);std::wstring command=L"\""+std::wstring(executable)+L"\"";
    SetEnvironmentVariableW(L"OPENMFC_AUX_CLEANUP_CHILD",native?L"1":L"0");STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION process{};
    BOOL created=CreateProcessW(executable,&command[0],nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&process);SetEnvironmentVariableW(L"OPENMFC_AUX_CLEANUP_CHILD",nullptr);
    check(created!=FALSE,"start AUX_DATA cleanup termination child");if(!created)return;
    DWORD waited=WaitForSingleObject(process.hProcess,15000);if(waited!=WAIT_OBJECT_0)TerminateProcess(process.hProcess,99);DWORD code=0;GetExitCodeProcess(process.hProcess,&code);CloseHandle(process.hThread);CloseHandle(process.hProcess);
    check(waited==WAIT_OBJECT_0 && code==3,"cached bitmap cleanup preserves the native no-throw destructor boundary");
}
static void defaultCache() {
    auto& data=openmfc::detail::GetAuxData();
    check(data.cxVScroll==realMetric(SM_CXVSCROLL)+1 && data.cyHScroll==realMetric(SM_CYHSCROLL)+1 && data.cxBorder2==2 && data.cyBorder2==2,
        "AUX_DATA is initialized with real cached scrollbar metrics and border widths before use");
    check(data.clrBtnFace==realColor(COLOR_BTNFACE) && data.clrBtnText==realColor(COLOR_BTNTEXT) && data.hbrBtnFace==realBrush(COLOR_BTNFACE) && data.hcurWait==realCursor(nullptr,IDC_WAIT),
        "AUX_DATA initializes native system colors, brushes and shared cursors");
}
static WNDPROC previousParentProcedure;
static LRESULT CALLBACK parentProcedure(HWND h,UINT message,WPARAM w,LPARAM l) {
    if(message==0x368){*reinterpret_cast<RECT*>(l)={3,5,133,105};return 69;}
    return CallWindowProcW(previousParentProcedure,h,message,w,l);
}
static void realWindows(const API& api) {
    Fixture f(api);capture=false;splitter_command_test::capture=false;self->m_hWnd=splitter_command_test::handles[3];
    api.data->cxVScroll=f.saved.cxVScroll;api.data->cyHScroll=f.saved.cyHScroll;
    RECT initial{};GetClientRect(self->m_hWnd,&initial);self->m_totalDev={340,260};self->m_totalLog={740,520};
    api.updateBars(self);SCROLLINFO horizontal{sizeof(SCROLLINFO),SIF_RANGE|SIF_PAGE},vertical=horizontal;
    GetScrollInfo(self->m_hWnd,SB_HORZ,&horizontal);GetScrollInfo(self->m_hWnd,SB_VERT,&vertical);
    check(horizontal.nMin==0 && horizontal.nMax==339 && horizontal.nPage==UINT(initial.right-api.data->cxVScroll) && vertical.nMax==259 && vertical.nPage==UINT(initial.bottom-api.data->cyHScroll),
        "real scroll sizing installs native ranges and page sizes on both window scrollbars");
    SetScrollPos(self->m_hWnd,SB_HORZ,51,FALSE);SetScrollPos(self->m_hWnd,SB_VERT,73,FALSE);api.scale(self,SIZE{640,480});RECT after{};GetClientRect(self->m_hWnd,&after);
    check(self->m_nMapMode==-1 && self->m_totalLog.cx==640 && self->m_totalLog.cy==480 && self->m_totalDev.cx==after.right && self->m_totalDev.cy==after.bottom && !(GetWindowLongW(self->m_hWnd,GWL_STYLE)&(WS_HSCROLL|WS_VSCROLL)) && self->m_bInsideUpdate==FALSE,
        "real scale-to-fit sizing removes scrollbars and records the current client extent");
    HWND parent=splitter_command_test::handles[1];previousParentProcedure=reinterpret_cast<WNDPROC>(SetWindowLongPtrW(parent,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&parentProcedure)));
    self->m_nMapMode=MM_TEXT;self->m_totalDev={340,260};api.updateBars(self);horizontal.fMask=vertical.fMask=SIF_RANGE|SIF_PAGE;
    GetScrollInfo(self->m_hWnd,SB_HORZ,&horizontal);GetScrollInfo(self->m_hWnd,SB_VERT,&vertical);
    check(horizontal.nPage==UINT(130-api.data->cxVScroll) && vertical.nPage==UINT(100-api.data->cyHScroll) && self->m_bInsideUpdate==FALSE,
        "real scrollbar updates honor the parent window's WM_RECALCPARENT rectangle");
    SetWindowLongPtrW(parent,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(previousParentProcedure));
}
}
static bool auxDataTerminationChild() {
    wchar_t mode[8];if(!GetEnvironmentVariableW(L"OPENMFC_AUX_CLEANUP_CHILD",mode,8))return false;
    SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);std::set_terminate([]{ExitProcess(3);});
    HMODULE module=GetModuleHandleW(nullptr);void* data=&openmfc::detail::GetAuxData();void (MS_ABI* cleanup)(void*)=&scroll_view_sizing_test::localCleanup;
    if(_wtoi(mode)) {
        wchar_t path[2048];if(!GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_SIZING_DLL",path,2048))ExitProcess(95);module=LoadLibraryW(path);if(!module)ExitProcess(94);
        auto* b=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
        if(pe->FileHeader.TimeDateStamp!=0x442cba30 || pe->OptionalHeader.SizeOfImage!=0x55c000)ExitProcess(93);
        data=b+0x3c32c0;cleanup=reinterpret_cast<decltype(cleanup)>(b+0x1d5d60);
    }
    auto** slot=recordset_value_test::import(module,"DeleteObject");if(!slot)ExitProcess(92);recordset_value_test::Patch patch(slot,reinterpret_cast<void*>(&scroll_view_sizing_test::throwingDelete));
    static_cast<openmfc::detail::AuxData*>(data)->hbmMenuDot=reinterpret_cast<HBITMAP>(UINT_PTR(79));try{cleanup(data);}catch(...){ExitProcess(97);}ExitProcess(98);
}
static void scrollViewSizing() {
    using namespace scroll_view_sizing_test;
    scroll_view_input_test::API input{};input.cacheInitialized=&openmfc::detail::scrollViewWheelSettingsInitialized;input.cacheLines=&openmfc::detail::scrollViewWheelLines;input.state=&AfxGetThreadState;
    scroll_view_sizing_test::API local{input,&impl__GetScrollBarSizes_CScrollView__IEAAXAEAVCSize___Z,&impl__GetTrueClientSize_CScrollView__IEAAHAEAVCSize__0_Z,&impl__GetDeviceScrollSizes_CScrollView__QEBAXAEAHAEAUtagSIZE__11_Z,
        &impl__GetScrollBarState_CScrollView__IEAAXVCSize__AEAV2_1AEAVCPoint__H_Z,&impl__ScrollToDevicePosition_CScrollView__IEAAXUtagPOINT___Z,&impl__UpdateBars_CScrollView__IEAAXXZ,
        &impl__SetScaleToFitSize_CScrollView__QEAAXUtagSIZE___Z,&impl__OnSize_CScrollView__QEAAXIHH_Z,&impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z,
        &impl__EnableScrollBarCtrl_CWnd__QEAAXHH_Z,&impl__SetScrollInfo_CWnd__QEAAHHPEAUtagSCROLLINFO__H_Z,&impl__SetScrollRange_CWnd__QEAAXHHHH_Z,&impl__EnableWindow_CWnd__QEAAHH_Z,
        &openmfc::detail::GetAuxData(),&openmfc::detail::UpdateAuxDataMetrics,&openmfc::detail::UpdateAuxDataColors,&localCleanup};
    scroll_view_input_test::Imports baseImports(GetModuleHandleW(nullptr),true);scroll_view_sizing_test::Imports imports(GetModuleHandleW(nullptr),true);defaultCache();expectations(local);scroll_view_sizing_test::termination(false);
    auto windowSetters=setters(local),client=dimensions(local),state=states(local),updates=updating(local),scale=scaling(local),rect=rectangles(local),aux=cachedData(local);realWindows(local);
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_SIZING_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for scroll view sizing comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"scroll sizing oracle uses the verified MFC image build");
            if(pinned) {
                scroll_view_input_test::Imports nativeBaseImports(native,false);scroll_view_sizing_test::Imports nativeImports(native,false);scroll_view_sizing_test::API api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(barSizes,0x288f70);RVA(clientSize,0x289000);RVA(deviceSizes,0x288560);RVA(barState,0x2890b0);RVA(devicePosition,0x288590);RVA(updateBars,0x2891b0);RVA(scale,0x2880a0);RVA(size,0x2888f0);RVA(calculate,0x2893f0);
                RVA(enableBars,0x28e820);RVA(setInfo,0x28e890);RVA(setRange,0x28e760);RVA(enable,0x2a9b30);RVA(metrics,0x1d5e40);RVA(colors,0x1d5dbc);RVA(cleanup,0x1d5d60);
#undef RVA
                api.data=reinterpret_cast<AuxData*>(b+0x3c32c0);api.input.state=reinterpret_cast<decltype(api.input.state)>(b+0x1332a0);
                check(memcmp(api.data,&openmfc::detail::GetAuxData(),sizeof(AuxData))==0,"default AUX_DATA storage matches the native initialized object");
                api.input.graph.construct=reinterpret_cast<decltype(api.input.graph.construct)>(b+0x28a700);api.input.graph.attach=reinterpret_cast<decltype(api.input.graph.attach)>(b+0x28adf0);api.input.graph.detach=reinterpret_cast<decltype(api.input.graph.detach)>(b+0x28ae50);api.input.graph.destroy=reinterpret_cast<decltype(api.input.graph.destroy)>(b+0x28b740);api.input.graph.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);
                nativeLookupReturn=b+0x2a9da9;expectations(api);realWindows(api);
                scroll_view_input_test::compare(setters(api),windowSetters,"scrollbar setters, enable/show routing, BOOL arguments and handle snapshots match native MFC");
                scroll_view_input_test::compare(dimensions(api),client,"client sizes, cached metrics, aliased outputs and device-size getters match native MFC");
                scroll_view_input_test::compare(states(api),state,"scrollbar state calculation, output aliasing and device movement ordering match native MFC");
                scroll_view_input_test::compare(updating(api),updates,"scrollbar updates, parent negotiation, callback mutations and retained recursion guards match native MFC");
                scroll_view_input_test::compare(scaling(api),scale,"scale-to-fit sizing and default-first size handling match native MFC");
                scroll_view_input_test::compare(rectangles(api),rect,"scroll-view rectangle adjustment, style filtering and callback failures match native MFC");
                scroll_view_input_test::compare(cachedData(api),aux,"AUX_DATA metric/color refresh and owned bitmap cleanup match native MFC");
                scroll_view_sizing_test::termination(true);
                nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
