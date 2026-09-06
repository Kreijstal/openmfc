// COleServerItem — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// COleServerItem entry points decoded from the retail mfc140 exports.
//
// Method used here is the project's usual one (see the header comment of
// core/ole/COleControl.cpp): each body below was read out of the retail
// implementation with objdump and transcribed; RVAs are cited per function.
//
// Retail member offsets seen in that disassembly, for reference (they are NOT
// OpenMFC's offsets -- OpenMFC's COleServerItem is a reduced model, so the
// bodies below are written against OpenMFC's named members):
//   +0x38  m_pModuleState        (CCmdTarget)
//   +0x40  m_pDocument           (CDocItem)
//   +0x50  m_bNeedUnlock         -- not modeled by OpenMFC
//   +0x54  m_bAutoDelete
//   +0x58  m_strItemName         -- not modeled by OpenMFC
//   +0x60  m_lpOleAdviseHolder   -- not modeled by OpenMFC
//   +0x68  m_lpDataAdviseHolder  -- not modeled by OpenMFC
//   +0xc8  m_dataSource.m_xDataObject (CItemDataSource, a COleDataSource)
//   +0xd0  m_xOleObject          -- nested IOleObject, not modeled by OpenMFC
//   +0xd8  m_xDataObject         -- nested IDataObject, not modeled by OpenMFC
// Every one of those offsets is one read out of the retail disassembly cited at
// the use site; none of them is asserted about OpenMFC. (The member *names* are
// the ones the shipping MFC header uses -- note m_lpOleAdviseHolder /
// m_lpDataAdviseHolder, not m_p*; an earlier revision of this comment had the
// wrong prefix.)
//
// Retail's COleServerItem virtual slots, dumped from the one shipping vtable
// that instantiates them -- CDocObjectServerItem's, at RVA 0x32d048.
// (COleServerItem itself is AFX_NOVTABLE/abstract, so it has no vtable of its
// own to read. Read that dump with care: slots 28, 33 and 35 hold
// CDocObjectServerItem's own OnDoVerb/OnShow/OnHide overrides, not
// COleServerItem's, which live at 0x2690d0 / 0x269170 / 0x2691e0. Every other
// slot listed resolves to a COleServerItem body. Slot 22 is COleServerItem's
// override of CDocItem::IsBlank -- CDocItem's own vtable, RVA 0x32c520, has a
// different body there.)
//   22 (0xb0)  IsBlank        23 (0xb8)  OnDraw (pure)  24 (0xc0)  OnUpdate
//   25 (0xc8)  OnDrawEx       26 (0xd0)  OnSetExtent    27 (0xd8)  OnGetExtent
//   28 (0xe0)  OnDoVerb       29 (0xe8)  OnSetColorScheme
//   30 (0xf0)  OnGetClipboardData      31 (0xf8) OnQueryUpdateItems
//   32 (0x100) OnUpdateItems  33 (0x108) OnShow  34 (0x110) OnOpen
//   35 (0x118) OnHide         36 (0x120) OnInitFromData
//   37 (0x128) OnRenderGlobalData      38 (0x130) OnRenderFileData
//   39 (0x138) OnRenderData   40 (0x140) OnSetData
//   41 (0x148) GetMetafileData         42 (0x150) OnSaveEmbedding
// Several of those slots point at one-instruction bodies that the retail linker
// folded (ICF) with unrelated exports -- 0x7260 `xor eax,eax; ret`, 0x3ae0
// `mov eax,1; ret`, 0xf540 `mov eax,0x80004001; ret`. That fold is why their
// own mangled names are missing from mfc140_rva_symbols.json; the vtable, not
// the symbol map, is how their bodies are recovered here.
//
// One deliberate deviation, stated once here rather than at seven call sites:
// where retail reads m_pDocument and tail-calls ?AfxThrowInvalidArgException@@
// YAXXZ (RVA 0x225b80, followed by int3) when it is NULL, the bodies below
// return quietly instead. That is a release-mode ENSURE that throws, not a
// debug-only ASSERT; an earlier revision of these comments wrote it as
// "ASSERT(pDoc != NULL)", which understated it.
//
// OpenMFC's COleServerItem declares none of OnShow / OnOpen / OnHide /
// OnDoVerb / GetMetafileData / NotifyClient as virtuals (checked against
// include/openmfc/afxole.h: the class carries only m_pServerDoc, m_bAutoDelete
// and char _oleserveritem_padding[136]), so where retail dispatches virtually
// this file calls the corresponding impl__ thunk directly -- a derived-class
// override is not picked up; noted per function.
// ---------------------------------------------------------------------------

// Forward declarations for the sibling thunks called below. Every one of these
// is defined later in THIS file, so no cross-object dependency is introduced.
extern "C" void MS_ABI impl__OnShow_COleServerItem__MEAAXXZ(COleServerItem* pThis);
extern "C" void MS_ABI impl__OnOpen_COleServerItem__MEAAXXZ(COleServerItem* pThis);
extern "C" void MS_ABI impl__OnHide_COleServerItem__MEAAXXZ(COleServerItem* pThis);
extern "C" void MS_ABI impl__NotifyClient_COleServerItem__QEAAXW4OLE_NOTIFICATION___K_Z(
    COleServerItem* pThis, unsigned int wNotification, ULONG_PTR dwParam);
extern "C" int MS_ABI impl__GetMetafileData_COleServerItem__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    COleServerItem* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium);
extern "C" void MS_ABI impl__GetClipboardData_COleServerItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
    COleServerItem* pThis, COleDataSource* pDataSource, int bIncludeLink,
    POINT* lpOffset, SIZE* lpSize);

// Sibling exports in other translation units. Per this DLL's rule, C++ methods
// exist only as these extern "C" thunks, so they are called by thunk name:
//   COleServerDoc::ActivateInPlace   -- core/ole/COleServerDoc.cpp
//   COleDocument::GetStartPosition   -- core/ole/Thunks.cpp
//   COleDocument::GetNextClientItem  -- core/ole/Thunks.cpp
//   AfxThrowOleException             -- detail/MfcExceptionsSupport.cpp
extern "C" int MS_ABI impl__ActivateInPlace_COleServerDoc__QEAAHXZ(COleServerDoc* pThis);
extern "C" void* MS_ABI impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(
    const COleDocument* pThis);
extern "C" COleClientItem* MS_ABI
impl__GetNextClientItem_COleDocument__QEBAPEAVCOleClientItem__AEAPEAU__POSITION___Z(
    const COleDocument* pThis, void** ppos);
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc);

// Retail's OLE_NOTIFICATION values. Read out of the NotifyClient switch at RVA
// 0x268ef0 (cases 0,1,2,3 select SendOnDataChange / SendOnSave / SendOnClose /
// SendOnRename respectively) and cross-checked against the shipping MFC header
// on this host, atlmfc/include/afxole.h, which declares
//   enum OLE_NOTIFICATION { OLE_CHANGED, OLE_SAVED, OLE_CLOSED, OLE_RENAMED,
//                           OLE_CHANGED_STATE, OLE_CHANGED_ASPECT };
// NOTE: openmfc/afxole.h typedefs OLE_NOTIFICATION to DWORD and #defines
// OLE_CHANGED as 5 in the OLE_STATUS run (OLE_EMPTY..OLE_OPEN), so the macro
// there names a different value than retail's enumerator. The literals below
// are used rather than that macro; the header is the real fix (headerRequests).
namespace {
enum RetailOleNotification {
    kRetailOleChanged = 0,
    kRetailOleSaved   = 1,
    kRetailOleClosed  = 2,
    kRetailOleRenamed = 3
};
} // namespace

























