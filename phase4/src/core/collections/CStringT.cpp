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

// Wide (Unicode) CStringT / CSimpleStringT destructors.
//
// In the MFC headers a Unicode CStringW is the concrete instantiation
//   CStringT<wchar_t, StrTraitMFC_DLL<wchar_t, ChTraitsCRT<wchar_t>>>
// whose base is CSimpleStringT<wchar_t, true>.  Both have an identical ABI
// layout to the repo's faithful CString: a single m_pszData pointer to a
// reference-counted, header-prefixed character buffer.  The destructor simply
// releases that buffer, which is exactly what CString::~CString() does.
//
// Only the wide variants are implemented here; the narrow (char) variants have
// no faithful CStringA equivalent in the repo, so their weak stubs are left.


// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(void* pThis) {
    new (pThis) CString();
    return pThis;
}
// Symbol: ??0?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@PEB_W@Z
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_PEB_W_Z(void* pThis, const wchar_t* psz) {
    new (pThis) CString(psz);
    return pThis;
}
// Symbol: ??1?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" void MS_ABI impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(void* pThis) {
    static_cast<CString*>(pThis)->~CString();
}
// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(void* pThis, const wchar_t* psz) {
    *static_cast<CString*>(pThis) = psz;
    return pThis;
}
// Symbol: ?Delete@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHHH@Z
extern "C" int MS_ABI impl__Delete___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHHH_Z(CString* pThis, int iIndex, int nCount) {
    if (!pThis) return 0;
    return pThis->Delete(iIndex, nCount);
}
// Symbol: ?Find@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAH_WH@Z
extern "C" int MS_ABI impl__Find___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAH_WH_Z(const CString* pThis, wchar_t ch, int iStart) {
    if (!pThis) return -1;
    return pThis->Find(ch, iStart);
}
// Symbol: ?Find@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_WH@Z
extern "C" int MS_ABI impl__Find___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_WH_Z(const CString* pThis, const wchar_t* pszSub, int iStart) {
    if (!pThis) return -1;
    return pThis->Find(pszSub, iStart);
}
// Symbol: ?FindOneOf@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEBAHPEB_W@Z
extern "C" int MS_ABI impl__FindOneOf___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEBAHPEB_W_Z(const CString* pThis, const wchar_t* pszCharSet) {
    if (!pThis) return -1;
    return pThis->FindOneOf(pszCharSet);
}
// Symbol: ?FormatV@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WPEAD@Z
extern "C" void MS_ABI impl__FormatV___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WPEAD_Z(CString* pThis, const wchar_t* pszFormat, char* args) {
    if (!pThis) return;
    pThis->FormatV(pszFormat, reinterpret_cast<va_list>(args));
}
// Symbol: ?Format@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAXPEB_WZZ
extern "C" void MS_ABI impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(CString* pThis, const wchar_t* pszFormat, ...) {
    if (!pThis) return;
    va_list args;
    va_start(args, pszFormat);
    pThis->FormatV(pszFormat, args);
    va_end(args);
}
// Symbol: ?MakeLower@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* MS_ABI
impl__MakeLower___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis) {
    if (pThis) pThis->MakeLower();
    return pThis;
}
// Symbol: ?MakeReverse@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* MS_ABI
impl__MakeReverse___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis) {
    if (pThis) pThis->MakeReverse();
    return pThis;
}
// Symbol: ?MakeUpper@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV12@XZ
extern "C" CString* MS_ABI
impl__MakeUpper___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis) {
    if (pThis) pThis->MakeUpper();
    return pThis;
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
// Implemented in global_cstring_core.cpp; this unit had a second, unlinked copy.
// Symbol: ??1?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
// CString::LoadString(UINT nID)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHI@Z
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHI_Z(CString* pThis, UINT nID)
{
    return CStringLoadStringImpl(pThis, GetResourceHandle(), nID);
}
// CString::LoadString(HINSTANCE, UINT)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@I@Z
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID)
{
    return CStringLoadStringImpl(pThis, hInst, nID);
}
// CString::LoadString(HINSTANCE, UINT, LANGID)
// Symbol: ?LoadStringW@?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHPEAUHINSTANCE__@@IG@Z
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____IG_Z(
    CString* pThis, HINSTANCE hInst, UINT nID, unsigned short /*wLanguageID*/)
{
    return CStringLoadStringImpl(pThis, hInst, nID);
}
