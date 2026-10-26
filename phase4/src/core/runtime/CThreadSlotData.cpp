// Native release-x64 slot allocation, module ownership and value destruction.
#define OPENMFC_APPCORE_IMPL
#include "detail/CThreadSlotDataSupport.h"
#include <cstring>
extern "C" {
void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long);
}
namespace {
[[noreturn]] void Overflow() {
    impl__AfxThrowOleException__YAXJ_Z(long(0x80070216u));
    __assume(0);
}
[[noreturn]] void Memory(CRITICAL_SECTION* section) {
    ::LeaveCriticalSection(section);
    impl__AfxThrowMemoryException__YAXXZ();
    __assume(0);
}
int Add(int a, UINT b) { return static_cast<int>(static_cast<UINT>(a) + b); }
} // namespace
CThreadSlotData::CThreadSlotData() {
    m_list.m_nNextOffset = 8;
    m_list.m_pHead = nullptr;
    m_nAlloc = 0;
    m_pSlotData = nullptr;
    m_nRover = 1;
    m_nMax = 0;
    m_tlsIndex = ::TlsAlloc();
    if (m_tlsIndex == TLS_OUT_OF_INDEXES)
        impl__AfxThrowMemoryException__YAXXZ();
    ::InitializeCriticalSection(&m_sect);
}
CThreadSlotData::~CThreadSlotData() {
    auto* data = static_cast<CThreadData*>(m_list.m_pHead);
    while (data) {
        auto* next = data->pNext;
        DeleteValues(data, nullptr);
        data = next;
    }
    if (m_tlsIndex != TLS_OUT_OF_INDEXES)
        ::TlsFree(m_tlsIndex);
    if (m_pSlotData)
        if (HGLOBAL block = ::GlobalHandle(m_pSlotData)) {
            ::GlobalUnlock(block);
            ::GlobalFree(block);
        }
    ::DeleteCriticalSection(&m_sect);
}
int CThreadSlotData::AllocSlot() {
    // Native failures release the lock only on the explicit memory-error
    // paths. API exceptions and size-overflow exceptions leave it held.
    auto* section = &m_sect;
    ::EnterCriticalSection(section);
    int slot = m_nRover, allocated = m_nAlloc;
    bool available = false;
    if (slot < allocated) {
        if (!m_pSlotData)
            Memory(section);
        available = !(m_pSlotData[slot].dwFlags & SLOT_USED);
    }
    if (!available) {
        slot = 1;
        if (allocated > 1) {
            if (!m_pSlotData)
                Memory(section);
            while (slot < allocated && (m_pSlotData[slot].dwFlags & SLOT_USED))
                ++slot;
        }
        if (slot >= allocated) {
            int capacity = Add(allocated, 32);
            HGLOBAL block = nullptr;
            auto bytes = [&]() -> DWORD {
                UINT64 size = UINT64(static_cast<UINT>(capacity)) * sizeof(CSlotData);
                if (size > UINT_MAX)
                    Overflow();
                return static_cast<DWORD>(size);
            };
            if (!m_pSlotData)
                block = ::GlobalAlloc(GMEM_MOVEABLE, bytes());
            else {
                block = ::GlobalHandle(m_pSlotData);
                if (block) {
                    ::GlobalUnlock(block);
                    block = ::GlobalReAlloc(block, bytes(), GMEM_MOVEABLE | GMEM_DDESHARE);
                }
            }
            if (!block) {
                if (m_pSlotData)
                    if (HGLOBAL previous = ::GlobalHandle(m_pSlotData))
                        ::GlobalLock(previous);
                Memory(section);
            }
            auto* values = static_cast<CSlotData*>(::GlobalLock(block));
            if (!values)
                Memory(section);
            std::memset(values + m_nAlloc, 0,
                        size_t(static_cast<INT64>(Add(capacity, 0u - UINT(m_nAlloc)))) *
                            sizeof(CSlotData));
            m_nAlloc = capacity;
            m_pSlotData = values;
        }
    }
    int next = Add(slot, 1);
    if (slot >= m_nMax)
        m_nMax = next;
    m_pSlotData[slot].dwFlags |= SLOT_USED;
    m_nRover = next;
    ::LeaveCriticalSection(section);
    return slot;
}
void CThreadSlotData::FreeSlot(int slot) {
    ::EnterCriticalSection(&m_sect);
    if (slot > 0 && slot < m_nMax) {
        auto* data = static_cast<CThreadData*>(m_list.m_pHead);
        while (data) {
            if (slot < data->nCount) {
                auto* value = static_cast<CNoTrackObject*>(data->pData[slot]);
                if (value)
                    delete value;
                data->pData[slot] = nullptr;
            }
            data = data->pNext;
        }
        m_pSlotData[slot].dwFlags &= ~DWORD(SLOT_USED);
    }
    ::LeaveCriticalSection(&m_sect);
}
void CThreadSlotData::SetValue(int slot, void* value) {
    auto* section = &m_sect;
    ::EnterCriticalSection(section);
    if (slot > 0 && slot < m_nMax) {
        auto* data = static_cast<CThreadData*>(::TlsGetValue(m_tlsIndex));
        bool grow = false;
        if (data)
            grow = slot >= data->nCount && value;
        else {
            data = new CThreadData;
            data->nCount = 0;
            data->pData = nullptr;
            *reinterpret_cast<void**>(reinterpret_cast<BYTE*>(data) + m_list.m_nNextOffset) =
                m_list.m_pHead;
            m_list.m_pHead = data;
            grow = true;
        }
        if (grow) {
            UINT64 bytes = UINT64(static_cast<UINT>(m_nMax)) * sizeof(void*);
            if (bytes > UINT_MAX)
                Overflow();
            HLOCAL values =
                data->pData ? ::LocalReAlloc(data->pData, static_cast<DWORD>(bytes), LMEM_MOVEABLE)
                            : ::LocalAlloc(LMEM_FIXED, static_cast<DWORD>(bytes));
            if (!values)
                Memory(section);
            // Allocator callbacks can change the maximum or the node count;
            // native MFC reads both again before initializing the new tail.
            data->pData = static_cast<void**>(values);
            std::memset(data->pData + data->nCount, 0,
                        size_t(static_cast<INT64>(Add(m_nMax, 0u - UINT(data->nCount)))) *
                            sizeof(void*));
            data->nCount = m_nMax;
            ::TlsSetValue(m_tlsIndex, data);
        }
        if (data->pData && slot < data->nCount)
            data->pData[slot] = value;
    }
    ::LeaveCriticalSection(section);
}
void CThreadSlotData::AssignInstance(HINSTANCE module) {
    ::EnterCriticalSection(&m_sect);
    for (int slot = 1; slot < m_nMax; ++slot)
        if (!m_pSlotData[slot].hInst && (m_pSlotData[slot].dwFlags & SLOT_USED))
            m_pSlotData[slot].hInst = module;
    ::LeaveCriticalSection(&m_sect);
}
void CThreadSlotData::DeleteValues(CThreadData* data, HINSTANCE module) {
    bool empty = true;
    for (int slot = 1; slot < data->nCount; ++slot) {
        if (!module || m_pSlotData[slot].hInst == module) {
            auto* value = static_cast<CNoTrackObject*>(data->pData[slot]);
            if (value)
                delete value;
            data->pData[slot] = nullptr;
        } else if (data->pData[slot])
            empty = false;
    }
    if (empty) {
        ::EnterCriticalSection(&m_sect);
        openmfc::detail::cthreadslotdata::RemoveThreadNode(m_list, data);
        ::LeaveCriticalSection(&m_sect);
        ::LocalFree(data->pData);
        delete data;
        // This clears the calling thread's TLS value even when deleting a
        // node belonging to another thread, as the native routine does.
        ::TlsSetValue(m_tlsIndex, nullptr);
    }
}
void CThreadSlotData::DeleteValues(HINSTANCE module, BOOL all) {
    ::EnterCriticalSection(&m_sect);
    if (!all) {
        if (auto* data = static_cast<CThreadData*>(::TlsGetValue(m_tlsIndex)))
            DeleteValues(data, module);
    } else {
        auto* data = static_cast<CThreadData*>(m_list.m_pHead);
        while (data) {
            auto* next = data->pNext;
            DeleteValues(data, module);
            data = next;
        }
    }
    ::LeaveCriticalSection(&m_sect);
}
// Symbol: ??0CThreadSlotData@@QEAA@XZ
extern "C" CThreadSlotData* MS_ABI impl___0CThreadSlotData__QEAA_XZ(CThreadSlotData* self) {
    return ::new (self) CThreadSlotData;
}
// Symbol: ??1CThreadSlotData@@QEAA@XZ
extern "C" void MS_ABI impl___1CThreadSlotData__QEAA_XZ(CThreadSlotData* self) {
    self->~CThreadSlotData();
}
// Symbol: ?AllocSlot@CThreadSlotData@@QEAAHXZ
extern "C" int MS_ABI impl__AllocSlot_CThreadSlotData__QEAAHXZ(CThreadSlotData* self) {
    return self->AllocSlot();
}
// Symbol: ?FreeSlot@CThreadSlotData@@QEAAXH@Z
extern "C" void MS_ABI impl__FreeSlot_CThreadSlotData__QEAAXH_Z(CThreadSlotData* self, int slot) {
    self->FreeSlot(slot);
}
// Symbol: ?SetValue@CThreadSlotData@@QEAAXHPEAX@Z
extern "C" void MS_ABI impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(CThreadSlotData* self, int slot,
                                                                    void* value) {
    self->SetValue(slot, value);
}
// Symbol: ?AssignInstance@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(
    CThreadSlotData* self, HINSTANCE module) {
    self->AssignInstance(module);
}
// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUCThreadData@@PEAUHINSTANCE__@@@Z
extern "C" void MS_ABI
impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z(CThreadSlotData* self,
                                                                              CThreadData* data,
                                                                              HINSTANCE module) {
    self->DeleteValues(data, module);
}
// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@H@Z
extern "C" void MS_ABI impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(
    CThreadSlotData* self, HINSTANCE module, BOOL all) {
    self->DeleteValues(module, all);
}
