#pragma once
#include <intrin.h>
// SDK-sized wrappers around a real HWND graph. Native MFC owns its own permanent
// handle-map entries; only peripheral Win32 focus/dialog results are controlled.
extern "C" {
HWND MS_ABI impl__AfxGetParentOwner__YAPEAUHWND____PEAU1__Z(HWND);
CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd*);
CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd*);
CWnd* MS_ABI impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ(const CWnd*);
CWnd* MS_ABI impl__GetTopLevelOwner_CWnd__QEBAPEAV1_XZ(const CWnd*);
CWnd* MS_ABI impl__GetParentOwner_CWnd__QEBAPEAV1_XZ(const CWnd*);
BOOL MS_ABI impl__IsTopParentActive_CWnd__QEBAHXZ(const CWnd*);
CWnd* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd*);
BOOL MS_ABI impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z(CWnd*,MSG*);
BOOL MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd*,MSG*);
BOOL MS_ABI impl__PreTranslateMessage_CFormView__MEAAHPEAUtagMSG___Z(CFormView*,MSG*);
void MS_ABI impl__OnActivateView_CFormView__MEAAXHPEAVCView__0_Z(CFormView*,BOOL,CView*,CView*);
}
namespace window_form_routing_test {
using Event=std::array<INT64,6>;
struct API {
    CFrameWnd* (MS_ABI* parentFrame)(const CWnd*);
    CFrameWnd* (MS_ABI* topFrame)(const CWnd*);
    CWnd* (MS_ABI* topParent)(const CWnd*);
    CWnd* (MS_ABI* topOwner)(const CWnd*);
    CWnd* (MS_ABI* parentOwner)(const CWnd*);
    BOOL (MS_ABI* active)(const CWnd*);
    CWnd* (MS_ABI* focus)(CWnd*);
    BOOL (MS_ABI* dialog)(CWnd*,MSG*);
    BOOL (MS_ABI* base)(CWnd*,MSG*);
    BOOL (MS_ABI* form)(CFormView*,MSG*);
    void (MS_ABI* activate)(CFormView*,BOOL,CView*,CView*);
    HWND (MS_ABI* parentHandle)(HWND);
    void (MS_ABI* construct)(CWnd*)=nullptr;
    BOOL (MS_ABI* attach)(CWnd*,HWND)=nullptr;
    HWND (MS_ABI* detach)(CWnd*)=nullptr;
    void (MS_ABI* destroy)(CWnd*)=nullptr;
};
static std::vector<Event> events;
static HWND handles[6];static CWnd* windows[6];
static void** nativeModule;
static void* nativeHandleLookupReturn;
static bool permanent[6];
static BOOL frames[6],consume[6],dialogResult,childResult;
static int failure,mutation,parentCalls;
static HWND focused,foreground,popup;
static bool capture;
static void event(INT64 id,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0) {events.push_back({id,a,b,c,d,e});if(failure==id)throw 91;}
static int id(HWND h) {if(!h)return -1;for(int i=0;i<6;++i)if(handles[i]==h)return i;return -2;}
static int id(const CWnd* w) {return w?id(w->m_hWnd):-1;}
static CWnd* lookup(HWND h) {int i=id(h);return i<0?nullptr:windows[i];}
static CWnd* permanentLookup(HWND h) {int i=id(h);return i>=0 && permanent[i]?windows[i]:nullptr;}
static BOOL MS_ABI isFrame(const CWnd* w) {int i=id(w);event(1,i);return frames[i];}
static BOOL MS_ABI translate(CWnd* w,MSG* m) {
    int i=id(w);event(2,i,m?m->message:0,m?INT64(m->wParam):0);
    if(mutation==2 && m)m->message=WM_MOUSEWHEEL;
    if(mutation==3 && i==2)::SetParent(handles[2],handles[1]);
    return consume[i];
}
static void MS_ABI filter(MSG* m,CWnd* w) {event(3,id(w),m?m->message:0);if(mutation==1 && m)m->message=WM_KEYDOWN;}
static void MS_ABI wrongFilter(MSG*,CWnd*) {std::abort();}
static BOOL MS_ABI dialog(void*,CWnd* w,MSG* m) {event(4,id(w),m?m->message:0);return dialogResult;}
static CWnd* MS_ABI siteFocus(void*) {event(5);return windows[1];}
static decltype(&::GetFocus) realGetFocus;
static decltype(&::SetFocus) realSetFocus;
static decltype(&::GetForegroundWindow) realForeground;
static decltype(&::GetLastActivePopup) realPopup;
static decltype(&::IsChild) realChild;
static decltype(&::IsDialogMessageW) realDialog;
static decltype(&::GetParent) realParent;
static HWND WINAPI getFocus() {if(!capture)return realGetFocus();event(6);return focused;}
static HWND WINAPI setFocus(HWND h) {if(!capture)return realSetFocus(h);event(7,id(h));return focused;}
static HWND WINAPI getForeground() {if(!capture)return realForeground();event(8);return foreground;}
static HWND WINAPI getPopup(HWND h) {if(!capture)return realPopup(h);event(9,id(h));return popup;}
static BOOL WINAPI isChild(HWND p,HWND c) {if(!capture)return realChild(p,c);event(10,id(p),id(c));return childResult;}
static BOOL WINAPI isDialog(HWND h,MSG* m) {if(!capture)return realDialog(h,m);event(11,id(h),m?m->message:0);return dialogResult;}
static HWND WINAPI getParent(HWND h) {
    // Native FromHandle also probes for an OLE control site. That independent
    // subsystem is absent from this fixture's handle-map double; prevent it
    // from interpreting the container canaries as live OLE objects.
    if(_ReturnAddress()==nativeHandleLookupReturn)return nullptr;
    if(!capture)return realParent(h);
    ++parentCalls;event(12,id(h));
    // A second lookup observes a hierarchy change. SetFocus must re-read it.
    if(mutation==4 && parentCalls==2)return handles[1];
    if(mutation==4 && parentCalls==3)return handles[0];
    return realParent(h);
}
struct Fixture {
    alignas(8) BYTE bytes[6][480]{};void* tables[6][114]{};void* original[6]{};AFX_MODULE_STATE* originalModule[6]{};
    alignas(8) BYTE container[6][224]{};void* managerTable[11]{};void** manager=managerTable;void* siteTable[47]{};void** site=siteTable;
    void* module[21]{};void* cached[17]{};void* savedNativeManager;
    bool bounds=true;
    AFX_MODULE_STATE* savedModule;CWnd* (*savedLookup)(HWND);CWnd* (*savedPermanentLookup)(HWND);const API& api;
    Fixture(const API& functions):api(functions) {
        capture=false;events.clear();failure=mutation=parentCalls=0;dialogResult=69;childResult=FALSE;
        memset(frames,0,sizeof(frames));memset(consume,0,sizeof(consume));frames[0]=69;frames[2]=-1;
        module[16]=reinterpret_cast<void*>(&wrongFilter);module[20]=&manager;cached[16]=reinterpret_cast<void*>(&filter);
        managerTable[10]=reinterpret_cast<void*>(&dialog);siteTable[46]=reinterpret_cast<void*>(&siteFocus);
        savedModule=g_cmdTestCurrentModule;g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(module);
        savedPermanentLookup=g_messagePermanentWindowLookup;g_messagePermanentWindowLookup=&permanentLookup;
        savedLookup=g_messageWindowLookup;g_messageWindowLookup=&lookup;
        if(nativeModule){savedNativeManager=nativeModule[20];nativeModule[20]=&manager;}
        for(int i=0;i<6;++i) {
            permanent[i]=true;
            HWND parent=i==0||i==1?nullptr:i==2?handles[0]:i==3?handles[2]:handles[3];
            handles[i]=CreateWindowExW(0,L"STATIC",L"routing",i<2?WS_OVERLAPPED:WS_CHILD,0,0,80,60,parent,nullptr,GetModuleHandleW(nullptr),nullptr);
            check(handles[i]!=nullptr,"create routing graph window");windows[i]=reinterpret_cast<CWnd*>(bytes[i]);
            if(api.construct){api.construct(windows[i]);original[i]=*reinterpret_cast<void**>(bytes[i]);originalModule[i]=windows[i]->m_pModuleState;check(api.attach(windows[i],handles[i])!=FALSE,"attach native routing wrapper");}
            windows[i]->m_hWnd=handles[i];windows[i]->m_nFlags=0;windows[i]->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(cached);
            *reinterpret_cast<void***>(bytes[i])=tables[i];tables[i][86]=reinterpret_cast<void*>(&isFrame);tables[i][69]=reinterpret_cast<void*>(&translate);
            *reinterpret_cast<void**>(container[i]+216)=reinterpret_cast<void*>(UINT_PTR(0x123));
            memset(bytes[i]+472,0xa5,8);
        }
        focused=handles[4];foreground=popup=handles[0];capture=true;events.clear();parentCalls=0;
    }
    CFormView* form() {return reinterpret_cast<CFormView*>(windows[3]);}
    void reset() {events.clear();failure=mutation=parentCalls=0;}
    ~Fixture() {
        capture=false;failure=0;
        check(bounds,"routing preserves SDK wrapper boundaries");
        for(int i=5;i>=0;--i) {
            windows[i]->m_pCtrlCont=nullptr;windows[i]->m_pCtrlSite=nullptr;
            if(api.detach){api.detach(windows[i]);windows[i]->m_pModuleState=originalModule[i];*reinterpret_cast<void**>(bytes[i])=original[i];api.destroy(windows[i]);}
            DestroyWindow(handles[i]);windows[i]=nullptr;handles[i]=nullptr;
        }
        if(nativeModule)nativeModule[20]=savedNativeManager;
        g_messagePermanentWindowLookup=savedPermanentLookup;g_messageWindowLookup=savedLookup;g_cmdTestCurrentModule=savedModule;
    }
    void save(std::vector<Event>& output,INT64 result,int caught=0) {
        output.insert(output.end(),events.begin(),events.end());output.push_back({90,result,caught,id(form()->m_hWndFocus),
            *reinterpret_cast<void**>(container[2]+216)==nullptr,*reinterpret_cast<void**>(container[0]+216)==nullptr});
        for(int i=0;i<6;++i)bounds=bounds && *reinterpret_cast<UINT64*>(bytes[i]+472)==0xa5a5a5a5a5a5a5a5ULL;
    }
};
template<class F> static int invoke(F action) {try{action();return 0;}catch(int e){return e==19?-1:e;}catch(CException* e){e->Delete();return -1;}}
static std::vector<Event> traversal(const API& api) {
    std::vector<Event> output;Fixture f(api);
    for(int owner:{-1,0,1})for(int node=0;node<6;++node)for(int operation=0;operation<6;++operation) {
        f.reset();windows[3]->m_hWndOwner=owner<0?nullptr:handles[owner];CWnd* result=nullptr;BOOL active=0;
        int caught=invoke([&]{switch(operation) {
            case 0:result=api.parentFrame(windows[node]);break;case 1:result=api.topFrame(windows[node]);break;
            case 2:result=api.topParent(windows[node]);break;case 3:result=api.topOwner(windows[node]);break;
            case 4:result=api.parentOwner(windows[node]);break;case 5:active=api.active(windows[node]);break;
        }});f.save(output,operation==5?active:id(result),caught);
    }
    windows[3]->m_hWndOwner=nullptr;
    // An owned popup follows owner ancestry, while GetParentOwner stops at it.
    SetWindowLongPtrW(handles[1],GWLP_HWNDPARENT,reinterpret_cast<LONG_PTR>(handles[0]));
    for(auto get:{api.topParent,api.topOwner,api.parentOwner}){f.reset();f.save(output,id(get(windows[1])));}
    for(int node:{1,3}) {
        f.reset();windows[node]->m_hWndOwner=node==3?handles[1]:nullptr;permanent[node]=false;
        if(api.detach){api.detach(windows[node]);windows[node]->m_hWnd=handles[node];}
        f.save(output,id(api.topParent(windows[node])));
        f.reset();f.save(output,id(api.parentHandle(handles[node])));
        if(api.attach){windows[node]->m_hWnd=nullptr;api.attach(windows[node],handles[node]);}
        permanent[node]=true;windows[node]->m_hWndOwner=nullptr;
    }
    for(int node=-1;node<6;++node){f.reset();f.save(output,id(api.parentHandle(node<0?nullptr:handles[node])));}
    for(auto get:{api.topParent,api.topOwner,api.parentOwner}){f.reset();f.save(output,id(get(nullptr)));}
    for(auto get:{api.parentFrame,api.topFrame}){f.reset();f.save(output,id(get(nullptr)));}
    f.reset();f.save(output,0,invoke([&]{api.active(nullptr);}));
    for(int at:{1,8,9,12}){f.reset();failure=at;int result=-7;int caught=invoke([&]{result=api.active(windows[3]);});f.save(output,result,caught);}
    return output;
}
static std::vector<Event> navigation(const API& api) {
    std::vector<Event> output;Fixture f(api);
    for(bool child:{false,true})for(bool ole:{false,true})for(BOOL help:{FALSE,BOOL(69)})for(int consumed:{-1,0,2})
    for(UINT code:{0u,0xffu,0x100u,0x109u,0x10au,0x1ffu,0x200u,0x209u,0x20au,0x20eu,UINT(-1)}) {
        if(child && !handles[5])std::abort(); // child nodes are temporarily hidden from GetWindow by reparenting.
        capture=false;SetParent(handles[4],child?handles[3]:handles[1]);SetParent(handles[5],child?handles[3]:handles[1]);capture=true;
        f.reset();consume[0]=consume[2]=0;if(consumed>=0)consume[consumed]=69;
        *reinterpret_cast<BOOL*>(f.bytes[0]+0x104)=help;windows[3]->m_nFlags=ole?0x100:0;
        MSG msg{};msg.message=code;msg.wParam=0x123456789;
        int result=-7,caught=invoke([&]{result=api.form(f.form(),&msg);});f.save(output,result,caught);
    }
    capture=false;SetParent(handles[4],handles[3]);SetParent(handles[5],handles[3]);capture=true;
    consume[0]=consume[2]=0;*reinterpret_cast<BOOL*>(f.bytes[0]+0x104)=0;windows[3]->m_nFlags=0x100;
    for(int mode:{0,1,2,3})for(int at:{0,1,2,3,4,12}) {
        f.reset();mutation=mode;failure=at;MSG msg{};msg.message=WM_MOUSEWHEEL;
        int result=-7,caught=invoke([&]{result=api.form(f.form(),&msg);});f.save(output,result,caught);
        capture=false;SetParent(handles[2],handles[0]);capture=true;
    }
    for(bool callback:{false,true})for(bool nullMessage:{false,true}) {
        f.reset();f.cached[16]=callback?reinterpret_cast<void*>(&filter):nullptr;MSG msg{};msg.message=WM_KEYDOWN;
        f.save(output,api.base(windows[3],nullMessage?nullptr:&msg));
    }
    for(bool ole:{false,true})for(BOOL result:{FALSE,BOOL(69),BOOL(-1)}) {
        f.reset();windows[3]->m_nFlags=ole?0x100:0;dialogResult=result;MSG msg{};msg.message=WM_KEYDOWN;
        f.save(output,api.dialog(windows[3],&msg));
    }
    return output;
}
static std::vector<Event> focus(const API& api) {
    std::vector<Event> output;Fixture f(api);
    for(bool site:{false,true})for(bool container:{false,true})for(int mode:{0,4})for(int at:{0,5,7,12}) {
        f.reset();mutation=mode;failure=at;windows[3]->m_pCtrlSite=site?reinterpret_cast<COleControlSite*>(&f.site):nullptr;
        for(int i:{0,1,2}){windows[i]->m_pCtrlCont=container?reinterpret_cast<COleControlContainer*>(f.container[i]):nullptr;*reinterpret_cast<void**>(f.container[i]+216)=reinterpret_cast<void*>(UINT_PTR(0x123));}
        CWnd* returned=nullptr;int caught=invoke([&]{returned=api.focus(windows[3]);});f.save(output,id(returned),caught);
    }
    windows[3]->m_pCtrlSite=nullptr;
    for(BOOL activate:{FALSE,BOOL(69),BOOL(-1)})for(bool child:{false,true})for(int focusedID:{-1,1,4})for(bool isActive:{false,true})for(int at:{0,6,7,8,9,10,12}) {
        f.reset();childResult=child?69:0;focused=focusedID<0?nullptr:handles[focusedID];foreground=isActive?handles[0]:handles[1];failure=at;
        f.form()->m_hWndFocus=handles[5];int caught=invoke([&]{api.activate(f.form(),activate,nullptr,nullptr);});f.save(output,0,caught);
    }
    return output;
}
static void expectations(const API& api) {
    Fixture f(api);
    check(id(api.parentFrame(windows[3]))==2 && id(api.topFrame(windows[3]))==0 && id(api.parentFrame(windows[2]))==0,
        "frame traversal honors virtual IsFrameWnd values on SDK-shaped wrappers");
    windows[3]->m_hWndOwner=handles[1];
    check(id(api.topParent(windows[3]))==1 && id(api.topOwner(windows[3]))==3 && id(api.parentOwner(windows[3]))==0,
        "explicit wrapper owners, Win32 owners and child parent ancestry use distinct traversal rules");
    windows[3]->m_hWndOwner=nullptr;MSG message{};message.message=WM_KEYDOWN;
    check(api.form(f.form(),&message)==69 && !events.empty() && events.back()[0]==11,
        "eligible form key messages preserve the dialog navigation result");
    f.reset();message.message=WM_MOUSEWHEEL;
    check(api.form(f.form(),&message)==FALSE && std::none_of(events.begin(),events.end(),[](const Event& e){return e[0]==11;}),
        "form pretranslation excludes mouse wheel messages from dialog navigation");
    f.reset();consume[2]=-1;
    check(api.form(f.form(),&message)==TRUE && events.back()[0]==2 && events.back()[1]==2,
        "the nearest consuming parent frame handles messages before dialog navigation");
    consume[2]=0;f.reset();windows[3]->m_nFlags=0x100;dialogResult=-1;
    check(api.dialog(windows[3],&message)==-1 && events.back()[0]==4,
        "OLE dialog routing uses the current module manager and preserves its BOOL");
    f.reset();mutation=1;message.message=WM_MOUSEWHEEL;
    check(api.base(windows[3],&message)==FALSE && message.message==WM_KEYDOWN && events.size()==1 && events[0][0]==3,
        "base pretranslation calls the cached module tooltip filter and returns FALSE");
    f.reset();windows[3]->m_pCtrlSite=reinterpret_cast<COleControlSite*>(&f.site);
    check(api.focus(windows[3])==windows[1] && events.size()==1 && events[0][0]==5,
        "OLE control-site focus bypasses Win32 focus and parent-container work");
    windows[3]->m_pCtrlSite=nullptr;f.reset();childResult=69;focused=handles[4];
    api.activate(f.form(),FALSE,nullptr,nullptr);
    check(f.form()->m_hWndFocus==handles[4] && events.size()==2 && events[0][0]==6 && events[1][0]==10,
        "deactivating a form still saves its focused child before testing activation");
}
static void compare(const std::vector<Event>& native,const std::vector<Event>& local,const char* label) {
    if(native!=local)for(size_t i=0;i<std::min(native.size(),local.size());++i)if(native[i]!=local[i]) {
        printf("routing mismatch %zu local:",i);for(auto v:local[i])printf(" %lld",v);printf(" native:");for(auto v:native[i])printf(" %lld",v);puts("");break;
    }
    check(native==local,label);
}
struct Imports {
    std::vector<std::unique_ptr<recordset_value_test::Patch>> patches;
    ~Imports() {while(!patches.empty())patches.pop_back();}
    Imports(HMODULE module,bool originals) {
#define HOOK(name, callback, saved) {void** slot=recordset_value_test::import(module,name);check(slot!=nullptr,"locate window-routing import " name);if(!slot)std::abort();if(originals)saved=reinterpret_cast<decltype(saved)>(*slot);patches.emplace_back(new recordset_value_test::Patch(slot,reinterpret_cast<void*>(&callback)));}
        HOOK("GetFocus",getFocus,realGetFocus);HOOK("SetFocus",setFocus,realSetFocus);HOOK("GetForegroundWindow",getForeground,realForeground);
        HOOK("GetLastActivePopup",getPopup,realPopup);HOOK("IsChild",isChild,realChild);HOOK("IsDialogMessageW",isDialog,realDialog);HOOK("GetParent",getParent,realParent);
#undef HOOK
    }
};
}
static void windowFormRouting() {
    using namespace window_form_routing_test;
    API local{&impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ,&impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ,
        &impl__GetTopLevelParent_CWnd__QEBAPEAV1_XZ,&impl__GetTopLevelOwner_CWnd__QEBAPEAV1_XZ,&impl__GetParentOwner_CWnd__QEBAPEAV1_XZ,
        &impl__IsTopParentActive_CWnd__QEBAHXZ,&impl__SetFocus_CWnd__QEAAPEAV1_XZ,&impl__IsDialogMessageW_CWnd__QEAAHPEAUtagMSG___Z,
        &impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z,&impl__PreTranslateMessage_CFormView__MEAAHPEAUtagMSG___Z,&impl__OnActivateView_CFormView__MEAAXHPEAVCView__0_Z,&impl__AfxGetParentOwner__YAPEAUHWND____PEAU1__Z};
    Imports imports(GetModuleHandleW(nullptr),true);
    expectations(local);
    auto parents=traversal(local),messages=navigation(local),activation=focus(local);
    check(!parents.empty() && !messages.empty() && !activation.empty(),"exercise production frame, message and focus routing");
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_ROUTING_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for window/form routing comparisons");
        if(native) {
            auto* b=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(b);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"window-routing oracle uses the verified MFC image build");
            if(pinned) {
                nativeHandleLookupReturn=b+0x2a9da9;nativeModule=reinterpret_cast<void** (MS_ABI*)()>(b+0x133930)();Imports nativeImports(native,false);API api=local;
#define RVA(field, offset) api.field=reinterpret_cast<decltype(api.field)>(b+offset)
                RVA(parentHandle,0x28e260);RVA(parentFrame,0x28e200);RVA(topFrame,0x28e490);RVA(topParent,0x28e2e0);RVA(topOwner,0x28e320);RVA(parentOwner,0x28e370);
                RVA(active,0x28e3d0);RVA(focus,0x2a9b60);RVA(dialog,0x2a9630);RVA(base,0x28bc00);RVA(form,0x27a730);RVA(activate,0x27a610);
                RVA(construct,0x28a700);RVA(attach,0x28adf0);RVA(detach,0x28ae50);RVA(destroy,0x28b740);
#undef RVA
                compare(traversal(api),parents,"virtual frame lookup, explicit owners, popup ancestry and foreground activation match native MFC");
                compare(navigation(api),messages,"tooltip filters, parent-frame callbacks, dialog ranges, help mode and OLE navigation match native MFC");
                compare(focus(api),activation,"control-site focus, repeated parent lookup and form activation ordering match native MFC");
                nativeModule=nullptr;nativeHandleLookupReturn=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
