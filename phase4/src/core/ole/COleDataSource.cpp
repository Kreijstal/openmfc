// COleDataSource — OpenMFC implementation.
// Sources: olecore.cpp; retail mfc140u.dll disassembly (bodies cited per thunk).
//
// =============================================================================
// Object model -- read this before touching any thunk below
// =============================================================================
// OpenMFC's COleDataSource is NOT laid out like retail. Retail (afxole.h:192,
// offsets read from the bodies cited below):
//     +0x00  CCmdTarget (0x40 bytes; m_pModuleState at +0x38)
//     +0x40  AFX_DATACACHE_ENTRY* m_pDataCache      (Lookup: `add 0x40(%rsi),%rdx`)
//     +0x48  UINT m_nMaxSize                         (GetCacheEntry: `lea 0x48(%rdi)`)
//     +0x4c  UINT m_nSize                            (Lookup: `cmp %edi,0x4c(%rcx)`)
//     +0x50  UINT m_nGrowBy                          (GetCacheEntry: `add 0x50(%rdi)`)
//     +0x58  XDataObject m_xDataObject               (XDataObject::*: `lea -0x58(%rcx)`)
//     sizeof == 0x60
// and AFX_DATACACHE_ENTRY (only forward-declared in the shipped headers; layout
// decoded from Lookup/GetCacheEntry, 0x40-byte stride `shl $0x6`):
//     +0x00 FORMATETC m_formatEtc, +0x20 STGMEDIUM m_stgMedium,
//     +0x38 DATADIR m_nDataDir.
//
// OpenMFC's class (include/openmfc/afxole.h) instead has a 0x20-byte CCmdTarget,
// BOOL m_bClipboardOwner @0x20, void* m_pDataCache @0x28 pointing at a side
// table (detail/OlecoreSupport.h: DataCacheState, a std::vector<DataCacheEntry>
// plus a separately allocated IDataObject, DataSourceDataObject, which is what
// COleDataSource::GetInterface hands out), then padding up to the same 0x60.
// DataCacheEntry happens to put FORMATETC @0 and STGMEDIUM @0x20 in a 0x40-byte
// record like AFX_DATACACHE_ENTRY, but +0x38 holds three bools (hasMedium,
// delayRender, delayRenderFile), not a DATADIR. It has no data direction at
// all. The COleDataSource members that create entries (CacheData,
// CacheGlobalData, DelayRenderData, DelayRenderFileData) are the ones retail
// files as DATADIR_GET. The side table's own IDataObject
// (DataSourceDataObject::SetData, detail/OlecoreSupport.cpp) ALSO pushes an
// entry for whatever an outside caller hands it. Retail has no counterpart for
// that: its XDataObject::SetData only reaches OnSetData, and only for a
// DelaySetData format. Such an entry is indistinguishable from the others,
// so it is treated as DATADIR_GET too.
// The thunks below therefore treat every OpenMFC entry as DATADIR_GET and treat
// the DATADIR_SET list as empty (DelaySetData, the only retail producer of
// DATADIR_SET entries, stays a stub for exactly this reason).
//
// Nested XDataObject methods: retail enters them with rcx == &m_xDataObject
// and recovers the data source with `lea -0x58(%rcx)`. The thunks do the same
// subtraction (kXDataObjectOffset) and then work on the OpenMFC object found
// there. OpenMFC never builds an interface at +0x58 (its IDataObject lives in
// the side table), so these entry points are reached only by a caller that
// computes &source + 0x58 itself, i.e. a retail-layout client.
//
// Deviations common to the nested methods, stated once here:
//   * AFX_MANAGE_STATE. GetData/GetDataHere/SetData open with
//     AFX_MAINTAIN_STATE2(pThis->m_pModuleState @+0x38) (the call to
//     0x133170 (mfc140u) right after the prologue). OpenMFC's CCmdTarget has no
//     m_pModuleState, so the module-state switch is omitted.
//   * Exceptions. Those three wrap their render/set call in TRY/CATCH_ALL. The
//     catch continuations are the `mov 0x..(%rsp),%eax` blocks that nothing
//     falls into: 0x253aed, 0x253bf5 and 0x253d09. They were identified from the
//     code shape; the FuncInfo tables were not decoded. This DLL raises MFC
//     exceptions through the MSVC runtime's _CxxThrowException, which a gcc
//     catch clause does not see (verified note in core/ole/COleDocument.cpp,
//     "Exceptions:"), so the catch arms are not reproduced: on the normal path
//     the behaviour is identical; an exception propagates instead of being
//     turned into an HRESULT.
//   * OnRenderData / OnSetData. Retail calls them through the data source's
//     vtable (slots 24 and 25, `call *0xc0(%rax)` / `*0xc8(%rax)`). OpenMFC's
//     COleDataSource declares neither as a virtual, so its vtable has no such
//     slots; the thunks call this file's OnRenderData/OnSetData thunks
//     directly. An override in a derived class is therefore not reached.
//
// Every RVA below is mfc140u.dll unless it says "mfc140.dll". The bodies were
// decoded in mfc140.dll and their mfc140u counterparts re-read with
// `disas.py --u` (entry points, call targets, IAT slots, and the short bodies
// in full). The only structural difference found is in _AfxCopyStgMedium's
// TYMED_FILE arm (see CopyStgMediumLikeRetail).
// =============================================================================

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"
#include "detail/CSharedFileSupport.h"   // sizeof(CSharedFile) for the stack buffer in OnRenderData

#include <cstddef>
#include <cstring>
#include <cwchar>

// --- pinned layout ----------------------------------------------------------
// Retail sizeof(COleDataSource) is 0x60 (COleServerItem::OnGetClipboardData
// allocates `operator new(0x60)`, see core/ole/COleServerItem.cpp); OpenMFC
// keeps the same size, so &source + 0x58 is inside every OpenMFC object.
static constexpr std::size_t kXDataObjectOffset = 0x58;
static_assert(sizeof(COleDataSource) == 0x60, "COleDataSource must keep retail sizeof 0x60");
static_assert(offsetof(COleDataSource, m_pDataCache) + sizeof(void*) <= kXDataObjectOffset,
              "OpenMFC data members must not overlap the retail m_xDataObject slot");
// DataCacheEntry is handed out as AFX_DATACACHE_ENTRY* by Lookup; these are
// the two fields it shares with the retail record.
static_assert(offsetof(openmfc::detail::olecore::DataCacheEntry, format) == 0x00, "m_formatEtc @0");
static_assert(offsetof(openmfc::detail::olecore::DataCacheEntry, medium) == 0x20, "m_stgMedium @0x20");
static_assert(sizeof(openmfc::detail::olecore::DataCacheEntry) == 0x40, "0x40-byte record");

