// CPropertySection — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/CPtrListSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <new>

// ---------------------------------------------------------------------------
// CPropertySection: one [MS-OLEPS] 2.20 "PropertySet" (MFC's src/mfc/oleprop.h
// calls it a section; the OLEPS "PropertySetStream" is MFC's CPropertySet).
// The class is MFC-internal -- it is not declared in any OpenMFC public header
// -- so this file is the only place its layout lives.  Client (MSVC) code
// allocates the object with the retail member layout and calls the exported
// thunks below on it, so every thunk takes a `void* pThis` and views it through
// PS_Section.
//
// Retail object layout, decoded from the two constructors and the destructor
// (mfc140u: ??0CPropertySection@@QEAA@XZ at RVA 0x2623d0,
// ??0CPropertySection@@QEAA@U_GUID@@@Z at 0x262490, ??1 at 0x262560):
//
//   +0x00  CLSID            m_FormatID        default ctor copies 16 zero bytes
//                                             from 0x2d98d8 (GUID_NULL); the
//                                             CLSID ctor copies *pFormatID
//   +0x10  SECTIONHEADER    m_SH              { DWORD cbSection; DWORD cProperties; }
//                                             zeroed (movq $0,0x10) -- the OLEPS
//                                             PropertySet Size / NumProperties
//   +0x18  CPtrList         m_PropList        vftable 0x32a758 (mfc140u, slot 0 =
//                                             CPtrList::GetRuntimeClass) -- so it
//                                             is a CPtrList, and GetList is exported
//                                             as returning CPtrList*.  head/tail/
//                                             count/free/blocks = 0, m_nBlockSize
//                                             (+0x48) = 10: CPtrList::CPtrList(10)
//   +0x50  CMapStringToPtr  m_NameDict        vftable 0x32a8d8 (mfc140u, slot 0 =
//                                             CMapStringToPtr::GetRuntimeClass);
//                                             m_pHashTable (+0x58) = 0,
//                                             m_nHashTableSize (+0x60) = 17,
//                                             m_nCount (+0x68) = 0, m_pFreeList
//                                             (+0x70) = 0, m_pBlocks (+0x78) = 0,
//                                             m_nBlockSize (+0x80) = 10
//   +0x88  CString          m_strSectionName  m_pszData = string manager
//                                             GetNilString() (vtable slot 3 on
//                                             the manager at 0x3b25e8) + 0x18
//
// sizeof == 0x90.  The destructor (0x262560) runs, in order: RemoveAll()
// (0x262770), the inlined CString destructor on +0x88, the inlined
// ~CMapStringToPtr (re-store the vftable, then CMapStringToPtr::RemoveAll at
// 0x2332e0), and finally the inlined ~CPtrList (re-store the vftable, then
// tail-jump to CPtrList::RemoveAll at 0x8350).
//
// CProperty (core/ole/CProperty.cpp, owned by another agent) is the element
// type.  Its layout is pinned by its own retail constructors and by the
// operator new(0x10) at 0x180262621 inside CPropertySection::Set (0x2625f0):
//
//   +0x00  DWORD  m_dwPropID      ??0CProperty@@QEAA@KQEAXK@Z (0x261970) stores edx
//   +0x04  DWORD  m_dwType        ... stores r9d
//   +0x08  LPVOID m_pValue        ... stores 0, then calls CProperty::Set(LPVOID)
//
// sizeof == 0x10.  Every property search in this class is the same inlined
// walk of m_PropList's CNode chain (pNext at +0, pPrev at +8, data at +0x10)
// comparing ((CProperty*)node->data)->m_dwPropID with the requested id; it is
// transcribed once as ps_find_node below.
//
// How the sub-objects are realised in OpenMFC:
//   * m_PropList is a real OpenMFC_CPtrList (detail/CPtrListSupport.h), which
//     is layout-faithful to retail's CPtrList (sizeof 0x38, CNode {pNext, pPrev,
//     data}); it is constructed/destroyed through the exported CPtrList thunks
//     and its nodes are walked directly, exactly as retail inlines the walk.
//   * m_strSectionName is an OpenMFC CString (include/openmfc/afxstr.h), a
//     single ATL-compatible m_pszData pointer -- the same 8 bytes retail keeps.
//   * m_NameDict cannot be an OpenMFC CMapStringToPtr: that class keeps its
//     state in a side table keyed by `this` (core/collections/CMapStringToPtr.cpp)
//     and has no exported way to obtain a start POSITION (retail's
//     GetStartPosition is inline, so no such export exists), which
//     WriteNameDictToStream needs.  The dictionary is therefore implemented
//     here, in the retail 0x38-byte footprint and with retail's own member
//     layout, hash function, bucket order and CAssoc layout (all decoded from
//     CMapStringToPtr::GetAssocAt 0x2334e0, operator[] 0x233600, NewAssoc
//     0x2333d0, GetNextAssoc 0x233760 and RemoveAll 0x2332e0, mfc140u).  Two
//     deliberate deviations: the vftable slot at +0x50 is left NULL (nothing
//     in this file dispatches through it, and none of the retail
//     CPropertySection members disassembled here do either -- the destructor
//     calls CMapStringToPtr::RemoveAll directly), and CAssoc nodes are malloc'ed one
//     at a time instead of being carved from CPlex blocks, so m_pFreeList and
//     m_pBlocks stay NULL.
// ---------------------------------------------------------------------------

