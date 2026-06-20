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

// Symbol: ??1?$CSimpleStringT@_W$00@ATL@@QEAA@XZ
extern "C" void MS_ABI
impl___1__CSimpleStringT__W_00_ATL__QEAA_XZ(CString* pThis) {
    if (!pThis) {
        return;
    }
    pThis->~CString();
}

// Symbol: ??1?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAA@XZ
extern "C" void MS_ABI
impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(CString* pThis) {
    if (!pThis) {
        return;
    }
    pThis->~CString();
}
