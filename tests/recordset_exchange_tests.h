#pragma once
extern "C" UINT MS_ABI impl__BindParams_CRecordset__QEAAIPEAX_Z(CRecordset*,void*);
extern "C" void MS_ABI impl__RebindParams_CRecordset__QEAAXPEAX_Z(CRecordset*,void*);
extern "C" UINT MS_ABI impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,CString*,const wchar_t*);
extern "C" UINT MS_ABI impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,void*,CString*,const wchar_t*);
extern "C" UINT MS_ABI impl__AppendNamesValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CRecordset*,void*,CString*,const wchar_t*);
namespace recordset_exchange_test {
struct API {
    decltype(&impl__BindParams_CRecordset__QEAAIPEAX_Z) bind;
    decltype(&impl__RebindParams_CRecordset__QEAAXPEAX_Z) rebind;
    decltype(&impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z) names;
    decltype(&impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z) values,pairs;
};
static std::vector<INT64> events;
static UINT count;
static int failure,deleted;
static CString* output;
struct Failure:CDBException{~Failure() override{++deleted;}};
static void exchange(CRecordset* rs,CFieldExchange* fx,bool bulk) {
    check(fx->m_prs==rs && fx->m_nFieldType==UINT(-1) && !fx->m_pvField && !fx->m_bField &&
        !fx->m_nFields && !fx->m_nParams && !fx->m_nParamFields && fx->m_lDefaultLBFetchSize==65536 && fx->m_lDefaultLBReallocSize==65536,
        "SQL field enumeration starts with native exchange defaults and the owning recordset");
    events.insert(events.end(),{1,fx->m_nOperation,bulk,INT64(UINT_PTR(fx->m_hstmt)),fx->m_pstr==output,fx->m_pstr!=nullptr,
        fx->m_nOperation>=8 && fx->m_lpszSeparator!=nullptr,rs->m_bRebindParams});
    if(failure==1)throw 77;
    if(failure==2)throw static_cast<CException*>(new Failure);
    fx->m_nFields=count;fx->m_nParams=count^0x76543210;
    if(fx->m_pstr)*fx->m_pstr+=L"callback";
}
static void MS_ABI ordinary(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,false);}
static void MS_ABI bulk(CRecordset* rs,CFieldExchange* fx){exchange(rs,fx,true);}
static std::vector<INT64> exercise(const API& api) {
    std::vector<INT64> result;
    for(int method=0;method<5;++method)for(bool multi:{false,true})for(BOOL rebind:{FALSE,BOOL(69)})for(UINT fields:{0u,1u,0x80000000u,0xffffffffu})for(int missing=0;missing<6;++missing)for(int fail=0;fail<3;++fail) {
        if(method==1 && missing==5)continue;
        CRecordset rs;rs.m_hstmt=missing==4?nullptr:reinterpret_cast<HSTMT>(0xbeef);rs.m_dwOptions=multi?0x200:0;rs.m_bRebindParams=rebind;
        auto** original=*reinterpret_cast<void***>(&rs);void* table[27];memcpy(table,original,sizeof(table));table[19]=reinterpret_cast<void*>(&ordinary);table[20]=reinterpret_cast<void*>(&bulk);*reinterpret_cast<void***>(&rs)=table;
        CString sql=L"prefix ";output=missing==2?nullptr:&sql;void* statement=missing==1?nullptr:reinterpret_cast<void*>(0xcafe);
        const wchar_t* separator=missing==3?nullptr:L", ";CRecordset* self=missing==5?nullptr:&rs;
        events.clear();count=fields;failure=fail;deleted=0;UINT returned=0xcccccccc;int caught=0;
        try {
            switch(method) {
            case 0:returned=api.bind(self,statement);break;case 1:api.rebind(self,statement);break;
            case 2:returned=api.names(self,output,separator);break;case 3:returned=api.values(self,statement,output,separator);break;
            case 4:returned=api.pairs(self,statement,output,separator);break;
            }
        }catch(int value){caught=value==19?-1:value;}catch(CException* error){caught=fail==2 && !events.empty()?-2:-1;error->Delete();}
        bool skip=method==1 && !rebind;
        bool invalid=!skip && (missing==4 || missing==5 || (method==4 && missing));
        check(invalid?caught==-1 && events.empty():skip?!caught && events.empty():events.size()==8 && caught==(fail==1?77:fail==2?-2:0),
            "binding and SQL enumeration preserve native validation, the rebind gate and callback exception propagation");
        if(!caught && !skip && method!=1)check(returned==(method==0?(fields^0x76543210):fields),
            "binding returns the unsigned parameter count while SQL append helpers return the unsigned field count");
        result.insert(result.end(),events.begin(),events.end());result.insert(result.end(),{90,method,returned,caught,deleted,rs.m_bRebindParams,sql.GetLength()});
        for(wchar_t ch:std::wstring(sql.GetString()))result.push_back(ch);result.push_back(-1);
        rs.m_hstmt=nullptr;*reinterpret_cast<void***>(&rs)=original;
    }
    return result;
}
}
static void recordsetExchange() {
    using namespace recordset_exchange_test;
    API local{&impl__BindParams_CRecordset__QEAAIPEAX_Z,&impl__RebindParams_CRecordset__QEAAXPEAX_Z,
        &impl__AppendNames_CRecordset__QEAAIPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z,
        &impl__AppendValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z,
        &impl__AppendNamesValues_CRecordset__QEAAIPEAXPEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z};
    auto expected=exercise(local);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_EXCHANGE_DLL",path,1024)) {
        HMODULE module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for SQL field enumeration comparison");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"SQL field enumeration oracle uses the verified MFC image build");
            if(pinned) {
                API native{reinterpret_cast<decltype(local.bind)>(base+0x1fcc10),reinterpret_cast<decltype(local.rebind)>(base+0x1fcc70),
                    reinterpret_cast<decltype(local.names)>(base+0x1fce40),reinterpret_cast<decltype(local.values)>(base+0x1fcf50),reinterpret_cast<decltype(local.pairs)>(base+0x1fcec0)};
                check(exercise(native)==expected,"parameter binding, rebind gating, bulk name enumeration, SQL callbacks and interrupted state match native MFC");
            }
            ::FreeLibrary(module);
        }
    }
}
