// COleServerDocEx retains the COleServerDoc layout and uses the same activation state.
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"
#include <new>

static_assert(sizeof(COleServerDocEx) == 0x298);

extern "C" CRuntimeClass* AFXAPI impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ();
extern "C" int AFXAPI impl__OnNewDocument_COleLinkingDoc__UEAAHXZ(void*);
extern "C" void AFXAPI impl__Serialize_COleDocument__UEAAXAEAVCArchive___Z(void*, void*);

COleServerDocEx::~COleServerDocEx() = default;

CRuntimeClass* AFXAPI COleServerDocEx::GetThisClass() {
    return impl__GetThisClass_COleServerDocEx__SAPEAUCRuntimeClass__XZ();
}

CRuntimeClass* COleServerDocEx::GetRuntimeClass() const {
    return GetThisClass();
}

void COleServerDocEx::OnDocWindowActivate(BOOL active) {
    COleServerDoc::OnDocWindowActivate(active);
}

void COleServerDocEx::OnResizeBorder(LPCRECT border, IOleInPlaceUIWindow* window, BOOL main) {
    if (!window) AfxThrowInvalidArgException();
    COleServerDoc::OnResizeBorder(border, window, main);
}

void COleServerDocEx::OnShowControlBars(CFrameWnd* frame, BOOL show) {
    COleServerDoc::OnShowControlBars(frame, show);
}

BOOL COleServerDocEx::OnNewDocument() {
    return impl__OnNewDocument_COleLinkingDoc__UEAAHXZ(this) != FALSE;
}

void COleServerDocEx::Serialize(CArchive& archive) {
    impl__Serialize_COleDocument__UEAAXAEAVCArchive___Z(this, &archive);
}

// Symbol: ?CreateObject@COleServerDocEx@@SAPEAVCObject@@XZ
extern "C" CObject* AFXAPI impl__CreateObject_COleServerDocEx__SAPEAVCObject__XZ() {
    return new COleServerDocEx;
}

// Symbol: ??0COleServerDocEx@@IEAA@XZ
extern "C" COleServerDocEx* AFXAPI impl___0COleServerDocEx__IEAA_XZ(void* storage) {
    return storage ? ::new (storage) COleServerDocEx : nullptr;
}

// Symbol: ??1COleServerDocEx@@UEAA@XZ
extern "C" void AFXAPI impl___1COleServerDocEx__UEAA_XZ(COleServerDocEx* document) {
    if (document) document->COleServerDocEx::~COleServerDocEx();
}

// Symbol: ?OnGetEmbeddedItem@COleServerDocEx@@MEAAPEAVCOleServerItem@@XZ
extern "C" COleServerItem* AFXAPI impl__OnGetEmbeddedItem_COleServerDocEx__MEAAPEAVCOleServerItem__XZ(
    COleServerDocEx*) {
    return nullptr; // The retail default is supplied by the application subclass.
}

// Symbol: ?OnDocWindowActivate@COleServerDocEx@@MEAAXH@Z
extern "C" void AFXAPI impl__OnDocWindowActivate_COleServerDocEx__MEAAXH_Z(COleServerDocEx* document, BOOL active) {
    if (document) document->COleServerDocEx::OnDocWindowActivate(active);
}

// Symbol: ?OnResizeBorder@COleServerDocEx@@MEAAXPEBUtagRECT@@PEAUIOleInPlaceUIWindow@@H@Z
extern "C" void AFXAPI impl__OnResizeBorder_COleServerDocEx__MEAAXPEBUtagRECT__PEAUIOleInPlaceUIWindow__H_Z(
    COleServerDocEx* document, LPCRECT border, IOleInPlaceUIWindow* window, BOOL main) {
    if (document) document->COleServerDocEx::OnResizeBorder(border, window, main);
}

// Symbol: ?OnShowControlBars@COleServerDocEx@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void AFXAPI impl__OnShowControlBars_COleServerDocEx__MEAAXPEAVCFrameWnd__H_Z(
    COleServerDocEx* document, CFrameWnd* frame, BOOL show) {
    if (document) document->COleServerDocEx::OnShowControlBars(frame, show);
}

// Symbol: ?OnNewDocument@COleServerDocEx@@MEAAHXZ
extern "C" BOOL AFXAPI impl__OnNewDocument_COleServerDocEx__MEAAHXZ(COleServerDocEx* document) {
    return document && document->COleServerDocEx::OnNewDocument();
}

// Symbol: ?Serialize@COleServerDocEx@@UEAAXAEAVCArchive@@@Z
extern "C" void AFXAPI impl__Serialize_COleServerDocEx__UEAAXAEAVCArchive___Z(
    COleServerDocEx* document, CArchive& archive) {
    if (document) document->COleServerDocEx::Serialize(archive);
}
