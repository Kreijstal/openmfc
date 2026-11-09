// Actual CArchiveException lifetime, runtime metadata and native object fields.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CArchiveException,CException)
static_assert(sizeof(CArchiveException)==32 && offsetof(CArchiveException,m_cause)==16 &&
    offsetof(CArchiveException,m_strFileName)==24,"native archive exception fields");
CArchiveException::CArchiveException(int cause,const wchar_t* name):CException(TRUE) {
    m_cause=cause;m_strFileName=name;
}
CArchiveException::~CArchiveException() = default;
// Symbol: ?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CArchiveException__SAPEAUCRuntimeClass__XZ() {return CArchiveException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CArchiveException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CArchiveException__UEBAPEAUCRuntimeClass__XZ(const CArchiveException*) {return CArchiveException::GetThisClass();}
