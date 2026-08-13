// CHtmlControlSite — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??0CHtmlControlSite@@QEAA@PEAVCOleControlContainer@@@Z
extern "C" void* MS_ABI impl___0CHtmlControlSite__QEAA_PEAVCOleControlContainer___Z(void* pThis, void* pContainer) {
    if (pThis == nullptr) {
        return pThis;
    }
    return new (pThis) COleControlSite(static_cast<COleControlContainer*>(pContainer));
}
// Symbol: ??1CHtmlControlSite@@UEAA@XZ
extern "C" void MS_ABI impl___1CHtmlControlSite__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) {
        return;
    }
    static_cast<COleControlSite*>(pThis)->~COleControlSite();
}

// Symbol: ?EnableModeless@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__EnableModeless_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?FilterDataObject@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUIDataObject@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__FilterDataObject_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUIDataObject__PEAPEAU3__Z(void* /*struct*/* p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?GetDropTarget@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUIDropTarget@@PEAPEAU3@@Z
extern "C" long MS_ABI impl__GetDropTarget_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUIDropTarget__PEAPEAU3__Z(void* /*struct*/* p0, void* /*struct*/** p1) {
    return 0;
}

// Symbol: ?GetExternal@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExternal_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAPEAUIDispatch___Z(void* /*struct*/** p0) {
    return 0;
}

// Symbol: ?GetHostInfo@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAU_DOCHOSTUIINFO@@@Z
extern "C" long MS_ABI impl__GetHostInfo_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAU_DOCHOSTUIINFO___Z(void* /*struct*/* p0) {
    return 0;
}

// Symbol: ?GetOptionKeyPath@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAPEA_WK@Z
extern "C" long MS_ABI impl__GetOptionKeyPath_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAPEA_WK_Z(wchar_t** p0, unsigned long p1) {
    return 0;
}

// Symbol: ?HideUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__HideUI_XDocHostUIHandler_CHtmlControlSite__UEAAJXZ() {
    return 0;
}

// Symbol: ?OnDocWindowActivate@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnDocWindowActivate_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?OnFrameWindowActivate@XDocHostUIHandler@CHtmlControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFrameWindowActivate_XDocHostUIHandler_CHtmlControlSite__UEAAJH_Z(int p0) {
    return 0;
}

// Symbol: ?ResizeBorder@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" long MS_ABI impl__ResizeBorder_XDocHostUIHandler_CHtmlControlSite__UEAAJPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(const void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}

// Symbol: ?ShowContextMenu@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEAUtagPOINT@@PEAUIUnknown@@PEAUIDispatch@@@Z
extern "C" long MS_ABI impl__ShowContextMenu_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEAUtagPOINT__PEAUIUnknown__PEAUIDispatch___Z(unsigned long p0, void* /*struct*/* p1, void* /*struct*/* p2, void* /*struct*/* p3) {
    return 0;
}

// Symbol: ?ShowUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEAUIOleInPlaceActiveObject@@PEAUIOleCommandTarget@@PEAUIOleInPlaceFrame@@PEAUIOleInPlaceUIWindow@@@Z
extern "C" long MS_ABI impl__ShowUI_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEAUIOleInPlaceActiveObject__PEAUIOleCommandTarget__PEAUIOleInPlaceFrame__PEAUIOleInPlaceUIWindow___Z(unsigned long p0, void* /*struct*/* p1, void* /*struct*/* p2, void* /*struct*/* p3, void* /*struct*/* p4) {
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XDocHostUIHandler@CHtmlControlSite@@UEAAJPEAUtagMSG@@PEBU_GUID@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XDocHostUIHandler_CHtmlControlSite__UEAAJPEAUtagMSG__PEBU_GUID__K_Z(void* /*struct*/* p0, const void* /*struct*/* p1, unsigned long p2) {
    return 0;
}

// Symbol: ?TranslateUrl@XDocHostUIHandler@CHtmlControlSite@@UEAAJKPEA_WPEAPEA_W@Z
extern "C" long MS_ABI impl__TranslateUrl_XDocHostUIHandler_CHtmlControlSite__UEAAJKPEA_WPEAPEA_W_Z(unsigned long p0, wchar_t* p1, wchar_t** p2) {
    return 0;
}

// Symbol: ?UpdateUI@XDocHostUIHandler@CHtmlControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__UpdateUI_XDocHostUIHandler_CHtmlControlSite__UEAAJXZ() {
    return 0;
}
