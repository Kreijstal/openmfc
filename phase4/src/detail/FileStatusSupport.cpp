#define OPENMFC_APPCORE_IMPL
#include "FileStatusSupport.h"
#include <shlwapi.h>
#include <ctime>
#include <cerrno>
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long);
extern "C" void impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
extern "C" void impl__AfxGetRoot__YAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t*,CString&);
namespace openmfc { namespace detail { namespace files {
void FillFileError(CFileException* error, const wchar_t* name) {
    if (!error) return;
    error->m_lOsError = GetLastError();
    error->m_cause = impl__OsErrorToException_CFileException__SAHJ_Z(error->m_lOsError);
    error->m_strFileName = name;
}
void BadFilePath(CFileException* error, const wchar_t* name) {
    if (!error) return;
    error->m_cause = CFileException::badPath;
    if (name) error->m_strFileName = name;
}
BOOL FullFilePath(wchar_t* output, const wchar_t* name, CFileException* error) {
    if (!output || !name) AfxThrowInvalidArgException();
    wchar_t* leaf;
    DWORD size = GetFullPathNameW(name,MAX_PATH,output,&leaf);
    if (!size) {
        wcsncpy_s(output,MAX_PATH,name,_TRUNCATE);
        FillFileError(error,name);
        return FALSE;
    }
    if (size >= MAX_PATH) { BadFilePath(error,name); return FALSE; }
    CString root;
    impl__AfxGetRoot__YAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(output,root);
    if (!PathIsUNCW(root.GetString())) {
        DWORD maxComponent, flags;
        if (!GetVolumeInformationW(root.GetString(),nullptr,0,nullptr,&maxComponent,&flags,nullptr,0)) {
            FillFileError(error,name);
            return FALSE;
        }
        if (!(flags & FILE_CASE_PRESERVED_NAMES)) CharUpperW(output);
        if (!(flags & FILE_UNICODE_ON_DISK)) {
            WIN32_FIND_DATAW data;
            HANDLE found = FindFirstFileW(name,&data);
            if (found != INVALID_HANDLE_VALUE) {
                FindClose(found);
                if (!leaf || leaf <= output) { FillFileError(error,name); return FALSE; }
                if (leaf-output+wcslen(data.cFileName) >= MAX_PATH) { BadFilePath(error,name); return FALSE; }
                wcscpy_s(leaf,MAX_PATH-(leaf-output),data.cFileName);
            }
        }
    }
    return TRUE;
}
LONGLONG FileTimeValue(const FILETIME& value) {
    SYSTEMTIME utc,local;
    if (!FileTimeToSystemTime(&value,&utc) || !SystemTimeToTzSpecificLocalTime(nullptr,&utc,&local)) {
        impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    }
    if (local.wYear<1900) return 0;
    const bool dateValid = local.wYear>=1970 || (local.wYear==1969 && local.wMonth==12 && local.wDay==31);
    if (!dateValid || local.wMonth<1 || local.wMonth>12 || local.wDay<1 || local.wDay>31 ||
        local.wHour>23 || local.wMinute>59 || local.wSecond>59) impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
    tm parts{};
    parts.tm_year=local.wYear-1900;parts.tm_mon=local.wMonth-1;parts.tm_mday=local.wDay;
    parts.tm_hour=local.wHour;parts.tm_min=local.wMinute;parts.tm_sec=local.wSecond;parts.tm_isdst=-1;
    LONGLONG time=_mktime64(&parts);
    if (time==-1) impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    return time;
}
void ReportDestructor(CException* error,const wchar_t* file,UINT line) {
    wchar_t text[512];CString message;
    if (error->GetErrorMessage(text,512,nullptr))
        message.Format(L"%ls (%ls:%d)\n%ls",L"Exception thrown in destructor",file,line,text);
    else message.Format(L"%ls (%ls:%d)",L"Exception thrown in destructor",file,line);
    impl__AfxMessageBox__YAHPEB_WII_Z(message,0,0);
    delete error;
}
} } }

