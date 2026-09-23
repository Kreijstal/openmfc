// CArchiveStream — OpenMFC implementation.
// Sources: retail mfc140u.dll disassembly (every RVA below is mfc140u; each
// export was resolved through its ordinal in mfc_complete_ordinal_mapping.json
// and the mfc140u export table, and cross-checked against the retail vftable);
// the shipping afxpriv2.h on this host (atlmfc/include/afxpriv2.h) for the
// declaration.
//
// =============================================================================
// Retail layout
//
//   class CArchiveStream : public IStream {     // afxpriv2.h
//   public:
//       CArchiveStream(CArchive* pArchive);
//       CArchive* m_pArchive;
//       ... the 14 IUnknown/IStream methods ...
//   };
//
//   +0x00  vptr        the IStream vftable (.rdata 0x321088, mfc140u)
//   +0x08  m_pArchive
//   sizeof == 0x10
//
// Both offsets are read from the exported constructor, ??0CArchiveStream
// (RVA 0x1d3680, mfc140u), which is the whole of:
//     lea  vftable(0x321088),%rax ; mov %rdx,0x8(%rcx) ; mov %rax,(%rcx) ; mov %rcx,%rax ; ret
// and every method below reads m_pArchive as `mov 0x8(%rcx),%rcx`.  There is
// no refcount member: AddRef/Release return constants.
//
// Retail vftable at 0x321088 (mfc140u), slot -> target RVA; every target agrees
// with the export-table RVA of the matching export:
//    0 QueryInterface 0x1d36a0     7 CopyTo       0xf4d0 (E_NOTIMPL)
//    1 AddRef         0x3a60       8 Commit       0xf4d0 (E_NOTIMPL)
//    2 Release        0x71e0       9 Revert       0xf4d0 (E_NOTIMPL)
//    3 Read           0x1d3720    10 LockRegion   0xf4d0 (E_NOTIMPL)
//    4 Write          0x1d3750    11 UnlockRegion 0xf4d0 (E_NOTIMPL)
//    5 Seek           0x1d3790    12 Stat         0xf4d0 (E_NOTIMPL)
//    6 SetSize        0x71e0      13 Clone        0xf4d0 (E_NOTIMPL)
// 0x3a60 (`mov $1,%eax; ret`), 0x71e0 (`xor %eax,%eax; ret`) and 0xf4d0
// (`mov $0x80004001,%eax; ret`) are linker-folded constant bodies shared with
// unrelated exports; the mfc140u symbol map names 0x3a60 after
// CMFCBaseAccessibleObject::accDoDefaultAction and 0x71e0 after
// COleUILinkInfo::AddRef (0xf4d0 happens to be named after
// CArchiveStream::Clone).  Note SetSize shares Release's body and returns
// S_OK, NOT E_NOTIMPL.
//
// OpenMFC deviations (all deliberate):
//   * The vtable.  Clients call CArchiveStream through its IStream vptr, so the
//     exported constructor must install one.  This DLL has no MSVC-emitted
//     vftable for the class, so kCArchiveStreamVtbl below is hand-authored in
//     the retail slot order (table above), each entry pointing at this file's
//     own MS_ABI thunk -- the same technique as g_CMFCAcceleratorKey_vtbl in
//     featurepack/customize/CMFCAcceleratorKey.cpp.  Unlike the retail
//     vftable it is not preceded by an RTTI Complete Object Locator slot.
//   * The archive's file.  Retail Seek reads CArchive::m_pFile straight off
//     retail's CArchive layout (+0x30).  OpenMFC's CArchive
//     (include/openmfc/afx.h) is a different layout (it has an OpenMFC vptr
//     and declares m_pFile as its first data member), and OpenMFC's own
//     CArchive::Read/Write/Flush thunks operate on that layout, so this file
//     reads the pointer through the inline CArchive::GetFile() accessor.
//   * Exceptions.  Retail wraps the CArchive::Read call in Read, the
//     CArchive::Write call in Write, and -- in Seek -- only the CFile::Seek
//     call and the *plibNewPosition store (NOT the preceding CArchive::Flush,
//     which the IP-to-state map puts in state -1, outside the try) in
//     try { ... } catch (CException* e) { e->Delete(); return E_UNEXPECTED; }
//     (decoded from each function's FuncInfo4: one try block, catch type
//     .PEAVCException@@, catch object at frame +0x20, funclet 0x2bddbb which
//     calls CException::Delete (0x227370) when the object is non-NULL; the
//     continuations are the `mov $0x8000ffff,%eax` blocks cited below).  This
//     DLL raises MFC exceptions through the MSVC runtime's _CxxThrowException,
//     which unwinds straight through gcc frames without a gcc catch clause
//     seeing it (verified note in core/ole/COleDocument.cpp, "Exceptions:"), so
//     the catch arm is not reproduced: on the normal path the behaviour is
//     identical; an archive exception propagates to the caller instead of
//     being converted to E_UNEXPECTED.
//
// =============================================================================

