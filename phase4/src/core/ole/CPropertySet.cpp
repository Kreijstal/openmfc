// CPropertySet — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// ===========================================================================
// CPropertySet -- MFC's internal OLE property-set container (src/mfc/oleprop.h
// in the MFC sources, which are NOT on this host).  It holds the stream-level
// header of an [MS-OLEPS] PropertySetStream and the list of CPropertySection
// objects that make up the stream's property sets.
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the RVA it was read from.  All addresses in this file are mfc140u.
// Ten of the 26 exports are not in mfc140u_rva_symbols.json; their RVAs were
// resolved through the ordinal map + export address table (ures.py) and are
// cited the same way.
//
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives.  It was read out of the two retail constructors
// (0x2630a0 default, 0x2630f0 CLSID) and the destructor (0x263140):
//
//   +0x00  FORMATHEADER m_FH        28 bytes -- [MS-OLEPS] 2.20 PropertySetStream
//          +0x00 WORD  wByteOrder    ctor stores 0xFFFE
//          +0x02 WORD  wFormat       ctor stores 0 (the 8-byte store at 0x1802630cc
//          +0x04 DWORD dwOSVer       covers wByteOrder/wFormat/dwOSVer at once)
//          +0x08 CLSID clsID         default ctor copies GUID_NULL (16 zero bytes
//                                    at 0x2d98d8), CLSID ctor copies its argument
//          +0x18 DWORD cSections     ctor stores 0
//   +0x20  CPtrList m_SectionList   0x38 bytes; vptr 0x18032a758 is the CPtrList
//                                    vftable (slot 0 = ?GetRuntimeClass@CPtrList@@,
//                                    read with uvt.py), block size 10 at +0x50
//   sizeof == 0x58 (the highest ctor store is the 8-byte m_nBlockSize at +0x50).
//
// The element type is confirmed by the export name of GetList
// (?GetList@CPropertySet@@QEAAPEAVCPtrList@@XZ) and by the ICF-folded list
// methods the bodies call: 0x231e70 is both ?AddTail@CPtrList@@ and
// ?AddTail@CObList@@, 0x2320d0 both RemoveAt, 0x8350 both RemoveAll.
//
// OpenMFC's CPtrList thunks (core/collections/CPtrList.cpp, backed by
// OpenMFC_CPtrList in detail/CPtrListSupport.h) keep the REAL MFC node layout
// in the object -- head at +0x08, tail +0x10, m_nCount +0x18, nodes
// {pNext, pPrev, data} -- so the inline node walks retail performs on
// m_SectionList are transcribed directly against the object below, exactly as
// the disassembly reads them.  (This is unlike CObList, whose OpenMFC wrapper
// keeps its contents in a side table.)
//
// CPropertySection (core/ole/CPropertySection.cpp, owned by another agent) is
// reached only through its impl__ thunks, declared below with the signatures
// the mangled names describe, plus two facts read from its own retail ctors
// (0x2623d0 / 0x262490) and used by the bodies in this file:
//   * sizeof(CPropertySection) == 0x90 -- the operator new size in AddSection
//     (0x263460), Set (0x263170) and ReadFromStream (0x263750); the retail
//     CLSID ctor's highest store is the CString at +0x88.
//   * m_FormatID is the CLSID at +0x00 -- the CLSID ctor copies its argument
//     there (movdqu %xmm0,(%rdi) at 0x18026253b); GetSection / Remove memcmp
//     it and ReadFromStream / WriteToStream copy it, all at +0x00.
// ===========================================================================