// Symbol: ??0COleServerItem@@IEAA@PEAVCOleServerDoc@@H@Z
// COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
extern "C" void* MS_ABI impl___0COleServerItem__IEAA_PEAVCOleServerDoc__H_Z(COleServerDoc* pServerDoc, unsigned int bAutoDelete) {
    return new COleServerItem(pServerDoc, bAutoDelete);
}
COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
    : m_pServerDoc(pServerDoc), m_bAutoDelete(bAutoDelete) {
    memset(_oleserveritem_padding, 0, sizeof(_oleserveritem_padding));
    m_pDocument = pServerDoc;
    GetServerItemState(this, true);
    if (pServerDoc) AddServerDocItem(pServerDoc, this);
}
COleServerItem::~COleServerItem() {
    if (m_pServerDoc) RemoveServerDocItem(m_pServerDoc, this);
    RemoveServerItemState(this);
}
BOOL COleServerItem::OnDraw(CDC* pDC, CSize& rSize) {
    return OnDrawEx(pDC, DVASPECT_CONTENT, rSize);
}
BOOL COleServerItem::OnDrawEx(CDC* pDC, DVASPECT nDrawAspect, CSize& rSize) {
    (void)pDC; // Lifecycle-only fallback: use stored extent when no rendering backend is available.
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    return OnGetExtent(nDrawAspect, rSize);
}
BOOL COleServerItem::OnGetExtent(DVASPECT nDrawAspect, CSize& rSize) {
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    ServerItemState* state = GetServerItemState(this, false);
    if (!state || !state->hasExtent) {
        rSize.cx = 0;
        rSize.cy = 0;
        return TRUE;
    }
    rSize.cx = state->contentExtent.cx;
    rSize.cy = state->contentExtent.cy;
    return TRUE;
}
BOOL COleServerItem::OnSetExtent(DVASPECT nDrawAspect, const CSize& size) {
    if (nDrawAspect != DVASPECT_CONTENT && nDrawAspect != DVASPECT_ICON) return FALSE;
    ServerItemState* state = GetServerItemState(this, true);
    if (!state) return FALSE;
    state->contentExtent.cx = size.cx;
    state->contentExtent.cy = size.cy;
    state->hasExtent = TRUE;
    return TRUE;
}
void COleServerItem::Serialize(CArchive& ar) {
    CDocItem::Serialize(ar);
}
COleDataSource* COleServerItem::GetDataSource() {
    ServerItemState* state = GetServerItemState(this, true);
    if (!state) return nullptr;
    if (!state->dataSource) state->dataSource = new COleDataSource();
    return state->dataSource;
}
void COleServerItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
    COleDataSource* dataSource = GetDataSource();
    if (dataSource) dataSource->SetClipboard();
}
void COleServerItem::NotifyChanged() {
    if (m_pServerDoc) m_pServerDoc->NotifyChanged();
}
BOOL COleServerItem::IsConnected() const {
    return m_pServerDoc && m_pServerDoc->IsRegistered();
}
COleServerDoc* COleServerItem::GetDocument() const {
    return m_pServerDoc;
}
// COleServerItem::AddOtherClipboardData(COleDataSource*) -- retail (0x269840),
// re-read instruction by instruction:
//     pdo = this->GetInterface(&IID_IDataObject);            // 0x26bc00
//     if (pdo->EnumFormatEtc(DATADIR_GET, &penum) != S_OK) return;  // slot 8
//     while (penum->Next(1, &fmt, NULL) == S_OK) {           // IEnum slot 3
//         STGMEDIUM medium;
//         if (pdo->GetData(&fmt, &medium) == S_OK &&         // slot 3
//             medium.pUnkForRelease == NULL) {
//             e = pDataSource->GetCacheEntry(&fmt, DATADIR_GET);  // 0x252260
//             e->stgMedium = medium;                         // entry+0x20/+0x30
//         } else {
//             if (GetData succeeded) ReleaseStgMedium(&medium);
//             CoTaskMemFree(fmt.ptd);                        // and skip it
//         }
//     }
//     penum->Release();
// NOTE: an earlier revision of this comment claimed the loop branched on
// `fmt.ptd != NULL`. It does not -- the discriminator is whether GetData
// succeeded AND handed back a medium with a NULL pUnkForRelease
// (`cmpq $0x0,-0x30(%rbp)` at 0x2698a3 is STGMEDIUM.pUnkForRelease, not
// FORMATETC.ptd). CoTaskMemFree(fmt.ptd) is what the *reject* path does with
// the target device the enumerator allocated, not the test.
// Left a stub: it needs COleServerItem's own IDataObject (retail's nested
// m_xDataObject at +0xd8), which OpenMFC's COleServerItem does not have, and it
// writes raw FORMATETCs into COleDataSource's cache through a retail-internal
// entry allocator whose OpenMFC counterpart takes a different shape.
// Symbol: ?AddOtherClipboardData@COleServerItem@@QEAAXPEAVCOleDataSource@@@Z
extern "C" void MS_ABI impl__AddOtherClipboardData_COleServerItem__QEAAXPEAVCOleDataSource___Z(void* /*class*/* p0) {}

// COleServerItem::DoDragDrop(LPCRECT, CPoint, BOOL, DWORD, LPCRECT) -- retail
// (0x269510):
//     size = { lpItemRect->right-left, lpItemRect->bottom-top };
//     pDataSource = OnGetClipboardData(bIncludeLink, &ptOffset, &size); // slot 30
//     pdo = pDataSource->GetInterface(&IID_IDataObject);
//     fmt = { cfLinkSource, NULL, DVASPECT_CONTENT, -1, -1 };
//     if (pdo->QueryGetData(&fmt) == S_OK) dwEffects |= DROPEFFECT_LINK;
//         // 0x2695d3 ors in DROPEFFECT_LINK and 0x2695d8 `cmovne` takes it
//         // back when QueryGetData returned non-zero: the link effect is added
//         // when the link format IS available. An earlier revision of this
//         // comment had this test inverted.
//     ... then COleDataSource::DoDragDrop over the start rect.
// Left a stub: it depends on OnGetClipboardData actually producing populated
// clipboard formats (the producers behind it are stubs here, see
// GetClipboardData below) and on a drag loop this file has no access to.
// Symbol: ?DoDragDrop@COleServerItem@@QEAAKPEBUtagRECT@@VCPoint@@HK0@Z
extern "C" unsigned long MS_ABI impl__DoDragDrop_COleServerItem__QEAAKPEBUtagRECT__VCPoint__HK0_Z(const void* /*struct*/* p0, void* /*class*/ p1, int p2, unsigned long p3, const void* /*struct*/* p4) {
    return 0;
}

// COleServerItem::GetClipboardData(COleDataSource*, BOOL, LPPOINT, LPSIZE) --
// retail (0x269650), transcribed:
//     STGMEDIUM stgMedium;
//     GetEmbedSourceData(&stgMedium);                          // 0x269730
//     pDataSource->CacheData(cfEmbedSource, &stgMedium);       // 0x252400
//     GetObjectDescriptorData(lpOffset, lpSize, &stgMedium);   // 0x269aa0
//     pDataSource->CacheData(cfObjectDescriptor, &stgMedium);
//     AddOtherClipboardData(pDataSource);                      // 0x269840
//     if (bIncludeLink && GetLinkSourceData(&stgMedium)) {     // 0x269990
//         pDataSource->CacheData(cfLinkSource, &stgMedium);
//         GetObjectDescriptorData(lpOffset, lpSize, &stgMedium);
//         pDataSource->CacheData(cfLinkSrcDescriptor, &stgMedium);
//     }
// Left a stub because all four data producers it calls are themselves stubs
// here: caching the STGMEDIUMs they leave untouched would put uninitialised
// handles into the data source and, from there, onto the clipboard.
// Symbol: ?GetClipboardData@COleServerItem@@QEAAXPEAVCOleDataSource@@HPEAUtagPOINT@@PEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__GetClipboardData_COleServerItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
    COleServerItem* pThis, COleDataSource* pDataSource, int bIncludeLink,
    POINT* lpOffset, SIZE* lpSize)
{
    (void)pThis; (void)pDataSource; (void)bIncludeLink; (void)lpOffset; (void)lpSize;
}

// COleServerItem::GetDataObject() -- retail (0x268f90) is a two-instruction
// tail call:
//     lea  rdx, IID_IDataObject ; jmp CCmdTarget::GetInterface   (0x26bc00)
// i.e. it hands out the item's own nested IDataObject (retail's m_xDataObject
// at +0xd8). Its five data-transfer methods are pure forwarders onto the item's
// embedded data source: XDataObject::GetData / GetDataHere / QueryGetData /
// SetData / EnumFormatEtc (0x26a3f0, 0x26a410, 0x26a430, 0x26a450, 0x26a470)
// each do `rcx -= 0x10` -- landing on m_dataSource's own IDataObject at +0xc8
// -- and reach the same vtable slot on it. The other four are NOT forwarders:
// GetCanonicalFormatEtc is a constant, and DAdvise / DUnadvise / EnumDAdvise
// drive the item's own m_lpDataAdviseHolder. (An earlier revision of this
// comment said "every method" was a forwarder.)
// Left a stub: OpenMFC's CCmdTarget::GetInterface is itself an unimplemented
// stub (core/runtime/CCmdTarget.cpp) and OpenMFC's COleServerItem has no nested
// IDataObject. The natural substitute -- handing out the forwarding target,
// GetDataSource()->GetInterface() -- is not reachable from here: COleDataSource
// ::GetInterface has a real definition in core/ole/COleDataSource.cpp but no
// extern "C" export thunk, and this DLL's linkage rule forbids calling the C++
// method directly. See headerRequests.
// Symbol: ?GetDataObject@COleServerItem@@QEAAPEAUIDataObject@@XZ
extern "C" void* MS_ABI impl__GetDataObject_COleServerItem__QEAAPEAUIDataObject__XZ(
    COleServerItem* pThis)
{
    (void)pThis;
    return nullptr;
}

