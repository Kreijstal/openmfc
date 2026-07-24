// COM helper X-interface compatibility stubs.
// Symbols are explicit to keep the stub manifest in sync.

#include "openmfc/afxole.h"
#include <cstring>

#ifdef __GNUC__
#  define MS_ABI __attribute__((ms_abi))
#else
#  define MS_ABI
#endif

template <typename T>
static inline void SetOutputNull(T** ppOut) {
    if (ppOut) {
        *ppOut = nullptr;
    }
}

// ---------------------------------------------------------------------------
// XBase
// ---------------------------------------------------------------------------

// Symbol: ??0XBase@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XBase_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int /*type*/, unsigned char /*reserved*/,
    void* /*arg2*/, void* /*arg3*/, void* /*arg4*/, void* /*arg5*/,
    void* /*arg6*/, void* /*arg7*/, void* /*arg8*/, void* /*arg9*/,
    void* /*arg10*/, void* /*arg11*/, void* /*arg12*/, unsigned char /*arg13*/) {
    return pThis;
}

// Symbol: ??1XBase@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XBase_CMFCRibbonInfo__UEAA_XZ(void* /*pThis*/) {
}

// Symbol: ?CreateFromTag@XBase@CMFCRibbonInfo@@SAPEAV12@AEAVXRibbonInfoParser@2@@Z
extern "C" void* MS_ABI impl__CreateFromTag_XBase_CMFCRibbonInfo__SAPEAV12_AEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?CreateFromType@XBase@CMFCRibbonInfo@@SAPEAV12@W4XElementType@2@@Z
extern "C" void* MS_ABI impl__CreateFromType_XBase_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(
    void* pThis, int /*type*/) {
    (void)pThis;
    return nullptr;
}

// Symbol: ?Write@XBase@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XBase_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XCategory
// ---------------------------------------------------------------------------

// Symbol: ??0XCategory@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XCategory_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XCategory@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XCategory_CMFCRibbonInfo__UEAA_XZ(void* /*pThis*/) {
}

// Symbol: ?Read@XCategory@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Write@XCategory@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XCategoryMain
// ---------------------------------------------------------------------------

// Symbol: ??0XCategoryMain@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XCategoryMain_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XCategoryMain@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XCategoryMain_CMFCRibbonInfo__UEAA_XZ(void* /*pThis*/) {
}

// Symbol: ?Read@XCategoryMain@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Write@XCategoryMain@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XContext
// ---------------------------------------------------------------------------

// Symbol: ??0XContext@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XContext_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XContext@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XContext_CMFCRibbonInfo__UEAA_XZ(void* /*pThis*/) {
}

// Symbol: ?Read@XContext@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Write@XContext@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XAdviseSink
// ---------------------------------------------------------------------------

// Symbol: ?OnClose@XAdviseSink@COleClientItem@@UEAAXXZ
extern "C" void MS_ABI impl__OnClose_XAdviseSink_COleClientItem__UEAAXXZ(void* /*pThis*/) {
}

// Symbol: ?OnDataChange@XAdviseSink@COleClientItem@@UEAAXPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" void MS_ABI impl__OnDataChange_XAdviseSink_COleClientItem__UEAAXPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* /*pThis*/, void* /*pFormatEtc*/, void* /*pStgMed*/) {
}

// Symbol: ?OnRename@XAdviseSink@COleClientItem@@UEAAXPEAUIMoniker@@@Z
extern "C" void MS_ABI impl__OnRename_XAdviseSink_COleClientItem__UEAAXPEAUIMoniker___Z(
    void* /*pThis*/, void* /*pMoniker*/) {
}

// Symbol: ?OnSave@XAdviseSink@COleClientItem@@UEAAXXZ
extern "C" void MS_ABI impl__OnSave_XAdviseSink_COleClientItem__UEAAXXZ(void* /*pThis*/) {
}

// Symbol: ?OnViewChange@XAdviseSink@COleClientItem@@UEAAXKJ@Z
extern "C" void MS_ABI impl__OnViewChange_XAdviseSink_COleClientItem__UEAAXKJ_Z(
    void* /*pThis*/, unsigned long /*dwAspect*/, long /*lindex*/) {
}

// ---------------------------------------------------------------------------
// XAmbientProps
// ---------------------------------------------------------------------------

