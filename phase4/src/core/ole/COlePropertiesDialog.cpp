// COlePropertiesDialog — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// -----------------------------------------------------------------------------
// XOleUIObjInfo (IOleUIObjInfoW) support.
//
// Retail layout, from the constructor ??0COlePropertiesDialog@@QEAA@PEAVCOleClientItem@@IIPEAVCWnd@@@Z
// (RVA 0x24ead0, mfc140u; mfc140 twin 0x24da10) and afxodlgs.h:394:
//     +0x138 m_op (0x48)  +0x180 m_gp (0x38)  +0x1b8 m_vp (0x40)
//     +0x1f8 m_lp (0x38)  +0x230 m_psh (0x60)
//     +0x290 m_pDoc   (the ctor copies the pointer at pItem+0x40 into it)
//     +0x298 m_xOleUIObjInfo (vptr only; vftable 0x18032e028, mfc140u)
//     +0x2a0 m_xLinkInfo (COleUILinkInfo, 0x30)          sizeof == 0x2d0
// The XOleUIObjInfo methods receive pThis == &m_xOleUIObjInfo; every one of
// them reads m_pDoc at pThis-8 (the SetViewInfo body instead forms the dialog
// pointer pThis-0x298 and reads +0x290 from it -- the same slot).
//
// OpenMFC's public COlePropertiesDialog (include/openmfc/afxole.h) matches
// retail's sizeof and the m_op offset (+0x138, measured with mingw offsetof);
// after that it diverges: its m_pItem sits at +0x180 (retail's m_gp.cbStruct)
// and +0x290/+0x298 fall inside its _olepropertiesdialog_padding (+0x188..),
// which OpenMFC's constructor zero-fills and never populates.  With m_pDoc
// NULL every method below throws CInvalidArgException, exactly as retail does
// for a NULL m_pDoc; they become reachable once the constructor installs the
// retail layout.
//
// Item lookup, common to all five methods (read from each body; e.g. in
// GetViewInfo 0x24f340 (mfc140u) the GetNextItemOfKind call is at 0x24f38e):
//     ENSURE(m_pDoc != NULL);                                // AfxThrowInvalidArgException 0x227720
//     POSITION pos = m_pDoc->GetStartPosition();             // vftable +0x238, slot 71
//     COleClientItem* pItem = NULL;
//     for (DWORD i = 0; i < dwObject; i++)
//         pItem = m_pDoc->GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem)); // 0x254120
//     ENSURE(pItem != NULL);                                 // also taken when dwObject == 0
// (RUNTIME_CLASS descriptor 0x18032e788, mfc140u: its class-name field reads
// "COleClientItem".)  dwObject is therefore the 1-based index of the item among
// the document's client items -- the constructor stores that index into
// m_op.dwObject (+0x148).
//
// OpenMFC deviation shared by all methods: calls into COleDocument /
// COleClientItem / this dialog go through the classes' impl__ export thunks
// rather than the retail vtable slots (as core/ole/COleUILinkInfo.cpp does for
// the same GetStartPosition slot).  For GetStartPosition, ConvertTo,
// SetDrawAspect and OnApplyScale -- none of which OpenMFC's headers declare
// virtual -- a client override is therefore not reached.  OnChange is the
// exception: OpenMFC declares it virtual (afxole.h, class COleClientItem), and
// its thunk in core/ole/Thunks.cpp makes a C++ virtual call, so it dispatches
// through OpenMFC's own (non-retail) vtable rather than retail slot 29.
// -----------------------------------------------------------------------------

// Sibling export thunks, declared with the parameter lists their definitions
// use.  Definitions: GetStartPosition, ConvertTo, Get/SetIconicMetafile and
// OnChange in core/ole/Thunks.cpp; GetNextItemOfKind in core/ole/COleDocument.cpp;
// GetThisClass in core/ole/RuntimeClasses.cpp; GetClassID and SetDrawAspect in
// core/ole/COleClientItem.cpp; AfxMessageBox in core/collections/Globals.cpp;
// AfxThrowInvalidArgException in detail/MfcExceptionsSupport.cpp.
extern "C" void* MS_ABI impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(
    const COleDocument* pThis);
extern "C" void* MS_ABI impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
    const COleDocument* pThis, void** pPos, CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" int MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType, UINT nIDHelp);
