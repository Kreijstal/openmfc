// Actual resource-critical exception, shared factory and native RTTI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CUserException,CSimpleException)
CUserException::CUserException():CSimpleException() {}
CUserException::CUserException(BOOL owned):CSimpleException(owned) {m_nResourceID=0xf024;}
CUserException::CUserException(BOOL owned,UINT resource):CSimpleException(owned) {m_nResourceID=resource;}
CUserException::~CUserException() = default;
namespace {CUserException sharedException(FALSE);}
void AFXAPI AfxThrowUserException() {throw &sharedException;}
// Symbol: ?AfxThrowUserException@@YAXXZ
extern "C" void impl__AfxThrowUserException__YAXXZ() {AfxThrowUserException();}
// Symbol: ?GetThisClass@CUserException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CUserException__SAPEAUCRuntimeClass__XZ() {return CUserException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CUserException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CUserException__UEBAPEAUCRuntimeClass__XZ(const CUserException*) {return CUserException::GetThisClass();}
