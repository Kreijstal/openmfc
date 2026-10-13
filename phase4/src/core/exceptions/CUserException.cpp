#define OPENMFC_APPCORE_IMPL
#include "detail/CsimpleexceptionRttiSupport.h"
IMPLEMENT_DYNAMIC(CUserException,CSimpleException)
CUserException::CUserException() : CUserException(TRUE) {}
CUserException::CUserException(BOOL autoDelete) : CUserException(autoDelete,0xf024) {}
CUserException::CUserException(BOOL autoDelete,UINT resource) : CSimpleException(autoDelete) { m_nResourceID=resource; }
CUserException::~CUserException() = default;
// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(const CUserException*) { return CUserException::GetThisClass(); }
// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ() { return CUserException::GetThisClass(); }
void AFXAPI AfxThrowUserException() {
    // Native MFC throws a shared resource-critical exception without allocating.
    static CUserException exception(FALSE);
    throw &exception;
}
// Symbol: ?AfxThrowUserException@@YAXXZ
extern "C" void MS_ABI impl__AfxThrowUserException__YAXXZ() { AfxThrowUserException(); }
