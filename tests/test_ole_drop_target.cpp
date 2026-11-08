#define OPENMFC_TEST_WINDOW_HANDLES
#define OPENMFC_TEST_REAL_MODULE_THREAD_STATE
#define OPENMFC_TEST_REAL_GLOBAL_LOCKS
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include "detail/OleDropTargetSupport.h"
#include "detail/InterfaceMapsSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include <array>
#include <vector>
#include <cstdio>
#include <cstring>
#include <cwchar>
#include <climits>
extern "C" {
void AFXAPI impl__AfxLockGlobals__YAXH_Z(int);
void AFXAPI impl__AfxUnlockGlobals__YAXH_Z(int);
void* AFXAPI impl___0CWnd__QEAA_XZ(void*);
void AFXAPI impl___1CWnd__UEAA_XZ(CWnd*);
BOOL AFXAPI impl__Attach_CWnd__QEAAHPEAUHWND_____Z(CWnd*,HWND);
HWND AFXAPI impl__Detach_CWnd__QEAAPEAUHWND____XZ(CWnd*);
}
namespace {
using A=openmfc::detail::OleDropTargetAccess;
using D=openmfc::detail::OleDataObjectAccess;
int failures;
void check(bool ok,const char* message){if(!ok){++failures;std::printf("FAIL: %s\n",message);}}
struct API {
    COleDropTarget* (AFXAPI* ctor)(void*);
    void (AFXAPI* dtor)(COleDropTarget*);
    BOOL (AFXAPI* reg)(COleDropTarget*,CWnd*);
    void (AFXAPI* revoke)(COleDropTarget*);
    DWORD (AFXAPI* enter)(COleDropTarget*,CWnd*,COleDataObject*,DWORD,CPoint);
    DWORD (AFXAPI* over)(COleDropTarget*,CWnd*,COleDataObject*,DWORD,CPoint);
    BOOL (AFXAPI* drop)(COleDropTarget*,CWnd*,COleDataObject*,DWORD,CPoint);
    DWORD (AFXAPI* dropEx)(COleDropTarget*,CWnd*,COleDataObject*,DWORD,DWORD,CPoint);
    void (AFXAPI* leave)(COleDropTarget*,CWnd*);
    DWORD (AFXAPI* scroll)(COleDropTarget*,CWnd*,DWORD,CPoint);
    const AFX_INTERFACEMAP* (AFXAPI* map)();
    _AFX_THREAD_STATE* (AFXAPI* thread)();
    AFX_MODULE_THREAD_STATE* (AFXAPI* state)();
    void (AFXAPI* destroyLocal)(S_Cthreadlocalobject*);
    void* (AFXAPI* windowCtor)(void*);
    void (AFXAPI* windowDtor)(CWnd*);
    BOOL (AFXAPI* attach)(CWnd*,HWND);
    HWND (AFXAPI* detach)(CWnd*);
    CRuntimeClass* (AFXAPI* viewClass)();
    CRuntimeClass* (AFXAPI* splitClass)();
    CRuntimeClass* (AFXAPI* windowClass)();
    void (AFXAPI* lock)(int);
    void (AFXAPI* unlock)(int);
    void* (AFXAPI* dataCtor)(void*);
    void (AFXAPI* dataAttach)(COleDataObject*,IDataObject*,BOOL);
    void (AFXAPI* dataRelease)(COleDataObject*);
    IDataObject* (AFXAPI* dataDetach)(COleDataObject*);
    IDataObject* (AFXAPI* dataGet)(COleDataObject*,BOOL);
    BOOL (AFXAPI* clipboard)(COleDataObject*);
    void (AFXAPI* ensure)(COleDataObject*);
    BOOL (AFXAPI* available)(COleDataObject*,CLIPFORMAT,FORMATETC*);
    UINT *delay,*interval;int* inset;
    HMODULE image;
};
const API* api;
alignas(16) BYTE modules[3][0x110]{};
void select(unsigned n){api->thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[n]);}
template<class T>T& field(void* p,size_t off){return *reinterpret_cast<T*>(static_cast<BYTE*>(p)+off);}
std::vector<int> events;
void** imported(HMODULE image,const char* wanted){
    auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* desc=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;desc->Name;++desc){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->FirstThunk);
        for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);
        }
    }return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(HMODULE image,const char* name,void* fn){slot=imported(image,name);check(slot!=nullptr,name);if(!slot)std::abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=fn;}
    ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
