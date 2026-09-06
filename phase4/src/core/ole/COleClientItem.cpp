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
// ---- Wave 3 sibling thunks ----
// AfxThrowMemoryException() exists in this DLL only as its impl__ thunk, so it
// is called through that (see the briefing's rule on impl__-only entry points).
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();

// ---- forward declarations for Wave 3 impl_ functions defined later below ----
extern "C" void MS_ABI impl__ReadItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& ar);
extern "C" void MS_ABI impl__ReadItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(
    COleClientItem* pThis, CArchive& ar);

//=============================================================================
// Wave 3 — COleClientItem, decoded from the retail mfc140 exports.
//
// Retail COleClientItem member offsets, read out of the disassembly cited at
// each function below and cross-checked between them.  COleClientItem derives
// CDocItem : CCmdTarget : CObject; CCmdTarget keeps m_dwRef at +0x08 and
// m_pModuleState at +0x38 (the value every nested-interface METHOD_PROLOGUE
// loads), and CDocItem adds m_pDocument at +0x40:
//
//   +0x40  m_pDocument      COleDocument*   GetNewItemNumber       0x242d0a
//   +0x48  m_lpObject       IOleObject*     GetUserType            0x249385
//   +0x50  m_lpViewObject   IViewObject2*   Release                0x242b10
//   +0x58  item number      DWORD           GetItemStorageCompound 0x244118
//   +0x5c  m_nDrawAspect    DVASPECT        GetObjectDescriptorData 0x24589b
//   +0x60  m_scLast         SCODE           CheckGeneral           0x248904
//   +0x68  m_lpStorage      IStorage*       GetItemStorageFlat     0x243cf8
//   +0x70  m_lpLockBytes    ILockBytes*     GetItemStorageFlat     0x243cda
//   +0x78  advise cookie    DWORD           Release                0x242b96
//   +0x90  item state       int             OnActivateUI           0x246c30
//   +0x98  m_pView          CView*          OnScrollBy             0x246ea0
//   +0xa0  saved view style DWORD           OnActivateUI           0x246c6e
//   +0xa8  in-place frame   COleFrameHook*  OnSetMenu              0x2473c6
//   +0xb0  in-place docframe COleFrameHook* OnSetMenu              0x2473bb
//   +0xd8  XOleClientSite sub-object  (OnShowWindow 0x245e7c: lea -0xd8(%rcx))
//   +0xe0  XAdviseSink    sub-object  (OnSave       0x246046: lea -0xe0(%rcx))
//   +0xe8  XOleIPSite     sub-object  (OnUIActivate 0x248216: lea -0xe8(%rcx))
//
// OpenMFC's COleClientItem (include/openmfc/afxole.h) declares only
// m_pContainerDoc, m_lpObject, m_lpLink, m_lpInPlaceObject, m_nStatus,
// m_nDrawAspect, m_pControlSite, m_lpFrame, m_lpDocFrame and m_bInPlaceActive,
// in a different order and at different offsets; there is no shadow struct for
// it under phase4/src/detail.  Every retail member above that OpenMFC does not
// declare — m_lpViewObject, the item number, m_scLast, m_lpStorage,
// m_lpLockBytes, the advise cookie, the item state, m_pView and the two
// in-place frame hooks — is unreachable from here, so the functions that need
// one are left as documented stubs.
//
// The COleClientItem primary vftable is at .rdata rva 0x329c48.  It was located
// by searching the image for the address of OnActivateUI (0x246bc0), which the
// disassembly of XOleIPSite::OnUIActivate shows sits at vftable byte 0x138.
// Slots referenced below.  The names marked (map) are the RVA symbol map's own
// names for those addresses; the ones marked (call site) have no RVA of their
// own — the linker folded their bodies — and are identified by the caller that
// uses that slot, named in brackets:
//   0x0e8 OnChange             0x244b40  (map)
//   0x0f0 OnGetItemPosition    0x002820  (call site: XOleIPSite::
//                                         GetWindowContext fills lprcPosRect)
//   0x100 OnDiscardUndoState   0x002820  (call site: XOleIPSite::
//                                         DiscardUndoState)
//   0x108 OnDeactivateAndUndo  0x2472b0  (map)
//   0x110 OnDeactivateUI       0x246f10  (map)
//   0x120 OnGetClipRect        0x244b70  (call site: XOleIPSite::
//                                         GetWindowContext fills lprcClipRect)
//   0x128 CanActivate          0x246a40  (map)
//   0x130 OnActivate           0x246b60  (map)
//   0x138 OnActivateUI         0x246bc0  (map)
//   0x140 OnDeactivate         0x2471d0  (map)
//   0x148 OnGetWindowContext   0x246dd0  (map)
//   0x150 OnChangeItemPosition 0x2472c0  (map)
//   0x158 OnInsertMenus        0x247360  (map)
//   0x160 OnSetMenu            0x2473b0  (map)
//   0x168 OnRemoveMenus        0x247470  (map)
//   0x170 OnUpdateFrameTitle   0x007260  (call site: COleFrameHook::
//                                         OnUpdateFrameTitle, 0x246790)
//   0x190 GetClientSite        0x248790  (map)
//   0x1a0 CheckGeneral         0x248900  (map)
//   0x1a8 OnDataChange         0x002820  (call site: XAdviseSink::OnDataChange)
//   0x1b8 ReadItem             0x244680  (map)
// 0x002820 is a one-instruction body (`ret`) and 0x007260 a two-instruction one
// (`xor %eax,%eax; ret`); both are identical-code-folded, and the RVA map names
// them after whichever symbol won the fold (CRichEditDoc::UpdateModifiedFlag
// and COleControl::XOleObject::Update), so those names carry no meaning here.
//
// GUIDs referenced by address in the disassembly, read out of .rdata:
//   0x2d79d8 IID_IPersistStorage   0x2d7a08 IID_IDataObject
//   0x2d7aa8 IID_IOleCache         0x2d7b08 IID_IOleLink
//   0x2d7b58 IID_IOleInPlaceObject
//
// Retail's OLE_NOTIFICATION codes, as passed to OnChange by the functions
// below, are OLE_CHANGED=0, OLE_SAVED=1, OLE_CLOSED=2, OLE_RENAMED=3,
// OLE_CHANGED_STATE=4.  OpenMFC's afxole.h defines OLE_CHANGED as 5 and has no
// macro at all for the other four, so the numeric codes are written out
// literally below rather than through those macros.
//
// Retail routes the HRESULT of most of these calls through the protected
// virtual CheckGeneral (vftable byte 0x1a0, 0x248900), whose whole body is
//     m_scLast = 0; if (sc) { m_scLast = sc; if (sc < 0) AfxThrowOleException(sc); }
// OpenMFC's CheckGeneral thunk (above in this file) instead writes the SCODE
// into m_nStatus, which is the item state, not m_scLast — calling it would
// corrupt the item state, so the throw-on-failure half is reproduced inline
// where retail calls it, and that deviation is noted at each site.
//=============================================================================

// Symbol: ?GetItemStorageCompound@COleClientItem@@QEAAXXZ
// COleClientItem::GetItemStorageCompound() — retail 0x2440c0:
//     COleDocument* pDoc = m_pDocument;                       // 0x2440dc
//     if (pDoc == NULL) AfxThrowInvalidArgException();         // 0x2440e3
//                                                              //  -> 0x2441cc
//     if (pDoc->[doc+0x1c8] /*root IStorage*/ == NULL) {
//         pDoc->[doc+0x124] = 0;                               // 0x2440f3
//         if (!pDoc->vftable[0x100]())                         // 0x2440fd
//             AfxThrowMemoryException();                       //  -> 0x2441d2
//     }
//     char szName[0x15];
//     <format helper 0x1ce450>(szName, 0x15, "Embedding %lu",  // 0x24411c
//                              [this+0x58]);
//         // the format string is the ANSI literal at .rdata 0x346e00
//     CString strName(szName);                                 // 0x24413c
//     HRESULT hr = pDoc->[doc+0x1c8]->CreateStorage(strName, 0x11012, 0, 0,
//                      &[this+0x68] /*m_lpStorage*/);          // 0x244142
//         // IStorage::CreateStorage is vftable byte 0x28; 0x11012 is
//         // STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE
//     if (FAILED(hr)) AfxThrowOleException(hr);                //  -> 0x2441c4
// Left as generated: OpenMFC's COleClientItem declares neither the item
// storage nor the item number, and its COleDocument declares no root IStorage,
// so none of the three members this function touches exists.
extern "C" void MS_ABI impl__GetItemStorageCompound_COleClientItem__QEAAXXZ(
        COleClientItem* pThis) {
    (void)pThis;
}

// Symbol: ?GetItemStorageFlat@COleClientItem@@QEAAXXZ
// COleClientItem::GetItemStorageFlat() — retail 0x243cd0:
//     HRESULT hr = CreateILockBytesOnHGlobal(NULL, TRUE,
//                                            &[this+0x70] /*m_lpLockBytes*/);
//     if (FAILED(hr)) AfxThrowOleException(hr);               // 0x243cf3
//     hr = StgCreateDocfileOnILockBytes([this+0x70], 0x1012, 0,
//                                       &[this+0x68] /*m_lpStorage*/);
//         // 0x1012 = STGM_READWRITE|STGM_SHARE_EXCLUSIVE|STGM_CREATE without
//         // STGM_TRANSACTED
//     if (FAILED(hr)) { [this+0x70]->Release(); [this+0x70] = NULL;
//                       AfxThrowOleException(hr); }            // 0x243d23
// Left as generated: OpenMFC's COleClientItem declares neither the ILockBytes
// nor the IStorage member, which are the only two things this function writes.
extern "C" void MS_ABI impl__GetItemStorageFlat_COleClientItem__QEAAXXZ(
        COleClientItem* pThis) {
    (void)pThis;
}