// COleServerItem::GetEmbedSourceData(LPSTGMEDIUM) -- retail 0x269730.
// Left a stub: its imports (verified against the DLL import table:
// CreateILockBytesOnHGlobal @0x2c5a58, StgCreateDocfileOnILockBytes @0x2c5a50)
// show it creating a docfile on an ILockBytes, saving the item into it and
// returning tymed 8 == TYMED_ISTORAGE (`movl $0x8,(%rsi)` at 0x2697ea).
// OpenMFC models neither the item's IPersistStorage nor a compound-file save
// path, and
// a partial version would hand callers an STGMEDIUM that is not a valid
// storage. Its one caller here (GetClipboardData) is stubbed for the same
// reason, so nothing in this file reads an uninitialised medium.
// Symbol: ?GetEmbedSourceData@COleServerItem@@QEAAXPEAUtagSTGMEDIUM@@@Z
extern "C" void MS_ABI impl__GetEmbedSourceData_COleServerItem__QEAAXPEAUtagSTGMEDIUM___Z(void* /*struct*/* p0) {}

// COleServerItem::GetLinkSourceData(LPSTGMEDIUM) -- retail 0x269990. It asks
// the item for its OLEWHICHMK_OBJFULL moniker and marshals the class id and the
// moniker into a stream (imports verified: CreateStreamOnHGlobal @0x2c5ab8,
// WriteClassStm @0x2c5958, OleSaveToStream @0x2c5a68), returning tymed 4 ==
// TYMED_ISTREAM (`movl $0x4,(%rbx)` at 0x269a3a).
// Left a stub: COleServerItem::GetMoniker below is
// itself stubbed (no nested IOleObject to ask), so there is no moniker to
// serialise and the medium would be returned uninitialised.
// Symbol: ?GetLinkSourceData@COleServerItem@@QEAAHPEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__GetLinkSourceData_COleServerItem__QEAAHPEAUtagSTGMEDIUM___Z(void* /*struct*/* p0) {
    return 0;
}

// COleServerItem::GetMetafileData(LPFORMATETC, LPSTGMEDIUM) -- retail 0x269210.
// Transcribed far enough to see what it needs: it requires
// lpFormatEtc->tymed & TYMED_MFPICT and lpStgMedium->hGlobal == NULL, creates a
// metafile DC (CreateMetaFileA @0x2c42a0, CMetaFileDC + CDC::Attach), calls
// the item's OnDrawEx virtual -- slot 25 / 0xc8, called with three arguments
// (pDC, lpFormatEtc->dwAspect, &size) at 0x2692a6; slot 25 is OnDrawEx, NOT
// OnDraw, which is the pure virtual one slot earlier (0xb8) -- into it, closes
// the metafile, allocates a moveable METAFILEPICT in
// GMEM_DDESHARE|GMEM_MOVEABLE (0x2002, 0x18 bytes), sets mm = MM_ANISOTROPIC,
// fills xExt/yExt from the extent the OnGetExtent virtual (slot 27, 0xd8)
// returns when OnDrawEx left it at (0,0), negating a negative yExt, and
// returns tymed 0x20 == TYMED_MFPICT.
// Left a stub: OpenMFC has no metafile-DC rendering backend (COleServerItem::
// OnDrawEx here explicitly documents itself as a lifecycle-only fallback), so
// the metafile handed back would be empty.
// Symbol: ?GetMetafileData@COleServerItem@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__GetMetafileData_COleServerItem__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    COleServerItem* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium)
{
    (void)pThis; (void)lpFormatEtc; (void)lpStgMedium;
    return 0;
}

// COleServerItem::GetMoniker(OLEGETMONIKER) -- retail (0x269940), transcribed:
//     LPMONIKER pmk = NULL;
//     this->GetInterface(&IID_IOleObject)
//         ->GetMoniker(nAssign, OLEWHICHMK_OBJFULL, &pmk);   // slot 8
//     return pmk;
// That nested IOleObject (retail m_xOleObject at +0xd0) is what OpenMFC's
// COleServerItem lacks, and its GetMoniker (0x269d00) in turn walks
// m_pDocument's GetMoniker virtual and CreateItemMoniker over m_strItemName --
// neither the item name nor the document moniker hookup is modeled here.
// Left a stub returning NULL, which is also what retail returns when the
// nested object cannot produce a moniker.
// Symbol: ?GetMoniker@COleServerItem@@QEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z
extern "C" void* MS_ABI impl__GetMoniker_COleServerItem__QEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(
    COleServerItem* pThis, int /*OLEGETMONIKER*/ nAssign)
{
    (void)pThis; (void)nAssign;
    return nullptr;
}

// COleServerItem::GetObjectDescriptorData(LPPOINT, LPSIZE, LPSTGMEDIUM) --
// retail 0x269aa0, as actually disassembled: it fetches the item's own
// IOleObject (GetInterface(&IID_IOleObject) at 0x269ac5), packs the caller's
// lpOffset/lpSize into a scratch slot, brackets an InternalAddRef/Release pair
// around a shared descriptor-building helper at 0x25eb44 (not itself an
// exported symbol) and returns its HGLOBAL as tymed 1 == TYMED_HGLOBAL with
// pUnkForRelease NULL. What that helper puts in the OBJECTDESCRIPTOR was not
// disassembled and is not asserted here. Left a stub: the entry point it hangs
// off -- the nested IOleObject reached through CCmdTarget::GetInterface -- is
// what OpenMFC's COleServerItem does not have (see GetMoniker above).
// Symbol: ?GetObjectDescriptorData@COleServerItem@@QEAAXPEAUtagPOINT@@PEAUtagSIZE@@PEAUtagSTGMEDIUM@@@Z
extern "C" void MS_ABI impl__GetObjectDescriptorData_COleServerItem__QEAAXPEAUtagPOINT__PEAUtagSIZE__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2) {}

// COleServerItem::GetOleObject() -- retail (0x268fa0) is a two-instruction tail
// call, exactly parallel to GetDataObject:
//     lea rdx, IID_IOleObject ; jmp CCmdTarget::GetInterface   (0x26bc00)
// Unlike GetDataObject there is no OpenMFC object standing in for the nested
// IOleObject (retail's m_xOleObject at +0xd0; its methods are the
// XOleObject::* exports further down this file, all stubs for the same
// reason), so this stays a stub returning NULL.
// Symbol: ?GetOleObject@COleServerItem@@QEAAPEAUIOleObject@@XZ
extern "C" void* MS_ABI impl__GetOleObject_COleServerItem__QEAAPEAUIOleObject__XZ(
    COleServerItem* pThis)
{
    (void)pThis;
    return nullptr;
}

// COleServerItem::IsBlank() const -- the mangled name is absent from
// mfc140_rva_symbols.json, but the body is recoverable from the vtable and was
// read there: CDocItem declares exactly one virtual of its own (IsBlank) after
// CCmdTarget's 22 slots, so it occupies slot 22 (0xb0). CDocItem's own vtable
// (RVA 0x32c520) has 0x7260 = `xor eax,eax; ret` there -- CDocItem::IsBlank
// returns FALSE -- while CDocObjectServerItem's (RVA 0x32d048), which inherits
// COleServerItem's override, has 0x3ae0 = `mov $0x1,%eax; ret`. So
// COleServerItem::IsBlank() returns TRUE unconditionally: it touches no member
// and takes no branch. (0x3ae0 carries the exported name
// ?OnEraseBkgnd@CPaneTrackingWnd@@IEAAHPEAVCDC@@@Z because the retail linker
// folded every identical `return 1` body together; that fold is why IsBlank has
// no RVA of its own.) Transcribed exactly.
// Symbol: ?IsBlank@COleServerItem@@MEBAHXZ
extern "C" int MS_ABI impl__IsBlank_COleServerItem__MEBAHXZ(const COleServerItem* pThis) {
    (void)pThis;
    return TRUE;
}

