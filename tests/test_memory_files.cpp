// Real file-family owners; exception factories and diagnostics are boundaries.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "detail/CSharedFileSupport.h"
#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cerrno>
#include <string>
#include <ctime>

// These surrounding services are outside the buffer and Win32 file tests.
CRuntimeClass CException::classCException{"CException",16,0xffff,nullptr,&CObject::GetThisClass,nullptr,nullptr};
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const { std::abort(); }
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) { std::abort(); }
int CException::ReportError(UINT,UINT) { std::abort(); }
int CFileException::GetErrorMessage(wchar_t*,UINT,UINT*) const { std::abort(); }
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT) { std::abort(); }
struct BoundaryError { int kind; long os; int cause=0; std::wstring path; bool nullPath=false; };
extern "C" void impl__AfxThrowOleException__YAXJ_Z(long code) { throw BoundaryError{6,code}; }
void AFXAPI AfxThrowOleException(long code) { throw BoundaryError{6,code}; }
void AFXAPI AfxThrowMemoryException() { throw BoundaryError{1,0}; }
void AFXAPI AfxThrowInvalidArgException() { throw BoundaryError{2,0}; }
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ() { throw BoundaryError{3,0}; }
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause,long os,const wchar_t* name) { throw BoundaryError{cause==9?4:5,os,cause,name?name:L"",!name}; }

#define FILE_API(X) \
 X(ctor,impl___0CMemFile__QEAA_I_Z,"??0CMemFile@@QEAA@I@Z",0x22ae70,CMemFile*,(void*,UINT)) \
 X(bufferCtor,impl___0CMemFile__QEAA_PEAEII_Z,"??0CMemFile@@QEAA@PEAEII@Z",0x22aec0,CMemFile*,(void*,BYTE*,UINT,UINT)) \
 X(dtor,impl___1CMemFile__UEAA_XZ,"??1CMemFile@@UEAA@XZ",0x22afa0,void,(CMemFile*)) \
 X(read,impl__Read_CMemFile__UEAAIPEAXI_Z,"?Read@CMemFile@@UEAAIPEAXI@Z",0x22b170,UINT,(CMemFile*,void*,UINT)) \
 X(write,impl__Write_CMemFile__UEAAXPEBXI_Z,"?Write@CMemFile@@UEAAXPEBXI@Z",0x22b1f0,void,(CMemFile*,const void*,UINT)) \
 X(seek,impl__Seek_CMemFile__UEAA_K_JI_Z,"?Seek@CMemFile@@UEAA_K_JI@Z",0x22b290,ULONGLONG,(CMemFile*,LONGLONG,UINT)) \
 X(length,impl__GetLength_CMemFile__UEBA_KXZ,"?GetLength@CMemFile@@UEBA_KXZ",0x22b110,ULONGLONG,(const CMemFile*)) \
 X(position,impl__GetPosition_CMemFile__UEBA_KXZ,"?GetPosition@CMemFile@@UEBA_KXZ",0x22b080,ULONGLONG,(const CMemFile*)) \
 X(setLength,impl__SetLength_CMemFile__UEAAX_K_Z,"?SetLength@CMemFile@@UEAAX_K@Z",0x22b120,void,(CMemFile*,ULONGLONG)) \
 X(grow,impl__GrowFile_CMemFile__MEAAX_K_Z,"?GrowFile@CMemFile@@MEAAX_K@Z",0x22b090,void,(CMemFile*,SIZE_T)) \
 X(copy,impl__Memcpy_CMemFile__MEAAPEAEPEAEPEBE_K_Z,"?Memcpy@CMemFile@@MEAAPEAEPEAEPEBE_K@Z",0x22b010,BYTE*,(CMemFile*,BYTE*,const BYTE*,SIZE_T)) \
 X(buffer,impl__GetBufferPtr_CMemFile__UEAAIIIPEAPEAX0_Z,"?GetBufferPtr@CMemFile@@UEAAIIIPEAPEAX0@Z",0x22b380,UINT,(CMemFile*,UINT,UINT,void**,void**)) \
 X(close,impl__Close_CMemFile__UEAAXXZ,"?Close@CMemFile@@UEAAXXZ",0x22b310,void,(CMemFile*)) \
 X(abort,impl__Abort_CMemFile__UEAAXXZ,"?Abort@CMemFile@@UEAAXXZ",0x129f10,void,(CMemFile*)) \
 X(attach,impl__Attach_CMemFile__QEAAXPEAEII_Z,"?Attach@CMemFile@@QEAAXPEAEII@Z",0x22af30,void,(CMemFile*,BYTE*,UINT,UINT)) \
 X(detach,impl__Detach_CMemFile__QEAAPEAEXZ,"?Detach@CMemFile@@QEAAPEAEXZ",0x22af80,BYTE*,(CMemFile*)) \
 X(status,impl__GetStatus_CMemFile__QEBAHAEAUCFileStatus___Z,"?GetStatus@CMemFile@@QEBAHAEAUCFileStatus@@@Z",0x22c080,int,(const CMemFile*,CFileStatus&)) \
 X(duplicate,impl__Duplicate_CMemFile__UEBAPEAVCFile__XZ,"?Duplicate@CMemFile@@UEBAPEAVCFile@@XZ",0x22b370,CFile*,(const CMemFile*)) \
 X(lock,impl__LockRange_CMemFile__UEAAX_K0_Z,"?LockRange@CMemFile@@UEAAX_K0@Z",0x22b370,void,(CMemFile*,ULONGLONG,ULONGLONG)) \
 X(unlock,impl__UnlockRange_CMemFile__UEAAX_K0_Z,"?UnlockRange@CMemFile@@UEAAX_K0@Z",0x22b370,void,(CMemFile*,ULONGLONG,ULONGLONG)) \
 X(sharedCtor,impl___0CSharedFile__QEAA_II_Z,"??0CSharedFile@@QEAA@II@Z",0x22b480,CSharedFile*,(void*,UINT,UINT)) \
 X(sharedDtor,impl___1CSharedFile__UEAA_XZ,"??1CSharedFile@@UEAA@XZ",0x22b4d0,void,(CSharedFile*)) \
 X(sharedDetach,impl__Detach_CSharedFile__QEAAPEAXXZ,"?Detach@CSharedFile@@QEAAPEAXXZ",0x22b610,void*,(CSharedFile*)) \
 X(setHandle,impl__SetHandle_CSharedFile__QEAAXPEAXH_Z,"?SetHandle@CSharedFile@@QEAAXPEAXH@Z",0x22b500,void,(CSharedFile*,void*,BOOL)) \
 X(fileCtor,impl___0CFile__QEAA_XZ,"??0CFile@@QEAA@XZ",0x2277e0,CFile*,(void*)) \
 X(handleCtor,impl___0CFile__QEAA_PEAX_Z,"??0CFile@@QEAA@PEAX@Z",0x2278a0,CFile*,(void*,HANDLE)) \
 X(fileDtor,impl___1CFile__UEAA_XZ,"??1CFile@@UEAA@XZ",0x227ac0,void,(CFile*)) \
 X(fileOpen,impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z,"?Open@CFile@@UEAAHPEB_WIPEAVCFileException@@@Z",0x227c50,BOOL,(CFile*,const wchar_t*,UINT,CFileException*)) \
 X(fileWrite,impl__Write_CFile__UEAAXPEBXI_Z,"?Write@CFile@@UEAAXPEBXI@Z",0x227ef0,void,(CFile*,const void*,UINT)) \
 X(fileRead,impl__Read_CFile__UEAAIPEAXI_Z,"?Read@CFile@@UEAAIPEAXI@Z",0x227e90,UINT,(CFile*,void*,UINT)) \
 X(fileSeek,impl__Seek_CFile__UEAA_K_JI_Z,"?Seek@CFile@@UEAA_K_JI@Z",0x227f70,ULONGLONG,(CFile*,LONGLONG,UINT)) \
 X(fileClose,impl__Close_CFile__UEAAXXZ,"?Close@CFile@@UEAAXXZ",0x228080,void,(CFile*)) \
 X(fileAbort,impl__Abort_CFile__UEAAXXZ,"?Abort@CFile@@UEAAXXZ",0x2280f0,void,(CFile*)) \
 X(fileBuffer,impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z,"?GetBufferPtr@CFile@@UEAAIIIPEAPEAX0@Z",0x71e0,UINT,(CFile*,UINT,UINT,void**,void**)) \
 X(fileStatus,impl__GetStatus_CFile__QEBAHAEAUCFileStatus___Z,"?GetStatus@CFile@@QEBAHAEAUCFileStatus@@@Z",0x22b8d0,int,(const CFile*,CFileStatus&)) \
 X(pathStatus,impl__GetStatus_CFile__SAHPEB_WAEAUCFileStatus__PEAVCAtlTransactionManager_ATL___Z,"?GetStatus@CFile@@SAHPEB_WAEAUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z",0x22bac0,int,(const wchar_t*,CFileStatus&,ATL::CAtlTransactionManager*)) \
 X(setStatus,impl__SetStatus_CFile__SAXPEB_WAEBUCFileStatus__PEAVCAtlTransactionManager_ATL___Z,"?SetStatus@CFile@@SAXPEB_WAEBUCFileStatus@@PEAVCAtlTransactionManager@ATL@@@Z",0x22bde0,void,(const wchar_t*,const CFileStatus&,ATL::CAtlTransactionManager*)) \
 X(rename,impl__Rename_CFile__SAXPEB_W0PEAVCAtlTransactionManager_ATL___Z,"?Rename@CFile@@SAXPEB_W0PEAVCAtlTransactionManager@ATL@@@Z",0x228270,void,(const wchar_t*,const wchar_t*,ATL::CAtlTransactionManager*)) \
 X(remove,impl__Remove_CFile__SAXPEB_WPEAVCAtlTransactionManager_ATL___Z,"?Remove@CFile@@SAXPEB_WPEAVCAtlTransactionManager@ATL@@@Z",0x228320,void,(const wchar_t*,ATL::CAtlTransactionManager*)) \
 X(fileName,impl__GetFileName_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ,"?GetFileName@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ",0x22b660,void*,(const CFile*,void*)) \
 X(fileTitle,impl__GetFileTitle_CFile__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ,"?GetFileTitle@CFile@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ",0x22b760,void*,(const CFile*,void*)) \
 X(nameHelper,impl__AfxGetFileName__YAIPEB_WPEA_WI_Z,"?AfxGetFileName@@YAIPEB_WPEA_WI@Z",0x1ce400,UINT,(const wchar_t*,wchar_t*,UINT)) \
 X(titleHelper,impl__AfxGetFileTitle__YAIPEB_WPEA_WI_Z,"?AfxGetFileTitle@@YAIPEB_WPEA_WI@Z",0x228ea0,UINT,(const wchar_t*,wchar_t*,UINT)) \
 X(timeHelper,impl__AfxTimeToFileTime__YAXAEBVCTime_ATL__PEAU_FILETIME___Z,"?AfxTimeToFileTime@@YAXAEBVCTime@ATL@@PEAU_FILETIME@@@Z",0x22bca0,void,(const LONGLONG*,FILETIME*)) \
 X(fullHelper,impl__AfxFullPath__YAHPEA_WPEB_W_Z,"?AfxFullPath@@YAHPEA_WPEB_W@Z",0x228c80,BOOL,(wchar_t*,const wchar_t*)) \
 X(rootHelper,impl__AfxGetRoot__YAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z,"?AfxGetRoot@@YAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z",0x228c90,void,(const wchar_t*,CString&))
