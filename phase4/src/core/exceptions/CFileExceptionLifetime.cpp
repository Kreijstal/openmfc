// Actual CFileException lifetime, runtime metadata and native object fields.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CFileException,CException)
static_assert(sizeof(CFileException)==32 && offsetof(CFileException,m_cause)==16 &&
    offsetof(CFileException,m_lOsError)==20 && offsetof(CFileException,m_strFileName)==24,"native file exception fields");
CFileException::CFileException(int cause,LONG error,const wchar_t* name):CException(TRUE) {
    m_cause=cause;m_lOsError=error;m_strFileName=name;
}
CFileException::~CFileException() = default;
// Symbol: ?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CFileException__SAPEAUCRuntimeClass__XZ() {return CFileException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CFileException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CFileException__UEBAPEAUCRuntimeClass__XZ(const CFileException*) {return CFileException::GetThisClass();}