// COleServerItem::NotifyClient(OLE_NOTIFICATION, DWORD_PTR) -- retail
// (0x268ef0), fully transcribed:
//     switch (wNotification) {
//     case 0 /*OLE_CHANGED*/:
//         if (m_lpDataAdviseHolder@0x68)
//             m_lpDataAdviseHolder->SendOnDataChange(               // slot 6
//                 this->GetInterface(&IID_IDataObject), 0, 0);
//         break;
//     case 1 /*OLE_SAVED*/:
//         if (m_lpOleAdviseHolder@0x60) m_lpOleAdviseHolder->SendOnSave();   // slot 7
//         break;
//     case 2 /*OLE_CLOSED*/:
//         if (m_lpOleAdviseHolder) m_lpOleAdviseHolder->SendOnClose();       // slot 8
//         break;
//     case 3 /*OLE_RENAMED*/:
//         if (m_lpOleAdviseHolder) m_lpOleAdviseHolder->SendOnRename(        // slot 6
//             (LPMONIKER)dwParam);
//         break;
//     }
// (The slot numbers are IOleAdviseHolder's / IDataAdviseHolder's, and the
// enumerator values are retail's, not openmfc/afxole.h's OLE_* macros.)
// Left a stub: OpenMFC's COleServerItem has neither m_lpOleAdviseHolder nor
// m_lpDataAdviseHolder, and there is no side table standing in for them, so
// there is nothing to notify. See headerRequests.
// Symbol: ?NotifyClient@COleServerItem@@QEAAXW4OLE_NOTIFICATION@@_K@Z
extern "C" void MS_ABI impl__NotifyClient_COleServerItem__QEAAXW4OLE_NOTIFICATION___K_Z(
    COleServerItem* pThis, unsigned int wNotification, ULONG_PTR dwParam)
{
    (void)pThis; (void)wNotification; (void)dwParam;
}

// COleServerItem::OnDoVerb(LONG) -- retail (0x2690d0), transcribed from the
// jump table:
//     iVerb == -1 (OLEIVERB_SHOW) or 0 (OLEIVERB_PRIMARY) -> slot 33 OnShow
//     iVerb == -2 (OLEIVERB_OPEN) or 1                    -> slot 34 OnOpen
//     iVerb == -3 (OLEIVERB_HIDE) or 2                    -> slot 35 OnHide
//     iVerb <  0  (any other)  -> AfxThrowOleException(E_NOTIMPL)
//     iVerb >= 0  (any other)  -> call vtable slot 28 with argument 0, then
//                                 AfxThrowOleException(0x00040180
//                                 == OLEOBJ_S_INVALIDVERB)
// Slot 28 is OnDoVerb itself: XOleObject::DoVerb (0x269f40) calls vtable slot
// 0xe0 with its iVerb argument. So the last branch is a re-entry with iVerb 0,
// the primary verb, which the first case arm above routes to OnShow --
// transcribed here as a direct OnShow() call rather than a recursive call.
// Dispatch is direct rather than virtual: OpenMFC does not declare OnShow /
// OnOpen / OnHide as virtuals, so a derived override is not picked up.
// Symbol: ?OnDoVerb@COleServerItem@@UEAAXJ@Z
extern "C" void MS_ABI impl__OnDoVerb_COleServerItem__UEAAXJ_Z(
    COleServerItem* pThis, long iVerb)
{
    if (!pThis) return;
    switch (iVerb) {
    case OLEIVERB_PRIMARY:      // 0
    case OLEIVERB_SHOW:         // -1
        impl__OnShow_COleServerItem__MEAAXXZ(pThis);
        return;
    case OLEIVERB_OPEN:         // -2
    case 1:
        impl__OnOpen_COleServerItem__MEAAXXZ(pThis);
        return;
    case OLEIVERB_HIDE:         // -3
    case 2:
        impl__OnHide_COleServerItem__MEAAXXZ(pThis);
        return;
    default:
        break;
    }
    if (iVerb < 0) {
        // Retail follows this call with int3: it never returns. The explicit
        // return keeps the two arms separate regardless.
        impl__AfxThrowOleException__YAXJ_Z(E_NOTIMPL);
        return;
    }
    impl__OnShow_COleServerItem__MEAAXXZ(pThis);
    impl__AfxThrowOleException__YAXJ_Z(OLEOBJ_S_INVALIDVERB);
}

// COleServerItem::OnFinalRelease() -- retail (0x269c70), transcribed:
//     COleServerDoc* pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();  // 0x225b80, then int3
//     InterlockedIncrement(&pDoc->m_dwRef@0x8);   // CCmdTarget::InternalAddRef
//     if (m_bNeedUnlock@0x50) {
//         CoLockObjectExternal(pDoc->GetInterface(&IID_IUnknown), FALSE, TRUE);
//         m_bNeedUnlock = FALSE;
//     }
//     if (m_bAutoDelete@0x54) delete this;               // vtable slot 1, arg 1
//     pDoc->InternalRelease();                    // tail call to 0x26bb70
// Only the m_bAutoDelete half is reproduced. OpenMFC models neither
// m_bNeedUnlock nor CCmdTarget's m_dwRef/InternalAddRef/InternalRelease pair,
// so the external lock drop and the document ref-count bracket around the
// delete are omitted; nothing here fakes them.
// Symbol: ?OnFinalRelease@COleServerItem@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COleServerItem__UEAAXXZ(COleServerItem* pThis)
{
    if (!pThis) return;
    if (pThis->m_bAutoDelete)
        delete pThis;
}

// COleServerItem::OnGetClipboardData(BOOL, LPPOINT, LPSIZE) -- retail
// (0x2694a0), transcribed:
//     p = operator new(0x60);                       // sizeof(COleDataSource)
//     pDataSource = p ? new (p) COleDataSource : NULL;
//     GetClipboardData(pDataSource, bIncludeLink, lpOffset, lpSize); // 0x269650
//     return pDataSource;
// The call is bracketed by retail's unwind bookkeeping (the data source is
// spilled to a frame slot and the call is followed by the usual funclet nop),
// i.e. a throw out of GetClipboardData destroys the new data source. That
// unwind cleanup is the one part not reproduced here.
// GetClipboardData is a stub here, so the caller currently gets an empty but
// valid, owned COleDataSource. Note retail makes no non-NULL guarantee: at
// 0x2694c7 it tests operator new's result and returns NULL when the allocation
// failed (`xor %ebx,%ebx` at 0x2694d9). An earlier revision of this comment
// claimed retail guarantees a non-NULL data source; it does not.
// Symbol: ?OnGetClipboardData@COleServerItem@@UEAAPEAVCOleDataSource@@HPEAUtagPOINT@@PEAUtagSIZE@@@Z
extern "C" void* MS_ABI impl__OnGetClipboardData_COleServerItem__UEAAPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
    COleServerItem* pThis, int bIncludeLink, POINT* lpOffset, SIZE* lpSize)
{
    if (!pThis) return nullptr;
    COleDataSource* pDataSource = new COleDataSource();
    impl__GetClipboardData_COleServerItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
        pThis, pDataSource, bIncludeLink, lpOffset, lpSize);
    return pDataSource;
}

// COleServerItem::OnHide() -- retail (0x2691e0), transcribed in full:
//     pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();  // 0x225b80, then int3
//     pDoc-><document virtual at slot 91 (0x2d8)>(0);
// Slot 91 is ?OnShowDocument@COleServerDoc@@MEAAXH@Z (RVA 0x265540). That is
// now established from a vtable dump, not inferred: COleServerDoc's vtable at
// RVA 0x2f0100 holds 0x265540 at byte offset 0x2d8. OnOpen passes 1 and OnHide
// passes 0. (An earlier revision of this comment said the identification rested
// on the signature and argument shape alone; it no longer does.)
// Symbol: ?OnHide@COleServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnHide_COleServerItem__MEAAXXZ(COleServerItem* pThis)
{
    if (!pThis) return;
    COleServerDoc* pDoc = pThis->GetDocument();
    if (pDoc) pDoc->OnShowDocument(FALSE);
}

