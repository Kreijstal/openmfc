// Native Unicode resource-critical exception message cache.
#define OPENMFC_APPCORE_IMPL
#include "detail/CsimpleexceptionRttiSupport.h"
#include <cwchar>
#include <cerrno>
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT,wchar_t*,UINT);
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
void AFXAPI AfxThrowMemoryException() {impl__AfxThrowMemoryException__YAXXZ();}
CSimpleException::CSimpleException() : m_bInitialized(FALSE),m_bLoaded(FALSE) {}
CSimpleException::CSimpleException(BOOL autoDelete) : CException(autoDelete),m_bInitialized(FALSE),m_bLoaded(FALSE) {}
CSimpleException::~CSimpleException() = default;
void CSimpleException::InitString() {
    m_bInitialized=TRUE;
    m_bLoaded=impl__AfxLoadString__YAHIPEA_WI_Z(m_nResourceID,m_szMessage,128)!=0;
}
int CSimpleException::GetErrorMessage(wchar_t* out,UINT capacity,UINT* help) const {
    if(!out || !capacity)return FALSE;
    if(help)*help=0;
    if(!m_bInitialized)const_cast<CSimpleException*>(this)->InitString();
    if(m_bLoaded) {
        int error=wcsncpy_s(out,capacity,m_szMessage,_TRUNCATE);
        if (error && error!=STRUNCATE) {
            if (error==ENOMEM) AfxThrowMemoryException();
            AfxThrowInvalidArgException();
        }
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
