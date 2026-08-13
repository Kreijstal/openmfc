// COleServerItem — OpenMFC implementation.
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
// Symbol: ?AddOtherClipboardData@COleServerItem@@QEAAXPEAVCOleDataSource@@@Z
extern "C" void MS_ABI impl__AddOtherClipboardData_COleServerItem__QEAAXPEAVCOleDataSource___Z(void* /*class*/* p0) {}

// Symbol: ?DoDragDrop@COleServerItem@@QEAAKPEBUtagRECT@@VCPoint@@HK0@Z
extern "C" unsigned long MS_ABI impl__DoDragDrop_COleServerItem__QEAAKPEBUtagRECT__VCPoint__HK0_Z(const void* /*struct*/* p0, void* /*class*/ p1, int p2, unsigned long p3, const void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?GetClipboardData@COleServerItem@@QEAAXPEAVCOleDataSource@@HPEAUtagPOINT@@PEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__GetClipboardData_COleServerItem__QEAAXPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(void* /*class*/* p0, int p1, void* /*struct*/* p2, void* /*struct*/* p3) {}

// Symbol: ?GetDataObject@COleServerItem@@QEAAPEAUIDataObject@@XZ
extern "C" void* MS_ABI impl__GetDataObject_COleServerItem__QEAAPEAUIDataObject__XZ() {
    return nullptr;
}

// Symbol: ?GetEmbedSourceData@COleServerItem@@QEAAXPEAUtagSTGMEDIUM@@@Z
extern "C" void MS_ABI impl__GetEmbedSourceData_COleServerItem__QEAAXPEAUtagSTGMEDIUM___Z(void* /*struct*/* p0) {}

// Symbol: ?GetLinkSourceData@COleServerItem@@QEAAHPEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__GetLinkSourceData_COleServerItem__QEAAHPEAUtagSTGMEDIUM___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetMetafileData@COleServerItem@@MEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__GetMetafileData_COleServerItem__MEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetMoniker@COleServerItem@@QEAAPEAUIMoniker@@W4tagOLEGETMONIKER@@@Z
extern "C" void* MS_ABI impl__GetMoniker_COleServerItem__QEAAPEAUIMoniker__W4tagOLEGETMONIKER___Z(int /*enum*/ p0, void* p1, void* p2, void* p3, void* p4, void* p5, unsigned char p6, unsigned short p7, unsigned char p8, void* p9, float p10, void* p11, double p12, unsigned int p13, unsigned long p14, unsigned char p15, void* p16) {
    return nullptr;
}

// Symbol: ?GetObjectDescriptorData@COleServerItem@@QEAAXPEAUtagPOINT@@PEAUtagSIZE@@PEAUtagSTGMEDIUM@@@Z
extern "C" void MS_ABI impl__GetObjectDescriptorData_COleServerItem__QEAAXPEAUtagPOINT__PEAUtagSIZE__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2) {}

// Symbol: ?GetOleObject@COleServerItem@@QEAAPEAUIOleObject@@XZ
extern "C" void* MS_ABI impl__GetOleObject_COleServerItem__QEAAPEAUIOleObject__XZ() {
    return nullptr;
}

// Symbol: ?IsBlank@COleServerItem@@MEBAHXZ
extern "C" int MS_ABI impl__IsBlank_COleServerItem__MEBAHXZ() {
    return 0;
}

// Symbol: ?NotifyClient@COleServerItem@@QEAAXW4OLE_NOTIFICATION@@_K@Z
extern "C" void MS_ABI impl__NotifyClient_COleServerItem__QEAAXW4OLE_NOTIFICATION___K_Z(int /*enum*/ p0, void* p1, void* p2, unsigned char p3, bool p4, void* p5, void* p6, unsigned int p7, short p8, unsigned int p9, void* p10, void** p11, unsigned int p12, void* p13, double p14, unsigned __int64 p15) {}

// Symbol: ?OnDoVerb@COleServerItem@@UEAAXJ@Z
extern "C" void MS_ABI impl__OnDoVerb_COleServerItem__UEAAXJ_Z(long p0) {}

// Symbol: ?OnFinalRelease@COleServerItem@@UEAAXXZ
extern "C" void MS_ABI impl__OnFinalRelease_COleServerItem__UEAAXXZ() {}

// Symbol: ?OnGetClipboardData@COleServerItem@@UEAAPEAVCOleDataSource@@HPEAUtagPOINT@@PEAUtagSIZE@@@Z
extern "C" void* MS_ABI impl__OnGetClipboardData_COleServerItem__UEAAPEAVCOleDataSource__HPEAUtagPOINT__PEAUtagSIZE___Z(int p0, void* /*struct*/* p1, void* /*struct*/* p2) {
    return nullptr;
}

// Symbol: ?OnHide@COleServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnHide_COleServerItem__MEAAXXZ() {}

// Symbol: ?OnInitFromData@COleServerItem@@UEAAHPEAVCOleDataObject@@H@Z
extern "C" int MS_ABI impl__OnInitFromData_COleServerItem__UEAAHPEAVCOleDataObject__H_Z(void* /*class*/* p0, int p1) {
    return 0;
}

// Symbol: ?OnOpen@COleServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnOpen_COleServerItem__MEAAXXZ() {}

// Symbol: ?OnQueryUpdateItems@COleServerItem@@UEAAHXZ
extern "C" int MS_ABI impl__OnQueryUpdateItems_COleServerItem__UEAAHXZ() {
    return 0;
}