#define DECLARE(field,alias,name,rva,result,args) extern "C" result alias args;
FILE_API(DECLARE)
#undef DECLARE
struct API {
#define MEMBER(field,alias,name,rva,result,args) decltype(&alias) field;
FILE_API(MEMBER)
#undef MEMBER
    HMODULE image;
};
const API* api;
int failures;
void check(bool ok,const char* what) { if(!ok) { ++failures; std::printf("FAIL: %s\n",what); } }
template<class T> T field(const void* p,size_t offset) { T v; memcpy(&v,static_cast<const BYTE*>(p)+offset,sizeof(v)); return v; }
template<class T> void store(void* p,size_t offset,T value) { memcpy(static_cast<BYTE*>(p)+offset,&value,sizeof(value)); }
template<class F> void expect(int kind,F action) {
    bool caught=false;
    try { action(); }
    catch(const BoundaryError& error) { caught=error.kind==kind; }
    catch(CException* error) {
        const char* name=error->GetRuntimeClass()->m_lpszClassName;
        if(kind==1)caught=!strcmp(name,"CMemoryException");
        if(kind==2)caught=!strcmp(name,"CInvalidArgException");
        if(kind==3)caught=!strcmp(name,"CNotSupportedException");
        if(kind==4)caught=!strcmp(name,"CFileException") && static_cast<CFileException*>(error)->m_cause==9;
        error->Delete();
    }
    check(caught,"framework failure category matches native");
}
struct Memory {
    alignas(8) BYTE bytes[112];
    CMemFile* p;
    explicit Memory(UINT grow=8) { memset(bytes,0xa5,sizeof(bytes));p=api->ctor(bytes,grow); }
    ~Memory() { api->dtor(p); }
};
std::vector<int> calls;
std::array<BYTE,256> arena;
bool failAllocation=false,mutateCopy=false,mutateFree=false;
BYTE* callbackAlloc(CMemFile*,SIZE_T size) { calls.push_back(1000+int(size));return failAllocation?nullptr:arena.data(); }
BYTE* callbackRealloc(CMemFile*,BYTE*,SIZE_T size) { calls.push_back(2000+int(size));return failAllocation?nullptr:arena.data(); }
BYTE* callbackCopy(CMemFile* p,BYTE* d,const BYTE* s,SIZE_T size) {
    calls.push_back(3000+int(size));if(size)memcpy(d,s,size);
    if(mutateCopy)store<ULONGLONG>(p,48,11);
    return d;
}
void callbackClose(CMemFile*) { calls.push_back(5000); }
void callbackFree(CMemFile* p,BYTE*) {
    calls.push_back(4000);
    check(field<ULONGLONG>(p,40)==0 && field<ULONGLONG>(p,48)==0 && field<ULONGLONG>(p,56)==0 && field<ULONGLONG>(p,64)==0,"Close clears bookkeeping before virtual Free");
    if(mutateFree){store<ULONGLONG>(p,48,19);store<BYTE*>(p,72,arena.data()+1);}
}
struct Callbacks {
    CMemFile* p;void** original;std::array<void*,29> table;
    explicit Callbacks(CMemFile* file):p(file),original(field<void**>(file,0)) {
        memcpy(table.data(),original,table.size()*sizeof(void*));
        table[24]=reinterpret_cast<void*>(callbackAlloc);table[25]=reinterpret_cast<void*>(callbackRealloc);
        table[26]=reinterpret_cast<void*>(callbackCopy);table[27]=reinterpret_cast<void*>(callbackFree);
        store(p,0,table.data());calls.clear();
    }
    ~Callbacks(){store(p,0,original);}
};
void basicMemory() {
    Memory m;auto p=m.p;
    check(field<HANDLE>(p,8)==INVALID_HANDLE_VALUE && !field<BOOL>(p,16) && !field<void*>(p,32),"memory file has an actual initialized CFile base");
    check(field<SIZE_T>(p,40)==8 && !field<ULONGLONG>(p,48) && !field<SIZE_T>(p,56) && !field<ULONGLONG>(p,64) && !field<BYTE*>(p,72) && field<BOOL>(p,80)==TRUE,"constructor initializes native memory fields");
    check(field<DWORD>(p,20)==0xa5a5a5a5 && field<DWORD>(p,84)==0xa5a5a5a5,"constructors preserve native padding");
    api->write(p,"abc",3);check(api->position(p)==3 && api->length(p)==3 && field<SIZE_T>(p,56)==8,"write grows by the configured quantum");
    check(api->seek(p,0,0)==0,"seek to beginning");char output[8]{};
    check(api->read(p,output,8)==3 && !memcmp(output,"abc",3) && api->position(p)==3,"read stops at logical EOF");
    api->seek(p,12,0);check(api->position(p)==12 && api->length(p)==3 && field<SIZE_T>(p,56)==16,"seek allocates without extending logical length");
    check(api->read(p,output,1)==0,"read past EOF returns zero");
    check(api->seek(p,100,97)==12,"unknown origin preserves position");
    expect(4,[&]{api->seek(p,1,2);});expect(4,[&]{api->seek(p,-1,0);});
    api->setLength(p,2);check(api->position(p)==2 && api->length(p)==2,"truncate clamps position");
    expect(1,[&]{api->setLength(p,0x100000000ull);});
    api->write(p,nullptr,0);check(api->read(p,nullptr,0)==0,"zero-count access does not validate buffers");
    expect(2,[&]{api->read(p,nullptr,1);});expect(2,[&]{api->write(p,nullptr,1);});
    expect(3,[&]{api->duplicate(p);});expect(3,[&]{api->lock(p,0,1);});expect(3,[&]{api->unlock(p,0,1);});
    CFileStatus status;memset(&status,0x5a,sizeof(status));
    check(api->status(p,status)==1 && status.m_size==2 && !status.m_ctime && !status.m_mtime && !status.m_atime && !status.m_attribute && !status.m_szFullName[0] && status.m_szFullName[1]==0x5a5a,"memory status preserves the unused filename tail");
    api->close(p);check(!field<BYTE*>(p,72) && !field<SIZE_T>(p,40) && field<BOOL>(p,80)==TRUE,"Close releases buffer and preserves ownership flag");
}
void buffers() {
    Memory m;auto p=m.p;void *start=reinterpret_cast<void*>(1),*end=reinterpret_cast<void*>(2);
    check(api->buffer(p,3,0,&start,&end)==1 && start==reinterpret_cast<void*>(1) && end==reinterpret_cast<void*>(2),"bufferCheck does not write output pointers");
    check(api->buffer(p,1,5,&start,&end)==5 && static_cast<BYTE*>(end)-static_cast<BYTE*>(start)==5 && api->position(p)==0 && api->length(p)==0,"bufferWrite reserves without advancing");
    memcpy(start,"hello",5);check(api->buffer(p,2,5,nullptr,nullptr)==0 && api->position(p)==5 && api->length(p)==5,"bufferCommit advances and extends");
    api->seek(p,0,0);check(api->buffer(p,0,UINT_MAX,&start,&end)==5 && !memcmp(start,"hello",5) && api->position(p)==5,"bufferRead exposes data and advances to EOF");
    check(api->buffer(p,1,1,nullptr,&end)==0,"buffer access requires both output pointers");
    BYTE attached[16]{};api->close(p);api->attach(p,attached,16,0);
    check(api->buffer(p,3,0,&start,&end)==0 && api->length(p)==16 && !field<BOOL>(p,80),"fixed attached buffers are readable and do not allow direct growth");
    api->seek(p,4,0);auto* detached=api->detach(p);
    check(detached==attached && api->position(p)==0 && api->length(p)==0 && !field<BOOL>(p,80),"Detach clears dimensions without changing ownership");
    api->attach(p,attached,16,8);check(api->length(p)==0 && field<SIZE_T>(p,40)==8,"growable attachment begins empty");
    api->detach(p);expect(2,[&]{api->attach(p,nullptr,1,0);});
    alignas(8) BYTE storage[88];auto* other=api->bufferCtor(storage,attached,16,8);
    check(api->length(other)==0 && field<SIZE_T>(other,56)==16 && !field<BOOL>(other,80),"buffer constructor uses growable-attachment semantics");
    api->dtor(other);attached[0]=42;
    other=api->bufferCtor(storage,attached,16,0);
    check(api->length(other)==16 && attached[0]==42,"fixed buffer constructor exposes the whole borrowed buffer");api->dtor(other);
}
void callbacks() {
    Memory m;auto p=m.p;Callbacks callbacks(p);
    api->write(p,"ab",2);check(calls==std::vector<int>{1008,3002},"Write dispatches through native Alloc and Memcpy slots");
    calls.clear();api->write(p,"cdefghij",8);check(calls==std::vector<int>{2016,3008},"growth uses virtual Realloc");
    calls.clear();mutateCopy=true;api->write(p,"z",1);mutateCopy=false;
    check(api->position(p)==12 && api->length(p)==12,"Write reloads position after the client copy callback");
    api->seek(p,0,0);char out[3];calls.clear();mutateCopy=true;check(api->read(p,out,2)==2,"callback read result");mutateCopy=false;
    check(api->position(p)==13 && calls==std::vector<int>{3002},"Read reloads position after the client copy callback");
    calls.clear();callbacks.table[22]=reinterpret_cast<void*>(callbackClose);api->abort(p);
    check(calls==std::vector<int>{5000} && field<BYTE*>(p,72)==arena.data(),"Abort dispatches to the client Close override");
    calls.clear();mutateFree=true;api->close(p);mutateFree=false;
    check(calls==std::vector<int>{4000} && api->position(p)==19 && !field<BYTE*>(p,72),"Close preserves callback position but clears the final buffer pointer");
    store<SIZE_T>(p,40,8);failAllocation=true;expect(1,[&]{api->grow(p,1);});failAllocation=false;
    check(!field<BYTE*>(p,72) && !field<SIZE_T>(p,56),"failed growth does not commit an allocation");
    store<ULONGLONG>(p,48,0x100000000ull);api->buffer(p,2,7,nullptr,nullptr);
    check(api->position(p)==0x100000007ull && api->length(p)==0x100000007ull,"buffer bookkeeping preserves positions above 4 GiB");
    store<ULONGLONG>(p,48,~0ull);expect(2,[&]{api->write(p,"x",1);});
    store<ULONGLONG>(p,48,0);store<ULONGLONG>(p,64,0);
    check(api->copy(p,nullptr,nullptr,0)==nullptr,"Memcpy accepts zero bytes with null pointers");
    BYTE bytes[]{1,2,3};expect(2,[&]{api->copy(p,bytes,nullptr,3);});check(bytes[0]==0 && bytes[1]==0 && bytes[2]==0 && errno==EINVAL,"Memcpy clears the destination on a null source");
    check(api->copy(p,bytes,reinterpret_cast<const BYTE*>("abc"),3)==bytes && !memcmp(bytes,"abc",3),"Memcpy uses the canonical four-argument ABI");
}
void shared() {
    alignas(8) BYTE bytes[112];memset(bytes,0xa5,sizeof(bytes));auto* p=api->sharedCtor(bytes,GMEM_MOVEABLE|GMEM_ZEROINIT,8);
    check(p && p->GetRuntimeClass()->m_nObjectSize==112 && field<UINT>(p,88)==(GMEM_MOVEABLE|GMEM_ZEROINIT) && field<BOOL>(p,104)==TRUE,"shared constructor installs its real model and runtime class");
    api->write(p,"abc",3);HGLOBAL h=field<HGLOBAL>(p,96);check(h && GlobalSize(h)>=8 && !memcmp(GlobalLock(h),"abc",3),"inherited Write allocates through shared global-memory override");GlobalUnlock(h);
    api->write(p,"defghijkl",9);check(api->length(p)==12 && field<SIZE_T>(p,56)==16,"inherited Write reallocates the shared global block");
    h=api->sharedDetach(p);check(h && !field<BYTE*>(p,72) && !field<SIZE_T>(p,56) && !field<HGLOBAL>(p,96) && api->length(p)==12 && api->position(p)==12,"shared Detach preserves logical size and position");
    api->sharedDtor(p);GlobalFree(h);
    p=api->sharedCtor(bytes,GMEM_MOVEABLE,8);h=GlobalAlloc(GMEM_MOVEABLE,8);store<ULONGLONG>(p,48,7);api->setHandle(p,h,FALSE);
    check(api->length(p)==GlobalSize(h) && field<BOOL>(p,80)==TRUE && field<BOOL>(p,104)==FALSE && api->position(p)==7,"SetHandle preserves position and controls growth separately from ownership");
    expect(1,[&]{api->setLength(p,field<SIZE_T>(p,56)+1);});
    api->sharedDtor(p);check(GlobalFlags(h)==GMEM_INVALID_HANDLE,"shared destructor frees an attached handle even when growth is disabled");
}
void files() {
    alignas(8) BYTE bytes[40];auto* p=api->fileCtor(bytes);void *s=reinterpret_cast<void*>(1),*e=reinterpret_cast<void*>(2);
    check(api->fileBuffer(p,1,1,&s,&e)==0 && s==reinterpret_cast<void*>(1) && e==reinterpret_cast<void*>(2),"CFile buffer method leaves outputs untouched");
    wchar_t folder[MAX_PATH],name[MAX_PATH];GetTempPathW(MAX_PATH,folder);GetTempFileNameW(folder,L"mfc",0,name);
    CFileException error;check(api->fileOpen(p,name,CFile::modeReadWrite|CFile::shareDenyNone,&error)==TRUE && field<BOOL>(p,16)==TRUE,"Open establishes handle ownership");
    api->fileWrite(p,"file",4);check(api->fileSeek(p,0,0)==0,"file seek uses Windows handle");char data[4];check(api->fileRead(p,data,4)==4 && !memcmp(data,"file",4),"file read/write round trip");
    HANDLE h=field<HANDLE>(p,8);api->fileDtor(p);check(GetFileType(h)==FILE_TYPE_UNKNOWN && GetLastError()==ERROR_INVALID_HANDLE,"owned file closes in destructor");
    h=CreateFileW(name,GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,0,nullptr);p=api->handleCtor(bytes,h);api->fileDtor(p);
    check(GetFileType(h)==FILE_TYPE_DISK,"HANDLE constructor borrows its handle");CloseHandle(h);
    p=api->fileCtor(bytes);store<BOOL>(p,16,7);api->fileAbort(p);check(field<BOOL>(p,16)==7,"Abort preserves close-on-delete state");api->fileDtor(p);DeleteFileW(name);
}

