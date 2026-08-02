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

//-----------------------------------------------------------------------------
// Activation
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// State
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// Clipboard / Data source
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// Query / Get accessors
//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------
// Storage / lifetime
//-----------------------------------------------------------------------------

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
