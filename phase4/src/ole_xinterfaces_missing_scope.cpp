// Missing COle-related X-interface stubs (requested 10-class scope).

#include "openmfc/afxole.h"

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
// XMessageFilter
// ---------------------------------------------------------------------------

// Symbol: ?HandleInComingCall@XMessageFilter@COleMessageFilter@@UEAAKKPEAUHTASK__@@KPEAUtagINTERFACEINFO@@@Z
extern "C" unsigned long MS_ABI impl__HandleInComingCall_XMessageFilter_COleMessageFilter__UEAAKKPEAUHTASK____KPEAUtagINTERFACEINFO___Z(
    void* pThis, unsigned long dwCallType, void* /*htaskCaller*/, unsigned long /*dwTickCount*/, void* /*pInterfaceInfo*/) {
    (void)pThis;
    (void)dwCallType;
    return 0;
}

// Symbol: ?MessagePending@XMessageFilter@COleMessageFilter@@UEAAKPEAUHTASK__@@KK@Z
extern "C" unsigned long MS_ABI impl__MessagePending_XMessageFilter_COleMessageFilter__UEAAKPEAUHTASK____KK_Z(
    void* pThis, void* /*htaskCallee*/, unsigned long /*dwTickCount*/, unsigned long /*dwPendingType*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?RetryRejectedCall@XMessageFilter@COleMessageFilter@@UEAAKPEAUHTASK__@@KK@Z
extern "C" unsigned long MS_ABI impl__RetryRejectedCall_XMessageFilter_COleMessageFilter__UEAAKPEAUHTASK____KK_Z(
    void* pThis, void* /*htaskCallee*/, unsigned long /*dwRejectType*/, unsigned long /*dwTickCount*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XInitializeWithStream
// ---------------------------------------------------------------------------

// Symbol: ?Initialize@XInitializeWithStream@CDocument@@UEAAJPEAUIStream@@K@Z
extern "C" long MS_ABI impl__Initialize_XInitializeWithStream_CDocument__UEAAJPEAUIStream__K_Z(
    void* pThis, void* /*pStm*/, unsigned long /*dwMode*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XNotifyDBEvents
// ---------------------------------------------------------------------------

// Symbol: ?AboutToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__AboutToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// Symbol: ?Cancelled@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__Cancelled_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// Symbol: ?DidEvent@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__DidEvent_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// Symbol: ?FailedToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__FailedToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// Symbol: ?FireEvent@XNotifyDBEvents@COleControlSite@@QEAAJKKQEAUtagDBNOTIFYREASON@@W4DSCSTATE@@@Z
extern "C" long MS_ABI impl__FireEvent_XNotifyDBEvents_COleControlSite__QEAAJKKQEAUtagDBNOTIFYREASON__W4DSCSTATE___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason, int dscState) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    (void)dscState;
    return 0;
}

// Symbol: ?OKToDo@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__OKToDo_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// Symbol: ?SyncAfter@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__SyncAfter_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// Symbol: ?SyncBefore@XNotifyDBEvents@COleControlSite@@UEAAJKKQEAUtagDBNOTIFYREASON@@@Z
extern "C" long MS_ABI impl__SyncBefore_XNotifyDBEvents_COleControlSite__UEAAJKKQEAUtagDBNOTIFYREASON___Z(
    void* pThis, unsigned long dwCookie, unsigned long dbcs, void* pReason) {
    (void)pThis;
    (void)dwCookie;
    (void)dbcs;
    (void)pReason;
    return 0;
}

// ---------------------------------------------------------------------------
// XOleCache
// ---------------------------------------------------------------------------

// Symbol: ?Cache@XOleCache@COleControl@@UEAAJPEAUtagFORMATETC@@KPEAK@Z
extern "C" long MS_ABI impl__Cache_XOleCache_COleControl__UEAAJPEAUtagFORMATETC__KPEAK_Z(
    void* pThis, void* /*pFormatetc*/, unsigned long /*advf*/, unsigned long* pAdvfOut) {
    (void)pThis;
    if (pAdvfOut) {
        *pAdvfOut = 0;
    }
    return 0;
}

