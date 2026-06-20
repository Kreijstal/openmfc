// GDI drawing shard 01 - generated real implementations
//
// Implements RTTI helpers (GetThisClass/GetRuntimeClass) and CreateObject for
// GDI classes whose memory layout the repo headers define faithfully via
// DECLARE_DYNAMIC. These are unambiguous and safe to override.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// CGdiObject
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CGdiObject@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CGdiObject__UEBAPEAUCRuntimeClass__XZ(
    const CGdiObject* pThis) {
    (void)pThis;
    return CGdiObject::GetThisClass();
}

// Symbol: ?GetThisClass@CGdiObject@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CGdiObject__SAPEAUCRuntimeClass__XZ() {
    return CGdiObject::GetThisClass();
}

// Symbol: ?CreateObject@CGdiObject@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CGdiObject__SAPEAVCObject__XZ() {
    return new CGdiObject();
}

// ---------------------------------------------------------------------------
// CPen
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CPen@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPen__UEBAPEAUCRuntimeClass__XZ(
    const CPen* pThis) {
    (void)pThis;
    return CPen::GetThisClass();
}

// Symbol: ?GetThisClass@CPen@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPen__SAPEAUCRuntimeClass__XZ() {
    return CPen::GetThisClass();
}

// ---------------------------------------------------------------------------
// CBrush
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CBrush@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBrush__UEBAPEAUCRuntimeClass__XZ(
    const CBrush* pThis) {
    (void)pThis;
    return CBrush::GetThisClass();
}

// Symbol: ?GetThisClass@CBrush@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBrush__SAPEAUCRuntimeClass__XZ() {
    return CBrush::GetThisClass();
}

// ---------------------------------------------------------------------------
// CFont
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CFont@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFont__UEBAPEAUCRuntimeClass__XZ(
    const CFont* pThis) {
    (void)pThis;
    return CFont::GetThisClass();
}

// Symbol: ?GetThisClass@CFont@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFont__SAPEAUCRuntimeClass__XZ() {
    return CFont::GetThisClass();
}

// ---------------------------------------------------------------------------
// CBitmap
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CBitmap@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBitmap__UEBAPEAUCRuntimeClass__XZ(
    const CBitmap* pThis) {
    (void)pThis;
    return CBitmap::GetThisClass();
}

// Symbol: ?GetThisClass@CBitmap@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBitmap__SAPEAUCRuntimeClass__XZ() {
    return CBitmap::GetThisClass();
}

// ---------------------------------------------------------------------------
// CPalette
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CPalette@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPalette__UEBAPEAUCRuntimeClass__XZ(
    const CPalette* pThis) {
    (void)pThis;
    return CPalette::GetThisClass();
}

// Symbol: ?GetThisClass@CPalette@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPalette__SAPEAUCRuntimeClass__XZ() {
    return CPalette::GetThisClass();
}

// ---------------------------------------------------------------------------
// CDC
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDC__UEBAPEAUCRuntimeClass__XZ(
    const CDC* pThis) {
    (void)pThis;
    return CDC::GetThisClass();
}

// Symbol: ?GetThisClass@CDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDC__SAPEAUCRuntimeClass__XZ() {
    return CDC::GetThisClass();
}

// ---------------------------------------------------------------------------
// CClientDC
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CClientDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CClientDC__UEBAPEAUCRuntimeClass__XZ(
    const CClientDC* pThis) {
    (void)pThis;
    return CClientDC::GetThisClass();
}

// Symbol: ?GetThisClass@CClientDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CClientDC__SAPEAUCRuntimeClass__XZ() {
    return CClientDC::GetThisClass();
}

// ---------------------------------------------------------------------------
// CPaintDC
// ---------------------------------------------------------------------------

// Symbol: ?GetRuntimeClass@CPaintDC@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPaintDC__UEBAPEAUCRuntimeClass__XZ(
    const CPaintDC* pThis) {
    (void)pThis;
    return CPaintDC::GetThisClass();
}

// Symbol: ?GetThisClass@CPaintDC@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPaintDC__SAPEAUCRuntimeClass__XZ() {
    return CPaintDC::GetThisClass();
}
