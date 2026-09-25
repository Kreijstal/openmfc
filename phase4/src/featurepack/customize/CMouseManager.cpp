// CMouseManager — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp (placeholders), rewritten from retail.
//
// The feature-pack mouse-event manager (retail declaration: atlmfc/include/
// afxmousemanager.h, `class CMouseManager : public CObject`, DECLARE_SERIAL).  OpenMFC
// only forward-declares it (include/openmfc/afxmfc.h), so -- like core/app/CJumpList.cpp --
// the thunks take `void* pThis` and work on the layout view S_CMouseManager pinned below.
//
// Every body here was transcribed from the retail disassembly (the method described in
// the header of core/ole/COleControl.cpp).  All RVAs in this file are mfc140u.dll RVAs.
// mfc140u's symbol map does not resolve Serialize, SaveState or SetCommandForDblClk; for
// those the mfc140 (ANSI twin) exports were disassembled (0x91080, 0x90eb0, 0x912a0 in
// mfc140) and the byte-identical bodies located in mfc140u at 0x90ac0, 0x908f0 and 0x90ce0
// (their first 60 instructions match mnemonic for mnemonic); 0x90ac0 is also slot 2 of
// the mfc140u vftable.
//
// ---- layout (read from the retail constructor ??0CMouseManager@@QEAA@XZ, RVA 0x902e0) ----
//   +0x00 vfptr                                   ctor: CMouseManager vftable (0x1802eff08)
//   +0x08 CMap<CString,LPCTSTR,int,int> m_ViewsNames      map vftable 0x1802efed8
//   +0x40 CMap<int,int,UINT,UINT>       m_ViewsToCommands map vftable 0x1802efea8
//   +0x78 CMap<int,int,UINT,UINT>       m_ViewsToIcons    map vftable 0x1802efea8
//   sizeof == 0xb0 (CreateObject 0x90270 allocates 0xb0; the deleting destructor 0x90390
//   passes 0xb0 as the size)
// Each CMap is { vfptr, m_pHashTable (NULL), m_nHashTableSize (17), m_nCount (0),
// m_pFreeList (NULL), m_pBlocks (NULL), m_nBlockSize (10) } -- the ctor's stores at
// +0x10..+0x38 / +0x48..+0x70 / +0x80..+0xa8.  Which map is which follows the
// declaration order and is confirmed by the code: GetViewIdByName probes +0x08 with a
// string key, GetViewDblClickCommand / SetCommandForDblClk / Serialize use +0x40, and
// GetViewIconId / AddView(icon) use +0x78.
//
// ---- the retail vftable (0x1802eff08, mfc140u) ----
//   0 GetRuntimeClass 0x902a0   1 {deleting dtor} 0x90390   2 Serialize 0x90ac0
//   3 AssertValid, 4 Dump: both 0x27d0, a bare `ret`
// Reproduced by kMouseManagerVtbl below, which the constructor installs.
//
// ---- the CMaps ----------------------------------------------------------------------
// The three maps are CMap<> template instantiations; retail compiles their members as
// local, non-exported helpers:
//   CMap<CString,...>: GetAssocAt 0x314e0, SetAt 0x90dc4, ~CMap (with RemoveAll) 0x90f00,
//                      HashKey<LPCWSTR> 0x2731c0 (exported), CompareElements 0x31558
//   CMap<int,...>:     GetAssocAt 0x236e0, SetAt 0x2346c, RemoveAll 0x1ba40,
//                      GetNextAssoc 0x910e4
// OpenMFC's CMap template has neither retail's layout nor thunks for these, so they are
// transcribed below (MM_*) over the retail layout, the way core/app/CJumpList.cpp does it.
// CAssoc layouts, read from those helpers:
//   string map: +0x00 CString key, +0x08 int value, +0x10 pNext, +0x18 nHashValue; 0x20
//   int map:    +0x00 int key, +0x04 UINT value, +0x08 pNext, +0x10 nHashValue;    0x18
// Retail allocates the hash table and the CPlex blocks with MFC's ::operator new (0x27f0)
// and frees them with CRT free (IAT 0x1802c74e8).  0x27f0 does not throw by itself: it
// retries malloc (IAT 0x1802c74c8) through the new handler stored at +0x50 of
// AfxGetModuleThreadState() (0x133a20) and returns NULL when there is no handler or the
// handler gives up.  With MFC's usual handler installed that surfaces as CMemoryException;
// otherwise InitHashTable's ENSURE raises AfxThrowInvalidArgException, and CPlex::Create
// (0x271300) stores through the NULL block without checking it.  malloc/free are used here,
// and every allocation failure raises CMemoryException through AfxThrowMemoryException
// (a deviation only on the out-of-memory path).
// DEVIATION: retail stores each map's (unexported) CMap<> vftable in the map's vfptr; no
// CMouseManager code dispatches through them, and OpenMFC has no equivalent tables, so
// nullptr is stored there (as core/app/CJumpList.cpp does for its map).
//
// ---- afxMouseManager, and the guard every member-touching thunk applies ------------
// Retail keeps the singleton in an unexported global (afxMouseManager, 0x1803be268 in
// mfc140u): the ctor ENSUREs it is NULL (AfxThrowInvalidArgException otherwise) and then
// stores `this`; the dtor stores NULL.  So at most one CMouseManager is alive at a time
// and every live one IS afxMouseManager.  That global is reproduced by g_pAfxMouseManager.
// DEVIATION (safety): OpenMFC's ?GetMouseManager@CWinAppEx@@ (core/app/CWinAppEx.cpp)
// does not create a CMouseManager -- it returns the address of a 4-byte `int` token.  A
// client calling theApp.GetMouseManager()->AddView(...) would otherwise have these bodies
// write 0xb0 bytes of map state over that int and its neighbours.  Every thunk that
// touches members therefore first checks pThis == g_pAfxMouseManager and, if not, does
// nothing and returns the neutral value (FALSE / 0 / -1).  For every object the exported
// constructor built the check is always true (see above), so on real objects behaviour is
// retail's.  headerRequests asks for CWinAppEx to create a real one.
//
// ---- exceptions -----------------------------------------------------------------------
// LoadState and SaveState wrap their CMemFile/CArchive work in try { } catch
// (CMemoryException*) / catch (CArchiveException*) (decoded from each function's FuncInfo4:
// one try block, two handlers, types .PEAVCMemoryException@@ and .PEAVCArchiveException@@).
// This DLL raises MFC exceptions through the MSVC runtime's _CxxThrowException, which a gcc
// catch clause cannot see (see core/ole/CMonikerFile.cpp / CArchiveStream.cpp), so the
// catch arms are not reproduced: on the normal path the behaviour is identical; an archive
// or memory exception propagates to the caller instead of making the function return FALSE.

