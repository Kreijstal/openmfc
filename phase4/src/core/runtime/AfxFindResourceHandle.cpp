#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

// AfxFindResourceHandle - Find which module contains a resource
// Symbol: ?AfxFindResourceHandle@@YAPEAUHINSTANCE__@@PEB_W0@Z
// Ordinal: 2187
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType) {
    // First, try the resource handle
    HINSTANCE hRes = AfxGetResourceHandle();
    if (hRes && lpszResource && lpszType) {
        if (::FindResourceW(hRes, lpszResource, lpszType)) {
            return hRes;
        }
    }
    // Fall back to instance handle
    HINSTANCE hInst = AfxGetInstanceHandle();
    if (hInst && lpszResource && lpszType) {
        if (::FindResourceW(hInst, lpszResource, lpszType)) {
            return hInst;
        }
    }
    // If still not found, check the main module
    HINSTANCE hMain = ::GetModuleHandleW(nullptr);
    if (hMain && hMain != hInst && hMain != hRes && lpszResource && lpszType) {
        if (::FindResourceW(hMain, lpszResource, lpszType)) {
            return hMain;
        }
    }
    return nullptr;
}
