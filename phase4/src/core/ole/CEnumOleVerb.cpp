// Native CEnumArray-shaped verb enumeration and COM task-allocator ownership.
#define OPENMFC_APPCORE_IMPL
#include "detail/OleVerbSupport.h"
#include <new>
#include <cstdlib>
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" int MS_ABI impl__OnNext_CEnumArray__MEAAHPEAX_Z(void*,void*);
extern "C" void* MS_ABI impl__OnClone_CEnumArray__MEAAPEAV1_XZ(void*);
namespace {
CEnumOleVerb* Owner(void* self) { return reinterpret_cast<CEnumOleVerb*>(static_cast<char*>(self)-0x68); }
HRESULT MS_ABI Query(void* self,REFIID iid,void** out) {
    return static_cast<HRESULT>(impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(Owner(self),&iid,out));
}
ULONG MS_ABI AddRef(void* self) { return impl__ExternalAddRef_CCmdTarget__QEAAKXZ(Owner(self)); }
ULONG MS_ABI Release(void* self) { return impl__ExternalRelease_CCmdTarget__QEAAKXZ(Owner(self)); }
void* const table[]={reinterpret_cast<void*>(Query),reinterpret_cast<void*>(AddRef),reinterpret_cast<void*>(Release),
    reinterpret_cast<void*>(impl__Next_XEnumVOID_CEnumArray__UEAAJKPEAXPEAK_Z),
    reinterpret_cast<void*>(impl__Skip_XEnumVOID_CEnumArray__UEAAJK_Z),
    reinterpret_cast<void*>(impl__Reset_XEnumVOID_CEnumArray__UEAAJXZ),
    reinterpret_cast<void*>(impl__Clone_XEnumVOID_CEnumArray__UEAAJPEAPEAUIEnumVOID___Z)};
const AFX_INTERFACEMAP* Map() {
    static const AFX_INTERFACEMAP_ENTRY entries[]={{&IID_IEnumOLEVERB,0x68},{nullptr,SIZE_MAX}};
    static const AFX_INTERFACEMAP map{impl__GetThisInterfaceMap_CCmdTarget__KAPEBUAFX_INTERFACEMAP__XZ,entries};return &map;
}
}
CEnumOleVerb::CEnumOleVerb() : m_elementSize(sizeof(OLEVERB)),m_clonedFrom(nullptr),m_verbs(nullptr),
    m_position(0),m_count(0),m_needFree(TRUE),m_enumPadding(0),m_enumTable(table) {}
CEnumOleVerb::~CEnumOleVerb() {
    if(m_clonedFrom)impl__InternalRelease_CCmdTarget__QEAAKXZ(m_clonedFrom);
    else for(ULONG i=0;i<m_count;++i)CoTaskMemFree(m_verbs[i].lpszVerbName);
    if(m_needFree)std::free(m_verbs);
}
const AFX_INTERFACEMAP* CEnumOleVerb::GetInterfaceMap() const { return Map(); }
IEnumOLEVERB* CEnumOleVerb::Interface() { return reinterpret_cast<IEnumOLEVERB*>(&m_enumTable); }
void CEnumOleVerb::SetVerbs(OLEVERB* verbs,ULONG count) { m_verbs=verbs;m_count=count;m_position=0; }
BOOL CEnumOleVerb::OnNext(void* out) {
    if(!impl__OnNext_CEnumArray__MEAAHPEAX_Z(this,out))return FALSE;
    auto* result=static_cast<OLEVERB*>(out);
    if(result->lpszVerbName) {
        size_t bytes=(wcslen(result->lpszVerbName)+1)*sizeof(wchar_t);
        auto* copy=static_cast<wchar_t*>(CoTaskMemAlloc(bytes));
        if(!copy) { impl__AfxThrowMemoryException__YAXXZ();return FALSE; }
        memcpy(copy,result->lpszVerbName,bytes);result->lpszVerbName=copy;
    }
    return TRUE;
}
BOOL CEnumOleVerb::OnSkip() { if(m_position>=m_count)return FALSE;return ++m_position<m_count; }
void CEnumOleVerb::OnReset() { m_position=0; }
// Retail inherits the generic clone: its Next copies entries shallowly.
void* CEnumOleVerb::OnClone() { return impl__OnClone_CEnumArray__MEAAPEAV1_XZ(this); }
// Symbol: ?OnNext@CEnumOleVerb@@MEAAHPEAX@Z
extern "C" int MS_ABI impl__OnNext_CEnumOleVerb__MEAAHPEAX_Z(CEnumOleVerb* self,void* out) {
    return self ? self->CEnumOleVerb::OnNext(out) : FALSE;
}
// Symbol: ?GetThisInterfaceMap@CEnumOleVerb@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CEnumOleVerb__KAPEBUAFX_INTERFACEMAP__XZ() { return Map(); }
// Symbol: ?GetInterfaceMap@CEnumOleVerb@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CEnumOleVerb__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*) { return Map(); }