// Symbol: ?GetLinkSourceData@COleClientItem@@IEAAHPEAUtagSTGMEDIUM@@@Z
// COleClientItem::GetLinkSourceData(LPSTGMEDIUM) — transcribed from retail
// 0x245660:
//     LPMONIKER pmk = NULL;
//     IOleLink* pLink = QI(m_lpObject, IID_IOleLink);          // 0x245698
//     if (pLink != NULL) {                                     // 0x2456ff
//         HRESULT hr = pLink->GetSourceMoniker(&pmk);          // vtbl 0x30
//         pLink->Release();
//         if (FAILED(hr)) return FALSE;                        // 0x245729
//     } else {                                                 // 0x2456a9
//         if (FAILED(GetClientSite()->GetMoniker(               // vftable 0x190
//                 OLEGETMONIKER_TEMPFORUSER, OLEWHICHMK_OBJFULL, &pmk)))
//             return FALSE;                                    // vtbl 0x20
//     }
//     LPSTREAM pStream;
//     if (FAILED(CreateStreamOnHGlobal(NULL, TRUE, &pStream)))  // 0x2456eb
//         { pmk->Release(); AfxThrowMemoryException(); }
//     HRESULT hr = OleSaveToStream(pmk, pStream);               // 0x245731
//     pmk->Release();
//     if (FAILED(hr)) { pStream->Release(); AfxThrowOleException(hr); }
//     CLSID clsid;
//     hr = m_lpObject->GetUserClassID(&clsid);                  // vtbl 0x78
//     if (FAILED(hr)) { pStream->Release(); AfxThrowOleException(hr); }
//     hr = WriteClassStm(pStream, clsid);                       // 0x245771
//     if (FAILED(hr)) { pStream->Release(); AfxThrowOleException(hr); }
//     lpMedium->tymed = TYMED_ISTREAM; lpMedium->pstm = pStream;
//     lpMedium->pUnkForRelease = NULL; return TRUE;             // 0x245781
// Deviations, all of them guards retail does not have:
//  * retail dereferences the GetClientSite() result without a null check
//    (0x2456ca).  OpenMFC's GetClientSite thunk returns NULL until a client
//    site is modelled, so the null case returns FALSE here instead of
//    faulting — which means this function currently always returns FALSE for a
//    non-link item;
//  * `pThis`, `lpMedium`, `m_lpObject` and the moniker the callee produced are
//    all null-checked here; retail dereferences every one of them unchecked.
extern "C" int MS_ABI impl__GetLinkSourceData_COleClientItem__IEAAHPEAUtagSTGMEDIUM___Z(
        COleClientItem* pThis, STGMEDIUM* lpMedium) {
    if (!pThis || !lpMedium) return FALSE;

    LPMONIKER pmk = nullptr;
    IOleLink* pLink = nullptr;
    if (pThis->m_lpObject) {
        pThis->m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&pLink));
    }
    if (pLink) {
        HRESULT hrMk = pLink->GetSourceMoniker(&pmk);
        pLink->Release();
        if (FAILED(hrMk)) return FALSE;
    } else {
        IOleClientSite* pSite =
            impl__GetClientSite_COleClientItem__MEAAPEAUIOleClientSite__XZ(pThis);
        if (!pSite) return FALSE;
        if (FAILED(pSite->GetMoniker(OLEGETMONIKER_TEMPFORUSER,
                                     OLEWHICHMK_OBJFULL, &pmk))) {
            return FALSE;
        }
    }
    if (!pmk) return FALSE;

    LPSTREAM pStream = nullptr;
    if (FAILED(CreateStreamOnHGlobal(nullptr, TRUE, &pStream)) || !pStream) {
        pmk->Release();
        impl__AfxThrowMemoryException__YAXXZ();
        return FALSE;
    }
    HRESULT hr = OleSaveToStream(pmk, pStream);
    pmk->Release();
    if (FAILED(hr)) {
        pStream->Release();
        AfxThrowOleException(hr);
        return FALSE;
    }
    CLSID clsid = CLSID_NULL;
    hr = pThis->m_lpObject ? pThis->m_lpObject->GetUserClassID(&clsid) : E_UNEXPECTED;
    if (FAILED(hr)) {
        pStream->Release();
        AfxThrowOleException(hr);
        return FALSE;
    }
    hr = WriteClassStm(pStream, clsid);
    if (FAILED(hr)) {
        pStream->Release();
        AfxThrowOleException(hr);
        return FALSE;
    }
    lpMedium->tymed = TYMED_ISTREAM;
    lpMedium->pstm = pStream;
    lpMedium->pUnkForRelease = nullptr;
    return TRUE;
}

// Symbol: ?GetLinkUpdateOptions@COleClientItem@@QEAA?AW4tagOLEUPDATE@@XZ
// COleClientItem::GetLinkUpdateOptions() — transcribed from retail 0x249690:
//     DWORD dwUpdateOpt;                       // uninitialised stack slot
//     IOleLink* pLink = QI(m_lpObject, IID_IOleLink);       // 0x2496a9
//     HRESULT hr = pLink->GetUpdateOptions(&dwUpdateOpt);   // vtbl 0x20
//     pLink->Release();                                     // vtbl 0x10
//     CheckGeneral(hr);                                     // vftable 0x1a0
//     return dwUpdateOpt;                                   // 0x2496f1
// The generated signature for this thunk was wrong (eight parameters from a
// bad demangle); the real one is (this) returning the OLEUPDATE enum in eax.
// Two deviations, both because retail cannot be reproduced safely here:
//  * retail never null-checks the QueryInterface result and faults when the
//    object is not a link; this returns the default instead;
//  * retail's CheckGeneral throws on a negative SCODE — reproduced inline,
//    because OpenMFC's CheckGeneral would also overwrite m_nStatus (see the
//    block comment above).
extern "C" int MS_ABI impl__GetLinkUpdateOptions_COleClientItem__QEAA_AW4tagOLEUPDATE__XZ(
        COleClientItem* pThis) {
    DWORD dwUpdateOpt = OLEUPDATE_ALWAYS;
    if (!pThis) return static_cast<int>(dwUpdateOpt);

    IOleLink* pLink = nullptr;
    if (pThis->m_lpObject) {
        pThis->m_lpObject->QueryInterface(IID_IOleLink, reinterpret_cast<void**>(&pLink));
    }
    if (!pLink) return static_cast<int>(dwUpdateOpt);

    HRESULT hr = pLink->GetUpdateOptions(&dwUpdateOpt);
    pLink->Release();
    if (FAILED(hr)) AfxThrowOleException(hr);
    return static_cast<int>(dwUpdateOpt);
}