namespace {

// [MS-OLEPS] 2.20 PropertySetStream header: ByteOrder, Version, SystemIdentifier,
// CLSID, NumPropertySets.  MFC oleprop.h calls it FORMATHEADER.
struct FORMATHEADER {
    WORD  wByteOrder;   // 0xFFFE
    WORD  wFormat;      // Version
    DWORD dwOSVer;      // SystemIdentifier
    CLSID clsID;
    DWORD cSections;    // NumPropertySets
};
static_assert(sizeof(FORMATHEADER) == 0x1c, "FORMATHEADER is the 28-byte [MS-OLEPS] header retail reads/writes");

// [MS-OLEPS] 2.20: one FMTID / Offset pair per property set.  MFC oleprop.h
// calls it SECTIONHEADER.
struct SECTIONHEADER {
    CLSID FormatID;
    DWORD dwOffset;
};
static_assert(sizeof(SECTIONHEADER) == 0x14, "SECTIONHEADER is the 20-byte FMTID/Offset pair retail reads/writes");

// Mirror of OpenMFC_CPtrList::CNode (detail/CPtrListSupport.h) and of the
// retail CPtrList node the disassembly walks: (%rax) = pNext, 0x10(%rax) = data.
struct S_PtrNode {
    S_PtrNode* pNext;
    S_PtrNode* pPrev;
    void*      data;
};
static_assert(offsetof(S_PtrNode, pNext) == 0x00, "CPtrList node pNext");
static_assert(offsetof(S_PtrNode, data) == 0x10, "CPtrList node data");

// Mirror of the retail CPtrList object (OpenMFC_CPtrList keeps this exact layout).
struct S_PtrList {
    void*      vfptr;
    S_PtrNode* m_pNodeHead;
    S_PtrNode* m_pNodeTail;
    INT_PTR    m_nCount;
    void*      m_pNodeFree;
    void*      m_pBlocks;
    INT_PTR    m_nBlockSize;
};
static_assert(sizeof(S_PtrList) == 0x38, "CPtrList is 0x38 bytes");
static_assert(offsetof(S_PtrList, m_pNodeHead) == 0x08, "CPtrList head");
static_assert(offsetof(S_PtrList, m_nCount) == 0x18, "CPtrList count");

// Retail CPropertySet object (see the header comment).
struct S_PropertySet {
    FORMATHEADER m_FH;            // +0x00
    S_PtrList    m_SectionList;   // +0x20
};
static_assert(offsetof(S_PropertySet, m_FH) == 0x00, "m_FH at +0x00");
static_assert(offsetof(S_PropertySet, m_FH) + offsetof(FORMATHEADER, cSections) == 0x18, "cSections at +0x18");
static_assert(offsetof(S_PropertySet, m_SectionList) == 0x20, "m_SectionList at +0x20");
static_assert(offsetof(S_PropertySet, m_SectionList) + offsetof(S_PtrList, m_pNodeHead) == 0x28, "section list head at +0x28");
static_assert(offsetof(S_PropertySet, m_SectionList) + offsetof(S_PtrList, m_nCount) == 0x38, "section list count at +0x38");
static_assert(sizeof(S_PropertySet) == 0x58, "retail CPropertySet is 0x58 bytes");

inline S_PropertySet* PS(void* pThis) { return static_cast<S_PropertySet*>(pThis); }

// sizeof(CPropertySection) in retail: the operator new argument at
// 0x180263488 (AddSection), 0x1802631a8 (Set) and 0x180263872 (ReadFromStream).
const std::size_t kPropertySectionSize = 0x90;

// The CLSID at +0x00 of a retail CPropertySection (m_FormatID); see the header.
inline CLSID* SectionFormatID(void* pSect) { return static_cast<CLSID*>(pSect); }

} // namespace

// --- CPtrList thunks (core/collections/CPtrList.cpp) -----------------------
extern "C" void* MS_ABI impl___0CPtrList__QEAA__J_Z(void* pThis, INT_PTR nBlockSize);                  // ??0CPtrList@@QEAA@_J@Z
extern "C" void  MS_ABI impl___1CPtrList__UEAA_XZ(void* pThis);                                        // ??1CPtrList@@UEAA@XZ
extern "C" void* MS_ABI impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(void* pThis, void* newElement); // ?AddTail@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
extern "C" void  MS_ABI impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(void* pThis, void* position); // ?RemoveAt@CPtrList@@QEAAXPEAU__POSITION@@@Z
extern "C" void  MS_ABI impl__RemoveAll_CPtrList__QEAAXXZ(void* pThis);                                // ?RemoveAll@CPtrList@@QEAAXXZ

// --- CPropertySection thunks (core/ole/CPropertySection.cpp) ---------------
// Signatures derived from the mangled names: `this` first; a GUID passed by
// value is a 16-byte aggregate and therefore arrives by hidden pointer under
// the MS x64 ABI; LARGE_INTEGER by value is an 8-byte union passed in a register.
extern "C" void* MS_ABI impl___0CPropertySection__QEAA_XZ(void* pThis);                                              // ??0CPropertySection@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPropertySection__QEAA_U_GUID___Z(void* pThis, const GUID* pFormatID);               // ??0CPropertySection@@QEAA@U_GUID@@@Z
extern "C" void  MS_ABI impl___1CPropertySection__QEAA_XZ(void* pThis);                                              // ??1CPropertySection@@QEAA@XZ (retail 0x262560 returns nothing; %rax is not `this` on exit)
extern "C" void  MS_ABI impl__AddProperty_CPropertySection__QEAAXPEAVCProperty___Z(void* pThis, void* pProp);        // ?AddProperty@CPropertySection@@QEAAXPEAVCProperty@@@Z
extern "C" void* MS_ABI impl__Get_CPropertySection__QEAAPEAXKPEAK_Z(void* pThis, unsigned long dwPropID, unsigned long* pcb); // ?Get@CPropertySection@@QEAAPEAXKPEAK@Z
extern "C" void* MS_ABI impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(void* pThis, unsigned long dwPropID);      // ?GetProperty@CPropertySection@@QEAAPEAVCProperty@@K@Z
extern "C" void  MS_ABI impl__Remove_CPropertySection__QEAAXK_Z(void* pThis, unsigned long dwPropID);                // ?Remove@CPropertySection@@QEAAXK@Z
extern "C" int   MS_ABI impl__Set_CPropertySection__QEAAHKPEAX_Z(void* pThis, unsigned long dwPropID, void* pValue); // ?Set@CPropertySection@@QEAAHKPEAX@Z
extern "C" int   MS_ABI impl__Set_CPropertySection__QEAAHKPEAXK_Z(void* pThis, unsigned long dwPropID, void* pValue, unsigned long dwType); // ?Set@CPropertySection@@QEAAHKPEAXK@Z
extern "C" int   MS_ABI impl__ReadFromStream_CPropertySection__QEAAHPEAUIStream__T_LARGE_INTEGER___Z(void* pThis, IStream* pStream, long long liPropSet); // ?ReadFromStream@CPropertySection@@QEAAHPEAUIStream@@T_LARGE_INTEGER@@@Z
extern "C" int   MS_ABI impl__WriteToStream_CPropertySection__QEAAHPEAUIStream___Z(void* pThis, IStream* pStream);   // ?WriteToStream@CPropertySection@@QEAAHPEAUIStream@@@Z