// Replace only imported OS/CRT services. All tested file owners, status records,
// transactions' inline ABI operations and exported helpers are production code.
void** imported(HMODULE image,const char* wanted){
    auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
    auto* d=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
    for(;d->Name;++d){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+d->FirstThunk);
        for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){
            auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);
            if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);
        }
    }return nullptr;
}
struct Patch {
    void** slot;void* old;DWORD protection;
    Patch(const char* name,void* fn){slot=imported(api->image?api->image:GetModuleHandleW(nullptr),name);check(slot!=nullptr,name);if(!slot)abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=fn;}
    ~Patch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}
};
std::vector<int> pathEvents;
int pathMode,failedLocal,failedSystem,systemCalls;DWORD attributeValue;BOOL attributeSuccess,timeSuccess,sizeSuccess;
FILETIME created,accessed,written;
FILETIME stamp(WORD year){SYSTEMTIME system{};system.wYear=year;system.wMonth=1;system.wDay=2;FILETIME result{};check(SystemTimeToFileTime(&system,&result)!=FALSE,"construct test FILETIME");return result;}
bool equalTime(const FILETIME& a,const FILETIME& b){return a.dwLowDateTime==b.dwLowDateTime&&a.dwHighDateTime==b.dwHighDateTime;}
DWORD WINAPI fullPath(const wchar_t*,DWORD size,wchar_t* out,wchar_t** leaf){pathEvents.push_back(1);check(size==260,"status uses MAX_PATH full-path capacity");wcscpy_s(out,size,L"C:\\parent\\example.ext");if(leaf)*leaf=out+10;return pathMode==1?0:pathMode==2?260:22;}
BOOL WINAPI volume(const wchar_t* root,wchar_t*,DWORD,DWORD*,DWORD* component,DWORD* flags,wchar_t*,DWORD){pathEvents.push_back(2);check(!wcscmp(root,L"C:\\"),"full path queries the extracted volume root");*component=255;*flags=FILE_CASE_PRESERVED_NAMES|FILE_UNICODE_ON_DISK;return pathMode!=3;}
BOOL WINAPI attributesEx(const wchar_t*,GET_FILEEX_INFO_LEVELS level,void* ptr){pathEvents.push_back(3);check(level==GetFileExInfoStandard,"status uses standard attributes query");auto* out=static_cast<WIN32_FILE_ATTRIBUTE_DATA*>(ptr);out->dwFileAttributes=attributeValue;out->nFileSizeHigh=7;out->nFileSizeLow=9;out->ftCreationTime=created;out->ftLastAccessTime=accessed;out->ftLastWriteTime=written;return attributeSuccess;}
DWORD WINAPI attributes(const wchar_t*){pathEvents.push_back(4);return attributeValue;}
BOOL WINAPI getTimes(HANDLE handle,FILETIME* c,FILETIME* a,FILETIME* w){pathEvents.push_back(5);check(handle==reinterpret_cast<HANDLE>(42),"instance status uses its actual handle");*c=created;*a=accessed;*w=written;return timeSuccess;}
BOOL WINAPI getSize(HANDLE handle,LARGE_INTEGER* size){pathEvents.push_back(6);check(handle==reinterpret_cast<HANDLE>(42),"instance status reloads its handle");size->QuadPart=0x700000009ull;return sizeSuccess;}
decltype(&FileTimeToLocalFileTime) realLocal;
decltype(&FileTimeToSystemTime) realSystem;
BOOL WINAPI localTime(const FILETIME* input,FILETIME* output){int which=equalTime(*input,created)?1:equalTime(*input,accessed)?2:3;return failedLocal==which?FALSE:realLocal(input,output);}
BOOL WINAPI systemTime(const FILETIME* input,SYSTEMTIME* output){++systemCalls;return failedSystem==systemCalls?FALSE:realSystem(input,output);}
struct StatusSample{int answer;std::array<BYTE,560> bytes;std::vector<int> events;};
std::vector<StatusSample> statusSamples;
template<class F>int resultCategory(const F& action){try{return action();}catch(BoundaryError e){return e.kind==6?-6:e.kind==2?-2:-5;}catch(CException* e){auto* name=e->GetRuntimeClass()->m_lpszClassName;int result=!strcmp(name,"COleException")?-6:!strcmp(name,"CInvalidArgException")?-2:-5;e->Delete();return result;}}
void statusOperations(){
    created=stamp(2020);accessed=stamp(2021);written=stamp(2022);
    Patch a("GetFullPathNameW",reinterpret_cast<void*>(&fullPath)),b("GetVolumeInformationW",reinterpret_cast<void*>(&volume)),c("GetFileAttributesExW",reinterpret_cast<void*>(&attributesEx)),d("GetFileAttributesW",reinterpret_cast<void*>(&attributes)),e("GetFileTime",reinterpret_cast<void*>(&getTimes)),f("GetFileSizeEx",reinterpret_cast<void*>(&getSize)),g("FileTimeToLocalFileTime",reinterpret_cast<void*>(&localTime)),h("FileTimeToSystemTime",reinterpret_cast<void*>(&systemTime));
    realLocal=reinterpret_cast<decltype(realLocal)>(g.old);realSystem=reinterpret_cast<decltype(realSystem)>(h.old);size_t sample=0;
    for(int instance:{0,1,2})for(int scenario=0;scenario<13;++scenario){
        alignas(8) BYTE bytes[40];auto* file=api->fileCtor(bytes);file->m_strFileName=L"relative\\example.ext";store<HANDLE>(file,8,instance==1?INVALID_HANDLE_VALUE:reinterpret_cast<HANDLE>(42));
        created=stamp(2020);accessed=stamp(2021);CFileStatus out;memset(&out,0xa5,sizeof(out));pathMode=0;failedLocal=failedSystem=systemCalls=0;attributeValue=0x8121|FILE_ATTRIBUTE_NORMAL;attributeSuccess=timeSuccess=sizeSuccess=TRUE;
        if(scenario<4)pathMode=scenario;
        if(scenario==4)attributeSuccess=FALSE;if(scenario==5)timeSuccess=FALSE;if(scenario==6)sizeSuccess=FALSE;if(scenario==7)attributeValue=INVALID_FILE_ATTRIBUTES;if(scenario==8)failedLocal=1;if(scenario==9)failedLocal=2;if(scenario==10)failedSystem=2;if(scenario==11)created=accessed=FILETIME{};if(scenario==12)created=stamp(1900);
        pathEvents.clear();int answer=resultCategory([&]{return instance?api->fileStatus(file,out):api->pathStatus(L"relative\\example.ext",out,nullptr);});
        if(!instance&&!scenario)check(answer==TRUE&&out.m_size==0x700000009ull&&out.m_attribute==0x8121&&out.m_ctime>1500000000&&out.m_ctime<1600000000,"static status returns seconds, full64 size and full attributes without NORMAL bit");
        if(instance==1)check(answer==TRUE&&!out.m_ctime&&!out.m_mtime&&!out.m_atime&&!out.m_size&&!out.m_attribute&&pathEvents.empty(),"invalid-handle status succeeds with zero metadata and stored filename");
        if(instance==2&&scenario==4)check(answer==TRUE&&out.m_attribute==(0x8121|FILE_ATTRIBUTE_NORMAL),"instance status uses GetFileAttributes rather than querying path metadata");
        if(instance==2&&scenario==5)check(!answer&&pathEvents==std::vector<int>{5}&&!out.m_size,"GetFileTime failure returns after clearing status and copying filename");
        if(instance==2&&scenario==6)check(!answer&&pathEvents==std::vector<int>({5,6})&&!out.m_size,"GetFileSizeEx failure preserves only the initialized filename");
        if(instance!=1&&(scenario==8||scenario==9||scenario==11))check((scenario==8?out.m_ctime:out.m_atime)==out.m_mtime,"missing creation/access conversion falls back to modification time");
        StatusSample value{};value.answer=answer;memcpy(value.bytes.data(),&out,sizeof(out));value.events=pathEvents;
        if(!api->image)statusSamples.push_back(value);else{auto& old=statusSamples[sample];check(value.answer==old.answer&&value.bytes==old.bytes&&value.events==old.events,"source/native status matches every byte, partial failure and OS query order");}++sample;
        store<HANDLE>(file,8,INVALID_HANDLE_VALUE);api->fileDtor(file);
    }
    CFileStatus out;memset(&out,0xa5,sizeof(out));auto original=out;pathEvents.clear();std::wstring longPath(260,L'x');
    check(!api->pathStatus(nullptr,out,nullptr)&&!memcmp(&out,&original,sizeof(out))&&!api->pathStatus(longPath.c_str(),out,nullptr)&&!memcmp(&out,&original,sizeof(out))&&pathEvents.empty(),"null and too-long static paths leave all output untouched");
}
struct FileFailure {bool caught=false;long os=0;std::wstring path;bool nullPath=false;};
template<class F>FileFailure fileFailure(F action){FileFailure result;try{action();}catch(BoundaryError e){result={true,e.os,e.path,e.nullPath};}catch(CException* e){check(!strcmp(e->GetRuntimeClass()->m_lpszClassName,"CFileException"),"file operation throws CFileException");auto* f=static_cast<CFileException*>(e);result={true,f->m_lOsError,f->m_strFileName.GetString(),f->m_strFileName.IsEmpty()!=FALSE};e->Delete();}return result;}
struct Transaction {HANDLE handle;BOOL fallback;};
Transaction* currentTransaction;int transactionMode;bool loaderMutates;DWORD changeAttributes;int failOperation;bool modifyStatus;CFileStatus* changingStatus;
BOOL WINAPI movePath(const wchar_t* oldName,const wchar_t* newName){pathEvents.push_back(10);check(oldName&&!wcscmp(oldName,L"old")&&newName&&!wcscmp(newName,L"new"),"rename forwards original paths");SetLastError(ERROR_ACCESS_DENIED);return failOperation!=10;}
BOOL WINAPI deletePath(const wchar_t* name){pathEvents.push_back(11);check(name&&!wcscmp(name,L"old"),"remove forwards original path");SetLastError(ERROR_ACCESS_DENIED);return failOperation!=11;}
BOOL WINAPI changePath(const wchar_t*,DWORD attrs){pathEvents.push_back(12);changeAttributes=attrs;SetLastError(ERROR_ACCESS_DENIED);return failOperation!=12;}
HANDLE WINAPI openPath(const wchar_t*,DWORD access,DWORD share,SECURITY_ATTRIBUTES* security,DWORD create,DWORD flags,HANDLE templateFile){pathEvents.push_back(13);check(access==(GENERIC_READ|GENERIC_WRITE)&&share==FILE_SHARE_READ&&!security&&create==OPEN_EXISTING&&flags==FILE_ATTRIBUTE_NORMAL&&!templateFile,"SetStatus opens with native read/write access and share flags");SetLastError(ERROR_SHARING_VIOLATION);return failOperation==13?INVALID_HANDLE_VALUE:reinterpret_cast<HANDLE>(77);}
FILETIME seenCreate,seenAccess,seenWrite;bool hasCreate,hasAccess;
BOOL WINAPI setTimes(HANDLE handle,const FILETIME* c,const FILETIME* a,const FILETIME* w){pathEvents.push_back(14);check(handle==reinterpret_cast<HANDLE>(77)&&w,"SetStatus always supplies modification FILETIME");hasCreate=c!=nullptr;hasAccess=a!=nullptr;if(c)seenCreate=*c;if(a)seenAccess=*a;seenWrite=*w;if(modifyStatus)changingStatus->m_attribute=0x4020;SetLastError(ERROR_WRITE_FAULT);return failOperation!=14;}
BOOL WINAPI closePath(HANDLE handle){pathEvents.push_back(15);check(handle==reinterpret_cast<HANDLE>(77),"SetStatus closes the opened handle");SetLastError(ERROR_INVALID_HANDLE);return failOperation!=15;}
BOOL WINAPI moveTransaction(const wchar_t* a,const wchar_t* b,LPPROGRESS_ROUTINE progress,void* data,DWORD flags,HANDLE transaction){check(!progress&&!data&&flags==MOVEFILE_COPY_ALLOWED&&transaction==currentTransaction->handle,"rename transacted arguments and fresh transaction handle");return movePath(a,b);}
BOOL WINAPI deleteTransaction(const wchar_t* name,HANDLE transaction){check(transaction==currentTransaction->handle,"delete uses current transaction");return deletePath(name);}
BOOL WINAPI changeTransaction(const wchar_t* name,DWORD attrs,HANDLE transaction){check(transaction==currentTransaction->handle,"attributes use current transaction");return changePath(name,attrs);}
BOOL WINAPI attributesTransaction(const wchar_t* name,GET_FILEEX_INFO_LEVELS level,void* out,HANDLE transaction){check(transaction==currentTransaction->handle,"attribute query uses current transaction");return attributesEx(name,level,out);}
HANDLE WINAPI openTransaction(const wchar_t* name,DWORD access,DWORD share,SECURITY_ATTRIBUTES* security,DWORD create,DWORD flags,HANDLE templateFile,HANDLE transaction,PUSHORT mini,void* extended){check(transaction==currentTransaction->handle&&!mini&&!extended,"open transacted arguments");return openPath(name,access,share,security,create,flags,templateFile);}
HMODULE WINAPI kernelModule(const wchar_t* name){pathEvents.push_back(20);check(!wcscmp(name,L"KERNEL32.DLL")||!wcscmp(name,L"kernel32.dll"),"transaction procedure module");return transactionMode==1?nullptr:reinterpret_cast<HMODULE>(33);}
FARPROC WINAPI transactionProcedure(HMODULE module,const char* name){pathEvents.push_back(21);check(module==reinterpret_cast<HMODULE>(33),"transaction procedure lookup uses module handle");if(loaderMutates)currentTransaction->handle=reinterpret_cast<HANDLE>(99);if(transactionMode==2)return nullptr;
    if(!strcmp(name,"MoveFileTransactedW"))return reinterpret_cast<FARPROC>(&moveTransaction);if(!strcmp(name,"DeleteFileTransactedW"))return reinterpret_cast<FARPROC>(&deleteTransaction);if(!strcmp(name,"GetFileAttributesTransactedW"))return reinterpret_cast<FARPROC>(&attributesTransaction);if(!strcmp(name,"SetFileAttributesTransactedW"))return reinterpret_cast<FARPROC>(&changeTransaction);if(!strcmp(name,"CreateFileTransactedW"))return reinterpret_cast<FARPROC>(&openTransaction);check(false,"known transaction procedure");return nullptr;}