// --- sibling thunks (every definition checked in the tree) -------------------
// core/file/CSharedFile.cpp
extern "C" void  MS_ABI impl___0CSharedFile__QEAA_II_Z(void* pThis, unsigned int nAllocFlags, unsigned int nGrowBytes);
extern "C" void  MS_ABI impl___1CSharedFile__UEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl__Detach_CSharedFile__QEAAPEAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(void* pThis, void* hGlobalMemory, int bAllowGrow);
// core/ole/COleStreamFile.cpp and core/ole/Thunks.cpp
extern "C" COleStreamFile* MS_ABI impl___0COleStreamFile__QEAA_PEAUIStream___Z(COleStreamFile* pThis, LPSTREAM lpStream);
extern "C" void     MS_ABI impl___1COleStreamFile__UEAA_XZ(void* pThis);
extern "C" void     MS_ABI impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(COleStreamFile* pThis, IStream* lpStream);
extern "C" IStream* MS_ABI impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ(COleStreamFile* pThis);
extern "C" int      MS_ABI impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z(void* pThis, void* pError);
// core/ole/Thunks.cpp
extern "C" void* MS_ABI impl___0CEnumFormatEtc__QEAA_XZ(void* pThis);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
// this file (forward declarations: used by the XDataObject thunks above their definitions)
extern "C" int MS_ABI impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    COleDataSource* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium);
extern "C" int MS_ABI impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    COleDataSource* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium, int bRelease);

