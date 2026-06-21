// CThreadSlotData implementation for OpenMFC
//
// CThreadSlotData is a standalone, non-polymorphic MFC helper declared in the
// internal header afxtls_.h. It is NOT CObject-derived: there is no
// CRuntimeClass, no vtable, no Serialize. It manages a table of "slots" of
// per-thread data, backed by a TLS index. Each thread owns a CThreadData node
// (chained through m_list) holding a grow-on-demand array of slot values.
//
// MSVC x64 layout (sizeof == 80, align 8), from afxtls_.h:
//   off  0  DWORD            m_tlsIndex
//   off  4  int              m_nAlloc
//   off  8  int              m_nRover
//   off 12  int              m_nMax
//   off 16  CTypedSimpleList m_list      ({void* m_pHead; size_t m_nNextOffset})
//   off 32  CSlotData*       m_pSlotData
//   off 40  CRITICAL_SECTION m_sect      (40 bytes on x64)
//
// All 8 exported symbols are QEAA (public, non-virtual, x64 member): `this` is
// passed in RCX and MUST be the first parameter of each impl, mirroring
// collections_csimplelist.cpp.

#include <windows.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Per-slot bookkeeping entry (CSlotData in real MFC: dwFlags + module tag).
struct CSlotData {
    DWORD     dwFlags;     // SLOT_USED bit etc.
    HINSTANCE hInst;       // owning module for AssignInstance/DeleteValues
};

enum { SLOT_USED = 0x01 };

// Per-thread node stored under the TLS index. In real MFC CThreadData also
// carries a CNoTrackObject link (pNext) used to thread all per-thread nodes
// through m_list; m_nNextOffset == offsetof(CThreadData, pNext) == 0.
struct CThreadData {
    CThreadData* pNext;    // off 0: m_list chain link
    int          nCount;   // off 8: number of valid entries in pData
    void**       pData;    // off 16: grow-on-demand array of slot values
};

// 16-byte simple list, identical layout to CSimpleList, threading CThreadData.
struct CThreadSlotList {
    void*  m_pHead;
    size_t m_nNextOffset;
};

struct CThreadSlotData {
    DWORD            m_tlsIndex;   // off 0
    int              m_nAlloc;     // off 4
    int              m_nRover;     // off 8
    int              m_nMax;       // off 12
    CThreadSlotList  m_list;       // off 16
    CSlotData*       m_pSlotData;  // off 32
    CRITICAL_SECTION m_sect;       // off 40

    // Inline, UNEXPORTED in real MFC (defined in afxtls_.h). Provided here so
    // the test can round-trip values; not added to the .def.
    void* GetThreadValue(int slot);
};

static_assert(sizeof(CThreadSlotData) == 80, "CThreadSlotData must be 80 bytes");
static_assert(offsetof(CThreadSlotData, m_tlsIndex) == 0, "m_tlsIndex@0");
static_assert(offsetof(CThreadSlotData, m_nAlloc) == 4, "m_nAlloc@4");
static_assert(offsetof(CThreadSlotData, m_nRover) == 8, "m_nRover@8");
static_assert(offsetof(CThreadSlotData, m_nMax) == 12, "m_nMax@12");
static_assert(offsetof(CThreadSlotData, m_list) == 16, "m_list@16");
static_assert(offsetof(CThreadSlotData, m_pSlotData) == 32, "m_pSlotData@32");
static_assert(offsetof(CThreadSlotData, m_sect) == 40, "m_sect@40");

namespace {

// Get-or-create this thread's CThreadData node, linking it into m_list.
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

// Ensure pData->pData can index [slot], zero-filling new entries.
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

} // namespace

void* CThreadSlotData::GetThreadValue(int slot) {
    CThreadData* pData = (CThreadData*)TlsGetValue(m_tlsIndex);
    if (pData == nullptr || slot <= 0 || slot >= pData->nCount)
        return nullptr;
    return pData->pData[slot];
}