// --- this file's own thunks that are called before their definition ---------
extern "C" void* MS_ABI impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(void* pThis, const GUID* pFormatID);
extern "C" void* MS_ABI impl__Get_CPropertySet__QEAAPEAXU_GUID__KPEAK_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID, unsigned long* pcb);
extern "C" void  MS_ABI impl__RemoveAll_CPropertySet__QEAAXXZ(void* pThis);

// --- allocator / exception thunks ------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);   // ??2@YAPEAX_K@Z (retail 0x27f0)
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);           // ??3@YAXPEAX@Z (retail 0x27c0; the sized delete retail calls at 0x2b77b0 tail-jumps to it)
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();   // ?AfxThrowInvalidArgException@@YAXXZ (retail 0x227720)

namespace {

// Shared tail of both constructors (0x2630a0 / 0x2630f0): every store except
// the CLSID copy.  Retail inlines the CPtrList ctor (vptr 0x18032a758, six
// zero/10 stores); OpenMFC constructs the list through its thunk so the
// object carries the OpenMFC CPtrList vtable and a valid empty list.
void ConstructCommon(S_PropertySet* p)
{
    p->m_FH.wByteOrder = 0xFFFE;   // movq $0xfffe,(%rcx): wByteOrder=0xFFFE, wFormat=0, dwOSVer=0
    p->m_FH.wFormat    = 0;
    p->m_FH.dwOSVer    = 0;
    p->m_FH.cSections  = 0;        // mov %edx,0x18(%rcx)
    impl___0CPtrList__QEAA__J_Z(&p->m_SectionList, 10);   // movq $0xa,0x50(%rcx) = m_nBlockSize
}

// Retail's inline walk of m_SectionList looking for a section whose
// m_FormatID (+0x00) memcmp-equals `FormatID` (GetSection, 0x263400).
// Returns the matching node so Remove can hand it to RemoveAt.
S_PtrNode* FindSectionNode(S_PropertySet* p, const GUID* pFormatID)
{
    for (S_PtrNode* pNode = p->m_SectionList.m_pNodeHead; pNode != nullptr; pNode = pNode->pNext) {
        void* pSect = pNode->data;
        // Deviation: retail hands pSect straight to memcmp (VCRUNTIME140
        // memcmp via IAT slot 0x1802c73e0) and would fault on a NULL entry;
        // a NULL entry can only come from AddSection(NULL), so it is skipped.
        if (pSect == nullptr) continue;
        if (std::memcmp(SectionFormatID(pSect), pFormatID, sizeof(GUID)) == 0) return pNode;
    }
    return nullptr;
}

// `delete pSect` as retail spells it: ??1CPropertySection@@ (0x262560) then the
// sized operator delete (0x2b77b0, 0x90 bytes), which forwards to ??3@YAXPEAX@Z.
void DeleteSection(void* pSect)
{
    impl___1CPropertySection__QEAA_XZ(pSect);
    impl___3_YAXPEAX_Z(pSect);
}

} // namespace

// Retail (RVA 0x2630a0, mfc140u), fully transcribed:
//     m_FH.wByteOrder = 0xFFFE; m_FH.wFormat = 0; m_FH.dwOSVer = 0;   // one 8-byte store
//     m_FH.clsID = GUID_NULL;                                          // 16 zero bytes at 0x2d98d8
//     m_FH.cSections = 0;
//     CPtrList::CPtrList(10) inlined into m_SectionList (+0x20);
//     return this;
// Symbol: ??0CPropertySet@@QEAA@XZ
extern "C" void* MS_ABI impl___0CPropertySet__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    S_PropertySet* p = PS(pThis);
    ConstructCommon(p);
    std::memset(&p->m_FH.clsID, 0, sizeof(CLSID));
    return pThis;
}

// Retail (RVA 0x2630f0, mfc140u), fully transcribed: identical to the default
// constructor except that m_FH.clsID is copied from the CLSID argument
// (movups (%rdx),%xmm0 / movdqu %xmm0,0x8(%rcx)); the argument arrives by
// hidden pointer because a CLSID by value is a 16-byte aggregate.
// Symbol: ??0CPropertySet@@QEAA@U_GUID@@@Z
extern "C" void* MS_ABI impl___0CPropertySet__QEAA_U_GUID___Z(void* pThis, const GUID* pClsID) {
    if (pThis == nullptr) return nullptr;
    S_PropertySet* p = PS(pThis);
    ConstructCommon(p);
    if (pClsID != nullptr) std::memcpy(&p->m_FH.clsID, pClsID, sizeof(CLSID));
    else std::memset(&p->m_FH.clsID, 0, sizeof(CLSID));   // deviation: retail dereferences unconditionally
    return pThis;
}

// Retail (RVA 0x263140, mfc140u), fully transcribed:
//     RemoveAll();                                        // 0x2633b0
//     m_SectionList.~CPtrList();                          // inlined: vptr = 0x18032a758, then
//                                                         // tail-jump to CPtrList::RemoveAll (0x8350)
// Symbol: ??1CPropertySet@@QEAA@XZ
extern "C" void MS_ABI impl___1CPropertySet__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__RemoveAll_CPropertySet__QEAAXXZ(pThis);
    impl___1CPtrList__UEAA_XZ(&PS(pThis)->m_SectionList);
}

