#include <cstring>

#include "openmfc/afxole.h"

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

IMPLEMENT_DYNAMIC(CRichEditDoc, COleDocument)
IMPLEMENT_DYNAMIC(CRichEditCntrItem, COleClientItem)

CRichEditDoc::CRichEditDoc() : COleDocument() {
    memset(_richeditdoc_padding, 0, sizeof(_richeditdoc_padding));
}

CRichEditDoc::~CRichEditDoc() {
}

COleClientItem* CRichEditDoc::GetInPlaceActiveItem(CWnd* pWnd) {
    (void)pWnd;
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    while (COleClientItem* item = GetNextClientItem(pos)) {
        if (item && item->IsInPlaceActive()) {
            return item;
        }
    }
    return nullptr;
}

COleClientItem* CRichEditDoc::GetPrimarySelectedItem(CView* pView) {
    (void)pView;
    COleClientItem* active = GetInPlaceActiveItem(nullptr);
    if (active) {
        return active;
    }

    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    return GetNextClientItem(pos);
}

CRichEditCntrItem* CRichEditDoc::LookupItem(IOleObject* pOleObject) const {
    POSITION pos = reinterpret_cast<POSITION>(GetStartPosition());
    while (COleClientItem* item = GetNextClientItem(pos)) {
        if (item && item->GetObject() == pOleObject) {
            return static_cast<CRichEditCntrItem*>(item);
        }
    }
    return nullptr;
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

// Symbol: ?GetInPlaceActiveItem@CRichEditDoc@@UEAAPEAVCOleClientItem@@PEAVCWnd@@@Z
extern "C" void* MS_ABI impl__GetInPlaceActiveItem_CRichEditDoc__UEAAPEAVCOleClientItem__PEAVCWnd___Z(CRichEditDoc* pThis, CWnd* pWnd) {
    return pThis ? pThis->GetInPlaceActiveItem(pWnd) : nullptr;
}

// Symbol: ?GetPrimarySelectedItem@CRichEditDoc@@UEAAPEAVCOleClientItem@@PEAVCView@@@Z
extern "C" void* MS_ABI impl__GetPrimarySelectedItem_CRichEditDoc__UEAAPEAVCOleClientItem__PEAVCView___Z(CRichEditDoc* pThis, CView* pView) {
    return pThis ? pThis->GetPrimarySelectedItem(pView) : nullptr;
}

// Symbol: ?LookupItem@CRichEditDoc@@QEBAPEAVCRichEditCntrItem@@PEAUIOleObject@@@Z
extern "C" void* MS_ABI impl__LookupItem_CRichEditDoc__QEBAPEAVCRichEditCntrItem__PEAUIOleObject___Z(CRichEditDoc* pThis, IOleObject* pOleObject) {
    return pThis ? pThis->LookupItem(pOleObject) : nullptr;
}

// Symbol: ?OnGetEmbeddedItem@CRichEditDoc@@MEAAPEAVCOleServerItem@@XZ
extern "C" COleServerItem* MS_ABI impl__OnGetEmbeddedItem_CRichEditDoc__MEAAPEAVCOleServerItem__XZ(CRichEditDoc* pThis) {
    COleClientItem* item = pThis ? pThis->GetPrimarySelectedItem(nullptr) : nullptr;
    return dynamic_cast<COleServerItem*>(item);
}