#include <cstddef>
#include <cstring>
#include <windows.h>
#include <objbase.h>

#include "openmfc/afx.h"

#if defined(__GNUC__) || defined(__clang__)
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

// CArchive thunks (definitions in core/runtime/Thunks.cpp, correctly typed).
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* pBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* pBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Flush_CArchive__QEAAXXZ(CArchive* pThis);

// Virtual CFile::Seek(LONGLONG, UINT) through the object's MSVC-layout vtable,
// slot 13 (core/file/FileDispatch.cpp).  Retail Seek makes the same call as
// `mov (%rsi),%rax; mov 0x68(%rax),%rax; call guard-dispatch` -- +0x68 is slot 13.
extern "C" unsigned long long MS_ABI OpenMFC_File_Seek(CFile* pFile, long long lOff, unsigned int nFrom);

// Forward declarations of this file's thunks, so the vtable can point at them.
extern "C" long MS_ABI impl__QueryInterface_CArchiveStream__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObject);
extern "C" unsigned long MS_ABI impl__AddRef_CArchiveStream__UEAAKXZ(void* pThis);
extern "C" unsigned long MS_ABI impl__Release_CArchiveStream__UEAAKXZ(void* pThis);
extern "C" long MS_ABI impl__Read_CArchiveStream__UEAAJPEAXKPEAK_Z(void* pThis, void* pv, unsigned long cb, unsigned long* pcbRead);
extern "C" long MS_ABI impl__Write_CArchiveStream__UEAAJPEBXKPEAK_Z(void* pThis, const void* pv, unsigned long cb, unsigned long* pcbWritten);
extern "C" long MS_ABI impl__Seek_CArchiveStream__UEAAJT_LARGE_INTEGER__KPEAT_ULARGE_INTEGER___Z(void* pThis, long long dlibMove, unsigned long dwOrigin, ULARGE_INTEGER* plibNewPosition);
extern "C" long MS_ABI impl__SetSize_CArchiveStream__UEAAJT_ULARGE_INTEGER___Z(void* pThis, unsigned long long libNewSize);
extern "C" long MS_ABI impl__CopyTo_CArchiveStream__UEAAJPEAUIStream__T_ULARGE_INTEGER__PEAT3_2_Z(void* pThis, IStream* pstm, unsigned long long cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);
extern "C" long MS_ABI impl__Commit_CArchiveStream__UEAAJK_Z(void* pThis, unsigned long grfCommitFlags);
extern "C" long MS_ABI impl__Revert_CArchiveStream__UEAAJXZ(void* pThis);
extern "C" long MS_ABI impl__LockRegion_CArchiveStream__UEAAJT_ULARGE_INTEGER__0K_Z(void* pThis, unsigned long long libOffset, unsigned long long cb, unsigned long dwLockType);
extern "C" long MS_ABI impl__UnlockRegion_CArchiveStream__UEAAJT_ULARGE_INTEGER__0K_Z(void* pThis, unsigned long long libOffset, unsigned long long cb, unsigned long dwLockType);
extern "C" long MS_ABI impl__Stat_CArchiveStream__UEAAJPEAUtagSTATSTG__K_Z(void* pThis, STATSTG* pstatstg, unsigned long grfStatFlag);
extern "C" long MS_ABI impl__Clone_CArchiveStream__UEAAJPEAPEAUIStream___Z(void* pThis, IStream** ppstm);

