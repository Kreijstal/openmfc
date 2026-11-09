// Actual resource-critical exception, shared factory and native RTTI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CResourceException,CSimpleException)
CResourceException::CResourceException():CSimpleException() {}
CResourceException::CResourceException(BOOL owned):CSimpleException(owned) {m_nResourceID=0xf022;}
CResourceException::CResourceException(BOOL owned,UINT resource):CSimpleException(owned) {m_nResourceID=resource;}
CResourceException::~CResourceException() = default;
namespace {CResourceException sharedException(FALSE);}
void AFXAPI AfxThrowResourceException() {throw &sharedException;}
// Symbol: ?AfxThrowResourceException@@YAXXZ
extern "C" void impl__AfxThrowResourceException__YAXXZ() {AfxThrowResourceException();}
// Symbol: ?GetThisClass@CResourceException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CResourceException__SAPEAUCRuntimeClass__XZ() {return CResourceException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CResourceException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CResourceException__UEBAPEAUCRuntimeClass__XZ(const CResourceException*) {return CResourceException::GetThisClass();}