// Symbol: ?GetNewItemNumber@COleClientItem@@QEAAKXZ
// COleClientItem::GetNewItemNumber() — retail 0x242d00:
//     COleDocument* pDoc = m_pDocument;                   // 0x242d0a
//     DWORD n = pDoc->[doc+0x1b8];   // the document's next-item-number seed
//     for (;;) {
//         POSITION pos = pDoc->vftable[0x238]();          // GetStartPosition
//         CDocItem* p;
//         while ((p = pDoc->GetNextItemOfKind(pos, RUNTIME_CLASS(COleClientItem)
//                        /* .rdata 0x32c5f8 */)) != NULL) // 0x2531e0
//             if (p->[item+0x58] == n) break;             // 0x242d2e
//         if (p == NULL) break;                           // 0x242d4f
//         n = n + 1;                                      // 0x242d4c/0x242d54
//     }
//     pDoc->[doc+0x1b8] = n + 1;                          // 0x242d5a
//     return n;                                           // 0x242d58
// i.e. it hands out the first item number not already in use by a sibling
// COleClientItem in the same document and bumps the document's seed past it.
// Left as generated: neither the per-item number (retail item+0x58) nor the
// document's seed (retail doc+0x1b8) is declared by OpenMFC, and OpenMFC's
// COleDocument exposes no GetNextItemOfKind, so there is nothing to compare
// against and nothing to store the result in.
extern "C" unsigned long MS_ABI impl__GetNewItemNumber_COleClientItem__QEAAKXZ(
        COleClientItem* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetObjectDescriptorData@COleClientItem@@IEAAXPEAUtagPOINT@@PEAUtagSIZE@@PEAUtagSTGMEDIUM@@@Z
// COleClientItem::GetObjectDescriptorData(LPPOINT, LPSIZE, LPSTGMEDIUM) —
// retail 0x245810:
//     POINT ptOffset = lpOffset ? *lpOffset : (0,0);
//     if (lpOffset) CDC::DPtoHIMETRIC(NULL, &ptOffset);      // 0x24583a
//     SIZE  sizeItem = lpSize   ? *lpSize   : (0,0);
//     if (lpSize)   CDC::DPtoHIMETRIC(NULL, &sizeItem);      // 0x24585b
//     InternalAddRef();                                      // 0x24586e
//     CString strSource(m_pDocument->[doc+0x48]);            // 0x24587a
//         // [doc+0x48] is a CString member of CDocument — m_strPathName in
//         // the standard CDocument layout, not verified independently here
//     HGLOBAL h = <helper 0x25eb44>(m_lpObject,
//                     m_pDocument->[doc+0x48] ? strSource : NULL,
//                     m_nDrawAspect, ptOffset, &sizeItem);   // 0x2458a3
//     InternalRelease();                                     // 0x2458a8
//     if (h == NULL) AfxThrowMemoryException();              // 0x2458f5
//     lpMedium->tymed = TYMED_HGLOBAL; lpMedium->hGlobal = h;
//     lpMedium->pUnkForRelease = NULL;                       // 0x2458b1
// Left as generated: the whole payload is built by the unexported helper at
// 0x25eb44 (MFC's private AfxOleGetObjectDescriptorData), which was not
// decoded, and OpenMFC has no equivalent — the sibling
// COleServerItem::GetObjectDescriptorData is stubbed for the same reason.
// Writing an OBJECTDESCRIPTOR here from the public format documentation
// instead of from the retail helper would be a guess, not a transcription.
extern "C" void MS_ABI impl__GetObjectDescriptorData_COleClientItem__IEAAXPEAUtagPOINT__PEAUtagSIZE__PEAUtagSTGMEDIUM___Z(
        COleClientItem* pThis, POINT* lpOffset, SIZE* lpSize, STGMEDIUM* lpMedium) {
    (void)pThis; (void)lpOffset; (void)lpSize; (void)lpMedium;
}

// Symbol: ?GetPrintDeviceInfo@COleClientItem@@IEAAHPEAPEAUIOleCache@@PEAPEAUtagDVTARGETDEVICE@@PEAK@Z
// COleClientItem::GetPrintDeviceInfo(IOleCache**, DVTARGETDEVICE**, DWORD*) —
// transcribed from retail 0x245100:
//     *ppOleCache = NULL; *pptd = NULL;                      // 0x24510a
//     IOleCache* pCache = QI(m_lpObject, IID_IOleCache);     // 0x245122
//     if (pCache == NULL) return FALSE;                      // 0x2451b8
//     IEnumSTATDATA* pEnum;
//     if (FAILED(pCache->EnumCache(&pEnum)) || pEnum == NULL) // vtbl 0x28
//         { pCache->Release(); return FALSE; }
//     STATDATA sd;
//     while (pEnum->Next(1, &sd, NULL) == S_OK) {             // vtbl 0x18
//         if (sd.formatetc.ptd != NULL) {                     // 0x245164
//             if (pdwConnection) *pdwConnection = sd.dwConnection; // 0x2451c9
//             *pptd = sd.formatetc.ptd;                       // 0x2451d7
//             pEnum->Release();
//             *ppOleCache = pCache;   // kept alive, handed to the caller
//             return TRUE;                                    // 0x2451e7
//         }
//     }
//     pEnum->Release(); pCache->Release(); return FALSE;      // 0x245193
// i.e. it finds the first cache entry that carries a target device and hands
// the caller that DVTARGETDEVICE, its advise connection, and a still-referenced
// IOleCache.  Deviations: retail leaks the IAdviseSink that IEnumSTATDATA::Next
// AddRefs into every STATDATA it returns, and this releases it (no caller can
// observe the difference); and `pThis`, `m_lpObject`, `ppOleCache` and `pptd`
// are null-checked here, where retail dereferences all four unchecked.
extern "C" int MS_ABI impl__GetPrintDeviceInfo_COleClientItem__IEAAHPEAPEAUIOleCache__PEAPEAUtagDVTARGETDEVICE__PEAK_Z(
        COleClientItem* pThis, IOleCache** ppOleCache, DVTARGETDEVICE** pptd,
        unsigned long* pdwConnection) {
    if (!ppOleCache || !pptd) return FALSE;
    *ppOleCache = nullptr;
    *pptd = nullptr;
    if (!pThis || !pThis->m_lpObject) return FALSE;

    IOleCache* pCache = nullptr;
    if (FAILED(pThis->m_lpObject->QueryInterface(IID_IOleCache,
                                                 reinterpret_cast<void**>(&pCache))) ||
        !pCache) {
        return FALSE;
    }

    IEnumSTATDATA* pEnum = nullptr;
    if (FAILED(pCache->EnumCache(&pEnum)) || !pEnum) {
        pCache->Release();
        return FALSE;
    }

    STATDATA sd;
    memset(&sd, 0, sizeof(sd));
    while (pEnum->Next(1, &sd, nullptr) == S_OK) {
        if (sd.formatetc.ptd != nullptr) {
            if (pdwConnection) *pdwConnection = sd.dwConnection;
            *pptd = sd.formatetc.ptd;
            if (sd.pAdvSink) sd.pAdvSink->Release();
            pEnum->Release();
            *ppOleCache = pCache;
            return TRUE;
        }
        if (sd.pAdvSink) sd.pAdvSink->Release();
        memset(&sd, 0, sizeof(sd));
    }
    pEnum->Release();
    pCache->Release();
    return FALSE;
}

// Symbol: ?GetUserType@COleClientItem@@QEAAXW4tagUSERCLASSTYPE@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
// COleClientItem::GetUserType(USERCLASSTYPE, CString&).  This wide-string
// overload has no RVA in the map (the retail DLL on this host is the ANSI
// mfc140.dll), but its ANSI twin
// ?GetUserType@COleClientItem@@QEAAXW4tagUSERCLASSTYPE@@AEAV?$CStringT@D...@Z
// is at 0x249370 and is the same code:
//     LPOLESTR pszUserType;   // stack slot, NOT pre-initialised by retail
//     HRESULT hr = m_lpObject->GetUserType(nUserClassType, &pszUserType);
//                                                          // vtbl 0x80
//     CheckGeneral(hr);                                    // vftable 0x1a0
//     rString = pszUserType;                               // 0x2493be
//     CoTaskMemFree(pszUserType);                          // 0x2493c8
// The generated signature was wrong (eight parameters from a bad demangle);
// the real one is (this, USERCLASSTYPE, CString&).
// Deviations:
//  * CheckGeneral (0x248900) throws when the SCODE is negative, so retail's
//    assignment and free are reached only when hr >= 0; that throw is
//    reproduced inline here for the reason given in the block comment above,
//    but this frees the string first, where retail (which never gets there)
//    would leak it;
//  * retail assigns and frees whatever the callee left in the uninitialised
//    slot even for a non-negative-but-non-S_OK hr; here the pointer starts
//    NULL and both steps are skipped while it is still NULL;
//  * the m_lpObject null guard is added — retail dereferences it unchecked.
extern "C" void MS_ABI impl__GetUserType_COleClientItem__QEAAXW4tagUSERCLASSTYPE__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        COleClientItem* pThis, int nUserClassType, CString& rString) {
    if (!pThis || !pThis->m_lpObject) return;

    LPOLESTR pszUserType = nullptr;
    HRESULT hr = pThis->m_lpObject->GetUserType(static_cast<DWORD>(nUserClassType),
                                                &pszUserType);
    if (FAILED(hr)) {
        if (pszUserType) CoTaskMemFree(pszUserType);
        AfxThrowOleException(hr);
        return;
    }
    if (pszUserType) {
        rString = pszUserType;
        CoTaskMemFree(pszUserType);
    }
}

// Symbol: ?OnActivateUI@COleClientItem@@UEAAXXZ
// COleClientItem::OnActivateUI() — retail 0x246bc0:
//     CFrameWnd *pMain = NULL, *pDocFrame = NULL;
//     if (OnGetWindowContext(&pMain, &pDocFrame, NULL)) {   // vftable 0x148
//         [this+0xc0] = pMain->vftable[0x320]();            // 0x246bfe
//         pMain->vftable[0x318](TRUE);                      // 0x246c1e
//     }
//     if ([this+0x90] != 4) { OnChange(4 /*OLE_CHANGED_STATE*/, 4);
//                             [this+0x90] = 4; }            // 0x246c30
//     [this+0xa0] = m_pView->GetStyle();                    // 0x246c5d
//     m_pView->ModifyStyle(WS_CLIPCHILDREN /*0x2000000*/, 0, 0); // 0x246c7a
//     IOleInPlaceObject* p = QI(m_lpObject, IID_IOleInPlaceObject);
//     HWND hwnd; if (p->GetWindow(&hwnd) != S_OK) hwnd = NULL;  // 0x246ca1
//         // the test at 0x246ca7 is `test eax,eax; je` — any non-S_OK
//         // HRESULT, S_FALSE included, nulls the handle
//     p->Release();  [this+0xb8] = hwnd;                    // 0x246cd0
//     ... then wires the two COleFrameHook objects at [this+0xa8]/[this+0xb0].
// Left as generated: the item state, the saved view style, m_pView and both
// frame hooks are retail-only members (see the map above), and OpenMFC's
// OLE_* item-state macros do not use retail's numbering either — writing 4
// into OpenMFC's m_nStatus would mean OLE_OPEN, not the in-place-UI-active
// state, and would make IsOpen() start returning TRUE.
extern "C" void MS_ABI impl__OnActivateUI_COleClientItem__UEAAXXZ(
        COleClientItem* pThis) {
    (void)pThis;
}

