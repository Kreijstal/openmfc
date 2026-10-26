#pragma once
#include <thread>
extern "C" {
void MS_ABI impl__OnPaint_CView__IEAAXXZ(CView*);
void MS_ABI impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(CView*,CDC*,void*);
void MS_ABI impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z(CView*,CDC*,void*);
LRESULT MS_ABI impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z(CView*,CDC*,UINT);
CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC*,CWnd*);
void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC*);
BOOL MS_ABI impl__Attach_CDC__QEAAHPEAUHDC_____Z(CDC*,HDC);
HDC MS_ABI impl__Detach_CDC__QEAAPEAUHDC____XZ(CDC*);
CDC* MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC);
void MS_ABI impl__DeleteTempMap_CDC__SAXXZ();
}
namespace view_rendering_test {
using Event=std::array<INT64,6>;
struct API {
    decltype(&impl__OnPaint_CView__IEAAXXZ) paint;
    decltype(&impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z) prepare;
    decltype(&impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z) print;
    decltype(&impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z) client;
    decltype(&impl___0CPaintDC__QEAA_PEAVCWnd___Z) construct;
    decltype(&impl___1CPaintDC__UEAA_XZ) destroy;
    decltype(&impl__Attach_CDC__QEAAHPEAUHDC_____Z) attach;
    decltype(&impl__Detach_CDC__QEAAPEAUHDC____XZ) detach;
    decltype(&impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z) lookup;
    decltype(&impl__DeleteTempMap_CDC__SAXXZ) clear;
};
static const API* current;
static std::vector<Event> events;
static HWND window,otherWindow;
static HDC output,attribute;
static bool capture,beginFails,renderPixels;
static int failure,mutation;
static LRESULT eraseResult;
static CDC* activeDC;
static bool mapped;
static decltype(&::BeginPaint) realBegin;
static decltype(&::EndPaint) realEnd;
static decltype(&::SendMessageW) realSend;
static decltype(&::DeleteDC) realDelete;
static INT64 dcID(HDC dc) {return !dc?-1:dc==output?1:dc==attribute?2:-2;}
static INT64 windowID(HWND h) {return h==window?1:h==otherWindow?2:-1;}
static void event(INT64 id,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0) {events.push_back({id,a,b,c,d,e});if(failure==id)throw 91;}
static HDC WINAPI begin(HWND h,PAINTSTRUCT* ps) {
    if(!capture)return realBegin(h,ps);
    event(1,windowID(h));memset(ps,0x5a,sizeof(*ps));ps->hdc=beginFails?nullptr:output;return ps->hdc;
}
static BOOL WINAPI end(HWND h,const PAINTSTRUCT* ps) {
    if(!capture)return realEnd(h,ps);
    event(2,windowID(h),dcID(ps->hdc),ps->fErase);return FALSE;
}
static LRESULT WINAPI send(HWND h,UINT message,WPARAM wp,LPARAM lp) {
    if(!capture)return realSend(h,message,wp,lp);
    event(3,windowID(h),message,dcID(reinterpret_cast<HDC>(wp)),lp);return eraseResult;
}
static BOOL WINAPI deleteDC(HDC dc) {if(!capture)return realDelete(dc);event(4,dcID(dc));return TRUE;}
static void MS_ABI prepare(CView*,CDC* dc,void* info) {
    activeDC=dc;mapped=current->lookup(dc->m_hDC)==dc;
    event(5,dcID(dc->m_hDC),dcID(dc->m_hAttribDC),info==nullptr,mapped);
    dc->m_hAttribDC=attribute;
    if(mutation==1)reinterpret_cast<CPaintDC*>(dc)->m_hWnd=otherWindow;
}
static void MS_ABI draw(CView*,void* pointer) {
    auto* dc=static_cast<CDC*>(pointer);activeDC=dc;
    if(dc)mapped=current->lookup(dc->m_hDC)==dc;
    event(6,dc!=nullptr,dc?dcID(dc->m_hDC):-1,dc?dcID(dc->m_hAttribDC):-1,dc?dc->m_bPrinting:-1);
    if(renderPixels && dc)SetPixel(dc->m_hDC,1,1,RGB(31,97,173));
}
static void MS_ABI setAttribute(CDC* dc,HDC handle) {event(7,dcID(handle));dc->m_hAttribDC=mutation==2?attribute:handle;}
static void MS_ABI releaseAttribute(CDC* dc) {event(8,dcID(dc->m_hDC),dcID(dc->m_hAttribDC));dc->m_hAttribDC=nullptr;}
struct Fixture {
    alignas(8) BYTE bytes[256]{};void* table[111]{};const API& api;
    Fixture(const API& functions):api(functions) {
        capture=false;current=&api;window=CreateWindowExW(0,L"STATIC",L"view paint",WS_OVERLAPPED,0,0,100,80,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
        otherWindow=CreateWindowExW(0,L"STATIC",L"paint target",WS_OVERLAPPED,0,0,100,80,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
        output=CreateCompatibleDC(nullptr);attribute=CreateCompatibleDC(nullptr);
        check(window && otherWindow && output && attribute,"create actual window and DC resources for view rendering");
        *reinterpret_cast<void***>(bytes)=table;view()->m_hWnd=window;table[100]=reinterpret_cast<void*>(&prepare);table[105]=reinterpret_cast<void*>(&draw);
        memset(bytes+248,0xa5,8);reset();capture=true;
    }
    CView* view() {return reinterpret_cast<CView*>(bytes);}
    void reset() {events.clear();failure=mutation=0;beginFails=false;activeDC=nullptr;mapped=false;eraseResult=-123456789123LL;}
    void save(std::vector<Event>& outputEvents,INT64 result,int caught=0) {
        outputEvents.insert(outputEvents.end(),events.begin(),events.end());outputEvents.push_back({90,result,caught,mapped,0,0});
        check(*reinterpret_cast<UINT64*>(bytes+248)==0xa5a5a5a5a5a5a5a5ULL,"view rendering preserves the native view boundary");
    }
    ~Fixture() {capture=false;api.clear();DeleteDC(output);DeleteDC(attribute);DestroyWindow(window);DestroyWindow(otherWindow);}
};
template<class F> static int invoke(F f) {try{f();return 0;}catch(int n){return n;}catch(CException* e){e->Delete();return 92;}}
static std::vector<Event> painting(const API& api) {
    std::vector<Event> result;Fixture f(api);
    for(bool failed:{false,true})for(int mode:{0,1})for(int at:{0,5,6}) {
        f.reset();beginFails=failed;mutation=mode;failure=at;
        int caught=invoke([&]{api.paint(f.view());});f.save(result,0,caught);
        check(failed?events.size()==1:events.back()[0]==2,"painting balances BeginPaint/EndPaint on normal and callback exception paths");
        check(failed || std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==4;}),"a borrowed paint HDC never reaches DeleteDC");
        auto* painted=activeDC;f.reset();auto* wrapper=api.lookup(output);check(wrapper!=painted,"paint destruction removes the permanent DC wrapper");api.clear();
    }
    return result;
}
static std::vector<Event> printing(const API& api) {
    std::vector<Event> result;Fixture f(api);CDC dc;dc.m_hDC=output;dc.m_hAttribDC=attribute;dc.m_bPrinting=69;
    for(bool nullDC:{false,true})for(UINT flags:{0u,1u,2u,4u,8u,12u,16u,31u,0x8000000cu,UINT(-1)})for(int at:{0,3,6}) {
        f.reset();failure=at;LRESULT returned=-7;int caught=invoke([&]{returned=api.client(f.view(),nullDC?nullptr:&dc,flags);});f.save(result,returned,caught);
    }
    for(bool nullDC:{false,true})for(int at:{0,6}) {
        f.reset();failure=at;int caught=invoke([&]{api.print(f.view(),nullDC?nullptr:&dc,reinterpret_cast<void*>(UINT_PTR(0x123)));});f.save(result,0,caught);
    }
    // Base preparation only sets CPrintInfo's continuation flag. It must not
    // reset an overridden mapping mode or viewport origin on either HDC.
    alignas(8) BYTE dialog[0x140],info[64];PRINTDLGW parameters{};
    for(WORD maximum:{WORD(0),WORD(1),WORD(100),WORD(0xffff)})for(UINT page:{0u,1u,2u,UINT(-1)}) {
        f.reset();memset(dialog,0xa5,sizeof(dialog));memset(info,0xa5,sizeof(info));*reinterpret_cast<void**>(info)=dialog;
        *reinterpret_cast<void**>(dialog+0x130)=&parameters;parameters.nMaxPage=maximum;*reinterpret_cast<UINT*>(info+0x18)=page;
        SetMapMode(output,MM_ANISOTROPIC);SetMapMode(attribute,MM_LOMETRIC);SetViewportOrgEx(output,7,9,nullptr);SetViewportOrgEx(attribute,11,13,nullptr);
        api.prepare(f.view(),&dc,info);POINT a{},b{};GetViewportOrgEx(output,&a);GetViewportOrgEx(attribute,&b);
        event(10,*reinterpret_cast<BOOL*>(info+0x14),GetMapMode(output),GetMapMode(attribute),a.x,b.y);f.save(result,0);
        check(*reinterpret_cast<BOOL*>(info+0x14)==(maximum!=0xffff || page==1) && a.x==7 && b.y==13,"base print preparation updates only page continuation and preserves both DC mappings");
    }
    f.reset();api.prepare(f.view(),nullptr,nullptr);f.save(result,0);
    dc.m_hDC=dc.m_hAttribDC=nullptr;return result;
}
static std::vector<Event> paintLifetime(const API& api) {
    std::vector<Event> result;Fixture f(api);
    for(bool failed:{false,true}) {
        alignas(8) BYTE raw[120];memset(raw,0xa5,sizeof(raw));auto* dc=reinterpret_cast<CPaintDC*>(raw);f.reset();beginFails=failed;
        int caught=invoke([&]{api.construct(dc,f.view());});
        event(11,dcID(dc->m_hDC),dcID(dc->m_hAttribDC),dc->m_bPrinting,windowID(dc->m_hWnd),dc->m_ps.fErase);
        if(!caught) {
            event(12,api.lookup(output)==dc);dc->m_hWnd=otherWindow;dc->m_hAttribDC=attribute;api.destroy(dc);
            event(13,dcID(dc->m_hDC),dcID(dc->m_hAttribDC));
        }
        f.save(result,0,caught);check(*reinterpret_cast<UINT64*>(raw+112)==0xa5a5a5a5a5a5a5a5ULL,"paint constructor/destructor preserve the native 112-byte object boundary");
    }
    return result;
}
static std::vector<Event> handles(const API& api) {
    std::vector<Event> result;Fixture f(api);
    for(bool nullHandle:{false,true})for(int mode:{0,2})for(int at:{0,7,8}) {
        alignas(8) BYTE bytes[40]{};void* table[32]{};*reinterpret_cast<void***>(bytes)=table;
        auto* dc=reinterpret_cast<CDC*>(bytes);table[5]=reinterpret_cast<void*>(&setAttribute);table[7]=reinterpret_cast<void*>(&releaseAttribute);
        dc->m_hAttribDC=attribute;f.reset();mutation=mode;failure=at;BOOL returned=-7;
        int caught=invoke([&]{returned=api.attach(dc,nullHandle?nullptr:output);});
        event(14,returned,dcID(dc->m_hDC),dcID(dc->m_hAttribDC),caught,api.lookup(output)==dc);
        HDC old=nullptr;caught=invoke([&]{old=api.detach(dc);});
        event(15,dcID(old),dcID(dc->m_hDC),dcID(dc->m_hAttribDC),caught,api.lookup(output)==dc);f.save(result,0);
        failure=0;api.detach(dc);api.clear();
    }
    return result;
}
static void expectations(const API& api) {
    Fixture f(api);CDC dc;dc.m_hDC=output;dc.m_hAttribDC=attribute;
    check(api.client(f.view(),&dc,PRF_CHECKVISIBLE)==0 && events.empty(),"print-client ignores visibility-only flags without preparing or drawing");
    check(api.client(f.view(),&dc,PRF_CLIENT|PRF_ERASEBKGND)==0 && events.size()==2 && events[0][0]==3 && events[0][2]==WM_ERASEBKGND && events[1][0]==6,
        "print-client erases before drawing and returns zero regardless of the erase result");
    f.reset();api.print(f.view(),&dc,nullptr);
    check(events.size()==1 && events[0][0]==6,"base OnPrint dispatches directly to virtual OnDraw without preparing the DC again");
    dc.m_hDC=dc.m_hAttribDC=nullptr;
}
static void realRendering(const API& api) {
    Fixture f(api);capture=false;renderPixels=true;
    HBITMAP bitmap=CreateBitmap(8,8,1,32,nullptr);auto previous=SelectObject(output,bitmap);CDC dc;
    dc.m_hDC=dc.m_hAttribDC=output;SetPixel(output,1,1,RGB(0,0,0));
    check(api.client(f.view(),&dc,PRF_CLIENT)==0 && GetPixel(output,1,1)==RGB(31,97,173),
        "real print-client rendering draws into the supplied borrowed HDC");
    ShowWindow(window,SW_SHOW);InvalidateRect(window,nullptr,FALSE);f.reset();api.paint(f.view());
    HDC screen=GetDC(window);bool visible=GetPixel(screen,1,1)==RGB(31,97,173);ReleaseDC(window,screen);
    check(visible && GetUpdateRect(window,nullptr,FALSE)==FALSE && mapped,
        "real painting registers the active paint DC, renders pixels and validates the update region");
    renderPixels=false;dc.m_hDC=dc.m_hAttribDC=nullptr;SelectObject(output,previous);DeleteObject(bitmap);
}
static void moduleIsolation(const API& api) {
    Fixture f(api);capture=false;AFX_MODULE_STATE first,second;auto* saved=g_cmdTestCurrentModule;CDC a,b;
    g_cmdTestCurrentModule=&first;api.attach(&a,output);
    bool owned=api.lookup(output)==&a,workerIsolated=false;
    std::thread worker([&] {
        g_cmdTestCurrentModule=&first;CDC own;
        workerIsolated=api.lookup(output)!=&a && api.attach(&own,output) && api.lookup(output)==&own;
        api.detach(&own);api.clear();g_cmdTestCurrentModule=nullptr;
    });worker.join();
    g_cmdTestCurrentModule=&second;bool separate=api.lookup(output)!=&a;api.attach(&b,output);
    g_cmdTestCurrentModule=&first;bool retained=api.lookup(output)==&a;api.detach(&a);api.clear();
    g_cmdTestCurrentModule=&second;bool otherRetained=api.lookup(output)==&b;api.detach(&b);api.clear();g_cmdTestCurrentModule=saved;
    check(owned && separate && retained && otherRetained && workerIsolated,
        "permanent and temporary DC wrappers stay isolated across framework modules and threads");
}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
        printf("render mismatch %zu local:",i);for(auto v:expected[i])printf(" %lld",v);printf(" native:");for(auto v:actual[i])printf(" %lld",v);puts("");break;
    }
    check(actual==expected,label);
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE module,bool originals) {
#define HOOK(name,callback,saved) {auto** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate view-rendering import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("BeginPaint",begin,realBegin);HOOK("EndPaint",end,realEnd);HOOK("SendMessageW",send,realSend);HOOK("DeleteDC",deleteDC,realDelete);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
}
static void viewRendering() {
    using namespace view_rendering_test;
    API local{&impl__OnPaint_CView__IEAAXXZ,&impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z,&impl__OnPrint_CView__MEAAXPEAVCDC__PEAUCPrintInfo___Z,
        &impl__OnPrintClient_CView__IEAA_JPEAVCDC__I_Z,&impl___0CPaintDC__QEAA_PEAVCWnd___Z,&impl___1CPaintDC__UEAA_XZ,
        &impl__Attach_CDC__QEAAHPEAUHDC_____Z,&impl__Detach_CDC__QEAAPEAUHDC____XZ,&impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z,&impl__DeleteTempMap_CDC__SAXXZ};
    Imports imports(GetModuleHandleW(nullptr),true);expectations(local);
    auto paint=painting(local),print=printing(local),life=paintLifetime(local),ownership=handles(local);realRendering(local);moduleIsolation(local);
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_VIEW_RENDERING_DLL",path,1024)) {
        HMODULE module=LoadLibraryW(path);check(module!=nullptr,"load native MFC for view-rendering comparisons");
        if(module) {
            auto* b=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"view-rendering oracle uses the verified MFC image build");
            if(pinned) {
                API api=local;Imports nativeImports(module,false);
#define RVA(field,offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(paint,0x277860);RVA(prepare,0x277d00);RVA(print,0x277940);RVA(client,0x277960);
                RVA(construct,0x2a3d20);RVA(destroy,0x2a3dd0);RVA(attach,0x2a2480);RVA(detach,0x2a24d0);
                RVA(lookup,0x2a2450);RVA(clear,0x2a6250);
#undef RVA
                expectations(api);realRendering(api);
                compare(painting(api),paint,"paint preparation, drawing, failed acquisition and exception cleanup match native MFC");
                compare(printing(api),print,"print-client flags, erasure ordering, drawing and page continuation match native MFC");
                compare(paintLifetime(api),life,"paint DC layout, saved HWND, registration and handle detachment match native MFC");
                compare(handles(api),ownership,"DC attach/detach virtual callbacks, permanent lookup and failures match native MFC");
            }
            FreeLibrary(module);
        }
    }
}
