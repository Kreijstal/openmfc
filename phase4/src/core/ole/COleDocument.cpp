// COleDocument — OpenMFC implementation.
// Sources: olecore.cpp
//
// Retail layout (mfc140u ??0COleDocument@@QEAA@XZ, RVA 0x253e90, cross-checked
// against the real afxole.h declaration on this host), sizeof == 0x1e8:
//   +0x180 CObList m_docItemList      +0x1b8 UINT m_dwNextItemNumber (=1)
//   +0x1bc BOOL m_bLastVisible (=0)   +0x1c0 BOOL m_bCompoundFile (=0)
//   +0x1c8 LPSTORAGE m_lpRootStg      +0x1d0 BOOL m_bSameAsLoad (=1)
//   +0x1d4 BOOL m_bRemember (=1)      +0x1d8 CString m_strStorageName
//   +0x1e0 DVTARGETDEVICE* m_ptd
// and, inherited from CDocument, m_strTitle@0x40, m_strPathName@0x48,
// m_bAutoDelete@0x120, m_bEmbedded@0x124 and the three handler-mode BOOLs
// m_bGetThumbnailMode@0x128 / m_bPreviewHandlerMode@0x12c / m_bSearchMode@0x130
// that IsSearchAndOrganizeHandler (mfc140u 0x21cdf0) ORs together.
//
// OpenMFC's COleDocument (include/openmfc/afxole.h:980) declares only
// m_bCompoundFile and m_bRemember followed by 96 bytes of opaque padding, and
// its CDocument (afxwin.h:3257) is a simplified layout of its own, so none of
// the retail offsets above are usable here.  The item list is the
// olecore DocumentState side table (COleClientItem* only) plus the
// OleDocExtra::otherItems list below for every other CDocItem kind, and the
// docfile members (m_lpRootStg, m_bSameAsLoad, m_bLastVisible, m_strStorageName,
// m_ptd) live in OleDocExtra.  A POSITION over the item list is (index + 1)
// into the concatenation [client items..., other items...]; retail's is a
// CObList node pointer.

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"
#include <cstdlib>

#if defined(__GNUC__) && !defined(__clang__)
// The IStream calls below must stay true virtual calls.  GCC's -O2 speculative
// devirtualization would otherwise guess CArchiveStream (the one IStream
// implementation visible through detail/OlecoreSupport.h) and pull that
// class's inline bodies -- and their CArchive::Read/Write references -- into
// this unit.
#pragma GCC optimize ("no-devirtualize-speculatively")
#endif

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);
// core/window/CWnd.cpp, core/window/Thunks.cpp
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);
// core/ole/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ();
// core/doc/RuntimeClasses.cpp, core/doc/CDocItem.cpp, core/ole/COleServerItem.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDocItem__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__IsBlank_CDocItem__UEBAHXZ(const CDocItem* pThis);
extern "C" int MS_ABI impl__IsBlank_COleServerItem__MEBAHXZ(const COleServerItem* pThis);
// core/ole/COleClientItem.cpp, core/ole/Thunks.cpp
extern "C" void MS_ABI impl__Release_COleClientItem__UEAAXW4tagOLECLOSE___Z(COleClientItem* pThis, unsigned long dwCloseOption);
extern "C" void MS_ABI impl__CommitItem_COleClientItem__UEAAXH_Z(COleClientItem* pThis, int bSuccess);
extern "C" int MS_ABI impl__SetPrintDevice_COleClientItem__QEAAHPEBUtagDVTARGETDEVICE___Z(COleClientItem* pThis, const tagDVTARGETDEVICE* ptd);
extern "C" int MS_ABI impl__IsModified_COleClientItem__QEBAHXZ(const COleClientItem* pThis);
extern "C" void MS_ABI impl__Deactivate_COleClientItem__QEAAXXZ(COleClientItem* pThis);
extern "C" void MS_ABI impl__Close_COleClientItem__QEAAXW4tagOLECLOSE___Z(COleClientItem* pThis, tagOLECLOSE dwCloseOption);
extern "C" int MS_ABI impl__DoVerb_COleClientItem__UEAAHJPEAVCView__PEAUtagMSG___Z(COleClientItem* pThis, long nVerb, CView* pView, MSG* lpMsg);
// core/ole/Thunks.cpp (the C++ member COleConvertDialog::DoConvert is defined
// in core/ole/COleConvertDialog.cpp but not referenced by the pristine unit,
// and checkfile's link audit rejects any new C++ undefined symbol)
extern "C" int MS_ABI impl__DoConvert_COleConvertDialog__QEAAHPEAVCOleClientItem___Z(COleConvertDialog* pThis, COleClientItem* pItem);
// core/runtime/CCmdTarget.cpp
extern "C" void* MS_ABI impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ();
extern "C" void MS_ABI impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void MS_ABI impl__EndWaitCursor_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
// core/doc/CDocument.cpp
extern "C" int MS_ABI impl__OnCmdMsg_CDocument__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(CDocument* pThis, unsigned int id, int code, void* extra, AFX_CMDHANDLERINFO* info);
extern "C" void MS_ABI impl__OnIdle_CDocument__UEAAXXZ(CDocument* pThis);
extern "C" void MS_ABI impl__OnFileSendMail_CDocument__IEAAXXZ(CDocument* pThis);
extern "C" int MS_ABI impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(const CDocument* pThis);
// detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc);
extern "C" void MS_ABI impl__AfxThrowArchiveException__YAXHPEB_W_Z(int cause, const wchar_t* lpszArchiveName);
extern "C" void MS_ABI impl__AfxThrowFileException__YAXHJPEB_W_Z(int cause, long lOsError, const wchar_t* lpszFileName);
// core/exceptions/CFileException.cpp
extern "C" int MS_ABI impl__OsErrorToException_CFileException__SAHJ_Z(long lOsError);
// core/collections/Globals.cpp, core/collections/CStringT.cpp
extern "C" int MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, unsigned int nType, unsigned int nIDHelp);
extern "C" void MS_ABI impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(CString* rString, unsigned int nIDS, const wchar_t* lpsz1);
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString* pThis, unsigned int nID);
// detail/RegcoreSupport.cpp (AfxGetApp(): in OpenMFC AfxGetThread() cast to
// CWinApp*, standing in for retail's AfxGetModuleState()->m_pCurrentWinApp)
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
// core/runtime/CArchive.cpp, core/runtime/Thunks.cpp
extern "C" void* MS_ABI impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(void* p, CFile* pFile, unsigned int nMode, int nBufSize, void* lpBuf);
extern "C" void MS_ABI impl___1CArchive__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__Close_CArchive__QEAAXXZ(CArchive* pThis);
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* lpBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* lpBuf, unsigned int nMax);
extern "C" CObject* MS_ABI impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z(CArchive* pThis, const CRuntimeClass* pClass);
extern "C" void MS_ABI impl__WriteObject_CArchive__QEAAXPEBVCObject___Z(CArchive* pThis, const CObject* pOb);

// Forward declarations of the exports defined later in this file that the
// bodies below call through their thunks (retail calls them virtually).
extern "C" void* MS_ABI impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(const COleDocument* pThis, void** pPos);
extern "C" void* MS_ABI impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(const COleDocument* pThis, void** pPos, CRuntimeClass* pClass);
extern "C" void MS_ABI impl__RemoveItem_COleDocument__UEAAXPEAVCDocItem___Z(COleDocument* pThis, CDocItem* pItem);
extern "C" void MS_ABI impl__CommitItems_COleDocument__QEAAXHPEAUIStorage___Z(COleDocument* pThis, int bSuccess, IStorage* pNewStorage);
extern "C" int MS_ABI impl__ApplyPrintDevice_COleDocument__QEAAHPEBUtagDVTARGETDEVICE___Z(COleDocument* pThis, const tagDVTARGETDEVICE* ptd);
extern "C" void MS_ABI impl__LoadFromStorage_COleDocument__MEAAXXZ(COleDocument* pThis);
extern "C" void MS_ABI impl__SaveToStorage_COleDocument__MEAAXPEAVCObject___Z(COleDocument* pThis, CObject* pObject);
extern "C" void MS_ABI impl__UpdateModifiedFlag_COleDocument__UEAAXXZ(COleDocument* pThis);
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_COleDocument__UEAAPEAVCOleClientItem__PEAVCWnd___Z(COleDocument* pThis, CWnd* pWnd);

namespace {

// ---------------------------------------------------------------------------
// Retail COleDocument members OpenMFC's header does not declare (see the file
// header for the retail offsets).  One record per live document, created on
// first use and torn down by ~COleDocument.  m_dwNextItemNumber (+0x1b8) is not
// mirrored: nothing in this file reads it.
// (Intrusive lists rather than std::vector so this unit adds no allocator
// symbols beyond the nothrow new / sized delete it already used.)
struct OtherItemNode {
    CDocItem* item;
    OtherItemNode* next;
};
struct OleDocExtra {
    COleDocument* doc = nullptr;
    OtherItemNode* otherItems = nullptr; // m_docItemList entries that are not COleClientItems, in insertion order
    LPSTORAGE lpRootStg = nullptr;       // +0x1c8 m_lpRootStg
    BOOL bLastVisible = FALSE;           // +0x1bc m_bLastVisible
    BOOL bSameAsLoad = TRUE;             // +0x1d0 m_bSameAsLoad
    CString strStorageName;              // +0x1d8 m_strStorageName
    DVTARGETDEVICE* ptd = nullptr;       // +0x1e0 m_ptd
    OleDocExtra* next = nullptr;
};
OleDocExtra* g_oleDocExtras = nullptr;

OleDocExtra* GetExtra(const COleDocument* doc, bool create) {
    if (!doc) return nullptr;
    for (OleDocExtra* e = g_oleDocExtras; e != nullptr; e = e->next) {
        if (e->doc == doc) return e;
    }
    if (!create) return nullptr;
    OleDocExtra* e = new (std::nothrow) OleDocExtra();
    if (!e) return nullptr;
    e->doc = const_cast<COleDocument*>(doc);
    e->next = g_oleDocExtras;
    g_oleDocExtras = e;
    return e;
}
void RemoveExtra(COleDocument* doc) {
    for (OleDocExtra** link = &g_oleDocExtras; *link != nullptr; link = &(*link)->next) {
        OleDocExtra* e = *link;
        if (e->doc != doc) continue;
        *link = e->next;
        while (e->otherItems) {
            OtherItemNode* n = e->otherItems;
            e->otherItems = n->next;
            delete n;
        }
        delete e;
        return;
    }
}
bool OtherItemsContain(const OleDocExtra* extra, const CDocItem* item) {
    for (OtherItemNode* n = extra->otherItems; n != nullptr; n = n->next) {
        if (n->item == item) return true;
    }
    return false;
}
void OtherItemsAppend(OleDocExtra* extra, CDocItem* item) {
    OtherItemNode* node = new (std::nothrow) OtherItemNode{item, nullptr};
    if (!node) return;
    OtherItemNode** link = &extra->otherItems;
    while (*link) link = &(*link)->next;
    *link = node;
}
void OtherItemsRemove(OleDocExtra* extra, const CDocItem* item) {
    for (OtherItemNode** link = &extra->otherItems; *link != nullptr;) {
        OtherItemNode* n = *link;
        if (n->item == item) {
            *link = n->next;
            delete n;
        } else {
            link = &n->next;
        }
    }
}

// The item list as retail's m_docItemList would enumerate it: client items
// first (olecore DocumentState), then everything else added through AddItem.
size_t ItemCount(const COleDocument* doc) {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(doc), false);
    size_t n = state ? state->items.size() : 0;
    OleDocExtra* extra = GetExtra(doc, false);
    if (extra) {
        for (OtherItemNode* node = extra->otherItems; node != nullptr; node = node->next) ++n;
    }
    return n;
}
CDocItem* ItemAt(const COleDocument* doc, size_t index) {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(doc), false);
    size_t nClient = state ? state->items.size() : 0;
    if (index < nClient) return state->items[index];
    OleDocExtra* extra = GetExtra(doc, false);
    if (!extra) return nullptr;
    index -= nClient;
    for (OtherItemNode* node = extra->otherItems; node != nullptr; node = node->next, --index) {
        if (index == 0) return node->item;
    }
    return nullptr;
}