// Symbol: ?GetIDsOfNames@XAmbientProps@COleControlSite@@UEAAJAEBU_GUID@@PEAPEA_WIKPEAJ@Z
extern "C" long MS_ABI impl__GetIDsOfNames_XAmbientProps_COleControlSite__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(
    void* pThis, const void* /*riid*/, wchar_t*** /*rgszNames*/, unsigned int cNames,
    unsigned long /*lcid*/, long* rgDispId) {
    (void)pThis;
    if (rgDispId) {
        for (unsigned int i = 0; i < cNames; ++i) {
            rgDispId[i] = -1;
        }
    }
    return 0;
}

// Symbol: ?GetTypeInfo@XAmbientProps@COleControlSite@@UEAAJIKPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetTypeInfo_XAmbientProps_COleControlSite__UEAAJIKPEAPEAUITypeInfo___Z(
    void* pThis, unsigned int /*iTInfo*/, unsigned long /*lcid*/, void** ppTInfo) {
    (void)pThis;
    SetOutputNull(ppTInfo);
    return 0;
}

// Symbol: ?GetTypeInfoCount@XAmbientProps@COleControlSite@@UEAAJPEAI@Z
extern "C" long MS_ABI impl__GetTypeInfoCount_XAmbientProps_COleControlSite__UEAAJPEAI_Z(
    void* pThis, unsigned int* pctinfo) {
    (void)pThis;
    if (pctinfo) {
        *pctinfo = 0;
    }
    return 0;
}

// Symbol: ?Invoke@XAmbientProps@COleControlSite@@UEAAJJAEBU_GUID@@KGPEAUtagDISPPARAMS@@PEAUtagVARIANT@@PEAUtagEXCEPINFO@@PEAI@Z
extern "C" long MS_ABI impl__Invoke_XAmbientProps_COleControlSite__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(
    void* pThis, long /*dispIdMember*/, const void* /*riid*/, unsigned short /*wFlags*/,
    void* /*pDispParams*/, void* /*pVarResult*/, void* /*pExcepInfo*/, unsigned int* /*puArgErr*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XBoundObjectSite
// ---------------------------------------------------------------------------

// Symbol: ?GetCursor@XBoundObjectSite@COleControlSite@@UEAAJJPEAPEAUICursor@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetCursor_XBoundObjectSite_COleControlSite__UEAAJJPEAPEAUICursor__PEAPEAX_Z(
    void* pThis, long /*nCursorType*/, void* /*ppCursor*/, void** /*ppUnknown*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XClassFactory
// ---------------------------------------------------------------------------

// Symbol: ?CreateInstance@XClassFactory@COleObjectFactory@@UEAAJPEAUIUnknown@@AEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__CreateInstance_XClassFactory_COleObjectFactory__UEAAJPEAUIUnknown__AEBU_GUID__PEAPEAX_Z(
    void* pThis, void* pOuterUnknown, const void* /*riid*/, void** ppvObject) {
    (void)pThis;
    (void)pOuterUnknown;
    if (ppvObject) {
        *ppvObject = nullptr;
    }
    return 0;
}

// Symbol: ?CreateInstanceLic@XClassFactory@COleObjectFactory@@UEAAJPEAUIUnknown@@0AEBU_GUID@@PEA_WPEAPEAX@Z
extern "C" long MS_ABI impl__CreateInstanceLic_XClassFactory_COleObjectFactory__UEAAJPEAUIUnknown__0AEBU_GUID__PEA_WPEAPEAX_Z(
    void* pThis, void* pOuterUnknown, void* pRequestedUnknown, const void* /*riid*/,
    wchar_t* /*pKey*/, void** ppvObject) {
    (void)pThis;
    (void)pOuterUnknown;
    (void)pRequestedUnknown;
    if (ppvObject) {
        *ppvObject = nullptr;
    }
    return 0;
}

// Symbol: ?GetLicInfo@XClassFactory@COleObjectFactory@@UEAAJPEAUtagLICINFO@@@Z
extern "C" long MS_ABI impl__GetLicInfo_XClassFactory_COleObjectFactory__UEAAJPEAUtagLICINFO___Z(
    void* pThis, void* pLicInfo) {
    (void)pThis;
    if (pLicInfo) {
        // Leave zero-initialized; indicate no license support.
        std::memset(pLicInfo, 0, sizeof(tagLICINFO));
    }
    return 0;
}

