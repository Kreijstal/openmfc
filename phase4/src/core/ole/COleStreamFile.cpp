// Native IStream-backed CFile behavior and ownership.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include "detail/FileStatusSupport.h"
#include <new>
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long);
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int,long,const wchar_t*);
namespace {
void FillStorageError(CFileException& error,HRESULT result) {
    long os=result;
    int cause;
    if (FAILED(result) && (static_cast<ULONG>(result)&0x1fff0000)==0x30000 && HRESULT_CODE(result)<256) {
        os=HRESULT_CODE(result);
        cause=impl__OsErrorToException_CFileException__SAHJ_Z(os);
    } else {
        cause=(result==STG_E_INUSE || result==STG_E_SHAREREQUIRED) ? CFileException::sharingViolation : CFileException::genericException;
    }
    error.m_cause=cause;
    error.m_lOsError=os;
}
void ThrowStorageError(HRESULT result) {
    if (FAILED(result)) {
        CFileException error;
        FillStorageError(error,result);
        impl__AfxThrowFileException__YAXHJPEB_W_Z(error.m_cause,error.m_lOsError,nullptr);
    }
}
void ReleaseStream(IStream*& stream) {
    if (stream) { stream->Release(); stream=nullptr; }
}
bool ValidFileTime(const FILETIME& time) {
    FILETIME local;SYSTEMTIME parts;
    return FileTimeToLocalFileTime(&time,&local) && FileTimeToSystemTime(&local,&parts);
}
}
using namespace openmfc::detail::files;
COleStreamFile::COleStreamFile(IStream* stream) : m_lpStream(stream) {
    m_strStorageName.Empty();
    if (m_lpStream) {
        STATSTG status;
        if (m_lpStream->Stat(&status,STATFLAG_DEFAULT)==S_OK && status.pwcsName) {
            CString name(status.pwcsName);
            if (name.GetLength()>=MAX_PATH) {
                CoTaskMemFree(status.pwcsName);
                impl__AfxThrowFileException__YAXHJPEB_W_Z(CFileException::badPath,-1,nullptr);
            }
            wchar_t full[MAX_PATH];
            FullFilePath(full,name,nullptr);
            CoTaskMemFree(status.pwcsName);
            m_strStorageName=full;
        }
    }
}
COleStreamFile::~COleStreamFile() {
    try { if (m_lpStream && m_bCloseOnDelete) COleStreamFile::Close(); }
    catch(CException* error) {
        ReportDestructor(error,L"D:\\a\\_work\\1\\s\\src\\vctools\\VC7Libs\\Ship\\ATLMFC\\Src\\MFC\\olestrm.cpp",73);
    }
}
IStream* COleStreamFile::GetStream() const { return m_lpStream; }
void COleStreamFile::Attach(IStream* stream) {
    if (!stream) AfxThrowInvalidArgException();
    m_lpStream=stream;
    m_bCloseOnDelete=FALSE;
}
IStream* COleStreamFile::Detach() {
    IStream* stream=m_lpStream;m_lpStream=nullptr;return stream;
}
BOOL COleStreamFile::OpenStream(IStorage* storage,const wchar_t* name,DWORD flags,CFileException* error) {
    if (!storage || !name) return FALSE;
    CString streamName(name);
    HRESULT result=storage->OpenStream(streamName,nullptr,flags,0,&m_lpStream);
    if (FAILED(result) && error) FillStorageError(*error,result);
    return SUCCEEDED(result);
}
BOOL COleStreamFile::CreateStream(IStorage* storage,const wchar_t* name,DWORD flags,CFileException* error) {
    if (!storage || !name) return FALSE;
    STATSTG status;
    if (storage->Stat(&status,STATFLAG_DEFAULT)==S_OK && status.pwcsName) {
        CString storageName(status.pwcsName);
        wchar_t full[MAX_PATH];
        FullFilePath(full,storageName,nullptr);
        CoTaskMemFree(status.pwcsName);
        m_strStorageName=full;
    }
    CString streamName(name);
    HRESULT result=storage->CreateStream(streamName,flags,0,0,&m_lpStream);
    if (FAILED(result) && error) FillStorageError(*error,result);
    return SUCCEEDED(result);
}
BOOL COleStreamFile::CreateMemoryStream(CFileException* error) {
    HRESULT result=CreateStreamOnHGlobal(nullptr,TRUE,&m_lpStream);
    if (FAILED(result) && error) FillStorageError(*error,result);
    return SUCCEEDED(result);
}
CFile* COleStreamFile::Duplicate() const {
    IStream* clone;
    HRESULT result=m_lpStream->Clone(&clone);
    if (FAILED(result)) ThrowStorageError(result);
    auto* copy=new COleStreamFile(clone);
    copy->m_bCloseOnDelete=m_bCloseOnDelete;
    return copy;
}
ULONGLONG COleStreamFile::GetPosition() const {
    ULARGE_INTEGER position;
    HRESULT result=m_lpStream->Seek(LARGE_INTEGER{},STREAM_SEEK_CUR,&position);
    if (result!=S_OK) ThrowStorageError(result);
    return position.QuadPart;
}
ULONGLONG COleStreamFile::Seek(LONGLONG offset,UINT origin) {
    LARGE_INTEGER move;move.QuadPart=offset;
    ULARGE_INTEGER position;
    HRESULT result=m_lpStream->Seek(move,origin,&position);
    if (result!=S_OK) ThrowStorageError(result);
    return position.QuadPart;
}
void COleStreamFile::SetLength(ULONGLONG length) {
    ULARGE_INTEGER size;size.QuadPart=length;
    HRESULT result=m_lpStream->SetSize(size);
    if (result!=S_OK) ThrowStorageError(result);
}
ULONGLONG COleStreamFile::GetLength() const {
    STATSTG status;
    HRESULT result=m_lpStream->Stat(&status,STATFLAG_NONAME);
    if (result!=S_OK) ThrowStorageError(result);
    return status.cbSize.QuadPart;
}
UINT COleStreamFile::Read(void* buffer,UINT count) {
    if (!count) return 0;
    ULONG read;
    HRESULT result=m_lpStream && buffer ? m_lpStream->Read(buffer,count,&read) : E_INVALIDARG;
    if (result!=S_OK) ThrowStorageError(result);
    return read;
}
void COleStreamFile::Write(const void* buffer,UINT count) {
    if (!count) return;
    if (!buffer) AfxThrowInvalidArgException();
    ULONG written;
    HRESULT result=m_lpStream->Write(buffer,count,&written);
    if (result!=S_OK) ThrowStorageError(result);
}
void COleStreamFile::LockRange(ULONGLONG offset,ULONGLONG count) {
    ULARGE_INTEGER position,size;position.QuadPart=offset;size.QuadPart=count;
    HRESULT result=m_lpStream->LockRegion(position,size,LOCK_EXCLUSIVE);
    if (result!=S_OK) ThrowStorageError(result);
}
void COleStreamFile::UnlockRange(ULONGLONG offset,ULONGLONG count) {
    ULARGE_INTEGER position,size;position.QuadPart=offset;size.QuadPart=count;
    HRESULT result=m_lpStream->UnlockRegion(position,size,LOCK_EXCLUSIVE);
    if (result!=S_OK) ThrowStorageError(result);
}
void COleStreamFile::Flush() {
    HRESULT result=m_lpStream->Commit(STGC_DEFAULT);
    if (result!=S_OK) ThrowStorageError(result);
}
void COleStreamFile::Close() {
    if (m_lpStream) { Flush(); ReleaseStream(m_lpStream); }
    m_strStorageName.Empty();
}
void COleStreamFile::Abort() {
    if (m_lpStream) { m_lpStream->Revert(); ReleaseStream(m_lpStream); }
    m_strStorageName.Empty();
}
int COleStreamFile::GetStatus(CFileStatus& status) const {
    STATSTG stream;
    if (m_lpStream->Stat(&stream,STATFLAG_DEFAULT)!=S_OK ||
        !ValidFileTime(stream.mtime) || !ValidFileTime(stream.ctime) || !ValidFileTime(stream.atime)) return FALSE;
    status.m_mtime=FileTimeValue(stream.mtime);
    status.m_ctime=FileTimeValue(stream.ctime);
    status.m_atime=FileTimeValue(stream.atime);
    // Native GetStatus exposes only the low DWORD; GetLength returns all 64 bits.
    status.m_size=stream.cbSize.LowPart;
    status.m_attribute=0;status.m_szFullName[0]=0;
    if (stream.pwcsName) {
        CString name(stream.pwcsName);
        wcsncpy_s(status.m_szFullName,MAX_PATH,name,_TRUNCATE);
        CoTaskMemFree(stream.pwcsName);
    }
    return TRUE;
}
const CString COleStreamFile::GetStorageName() const { return m_strStorageName; }
static CRuntimeClass* AFXAPI StreamFileBase() { return CFile::GetThisClass(); }
CRuntimeClass COleStreamFile::classCOleStreamFile={"COleStreamFile",sizeof(COleStreamFile),0xffff,nullptr,StreamFileBase,nullptr,nullptr};