// COleServerItem::OnInitFromData(COleDataObject*, BOOL) -- retail (0x269460)
// is three instructions and never returns:
//     mov ecx, 0x80004001 ; call AfxThrowOleException ; int3
// i.e. the base implementation unconditionally throws COleException(E_NOTIMPL);
// a server that wants "insert from data" overrides it. Transcribed as such.
// The trailing `return FALSE` is unreachable in retail (int3 follows the call)
// and exists only because this thunk's C++ signature requires a return.
// Symbol: ?OnInitFromData@COleServerItem@@UEAAHPEAVCOleDataObject@@H@Z
extern "C" int MS_ABI impl__OnInitFromData_COleServerItem__UEAAHPEAVCOleDataObject__H_Z(
    COleServerItem* pThis, COleDataObject* pDataObject, int bCreation)
{
    (void)pThis; (void)pDataObject; (void)bCreation;
    impl__AfxThrowOleException__YAXJ_Z(E_NOTIMPL);
    return 0;
}

// COleServerItem::OnOpen() -- retail (0x2691b0), transcribed in full:
//     pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();  // 0x225b80, then int3
//     pDoc-><document virtual at slot 91 (0x2d8)>(1);   // see OnHide below
// Symbol: ?OnOpen@COleServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnOpen_COleServerItem__MEAAXXZ(COleServerItem* pThis)
{
    if (!pThis) return;
    COleServerDoc* pDoc = pThis->GetDocument();
    if (pDoc) pDoc->OnShowDocument(TRUE);
}

// COleServerItem::OnQueryUpdateItems() -- retail (0x268fb0), transcribed:
//     COleDocument* pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();  // 0x225b80, then int3
//     POSITION pos = pDoc->GetStartPosition();          // vtable slot 71 (0x238)
//     while ((pItem = pDoc->GetNextItem(pos, RUNTIME_CLASS(COleClientItem))))
//         if (pItem->m_lpObject@0x48->IsUpToDate() != S_OK)   // IOleObject slot 14
//             return TRUE;
//     return FALSE;
// OpenMFC spells the enumerator GetStartPosition()/GetNextClientItem(POSITION&)
// (real definitions in core/ole/COleDocument.cpp) and keeps the OLE object in
// the public COleClientItem::m_lpObject, so the loop maps over directly.
// Symbol: ?OnQueryUpdateItems@COleServerItem@@UEAAHXZ
extern "C" int MS_ABI impl__OnQueryUpdateItems_COleServerItem__UEAAHXZ(COleServerItem* pThis)
{
    if (!pThis) return FALSE;
    COleDocument* pDoc = pThis->GetDocument();
    if (!pDoc) return FALSE;
    void* pos = impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(pDoc);
    COleClientItem* pItem;
    while ((pItem = impl__GetNextClientItem_COleDocument__QEBAPEAVCOleClientItem__AEAPEAU__POSITION___Z(
                pDoc, &pos)) != nullptr) {
        if (pItem->m_lpObject != nullptr && pItem->m_lpObject->IsUpToDate() != S_OK)
            return TRUE;
    }
    return FALSE;
}

// COleServerItem::OnRenderData(LPFORMATETC, LPSTGMEDIUM) -- retail (0x269bd0),
// transcribed in full (it is nine instructions):
//     if (lpFormatEtc->lindex == -1 && lpFormatEtc->cfFormat == CF_METAFILEPICT)
//         return GetMetafileData(lpFormatEtc, lpStgMedium);  // vtable slot 41
//     return FALSE;
// GetMetafileData is a stub here (see its comment), so the metafile branch
// currently also returns FALSE; the shape is kept so implementing
// GetMetafileData is all that is needed. Dispatch is direct, not virtual:
// OpenMFC does not declare GetMetafileData on COleServerItem.
// Symbol: ?OnRenderData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    COleServerItem* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpStgMedium)
{
    if (!pThis || !lpFormatEtc) return FALSE;
    if (lpFormatEtc->lindex == -1 && lpFormatEtc->cfFormat == CF_METAFILEPICT)
        return impl__GetMetafileData_COleServerItem__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
            pThis, lpFormatEtc, lpStgMedium);
    return FALSE;
}

// COleServerItem::OnRenderFileData(LPFORMATETC, CFile*) -- the mangled name is
// absent from mfc140_rva_symbols.json, but the body was recovered from the
// vtable: slot 38 (0x130) of CDocObjectServerItem's vtable (RVA 0x32d048) holds
// 0x7260, which disassembles to `xor %eax,%eax; ret`. The retail base
// implementation therefore returns FALSE and touches nothing; the name is
// missing from the map only because the linker folded every identical
// `return 0` body onto that one address. `return 0` below is exact, not a stub.
// Symbol: ?OnRenderFileData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAVCFile___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// COleServerItem::OnRenderGlobalData(LPFORMATETC, HGLOBAL*) -- same story as
// OnRenderFileData above: slot 37 (0x128) of CDocObjectServerItem's vtable
// (RVA 0x32d048) is 0x7260 == `xor %eax,%eax; ret`, so the retail base
// implementation returns FALSE and touches nothing. `return 0` is exact.
// Symbol: ?OnRenderGlobalData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAPEAX_Z(void* /*struct*/* p0, void** p1) {
    return 0;
}

// COleServerItem::OnSaveEmbedding(LPSTORAGE) -- retail (0x269b70),
// transcribed:
//     COleServerDoc* pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();   // 0x225b80, then int3
//     LPSTORAGE lpSaved = pDoc->[0x1c8];      // the doc's root storage
//     pDoc->[0x1c8] = lpStg;
//     pDoc->SaveToStorage(this);              // doc vtable slot 81 (0x288);
//         // slot 81 resolves to ?SaveToStorage@COleLinkingDoc@@MEAAXPEAVCObject@@@Z
//         // in COleServerDoc's vtable (RVA 0x2f0100), read from a vtable dump
//     pDoc->[0x1c8] = lpSaved;                // restored even on the way out
// Left a stub: OpenMFC's COleServerDoc models no root-storage member to swap
// and no corresponding save entry point, so there is nothing to redirect.
// Symbol: ?OnSaveEmbedding@COleServerItem@@MEAAXPEAUIStorage@@@Z
extern "C" void MS_ABI impl__OnSaveEmbedding_COleServerItem__MEAAXPEAUIStorage___Z(void* /*struct*/* p0) {}

// COleServerItem::OnSetColorScheme(const LOGPALETTE*) -- recovered from the
// vtable, not the symbol map: slot 29 (0xe8) of CDocObjectServerItem's vtable
// (RVA 0x32d048) is 0x7260 == `xor %eax,%eax; ret`. Retail's base
// implementation returns FALSE and ignores the palette, matching the documented
// "does nothing unless overridden". `return 0` is exact.
// Symbol: ?OnSetColorScheme@COleServerItem@@UEAAHPEBUtagLOGPALETTE@@@Z
extern "C" int MS_ABI impl__OnSetColorScheme_COleServerItem__UEAAHPEBUtagLOGPALETTE___Z(const void* /*struct*/* p0) {
    return 0;
}

// COleServerItem::OnSetData(LPFORMATETC, LPSTGMEDIUM, BOOL) -- recovered from
// the vtable: slot 40 (0x140) of CDocObjectServerItem's vtable (RVA 0x32d048)
// is 0x7260 == `xor %eax,%eax; ret`. Retail's base implementation returns FALSE
// (it does not release the medium either). `return 0` is exact.
// Symbol: ?OnSetData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// COleServerItem::OnShow() -- retail (0x269170), transcribed in full:
//     pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();  // 0x225b80, then int3
//     if (!pDoc->ActivateInPlace())       // direct call to 0x265b50
//         this->OnOpen();                 // vtable slot 34 (0x110)
// COleServerDoc::ActivateInPlace is reached through its own export thunk
// (implemented in core/ole/COleServerDoc.cpp, where it currently returns FALSE
// because no in-place frame state is modeled) -- so today this always falls
// through to OnOpen, which is retail's non-in-place path.
// Symbol: ?OnShow@COleServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnShow_COleServerItem__MEAAXXZ(COleServerItem* pThis)
{
    if (!pThis) return;
    COleServerDoc* pDoc = pThis->GetDocument();
    if (!pDoc) return;
    if (!impl__ActivateInPlace_COleServerDoc__QEAAHXZ(pDoc))
        impl__OnOpen_COleServerItem__MEAAXXZ(pThis);
}