// Symbol: ?EnumCache@XOleCache@COleControl@@UEAAJPEAPEAUIEnumSTATDATA@@@Z
extern "C" long MS_ABI impl__EnumCache_XOleCache_COleControl__UEAAJPEAPEAUIEnumSTATDATA___Z(
    void* pThis, void** ppEnum) {
    (void)pThis;
    SetOutputNull(ppEnum);
    return 0;
}

// Symbol: ?InitCache@XOleCache@COleControl@@UEAAJPEAUIDataObject@@@Z
extern "C" long MS_ABI impl__InitCache_XOleCache_COleControl__UEAAJPEAUIDataObject___Z(
    void* pThis, void* /*pDataObject*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?SetData@XOleCache@COleControl@@UEAAJPEAUtagFORMATETC@@PEAUtagSTGMEDIUM@@H@Z
extern "C" long MS_ABI impl__SetData_XOleCache_COleControl__UEAAJPEAUtagFORMATETC__PEAUtagSTGMEDIUM__H_Z(
    void* pThis, void* /*pFormatetc*/, void* /*pmedium*/, int /*fRelease*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Uncache@XOleCache@COleControl@@UEAAJK@Z
extern "C" long MS_ABI impl__Uncache_XOleCache_COleControl__UEAAJK_Z(
    void* pThis, unsigned long /*dwFlags*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XOleClientSite
// ---------------------------------------------------------------------------

// Symbol: ?GetContainer@XOleClientSite@COleClientItem@@UEAAJPEAPEAUIOleContainer@@@Z
extern "C" long MS_ABI impl__GetContainer_XOleClientSite_COleClientItem__UEAAJPEAPEAUIOleContainer___Z(
    void* pThis, void** ppContainer) {
    (void)pThis;
    SetOutputNull(ppContainer);
    return 0;
}

// Symbol: ?GetContainer@XOleClientSite@COleControlSite@@UEAAJPEAPEAUIOleContainer@@@Z
extern "C" long MS_ABI impl__GetContainer_XOleClientSite_COleControlSite__UEAAJPEAPEAUIOleContainer___Z(
    void* pThis, void** ppContainer) {
    (void)pThis;
    SetOutputNull(ppContainer);
    return 0;
}

// Symbol: ?GetMoniker@XOleClientSite@COleClientItem@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleClientSite_COleClientItem__UEAAJKKPEAPEAUIMoniker___Z(
    void* pThis, unsigned long dwAssign, unsigned long dwWhichMoniker, void** ppmk) {
    (void)pThis;
    (void)dwAssign;
    (void)dwWhichMoniker;
    SetOutputNull(ppmk);
    return 0;
}

// Symbol: ?GetMoniker@XOleClientSite@COleControlSite@@UEAAJKKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__GetMoniker_XOleClientSite_COleControlSite__UEAAJKKPEAPEAUIMoniker___Z(
    void* pThis, unsigned long dwAssign, unsigned long dwWhichMoniker, void** ppmk) {
    (void)pThis;
    (void)dwAssign;
    (void)dwWhichMoniker;
    SetOutputNull(ppmk);
    return 0;
}

// Symbol: ?OnShowWindow@XOleClientSite@COleClientItem@@UEAAJH@Z
extern "C" long MS_ABI impl__OnShowWindow_XOleClientSite_COleClientItem__UEAAJH_Z(
    void* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return 0;
}

// Symbol: ?OnShowWindow@XOleClientSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnShowWindow_XOleClientSite_COleControlSite__UEAAJH_Z(
    void* pThis, int bShow) {
    (void)pThis;
    (void)bShow;
    return 0;
}

// Symbol: ?RequestNewObjectLayout@XOleClientSite@COleClientItem@@UEAAJXZ
extern "C" long MS_ABI impl__RequestNewObjectLayout_XOleClientSite_COleClientItem__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?RequestNewObjectLayout@XOleClientSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__RequestNewObjectLayout_XOleClientSite_COleControlSite__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?SaveObject@XOleClientSite@COleClientItem@@UEAAJXZ
extern "C" long MS_ABI impl__SaveObject_XOleClientSite_COleClientItem__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?SaveObject@XOleClientSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__SaveObject_XOleClientSite_COleControlSite__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?ShowObject@XOleClientSite@COleClientItem@@UEAAJXZ
extern "C" long MS_ABI impl__ShowObject_XOleClientSite_COleClientItem__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?ShowObject@XOleClientSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__ShowObject_XOleClientSite_COleControlSite__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XOleCommandTarget
// ---------------------------------------------------------------------------

// Symbol: ?Exec@XOleCommandTarget@CDocObjectServer@@UEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
extern "C" long MS_ABI impl__Exec_XOleCommandTarget_CDocObjectServer__UEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    void* pThis, const void* /*rguidCmdGroup*/, unsigned long /*nCmdID*/, unsigned long /*nCmdexecopt*/, void* /*pvaIn*/, void* /*pvaOut*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Exec@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KKPEAUtagVARIANT@@1@Z
extern "C" long MS_ABI impl__Exec_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KKPEAUtagVARIANT__1_Z(
    void* pThis, const void* /*rguidCmdGroup*/, unsigned long /*nCmdID*/, unsigned long /*nCmdexecopt*/, void* /*pvaIn*/, void* /*pvaOut*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?QueryStatus@XOleCommandTarget@CDocObjectServer@@UEAAJPEBU_GUID@@KQEAU_tagOLECMD@@PEAU_tagOLECMDTEXT@@@Z
extern "C" long MS_ABI impl__QueryStatus_XOleCommandTarget_CDocObjectServer__UEAAJPEBU_GUID__KQEAU_tagOLECMD__PEAU_tagOLECMDTEXT___Z(
    void* pThis, const void* /*rguidCmdGroup*/, unsigned long /*cCmds*/, const void* /*prgCmds*/, void* /*pCmdText*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?QueryStatus@XOleCommandTarget@COleFrameHook@@UEAAJPEBU_GUID@@KQEAU_tagOLECMD@@PEAU_tagOLECMDTEXT@@@Z
extern "C" long MS_ABI impl__QueryStatus_XOleCommandTarget_COleFrameHook__UEAAJPEBU_GUID__KQEAU_tagOLECMD__PEAU_tagOLECMDTEXT___Z(
    void* pThis, const void* /*rguidCmdGroup*/, unsigned long /*cCmds*/, const void* /*prgCmds*/, void* /*pCmdText*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XOleContainer
// ---------------------------------------------------------------------------

// Symbol: ?EnumObjects@XOleContainer@COleControlContainer@@UEAAJKPEAPEAUIEnumUnknown@@@Z
extern "C" long MS_ABI impl__EnumObjects_XOleContainer_COleControlContainer__UEAAJKPEAPEAUIEnumUnknown___Z(
    void* pThis, unsigned long /*grfFlags*/, void** ppenumUnknown) {
    (void)pThis;
    SetOutputNull(ppenumUnknown);
    return 0;
}

