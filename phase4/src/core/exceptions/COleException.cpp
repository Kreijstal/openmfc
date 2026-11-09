// Native OLE exception object, system message lookup and typed pointer throw.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxdisp.h"
#include <cerrno>
#include <cwchar>
#include <new>
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
IMPLEMENT_DYNAMIC(COleException,CException)
static_assert(sizeof(COleException)==24 && offsetof(COleException,m_sc)==16,"native OLE exception storage");
COleException::COleException():CException(TRUE),m_sc(0) {}
COleException::~COleException() = default;
int COleException::GetErrorMessage(wchar_t* output,UINT capacity,UINT* help) const {
    if(help)*help=0;
    wchar_t* message;
    if(!::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,nullptr,DWORD(m_sc),
        MAKELANGID(LANG_NEUTRAL,SUBLANG_SYS_DEFAULT),reinterpret_cast<wchar_t*>(&message),0,nullptr)) {
        *output=0;return FALSE;
    }
    int result=wcsncpy_s(output,capacity,message,_TRUNCATE);
    if(result && result!=STRUNCATE) {
        if(result==ENOMEM)AfxThrowMemoryException();
        else AfxThrowInvalidArgException();
    }
    ::LocalFree(message);
    return TRUE;
}
void AFXAPI AfxThrowOleException(LONG code) {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(COleException));
    COleException* error=nullptr;
    if(storage) {
        try {error=::new(storage) COleException;}
        catch(...){impl___3_YAXPEAX_Z(storage);throw;}
    }
    error->m_sc=code;
    throw error;
}
// Symbol: ?GetErrorMessage@COleException@@UEBAHPEA_WIPEAI@Z
extern "C" int impl__GetErrorMessage_COleException__UEBAHPEA_WIPEAI_Z(const COleException* self,wchar_t* output,UINT capacity,UINT* help) {
    return self->COleException::GetErrorMessage(output,capacity,help);
}
// Symbol: ?AfxThrowOleException@@YAXJ@Z
extern "C" void impl__AfxThrowOleException__YAXJ_Z(LONG code) {AfxThrowOleException(code);}
// Symbol: ?GetThisClass@COleException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_COleException__SAPEAUCRuntimeClass__XZ() {return COleException::GetThisClass();}
// Symbol: ?GetRuntimeClass@COleException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_COleException__UEBAPEAUCRuntimeClass__XZ(const COleException*) {return COleException::GetThisClass();}
