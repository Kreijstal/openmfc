#pragma once
#include "detail/DatabaseStringBuffer.h"
extern "C" CString* MS_ABI impl__GetDefaultConnect_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CRecordset*,CString*);
extern "C" CString* MS_ABI impl__GetDefaultSQL_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CRecordset*,CString*);
extern "C" const wchar_t* MS_ABI impl__FindSQLToken_CRecordset__SAPEB_WPEB_W0_Z(const wchar_t*,const wchar_t*);
extern "C" BOOL MS_ABI impl__IsJoin_CRecordset__SAHPEB_W_Z(const wchar_t*);
extern "C" BOOL MS_ABI impl__IsSelectQueryUpdatable_CRecordset__QEAAHPEB_W_Z(CRecordset*,const wchar_t*);
extern "C" BOOL MS_ABI impl__IsSQLUpdatable_CRecordset__QEAAHPEB_W_Z(CRecordset*,const wchar_t*);
extern "C" BOOL MS_ABI impl__IsRecordsetUpdatable_CRecordset__IEAAHXZ(CRecordset*);
extern "C" void MS_ABI impl__BuildSelectSQL_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__AppendFilterAndSortSQL_CRecordset__IEAAXXZ(CRecordset*);
extern "C" void MS_ABI impl__BuildSQL_CRecordset__IEAAXPEB_W_Z(CRecordset*,const wchar_t*);
namespace recordset_sql_test {
struct API {
    decltype(&impl__FindSQLToken_CRecordset__SAPEB_WPEB_W0_Z) find;
    decltype(&impl__IsJoin_CRecordset__SAHPEB_W_Z) join;
    decltype(&impl__IsSelectQueryUpdatable_CRecordset__QEAAHPEB_W_Z) select,sql;
    decltype(&impl__IsRecordsetUpdatable_CRecordset__IEAAHXZ) recordset;
    decltype(&impl__BuildSelectSQL_CRecordset__IEAAXXZ) buildSelect,append;
    decltype(&impl__BuildSQL_CRecordset__IEAAXPEB_W_Z) build;
    decltype(&impl__GetDefaultSQL_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ) defaultSQL,defaultConnect;
};
static void text(std::vector<INT64>& output,const CString& value) {
    output.push_back(value.GetLength());for(int i=0;i<value.GetLength();++i)output.push_back(value.GetString()[i]);
}
static const wchar_t* MS_ABI crtSearch(const wchar_t* text,const wchar_t* token) {return std::wcsstr(text,token);}
static int caught(CException* error) {error->Delete();return -1;}
static const wchar_t* queries[]={L"",L"table",L"SELECT * FROM table",L"select a from [some table] where a=1 order by a",L"SELECT ' FROM ' FROM t",
    L"SELECT [a FROM b] FROM t",L"SELECT * FROM [t,t]",L"SELECT * FROM t,t2",L"SELECT * FROM t JOIN x ON t.a=x.a",L"SELECT * FROM t WHERE a=' JOIN x,y'",
    L"SELECT * FROM t GROUP BY a",L"SELECT * FROM t UNION SELECT * FROM x",L"SELECT * FROM t WHERE a=' GROUP BY '",L"SELECT * FROM t ORDER BY a",
    L"SELECT * FROM t ORDER BY a WHERE b=1",L"SELECT * FROM ",L"x FROM t",L"{CALL select * from t}",L"{?=CALL query}",L"{anything FROM t}",
    L"' FROM ' FROM t",L"SELECT '' FROM t",L"SELECT 'x'' FROM y' FROM t",L"SELECT [[ FROM t",L"SELECT [a]] FROM b] FROM t",L"SELECT ] [ FROM t",
    L"select \u03b4 from \u03b1",L"SELECTED FROM t"};
static std::vector<INT64> analysis(const API& api) {
    std::vector<INT64> result;
    for(const wchar_t* sql:queries)for(const wchar_t* token:{L" FROM ",L" WHERE ",L" ORDER BY ",L"SELECT",L"from",L",",L" JOIN ",L"",L"[",L"'",L"\u0394"}) {
        auto* found=api.find(sql,token);result.push_back(found?found-sql:-1);
    }
    for(const wchar_t* sql:queries) {
        result.push_back(api.join(sql));
        for(int method=0;method<2;++method) {
            CRecordset rs;rs.m_strTableName=L"old table";
            result.push_back((method?api.sql:api.select)(&rs,sql));text(result,rs.m_strTableName);
        }
    }
    for(int method=0;method<4;++method) {
        int error=0;CRecordset rs;
        try{if(method==0)api.find(nullptr,L"x");else if(method==1)api.find(L"x",nullptr);else if(method==2)api.join(nullptr);else api.sql(&rs,nullptr);}
        catch(int value){error=value==19?-1:value;}catch(COleException* value){error=value->m_sc;value->Delete();}catch(CException* value){error=caught(value);}
        check(error==-1,"SQL token and updatability entry points reject required null arguments");result.push_back(error);
    }
    for(bool valid:{false,true})for(bool binary:{false,true})for(BOOL useSQL:{FALSE,BOOL(69)})for(DWORD options:{0ul,1ul,2ul,3ul,4ul,6ul,0xfffffffful})for(bool missing:{false,true}) {
        CRecordset rs;CDatabase db;rs.m_pDatabase=missing?nullptr:&db;rs.m_strSQL=valid?L"SELECT * FROM t":L"not a query";
        rs.m_strTableName=L"old table";rs.m_bLongBinaryColumns=binary;rs.m_bUseUpdateSQL=useSQL;db.m_dwUpdateOptions=options;
        int error=0;BOOL returned=-3;try{returned=api.recordset(&rs);}catch(int value){error=value==19?-1:value;}catch(COleException* value){error=value->m_sc;value->Delete();}catch(CException* value){error=caught(value);}
        result.insert(result.end(),{returned,error,rs.m_bUseUpdateSQL});text(result,rs.m_strTableName);rs.m_pDatabase=nullptr;
    }
    return result;
}
static std::vector<INT64> events;
static UINT namesCount;
static const wchar_t* names;
static bool returningThrow;
static int failure;
static void exchange(CRecordset* rs,CFieldExchange* fx,bool bulk) {
    events.insert(events.end(),{1,fx->m_nOperation,bulk,INT64(UINT_PTR(fx->m_hstmt)),fx->m_nOperation==8 && fx->m_pstr==&rs->m_strSQL,
        rs->m_nFields?rs->m_pbFieldFlags[0]:0,rs->m_nFields>1?rs->m_pbFieldFlags[1]:0});
    if(failure==1)throw 77;
    if(fx->m_nOperation==8) {
        check(fx->m_lpszSeparator && !wcscmp(fx->m_lpszSeparator,L","),"SELECT construction enumerates names with the native comma separator");
        if(names)openmfc::detail::DatabaseStringBuffer(*fx->m_pstr).append(names);
        else fx->m_pstr->Empty();
        fx->m_nFields=namesCount;
    }
    fx->m_nParams=21;
}
static void MS_ABI ordinary(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,false);}
static void MS_ABI bulk(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,true);}
static void MS_ABI throwStatus(CRecordset*,RETCODE code,HSTMT statement){events.insert(events.end(),{2,code,INT64(UINT_PTR(statement))});if(!returningThrow)throw 700+code;}
static CString* MS_ABI defaultSQL(CRecordset*,CString* output){events.push_back(3);if(failure==2)throw 78;return new(output) CString(L"[default table]");}
struct Fixture {
    CDatabase db;CRecordset rs;BYTE flags[2]{0x80,0x82};void** original;void* table[27];
    Fixture() {
        original=*reinterpret_cast<void***>(&rs);memcpy(table,original,sizeof(table));table[16]=reinterpret_cast<void*>(&defaultSQL);table[19]=reinterpret_cast<void*>(&ordinary);table[20]=reinterpret_cast<void*>(&bulk);table[24]=reinterpret_cast<void*>(&throwStatus);*reinterpret_cast<void***>(&rs)=table;
        rs.m_pDatabase=&db;rs.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);rs.m_nFields=2;rs.m_pbFieldFlags=flags;
        rs.m_strSQL=L"old SQL";rs.m_strTableName=L"old table";rs.m_bUpdatable=69;rs.m_bAppendable=71;rs.m_bUseUpdateSQL=73;
        db.m_chIDQuoteChar=L'"';db.m_dwUpdateOptions=2;db.m_bAddForUpdate=FALSE;
        names=L"[first],[second],";namesCount=2;returningThrow=false;failure=0;events.clear();
    }
    ~Fixture(){*reinterpret_cast<void***>(&rs)=original;rs.m_hstmt=nullptr;rs.m_pDatabase=nullptr;rs.m_pbFieldFlags=nullptr;}
};
static void save(std::vector<INT64>& result,Fixture& f,int error) {
    auto& rs=f.rs;result.insert(result.end(),events.begin(),events.end());
    result.insert(result.end(),{90,error,rs.m_bUpdatable,rs.m_bAppendable,rs.m_bUseUpdateSQL,f.flags[0],f.flags[1]});
    text(result,rs.m_strSQL);text(result,rs.m_strTableName);
}
static std::vector<INT64> construction(const API& api) {
    std::vector<INT64> result;
    for(const wchar_t* sql:{L"table",L"",L"SELECT * FROM t",L"selected",L"select\tx",L"{CALL query}",L"{anything}",L"[table]"})for(bool multi:{false,true})for(int outputCase=0;outputCase<4;++outputCase)for(bool returns:{false,true}) {
        Fixture f;f.rs.m_strSQL=sql;f.rs.m_dwOptions=multi?0x200:0;
        if(outputCase==1)namesCount=0;else if(outputCase==2)names=L"";else if(outputCase==3)names=nullptr;
        returningThrow=returns;int error=0;
        try{api.buildSelect(&f.rs);}catch(int value){error=value;}catch(COleException* value){error=value->m_sc;value->Delete();}catch(CException* value){error=caught(value);}
        save(result,f,error);
    }
    for(int input=0;input<4;++input)for(bool parameters:{false,true})for(bool multi:{false,true})for(bool writable:{false,true})for(bool forUpdate:{false,true})for(int quote:{0,int('"'),int('`')}) {
        Fixture f;f.rs.m_nParams=parameters?3:0;f.rs.m_dwOptions=multi?0x200:0;
        f.rs.m_bUpdatable=writable?69:0;f.rs.m_bAppendable=writable?71:0;f.db.m_bAddForUpdate=forUpdate?79:0;f.db.m_chIDQuoteChar=wchar_t(quote);
        f.rs.m_strFilter=L"[first]>1";f.rs.m_strSort=L"[second] DESC";
        const wchar_t* sql=input==0?nullptr:input==1?L"[some table]":input==2?L"SELECT [first] FROM [some table]":f.rs.m_strSQL.GetString()+4;
        int error=0;try{api.build(&f.rs,sql);}catch(int value){error=value;}catch(COleException* value){error=value->m_sc;value->Delete();}catch(CException* value){error=caught(value);}save(result,f,error);
    }
    for(bool filter:{false,true})for(bool sort:{false,true})for(bool binary:{false,true}) {
        Fixture f;f.rs.m_strSQL=L"SELECT * FROM t WHERE a=1";f.rs.m_strFilter=filter?L"b=2":L"";f.rs.m_strSort=sort?L"c":L"";
        if(binary && filter){auto* data=database_strings_test::data(f.rs.m_strFilter);f.rs.m_strFilter.GetBuffer(3);auto* chars=const_cast<wchar_t*>(f.rs.m_strFilter.GetString());chars[1]=0;(void)data;}
        api.append(&f.rs);save(result,f,0);
    }
    for(bool missingDB:{false,true})for(int callback:{0,1,2}) {
        Fixture f;if(missingDB)f.rs.m_pDatabase=nullptr;failure=callback;int error=0;
        try{api.build(&f.rs,nullptr);}catch(int value){error=value==19?-1:value;}catch(COleException* value){error=value->m_sc;value->Delete();}catch(CException* value){error=caught(value);}save(result,f,error);
    }
    return result;
}
static void defaults(const API& api) {
    Fixture fixture;
    for(bool connect:{false,true})for(bool missing:{false,true}) {
        alignas(CString) BYTE storage[16];memset(storage,0xa5,sizeof(storage));events.clear();failure=2;
        auto* returned=(connect?api.defaultConnect:api.defaultSQL)(missing?nullptr:&fixture.rs,reinterpret_cast<CString*>(storage));
        check(returned==reinterpret_cast<CString*>(storage) && *reinterpret_cast<UINT64*>(storage+8)==0xa5a5a5a5a5a5a5a5ULL && events.empty() &&
            !wcscmp(returned->GetString(),connect?L"ODBC;":L""),
            "default recordset string exports use this-first return buffers, preserve guards and bypass derived virtual getters");
        returned->~CString();
    }
}
static std::vector<INT64> ownership(const API& api) {
    std::vector<INT64> result;
    for(int method=0;method<3;++method)for(int refs:{1,2,-1})for(bool different:{false,true}) {
        database_strings_test::Manager source,other;source.clone=different?&other:&source;
        Fixture f;auto* original=database_strings_test::data(f.rs.m_strSQL);
        auto* buffer=source.Allocate(256,2);buffer->nDataLength=7;buffer->nRefs=refs;memcpy(buffer+1,L"[table]",16);
        database_strings_test::attach(f.rs.m_strSQL,buffer);f.rs.m_strFilter=L"x=1";f.rs.m_strSort=L"x";
        if(method==0)api.append(&f.rs);else if(method==1)api.buildSelect(&f.rs);else api.build(&f.rs,L"SELECT * FROM [table]");
        auto* data=database_strings_test::data(f.rs.m_strSQL);
        result.insert(result.end(),{source.allocations,source.reallocations,source.frees,source.clones,
            other.allocations,other.reallocations,other.frees,other.clones,data->nRefs,data->nAllocLength,data->pStringMgr==&source,data->pStringMgr==&other,
            refs==2?buffer->nRefs:0});text(result,f.rs.m_strSQL);
        f.rs.m_strSQL.~CString();new(&f.rs.m_strSQL) CString;
        database_strings_test::data(f.rs.m_strSQL)->Release();database_strings_test::attach(f.rs.m_strSQL,original);
        if(refs==2 && buffer->Release())source.Free(buffer);
        result.insert(result.end(),{source.frees,other.frees});
    }
    return result;
}
}
static void recordsetSQL() {
    using namespace recordset_sql_test;
    API local{&impl__FindSQLToken_CRecordset__SAPEB_WPEB_W0_Z,&impl__IsJoin_CRecordset__SAHPEB_W_Z,
        &impl__IsSelectQueryUpdatable_CRecordset__QEAAHPEB_W_Z,&impl__IsSQLUpdatable_CRecordset__QEAAHPEB_W_Z,
        &impl__IsRecordsetUpdatable_CRecordset__IEAAHXZ,&impl__BuildSelectSQL_CRecordset__IEAAXXZ,&impl__AppendFilterAndSortSQL_CRecordset__IEAAXXZ,&impl__BuildSQL_CRecordset__IEAAXPEB_W_Z,
        &impl__GetDefaultSQL_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ,
        &impl__GetDefaultConnect_CRecordset__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ};
    defaults(local);auto parsed=analysis(local);auto built=construction(local);auto owners=ownership(local);
    {
        const wchar_t* sql=L"SELECT ' FROM ' FROM [t]";
        check(local.find(sql,L" FROM ")==sql+15 && !local.find(sql,L"from"),"SQL token search skips quoted matches and expects an uppercase token");
        Fixture f;f.rs.m_strSQL=L"table";local.buildSelect(&f.rs);
        check(!wcscmp(f.rs.m_strSQL.GetString(),L"SELECT [first],[second]  FROM table") && f.flags[0]==0x81 && f.flags[1]==0x83,
            "table-name SQL expands the derived column list and marks all declared fields dirty");
    }
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_SQL_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for SQL construction and parsing comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"SQL construction oracle uses the verified MFC image build");
            if(pinned) {
                // Wine's VCRUNTIME forwarder returns null for wcsstr(L"",L"").
                // Use the real MSVC CRT linked into this executable at that
                // dependency boundary, matching the Windows runtime contract.
                auto** searchSlot=reinterpret_cast<void**>(base+0x2c73f8);void* oldSearch=*searchSlot;DWORD protection;
                ::VirtualProtect(searchSlot,8,PAGE_READWRITE,&protection);*searchSlot=reinterpret_cast<void*>(&crtSearch);
                API native{reinterpret_cast<decltype(local.find)>(base+0x1fb140),reinterpret_cast<decltype(local.join)>(base+0x1fb100),
                    reinterpret_cast<decltype(local.select)>(base+0x1fae90),reinterpret_cast<decltype(local.sql)>(base+0x1fae00),
                    reinterpret_cast<decltype(local.recordset)>(base+0x1fbc70),reinterpret_cast<decltype(local.buildSelect)>(base+0x1fb9f0),
                    reinterpret_cast<decltype(local.append)>(base+0x1fbbd0),reinterpret_cast<decltype(local.build)>(base+0x1fa210),
                    reinterpret_cast<decltype(local.defaultSQL)>(base+0xc4270),reinterpret_cast<decltype(local.defaultConnect)>(base+0x1f9d60)};
                defaults(native);
                auto actual=analysis(native);if(actual!=parsed)for(size_t i=0;i<std::min(actual.size(),parsed.size());++i)if(actual[i]!=parsed[i]){std::printf("SQL analysis mismatch %zu local %lld native %lld\n",i,parsed[i],actual[i]);break;}
                check(actual==parsed,"SQL token quoting, bracket escapes, table extraction, joins, call prefixes and long-binary update capabilities match native MFC");
                actual=construction(native);if(actual!=built)for(size_t i=0;i<std::min(actual.size(),built.size());++i)if(actual[i]!=built[i]){std::printf("SQL construction mismatch %zu local %lld native %lld\n",i,built[i],actual[i]);break;}
                check(actual==built,"SQL construction, derived name and parameter exchanges, filter ordering, quoting and partial failure state match native MFC");
                actual=ownership(native);if(actual!=owners)for(size_t i=0;i<std::min(actual.size(),owners.size());++i)if(actual[i]!=owners[i]){std::printf("SQL ownership mismatch %zu local %lld native %lld\n",i,owners[i],actual[i]);break;}
                check(actual==owners,"SQL construction preserves native custom-manager cloning, locked buffers, shared storage and final string ownership");
                *searchSlot=oldSearch;DWORD ignored;::VirtualProtect(searchSlot,8,protection,&ignored);
            }
            ::FreeLibrary(module);
        }
    }
}
