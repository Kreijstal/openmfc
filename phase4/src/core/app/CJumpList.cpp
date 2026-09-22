// CJumpList — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp (placeholders), rewritten from retail.
//
// The Windows 7 jump-list wrapper (retail declaration: atlmfc/include/afxadv.h, class
// CJumpList).  It is NOT a CObject and has no vtable.  OpenMFC has no public declaration
// of it, so -- like featurepack/ribbon/CMFCRibbonInfo.cpp -- the thunks take `void* pThis`
// and work on the layout view S_CJumpList pinned below.
//
// Every body here was transcribed from the retail mfc140u.dll disassembly (the method
// described in the header of core/ole/COleControl.cpp).  Each function cites the entry
// RVA it was read from; all RVAs in this file are mfc140u.dll RVAs.  Where a body departs
// from retail the comment says so and why.
//
// ---- layout (read from the retail constructor ??0CJumpList@@QEAA@H@Z, RVA 0x72090) ----
//   +0x00 CString  m_strAppID          ctor: nil string from the string manager
//   +0x08 BOOL     m_bInitialized      ctor: the qword store at +0x08 zeroes +0x08 and +0x0c
//   +0x0c BOOL     m_bIsSupported
//   +0x10 UINT     m_nMaxSlots         ctor: 0
//   +0x14 BOOL     m_bAutoCommit       ctor: the bAutoCommit argument
//   +0x18 CComPtr<IObjectArray>           m_removedItemsPtr   ctor: NULL
//   +0x20 CComPtr<ICustomDestinationList> m_destListPtr       ctor: NULL
//   +0x28 CComPtr<IObjectCollection>      m_tasksPtr          ctor: NULL
//   +0x30 CMap<CString,LPCTSTR,IObjectCollection*,IObjectCollection*> m_mapDestinations
//         +0x30 vfptr, +0x38 m_pHashTable (NULL), +0x40 m_nHashTableSize (17),
//         +0x48 m_nCount (0), +0x50 m_pFreeList, +0x58 m_pBlocks, +0x60 m_nBlockSize (10)
//   sizeof == 0x68
// That is exactly the member order of afxadv.h.  Which BOOL is which was settled from the
// code, not only from the declaration order: InitializeList (0x72230) sets +0x0c as soon
// as the OS check passes and sets +0x08 only after BeginList succeeds, and ClearAll
// (0x721c0) clears only +0x08.
//
// ---- m_mapDestinations -------------------------------------------------------------
// The map is a CMap<> *template* instantiation; retail compiles its members as local,
// non-exported helpers (GetAssocAt 0x314e0, SetAt 0x310c4, GetNextAssoc 0x73208,
// RemoveAll 0x3a400).  OpenMFC's own CMap template (include/openmfc/afx.h) and its
// CMapStringToPtr (a side-table keyed by `this`, with a struct POSITION and no exported
// GetStartPosition) have neither retail's layout nor an enumeration entry point reachable
// through impl__ thunks, so the four helpers are transcribed below (JL_Map*) over the
// retail layout.  CAssoc layout, read from those helpers: +0x00 CString key,
// +0x08 IObjectCollection* value, +0x10 pNext, +0x18 UINT nHashValue; sizeof 0x20.
// DEVIATION: retail's constructor stores the (unexported) CMap<...> vftable at +0x30; no
// retail CJumpList code dispatches through it (the destructor calls RemoveAll 0x3a400
// directly), and OpenMFC has no equivalent table, so nullptr is stored there instead.
//
// ---- CString members ---------------------------------------------------------------
// m_strAppID and the map keys are ATL-layout CStrings (one m_pszData pointer with a
// CStringData header before it); they are driven through detail/AtlStrTCore.h's AtlStrT,
// exactly as detail/CAppDestinationsSupport.cpp does for CAppDestinations::m_strAppID.
//
// ---- COM ---------------------------------------------------------------------------
// Interfaces are dispatched through their vtables by slot number (ComCall below); every
// slot is retail's call-site byte offset / 8.  The CLSID/IID/PROPERTYKEY values were read
// out of the retail image at the addresses each call site loads, and are defined locally
// so this file does not depend on libuuid carrying the Windows 7 shell GUIDs.

#include "detail/AtlStrTCore.h"      // <windows.h>, openmfc/afxstr.h, AtlStrT<wchar_t>

#include <objbase.h>
#include <propidl.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

using openmfc_str::AtlStrT;

// Forward declarations of this file's own thunks (the destructor and several methods
// call siblings that are defined further down).  Signatures follow the mangled names.
extern "C" int   MS_ABI impl__InitializeList_CJumpList__QEAAHXZ(void* pThis);
extern "C" int   MS_ABI impl__CommitList_CJumpList__QEAAHXZ(void* pThis);
extern "C" void  MS_ABI impl__ClearAll_CJumpList__QEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__ClearAllDestinations_CJumpList__QEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__CheckRemovedItems_CJumpList__IEAAPEAUIObjectCollection__PEAU2__Z(void* pThis, void* pColl);
extern "C" void* MS_ABI impl__GetObjectCollection_CJumpList__IEAAPEAUIObjectCollection__PEB_W_Z(void* pThis, const wchar_t* lpcszCategoryName);
extern "C" void  MS_ABI impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(void* pThis, void* pColl, int bReleaseItemsOnly);

