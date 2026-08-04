// CBaseTabbedPane — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ??0CBaseTabbedPane@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CBaseTabbedPane__QEAA_H_Z(void* pThis, int iStyle) { return new (pThis) CBaseTabbedPane(iStyle); }
// Symbol: ??1CBaseTabbedPane@@UEAA@XZ
extern "C" void MS_ABI impl___1CBaseTabbedPane__UEAA_XZ(CBaseTabbedPane* pThis) { if (pThis) pThis->~CBaseTabbedPane(); }
CBaseTabbedPane::CBaseTabbedPane(int iStyle) {
    memset(_basetabbedpane_padding, 0, sizeof(_basetabbedpane_padding));
    (void)iStyle;
}
CBaseTabbedPane::~CBaseTabbedPane() {}
