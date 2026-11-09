// Actual stream-file owners with client COM objects; exception factories are boundaries.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <array>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <climits>
extern "C" void impl__Write_COleStreamFile__UEAAXPEBXI_Z(COleStreamFile* p,const void* b,UINT n);
extern "C" void impl__UnlockRange_COleStreamFile__UEAAX_K0_Z(COleStreamFile* p,ULONGLONG o,ULONGLONG n);
extern "C" void impl__SetLength_COleStreamFile__UEAAX_K_Z(COleStreamFile* p,ULONGLONG n);
extern "C" ULONGLONG impl__Seek_COleStreamFile__UEAA_K_JI_Z(COleStreamFile* p,LONGLONG o,UINT f);
extern "C" UINT impl__Read_COleStreamFile__UEAAIPEAXI_Z(COleStreamFile* p,void* b,UINT n);
extern "C" BOOL impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(COleStreamFile* p,IStorage* s,const wchar_t* n,DWORD f,CFileException* e);
extern "C" void impl__LockRange_COleStreamFile__UEAAX_K0_Z(COleStreamFile* p,ULONGLONG o,ULONGLONG n);
extern "C" CRuntimeClass* impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ();
extern "C" IStream* impl__GetStream_COleStreamFile__QEBAPEAUIStream__XZ(const COleStreamFile* p);
extern "C" void* impl__GetStorageName_COleStreamFile__UEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const COleStreamFile* p,void* r);
extern "C" int impl__GetStatus_COleStreamFile__QEBAHAEAUCFileStatus___Z(const COleStreamFile* p,CFileStatus& s);
extern "C" CRuntimeClass* impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ(const COleStreamFile* p);
extern "C" ULONGLONG impl__GetPosition_COleStreamFile__UEBA_KXZ(const COleStreamFile* p);
extern "C" ULONGLONG impl__GetLength_COleStreamFile__UEBA_KXZ(const COleStreamFile* p);
extern "C" void impl__Flush_COleStreamFile__UEAAXXZ(COleStreamFile* p);
extern "C" CFile* impl__Duplicate_COleStreamFile__UEBAPEAVCFile__XZ(const COleStreamFile* p);
extern "C" IStream* impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ(COleStreamFile* p);
extern "C" BOOL impl__CreateStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(COleStreamFile* p,IStorage* s,const wchar_t* n,DWORD f,CFileException* e);
extern "C" BOOL impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z(COleStreamFile* p,CFileException* e);
extern "C" void impl__Close_COleStreamFile__UEAAXXZ(COleStreamFile* p);
extern "C" void impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(COleStreamFile* p,IStream* s);
extern "C" void impl__Abort_COleStreamFile__UEAAXXZ(COleStreamFile* p);
extern "C" void impl___1COleStreamFile__UEAA_XZ(COleStreamFile* p);
extern "C" COleStreamFile* impl___0COleStreamFile__QEAA_PEAUIStream___Z(void* p,IStream* s);
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long error);
extern "C" void impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long error,const wchar_t* name);
struct API {
    decltype(&impl__Write_COleStreamFile__UEAAXPEBXI_Z) Write;
    decltype(&impl__UnlockRange_COleStreamFile__UEAAX_K0_Z) UnlockRange;
    decltype(&impl__SetLength_COleStreamFile__UEAAX_K_Z) SetLength;
    decltype(&impl__Seek_COleStreamFile__UEAA_K_JI_Z) Seek;
    decltype(&impl__Read_COleStreamFile__UEAAIPEAXI_Z) Read;
    decltype(&impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z) OpenStream;
    decltype(&impl__LockRange_COleStreamFile__UEAAX_K0_Z) LockRange;
    decltype(&impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ) GetThisClass;
    decltype(&impl__GetStream_COleStreamFile__QEBAPEAUIStream__XZ) GetStream;
    decltype(&impl__GetStorageName_COleStreamFile__UEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ) GetStorageName;
    decltype(&impl__GetStatus_COleStreamFile__QEBAHAEAUCFileStatus___Z) GetStatus;
    decltype(&impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ) GetRuntimeClass;
    decltype(&impl__GetPosition_COleStreamFile__UEBA_KXZ) GetPosition;
    decltype(&impl__GetLength_COleStreamFile__UEBA_KXZ) GetLength;
    decltype(&impl__Flush_COleStreamFile__UEAAXXZ) Flush;
    decltype(&impl__Duplicate_COleStreamFile__UEBAPEAVCFile__XZ) Duplicate;
    decltype(&impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ) Detach;
    decltype(&impl__CreateStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z) CreateStream;
    decltype(&impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z) CreateMemoryStream;
    decltype(&impl__Close_COleStreamFile__UEAAXXZ) Close;
    decltype(&impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z) Attach;
    decltype(&impl__Abort_COleStreamFile__UEAAXXZ) Abort;
    decltype(&impl___1COleStreamFile__UEAA_XZ) dtor;
    decltype(&impl___0COleStreamFile__QEAA_PEAUIStream___Z) ctor;
    decltype(&impl__OsErrorToException_CFileException__SAHJ_Z) osCause;
    decltype(&impl__ThrowOsError_CFileException__SAXJPEB_W_Z) throwOs;
    HMODULE image;
};
const API* api;
API sourceAPI() { API result{};
    result.Write=&impl__Write_COleStreamFile__UEAAXPEBXI_Z;
    result.UnlockRange=&impl__UnlockRange_COleStreamFile__UEAAX_K0_Z;
    result.SetLength=&impl__SetLength_COleStreamFile__UEAAX_K_Z;
    result.Seek=&impl__Seek_COleStreamFile__UEAA_K_JI_Z;
    result.Read=&impl__Read_COleStreamFile__UEAAIPEAXI_Z;
    result.OpenStream=&impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z;
    result.LockRange=&impl__LockRange_COleStreamFile__UEAAX_K0_Z;
    result.GetThisClass=&impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ;
    result.GetStream=&impl__GetStream_COleStreamFile__QEBAPEAUIStream__XZ;
    result.GetStorageName=&impl__GetStorageName_COleStreamFile__UEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ;
    result.GetStatus=&impl__GetStatus_COleStreamFile__QEBAHAEAUCFileStatus___Z;
    result.GetRuntimeClass=&impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ;
    result.GetPosition=&impl__GetPosition_COleStreamFile__UEBA_KXZ;
    result.GetLength=&impl__GetLength_COleStreamFile__UEBA_KXZ;
    result.Flush=&impl__Flush_COleStreamFile__UEAAXXZ;
    result.Duplicate=&impl__Duplicate_COleStreamFile__UEBAPEAVCFile__XZ;
    result.Detach=&impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ;
    result.CreateStream=&impl__CreateStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z;
    result.CreateMemoryStream=&impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z;
    result.Close=&impl__Close_COleStreamFile__UEAAXXZ;
    result.Attach=&impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z;
    result.Abort=&impl__Abort_COleStreamFile__UEAAXXZ;
    result.dtor=&impl___1COleStreamFile__UEAA_XZ;
    result.ctor=&impl___0COleStreamFile__QEAA_PEAUIStream___Z;
    result.osCause=&impl__OsErrorToException_CFileException__SAHJ_Z;
    result.throwOs=&impl__ThrowOsError_CFileException__SAXJPEB_W_Z;
    return result;
}
API nativeAPI(HMODULE image) { API result{};auto* base=reinterpret_cast<BYTE*>(image);result.image=image;
    result.Write=reinterpret_cast<decltype(result.Write)>(base+0x265650);
    result.UnlockRange=reinterpret_cast<decltype(result.UnlockRange)>(base+0x2656c0);
    result.SetLength=reinterpret_cast<decltype(result.SetLength)>(base+0x265570);
    result.Seek=reinterpret_cast<decltype(result.Seek)>(base+0x265540);
    result.Read=reinterpret_cast<decltype(result.Read)>(base+0x265600);
    result.OpenStream=reinterpret_cast<decltype(result.OpenStream)>(base+0x265250);
    result.LockRange=reinterpret_cast<decltype(result.LockRange)>(base+0x265690);
    result.GetThisClass=reinterpret_cast<decltype(result.GetThisClass)>(base+0x261150);
    result.GetStream=reinterpret_cast<decltype(result.GetStream)>(base+0x2657a0);
    result.GetStorageName=reinterpret_cast<decltype(result.GetStorageName)>(base+0x2651e0);
    result.GetStatus=reinterpret_cast<decltype(result.GetStatus)>(base+0x265030);
    result.GetRuntimeClass=reinterpret_cast<decltype(result.GetRuntimeClass)>(base+0x261150);
    result.GetPosition=reinterpret_cast<decltype(result.GetPosition)>(base+0x265210);
    result.GetLength=reinterpret_cast<decltype(result.GetLength)>(base+0x2655a0);
    result.Flush=reinterpret_cast<decltype(result.Flush)>(base+0x265730);
    result.Duplicate=reinterpret_cast<decltype(result.Duplicate)>(base+0x2654e0);
    result.Detach=reinterpret_cast<decltype(result.Detach)>(base+0x264f10);
    result.CreateStream=reinterpret_cast<decltype(result.CreateStream)>(base+0x265310);
    result.CreateMemoryStream=reinterpret_cast<decltype(result.CreateMemoryStream)>(base+0x265490);
    result.Close=reinterpret_cast<decltype(result.Close)>(base+0x265760);
    result.Attach=reinterpret_cast<decltype(result.Attach)>(base+0x264f20);
    result.Abort=reinterpret_cast<decltype(result.Abort)>(base+0x2656f0);
    result.dtor=reinterpret_cast<decltype(result.dtor)>(base+0x264e80);
    result.ctor=reinterpret_cast<decltype(result.ctor)>(base+0x264d10);
    result.osCause=reinterpret_cast<decltype(result.osCause)>(base+0x22cf10);
    result.throwOs=reinterpret_cast<decltype(result.throwOs)>(base+0x22cc70);
    return result;
}
int failures;
void check(bool ok,const char* message) { if(!ok){++failures;std::printf("FAIL: %s\n",message);} }
// Surrounding framework exception creation and presentation are outside this fixture.
CRuntimeClass CException::classCException{"CException",16,0xffff,nullptr,&CObject::GetThisClass,nullptr,nullptr};
int CException::GetErrorMessage(wchar_t*,UINT,UINT*) const {std::puts("unexpected CException diagnostic");std::abort();}
int CException::GetErrorMessage(wchar_t* p,UINT n,UINT* h){return static_cast<const CException*>(this)->GetErrorMessage(p,n,h);}
int CException::ReportError(UINT,UINT){std::abort();}
int CFileException::GetErrorMessage(wchar_t*,UINT,UINT*) const {std::puts("unexpected CFileException diagnostic");std::abort();}
struct Boundary {int kind;long code;};
void AFXAPI AfxThrowOleException(long code){throw Boundary{3,code};}
void AFXAPI AfxThrowMemoryException(){throw Boundary{1,0};}
void AFXAPI AfxThrowInvalidArgException(){throw Boundary{2,0};}
extern "C" void impl__AfxThrowOleException__YAXJ_Z(long code){throw Boundary{3,code};}
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause,long os,const wchar_t* name){
    auto* error=new CFileException(cause,os);if(name)error->m_strFileName=name;throw error;
}
std::wstring diagnostic;
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* text,UINT type,UINT help){check(!type&&!help,"native destructor message flags");diagnostic=text;return 0;}
std::vector<int> events;
template<class T> T field(const void* p,size_t offset){T value;memcpy(&value,static_cast<const BYTE*>(p)+offset,sizeof(value));return value;}
template<class T> void store(void* p,size_t offset,T value){memcpy(static_cast<BYTE*>(p)+offset,&value,sizeof(value));}
struct File {
    alignas(8) BYTE bytes[56];COleStreamFile* p;
    explicit File(IStream* stream=nullptr){memset(bytes,0xa5,sizeof(bytes));p=api->ctor(bytes,stream);}
    ~File(){api->dtor(p);}
};
std::wstring storageName(COleStreamFile* p){alignas(CString) BYTE bytes[sizeof(CString)];void* result=api->GetStorageName(p,bytes);auto* s=reinterpret_cast<CString*>(bytes);check(result==bytes,"GetStorageName returns its hidden result buffer in RAX");std::wstring name=s->GetString();s->~CString();return name;}
FILETIME timeAt(WORD year,WORD month=1,WORD day=1){SYSTEMTIME s{};s.wYear=year;s.wMonth=month;s.wDay=day;s.wHour=12;FILETIME f{};check(SystemTimeToFileTime(&s,&f)!=FALSE,"build a file-time value");return f;}
struct ClientStream:IStream {
    ULONG refs=1,readCount=3,writeCount=1;HRESULT answer=S_OK;ULONGLONG position=0x100000002ull,size=0x200000003ull;
    ULONGLONG lastOffset=0,lastSize=0;DWORD lastFlags=0;const wchar_t* name=nullptr;
    FILETIME mtime=timeAt(2020),ctime=timeAt(2021),atime=timeAt(2022);
    IStream* clone=nullptr;COleStreamFile* mutate=nullptr;IStream* replacement=nullptr;bool detachOnCommit=false;
    CException* thrown=nullptr;void* lastStatusName=nullptr;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void** p) override{*p=nullptr;return E_NOINTERFACE;}
    ULONG STDMETHODCALLTYPE AddRef() override{events.push_back(2);return ++refs;}
    ULONG STDMETHODCALLTYPE Release() override{events.push_back(3);if(mutate)store<IStream*>(mutate,40,replacement);return --refs;}
    HRESULT STDMETHODCALLTYPE Read(void* p,ULONG n,ULONG* count) override{events.push_back(4);lastSize=n;*count=readCount;if(n&&p)memset(p,'r',(std::min)(n,readCount));return answer;}
    HRESULT STDMETHODCALLTYPE Write(const void*,ULONG n,ULONG* count) override{events.push_back(5);lastSize=n;*count=writeCount;return answer;}
    HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER offset,DWORD origin,ULARGE_INTEGER* out) override{events.push_back(6);lastOffset=offset.QuadPart;lastFlags=origin;out->QuadPart=position;return answer;}
    HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER n) override{events.push_back(7);lastSize=n.QuadPart;return answer;}
    HRESULT STDMETHODCALLTYPE CopyTo(IStream*,ULARGE_INTEGER,ULARGE_INTEGER*,ULARGE_INTEGER*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Commit(DWORD flags) override{events.push_back(9);lastFlags=flags;if(thrown)throw thrown;if(detachOnCommit)store<IStream*>(mutate,40,replacement);return answer;}
    HRESULT STDMETHODCALLTYPE Revert() override{events.push_back(10);if(mutate)store<IStream*>(mutate,40,replacement);return answer;}
    HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER o,ULARGE_INTEGER n,DWORD flags) override{events.push_back(11);lastOffset=o.QuadPart;lastSize=n.QuadPart;lastFlags=flags;return answer;}
    HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER o,ULARGE_INTEGER n,DWORD flags) override{events.push_back(12);lastOffset=o.QuadPart;lastSize=n.QuadPart;lastFlags=flags;return answer;}
    HRESULT STDMETHODCALLTYPE Stat(STATSTG* s,DWORD flags) override {
        events.push_back(13);lastFlags=flags;memset(s,0,sizeof(*s));s->cbSize.QuadPart=size;s->mtime=mtime;s->ctime=ctime;s->atime=atime;
        if(name&&flags==STATFLAG_DEFAULT){size_t bytes=(wcslen(name)+1)*sizeof(wchar_t);s->pwcsName=static_cast<wchar_t*>(CoTaskMemAlloc(bytes));memcpy(s->pwcsName,name,bytes);lastStatusName=s->pwcsName;}
        return answer;
    }
    HRESULT STDMETHODCALLTYPE Clone(IStream** out) override{events.push_back(14);*out=clone;return answer;}
};
struct ClientStorage:IStorage {
    IStream* stream=nullptr;HRESULT answer=S_OK;DWORD flags=0;std::wstring seenName;const wchar_t* name=nullptr;IStream** seenOutput=nullptr;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID,void** p) override{*p=nullptr;return E_NOINTERFACE;}
    ULONG STDMETHODCALLTYPE AddRef() override{return 2;}ULONG STDMETHODCALLTYPE Release() override{return 1;}
    HRESULT STDMETHODCALLTYPE CreateStream(const wchar_t* n,DWORD f,DWORD a,DWORD b,IStream** out) override{events.push_back(20);check(!a&&!b,"CreateStream reserved arguments");seenName=n;flags=f;seenOutput=out;*out=stream;return answer;}
    HRESULT STDMETHODCALLTYPE OpenStream(const wchar_t* n,void* reserved,DWORD f,DWORD r,IStream** out) override{events.push_back(21);check(!reserved&&!r,"OpenStream reserved arguments");seenName=n;flags=f;seenOutput=out;*out=stream;return answer;}
    HRESULT STDMETHODCALLTYPE CreateStorage(const wchar_t*,DWORD,DWORD,DWORD,IStorage**) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE OpenStorage(const wchar_t*,IStorage*,DWORD,SNB,DWORD,IStorage**) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE CopyTo(DWORD,const IID*,SNB,IStorage*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE MoveElementTo(const wchar_t*,IStorage*,const wchar_t*,DWORD) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Commit(DWORD) override{return E_NOTIMPL;}HRESULT STDMETHODCALLTYPE Revert() override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE EnumElements(DWORD,void*,DWORD,IEnumSTATSTG**) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE DestroyElement(const wchar_t*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE RenameElement(const wchar_t*,const wchar_t*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE SetElementTimes(const wchar_t*,const FILETIME*,const FILETIME*,const FILETIME*) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE SetClass(REFCLSID) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE SetStateBits(DWORD,DWORD) override{return E_NOTIMPL;}
    HRESULT STDMETHODCALLTYPE Stat(STATSTG* s,DWORD flags) override{events.push_back(22);check(flags==STATFLAG_DEFAULT,"storage Stat requests the name");memset(s,0,sizeof(*s));if(name){size_t n=(wcslen(name)+1)*2;s->pwcsName=static_cast<wchar_t*>(CoTaskMemAlloc(n));memcpy(s->pwcsName,name,n);}return S_OK;}
};
template<class F> void fileError(int cause,long os,F call){bool caught=false;try{call();}catch(CException* e){check(!strcmp(e->GetRuntimeClass()->m_lpszClassName,"CFileException"),"stream failures throw CFileException");auto* f=static_cast<CFileException*>(e);caught=f->m_cause==cause&&f->m_lOsError==os&&f->m_strFileName.IsEmpty();e->Delete();}check(caught,"storage failure carries native cause, OS code and empty path");}
template<class F> void invalidArgument(F call){bool caught=false;try{call();}catch(Boundary e){caught=e.kind==2;}catch(CException* e){caught=!strcmp(e->GetRuntimeClass()->m_lpszClassName,"CInvalidArgException");e->Delete();}check(caught,"invalid buffer arguments use the native exception category");}
void basic() {
    ClientStream stream;File file(&stream);auto* p=file.p;
    check(events==std::vector<int>{13}&&stream.refs==1,"construction obtains Stat without AddRef");events.clear();
    check(!field<BOOL>(p,16)&&field<HANDLE>(p,8)==INVALID_HANDLE_VALUE&&!field<void*>(p,32)&&field<DWORD>(p,20)==0xa5a5a5a5,"stream constructor preserves the actual borrowed CFile base");
    check(p->GetRuntimeClass()->m_nObjectSize==56&&!strcmp(p->GetRuntimeClass()->m_lpszClassName,"COleStreamFile"),"actual compiler root identifies the stream runtime class");
    check(storageName(p).empty()&&events.empty(),"GetStorageName returns the saved name without calling Stat");
    char data[8];check(api->Read(p,data,8)==3&&data[0]=='r'&&stream.lastSize==8,"Read returns the COM byte count");
    api->Write(p,"hello",5);check(stream.lastSize==5,"Write accepts a successful short write");
    check(api->Seek(p,-7,19)==stream.position&&stream.lastOffset==ULONGLONG(-7)&&stream.lastFlags==19,"Seek forwards full offset and raw origin");
    check(api->GetPosition(p)==stream.position&&stream.lastOffset==0&&stream.lastFlags==STREAM_SEEK_CUR,"GetPosition calls Seek with CURRENT");
    check(api->GetLength(p)==stream.size&&stream.lastFlags==STATFLAG_NONAME,"GetLength preserves 64-bit size and suppresses the COM name allocation");
    api->SetLength(p,0x300000004ull);check(stream.lastSize==0x300000004ull,"SetLength preserves all 64 bits");
    api->LockRange(p,0x100000002ull,0x200000003ull);check(stream.lastOffset==0x100000002ull&&stream.lastSize==0x200000003ull&&stream.lastFlags==LOCK_EXCLUSIVE,"LockRange forwards native lock flags and widths");
    api->UnlockRange(p,4,5);check(stream.lastOffset==4&&stream.lastSize==5&&stream.lastFlags==LOCK_EXCLUSIVE,"UnlockRange mirrors LockRegion");
    stream.answer=S_FALSE;check(api->Read(p,data,1)==3,"S_FALSE is not a storage failure");api->Write(p,data,1);api->Flush(p);api->SetLength(p,2);
    stream.answer=STG_E_ACCESSDENIED;
    fileError(5,ERROR_ACCESS_DENIED,[&]{api->Read(p,data,1);});fileError(5,ERROR_ACCESS_DENIED,[&]{api->Write(p,data,1);});
    fileError(5,ERROR_ACCESS_DENIED,[&]{api->Seek(p,0,0);});fileError(5,ERROR_ACCESS_DENIED,[&]{api->GetPosition(p);});
    fileError(5,ERROR_ACCESS_DENIED,[&]{api->GetLength(p);});fileError(5,ERROR_ACCESS_DENIED,[&]{api->SetLength(p,1);});
    fileError(5,ERROR_ACCESS_DENIED,[&]{api->LockRange(p,1,2);});fileError(5,ERROR_ACCESS_DENIED,[&]{api->UnlockRange(p,1,2);});fileError(5,ERROR_ACCESS_DENIED,[&]{api->Flush(p);});
    stream.answer=S_OK;api->Detach(p);events.clear();check(api->Read(p,nullptr,0)==0,"zero count Read does not inspect a null stream");api->Write(p,nullptr,0);check(events.empty(),"zero count I/O does not call COM");
    fileError(1,E_INVALIDARG,[&]{api->Read(p,data,1);});fileError(1,E_INVALIDARG,[&]{api->Read(p,nullptr,1);});invalidArgument([&]{api->Write(p,nullptr,1);});invalidArgument([&]{api->Attach(p,nullptr);});
    api->Attach(p,&stream);fileError(1,E_INVALIDARG,[&]{api->Read(p,nullptr,1);});api->Close(p);check(!api->GetStream(p)&&stream.refs==0,"Close releases the current stream");
}
void ownership() {
    events.clear();ClientStream first,second;{File f(&first);events.clear();store<BOOL>(f.p,16,7);api->Attach(f.p,&second);check(!field<BOOL>(f.p,16)&&api->GetStream(f.p)==&second&&events.empty(),"Attach replaces the stream and clears ownership without releasing the old one");check(api->Detach(f.p)==&second&&events.empty(),"Detach transfers without releasing");}
    check(first.refs==1&&second.refs==1,"borrowed stream lifetimes remain with the caller");
    events.clear();{File f(&first);events.clear();}check(events.empty()&&first.refs==1,"borrowed destructor performs no commit or release");
    {File f(&first);store<BOOL>(f.p,16,7);events.clear();}check(events==std::vector<int>{9,3}&&first.refs==0,"owned destructor commits then releases");
    first.refs=1;{File f(&first);events.clear();first.answer=E_FAIL;fileError(1,E_FAIL,[&]{api->Close(f.p);});check(api->GetStream(f.p)==&first&&events==std::vector<int>{9},"failed Close preserves the stream for recovery");events.clear();api->Abort(f.p);check(events==std::vector<int>{10,3}&&!api->GetStream(f.p),"Abort ignores a failed Revert and releases");}first.answer=S_OK;
    first.refs=1;second.refs=1;{File f(&first);first.mutate=f.p;first.replacement=&second;events.clear();api->Abort(f.p);check(events==std::vector<int>{10,3}&&first.refs==1&&second.refs==0&&!api->GetStream(f.p),"Abort reloads the stream after the Revert callback");first.mutate=nullptr;}
    first.refs=1;{File f(&first);first.mutate=f.p;first.replacement=nullptr;first.detachOnCommit=true;events.clear();api->Close(f.p);check(events==std::vector<int>{9}&&first.refs==1&&!api->GetStream(f.p),"Close honors stream mutation during virtual Flush");first.mutate=nullptr;first.detachOnCommit=false;}
}
void namesAndStorage() {
    events.clear();ClientStream stream;stream.name=L"stream-file.bin";File f(&stream);wchar_t full[MAX_PATH];GetFullPathNameW(stream.name,MAX_PATH,full,nullptr);
    check(storageName(f.p)==full,"constructor saves the normalized name from Stat");stream.name=L"changed.bin";events.clear();check(storageName(f.p)==full&&events.empty(),"saved storage name is independent of later Stat results");
    auto detached=api->Detach(f.p);check(detached==&stream&&storageName(f.p)==full,"Detach preserves the storage name");api->Close(f.p);check(storageName(f.p).empty(),"Close clears saved name even without a stream");
    ClientStorage storage;storage.stream=&stream;storage.name=L"storage-file.bin";CFileException error(7,123);error.m_strFileName=L"preserved";
    events.clear();check(api->CreateStream(f.p,&storage,L"Contents",0x1234,&error)==TRUE&&events==std::vector<int>{22,20},"CreateStream requests storage Stat before creating");
    GetFullPathNameW(storage.name,MAX_PATH,full,nullptr);check(storageName(f.p)==full&&storage.flags==0x1234&&storage.seenName==L"Contents"&&storage.seenOutput==reinterpret_cast<IStream**>(reinterpret_cast<BYTE*>(f.p)+40)&&!field<BOOL>(f.p,16),"CreateStream forwards raw flags and its actual stream member without changing ownership");
    storage.answer=STG_E_FILENOTFOUND;events.clear();check(api->OpenStream(f.p,&storage,L"missing",77,&error)==FALSE&&events==std::vector<int>{21}&&error.m_cause==2&&error.m_lOsError==2&&!wcscmp(error.m_strFileName,L"preserved"),"OpenStream maps the storage HRESULT and preserves error filename");
    check(storageName(f.p)==full&&stream.refs==1,"OpenStream does not refresh name or release the previous stream");
    storage.answer=S_FALSE;check(api->OpenStream(f.p,&storage,L"",9,&error)==TRUE,"OpenStream accepts an empty name and successful nonzero HRESULT");events.clear();check(!api->CreateStream(f.p,nullptr,L"x",0,&error)&&!api->OpenStream(f.p,&storage,nullptr,0,&error)&&events.empty(),"invalid storage/name inputs return false without COM calls");
    api->Abort(f.p);
    std::wstring tooLong(260,L'x');stream.name=tooLong.c_str();fileError(3,-1,[&]{File invalid(&stream);});check(stream.refs==0,"failed constructor does not release the borrowed stream");
}
void preflightFailure(COleStreamFile*);
void statusAndDuplicate() {
    ClientStream stream,clone;File f(&stream);CFileStatus status;memset(&status,0x5a,sizeof(status));
    check(api->GetStatus(f.p,status)==TRUE&&status.m_size==3&&!status.m_attribute&&!status.m_szFullName[0]&&status.m_szFullName[1]==0x5a5a,"GetStatus exposes low-DWORD size and preserves the unused filename tail");
    auto asSeconds=[](const FILETIME& ft){ULARGE_INTEGER n;n.LowPart=ft.dwLowDateTime;n.HighPart=ft.dwHighDateTime;return (n.QuadPart-116444736000000000ull)/10000000ull;};
    check(status.m_mtime==asSeconds(stream.mtime)&&status.m_ctime==asSeconds(stream.ctime)&&status.m_atime==asSeconds(stream.atime),"status times are CTime epoch seconds, not FILETIME ticks");
    stream.name=L"named-stream";check(api->GetStatus(f.p,status)&&!wcscmp(status.m_szFullName,stream.name),"GetStatus uses the current Stat name");stream.name=nullptr;
    memset(&status,0x5a,sizeof(status));auto untouched=status;stream.answer=S_FALSE;check(!api->GetStatus(f.p,status)&&!memcmp(&status,&untouched,sizeof(status)),"non-S_OK status leaves the output untouched");stream.answer=S_OK;
    preflightFailure(f.p);
    stream.ctime=timeAt(1800);check(api->GetStatus(f.p,status)&&status.m_ctime==0,"pre-1900 status time follows the native zero rule");stream.ctime=timeAt(2021);
    stream.clone=&clone;store<BOOL>(f.p,16,7);events.clear();auto* copy=static_cast<COleStreamFile*>(api->Duplicate(f.p));check(copy&&api->GetStream(copy)==&clone&&field<BOOL>(copy,16)==7&&events==std::vector<int>{14,13},"Duplicate clones then constructs a real stream file and copies raw ownership");api->dtor(copy);::operator delete(copy);
    stream.answer=STG_E_LOCKVIOLATION;fileError(12,33,[&]{api->Duplicate(f.p);});stream.answer=S_OK;store<BOOL>(f.p,16,FALSE);api->Close(f.p);
}
void** imported(HMODULE image,const char* wanted){auto* b=reinterpret_cast<BYTE*>(image);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);auto* desc=reinterpret_cast<IMAGE_IMPORT_DESCRIPTOR*>(b+pe->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);for(;desc->Name;++desc){auto* n=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->OriginalFirstThunk);auto* v=reinterpret_cast<IMAGE_THUNK_DATA*>(b+desc->FirstThunk);for(;n->u1.AddressOfData;++n,++v)if(!IMAGE_SNAP_BY_ORDINAL(n->u1.Ordinal)){auto* name=reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(b+n->u1.AddressOfData);if(!strcmp(reinterpret_cast<char*>(name->Name),wanted))return reinterpret_cast<void**>(&v->u1.Function);}}return nullptr;}
struct ImportPatch {void** slot;void* old;DWORD protection;ImportPatch(HMODULE image,const char* name,void* fn){slot=imported(image,name);check(slot!=nullptr,name);if(!slot)std::abort();old=*slot;VirtualProtect(slot,8,PAGE_READWRITE,&protection);*slot=fn;}~ImportPatch(){*slot=old;DWORD ignored;VirtualProtect(slot,8,protection,&ignored);}};
BOOL WINAPI failLocalTime(const FILETIME*,FILETIME*){return FALSE;}
BOOL WINAPI failSystemTime(const FILETIME*,SYSTEMTIME*){return FALSE;}
void preflightFailure(COleStreamFile* file){
    HMODULE image=api->image?api->image:GetModuleHandleW(nullptr);
    for(bool local:{true,false}){
        CFileStatus status;memset(&status,0x5a,sizeof(status));auto unchanged=status;
        ImportPatch patch(image,local?"FileTimeToLocalFileTime":"FileTimeToSystemTime",local?reinterpret_cast<void*>(failLocalTime):reinterpret_cast<void*>(failSystemTime));
        check(!api->GetStatus(file,status)&&!memcmp(&status,&unchanged,sizeof(status)),"file-time preflight failure returns false before touching status");
    }
}
HRESULT memoryResult;COleStreamFile* memoryFile;IStream* memoryOutput=nullptr;
HRESULT WINAPI createMemory(HGLOBAL h,BOOL deleteOnRelease,IStream** out){events.push_back(30);check(!h&&deleteOnRelease==TRUE&&out==reinterpret_cast<IStream**>(reinterpret_cast<BYTE*>(memoryFile)+40),"CreateMemoryStream passes the actual output member and native ownership flag");if(memoryOutput)*out=memoryOutput;return memoryResult;}
API reference;
int expectedCause(long os){
    if(reference.image)return reference.osCause(os);
    // Fixed independent anchors for CI; the local DLL oracle checks every code.
    switch(os){case 0:return 0;case 2:case 6:return 2;case 3:return 3;case 4:return 4;
    case 5:return 5;case 16:return 7;case 25:return 9;case 32:return 11;case 33:return 12;
    case 38:return 14;case 39:case 112:return 13;case 995:return 10;
    case LONG_MIN:case LONG_MAX:case -1:case 65535:return 1;default:return -1;}
}
void errorMappingAndMemory() {
    for(long error=0;error<=1024;++error)if(expectedCause(error)>=0)check(api->osCause(error)==expectedCause(error),"complete Win32 file-error conversion matches native");
    for(long error:std::array<long,4>{LONG_MIN,LONG_MAX,-1,65535})check(api->osCause(error)==expectedCause(error),"out-of-range OS error conversion");
    api->throwOs(0,L"ignored");fileError(12,33,[&]{api->throwOs(33,nullptr);});
    ClientStream previous;File f(&previous);memoryFile=f.p;store<BOOL>(f.p,16,7);CFileException error(8,17);error.m_strFileName=L"untouched";
    {ImportPatch patch(api->image?api->image:GetModuleHandleW(nullptr),"CreateStreamOnHGlobal",reinterpret_cast<void*>(createMemory));
        for(int code=0;code<256;++code){if(expectedCause(code)<0)continue;memoryResult=HRESULT(0x80030000u+code);events.clear();check(!api->CreateMemoryStream(f.p,&error)&&error.m_cause==expectedCause(code)&&error.m_lOsError==code,"FACILITY_STORAGE low error codes map through the real OS converter");check(api->GetStream(f.p)==&previous&&previous.refs==1&&field<BOOL>(f.p,16)==7&&!wcscmp(error.m_strFileName,L"untouched"),"memory creation failure preserves untouched state and previous reference");}
        for(HRESULT result:{E_FAIL,E_INVALIDARG,E_OUTOFMEMORY,HRESULT(0x80030100u),HRESULT(0x80030101u),HRESULT(0x80030105u),HRESULT(0x80030106u)}){
            memoryResult=result;check(!api->CreateMemoryStream(f.p,&error)&&error.m_cause==((result==STG_E_INUSE||result==STG_E_SHAREREQUIRED)?11:1)&&error.m_lOsError==result,"extended storage and generic HRESULT failures preserve the full error code");}
        memoryResult=E_FAIL;check(!api->CreateMemoryStream(f.p,nullptr),"failed creation without an exception output returns false");memoryResult=S_FALSE;check(api->CreateMemoryStream(f.p,&error)==TRUE,"successful nonzero creation status is accepted");
    }
    store<BOOL>(f.p,16,FALSE);api->Detach(f.p);check(api->CreateMemoryStream(f.p,nullptr)&&api->GetStream(f.p)&&!field<BOOL>(f.p,16),"real memory stream creation preserves the borrowed-file flag");
    const char* message="stream data";api->Write(f.p,message,11);check(api->GetLength(f.p)==11,"real IStream write and size");api->Seek(f.p,0,0);char data[12]{};check(api->Read(f.p,data,12)==11&&!strcmp(data,message),"real IStream read handles S_FALSE at EOF");api->Close(f.p);
}
int deletedErrors;
struct DiagnosticError:CException {
    bool describe;explicit DiagnosticError(bool value):CException(FALSE),describe(value){}
    ~DiagnosticError() override{++deletedErrors;}
    int GetErrorMessage(wchar_t* out,UINT count,UINT* help) const override{check(count==512&&!help,"destructor description buffer and help argument");if(describe)wcscpy_s(out,count,L"stream callback");return describe;}
    int GetErrorMessage(wchar_t* out,UINT count,UINT* help) override{return static_cast<const DiagnosticError*>(this)->GetErrorMessage(out,count,help);}
};
struct CodePatch {
    void* address;std::array<BYTE,14> original;DWORD protection;
    CodePatch(void* entry,void* target):address(entry){memcpy(original.data(),entry,14);VirtualProtect(entry,14,PAGE_EXECUTE_READWRITE,&protection);BYTE code[14]{0x48,0xb8};memcpy(code+2,&target,8);code[10]=0xff;code[11]=0xe0;code[12]=code[13]=0x90;memcpy(entry,code,14);FlushInstructionCache(GetCurrentProcess(),entry,14);}
    ~CodePatch(){memcpy(address,original.data(),14);FlushInstructionCache(GetCurrentProcess(),address,14);DWORD ignored;VirtualProtect(address,14,protection,&ignored);}
};
void destructorErrors() {
    for(bool description:{false,true}) {
        ClientStream stream;alignas(8) BYTE storage[56];auto* file=api->ctor(storage,&stream);store<BOOL>(file,16,TRUE);stream.thrown=new DiagnosticError(description);diagnostic.clear();int before=deletedErrors;
        if(api->image){CodePatch patch(reinterpret_cast<BYTE*>(api->image)+0x1cec10,reinterpret_cast<void*>(impl__AfxMessageBox__YAHPEB_WII_Z));api->dtor(file);}else api->dtor(file);
        std::wstring expected=L"Exception thrown in destructor (D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\olestrm.cpp:73)";if(description)expected+=L"\nstream callback";
        check(diagnostic==expected&&deletedErrors==before+1&&stream.refs==1&&field<IStream*>(storage,40)==&stream,"destructor reports and unconditionally deletes framework exceptions while preserving a failed stream");
    }
}
void timeFailures() {
    ClientStream stream;File f(&stream);stream.ctime=timeAt(1960);CFileStatus status;memset(&status,0x5a,sizeof(status));bool caught=false;
    try{api->GetStatus(f.p,status);}catch(Boundary e){caught=e.kind==3&&e.code==E_FAIL;}catch(CException* e){caught=!strcmp(e->GetRuntimeClass()->m_lpszClassName,"COleException")&&field<long>(e,16)==E_FAIL;e->Delete();}
    check(caught&&status.m_mtime!=0x5a5a5a5a5a5a5a5aull&&status.m_ctime==0x5a5a5a5a5a5a5a5aull,"CTime range failure preserves the native partial-write order");api->Close(f.p);
}
void virtualFlush(COleStreamFile* p){events.push_back(40);store<IStream*>(p,40,nullptr);}
void virtualDispatch() {
    ClientStream stream;File f(&stream);void** original=field<void**>(f.p,0);std::array<void*,25> root;memcpy(root.data(),original,25*8);root[21]=reinterpret_cast<void*>(virtualFlush);store(f.p,0,root.data());events.clear();api->Close(f.p);check(events==std::vector<int>{40}&&stream.refs==1,"canonical Close honors the client virtual Flush override and its stream mutation");store(f.p,0,original);
}
int VerifyOleFileRuntimeClasses();
void run(const API& functions,const char* name){api=&functions;int before=failures;events.clear();std::printf("%s basic\n",name);basic();std::puts("ownership");ownership();std::puts("names");namesAndStorage();std::puts("status");statusAndDuplicate();std::puts("errors and memory");errorMappingAndMemory();std::puts("destructor errors");destructorErrors();std::puts("time failures");timeFailures();std::puts("virtual dispatch");virtualDispatch();std::printf("%s stream files: %d failures\n",name,failures-before);}
int main() {
    setvbuf(stdout,nullptr,_IONBF,0);CoInitialize(nullptr);reference=sourceAPI();wchar_t path[1024];HMODULE native=nullptr;
    if(GetEnvironmentVariableW(L"OPENMFC_NATIVE_FILES_DLL",path,1024)){
        native=LoadLibraryW(path);check(native!=nullptr,"load pinned native MFC");if(!native)return 1;auto* b=reinterpret_cast<BYTE*>(native);auto* pe=reinterpret_cast<IMAGE_NT_HEADERS*>(b+reinterpret_cast<IMAGE_DOS_HEADER*>(b)->e_lfanew);if(pe->FileHeader.TimeDateStamp!=0x442cba30||pe->OptionalHeader.SizeOfImage!=0x55c000){std::puts("Unsupported oracle DLL");return 1;}reference=nativeAPI(native);
    }
    try {check(VerifyOleFileRuntimeClasses()==0,"actual OLE file runtime-class chain");run(sourceAPI(),"Source");if(native)run(reference,"Native");}
    catch(Boundary e){std::printf("Unexpected boundary failure %d %lx\n",e.kind,e.code);return 1;}
    catch(CException* e){std::printf("Unexpected framework exception %s\n",e->GetRuntimeClass()->m_lpszClassName);e->Delete();return 1;}
    CoUninitialize();return failures?1:0;
}