namespace {

// ---- pinned layout -----------------------------------------------------------------
struct JL_Assoc {
    wchar_t*  key;          // +0x00 CString (m_pszData)
    void*     value;        // +0x08 IObjectCollection*
    JL_Assoc* pNext;        // +0x10
    UINT      nHashValue;   // +0x18
};
static_assert(sizeof(JL_Assoc) == 0x20, "CMap<CString,...>::CAssoc is 0x20 bytes (NewAssoc in SetAt 0x310c4)");
static_assert(offsetof(JL_Assoc, value) == 0x08, "assoc value @+0x08 (GetAssocAt caller 0x72910)");
static_assert(offsetof(JL_Assoc, pNext) == 0x10, "assoc pNext @+0x10 (GetAssocAt 0x314e0)");
static_assert(offsetof(JL_Assoc, nHashValue) == 0x18, "assoc nHashValue @+0x18 (GetAssocAt 0x314e0)");

struct JL_Plex { JL_Plex* pNext; };   // CPlex header; the assoc blocks follow it

struct JL_Map {
    void*      vfptr;             // +0x00 (see DEVIATION in the file header)
    JL_Assoc** m_pHashTable;      // +0x08
    UINT       m_nHashTableSize;  // +0x10
    INT_PTR    m_nCount;          // +0x18
    JL_Assoc*  m_pFreeList;       // +0x20
    JL_Plex*   m_pBlocks;         // +0x28
    INT_PTR    m_nBlockSize;      // +0x30
};
static_assert(sizeof(JL_Map) == 0x38, "CMap is 0x38 bytes");
static_assert(offsetof(JL_Map, m_pHashTable) == 0x08, "m_pHashTable");
static_assert(offsetof(JL_Map, m_nHashTableSize) == 0x10, "m_nHashTableSize");
static_assert(offsetof(JL_Map, m_nCount) == 0x18, "m_nCount");
static_assert(offsetof(JL_Map, m_pFreeList) == 0x20, "m_pFreeList");
static_assert(offsetof(JL_Map, m_pBlocks) == 0x28, "m_pBlocks");
static_assert(offsetof(JL_Map, m_nBlockSize) == 0x30, "m_nBlockSize");

struct S_CJumpList {
    wchar_t* m_strAppID;          // +0x00 CString
    BOOL     m_bInitialized;      // +0x08
    BOOL     m_bIsSupported;      // +0x0c
    UINT     m_nMaxSlots;         // +0x10
    BOOL     m_bAutoCommit;       // +0x14
    void*    m_removedItemsPtr;   // +0x18 CComPtr<IObjectArray>
    void*    m_destListPtr;       // +0x20 CComPtr<ICustomDestinationList>
    void*    m_tasksPtr;          // +0x28 CComPtr<IObjectCollection>
    JL_Map   m_mapDestinations;   // +0x30
};
static_assert(offsetof(S_CJumpList, m_bInitialized) == 0x08, "m_bInitialized @+0x08 (ctor 0x72090)");
static_assert(offsetof(S_CJumpList, m_bIsSupported) == 0x0c, "m_bIsSupported @+0x0c (InitializeList 0x72230)");
static_assert(offsetof(S_CJumpList, m_nMaxSlots) == 0x10, "m_nMaxSlots @+0x10 (GetMaxSlots 0x723c0)");
static_assert(offsetof(S_CJumpList, m_bAutoCommit) == 0x14, "m_bAutoCommit @+0x14 (ctor 0x72090)");
static_assert(offsetof(S_CJumpList, m_removedItemsPtr) == 0x18, "m_removedItemsPtr @+0x18");
static_assert(offsetof(S_CJumpList, m_destListPtr) == 0x20, "m_destListPtr @+0x20");
static_assert(offsetof(S_CJumpList, m_tasksPtr) == 0x28, "m_tasksPtr @+0x28");
static_assert(offsetof(S_CJumpList, m_mapDestinations) == 0x30, "m_mapDestinations @+0x30");
static_assert(sizeof(S_CJumpList) == 0x68, "sizeof(CJumpList) == 0x68");

inline S_CJumpList* Self(void* p) { return static_cast<S_CJumpList*>(p); }
inline AtlStrT<wchar_t>* Str(wchar_t** pp) { return reinterpret_cast<AtlStrT<wchar_t>*>(pp); }

// ---- COM dispatch by vtable slot ---------------------------------------------------
template <typename R, typename... A>
inline R ComCall(void* obj, unsigned slot, A... args) {
    using Fn = R (STDMETHODCALLTYPE*)(void*, A...);
    return reinterpret_cast<Fn>((*static_cast<void***>(obj))[slot])(obj, args...);
}
inline ULONG ComAddRef(void* p)  { return ComCall<ULONG>(p, 1); }   // IUnknown +0x08
inline ULONG ComRelease(void* p) { return ComCall<ULONG>(p, 2); }   // IUnknown +0x10
inline HRESULT ComQI(void* p, const GUID& iid, void** ppv) { return ComCall<HRESULT>(p, 0, &iid, ppv); }

// IObjectArray / IObjectCollection
enum : unsigned { kOA_GetCount = 3, kOA_GetAt = 4, kOC_AddObject = 5, kOC_AddFromArray = 6 };
// ICustomDestinationList
enum : unsigned { kCDL_SetAppID = 3, kCDL_BeginList = 4, kCDL_AppendCategory = 5,
                  kCDL_AppendKnownCategory = 6, kCDL_AddUserTasks = 7, kCDL_CommitList = 8 };
// IShellLinkW
enum : unsigned { kSL_SetArguments = 11, kSL_SetIconLocation = 17, kSL_SetPath = 20 };
// IPropertyStore
enum : unsigned { kPS_SetValue = 6, kPS_Commit = 7 };
// IShellItem
enum : unsigned { kSI_Compare = 7 };

// ---- GUIDs, read from the retail image at the addresses the call sites load ------------
struct JL_PropKey { GUID fmtid; DWORD pid; };
const GUID kIID_IUnknown = {0x00000000, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};                      // 0x2d9a58
const GUID kIID_IShellItem = {0x43826d1e, 0xe718, 0x42ee, {0xbc, 0x55, 0xa1, 0xe2, 0x61, 0xc3, 0x7b, 0xfe}};                    // 0x2d9958
const GUID kCLSID_ShellLink = {0x00021401, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};                   // 0x2d9998
const GUID kIID_IShellLinkW = {0x000214f9, 0x0000, 0x0000, {0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46}};                   // 0x33f6a0
const GUID kIID_IPropertyStore = {0x886d8eeb, 0x8cf2, 0x4446, {0x8d, 0x02, 0xcd, 0xba, 0x1d, 0xbd, 0xcf, 0x99}};                // 0x33f6b0
const GUID kCLSID_DestinationList = {0x77f10cf0, 0x3db5, 0x4966, {0xb5, 0x20, 0xb7, 0xc5, 0x4f, 0xd3, 0x5e, 0xd6}};             // 0x2d99d8
const GUID kIID_ICustomDestinationList = {0x6332debf, 0x87b5, 0x4670, {0x90, 0xc0, 0x5e, 0x57, 0xb4, 0x08, 0xa4, 0x9e}};        // 0x33f670
const GUID kCLSID_EnumerableObjectCollection = {0x2d3468c1, 0x36a7, 0x43b6, {0xac, 0x24, 0xd3, 0xf0, 0x2f, 0xd9, 0x60, 0x7a}};  // 0x2d99f8
const GUID kIID_IObjectCollection = {0x5632b1a4, 0xe38a, 0x400a, {0x92, 0x8a, 0xd4, 0xcd, 0x63, 0x23, 0x02, 0x95}};             // 0x2d9a18 / 0x33f680
const GUID kIID_IObjectArray = {0x92ca9dcd, 0x5622, 0x4bba, {0xa8, 0x05, 0x5e, 0x9f, 0x54, 0x1b, 0xd8, 0xc9}};                  // 0x2d9a08
const JL_PropKey kPKEY_Title = {{0xf29f85e0, 0x4ff9, 0x1068, {0xab, 0x91, 0x08, 0x00, 0x2b, 0x27, 0xb3, 0xd9}}, 2};                 // 0x2d9a28
const JL_PropKey kPKEY_AppUserModel_IsDestListSeparator = {{0x9f4c2855, 0x9f79, 0x4b39, {0xa8, 0xd0, 0xe1, 0xd4, 0x2d, 0xe1, 0xd5, 0xf3}}, 6}; // 0x2d9a40

constexpr DWORD kClsCtxInprocServer = 1;   // CLSCTX_INPROC_SERVER, the r8d=1 at every call site

// ---- CMap<CString,LPCTSTR,IObjectCollection*,IObjectCollection*> helpers ----------------
// ??$HashKey@PEB_W@@YAIPEB_W@Z (RVA 0x2731c0): FNV-1 order (multiply by 0x01000193, THEN
// xor the character -- the imul precedes the xor in the loop at 0x2731f9), seeded with
// 0x811c9dc5, over at most 10 sampled characters -- step = len/10 + 1.  (Retail throws on a NULL key; every caller here has
// already rejected NULL: it calls AfxThrowInvalidArgException, 0x227720.)
UINT JL_HashKey(const wchar_t* key) {
    UINT nHash = 0x811c9dc5u;
    const UINT len = static_cast<UINT>(std::wcslen(key));
    const UINT step = len / 10 + 1;
    for (UINT i = 0; i < len; i += step) nHash = (nHash * 0x01000193u) ^ static_cast<unsigned short>(key[i]);
    return nHash;
}

// GetAssocAt (RVA 0x314e0): hash, bucket = hash % m_nHashTableSize, then walk the chain
// comparing nHashValue and wcscmp(key) == 0 (CompareElements, 0x31558).
JL_Assoc* JL_MapGetAssocAt(JL_Map* m, const wchar_t* key, UINT& nHashBucket, UINT& nHashValue) {
    nHashValue = JL_HashKey(key);
    nHashBucket = nHashValue % m->m_nHashTableSize;
    if (m->m_pHashTable == nullptr) return nullptr;
    for (JL_Assoc* p = m->m_pHashTable[nHashBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHashValue && std::wcscmp(p->key, key) == 0) return p;
    }
    return nullptr;
}

// SetAt (RVA 0x310c4): GetAssocAt; if absent, allocate the hash table on first use
// (zeroed), take an assoc off the free list -- refilling it from a new CPlex block of
// m_nBlockSize assocs (CPlex::Create, 0x271300) chained last-to-first -- zero it,
// construct its key CString, ++m_nCount, link it at the head of its bucket.  Then store
// the value.  Retail allocates through MFC's operator new (0x27f0, malloc + new-handler
// loop) and frees with free() (RemoveAll 0x3a400); malloc/free are used here, and an
// allocation failure returns without inserting where retail throws.
void JL_MapSetAt(JL_Map* m, const wchar_t* key, void* value) {
    UINT nHashBucket = 0, nHashValue = 0;
    JL_Assoc* pAssoc = JL_MapGetAssocAt(m, key, nHashBucket, nHashValue);
    if (pAssoc == nullptr) {
        if (m->m_pHashTable == nullptr) {
            const UINT n = m->m_nHashTableSize;
            m->m_pHashTable = static_cast<JL_Assoc**>(std::malloc(sizeof(JL_Assoc*) * n));
            if (m->m_pHashTable == nullptr) return;
            std::memset(m->m_pHashTable, 0, sizeof(JL_Assoc*) * n);
            m->m_nHashTableSize = n;
        }
        if (m->m_pFreeList == nullptr) {
            const INT_PTR nBlock = m->m_nBlockSize;
            JL_Plex* pNew = static_cast<JL_Plex*>(std::malloc(sizeof(JL_Plex) + sizeof(JL_Assoc) * nBlock));
            if (pNew == nullptr) return;
            pNew->pNext = m->m_pBlocks;
            m->m_pBlocks = pNew;
            JL_Assoc* pFirst = reinterpret_cast<JL_Assoc*>(pNew + 1);
            for (INT_PTR i = nBlock - 1; i >= 0; --i) {
                pFirst[i].pNext = m->m_pFreeList;
                m->m_pFreeList = &pFirst[i];
            }
        }
        pAssoc = m->m_pFreeList;
        JL_Assoc* pNextFree = pAssoc->pNext;
        std::memset(pAssoc, 0, sizeof(JL_Assoc));
        pAssoc->pNext = pNextFree;
        m->m_pFreeList = pAssoc->pNext;
        ++m->m_nCount;
        Str(&pAssoc->key)->ConstructPsz(key);
        pAssoc->nHashValue = nHashValue;
        pAssoc->pNext = m->m_pHashTable[nHashBucket];
        m->m_pHashTable[nHashBucket] = pAssoc;
    }
    pAssoc->value = value;
}

// GetStartPosition (inline in afxtempl.h): m_nCount ? BEFORE_START_POSITION : NULL.
inline void* JL_MapGetStartPosition(const JL_Map* m) {
    return m->m_nCount != 0 ? reinterpret_cast<void*>(static_cast<INT_PTR>(-1)) : nullptr;
}

// GetNextAssoc (RVA 0x73208).  Retail copies the key into the caller's CString; the
// callers here only read it while the map is unchanged, so the assoc itself is returned.
// Retail calls AfxThrowInvalidArgException (0x227720) on a NULL table/position or an
// empty table; this returns NULL instead.
JL_Assoc* JL_MapGetNextAssoc(const JL_Map* m, void*& rNextPosition) {
    JL_Assoc* pAssocRet = static_cast<JL_Assoc*>(rNextPosition);
    if (m->m_pHashTable == nullptr || pAssocRet == nullptr) { rNextPosition = nullptr; return nullptr; }
    if (pAssocRet == reinterpret_cast<JL_Assoc*>(static_cast<INT_PTR>(-1))) {
        pAssocRet = nullptr;
        for (UINT nBucket = 0; nBucket < m->m_nHashTableSize; ++nBucket) {
            if ((pAssocRet = m->m_pHashTable[nBucket]) != nullptr) break;
        }
        if (pAssocRet == nullptr) { rNextPosition = nullptr; return nullptr; }
    }
    JL_Assoc* pAssocNext = pAssocRet->pNext;
    if (pAssocNext == nullptr) {
        for (UINT nBucket = (pAssocRet->nHashValue % m->m_nHashTableSize) + 1;
             nBucket < m->m_nHashTableSize; ++nBucket) {
            if ((pAssocNext = m->m_pHashTable[nBucket]) != nullptr) break;
        }
    }
    rNextPosition = pAssocNext;
    return pAssocRet;
}

// RemoveAll (RVA 0x3a400): destroy every key CString, free the hash table, reset
// m_nCount / m_pFreeList, free the CPlex chain.  m_nHashTableSize is kept.  The values
// are not touched.
void JL_MapRemoveAll(JL_Map* m) {
    if (m->m_pHashTable != nullptr) {
        for (UINT nHash = 0; nHash < m->m_nHashTableSize; ++nHash) {
            for (JL_Assoc* p = m->m_pHashTable[nHash]; p != nullptr; p = p->pNext) {
                Str(&p->key)->Destruct();
            }
        }
        std::free(m->m_pHashTable);
        m->m_pHashTable = nullptr;
    }
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    for (JL_Plex* p = m->m_pBlocks; p != nullptr;) {
        JL_Plex* pNext = p->pNext;
        std::free(p);
        p = pNext;
    }
    m->m_pBlocks = nullptr;
}

// ---- helpers shared by the thunks ------------------------------------------------------

// Retail reaches SHCreateItemFromParsingName through a local helper (RVA 0x1cab08) that
// does GetModuleHandleW(L"shell32.dll") + GetProcAddress("SHCreateItemFromParsingName"),
// caches the result (Encode/DecodePointer) and returns E_FAIL (0x80004005) when either
// lookup fails.  Transcribed with a plain static cache.
HRESULT JL_SHCreateItemFromParsingName(const wchar_t* pszPath, void* pbc, const GUID& riid, void** ppv) {
    using Fn = HRESULT (STDAPICALLTYPE*)(const wchar_t*, void*, const GUID&, void**);
    static Fn s_pfn = nullptr;
    static bool s_resolved = false;
    if (!s_resolved) {
        HMODULE hShell = ::GetModuleHandleW(L"shell32.dll");
        if (hShell == nullptr) return E_FAIL;
        s_pfn = reinterpret_cast<Fn>(::GetProcAddress(hShell, "SHCreateItemFromParsingName"));
        s_resolved = true;
    }
    if (s_pfn == nullptr) return E_FAIL;
    return s_pfn(pszPath, pbc, riid, ppv);
}

// InitializeList (0x72230) reads AfxGetModuleState()->m_pCurrentWinApp->m_bIsWindows7
// (+0x158), which the retail CWinApp constructor (??0CWinApp@@QEAA@PEB_W@Z, RVA 0x1cb720)
// computes once from GetVersionExW as (major == 6 && minor >= 1) || major > 6.
// DEVIATION: OpenMFC's CWinApp has no m_bIsWindows7 member at +0x158 (its constructor in
// core/app/CWinApp.cpp does not set one), so the same predicate is evaluated here.
BOOL JL_IsWindows7OrLater() {
    OSVERSIONINFOW osvi;
    std::memset(&osvi, 0, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);   // 0x114, as in the retail constructor
    ::GetVersionExW(&osvi);
    return ((osvi.dwMajorVersion == 6 && osvi.dwMinorVersion >= 1) || osvi.dwMajorVersion > 6) ? TRUE : FALSE;
}

// InitializeList falls back to pApp->m_pszAppID (+0xa8) when m_strAppID is empty.
// NOT REPRODUCED: OpenMFC's CWinApp has no m_pszAppID member; CWinApp::SetAppID
// (core/app/CWinApp.cpp) stores the id in that file's side table
// (openmfc::detail::cwinapp::g_appRuntimeStates), which has no extern "C" accessor, and
// reaching the C++ symbol from here is exactly the link hazard the thunk rule forbids.
// So the fallback yields no id: a jump list whose own m_strAppID is empty is built for
// the process's default AppUserModelID.  (Requested: an accessor in CWinApp.cpp.)
const wchar_t* JL_AppID() {
    return nullptr;
}

}  // namespace

