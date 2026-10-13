#pragma once
#include "openmfc/afxctrlcontainer.h"
#include <map>
extern "C" {
void* MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(void*,UINT,CWnd*);
void MS_ABI impl___1CDialog__UEAA_XZ(CDialog*);
void MS_ABI impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(CDialog*,CFont*,BOOL);
CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread*);
void* MS_ABI impl___0CMFCControlContainer__QEAA_PEAVCWnd___Z(void*,CWnd*);
void MS_ABI impl___1CMFCControlContainer__UEAA_XZ(CMFCControlContainer*);
void* MS_ABI impl___0CByteArray__QEAA_XZ(CByteArray*);
int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEAX_Z(CWnd*,void*);
int MS_ABI impl__ExecuteDlgInit_CWnd__QEAAHPEB_W_Z(CWnd*,const wchar_t*);
void* MS_ABI impl__GetAssocAt_CMapWordToOb__IEBAPEAUCAssoc_1_GAEAI0_Z(const CMapWordToOb*,WORD,UINT&,UINT&);
}
namespace dlginit_test {
class Container:public CMFCControlContainer {public:using CMFCControlContainer::CMFCControlContainer;using CMFCControlContainer::m_mapControlData;};
struct Probe {
    int initial=0,messages=0;
    UINT reject=0;
    std::vector<BYTE> data;
};
static LRESULT CALLBACK procedure(HWND window,UINT message,WPARAM wparam,LPARAM lparam) {
    if(message==WM_NCCREATE)SetWindowLongPtrW(window,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCTW*>(lparam)->lpCreateParams));
    auto* probe=reinterpret_cast<Probe*>(GetWindowLongPtrW(window,GWLP_USERDATA));
    if(probe && message==0x364) {++probe->initial;return 0;}
    if(probe && message==probe->reject) {++probe->messages;return -1;}
    if(probe && message==0x37c) {++probe->messages;auto* begin=reinterpret_cast<BYTE*>(lparam);probe->data.assign(begin,begin+wparam);return 0;}
    return DefWindowProcW(window,message,wparam,lparam);
}
static HWND create(HWND parent,int id,Probe& probe) {
    return CreateWindowExW(0,L"OpenMfcDlgInitProbe",L"control",WS_CHILD,0,0,30,20,parent,reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),GetModuleHandleW(nullptr),&probe);
}
static void append(std::vector<BYTE>& records,WORD id,WORD message,const BYTE* bytes,DWORD size) {
    size_t offset=records.size();records.resize(offset+8+size);
    memcpy(records.data()+offset,&id,2);memcpy(records.data()+offset+2,&message,2);memcpy(records.data()+offset+4,&size,4);
    if(size)memcpy(records.data()+offset+8,bytes,size);
}
static void finish(std::vector<BYTE>& records) {records.push_back(0);records.push_back(0);}
class Owned:public CWnd {
public:
    static int destroyed;
    ~Owned() override {++destroyed;m_hWnd=nullptr;}
};
int Owned::destroyed=0;
static void storage() {
    static_assert(sizeof(CByteArray)==40 && offsetof(CByteArray,m_pData)==8 && offsetof(CByteArray,m_nSize)==16,"native array members");
    static_assert(sizeof(CObArray)==40 && offsetof(CMapWordToOb,m_nCount)==24 && offsetof(CMapWordToOb,m_pBlocks)==40,"native embedded collection members");
    alignas(CByteArray) BYTE raw[48];memset(raw,0xa5,sizeof(raw));
    auto* bytes=static_cast<CByteArray*>(impl___0CByteArray__QEAA_XZ(reinterpret_cast<CByteArray*>(raw)));
    check(!bytes->m_pData && !bytes->m_nSize && !bytes->m_nMaxSize && !bytes->m_nGrowBy && *reinterpret_cast<UINT64*>(raw+40)==0xa5a5a5a5a5a5a5a5ULL,
        "byte-array constructor initializes native 40-byte storage and preserves caller guards");
    bytes->SetSize(3);bytes->SetAt(0,10);bytes->SetAt(1,20);bytes->SetAt(2,30);
    BYTE* first=bytes->m_pData;bytes->SetSize(1);bytes->SetSize(3);
    check(bytes->m_pData==first && bytes->m_nSize==3 && bytes->m_pData[0]==10 && !bytes->m_pData[1] && !bytes->m_pData[2],"array shrink retains capacity and regrowth initializes exposed elements");
    bytes->InsertAt(1,9,2);bytes->RemoveAt(2,2);bytes->FreeExtra();
    check(bytes->GetSize()==3 && bytes->m_nMaxSize==3 && bytes->GetAt(0)==10 && bytes->GetAt(1)==9 && !bytes->GetAt(2),"array insertion/removal and compaction update the physical size and data");
    bytes->m_nSize=2;check(bytes->GetSize()==2,"array operations honor size updates made by native inline client accessors");
    bytes->~CByteArray();
    CObArray objects;objects.SetSize(2);struct Object: CObject{};Object one,two;objects.SetAt(0,&one);objects.SetAtGrow(3,&two);
    check(objects.m_nSize==4 && objects.m_pData[0]==&one && !objects.m_pData[1] && !objects.m_pData[2] && objects.m_pData[3]==&two,
        "object arrays expose native pointer storage and initialize new slots");
    const CObArray& constObjects=objects;const CObject** constData=constObjects.GetData();
    check(constData[0]==&one && constData[3]==&two,"const object-array data access has the native const CObject** signature");
    bool memory=false;try {objects.SetSize(PTRDIFF_MAX);}catch(std::bad_alloc&) {memory=true;}
    check(memory && objects.m_nSize==4 && objects.m_pData[0]==&one,"overflowing array allocations use the framework memory failure without losing existing data");
    CMapWordToOb map(2);CObject* value=reinterpret_cast<CObject*>(0x1234);
    check(!map.Lookup(1,value) && value==reinterpret_cast<CObject*>(0x1234) && !map.GetStartPosition(),"empty native maps preserve lookup outputs");
    map.SetAt(1,&one);map.SetAt(2,&two);map.SetAt(33,&one);
    UINT hash,bucket;auto* assoc=static_cast<CMapWordToOb::CAssoc*>(impl__GetAssocAt_CMapWordToOb__IEBAPEAUCAssoc_1_GAEAI0_Z(&map,2,hash,bucket));
    check(map.m_nCount==3 && map.m_pBlocks && assoc && assoc->value==&two && assoc->key==2 && hash==0 && bucket==0 &&
        map.m_pHashTable[0]==assoc,"map exports return real native associations with value at offset 8 and WORD key at 16");
    std::map<WORD,CObject*> seen;auto pos=map.GetStartPosition();
    check(sizeof(pos)==8 && reinterpret_cast<INT_PTR>(pos)==-1,"map iteration starts with the native pointer-sized BEFORE_START_POSITION sentinel");
    while(pos) {WORD key;CObject* object;map.GetNextAssoc(pos,key,object);seen[key]=object;}
    check(seen.size()==3 && seen[1]==&one && seen[2]==&two && seen[33]==&one,"native map iteration traverses collisions and subsequent buckets");
    map.RemoveKey(2);map.RemoveKey(1);map.RemoveKey(33);
    check(!map.m_nCount && !map.m_pHashTable && !map.m_pBlocks && !map.m_pFreeList,"removing the final association releases the native map's buckets and plex blocks");
    CWnd parent;alignas(Container) BYTE containerStorage[120];memset(containerStorage,0xa5,sizeof(containerStorage));
    auto* container=static_cast<Container*>(impl___0CMFCControlContainer__QEAA_PEAVCWnd___Z(containerStorage,&parent));
    check(container->m_pWnd==&parent && !container->m_arSubclassedCtrls.m_pData && !container->m_mapControlData.m_pHashTable &&
        container->m_mapControlData.m_nHashTableSize==17 && container->m_mapControlData.m_nBlockSize==10 &&
        *reinterpret_cast<UINT64*>(containerStorage+112)==0xa5a5a5a5a5a5a5a5ULL,"container constructor initializes both native embedded collections within 112 bytes");
    BYTE source[]{1,0,3};container->SetControlData(12,3,source);source[0]=99;DWORD size=77;BYTE* data=nullptr;
    check(container->GetControlData(12,size,data) && size==3 && data[0]==1 && !data[1] && data[2]==3,"control data is copied into an owned native byte array");
    container->SetControlData(13,4,nullptr);check(container->GetControlData(13,size,data) && size==4 && !data[0] && !data[3],"null control-data inputs allocate zero-filled storage");
    size=88;data=reinterpret_cast<BYTE*>(0x4321);check(!container->GetControlData(99,size,data) && size==88 && data==reinterpret_cast<BYTE*>(0x4321),"missing control-data IDs preserve both output references");
    container->m_arSubclassedCtrls.Add(new Owned);container->m_arSubclassedCtrls.Add(nullptr);
    int destroyed=Owned::destroyed;impl___1CMFCControlContainer__UEAA_XZ(container);
    check(Owned::destroyed==destroyed+1,"container destruction deletes owned controls as well as control-data arrays");
}
static void records() {
    WNDCLASSW cls{};cls.hInstance=GetModuleHandleW(nullptr);cls.lpfnWndProc=procedure;cls.lpszClassName=L"OpenMfcDlgInitProbe";RegisterClassW(&cls);
    CWnd parent;parent.m_hWnd=CreateWindowExW(0,L"STATIC",L"original caption",WS_OVERLAPPEDWINDOW,0,0,200,150,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HWND list=CreateWindowExW(0,L"LISTBOX",L"",WS_CHILD,0,0,30,20,parent.m_hWnd,reinterpret_cast<HMENU>(101),GetModuleHandleW(nullptr),nullptr);
    HWND combo=CreateWindowExW(0,L"COMBOBOX",L"",WS_CHILD|CBS_DROPDOWNLIST,0,25,30,20,parent.m_hWnd,reinterpret_cast<HMENU>(102),GetModuleHandleW(nullptr),nullptr);
    Probe control,nested;HWND target=create(parent.m_hWnd,103,control);create(target,104,nested);
    check(parent.m_hWnd && list && combo && target,"create native dialog children for packed DLGINIT tests");
    std::vector<BYTE> data;BYTE ignored[]{9};append(data,101,0x888,ignored,1);
    append(data,101,0x401,reinterpret_cast<const BYTE*>("one"),4);append(data,102,0x403,reinterpret_cast<const BYTE*>("ab"),3);finish(data);
    check(impl__ExecuteDlgInit_CWnd__QEAAHPEAX_Z(&parent,data.data()) && SendMessageW(list,LB_GETCOUNT,0,0)==1 && SendMessageW(combo,CB_GETCOUNT,0,0)==1,
        "DLGINIT skips unknown records, advances over odd payload lengths and translates legacy list/combo messages");
    wchar_t text[32]{};SendMessageW(list,LB_GETTEXT,0,reinterpret_cast<LPARAM>(text));check(!wcscmp(text,L"one") && control.initial==1 && !nested.initial,
        "ANSI DLGINIT strings reach Unicode controls and initial-update visits direct children only");
    check(impl__ExecuteDlgInit_CWnd__QEAAHPEB_W_Z(&parent,MAKEINTRESOURCEW(210)) && SendMessageW(list,LB_GETCOUNT,0,0)==2 && SendMessageW(combo,CB_GETCOUNT,0,0)==2,
        "numeric resource initialization reads RT_DLGINIT instead of treating the resource ID as text");
    check(parent.ExecuteDlgInit(L"NamedInit") && SendMessageW(list,LB_GETCOUNT,0,0)==3,"named resources also execute native list initialization messages");
    GetWindowTextW(parent.m_hWnd,text,32);check(!wcscmp(text,L"original caption"),"DLGINIT leaves the dialog caption unchanged");
    int initialized=control.initial;
    check(parent.ExecuteDlgInit(static_cast<void*>(nullptr)) && parent.ExecuteDlgInit(static_cast<const wchar_t*>(nullptr)) && parent.ExecuteDlgInit(MAKEINTRESOURCEW(999)) && control.initial==initialized+3,
        "null blobs, null resource names and absent resources succeed and deliver initial update");
    data.clear();BYTE payload[]{5,0,7};append(data,103,0x37c,payload,3);finish(data);
    check(parent.ExecuteDlgInit(data.data()) && parent.m_pMFCCtrlContainer && parent.m_pMFCCtrlContainer->m_pWnd==&parent && control.messages==1 && control.data==std::vector<BYTE>({5,0,7}),
        "feature-pack DLGINIT creates the native container and sends byte count plus original data to the child");
    DWORD size=0;BYTE* saved=nullptr;check(parent.m_pMFCCtrlContainer->GetControlData(103,size,saved) && size==3 && saved[0]==5 && !saved[1] && saved[2]==7 && saved!=data.data()+8,
        "feature-pack initialization preserves an independent copy for later control replacement");
    control.reject=0x37c;initialized=control.initial;data.clear();append(data,103,0x37c,payload,3);append(data,101,LB_ADDSTRING,reinterpret_cast<const BYTE*>("later"),6);finish(data);
    check(!parent.ExecuteDlgInit(data.data()) && control.initial==initialized && SendMessageW(list,LB_GETCOUNT,0,0)==3 && parent.m_pMFCCtrlContainer->GetControlData(103,size,saved),
        "rejected feature-pack initialization stores its data but stops remaining records and initial update");
    control.reject=LB_ADDSTRING;data.clear();append(data,103,LB_ADDSTRING,reinterpret_cast<const BYTE*>("fail"),5);finish(data);
    check(!parent.ExecuteDlgInit(data.data()) && control.initial==initialized,"native list-message failure also suppresses initial update");
    control.reject=0;initialized=control.initial;int grand=nested.initial;
    CWnd::SendMessageToDescendants(parent.m_hWnd,0x364,0,0,TRUE,FALSE);
    check(control.initial==initialized+1 && nested.initial==grand+1,"deep descendant broadcasts include hidden grandchildren");
    DestroyWindow(parent.m_hWnd);parent.m_hWnd=nullptr;
}

class Dialog:public CDialog {
public:
    int exchanges=0,ended=0,lastResult=0;
    BOOL fail=FALSE,lastSave=FALSE;
    int number=9;
    Probe* child=nullptr;
    explicit Dialog(const wchar_t* name):CDialog(name){}
    CMenu* GetMenu() const override {return nullptr;}
    void DoDataExchange(CDataExchange* dx) override {
        ++exchanges;lastSave=dx->m_bSaveAndValidate;
        if(!lastSave && child)check(child->initial>0 && m_pDynamicLayout,"dialog initialization executes child initial-update and loads layout before data exchange");
        if(fail)dx->Fail();
        impl__DDX_Text__YAXPEAVCDataExchange__HAEAH_Z(dx,101,&number);
    }
    void EndModalLoop(int result) override {++ended;lastResult=result;}
};
class Slots:public CDialog {
public:
    int called=0;CFont* font=nullptr;UINT defaultMessage=0;WPARAM defaultParam=0;
    BOOL Create(UINT,CWnd*) override {called=91;return TRUE;}
    BOOL Create(const wchar_t*,CWnd*) override {called=92;return TRUE;}
    BOOL CreateIndirect(HGLOBAL,CWnd*) override {called=93;return TRUE;}
    BOOL CreateIndirect(const DLGTEMPLATE*,CWnd*,void*) override {called=94;return TRUE;}
    INT_PTR DoModal() override {called=95;return 95;}
    BOOL OnInitDialog() override {called=96;return TRUE;}
    void OnSetFont(CFont* value) override {called=97;font=value;}
    LRESULT DefWindowProcW(UINT message,WPARAM param,LPARAM) override {defaultMessage=message;defaultParam=param;return 17;}
    void OnOK() override {called=98;}
    void OnCancel() override {called=99;}
    void PreInitDialog() override {called=100;}
};
class HelpTarget:public CWnd {
public:
    BOOL enabled=FALSE;
    int queries=0;
    BOOL OnCmdMsg(UINT id,int code,void* extra,AFX_CMDHANDLERINFO* info) override {
        ++queries;check(id==0xe146 && code==CN_COMMAND && !extra && info,"Help availability probes handlers without executing a command");return enabled;
    }
};
struct Thread {void** table;CWnd* main;};
static CWnd* MS_ABI mainWindow(Thread* thread) {return thread->main;}
static void dialogs() {
    static_assert(sizeof(CDialog)==304 && offsetof(CDialog,m_nIDHelp)==232 && offsetof(CDialog,m_lpszTemplateName)==240 &&
        offsetof(CDialog,m_lpDialogInit)==264 && offsetof(CDialog,m_pParentWnd)==272 && offsetof(CDialog,m_pOccDialogInfo)==296,"native dialog fields");
    Slots slots;auto* table=*reinterpret_cast<void***>(&slots);
    using ID=BOOL (MS_ABI*)(void*,UINT,CWnd*);using Name=BOOL (MS_ABI*)(void*,const wchar_t*,CWnd*);
    using Global=BOOL (MS_ABI*)(void*,HGLOBAL,CWnd*);using Template=BOOL (MS_ABI*)(void*,const DLGTEMPLATE*,CWnd*,void*);
    using Modal=INT_PTR (MS_ABI*)(void*);using Init=BOOL (MS_ABI*)(void*);using Font=void (MS_ABI*)(void*,CFont*);using Empty=void (MS_ABI*)(void*);
    reinterpret_cast<ID>(table[91])(&slots,1,nullptr);check(slots.called==91,"native dialog slot 91 calls Create(UINT)");
    reinterpret_cast<Name>(table[92])(&slots,L"name",nullptr);check(slots.called==92,"native dialog slot 92 calls Create(resource name)");
    reinterpret_cast<Global>(table[93])(&slots,nullptr,nullptr);check(slots.called==93,"native dialog slot 93 calls CreateIndirect(HGLOBAL)");
    reinterpret_cast<Template>(table[94])(&slots,nullptr,nullptr,nullptr);check(slots.called==94,"native dialog slot 94 calls CreateIndirect(template,init data)");
    check(reinterpret_cast<Modal>(table[95])(&slots)==95 && slots.called==95,"native dialog slot 95 calls DoModal");
    check(reinterpret_cast<Init>(table[96])(&slots) && slots.called==96,"native dialog slot 96 calls OnInitDialog");
    reinterpret_cast<Font>(table[97])(&slots,nullptr);check(slots.called==97,"native dialog slot 97 calls OnSetFont(CFont*)");
    for(int i=98;i<=100;++i) {reinterpret_cast<Empty>(table[i])(&slots);check(slots.called==i,"native dialog slots 98..100 call OK, Cancel and PreInitDialog overrides");}
    auto* state=AfxGetThreadState();MSG previous=state->m_lastSentMsg;state->m_lastSentMsg.message=WM_SETFONT;state->m_lastSentMsg.wParam=0x5678;
    impl__OnSetFont_CDialog__IEAAXPEAVCFont__H_Z(&slots,reinterpret_cast<CFont*>(0x1234),TRUE);
    check(slots.font==reinterpret_cast<CFont*>(0x1234) && slots.defaultMessage==WM_SETFONT && slots.defaultParam==0x5678,
        "dialog font changes notify slot 97 and default-process the current sent message through virtual slot 73");state->m_lastSentMsg=previous;
    CWnd first,second;
    alignas(CDialog) BYTE rawDialog[312];memset(rawDialog,0xa5,sizeof(rawDialog));
    auto* constructed=static_cast<CDialog*>(impl___0CDialog__QEAA_IPEAVCWnd___Z(rawDialog,210,&first));
    check(constructed->GetRuntimeClass()==CDialog::GetThisClass() && constructed->m_nIDHelp==210 && constructed->m_lpszTemplateName==MAKEINTRESOURCEW(210) &&
        constructed->m_pParentWnd==&first && !constructed->m_hWnd && !constructed->m_lpDialogInit && !constructed->m_hDialogTemplate &&
        !constructed->m_lpDialogTemplate && !constructed->m_pOccDialogInfo && !constructed->m_bClosedByEndDialog &&
        *reinterpret_cast<UINT64*>(rawDialog+304)==0xa5a5a5a5a5a5a5a5ULL,"exported dialog construction initializes its real vtable and complete 304-byte native state without overwriting guards");
    impl___1CDialog__UEAA_XZ(constructed);
    CDialog retained(5,&first);DLGTEMPLATE temporary{};BYTE init[]{0,0};
    retained.InitModalIndirect(&temporary,&second,init);check(retained.m_pParentWnd==&first && retained.m_lpDialogTemplate==&temporary && retained.m_lpDialogInit==init,"InitModalIndirect stores physical template and init-data fields while preserving an existing parent");
    retained.Initialize();check(!retained.m_nIDHelp && !retained.m_lpszTemplateName && !retained.m_lpDialogTemplate && !retained.m_lpDialogInit && !retained.m_pParentWnd,"Initialize resets the complete native dialog state");
    Dialog dialog(L"NamedLayout");dialog.m_hWnd=CreateWindowExW(0,L"STATIC",L"dialog",WS_OVERLAPPEDWINDOW,0,0,250,150,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    HWND edit=CreateWindowExW(0,L"EDIT",L"",WS_CHILD,0,0,80,20,dialog.m_hWnd,reinterpret_cast<HMENU>(101),GetModuleHandleW(nullptr),nullptr);
    HWND help=CreateWindowExW(0,L"BUTTON",L"Help",WS_CHILD|WS_VISIBLE,0,22,80,20,dialog.m_hWnd,reinterpret_cast<HMENU>(0xe146),GetModuleHandleW(nullptr),nullptr);
    Probe probe;create(dialog.m_hWnd,103,probe);dialog.child=&probe;dialog.m_lpDialogInit=init;
    auto oldModule=g_cmdTestCurrentModule;auto oldThread=g_dialogHelpThread;g_cmdTestCurrentModule=nullptr;g_dialogHelpThread=nullptr;
    check(dialog.OnInitDialog() && dialog.exchanges==1 && !dialog.lastSave && !dialog.m_bClosedByEndDialog && !(GetWindowLongPtrW(help,GWL_STYLE)&WS_VISIBLE),
        "default OnInitDialog runs the load exchange and hides Help when no application handler exists");
    wchar_t text[16]{};GetWindowTextW(edit,text,16);check(!wcscmp(text,L"9"),"default dialog initialization writes member values through the user DDX override");
    HelpTarget app,main;
    alignas(CWinThread) BYTE threadPrefix[sizeof(CWinThread)]{};auto* nativeThread=reinterpret_cast<CWinThread*>(threadPrefix);
    nativeThread->m_pMainWnd=&app;nativeThread->m_pActiveWnd=&main;
    check(impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(nativeThread)==&main,"thread main-window access prefers the native active-window field at offset 72");
    nativeThread->m_pActiveWnd=nullptr;check(impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(nativeThread)==&app,"thread main-window access falls back to the native main-window field at offset 64");
    struct Module {void* table;CCmdTarget* app;} module{nullptr,&app};
    g_cmdTestCurrentModule=reinterpret_cast<AFX_MODULE_STATE*>(&module);
    void* threadTable[32]{};threadTable[31]=reinterpret_cast<void*>(&mainWindow);Thread thread{threadTable,&main};g_dialogHelpThread=reinterpret_cast<CWinThread*>(&thread);
    main.enabled=TRUE;check(dialog.OnInitDialog() && (GetWindowLongPtrW(help,GWL_STYLE)&WS_VISIBLE) && main.queries==1 && !app.queries,"Help probes the current thread's virtual main window before the application");
    main.enabled=FALSE;app.enabled=TRUE;check(dialog.OnInitDialog() && app.queries==1 && (GetWindowLongPtrW(help,GWL_STYLE)&WS_VISIBLE),"Help falls back to the application command handler");
    app.enabled=FALSE;check(dialog.OnInitDialog() && !(GetWindowLongPtrW(help,GWL_STYLE)&WS_VISIBLE),"Help remains hidden when neither target has a handler");
    g_cmdTestCurrentModule=nullptr;g_dialogHelpThread=nullptr;
    dialog.fail=TRUE;dialog.m_nFlags|=8;
    check(!dialog.OnInitDialog() && dialog.m_bClosedByEndDialog && dialog.ended==1 && dialog.lastResult==-1,"load validation failure aborts dialog initialization and ends its modal loop with -1");
    dialog.m_bClosedByEndDialog=FALSE;int ended=dialog.ended;dialog.OnOK();
    check(dialog.lastSave && !dialog.m_bClosedByEndDialog && dialog.ended==ended,"OnOK validates saved data and leaves the dialog open when validation fails");
    dialog.fail=FALSE;SetWindowTextW(edit,L"24");dialog.OnOK();
    check(dialog.number==24 && dialog.m_bClosedByEndDialog && dialog.lastResult==IDOK,"OnOK saves the edited member before ending the dialog with IDOK");
    int exchanges=dialog.exchanges;dialog.m_bClosedByEndDialog=FALSE;dialog.OnCancel();
    check(dialog.m_bClosedByEndDialog && dialog.lastResult==IDCANCEL && dialog.exchanges==exchanges,"OnCancel ends the dialog without saving control data");
    dialog.m_bClosedByEndDialog=FALSE;probe.reject=LB_ADDSTRING;std::vector<BYTE> bad;append(bad,103,LB_ADDSTRING,reinterpret_cast<const BYTE*>("no"),3);finish(bad);dialog.m_lpDialogInit=bad.data();exchanges=dialog.exchanges;
    check(!dialog.OnInitDialog() && dialog.m_bClosedByEndDialog && dialog.lastResult==-1 && dialog.exchanges==exchanges,"DLGINIT failure ends initialization before the data exchange handler runs");
    g_cmdTestCurrentModule=oldModule;g_dialogHelpThread=oldThread;DestroyWindow(dialog.m_hWnd);dialog.m_hWnd=nullptr;
}
}
static void dlgInit() {dlginit_test::storage();dlginit_test::records();dlginit_test::dialogs();}