namespace {

// [MS-OLEPS] 2.20 PropertySet header: Size, NumProperties.  MFC's SECTIONHEADER.
struct PS_SectionHeader {
    DWORD cbSection;
    DWORD cProperties;
};

// [MS-OLEPS] 2.19 PropertyIdentifierAndOffset.  MFC's PROPERTYIDOFFSET.
struct PS_PropertyIdOffset {
    DWORD dwPropID;
    DWORD dwOffset;
};

// Retail CMapStringToPtr::CAssoc (0x20 bytes: NewAssoc at 0x2333d0 passes 0x20
// to CPlex::Create; RemoveAll at 0x2332e0 destroys the CString at +0x10;
// Lookup at 0x2335a0 reads the value at +0x18; GetAssocAt compares +0x8).
struct PS_Assoc {
    PS_Assoc* pNext;
    UINT      nHashValue;
    CString   key;
    void*     value;
};

// Retail CMapStringToPtr member layout (see the constructor decode above).
struct PS_NameDict {
    void*      vfptr;
    PS_Assoc** m_pHashTable;
    UINT       m_nHashTableSize;
    INT_PTR    m_nCount;
    PS_Assoc*  m_pFreeList;
    void*      m_pBlocks;
    INT_PTR    m_nBlockSize;
};

struct PS_Section {
    CLSID            m_FormatID;
    PS_SectionHeader m_SH;
    OpenMFC_CPtrList m_PropList;
    PS_NameDict      m_NameDict;
    CString          m_strSectionName;
};

// Retail CProperty (see the header comment).
struct PS_Property {
    DWORD  m_dwPropID;
    DWORD  m_dwType;
    void*  m_pValue;
};

static_assert(sizeof(PS_SectionHeader) == 8, "SECTIONHEADER is 8 bytes");
static_assert(sizeof(PS_PropertyIdOffset) == 8, "PROPERTYIDOFFSET is 8 bytes");
static_assert(sizeof(PS_Assoc) == 0x20, "CMapStringToPtr::CAssoc is 0x20 bytes");
static_assert(offsetof(PS_Assoc, nHashValue) == 0x08, "CAssoc::nHashValue");
static_assert(offsetof(PS_Assoc, key) == 0x10, "CAssoc::key");
static_assert(offsetof(PS_Assoc, value) == 0x18, "CAssoc::value");
static_assert(sizeof(PS_NameDict) == 0x38, "CMapStringToPtr is 0x38 bytes");
static_assert(offsetof(PS_NameDict, m_pHashTable) == 0x08, "CMapStringToPtr::m_pHashTable");
static_assert(offsetof(PS_NameDict, m_nHashTableSize) == 0x10, "CMapStringToPtr::m_nHashTableSize");
static_assert(offsetof(PS_NameDict, m_nCount) == 0x18, "CMapStringToPtr::m_nCount");
static_assert(offsetof(PS_NameDict, m_pFreeList) == 0x20, "CMapStringToPtr::m_pFreeList");
static_assert(offsetof(PS_NameDict, m_pBlocks) == 0x28, "CMapStringToPtr::m_pBlocks");
static_assert(offsetof(PS_NameDict, m_nBlockSize) == 0x30, "CMapStringToPtr::m_nBlockSize");
static_assert(sizeof(OpenMFC_CPtrList) == 0x38, "CPtrList is 0x38 bytes");
static_assert(offsetof(OpenMFC_CPtrList, m_pNodeHead) == 0x08, "CPtrList::m_pNodeHead");
static_assert(offsetof(OpenMFC_CPtrList, m_nCount) == 0x18, "CPtrList::m_nCount");
static_assert(sizeof(OpenMFC_CPtrList::CNode) == 0x18, "CPtrList::CNode is 0x18 bytes");
static_assert(offsetof(OpenMFC_CPtrList::CNode, data) == 0x10, "CPtrList::CNode::data");
static_assert(sizeof(CString) == 8, "CString is a single m_pszData pointer");
static_assert(offsetof(PS_Section, m_FormatID) == 0x00, "CPropertySection::m_FormatID");
static_assert(offsetof(PS_Section, m_SH) == 0x10, "CPropertySection::m_SH");
static_assert(offsetof(PS_Section, m_SH) + offsetof(PS_SectionHeader, cProperties) == 0x14, "CPropertySection::m_SH.cProperties");
static_assert(offsetof(PS_Section, m_PropList) == 0x18, "CPropertySection::m_PropList");
static_assert(offsetof(PS_Section, m_NameDict) == 0x50, "CPropertySection::m_NameDict");
static_assert(offsetof(PS_Section, m_strSectionName) == 0x88, "CPropertySection::m_strSectionName");
static_assert(sizeof(PS_Section) == 0x90, "CPropertySection is 0x90 bytes");
static_assert(sizeof(PS_Property) == 0x10, "CProperty is 0x10 bytes");
static_assert(offsetof(PS_Property, m_dwType) == 0x04, "CProperty::m_dwType");
static_assert(offsetof(PS_Property, m_pValue) == 0x08, "CProperty::m_pValue");

using PS_Node = OpenMFC_CPtrList::CNode;

inline PS_Section* PS(void* pThis) { return static_cast<PS_Section*>(pThis); }
inline PS_Property* PS_Prop(PS_Node* pNode) { return static_cast<PS_Property*>(pNode->data); }

// Exported operator new / delete (detail/MemcoreSupport.cpp).  Retail allocates
// every CProperty through ??2@YAPEAX_K@Z (0x27f0, mfc140u) and frees it through
// ??3@YAXPEAX@Z (the `call 0x2b77b0` in Remove/RemoveAll is a jump stub onto
// operator delete at 0x27c0), so these bodies do the same.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);

// CPtrList thunks (core/collections/CPtrList.cpp).  Retail names, mfc140u RVAs:
// ??0CPtrList@@QEAA@_J@Z 0x232280 (inlined into the ctors), ??1CPtrList@@UEAA@XZ
// 0x2322b0 (inlined into the dtor as vftable store + RemoveAll 0x8350),
// ?AddTail@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z 0x231e70,
// ?RemoveAt@CPtrList@@QEAAXPEAU__POSITION@@@Z 0x2320d0,
// ?RemoveAll@CPtrList@@QEAAXXZ 0x8350.
extern "C" OpenMFC_CPtrList* MS_ABI impl___0CPtrList__QEAA__J_Z(OpenMFC_CPtrList* pThis, INT_PTR nBlockSize);
extern "C" void        MS_ABI impl___1CPtrList__UEAA_XZ(OpenMFC_CPtrList* pThis);
extern "C" __POSITION* MS_ABI impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(OpenMFC_CPtrList* pThis, void* newElement);
extern "C" void        MS_ABI impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(OpenMFC_CPtrList* pThis, __POSITION* position);
extern "C" void        MS_ABI impl__RemoveAll_CPtrList__QEAAXXZ(OpenMFC_CPtrList* pThis);

// CProperty thunks (core/ole/CProperty.cpp), declared with the signatures their
// mangled names describe.  Retail mfc140u RVAs: ?Set@CProperty@@QEAAHQEAX@Z
// 0x2619d0, ?Get@CProperty@@QEAAPEAXPEAK@Z 0x261c40, ?FreeValue@CProperty@@AEAAXXZ
// 0x2623a0, ?ReadFromStream@CProperty@@QEAAHPEAUIStream@@@Z 0x262020,
// ?WriteToStream@CProperty@@QEAAHPEAUIStream@@@Z 0x261df0.
extern "C" int   MS_ABI impl__Set_CProperty__QEAAHQEAX_Z(void* pThis, void* pValue);
extern "C" void* MS_ABI impl__Get_CProperty__QEAAPEAXPEAK_Z(void* pThis, unsigned long* pcb);
extern "C" void  MS_ABI impl__FreeValue_CProperty__AEAAXXZ(void* pThis);
extern "C" int   MS_ABI impl__ReadFromStream_CProperty__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream);
extern "C" int   MS_ABI impl__WriteToStream_CProperty__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream);

// CString::MakeLower (core/collections/CStringT.cpp); retail
// ?MakeLower@...CStringT...@@QEAAAEAV12@XZ is 0x2da0 (mfc140u).
extern "C" CString* MS_ABI impl__MakeLower___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis);

// AfxThrowOleException / AfxThrowMemoryException (detail/MfcExceptionsSupport.cpp);
// retail 0x25f2c0 / 0x2276c0 (mfc140u).
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