extern "C" int MS_ABI impl__ConvertTo_COleClientItem__UEAAHAEBU_GUID___Z(
    COleClientItem* pThis, const _GUID* clsidNew);
extern "C" void MS_ABI impl__GetClassID_COleClientItem__QEBAXPEAU_GUID___Z(
    const COleClientItem* pThis, GUID* pClassID);
extern "C" void* MS_ABI impl__GetIconicMetafile_COleClientItem__QEAAPEAXXZ(COleClientItem* pThis);
extern "C" int MS_ABI impl__SetIconicMetafile_COleClientItem__QEAAHPEAX_Z(
    COleClientItem* pThis, void* hMetaPict);
extern "C" void MS_ABI impl__OnChange_COleClientItem__UEAAXW4OLE_NOTIFICATION__K_Z(
    COleClientItem* pThis, OLE_NOTIFICATION nCode, unsigned long dwParam);
extern "C" void MS_ABI impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(
    COleClientItem* pThis, int nDrawAspect);
// This file's own OnApplyScale thunk (defined below).
extern "C" int MS_ABI impl__OnApplyScale_COlePropertiesDialog__UEAAHPEAVCOleClientItem__HH_Z(
    COlePropertiesDialog* pThis, COleClientItem* pItem, int nCurrentScale, int bRelativeToOrig);

namespace {

static_assert(sizeof(COlePropertiesDialog) == 0x2d0,
              "COlePropertiesDialog must keep retail sizeof 0x2d0 (ctor 0x24ead0, mfc140u)");

constexpr size_t kOffDoc        = 0x290;  // m_pDoc
constexpr size_t kOffObjInfo    = 0x298;  // m_xOleUIObjInfo

// Retail enum OLE_NOTIFICATION (atlmfc afxole.h:115): OLE_CHANGED = 0,
// OLE_CHANGED_ASPECT = 5.  OpenMFC's own afxole.h #defines OLE_CHANGED as 5,
// which is not the retail value, so the literals are spelled here.
constexpr OLE_NOTIFICATION kOLE_CHANGED        = 0;
constexpr OLE_NOTIFICATION kOLE_CHANGED_ASPECT = 5;
constexpr UINT kAFX_IDP_FAILED_TO_CONVERT      = 0xF18B;  // atlmfc afxres.h

COlePropertiesDialog* DialogOfObjInfo(void* pThis) {
    return reinterpret_cast<COlePropertiesDialog*>(static_cast<char*>(pThis) - kOffObjInfo);
}

// The shared item lookup documented above.
COleClientItem* ObjInfoItem(void* pThis, unsigned long dwObject) {
    COleDocument* pDoc = *reinterpret_cast<COleDocument**>(
        static_cast<char*>(pThis) - (kOffObjInfo - kOffDoc));
    if (pDoc == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;  // not reached: the thunk throws
    }
    void* pos = impl__GetStartPosition_COleDocument__UEBAPEAU__POSITION__XZ(pDoc);
    COleClientItem* pItem = nullptr;
    for (unsigned long i = 0; i < dwObject; ++i)
        pItem = static_cast<COleClientItem*>(static_cast<CDocItem*>(
            impl__GetNextItemOfKind_COleDocument__IEBAPEAVCDocItem__AEAPEAU__POSITION__PEAUCRuntimeClass___Z(
                pDoc, &pos, impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ())));
    if (pItem == nullptr)
        impl__AfxThrowInvalidArgException__YAXXZ();
    return pItem;
}

}  // namespace

