// Retail (RVA 0x263550, mfc140u), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect != NULL) {
//         pSect->m_PropList.AddTail(pProp);               // CPtrList at pSect+0x18, AddTail 0x231e70
//         pSect->m_SH.cProperties++;                      // incl 0x14(%rbx)
//     }
// The two statements inside the `if` are the same operations as the body of
// the export ?AddProperty@CPropertySection@@QEAAXPEAVCProperty@@@Z (0x2627e0):
// AddTail on this+0x18 then incl 0x14(this).  The bytes are NOT identical (the
// inline copy computes the list pointer with `lea 0x18(%rax),%rcx`, the export
// with `add $0x18,%rcx`), only the effect is, so OpenMFC forwards to that
// thunk instead of touching CPropertySection's members here.
// Symbol: ?AddProperty@CPropertySet@@QEAAXU_GUID@@PEAVCProperty@@@Z
extern "C" void MS_ABI impl__AddProperty_CPropertySet__QEAAXU_GUID__PEAVCProperty___Z(void* pThis, const GUID* pFormatID, void* pProp) {
    if (pThis == nullptr || pFormatID == nullptr) return;
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect != nullptr) {
        impl__AddProperty_CPropertySection__QEAAXPEAVCProperty___Z(pSect, pProp);
    }
}

// Retail (RVA 0x263460, mfc140u), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect != NULL) return pSect;
//     void* p = operator new(0x90);                       // ??2@YAPEAX_K@Z, 0x27f0
//     if (p == NULL) return NULL;                         // 0x18026349a -> xor ebx
//     pSect = new (p) CPropertySection(FormatID);         // 0x262490
//     if (pSect == NULL) return NULL;                     // dead: the ctor returns `this`
//     m_SectionList.AddTail(pSect);                       // 0x231e70
//     m_FH.cSections++;                                   // incl 0x18(%rdi)
//     return pSect;
// Symbol: ?AddSection@CPropertySet@@QEAAPEAVCPropertySection@@U_GUID@@@Z
extern "C" void* MS_ABI impl__AddSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(void* pThis, const GUID* pFormatID) {
    if (pThis == nullptr || pFormatID == nullptr) return nullptr;
    S_PropertySet* p = PS(pThis);
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect != nullptr) return pSect;
    void* pStorage = impl___2_YAPEAX_K_Z(kPropertySectionSize);
    if (pStorage == nullptr) return nullptr;
    pSect = impl___0CPropertySection__QEAA_U_GUID___Z(pStorage, pFormatID);
    if (pSect == nullptr) return nullptr;
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&p->m_SectionList, pSect);
    p->m_FH.cSections++;
    return pSect;
}

// Retail (RVA 0x2634e0, mfc140u), fully transcribed:
//     m_SectionList.AddTail(pSect);                       // 0x231e70 on this+0x20
//     m_FH.cSections++;                                   // incl 0x18(%rbx)
// No NULL check on pSect in retail; none added (the list simply holds NULL).
// Symbol: ?AddSection@CPropertySet@@QEAAXPEAVCPropertySection@@@Z
extern "C" void MS_ABI impl__AddSection_CPropertySet__QEAAXPEAVCPropertySection___Z(void* pThis, void* pSect) {
    if (pThis == nullptr) return;
    S_PropertySet* p = PS(pThis);
    impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&p->m_SectionList, pSect);
    p->m_FH.cSections++;
}

// Retail (RVA 0x2632c0, mfc140u), fully transcribed:
//     return Get(FormatID, dwPropID, NULL);               // 0x263260 with r9 = 0
// Symbol: ?Get@CPropertySet@@QEAAPEAXU_GUID@@K@Z
extern "C" void* MS_ABI impl__Get_CPropertySet__QEAAPEAXU_GUID__K_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID) {
    return impl__Get_CPropertySet__QEAAPEAXU_GUID__KPEAK_Z(pThis, pFormatID, dwPropID, nullptr);
}

// Retail (RVA 0x263260, mfc140u), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect == NULL) return NULL;
//     for (node = pSect->m_PropList.head (pSect+0x20); node; node = node->pNext) {
//         CProperty* pProp = node->data;
//         if (pProp->m_dwPropID (+0x00) == dwPropID) return pProp->Get(pcb);   // 0x261c40
//     }
//     return NULL;
// The loop performs the same walk as the body of
// ?Get@CPropertySection@@QEAAPEAXKPEAK@Z (0x2626f0): same node loads and the
// same compare of m_dwPropID, then CProperty::Get (0x261c40) on the match.
// The two are NOT byte-identical -- the export compares from %edx and
// tail-jumps to CProperty::Get, the inline copy compares from %ebx, `call`s
// it, and returns an explicit `xor %eax,%eax` when the list is exhausted
// (the export just returns the NULL walk pointer) -- but the effect is the
// same, so OpenMFC forwards to that thunk instead of walking
// CPropertySection's list here.
// Symbol: ?Get@CPropertySet@@QEAAPEAXU_GUID@@KPEAK@Z
extern "C" void* MS_ABI impl__Get_CPropertySet__QEAAPEAXU_GUID__KPEAK_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID, unsigned long* pcb) {
    if (pThis == nullptr || pFormatID == nullptr) return nullptr;
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect == nullptr) return nullptr;
    return impl__Get_CPropertySection__QEAAPEAXKPEAK_Z(pSect, dwPropID, pcb);
}