// Symbol: ?OnDataChange@COleClientItem@@MEAAXPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
// COleClientItem::OnDataChange(LPFORMATETC, LPSTGMEDIUM) — this symbol has no
// RVA of its own; slot 0x1a8 of the COleClientItem vftable (.rdata 0x329c48)
// points at 0x002820, whose whole body is `ret`.  The base implementation
// therefore does nothing with either argument, and derived classes override it;
// XAdviseSink::OnDataChange (0x245f70, below) is what calls it.  Transcribed
// exactly.  The generated signature was missing the `this` parameter.
extern "C" void MS_ABI impl__OnDataChange_COleClientItem__MEAAXPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
        COleClientItem* pThis, FORMATETC* lpFormatEtc, STGMEDIUM* lpMedium) {
    (void)pThis; (void)lpFormatEtc; (void)lpMedium;
}

// Symbol: ?OnDeactivateAndUndo@COleClientItem@@MEAAXXZ
// COleClientItem::OnDeactivateAndUndo() — retail 0x2472b0 is a single
// instruction, `jmp 0x180247520`, and 0x247520 is
// ?DeactivateUI@COleClientItem@@QEAAXXZ.  So the whole body is a tail call to
// DeactivateUI().  Transcribed exactly, through OpenMFC's DeactivateUI thunk.
// (For reference, retail DeactivateUI is:
//      IOleInPlaceObject* p = QI(m_lpObject, IID_IOleInPlaceObject);
//      if (p) { m_scLast = p->UIDeactivate(); p->Release();
//               if (m_scLast >= 0) { if (state == 4) state = 3; return; } }
//      Close(OLECLOSE_SAVEIFDIRTY);
//  OpenMFC's DeactivateUI thunk is its own partial transcription of that.)
extern "C" void MS_ABI impl__OnDeactivateAndUndo_COleClientItem__MEAAXXZ(
        COleClientItem* pThis) {
    if (!pThis) return;
    impl__DeactivateUI_COleClientItem__QEAAXXZ(pThis);
}

// Symbol: ?OnDeactivateUI@COleClientItem@@UEAAXH@Z
// COleClientItem::OnDeactivateUI(BOOL bUndoable) — retail 0x246f10:
//     if ([this+0x90] != 3) { OnChange(4 /*OLE_CHANGED_STATE*/, 3);
//                             [this+0x90] = 3; }              // 0x246f22
//     if (m_pView != NULL && m_pDocument->vftable[0xe0]() != NULL)
//         m_pView->ModifyStyle(WS_CLIPCHILDREN,
//                              [this+0xa0] & WS_CLIPCHILDREN, 0); // 0x246f82
//         // i.e. restores the clip-children bit saved by OnActivateUI
//     CFrameWnd *pMain = NULL, *pDocFrame = NULL;
//     if (OnGetWindowContext(&pMain, &pDocFrame, NULL)) {        // vftable 0x148
//         if (pMain->vftable[0x320]() == 1) {                    // 0x246fc0
//             pMain->vftable[0x318]([this+0xc0]);                // restore
//             pMain->[frame+0x1d0] |= 2; ...                     // 0x246ff1
//         }
//         if (pDocFrame) { pDocFrame->[frame+0x1d0] |= 2; ... }  // 0x247031
//     }
//     bUndoable is not read anywhere in the body.
// Left as generated: the item state, the saved view style, m_pView and the
// frame-window bookkeeping are all retail-only members, and OpenMFC's
// OLE_* macros do not use retail's item-state numbering (see OnActivateUI).
extern "C" void MS_ABI impl__OnDeactivateUI_COleClientItem__UEAAXH_Z(
        COleClientItem* pThis, int bUndoable) {
    (void)pThis; (void)bUndoable;
}

// Symbol: ?OnGetClipboardData@COleClientItem@@UEAAPEAVCOleDataSource@@HPEAUtagPOINT@@PEAUtagSIZE@@@Z
// COleClientItem::OnGetClipboardData(BOOL, LPPOINT, LPSIZE) — transcribed from
// retail 0x248960:
//     void* p = ??2@YAPEAX_K@Z(0x60);          // operator new, 0x002840
//     COleDataSource* pDataSource =
//         p ? ??0COleDataSource@@QEAA@XZ(p) : NULL;  // ctor at 0x252050
//     GetClipboardData(pDataSource, bIncludeLink, lpOffset, lpSize); // 0x248b10
//         // ?GetClipboardData@COleClientItem@@QEAAXPEAVCOleDataSource@@H
//         //  PEAUtagPOINT@@PEAUtagSIZE@@@Z, confirmed by RVA lookup
//     return pDataSource;                               // 0x2489b7
// The GetClipboardData call site is followed by a `nop` (0x2489b6), i.e. it
// sits inside a try region; the handler funclet was not decoded, so nothing is
// claimed about it here.  Deviations: retail tolerates a failed allocation and
// calls GetClipboardData with NULL, whereas OpenMFC's operator new throws
// instead of returning NULL.
extern "C" COleDataSource* MS_ABI impl__OnGetClipboardData_COleClientItem__UEAAPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
        COleClientItem* pThis, int bIncludeLink, POINT* lpOffset, SIZE* lpSize) {
    if (!pThis) return nullptr;
    COleDataSource* pDataSource = new COleDataSource();
    impl__GetClipboardData_COleClientItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(
        pThis, pDataSource, bIncludeLink, lpOffset, lpSize);
    return pDataSource;
}

// Symbol: ?OnGetClipRect@COleClientItem@@MEAAXAEAVCRect@@@Z
// COleClientItem::OnGetClipRect(CRect&).  This symbol has no RVA in the map,
// but slot 0x120 of the COleClientItem vftable (.rdata 0x329c48) points at
// 0x244b70, and XOleIPSite::GetWindowContext (0x248341) calls exactly that
// slot to fill the caller's clip rectangle.  The body at 0x244b70 is three
// instructions:
//     mov 0x98(%rcx),%rcx      ; m_pView
//     mov 0x40(%rcx),%rcx      ; m_pView->m_hWnd
//     jmp *GetClientRect       ; GetClientRect(hWnd, &rClipRect)
// (the RVA map names 0x244b70 after COleDocObjectItem::OnGetItemPosition,
// which the linker folded with it; the name carries no meaning here.)
// Deviation: OpenMFC's COleClientItem has no m_pView, so retail's window can
// not be reached.  The window used instead is the item's own — the control
// site's, falling back to the in-place object's — which is the same pair, in
// the same order, that this file's COleClientItem::OnGetItemPosition (line
// ~1206) uses; and with neither available the rectangle is emptied rather than
// left untouched, which retail (an unconditional GetClientRect) never does.
extern "C" void MS_ABI impl__OnGetClipRect_COleClientItem__MEAAXAEAVCRect___Z(
        COleClientItem* pThis, CRect& rClipRect) {
    rClipRect.SetRectEmpty();
    if (!pThis) return;

    HWND hWnd = nullptr;
    if (pThis->m_pControlSite) {
        CWnd* pSiteWnd = pThis->m_pControlSite->GetWindow();
        if (pSiteWnd) hWnd = pSiteWnd->GetSafeHwnd();
    }
    if (!hWnd && pThis->m_lpInPlaceObject) {
        if (FAILED(pThis->m_lpInPlaceObject->GetWindow(&hWnd))) hWnd = nullptr;
    }
    if (hWnd) ::GetClientRect(hWnd, static_cast<RECT*>(rClipRect));
}

// Symbol: ?OnGetWindowContext@COleClientItem@@MEAAHPEAPEAVCFrameWnd@@0PEAUtagOIFI@@@Z
// COleClientItem::OnGetWindowContext(CFrameWnd**, CFrameWnd**, LPOLEINPLACEFRAMEINFO)
// — retail 0x246dd0:
//     if (ppMainFrame == NULL || ppDocFrame == NULL) return 0x80004003;
//         // E_POINTER returned through a BOOL-typed result: the value is
//         // non-zero, so a caller testing it as a BOOL sees TRUE (0x246e79)
//     *ppMainFrame = m_pView->GetTopLevelFrame();               // 0x246dff
//     if (*ppMainFrame == NULL) AfxThrowInvalidArgException();  // 0x246e88
//     CFrameWnd* pDoc = m_pView->GetParentFrame();              // 0x246e13
//     if (pDoc != *ppMainFrame) *ppDocFrame = pDoc;             // 0x246e1d
//     if (lpFrameInfo != NULL) {
//         HACCEL h = m_pDocument->[doc+0x50] /*the document's template*/ ?
//                    that template->[tpl+0x70] : NULL;          // 0x246e25
//         lpFrameInfo->cAccelEntries =
//             h ? CopyAcceleratorTable(h, NULL, 0) : 0;         // 0x246e43
//         lpFrameInfo->haccel   = cAccelEntries ? h : NULL;     // 0x246e58
//         lpFrameInfo->hwndFrame = (*ppMainFrame)->m_hWnd;      // 0x246e65
//         lpFrameInfo->fMDIApp   = (*ppDocFrame != NULL);       // 0x246e6f
//     }
//     return TRUE;
// Left as generated: m_pView is a retail-only member and there is no other way
// to reach the frame windows from an OpenMFC COleClientItem; OpenMFC's
// CDocTemplate also declares no in-place accelerator handle.  The E_POINTER
// path is not reproduced on its own because returning it would report success
// to a BOOL-testing caller while leaving both out-parameters untouched.
extern "C" int MS_ABI impl__OnGetWindowContext_COleClientItem__MEAAHPEAPEAVCFrameWnd__0PEAUtagOIFI___Z(
        COleClientItem* pThis, CFrameWnd** ppMainFrame, CFrameWnd** ppDocFrame,
        void* lpFrameInfo) {
    (void)pThis; (void)lpFrameInfo;
    if (ppMainFrame) *ppMainFrame = nullptr;
    if (ppDocFrame) *ppDocFrame = nullptr;
    return FALSE;
}

