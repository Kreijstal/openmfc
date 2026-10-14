#pragma once
#include "detail/DatabaseEnvironment.h"
extern "C" {
extern decltype(&::CryptProtectData) __imp_CryptProtectData;
extern decltype(&::CryptUnprotectData) __imp_CryptUnprotectData;
void MS_ABI impl__GetConnectInfo_CDatabase__IEAAXXZ(CDatabase*);
void MS_ABI impl__VerifyConnect_CDatabase__IEAAXXZ(CDatabase*);
BOOL MS_ABI impl__Connect_CDatabase__IEAAHK_Z(CDatabase*,DWORD);
BOOL MS_ABI impl__OpenEx_CDatabase__UEAAHPEB_WK_Z(CDatabase*,const wchar_t*,DWORD);
BOOL MS_ABI impl__Open_CDatabase__UEAAHPEB_WHH0H_Z(CDatabase*,const wchar_t*,BOOL,BOOL,const wchar_t*,BOOL);
HWND MS_ABI impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z(HWND,HWND*);
}
namespace database_connect_test {
struct API {
    decltype(&impl__GetConnectInfo_CDatabase__IEAAXXZ) info;
    decltype(&impl__VerifyConnect_CDatabase__IEAAXXZ) verify;
    decltype(&impl__Connect_CDatabase__IEAAHK_Z) connect;
    decltype(&impl__OpenEx_CDatabase__UEAAHPEB_WK_Z) openEx;
    decltype(&impl__Open_CDatabase__UEAAHPEB_WHH0H_Z) open;
    decltype(&impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z) owner;
    decltype(&impl__Free_CDatabase__IEAAXXZ) free;
    decltype(&impl__Close_CDatabase__UEAAXXZ) close;
};
static const API* active;
struct Info {WORD kind;DWORD value;const wchar_t* text;SHORT length;RETCODE result=SQL_SUCCESS;BOOL accepted=TRUE;};
static std::vector<Info> infoRows;
static size_t infoPosition;
static std::vector<std::wstring> events;
static BOOL lastAccepted,driverAccepted;
static bool infoPending,protectOK,decryptOK,frameworkFailure;
static RETCODE driverResult;
static wchar_t* driverOutput;
static const wchar_t* driverText=L"DSN=returned;UID=fixture";
static CDatabase* current;
static HWND root,popup,child;
static void event(const std::wstring& name,INT64 a=0,INT64 b=0,INT64 c=0) {
    events.push_back(name+L":"+std::to_wstring(a)+L","+std::to_wstring(b)+L","+std::to_wstring(c));
}
static void defaults() {
    infoRows={{79,0x1c,nullptr,4},{80,3,nullptr,4},{46,2,nullptr,2},{23,1,nullptr,2},{24,2,nullptr,2},
        {82,0x87654321,nullptr,4},{81,8,nullptr,4},{25,0,L"N",2},{29,0,L"`",2},{9,1,nullptr,2},{15,0,nullptr,2}};
    infoPosition=0;events.clear();lastAccepted=driverAccepted=TRUE;infoPending=false;protectOK=decryptOK=true;frameworkFailure=false;
    driverResult=SQL_SUCCESS;driverOutput=nullptr;
}
static Info& row(WORD kind) {return *std::find_if(infoRows.begin(),infoRows.end(),[&](const Info& value){return value.kind==kind;});}
static SQLRETURN SQL_API info(SQLHDBC connection,SQLUSMALLINT kind,SQLPOINTER output,SQLSMALLINT capacity,SQLSMALLINT* length) {
    auto found=std::find_if(infoRows.begin(),infoRows.end(),[&](const Info& value){return value.kind==kind;});
    if(found==infoRows.end())std::abort();auto& value=*found;lastAccepted=value.accepted;infoPending=true;++infoPosition;driverOutput=nullptr;
    event(L"INFO",kind,capacity,UINT_PTR(connection));*length=value.length;
    if(value.text) {std::wmemset(static_cast<wchar_t*>(output),0,capacity/2);std::wmemcpy(static_cast<wchar_t*>(output),value.text,std::min<size_t>(std::wcslen(value.text),capacity/2));}
    else std::memcpy(output,&value.value,std::min<size_t>(capacity,sizeof(value.value)));
    return value.result;
}
static SQLRETURN SQL_API attribute(SQLHDBC connection,SQLINTEGER kind,SQLPOINTER value,SQLINTEGER size) {
    event(L"ATTR",kind,UINT_PTR(value),size);return SQL_ERROR;
}
static BOOL MS_ABI validate(const CDatabase*,RETCODE code) {
    BOOL accepted=infoPending?lastAccepted:driverOutput?driverAccepted:TRUE;
    event(L"CHECK",code,::IsWindowEnabled(root),accepted);
    infoPending=false;return accepted;
}
static void MS_ABI throwError(CDatabase*,RETCODE code) {
    bool wiped=true;if(driverOutput)for(size_t i=0;i<1024;++i)wiped&=driverOutput[i]==0;
    event(L"THROW",code,wiped,::IsWindowEnabled(root));
    if(frameworkFailure)throw new CDBException(code);
    throw int(code)+2000;
}
static BOOL WINAPI protect(DATA_BLOB* input,LPCWSTR description,DATA_BLOB* entropy,PVOID reserved,CRYPTPROTECT_PROMPTSTRUCT* prompt,DWORD flags,DATA_BLOB* output) {
    check(!description && !entropy && !reserved && !prompt && !flags && output==&current->m_blobConnect,
        "database protection uses native DPAPI arguments and the native blob destination");
    event(L"PROTECT",input->cbData,protectOK);events.emplace_back(reinterpret_cast<wchar_t*>(input->pbData));
    if(!protectOK)return FALSE;
    output->cbData=input->cbData;output->pbData=static_cast<BYTE*>(::LocalAlloc(LMEM_FIXED,input->cbData));std::memcpy(output->pbData,input->pbData,input->cbData);return TRUE;
}
static BOOL WINAPI decrypt(DATA_BLOB* input,LPWSTR* description,DATA_BLOB* entropy,PVOID reserved,CRYPTPROTECT_PROMPTSTRUCT* prompt,DWORD flags,DATA_BLOB* output) {
    check(input==&current->m_blobConnect && !description && !entropy && !reserved && !prompt && !flags,
        "database decryption consumes its native encrypted blob with unchanged DPAPI flags");
    event(L"DECRYPT",input->cbData,decryptOK);
    if(!decryptOK || !input->pbData)return FALSE;
    output->cbData=input->cbData;output->pbData=static_cast<BYTE*>(::LocalAlloc(LMEM_FIXED,input->cbData));std::memcpy(output->pbData,input->pbData,input->cbData);return TRUE;
}
static BOOL MS_ABI forwarded(CDatabase*,const wchar_t* text,DWORD options) {event(L"FORWARD",options);events.emplace_back(text);return 71;}
static void MS_ABI closeDatabase(CDatabase* database) {event(L"CLOSE",UINT_PTR(database->m_hdbc));active->close(database);}
static void save(std::vector<std::wstring>& result,CDatabase& database) {
    result.insert(result.end(),events.begin(),events.end());result.emplace_back(database.m_strConnect.GetString());
    if(database.m_blobConnect.pbData)result.emplace_back(reinterpret_cast<wchar_t*>(database.m_blobConnect.pbData));else result.emplace_back(L"<no blob>");
    for(INT64 value:{INT64(database.m_dwUpdateOptions),INT64(database.m_bTransactions),INT64(database.m_nTransactionCapable),INT64(database.m_nCursorCommitBehavior),
        INT64(database.m_nCursorRollbackBehavior),INT64(database.m_dwBookmarkAttributes),INT64(database.m_bUpdatable),INT64(database.m_chIDQuoteChar)})result.push_back(std::to_wstring(value));
}
std::vector<std::wstring> exercise(const API& api,_AFX_THREAD_STATE* state,HENV& environment,LONG& connections) {
    CDatabase database;current=&database;active=&api;auto* previousRouting=state->m_pRoutingFrame;
    alignas(CWnd) BYTE frame[sizeof(CWnd)]{};auto* window=reinterpret_cast<CWnd*>(frame);window->m_hWnd=root;
    state->m_pRoutingFrame=reinterpret_cast<CFrameWnd*>(window);
    auto** original=*reinterpret_cast<void***>(&database);void* table[12];std::memcpy(table,original,sizeof(table));
    table[7]=reinterpret_cast<void*>(&closeDatabase);table[10]=reinterpret_cast<void*>(&validate);table[11]=reinterpret_cast<void*>(&throwError);*reinterpret_cast<void***>(&database)=table;
    std::vector<std::wstring> result;
    for(bool updatable:{false,true})for(int reject=-1;reject<9;++reject) {
        defaults();if(reject>=0){infoRows[reject].result=SQL_ERROR;infoRows[reject].accepted=FALSE;}
        database.m_hdbc=reinterpret_cast<HDBC>(0xdef);database.m_dwUpdateOptions=0xff;database.m_bTransactions=9;database.m_nTransactionCapable=0x5555;
        database.m_nCursorCommitBehavior=12;database.m_nCursorRollbackBehavior=13;database.m_dwBookmarkAttributes=0xaaaa;database.m_bUpdatable=updatable?7:0;
        database.m_chIDQuoteChar=L'!';api.info(&database);
        check(database.m_nTransactionCapable==0x5555 && database.m_bTransactions==(reject==2?9:1) && infoPosition==(updatable?9:8),
            "database discovery preserves native transaction capability storage, cached flags and query counts");save(result,database);
    }
    for(const auto& text:std::vector<Info>{{25,0,L"Y",2},{25,0,L"N",0},{25,0,L"N",4},{29,0,L"[",2},{29,0,L"`",0},{29,0,L"",2}}) {
        defaults();row(text.kind)=text;database.m_bUpdatable=TRUE;api.info(&database);save(result,database);
    }
    for(WORD kind:{WORD(79),WORD(80),WORD(81),WORD(46)}) {
        defaults();row(kind).value=0;database.m_bTransactions=9;database.m_bUpdatable=TRUE;api.info(&database);save(result,database);
    }
    for(WORD kind:{WORD(9),WORD(15)})for(bool rejected:{false,true}) {
        defaults();row(kind).value=DWORD(-1);if(kind==9)row(kind).value=0;
        if(rejected){row(kind).accepted=FALSE;row(kind).result=SQL_ERROR;}
        int error=0;try {api.verify(&database);}catch(int value){error=value;}
        check(error==(rejected?1999:kind==9?3014:3015),"database verification throws through its virtual callback for ODBC errors and insufficient conformance");save(result,database);
    }
    defaults();row(9).value=1;row(15).value=0;api.verify(&database);check(infoPosition==2,"database verification queries both native conformance levels");save(result,database);
    for(HWND parent:{root,child,popup})for(bool enabled:{false,true}) {
        ::EnableWindow(root,enabled);HWND disabled=reinterpret_cast<HWND>(1);HWND owner=api.owner(parent,&disabled);
        check(owner==(parent==child?root:parent) && disabled==(parent==popup && enabled?root:nullptr),
            "safe modal ownership climbs child windows and disables only a distinct enabled top-level owner");
        event(L"OWNER",owner==root?1:2,disabled==root,::IsWindowEnabled(root));if(disabled)::EnableWindow(disabled,TRUE);save(result,database);
    }
    for(DWORD options:{DWORD(0),DWORD(8),DWORD(16),DWORD(24)})for(bool encrypted:{false,true})for(bool success:{false,true}) {
        defaults();::EnableWindow(root,TRUE);::SetActiveWindow(popup);protectOK=success;database.m_strConnect=L"DSN=input";
        if(encrypted) {
            database.m_blobConnect.cbData=20;database.m_blobConnect.pbData=static_cast<BYTE*>(::LocalAlloc(LMEM_FIXED,20));std::memcpy(database.m_blobConnect.pbData,L"DSN=input",20);database.m_strConnect.Empty();
        }
        BOOL connected=api.connect(&database,options);
        check(connected && ::IsWindowEnabled(root) && (success?database.m_strConnect.IsEmpty() && database.m_blobConnect.pbData:database.m_strConnect==L"ODBC;DSN=returned;UID=fixture"),
            "database connection selects its prompt mode, restores modal ownership and stores the returned connection with native protection behavior");save(result,database);
        if(database.m_blobConnect.pbData){::LocalFree(database.m_blobConnect.pbData);database.m_blobConnect.pbData=nullptr;}
    }
    defaults();decryptOK=false;protectOK=false;database.m_strConnect.Empty();api.connect(&database,0);save(result,database);
    for(int references:{-1,1,2})for(bool protection:{false,true}) {
        defaults();protectOK=protection;database_strings_test::Manager manager;
        auto* owned=manager.Allocate(100,2);owned->nDataLength=9;owned->nRefs=references;std::memcpy(owned+1,L"DSN=input",20);
        auto* previous=database_strings_test::data(database.m_strConnect);database_strings_test::attach(database.m_strConnect,owned);
        api.connect(&database,0);
        auto* final=database_strings_test::data(database.m_strConnect);
        check(references<0?final==owned && owned->nRefs==-1:final->pStringMgr==&manager,
            "database connection writes preserve locked storage and the native CString manager");
        if(references<0 && protection) {
            bool wiped=true;auto* text=reinterpret_cast<wchar_t*>(owned+1);
            for(size_t i=0;i<std::wcslen(L"ODBC;DSN=returned;UID=fixture");++i)wiped&=text[i]==0;
            check(wiped,"database protection wipes locked plaintext before emptying the CString");
        }
        save(result,database);result.push_back(std::to_wstring(manager.allocations));result.push_back(std::to_wstring(manager.reallocations));result.push_back(std::to_wstring(manager.clones));
        if(references<0) {owned->nRefs=1;manager.Free(owned);}
        else if(final->Release())manager.Free(final);
        if(references>1){owned->nRefs=1;manager.Free(owned);}
        database_strings_test::attach(database.m_strConnect,previous);
        if(database.m_blobConnect.pbData){::LocalFree(database.m_blobConnect.pbData);database.m_blobConnect.pbData=nullptr;}
    }
    for(RETCODE status:{RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA)}) {
        defaults();::EnableWindow(root,TRUE);::SetActiveWindow(popup);driverResult=status;driverAccepted=FALSE;database.m_strConnect=L"DSN=input";
        // Cancel's Free path owns the shared connection count. This standalone
        // Connect probe uses no allocated connection handle.
        if(status==SQL_NO_DATA)database.m_hdbc=nullptr;
        int error=0;BOOL returned=TRUE;try {returned=api.connect(&database,0);}catch(int value){error=value;}
        check(::IsWindowEnabled(root) && (status==SQL_ERROR?error==1999:!returned),"database connection errors restore the modal owner; cancellation returns FALSE after cleanup");save(result,database);
    }
    for(const wchar_t* connection:{static_cast<const wchar_t*>(nullptr),L"ODBC;",L"odbc;UID=fixture",L"OTHER;"})for(const wchar_t* dsn:{static_cast<const wchar_t*>(nullptr),L"",L"fixture"}) {
        defaults();table[6]=reinterpret_cast<void*>(&forwarded);
        BOOL returned=api.open(&database,dsn,17,31,connection,19);
        check(returned==71,"database Open forwards normalized options and assembled DSN text through virtual OpenEx");save(result,database);
    }
    table[6]=original[6];database.m_hdbc=nullptr;
    auto savedEnvironment=environment;LONG savedConnections=connections;environment=nullptr;connections=0;
    for(DWORD options:{DWORD(0),DWORD(2),DWORD(4),DWORD(8),DWORD(16)})for(bool encrypted:{false,true}) {
        defaults();database_lifecycle_test::reset();protectOK=encrypted;database.m_blobConnect.cbData=0;
        ::EnableWindow(root,TRUE);::SetActiveWindow(popup);database.m_bTransactions=FALSE;database.m_nTransactionCapable=0x5555;
        BOOL opened=api.openEx(&database,L"DSN=input",options);
        check(opened && database.m_hdbc==reinterpret_cast<HDBC>(0xdef) && connections==1 && environment==reinterpret_cast<HENV>(0xabc) &&
            database.m_bTransactions==TRUE && database.m_dwUpdateOptions==7 && database.m_bUpdatable==((options&2)?FALSE:TRUE),
            "database OpenEx allocates, connects, verifies and populates native driver capabilities");save(result,database);
        api.free(&database);check(!database.m_hdbc && !environment && !connections,"database connection cleanup after OpenEx releases its native handles and connection count");
    }
    for(int failure:{0,1,2,3}) {
        defaults();database_lifecycle_test::reset();frameworkFailure=true;database.m_blobConnect.cbData=0;
        ::EnableWindow(root,TRUE);::SetActiveWindow(popup);
        if(failure==0) {driverResult=SQL_ERROR;driverAccepted=FALSE;}
        if(failure==1)row(9).value=0;
        if(failure==2)row(15).value=DWORD(-1);
        if(failure==3)driverResult=SQL_NO_DATA;
        bool caught=false;BOOL opened=TRUE;RETCODE code=0;
        try {opened=api.openEx(&database,L"DSN=input",0);}catch(CDBException* error){caught=true;code=error->m_nRetCode;error->Delete();}
        check(!database.m_hdbc && !environment && !connections && !database.m_blobConnect.pbData && ::IsWindowEnabled(root) &&
            (failure==3?!opened && !caught:caught && code==(failure==0?SQL_ERROR:failure==1?1014:1015)),
            "database OpenEx cancels cleanly and frees all connection resources before rethrowing framework failures");save(result,database);
    }
    defaults();bool invalid=false;
    try {api.openEx(&database,L"DSN=input",24);}catch(int value){invalid=value==19;}catch(CException* error){invalid=true;error->Delete();}
    check(invalid && events.empty() && !database.m_hdbc,"database OpenEx rejects conflicting prompt flags before crypto or ODBC calls");
    environment=savedEnvironment;connections=savedConnections;database.m_hdbc=nullptr;database.m_strConnect.Empty();
    *reinterpret_cast<void***>(&database)=original;state->m_pRoutingFrame=previousRouting;current=nullptr;active=nullptr;return result;
}
}
extern "C" SQLRETURN SQL_API SQLDriverConnectW(SQLHDBC connection,SQLHWND owner,SQLWCHAR* input,SQLSMALLINT inputLength,SQLWCHAR* output,SQLSMALLINT capacity,SQLSMALLINT* outputLength,SQLUSMALLINT completion) {
    using namespace database_connect_test;
    event(L"DRIVER",completion,inputLength,capacity);event(L"MODAL",owner==popup,::IsWindowEnabled(root),UINT_PTR(connection));events.emplace_back(reinterpret_cast<wchar_t*>(input));
    driverOutput=reinterpret_cast<wchar_t*>(output);std::wmemset(driverOutput,L'!',capacity);
    std::wcscpy(driverOutput,driverText);*outputLength=SHORT(std::wcslen(driverText));return driverResult;
}
static void databaseConnect() {
    using namespace database_connect_test;
    root=::CreateWindowExW(0,L"STATIC",L"Database owner",WS_OVERLAPPEDWINDOW,0,0,200,100,nullptr,nullptr,::GetModuleHandleW(nullptr),nullptr);
    popup=::CreateWindowExW(0,L"STATIC",L"Database popup",WS_POPUP,0,0,100,50,root,nullptr,::GetModuleHandleW(nullptr),nullptr);
    child=::CreateWindowExW(0,L"STATIC",L"Database child",WS_CHILD,0,0,50,25,root,nullptr,::GetModuleHandleW(nullptr),nullptr);
    check(root && popup && child,"create real windows for native database modal ownership probes");::ShowWindow(root,SW_SHOWNA);::ShowWindow(popup,SW_SHOWNA);
    auto previousInfo=database_strings_test::infoHook;database_strings_test::infoHook=&info;
    auto previousAttr=database_lifecycle_test::attributeHook;database_lifecycle_test::attributeHook=&attribute;
    window_input_tests::Import protectedImport(__imp_CryptProtectData,&protect);window_input_tests::Import decryptedImport(__imp_CryptUnprotectData,&decrypt);
    API local{&impl__GetConnectInfo_CDatabase__IEAAXXZ,&impl__VerifyConnect_CDatabase__IEAAXXZ,&impl__Connect_CDatabase__IEAAHK_Z,
        &impl__OpenEx_CDatabase__UEAAHPEB_WK_Z,&impl__Open_CDatabase__UEAAHPEB_WHH0H_Z,&impl__GetSafeOwner__CWnd__SAPEAUHWND____PEAU2_PEAPEAU2__Z,&impl__Free_CDatabase__IEAAXXZ,&impl__Close_CDatabase__UEAAXXZ};
    auto expected=exercise(local,AfxGetThreadState(),OpenMfcDatabaseEnvironment(),OpenMfcAllocatedDatabaseConnections());
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_CONNECT_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for database connection comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"database connection oracle uses the verified MFC image build");
            if(pinned) {
                struct Patch {size_t rva;void* function;void* saved;DWORD protection;};
                Patch patches[]={{0x3e9078,reinterpret_cast<void*>(&SQLGetInfoW)},{0x3e9030,reinterpret_cast<void*>(&SQLSetConnectAttr)},
                    {0x3e9118,reinterpret_cast<void*>(&SQLDriverConnectW)},{0x3e9000,reinterpret_cast<void*>(&protect)},{0x3e9008,reinterpret_cast<void*>(&decrypt)},
                    {0x3e90e8,reinterpret_cast<void*>(&SQLAllocEnv)},{0x3e9110,reinterpret_cast<void*>(&SQLAllocConnect)},
                    {0x3e9098,reinterpret_cast<void*>(&SQLDisconnect)},{0x3e9090,reinterpret_cast<void*>(&SQLFreeConnect)},{0x3e9088,reinterpret_cast<void*>(&SQLFreeEnv)}};
                for(auto& patch:patches){auto** slot=reinterpret_cast<void**>(base+patch.rva);patch.saved=*slot;::VirtualProtect(slot,8,PAGE_READWRITE,&patch.protection);*slot=patch.function;}
                API native{reinterpret_cast<decltype(local.info)>(base+0x1f7b40),reinterpret_cast<decltype(local.verify)>(base+0x1f7a50),
                    reinterpret_cast<decltype(local.connect)>(base+0x1f77f0),reinterpret_cast<decltype(local.openEx)>(base+0x1f6dd0),
                    reinterpret_cast<decltype(local.open)>(base+0x1f6c30),reinterpret_cast<decltype(local.owner)>(base+0x1ced10),reinterpret_cast<decltype(local.free)>(base+0x1f7160),reinterpret_cast<decltype(local.close)>(base+0x1f70a0)};
                auto* state=reinterpret_cast<_AFX_THREAD_STATE* (MS_ABI*)()>(base+0x1332a0)();
                reinterpret_cast<HENV (MS_ABI*)()>(base+0x1f6ae0)();auto* process=*reinterpret_cast<BYTE**>(base+0x3c4160);
                check(exercise(native,state,*reinterpret_cast<HENV*>(process+8),*reinterpret_cast<LONG*>(process+16))==expected,
                    "database capabilities, driver calls, encrypted strings, conformance errors, modal ownership and Open forwarding match native MFC");
                for(auto& patch:patches){auto** slot=reinterpret_cast<void**>(base+patch.rva);*slot=patch.saved;DWORD ignored;::VirtualProtect(slot,8,patch.protection,&ignored);}
            }
            ::FreeLibrary(module);
        }
    }
    database_strings_test::infoHook=previousInfo;database_lifecycle_test::attributeHook=previousAttr;
    ::DestroyWindow(child);::DestroyWindow(popup);::DestroyWindow(root);root=popup=child=nullptr;
}
