#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// This shard implements MFC RTTI accessors (GetThisClass / GetRuntimeClass)
// for OLE/COM/ActiveX classes that are fully defined in the OpenMFC headers
// with DECLARE_DYNAMIC + IMPLEMENT_DYNAMIC. These are unambiguous: GetThisClass
// returns the static CRuntimeClass for the class, and GetRuntimeClass returns
// the same (virtual) value. We null-guard the virtual variants.

//-----------------------------------------------------------------------------
// CCmdTarget
//-----------------------------------------------------------------------------
// Symbol: ?GetRuntimeClass@CCmdTarget@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CCmdTarget__UEBAPEAUCRuntimeClass__XZ(
        const CCmdTarget* pThis) {
    if (!pThis) return CCmdTarget::GetThisClass();
    return CCmdTarget::GetThisClass();
}

//-----------------------------------------------------------------------------
// COleBusyDialog
//-----------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleBusyDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleBusyDialog__SAPEAUCRuntimeClass__XZ() {
    return COleBusyDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleBusyDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleBusyDialog__UEBAPEAUCRuntimeClass__XZ(
        const COleBusyDialog* pThis) {
    if (!pThis) return COleBusyDialog::GetThisClass();
    return COleBusyDialog::GetThisClass();
}

//-----------------------------------------------------------------------------
// COleChangeIconDialog
//-----------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleChangeIconDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleChangeIconDialog__SAPEAUCRuntimeClass__XZ() {
    return COleChangeIconDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleChangeIconDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleChangeIconDialog__UEBAPEAUCRuntimeClass__XZ(
        const COleChangeIconDialog* pThis) {
    if (!pThis) return COleChangeIconDialog::GetThisClass();
    return COleChangeIconDialog::GetThisClass();
}

//-----------------------------------------------------------------------------
// COleChangeSourceDialog
//-----------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleChangeSourceDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleChangeSourceDialog__SAPEAUCRuntimeClass__XZ() {
    return COleChangeSourceDialog::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleChangeSourceDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleChangeSourceDialog__UEBAPEAUCRuntimeClass__XZ(
        const COleChangeSourceDialog* pThis) {
    if (!pThis) return COleChangeSourceDialog::GetThisClass();
    return COleChangeSourceDialog::GetThisClass();
}

//-----------------------------------------------------------------------------
// COleClientItem
//-----------------------------------------------------------------------------
// Symbol: ?GetThisClass@COleClientItem@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_COleClientItem__SAPEAUCRuntimeClass__XZ() {
    return COleClientItem::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleClientItem@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_COleClientItem__UEBAPEAUCRuntimeClass__XZ(
        const COleClientItem* pThis) {
    if (!pThis) return COleClientItem::GetThisClass();
    return COleClientItem::GetThisClass();
}
