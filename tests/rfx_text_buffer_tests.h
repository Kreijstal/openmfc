#pragma once
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEADHHF_Z(CFieldExchange*,const wchar_t*,char*,int,int,short);
extern "C" void MS_ABI impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEA_WHHF_Z(CFieldExchange*,const wchar_t*,wchar_t*,int,int,short);
namespace text_buffer_rfx_test {
template<class CH> using Action=void (MS_ABI*)(CFieldExchange*,const wchar_t*,CH*,int,int,short);
template<class CH> std::vector<UINT64> exercise(Action<CH> action) {
    using namespace binary_rfx_test;
    recordset_storage_test::Fixture fixture(1,1);auto* rs=fixture.recordset;
    rs->AllocStatusArrays();rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);
    *reinterpret_cast<void***>(rs)=slots;
    CODBCFieldInfo meta{};meta.m_nSQLType=SQL_VARCHAR;meta.m_nPrecision=300;rs->m_rgODBCFieldInfos=&meta;
    CH value[514];for(auto& ch:value)ch=CH('g');
    auto write=[&] {value[0]=CH('a');value[1]=CH('b');value[2]=CH('c');value[3]=0;};write();
    auto run=[&](UINT op,UINT type=0) {CFieldExchange fx(op,rs);fx.SetFieldType(type);fx.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);
        action(&fx,L"text",value,512,SQL_VARCHAR,3);return fx;};
    constexpr int cType=sizeof(CH)==1?SQL_C_CHAR:SQL_C_WCHAR;
    std::vector<UINT64> result;
    auto save=[&] {result.push_back(rs->m_pbFieldFlags[0]);result.push_back(rs->m_rgFieldInfos[0].m_nLength);
        for(int i=0;i<4;++i)result.push_back(value[i]);};
    resetSQL();run(CFieldExchange::BindFieldToColumn);
    check(binds==1 && checks==1 && binding.cType==cType && binding.capacity==301*sizeof(CH) && value[300]==0 && value[301]==CH('g') &&
        binding.data==value && binding.length==&rs->m_rgFieldInfos[0].m_nLength && rs->GetBoundFieldIndex(value)==1,
        "raw text binding uses native metadata, exact terminator and byte capacity");
    meta.m_nSQLType=SQL_DECIMAL;meta.m_nPrecision=300;run(CFieldExchange::BindFieldToColumn);
    check(binding.capacity==303*sizeof(CH) && value[302]==0,"numeric raw text binding reserves sign and decimal characters");
    meta.m_nSQLType=SQL_INTEGER;run(CFieldExchange::BindFieldToColumn);
    check(binding.capacity==302*sizeof(CH),"integer raw text binding reserves a sign character");
    meta.m_nSQLType=SQL_VARCHAR;meta.m_nPrecision=30;run(CFieldExchange::BindFieldToColumn);
    check(binding.capacity==513*sizeof(CH) && value[512]==0 && value[513]==CH('g'),"short text columns bind the caller's maximum capacity");
    for(BYTE flag: {BYTE(0),BYTE(1),BYTE(2),BYTE(0x80)}) {
        rs->m_pbParamFlags[0]=flag;resetSQL();run(CFieldExchange::BindParam,4);
        check(binds==1 && checks==0 && binding.param && binding.statement==reinterpret_cast<HSTMT>(0xbeef) && binding.direction==4 &&
            binding.cType==cType && binding.sqlType==SQL_VARCHAR && binding.precision==512 && binding.scale==3 && binding.capacity==512*sizeof(CH) &&
            binding.length==rs->m_plParamLength && *binding.length==(SQLLEN(flag)|SQL_NTS) && rs->GetBoundParamIndex(value)==1,
            "raw text parameters preserve native flag indicator, direction and exchange statement");
        result.push_back(*binding.length);
    }
    rs->m_pbParamFlags[0]=0;resetSQL(SQL_SUCCESS_WITH_INFO);int exception=0;
    try {run(CFieldExchange::BindParam,4);}catch(int code){exception=code;}
    check(exception==701 && throws==1 && binding.statement==reinterpret_cast<HSTMT>(0xbeef),"raw text parameter binding rejects success-with-info through the recordset callback");
    resetSQL(SQL_ERROR,false);exception=0;
    try {run(CFieldExchange::BindFieldToColumn);}catch(int code){exception=code;}
    check(exception==699 && throws==1 && checks==1 && !binding.statement,"raw text column errors use the virtual check and null-statement exception callback");
    resetSQL();CFieldExchange fx(CFieldExchange::SetFieldNull,rs);fx.SetFieldType(0);fx.m_bField=TRUE;
    action(&fx,L"text",value,512,SQL_VARCHAR,0);
    check(rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1 && value[0]==CH('a'),"raw text null setting leaves the buffer untouched");save();
    fx.m_nFields=0;fx.m_bField=FALSE;action(&fx,L"text",value,512,SQL_VARCHAR,0);
    check(!rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==SQL_NTS && value[0]==CH('a'),"raw text null clearing restores SQL_NTS without rewriting text");save();
    CH other=0;fx.m_nFields=0;fx.m_bField=TRUE;fx.m_pvField=&other;action(&fx,L"text",value,512,SQL_VARCHAR,0);
    check(!rs->IsFieldStatusNull(0),"raw text null operation ignores other selected addresses");
    fx.m_nFields=0;fx.m_pvField=value;action(&fx,L"text",value,512,SQL_VARCHAR,0);
    check(rs->IsFieldStatusNull(0)==2,"raw text null operation targets a selected buffer");
    run(CFieldExchange::Fixup);check(value[0]==0 && rs->IsFieldStatusNull(0)==2,"raw text SQL-null Fixup empties the buffer");save();
    rs->ClearFieldStatus();run(CFieldExchange::MarkForAddNew);check(!rs->IsFieldStatusDirty(0),"empty text remains clean for new records");
    write();run(CFieldExchange::MarkForAddNew);check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0),"nonempty text becomes dirty and non-null for new records");save();
    rs->ClearFieldStatus();run(CFieldExchange::AllocCache);
    check(rs->m_rgFieldInfos[0].m_pvDataCache && rs->m_rgFieldInfos[0].m_nDataType==(sizeof(CH)==1?19:18),"raw text cache allocation publishes native ANSI or Unicode type");
    run(CFieldExchange::StoreField);value[0]=CH('z');rs->SetDirtyFieldStatus(0);run(CFieldExchange::LoadField);
    check(value[0]==CH('z') && rs->m_rgFieldInfos[0].m_nLength==3*sizeof(CH) && rs->IsFieldStatusDirty(0)==1,
        "raw text cache load restores byte length and merges status without copying buffer characters");save();
    rs->SetNullFieldStatus(0);run(CFieldExchange::StoreField);rs->ClearFieldStatus();run(CFieldExchange::LoadField);
    check(value[0]==CH('z') && rs->m_rgFieldInfos[0].m_nLength==-1 && rs->IsFieldStatusNull(0)==2,"raw text cached-null load preserves text bytes");save();
    rs->FreeDataCache();rs->ClearFieldStatus();rs->m_rgFieldInfos[0].m_bStatus=0;run(CFieldExchange::LoadField);
    check(rs->m_rgFieldInfos[0].m_nLength==0 && value[0]==CH('z'),"raw text missing cache produces zero length without changing text");save();
    check(*reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL,"raw text operations preserve the recordset allocation guard");return result;
}
}
static void textBufferFieldExchange() {
    using namespace text_buffer_rfx_test;
    auto ansi=exercise<char>(&impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEADHHF_Z);
    auto wide=exercise<wchar_t>(&impl__RFX_Text__YAXPEAVCFieldExchange__PEB_WPEA_WHHF_Z);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_TEXT_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for raw text RFX comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);void** column=reinterpret_cast<void**>(base+0x3e9038);void** parameter=reinterpret_cast<void**>(base+0x3e9040);
            auto* savedColumn=*column;auto* savedParameter=*parameter;DWORD protection;
            ::VirtualProtect(column,16,PAGE_READWRITE,&protection);*column=reinterpret_cast<void*>(&SQLBindCol);*parameter=reinterpret_cast<void*>(&SQLBindParameter);
            check(exercise<char>(reinterpret_cast<Action<char>>(base+0x1ff140))==ansi,"ANSI buffer operations match native MFC flags, bytes and indicators");
            check(exercise<wchar_t>(reinterpret_cast<Action<wchar_t>>(base+0x1feca0))==wide,"Unicode buffer operations match native MFC flags, bytes and indicators");
            *column=savedColumn;*parameter=savedParameter;DWORD ignored;::VirtualProtect(column,16,protection,&ignored);::FreeLibrary(module);
        }
    }
}