// CObject::IsKindOf as retail evaluates it (a pointer walk up the descriptor
// chain: mfc140u 0x234cf0 -> CRuntimeClass::IsDerivedFrom), with one addition
// that this tree needs: a descriptor also matches when its class NAME matches.
// COleServerItem and COleServerDoc have two CRuntimeClass objects in this DLL
// (the IMPLEMENT_DYNAMIC statics in detail/OlecoreSupport.cpp, which the
// header-compiled GetRuntimeClass returns, and the OLED_DESC globals in
// core/ole/RuntimeClasses.cpp, which the exported GetThisClass thunks return),
// so a pure pointer compare would make GetNextServerItem miss every
// OpenMFC-constructed server item.
bool ClassMatches(const CRuntimeClass* pObjClass, const CRuntimeClass* pClass) {
    if (!pClass) return false;
    for (const CRuntimeClass* p = pObjClass; p != nullptr; p = p->BaseClass()) {
        if (p == pClass) return true;
        if (p->m_lpszClassName && pClass->m_lpszClassName &&
            std::strcmp(p->m_lpszClassName, pClass->m_lpszClassName) == 0) {
            return true;
        }
    }
    return false;
}
bool ItemIsKindOf(const CDocItem* pItem, const CRuntimeClass* pClass) {
    return pItem != nullptr && ClassMatches(pItem->GetRuntimeClass(), pClass);
}
bool IsClientItem(const CDocItem* pItem) {
    return ItemIsKindOf(pItem, impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ());
}

// AfxComparePath as retail defines it (mfc140u 0x228d60): TRUE when the two
// paths compare equal under lstrcmpiW; the extra DBCS lead-byte pass retail
// makes when GetSystemMetrics(SM_DBCSENABLED) is set is not reproduced.
// Written locally because the tree's impl__AfxComparePath__YAHPEB_W0_Z
// (core/collections/Globals.cpp) returns a strcmp-style 0 on equality, the
// inverse of retail's BOOL contract; see headerRequests.
bool SamePath(const wchar_t* a, const wchar_t* b) {
    if (!a) a = L"";
    if (!b) b = L"";
    return ::lstrcmpiW(a, b) == 0;
}

// RELEASE(lpUnk) as retail spells it (helper at mfc140u 0x26ccc4, called with
// the address of the member): Release the interface and clear the slot.
template <class T> void ReleaseAndClear(T*& p) {
    if (p) { p->Release(); p = nullptr; }
}

// _AfxFillOleFileException (mfc140u 0x264f74): map a storage SCODE onto
// CFileException::m_cause / m_lOsError the way COleStreamFile does.
void FillOleFileException(int& cause, long& lOsError, HRESULT sc) {
    if (FAILED(sc) && (sc & 0x1FFF0000) == 0x00030000 /*FACILITY_STORAGE*/ &&
        (sc & 0xFFFF) < 0x100) {
        // STG_E_* codes that wrap a DOS error: reuse CFileException's mapping.
        lOsError = sc & 0xFFFF;
        cause = impl__OsErrorToException_CFileException__SAHJ_Z(lOsError);
        return;
    }
    lOsError = sc;
    if (sc == static_cast<HRESULT>(0x80030100) /*STG_E_INUSE*/ ||
        (static_cast<unsigned long>(sc) - 0x80030101UL) <= 4 /*STG_E_NOTCURRENT..STG_E_OLDDLL*/ ||
        sc == static_cast<HRESULT>(0x80030106) /*STG_E_SHAREREQUIRED*/) {
        cause = CFileException::sharingViolation;
    } else {
        cause = CFileException::genericException;
    }
}
// Retail's two-step "open Contents or throw" (SaveToStorage/LoadFromStorage):
// CFileException::fileNotFound becomes CArchiveException::badSchema, anything
// else is rethrown as the CFileException COleStreamFile filled in.
[[noreturn]] void ThrowContentsStreamError(HRESULT sc) {
    int cause = CFileException::none;
    long lOsError = -1;
    FillOleFileException(cause, lOsError, sc);
    if (cause == CFileException::fileNotFound) {
        impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::badSchema, nullptr);
    }
    impl__AfxThrowFileException__YAXHJPEB_W_Z(cause, lOsError, nullptr);
    std::abort();   // not reached: both thunks throw
}

// ---------------------------------------------------------------------------
// A CFile stand-in over an IStream with an MSVC-slot-layout vtable.
//
// Retail drives its "Contents" stream through a stack COleStreamFile and a
// CArchive.  OpenMFC's CArchive (core/runtime/CArchive.cpp) reaches its CFile
// through the fixed MSVC slot indices in core/file/FileDispatch.cpp (16 Read,
// 17 Write, 21 Flush, ...).  A COleStreamFile built by its exported ctor carries
// a gcc-layout vtable, which those indices mis-dispatch on, so this object is
// used instead: its vtable is laid out in the MSVC CFile order and the slots
// CArchive touches forward to the IStream.  The remaining slots are inert.
// The forwarding slots follow retail's COleStreamFile bodies (mfc140u
// GetLength 0x2655a0, Close 0x265760, Flush 0x265730; Read/Write read in
// mfc140 at 0x2643b0/0x264400, bodies identical): every IStream failure goes
// through _AfxThrowOleFileException (mfc140u 0x264fe4), which throws the
// mapped CFileException only when FAILED(sc) -- S_FALSE returns normally.
struct StreamCFile {
    void* const* vptr;
    IStream* stream;
};
// _AfxThrowOleFileException (mfc140u 0x264fe4): `test ecx,ecx; jns ret`, then
// a stack CFileException filled by 0x264f74 and AfxThrowFileException.
void ThrowOleFileException(HRESULT sc) {
    if (!FAILED(sc)) return;
    int cause; long os;
    FillOleFileException(cause, os, sc);
    impl__AfxThrowFileException__YAXHJPEB_W_Z(cause, os, nullptr);
}
void* MS_ABI sf_Inert(StreamCFile*) { return nullptr; }
unsigned long long MS_ABI sf_GetPosition(StreamCFile* p) {
    if (!p->stream) return 0;
    LARGE_INTEGER move; move.QuadPart = 0;
    ULARGE_INTEGER pos; pos.QuadPart = 0;
    ThrowOleFileException(p->stream->Seek(move, STREAM_SEEK_CUR, &pos));
    return pos.QuadPart;
}
unsigned long long MS_ABI sf_Seek(StreamCFile* p, long long off, unsigned int from) {
    if (!p->stream) return 0;
    LARGE_INTEGER move; move.QuadPart = off;
    ULARGE_INTEGER pos; pos.QuadPart = 0;
    ThrowOleFileException(p->stream->Seek(move, from, &pos));
    return pos.QuadPart;
}
void MS_ABI sf_SetLength(StreamCFile* p, unsigned long long n) {
    if (!p->stream) return;
    ULARGE_INTEGER size; size.QuadPart = n;
    ThrowOleFileException(p->stream->SetSize(size));
}
// COleStreamFile::GetLength (0x2655a0): Stat(STATFLAG_NONAME), throw on
// failure, return cbSize.
unsigned long long MS_ABI sf_GetLength(StreamCFile* p) {
    if (!p->stream) return 0;
    STATSTG stat;
    std::memset(&stat, 0, sizeof(stat));
    ThrowOleFileException(p->stream->Stat(&stat, STATFLAG_NONAME));
    return stat.cbSize.QuadPart;
}
// COleStreamFile::Read (mfc140 0x2643b0; inlined at mfc140u 0x2553f8 inside
// OnLoadDocumentFromStream (0x2552e0)): nCount == 0 returns 0; a NULL stream
// or buffer is E_INVALIDARG; the IStream result goes through
// _AfxThrowOleFileException; the byte count read comes back.
unsigned int MS_ABI sf_Read(StreamCFile* p, void* buf, unsigned int n) {
    if (n == 0) return 0;
    ULONG cbRead = 0;
    if (!p->stream || !buf) {
        ThrowOleFileException(E_INVALIDARG);
    } else {
        ThrowOleFileException(p->stream->Read(buf, n, &cbRead));
    }
    return cbRead;
}
// COleStreamFile::Write (mfc140 0x264400): nCount == 0 returns; a NULL buffer
// is ENSURE'd (AfxThrowInvalidArgException); retail then dereferences
// m_lpStream unconditionally -- a NULL stream is E_INVALIDARG here.
void MS_ABI sf_Write(StreamCFile* p, const void* buf, unsigned int n) {
    if (n == 0) return;
    if (!buf) impl__AfxThrowInvalidArgException__YAXXZ();
    if (!p->stream) ThrowOleFileException(E_INVALIDARG);
    ULONG cbWritten = 0;
    ThrowOleFileException(p->stream->Write(buf, n, &cbWritten));
}
// COleStreamFile::Flush (0x265730) is Commit(STGC_DEFAULT) with the throw on
// failure.  This slot stays lenient because OpenMFC's CArchive::Flush (and
// therefore its Close and, without bNoFlushOnDelete, its destructor) calls the
// CFile's Flush, which retail's CArchive never does; a throw from the
// ScopedArchive destructor would be fatal.  sf_Close makes the retail check.
void MS_ABI sf_Flush(StreamCFile* p) {
    if (p->stream) p->stream->Commit(STGC_DEFAULT);
}
// COleStreamFile::Close (0x265760): if m_lpStream, Flush() (virtual, slot 21)
// then RELEASE(m_lpStream); m_strStorageName.Empty().  Retail's Flush throws
// before the Release, leaving the stream referenced; here the release comes
// first so a failed Commit does not leak it, then the same throw is made.
void MS_ABI sf_Close(StreamCFile* p) {
    if (!p->stream) return;
    HRESULT sc = p->stream->Commit(STGC_DEFAULT);
    ReleaseAndClear(p->stream);
    ThrowOleFileException(sc);
}
// MSVC CFile slot order (core/file/CFileVtable.cpp): 0 GetRuntimeClass 1 dtor
// 2 Serialize 3 AssertValid 4 Dump 5 GetPosition 6 GetFileName 7 GetFileTitle
// 8 GetFilePath 9 SetFilePath 10/11 Open 12 Duplicate 13 Seek 14 SetLength
// 15 GetLength 16 Read 17 Write 18 LockRange 19 UnlockRange 20 Abort 21 Flush
// 22 Close 23 GetBufferPtr.
void* const g_streamCFileVtbl[24] = {
    (void*)sf_Inert, (void*)sf_Inert, (void*)sf_Inert, (void*)sf_Inert,
    (void*)sf_Inert, (void*)sf_GetPosition, (void*)sf_Inert, (void*)sf_Inert,
    (void*)sf_Inert, (void*)sf_Inert, (void*)sf_Inert, (void*)sf_Inert,
    (void*)sf_Inert, (void*)sf_Seek, (void*)sf_SetLength, (void*)sf_GetLength,
    (void*)sf_Read, (void*)sf_Write, (void*)sf_Inert, (void*)sf_Inert,
    (void*)sf_Inert, (void*)sf_Flush, (void*)sf_Close, (void*)sf_Inert,
};
inline CFile* AsCFile(StreamCFile& f) { return reinterpret_cast<CFile*>(&f); }

// A CArchive built in place through its exported ctor/dtor thunks, since the
// C++ ctor is not linkable from this unit (same pattern as
// featurepack/toolbar/CMFCToolBar.cpp SaveState).
struct ScopedArchive {
    alignas(16) unsigned char storage[sizeof(CArchive)];
    CArchive* ar;
    ScopedArchive(CFile* pFile, unsigned int nMode)
        : ar(static_cast<CArchive*>(impl___0CArchive__QEAA_PEAVCFile__IHPEAX_Z(storage, pFile, nMode, 4096, nullptr))) {}
    ~ScopedArchive() { impl___1CArchive__QEAA_XZ(ar); }
    ScopedArchive(const ScopedArchive&) = delete;
    ScopedArchive& operator=(const ScopedArchive&) = delete;
};

// CDocItem::IsBlank is virtual in retail (CDocItem vtable slot 22).  OpenMFC's
// header does not declare it, so the two retail bodies are chosen by kind:
// COleServerItem::IsBlank (TRUE) for server items, CDocItem::IsBlank for the
// rest (COleClientItem does not override it in retail).
int ItemIsBlank(const CDocItem* pItem) {
    if (ItemIsKindOf(pItem, impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ())) {
        return impl__IsBlank_COleServerItem__MEBAHXZ(static_cast<const COleServerItem*>(pItem));
    }
    return impl__IsBlank_CDocItem__UEBAHXZ(pItem);
}

