// CStringT — OpenMFC implementation.
// Sources: global_cstring_core.cpp, global_string_buffer_format.cpp, global_string_compare_afx.cpp, global_string_destructors.cpp, strcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CStringTSupport.h"
#include "detail/StrcoreSupport.h"

// OpenMFC: core exported CStringT<wchar_t, StrTraitMFC_DLL> / CSimpleStringT methods.
//
// A drop-in MSVC client does NOT inline these — MFC exports the explicit template
// instantiations and the client imports them BY ORDINAL (e.g. CString default ctor =
// ord 296, operator=(PCXSTR)=1503, GetLength=5674, operator PCXSTR=1641). They were
// weak stubs, so a client `CString d;` ran a no-op ctor -> garbage m_pszData -> reads
// empty / NULL. OpenMFC's CString is now ATL-binary-compatible (same 8-byte object,
// m_pszData@0, ATL CStringData layout — see afxstr.h), so these thunks just forward
// to it: the client's CStringT object IS an OpenMFC CString.

#include "openmfc/afxstr.h"
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif









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




// ---------------------------------------------------------------------------
// CStringT<wchar_t> comparison helpers
//
// The ABI 'this' pointer is the exported CStringT<wchar_t,...> object, whose
// memory layout (a single m_pszData pointer with a CStringData header stored
// before the character buffer) is faithfully modelled by openmfc's CString.
// ---------------------------------------------------------------------------




#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ?Delete@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHHH@Z
extern "C" int MS_ABI impl__Delete___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHHH_Z(CString* pThis, int iIndex, int nCount) {
    if (!pThis) return 0;
    return pThis->Delete(iIndex, nCount);
}
// Symbol: ?Replace@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAH_W0@Z
extern "C" int MS_ABI
impl__Replace___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAH_W0_Z(
        CString* pThis, wchar_t chOld, wchar_t chNew) {
    return pThis ? pThis->Replace(chOld, chNew) : 0;
}
// Symbol: ?Replace@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEB_W0@Z
extern "C" int MS_ABI
impl__Replace___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEB_W0_Z(
        CString* pThis, const wchar_t* pszOld, const wchar_t* pszNew) {
    return pThis ? pThis->Replace(pszOld, pszNew) : 0;
}
// Symbol: ?Trim@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@_W@Z
extern "C" CString* MS_ABI
impl__Trim___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12__W_Z(
        CString* pThis, wchar_t chTarget) {
    wchar_t trimSet[2] = { chTarget, L'\0' };
    TrimRightWithSet(pThis, trimSet);
    return TrimLeftWithSet(pThis, trimSet);
}
// Symbol: ?Trim@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@PEB_W@Z
extern "C" CString* MS_ABI
impl__Trim___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_PEB_W_Z(
        CString* pThis, const wchar_t* pszTargets) {
    TrimRightWithSet(pThis, pszTargets);
    return TrimLeftWithSet(pThis, pszTargets);
}
// Symbol: ?Trim@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* MS_ABI
impl__Trim___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis) {
    if (pThis) pThis->Trim();
    return pThis;
}
// Symbol: ?TrimLeft@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@_W@Z
extern "C" CString* MS_ABI
impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12__W_Z(
        CString* pThis, wchar_t chTarget) {
    wchar_t trimSet[2] = { chTarget, L'\0' };
    return TrimLeftWithSet(pThis, trimSet);
}
// Symbol: ?TrimLeft@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@PEB_W@Z
extern "C" CString* MS_ABI
impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_PEB_W_Z(
        CString* pThis, const wchar_t* pszTargets) {
    return TrimLeftWithSet(pThis, pszTargets);
}
// Symbol: ?TrimLeft@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* MS_ABI
impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis) {
    if (pThis) pThis->TrimLeft();
    return pThis;
}
// Symbol: ?TrimRight@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@_W@Z
extern "C" CString* MS_ABI
impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12__W_Z(
        CString* pThis, wchar_t chTarget) {
    wchar_t trimSet[2] = { chTarget, L'\0' };
    return TrimRightWithSet(pThis, trimSet);
}
// Symbol: ?TrimRight@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@PEB_W@Z
extern "C" CString* MS_ABI
impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_PEB_W_Z(
        CString* pThis, const wchar_t* pszTargets) {
    return TrimRightWithSet(pThis, pszTargets);
}
// Symbol: ?TrimRight@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* MS_ABI
impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis) {
    if (pThis) pThis->TrimRight();
    return pThis;
}
// int CStringT<wchar_t>::Compare(PCXSTR) const
// int CStringT<wchar_t>::CompareNoCase(PCXSTR) const
// int CStringT<wchar_t>::Collate(PCXSTR) const
// MFC implements Collate via _tcscoll (locale-aware comparison).
// int CStringT<wchar_t>::CollateNoCase(PCXSTR) const
// MFC implements CollateNoCase via _tcsicoll (locale-aware, case-insensitive).
