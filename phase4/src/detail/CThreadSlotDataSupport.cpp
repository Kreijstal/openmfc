#include "CThreadSlotDataSupport.h"
static_assert(sizeof(CSlotData) == 16 && offsetof(CSlotData, hInst) == 8,
              "native TLS slot metadata");
static_assert(sizeof(CThreadData) == 32 && offsetof(CThreadData, pNext) == 8 &&
                  offsetof(CThreadData, nCount) == 16 && offsetof(CThreadData, pData) == 24,
              "native TLS thread node");
static_assert(sizeof(CThreadSlotData) == 80 && offsetof(CThreadSlotData, m_pSlotData) == 16 &&
                  offsetof(CThreadSlotData, m_list) == 24 &&
                  offsetof(CThreadSlotData, m_sect) == 40,
              "native TLS slot storage");
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
namespace openmfc {
namespace detail {
namespace cthreadslotdata {
void RemoveThreadNode(CThreadSlotList& list, CThreadData* node) {
    auto next = [&](void* p) -> void*& {
        return *reinterpret_cast<void**>(static_cast<BYTE*>(p) + list.m_nNextOffset);
    };
    void* current = list.m_pHead;
    if (!current)
        return;
    if (current == node) {
        if (!node)
            impl__AfxThrowInvalidArgException__YAXXZ();
        list.m_pHead = next(node);
        return;
    }
    while (current) {
        void* following = next(current);
        if (following == node) {
            if (!node)
                impl__AfxThrowInvalidArgException__YAXXZ();
            next(current) = next(node);
            return;
        }
        current = following;
    }
}
} // namespace cthreadslotdata
} // namespace detail
} // namespace openmfc
void* CThreadSlotData::GetThreadValue(int slot) {
    ::EnterCriticalSection(&m_sect);
    void* value = nullptr;
    if (slot > 0 && slot < m_nMax) {
        auto* data = static_cast<CThreadData*>(::TlsGetValue(m_tlsIndex));
        if (data && data->pData && slot < data->nCount)
            value = data->pData[slot];
    }
    ::LeaveCriticalSection(&m_sect);
    return value;
}