// COleServerItem::OnUpdate(COleServerItem*, LPARAM, CObject*, DVASPECT) --
// retail (0x264a70) is three instructions:
//     movslq [rsp+0x28], r8   ; r8 = (LONG_PTR)nDrawAspect  (5th arg, on stack)
//     xor    edx, edx         ; edx = 0 == OLE_CHANGED
//     jmp    NotifyClient                                  (0x268ef0)
// i.e. NotifyClient(OLE_CHANGED, nDrawAspect); pSender/lHint/pHint are ignored.
// The 0 is retail's OLE_NOTIFICATION::OLE_CHANGED, not openmfc/afxole.h's
// OLE_CHANGED macro (which is 5 -- see the note at the top of this file).
// NotifyClient is a stub here, so this presently does nothing observable.
// Symbol: ?OnUpdate@COleServerItem@@UEAAXPEAV1@_JPEAVCObject@@W4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__OnUpdate_COleServerItem__UEAAXPEAV1__JPEAVCObject__W4tagDVASPECT___Z(
    COleServerItem* pThis, COleServerItem* pSender, LONG_PTR lHint,
    CObject* pHint, int /*DVASPECT*/ nDrawAspect)
{
    (void)pSender; (void)lHint; (void)pHint;
    if (!pThis) return;
    impl__NotifyClient_COleServerItem__QEAAXW4OLE_NOTIFICATION___K_Z(
        pThis, kRetailOleChanged, static_cast<ULONG_PTR>(static_cast<LONG_PTR>(nDrawAspect)));
}

// COleServerItem::OnUpdateItems() -- retail (0x269020), transcribed:
//     COleDocument* pDoc = m_pDocument@0x40;
//     if (pDoc == NULL) AfxThrowInvalidArgException();  // 0x225b80, then int3
//     POSITION pos = pDoc->GetStartPosition();          // vtable slot 71 (0x238)
//     while ((pItem = pDoc->GetNextItem(pos, RUNTIME_CLASS(COleClientItem))))
//         if (pItem->m_lpObject@0x48->IsUpToDate() != S_OK)  // IOleObject slot 14
//             pItem->m_lpObject->Update();                   // IOleObject slot 13
// Symbol: ?OnUpdateItems@COleServerItem@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateItems_COleServerItem__UEAAXXZ(COleServerItem* pThis)
{
    if (!pThis) return;
    COleDocument* pDoc = pThis->GetDocument();
    if (!pDoc) return;
    void* pos = impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(pDoc);
    COleClientItem* pItem;
    while ((pItem = impl__GetNextClientItem_COleDocument__QEBAPEAVCOleClientItem__AEAPEAU__POSITION___Z(
                pDoc, &pos)) != nullptr) {
        if (pItem->m_lpObject != nullptr && pItem->m_lpObject->IsUpToDate() != S_OK)
            pItem->m_lpObject->Update();
    }
}

//===========================================================================
// COleServerItem::XOleObject / XDataObject -- the item's nested COM
// sub-objects.
//
// Every export below is a method on one of two interface sub-objects embedded
// in the retail COleServerItem: m_xOleObject at +0xd0 and m_xDataObject at
// +0xd8. Each one starts by recovering the owning item with pointer
// arithmetic on its own `this` -- `lea rbx,[rcx-0xd0]` (e.g. XOleObject::
// GetExtent 0x26a1b0, ::DoVerb 0x269f40, ::GetClipboardData 0x269ec0) or
// `rcx-0xd8` (XDataObject::DAdvise 0x26a490) -- and then works through that
// item.
//
// OpenMFC's COleServerItem has no such sub-objects: the class carries
// m_pServerDoc, m_bAutoDelete and an opaque 136-byte padding array, and
// nothing ever writes an interface vptr into it. So there is no `this` for
// these methods to be called on, and the offsets above cannot be applied to an
// OpenMFC item without reading uninitialised padding. Everything in this
// section is therefore left stubbed except the methods that do not touch the
// object at all -- XDataObject::GetCanonicalFormatEtc, XOleObject::
// GetClientSite and XOleObject::Close -- plus the advise-holder methods, whose
// no-holder arm is the only arm OpenMFC can ever reach. Reasons specific to
// individual methods are noted with them; the layout gap itself is reported
// once, in headerRequests.
//
// Where a method's own mangled name is missing from mfc140_rva_symbols.json,
// its body was recovered from the two sub-object vtables instead:
// One trap when editing this section: the generated stubs below were emitted
// WITHOUT a leading `this` parameter, so in an ms_abi function the first
// declared parameter lands in RCX and actually receives the sub-object pointer.
// That is harmless while a body only returns a constant, but any body that
// dereferences a parameter must declare `void* pThis` first or it will write
// through the interface pointer. The bodies here that take arguments do.
//
// COleServerItem's XOleObject vtable is at RVA 0x32edf8 (IOleObject slot order:
// 3 SetClientSite ... 6 Close ... 21 EnumAdvise ... 23 SetColorScheme) and its
// XDataObject vtable at RVA 0x32ecc8 (IDataObject slot order: 3 GetData ...
// 10 DUnadvise, 11 EnumDAdvise).
//===========================================================================

// COleServerItem::XOleObject::Advise(IAdviseSink*, DWORD*) -- retail
// (0x26a240), transcribed:
//     // `lea -0x70(%rcx),%rbx` at 0x26a24f does NOT produce the item: for a
//     // sub-object at item+0xd0 it produces &item->m_lpOleAdviseHolder
//     // (item+0x60) directly, and every later access is through that address.
//     LPOLEADVISEHOLDER* pp = &item->m_lpOleAdviseHolder;   // item+0x60
//     *pdwConnection = 0;
//     if (*pp == NULL && CreateOleAdviseHolder(pp) != S_OK) // ole32 @0x2c5a70
//         return E_OUTOFMEMORY;                             // 0x8007000e
//     return (*pp)->Advise(pAdvSink, pdwConnection);        // holder slot 3
// Stubbed: no nested sub-object and no m_lpOleAdviseHolder member. Note the
// body below returns 0 (S_OK) without writing *pdwConnection -- it cannot do
// better without somewhere to keep a holder, and it is unreachable in practice
// because COleServerItem::GetOleObject() hands out NULL.
// Symbol: ?Advise@XOleObject@COleServerItem@@UEAAJPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XOleObject_COleServerItem__UEAAJPEAUIAdviseSink__PEAK_Z(void* /*struct*/* p0, unsigned long* p1) {
    return 0;
}

// COleServerItem::XOleObject::Close(DWORD) -- the mangled name is absent from
// mfc140_rva_symbols.json, but the body was recovered from the XOleObject
// vtable at RVA 0x32edf8: IOleObject slot 6 (Close) holds 0xf540, which
// disassembles to `mov $0x80004001,%eax; ret` -- E_NOTIMPL, with `this`, the
// item and dwSaveOption all untouched. (Slots 3 SetClientSite, 5 SetHostNames,
// 7 SetMoniker and 9 InitFromData carry the same folded body; the exported name
// on it, ?UpdateUI@CDHtmlDialog@@UEAAJXZ, is just the ICF representative.)
// Closing a *document* is COleServerDoc's job, so the item's IOleObject
// declines. Transcribes exactly without the nested sub-object.
// NOTE: the generated stub returned 0 == S_OK here, which told containers the
// close had succeeded.
// Symbol: ?Close@XOleObject@COleServerItem@@UEAAJK@Z
extern "C" long MS_ABI impl__Close_XOleObject_COleServerItem__UEAAJK_Z(
    void* pThis, unsigned long dwSaveOption)
{
    (void)pThis; (void)dwSaveOption;
    return E_NOTIMPL;
}

// COleServerItem::XDataObject::DAdvise(FORMATETC*, DWORD, IAdviseSink*, DWORD*)
// -- retail (0x26a490), transcribed:
//     if (lpFormatEtc == NULL) return E_INVALIDARG;
//     if (pdwConnection == NULL) return E_POINTER;  *pdwConnection = 0;
//     COleServerItem* pThis = this - 0xd8;
//     FORMATETC fmt = *lpFormatEtc;                 // normalised copy
//     if (fmt.cfFormat == CF_METAFILEPICT && fmt.dwAspect == DVASPECT_DOCPRINT)
//         fmt.dwAspect = DVASPECT_CONTENT;          // 4 -> 1
//     // a wholly-wildcard FORMATETC (cfFormat 0, ptd NULL, aspect/lindex/
//     // tymed all -1) is accepted unconditionally; otherwise the item's own
//     // IDataObject must accept it:
//     if (!wildcard &&
//         pThis->GetInterface(&IID_IDataObject)->QueryGetData(&fmt) != S_OK)
//         return DV_E_FORMATETC;
//     if (pThis->m_lpDataAdviseHolder@0x68 == NULL &&
//         CreateDataAdviseHolder(&pThis->m_lpDataAdviseHolder) != S_OK)
//         return E_OUTOFMEMORY;
//     return pThis->m_lpDataAdviseHolder->DAdvise(
//         (IDataObject*)this, lpFormatEtc, advf, pAdvSink, pdwConnection);
// Stubbed: no nested sub-object and no m_lpDataAdviseHolder member. As with
// XOleObject::Advise, the body below returns 0 (S_OK) without writing
// *pdwConnection and is unreachable in practice (GetDataObject hands out NULL).
// Symbol: ?DAdvise@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(void* /*struct*/* p0, unsigned long p1, void* /*struct*/* p2, unsigned long* p3) {
    return 0;
}