namespace {

// Retail layout, pinned (see the header comment).
struct S_CArchiveStream {
    const void* const* vptr;   // +0x00
    CArchive* m_pArchive;      // +0x08
};
static_assert(offsetof(S_CArchiveStream, vptr) == 0x00, "CArchiveStream vptr");
static_assert(offsetof(S_CArchiveStream, m_pArchive) == 0x08, "CArchiveStream::m_pArchive (ctor 0x1d3680: mov %rdx,0x8(%rcx))");
static_assert(sizeof(S_CArchiveStream) == 0x10, "sizeof(CArchiveStream)");

// By-value LARGE_INTEGER / ULARGE_INTEGER are 8-byte aggregates passed in one
// register under the MS x64 ABI; the thunks model them as 64-bit integers.
static_assert(sizeof(LARGE_INTEGER) == sizeof(long long), "LARGE_INTEGER by value");
static_assert(sizeof(ULARGE_INTEGER) == sizeof(unsigned long long), "ULARGE_INTEGER by value");

// Hand-authored MSVC-layout IStream vtable, retail slot order (vftable
// 0x321088, mfc140u).
const void* const kCArchiveStreamVtbl[14] = {
    (const void*)&impl__QueryInterface_CArchiveStream__UEAAJAEBU_GUID__PEAPEAX_Z,              //  0
    (const void*)&impl__AddRef_CArchiveStream__UEAAKXZ,                                        //  1
    (const void*)&impl__Release_CArchiveStream__UEAAKXZ,                                       //  2
    (const void*)&impl__Read_CArchiveStream__UEAAJPEAXKPEAK_Z,                                 //  3
    (const void*)&impl__Write_CArchiveStream__UEAAJPEBXKPEAK_Z,                                //  4
    (const void*)&impl__Seek_CArchiveStream__UEAAJT_LARGE_INTEGER__KPEAT_ULARGE_INTEGER___Z,   //  5
    (const void*)&impl__SetSize_CArchiveStream__UEAAJT_ULARGE_INTEGER___Z,                     //  6
    (const void*)&impl__CopyTo_CArchiveStream__UEAAJPEAUIStream__T_ULARGE_INTEGER__PEAT3_2_Z,  //  7
    (const void*)&impl__Commit_CArchiveStream__UEAAJK_Z,                                       //  8
    (const void*)&impl__Revert_CArchiveStream__UEAAJXZ,                                        //  9
    (const void*)&impl__LockRegion_CArchiveStream__UEAAJT_ULARGE_INTEGER__0K_Z,                // 10
    (const void*)&impl__UnlockRegion_CArchiveStream__UEAAJT_ULARGE_INTEGER__0K_Z,              // 11
    (const void*)&impl__Stat_CArchiveStream__UEAAJPEAUtagSTATSTG__K_Z,                         // 12
    (const void*)&impl__Clone_CArchiveStream__UEAAJPEAPEAUIStream___Z,                         // 13
};

inline S_CArchiveStream* Self(void* pThis) { return static_cast<S_CArchiveStream*>(pThis); }

} // namespace

// CArchiveStream::CArchiveStream(CArchive*) -- RVA 0x1d3680 (mfc140u):
//     this->vptr = &CArchiveStream::`vftable';   // .rdata 0x321088
//     this->m_pArchive = pArchive;               // +0x08
//     return this;
// Deviation: the vptr stored is this file's kCArchiveStreamVtbl (see header).
// Symbol: ??0CArchiveStream@@QEAA@PEAVCArchive@@@Z
extern "C" void* MS_ABI impl___0CArchiveStream__QEAA_PEAVCArchive___Z(void* pThis, CArchive* pArchive) {
    S_CArchiveStream* self = Self(pThis);
    self->m_pArchive = pArchive;
    self->vptr = kCArchiveStreamVtbl;
    return pThis;
}

// CArchiveStream::AddRef -- RVA 0x3a60 (mfc140u), a folded `mov $1,%eax; ret`.
// No reference count.  Retail's own users construct the object inline in a
// stack slot (e.g. `lea vftable; mov %rax,0x28(%rsp)` at instruction 0x1ed5a7,
// mfc140u), so nothing ever frees it through Release.
// Symbol: ?AddRef@CArchiveStream@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CArchiveStream__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 1;
}

// CArchiveStream::Clone -- RVA 0xf4d0 (mfc140u): `mov $0x80004001,%eax; ret`.
// Symbol: ?Clone@CArchiveStream@@UEAAJPEAPEAUIStream@@@Z
extern "C" long MS_ABI impl__Clone_CArchiveStream__UEAAJPEAPEAUIStream___Z(void* pThis, IStream** ppstm) {
    (void)pThis; (void)ppstm;   // retail does not touch *ppstm
    return E_NOTIMPL;
}

// CArchiveStream::Commit -- RVA 0xf4d0 (mfc140u), the same folded E_NOTIMPL body.
// Symbol: ?Commit@CArchiveStream@@UEAAJK@Z
extern "C" long MS_ABI impl__Commit_CArchiveStream__UEAAJK_Z(void* pThis, unsigned long grfCommitFlags) {
    (void)pThis; (void)grfCommitFlags;
    return E_NOTIMPL;
}

