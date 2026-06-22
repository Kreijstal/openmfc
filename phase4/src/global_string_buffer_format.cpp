#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#include <cstring>
#include <cwchar>
#include <cstdarg>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// This shard implements the wide-character (CStringW / CSimpleStringT<wchar_t>)
// string methods.  The repo's CString class (include/openmfc/afxstr.h) is an
// ABI-faithful CStringW: a single m_pszData pointer with a CStringData header
// stored immediately before the character buffer.  CSimpleStringT<wchar_t,1>
// is the base subobject of CStringT and shares the exact same layout (one
// pointer), so a CSimpleStringT<wchar_t>* and a CStringT<wchar_t>* both alias a
// CString*.  We therefore route every method through the CString public API,
// or, for the few accessors not exposed publicly, through the documented
// CStringData layout.

namespace {
// Read the (private) m_pszData pointer: it is the first and only data member.
inline wchar_t* PszOf(void* pThis) {
    return *reinterpret_cast<wchar_t**>(pThis);
}
inline CStringData* DataOf(void* pThis) {
    return reinterpret_cast<CStringData*>(PszOf(pThis)) - 1;
}
} // namespace

// ---- CSimpleStringT<wchar_t,1> accessors -------------------------------------

// Symbol: ?Empty@?$CSimpleStringT@_W$00@ATL@@QEAAXXZ
extern "C" void MS_ABI impl__Empty___CSimpleStringT__W_00_ATL__QEAAXXZ(CString* pThis) {
    if (pThis) pThis->Empty();
}

// Symbol: ?GetAt@?$CSimpleStringT@_W$00@ATL@@QEBA_WH@Z
extern "C" wchar_t MS_ABI impl__GetAt___CSimpleStringT__W_00_ATL__QEBA_WH_Z(const CString* pThis, int nIndex) {
    if (!pThis) return L'\0';
    return pThis->GetAt(nIndex);
}

// Symbol: ?GetBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WH@Z
extern "C" wchar_t* MS_ABI impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString* pThis, int nMinBufferLength) {
    if (!pThis) return nullptr;
    return pThis->GetBuffer(nMinBufferLength);
}

// Symbol: ?GetBuffer@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WXZ
extern "C" wchar_t* MS_ABI impl__GetBuffer___CSimpleStringT__W_00_ATL__QEAAPEA_WXZ(CString* pThis) {
    if (!pThis) return nullptr;
    return pThis->GetBuffer();
}

// Symbol: ?GetBufferSetLength@?$CSimpleStringT@_W$00@ATL@@QEAAPEA_WH@Z
extern "C" wchar_t* MS_ABI impl__GetBufferSetLength___CSimpleStringT__W_00_ATL__QEAAPEA_WH_Z(CString* pThis, int nLength) {
    if (!pThis) return nullptr;
    if (nLength < 0) nLength = 0;
    wchar_t* pBuf = pThis->GetBuffer(nLength);
    // Set the logical length via the public API rather than poking
    // nDataLength directly (ReleaseBuffer keeps the string's state flags and
    // shared-empty-buffer handling correct, and writes the terminator).
    pThis->ReleaseBuffer(nLength);
    return pBuf;
}

// Symbol: ?GetAllocLength@?$CSimpleStringT@_W$00@ATL@@QEBAHXZ
extern "C" int MS_ABI impl__GetAllocLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const CString* pThis) {
    if (!pThis) return 0;
    return DataOf(const_cast<CString*>(pThis))->nAllocLength;
}

// ---- CStringT<wchar_t> methods ----------------------------------------------

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