// _AfxOleCopyTargetDevice (mfc140u 0x260648): CoTaskMemAlloc(tdSize) + memcpy.
DVTARGETDEVICE* OleCopyTargetDevice(const DVTARGETDEVICE* ptd) {
    if (!ptd) return nullptr;
    DVTARGETDEVICE* ptdNew = static_cast<DVTARGETDEVICE*>(::CoTaskMemAlloc(ptd->tdSize));
    if (!ptdNew) return nullptr;
    if (ptd->tdSize) std::memcpy(ptdNew, ptd, ptd->tdSize);
    return ptdNew;
}
// _AfxOleCreateTargetDevice(LPDEVNAMES, LPDEVMODEW) (mfc140u 0x26031c): build a
// DVTARGETDEVICE holding the driver/device/port names and the DEVMODE.
// Retail's wcscpy_s result handling (the three `cmp $0xc / cmp $0x50` pairs):
// 0 and STRUNCATE (0x50) continue, ENOMEM (0xc) is AfxThrowMemoryException
// (0x2276c0), anything else AfxThrowInvalidArgException (0x227720).
void CheckWcscpyResult(int err) {
    if (err == 0 || err == 0x50 /*STRUNCATE*/) return;
    if (err == 0xc /*ENOMEM*/) impl__AfxThrowMemoryException__YAXXZ();
    impl__AfxThrowInvalidArgException__YAXXZ();
}
DVTARGETDEVICE* OleCreateTargetDevice(const DEVNAMES* lpDevNames, const DEVMODEW* lpDevMode) {
    const wchar_t* base = reinterpret_cast<const wchar_t*>(lpDevNames);
    const wchar_t* lpszDriver = lpDevNames->wDriverOffset ? base + lpDevNames->wDriverOffset : nullptr;
    const wchar_t* lpszDevice = lpDevNames->wDeviceOffset ? base + lpDevNames->wDeviceOffset : nullptr;
    const wchar_t* lpszPort   = lpDevNames->wOutputOffset ? base + lpDevNames->wOutputOffset : nullptr;
    if (!lpszDriver || !lpszDevice || !lpszPort) impl__AfxThrowInvalidArgException__YAXXZ();
    if (!lpDevMode) impl__AfxThrowInvalidArgException__YAXXZ();
    const unsigned int cbDriver = static_cast<unsigned int>((std::wcslen(lpszDriver) + 1) * sizeof(wchar_t));
    const unsigned int cbDevice = static_cast<unsigned int>((std::wcslen(lpszDevice) + 1) * sizeof(wchar_t));
    const unsigned int cbPort   = static_cast<unsigned int>((std::wcslen(lpszPort) + 1) * sizeof(wchar_t));
    const unsigned int cbTotal  = 0x10 /*sizeof(DVTARGETDEVICE) header*/ + lpDevMode->dmSize +
                                  lpDevMode->dmDriverExtra + cbDriver + cbDevice + cbPort;
    DVTARGETDEVICE* ptd = static_cast<DVTARGETDEVICE*>(::CoTaskMemAlloc(cbTotal));
    if (!ptd) return nullptr;
    ptd->tdSize = cbTotal;
    ptd->tdDriverNameOffset = 0x10;
    CheckWcscpyResult(::wcscpy_s(reinterpret_cast<wchar_t*>(reinterpret_cast<BYTE*>(ptd) + ptd->tdDriverNameOffset),
                                 cbDriver / sizeof(wchar_t), lpszDriver));
    ptd->tdDeviceNameOffset = static_cast<WORD>(ptd->tdDriverNameOffset + cbDriver);
    CheckWcscpyResult(::wcscpy_s(reinterpret_cast<wchar_t*>(reinterpret_cast<BYTE*>(ptd) + ptd->tdDeviceNameOffset),
                                 cbDevice / sizeof(wchar_t), lpszDevice));
    ptd->tdPortNameOffset = static_cast<WORD>(ptd->tdDeviceNameOffset + cbDevice);
    CheckWcscpyResult(::wcscpy_s(reinterpret_cast<wchar_t*>(reinterpret_cast<BYTE*>(ptd) + ptd->tdPortNameOffset),
                                 cbPort / sizeof(wchar_t), lpszPort));
    ptd->tdExtDevmodeOffset = static_cast<WORD>(ptd->tdPortNameOffset + cbPort);
    // Retail copies sizeof(DEVMODEW) + dmDriverExtra bytes through memcpy_s
    // and treats a destination too short for that as an invalid argument.
    const unsigned int cbDevMode = sizeof(DEVMODEW) + lpDevMode->dmDriverExtra;
    if (cbTotal - ptd->tdExtDevmodeOffset < cbDevMode) impl__AfxThrowInvalidArgException__YAXXZ();
    std::memcpy(reinterpret_cast<BYTE*>(ptd) + ptd->tdExtDevmodeOffset, lpDevMode, cbDevMode);
    return ptd;
}
// _AfxOleCreateTargetDevice(const PRINTDLGW*) (mfc140u 0x2605d8): lock the
// PRINTDLG's hDevNames/hDevMode and build from them.
DVTARGETDEVICE* OleCreateTargetDevice(const PRINTDLGW* ppd) {
    const DEVNAMES* lpDevNames = static_cast<const DEVNAMES*>(::GlobalLock(ppd->hDevNames));
    if (!lpDevNames) return nullptr;
    const DEVMODEW* lpDevMode = static_cast<const DEVMODEW*>(::GlobalLock(ppd->hDevMode));
    if (!lpDevMode) {
        ::GlobalUnlock(ppd->hDevNames);
        return nullptr;
    }
    DVTARGETDEVICE* ptd = OleCreateTargetDevice(lpDevNames, lpDevMode);
    ::GlobalUnlock(ppd->hDevNames);
    ::GlobalUnlock(ppd->hDevMode);
    return ptd;
}

// Resource ids from the real afxres.h (OpenMFC's headers do not define them).
const unsigned int kAFX_IDS_UNTITLED        = 0xF003;
const unsigned int kAFX_IDP_ASK_TO_DISCARD  = 0xF18E;
const unsigned int kID_OLE_VERB_FIRST       = 0xE210;
const unsigned int kID_OLE_VERB_LAST        = 0xE21F;
const unsigned int kID_OLE_VERB_POPUP       = 0xE220;

} // namespace

// Symbol: ?OnUpdateEditChangeIcon@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditChangeIcon_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateEditLinksMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateEditLinksMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateObjectVerbMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateObjectVerbMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdateObjectVerbPopup@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdateObjectVerbPopup_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdatePasteLinkMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePasteLinkMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ?OnUpdatePasteMenu@COleDocument@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePasteMenu_COleDocument__IEAAXPEAVCCmdUI___Z(COleDocument*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
COleDocument::COleDocument()
    : CDocument(), m_bCompoundFile(FALSE), m_bRemember(TRUE) {
    memset(_coledocument_padding, 0, sizeof(_coledocument_padding));
    // Retail (mfc140u 0x253e90) also zeroes m_lpRootStg/m_ptd/m_bLastVisible
    // and sets m_bSameAsLoad = TRUE; those live in the side table here, with
    // the same initial values.  Retail's AfxOleLockApp() call is not made.
    GetExtra(this, true);
}
COleDocument::~COleDocument() {
    DocumentState* state = GetDocumentState(this, false);
    if (state) {
        for (COleClientItem* item : state->items) {
            if (item && item->m_pContainerDoc == this) item->m_pContainerDoc = nullptr;
            if (item && item->m_pDocument == this) item->m_pDocument = nullptr;
        }
    }
    // Retail (mfc140u 0x253fd0) deletes every item still in m_docItemList,
    // then RELEASE(m_lpRootStg), CoTaskMemFree(m_ptd) and AfxOleUnlockApp().
    // The items are only detached here (pre-existing OpenMFC behaviour); the
    // storage and target device are released as retail does.
    OleDocExtra* extra = GetExtra(this, false);
    if (extra) {
        for (OtherItemNode* node = extra->otherItems; node != nullptr; node = node->next) {
            if (node->item && node->item->m_pDocument == this) node->item->m_pDocument = nullptr;
        }
        ReleaseAndClear(extra->lpRootStg);
        if (extra->ptd) { ::CoTaskMemFree(extra->ptd); extra->ptd = nullptr; }
    }
    RemoveExtra(this);
    RemoveDocumentState(this);
}
void COleDocument::AddItem(COleClientItem* pItem) {
    if (pItem) AddDocumentItem(this, pItem);
}
void COleDocument::RemoveItem(COleClientItem* pItem) {
    RemoveDocumentItem(this, pItem);
}
// Retail GetStartPosition (mfc140u 0x254110) is `return m_docItemList.m_pNodeHead`.
// Here a POSITION is (index + 1) into the client-items-then-other-items list.
COleClientItem* COleDocument::GetStartPosition() const {
    if (ItemCount(this) == 0) return nullptr;
    return reinterpret_cast<COleClientItem*>(static_cast<uintptr_t>(1));
}
// Retail GetNextClientItem (mfc140u 0x254180) is a tail-jump into
// GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem)).
COleClientItem* COleDocument::GetNextClientItem(POSITION& pos) const {
    return static_cast<COleClientItem*>(
        impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
            this, &pos, impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ()));
}
COleClientItem* COleDocument::GetPrimarySelectedItem(CView* pView) const {
    (void)pView;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}
int COleDocument::GetItemCount() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    return state ? static_cast<int>(state->items.size()) : 0;
}
void COleDocument::EnableCompoundFile(BOOL bEnable) {
    m_bCompoundFile = bEnable;
}
BOOL COleDocument::HasBlankItems() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->GetItemState() == OLE_EMPTY) return TRUE;
    }
    return FALSE;
}
BOOL COleDocument::IsInPlaceActive() const {
    DocumentState* state = GetDocumentState(const_cast<COleDocument*>(this), false);
    if (!state) return FALSE;
    for (COleClientItem* item : state->items) {
        if (item && item->IsInPlaceActive()) return TRUE;
    }
    return FALSE;
}
void COleDocument::OnShowViews(BOOL bVisible) {
    POSITION pos = GetFirstViewPosition();
    while (pos) {
        CWnd* pView = static_cast<CWnd*>(GetNextView(pos));
        if (pView && pView->GetSafeHwnd()) {
            ::ShowWindow(pView->GetSafeHwnd(), bVisible ? SW_SHOW : SW_HIDE);
        }
    }
}
COleClientItem* COleDocument::OnGetLinkedItem(const wchar_t* lpszItemName) {
    DocumentState* state = GetDocumentState(this, false);
    if (!state || state->items.empty()) return nullptr;
    if (!lpszItemName || !*lpszItemName) return state->items.front();

    size_t index = ParseLinkedItemIndex(lpszItemName);
    if (index != 0 && index <= state->items.size()) {
        return state->items[index - 1];
    }

    for (COleClientItem* item : state->items) {
        if (!item) continue;
        CLSID clsid = CLSID_NULL;
        item->GetClassID(&clsid);
        LPOLEOBJECT object = item->GetObject();
        if (object) {
            LPOLESTR userType = nullptr;
            if (SUCCEEDED(object->GetUserType(USERCLASSTYPE_FULL, &userType)) && userType) {
                bool match = wcscmp(userType, lpszItemName) == 0;
                CoTaskMemFree(userType);
                if (match) return item;
            }
        }
    }
    return state->items.size() == 1 ? state->items.front() : nullptr;
}
COleClientItem* COleDocument::OnGetEmbeddedItem() {
    return GetPrimarySelectedItem(nullptr);
}
void COleDocument::OnEditChangeIcon(COleClientItem* pItem) {
    COleClientItem* target = pItem ? pItem : OnGetEmbeddedItem();
    if (!target) return;

    COleChangeIconDialog dlg(target);
    if (dlg.DoChangeIcon(target) == TRUE) {
        SetModifiedFlag(TRUE);
    }
}
void COleDocument::OnEditConvert(COleClientItem* pItem) {
    COleClientItem* target = pItem ? pItem : OnGetEmbeddedItem();
    if (!target) return;

    CLSID oldClass = {};
    target->GetClassID(&oldClass);
    COleConvertDialog dlg(target);
    if (dlg.DoModal() != IDOK) return;

    OLEUICONVERTW* cv = dlg.GetOleUIConvert();
    if (!cv) return;

    CLSID newClass = cv->clsidNew;
    if (IsEqualGUID(newClass, CLSID_NULL) || IsEqualGUID(oldClass, newClass)) {
        return;
    }

    if (dlg.IsConvertTo()) {
        if (target->ConvertTo(newClass)) {
            SetModifiedFlag(TRUE);
            target->SetModifiedFlag(TRUE);
        }
    } else {
        if (target->ActivateAs(newClass, oldClass)) {
            SetModifiedFlag(TRUE);
            target->SetModifiedFlag(TRUE);
        }
    }
}
void COleDocument::OnEditLinks() {
    COleLinksDialog dlg(this);
    if (dlg.DoModal() == IDOK) {
        SetModifiedFlag(TRUE);
    }
}
LPOLEITEMCONTAINER COleDocument::GetItemContainer() {
    return new(std::nothrow) OleItemContainerAdapter(this);
}