// CArchiveStream::CopyTo -- RVA 0xf4d0 (mfc140u), the same folded E_NOTIMPL body.
// Symbol: ?CopyTo@CArchiveStream@@UEAAJPEAUIStream@@T_ULARGE_INTEGER@@PEAT3@2@Z
extern "C" long MS_ABI impl__CopyTo_CArchiveStream__UEAAJPEAUIStream__T_ULARGE_INTEGER__PEAT3_2_Z(
    void* pThis, IStream* pstm, unsigned long long cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten) {
    (void)pThis; (void)pstm; (void)cb; (void)pcbRead; (void)pcbWritten;   // out-params untouched, as retail
    return E_NOTIMPL;
}

// CArchiveStream::LockRegion -- RVA 0xf4d0 (mfc140u), the same folded E_NOTIMPL body.
// Symbol: ?LockRegion@CArchiveStream@@UEAAJT_ULARGE_INTEGER@@0K@Z
extern "C" long MS_ABI impl__LockRegion_CArchiveStream__UEAAJT_ULARGE_INTEGER__0K_Z(
    void* pThis, unsigned long long libOffset, unsigned long long cb, unsigned long dwLockType) {
    (void)pThis; (void)libOffset; (void)cb; (void)dwLockType;
    return E_NOTIMPL;
}

// CArchiveStream::QueryInterface -- RVA 0x1d36a0 (mfc140u):
//     if (memcmp(&riid, &IID_IUnknown, 16) != 0 &&      // .rdata 0x2d9a58, memcmp via IAT 0x1802c73e0
//         memcmp(&riid, &IID_IStream, 16) != 0)         // .rdata 0x2d9af8
//         return E_NOINTERFACE;                         // 0x80004002; *ppvObject untouched
//     if (ppvObject == NULL) return E_POINTER;          // 0x80004003
//     *ppvObject = this;                                // no AddRef call (it is a constant anyway)
//     return S_OK;
// Symbol: ?QueryInterface@CArchiveStream@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CArchiveStream__UEAAJAEBU_GUID__PEAPEAX_Z(void* pThis, const GUID* riid, void** ppvObject) {
    if (std::memcmp(riid, &IID_IUnknown, sizeof(GUID)) != 0 &&
        std::memcmp(riid, &IID_IStream, sizeof(GUID)) != 0)
        return E_NOINTERFACE;
    if (ppvObject == nullptr)
        return E_POINTER;
    *ppvObject = pThis;
    return S_OK;
}

// CArchiveStream::Read -- RVA 0x1d3720 (mfc140u):
//     try { n = m_pArchive->Read(pv, cb); }             // CArchive::Read, direct call 0x1d1840
//     catch (CException* e) { e->Delete(); return E_UNEXPECTED; }   // continuation 0x1d373e
//     if (pcbRead != NULL) *pcbRead = n;
//     return S_OK;
// Deviation: the catch arm is not reproduced (header comment, "Exceptions").
// Symbol: ?Read@CArchiveStream@@UEAAJPEAXKPEAK@Z
extern "C" long MS_ABI impl__Read_CArchiveStream__UEAAJPEAXKPEAK_Z(void* pThis, void* pv, unsigned long cb, unsigned long* pcbRead) {
    unsigned int n = impl__Read_CArchive__QEAAIPEAXI_Z(Self(pThis)->m_pArchive, pv, cb);
    if (pcbRead != nullptr)
        *pcbRead = n;
    return S_OK;
}

// CArchiveStream::Release -- RVA 0x71e0 (mfc140u), a folded `xor %eax,%eax; ret`.
// Symbol: ?Release@CArchiveStream@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CArchiveStream__UEAAKXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// CArchiveStream::Revert -- RVA 0xf4d0 (mfc140u), the same folded E_NOTIMPL body.
// Symbol: ?Revert@CArchiveStream@@UEAAJXZ
extern "C" long MS_ABI impl__Revert_CArchiveStream__UEAAJXZ(void* pThis) {
    (void)pThis;
    return E_NOTIMPL;
}

