// Native FORMATETC enumerator with owned target-device records.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include "detail/InterfaceMapsSupport.h"
#include <cstring>
#include <new>
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void*);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
namespace {
const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IEnumFORMATETC,0x68},{nullptr,SIZE_MAX}};
}
namespace openmfc { namespace detail { namespace interfacemaps {
const AFX_INTERFACEMAP* AFXAPI gib_CEnumFormatEtc(){return impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ();}
const AFX_INTERFACEMAP g_imap_CEnumFormatEtc={gib_CEnumFormatEtc,entries};
}}}
namespace openmfc { namespace detail { struct EnumFormatAccess {
    static BOOL Next(CEnumFormatEtc* self,void* out){return self->CEnumFormatEtc::OnNext(out);}
};}}
CEnumFormatEtc::CEnumFormatEtc():CEnumArray(sizeof(FORMATETC),nullptr,0,TRUE),m_nMaxSize(0){}
CEnumFormatEtc::~CEnumFormatEtc(){
    if(!m_pClonedFrom){auto* values=static_cast<const FORMATETC*>(m_pvEnum);for(UINT i=0;i<m_nSize;++i)::CoTaskMemFree(values[i].ptd);}
}
BOOL CEnumFormatEtc::OnNext(void* out){
    if(!CEnumArray::OnNext(out))return FALSE;
    auto* result=static_cast<FORMATETC*>(out);
    if(auto* original=result->ptd){
        result->ptd=static_cast<DVTARGETDEVICE*>(::CoTaskMemAlloc(original->tdSize));
        if(result->ptd && original->tdSize)std::memcpy(result->ptd,original,original->tdSize);
        if(!result->ptd)impl__AfxThrowMemoryException__YAXXZ();
    }
    return TRUE;
}
void CEnumFormatEtc::AddFormat(const FORMATETC* value){
    if(m_nSize==m_nMaxSize){
        auto* copy=static_cast<FORMATETC*>(impl___2_YAPEAX_K_Z(size_t(UINT(m_nSize+10))*sizeof(FORMATETC)));
        m_nMaxSize+=10;
        size_t bytes=size_t(m_nSize)*sizeof(FORMATETC);
        if(bytes && memcpy_s(copy,size_t(UINT(m_nSize+10))*sizeof(FORMATETC),m_pvEnum,bytes))impl__AfxThrowInvalidArgException__YAXXZ();
        impl___3_YAXPEAX_Z(const_cast<void*>(m_pvEnum));m_pvEnum=copy;
    }
    auto& target=const_cast<FORMATETC*>(static_cast<const FORMATETC*>(m_pvEnum))[m_nSize];
    target.cfFormat=value->cfFormat;target.ptd=value->ptd;target.dwAspect=value->dwAspect;target.lindex=value->lindex;target.tymed=value->tymed;++m_nSize;
}
const AFX_INTERFACEMAP* AFXAPI CEnumFormatEtc::GetThisInterfaceMap(){return &openmfc::detail::interfacemaps::g_imap_CEnumFormatEtc;}
const AFX_INTERFACEMAP* CEnumFormatEtc::GetInterfaceMap() const{return GetThisInterfaceMap();}
// Symbol: ??0CEnumFormatEtc@@QEAA@XZ
extern "C" void* MS_ABI impl___0CEnumFormatEtc__QEAA_XZ(void* self){return new(self)CEnumFormatEtc();}
// Symbol: ??1CEnumFormatEtc@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumFormatEtc__UEAA_XZ(void* self){static_cast<CEnumFormatEtc*>(self)->CEnumFormatEtc::~CEnumFormatEtc();}
// Symbol: ?OnNext@CEnumFormatEtc@@MEAAHPEAX@Z
extern "C" BOOL MS_ABI impl__OnNext_CEnumFormatEtc__MEAAHPEAX_Z(CEnumFormatEtc* self,void* out){return openmfc::detail::EnumFormatAccess::Next(self,out);}
// Symbol: ?AddFormat@CEnumFormatEtc@@QEAAXPEBUtagFORMATETC@@@Z
extern "C" void MS_ABI impl__AddFormat_CEnumFormatEtc__QEAAXPEBUtagFORMATETC___Z(CEnumFormatEtc* self,const FORMATETC* value){self->AddFormat(value);}
// Symbol: ?GetThisInterfaceMap@CEnumFormatEtc@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CEnumFormatEtc__KAPEBUAFX_INTERFACEMAP__XZ(){return &openmfc::detail::interfacemaps::g_imap_CEnumFormatEtc;}
// Symbol: ?GetInterfaceMap@CEnumFormatEtc@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CEnumFormatEtc__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*){return &openmfc::detail::interfacemaps::g_imap_CEnumFormatEtc;}
