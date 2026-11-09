// Actual resource-critical exception, shared factory and native RTTI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CMemoryException,CSimpleException)
CMemoryException::CMemoryException():CSimpleException() {}
CMemoryException::CMemoryException(BOOL owned):CSimpleException(owned) {m_nResourceID=0xf023;}
CMemoryException::CMemoryException(BOOL owned,UINT resource):CSimpleException(owned) {m_nResourceID=resource;}
CMemoryException::~CMemoryException() = default;
namespace {CMemoryException sharedException(FALSE);}
// Symbol: ?AfxThrowMemoryException@@YAXXZ
extern "C" void impl__AfxThrowMemoryException__YAXXZ() {throw &sharedException;}
// Symbol: ?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ() {return CMemoryException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CMemoryException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CMemoryException__UEBAPEAUCRuntimeClass__XZ(const CMemoryException*) {return CMemoryException::GetThisClass();}