// ---------------------------------------------------------------------------------------
// Exported thunks
// ---------------------------------------------------------------------------------------

// Retail RVA 0x72090.  Not on the stub list this file was assigned from (its placeholder
// body `return pThis;` was not counted as a stub), but it initialised nothing, and the
// destructor below reads every member, so it is transcribed as well.
// Symbol: ??0CJumpList@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CJumpList__QEAA_H_Z(void* pThis, int bAutoCommit) {
    S_CJumpList* self = Self(pThis);
    Str(&self->m_strAppID)->Construct();
    self->m_removedItemsPtr = nullptr;
    self->m_destListPtr = nullptr;
    self->m_tasksPtr = nullptr;
    self->m_mapDestinations.vfptr = nullptr;           // retail: CMap<...> vftable (see header)
    self->m_mapDestinations.m_pHashTable = nullptr;
    self->m_mapDestinations.m_nHashTableSize = 17;
    self->m_mapDestinations.m_nCount = 0;
    self->m_mapDestinations.m_pFreeList = nullptr;
    self->m_mapDestinations.m_pBlocks = nullptr;
    self->m_mapDestinations.m_nBlockSize = 10;
    self->m_bInitialized = FALSE;
    self->m_bIsSupported = FALSE;
    self->m_nMaxSlots = 0;
    self->m_bAutoCommit = bAutoCommit;
    return pThis;
}