#include "openmfc/afx.h"
#include "detail/AtlStrTCore.h"      // <windows.h>, openmfc/afxstr.h, AtlStrT<wchar_t>

#include <windows.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>

#ifndef MS_ABI
#  if defined(__GNUC__) || defined(__clang__)
#    define MS_ABI __attribute__((ms_abi))
#  else
#    define MS_ABI
#  endif
#endif

using openmfc_str::AtlStrT;

// ---- cross-file thunks (each grepped to its definition; signatures from the mangled names)
//   featurepack/customize/RuntimeClasses.cpp : GetRuntimeClass@CMouseManager
//   detail/MfcExceptionsSupport.cpp          : AfxThrowInvalidArgException, AfxThrowMemoryException,
//                                              AfxThrowArchiveException
//   featurepack/CMFC_misc_stubs.cpp          : AfxFindStringResourceHandle, AFXGetRegPath
//   detail/MemcoreSupport.cpp                : ??2@YAPEAX_K@Z (global operator new)
//   core/collections/CStringList.cpp         : RemoveAll@CStringList, AddTail(LPCTSTR)@CStringList
//   core/runtime/Thunks.cpp                  : Read / Write / Flush / ??1 @CArchive
//   core/runtime/CArchive.cpp                : ??0CArchive
//   core/file/Thunks.cpp                     : ??0CMemFile (both), ??1CMemFile, Detach, GetLength
//   core/app/CSettingsStoreSP.cpp            : Create@CSettingsStoreSP
//   core/app/CSettingsStore.cpp              : Open / CreateKey / Read(LPCTSTR,BYTE**,UINT*) /
//                                              Write(LPCTSTR,LPBYTE,UINT)
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMouseManager__UEBAPEAUCRuntimeClass__XZ(const void* pThis);
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void  MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void  MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName);
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" CString* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
    CString* pRet, const wchar_t* lpszPostFix, const wchar_t* lpszProfileName);
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void  MS_ABI impl__RemoveAll_CStringList__QEAAXXZ(void* pList);
extern "C" void* MS_ABI impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(void* pList, const wchar_t* value);
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* lpBuf, unsigned int nMax);
extern "C" void  MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* lpBuf, unsigned int nMax);
extern "C" void  MS_ABI impl__Flush_CArchive__QEAAXXZ(CArchive* pThis);
extern "C" void* MS_ABI impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(void* pThis, CFile* pFile, unsigned int nMode, int nBufSize, void* lpBuf);
extern "C" void  MS_ABI impl___1CArchive__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0CMemFile__QEAA_I_Z(void* pThis, unsigned int nGrowBytes);
extern "C" void* MS_ABI impl___0CMemFile__QEAA_PEAEII_Z(void* pThis, unsigned char* lpBuffer, unsigned int nBufferSize, unsigned int nGrowBytes);
extern "C" void  MS_ABI impl___1CMemFile__UEAA_XZ(void* pThis);
extern "C" unsigned char* MS_ABI impl__Detach_CMemFile__QEAAPEAEXZ(CMemFile* pThis);
extern "C" unsigned __int64 MS_ABI impl__GetLength_CMemFile__UEBA_KXZ(const CMemFile* pThis);
extern "C" void* MS_ABI impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(void* pThis, int bAdmin, int bReadOnly);
extern "C" int   MS_ABI impl__Open_CSettingsStore__UEAAHPEB_W_Z(void* pStore, const wchar_t* lpszPath);
extern "C" int   MS_ABI impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(void* pStore, const wchar_t* lpszPath);
extern "C" int   MS_ABI impl__Read_CSettingsStore__UEAAHPEB_WPEAPEAEPEAI_Z(void* pStore, const wchar_t* lpszValueName, unsigned char** ppData, unsigned int* pBytes);
extern "C" int   MS_ABI impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(void* pStore, const wchar_t* lpszValueName, const unsigned char* pData, unsigned int nBytes);

// This file's own thunks that are called before their definitions.
extern "C" void* MS_ABI impl___0CMouseManager__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMouseManager__UEAA_XZ(void* pThis);
extern "C" int   MS_ABI impl__AddView_CMouseManager__QEAAHHPEB_WI_Z(void* pThis, int iId, const wchar_t* lpszViewName, unsigned int uiIconId);
extern "C" void  MS_ABI impl__Serialize_CMouseManager__MEAAXAEAVCArchive___Z(void* pThis, CArchive* ar);