// Symbol: ?OnRenderData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" int MS_ABI impl__OnRenderData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?OnRenderFileData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAVCFile@@@Z
extern "C" int MS_ABI impl__OnRenderFileData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAVCFile___Z(void* /*struct*/* p0, void* /*class*/* p1) {
    return 0;
}

// Symbol: ?OnRenderGlobalData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAPEAX@Z
extern "C" int MS_ABI impl__OnRenderGlobalData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAPEAX_Z(void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?OnSaveEmbedding@COleServerItem@@MEAAXPEAUIStorage@@@Z
extern "C" void MS_ABI impl__OnSaveEmbedding_COleServerItem__MEAAXPEAUIStorage___Z(void* /*struct*/* p0) {}

// Symbol: ?OnSetColorScheme@COleServerItem@@UEAAHPEBUtagLOGPALETTE@@@Z
extern "C" int MS_ABI impl__OnSetColorScheme_COleServerItem__UEAAHPEBUtagLOGPALETTE___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?OnSetData@COleServerItem@@UEAAHPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" int MS_ABI impl__OnSetData_COleServerItem__UEAAHPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?OnShow@COleServerItem@@MEAAXXZ
extern "C" void MS_ABI impl__OnShow_COleServerItem__MEAAXXZ() {}

// Symbol: ?OnUpdate@COleServerItem@@UEAAXPEAV1@_JPEAVCObject@@W4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__OnUpdate_COleServerItem__UEAAXPEAV1__JPEAVCObject__W4tagDVASPECT___Z(void* /*class*/* p0, int /*enum*/ p1, void* p2, void* p3, void* p4, char p5, void* /*class*/ p6) {}

// Symbol: ?OnUpdateItems@COleServerItem@@UEAAXXZ
extern "C" void MS_ABI impl__OnUpdateItems_COleServerItem__UEAAXXZ() {}

// Symbol: ?Advise@XOleObject@COleServerItem@@UEAAJPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__Advise_XOleObject_COleServerItem__UEAAJPEAUIAdviseSink__PEAK_Z(void* /*struct*/* p0, unsigned long* p1) {
    return 0;
}

// Symbol: ?Close@XOleObject@COleServerItem@@UEAAJK@Z
extern "C" long MS_ABI impl__Close_XOleObject_COleServerItem__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?DAdvise@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@KPEAUIAdviseSink@@PEAK@Z
extern "C" long MS_ABI impl__DAdvise_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__KPEAUIAdviseSink__PEAK_Z(void* /*struct*/* p0, unsigned long p1, void* /*struct*/* p2, unsigned long* p3) {
    return 0;
}

// Symbol: ?DUnadvise@XDataObject@COleServerItem@@UEAAJK@Z
extern "C" long MS_ABI impl__DUnadvise_XDataObject_COleServerItem__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// Symbol: ?DoVerb@XOleObject@COleServerItem@@UEAAJJPEAUtagMSG@@PEAUIOleClientSite@@JPEAUHWND__@@PEBUtagRECT@@@Z
extern "C" long MS_ABI impl__DoVerb_XOleObject_COleServerItem__UEAAJJPEAUtagMSG__PEAUIOleClientSite__JPEAUHWND____PEBUtagRECT___Z(long p0, void* /*struct*/* p1, void* /*struct*/* p2, long p3, void* /*struct*/* p4, const void* /*struct*/* p5) {
    return 0;
}

// Symbol: ?EnumAdvise@XOleObject@COleServerItem@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumAdvise_XOleObject_COleServerItem__UEAAJPEAPEAUIEnumSTATDATA___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?EnumDAdvise@XDataObject@COleServerItem@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumDAdvise_XDataObject_COleServerItem__UEAAJPEAPEAUIEnumSTATDATA___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?EnumFormatEtc@XDataObject@COleServerItem@@UEAAJKPEAPEAUIEnumFORMATETC@@@Z
extern "C" long MS_ABI impl__EnumFormatEtc_XDataObject_COleServerItem__UEAAJKPEAPEAUIEnumFORMATETC___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?EnumVerbs@XOleObject@COleServerItem@@UEAAJPEAPEAUIEnumOLEVERB@@@Z
extern "C" long MS_ABI impl__EnumVerbs_XOleObject_COleServerItem__UEAAJPEAPEAUIEnumOLEVERB___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetCanonicalFormatEtc@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@0@Z
extern "C" long MS_ABI impl__GetCanonicalFormatEtc_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__0_Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetClientSite@XOleObject@COleServerItem@@UEAAJPEAPEAUIOleClientSite@@@Z
extern "C" long MS_ABI impl__GetClientSite_XOleObject_COleServerItem__UEAAJPEAPEAUIOleClientSite___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetClipboardData@XOleObject@COleServerItem@@UEAAJKPEAPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__GetClipboardData_XOleObject_COleServerItem__UEAAJKPEAPEAUIDataObject___Z(unsigned long p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?GetData@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetData_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetDataHere@XDataObject@COleServerItem@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@@Z
extern "C" long MS_ABI impl__GetDataHere_XDataObject_COleServerItem__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM___Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetExtent@XOleObject@COleServerItem@@UEAAJKPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetExtent_XOleObject_COleServerItem__UEAAJKPEAUtagSIZE___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// Symbol: ?GetMiscStatus@XOleObject@COleServerItem@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetMiscStatus_XOleObject_COleServerItem__UEAAJKPEAK_Z(unsigned long p0, unsigned long* p1) {
    return 0;
}

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
