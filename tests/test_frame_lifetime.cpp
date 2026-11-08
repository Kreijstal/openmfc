// Frame constructor/destructor and module frame lists run on the real window,
// TLS, module, map, list and string owners. Other frame virtuals are boundaries.
#define OPENMFC_TEST_WINDOW_HANDLES
#define OPENMFC_TEST_REAL_MODULE_THREAD_STATE
#include "detail/D2DTextSupport.h"
#include "detail/GdicoreSupport.h"
#include "d2d_test_support.h"
#include "detail/FrameLifetimeSupport.h"
#include "detail/CThreadLocalObjectSupport.h"
#include "openmfc/afxpriv.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <array>
#include <climits>

// These operations have their own implementations outside the lifetime owner.
// Aborting bodies ensure this fixture never substitutes window-creation or
// layout behavior for native frame semantics.
BOOL CFrameWnd::Create(const wchar_t*,const wchar_t*,DWORD,const RECT&,CWnd*,const wchar_t*,DWORD,CCreateContext*) {std::abort();}
BOOL CFrameWnd::LoadFrame(UINT,DWORD,CWnd*,CCreateContext*) {std::abort();}
void CFrameWnd::GetMessageString(UINT,CString&) const {std::abort();}
void CFrameWnd::RecalcLayout(BOOL) {std::abort();}
void CFrameWnd::ActivateFrame(int) {std::abort();}
void CFrameWnd::OnSetPreviewMode(BOOL,CPrintPreviewState*) {std::abort();}
CWnd* CFrameWnd::GetMessageBar() {std::abort();}
BOOL CFrameWnd::NegotiateBorderSpace(UINT,RECT*) {std::abort();}
BOOL CFrameWnd::OnCreateClient(CREATESTRUCTW*,CCreateContext*) {std::abort();}
void CFrameWnd::OnUpdateFrameTitle(BOOL) {std::abort();}
void CFrameWnd::OnUpdateFrameMenu(HMENU) {std::abort();}
void CFrameWnd::ExitHelpMode() {std::abort();}
CMenu* CFrameWnd::GetMenu() const {std::abort();}
BOOL CFrameWnd::SetMenu(CMenu*) {std::abort();}
BOOL CFrameWnd::PreCreateWindow(CREATESTRUCTW&) {std::abort();}
void CFrameWnd::BeginModalState() {std::abort();}
void CFrameWnd::EndModalState() {std::abort();}
BOOL CFrameWnd::PreTranslateMessage(MSG*) {std::abort();}
BOOL CFrameWnd::OnCommand(WPARAM,LPARAM) {std::abort();}
void CFrameWnd::PostNcDestroy() {std::abort();}
extern "C" {
CFrameWnd* MS_ABI impl___0CFrameWnd__QEAA_XZ(CFrameWnd*);
void MS_ABI impl___1CFrameWnd__UEAA_XZ(CFrameWnd*);
void MS_ABI impl__AddFrameWnd_CFrameWnd__IEAAXXZ(CFrameWnd*);
void MS_ABI impl__RemoveFrameWnd_CFrameWnd__IEAAXXZ(CFrameWnd*);
BOOL MS_ABI impl__IsFrameWnd_CFrameWnd__UEBAHXZ(const CFrameWnd*);
void MS_ABI impl__SetMenuBarVisibility_CFrameWnd__UEAAXK_Z(CFrameWnd*,DWORD);
BOOL MS_ABI impl__SetMenuBarState_CFrameWnd__UEAAHK_Z(CFrameWnd*,DWORD);
DWORD MS_ABI impl__GetMenuBarVisibility_CFrameWnd__UEBAKXZ(const CFrameWnd*);
DWORD MS_ABI impl__GetMenuBarState_CFrameWnd__UEBAKXZ(const CFrameWnd*);
CView* MS_ABI impl__GetActiveView_CFrameWnd__QEBAPEAVCView__XZ(const CFrameWnd*);
CDocument* MS_ABI impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ(CFrameWnd*);
CFrameWnd* MS_ABI impl__GetActiveFrame_CFrameWnd__UEAAPEAV1_XZ(CFrameWnd*);
void MS_ABI impl__SetActivePreviewView_CFrameWnd__QEAAXPEAVCView___Z(CFrameWnd*,CView*);
void MS_ABI impl__SetActiveView_CFrameWnd__QEAAXPEAVCView__H_Z(CFrameWnd*,CView*,BOOL);
HACCEL MS_ABI impl__GetDefaultAccelerator_CFrameWnd__UEAAPEAUHACCEL____XZ(CFrameWnd*);
void MS_ABI impl__DelayUpdateFrameMenu_CFrameWnd__UEAAXPEAUHMENU_____Z(CFrameWnd*,HMENU);
void MS_ABI impl___1CThreadLocalObject__QEAA_XZ(S_Cthreadlocalobject*);
}
namespace {
int failures;
void check(bool value,const char* label) {if(!value) {++failures;std::printf("FAIL: %s\n",label);}}
struct API {
    decltype(&impl___0CFrameWnd__QEAA_XZ) construct;
    decltype(&impl___1CFrameWnd__UEAA_XZ) destroy;
    decltype(&impl__AddFrameWnd_CFrameWnd__IEAAXXZ) add;
    decltype(&impl__RemoveFrameWnd_CFrameWnd__IEAAXXZ) remove;
    decltype(&impl__IsFrameWnd_CFrameWnd__UEBAHXZ) isFrame;
    _AFX_THREAD_STATE* (MS_ABI* thread)();
    AFX_MODULE_THREAD_STATE* (MS_ABI* state)();
    decltype(&impl___1CThreadLocalObject__QEAA_XZ) destroyLocal;
    HMODULE image;
    CStringData* nil;
    decltype(&impl__SetMenuBarVisibility_CFrameWnd__UEAAXK_Z) setVisibility;
    decltype(&impl__SetMenuBarState_CFrameWnd__UEAAHK_Z) setMenuState;
    decltype(&impl__GetMenuBarVisibility_CFrameWnd__UEBAKXZ) visibility;
    decltype(&impl__GetMenuBarState_CFrameWnd__UEBAKXZ) menuState;
    decltype(&impl__GetActiveView_CFrameWnd__QEBAPEAVCView__XZ) view;
    decltype(&impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ) document;
    decltype(&impl__GetActiveFrame_CFrameWnd__UEAAPEAV1_XZ) frame;
    decltype(&impl__SetActivePreviewView_CFrameWnd__QEAAXPEAVCView___Z) preview;
    decltype(&impl__SetActiveView_CFrameWnd__QEAAXPEAVCView__H_Z) activate;
    decltype(&impl__GetDefaultAccelerator_CFrameWnd__UEAAPEAUHACCEL____XZ) accelerator;
    decltype(&impl__DelayUpdateFrameMenu_CFrameWnd__UEAAXPEAUHMENU_____Z) delayMenu;
};
struct alignas(16) Object {
    BYTE bytes[0x1f8];
    CFrameWnd* get() {return reinterpret_cast<CFrameWnd*>(bytes+16);}
    void poison() {std::memset(bytes,0xa5,sizeof(bytes));}
};
alignas(16) BYTE modules[3][0x110]{};
AFX_MODULE_THREAD_STATE* states[3];
const API* active;
void select(unsigned i) {active->thread()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[i]);}
template<class T> T& field(void* object,size_t offset) {return *reinterpret_cast<T*>(static_cast<BYTE*>(object)+offset);}

