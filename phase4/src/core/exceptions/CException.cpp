// Native NOVTABLE exception base, message forwarding and UI reporting.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
IMPLEMENT_DYNAMIC(CException,CObject)
extern "C" int impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t*,UINT,UINT);
extern "C" int impl__AfxMessageBox__YAHIII_Z(UINT,UINT,UINT);
int CException::GetErrorMessage(wchar_t* output,UINT size,UINT* help) const {
    if (help) *help=0;
    if (size && output) *output=0;
    return FALSE;
}
int CException::ReportError(UINT type,UINT message) {
    wchar_t text[512];UINT help;
    if (GetErrorMessage(text,512,&help)) return impl__AfxMessageBox__YAHPEB_WII_Z(text,type,help);
    return impl__AfxMessageBox__YAHIII_Z(message?message:0xf020,type,help);
}
// These exported base constructors only initialize the actual base field. The
// abstract NOVTABLE base has no independent complete object or replacement root.
// Symbol: ??0CException@@QEAA@XZ
extern "C" CException* impl___0CException__QEAA_XZ(CException* p) {p->m_bAutoDelete=TRUE;return p;}
// Symbol: ??0CException@@QEAA@H@Z
extern "C" CException* impl___0CException__QEAA_H_Z(CException* p,BOOL value) {p->m_bAutoDelete=value;return p;}
// Symbol: ?GetErrorMessage@CException@@UEBAHPEA_WIPEAI@Z
extern "C" int impl__GetErrorMessage_CException__UEBAHPEA_WIPEAI_Z(const CException* p,wchar_t* o,UINT n,UINT* h) {return p->CException::GetErrorMessage(o,n,h);}
// Symbol: ?GetErrorMessage@CException@@UEAAHPEA_WIPEAI@Z
extern "C" int impl__GetErrorMessage_CException__UEAAHPEA_WIPEAI_Z(CException* p,wchar_t* o,UINT n,UINT* h) {return p->CException::GetErrorMessage(o,n,h);}
// Symbol: ?ReportError@CException@@UEAAHII@Z
extern "C" int impl__ReportError_CException__UEAAHII_Z(CException* p,UINT t,UINT m) {return p->CException::ReportError(t,m);}
// Symbol: ?Delete@CException@@QEAAXXZ
extern "C" void impl__Delete_CException__QEAAXXZ(CException* p) {p->Delete();}
// Symbol: ?GetThisClass@CException@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetThisClass_CException__SAPEAUCRuntimeClass__XZ() {return CException::GetThisClass();}
// Symbol: ?GetRuntimeClass@CException@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* impl__GetRuntimeClass_CException__UEBAPEAUCRuntimeClass__XZ(const CException*) {return CException::GetThisClass();}