// Symbol: ?LockContainer@XOleContainer@COleControlContainer@@UEAAJH@Z
extern "C" long MS_ABI impl__LockContainer_XOleContainer_COleControlContainer__UEAAJH_Z(
    void* pThis, int /*fLock*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ParseDisplayName@XOleContainer@COleControlContainer@@UEAAJPEAUIBindCtx@@PEA_WPEAKPEAPEAUIMoniker@@@Z
extern "C" long MS_ABI impl__ParseDisplayName_XOleContainer_COleControlContainer__UEAAJPEAUIBindCtx__PEA_WPEAKPEAPEAUIMoniker___Z(
    void* pThis, void* /*pbc*/, wchar_t* /*pszName*/, unsigned long* pchEaten, void** ppmk) {
    (void)pThis;
    if (pchEaten) {
        *pchEaten = 0;
    }
    SetOutputNull(ppmk);
    return 0;
}

// ---------------------------------------------------------------------------
// XOleControl
// ---------------------------------------------------------------------------

// Symbol: ?FreezeEvents@XOleControl@COleControl@@UEAAJH@Z
extern "C" long MS_ABI impl__FreezeEvents_XOleControl_COleControl__UEAAJH_Z(
    void* pThis, int /*bFreeze*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetControlInfo@XOleControl@COleControl@@UEAAJPEAUtagCONTROLINFO@@@Z
extern "C" long MS_ABI impl__GetControlInfo_XOleControl_COleControl__UEAAJPEAUtagCONTROLINFO___Z(
    void* pThis, void* /*pCI*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnAmbientPropertyChange@XOleControl@COleControl@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnAmbientPropertyChange_XOleControl_COleControl__UEAAJJ_Z(
    void* pThis, long /*dispID*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnMnemonic@XOleControl@COleControl@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__OnMnemonic_XOleControl_COleControl__UEAAJPEAUtagMSG___Z(
    void* pThis, void* /*lpMsg*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XOleControlSite
// ---------------------------------------------------------------------------

// Symbol: ?GetExtendedControl@XOleControlSite@COleControlSite@@UEAAJPEAPEAUIDispatch@@@Z
extern "C" long MS_ABI impl__GetExtendedControl_XOleControlSite_COleControlSite__UEAAJPEAPEAUIDispatch___Z(
    void* pThis, void** ppDispatch) {
    (void)pThis;
    SetOutputNull(ppDispatch);
    return 0;
}

// Symbol: ?LockInPlaceActive@XOleControlSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__LockInPlaceActive_XOleControlSite_COleControlSite__UEAAJH_Z(
    void* pThis, int /*fLock*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnControlInfoChanged@XOleControlSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__OnControlInfoChanged_XOleControlSite_COleControlSite__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnFocus@XOleControlSite@COleControlSite@@UEAAJH@Z
extern "C" long MS_ABI impl__OnFocus_XOleControlSite_COleControlSite__UEAAJH_Z(
    void* pThis, int /*bFocus*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ShowPropertyFrame@XOleControlSite@COleControlSite@@UEAAJXZ
extern "C" long MS_ABI impl__ShowPropertyFrame_XOleControlSite_COleControlSite__UEAAJXZ(
    void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?TransformCoords@XOleControlSite@COleControlSite@@UEAAJPEAU_POINTL@@PEAUtagPOINTF@@K@Z
extern "C" long MS_ABI impl__TransformCoords_XOleControlSite_COleControlSite__UEAAJPEAU_POINTL__PEAUtagPOINTF__K_Z(
    void* pThis, void* /*pptlHimetric*/, void* /*pPoints*/, unsigned long /*dwFlags*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XOleControlSite@COleControlSite@@UEAAJPEAUtagMSG@@K@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XOleControlSite_COleControlSite__UEAAJPEAUtagMSG__K_Z(
    void* pThis, void* /*pMsg*/, unsigned long /*grfModifiers*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XOleDocument
// ---------------------------------------------------------------------------

// Symbol: ?CreateView@XOleDocument@CDocObjectServer@@UEAAJPEAUIOleInPlaceSite@@PEAUIStream@@KPEAPEAUIOleDocumentView@@@Z
extern "C" long MS_ABI impl__CreateView_XOleDocument_CDocObjectServer__UEAAJPEAUIOleInPlaceSite__PEAUIStream__KPEAPEAUIOleDocumentView___Z(
    void* pThis, void* /*pIPSite*/, void* /*pStream*/, unsigned long /*dwReserved*/, void** ppView) {
    (void)pThis;
    SetOutputNull(ppView);
    return 0;
}

