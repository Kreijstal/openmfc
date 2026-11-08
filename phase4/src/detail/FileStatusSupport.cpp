#define OPENMFC_APPCORE_IMPL
#include "FileStatusSupport.h"
#include <shlwapi.h>
#include <ctime>
extern "C" int impl__OsErrorToException_CFileException__SAHJ_Z(long);
extern "C" void impl__AfxThrowOleException__YAXJ_Z(long);
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
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
    wchar_t* buffer=root.GetBuffer(MAX_PATH);
    memset(buffer,0,MAX_PATH);
    wcsncpy_s(buffer,MAX_PATH,output,_TRUNCATE);
    PathStripToRootW(buffer);
    root.ReleaseBuffer();
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
