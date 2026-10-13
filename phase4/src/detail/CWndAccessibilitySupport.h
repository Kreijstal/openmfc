#pragma once
#include "openmfc/afxwin.h"
#ifndef MS_ABI
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
#endif
// VARIANT is a 24-byte by-value argument in MFC signatures. The x64 MSVC
// calling convention passes it through a pointer to caller-owned storage.
extern "C" long MS_ABI impl__EnsureStdObj_CWnd__UEAAJXZ(CWnd* pThis);
extern "C" long MS_ABI impl__accNavigate_CWnd__UEAAJJUtagVARIANT__PEAU2__Z(void* pThis, long p0, void* p1, void* p2);
extern "C" long MS_ABI impl__accDoDefaultAction_CWnd__UEAAJUtagVARIANT___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__accHitTest_CWnd__UEAAJJJPEAUtagVARIANT___Z(void* pThis, long p0, long p1, void* p2);
extern "C" long MS_ABI impl__accLocation_CWnd__UEAAJPEAJ000UtagVARIANT___Z(void* pThis, void* p0, void* p1, void* p2, void* p3, void* p4);
extern "C" long MS_ABI impl__accSelect_CWnd__UEAAJJUtagVARIANT___Z(void* pThis, long p0, void* p1);
extern "C" long MS_ABI impl__get_accChildCount_CWnd__UEAAJPEAJ_Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accChild_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accDefaultAction_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accDescription_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accFocus_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accHelp_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accHelpTopic_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(void* pThis, void* p0, void* p1, void* p2);
extern "C" long MS_ABI impl__get_accKeyboardShortcut_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accName_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accParent_CWnd__UEAAJPEAPEAUIDispatch___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accRole_CWnd__UEAAJUtagVARIANT__PEAU2__Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accSelection_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, void* p0);
extern "C" long MS_ABI impl__get_accState_CWnd__UEAAJUtagVARIANT__PEAU2__Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__get_accValue_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__put_accName_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, void* p0, void* p1);
extern "C" long MS_ABI impl__put_accValue_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, void* p0, void* p1);