namespace {

// ---- pinned layout -----------------------------------------------------------------
struct MM_Plex { MM_Plex* pNext; };   // CPlex header; the assoc blocks follow it

struct MM_StrAssoc {
    wchar_t*     key;          // +0x00 CString (m_pszData)
    int          value;        // +0x08
    MM_StrAssoc* pNext;        // +0x10
    UINT         nHashValue;   // +0x18
};
static_assert(offsetof(MM_StrAssoc, value) == 0x08, "string assoc value @+0x08 (GetViewIdByName 0x906c0 reads 0x8(%rax))");
static_assert(offsetof(MM_StrAssoc, pNext) == 0x10, "string assoc pNext @+0x10 (GetAssocAt 0x314e0)");
static_assert(offsetof(MM_StrAssoc, nHashValue) == 0x18, "string assoc nHashValue @+0x18 (GetAssocAt 0x314e0)");
static_assert(sizeof(MM_StrAssoc) == 0x20, "CMap<CString,...>::CAssoc is 0x20 (SetAt 0x90dc4 passes 0x20 to CPlex::Create)");

struct MM_IntAssoc {
    int          key;          // +0x00
    UINT         value;        // +0x04
    MM_IntAssoc* pNext;        // +0x08
    UINT         nHashValue;   // +0x10
};
static_assert(offsetof(MM_IntAssoc, value) == 0x04, "int assoc value @+0x04 (GetViewDblClickCommand 0x90560 reads 0x4(%rax))");
static_assert(offsetof(MM_IntAssoc, pNext) == 0x08, "int assoc pNext @+0x08 (GetAssocAt 0x236e0)");
static_assert(offsetof(MM_IntAssoc, nHashValue) == 0x10, "int assoc nHashValue @+0x10 (GetAssocAt 0x236e0)");
static_assert(sizeof(MM_IntAssoc) == 0x18, "CMap<int,...>::CAssoc is 0x18 (SetAt 0x2346c passes 0x18 to CPlex::Create)");

template <class A>
struct MM_Map {
    void*    vfptr;             // +0x00 (see DEVIATION in the file header)
    A**      m_pHashTable;      // +0x08
    UINT     m_nHashTableSize;  // +0x10
    INT_PTR  m_nCount;          // +0x18
    A*       m_pFreeList;       // +0x20
    MM_Plex* m_pBlocks;         // +0x28
    INT_PTR  m_nBlockSize;      // +0x30
};
typedef MM_Map<MM_StrAssoc> MM_StrMap;
typedef MM_Map<MM_IntAssoc> MM_IntMap;
static_assert(sizeof(MM_IntMap) == 0x38 && sizeof(MM_StrMap) == 0x38, "CMap is 0x38 bytes");
static_assert(offsetof(MM_IntMap, m_pHashTable) == 0x08, "m_pHashTable");
static_assert(offsetof(MM_IntMap, m_nHashTableSize) == 0x10, "m_nHashTableSize");
static_assert(offsetof(MM_IntMap, m_nCount) == 0x18, "m_nCount");
static_assert(offsetof(MM_IntMap, m_pFreeList) == 0x20, "m_pFreeList");
static_assert(offsetof(MM_IntMap, m_pBlocks) == 0x28, "m_pBlocks");
static_assert(offsetof(MM_IntMap, m_nBlockSize) == 0x30, "m_nBlockSize");

struct S_CMouseManager {
    void*     vfptr;              // +0x00
    MM_StrMap m_ViewsNames;       // +0x08
    MM_IntMap m_ViewsToCommands;  // +0x40
    MM_IntMap m_ViewsToIcons;     // +0x78
};
static_assert(offsetof(S_CMouseManager, m_ViewsNames) == 0x08, "m_ViewsNames @+0x08 (ctor 0x902e0; GetViewIdByName `add $0x8,%rcx`)");
static_assert(offsetof(S_CMouseManager, m_ViewsToCommands) == 0x40, "m_ViewsToCommands @+0x40 (GetViewDblClickCommand `add $0x40,%rcx`)");
static_assert(offsetof(S_CMouseManager, m_ViewsToIcons) == 0x78, "m_ViewsToIcons @+0x78 (GetViewIconId `add $0x78,%rcx`)");
static_assert(sizeof(S_CMouseManager) == 0xb0, "sizeof(CMouseManager) == 0xb0 (CreateObject 0x90270)");

inline S_CMouseManager* Self(void* p) { return static_cast<S_CMouseManager*>(p); }
inline AtlStrT<wchar_t>* Str(wchar_t** pp) { return reinterpret_cast<AtlStrT<wchar_t>*>(pp); }

// OpenMFC's CString is the single-pointer ATL layout AtlStrT wraps (detail/AtlStrTCore.h);
// AFXGetRegPath's by-value return is received into such a pointer below.
static_assert(sizeof(CString) == sizeof(wchar_t*), "CString is { m_pszData }");

// Retail's afxMouseManager (0x1803be268, mfc140u; not exported).  See the file header.
void* g_pAfxMouseManager = nullptr;

// The member-touching thunks' guard (DEVIATION, see the file header).
inline S_CMouseManager* Live(const void* pThis) {
    return (pThis != nullptr && pThis == g_pAfxMouseManager) ? Self(const_cast<void*>(pThis)) : nullptr;
}

// Retail's two wide-string literals, read out of the mfc140u image at the addresses the
// LoadState/SaveState call sites load: 0x33fd98 (AFXGetRegPath postfix) and 0x33fdb8
// (the value name).
const wchar_t kMouseProfile[]   = L"MouseManager";
const wchar_t kMouseEntryName[] = L"Mouse";

// ---- CMap helpers -------------------------------------------------------------------

template <class A>
void MM_MapInit(MM_Map<A>* m) {
    m->vfptr = nullptr;           // retail: CMap<...> vftable (see header)
    m->m_pHashTable = nullptr;
    m->m_nHashTableSize = 17;
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    m->m_pBlocks = nullptr;
    m->m_nBlockSize = 10;
}

// ??$HashKey@PEB_W@@YAIPEB_W@Z (RVA 0x2731c0): NULL key -> AfxThrowInvalidArgException;
// otherwise FNV-1 order (multiply by 0x01000193, then xor the character) seeded with
// 0x811c9dc5 over every (len/10 + 1)-th character.
UINT MM_HashStr(const wchar_t* key) {
    if (key == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    UINT nHash = 0x811c9dc5u;
    const UINT len = static_cast<UINT>(std::wcslen(key));
    const UINT step = len / 10 + 1;
    for (UINT i = 0; i < len; i += step) nHash = (nHash * 0x01000193u) ^ static_cast<unsigned short>(key[i]);
    return nHash;
}

// The integral HashKey inlined in GetAssocAt 0x236e0 and SetCommandForDblClk 0x90ce0:
//   ldiv(key, 127773) (IAT 0x1802c7868 -> ucrt ldiv); v = 16807*rem - 2836*quot;
//   if (v < 0) v += 0x7fffffff; return (UINT)v;
UINT MM_HashInt(int key) {
    const long quot = static_cast<long>(key) / 127773L;
    const long rem  = static_cast<long>(key) % 127773L;
    int v = static_cast<int>(16807L * rem - 2836L * quot);
    if (v < 0) v += 0x7fffffff;
    return static_cast<UINT>(v);
}

// GetAssocAt (string map, RVA 0x314e0): hash, bucket = hash % m_nHashTableSize (both
// reported to the caller), NULL when there is no table, else walk the bucket comparing
// nHashValue and then wcscmp(key) == 0 (CompareElements 0x31558, IAT 0x1802c7770 -> wcscmp).
MM_StrAssoc* MM_StrGetAssocAt(const MM_StrMap* m, const wchar_t* key, UINT& nBucket, UINT& nHash) {
    nHash = MM_HashStr(key);
    nBucket = nHash % m->m_nHashTableSize;
    if (m->m_pHashTable == nullptr) return nullptr;
    for (MM_StrAssoc* p = m->m_pHashTable[nBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHash && std::wcscmp(p->key, key) == 0) return p;
    }
    return nullptr;
}

// GetAssocAt (int map, RVA 0x236e0): same shape, comparing nHashValue then the key.
MM_IntAssoc* MM_IntGetAssocAt(const MM_IntMap* m, int key, UINT& nBucket, UINT& nHash) {
    nHash = MM_HashInt(key);
    nBucket = nHash % m->m_nHashTableSize;
    if (m->m_pHashTable == nullptr) return nullptr;
    for (MM_IntAssoc* p = m->m_pHashTable[nBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHash && p->key == key) return p;
    }
    return nullptr;
}

// The shared first half of both SetAt helpers (0x90dc4 / 0x2346c) for a key that is not
// in the map yet: InitHashTable on first use (zeroed table of m_nHashTableSize slots,
// then ENSURE(m_pHashTable)), then NewAssoc -- refill the free list from a new CPlex
// block of m_nBlockSize assocs chained last-to-first when it is empty, ENSURE a free
// assoc, zero it (keeping its pNext), pop it, ++m_nCount.  The caller constructs the key,
// stores nHashValue and links the assoc at the head of its bucket.
template <class A>
A* MM_NewAssoc(MM_Map<A>* m) {
    if (m->m_pHashTable == nullptr) {
        const UINT n = m->m_nHashTableSize;
        A** table = static_cast<A**>(std::malloc(sizeof(A*) * (n ? n : 1)));
        if (table == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        std::memset(table, 0, sizeof(A*) * n);
        m->m_pHashTable = table;
        m->m_nHashTableSize = n;
    }
    if (m->m_pFreeList == nullptr) {
        const INT_PTR nBlock = m->m_nBlockSize;
        MM_Plex* pNew = static_cast<MM_Plex*>(std::malloc(sizeof(MM_Plex) + sizeof(A) * nBlock));
        if (pNew == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        pNew->pNext = m->m_pBlocks;
        m->m_pBlocks = pNew;
        A* pFirst = reinterpret_cast<A*>(pNew + 1);
        for (INT_PTR i = nBlock - 1; i >= 0; --i) {
            pFirst[i].pNext = m->m_pFreeList;
            m->m_pFreeList = &pFirst[i];
        }
    }
    A* pAssoc = m->m_pFreeList;
    if (pAssoc == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    A* pSavedNext = pAssoc->pNext;
    std::memset(pAssoc, 0, sizeof(A));
    pAssoc->pNext = pSavedNext;
    m->m_pFreeList = pAssoc->pNext;
    ++m->m_nCount;
    return pAssoc;
}

// SetAt, string map (RVA 0x90dc4): the key CString is constructed from the LPCTSTR.
void MM_StrSetAt(MM_StrMap* m, const wchar_t* key, int value) {
    UINT nBucket = 0, nHash = 0;
    MM_StrAssoc* pAssoc = MM_StrGetAssocAt(m, key, nBucket, nHash);
    if (pAssoc == nullptr) {
        pAssoc = MM_NewAssoc(m);
        Str(&pAssoc->key)->ConstructPsz(key);
        pAssoc->nHashValue = nHash;
        pAssoc->pNext = m->m_pHashTable[nBucket];
        m->m_pHashTable[nBucket] = pAssoc;
    }
    pAssoc->value = value;
}

// SetAt, int map (RVA 0x2346c).
void MM_IntSetAt(MM_IntMap* m, int key, UINT value) {
    UINT nBucket = 0, nHash = 0;
    MM_IntAssoc* pAssoc = MM_IntGetAssocAt(m, key, nBucket, nHash);
    if (pAssoc == nullptr) {
        pAssoc = MM_NewAssoc(m);
        pAssoc->key = key;
        pAssoc->nHashValue = nHash;
        pAssoc->pNext = m->m_pHashTable[nBucket];
        m->m_pHashTable[nBucket] = pAssoc;
    }
    pAssoc->value = value;
}

// RemoveAll, int map (RVA 0x1ba40): free the hash table, zero m_nCount and m_pFreeList,
// free the CPlex chain.  m_nHashTableSize is kept.
void MM_IntRemoveAll(MM_IntMap* m) {
    if (m->m_pHashTable != nullptr) {
        std::free(m->m_pHashTable);
        m->m_pHashTable = nullptr;
    }
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    for (MM_Plex* p = m->m_pBlocks; p != nullptr;) {
        MM_Plex* pNext = p->pNext;
        std::free(p);
        p = pNext;
    }
    m->m_pBlocks = nullptr;
}

// The RemoveAll inlined in ~CMap<CString,...> (RVA 0x90f00): as above, but every key
// CString still in the table is released first.
void MM_StrRemoveAll(MM_StrMap* m) {
    if (m->m_pHashTable != nullptr) {
        for (UINT nHash = 0; nHash < m->m_nHashTableSize; ++nHash) {
            for (MM_StrAssoc* p = m->m_pHashTable[nHash]; p != nullptr; p = p->pNext) {
                Str(&p->key)->Destruct();
            }
        }
        std::free(m->m_pHashTable);
        m->m_pHashTable = nullptr;
    }
    m->m_nCount = 0;
    m->m_pFreeList = nullptr;
    for (MM_Plex* p = m->m_pBlocks; p != nullptr;) {
        MM_Plex* pNext = p->pNext;
        std::free(p);
        p = pNext;
    }
    m->m_pBlocks = nullptr;
}

// GetStartPosition (inline in afxtempl.h; retail's `neg/sbb` on m_nCount):
// m_nCount ? BEFORE_START_POSITION : NULL.
template <class A>
inline void* MM_GetStartPosition(const MM_Map<A>* m) {
    return m->m_nCount != 0 ? reinterpret_cast<void*>(static_cast<INT_PTR>(-1)) : nullptr;
}

// GetNextAssoc (RVA 0x910e4 for the int map; the string map's copy is inlined in
// GetViewNames 0x905a0 with the same shape): ENSURE(m_pHashTable && rNextPosition), from
// BEFORE_START_POSITION find the first non-empty bucket (ENSURE one exists), then advance
// rNextPosition to pNext or to the head of the next non-empty bucket.  The assoc is
// returned; the callers copy key/value out of it.
template <class A>
A* MM_GetNextAssoc(const MM_Map<A>* m, void*& rNextPosition) {
    A* pAssocRet = static_cast<A*>(rNextPosition);
    if (m->m_pHashTable == nullptr || pAssocRet == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    if (pAssocRet == reinterpret_cast<A*>(static_cast<INT_PTR>(-1))) {
        pAssocRet = nullptr;
        for (UINT nBucket = 0; nBucket < m->m_nHashTableSize; ++nBucket) {
            if ((pAssocRet = m->m_pHashTable[nBucket]) != nullptr) break;
        }
        if (pAssocRet == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    }
    A* pAssocNext = pAssocRet->pNext;
    if (pAssocNext == nullptr) {
        for (UINT nBucket = (pAssocRet->nHashValue % m->m_nHashTableSize) + 1;
             nBucket < m->m_nHashTableSize; ++nBucket) {
            if ((pAssocNext = m->m_pHashTable[nBucket]) != nullptr) break;
        }
    }
    rNextPosition = pAssocNext;
    return pAssocRet;
}

// ---- CArchive primitives ------------------------------------------------------------
// Retail inlines CArchive's 4-byte operator<< / operator>> into Serialize (0x90ac0):
//   loading: if (m_lpBufCur + 4 > m_lpBufMax) FillBuffer(...)   -- FillBuffer 0x1d1cc0
//            throws CArchiveException::endOfFile (3, NULL name, at 0x1d1e55) when the
//            file cannot supply the bytes; then copy 4 bytes out.
//   storing: if (m_lpBufCur + 4 > m_lpBufMax) Flush();  copy 4 bytes in.
// OpenMFC's CArchive (include/openmfc/afx.h) has a different member layout, so these go
// through its Read/Write thunks instead; a short Read raises the same endOfFile exception
// FillBuffer would.  (Retail's per-primitive mode test -- testb $1,0x20(ar) -- cannot fail
// inside the branch that already tested it, so it is not repeated.)
template <class T>
T MM_ArGet(CArchive* ar) {
    T value{};
    if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &value, sizeof(value)) != sizeof(value))
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(3 /* CArchiveException::endOfFile */, nullptr);
    return value;
}
template <class T>
void MM_ArPut(CArchive* ar, T value) {
    impl__Write_CArchive__QEAAXPEBXI_Z(ar, &value, sizeof(value));
}

// ---- the 16-byte CSettingsStoreSP owner object ----------------------------------------
// Retail builds CSettingsStoreSP on the stack ({m_pRegistry, m_dwUserData}, both zeroed:
// the two qword stores at 0x40/0x48(%rsp) in LoadState 0x90700), calls
// ?Create@CSettingsStoreSP@@ (0x12a550) and on scope exit deletes m_pRegistry through its
// virtual deleting destructor (vslot 0x08, flag 1).  Hand-rolled the same way as in
// featurepack/toolbar/CMFCToolBar.cpp; the store is an OpenMFC CSettingsStore, so the
// C++ virtual destructor is the right way to delete it.
struct MM_SettingsStoreSP {
    void* slots[2] = { nullptr, nullptr };
    void* Create(int bAdmin, int bReadOnly) {
        return impl__Create_CSettingsStoreSP__QEAAAEAVCSettingsStore__HH_Z(slots, bAdmin, bReadOnly);
    }
    ~MM_SettingsStoreSP() {
        if (slots[0]) delete static_cast<CObject*>(slots[0]);
    }
};

// Receives AFXGetRegPath(kMouseProfile, lpszProfileName) -- a by-value CString -- and
// releases it on scope exit.
struct MM_RegPath {
    wchar_t* psz = nullptr;
    explicit MM_RegPath(const wchar_t* lpszProfileName) {
        impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(
            reinterpret_cast<CString*>(&psz), kMouseProfile, lpszProfileName);
    }
    ~MM_RegPath() { if (psz) Str(&psz)->Destruct(); }
};

// ---- vtable slots -----------------------------------------------------------------------

// Slot 1, the scalar deleting destructor (RVA 0x90390): ~CMouseManager(); if (flags & 1)
// { if (flags & 4) call 0x27d0 -- a bare `ret`, i.e. nothing -- else free(this) (IAT
// 0x1802c74e8 -> ucrt free) }; return this.
void* MS_ABI MM_DeletingDtor(void* pThis, unsigned int flags) {
    impl___1CMouseManager__UEAA_XZ(pThis);
    if ((flags & 1) != 0 && (flags & 4) == 0) std::free(pThis);
    return pThis;
}

// Slots 3 and 4 (AssertValid, Dump): both are RVA 0x27d0, a bare `ret`.
void MS_ABI MM_NoOp(const void*, void*) {}

// The MSVC-layout vftable installed by the constructor (retail 0x1802eff08, mfc140u).
void* const kMouseManagerVtbl[5] = {
    reinterpret_cast<void*>(&impl__GetRuntimeClass_CMouseManager__UEBAPEAUCRuntimeClass__XZ), // 0 GetRuntimeClass (0x902a0)
    reinterpret_cast<void*>(&MM_DeletingDtor),                                                // 1 {deleting dtor} (0x90390)
    reinterpret_cast<void*>(&impl__Serialize_CMouseManager__MEAAXAEAVCArchive___Z),           // 2 Serialize       (0x90ac0)
    reinterpret_cast<void*>(&MM_NoOp),                                                        // 3 AssertValid     (0x27d0)
    reinterpret_cast<void*>(&MM_NoOp),                                                        // 4 Dump            (0x27d0)
};

// Retail calls Serialize through vslot 2 (`call *0x10(%rax)` in LoadState/SaveState), so a
// derived class's override runs; dispatch the same way.
inline void MM_VirtualSerialize(void* pThis, CArchive* ar) {
    using Fn = void (MS_ABI*)(void*, CArchive*);
    reinterpret_cast<Fn>((*static_cast<void***>(pThis))[2])(pThis, ar);
}

}  // namespace

// Accessor for retail's afxMouseManager (not an export).  Unused inside this file; it is
// here so core/app/CWinAppEx.cpp (InitMouseManager/GetMouseManager) and
// featurepack/customize/CMFCToolBarsCustomizeDialog.cpp (the "is there a mouse manager"
// gate) can read the singleton without a C++-linkage reference -- see headerRequests.
extern "C" void* OpenMfcGetAfxMouseManager() {
    return g_pAfxMouseManager;
}

// ---------------------------------------------------------------------------------------
// Exported thunks
// ---------------------------------------------------------------------------------------

// Retail RVA 0x902e0.  Not on the stub list this file was assigned from (its placeholder
// `return pThis;` was not counted as a stub), but it initialised nothing, and every body
// below depends on the maps it sets up, so it is transcribed as well:
//   vfptr = CMouseManager vftable; the three maps as in the file header;
//   ENSURE(afxMouseManager == NULL)   -> AfxThrowInvalidArgException (0x227720) otherwise;
//   afxMouseManager = this; return this;
// The ENSURE comes after the member initialisation, as in retail.
// Symbol: ??0CMouseManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMouseManager__QEAA_XZ(void* pThis) {
    S_CMouseManager* self = Self(pThis);
    self->vfptr = const_cast<void**>(kMouseManagerVtbl);
    MM_MapInit(&self->m_ViewsNames);
    MM_MapInit(&self->m_ViewsToCommands);
    MM_MapInit(&self->m_ViewsToIcons);
    if (g_pAfxMouseManager != nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return pThis;   // not reached: the thunk raises
    }
    g_pAfxMouseManager = pThis;
    return pThis;
}

// Retail RVA 0x903e0:
//   vfptr = CMouseManager vftable; afxMouseManager = NULL;
//   m_ViewsToIcons: map vftable, RemoveAll (0x1ba40);
//   m_ViewsToCommands: map vftable, RemoveAll (0x1ba40);
//   m_ViewsNames: tail-jump to ~CMap<CString,...> (0x90f00: RemoveAll releasing the keys).
// Guarded (file header): a pointer that is not the live manager is left untouched.
// Symbol: ??1CMouseManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CMouseManager__UEAA_XZ(void* pThis) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return;
    self->vfptr = const_cast<void**>(kMouseManagerVtbl);
    g_pAfxMouseManager = nullptr;
    MM_IntRemoveAll(&self->m_ViewsToIcons);
    MM_IntRemoveAll(&self->m_ViewsToCommands);
    MM_StrRemoveAll(&self->m_ViewsNames);
}

// Retail RVA 0x90440:
//   CString strName;                                   // nil, from the string manager
//   ENSURE(strName.LoadString(uiViewNameResId));       // AfxFindStringResourceHandle
//                                                      // (0x2aee00) == NULL, or
//                                                      // CStringT::LoadStringW(hInst, id)
//                                                      // (0xdb70) == FALSE -> both call
//                                                      // AfxThrowInvalidArgException
//   return AddView(iViewId, strName, uiIconId);        // 0x904e0
// CStringT::LoadStringW(HINSTANCE, UINT) fails exactly when the string-table entry is
// missing or empty (ATL's AtlGetStringResourceImage returns NULL for nLength == 0);
// ::LoadStringW in its read-only mode (cchBufferMax == 0) returns that entry's length,
// 0 in the same two cases, so it is used for the lookup and the text is copied out.
// Not guarded itself: it touches no member, and the AddView it calls is guarded.
// Symbol: ?AddView@CMouseManager@@QEAAHHII@Z
extern "C" int MS_ABI impl__AddView_CMouseManager__QEAAHHII_Z(void* pThis, int iViewId, unsigned int uiViewNameResId, unsigned int uiIconId) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(uiViewNameResId));
    if (hInst == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // not reached
    }
    const wchar_t* pRes = nullptr;
    const int nLen = ::LoadStringW(hInst, uiViewNameResId, reinterpret_cast<LPWSTR>(&pRes), 0);
    if (nLen <= 0 || pRes == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // not reached
    }
    wchar_t* strName = nullptr;
    Str(&strName)->ConstructPszN(pRes, nLen);
    const int bResult = impl__AddView_CMouseManager__QEAAHHPEB_WI_Z(pThis, iViewId, strName, uiIconId);
    Str(&strName)->Destruct();
    return bResult;
}

