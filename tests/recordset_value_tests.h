#pragma once
extern "C" void* MS_ABI impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z(CDBVariant*,short,SQLLEN*,short,SQLULEN);
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z(CDatabase*,HSTMT,short,SQLLEN,void**,SQLLEN,CString*,short,short);
extern "C" void MS_ABI impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z(CDatabase*,HSTMT,short,SQLLEN,void**,SQLLEN,void*,short,short);
extern "C" void MS_ABI impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z(CDatabase*,HSTMT,short,SQLLEN,void**,SQLLEN,CDBVariant*,short);
namespace recordset_value_test {
using namespace recordset_move_test;
struct ValueAPI {
    decltype(&impl__GetDataBuffer_CRecordset__SAPEAXAEAVCDBVariant__FPEA_JF_K_Z) buffer;
    decltype(&impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__FF_Z) wide;
    decltype(&impl__GetLongCharDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAV__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__FF_Z) narrow;
    decltype(&impl__GetLongBinaryDataAndCleanup_CRecordset__SAXPEAVCDatabase__PEAXF_JPEAPEAX2AEAVCDBVariant__F_Z) binary;
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
static int driverFailure;
static SQLRETURN SQL_API data(SQLHSTMT statement,SQLUSMALLINT field,SQLSMALLINT type,SQLPOINTER output,SQLLEN capacity,SQLLEN* indicator) {
    void* base=stringObject?*static_cast<void**>(stringObject):nullptr;
    if(activeBinary){base=::GlobalLock(activeBinary->m_hData);::GlobalUnlock(activeBinary->m_hData);}
    event(82,INT64(UINT_PTR(statement)),field,type,capacity,INT64(UINT_PTR(output)-UINT_PTR(base)));
    if(nextStep>=steps.size())std::abort();auto step=steps[nextStep++];
    if(driverFailure==1)throw 77;
    if(driverFailure==2)throw static_cast<CException*>(new CDBException(77));
    *indicator=step.length;
    if(output && capacity>0 && !step.bytes.empty())memcpy(output,step.bytes.data(),std::min<size_t>(size_t(capacity),step.bytes.size()));
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
    events.clear();captureGlobals=false;failAllocate=failReallocate=false;driverFailure=0;stringObject=nullptr;activeBinary=nullptr;nextStep=0;
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
    auto allocated=buffers(local);auto strings=text(local);auto blobs=binary(local);
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
                auto compare=[&](const std::vector<Event>& actual,const std::vector<Event>& expected,const char* label){
                    if(actual!=expected)for(size_t i=0;i<std::min(actual.size(),expected.size());++i)if(actual[i]!=expected[i]){std::printf("%s mismatch %zu local:",label,i);for(auto n:expected[i])std::printf(" %lld",n);std::printf(" native:");for(auto n:actual[i])std::printf(" %lld",n);puts("");break;}check(actual==expected,label);};
                compare(buffers(native),allocated,"variant data-buffer types, sizes, inline bits, ownership and full-width allocation failures match native MFC");
                compare(text(native),strings,"ANSI/Unicode continuation offsets, manager ownership, release lengths, asynchronous fetches and failures match native MFC");
                compare(binary(native),blobs,"long binary reallocation, retained handles, fetch offsets, lengths and interrupted lock state match native MFC");
            }
            ::FreeLibrary(module);
        }
    }
    longbinary_rfx_test::dataHook=oldData;captureGlobals=false;longbinary_rfx_test::reset({});
}
