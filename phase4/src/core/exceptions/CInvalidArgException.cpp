// Actual resource-critical exception, shared factory and native RTTI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CInvalidArgException,CSimpleException)
CInvalidArgException::CInvalidArgException():CSimpleException() {}
CInvalidArgException::CInvalidArgException(BOOL owned):CSimpleException(owned) {m_nResourceID=0xf025;}
CInvalidArgException::CInvalidArgException(BOOL owned,UINT resource):CSimpleException(owned) {m_nResourceID=resource;}
CInvalidArgException::~CInvalidArgException() = default;
namespace {CInvalidArgException sharedException(FALSE);}
void AFXAPI AfxThrowInvalidArgException() {throw &sharedException;}
// Symbol: ?AfxThrowInvalidArgException@@YAXXZ
extern "C" void impl__AfxThrowInvalidArgException__YAXXZ() {AfxThrowInvalidArgException();}
// Symbol: ?GetThisClass@CInvalidArgException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CInvalidArgException__SAPEAUCRuntimeClass__XZ() {return CInvalidArgException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CInvalidArgException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CInvalidArgException__UEBAPEAUCRuntimeClass__XZ(const CInvalidArgException*) {return CInvalidArgException::GetThisClass();}
