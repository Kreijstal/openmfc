#pragma once
#include "detail/CPtrListSupport.h"
extern "C" void MS_ABI impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z(CDatabase*,wchar_t*);
namespace database_layout_test {
static int optionCalls,checkCalls,throwCalls;
static HSTMT optionStatement,throwStatement;
static CDatabase* throwDatabase;
static RETCODE optionResult,throwCode;
static SQLUSMALLINT option;
static SQLULEN timeout;
static bool accepted;
static BOOL MS_ABI checkOption(const CDatabase*,RETCODE code) {++checkCalls;return accepted;}
}
// Observe the ODBC and framework exception boundaries; the database methods
// and its constructor are the actual production implementations.
extern "C" SQLRETURN SQL_API SQLSetStmtOption(SQLHSTMT statement,SQLUSMALLINT which,SQLULEN value) {
    using namespace database_layout_test;
    ++optionCalls;optionStatement=statement;option=which;timeout=value;return optionResult;
}
extern "C" void MS_ABI impl__AfxThrowDBException__YAXFPEAVCDatabase__PEAX_Z(RETCODE code,CDatabase* database,HSTMT statement) {
    using namespace database_layout_test;
    ++throwCalls;throwCode=code;throwDatabase=database;throwStatement=statement;throw int(code)+900;
}
namespace database_layout_test {
using Constructor=void* (MS_ABI*)(void*);
using Brackets=void (MS_ABI*)(CDatabase*,wchar_t*);
std::vector<BYTE> construct(Constructor constructor,bool native) {
    alignas(CDatabase) BYTE storage[168];memset(storage,0xa5,sizeof(storage));
    auto* database=static_cast<CDatabase*>(constructor(storage));
    check(database==reinterpret_cast<CDatabase*>(storage) && sizeof(CDatabase)==160,"database constructor fits the native 160-byte allocation");
    check(!database->m_hdbc && !database->m_hstmt && !database->m_bStripTrailingSpaces && !database->m_bIncRecordCountOnAdd &&
        !database->m_bAddForUpdate && !database->m_bUpdatable && !database->m_bTransactions && database->m_dwLoginTimeout==15 && database->m_dwQueryTimeout==15 &&
        !database->m_blobConnect.pbData && database->m_strConnect.IsEmpty(),"database constructor initializes native flags, handles, string and per-instance timeouts");
    auto* list=reinterpret_cast<OpenMFC_CPtrList*>(database->m_listRecordsets);
    check(database->GetRuntimeClass()->m_nObjectSize==160 && !strcmp(database->GetRuntimeClass()->m_lpszClassName,"CDatabase") &&
        list->GetRuntimeClass()->m_nObjectSize==56 && !strcmp(list->GetRuntimeClass()->m_lpszClassName,"CPtrList"),
        "database and its embedded pointer list expose native runtime class identities and sizes");
    check(!list->m_pNodeHead && !list->m_pNodeTail && !list->m_nCount && !list->m_pNodeFree && !list->m_pBlocks && list->m_nBlockSize==10,
        "database owns native CPtrList storage with the native empty-list defaults");
    check(*reinterpret_cast<UINT64*>(storage+160)==0xa5a5a5a5a5a5a5a5ULL,"database constructor preserves the caller allocation guard");
    std::vector<BYTE> result;
    for(size_t i=8;i<160;++i)if(!(i>=32 && i<40) && !(i>=56 && i<64))result.push_back(storage[i]);
    if(native) {
        auto** table=*reinterpret_cast<void***>(database);
        using Destructor=void* (MS_ABI*)(void*,unsigned);
        reinterpret_cast<Destructor>(table[1])(database,0);
    } else database->~CDatabase();
    check(*reinterpret_cast<UINT64*>(storage+160)==0xa5a5a5a5a5a5a5a5ULL,"database destructor preserves the caller allocation guard");
    return result;
}
static void* MS_ABI localConstruct(void* storage) {return new(storage) CDatabase;}
std::vector<DWORD> options(CDatabase* database,void* optionMethod) {
    auto** original=*reinterpret_cast<void***>(database);void* table[12];memcpy(table,original,sizeof(table));
    table[10]=reinterpret_cast<void*>(&checkOption);*reinterpret_cast<void***>(database)=table;
    using SetOptions=void (MS_ABI*)(CDatabase*,HSTMT);
    auto apply=reinterpret_cast<SetOptions>(optionMethod);
    std::vector<DWORD> result;
    for(bool success: {true,false}) {
        accepted=success;optionResult=SQL_ERROR;optionCalls=checkCalls=0;database->m_dwQueryTimeout=37;
        apply(database,reinterpret_cast<HSTMT>(0xbeef));
        check(optionCalls==1 && checkCalls==1 && optionStatement==reinterpret_cast<HSTMT>(0xbeef) && option==SQL_QUERY_TIMEOUT && timeout==37 &&
            database->m_dwQueryTimeout==(success?37:DWORD(-1)),"database statement timeout uses its instance field and native virtual Check callback");
        result.push_back(database->m_dwQueryTimeout);
    }
    optionCalls=checkCalls=0;apply(database,reinterpret_cast<HSTMT>(0xcafe));
    check(!optionCalls && !checkCalls,"database disabled query timeout skips ODBC and validation");
    *reinterpret_cast<void***>(database)=original;database->m_dwQueryTimeout=15;return result;
}
std::vector<std::wstring> brackets(CDatabase* database,Brackets replace) {
    std::vector<std::wstring> result;
    for(wchar_t quote: {L'"',L'`',L'\0'})for(const wchar_t* sql: {L"[col]",L"'literal [col]' [table]",L"[[a]]",L"''[x]",L"'it''s [literal]' [y]",L"[[[x]]]",L""}) {
        wchar_t buffer[128];wcscpy(buffer,sql);database->m_chIDQuoteChar=quote;replace(database,buffer);result.emplace_back(buffer);
    }
    check(result[0]==L"\"col\"" && result[1]==L"'literal [col]' \"table\"" && result[2]==L"[a]" && result[4]==L"'it''s [literal]' \"y\"",
        "database bracket replacement respects literals, escaped quotes and escaped brackets");return result;
}
}
static void databaseLayout() {
    using namespace database_layout_test;
    static_assert(offsetof(CDatabase,m_bStripTrailingSpaces)==16 && offsetof(CDatabase,m_listRecordsets)==56 && offsetof(CDatabase,m_dwQueryTimeout)==152,"native database offsets");
    auto bytes=construct(&localConstruct,false);CDatabase database;
    database.SetLoginTimeout(27);database.SetQueryTimeout(41);
    CDatabase second;
    check(database.m_dwLoginTimeout==27 && database.m_dwQueryTimeout==41 && second.m_dwLoginTimeout==15 && second.m_dwQueryTimeout==15,
        "database timeout setters update only their native instance fields");
    database.m_bUpdatable=37;database.m_bTransactions=51;
    check(database.CanUpdate()==37 && database.CanTransact()==51,"database capability getters return their cached native BOOL fields");
    auto** table=*reinterpret_cast<void***>(&database);
    auto values=options(&database,table[8]);auto text=brackets(&database,&impl__ReplaceBrackets_CDatabase__QEAAXPEA_W_Z);
    using Check=BOOL (MS_ABI*)(const CDatabase*,RETCODE);
    auto validate=reinterpret_cast<Check>(table[10]);
    check(validate(&database,0) && validate(&database,1) && validate(&database,100) && !validate(&database,99) && !validate(&database,-1),
        "database Check accepts only native success, success-with-info and no-data results");
    using Bind=void (MS_ABI*)(CDatabase*,HSTMT);reinterpret_cast<Bind>(table[9])(&database,reinterpret_cast<HSTMT>(0xface));
    check(optionCalls==0,"database base BindParameters remains the native empty callback");
    database.m_hstmt=reinterpret_cast<HSTMT>(0xcafe);int code=0;
    using Throw=void (MS_ABI*)(CDatabase*,RETCODE);
    try {reinterpret_cast<Throw>(table[11])(&database,SQL_ERROR);}catch(int value){code=value;}
    check(code==899 && throwCalls==1 && throwCode==SQL_ERROR && throwDatabase==&database && throwStatement==database.m_hstmt,
        "database exception virtual slot forwards code, database and its native statement");database.m_hstmt=nullptr;
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_DATABASE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for database layout and behavior comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);
            check(construct(reinterpret_cast<Constructor>(base+0x1f6b10),true)==bytes,"database constructor matches every native written and untouched data byte");
            auto** optionSlot=reinterpret_cast<void**>(base+0x3e9080);
            auto* saved=*optionSlot;DWORD protection;::VirtualProtect(optionSlot,8,PAGE_READWRITE,&protection);*optionSlot=reinterpret_cast<void*>(&SQLSetStmtOption);
            check(options(&database,base+0x1f7200)==values,"database timeout state and callback behavior match native MFC");
            *optionSlot=saved;DWORD ignored;::VirtualProtect(optionSlot,8,protection,&ignored);
            check(brackets(&database,reinterpret_cast<Brackets>(base+0x1f75f0))==text,"database bracket replacement matches native MFC across escapes and custom quote characters");
            auto nativeCheck=reinterpret_cast<Check>(base+0x1f75d0);bool same=true;
            for(RETCODE rc: {RETCODE(0),RETCODE(1),RETCODE(99),RETCODE(100),RETCODE(-1),RETCODE(-2)})same&=nativeCheck(&database,rc)==validate(&database,rc);
            check(same,"database result-code validation matches native MFC");::FreeLibrary(module);
        }
    }
}
