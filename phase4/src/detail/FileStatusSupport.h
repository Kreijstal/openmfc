#pragma once
#include "openmfc/afxwin.h"
namespace openmfc { namespace detail { namespace files {
void FillFileError(CFileException*,const wchar_t*);
void BadFilePath(CFileException*,const wchar_t*);
BOOL FullFilePath(wchar_t*,const wchar_t*,CFileException*);
HANDLE OpenTransactionFile(ATL::CAtlTransactionManager*,const wchar_t*,DWORD,DWORD,SECURITY_ATTRIBUTES*,DWORD,DWORD);
BOOL FileAttributes(const wchar_t*,WIN32_FILE_ATTRIBUTE_DATA*,ATL::CAtlTransactionManager*);
DWORD FileAttributes(const wchar_t*,ATL::CAtlTransactionManager*);
BOOL ChangeAttributes(const wchar_t*,DWORD,ATL::CAtlTransactionManager*);
BOOL MovePath(const wchar_t*,const wchar_t*,ATL::CAtlTransactionManager*);
BOOL DeletePath(const wchar_t*,ATL::CAtlTransactionManager*);
LONGLONG StatusTime(const FILETIME&);
void TimeToFileTime(const LONGLONG*,FILETIME*);
LONGLONG FileTimeValue(const FILETIME&);
void ReportDestructor(CException*,const wchar_t*,UINT);
} } }
