#pragma once
extern "C" {
CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC*,CWnd*);
void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC*);
CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC*,CWnd*);
void MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC*);
CRuntimeClass* MS_ABI impl__GetThisClass_CWindowDC__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWindowDC__UEBAPEAUCRuntimeClass__XZ(const CWindowDC*);
CRuntimeClass* MS_ABI impl__GetThisClass_CClientDC__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* MS_ABI impl__GetRuntimeClass_CClientDC__UEBAPEAUCRuntimeClass__XZ(const CClientDC*);
void MS_ABI impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z(CScrollView*,int,SIZE,const SIZE*,const SIZE*);
CPoint* MS_ABI impl__GetScrollPosition_CScrollView__QEBA_AVCPoint__XZ(const CScrollView*,CPoint*);
void MS_ABI impl__ScrollToPosition_CScrollView__QEAAXUtagPOINT___Z(CScrollView*,POINT);
void MS_ABI impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z(CScrollView*,CPoint);
void MS_ABI impl__FillOutsideRect_CScrollView__QEAAXPEAVCDC__PEAVCBrush___Z(CScrollView*,CDC*,CBrush*);
}
namespace scroll_view_coordinate_test {
using namespace scroll_view_sizing_test;
struct API {
    scroll_view_sizing_test::API sizing;
    decltype(&impl___0CWindowDC__QEAA_PEAVCWnd___Z) window;
    decltype(&impl___1CWindowDC__UEAA_XZ) destroyWindow;
    decltype(&impl___0CClientDC__QEAA_PEAVCWnd___Z) client;
    decltype(&impl___1CClientDC__UEAA_XZ) destroyClient;
    decltype(&impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z) sizes;
    decltype(&impl__GetScrollPosition_CScrollView__QEBA_AVCPoint__XZ) position;
    decltype(&impl__ScrollToPosition_CScrollView__QEAAXUtagPOINT___Z) scroll;
    decltype(&impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z) center;
    decltype(&impl__FillOutsideRect_CScrollView__QEAAXPEAVCDC__PEAVCBrush___Z) fill;
    decltype(&impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z) lookup;
    decltype(&impl__DeleteTempMap_CDC__SAXXZ) clear;
    decltype(&impl__Attach_CDC__QEAAHPEAUHDC_____Z) attach;
    decltype(&impl__Detach_CDC__QEAAPEAUHDC____XZ) detach;
};
static const API* active;
static HDC borrowed,attribute;
static CDC* pending;
static CDC* mappedDC;
static CBrush* drawingBrush;
static CDC* drawingDC;
static bool acquireFails;
static int conversions;
static decltype(&::GetWindowDC) realWindowDC;
static decltype(&::GetDC) previousClientDC;
static decltype(&::ReleaseDC) previousRelease;
static decltype(&::DeleteDC) realDelete;
static decltype(&::SetMapMode) realMapMode;
static decltype(&::LPtoDP) realLogical;
static decltype(&::DPtoLP) realDevice;
static decltype(&::IsRectEmpty) realEmpty;
static decltype(&::FillRect) realFill;
static INT64 dcID(HDC h) {return !h?0:h==borrowed?1:h==attribute?2:-1;}
static int type(CDC* dc) {if(!dc)return -1;const char* name=dc->GetRuntimeClass()->m_lpszClassName;return strcmp(name,"CWindowDC")==0?1:strcmp(name,"CClientDC")==0?2:0;}
static HDC acquire(HWND h,int which) {
    if(mutation==21 && pending){pending->m_hDC=borrowed;pending->m_hAttribDC=attribute;pending->m_bPrinting=69;}
    event(which,windowID(h),pending?dcID(pending->m_hDC):-1,pending?pending->m_bPrinting:-1,pending?type(pending):-1);
    if(mutation==10)self->m_nMapMode=MM_LOMETRIC;
    if(mutation==11)self->m_hWnd=nullptr;
    if(mutation==12)self->m_hWnd=hwnd(12);
    return acquireFails?nullptr:borrowed;
}
static HDC WINAPI windowDC(HWND h) {return capture?acquire(h,80):realWindowDC(h);}
static HDC WINAPI clientDC(HWND h) {return capture?acquire(h,81):previousClientDC(h);}
static int WINAPI release(HWND h,HDC dc) {
    if(!capture)return previousRelease(h,dc);
    CDC* object=pending?pending:mappedDC;BOOL removed=dc?active->lookup(dc)!=object:TRUE;
    event(82,windowID(h),dcID(dc),object?dcID(object->m_hDC):-1,object?dcID(object->m_hAttribDC):-1,object?type(object):-1,removed,self->m_totalDev.cx);
    if(mutation==13){object->m_hDC=attribute;object->m_hAttribDC=attribute;}
    if(mutation==14){self->m_pageDev={0,0};self->m_lineDev={0,0};self->m_totalDev={197,211};}
    return 0;
}
static BOOL WINAPI deleteDC(HDC dc) {if(!capture)return realDelete(dc);event(83,dcID(dc));return FALSE;}
static int WINAPI mapMode(HDC dc,int mode) {
    if(!capture)return realMapMode(dc,mode);mappedDC=active->lookup(borrowed);
    event(84,dcID(dc),mode,type(mappedDC),mappedDC->m_bPrinting);
    if(mutation==20)mappedDC->m_hAttribDC=attribute;
    return 0;
}
static BOOL transform(HDC dc,POINT* point,int count,bool logical) {
    event(logical?85:86,dcID(dc),count,point->x,point->y,self->m_nMapMode,self->m_bInsideUpdate);++conversions;
    if(logical){point->x=LONG(ULONG(point->x)*3u+7u);point->y=LONG(ULONG(point->y)*UINT(-2)+11u);}
    else{point->x=LONG(ULONG(point->x)-7u)/3;point->y=LONG(ULONG(point->y)-11u)/-2;}
    if(mutation==15 && conversions==1){self->m_pageDev={17,19};self->m_lineDev={23,29};}
    if(mutation==16)self->m_nMapMode=MM_TEXT;
    return FALSE;
}
static BOOL WINAPI logical(HDC dc,POINT* p,int n) {return capture?transform(dc,p,n,true):realLogical(dc,p,n);}
static BOOL WINAPI device(HDC dc,POINT* p,int n) {return capture?transform(dc,p,n,false):realDevice(dc,p,n);}
static BOOL WINAPI empty(const RECT* r) {if(!capture)return realEmpty(r);event(87,r->left,r->top,r->right,r->bottom);return realEmpty(r);}
static int WINAPI fill(HDC dc,const RECT* r,HBRUSH brush) {
    if(!capture)return realFill(dc,r,brush);event(88,dcID(dc),r->left,r->top,r->right,r->bottom,INT64(UINT_PTR(brush)));
    if(mutation==17){self->m_totalDev={31,37};drawingDC->m_hDC=attribute;if(drawingBrush)drawingBrush->m_hObject=reinterpret_cast<HGDIOBJ>(UINT_PTR(79));}
    return 0;
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate scroll coordinate import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetWindowDC",windowDC,realWindowDC);HOOK("GetDC",clientDC,previousClientDC);HOOK("ReleaseDC",release,previousRelease);HOOK("DeleteDC",deleteDC,realDelete);
        HOOK("SetMapMode",mapMode,realMapMode);HOOK("LPtoDP",logical,realLogical);HOOK("DPtoLP",device,realDevice);HOOK("IsRectEmpty",empty,realEmpty);HOOK("FillRect",fill,realFill);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    scroll_view_sizing_test::Fixture f;const API& api;
    Fixture(const API& functions):f(functions.sizing),api(functions) {
        capture=false;borrowed=CreateCompatibleDC(nullptr);attribute=CreateCompatibleDC(nullptr);check(borrowed && attribute,"create real HDCs for scroll coordinate comparisons");reset();capture=true;
    }
    void cleanupMap() {
        // Foreign exceptions leave the native map pointing at abandoned stack
        // storage. Replace those entries before inspecting another test case.
        CDC replacement;
        api.attach(&replacement,borrowed);api.detach(&replacement);
        api.attach(&replacement,attribute);api.detach(&replacement);
    }
    void reset() {capture=false;cleanupMap();f.reset();active=&api;pending=mappedDC=nullptr;drawingBrush=nullptr;drawingDC=nullptr;acquireFails=false;conversions=0;self->m_bInsideUpdate=TRUE;capture=true;}
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        f.save(out,result,caught);out.push_back({97,self->m_pageDev.cx,self->m_pageDev.cy,self->m_lineDev.cx,self->m_lineDev.cy,0,0,0});
    }
    ~Fixture(){capture=false;failure=0;cleanupMap();api.clear();DeleteDC(borrowed);DeleteDC(attribute);}
};
template<class F>static int invoke(F action){try{action();return 0;}catch(int n){return n;}catch(CException* e){e->Delete();return 92;}}
static std::vector<Event> lifetimes(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(bool client:{false,true})for(bool window:{false,true})for(bool failed:{false,true})for(int mode:{0,12,13,21})for(int at:{0,80,81}) {
        f.reset();alignas(8) BYTE bytes[48];memset(bytes,0xa5,sizeof(bytes));pending=reinterpret_cast<CDC*>(bytes);acquireFails=failed;mutation=mode;failure=at;bool constructed=false;
        int caught=invoke([&]{if(client)api.client(reinterpret_cast<CClientDC*>(pending),window?self:nullptr);else api.window(reinterpret_cast<CWindowDC*>(pending),window?self:nullptr);constructed=true;});
        if(constructed){event(89,api.lookup(borrowed)==pending,type(pending));pending->m_hAttribDC=attribute;self->m_hWnd=hwnd(12);if(client)api.destroyClient(reinterpret_cast<CClientDC*>(pending));else api.destroyWindow(reinterpret_cast<CWindowDC*>(pending));}
        event(98,dcID(pending->m_hDC),dcID(pending->m_hAttribDC),pending->m_bPrinting,windowID(*reinterpret_cast<HWND*>(bytes+32)),type(pending));
        f.save(out,constructed,caught);out.push_back({99,*reinterpret_cast<UINT64*>(bytes+40)==0xa5a5a5a5a5a5a5a5ULL,*reinterpret_cast<DWORD*>(bytes+28)==0xa5a5a5a5u,0,0,0,0,0});
        failure=0;pending=nullptr;api.clear();
    }
    return out;
}
static std::vector<Event> sizes(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int mode:{MM_TEXT,MM_LOMETRIC,MM_ANISOTROPIC,-1})for(int component:{0,1,-1,101,INT_MIN,INT_MAX})for(int aliases:{0,1,2,3})for(int mutationMode:{0,10,11,14,15,16,20}) {
        f.reset();SIZE page{0,19},line{23,0};mutation=mutationMode;
        const SIZE* pagePtr=aliases&1?&self->m_totalDev:&page;const SIZE* linePtr=aliases&2?&self->m_pageDev:&line;
        api.sizes(self,mode,SIZE{component,component},pagePtr,linePtr);f.save(out,0);
    }
    for(bool failed:{false,true})for(int at:{0,80,84,85}) {f.reset();acquireFails=failed;failure=at;SIZE zero{};int caught=invoke([&]{api.sizes(self,MM_LOMETRIC,SIZE{101,103},&zero,&zero);});f.save(out,0,caught);}
    // Exercise the conversion-to-layout handoff with the real UpdateBars body.
    for(int mutationMode:{0,30,31,36,38}){f.reset();mutation=mutationMode;self->m_bInsideUpdate=FALSE;hasParent=true;parentReply=0;SIZE zero{};api.sizes(self,MM_LOMETRIC,SIZE{101,103},&zero,&zero);f.save(out,0);}
    return out;
}
static std::vector<Event> positionCases(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int mode:{-1,MM_TEXT,MM_LOMETRIC,MM_ANISOTROPIC})for(BOOL center:{FALSE,BOOL(69),BOOL(-1)})for(int component:{-17,0,100,INT_MIN,INT_MAX})for(int at:{0,1,12,31,80,84,86})for(bool alias:{false,true}) {
        f.reset();self->m_nMapMode=mode;self->m_bCenter=center;self->m_totalDev={component,component};failure=at;CPoint point(123,127);CPoint* result=nullptr;
        CPoint* target=alias?reinterpret_cast<CPoint*>(&self->m_totalDev):&point;
        int caught=invoke([&]{result=api.position(self,target);});f.save(out,result==target,caught);out.push_back({98,target->x,target->y,0,0,0,0,0});
    }
    for(int mode:{MM_TEXT,MM_LOMETRIC})for(int x:{INT_MIN,-1,0,7,INT_MAX})for(int y:{INT_MIN,-1,0,11,INT_MAX})for(int maximum:{-7,0,199})for(int at:{0,1,13,14,15,17,80,84,85}) {
        f.reset();self->m_nMapMode=mode;maxima[0]=maxima[1]=maximum;failure=at;int caught=invoke([&]{api.scroll(self,POINT{x,y});});f.save(out,0,caught);
    }
    return out;
}
static std::vector<Event> centering(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(DWORD bits:{DWORD(0),DWORD(WS_HSCROLL),DWORD(WS_VSCROLL),DWORD(WS_HSCROLL|WS_VSCROLL)})for(int component:{INT_MIN,-1,0,100,INT_MAX})for(int maximum:{-7,0,199})for(int mode:{0,1,2}) {
        f.reset();styleBits=bits;maxima[0]=maxima[1]=maximum;mutation=mode;api.center(self,CPoint(component,component));f.save(out,0);
    }
    for(int at:{1,10,13,14,15,31}) {f.reset();failure=at;int caught=invoke([&]{api.center(self,CPoint(101,103));});f.save(out,0,caught);}return out;
}
static std::vector<Event> painting(const API& api) {
    Fixture f(api);std::vector<Event> out;alignas(8) BYTE brushBytes[16]{};auto* brush=reinterpret_cast<CBrush*>(brushBytes);
    CDC dc;drawingDC=&dc;
    for(int width:{INT_MIN,-1,0,31,100,101,INT_MAX})for(int height:{-1,0,37,103,INT_MAX})for(bool noBrush:{false,true})for(int mode:{0,17})for(int at:{0,31,87,88}) {
        f.reset();dc.m_hDC=borrowed;dc.m_hAttribDC=attribute;brush->m_hObject=reinterpret_cast<HGDIOBJ>(UINT_PTR(77));drawingDC=&dc;drawingBrush=noBrush?nullptr:brush;mutation=mode;failure=at;self->m_totalDev={width,height};
        int caught=invoke([&]{api.fill(self,&dc,drawingBrush);});f.save(out,0,caught);
    }
    dc.m_hDC=dc.m_hAttribDC=nullptr;return out;
}
static void expectations(const API& api) {
    Fixture f(api);SIZE zero{};api.sizes(self,MM_TEXT,SIZE{100,100},&zero,&zero);
    check(self->m_totalLog.cx==100 && self->m_totalDev.cx==307 && self->m_totalDev.cy==189,"logical scroll sizes convert through the attribute DC before storing device extents");
    check(self->m_pageDev.cx==7 && self->m_pageDev.cy==11 && self->m_lineDev.cx==7 && self->m_lineDev.cy==11,"each page and line axis is converted independently before applying zero defaults");
    f.reset();self->m_nMapMode=-1;CPoint position(123,127);check(api.position(self,&position)==&position && position.x==0 && position.y==0 && events.empty(),"scale-to-fit scroll positions return zero without acquiring a DC");
    f.reset();self->m_nMapMode=MM_LOMETRIC;failure=86;position=CPoint(123,127);int caught=invoke([&]{api.position(self,&position);});
    check(caught==91 && position.x==123 && position.y==127 && events.back()[0]==86,"foreign conversion exceptions preserve the caller's output and the native skipped cleanup path");
    f.reset();api.center(self,CPoint(101,103));check(std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==17 || e[0]==46;}),"centering updates scrollbar positions without scrolling or invalidating the window");
}
static void realWindows(const API& api) {
    Fixture f(api);capture=false;splitter_command_test::capture=false;self->m_hWnd=splitter_command_test::handles[3];self->m_bInsideUpdate=FALSE;
    SIZE zero{};api.sizes(self,MM_LOMETRIC,SIZE{2540,2540},&zero,&zero);
    HDC screen=realWindowDC(nullptr);SetMapMode(screen,MM_LOMETRIC);POINT expected{2540,2540};LPtoDP(screen,&expected,1);previousRelease(nullptr,screen);
    check(self->m_totalDev.cx==expected.x && self->m_totalDev.cy==-expected.y && self->m_totalLog.cx==2540 && self->m_pageDev.cx==expected.x/10,
        "real logical scroll sizing follows Win32 mapping modes and installs converted device extents");
    api.scroll(self,POINT{254,254});CPoint logical;api.position(self,&logical);CPoint device;api.sizing.input.device(self,&device);
    screen=realWindowDC(nullptr);SetMapMode(screen,MM_LOMETRIC);POINT wanted{device.x,device.y};DPtoLP(screen,&wanted,1);previousRelease(nullptr,screen);
    check(logical.x==wanted.x && logical.y==wanted.y,"real logical scroll-position reads agree with Win32 inverse coordinate conversion");
    HBITMAP bitmap=CreateBitmap(128,128,1,32,nullptr);auto oldBitmap=SelectObject(borrowed,bitmap);HBRUSH red=CreateSolidBrush(RGB(199,31,71));alignas(8) BYTE brushStorage[16]{};auto* brush=reinterpret_cast<CBrush*>(brushStorage);brush->m_hObject=red;CDC dc;dc.m_hDC=dc.m_hAttribDC=borrowed;
    self->m_totalDev={31,37};api.fill(self,&dc,brush);
    check(GetPixel(borrowed,100,10)==RGB(199,31,71) && GetPixel(borrowed,10,60)==RGB(199,31,71),"outside-content painting fills real pixels to the right and below device extents");
    dc.m_hDC=dc.m_hAttribDC=nullptr;brush->m_hObject=nullptr;SelectObject(borrowed,oldBitmap);DeleteObject(bitmap);DeleteObject(red);
}
static int WINAPI throwingRelease(HWND,HDC) {throw 91;}
static void termination(bool native,bool client) {
    wchar_t executable[2048];GetModuleFileNameW(nullptr,executable,2048);std::wstring command=L"\""+std::wstring(executable)+L"\"";
    wchar_t mode[2]={wchar_t(L'0'+(native?2:0)+(client?1:0)),0};SetEnvironmentVariableW(L"OPENMFC_BORROWED_DC_CHILD",mode);
    STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION process{};
    BOOL created=CreateProcessW(executable,&command[0],nullptr,nullptr,FALSE,0,nullptr,nullptr,&startup,&process);SetEnvironmentVariableW(L"OPENMFC_BORROWED_DC_CHILD",nullptr);
    check(created!=FALSE,"start borrowed DC destructor termination child");if(!created)return;
    DWORD waited=WaitForSingleObject(process.hProcess,15000);if(waited!=WAIT_OBJECT_0)TerminateProcess(process.hProcess,99);DWORD code=0;GetExitCodeProcess(process.hProcess,&code);CloseHandle(process.hThread);CloseHandle(process.hProcess);
    check(waited==WAIT_OBJECT_0 && code==3,"borrowed DC destruction preserves the native no-throw boundary");
}
}
static bool borrowedDCTerminationChild() {
    wchar_t mode[8];if(!GetEnvironmentVariableW(L"OPENMFC_BORROWED_DC_CHILD",mode,8))return false;
    SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);std::set_terminate([]{ExitProcess(3);});int kind=_wtoi(mode);bool client=kind&1;
    HMODULE module=GetModuleHandleW(nullptr);alignas(8) BYTE bytes[40];
    if(kind>=2) {
        wchar_t path[2048];if(!GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_COORDINATES_DLL",path,2048))ExitProcess(95);module=LoadLibraryW(path);if(!module)ExitProcess(94);
        auto* b=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
        if(pe->FileHeader.TimeDateStamp!=0x442cba30 || pe->OptionalHeader.SizeOfImage!=0x55c000)ExitProcess(93);
        using Construct=void* (MS_ABI*)(void*,CWnd*);reinterpret_cast<Construct>(b+(client?0x2a3b20:0x2a3c20))(bytes,nullptr);
    } else {if(client)impl___0CClientDC__QEAA_PEAVCWnd___Z(reinterpret_cast<CClientDC*>(bytes),nullptr);else impl___0CWindowDC__QEAA_PEAVCWnd___Z(reinterpret_cast<CWindowDC*>(bytes),nullptr);}
    auto** slot=recordset_value_test::import(module,"ReleaseDC");if(!slot)ExitProcess(92);recordset_value_test::Patch patch(slot,reinterpret_cast<void*>(&scroll_view_coordinate_test::throwingRelease));
    try {
        if(kind>=2){using Destroy=void (MS_ABI*)(void*);reinterpret_cast<Destroy>(reinterpret_cast<char*>(module)+(client?0x2a3be0:0x2a3ce0))(bytes);}
        else if(client)impl___1CClientDC__UEAA_XZ(reinterpret_cast<CClientDC*>(bytes));else impl___1CWindowDC__UEAA_XZ(reinterpret_cast<CWindowDC*>(bytes));
    }catch(...){ExitProcess(97);}ExitProcess(98);
}
static void scrollViewCoordinates() {
    using namespace scroll_view_coordinate_test;
    scroll_view_input_test::API input{};input.cacheInitialized=&openmfc::detail::scrollViewWheelSettingsInitialized;input.cacheLines=&openmfc::detail::scrollViewWheelLines;input.state=&AfxGetThreadState;
    input.device=&impl__GetDeviceScrollPosition_CScrollView__QEBA_AVCPoint__XZ;
    scroll_view_sizing_test::API sizing{input};sizing.data=&openmfc::detail::GetAuxData();sizing.updateBars=&impl__UpdateBars_CScrollView__IEAAXXZ;
    scroll_view_coordinate_test::API local{sizing,&impl___0CWindowDC__QEAA_PEAVCWnd___Z,&impl___1CWindowDC__UEAA_XZ,&impl___0CClientDC__QEAA_PEAVCWnd___Z,&impl___1CClientDC__UEAA_XZ,
        &impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z,&impl__GetScrollPosition_CScrollView__QEBA_AVCPoint__XZ,&impl__ScrollToPosition_CScrollView__QEAAXUtagPOINT___Z,
        &impl__CenterOnPoint_CScrollView__IEAAXVCPoint___Z,&impl__FillOutsideRect_CScrollView__QEAAXPEAVCDC__PEAVCBrush___Z,&impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z,&impl__DeleteTempMap_CDC__SAXXZ,&impl__Attach_CDC__QEAAHPEAUHDC_____Z,&impl__Detach_CDC__QEAAPEAUHDC____XZ};
    scroll_view_input_test::Imports inputImports(GetModuleHandleW(nullptr),true);scroll_view_sizing_test::Imports sizingImports(GetModuleHandleW(nullptr),true);scroll_view_coordinate_test::Imports imports(GetModuleHandleW(nullptr),true);
    expectations(local);auto dc=lifetimes(local),size=sizes(local),position=positionCases(local),center=centering(local),paint=painting(local);realWindows(local);scroll_view_coordinate_test::termination(false,false);scroll_view_coordinate_test::termination(false,true);
    check(impl__GetThisClass_CClientDC__SAPEAUCRuntimeClass__XZ()==&CClientDC::classCClientDC && impl__GetRuntimeClass_CClientDC__UEBAPEAUCRuntimeClass__XZ(nullptr)==&CClientDC::classCClientDC && impl__GetThisClass_CWindowDC__SAPEAUCRuntimeClass__XZ()==&CWindowDC::classCWindowDC,
        "borrowed DC runtime accessors return their canonical descriptors");
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_COORDINATES_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for scroll coordinate comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"scroll coordinate oracle uses the verified MFC image build");
            if(pinned) {
                scroll_view_input_test::Imports nativeInputImports(native,false);scroll_view_sizing_test::Imports nativeSizingImports(native,false);scroll_view_coordinate_test::Imports nativeImports(native,false);scroll_view_coordinate_test::API api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(window,0x2a3c20);RVA(destroyWindow,0x2a3ce0);RVA(client,0x2a3b20);RVA(destroyClient,0x2a3be0);RVA(sizes,0x288180);RVA(position,0x288330);RVA(scroll,0x288440);RVA(center,0x288e70);RVA(fill,0x288660);RVA(lookup,0x2a2450);RVA(clear,0x2a6250);RVA(attach,0x2a2480);RVA(detach,0x2a24d0);
#undef RVA
                api.sizing.data=reinterpret_cast<AuxData*>(b+0x3c32c0);api.sizing.input.device=reinterpret_cast<decltype(input.device)>(b+0x288510);api.sizing.updateBars=reinterpret_cast<decltype(sizing.updateBars)>(b+0x2891b0);
                api.sizing.input.graph.construct=reinterpret_cast<decltype(input.graph.construct)>(b+0x28a700);api.sizing.input.graph.attach=reinterpret_cast<decltype(input.graph.attach)>(b+0x28adf0);api.sizing.input.graph.detach=reinterpret_cast<decltype(input.graph.detach)>(b+0x28ae50);api.sizing.input.graph.destroy=reinterpret_cast<decltype(input.graph.destroy)>(b+0x28b740);api.sizing.input.graph.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);
                scroll_view_sizing_test::nativeLookupReturn=b+0x2a9da9;expectations(api);
                scroll_view_input_test::compare(lifetimes(api),dc,"client/window DC construction, HWND capture, permanent mapping and detach-before-release match native MFC");
                scroll_view_input_test::compare(sizes(api),size,"logical size conversion, aliased inputs, zero defaults and layout handoff match native MFC");
                scroll_view_input_test::compare(positionCases(api),position,"logical scroll positions, centering offsets, signed limits and exception cleanup match native MFC");
                scroll_view_input_test::compare(centering(api),center,"point centering preserves native style snapshots and scrollbar update ordering");
                scroll_view_input_test::compare(painting(api),paint,"outside-content rectangles, borrowed CDC/brush handles and callback mutations match native MFC");
                realWindows(api);scroll_view_coordinate_test::termination(true,false);scroll_view_coordinate_test::termination(true,true);scroll_view_sizing_test::nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