void transactionOperations(){
    Patch a("MoveFileW",reinterpret_cast<void*>(&movePath)),b("DeleteFileW",reinterpret_cast<void*>(&deletePath)),c("GetProcAddress",reinterpret_cast<void*>(&transactionProcedure)),d("GetModuleHandleW",reinterpret_cast<void*>(&kernelModule));
    // Patch construction also uses GetModuleHandleW; subsequent patches must
    // already have been constructed before replacing that import.
    for(int type=0;type<6;++type)for(int operation:{0,1})for(bool failure:{false,true}){
        Transaction manager{type>=3?reinterpret_cast<HANDLE>(55):nullptr,type==2?TRUE:FALSE};currentTransaction=&manager;transactionMode=type==4?1:type==5?2:0;loaderMutates=type==3;failOperation=failure?(operation?11:10):0;pathEvents.clear();SetLastError(ERROR_ACCESS_DENIED);
        auto* tx=type?reinterpret_cast<ATL::CAtlTransactionManager*>(&manager):nullptr;auto result=fileFailure([&]{if(operation)api->remove(L"old",tx);else api->rename(L"old",L"new",tx);});
        bool blocked=type==1||type>=4;check(result.caught==(blocked||failure),"rename/remove honor fallback, loader failure and API errors");if(result.caught)check(result.os==ERROR_ACCESS_DENIED&&result.path==L"old","rename/remove preserve original path and OS error");
        check(pathEvents==(type>=3?(type==4?std::vector<int>{20}:type==5?std::vector<int>{20,21}:std::vector<int>{20,21,operation?11:10}):type==1?std::vector<int>{}:std::vector<int>{operation?11:10}),"transaction dispatch never silently falls back when a transacted procedure is missing");
    }
}
void statusSetting(){
    Patch a("GetFileAttributesW",reinterpret_cast<void*>(&attributes)),b("GetFileAttributesExW",reinterpret_cast<void*>(&attributesEx)),c("SetFileAttributesW",reinterpret_cast<void*>(&changePath)),d("CreateFileW",reinterpret_cast<void*>(&openPath)),e("SetFileTime",reinterpret_cast<void*>(&setTimes)),f("CloseHandle",reinterpret_cast<void*>(&closePath));
    Patch g("GetProcAddress",reinterpret_cast<void*>(&transactionProcedure)),h("GetModuleHandleW",reinterpret_cast<void*>(&kernelModule));
    created=stamp(2020);accessed=stamp(2021);written=stamp(2022);transactionMode=0;loaderMutates=false;attributeSuccess=TRUE;
    for(bool transacted:{false,true})for(bool readOnly:{false,true})for(int times:{0,1,2})for(int failure:{0,12,13,14,15}){
        Transaction manager{reinterpret_cast<HANDLE>(55),FALSE};currentTransaction=&manager;
        CFileStatus status{};status.m_attribute=FILE_ATTRIBUTE_HIDDEN;status.m_mtime=times?1609459200:0;status.m_ctime=times==2?1577836800:0;status.m_atime=times==2?1640995200:0;attributeValue=readOnly?FILE_ATTRIBUTE_READONLY:FILE_ATTRIBUTE_ARCHIVE;
        failOperation=failure;modifyStatus=false;changingStatus=&status;pathEvents.clear();auto result=fileFailure([&]{api->setStatus(L"old",status,transacted?reinterpret_cast<ATL::CAtlTransactionManager*>(&manager):nullptr);});
        bool failed=failure==12||(times&&failure>=13);check(result.caught==failed,"SetStatus reports each attribute/open/time/close failure");if(failed)check(result.os==(failure==12?ERROR_ACCESS_DENIED:failure==13?ERROR_SHARING_VIOLATION:failure==14?ERROR_WRITE_FAULT:ERROR_INVALID_HANDLE)&&result.path==L"old","SetStatus preserves OS error across cleanup and keeps original path");
        std::vector<int> ordinary;for(auto event:pathEvents)if(event!=20&&event!=21)ordinary.push_back(event);
        std::vector<int> expected{transacted?3:4};if(readOnly)expected.push_back(12);
        if(!(readOnly&&failure==12)&&times){expected.push_back(13);if(failure!=13){expected.push_back(14);expected.push_back(15);}}
        if(!failed&&!readOnly)expected.push_back(12);else if(failure==12&&!readOnly)expected.push_back(12);
        check(ordinary==expected,"SetStatus clears read-only first, opens only for modification time, and applies other attributes last");
        if(times&&(!readOnly||failure!=12)&&failure!=13){check(hasCreate==(times==2)&&hasAccess==(times==2),"zero creation/access times become null SetFileTime arguments");}
    }
    failOperation=0;attributeValue=FILE_ATTRIBUTE_ARCHIVE;CFileStatus status{};status.m_attribute=FILE_ATTRIBUTE_HIDDEN;status.m_mtime=1609459200;changingStatus=&status;modifyStatus=true;pathEvents.clear();api->setStatus(L"old",status,nullptr);check(changeAttributes==0x4020,"SetStatus reloads requested attributes after time callbacks");modifyStatus=false;
    CFileStatus empty{};Transaction blocked{nullptr,FALSE};pathEvents.clear();SetLastError(ERROR_ACCESS_DENIED);api->setStatus(L"old",empty,reinterpret_cast<ATL::CAtlTransactionManager*>(&blocked));check(pathEvents.empty(),"disabled transaction attribute lookup yields zero and needs no OS call when nothing changes");
}
struct NameSample{UINT result;std::array<wchar_t,400> text;};std::vector<NameSample> nameSamples;
void fileNames(){
    size_t sample=0;
    for(auto* path:{L"",L"simple.ext",L"C:\\a\\report.txt",L"C:relative.ext",L"folder/last.ext",L"C:\\folder\\",L"C:\\spaces in name.ext",L"C:\\*.ext"})for(bool title:{false,true})for(UINT capacity:{0u,1u,4u,256u})for(bool output:{false,true}){
        NameSample current{};current.text.fill(0xa5a5);current.result=resultCategory([&]{return int((title?api->titleHelper:api->nameHelper)(path,output?current.text.data():nullptr,capacity));});
        if(!api->image)nameSamples.push_back(current);else check(current.result==nameSamples[sample].result&&current.text==nameSamples[sample].text,"filename/title helpers match native null output, truncation, errors and path parsing");++sample;
    }
    check(resultCategory([&]{return int(api->nameHelper(nullptr,nullptr,0));})==-2,"filename helper rejects a null path before parsing");
    for(auto* name:{L"C:\\dir\\file.ext",L"C:relative.txt",L"dir/file.txt",L""}){
        alignas(8) BYTE bytes[40];auto* file=api->fileCtor(bytes);file->m_strFileName=name;
        for(bool title:{false,true}){alignas(CString) BYTE result[sizeof(CString)];(title?api->fileTitle:api->fileName)(file,result);auto* text=reinterpret_cast<CString*>(result);wchar_t expected[256];(title?api->titleHelper:api->nameHelper)(name,expected,256);check(!wcscmp(text->GetString(),expected),"file name/title dispatches through native status and helper");text->~CString();}api->fileDtor(file);
    }
}
int localCalls;LONGLONG* localInput;bool failSystemTime,failLocalTime;SYSTEMTIME convertedSystem;
errno_t __cdecl localParts(tm* parts,const __time64_t* input){++localCalls;check(input==localInput&&*input==100+localCalls,"time helper reloads original CTime for each field");++*localInput;memset(parts,0,sizeof(*parts));parts->tm_year=101;parts->tm_mon=1;parts->tm_mday=3;parts->tm_hour=4;parts->tm_min=5;parts->tm_sec=6;return 0;}
BOOL WINAPI toFileTime(const SYSTEMTIME* input,FILETIME* output){convertedSystem=*input;*output={17,18};SetLastError(ERROR_INVALID_TIME);return !failSystemTime;}
BOOL WINAPI fromLocalTime(const FILETIME* input,FILETIME* output){check(input->dwLowDateTime==17&&input->dwHighDateTime==18,"time helper converts generated local FILETIME");*output={21,22};SetLastError(ERROR_INVALID_TIME);return !failLocalTime;}
void timeConversion(){
    Patch a("_localtime64_s",reinterpret_cast<void*>(&localParts)),b("SystemTimeToFileTime",reinterpret_cast<void*>(&toFileTime)),c("LocalFileTimeToFileTime",reinterpret_cast<void*>(&fromLocalTime));
    for(int failure:{0,1,2}){LONGLONG time=101;localInput=&time;localCalls=0;failSystemTime=failure==1;failLocalTime=failure==2;FILETIME out{};auto result=fileFailure([&]{api->timeHelper(&time,&out);});
        check(localCalls==6&&time==107&&convertedSystem.wYear==2001&&convertedSystem.wMonth==2&&convertedSystem.wDay==3&&convertedSystem.wHour==4&&convertedSystem.wMinute==5&&convertedSystem.wSecond==6&&!convertedSystem.wMilliseconds,"CTime conversion uses all six local-time accessors and clears milliseconds");
        check(result.caught==(failure!=0)&&(!failure||(result.os==ERROR_INVALID_TIME&&result.nullPath)),"time-conversion failures throw file exceptions with no filename");if(!failure)check(out.dwLowDateTime==21&&out.dwHighDateTime==22,"time helper returns converted UTC FILETIME");}
    localCalls=0;check(resultCategory([&]{LONGLONG time=101;api->timeHelper(&time,nullptr);return 0;})==-2&&!localCalls,"null FILETIME output throws before consulting CTime");
}

