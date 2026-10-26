#pragma once
extern "C" {
void MS_ABI impl___1CFormView__UEAA_XZ(CFormView*) noexcept;
const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ();
BOOL MS_ABI impl__SaveFocusControl_CFormView__IEAAHXZ(CFormView*);
void MS_ABI impl__OnSetFocus_CFormView__IEAAXPEAVCWnd___Z(CFormView*,CWnd*);
void MS_ABI impl__OnActivateFrame_CFormView__MEAAXIPEAVCFrameWnd___Z(CFormView*,UINT,CFrameWnd*);
BOOL MS_ABI impl__SetOccDialogInfo_CFormView__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z(CFormView*,_AFX_OCC_DIALOG_INFO*);
_AFX_OCC_DIALOG_INFO* MS_ABI impl__GetOccDialogInfo_CFormView__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ(CFormView*);
LRESULT MS_ABI impl__HandleInitDialog_CFormView__IEAA_J_K_J_Z(CFormView*,WPARAM,LPARAM);
int MS_ABI impl__OnCreate_CFormView__IEAAHPEAUtagCREATESTRUCTW___Z(CFormView*,CREATESTRUCTW*);
LRESULT MS_ABI impl__OnPrintClient_CFormView__IEAA_JPEAVCDC__I_Z(CFormView*,CDC*,UINT);
}
namespace formview_dialog_test {
using Event=std::array<INT64,6>;
struct API {
    decltype(&impl__SaveFocusControl_CFormView__IEAAHXZ) save;
    decltype(&impl__OnSetFocus_CFormView__IEAAXPEAVCWnd___Z) focus;
    decltype(&impl__OnActivateFrame_CFormView__MEAAXIPEAVCFrameWnd___Z) activate;
    decltype(&impl__SetOccDialogInfo_CFormView__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z) set;
    decltype(&impl__GetOccDialogInfo_CFormView__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ) get;
    decltype(&impl__HandleInitDialog_CFormView__IEAA_J_K_J_Z) init;
    const AFX_MSGMAP* (MS_ABI* map)();
    void (MS_ABI* destroy)(CFormView*);
};
static std::vector<Event> events;
static CFormView* active;
static void** nativeModule;
static _AFX_THREAD_STATE* nativeThread;
static void (MS_ABI* nativeFormConstructor)(CFormView*,UINT);
static _AFX_OCC_DIALOG_INFO* lastPre;
static void* released;
static int failure,mutation;
static BOOL controlResult;
static LRESULT defaultResult;
static bool preResult;
static int errorOwner;
static BOOL errorDescription;
class Failure:public CException {
public:
    Failure():CException(errorOwner){}
    ~Failure() override {eventDestroyed();}
    int GetErrorMessage(wchar_t*,UINT count,UINT* help) const override;
    int GetErrorMessage(wchar_t*,UINT count,UINT* help) override;
    static void eventDestroyed();
};
static void event(INT64 id,INT64 a=0,INT64 b=0,INT64 c=0,INT64 d=0,INT64 e=0) { events.push_back({id,a,b,c,d,e}); }
void Failure::eventDestroyed() {event(15);}
int Failure::GetErrorMessage(wchar_t* text,UINT count,UINT* help) const {event(14,count,help==nullptr);if(errorDescription)wcscpy_s(text,count,L"manager cleanup failed");return errorDescription;}
int Failure::GetErrorMessage(wchar_t* text,UINT count,UINT* help) {event(14,count,help==nullptr);if(errorDescription)wcscpy_s(text,count,L"manager cleanup failed");return errorDescription;}
static void fail(int point) {if(failure==point)throw 91;if(point==3 && failure==30)throw static_cast<CException*>(new Failure);}
static INT64 nameID(const wchar_t* name) {return !name?-1:IS_INTRESOURCE(name)?INT64(UINT_PTR(name)):-2;}
static _AFX_OCC_DIALOG_INFO externalInfo{};
static LRESULT MS_ABI defaultProcedure(CFormView*,UINT message,WPARAM wparam,LPARAM lparam) {
    event(1,message,INT64(wparam),lparam);fail(1);
    if(mutation==1) {active->m_lpszTemplateName=L"NamedLayout";active->m_pOccDialogInfo=&externalInfo;}
    return defaultResult;
}
static void MS_ABI unexpected(CFormView*) {std::abort();}
static CRuntimeClass* MS_ABI runtime(const CFormView*) {return CFormView::GetThisClass();}
static const DLGTEMPLATE* MS_ABI pre(void*,_AFX_OCC_DIALOG_INFO* info,const DLGTEMPLATE* dialog) {
    lastPre=info;event(2,dialog!=nullptr,active->m_pOccDialogInfo==nullptr,active->m_pCreatedOccDialogInfo!=nullptr);fail(2);
    info->m_pNewTemplate=nullptr;info->m_ppOleDlgItems=nullptr;info->m_cItems=13;info->m_pItemInfo=nullptr;
    return preResult?dialog:nullptr;
}
static void MS_ABI post(void*,_AFX_OCC_DIALOG_INFO* info) {
    event(3,info==active->m_pCreatedOccDialogInfo,active->m_pOccDialogInfo==lastPre);fail(3);
    released=active->m_pCreatedOccDialogInfo;
}
static BOOL MS_ABI controls(void*,CWnd* owner,const wchar_t* name,_AFX_OCC_DIALOG_INFO* info) {
    event(4,owner==active,nameID(name),info==active->m_pOccDialogInfo);fail(4);
    if(mutation==2)active->m_lpszTemplateName=MAKEINTRESOURCEW(220);
    return controlResult;
}
static LRESULT CALLBACK childProcedure(HWND window,UINT message,WPARAM wparam,LPARAM lparam) {
    if(message==0x364) {event(5,GetDlgCtrlID(window));fail(5);return 0;}
    return DefWindowProcW(window,message,wparam,lparam);
}
static int report(const wchar_t* text,UINT type,UINT help) {
    UINT64 hash=1469598103934665603ULL;for(auto* at=text;*at;++at){hash^=WORD(*at);hash*=1099511628211ULL;}
    event(16,type,help,INT64(hash),wcslen(text));return IDOK;
}
static int MS_ABI messageBox(void*,const wchar_t* text,UINT type,UINT help) { return report(text,type,help); }
struct Fixture {
    alignas(8) unsigned char bytes[360]{};void* table[112]{};
    void* managerTable[10]{};void** manager=managerTable;void* module[21]{};
    void* appTable[48]{};alignas(8) BYTE app[1024]{};
    decltype(g_formViewMessageBoxHook) oldMessageBox;
    AFX_MODULE_STATE* previous;MSG sent{},nativeSent{};void* savedNative[4]{};
    std::vector<void*> allocations;
    Fixture() {
        memset(bytes+352,0xa5,8);*reinterpret_cast<void***>(bytes)=table;
        table[0]=reinterpret_cast<void*>(&runtime);table[12]=reinterpret_cast<void*>(&CFormView::GetThisMessageMap);
        table[73]=reinterpret_cast<void*>(&defaultProcedure);table[101]=reinterpret_cast<void*>(&unexpected);
        table[89]=reinterpret_cast<void*>(&unexpected);table[90]=reinterpret_cast<void*>(&unexpected);
        managerTable[5]=reinterpret_cast<void*>(&pre);managerTable[6]=reinterpret_cast<void*>(&post);managerTable[9]=reinterpret_cast<void*>(&controls);
        appTable[47]=reinterpret_cast<void*>(&messageBox);*reinterpret_cast<void***>(app)=appTable;module[1]=app;
        oldMessageBox=g_formViewMessageBoxHook;g_formViewMessageBoxHook=&report;
        module[20]=&manager;module[2]=module[3]=GetModuleHandleW(nullptr);
        previous=g_cmdTestCurrentModule;g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(module);
        auto* thread=AfxGetThreadState();sent=thread->m_lastSentMsg;
        MSG message{};message.message=WM_INITDIALOG;message.wParam=UINT_PTR(0x123456789);message.lParam=-123456789123LL;
        thread->m_lastSentMsg=message;
        if(nativeModule) {savedNative[0]=nativeModule[2];savedNative[1]=nativeModule[3];savedNative[2]=nativeModule[20];savedNative[3]=nativeModule[1];nativeModule[1]=app;nativeModule[2]=module[2];nativeModule[3]=module[3];nativeModule[20]=module[20];}
        if(nativeThread){nativeSent=nativeThread->m_lastSentMsg;nativeThread->m_lastSentMsg=message;}
        active=view();lastPre=nullptr;released=nullptr;failure=mutation=0;controlResult=69;defaultResult=0x123456789;preResult=true;events.clear();
    }
    CFormView* view() {return reinterpret_cast<CFormView*>(bytes);}
    void managerEnabled(bool enable) {module[20]=enable?&manager:nullptr;if(nativeModule)nativeModule[20]=module[20];}
    _AFX_OCC_DIALOG_INFO* owned() {void* info=std::malloc(32);memset(info,0,32);allocations.push_back(info);return static_cast<_AFX_OCC_DIALOG_INFO*>(info);}
    ~Fixture() {
        if(view()->m_pDynamicLayout) { delete view()->m_pDynamicLayout;view()->m_pDynamicLayout=nullptr; }
        if(lastPre)allocations.push_back(lastPre);std::sort(allocations.begin(),allocations.end());allocations.erase(std::unique(allocations.begin(),allocations.end()),allocations.end());
        for(void* pointer:allocations)if(pointer!=released)std::free(pointer);
        AfxGetThreadState()->m_lastSentMsg=sent;g_cmdTestCurrentModule=previous;
        if(nativeModule){nativeModule[2]=savedNative[0];nativeModule[3]=savedNative[1];nativeModule[20]=savedNative[2];nativeModule[1]=savedNative[3];}
        if(nativeThread)nativeThread->m_lastSentMsg=nativeSent;
        g_formViewMessageBoxHook=oldMessageBox;
    }
    void save(std::vector<Event>& out,INT64 returned,int caught=0) {
        out.insert(out.end(),events.begin(),events.end());
        out.push_back({90,returned,caught,view()->m_bInitDlgCompleted,INT64(UINT_PTR(view()->m_hWndFocus)),view()->m_pOccDialogInfo==lastPre && lastPre!=nullptr});
        out.push_back({91,view()->m_pCreatedOccDialogInfo==lastPre && lastPre!=nullptr,view()->m_pOccDialogInfo!=nullptr,view()->m_pCreatedOccDialogInfo!=nullptr,
            lastPre && caught!=91?lastPre->m_cItems:0,INT64(*reinterpret_cast<UINT64*>(bytes+352))});
    }
};
template<class F> static int invoke(F action) {try{action();return 0;}catch(int value){return value;}catch(CException* e){e->Delete();return -1;}}
static bool captureFocus;
static HWND focusHandle;
static BOOL childResult,windowResult;
static decltype(&::GetFocus) realGetFocus;
static decltype(&::IsChild) realIsChild;
static decltype(&::IsWindow) realIsWindow;
static decltype(&::SetFocus) realSetFocus;
static HWND WINAPI getFocus() {if(!captureFocus)return realGetFocus();event(6);fail(6);return focusHandle;}
static BOOL WINAPI isChild(HWND parent,HWND child) {if(!captureFocus)return realIsChild(parent,child);event(7,INT64(UINT_PTR(parent)),INT64(UINT_PTR(child)));fail(7);return childResult;}
static BOOL WINAPI isWindow(HWND window) {if(!captureFocus)return realIsWindow(window);event(8,INT64(UINT_PTR(window)));fail(8);return windowResult;}
static HWND WINAPI setFocus(HWND window) {if(!captureFocus)return realSetFocus(window);event(9,INT64(UINT_PTR(window)));fail(9);return nullptr;}
static std::vector<Event> focusCases(const API& api) {
    std::vector<Event> output;captureFocus=true;
    for(UINT operation:{0u,1u,2u,3u,UINT(-1)})for(UINT_PTR focused:{UINT_PTR(0),UINT_PTR(0x111),UINT_PTR(0x222)})
    for(BOOL child:{FALSE,BOOL(69)})for(BOOL window:{FALSE,BOOL(-1)})for(int at:{0,1,6,7,8,9}) {
        Fixture f;f.view()->m_hWnd=reinterpret_cast<HWND>(UINT_PTR(0x111));f.view()->m_hWndFocus=reinterpret_cast<HWND>(UINT_PTR(0x333));
        focusHandle=reinterpret_cast<HWND>(focused);childResult=child;windowResult=window;failure=at;
        BOOL result=-7;int caught=invoke([&]{if(operation==0)result=api.save(f.view());else if(operation==1)api.focus(f.view(),reinterpret_cast<CWnd*>(UINT_PTR(0x123)));else api.activate(f.view(),operation-2,nullptr);});
        f.save(output,result,caught);
    }
    captureFocus=false;return output;
}
static std::vector<Event> metadata(const API& api) {
    std::vector<Event> output;
    for(int cached:{0,1})for(int resource:{0,1,2,3})for(bool manager:{false,true})for(bool owned:{false,true})for(bool prepared:{false,true})for(int at:{0,2,3}) {
        Fixture f;auto* view=f.view();view->m_lpszTemplateName=resource==0?nullptr:resource==1?MAKEINTRESOURCEW(220):resource==2?L"NamedForm":MAKEINTRESOURCEW(65530);
        if(cached)view->m_pOccDialogInfo=&externalInfo;
        if(owned)view->m_pCreatedOccDialogInfo=f.owned();f.managerEnabled(manager);preResult=prepared;failure=at;
        _AFX_OCC_DIALOG_INFO* result=nullptr;int caught=invoke([&]{result=api.get(view);});
        event(10,result!=nullptr,result==lastPre && lastPre!=nullptr,result==&externalInfo);f.save(output,0,caught);
    }
    return output;
}
static std::vector<Event> initialization(const API& api) {
    std::vector<Event> output;
    WNDCLASSW cls{};cls.lpfnWndProc=childProcedure;cls.hInstance=GetModuleHandleW(nullptr);cls.lpszClassName=L"OpenMfcFormInitChild";RegisterClassW(&cls);
    HWND parent=CreateWindowExW(0,L"STATIC",L"form",WS_OVERLAPPED,0,0,100,80,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HWND list=CreateWindowExW(0,L"LISTBOX",L"",WS_CHILD,0,0,30,30,parent,reinterpret_cast<HMENU>(101),GetModuleHandleW(nullptr),nullptr);
    HWND combo=CreateWindowExW(0,L"COMBOBOX",L"",WS_CHILD|CBS_DROPDOWNLIST,0,30,30,30,parent,reinterpret_cast<HMENU>(102),GetModuleHandleW(nullptr),nullptr);
    HWND probe=CreateWindowExW(0,cls.lpszClassName,L"",WS_CHILD,0,0,20,20,parent,reinterpret_cast<HMENU>(103),GetModuleHandleW(nullptr),nullptr);
    check(parent && list && combo && probe,"create real form controls for native layout-resource comparisons");
    for(bool manager:{false,true})for(bool info:{false,true})for(BOOL accepted:{FALSE,BOOL(69),BOOL(-1)})for(int name:{0,1,2,3})for(int change:{0,1,2})for(int at:{0,1,4}) {
        Fixture f;auto* view=f.view();view->m_hWnd=parent;view->m_lpszTemplateName=name==0?nullptr:name==1?MAKEINTRESOURCEW(200):name==2?L"NamedLayout":MAKEINTRESOURCEW(65530);
        if(info)view->m_pOccDialogInfo=&externalInfo;f.managerEnabled(manager);controlResult=accepted;mutation=change;failure=at;
        SendMessageW(list,LB_RESETCONTENT,0,0);SendMessageW(combo,CB_RESETCONTENT,0,0);
        LRESULT result=-7;int caught=invoke([&]{result=api.init(view,UINT_PTR(0xfedcba987),-918273645);});
        event(11,SendMessageW(list,LB_GETCOUNT,0,0),SendMessageW(combo,CB_GETCOUNT,0,0),nameID(view->m_lpszTemplateName),view->m_pDynamicLayout!=nullptr);f.save(output,result,caught);
    }
    DestroyWindow(parent);return output;
}
static std::vector<Event> destruction(const API& api) {
    std::vector<Event> output;
    for(bool manager:{false,true})for(bool owned:{false,true})for(bool borrowed:{false,true})for(int failAt:{0,30})for(int autoDelete:{-7,0,1,69})for(BOOL described:{FALSE,BOOL(69)}) {
        Fixture f;if(nativeFormConstructor)nativeFormConstructor(f.view(),0);f.managerEnabled(manager);failure=failAt;errorOwner=autoDelete;errorDescription=described;
        if(owned)f.view()->m_pCreatedOccDialogInfo=f.owned();
        if(borrowed)f.view()->m_pOccDialogInfo=&externalInfo;
        auto* ownedInfo=f.view()->m_pCreatedOccDialogInfo;
        g_formViewBaseDestructorExpected=true;int caught=invoke([&]{api.destroy(f.view());});g_formViewBaseDestructorExpected=false;
        // Teardown frees owned metadata even when no manager callback runs.
        if(ownedInfo && !f.view()->m_pCreatedOccDialogInfo)released=ownedInfo;
        f.save(output,0,caught);
    }
    return output;
}
static void realFocus(const API& api) {
    Fixture f;HWND parent=CreateWindowExW(0,L"STATIC",L"focus",WS_OVERLAPPEDWINDOW,30,30,160,120,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HWND child=CreateWindowExW(0,L"EDIT",L"",WS_CHILD|WS_VISIBLE,0,0,60,20,parent,nullptr,GetModuleHandleW(nullptr),nullptr);
    f.view()->m_hWnd=parent;ShowWindow(parent,SW_SHOW);SetActiveWindow(parent);SetFocus(child);
    bool saved=GetFocus()==child && api.save(f.view()) && f.view()->m_hWndFocus==child;
    SetFocus(parent);api.focus(f.view(),nullptr);
    check(saved && GetFocus()==child,"form views save and restore the actual focused child control");
    DestroyWindow(child);events.clear();api.focus(f.view(),nullptr);
    check(!f.view()->m_hWndFocus && !events.empty() && events.front()[0]==1,"destroyed saved controls clear the focus slot and reach the default handler");
    DestroyWindow(parent);
}
static int createHook(CView* view,CREATESTRUCTW* create) {event(12,view==active,create->lpCreateParams==active->m_pCreateContext);return -2;}
static LRESULT printHook(CView* view,CDC* dc,UINT flags) {event(13,view==active,dc==reinterpret_cast<CDC*>(UINT_PTR(0x222)),flags);return -123456789123LL;}
static void maps(const API& api) {
    const auto* map=api.map();const UINT messages[]={WM_SETFOCUS,WM_CREATE,WM_PRINTCLIENT,WM_INITDIALOG};const UINT_PTR signatures[]={36,13,86,14};bool valid=map && map->pfnGetBaseMap;
    if(valid)for(int i=0;i<4;++i){auto& entry=map->lpEntries[i];valid=valid && entry.nMessage==messages[i] && !entry.nCode && !entry.nID && !entry.nLastID && entry.nSig==signatures[i] && entry.pfn;}
    check(valid && map->lpEntries[4].nSig==AfxSig_end,"form-view message map matches native focus, create, print-client and initialization entries");
}
static void compare(const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label) {
    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]) {
        std::printf("mismatch %zu local:",i);for(auto value:expected[i])std::printf(" %lld",value);std::printf(" native:");for(auto value:actual[i])std::printf(" %lld",value);puts("");break;
    }
    check(actual==expected,label);
}
}
static void formviewDialog() {
    using namespace formview_dialog_test;using recordset_value_test::Patch;
    API local{&impl__SaveFocusControl_CFormView__IEAAHXZ,&impl__OnSetFocus_CFormView__IEAAXPEAVCWnd___Z,&impl__OnActivateFrame_CFormView__MEAAXIPEAVCFrameWnd___Z,
        &impl__SetOccDialogInfo_CFormView__MEAAHPEAU_AFX_OCC_DIALOG_INFO___Z,&impl__GetOccDialogInfo_CFormView__MEAAPEAU_AFX_OCC_DIALOG_INFO__XZ,
        &impl__HandleInitDialog_CFormView__IEAA_J_K_J_Z,&impl__GetThisMessageMap_CFormView__KAPEBUAFX_MSGMAP__XZ,&impl___1CFormView__UEAA_XZ};
    auto module=GetModuleHandleW(nullptr);auto* getSlot=recordset_value_test::import(module,"GetFocus");auto* childSlot=recordset_value_test::import(module,"IsChild");
    auto* windowSlot=recordset_value_test::import(module,"IsWindow");auto* focusSlot=recordset_value_test::import(module,"SetFocus");
    check(getSlot && childSlot && windowSlot && focusSlot,"locate form-focus imports for deterministic callback comparisons");if(!getSlot || !childSlot || !windowSlot || !focusSlot)return;
    realGetFocus=reinterpret_cast<decltype(realGetFocus)>(*getSlot);realIsChild=reinterpret_cast<decltype(realIsChild)>(*childSlot);
    realIsWindow=reinterpret_cast<decltype(realIsWindow)>(*windowSlot);realSetFocus=reinterpret_cast<decltype(realSetFocus)>(*focusSlot);
    Patch g(getSlot,reinterpret_cast<void*>(&getFocus)),c(childSlot,reinterpret_cast<void*>(&isChild)),w(windowSlot,reinterpret_cast<void*>(&isWindow)),s(focusSlot,reinterpret_cast<void*>(&setFocus));
    maps(local);auto focus=focusCases(local),information=metadata(local),initial=initialization(local),lifetime=destruction(local);realFocus(local);
    {
        Fixture f;auto* old=f.owned();f.view()->m_pCreatedOccDialogInfo=old;
        check(local.set(f.view(),&externalInfo)==TRUE && f.view()->m_pOccDialogInfo==&externalInfo && f.view()->m_pCreatedOccDialogInfo==old &&
            local.set(f.view(),nullptr)==TRUE && !f.view()->m_pOccDialogInfo && f.view()->m_pCreatedOccDialogInfo==old,"OLE dialog-info setters update the native slot without redispatch or ownership changes");
        f.view()->m_lpszTemplateName=MAKEINTRESOURCEW(220);g_testGenericAllocationFailure=true;bool memory=false;
        try{local.get(f.view());}catch(const std::bad_alloc&){memory=true;}g_testGenericAllocationFailure=false;
        check(memory && !f.view()->m_pOccDialogInfo && f.view()->m_pCreatedOccDialogInfo==old,"dialog metadata allocation failure preserves both existing ownership slots");
    }
    {
        Fixture f;f.view()->m_pCreateContext=reinterpret_cast<CCreateContext*>(UINT_PTR(0x777));CREATESTRUCTW create{};g_formViewCreateHook=createHook;g_formViewPrintHook=printHook;
        check(impl__OnCreate_CFormView__IEAAHPEAUtagCREATESTRUCTW___Z(f.view(),&create)==-2 && create.lpCreateParams==f.view()->m_pCreateContext &&
            impl__OnPrintClient_CFormView__IEAA_JPEAVCDC__I_Z(f.view(),reinterpret_cast<CDC*>(UINT_PTR(0x222)),0x80000001)==-123456789123LL,
            "form create/print adapters forward the stored context, CDC, full flags and inherited return values");
        g_formViewCreateHook=nullptr;g_formViewPrintHook=nullptr;
    }
    {
        Fixture f;f.managerEnabled(false);f.view()->m_hWnd=CreateWindowExW(0,L"STATIC",L"mapped form",WS_OVERLAPPED,0,0,60,40,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
        LRESULT result=-7;
        check(openmfc::detail::DispatchWindowMessageMap(f.view(),WM_INITDIALOG,0,0,&result) && result==0 &&
            events.size()==1 && events[0][0]==1 && !f.view()->m_bInitDlgCompleted,
            "WM_INITDIALOG reaches the form's real initialization handler without running initial data exchange");
        events.clear();AfxGetThreadState()->m_lastSentMsg.message=WM_SETFOCUS;
        f.view()->m_hWndFocus=reinterpret_cast<HWND>(UINT_PTR(0x333));
        check(openmfc::detail::DispatchWindowMessageMap(f.view(),WM_SETFOCUS,0,0,&result) && !f.view()->m_hWndFocus &&
            events.size()==1 && events[0][0]==1 && events[0][1]==WM_SETFOCUS,
            "WM_SETFOCUS routes through the form map and forwards invalid saved focus to the active default message");
        DestroyWindow(f.view()->m_hWnd);
    }
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_FORMVIEW_DLL",path,1024)) {
        HMODULE native=LoadLibraryW(path);check(native!=nullptr,"load native MFC for form-view behavior comparisons");
        if(native) {
            auto* base=reinterpret_cast<char*>(native);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"form-view oracle uses the verified MFC image build");
            if(pinned) {
                nativeModule=reinterpret_cast<void** (MS_ABI*)()>(base+0x133930)();nativeThread=reinterpret_cast<_AFX_THREAD_STATE* (MS_ABI*)()>(base+0x1332a0)();
                Patch ng(reinterpret_cast<void**>(base+0x2c71b0),reinterpret_cast<void*>(&getFocus)),nc(reinterpret_cast<void**>(base+0x2c71b8),reinterpret_cast<void*>(&isChild)),
                    nw(reinterpret_cast<void**>(base+0x2c7138),reinterpret_cast<void*>(&isWindow)),ns(reinterpret_cast<void**>(base+0x2c6da8),reinterpret_cast<void*>(&setFocus));
                API functions{reinterpret_cast<decltype(local.save)>(base+0x27a670),reinterpret_cast<decltype(local.focus)>(base+0x27a6c0),reinterpret_cast<decltype(local.activate)>(base+0x27a650),
                    reinterpret_cast<decltype(local.set)>(base+0x1d7fd0),reinterpret_cast<decltype(local.get)>(base+0x27a880),reinterpret_cast<decltype(local.init)>(base+0x27a820),reinterpret_cast<decltype(local.map)>(base+0x27a240),reinterpret_cast<decltype(local.destroy)>(base+0x27a360)};
                maps(functions);compare(focusCases(functions),focus,"form focus saving/restoration, inactive-frame updates and callback failures match native MFC");
                compare(metadata(functions),information,"form OLE metadata caching, resource lookup, manager callbacks and interrupted ownership match native MFC");
                compare(initialization(functions),initial,"form initialization ordering, OLE controls, real layout resources and failure paths match native MFC");
                nativeFormConstructor=reinterpret_cast<decltype(nativeFormConstructor)>(base+0x27a300);
                compare(destruction(functions),lifetime,"form destruction releases owned metadata and matches native exception description and unconditional deletion");
                nativeFormConstructor=nullptr;realFocus(functions);nativeModule=nullptr;nativeThread=nullptr;
            }
            FreeLibrary(native);
        }
    }
}