// Retail (RVA 0x2635a0, mfc140u): movzwl (%rcx),%eax -- return m_FH.wByteOrder.
// Symbol: ?GetByteOrder@CPropertySet@@QEAAGXZ
extern "C" unsigned short MS_ABI impl__GetByteOrder_CPropertySet__QEAAGXZ(void* pThis) {
    if (pThis == nullptr) return 0;
    return PS(pThis)->m_FH.wByteOrder;
}

// Retail (RVA 0x2635d0, mfc140u): copies the 16 bytes at +0x08 (m_FH.clsID)
// into the hidden return slot in RDX and returns that pointer in RAX.
// Symbol: ?GetClassID@CPropertySet@@QEAA?AU_GUID@@XZ
extern "C" void* MS_ABI impl__GetClassID_CPropertySet__QEAA_AU_GUID__XZ(void* pThis, GUID* pResult) {
    if (pResult == nullptr) return nullptr;
    if (pThis != nullptr) std::memcpy(pResult, &PS(pThis)->m_FH.clsID, sizeof(CLSID));
    else std::memset(pResult, 0, sizeof(CLSID));
    return pResult;
}

// Retail (RVA 0x21e570, mfc140u): mov 0x38(%rcx),%eax -- returns the low DWORD
// of m_SectionList.m_nCount, NOT m_FH.cSections.
// Symbol: ?GetCount@CPropertySet@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetCount_CPropertySet__QEAAKXZ(void* pThis) {
    if (pThis == nullptr) return 0;
    return static_cast<unsigned long>(PS(pThis)->m_SectionList.m_nCount);
}

// Retail (RVA 0x2635b0, mfc140u): movzwl 0x2(%rcx),%eax -- return m_FH.wFormat.
// Symbol: ?GetFormatVersion@CPropertySet@@QEAAGXZ
extern "C" unsigned short MS_ABI impl__GetFormatVersion_CPropertySet__QEAAGXZ(void* pThis) {
    if (pThis == nullptr) return 0;
    return PS(pThis)->m_FH.wFormat;
}

// Retail (RVA 0x21e580, mfc140u): lea 0x20(%rcx),%rax -- return &m_SectionList.
// Symbol: ?GetList@CPropertySet@@QEAAPEAVCPtrList@@XZ
extern "C" void* MS_ABI impl__GetList_CPropertySet__QEAAPEAVCPtrList__XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    return &PS(pThis)->m_SectionList;
}

// Retail (RVA 0x261db0, mfc140u): mov 0x4(%rcx),%eax -- return m_FH.dwOSVer.
// Symbol: ?GetOSVersion@CPropertySet@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetOSVersion_CPropertySet__QEAAKXZ(void* pThis) {
    if (pThis == nullptr) return 0;
    return PS(pThis)->m_FH.dwOSVer;
}

// Retail (RVA 0x263500, mfc140u), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect == NULL) return NULL;
//     for (node = pSect->m_PropList.head (pSect+0x20); node; node = node->pNext)
//         if (node->data->m_dwPropID (+0x00) == dwPropID) return node->data;
//     return NULL;
// The loop performs the same walk as the body of
// ?GetProperty@CPropertySection@@QEAAPEAVCProperty@@K@Z (0x2627c0): same node
// loads and the same compare of m_dwPropID.  Not byte-identical (the export
// compares from %edx and returns the NULL walk pointer on exhaustion; the
// inline copy compares from %ebx and materialises the NULL with
// `xor %ecx,%ecx`), but the effect is the same, so OpenMFC forwards to that
// thunk.
// Symbol: ?GetProperty@CPropertySet@@QEAAPEAVCProperty@@U_GUID@@K@Z
extern "C" void* MS_ABI impl__GetProperty_CPropertySet__QEAAPEAVCProperty__U_GUID__K_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID) {
    if (pThis == nullptr || pFormatID == nullptr) return nullptr;
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect == nullptr) return nullptr;
    return impl__GetProperty_CPropertySection__QEAAPEAVCProperty__K_Z(pSect, dwPropID);
}

// Retail (RVA 0x263400, mfc140u), fully transcribed:
//     for (node = m_SectionList.head (+0x28); node; node = node->pNext) {
//         CPropertySection* pSect = node->data;
//         if (memcmp(pSect->m_FormatID (+0x00), &FormatID, 16) == 0) return pSect;   // memcmp via IAT 0x1802c73e0
//     }
//     return NULL;
// Symbol: ?GetSection@CPropertySet@@QEAAPEAVCPropertySection@@U_GUID@@@Z
extern "C" void* MS_ABI impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(void* pThis, const GUID* pFormatID) {
    if (pThis == nullptr || pFormatID == nullptr) return nullptr;
    S_PtrNode* pNode = FindSectionNode(PS(pThis), pFormatID);
    return pNode ? pNode->data : nullptr;
}