// Retail RVA 0x904e0:
//   ENSURE(lpszViewName != NULL);                      // AfxThrowInvalidArgException
//   int iTmp;
//   if (m_ViewsNames.Lookup(lpszViewName, iTmp))       // GetAssocAt 0x314e0
//       return FALSE;
//   m_ViewsNames.SetAt(lpszViewName, iId);             // 0x90dc4
//   if (uiIconId != 0)
//       m_ViewsToIcons.SetAt(iId, uiIconId);           // 0x2346c
//   return TRUE;
// Symbol: ?AddView@CMouseManager@@QEAAHHPEB_WI@Z
extern "C" int MS_ABI impl__AddView_CMouseManager__QEAAHHPEB_WI_Z(void* pThis, int iId, const wchar_t* lpszViewName, unsigned int uiIconId) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return FALSE;
    if (lpszViewName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;   // not reached
    }
    UINT nBucket = 0, nHash = 0;
    if (MM_StrGetAssocAt(&self->m_ViewsNames, lpszViewName, nBucket, nHash) != nullptr) return FALSE;
    MM_StrSetAt(&self->m_ViewsNames, lpszViewName, iId);
    if (uiIconId != 0) MM_IntSetAt(&self->m_ViewsToIcons, iId, uiIconId);
    return TRUE;
}