// COleServerItem::XDataObject::DUnadvise(DWORD) -- the mangled name is not in
// mfc140_rva_symbols.json because the retail linker folded this body with
// ?Unadvise@XOleObject@COleServerItem@@UEAAJK@Z, which does carry an RVA:
// slot 10 (DUnadvise) of the XDataObject vtable at RVA 0x32ecc8 and slot 20
// (Unadvise) of the XOleObject vtable at RVA 0x32edf8 both hold 0x26a2a0. The
// fold is exact because each sub-object sits 0x70 above its own holder
// (item+0xd8 - 0x70 == item+0x68 == m_lpDataAdviseHolder; item+0xd0 - 0x70 ==
// item+0x60 == m_lpOleAdviseHolder). The body reads:
//     HOLDER* p = *(this - 0x70);
//     if (p == NULL) return E_FAIL;            // 0x80004005
//     return p->Unadvise(dwConnection);        // holder slot 4
// Only the no-holder arm is reproduced, and it is the only arm OpenMFC can
// reach: nothing in this build ever creates an advise holder (DAdvise, which
// would, is stubbed), so the holder is permanently NULL and E_FAIL is exactly
// what retail returns in that state. If holders are ever added, this needs the
// second arm.
// Symbol: ?DUnadvise@XDataObject@COleServerItem@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleServerItem__UEAAJK_Z(
    void* pThis, unsigned long dwConnection)
{
    (void)pThis; (void)dwConnection;
    return E_FAIL;
}

// COleServerItem::XOleObject::DoVerb(...) -- retail (0x269f40), transcribed:
//     COleServerItem* pThis = this - 0xd0;
//     AFX_MANAGE_STATE(pThis->m_pModuleState@0x38);
//     pThis->InternalAddRef();
//     TRY   { pThis->OnDoVerb(iVerb); hr = S_OK; }    // vtable slot 28 (0xe0)
//     CATCH { hr = the COleException's status code; }
//     pThis->InternalRelease();
//     return hr;
// (lpMsg, pActiveSite, lindex, hwndParent and lprcPosRect are all ignored by
// the base implementation.) Stubbed: no nested sub-object to recover the item
// from.
// Symbol: ?DoVerb@XOleObject@COleServerItem@@UEAAJJPEAUtagMSG@@PEAUIOleClientSite@@JPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" long MS_ABI impl__DoVerb_XOleObject_COleServerItem__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(long p0, void* /*struct*/* p1, void* /*struct*/* p2, long p3, void* /*struct*/* p4, const void* /*struct*/* p5) {
    return 0;
}

// COleServerItem::XOleObject::EnumAdvise(IEnumSTATDATA**) -- the mangled name
// is absent from mfc140_rva_symbols.json because the retail linker folded this
// body with XDataObject::EnumDAdvise, which does carry an RVA: slot 21
// (EnumAdvise) of the XOleObject vtable at RVA 0x32edf8 and slot 11
// (EnumDAdvise) of the XDataObject vtable at RVA 0x32ecc8 both hold 0x26a2c0.
// The fold is exact for the same -0x70 reason as DUnadvise above -- here it
// lands on m_lpOleAdviseHolder (item+0xd0 - 0x70 == item+0x60). The body:
//     *ppenumAdvise = NULL;
//     HOLDER* p = *(this - 0x70);
//     if (p == NULL) return E_FAIL;            // 0x80004005
//     return p->EnumAdvise(ppenumAdvise);      // holder slot 5
// Only the no-holder arm is reproduced; it is the only arm OpenMFC can reach
// (nothing here ever creates an advise holder), so it is exact for this build.
// Symbol: ?EnumAdvise@XOleObject@COleServerItem@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_COleServerItem__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppenumAdvise)
{
    (void)pThis;
    if (ppenumAdvise) *ppenumAdvise = nullptr;
    return E_FAIL;
}

// COleServerItem::XDataObject::EnumDAdvise(IEnumSTATDATA**) -- retail
// (0x26a2c0), transcribed in full:
//     *ppenumAdvise = NULL;
//     IDataAdviseHolder* p = *(this - 0x70);   // item+0x68, m_lpDataAdviseHolder
//     if (p == NULL) return E_FAIL;            // 0x80004005
//     return p->EnumAdvise(ppenumAdvise);      // holder slot 5
// Only the no-holder arm is reproduced -- it is the only arm OpenMFC can reach,
// since DAdvise (which would create the holder) is stubbed and the holder is
// therefore permanently NULL. The generated stub returned 0 == S_OK while
// leaving *ppenumAdvise unwritten, which would have handed a caller an
// uninitialised enumerator pointer.
// Symbol: ?EnumDAdvise@XDataObject@COleServerItem@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleServerItem__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppenumAdvise)
{
    (void)pThis;
    if (ppenumAdvise) *ppenumAdvise = nullptr;
    return E_FAIL;
}

// COleServerItem::XDataObject::EnumFormatEtc -- retail (0x26a470) is four
// instructions: `rcx -= 0x10` then a tail jump through vtable slot 8. That
// -0x10 walks from the item's m_xDataObject (+0xd8) back to the embedded data
// source's own IDataObject (+0xc8), i.e. the whole method is a forward to
// COleDataSource's implementation. GetData (0x26a3f0, slot 3), GetDataHere
// (0x26a410, slot 4) and QueryGetData (0x26a430, slot 5) are the same four
// instructions with a different slot; SetData (0x26a450, slot 7) is the same
// forward but keeps a stack frame (`sub $0x38,%rsp` ... `call` ... `ret`)
// because it has a fifth argument, so it is five instructions, not four.
// Stubbed: OpenMFC's COleServerItem has neither sub-object, so there is no
// `this` to walk back from. (COleServerItem::GetDataSource() here holds the
// object the forward lands on -- see GetDataObject above.)
// Symbol: ?EnumFormatEtc@XDataObject@COleServerItem@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleServerItem__UEAAJKPEAPEAUIEnumFORMATETC___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// COleServerItem::XOleObject::EnumVerbs(IEnumOLEVERB**) -- retail (0x269fc0),
// transcribed:
//     *ppenumOleVerb = NULL;
//     CLSID clsid;
//     ((IOleObject*)(this - 0xd0)->GetInterface(&IID_IOleObject))
//         ->GetUserClassID(&clsid);        // IOleObject slot 15
//     return OleRegEnumVerbs(clsid, ppenumOleVerb);
// Stubbed: no nested sub-object, and CCmdTarget::GetInterface is unimplemented
// in OpenMFC, so there is no route to the item's CLSID here.
// Symbol: ?EnumVerbs@XOleObject@COleServerItem@@UEAAJPEAPEAUIEnumOLEVERB@@@Z
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_COleServerItem__UEAAJPEAPEAUIEnumOLEVERB___Z(void* /*struct*/** p0) {
    return 0;
}

// COleServerItem::XDataObject::GetCanonicalFormatEtc -- retail (0x252d30) is
// two instructions and touches nothing:
//     mov eax, 0x40130 ; ret        // 0x00040130 == DATA_S_SAMEFORMATETC
// Neither `this` nor either FORMATETC is read, so this one transcribes exactly
// even without the nested sub-object.
// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__0_Z(
    void* pThis, FORMATETC* lpFormatEtcIn, FORMATETC* lpFormatEtcOut)
{
    (void)pThis; (void)lpFormatEtcIn; (void)lpFormatEtcOut;
    return DATA_S_SAMEFORMATETC;
}

