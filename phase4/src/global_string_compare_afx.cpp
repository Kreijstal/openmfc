#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <windows.h>
#include <oleauto.h>

#include <cwchar>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Global Win32 forwards
// ---------------------------------------------------------------------------

// AfxPostQuitMessage(int nExitCode): thin forward to ::PostQuitMessage.
// Symbol: ?AfxPostQuitMessage@@YAXH@Z
extern "C" void MS_ABI impl__AfxPostQuitMessage__YAXH_Z(int nExitCode) {
    ::PostQuitMessage(nExitCode);
}

// AfxSetWindowText(HWND, LPCWSTR): thin forward to ::SetWindowTextW.
// Symbol: ?AfxSetWindowText@@YAXPEAUHWND__@@PEB_W@Z
extern "C" void MS_ABI impl__AfxSetWindowText__YAXPEAUHWND____PEB_W_Z(
        HWND hWnd, const wchar_t* lpszText) {
    if (hWnd == nullptr) {
        return;
    }
    ::SetWindowTextW(hWnd, lpszText ? lpszText : L"");
}

// AfxVariantInit(VARIANT*): clear a VARIANT to VT_EMPTY. Thin forward to
// ::VariantInit (which simply zeroes the discriminant fields).
// Symbol: ?AfxVariantInit@@YAXPEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__AfxVariantInit__YAXPEAUtagVARIANT___Z(VARIANTARG* pVar) {
    if (pVar == nullptr) {
        return;
    }
    ::VariantInit(pVar);
}

// ---------------------------------------------------------------------------
// CStringT<wchar_t> comparison helpers
//
// The ABI 'this' pointer is the exported CStringT<wchar_t,...> object, whose
// memory layout (a single m_pszData pointer with a CStringData header stored
// before the character buffer) is faithfully modelled by openmfc's CString.
// ---------------------------------------------------------------------------

// int CStringT<wchar_t>::Compare(PCXSTR) const
// Symbol: ?Compare@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int MS_ABI
impl__Compare___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(
        const CString* pThis, const wchar_t* psz) {
    if (pThis == nullptr) {
        return 0;
    }
    return ::wcscmp(pThis->GetString(), psz ? psz : L"");
}

// int CStringT<wchar_t>::CompareNoCase(PCXSTR) const
// Symbol: ?CompareNoCase@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int MS_ABI
impl__CompareNoCase___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(
        const CString* pThis, const wchar_t* psz) {
    if (pThis == nullptr) {
        return 0;
    }
    return pThis->CompareNoCase(psz);
}

// int CStringT<wchar_t>::Collate(PCXSTR) const
// MFC implements Collate via _tcscoll (locale-aware comparison).
// Symbol: ?Collate@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int MS_ABI
impl__Collate___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(
        const CString* pThis, const wchar_t* psz) {
    if (pThis == nullptr) {
        return 0;
    }
    return ::wcscoll(pThis->GetString(), psz ? psz : L"");
}

// int CStringT<wchar_t>::CollateNoCase(PCXSTR) const
// MFC implements CollateNoCase via _tcsicoll (locale-aware, case-insensitive).
// Symbol: ?CollateNoCase@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int MS_ABI
impl__CollateNoCase___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(
        const CString* pThis, const wchar_t* psz) {
    if (pThis == nullptr) {
        return 0;
    }
    return ::_wcsicoll(pThis->GetString(), psz ? psz : L"");
}