// Symbol: ??0COlePropertiesDialog@@QEAA@PEAVCOleClientItem@@IIPEAVCWnd@@@Z
// COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
// A constructor export receives the caller's storage as `this` (RCX) and must
// construct in place and return it, like the sibling COleChangeIconDialog /
// COleUpdateDialog thunks.  The previous placeholder had no `this`, so every
// argument was shifted by one register and it heap-allocated a fresh object
// the caller never saw.  This fixes only the calling convention; the OpenMFC
// C++ constructor still does not build retail's layout (see the note at the
// top of this file).
extern "C" void* MS_ABI impl___0COlePropertiesDialog__QEAA_PEAVCOleClientItem__IIPEAVCWnd___Z(
    void* p, COleClientItem* pItem, unsigned int nScaleMin, unsigned int nScaleMax,
    CWnd* pParentWnd) {
    return new (p) COlePropertiesDialog(pItem, nScaleMin, nScaleMax, pParentWnd);
}
// Symbol: ?OnApplyScale@COlePropertiesDialog@@UEAAHPEAVCOleClientItem@@HH@Z
// COlePropertiesDialog::OnApplyScale(COleClientItem*, int nCurrentScale, BOOL bRelativeToOrig)
// Export ordinal 8669 resolves through mfc140u.dll's export table to RVA 0x71e0
// (mfc140u) -- a body shared by many exports, so the RVA-keyed
// mfc140u_rva_symbols.json lists it under another name.  Slot 101 (+0x328, the
// slot SetViewInfo 0x24f3f0 calls with (pItem, nScale, bRel)) of the
// COlePropertiesDialog vftable 0x18032e070 (mfc140u, installed by the
// constructor 0x24ead0) points at the same RVA.  0x71e0 is
// `xor eax,eax; ret`: the default does nothing and returns FALSE.
extern "C" int MS_ABI impl__OnApplyScale_COlePropertiesDialog__UEAAHPEAVCOleClientItem__HH_Z(
    COlePropertiesDialog* pThis, COleClientItem* pItem, int nCurrentScale, int bRelativeToOrig) {
    (void)pThis; (void)pItem; (void)nCurrentScale; (void)bRelativeToOrig;
    return FALSE;
}
// Symbol: ?OnInitDialog@COlePropertiesDialog@@UEAAHXZ
// COlePropertiesDialog::OnInitDialog
extern "C" int MS_ABI impl__OnInitDialog_COlePropertiesDialog__UEAAHXZ(
    COlePropertiesDialog* pThis) {
    return pThis ? static_cast<CDialog*>(static_cast<COleDialog*>(pThis))->OnInitDialog() : FALSE;
}
COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
    : COleDialog(0, pParentWnd), m_pItem(pItem) {
    (void)nScaleMin; (void)nScaleMax;
    memset(_olepropertiesdialog_padding, 0, sizeof(_olepropertiesdialog_padding));
}
COlePropertiesDialog::~COlePropertiesDialog() {
}
intptr_t COlePropertiesDialog::DoModal() {
    return IDOK;  // OLEUIOBJECTPROPSW is not fully defined in MinGW
}

// Symbol: ?ConvertObject@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKAEBU_GUID@@@Z
// Transcribed from retail RVA 0x24f2a0 (mfc140u; mfc140 twin 0x24e270):
//     pItem = <item dwObject, see ObjInfoItem>;
//     if (!pItem->ConvertTo(clsidNew)) {                     // vftable +0xd8, slot 27
//         AfxMessageBox(AFX_IDP_FAILED_TO_CONVERT /*0xF18B*/,
//                       MB_ICONEXCLAMATION /*0x30*/, (UINT)-1);   // 0x1cec70
//         return E_FAIL;
//     }
//     return S_OK;
// Deviation: ConvertTo is reached through its export thunk, not the item's
// vtable (see the file-scope note above ObjInfoItem).
extern "C" long MS_ABI impl__ConvertObject_XOleUIObjInfo_COlePropertiesDialog__UEAAJKAEBU_GUID___Z(
    void* pThis, unsigned long dwObject, const GUID* clsidNew) {
    COleClientItem* pItem = ObjInfoItem(pThis, dwObject);
    if (!impl__ConvertTo_COleClientItem__UEAAHAEBU_GUID___Z(pItem, clsidNew)) {
        impl__AfxMessageBox__YAHIII_Z(kAFX_IDP_FAILED_TO_CONVERT, MB_ICONEXCLAMATION,
                                      static_cast<UINT>(-1));
        return E_FAIL;
    }
    return S_OK;
}

