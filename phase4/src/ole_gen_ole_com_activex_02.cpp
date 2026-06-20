// OLE/COM ActiveX shard 02 - real exports overriding weak stubs.
// Implements RTTI helpers (GetThisClass/GetRuntimeClass) and message-map
// accessors for COleControl. COleControl is declared with DECLARE_DYNAMIC in
// afxole.h and registered via IMPLEMENT_DYNAMIC(COleControl, CWnd) in
// olecore.cpp, so its static runtime class and the CWnd message map are
// available here.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// COleControl runtime class (RTTI)
// ---------------------------------------------------------------------------

// Symbol: ?GetThisClass@COleControl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetThisClass_COleControl__SAPEAUCRuntimeClass__XZ() {
    return COleControl::GetThisClass();
}

// Symbol: ?GetRuntimeClass@COleControl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI
impl__GetRuntimeClass_COleControl__UEBAPEAUCRuntimeClass__XZ(const COleControl* pThis) {
    (void)pThis;
    return COleControl::GetThisClass();
}

// ---------------------------------------------------------------------------
// COleControl message map accessors
// COleControl carries no additional message-map entries that we model, so the
// base CWnd message map is the correct fallback.
// ---------------------------------------------------------------------------

// Symbol: ?GetThisMessageMap@COleControl@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI
impl__GetThisMessageMap_COleControl__KAPEBUAFX_MSGMAP__XZ() {
    return CWnd::GetThisMessageMap();
}

// Symbol: ?GetMessageMap@COleControl@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI
impl__GetMessageMap_COleControl__MEBAPEBUAFX_MSGMAP__XZ(const COleControl* pThis) {
    (void)pThis;
    return CWnd::GetThisMessageMap();
}