// CArchiveStream::Seek -- RVA 0x1d3790 (mfc140u):
//     CFile* pFile = m_pArchive->m_pFile;               // retail CArchive +0x30
//     if (pFile == NULL) return E_NOTIMPL;              // 0x80004001
//     m_pArchive->Flush();                              // CArchive::Flush, direct call 0x1d1be0;
//                                                       // OUTSIDE the try (IP-to-state: state -1
//                                                       // from 0x1d37b7, state 0 from 0x1d37cd)
//     try {
//         ULONGLONG pos = pFile->Seek(dlibMove.QuadPart, dwOrigin);   // vtable +0x68 (slot 13)
//         if (plibNewPosition != NULL) plibNewPosition->QuadPart = pos;
//     } catch (CException* e) { e->Delete(); return E_UNEXPECTED; }  // continuation 0x1d37e0
//     return S_OK;
// dwOrigin is passed straight through: STREAM_SEEK_SET/CUR/END (0/1/2) equal
// CFile::begin/current/end.  pFile is read BEFORE the Flush and the Seek goes
// to that saved pointer, as retail does.
// Deviations: m_pFile comes from the OpenMFC CArchive layout via GetFile(), and
// the catch arm is not reproduced (header comment).
// Symbol: ?Seek@CArchiveStream@@UEAAJT_LARGE_INTEGER@@KPEAT_ULARGE_INTEGER@@@Z
extern "C" long MS_ABI impl__Seek_CArchiveStream__UEAAJT_LARGE_INTEGER__KPEAT_ULARGE_INTEGER___Z(
    void* pThis, long long dlibMove, unsigned long dwOrigin, ULARGE_INTEGER* plibNewPosition) {
    CArchive* pArchive = Self(pThis)->m_pArchive;
    CFile* pFile = pArchive->GetFile();
    if (pFile == nullptr)
        return E_NOTIMPL;
    impl__Flush_CArchive__QEAAXXZ(pArchive);
    unsigned long long pos = OpenMFC_File_Seek(pFile, dlibMove, dwOrigin);
    if (plibNewPosition != nullptr)
        plibNewPosition->QuadPart = pos;
    return S_OK;
}

// CArchiveStream::SetSize -- RVA 0x71e0 (mfc140u), the folded `xor %eax,%eax; ret`
// it shares with Release: it returns S_OK and does nothing.
// Symbol: ?SetSize@CArchiveStream@@UEAAJT_ULARGE_INTEGER@@@Z
extern "C" long MS_ABI impl__SetSize_CArchiveStream__UEAAJT_ULARGE_INTEGER___Z(void* pThis, unsigned long long libNewSize) {
    (void)pThis; (void)libNewSize;
    return S_OK;
}

// CArchiveStream::Stat -- RVA 0xf4d0 (mfc140u), the same folded E_NOTIMPL body.
// Symbol: ?Stat@CArchiveStream@@UEAAJPEAUtagSTATSTG@@K@Z
extern "C" long MS_ABI impl__Stat_CArchiveStream__UEAAJPEAUtagSTATSTG__K_Z(void* pThis, STATSTG* pstatstg, unsigned long grfStatFlag) {
    (void)pThis; (void)pstatstg; (void)grfStatFlag;   // *pstatstg untouched, as retail
    return E_NOTIMPL;
}

// CArchiveStream::UnlockRegion -- RVA 0xf4d0 (mfc140u), the same folded E_NOTIMPL body.
// Symbol: ?UnlockRegion@CArchiveStream@@UEAAJT_ULARGE_INTEGER@@0K@Z
extern "C" long MS_ABI impl__UnlockRegion_CArchiveStream__UEAAJT_ULARGE_INTEGER__0K_Z(
    void* pThis, unsigned long long libOffset, unsigned long long cb, unsigned long dwLockType) {
    (void)pThis; (void)libOffset; (void)cb; (void)dwLockType;
    return E_NOTIMPL;
}

// CArchiveStream::Write -- RVA 0x1d3750 (mfc140u):
//     try { m_pArchive->Write(pv, cb); }                // CArchive::Write, direct call 0x1d1a70
//     catch (CException* e) { e->Delete(); return E_UNEXPECTED; }   // continuation 0x1d3775
//     if (pcbWritten != NULL) *pcbWritten = cb;         // the requested count, not a measured one
//     return S_OK;
// Deviation: the catch arm is not reproduced (header comment, "Exceptions").
// Symbol: ?Write@CArchiveStream@@UEAAJPEBXKPEAK@Z
extern "C" long MS_ABI impl__Write_CArchiveStream__UEAAJPEBXKPEAK_Z(void* pThis, const void* pv, unsigned long cb, unsigned long* pcbWritten) {
    impl__Write_CArchive__QEAAXPEBXI_Z(Self(pThis)->m_pArchive, pv, cb);
    if (pcbWritten != nullptr)
        *pcbWritten = cb;
    return S_OK;
}