// Symbol: ??0CThreadSlotData@@QEAA@XZ
extern "C" void MS_ABI impl___0CThreadSlotData__QEAA_XZ(CThreadSlotData* pThis) {
    if (pThis == nullptr)
        return;
    pThis->m_tlsIndex = TlsAlloc();
    pThis->m_nAlloc   = 0;
    pThis->m_nRover   = 1;   // slot 0 is reserved; rover starts at 1
    pThis->m_nMax     = 0;
    pThis->m_list.m_pHead = nullptr;
    pThis->m_list.m_nNextOffset = offsetof(CThreadData, pNext); // == 0
    pThis->m_pSlotData = nullptr;
    InitializeCriticalSection(&pThis->m_sect);
}

// Symbol: ??1CThreadSlotData@@QEAA@XZ
extern "C" void MS_ABI impl___1CThreadSlotData__QEAA_XZ(CThreadSlotData* pThis) {
    if (pThis == nullptr)
        return;
    EnterCriticalSection(&pThis->m_sect);
    // Walk m_list, freeing each thread's CThreadData and its value array.
    void* p = pThis->m_list.m_pHead;
    const size_t off = pThis->m_list.m_nNextOffset;
    while (p != nullptr) {
        void* pNext = *(void**)((BYTE*)p + off);
        CThreadData* pData = (CThreadData*)p;
        free(pData->pData);
        free(pData);
        p = pNext;
    }
    pThis->m_list.m_pHead = nullptr;
    if (pThis->m_tlsIndex != TLS_OUT_OF_INDEXES)
        TlsFree(pThis->m_tlsIndex);
    pThis->m_tlsIndex = TLS_OUT_OF_INDEXES;
    free(pThis->m_pSlotData);
    pThis->m_pSlotData = nullptr;
    pThis->m_nAlloc = 0;
    pThis->m_nMax   = 0;
    LeaveCriticalSection(&pThis->m_sect);
    DeleteCriticalSection(&pThis->m_sect);
}

// Symbol: ?AllocSlot@CThreadSlotData@@QEAAHXZ
extern "C" int MS_ABI impl__AllocSlot_CThreadSlotData__QEAAHXZ(CThreadSlotData* pThis) {
    if (pThis == nullptr)
        return 0;
    EnterCriticalSection(&pThis->m_sect);

    int slot = 0;
    // Look for a freed slot in [1, m_nMax), starting at the rover cursor.
    if (pThis->m_pSlotData != nullptr) {
        for (int i = pThis->m_nRover; i < pThis->m_nMax; ++i) {
            if ((pThis->m_pSlotData[i].dwFlags & SLOT_USED) == 0) {
                slot = i;
                break;
            }
        }
        if (slot == 0) {
            for (int i = 1; i < pThis->m_nRover && i < pThis->m_nMax; ++i) {
                if ((pThis->m_pSlotData[i].dwFlags & SLOT_USED) == 0) {
                    slot = i;
                    break;
                }
            }
        }
    }

    if (slot == 0) {
        // No free slot reusable: allocate a brand new one at m_nMax (>=1).
        slot = (pThis->m_nMax < 1) ? 1 : pThis->m_nMax;
        if (slot >= pThis->m_nAlloc) {
            int newAlloc = slot + 8;  // grow with headroom
            CSlotData* p = (CSlotData*)realloc(
                pThis->m_pSlotData, (size_t)newAlloc * sizeof(CSlotData));
            if (p == nullptr) {
                LeaveCriticalSection(&pThis->m_sect);
                return 0;
            }
            for (int i = pThis->m_nAlloc; i < newAlloc; ++i) {
                p[i].dwFlags = 0;
                p[i].hInst   = nullptr;
            }
            pThis->m_pSlotData = p;
            pThis->m_nAlloc    = newAlloc;
        }
        if (slot + 1 > pThis->m_nMax)
            pThis->m_nMax = slot + 1;
    }

    pThis->m_pSlotData[slot].dwFlags |= SLOT_USED;
    pThis->m_nRover = slot + 1;
    LeaveCriticalSection(&pThis->m_sect);
    return slot;
}

// Symbol: ?AssignInstance@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AssignInstance_CThreadSlotData__QEAAXPEAUHINSTANCE_____Z(
    CThreadSlotData* pThis, HINSTANCE hInst) {
    if (pThis == nullptr)
        return;
    EnterCriticalSection(&pThis->m_sect);
    // Tag any currently-used-but-untagged slots with this module instance.
    for (int i = 1; i < pThis->m_nMax; ++i) {
        if ((pThis->m_pSlotData[i].dwFlags & SLOT_USED) &&
            pThis->m_pSlotData[i].hInst == nullptr) {
            pThis->m_pSlotData[i].hInst = hInst;
        }
    }
    LeaveCriticalSection(&pThis->m_sect);
}