// Symbol: ?OnInsertMenus@COleClientItem@@UEAAXPEAVCMenu@@PEAUtagOleMenuGroupWidths@@@Z
// COleClientItem::OnInsertMenus(CMenu*, LPOLEMENUGROUPWIDTHS) — retail
// 0x247360:
//     lpMenuWidths->width[0] = lpMenuWidths->width[2] =
//         lpMenuWidths->width[4] = 0;                          // 0x247364
//     HMENU hMenuInPlace = m_pDocument->[doc+0x50] /*the document's
//                              template*/ ->[tpl+0x68];        // 0x247372
//         // both loads are unguarded: a null document or template faults here
//     if (hMenuInPlace != NULL)
//         AfxMergeMenus(pMenuShared ? pMenuShared->m_hMenu : NULL,
//                       hMenuInPlace, &lpMenuWidths->width[0], 0, 0);
//                                                              // 0x247397
// Left as generated: OpenMFC's CDocTemplate (include/openmfc/afxwin.h) declares
// no in-place menu handle (retail template+0x68), so there is nothing to merge
// and no way to reach one from here.  That is the only blocker — the
// AfxMergeMenus export itself is implemented (phase4/src/featurepack/
// CMFC_misc_stubs.cpp), so this becomes implementable the moment the template
// carries the handle.
extern "C" void MS_ABI impl__OnInsertMenus_COleClientItem__UEAAXPEAVCMenu__PEAUtagOleMenuGroupWidths___Z(
        COleClientItem* pThis, CMenu* pMenuShared, void* lpMenuWidths) {
    (void)pThis; (void)pMenuShared; (void)lpMenuWidths;
}

// Symbol: ?OnRemoveMenus@COleClientItem@@UEAAXPEAVCMenu@@@Z
// COleClientItem::OnRemoveMenus(CMenu*) — retail 0x247470, the exact mirror of
// OnInsertMenus above:
//     HMENU hMenuInPlace = m_pDocument->[doc+0x50] /*the document's
//                              template*/ ->[tpl+0x68];        // 0x247474
//     if (hMenuInPlace != NULL)
//         AfxUnmergeMenus(pMenuShared ? pMenuShared->m_hMenu : NULL,
//                         hMenuInPlace, NULL);                 // 0x247498
// Left as generated for the same reason as OnInsertMenus: OpenMFC's
// CDocTemplate carries no in-place HMENU at template+0x68.
extern "C" void MS_ABI impl__OnRemoveMenus_COleClientItem__UEAAXPEAVCMenu___Z(
        COleClientItem* pThis, CMenu* pMenuShared) {
    (void)pThis; (void)pMenuShared;
}

// Symbol: ?OnScrollBy@COleClientItem@@MEAAHVCSize@@@Z
// COleClientItem::OnScrollBy(CSize) — retail 0x246e90:
//     CSplitterWnd* pSplitter = CView::GetParentSplitter(m_pView, FALSE);
//                                                              // 0x246ea9
//     if (pSplitter != NULL)
//         return pSplitter->vftable[0x370](m_pView, sizeExtent, TRUE);
//                                                              // 0x246ec2
//     return m_pView->vftable[0x2e8](sizeExtent, TRUE);        // 0x246eeb
// i.e. it scrolls through the splitter when the view is in one and otherwise
// asks the view itself, in both cases with bDoScroll = TRUE.
// Left as generated: m_pView is a retail-only member, so neither branch has an
// object to work on.
extern "C" int MS_ABI impl__OnScrollBy_COleClientItem__MEAAHVCSize___Z(
        COleClientItem* pThis, SIZE sizeExtent) {
    (void)pThis; (void)sizeExtent;
    return FALSE;
}

// Symbol: ?OnSetMenu@COleClientItem@@UEAAXPEAVCMenu@@PEAXPEAUHWND__@@@Z
// COleClientItem::OnSetMenu(CMenu*, HOLEMENU, HWND) — retail 0x2473b0:
//     CFrameWnd* pFrame = [this+0xa8] /*in-place frame hook*/ ->[hook+0x40];
//     if ([this+0xb0] /*doc-frame hook*/ != NULL &&
//         [this+0xb0]->[hook+0x40] != pFrame->vftable[0x2f0]())
//         return;                                              // 0x2473fa
//     pFrame->vftable[0x370](pMenuShared ? pMenuShared->m_hMenu : NULL);
//                                                              // 0x247416
//     OleSetMenuDescriptor(hOleMenu, pFrame->m_hWnd, hwndActiveObject,
//                          NULL, NULL);                        // 0x247432
//     if ([this+0xb0] != NULL)
//         OleSetMenuDescriptor(hOleMenu, [this+0xb0]->[hook+0x40]->m_hWnd,
//                              hwndActiveObject, NULL, NULL);  // 0x24745e
// Left as generated: both COleFrameHook members (retail item+0xa8 and +0xb0)
// are unmodelled by OpenMFC, and the body dereferences the first of them
// (0x2473c6/0x2473d6, before any null test) on every path.
extern "C" void MS_ABI impl__OnSetMenu_COleClientItem__UEAAXPEAVCMenu__PEAXPEAUHWND_____Z(
        COleClientItem* pThis, CMenu* pMenuShared, void* hOleMenu, HWND hwndActiveObject) {
    (void)pThis; (void)pMenuShared; (void)hOleMenu; (void)hwndActiveObject;
}

// Symbol: ?OnUpdateFrameTitle@COleClientItem@@UEAAHXZ
// COleClientItem::OnUpdateFrameTitle().  This symbol has no RVA in the map,
// but slot 0x170 of the COleClientItem vftable (.rdata 0x329c48) holds
// 0x007260, and COleFrameHook::OnUpdateFrameTitle (0x246790) confirms the slot
// by calling exactly vftable byte 0x170 on its item after checking that the
// item's in-place frame hook (item+0xa8) is itself.  0x007260 is the folded
// two-instruction body `xor %eax,%eax; ret` — an unconditional FALSE, which is
// MFC's documented default ("the container did not update the title, so the
// caller should").  Transcribed exactly.
extern "C" int MS_ABI impl__OnUpdateFrameTitle_COleClientItem__UEAAHXZ(
        COleClientItem* pThis) {
    (void)pThis;
    return FALSE;   // retail: xor %eax,%eax; ret
}

// Symbol: ?ReactivateAndUndo@COleClientItem@@QEAAHXZ
// COleClientItem::ReactivateAndUndo() — transcribed from retail 0x247690:
//     IOleInPlaceObject* p = QI(m_lpObject, IID_IOleInPlaceObject); // 0x2476a8
//     if (p != NULL) {
//         m_scLast = p->ReactivateAndUndo();          // vtbl 0x40 = slot 8
//         p->Release();
//         if (m_scLast >= 0) return TRUE;             // 0x2476de
//     }
//     Close(OLECLOSE_SAVEIFDIRTY /* 0 */);            // 0x2476ea
//     return FALSE;
// Deviation: OpenMFC declares no m_scLast (retail item+0x60), so the HRESULT is
// only tested, not stored.
extern "C" int MS_ABI impl__ReactivateAndUndo_COleClientItem__QEAAHXZ(
        COleClientItem* pThis) {
    if (!pThis) return FALSE;

    IOleInPlaceObject* pInPlace = nullptr;
    if (pThis->m_lpObject) {
        pThis->m_lpObject->QueryInterface(IID_IOleInPlaceObject,
                                          reinterpret_cast<void**>(&pInPlace));
    }
    if (pInPlace) {
        HRESULT hr = pInPlace->ReactivateAndUndo();
        pInPlace->Release();
        if (SUCCEEDED(hr)) return TRUE;
    }
    pThis->Close(OLECLOSE_SAVEIFDIRTY);
    return FALSE;
}

// Symbol: ?ReadItem@COleClientItem@@UEAAXAEAVCArchive@@@Z
// COleClientItem::ReadItem(CArchive&) — transcribed from retail 0x244680, a
// four-instruction dispatcher:
//     if (m_pDocument->[doc+0x1c0] != 0)  // the document's compound-file
//                                         // flag: the branch picks the
//                                         // compound reader over the flat one
//         ReadItemCompound(ar);            // jmp 0x2441e0
//     else
//         ReadItemFlat(ar);                // jmp 0x243d50
// OpenMFC's COleDocument does declare m_bCompoundFile, so the dispatch itself
// is reproduced; both targets are still stubs (see immediately below), so the
// call is currently a no-op either way.
extern "C" void MS_ABI impl__ReadItem_COleClientItem__UEAAXAEAVCArchive___Z(
        COleClientItem* pThis, CArchive& ar) {
    if (!pThis) return;
    if (pThis->m_pContainerDoc && pThis->m_pContainerDoc->m_bCompoundFile) {
        impl__ReadItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(pThis, ar);
    } else {
        impl__ReadItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(pThis, ar);
    }
}