// Retail RVA 0x90270: p = ::operator new(0xb0) (0x27f0); if (p) CMouseManager::CMouseManager(p)
// (0x902e0); return p.  OpenMFC's ??2@YAPEAX_K@Z thunk is the allocator here.  (Retail's
// new-expression also frees the block if the constructor throws; that EH cleanup is not
// reproduced -- see "exceptions" in the file header.)
// Symbol: ?CreateObject@CMouseManager@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMouseManager__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_CMouseManager));
    if (p != nullptr) impl___0CMouseManager__QEAA_XZ(p);
    return p;
}

// Retail RVA 0x90560: UINT uiCmd; return m_ViewsToCommands.Lookup(iId, uiCmd) ? uiCmd : 0;
// (GetAssocAt 0x236e0 on +0x40, value at assoc+0x4).
// Symbol: ?GetViewDblClickCommand@CMouseManager@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetViewDblClickCommand_CMouseManager__QEBAIH_Z(const void* pThis, int iId) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return 0;
    UINT nBucket = 0, nHash = 0;
    const MM_IntAssoc* pAssoc = MM_IntGetAssocAt(&self->m_ViewsToCommands, iId, nBucket, nHash);
    return pAssoc != nullptr ? pAssoc->value : 0;
}

// Retail RVA 0x90d90: UINT uiIconId; return m_ViewsToIcons.Lookup(iViewId, uiIconId) ? uiIconId : 0;
// (GetAssocAt 0x236e0 on +0x78, value at assoc+0x4).
// Symbol: ?GetViewIconId@CMouseManager@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetViewIconId_CMouseManager__QEBAIH_Z(const void* pThis, int iViewId) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return 0;
    UINT nBucket = 0, nHash = 0;
    const MM_IntAssoc* pAssoc = MM_IntGetAssocAt(&self->m_ViewsToIcons, iViewId, nBucket, nHash);
    return pAssoc != nullptr ? pAssoc->value : 0;
}