namespace {

using openmfc::detail::olecore::DataCacheEntry;
using openmfc::detail::olecore::DataCacheState;

COleDataSource* SourceFromDataObject(void* pThis) {
    return reinterpret_cast<COleDataSource*>(static_cast<unsigned char*>(pThis) - kXDataObjectOffset);
}

// COleDataSource::Lookup -- RVA 0x2534e0 (mfc140u), transcribed:
//     AFX_DATACACHE_ENTRY* pLast = NULL;
//     for (UINT i = 0; i < m_nSize; i++) {
//         AFX_DATACACHE_ENTRY* e = &m_pDataCache[i];
//         if (e->cf != f->cf) continue;
//         if ((e->tymed & f->tymed) == 0) continue;
//         if (e->dwAspect != DVASPECT_THUMBNAIL && e->dwAspect != DVASPECT_ICON  // `(a-2) & ~2`
//             && e->m_stgMedium.tymed != TYMED_NULL                              // `cmpl $0,0x20(%rdx)`
//             && !(e->lindex == f->lindex
//                  || (e->lindex == 0 && f->lindex == -1)
//                  || (e->lindex == -1 && f->lindex == 0))) continue;
//         if (f->dwAspect != e->dwAspect) continue;
//         if (e->m_nDataDir != nDataDir) continue;
//         pLast = e;                         // set BEFORE the ptd test
//         if (e->ptd == NULL ? f->ptd == NULL
//             : (f->ptd != NULL && e->ptd->tdSize == f->ptd->tdSize
//                && memcmp(e->ptd, f->ptd, e->ptd->tdSize) == 0))
//             break;                         // exact target-device match
//     }
//     return pLast;   // so a ptd mismatch still returns the last near-match
// OpenMFC: the loop runs over the side table's entries, all of which are
// DATADIR_GET (header comment), so a DATADIR_SET lookup finds nothing.
DataCacheEntry* LookupEntry(const COleDataSource* pSource, const FORMATETC* f, DWORD nDataDir) {
    DataCacheState* state = openmfc::detail::olecore::GetDataCacheState(
        const_cast<COleDataSource*>(pSource), false);
    if (!state || nDataDir != DATADIR_GET) return nullptr;
    DataCacheEntry* pLast = nullptr;
    for (DataCacheEntry& entry : state->entries) {
        const FORMATETC& e = entry.format;
        if (e.cfFormat != f->cfFormat) continue;
        if ((e.tymed & f->tymed) == 0) continue;
        if (e.dwAspect != DVASPECT_THUMBNAIL && e.dwAspect != DVASPECT_ICON &&
            entry.medium.tymed != TYMED_NULL &&
            !(e.lindex == f->lindex ||
              (e.lindex == 0 && f->lindex == -1) ||
              (e.lindex == -1 && f->lindex == 0)))
            continue;
        if (f->dwAspect != e.dwAspect) continue;
        pLast = &entry;
        const DVTARGETDEVICE* ptd1 = e.ptd;
        const DVTARGETDEVICE* ptd2 = f->ptd;
        if (ptd1 == nullptr) {
            if (ptd2 == nullptr) break;
        } else if (ptd2 != nullptr && ptd1->tdSize == ptd2->tdSize &&
                   std::memcmp(ptd1, ptd2, ptd1->tdSize) == 0) {
            break;
        }
    }
    return pLast;
}

// _AfxCopyGlobalMemory(hDest, hSource) -- RVA 0x25f748 (mfc140u; 0x25e678 in
// mfc140.dll), not exported. Transcribed:
//     UINT nSize = (UINT)GlobalSize(hSource);
//     if (hDest == NULL) { hDest = GlobalAlloc(GMEM_SHARE|GMEM_MOVEABLE /*0x2002*/, nSize);
//                          if (hDest == NULL) return NULL; }
//     else if (nSize > GlobalSize(hDest)) return NULL;
//     src = GlobalLock(hSource); dst = GlobalLock(hDest);
//     if (nSize != 0) memcpy_s(dst, GlobalSize(hDest), src, nSize);
//     GlobalUnlock(hDest); GlobalUnlock(hSource); return hDest;
// A failing memcpy_s ends in AfxThrowInvalidArgException (0x227720) after the
// CRT invalid-parameter report; reproduced as the throw alone.
HGLOBAL CopyGlobalMemoryLikeRetail(HGLOBAL hDest, HGLOBAL hSource) {
    const UINT nSize = static_cast<UINT>(::GlobalSize(hSource));
    if (hDest == nullptr) {
        hDest = ::GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, nSize);
        if (hDest == nullptr) return nullptr;
    } else if (nSize > ::GlobalSize(hDest)) {
        return nullptr;
    }
    void* lpSource = ::GlobalLock(hSource);
    void* lpDest = ::GlobalLock(hDest);
    const SIZE_T nDestSize = ::GlobalSize(hDest);
    if (nSize != 0) {
        if (lpDest == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
        if (lpSource == nullptr || nDestSize < nSize) {
            std::memset(lpDest, 0, nDestSize);          // memcpy_s clears dest before failing
            impl__AfxThrowInvalidArgException__YAXXZ();
        }
        std::memcpy(lpDest, lpSource, nSize);
    }
    ::GlobalUnlock(hDest);
    ::GlobalUnlock(hSource);
    return hDest;
}

// _AfxCopyStgMedium(cfFormat, lpDest, lpSource) -- RVA 0x25f83c (mfc140u;
// 0x25e76c in mfc140.dll), not exported; the copy helper GetData and
// GetDataHere call. Transcribed:
//   Phase 1, only when lpDest->tymed == TYMED_NULL, switch on lpSource->tymed:
//     HGLOBAL, ENHMF, GDI: dest->tymed = source tymed; dest->hGlobal = NULL;
//                          fall into phase 2
//     FILE:    dest->tymed = TYMED_FILE; NULL source name -> AfxThrowInvalidArgException;
//              dest->lpszFileName = CoTaskMemAlloc((wcslen+1)*2), NULL -> FALSE;
//              copy the name; return TRUE
//     ISTREAM / ISTORAGE: share the source pointer, AddRef (slot 1), set
//              dest->tymed; return TRUE
//     MFPICT:  h = _AfxCopyGlobalMemory(NULL, src->hMetaFilePict), NULL -> FALSE;
//              pict = GlobalLock(h); pict->hMF = CopyMetaFile(pict->hMF, NULL);
//              NULL -> GlobalUnlock(h), GlobalFree(h), FALSE;
//              GlobalUnlock(h); dest->hMetaFilePict = h; dest->tymed = MFPICT; TRUE
//     other:   FALSE
//   Phase 2 (dest already typed), switch on lpSource->tymed:
//     HGLOBAL:  h = _AfxCopyGlobalMemory(dest->hGlobal, src->hGlobal); NULL -> FALSE;
//               dest->hGlobal = h; TRUE
//     FILE:     return CopyFile(src name, dest name, FALSE)  (each name through a
//               CString, NULL passed through as NULL). This is the arm whose code
//               differs between the images: mfc140.dll builds two CStringA
//               (call 0x3b330 twice, mfc140.dll) and calls CopyFileA; mfc140u
//               builds two CStringW (one through a call, one inline) and calls
//               CopyFileW (iatu.py 0x1802c67c0). Same semantics.
//     ISTREAM:  Stat(&st, STATFLAG_NONAME) (slot 12) != S_OK -> FALSE;
//               Seek(0, SET) dest then source (slot 5);
//               src->CopyTo(dest, st.cbSize, NULL, NULL) (slot 7) != S_OK -> FALSE;
//               Seek(0, SET) dest then source; TRUE
//     ISTORAGE: src->CopyTo(0, NULL, NULL, dest) (IStorage slot 7) == S_OK
//     GDI, ENHMF: dest->hGlobal != NULL -> FALSE;
//               dest->hGlobal = OleDuplicateData(src->hGlobal, cfFormat, 0); != NULL
//     other (MFPICT into a typed dest included): FALSE
// Speculative devirtualization is switched off for this function under GCC:
// the only IStream implementation visible in this unit is the
// CArchiveStream helper in detail/OlecoreSupport.h, and GCC otherwise inlines
// its Stat/Seek/CopyTo guesses here, dragging in its CArchive::Read/Write
// calls, which have no C++ definition to link against (thunk rule).
#if defined(__GNUC__) && !defined(__clang__)
__attribute__((optimize("no-devirtualize-speculatively")))
#endif
BOOL CopyStgMediumLikeRetail(CLIPFORMAT cfFormat, STGMEDIUM* lpDest, STGMEDIUM* lpSource) {
    if (lpDest->tymed == TYMED_NULL) {
        switch (lpSource->tymed) {
        case TYMED_HGLOBAL:
        case TYMED_ENHMF:
        case TYMED_GDI:
            lpDest->tymed = lpSource->tymed;
            lpDest->hGlobal = nullptr;
            break;
        case TYMED_FILE: {
            lpDest->tymed = TYMED_FILE;
            if (lpSource->lpszFileName == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
            const unsigned long long cb =
                static_cast<unsigned long long>(static_cast<UINT>(std::wcslen(lpSource->lpszFileName) + 1)) * 2;
            lpDest->lpszFileName = cb <= 0xFFFFFFFFull
                ? static_cast<LPOLESTR>(::CoTaskMemAlloc(static_cast<ULONG>(cb))) : nullptr;
            if (lpDest->lpszFileName == nullptr) return FALSE;
            if (cb != 0) std::memcpy(lpDest->lpszFileName, lpSource->lpszFileName, static_cast<size_t>(cb));
            return TRUE;
        }
        case TYMED_ISTREAM:
            lpDest->pstm = lpSource->pstm;
            lpDest->pstm->AddRef();
            lpDest->tymed = TYMED_ISTREAM;
            return TRUE;
        case TYMED_ISTORAGE:
            lpDest->pstg = lpSource->pstg;
            lpDest->pstg->AddRef();
            lpDest->tymed = TYMED_ISTORAGE;
            return TRUE;
        case TYMED_MFPICT: {
            HGLOBAL hDest = CopyGlobalMemoryLikeRetail(nullptr, lpSource->hMetaFilePict);
            if (hDest == nullptr) return FALSE;
            METAFILEPICT* lpPict = static_cast<METAFILEPICT*>(::GlobalLock(hDest));
            lpPict->hMF = ::CopyMetaFile(lpPict->hMF, nullptr);
            if (lpPict->hMF == nullptr) {
                ::GlobalUnlock(hDest);
                ::GlobalFree(hDest);
                return FALSE;
            }
            ::GlobalUnlock(hDest);
            lpDest->hMetaFilePict = hDest;
            lpDest->tymed = TYMED_MFPICT;
            return TRUE;
        }
        default:
            return FALSE;
        }
    }

    switch (lpSource->tymed) {
    case TYMED_HGLOBAL: {
        HGLOBAL h = CopyGlobalMemoryLikeRetail(lpDest->hGlobal, lpSource->hGlobal);
        if (h == nullptr) return FALSE;
        lpDest->hGlobal = h;
        return TRUE;
    }
    case TYMED_FILE:
        return ::CopyFile(lpSource->lpszFileName, lpDest->lpszFileName, FALSE);
    case TYMED_ISTREAM: {
        STATSTG stat;
        if (lpSource->pstm->Stat(&stat, STATFLAG_NONAME) != S_OK) return FALSE;
        LARGE_INTEGER zero;
        zero.QuadPart = 0;
        lpDest->pstm->Seek(zero, STREAM_SEEK_SET, nullptr);
        lpSource->pstm->Seek(zero, STREAM_SEEK_SET, nullptr);
        if (lpSource->pstm->CopyTo(lpDest->pstm, stat.cbSize, nullptr, nullptr) != S_OK) return FALSE;
        lpDest->pstm->Seek(zero, STREAM_SEEK_SET, nullptr);
        lpSource->pstm->Seek(zero, STREAM_SEEK_SET, nullptr);
        return TRUE;
    }
    case TYMED_ISTORAGE:
        return lpSource->pstg->CopyTo(0, nullptr, nullptr, lpDest->pstg) == S_OK;
    case TYMED_GDI:
    case TYMED_ENHMF:
        if (lpDest->hGlobal != nullptr) return FALSE;
        lpDest->hGlobal = ::OleDuplicateData(lpSource->hGlobal, cfFormat, 0);
        return lpDest->hGlobal != nullptr;
    default:
        return FALSE;
    }
}

// Stack objects for OnRenderData, built through the sibling thunks (the C++
// methods have no linkable definition outside their own units). The
// destructors run on normal exit only; an MFC exception unwinds past them (see
// the header comment), as in retail no object has acquired anything by the
// time either throw site in OnRenderData is reached.
struct SharedFileOnStack {
    alignas(8) unsigned char buf[sizeof(CSharedFile)];
    SharedFileOnStack(UINT nAllocFlags, UINT nGrowBytes) {
        impl___0CSharedFile__QEAA_II_Z(buf, nAllocFlags, nGrowBytes);
    }
    ~SharedFileOnStack() { impl___1CSharedFile__UEAA_XZ(buf); }
    CFile* File() { return reinterpret_cast<CFile*>(buf); }
};

struct StreamFileOnStack {
    alignas(8) unsigned char buf[sizeof(COleStreamFile)];
    StreamFileOnStack() { impl___0COleStreamFile__QEAA_PEAUIStream___Z(Obj(), nullptr); }
    ~StreamFileOnStack() { impl___1COleStreamFile__UEAA_XZ(buf); }
    COleStreamFile* Obj() { return reinterpret_cast<COleStreamFile*>(buf); }
};

} // namespace

COleDataSource::COleDataSource()
    : m_bClipboardOwner(FALSE), m_pDataCache(nullptr) {
    memset(_oledatasource_padding, 0, sizeof(_oledatasource_padding));
}
COleDataSource::~COleDataSource() {
    Empty();
}
void COleDataSource::CacheGlobalData(CLIPFORMAT cfFormat, HGLOBAL hGlobal, FORMATETC* lpFormatEtc) {
    STGMEDIUM medium = {};
    medium.tymed = TYMED_HGLOBAL;
    medium.hGlobal = hGlobal;
    medium.pUnkForRelease = nullptr;
    CacheData(cfFormat, &medium, lpFormatEtc);
}
void COleDataSource::CacheData(CLIPFORMAT cfFormat, STGMEDIUM* lpStorageMedium, FORMATETC* lpFormatEtc) {
    if (!lpStorageMedium) return;

    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;

    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }

    entry->format = format;
    entry->delayRender = false;
    entry->delayRenderFile = false;
    entry->hasMedium = CopyStorageMedium(*lpStorageMedium, &entry->medium);
}
void COleDataSource::DelayRenderData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }
    entry->format = format;
    entry->delayRender = true;
    entry->delayRenderFile = false;
}
void COleDataSource::DelayRenderFileData(CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return;
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    format.tymed = TYMED_FILE;
    DataCacheEntry* entry = FindCacheEntry(state, format);
    if (!entry) {
        state->entries.push_back(DataCacheEntry());
        entry = &state->entries.back();
    } else if (entry->hasMedium) {
        ReleaseStgMedium(&entry->medium);
        entry->hasMedium = false;
    }
    entry->format = format;
    entry->delayRender = false;
    entry->delayRenderFile = true;
}
void COleDataSource::SetClipboard() {
    IDataObject* dataObject = GetInterface(TRUE);
    if (dataObject && SUCCEEDED(OleSetClipboard(dataObject))) {
        if (g_pClipboardOwner && g_pClipboardOwner != this) {
            g_pClipboardOwner->m_bClipboardOwner = FALSE;
        }
        g_pClipboardOwner = this;
        m_bClipboardOwner = TRUE;
    }
    if (dataObject) dataObject->Release();
}
COleDataSource* COleDataSource::GetClipboardOwner() {
    return g_pClipboardOwner;
}
DROPEFFECT COleDataSource::DoDragDrop(DWORD dwEffects, LPCRECT lpRectStartDrag,
                                       COleDropSource* pDropSource) {
    (void)lpRectStartDrag;
    IDataObject* dataObject = GetInterface(TRUE);
    if (!dataObject) return DROPEFFECT_NONE;

    COleDropSource defaultDropSource;
    COleDropSource* source = pDropSource ? pDropSource : &defaultDropSource;
    DropSourceAdapter* adapter = new DropSourceAdapter(source);
    DWORD effect = DROPEFFECT_NONE;
    HRESULT hr = ::DoDragDrop(dataObject, adapter, dwEffects, &effect);
    adapter->Release();
    dataObject->Release();
    return SUCCEEDED(hr) ? effect : DROPEFFECT_NONE;
}
int COleDataSource::OnRenderGlobalData(FORMATETC* lpFormatEtc, void** phGlobal) {
    if (phGlobal) *phGlobal = nullptr;
    if (!lpFormatEtc || !phGlobal) return FALSE;
    DataCacheState* state = GetDataCacheState(this, false);
    DataCacheEntry* entry = FindCacheEntry(state, *lpFormatEtc);
    if (!entry || !entry->hasMedium || entry->medium.tymed != TYMED_HGLOBAL) return FALSE;
    *phGlobal = DuplicateGlobalMemory(entry->medium.hGlobal);
    return *phGlobal != nullptr;
}
int COleDataSource::OnRenderFileData(FORMATETC* lpFormatEtc, CFile* pFile) {
    if (!lpFormatEtc || !pFile) return FALSE;
    DataCacheState* state = GetDataCacheState(this, false);
    DataCacheEntry* entry = FindCacheEntry(state, *lpFormatEtc);
    if (!entry || !entry->hasMedium) return FALSE;

    if (entry->medium.tymed == TYMED_HGLOBAL && entry->medium.hGlobal) {
        SIZE_T size = GlobalSize(entry->medium.hGlobal);
        void* data = GlobalLock(entry->medium.hGlobal);
        if (!data) return FALSE;
        pFile->Write(data, static_cast<UINT>(size));
        GlobalUnlock(entry->medium.hGlobal);
        return TRUE;
    }
    if (entry->medium.tymed == TYMED_FILE && entry->medium.lpszFileName) {
        CFile source(entry->medium.lpszFileName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary);
        if (source.m_hFile == INVALID_HANDLE_VALUE) {
            return FALSE;
        }
        BYTE buffer[4096];
        UINT read = 0;
        while ((read = source.Read(buffer, sizeof(buffer))) > 0) {
            pFile->Write(buffer, read);
        }
        return TRUE;
    }
    return FALSE;
}
void COleDataSource::Empty() {
    if (g_pClipboardOwner == this) g_pClipboardOwner = nullptr;
    delete static_cast<DataCacheState*>(m_pDataCache);
    m_pDataCache = nullptr;
    m_bClipboardOwner = FALSE;
}
LPDATAOBJECT COleDataSource::GetInterface(BOOL bAddRef) {
    DataCacheState* state = GetDataCacheState(this, true);
    if (!state) return nullptr;
    if (bAddRef) state->dataObject.AddRef();
    return &state->dataObject;
}

