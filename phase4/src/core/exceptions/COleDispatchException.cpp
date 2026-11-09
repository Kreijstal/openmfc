// Native dispatch exception ownership, layout, and metadata conversion.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleExceptionTranslationSupport.h"
#include <cerrno>
#include <cwchar>
#include <new>
#include <cstring>
static_assert(sizeof(COleDispatchException)==0x40, "native dispatch exception size");
static_assert(offsetof(COleDispatchException,m_wCode)==0x10, "native error code");
static_assert(offsetof(COleDispatchException,m_strDescription)==0x18, "native description");
static_assert(offsetof(COleDispatchException,m_dwHelpContext)==0x20, "native help context");
static_assert(offsetof(COleDispatchException,m_strHelpFile)==0x28, "native help file");
static_assert(offsetof(COleDispatchException,m_strSource)==0x30, "native source");
static_assert(offsetof(COleDispatchException,m_scError)==0x38, "native SCODE");
IMPLEMENT_DYNAMIC(COleDispatchException,CException)
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT,wchar_t*,UINT);
COleDispatchException::COleDispatchException(const wchar_t* description,UINT help,WORD code):CException(TRUE) {
    m_dwHelpContext=help ? help+0x60000:0;
    m_wCode=code;
    if(description)m_strDescription=description;
    m_scError=code ? S_OK:E_UNEXPECTED;
}
COleDispatchException::~COleDispatchException() = default;
// Symbol: ??1COleDispatchException@@UEAA@XZ
extern "C" void MS_ABI impl___1COleDispatchException__UEAA_XZ(COleDispatchException* self) {
    self->COleDispatchException::~COleDispatchException();
}
int COleDispatchException::GetErrorMessage(wchar_t* out,UINT capacity,UINT* help) const {
    if(help)*help=0;
    int result=wcsncpy_s(out,capacity,m_strDescription.GetString(),_TRUNCATE);
    if(result && result!=STRUNCATE) {
        if(result==ENOMEM)AfxThrowMemoryException();
        else AfxThrowInvalidArgException();
    }
    return TRUE;
}
// Symbol: ?GetErrorMessage@COleDispatchException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z(
    const COleDispatchException* self,wchar_t* out,UINT capacity,UINT* help) {
    return self->COleDispatchException::GetErrorMessage(out,capacity,help);
}
// Symbol: ??0COleDispatchException@@QEAA@PEB_WIG@Z
extern "C" COleDispatchException* MS_ABI impl___0COleDispatchException__QEAA_PEB_WIG_Z(
    COleDispatchException* self,const wchar_t* description,UINT helpID,WORD code) {
    return ::new(self) COleDispatchException(description,helpID,code);
}
void AFXAPI AfxThrowOleDispatchException(WORD code,const wchar_t* description,UINT help) {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(COleDispatchException));
    COleDispatchException* error=nullptr;
    if(storage) {
        try {error=::new(storage) COleDispatchException(description,help,code);}
        catch(...){impl___3_YAXPEAX_Z(storage);throw;}
    }
    throw error;
}
void AFXAPI AfxThrowOleDispatchException(WORD code,UINT description,UINT help) {
    wchar_t text[256];
    impl__AfxLoadString__YAHIPEA_WI_Z(description,text,256);
    AfxThrowOleDispatchException(code,text,help==UINT_MAX ? description:help);
}
// Symbol: ?AfxThrowOleDispatchException@@YAXGPEB_WI@Z
extern "C" void impl__AfxThrowOleDispatchException__YAXGPEB_WI_Z(WORD code,const wchar_t* description,UINT help) {AfxThrowOleDispatchException(code,description,help);}
// Symbol: ?AfxThrowOleDispatchException@@YAXGII@Z
extern "C" void impl__AfxThrowOleDispatchException__YAXGII_Z(WORD code,UINT description,UINT help) {AfxThrowOleDispatchException(code,description,help);}
// Symbol: ?GetThisClass@COleDispatchException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_COleDispatchException__SAPEAUCRuntimeClass__XZ() {return COleDispatchException::GetThisClass();}
// Symbol: ?GetRuntimeClass@COleDispatchException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_COleDispatchException__UEBAPEAUCRuntimeClass__XZ(const COleDispatchException*) {return COleDispatchException::GetThisClass();}
// Symbol: ?Process@COleDispatchException@@SAXPEAUtagEXCEPINFO@@PEBVCException@@@Z
extern "C" void MS_ABI impl__Process_COleDispatchException__SAXPEAUtagEXCEPINFO__PEBVCException___Z(
    EXCEPINFO* out,const CException* error) {
    COleDispatchException::Process(out,error);
}