// =============================================================================
// Exports decoded from the retail image.  Unless a comment names mfc140 (the
// ANSI twin), every RVA below is an mfc140u address; the bodies are identical
// between the two images.  Retail reaches its own virtuals through the
// COleDocument vtable (slot numbers from the harvested
// artifacts_dl/harvest/vtable_slots_docview_batch2.json); here those become
// C++ virtual calls through OpenMFC's header, which is what the surrounding
// doc/view code does too.  Callee RVAs the mfc140u map does not name (CDocument
// bases and helpers the linker folded) are marked "unnamed": they were
// identified from their bodies and from the mfc140 map's name at the same
// call site.
//
// Exceptions: this DLL raises every AfxThrow* through the MSVC runtime's
// _CxxThrowException (detail/MfcExceptionsSupport.cpp).  Verified 2026-09-19
// with an MSVC client under wine calling into a gcc DLL: such an exception
// unwinds straight through gcc frames -- a gcc catch(...) never sees it and
// gcc destructors / scope guards in the frames it crosses do NOT run.  So
// none of retail's TRY/CATCH_ALL blocks below are reproduced, and the RAII
// objects used here (ScopedArchive, ResetFlags) only act on the normal path.
// =============================================================================

// COleDocument::AddItem(CDocItem*) -- mfc140u 0x2540a0:
//     m_docItemList.AddTail(pItem);          // CObList::AddTail, 0x231e70
//     pItem->m_pDocument = this;             // +0x40
// A COleClientItem goes into the olecore DocumentState list (which also sets
// the OpenMFC-only m_pContainerDoc back pointer); any other CDocItem kind goes
// into OleDocExtra::otherItems.  Both are append-if-absent, so an item that
// registered itself through its constructor is not listed twice.
// Symbol: ?AddItem@COleDocument@@UEAAXPEAVCDocItem@@@Z
extern "C" void MS_ABI impl__AddItem_COleDocument__UEAAXPEAVCDocItem___Z(COleDocument* pThis, CDocItem* pItem) {
    if (!pThis || !pItem) return;
    if (IsClientItem(pItem)) {
        AddDocumentItem(pThis, static_cast<COleClientItem*>(pItem));
    } else {
        OleDocExtra* extra = GetExtra(pThis, true);
        if (extra && !OtherItemsContain(extra, pItem)) OtherItemsAppend(extra, pItem);
    }
    pItem->m_pDocument = pThis;
}

// COleDocument::RemoveItem(CDocItem*) -- mfc140u 0x2540d0:
//     POSITION pos = m_docItemList.Find(pItem)   (open-coded head walk, +0x188)
//     m_docItemList.RemoveAt(pos);               // 0x2320d0 (CObList::RemoveAt, unnamed)
//     pItem->m_pDocument = NULL;
// CObList::RemoveAt opens with ENSURE(pos != NULL) (`test rdx,rdx; je` ->
// 0x227720 AfxThrowInvalidArgException), so retail throws
// CInvalidArgException for an item that is not in the list (a NULL pItem
// included).  Deviation: this body ignores an unlisted item instead, because
// the exported COleClientItem ctor (core/ole/COleClientItem.cpp) registers a
// separate heap object rather than the caller's, so an MSVC client's own item
// is never in these lists and retail's throw would fire where retail itself
// would find the item.
// Symbol: ?RemoveItem@COleDocument@@UEAAXPEAVCDocItem@@@Z
extern "C" void MS_ABI impl__RemoveItem_COleDocument__UEAAXPEAVCDocItem___Z(COleDocument* pThis, CDocItem* pItem) {
    if (!pThis || !pItem) return;
    if (IsClientItem(pItem)) {
        RemoveDocumentItem(pThis, static_cast<COleClientItem*>(pItem));
    } else {
        OleDocExtra* extra = GetExtra(pThis, false);
        if (extra) OtherItemsRemove(extra, pItem);
    }
    if (pItem->m_pDocument == pThis) pItem->m_pDocument = nullptr;
}

// COleDocument::GetNextItem(POSITION&) -- mfc140u 0x21c740 (the body is
// identical-code-folded with CDocument::GetNextView):
//     CNode* node = pos; if (!node) return NULL;
//     pos = node->pNext; return (CDocItem*)node->data;
// Retail dereferences whatever pos holds; here an out-of-range index is
// treated as end-of-list.
// Symbol: ?GetNextItem@COleDocument@@UEBAPEAVCDocItem@@AEAPEAU__POSITION@@@Z
extern "C" void* MS_ABI impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(const COleDocument* pThis, void** pPos) {
    if (!pPos) return nullptr;
    uintptr_t index = reinterpret_cast<uintptr_t>(*pPos);
    if (!pThis || index == 0 || index > ItemCount(pThis)) {
        *pPos = nullptr;
        return nullptr;
    }
    CDocItem* item = ItemAt(pThis, index - 1);
    *pPos = (index < ItemCount(pThis)) ? reinterpret_cast<void*>(index + 1) : nullptr;
    return item;
}

// COleDocument::GetNextItemOfKind(POSITION&, CRuntimeClass*) -- mfc140u 0x254120:
//     if (pos == NULL) return NULL;
//     do { CDocItem* pItem = GetNextItem(pos);          // vtable slot 72
//          if (pItem->IsKindOf(pClass)) return pItem;   // 0x234cf0
//     } while (pos != NULL);
//     return NULL;
// Symbol: ?GetNextItemOfKind@COleDocument@@IEBAPEAVCDocItem@@AEAPEAU__POSITION@@PEAUCRuntimeClass@@@Z
extern "C" void* MS_ABI impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(const COleDocument* pThis, void** pPos, CRuntimeClass* pClass) {
    if (!pThis || !pPos || *pPos == nullptr) return nullptr;
    do {
        CDocItem* pItem = static_cast<CDocItem*>(
            impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(pThis, pPos));
        if (pItem && ItemIsKindOf(pItem, pClass)) return pItem;
    } while (*pPos != nullptr);
    return nullptr;
}

// COleDocument::GetNextServerItem(POSITION&) -- mfc140u 0x254190: a tail-jump
// into GetNextItemOfKind(pos, RUNTIME_CLASS(COleServerItem)).
// Symbol: ?GetNextServerItem@COleDocument@@QEBAPEAVCOleServerItem@@AEAPEAU__POSITION@@@Z
extern "C" void* MS_ABI impl__GetNextServerItem_COleDocument__QEBAPEAVCOleServerItem__AEAPEAU__POSITION___Z(const COleDocument* pThis, void** pPos) {
    return impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
        pThis, pPos, impl__GetThisClass_COleServerItem__SAPEAUCRuntimeClass__XZ());
}

// COleDocument::OnEditChangeIcon() -- mfc140u 0x26cad0 (mfc140 0x26b890):
//     CView* pView = AfxGetThreadState()->m_pRoutingView;        // +0x1a8
//     COleClientItem* pItem = GetPrimarySelectedItem(pView);     // vtable slot 76
//     if (pItem != NULL) {
//         COleChangeIconDialog dlg(pItem, CIF_SELECTCURRENT /*2*/, NULL);  // 0x24db10
//         if (dlg.DoModal() == IDOK) dlg.DoChangeIcon(pItem);    // 0x24dcc0
//     }
// Deviations: the routing view is read through CCmdTarget::GetRoutingView_
// (core/runtime/CCmdTarget.cpp), which is the same thread-state member and is
// currently a NULL stub there; GetPrimarySelectedItem is not virtual in
// OpenMFC's header, so this file's own definition is called.  Retail's
// DoChangeIcon (0x24dcc0) does not run the dialog itself, but OpenMFC's
// (core/ole/COleChangeIconDialog.cpp) does -- it is `if (DoModal() != IDOK)
// return FALSE; apply` -- so calling DoModal here as well would show the
// dialog twice.  DoChangeIcon alone reproduces retail's observable sequence
// (one dialog, apply on IDOK).
// Symbol: ?OnEditChangeIcon@COleDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditChangeIcon_COleDocument__IEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    CView* pView = static_cast<CView*>(impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ());
    COleClientItem* pItem = pThis->GetPrimarySelectedItem(pView);
    if (pItem == nullptr) return;
    COleChangeIconDialog dlg(pItem, 2 /*CIF_SELECTCURRENT*/, nullptr);
    dlg.DoChangeIcon(pItem);   // DoModal() + apply on IDOK (see above)
}

// COleDocument::OnEditConvert() -- mfc140u 0x26c9b0 (mfc140 0x26b770):
//     CView* pView = AfxGetThreadState()->m_pRoutingView;
//     COleClientItem* pItem = GetPrimarySelectedItem(pView);     // vtable slot 76
//     if (pItem != NULL) {
//         COleConvertDialog dlg(pItem, CF_SELECTCONVERTTO /*8*/, NULL, NULL);  // 0x24d6d0
//         if (dlg.DoModal() == IDOK) dlg.DoConvert(pItem);       // 0x24d980
//     }
// Same deviations as OnEditChangeIcon: OpenMFC's DoConvert
// (core/ole/COleConvertDialog.cpp) runs DoModal itself and applies only on
// IDOK, where retail's DoConvert (0x24d980) does not run the dialog, so it is
// called on its own (through its thunk: the C++ member is not referenced by
// the pristine unit and the link audit rejects a new C++ undefined symbol).
// Symbol: ?OnEditConvert@COleDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditConvert_COleDocument__IEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    CView* pView = static_cast<CView*>(impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ());
    COleClientItem* pItem = pThis->GetPrimarySelectedItem(pView);
    if (pItem == nullptr) return;
    COleConvertDialog dlg(pItem, 8 /*CF_SELECTCONVERTTO*/, nullptr, nullptr);
    impl__DoConvert_COleConvertDialog__QEAAHPEAVCOleClientItem___Z(&dlg, pItem);   // DoModal() + apply on IDOK
}

// COleDocument::ApplyPrintDevice(const DVTARGETDEVICE*) -- mfc140u 0x2556b0:
//     DVTARGETDEVICE* ptdNew = NULL;
//     if (ptd != NULL && (ptdNew = _AfxOleCopyTargetDevice(ptd)) == NULL)  // 0x260648
//         return FALSE;
//     CoTaskMemFree(m_ptd); m_ptd = ptdNew;                      // +0x1e0
//     POSITION pos = GetStartPosition();                         // vtable slot 71
//     COleClientItem* pItem;
//     while ((pItem = GetNextClientItem(pos)) != NULL)           // 0x254120 + RUNTIME_CLASS(COleClientItem)
//         pItem->SetPrintDevice(ptdNew);                         // 0x24acd0 (unnamed)
//     return TRUE;
// Symbol: ?ApplyPrintDevice@COleDocument@@QEAAHPEBUtagDVTARGETDEVICE@@@Z
extern "C" int MS_ABI impl__ApplyPrintDevice_COleDocument__QEAAHPEBUtagDVTARGETDEVICE___Z(COleDocument* pThis, const tagDVTARGETDEVICE* ptd) {
    if (!pThis) return FALSE;
    DVTARGETDEVICE* ptdNew = nullptr;
    if (ptd != nullptr) {
        ptdNew = OleCopyTargetDevice(ptd);
        if (ptdNew == nullptr) return FALSE;
    }
    OleDocExtra* extra = GetExtra(pThis, true);
    if (extra) {
        ::CoTaskMemFree(extra->ptd);
        extra->ptd = ptdNew;
    }
    POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
    COleClientItem* pItem;
    while ((pItem = pThis->GetNextClientItem(pos)) != nullptr) {
        impl__SetPrintDevice_COleClientItem__QEAAHPEBUtagDVTARGETDEVICE___Z(pItem, ptdNew);
    }
    return TRUE;
}

