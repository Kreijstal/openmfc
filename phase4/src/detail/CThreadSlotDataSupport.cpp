#include "CThreadSlotDataSupport.h"

static_assert(sizeof(CThreadSlotData) == 80, "CThreadSlotData must be 80 bytes");
static_assert(offsetof(CThreadSlotData, m_tlsIndex) == 0, "m_tlsIndex@0");
static_assert(offsetof(CThreadSlotData, m_nAlloc) == 4, "m_nAlloc@4");
static_assert(offsetof(CThreadSlotData, m_nRover) == 8, "m_nRover@8");
static_assert(offsetof(CThreadSlotData, m_nMax) == 12, "m_nMax@12");
static_assert(offsetof(CThreadSlotData, m_list) == 16, "m_list@16");
static_assert(offsetof(CThreadSlotData, m_pSlotData) == 32, "m_pSlotData@32");
static_assert(offsetof(CThreadSlotData, m_sect) == 40, "m_sect@40");
void* CThreadSlotData::GetThreadValue(int slot) {
    CThreadData* pData = (CThreadData*)TlsGetValue(m_tlsIndex);
    if (pData == nullptr || slot <= 0 || slot >= pData->nCount)
        return nullptr;
    return pData->pData[slot];
}
namespace openmfc { namespace detail { namespace cthreadslotdata {
CThreadData* GetOrCreateThreadData(CThreadSlotData* pThis) {
    CThreadData* pData = (CThreadData*)TlsGetValue(pThis->m_tlsIndex);
    if (pData == nullptr) {
        pData = (CThreadData*)calloc(1, sizeof(CThreadData));
        if (pData == nullptr)
            return nullptr;
        pData->pNext  = nullptr;
        pData->nCount = 0;
        pData->pData  = nullptr;
        TlsSetValue(pThis->m_tlsIndex, pData);
        // AddHead into m_list (m_nNextOffset == 0 == offsetof(pNext)).
        *(void**)((BYTE*)pData + pThis->m_list.m_nNextOffset) = pThis->m_list.m_pHead;
        pThis->m_list.m_pHead = pData;
    }
    return pData;
}
bool GrowThreadData(CThreadData* pData, int slot) {
    if (slot < pData->nCount)
        return true;
    int newCount = slot + 1;
    void** p = (void**)realloc(pData->pData, (size_t)newCount * sizeof(void*));
    if (p == nullptr)
        return false;
    for (int i = pData->nCount; i < newCount; ++i)
        p[i] = nullptr;
    pData->pData  = p;
    pData->nCount = newCount;
    return true;
}
} } }  // namespace openmfc::detail::cthreadslotdata