// Retail RVA 0x72110:
//   if (m_bIsSupported) { if (m_bAutoCommit) CommitList(); else ClearAll(); }
//   then member destructors: m_mapDestinations (RemoveAll 0x3a400), m_tasksPtr,
//   m_destListPtr, m_removedItemsPtr (each: if non-NULL, Release), m_strAppID.
// Symbol: ??1CJumpList@@QEAA@XZ
extern "C" void MS_ABI impl___1CJumpList__QEAA_XZ(void* pThis) {
    S_CJumpList* self = Self(pThis);
    if (self->m_bIsSupported) {
        if (self->m_bAutoCommit) impl__CommitList_CJumpList__QEAAHXZ(pThis);
        else impl__ClearAll_CJumpList__QEAAXXZ(pThis);
    }
    JL_MapRemoveAll(&self->m_mapDestinations);
    if (self->m_tasksPtr) ComRelease(self->m_tasksPtr);
    if (self->m_destListPtr) ComRelease(self->m_destListPtr);
    if (self->m_removedItemsPtr) ComRelease(self->m_removedItemsPtr);
    Str(&self->m_strAppID)->Destruct();
}

// Both AddDestination(LPCTSTR, IShellItem*) and AddDestination(LPCTSTR, IShellLinkW*)
// resolve to the same retail entry, RVA 0x728c0 (identical-code folding: both only hand
// the pointer to IObjectCollection::AddObject).  The LPCTSTR/LPCTSTR overload is 0x72830:
//   if (!m_bIsSupported || !lpcszCategoryName || !strDestinationPath) return FALSE;
//   pColl = GetObjectCollection(lpcszCategoryName); if (!pColl) return FALSE;
//   IShellItem* psi = NULL;
//   if (FAILED(SHCreateItemFromParsingName(path, NULL, IID_IShellItem, &psi))) return FALSE;
//   if (FAILED(pColl->AddObject(psi))) { psi->Release(); return FALSE; }      // slot 5
//   return TRUE;          // psi's reference is not released on success
// Note m_bIsSupported is only set by InitializeList, which this does not call.
// Symbol: ?AddDestination@CJumpList@@QEAAHPEB_W0@Z
extern "C" int MS_ABI impl__AddDestination_CJumpList__QEAAHPEB_W0_Z(void* pThis, const wchar_t* lpcszCategoryName, const wchar_t* strDestinationPath) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported || lpcszCategoryName == nullptr || strDestinationPath == nullptr) return FALSE;
    void* pColl = impl__GetObjectCollection_CJumpList__IEAAPEAUIObjectCollection__PEB_W_Z(pThis, lpcszCategoryName);
    if (pColl == nullptr) return FALSE;
    void* psi = nullptr;
    if (FAILED(JL_SHCreateItemFromParsingName(strDestinationPath, nullptr, kIID_IShellItem, &psi))) return FALSE;
    if (FAILED(ComCall<HRESULT>(pColl, kOC_AddObject, psi))) {
        ComRelease(psi);
        return FALSE;
    }
    return TRUE;
}

