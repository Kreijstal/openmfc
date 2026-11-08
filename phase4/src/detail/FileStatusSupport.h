#pragma once
#include "openmfc/afxwin.h"
namespace openmfc { namespace detail { namespace files {
void FillFileError(CFileException*,const wchar_t*);
void BadFilePath(CFileException*,const wchar_t*);
BOOL FullFilePath(wchar_t*,const wchar_t*,CFileException*);
LONGLONG FileTimeValue(const FILETIME&);
void ReportDestructor(CException*,const wchar_t*,UINT);
} } }