// Thunks of this class that other thunks in this file call (defined below).
extern "C" int  MS_ABI impl__Set_CPropertySection__QEAAHKPEAXK_Z(void* pThis, unsigned long dwPropID, void* pValue, unsigned long dwType);
extern "C" void MS_ABI impl__RemoveAll_CPropertySection__QEAAXXZ(void* pThis);
extern "C" int  MS_ABI impl__SetName_CPropertySection__QEAAHKPEB_W_Z(void* pThis, unsigned long dwPropID, const wchar_t* pszName);
extern "C" int  MS_ABI impl__ReadNameDictFromStream_CPropertySection__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream);
extern "C" int  MS_ABI impl__WriteNameDictToStream_CPropertySection__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream);

// The property search every accessor inlines (e.g. GetProperty, RVA 0x2627c0
// mfc140u):
//   for (n = m_PropList.m_pNodeHead; n; n = n->pNext)
//       if (((CProperty*)n->data)->m_dwPropID == dwPropID) return n;
//   return NULL;
// Retail dereferences n->data without a NULL test; so does this.
PS_Node* ps_find_node(PS_Section* s, DWORD dwPropID) {
    for (PS_Node* n = s->m_PropList.m_pNodeHead; n != nullptr; n = n->pNext) {
        if (PS_Prop(n)->m_dwPropID == dwPropID) return n;
    }
    return nullptr;
}

// --- name dictionary (retail CMapStringToPtr, see the header comment) ---------

// Retail CMapStringToPtr::GetAssocAt (0x2334e0, mfc140u) hashes the key as:
//   nHash = 0x811c9dc5; nLen = wcslen(key); nStep = nLen / 10 + 1;
//   for (i = 0; i < nLen; i += nStep) nHash = (nHash * 0x01000193) ^ key[i];
// (key[i] zero-extended), then bucket = nHash % m_nHashTableSize, and walks the
// bucket comparing nHashValue first and wcscmp(assoc->key, key) == 0 second.
UINT ps_hash_key(const wchar_t* key) {
    UINT nHash = 0x811c9dc5u;
    UINT nLen = static_cast<UINT>(wcslen(key));
    UINT nStep = nLen / 10 + 1;
    for (UINT i = 0; i < nLen; i += nStep) {
        nHash = (nHash * 0x01000193u) ^ static_cast<UINT>(static_cast<unsigned short>(key[i]));
    }
    return nHash;
}

PS_Assoc* ps_dict_get_assoc(PS_NameDict* d, const wchar_t* key, UINT& nHash, UINT& nBucket) {
    nHash = ps_hash_key(key);
    nBucket = nHash % d->m_nHashTableSize;
    if (d->m_pHashTable == nullptr) return nullptr;
    for (PS_Assoc* a = d->m_pHashTable[nBucket]; a != nullptr; a = a->pNext) {
        if (a->nHashValue == nHash && wcscmp(a->key.GetString(), key) == 0) return a;
    }
    return nullptr;
}

// Retail ?Lookup@CMapStringToPtr@@QEBAHPEB_WAEAPEAX@Z (0x2335a0, mfc140u):
// a = GetAssocAt(key); if (!a) return FALSE; rValue = a->value; return TRUE.
BOOL ps_dict_lookup(PS_NameDict* d, const wchar_t* key, void*& rValue) {
    UINT nHash, nBucket;
    PS_Assoc* a = ps_dict_get_assoc(d, key, nHash, nBucket);
    if (a == nullptr) return FALSE;
    rValue = a->value;
    return TRUE;
}

// Retail ??ACMapStringToPtr@@QEAAAEAPEAXPEB_W@Z (0x233600, mfc140u):
// a = GetAssocAt(key); if (!a) { if (!m_pHashTable) InitHashTable(m_nHashTableSize,
// TRUE); a = NewAssoc(); a->nHashValue = nHash; a->key = key; a->pNext =
// m_pHashTable[nBucket]; m_pHashTable[nBucket] = a; } return a->value;
// (NewAssoc zero-fills value and bumps m_nCount.)  Deviation noted in the
// header: the node comes from malloc, not a CPlex block.
void*& ps_dict_index(PS_NameDict* d, const wchar_t* key) {
    UINT nHash, nBucket;
    PS_Assoc* a = ps_dict_get_assoc(d, key, nHash, nBucket);
    if (a == nullptr) {
        if (d->m_pHashTable == nullptr) {
            d->m_pHashTable = static_cast<PS_Assoc**>(std::calloc(d->m_nHashTableSize, sizeof(PS_Assoc*)));
            if (d->m_pHashTable == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        }
        a = static_cast<PS_Assoc*>(std::malloc(sizeof(PS_Assoc)));
        if (a == nullptr) impl__AfxThrowMemoryException__YAXXZ();
        new (&a->key) CString(key);
        a->nHashValue = nHash;
        a->value = nullptr;
        a->pNext = d->m_pHashTable[nBucket];
        d->m_pHashTable[nBucket] = a;
        d->m_nCount++;
    }
    return a->value;
}

// Retail ?RemoveAll@CMapStringToPtr@@QEAAXXZ (0x2332e0, mfc140u): if
// m_pHashTable, destroy every assoc's key bucket by bucket, free(m_pHashTable),
// m_pHashTable = NULL; then m_nCount = 0, m_pFreeList = NULL, free the CPlex
// chain, m_pBlocks = NULL.  Here each assoc is free()d individually.
void ps_dict_remove_all(PS_NameDict* d) {
    if (d->m_pHashTable != nullptr) {
        for (UINT i = 0; i < d->m_nHashTableSize; ++i) {
            PS_Assoc* a = d->m_pHashTable[i];
            while (a != nullptr) {
                PS_Assoc* next = a->pNext;
                a->key.~CString();
                std::free(a);
                a = next;
            }
        }
        std::free(d->m_pHashTable);
        d->m_pHashTable = nullptr;
    }
    d->m_nCount = 0;
    d->m_pFreeList = nullptr;
    d->m_pBlocks = nullptr;
}

// Retail ?GetNextAssoc@CMapStringToPtr@@... (0x233760, mfc140u), with POSITION
// being a CAssoc* and BEFORE_START_POSITION == (POSITION)-1:
//   a = pos; if (!a) return;
//   if (a == BEFORE_START_POSITION && m_nHashTableSize) { a = first non-NULL
//       bucket head; none -> AfxThrowInvalidArgException }
//   next = a->pNext; if (!next) for (b = a->nHashValue % size + 1; b < size; b++)
//       if (m_pHashTable[b]) { next = m_pHashTable[b]; break; }
//   pos = next; rKey = a->key; rValue = a->value;
// Only ever called here after the caller checked m_nCount != 0, so the
// no-bucket throw cannot fire; it is reported by returning FALSE instead.
BOOL ps_dict_get_next_assoc(PS_NameDict* d, PS_Assoc*& pos, CString& rKey, void*& rValue) {
    PS_Assoc* a = pos;
    if (a == nullptr) return FALSE;
    if (a == reinterpret_cast<PS_Assoc*>(-1)) {
        a = nullptr;
        if (d->m_pHashTable != nullptr) {
            for (UINT b = 0; b < d->m_nHashTableSize; ++b) {
                if (d->m_pHashTable[b] != nullptr) { a = d->m_pHashTable[b]; break; }
            }
        }
        if (a == nullptr) return FALSE;
    }
    PS_Assoc* next = a->pNext;
    if (next == nullptr) {
        for (UINT b = a->nHashValue % d->m_nHashTableSize + 1; b < d->m_nHashTableSize; ++b) {
            if (d->m_pHashTable[b] != nullptr) { next = d->m_pHashTable[b]; break; }
        }
    }
    pos = next;
    rKey = a->key;
    rValue = a->value;
    return TRUE;
}

// Retail's file-static dictionary-entry writer at 0x262e70 (mfc140u), called
// from WriteNameDictToStream as (pIStream, dwPropID, &CString):
//   entry.dwPropID = dwPropID;
//   cbName = min(str.GetLength() + 1, 255);          // `mov $0xff,%eax; cmovl`
//   entry.cbName = cbName;
//   WideCharToMultiByte(CP_THREAD_ACP /* 3 */, 0, str, -1, entry.szName, 256, NULL, NULL);
//   hr = pIStream->Write(&entry, cbName + 8, &cb);   // vtable slot 4
//   if (FAILED(hr)) return FALSE;
//   return cb == cbName + 8;
// This is the [MS-OLEPS] 2.16 DictionaryEntry: PropertyIdentifier, Length,
// Name (Length bytes).  Note the byte count is derived from the UTF-16 length,
// not from the converted length -- transcribed as-is.  Deviation: the 256-byte
// name buffer is zero-filled first (retail writes whatever the stack held past
// the converted string).
BOOL ps_write_dict_entry(IStream* pIStream, DWORD dwPropID, const CString& str) {
    struct {
        DWORD dwPropID;
        DWORD cbName;
        char  szName[256];
    } entry;
    std::memset(entry.szName, 0, sizeof(entry.szName));
    entry.dwPropID = dwPropID;
    int cbName = str.GetLength() + 1;
    if (cbName > 255) cbName = 255;
    entry.cbName = static_cast<DWORD>(cbName);
    ::WideCharToMultiByte(CP_THREAD_ACP, 0, str.GetString(), -1, entry.szName, 256, nullptr, nullptr);
    ULONG cbToWrite = static_cast<ULONG>(cbName) + 8;
    ULONG cb = 0;
    HRESULT hr = pIStream->Write(&entry, cbToWrite, &cb);
    if (FAILED(hr)) return FALSE;
    return cb == cbToWrite ? TRUE : FALSE;
}

} // namespace

