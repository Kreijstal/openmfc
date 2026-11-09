// Native WinInet exception object, message lookup and typed pointer throw.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxinet.h"
#include "detail/WindowActivationSupport.h"
#include <cerrno>
#include <new>
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
IMPLEMENT_DYNAMIC(CInternetException,CException)
static_assert(sizeof(CInternetException)==32 && offsetof(CInternetException,m_dwError)==16 &&
    offsetof(CInternetException,m_dwContext)==24,"native Internet exception fields");

CInternetException::CInternetException(DWORD error):CException(TRUE),m_dwError(error) {}
CInternetException::~CInternetException() = default;
namespace {
void CopyMessage(wchar_t* output,UINT capacity,const wchar_t* text) {
    int result=wcsncpy_s(output,capacity,text,_TRUNCATE);
    if(result && result!=STRUNCATE) {
        if(result==ENOMEM)AfxThrowMemoryException();
        else AfxThrowInvalidArgException();
    }
}
}
int CInternetException::GetErrorMessage(wchar_t* output,UINT capacity,UINT* help) const {
    if(help)*help=0;
    HMODULE module=openmfc::detail::ContextLoadLibrary(L"WININET.DLL",nullptr,
        LOAD_LIBRARY_AS_IMAGE_RESOURCE|LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE);
    if(!module)module=openmfc::detail::ContextLoadLibrary(L"WININET.DLL",nullptr,LOAD_LIBRARY_AS_DATAFILE);
    wchar_t* message;
    if((!module || !::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_HMODULE,
            module,m_dwError,MAKELANGID(LANG_NEUTRAL,SUBLANG_SYS_DEFAULT),
            reinterpret_cast<wchar_t*>(&message),0,nullptr)) &&
        !::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,nullptr,m_dwError,
            MAKELANGID(LANG_NEUTRAL,SUBLANG_SYS_DEFAULT),reinterpret_cast<wchar_t*>(&message),0,nullptr)) {
        *output=0;return FALSE;
    }
    if(m_dwError==ERROR_INTERNET_EXTENDED_ERROR) {
        DWORD error,length=0;
        if(!::InternetGetLastResponseInfoW(&error,nullptr,&length) && ::GetLastError()==ERROR_INSUFFICIENT_BUFFER) {
            auto* response=static_cast<wchar_t*>(::LocalAlloc(LPTR,size_t(length)*sizeof(wchar_t)));
            if(response) {
                ::InternetGetLastResponseInfoW(&error,response,&length);
                if(capacity>=length)CopyMessage(output,capacity,response);
                else *output=0;
                ::LocalFree(response);
            } else *output=0;
        }
    } else CopyMessage(output,capacity,message);
    ::LocalFree(message);
    return TRUE;
}
// Convenience ANSI overload; the native Unicode virtual is the ABI owner.
int CInternetException::GetErrorMessage(char* output,UINT capacity,UINT* help) const {
    if(help)*help=0;
    if(!output || !capacity)return FALSE;
    wchar_t text[1024];
    int result=GetErrorMessage(text,1024,nullptr);
    if(result)::WideCharToMultiByte(CP_ACP,0,text,-1,output,capacity,nullptr,nullptr);
    return result;
}
void AFXAPI AfxThrowInternetException(DWORD_PTR context,DWORD code) {
    if(!code)code=::GetLastError();
    void* storage=impl___2_YAPEAX_K_Z(sizeof(CInternetException));
    auto* error=storage?::new(storage) CInternetException(code):nullptr;
    error->m_dwContext=context;
    throw error;
}
// Symbol: ??0CInternetException@@QEAA@K@Z
extern "C" CInternetException* impl___0CInternetException__QEAA_K_Z(CInternetException* self,DWORD code) {return ::new(self) CInternetException(code);}
// Symbol: ??1CInternetException@@UEAA@XZ
extern "C" void impl___1CInternetException__UEAA_XZ(CInternetException* self) {self->CInternetException::~CInternetException();}
// Symbol: ?GetErrorMessage@CInternetException@@UEBAHPEA_WIPEAI@Z
extern "C" int impl__GetErrorMessage_CInternetException__UEBAHPEA_WIPEAI_Z(const CInternetException* self,wchar_t* output,UINT capacity,UINT* help) {return self->CInternetException::GetErrorMessage(output,capacity,help);}
// Symbol: ?AfxThrowInternetException@@YAX_KK@Z
extern "C" void impl__AfxThrowInternetException__YAX_KK_Z(DWORD_PTR context,DWORD code) {AfxThrowInternetException(context,code);}
// Symbol: ?GetRuntimeClass@CInternetException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CInternetException__UEBAPEAUCRuntimeClass__XZ(const CInternetException*) {return CInternetException::GetThisClass();}
// Symbol: ?GetThisClass@CInternetException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CInternetException__SAPEAUCRuntimeClass__XZ() {return CInternetException::GetThisClass();}
