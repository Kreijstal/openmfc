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

extern "C" long MS_ABI impl__GetTypeInfoCount_XAccessible_CWnd__UEAAJPEAI_Z(void* pThis, unsigned int* pctinfo);
extern "C" long MS_ABI impl__GetTypeInfo_XAccessible_CWnd__UEAAJIKPEAPEAUITypeInfo___Z(void* pThis, unsigned int iTInfo, unsigned long lcid, void** ppTInfo);
extern "C" long MS_ABI impl__GetIDsOfNames_XAccessible_CWnd__UEAAJAEBU_GUID__PEAPEA_WIKPEAJ_Z(void* pThis, const GUID* riid, wchar_t** rgszNames, unsigned int cNames, unsigned long lcid, long* rgDispId);
extern "C" long MS_ABI impl__get_accParent_XAccessible_CWnd__UEAAJPEAPEAUIDispatch___Z(void* pThis, void** ppdispParent);
extern "C" long MS_ABI impl__get_accChildCount_XAccessible_CWnd__UEAAJPEAJ_Z(void* pThis, long* pcountChildren);
extern "C" long MS_ABI impl__get_accChild_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEAUIDispatch___Z(void* pThis, VARIANT varChild, void** ppdispChild);
extern "C" long MS_ABI impl__get_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszName);
extern "C" long MS_ABI impl__get_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszValue);
extern "C" long MS_ABI impl__get_accDescription_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszDescription);
extern "C" long MS_ABI impl__get_accRole_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(void* pThis, VARIANT varChild, VARIANT* pvarRole);
extern "C" long MS_ABI impl__get_accState_XAccessible_CWnd__UEAAJUtagVARIANT__PEAU3__Z(void* pThis, VARIANT varChild, VARIANT* pvarState);
extern "C" long MS_ABI impl__get_accHelp_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszHelp);
extern "C" long MS_ABI impl__get_accHelpTopic_XAccessible_CWnd__UEAAJPEAPEA_WUtagVARIANT__PEAJ_Z(void* pThis, wchar_t** pszHelpFile, VARIANT varChild, long* pidTopic);
extern "C" long MS_ABI impl__get_accKeyboardShortcut_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszKeyboardShortcut);
extern "C" long MS_ABI impl__get_accFocus_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, VARIANT* pvarChild);
extern "C" long MS_ABI impl__get_accSelection_XAccessible_CWnd__UEAAJPEAUtagVARIANT___Z(void* pThis, VARIANT* pvarChildren);
extern "C" long MS_ABI impl__get_accDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT__PEAPEA_W_Z(void* pThis, VARIANT varChild, wchar_t** pszDefaultAction);
extern "C" long MS_ABI impl__accSelect_XAccessible_CWnd__UEAAJJUtagVARIANT___Z(void* pThis, long flagsSelect, VARIANT varChild);
extern "C" long MS_ABI impl__accLocation_XAccessible_CWnd__UEAAJPEAJ000UtagVARIANT___Z(void* pThis, long* pxLeft, long* pyTop, long* pcxWidth, long* pcyHeight, VARIANT varChild);
extern "C" long MS_ABI impl__accNavigate_XAccessible_CWnd__UEAAJJUtagVARIANT__PEAU3__Z(void* pThis, long navDir, VARIANT varStart, VARIANT* pvarEndUpAt);
extern "C" long MS_ABI impl__accHitTest_XAccessible_CWnd__UEAAJJJPEAUtagVARIANT___Z(void* pThis, long xLeft, long yTop, VARIANT* pvarChild);
extern "C" long MS_ABI impl__accDoDefaultAction_XAccessible_CWnd__UEAAJUtagVARIANT___Z(void* pThis, VARIANT varChild);
extern "C" long MS_ABI impl__put_accName_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, VARIANT varChild, wchar_t* szName);
extern "C" long MS_ABI impl__put_accValue_XAccessible_CWnd__UEAAJUtagVARIANT__PEA_W_Z(void* pThis, VARIANT varChild, wchar_t* szValue);
extern "C" long MS_ABI impl__Invoke_XAccessible_CWnd__UEAAJJAEBU_GUID__KGPEAUtagDISPPARAMS__PEAUtagVARIANT__PEAUtagEXCEPINFO__PEAI_Z(void* pThis, long dispIdMember, const GUID* riid, unsigned long lcid, unsigned short wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, unsigned int* puArgErr);
extern "C" long MS_ABI impl__SetProxy_XAccessibleServer_CWnd__UEAAJPEAUIAccessibleProxy___Z(void* pThis, void* pProxy);
extern "C" long MS_ABI impl__GetHWND_XAccessibleServer_CWnd__UEAAJPEAPEAUHWND_____Z(void* pThis, HWND* phWnd);
extern "C" long MS_ABI impl__GetEnumVariant_XAccessibleServer_CWnd__UEAAJPEAPEAUIEnumVARIANT___Z(void* pThis, void** ppEnumVariant);
extern "C" long MS_ABI impl__SetProxy_CWnd__UEAAJPEAUIAccessibleProxy___Z(CWnd*, void*);
extern "C" long MS_ABI impl__CreateAccessibleProxy_CWnd__UEAAJ_K_JPEA_J_Z(CWnd*, unsigned __int64, __int64, __int64*);
extern "C" __int64 MS_ABI impl__OnGetObject_CWnd__IEAA_J_K_J_Z(CWnd*, unsigned __int64, __int64);