// Symbol: ??0CPropertySection@@QEAA@U_GUID@@@Z
extern "C" void* MS_ABI impl___0CPropertySection__QEAA_U_GUID___Z(void* pThis, const void* pFormatID) {
    // Retail RVA 0x262490 (mfc140u).  Identical to the default constructor
    // below except that m_FormatID is copied from the by-reference CLSID
    // argument (`movups (%rbx),%xmm0; movdqu %xmm0,(%rdi)`) instead of from
    // GUID_NULL.  Member order of construction as in retail: m_PropList,
    // m_NameDict, m_strSectionName, then m_FormatID and m_SH.
    PS_Section* s = PS(pThis);
    impl___0CPtrList__QEAA__J_Z(&s->m_PropList, 10);
    s->m_NameDict.vfptr = nullptr;
    s->m_NameDict.m_pHashTable = nullptr;
    s->m_NameDict.m_nHashTableSize = 17;
    s->m_NameDict.m_nCount = 0;
    s->m_NameDict.m_pFreeList = nullptr;
    s->m_NameDict.m_pBlocks = nullptr;
    s->m_NameDict.m_nBlockSize = 10;
    new (&s->m_strSectionName) CString();
    std::memcpy(&s->m_FormatID, pFormatID, sizeof(CLSID));
    s->m_SH.cbSection = 0;
    s->m_SH.cProperties = 0;
    return pThis;
}
// Symbol: ??0CPropertySection@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPropertySection__QEAA_XZ(void* pThis) {
    // Retail RVA 0x2623d0 (mfc140u): CPtrList::CPtrList(10) inlined into
    // m_PropList, CMapStringToPtr::CMapStringToPtr(10) inlined into m_NameDict
    // (hash table NULL, 17 buckets, count 0, block size 10), m_strSectionName
    // set to the string manager's nil string, m_FormatID = the 16 zero bytes at
    // 0x2d98d8 (GUID_NULL), m_SH = {0, 0}.
    PS_Section* s = PS(pThis);
    impl___0CPtrList__QEAA__J_Z(&s->m_PropList, 10);
    s->m_NameDict.vfptr = nullptr;
    s->m_NameDict.m_pHashTable = nullptr;
    s->m_NameDict.m_nHashTableSize = 17;
    s->m_NameDict.m_nCount = 0;
    s->m_NameDict.m_pFreeList = nullptr;
    s->m_NameDict.m_pBlocks = nullptr;
    s->m_NameDict.m_nBlockSize = 10;
    new (&s->m_strSectionName) CString();
    std::memset(&s->m_FormatID, 0, sizeof(CLSID));
    s->m_SH.cbSection = 0;
    s->m_SH.cProperties = 0;
    return pThis;
}
// Symbol: ??1CPropertySection@@QEAA@XZ
extern "C" void MS_ABI impl___1CPropertySection__QEAA_XZ(void* pThis) {
    // Retail RVA 0x262560 (mfc140u): RemoveAll() (0x262770); inlined
    // ~CString on m_strSectionName; inlined ~CMapStringToPtr on m_NameDict
    // (vftable store, then CMapStringToPtr::RemoveAll 0x2332e0); inlined
    // ~CPtrList on m_PropList (vftable store, then tail-jump to
    // CPtrList::RemoveAll 0x8350).
    PS_Section* s = PS(pThis);
    impl__RemoveAll_CPropertySection__QEAAXXZ(pThis);
    s->m_strSectionName.~CString();
    ps_dict_remove_all(&s->m_NameDict);
    impl___1CPtrList__UEAA_XZ(&s->m_PropList);
}

// Symbol: ?AddProperty@CPropertySection@@QEAAXPEAVCProperty@@@Z
extern "C" void MS_ABI impl__AddProperty_CPropertySection__QEAAXPEAVCProperty___Z(void* pThis, void* pProp) {
    // Retail RVA 0x2627e0 (mfc140u): m_PropList.AddTail(pProp) (0x231e70);
    // ++m_SH.cProperties.
    PS_Section* s = PS(pThis);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&s->m_PropList, pProp);
    s->m_SH.cProperties++;
}

// Symbol: ?Get@CPropertySection@@QEAAPEAXK@Z
extern "C" void* MS_ABI impl__Get_CPropertySection__QEAAPEAXK_Z(void* pThis, unsigned long dwPropID) {
    // Retail RVA 0x2626d0 (mfc140u): n = ps_find_node(dwPropID); if (!n)
    // return NULL; tail-jump CProperty::Get(pcb = NULL) (0x261c40) on n->data.
    PS_Node* n = ps_find_node(PS(pThis), dwPropID);
    if (n == nullptr) return nullptr;
    return impl__Get_CProperty__QEAAPEAXPEAK_Z(n->data, nullptr);
}