// Symbol: ?ReadItemCompound@COleClientItem@@QEAAXAEAVCArchive@@@Z
// COleClientItem::ReadItemCompound(CArchive&) — retail 0x2441e0.  It reads the
// item number from the archive, formats "Embedding %lu" from it exactly as
// GetItemStorageCompound (0x2440c0) does, opens that sub-storage out of the
// document's root IStorage (doc+0x1c8) into m_lpStorage (item+0x68), and then
// loads the object from it.
// Left as generated: the item number, m_lpStorage and the document's root
// IStorage are all retail-only members that OpenMFC does not declare, so there
// is nowhere to put anything this function would read.  The sibling writer,
// WriteItemCompound, is stubbed in this file for the same reason.
extern "C" void MS_ABI impl__ReadItemCompound_COleClientItem__QEAAXAEAVCArchive___Z(
        COleClientItem* pThis, CArchive& ar) {
    (void)pThis; (void)ar;
}

// Symbol: ?ReadItemFlat@COleClientItem@@QEAAXAEAVCArchive@@@Z
// COleClientItem::ReadItemFlat(CArchive&) — retail 0x243d50:
//     if (!ar.IsLoading() /*ar+0x20 bit 0*/) ... ;             // 0x243d62
//     DWORD cb = <read 4 bytes from the archive buffer,
//                 refilling it through 0x1cfc70 when short>;   // 0x243d91
//     HGLOBAL h = GlobalAlloc(GMEM_MOVEABLE|GMEM_ZEROINIT /*0x2002*/, cb);
//     if (h == NULL) AfxThrowMemoryException();                // 0x243db2
//     ar.Read(GlobalLock(h), cb);                              // 0x243dca
//     GlobalUnlock(h);
//     if (bytesRead != cb) AfxThrowArchiveException(...);      // 0x243ee2
//     CreateILockBytesOnHGlobal(h, TRUE, &m_lpLockBytes /*item+0x70*/);
//     ... then StgOpenStorageOnILockBytes into m_lpStorage (item+0x68) and
//     loads the object from it.
// Left as generated: m_lpLockBytes and m_lpStorage are retail-only members, so
// the loaded bytes would have nowhere to go.  The sibling writer,
// WriteItemFlat, is stubbed in this file for the same reason.
extern "C" void MS_ABI impl__ReadItemFlat_COleClientItem__QEAAXAEAVCArchive___Z(
        COleClientItem* pThis, CArchive& ar) {
    (void)pThis; (void)ar;
}

// Symbol: ?Release@COleClientItem@@UEAAXW4tagOLECLOSE@@@Z
// COleClientItem::Release(OLE_CLOSE) — retail 0x242b00:
//     m_scLast /*item+0x60*/ = 0;                              // 0x242b14
//     if (m_lpViewObject /*item+0x50*/ != NULL) {
//         DWORD aspects; IAdviseSink* pSink = NULL;
//         m_lpViewObject->GetAdvise(&aspects, NULL, &pSink);   // vtbl 0x40
//         if (pSink) pSink->Release();                         // 0x242b52
//         m_lpViewObject->SetAdvise(aspects, 0, NULL);         // vtbl 0x38
//         m_lpViewObject->Release(); m_lpViewObject = NULL;    // 0x242b85
//     }
//     if (m_lpObject != NULL) {
//         if ([item+0x78] /*advise cookie*/ != 0)
//             { m_lpObject->Unadvise(cookie); cookie = 0; }    // vtbl 0xa0
//         m_scLast = m_lpObject->Close(dwCloseOption);         // vtbl 0x30
//         m_lpObject->Release(); m_lpObject = NULL;            // 0x242bcc
//     }
//     RELEASE(m_lpStorage /*item+0x68*/);                      // 0x242bd1
//     RELEASE(m_lpLockBytes /*item+0x70*/);                    // 0x242bda
//     if ([item+0xa8]) { InternalRelease(); [item+0xa8] = NULL;
//         if ([item+0xb0]) { InternalRelease(); [item+0xb0] = NULL; } }
// The generated signature was wrong (twelve parameters from a bad demangle);
// the real one is (this, OLE_CLOSE).
// Transcribed for the members OpenMFC declares.  Deviations, all forced by the
// layout: OpenMFC has no m_lpViewObject, so the view-advise teardown is absent;
// no advise cookie, so IOleObject::Unadvise is not called; no m_scLast, so the
// Close HRESULT is discarded; no storage, lock bytes or frame hooks.  OpenMFC's
// m_lpLink and m_lpInPlaceObject have no retail counterpart at these offsets
// but are the interfaces this class holds here, and leaving them referenced
// after a Release would leak them, so they are released too.  For the same
// reason m_bInPlaceActive is cleared, which retail has no member for; the
// result is this file's own COleClientItem::Close (line ~1126) minus its
// `m_nStatus = OLE_EMPTY`, which retail's Release does not do either (retail
// writes only m_scLast and its own item state stays where it was).
extern "C" void MS_ABI impl__Release_COleClientItem__UEAAXW4tagOLECLOSE___Z(
        COleClientItem* pThis, unsigned long dwCloseOption) {
    if (!pThis) return;
    if (pThis->m_lpObject) {
        pThis->m_lpObject->Close(dwCloseOption);
        pThis->m_lpObject->Release();
        pThis->m_lpObject = nullptr;
    }
    if (pThis->m_lpLink) {
        pThis->m_lpLink->Release();
        pThis->m_lpLink = nullptr;
    }
    if (pThis->m_lpInPlaceObject) {
        pThis->m_lpInPlaceObject->Release();
        pThis->m_lpInPlaceObject = nullptr;
    }
    pThis->m_bInPlaceActive = FALSE;
}

//=============================================================================
// COleClientItem nested COM interface implementations — XOleClientSite,
// XAdviseSink, XOleIPSite.
//
// Every entry point below opens with MFC's METHOD_PROLOGUE, which recovers the
// owning COleClientItem from the address of an 8-byte (vftable-pointer-only)
// sub-object embedded in it, and then re-enters the item's module state.  The
// three sub-object offsets were read straight out of those prologues in retail
// mfc140.dll and are listed in the block comment at the top of this section:
// item+0xd8 XOleClientSite, item+0xe0 XAdviseSink, item+0xe8 XOleIPSite.
//
// OpenMFC's COleClientItem has none of these sub-objects: it declares no
// vftable members at +0xd8..+0xe8 (its declared members stop at
// m_bInPlaceActive), it hands no such interface pointer out — its GetClientSite
// thunk returns NULL — and no other file in phase4/src references any of these
// thunks (checked by grep).  So `this` here can never be turned back into a
// COleClientItem, and the METHOD_PROLOGUE subtraction would land in unrelated
// memory.  Consequently, following the same rule as the COleControlSite nested
// interfaces in phase4/src/core/ole/COleControlSite.cpp:
//   * a method whose entire retail body is a constant return is transcribed
//     exactly;
//   * a method whose only observable effect for the base class is its return
//     value is given that return value, with the forwarding it skips written
//     down;
//   * every other one is left as generated with the decode recorded.  Its
//     `this` parameter is spelled out only to document the real calling
//     convention — the stub body never dereferences it.
//
// Three of these symbols have no RVA of their own because the linker folded
// their bodies; those were found by reading the sub-object's vftable out of
// .rdata instead.  The two vftables involved are at rva 0x329b80 (XAdviseSink)
// and 0x329e20 (XOleClientSite), located by searching the image for the
// addresses of XAdviseSink::OnDataChange (0x245f70) and
// XOleClientSite::GetContainer (0x245dd0), which sit at IAdviseSink slot 3 and
// IOleClientSite slot 5 respectively.
//=============================================================================

// Symbol: ?CanInPlaceActivate@XOleIPSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleIPSite::CanInPlaceActivate() — retail 0x248160, after
// the module-state prologue:
//     return pThis->CanActivate() ? S_OK : S_FALSE;
// (vftable byte 0x128 = COleClientItem::CanActivate, 0x246a40; the compiler
// spells the mapping `xor %edx,%edx; test %eax,%eax; sete %dl`, so a TRUE from
// CanActivate becomes 0 = S_OK and a FALSE becomes 1 = S_FALSE.)
// Left as generated: the item cannot be recovered from the sub-object pointer,
// so CanActivate cannot be called; returning 0 reports S_OK, which is the
// answer for an item that can be activated.
extern "C" long MS_ABI impl__CanInPlaceActivate_XOleIPSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return 0;   // retail: S_OK when COleClientItem::CanActivate() returns TRUE
}

// Symbol: ?ContextSensitiveHelp@XOleIPSite@COleClientItem@@UEAAJH@Z
// COleClientItem::XOleIPSite::ContextSensitiveHelp(BOOL) — retail 0x248130.
// This one has no module-state prologue; the whole body is:
//     COleFrameHook* pFrame = pThis->[item+0xa8];   // mov -0x40(%rcx),%rcx
//     if (pFrame == NULL) return E_UNEXPECTED;      // 0x8000ffff
//     return (pFrame + 0x80)->vftable[0x20](fEnterMode);
//         // a tail jump into the frame hook's own embedded interface,
//         // IOleWindow::ContextSensitiveHelp at slot 4
// Left as generated: the item is unrecoverable and OpenMFC models no
// COleFrameHook member, which is the null case retail answers with
// E_UNEXPECTED — that is what is returned.
extern "C" long MS_ABI impl__ContextSensitiveHelp_XOleIPSite_COleClientItem__UEAAJH_Z(
        void* pThisItf, int fEnterMode) {
    (void)pThisItf; (void)fEnterMode;
    return E_UNEXPECTED;   // retail: the [item+0xa8] == NULL path, 0x248139
}