struct alignas(16) Object {
    std::array<BYTE,136> bytes;
    COleDropTarget* get(){return reinterpret_cast<COleDropTarget*>(bytes.data()+16);}
    void reset(){bytes.fill(0xa5);}
};
std::array<BYTE,136> constructorBytes;
COleDropTarget* watched;
CRITICAL_SECTION* metricLock;bool captureLock,recordCtor;
decltype(&EnterCriticalSection) realEnter;decltype(&LeaveCriticalSection) realLeave;
void WINAPI enterLock(CRITICAL_SECTION* lock){
    if(captureLock)metricLock=lock;
    if(recordCtor && lock==metricLock){events.push_back(1);check(!A::Window(watched) && !A::Data(watched) && A::Timer(watched)==0xffff &&
        A::Tick(watched)==0xa5a5a5a5 && A::Delay(watched)==0xa5a5a5a5 && watched->m_dwRef==1,"constructor initializes native fields and preserves uninitialized timer state before locking");
        auto* com=&watched->m_xDropTarget;check(com->AddRef()==2 && com->Release()==1,"constructor installs its real owner-counted interface before locking");}
    realEnter(lock);
}
void WINAPI leaveLock(CRITICAL_SECTION* lock){if(recordCtor && lock==metricLock)events.push_back(5);realLeave(lock);}
UINT WINAPI profile(LPCWSTR section,LPCWSTR key,INT fallback){
    check(recordCtor && !std::wcscmp(section,L"windows"),"scroll settings use the native profile section");
    if(!std::wcscmp(key,L"DragScrollInset")){check(fallback==11,"inset default is eleven");events.push_back(2);return 5;}
    if(!std::wcscmp(key,L"DragScrollDelay")){check(fallback==50 && *api->inset==5,"scroll delay follows the committed inset");events.push_back(3);return 7;}
    check(!std::wcscmp(key,L"DragScrollInterval") && fallback==50 && *api->delay==7,"scroll interval follows the committed delay");events.push_back(4);return 3;
}
void constructors(){
    Object object;object.reset();watched=object.get();events.clear();recordCtor=true;check(api->ctor(watched)==watched,"constructor returns placement storage");recordCtor=false;
    check(events==std::vector<int>({1,2,3,4,5}),"native settings are loaded under the real process lock in order");
    auto bytes=object.bytes;std::memset(bytes.data()+16,0,8);std::memset(bytes.data()+16+0x60,0,8);
    if(!api->image)constructorBytes=bytes;else check(bytes==constructorBytes,"constructor matches every native field, padding and guard byte");
    auto* type=watched->GetRuntimeClass();check(api->image ? type->m_lpszClassName && !std::strcmp(type->m_lpszClassName,"CCmdTarget"):type==CCmdTarget::GetThisClass(),"target inherits command-target runtime metadata");
    api->dtor(watched);object.reset();watched=object.get();*api->delay=23;events.clear();recordCtor=true;api->ctor(watched);recordCtor=false;
    check(events==std::vector<int>({1,5}) && *api->delay==23,"later construction preserves caller-written cached metrics");api->dtor(watched);
}
HRESULT queryResult;FORMATETC queried;FORMATETC* queriedPointer;
struct Data : IDataObject {
    ULONG refs=1;int id=0;void (*onRelease)(Data*)=nullptr;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid,void** out) override{*out=nullptr;if(iid==IID_IUnknown || iid==IID_IDataObject){*out=this;AddRef();return S_OK;}return E_NOINTERFACE;}
    ULONG STDMETHODCALLTYPE AddRef() override{events.push_back(10+id);return ++refs;}
    ULONG STDMETHODCALLTYPE Release() override{events.push_back(20+id);if(onRelease)onRelease(this);return --refs;}
    HRESULT STDMETHODCALLTYPE GetData(FORMATETC*,STGMEDIUM*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE GetDataHere(FORMATETC*,STGMEDIUM*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE QueryGetData(FORMATETC* value) override{events.push_back(81);queried=*value;queriedPointer=value;return queryResult;}
    HRESULT STDMETHODCALLTYPE GetCanonicalFormatEtc(FORMATETC*,FORMATETC*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE SetData(FORMATETC*,STGMEDIUM*,BOOL) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE EnumFormatEtc(DWORD,IEnumFORMATETC**) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE DAdvise(FORMATETC*,DWORD,IAdviseSink*,DWORD*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE DUnadvise(DWORD) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE EnumDAdvise(IEnumSTATDATA**) override{return E_NOTIMPL;}
} data[3];
struct Enumerator : IEnumFORMATETC {
    ULONG refs=1;int released=0;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void**) override{return E_NOINTERFACE;}
    ULONG STDMETHODCALLTYPE AddRef() override{return ++refs;}
    ULONG STDMETHODCALLTYPE Release() override{events.push_back(29);++released;return --refs;}
    HRESULT STDMETHODCALLTYPE Next(ULONG,FORMATETC*,ULONG*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Skip(ULONG) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Reset() override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Clone(IEnumFORMATETC**) override{return E_NOTIMPL;}
} enumerator;
int deletedErrors;bool describeError;
struct Error : CException {
    explicit Error(BOOL owned):CException(owned){}
    ~Error() override{++deletedErrors;}
    int GetErrorMessage(wchar_t* text,UINT capacity,UINT* help) const override{
        check(capacity==512 && !help,"destructor diagnostics request the native description buffer");
        if(describeError)wcscpy_s(text,capacity,L"target error");return describeError;
    }
};
Error* lastError;int callbackMode;bool scrollingTest=false;
CWnd *window,*otherWindow,*splitterWindow;bool classifyView=false,classifySplitter=false;
DWORD answer,scrollAnswer,exAnswer;BOOL dropAnswer;DWORD* effectPointer;
POINT expectedPoint;
CRuntimeClass* AFXAPI runtime(const CCmdTarget* self){return classifyView && self==window ? api->viewClass():classifySplitter && self==splitterWindow ? api->splitClass():api->windowClass();}
void callbackState(COleDropTarget* self,CWnd* w,int event){events.push_back(event);check(self==watched && w==window,"drag callback receives its actual owner and permanent window wrapper");}
DWORD AFXAPI dragScroll(COleDropTarget* self,CWnd* w,DWORD keys,CPoint pt){callbackState(self,w,30);check(keys==0x1234 && pt.x==expectedPoint.x && pt.y==expectedPoint.y,"COM coordinates are converted to client coordinates before callbacks");return scrollAnswer;}
void borrowed(COleDataObject* value){check(value && value->m_lpDataObject && !value->m_lpEnumerator && !value->m_bClipboard && !D::AutoRelease(value),"callback receives the actual 24-byte borrowed data wrapper");}
DWORD AFXAPI dragEnter(COleDropTarget* self,CWnd* w,COleDataObject* value,DWORD keys,CPoint pt){
    callbackState(self,w,31);borrowed(value);if(!scrollingTest)check(keys==0x1234 && pt.x==expectedPoint.x && pt.y==expectedPoint.y,"enter forwards arguments");
    if(callbackMode==1)value->m_lpEnumerator=&enumerator;
    if(callbackMode==2){lastError=new Error(FALSE);throw static_cast<CException*>(lastError);}
    if(callbackMode==3)*effectPointer=7;
    return answer;
}
DWORD AFXAPI dragOver(COleDropTarget* self,CWnd* w,COleDataObject* value,DWORD keys,CPoint pt){
    callbackState(self,w,32);borrowed(value);check(keys==0x1234 && pt.x==expectedPoint.x && pt.y==expectedPoint.y,"over forwards arguments");
    if(callbackMode==1)value->m_lpEnumerator=&enumerator;
    if(callbackMode==2){lastError=new Error(TRUE);throw static_cast<CException*>(lastError);}
    if(callbackMode==3)*effectPointer=7;
    return answer;
}
void AFXAPI dragLeave(COleDropTarget* self,CWnd* w){callbackState(self,w,33);}
BOOL AFXAPI drop(COleDropTarget* self,CWnd* w,COleDataObject* value,DWORD,CPoint){callbackState(self,w,34);borrowed(value);return dropAnswer;}
DWORD AFXAPI dropEx(COleDropTarget* self,CWnd* w,COleDataObject* value,DWORD,DWORD allowed,CPoint){callbackState(self,w,35);borrowed(value);check(allowed==*effectPointer,"extended drop sees the fresh allowed-effects value");return exAnswer;}
void resetData(){for(int i=0;i<3;++i){data[i].refs=1;data[i].id=i;data[i].onRelease=nullptr;}enumerator.refs=1;enumerator.released=0;lastError=nullptr;deletedErrors=0;callbackMode=0;}
struct Window {
    alignas(16) BYTE bytes[sizeof(CView)]{};CWnd* get(){return reinterpret_cast<CWnd*>(bytes);}
    HWND handle=nullptr;
    Window(HWND parent=nullptr,UINT id=0){api->windowCtor(get());handle=CreateWindowExW(0,L"STATIC",L"drop target",parent ? WS_CHILD:WS_POPUP,10,20,100,80,parent,reinterpret_cast<HMENU>(UINT_PTR(id)),nullptr,nullptr);check(handle && api->attach(get(),handle),"create and map real drop windows");}
    ~Window(){api->detach(get());api->windowDtor(get());DestroyWindow(handle);}
};
HRESULT lockResult,registerResult;int registrationMode;bool observeRegistration;
void* destructorRoot;
HRESULT WINAPI lockObject(IUnknown* unknown,BOOL lock,BOOL last){
    if(observeRegistration){events.push_back(lock ? 40:last ? 43:42);check(unknown==&watched->m_xDropTarget,"external lock uses the real controlling interface");if(lock && registrationMode==1)window->m_hWnd=otherWindow->m_hWnd;}
    return lockResult;
}
HRESULT WINAPI registerDrop(HWND handle,IDropTarget* target){if(observeRegistration){events.push_back(41);check(handle==window->m_hWnd && target==&watched->m_xDropTarget,"registration reloads the window handle and uses the embedded COM part");if(registrationMode==2)window->m_hWnd=otherWindow->m_hWnd;}return registerResult;}
HRESULT WINAPI revokeDrop(HWND){
    if(observeRegistration){events.push_back(44);if(registrationMode==3)A::Window(watched)=otherWindow->m_hWnd;
        if(registrationMode==4){check(field<void*>(watched,0)==destructorRoot,"destruction restores its own root before external revocation");lastError=new Error(FALSE);throw static_cast<CException*>(lastError);}}
    return E_FAIL;
}
int messageBox(const wchar_t* text,UINT type,UINT help){
    std::wstring expected=L"Exception thrown in destructor (D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\oledrop2.cpp:67)";
    if(describeError)expected+=L"\ntarget error";
    check(text==expected && !type && !help && watched->m_pModuleState,"destructor diagnostics preserve the native text, flags and live base");events.push_back(45);return 0;
}
int AFXAPI appMessage(void*,const wchar_t* text,UINT type,UINT help){return messageBox(text,type,help);}
void registration(){
    Object object;object.reset();watched=object.get();api->ctor(watched);void* root=field<void*>(watched,0);destructorRoot=root;
    auto* map=api->map();check(map && map->pEntries[0].nOffset==0x60 && IsEqualIID(*static_cast<const IID*>(map->pEntries[0].piid),IID_IDropTarget) && !map->pEntries[1].piid && map->pEntries[1].nOffset==SIZE_MAX,"target map exposes its native interface offset");
    void* out=nullptr;auto* com=&watched->m_xDropTarget;check(com->QueryInterface(IID_IUnknown,&out)==S_OK && out==com && watched->m_dwRef==2 && com->Release()==1,"target identity and interface references use the command target");
    HMODULE image=api->image ? api->image:GetModuleHandleW(nullptr);
    {
        Patch a(image,"CoLockObjectExternal",reinterpret_cast<void*>(&lockObject)),b(image,"RegisterDragDrop",reinterpret_cast<void*>(&registerDrop)),c(image,"RevokeDragDrop",reinterpret_cast<void*>(&revokeDrop));
        for(HRESULT first:{S_OK,S_FALSE,E_FAIL})for(HRESULT second:{S_OK,S_FALSE,E_FAIL})for(int mutation:{0,1,2}){
            lockResult=first;registerResult=second;registrationMode=mutation;A::Window(watched)=nullptr;window->m_pDropTarget=nullptr;HWND original=window->m_hWnd;
            events.clear();observeRegistration=true;BOOL result=api->reg(watched,window);observeRegistration=false;
            bool success=first==S_OK && second==S_OK;
            check(bool(result)==success && (success ? A::Window(watched)==window->m_hWnd && window->m_pDropTarget==watched:!A::Window(watched) && !window->m_pDropTarget),"only exact S_OK registers, using fresh fields after both callbacks");
            check(events==(first!=S_OK ? std::vector<int>{40}:second!=S_OK ? std::vector<int>{40,41,42}:std::vector<int>{40,41}),"failed registration balances the external lock in native order");
            window->m_hWnd=original;A::Window(watched)=nullptr;window->m_pDropTarget=nullptr;
        }
        lockResult=S_OK;
        for(int mutation:{0,3}){
            A::Window(watched)=window->m_hWnd;window->m_pDropTarget=watched;otherWindow->m_pDropTarget=watched;events.clear();registrationMode=mutation;observeRegistration=true;api->revoke(watched);observeRegistration=false;
            check(events==std::vector<int>({44,43}) && !A::Window(watched) && (mutation ? !otherWindow->m_pDropTarget && window->m_pDropTarget==watched:!window->m_pDropTarget),"revoke ignores API failure, unlocks with last-release and reloads the stored handle before clearing its window");
            window->m_pDropTarget=otherWindow->m_pDropTarget=nullptr;
        }
        for(describeError=false;;describeError=true){
            A::Window(watched)=window->m_hWnd;registrationMode=4;lastError=nullptr;deletedErrors=0;events.clear();
            void* appTable[70]{};appTable[47]=reinterpret_cast<void*>(&appMessage);void** app=appTable;field<void*>(modules[0],8)=&app;
            auto old=g_formViewMessageBoxHook;g_formViewMessageBoxHook=messageBox;observeRegistration=true;field<void*>(watched,0)=reinterpret_cast<void*>(UINT_PTR(1));api->dtor(watched);observeRegistration=false;g_formViewMessageBoxHook=old;field<void*>(modules[0],8)=nullptr;
            check(events==std::vector<int>({44,45}) && deletedErrors==1 && !watched->m_pModuleState,"destructor restores its root, reports framework errors, deletes borrowed exceptions and clears the base module");
            if(describeError)break;object.reset();api->ctor(watched);
        }
    }
    object.reset();api->ctor(watched);check(api->reg(watched,window)!=FALSE,"register the actual interface with Windows OLE");api->revoke(watched);
    check(!A::Window(watched) && !window->m_pDropTarget && watched->m_dwRef==1,"real OLE registration/revoke balances owner references");api->dtor(watched);(void)root;
}
struct Sample {HRESULT result;DWORD effect;UINT timer;ULONG a,b;std::vector<int> trace;bool operator==(const Sample& s)const{return result==s.result && effect==s.effect && timer==s.timer && a==s.a && b==s.b && trace==s.trace;}};
std::vector<Sample> samples;
void comCallbacks(){
    Object object;object.reset();watched=object.get();api->ctor(watched);A::Window(watched)=window->m_hWnd;
    void* root[29];std::memcpy(root,*reinterpret_cast<void***>(watched),sizeof(root));root[23]=reinterpret_cast<void*>(&dragEnter);root[24]=reinterpret_cast<void*>(&dragOver);root[25]=reinterpret_cast<void*>(&drop);root[26]=reinterpret_cast<void*>(&dropEx);root[27]=reinterpret_cast<void*>(&dragLeave);root[28]=reinterpret_cast<void*>(&dragScroll);field<void*>(watched,0)=root;
    auto* com=&watched->m_xDropTarget;size_t sample=0;
    for(int call:{0,1,2})for(DWORD allowed:{0u,1u,2u,3u,4u,7u,0x80000000u,UINT_MAX})for(DWORD requested:{0u,1u,2u,3u,4u,7u,0x80000000u,0x80000001u,UINT_MAX})for(int mode:{0,1,2,3}){
        resetData();answer=requested;scrollAnswer=mode==1 ? requested|DROPEFFECT_SCROLL:0;exAnswer=mode==2 ? requested:DWORD(-1);dropAnswer=mode==3 ? FALSE:TRUE;
        A::Data(watched)=&data[1];data[1].refs=2;A::Timer(watched)=99;DWORD effect=allowed;effectPointer=&effect;events.clear();select(1);
        HRESULT result=call==0 ? com->DragEnter(&data[0],0x1234,POINTL{100,200},&effect):call==1 ? com->DragOver(0x1234,POINTL{100,200},&effect):com->Drop(&data[0],0x1234,POINTL{100,200},&effect);
        Sample value{result,effect,A::Timer(watched),data[0].refs,data[1].refs,events};
        check(result==S_OK && api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"successful COM callbacks return S_OK and restore the ambient module");
        if(!api->image)samples.push_back(value);else check(value==samples[sample],"native COM effects, fallback calls, timer state, references and callback order match");++sample;
        if(A::Data(watched)){com->DragLeave();}select(0);
    }
    for(int call:{0,1,2}){
        resetData();callbackMode=3;answer=2;scrollAnswer=0;exAnswer=DWORD(-1);dropAnswer=TRUE;
        A::Data(watched)=&data[1];data[1].refs=2;DWORD effect=1;effectPointer=&effect;select(1);
        HRESULT result=call==0 ? com->DragEnter(&data[0],0x1234,POINTL{100,200},&effect):call==1 ? com->DragOver(0x1234,POINTL{100,200},&effect):com->Drop(&data[0],0x1234,POINTL{100,200},&effect);
        check(result==S_OK && effect==(call==2 ? 1u:2u),"enter/over use fresh allowed effects while drop retains its pre-callback mask for fallback");
        callbackMode=0;if(A::Data(watched))com->DragLeave();select(0);
    }
    resetData();callbackMode=1;answer=1;scrollAnswer=0;A::Data(watched)=&data[1];data[1].refs=2;DWORD borrowedEffect=7;effectPointer=&borrowedEffect;
    check(com->DragOver(0x1234,POINTL{100,200},&borrowedEffect)==S_OK && enumerator.released==1 && data[1].refs==2,
          "temporary callback wrappers release their enumerator without releasing borrowed cached data");callbackMode=0;com->DragLeave();
    // Exception catches preserve output/cache state and honor CException::Delete.
    for(int call:{0,1}){
        resetData();callbackMode=2;answer=2;scrollAnswer=0;A::Data(watched)=&data[1];data[1].refs=2;DWORD effect=7;effectPointer=&effect;events.clear();select(1);
        HRESULT result=call ? com->DragOver(0x1234,POINTL{100,200},&effect):com->DragEnter(&data[0],0x1234,POINTL{100,200},&effect);
        check(result==E_UNEXPECTED && effect==7 && deletedErrors==call && api->thread()->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[1]),"framework callback exceptions retain native output, cache and conditional exception ownership");
        if(!deletedErrors)delete lastError;callbackMode=0;com->DragLeave();select(0);
    }
    resetData();A::Data(watched)=&data[1];data[1].refs=2;A::Timer(watched)=77;events.clear();DWORD effect=3;select(1);
    check(com->DragEnter(nullptr,0,POINTL{},&effect)==E_INVALIDARG && com->DragOver(0,POINTL{},nullptr)==E_INVALIDARG && com->Drop(nullptr,0,POINTL{},&effect)==E_INVALIDARG &&
          events.empty() && effect==3 && A::Timer(watched)==77 && A::Data(watched)==&data[1],"invalid COM arguments preserve cache, output and timer state");
    callbackMode=0;com->DragLeave();check(!A::Data(watched) && A::Timer(watched)==0xffff && data[1].refs==1,"drag leave clears the live cache and timer with one release");select(0);A::Window(watched)=nullptr;api->dtor(watched);
}
DWORD viewAnswer;int scrollCalls;BOOL canScroll;
DWORD AFXAPI viewEnter(CView*,COleDataObject*,DWORD,CPoint){events.push_back(60);return viewAnswer;}
DWORD AFXAPI viewOver(CView*,COleDataObject*,DWORD,CPoint){events.push_back(61);return viewAnswer;}
void AFXAPI viewLeave(CView*){events.push_back(62);}
BOOL AFXAPI viewDrop(CView*,COleDataObject*,DWORD,CPoint){events.push_back(63);return static_cast<BOOL>(viewAnswer);}
DWORD AFXAPI viewEx(CView*,COleDataObject*,DWORD,DWORD,CPoint){events.push_back(64);return viewAnswer;}
DWORD AFXAPI viewScroll(CView*,DWORD,CPoint){events.push_back(65);return viewAnswer;}
BOOL AFXAPI doScroll(CView*,UINT code,UINT position,BOOL perform){check(!position && code!=0xffff,"view scroll receives the packed native edge code");events.push_back(perform ? 67:66);++scrollCalls;return canScroll;}
BOOL AFXAPI splitScroll(CSplitterWnd*,CView*,UINT,BOOL perform){events.push_back(perform ? 69:68);++scrollCalls;return canScroll;}
ULONGLONG clockValue;ULONGLONG WINAPI clockTick(){events.push_back(70);return clockValue;}
std::vector<std::array<DWORD,5>> scrollSamples;std::vector<std::vector<int>> scrollEvents;
void viewCallbacks(){
    Object object;object.reset();watched=object.get();api->ctor(watched);void* targetRoot[29];std::memcpy(targetRoot,*reinterpret_cast<void***>(watched),sizeof(targetRoot));
    targetRoot[23]=reinterpret_cast<void*>(&dragEnter);targetRoot[27]=reinterpret_cast<void*>(&dragLeave);field<void*>(watched,0)=targetRoot;
    COleDataObject wrapper;wrapper.Attach(&data[0],FALSE);callbackMode=0;effectPointer=nullptr;answer=2;
    for(bool view:{false,true}){
        classifyView=view;viewAnswer=0x12345678;events.clear();
        check(api->enter(watched,window,&wrapper,0,CPoint{})==(view ? viewAnswer:0) && api->over(watched,window,&wrapper,0,CPoint{})==(view ? viewAnswer:0) &&
              api->drop(watched,window,&wrapper,0,CPoint{})==(view ? BOOL(viewAnswer):FALSE) && api->dropEx(watched,window,&wrapper,0,0,CPoint{})==(view ? viewAnswer:UINT_MAX),"qualified base callbacks delegate only to actual view runtime types");
        api->leave(watched,window);check(api->scroll(watched,window,0,CPoint{})==(view ? viewAnswer:0),"custom view scroll effects bypass the default scroll protocol");
        check(events==(view ? std::vector<int>{60,61,63,64,62,65}:std::vector<int>{}),"view callbacks use the native virtual slots and non-view callbacks do nothing");
    }
    classifyView=true;callbackMode=0;scrollingTest=true;A::Data(watched)=&data[0];viewAnswer=DROPEFFECT_SCROLL;
    HMODULE image=api->image ? api->image:GetModuleHandleW(nullptr);size_t sample=0;
    {Patch tick(image,"GetTickCount64",reinterpret_cast<void*>(&clockTick));
        for(bool split:{false,true})for(int inset:{-1,0,5,11,120})for(BOOL possible:{FALSE,TRUE})for(UINT oldTimer:{0xffffu,0xff00u,0u,0x101u})for(DWORD keys:{0u,4u,8u,12u})for(CPoint pt:{CPoint(-1,-1),CPoint(0,0),CPoint(50,40),CPoint(99,79),CPoint(100,80)}){
            classifySplitter=split;
            *api->inset=inset;*api->delay=7;*api->interval=3;A::Timer(watched)=oldTimer;A::Tick(watched)=0xfffffff0;A::Delay(watched)=7;clockValue=0x100000004ULL;canScroll=possible;scrollCalls=0;events.clear();
            DWORD result=api->scroll(watched,window,keys,pt);std::array<DWORD,5> value{result,A::Timer(watched),A::Tick(watched),A::Delay(watched),DWORD(scrollCalls)};
            if(!api->image){scrollSamples.push_back(value);scrollEvents.push_back(events);}else check(value==scrollSamples[sample] && events==scrollEvents[sample],"native scroll edges, timer transitions, wrap arithmetic and callback order match");++sample;
        }
    }
    A::Data(watched)=nullptr;A::Window(watched)=nullptr;api->dtor(watched);classifyView=false;classifySplitter=false;scrollingTest=false;
}
HRESULT clipboardResult;
HRESULT WINAPI clipboardData(IDataObject** out){events.push_back(80);*out=&data[0];return clipboardResult;}
BOOL availableResult;UINT availableFormat;
BOOL WINAPI clipboardAvailable(UINT format){events.push_back(82);availableFormat=format;return availableResult;}
void dataLifetime(){
    alignas(16) BYTE storage[56];std::memset(storage,0xa5,sizeof(storage));auto* value=reinterpret_cast<COleDataObject*>(storage+16);api->dataCtor(value);
    check(!value->m_lpDataObject && !value->m_lpEnumerator && !value->m_bClipboard && D::AutoRelease(value)==TRUE && storage[15]==0xa5 && storage[40]==0xa5,"data wrapper initializes exactly its native 24 bytes");
    for(BOOL owned:{FALSE,TRUE,-1}){
        resetData();api->dataAttach(value,&data[0],owned);value->m_lpEnumerator=&enumerator;events.clear();api->dataRelease(value);
        check(events==(owned ? std::vector<int>{29,20}:std::vector<int>{29}) && !value->m_lpDataObject && !value->m_lpEnumerator && !value->m_bClipboard && D::AutoRelease(value)==owned,"data release cleans enumeration before conditionally owned data and preserves its ownership flag");
    }
    resetData();api->dataAttach(value,&data[0],FALSE);check(api->dataGet(value,TRUE)==&data[0] && data[0].refs==2,"GetIDataObject optionally adds a real reference");
    check(api->dataDetach(value)==&data[0] && !value->m_lpDataObject && data[0].refs==2,"detach transfers the data pointer without releasing it");data[0].refs=1;api->dataRelease(value);
    {
        Patch clipboardHook(api->image ? api->image:GetModuleHandleW(nullptr),"OleGetClipboard",reinterpret_cast<void*>(&clipboardData));
        Patch availabilityHook(api->image ? api->image:GetModuleHandleW(nullptr),"IsClipboardFormatAvailable",reinterpret_cast<void*>(&clipboardAvailable));
        events.clear();check(api->clipboard(value)==TRUE && value->m_bClipboard && !value->m_lpDataObject && events.empty(),"clipboard attachment marks a lazy wrapper without fetching data");
        FORMATETC ignored{};
        for(BOOL answer:{FALSE,TRUE,-7})for(CLIPFORMAT format:{CLIPFORMAT(0),CLIPFORMAT(CF_UNICODETEXT)}){
            availableResult=answer;events.clear();
            check(api->available(value,format,&ignored)==answer && availableFormat==format && events==std::vector<int>{82} && value->m_bClipboard && !value->m_lpDataObject,"clipboard availability queries Windows directly, preserves raw BOOL and keeps lazy data unfetched");
        }
        for(HRESULT result:{S_FALSE,E_FAIL}){clipboardResult=result;events.clear();api->ensure(value);check(events==std::vector<int>{80} && value->m_bClipboard && !value->m_lpDataObject,"lazy clipboard fetch requires exact S_OK");}
        clipboardResult=S_OK;events.clear();api->ensure(value);
        check(events==std::vector<int>{80} && value->m_lpDataObject==&data[0] && !value->m_bClipboard && D::AutoRelease(value),"successful clipboard fetch transfers an owned reference and clears lazy state");
        events.clear();check(api->dataGet(value,FALSE)==&data[0] && events.empty(),"cached clipboard data avoids another fetch");api->dataRelease(value);
    }
    api->dataAttach(value,&data[0],FALSE);
    for(HRESULT result:{S_OK,S_FALSE,E_FAIL}){
        queryResult=result;events.clear();
        check(api->available(value,CF_TEXT,nullptr)==(result==S_OK) && events==std::vector<int>{81} && queried.cfFormat==CF_TEXT && !queried.ptd && queried.dwAspect==DVASPECT_CONTENT && queried.lindex==-1 && queried.tymed==DWORD(-1),"attached data availability queries all media and requires exact S_OK");
        FORMATETC explicitFormat{CF_UNICODETEXT,nullptr,DVASPECT_ICON,4,TYMED_ISTREAM};events.clear();
        check(api->available(value,0,&explicitFormat)==(result==S_OK) && queriedPointer==&explicitFormat && events==std::vector<int>{81},"attached availability forwards an explicit format unchanged");
    }
    events.clear();check(api->available(value,0,nullptr)==FALSE && events.empty(),"an absent format fails before querying attached data");api->dataRelease(value);
}
void run(const API& functions){
    api=&functions;auto* previous=api->thread()->m_pModuleState;select(0);api->lock(4);api->unlock(4);HMODULE image=api->image ? api->image:GetModuleHandleW(nullptr);
    {Patch a(image,"EnterCriticalSection",reinterpret_cast<void*>(&enterLock)),b(image,"LeaveCriticalSection",reinterpret_cast<void*>(&leaveLock)),c(image,"GetProfileIntW",reinterpret_cast<void*>(&profile));realEnter=reinterpret_cast<decltype(realEnter)>(a.old);realLeave=reinterpret_cast<decltype(realLeave)>(b.old);captureLock=true;api->lock(4);captureLock=false;api->unlock(4);constructors();}
    std::puts("Target construction completed");dataLifetime();
    {Window parent;Window first(parent.handle,0xe900),second;window=first.get();otherWindow=second.get();splitterWindow=parent.get();
        void* splitRoot[120]{};std::memcpy(splitRoot,*reinterpret_cast<void***>(splitterWindow),91*8);splitRoot[0]=reinterpret_cast<void*>(&runtime);splitRoot[109]=reinterpret_cast<void*>(&splitScroll);field<void*>(splitterWindow,0)=splitRoot;
        void* windowRoot[120]{};std::memcpy(windowRoot,*reinterpret_cast<void***>(window),91*8);windowRoot[0]=reinterpret_cast<void*>(&runtime);windowRoot[92]=reinterpret_cast<void*>(&doScroll);windowRoot[94]=reinterpret_cast<void*>(&viewEnter);windowRoot[95]=reinterpret_cast<void*>(&viewOver);windowRoot[96]=reinterpret_cast<void*>(&viewLeave);windowRoot[97]=reinterpret_cast<void*>(&viewDrop);windowRoot[98]=reinterpret_cast<void*>(&viewEx);windowRoot[99]=reinterpret_cast<void*>(&viewScroll);field<void*>(window,0)=windowRoot;
        expectedPoint={100,200};ScreenToClient(window->m_hWnd,&expectedPoint);registration();std::puts("Registration and cleanup completed");comCallbacks();std::puts("COM data callbacks completed");viewCallbacks();std::puts("View and scroll callbacks completed");
    }
    api->thread()->m_pModuleState=previous;for(auto& module:modules)api->destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(module+0xf8));std::memset(modules,0,sizeof(modules));
    std::printf("%s drop target completed\n",api->image ? "Native":"Source");
}
}
int main(){
    setvbuf(stdout,nullptr,_IONBF,0);events.reserve(128);samples.reserve(864);scrollSamples.reserve(800);scrollEvents.reserve(800);OleInitialize(nullptr);
    API source{
        [](void* p){return new(p)COleDropTarget;},[](COleDropTarget* p){p->COleDropTarget::~COleDropTarget();},
        [](COleDropTarget* p,CWnd* w){return p->Register(w);},[](COleDropTarget* p){p->COleDropTarget::Revoke();},
        [](COleDropTarget* p,CWnd* w,COleDataObject* d,DWORD k,CPoint pt){return p->COleDropTarget::OnDragEnter(w,d,k,pt);},
        [](COleDropTarget* p,CWnd* w,COleDataObject* d,DWORD k,CPoint pt){return p->COleDropTarget::OnDragOver(w,d,k,pt);},
        [](COleDropTarget* p,CWnd* w,COleDataObject* d,DWORD k,CPoint pt){return p->COleDropTarget::OnDrop(w,d,k,pt);},
        [](COleDropTarget* p,CWnd* w,COleDataObject* d,DWORD a,DWORD b,CPoint pt){return p->COleDropTarget::OnDropEx(w,d,a,b,pt);},
        [](COleDropTarget* p,CWnd* w){p->COleDropTarget::OnDragLeave(w);},[](COleDropTarget* p,CWnd* w,DWORD k,CPoint pt){return p->COleDropTarget::OnDragScroll(w,k,pt);},
        impl__GetThisInterfaceMap_COleDropTarget__KAPEBUAFX_INTERFACEMAP__XZ,AfxGetThreadState,impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,impl___1CThreadLocalObject__QEAA_XZ,
        impl___0CWnd__QEAA_XZ,impl___1CWnd__UEAA_XZ,impl__Attach_CWnd__QEAAHPEAUHWND_____Z,impl__Detach_CWnd__QEAAPEAUHWND____XZ,CView::GetThisClass,CSplitterWnd::GetThisClass,CWnd::GetThisClass,impl__AfxLockGlobals__YAXH_Z,impl__AfxUnlockGlobals__YAXH_Z,
        [](void* p)->void*{return new(p)COleDataObject;},[](COleDataObject* p,IDataObject* d,BOOL b){p->Attach(d,b);},[](COleDataObject* p){p->Release();},[](COleDataObject* p){return p->Detach();},[](COleDataObject* p,BOOL b){return p->GetIDataObject(b);},[](COleDataObject* p){return p->AttachClipboard();},[](COleDataObject* p){p->EnsureClipboardObject();},[](COleDataObject* p,CLIPFORMAT f,FORMATETC* d){return p->IsDataAvailable(f,d);},&A::ScrollDelay(),&A::ScrollInterval(),&A::ScrollInset(),nullptr};
    run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_DROP_TARGET_DLL")){
        auto image=LoadLibraryA(path);check(image!=nullptr,"load pinned native target oracle");if(image){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"target oracle uses the pinned image");if(pinned){API native=source;native.image=image;
#define RVA(member,value) native.member=reinterpret_cast<decltype(native.member)>(b+value)
                RVA(ctor,0x25a810);RVA(dtor,0x25a8f0);RVA(reg,0x25a960);RVA(revoke,0x25a9f0);RVA(enter,0x25ad30);RVA(over,0x25ad90);RVA(drop,0x25adf0);RVA(dropEx,0x25ae50);RVA(leave,0x25aec0);RVA(scroll,0x25aa50);RVA(map,0x25af00);
                RVA(thread,0x1332a0);RVA(state,0x133a20);RVA(destroyLocal,0x14d0d0);RVA(windowCtor,0x28a700);RVA(windowDtor,0x28b740);RVA(attach,0x28adf0);RVA(detach,0x28ae50);RVA(viewClass,0x13d0e0);RVA(splitClass,0x132ac0);RVA(windowClass,0xac90);RVA(lock,0x33540);RVA(unlock,0x335e0);
                RVA(dataCtor,0x2528c0);RVA(dataAttach,0x2528e0);RVA(dataRelease,0x252920);RVA(dataDetach,0x252970);RVA(dataGet,0x2529a0);RVA(clipboard,0x252f40);RVA(ensure,0x252f50);RVA(available,0x252ec0);RVA(delay,0x3be428);RVA(interval,0x3be424);RVA(inset,0x3be420);
#undef RVA
                field<DWORD>(b,0x3c41b4)=0;run(native);
            }FreeLibrary(image);
        }
    }
    OleUninitialize();std::printf("Drop target: %d failures\n",failures);return failures ? 1:0;
}