std::array<BYTE,sizeof(CFrameWnd)> constructorSample;
bool sampled;
void sample(CFrameWnd* self,bool native) {
    std::array<BYTE,sizeof(CFrameWnd)> bytes;std::memcpy(bytes.data(),self,bytes.size());
    for(size_t offset:{size_t(0),size_t(96),size_t(104),size_t(0x128),size_t(0x198)})
        std::memset(bytes.data()+offset,0,sizeof(void*));
    if(!native) {constructorSample=bytes;sampled=true;}
    else check(sampled && bytes==constructorSample,"native and source constructors match every data and padding byte");
}
void registration(const API& api) {
    Object nodes[3];for(auto& node:nodes) {node.poison();node.get()->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[0]);}
    auto& list=states[0]->m_frameList;auto oldOffset=list.m_nNextOffset;list.m_nNextOffset=0x178;select(1);
    for(auto& node:nodes)api.add(node.get());
    check(list.m_pHead==nodes[2].get() && field<void*>(nodes[2].get(),0x178)==nodes[1].get() &&
          field<void*>(nodes[1].get(),0x178)==nodes[0].get(),"registration uses the real list's configurable link offset");
    api.remove(nodes[1].get());check(field<void*>(nodes[2].get(),0x178)==nodes[0].get(),"removing an interior frame links its predecessor to its successor");
    api.remove(nodes[1].get());api.remove(nodes[0].get());api.remove(nodes[2].get());
    check(list.IsEmpty() && field<void*>(nodes[1].get(),0x178)==nodes[0].get(),"removal is idempotent and does not overwrite the removed frame's link");
    list.m_nNextOffset=oldOffset;
    check(api.isFrame(reinterpret_cast<CFrameWnd*>(UINT_PTR(1)))==TRUE,"base frame identification ignores its this pointer");
}
void** import(HMODULE image,const char* wanted) {
    auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* item=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;item->Name;++item) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(b+item->OriginalFirstThunk);
        auto* values=reinterpret_cast<IMAGE_THUNK_DATA*>(b+item->FirstThunk);
        for(;names->u1.AddressOfData;++names,++values)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+names->u1.AddressOfData);
            if(!std::strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&values->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(HMODULE image,const char* name,void* callback) {
        slot=import(image,name);check(slot!=nullptr,name);if(!slot)std::abort();old=*slot;
        VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=callback;
    }
    ~Patch() {*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
std::vector<int> events;
CFrameWnd* watched;
void* expectedFrameRoot;void* expectedListRoot;void* disabled;void* barBlock;
BYTE hookA[0x48],hookB[0x48];
int mutation,errorOwner,deletedErrors;
bool errorText;
void (*realFree)(void*);
struct Failure:CException {
    explicit Failure():CException(errorOwner) {}
    ~Failure() override {++deletedErrors;events.push_back(43);}
    BOOL GetErrorMessage(wchar_t* text,UINT count,UINT* help) const override {
        check(count==512 && !help,"destructor error diagnostics use the native buffer size and NULL help output");
        events.push_back(41);if(errorText)wcscpy_s(text,count,L"frame cleanup failed");return errorText;
    }
    BOOL GetErrorMessage(wchar_t* text,UINT count,UINT* help) override {
        return static_cast<const Failure*>(this)->GetErrorMessage(text,count,help);
    }
};
int messageBox(const wchar_t* text,UINT flags,UINT help) {
    events.push_back(42);check(flags==0 && help==0 && deletedErrors==0,"destructor warning precedes exception deletion with native message-box arguments");
    std::wstring expected=L"Exception thrown in destructor (D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\winfrm.cpp:159)";
    if(errorText)expected+=L"\nframe cleanup failed";
    check(expected==text,"destructor warning text and optional description match native MFC");return IDOK;
}
int AFXAPI appMessageBox(CWinApp*,const wchar_t* text,UINT flags,UINT help) {return messageBox(text,flags,help);}
void __cdecl freeBlock(void* p) {
    if(p==disabled) {
        events.push_back(10);
        check(*reinterpret_cast<void**>(watched)==expectedFrameRoot && states[0]->m_frameList.IsEmpty() &&
              active->thread()->m_pRoutingFrame!=watched,"frame root, registration and routing cleanup precede disabled-window array release");
        if(mutation==1)*reinterpret_cast<void**>(watched->m_retailMiddle.m_pNotifyHook)=hookB;
        if(mutation==2)field<void*>(hookA,0x40)=reinterpret_cast<void*>(UINT_PTR(0x44440000));
        if(mutation==3)throw static_cast<CException*>(new Failure);
    }
    if(p==barBlock) {
        events.push_back(21);check(field<void*>(watched,0x128)==expectedListRoot && field<INT_PTR>(watched,0x140)==0,
            "the embedded bar-list destructor restores its own table and clears count before releasing nodes");
    }
    realFree(p);
}
struct Manager:IAtlStringMgr {
    CStringData* Allocate(int length,int width) override {
        auto* p=static_cast<CStringData*>(std::malloc(sizeof(CStringData)+(length+1)*width));
        p->pStringMgr=this;p->nRefs=1;p->nAllocLength=length;p->nDataLength=length;return p;
    }
    void Free(CStringData* p) override {
        events.push_back(20);check(*reinterpret_cast<void**>(watched)==expectedFrameRoot,"title release occurs before the base window destructor changes the frame root");
        void* actual=field<void*>(mutation==1 ? hookB:hookA,0x40);
        void* wanted=mutation==3 ? watched : mutation==2 ? reinterpret_cast<void*>(UINT_PTR(0x44440000)):nullptr;
        check(actual==wanted,"OLE hook cleanup rereads the post-free hook and clears only a matching frame back-reference");std::free(p);
    }
    CStringData* Reallocate(CStringData*,int,int) override {std::abort();}
    CStringData* GetNilString() override {std::abort();}
    IAtlStringMgr* Clone() override {return this;}
} manager;
void* AFXAPI containerDelete(void* self,unsigned flags) {
    events.push_back(30);check(flags==1 && field<void*>(watched,0x128)==expectedListRoot &&
        field<void*>(watched,0x150)==nullptr,"base window cleanup follows bar-list release");return self;
}
struct Scope {CFrameWnd* old;_AFX_THREAD_STATE* state;Scope* previous;};
void ownership(const API& api) {
    for(mutation=0;mutation<=3;++mutation)for(int owned:{0,1})for(bool described:{false,true}) {
        errorOwner=owned;errorText=described;
        select(0);Object object;object.poison();api.construct(object.get());watched=object.get();
        expectedFrameRoot=field<void*>(watched,0);expectedListRoot=field<void*>(watched,0x128);
        auto* old=reinterpret_cast<CStringData*>(field<wchar_t*>(watched,0x198))-1;
        if(old->Release())old->pStringMgr->Free(old);
        auto* data=manager.Allocate(3,sizeof(wchar_t));wcscpy_s(data->data(),4,L"own");
        field<wchar_t*>(watched,0x198)=data->data();
        auto& bars=openmfc::detail::frame::ControlBars(watched);bars.AddTail(reinterpret_cast<void*>(UINT_PTR(0x1111)));
        bars.AddTail(reinterpret_cast<void*>(UINT_PTR(0x2222)));barBlock=bars.m_pBlocks;
        disabled=std::malloc(32);watched->m_retailTail.m_phWndDisable=static_cast<HWND*>(disabled);
        std::memset(hookA,0,sizeof(hookA));std::memset(hookB,0,sizeof(hookB));
        field<CFrameWnd*>(hookA,0x40)=field<CFrameWnd*>(hookB,0x40)=watched;
        *reinterpret_cast<void**>(watched->m_retailMiddle.m_pNotifyHook)=hookA;
        auto* thread=api.thread();auto* oldFrame=thread->m_pRoutingFrame;auto* oldPush=thread->m_pPushRoutingFrame;
        auto* outside=reinterpret_cast<CFrameWnd*>(UINT_PTR(0x99990000));
        Scope stop{outside,thread,nullptr},outer{outside,thread,&stop},inner{watched,thread,&outer};
        thread->m_pRoutingFrame=watched;thread->m_pPushRoutingFrame=reinterpret_cast<CPushRoutingFrame*>(&inner);
        void* containerTable[2]={nullptr,reinterpret_cast<void*>(&containerDelete)};void** container=containerTable;
        watched->m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
        void* fakeFrame[114]{};void* fakeList[8]{};field<void*>(watched,0)=fakeFrame;field<void*>(watched,0x128)=fakeList;
        void* appTable[70]{};appTable[47]=reinterpret_cast<void*>(&appMessageBox);void** app=appTable;
        field<void*>(modules[0],8)=&app;
        auto oldBox=g_formViewMessageBoxHook;g_formViewMessageBoxHook=messageBox;
        events.clear();deletedErrors=0;
        {
            Patch patch(api.image ? api.image:GetModuleHandleW(nullptr),"free",reinterpret_cast<void*>(&freeBlock));
            realFree=reinterpret_cast<decltype(realFree)>(patch.old);api.destroy(watched);
        }
        std::vector<int> wanted=mutation==3 ? std::vector<int>{10,41,42,43,20,21,30}:std::vector<int>{10,20,21,30};
        check(events==wanted,"native/source frame resource, diagnostic and parent-cleanup ordering matches");
        check(inner.state==nullptr && outer.state==nullptr && stop.state==thread &&
              thread->m_pRoutingFrame==outside && thread->m_pPushRoutingFrame==reinterpret_cast<CPushRoutingFrame*>(&stop),
              "destruction pops every routing scope for this frame and preserves the enclosing scope");
        if(mutation==3)std::free(disabled);
        check(deletedErrors==(mutation==3 ? 1:0),"framework cleanup errors are deleted even when marked borrowed");
        g_formViewMessageBoxHook=oldBox;field<void*>(modules[0],8)=nullptr;
        thread->m_pRoutingFrame=oldFrame;thread->m_pPushRoutingFrame=oldPush;
    }
}

int ctorMode,callocFailures;
decltype(&::SetRectEmpty) realRect;
decltype(&::LocalAlloc) realLocalAlloc;
BOOL WINAPI constructorRect(RECT* rectangle) {
    check(rectangle==&watched->m_retailMiddle.m_rectBorder && active->nil->nRefs>2,
          "rectangle initialization follows construction of the embedded list and owned title");
    check(field<DWORD>(watched,0x100)==0xa5a5a5a5 && field<int>(watched,0x160)==int(0xa5a5a5a5) &&
          watched->m_retailPrefix.m_nWindow==-1 && watched->m_nIDTracking==0,
          "the native constructor's external call occurs between its two field-initialization phases");
    if(ctorMode==1)watched->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[1]);
    if(ctorMode==2)select(1);
    if(ctorMode==3)watched->m_pModuleState=reinterpret_cast<AFX_MODULE_STATE*>(modules[2]);
    BOOL result=realRect(rectangle);if(ctorMode==1)rectangle->left=17;return result;
}
HLOCAL WINAPI constructorAllocation(UINT flags,SIZE_T size) {
    if(ctorMode==3 && size==sizeof(AFX_MODULE_THREAD_STATE)) {++callocFailures;return nullptr;}
    return realLocalAlloc(flags,size);
}
void constructionCallbacks(const API& api) {
    for(ctorMode=0;ctorMode<4;++ctorMode) {
        select(0);Object object;object.poison();watched=object.get();
        LONG oldRefs=api.nil->nRefs;bool memory=false;callocFailures=0;
        {
            Patch rect(api.image ? api.image:GetModuleHandleW(nullptr),"SetRectEmpty",reinterpret_cast<void*>(&constructorRect));
            Patch allocation(api.image ? api.image:GetModuleHandleW(nullptr),"LocalAlloc",reinterpret_cast<void*>(&constructorAllocation));
            realRect=reinterpret_cast<decltype(realRect)>(rect.old);realLocalAlloc=reinterpret_cast<decltype(realLocalAlloc)>(allocation.old);
            try {api.construct(watched);}catch(CException* error) {memory=true;error->Delete();}
        }
        if(ctorMode==3) {
            check(memory && callocFailures==1 && api.nil->nRefs==oldRefs && states[0]->m_frameList.IsEmpty(),
                  "failed frame registration unwinds title and bar-list construction without registering a partial object");
            select(2);states[2]=api.state();check(states[2]->m_frameList.IsEmpty(),"failed registration does not leave a frame in the cold module");
        } else {
            unsigned owner=ctorMode==1 ? 1:0;
            check(!memory && states[owner]->m_frameList.m_pHead==watched && api.nil->nRefs==oldRefs+1,
                  "frame registration reads the object's module after constructor callbacks and owns one title reference");
            if(ctorMode==1)check(watched->m_retailMiddle.m_rectBorder.left==17,"later constructor initialization preserves callback rectangle changes");
            api.destroy(watched);check(api.nil->nRefs==oldRefs,"normal destruction releases the constructor's title reference");
        }
    }
    ctorMode=0;select(0);
}

int menuMutation;DWORD menuArgument,expectedStyle,expectedOld;HMENU replacement;HWND replacementWindow;
BOOL AFXAPI stateOverride(CFrameWnd* self,DWORD stateValue) {
    events.push_back(61);menuArgument=stateValue;
    check(self->m_retailTail.m_dwMenuBarVisibility==expectedStyle,"visibility is stored before invoking the virtual state setter");
    return -17;
}
void menuCallback(CFrameWnd* self,int id) {
    events.push_back(id);
    check(self->m_retailTail.m_dwMenuBarState==expectedOld,"menu callbacks run before the final state update");
    if(menuMutation==1) {self->m_hWnd=replacementWindow;self->m_retailTail.m_hMenu=replacement;}
    if(menuMutation==2)throw 77;
}
void AFXAPI showMenu(CFrameWnd* self) {menuCallback(self,62);}
void AFXAPI hideMenu(CFrameWnd* self) {menuCallback(self,63);}
template<class F> int invalid(F action) {
    try {action();return 0;}catch(int value) {return value==19 ? 1:value;}catch(CException* error) {error->Delete();return 1;}
}
void menuPolicies(const API& api) {
    Object object;auto* self=object.get();void* table[114]{};
    table[101]=reinterpret_cast<void*>(&stateOverride);table[112]=reinterpret_cast<void*>(&showMenu);table[113]=reinterpret_cast<void*>(&hideMenu);
    for(DWORD before:{0u,1u,2u,6u,UINT_MAX})for(DWORD style:{0u,1u,2u,3u,4u,5u,6u,UINT_MAX}) {
        object.poison();field<void*>(self,0)=table;self->m_retailTail.m_dwMenuBarVisibility=before;
        expectedStyle=style;events.clear();menuArgument=0;
        int caught=invalid([&]{api.setVisibility(self,style);});bool valid=style==1 || style==2 || style==6;
        check(caught==int(!valid) && api.visibility(self)==(valid ? style:before),"menu visibility validates native styles and stores the physical field");
        check(events==std::vector<int>(valid && before!=style ? std::initializer_list<int>{61}:std::initializer_list<int>{}),
              "unchanged visibility bypasses state changes and changed visibility uses virtual slot 101 exactly once");
        if(valid && before!=style)check(menuArgument==(style==1 ? 1u:2u),"native visibility policies request the appropriate shown or hidden state");
    }
    HWND windows[2];for(auto& h:windows)h=CreateWindowExW(0,L"STATIC",L"frame menu",WS_OVERLAPPEDWINDOW,0,0,100,100,nullptr,nullptr,nullptr,nullptr);
    HMENU menus[3];for(auto& menu:menus)menu=CreateMenu();
    check(windows[0] && windows[1] && menus[0] && menus[1] && menus[2],"create real menu-state fixtures");
    for(DWORD before:{1u,2u})for(DWORD value:{0u,1u,2u,3u,UINT_MAX})for(int change:{0,1,2}) {
        object.poison();field<void*>(self,0)=table;self->m_hWnd=windows[0];
        self->m_retailTail.m_dwMenuBarState=before;self->m_retailTail.m_hMenu=menus[1];
        SetMenu(windows[0],menus[0]);SetMenu(windows[1],nullptr);
        menuMutation=change;expectedOld=before;replacement=menus[2];replacementWindow=windows[1];events.clear();
        BOOL returned=-19;int caught=invalid([&]{returned=api.setMenuState(self,value);});bool valid=value==1 || value==2;
        bool callback=valid && value!=before;
        check(caught==(!valid ? 1:callback && change==2 ? 77:0),"menu-state validation and callback exceptions match native behavior");
        if(!caught)check(returned==BOOL(callback),"menu-state return indicates a transition even if the Win32 operation fails");
        check(api.menuState(self)==(callback && !caught ? value:before),"menu state is committed only after the callback and Win32 operation");
        check(events==std::vector<int>(callback ? std::initializer_list<int>{value==1 ? 62:63}:std::initializer_list<int>{}),
              "menu transitions invoke only the corresponding native visibility callback");
        if(callback && !caught) {
            HWND target=change==1 ? windows[1]:windows[0];HMENU wanted=value==1 ? change==1 ? menus[2]:menus[1]:nullptr;
            check(GetMenu(target)==wanted,"menu transitions reload callback changes to the HWND and saved menu");
        }
        if(callback && value==2 && change!=1)check(self->m_retailTail.m_hMenu==menus[0],"hiding captures the original menu before the callback, including when it throws");
    }
    object.poison();field<void*>(self,0)=table;self->m_hWnd=nullptr;self->m_retailTail.m_dwMenuBarState=2;self->m_retailTail.m_hMenu=nullptr;
    menuMutation=0;expectedOld=2;events.clear();
    check(api.setMenuState(self,1)==TRUE && api.menuState(self)==1,"a NULL HWND does not suppress native menu-state callbacks or updates");
    SetMenu(windows[0],nullptr);SetMenu(windows[1],nullptr);
    for(auto h:windows)DestroyWindow(h);for(auto menu:menus)DestroyMenu(menu);
}

const API* viewAPI;
CFrameWnd* viewFrame;
CView *previousView,*requestedView,*alternativeView;
enum ViewMutation {Normal,SelectOther,SelectRequested,Recursive,ThrowOld,ThrowNew,SwapTable,ClearNew};
ViewMutation viewMutation;
void* changedViewTable[111];
void AFXAPI activateView(CView* self,BOOL activate,CView* selected,CView* previous) {
    if(self==alternativeView) {
        events.push_back(82);
        check(activate==TRUE && selected==self && previous==nullptr && viewFrame->m_pViewActive==self,
              "nested activation observes the cleared frame and selects its own view");
        return;
    }
    if(!activate) {
        events.push_back(80);
        check(self==previousView && selected==requestedView && previous==previousView && !viewFrame->m_pViewActive,
              "deactivation receives the requested and previous views after the frame clears its active pointer");
        if(viewMutation==SelectOther)viewAPI->preview(viewFrame,alternativeView);
        if(viewMutation==SelectRequested)viewAPI->preview(viewFrame,requestedView);
        if(viewMutation==Recursive)viewAPI->activate(viewFrame,alternativeView,TRUE);
        if(viewMutation==ThrowOld)throw 77;
        if(viewMutation==SwapTable && requestedView)*reinterpret_cast<void***>(requestedView)=changedViewTable;
    } else {
        events.push_back(81);
        check(activate==TRUE && self==requestedView && selected==self && previous==previousView && viewFrame->m_pViewActive==self,
              "activation observes the installed view and retains the original previous-view argument");
        if(viewMutation==ThrowNew)throw 78;
        if(viewMutation==ClearNew)viewAPI->preview(viewFrame,nullptr);
    }
}
void AFXAPI changedActivation(CView* self,BOOL activate,CView* selected,CView* previous) {
    events.push_back(83);
    check(activate==TRUE && self==requestedView && selected==self && previous==previousView && viewFrame->m_pViewActive==self,
          "activation reloads a view's virtual table after deactivation callbacks");
}
void AFXAPI unexpectedFrameVirtual() {std::abort();}
bool sameExcept(const Object& object,std::array<BYTE,sizeof(CFrameWnd)> before,size_t offset,size_t length) {
    auto* after=object.bytes+16;std::memcpy(before.data()+offset,after+offset,length);
    return !std::memcmp(before.data(),after,before.size());
}
void viewPolicies(const API& api) {
    viewAPI=&api;Object object;auto* self=object.get();viewFrame=self;
    alignas(16) BYTE viewStorage[3][sizeof(CView)];
    CView* views[3];void* viewTable[111]{};viewTable[102]=reinterpret_cast<void*>(&activateView);
    std::memcpy(changedViewTable,viewTable,sizeof(viewTable));changedViewTable[102]=reinterpret_cast<void*>(&changedActivation);
    for(unsigned i=0;i<3;++i)views[i]=reinterpret_cast<CView*>(viewStorage[i]);
    void* frameTable[114]{};
    for(unsigned slot:{93u,94u,108u,109u,110u})frameTable[slot]=reinterpret_cast<void*>(&unexpectedFrameVirtual);
    object.poison();field<void*>(self,0)=frameTable;
    std::memset(viewStorage,0xa5,sizeof(viewStorage));
    for(unsigned i=0;i<3;++i)views[i]->m_pDocument=i==2 ? nullptr:reinterpret_cast<CDocument*>(UINT_PTR(0x1000+i*16));
    for(auto* view:{static_cast<CView*>(nullptr),views[0],views[1],views[2]}) {
        self->m_pViewActive=view;std::array<BYTE,sizeof(CFrameWnd)> before;std::memcpy(before.data(),self,before.size());
        check(api.view(self)==view && api.document(self)==(view ? view->m_pDocument:nullptr) && api.frame(self)==self,
              "base view, document and frame getters read native fields without RTTI, child lookup or virtual redispatch");
        check(!std::memcmp(before.data(),self,before.size()),"active-view queries preserve every frame byte");
    }
    self->m_pViewActive=reinterpret_cast<CWnd*>(UINT_PTR(1));
    check(api.view(self)==reinterpret_cast<CView*>(UINT_PTR(1)),"active-view lookup returns the stored pointer without inspecting the view");
    check(api.frame(reinterpret_cast<CFrameWnd*>(UINT_PTR(1)))==reinterpret_cast<CFrameWnd*>(UINT_PTR(1)),
          "the base active-frame getter returns its this pointer without reading it");
    for(auto* view:{static_cast<CView*>(nullptr),views[0],reinterpret_cast<CView*>(UINT_PTR(1))}) {
        std::array<BYTE,sizeof(CFrameWnd)> before;std::memcpy(before.data(),self,before.size());events.clear();
        api.preview(self,view);
        check(api.view(self)==view && events.empty() && sameExcept(object,before,0x170,sizeof(void*)),
              "preview-view selection is only a pointer assignment, with no activation or focus callbacks");
    }
    struct Case {int oldView,newView;BOOL notify;ViewMutation mutation;std::vector<int> calls;int finalView,error;};
    const Case cases[]={
        {-1,-1,TRUE,Normal,{},-1,0}, {-1,1,TRUE,Normal,{81},1,0},
        {-1,1,FALSE,Normal,{},1,0}, {0,0,TRUE,ThrowOld,{},0,0},
        {0,-1,TRUE,Normal,{80},-1,0}, {0,1,FALSE,Normal,{80},1,0},
        {0,1,-1,Normal,{80,81},1,0}, {0,1,TRUE,SelectOther,{80},2,0},
        {0,1,TRUE,SelectRequested,{80},1,0}, {0,-1,TRUE,SelectRequested,{80},-1,0},
        {0,1,TRUE,Recursive,{80,82},2,0}, {0,1,TRUE,ThrowOld,{80},-1,77},
        {-1,1,TRUE,ThrowNew,{81},1,78}, {0,1,TRUE,ThrowNew,{80,81},1,78},
        {0,1,TRUE,SwapTable,{80,83},1,0}, {0,1,TRUE,ClearNew,{80,81},-1,0},
        {0,-1,FALSE,ThrowOld,{80},-1,77}};
    HWND windows[2];for(auto& window:windows)window=CreateWindowExW(0,L"STATIC",L"view focus",WS_OVERLAPPEDWINDOW,0,0,100,100,nullptr,nullptr,nullptr,nullptr);
    check(windows[0] && windows[1],"create view-activation focus fixtures");
    for(const auto& item:cases) {
        object.poison();field<void*>(self,0)=frameTable;
        for(auto* view:views) {std::memset(view,0xa5,sizeof(CView));field<void*>(view,0)=viewTable;view->m_hWnd=windows[1];}
        previousView=item.oldView<0 ? nullptr:views[item.oldView];requestedView=item.newView<0 ? nullptr:views[item.newView];alternativeView=views[2];
        self->m_pViewActive=previousView;viewMutation=item.mutation;events.clear();
        SetFocus(windows[0]);HWND oldFocus=GetFocus();int caught=0;
        std::array<BYTE,sizeof(CFrameWnd)> before;std::memcpy(before.data(),self,before.size());
        try {api.activate(self,requestedView,item.notify);}catch(int error) {caught=error;}
        check(caught==item.error && events==item.calls && self->m_pViewActive==(item.finalView<0 ? nullptr:views[item.finalView]),
              "view transitions preserve native notification, reentrancy and throwing-callback semantics");
        check(sameExcept(object,before,0x170,sizeof(void*)) && GetFocus()==oldFocus,
              "frame activation changes only the active pointer and does not add its own focus operation");
    }
    for(auto window:windows)DestroyWindow(window);
    for(DWORD flags:{0u,1u,2u,0x80000000u,UINT_MAX})for(UINT_PTR menu:{UINT_PTR(0),UINT_PTR(0x1234)}) {
        object.poison();field<void*>(self,0)=frameTable;self->m_retailTail.m_dwBorderFlags=flags;
        std::array<BYTE,sizeof(CFrameWnd)> before;std::memcpy(before.data(),self,before.size());
        api.delayMenu(self,reinterpret_cast<HMENU>(menu));
        std::memcpy(before.data()+0x190,reinterpret_cast<BYTE*>(self)+0x190,sizeof(HMENU));
        check(self->m_retailTail.m_hMenuAlt==reinterpret_cast<HMENU>(menu) && self->m_retailTail.m_dwBorderFlags==(flags|1) &&
              sameExcept(object,before,0x1d0,sizeof(DWORD)),"delayed menu updates record the requested menu and idle bit without synchronous callbacks");
    }
}

CDocument* selectedDocument;
HACCEL documentAccelerator;
int acceleratorMutation;
CDocument* AFXAPI activeDocument(CFrameWnd* self) {
    events.push_back(90);self->m_hAccelTable=reinterpret_cast<HACCEL>(UINT_PTR(0x2222));
    if(acceleratorMutation==1)throw 76;
    return selectedDocument;
}
HACCEL AFXAPI getDocumentAccelerator(CDocument* self) {
    events.push_back(91);check(self==selectedDocument,"accelerator lookup uses the document returned by the frame override");
    viewFrame->m_hAccelTable=reinterpret_cast<HACCEL>(UINT_PTR(0x3333));
    if(acceleratorMutation==2)throw 77;
    return documentAccelerator;
}
void acceleratorPolicies(const API& api) {
    Object object;object.poison();auto* self=object.get();viewFrame=self;
    void* frameTable[114]{};frameTable[93]=reinterpret_cast<void*>(&activeDocument);frameTable[109]=reinterpret_cast<void*>(&unexpectedFrameVirtual);
    void* documentTable[63]{};documentTable[62]=reinterpret_cast<void*>(&getDocumentAccelerator);void** document=documentTable;
    field<void*>(self,0)=frameTable;
    for(bool hasDocument:{false,true})for(UINT_PTR accelerator:{UINT_PTR(0),UINT_PTR(0x4444)})for(int mutation:{0,1,2}) {
        self->m_hAccelTable=reinterpret_cast<HACCEL>(UINT_PTR(0x1111));
        selectedDocument=hasDocument ? reinterpret_cast<CDocument*>(&document):nullptr;
        documentAccelerator=reinterpret_cast<HACCEL>(accelerator);acceleratorMutation=mutation;events.clear();int caught=0;HACCEL result=nullptr;
        try {result=api.accelerator(self);}catch(int error) {caught=error;}
        check(caught==(mutation==1 ? 76:hasDocument && mutation==2 ? 77:0),"accelerator callbacks propagate their exceptions");
        check(events==(hasDocument && mutation!=1 ? std::vector<int>{90,91}:std::vector<int>{90}),
              "base accelerator selection invokes virtual frame slot 93 and document slot 62 exactly once");
        if(!caught)check(result==reinterpret_cast<HACCEL>(hasDocument && accelerator ? accelerator:UINT_PTR(0x1111)),
                         "the document accelerator takes precedence, otherwise the pre-callback frame accelerator is retained");
    }
}

void baseline(const API& api) {
    Object objects[3];select(0);
    for(unsigned i=0;i<3;++i) {
        auto& o=objects[i];o.poison();check(api.construct(o.get())==o.get(),"frame constructor returns caller storage");
        auto* self=o.get();
        check(states[0]->m_frameList.GetHead()==self && self->m_retailMiddle.m_pNextFrameWnd==(i ? objects[i-1].get():nullptr),
              "construction registers frames in the captured module's intrusive list");
        check(self->m_pModuleState==reinterpret_cast<AFX_MODULE_STATE*>(modules[0]) && self->m_hWnd==nullptr,
              "frame construction retains the real CWnd/CCmdTarget module and handle prefix");
        check(self->m_retailPrefix.m_bAutoMenuEnable==1 && self->m_retailPrefix.m_nWindow==-1 &&
              self->m_nIDHelp==0 && self->m_nIDTracking==0 && !self->m_nIDLastMessage &&
              self->m_retailMiddle.m_nShowDelay==-1 && !self->m_pViewActive,
              "frame constructor establishes native command, tracking, view and delay defaults");
        auto& tail=self->m_retailTail;
        check(tail.m_dwMenuBarVisibility==1 && tail.m_dwMenuBarState==1 &&
              !tail.m_hMenu && !tail.m_bTempShowMenu && !tail.m_bMouseHitMenu &&
              tail.m_nProgressBarRangeMin==0 && tail.m_nProgressBarRangeMax==100,
              "native menu and progress defaults use their real frame fields");
        check(field<DWORD>(self,0x184)==0xa5a5a5a5 && field<DWORD>(self,0x1a4)==0xa5a5a5a5 &&
              field<DWORD>(self,0x1d4)==0xa5a5a5a5 && field<UINT64>(o.bytes,0)==0xa5a5a5a5a5a5a5a5ull &&
              field<UINT64>(o.bytes,0x1e8)==0xa5a5a5a5a5a5a5a5ull,
              "placement construction preserves native padding and both external guards");
        auto& bars=openmfc::detail::frame::ControlBars(self);
        auto& title=openmfc::detail::frame::Title(self);
        check(bars.m_nCount==0 && bars.m_nBlockSize==10 && !bars.m_pBlocks && title.IsEmpty(),
              "the embedded native control-bar list and title string are constructed");
        check(api.isFrame(self)==TRUE,"base frame identification is true");
        if(!i)sample(self,api.image!=nullptr);
        auto* runtime=self->GetRuntimeClass();
        check(runtime && !std::strcmp(runtime->m_lpszClassName,"CFrameWnd") && runtime->m_nObjectSize==sizeof(CFrameWnd),
              "the real constructed root exposes the frame runtime descriptor");
    }
    select(1);api.destroy(objects[1].get());
    check(states[0]->m_frameList.GetHead()==objects[2].get() &&
          objects[2].get()->m_retailMiddle.m_pNextFrameWnd==objects[0].get() && states[1]->m_frameList.IsEmpty(),
          "destruction unlinks from the frame's module even when another module is active");
    api.destroy(objects[2].get());api.destroy(objects[0].get());
    check(states[0]->m_frameList.IsEmpty(),"destruction removes the remaining registered frames");
}
void run(const API& api) {
    active=&api;auto* old=api.thread()->m_pModuleState;
    for(unsigned i=0;i<2;++i) {select(i);states[i]=api.state();}
    std::puts(api.image ? "Native baseline":"Source baseline");baseline(api);registration(api);ownership(api);constructionCallbacks(api);menuPolicies(api);
    viewPolicies(api);acceleratorPolicies(api);
    if(!api.image) {
        API cpp=api;
        cpp.view=[](const CFrameWnd* self) {return self->GetActiveView();};
        cpp.document=[](CFrameWnd* self) {return self->CFrameWnd::GetActiveDocument();};
        cpp.frame=[](CFrameWnd* self) {return self->CFrameWnd::GetActiveFrame();};
        cpp.preview=[](CFrameWnd* self,CView* view) {self->SetActivePreviewView(view);};
        cpp.activate=[](CFrameWnd* self,CView* view,BOOL notify) {self->SetActiveView(view,notify);};
        cpp.accelerator=[](CFrameWnd* self) {return self->CFrameWnd::GetDefaultAccelerator();};
        cpp.delayMenu=[](CFrameWnd* self,HMENU menu) {self->CFrameWnd::DelayUpdateFrameMenu(menu);};
        viewPolicies(cpp);acceleratorPolicies(cpp);
    }
    api.thread()->m_pModuleState=old;
    for(auto& m:modules)api.destroyLocal(reinterpret_cast<S_Cthreadlocalobject*>(m+0xf8));
    std::memset(modules,0,sizeof(modules));std::printf("%s frame lifetime completed\n",api.image ? "Native":"Source");
}
}
int main() {
    setvbuf(stdout,nullptr,_IONBF,0);events.reserve(64);
    API source{impl___0CFrameWnd__QEAA_XZ,impl___1CFrameWnd__UEAA_XZ,impl__AddFrameWnd_CFrameWnd__IEAAXXZ,
        impl__RemoveFrameWnd_CFrameWnd__IEAAXXZ,impl__IsFrameWnd_CFrameWnd__UEBAHXZ,
        AfxGetThreadState,impl__AfxGetModuleThreadState__YAPEAVAFX_MODULE_THREAD_STATE__XZ,
        impl___1CThreadLocalObject__QEAA_XZ,nullptr,GetNilStringData(),
        impl__SetMenuBarVisibility_CFrameWnd__UEAAXK_Z,impl__SetMenuBarState_CFrameWnd__UEAAHK_Z,
        impl__GetMenuBarVisibility_CFrameWnd__UEBAKXZ,impl__GetMenuBarState_CFrameWnd__UEBAKXZ,
        impl__GetActiveView_CFrameWnd__QEBAPEAVCView__XZ,impl__GetActiveDocument_CFrameWnd__UEAAPEAVCDocument__XZ,
        impl__GetActiveFrame_CFrameWnd__UEAAPEAV1_XZ,impl__SetActivePreviewView_CFrameWnd__QEAAXPEAVCView___Z,
        impl__SetActiveView_CFrameWnd__QEAAXPEAVCView__H_Z,impl__GetDefaultAccelerator_CFrameWnd__UEAAPEAUHACCEL____XZ,
        impl__DelayUpdateFrameMenu_CFrameWnd__UEAAXPEAUHMENU_____Z};
    run(source);
    if(const char* path=std::getenv("OPENMFC_NATIVE_FRAME_DLL")) {
        HMODULE image=LoadLibraryA(path);check(image!=nullptr,"load native frame oracle");
        if(image) {
            auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"frame oracle uses the pinned native image");
            if(pinned) {
                API native=source;native.image=image;native.nil=reinterpret_cast<CStringData*>(b+0x3b25f0);
#define RVA(member,offset) native.member=reinterpret_cast<decltype(native.member)>(b+offset)
                RVA(construct,0x29cae0);RVA(destroy,0x29cc60);RVA(add,0x29cd80);RVA(remove,0x29cdd0);
                RVA(setVisibility,0x2a0f00);RVA(setMenuState,0x2a0f70);RVA(visibility,0x2a0f60);RVA(menuState,0x2a1000);
                RVA(isFrame,0x3a60);RVA(thread,0x1332a0);RVA(state,0x133a20);RVA(destroyLocal,0x14d0d0);
                RVA(view,0x29ed10);RVA(document,0x29ede0);RVA(frame,0x10130);RVA(preview,0x29edb0);
                RVA(activate,0x29ed20);RVA(accelerator,0x29ce60);RVA(delayMenu,0x2a0130);
#undef RVA
                run(native);
            }
            FreeLibrary(image);
        }
    }
    std::printf("Frame lifetime: %d failures\n",failures);return failures ? 1:0;
}
