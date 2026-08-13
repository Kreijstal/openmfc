// COleUILinkInfo — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0COleUILinkInfo@@QEAA@PEAVCOleClientItem@@@Z
extern "C" void* MS_ABI impl___0COleUILinkInfo__QEAA_PEAVCOleClientItem___Z(void* pThis, void* pItem) {
    SetCOleUILinkInfoTarget(pThis, nullptr, static_cast<COleClientItem*>(pItem));
    return pThis;
}
// Symbol: ??0COleUILinkInfo@@QEAA@PEAVCOleDocument@@@Z
extern "C" void* MS_ABI impl___0COleUILinkInfo__QEAA_PEAVCOleDocument___Z(void* pThis, void* pDocument) {
    SetCOleUILinkInfoTarget(pThis, static_cast<COleDocument*>(pDocument), nullptr);
    return pThis;
}
// Symbol: ?UpdateLink@COleUILinkInfo@@UEAAJKHH@Z
extern "C" unsigned long MS_ABI impl__UpdateLink_COleUILinkInfo__UEAAJKHH_Z(
    void* pThis, unsigned long dwLink, unsigned short fErrorMessage, unsigned short fReserved) {
    (void)fErrorMessage;
    (void)fReserved;

    if (pThis == nullptr) return E_POINTER;

    COleDocument* pDocument = nullptr;
    COleClientItem* pItem = nullptr;
    if (!GetCOleUILinkInfoTarget(pThis, pDocument, pItem)) return E_INVALIDARG;

    if (pItem != nullptr) {
        if (dwLink != 0 && dwLink != 1) return E_INVALIDARG;
        return UpdateSingleOleLinkItem(pItem);
    }

    if (pDocument == nullptr) return E_INVALIDARG;

    if (dwLink == 0) {
        POSITION pos = pDocument->GetStartPosition();
        if (pos == nullptr) return S_FALSE;

        HRESULT hr = S_OK;
        bool didUpdate = false;
        while (pos != nullptr) {
            COleClientItem* updateItem = pDocument->GetNextClientItem(pos);
            if (updateItem == nullptr) continue;
            didUpdate = true;
            const HRESULT linkResult = UpdateSingleOleLinkItem(updateItem);
            if (FAILED(linkResult) && SUCCEEDED(hr)) hr = linkResult;
        }
        return didUpdate ? hr : S_FALSE;
    }

    COleClientItem* targetItem = FindItemByLinkIndex(pDocument, dwLink);
    if (targetItem == nullptr) return E_INVALIDARG;
    return UpdateSingleOleLinkItem(targetItem);
}
// ?AddRef@COleUILinkInfo@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_COleUILinkInfo__UEAAKXZ() {
    return 0;
}

// ?CancelLink@COleUILinkInfo@@UEAAJK@Z
extern "C" long MS_ABI impl__CancelLink_COleUILinkInfo__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// ?GetLastUpdate@COleUILinkInfo@@UEAAJKPEAU_FILETIME@@@Z
extern "C" long MS_ABI impl__GetLastUpdate_COleUILinkInfo__UEAAJKPEAU_FILETIME___Z(unsigned long p0, void* /*struct*/* p1) {
    return 0;
}

// ?GetLinkItem@COleUILinkInfo@@AEAAPEAVCOleClientItem@@K@Z
extern "C" void* MS_ABI impl__GetLinkItem_COleUILinkInfo__AEAAPEAVCOleClientItem__K_Z(unsigned long p0) {
    return nullptr;
}

// ?GetLinkSource@COleUILinkInfo@@UEAAJKPEAPEA_WPEAK00PEAH2@Z
extern "C" long MS_ABI impl__GetLinkSource_COleUILinkInfo__UEAAJKPEAPEA_WPEAK00PEAH2_Z(unsigned long p0, wchar_t** p1, unsigned long* p2, unsigned long p3, unsigned long p4, int* p5, unsigned long* p6) {
    return 0;
}

// ?GetLinkUpdateOptions@COleUILinkInfo@@UEAAJKPEAK@Z
extern "C" long MS_ABI impl__GetLinkUpdateOptions_COleUILinkInfo__UEAAJKPEAK_Z(unsigned long p0, unsigned long* p1) {
    return 0;
}

// ?GetNextLink@COleUILinkInfo@@UEAAKK@Z
extern "C" unsigned long MS_ABI impl__GetNextLink_COleUILinkInfo__UEAAKK_Z(unsigned long p0) {
    return 0;
}

// ?OpenLinkSource@COleUILinkInfo@@UEAAJK@Z
extern "C" long MS_ABI impl__OpenLinkSource_COleUILinkInfo__UEAAJK_Z(unsigned long p0) {
    return 0;
}

// ?QueryInterface@COleUILinkInfo@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_COleUILinkInfo__UEAAJAEBU_GUID__PEAPEAX_Z(const void* /*struct*/* p0, void** p1) {
    return 0;
}

// ?Release@COleUILinkInfo@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_COleUILinkInfo__UEAAKXZ() {
    return 0;
}

// ?SetLinkSource@COleUILinkInfo@@UEAAJKPEA_WKPEAKH@Z
extern "C" long MS_ABI impl__SetLinkSource_COleUILinkInfo__UEAAJKPEA_WKPEAKH_Z(unsigned long p0, wchar_t* p1, unsigned long p2, unsigned long* p3, int p4) {
    return 0;
}

// ?SetLinkUpdateOptions@COleUILinkInfo@@UEAAJKK@Z
extern "C" long MS_ABI impl__SetLinkUpdateOptions_COleUILinkInfo__UEAAJKK_Z(unsigned long p0, unsigned long p1) {
    return 0;
}

