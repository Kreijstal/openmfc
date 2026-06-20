// OpenMFC - ole_com_activex shard 06/06
// Real implementations overriding weak stubs for a conservative subset of
// COleUpdateDialog (RTTI) and CView (OLE drag/drop default behaviors).

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// DROPEFFECT_NONE == 0; the "not handled" sentinel for OnDropEx is (DROPEFFECT)-1.
#ifndef DROPEFFECT_NONE
  #define DROPEFFECT_NONE 0
#endif

// ---------------------------------------------------------------------------
// COleUpdateDialog - runtime class (RTTI). classCOleUpdateDialog static is
// defined via IMPLEMENT_DYNAMIC(COleUpdateDialog, COleDialog) in olecore.cpp.
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@COleUpdateDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_COleUpdateDialog__SAPEAUCRuntimeClass__XZ() {
    return COleUpdateDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleUpdateDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_COleUpdateDialog__UEBAPEAUCRuntimeClass__XZ(
    const COleUpdateDialog* pThis) {
    (void)pThis;
    return COleUpdateDialog::GetThisClass();
}

// ---------------------------------------------------------------------------
// CView - default OLE drag/drop handlers. These base-class implementations
// indicate "no drop support" and match MFC's documented defaults.
// ---------------------------------------------------------------------------

// Symbol: ?OnDragEnter@CView@@UEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDragEnter_CView__UEAAKPEAVCOleDataObject__KVCPoint___Z(
    CView* pThis, COleDataObject* pDataObject, unsigned long dwKeyState,
    CPoint point) {
    (void)pThis; (void)pDataObject; (void)dwKeyState; (void)point;
    return DROPEFFECT_NONE;
}

// Symbol: ?OnDragOver@CView@@UEAAKPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDragOver_CView__UEAAKPEAVCOleDataObject__KVCPoint___Z(
    CView* pThis, COleDataObject* pDataObject, unsigned long dwKeyState,
    CPoint point) {
    (void)pThis; (void)pDataObject; (void)dwKeyState; (void)point;
    return DROPEFFECT_NONE;
}

// Symbol: ?OnDrop@CView@@UEAAHPEAVCOleDataObject@@KVCPoint@@@Z
extern "C" int MS_ABI
impl__OnDrop_CView__UEAAHPEAVCOleDataObject__KVCPoint___Z(
    CView* pThis, COleDataObject* pDataObject, unsigned long dropEffect,
    CPoint point) {
    (void)pThis; (void)pDataObject; (void)dropEffect; (void)point;
    return 0; // FALSE - not handled
}

// Symbol: ?OnDropEx@CView@@UEAAKPEAVCOleDataObject@@KKVCPoint@@@Z
extern "C" unsigned long MS_ABI
impl__OnDropEx_CView__UEAAKPEAVCOleDataObject__KKVCPoint___Z(
    CView* pThis, COleDataObject* pDataObject, unsigned long dropDefault,
    unsigned long dropList, CPoint point) {
    (void)pThis; (void)pDataObject; (void)dropDefault; (void)dropList;
    (void)point;
    // (DROPEFFECT)-1 tells the framework OnDropEx is not implemented so it
    // should fall back to OnDrop.
    return static_cast<unsigned long>(-1);
}