// Retail RVA 0x728c0:
//   if (!m_bIsSupported || !pShellItem) return FALSE;
//   pColl = GetObjectCollection(strCategoryName); if (!pColl) return FALSE;
//   return SUCCEEDED(pColl->AddObject(pShellItem));                             // slot 5
// Symbol: ?AddDestination@CJumpList@@QEAAHPEB_WPEAUIShellItem@@@Z
extern "C" int MS_ABI impl__AddDestination_CJumpList__QEAAHPEB_WPEAUIShellItem___Z(void* pThis, const wchar_t* strCategoryName, void* pShellItem) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported || pShellItem == nullptr) return FALSE;
    void* pColl = impl__GetObjectCollection_CJumpList__IEAAPEAUIObjectCollection__PEB_W_Z(pThis, strCategoryName);
    if (pColl == nullptr) return FALSE;
    return SUCCEEDED(ComCall<HRESULT>(pColl, kOC_AddObject, pShellItem)) ? TRUE : FALSE;
}

// Same retail entry as the IShellItem overload (RVA 0x728c0, see above).
// Symbol: ?AddDestination@CJumpList@@QEAAHPEB_WPEAUIShellLinkW@@@Z
extern "C" int MS_ABI impl__AddDestination_CJumpList__QEAAHPEB_WPEAUIShellLinkW___Z(void* pThis, const wchar_t* strCategoryName, void* pShellLink) {
    return impl__AddDestination_CJumpList__QEAAHPEB_WPEAUIShellItem___Z(pThis, strCategoryName, pShellLink);
}

// Retail RVA 0x723d0:
//   if (!InitializeList()) return FALSE;
//   return SUCCEEDED(m_destListPtr->AppendKnownCategory(category));              // slot 6
// Symbol: ?AddKnownCategory@CJumpList@@QEAAHW4KNOWNDESTCATEGORY@@@Z
extern "C" int MS_ABI impl__AddKnownCategory_CJumpList__QEAAHW4KNOWNDESTCATEGORY___Z(void* pThis, int category) {
    S_CJumpList* self = Self(pThis);
    if (!impl__InitializeList_CJumpList__QEAAHXZ(pThis)) return FALSE;
    return SUCCEEDED(ComCall<HRESULT>(self->m_destListPtr, kCDL_AppendKnownCategory, category)) ? TRUE : FALSE;
}

// Retail RVA 0x72780:
//   if (!m_bIsSupported || !pShellLink) return FALSE;
//   if (!InitializeList()) return FALSE;
//   if (FAILED(m_tasksPtr->AddObject(pShellLink))) return FALSE;                 // slot 5
//   pShellLink->AddRef();                                                        // slot 1
//   return TRUE;
// Symbol: ?AddTask@CJumpList@@QEAAHPEAUIShellLinkW@@@Z
extern "C" int MS_ABI impl__AddTask_CJumpList__QEAAHPEAUIShellLinkW___Z(void* pThis, void* pShellLink) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported || pShellLink == nullptr) return FALSE;
    if (!impl__InitializeList_CJumpList__QEAAHXZ(pThis)) return FALSE;
    if (FAILED(ComCall<HRESULT>(self->m_tasksPtr, kOC_AddObject, pShellLink))) return FALSE;
    ComAddRef(pShellLink);
    return TRUE;
}

// Retail RVA 0x72420:
//   if (!InitializeList() || !strTargetExecutablePath || !strTitle) return FALSE;
//   CComPtr<IShellLinkW> psl;
//   if (FAILED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, &psl))) return FALSE;
//   psl->SetPath(path);                    // slot 20, result ignored
//   psl->SetArguments(args);               // slot 11, result ignored
//   psl->SetIconLocation(icon, iIcon);     // slot 17, result ignored
//   CComQIPtr<IPropertyStore> pps(psl);    // QI; NULL on failure
//   if (pps) {
//       PROPVARIANT pv;  InitPropVariantFromString(strTitle, &pv)  -- inlined:
//           CoTaskMemAlloc((wcslen+1)*2); on failure memset(&pv,0,0x18) and return FALSE;
//           else memcpy, vt = VT_LPWSTR
//       if (FAILED(pps->SetValue(PKEY_Title, pv))) { PropVariantClear(&pv); return FALSE; }  // slot 6
//       hr = pps->Commit(); PropVariantClear(&pv); if (FAILED(hr)) return FALSE;              // slot 7
//   }
//   return SUCCEEDED(m_tasksPtr->AddObject(psl.Detach()));                       // slot 5
// (Every FALSE path between CoCreateInstance and AddObject releases pps and psl; the
// Detach means the creation reference of psl is handed to AddObject and never released,
// whether AddObject succeeds or fails.)
// Symbol: ?AddTask@CJumpList@@QEAAHPEB_W000H@Z
extern "C" int MS_ABI impl__AddTask_CJumpList__QEAAHPEB_W000H_Z(void* pThis, const wchar_t* strTargetExecutablePath, const wchar_t* strCommandLineArgs, const wchar_t* strTitle, const wchar_t* strIconLocation, int iIconIndex) {
    S_CJumpList* self = Self(pThis);
    if (!impl__InitializeList_CJumpList__QEAAHXZ(pThis)) return FALSE;
    if (strTargetExecutablePath == nullptr || strTitle == nullptr) return FALSE;

    void* psl = nullptr;
    if (FAILED(::CoCreateInstance(kCLSID_ShellLink, nullptr, kClsCtxInprocServer, kIID_IShellLinkW, &psl))) return FALSE;
    ComCall<HRESULT>(psl, kSL_SetPath, strTargetExecutablePath);
    ComCall<HRESULT>(psl, kSL_SetArguments, strCommandLineArgs);
    ComCall<HRESULT>(psl, kSL_SetIconLocation, strIconLocation, iIconIndex);

    void* pps = nullptr;
    if (FAILED(ComQI(psl, kIID_IPropertyStore, &pps))) pps = nullptr;

    BOOL bResult = FALSE;
    bool bTitleOk = true;
    if (pps != nullptr) {
        PROPVARIANT pv;
        const size_t cb = (std::wcslen(strTitle) + 1) * sizeof(wchar_t);
        wchar_t* pwsz = static_cast<wchar_t*>(::CoTaskMemAlloc(cb));
        if (pwsz == nullptr) {
            std::memset(&pv, 0, sizeof(pv));
            bTitleOk = false;
        } else {
            std::memcpy(pwsz, strTitle, cb);
            std::memset(&pv, 0, sizeof(pv));
            pv.vt = VT_LPWSTR;
            pv.pwszVal = pwsz;
            if (FAILED(ComCall<HRESULT>(pps, kPS_SetValue, &kPKEY_Title, &pv))) {
                ::PropVariantClear(&pv);
                bTitleOk = false;
            } else {
                HRESULT hr = ComCall<HRESULT>(pps, kPS_Commit);
                ::PropVariantClear(&pv);
                if (FAILED(hr)) bTitleOk = false;
            }
        }
    }
    if (bTitleOk) {
        void* pDetached = psl;
        psl = nullptr;
        bResult = SUCCEEDED(ComCall<HRESULT>(self->m_tasksPtr, kOC_AddObject, pDetached)) ? TRUE : FALSE;
    }
    if (pps != nullptr) ComRelease(pps);
    if (psl != nullptr) ComRelease(psl);
    return bResult;
}

