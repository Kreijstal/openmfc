// CRichEditCntrItem — OpenMFC implementation.
// Sources: ole_olectors_exports.cpp, richedit_ole_ext.cpp

#include "detail/CRichEditDocSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ??0CRichEditCntrItem@@QEAA@PEAU_reobject@@PEAVCRichEditDoc@@@Z
// CRichEditCntrItem::CRichEditCntrItem(REOBJECT* pObject, CRichEditDoc* pContainerDoc)
// NOTE: the OpenMFC constructor currently takes only the container document.
extern "C" void* MS_ABI impl___0CRichEditCntrItem__QEAA_PEAU_reobject__PEAVCRichEditDoc___Z(void* pObject, CRichEditDoc* pContainerDoc) {
    (void)pObject;
    return new CRichEditCntrItem(pContainerDoc);
}
// Symbol: ?GetClientSite@CRichEditCntrItem@@UEAAPEAUIOleClientSite@@XZ
extern "C" void* MS_ABI impl__GetClientSite_CRichEditCntrItem__UEAAPEAUIOleClientSite__XZ(CRichEditCntrItem* pThis) {
    return pThis ? pThis->GetClientSite() : nullptr;
}
// Symbol: ?GetWindowContext@CRichEditCntrItem@@QEAAJPEAPEAUIOleInPlaceFrame@@PEAPEAUIOleInPlaceUIWindow@@PEAUtagOIFI@@@Z
extern "C" long MS_ABI impl__GetWindowContext_CRichEditCntrItem__QEAAJPEAPEAUIOleInPlaceFrame__PEAPEAUIOleInPlaceUIWindow__PEAUtagOIFI___Z(
    CRichEditCntrItem* pThis,
    IOleInPlaceFrame** ppFrame,
    IOleInPlaceUIWindow** ppDoc,
    LPOLEINPLACEFRAMEINFO lpFrameInfo) {
    return pThis ? pThis->GetWindowContext(ppFrame, ppDoc, lpFrameInfo) : -1;
}
CRichEditCntrItem::CRichEditCntrItem(COleDocument* pContainerDoc)
    : COleClientItem(pContainerDoc) {
    memset(_richeditcntritem_padding, 0, sizeof(_richeditcntritem_padding));
}
CRichEditCntrItem::~CRichEditCntrItem() {
}
IOleClientSite* CRichEditCntrItem::GetClientSite() {
    if (!m_lpObject) {
        return nullptr;
    }

    IOleClientSite* pSite = nullptr;
    if (FAILED(m_lpObject->GetClientSite(&pSite))) {
        return nullptr;
    }

    return pSite;
}
long CRichEditCntrItem::GetWindowContext(IOleInPlaceFrame** ppFrame,
                                        IOleInPlaceUIWindow** ppDoc,
                                        LPOLEINPLACEFRAMEINFO lpFrameInfo) {
    if (lpFrameInfo) {
        memset(lpFrameInfo, 0, sizeof(*lpFrameInfo));
        lpFrameInfo->cb = sizeof(*lpFrameInfo);
    }

    if (ppFrame) {
        *ppFrame = nullptr;
        if (m_lpFrame) {
            *ppFrame = m_lpFrame;
            m_lpFrame->AddRef();
        }
    }

    if (ppDoc) {
        *ppDoc = nullptr;
    }

    if (lpFrameInfo && m_pContainerDoc) {
        POSITION pos = m_pContainerDoc->GetFirstViewPosition();
        if (pos) {
            CWnd* pView = static_cast<CWnd*>(m_pContainerDoc->GetNextView(pos));
            if (pView) {
                lpFrameInfo->hwndFrame = pView->GetSafeHwnd();
            }
        }
    }
    return S_OK;
}
// Symbol: ??1CRichEditCntrItem@@UEAA@XZ
extern "C" void* MS_ABI impl___1CRichEditCntrItem__UEAA_XZ() {
    return nullptr;
}

// Symbol: ?ActivateAs@CRichEditCntrItem@@UEAAHPEB_WAEBU_GUID@@1@Z
extern "C" int MS_ABI impl__ActivateAs_CRichEditCntrItem__UEAAHPEB_WAEBU_GUID__1_Z(const wchar_t* p0, const void* /*struct*/* p1, const void* /*struct*/* p2) {
    return 0;
}

// Symbol: ?CanActivate@CRichEditCntrItem@@UEAAHXZ
extern "C" int MS_ABI impl__CanActivate_CRichEditCntrItem__UEAAHXZ() {
    return 0;
}

// Symbol: ?ConvertTo@CRichEditCntrItem@@UEAAHAEBU_GUID@@@Z
extern "C" int MS_ABI impl__ConvertTo_CRichEditCntrItem__UEAAHAEBU_GUID___Z(const void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?CreateObject@CRichEditCntrItem@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CRichEditCntrItem__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?OnChangeItemPosition@CRichEditCntrItem@@MEAAHAEBVCRect@@@Z
extern "C" int MS_ABI impl__OnChangeItemPosition_CRichEditCntrItem__MEAAHAEBVCRect___Z(const void* /*class*/* p0) {
    return 0;
}

// Symbol: ?OnDeactivateUI@CRichEditCntrItem@@UEAAXH@Z
extern "C" void MS_ABI impl__OnDeactivateUI_CRichEditCntrItem__UEAAXH_Z(int p0) {}

// Symbol: ?SetDrawAspect@CRichEditCntrItem@@UEAAXW4tagDVASPECT@@@Z
extern "C" void MS_ABI impl__SetDrawAspect_CRichEditCntrItem__UEAAXW4tagDVASPECT___Z(int /*enum*/ p0, void* p1, void* p2, void* p3, char p4, void* /*class*/ p5) {}

// Symbol: ?ShowContainerUI@CRichEditCntrItem@@QEAAJH@Z
extern "C" long MS_ABI impl__ShowContainerUI_CRichEditCntrItem__QEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?SyncToRichEditObject@CRichEditCntrItem@@QEAAXAEAU_reobject@@@Z
extern "C" void MS_ABI impl__SyncToRichEditObject_CRichEditCntrItem__QEAAXAEAU_reobject___Z(void* /*struct*/* p0) {}