// COleDocument::ApplyPrintDevice(const PRINTDLGW*) -- mfc140u 0x255740:
//     DVTARGETDEVICE* ptd = ppd ? _AfxOleCreateTargetDevice(ppd) : NULL;  // 0x2605d8
//     BOOL bResult = ApplyPrintDevice(ptd);                      // 0x2556b0
//     CoTaskMemFree(ptd);
//     return bResult;
// Symbol: ?ApplyPrintDevice@COleDocument@@QEAAHPEBUtagPDW@@@Z
extern "C" int MS_ABI impl__ApplyPrintDevice_COleDocument__QEAAHPEBUtagPDW___Z(COleDocument* pThis, const tagPDW* ppd) {
    DVTARGETDEVICE* ptd = ppd ? OleCreateTargetDevice(ppd) : nullptr;
    int bResult = impl__ApplyPrintDevice_COleDocument__QEAAHPEBUtagDVTARGETDEVICE___Z(pThis, ptd);
    ::CoTaskMemFree(ptd);
    return bResult;
}

// COleDocument::CommitItems(BOOL, LPSTORAGE) -- mfc140u 0x254510:
//     POSITION pos = GetStartPosition();                         // vtable slot 71
//     COleClientItem* pItem;
//     while ((pItem = GetNextClientItem(pos)) != NULL) {
//         if (pItem->[+0x7f] /*BYTE flag*/ && pNewStorage != NULL) {
//             RELEASE(pItem->[+0x88]);                           // an IUnknown* slot
//             pNewStorage->AddRef(); pItem->[+0x88] = pNewStorage;
//         }
//         pItem->CommitItem(bSuccess);                           // item vtable slot 57
//     }
// OpenMFC's COleClientItem declares neither the +0x7f flag nor the +0x88
// interface (see the offsets table in core/ole/COleClientItem.cpp), so the
// storage hand-over is not reproduced and pNewStorage is unused; OpenMFC's
// header does not declare CommitItem at all, so its thunk is called directly.
// Symbol: ?CommitItems@COleDocument@@QEAAXHPEAUIStorage@@@Z
extern "C" void MS_ABI impl__CommitItems_COleDocument__QEAAXHPEAUIStorage___Z(COleDocument* pThis, int bSuccess, IStorage* pNewStorage) {
    (void)pNewStorage;
    if (!pThis) return;
    POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
    COleClientItem* pItem;
    while ((pItem = pThis->GetNextClientItem(pos)) != nullptr) {
        impl__CommitItem_COleClientItem__UEAAXH_Z(pItem, bSuccess);
    }
}

// COleDocument::DeleteContents() -- mfc140u 0x2541a0:
//     POSITION pos = GetStartPosition();                         // vtable slot 71
//     COleClientItem* pItem;
//     while ((pItem = GetNextClientItem(pos)) != NULL) {
//         if (pItem->m_lpObject != NULL)                         // +0x48
//             pItem->Release(OLECLOSE_NOSAVE);                   // item vtable slot 24
//         RemoveItem(pItem);                                     // vtable slot 74
//         pItem->InternalRelease();                              // 0x26cdb0: drops the
//     }                                                          //  document's reference,
//                                                                //  which deletes the item
//     if (IsSearchAndOrganizeHandler())                          // +0x128|+0x12c|+0x130
//         RELEASE(m_lpRootStg);                                  // +0x1c8
// OpenMFC's CDocItem is a CObject, not a refcounted CCmdTarget, so the item is
// deleted directly (the listed items are the OpenMFC-constructed objects the
// exported COleClientItem ctor thunk creates, so the C++ virtual destructor
// is the right one).  Because a POSITION here is an index that removal
// shifts, the walk restarts at the head after each item; RemoveItem always
// takes the item out of the list, so this visits the items in the same order
// retail does and terminates.
// Symbol: ?DeleteContents@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_COleDocument__UEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    for (;;) {
        POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
        COleClientItem* pItem = pThis->GetNextClientItem(pos);
        if (pItem == nullptr) break;
        if (pItem->m_lpObject != nullptr) {
            impl__Release_COleClientItem__UEAAXW4tagOLECLOSE___Z(pItem, OLECLOSE_NOSAVE);
        }
        impl__RemoveItem_COleDocument__UEAAXPEAVCDocItem___Z(pThis, pItem);
        if (pItem->m_pDocument == pThis || pItem->m_pContainerDoc == pThis) break;   // still attached: stop rather than spin
        delete pItem;
    }
    if (impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(pThis)) {
        OleDocExtra* extra = GetExtra(pThis, false);
        if (extra) ReleaseAndClear(extra->lpRootStg);
    }
}

// COleDocument::DoSave(LPCTSTR, BOOL) -- mfc140u 0x2546d0:
//     BOOL bRemember = m_bRemember;                              // +0x1d4
//     m_bRemember = bReplace;
//     BOOL bResult = CDocument::DoSave(lpszPathName, bReplace);  // 0x21b1f0
//     m_bRemember = bRemember;
//     return bResult;
// Symbol: ?DoSave@COleDocument@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DoSave_COleDocument__UEAAHPEB_WH_Z(COleDocument* pThis, const wchar_t* lpszPathName, int bReplace) {
    if (!pThis) return FALSE;
    int bRemember = pThis->m_bRemember;
    pThis->m_bRemember = bReplace;
    int bResult = pThis->CDocument::DoSave(lpszPathName, bReplace);
    pThis->m_bRemember = bRemember;
    return bResult;
}

// COleDocument::GetContainer() -- retail returns NULL: the COleDocument vtable
// slot 79 (0x278) resolves to mfc140u 0x71e0, the identical-code-folded
// `xor eax,eax; ret` body (the harvested slot table shows it under whichever
// symbol won the fold).  Linking to embedded items needs COleLinkingDoc, whose
// own GetContainer is exported separately.
// Symbol: ?GetContainer@COleDocument@@UEAAPEAUIOleItemContainer@@XZ
extern "C" void* MS_ABI impl__GetContainer_COleDocument__UEAAPEAUIOleItemContainer__XZ(COleDocument* pThis) {
    (void)pThis;
    return nullptr;
}

// COleDocument::GetFirstFrame() -- mfc140u 0x254980:
//     POSITION pos = GetFirstViewPosition();                     // vtable slot 28
//     CView* pView = GetNextView(pos);                           // vtable slot 29
//     return pView != NULL ? pView->GetParentFrame() : NULL;     // 0x28e200
// Symbol: ?GetFirstFrame@COleDocument@@QEAAPEAVCFrameWnd@@XZ
extern "C" void* MS_ABI impl__GetFirstFrame_COleDocument__QEAAPEAVCFrameWnd__XZ(COleDocument* pThis) {
    if (!pThis) return nullptr;
    void* pos = pThis->GetFirstViewPosition();
    CView* pView = pThis->GetNextView(pos);
    if (pView == nullptr) return nullptr;
    return impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pView);
}

// COleDocument::GetInPlaceActiveItem(CWnd*) -- mfc140u 0x26c6b0:
//     if (!pWnd->IsFrameWnd()) {                                 // CWnd vtable slot 86
//         CFrameWnd* pFrame = pWnd->GetParentFrame();            // 0x28e200
//         if (pFrame != NULL) pWnd = pFrame;
//     }
//     POSITION pos = GetStartPosition();                         // vtable slot 71
//     COleClientItem* pItem;
//     while ((pItem = GetNextClientItem(pos)) != NULL) {
//         if (pItem->m_pView != NULL &&                          // item +0x98
//             (pItem->m_nItemState == activeState ||             // item +0x90 in {3,4}
//              pItem->m_nItemState == activeUIState) &&
//             (pItem->m_pView == pWnd || pItem->m_pView->GetParentFrame() == pWnd))
//             return pItem;
//     }
//     return NULL;
// Deviation: OpenMFC's COleClientItem has no m_pView (core/ole/COleClientItem.cpp
// lists it as a retail-only member), so the "active in THIS frame" test cannot
// be evaluated; the first in-place-active client item (COleClientItem::
// IsInPlaceActive, the item-state test above) is returned for any frame.  With
// several frames each hosting an active item this can name the wrong one.
// Symbol: ?GetInPlaceActiveItem@COleDocument@@UEAAPEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_COleDocument__UEAAPEAVCOleClientItem__PEAVCWnd___Z(COleDocument* pThis, CWnd* pWnd) {
    if (!pThis) return nullptr;
    if (pWnd != nullptr && !impl__IsFrameWnd_CWnd__UEBAHXZ(pWnd)) {
        CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pWnd);
        if (pFrame != nullptr) pWnd = pFrame;
    }
    (void)pWnd;   // no per-item view to compare against (see above)
    POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
    COleClientItem* pItem;
    while ((pItem = pThis->GetNextClientItem(pos)) != nullptr) {
        if (pItem->IsInPlaceActive()) return pItem;
    }
    return nullptr;
}

// COleDocument::GetMoniker(OLEGETMONIKER) -- mfc140u 0x2549d0:
//     if (m_strPathName.IsEmpty()) return NULL;                  // +0x48
//     LPMONIKER lpMoniker;
//     HRESULT hr = ::CreateFileMoniker(m_strPathName, &lpMoniker);
//     return SUCCEEDED(hr) ? lpMoniker : NULL;
// nAssign is not read (COleLinkingDoc overrides this with the real protocol).
// Symbol: ?GetMoniker@COleDocument@@UEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z
extern "C" void* MS_ABI impl__GetMoniker_COleDocument__UEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(COleDocument* pThis, int nAssign) {
    (void)nAssign;
    if (!pThis || pThis->m_strPathName.IsEmpty()) return nullptr;
    LPMONIKER lpMoniker = nullptr;
    HRESULT hr = ::CreateFileMoniker(pThis->m_strPathName, &lpMoniker);
    return SUCCEEDED(hr) ? lpMoniker : nullptr;
}

// COleDocument::LoadFromStorage() -- mfc140u 0x255150:
//     COleStreamFile file; CFileException fe;
//     if (!file.OpenStream(m_lpRootStg, L"Contents", CFile::shareExclusive /*0x10*/, &fe) &&
//         !file.CreateStream(m_lpRootStg, L"Contents",
//                            CFile::modeCreate|CFile::shareExclusive /*0x1010*/, &fe)) {
//         if (fe.m_cause == CFileException::fileNotFound)
//             AfxThrowArchiveException(CArchiveException::badSchema);     // 0x1d3610
//         AfxThrowFileException(fe.m_cause, fe.m_lOsError);              // 0x22ce40
//     }
//     CArchive loadArchive(&file, CArchive::load|CArchive::bNoFlushOnDelete, 4096, NULL);
//     loadArchive.m_pDocument = this; loadArchive.m_bForceFlat = FALSE;
//     TRY {
//         if (file.GetLength() != 0) Serialize(loadArchive);             // vtable slot 2
//         loadArchive.Close(); file.Close();
//     } CATCH_ALL(e) { file.Abort(); loadArchive.Abort(); THROW_LAST(); }
// The stream is opened with the same IStorage calls COleStreamFile makes (the
// CFile mode bits are the STGM_* bits); CArchive::m_pDocument/m_bForceFlat do
// not exist in OpenMFC's CArchive; the CATCH_ALL cleanup (file.Abort,
// loadArchive.Abort, rethrow) is not reproduced -- see the exceptions note in
// the section banner: a gcc catch cannot take the exception and the
// ScopedArchive destructor does not run either, so a Serialize failure leaks
// the archive buffer and the stream reference.  A document with no root
// storage throws CFileException(genericException) where retail (release
// build) dereferences the NULL IStorage in COleStreamFile::OpenStream.
// Symbol: ?LoadFromStorage@COleDocument@@MEAAXXZ
extern "C" void MS_ABI impl__LoadFromStorage_COleDocument__MEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    OleDocExtra* extra = GetExtra(pThis, true);
    LPSTORAGE lpRootStg = extra ? extra->lpRootStg : nullptr;
    if (lpRootStg == nullptr) ThrowContentsStreamError(E_INVALIDARG);
    StreamCFile file = { g_streamCFileVtbl, nullptr };
    HRESULT sc = lpRootStg->OpenStream(L"Contents", nullptr, 0x10 /*STGM_SHARE_EXCLUSIVE*/, 0, &file.stream);
    if (sc != S_OK) {
        file.stream = nullptr;
        sc = lpRootStg->CreateStream(L"Contents", 0x1010 /*STGM_CREATE|STGM_SHARE_EXCLUSIVE*/, 0, 0, &file.stream);
        if (sc != S_OK) {
            file.stream = nullptr;
            ThrowContentsStreamError(sc);
        }
    }
    {
        ScopedArchive loadArchive(AsCFile(file), CArchive::load | CArchive::bNoFlushOnDelete);
        if (sf_GetLength(&file) != 0) pThis->Serialize(*loadArchive.ar);
        impl__Close_CArchive__QEAAXXZ(loadArchive.ar);
    }
    sf_Close(&file);
}