// Symbol: ?GetConvertInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAU_GUID@@PEAG0PEAPEAU3@PEAI@Z
// Retail RVA 0x24f1c0 (mfc140u; mfc140 twin 0x24e190) reads only dwObject,
// lpClassID and lpwFormat; lpConvertDefaultClassID, lplpClsidExclude and
// lpcClsidExclude (the stack arguments) are never touched:
//     pItem = <item dwObject>;
//     if (lpClassID != NULL &&
//         (pItem->m_nItemType /*+0x94*/ == OT_LINK ||
//          ReadClassStg(pItem->m_lpStorage /*+0x68*/, lpClassID) != S_OK))
//         pItem->GetClassID(lpClassID);                      // 0x246690
//     if (lpwFormat != NULL) {
//         *lpwFormat = 0;
//         CLIPFORMAT cf;
//         if (ReadFmtUserTypeStg(pItem->m_lpStorage, &cf, NULL) == S_OK)
//             *lpwFormat = cf;
//     }
//     return S_OK;
// (ReadClassStg / ReadFmtUserTypeStg: ole32 imports, IAT slots 0x1802c7948 /
// 0x1802c7a68 in mfc140u, resolved with iatu.py.)
// DEVIATION -- partial: OpenMFC's COleClientItem models neither m_lpStorage
// nor m_nItemType (see core/ole/COleClientItem.cpp, UpdateItemType and the
// storage notes near GetItemStorageFlat), so there is no item storage to read.
// OpenMFC therefore always takes retail's fallback path: the class ID comes
// from GetClassID (IOleObject::GetUserClassID) and *lpwFormat stays 0.  For an
// embedded item whose storage records a different class (TreatAs/emulation)
// or a native format, retail would report those instead.
extern "C" long MS_ABI impl__GetConvertInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAU_GUID__PEAG0PEAPEAU3_PEAI_Z(
    void* pThis, unsigned long dwObject, GUID* lpClassID, unsigned short* lpwFormat,
    GUID* lpConvertDefaultClassID, GUID** lplpClsidExclude, unsigned int* lpcClsidExclude) {
    (void)lpConvertDefaultClassID; (void)lplpClsidExclude; (void)lpcClsidExclude;
    COleClientItem* pItem = ObjInfoItem(pThis, dwObject);
    if (lpClassID)
        impl__GetClassID_COleClientItem__QEBAXPEAU_GUID___Z(pItem, lpClassID);
    if (lpwFormat)
        *lpwFormat = 0;
    return S_OK;
}

// Symbol: ?GetObjectInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAKPEAPEA_W111@Z
// STUB.  Retail RVA 0x24ee30 (mfc140u) fills the size from
// m_lpLockBytes->Stat (+0x70) falling back to m_lpStorage->Stat (+0x68, not
// NULL-checked), picks the label/location by m_nItemType (+0x94) == OT_LINK,
// and builds strings with GetUserType(USERCLASSTYPE, CString&) (0x24ae80),
// AfxLoadString(AFX_IDS_PASTELINKEDTYPE) (0x2af0b0) and CString::Format.
// OpenMFC's COleClientItem has no m_lpStorage / m_lpLockBytes / m_nItemType,
// so the body cannot be transcribed; left returning S_OK with no outputs set.
extern "C" long MS_ABI impl__GetObjectInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAKPEAPEA_W111_Z(
    void* pThis, unsigned long dwObject, unsigned long* lpdwObjSize, wchar_t** lplpszLabel,
    wchar_t** lplpszType, wchar_t** lplpszShortType, wchar_t** lplpszLocation) {
    (void)pThis; (void)dwObject; (void)lpdwObjSize; (void)lplpszLabel;
    (void)lplpszType; (void)lplpszShortType; (void)lplpszLocation;
    return 0;
}

// Symbol: ?GetViewInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAPEAXPEAKPEAH@Z
// Transcribed from retail RVA 0x24f340 (mfc140u; mfc140 twin 0x24e310):
//     pItem = <item dwObject>;
//     if (phMetaPict != NULL)     *phMetaPict = pItem->GetIconicMetafile();  // 0x246850
//     if (pdvAspect != NULL)      *pdvAspect = pItem->m_nDrawAspect;         // +0x5c
//     if (pnCurrentScale != NULL) *pnCurrentScale = 100;
//     return S_OK;
// m_nDrawAspect is read through OpenMFC's own COleClientItem member (its
// layout is not retail's; +0x5c is the retail offset).
extern "C" long MS_ABI impl__GetViewInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAPEAXPEAKPEAH_Z(
    void* pThis, unsigned long dwObject, HGLOBAL* phMetaPict, unsigned long* pdvAspect,
    int* pnCurrentScale) {
    COleClientItem* pItem = ObjInfoItem(pThis, dwObject);
    if (phMetaPict)
        *phMetaPict = static_cast<HGLOBAL>(
            impl__GetIconicMetafile_COleClientItem__QEAAPEAXXZ(pItem));
    if (pdvAspect)
        *pdvAspect = static_cast<unsigned long>(pItem->m_nDrawAspect);
    if (pnCurrentScale)
        *pnCurrentScale = 100;
    return S_OK;
}