// Symbol: ?Write@COleStreamFile@@UEAAXPEBXI@Z
extern "C" void impl__Write_COleStreamFile__UEAAXPEBXI_Z(COleStreamFile* p,const void* b,UINT n) { p->COleStreamFile::Write(b,n); }

// Symbol: ?UnlockRange@COleStreamFile@@UEAAX_K0@Z
extern "C" void impl__UnlockRange_COleStreamFile__UEAAX_K0_Z(COleStreamFile* p,ULONGLONG o,ULONGLONG n) { p->COleStreamFile::UnlockRange(o,n); }

// Symbol: ?SetLength@COleStreamFile@@UEAAX_K@Z
extern "C" void impl__SetLength_COleStreamFile__UEAAX_K_Z(COleStreamFile* p,ULONGLONG n) { p->COleStreamFile::SetLength(n); }

// Symbol: ?Seek@COleStreamFile@@UEAA_K_JI@Z
extern "C" ULONGLONG impl__Seek_COleStreamFile__UEAA_K_JI_Z(COleStreamFile* p,LONGLONG o,UINT f) { return p->COleStreamFile::Seek(o,f); }

// Symbol: ?Read@COleStreamFile@@UEAAIPEAXI@Z
extern "C" UINT impl__Read_COleStreamFile__UEAAIPEAXI_Z(COleStreamFile* p,void* b,UINT n) { return p->COleStreamFile::Read(b,n); }