// Symbol: ?FreeSlot@CThreadSlotData@@QEAAXH@Z
extern "C" void MS_ABI impl__FreeSlot_CThreadSlotData__QEAAXH_Z(
    CThreadSlotData* pThis, int slot) {
    if (pThis == nullptr || slot <= 0)
        return;
    EnterCriticalSection(&pThis->m_sect);
    if (pThis->m_pSlotData != nullptr && slot < pThis->m_nMax) {
        pThis->m_pSlotData[slot].dwFlags = 0;
        pThis->m_pSlotData[slot].hInst   = nullptr;
    }
    // Clear that slot's value in every thread's CThreadData.
    void* p = pThis->m_list.m_pHead;
    const size_t off = pThis->m_list.m_nNextOffset;
    while (p != nullptr) {
        CThreadData* pData = (CThreadData*)p;
        if (pData->pData != nullptr && slot < pData->nCount)
            pData->pData[slot] = nullptr;
        p = *(void**)((BYTE*)p + off);
    }
    if (slot < pThis->m_nRover)
        pThis->m_nRover = slot;  // prefer reusing the freed slot next
    LeaveCriticalSection(&pThis->m_sect);
}

// Symbol: ?SetValue@CThreadSlotData@@QEAAXHPEAX@Z
extern "C" void MS_ABI impl__SetValue_CThreadSlotData__QEAAXHPEAX_Z(
    CThreadSlotData* pThis, int slot, void* pValue) {
    if (pThis == nullptr || slot <= 0)
        return;
    EnterCriticalSection(&pThis->m_sect);
    CThreadData* pData = GetOrCreateThreadData(pThis);
    if (pData != nullptr && GrowThreadData(pData, slot))
        pData->pData[slot] = pValue;
    LeaveCriticalSection(&pThis->m_sect);
}

// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUHINSTANCE__@@H@Z
// Clears, for the calling thread, all slot values belonging to a module.
extern "C" void MS_ABI impl__DeleteValues_CThreadSlotData__QEAAXPEAUHINSTANCE____H_Z(
    CThreadSlotData* pThis, HINSTANCE hInst, int /*bAll*/) {
    if (pThis == nullptr)
        return;
    EnterCriticalSection(&pThis->m_sect);
    CThreadData* pData = (CThreadData*)TlsGetValue(pThis->m_tlsIndex);
    if (pData != nullptr && pData->pData != nullptr && pThis->m_pSlotData != nullptr) {
        int limit = pData->nCount < pThis->m_nMax ? pData->nCount : pThis->m_nMax;
        for (int i = 1; i < limit; ++i) {
            if (hInst == nullptr || pThis->m_pSlotData[i].hInst == hInst)
                pData->pData[i] = nullptr;
        }
    }
    LeaveCriticalSection(&pThis->m_sect);
}

// Symbol: ?DeleteValues@CThreadSlotData@@QEAAXPEAUCThreadData@@PEAUHINSTANCE__@@@Z
// Clears slot values for a specific thread's CThreadData, optionally scoped to
// a module. When pData is null, falls back to the calling thread's node.
extern "C" void MS_ABI impl__DeleteValues_CThreadSlotData__QEAAXPEAUCThreadData__PEAUHINSTANCE_____Z(
    CThreadSlotData* pThis, CThreadData* pData, HINSTANCE hInst) {
    if (pThis == nullptr)
        return;
    EnterCriticalSection(&pThis->m_sect);
    if (pData == nullptr)
        pData = (CThreadData*)TlsGetValue(pThis->m_tlsIndex);
    if (pData != nullptr && pData->pData != nullptr && pThis->m_pSlotData != nullptr) {
        int limit = pData->nCount < pThis->m_nMax ? pData->nCount : pThis->m_nMax;
        for (int i = 1; i < limit; ++i) {
            if (hInst == nullptr || pThis->m_pSlotData[i].hInst == hInst)
                pData->pData[i] = nullptr;
        }
    }
    LeaveCriticalSection(&pThis->m_sect);
}
