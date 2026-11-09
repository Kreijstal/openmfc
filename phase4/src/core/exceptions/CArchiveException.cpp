// Actual native archive exception, localized message resources and typed throws.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cerrno>
#include <new>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
int CArchiveException::GetErrorMessage(wchar_t* output,UINT capacity,UINT* help) const {
    if(!output)return FALSE;
    try {
    if(help)*help=UINT(m_cause)+0xf1b0;
    CString message,filename(m_strFileName);
    if(filename.IsEmpty()) {
        if(!filename.LoadString(0xf006))AfxThrowInvalidArgException();
    }
    const wchar_t* argument=filename.GetString();
    AfxFormatStrings(message,UINT(m_cause)+0xf1b0,&argument,1);
    int result=wcsncpy_s(output,capacity,message.GetString(),_TRUNCATE);
    if(result && result!=STRUNCATE) {
        if(result==ENOMEM)AfxThrowMemoryException();
        else AfxThrowInvalidArgException();
    }
    return TRUE;
    } catch(CException* error) {error->Delete();return FALSE;}
}

void AFXAPI AfxThrowArchiveException(int cause,const wchar_t* name) {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(CArchiveException));
    CArchiveException* error=nullptr;
    if(storage) {
        try {error=::new(storage) CArchiveException(cause,name);}
        catch(...){impl___3_YAXPEAX_Z(storage);throw;}
    }
    throw error;
}
// Symbol: ?GetErrorMessage@CArchiveException@@UEBAHPEA_WIPEAI@Z
extern "C" int impl__GetErrorMessage_CArchiveException__UEBAHPEA_WIPEAI_Z(const CArchiveException* self,wchar_t* output,UINT capacity,UINT* help) {return self->CArchiveException::GetErrorMessage(output,capacity,help);}
// Symbol: ?AfxThrowArchiveException@@YAXHPEB_W@Z
extern "C" void impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause,const wchar_t* name) {AfxThrowArchiveException(cause,name);}
