// Native CEnumArray owners and MSVC-emitted class/interface vtables.
#define OPENMFC_APPCORE_IMPL
#include "detail/CEnumArrayInterfacesSupport.h"
#include "openmfc/afxole.h"
#include <new>
#include <cstring>
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(size_t);
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void*);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace {
template<class T>T& Field(void* self,size_t offset){return *reinterpret_cast<T*>(static_cast<BYTE*>(self)+offset);}
CEnumArray* Owner(CEnumArray::XEnumVOID* self){return reinterpret_cast<CEnumArray*>(reinterpret_cast<BYTE*>(self)-0x68);}
}
CEnumArray::CEnumArray(size_t size,const void* values,UINT count,BOOL freeValues)
    :m_nSizeElem(size),m_pClonedFrom(nullptr),m_pvEnum(values),m_nCurPos(0),m_nSize(count),m_bNeedFree(freeValues){}
CEnumArray::~CEnumArray(){
    if(m_pClonedFrom)impl__InternalRelease_CCmdTarget__QEAAKXZ(m_pClonedFrom);
    if(m_bNeedFree)impl___3_YAXPEAX_Z(const_cast<void*>(m_pvEnum));
}
// These base operations also serve existing enumerators with the native prefix.
// Symbol: ?OnNext@CEnumArray@@MEAAHPEAX@Z
extern "C" BOOL MS_ABI impl__OnNext_CEnumArray__MEAAHPEAX_Z(void* self,void* out){
    UINT position=Field<UINT>(self,0x58);if(position>=Field<UINT>(self,0x5c))return FALSE;
    size_t size=Field<size_t>(self,0x40);
    auto address=reinterpret_cast<UINT_PTR>(Field<const void*>(self,0x50))+size*position;
    if(size && memcpy_s(out,size,reinterpret_cast<const void*>(address),size))impl__AfxThrowInvalidArgException__YAXXZ();
    ++Field<UINT>(self,0x58);return TRUE;
}
// Symbol: ?OnSkip@CEnumArray@@MEAAHXZ
extern "C" BOOL MS_ABI impl__OnSkip_CEnumArray__MEAAHXZ(void* self){
    auto& position=Field<UINT>(self,0x58);UINT count=Field<UINT>(self,0x5c);
    if(position>=count)return FALSE;return ++position<count;
}
// Symbol: ?OnReset@CEnumArray@@MEAAXXZ
extern "C" void MS_ABI impl__OnReset_CEnumArray__MEAAXXZ(void* self){Field<UINT>(self,0x58)=0;}
// Symbol: ?OnClone@CEnumArray@@MEAAPEAV1@XZ
extern "C" void* MS_ABI impl__OnClone_CEnumArray__MEAAPEAV1_XZ(void* self){
    void* memory=impl___2_YAPEAX_K_Z(sizeof(CEnumArray));
    auto* copy=memory ? new(memory)CEnumArray(Field<size_t>(self,0x40),Field<const void*>(self,0x50),Field<UINT>(self,0x5c),FALSE):nullptr;
    copy->m_nCurPos=Field<UINT>(self,0x58);return copy;
}
BOOL CEnumArray::OnNext(void* out){return impl__OnNext_CEnumArray__MEAAHPEAX_Z(this,out);}
BOOL CEnumArray::OnSkip(){return impl__OnSkip_CEnumArray__MEAAHXZ(this);}
void CEnumArray::OnReset(){impl__OnReset_CEnumArray__MEAAXXZ(this);}
CEnumArray* CEnumArray::OnClone(){return static_cast<CEnumArray*>(impl__OnClone_CEnumArray__MEAAPEAV1_XZ(this));}
STDMETHODIMP CEnumArray::XEnumVOID::QueryInterface(REFIID iid,void** out){return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(this),&iid,out));}
STDMETHODIMP_(ULONG) CEnumArray::XEnumVOID::AddRef(){return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP_(ULONG) CEnumArray::XEnumVOID::Release(){return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(this));}
STDMETHODIMP CEnumArray::XEnumVOID::Next(ULONG count,void* out,ULONG* fetched){return impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z(this,count,out,fetched);}
STDMETHODIMP CEnumArray::XEnumVOID::Skip(ULONG count){return impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z(this,count);}
STDMETHODIMP CEnumArray::XEnumVOID::Reset(){return impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ(this);}
STDMETHODIMP CEnumArray::XEnumVOID::Clone(IEnumVOID** out){return impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z(this,reinterpret_cast<void**>(out));}
// Symbol: ??0CEnumArray@@QEAA@_KPEBXIH@Z
extern "C" void* MS_ABI impl___0CEnumArray__QEAA__KPEBXIH_Z(void* self,size_t size,const void* values,UINT count,BOOL freeValues){return new(self)CEnumArray(size,values,count,freeValues);}
// Symbol: ??1CEnumArray@@UEAA@XZ
extern "C" void MS_ABI impl___1CEnumArray__UEAA_XZ(void* self){static_cast<CEnumArray*>(self)->CEnumArray::~CEnumArray();}
