#pragma once
#include <malloc.h>
extern "C" void MS_ABI impl__RFX_Binary_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_JH_Z(CFieldExchange*,const wchar_t*,BYTE**,SQLLEN**,int);
extern "C" void MS_ABI impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEADPEAPEA_JH_Z(CFieldExchange*,const wchar_t*,char**,SQLLEN**,int);
extern "C" void MS_ABI impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEA_WPEAPEA_JH_Z(CFieldExchange*,const wchar_t*,wchar_t**,SQLLEN**,int);
extern "C" void MS_ABI impl__RFX_Bool_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(CFieldExchange*,const wchar_t*,int**,SQLLEN**);
extern "C" void MS_ABI impl__RFX_Byte_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_J_Z(CFieldExchange*,const wchar_t*,BYTE**,SQLLEN**);
extern "C" void MS_ABI impl__RFX_Date_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAUtagTIMESTAMP_STRUCT__PEAPEA_J_Z(CFieldExchange*,const wchar_t*,TIMESTAMP_STRUCT**,SQLLEN**);
extern "C" void MS_ABI impl__RFX_Double_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEANPEAPEA_J_Z(CFieldExchange*,const wchar_t*,double**,SQLLEN**);
extern "C" void MS_ABI impl__RFX_Int_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z(CFieldExchange*,const wchar_t*,int**,SQLLEN**);
extern "C" void MS_ABI impl__RFX_Long_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAJPEAPEA_J_Z(CFieldExchange*,const wchar_t*,long**,SQLLEN**);
extern "C" void MS_ABI impl__RFX_Single_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAMPEAPEA_J_Z(CFieldExchange*,const wchar_t*,float**,SQLLEN**);
extern "C" void MS_ABI impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z(CFieldExchange*,const wchar_t*,void*,SQLLEN*,int,SQLULEN);
namespace bulk_rfx_test {
using Action=void (MS_ABI*)(CFieldExchange*,const wchar_t*,void**,SQLLEN**,int);
using Defaults=decltype(&impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z);
struct Type {const char* name;Action action;int cType,bytes;bool sized,text,earlyPointers;size_t rva;};
std::array<Type,10> types() {return {{
    {"binary",reinterpret_cast<Action>(&impl__RFX_Binary_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_JH_Z),SQL_C_BINARY,1,true,false,false,0x203d30},
    {"text ANSI",reinterpret_cast<Action>(&impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEADPEAPEA_JH_Z),SQL_C_CHAR,1,true,true,false,0x203970},
    {"text Unicode",reinterpret_cast<Action>(&impl__RFX_Text_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEA_WPEAPEA_JH_Z),SQL_C_WCHAR,2,true,true,false,0x203870},
    {"bool",reinterpret_cast<Action>(&impl__RFX_Bool_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z),SQL_C_LONG,4,false,false,false,0x203a70},
    {"byte",reinterpret_cast<Action>(&impl__RFX_Byte_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAEPEAPEA_J_Z),SQL_C_TINYINT,1,false,false,false,0x203c50},
    {"date",reinterpret_cast<Action>(&impl__RFX_Date_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAUtagTIMESTAMP_STRUCT__PEAPEA_J_Z),SQL_C_TIMESTAMP,16,false,false,false,0x203b60},
    {"double",reinterpret_cast<Action>(&impl__RFX_Double_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEANPEAPEA_J_Z),SQL_C_DOUBLE,8,false,false,true,0x1fe4a0},
    {"int",reinterpret_cast<Action>(&impl__RFX_Int_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAHPEAPEA_J_Z),SQL_C_LONG,4,false,false,false,0x203a70},
    {"long",reinterpret_cast<Action>(&impl__RFX_Long_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAJPEAPEA_J_Z),SQL_C_LONG,4,false,false,false,0x203a70},
    {"single",reinterpret_cast<Action>(&impl__RFX_Single_Bulk__YAXPEAVCFieldExchange__PEB_WPEAPEAMPEAPEA_J_Z),SQL_C_FLOAT,4,false,false,true,0x1fe3b0}
}};}
std::vector<UINT64> exercise(const Type& type,Defaults defaults) {
    using namespace binary_rfx_test;
    recordset_storage_test::Fixture fixture(3,1);auto* rs=fixture.recordset;rs->m_hstmt=reinterpret_cast<HSTMT>(0xcafe);rs->m_dwRowsetSize=3;
    void* slots[27]{};slots[21]=reinterpret_cast<void*>(&checkDB);slots[24]=reinterpret_cast<void*>(&throwDB);*reinterpret_cast<void***>(rs)=slots;
    struct Outputs {void* values=nullptr;SQLLEN* lengths=nullptr;UINT64 guard=0xa5a5a5a5a5a5a5a5ULL;} output;
    auto run=[&](UINT operation,UINT fieldType=0,int maximum=7) {CFieldExchange fx(operation,rs);fx.SetFieldType(fieldType);type.action(&fx,L"bulk",&output.values,&output.lengths,maximum);return fx;};
    std::vector<UINT64> result;
    run(CFieldExchange::AllocMultiRowBuffer);
    size_t capacity=3*type.bytes*(type.sized?7:1);
    check(output.values && output.lengths && _msize(output.values)>=capacity && _msize(output.lengths)>=3*sizeof(SQLLEN),
        "bulk exchange allocates contiguous native value and SQLLEN row arrays");
    memset(output.values,0x5a,capacity);for(int i=0;i<3;++i)output.lengths[i]=i+1;
    resetSQL();auto column=run(CFieldExchange::BindFieldToColumn);
    check(binds==1 && checks==1 && !binding.param && binding.statement==rs->m_hstmt && binding.number==1 && binding.cType==type.cType &&
        binding.data==output.values && binding.length==output.lengths && binding.capacity==(type.sized?7:type.bytes),
        "bulk column binding passes native per-row capacity, array addresses and field number");
    result.push_back(binding.cType);result.push_back(binding.capacity);result.push_back(column.m_nFields);
    CString sql=L"prefix ";CFieldExchange names(CFieldExchange::Name,rs);names.SetFieldType(0);names.m_pstr=&sql;names.m_lpszSeparator=L", ";resetSQL();
    type.action(&names,L"first",&output.values,&output.lengths,7);type.action(&names,L"second",&output.values,&output.lengths,7);
    check(sql==L"prefix first, second, " && names.m_nFields==2 && !binds,"bulk names append every exchanged column and configured separator");
    names.m_lpszSeparator=nullptr;type.action(&names,L"third",&output.values,&output.lengths,7);
    check(sql==L"prefix first, second, third","bulk name generation treats a null separator as empty text");
    result.push_back(sql.GetLength());
    for(UINT operation: {UINT(CFieldExchange::Fixup),UINT(CFieldExchange::MarkForUpdate),UINT(CFieldExchange::StoreField),UINT(CFieldExchange::LoadField)}) {
        resetSQL();run(operation);check(!binds && output.lengths[2]==3 && static_cast<BYTE*>(output.values)[capacity-1]==0x5a,"unused bulk operations leave row storage and lengths untouched");
    }
    resetSQL(SQL_ERROR,false);int error=0;try {run(CFieldExchange::BindFieldToColumn);}catch(int code){error=code;}
    check(error==699 && checks==1 && throws==1 && !binding.statement,"bulk column errors use virtual recordset validation and the null-statement exception callback");
    run(CFieldExchange::DeleteMultiRowBuffer);check(!output.values && !output.lengths,"bulk deletion frees both row arrays and clears caller pointers");
    run(CFieldExchange::DeleteMultiRowBuffer);check(!output.values && !output.lengths,"bulk deletion accepts already empty arrays");
    output.values=reinterpret_cast<void*>(0x1234);output.lengths=reinterpret_cast<SQLLEN*>(0x5678);run(CFieldExchange::Name,4);
    check(type.text ? !output.values && !output.lengths : output.values==reinterpret_cast<void*>(0x1234) && output.lengths==reinterpret_cast<SQLLEN*>(0x5678),
        "skipped text bulk fields clear outputs while skipped scalar and binary fields preserve them");
    result.push_back(output.values==nullptr);result.push_back(output.lengths==nullptr);output.values=nullptr;output.lengths=nullptr;
    bool invalid=false;CFieldExchange skipped(CFieldExchange::Name,rs);skipped.SetFieldType(4);
    if(!type.text) {
        try {type.action(&skipped,L"bulk",nullptr,nullptr,7);}catch(int){invalid=true;}catch(CException* exception){invalid=true;exception->Delete();}
        check(invalid==type.earlyPointers,"floating bulk exchanges validate output arguments before field-type filtering");result.push_back(invalid);
    }
    if(type.sized) {
        rs->m_dwRowsetSize=65536;run(CFieldExchange::AllocMultiRowBuffer,0,65536);
        check(output.values && _msize(output.values)<=64 && output.lengths && _msize(output.lengths)>=65536*sizeof(SQLLEN),
            "bulk text and binary allocation preserves native signed 32-bit row-count multiplication wrap");
        run(CFieldExchange::DeleteMultiRowBuffer);rs->m_dwRowsetSize=3;
    }
    CFieldExchange direct(CFieldExchange::BindFieldToColumn,rs);direct.m_nFields=65538;resetSQL();
    defaults(&direct,nullptr,nullptr,nullptr,SQL_C_BINARY,UINT64(-7));
    check(binding.number==2 && binding.capacity==-7 && binding.data==nullptr && binding.length==nullptr,
        "bulk default binds native truncated field numbers and signed lengths without extra argument checks");
    result.push_back(binding.number);result.push_back(binding.capacity);
    direct.m_nOperation=CFieldExchange::Name;direct.m_pstr=&sql;direct.m_lpszSeparator=L";";sql.Empty();defaults(&direct,L"direct",nullptr,nullptr,0,0);
    check(sql==L"direct;","bulk default directly generates a column name and separator");
    check(output.guard==0xa5a5a5a5a5a5a5a5ULL && *reinterpret_cast<UINT64*>(fixture.storage+440)==0xa5a5a5a5a5a5a5a5ULL,"bulk operations preserve caller output and native recordset allocation guards");
    return result;
}
}
static void bulkFieldExchange() {
    using namespace bulk_rfx_test;
    auto list=types();std::vector<std::vector<UINT64>> results;
    for(const auto& type:list) {printf("bulk RFX: %s\n",type.name);results.push_back(exercise(type,&impl__AfxRFXBulkDefault__YAXPEAVCFieldExchange__PEB_WPEAXPEA_JH_K_Z));}
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_BULK_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for bulk RFX comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);void** slot=reinterpret_cast<void**>(base+0x3e9038);auto* saved=*slot;DWORD protection;
            ::VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=reinterpret_cast<void*>(&SQLBindCol);
            for(size_t i=0;i<list.size();++i) {auto native=list[i];native.action=reinterpret_cast<Action>(base+native.rva);
                check(exercise(native,reinterpret_cast<Defaults>(base+0x203e20))==results[i],"bulk allocation, SQL names, array binding, skips and cleanup match native MFC");}
            *slot=saved;DWORD ignored;::VirtualProtect(slot,8,protection,&ignored);::FreeLibrary(module);
        }
    }
}
