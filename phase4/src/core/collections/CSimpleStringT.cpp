// CSimpleStringT — OpenMFC implementation.
// Sources: global_cstring_core.cpp, global_string_buffer_format.cpp, global_string_destructors.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CStringTSupport.h"

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
// Symbol: ?GetLength@?$CSimpleStringT@_W$00@ATL@@QEBAHXZ
extern "C" int MS_ABI impl__GetLength___CSimpleStringT__W_00_ATL__QEBAHXZ(const void* pThis) {
    return static_cast<const CString*>(pThis)->GetLength();
}
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
// Symbol: ??1?$CSimpleStringT@_W$00@ATL@@QEAA@XZ
extern "C" void MS_ABI
impl___1__CSimpleStringT__W_00_ATL__QEAA_XZ(CString* pThis) {
    if (!pThis) {
        return;
    }
    pThis->~CString();
}
