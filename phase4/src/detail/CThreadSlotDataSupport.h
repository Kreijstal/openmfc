#pragma once
#include "openmfc/afxstat_.h"
#include <cstddef>
#include <cstdint>
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
// Release-x64 layouts read from mfc140u 0x14c8b0..0x14cf39.
struct CSlotData {
    DWORD dwFlags;
    HINSTANCE hInst;
};
enum { SLOT_USED = 1 };
struct CThreadData : CNoTrackObject {
    CThreadData* pNext;
    int nCount;
    void** pData;
};
struct CThreadSlotList {
    void* m_pHead;
    size_t m_nNextOffset;
};
struct CThreadSlotData {
    DWORD m_tlsIndex;
    int m_nAlloc, m_nRover, m_nMax;
    CSlotData* m_pSlotData;
    CThreadSlotList m_list;
    CRITICAL_SECTION m_sect;
    CThreadSlotData();
    ~CThreadSlotData();
    int AllocSlot();
    void FreeSlot(int);
    void SetValue(int, void*);
    void AssignInstance(HINSTANCE);
    void DeleteValues(CThreadData*, HINSTANCE);
    void DeleteValues(HINSTANCE, BOOL);
    void* GetThreadValue(int);
};
namespace openmfc {
namespace detail {
namespace cthreadslotdata {
void RemoveThreadNode(CThreadSlotList&, CThreadData*);
}
} // namespace detail
} // namespace openmfc