// Symbol: ?EnumViews@XOleDocument@CDocObjectServer@@UEAAJPEAPEAUIEnumOleDocumentViews@@PEAPEAUIOleDocumentView@@@Z
extern "C" long MS_ABI impl__EnumViews_XOleDocument_CDocObjectServer__UEAAJPEAPEAUIEnumOleDocumentViews__PEAPEAUIOleDocumentView___Z(
    void* pThis, void** ppenumViews, void** ppView) {
    (void)pThis;
    SetOutputNull(ppenumViews);
    SetOutputNull(ppView);
    return 0;
}

// Symbol: ?GetDocMiscStatus@XOleDocument@CDocObjectServer@@UEAAJPEAK@Z
extern "C" long MS_ABI impl__GetDocMiscStatus_XOleDocument_CDocObjectServer__UEAAJPEAK_Z(
    void* pThis, unsigned long* pdwMiscStatus) {
    (void)pThis;
    if (pdwMiscStatus) {
        *pdwMiscStatus = 0;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// XObjectWithSite
// ---------------------------------------------------------------------------

// Symbol: ?GetSite@XObjectWithSite@CDocument@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetSite_XObjectWithSite_CDocument__UEAAJAEBU_GUID__PEAPEAX_Z(
    void* pThis, const void* /*riid*/, void** ppvSite) {
    (void)pThis;
    SetOutputNull(ppvSite);
    return 0;
}

// Symbol: ?SetSite@XObjectWithSite@CDocument@@UEAAJPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetSite_XObjectWithSite_CDocument__UEAAJPEAUIUnknown___Z(
    void* pThis, void* /*pUnknown*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XPreviewHandlerVisuals
// ---------------------------------------------------------------------------

// Symbol: ?SetBackgroundColor@XPreviewHandlerVisuals@CDocument@@UEAAJK@Z
extern "C" long MS_ABI impl__SetBackgroundColor_XPreviewHandlerVisuals_CDocument__UEAAJK_Z(
    void* pThis, unsigned long /*dwColor*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?SetFont@XPreviewHandlerVisuals@CDocument@@UEAAJPEBUtagLOGFONTW@@@Z
extern "C" long MS_ABI impl__SetFont_XPreviewHandlerVisuals_CDocument__UEAAJPEBUtagLOGFONTW___Z(
    void* pThis, const void* /*pLogFont*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?SetTextColor@XPreviewHandlerVisuals@CDocument@@UEAAJK@Z
extern "C" long MS_ABI impl__SetTextColor_XPreviewHandlerVisuals_CDocument__UEAAJK_Z(
    void* pThis, unsigned long /*clrText*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XPrint
// ---------------------------------------------------------------------------

// Symbol: ?GetPageInfo@XPrint@CDocObjectServer@@UEAAJPEAJ0@Z
extern "C" long MS_ABI impl__GetPageInfo_XPrint_CDocObjectServer__UEAAJPEAJ0_Z(
    void* pThis, long* pFirstPage, long* pLastPage) {
    (void)pThis;
    if (pFirstPage) {
        *pFirstPage = 0;
    }
    if (pLastPage) {
        *pLastPage = 0;
    }
    return 0;
}

// Symbol: ?Print@XPrint@CDocObjectServer@@UEAAJKPEAPEAUtagDVTARGETDEVICE@@PEAPEAUtagPAGESET@@PEAUtagSTGMEDIUM@@PEAUIContinueCallback@@JPEAJ4@Z
extern "C" long MS_ABI impl__Print_XPrint_CDocObjectServer__UEAAJKPEAPEAUtagDVTARGETDEVICE__PEAPEAUtagPAGESET__PEAUtagSTGMEDIUM__PEAUIContinueCallback__JPEAJ4_Z(
    void* pThis, void** /*ppDevMode*/, void** /*ppPageSet*/, void* /*pPageInfo*/,
    void* /*pCallback*/, long /*nFlags*/, long* pFirstPage, long* pLastPage) {
    (void)pThis;
    if (pFirstPage) {
        *pFirstPage = 0;
    }
    if (pLastPage) {
        *pLastPage = 0;
    }
    return 0;
}