extern "C" void impl__ThrowOsError_CFileException__SAXJPEB_W_Z(long,const wchar_t*);
namespace {
// The external ATL transaction manager starts with HANDLE and fallback BOOL.
// Reload HANDLE after loader callbacks, as the native inline ATL methods do.
HANDLE TransactionHandle(ATL::CAtlTransactionManager* manager) {
    HANDLE value;memcpy(&value,manager,sizeof(value));return value;
}
BOOL TransactionFallback(ATL::CAtlTransactionManager* manager) {
    BOOL value;memcpy(&value,reinterpret_cast<BYTE*>(manager)+8,sizeof(value));return value;
}
template<class T> T TransactionProcedure(const char* name) {
    auto module=GetModuleHandleW(L"kernel32.dll");
    return module?reinterpret_cast<T>(GetProcAddress(module,name)):nullptr;
}
void CheckedStringResult(int error) {
    if (!error || error==STRUNCATE) return;
    if (error==ENOMEM) AfxThrowMemoryException();
    AfxThrowInvalidArgException();
}
}
namespace openmfc { namespace detail { namespace files {
HANDLE OpenTransactionFile(ATL::CAtlTransactionManager* manager,const wchar_t* name,DWORD access,DWORD share,
                           SECURITY_ATTRIBUTES* security,DWORD create,DWORD flags) {
    if (!TransactionHandle(manager)) return TransactionFallback(manager)?CreateFileW(name,access,share,security,create,flags,nullptr):INVALID_HANDLE_VALUE;
    using Call=HANDLE (WINAPI*)(LPCWSTR,DWORD,DWORD,LPSECURITY_ATTRIBUTES,DWORD,DWORD,HANDLE,HANDLE,PUSHORT,PVOID);
    auto call=TransactionProcedure<Call>("CreateFileTransactedW");
    return call?call(name,access,share,security,create,flags,nullptr,TransactionHandle(manager),nullptr,nullptr):INVALID_HANDLE_VALUE;
}
BOOL FileAttributes(const wchar_t* name,WIN32_FILE_ATTRIBUTE_DATA* data,ATL::CAtlTransactionManager* manager) {
    if (!manager || (!TransactionHandle(manager)&&TransactionFallback(manager)))
        return GetFileAttributesExW(name,GetFileExInfoStandard,data);
    if (!TransactionHandle(manager)) return FALSE;
    using Call=BOOL (WINAPI*)(LPCWSTR,GET_FILEEX_INFO_LEVELS,LPVOID,HANDLE);
    auto call=TransactionProcedure<Call>("GetFileAttributesTransactedW");
    return call?call(name,GetFileExInfoStandard,data,TransactionHandle(manager)):FALSE;
}
DWORD FileAttributes(const wchar_t* name,ATL::CAtlTransactionManager* manager) {
    if (!manager) return GetFileAttributesW(name);
    WIN32_FILE_ATTRIBUTE_DATA data;
    return FileAttributes(name,&data,manager)?data.dwFileAttributes:0;
}
BOOL ChangeAttributes(const wchar_t* name,DWORD attributes,ATL::CAtlTransactionManager* manager) {
    if (!manager || (!TransactionHandle(manager)&&TransactionFallback(manager))) return SetFileAttributesW(name,attributes);
    if (!TransactionHandle(manager)) return FALSE;
    using Call=BOOL (WINAPI*)(LPCWSTR,DWORD,HANDLE);
    auto call=TransactionProcedure<Call>("SetFileAttributesTransactedW");
    return call?call(name,attributes,TransactionHandle(manager)):FALSE;
}
BOOL MovePath(const wchar_t* oldName,const wchar_t* newName,ATL::CAtlTransactionManager* manager) {
    if (!manager || (!TransactionHandle(manager)&&TransactionFallback(manager))) return MoveFileW(oldName,newName);
    if (!TransactionHandle(manager)) return FALSE;
    using Call=BOOL (WINAPI*)(LPCWSTR,LPCWSTR,LPPROGRESS_ROUTINE,LPVOID,DWORD,HANDLE);
    auto call=TransactionProcedure<Call>("MoveFileTransactedW");
    return call?call(oldName,newName,nullptr,nullptr,MOVEFILE_COPY_ALLOWED,TransactionHandle(manager)):FALSE;
}
BOOL DeletePath(const wchar_t* name,ATL::CAtlTransactionManager* manager) {
    if (!manager || (!TransactionHandle(manager)&&TransactionFallback(manager))) return DeleteFileW(name);
    if (!TransactionHandle(manager)) return FALSE;
    using Call=BOOL (WINAPI*)(LPCWSTR,HANDLE);
    auto call=TransactionProcedure<Call>("DeleteFileTransactedW");
    return call?call(name,TransactionHandle(manager)):FALSE;
}
LONGLONG StatusTime(const FILETIME& time) {
    FILETIME local;SYSTEMTIME system;
    return FileTimeToLocalFileTime(&time,&local)&&FileTimeToSystemTime(&local,&system)?FileTimeValue(time):0;
}
void TimeToFileTime(const LONGLONG* time,FILETIME* output) {
    if (!output) AfxThrowInvalidArgException();
    // Native CTime field accessors each call _localtime64_s independently.
    // Keep all six calls, including their failure defaults and input reloads.
    tm parts;SYSTEMTIME local;
    local.wYear=_localtime64_s(&parts,time)?0:WORD(parts.tm_year+1900);
    local.wMonth=_localtime64_s(&parts,time)?0:WORD(parts.tm_mon+1);
    local.wDay=_localtime64_s(&parts,time)?0:WORD(parts.tm_mday);
    local.wHour=_localtime64_s(&parts,time)?WORD(-1):WORD(parts.tm_hour);
    local.wMinute=_localtime64_s(&parts,time)?WORD(-1):WORD(parts.tm_min);
    local.wSecond=_localtime64_s(&parts,time)?WORD(-1):WORD(parts.tm_sec);
    local.wMilliseconds=0;
    FILETIME intermediate;
    if (!SystemTimeToFileTime(&local,&intermediate)) impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),nullptr);
    if (!LocalFileTimeToFileTime(&intermediate,output)) impl__ThrowOsError_CFileException__SAXJPEB_W_Z(GetLastError(),nullptr);
}
} } }
// Symbol: ?AfxGetFileName@@YAIPEB_WPEA_WI@Z
extern "C" UINT impl__AfxGetFileName__YAIPEB_WPEA_WI_Z(const wchar_t* path,wchar_t* output,UINT size) {
    if (!path) AfxThrowInvalidArgException();
    const wchar_t* name=PathFindFileNameW(path);
    if (!output) return UINT(wcslen(name)+1);
    CheckedStringResult(wcsncpy_s(output,size,name,_TRUNCATE));
    return 0;
}
// Symbol: ?AfxGetFileTitle@@YAIPEB_WPEA_WI@Z
extern "C" UINT impl__AfxGetFileTitle__YAIPEB_WPEA_WI_Z(const wchar_t* path,wchar_t* output,UINT size) {
    wchar_t temporary[MAX_PATH];
    if (!output) size=MAX_PATH;
    auto* buffer=output?output:temporary;
    if (::GetFileTitleW(path,buffer,WORD(size))) return impl__AfxGetFileName__YAIPEB_WPEA_WI_Z(path,output,size);
    return output?0:UINT(wcslen(buffer)+1);
}
// Symbol: ?AfxTimeToFileTime@@YAXAEBVCTime@ATL@@PEAU_FILETIME@@@Z
extern "C" void impl__AfxTimeToFileTime__YAXAEBVCTime_ATL__PEAU_FILETIME___Z(const LONGLONG* time,FILETIME* output) {
    openmfc::detail::files::TimeToFileTime(time,output);
}

// Symbol: ?AfxFullPath@@YAHPEA_WPEB_W@Z
extern "C" BOOL impl__AfxFullPath__YAHPEA_WPEB_W_Z(wchar_t* output,const wchar_t* input) {
    return openmfc::detail::files::FullFilePath(output,input,nullptr);
}
// Symbol: ?AfxGetRoot@@YAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void impl__AfxGetRoot__YAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* path,CString& output) {
    auto* buffer=output.GetBuffer(MAX_PATH);
    // The native helper clears MAX_PATH bytes, then copies MAX_PATH WCHARs.
    memset(buffer,0,MAX_PATH);
    CheckedStringResult(wcsncpy_s(buffer,MAX_PATH,path,_TRUNCATE));
    PathStripToRootW(buffer);
    output.ReleaseBuffer();
}
