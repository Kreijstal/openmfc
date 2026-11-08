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

// These surrounding services are outside the buffer and Win32 file tests.
CRuntimeClass CException::classCException{"CException",16,0xffff,nullptr,&CObject::GetThisClass,nullptr,nullptr};
CRuntimeClass CFileException::classCFileException{"CFileException",32,0xffff,nullptr,&CException::GetThisClass,nullptr,nullptr};
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const { std::abort(); }
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) { std::abort(); }
int CException::ReportError(UINT,UINT) { std::abort(); }
void CException::Dump() const { std::abort(); }
void CException::AssertValid() const { std::abort(); }
int CFileException::GetErrorMessage(wchar_t*,UINT,UINT*) const { std::abort(); }
void CFileException::Dump() const { std::abort(); }
void CFileException::AssertValid() const { std::abort(); }
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT) { std::abort(); }
struct BoundaryError { int kind; long os; };
extern "C" void impl__AfxThrowOleException__YAXJ_Z(long code) { throw BoundaryError{6,code}; }
void AFXAPI AfxThrowMemoryException() { throw BoundaryError{1,0}; }
void AFXAPI AfxThrowInvalidArgException() { throw BoundaryError{2,0}; }
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ() { throw BoundaryError{3,0}; }
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause,long os,const wchar_t*) { throw BoundaryError{cause==9?4:5,os}; }

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
 X(fileBuffer,impl__GetBufferPtr_CFile__UEAAIIIPEAPEAX0_Z,"?GetBufferPtr@CFile@@UEAAIIIPEAPEAX0@Z",0x71e0,UINT,(CFile*,UINT,UINT,void**,void**))
#define DECLARE(field,alias,name,rva,result,args) extern "C" result alias args;
FILE_API(DECLARE)
#undef DECLARE
struct API {
#define MEMBER(field,alias,name,rva,result,args) decltype(&alias) field;
FILE_API(MEMBER)
#undef MEMBER
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
void run(const API& functions,const char* label) { api=&functions;int before=failures;basicMemory();buffers();callbacks();shared();files();std::printf("%s memory and shared files: %d failures\n",label,failures-before); }
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
        API native{};
#define NATIVE(field,alias,name,rva,result,args) native.field=reinterpret_cast<decltype(native.field)>(b+rva);check(native.field!=nullptr,name);if(!native.field)return 1;
        FILE_API(NATIVE)
#undef NATIVE
        // The native CRT has its own checked-copy handler.
        auto crt=GetModuleHandleW(L"ucrtbase.dll");if(crt){using SetHandler=_invalid_parameter_handler (__cdecl*)(_invalid_parameter_handler);auto set=reinterpret_cast<SetHandler>(GetProcAddress(crt,"_set_invalid_parameter_handler"));if(set)set(invalidParameter);}
        run(native,"Native");
    }
    return failures?1:0;
}