// COleDataSource::DelaySetData(CLIPFORMAT, LPFORMATETC) -- RVA 0x253480
// (mfc140u), transcribed:
//     FORMATETC fe;  LPFORMATETC p = lpFormatEtc;
//     if (p == NULL && cfFormat != 0) {      // NB: cfFormat == 0 leaves p NULL
//         fe = { cfFormat, NULL, DVASPECT_CONTENT, -1, (DWORD)-1 /*all TYMEDs*/ };
//         p = &fe;
//     }
//     AFX_DATACACHE_ENTRY* e = GetCacheEntry(p, DATADIR_SET);   // call 0x2531a0
//     e->m_stgMedium.tymed = TYMED_NULL; hGlobal = NULL; pUnkForRelease = NULL;
// STUB: this registers a DATADIR_SET cache entry, and OpenMFC's DataCacheEntry
// has no data-direction field (header comment), so there is nothing to record
// it in. Filing it as an ordinary entry would make it visible to GetData,
// QueryGetData and EnumFormatEtc as if it were data this source offers.
// Consequence: XDataObject::SetData below never finds a DATADIR_SET entry.
// Symbol: ?DelaySetData@COleDataSource@@QEAAXGPEAUtagFORMATETC@@@Z
extern "C" void MS_ABI impl__DelaySetData_COleDataSource__QEAAXGPEAUtagFORMATETC___Z(
    COleDataSource* pThis, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)pThis; (void)cfFormat; (void)lpFormatEtc;
}

// COleDataSource::FlushClipboard() (static) -- RVA 0x253110 (mfc140u),
// transcribed in full:
//     if (GetClipboardOwner() != NULL)       // call 0x253130 == GetClipboardOwner
//         ::OleFlushClipboard();             // iatu.py 0x1802c79f0 -> ole32!OleFlushClipboard
// GetClipboardOwner is this file's C++ definition. Note it differs from retail
// (0x253130), which also asks ::OleIsCurrentClipboard and forgets the owner
// once another data object has replaced it; OpenMFC's returns the last
// SetClipboard caller unconditionally, so after a flush it can still be
// non-NULL. That is a GetClipboardOwner gap, not reproduced or fixed here.
// Symbol: ?FlushClipboard@COleDataSource@@SAXXZ
extern "C" void MS_ABI impl__FlushClipboard_COleDataSource__SAXXZ() {
    if (COleDataSource::GetClipboardOwner() != nullptr)
        ::OleFlushClipboard();
}

// COleDataSource::GetCacheEntry(LPFORMATETC, DATADIR) -- RVA 0x2531a0
// (mfc140u), transcribed:
//     AFX_DATACACHE_ENTRY* e = Lookup(lpFormatEtc, nDataDir);   // call 0x2534e0
//     if (e != NULL) {
//         CoTaskMemFree(e->m_formatEtc.ptd);                      // ole32
//         ReleaseStgMedium(&e->m_stgMedium);                      // ole32
//     } else {
//         if (m_pDataCache == NULL || m_nSize == m_nMaxSize) {    // grow by m_nGrowBy
//             p = operator new((m_nMaxSize + m_nGrowBy) * 0x40);
//             if (m_pDataCache) { memcpy_s(p, .., m_pDataCache, m_nSize * 0x40);
//                                 free(m_pDataCache); }   // CRT free via the IAT
//             m_nMaxSize += m_nGrowBy; m_pDataCache = p;
//         }
//         e = &m_pDataCache[m_nSize++];
//     }
//     e->m_nDataDir = nDataDir;
//     e->m_formatEtc = *lpFormatEtc;   // 32-byte copy, ptd pointer included
//     return e;                        // never NULL (allocation failure throws)
// STUB: the caller fills e->m_stgMedium itself (retail CacheData at 0x253340,
// CacheGlobalData at 0x2532d0 and DelaySetData above all write it through the
// returned pointer). OpenMFC's DataCacheEntry decides whether it owns a
// medium by its separate hasMedium flag, which such a caller cannot set, so a
// medium written through the returned pointer would be ignored by the
// IDataObject GetInterface hands out and never released; and there is no
// m_nDataDir to store (header comment). Returning an entry would lose data
// silently, so this stays NULL (as the generated stub was).
// Symbol: ?GetCacheEntry@COleDataSource@@IEAAPEAUAFX_DATACACHE_ENTRY@@PEAUtagFORMATETC@@W4tagDATADIR@@@Z
extern "C" void* MS_ABI impl__GetCacheEntry_COleDataSource__IEAAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z(
    COleDataSource* pThis, FORMATETC* lpFormatEtc, DATADIR nDataDir) {
    (void)pThis; (void)lpFormatEtc; (void)nDataDir;
    return nullptr;
}

