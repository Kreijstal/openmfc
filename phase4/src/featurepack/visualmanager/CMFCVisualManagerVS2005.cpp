// CMFCVisualManagerVS2005 — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ?CreateObject@CMFCVisualManagerVS2005@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCVisualManagerVS2005__SAPEAVCObject__XZ() { return new CMFCVisualManagerVS2005(); }
// Symbol: ?GetThisClass@CMFCVisualManagerVS2005@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ() { return CMFCVisualManagerVS2005::GetThisClass(); }
// Symbol: ?GetRuntimeClass@CMFCVisualManagerVS2005@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMFCVisualManagerVS2005__UEBAPEAUCRuntimeClass__XZ(CMFCVisualManagerVS2005* pThis) { return CMFCVisualManagerVS2005::GetThisClass(); }
CMFCVisualManagerVS2005::CMFCVisualManagerVS2005() {
    memset(_pad, 0, sizeof(_pad));
    impl__m_bRoundedAutohideButtons_CMFCVisualManagerVS2005__2HA = TRUE;
    ApplyVisualPaletteForClass(GetThisClass());
}
CMFCVisualManagerVS2005::~CMFCVisualManagerVS2005() {}
CObject* CMFCVisualManagerVS2005::CreateObject() { return new CMFCVisualManagerVS2005(); }
