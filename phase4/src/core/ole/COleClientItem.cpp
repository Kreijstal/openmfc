// COleClientItem — OpenMFC implementation.
// Sources: global_ole-1_impl.cpp, global_ole-2_impl.cpp, ole_olectors_exports.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/OlecoreSupport.h"

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Forward declarations for impl functions referenced from other impls below.
extern "C" int MS_ABI impl__FinishCreate_COleClientItem__MEAAHJ_Z(
        COleClientItem* pThis, long hr);
extern "C" void MS_ABI impl__GetEmbeddedItemData_COleClientItem__IEAAXPEAUtagSTGMEDIUM___Z(
        COleClientItem* pThis, STGMEDIUM* lpMedium);
extern "C" void MS_ABI impl__AddCachedData_COleClientItem__IEAAXPEAVCOleDataSource___Z(
        COleClientItem* pThis, COleDataSource* pDataSource);
extern "C" void MS_ABI impl__GetClipboardData_COleClientItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
        COleClientItem* pThis, COleDataSource* pDataSource, int bIncludeLink,
        tagPOINT* lpOffset, tagSIZE* lpSize);
extern "C" void MS_ABI impl__GetClassID_COleClientItem__QEBAXPEAU_GUID___Z(
        const COleClientItem* pThis, GUID* pClassID);
extern "C" HICON MS_ABI impl__GetIconFromRegistry_COleClientItem__SAPEAUHICON____AEAU_GUID___Z(
        GUID& clsid);
extern "C" int MS_ABI impl__CanPaste_COleClientItem__SAHXZ();
extern "C" int MS_ABI impl__CanPasteLink_COleClientItem__SAHXZ();

//=============================================================================
// Wave 2: COleClientItem methods (30 exports).
//
// Each implementation is an extern "C" ms_abi function named impl_<mangled>
// with the MSVC-retail parameter list passed explicitly (first parameter is
// the COleClientItem "this" pointer).  The build script scans the
// "// Symbol:" comments below and excludes those symbols from the generated
// weak stubs, so these strong definitions take effect.
//
// Layout note: the OpenMFC COleClientItem header declares
//   m_pContainerDoc@0x40 m_lpObject@0x48 m_lpLink@0x50 m_lpInPlaceObject@0x58
//   m_nStatus@0x5C m_nDrawAspect@0x60 m_pControlSite@0x68 m_lpFrame@0x70
//   m_lpDocFrame@0x78 m_bInPlaceActive@0x7C  (then 96 bytes of padding).
// Only those members are touched.  Retail functions that also touch unmodeled
// state (item-name id, storage, item state machine at 0x80..0x98) are
// implemented conservatively and marked with TODO(clean-room).
//=============================================================================

//-----------------------------------------------------------------------------
// Creation
//-----------------------------------------------------------------------------







//-----------------------------------------------------------------------------
// Activation
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
// State
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Clipboard / Data source
//-----------------------------------------------------------------------------









//-----------------------------------------------------------------------------
// Query / Get accessors
//-----------------------------------------------------------------------------









//-----------------------------------------------------------------------------
// Storage / lifetime
//-----------------------------------------------------------------------------




// COleClientItem / COleServerDoc methods, wave 2 (ole-2). Every function here
// was decoded from the retail mfc140u export (ordinal -> RVA -> objdump, see
// /tmp/openmfc_remaining/disasm.py) rather than inferred, and the members
// touched are exactly the ones declared in include/openmfc/afxole.h.
//
// COleClientItem layout (per afxole.h, member names only):
//   m_pDocument@0x40 (CDocItem) m_pContainerDoc@0x48 m_lpObject@0x50
//   m_lpLink@0x58 m_lpInPlaceObject@0x60 m_nStatus@0x68 m_nDrawAspect@0x6C
//   m_pControlSite@0x70 m_lpFrame@0x78 m_lpDocFrame@0x80 m_bInPlaceActive@0x88
// COleServerDoc declares only m_bEmbedded@0x230; the retail in-place state
// (m_pInPlaceFrame@0x230, m_pInPlaceDocFrame@0x250, m_rectPos/m_rectClip,
// m_pDocObjectServer@0x268, ...) is NOT modeled, so COleServerDoc methods that
// need it are implemented conservatively (safe defaults + TODO(clean-room)).
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass drops the
// generated stubs for these symbols.

#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include "openmfc/afxstr.h"

#include <ole2.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---- sibling impl_ exports (defined in other phase4/src/*.cpp) ----
extern "C" int MS_ABI impl__CanCloseFrame_CDocument__UEAAHPEAVCFrameWnd___Z(
    CDocument* pThis, CFrameWnd* pFrameWnd);
// AfxMessageBox(UINT,UINT,UINT) — impl defined in strcore.cpp
// (mangled: ?AfxMessageBox@@YAHIII@Z)
extern "C" int MS_ABI impl__AfxMessageBox__YAHIII_Z(UINT nIDPrompt, UINT nType,
                                                    UINT nIDHelp);

// ---- forward declarations for impl_ functions defined later in this file ----
extern "C" void MS_ABI impl__WriteItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& /*ar*/);
extern "C" void MS_ABI impl__WriteItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& /*ar*/);

//=============================================================================
// COleClientItem
//=============================================================================












//=============================================================================
// COleServerDoc
//=============================================================================
//
// The retail COleServerDoc carries its in-place frame, doc-frame, item rects,
// and doc-object server inside the region that OpenMFC leaves as padding (only
// m_bEmbedded is declared). Functions below that depend on that region return
// safe defaults and are marked TODO(clean-room).



