// Retail (RVA 0x263750, mfc140u -- not in the RVA map, resolved via ordinal
// 12092 / export address table), fully transcribed:
//     ULARGE_INTEGER uliPropSet;  LARGE_INTEGER li = {0};
//     pStream->Seek(li, STREAM_SEEK_CUR, &uliPropSet);            // vslot 5; records where the set starts
//     if (m_FH.cSections != 0 || m_SectionList.m_nCount != 0) RemoveAll();   // 0x2633b0
//     ULONG cb;
//     HRESULT hr = pStream->Read(&m_FH, 0x1c, &cb);               // vslot 3; [MS-OLEPS] PropertySetStream header
//     if (FAILED(hr) || cb != 0x1c) return FALSE;
//     IStream* pClone;
//     if (FAILED(pStream->Clone(&pClone))) return FALSE;         // vslot 13
//     for (DWORD n = m_FH.cSections; n != 0; n--) {
//         SECTIONHEADER sh;
//         hr = pClone->Read(&sh, 0x14, &cb);                      // FMTID/Offset pair from the clone
//         if (FAILED(hr) || cb != 0x14) { pClone->Release(); return FALSE; }
//         pStream->Seek(uliPropSet, STREAM_SEEK_SET, NULL);       // back to the set start ...
//         pStream->Seek(sh.dwOffset, STREAM_SEEK_CUR, NULL);      // ... then to this section
//         CPropertySection* pSect = new CPropertySection();       // operator new 0x90 + 0x2623d0
//         pSect->m_FormatID = sh.FormatID;                         // movdqu %xmm0,(%rbx): +0x00
//         pSect->ReadFromStream(pStream, uliPropSet);              // 0x262a00; return value IGNORED
//         m_SectionList.AddTail(pSect);                            // 0x231e70
//     }
//     pClone->Release();
//     return TRUE;
// Every HRESULT from Seek is ignored, exactly as retail ignores it.
// Deviation: retail never tests operator new's result (MFC's operator new
// throws); OpenMFC's allocator can return NULL, in which case the loop
// releases the clone and returns FALSE instead of writing through NULL.
// Symbol: ?ReadFromStream@CPropertySet@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__ReadFromStream_CPropertySet__QEAAHPEAUIStream___Z(void* pThis, IStream* pStream) {
    if (pThis == nullptr || pStream == nullptr) return 0;
    S_PropertySet* p = PS(pThis);

    LARGE_INTEGER liZero;
    liZero.QuadPart = 0;
    ULARGE_INTEGER uliPropSet;
    uliPropSet.QuadPart = 0;
    pStream->Seek(liZero, STREAM_SEEK_CUR, &uliPropSet);

    if (p->m_FH.cSections != 0 || p->m_SectionList.m_nCount != 0) {
        impl__RemoveAll_CPropertySet__QEAAXXZ(pThis);
    }

    ULONG cb = 0;
    HRESULT hr = pStream->Read(&p->m_FH, sizeof(FORMATHEADER), &cb);
    if (FAILED(hr) || cb != sizeof(FORMATHEADER)) return 0;

    IStream* pClone = nullptr;
    hr = pStream->Clone(&pClone);
    if (FAILED(hr)) return 0;
    if (pClone == nullptr) return 0;   // deviation: retail dispatches through whatever Clone returned

    for (DWORD n = p->m_FH.cSections; n != 0; n--) {
        SECTIONHEADER sh;
        hr = pClone->Read(&sh, sizeof(SECTIONHEADER), &cb);
        if (FAILED(hr) || cb != sizeof(SECTIONHEADER)) {
            pClone->Release();
            return 0;
        }
        LARGE_INTEGER liSeek;
        liSeek.QuadPart = static_cast<LONGLONG>(uliPropSet.QuadPart);
        pStream->Seek(liSeek, STREAM_SEEK_SET, nullptr);
        liSeek.QuadPart = static_cast<LONGLONG>(sh.dwOffset);   // mov %ebx,%edx: zero-extended DWORD
        pStream->Seek(liSeek, STREAM_SEEK_CUR, nullptr);

        void* pStorage = impl___2_YAPEAX_K_Z(kPropertySectionSize);
        if (pStorage == nullptr) {
            pClone->Release();
            return 0;
        }
        void* pSect = impl___0CPropertySection__QEAA_XZ(pStorage);
        std::memcpy(SectionFormatID(pSect), &sh.FormatID, sizeof(GUID));
        impl__ReadFromStream_CPropertySection__QEAAHPEAUIStream__T_LARGE_INTEGER___Z(
            pSect, pStream, static_cast<long long>(uliPropSet.QuadPart));
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&p->m_SectionList, pSect);
    }
    pClone->Release();
    return 1;
}

// Retail (RVA 0x263320, mfc140u -- ordinal 12272, not in the RVA map), fully transcribed:
//     for (node = m_SectionList.head (+0x28); node; node = node->pNext) {
//         CPropertySection* pSect = node->data;
//         if (memcmp(pSect->m_FormatID (+0x00), &FormatID, 16) != 0) continue;
//         m_SectionList.RemoveAt(node);                       // 0x2320d0
//         if (pSect != NULL) { pSect->~CPropertySection(); operator delete(pSect, 0x90); }   // 0x262560, 0x2b77b0
//         m_FH.cSections--;                                    // decl 0x18(%rdi)
//         return;                                              // only the first match is removed
//     }
// Symbol: ?Remove@CPropertySet@@QEAAXU_GUID@@@Z
extern "C" void MS_ABI impl__Remove_CPropertySet__QEAAXU_GUID___Z(void* pThis, const GUID* pFormatID) {
    if (pThis == nullptr || pFormatID == nullptr) return;
    S_PropertySet* p = PS(pThis);
    S_PtrNode* pNode = FindSectionNode(p, pFormatID);
    if (pNode == nullptr) return;
    void* pSect = pNode->data;
    impl__RemoveAt_CPtrList__QEAAXPEAU__POSITION___Z(&p->m_SectionList, pNode);
    if (pSect != nullptr) DeleteSection(pSect);
    p->m_FH.cSections--;
}