// Symbol: ?DeactivateAndUndo@XOleIPSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleIPSite::DeactivateAndUndo() — retail 0x248650, after the
// module-state prologue:
//     pThis->OnDeactivateAndUndo();    // vftable byte 0x108, 0x2472b0
//     return S_OK;                     // xor %eax,%eax at 0x248687
// Left as generated: the item cannot be recovered, so the virtual is not
// called; the S_OK is reproduced.
extern "C" long MS_ABI impl__DeactivateAndUndo_XOleIPSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return S_OK;   // retail: S_OK after COleClientItem::OnDeactivateAndUndo()
}

// Symbol: ?DiscardUndoState@XOleIPSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleIPSite::DiscardUndoState() — retail 0x2485f0, after the
// module-state prologue:
//     pThis->OnDiscardUndoState();     // vftable byte 0x100
//     return S_OK;                     // xor %eax,%eax at 0x248627
// Slot 0x100 of the COleClientItem vftable is 0x002820, whose body is a bare
// `ret`, so for the base class the call has no effect and the S_OK is the whole
// observable behaviour.  A derived class may override OnDiscardUndoState, and
// that override is what is lost here, because the item cannot be recovered from
// the sub-object pointer.
extern "C" long MS_ABI impl__DiscardUndoState_XOleIPSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return S_OK;
}

// Symbol: ?GetContainer@XOleClientSite@COleClientItem@@UEAAJPEAPEAUIOleContainer@@@Z
// COleClientItem::XOleClientSite::GetContainer(IOleContainer**) — retail
// 0x245dd0, which has no module-state prologue:
//     if (ppContainer == NULL) return E_POINTER;              // 0x245dde
//     COleDocument* pDoc = pThis->m_pDocument;                // 0x245de5
//     if (pDoc == NULL) return E_UNEXPECTED;                  // 0x245df1
//     *ppContainer = pDoc->vftable[0x278]();                  // 0x245dfb
//     return *ppContainer ? S_OK : E_NOINTERFACE;             // 0x245e12
// The E_POINTER test is the first thing the body does and is reproduced.  The
// document cannot be reached (the item is unrecoverable from the sub-object
// pointer), which is observationally the same as retail's null-document case,
// so that path's E_UNEXPECTED is returned.  Deviation: the out-parameter is
// nulled first, which retail does not do on this path, so the caller is not
// handed an uninitialised pointer.
extern "C" long MS_ABI impl__GetContainer_XOleClientSite_COleClientItem__UEAAJPEAPEAUIOleContainer___Z(
        void* pThisItf, IOleContainer** ppContainer) {
    (void)pThisItf;
    if (!ppContainer) return E_POINTER;
    *ppContainer = nullptr;
    return E_UNEXPECTED;
}

// Symbol: ?GetMoniker@XOleClientSite@COleClientItem@@UEAAJKKPEAPEAUIMoniker@@@Z
// COleClientItem::XOleClientSite::GetMoniker(DWORD, DWORD, IMoniker**) —
// retail 0x245b70, after the module-state prologue:
//     if (ppmk == NULL) return E_POINTER;                     // 0x245bb2
//     *ppmk = NULL;                                           // 0x245bc3
//     switch (dwWhichMoniker) {                               // 0x245bca
//       case OLEWHICHMK_CONTAINER: ... (0x245d6c)
//       case OLEWHICHMK_OBJREL:    ... (0x245c48)
//       case OLEWHICHMK_OBJFULL:   // 0x245be1: composes the container moniker
//           GetMoniker(dwAssign, OLEWHICHMK_CONTAINER, &mkCont); // vtbl 0x20
//           GetMoniker(dwAssign, OLEWHICHMK_OBJREL,    &mkItem);
//           ... CreateGenericComposite of the two ...
//       default: break;                                       // 0x245d85
//     }
//     return *ppmk ? S_OK : E_FAIL;                           // 0x245d85:
//         // neg/sbb/not/and $0x80004005 — every branch, the unrecognised
//         // dwWhichMoniker included, falls into this one tail, so an
//         // unrecognised code returns E_FAIL, not E_INVALIDARG
//   The three real branches all read the item's document (item+0x40) and its
//   item name.
// Left as generated apart from the E_POINTER guard: the item, its document and
// its item name are all unreachable from the sub-object pointer.  The
// out-parameter is nulled and E_FAIL returned — which is exactly what the tail
// above computes when no moniker was produced — so no caller is told a moniker
// exists.
extern "C" long MS_ABI impl__GetMoniker_XOleClientSite_COleClientItem__UEAAJKKPEAPEAUIMoniker___Z(
        void* pThisItf, unsigned long dwAssign, unsigned long dwWhichMoniker,
        IMoniker** ppmk) {
    (void)pThisItf; (void)dwAssign; (void)dwWhichMoniker;
    if (!ppmk) return E_POINTER;
    *ppmk = nullptr;
    return E_FAIL;   // retail's tail at 0x245d85 when *ppmk is still NULL
}

// Symbol: ?GetWindow@XOleIPSite@COleClientItem@@UEAAJPEAPEAUHWND__@@@Z
// COleClientItem::XOleIPSite::GetWindow(HWND*) — retail 0x248100, no
// module-state prologue, the whole body:
//     if (phwnd == NULL) return E_POINTER;                    // 0x248105
//     CView* pView = pThis->m_pView;   // mov -0x50(%rcx),%rax
//     *phwnd = pView ? pView->m_hWnd : NULL;                  // 0x248114
//     return *phwnd ? S_OK : E_FAIL;                          // 0x248122
// Reproduced for the reachable half: m_pView is a retail-only member, so this
// takes exactly retail's null-view path — write NULL, report E_FAIL.
extern "C" long MS_ABI impl__GetWindow_XOleIPSite_COleClientItem__UEAAJPEAPEAUHWND_____Z(
        void* pThisItf, HWND* phwnd) {
    (void)pThisItf;
    if (!phwnd) return E_POINTER;
    *phwnd = nullptr;
    return E_FAIL;
}

// Symbol: ?GetWindowContext@XOleIPSite@COleClientItem@@UEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagRECT@@2PEAUtagOIFI@@@Z
// COleClientItem::XOleIPSite::GetWindowContext(IOleInPlaceFrame**,
// IOleInPlaceUIWindow**, LPRECT lprcPosRect, LPRECT lprcClipRect,
// LPOLEINPLACEFRAMEINFO) — retail 0x248270, after the module-state prologue:
//     if (ppFrame == NULL || ppDoc == NULL) return E_POINTER;  // 0x248464
//     *ppFrame = NULL; *ppDoc = NULL;                          // 0x2482df
//     CRect rect(0,0,0,0);
//     pThis->OnGetItemPosition(rect);      // vftable 0x0f0 (0x002820, `ret`)
//     CopyRect(lprcPosRect, &rect);                            // 0x248330
//     pThis->OnGetClipRect(rect);          // vftable 0x120 (0x244b70)
//     CopyRect(lprcClipRect, &rect);                           // 0x248358
//     pThis->OnGetWindowContext(&pMainFrame, &pDocFrame, lpFrameInfo);
//                                          // vftable 0x148 (0x246dd0)
//     ... then publishes the two frame windows' IOleInPlaceFrame /
//     IOleInPlaceUIWindow interfaces through the COleFrameHook objects at
//     item+0xa8 / item+0xb0, creating them if needed.
// Left as generated: the item is unrecoverable from the sub-object pointer, and
// even with it the frame hooks and m_pView that the tail of this function needs
// are not modelled by OpenMFC.  The two interface out-parameters are nulled and
// E_UNEXPECTED reported so the caller is not handed uninitialised pointers.
extern "C" long MS_ABI impl__GetWindowContext_XOleIPSite_COleClientItem__UEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagRECT__2PEAUtagOIFI___Z(
        void* pThisItf, void** ppFrame, void** ppDoc, RECT* lprcPosRect,
        RECT* lprcClipRect, void* lpFrameInfo) {
    (void)pThisItf; (void)lprcPosRect; (void)lprcClipRect; (void)lpFrameInfo;
    if (!ppFrame || !ppDoc) return E_POINTER;
    *ppFrame = nullptr;
    *ppDoc = nullptr;
    return E_UNEXPECTED;
}

// Symbol: ?OnClose@XAdviseSink@COleClientItem@@UEAAXXZ
// COleClientItem::XAdviseSink::OnClose() — retail 0x246090, after the
// module-state prologue, is one call:
//     pThis->OnChange(OLE_CLOSED /*2*/, 0);    // vftable 0x0e8, edx=2, r8d=0
// Left as generated: the item cannot be recovered from the sub-object pointer,
// so the notification cannot be delivered.
extern "C" void MS_ABI impl__OnClose_XAdviseSink_COleClientItem__UEAAXXZ(
        void* pThisItf) {
    (void)pThisItf;
}