// COleDataSource::Lookup(LPFORMATETC, DATADIR) const -- RVA 0x2534e0 (mfc140u);
// the transcription is on LookupEntry at the top of this file.
// Stated deviation: the returned record is OpenMFC's DataCacheEntry. Its
// m_formatEtc (+0) and m_stgMedium (+0x20) are where a retail caller expects
// them (static_asserts at the top); +0x38 is not m_nDataDir.
// Symbol: ?Lookup@COleDataSource@@IEBAPEAUAFX_DATACACHE_ENTRY@@PEAUtagFORMATETC@@W4tagDATADIR@@@Z
extern "C" void* MS_ABI impl__Lookup_COleDataSource__IEBAPEAUAFX_DATACACHE_ENTRY__PEAUtagFORMATETC__W4tagDATADIR___Z(
    const COleDataSource* pThis, FORMATETC* lpFormatEtc, DATADIR nDataDir) {
    return LookupEntry(pThis, lpFormatEtc, static_cast<DWORD>(nDataDir));
}

// COleDataSource::OnRenderData(LPFORMATETC, LPSTGMEDIUM) -- RVA 0x2535a0
// (mfc140u; ordinal 10901, resolved from the export table -- the symbol map has
// no entry), transcribed:
//     if (lpFormatEtc->tymed & TYMED_HGLOBAL) {
//         HGLOBAL hGlobal = lpStgMedium->hGlobal;       // read whatever tymed says
//         if (OnRenderGlobalData(lpFormatEtc, &hGlobal)) {      // vslot 22 (0xb0)
//             lpStgMedium->tymed = TYMED_HGLOBAL; lpStgMedium->hGlobal = hGlobal;
//             return TRUE;
//         }
//         CSharedFile file(GMEM_MOVEABLE /*2*/, 0x1000);        // 0x22b480
//         if (lpStgMedium->tymed == TYMED_HGLOBAL)
//             file.SetHandle(lpStgMedium->hGlobal, FALSE);      // 0x22b500
//         if (OnRenderFileData(lpFormatEtc, &file)) {           // vslot 23 (0xb8)
//             lpStgMedium->tymed = TYMED_HGLOBAL;
//             lpStgMedium->hGlobal = file.Detach();             // 0x22b610
//             return TRUE;                                      // ~CSharedFile 0x22b4d0
//         }
//         if (lpStgMedium->tymed == TYMED_HGLOBAL)
//             file.Detach();                   // hand the caller's block back
//     }
//     if (lpFormatEtc->tymed & TYMED_ISTREAM) {
//         COleStreamFile file(NULL);                            // 0x264d10
//         if (lpStgMedium->tymed == TYMED_ISTREAM) {
//             if (lpStgMedium->pstm == NULL) AfxThrowInvalidArgException();  // 0x227720
//             file.Attach(lpStgMedium->pstm);  // inline: m_lpStream(+0x28) = pstm, no
//                                              // AddRef; m_bCloseOnDelete(+0x10) = FALSE
//         } else if (!file.CreateMemoryStream(NULL))            // 0x265490
//             AfxThrowMemoryException();                        // 0x2276c0
//         if (OnRenderFileData(lpFormatEtc, &file)) {           // vslot 23
//             lpStgMedium->tymed = TYMED_ISTREAM;
//             lpStgMedium->pstm = file.Detach();                // inline
//             return TRUE;                                      // ~COleStreamFile 0x264e80
//         }
//         if (lpStgMedium->tymed == TYMED_ISTREAM)
//             file.Detach();                   // inline; caller keeps its stream
//     }
//     return FALSE;
// (0x22b500 has no symbol-map entry in mfc140u. The export table resolves
// ?SetHandle@CSharedFile ordinal 13228 to it, and the mfc140.dll body calls
// ?SetHandle@CSharedFile at 0x229ba0 (mfc140.dll) from the identical position.)
// OpenMFC: OnRenderGlobalData/OnRenderFileData are virtuals of OpenMFC's own
// class, so the C++ calls below dispatch virtually as retail does. The slots
// are OpenMFC's, not retail's 22/23. The two files are built through the
// sibling thunks. OpenMFC's COleStreamFile::Attach (core/ole/COleStreamFile.cpp)
// also stores the stream without AddRef, and every path detaches the caller's
// stream before the destructor runs. It does not clear m_bCloseOnDelete as
// retail's inline Attach does; that flag is not consulted here, because the
// object never holds a file handle.
// Symbol: ?OnRenderData@COleDataSource@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    COleDataSource* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium) {
    if (lpFormatEtc->tymed & TYMED_HGLOBAL) {
        HGLOBAL hGlobal = lpStgMedium->hGlobal;
        if (pThis->OnRenderGlobalData(lpFormatEtc, reinterpret_cast<void**>(&hGlobal))) {
            lpStgMedium->tymed = TYMED_HGLOBAL;
            lpStgMedium->hGlobal = hGlobal;
            return TRUE;
        }
        SharedFileOnStack file(GMEM_MOVEABLE, 0x1000);
        if (lpStgMedium->tymed == TYMED_HGLOBAL)
            impl__SetHandle_CSharedFile__QEAAXPEAXH_Z(file.buf, lpStgMedium->hGlobal, FALSE);
        if (pThis->OnRenderFileData(lpFormatEtc, file.File())) {
            lpStgMedium->tymed = TYMED_HGLOBAL;
            lpStgMedium->hGlobal = impl__Detach_CSharedFile__QEAAPEAXXZ(file.buf);
            return TRUE;
        }
        if (lpStgMedium->tymed == TYMED_HGLOBAL)
            impl__Detach_CSharedFile__QEAAPEAXXZ(file.buf);
    }
    if (lpFormatEtc->tymed & TYMED_ISTREAM) {
        StreamFileOnStack file;
        if (lpStgMedium->tymed == TYMED_ISTREAM) {
            if (lpStgMedium->pstm == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
            impl__Attach_COleStreamFile__QEAAXPEAUIStream___Z(file.Obj(), lpStgMedium->pstm);
        } else if (!impl__CreateMemoryStream_COleStreamFile__QEAAHPEAVCFileException___Z(file.Obj(), nullptr)) {
            impl__AfxThrowMemoryException__YAXXZ();
        }
        if (pThis->OnRenderFileData(lpFormatEtc, file.Obj())) {
            lpStgMedium->tymed = TYMED_ISTREAM;
            lpStgMedium->pstm = impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ(file.Obj());
            return TRUE;
        }
        if (lpStgMedium->tymed == TYMED_ISTREAM)
            impl__Detach_COleStreamFile__QEAAPEAUIStream__XZ(file.Obj());
    }
    return FALSE;
}