// Symbol: ?LockServer@XClassFactory@COleObjectFactory@@UEAAJH@Z
extern "C" long MS_ABI impl__LockServer_XClassFactory_COleObjectFactory__UEAAJH_Z(
    void* pThis, int /*fLock*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?RequestLicKey@XClassFactory@COleObjectFactory@@UEAAJKPEAPEA_W@Z
extern "C" long MS_ABI impl__RequestLicKey_XClassFactory_COleObjectFactory__UEAAJKPEAPEA_W_Z(
    void* pThis, unsigned long /*dwReserved*/, wchar_t** ppwszKey) {
    (void)pThis;
    SetOutputNull(ppwszKey);
    return 0;
}

// ---------------------------------------------------------------------------
// XConnPt
// ---------------------------------------------------------------------------

// Symbol: ?Advise@XConnPt@CConnectionPoint@@UEAAJPEAUIUnknown@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XConnPt_CConnectionPoint__UEAAJPEAUIUnknown__PEAK_Z(
    void* pThis, void* pUnkSink, unsigned long* pdwCookie) {
    (void)pThis;
    (void)pUnkSink;
    if (pdwCookie) {
        *pdwCookie = 0;
    }
    return 0;
}

// Symbol: ?EnumConnections@XConnPt@CConnectionPoint@@UEAAJPEAPEAUIEnumConnections@@@Z
extern "C" long MS_ABI impl__EnumConnections_XConnPt_CConnectionPoint__UEAAJPEAPEAUIEnumConnections___Z(
    void* pThis, void** ppEnum) {
    (void)pThis;
    SetOutputNull(ppEnum);
    return 0;
}

// Symbol: ?GetConnectionInterface@XConnPt@CConnectionPoint@@UEAAJPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetConnectionInterface_XConnPt_CConnectionPoint__UEAAJPEAU_GUID___Z(
    void* pThis, void* pIID) {
    (void)pThis;
    (void)pIID;
    return 0;
}

// Symbol: ?GetConnectionPointContainer@XConnPt@CConnectionPoint@@UEAAJPEAPEAUIConnectionPointContainer@@@Z
extern "C" long MS_ABI impl__GetConnectionPointContainer_XConnPt_CConnectionPoint__UEAAJPEAPEAUIConnectionPointContainer___Z(
    void* pThis, void** ppCPC) {
    (void)pThis;
    SetOutputNull(ppCPC);
    return 0;
}

// Symbol: ?Unadvise@XConnPt@CConnectionPoint@@UEAAJK@Z
extern "C" long MS_ABI impl__Unadvise_XConnPt_CConnectionPoint__UEAAJK_Z(
    void* pThis, unsigned long dwCookie) {
    (void)pThis;
    (void)dwCookie;
    return 0;
}

// ---------------------------------------------------------------------------
// XDataObject helpers
// ---------------------------------------------------------------------------

// Symbol: ?DAdvise@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(
    void* pThis, void* pFormatEtc, unsigned long dwConnection, void* pAdviseSink, unsigned long* pdwConnection) {
    (void)pThis; (void)pFormatEtc; (void)dwConnection; (void)pAdviseSink;
    if (pdwConnection) { *pdwConnection = 0; }
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleControl__UEAAJK_Z(
    void* pThis, unsigned long dwConnection) {
    (void)pThis; (void)dwConnection;
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleControl@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleControl__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppEnumAdvise) {
    (void)pThis;
    SetOutputNull(ppEnumAdvise);
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleControl@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleControl__UEAAJKPEAPEAUIEnumFORMATETC___Z(
    void* pThis, unsigned long dwDirection, void** ppEnumFormatEtc) {
    (void)pThis; (void)dwDirection;
    SetOutputNull(ppEnumFormatEtc);
    return 0;
}

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__0_Z(
    void* pThis, void* pFormatEtc, void* pOutFormatEtc) {
    (void)pThis; (void)pFormatEtc; (void)pOutFormatEtc;
    return 0;
}

// Symbol: ?GetData@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleControl__UEAAJPEAUtagFORMATETC___Z(
    void* pThis, void* pFormatEtc) {
    (void)pThis; (void)pFormatEtc;
    return 0;
}

// Symbol: ?SetData@XDataObject@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* pThis, void* pFormatEtc, void* pMedium, int fRelease) {
    (void)pThis; (void)pFormatEtc; (void)pMedium; (void)fRelease;
    return 0;
}

