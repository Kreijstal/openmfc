#pragma once
extern "C" {
BOOL MS_ABI impl__OnScroll_CView__UEAAHIIH_Z(CView*,UINT,UINT,BOOL);
BOOL MS_ABI impl__OnScrollBy_CView__UEAAHVCSize__H_Z(CView*,CSize,BOOL);
BOOL MS_ABI impl__DoScroll_CSplitterWnd__UEAAHPEAVCView__IH_Z(CSplitterWnd*,CView*,UINT,BOOL);
BOOL MS_ABI impl__DoScrollBy_CSplitterWnd__UEAAHPEAVCView__VCSize__H_Z(CSplitterWnd*,CView*,CSize,BOOL);
BOOL MS_ABI impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(CSplitterWnd*,CWnd*,int*,int*);
int MS_ABI impl__IdFromRowCol_CSplitterWnd__QEBAHHH_Z(const CSplitterWnd*,int,int);
}
namespace splitter_scrolling_test {
using namespace scroll_view_input_test;
struct API {
    splitter_command_test::API graph;
    decltype(&impl__OnScroll_CView__UEAAHIIH_Z) baseScroll;
    decltype(&impl__OnScrollBy_CView__UEAAHVCSize__H_Z) baseScrollBy;
    decltype(&impl__DoScroll_CSplitterWnd__UEAAHPEAVCView__IH_Z) scroll;
    decltype(&impl__DoScrollBy_CSplitterWnd__UEAAHPEAVCView__VCSize__H_Z) scrollBy;
    decltype(&impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z) child;
    decltype(&impl__IdFromRowCol_CSplitterWnd__QEBAHHH_Z) paneID;
};
static CSplitterWnd* splitter;
static CView* source;
static CWnd* bars[2];
static BOOL childResult,results[5];
static int controlID,positions[2],barMask,barCalls,positionCalls,scrollCalls,lookupCalls,peerChoice;
static bool physical;
static INT64 node(const CWnd* w) {for(int i=0;i<5;++i)if(splitter_command_test::windows[i]==w)return i;return -1;}
static INT64 handle(HWND h) {return splitter_command_test::id(h);}
static void changes(int caller) {
    if(mutation==5 && caller==3 && scrollCalls==1){splitter->m_nRows=3;splitter->m_nCols=3;bars[1]->m_hWnd=splitter_command_test::handles[0];}
    if(mutation==6 && caller!=3){splitter->m_nRows=1;splitter->m_nCols=1;}
    if(mutation==10 && caller==3 && scrollCalls==1){positions[0]=-311;positions[1]=INT_MAX;}
}
static BOOL MS_ABI scroll(CView* view,UINT code,UINT position,BOOL perform) {
    ++scrollCalls;event(201,node(view),code,position,perform,splitter->m_nRows,splitter->m_nCols);changes(int(node(view)));
    return results[node(view)];
}
static BOOL MS_ABI scrollBy(CView* view,CSize distance,BOOL perform) {
    ++scrollCalls;event(202,node(view),distance.cx,distance.cy,perform,splitter->m_nRows,splitter->m_nCols);changes(int(node(view)));
    if(physical && perform) {
        ::ScrollWindow(view->m_hWnd,-distance.cx,-distance.cy,nullptr,nullptr);
        if(distance.cx)::SetScrollPos(bars[0]->m_hWnd,SB_CTL,::GetScrollPos(bars[0]->m_hWnd,SB_CTL)+distance.cx,FALSE);
        if(distance.cy)::SetScrollPos(bars[1]->m_hWnd,SB_CTL,::GetScrollPos(bars[1]->m_hWnd,SB_CTL)+distance.cy,FALSE);
    }
    return results[node(view)];
}
static BOOL MS_ABI alternateScroll(CView* view,UINT code,UINT position,BOOL perform) {event(210,node(view),code,position,perform);return -7;}
static BOOL MS_ABI alternateScrollBy(CView* view,CSize distance,BOOL perform) {event(211,node(view),distance.cx,distance.cy,perform);return -7;}
static CScrollBar* MS_ABI alternateBar(const CWnd* w,int axis) {event(209,node(w),axis);return barMask&(1<<axis)?reinterpret_cast<CScrollBar*>(bars[axis]):nullptr;}
static CScrollBar* MS_ABI bar(const CWnd* w,int axis) {
    event(200,node(w),axis);++barCalls;
    if(mutation==3 && axis==SB_VERT)(*reinterpret_cast<void***>(source))[32]=reinterpret_cast<void*>(&alternateBar);
    return barMask&(1<<axis)?reinterpret_cast<CScrollBar*>(bars[axis]):nullptr;
}
static void* nativeLookupReturn;
static decltype(&::GetParent) realParent;
// Native FromHandle probes a parent container through its private fields.
// These test containers expose only the GetDlgItem virtual; skip that unrelated
// ownership probe at its verified return address.
static HWND WINAPI parent(HWND h) {return _ReturnAddress()==nativeLookupReturn?nullptr:realParent(h);}
static decltype(&::GetDlgCtrlID) realID;
static decltype(&::IsChild) realChild;
static decltype(&::GetDlgItem) realItem;
static decltype(&::GetScrollPos) realGetPosition;
static decltype(&::SetScrollPos) realSetPosition;
static decltype(&::ScrollWindowEx) realScroll=&::ScrollWindowEx;
static decltype(&::UpdateWindow) realUpdate=&::UpdateWindow;
static int WINAPI id(HWND h) {
    if(!capture)return realID(h);event(203,handle(h));
    if(mutation==1){source->m_hWnd=splitter_command_test::handles[0];splitter->m_hWnd=splitter_command_test::handles[2];}
    return controlID;
}
static BOOL WINAPI child(HWND parent,HWND h) {
    if(!capture)return realChild(parent,h);event(204,handle(parent),handle(h));
    if(mutation==2)source->m_hWnd=nullptr;return childResult;
}
static CWnd* peer(int id) {
    if(peerChoice==1)return splitter_command_test::windows[4];if(peerChoice==2)return source;
    return splitter_command_test::windows[(id&1)?2:4];
}
static void afterLookup() {
    ++lookupCalls;
    if(mutation==9 && lookupCalls==1)for(int i:{2,3,4}){auto** table=*reinterpret_cast<void***>(splitter_command_test::windows[i]);table[92]=reinterpret_cast<void*>(&alternateScroll);table[93]=reinterpret_cast<void*>(&alternateScrollBy);}
}
static void* containerTable[32];
static void** container=containerTable;
static HWND WINAPI item(HWND h,int id) {
    if(!capture)return realItem(h,id);event(205,handle(h),id);CWnd* result=peer(id);afterLookup();
    if(mutation==8 && lookupCalls==1)splitter->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
    return splitter_command_test::handles[node(result)];
}
static CWnd* MS_ABI containerItem(void*,int id) {event(208,id);CWnd* result=peer(id);afterLookup();return result;}
static int WINAPI getPosition(HWND h,int axis) {
    if(!capture)return realGetPosition(h,axis);int value=h==bars[1]->m_hWnd?positions[1]:positions[0];event(206,handle(h),axis);++positionCalls;
    if(mutation==4 && positionCalls==1)bars[1]->m_hWnd=splitter_command_test::handles[0];
    return value;
}
static int WINAPI setPosition(HWND h,int axis,int value,BOOL redraw) {
    if(!capture)return realSetPosition(h,axis,value,redraw);event(207,handle(h),axis,value,redraw);
    if(mutation==7)splitter->m_hWnd=splitter_command_test::handles[0];return INT_MIN;
}
static int WINAPI scrollWindow(HWND h,int x,int y,const RECT* r,const RECT* clip,HRGN region,RECT* update,UINT flags) {
    if(!capture)return realScroll(h,x,y,r,clip,region,update,flags);event(212,handle(h),x,y,flags);return ERROR;
}
static BOOL WINAPI update(HWND h) {if(!capture)return realUpdate(h);event(213,handle(h));return FALSE;}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate splitter scrolling import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetParent",parent,realParent);HOOK("GetDlgCtrlID",id,realID);HOOK("IsChild",child,realChild);HOOK("GetDlgItem",item,realItem);HOOK("GetScrollPos",getPosition,realGetPosition);HOOK("SetScrollPos",setPosition,realSetPosition);HOOK("ScrollWindowEx",scrollWindow,realScroll);HOOK("UpdateWindow",update,realUpdate);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    splitter_command_test::Fixture graph;
    Fixture(const API& api):graph(api.graph) {splitter=graph.splitter();source=graph.view();reset();}
    void reset() {
        graph.reset();events.clear();failure=mutation=barCalls=positionCalls=scrollCalls=lookupCalls=0;barMask=3;childResult=69;controlID=0xe900;peerChoice=0;physical=false;positions[0]=37;positions[1]=41;
        for(int i=0;i<5;++i){auto* w=splitter_command_test::windows[i];w->m_hWnd=splitter_command_test::handles[i];w->m_pCtrlCont=nullptr;w->m_pCtrlSite=nullptr;graph.table[i][32]=reinterpret_cast<void*>(&bar);graph.table[i][92]=reinterpret_cast<void*>(&scroll);graph.table[i][93]=reinterpret_cast<void*>(&scrollBy);results[i]=FALSE;}
        bars[0]=splitter_command_test::windows[2];bars[1]=splitter_command_test::windows[4];splitter->m_nRows=splitter->m_nCols=3;containerTable[31]=reinterpret_cast<void*>(&containerItem);capture=true;
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        out.insert(out.end(),events.begin(),events.end());out.push_back({220,result,caught,splitter->m_nRows,splitter->m_nCols,handle(source->m_hWnd),handle(bars[0]->m_hWnd),handle(bars[1]->m_hWnd)});
        for(int i=0;i<5;++i)graph.boundaries=graph.boundaries && *reinterpret_cast<UINT64*>(graph.bytes[i]+384)==0xa5a5a5a5a5a5a5a5ULL;
    }
    ~Fixture(){capture=false;failure=0;}
};
template<class F>static int invoke(F action){try{action();return 0;}catch(int n){return n;}}
static std::vector<Event> baseViews(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(UINT code:{0u,1u,5u,0xff01u,UINT_MAX})for(BOOL perform:{FALSE,BOOL(69),BOOL(-1)})for(int pointer:{0,1,2}) {
        f.reset();CView* view=pointer==0?nullptr:pointer==1?reinterpret_cast<CView*>(UINT_PTR(1)):source;
        BOOL result=api.baseScroll(view,code,UINT_MAX,perform);f.save(out,result);
        result=api.baseScrollBy(view,CSize(INT_MIN,INT_MAX),perform);f.save(out,result);
    }
    return out;
}
static std::vector<Event> membership(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int identifier:{INT_MIN,-1,0xe8ff,0xe900,0xe901,0xe90f,0xe9ff,0xea00,INT_MAX})for(BOOL inside:{FALSE,BOOL(69),BOOL(-1)})for(int absent:{0,1,2,3})for(int alias:{0,1,2,3,4})for(int mode:{0,1,2})for(int at:{0,203,204}) {
        f.reset();controlID=identifier;childResult=inside;mutation=mode;failure=at;if(absent&1)source->m_hWnd=nullptr;if(absent&2)splitter->m_hWnd=nullptr;
        int row=37,col=41;int* rowOut=alias==1?nullptr:alias==4?&splitter->m_nRows:&row;int* colOut=alias==2?nullptr:alias==3?&row:&col;BOOL result=17;
        int caught=invoke([&]{result=api.child(splitter,source,rowOut,colOut);});f.save(out,result,caught);out.push_back({221,row,col,0,0,0,0,0});
    }
    for(int row:{INT_MIN,-1,0,1,15,INT_MAX})for(int col:{INT_MIN,-1,0,1,15,INT_MAX}) {f.reset();out.push_back({222,api.paneID(splitter,row,col),0,0,0,0,0,0});}
    return out;
}
static std::vector<Event> scrolling(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int operation:{0,1})for(int axes:{0,1,2,3})for(int count:{-1,0,1,2,3})for(int origin:{0xe8ff,0xe900,0xe901,0xe910,0xe911,0xe9ff})for(BOOL perform:{FALSE,BOOL(69),BOOL(-1)})for(int returned:{0,69,-7})for(bool ole:{false,true}) {
        f.reset();barMask=axes;splitter->m_nRows=count;splitter->m_nCols=count;controlID=origin;results[3]=returned;results[2]=returned;results[4]=returned;
        if(ole)splitter->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
        BOOL result=operation?api.scrollBy(splitter,source,CSize(INT_MIN,INT_MAX),perform):api.scroll(splitter,source,0xabcdef01u,perform);f.save(out,result);
    }
    for(int operation:{0,1})for(int mode=0;mode<=10;++mode)for(int at:{0,200,201,202,203,204,205,206,207,208,209,210,211})for(int choice:{0,1,2}) {
        f.reset();mutation=mode;failure=at;peerChoice=choice;results[2]=69;results[3]=FALSE;results[4]=-7;controlID=0xe911;BOOL result=17;
        int caught=invoke([&]{result=operation?api.scrollBy(splitter,source,CSize(-37,41),69):api.scroll(splitter,source,0xdeadbeefu,69);});f.save(out,result,caught);
    }
    return out;
}
static void expectations(const API& api) {
    Fixture f(api);check(api.baseScroll(source,SB_LINEDOWN,17,TRUE)==FALSE && api.baseScrollBy(source,CSize(7,9),TRUE)==FALSE && events.empty(),"base views decline scrolling without calling Win32 or derived handlers");
    f.reset();results[3]=69;BOOL result=api.scroll(splitter,source,0xabcdef01u,-1);
    auto calls=events;check(result==TRUE && calls.size()>8 && calls[0][0]==203 && calls[1][0]==204 && calls[2][0]==200 && calls[2][2]==SB_VERT && calls[4][0]==200 && calls[4][2]==SB_HORZ && calls[6][0]==201 && calls[6][2]==0xabcdef01u && calls[6][3]==0 && calls[6][4]==-1,
        "splitter scrolling validates membership, snapshots vertical then horizontal bars and dispatches the source pane virtually");
    check(std::any_of(calls.begin(),calls.end(),[](const Event& e){return e[0]==201 && e[2]==0xefffu;}) && std::any_of(calls.begin(),calls.end(),[](const Event& e){return e[0]==201 && e[2]==0xff01u;}),"sibling scroll commands retain only their shared axis and discard high code bits");
    f.reset();results[3]=FALSE;results[4]=-7;check(api.scrollBy(splitter,source,CSize(7,-9),FALSE)==TRUE && std::count_if(events.begin(),events.end(),[](const Event& e){return e[0]==207;})==4,"probe scrolling still restores shared positions and combines pane results into canonical BOOL");
}
static void realWindows(const API& api) {
    Fixture f(api);capture=false;splitter_command_test::capture=false;physical=true;splitter->m_nRows=splitter->m_nCols=2;
    SetParent(splitter_command_test::handles[4],splitter->m_hWnd);
    for(int index:{2,3,4})SetWindowLongPtrW(splitter_command_test::handles[index],GWLP_ID,index==3?0xe900:index==2?0xe901:0xe910);
    alignas(8) BYTE barBytes[2][232]{};HWND barHandles[2]{};
    for(int axis=0;axis<2;++axis){barHandles[axis]=CreateWindowExW(0,L"SCROLLBAR",L"",WS_CHILD|(axis?SBS_VERT:SBS_HORZ),0,0,80,80,splitter->m_hWnd,nullptr,GetModuleHandleW(nullptr),nullptr);bars[axis]=reinterpret_cast<CWnd*>(barBytes[axis]);bars[axis]->m_hWnd=barHandles[axis];SetScrollRange(barHandles[axis],SB_CTL,0,100,FALSE);SetScrollPos(barHandles[axis],SB_CTL,positions[axis],FALSE);}
    HWND markers[3];RECT before[3]{},after[3]{};int indices[3]={3,2,4};
    for(int i=0;i<3;++i){markers[i]=CreateWindowExW(0,L"STATIC",L"",WS_CHILD,5,7,10,10,splitter_command_test::handles[indices[i]],nullptr,GetModuleHandleW(nullptr),nullptr);GetWindowRect(markers[i],&before[i]);}
    results[3]=69;BOOL result=api.scrollBy(splitter,source,CSize(7,-11),TRUE);
    for(int i=0;i<3;++i)GetWindowRect(markers[i],&after[i]);
    check(result==TRUE && after[0].left-before[0].left==-7 && after[0].top-before[0].top==11 && after[1].left==before[1].left && after[1].top-before[1].top==11 && after[2].left-before[2].left==-7 && after[2].top==before[2].top,
        "real splitter scrolling moves each pane's child window along its shared axes");
    check(GetScrollPos(barHandles[0],SB_CTL)==44 && GetScrollPos(barHandles[1],SB_CTL)==30,"real splitter dispatch advances each shared scrollbar once across all panes");
    for(HWND h:markers)DestroyWindow(h);for(HWND h:barHandles)DestroyWindow(h);bars[0]=splitter_command_test::windows[2];bars[1]=splitter_command_test::windows[4];
}
}
static void splitterScrolling() {
    using namespace splitter_scrolling_test;
    // Keep the negative-call probes available in the IAT with optimization.
    auto volatile scrollImport=&::ScrollWindowEx;auto volatile updateImport=&::UpdateWindow;
    splitter_scrolling_test::API local{{},&impl__OnScroll_CView__UEAAHIIH_Z,&impl__OnScrollBy_CView__UEAAHVCSize__H_Z,&impl__DoScroll_CSplitterWnd__UEAAHPEAVCView__IH_Z,&impl__DoScrollBy_CSplitterWnd__UEAAHPEAVCView__VCSize__H_Z,&impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z,&impl__IdFromRowCol_CSplitterWnd__QEBAHHH_Z};
    splitter_scrolling_test::Imports imports(GetModuleHandleW(nullptr),true);expectations(local);auto base=baseViews(local),members=membership(local),scrolls=scrolling(local);realWindows(local);
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_SPLITTER_SCROLL_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for splitter scrolling comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"splitter scroll oracle uses the verified MFC image build");
            if(pinned) {
                splitter_scrolling_test::Imports nativeImports(native,false);auto api=local;
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(baseScroll,0x71e0);RVA(baseScrollBy,0x71e0);RVA(scroll,0x2adb00);RVA(scrollBy,0x2add10);RVA(child,0x2aab60);RVA(paneID,0x2aab30);
#undef RVA
                api.graph.construct=reinterpret_cast<decltype(api.graph.construct)>(b+0x28a700);api.graph.attach=reinterpret_cast<decltype(api.graph.attach)>(b+0x28adf0);api.graph.detach=reinterpret_cast<decltype(api.graph.detach)>(b+0x28ae50);api.graph.destroy=reinterpret_cast<decltype(api.graph.destroy)>(b+0x28b740);api.graph.type=reinterpret_cast<CRuntimeClass*>(b+0x332860);
                nativeLookupReturn=b+0x2a9da9;expectations(api);scroll_view_input_test::compare(baseViews(api),base,"base-view scrolling is the native unconditional FALSE body");
                scroll_view_input_test::compare(membership(api),members,"pane membership queries, output aliases, ID arithmetic and interrupted operations match native MFC");
                scroll_view_input_test::compare(scrolling(api),scrolls,"splitter scrolling virtuals, shared-bar snapshots, sibling axes and callback mutations match native MFC");realWindows(api);nativeLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
