#pragma once
#include <exception>
extern "C" void* MS_ABI impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z(CDBVariant*,short,SQLLEN*,short,SQLULEN);
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z(CDatabase*,HSTMT,short,SQLLEN,void**,SQLLEN,CString*,short,short);
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z(CDatabase*,HSTMT,short,SQLLEN,void**,SQLLEN,void*,short,short);
extern "C" void MS_ABI impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z(CDatabase*,HSTMT,short,SQLLEN,void**,SQLLEN,CDBVariant*,short);
extern "C" short MS_ABI impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z(CRecordset*,const wchar_t*);
extern "C" void MS_ABI impl__GetODBCFieldInfo_CRecordset__QEAAXFAEAUCODBCFieldInfo___Z(CRecordset*,short,CODBCFieldInfo*);
extern "C" void MS_ABI impl__GetODBCFieldInfo_CRecordset__QEAAXPEB_WAEAUCODBCFieldInfo___Z(CRecordset*,const wchar_t*,CODBCFieldInfo*);
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAVCDBVariant__F_Z(CRecordset*,short,CDBVariant*,short);
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAVCDBVariant__F_Z(CRecordset*,const wchar_t*,CDBVariant*,short);
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CRecordset*,short,CString*);
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(CRecordset*,const wchar_t*,CString*);
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(CRecordset*,short,void*);
extern "C" void MS_ABI impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z(CRecordset*,const wchar_t*,void*);
extern "C" BOOL MS_ABI impl__IsFieldNullable_CRecordset__QEBAHK_Z(const CRecordset*,DWORD);
extern "C" BOOL MS_ABI impl__IsFieldNullable_CRecordset__QEAAHPEAX_Z(CRecordset*,void*);
namespace recordset_value_test {
using namespace recordset_move_test;
struct ValueAPI {
    decltype(&impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z) buffer;
    decltype(&impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z) wide;
    decltype(&impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z) narrow;
    decltype(&impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z) binary;
};
struct FieldAPI {
    decltype(&impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z) index;
    decltype(&impl__GetODBCFieldInfo_CRecordset__QEAAXFAEAUCODBCFieldInfo___Z) info;
    decltype(&impl__GetODBCFieldInfo_CRecordset__QEAAXPEB_WAEAUCODBCFieldInfo___Z) namedInfo;
    decltype(&impl__GetFieldValue_CRecordset__QEAAXFAEAVCDBVariant__F_Z) variant;
    decltype(&impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAVCDBVariant__F_Z) namedVariant;
    decltype(&impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z) wide;
    decltype(&impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z) namedWide;
    decltype(&impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z) narrow;
    decltype(&impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z) namedNarrow;
    decltype(&impl__IsFieldNullable_CRecordset__QEBAHK_Z) nullable;
    decltype(&impl__IsFieldNullable_CRecordset__QEAAHPEAX_Z) boundNullable;
};
using GlobalAllocate=HGLOBAL(WINAPI*)(UINT,SIZE_T);
using GlobalReallocate=HGLOBAL(WINAPI*)(HGLOBAL,SIZE_T,UINT);
static GlobalAllocate realAllocate;
static GlobalReallocate realReallocate;
static bool captureGlobals,failAllocate,failReallocate;
static HGLOBAL WINAPI allocate(UINT flags,SIZE_T bytes) {
    if(captureGlobals)event(80,flags,INT64(bytes),failAllocate);
    if(captureGlobals && failAllocate){::SetLastError(ERROR_NOT_ENOUGH_MEMORY);return nullptr;}
    return realAllocate(flags,bytes);
}
static HGLOBAL WINAPI reallocate(HGLOBAL handle,SIZE_T bytes,UINT flags) {
    if(captureGlobals)event(81,flags,INT64(bytes),failReallocate);
    if(captureGlobals && failReallocate){::SetLastError(ERROR_NOT_ENOUGH_MEMORY);return nullptr;}
    return realReallocate(handle,bytes,flags);
}
static void** import(HMODULE module,const char* name) {
    auto* base=reinterpret_cast<BYTE*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
    auto* descriptor=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(base+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;descriptor->Name;++descriptor) {
        auto* names=reinterpret_cast<IMAGE_THUNK_DATA*>(base+descriptor->OriginalFirstThunk);auto* slots=reinterpret_cast<IMAGE_THUNK_DATA*>(base+descriptor->FirstThunk);
        for(;names->u1.AddressOfData;++names,++slots)if(!IMAGE_SNAP_BY_ORDINAL(names->u1.Ordinal)) {
            auto* entry=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base+names->u1.AddressOfData);
            if(!strcmp(reinterpret_cast<char*>(entry->Name),name))return reinterpret_cast<void**>(&slots->u1.Function);
        }
    }
    return nullptr;
}
struct Patch {
    void** slot;void* saved;DWORD protection;
    Patch(void** address,void* function):slot(address),saved(*address) {::VirtualProtect(slot,sizeof(void*),PAGE_READWRITE,&protection);*slot=function;}
    ~Patch(){*slot=saved;DWORD ignored;::VirtualProtect(slot,sizeof(void*),protection,&ignored);}
};
struct Step {RETCODE code;SQLLEN length;std::vector<BYTE> bytes;};
static std::vector<Step> steps;
static size_t nextStep;
static void* stringObject;
static CLongBinary* activeBinary;
static CDBVariant* activeVariant;
static bool terminatedPayloads;
static bool ownedVariantData;
static int driverFailure;
static SQLRETURN SQL_API data(SQLHSTMT statement,SQLUSMALLINT field,SQLSMALLINT type,SQLPOINTER output,SQLLEN capacity,SQLLEN* indicator) {
    void* base=stringObject?*static_cast<void**>(stringObject):nullptr;
    if(activeVariant) {
        if(activeVariant->m_dwType>=DBVT_DATE && activeVariant->m_dwType<=DBVT_WSTRING)ownedVariantData=true;
        if(activeVariant->m_dwType==DBVT_ASTRING || activeVariant->m_dwType==DBVT_WSTRING)base=*reinterpret_cast<void**>(activeVariant->m_pstring);
        else if(activeVariant->m_dwType==DBVT_BINARY)activeBinary=activeVariant->m_pbinary;
        else if(activeVariant->m_dwType==DBVT_DATE)base=activeVariant->m_pdate;
        else base=&activeVariant->m_lVal;
    }
    if(activeBinary){base=::GlobalLock(activeBinary->m_hData);::GlobalUnlock(activeBinary->m_hData);}
    event(82,INT64(UINT_PTR(statement)),field,type,capacity,output?INT64(UINT_PTR(output)-UINT_PTR(base)):-1);
    if(nextStep>=steps.size())std::abort();auto step=steps[nextStep++];
    if(driverFailure==1)throw 77;
    if(driverFailure==2)throw static_cast<CException*>(new CDBException(77));
    *indicator=step.length;
    if(output && capacity>0 && !step.bytes.empty()) {
        if(terminatedPayloads && (type==SQL_C_CHAR || type==SQL_C_WCHAR)) {
            size_t width=type==SQL_C_WCHAR?2:1;
            if(size_t(capacity)>=width){size_t count=std::min(size_t(capacity)-width,step.bytes.size());count-=count%width;memcpy(output,step.bytes.data(),count);memset(static_cast<BYTE*>(output)+count,0,width);}
        } else memcpy(output,step.bytes.data(),std::min<size_t>(size_t(capacity),step.bytes.size()));
    }
    return step.code;
}
template<class F> static int invoke(F action) {
    try{action();return 0;}catch(const std::bad_alloc&){return 2;}
    catch(CMemoryException* error){error->Delete();return 2;}
    catch(COleException* error){int code=error->m_sc;error->Delete();return code;}
    catch(CDBException* error){int code=1000+error->m_nRetCode;error->Delete();return code;}
    catch(CException* error){error->Delete();return 1;}catch(int code){return code==19?1:code;}
}
static void reset() {
    events.clear();captureGlobals=false;failAllocate=failReallocate=false;driverFailure=0;stringObject=nullptr;activeBinary=nullptr;activeVariant=nullptr;terminatedPayloads=false;ownedVariantData=false;nextStep=0;
    steps={{SQL_STILL_EXECUTING,0,{}},{SQL_SUCCESS,2,{'Z',0}}};longbinary_rfx_test::reset({},{{L"HY000",5,L"[Driver] field retrieval failed"}});
}
static void save(std::vector<Event>& result,int caught) {
    event(83,caught,INT64(nextStep),INT64(longbinary_rfx_test::diagnosticReads.size()));result.insert(result.end(),events.begin(),events.end());
}
static std::vector<Event> buffers(const ValueAPI& functions) {
    std::vector<Event> result;
    auto run=[&](short type,short sqlType,SQLULEN precision,bool missing=false,bool allocationFailure=false) {
        reset();CDBVariant value;UINT64 bits=0xa5a5a5a5a5a5a5a5ULL;memcpy(&value.m_lVal,&bits,8);SQLLEN length=73;void* returned=reinterpret_cast<void*>(1);
        captureGlobals=true;failAllocate=allocationFailure;
        int caught=invoke([&]{returned=functions.buffer(&value,type,missing?nullptr:&length,sqlType,precision);});captureGlobals=false;
        event(84,value.m_dwType,length,returned!=nullptr,returned==&value.m_lVal);
        if(value.m_dwType==DBVT_ASTRING || value.m_dwType==DBVT_WSTRING) {
            auto* string=value.m_pstring;auto* bytes=*reinterpret_cast<void**>(string);auto* header=reinterpret_cast<CStringData*>(bytes)-1;
            // Each module owns a different number of unrelated nil strings.
            // Compare the local value's ownership state, not that global count.
            event(85,header->nDataLength,header->nAllocLength,header->nRefs>1?2:header->nRefs,returned==bytes);
        } else if(value.m_dwType==DBVT_BINARY) {
            auto* binary=value.m_pbinary;
            event(86,binary->m_hData!=nullptr,binary->m_hData?INT64(binary->m_dwDataLength):0,binary->m_hData?::GlobalFlags(binary->m_hData)&GMEM_LOCKCOUNT:0);
            if(binary->m_hData)while(::GlobalUnlock(binary->m_hData)){}
        } else if(value.m_dwType==DBVT_DATE)event(87,value.m_pdate!=nullptr,returned==value.m_pdate);
        else {memcpy(&bits,&value.m_lVal,8);event(88,INT64(bits));}
        save(result,caught);
    };
    for(short type:{short(SQL_C_BIT),short(SQL_C_UTINYINT),short(SQL_C_SSHORT),short(SQL_C_SLONG),short(SQL_C_FLOAT),short(SQL_C_DOUBLE),
                    short(SQL_C_TIMESTAMP),short(SQL_C_CHAR),short(SQL_C_WCHAR),short(SQL_C_BINARY),short(0),short(4),short(93)})
    for(short sqlType:{short(SQL_LONGVARBINARY),short(SQL_LONGVARCHAR),short(SQL_WLONGVARCHAR),short(SQL_VARCHAR),short(SQL_NUMERIC)})
    for(SQLULEN precision:{SQLULEN(0),SQLULEN(1),SQLULEN(7),SQLULEN(33)})run(type,sqlType,precision);
    for(short type:{short(SQL_C_CHAR),short(SQL_C_WCHAR),short(SQL_C_BINARY),short(0)}) {
        run(type,SQL_VARCHAR,7,true);run(type,SQL_VARCHAR,1ULL<<40,false,true);run(type,SQL_VARCHAR,SQLULEN(-1),false,true);
    }
    return result;
}
struct Manager:database_strings_test::Manager {
    CStringData* Reallocate(CStringData* old,int length,int size) override {
        int capacity=old->nAllocLength;auto* fresh=database_strings_test::Manager::Reallocate(old,length,size);
        if(length>capacity)memset(static_cast<BYTE*>(static_cast<void*>(fresh+1))+size_t(capacity+1)*size,0,size_t(length-capacity)*size);
        return fresh;
    }
};
static std::vector<Event> text(const ValueAPI& functions) {
    std::vector<Event> result;
    auto run=[&](bool wide,short sqlType,SQLLEN needed,SQLLEN initial,RETCODE code,bool locked=false,bool shared=false,bool cloneElsewhere=false,bool noPosition=false,int failAt=0) {
        reset();Manager manager,other;manager.clone=cloneElsewhere?&other:&manager;int size=wide?2:1;
        auto* original=manager.Allocate(128,size);original->nDataLength=8;original->nRefs=locked?-1:shared?2:1;
        for(int i=0;i<128;++i){if(wide)reinterpret_cast<wchar_t*>(original+1)[i]=wchar_t('a'+i%26);else reinterpret_cast<char*>(original+1)[i]=char('a'+i%26);}
        if(wide)reinterpret_cast<wchar_t*>(original+1)[8]=0;else reinterpret_cast<char*>(original+1)[8]=0;
        void* object=original+1;stringObject=&object;void* position=object;CDatabase db;db.m_hdbc=reinterpret_cast<HDBC>(0xface);
        steps={{SQL_STILL_EXECUTING,0,{}},{code,2,{'Z',0}}};driverFailure=failAt;
        int caught=invoke([&]{if(wide)functions.wide(&db,reinterpret_cast<HSTMT>(0xbeef),1,needed,noPosition?nullptr:&position,initial,reinterpret_cast<CString*>(&object),sqlType,SQL_C_WCHAR);
            else functions.narrow(&db,reinterpret_cast<HSTMT>(0xbeef),1,needed,noPosition?nullptr:&position,initial,&object,sqlType,SQL_C_CHAR);});
        auto* output=reinterpret_cast<CStringData*>(object)-1;
        event(89,output->nDataLength,output->nAllocLength,output->nRefs,INT64(UINT_PTR(position)-UINT_PTR(object)));
        event(90,manager.allocations,manager.reallocations,manager.clones,manager.frees,other.allocations,other.reallocations);
        auto* bytes=static_cast<BYTE*>(object);for(int i=0;i<std::min(32,(output->nAllocLength+1)*size);++i)event(91,i,bytes[i]);
        save(result,caught);
        if(output->Release())output->pStringMgr->Free(output);
        if(shared && !locked)manager.Free(original);
        db.m_hdbc=nullptr;stringObject=nullptr;
    };
    for(bool wide:{false,true})for(short sqlType:{short(SQL_CHAR),short(SQL_WCHAR),short(SQL_VARCHAR),short(SQL_WVARCHAR),short(SQL_LONGVARCHAR),short(SQL_WLONGVARCHAR),short(SQL_LONGVARBINARY)})
    for(SQLLEN needed:{SQLLEN(0),SQLLEN(3),SQLLEN(4),SQLLEN(8)})for(SQLLEN initial:{SQLLEN(1),SQLLEN(4)})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA)})run(wide,sqlType,needed,initial,code);
    for(bool wide:{false,true})for(bool locked:{false,true})for(bool shared:{false,true})for(bool cloneElsewhere:{false,true})
        run(wide,SQL_VARCHAR,8,4,SQL_SUCCESS,locked,shared,cloneElsewhere);
    for(bool wide:{false,true})for(SQLLEN needed:{SQLLEN(-1),SQLLEN(-2),SQLLEN(1ULL<<40),SQLLEN(INT_MAX)})run(wide,SQL_VARCHAR,needed,4,SQL_SUCCESS);
    for(bool wide:{false,true})for(bool missing:{false,true})for(int failure:{0,1,2})run(wide,SQL_VARCHAR,8,4,SQL_SUCCESS,false,false,false,missing,failure);
    return result;
}
static std::vector<Event> binary(const ValueAPI& functions) {
    std::vector<Event> result;
    for(SQLLEN needed:{SQLLEN(0),SQLLEN(4),SQLLEN(8),SQLLEN(32)})for(SQLLEN initial:{SQLLEN(4),SQLLEN(8)})for(short sqlType:{short(SQL_BINARY),short(SQL_LONGVARBINARY)})
    for(RETCODE code:{RETCODE(SQL_SUCCESS),RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA)})for(bool failed:{false,true}) {
        reset();CDBVariant value;value.m_dwType=DBVT_BINARY;value.m_pbinary=new CLongBinary;auto* object=value.m_pbinary;activeBinary=object;
        object->m_hData=::GlobalAlloc(GMEM_MOVEABLE,16);object->m_dwDataLength=16;void* position=::GlobalLock(object->m_hData);memset(position,0x5a,16);
        CDatabase db;db.m_hdbc=reinterpret_cast<HDBC>(0xface);failReallocate=failed;captureGlobals=true;steps={{SQL_STILL_EXECUTING,0,{}},{code,2,{'Z',0}}};
        int caught=invoke([&]{functions.binary(&db,reinterpret_cast<HSTMT>(0xbeef),2,needed,&position,initial,&value,sqlType);});captureGlobals=false;
        event(92,object->m_dwDataLength,::GlobalFlags(object->m_hData)&GMEM_LOCKCOUNT,object->m_hData!=nullptr);
        auto* start=static_cast<BYTE*>(::GlobalLock(object->m_hData));event(93,INT64(UINT_PTR(position)-UINT_PTR(start)));
        for(int i=0;i<std::min<int>(4,int(::GlobalSize(object->m_hData)));++i)event(91,i,start[i]);::GlobalUnlock(object->m_hData);
        save(result,caught);while(::GlobalUnlock(object->m_hData)){}db.m_hdbc=nullptr;activeBinary=nullptr;
    }
    return result;
}
static void stringState(int kind,void* object,int width,bool content=true) {
    auto* bytes=*static_cast<BYTE**>(object);auto* header=reinterpret_cast<CStringData*>(bytes)-1;
    event(kind,header->nDataLength,header->nAllocLength,header->nRefs>1?2:header->nRefs);
    if(content)for(int i=0;i<std::min(64,header->nDataLength*width);++i)event(91,i,bytes[i]);
}
static std::vector<Event> metadata(const FieldAPI& functions) {
    std::vector<Event> result;
    for(short columns:{short(-1),short(0),short(1),short(3)})for(const wchar_t* name:{L"VALUE",L"other",L"missing",L""})for(bool returns:{false,true}) {
        Fixture f;reset();CODBCFieldInfo info[5];f.rs.m_rgODBCFieldInfos=info+1;f.rs.m_nResultCols=columns;
        info[0].m_strName=L"before";info[1].m_strName=L"value";info[2].m_strName=L"other";info[3].m_strName=L"VALUE";info[4].m_strName=L"after";
        for(int i=0;i<5;++i){info[i].m_nSQLType=short(SQL_INTEGER+i);info[i].m_nPrecision=(1ULL<<40)+i;info[i].m_nScale=short(i);info[i].m_nNullability=short(7+i);}
        returningThrow=returns;short index=-7;int caught=invoke([&]{index=functions.index(&f.rs,name);});event(94,index);save(result,caught);
        for(short field:{short(-1),short(0),short(1)}) {
            reset();returningThrow=returns;CODBCFieldInfo output;memset(reinterpret_cast<BYTE*>(&output)+8,0xa5,24);output.m_strName=L"old";
            caught=invoke([&]{functions.info(&f.rs,field,&output);});stringState(95,&output.m_strName,2);
            for(int i=8;i<32;++i)event(91,i,reinterpret_cast<BYTE*>(&output)[i]);save(result,caught);
        }
        reset();returningThrow=returns;CODBCFieldInfo output;memset(reinterpret_cast<BYTE*>(&output)+8,0xa5,24);output.m_strName=L"old";
        caught=invoke([&]{functions.namedInfo(&f.rs,name,&output);});stringState(95,&output.m_strName,2);
        for(int i=8;i<32;++i)event(91,i,reinterpret_cast<BYTE*>(&output)[i]);save(result,caught);
        f.rs.m_rgODBCFieldInfos=nullptr;
    }
    return result;
}
static std::vector<Event> fields(const FieldAPI& functions) {
    std::vector<Event> result;
    auto run=[&](int kind,bool named,short sqlType,short cType,SQLLEN reported,short columns=1,short index=0,RETCODE code=SQL_SUCCESS,
                 bool returns=false,bool missingStatement=false,bool missingName=false,bool oldOwned=false,bool locked=false,bool shared=false,SQLULEN precision=7,int failAt=0) {
        Fixture f;reset();Manager manager;CODBCFieldInfo info[3];for(auto& field:info){field.m_strName=L"value";field.m_nSQLType=sqlType;field.m_nPrecision=precision;field.m_nScale=0;field.m_nNullability=1;}
        f.rs.m_rgODBCFieldInfos=info+1;f.rs.m_nResultCols=columns;f.rs.m_lOpen=1;if(missingStatement)f.rs.m_hstmt=nullptr;
        const wchar_t* name=missingName?nullptr:index==1?L"missing":L"value";
        CDBVariant value;value.m_dwType=DBVT_LONG;UINT64 bits=0xa5a5a5a5a5a5a5a5ULL;memcpy(&value.m_lVal,&bits,8);
        auto* old=manager.Allocate(128,kind==2?1:2);old->nDataLength=3;old->nRefs=locked?-1:shared?2:1;
        if(kind==2)memcpy(old+1,"old",4);else memcpy(old+1,L"old",8);
        void* object=old+1;
        if(oldOwned){auto** string=static_cast<void**>(::operator new(sizeof(void*)));*string=object;value.m_dwType=DBVT_WSTRING;value.m_pstring=reinterpret_cast<CString*>(string);}
        stringObject=kind?&object:nullptr;activeVariant=kind?nullptr:&value;terminatedPayloads=true;driverFailure=failAt;returningThrow=returns;
        short actualType=cType?cType:CRecordset::GetDefaultFieldType(sqlType);
        bool wide=kind==1 || (!kind && actualType==SQL_C_WCHAR);bool binaryType=!kind && actualType==SQL_C_BINARY;
        std::vector<BYTE> payload;
        if(kind || actualType==SQL_C_CHAR || actualType==SQL_C_WCHAR) {
            for(char c:std::string("ABCDEF")){payload.push_back(BYTE(c));if(wide)payload.push_back(0);}
        } else for(int i=0;i<16;++i)payload.push_back(BYTE(0x31+i));
        steps={{SQL_STILL_EXECUTING,0,{}},{code,reported,payload},{SQL_STILL_EXECUTING,0,{}},
            {SQL_SUCCESS,binaryType?SQLLEN(5):wide?SQLLEN(12):SQLLEN(2),binaryType?std::vector<BYTE>{0x32,0x33,0x34,0x35,0x36}:wide?payload:std::vector<BYTE>{'Z'}}};
        captureGlobals=true;
        int caught=invoke([&]{
            if(!kind){if(named)functions.namedVariant(&f.rs,name,&value,cType);else functions.variant(&f.rs,index,&value,cType);}
            else if(kind==1){if(named)functions.namedWide(&f.rs,name,reinterpret_cast<CString*>(&object));else functions.wide(&f.rs,index,reinterpret_cast<CString*>(&object));}
            else {if(named)functions.namedNarrow(&f.rs,name,&object);else functions.narrow(&f.rs,index,&object);}
        });captureGlobals=false;
        if(kind)stringState(96,&object,kind==1?2:1);
        else {
            event(97,value.m_dwType);
            if(value.m_dwType==DBVT_ASTRING || value.m_dwType==DBVT_WSTRING)stringState(96,value.m_pstring,value.m_dwType==DBVT_WSTRING?2:1,!caught);
            else if(value.m_dwType==DBVT_BINARY) {
                auto* binary=value.m_pbinary;event(98,binary->m_dwDataLength,::GlobalFlags(binary->m_hData)&GMEM_LOCKCOUNT);
                if(!caught){auto* data=static_cast<BYTE*>(::GlobalLock(binary->m_hData));for(int i=0;i<std::min<SQLULEN>(16,binary->m_dwDataLength);++i)event(91,i,data[i]);::GlobalUnlock(binary->m_hData);}
                while(::GlobalUnlock(binary->m_hData)){}
            } else if(value.m_dwType==DBVT_DATE) {
                if(!caught)for(int i=0;i<16;++i)event(91,i,reinterpret_cast<BYTE*>(value.m_pdate)[i]);
            } else if(!oldOwned){memcpy(&bits,&value.m_lVal,8);event(99,ownedVariantData?INT64(bits!=0):INT64(bits));}
        }
        event(100,manager.frees,manager.allocations,manager.reallocations,manager.clones);save(result,caught);
        if(!oldOwned) {
            auto* output=reinterpret_cast<CStringData*>(object)-1;if(output->Release())output->pStringMgr->Free(output);
            if(shared && !locked)manager.Free(old);
        }
        f.rs.m_rgODBCFieldInfos=nullptr;activeVariant=nullptr;activeBinary=nullptr;stringObject=nullptr;
    };
    for(int kind:{0,1,2})for(bool named:{false,true})for(short sqlType:{short(SQL_INTEGER),short(SQL_VARCHAR),short(SQL_WVARCHAR),short(SQL_LONGVARCHAR),
        short(SQL_WLONGVARCHAR),short(SQL_VARBINARY),short(SQL_LONGVARBINARY),short(SQL_TIMESTAMP)}) {
        SQLLEN normal=sqlType==SQL_TIMESTAMP?16:kind==1 || (!kind && CRecordset::GetDefaultFieldType(sqlType)==SQL_C_WCHAR)?12:6;
        for(SQLLEN reported:{SQLLEN(SQL_NULL_DATA),normal})run(kind,named,sqlType,0,reported);
    }
    for(short type:{short(SQL_C_BIT),short(SQL_C_UTINYINT),short(SQL_C_SSHORT),short(SQL_C_SLONG),short(SQL_C_FLOAT),short(SQL_C_DOUBLE),short(SQL_C_TIMESTAMP),short(SQL_C_CHAR),short(SQL_C_WCHAR),short(SQL_C_BINARY),short(4)})
        run(0,false,SQL_VARCHAR,type,type==SQL_C_TIMESTAMP?16:type==SQL_C_DOUBLE?8:4);
    for(int kind:{0,1,2})for(bool named:{false,true})for(short columns:{short(-1),short(0),short(1)})for(short index:{short(-1),short(0),short(1)})
        run(kind,named,SQL_INTEGER,0,4,columns,index);
    for(int kind:{0,1,2})for(bool named:{false,true})for(RETCODE code:{RETCODE(SQL_SUCCESS_WITH_INFO),RETCODE(SQL_ERROR),RETCODE(SQL_NO_DATA)})for(int failAt:{0,1,2})
        run(kind,named,SQL_VARCHAR,0,6,1,0,code,false,false,false,false,false,false,7,failAt);
    for(int kind:{0,1,2})for(bool named:{false,true})for(bool missingStatement:{false,true})
        run(kind,named,SQL_VARCHAR,0,6,1,0,SQL_SUCCESS,false,missingStatement);
    for(int kind:{0,1,2})for(bool named:{false,true})for(bool owned:{false,true})for(short columns:{short(0),short(1)})
        run(kind,named,SQL_INTEGER,0,4,columns,0,SQL_SUCCESS,false,false,false,owned && !kind);
    for(int kind:{1,2})for(bool locked:{false,true})for(bool shared:{false,true})run(kind,false,SQL_VARCHAR,0,6,1,0,SQL_SUCCESS,false,false,false,false,locked,shared);
    for(int kind:{0,1,2})run(kind,false,SQL_VARCHAR,0,6,1,0,SQL_SUCCESS,false,false,false,false,false,false,1ULL<<40);
    for(int kind:{0,1,2})for(bool returns:{false,true})run(kind,false,SQL_INTEGER,0,4,1,-1,SQL_SUCCESS,returns);
    // Wide by-name validates a null name before looking at the column count.
    for(short columns:{short(-1),short(0),short(1)})run(1,true,SQL_INTEGER,0,4,columns,0,SQL_SUCCESS,false,false,true);
    for(int kind:{0,1,2})run(kind,true,SQL_INTEGER,0,4,0,0,SQL_SUCCESS,false,false,true);
    return result;
}
static std::vector<Event> nullability(const FieldAPI& functions) {
    std::vector<Event> result;
    for(short columns:{short(-1),short(0),short(1),short(2)})for(UINT declared:{0u,7u})for(short nullable:{short(-7),short(0),short(1),short(2),short(69)})
    for(DWORD index:{0ul,1ul,2ul,0x7ffffffful,0x80000000ul,0xfffffffful}) {
        Fixture f;reset();CODBCFieldInfo metadata[2];metadata[0].m_nNullability=nullable;metadata[1].m_nNullability=nullable;
        f.rs.m_rgODBCFieldInfos=metadata;f.rs.m_nResultCols=columns;f.rs.m_nFields=declared;BOOL returned=-7;
        int caught=invoke([&]{returned=functions.nullable(&f.rs,index);});event(103,returned);save(result,caught);f.rs.m_rgODBCFieldInfos=nullptr;
    }
    for(short columns:{short(0),short(2)})for(UINT_PTR value:{UINT_PTR(0),UINT_PTR(1),UINT_PTR(2),UINT_PTR(3),UINT_PTR(0xffffffffu),UINT_PTR(0x80000000u),UINT_PTR(0x100000001ULL)})
    for(bool found:{false,true})for(bool null:{false,true})for(bool returns:{false,true}) {
        Fixture f;reset();CODBCFieldInfo metadata[2];metadata[0].m_nNullability=SQL_NULLABLE;metadata[1].m_nNullability=SQL_NO_NULLS;
        f.rs.m_rgODBCFieldInfos=metadata;f.rs.m_nResultCols=columns;int field=0;
        if(found)f.rs.m_mapFieldIndex.SetAt(&field,reinterpret_cast<void*>(value));returningThrow=returns;BOOL returned=-7;
        int caught=invoke([&]{returned=functions.boundNullable(&f.rs,null?nullptr:&field);});event(103,returned);save(result,caught);f.rs.m_rgODBCFieldInfos=nullptr;
    }
    return result;
}
static void capabilityGetters() {
    // afxdb.inl in the shipping 14.51 SDK supplies these release contracts.
    Fixture f;f.rs.m_hstmt=nullptr;f.rs.m_lOpen=0;f.rs.m_pDatabase=nullptr;f.rs.m_dwOptions=CRecordset::readOnly;f.rs.m_bDeleted=TRUE;
    f.rs.m_bAppendable=69;f.rs.m_bUpdatable=71;f.rs.m_bScrollable=73;
    check(f.rs.CanAppend()==69 && f.rs.CanUpdate()==71 && f.rs.CanScroll()==73 && f.rs.CanRestart()==TRUE && events.empty(),
        "SDK release capability getters preserve full BOOL flags independently of open state, deleted rows and read-only options");
    f.rs.m_dwOptions|=CRecordset::executeDirect;
    check(f.rs.CanRestart()==FALSE,"SDK CanRestart rejects direct-execution recordsets using their stored option flag");
    f.rs.m_pDatabase=&f.db;f.db.m_hdbc=nullptr;f.db.m_bTransactions=69;
    check(f.rs.CanTransact()==69,"SDK CanTransact returns the database's full transaction capability flag");
    f.rs.m_pDatabase=nullptr;
    check(invoke([&]{f.rs.CanTransact();})==1,"SDK CanTransact raises the framework user exception for a missing database");
}
static void termination(bool native) {
    wchar_t path[2048];::GetModuleFileNameW(nullptr,path,2048);::SetEnvironmentVariableW(L"OPENMFC_FIELD_NAME_TERMINATION_CHILD",native?L"1":L"0");
    std::wstring command=L"\""+std::wstring(path)+L"\"";std::vector<wchar_t> input(command.begin(),command.end());input.push_back(0);
    STARTUPINFOW startup{};startup.cb=sizeof(startup);PROCESS_INFORMATION child{};
    BOOL started=::CreateProcessW(path,input.data(),nullptr,nullptr,FALSE,CREATE_NO_WINDOW,nullptr,nullptr,&startup,&child);
    ::SetEnvironmentVariableW(L"OPENMFC_FIELD_NAME_TERMINATION_CHILD",nullptr);check(started!=FALSE,"start an isolated null field-name comparison");
    if(started){DWORD waited=::WaitForSingleObject(child.hProcess,10000);if(waited!=WAIT_OBJECT_0)::TerminateProcess(child.hProcess,96);
        DWORD code=0;::GetExitCodeProcess(child.hProcess,&code);::CloseHandle(child.hThread);::CloseHandle(child.hProcess);
        if(waited!=WAIT_OBJECT_0 || code!=3)std::printf("null-name child native=%d wait=%lu code=%lu\n",int(native),waited,code);
        check(waited==WAIT_OBJECT_0 && code==3,"a null lookup name terminates at native CString's no-throw comparison boundary");}
}
}
static bool recordsetFieldNameTerminationChild() {
    wchar_t mode[8];if(!::GetEnvironmentVariableW(L"OPENMFC_FIELD_NAME_TERMINATION_CHILD",mode,8))return false;
    ::SetErrorMode(SEM_NOGPFAULTERRORBOX|SEM_FAILCRITICALERRORS);std::set_terminate([]{::ExitProcess(3);});
    auto query=&impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z;
    if(_wtoi(mode)) {wchar_t path[2048];if(!::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_VALUES_DLL",path,2048))::ExitProcess(95);
        auto module=::LoadLibraryW(path);if(!module)::ExitProcess(94);query=reinterpret_cast<decltype(query)>(reinterpret_cast<char*>(module)+0x1fc8c0);}
    CRecordset rs;CODBCFieldInfo info;info.m_strName=L"value";rs.m_nResultCols=1;rs.m_rgODBCFieldInfos=&info;
    try{query(&rs,nullptr);}catch(...){::ExitProcess(97);}::ExitProcess(98);
}
static void recordsetValues() {
    using namespace recordset_value_test;
    auto** allocation=import(::GetModuleHandleW(nullptr),"GlobalAlloc");auto** reallocation=import(::GetModuleHandleW(nullptr),"GlobalReAlloc");
    check(allocation && reallocation,"find local GlobalAlloc/GlobalReAlloc imports for bounded field-buffer failure tests");if(!allocation || !reallocation)return;
    realAllocate=reinterpret_cast<GlobalAllocate>(*allocation);realReallocate=reinterpret_cast<GlobalReallocate>(*reallocation);
    Patch localAllocation(allocation,reinterpret_cast<void*>(&allocate)),localReallocation(reallocation,reinterpret_cast<void*>(&reallocate));
    auto oldData=longbinary_rfx_test::dataHook;longbinary_rfx_test::dataHook=&data;
    ValueAPI local{&impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z,
        &impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z,
        &impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z,
        &impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z};
    FieldAPI localFields{&impl__GetFieldIndexByName_CRecordset__QEAAFPEB_W_Z,&impl__GetODBCFieldInfo_CRecordset__QEAAXFAEAUCODBCFieldInfo___Z,&impl__GetODBCFieldInfo_CRecordset__QEAAXPEB_WAEAUCODBCFieldInfo___Z,
        &impl__GetFieldValue_CRecordset__QEAAXFAEAVCDBVariant__F_Z,&impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAVCDBVariant__F_Z,
        &impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z,&impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z,
        &impl__GetFieldValue_CRecordset__QEAAXFAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z,&impl__GetFieldValue_CRecordset__QEAAXPEB_WAEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL___Z,
        &impl__IsFieldNullable_CRecordset__QEBAHK_Z,&impl__IsFieldNullable_CRecordset__QEAAHPEAX_Z};
    capabilityGetters();auto allocated=buffers(local);auto strings=text(local);auto blobs=binary(local);auto information=metadata(localFields);auto values=fields(localFields);auto nullable=nullability(localFields);termination(false);
    wchar_t path[1024];if(::GetEnvironmentVariableW(L"OPENMFC_NATIVE_RECORDSET_VALUES_DLL",path,1024)) {
        auto module=::LoadLibraryW(path);check(module!=nullptr,"load native MFC for field-buffer comparisons");
        if(module) {
            auto* base=reinterpret_cast<char*>(module);auto* dos=reinterpret_cast<IMAGE_DOS_HEADER*>(base);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(base+dos->e_lfanew);
            bool pinned=pe->FileHeader.TimeDateStamp==0x442cba30 && pe->OptionalHeader.SizeOfImage==0x55c000;check(pinned,"field-buffer oracle uses the verified MFC image build");
            if(pinned) {
                Patch a(reinterpret_cast<void**>(base+0x2c6620),reinterpret_cast<void*>(&allocate)),r(reinterpret_cast<void**>(base+0x2c6720),reinterpret_cast<void*>(&reallocate));
                Patch d(reinterpret_cast<void**>(base+0x3e9120),reinterpret_cast<void*>(&SQLGetData)),e(reinterpret_cast<void**>(base+0x3e90e0),reinterpret_cast<void*>(&SQLErrorW));
                ValueAPI native{reinterpret_cast<decltype(local.buffer)>(base+0x1fd570),reinterpret_cast<decltype(local.wide)>(base+0x1fd970),
                    reinterpret_cast<decltype(local.narrow)>(base+0x1fdb60),reinterpret_cast<decltype(local.binary)>(base+0x1fdd50)};
                FieldAPI nativeFields{reinterpret_cast<decltype(localFields.index)>(base+0x1fc8c0),reinterpret_cast<decltype(localFields.info)>(base+0x1f9290),reinterpret_cast<decltype(localFields.namedInfo)>(base+0x1f9250),
                    reinterpret_cast<decltype(localFields.variant)>(base+0x1f9350),reinterpret_cast<decltype(localFields.namedVariant)>(base+0x1f92f0),
                    reinterpret_cast<decltype(localFields.wide)>(base+0x1f9600),reinterpret_cast<decltype(localFields.namedWide)>(base+0x1f9560),
                    reinterpret_cast<decltype(localFields.narrow)>(base+0x1f9780),reinterpret_cast<decltype(localFields.namedNarrow)>(base+0x1f95c0),
                    reinterpret_cast<decltype(localFields.nullable)>(base+0x1fcbd0),reinterpret_cast<decltype(localFields.boundNullable)>(base+0x1f8970)};
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(buffers(native),allocated,"variant data-buffer types, sizes, inline bits, ownership and full-width allocation failures match native MFC");
                compare(text(native),strings,"ANSI/Unicode continuation offsets, manager ownership, release lengths, asynchronous fetches and failures match native MFC");
                compare(binary(native),blobs,"long binary reallocation, retained handles, fetch offsets, lengths and interrupted lock state match native MFC");
                compare(metadata(nativeFields),information,"field-name lookup, metadata copying, signed column counts, ownership and padding match native MFC");
                compare(fields(nativeFields),values,"all six field-value overloads, typed variants, NULLs, continuation reads, name lookup, output ownership and failures match native MFC");
                compare(nullability(nativeFields),nullable,"nullable result columns, bound-field maps, zero-based indices, signed flags and exception paths match native MFC");
                termination(true);
            }
            ::FreeLibrary(module);
        }
    }
    longbinary_rfx_test::dataHook=oldData;captureGlobals=false;longbinary_rfx_test::reset({});
}