// Symbol: ?OnDataChange@XAdviseSink@COleClientItem@@UEAAXPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
// COleClientItem::XAdviseSink::OnDataChange(FORMATETC*, STGMEDIUM*) — retail
// 0x245f70, after the module-state prologue, is one call:
//     pThis->OnDataChange(lpFormatEtc, lpStgMedium);   // vftable 0x1a8
// For the base class that virtual is 0x002820 (`ret`), so nothing happens; a
// derived override is what is lost here, since the item cannot be recovered
// from the sub-object pointer.
extern "C" void MS_ABI impl__OnDataChange_XAdviseSink_COleClientItem__UEAAXPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
        void* pThisItf, FORMATETC* lpFormatEtc, STGMEDIUM* lpMedium) {
    (void)pThisItf; (void)lpFormatEtc; (void)lpMedium;
}

// Symbol: ?OnInPlaceActivate@XOleIPSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleIPSite::OnInPlaceActivate() — retail 0x2481b0, after the
// module-state prologue:
//     pThis->OnActivate();    // vftable byte 0x130 (0x246b60)
//     return S_OK;            // xor %eax,%eax at 0x2481e7
// Left as generated: the item cannot be recovered, so OnActivate is not called;
// the unconditional S_OK is reproduced.
extern "C" long MS_ABI impl__OnInPlaceActivate_XOleIPSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return S_OK;   // retail: S_OK after COleClientItem::OnActivate()
}

// Symbol: ?OnInPlaceDeactivate@XOleIPSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleIPSite::OnInPlaceDeactivate() — retail 0x248590, the
// same shape as OnInPlaceActivate:
//     pThis->OnDeactivate();  // vftable byte 0x140 (0x2471d0)
//     return S_OK;            // xor %eax,%eax at 0x2485c7
// Left as generated for the same reason.
extern "C" long MS_ABI impl__OnInPlaceDeactivate_XOleIPSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return S_OK;   // retail: S_OK after COleClientItem::OnDeactivate()
}

// Symbol: ?OnPosRectChange@XOleIPSite@COleClientItem@@UEAAJPEBUtagRECT@@@Z
// COleClientItem::XOleIPSite::OnPosRectChange(LPCRECT) — retail 0x2486b0,
// after the module-state prologue:
//     CRect rect(0,0,0,0); CopyRect(&rect, lprcPosRect);       // 0x2486fa
//     pThis->OnChangeItemPosition(rect);   // vftable byte 0x150 (0x2472c0)
//     return S_OK;                         // xor %eax,%eax at 0x248718
// (the BOOL the virtual returns is discarded.)  Left as generated: the item
// cannot be recovered from the sub-object pointer.
extern "C" long MS_ABI impl__OnPosRectChange_XOleIPSite_COleClientItem__UEAAJPEBUtagRECT___Z(
        void* pThisItf, const RECT* lprcPosRect) {
    (void)pThisItf; (void)lprcPosRect;
    return S_OK;   // retail: unconditional S_OK
}

// Symbol: ?OnRename@XAdviseSink@COleClientItem@@UEAAXPEAUIMoniker@@@Z
// COleClientItem::XAdviseSink::OnRename(IMoniker*).  This symbol has no RVA in
// the map, but slot 5 of the XAdviseSink vftable (.rdata 0x329b80, the table
// whose slot 3 is XAdviseSink::OnDataChange at 0x245f70) holds 0x002820, whose
// entire body is `ret`.  Unlike its four siblings on the same interface it does
// not even take the module state — it is an empty function that ignores the
// moniker.  Transcribed exactly.  (0x002820 is an identical-code-folded body
// the RVA map names after CRichEditDoc::UpdateModifiedFlag; the name carries no
// meaning here.)
extern "C" void MS_ABI impl__OnRename_XAdviseSink_COleClientItem__UEAAXPEAUIMoniker___Z(
        void* pThisItf, IMoniker* pmk) {
    (void)pThisItf; (void)pmk;
}

// Symbol: ?OnSave@XAdviseSink@COleClientItem@@UEAAXXZ
// COleClientItem::XAdviseSink::OnSave() — retail 0x246040, after the
// module-state prologue, is one call:
//     pThis->OnChange(OLE_SAVED /*1*/, 0);     // vftable 0x0e8, edx=1, r8d=0
// Left as generated: the item cannot be recovered from the sub-object pointer,
// so the notification cannot be delivered.
extern "C" void MS_ABI impl__OnSave_XAdviseSink_COleClientItem__UEAAXXZ(
        void* pThisItf) {
    (void)pThisItf;
}

// Symbol: ?OnShowWindow@XOleClientSite@COleClientItem@@UEAAJH@Z
// COleClientItem::XOleClientSite::OnShowWindow(BOOL) — retail 0x245e70, after
// the module-state prologue:
//     int state = pThis->[item+0x90];                          // 0x245e92
//     if (state != 3 && state != 4) {          // sub $3; cmp $1; jbe
//         int newState = fShow ? 2 : 1;        // neg/sbb/neg/inc at 0x245ea0
//         if (newState != state) {
//             pThis->OnChange(OLE_CHANGED_STATE /*4*/, newState); // vftable 0x0e8
//             pThis->[item+0x90] = newState;                   // 0x245ecb
//         }
//     }
//     return S_OK;                                             // 0x245ee4
// i.e. while the item is not in-place active it moves between the loaded (1)
// and open (2) states and notifies.  Left as generated: the item cannot be
// recovered from the sub-object pointer, and its state member is retail-only.
// The unconditional S_OK is reproduced.
extern "C" long MS_ABI impl__OnShowWindow_XOleClientSite_COleClientItem__UEAAJH_Z(
        void* pThisItf, int fShow) {
    (void)pThisItf; (void)fShow;
    return S_OK;   // retail: unconditional S_OK
}

// Symbol: ?OnUIActivate@XOleIPSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleIPSite::OnUIActivate() — retail 0x248210, after the
// module-state prologue:
//     pThis->OnActivateUI();   // vftable byte 0x138 (0x246bc0)
//     return S_OK;             // xor %eax,%eax at 0x248247
// Left as generated: the item cannot be recovered from the sub-object pointer.
extern "C" long MS_ABI impl__OnUIActivate_XOleIPSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return S_OK;   // retail: S_OK after COleClientItem::OnActivateUI()
}

// Symbol: ?OnUIDeactivate@XOleIPSite@COleClientItem@@UEAAJH@Z
// COleClientItem::XOleIPSite::OnUIDeactivate(BOOL fUndoable) — retail
// 0x248520, after the module-state prologue:
//     pThis->OnDeactivateUI(fUndoable);   // vftable byte 0x110 (0x246f10)
//     return S_OK;                        // xor %eax,%eax at 0x24855f
// Left as generated: the item cannot be recovered from the sub-object pointer.
extern "C" long MS_ABI impl__OnUIDeactivate_XOleIPSite_COleClientItem__UEAAJH_Z(
        void* pThisItf, int fUndoable) {
    (void)pThisItf; (void)fUndoable;
    return S_OK;   // retail: S_OK after COleClientItem::OnDeactivateUI()
}

// Symbol: ?OnViewChange@XAdviseSink@COleClientItem@@UEAAXKJ@Z
// COleClientItem::XAdviseSink::OnViewChange(DWORD aspect, LONG lindex) —
// retail 0x245fe0, after the module-state prologue, is one call:
//     pThis->OnChange(OLE_CHANGED /*0*/, aspect);  // vftable 0x0e8,
//                                                  // edx=0, r8d=aspect
// lindex is not read.  Left as generated: the item cannot be recovered from the
// sub-object pointer, so the notification cannot be delivered.
extern "C" void MS_ABI impl__OnViewChange_XAdviseSink_COleClientItem__UEAAXKJ_Z(
        void* pThisItf, unsigned long aspect, long lindex) {
    (void)pThisItf; (void)aspect; (void)lindex;
}

// Symbol: ?RequestNewObjectLayout@XOleClientSite@COleClientItem@@UEAAJXZ
// COleClientItem::XOleClientSite::RequestNewObjectLayout().  This symbol has no
// RVA in the map, but slot 8 of the XOleClientSite vftable (.rdata 0x329e20,
// the table whose slot 5 is XOleClientSite::GetContainer at 0x245dd0) holds
// 0x00f540, whose entire body is `mov $0x80004001,%eax; ret` — an
// unconditional E_NOTIMPL that touches no item state.  Transcribed exactly.
// (0x00f540 is an identical-code-folded body the RVA map names after
// CDHtmlDialog::UpdateUI; the name carries no meaning here.)
extern "C" long MS_ABI impl__RequestNewObjectLayout_XOleClientSite_COleClientItem__UEAAJXZ(
        void* pThisItf) {
    (void)pThisItf;
    return E_NOTIMPL;   // retail: mov $0x80004001,%eax; ret
}

// Symbol: ?SaveObject@XOleClientSite@COleClientItem@@UEAAJXZ
extern "C" long MS_ABI impl__SaveObject_XOleClientSite_COleClientItem__UEAAJXZ() {
    return 0;
}

// Symbol: ?Scroll@XOleIPSite@COleClientItem@@UEAAJUtagSIZE@@@Z
extern "C" long MS_ABI impl__Scroll_XOleIPSite_COleClientItem__UEAAJUtagSIZE___Z(void* /*struct*/ p0) {
    return 0;
}

// Symbol: ?ShowObject@XOleClientSite@COleClientItem@@UEAAJXZ
extern "C" long MS_ABI impl__ShowObject_XOleClientSite_COleClientItem__UEAAJXZ() {
    return 0;
}
