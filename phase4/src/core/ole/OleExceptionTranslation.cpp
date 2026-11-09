// Native COleDispatchException::Process writes EXCEPINFO in allocation order.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleExceptionTranslationSupport.h"
#include "detail/RegcoreSupport.h"
#include <cstring>
extern "C" CRuntimeClass* impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
extern "C" AFX_MODULE_STATE* impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
extern "C" int impl__AfxLoadString__YAHIPEA_WI_Z(UINT,wchar_t*,UINT);
static_assert(offsetof(CWinApp,m_pszHelpFilePath)==0xd0,"native application help file");
void PASCAL COleDispatchException::Process(EXCEPINFO* out,const CException* error) {
    std::memset(out,0,sizeof(*out));
    wchar_t buffer[256];
    const wchar_t* description;
    if(error->IsKindOf(COleDispatchException::GetThisClass())) {
        const auto* dispatch=static_cast<const COleDispatchException*>(error);
        description=dispatch->m_strDescription.GetString();
        out->wCode=dispatch->m_wCode;
        out->dwHelpContext=dispatch->m_dwHelpContext;
        out->scode=dispatch->m_scError;
        if(!dispatch->m_strHelpFile.IsEmpty()) {
            CString text(dispatch->m_strHelpFile);
            out->bstrHelpFile=::SysAllocString(text.GetString());
        }
        if(!dispatch->m_strSource.IsEmpty()) {
            CString text(dispatch->m_strSource);
            out->bstrSource=::SysAllocString(text.GetString());
        }
    } else {
        UINT resource=error->IsKindOf(impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ()) ? 0xf10a:0xf108;
        impl__AfxLoadString__YAHIPEA_WI_Z(resource,buffer,256);
        out->wCode=WORD(resource);
        description=buffer;
    }
    {
        CString text(description);
        out->bstrDescription=::SysAllocString(text.GetString());
    }
    if(!out->bstrSource) {
        CString text(impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_lpszCurrentAppName);
        out->bstrSource=::SysAllocString(text.GetString());
    }
    if(!out->bstrHelpFile && out->dwHelpContext) {
        auto* app=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ()->m_pCurrentWinApp;
        CString text(app->m_pszHelpFilePath);
        out->bstrHelpFile=::SysAllocString(text.GetString());
    }
}
// IDispatch and event boundaries allow callers to omit EXCEPINFO.
HRESULT FillOleDispatchExceptionInfo(EXCEPINFO* out,const CException* error) {
    if(out)COleDispatchException::Process(out,error);
    return S_OK;
}