// COleDataSource::OnSetData(LPFORMATETC, LPSTGMEDIUM, BOOL) -- ordinal 11052
// resolves (export table, mfc140u) to RVA 0x71e0, a shared folded
// `xor %eax,%eax; ret` body (the map labels it COleUILinkInfo::AddRef). The base
// class accepts nothing: return FALSE. Exact.
// Symbol: ?OnSetData@COleDataSource@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    COleDataSource* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium, int bRelease) {
    (void)pThis; (void)lpFormatEtc; (void)lpStgMedium; (void)bRelease;
    return FALSE;
}

// COleDataSource::XDataObject::DAdvise -- RVA 0x253e50 (mfc140u), exact:
//     *pdwConnection = 0;               // 5th argument, `mov 0x28(%rsp),%rax`
//     return OLE_E_ADVISENOTSUPPORTED;  // 0x80040003
// `this` is never read. No NULL check on pdwConnection, as in retail.
// Symbol: ?DAdvise@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(
    void* pThis, FORMATETC* lpFormatEtc, unsigned long advf, IAdviseSink* pAdvSink, unsigned long* pdwConnection) {
    (void)pThis; (void)lpFormatEtc; (void)advf; (void)pAdvSink;
    *pdwConnection = 0;
    return OLE_E_ADVISENOTSUPPORTED;
}

// COleDataSource::XDataObject::DUnadvise -- RVA 0x253e70 (mfc140u), exact:
// `mov $0x80040003,%eax; ret` -- OLE_E_ADVISENOTSUPPORTED.
// Symbol: ?DUnadvise@XDataObject@COleDataSource@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleDataSource__UEAAJK_Z(void* pThis, unsigned long dwConnection) {
    (void)pThis; (void)dwConnection;
    return OLE_E_ADVISENOTSUPPORTED;
}

// COleDataSource::XDataObject::EnumDAdvise -- RVA 0x253e80 (mfc140u), exact:
//     *ppenumAdvise = NULL; return OLE_E_ADVISENOTSUPPORTED;   (no NULL check)
// Symbol: ?EnumDAdvise@XDataObject@COleDataSource@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleDataSource__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, IEnumSTATDATA** ppenumAdvise) {
    (void)pThis;
    *ppenumAdvise = nullptr;
    return OLE_E_ADVISENOTSUPPORTED;
}

// COleDataSource::XDataObject::EnumFormatEtc -- RVA 0x253d50 (mfc140u),
// transcribed (pThis = this - 0x58; m_nSize read as -0xc(%r13), m_pDataCache
// as -0x18(%r13), i.e. +0x4c / +0x40 of the data source):
//     if (ppenumFormatEtc == NULL) return E_POINTER;
//     *ppenumFormatEtc = NULL;
//     CEnumFormatEtc* pList = new CEnumFormatEtc;          // 0x78 bytes, ctor 0x253750
//     for (UINT i = 0; i < pThis->m_nSize; i++) {
//         AFX_DATACACHE_ENTRY* e = &pThis->m_pDataCache[i];
//         if ((DWORD)e->m_nDataDir & dwDirection) {         // bit test, not equality
//             FORMATETC fe = e->m_formatEtc;
//             fe.ptd = _AfxOleCopyTargetDevice(e->m_formatEtc.ptd);   // 0x260648
//             pList->AddFormat(&fe);                        // 0x253880
//         }
//     }
//     *ppenumFormatEtc = (LPENUMFORMATETC)&pList->m_xEnumVOID;   // +0x68
//     return S_OK;                // a CATCH_ALL returns E_OUTOFMEMORY instead
// OpenMFC: its CEnumFormatEtc is a different, self-contained class (an
// IEnumFORMATETC itself, released by `delete this`). Its constructor thunk
// (core/ole/Thunks.cpp) works, but its AddFormat thunk (core/ole/
// CEnumFormatEtc.cpp) is a no-op. The C++ CEnumFormatEtc::AddFormat IS defined
// (core/ole/CEnumFormatEtc.cpp) and DataSourceDataObject calls it, but this
// unit follows the thunk-only rule for cross-unit C++ calls, which the per-file
// link audit enforces. So the populated list here is built by the data
// source's own IDataObject (the DataSourceDataObject that GetInterface
// returns); its EnumFormatEtc(DATADIR_GET) lists every side-table entry. Since every
// OpenMFC entry is DATADIR_GET (header comment), that is exactly the set
// retail's loop selects whenever dwDirection has the DATADIR_GET bit, so that
// case is forwarded there. Without the bit, retail's loop selects nothing (no
// DATADIR_SET entries exist here) and returns an empty list with S_OK; that is
// built directly: operator new + the constructor thunk, no AddFormat.
// Stated deviation: the populated enumerator is OpenMFC's, which hands out
// FORMATETCs with ptd == NULL instead of a copied target device.
// Symbol: ?EnumFormatEtc@XDataObject@COleDataSource@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleDataSource__UEAAJKPEAPEAUIEnumFORMATETC___Z(
    void* pThis, unsigned long dwDirection, IEnumFORMATETC** ppenumFormatEtc) {
    if (ppenumFormatEtc == nullptr) return E_POINTER;
    *ppenumFormatEtc = nullptr;
    if (dwDirection & DATADIR_GET) {
        IDataObject* pDataObject = SourceFromDataObject(pThis)->GetInterface(FALSE);
        return pDataObject->EnumFormatEtc(DATADIR_GET, ppenumFormatEtc);
    }
    CEnumFormatEtc* pList = static_cast<CEnumFormatEtc*>(
        impl___0CEnumFormatEtc__QEAA_XZ(::operator new(sizeof(CEnumFormatEtc))));
    *ppenumFormatEtc = pList;
    return S_OK;
}

// COleDataSource::XDataObject::GetCanonicalFormatEtc -- RVA 0x253c70
// (mfc140u), exact: `mov $0x40130,%eax; ret` -- DATA_S_SAMEFORMATETC, neither
// `this` nor either FORMATETC touched.
// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__0_Z(
    void* pThis, FORMATETC* lpFormatEtcIn, FORMATETC* lpFormatEtcOut) {
    (void)pThis; (void)lpFormatEtcIn; (void)lpFormatEtcOut;
    return DATA_S_SAMEFORMATETC;
}