// COleServerItem::XOleObject::GetClientSite(IOleClientSite**) -- retail
// (0x269cf0) is three instructions and never reads `this`:
//     mov qword ptr [rdx], 0 ; mov eax, 0x80004001 ; ret   // E_NOTIMPL
// A server item has no client site of its own, so the base implementation
// declines. Transcribes exactly without the nested sub-object.
// Symbol: ?GetClientSite@XOleObject@COleServerItem@@UEAAJPEAPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__GetClientSite_XOleObject_COleServerItem__UEAAJPEAPEAUIOleClientSite___Z(
    void* pThis, void** ppClientSite)
{
    (void)pThis;
    if (ppClientSite) *ppClientSite = nullptr;
    return E_NOTIMPL;
}

// COleServerItem::XOleObject::GetClipboardData(DWORD, IDataObject**) -- retail
// (0x269ec0), transcribed:
//     COleServerItem* pThis = this - 0xd0;
//     AFX_MANAGE_STATE(pThis->m_pModuleState@0x38);
//     *ppDataObject = NULL;
//     COleDataSource* pSource =
//         pThis->OnGetClipboardData(TRUE, NULL, NULL);      // vtable slot 30
//     *ppDataObject = pSource->GetInterface(&IID_IDataObject);
//     return S_OK;                                          // unconditionally
// (dwReserved is ignored.) Stubbed: no nested sub-object to recover the item.
// Symbol: ?GetClipboardData@XOleObject@COleServerItem@@UEAAJKPEAPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_COleServerItem__UEAAJKPEAPEAUIDataObject___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// COleServerItem::XDataObject::GetData -- retail (0x26a3f0): `rcx -= 0x10`
// then tail-jump through vtable slot 3, i.e. a forward to the embedded data
// source's IDataObject (see EnumFormatEtc above). Stubbed for the same reason.
// Symbol: ?GetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// COleServerItem::XDataObject::GetDataHere -- retail (0x26a410): `rcx -= 0x10`
// then tail-jump through vtable slot 4 -- the same forward to the embedded data
// source. Stubbed for the same reason.
// Symbol: ?GetDataHere@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// COleServerItem::XOleObject::GetExtent(DWORD, SIZEL*) -- retail (0x26a1b0),
// transcribed:
//     COleServerItem* pThis = this - 0xd0;
//     AFX_MANAGE_STATE(pThis->m_pModuleState@0x38);
//     HRESULT hr = E_INVALIDARG;
//     CSize size(0, 0);
//     if (pThis->OnGetExtent((DVASPECT)dwDrawAspect, size)) {   // slot 27 (0xd8)
//         if (size.cy < 0) size.cy = -size.cy;
//         *lpsizel = size;
//         hr = S_OK;
//     }
//     return hr;
// Stubbed: no nested sub-object to recover the item from. (OpenMFC's
// COleServerItem::OnGetExtent, defined at the top of this file, is the call
// this would land on.)
// Symbol: ?GetExtent@XOleObject@COleServerItem@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetExtent_XOleObject_COleServerItem__UEAAJKPEAUtagSIZE___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// COleServerItem::XOleObject::GetMiscStatus(DWORD, DWORD*) -- retail
// (0x26a2f0), transcribed:
//     COleServerItem* pThis = this - 0xd0;
//     *pdwStatus = 0;
//     CLSID clsid;
//     ((IOleObject*)pThis->GetInterface(&IID_IOleObject))
//         ->GetUserClassID(&clsid);            // IOleObject slot 15
//     return OleRegGetMiscStatus(clsid, dwAspect, pdwStatus);
// Stubbed for the same reasons as EnumVerbs above.
// Symbol: ?GetMiscStatus@XOleObject@COleServerItem@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_COleServerItem__UEAAJKPEAK_Z(unsigned long p0, unsigned long* p1) {
    return 0;
}

// COleServerItem::XOleObject::GetMoniker(DWORD, DWORD, IMoniker**) -- retail
// (0x269d00), transcribed:
//     COleServerItem* pThis = this - 0xd0;
//     AFX_MANAGE_STATE(pThis->m_pModuleState@0x38);
//     if (pThis->m_pDocument@0x40 == NULL) AfxThrowInvalidArgException();
//     *ppmk = NULL;
//     switch (dwWhichMoniker) {
//     case OLEWHICHMK_CONTAINER:                       // 1
//         *ppmk = pDoc->GetMoniker(dwAssign);          // doc vtable slot 78
//         break;
//     case OLEWHICHMK_OBJREL:                          // 2
//         if (pThis->m_strItemName@0x58 is empty) break;
//         pDoc->GetMoniker(dwAssign)->Release();       // force assignment
//         if (dwAssign is 1, 2 or 4)
//             CreateItemMoniker(L"\\", m_strItemName, ppmk);
//             // the delimiter operand at 0x269e2b is the wide string at RVA
//             // 0x33d594, whose bytes are 5C 00 00 00 -- L"\\", not L"!".
//             // An earlier revision of this comment had it as L"!".
//         break;
//     case OLEWHICHMK_OBJFULL:                         // 3
//         mkContainer = pThis->GetMoniker(OLEWHICHMK_CONTAINER-equivalent);
//         mkRel       = pThis->GetMoniker(OLEWHICHMK_OBJREL-equivalent);
//         if (mkContainer)
//             *ppmk = mkRel ? CreateGenericComposite(mkContainer, mkRel, ppmk)
//                           : mkContainer;
//         break;
//     }
//     return *ppmk ? S_OK : E_FAIL;
// Stubbed: no nested sub-object, and OpenMFC's COleServerItem models no item
// name (retail m_strItemName at +0x58) for the relative moniker.
// Symbol: ?GetMoniker@XOleObject@COleServerItem@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleObject_COleServerItem__UEAAJKKPEAPEAUIMoniker___Z(unsigned long p0, unsigned long p1, void* /*struct*/** p2) {
    return 0;
}

// Symbol: ?GetUserClassID@XOleObject@COleServerItem@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetUserClassID_XOleObject_COleServerItem__UEAAJPEAU_GUID___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetUserType@XOleObject@COleServerItem@@UEAAJKPEAPEA_W@Z
extern "C" long MS_ABI impl__GetUserType_XOleObject_COleServerItem__UEAAJKPEAPEA_W_Z(unsigned long p0, wchar_t** p1) {
    return 0;
}

// Symbol: ?InitFromData@XOleObject@COleServerItem@@UEAAJPEAUIDataObject@@HK@Z
extern "C" long MS_ABI impl__InitFromData_XOleObject_COleServerItem__UEAAJPEAUIDataObject__HK_Z(void* /*struct*/* p0, int p1, unsigned long p2) {
    return 0;
}

// Symbol: ?IsUpToDate@XOleObject@COleServerItem@@UEAAJXZ
extern "C" long MS_ABI impl__IsUpToDate_XOleObject_COleServerItem__UEAAJXZ() {
    return 0;
}

// Symbol: ?OnRenderData@CItemDataSource@COleServerItem@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_CItemDataSource_COleServerItem__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnRenderFileData@CItemDataSource@COleServerItem@@MEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_CItemDataSource_COleServerItem__MEAAHPEAUtagFORMATETC__PEAVCFile___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnRenderGlobalData@CItemDataSource@COleServerItem@@MEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_CItemDataSource_COleServerItem__MEAAHPEAUtagFORMATETC__PEAPEAX_Z(void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?OnSetData@CItemDataSource@COleServerItem@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_CItemDataSource_COleServerItem__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetClientSite@XOleObject@COleServerItem@@UEAAJPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__SetClientSite_XOleObject_COleServerItem__UEAAJPEAUIOleClientSite___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetColorScheme@XOleObject@COleServerItem@@UEAAJPEAUtagLOGPALETTE@@@Z
extern "C" long MS_ABI impl__SetColorScheme_XOleObject_COleServerItem__UEAAJPEAUtagLOGPALETTE___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?SetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?SetExtent@XOleObject@COleServerItem@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__SetExtent_XOleObject_COleServerItem__UEAAJKPEAUtagSIZE___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?SetHostNames@XOleObject@COleServerItem@@UEAAJPEB_W0@Z
extern "C" long MS_ABI impl__SetHostNames_XOleObject_COleServerItem__UEAAJPEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {
    return 0;
}

// Symbol: ?SetMoniker@XOleObject@COleServerItem@@UEAAJKPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__SetMoniker_XOleObject_COleServerItem__UEAAJKPEAUIMoniker___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?Unadvise@XOleObject@COleServerItem@@UEAAJK@Z
extern "C" long MS_ABI impl__Unadvise_XOleObject_COleServerItem__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?Update@XOleObject@COleServerItem@@UEAAJXZ
extern "C" long MS_ABI impl__Update_XOleObject_COleServerItem__UEAAJXZ() {
    return 0;
}