// Symbol: ?SetViewInfo@XOleUIObjInfo@COlePropertiesDialog@@UEAAJKPEAXKHH@Z
// Transcribed from retail RVA 0x24f3f0 (mfc140u; export ordinal 13741 resolves
// there through the export table, though mfc140u_rva_symbols.json lacks it, and
// slot 7 of the XOleUIObjInfo vftable 0x18032e028 (mfc140u) points there too;
// mfc140 twin 0x24e3c0).  pThis - 0x298 is the dialog.
//     pItem = <item dwObject>;
//     int nScale; BOOL bRel;
//     if (dvAspect != (DWORD)-1) {
//         pItem->OnChange(OLE_CHANGED_ASPECT /*5*/, dvAspect);    // vftable +0xe8, slot 29
//         pItem->SetDrawAspect((DVASPECT)dvAspect);               // vftable +0xb8, slot 23
//         if (dvAspect == DVASPECT_ICON)      { nScale = 100; bRel = TRUE; }
//         else if (nCurrentScale == -1)       { nScale = 100; bRel = FALSE; }
//         else { nScale = nCurrentScale; bRel = bRelativeToOrig; }
//     } else { nScale = nCurrentScale; bRel = bRelativeToOrig; }
//     if (hMetaPict != NULL) {
//         pItem->SetIconicMetafile(hMetaPict);                    // 0x246760 (ordinal 13252), result ignored
//         if (pItem->m_nDrawAspect /*+0x5c*/ == DVASPECT_ICON)
//             pItem->OnChange(OLE_CHANGED /*0*/, DVASPECT_ICON);  // slot 29
//     }
//     if (nScale != -1)
//         pDlg->OnApplyScale(pItem, nScale, bRel);                // dialog vftable +0x328, slot 101
//     return S_OK;
// Deviations: OnChange, SetDrawAspect and OnApplyScale are reached through
// their export thunks rather than the retail vtable slots.  SetDrawAspect and
// OnApplyScale are not virtual in OpenMFC's headers, so a client override is
// not reached; for OnApplyScale the default (retail slot 101 -> the shared
// `xor eax,eax; ret` at 0x71e0, mfc140u) is what runs.  OnChange's thunk makes
// a C++ virtual call through OpenMFC's own vtable (see the note at the top).
// The notification codes passed are retail's (5 and 0).  OpenMFC's default
// COleClientItem::OnChange (core/ole/COleClientItem.cpp) compares nCode against
// its own `#define OLE_CHANGED 5`, so it treats the aspect change here as a
// content change and ignores the code-0 call; that is a defect of the header
// value, not of this transcription.
extern "C" long MS_ABI impl__SetViewInfo_XOleUIObjInfo_COlePropertiesDialog__UEAAJKPEAXKHH_Z(
    void* pThis, unsigned long dwObject, HGLOBAL hMetaPict, unsigned long dvAspect,
    int nCurrentScale, int bRelativeToOrig) {
    COlePropertiesDialog* pDlg = DialogOfObjInfo(pThis);
    COleClientItem* pItem = ObjInfoItem(pThis, dwObject);
    int nScale = nCurrentScale;
    int bRel = bRelativeToOrig;
    if (dvAspect != static_cast<unsigned long>(-1)) {
        impl__OnChange_COleClientItem__UEAAXW4OLE_NOTIFICATION__K_Z(
            pItem, kOLE_CHANGED_ASPECT, dvAspect);
        impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(
            pItem, static_cast<int>(dvAspect));
        if (dvAspect == DVASPECT_ICON) {
            nScale = 100;
            bRel = TRUE;
        } else if (nCurrentScale == -1) {
            nScale = 100;
            bRel = FALSE;
        }
    }
    if (hMetaPict) {
        (void)impl__SetIconicMetafile_COleClientItem__QEAAHPEAX_Z(pItem, hMetaPict);
        if (pItem->m_nDrawAspect == DVASPECT_ICON)
            impl__OnChange_COleClientItem__UEAAXW4OLE_NOTIFICATION__K_Z(
                pItem, kOLE_CHANGED, DVASPECT_ICON);
    }
    if (nScale != -1)
        impl__OnApplyScale_COlePropertiesDialog__UEAAHPEAVCOleClientItem__HH_Z(
            pDlg, pItem, nScale, bRel);
    return S_OK;
}