// Symbol: ?Get@CPropertySection@@QEAAPEAXKPEAK@Z
extern "C" void* MS_ABI impl__Get_CPropertySection__QEAAPEAXKPEAK_Z(void* pThis, unsigned long dwPropID, unsigned long* pcb) {
    // Retail RVA 0x2626f0 (mfc140u): n = ps_find_node(dwPropID); if (!n)
    // return NULL; tail-jump CProperty::Get(pcb) (0x261c40) on n->data.
    PS_Node* n = ps_find_node(PS(pThis), dwPropID);
    if (n == nullptr) return nullptr;
    return impl__Get_CProperty__QEAAPEAXPEAK_Z(n->data, pcb);
}

// Symbol: ?GetCount@CPropertySection@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetCount_CPropertySection__QEAAKXZ(void* pThis) {
    // Retail RVA 0x262800 (mfc140u): return (DWORD)m_PropList.m_nCount (+0x30).
    return static_cast<unsigned long>(PS(pThis)->m_PropList.m_nCount);
}

// Symbol: ?GetFormatID@CPropertySection@@QEAA?AU_GUID@@XZ
extern "C" void* MS_ABI impl__GetFormatID_CPropertySection__QEAA_AU_GUID__XZ(void* pThis, void* pRetGuid) {
    // Retail RVA 0x2625d0 (mfc140u): the 16-byte CLSID is returned through the
    // hidden buffer in rdx: `movups (%rcx),%xmm0; movdqu %xmm0,(%rdx)`; rax = rdx.
    std::memcpy(pRetGuid, &PS(pThis)->m_FormatID, sizeof(CLSID));
    return pRetGuid;
}

// Symbol: ?GetID@CPropertySection@@QEAAHPEB_WPEAK@Z
extern "C" int MS_ABI impl__GetID_CPropertySection__QEAAHPEB_WPEAK_Z(void* pThis, const wchar_t* pszName, unsigned long* pdwPropID) {
    // Retail RVA 0x262bb0 (mfc140u):
    //   CString strName(pszName);          (0xdcb0)
    //   strName.MakeLower();               (0x2da0)
    //   void* pv;
    //   if (m_NameDict.Lookup(strName, pv)) { *pdwPropID = (DWORD)pv; return TRUE; }   (0x2335a0)
    //   return FALSE;
    CString strName(pszName);
    impl__MakeLower___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&strName);
    void* pv = nullptr;
    if (ps_dict_lookup(&PS(pThis)->m_NameDict, strName.GetString(), pv)) {
        *pdwPropID = static_cast<unsigned long>(reinterpret_cast<ULONG_PTR>(pv));
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?GetList@CPropertySection@@QEAAPEAVCPtrList@@XZ
extern "C" void* MS_ABI impl__GetList_CPropertySection__QEAAPEAVCPtrList__XZ(void* pThis) {
    // Retail RVA 0x242b80 (mfc140u): return &m_PropList (this + 0x18).
    return &PS(pThis)->m_PropList;
}

// Symbol: ?GetProperty@CPropertySection@@QEAAPEAVCProperty@@K@Z
extern "C" void* MS_ABI impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(void* pThis, unsigned long dwPropID) {
    // Retail RVA 0x2627c0 (mfc140u): n = ps_find_node(dwPropID); return n ? n->data : NULL.
    PS_Node* n = ps_find_node(PS(pThis), dwPropID);
    return n != nullptr ? n->data : nullptr;
}

// Symbol: ?GetSectionName@CPropertySection@@QEAAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetSectionName_CPropertySection__QEAAPEB_WXZ(void* pThis) {
    // Retail RVA 0x263090 (mfc140u): return m_strSectionName.m_pszData (+0x88).
    return PS(pThis)->m_strSectionName.GetString();
}

// Symbol: ?GetSize@CPropertySection@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetSize_CPropertySection__QEAAKXZ(void* pThis) {
    // Retail RVA 0x9770 (mfc140u): return m_SH.cbSection (+0x10).
    return PS(pThis)->m_SH.cbSection;
}

// Symbol: ?ReadFromStream@CPropertySection@@QEAAHPEAUIStream@@T_LARGE_INTEGER@@@Z
extern "C" int MS_ABI impl__ReadFromStream_CPropertySection__QEAAHPEAUIStream__T_LARGE_INTEGER___Z(void* pThis, IStream* pIStream, long long liPropSet) {
    // Retail RVA 0x262a00 (mfc140u).  The LARGE_INTEGER is an 8-byte aggregate
    // passed by value in r8; it is modelled as long long.
    //   if (m_SH.cProperties != 0 || m_PropList.m_nCount != 0) RemoveAll();
    //   pIStream->Seek(0, STREAM_SEEK_CUR, &uliStart);                 (slot 5, result ignored)
    //   hr = pIStream->Read(&m_SH, 8, &cb);                             (slot 3)
    //   if (FAILED(hr) || cb != 8) return FALSE;
    //   hr = pIStream->Clone(&pIStrPIDO);                               (slot 13)
    //   if (FAILED(hr)) return FALSE;
    //   for (cProps = m_SH.cProperties; cProps != 0; --cProps) {
    //       hr = pIStrPIDO->Read(&pido, 8, &cb);
    //       if (FAILED(hr) || cb != 8) { pIStrPIDO->Release(); return FALSE; }
    //       li = uliStart + (ULONGLONG)pido.dwOffset;                   (mov ebx; add 64-bit)
    //       pIStream->Seek(liPropSet, STREAM_SEEK_SET, NULL);
    //       pIStream->Seek(li, STREAM_SEEK_CUR, NULL);
    //       if (pido.dwPropID != 0) {
    //           p = operator new(0x10);                                  (0x27f0)
    //           if (p) { p->m_pValue = NULL; p->m_dwType = 0; p->m_dwPropID = pido.dwPropID; }
    //           p->ReadFromStream(pIStream);                            (0x262020, result ignored)
    //           m_PropList.AddTail(p);                                  (0x231e70)
    //       } else {
    //           ReadNameDictFromStream(pIStream);                       (0x262cd0, result ignored)
    //       }
    //   }
    //   pIStrPIDO->Release();
    //   return TRUE;
    // The two seeks land on liPropSet + uliStart + dwOffset, i.e. on the
    // [MS-OLEPS] 2.19 PropertyIdentifierAndOffset target only when liPropSet
    // is 0 (property set at the stream origin); transcribed as retail has it.
    // The m_SH.cProperties count is what the stream said; the AddTail here
    // deliberately does not bump it (unlike AddProperty).  Retail calls
    // CProperty::ReadFromStream even when operator new returned NULL.  Retail's
    // ??2@YAPEAX_K@Z (0x27f0, mfc140u) returns NULL only when the module
    // thread state's new handler (+0x50, installed by AfxSetNewHandler) is
    // NULL or returns 0; the default handler at 0x8a8b0 calls
    // AfxThrowMemoryException, so the NULL path is unreachable by default.
    // Here a NULL allocation is skipped instead of dereferenced.  `cb` is an
    // uninitialised stack slot (0x30(%rbp)) in retail; it starts at 0 here.
    PS_Section* s = PS(pThis);
    if (s->m_SH.cProperties != 0 || s->m_PropList.m_nCount != 0) {
        impl__RemoveAll_CPropertySection__QEAAXXZ(pThis);
    }
    LARGE_INTEGER liZero;
    liZero.QuadPart = 0;
    ULARGE_INTEGER uliStart;
    uliStart.QuadPart = 0;
    pIStream->Seek(liZero, STREAM_SEEK_CUR, &uliStart);
    ULONG cb = 0;
    HRESULT hr = pIStream->Read(&s->m_SH, sizeof(s->m_SH), &cb);
    if (FAILED(hr) || cb != sizeof(s->m_SH)) return FALSE;
    IStream* pIStrPIDO = nullptr;
    hr = pIStream->Clone(&pIStrPIDO);
    if (FAILED(hr)) return FALSE;
    for (DWORD cProps = s->m_SH.cProperties; cProps != 0; --cProps) {
        PS_PropertyIdOffset pido;
        hr = pIStrPIDO->Read(&pido, sizeof(pido), &cb);
        if (FAILED(hr) || cb != sizeof(pido)) {
            pIStrPIDO->Release();
            return FALSE;
        }
        LARGE_INTEGER li;
        li.QuadPart = static_cast<LONGLONG>(uliStart.QuadPart + pido.dwOffset);
        LARGE_INTEGER liSet;
        liSet.QuadPart = liPropSet;
        pIStream->Seek(liSet, STREAM_SEEK_SET, nullptr);
        pIStream->Seek(li, STREAM_SEEK_CUR, nullptr);
        if (pido.dwPropID != 0) {
            PS_Property* p = static_cast<PS_Property*>(impl___2_YAPEAX_K_Z(sizeof(PS_Property)));
            if (p != nullptr) {
                p->m_pValue = nullptr;
                p->m_dwType = 0;
                p->m_dwPropID = pido.dwPropID;
                impl__ReadFromStream_CProperty__QEAAHPEAUIStream___Z(p, pIStream);
                impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&s->m_PropList, p);
            }
        } else {
            impl__ReadNameDictFromStream_CPropertySection__QEAAHPEAUIStream___Z(pThis, pIStream);
        }
    }
    pIStrPIDO->Release();
    return TRUE;
}

