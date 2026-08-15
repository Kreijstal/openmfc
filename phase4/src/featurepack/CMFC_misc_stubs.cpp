// Auto-generated remaining non-class stub implementations
// Kept intentionally grouped to ensure ABI coverage while class-specific
// source placement is unresolved.
#include <cstddef>
#include <windows.h>
#include <objbase.h>
#include <ocidl.h>
#include "openmfc/afxwin.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
int g_bPerUserRegistration = 0;
}
// Symbol: ?AFXGetParentFrame@@YAPEAVCFrameWnd@@PEBVCWnd@@@Z
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const CWnd* pWnd) {
    return pWnd ? pWnd->GetParentFrame() : nullptr;
}
// Symbol: ?AFXGetRegPath@@YA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0@Z
extern "C" void* MS_ABI impl__AFXGetRegPath__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(void* /*class*/* p0, void** p1, void* p2, const wchar_t* p3, void* /*class*/* p4) {
    return nullptr;
}
// Symbol: ?AFXPlaySystemSound@@YAXH@Z
extern "C" void MS_ABI impl__AFXPlaySystemSound__YAXH_Z(int p0) {}
// Symbol: ?AFXPrintPreview@@YAXPEAVCView@@@Z
extern "C" void MS_ABI impl__AFXPrintPreview__YAXPEAVCView___Z(void* /*class*/* p0) {}
// Symbol: ?AFXSoundThreadProc@@YAXPEAX@Z
extern "C" void MS_ABI impl__AFXSoundThreadProc__YAXPEAX_Z(void* p0) {}
// Symbol: ?AfxApplicationRecoveryWrapper@@YAKPEAX@Z
extern "C" unsigned long MS_ABI impl__AfxApplicationRecoveryWrapper__YAKPEAX_Z(void* p0) {
    return 0;
}
// Symbol: ?AfxAutosaveTimerProc@@YAXPEAUHWND__@@I_KK@Z
extern "C" void MS_ABI impl__AfxAutosaveTimerProc__YAXPEAUHWND____I_KK_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, unsigned long p3) {}
// Symbol: ?AfxBeginThread@@YAPEAVCWinThread@@P6AIPEAX@Z0HIKPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI impl__AfxBeginThread__YAPEAVCWinThread__P6AIPEAX_Z0HIKPEAU_SECURITY_ATTRIBUTES___Z(void* /*fnptr*/ p0, void* /*fnptr*/ p1, int p2, unsigned int p3, unsigned long p4, void* /*struct*/* p5) {
    return nullptr;
}
// Symbol: ?AfxBeginThread@@YAPEAVCWinThread@@PEAUCRuntimeClass@@HIKPEAU_SECURITY_ATTRIBUTES@@@Z
extern "C" void* MS_ABI impl__AfxBeginThread__YAPEAVCWinThread__PEAUCRuntimeClass__HIKPEAU_SECURITY_ATTRIBUTES___Z(void* /*struct*/* p0, int p1, unsigned int p2, unsigned long p3, void* /*struct*/* p4) {
    return nullptr;
}
// Symbol: ?AfxCallWndProc@@YA_JPEAVCWnd@@PEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxCallWndProc__YA_JPEAVCWnd__PEAUHWND____I_K_J_Z(void* /*class*/* p0, void* /*struct*/* p1, unsigned int p2, unsigned __int64 p3, __int64 p4) {
    return 0;
}
// Symbol: ?AfxCancelModes@@YAXPEAUHWND__@@@Z
extern "C" void MS_ABI impl__AfxCancelModes__YAXPEAUHWND_____Z(void* /*struct*/* p0) {}
// Symbol: ?AfxCheckError@@YAXJ@Z
extern "C" void MS_ABI impl__AfxCheckError__YAXJ_Z(long p0) {}
// Symbol: ?AfxCompareValueByRef@@YAHPEAX0H@Z
extern "C" int MS_ABI impl__AfxCompareValueByRef__YAHPEAX0H_Z(void* p0, void* p1, int p2) {
    return 0;
}
// Symbol: ?AfxConnectionAdvise@@YAHPEAUIUnknown@@AEBU_GUID@@0HPEAK@Z
extern "C" int MS_ABI impl__AfxConnectionAdvise__YAHPEAUIUnknown__AEBU_GUID__0HPEAK_Z(
    IUnknown* pUnkSrc, const GUID* iid, IUnknown* pUnkSink, int bRefCount, unsigned long* pdwCookie) {
    (void)bRefCount;
    if (!pUnkSrc || !iid || !pUnkSink || !pdwCookie) return FALSE;
    IConnectionPointContainer* pCPC = nullptr;
    if (FAILED(pUnkSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC))) return FALSE;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pCPC->FindConnectionPoint(*iid, &pCP);
    pCPC->Release();
    if (FAILED(hr)) return FALSE;
    hr = pCP->Advise(pUnkSink, pdwCookie);
    pCP->Release();
    return SUCCEEDED(hr);
}
// Symbol: ?AfxConnectionUnadvise@@YAHPEAUIUnknown@@AEBU_GUID@@0HK@Z
extern "C" int MS_ABI impl__AfxConnectionUnadvise__YAHPEAUIUnknown__AEBU_GUID__0HK_Z(
    IUnknown* pUnkSrc, const GUID* iid, IUnknown* pUnkSink, int bRefCount, unsigned long dwCookie) {
    (void)pUnkSink;
    (void)bRefCount;
    if (!pUnkSrc || !iid) return FALSE;
    IConnectionPointContainer* pCPC = nullptr;
    if (FAILED(pUnkSrc->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC))) return FALSE;
    IConnectionPoint* pCP = nullptr;
    HRESULT hr = pCPC->FindConnectionPoint(*iid, &pCP);
    pCPC->Release();
    if (FAILED(hr)) return FALSE;
    hr = pCP->Unadvise(dwCookie);
    pCP->Release();
    return SUCCEEDED(hr);
}
// Symbol: ?AfxCopyValueByRef@@YAXPEAX0PEA_JH@Z
extern "C" void MS_ABI impl__AfxCopyValueByRef__YAXPEAX0PEA_JH_Z(void* p0, void* p1, __int64* p2, int p3) {}
// Symbol: ?AfxCoreInitModule@@YAXXZ
extern "C" void MS_ABI impl__AfxCoreInitModule__YAXXZ() {}
// Symbol: ?AfxCreateDC@@YAPEAUHDC__@@PEAX0@Z
extern "C" void* MS_ABI impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(void* p0, void* p1) {
    return nullptr;
}
// Symbol: ?AfxCriticalInit@@YAHXZ
extern "C" int MS_ABI impl__AfxCriticalInit__YAHXZ() {
    return 0;
}
// Symbol: ?AfxCriticalNewHandler@@YAH_K@Z
extern "C" int MS_ABI impl__AfxCriticalNewHandler__YAH_K_Z(unsigned __int64 p0) {
    return 0;
}
// Symbol: ?AfxCriticalTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxCriticalTerm__YAXXZ() {}
// Symbol: ?AfxCustomLogFont@@YAHIPEAUtagLOGFONTW@@@Z
extern "C" int MS_ABI impl__AfxCustomLogFont__YAHIPEAUtagLOGFONTW___Z(unsigned int p0, void* /*struct*/* p1) {
    return 0;
}
// Symbol: ?AfxDelRegTreeHelper@@YAJPEAUHKEY__@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEAVCAtlTransactionManager@3@@Z
extern "C" long MS_ABI impl__AfxDelRegTreeHelper__YAJPEAUHKEY____AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCAtlTransactionManager_3__Z(void* /*struct*/* p0, const void* /*class*/* p1, void** p2, void* p3, void* /*class*/* p4) {
    return 0;
}
// Symbol: ?AfxDeleteObject@@YAXPEAPEAX@Z
extern "C" void MS_ABI impl__AfxDeleteObject__YAXPEAPEAX_Z(void** p0) {}
// Symbol: ?AfxDlgProc@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxDlgProc__YA_JPEAUHWND____I_K_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return 0;
}
// Symbol: ?AfxDllCanUnloadNow@@YAJXZ
extern "C" long MS_ABI impl__AfxDllCanUnloadNow__YAJXZ() {
    return 0;
}
// Symbol: ?AfxDllGetClassObject@@YAJAEBU_GUID@@0PEAPEAX@Z
extern "C" long MS_ABI impl__AfxDllGetClassObject__YAJAEBU_GUID__0PEAPEAX_Z(const void* /*struct*/* p0, const void* /*struct*/* p1, void** p2) {
    return 0;
}
// Symbol: ?AfxDoRegisterMFCControlClass@@YAXPEB_W0@Z
extern "C" void MS_ABI impl__AfxDoRegisterMFCControlClass__YAXPEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {}
// Symbol: ?AfxDrawDitheredBitmap@@YAXPEAVCDC@@HHAEBVCBitmap@@KK@Z
extern "C" void MS_ABI impl__AfxDrawDitheredBitmap__YAXPEAVCDC__HHAEBVCBitmap__KK_Z(void* /*class*/* p0, int p1, int p2, const void* /*class*/* p3, unsigned long p4, unsigned long p5) {}
// Symbol: ?AfxDrawGrayBitmap@@YAXPEAVCDC@@HHAEBVCBitmap@@K@Z
extern "C" void MS_ABI impl__AfxDrawGrayBitmap__YAXPEAVCDC__HHAEBVCBitmap__K_Z(void* /*class*/* p0, int p1, int p2, const void* /*class*/* p3, unsigned long p4) {}
// Symbol: ?AfxEditviewTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxEditviewTerm__YAXXZ() {}
// Symbol: ?AfxEnableControlContainer@@YAXPEAVCOccManager@@@Z
extern "C" void MS_ABI impl__AfxEnableControlContainer__YAXPEAVCOccManager___Z(void* /*class*/* p0) {}
// Symbol: ?AfxEndDeferRegisterClass@@YAHJ@Z
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long p0) {
    return 0;
}
// Symbol: ?AfxEndThread@@YAXIH@Z
extern "C" void MS_ABI impl__AfxEndThread__YAXIH_Z(unsigned int p0, int p1) {}
// Symbol: ?AfxEnumMetaFileProc@@YAHPEAUHDC__@@PEAUtagHANDLETABLE@@PEAUtagMETARECORD@@H_J@Z
extern "C" int MS_ABI impl__AfxEnumMetaFileProc__YAHPEAUHDC____PEAUtagHANDLETABLE__PEAUtagMETARECORD__H_J_Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2, int p3, __int64 p4) {
    return 0;
}
// Symbol: ?AfxFindMessageEntry@@YAPEBUAFX_MSGMAP_ENTRY@@PEBU1@III@Z
extern "C" void* MS_ABI impl__AfxFindMessageEntry__YAPEBUAFX_MSGMAP_ENTRY__PEBU1_III_Z(const void* /*struct*/* p0, unsigned int p1, unsigned int p2, unsigned int p3) {
    return nullptr;
}
// Symbol: ?AfxFindStringResourceHandle@@YAPEAUHINSTANCE__@@I@Z
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int p0) {
    return nullptr;
}
// Symbol: ?AfxGetAfxWndProc@@YAP6A_JPEAUHWND__@@I_K_J@ZXZ
extern "C" void* MS_ABI impl__AfxGetAfxWndProc__YAP6A_JPEAUHWND____I_K_J_ZXZ() {
    return nullptr;
}
// Symbol: ?AfxGetClassIDFromString@@YAJPEB_WPEAU_GUID@@@Z
extern "C" long MS_ABI impl__AfxGetClassIDFromString__YAJPEB_WPEAU_GUID___Z(const wchar_t* p0, void* /*struct*/* p1) {
    return 0;
}
// Symbol: ?AfxGetCurrentMessage@@YAPEAUtagMSG@@XZ
extern "C" void* MS_ABI impl__AfxGetCurrentMessage__YAPEAUtagMSG__XZ() {
    return nullptr;
}
// Symbol: ?AfxGetDitheredBitmap@@YAXAEBVCBitmap@@PEAV1@KK@Z
extern "C" void MS_ABI impl__AfxGetDitheredBitmap__YAXAEBVCBitmap__PEAV1_KK_Z(const void* /*class*/* p0, void* /*class*/* p1, unsigned long p2, unsigned long p3) {}
// Symbol: ?AfxGetDlgItemStartFromHWND@@YAPEAUHWND__@@HPEAU1@@Z
extern "C" void* MS_ABI impl__AfxGetDlgItemStartFromHWND__YAPEAUHWND____HPEAU1__Z(int p0, void* /*struct*/* p1) {
    return nullptr;
}
// Symbol: ?AfxGetFileTitle@@YAIPEB_WPEA_WI@Z
extern "C" unsigned int MS_ABI impl__AfxGetFileTitle__YAIPEB_WPEA_WI_Z(const wchar_t* p0, wchar_t* p1, unsigned int p2) {
    return 0;
}
// Symbol: ?AfxGetGrayBitmap@@YAXAEBVCBitmap@@PEAV1@K@Z
extern "C" void MS_ABI impl__AfxGetGrayBitmap__YAXAEBVCBitmap__PEAV1_K_Z(const void* /*class*/* p0, void* /*class*/* p1, unsigned long p2) {}
// Symbol: ?AfxGetHENV@@YAPEAXXZ
extern "C" void* MS_ABI impl__AfxGetHENV__YAPEAXXZ() {
    return nullptr;
}
// Symbol: ?AfxGetInProcServer@@YAHPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__AfxGetInProcServer__YAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* p0, void* /*class*/* p1, void** p2, void* p3) {
    return 0;
}
// Symbol: ?AfxGetInternetHandleType@@YAKPEAX@Z
extern "C" unsigned long MS_ABI impl__AfxGetInternetHandleType__YAKPEAX_Z(void* p0) {
    return 0;
}
// Symbol: ?AfxGetModuleFileName@@YAXPEAUHINSTANCE__@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__AfxGetModuleFileName__YAXPEAUHINSTANCE____AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*struct*/* p0, void* /*class*/* p1, void** p2, void* p3) {}
// Symbol: ?AfxGetModuleShortFileName@@YAXPEAUHINSTANCE__@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__AfxGetModuleShortFileName__YAXPEAUHINSTANCE____AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* /*struct*/* p0, void* /*class*/* p1, void** p2, void* p3) {}
// Symbol: ?AfxGetNewHandler@@YAP6AH_K@ZXZ
extern "C" void* MS_ABI impl__AfxGetNewHandler__YAP6AH_K_ZXZ() {
    return nullptr;
}
// Symbol: ?AfxGetParentOwner@@YAPEAUHWND__@@PEAU1@@Z
extern "C" void* MS_ABI impl__AfxGetParentOwner__YAPEAUHWND____PEAU1__Z(void* /*struct*/* p0) {
    return nullptr;
}
// Symbol: ?AfxGetPerUserRegistration@@YAHXZ
extern "C" int MS_ABI impl__AfxGetPerUserRegistration__YAHXZ() {
    return g_bPerUserRegistration;
}
// Symbol: ?AfxGetPropSheetFont@@YAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAGH@Z
extern "C" int MS_ABI impl__AfxGetPropSheetFont__YAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAGH_Z(void* /*class*/* p0, void** p1, void* p2, unsigned short* p3, int p4) {
    return 0;
}
// Symbol: ?AfxGetRoot@@YAXPEB_WAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__AfxGetRoot__YAXPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const wchar_t* p0, void* /*class*/* p1, void** p2, void* p3) {}
// Symbol: ?AfxGlobalFree@@YAXPEAX@Z
extern "C" void MS_ABI impl__AfxGlobalFree__YAXPEAX_Z(void* p0) {
    ::GlobalFree(p0);
}
// Symbol: ?AfxGlobalsAddRef@@YAXXZ
extern "C" void MS_ABI impl__AfxGlobalsAddRef__YAXXZ() {}
// Symbol: ?AfxGlobalsRelease@@YAXXZ
extern "C" void MS_ABI impl__AfxGlobalsRelease__YAXXZ() {}
// Symbol: ?AfxHelpEnabled@@YAHXZ
extern "C" int MS_ABI impl__AfxHelpEnabled__YAHXZ() {
    return 0;
}
// Symbol: ?AfxHookWindowCreate@@YAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__AfxHookWindowCreate__YAXPEAVCWnd___Z(void* /*class*/* p0) {}
// Symbol: ?AfxHtmlHelp@@YAPEAUHWND__@@PEAU1@PEB_WI_K@Z
extern "C" void* MS_ABI impl__AfxHtmlHelp__YAPEAUHWND____PEAU1_PEB_WI_K_Z(void* /*struct*/* p0, const wchar_t* p1, unsigned int p2, unsigned __int64 p3) {
    return nullptr;
}
// Symbol: ?AfxInitCurrentStateApp@@YAHXZ
extern "C" int MS_ABI impl__AfxInitCurrentStateApp__YAHXZ() {
    return 0;
}
// Symbol: ?AfxInitExtensionModule@@YAHAEAUAFX_EXTENSION_MODULE@@PEAUHINSTANCE__@@@Z
extern "C" int MS_ABI impl__AfxInitExtensionModule__YAHAEAUAFX_EXTENSION_MODULE__PEAUHINSTANCE_____Z(void* /*struct*/* p0, void* /*struct*/* p1) {
    return 0;
}
// Symbol: ?AfxInitLocalData@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AfxInitLocalData__YAXPEAUHINSTANCE_____Z(void* /*struct*/* p0) {}
// Symbol: ?AfxInitNetworkAddressControl@@YAHXZ
extern "C" int MS_ABI impl__AfxInitNetworkAddressControl__YAHXZ() {
    return 0;
}
// Symbol: ?AfxInitRichEdit2@@YAHXZ
extern "C" int MS_ABI impl__AfxInitRichEdit2__YAHXZ() {
    return 0;
}
// Symbol: ?AfxInitRichEdit5@@YAHXZ
extern "C" int MS_ABI impl__AfxInitRichEdit5__YAHXZ() {
    return 0;
}
// Symbol: ?AfxInitRichEdit@@YAHXZ
extern "C" int MS_ABI impl__AfxInitRichEdit__YAHXZ() {
    return 0;
}
// Symbol: ?AfxInitThread@@YAXXZ
extern "C" void MS_ABI impl__AfxInitThread__YAXXZ() {}
// Symbol: ?AfxInternalIsIdleMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxInternalIsIdleMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?AfxInternalPreTranslateMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxInternalPreTranslateMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?AfxInternalProcessWndProcException@@YA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" __int64 MS_ABI impl__AfxInternalProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(void* /*class*/* p0, const void* /*struct*/* p1) {
    return 0;
}
// Symbol: ?AfxInternalPumpMessage@@YAHXZ
extern "C" int MS_ABI impl__AfxInternalPumpMessage__YAHXZ() {
    return 0;
}
// Symbol: ?AfxInternetStatusCallback@@YAXPEAX_KK0K@Z
extern "C" void MS_ABI impl__AfxInternetStatusCallback__YAXPEAX_KK0K_Z(void* p0, unsigned __int64 p1, unsigned long p2, void* p3, unsigned long p4) {}
// Symbol: ?AfxIsIdleMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxIsIdleMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?AfxIsMFCToolBar@@YAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__AfxIsMFCToolBar__YAHPEAVCWnd___Z(void* /*class*/* p0) {
    return 0;
}
// Symbol: ?AfxIsModuleDll@@YAHXZ
extern "C" int MS_ABI impl__AfxIsModuleDll__YAHXZ() {
    return 0;
}
// Symbol: ?AfxLoadField@@YAXAEAVCRecordset@@IPEAXPEA_J@Z
extern "C" void MS_ABI impl__AfxLoadField__YAXAEAVCRecordset__IPEAXPEA_J_Z(void* /*class*/* p0, unsigned int p1, void* p2, __int64* p3) {}
// Symbol: ?AfxLoadLangResourceDLL@@YAPEAUHINSTANCE__@@PEB_W0@Z
extern "C" void* MS_ABI impl__AfxLoadLangResourceDLL__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* p0, const wchar_t* p1) {
    return nullptr;
}
// Symbol: ?AfxLoadLangResourceDLL@@YAPEAUHINSTANCE__@@PEB_W@Z
extern "C" void* MS_ABI impl__AfxLoadLangResourceDLL__YAPEAUHINSTANCE____PEB_W_Z(const wchar_t* p0) {
    return nullptr;
}
// Symbol: ?AfxLoadString@@YAHIPEADI@Z
extern "C" int MS_ABI impl__AfxLoadString__YAHIPEADI_Z(unsigned int nID, char* lpszBuf, unsigned int nMaxBuf) {
    return ::LoadStringA(::GetModuleHandleW(nullptr), nID, lpszBuf, nMaxBuf);
}
// Symbol: ?AfxLoadString@@YAHIPEA_WI@Z
extern "C" int MS_ABI impl__AfxLoadString__YAHIPEA_WI_Z(unsigned int nID, wchar_t* lpszBuf, unsigned int nMaxBuf) {
    return ::LoadStringW(::GetModuleHandleW(nullptr), nID, lpszBuf, nMaxBuf);
}
// Symbol: ?AfxLoadSysColorBitmap@@YAPEAUHBITMAP__@@PEAUHINSTANCE__@@PEAUHRSRC__@@H@Z
extern "C" void* MS_ABI impl__AfxLoadSysColorBitmap__YAPEAUHBITMAP____PEAUHINSTANCE____PEAUHRSRC____H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return nullptr;
}
// Symbol: ?AfxLockGlobals@@YAXH@Z
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int p0) {}
// Symbol: ?AfxLockTempMaps@@YAXXZ
extern "C" void MS_ABI impl__AfxLockTempMaps__YAXXZ() {}
// Symbol: ?AfxMergeMenus@@YAPEAUHMENU__@@PEAU1@0PEAJHH@Z
extern "C" void* MS_ABI impl__AfxMergeMenus__YAPEAUHMENU____PEAU1_0PEAJHH_Z(void* /*struct*/* p0, void* /*struct*/* p1, long* p2, int p3, int p4) {
    return nullptr;
}
// Symbol: ?AfxOleCanExitApp@@YAHXZ
extern "C" int MS_ABI impl__AfxOleCanExitApp__YAHXZ() {
    return 0;
}
// Symbol: ?AfxOleGetUserCtrl@@YAHXZ
extern "C" int MS_ABI impl__AfxOleGetUserCtrl__YAHXZ() {
    return 0;
}
// Symbol: ?AfxOleInit@@YAHXZ
extern "C" int MS_ABI impl__AfxOleInit__YAHXZ() {
    return SUCCEEDED(::OleInitialize(nullptr));
}
// Symbol: ?AfxOleInprocRegisterHelper@@YAHPEAUHKEY__@@0H@Z
extern "C" int MS_ABI impl__AfxOleInprocRegisterHelper__YAHPEAUHKEY____0H_Z(void* /*struct*/* p0, void* /*struct*/* p1, int p2) {
    return 0;
}
// Symbol: ?AfxOleLockApp@@YAXXZ
extern "C" void MS_ABI impl__AfxOleLockApp__YAXXZ() {}
// Symbol: ?AfxOleLockControl@@YAHAEBU_GUID@@@Z
extern "C" int MS_ABI impl__AfxOleLockControl__YAHAEBU_GUID___Z(const void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?AfxOleLockControl@@YAHPEB_W@Z
extern "C" int MS_ABI impl__AfxOleLockControl__YAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}
// Symbol: ?AfxOleOnReleaseAllObjects@@YAXXZ
extern "C" void MS_ABI impl__AfxOleOnReleaseAllObjects__YAXXZ() {}
// Symbol: ?AfxOleRegisterControlClass@@YAHPEAUHINSTANCE__@@AEBU_GUID@@PEB_WIIHK1GG@Z
extern "C" int MS_ABI impl__AfxOleRegisterControlClass__YAHPEAUHINSTANCE____AEBU_GUID__PEB_WIIHK1GG_Z(void* /*struct*/* p0, const void* /*struct*/* p1, const wchar_t* p2, unsigned int p3, unsigned int p4, int p5, unsigned long p6, const void* /*struct*/* p7, unsigned short p8, unsigned short p9) {
    return 0;
}
// Symbol: ?AfxOleRegisterHelper@@YAHPEBQEB_W0HHPEAUHKEY__@@@Z
extern "C" int MS_ABI impl__AfxOleRegisterHelper__YAHPEBQEB_W0HHPEAUHKEY_____Z(const void** p0, unsigned char p1, void* p2, wchar_t p3, const void** p4, int p5, int p6, void* /*struct*/* p7) {
    return 0;
}
// Symbol: ?AfxOleRegisterPropertyPageClass@@YAHPEAUHINSTANCE__@@AEBU_GUID@@I@Z
extern "C" int MS_ABI impl__AfxOleRegisterPropertyPageClass__YAHPEAUHINSTANCE____AEBU_GUID__I_Z(void* /*struct*/* p0, const void* /*struct*/* p1, unsigned int p2) {
    return 0;
}
// Symbol: ?AfxOleRegisterPropertyPageClass@@YAHPEAUHINSTANCE__@@AEBU_GUID@@IH@Z
extern "C" int MS_ABI impl__AfxOleRegisterPropertyPageClass__YAHPEAUHINSTANCE____AEBU_GUID__IH_Z(void* /*struct*/* p0, const void* /*struct*/* p1, unsigned int p2, int p3) {
    return 0;
}
// Symbol: ?AfxOleRegisterServerClass@@YAHAEBU_GUID@@PEB_W11W4OLE_APPTYPE@@PEAPEB_W3H11@Z
extern "C" int MS_ABI impl__AfxOleRegisterServerClass__YAHAEBU_GUID__PEB_W11W4OLE_APPTYPE__PEAPEB_W3H11_Z(const void* /*struct*/* p0, const wchar_t* p1, const wchar_t* p2, const wchar_t* p3, int /*enum*/ p4, void* p5, void* p6, unsigned char p7, int p8, void**** p9, void* p10, unsigned char* p11, const wchar_t** p12, void* p13, int p14, const wchar_t* p15, const wchar_t* p16) {
    return 0;
}
// Symbol: ?AfxOleRegisterServerClass@@YAHAEBU_GUID@@PEB_W11W4OLE_APPTYPE@@PEAPEB_W3H1@Z
extern "C" int MS_ABI impl__AfxOleRegisterServerClass__YAHAEBU_GUID__PEB_W11W4OLE_APPTYPE__PEAPEB_W3H1_Z(const void* /*struct*/* p0, const wchar_t* p1, const wchar_t* p2, const wchar_t* p3, int /*enum*/ p4, void* p5, void* p6, unsigned char p7, int p8, void**** p9, void* p10, unsigned char* p11, const wchar_t** p12, void* p13, int p14, const wchar_t* p15) {
    return 0;
}
// Symbol: ?AfxOleRegisterTypeLib@@YAHPEAUHINSTANCE__@@AEBU_GUID@@PEB_W2@Z
extern "C" int MS_ABI impl__AfxOleRegisterTypeLib__YAHPEAUHINSTANCE____AEBU_GUID__PEB_W2_Z(void* /*struct*/* p0, const void* /*struct*/* p1, const wchar_t* p2, const wchar_t* p3) {
    return 0;
}
// Symbol: ?AfxOleSetEditMenu@@YAXPEAVCOleClientItem@@PEAVCMenu@@IIII@Z
extern "C" void MS_ABI impl__AfxOleSetEditMenu__YAXPEAVCOleClientItem__PEAVCMenu__IIII_Z(void* /*class*/* p0, void* /*class*/* p1, unsigned int p2, unsigned int p3, unsigned int p4, unsigned int p5) {}
// Symbol: ?AfxOleSetUserCtrl@@YAXH@Z
extern "C" void MS_ABI impl__AfxOleSetUserCtrl__YAXH_Z(int p0) {}
// Symbol: ?AfxOleTerm@@YAXH@Z
extern "C" void MS_ABI impl__AfxOleTerm__YAXH_Z(int p0) {
    (void)p0;
    ::OleUninitialize();
}
// Symbol: ?AfxOleTermOrFreeLib@@YAXHH@Z
extern "C" void MS_ABI impl__AfxOleTermOrFreeLib__YAXHH_Z(int p0, int p1) {
    (void)p0;
    (void)p1;
    ::OleUninitialize();
}
// Symbol: ?AfxOleUnlockAllControls@@YAXXZ
extern "C" void MS_ABI impl__AfxOleUnlockAllControls__YAXXZ() {}
// Symbol: ?AfxOleUnlockApp@@YAXXZ
extern "C" void MS_ABI impl__AfxOleUnlockApp__YAXXZ() {}
// Symbol: ?AfxOleUnlockControl@@YAHAEBU_GUID@@@Z
extern "C" int MS_ABI impl__AfxOleUnlockControl__YAHAEBU_GUID___Z(const void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?AfxOleUnlockControl@@YAHPEB_W@Z
extern "C" int MS_ABI impl__AfxOleUnlockControl__YAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}
// Symbol: ?AfxOleUnregisterClass@@YAHAEBU_GUID@@PEB_W@Z
extern "C" int MS_ABI impl__AfxOleUnregisterClass__YAHAEBU_GUID__PEB_W_Z(const void* /*struct*/* p0, const wchar_t* p1) {
    return 0;
}
// Symbol: ?AfxOleUnregisterHelper@@YAHPEBQEB_W0HPEAUHKEY__@@@Z
extern "C" int MS_ABI impl__AfxOleUnregisterHelper__YAHPEBQEB_W0HPEAUHKEY_____Z(const void** p0, unsigned char p1, void* p2, wchar_t p3, const void** p4, int p5, void* /*struct*/* p6) {
    return 0;
}
// Symbol: ?AfxOleUnregisterServerClass@@YAHAEBU_GUID@@PEB_W11W4OLE_APPTYPE@@PEAPEB_W3@Z
extern "C" int MS_ABI impl__AfxOleUnregisterServerClass__YAHAEBU_GUID__PEB_W11W4OLE_APPTYPE__PEAPEB_W3_Z(const void* /*struct*/* p0, const wchar_t* p1, const wchar_t* p2, const wchar_t* p3, int /*enum*/ p4, void* p5, void* p6, unsigned char p7, int p8, void**** p9, void* p10, unsigned char* p11, const wchar_t** p12, void* p13) {
    return 0;
}
// Symbol: ?AfxOleUnregisterTypeLib@@YAHAEBU_GUID@@GGK@Z
extern "C" int MS_ABI impl__AfxOleUnregisterTypeLib__YAHAEBU_GUID__GGK_Z(const void* /*struct*/* p0, unsigned short p1, unsigned short p2, unsigned long p3) {
    return 0;
}
// Symbol: ?AfxParseURL@@YAHPEB_WAEAKAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2AEAG@Z
extern "C" int MS_ABI impl__AfxParseURL__YAHPEB_WAEAKAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2AEAG_Z(const wchar_t* p0, unsigned long* p1, void* /*class*/* p2, void** p3, void* p4, void* /*class*/* p5, unsigned short* p6) {
    return 0;
}
// Symbol: ?AfxParseURLEx@@YAHPEB_WAEAKAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@2AEAG22K@Z
extern "C" int MS_ABI impl__AfxParseURLEx__YAHPEB_WAEAKAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__2AEAG22K_Z(const wchar_t* p0, unsigned long* p1, void* /*class*/* p2, void** p3, void* p4, void* /*class*/* p5, unsigned short* p6, void* /*class*/* p7, void* /*class*/* p8, unsigned long p9) {
    return 0;
}
// Symbol: ?AfxPreTranslateMessage@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__AfxPreTranslateMessage__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?AfxProcessWndProcException@@YA_JPEAVCException@@PEBUtagMSG@@@Z
extern "C" __int64 MS_ABI impl__AfxProcessWndProcException__YA_JPEAVCException__PEBUtagMSG___Z(void* /*class*/* p0, const void* /*struct*/* p1) {
    return 0;
}
// Symbol: ?AfxPropPageCallback@@YAIPEAUHWND__@@IPEAU_PROPSHEETPAGEW@@@Z
extern "C" unsigned int MS_ABI impl__AfxPropPageCallback__YAIPEAUHWND____IPEAU_PROPSHEETPAGEW___Z(void* /*struct*/* p0, unsigned int p1, void* /*struct*/* p2) {
    return 0;
}
// Symbol: ?AfxPropSheetCallback@@YAHPEAUHWND__@@I_J@Z
extern "C" int MS_ABI impl__AfxPropSheetCallback__YAHPEAUHWND____I_J_Z(void* /*struct*/* p0, unsigned int p1, __int64 p2) {
    return 0;
}
// Symbol: ?AfxPumpMessage@@YAHXZ
extern "C" int MS_ABI impl__AfxPumpMessage__YAHXZ() {
    return 0;
}
// Symbol: ?AfxRegCreateKey@@YAJPEAUHKEY__@@PEB_WPEAPEAU1@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegCreateKey__YAJPEAUHKEY____PEB_WPEAPEAU1_PEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, HKEY* phkResult, void* pTransaction) {
    (void)pTransaction;
    return ::RegCreateKeyExW(hKey, lpszSubKey, 0, nullptr, 0, KEY_ALL_ACCESS, nullptr, phkResult, nullptr);
}
// Symbol: ?AfxRegDeleteKey@@YAJPEAUHKEY__@@PEB_WPEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegDeleteKey__YAJPEAUHKEY____PEB_WPEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, void* pTransaction) {
    (void)pTransaction;
    return ::RegDeleteKeyW(hKey, lpszSubKey);
}
// Symbol: ?AfxRegOpenKey@@YAJPEAUHKEY__@@PEB_WPEAPEAU1@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegOpenKey__YAJPEAUHKEY____PEB_WPEAPEAU1_PEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, HKEY* phkResult, void* pTransaction) {
    (void)pTransaction;
    return ::RegOpenKeyExW(hKey, lpszSubKey, 0, KEY_ALL_ACCESS, phkResult);
}
// Symbol: ?AfxRegOpenKeyEx@@YAJPEAUHKEY__@@PEB_WKKPEAPEAU1@PEAVCAtlTransactionManager@ATL@@@Z
extern "C" long MS_ABI impl__AfxRegOpenKeyEx__YAJPEAUHKEY____PEB_WKKPEAPEAU1_PEAVCAtlTransactionManager_ATL___Z(
    HKEY hKey, const wchar_t* lpszSubKey, unsigned long ulOptions, unsigned long samDesired,
    HKEY* phkResult, void* pTransaction) {
    (void)pTransaction;
    return ::RegOpenKeyExW(hKey, lpszSubKey, (DWORD)ulOptions, (REGSAM)samDesired, phkResult);
}
// Symbol: ?AfxRegQueryValue@@YAJPEAUHKEY__@@PEB_WPEA_WPEAJ@Z
extern "C" long MS_ABI impl__AfxRegQueryValue__YAJPEAUHKEY____PEB_WPEA_WPEAJ_Z(
    HKEY hKey, const wchar_t* lpszSubKey, wchar_t* lpszValue, long* plValueLength) {
    return ::RegQueryValueW(hKey, lpszSubKey, lpszValue, plValueLength);
}
// Symbol: ?AfxRegSetValue@@YAJPEAUHKEY__@@PEB_WK1K@Z
extern "C" long MS_ABI impl__AfxRegSetValue__YAJPEAUHKEY____PEB_WK1K_Z(
    HKEY hKey, const wchar_t* lpszSubKey, unsigned long dwType, const wchar_t* lpszValue, unsigned long cbValue) {
    return ::RegSetValueExW(hKey, lpszSubKey, 0, (DWORD)dwType, (const BYTE*)lpszValue, (DWORD)cbValue);
}
// Symbol: ?AfxRegisterMFCCtrlClasses@@YAXXZ
extern "C" void MS_ABI impl__AfxRegisterMFCCtrlClasses__YAXXZ() {}
// Symbol: ?AfxRegisterPreviewHandler@@YAHPEB_W00@Z
extern "C" int MS_ABI impl__AfxRegisterPreviewHandler__YAHPEB_W00_Z(const wchar_t* p0, const wchar_t* p1, const wchar_t* p2) {
    return 0;
}
// Symbol: ?AfxRegisterSiteFactory@@YAHPEAVIControlSiteFactory@@@Z
extern "C" int MS_ABI impl__AfxRegisterSiteFactory__YAHPEAVIControlSiteFactory___Z(void* /*class*/* p0) {
    return 0;
}
// Symbol: ?AfxRegisterThumbnailHandler@@YAHPEB_W0K@Z
extern "C" int MS_ABI impl__AfxRegisterThumbnailHandler__YAHPEB_W0K_Z(const wchar_t* p0, const wchar_t* p1, unsigned long p2) {
    return 0;
}
// Symbol: ?AfxReleaseD2DRefs@@YAXXZ
extern "C" void MS_ABI impl__AfxReleaseD2DRefs__YAXXZ() {}
// Symbol: ?AfxRepositionWindow@@YAXPEAUAFX_SIZEPARENTPARAMS@@PEAUHWND__@@PEBUtagRECT@@@Z
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(void* /*struct*/* p0, void* /*struct*/* p1, const void* /*struct*/* p2) {}
// Symbol: ?AfxResetMsgCache@@YAXXZ
extern "C" void MS_ABI impl__AfxResetMsgCache__YAXXZ() {}
// Symbol: ?AfxResolveShortcut@@YAHPEAVCWnd@@PEB_WPEA_WH@Z
extern "C" int MS_ABI impl__AfxResolveShortcut__YAHPEAVCWnd__PEB_WPEA_WH_Z(void* /*class*/* p0, const wchar_t* p1, wchar_t* p2, int p3) {
    return 0;
}
// Symbol: ?AfxSafeArrayInit@@YAXPEAVCOleSafeArray@@@Z
extern "C" void MS_ABI impl__AfxSafeArrayInit__YAXPEAVCOleSafeArray___Z(void* /*class*/* p0) {}
// Symbol: ?AfxSetNewHandler@@YAP6AH_K@ZP6AH0@Z@Z
extern "C" void* MS_ABI impl__AfxSetNewHandler__YAP6AH_K_ZP6AH0_Z_Z(void* /*fnptr*/ p0) {
    return nullptr;
}
// Symbol: ?AfxSetPerUserRegistration@@YAXH@Z
extern "C" void MS_ABI impl__AfxSetPerUserRegistration__YAXH_Z(int p0) {
    g_bPerUserRegistration = p0;
}
// Symbol: ?AfxStoreField@@YAXAEAVCRecordset@@IPEAX@Z
extern "C" void MS_ABI impl__AfxStoreField__YAXAEAVCRecordset__IPEAX_Z(void* /*class*/* p0, unsigned int p1, void* p2) {}
// Symbol: ?AfxStringFromCLSID@@YA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBU_GUID@@@Z
extern "C" void* MS_ABI impl__AfxStringFromCLSID__YA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBU_GUID___Z(void* /*class*/* p0, void** p1, void* p2, const void* /*struct*/* p3) {
    return nullptr;
}
// Symbol: ?AfxTermExtensionModule@@YAXAEAUAFX_EXTENSION_MODULE@@H@Z
extern "C" void MS_ABI impl__AfxTermExtensionModule__YAXAEAUAFX_EXTENSION_MODULE__H_Z(void* /*struct*/* p0, int p1) {}
// Symbol: ?AfxTermLocalData@@YAXPEAUHINSTANCE__@@H@Z
extern "C" void MS_ABI impl__AfxTermLocalData__YAXPEAUHINSTANCE____H_Z(void* /*struct*/* p0, int p1) {}
// Symbol: ?AfxTermThread@@YAXPEAUHINSTANCE__@@@Z
extern "C" void MS_ABI impl__AfxTermThread__YAXPEAUHINSTANCE_____Z(void* /*struct*/* p0) {}
// Symbol: ?AfxTimeToFileTime@@YAXAEBVCTime@ATL@@PEAU_FILETIME@@@Z
extern "C" void MS_ABI impl__AfxTimeToFileTime__YAXAEBVCTime_ATL__PEAU_FILETIME___Z(const void* /*class*/* p0, void* /*struct*/* p1) {}
// Symbol: ?AfxTlsAddRef@@YAXXZ
extern "C" void MS_ABI impl__AfxTlsAddRef__YAXXZ() {}
// Symbol: ?AfxTlsRelease@@YAXXZ
extern "C" void MS_ABI impl__AfxTlsRelease__YAXXZ() {}
// Symbol: ?AfxTrackerTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxTrackerTerm__YAXXZ() {}
// Symbol: ?AfxTryCleanup@@YAXXZ
extern "C" void MS_ABI impl__AfxTryCleanup__YAXXZ() {}
// Symbol: ?AfxUnRegisterPreviewHandler@@YAHPEB_W@Z
extern "C" int MS_ABI impl__AfxUnRegisterPreviewHandler__YAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}
// Symbol: ?AfxUnhookWindowCreate@@YAHXZ
extern "C" int MS_ABI impl__AfxUnhookWindowCreate__YAHXZ() {
    return 0;
}
// Symbol: ?AfxUnlockGlobals@@YAXH@Z
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int p0) {}
// Symbol: ?AfxUnlockTempMaps@@YAHH@Z
extern "C" int MS_ABI impl__AfxUnlockTempMaps__YAHH_Z(int p0) {
    return 0;
}
// Symbol: ?AfxUnmergeMenus@@YAXPEAUHMENU__@@00@Z
extern "C" void MS_ABI impl__AfxUnmergeMenus__YAXPEAUHMENU____00_Z(void* /*struct*/* p0, void* /*struct*/* p1, void* /*struct*/* p2) {}
// Symbol: ?AfxUnregisterSiteFactory@@YAHPEAVIControlSiteFactory@@@Z
extern "C" int MS_ABI impl__AfxUnregisterSiteFactory__YAHPEAVIControlSiteFactory___Z(void* /*class*/* p0) {
    return 0;
}
// Symbol: ?AfxUnregisterWndClasses@@YAXXZ
extern "C" void MS_ABI impl__AfxUnregisterWndClasses__YAXXZ() {}
// Symbol: ?AfxVerifyLicFile@@YAHPEAUHINSTANCE__@@PEB_W1I@Z
extern "C" int MS_ABI impl__AfxVerifyLicFile__YAHPEAUHINSTANCE____PEB_W1I_Z(void* /*struct*/* p0, const wchar_t* p1, const wchar_t* p2, unsigned int p3) {
    return 0;
}
// Symbol: ?AfxWinTerm@@YAXXZ
extern "C" void MS_ABI impl__AfxWinTerm__YAXXZ() {}
// Symbol: ?AfxWndProc@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxWndProc__YA_JPEAUHWND____I_K_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return 0;
}
// Symbol: ?AfxWndProcDllOle@@YA_JPEAUHWND__@@I_K_J@Z
extern "C" __int64 MS_ABI impl__AfxWndProcDllOle__YA_JPEAUHWND____I_K_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3) {
    return 0;
}
// Symbol: ?ChWindowFromPoint@@YAPEAUHWND__@@PEAU1@UtagPOINT@@@Z
extern "C" void* MS_ABI impl__ChWindowFromPoint__YAPEAUHWND____PEAU1_UtagPOINT___Z(void* /*struct*/* p0) {
    return nullptr;
}
// Symbol: ?ControlBarCleanUp@@YAXXZ
extern "C" void MS_ABI impl__ControlBarCleanUp__YAXXZ() {}
// Symbol: ?GetCmdMgr@@YAPEAVCCommandManager@@XZ
extern "C" void* MS_ABI impl__GetCmdMgr__YAPEAVCCommandManager__XZ() {
    return nullptr;
}
// Symbol: ?GetCommCtrlVersion@@YAJPEAK0@Z
extern "C" long MS_ABI impl__GetCommCtrlVersion__YAJPEAK0_Z(unsigned long* p0, unsigned long* p1) {
    return 0;
}
// Symbol: ?GetDHtmlEventMap@@YAPEBUDHtmlEventMapEntry@@XZ
extern "C" void* MS_ABI impl__GetDHtmlEventMap__YAPEBUDHtmlEventMapEntry__XZ() {
    return nullptr;
}
// Symbol: ?IsHelpKey@@YAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsHelpKey__YAHPEAUtagMSG___Z(void* /*struct*/* p0) {
    return 0;
}
// Symbol: ?ReadBoolProp@@YAHAEAVCTagManager@@PEB_WAEAH@Z
extern "C" int MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(void* /*class*/* p0, const wchar_t* p1, int* p2) {
    return 0;
}
// Symbol: ?TaskDialogCallback@@YAJPEAUHWND__@@I_K_J2@Z
extern "C" long MS_ABI impl__TaskDialogCallback__YAJPEAUHWND____I_K_J2_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3, unsigned __int64 p4) {
    return 0;
}
// Symbol: ?_AfxDeleteRegKey@@YAHPEB_W@Z
extern "C" int MS_ABI impl___AfxDeleteRegKey__YAHPEB_W_Z(const wchar_t* p0) {
    return 0;
}
// Symbol: ?_AfxFindSiteOrWnd@@YAPEAUCOleControlSiteOrWnd@@PEAVCWnd@@0@Z
extern "C" void* MS_ABI impl___AfxFindSiteOrWnd__YAPEAUCOleControlSiteOrWnd__PEAVCWnd__0_Z(void* /*class*/* p0, void* /*class*/* p1) {
    return nullptr;
}
// Symbol: ?afxData@@3UAUX_DATA@@A
extern "C" unsigned int MS_ABI impl__afxData__3UAUX_DATA__A = {};
// Symbol: ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A
extern "C" unsigned int MS_ABI impl__afxGlobalData__3UAFX_GLOBAL_DATA__A = {};
// Symbol: ?afxGlobalUtils@@3VCGlobalUtils@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__afxGlobalUtils__3VCGlobalUtils__A = {};
// Symbol: ?afxKeyboardManager@@3PEAVCKeyboardManager@@EA
extern "C" void* MS_ABI impl__afxKeyboardManager__3PEAVCKeyboardManager__EA = {};
// Symbol: ?afxMenuHash@@3VCMenuHash@@A
extern "C" void* /*CRuntimeClass*/ MS_ABI impl__afxMenuHash__3VCMenuHash__A = {};
// Symbol: ?afxShellManager@@3PEAVCShellManager@@EA
extern "C" void* MS_ABI impl__afxShellManager__3PEAVCShellManager__EA = {};
// Symbol: ?afxUserToolsManager@@3PEAVCUserToolsManager@@EA
extern "C" void* MS_ABI impl__afxUserToolsManager__3PEAVCUserToolsManager__EA = {};
// Symbol: ?g_pTopLevelFrame@@3PEAVCFrameWnd@@EA
extern "C" void* MS_ABI impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA = {};