// COleDocument::OnCloseDocument() -- mfc140u 0x254f30:
//     if (IsSearchAndOrganizeHandler() && !m_bFinalRelease)      // +0x128.. / +0x94
//         return;
//     BOOL bAutoDelete = m_bAutoDelete; m_bAutoDelete = FALSE;   // +0x120
//     CDocument::OnCloseDocument();                              // 0x21c480
//     RELEASE(m_lpRootStg);                                      // +0x1c8
//     if (bAutoDelete) delete this;                              // vtable slot 1
// CDocument::m_bFinalRelease (set by CDocument::OnFinalRelease, mfc140u
// 0x21acb0) is not modeled in OpenMFC, so it reads as FALSE here.
// Symbol: ?OnCloseDocument@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_COleDocument__UEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    if (impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(pThis)) return;
    int bAutoDelete = pThis->m_bAutoDelete;
    pThis->m_bAutoDelete = FALSE;
    pThis->CDocument::OnCloseDocument();
    OleDocExtra* extra = GetExtra(pThis, false);
    if (extra) ReleaseAndClear(extra->lpRootStg);
    if (bAutoDelete) delete pThis;
}

// COleDocument::OnCmdMsg(UINT, int, void*, AFX_CMDHANDLERINFO*) -- mfc140u 0x26c540:
//     if (nCode == CN_COMMAND /*0*/) {
//         if (nID == ID_OLE_VERB_POPUP) nID = ID_OLE_VERB_FIRST;
//         if (nID - ID_OLE_VERB_FIRST <= 0xF) {                  // ID_OLE_VERB_FIRST..LAST
//             CView* pView = AfxGetThreadState()->m_pRoutingView;        // +0x1a8
//             COleClientItem* pItem = GetPrimarySelectedItem(pView);     // vtable slot 76
//             if (pItem != NULL) {
//                 if (pHandlerInfo != NULL) { pHandlerInfo->pTarget = this; return TRUE; }
//                 CWaitCursor wait;                              // AfxGetApp()->BeginWaitCursor()
//                 pItem->DoVerb(nID - ID_OLE_VERB_FIRST, pView, NULL);   // item vtable slot 25
//                 return TRUE;                                   // ~CWaitCursor -> EndWaitCursor
//             }
//         }
//     }
//     return CDocument::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);      // 0x21c840
// Deviations as in OnEditChangeIcon (routing view via GetRoutingView_,
// GetPrimarySelectedItem non-virtual here).  pTarget is the first member of
// retail's AFX_CMDHANDLERINFO, which OpenMFC only forward-declares.
// Symbol: ?OnCmdMsg@COleDocument@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_COleDocument__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(COleDocument* pThis, unsigned int nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    if (!pThis) return FALSE;
    if (nCode == 0 /*CN_COMMAND*/) {
        if (nID == kID_OLE_VERB_POPUP) nID = kID_OLE_VERB_FIRST;
        if (nID >= kID_OLE_VERB_FIRST && nID <= kID_OLE_VERB_LAST) {
            CView* pView = static_cast<CView*>(impl__GetRoutingView__CCmdTarget__KAPEAVCView__XZ());
            COleClientItem* pItem = pThis->GetPrimarySelectedItem(pView);
            if (pItem != nullptr) {
                if (pHandlerInfo != nullptr) {
                    *reinterpret_cast<CCmdTarget**>(pHandlerInfo) = pThis;   // pHandlerInfo->pTarget
                    return TRUE;
                }
                CCmdTarget* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();   // AfxGetModuleState()->m_pCurrentWinApp in retail
                impl__BeginWaitCursor_CCmdTarget__QEAAXXZ(pApp);
                impl__DoVerb_COleClientItem__UEAAHJPEAVCView__PEAUtagMSG___Z(
                    pItem, static_cast<long>(nID - kID_OLE_VERB_FIRST), pView, nullptr);
                impl__EndWaitCursor_CCmdTarget__QEAAXXZ(pApp);
                return TRUE;
            }
        }
    }
    return impl__OnCmdMsg_CDocument__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(pThis, nID, nCode, pExtra, pHandlerInfo);
}

// COleDocument::OnFileSendMail() -- mfc140u 0x2226c0 (mfc140 0x220b80):
//     LPSTORAGE lpOrigStg = m_lpRootStg; m_lpRootStg = NULL;    // +0x1c8
//     m_bRemember = FALSE;                                       // +0x1d4
//     CDocument::OnFileSendMail();                               // 0x2220c0 (unnamed)
//     m_lpRootStg = lpOrigStg; m_bRemember = TRUE;               // (also on unwind)
// The restore on the exception path is not reproduced (see the exceptions
// note in the section banner).
// Symbol: ?OnFileSendMail@COleDocument@@IEAAXXZ
extern "C" void MS_ABI impl__OnFileSendMail_COleDocument__IEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    OleDocExtra* extra = GetExtra(pThis, true);
    LPSTORAGE lpOrigStg = extra ? extra->lpRootStg : nullptr;
    if (extra) extra->lpRootStg = nullptr;
    pThis->m_bRemember = FALSE;
    impl__OnFileSendMail_CDocument__IEAAXXZ(pThis);
    if (extra) extra->lpRootStg = lpOrigStg;
    pThis->m_bRemember = TRUE;
}

// COleDocument::OnIdle() -- mfc140u 0x2548a0:
//     if (IsSearchAndOrganizeHandler()) return;                  // +0x128|+0x12c|+0x130
//     CDocument::OnIdle();                                       // 0x21c620 (unnamed)
//     BOOL bVisible = FALSE;
//     POSITION pos = GetFirstViewPosition();                     // vtable slot 28
//     while (pos != NULL) {
//         CView* pView = GetNextView(pos);                       // vtable slot 29
//         ENSURE(pView);                                         // 0x227720 on NULL
//         CFrameWnd* pFrame = pView->GetParentFrame();           // 0x28e200
//         ENSURE(pFrame);
//         if (pFrame->GetStyle() & WS_VISIBLE) { bVisible = TRUE; break; }   // 0x2a9690, bit 28
//     }
//     if (bVisible != m_bLastVisible) {                          // +0x1bc
//         OnShowViews(bVisible);                                 // vtable slot 77
//         m_bLastVisible = bVisible;
//     }
// OnShowViews is not virtual in OpenMFC's header, so this file's definition
// is called.
// Symbol: ?OnIdle@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnIdle_COleDocument__UEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    if (impl__IsSearchAndOrganizeHandler_CDocument__QEBAHXZ(pThis)) return;
    impl__OnIdle_CDocument__UEAAXXZ(pThis);
    BOOL bVisible = FALSE;
    void* pos = pThis->GetFirstViewPosition();
    while (pos != nullptr) {
        CView* pView = pThis->GetNextView(pos);
        if (pView == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
        CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pView);
        if (pFrame == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
        if (impl__GetStyle_CWnd__QEBAKXZ(pFrame) & WS_VISIBLE) {
            bVisible = TRUE;
            break;
        }
    }
    OleDocExtra* extra = GetExtra(pThis, true);
    if (extra && bVisible != extra->bLastVisible) {
        pThis->OnShowViews(bVisible);
        extra->bLastVisible = bVisible;
    }
}

// COleDocument::OnLoadDocumentFromStream(IStream*, DWORD) -- mfc140u 0x2552e0:
//     if (pStream == NULL) return E_INVALIDARG;
//     if (!m_bCompoundFile) {                                    // +0x1c0
//         COleStreamFile file; file.m_lpStream = pStream; file.m_bCloseOnDelete = FALSE;
//         CArchive ar(&file, CArchive::load, 4096, NULL);
//         Serialize(ar);                                         // vtable slot 2
//         file.m_lpStream = NULL;                                // caller keeps its reference
//         return S_OK;
//     }
//     LPSTORAGE lpStorage = NULL; ILockBytes* lpLockBytes = NULL;
//     { COleStreamFile file; file.m_lpStream = pStream; file.m_bCloseOnDelete = FALSE;
//       ULONGLONG len = file.GetLength();                        // 0x2655a0
//       if (len != 0) {
//           HGLOBAL h = ::GlobalAlloc(GMEM_MOVEABLE|GMEM_NODISCARD /*0x22*/, len);
//           if (h != NULL) {
//               void* p = ::GlobalLock(h);
//               if ((UINT)len != 0) file.Read(p, (UINT)len);      // throws on failure
//               ::GlobalUnlock(h);
//               HRESULT hr = ::CreateILockBytesOnHGlobal(h, TRUE, &lpLockBytes);
//               if (FAILED(hr)) { ::GlobalFree(h); AfxThrowOleException(hr); }
//           }
//       }
//       file.m_lpStream = NULL; }
//     if (lpLockBytes != NULL) {
//         HRESULT hr = ::StgOpenStorageOnILockBytes(lpLockBytes, NULL, STGM_PRIORITY /*0x40000*/, NULL, 0, &lpStorage);
//         if (FAILED(hr)) { RELEASE(lpLockBytes); AfxThrowOleException(hr); }
//     }
//     if (lpStorage != NULL) { m_lpRootStg = lpStorage; LoadFromStorage(); }   // vtable slot 80
//     if (lpLockBytes) lpLockBytes->Release();
//     return S_OK;
// grfMode is not read.  The CATCH_ALL that returns the failure HRESULT is
// not reproduced (see the exceptions note in the section banner).
// Symbol: ?OnLoadDocumentFromStream@COleDocument@@MEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__OnLoadDocumentFromStream_COleDocument__MEAAJPEAUIStream__K_Z(COleDocument* pThis, IStream* pStream, unsigned long grfMode) {
    (void)grfMode;
    if (!pThis) return E_POINTER;
    if (pStream == nullptr) return E_INVALIDARG;
    if (!pThis->m_bCompoundFile) {
        StreamCFile file = { g_streamCFileVtbl, pStream };
        {
            ScopedArchive ar(AsCFile(file), CArchive::load);
            pThis->Serialize(*ar.ar);
        }
        file.stream = nullptr;
        return S_OK;
    }
    LPSTORAGE lpStorage = nullptr;
    ILockBytes* lpLockBytes = nullptr;
    {
        StreamCFile file = { g_streamCFileVtbl, pStream };
        unsigned long long len = sf_GetLength(&file);
        if (len != 0) {
            HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD, static_cast<SIZE_T>(len));
            if (hGlobal != nullptr) {
                void* p = ::GlobalLock(hGlobal);
                if (static_cast<unsigned int>(len) != 0) sf_Read(&file, p, static_cast<unsigned int>(len));
                ::GlobalUnlock(hGlobal);
                HRESULT hr = ::CreateILockBytesOnHGlobal(hGlobal, TRUE, &lpLockBytes);
                if (FAILED(hr)) {
                    ::GlobalFree(hGlobal);
                    impl__AfxThrowOleException__YAXJ_Z(hr);
                }
            }
        }
        file.stream = nullptr;
    }
    if (lpLockBytes != nullptr) {
        HRESULT hr = ::StgOpenStorageOnILockBytes(lpLockBytes, nullptr, 0x40000 /*STGM_PRIORITY*/, nullptr, 0, &lpStorage);
        if (FAILED(hr)) {
            ReleaseAndClear(lpLockBytes);
            impl__AfxThrowOleException__YAXJ_Z(hr);
        }
    }
    if (lpStorage != nullptr) {
        OleDocExtra* extra = GetExtra(pThis, true);
        if (extra) extra->lpRootStg = lpStorage;
        impl__LoadFromStorage_COleDocument__MEAAXXZ(pThis);
    }
    if (lpLockBytes) lpLockBytes->Release();
    return S_OK;
}

