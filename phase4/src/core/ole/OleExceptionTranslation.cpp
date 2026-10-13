// COleDispatchException::Process, retail mfc140u RVA 0x251270.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleExceptionTranslationSupport.h"
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ();
namespace {
struct OwnedInfo {
    EXCEPINFO value{};
    ~OwnedInfo() { SysFreeString(value.bstrSource);SysFreeString(value.bstrDescription);SysFreeString(value.bstrHelpFile); }
};
BSTR Allocate(const CString& text) { return SysAllocStringLen(text,static_cast<UINT>(text.GetLength())); }
}
HRESULT FillOleDispatchExceptionInfo(EXCEPINFO* out,const CException* error) {
    if(!out)return S_OK;
    *out={};
    OwnedInfo temporary;
    auto& info=temporary.value;
    CWinApp* app=AfxGetApp();
    if(error && error->IsKindOf(COleDispatchException::GetThisClass())) {
        const auto* dispatch=static_cast<const COleDispatchException*>(error);
        info.wCode=dispatch->m_wCode;info.dwHelpContext=dispatch->m_dwHelpContext;info.scode=dispatch->m_scError;
        info.bstrDescription=Allocate(dispatch->m_strDescription);
        if(!info.bstrDescription)return E_OUTOFMEMORY;
        if(!dispatch->m_strSource.IsEmpty()) {
            info.bstrSource=Allocate(dispatch->m_strSource);
            if(!info.bstrSource)return E_OUTOFMEMORY;
        }
        if(!dispatch->m_strHelpFile.IsEmpty()) {
            info.bstrHelpFile=Allocate(dispatch->m_strHelpFile);
            if(!info.bstrHelpFile)return E_OUTOFMEMORY;
        }
    } else {
        UINT resource=error && error->IsKindOf(impl__GetThisClass_CMemoryException__SAPEAUCRuntimeClass__XZ()) ? 0xf10a : 0xf108;
        wchar_t description[256]{};
        LoadStringW(AfxGetResourceHandle(),resource,description,256);
        info.wCode=static_cast<WORD>(resource);
        info.bstrDescription=SysAllocString(description);
        if(!info.bstrDescription)return E_OUTOFMEMORY;
    }
    if(!info.bstrSource) {
        info.bstrSource=SysAllocString(app && app->m_pszAppName ? app->m_pszAppName : L"");
        if(!info.bstrSource)return E_OUTOFMEMORY;
    }
    if(!info.bstrHelpFile && info.dwHelpContext) {
        info.bstrHelpFile=SysAllocString(app && app->m_pszHelpFilePath ? app->m_pszHelpFilePath : L"");
        if(!info.bstrHelpFile)return E_OUTOFMEMORY;
    }
    *out=info;info.bstrSource=info.bstrDescription=info.bstrHelpFile=nullptr;
    return S_OK;
}
