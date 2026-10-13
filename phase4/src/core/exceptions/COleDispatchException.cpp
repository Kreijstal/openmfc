// Native dispatch exception ownership, layout, and metadata conversion.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleExceptionTranslationSupport.h"
#include <algorithm>
#include <cstring>
static_assert(sizeof(COleDispatchException)==0x40, "native dispatch exception size");
static_assert(offsetof(COleDispatchException,m_wCode)==0x10, "native error code");
static_assert(offsetof(COleDispatchException,m_strDescription)==0x18, "native description");
static_assert(offsetof(COleDispatchException,m_dwHelpContext)==0x20, "native help context");
static_assert(offsetof(COleDispatchException,m_strHelpFile)==0x28, "native help file");
static_assert(offsetof(COleDispatchException,m_strSource)==0x30, "native source");
static_assert(offsetof(COleDispatchException,m_scError)==0x38, "native SCODE");
// Symbol: ??1COleDispatchException@@UEAA@XZ
extern "C" COleDispatchException* MS_ABI impl___1COleDispatchException__UEAA_XZ(COleDispatchException* self) {
    if(self)self->COleDispatchException::~COleDispatchException();
    return self;
}
int COleDispatchException::GetErrorMessage(wchar_t* out,UINT capacity,UINT* help) const {
    if(help)*help=m_dwHelpContext;
    if(!out || !capacity)return FALSE;
    size_t length=std::min(static_cast<size_t>(m_strDescription.GetLength()),static_cast<size_t>(capacity-1));
    if(length)std::memcpy(out,static_cast<const wchar_t*>(m_strDescription),length*sizeof(wchar_t));
    out[length]=L'\0';return TRUE;
}
// Symbol: ?GetErrorMessage@COleDispatchException@@UEBAHPEA_WIPEAI@Z
extern "C" int MS_ABI impl__GetErrorMessage_COleDispatchException__UEBAHPEA_WIPEAI_Z(
    const COleDispatchException* self,wchar_t* out,UINT capacity,UINT* help) {
    if(!self) { if(help)*help=0;if(out && capacity)*out=L'\0';return FALSE; }
    return self->COleDispatchException::GetErrorMessage(out,capacity,help);
}
// Symbol: ??0COleDispatchException@@QEAA@PEB_WIG@Z
extern "C" COleDispatchException* MS_ABI impl___0COleDispatchException__QEAA_PEB_WIG_Z(
    COleDispatchException* self,const wchar_t* description,UINT helpID,WORD code) {
    return self ? new(self)COleDispatchException(description,helpID,code) : nullptr;
}
// Symbol: ?Process@COleDispatchException@@SAXPEAUtagEXCEPINFO@@PEBVCException@@@Z
extern "C" void MS_ABI impl__Process_COleDispatchException__SAXPEAUtagEXCEPINFO__PEBVCException___Z(
    EXCEPINFO* out,const CException* error) {
    HRESULT hr=FillOleDispatchExceptionInfo(out,error);
    if(FAILED(hr))impl__AfxThrowMemoryException__YAXXZ();
}
