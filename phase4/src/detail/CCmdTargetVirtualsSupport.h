#pragma once
#include "openmfc/afxwin.h"
#include "detail/InterfaceMapsSupport.h"
#include <cstddef>
// --- map structures ----------------------------------------------------------
// Transcribed from the shipping afxwin.h (_AFXDLL flavour, which is what
// mfc140u is built as: every pfnGetBaseMap slot below is a function pointer,
// and the disassembly indirect-calls through it).  AFX_INTERFACEMAP itself is
// already declared by detail/InterfaceMapsSupport.h and is reused as-is.
enum AFX_DISPMAP_FLAGS_ { afxDispCustom_ = 0, afxDispStock_ = 1 };

struct AFX_DISPMAP_ENTRY {
    const wchar_t*      lpszName;     // +0x00
    long                lDispID;      // +0x08
    const char*         lpszParams;   // +0x10
    unsigned short      vt;           // +0x18
    void*               pfn;          // +0x20
    void*               pfnSet;       // +0x28
    size_t              nPropOffset;  // +0x30
    AFX_DISPMAP_FLAGS_  flags;        // +0x38
};
static_assert(offsetof(AFX_DISPMAP_ENTRY, lDispID) == 0x08, "AFX_DISPMAP_ENTRY::lDispID at +0x08");
static_assert(offsetof(AFX_DISPMAP_ENTRY, vt) == 0x18, "AFX_DISPMAP_ENTRY::vt at +0x18");
static_assert(offsetof(AFX_DISPMAP_ENTRY, nPropOffset) == 0x30, "AFX_DISPMAP_ENTRY::nPropOffset at +0x30");
static_assert(sizeof(AFX_DISPMAP_ENTRY) == 0x40, "AFX_DISPMAP_ENTRY stride is 0x40 (GetEntryCount: add $0x40 / sar $0x6)");

struct AFX_DISPMAP {
    const AFX_DISPMAP* (AFXAPI* pfnGetBaseMap)();  // +0x00
    const AFX_DISPMAP_ENTRY*    lpEntries;         // +0x08
    unsigned int*               lpEntryCount;      // +0x10
    unsigned long*              lpStockPropMask;   // +0x18 (not read by anything here)
};

struct AFX_EVENTSINKMAP_ENTRY {
    AFX_DISPMAP_ENTRY dispEntry;     // +0x00
    unsigned int      nCtrlIDFirst;  // +0x40
    unsigned int      nCtrlIDLast;   // +0x44
};
static_assert(sizeof(AFX_EVENTSINKMAP_ENTRY) == 0x48, "AFX_EVENTSINKMAP_ENTRY stride is 0x48 (GetEventSinkEntry: add $0x48)");
static_assert(offsetof(AFX_EVENTSINKMAP_ENTRY, nCtrlIDFirst) == 0x40, "nCtrlIDFirst at +0x40");
static_assert(offsetof(AFX_EVENTSINKMAP_ENTRY, nCtrlIDLast) == 0x44, "nCtrlIDLast at +0x44");

struct AFX_EVENTSINKMAP {
    const AFX_EVENTSINKMAP* (AFXAPI* pfnGetBaseMap)();
    const AFX_EVENTSINKMAP_ENTRY*    lpEntries;
    unsigned int*                    lpEntryCount;
};

struct AFX_CONNECTIONMAP_ENTRY { const void* piid; size_t nOffset; };
struct AFX_CONNECTIONMAP {
    const AFX_CONNECTIONMAP* (AFXAPI* pfnGetBaseMap)();
    const AFX_CONNECTIONMAP_ENTRY*    pEntry;
};

struct AFX_OLECMDMAP_ENTRY { const GUID* pguid; unsigned long cmdID; unsigned int nID; };
struct AFX_OLECMDMAP {
    const AFX_OLECMDMAP* (AFXAPI* pfnGetBaseMap)();
    const AFX_OLECMDMAP_ENTRY*    lpEntries;
};


extern "C" void* MS_ABI impl__GetTypeLibCache_CCmdTarget__UEAAPEAVCTypeLibCache__XZ(CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetCommandMap_CCmdTarget__MEBAPEBUAFX_OLECMDMAP__XZ(const CCmdTarget* pThis);
extern "C" void* MS_ABI impl__GetConnectionHook_CCmdTarget__MEAAPEAUIConnectionPoint__AEBU_GUID___Z(CCmdTarget* pThis, const void* iid);
extern "C" const void* MS_ABI impl__GetConnectionMap_CCmdTarget__MEBAPEBUAFX_CONNECTIONMAP__XZ(const CCmdTarget* pThis);
extern "C" int MS_ABI impl__GetDispatchIID_CCmdTarget__UEAAHPEAU_GUID___Z(CCmdTarget* pThis, void* pIID);
extern "C" const void* MS_ABI impl__GetDispatchMap_CCmdTarget__MEBAPEBUAFX_DISPMAP__XZ(const CCmdTarget* pThis);
extern "C" const void* MS_ABI impl__GetEventSinkMap_CCmdTarget__MEBAPEBUAFX_EVENTSINKMAP__XZ(const CCmdTarget* pThis);
extern "C" int MS_ABI impl__GetExtraConnectionPoints_CCmdTarget__MEAAHPEAVCPtrArray___Z(CCmdTarget* pThis, void* pConnPoints);
extern "C" IUnknown* MS_ABI impl__GetInterface_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid);
extern "C" IUnknown* MS_ABI impl__GetInterfaceHook_CCmdTarget__UEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid);
extern "C" const void* MS_ABI impl__GetThisCommandMap_CCmdTarget__KAPEBUAFX_OLECMDMAP__XZ();
extern "C" const void* MS_ABI impl__GetThisConnectionMap_CCmdTarget__KAPEBUAFX_CONNECTIONMAP__XZ();
extern "C" const void* MS_ABI impl__GetThisDispatchMap_CCmdTarget__KAPEBUAFX_DISPMAP__XZ();
extern "C" const void* MS_ABI impl__GetThisEventSinkMap_CCmdTarget__KAPEBUAFX_EVENTSINKMAP__XZ();
extern "C" unsigned int MS_ABI impl__GetTypeInfoCount_CCmdTarget__UEAAIXZ(CCmdTarget* pThis);
extern "C" long MS_ABI impl__GetTypeLib_CCmdTarget__UEAAJKPEAPEAUITypeLib___Z(CCmdTarget* pThis, unsigned long lcid, void** ppTypeLib);
extern "C" int MS_ABI impl__IsInvokeAllowed_CCmdTarget__UEAAHJ_Z(CCmdTarget* pThis, long dispid);
extern "C" int MS_ABI impl__OnCreateAggregates_CCmdTarget__UEAAHXZ(CCmdTarget* pThis);
extern "C" void MS_ABI impl__OnFinalRelease_CCmdTarget__UEAAXXZ(CCmdTarget*);
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CCmdTarget__MEBAPEBUAFX_INTERFACEMAP__XZ(const void*);

extern "C" unsigned long MS_ABI impl__ExternalAddRef_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);

extern "C" unsigned long MS_ABI impl__InternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj);

extern "C" unsigned long MS_ABI impl__InternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);

extern "C" IUnknown* MS_ABI impl__QueryAggregates_CCmdTarget__QEAAPEAUIUnknown__PEBX_Z(CCmdTarget* pThis, const void* iid);