// COleDocument::OnNewDocument() -- mfc140u 0x254a40:
//     if (!CDocument::OnNewDocument()) return FALSE;             // 0x21c060 (unnamed)
//     if (m_bCompoundFile && !m_bEmbedded) {                     // +0x1c0, +0x124
//         RELEASE(m_lpRootStg);                                  // +0x1c8
//         LPSTORAGE lpStorage;
//         HRESULT sc = ::StgCreateDocfile(NULL,
//             STGM_DELETEONRELEASE|STGM_CREATE|STGM_SHARE_EXCLUSIVE|STGM_READWRITE /*0x4011012*/,
//             0, &lpStorage);
//         if (sc != S_OK) return FALSE;
//         m_lpRootStg = lpStorage;
//     }
//     return TRUE;
// CDocument::m_bEmbedded is not modeled in OpenMFC and reads as FALSE here.
// Symbol: ?OnNewDocument@COleDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_COleDocument__UEAAHXZ(COleDocument* pThis) {
    if (!pThis) return FALSE;
    if (!pThis->CDocument::OnNewDocument()) return FALSE;
    if (pThis->m_bCompoundFile) {
        OleDocExtra* extra = GetExtra(pThis, true);
        if (extra) ReleaseAndClear(extra->lpRootStg);
        LPSTORAGE lpStorage = nullptr;
        HRESULT sc = ::StgCreateDocfile(nullptr,
            STGM_DELETEONRELEASE | STGM_CREATE | STGM_SHARE_EXCLUSIVE | STGM_READWRITE, 0, &lpStorage);
        if (sc != S_OK) return FALSE;
        if (extra) extra->lpRootStg = lpStorage;
        else lpStorage->Release();
    }
    return TRUE;
}

// COleDocument::OnOpenDocument(LPCTSTR) -- mfc140u 0x254ab0 (mfc140 0x253b80,
// where the map names the callees):
//     if (!m_bCompoundFile && m_lpRootStg == NULL)               // +0x1c0, +0x1c8
//         return CDocument::OnOpenDocument(lpszPathName);        // 0x21c0c0 (unnamed)
//     IsModified();                                              // vtable slot 26 (TRACE only)
//     if (lpszPathName != NULL) { DeleteContents(); RELEASE(m_lpRootStg); }   // slot 31
//     SetModifiedFlag(TRUE);                                     // slot 27: dirty while loading
//     BOOL bResult = FALSE;
//     TRY {
//         if (m_lpRootStg == NULL) {
//             LPSTORAGE lpStorage = NULL; HRESULT sc;
//             if (lpszPathName != NULL && ::StgIsStorageFile(lpszPathName) == S_FALSE)
//                 sc = ::StgCreateDocfile(lpszPathName,
//                     STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE|STGM_CONVERT /*0x30022*/, 0, &lpStorage);
//             else {
//                 sc = ::StgOpenStorage(lpszPathName, NULL,
//                     STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE /*0x10022*/, NULL, 0, &lpStorage);
//                 if (FAILED(sc) || lpStorage == NULL)
//                     sc = ::StgOpenStorage(lpszPathName, NULL, STGM_READ|STGM_TRANSACTED /*0x10000*/, NULL, 0, &lpStorage);
//                 if (FAILED(sc) || lpStorage == NULL)
//                     sc = ::StgOpenStorage(lpszPathName, NULL, STGM_READ|STGM_PRIORITY /*0x40000*/, NULL, 0, &lpStorage);
//             }
//             if (FAILED(sc)) AfxThrowOleException(sc);          // 0x25f2c0
//             m_lpRootStg = lpStorage;
//         }
//         LoadFromStorage();                                     // vtable slot 80
//         SetModifiedFlag(FALSE);
//         bResult = TRUE;
//     } CATCH_ALL(e) {                                           // funclet mfc140u 0x2c2f68
//         DeleteContents(); RELEASE(m_lpRootStg);
//         if (lpszPathName == NULL) THROW_LAST();
//         ReportSaveLoadException(lpszPathName, e, FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);   // slot 36
//         DELETE_EXCEPTION(e);
//     }
//     m_strPathName = lpszPathName;                              // +0x48 (SetString)
//     m_strStorageName = lpszPathName;                           // +0x1d8
//     return bResult;
// The CATCH_ALL branch is not reproduced (see the exceptions note in the
// section banner): a failure propagates as the exception the callee threw.
// Symbol: ?OnOpenDocument@COleDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_COleDocument__UEAAHPEB_W_Z(COleDocument* pThis, const wchar_t* lpszPathName) {
    if (!pThis) return FALSE;
    OleDocExtra* extra = GetExtra(pThis, true);
    if (!extra) return FALSE;
    if (!pThis->m_bCompoundFile && extra->lpRootStg == nullptr) {
        return pThis->CDocument::OnOpenDocument(lpszPathName);
    }
    (void)pThis->IsModified();
    if (lpszPathName != nullptr) {
        pThis->DeleteContents();
        ReleaseAndClear(extra->lpRootStg);
    }
    pThis->SetModifiedFlag(TRUE);
    if (extra->lpRootStg == nullptr) {
        LPSTORAGE lpStorage = nullptr;
        HRESULT sc;
        if (lpszPathName != nullptr && ::StgIsStorageFile(lpszPathName) == S_FALSE) {
            sc = ::StgCreateDocfile(lpszPathName,
                STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE | STGM_CONVERT, 0, &lpStorage);
        } else {
            sc = ::StgOpenStorage(lpszPathName, nullptr,
                STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE, nullptr, 0, &lpStorage);
            if (FAILED(sc) || lpStorage == nullptr) {
                sc = ::StgOpenStorage(lpszPathName, nullptr, STGM_READ | STGM_TRANSACTED, nullptr, 0, &lpStorage);
            }
            if (FAILED(sc) || lpStorage == nullptr) {
                sc = ::StgOpenStorage(lpszPathName, nullptr, STGM_READ | STGM_PRIORITY, nullptr, 0, &lpStorage);
            }
        }
        if (FAILED(sc)) impl__AfxThrowOleException__YAXJ_Z(sc);
        extra->lpRootStg = lpStorage;
    }
    impl__LoadFromStorage_COleDocument__MEAAXXZ(pThis);
    pThis->SetModifiedFlag(FALSE);
    pThis->m_strPathName = lpszPathName ? lpszPathName : L"";
    extra->strStorageName = lpszPathName ? lpszPathName : L"";
    return TRUE;
}

// COleDocument::OnSaveDocument(LPCTSTR) -- mfc140u 0x254d20 (mfc140 0x253df0):
//     if (!m_bCompoundFile && m_lpRootStg == NULL)
//         return CDocument::OnSaveDocument(lpszPathName);        // 0x21c2c0 (unnamed)
//     LPSTORAGE lpOrigStg = NULL;
//     if (lpszPathName != NULL)
//         m_bSameAsLoad = AfxComparePath(m_strPathName, lpszPathName);   // +0x1d0; 0x228d60
//     BOOL bResult = FALSE;
//     TRY {
//         if (lpszPathName != NULL && !m_bSameAsLoad) {
//             lpOrigStg = m_lpRootStg; m_lpRootStg = NULL;
//             LPSTORAGE lpStorage;
//             HRESULT sc = ::StgCreateDocfile(lpszPathName,
//                 STGM_READWRITE|STGM_TRANSACTED|STGM_SHARE_EXCLUSIVE|STGM_CREATE /*0x11022*/, 0, &lpStorage);
//             if (sc != S_OK) AfxThrowOleException(sc);
//             m_lpRootStg = lpStorage;
//         }
//         SaveToStorage(NULL);                                   // vtable slot 81
//         if (lpszPathName != NULL) {
//             CommitItems(m_bRemember && !m_bSameAsLoad, NULL);  // 0x254510
//             if (m_bRemember) {                                 // +0x1d4
//                 SetModifiedFlag(FALSE);                        // slot 27
//                 m_strPathName = lpszPathName;                  // +0x48
//                 m_strStorageName = lpszPathName;               // +0x1d8
//             }
//             if (!m_bSameAsLoad) {
//                 if (m_bRemember) lpOrigStg->Release();         // no NULL check in retail
//                 else { m_lpRootStg->Release(); m_lpRootStg = lpOrigStg; }
//             }
//         }
//         bResult = TRUE;
//     } CATCH_ALL(e) {                                           // funclet mfc140u 0x2c3022
//         if (lpOrigStg != NULL) { RELEASE(m_lpRootStg); m_lpRootStg = lpOrigStg; }
//         if (lpszPathName == NULL) THROW_LAST();
//         ReportSaveLoadException(lpszPathName, e, TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
//         DELETE_EXCEPTION(e);
//     }
//     m_bSameAsLoad = TRUE; m_bRemember = TRUE;
//     return bResult;
// The CATCH_ALL branch (restoring the original storage) is not reproduced
// (see the exceptions note in the section banner).  The trailing
// m_bSameAsLoad/m_bRemember reset is a scope guard that only runs on the
// normal path: verified 2026-09-19 that gcc destructors do NOT run while one
// of this DLL's _CxxThrowException-raised exceptions unwinds through a gcc
// frame, so after a throw both flags keep the values retail would also have
// reset.  AfxComparePath is evaluated by the local SamePath (see its comment).
// Symbol: ?OnSaveDocument@COleDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_COleDocument__UEAAHPEB_W_Z(COleDocument* pThis, const wchar_t* lpszPathName) {
    if (!pThis) return FALSE;
    OleDocExtra* extra = GetExtra(pThis, true);
    if (!extra) return FALSE;
    if (!pThis->m_bCompoundFile && extra->lpRootStg == nullptr) {
        return pThis->CDocument::OnSaveDocument(lpszPathName);
    }
    LPSTORAGE lpOrigStg = nullptr;
    if (lpszPathName != nullptr) {
        extra->bSameAsLoad = SamePath(pThis->m_strPathName, lpszPathName) ? TRUE : FALSE;
    }
    struct ResetFlags {
        COleDocument* doc; OleDocExtra* extra;
        ~ResetFlags() { extra->bSameAsLoad = TRUE; doc->m_bRemember = TRUE; }
    } reset = { pThis, extra };
    if (lpszPathName != nullptr && !extra->bSameAsLoad) {
        lpOrigStg = extra->lpRootStg;
        extra->lpRootStg = nullptr;
        LPSTORAGE lpStorage = nullptr;
        HRESULT sc = ::StgCreateDocfile(lpszPathName,
            STGM_READWRITE | STGM_TRANSACTED | STGM_SHARE_EXCLUSIVE | STGM_CREATE, 0, &lpStorage);
        if (sc != S_OK) impl__AfxThrowOleException__YAXJ_Z(sc);
        extra->lpRootStg = lpStorage;
    }
    impl__SaveToStorage_COleDocument__MEAAXPEAVCObject___Z(pThis, nullptr);
    if (lpszPathName != nullptr) {
        impl__CommitItems_COleDocument__QEAAXHPEAUIStorage___Z(
            pThis, (pThis->m_bRemember && !extra->bSameAsLoad) ? TRUE : FALSE, nullptr);
        if (pThis->m_bRemember) {
            pThis->SetModifiedFlag(FALSE);
            pThis->m_strPathName = lpszPathName;
            extra->strStorageName = lpszPathName;
        }
        if (!extra->bSameAsLoad) {
            if (pThis->m_bRemember) {
                if (lpOrigStg) lpOrigStg->Release();
            } else {
                ReleaseAndClear(extra->lpRootStg);
                extra->lpRootStg = lpOrigStg;
            }
        }
    }
    return TRUE;
}

// COleDocument::PreCloseFrame(CFrameWnd*) -- mfc140u 0x254690 (mfc140 0x253760):
//     COleClientItem* pItem = GetInPlaceActiveItem(pFrameWnd);  // vtable slot 70
//     if (pItem != NULL) {
//         pItem->Deactivate();                                   // 0x249050
//         pItem->Close(OLECLOSE_NOSAVE);                         // 0x244630
//     }
// GetInPlaceActiveItem is virtual in retail; OpenMFC's header does not declare
// it, so the export above is called directly.
// Symbol: ?PreCloseFrame@COleDocument@@UEAAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__PreCloseFrame_COleDocument__UEAAXPEAVCFrameWnd___Z(COleDocument* pThis, CFrameWnd* pFrameWnd) {
    if (!pThis) return;
    COleClientItem* pItem = static_cast<COleClientItem*>(
        impl__GetInPlaceActiveItem_COleDocument__UEAAPEAVCOleClientItem__PEAVCWnd___Z(pThis, pFrameWnd));
    if (pItem != nullptr) {
        impl__Deactivate_COleClientItem__QEAAXXZ(pItem);
        impl__Close_COleClientItem__QEAAXW4tagOLECLOSE___Z(pItem, OLECLOSE_NOSAVE);
    }
}

