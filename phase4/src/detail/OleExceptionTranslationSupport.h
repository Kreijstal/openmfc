#pragma once
#include "detail/InterfaceMapsSupport.h"
#include "openmfc/afxdisp.h"
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__Process_COleDispatchException__SAXPEAUtagEXCEPINFO__PEBVCException___Z(EXCEPINFO*,const CException*);
extern "C" COleDispatchException* MS_ABI impl___0COleDispatchException__QEAA_PEB_WIG_Z(COleDispatchException*,const wchar_t*,UINT,WORD);
extern "C" COleDispatchException* MS_ABI impl___1COleDispatchException__UEAA_XZ(COleDispatchException*);
HRESULT FillOleDispatchExceptionInfo(EXCEPINFO*,const CException*);
namespace openmfc { namespace detail {
struct FrameworkExceptionCleanup {
    CException* error;
    ~FrameworkExceptionCleanup() { if(error)error->Delete(); }
};
}}