// Symbol: ?ReadNameDictFromStream@CPropertySection@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__ReadNameDictFromStream_CPropertySection__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream) {
    // Retail RVA 0x262cd0 (mfc140u) -- reads an [MS-OLEPS] 2.17 Dictionary:
    //   hr = pIStream->Read(&cEntries, 4, &cb); if (FAILED(hr) || cb != 4) return FALSE;
    //   for (i = 0; i < cEntries; i++) {                                (unsigned compare)
    //       hr = pIStream->Read(&hdr /* {dwPropID, cbName} */, 8, &cb);
    //       if (FAILED(hr) || cb != 8) return FALSE;
    //       hr = pIStream->Read(szName, hdr.cbName, &cb);               (fixed stack buffer, 0x108 bytes below wszName)
    //       if (FAILED(hr) || cb != hdr.cbName) return FALSE;
    //       n = MultiByteToWideChar(CP_THREAD_ACP /* 3 */, 0, szName, -1, wszName, 256);
    //       if (n > 256) throw (E_FAIL -> AfxThrowOleException);        (0x333c helper)
    //       if (n > 0) wszName[n - 1] = 0;
    //       if (i == 0 && hdr.dwPropID == 0) m_strSectionName.SetString(wszName, wcslen(wszName));   (0x2e30)
    //       else SetName(hdr.dwPropID, wszName);                        (0x262c30, result ignored)
    //   }
    //   return TRUE;
    // Deviations: retail reads cbName bytes into its fixed stack buffer with no
    // bound (the declared size is not recoverable from the image; in the retail
    // frame szName sits 0x108 bytes below wszName) and relies on the stream
    // supplying the NUL that the [MS-OLEPS] 2.16 DictionaryEntry Name field
    // carries; here cbName > 256 fails the call, the buffer is always
    // NUL-terminated after the read, and wszName is zero-filled so a failed
    // conversion yields an empty name instead of stack garbage.
    PS_Section* s = PS(pThis);
    DWORD cEntries = 0;
    ULONG cb = 0;
    HRESULT hr = pIStream->Read(&cEntries, sizeof(cEntries), &cb);
    if (FAILED(hr) || cb != sizeof(cEntries)) return FALSE;
    for (DWORD i = 0; i < cEntries; ++i) {
        struct {
            DWORD dwPropID;
            DWORD cbName;
        } hdr;
        hr = pIStream->Read(&hdr, sizeof(hdr), &cb);
        if (FAILED(hr) || cb != sizeof(hdr)) return FALSE;
        char szName[257];
        if (hdr.cbName > 256) return FALSE;
        hr = pIStream->Read(szName, hdr.cbName, &cb);
        if (FAILED(hr) || cb != hdr.cbName) return FALSE;
        szName[hdr.cbName] = '\0';
        wchar_t wszName[256];
        std::memset(wszName, 0, sizeof(wszName));
        int n = ::MultiByteToWideChar(CP_THREAD_ACP, 0, szName, -1, wszName, 256);
        if (n > 256) impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
        if (n > 0) wszName[n - 1] = L'\0';
        if (i == 0 && hdr.dwPropID == 0) {
            s->m_strSectionName = wszName;
        } else {
            impl__SetName_CPropertySection__QEAAHKPEB_W_Z(pThis, hdr.dwPropID, wszName);
        }
    }
    return TRUE;
}

// Symbol: ?Remove@CPropertySection@@QEAAXK@Z
extern "C" void MS_ABI impl__Remove_CPropertySection__QEAAXK_Z(void* pThis, unsigned long dwPropID) {
    // Retail RVA 0x262710 (mfc140u): n = ps_find_node(dwPropID); if (!n) return;
    // m_PropList.RemoveAt(n) (0x2320d0); inlined `delete pProp`: CProperty::
    // FreeValue (0x2623a0) then operator delete (0x2b77b0 -> 0x27c0);
    // --m_SH.cProperties.
    PS_Section* s = PS(pThis);
    PS_Node* n = ps_find_node(s, dwPropID);
    if (n == nullptr) return;
    void* pProp = n->data;
    impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(&s->m_PropList, reinterpret_cast<__POSITION*>(n));
    impl__FreeValue_CProperty__AEAAXXZ(pProp);
    impl___3_YAXPEAX_Z(pProp);
    s->m_SH.cProperties--;
}