// Retail (RVA 0x2632e0, mfc140u -- ordinal 12273, not in the RVA map), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect != NULL) pSect->Remove(dwPropID);         // 0x262710
// Symbol: ?Remove@CPropertySet@@QEAAXU_GUID@@K@Z
extern "C" void MS_ABI impl__Remove_CPropertySet__QEAAXU_GUID__K_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID) {
    if (pThis == nullptr || pFormatID == nullptr) return;
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect != nullptr) impl__Remove_CPropertySection__QEAAXK_Z(pSect, dwPropID);
}

// Retail (RVA 0x2633b0, mfc140u -- ordinal 12291, not in the RVA map), fully transcribed:
//     for (node = m_SectionList.head (+0x28); node; node = node->pNext) {
//         CPropertySection* pSect = node->data;
//         if (pSect != NULL) { pSect->~CPropertySection(); operator delete(pSect, 0x90); }   // 0x262560, 0x2b77b0
//     }
//     m_SectionList.RemoveAll();                              // 0x8350 on this+0x20
//     m_FH.cSections = 0;                                     // mov %ebx,0x18(%rsi) with rbx == 0
// Retail reads node->pNext (mov (%rbx),%rbx) before deleting the section, and
// the section destructor only touches the section's own members, so the walk
// is done in place exactly as retail does it.
// Symbol: ?RemoveAll@CPropertySet@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAll_CPropertySet__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_PropertySet* p = PS(pThis);
    S_PtrNode* pNode = p->m_SectionList.m_pNodeHead;
    while (pNode != nullptr) {
        void* pSect = pNode->data;
        pNode = pNode->pNext;
        if (pSect != nullptr) DeleteSection(pSect);
    }
    impl__RemoveAll_CPtrList__QEAAXXZ(&p->m_SectionList);
    p->m_FH.cSections = 0;
}

// Retail (RVA 0x263220, mfc140u -- ordinal 12855, not in the RVA map), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect == NULL) return FALSE;                     // eax is GetSection's 0
//     return pSect->Set(dwPropID, pValue);                 // 0x262690, result passed through
// Symbol: ?Set@CPropertySet@@QEAAHU_GUID@@KPEAX@Z
extern "C" int MS_ABI impl__Set_CPropertySet__QEAAHU_GUID__KPEAX_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID, void* pValue) {
    if (pThis == nullptr || pFormatID == nullptr) return 0;
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect == nullptr) return 0;
    return impl__Set_CPropertySection__QEAAHKPEAX_Z(pSect, dwPropID, pValue);
}

