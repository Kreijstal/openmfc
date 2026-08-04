// CMenuHash — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ??0CMenuHash@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMenuHash__QEAA_XZ(void* pThis) { return new (pThis) CMenuHash(); }
// Symbol: ??1CMenuHash@@UEAA@XZ
extern "C" void MS_ABI impl___1CMenuHash__UEAA_XZ(CMenuHash* pThis) { if (pThis) pThis->~CMenuHash(); }
// Symbol: ?LoadMenuBar@CMenuHash@@QEAAHPEAUHMENU__@@PEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__LoadMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU menu, CMFCToolBar* bar) { return pThis ? pThis->LoadMenuBar(menu, bar) : FALSE; }
// Symbol: ?SaveMenuBar@CMenuHash@@QEAAHPEAUHMENU__@@PEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__SaveMenuBar_CMenuHash__QEAAHPEAUHMENU____PEAVCMFCToolBar___Z(CMenuHash* pThis, HMENU menu, CMFCToolBar* bar) { return pThis ? pThis->SaveMenuBar(menu, bar) : FALSE; }
// Symbol: ?RemoveMenu@CMenuHash@@QEAAHPEAUHMENU__@@@Z
extern "C" int MS_ABI impl__RemoveMenu_CMenuHash__QEAAHPEAUHMENU_____Z(CMenuHash* pThis, HMENU menu) { return pThis ? pThis->RemoveMenu(menu) : FALSE; }
// Symbol: ?CleanUp@CMenuHash@@QEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMenuHash__QEAAXXZ(CMenuHash* pThis) { if (pThis) pThis->CleanUp(); }
CMenuHash::CMenuHash() { memset(_menuhash_padding, 0, sizeof(_menuhash_padding)); }
CMenuHash::~CMenuHash() { g_menuHashStates.erase(this); }
BOOL CMenuHash::LoadMenuBar(HMENU hMenu, CMFCToolBar* pBar) { g_menuHashStates[this].barsByMenu[hMenu] = pBar; return TRUE; }
BOOL CMenuHash::SaveMenuBar(HMENU hMenu, CMFCToolBar* pBar) { g_menuHashStates[this].barsByMenu[hMenu] = pBar; return TRUE; }
BOOL CMenuHash::RemoveMenu(HMENU hMenu) { return g_menuHashStates[this].barsByMenu.erase(hMenu) != 0; }
void CMenuHash::CleanUp() { g_menuHashStates[this].barsByMenu.clear(); }