// Symbol: ?RemoveAll@CPropertySection@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CPropertySection__QEAAXXZ(void* pThis) {
    // Retail RVA 0x262770 (mfc140u):
    //   for (n = m_PropList.m_pNodeHead; n; n = next) {
    //       p = n->data; next = n->pNext;
    //       if (p) { p->FreeValue(); operator delete(p); }
    //   }
    //   m_PropList.RemoveAll();                                          (0x8350)
    //   m_SH.cProperties = 0;
    PS_Section* s = PS(pThis);
    for (PS_Node* n = s->m_PropList.m_pNodeHead; n != nullptr;) {
        void* p = n->data;
        n = n->pNext;
        if (p != nullptr) {
            impl__FreeValue_CProperty__AEAAXXZ(p);
            impl___3_YAXPEAX_Z(p);
        }
    }
    impl__RemoveAll_CPtrList__QEAAXXZ(&s->m_PropList);
    s->m_SH.cProperties = 0;
}

// Symbol: ?Set@CPropertySection@@QEAAHKPEAX@Z
extern "C" int MS_ABI impl__Set_CPropertySection__QEAAHKPEAX_Z(void* pThis, unsigned long dwPropID, void* pValue) {
    // Retail RVA 0x262690 (mfc140u): n = ps_find_node(dwPropID); if (!n) return
    // FALSE; p = n->data; if (p->m_dwType == 0) return FALSE; p->m_dwPropID =
    // dwPropID; p->Set(pValue) (0x2619d0, result ignored); return TRUE.
    PS_Node* n = ps_find_node(PS(pThis), dwPropID);
    if (n == nullptr) return FALSE;
    PS_Property* p = PS_Prop(n);
    if (p->m_dwType == 0) return FALSE;
    p->m_dwPropID = dwPropID;
    impl__Set_CProperty__QEAAHQEAX_Z(p, pValue);
    return TRUE;
}

// Symbol: ?Set@CPropertySection@@QEAAHKPEAXK@Z
extern "C" int MS_ABI impl__Set_CPropertySection__QEAAHKPEAXK_Z(void* pThis, unsigned long dwPropID, void* pValue, unsigned long dwType) {
    // Retail RVA 0x2625f0 (mfc140u):
    //   n = ps_find_node(dwPropID);
    //   if (n) { p = n->data; p->m_dwType = dwType; p->m_dwPropID = dwPropID;
    //            p->Set(pValue) (0x2619d0, result ignored); return TRUE; }
    //   p = operator new(0x10);                                           (0x27f0)
    //   if (p) { p->m_pValue = NULL; p->m_dwType = dwType; p->m_dwPropID = dwPropID;
    //            p->Set(pValue); }          -- the inlined CProperty(id, value, type)
    //   if (p) { m_PropList.AddTail(p) (0x231e70); ++m_SH.cProperties; }
    //   return p != NULL;
    PS_Section* s = PS(pThis);
    PS_Node* n = ps_find_node(s, dwPropID);
    if (n != nullptr) {
        PS_Property* p = PS_Prop(n);
        p->m_dwType = dwType;
        p->m_dwPropID = dwPropID;
        impl__Set_CProperty__QEAAHQEAX_Z(p, pValue);
        return TRUE;
    }
    PS_Property* p = static_cast<PS_Property*>(impl___2_YAPEAX_K_Z(sizeof(PS_Property)));
    if (p != nullptr) {
        p->m_pValue = nullptr;
        p->m_dwType = dwType;
        p->m_dwPropID = dwPropID;
        impl__Set_CProperty__QEAAHQEAX_Z(p, pValue);
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&s->m_PropList, p);
        s->m_SH.cProperties++;
    }
    return p != nullptr ? TRUE : FALSE;
}

// Symbol: ?SetFormatID@CPropertySection@@QEAAXU_GUID@@@Z
extern "C" void MS_ABI impl__SetFormatID_CPropertySection__QEAAXU_GUID___Z(void* pThis, const void* pFormatID) {
    // Retail RVA 0x2625e0 (mfc140u): the by-value CLSID arrives by reference in
    // rdx; `movups (%rdx),%xmm0; movdqu %xmm0,(%rcx)` copies it into m_FormatID.
    std::memcpy(&PS(pThis)->m_FormatID, pFormatID, sizeof(CLSID));
}

// Symbol: ?SetName@CPropertySection@@QEAAHKPEB_W@Z
extern "C" int MS_ABI impl__SetName_CPropertySection__QEAAHKPEB_W_Z(void* pThis, unsigned long dwPropID, const wchar_t* pszName) {
    // Retail RVA 0x262c30 (mfc140u):
    //   BOOL bResult = TRUE;
    //   CString strName(pszName); strName.MakeLower();                  (0xdcb0, 0x2da0)
    //   void* pv;
    //   if (m_NameDict.Lookup(strName, pv)) bResult = FALSE;           (0x2335a0: name already mapped)
    //   else m_NameDict[strName] = (void*)(ULONG_PTR)dwPropID;          (0x233600: `mov %rsi,(%rax)`, rsi = zero-extended edx)
    //   return bResult;
    PS_Section* s = PS(pThis);
    int bResult = TRUE;
    CString strName(pszName);
    impl__MakeLower___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&strName);
    void* pv = nullptr;
    if (ps_dict_lookup(&s->m_NameDict, strName.GetString(), pv)) {
        bResult = FALSE;
    } else {
        ps_dict_index(&s->m_NameDict, strName.GetString()) =
            reinterpret_cast<void*>(static_cast<ULONG_PTR>(dwPropID));
    }
    return bResult;
}

// Symbol: ?SetSectionName@CPropertySection@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SetSectionName_CPropertySection__QEAAHPEB_W_Z(void* pThis, const wchar_t* pszName) {
    // Retail RVA 0x263040 (mfc140u): len = pszName ? wcslen(pszName) : 0;
    // m_strSectionName.SetString(pszName, len) (0x2e30: len == 0 -> Empty());
    // return TRUE.  OpenMFC CString::operator=(const wchar_t*) has exactly
    // that NULL/empty -> Empty() behaviour.
    PS(pThis)->m_strSectionName = pszName;
    return TRUE;
}