// Retail RVA 0x72620:
//   if (!InitializeList()) return FALSE;
//   CComPtr<IShellLinkW> psl;
//   if (FAILED(CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, &psl))) return FALSE;
//   CComQIPtr<IPropertyStore> pps(psl);
//   PROPVARIANT pv; pv.vt = VT_BOOL; pv.boolVal = VARIANT_TRUE;
//   if (FAILED(pps->SetValue(PKEY_AppUserModel_IsDestListSeparator, pv))) { PropVariantClear(&pv); return FALSE; }
//   hr = pps->Commit(); PropVariantClear(&pv); if (FAILED(hr)) return FALSE;
//   return SUCCEEDED(m_tasksPtr->AddObject(psl.Detach()));
// DEVIATION: retail calls pps->SetValue without testing pps, so a failed QI faults on a
// NULL vtable read (the load at 0x726c1 inside this function); here a failed QI
// returns FALSE instead.
// Symbol: ?AddTaskSeparator@CJumpList@@QEAAHXZ
extern "C" int MS_ABI impl__AddTaskSeparator_CJumpList__QEAAHXZ(void* pThis) {
    S_CJumpList* self = Self(pThis);
    if (!impl__InitializeList_CJumpList__QEAAHXZ(pThis)) return FALSE;

    void* psl = nullptr;
    if (FAILED(::CoCreateInstance(kCLSID_ShellLink, nullptr, kClsCtxInprocServer, kIID_IShellLinkW, &psl))) return FALSE;
    void* pps = nullptr;
    if (FAILED(ComQI(psl, kIID_IPropertyStore, &pps))) pps = nullptr;

    BOOL bResult = FALSE;
    if (pps != nullptr) {
        PROPVARIANT pv;
        std::memset(&pv, 0, sizeof(pv));
        pv.vt = VT_BOOL;
        pv.boolVal = VARIANT_TRUE;
        bool bOk = true;
        if (FAILED(ComCall<HRESULT>(pps, kPS_SetValue, &kPKEY_AppUserModel_IsDestListSeparator, &pv))) {
            ::PropVariantClear(&pv);
            bOk = false;
        } else {
            HRESULT hr = ComCall<HRESULT>(pps, kPS_Commit);
            ::PropVariantClear(&pv);
            if (FAILED(hr)) bOk = false;
        }
        if (bOk) {
            void* pDetached = psl;
            psl = nullptr;
            bResult = SUCCEEDED(ComCall<HRESULT>(self->m_tasksPtr, kOC_AddObject, pDetached)) ? TRUE : FALSE;
        }
        ComRelease(pps);
    }
    if (psl != nullptr) ComRelease(psl);
    return bResult;
}

// Retail RVA 0x727e0:
//   if (!InitializeList() || !pObjectCollection) return FALSE;
//   return SUCCEEDED(m_tasksPtr->AddFromArray(pObjectCollection));               // slot 6
// Symbol: ?AddTasks@CJumpList@@QEAAHPEAUIObjectArray@@@Z
extern "C" int MS_ABI impl__AddTasks_CJumpList__QEAAHPEAUIObjectArray___Z(void* pThis, void* pObjectCollection) {
    S_CJumpList* self = Self(pThis);
    if (!impl__InitializeList_CJumpList__QEAAHXZ(pThis)) return FALSE;
    if (pObjectCollection == nullptr) return FALSE;
    return SUCCEEDED(ComCall<HRESULT>(self->m_tasksPtr, kOC_AddFromArray, pObjectCollection)) ? TRUE : FALSE;
}

