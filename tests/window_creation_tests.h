#pragma once
#include "detail/WindowCreationSupport.h"
extern "C" {
void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(CWnd*);
BOOL MS_ABI impl__AfxUnhookWindowCreate__YAHXZ();
WNDPROC MS_ABI impl__AfxGetAfxWndProc__YAP6A_JPEAUHWND____I_K_J_ZXZ();
LRESULT MS_ABI impl__AfxWndProc__YA_JPEAUHWND____I_K_J_Z(HWND,UINT,WPARAM,LPARAM);
BOOL MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(CWnd*,DWORD,LPCWSTR,LPCWSTR,DWORD,int,int,int,int,HWND,HMENU,void*);
BOOL MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(CWnd*,DWORD,LPCWSTR,LPCWSTR,DWORD,const RECT&,CWnd*,UINT,void*);
BOOL MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(CWnd*,LPCWSTR,LPCWSTR,DWORD,const RECT&,CWnd*,UINT,CCreateContext*);
}
namespace window_creation_test {
using Event=window_activation_test::Event;
struct API {
    decltype(&impl__AfxHookWindowCreate__YAXPEAVCWnd___Z) hook;
    decltype(&impl__AfxUnhookWindowCreate__YAHXZ) unhook;
    decltype(&impl__AfxGetAfxWndProc__YAP6A_JPEAUHWND____I_K_J_ZXZ) procedure;
    decltype(&impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z) create;
    decltype(&impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z) rect;
    decltype(&impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z) child;
    HOOKPROC cbt;
    WNDPROC framework,activation;
    _AFX_THREAD_STATE* thread;
    BYTE* module;
    BOOL *bypass,*unavailable;
    ATOM* menuAtom;
    void (MS_ABI* construct)(CWnd*)=nullptr;
    HWND (MS_ABI* detach)(CWnd*)=nullptr;
    void (MS_ABI* destroy)(CWnd*)=nullptr;
};
static const API* active;
static CWnd* window;
static HWND hwnd;
static std::vector<Event> events;
static int failure,mutation,propertyCalls,posts,subclasses,dispatches;
static BOOL preResult,installResult,createdResult,fireHook,storeProperty;
static ATOM classResult;
static HANDLE property;
static LONG_PTR previousProcedure;
static WNDPROC savedProcedure;
static HOOKPROC installed;
static bool capture,realCreation;
static void* observedParameter;
static int createMessages;
static LRESULT CALLBACK originalProcedure(HWND h,UINT message,WPARAM w,LPARAM l){return DefWindowProcW(h,message,w,l);}
static DWORD firstStyle,nextStyle;
static int styleCalls,rectCalls;
static BOOL ownerExists,enabled,popupExists,foregroundSame,otherValid;
static bool moved;
static bool frameworkFailure;
static int deletedErrors;
class Failure:public CException {public:Failure():CException(TRUE){}~Failure()override{++deletedErrors;}};
static void* siteTable[44];static void** site=siteTable;
alignas(8) static BYTE alternateModule[0xa8]{};
static INT64 ptr(const void* p){return reinterpret_cast<INT_PTR>(p);}
static INT64 handle(HWND h){return h==hwnd?1:h?2:0;}
static INT64 proc(WNDPROC p){return p==active->framework?1:p==active->activation?2:p?3:0;}
static INT64 pending(){return active->thread->m_pWndInit==window?1:active->thread->m_pWndInit?2:0;}
static void event(INT64 a,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0,INT64 f=0,INT64 g=0,INT64 h=0) {
    events.push_back({a,b,c,d,e,f,g,h});if(failure==a){if(frameworkFailure)throw static_cast<CException*>(new Failure);throw 91;}
}
static BOOL MS_ABI pre(CWnd*,CREATESTRUCTW& c) {
    event(1,c.dwExStyle,c.style,c.x,c.y,c.cx,c.cy,ptr(c.hInstance));event(2,ptr(c.lpszClass),ptr(c.lpszName),ptr(c.hwndParent),ptr(c.hMenu),ptr(c.lpCreateParams));
    if(mutation==1){c.dwExStyle=37;c.style=41;c.x=-43;c.y=47;c.cx=53;c.cy=-59;c.lpszClass=MAKEINTRESOURCEW(61);c.lpszName=MAKEINTRESOURCEW(67);c.hMenu=reinterpret_cast<HMENU>(UINT_PTR(71));c.hwndParent=reinterpret_cast<HWND>(UINT_PTR(73));c.hInstance=reinterpret_cast<HINSTANCE>(UINT_PTR(79));c.lpCreateParams=reinterpret_cast<void*>(UINT_PTR(83));}
    return preResult;
}
static void MS_ABI post(CWnd*) {++posts;event(3,pending(),handle(window->m_hWnd));}
static void MS_ABI subclass(CWnd* self) {
    ++subclasses;event(4,self==window,handle(self->m_hWnd),active->thread->m_pModuleState==self->m_pModuleState);
    if(mutation==2)self->m_hWnd=reinterpret_cast<HWND>(UINT_PTR(0x1234));
    if(mutation==3)*reinterpret_cast<WNDPROC*>(active->module+0x70)=reinterpret_cast<WNDPROC>(UINT_PTR(0x2234));
}
static WNDPROC* MS_ABI super(CWnd*) {event(5);if(mutation==4)*reinterpret_cast<WNDPROC*>(active->module+0x70)=reinterpret_cast<WNDPROC>(UINT_PTR(0x2234));return &savedProcedure;}
static DWORD MS_ABI siteStyle(void*) {event(49);return styleCalls++?nextStyle:firstStyle;}
static BOOL MS_ABI siteEnabled(void*) {event(50);return enabled;}
static BOOL MS_ABI noCenter(CWnd*) {event(6);return FALSE;}
static LRESULT MS_ABI dispatch(CWnd* self,UINT message,WPARAM w,LPARAM l) {
    ++dispatches;event(7,message,w,l);
    if(realCreation) {
        if(message==WM_NCCREATE || message==WM_CREATE){observedParameter=reinterpret_cast<CREATESTRUCTW*>(l)->lpCreateParams;++createMessages;}
        return CallWindowProcW(savedProcedure,self->m_hWnd,message,w,l);
    }
    return 0x123456789LL;
}
static BOOL MS_ABI numeric(CWnd*,DWORD ex,LPCWSTR cls,LPCWSTR title,DWORD style,int x,int y,int cx,int cy,HWND parent,HMENU menu,void* param) {
    event(8,ex,ptr(cls),ptr(title),style,x,y,cx);event(9,cy,ptr(parent),ptr(menu),ptr(param));return -7;
}
static decltype(&::GetWindowLongW) realStyle;
static decltype(&::GetWindowRect) realRect;
static decltype(&::GetWindow) realWindow;
static decltype(&::IsWindowEnabled) realEnabled;
static decltype(&::SendMessageW) realSend;
static decltype(&::GetLastActivePopup) realPopup;
static decltype(&::GetForegroundWindow) realForeground;
static decltype(&::SetForegroundWindow) realSetForeground;
static decltype(&::IsWindow) realIsWindow;
static LONG WINAPI style(HWND h,int index) {if(!capture)return realStyle(h,index);event(40,handle(h),index);return LONG(styleCalls++?nextStyle:firstStyle);}
static BOOL WINAPI rectangle(HWND h,RECT* rect) {if(!capture)return realRect(h,rect);event(41,handle(h));*rect={7,11,47,61};if(rectCalls++ && moved)++rect->left;return FALSE;}
static HWND WINAPI owner(HWND h,UINT which) {if(!capture)return realWindow(h,which);event(42,handle(h),which);return ownerExists?hwnd:nullptr;}
static BOOL WINAPI isEnabled(HWND h) {if(!capture)return realEnabled(h);event(43,handle(h));return enabled;}
static LRESULT WINAPI send(HWND h,UINT message,WPARAM w,LPARAM l) {if(!capture)return realSend(h,message,w,l);auto* pair=reinterpret_cast<HWND*>(l);event(44,handle(h),message,w,handle(pair[0]),handle(pair[1]));return 0;}
static HWND WINAPI popup(HWND h) {if(!capture)return realPopup(h);event(45,handle(h));return popupExists?hwnd:nullptr;}
static HWND WINAPI foreground() {if(!capture)return realForeground();event(46);return foregroundSame?hwnd:nullptr;}
static BOOL WINAPI setForeground(HWND h) {if(!capture)return realSetForeground(h);event(47,handle(h));return FALSE;}
static BOOL WINAPI isWindow(HWND h) {if(!capture)return realIsWindow(h);event(48,handle(h));return otherValid;}
static decltype(&::SetWindowLongPtrW) realSet;
static decltype(&::GetWindowLongPtrW) realGet;
static decltype(&::CallWindowProcW) realCall;
static decltype(&::DefWindowProcW) realDefault;
static HHOOK WINAPI install(int id,HOOKPROC callback,HINSTANCE instance,DWORD thread) {
    event(10,id,callback==active->cbt,instance==nullptr,thread==GetCurrentThreadId());installed=callback;
    return installResult?reinterpret_cast<HHOOK>(UINT_PTR(0x3344)):nullptr;
}
static LRESULT WINAPI next(HHOOK hook,int code,WPARAM w,LPARAM l) {
    event(11,ptr(hook),code,handle(reinterpret_cast<HWND>(w)),l?1:0,pending(),active->thread->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(active->module));return -0x123456789LL;
}
static LONG_PTR WINAPI set(HWND h,int index,LONG_PTR value) {
    if(!capture)return realSet(h,index,value);event(12,handle(h),index,proc(reinterpret_cast<WNDPROC>(value)),pending());
    if(mutation==5)active->thread->m_hHookOldCbtFilter=reinterpret_cast<HHOOK>(UINT_PTR(0x4455));return previousProcedure;
}
static LONG_PTR WINAPI get(HWND h,int index) {if(!capture)return realGet(h,index);event(13,handle(h),index);return previousProcedure;}
static HANDLE WINAPI getProperty(HWND h,LPCWSTR key) {event(14,handle(h),wcscmp(key,L"AfxOldWndProc423")==0);++propertyCalls;return property;}
static BOOL WINAPI setProperty(HWND h,LPCWSTR key,HANDLE value) {event(15,handle(h),wcscmp(key,L"AfxOldWndProc423")==0,proc(reinterpret_cast<WNDPROC>(value)));if(storeProperty)property=value;return FALSE;}
static ATOM WINAPI addAtom(LPCWSTR key) {event(16,wcscmp(key,L"AfxOldWndProc423")==0);return 0x5566;}
static BOOL WINAPI classInfo(HINSTANCE instance,LPCWSTR name,WNDCLASSEXW* info) {event(17,ptr(instance),wcscmp(name,L"#32768")==0,info->cbSize);return classResult;}
static int WINAPI className(HWND h,LPWSTR name,int capacity) {event(18,handle(h),capacity);wcscpy_s(name,capacity,mutation==6?L"#32768":L"Ordinary");return mutation==7?0:8;}
static ULONG_PTR WINAPI classLong(HWND h,int index) {event(19,handle(h),index);return mutation==8?0x6677:0x7788;}
static HWND WINAPI create(DWORD ex,LPCWSTR cls,LPCWSTR title,DWORD style,int x,int y,int cx,int cy,HWND parent,HMENU menu,HINSTANCE instance,void* param) {
    event(20,ex,ptr(cls),ptr(title),style,x,y,cx);event(21,cy,ptr(parent),ptr(menu),ptr(instance),ptr(param),pending());
    if(fireHook){CREATESTRUCTW c{};c.style=style;CBT_CREATEWNDW cbt{&c,nullptr};active->cbt(HCBT_CREATEWND,reinterpret_cast<WPARAM>(hwnd),reinterpret_cast<LPARAM>(&cbt));}
    return createdResult?hwnd:nullptr;
}
static BOOL WINAPI deactivate(DWORD,ULONG_PTR){event(22);return FALSE;}
static LRESULT WINAPI defaultProc(HWND h,UINT message,WPARAM w,LPARAM l) {if(!capture)return realDefault(h,message,w,l);event(23,handle(h),message,w,l);return -0x123456789LL;}
static LRESULT WINAPI call(WNDPROC p,HWND h,UINT message,WPARAM w,LPARAM l) {if(!capture)return realCall(p,h,message,w,l);event(24,proc(p),handle(h),message,w,l);return 0x123456789LL;}
static HANDLE WINAPI removeProperty(HWND h,LPCWSTR key){event(25,handle(h),wcscmp(key,L"AfxOldWndProc423")==0);return nullptr;}
static ATOM WINAPI findAtom(LPCWSTR key){event(26,wcscmp(key,L"AfxOldWndProc423")==0);return 0x5566;}
static ATOM WINAPI deleteAtom(ATOM atom){event(27,atom);return 0;}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    Imports(HMODULE image,bool originals) {
#define HOOK(name,callback) {auto** slot=recordset_value_test::import(image,name);check(slot!=nullptr,"locate window creation import " name);if(!slot)std::abort();patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
#define SAVE(name,saved) if(originals)saved=reinterpret_cast<decltype(saved)>(*recordset_value_test::import(image,name));
        SAVE("GetWindowLongW",realStyle);SAVE("GetWindowRect",realRect);SAVE("GetWindow",realWindow);SAVE("IsWindowEnabled",realEnabled);SAVE("SendMessageW",realSend);SAVE("GetLastActivePopup",realPopup);SAVE("GetForegroundWindow",realForeground);SAVE("SetForegroundWindow",realSetForeground);SAVE("IsWindow",realIsWindow);
        HOOK("GetWindowLongW",style);HOOK("GetWindowRect",rectangle);HOOK("GetWindow",owner);HOOK("IsWindowEnabled",isEnabled);HOOK("SendMessageW",send);HOOK("GetLastActivePopup",popup);HOOK("GetForegroundWindow",foreground);HOOK("SetForegroundWindow",setForeground);HOOK("IsWindow",isWindow);
        SAVE("SetWindowLongPtrW",realSet);SAVE("GetWindowLongPtrW",realGet);SAVE("DefWindowProcW",realDefault);SAVE("CallWindowProcW",realCall);
        HOOK("SetWindowsHookExW",install);HOOK("CallNextHookEx",next);HOOK("SetWindowLongPtrW",set);HOOK("GetWindowLongPtrW",get);
        HOOK("GetPropW",getProperty);HOOK("SetPropW",setProperty);HOOK("GlobalAddAtomW",addAtom);HOOK("GetClassInfoExW",classInfo);
        HOOK("GetClassNameW",className);HOOK("GetClassLongPtrW",classLong);HOOK("CreateWindowExW",create);HOOK("DeactivateActCtx",deactivate);
        HOOK("DefWindowProcW",defaultProc);HOOK("CallWindowProcW",call);HOOK("RemovePropW",removeProperty);HOOK("GlobalFindAtomW",findAtom);HOOK("GlobalDeleteAtom",deleteAtom);
#undef SAVE
#undef HOOK
    }
    ~Imports(){while(!patches.empty())patches.pop_back();}
};
static CWnd* lookup(HWND h){return window && window->m_hWnd==h?window:nullptr;}
static CWnd* permanent(HWND h){return realCreation?lookup(h):h==hwnd?window:nullptr;}
struct Fixture {
    const API& api;alignas(8) BYTE bytes[240]{};void* table[91]{};void* original;
    AFX_MODULE_STATE* originalWindowModule;
    CWnd* oldPending;HHOOK oldHook;AFX_MODULE_STATE* oldModule;BOOL oldBypass,oldUnavailable;ATOM oldAtom;
    WNDPROC oldProcedure;HINSTANCE oldInstance;BYTE oldDll;
    CWnd* (*oldLookup)(HWND);CWnd* (*oldPermanent)(HWND);
    explicit Fixture(const API& a):api(a) {
        active=&api;capture=false;window=reinterpret_cast<CWnd*>(bytes);
        oldPending=api.thread->m_pWndInit;oldHook=api.thread->m_hHookOldCbtFilter;oldModule=api.thread->m_pModuleState;
        oldBypass=*api.bypass;oldUnavailable=*api.unavailable;oldAtom=*api.menuAtom;
        oldProcedure=*reinterpret_cast<WNDPROC*>(api.module+0x70);oldInstance=*reinterpret_cast<HINSTANCE*>(api.module+0x10);oldDll=api.module[0x28];
        if(api.construct)api.construct(window);original=*reinterpret_cast<void**>(window);originalWindowModule=window->m_pModuleState;
        if(original)memcpy(table,original,sizeof(table));*reinterpret_cast<void***>(window)=table;
        table[22]=reinterpret_cast<void*>(&subclass);table[27]=reinterpret_cast<void*>(&pre);table[65]=reinterpret_cast<void*>(&super);table[71]=reinterpret_cast<void*>(&dispatch);table[74]=reinterpret_cast<void*>(&post);table[85]=reinterpret_cast<void*>(&noCenter);
        oldLookup=g_messageWindowLookup;oldPermanent=g_messagePermanentWindowLookup;g_messageWindowLookup=lookup;g_messagePermanentWindowLookup=permanent;
        reset();
    }
    void detach() {
        capture=false;api.thread->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(api.module);
        if(api.detach && window->m_hWnd){window->m_hWnd=hwnd;api.detach(window);}else window->m_hWnd=nullptr;
    }
    void reset() {
        detach();events.clear();failure=mutation=propertyCalls=posts=subclasses=dispatches=0;frameworkFailure=realCreation=false;preResult=installResult=createdResult=storeProperty=69;fireHook=FALSE;
        classResult=0;property=nullptr;previousProcedure=0x3377;savedProcedure=reinterpret_cast<WNDPROC>(UINT_PTR(0x4488));installed=nullptr;
        *api.bypass=*api.unavailable=TRUE;*api.menuAtom=0;*reinterpret_cast<WNDPROC*>(api.module+0x70)=api.framework;
        *reinterpret_cast<HINSTANCE*>(api.module+0x10)=reinterpret_cast<HINSTANCE>(UINT_PTR(0x1122));api.module[0x28]=0;
        api.thread->m_hHookOldCbtFilter=nullptr;api.thread->m_pWndInit=nullptr;api.thread->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(api.module);
        window->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(api.module);window->m_pCtrlSite=nullptr;
        firstStyle=nextStyle=0;styleCalls=rectCalls=0;ownerExists=foregroundSame=FALSE;enabled=popupExists=otherValid=69;moved=false;
        siteTable[32]=reinterpret_cast<void*>(&siteStyle);siteTable[43]=reinterpret_cast<void*>(&siteEnabled);capture=true;
    }
    void bind() {
        api.thread->m_pWndInit=window;CREATESTRUCTW c{};CBT_CREATEWNDW d{&c,nullptr};api.cbt(HCBT_CREATEWND,reinterpret_cast<WPARAM>(hwnd),reinterpret_cast<LPARAM>(&d));
        events.clear();posts=subclasses=dispatches=styleCalls=rectCalls=0;
    }
    void save(std::vector<Event>& out,INT64 result,int caught=0) {
        out.insert(out.end(),events.begin(),events.end());out.push_back({30,result,caught,pending(),ptr(api.thread->m_hHookOldCbtFilter),posts,subclasses,handle(window->m_hWnd)});
        out.push_back({31,proc(savedProcedure),api.thread->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(api.module),*api.menuAtom,propertyCalls,dispatches,0,0});
    }
    ~Fixture(){detach();*reinterpret_cast<void**>(window)=original;window->m_pModuleState=originalWindowModule;if(api.destroy)api.destroy(window);
        *api.bypass=oldBypass;*api.unavailable=oldUnavailable;*api.menuAtom=oldAtom;*reinterpret_cast<WNDPROC*>(api.module+0x70)=oldProcedure;*reinterpret_cast<HINSTANCE*>(api.module+0x10)=oldInstance;api.module[0x28]=oldDll;
        api.thread->m_pWndInit=oldPending;api.thread->m_hHookOldCbtFilter=oldHook;api.thread->m_pModuleState=oldModule;
        g_messageWindowLookup=oldLookup;g_messagePermanentWindowLookup=oldPermanent;window=nullptr;active=nullptr;}
};
template<class F>static int invoke(F action){try{action();return 0;}catch(int n){return n;}catch(const std::bad_alloc&){return 92;}catch(CException* e){e->Delete();return 92;}}
static std::vector<Event> hooks(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(bool existing:{false,true})for(int pending=0;pending<3;++pending)for(int request=0;request<3;++request)for(BOOL succeeds:{FALSE,BOOL(69)}) {
        f.reset();if(existing)api.thread->m_hHookOldCbtFilter=reinterpret_cast<HHOOK>(UINT_PTR(0x3344));
        CWnd* values[]={nullptr,window,reinterpret_cast<CWnd*>(UINT_PTR(0x5566))};api.thread->m_pWndInit=values[pending];installResult=succeeds;
        int caught=invoke([&]{api.hook(values[request]);});f.save(out,0,caught);f.save(out,api.unhook());f.save(out,api.unhook());
    }
    return out;
}
static std::vector<Event> creation(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(BOOL accepted:{FALSE,BOOL(69),BOOL(-7)})for(BOOL created:{FALSE,BOOL(69)})for(BOOL hook:{FALSE,BOOL(69)})for(BOOL install:{FALSE,BOOL(69)})for(int mode:{0,1})for(int at:{0,1,2,3,4,5,10,11,12,20,21}) {
        f.reset();preResult=accepted;createdResult=created;fireHook=hook;installResult=install;mutation=mode;failure=at;BOOL result=17;
        int caught=invoke([&]{result=api.create(window,0x87654321,MAKEINTRESOURCEW(17),MAKEINTRESOURCEW(19),0x81234567,INT_MIN,INT_MAX,-37,41,reinterpret_cast<HWND>(UINT_PTR(0x4455)),reinterpret_cast<HMENU>(UINT_PTR(0x5566)),reinterpret_cast<void*>(UINT_PTR(0x6677)));});f.save(out,result,caught);
    }
    for(int operation:{0,1})for(bool parent:{false,true})for(UINT id:{0u,0x81234567u,UINT_MAX}) {
        f.reset();f.table[25]=reinterpret_cast<void*>(&numeric);RECT rect{INT_MIN,INT_MAX,INT_MAX,INT_MIN};alignas(8) BYTE pbytes[232]{};auto* p=reinterpret_cast<CWnd*>(pbytes);p->m_hWnd=reinterpret_cast<HWND>(UINT_PTR(0x4455));
        BOOL result=operation?api.child(window,MAKEINTRESOURCEW(17),MAKEINTRESOURCEW(19),0x81234567,rect,parent?p:nullptr,id,reinterpret_cast<CCreateContext*>(UINT_PTR(0x6677))):api.rect(window,0x87654321,MAKEINTRESOURCEW(17),MAKEINTRESOURCEW(19),0x81234567,rect,parent?p:nullptr,id,reinterpret_cast<void*>(UINT_PTR(0x6677)));f.save(out,result);
    }
    return out;
}
static std::vector<Event> callbacks(const API& api) {
    Fixture f(api);std::vector<Event> out;CREATESTRUCTW c{};CBT_CREATEWNDW data{&c,nullptr};
    for(bool pending:{false,true})for(int mode=0;mode<=9;++mode)for(int at:{0,4,5,11,12,13,14,15,16,17,18,19})for(LONG style:{LONG(0),LONG(WS_CHILD)})for(int dll:{0,1})for(int cached:{0,0x6677}) {
        f.reset();api.thread->m_hHookOldCbtFilter=reinterpret_cast<HHOOK>(UINT_PTR(0x3344));if(pending)api.thread->m_pWndInit=window;
        mutation=mode;failure=at;c.style=style;if(mode==9)api.thread->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(alternateModule);api.module[0x28]=BYTE(dll);*api.menuAtom=ATOM(cached);classResult=mode==8?0x6677:0;
        if(mode==1)property=reinterpret_cast<HANDLE>(UINT_PTR(0x4488));if(mode==2)previousProcedure=0;if(mode==3)storeProperty=FALSE;
        if(mode==4)previousProcedure=reinterpret_cast<LONG_PTR>(api.framework);
        LRESULT result=17;int caught=invoke([&]{result=api.cbt(HCBT_CREATEWND,reinterpret_cast<WPARAM>(hwnd),reinterpret_cast<LPARAM>(&data));});f.save(out,result,caught);
    }
    for(int code:{-1,0,1,2,4,INT_MIN,INT_MAX}) {f.reset();f.save(out,api.cbt(code,reinterpret_cast<WPARAM>(hwnd),0));}
    return out;
}
static std::vector<Event> activationMessages(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(int operation:{0,1})for(DWORD original:{DWORD(0),DWORD(WS_VISIBLE),DWORD(WS_CHILD)})for(DWORD later:{DWORD(0),DWORD(WS_VISIBLE),DWORD(WS_CHILD)})for(bool move:{false,true})for(BOOL hasOwner:{FALSE,BOOL(69)})for(BOOL ownerEnabled:{FALSE,BOOL(-7)})for(bool ole:{false,true}) {
        f.reset();f.bind();firstStyle=original;nextStyle=later;moved=move;ownerExists=hasOwner;enabled=ownerEnabled;
        if(ole)window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);
        LRESULT result=operation?api.framework(hwnd,WM_INITDIALOG,0x987654321ULL,-0x123456789LL):api.activation(hwnd,WM_INITDIALOG,0x987654321ULL,-0x123456789LL);f.save(out,result);
    }
    for(DWORD style:{DWORD(0),DWORD(WS_CHILD)})for(bool other:{false,true})for(BOOL valid:{FALSE,BOOL(69)}) {
        f.reset();f.bind();firstStyle=nextStyle=style;otherValid=valid;
        f.save(out,api.activation(hwnd,WM_ACTIVATE,0x987654321ULL,other?reinterpret_cast<LPARAM>(hwnd):0));
    }
    for(int hit:{HTERROR,HTCLIENT,HTNOWHERE})for(UINT message:{UINT(WM_LBUTTONDOWN),UINT(WM_RBUTTONDOWN),UINT(WM_MBUTTONDOWN),UINT(WM_MOUSEMOVE),UINT(WM_LBUTTONUP)})for(BOOL exists:{FALSE,BOOL(69)})for(BOOL foreground:{FALSE,BOOL(69)})for(BOOL on:{FALSE,BOOL(-7)})for(bool ole:{false,true}) {
        f.reset();f.bind();popupExists=exists;foregroundSame=foreground;enabled=on;if(ole)window->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&site);
        f.save(out,api.activation(hwnd,WM_SETCURSOR,0x987654321ULL,MAKELPARAM(hit,message)));
    }
    for(UINT message:{UINT(WM_INITDIALOG),UINT(WM_ACTIVATE),UINT(WM_SETCURSOR)})for(int at:{6,7,14,24,40,41,42,43,44,45,46,47,48,49,50}) {
        f.reset();f.bind();failure=at;LRESULT result=17;
        int caught=invoke([&]{result=api.activation(hwnd,message,0x987654321ULL,message==WM_SETCURSOR?MAKELPARAM(HTERROR,WM_LBUTTONDOWN):0);});f.save(out,result,caught);
    }
    return out;
}
static LRESULT processException(CException*,const MSG* record) {
    event(60,handle(record->hwnd),record->message,record->wParam,record->lParam);return -0x3456789LL;
}
static void expectations(const API& api) {
    Fixture f(api);preResult=FALSE;
    check(api.create(window,0,MAKEINTRESOURCEW(17),MAKEINTRESOURCEW(19),0,0,0,1,1,nullptr,nullptr,nullptr)==FALSE && posts==1 && !installed && !window->m_hWnd,
        "rejected PreCreateWindow calls PostNcDestroy without installing a hook or creating a window");
    f.reset();createdResult=FALSE;
    check(api.create(window,0,MAKEINTRESOURCEW(17),MAKEINTRESOURCEW(19),0,0,0,1,1,nullptr,nullptr,nullptr)==FALSE && posts==1 && !api.thread->m_pWndInit && api.thread->m_hHookOldCbtFilter,
        "failed unattached creation clears the pending object, calls PostNcDestroy and retains the thread hook");
    f.reset();fireHook=TRUE;
    check(api.create(window,0,MAKEINTRESOURCEW(17),MAKEINTRESOURCEW(19),0,0,0,1,1,nullptr,nullptr,nullptr)==TRUE && !posts && subclasses==1 && !api.thread->m_pWndInit && window->m_hWnd==hwnd,
        "CBT attachment clears the pending object before successful CreateEx returns");
    f.reset();api.hook(window);auto existing=api.thread->m_hHookOldCbtFilter;events.clear();api.hook(window);
    check(events.empty() && api.unhook()==FALSE && api.unhook()==TRUE && api.thread->m_hHookOldCbtFilter==existing,
        "creation hooks are reused and unhook reports whether the pending object was already attached");
}
static void exceptions(const API& api) {
    Fixture f(api);auto previous=g_wndExceptionHandler;g_wndExceptionHandler=&processException;
    for(int at:{14,24}) {
        f.reset();f.bind();frameworkFailure=true;failure=at;int before=deletedErrors;LRESULT result=17;
        int caught=invoke([&]{result=api.activation(hwnd,WM_APP,0x987654321ULL,-0x123456789LL);});
        check(deletedErrors==before+1 && (at==14?caught==92 && result==17:caught==0 && result==-0x3456789LL && events.back()[0]==60 && events.back()[2]==WM_APP && events.back()[3]==0x987654321LL && events.back()[4]==-0x123456789LL),
            at==14?"activation property lookup failures escape the framework exception boundary":"activation procedure translates and deletes framework errors with the original message arguments");
    }
    g_wndExceptionHandler=previous;
}
static void realWindows(const API& api) {
    Fixture f(api);capture=false;
    WNDCLASSW cls{};cls.lpfnWndProc=&originalProcedure;cls.hInstance=GetModuleHandleW(nullptr);cls.lpszClassName=L"OpenMFC_Creation_Lifecycle_Test";
    ATOM atom=RegisterClassW(&cls);check(atom!=0,"register a real superclass for creation lifecycle tests");
    *reinterpret_cast<HINSTANCE*>(api.module+0x10)=cls.hInstance;realCreation=true;observedParameter=nullptr;createMessages=0;
    auto* marker=reinterpret_cast<void*>(UINT_PTR(0x123456789ULL));HWND fixture=hwnd;
    BOOL result=api.create(window,0,cls.lpszClassName,L"actual creation",WS_POPUP,17,19,101,103,nullptr,nullptr,marker);
    HWND created=window->m_hWnd;
    check(result==TRUE && created && IsWindow(created) && observedParameter==marker && createMessages==2 && subclasses==1 && !posts && !api.thread->m_pWndInit,
        "real CBT creation attaches before messages and preserves caller data in WM_NCCREATE and WM_CREATE");
    check(savedProcedure==&originalProcedure && reinterpret_cast<WNDPROC>(GetWindowLongPtrW(created,GWLP_WNDPROC))==api.framework,
        "real creation saves the superclass procedure and installs the module's framework procedure");
    if(created) {
        hwnd=created;f.detach();SetWindowLongPtrW(created,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&originalProcedure));DestroyWindow(created);hwnd=fixture;
    }
    if(api.thread->m_hHookOldCbtFilter){UnhookWindowsHookEx(api.thread->m_hHookOldCbtFilter);api.thread->m_hHookOldCbtFilter=nullptr;}
    realCreation=false;UnregisterClassW(cls.lpszClassName,cls.hInstance);
}
static std::vector<Event> procedures(const API& api) {
    Fixture f(api);std::vector<Event> out;
    for(UINT message:{UINT(0x360),UINT(WM_NCCREATE),UINT(WM_NCDESTROY),UINT(WM_APP),UINT_MAX})for(int binding:{0,1,2}) {
        f.reset();if(binding){capture=false;if(api.construct){api.thread->m_pWndInit=window;CREATESTRUCTW c{};CBT_CREATEWNDW d{&c,nullptr};capture=true;api.cbt(HCBT_CREATEWND,reinterpret_cast<WPARAM>(hwnd),reinterpret_cast<LPARAM>(&d));}else window->m_hWnd=hwnd;}
        if(binding==2)window->m_hWnd=reinterpret_cast<HWND>(UINT_PTR(0x1111));events.clear();posts=subclasses=dispatches=0;capture=true;
        f.save(out,api.framework(hwnd,message,0x987654321ULL,-0x123456789LL));
    }
    for(UINT message:{UINT(WM_APP),UINT(WM_NCDESTROY),UINT(WM_SETCURSOR)}) {
        f.reset();property=reinterpret_cast<HANDLE>(UINT_PTR(0x3377));window->m_hWnd=hwnd;
        // Non-HTERROR cursor messages never need parent activation state.
        f.save(out,api.activation(hwnd,message,0x987654321ULL,MAKELPARAM(HTCLIENT,WM_MOUSEMOVE)));
    }
    return out;
}
}
static void windowCreation(HMODULE native) {
    using namespace window_creation_test;alignas(8) BYTE module[0xa8]{};auto& state=openmfc::detail::windowActivation;
    API local{&impl__AfxHookWindowCreate__YAXPEAVCWnd___Z,&impl__AfxUnhookWindowCreate__YAHXZ,&impl__AfxGetAfxWndProc__YAP6A_JPEAUHWND____I_K_J_ZXZ,&impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z,&impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z,&impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z,
        &openmfc::detail::WindowCreationHook,&impl__AfxWndProc__YA_JPEAUHWND____I_K_J_Z,&openmfc::detail::ActivationWindowProcedure,AfxGetThreadState(),module,&state.bypass,&state.unavailable,&openmfc::detail::menuWindowAtom};
    hwnd=CreateWindowExW(0,L"STATIC",L"creation fixture",WS_POPUP,0,0,80,60,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);check(hwnd!=nullptr,"create a real HWND for native creation attachment");
    API oracle=local;bool oracleReady=false;
    {
        Imports imports(GetModuleHandleW(nullptr),true);expectations(local);exceptions(local);auto hookResults=hooks(local),createResults=creation(local),callbackResults=callbacks(local),procedureResults=procedures(local),activationResults=activationMessages(local);
        {Fixture f(local);check(local.procedure()==local.framework && &AfxWndProc==local.framework,"module and exported framework procedures have the same callable address");}
        if(native) {
            auto* b=reinterpret_cast<BYTE*>(native);oracleReady=true;
#define RVA(field,offset) oracle.field=reinterpret_cast<decltype(oracle.field)>(b+offset)
            RVA(hook,0x28b380);RVA(unhook,0x28b400);RVA(procedure,0x28af60);RVA(create,0x28b500);RVA(rect,0x28b440);RVA(child,0x28b690);RVA(cbt,0x28b130);RVA(framework,0x28aef0);RVA(activation,0x28af80);RVA(construct,0x28a700);RVA(detach,0x28ae50);RVA(destroy,0x28b740);
#undef RVA
            oracle.thread=reinterpret_cast<_AFX_THREAD_STATE*(*)()>(b+0x1332a0)();oracle.module=reinterpret_cast<BYTE*(*)()>(b+0x133930)();oracle.bypass=reinterpret_cast<BOOL*>(b+0x3c3a68);oracle.unavailable=reinterpret_cast<BOOL*>(b+0x3c3a6c);oracle.menuAtom=reinterpret_cast<ATOM*>(b+0x3c40a0);
            Imports nativeImports(native,false);
            window_activation_test::compare(hooks(oracle),hookResults,"creation hook reuse, pending-window replacement, failures and unhook results match native MFC");
            window_activation_test::compare(creation(oracle),createResults,"window creation virtual dispatch, caller parameters, attachment, failures and rectangle overloads match native MFC");
            window_activation_test::compare(callbacks(oracle),callbackResults,"CBT pending attachment, superclass capture, menu detection and activation subclassing match native MFC");
            window_activation_test::compare(procedures(oracle),procedureResults,"framework dispatch and activation subclass teardown match native MFC");
            window_activation_test::compare(activationMessages(oracle),activationResults,"dialog initialization, OLE site styles, activation notifications and cursor foreground handling match native MFC");
        }
    }
    realWindows(local);if(oracleReady)realWindows(oracle);DestroyWindow(hwnd);hwnd=nullptr;
}