// Symbol: ?SetInitialPageNum@XPrint@CDocObjectServer@@UEAAJJ@Z
extern "C" long MS_ABI impl__SetInitialPageNum_XPrint_CDocObjectServer__UEAAJJ_Z(
    void* pThis, long /*nPage*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XPropertyNotifySink / XPropNotifySink
// ---------------------------------------------------------------------------

// Symbol: ?OnChanged@XPropertyNotifySink@COleControlSite@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(
    void* pThis, long /*dispid*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnChanged@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnChanged_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(
    void* pThis, long /*dispid*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnRequestEdit@XPropertyNotifySink@COleControlSite@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XPropertyNotifySink_COleControlSite__UEAAJJ_Z(
    void* pThis, long /*dispid*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?OnRequestEdit@XPropNotifySink@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__OnRequestEdit_XPropNotifySink_COlePropertyPage__UEAAJJ_Z(
    void* pThis, long /*dispid*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XPropertyPage
// ---------------------------------------------------------------------------

// Symbol: ?Activate@XPropertyPage@COlePropertyPage@@UEAAJPEAUHWND__@@PEBUtagRECT@@H@Z
extern "C" long MS_ABI impl__Activate_XPropertyPage_COlePropertyPage__UEAAJPEAUHWND____PEBUtagRECT__H_Z(
    void* pThis, void* /*hwnd*/, const void* /*lpRect*/, int /*nCmd*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Apply@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Apply_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?Deactivate@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__Deactivate_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?EditProperty@XPropertyPage@COlePropertyPage@@UEAAJJ@Z
extern "C" long MS_ABI impl__EditProperty_XPropertyPage_COlePropertyPage__UEAAJJ_Z(
    void* pThis, long /*dispid*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?GetPageInfo@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagPROPPAGEINFO@@@Z
extern "C" long MS_ABI impl__GetPageInfo_XPropertyPage_COlePropertyPage__UEAAJPEAUtagPROPPAGEINFO___Z(
    void* pThis, void* /*pPageInfo*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Help@XPropertyPage@COlePropertyPage@@UEAAJPEB_W@Z
extern "C" long MS_ABI impl__Help_XPropertyPage_COlePropertyPage__UEAAJPEB_W_Z(
    void* pThis, const wchar_t* /*pszHelp*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?IsPageDirty@XPropertyPage@COlePropertyPage@@UEAAJXZ
extern "C" long MS_ABI impl__IsPageDirty_XPropertyPage_COlePropertyPage__UEAAJXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Symbol: ?Move@XPropertyPage@COlePropertyPage@@UEAAJPEBUtagRECT@@@Z
extern "C" long MS_ABI impl__Move_XPropertyPage_COlePropertyPage__UEAAJPEBUtagRECT___Z(
    void* pThis, const void* /*lpRect*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?SetObjects@XPropertyPage@COlePropertyPage@@UEAAJKPEAPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetObjects_XPropertyPage_COlePropertyPage__UEAAJKPEAPEAUIUnknown___Z(
    void* pThis, unsigned long /*nObjects*/, void** /*ppUnk*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?SetPageSite@XPropertyPage@COlePropertyPage@@UEAAJPEAUIPropertyPageSite@@@Z
extern "C" long MS_ABI impl__SetPageSite_XPropertyPage_COlePropertyPage__UEAAJPEAUIPropertyPageSite___Z(
    void* pThis, void* /*pPageSite*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Show@XPropertyPage@COlePropertyPage@@UEAAJI@Z
extern "C" long MS_ABI impl__Show_XPropertyPage_COlePropertyPage__UEAAJI_Z(
    void* pThis, unsigned int /*nCmdShow*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?TranslateAcceleratorW@XPropertyPage@COlePropertyPage@@UEAAJPEAUtagMSG@@@Z
extern "C" long MS_ABI impl__TranslateAcceleratorW_XPropertyPage_COlePropertyPage__UEAAJPEAUtagMSG___Z(
    void* pThis, void* /*pMsg*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XProvideClassInfo
// ---------------------------------------------------------------------------

// Symbol: ?GetClassInfoW@XProvideClassInfo@COleControl@@UEAAJPEAPEAUITypeInfo@@@Z
extern "C" long MS_ABI impl__GetClassInfoW_XProvideClassInfo_COleControl__UEAAJPEAPEAUITypeInfo___Z(
    void* pThis, void** ppTypeInfo) {
    (void)pThis;
    SetOutputNull(ppTypeInfo);
    return 0;
}

// Symbol: ?GetGUID@XProvideClassInfo@COleControl@@UEAAJKPEAU_GUID@@@Z
extern "C" long MS_ABI impl__GetGUID_XProvideClassInfo_COleControl__UEAAJKPEAU_GUID___Z(
    void* pThis, unsigned long /*lRef*/, void* /*pGuid*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XQuickActivate
// ---------------------------------------------------------------------------

// Symbol: ?GetContentExtent@XQuickActivate@COleControl@@UEAAJPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__GetContentExtent_XQuickActivate_COleControl__UEAAJPEAUtagSIZE___Z(
    void* pThis, void* /*pSize*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?QuickActivate@XQuickActivate@COleControl@@UEAAJPEAUtagQACONTAINER@@PEAUtagQACONTROL@@@Z
extern "C" long MS_ABI impl__QuickActivate_XQuickActivate_COleControl__UEAAJPEAUtagQACONTAINER__PEAUtagQACONTROL___Z(
    void* pThis, void* pContainer, void* pControl) {
    (void)pThis;
    (void)pContainer;
    (void)pControl;
    return 0;
}

// Symbol: ?SetContentExtent@XQuickActivate@COleControl@@UEAAJPEAUtagSIZE@@@Z
extern "C" long MS_ABI impl__SetContentExtent_XQuickActivate_COleControl__UEAAJPEAUtagSIZE___Z(
    void* pThis, void* /*pSize*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XQAT
// ---------------------------------------------------------------------------

// Symbol: ??0XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XQAT@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XQAT_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?AddItem@XQAT@CMFCRibbonInfo@@QEAA_JAEBVXQATItem@12@@Z
extern "C" long MS_ABI impl__AddItem_XQAT_CMFCRibbonInfo__QEAA_JAEBVXQATItem_12__Z(
    void* pThis, const void* /*item*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?Read@XQAT@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XQAT_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XQATItem
// ---------------------------------------------------------------------------

// Symbol: ??0XQATItem@XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XQATItem@XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?Read@XQATItem@XQAT@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@3@@Z
extern "C" int MS_ABI impl__Read_XQATItem_XQAT_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(
    void* pThis, void* /*item*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XRibbonBar
// ---------------------------------------------------------------------------

// Symbol: ??0XRibbonBar@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonBar_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XRibbonBar@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonBar_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?Read@XRibbonBar@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(
    void* pThis, void* /*parser*/) {
    (void)pThis;
    return 0;
}

// ---------------------------------------------------------------------------
// XRibbonInfoParser
// ---------------------------------------------------------------------------

// Symbol: ??0XRibbonInfoParser@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParser_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1XRibbonInfoParser@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonInfoParser_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?ReadBool@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadBool_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* pThis, const void* /*name*/, int* /*value*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ReadColor@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ReadColor_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(
    void* pThis, const void* /*name*/, unsigned long* /*value*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ReadInt@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadInt_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* pThis, const void* /*name*/, int* /*value*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ReadSize@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCSize@@@Z
extern "C" int MS_ABI impl__ReadSize_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCSize___Z(
    void* pThis, const void* /*name*/, void* /*size*/) {
    (void)pThis;
    return 0;
}

// Symbol: ?ReadUInt@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAI@Z
extern "C" int MS_ABI impl__ReadUInt_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAI_Z(
    void* pThis, const void* /*name*/, unsigned int* /*value*/) {
    (void)pThis;
    return 0;
}
