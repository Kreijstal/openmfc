#pragma once
#include <intrin.h>
extern "C" {
void MS_ABI impl__GetRowInfo_CSplitterWnd__QEBAXHAEAH0_Z(const CSplitterWnd*,int,int*,int*);
void MS_ABI impl__GetColumnInfo_CSplitterWnd__QEBAXHAEAH0_Z(const CSplitterWnd*,int,int*,int*);
void MS_ABI impl__SetRowInfo_CSplitterWnd__QEAAXHHH_Z(CSplitterWnd*,int,int,int);
void MS_ABI impl__SetColumnInfo_CSplitterWnd__QEAAXHHH_Z(CSplitterWnd*,int,int,int);
BOOL MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject*,const CRuntimeClass*);
BOOL MS_ABI impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z(const CRuntimeClass*,const CRuntimeClass*);
CSplitterWnd* MS_ABI impl__GetParentSplitter_CView__SAPEAVCSplitterWnd__PEBVCWnd__H_Z(const CWnd*,BOOL);
BOOL MS_ABI impl__OnSplitCmd_CView__IEAAHI_Z(CView*,UINT);
BOOL MS_ABI impl__OnNextPaneCmd_CView__IEAAHI_Z(CView*,UINT);
void MS_ABI impl__OnUpdateSplitCmd_CView__IEAAXPEAVCCmdUI___Z(CView*,CCmdUI*);
void MS_ABI impl__OnUpdateNextPaneMenu_CView__IEAAXPEAVCCmdUI___Z(CView*,CCmdUI*);
BOOL MS_ABI impl__CanActivateNext_CSplitterWnd__UEAAHH_Z(CSplitterWnd*,BOOL);
void MS_ABI impl__ActivateNext_CSplitterWnd__UEAAXH_Z(CSplitterWnd*,BOOL);
BOOL MS_ABI impl__DoKeyboardSplit_CSplitterWnd__UEAAHXZ(CSplitterWnd*);
void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd*,RECT*);
DWORD MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd*);
CRuntimeClass* MS_ABI impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ();
CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitterWnd__UEBAPEAUCRuntimeClass__XZ(const CSplitterWnd*);
}
namespace splitter_command_test {
using Event=std::array<INT64,6>;
struct API {
    decltype(&impl__GetParentSplitter_CView__SAPEAVCSplitterWnd__PEBVCWnd__H_Z) parent;
    decltype(&impl__OnSplitCmd_CView__IEAAHI_Z) split;
    decltype(&impl__OnNextPaneCmd_CView__IEAAHI_Z) next;
    decltype(&impl__OnUpdateSplitCmd_CView__IEAAXPEAVCCmdUI___Z) updateSplit;
    decltype(&impl__OnUpdateNextPaneMenu_CView__IEAAXPEAVCCmdUI___Z) updateNext;
    decltype(&impl__CanActivateNext_CSplitterWnd__UEAAHH_Z) can;
    decltype(&impl__ActivateNext_CSplitterWnd__UEAAXH_Z) activate;
    decltype(&impl__DoKeyboardSplit_CSplitterWnd__UEAAHXZ) keyboard;
    decltype(&impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z) coordinates;
    decltype(&impl__GetExStyle_CWnd__QEBAKXZ) style;
    decltype(&impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z) kindOf;
    decltype(&impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z) derives;
    decltype(&impl__GetRowInfo_CSplitterWnd__QEBAXHAEAH0_Z) getRow;
    decltype(&impl__GetColumnInfo_CSplitterWnd__QEBAXHAEAH0_Z) getColumn;
    decltype(&impl__SetRowInfo_CSplitterWnd__QEAAXHHH_Z) setRow;
    decltype(&impl__SetColumnInfo_CSplitterWnd__QEAAXHHH_Z) setColumn;
    void (MS_ABI* construct)(CWnd*)=nullptr;
    BOOL (MS_ABI* attach)(CWnd*,HWND)=nullptr;
    HWND (MS_ABI* detach)(CWnd*)=nullptr;
    void (MS_ABI* destroy)(CWnd*)=nullptr;
    CRuntimeClass* type=nullptr;
};
static std::vector<Event> events;
static HWND handles[5];static CWnd* windows[5];
static CRuntimeClass* root;
static int kind,failure,mutation,row,column,coordinateCalls,minimized;
static BOOL callbackResult;
static DWORD extendedStyle;
static bool capture,hasActive;
static void* nativeLookupReturn;
static CCmdUI* activeUI;
static void* alternateUI[4];
static INT64 id(HWND h) {if(!h)return -1;for(int i=0;i<5;++i)if(handles[i]==h)return i;return -2;}
static INT64 id(const CWnd* w) {return w?id(w->m_hWnd):-1;}
static void event(INT64 n,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0) {events.push_back({n,a,b,c,d,e});if(failure==n)throw 91;}
static CWnd* lookup(HWND h) {auto n=id(h);return n>=0?windows[n]:nullptr;}
static CRuntimeClass* AFXAPI base() {return root;}
static CRuntimeClass derived{"DerivedSplitter",384,0xffff,nullptr,&base,nullptr,nullptr};
static CRuntimeClass collision{"CSplitterWnd",384,0xffff,nullptr,nullptr,nullptr,nullptr};
static CRuntimeClass generic{"CWnd",232,0xffff,nullptr,nullptr,nullptr,nullptr};
static CRuntimeClass* MS_ABI runtime(const CWnd* w) {
    int n=static_cast<int>(id(w));event(1,n);
    if(mutation==1 && n==1)SetParent(handles[3],handles[0]);
    return n==1?(kind==0?root:kind==1?&derived:kind==3?nullptr:&collision):&generic;
}
static BOOL MS_ABI split(CSplitterWnd*) {event(3);return callbackResult;}
static void MS_ABI activate(CSplitterWnd*,BOOL previous) {event(4,previous);}
static BOOL MS_ABI can(CSplitterWnd*,BOOL previous) {
    event(5,previous);if(mutation==2)*reinterpret_cast<void***>(activeUI)=alternateUI;return callbackResult;
}
static void MS_ABI enable(CCmdUI* ui,BOOL on) {event(6,on,ui==activeUI);}
static void MS_ABI changedEnable(CCmdUI*,BOOL on) {event(7,on);}
static CWnd* MS_ABI active(CSplitterWnd* self,int* r,int* c) {
    event(9,r!=nullptr,c!=nullptr);if(r)*r=row;if(c)*c=column;
    if(mutation==3){self->m_nRows=2;self->m_nCols=3;}
    return hasActive?windows[3]:nullptr;
}
static void MS_ABI setActive(CSplitterWnd*,int r,int c,CWnd* w) {event(10,r,c,w==nullptr);}
static void MS_ABI start(CSplitterWnd* self,int hit) {
    event(11,hit);
    if(mutation==4) {self->m_rectTracker=CRect(11,17,-20,62);self->m_ptTrackOffset=CPoint(-3,7);self->m_bTracking2=69;self->m_rectTracker2.left=91;}
}
static DWORD MS_ABI siteStyle(void*) {event(14);return extendedStyle;}
static decltype(&::GetParent) realParent;
static decltype(&::IsIconic) realIconic;
static decltype(&::ClientToScreen) realCoordinates;
static decltype(&::SetCursorPos) realCursor;
static decltype(&::GetWindowLongW) realStyle;
static HWND WINAPI parent(HWND h) {
    if(_ReturnAddress()==nativeLookupReturn)return nullptr;
    if(capture)event(8,id(h));return realParent(h);
}
static BOOL WINAPI iconic(HWND h) {if(!capture)return realIconic(h);event(2,id(h));return id(h)==minimized?69:FALSE;}
static BOOL WINAPI coordinates(HWND h,POINT* p) {
    if(!capture)return realCoordinates(h,p);event(12,id(h),p->x,p->y);
    p->x=LONG(ULONG(p->x)+100u);p->y=LONG(ULONG(p->y)+200u);
    if(mutation==5 && ++coordinateCalls==1)windows[1]->m_hWnd=handles[0];return FALSE;
}
static BOOL WINAPI cursor(int x,int y) {if(!capture)return realCursor(x,y);event(15,x,y);return FALSE;}
static LONG WINAPI style(HWND h,int which) {if(!capture || which!=GWL_EXSTYLE)return realStyle(h,which);event(13,id(h));return LONG(extendedStyle);}
struct Fixture {
    alignas(8) BYTE bytes[5][392]{};void* table[5][120]{};void* original[5]{};AFX_MODULE_STATE* originalModule[5]{};
    void* uiTable[4]{};CCmdUI ui;void* siteTable[34]{};void** site=siteTable;
    CWnd* (*previousLookup)(HWND);const API& api;bool boundaries=true;
    Fixture(const API& functions):api(functions) {
        capture=false;root=api.type?api.type:&CSplitterWnd::classCSplitterWnd;
        previousLookup=g_messageWindowLookup;g_messageWindowLookup=&lookup;
        for(int i=0;i<5;++i) {
            HWND p=i==0?nullptr:i==1?handles[0]:i==4?handles[2]:handles[1];
            handles[i]=CreateWindowExW(0,L"STATIC",L"split routing",i==0?WS_OVERLAPPEDWINDOW:WS_CHILD,7,9,120,80,p,nullptr,GetModuleHandleW(nullptr),nullptr);
            check(handles[i]!=nullptr,"create splitter command window graph");windows[i]=reinterpret_cast<CWnd*>(bytes[i]);
            if(api.construct){api.construct(windows[i]);original[i]=*reinterpret_cast<void**>(bytes[i]);originalModule[i]=windows[i]->m_pModuleState;check(api.attach(windows[i],handles[i])!=FALSE,"attach native splitter command wrapper");}
            windows[i]->m_hWnd=handles[i];*reinterpret_cast<void***>(bytes[i])=table[i];table[i][0]=reinterpret_cast<void*>(&runtime);
            table[i][104]=reinterpret_cast<void*>(&active);table[i][105]=reinterpret_cast<void*>(&setActive);
            table[i][106]=reinterpret_cast<void*>(&can);table[i][107]=reinterpret_cast<void*>(&activate);table[i][108]=reinterpret_cast<void*>(&split);table[i][118]=reinterpret_cast<void*>(&start);
            memset(bytes[i]+384,0xa5,8);
        }
        uiTable[0]=reinterpret_cast<void*>(&enable);alternateUI[0]=reinterpret_cast<void*>(&changedEnable);siteTable[33]=reinterpret_cast<void*>(&siteStyle);activeUI=&ui;reset();capture=true;
    }
    CSplitterWnd* splitter() {return reinterpret_cast<CSplitterWnd*>(windows[1]);}
    CView* view() {return reinterpret_cast<CView*>(windows[3]);}
    void reset() {
        events.clear();failure=mutation=coordinateCalls=kind=0;minimized=-3;callbackResult=69;hasActive=true;row=column=0;extendedStyle=0;
        *reinterpret_cast<void***>(&ui)=uiTable;ui.m_nID=0xe150;windows[1]->m_hWnd=handles[1];windows[1]->m_pCtrlSite=nullptr;
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        out.insert(out.end(),events.begin(),events.end());out.push_back({90,result,caught,0,0,0});
        for(int i=0;i<5;++i)boundaries=boundaries && *reinterpret_cast<UINT64*>(bytes[i]+384)==0xa5a5a5a5a5a5a5a5ULL;
    }
    ~Fixture() {
        capture=false;failure=0;check(boundaries,"splitter operations preserve SDK object boundaries");
        for(int i=4;i>=0;--i) {
            windows[i]->m_hWnd=handles[i];windows[i]->m_pCtrlSite=nullptr;windows[i]->m_pCtrlCont=nullptr;
            if(api.detach){api.detach(windows[i]);windows[i]->m_pModuleState=originalModule[i];*reinterpret_cast<void**>(bytes[i])=original[i];api.destroy(windows[i]);}
            DestroyWindow(handles[i]);windows[i]=nullptr;handles[i]=nullptr;
        }
        g_messageWindowLookup=previousLookup;
    }
};
template<class F> static int invoke(F action) {try{action();return 0;}catch(int n){return n==19?-1:n;}catch(CException* e){e->Delete();return -1;}}
static std::vector<Event> parents(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int node:{0,1,2,3,4,-1})for(int type:{0,1,2})for(int minimizedID:{-3,0,1,3})for(BOOL any:{FALSE,BOOL(69),BOOL(-1)}) {
        f.reset();kind=type;minimized=minimizedID;CSplitterWnd* result=nullptr;int caught=invoke([&]{result=api.parent(node<0?nullptr:windows[node],any);});f.save(out,id(result),caught);
    }
    for(int mode:{0,1})for(int at:{0,1,2,8}) {
        f.reset();mutation=mode;failure=at;CSplitterWnd* result=nullptr;int caught=invoke([&]{result=api.parent(windows[3],FALSE);});f.save(out,id(result),caught);
        capture=false;SetParent(handles[3],handles[1]);capture=true;
    }
    return out;
}
static std::vector<Event> commands(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int operation=0;operation<4;++operation)for(int type:{0,1,2})for(int minimizedID:{-3,0,1})
    for(UINT command:{0u,0xe135u,0xe150u,0xe151u,UINT(-1)})for(BOOL returned:{FALSE,BOOL(69),BOOL(-1)})for(BOOL tracking:{FALSE,BOOL(69)}) {
        f.reset();kind=type;minimized=minimizedID;callbackResult=returned;f.ui.m_nID=command;f.splitter()->m_bTracking=tracking;
        int result=-7,caught=invoke([&]{switch(operation){case 0:result=api.split(f.view(),command);break;case 1:result=api.next(f.view(),command);break;case 2:api.updateSplit(f.view(),&f.ui);break;case 3:api.updateNext(f.view(),&f.ui);break;}});f.save(out,result,caught);
    }
    for(int mode:{0,2})for(int at:{0,1,2,3,4,5,6,7,8})for(int operation=0;operation<4;++operation) {
        f.reset();mutation=mode;failure=at;int result=-7,caught=invoke([&]{switch(operation){case 0:result=api.split(f.view(),0xe135);break;case 1:result=api.next(f.view(),0xe151);break;case 2:api.updateSplit(f.view(),&f.ui);break;case 3:api.updateNext(f.view(),&f.ui);break;}});f.save(out,result,caught);
    }
    f.reset();f.save(out,0,invoke([&]{api.updateSplit(f.view(),nullptr);}));return out;
}
static std::vector<Event> navigation(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(bool available:{false,true})for(int rows:{-1,0,1,2,3})for(int cols:{-1,0,1,2,3})for(int r:{0,1,INT_MAX,INT_MIN})for(int c:{0,1,2,INT_MAX,INT_MIN})for(BOOL previous:{FALSE,BOOL(69),BOOL(-1)}) {
        for(int operation:{0,1}) {
            f.reset();hasActive=available;row=r;column=c;f.splitter()->m_nRows=rows;f.splitter()->m_nCols=cols;BOOL result=-7;
            int caught=invoke([&]{if(!operation)result=api.can(f.splitter(),previous);else api.activate(f.splitter(),previous);});f.save(out,result,caught);
        }
    }
    for(int mode:{0,3})for(int at:{0,9,10}) {
        f.reset();row=1;column=2;mutation=mode;failure=at;f.splitter()->m_nRows=f.splitter()->m_nCols=1;
        int caught=invoke([&]{api.activate(f.splitter(),FALSE);});f.save(out,0,caught);
    }
    return out;
}
static std::vector<Event> keyboard(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int rows:{0,1,2})for(int cols:{0,1,2})for(int maxRows:{1,2})for(int maxCols:{1,2})for(int mode:{0,4,5})for(bool ole:{false,true})for(bool rtl:{false,true}) {
        f.reset();mutation=mode;extendedStyle=rtl?WS_EX_LAYOUTRTL:0;
        auto* s=f.splitter();s->m_nRows=rows;s->m_nCols=cols;s->m_nMaxRows=maxRows;s->m_nMaxCols=maxCols;
        s->m_rectTracker=CRect(3,7,28,42);s->m_ptTrackOffset=CPoint(0,0);s->m_bTracking2=FALSE;s->m_rectTracker2.left=70;
        s->m_pCtrlSite=ole?reinterpret_cast<COleControlSite*>(&f.site):nullptr;BOOL result=-7;int caught=invoke([&]{result=api.keyboard(s);});f.save(out,result,caught);
    }
    for(int at:{0,11,12,13,14,15})for(bool ole:{false,true}) {
        f.reset();failure=at;mutation=4;f.splitter()->m_nRows=f.splitter()->m_nCols=2;f.splitter()->m_pCtrlSite=ole?reinterpret_cast<COleControlSite*>(&f.site):nullptr;
        BOOL result=-7;int caught=invoke([&]{result=api.keyboard(f.splitter());});f.save(out,result,caught);
    }
    return out;
}
static std::vector<Event> typeChecks(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int type:{0,1,2,3})for(bool nullObject:{false,true})for(bool nullBase:{false,true})for(int at:{0,1}) {
        f.reset();kind=type;failure=at;BOOL result=-7;int caught=invoke([&]{result=api.kindOf(nullObject?nullptr:windows[1],nullBase?nullptr:root);});f.save(out,result,caught);
    }
    for(auto* type:{root,&derived,&collision,static_cast<CRuntimeClass*>(nullptr)})for(bool nullBase:{false,true}) {
        f.reset();BOOL result=-7;int caught=invoke([&]{result=api.derives(type,nullBase?nullptr:root);});f.save(out,result,caught);
    }
    return out;
}
static std::vector<Event> dimensions(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int index:{0,1})for(int ideal:{-3,0,17,INT_MAX})for(int minimum:{-1,0,7,INT_MIN}) {
        f.reset();CSplitterWnd::CRowColInfo rows[]={{3,10,20},{5,30,40}},cols[]={{7,50,60},{9,70,80}};
        f.splitter()->m_pRowInfo=rows;f.splitter()->m_pColInfo=cols;
        api.setRow(f.splitter(),index,ideal,minimum);api.setColumn(f.splitter(),index,minimum,ideal);
        int r=-7,rMin=-7,c=-7,cMin=-7;api.getRow(f.splitter(),index,&r,&rMin);api.getColumn(f.splitter(),index,&c,&cMin);
        event(17,r,rMin,c,cMin);event(18,rows[index].nIdealSize,cols[index].nIdealSize,rows[1-index].nMinSize,cols[1-index].nIdealSize);f.save(out,0);
        check(r==rows[index].nCurSize && rMin==minimum && cMin==ideal && rows[index].nIdealSize==ideal && cols[index].nIdealSize==minimum,
            "SDK-supplied row/column arrays preserve current sizes and receive both reference outputs");
    }
    return out;
}
static void expectations(const API& api) {
    Fixture f(api);
    kind=3;check(invoke([&]{windows[1]->IsKindOf(root);})==-1 && invoke([&]{root->IsDerivedFrom(nullptr);})==-1,
        "public runtime-type helpers reject null descriptors and null base classes");f.reset();
    check(api.parent(windows[3],FALSE)==f.splitter(),"splitter lookup recognizes the immediate parent's runtime class");
    f.reset();check(api.parent(windows[4],FALSE)==nullptr,"splitter lookup rejects a non-splitter parent even when a splitter is its ancestor");
    f.reset();kind=2;check(api.parent(windows[3],FALSE)==nullptr,"a matching class-name string cannot impersonate splitter runtime ancestry");
    f.reset();minimized=0;check(!api.parent(windows[3],FALSE) && api.parent(windows[3],69)==f.splitter(),"any-state splitter lookup bypasses minimized ancestors");
    f.reset();callbackResult=FALSE;check(api.split(f.view(),0xe135)==TRUE,"split commands remain handled when a splitter's keyboard callback returns FALSE");
    f.reset();mutation=2;api.updateNext(f.view(),&f.ui);
    check(events.back()[0]==6 && events.back()[1]==TRUE,"next-pane updates retain the original UI callback across CanActivateNext mutations");
    f.reset();f.splitter()->m_nRows=2;f.splitter()->m_nCols=3;row=1;column=2;api.activate(f.splitter(),FALSE);
    check(events.back()[0]==10 && events.back()[1]==0 && events.back()[2]==0,"pane activation wraps through virtual GetActivePane and SetActivePane callbacks");
}
static void realCoordinateSmoke(const API& api) {
    Fixture f(api);capture=false;
    for(DWORD style:{DWORD(0),DWORD(WS_EX_LAYOUTRTL)}) {
        SetWindowLongPtrW(handles[1],GWL_EXSTYLE,style);RECT actual={2,3,42,33},expected=actual;
        MapWindowPoints(handles[1],nullptr,reinterpret_cast<POINT*>(&expected),2);api.coordinates(windows[1],&actual);
        check(EqualRect(&actual,&expected)!=FALSE,"rectangle conversion agrees with actual Win32 coordinates in normal and RTL layouts");
    }
}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
        printf("splitter mismatch %zu local:",i);for(auto v:expected[i])printf(" %lld",v);printf(" native:");for(auto v:actual[i])printf(" %lld",v);puts("");break;
    }
    check(actual==expected,label);
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate splitter import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetParent",parent,realParent);HOOK("IsIconic",iconic,realIconic);HOOK("ClientToScreen",coordinates,realCoordinates);HOOK("SetCursorPos",cursor,realCursor);HOOK("GetWindowLongW",style,realStyle);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
}
static void splitterCommands() {
    using namespace splitter_command_test;
    API local{&impl__GetParentSplitter_CView__SAPEAVCSplitterWnd__PEBVCWnd__H_Z,&impl__OnSplitCmd_CView__IEAAHI_Z,&impl__OnNextPaneCmd_CView__IEAAHI_Z,
        &impl__OnUpdateSplitCmd_CView__IEAAXPEAVCCmdUI___Z,&impl__OnUpdateNextPaneMenu_CView__IEAAXPEAVCCmdUI___Z,&impl__CanActivateNext_CSplitterWnd__UEAAHH_Z,
        &impl__ActivateNext_CSplitterWnd__UEAAXH_Z,&impl__DoKeyboardSplit_CSplitterWnd__UEAAHXZ,&impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z,&impl__GetExStyle_CWnd__QEBAKXZ,&impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z,&impl__IsDerivedFrom_CRuntimeClass__QEBAHPEBU1__Z,&impl__GetRowInfo_CSplitterWnd__QEBAXHAEAH0_Z,&impl__GetColumnInfo_CSplitterWnd__QEBAXHAEAH0_Z,&impl__SetRowInfo_CSplitterWnd__QEAAXHHH_Z,&impl__SetColumnInfo_CSplitterWnd__QEAAXHHH_Z};
    Imports imports(GetModuleHandleW(nullptr),true);expectations(local);realCoordinateSmoke(local);
    auto ancestry=parents(local),routing=commands(local),movement=navigation(local),tracking=keyboard(local),types=typeChecks(local),info=dimensions(local);
    check(impl__GetThisClass_CSplitterWnd__SAPEAUCRuntimeClass__XZ()==&CSplitterWnd::classCSplitterWnd && impl__GetRuntimeClass_CSplitterWnd__UEBAPEAUCRuntimeClass__XZ(nullptr)==&CSplitterWnd::classCSplitterWnd,
        "imported splitter runtime accessors share the public canonical descriptor");
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SPLITTER_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for splitter command comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"splitter oracle uses the verified MFC image build");
            if(pinned) {
                Imports nativeImports(native,false);API api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(parent,0x277aa0);RVA(split,0x277c20);RVA(next,0x277cc0);RVA(updateSplit,0x277bd0);RVA(updateNext,0x277c50);
                RVA(can,0x2adf40);RVA(activate,0x2adf80);RVA(keyboard,0x2ac210);RVA(coordinates,0x2a3310);RVA(style,0x2a96c0);
                RVA(kindOf,0x234cf0);RVA(derives,0x234e20);RVA(getRow,0x2aabe0);RVA(getColumn,0x2aac30);RVA(setRow,0x2aac10);RVA(setColumn,0x2aac60);RVA(construct,0x28a700);RVA(attach,0x28adf0);RVA(detach,0x28ae50);RVA(destroy,0x28b740);
#undef RVA
                api.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);nativeLookupReturn=b+0x2a9da9;
                expectations(api);realCoordinateSmoke(api);
                compare(parents(api),ancestry,"splitter runtime ancestry, immediate parents, minimized windows and lookup failures match native MFC");
                compare(commands(api),routing,"split/next-pane commands, tracking guards, cached UI callbacks and callback failures match native MFC");
                compare(navigation(api),movement,"virtual pane lookup, forward/backward wrapping and signed boundary cases match native MFC");
                compare(keyboard(api),tracking,"keyboard split selection, tracking callbacks, RTL/OLE coordinates and interrupted cursor updates match native MFC");
                compare(typeChecks(api),types,"runtime-class null validation and virtual GetRuntimeClass ordering match native MFC");
                compare(dimensions(api),info,"inline row/column arrays and full pointer-width reference outputs match native MFC");
                nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