void transactionStatus(){
    created=stamp(2020);accessed=stamp(2021);written=stamp(2022);pathMode=failedLocal=failedSystem=0;attributeValue=0x4080;attributeSuccess=timeSuccess=sizeSuccess=TRUE;
    Patch a("GetFullPathNameW",reinterpret_cast<void*>(&fullPath)),b("GetVolumeInformationW",reinterpret_cast<void*>(&volume)),c("GetFileAttributesExW",reinterpret_cast<void*>(&attributesEx)),d("GetFileTime",reinterpret_cast<void*>(&getTimes)),e("GetFileSizeEx",reinterpret_cast<void*>(&getSize)),f("GetProcAddress",reinterpret_cast<void*>(&transactionProcedure)),g("GetModuleHandleW",reinterpret_cast<void*>(&kernelModule));
    for(int mode=0;mode<5;++mode)for(bool instance:{false,true}){
        Transaction manager{mode>=2?reinterpret_cast<HANDLE>(55):nullptr,mode==1?TRUE:FALSE};currentTransaction=&manager;transactionMode=mode==3?1:mode==4?2:0;loaderMutates=mode==2;
        alignas(8) BYTE bytes[40];auto* file=api->fileCtor(bytes);file->m_strFileName=L"relative\\example.ext";store<HANDLE>(file,8,reinterpret_cast<HANDLE>(42));store<void*>(file,32,&manager);CFileStatus out;memset(&out,0xa5,sizeof(out));pathEvents.clear();
        int answer=instance?api->fileStatus(file,out):api->pathStatus(L"relative\\example.ext",out,reinterpret_cast<ATL::CAtlTransactionManager*>(&manager));bool succeeds=mode==1||mode==2;
        check(answer==(instance||succeeds),"static transaction status fails on missing query; instance status retains valid handle metadata");
        check(instance?out.m_attribute==(succeeds?attributeValue:0):succeeds?out.m_attribute==(attributeValue&~FILE_ATTRIBUTE_NORMAL):out.m_attribute==0xa5a5a5a5,"transaction attribute helper preserves native zero-on-failure and static output ordering");
        store<HANDLE>(file,8,INVALID_HANDLE_VALUE);api->fileDtor(file);
    }
}
void nameStatusSideEffects(){
    Patch a("GetFileTime",reinterpret_cast<void*>(&getTimes)),b("GetFileSizeEx",reinterpret_cast<void*>(&getSize));timeSuccess=FALSE;pathEvents.clear();
    alignas(8) BYTE bytes[40];auto* file=api->fileCtor(bytes);file->m_strFileName=L"C:\\folder\\file.ext";store<HANDLE>(file,8,reinterpret_cast<HANDLE>(42));
    for(auto method:{api->fileName,api->fileTitle}){alignas(CString) BYTE buffer[sizeof(CString)];pathEvents.clear();method(file,buffer);auto* result=reinterpret_cast<CString*>(buffer);check(pathEvents==std::vector<int>{5}&&!wcscmp(result->GetString(),L"file.ext"),"GetFileName/GetFileTitle query status even when it fails, then use the copied filename");result->~CString();}
    store<HANDLE>(file,8,INVALID_HANDLE_VALUE);api->fileDtor(file);
}
void pathRoundTrip(){
    wchar_t folder[MAX_PATH],name[MAX_PATH];check(GetTempPathW(MAX_PATH,folder)!=0&&GetTempFileNameW(folder,L"mfs",0,name)!=0,"create real file for path operations");
    auto handle=CreateFileW(name,GENERIC_WRITE,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,nullptr);DWORD writtenBytes=0;check(handle!=INVALID_HANDLE_VALUE&&WriteFile(handle,"abc",3,&writtenBytes,nullptr)&&writtenBytes==3,"write real status fixture");CloseHandle(handle);
    CFileStatus status{};check(api->pathStatus(name,status,nullptr)&&status.m_size==3&&status.m_szFullName[0],"static status reads a real file and resolves full path");
    status.m_mtime=1609459200;status.m_ctime=status.m_atime=0;status.m_attribute=FILE_ATTRIBUTE_READONLY;api->setStatus(name,status,nullptr);status={};
    check(api->pathStatus(name,status,nullptr)&&status.m_mtime==1609459200&&(status.m_attribute&FILE_ATTRIBUTE_READONLY),"SetStatus converts epoch seconds and applies read-only after updating real file times");
    auto error=fileFailure([&]{api->remove(name,nullptr);});check(error.caught&&error.os==ERROR_ACCESS_DENIED&&error.path==name,"removing a real read-only file reports access denied and its name");SetFileAttributesW(name,FILE_ATTRIBUTE_NORMAL);
    std::wstring renamed=std::wstring(name)+L".renamed";api->rename(name,renamed.c_str(),nullptr);check(GetFileAttributesW(name)==INVALID_FILE_ATTRIBUTES&&api->pathStatus(renamed.c_str(),status,nullptr)&&status.m_size==3,"rename moves a real file and preserves contents");api->remove(renamed.c_str(),nullptr);check(GetFileAttributesW(renamed.c_str())==INVALID_FILE_ATTRIBUTES,"remove deletes the renamed file");
    error=fileFailure([&]{api->setStatus(renamed.c_str(),status,nullptr);});check(error.caught&&error.os==ERROR_FILE_NOT_FOUND&&error.path==renamed,"SetStatus reports an initial attribute-query failure on a missing real file");
}

