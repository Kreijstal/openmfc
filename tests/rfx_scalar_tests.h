#pragma once
#include <array>
extern "C" void MS_ABI impl__RFX_BigInt__YAXPEAVCFieldExchange__PEB_WAEA_J_Z(CFieldExchange*,const wchar_t*,__int64*);
extern "C" void MS_ABI impl__RFX_Bool__YAXPEAVCFieldExchange__PEB_WAEAH_Z(CFieldExchange*,const wchar_t*,int*);
extern "C" void MS_ABI impl__RFX_Byte__YAXPEAVCFieldExchange__PEB_WAEAE_Z(CFieldExchange*,const wchar_t*,BYTE*);
extern "C" void MS_ABI impl__RFX_Int__YAXPEAVCFieldExchange__PEB_WAEAH_Z(CFieldExchange*,const wchar_t*,int*);
extern "C" void MS_ABI impl__RFX_Long__YAXPEAVCFieldExchange__PEB_WAEAJ_Z(CFieldExchange*,const wchar_t*,long*);
extern "C" void MS_ABI impl__RFX_Single__YAXPEAVCFieldExchange__PEB_WAEAM_Z(CFieldExchange*,const wchar_t*,float*);
extern "C" void MS_ABI impl__RFX_Double__YAXPEAVCFieldExchange__PEB_WAEAN_Z(CFieldExchange*,const wchar_t*,double*);
extern "C" void MS_ABI impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAUtagTIMESTAMP_STRUCT___Z(CFieldExchange*,const wchar_t*,TIMESTAMP_STRUCT*);
extern "C" void MS_ABI impl__SetFieldNull_CRecordset__QEAAXPEAXH_Z(CRecordset*,void*,BOOL);
namespace scalar_rfx_test {
using Action=void (MS_ABI*)(CFieldExchange*,const wchar_t*,void*);
struct Type {
    const char* name;Action action;size_t bytes;int cache,cType;SQLULEN precision;size_t rva;
    alignas(8) std::array<BYTE,16> null{},real{};
};
template<class T> void Put(std::array<BYTE,16>& buffer,T value) {memcpy(buffer.data(),&value,sizeof(value));}
std::array<Type,8> types() {
    std::array<Type,8> list{{
        {"bigint",reinterpret_cast<Action>(&impl__RFX_BigInt__YAXPEAVCFieldExchange__PEB_WAEA_J_Z),8,22,SQL_C_SBIGINT,19,0x200720},
        {"bool",reinterpret_cast<Action>(&impl__RFX_Bool__YAXPEAVCFieldExchange__PEB_WAEAH_Z),4,1,SQL_C_BIT,1,0x200c20},
        {"byte",reinterpret_cast<Action>(&impl__RFX_Byte__YAXPEAVCFieldExchange__PEB_WAEAE_Z),1,2,SQL_C_TINYINT,3,0x200a80},
        {"int",reinterpret_cast<Action>(&impl__RFX_Int__YAXPEAVCFieldExchange__PEB_WAEAH_Z),4,3,SQL_C_LONG,5,0x200580},
        {"long",reinterpret_cast<Action>(&impl__RFX_Long__YAXPEAVCFieldExchange__PEB_WAEAJ_Z),4,4,SQL_C_LONG,10,0x2008e0},
        {"single",reinterpret_cast<Action>(&impl__RFX_Single__YAXPEAVCFieldExchange__PEB_WAEAM_Z),4,6,SQL_C_FLOAT,13,0x1fe030},
        {"double",reinterpret_cast<Action>(&impl__RFX_Double__YAXPEAVCFieldExchange__PEB_WAEAN_Z),8,7,SQL_C_DOUBLE,22,0x1fe1e0},
        {"timestamp",reinterpret_cast<Action>(&impl__RFX_Date__YAXPEAVCFieldExchange__PEB_WAEAUtagTIMESTAMP_STRUCT___Z),16,15,SQL_C_TIMESTAMP,23,0x201be0}
    }};
    Put(list[0].null,__int64(0x4a4d4120));Put(list[0].real,__int64(7));
    Put(list[1].null,int(2));Put(list[1].real,int(7));
    Put(list[2].null,BYTE(0xff));Put(list[2].real,BYTE(7));
    Put(list[3].null,int(0x7ee4));Put(list[3].real,int(7));
    Put(list[4].null,long(0x4a4d4120));Put(list[4].real,long(7));
    Put(list[5].null,uint32_t(0xe09e4242));Put(list[5].real,float(1.25));
    Put(list[6].null,uint64_t(0xc413c84846922b8cULL));Put(list[6].real,double(3.5));
    Put(list[7].null,TIMESTAMP_STRUCT{99,99,99,99,99,99,99});Put(list[7].real,TIMESTAMP_STRUCT{2026,10,14,12,30,45,17});
    return list;
}
static const Type* selectedType;
static void* selectedValue;
static int exchangeCalls;
static bool validExchange;
static void MS_ABI fieldExchange(CRecordset* rs,CFieldExchange* fx) {
    ++exchangeCalls;
    validExchange=fx->m_nOperation==CFieldExchange::SetFieldNull && fx->m_prs==rs && !fx->m_nFieldFound;
    fx->SetFieldType(0);selectedType->action(fx,L"value",selectedValue);
}
using SetNull=void (MS_ABI*)(CRecordset*,void*,BOOL);
std::vector<UINT64> exercise(const Type& type,SetNull setNull) {
    using namespace binary_rfx_test;
    recordset_storage_test::Fixture fixture(1,1);auto* rs=fixture.recordset;
    rs->AllocStatusArrays();rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);*reinterpret_cast<void***>(rs)=slots;
    CODBCFieldInfo meta{};meta.m_nSQLType=SQL_INTEGER;meta.m_nPrecision=19;meta.m_nScale=2;rs->m_rgODBCFieldInfos=&meta;
    alignas(8) std::array<BYTE,24> storage;storage.fill(0xa5);void* value=storage.data();
    auto real=[&] {memcpy(value,type.real.data(),type.bytes);};real();
    auto run=[&](UINT operation,UINT fieldType=0) {CFieldExchange fx(operation,rs);fx.SetFieldType(fieldType);fx.m_hstmt=reinterpret_cast<HSTMT>(0xbeef);
        type.action(&fx,L"value",value);return fx;};
    std::vector<UINT64> result;
    auto save=[&] {result.push_back(rs->m_pbFieldFlags[0]);result.push_back(rs->m_rgFieldInfos[0].m_nLength);
        for(size_t i=0;i<type.bytes;++i)result.push_back(storage[i]);};
    resetSQL();run(CFieldExchange::BindFieldToColumn);
    check(binds==1 && checks==1 && !binding.param && binding.cType==type.cType && binding.capacity==SQLLEN(type.bytes) &&
        binding.data==value && binding.length==&rs->m_rgFieldInfos[0].m_nLength && rs->GetBoundFieldIndex(value)==1,
        "scalar column binding uses native type, byte size, length address and one-based index");
    resetSQL();run(CFieldExchange::BindParam,4);
    check(binding.param && binding.direction==4 && binding.statement==reinterpret_cast<HSTMT>(0xbeef) && binding.cType==type.cType &&
        binding.sqlType==type.cType && binding.precision==type.precision && rs->m_plParamLength[0]==SQLLEN(type.bytes),
        "scalar parameter binding uses native direction, exchange statement and precision");
    CFieldExchange fx(CFieldExchange::SetFieldNull,rs);fx.SetFieldType(0);fx.m_bField=TRUE;
    type.action(&fx,L"value",value);
    check(!memcmp(value,type.null.data(),type.bytes) && rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1,
        "scalar SetFieldNull writes its exact native sentinel and SQL_NULL_DATA");save();
    fx.m_nFields=0;fx.m_bField=FALSE;type.action(&fx,L"value",value);
    check(!rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==SQLLEN(type.bytes) && !memcmp(value,type.null.data(),type.bytes),
        "scalar null clearing restores byte length without rewriting the sentinel");
    int other=0;fx.m_nFields=0;fx.m_bField=TRUE;fx.m_pvField=&other;real();type.action(&fx,L"value",value);
    check(!memcmp(value,type.real.data(),type.bytes) && !rs->IsFieldStatusNull(0),"scalar null operation ignores a different selected field");
    fx.m_nFields=0;fx.m_pvField=value;type.action(&fx,L"value",value);
    check(!memcmp(value,type.null.data(),type.bytes) && rs->IsFieldStatusNull(0)==2,"scalar null operation targets the selected value address");
    real();rs->m_rgFieldInfos[0].m_nLength=SQL_NULL_DATA;run(CFieldExchange::Fixup);
    check(!memcmp(value,type.null.data(),type.bytes) && rs->IsFieldStatusNull(0)==2,"scalar null Fixup installs the exact sentinel");save();
    rs->ClearDirtyFieldStatus(0);run(CFieldExchange::MarkForAddNew);
    check(!rs->IsFieldStatusDirty(0) && rs->IsFieldStatusNull(0)==2,"pseudo-null scalar remains clean during MarkForAddNew");
    real();run(CFieldExchange::MarkForAddNew);
    check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0),"real scalar becomes dirty and non-null during MarkForAddNew");save();
    rs->ClearFieldStatus();run(CFieldExchange::AllocCache);
    bool heap=type.cache==7 || type.cache==15 || type.cache==22;
    check(rs->m_rgFieldInfos[0].m_nDataType==type.cache && (!!rs->m_rgFieldInfos[0].m_pvDataCache)==heap,
        "scalar AllocCache sets its native type and allocates only non-inline value storage");
    run(CFieldExchange::StoreField);storage[0]^=1;
    auto changed=run(CFieldExchange::MarkForUpdate);
    check(rs->IsFieldStatusDirty(0)==1 && changed.m_bField,"scalar cache comparison marks changed values and reports the selected field");save();
    rs->ClearFieldStatus();run(CFieldExchange::LoadField);
    check(!memcmp(value,type.real.data(),type.bytes) && rs->m_rgFieldInfos[0].m_nLength==SQLLEN(type.bytes),"scalar LoadField restores cached bytes and native length");
    auto clean=run(CFieldExchange::MarkForUpdate);
    check(!rs->IsFieldStatusDirty(0) && !clean.m_bField,"unchanged scalar cache value remains clean");
    rs->SetNullFieldStatus(0);run(CFieldExchange::StoreField);rs->ClearFieldStatus();real();run(CFieldExchange::LoadField);
    check(rs->IsFieldStatusNull(0)==2 && rs->m_rgFieldInfos[0].m_nLength==-1 && !memcmp(value,type.real.data(),type.bytes),
        "cached-null scalar restore leaves value bytes untouched");
    changed=run(CFieldExchange::MarkForUpdate);
    check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0) && changed.m_bField,"real scalar after cached null becomes dirty");
    CString sql;fx=CFieldExchange(CFieldExchange::NameValue,rs);fx.SetFieldType(0);fx.m_pstr=&sql;fx.m_lpszSeparator=L", ";fx.m_hstmt=reinterpret_cast<HSTMT>(0xface);resetSQL();
    type.action(&fx,L"value",value);
    check(sql==L"value=?, " && binding.param && binding.cType==type.cType && binding.sqlType==SQL_INTEGER && binding.precision==19 && binding.scale==2,
        "scalar NameValue builds SQL and binds with native column metadata");
    rs->ClearFieldStatus();run(CFieldExchange::BindFieldForUpdate);
    check(rs->m_rgFieldInfos[0].m_nLength==SQL_IGNORE,"scalar update binding ignores a clean field");
    rs->SetDirtyFieldStatus(0);rs->SetNullFieldStatus(0);run(CFieldExchange::BindFieldForUpdate);
    check(rs->m_rgFieldInfos[0].m_nLength==-1,"scalar update binding keeps null status");
    run(CFieldExchange::UnbindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==-1,"scalar unbind preserves a dirty field's update indicator");
    rs->ClearFieldStatus();run(CFieldExchange::UnbindFieldForUpdate);check(rs->m_rgFieldInfos[0].m_nLength==SQLLEN(type.bytes),"scalar unbind restores clean field byte size");
    rs->m_rgFieldInfos[0].m_nLength=SQLLEN(type.bytes);real();
    if(type.cache==1) {int raw=0xaabbcc05;memcpy(value,&raw,4);run(CFieldExchange::Fixup);check(*static_cast<int*>(value)==5,"bool non-null Fixup normalizes from the fetched low byte only");}
    else {run(CFieldExchange::Fixup);check(!memcmp(value,type.real.data(),type.bytes),"non-bool scalar Fixup preserves non-null value bytes");}
    auto input=run(CFieldExchange::Fixup,1);check(input.m_nParams==1,"input scalar parameters are counted while Fixup is skipped");
    rs->m_plParamLength[0]=SQL_NULL_DATA;real();run(CFieldExchange::Fixup,2);
    check(!memcmp(value,type.null.data(),type.bytes),"output scalar Fixup reads the native parameter length");
    if(type.cache==6 || type.cache==7) {
        if(type.cache==6) {float nan=std::numeric_limits<float>::quiet_NaN();memcpy(value,&nan,4);}else {double nan=std::numeric_limits<double>::quiet_NaN();memcpy(value,&nan,8);}
        rs->SetNullFieldStatus(0);rs->ClearDirtyFieldStatus(0);run(CFieldExchange::MarkForAddNew);
        check(rs->IsFieldStatusDirty(0)==1 && !rs->IsFieldStatusNull(0),"floating NaN is a real value rather than the native pseudo-null sentinel");
    }
    bool failed=false;CFieldExchange invalid(CFieldExchange::Fixup,rs);invalid.SetFieldType(0);
    try {type.action(&invalid,nullptr,value);}catch(...) {failed=true;}
    check(failed && !invalid.m_nFields,"scalar name validation fails before field enumeration");
    failed=false;invalid.m_nFields=1;try {type.action(&invalid,L"value",value);}catch(CUserException*) {failed=true;}
    check(failed,"scalar field bounds raise the native typed user exception");
    bool guard=true;for(size_t i=type.bytes;i<storage.size();++i)guard &= storage[i]==0xa5;
    check(guard,"scalar operations preserve bytes beyond the caller's native value size");
    rs->m_mapParamIndex.RemoveAll(); // Earlier parameter-binding probes used the same value address.
    slots[19]=reinterpret_cast<void*>(&fieldExchange);selectedType=&type;selectedValue=value;
    exchangeCalls=0;validExchange=false;rs->ClearFieldStatus();real();setNull(rs,value,TRUE);
    check(exchangeCalls==1 && validExchange && rs->IsFieldStatusNull(0)==2 && !memcmp(value,type.null.data(),type.bytes),
        "recordset SetFieldNull routes scalar values through native operation eleven and virtual slot 19");
    setNull(rs,nullptr,FALSE);
    check(exchangeCalls==2 && !rs->IsFieldStatusNull(0) && rs->m_rgFieldInfos[0].m_nLength==SQLLEN(type.bytes),
        "recordset all-field null clearing runs the native scalar exchange pass");
    int parameter=0;rs->m_mapParamIndex.SetAt(&parameter,reinterpret_cast<void*>(1));setNull(rs,&parameter,37);
    check(exchangeCalls==2 && rs->IsParamStatusNull(0)==2,"recordset parameter null selection updates flags without a field-exchange callback");
    setNull(rs,&parameter,FALSE);check(exchangeCalls==2 && !rs->IsParamStatusNull(0),"recordset parameter null clearing preserves its value and skips field exchange");
    rs->FreeDataCache();return result;
}
}
static void scalarFieldExchange() {
    using namespace scalar_rfx_test;
    auto list=types();std::vector<std::vector<UINT64>> expected;
    for(const auto& type:list) {printf("scalar RFX: %s\n",type.name);expected.push_back(exercise(type,&impl__SetFieldNull_CRecordset__QEAAXPEAXH_Z));}
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_SCALAR_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for scalar RFX comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);
            void** column=reinterpret_cast<void**>(base+0x3e9038);void** parameter=reinterpret_cast<void**>(base+0x3e9040);
            auto* savedColumn=*column;auto* savedParameter=*parameter;DWORD oldProtection;
            ::VirtualProtect(column,16,PAGE_READWRITE,&oldProtection);*column=reinterpret_cast<void*>(&SQLBindCol);*parameter=reinterpret_cast<void*>(&SQLBindParameter);
            for(size_t i=0;i<list.size();++i) {auto native=list[i];native.action=reinterpret_cast<Action>(base+native.rva);check(exercise(native,reinterpret_cast<SetNull>(base+0x1f9980))==expected[i],"scalar values, flags, caches and lengths match native MFC");}
            *column=savedColumn;*parameter=savedParameter;DWORD ignored;::VirtualProtect(column,16,oldProtection,&ignored);::FreeLibrary(module);
        }
    }
}