// Retail (RVA 0x263170, mfc140u -- ordinal 12856, not in the RVA map), fully transcribed:
//     CPropertySection* pSect = GetSection(FormatID);     // 0x263400
//     if (pSect == NULL) {
//         void* p = operator new(0x90);                   // 0x27f0
//         ENSURE(p != NULL);                              // 0x1802631ba -> AfxThrowInvalidArgException (0x227720)
//         pSect = new (p) CPropertySection(FormatID);     // 0x262490
//         ENSURE(pSect != NULL);                          // 0x1802631d8 -> same throw (dead: ctor returns this)
//         m_SectionList.AddTail(pSect);                   // 0x231e70
//         m_FH.cSections++;                               // incl 0x18(%rdi)
//     }
//     pSect->Set(dwPropID, pValue, dwType);               // 0x2625f0; its result is DISCARDED
//     return TRUE;                                        // mov $0x1,%eax unconditionally
// Symbol: ?Set@CPropertySet@@QEAAHU_GUID@@KPEAXK@Z
extern "C" int MS_ABI impl__Set_CPropertySet__QEAAHU_GUID__KPEAXK_Z(void* pThis, const GUID* pFormatID, unsigned long dwPropID, void* pValue, unsigned long dwType) {
    if (pThis == nullptr || pFormatID == nullptr) return 0;
    S_PropertySet* p = PS(pThis);
    void* pSect = impl__GetSection_CPropertySet__QEAAPEAVCPropertySection__U_GUID___Z(pThis, pFormatID);
    if (pSect == nullptr) {
        void* pStorage = impl___2_YAPEAX_K_Z(kPropertySectionSize);
        if (pStorage == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
        pSect = impl___0CPropertySection__QEAA_U_GUID___Z(pStorage, pFormatID);
        if (pSect == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return 0; }
        impl__AddTail_CPtrList__QEAAPEAU__POSITION__PEAX_Z(&p->m_SectionList, pSect);
        p->m_FH.cSections++;
    }
    impl__Set_CPropertySection__QEAAHKPEAXK_Z(pSect, dwPropID, pValue, dwType);
    return 1;
}

// Retail (RVA 0x2635e0, mfc140u -- ordinal 13024, not in the RVA map):
// movups (%rdx),%xmm0 / movdqu %xmm0,0x8(%rcx) -- m_FH.clsID = ClsID.
// Symbol: ?SetClassID@CPropertySet@@QEAAXU_GUID@@@Z
extern "C" void MS_ABI impl__SetClassID_CPropertySet__QEAAXU_GUID___Z(void* pThis, const GUID* pClsID) {
    if (pThis == nullptr || pClsID == nullptr) return;
    std::memcpy(&PS(pThis)->m_FH.clsID, pClsID, sizeof(CLSID));
}

// Retail (RVA 0x2635c0, mfc140u -- ordinal 13218, not in the RVA map):
// mov %dx,0x2(%rcx) -- m_FH.wFormat = wFmtVersion.
// Symbol: ?SetFormatVersion@CPropertySet@@QEAAXG@Z
extern "C" void MS_ABI impl__SetFormatVersion_CPropertySet__QEAAXG_Z(void* pThis, unsigned short wFmtVersion) {
    if (pThis == nullptr) return;
    PS(pThis)->m_FH.wFormat = wFmtVersion;
}

// Retail (RVA 0x261dc0, mfc140u -- ordinal 13417, not in the RVA map):
// mov %edx,0x4(%rcx) -- m_FH.dwOSVer = dwOSVer.
// Symbol: ?SetOSVersion@CPropertySet@@QEAAXK@Z
extern "C" void MS_ABI impl__SetOSVersion_CPropertySet__QEAAXK_Z(void* pThis, unsigned long dwOSVer) {
    if (pThis == nullptr) return;
    PS(pThis)->m_FH.dwOSVer = dwOSVer;
}

// Retail (RVA 0x2635f0, mfc140u -- ordinal 14304, not in the RVA map), fully transcribed:
//     m_FH.cSections = (DWORD)m_SectionList.m_nCount;         // mov 0x38(%rcx),%eax ; mov %eax,0x18(%rcx)
//     ULONG cb;
//     pStream->Write(&m_FH, 0x1c, &cb);                       // vslot 4; HRESULT ignored
//     if (cb != 0x1c) return FALSE;
//     if (m_SectionList.m_nCount == 0) return TRUE;
//     IStream* pClone;
//     if (FAILED(pStream->Clone(&pClone))) return FALSE;     // vslot 13; the clone stays at the FMTID/Offset table
//     ULARGE_INTEGER uliPos;
//     pStream->Seek(m_FH.cSections * 0x14, STREAM_SEEK_CUR, &uliPos);   // skip the table; uliPos = first section's position
//     for (node = m_SectionList.head (+0x28); node; node = node->pNext) {
//         CPropertySection* pSect = node->data;
//         if (!pSect->WriteToStream(pStream)) { pClone->Release(); return FALSE; }   // 0x262810
//         SECTIONHEADER sh = { pSect->m_FormatID (+0x00), uliPos.LowPart };
//         pClone->Write(&sh, 0x14, &cb);                      // HRESULT ignored
//         if (cb != 0x14) { pClone->Release(); return FALSE; }
//         pStream->Seek(0, STREAM_SEEK_CUR, &uliPos);         // position of the next section
//     }
//     pClone->Release();
//     return TRUE;
// Every HRESULT from Write and Seek is ignored, exactly as retail ignores it.
// Symbol: ?WriteToStream@CPropertySet@@QEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__WriteToStream_CPropertySet__QEAAHPEAUIStream___Z(void* pThis, IStream* pStream) {
    if (pThis == nullptr || pStream == nullptr) return 0;
    S_PropertySet* p = PS(pThis);

    p->m_FH.cSections = static_cast<DWORD>(p->m_SectionList.m_nCount);
    ULONG cb = 0;
    pStream->Write(&p->m_FH, sizeof(FORMATHEADER), &cb);
    if (cb != sizeof(FORMATHEADER)) return 0;
    if (p->m_SectionList.m_nCount == 0) return 1;

    IStream* pClone = nullptr;
    HRESULT hr = pStream->Clone(&pClone);
    if (FAILED(hr)) return 0;
    if (pClone == nullptr) return 0;   // deviation: retail dispatches through whatever Clone returned

    LARGE_INTEGER liSkip;
    liSkip.QuadPart = static_cast<LONGLONG>(p->m_FH.cSections) * static_cast<LONGLONG>(sizeof(SECTIONHEADER));
    ULARGE_INTEGER uliPos;
    uliPos.QuadPart = 0;
    pStream->Seek(liSkip, STREAM_SEEK_CUR, &uliPos);

    for (S_PtrNode* pNode = p->m_SectionList.m_pNodeHead; pNode != nullptr; pNode = pNode->pNext) {
        void* pSect = pNode->data;
        if (pSect == nullptr) {   // deviation: retail would call through NULL here
            pClone->Release();
            return 0;
        }
        if (!impl__WriteToStream_CPropertySection__QEAAHPEAUIStream___Z(pSect, pStream)) {
            pClone->Release();
            return 0;
        }
        SECTIONHEADER sh;
        std::memcpy(&sh.FormatID, SectionFormatID(pSect), sizeof(GUID));
        sh.dwOffset = uliPos.LowPart;
        pClone->Write(&sh, sizeof(SECTIONHEADER), &cb);
        if (cb != sizeof(SECTIONHEADER)) {
            pClone->Release();
            return 0;
        }
        LARGE_INTEGER liZero;
        liZero.QuadPart = 0;
        pStream->Seek(liZero, STREAM_SEEK_CUR, &uliPos);
    }
    pClone->Release();
    return 1;
}