std::vector<std::wstring> rootSamples;std::vector<std::array<BYTE,520>> fullSamples;
void rootsAndFullPaths(){
    size_t sample=0;
    for(auto* path:{L"C:\\folder\\file.ext",L"\\\\server\\share\\file",L"relative\\file",L"/unix/file",L"//server/share/file",L"C:relative",L""}){
        CString result(L"seed");api->rootHelper(path,result);if(!api->image)rootSamples.emplace_back(result.GetString());else check(rootSamples[sample]==result.GetString(),"root helper follows PathStripToRoot semantics for drive, UNC and relative paths");++sample;
    }
    CString result(L"seed");check(resultCategory([&]{api->rootHelper(nullptr,result);return 0;})==-2,"root helper rejects null path through checked string copy");
    Patch a("GetFullPathNameW",reinterpret_cast<void*>(&fullPath)),b("GetVolumeInformationW",reinterpret_cast<void*>(&volume));sample=0;
    for(int mode=0;mode<4;++mode){pathMode=mode;wchar_t output[260];memset(output,0xa5,sizeof(output));pathEvents.clear();auto ok=api->fullHelper(output,L"relative");check(ok==(mode==0),"full-path helper reports path resolution and volume failures");
        check(!wcscmp(output,mode==1?L"relative":L"C:\\parent\\example.ext"),"full-path failure preserves the native partial output");std::array<BYTE,520> bytes;memcpy(bytes.data(),output,sizeof(output));if(!api->image)fullSamples.push_back(bytes);else check(bytes==fullSamples[sample],"full-path helper preserves every output byte on all exits");++sample;}
    check(resultCategory([&]{return api->fullHelper(nullptr,L"input");})==-2&&resultCategory([&]{wchar_t out[260];return api->fullHelper(out,nullptr);})==-2,"full-path helper validates both pointers");
}
void run(const API& functions,const char* label) { api=&functions;int before=failures;basicMemory();buffers();callbacks();shared();files();statusOperations();fileNames();statusSetting();transactionOperations();timeConversion();transactionStatus();nameStatusSideEffects();pathRoundTrip();rootsAndFullPaths();std::printf("%s memory and shared files: %d failures\n",label,failures-before); }
void __cdecl invalidParameter(const wchar_t*,const wchar_t*,const wchar_t*,unsigned,uintptr_t) {}
int main() {
    _set_invalid_parameter_handler(invalidParameter);
    API source{};
#define SOURCE(field,alias,name,rva,result,args) source.field=&alias;
    FILE_API(SOURCE)
#undef SOURCE
    run(source,"Source");
    wchar_t path[1024];if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_FILES_DLL",path,1024)) {
        HMODULE module=LoadLibraryW(path);check(module!=nullptr,"load the pinned native MFC DLL");if(!module)return 1;
        auto* b=reinterpret_cast<BYTE*>(module);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);
        if(pe->FileHeader.TimeDateStamp!=0x442cba30 || pe->OptionalHeader.SizeOfImage!=0x55c000){std::puts("Unsupported native oracle version");return 1;}
        API native{};native.image=module;
#define NATIVE(field,alias,name,rva,result,args) native.field=reinterpret_cast<decltype(native.field)>(b+rva);check(native.field!=nullptr,name);if(!native.field)return 1;
        FILE_API(NATIVE)
#undef NATIVE
        // The native CRT has its own checked-copy handler.
        auto crt=GetModuleHandleW(L"ucrtbase.dll");if(crt){using SetHandler=_invalid_parameter_handler (__cdecl*)(_invalid_parameter_handler);auto set=reinterpret_cast<SetHandler>(GetProcAddress(crt,"_set_invalid_parameter_handler"));if(set)set(invalidParameter);}
        run(native,"Native");
    }
    return failures?1:0;
}