// Symbol: ?DAdvise@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(
    void* pThis, void* pFormatEtc, unsigned long dwConnection, void* pAdviseSink, unsigned long* pdwConnection) {
    (void)pThis; (void)pFormatEtc; (void)dwConnection; (void)pAdviseSink;
    if (pdwConnection) { *pdwConnection = 0; }
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleDataSource@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleDataSource__UEAAJK_Z(
    void* pThis, unsigned long dwConnection) {
    (void)pThis; (void)dwConnection;
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleDataSource@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleDataSource__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppEnumAdvise) {
    (void)pThis;
    SetOutputNull(ppEnumAdvise);
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleDataSource@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleDataSource__UEAAJKPEAPEAUIEnumFORMATETC___Z(
    void* pThis, unsigned long dwDirection, void** ppEnumFormatEtc) {
    (void)pThis; (void)dwDirection;
    SetOutputNull(ppEnumFormatEtc);
    return 0;
}

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__0_Z(
    void* pThis, void* pFormatEtc, void* pOutFormatEtc) {
    (void)pThis; (void)pFormatEtc; (void)pOutFormatEtc;
    return 0;
}

// Symbol: ?GetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC___Z(
    void* pThis, void* pFormatEtc) {
    (void)pThis; (void)pFormatEtc;
    return 0;
}

// Symbol: ?SetData@XDataObject@COleDataSource@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleDataSource__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* pThis, void* pFormatEtc, void* pMedium, int fRelease) {
    (void)pThis; (void)pFormatEtc; (void)pMedium; (void)fRelease;
    return 0;
}

// Symbol: ?DAdvise@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(
    void* pThis, void* pFormatEtc, unsigned long dwConnection, void* pAdviseSink, unsigned long* pdwConnection) {
    (void)pThis; (void)pFormatEtc; (void)dwConnection; (void)pAdviseSink;
    if (pdwConnection) { *pdwConnection = 0; }
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleServerDoc@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleServerDoc__UEAAJK_Z(
    void* pThis, unsigned long dwConnection) {
    (void)pThis; (void)dwConnection;
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleServerDoc@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleServerDoc__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppEnumAdvise) {
    (void)pThis;
    SetOutputNull(ppEnumAdvise);
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleServerDoc@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleServerDoc__UEAAJKPEAPEAUIEnumFORMATETC___Z(
    void* pThis, unsigned long dwDirection, void** ppEnumFormatEtc) {
    (void)pThis; (void)dwDirection;
    SetOutputNull(ppEnumFormatEtc);
    return 0;
}

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__0_Z(
    void* pThis, void* pFormatEtc, void* pOutFormatEtc) {
    (void)pThis; (void)pFormatEtc; (void)pOutFormatEtc;
    return 0;
}

// Symbol: ?GetData@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC___Z(
    void* pThis, void* pFormatEtc) {
    (void)pThis; (void)pFormatEtc;
    return 0;
}

// Symbol: ?SetData@XDataObject@COleServerDoc@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleServerDoc__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* pThis, void* pFormatEtc, void* pMedium, int fRelease) {
    (void)pThis; (void)pFormatEtc; (void)pMedium; (void)fRelease;
    return 0;
}

// Symbol: ?DAdvise@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(
    void* pThis, void* pFormatEtc, unsigned long dwConnection, void* pAdviseSink, unsigned long* pdwConnection) {
    (void)pThis; (void)pFormatEtc; (void)dwConnection; (void)pAdviseSink;
    if (pdwConnection) { *pdwConnection = 0; }
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleServerItem@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleServerItem__UEAAJK_Z(
    void* pThis, unsigned long dwConnection) {
    (void)pThis; (void)dwConnection;
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleServerItem@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleServerItem__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppEnumAdvise) {
    (void)pThis;
    SetOutputNull(ppEnumAdvise);
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleServerItem@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleServerItem__UEAAJKPEAPEAUIEnumFORMATETC___Z(
    void* pThis, unsigned long dwDirection, void** ppEnumFormatEtc) {
    (void)pThis; (void)dwDirection;
    SetOutputNull(ppEnumFormatEtc);
    return 0;
}

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__0_Z(
    void* pThis, void* pFormatEtc, void* pOutFormatEtc) {
    (void)pThis; (void)pFormatEtc; (void)pOutFormatEtc;
    return 0;
}

// Symbol: ?GetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(
    void* pThis, void* pFormatEtc, void* pMedium) {
    (void)pThis; (void)pFormatEtc; (void)pMedium;
    return 0;
}

// Symbol: ?QueryGetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@@Z
extern "C" long MS_ABI impl__QueryGetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC___Z(
    void* pThis, void* pFormatEtc) {
    (void)pThis; (void)pFormatEtc;
    return 0;
}

// Symbol: ?SetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* pThis, void* pFormatEtc, void* pMedium, int fRelease) {
    (void)pThis; (void)pFormatEtc; (void)pMedium; (void)fRelease;
    return 0;
}

