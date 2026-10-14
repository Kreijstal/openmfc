#pragma once
#include "detail/DatabaseEnvironment.h"
extern "C" CDBException* MS_ABI impl___0CDBException__QEAA_F_Z(void*,RETCODE);
extern "C" void MS_ABI impl___1CDBException__UEAA_XZ(CDBException*);
extern "C" void MS_ABI impl__BuildErrorString_CDBException__UEAAXPEAVCDatabase__PEAXH_Z(CDBException*,CDatabase*,HSTMT,BOOL);
extern "C" BOOL MS_ABI impl__GetErrorMessage_CDBException__UEBAHPEA_WIPEAI_Z(const CDBException*,wchar_t*,UINT,UINT*);
extern "C" void MS_ABI impl__Empty_CDBException__QEAAXXZ(CDBException*);
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE,CDatabase*,HSTMT);
extern "C" void* MS_ABI impl__AfxGetHENV__YAPEAXXZ();
namespace database_exception_test {
static void __cdecl invalidParameter(const wchar_t*,const wchar_t*,const wchar_t*,unsigned,uintptr_t) {}
struct API {
    decltype(&impl___0CDBException__QEAA_F_Z) construct;
    decltype(&impl___1CDBException__UEAA_XZ) destroy;
    decltype(&impl__BuildErrorString_CDBException__UEAAXPEAVCDatabase__PEAXH_Z) build;
    decltype(&impl__GetErrorMessage_CDBException__UEBAHPEA_WIPEAI_Z) message;
    decltype(&impl__Empty_CDBException__QEAAXXZ) empty;
    decltype(&impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z) raise;
};
std::vector<std::wstring> exercise(const API& api) {
    using namespace longbinary_rfx_test;
    alignas(CDBException) BYTE storage[48];memset(storage,0xa5,sizeof(storage));
    auto* error=api.construct(storage,-123);
    check(sizeof(CDBException)==40 && error==reinterpret_cast<CDBException*>(storage) && error->m_nRetCode==-123 &&
        error->m_strError.IsEmpty() && error->m_strStateNativeOrigin.IsEmpty() &&
        *reinterpret_cast<UINT64*>(storage+40)==0xa5a5a5a5a5a5a5a5ULL,
        "database exception constructor fits the native 40-byte allocation and initializes code and two strings");
    check(*reinterpret_cast<DWORD*>(storage+12)==0xa5a5a5a5 && storage[18]==0xa5 && storage[23]==0xa5,
        "database exception constructor preserves native base and return-code padding");
    auto* runtime=error->GetRuntimeClass();
    check(!strcmp(runtime->m_lpszClassName,"CDBException") && runtime->m_nObjectSize==40 &&
        !strcmp(runtime->BaseClass()->m_lpszClassName,"CException") && !runtime->m_pfnCreateObject,
        "database exceptions expose their own native dynamic runtime identity and exception base");
    CDatabase database;database.m_hdbc=reinterpret_cast<HDBC>(0xfeed);auto statement=reinterpret_cast<HSTMT>(0xcafe);
    std::vector<std::wstring> result;
    for(const auto& rows:std::vector<std::vector<Diagnostic>>{
        {{L"HY000",-7,L"[Driver][Source] failure"},{L"01004",42,L"[Driver] truncated",SQL_SUCCESS_WITH_INFO}},
        {{L"01000",0,L"warning"}},{{L"HY001",1,L"[unterminated"}},
        {{L"01000",0,L"[A]  two spaces"}},{{L"01000",0,L"[A] [B]tail"}},
        {{L"01000",0,L"[A]"}},{{L"01000",0,L""}},
        {{L"00000",0,L"discard"},{L"HY000",1,L"unread"}},
        {{L"HY000",1,L"discard",SQL_ERROR},{L"HY000",2,L"unread"}},{}
    }) {
        error->m_strError=L"prefix:";error->m_strStateNativeOrigin=L"states:";
        reset({},rows);api.build(error,&database,statement,FALSE);
        result.emplace_back(static_cast<const wchar_t*>(error->m_strError));result.emplace_back(static_cast<const wchar_t*>(error->m_strStateNativeOrigin));
        result.push_back(std::to_wstring(diagCalls));
        check(diagCalls>0 && diagnosticReads.front().environment==nullptr && diagnosticReads.front().database==database.m_hdbc &&
            diagnosticReads.front().statement==statement && diagnosticReads.front().capacity==511,
            "database diagnostics use the process environment and preserve database, statement and native buffer capacity");
    }
    check(result[0]==L"prefix:failure\ntruncated\n" && result[1]==L"states:State:HY000,Native:-7,Origin:[Driver][Source]\nState:01004,Native:42,Origin:[Driver]\n" && result[2]==L"3",
        "database diagnostics drain all records, strip leading origins and append native state, code and message text");
    reset({},{{L"HY000",1,L"unread"}});auto old=error->m_strError;api.build(error,nullptr,statement,TRUE);
    check(!diagCalls && error->m_strError==old,"database diagnostics with a null database leave strings and ODBC stream untouched");
    error->m_strError=L"message";error->m_strStateNativeOrigin=L"state";api.empty(error);
    check(error->m_nRetCode==-123 && error->m_strError.IsEmpty() && error->m_strStateNativeOrigin.IsEmpty(),
        "database exception Empty clears both strings while preserving the original ODBC return code");
    for(const wchar_t* text:{L"",L"abcdef",L"\x7de8\x96c6"})for(UINT capacity:{1u,3u,9u}) {
        wchar_t output[12];std::wmemset(output,L'!',12);UINT help=99;error->m_strError=text;
        BOOL copied=api.message(error,output,capacity,&help);
        check(copied && !help && output[capacity-1]==0 && output[capacity]==L'!',
            "database exception messages return TRUE, bound and terminate the copy and clear the help context");
        result.emplace_back(output);
    }
    UINT help=99;check(!api.message(error,nullptr,9,&help) && help==99,
        "database exception null message destinations preserve the help output");
    error->m_strError=L"abc";wchar_t output[1]={L'!'};bool invalid=false;
    // Secure CRT calls notify the invalid-parameter handler before MFC turns
    // their errno into an exception. Install a returning handler in both CRTs.
    auto previousHandler=::_set_thread_local_invalid_parameter_handler(&invalidParameter);
    HMODULE crt=::LoadLibraryW(L"ucrtbase.dll");
    using SetHandler=_invalid_parameter_handler (__cdecl*)(_invalid_parameter_handler);
    auto setHandler=crt?reinterpret_cast<SetHandler>(::GetProcAddress(crt,"_set_thread_local_invalid_parameter_handler")):nullptr;
    auto previousNativeHandler=setHandler?setHandler(&invalidParameter):nullptr;
    try {api.message(error,output,0,&help);}catch(int code){invalid=code==19;}catch(CException* exception){invalid=true;exception->Delete();}
    if(setHandler)setHandler(previousNativeHandler);if(crt)::FreeLibrary(crt);
    ::_set_thread_local_invalid_parameter_handler(previousHandler);
    check(invalid && !help,"database exception zero-capacity copies throw the framework invalid-argument exception");
    auto** table=*reinterpret_cast<void***>(error);
    using Build=void (MS_ABI*)(CDBException*,CDatabase*,HSTMT,BOOL);
    reset({},{{L"HY000",3,L"[D] virtual"}});error->Empty();reinterpret_cast<Build>(table[8])(error,&database,statement,TRUE);
    check(error->m_strError==L"virtual\n","database diagnostic construction occupies native virtual slot eight");
    api.destroy(error);
    check(*reinterpret_cast<UINT64*>(storage+40)==0xa5a5a5a5a5a5a5a5ULL,"database exception destruction preserves the caller allocation guard");
    for(RETCODE code:{RETCODE(SQL_ERROR),RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(1000),RETCODE(1029)}) {
        reset({},{{L"HY000",3,L"[D] thrown"}});bool caught=false;
        try {api.raise(code,&database,statement);}catch(CDBException* thrown) {
            caught=thrown->m_nRetCode==code && (code==SQL_ERROR?thrown->m_strError==L"thrown\n":thrown->m_strError.IsEmpty());
            result.emplace_back(static_cast<const wchar_t*>(thrown->m_strError));thrown->Delete();
        }
        check(caught && diagCalls==(code==SQL_ERROR?2:0),"database throw helper constructs diagnostics only for SQL_ERROR and throws a deletable typed pointer");
    }
    reset({},{{L"HY000",3,L"unread"}});bool caught=false;
    try {api.raise(SQL_ERROR,nullptr,statement);}catch(CDBException* thrown){caught=thrown->m_strError.IsEmpty();thrown->Delete();}
    check(caught && !diagCalls,"database SQL_ERROR throwing without a database preserves an empty diagnostic and skips ODBC");
    database.m_hdbc=nullptr;reset({});return result;
}
std::wstring internalError(const API& api,RETCODE code) {
    std::wstring text;
    try {api.raise(code,nullptr,nullptr);}catch(CDBException* error){text=static_cast<const wchar_t*>(error->m_strError);error->Delete();}
    return text;
}
}
static void databaseExceptions() {
    using namespace database_exception_test;
    API local{&impl___0CDBException__QEAA_F_Z,&impl___1CDBException__UEAA_XZ,&impl__BuildErrorString_CDBException__UEAAXPEAVCDatabase__PEAXH_Z,
        &impl__GetErrorMessage_CDBException__UEBAHPEA_WIPEAI_Z,&impl__Empty_CDBException__QEAAXXZ,&impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z};
    check(!impl__AfxGetHENV__YAPEAXXZ(),"database environment lookup does not allocate a connection environment");
    auto expected=exercise(local);
    HMODULE fixture=::LoadLibraryExW(L"database_exception_fixture.dll",nullptr,LOAD_LIBRARY_AS_DATAFILE);
    check(fixture!=nullptr,"load isolated database error string fixture");
    auto resourceModule=g_d2dTestResourceModule;g_d2dTestResourceModule=fixture;
    check(internalError(local,1001)==L"Fixture database connection failure" && internalError(local,1002)==L"MFC internal error: unable to load error string from resource.",
        "database internal error codes map to framework string resources and preserve the native missing-resource fallback");
    g_d2dTestResourceModule=resourceModule;if(fixture)::FreeLibrary(fixture);
    auto savedEnvironment=OpenMfcDatabaseEnvironment();OpenMfcDatabaseEnvironment()=reinterpret_cast<HENV>(0x1234);
    CDatabase database;CDBException error;
    longbinary_rfx_test::reset({},{{L"HY000",1,L"environment"}});local.build(&error,&database,nullptr,TRUE);
    check(impl__AfxGetHENV__YAPEAXXZ()==reinterpret_cast<HENV>(0x1234) && longbinary_rfx_test::diagnosticReads.front().environment==reinterpret_cast<HENV>(0x1234),
        "database environment lookup and diagnostic construction share the process environment handle");
    OpenMfcDatabaseEnvironment()=savedEnvironment;longbinary_rfx_test::reset({});
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DB_EXCEPTION_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for database exception comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);
            auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);
            auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;
            check(pinned,"native database exception oracle uses the verified MFC image build");
            if(!pinned) {::FreeLibrary(module);return;}
            auto** slot=reinterpret_cast<void**>(base+0x3e90e0);auto* saved=*slot;DWORD protection;
            ::VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=reinterpret_cast<void*>(&SQLErrorW);
            API native{reinterpret_cast<decltype(local.construct)>(base+0x1f6540),reinterpret_cast<decltype(local.destroy)>(base+0x1f6610),
                reinterpret_cast<decltype(local.build)>(base+0x1f6680),reinterpret_cast<decltype(local.message)>(base+0x1f6a50),
                reinterpret_cast<decltype(local.empty)>(base+0x1f6ac0),reinterpret_cast<decltype(local.raise)>(base+0x1f6490)};
            check(exercise(native)==expected,"database exception layout, diagnostic streams, messages and typed throws match native MFC");
            auto previous=g_d2dTestResourceModule;g_d2dTestResourceModule=module;bool same=true;
            for(RETCODE code=1001;code<=1028;++code) {
                auto ours=internalError(local,code),theirs=internalError(native,code);
                same&=ours==theirs;
            }
            check(same,"all 28 internal database error codes select the same native framework resources");g_d2dTestResourceModule=previous;
            *slot=saved;DWORD ignored;::VirtualProtect(slot,8,protection,&ignored);::FreeLibrary(module);
        }
    }
}