// Retail RVA 0x906c0:
//   ENSURE(lpszName != NULL);                          // AfxThrowInvalidArgException
//   int iId; return m_ViewsNames.Lookup(lpszName, iId) ? iId : -1;   // GetAssocAt 0x314e0
// Symbol: ?GetViewIdByName@CMouseManager@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__GetViewIdByName_CMouseManager__QEBAHPEB_W_Z(const void* pThis, const wchar_t* lpszName) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return -1;
    if (lpszName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return -1;   // not reached
    }
    UINT nBucket = 0, nHash = 0;
    const MM_StrAssoc* pAssoc = MM_StrGetAssocAt(&self->m_ViewsNames, lpszName, nBucket, nHash);
    return pAssoc != nullptr ? pAssoc->value : -1;
}

// Retail RVA 0x905a0:
//   listOfNames.RemoveAll();                           // CStringList::RemoveAll (inlined
//                                                      // copy at 0x232300)
//   for (POSITION pos = m_ViewsNames.GetStartPosition(); pos != NULL;) {
//       CString strName; int iId;
//       m_ViewsNames.GetNextAssoc(pos, strName, iId);  // inlined, ENSUREs as MM_GetNextAssoc
//       listOfNames.AddTail(strName);                  // AddTail(const CString&) 0x2325f0
//   }
// Deviation: the name is handed to CStringList::AddTail(LPCTSTR) instead of through a
// temporary CString copy; the list receives an equal string either way.
// Symbol: ?GetViewNames@CMouseManager@@QEBAXAEAVCStringList@@@Z
extern "C" void MS_ABI impl__GetViewNames_CMouseManager__QEBAXAEAVCStringList___Z(const void* pThis, void* listOfNames) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return;
    impl__RemoveAll_CStringList__QEAAXXZ(listOfNames);
    for (void* pos = MM_GetStartPosition(&self->m_ViewsNames); pos != nullptr;) {
        const MM_StrAssoc* pAssoc = MM_GetNextAssoc(&self->m_ViewsNames, pos);
        impl__AddTail_CStringList__QEAAPEAU__POSITION__PEB_W_Z(listOfNames, pAssoc->key);
    }
}

