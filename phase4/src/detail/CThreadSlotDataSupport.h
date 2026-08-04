#pragma once
// Shared internals of the former global_cthreadslotdata.cpp translation unit.
// Definitions live in detail/CThreadSlotDataSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cthreadslotdata {} } }
using namespace openmfc::detail::cthreadslotdata;
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



// Get-or-create this thread's CThreadData node, linking it into m_list.
namespace openmfc { namespace detail { namespace cthreadslotdata {
CThreadData* GetOrCreateThreadData(CThreadSlotData* pThis);
} } }

// Ensure pData->pData can index [slot], zero-filling new entries.
namespace openmfc { namespace detail { namespace cthreadslotdata {
bool GrowThreadData(CThreadData* pData, int slot);
} } }