// ---------------------------------------------------------------------------
// XDocHostUIHandler
// ---------------------------------------------------------------------------

// Symbol: ?EnableModeless@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(
    void* pThis, int fEnable) {
    (void)pThis;
    (void)fEnable;
    return 0;
}

// Symbol: ?FilterDataObject@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUIDataObject@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__FilterDataObject_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUIDataObject__PEAPEAU3__Z(
    void* pThis, void* pDataObject, void** ppDataObject) {
    (void)pThis;
    (void)pDataObject;
    SetOutputNull(ppDataObject);
    return 0;
}

// Symbol: ?GetDropTarget@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUIDropTarget@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__GetDropTarget_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUIDropTarget__PEAPEAU3__Z(
    void* pThis, void* pDropTarget, void** ppDropTarget) {
    (void)pThis;
    (void)pDropTarget;
    SetOutputNull(ppDropTarget);
    return 0;
}

// Symbol: ?GetExternal@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExternal_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAPEAUIDispatch___Z(
    void* pThis, void** ppDisp) {
    (void)pThis;
    SetOutputNull(ppDisp);
    return 0;
}

// Symbol: ?GetHostInfo@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__GetHostInfo_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAU_DOCHOSTUIINFO___Z(
    void* pThis, void* pInfo) {
    (void)pThis;
    (void)pInfo;
    return 0;
}

// Symbol: ?GetOptionKeyPath@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__GetOptionKeyPath_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAPEA_WK_Z(
    void* pThis, wchar_t** pchKeyPath, unsigned long /*dw*/) {
    (void)pThis;
    SetOutputNull(pchKeyPath);
    return 0;
}

// Symbol: ?HideUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__HideUI_XDocHostUIHandler_CHtmlControlSite__UEAAJXZ(void* /*pThis*/) {
    return 0;
}

// Symbol: ?OnDocWindowActivate@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(
    void* pThis, int fActivate) {
    (void)pThis;
    (void)fActivate;
    return 0;
}

// Symbol: ?OnFrameWindowActivate@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(
    void* pThis, int fActivate) {
    (void)pThis;
    (void)fActivate;
    return 0;
}

// Symbol: ?ResizeBorder@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_XDocHostUIHandler_CHtmlControlSite__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    void* pThis, const void* /*prcBorder*/, void* /*pUIWindow*/, int /*fFrame*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ShowContextMenu@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__ShowContextMenu_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(
    void* pThis, unsigned long dwID, void* /*ppt*/, void* /*pCmdTarget*/, void* /*pDispatchObject*/) {
    (void)pThis;
    (void)dwID;
    return 0;
}

// Symbol: ?ShowUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__ShowUI_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(
    void* pThis, unsigned long dwID, void* /*pActiveObject*/, void* /*pCommandTarget*/,
    void* /*pFrame*/, void* /*pUIWindow*/) {
    (void)pThis;
    (void)dwID;
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUtagMSG__PEBU_GUID__K_Z(
    void* pThis, void* /*pMsg*/, const void* /*pguidCmdGroup*/, unsigned long /*nCmdID*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?TranslateUrl@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__TranslateUrl_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEA_WPEAPEA_W_Z(
    void* pThis, unsigned long dwTranslate, wchar_t* /*pchURLIn*/, wchar_t** ppchTranslatedUrl) {
    (void)pThis;
    (void)dwTranslate;
    SetOutputNull(ppchTranslatedUrl);
    return 0;
}

// Symbol: ?UpdateUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__UpdateUI_XDocHostUIHandler_CHtmlControlSite__UEAAJXZ(void* /*pThis*/) {
    return 0;
}
