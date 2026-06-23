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

// Symbol: ?IsEmpty@?$CSimpleStringT@_W$00@ATL@@QEBA_NXZ
extern "C" bool MS_ABI impl__IsEmpty___CSimpleStringT__W_00_ATL__QEBA_NXZ(const void* pThis) {
    return static_cast<const CString*>(pThis)->IsEmpty();
}

// Symbol: ?GetString@?$CSimpleStringT@_W$00@ATL@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetString___CSimpleStringT__W_00_ATL__QEBAPEB_WXZ(const void* pThis) {
    return static_cast<const CString*>(pThis)->GetString();
}

// Symbol: ??A?$CSimpleStringT@_W$00@ATL@@QEBA_WH@Z
extern "C" wchar_t MS_ABI impl___A__CSimpleStringT__W_00_ATL__QEBA_WH_Z(const void* pThis, int i) {
    return static_cast<const CString*>(pThis)->operator[](i);
}

// Symbol: ??4?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAAEAV01@PEB_W@Z
extern "C" void* MS_ABI impl___4__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV01_PEB_W_Z(void* pThis, const wchar_t* psz) {
    *static_cast<CString*>(pThis) = psz;
    return pThis;
}

// Symbol: ?GetLength@?$CSimpleStringT@_W$00@ATL@@QEBAHXZ
extern "C" int MS_ABI impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const void* pThis) {
    return static_cast<const CString*>(pThis)->GetLength();
}

// Symbol: ??B?$CSimpleStringT@_W$00@ATL@@QEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl___B__CSimpleStringT__W_00_ATL__QEBAPEB_WXZ(const void* pThis) {
    return static_cast<const CString*>(pThis)->GetString();
}
