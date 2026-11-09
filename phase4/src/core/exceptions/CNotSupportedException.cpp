// Actual resource-critical exception, shared factory and native RTTI.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CNotSupportedException,CSimpleException)
CNotSupportedException::CNotSupportedException():CSimpleException() {}
CNotSupportedException::CNotSupportedException(BOOL owned):CSimpleException(owned) {m_nResourceID=0xf021;}
CNotSupportedException::CNotSupportedException(BOOL owned,UINT resource):CSimpleException(owned) {m_nResourceID=resource;}
CNotSupportedException::~CNotSupportedException() = default;
namespace {CNotSupportedException sharedException(FALSE);}
void AFXAPI AfxThrowNotSupportedException() {throw &sharedException;}
// Symbol: ?AfxThrowNotSupportedException@@YAXXZ
extern "C" void impl__AfxThrowNotSupportedException__YAXXZ() {AfxThrowNotSupportedException();}
// Symbol: ?GetThisClass@CNotSupportedException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CNotSupportedException__SAPEAUCRuntimeClass__XZ() {return CNotSupportedException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CNotSupportedException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CNotSupportedException__UEBAPEAUCRuntimeClass__XZ(const CNotSupportedException*) {return CNotSupportedException::GetThisClass();}