// Symbol: ?WriteNameDictToStream@CPropertySection@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__WriteNameDictToStream_CPropertySection__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream) {
    // Retail RVA 0x262f40 (mfc140u) -- writes an [MS-OLEPS] 2.17 Dictionary:
    //   BOOL bResult = TRUE;
    //   cEntries = (DWORD)m_NameDict.m_nCount + 1;                     (+1: the section-name entry)
    //   pIStream->Write(&cEntries, 4, &cb); if (cb != 4) return FALSE;   (HRESULT ignored)
    //   CString strName;
    //   if (!WriteEntry(pIStream, 0, &m_strSectionName)) bResult = FALSE;   (0x262e70; PID 0 = section name)
    //   else if (m_NameDict.m_nCount != 0) {
    //       pos = BEFORE_START_POSITION;                                (inlined GetStartPosition)
    //       do {
    //           m_NameDict.GetNextAssoc(pos, strName, (void*&)pv);      (0x233760)
    //           if (!WriteEntry(pIStream, (DWORD)pv, &strName)) { bResult = FALSE; break; }
    //       } while (pos != NULL);
    //   }
    //   ~strName; return bResult;
    // `cb` (0x30(%rbp)) is never initialised in retail before the first Write;
    // it starts at 0 here, so a Write that fails without storing it cannot
    // pass the `cb == 4` test.
    PS_Section* s = PS(pThis);
    int bResult = TRUE;
    DWORD cEntries = static_cast<DWORD>(s->m_NameDict.m_nCount) + 1;
    ULONG cb = 0;
    pIStream->Write(&cEntries, sizeof(cEntries), &cb);
    if (cb != sizeof(cEntries)) return FALSE;
    CString strName;
    if (!ps_write_dict_entry(pIStream, 0, s->m_strSectionName)) {
        bResult = FALSE;
    } else if (s->m_NameDict.m_nCount != 0) {
        PS_Assoc* pos = reinterpret_cast<PS_Assoc*>(-1);
        do {
            void* pv = nullptr;
            if (!ps_dict_get_next_assoc(&s->m_NameDict, pos, strName, pv)) {
                bResult = FALSE;
                break;
            }
            DWORD dwPropID = static_cast<DWORD>(reinterpret_cast<ULONG_PTR>(pv));
            if (!ps_write_dict_entry(pIStream, dwPropID, strName)) {
                bResult = FALSE;
                break;
            }
        } while (pos != nullptr);
    }
    return bResult;
}

// Symbol: ?WriteToStream@CPropertySection@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__WriteToStream_CPropertySection__QEAAHPEAUIStream___Z(void* pThis, IStream* pIStream) {
    // Retail RVA 0x262810 (mfc140u) -- writes an [MS-OLEPS] 2.20 PropertySet:
    //   Set(0, NULL, 0);                                (0x2625f0: make sure a PID_DICTIONARY
    //                                                    (0) placeholder property is in the list)
    //   m_SH.cProperties = (DWORD)m_PropList.m_nCount;
    //   m_SH.cbSection = 0;
    //   pIStream->Seek(0, STREAM_SEEK_CUR, &uliStart);
    //   pIStream->Write(&m_SH, 8, &cb); if (cb != 8) return FALSE;      (HRESULT ignored)
    //   if (m_PropList.m_nCount == 0) return TRUE;
    //   hr = pIStream->Clone(&pIStrPIDO); if (FAILED(hr)) return FALSE;
    //   pIStream->Seek((ULONGLONG)m_SH.cProperties << 3, STREAM_SEEK_CUR, &uliCur);   (skip the PIDO table)
    //   for (n = head; n; n = next) {
    //       next = n->pNext; p = n->data;
    //       ok = (p->m_dwPropID != 0) ? p->WriteToStream(pIStream)      (0x261df0)
    //                                 : WriteNameDictToStream(pIStream); (0x262f40)
    //       if (!ok) { pIStrPIDO->Release(); return FALSE; }
    //       pido.dwOffset = uliCur.LowPart - uliStart.LowPart;           (32-bit subtract)
    //       pido.dwPropID = p->m_dwPropID;
    //       pIStrPIDO->Write(&pido, 8, &cb);                             (HRESULT ignored)
    //       if (cb != 8) { pIStrPIDO->Release(); return FALSE; }
    //       pIStream->Seek(0, STREAM_SEEK_CUR, &uliCur);
    //   }
    //   pIStrPIDO->Release();
    //   m_SH.cbSection = uliCur.LowPart - uliStart.LowPart;
    //   pIStream->Seek(uliStart, STREAM_SEEK_SET, NULL);
    //   pIStream->Write(&m_SH, 8, &cb); if (cb != 8) return FALSE;
    //   pIStream->Seek(uliCur, STREAM_SEEK_SET, NULL);
    //   return TRUE;
    // `cb` is an uninitialised stack slot in retail; it starts at 0 here so a
    // Write that fails without storing it cannot pass the `cb == 8` tests.
    PS_Section* s = PS(pThis);
    impl__Set_CPropertySection__QEAAHKPEAXK_Z(pThis, 0, nullptr, 0);
    s->m_SH.cProperties = static_cast<DWORD>(s->m_PropList.m_nCount);
    s->m_SH.cbSection = 0;
    LARGE_INTEGER liZero;
    liZero.QuadPart = 0;
    ULARGE_INTEGER uliStart;
    uliStart.QuadPart = 0;
    pIStream->Seek(liZero, STREAM_SEEK_CUR, &uliStart);
    ULONG cb = 0;
    pIStream->Write(&s->m_SH, sizeof(s->m_SH), &cb);
    if (cb != sizeof(s->m_SH)) return FALSE;
    if (s->m_PropList.m_nCount == 0) return TRUE;
    IStream* pIStrPIDO = nullptr;
    HRESULT hr = pIStream->Clone(&pIStrPIDO);
    if (FAILED(hr)) return FALSE;
    LARGE_INTEGER liSkip;
    liSkip.QuadPart = static_cast<LONGLONG>(static_cast<ULONGLONG>(s->m_SH.cProperties) << 3);
    ULARGE_INTEGER uliCur;
    uliCur.QuadPart = 0;
    pIStream->Seek(liSkip, STREAM_SEEK_CUR, &uliCur);
    for (PS_Node* n = s->m_PropList.m_pNodeHead; n != nullptr;) {
        PS_Node* next = n->pNext;
        PS_Property* p = PS_Prop(n);
        int ok;
        if (p->m_dwPropID != 0) {
            ok = impl__WriteToStream_CProperty__QEAAHPEAUIStream___Z(p, pIStream);
        } else {
            ok = impl__WriteNameDictToStream_CPropertySection__QEAAHPEAUIStream___Z(pThis, pIStream);
        }
        if (!ok) {
            pIStrPIDO->Release();
            return FALSE;
        }
        PS_PropertyIdOffset pido;
        pido.dwOffset = uliCur.LowPart - uliStart.LowPart;
        pido.dwPropID = p->m_dwPropID;
        pIStrPIDO->Write(&pido, sizeof(pido), &cb);
        if (cb != sizeof(pido)) {
            pIStrPIDO->Release();
            return FALSE;
        }
        pIStream->Seek(liZero, STREAM_SEEK_CUR, &uliCur);
        n = next;
    }
    pIStrPIDO->Release();
    s->m_SH.cbSection = uliCur.LowPart - uliStart.LowPart;
    LARGE_INTEGER liStart;
    liStart.QuadPart = static_cast<LONGLONG>(uliStart.QuadPart);
    pIStream->Seek(liStart, STREAM_SEEK_SET, nullptr);
    pIStream->Write(&s->m_SH, sizeof(s->m_SH), &cb);
    if (cb != sizeof(s->m_SH)) return FALSE;
    LARGE_INTEGER liCur;
    liCur.QuadPart = static_cast<LONGLONG>(uliCur.QuadPart);
    pIStream->Seek(liCur, STREAM_SEEK_SET, nullptr);
    return TRUE;
}