// Retail RVA 0x90700:
//   CString strProfileName = ::AFXGetRegPath(L"MouseManager", lpszProfileName);   // 0xd2070
//   BOOL bResult = FALSE; LPBYTE lpbData = NULL; UINT uiDataSize;
//   CSettingsStoreSP regSP;
//   CSettingsStore& reg = regSP.Create(FALSE, TRUE);                               // 0x12a550
//   if (!reg.Open(strProfileName)) return FALSE;                                   // vslot 0x30
//   if (!reg.Read(L"Mouse", &lpbData, &uiDataSize)) return FALSE;                  // vslot 0x98
//   try {
//       CMemFile file(lpbData, uiDataSize);                                        // 0x22aec0 (nGrowBytes 0)
//       CArchive ar(&file, CArchive::load);                                        // 0x1d1550 (mode 1, 0x1000, NULL)
//       Serialize(ar);                                                             // vslot 0x10
//       bResult = TRUE;
//   } catch (CMemoryException*) / catch (CArchiveException*) { ... }               // ~CArchive 0x1d1700, ~CMemFile 0x22afa0
//   if (lpbData != NULL) free(lpbData);                                            // IAT 0x1802c74e8 -> ucrt free
//   return bResult;      // (regSP's store and strProfileName are released on every return)
// Deviations: (1) the catch arms (file header).  (2) Open/Read are called through their
// exported thunks, not the store's vtable, so a CSettingsStore subclass installed with
// CSettingsStoreSP::SetRuntimeClass is not dispatched to (as in CMFCToolBar.cpp).
// (3) The buffer is released with ::LocalFree, not free: OpenMFC's
// CSettingsStore::Read (core/app/CSettingsStore.cpp) allocates it with LocalAlloc, and
// freeing that with the CRT would corrupt the heap.
// Symbol: ?LoadState@CMouseManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadState_CMouseManager__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    if (Live(pThis) == nullptr) return FALSE;
    MM_RegPath strProfileName(lpszProfileName);
    MM_SettingsStoreSP regSP;
    void* pStore = regSP.Create(FALSE, TRUE);
    if (pStore == nullptr || !impl__Open_CSettingsStore__UEAAHPEB_W_Z(pStore, strProfileName.psz)) return FALSE;

    unsigned char* lpbData = nullptr;
    unsigned int uiDataSize = 0;
    if (!impl__Read_CSettingsStore__UEAAHPEB_WPEAPEAEPEAI_Z(pStore, kMouseEntryName, &lpbData, &uiDataSize)) return FALSE;

    BOOL bResult = FALSE;
    {
        alignas(16) unsigned char fileStorage[sizeof(CMemFile)];
        CMemFile* pFile = static_cast<CMemFile*>(impl___0CMemFile__QEAA_PEAEII_Z(fileStorage, lpbData, uiDataSize, 0));
        alignas(16) unsigned char arStorage[sizeof(CArchive)];
        CArchive* pAr = static_cast<CArchive*>(impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(
            arStorage, pFile, 1 /* CArchive::load */, 0x1000, nullptr));
        MM_VirtualSerialize(pThis, pAr);
        bResult = TRUE;
        impl___1CArchive__QEAA_XZ(pAr);
        impl___1CMemFile__UEAA_XZ(pFile);
    }
    if (lpbData != nullptr) ::LocalFree(lpbData);
    return bResult;
}

