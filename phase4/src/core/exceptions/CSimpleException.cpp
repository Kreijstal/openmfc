// Native Unicode resource-critical exception message cache.
#define OPENMFC_APPCORE_IMPL
#include "detail/CsimpleexceptionRttiSupport.h"
#include <cwchar>
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
void AFXAPI AfxThrowMemoryException() {impl__AfxThrowMemoryException__YAXXZ();}
CSimpleException::CSimpleException() : CSimpleException(TRUE) {}
CSimpleException::CSimpleException(BOOL autoDelete) : CException(autoDelete),m_bAutoDelete(autoDelete),m_bInitialized(FALSE),m_bLoaded(FALSE),m_szMessage{},m_nResourceID(0) {}
CSimpleException::~CSimpleException() = default;
void CSimpleException::InitString() {
    m_bInitialized=TRUE;
    auto module=static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(m_nResourceID));
    m_bLoaded=module && ::LoadStringW(module,m_nResourceID,m_szMessage,128)!=0;
}
int CSimpleException::GetErrorMessage(wchar_t* out,UINT capacity,UINT* help) const {
    if(!out || !capacity)return FALSE;
    if(help)*help=0;
    if(!m_bInitialized)const_cast<CSimpleException*>(this)->InitString();
    if(m_bLoaded) {
        std::wcsncpy(out,m_szMessage,capacity-1);out[capacity-1]=L'\0';
    } else out[0]=L'\0';
    return m_bLoaded;
}
// Symbol: ?GetErrorMessage@CSimpleException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_CSimpleException__UEBAHPEA_WIPEAI_Z(const CSimpleException* self,wchar_t* out,UINT capacity,UINT* help) { return self->CSimpleException::GetErrorMessage(out,capacity,help); }
// Symbol: ?GetRuntimeClass@CSimpleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSimpleException__UEBAPEAUCRuntimeClass__XZ(const CSimpleException*) { return CSimpleException::GetThisClass(); }
// Symbol: ?GetThisClass@CSimpleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSimpleException__SAPEAUCRuntimeClass__XZ() { return CSimpleException::GetThisClass(); }
// Symbol: ?InitString@CSimpleException@@QEAAXXZ
extern "C" void MS_ABI impl__InitString_CSimpleException__QEAAXXZ(CSimpleException* self) { self->InitString(); }