#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ?CreateFromFile@COleClientItem@@QEAAHPEB_WAEBU_GUID@@W4tagOLERENDER@@GPEAUtagFORMATETC@@@Z
extern "C" int MS_ABI impl__CreateFromFile_COleClientItem__QEAAHPEB_WAEBU_GUID__W4tagOLERENDER__GPEAUtagFORMATETC___Z(
        COleClientItem* pThis, const wchar_t* lpszFileName, const GUID& clsid,
        OLERENDER render, unsigned short cfFormat, FORMATETC* lpFormatEtc) {
    if (!pThis) return FALSE;
    if (!lpszFileName) return FALSE;
    pThis->Close(OLECLOSE_NOSAVE);

    FORMATETC formatEtc;
    memset(&formatEtc, 0, sizeof(formatEtc));
    LPFORMATETC lpFormatEtcUsed = lpFormatEtc;
    if (render == OLERENDER_DRAW && lpFormatEtcUsed == nullptr) {
        // Default to a metafile draw aspect (matches retail behavior).
        formatEtc.cfFormat = CF_METAFILEPICT;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_MFPICT;
        lpFormatEtcUsed = &formatEtc;
    } else if (render == OLERENDER_FORMAT && lpFormatEtcUsed == nullptr && cfFormat != 0) {
        formatEtc.cfFormat = (CLIPFORMAT)cfFormat;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_HGLOBAL;
        lpFormatEtcUsed = &formatEtc;
    }

    HRESULT hr = OleCreateFromFile(clsid, lpszFileName, IID_IOleObject, render,
                                   lpFormatEtcUsed, nullptr, nullptr,
                                   reinterpret_cast<void**>(&pThis->m_lpObject));
    if (FAILED(hr) || !pThis->m_lpObject) {
        pThis->m_lpObject = nullptr;
        pThis->m_lpLink = nullptr;
        return FALSE;
    }
    return impl__FinishCreate_COleClientItem__MEAAHJ_Z(pThis, hr);
}
// Symbol: ?CreateLinkFromFile@COleClientItem@@QEAAHPEB_WW4tagOLERENDER@@GPEAUtagFORMATETC@@@Z
extern "C" int MS_ABI impl__CreateLinkFromFile_COleClientItem__QEAAHPEB_WW4tagOLERENDER__GPEAUtagFORMATETC___Z(
        COleClientItem* pThis, const wchar_t* lpszFileName, OLERENDER render,
        unsigned short cfFormat, FORMATETC* lpFormatEtc) {
    if (!pThis) return FALSE;
    if (!lpszFileName) return FALSE;
    pThis->Close(OLECLOSE_NOSAVE);

    FORMATETC formatEtc;
    memset(&formatEtc, 0, sizeof(formatEtc));
    LPFORMATETC lpFormatEtcUsed = lpFormatEtc;
    if (render == OLERENDER_DRAW && lpFormatEtcUsed == nullptr) {
        formatEtc.cfFormat = CF_METAFILEPICT;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_MFPICT;
        lpFormatEtcUsed = &formatEtc;
    } else if (render == OLERENDER_FORMAT && lpFormatEtcUsed == nullptr && cfFormat != 0) {
        formatEtc.cfFormat = (CLIPFORMAT)cfFormat;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_HGLOBAL;
        lpFormatEtcUsed = &formatEtc;
    }

    HRESULT hr = OleCreateLinkToFile(lpszFileName, IID_IOleObject, render,
                                     lpFormatEtcUsed, nullptr, nullptr,
                                     reinterpret_cast<void**>(&pThis->m_lpObject));
    if (FAILED(hr) || !pThis->m_lpObject) {
        pThis->m_lpObject = nullptr;
        pThis->m_lpLink = nullptr;
        return FALSE;
    }
    pThis->m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&pThis->m_lpLink));
    return impl__FinishCreate_COleClientItem__MEAAHJ_Z(pThis, hr);
}
// Symbol: ?CreateLinkFromData@COleClientItem@@QEAAHPEAVCOleDataObject@@W4tagOLERENDER@@GPEAUtagFORMATETC@@@Z
extern "C" int MS_ABI impl__CreateLinkFromData_COleClientItem__QEAAHPEAVCOleDataObject__W4tagOLERENDER__GPEAUtagFORMATETC___Z(
        COleClientItem* pThis, COleDataObject* pDataObject, OLERENDER render,
        unsigned short cfFormat, FORMATETC* lpFormatEtc) {
    if (!pThis || !pDataObject) return FALSE;
    IDataObject* pData = pDataObject->GetIDataObject(TRUE);
    if (!pData) return FALSE;
    pThis->Close(OLECLOSE_NOSAVE);

    FORMATETC formatEtc;
    memset(&formatEtc, 0, sizeof(formatEtc));
    LPFORMATETC lpFormatEtcUsed = lpFormatEtc;
    if (render == OLERENDER_DRAW && lpFormatEtcUsed == nullptr) {
        formatEtc.cfFormat = CF_METAFILEPICT;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_MFPICT;
        lpFormatEtcUsed = &formatEtc;
    } else if (render == OLERENDER_FORMAT && lpFormatEtcUsed == nullptr && cfFormat != 0) {
        formatEtc.cfFormat = (CLIPFORMAT)cfFormat;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_HGLOBAL;
        lpFormatEtcUsed = &formatEtc;
    }

    HRESULT hr = OleCreateLinkFromData(pData, IID_IOleObject, render, lpFormatEtcUsed,
                                       nullptr, nullptr,
                                       reinterpret_cast<void**>(&pThis->m_lpObject));
    pData->Release();
    if (FAILED(hr) || !pThis->m_lpObject) {
        pThis->m_lpObject = nullptr;
        pThis->m_lpLink = nullptr;
        return FALSE;
    }
    pThis->m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&pThis->m_lpLink));
    return impl__FinishCreate_COleClientItem__MEAAHJ_Z(pThis, hr);
}
// Symbol: ?CreateStaticFromData@COleClientItem@@QEAAHPEAVCOleDataObject@@W4tagOLERENDER@@GPEAUtagFORMATETC@@@Z
extern "C" int MS_ABI impl__CreateStaticFromData_COleClientItem__QEAAHPEAVCOleDataObject__W4tagOLERENDER__GPEAUtagFORMATETC___Z(
        COleClientItem* pThis, COleDataObject* pDataObject, OLERENDER render,
        unsigned short cfFormat, FORMATETC* lpFormatEtc) {
    if (!pThis || !pDataObject) return FALSE;
    IDataObject* pData = pDataObject->GetIDataObject(TRUE);
    if (!pData) return FALSE;
    pThis->Close(OLECLOSE_NOSAVE);

    FORMATETC formatEtc;
    memset(&formatEtc, 0, sizeof(formatEtc));
    LPFORMATETC lpFormatEtcUsed = lpFormatEtc;
    if (render == OLERENDER_DRAW && lpFormatEtcUsed == nullptr) {
        formatEtc.cfFormat = CF_METAFILEPICT;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_MFPICT;
        lpFormatEtcUsed = &formatEtc;
    } else if (render == OLERENDER_FORMAT && lpFormatEtcUsed == nullptr && cfFormat != 0) {
        formatEtc.cfFormat = (CLIPFORMAT)cfFormat;
        formatEtc.dwAspect = DVASPECT_CONTENT;
        formatEtc.lindex = -1;
        formatEtc.tymed = TYMED_HGLOBAL;
        lpFormatEtcUsed = &formatEtc;
    }

    HRESULT hr = OleCreateStaticFromData(pData, IID_IOleObject, render, lpFormatEtcUsed,
                                         nullptr, nullptr,
                                         reinterpret_cast<void**>(&pThis->m_lpObject));
    pData->Release();
    if (FAILED(hr) || !pThis->m_lpObject) {
        pThis->m_lpObject = nullptr;
        pThis->m_lpLink = nullptr;
        return FALSE;
    }
    return impl__FinishCreate_COleClientItem__MEAAHJ_Z(pThis, hr);
}
// Symbol: ?CreateCloneFrom@COleClientItem@@QEAAHPEBV1@@Z
// TODO(clean-room): transcribed partially.  Retail clones the source item's
// storage via its IStorage; OpenMFC has no declared IStorage member, so we
// cannot produce a faithful clone.  Conservative: fail.
extern "C" int MS_ABI impl__CreateCloneFrom_COleClientItem__QEAAHPEBV1__Z(
        COleClientItem* pThis, const COleClientItem* pSrcItem) {
    (void)pSrcItem;
    if (!pThis) return FALSE;
    if (!pSrcItem) return FALSE;
    // TODO(clean-room): fully implement via OleClone / storage copy.
    return FALSE;
}
// Symbol: ?FinishCreate@COleClientItem@@MEAAHJ@Z
// Sets up the item after a successful OLE create call.  Retail also queries
// for IOleLink (when the item is a link) and assigns the item name id.
extern "C" int MS_ABI impl__FinishCreate_COleClientItem__MEAAHJ_Z(
        COleClientItem* pThis, long hr) {
    if (!pThis) return FALSE;
    if (!pThis->m_lpObject) return FALSE;
    if (FAILED(hr)) return FALSE;
    if (!pThis->m_lpLink) {
        pThis->m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&pThis->m_lpLink));
    }
    pThis->m_nStatus = OLE_LOADED;
    return TRUE;
}
// Symbol: ?Activate@COleClientItem@@QEAAXJPEAVCView@@PEAUtagMSG@@@Z
// Retail 3-argument activation.  Performs DoVerb on the object using the
// item position from OnGetItemPosition and the view's window as parent.
extern "C" void MS_ABI impl__Activate_COleClientItem__QEAAXJPEAVCView__PEAUtagMSG___Z(
        COleClientItem* pThis, long nVerb, CView* pView, tagMSG* lpMsg) {
    if (!pThis || !pThis->m_lpObject) return;
    HWND hwndParent = pView ? pView->GetSafeHwnd() : nullptr;
    CRect rect;
    pThis->OnGetItemPosition(rect);
    HRESULT hr = pThis->m_lpObject->DoVerb(nVerb, lpMsg, nullptr, 0, hwndParent, rect);
    if (SUCCEEDED(hr)) {
        pThis->m_nStatus = OLE_RUNNING;
        pThis->OnActivate();
    }
}
// Symbol: ?ActivateAs@COleClientItem@@UEAAHPEB_WAEBU_GUID@@1@Z
// TODO(clean-room): transcribed partially.  Retail re-registers the object's
// class in the registry (OleReg* / registry rewrites) and closes/recreates
// the object.  Conservative: fail.
extern "C" int MS_ABI impl__ActivateAs_COleClientItem__UEAAHPEB_WAEBU_GUID__1_Z(
        COleClientItem* pThis, const wchar_t* lpszUserType,
        const GUID& clsidOld, const GUID& clsidNew) {
    (void)lpszUserType; (void)clsidOld; (void)clsidNew;
    if (!pThis) return FALSE;
    // TODO(clean-room): fully implement registry conversion.
    return FALSE;
}
// Symbol: ?DeactivateUI@COleClientItem@@QEAAXXZ
// TODO(clean-room): transcribed partially.  Retail deactivates the in-place
// UI frame/doc-frame without fully deactivating the object; OpenMFC keeps
// only the basic in-place object.
extern "C" void MS_ABI impl__DeactivateUI_COleClientItem__QEAAXXZ(
        COleClientItem* pThis) {
    if (!pThis) return;
    if (pThis->m_lpInPlaceObject) {
        pThis->m_lpInPlaceObject->InPlaceDeactivate();
    }
    pThis->m_bInPlaceActive = FALSE;
}
// Symbol: ?CheckGeneral@COleClientItem@@MEAAXJ@Z
// Retail stores the OLE notification code as the item status and throws an
// OLE exception when the code is negative.
extern "C" void MS_ABI impl__CheckGeneral_COleClientItem__MEAAXJ_Z(
        COleClientItem* pThis, long nCode) {
    if (!pThis) return;
    pThis->m_nStatus = (OLE_STATUS)nCode;
    if (nCode < 0) {
        AfxThrowOleException(nCode);
    }
}
// Symbol: ?CanPaste@COleClientItem@@SAHXZ
// Retail checks the MFC-registered clipboard formats.  We register the same
// well-known format names and ask the clipboard for each.
extern "C" int MS_ABI impl__CanPaste_COleClientItem__SAHXZ() {
    static const CLIPFORMAT cfEmbeddedObject =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Embedded Object");
    static const CLIPFORMAT cfLinkSource =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Link Source");
    static const CLIPFORMAT cfObjectDescriptor =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Object Descriptor");
    static const CLIPFORMAT cfLinkSourceDescriptor =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Link Source Descriptor");
    static const CLIPFORMAT cfFileName =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"FileName");
    static const CLIPFORMAT cfFileNameW =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"FileNameW");
    static const CLIPFORMAT cfRichTextFormat =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Rich Text Format");
    static const CLIPFORMAT cfRichTextFormatW =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Rich Text FormatW");

    return ::IsClipboardFormatAvailable(cfEmbeddedObject) ||
           ::IsClipboardFormatAvailable(cfLinkSource) ||
           ::IsClipboardFormatAvailable(cfObjectDescriptor) ||
           ::IsClipboardFormatAvailable(cfLinkSourceDescriptor) ||
           ::IsClipboardFormatAvailable(cfFileName) ||
           ::IsClipboardFormatAvailable(cfFileNameW) ||
           ::IsClipboardFormatAvailable(cfRichTextFormat) ||
           ::IsClipboardFormatAvailable(cfRichTextFormatW);
}
// Symbol: ?CanPasteLink@COleClientItem@@SAHXZ
extern "C" int MS_ABI impl__CanPasteLink_COleClientItem__SAHXZ() {
    static const CLIPFORMAT cfLinkSource =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Link Source");
    static const CLIPFORMAT cfLinkSourceDescriptor =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"Link Source Descriptor");
    static const CLIPFORMAT cfFileName =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"FileName");
    static const CLIPFORMAT cfFileNameW =
        (CLIPFORMAT)::RegisterClipboardFormatW(L"FileNameW");

    return ::IsClipboardFormatAvailable(cfLinkSource) ||
           ::IsClipboardFormatAvailable(cfLinkSourceDescriptor) ||
           ::IsClipboardFormatAvailable(cfFileName) ||
           ::IsClipboardFormatAvailable(cfFileNameW);
}
// Symbol: ?CanCreateFromData@COleClientItem@@SAHPEBVCOleDataObject@@@Z
// A clipboard-backed data object can always be pasted; an arbitrary
// IDataObject is only usable if it exposes embedded-object creation.
extern "C" int MS_ABI impl__CanCreateFromData_COleClientItem__SAHPEBVCOleDataObject___Z(
        const COleDataObject* pDataObject) {
    if (!pDataObject) return FALSE;
    if (pDataObject->m_bAutoRelease) {
        return impl__CanPaste_COleClientItem__SAHXZ();
    }
    LPDATAOBJECT pData = pDataObject->m_lpDataObject;
    if (!pData) return FALSE;
    // TODO(clean-room): transcribed partially.  Retail queries the data
    // object for embedded-object support; without that IID we conservatively
    // refuse non-clipboard data objects.
    return FALSE;
}
// Symbol: ?CanCreateLinkFromData@COleClientItem@@SAHPEBVCOleDataObject@@@Z
extern "C" int MS_ABI impl__CanCreateLinkFromData_COleClientItem__SAHPEBVCOleDataObject___Z(
        const COleDataObject* pDataObject) {
    if (!pDataObject) return FALSE;
    if (pDataObject->m_bAutoRelease) {
        return impl__CanPasteLink_COleClientItem__SAHXZ();
    }
    LPDATAOBJECT pData = pDataObject->m_lpDataObject;
    if (!pData) return FALSE;
    // TODO(clean-room): transcribed partially (see CanCreateFromData).
    return FALSE;
}
// Symbol: ?GetClipboardData@COleClientItem@@QEAAXPEAVCOleDataSource@@HPEAUtagPOINT@@PEAUtagSIZE@@@Z
// Builds a COleDataSource for this item: embeds the item data and (when
// requested) caches the formats provided by the object itself.
extern "C" void MS_ABI impl__GetClipboardData_COleClientItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
        COleClientItem* pThis, COleDataSource* pDataSource, int bIncludeLink,
        tagPOINT* lpOffset, tagSIZE* lpSize) {
    (void)lpOffset; (void)lpSize;
    if (!pThis || !pDataSource) return;

    STGMEDIUM medium;
    memset(&medium, 0, sizeof(medium));
    impl__GetEmbeddedItemData_COleClientItem__IEAAXPEAUtagSTGMEDIUM___Z(pThis, &medium);
    if (medium.tymed != TYMED_NULL && medium.tymed != 0) {
        static const CLIPFORMAT cfEmbeddedObject =
            (CLIPFORMAT)::RegisterClipboardFormatW(L"Embedded Object");
        pDataSource->CacheData(cfEmbeddedObject, &medium, nullptr);
    }
    if (bIncludeLink) {
        impl__AddCachedData_COleClientItem__IEAAXPEAVCOleDataSource___Z(pThis, pDataSource);
    }
}
// Symbol: ?AddCachedData@COleClientItem@@IEAAXPEAVCOleDataSource@@@Z
// Enumerates the object's advertised formats and delays-render each of them
// through the supplied data source (matches retail's cache strategy).
extern "C" void MS_ABI impl__AddCachedData_COleClientItem__IEAAXPEAVCOleDataSource___Z(
        COleClientItem* pThis, COleDataSource* pDataSource) {
    if (!pThis || !pDataSource || !pThis->m_lpObject) return;

    IDataObject* pData = nullptr;
    if (FAILED(pThis->m_lpObject->QueryInterface(
            IID_IDataObject, reinterpret_cast<void**>(&pData)))) {
        return;
    }
    IEnumFORMATETC* pEnum = nullptr;
    if (SUCCEEDED(pData->EnumFormatEtc(DATADIR_GET, &pEnum))) {
        FORMATETC formatEtc;
        ULONG fetched = 0;
        while (pEnum->Next(1, &formatEtc, &fetched) == S_OK && fetched != 0) {
            pDataSource->DelayRenderData(formatEtc.cfFormat, &formatEtc);
        }
        pEnum->Release();
    }
    pData->Release();
}
// Symbol: ?GetEmbeddedItemData@COleClientItem@@IEAAXPEAUtagSTGMEDIUM@@@Z
// TODO(clean-room): transcribed partially.  Retail creates a compound-doc
// storage (StgCreateDocfile) containing the embedded object and returns a
// TYMED_ISTORAGE medium.  OpenMFC has no declared IStorage member to back
// this, so the medium is left empty.
extern "C" void MS_ABI impl__GetEmbeddedItemData_COleClientItem__IEAAXPEAUtagSTGMEDIUM___Z(
        COleClientItem* pThis, STGMEDIUM* lpMedium) {
    if (!pThis || !lpMedium) return;
    memset(lpMedium, 0, sizeof(STGMEDIUM));
}
// Symbol: ?DoDragDrop@COleClientItem@@QEAAKPEBUtagRECT@@VCPoint@@HK0@Z
// Starts a drag/drop using this item's clipboard data.  Returns the drop
// effect chosen by the drop target.
extern "C" unsigned long MS_ABI impl__DoDragDrop_COleClientItem__QEAAKPEBUtagRECT__VCPoint__HK0_Z(
        COleClientItem* pThis, const tagRECT* lpRect, CPoint pt,
        int dwKeyState, unsigned long dwEffect, unsigned long dwUnused) {
    (void)pt; (void)dwKeyState; (void)dwUnused;
    if (!pThis || !pThis->m_lpObject) return (unsigned long)DROPEFFECT_NONE;

    COleDataSource* pDataSource = new COleDataSource();
    if (!pDataSource) return (unsigned long)DROPEFFECT_NONE;
    impl__GetClipboardData_COleClientItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
        pThis, pDataSource, TRUE, nullptr, nullptr);
    DROPEFFECT dropEffect = pDataSource->DoDragDrop((DWORD)dwEffect, lpRect, nullptr);
    delete pDataSource;
    return (unsigned long)dropEffect;
}
// Symbol: ?GetClassID@COleClientItem@@QEBAXPEAU_GUID@@@Z
// Retail delegates to IOleObject::GetUserClassID and falls back to
// CLSID_NULL on failure (or when no object is present).
extern "C" void MS_ABI impl__GetClassID_COleClientItem__QEBAXPEAU_GUID___Z(
        const COleClientItem* pThis, GUID* pClassID) {
    if (!pClassID) return;
    memset(pClassID, 0, sizeof(GUID));
    if (!pThis || !pThis->m_lpObject) return;
    if (FAILED(pThis->m_lpObject->GetUserClassID(pClassID))) {
        memset(pClassID, 0, sizeof(GUID));
    }
}
// Symbol: ?GetCachedExtent@COleClientItem@@QEAAHPEAUtagSIZE@@W4tagDVASPECT@@@Z
// Retail uses IViewObject2::GetExtent on the cached view when the aspect is
// DVASPECT_DEFAULT (-1); we resolve the stored aspect and ask the object.
extern "C" int MS_ABI impl__GetCachedExtent_COleClientItem__QEAAHPEAUtagSIZE__W4tagDVASPECT___Z(
        COleClientItem* pThis, tagSIZE* lpSize, DVASPECT nDrawAspect) {
    if (!pThis || !lpSize) return FALSE;
    DVASPECT aspect = nDrawAspect;
    if (aspect == (DVASPECT)-1) {
        aspect = (DVASPECT)pThis->m_nDrawAspect;
    }
    if (!pThis->m_lpObject) return FALSE;

    SIZEL size = {0, 0};
    HRESULT hr = pThis->m_lpObject->GetExtent((DWORD)aspect, &size);
    if (FAILED(hr)) return FALSE;
    lpSize->cx = (LONG)size.cx;
    lpSize->cy = (LONG)size.cy;
    return TRUE;
}
// Symbol: ?GetClientSite@COleClientItem@@MEAAPEAUIOleClientSite@@XZ
// TODO(clean-room): transcribed partially.  Retail returns the container's
// IOleClientSite wrapper; OpenMFC does not yet model a client site object.
extern "C" IOleClientSite* MS_ABI impl__GetClientSite_COleClientItem__MEAAPEAUIOleClientSite__XZ(
        COleClientItem* pThis) {
    if (!pThis) return nullptr;
    // TODO(clean-room): return a real IOleClientSite when modeled.
    return nullptr;
}
// Symbol: ?GetIconFromRegistry@COleClientItem@@QEBAPEAUHICON__@@XZ
// Delegates to the static registry lookup using this item's class id.
extern "C" HICON MS_ABI impl__GetIconFromRegistry_COleClientItem__QEBAPEAUHICON____XZ(
        const COleClientItem* pThis) {
    if (!pThis) return nullptr;
    GUID clsid;
    memset(&clsid, 0, sizeof(clsid));
    impl__GetClassID_COleClientItem__QEBAXPEAU_GUID___Z(pThis, &clsid);
    return impl__GetIconFromRegistry_COleClientItem__SAPEAUHICON____AEAU_GUID___Z(clsid);
}
// Symbol: ?GetIconFromRegistry@COleClientItem@@SAPEAUHICON__@@AEAU_GUID@@@Z
// TODO(clean-room): transcribed partially.  Retail reads the
// HKCR\CLSID\{guid}\DefaultIcon value and loads the icon resource; the
// clean-room registry parsing is deferred.
extern "C" HICON MS_ABI impl__GetIconFromRegistry_COleClientItem__SAPEAUHICON____AEAU_GUID___Z(
        GUID& clsid) {
    (void)clsid;
    // TODO(clean-room): implement HKCR\CLSID\{guid}\DefaultIcon lookup.
    return nullptr;
}
// Symbol: ?GetInPlaceWindow@COleClientItem@@QEAAPEAVCWnd@@XZ
// TODO(clean-room): transcribed partially.  Retail returns the in-place
// frame window when the item is open; the frame window is not modeled.
extern "C" CWnd* MS_ABI impl__GetInPlaceWindow_COleClientItem__QEAAPEAVCWnd__XZ(
        COleClientItem* pThis) {
    if (!pThis) return nullptr;
    // TODO(clean-room): return the modeled in-place frame HWND.
    return nullptr;
}
// Symbol: ?GetItemName@COleClientItem@@QEBAXPEA_W@Z
// TODO(clean-room): transcribed partially.  Retail formats the item's unique
// serial number ("%do"); OpenMFC does not model the item-name id member, so
// an empty name is written.
extern "C" void MS_ABI impl__GetItemName_COleClientItem__QEBAXPEA_W_Z(
        const COleClientItem* pThis, wchar_t* lpszBuffer) {
    if (!pThis || !lpszBuffer) return;
    lpszBuffer[0] = L'\0';
}
// Symbol: ?GetItemName@COleClientItem@@QEBAXPEA_WI@Z
// See the one-argument overload; also honors the caller-provided buffer size.
extern "C" void MS_ABI impl__GetItemName_COleClientItem__QEBAXPEA_WI_Z(
        const COleClientItem* pThis, wchar_t* lpszBuffer, unsigned int nMaxLen) {
    if (!pThis || !lpszBuffer || nMaxLen == 0) return;
    lpszBuffer[0] = L'\0';
}
// Symbol: ?CommitItem@COleClientItem@@UEAAXH@Z
// TODO(clean-room): transcribed partially.  Retail commits the item's
// IStorage (0x88) after checking the modified flag (0x7F); neither member is
// modeled in OpenMFC, so this is a no-op.
extern "C" void MS_ABI impl__CommitItem_COleClientItem__UEAAXH_Z(
        COleClientItem* pThis, int bSuccess) {
    (void)bSuccess;
    if (!pThis) return;
    // TODO(clean-room): commit IStorage when storage is modeled.
}
// Symbol: ?GetItemStorage@COleClientItem@@UEAAXXZ
// TODO(clean-room): transcribed partially.  Retail saves the item into the
// container document's storage based on the document's compound-file flag;
// OpenMFC defers storage management.
extern "C" void MS_ABI impl__GetItemStorage_COleClientItem__UEAAXXZ(
        COleClientItem* pThis) {
    if (!pThis) return;
    // TODO(clean-room): implement item storage save/load.
}
// Symbol: ?Delete@COleClientItem@@QEAAXH@Z
// Removes the item from its container document, releases the OLE object
// references and optionally destroys the item itself.
extern "C" void MS_ABI impl__Delete_COleClientItem__QEAAXH_Z(
        COleClientItem* pThis, int bAutoDelete) {
    if (!pThis) return;

    if (pThis->m_pContainerDoc) {
        pThis->m_pContainerDoc->RemoveItem(pThis);
    }
    pThis->m_pDocument = nullptr;
    pThis->m_pContainerDoc = nullptr;

    if (pThis->m_lpLink) {
        pThis->m_lpLink->Release();
        pThis->m_lpLink = nullptr;
    }
    if (pThis->m_lpInPlaceObject) {
        pThis->m_lpInPlaceObject->Release();
        pThis->m_lpInPlaceObject = nullptr;
    }
    if (pThis->m_lpObject) {
        pThis->m_lpObject->Release();
        pThis->m_lpObject = nullptr;
    }
    pThis->m_nStatus = OLE_EMPTY;
    pThis->m_bInPlaceActive = FALSE;

    if (bAutoDelete) {
        delete pThis;
    }
}
// Symbol: ?FreezeLink@COleClientItem@@UEAAHXZ
// TODO(clean-room): transcribed partially.  Retail converts a linked item
// into a static (embedded) item by cloning the link data; OpenMFC defers.
extern "C" int MS_ABI impl__FreezeLink_COleClientItem__UEAAHXZ(
        COleClientItem* pThis) {
    if (!pThis) return FALSE;
    // TODO(clean-room): convert link -> static via OleCreateStaticFromData.
    return FALSE;
}
// COleClientItem::ReportError(int) const — retail (0x26c4c0). Maps the given
// HRESULT to a prompt resource and shows it with a message box; returns
// TRUE when a prompt was shown, FALSE otherwise.
//   E_NOTIMPL(0x80004001) / E_NOINTERFACE(0x80004002) / E_FAIL(0x80004005)
//     -> AFX_IDP_OLE_INIT_FAILED (0xF181)
//   OLE_E_STATIC(0x8004000B) -> AFX_IDP_OLE_CANT_INSERT (0xF180)
//   E_OUTOFMEMORY(0x8007000E) -> AFX_IDP_E_OUTOFMEMORY (0xF10A)
// The message box uses nType = MB_OK(0) and nIDHelp = -1, exactly as retail
// passes them (rdx=0, r8d=-1).
// Symbol: ?ReportError@COleClientItem@@UEBAHJ@Z
extern "C" int MS_ABI impl__ReportError_COleClientItem__UEBAHJ_Z(
    const COleClientItem* pThis, long nCode)
{
    (void)pThis;
    if (nCode == static_cast<long>(0x80004001) || nCode == static_cast<long>(0x80004002) ||
        nCode == static_cast<long>(0x80004005)) {
        impl__AfxMessageBox__YAHIII_Z(0xF181, 0 /* MB_OK */, static_cast<UINT>(-1));
        return TRUE;
    }
    if (nCode == static_cast<long>(0x8004000B) /* OLE_E_STATIC */) {
        impl__AfxMessageBox__YAHIII_Z(0xF180, 0 /* MB_OK */, static_cast<UINT>(-1));
        return TRUE;
    }
    if (nCode == static_cast<long>(0x8007000E) /* E_OUTOFMEMORY */) {
        impl__AfxMessageBox__YAHIII_Z(0xF10A, 0 /* MB_OK */, static_cast<UINT>(-1));
        return TRUE;
    }
    return FALSE;
}
// COleClientItem::Run() — retail (0x24af10):
//     if (m_lpObject) {
//         if (!OleIsRunning(m_lpObject)) {
//             hr = OleRun(m_lpObject);
//             OnRun(hr);                 // virtual, vtable slot 0x1A0
//         }
//     }
// The OnRun notification virtual is not declared in OpenMFC's header, so the
// notification step is skipped (TODO below).
// Symbol: ?Run@COleClientItem@@QEAAXXZ
extern "C" void MS_ABI impl__Run_COleClientItem__QEAAXXZ(COleClientItem* pThis)
{
    if (!pThis || !pThis->m_lpObject) return;
    if (!OleIsRunning(pThis->m_lpObject)) {
        HRESULT hr = OleRun(pThis->m_lpObject);
        // TODO(clean-room): transcribed partially. Retail dispatches the
        // virtual OnRun(HRESULT) (COleClientItem vtable slot 0x1A0) so derived
        // classes can react to the run result; OnRun is not modeled in the
        // OpenMFC header.
        (void)hr;
    }
}
// COleClientItem::SetDrawAspect(DVASPECT) — retail (0x246c20):
//     if (nDrawAspect == DVASPECT_ICON) {
//         // ensure the icon metafile is cached: render via IOleItemContainer
//         // (helpers 0x246760 / 0x246850) and release a temporary
//         // TYMED_MFPICT medium (ReleaseStgMedium).
//     }
//     m_nDrawAspect = nDrawAspect;
//     GetDocument()->SetModifiedFlag();
// Symbol: ?SetDrawAspect@COleClientItem@@UEAAXW4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__SetDrawAspect_COleClientItem__UEAAXW4tagDVASPECT___Z(
    COleClientItem* pThis, int nDrawAspect)
{
    if (!pThis) return;
    if (nDrawAspect == 4 /* DVASPECT_ICON */ && !pThis->GetIconicMetafile()) {
        // TODO(clean-room): transcribed partially. Retail renders and caches
        // the icon metafile through the object's IOleItemContainer render
        // machinery (retail helpers 0x246760/0x246850), then releases a
        // temporary TYMED_MFPICT medium. That render path is not modeled;
        // an absent icon is created lazily by the caller of GetIconicMetafile.
    }
    pThis->m_nDrawAspect = nDrawAspect;
    if (pThis->m_pContainerDoc) {
        pThis->m_pContainerDoc->SetModifiedFlag(TRUE);
    }
}
// COleClientItem::SetItemRects(const RECT*, const RECT*) — retail (0x249140):
//     if (m_lpObject) {
//         pIPO = QueryInterface(m_lpObject, IID_IOleInPlaceObject);
//         if (pIPO) {
//             if (lpPosRect) rcPos = *lpPosRect; else OnGetItemPosition(rcPos);
//             if (lpClipRect) rcClip = *lpClipRect; else <OnGetClipRect virtual, slot 0x120>;
//             hr = pIPO->SetObjectRects(&rcPos, &rcClip);
//             m_nStatus = hr;
//             pIPO->Release();
//             return SUCCEEDED(hr);
//         }
//     }
//     return FALSE;
// The clip-rect default virtual (OnGetClipRect) is not declared in the header;
// its documented action is IOleInPlaceObject::GetClipRect, which is used here.
// Symbol: ?SetItemRects@COleClientItem@@QEAAHPEBUtagRECT@@0@Z
extern "C" int MS_ABI impl__SetItemRects_COleClientItem__QEAAHPEBUtagRECT__0_Z(
    COleClientItem* pThis, const RECT* lpPosRect, const RECT* lpClipRect)
{
    if (!pThis || !pThis->m_lpObject) return FALSE;
    IOleInPlaceObject* pInPlace = nullptr;
    if (FAILED(pThis->m_lpObject->QueryInterface(
            IID_IOleInPlaceObject, reinterpret_cast<void**>(&pInPlace))) ||
        !pInPlace) {
        return FALSE;
    }
    CRect rcPos;
    if (lpPosRect) {
        rcPos = *lpPosRect;
    } else {
        pThis->OnGetItemPosition(rcPos);
    }
    CRect rcClip;
    if (lpClipRect) {
        rcClip = *lpClipRect;
    } else {
        // Retail dispatches the virtual OnGetClipRect (vtable slot 0x120),
        // which queries the object for IID_IOleInPlaceActiveObject and calls
        // its GetClipRect extension. Neither that virtual nor the extension
        // exists in OpenMFC/MinGW headers, so the position rect is reused
        // (a conservative, type-correct stand-in).
        rcClip = rcPos;
    }
    HRESULT hr = pInPlace->SetObjectRects(rcPos, rcClip);
    pThis->m_nStatus = (OLE_STATUS)hr;
    pInPlace->Release();
    return SUCCEEDED(hr);
}
// COleClientItem::SetLinkUpdateOptions(OLEUPDATE) — retail (0x24b240):
//     pLink = QueryInterface(m_lpObject, IID_IOleLink);
//     if (pLink) {
//         hr = pLink->SetUpdateOptions(nUpdateOptions);
//         pLink->Release();
//         OnRun(hr);                   // virtual, vtable slot 0x1A0
//     }
// The OnRun notification virtual is not declared in OpenMFC's header.
// Symbol: ?SetLinkUpdateOptions@COleClientItem@@QEAAXW4tagOLEUPDATE@@@Z
extern "C" void MS_ABI impl__SetLinkUpdateOptions_COleClientItem__QEAAXW4tagOLEUPDATE___Z(
    COleClientItem* pThis, int nUpdateOptions)
{
    if (!pThis || !pThis->m_lpObject) return;
    IOleLink* pLink = nullptr;
    if (FAILED(pThis->m_lpObject->QueryInterface(
            IID_IOleLink, reinterpret_cast<void**>(&pLink))) || !pLink) {
        return;
    }
    HRESULT hr = pLink->SetUpdateOptions(
        static_cast<OLEUPDATE>(nUpdateOptions));
    pLink->Release();
    // TODO(clean-room): transcribed partially. Retail then dispatches the
    // virtual OnRun(HRESULT) (vtable slot 0x1A0); OnRun is not modeled in the
    // OpenMFC header.
    (void)hr;
}
// COleClientItem::SetPrintDevice(const DVTARGETDEVICE*) — retail (0x24acd0).
// The retail body wraps the target device in an IStream (helper 0x246c90) and
// hands it to the item's storage / IOleItemContainer machinery so the object
// can render for printing. That state is not modeled, so a conservative
// failure is emitted (safe, type-correct).
// Symbol: ?SetPrintDevice@COleClientItem@@QEAAHPEBUtagDVTARGETDEVICE@@@Z
extern "C" int MS_ABI impl__SetPrintDevice_COleClientItem__QEAAHPEBUtagDVTARGETDEVICE___Z(
    COleClientItem* pThis, const tagDVTARGETDEVICE* ptd)
{
    (void)ptd;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail serializes the target
    // device into a stream (CreateStreamOnHGlobal) and passes it through the
    // item's unmodeled storage state; OpenMFC cannot reproduce that path.
    return FALSE;
}
// COleClientItem::SetPrintDevice(const PRINTDLG*) — retail (0x24ae30):
//     ptd = (pPD ? BuildTargetDevice(pPD) : NULL);   // helper 0x2605d8
//     bResult = SetPrintDevice(ptd);
//     if (ptd) CoTaskMemFree(ptd);
//     return bResult;
// Both the DVTARGETDEVICE builder and the ptd overload are unimplementable
// without the unmodeled print/stream state, so the wrapper fails too.
// Symbol: ?SetPrintDevice@COleClientItem@@QEAAHPEBUtagPDW@@@Z
extern "C" int MS_ABI impl__SetPrintDevice_COleClientItem__QEAAHPEBUtagPDW___Z(
    COleClientItem* pThis, const tagPDW* pPD)
{
    (void)pPD;
    if (!pThis) return FALSE;
    // TODO(clean-room): transcribed partially. Retail converts the PRINTDLG
    // (hDevMode/hDevNames) into a DVTARGETDEVICE (helper 0x2605d8) and
    // forwards to SetPrintDevice(const DVTARGETDEVICE*); see that stub.
    return FALSE;
}
// COleClientItem::UpdateItemType() — retail (0x244b20) stores the resolved
// item type into the unmodeled member at offset 0x94:
//     if (QueryInterface(m_lpObject, IID_IOleLink)) { m_nItemType = 1; pLink->Release(); }
//     else if (SUCCEEDED(m_lpObject->GetMiscStatus(DVASPECT_CONTENT, &status)) &&
//              !(status & OLEMISC_STATIC)) m_nItemType = 2;
//     else m_nItemType = 3;
// OpenMFC does not model m_nItemType, so no state can be updated.
// Symbol: ?UpdateItemType@COleClientItem@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateItemType_COleClientItem__QEAAXXZ(
    COleClientItem* pThis)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail sets m_nItemType@0x94 to
    // otLink(1) / otEmbedded(2) / otStatic(3) after querying the object for
    // IOleLink and OLEMISC_STATIC; the member is not modeled in OpenMFC.
}
// COleClientItem::WriteItem(CArchive&) — retail (0x2460a0) dispatches on the
// container document's compound-file flag:
//     if (GetDocument()->m_bCompoundFile) WriteItemCompound(ar);
//     else                               WriteItemFlat(ar);
// Symbol: ?WriteItem@COleClientItem@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteItem_COleClientItem__UEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& ar)
{
    if (!pThis) return;
    if (pThis->m_pContainerDoc && pThis->m_pContainerDoc->m_bCompoundFile) {
        impl__WriteItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(pThis, ar);
    } else {
        impl__WriteItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(pThis, ar);
    }
}
// COleClientItem::WriteItemCompound(CArchive&) — retail (0x245e60). The retail
// body saves the item into a sub-storage of the archive's compound file using
// the item's unmodeled storage state (retail offsets 0x68/0x70) and the
// archive's IStorage; it also forwards to WriteItemFlat. OpenMFC models neither
// the storage members nor the archive storage, so this is a no-op.
// Symbol: ?WriteItemCompound@COleClientItem@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail writes the item into a
    // compound-document sub-storage (item storage members at retail 0x68/0x70
    // and CArchive's IStorage are not modeled in OpenMFC).
}
// COleClientItem::WriteItemFlat(CArchive&) — retail (0x245910). The retail
// body writes the item's CLSID (via IPersist::GetClassID) and its lock-object
// / storage state through the unmodeled members at retail offsets 0x68/0x70
// and CArchive internals. Not reproducible in OpenMFC, so a no-op is emitted.
// Symbol: ?WriteItemFlat@COleClientItem@@QEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__WriteItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& /*ar*/)
{
    if (!pThis) return;
    // TODO(clean-room): transcribed partially. Retail queries the object for
    // IPersist, writes the class id and the item's storage/stream state into
    // the archive; those members and the archive internals are not modeled.
}
// Symbol: ??0COleClientItem@@QEAA@PEAVCOleDocument@@@Z
// COleClientItem::COleClientItem(COleDocument* pContainerDoc)
extern "C" void* MS_ABI impl___0COleClientItem__QEAA_PEAVCOleDocument___Z(COleDocument* pContainerDoc) {
    return new COleClientItem(pContainerDoc);
}
COleClientItem::COleClientItem(COleDocument* pContainerDoc)
    : m_pContainerDoc(pContainerDoc), m_lpObject(nullptr), m_lpLink(nullptr),
      m_lpInPlaceObject(nullptr), m_nStatus(OLE_EMPTY), m_nDrawAspect(DVASPECT_CONTENT),
      m_pControlSite(nullptr), m_lpFrame(nullptr), m_lpDocFrame(nullptr),
      m_bInPlaceActive(FALSE) {
    memset(_oleclientitem_padding, 0, sizeof(_oleclientitem_padding));
    m_pDocument = pContainerDoc;
    GetClientItemState(this, true);
    if (pContainerDoc) AddDocumentItem(pContainerDoc, this);
}
COleClientItem::~COleClientItem() {
    if (m_pContainerDoc) RemoveDocumentItem(m_pContainerDoc, this);
    Close(OLECLOSE_NOSAVE);
    RemoveClientItemState(this);
}
BOOL COleClientItem::CreateFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    IDataObject* dataObject = nullptr;
    HRESULT hr = OleGetClipboard(&dataObject);
    if (FAILED(hr) || !dataObject) return FALSE;
    COleDataObject wrapper;
    wrapper.Attach(dataObject, TRUE);
    return CreateFromData(&wrapper, render, cfFormat, lpFormatEtc);
}
BOOL COleClientItem::CreateNewItem(REFCLSID clsid, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    (void)render; (void)cfFormat; (void)lpFormatEtc;
    Close(OLECLOSE_NOSAVE);
    HRESULT hr = CoCreateInstance(clsid, nullptr, CLSCTX_ALL, IID_IOleObject,
                                  reinterpret_cast<void**>(&m_lpObject));
    if (SUCCEEDED(hr) && m_lpObject) {
        OleSetContainedObject(m_lpObject, TRUE);
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}
BOOL COleClientItem::CreateStaticFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    return CreateFromClipboard(render, cfFormat, lpFormatEtc);
}
BOOL COleClientItem::CreateFromData(COleDataObject* pDataObject, OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    if (!pDataObject) return FALSE;
    IDataObject* dataObject = pDataObject->GetIDataObject(TRUE);
    if (!dataObject) return FALSE;

    Close(OLECLOSE_NOSAVE);
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    HRESULT hr = OleCreateFromData(dataObject, IID_IOleObject, render,
                                   cfFormat || lpFormatEtc ? &format : nullptr,
                                   nullptr, nullptr,
                                   reinterpret_cast<void**>(&m_lpObject));
    dataObject->Release();
    if (SUCCEEDED(hr) && m_lpObject) {
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}
BOOL COleClientItem::CreateLinkFromClipboard(OLERENDER render, CLIPFORMAT cfFormat, FORMATETC* lpFormatEtc) {
    IDataObject* dataObject = nullptr;
    HRESULT hr = OleGetClipboard(&dataObject);
    if (FAILED(hr) || !dataObject) return FALSE;

    Close(OLECLOSE_NOSAVE);
    FORMATETC format = MakeFormatEtc(cfFormat, lpFormatEtc);
    hr = OleCreateLinkFromData(dataObject, IID_IOleObject, render,
                               cfFormat || lpFormatEtc ? &format : nullptr,
                               nullptr, nullptr,
                               reinterpret_cast<void**>(&m_lpObject));
    dataObject->Release();
    if (SUCCEEDED(hr) && m_lpObject) {
        m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&m_lpLink));
        m_nStatus = OLE_LOADED;
        return TRUE;
    }
    m_lpObject = nullptr;
    return FALSE;
}
void COleClientItem::Activate(LONG nVerb, CView* pView, HWND hwndParent, LPCRECT lpRect, LPCRECT lpClipRect, BOOL bSplit) {
    (void)lpClipRect; (void)bSplit;
    if (!m_lpObject) return;
    HWND parent = hwndParent;
    if (!parent && pView) parent = pView->GetSafeHwnd();
    RECT rect = {};
    LPCRECT rectToUse = lpRect;
    if (!rectToUse) {
        CRect itemRect;
        OnGetItemPosition(itemRect);
        rect.left = itemRect.left;
        rect.top = itemRect.top;
        rect.right = itemRect.right;
        rect.bottom = itemRect.bottom;
        rectToUse = &rect;
    }
    HRESULT hr = m_lpObject->DoVerb(nVerb, nullptr, nullptr, 0, parent, rectToUse);
    if (SUCCEEDED(hr)) {
        ClientItemState* state = GetClientItemState(this, true);
        if (state) state->activeVerb = nVerb;
        if (m_lpInPlaceObject) {
            m_lpInPlaceObject->Release();
            m_lpInPlaceObject = nullptr;
        }
        if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject)))) {
            m_bInPlaceActive = TRUE;
        }
        m_nStatus = OLE_RUNNING;
        OnActivate();
    }
}
void COleClientItem::Deactivate() {
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->InPlaceDeactivate();
    }
    m_bInPlaceActive = FALSE;
}
int COleClientItem::DoVerb(LONG nVerb, CView* pView, MSG* lpMsg) {
    if (!m_lpObject) return -1;
    HWND parent = pView ? pView->GetSafeHwnd() : nullptr;
    CRect rect;
    OnGetItemPosition(rect);
    HRESULT hr = m_lpObject->DoVerb(nVerb, lpMsg, nullptr, 0, parent, rect);
    if (FAILED(hr)) return static_cast<int>(hr);
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->activeVerb = nVerb;
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = nullptr;
    }
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IOleInPlaceObject, reinterpret_cast<void**>(&m_lpInPlaceObject)))) {
        m_bInPlaceActive = TRUE;
    }
    m_nStatus = OLE_RUNNING;
    return 0;
}
BOOL COleClientItem::IsInPlaceActive() const {
    return m_bInPlaceActive;
}
OLE_STATUS COleClientItem::GetItemState() const {
    return m_nStatus;
}
void COleClientItem::SetItemState(OLE_STATUS nNewState) {
    m_nStatus = nNewState;
}
void COleClientItem::Close(OLE_CLOSE dwCloseOption) {
    if (m_lpObject) {
        m_lpObject->Close(dwCloseOption);
        m_lpObject->Release();
        m_lpObject = nullptr;
    }
    if (m_lpLink) {
        m_lpLink->Release();
        m_lpLink = nullptr;
    }
    if (m_lpInPlaceObject) {
        m_lpInPlaceObject->Release();
        m_lpInPlaceObject = nullptr;
    }
    m_bInPlaceActive = FALSE;
    m_nStatus = OLE_EMPTY;
}
BOOL COleClientItem::Draw(CDC* pDC, LPCRECT lpBounds, DVASPECT nDrawAspect) {
    if (!m_lpObject || !pDC || !pDC->GetSafeHdc() || !lpBounds) return FALSE;
    return SUCCEEDED(OleDraw(m_lpObject, nDrawAspect, pDC->GetSafeHdc(), lpBounds));
}
BOOL COleClientItem::GetExtent(LPSIZE lpSize, DVASPECT nDrawAspect) {
    if (!m_lpObject || !lpSize) return FALSE;
    SIZEL sizel = {};
    HRESULT hr = m_lpObject->GetExtent(nDrawAspect, &sizel);
    if (FAILED(hr)) return FALSE;
    lpSize->cx = sizel.cx;
    lpSize->cy = sizel.cy;
    return TRUE;
}
BOOL COleClientItem::SetExtent(const CSize& size, DVASPECT nDrawAspect) {
    if (!m_lpObject) return FALSE;
    SIZEL sizel = { size.cx, size.cy };
    return SUCCEEDED(m_lpObject->SetExtent(nDrawAspect, &sizel));
}
void COleClientItem::GetClassID(CLSID* pClassID) {
    if (!pClassID) return;
    *pClassID = CLSID_NULL;
    if (m_lpObject) m_lpObject->GetUserClassID(pClassID);
}
void COleClientItem::CopyToClipboard(BOOL bIncludeLink) {
    (void)bIncludeLink;
    if (!m_lpObject) return;
    IDataObject* dataObject = nullptr;
    if (SUCCEEDED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&dataObject)))) {
        OleSetClipboard(dataObject);
        dataObject->Release();
    }
}
void COleClientItem::OnChange(OLE_NOTIFICATION nCode, DWORD dwParam) {
    (void)dwParam;
    if (nCode == OLE_CHANGED) {
        SetModifiedFlag(TRUE);
        if (m_pContainerDoc) {
            m_pContainerDoc->SetModifiedFlag(TRUE);
            m_pContainerDoc->UpdateAllViews(nullptr, 0, nullptr);
        }
    }
}
void COleClientItem::OnActivate() {
    m_bInPlaceActive = TRUE;
    if (m_nStatus == OLE_EMPTY && m_lpObject) {
        m_nStatus = OLE_LOADED;
    }
    if (m_nStatus == OLE_LOADED || m_nStatus == OLE_OPEN) {
        m_nStatus = OLE_RUNNING;
    }
    if (m_pControlSite) {
        m_pControlSite->Activate(TRUE);
    }
}
void COleClientItem::OnDeactivate() {
    m_bInPlaceActive = FALSE;
    if (m_nStatus == OLE_RUNNING) {
        m_nStatus = OLE_OPEN;
    }
    if (m_pControlSite) {
        m_pControlSite->Activate(FALSE);
    }
}
void COleClientItem::OnGetItemPosition(CRect& rPosition) {
    if (m_pControlSite) {
        CWnd* siteWindow = m_pControlSite->GetWindow();
        HWND hwnd = siteWindow ? siteWindow->GetSafeHwnd() : nullptr;
        if (hwnd) {
            RECT rc = {};
            if (::GetWindowRect(hwnd, &rc)) {
                HWND parent = ::GetParent(hwnd);
                if (parent) {
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.left));
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.right));
                }
                rPosition = rc;
                return;
            }
        }
    }

    if (m_lpInPlaceObject) {
        HWND hwnd = nullptr;
        if (SUCCEEDED(m_lpInPlaceObject->GetWindow(&hwnd)) && hwnd) {
            RECT rc = {};
            if (::GetWindowRect(hwnd, &rc)) {
                HWND parent = ::GetParent(hwnd);
                if (parent) {
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.left));
                    ::ScreenToClient(parent, reinterpret_cast<LPPOINT>(&rc.right));
                }
                rPosition = rc;
                return;
            }
        }
    }

    if (m_pContainerDoc) {
        POSITION pos = m_pContainerDoc->GetFirstViewPosition();
        if (pos) {
            CWnd* pView = static_cast<CWnd*>(m_pContainerDoc->GetNextView(pos));
            if (pView && pView->GetSafeHwnd()) {
                RECT rc = {};
                if (::GetClientRect(pView->GetSafeHwnd(), &rc)) {
                    rPosition = rc;
                    return;
                }
            }
        }
    }

    rPosition = CRect(0, 0, 100, 100);
}
BOOL COleClientItem::OnChangeItemPosition(const CRect& rectPos) {
    if (!m_lpInPlaceObject) return FALSE;
    RECT rect = { rectPos.left, rectPos.top, rectPos.right, rectPos.bottom };
    return SUCCEEDED(m_lpInPlaceObject->SetObjectRects(&rect, &rect));
}
void COleClientItem::OnDiscardUndoState() {
    SetModifiedFlag(FALSE);
    if (m_nStatus == OLE_CHANGED) {
        m_nStatus = OLE_OPEN;
    }
    ClientItemState* state = FindClientItemState(this);
    if (state) {
        if (state->iconicMetafile) {
            GlobalFree(state->iconicMetafile);
            state->iconicMetafile = nullptr;
        }
        if (state->contentMetafile) {
            GlobalFree(state->contentMetafile);
            state->contentMetafile = nullptr;
        }
    }
}
void COleClientItem::SetControlSite(COleControlSite* pSite) {
    m_pControlSite = pSite;
}
COleControlSite* COleClientItem::GetControlSite() const {
    return m_pControlSite;
}
void COleClientItem::Serialize(CArchive& ar) {
    CDocItem::Serialize(ar);
    if (ar.IsStoring()) {
        ar << m_nStatus << m_nDrawAspect << m_bInPlaceActive;
        ClientItemState* state = FindClientItemState(this);
        const BOOL modified = state ? state->modified : FALSE;
        const LONG activeVerb = state ? state->activeVerb : OLEIVERB_PRIMARY;
        ar << modified << activeVerb;
        return;
    }

    ar >> m_nStatus >> m_nDrawAspect >> m_bInPlaceActive;
    ClientItemState* state = GetClientItemState(this, true);
    if (!state) return;
    ar >> state->modified >> state->activeVerb;
}
void COleClientItem::AssertValid() const {
    ClientItemState* state = FindClientItemState(this);
    if (!state) {
        state = GetClientItemState(const_cast<COleClientItem*>(this), true);
    }
    if (state) state->item = const_cast<COleClientItem*>(this);

    if (m_nStatus != OLE_EMPTY && !m_lpObject && !m_lpLink) {
        const_cast<COleClientItem*>(this)->m_nStatus = OLE_EMPTY;
    }
}
void COleClientItem::OnShowItem() {
    if (m_nStatus == OLE_EMPTY) {
        m_nStatus = OLE_LOADED;
    }
    m_bInPlaceActive = TRUE;
}
void COleClientItem::OnOpen() {
    if (m_nStatus < OLE_OPEN) {
        m_nStatus = OLE_OPEN;
    }
    m_bInPlaceActive = TRUE;
}
void COleClientItem::OnClose() {
    if (m_nStatus == OLE_OPEN || m_nStatus == OLE_RUNNING) {
        m_nStatus = OLE_LOADED;
    }
    m_bInPlaceActive = FALSE;
}
BOOL COleClientItem::OnShowControlBars(CFrameWnd* pFrameWnd, BOOL bShow) {
    if (!pFrameWnd) return TRUE;
    pFrameWnd->RedrawWindow(nullptr, nullptr, bShow ? RDW_UPDATENOW : 0);
    return TRUE;
}
HGLOBAL COleClientItem::GetIconicMetafile() {
    ClientItemState* state = FindClientItemState(this);
    if (state && state->iconicMetafile) return DuplicateGlobalMemory(state->iconicMetafile);
    if (!m_lpObject) return nullptr;
    FORMATETC fmt = {};
    fmt.cfFormat = CF_METAFILEPICT;
    fmt.dwAspect = DVASPECT_ICON;
    fmt.lindex = -1;
    fmt.tymed = TYMED_MFPICT;
    STGMEDIUM medium = {};
    IDataObject* data = nullptr;
    if (FAILED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&data))) || !data) return nullptr;
    HRESULT hr = data->GetData(&fmt, &medium);
    data->Release();
    if (FAILED(hr) || medium.tymed != TYMED_MFPICT) {
        ReleaseStgMedium(&medium);
        return nullptr;
    }
    HGLOBAL copy = DuplicateGlobalMemory(medium.hMetaFilePict);
    ReleaseStgMedium(&medium);
    return copy;
}
BOOL COleClientItem::SetIconicMetafile(HGLOBAL hMetaPict) {
    ClientItemState* state = GetClientItemState(this, true);
    if (!state) return FALSE;
    HGLOBAL copy = hMetaPict ? DuplicateGlobalMemory(hMetaPict) : nullptr;
    if (hMetaPict && !copy) return FALSE;
    if (state->iconicMetafile) GlobalFree(state->iconicMetafile);
    state->iconicMetafile = copy;
    m_nDrawAspect = hMetaPict ? DVASPECT_ICON : DVASPECT_CONTENT;
    SetModifiedFlag(TRUE);
    return TRUE;
}
HGLOBAL COleClientItem::GetMetaFile() {
    ClientItemState* state = FindClientItemState(this);
    if (state && state->contentMetafile) return DuplicateGlobalMemory(state->contentMetafile);
    if (!m_lpObject) return nullptr;
    IDataObject* data = nullptr;
    if (FAILED(m_lpObject->QueryInterface(IID_IDataObject, reinterpret_cast<void**>(&data))) || !data) return nullptr;
    FORMATETC fmt = {};
    fmt.cfFormat = CF_METAFILEPICT;
    fmt.dwAspect = DVASPECT_CONTENT;
    fmt.lindex = -1;
    fmt.tymed = TYMED_MFPICT;
    STGMEDIUM medium = {};
    HRESULT hr = data->GetData(&fmt, &medium);
    data->Release();
    if (FAILED(hr) || medium.tymed != TYMED_MFPICT) {
        ReleaseStgMedium(&medium);
        return nullptr;
    }
    HGLOBAL copy = DuplicateGlobalMemory(medium.hMetaFilePict);
    ReleaseStgMedium(&medium);
    return copy;
}
void COleClientItem::SetHostNames(const wchar_t* lpszHost, const wchar_t* lpszHostObj) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) {
        state->hostName = lpszHost ? lpszHost : L"";
        state->hostObjectName = lpszHostObj ? lpszHostObj : L"";
    }

    if (m_lpObject) {
        m_lpObject->SetHostNames(lpszHost ? lpszHost : L"",
                                 lpszHostObj ? lpszHostObj : L"");
    }
}
BOOL COleClientItem::ConvertTo(REFCLSID clsidNew) {
    if (!m_lpObject) return FALSE;
    CLSID clsidOld = CLSID_NULL;
    if (FAILED(m_lpObject->GetUserClassID(&clsidOld))) return FALSE;
    return SUCCEEDED(CoTreatAsClass(clsidOld, clsidNew));
}
BOOL COleClientItem::ActivateAs(REFCLSID clsidNew, REFCLSID clsidOld) {
    return SUCCEEDED(CoTreatAsClass(clsidOld, clsidNew));
}
BOOL COleClientItem::Reload() {
    IPersistStorage* persist = nullptr;
    if (!m_lpObject || FAILED(m_lpObject->QueryInterface(IID_IPersistStorage, reinterpret_cast<void**>(&persist)))) {
        return FALSE;
    }
    HRESULT hr = persist->HandsOffStorage();
    persist->Release();
    return SUCCEEDED(hr);
}
BOOL COleClientItem::UpdateLink() {
    return m_lpLink && SUCCEEDED(m_lpLink->Update(nullptr));
}
BOOL COleClientItem::IsLinkUpToDate() const {
    if (!m_lpObject) return TRUE;
    return m_lpObject->IsUpToDate() == S_OK;
}
BOOL COleClientItem::CanActivate() { return m_lpObject != nullptr; }
BOOL COleClientItem::IsOpen() const { return m_nStatus == OLE_OPEN; }
BOOL COleClientItem::IsRunning() const { return m_nStatus == OLE_RUNNING; }
HRESULT COleClientItem::EnumVerbs(IEnumOLEVERB** ppEnumOleVerb) {
    if (!ppEnumOleVerb) return E_POINTER;
    *ppEnumOleVerb = nullptr;
    if (!m_lpObject) return OLE_E_NOTRUNNING;
    CLSID clsid = CLSID_NULL;
    HRESULT hr = m_lpObject->GetUserClassID(&clsid);
    if (FAILED(hr)) return hr;
    return OleRegEnumVerbs(clsid, ppEnumOleVerb);
}
LONG COleClientItem::GetActiveVerb() const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), false);
    return state ? state->activeVerb : OLEIVERB_PRIMARY;
}
void COleClientItem::SetActiveVerb(LONG nVerb) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->activeVerb = nVerb;
}
BOOL COleClientItem::IsModified() const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), false);
    return state ? state->modified : FALSE;
}
void COleClientItem::SetModifiedFlag(BOOL bModified) {
    ClientItemState* state = GetClientItemState(this, true);
    if (state) state->modified = bModified;
}
void COleClientItem::AttachDataObject(COleDataObject& dataObject) const {
    ClientItemState* state = GetClientItemState(const_cast<COleClientItem*>(this), true);
    if (!state) return;

    if (state->attachedDataObject) {
        state->attachedDataObject->Release();
        state->attachedDataObject = nullptr;
    }

    state->attachedDataObject = dataObject.GetIDataObject(TRUE);
}