// COleDocument::SaveModified() -- mfc140u 0x254700 (mfc140 0x2537d0):
//     if (::InSendMessage()) {
//         POSITION pos = GetStartPosition();                     // vtable slot 71
//         COleClientItem* pItem;
//         while ((pItem = GetNextClientItem(pos)) != NULL) {     // 0x254120
//             HRESULT sc = pItem->m_lpObject->IsUpToDate();      // +0x48, IOleObject slot 14
//             if (sc != OLE_E_NOTRUNNING /*0x80040005*/ && FAILED(sc)) {
//                 CString name = m_strPathName;                  // +0x48
//                 if (name.IsEmpty()) name.LoadString(AFX_IDS_UNTITLED);
//                 CString prompt;
//                 AfxFormatStrings(prompt, AFX_IDP_ASK_TO_DISCARD, &name, 1);  // 0x1cdc90 (AfxFormatString1)
//                 return AfxMessageBox(prompt, MB_OKCANCEL|MB_DEFBUTTON2 /*0x101*/,
//                                      AFX_IDP_ASK_TO_DISCARD) == IDOK;         // 0x1cec10
//             }
//         }
//     }
//     UpdateModifiedFlag();                                      // vtable slot 75
//     return CDocument::SaveModified();                          // 0x21b5b0 (unnamed)
// Retail dereferences m_lpObject unconditionally; a blank item is skipped here.
// Symbol: ?SaveModified@COleDocument@@UEAAHXZ
extern "C" int MS_ABI impl__SaveModified_COleDocument__UEAAHXZ(COleDocument* pThis) {
    if (!pThis) return TRUE;
    if (::InSendMessage()) {
        POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
        COleClientItem* pItem;
        while ((pItem = pThis->GetNextClientItem(pos)) != nullptr) {
            if (pItem->m_lpObject == nullptr) continue;
            HRESULT sc = pItem->m_lpObject->IsUpToDate();
            if (sc != static_cast<HRESULT>(0x80040005) /*OLE_E_NOTRUNNING*/ && FAILED(sc)) {
                CString name = pThis->m_strPathName;
                if (name.IsEmpty()) {
                    impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(&name, kAFX_IDS_UNTITLED);
                }
                CString prompt;
                impl__AfxFormatString1__YAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__IPEB_W_Z(
                    &prompt, kAFX_IDP_ASK_TO_DISCARD, name);
                return impl__AfxMessageBox__YAHPEB_WII_Z(prompt, MB_OKCANCEL | MB_DEFBUTTON2, kAFX_IDP_ASK_TO_DISCARD) == IDOK;
            }
        }
    }
    impl__UpdateModifiedFlag_COleDocument__UEAAXXZ(pThis);
    return pThis->CDocument::SaveModified();
}

// COleDocument::SaveToStorage(CObject*) -- mfc140u 0x254fb0 (mfc140 0x254080):
//     COleStreamFile file; CFileException fe;
//     if (!file.CreateStream(m_lpRootStg, L"Contents",
//             CFile::modeReadWrite|CFile::shareExclusive|CFile::modeCreate /*0x1012*/, &fe)) {
//         if (fe.m_cause == CFileException::fileNotFound)
//             AfxThrowArchiveException(CArchiveException::badSchema);
//         AfxThrowFileException(fe.m_cause, fe.m_lOsError);
//     }
//     CArchive saveArchive(&file, CArchive::store|CArchive::bNoFlushOnDelete, 4096, NULL);
//     saveArchive.m_pDocument = this; saveArchive.m_bForceFlat = FALSE;
//     TRY {
//         if (pObject != NULL) pObject->Serialize(saveArchive);   // CObject vtable slot 2
//         else Serialize(saveArchive);                            // this, vtable slot 2
//         saveArchive.Close(); file.Close();
//         HRESULT sc = m_lpRootStg->Commit(STGC_ONLYIFCURRENT /*2*/);   // IStorage slot 9
//         if (sc != S_OK) AfxThrowOleException(sc);
//     } CATCH_ALL(e) { file.Abort(); saveArchive.Abort(); THROW_LAST(); }
// Same notes as LoadFromStorage.
// Symbol: ?SaveToStorage@COleDocument@@MEAAXPEAVCObject@@@Z
extern "C" void MS_ABI impl__SaveToStorage_COleDocument__MEAAXPEAVCObject___Z(COleDocument* pThis, CObject* pObject) {
    if (!pThis) return;
    OleDocExtra* extra = GetExtra(pThis, true);
    LPSTORAGE lpRootStg = extra ? extra->lpRootStg : nullptr;
    if (lpRootStg == nullptr) ThrowContentsStreamError(E_INVALIDARG);
    StreamCFile file = { g_streamCFileVtbl, nullptr };
    HRESULT sc = lpRootStg->CreateStream(L"Contents",
        0x1012 /*STGM_CREATE|STGM_SHARE_EXCLUSIVE|STGM_READWRITE*/, 0, 0, &file.stream);
    if (sc != S_OK) {
        file.stream = nullptr;
        ThrowContentsStreamError(sc);
    }
    {
        ScopedArchive saveArchive(AsCFile(file), CArchive::store | CArchive::bNoFlushOnDelete);
        if (pObject != nullptr) pObject->Serialize(*saveArchive.ar);
        else pThis->Serialize(*saveArchive.ar);
        impl__Close_CArchive__QEAAXXZ(saveArchive.ar);
    }
    sf_Close(&file);
    sc = lpRootStg->Commit(STGC_ONLYIFCURRENT);
    if (sc != S_OK) impl__AfxThrowOleException__YAXJ_Z(sc);
}

// COleDocument::Serialize(CArchive&) -- mfc140u 0x254390 (mfc140 0x253460):
//     if (ar.IsStoring()) {                                      // ar +0x20 bit 0 clear
//         DWORD dwCount = 0;
//         POSITION pos = GetStartPosition();                     // vtable slot 71
//         while (pos) { CDocItem* p = GetNextItem(pos); if (!p->IsBlank()) ++dwCount; }  // slot 72; item slot 22
//         ar << dwCount;                                         // inlined buffer write
//         pos = GetStartPosition();
//         while (pos) { CDocItem* p = GetNextItem(pos); if (!p->IsBlank()) ar << p; }   // 0x1d2570 WriteObject (unnamed)
//     } else {
//         DWORD dwCount; ar >> dwCount;                          // inlined FillBuffer read
//         while (dwCount--) {
//             CDocItem* pItem;
//             ar >> pItem;                                       // 0x255660: ReadObject(RUNTIME_CLASS(CDocItem))
//         }                                                      // the item registers itself with
//     }                                                          //  ar.m_pDocument in its ctor
// OpenMFC's CArchive has no m_pDocument for a loaded item to attach to, so an
// item that comes back without a document is added here.  IsBlank is
// dispatched by kind (see ItemIsBlank).  The DWORD goes through CArchive's
// Read/Write thunks and the objects through ReadObject/WriteObject.
// Symbol: ?Serialize@COleDocument@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_COleDocument__UEAAXAEAVCArchive___Z(COleDocument* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    if (ar->IsStoring()) {
        unsigned long dwCount = 0;
        POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
        while (pos) {
            CDocItem* pItem = static_cast<CDocItem*>(impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(pThis, &pos));
            if (pItem && !ItemIsBlank(pItem)) ++dwCount;
        }
        impl__Write_CArchive__QEAAXPEBXI_Z(ar, &dwCount, sizeof(dwCount));
        pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
        while (pos) {
            CDocItem* pItem = static_cast<CDocItem*>(impl__GetNextItem_COleDocument__UEBAPEAVCDocItem__AEAPEAU__POSITION___Z(pThis, &pos));
            if (pItem && !ItemIsBlank(pItem)) impl__WriteObject_CArchive__QEAAXPEBVCObject___Z(ar, pItem);
        }
    } else {
        unsigned long dwCount = 0;
        if (impl__Read_CArchive__QEAAIPEAXI_Z(ar, &dwCount, sizeof(dwCount)) != sizeof(dwCount)) {
            impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::endOfFile, nullptr);
        }
        CRuntimeClass* pDocItemClass = impl__GetThisClass_CDocItem__SAPEAUCRuntimeClass__XZ();
        while (dwCount--) {
            CObject* pOb = impl__ReadObject_CArchive__QEAAPEAVCObject__PEBUCRuntimeClass___Z(ar, pDocItemClass);
            // Retail's ReadObject rejects an object of the wrong class with
            // CArchiveException::badClass; OpenMFC's ignores its class argument,
            // so the check is made here.
            if (pOb != nullptr && !ClassMatches(pOb->GetRuntimeClass(), pDocItemClass)) {
                impl__AfxThrowArchiveException__YAXHPEB_W_Z(CArchiveException::badClass, nullptr);
            }
            CDocItem* pItem = static_cast<CDocItem*>(pOb);
            if (pItem != nullptr && pItem->m_pDocument == nullptr) {
                impl__AddItem_COleDocument__UEAAXPEAVCDocItem___Z(pThis, pItem);
            }
        }
    }
}

// COleDocument::SetPathName(LPCTSTR, BOOL) -- mfc140u 0x254260 (mfc140 0x253320):
//     m_bSameAsLoad = AfxComparePath(m_strStorageName, lpszPathName);   // +0x1d0, +0x1d8; 0x228d60
//     CDocument::SetPathName(lpszPathName, bAddToMRU);           // 0x21afe0 (unnamed)
//     POSITION pos = GetStartPosition();                         // vtable slot 71
//     COleClientItem* pItem;
//     while ((pItem = GetNextClientItem(pos)) != NULL) {         // 0x254120
//         ENSURE(pItem->m_lpObject != NULL);                     // +0x48; 0x227720 (AfxThrowInvalidArgException)
//         CString strApp(AfxGetModuleState()->m_lpszCurrentAppName);   // module state +0x20
//         CString strTitle = m_strTitle;                         // +0x40
//         pItem->m_lpObject->SetHostNames(strApp, strTitle);     // IOleObject slot 5
//     }
// The app name comes from CWinApp::m_pszAppName, which is what
// CWinApp::SetCurrentHandles copies into the module state in retail.
// AfxComparePath is evaluated by the local SamePath (see its comment).
// Symbol: ?SetPathName@COleDocument@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_COleDocument__UEAAXPEB_WH_Z(COleDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU) {
    if (!pThis) return;
    OleDocExtra* extra = GetExtra(pThis, true);
    if (extra) extra->bSameAsLoad = SamePath(extra->strStorageName, lpszPathName) ? TRUE : FALSE;
    pThis->CDocument::SetPathName(lpszPathName, bAddToMRU);
    POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
    COleClientItem* pItem;
    while ((pItem = pThis->GetNextClientItem(pos)) != nullptr) {
        if (pItem->m_lpObject == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
        CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
        CString strApp(pApp ? pApp->m_pszAppName : nullptr);
        CString strTitle = pThis->m_strTitle;
        pItem->m_lpObject->SetHostNames(strApp, strTitle);
    }
}

// COleDocument::UpdateModifiedFlag() -- mfc140u 0x254620 (mfc140 0x2536f0):
//     POSITION pos = GetStartPosition();                         // vtable slot 71
//     COleClientItem* pItem;
//     while ((pItem = GetNextClientItem(pos)) != NULL) {         // 0x254120
//         if (pItem->IsModified()) {                             // 0x245600
//             SetModifiedFlag(TRUE);                             // vtable slot 27
//             break;
//         }
//     }
// Symbol: ?UpdateModifiedFlag@COleDocument@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateModifiedFlag_COleDocument__UEAAXXZ(COleDocument* pThis) {
    if (!pThis) return;
    POSITION pos = reinterpret_cast<POSITION>(pThis->GetStartPosition());
    COleClientItem* pItem;
    while ((pItem = pThis->GetNextClientItem(pos)) != nullptr) {
        if (impl__IsModified_COleClientItem__QEBAHXZ(pItem)) {
            pThis->SetModifiedFlag(TRUE);
            break;
        }
    }
}
