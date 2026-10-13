#pragma once
#include "detail/CCmdTargetVirtualsSupport.h"
#include <oaidl.h>
extern "C" const void* MS_ABI impl__GetDispEntry_CCmdTarget__IEAAPEBUAFX_DISPMAP_ENTRY__J_Z(CCmdTarget* pThis, long memid);
extern "C" unsigned int MS_ABI impl__GetEntryCount_CCmdTarget__KAIPEBUAFX_DISPMAP___Z(const void* pDispMap);
extern "C" long MS_ABI impl__MemberIDFromName_CCmdTarget__KAJPEBUAFX_DISPMAP__PEB_W_Z(const void* pDispMap, const wchar_t* lpszName);
extern "C" long MS_ABI impl__SetStandardProp_CCmdTarget__IEAAJPEBUAFX_DISPMAP_ENTRY__PEAUtagDISPPARAMS__PEAI_Z(CCmdTarget* pThis, const void* pEntryV, DISPPARAMS* pDispParams, unsigned int* puArgErr);
extern "C" void MS_ABI impl__GetStandardProp_CCmdTarget__IEAAXPEBUAFX_DISPMAP_ENTRY__PEAUtagVARIANT__PEAI_Z(CCmdTarget* pThis, const void* pEntry, void* pvarResult, unsigned int* puArgErr);
extern "C" void MS_ABI impl__EnableAutomation_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
extern "C" void* MS_ABI impl__FromIDispatch_CCmdTarget__SAPEAV1_PEAUIDispatch___Z(void* pDispatch);
extern "C" void* MS_ABI impl__GetIDispatch_CCmdTarget__QEAAPEAUIDispatch__H_Z(CCmdTarget* pThis, int bAddRef);
extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget*);
HRESULT ReadDispatchProperty(CCmdTarget*, const AFX_DISPMAP_ENTRY*, VARIANT*);
void** CommandTargetDispatchTable();

extern "C" unsigned long MS_ABI impl__ExternalRelease_CCmdTarget__QEAAKXZ(CCmdTarget* pThis);
extern "C" unsigned long MS_ABI impl__ExternalQueryInterface_CCmdTarget__QEAAKPEBXPEAPEAX_Z(CCmdTarget* pThis, const void* iid, void** ppvObj);
extern "C" void MS_ABI impl__ExternalDisconnect_CCmdTarget__QEAAXXZ(CCmdTarget* pThis);