// Symbol: ?OpenStream@COleStreamFile@@QEAAHPEAUIStorage@@PEB_WKPEAVCFileException@@@Z
extern "C" BOOL impl__OpenStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(COleStreamFile* p,IStorage* s,const wchar_t* n,DWORD f,CFileException* e) { return p->OpenStream(s,n,f,e); }

// Symbol: ?LockRange@COleStreamFile@@UEAAX_K0@Z
extern "C" void impl__LockRange_COleStreamFile__UEAAX_K0_Z(COleStreamFile* p,ULONGLONG o,ULONGLONG n) { p->COleStreamFile::LockRange(o,n); }

// Symbol: ?GetThisClass@COleStreamFile@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_COleStreamFile__SAPEAUCRuntimeClass__XZ() { return COleStreamFile::GetThisClass(); }

// Symbol: ?GetStream@COleStreamFile@@QEBAPEAUIStream@@XZ
extern "C" IStream* impl__GetStream_COleStreamFile__QEBAPEAUIStream__XZ(const COleStreamFile* p) { return p->GetStream(); }

// Symbol: ?GetStorageName@COleStreamFile@@UEBA?BV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void* impl__GetStorageName_COleStreamFile__UEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(const COleStreamFile* p,void* r) { new(r) CString(p->COleStreamFile::GetStorageName()); return r; }

