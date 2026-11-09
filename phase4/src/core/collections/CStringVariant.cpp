// Native VARIANT coercion and cleanup for both CString character widths.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <oleauto.h>
namespace {
class ConvertedVariant {
public:
    VARIANT value;
    ConvertedVariant() {std::memset(&value,0,sizeof(value));::VariantInit(&value);}
    ~ConvertedVariant() {::VariantClear(&value);}
    HRESULT Coerce(const VARIANT& source) {return ::VariantChangeType(&value,&source,0,VT_BSTR);}
};
void CheckCoercion(HRESULT status) {
    if(FAILED(status)) {
        if(status==E_OUTOFMEMORY)AfxThrowMemoryException();
        else AfxThrowOleException(status);
    }
}
}
CStringA::CStringA(const VARIANT& source):CStringA(source,OpenMFC_GetStringMgr()) {}
CStringA::CStringA(const VARIANT& source,IAtlStringMgr* manager):CSimpleStringA(manager) {
    ConvertedVariant converted;CheckCoercion(converted.Coerce(source));operator=(converted.value.bstrVal);
}
CStringA& CStringA::operator=(const VARIANT& source) {
    ConvertedVariant converted;CheckCoercion(converted.Coerce(source));
    operator=(converted.value.bstrVal);return *this;
}
CStringA& CStringA::operator+=(const wchar_t* source) {
    auto* manager=GetManager();if(!manager)manager=OpenMFC_GetStringMgr()->Clone();
    CStringA converted(source,manager);Append(converted);return *this;
}
CStringA& CStringA::operator+=(const VARIANT& source) {
    ConvertedVariant converted;CheckCoercion(converted.Coerce(source));
    operator+=(converted.value.bstrVal);return *this;
}
CString::CString(const VARIANT& source):CString(source,OpenMFC_GetStringMgr()) {}
CString::CString(const VARIANT& source,IAtlStringMgr* manager):CSimpleStringW(manager) {
    ConvertedVariant converted;CheckCoercion(converted.Coerce(source));operator=(converted.value.bstrVal);
}
CString& CString::operator=(const VARIANT& source) {
    ConvertedVariant converted;CheckCoercion(converted.Coerce(source));
    operator=(converted.value.bstrVal);return *this;
}
CString& CString::operator+=(const VARIANT& source) {
    ConvertedVariant converted;CheckCoercion(converted.Coerce(source));
    Append(converted.value.bstrVal);return *this;
}

#include <new>
// Symbol: ??Y?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBUtagVARIANT@@@Z
extern "C" CString* impl___Y__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CString* self,const VARIANT* source) {*self+=*source;return self;}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEB_W_Z(CStringA* self,const wchar_t* text) {*self+=text;return self;}
// Symbol: ??Y?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@AEBUtagVARIANT@@@Z
extern "C" CStringA* impl___Y__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CStringA* self,const VARIANT* source) {*self+=*source;return self;}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@AEBUtagVARIANT@@@Z
extern "C" CString* impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CString* self,const VARIANT* source) {*self=*source;return self;}
// Symbol: ??4?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAAAEAV01@AEBUtagVARIANT@@@Z
extern "C" CStringA* impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_AEBUtagVARIANT___Z(CStringA* self,const VARIANT* source) {*self=*source;return self;}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@AEBUtagVARIANT@@PEAUIAtlStringMgr@1@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBUtagVARIANT__PEAUIAtlStringMgr_1__Z(CString* self,const VARIANT* source,IAtlStringMgr* manager) {return ::new(self) CString(*source,manager);}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@AEBUtagVARIANT@@@Z
extern "C" CString* impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_AEBUtagVARIANT___Z(CString* self,const VARIANT* source) {return ::new(self) CString(*source);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@AEBUtagVARIANT@@PEAUIAtlStringMgr@1@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBUtagVARIANT__PEAUIAtlStringMgr_1__Z(CStringA* self,const VARIANT* source,IAtlStringMgr* manager) {return ::new(self) CStringA(*source,manager);}
// Symbol: ??0?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@QEAA@AEBUtagVARIANT@@@Z
extern "C" CStringA* impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_AEBUtagVARIANT___Z(CStringA* self,const VARIANT* source) {return ::new(self) CStringA(*source);}
// Symbol: ?Append@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_WH@Z
extern "C" void impl__Append___CSimpleStringT__W_00_ATL__QEAAXPEB_WH_Z(CString* self,const wchar_t* text,int count) {self->Append(text,count);}
// Symbol: ?Append@?$CSimpleStringT@_W$00@ATL@@QEAAXPEB_W@Z
extern "C" void impl__Append___CSimpleStringT__W_00_ATL__QEAAXPEB_W_Z(CString* self,const wchar_t* text) {self->Append(text);}
// Symbol: ?Append@?$CSimpleStringT@_W$00@ATL@@QEAAXAEBV12@@Z
extern "C" void impl__Append___CSimpleStringT__W_00_ATL__QEAAXAEBV12__Z(CString* self,const CString* text) {self->Append(*text);}