// Retail RVA 0x72b80:
//   if (!m_bIsSupported) return NULL;
//   if (!m_removedItemsPtr) return pColl;
//   UINT nRemoved = 0, nItems = 0;
//   if (FAILED(m_removedItemsPtr->GetCount(&nRemoved)) || !nRemoved) return pColl;
//   if (FAILED(pColl->GetCount(&nItems)) || !nItems) return pColl;
//   IObjectCollection* pNew = NULL;
//   if (FAILED(CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, 1, IID_IObjectCollection, &pNew))) return pColl;
//   for (i = 0; i < nItems; i++) {
//       IShellItem* psi = NULL;
//       if (FAILED(pColl->GetAt(i, IID_IShellItem, &psi)) || !psi) continue;
//       for (j = 0; j < nRemoved; j++) {
//           IShellItem* pRemoved = NULL;
//           if (FAILED(m_removedItemsPtr->GetAt(i, IID_IShellItem, &pRemoved)) || !pRemoved) continue;
//           int order = 0;
//           if (SUCCEEDED(psi->Compare(pRemoved, 0, &order)) && order != 0)    // slot 7
//               pNew->AddObject(psi);
//           else
//               psi->Release();
//       }
//   }
//   pColl->Release();
//   return pNew;
// Transcribed as-is, including three retail quirks: the inner GetAt indexes the removed
// list with the OUTER index i (the `mov %edi,%edx` at 0x72c96 inside this function),
// pRemoved is never released, and psi is added to / released once per inner iteration
// rather than once in total (so with nRemoved >= 2 and a match, psi is over-released).
// Because every inner iteration fetches removed[i], an item whose index i >= nRemoved gets
// a failing GetAt on every inner pass and is therefore neither added to pNew nor released:
// it is dropped from the category (and its GetAt reference leaks).  An item with
// i < nRemoved that does not match removed[i] is added nRemoved times.
// Symbol: ?CheckRemovedItems@CJumpList@@IEAAPEAUIObjectCollection@@PEAU2@@Z
extern "C" void* MS_ABI impl__CheckRemovedItems_CJumpList__IEAAPEAUIObjectCollection__PEAU2__Z(void* pThis, void* pColl) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported) return nullptr;
    if (self->m_removedItemsPtr == nullptr) return pColl;
    UINT nRemoved = 0;
    UINT nItems = 0;
    if (FAILED(ComCall<HRESULT>(self->m_removedItemsPtr, kOA_GetCount, &nRemoved)) || nRemoved == 0) return pColl;
    if (FAILED(ComCall<HRESULT>(pColl, kOA_GetCount, &nItems)) || nItems == 0) return pColl;
    void* pNew = nullptr;
    if (FAILED(::CoCreateInstance(kCLSID_EnumerableObjectCollection, nullptr, kClsCtxInprocServer, kIID_IObjectCollection, &pNew))) return pColl;
    for (UINT i = 0; i < nItems; ++i) {
        void* psi = nullptr;
        if (FAILED(ComCall<HRESULT>(pColl, kOA_GetAt, i, &kIID_IShellItem, &psi)) || psi == nullptr) continue;
        for (UINT j = 0; j < nRemoved; ++j) {
            void* pRemoved = nullptr;
            if (FAILED(ComCall<HRESULT>(self->m_removedItemsPtr, kOA_GetAt, i, &kIID_IShellItem, &pRemoved)) || pRemoved == nullptr) continue;
            int order = 0;
            if (SUCCEEDED(ComCall<HRESULT>(psi, kSI_Compare, pRemoved, static_cast<DWORD>(0), &order)) && order != 0)
                ComCall<HRESULT>(pNew, kOC_AddObject, psi);
            else
                ComRelease(psi);
        }
    }
    ComRelease(pColl);
    return pNew;
}

// Retail RVA 0x721c0:
//   if (!m_bIsSupported) return;
//   ClearAllDestinations();
//   m_destListPtr.Release();                         // detach to NULL, then Release
//   ReleaseObjectArray(m_tasksPtr.Detach());
//   ReleaseObjectArray(m_removedItemsPtr.Detach());
//   m_bInitialized = FALSE;
// Symbol: ?ClearAll@CJumpList@@QEAAXXZ
extern "C" void MS_ABI impl__ClearAll_CJumpList__QEAAXXZ(void* pThis) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported) return;
    impl__ClearAllDestinations_CJumpList__QEAAXXZ(pThis);
    if (void* p = self->m_destListPtr) {
        self->m_destListPtr = nullptr;
        ComRelease(p);
    }
    void* pTasks = self->m_tasksPtr;
    self->m_tasksPtr = nullptr;
    impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(pThis, pTasks, FALSE);
    void* pRemoved = self->m_removedItemsPtr;
    self->m_removedItemsPtr = nullptr;
    impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(pThis, pRemoved, FALSE);
    self->m_bInitialized = FALSE;
}

// Retail RVA 0x72d50:
//   if (!m_bIsSupported) return;
//   for each (key, pColl) in m_mapDestinations: ReleaseObjectArray(pColl, FALSE)  (inlined)
//   m_mapDestinations.RemoveAll();
// Symbol: ?ClearAllDestinations@CJumpList@@QEAAXXZ
extern "C" void MS_ABI impl__ClearAllDestinations_CJumpList__QEAAXXZ(void* pThis) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported) return;
    JL_Map* m = &self->m_mapDestinations;
    void* pos = JL_MapGetStartPosition(m);
    while (pos != nullptr) {
        JL_Assoc* pAssoc = JL_MapGetNextAssoc(m, pos);
        if (pAssoc == nullptr) break;
        impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(pThis, pAssoc->value, FALSE);
    }
    JL_MapRemoveAll(m);
}

// Retail RVA 0x729b0:
//   if (!InitializeList()) return FALSE;
//   for each (strCategory, pColl) in m_mapDestinations {
//       IObjectCollection* p = CheckRemovedItems(pColl);
//       if (p) {
//           UINT n = 0; p->GetCount(&n);                     // result and n unused
//           m_destListPtr->AppendCategory(strCategory, p);   // slot 5, result ignored
//           ReleaseObjectArray(p, FALSE);
//       }
//   }
//   UINT nTasks = 0; m_tasksPtr->GetCount(&nTasks);
//   if (nTasks > 0) {
//       m_destListPtr->AddUserTasks(m_tasksPtr);             // slot 7, result ignored
//       ReleaseObjectArray(m_tasksPtr.Detach(), FALSE);
//   }
//   m_mapDestinations.RemoveAll();
//   BOOL bResult = SUCCEEDED(m_destListPtr->CommitList());  // slot 8
//   ClearAll();
//   m_bAutoCommit = FALSE;
//   return bResult;
// Symbol: ?CommitList@CJumpList@@QEAAHXZ
extern "C" int MS_ABI impl__CommitList_CJumpList__QEAAHXZ(void* pThis) {
    S_CJumpList* self = Self(pThis);
    if (!impl__InitializeList_CJumpList__QEAAHXZ(pThis)) return FALSE;

    JL_Map* m = &self->m_mapDestinations;
    void* pos = JL_MapGetStartPosition(m);
    while (pos != nullptr) {
        JL_Assoc* pAssoc = JL_MapGetNextAssoc(m, pos);
        if (pAssoc == nullptr) break;
        void* p = impl__CheckRemovedItems_CJumpList__IEAAPEAUIObjectCollection__PEAU2__Z(pThis, pAssoc->value);
        if (p == nullptr) continue;
        UINT n = 0;
        ComCall<HRESULT>(p, kOA_GetCount, &n);
        ComCall<HRESULT>(self->m_destListPtr, kCDL_AppendCategory, static_cast<const wchar_t*>(pAssoc->key), p);
        impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(pThis, p, FALSE);
    }

    UINT nTasks = 0;
    ComCall<HRESULT>(self->m_tasksPtr, kOA_GetCount, &nTasks);
    if (nTasks > 0) {
        ComCall<HRESULT>(self->m_destListPtr, kCDL_AddUserTasks, self->m_tasksPtr);
        void* pTasks = self->m_tasksPtr;
        self->m_tasksPtr = nullptr;
        impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(pThis, pTasks, FALSE);
    }
    JL_MapRemoveAll(m);
    const BOOL bResult = SUCCEEDED(ComCall<HRESULT>(self->m_destListPtr, kCDL_CommitList)) ? TRUE : FALSE;
    impl__ClearAll_CJumpList__QEAAXXZ(pThis);
    self->m_bAutoCommit = FALSE;
    return bResult;
}

