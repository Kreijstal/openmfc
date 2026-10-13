#pragma once
#include <cstddef>
extern "C" {
void* MS_ABI impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(void*,void*,BOOL);
int MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd*,int);
void MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(void*,int,int*);
void MS_ABI impl__DDV_MinMaxInt__YAXPEAVCDataExchange__HHH_Z(void*,int,int,int);
void MS_ABI impl__AfxThrowUserException__YAXXZ();
}
namespace exchange_test {
class Window : public CWnd {
public:
    int number=12,calls=0,mode=0;
    BOOL lastSave=FALSE;
    HWND nestedWindow=nullptr;
    void DoDataExchange(CDataExchange* dx) override {
        ++calls;lastSave=dx->m_bSaveAndValidate;
        check(dx->m_pDlgWnd==this && !dx->m_idLastControl && !dx->m_bEditLastControl,
            "UpdateData passes a native initialized exchange object to virtual slot 66");
        check(AfxGetThreadState()->m_hLockoutNotifyWindow==m_hWnd,"notification lockout names the current exchange window");
        if(mode==1)dx->Fail();
        if(mode==2)throw new CException;
        if(mode==3)throw 77;
        if(mode==4) {
            Window nested;nested.m_hWnd=nestedWindow;nested.mode=1;
            check(!nested.UpdateData(TRUE),"nested exchange catches validation failures");nested.m_hWnd=nullptr;
            check(AfxGetThreadState()->m_hLockoutNotifyWindow==m_hWnd,"nested exchange restores the outer notification lockout");return;
        }
        impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(dx,101,&number);
        impl__DDV_MinMaxInt__YAXPEAVCDataExchange__HHH_Z(dx,number,0,50);
    }
};
class Exception : public CUserException {
public:
    explicit Exception(UINT id):CUserException(TRUE,id){}
    BOOL initialized() const {return m_bInitialized;}
    BOOL loaded() const {return m_bLoaded;}
};
static void exceptions() {
    Exception error(103);wchar_t text[64]{};UINT help=99;
    check(sizeof(CUserException)==288 && error.GetRuntimeClass()->m_nObjectSize==288 &&
        error.GetRuntimeClass()->m_pfnGetBaseClass()==CSimpleException::GetThisClass() &&
        CSimpleException::GetThisClass()->m_pfnGetBaseClass()==CException::GetThisClass(),"user exception has the native 288-byte layout and runtime inheritance");
    check(!error.initialized() && error.GetErrorMessage(text,64,&help) && error.initialized() && error.loaded() &&
        !wcscmp(text,L"Validation failed") && !help,"simple exceptions lazily load their resource message and clear the help ID");
    auto* table=*reinterpret_cast<void***>(&error);
    using Message=int (MS_ABI*)(void*,wchar_t*,UINT,UINT*);
    text[0]=L'\0';check(reinterpret_cast<Message>(table[5])(&error,text,64,&help) && !wcscmp(text,L"Validation failed"),
        "native non-const exception message slot 5 forwards to the derived const override");
    text[0]=L'\0';check(reinterpret_cast<Message>(table[6])(&error,text,64,&help) && !wcscmp(text,L"Validation failed"),
        "native const exception message slot 6 reaches the user exception cache");
    wchar_t shortened[5]{};check(error.GetErrorMessage(shortened,5) && !wcscmp(shortened,L"Vali"),"cached exception messages truncate with a terminator");
    Exception absent(104);help=99;text[0]=L'x';
    check(!absent.GetErrorMessage(text,64,&help) && !text[0] && !help && absent.initialized() && !absent.loaded(),"missing resource messages cache failure and clear the output");
    Exception invalid(103);help=99;
    check(!invalid.GetErrorMessage(nullptr,4,&help) && help==99 && !invalid.initialized(),"invalid message destinations leave lazy state and help output unchanged");
    CUserException* first=nullptr;
    try {impl__AfxThrowUserException__YAXXZ();}catch(CUserException* p) {first=p;p->Delete();}
    bool base=false;
    try {impl__AfxThrowUserException__YAXXZ();}catch(CSimpleException* p) {base=p==first;p->Delete();}
    check(first && base && !first->m_bAutoDelete,"user failures throw a shared nondeleting pointer catchable as CSimpleException");
}
struct Container {
    void** table;
    COleControlSite* site;
    UINT lastID;
};
static COleControlSite* MS_ABI findSite(const void* raw,UINT id) {
    auto* container=const_cast<Container*>(static_cast<const Container*>(raw));container->lastID=id;
    return id==202 ? container->site : nullptr;
}
static void exchange() {
    Window window;window.m_hWnd=CreateWindowExW(0,L"STATIC",L"DDX",WS_OVERLAPPEDWINDOW,0,0,250,100,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HWND edit=CreateWindowExW(0,L"EDIT",L"",WS_CHILD|WS_VISIBLE,0,0,100,24,window.m_hWnd,reinterpret_cast<HMENU>(101),GetModuleHandleW(nullptr),nullptr);
    check(window.m_hWnd && edit,"create native parent and edit controls for data exchange");
    auto* state=AfxGetThreadState();HWND previous=state->m_hLockoutNotifyWindow;
    HWND sentinel=reinterpret_cast<HWND>(0x1234);state->m_hLockoutNotifyWindow=sentinel;
    check(impl__UpdateData_CWnd__QEAAHH_Z(&window,FALSE) && window.calls==1 && !window.lastSave && state->m_hLockoutNotifyWindow==sentinel,
        "exported UpdateData invokes the user load handler and restores prior lockout");
    wchar_t text[16]{};GetWindowTextW(edit,text,16);check(!wcscmp(text,L"12"),"DDX writes the numeric member to the actual edit control");
    SetWindowTextW(edit,L"31");check(window.UpdateData(TRUE) && window.number==31 && window.lastSave,"DDX reads edited values into the user member");
    SetWindowTextW(edit,L"70");ShowWindow(window.m_hWnd,SW_SHOW);
    int boxes=g_exchangeMessageBoxes;
    check(!window.UpdateData(TRUE) && window.number==70 && g_exchangeMessageBoxes==boxes && state->m_hLockoutNotifyWindow==sentinel,
        "DDV failures return FALSE without an additional framework message box");
    DWORD start=99,end=99;SendMessageW(edit,EM_GETSEL,reinterpret_cast<WPARAM>(&start),reinterpret_cast<LPARAM>(&end));
    check(start==0 && end==2 && GetFocus()==edit,"save validation failure focuses and selects the tracked edit text");
    alignas(CDataExchange) unsigned char raw[32];memset(raw,0xa5,sizeof(raw));
    auto* dx=static_cast<CDataExchange*>(impl___0CDataExchange__QEAA_PEAVCWnd__H_Z(raw,&window,FALSE));
    check(sizeof(CDataExchange)==24 && !dx->m_bSaveAndValidate && dx->m_pDlgWnd==&window && !dx->m_idLastControl && !dx->m_bEditLastControl &&
        *reinterpret_cast<UINT64*>(raw+24)==0xa5a5a5a5a5a5a5a5ULL,"CDataExchange constructor preserves the guard after its native 24 bytes");
    check(dx->PrepareEditCtrl(101)==edit && dx->m_idLastControl==101 && dx->m_bEditLastControl,"PrepareEditCtrl tracks the ID and edit flag in native fields");
    SendMessageW(edit,EM_SETSEL,1,1);
    try {dx->Fail();}catch(CUserException* p) {p->Delete();}
    SendMessageW(edit,EM_GETSEL,reinterpret_cast<WPARAM>(&start),reinterpret_cast<LPARAM>(&end));
    check(start==1 && end==1,"load failures throw without changing the control selection");
    check(dx->PrepareCtrl(101)==edit && !dx->m_bEditLastControl,"PrepareCtrl clears the previous edit flag");
    bool missing=false;try {dx->PrepareCtrl(999);}catch(CException* p) {missing=true;p->Delete();}
    check(missing && dx->m_idLastControl==101,"missing control IDs throw before modifying the previous tracking record");
    void* table[24]{};table[23]=reinterpret_cast<void*>(&findSite);
    Container container{table,reinterpret_cast<COleControlSite*>(0x4321),0};window.m_pCtrlCont=reinterpret_cast<COleControlContainer*>(&container);
    check(!dx->PrepareCtrl(202) && container.lastID==202 && dx->m_idLastControl==202 && !dx->m_bEditLastControl,
        "windowless OLE controls are found through the native container virtual slot 23");
    check(dx->PrepareOleCtrl(202)==container.site && dx->m_idLastControl==202,"PrepareOleCtrl uses the physical CWnd container and tracks the site ID");
    missing=false;try {dx->PrepareOleCtrl(203);}catch(CException* p) {missing=true;p->Delete();}
    check(missing && dx->m_idLastControl==202,"missing OLE sites throw before modifying the tracking record");
    window.m_pCtrlCont=nullptr;
    window.mode=2;
    check(!window.UpdateData(TRUE) && g_exchangeMessageBoxes==boxes+1 && g_exchangeMessageID==0xf108 && g_exchangeMessageFlags==MB_ICONSTOP &&
        state->m_hLockoutNotifyWindow==sentinel,"framework exceptions show the native initialization error and restore lockout");
    window.mode=3;bool other=false;try {window.UpdateData(TRUE);}catch(int n) {other=n==77;}
    check(other && state->m_hLockoutNotifyWindow==sentinel,"nonframework exceptions propagate with notification state restored");
    window.mode=4;window.nestedWindow=edit;check(window.UpdateData(TRUE) && state->m_hLockoutNotifyWindow==sentinel,"nested exchange restores the caller lockout on completion");
    state->m_hLockoutNotifyWindow=previous;DestroyWindow(window.m_hWnd);window.m_hWnd=nullptr;
}
}
static void dataExchange() { exchange_test::exceptions();exchange_test::exchange(); }