// Retail RVA 0x908f0:
//   CString strProfileName = ::AFXGetRegPath(L"MouseManager", lpszProfileName);   // 0xd2070
//   BOOL bResult = FALSE;
//   try {
//       CMemFile file;                                                             // 0x22ae70 (nGrowBytes 0x400)
//       { CArchive ar(&file, CArchive::store);                                     // 0x1d1550 (mode 0, 0x1000, NULL)
//         Serialize(ar);                                                           // vslot 0x10
//         ar.Flush(); }                                                            // 0x1d1be0, ~CArchive 0x1d1700
//       UINT uiDataSize = (UINT)file.GetLength();                                  // inlined: file.m_nFileSize
//       LPBYTE lpbData = file.Detach();                                            // inlined
//       if (lpbData != NULL) {
//           CSettingsStoreSP regSP;
//           CSettingsStore& reg = regSP.Create(FALSE, FALSE);                      // 0x12a550
//           if (reg.CreateKey(strProfileName))                                     // vslot 0x28
//               bResult = reg.Write(L"Mouse", lpbData, uiDataSize);                // vslot 0x60
//           free(lpbData);                                                         // IAT 0x1802c74e8
//       }                                                                          // regSP releases its store
//   } catch (CMemoryException*) / catch (CArchiveException*) { ... }               // ~CMemFile 0x22afa0
//   return bResult;
// Deviations: the catch arms (file header); CreateKey/Write through their exported
// thunks rather than the store's vtable (see LoadState).  OpenMFC's CMemFile grows its
// buffer with realloc, so the detached buffer is released with free as in retail.
// Symbol: ?SaveState@CMouseManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveState_CMouseManager__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpszProfileName) {
    if (Live(pThis) == nullptr) return FALSE;
    MM_RegPath strProfileName(lpszProfileName);
    BOOL bResult = FALSE;

    alignas(16) unsigned char fileStorage[sizeof(CMemFile)];
    CMemFile* pFile = static_cast<CMemFile*>(impl___0CMemFile__QEAA_I_Z(fileStorage, 0x400));
    {
        alignas(16) unsigned char arStorage[sizeof(CArchive)];
        CArchive* pAr = static_cast<CArchive*>(impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(
            arStorage, pFile, 0 /* CArchive::store */, 0x1000, nullptr));
        MM_VirtualSerialize(pThis, pAr);
        impl__Flush_CArchive__QEAAXXZ(pAr);
        impl___1CArchive__QEAA_XZ(pAr);
    }
    const unsigned int uiDataSize = static_cast<unsigned int>(impl__GetLength_CMemFile__UEBA_KXZ(pFile));
    unsigned char* lpbData = impl__Detach_CMemFile__QEAAPEAEXZ(pFile);
    if (lpbData != nullptr) {
        MM_SettingsStoreSP regSP;
        void* pStore = regSP.Create(FALSE, FALSE);
        if (pStore != nullptr && impl__CreateKey_CSettingsStore__UEAAHPEB_W_Z(pStore, strProfileName.psz)) {
            bResult = impl__Write_CSettingsStore__UEAAHPEB_WPEAEI_Z(pStore, kMouseEntryName, lpbData, uiDataSize);
        }
        std::free(lpbData);
    }
    impl___1CMemFile__UEAA_XZ(pFile);
    return bResult;
}

// Retail RVA 0x90ac0 (only m_ViewsToCommands is persisted):
//   if (ar.IsLoading()) {                              // testb $1,0x20(ar)
//       m_ViewsToCommands.RemoveAll();                 // 0x1ba40
//       int nCount; ar >> nCount;
//       for (int i = 0; i < nCount; i++) {
//           int iViewId; UINT uiCmd;
//           ar >> iViewId; ar >> uiCmd;
//           m_ViewsToCommands.SetAt(iViewId, uiCmd);   // 0x2346c
//       }
//   } else {
//       ar << (int)m_ViewsToCommands.GetCount();       // low 32 bits of m_nCount (+0x58)
//       for (POSITION pos = m_ViewsToCommands.GetStartPosition(); pos != NULL;) {
//           int iViewId; UINT uiCmd;
//           m_ViewsToCommands.GetNextAssoc(pos, iViewId, uiCmd);   // 0x910e4
//           ar << iViewId << uiCmd;
//       }
//   }
// The primitive reads/writes are MM_ArGet/MM_ArPut (see above).
// Symbol: ?Serialize@CMouseManager@@MEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMouseManager__MEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr || ar == nullptr) return;
    MM_IntMap* m = &self->m_ViewsToCommands;
    if (ar->IsLoading()) {
        MM_IntRemoveAll(m);
        const int nCount = MM_ArGet<int>(ar);
        for (int i = 0; i < nCount; i++) {
            const int iViewId = MM_ArGet<int>(ar);
            const UINT uiCmd = MM_ArGet<UINT>(ar);
            MM_IntSetAt(m, iViewId, uiCmd);
        }
    } else {
        MM_ArPut<int>(ar, static_cast<int>(m->m_nCount));
        for (void* pos = MM_GetStartPosition(m); pos != nullptr;) {
            const MM_IntAssoc* pAssoc = MM_GetNextAssoc(m, pos);
            MM_ArPut<int>(ar, pAssoc->key);
            MM_ArPut<UINT>(ar, pAssoc->value);
        }
    }
}

// Retail RVA 0x90ce0:
//   if (uiCmd != 0) { m_ViewsToCommands.SetAt(iViewId, uiCmd); return; }   // 0x2346c
//   m_ViewsToCommands.RemoveKey(iViewId);              // inlined:
//       if (m_pHashTable == NULL) return;
//       walk &m_pHashTable[hash % size] via pNext links; on nHashValue == hash && key == iViewId:
//           unlink; FreeAssoc: pAssoc->pNext = m_pFreeList; --m_nCount; m_pFreeList = pAssoc;
//           if (m_nCount == 0) RemoveAll();            // 0x1ba40
// Symbol: ?SetCommandForDblClk@CMouseManager@@QEAAXHI@Z
extern "C" void MS_ABI impl__SetCommandForDblClk_CMouseManager__QEAAXHI_Z(void* pThis, int iViewId, unsigned int uiCmd) {
    S_CMouseManager* self = Live(pThis);
    if (self == nullptr) return;
    MM_IntMap* m = &self->m_ViewsToCommands;
    if (uiCmd != 0) {
        MM_IntSetAt(m, iViewId, uiCmd);
        return;
    }
    if (m->m_pHashTable == nullptr) return;
    const UINT nHash = MM_HashInt(iViewId);
    MM_IntAssoc** ppAssocPrev = &m->m_pHashTable[nHash % m->m_nHashTableSize];
    for (MM_IntAssoc* pAssoc = *ppAssocPrev; pAssoc != nullptr; pAssoc = *ppAssocPrev) {
        if (pAssoc->nHashValue == nHash && pAssoc->key == iViewId) {
            *ppAssocPrev = pAssoc->pNext;
            pAssoc->pNext = m->m_pFreeList;
            --m->m_nCount;
            m->m_pFreeList = pAssoc;
            if (m->m_nCount == 0) MM_IntRemoveAll(m);
            return;
        }
        ppAssocPrev = &pAssoc->pNext;
    }
}
