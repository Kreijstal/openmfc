#pragma once
extern "C" {
void MS_ABI impl__ResizeParentToFit_CScrollView__QEAAXH_Z(CScrollView*,BOOL);
int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd*,const CWnd*,int,int,int,int,UINT);
}
namespace scroll_view_parent_test {
using namespace scroll_view_sizing_test;
struct API {
    scroll_view_sizing_test::API sizing;
    decltype(&impl__ResizeParentToFit_CScrollView__QEAAXH_Z) resize;
    decltype(&impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z) position;
    decltype(&impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z) calculate;
};
static const API* active;
static int frameIndex,rectangleCalls,offsetCalls;
static BOOL positioningResult;
static bool partialRectangle,delegateCalculation;
static RECT viewRectangle,frameRectangle;
static RECT* observedClient;
static void* siteTable[42];
struct Site {void** table;int index;};
static Site site{siteTable,7};
static CWnd* positioned;
static decltype(&::GetParent) realParent;
static decltype(&::GetWindowRect) realRectangle;
static decltype(&::OffsetRect) realOffset;
static decltype(&::SetWindowPos) realPosition;
static INT64 windowID(HWND h) {return splitter_command_test::id(h);}
static INT64 insertionID(HWND h) {
    if(h==HWND_TOPMOST)return -12;if(h==HWND_NOTOPMOST)return -13;if(h==HWND_BOTTOM)return -14;return windowID(h);
}
static INT64 afterID(const CWnd* after) {
    if(!after)return -1;if(after==reinterpret_cast<CWnd*>(UINT_PTR(1)))return -11;
    if(after==self)return 10;return insertionID(after->m_hWnd);
}
static HWND WINAPI parent(HWND h) {
    if(_ReturnAddress()==scroll_view_sizing_test::nativeLookupReturn)return nullptr;
    if(!capture)return realParent(h);event(163,windowID(h));return realParent(h);
}
static BOOL MS_ABI isFrame(const CWnd* window) {
    int index=int(windowID(window->m_hWnd));event(164,index);
    if(mutation==5 && index==frameIndex)const_cast<CWnd*>(window)->m_hWnd=splitter_command_test::handles[0];
    if(mutation==11 && index==frameIndex)const_cast<CWnd*>(window)->m_hWnd=nullptr;
    return index==frameIndex?69:FALSE;
}
static BOOL WINAPI rectangle(HWND h,RECT* r) {
    if(!capture)return realRectangle(h,r);
    event(165,windowID(h),r->left,r->top,r->right,r->bottom,rectangleCalls);++rectangleCalls;
    const RECT& value=rectangleCalls==1?viewRectangle:frameRectangle;
    if(partialRectangle)r->right=value.right;else *r=value;
    if(mutation==1 && rectangleCalls==1)self->m_totalDev={71,83};
    if(mutation==6 && rectangleCalls==2 && frameIndex>=0)splitter_command_test::windows[frameIndex]->m_hWnd=nullptr;
    return FALSE;
}
static int Add(int a,int b) {return int(UINT(a)+UINT(b));}
static void MS_ABI alternateCalculation(CWnd*,RECT* r,UINT adjust) {
    event(168,adjust,r->left,r->top,r->right,r->bottom);
    r->left=Add(r->left,-29);r->top=Add(r->top,-31);r->right=Add(r->right,37);r->bottom=Add(r->bottom,41);
}
static void MS_ABI calculate(CWnd*,RECT* r,UINT adjust) {
    event(166,adjust,r->left,r->top,r->right,r->bottom,self->m_totalDev.cx,self->m_totalDev.cy);
    if(delegateCalculation) {active->calculate(self,r,adjust);return;}
    r->left=Add(r->left,adjust?-13:-3);r->top=Add(r->top,adjust?-17:-5);
    r->right=Add(r->right,adjust?19:7);r->bottom=Add(r->bottom,adjust?23:11);
    if(mutation==2 && !adjust)self->m_totalDev={47,53};
    if(mutation==3 && adjust)self->m_totalDev={INT_MAX,INT_MIN};
    if(mutation==4 && adjust)self->m_hWnd=nullptr;
    if(mutation==9 && adjust && observedClient){observedClient->left=13;observedClient->right=59;observedClient->bottom=61;}
    if(mutation==10 && !adjust)(*reinterpret_cast<void***>(self))[28]=reinterpret_cast<void*>(&alternateCalculation);
}
static BOOL WINAPI offset(RECT* r,int x,int y) {
    if(!capture)return realOffset(r,x,y);
    event(167,r->left,r->top,r->right,r->bottom,x,y,offsetCalls);++offsetCalls;
    if(offsetCalls==1)observedClient=r;
    realOffset(r,x,y);
    if(mutation==7 && offsetCalls==1)self->m_totalDev={79,89};
    if(mutation==8 && offsetCalls==2){self->m_totalDev={INT_MIN,INT_MAX};r->right=Add(r->right,13);}
    return FALSE;
}
static BOOL WINAPI position(HWND h,HWND after,int x,int y,int width,int height,UINT flags) {
    if(!capture)return realPosition(h,after,x,y,width,height,flags);
    event(160,windowID(h),insertionID(after),x,y,width,height,flags);
    if(mutation==12 && positioned)positioned->m_hWnd=nullptr;
    return positioningResult;
}
static BOOL MS_ABI sitePosition(void* object,const CWnd* after,int x,int y,int width,int height,UINT flags) {
    event(161,static_cast<Site*>(object)->index,afterID(after),x,y,width,height,flags);
    if(mutation==12 && positioned){positioned->m_hWnd=nullptr;positioned->m_pCtrlSite=nullptr;}
    return positioningResult;
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate parent sizing import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetParent",parent,realParent);HOOK("GetWindowRect",rectangle,realRectangle);HOOK("OffsetRect",offset,realOffset);HOOK("SetWindowPos",position,realPosition);
        if(originals)realParent=scroll_view_sizing_test::realParent;
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    scroll_view_sizing_test::Fixture f;const API& api;
    Fixture(const API& functions):f(functions.sizing),api(functions) {reset();}
    void reset() {
        f.reset();active=&api;self->m_hWnd=splitter_command_test::handles[3];f.f.table[28]=reinterpret_cast<void*>(&calculate);
        for(int i=0;i<5;++i){splitter_command_test::windows[i]->m_hWnd=splitter_command_test::handles[i];splitter_command_test::windows[i]->m_pCtrlSite=nullptr;f.graph->table[i][86]=reinterpret_cast<void*>(&isFrame);}
        frameIndex=1;rectangleCalls=offsetCalls=0;positioningResult=-7;partialRectangle=delegateCalculation=false;observedClient=nullptr;positioned=nullptr;
        viewRectangle={17,19,121,117};frameRectangle={-37,41,503,439};siteTable[41]=reinterpret_cast<void*>(&sitePosition);capture=true;
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        f.save(out,result,caught);out.push_back({169,windowID(self->m_hWnd),rectangleCalls,offsetCalls,0,0,0,0});
    }
};
template<class F>static int invoke(F action) {try{action();return 0;}catch(int n){return n==19?92:n;}catch(CException* e){e->Delete();return 92;}}
static std::vector<Event> positioning(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(bool ole:{false,true})for(bool handle:{false,true})for(int after:{0,1,2,3,4,5,6,7})for(int value:{INT_MIN,-17,0,INT_MAX})for(int result:{INT_MIN,0,69,-7})for(int mode:{0,12})for(int at:{0,160,161}) {
        f.reset();self->m_hWnd=handle?splitter_command_test::handles[3]:nullptr;self->m_pCtrlSite=ole?reinterpret_cast<COleControlSite*>(&site):nullptr;positioned=self;positioningResult=result;mutation=mode;failure=at;
        if(after>=4)splitter_command_test::windows[2]->m_hWnd=after==4?HWND_TOP:after==5?HWND_BOTTOM:after==6?HWND_TOPMOST:HWND_NOTOPMOST;
        const CWnd* insert=after==0?nullptr:after==1?self:after==2?splitter_command_test::windows[0]:after==3 && ole?reinterpret_cast<CWnd*>(UINT_PTR(1)):splitter_command_test::windows[2];
        int returned=17,caught=invoke([&]{returned=api.position(self,insert,value,Add(value,1),Add(value,-1),value,UINT_MAX);});f.save(out,returned,caught);
    }
    return out;
}
static std::vector<Event> resizing(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(BOOL shrink:{FALSE,BOOL(69),BOOL(-1)})for(int width:{INT_MIN,-17,0,97,140,INT_MAX})for(int height:{INT_MIN,-17,0,98,160,INT_MAX})for(int frame:{-1,0,1})for(bool partial:{false,true}) {
        f.reset();frameIndex=frame;partialRectangle=partial;self->m_totalLog={INT_MAX,INT_MIN};self->m_totalDev={width,height};
        int caught=invoke([&]{api.resize(self,shrink);});f.save(out,0,caught);
    }
    for(BOOL shrink:{FALSE,BOOL(69),BOOL(-1)})for(bool ole:{false,true})for(int mode=0;mode<=12;++mode)for(int at:{0,160,161,163,164,165,166,167,168}) {
        f.reset();if(ole)splitter_command_test::windows[1]->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);self->m_totalDev={101,103};mutation=mode;failure=at;
        int caught=invoke([&]{api.resize(self,shrink);});f.save(out,0,caught);
    }
    for(bool empty:{false,true})for(int coordinate:{INT_MIN,INT_MAX}) {
        f.reset();if(empty)self->m_hWnd=nullptr;viewRectangle={coordinate,coordinate,Add(coordinate,101),Add(coordinate,103)};frameRectangle={INT_MIN,INT_MAX,INT_MAX,INT_MIN};
        int caught=invoke([&]{api.resize(self,69);});f.save(out,0,caught);
    }
    return out;
}
static void expectations(const API& api) {
    Fixture f(api);self->m_totalDev={37,41};self->m_totalLog={100000,200000};api.resize(self,FALSE);
    check(events.size()>=8 && events[1][0]==166 && events[1][1]==0 && events[3][0]==166 && events[3][1]==1 && events.back()[0]==160 && events.back()[5]==515 && events.back()[6]==397 && events.back()[7]==0x16,
        "parent fitting uses device extents, both rectangle virtuals and the containing frame's nonclient size");
    f.reset();self->m_totalDev={1000,1000};api.resize(self,69);check(events.back()[5]==540 && events.back()[6]==398,"shrink-only fitting prevents each oversized content axis from enlarging the frame");
    f.reset();frameIndex=-1;check(invoke([&]{api.resize(self,FALSE);})==92 && rectangleCalls==1 && offsetCalls==2,"missing parent frames throw after both virtual rectangle calculations");
    f.reset();self->m_hWnd=nullptr;self->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);positioningResult=69;
    check(api.position(self,reinterpret_cast<CWnd*>(UINT_PTR(1)),INT_MIN,INT_MAX,-1,0,UINT_MAX)==69 && events.size()==1 && events[0][0]==161 && events[0][2]==-11,
        "windowless positioning forwards the untouched insertion object to the native OLE slot and preserves BOOL values");
}
static void realWindows(const API& api) {
    Fixture f(api);capture=false;splitter_command_test::capture=false;delegateCalculation=true;self->m_totalDev={37,41};self->m_nMapMode=MM_TEXT;
    RECT before{},after{};realRectangle(splitter_command_test::handles[1],&before);
    api.resize(self,FALSE);realRectangle(splitter_command_test::handles[1],&after);
    check(after.left==before.left && after.top==before.top && after.right-after.left==37 && after.bottom-after.top==41,
        "real parent fitting resizes the frame to the device content and retains its position");
    self->m_totalDev={1000,1000};api.resize(self,69);RECT capped{};realRectangle(splitter_command_test::handles[1],&capped);
    check(capped.right-capped.left==after.right-after.left && capped.bottom-capped.top==after.bottom-after.top,"real shrink-only fitting leaves an already smaller frame at its size");
}
static int MS_ABI memberPosition(CWnd* self,const CWnd* after,int x,int y,int width,int height,UINT flags) {return self->SetWindowPos(after,x,y,width,height,flags);}
}
static void scrollViewParentSizing() {
    using namespace scroll_view_parent_test;
    scroll_view_input_test::API input{};input.cacheInitialized=&openmfc::detail::scrollViewWheelSettingsInitialized;input.cacheLines=&openmfc::detail::scrollViewWheelLines;input.state=&AfxGetThreadState;
    scroll_view_sizing_test::API sizing{input};sizing.data=&openmfc::detail::GetAuxData();sizing.updateBars=&impl__UpdateBars_CScrollView__IEAAXXZ;
    scroll_view_parent_test::API local{sizing,&impl__ResizeParentToFit_CScrollView__QEAAXH_Z,&impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z,&impl__CalcWindowRect_CScrollView__UEAAXPEAUtagRECT__I_Z};
    scroll_view_input_test::Imports inputImports(GetModuleHandleW(nullptr),true);scroll_view_sizing_test::Imports sizingImports(GetModuleHandleW(nullptr),true);scroll_view_parent_test::Imports imports(GetModuleHandleW(nullptr),true);
    expectations(local);auto positions=positioning(local),sizes=resizing(local);realWindows(local);
    auto memberAPI=local;memberAPI.position=&memberPosition;scroll_view_input_test::compare(positioning(memberAPI),positions,"CWnd member positioning and its export share OLE dispatch, raw results and failure behavior");
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCROLL_PARENT_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for parent-frame sizing comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"parent sizing oracle uses the verified MFC image build");
            if(pinned) {
                scroll_view_input_test::Imports nativeInput(native,false);scroll_view_sizing_test::Imports nativeSizing(native,false);scroll_view_parent_test::Imports nativeImports(native,false);scroll_view_parent_test::API api=local;
                api.resize=reinterpret_cast<decltype(api.resize)>(b+0x288730);api.position=reinterpret_cast<decltype(api.position)>(b+0x2a9a60);api.calculate=reinterpret_cast<decltype(api.calculate)>(b+0x2893f0);
                api.sizing.data=reinterpret_cast<AuxData*>(b+0x3c32c0);api.sizing.input.graph.construct=reinterpret_cast<decltype(input.graph.construct)>(b+0x28a700);api.sizing.input.graph.attach=reinterpret_cast<decltype(input.graph.attach)>(b+0x28adf0);api.sizing.input.graph.detach=reinterpret_cast<decltype(input.graph.detach)>(b+0x28ae50);api.sizing.input.graph.destroy=reinterpret_cast<decltype(input.graph.destroy)>(b+0x28b740);api.sizing.input.graph.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);
                scroll_view_sizing_test::nativeLookupReturn=b+0x2a9da9;expectations(api);
                scroll_view_input_test::compare(positioning(api),positions,"HWND and OLE positioning arguments, insertion objects, BOOL results and exceptions match native MFC");
                scroll_view_input_test::compare(resizing(api),sizes,"parent fitting geometry, shrink limits, callback mutations, overflows and failure paths match native MFC");
                realWindows(api);scroll_view_sizing_test::nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
