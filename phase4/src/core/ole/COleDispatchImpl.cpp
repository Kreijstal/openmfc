// IDispatch embedded at CCmdTarget+0x20, using the owner's native COM lifetime.
#define OPENMFC_APPCORE_IMPL
#include "detail/CCmdTargetDispatchSupport.h"
#include "detail/OleExceptionTranslationSupport.h"
#include <cstring>
namespace {
CCmdTarget* Owner(void* self) {
    return self ? reinterpret_cast<CCmdTarget*>(static_cast<char*>(self)-offsetof(CCmdTarget,m_xDispatch)) : nullptr;
}
}
// Symbol: ?AddRef@COleDispatchImpl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_COleDispatchImpl__UEAAKXZ(void* self) {
    return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(self));
}
// Symbol: ?Release@COleDispatchImpl@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_COleDispatchImpl__UEAAKXZ(void* self) {
    return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(self));
}
// Symbol: ?QueryInterface@COleDispatchImpl@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_COleDispatchImpl__UEAAJAEBU_GUID__PEAPEAX_Z(void* self,const GUID* iid,void** result) {
    if (!self || !iid || !result) return E_POINTER;
    return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(self),iid,result));
}
// Symbol: ?Disconnect@COleDispatchImpl@@UEAAXXZ
extern "C" void MS_ABI impl__Disconnect_COleDispatchImpl__UEAAXXZ(void* self) {
    impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(Owner(self));
}
// Symbol: ?GetTypeInfoCount@COleDispatchImpl@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_COleDispatchImpl__UEAAJPEAI_Z(void* self,unsigned int* result) {
    if (!self || !result) return E_POINTER;
    *result=Owner(self)->GetTypeInfoCount();
    return S_OK;
}
// Symbol: ?GetTypeInfo@COleDispatchImpl@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_COleDispatchImpl__UEAAJIKPEAPEAUITypeInfo___Z(void* self,unsigned int index,unsigned long locale,ITypeInfo** result) {
    if (!self || !result) return E_POINTER;
    *result=nullptr;
    CCmdTarget* target=Owner(self);
    IID iid{};
    if (index || !target->GetDispatchIID(&iid)) return DISP_E_BADINDEX;
    openmfc::detail::CmdTargetModuleScope module(target->m_pModuleState);
    ITypeLib* library=nullptr;
    HRESULT hr=target->GetTypeLib(locale,&library);
    if (FAILED(hr)) return hr;
    if (!library) return TYPE_E_CANTLOADLIBRARY;
    hr=library->GetTypeInfoOfGuid(iid,result);
    library->Release();
    return hr;
}
// Symbol: ?GetIDsOfNames@COleDispatchImpl@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_COleDispatchImpl__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* self,const GUID* iid,wchar_t** names,unsigned int count,unsigned long locale,long* ids) {
    (void)locale;
    if (!self || !iid || (count && (!names || !ids))) return E_POINTER;
    if (*iid!=IID_NULL) return DISP_E_UNKNOWNINTERFACE;
    CCmdTarget* target=Owner(self);
    HRESULT hr=S_OK;
    for (UINT i=0;i<count;++i) {
        ids[i]=impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(target->GetDispatchMap(),names[i]);
        if (ids[i]==DISPID_UNKNOWN) hr=DISP_E_UNKNOWNNAME;
    }
    return hr;
}
namespace {
HRESULT InvokeTarget(CCmdTarget* target,long id,WORD flags,DISPPARAMS* args,VARIANT* result,UINT* argError) {
    target->m_bResultExpected=result!=nullptr;
    if (!target->IsInvokeAllowed(id)) return E_UNEXPECTED;
    auto* entry=static_cast<const AFX_DISPMAP_ENTRY*>(impl__GetDispEntry_CCmdTarget__IEAAPEBUAFX_DISPMAP_ENTRY__J_Z(target,id));
    if (!entry) return DISP_E_MEMBERNOTFOUND;
    if (entry->nPropOffset==0)
        return impl__CallMemberFunc_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__GPEAUtagVARIANT__PEAUtagDISPPARAMS__PEAI_Z(target,entry,flags,result,args,argError);
    UINT localError=0;
    if (!argError) argError=&localError;
    if (flags & (DISPATCH_PROPERTYPUT | DISPATCH_PROPERTYPUTREF)) {
        if (args->cArgs!=1 || !args->rgvarg) return DISP_E_BADPARAMCOUNT;
        if (args->cNamedArgs!=1 || !args->rgdispidNamedArgs || args->rgdispidNamedArgs[0]!=DISPID_PROPERTYPUT)
            return DISP_E_PARAMNOTFOUND;
        return impl__SetStandardProp_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__PEAUtagDISPPARAMS__PEAI_Z(target,entry,args,argError);
    }
    if (!(flags & DISPATCH_PROPERTYGET)) return DISP_E_MEMBERNOTFOUND;
    if (args->cArgs) return DISP_E_BADPARAMCOUNT;
    VARIANT ignored{};
    HRESULT hr=ReadDispatchProperty(target,entry,result ? result : &ignored);
    if (!result) VariantClear(&ignored);
    return hr;

}
}
// Symbol: ?Invoke@COleDispatchImpl@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_COleDispatchImpl__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* self,long id,const GUID* iid,unsigned long locale,unsigned short flags,DISPPARAMS* args,
    VARIANT* result,EXCEPINFO* exception,unsigned int* argError) {
    (void)locale;
    if (!self || !iid || !args) return E_POINTER;
    if (*iid!=IID_NULL) return DISP_E_UNKNOWNINTERFACE;
    if(result)VariantInit(result);
    CCmdTarget* target=Owner(self);
    openmfc::detail::CmdTargetModuleScope module(target->m_pModuleState);
    try { return InvokeTarget(target,id,flags,args,result,argError); }
    catch(COleException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        return error ? error->m_sc : E_UNEXPECTED;
    } catch(CException* error) {
        openmfc::detail::FrameworkExceptionCleanup cleanup{error};
        HRESULT translated=FillOleDispatchExceptionInfo(exception,error);
        return FAILED(translated) ? translated : DISP_E_EXCEPTION;
    }
}

void** CommandTargetDispatchTable() {
    static void* table[]={
        reinterpret_cast<void*>(impl__QueryInterface_COleDispatchImpl__UEAAJAEBU_GUID__PEAPEAX_Z),
        reinterpret_cast<void*>(impl__AddRef_COleDispatchImpl__UEAAKXZ),
        reinterpret_cast<void*>(impl__Release_COleDispatchImpl__UEAAKXZ),
        reinterpret_cast<void*>(impl__GetTypeInfoCount_COleDispatchImpl__UEAAJPEAI_Z),
        reinterpret_cast<void*>(impl__GetTypeInfo_COleDispatchImpl__UEAAJIKPEAPEAUITypeInfo___Z),
        reinterpret_cast<void*>(impl__GetIDsOfNames_COleDispatchImpl__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z),
        reinterpret_cast<void*>(impl__Invoke_COleDispatchImpl__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z),
        reinterpret_cast<void*>(impl__Disconnect_COleDispatchImpl__UEAAXXZ)
    };
    return table;
}