// COleDataSource::XDataObject::GetData -- RVA 0x253a20 (mfc140u), transcribed:
//     COleDataSource* pThis = this - 0x58;          // `lea -0x58(%rcx),%rsi`
//     AFX_MANAGE_STATE(pThis->m_pModuleState);      // omitted, header comment
//     if (lpFormatEtc == NULL || lpStgMedium == NULL) return E_INVALIDARG;
//     AFX_DATACACHE_ENTRY* e = pThis->Lookup(lpFormatEtc, DATADIR_GET);   // 0x2534e0
//     if (e == NULL) return DATA_E_FORMATETC;       // 0x80040064
//     memset(lpStgMedium, 0, sizeof(STGMEDIUM));    // iatu.py 0x1802c7418 -> memset
//     if (e->m_stgMedium.tymed != TYMED_NULL)       // cached, not delay-rendered
//         return _AfxCopyStgMedium(lpFormatEtc->cfFormat, lpStgMedium,
//                                  &e->m_stgMedium) ? S_OK : DATA_E_FORMATETC;   // 0x25f83c
//     return pThis->OnRenderData(lpFormatEtc, lpStgMedium)    // vslot 24, in TRY
//         ? S_OK : DATA_E_FORMATETC;
// Deviations: module state, TRY/CATCH and the direct OnRenderData call -- all in
// the header comment.
// Symbol: ?GetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium) {
    COleDataSource* pSource = SourceFromDataObject(pThis);
    if (lpFormatEtc == nullptr || lpStgMedium == nullptr) return E_INVALIDARG;
    DataCacheEntry* pEntry = LookupEntry(pSource, lpFormatEtc, DATADIR_GET);
    if (pEntry == nullptr) return DATA_E_FORMATETC;
    std::memset(lpStgMedium, 0, sizeof(STGMEDIUM));
    if (pEntry->medium.tymed != TYMED_NULL) {
        return CopyStgMediumLikeRetail(lpFormatEtc->cfFormat, lpStgMedium, &pEntry->medium)
            ? S_OK : DATA_E_FORMATETC;
    }
    return impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
               pSource, lpFormatEtc, lpStgMedium)
        ? S_OK : DATA_E_FORMATETC;
}

// COleDataSource::XDataObject::GetDataHere -- RVA 0x253b30 (mfc140u),
// transcribed:
//     COleDataSource* pThis = this - 0x58;          // `lea -0x58(%rcx),%rsi`
//     AFX_MANAGE_STATE(pThis->m_pModuleState);      // omitted, header comment
//     if (lpFormatEtc == NULL || lpStgMedium == NULL) return E_INVALIDARG;
//     lpFormatEtc->tymed = lpStgMedium->tymed;      // writes the CALLER's FORMATETC
//     AFX_DATACACHE_ENTRY* e = pThis->Lookup(lpFormatEtc, DATADIR_GET);
//     if (e == NULL) return DATA_E_FORMATETC;
//     if (e->m_stgMedium.tymed != TYMED_NULL)       // copy into the caller's medium
//         return _AfxCopyStgMedium(lpFormatEtc->cfFormat, lpStgMedium,
//                                  &e->m_stgMedium) ? S_OK : DATA_E_FORMATETC;
//     return pThis->OnRenderData(lpFormatEtc, lpStgMedium)    // vslot 24, in TRY
//         ? S_OK : DATA_E_FORMATETC;
// (No memset here, unlike GetData: the caller's medium is the destination.)
// Deviations: as for GetData.
// Symbol: ?GetDataHere@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium) {
    COleDataSource* pSource = SourceFromDataObject(pThis);
    if (lpFormatEtc == nullptr || lpStgMedium == nullptr) return E_INVALIDARG;
    lpFormatEtc->tymed = lpStgMedium->tymed;
    DataCacheEntry* pEntry = LookupEntry(pSource, lpFormatEtc, DATADIR_GET);
    if (pEntry == nullptr) return DATA_E_FORMATETC;
    if (pEntry->medium.tymed != TYMED_NULL) {
        return CopyStgMediumLikeRetail(lpFormatEtc->cfFormat, lpStgMedium, &pEntry->medium)
            ? S_OK : DATA_E_FORMATETC;
    }
    return impl__OnRenderData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
               pSource, lpFormatEtc, lpStgMedium)
        ? S_OK : DATA_E_FORMATETC;
}

// COleDataSource::XDataObject::QueryGetData -- RVA 0x253c40 (mfc140u; ordinal
// 11990, resolved from the export table -- the symbol map has no entry),
// transcribed in full:
//     if (lpFormatEtc == NULL) return E_INVALIDARG;
//     return ((COleDataSource*)(this - 0x58))->Lookup(lpFormatEtc, DATADIR_GET)
//         ? S_OK : DATA_E_FORMATETC;
// (No module-state switch in this one.)
// Symbol: ?QueryGetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC___Z(
    void* pThis, FORMATETC* lpFormatEtc) {
    if (lpFormatEtc == nullptr) return E_INVALIDARG;
    return LookupEntry(SourceFromDataObject(pThis), lpFormatEtc, DATADIR_GET) != nullptr
        ? S_OK : DATA_E_FORMATETC;
}

// COleDataSource::XDataObject::SetData -- RVA 0x253c80 (mfc140u; ordinal 13098,
// resolved from the export table -- the symbol map has no entry), transcribed:
//     COleDataSource* pThis = this - 0x58;          // `lea -0x58(%rcx),%rbx`
//     AFX_MANAGE_STATE(pThis->m_pModuleState);      // omitted, header comment
//     if (lpFormatEtc == NULL || lpStgMedium == NULL) return E_INVALIDARG;
//     if (pThis->Lookup(lpFormatEtc, DATADIR_SET) == NULL) return DATA_E_FORMATETC;
//     return pThis->OnSetData(lpFormatEtc, lpStgMedium, bRelease)   // vslot 25, in TRY
//         ? S_OK : E_UNEXPECTED;                                     // 0x8000ffff
// OpenMFC has no DATADIR_SET entries (DelaySetData is a stub), so the lookup
// always fails and DATA_E_FORMATETC is returned -- which is also what retail
// returns for a source that never called DelaySetData. The OnSetData arm is
// kept as transcribed. Deviations otherwise as for GetData.
// Symbol: ?SetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium, int bRelease) {
    COleDataSource* pSource = SourceFromDataObject(pThis);
    if (lpFormatEtc == nullptr || lpStgMedium == nullptr) return E_INVALIDARG;
    if (LookupEntry(pSource, lpFormatEtc, DATADIR_SET) == nullptr) return DATA_E_FORMATETC;
    return impl__OnSetData_COleDataSource__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
               pSource, lpFormatEtc, lpStgMedium, bRelease)
        ? S_OK : E_UNEXPECTED;
}
