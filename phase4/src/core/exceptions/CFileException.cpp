// Actual native file exception, localized message resources and typed throws.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <cerrno>
#include <new>
extern "C" void* impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(UINT);
extern "C" void* impl___2_YAPEAX_K_Z(size_t);
extern "C" void impl___3_YAXPEAX_Z(void*);
int CFileException::GetErrorMessage(wchar_t* output,UINT capacity,UINT* help) const {
    if(!output || !capacity)return FALSE;
    if(help)*help=UINT(m_cause)+0xf1a0;
    CString message,filename(m_strFileName);
    if(filename.IsEmpty()) {
        if(!filename.LoadString(0xf006))AfxThrowInvalidArgException();
    }
    const wchar_t* argument=filename.GetString();
    AfxFormatStrings(message,UINT(m_cause)+0xf1a0,&argument,1);
    int result=wcsncpy_s(output,capacity,message.GetString(),_TRUNCATE);
    if(result && result!=STRUNCATE) {
        if(result==ENOMEM)AfxThrowMemoryException();
        else AfxThrowInvalidArgException();
    }
    return TRUE;
}
int PASCAL CFileException::ErrnoToException(int error) {
    switch(error) {
    case 1:case 13:return accessDenied;
    case 2:case 23:return fileNotFound;
    case 5:case 22:return hardIO;
    case 9:return invalidFile;
    case 24:return tooManyOpenFiles;
    case 28:return diskFull;
    case 36:return sharingViolation;
    default:return genericException;
    }
}
void PASCAL CFileException::ThrowErrno(int error,const wchar_t* name) {
    if(error) {
        auto* dosError=__doserrno();
        int cause=ErrnoToException(error);
        AfxThrowFileException(cause,LONG(*dosError),name);
    }
}
void AFXAPI AfxThrowFileException(int cause,long osError,const wchar_t* name) {
    void* storage=impl___2_YAPEAX_K_Z(sizeof(CFileException));
    CFileException* error=nullptr;
    if(storage) {
        try {error=::new(storage) CFileException(cause,osError,name);}
        catch(...){impl___3_YAXPEAX_Z(storage);throw;}
    }
    throw error;
}
// Symbol: ?GetErrorMessage@CFileException@@UEBAHPEA_WIPEAI@Z
extern "C" int impl__GetErrorMessage_CFileException__UEBAHPEA_WIPEAI_Z(const CFileException* self,wchar_t* output,UINT capacity,UINT* help) {return self->CFileException::GetErrorMessage(output,capacity,help);}
// Symbol: ?ErrnoToException@CFileException@@SAHH@Z
extern "C" int impl__ErrnoToException_CFileException__SAHH_Z(int error) {return CFileException::ErrnoToException(error);}
// Symbol: ?ThrowErrno@CFileException@@SAXHPEB_W@Z
extern "C" void impl__ThrowErrno_CFileException__SAXHPEB_W_Z(int error,const wchar_t* name) {CFileException::ThrowErrno(error,name);}
// Symbol: ?AfxThrowFileException@@YAXHJPEB_W@Z
extern "C" void impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause,LONG error,const wchar_t* name) {AfxThrowFileException(cause,error,name);}