// Symbol: ?GetStatus@COleStreamFile@@QEBAHAEAUCFileStatus@@@Z
extern "C" int impl__GetStatus_COleStreamFile__QEBAHAEAUCFileStatus___Z(const COleStreamFile* p,CFileStatus& s) { return p->GetStatus(s); }

// Symbol: ?GetRuntimeClass@COleStreamFile@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_COleStreamFile__UEBAPEAUCRuntimeClass__XZ(const COleStreamFile* p) { return COleStreamFile::GetThisClass(); }

// Symbol: ?GetPosition@COleStreamFile@@UEBA_KXZ
extern "C" ULONGLONG impl__GetPosition_COleStreamFile__UEBA_KXZ(const COleStreamFile* p) { return p->COleStreamFile::GetPosition(); }

// Symbol: ?GetLength@COleStreamFile@@UEBA_KXZ
extern "C" ULONGLONG impl__GetLength_COleStreamFile__UEBA_KXZ(const COleStreamFile* p) { return p->COleStreamFile::GetLength(); }

// Symbol: ?Flush@COleStreamFile@@UEAAXXZ
extern "C" void impl__Flush_COleStreamFile__UEAAXXZ(COleStreamFile* p) { p->COleStreamFile::Flush(); }

// Symbol: ?Duplicate@COleStreamFile@@UEBAPEAVCFile@@XZ
extern "C" CFile* impl__Duplicate_COleStreamFile__UEBAPEAVCFile__XZ(const COleStreamFile* p) { return p->COleStreamFile::Duplicate(); }

// Symbol: ?Detach@COleStreamFile@@QEAAPEAUIStream@@XZ
extern "C" IStream* impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ(COleStreamFile* p) { return p->Detach(); }

// Symbol: ?CreateStream@COleStreamFile@@QEAAHPEAUIStorage@@PEB_WKPEAVCFileException@@@Z
extern "C" BOOL impl__CreateStream_COleStreamFile__QEAAHPEAUIStorage__PEB_WKPEAVCFileException___Z(COleStreamFile* p,IStorage* s,const wchar_t* n,DWORD f,CFileException* e) { return p->CreateStream(s,n,f,e); }

// Symbol: ?CreateMemoryStream@COleStreamFile@@QEAAHPEAVCFileException@@@Z
extern "C" BOOL impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z(COleStreamFile* p,CFileException* e) { return p->CreateMemoryStream(e); }

// Symbol: ?Close@COleStreamFile@@UEAAXXZ
extern "C" void impl__Close_COleStreamFile__UEAAXXZ(COleStreamFile* p) { p->COleStreamFile::Close(); }

// Symbol: ?Attach@COleStreamFile@@QEAAXPEAUIStream@@@Z
extern "C" void impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(COleStreamFile* p,IStream* s) { p->Attach(s); }

// Symbol: ?Abort@COleStreamFile@@UEAAXXZ
extern "C" void impl__Abort_COleStreamFile__UEAAXXZ(COleStreamFile* p) { p->COleStreamFile::Abort(); }

// Symbol: ??1COleStreamFile@@UEAA@XZ
extern "C" void impl___1COleStreamFile__UEAA_XZ(COleStreamFile* p) { p->COleStreamFile::~COleStreamFile(); }

// Symbol: ??0COleStreamFile@@QEAA@PEAUIStream@@@Z
extern "C" COleStreamFile* impl___0COleStreamFile__QEAA_PEAUIStream___Z(void* p,IStream* s) { return new(p) COleStreamFile(s); }
