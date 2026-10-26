#pragma once
#include "detail/WindowActivationSupport.h"
namespace window_activation_test {
using Event=std::array<INT64,8>;
struct API {
    BOOL (*initialize)();
    BOOL (*activate)(ULONG_PTR*);
    HWND (*create)(CREATESTRUCTW);
    BOOL (*menu)(WNDCLASSEXW*);
    BOOL *cleanup,*bypass,*unavailable;
    HANDLE* context;
};
static const API* active;
static std::vector<Event> events;
static int failure,mutation,stage,activateCalls;
static DWORD error;
static HANDLE queried,created;
static BOOL activated,sectionFound,apiResult;
static void event(INT64 a,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0,INT64 h=0) {
    events.push_back({a,b,c,d,e,f,g,h});if(failure==a)throw 91;
}
static INT64 handle(HANDLE h){return reinterpret_cast<INT_PTR>(h);}
static BOOL WINAPI query(DWORD flags,HANDLE context,const void* sub,ULONG cls,void* info,SIZE_T length,SIZE_T* written) {
    event(1,flags,context==active->context,sub==nullptr,cls,length,written==nullptr);
    if(stage==1){SetLastError(error);return FALSE;}
    static_cast<ACTIVATION_CONTEXT_BASIC_INFORMATION*>(info)->hActCtx=queried;return 69;
}
static BOOL WINAPI module(DWORD flags,LPCWSTR address,HMODULE* out) {
    event(2,flags,address==reinterpret_cast<LPCWSTR>(active->context));
    if(stage==2){SetLastError(error);return FALSE;}*out=reinterpret_cast<HMODULE>(UINT_PTR(0x2233));return 69;
}
static DWORD WINAPI filename(HMODULE module,LPWSTR out,DWORD length) {
    event(3,handle(module),length);if(stage==3){SetLastError(error);return 0;}
    if(stage==4)return 261;wcscpy_s(out,length,L"activation-fixture.dll");return 22;
}
static HANDLE WINAPI createContext(const ACTCTXW* info) {
    event(4,info->cbSize,info->dwFlags,wcscmp(info->lpSource,L"activation-fixture.dll")==0,
        handle(const_cast<wchar_t*>(info->lpResourceName)),handle(info->hModule));
    SetLastError(error);return created;
}
static BOOL WINAPI activateContext(HANDLE context,ULONG_PTR* cookie) {
    event(5,handle(context),*cookie);++activateCalls;
    if(mutation==1)*active->unavailable=69;
    if(mutation==2)*active->bypass=69;
    if(!activated){SetLastError(error);return FALSE;}*cookie=0x123456789ULL;return activated;
}
static BOOL WINAPI findSection(DWORD flags,const GUID* extension,ULONG section,LPCWSTR name,PACTCTX_SECTION_KEYED_DATA out) {
    event(6,flags,extension==nullptr,section,wcscmp(name,L"Comctl32.dll")==0,out->cbSize);return sectionFound;
}
static HMODULE WINAPI load(LPCWSTR name) {event(7,wcscmp(name,L"Comctl32.dll")==0);return nullptr;}
static BOOL WINAPI deactivate(DWORD flags,ULONG_PTR cookie) {event(8,flags,cookie);SetLastError(0x54321);return FALSE;}
static void WINAPI diagnostic(LPCSTR text) {event(9,strcmp(text,"IsolationAware function called after IsolationAwareCleanup\n")==0);}
static HWND WINAPI createWindow(DWORD ex,LPCWSTR cls,LPCWSTR title,DWORD style,int x,int y,int cx,int cy,HWND parent,HMENU menu,HINSTANCE instance,void* param) {
    event(10,ex,handle(const_cast<wchar_t*>(cls)),handle(const_cast<wchar_t*>(title)),style,x,y,cx);
    event(11,cy,handle(parent),handle(menu),handle(instance),handle(param));
    if(mutation==3){*active->bypass=69;*active->unavailable=-7;}
    SetLastError(error);return apiResult?reinterpret_cast<HWND>(UINT_PTR(0x9988)):nullptr;
}
static BOOL WINAPI classInfo(HINSTANCE instance,LPCWSTR name,WNDCLASSEXW* info) {
    event(12,handle(instance),wcscmp(name,L"#32768")==0,info->cbSize);
    if(mutation==3){*active->bypass=69;*active->unavailable=-7;}
    SetLastError(error);return apiResult;
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    explicit Imports(HMODULE image) {
#define HOOK(name,callback) {auto** slot=recordset_value_test::import(image,name);check(slot!=nullptr,"locate window activation import " name);if(!slot)std::abort();patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("QueryActCtxW",query);HOOK("GetModuleHandleExW",module);HOOK("GetModuleFileNameW",filename);
        HOOK("CreateActCtxW",createContext);HOOK("ActivateActCtx",activateContext);HOOK("FindActCtxSectionStringW",findSection);
        HOOK("LoadLibraryW",load);HOOK("DeactivateActCtx",deactivate);HOOK("OutputDebugStringA",diagnostic);
        HOOK("CreateWindowExW",createWindow);HOOK("GetClassInfoExW",classInfo);
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
struct Fixture {
    const API& api;BOOL cleanup,bypass,unavailable;HANDLE context;DWORD last;
    explicit Fixture(const API& a):api(a),cleanup(*a.cleanup),bypass(*a.bypass),unavailable(*a.unavailable),context(*a.context),last(GetLastError()) {active=&api;reset();}
    void reset() {
        events.clear();failure=mutation=stage=activateCalls=0;error=12345;queried=nullptr;created=reinterpret_cast<HANDLE>(UINT_PTR(0x3344));
        activated=sectionFound=apiResult=69;*api.cleanup=*api.bypass=*api.unavailable=FALSE;*api.context=INVALID_HANDLE_VALUE;SetLastError(0x43210);
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0,ULONG_PTR cookie=0) {
        DWORD lastError=GetLastError();out.insert(out.end(),events.begin(),events.end());
        out.push_back({20,result,caught,*api.cleanup,*api.bypass,*api.unavailable,handle(*api.context),lastError});out.push_back({21,INT64(cookie),activateCalls,0,0,0,0,0});
    }
    ~Fixture(){*api.cleanup=cleanup;*api.bypass=bypass;*api.unavailable=unavailable;*api.context=context;SetLastError(last);active=nullptr;}
};
template<class F>static int invoke(F action){try{action();return 0;}catch(int n){return n;}}
static CREATESTRUCTW arguments() {
    CREATESTRUCTW c{};c.dwExStyle=0x87654321;c.style=LONG(0xf1234567u);c.lpszClass=MAKEINTRESOURCEW(17);c.lpszName=MAKEINTRESOURCEW(19);
    c.x=INT_MIN;c.y=INT_MAX;c.cx=-37;c.cy=41;c.hwndParent=reinterpret_cast<HWND>(UINT_PTR(0x4455));c.hMenu=reinterpret_cast<HMENU>(UINT_PTR(0x5566));
    c.hInstance=reinterpret_cast<HINSTANCE>(UINT_PTR(0x6677));c.lpCreateParams=reinterpret_cast<void*>(UINT_PTR(0x7788));return c;
}
static std::vector<Event> initialization(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int initial:{-1,0,0x1122})for(BOOL missing:{FALSE,BOOL(69)})for(int at=0;at<=4;++at)
    for(DWORD code:{0u,1u,2u,3u,4u,1811u,1812u,1813u,1814u,1815u,1816u,UINT_MAX})for(bool hasQueried:{false,true})for(bool createdOK:{false,true})for(BOOL ok:{FALSE,BOOL(69)}) {
        f.reset();*api.context=reinterpret_cast<HANDLE>(INT_PTR(initial));*api.unavailable=missing;stage=at;error=code;
        if(hasQueried)queried=reinterpret_cast<HANDLE>(UINT_PTR(0x8899));if(!createdOK)created=INVALID_HANDLE_VALUE;activated=ok;
        BOOL result=api.initialize();f.save(out,result);
    }
    for(int at:{1,2,3,4,5,6,7,8}) {f.reset();failure=at;BOOL result=17;int caught=invoke([&]{result=api.initialize();});f.save(out,result,caught);}
    return out;
}
static std::vector<Event> activation(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(BOOL cleanup:{FALSE,BOOL(69)})for(BOOL unavailable:{FALSE,BOOL(-7)})for(int initial:{-1,0,0x1122})
    for(DWORD code:{0u,1u,2u,50u,120u,126u,127u,128u,UINT_MAX})for(BOOL ok:{FALSE,BOOL(69)})for(int at:{0,1}) {
        f.reset();*api.context=reinterpret_cast<HANDLE>(INT_PTR(initial));*api.cleanup=cleanup;*api.unavailable=unavailable;error=code;activated=ok;stage=at;
        ULONG_PTR cookie=0x9876;BOOL result=api.activate(&cookie);f.save(out,result,0,cookie);
    }
    return out;
}
static std::vector<Event> wrappers(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int operation:{0,1})for(BOOL bypass:{FALSE,BOOL(69)})for(BOOL missing:{FALSE,BOOL(-7)})for(BOOL ok:{FALSE,BOOL(69)})
    for(BOOL result:{FALSE,BOOL(69),BOOL(-7)})for(int mode:{0,1,2,3})for(int at:{0,10,11,12}) {
        f.reset();*api.bypass=bypass;*api.unavailable=missing;*api.context=reinterpret_cast<HANDLE>(UINT_PTR(0x1122));activated=ok;apiResult=result;mutation=mode;failure=at;
        INT64 returned=17;WNDCLASSEXW info{};info.cbSize=sizeof(info);auto c=arguments();
        int caught=invoke([&]{returned=operation?api.menu(&info):handle(api.create(c));});f.save(out,returned,caught);
    }
    return out;
}
using RawCreate=HWND (WINAPI*)(DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HMENU,HINSTANCE,void*);
static RawCreate nativeCreate;
static BOOL (WINAPI* nativeMenu)(HINSTANCE,LPCWSTR,WNDCLASSEXW*);
static HWND nativeCreateAdapter(CREATESTRUCTW c) {return nativeCreate(c.dwExStyle,c.lpszClass,c.lpszName,c.style,c.x,c.y,c.cx,c.cy,c.hwndParent,c.hMenu,c.hInstance,c.lpCreateParams);}
static BOOL nativeMenuAdapter(WNDCLASSEXW* info){return nativeMenu(nullptr,L"#32768",info);}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected){size_t i=0;while(i<actual.size() && i<expected.size() && actual[i]==expected[i])++i;printf("window activation mismatch %s at %zu/%zu/%zu\n",label,i,actual.size(),expected.size());
        for(size_t j=i>3?i-3:0;j<std::min(i+4,std::max(actual.size(),expected.size()));++j){printf("%zu local:",j);if(j<expected.size())for(auto n:expected[j])printf(" %lld",n);printf(" native:");if(j<actual.size())for(auto n:actual[j])printf(" %lld",n);puts("");}}
    check(actual==expected,label);
}
}
static void windowActivation(HMODULE native) {
    using namespace window_activation_test;auto& state=openmfc::detail::windowActivation;
    API local{&openmfc::detail::InitializeWindowActivation,&openmfc::detail::ActivateWindowContext,&openmfc::detail::ContextCreateWindow,&openmfc::detail::ContextMenuClassInfo,&state.cleanupStarted,&state.bypass,&state.unavailable,&state.context};
    Imports imports(GetModuleHandleW(nullptr));auto init=initialization(local),activate=activation(local),wrapped=wrappers(local);
    {
        Fixture f(local);auto c=arguments();*local.context=nullptr;apiResult=FALSE;
        check(local.create(c)==nullptr && GetLastError()==error && events.back()[0]==8,"failed window creation preserves its Win32 error across activation cleanup");
        f.reset();*local.bypass=*local.unavailable=TRUE;apiResult=-7;WNDCLASSEXW info{};info.cbSize=sizeof(info);
        check(local.menu(&info)==-7 && events.size()==1,"class lookup preserves raw atom results and skips cleanup only when both isolation flags are set");
    }
    if(native) {
        auto* b=reinterpret_cast<BYTE*>(native);Imports oracleImports(native);
        API oracle{reinterpret_cast<decltype(local.initialize)>(b+0x1c294),reinterpret_cast<decltype(local.activate)>(b+0x1c45c),nativeCreateAdapter,nativeMenuAdapter,
            reinterpret_cast<BOOL*>(b+0x3c3a58),reinterpret_cast<BOOL*>(b+0x3c3a68),reinterpret_cast<BOOL*>(b+0x3c3a6c),reinterpret_cast<HANDLE*>(b+0x3b2860)};
        nativeCreate=reinterpret_cast<RawCreate>(b+0x2934d4);nativeMenu=reinterpret_cast<decltype(nativeMenu)>(b+0x2935f0);
        compare(initialization(oracle),init,"activation-context initialization and failure boundaries match native MFC");
        compare(activation(oracle),activate,"activation-context fallback errors, cleanup diagnostics and cookie outputs match native MFC");
        compare(wrappers(oracle),wrapped,"window and menu isolation wrappers match native callback ordering, exceptions and last-error preservation");
    }
}
