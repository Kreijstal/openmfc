// Native x64 CPtrArray storage with the existing shared collection backend.
#include "detail/FilecoreSupport.h"
static_assert(sizeof(CPtrArray)==0x28, "native CPtrArray size");
IMPLEMENT_DYNAMIC(CPtrArray, CObject)
void CPtrArray::SyncNativeArray() {
    const auto* data=FindArrayStorage<CPtrArray,void*,void*>(this);
    m_pData=data ? const_cast<void**>(data->GetData()) : nullptr;
    m_nSize=data ? data->m_nSize : 0;
    m_nMaxSize=data ? data->m_nMaxSize : 0;
    m_nGrowBy=data ? data->m_nGrowBy : 0;
}
OPENMFC_DEFINE_ARRAY_METHODS(CPtrArray,void*,void*)
