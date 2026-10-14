#pragma once
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__HHF_Z(CFieldExchange*,const wchar_t*,void*,int,int,short);
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHF_Z(CFieldExchange*,const wchar_t*,void*,int,int,short);
namespace text_string_rfx_test {
using Action=void (MS_ABI*)(CFieldExchange*,const wchar_t*,void*,int,int,short);
template<class CH> struct Value {
    CH* text;
    explicit Value(IAtlStringMgr* manager) : text(reinterpret_cast<CH*>(manager->GetNilString()+1)) {}
    CStringData* data() const {return reinterpret_cast<CStringData*>(text)-1;}
    ~Value() {auto* p=data();if(p->Release())p->pStringMgr->Free(p);}
    void set(const char* string) {
        int length=int(strlen(string));auto* p=data();
        if(p->IsShared() || p->nAllocLength<length) {
            auto* fresh=p->pStringMgr->Allocate(length,sizeof(CH));
            if(p->Release())p->pStringMgr->Free(p);p=fresh;text=reinterpret_cast<CH*>(p+1);
        }
        for(int i=0;i<=length;++i)text[i]=CH(string[i]);p->nDataLength=length;
    }
};
template<class CH> std::vector<UINT64> exercise(Action action) {
    using namespace binary_rfx_test;
    database_strings_test::Manager manager;
    recordset_storage_test::Fixture fixture(1,1);auto* rs=fixture.recordset;rs->AllocStatusArrays();
    CDatabase database;rs->m_pDatabase=&database;rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    CODBCFieldInfo meta{};meta.m_nSQLType=SQL_VARCHAR;meta.m_nPrecision=300;rs->m_rgODBCFieldInfos=&meta;
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);
    *reinterpret_cast<void***>(rs)=slots;
    Value<CH> value(&manager);std::vector<UINT64> result;
    auto run=[&](UINT operation,UINT type=0,int maximum=512) {
        CFieldExchange fx(operation,rs);fx.SetFieldType(type);fx.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);
        action(&fx,L"text",&value,maximum,SQL_VARCHAR,3);return fx;
    };
    auto save=[&] {
        result.push_back(rs->m_pbFieldFlags[0]);result.push_back(rs->m_rgFieldInfos[0].m_nLength);
        result.push_back(value.data()->nDataLength);result.push_back(value.data()->nAllocLength);result.push_back(value.data()->nRefs);
        for(int i=0;i<value.data()->nDataLength;++i)result.push_back(value.text[i]);result.push_back(0xffff);
    };
    CFieldExchange empty(CFieldExchange::SetFieldNull,rs);empty.SetFieldType(0);empty.m_bField=TRUE;
    action(&empty,L"text",&value,512,SQL_VARCHAR,0);
    check(value.data()!=reinterpret_cast<CStringData*>(manager.nil) && value.data()->nRefs==1 &&
        reinterpret_cast<CStringData*>(manager.nil)->nRefs==2,"CString null setting forks a shared empty string and releases its original owned reference");
    rs->ClearFieldStatus();value.set("alpha");
    resetSQL();run(CFieldExchange::BindFieldToColumn);
    constexpr int cType=sizeof(CH)==1?SQL_C_CHAR:SQL_C_WCHAR;
    check(binds==1 && checks==1 && binding.cType==cType && binding.capacity==301*sizeof(CH) && binding.data==value.text &&
        binding.length==&rs->m_rgFieldInfos[0].m_nLength && rs->GetBoundFieldIndex(&value)==1 && value.data()->nRefs==-1 && value.data()->nDataLength==5,
        "CString column binding locks the buffer, preserves its string length and registers the object address");save();
    CH* bound=value.text;int allocated=manager.allocations,reallocated=manager.reallocations;
    for(bool strip: {false,true}) {
        database.m_bStripTrailingSpaces=strip;
        for(const char* row: {"row   ","a \t ","   ",""}) {
            value.set(row);rs->m_rgFieldInfos[0].m_nLength=999;run(CFieldExchange::Fixup);
            size_t count=strlen(row);if(strip)while(count && row[count-1]==' ')--count;
            check(value.text==bound && value.data()->nRefs==-1 && value.data()->nDataLength==int(count) && rs->m_rgFieldInfos[0].m_nLength==SQLLEN(count*sizeof(CH)),
                "CString Fixup trims requested trailing spaces and releases length without moving or unlocking the ODBC buffer");save();
        }
    }
    check(manager.allocations==allocated && manager.reallocations==reallocated,"CString Fixup retains the original binding allocation");
    value.set("alpha");CFieldExchange fx(CFieldExchange::SetFieldNull,rs);fx.SetFieldType(0);fx.m_bField=TRUE;
    action(&fx,L"text",&value,512,SQL_VARCHAR,0);
    check(value.text==bound && !value.data()->nDataLength && value.data()->nRefs==-1 && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1,
        "CString null setting empties the selected string while preserving its locked buffer");save();
    fx.m_nFields=0;fx.m_bField=FALSE;action(&fx,L"text",&value,512,SQL_VARCHAR,0);
    check(!rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==SQL_NTS,"CString null clearing restores SQL_NTS");
    Value<CH> other(&manager);fx.m_nFields=0;fx.m_bField=TRUE;fx.m_pvField=&other;value.set("alpha");
    action(&fx,L"text",&value,512,SQL_VARCHAR,0);check(value.data()->nDataLength==5 && !rs->IsFieldStatusNull(0),"CString null operation ignores a different selected object");
    fx.m_nFields=0;fx.m_pvField=&value;action(&fx,L"text",&value,512,SQL_VARCHAR,0);check(!value.data()->nDataLength,"CString null operation selects the string object address");
    value.set("alpha");rs->m_rgFieldInfos[0].m_nLength=-1;run(CFieldExchange::Fixup);
    check(!value.data()->nDataLength && value.text==bound && rs->IsFieldStatusNull(0)==2,"CString SQL-null Fixup empties the existing bound buffer");
    rs->ClearFieldStatus();run(CFieldExchange::MarkForAddNew);check(!rs->IsFieldStatusDirty(0),"empty CString remains clean when adding a record");
    value.set("alpha");run(CFieldExchange::MarkForAddNew);check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0),"nonempty CString becomes dirty and non-null for new records");
    rs->ClearFieldStatus();run(CFieldExchange::AllocCache);int cacheType=sizeof(CH)==1?21:20;
    check(rs->m_rgFieldInfos[0].m_pvDataCache && rs->m_rgFieldInfos[0].m_nDataType==cacheType &&
        !(*reinterpret_cast<CStringData**>(rs->m_rgFieldInfos[0].m_pvDataCache)-1)->nDataLength,
        "CString cache allocation constructs a native empty string and records its ANSI or Unicode type");
    run(CFieldExchange::StoreField);value.set("changed");auto changed=run(CFieldExchange::MarkForUpdate);
    check(rs->IsFieldStatusDirty(0)==1 && changed.m_bField,"CString MarkForUpdate compares the string object with its cached value");save();
    rs->ClearFieldStatus();run(CFieldExchange::LoadField);
    check(value.data()->nDataLength==5 && value.text[0]==CH('a') && value.data()->nRefs==-1 && rs->m_rgFieldInfos[0].m_nLength==5*sizeof(CH),
        "CString cache restore copies characters into the locked buffer and restores byte length");save();
    changed=run(CFieldExchange::MarkForUpdate);check(!rs->IsFieldStatusDirty(0) && !changed.m_bField,"unchanged CString cache values remain clean");
    rs->SetNullFieldStatus(0);run(CFieldExchange::StoreField);rs->ClearFieldStatus();value.set("marker");run(CFieldExchange::LoadField);
    check(value.data()->nDataLength==6 && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1,"cached-null CString load preserves the current text");save();
    rs->ClearFieldStatus();run(CFieldExchange::LoadField); // cached null still merges into the live status
    rs->ClearFieldStatus();value.set("sql");run(CFieldExchange::BindFieldForUpdate);
    check(rs->m_rgFieldInfos[0].m_nLength==SQL_IGNORE,"clean CString update binding uses SQL_IGNORE");
    run(CFieldExchange::UnbindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==3*sizeof(CH),"CString unbind restores its current byte length");
    rs->SetDirtyFieldStatus(0);CString sql;
    CFieldExchange sqlFX(CFieldExchange::NameValue,rs);sqlFX.SetFieldType(0);sqlFX.m_pstr=&sql;sqlFX.m_lpszSeparator=L", ";sqlFX.m_hstmt=reinterpret_cast<HSTMT>(0xface);resetSQL();
    action(&sqlFX,L"text",&value,512,SQL_VARCHAR,0);
    check(sql==L"text=?, " && binds==1 && binding.param && binding.data==value.text && binding.cType==cType && binding.sqlType==SQL_VARCHAR &&
        binding.precision==300 && !binding.capacity && binding.statement==reinterpret_cast<HSTMT>(0xface) && rs->m_rgFieldInfos[0].m_nLength==3*sizeof(CH),
        "CString NameValue builds SQL and binds the locked character buffer with native column metadata");save();
    rs->m_pbParamFlags[0]=0;resetSQL();run(CFieldExchange::RebindParam,4);
    check(!binds && rs->m_plParamLength[0]==3*sizeof(CH),"CString parameter rebinding updates byte length without an ODBC binding call");
    value.set("marker");
    resetSQL();int locks=value.data()->nRefs;rs->m_pbParamFlags[0]=0;run(CFieldExchange::BindParam,4,512);
    check(binding.param && binding.statement==reinterpret_cast<HSTMT>(0xbeef) && binding.direction==4 && binding.cType==cType && binding.capacity==512*sizeof(CH) &&
        binding.data==value.text && rs->GetBoundParamIndex(&value)==1 && rs->m_plParamLength[0]==SQL_NTS && value.data()->nRefs==locks-1 && value.data()->nDataLength==6,
        "CString parameter binding uses the exchange statement and direction while preserving nested buffer locks");save();
    rs->m_pbParamFlags[0]=2;run(CFieldExchange::BindParam,4,512);check(rs->m_plParamLength[0]==-1,"CString parameter binding preserves the native SQL-null indicator");
    resetSQL(SQL_SUCCESS_WITH_INFO);int exception=0;
    try {run(CFieldExchange::BindParam,4);}catch(int code){exception=code;}
    check(exception==701 && throws==1 && binding.statement==reinterpret_cast<HSTMT>(0xbeef),"CString parameter errors reach the recordset callback after releasing the logical string length");
    bool invalid=false;try {run(CFieldExchange::BindParam,4,-1);}catch(COleException* error){invalid=error->m_sc==E_INVALIDARG;error->Delete();}
    check(invalid,"CString parameter binding rejects a negative maximum length with native E_INVALIDARG");
    rs->FreeDataCache();check(*reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL,"CString exchange preserves the native recordset allocation guard");
    return result;
}
}
static void textStringFieldExchange() {
    using namespace text_string_rfx_test;
    auto ansi=exercise<char>(&impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__HHF_Z);
    auto wide=exercise<wchar_t>(&impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HHF_Z);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_CSTRING_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for CString RFX comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);void** column=reinterpret_cast<void**>(base+0x3e9038);void** parameter=reinterpret_cast<void**>(base+0x3e9040);
            auto* savedColumn=*column;auto* savedParameter=*parameter;DWORD protection;::VirtualProtect(column,16,PAGE_READWRITE,&protection);
            *column=reinterpret_cast<void*>(&SQLBindCol);*parameter=reinterpret_cast<void*>(&SQLBindParameter);
            check(exercise<char>(reinterpret_cast<Action>(base+0x1ffdb0))==ansi,"ANSI CString exchange matches native text, status, cache, lengths and buffer locks");
            check(exercise<wchar_t>(reinterpret_cast<Action>(base+0x1ff5c0))==wide,"Unicode CString exchange matches native text, status, cache, lengths and buffer locks");
            *column=savedColumn;*parameter=savedParameter;DWORD ignored;::VirtualProtect(column,16,protection,&ignored);::FreeLibrary(module);
        }
    }
}