// Retail RVA 0x723c0:  return m_bIsSupported ? m_nMaxSlots : 0;
// Symbol: ?GetMaxSlots@CJumpList@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetMaxSlots_CJumpList__QEBAIXZ(const void* pThis) {
    const S_CJumpList* self = static_cast<const S_CJumpList*>(pThis);
    return self->m_bIsSupported ? self->m_nMaxSlots : 0u;
}

// Retail RVA 0x72910:
//   if (!m_bIsSupported || !lpcszCategoryName) return NULL;
//   IObjectCollection* p = NULL;
//   if (m_mapDestinations.Lookup(name, p) && p) return p;     // Lookup inlined; it calls GetAssocAt 0x314e0
//   if (FAILED(CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, 1, IID_IObjectCollection, &p))) return NULL;
//   m_mapDestinations.SetAt(name, p);                         // 0x310c4
//   return p;
// Symbol: ?GetObjectCollection@CJumpList@@IEAAPEAUIObjectCollection@@PEB_W@Z
extern "C" void* MS_ABI impl__GetObjectCollection_CJumpList__IEAAPEAUIObjectCollection__PEB_W_Z(void* pThis, const wchar_t* lpcszCategoryName) {
    S_CJumpList* self = Self(pThis);
    if (!self->m_bIsSupported || lpcszCategoryName == nullptr) return nullptr;
    UINT nHashBucket = 0, nHashValue = 0;
    void* p = nullptr;
    if (JL_Assoc* pAssoc = JL_MapGetAssocAt(&self->m_mapDestinations, lpcszCategoryName, nHashBucket, nHashValue))
        p = pAssoc->value;
    if (p != nullptr) return p;
    if (FAILED(::CoCreateInstance(kCLSID_EnumerableObjectCollection, nullptr, kClsCtxInprocServer, kIID_IObjectCollection, &p))) return nullptr;
    JL_MapSetAt(&self->m_mapDestinations, lpcszCategoryName, p);
    return p;
}

// Retail RVA 0x72230:
//   CWinApp* pApp = AfxGetModuleState()->m_pCurrentWinApp;       // 0x133930, +0x8
//   if (!pApp->m_bIsWindows7) return FALSE;                        // +0x158
//   m_bIsSupported = TRUE;
//   if (!m_destListPtr && FAILED(CoCreateInstance(CLSID_DestinationList, NULL, 1,
//                                    IID_ICustomDestinationList, &m_destListPtr))) return FALSE;
//   if (!m_tasksPtr && FAILED(CoCreateInstance(CLSID_EnumerableObjectCollection, NULL, 1,
//                                    IID_IObjectCollection, &m_tasksPtr))) return FALSE;
//   if (m_bInitialized) return TRUE;
//   CString strAppID = m_strAppID;
//   if (strAppID.IsEmpty()) strAppID.SetString(pApp->m_pszAppID);  // +0xa8
//   if (!strAppID.IsEmpty()) m_destListPtr->SetAppID(strAppID);    // slot 3, result ignored
//   if (FAILED(m_destListPtr->BeginList(&m_nMaxSlots, IID_IObjectArray,
//                                       (void**)&m_removedItemsPtr))) return FALSE;  // slot 4
//   m_bInitialized = TRUE;
//   return TRUE;
// DEVIATIONS (see JL_IsWindows7OrLater / JL_AppID above): the Windows-7 test is
// evaluated here rather than read from the CWinApp, and the CWinApp::m_pszAppID fallback
// is not available, so only m_strAppID is ever passed to SetAppID.  The local CString
// copy is not materialised: m_strAppID's buffer is passed directly.
// Symbol: ?InitializeList@CJumpList@@QEAAHXZ
extern "C" int MS_ABI impl__InitializeList_CJumpList__QEAAHXZ(void* pThis) {
    S_CJumpList* self = Self(pThis);
    if (!JL_IsWindows7OrLater()) return FALSE;
    self->m_bIsSupported = TRUE;
    if (self->m_destListPtr == nullptr &&
        FAILED(::CoCreateInstance(kCLSID_DestinationList, nullptr, kClsCtxInprocServer, kIID_ICustomDestinationList, &self->m_destListPtr)))
        return FALSE;
    if (self->m_tasksPtr == nullptr &&
        FAILED(::CoCreateInstance(kCLSID_EnumerableObjectCollection, nullptr, kClsCtxInprocServer, kIID_IObjectCollection, &self->m_tasksPtr)))
        return FALSE;
    if (self->m_bInitialized) return TRUE;

    const wchar_t* pszAppID = Str(&self->m_strAppID)->IsEmpty() ? JL_AppID() : Str(&self->m_strAppID)->GetString();
    if (pszAppID != nullptr && *pszAppID != L'\0')
        ComCall<HRESULT>(self->m_destListPtr, kCDL_SetAppID, pszAppID);
    if (FAILED(ComCall<HRESULT>(self->m_destListPtr, kCDL_BeginList, &self->m_nMaxSlots, &kIID_IObjectArray, &self->m_removedItemsPtr)))
        return FALSE;
    self->m_bInitialized = TRUE;
    return TRUE;
}

// Retail RVA 0x72e90 (reached through the export table; ClearAll and CommitList call it
// directly):
//   if (!pColl) return;
//   UINT n = 0; if (FAILED(pColl->GetCount(&n))) return;
//   for (i = 0; i < n; i++) {
//       IUnknown* p = NULL;
//       if (SUCCEEDED(pColl->GetAt(i, IID_IUnknown, (void**)&p)) && p) p->Release();
//   }
//   if (!bReleaseItemsOnly) pColl->Release();
// (Each item's Release balances the reference GetAt just added, so the loop leaves the
// items' reference counts unchanged -- that is what retail does.)
// Symbol: ?ReleaseObjectArray@CJumpList@@IEAAXPEAUIObjectArray@@H@Z
extern "C" void MS_ABI impl__ReleaseObjectArray_CJumpList__IEAAXPEAUIObjectArray__H_Z(void* pThis, void* pColl, int bReleaseItemsOnly) {
    (void)pThis;
    if (pColl == nullptr) return;
    UINT n = 0;
    if (FAILED(ComCall<HRESULT>(pColl, kOA_GetCount, &n))) return;
    for (UINT i = 0; i < n; ++i) {
        void* p = nullptr;
        if (SUCCEEDED(ComCall<HRESULT>(pColl, kOA_GetAt, i, &kIID_IUnknown, &p)) && p != nullptr) ComRelease(p);
    }
    if (!bReleaseItemsOnly) ComRelease(pColl);
}
