#pragma once
// Shared internals of the former ole_csafearray_ext.cpp translation unit.
// Definitions live in detail/COleSafeArraySupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace colesafearray {} } }
using namespace openmfc::detail::colesafearray;
// OpenMFC: COleSafeArray — extended method set (oleaut32-backed).
//
// The repo models COleSafeArray as `class COleSafeArray : public SAFEARRAY`
// (see include/openmfc/afxole.h) — an embedded SAFEARRAY descriptor plus 24
// bytes of trailing padding (room for up to 4 dimensions of rgsabound). This is
// the repo's internal layout target (NOT retail mfc140u.dll, which derives from
// tagVARIANT). All methods here therefore treat `pThis` as a SAFEARRAY* and
// drive the real OLE Automation SafeArray* APIs, which are fully available under
// Wine (oleaut32), so the behavior is exercised end-to-end at runtime.
//
// olecore.cpp already owns Destroy/Clear/Create(SAFEARRAYBOUND*)/AccessData/
// UnaccessData/Attach(SAFEARRAY&)/Detach()->SAFEARRAY*/Copy(COleSafeArray*).
// thunks.cpp owns their exported impl__ thunks. This translation unit adds the
// remaining safely-implementable exports without touching the header or those
// files; each is a distinct mangled symbol from the ones already present.
//
// Deliberately NOT implemented here (left as honest weak stubs):
//   - Detach()->VARIANT.
//
// GetByteArray(CByteArray&) was listed here as unimplemented, but the code
// below implements it (and GetByteArrayFromVariantArray) against a local view
// of CByteArray. Both markers carried 32-bit mangling (QAA/AAV) where retail
// exports the 64-bit QEAA/AEAV form, so the .def aliased each export to a
// correctly-mangled impl__ name that only the stub generator defined -- the
// exports returned without doing anything and this code was unreachable.
// Markers and impl__ names corrected; the header now matches.

#include <windows.h>
#include <oleauto.h>
#include <cstring>
#include "openmfc/afx.h"
#include "openmfc/afxdisp.h"  // COleVariant
#include "openmfc/afxole.h"  // COleSafeArray

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


// COleSafeArray : public tagVARIANT (24) + two cached DWORDs (size 32). The
// SAFEARRAY is held by the variant's `parray`, not inline; vt = VT_ARRAY|<elem>.
namespace openmfc { namespace detail { namespace colesafearray {
struct CSAView {
    VARIANT       var;               // @0  (parray @8, vt @0)
    unsigned long m_dwElementSize;   // @24
    unsigned long m_dwDims;          // @28
};
} } }

// The owned SAFEARRAY for this object.
namespace openmfc { namespace detail { namespace colesafearray {
inline SAFEARRAY* SA(void* pThis) { return reinterpret_cast<CSAView*>(pThis)->var.parray; }
} } }

// Take ownership of a freshly-created heap SAFEARRAY, caching element size /
// dimension count like real MFC. No inline copy, so any dim count is fine.
namespace openmfc { namespace detail { namespace colesafearray {
inline void StoreArray(void* pThis, SAFEARRAY* psa, unsigned short vt) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    // If creation failed, leave a clean empty VARIANT rather than VT_ARRAY+null.
    v->var.vt = psa ? static_cast<unsigned short>(VT_ARRAY | vt) : static_cast<unsigned short>(VT_EMPTY);
    v->var.parray = psa;
    v->m_dwElementSize = psa ? psa->cbElements : 0;
    v->m_dwDims = psa ? psa->cDims : 0;
}
} } }

namespace openmfc { namespace detail { namespace colesafearray {
inline void StoreDescriptor(void* pThis, SAFEARRAY* psa) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    v->var.vt = psa ? static_cast<unsigned short>(VT_ARRAY) : static_cast<unsigned short>(VT_EMPTY);
    v->var.parray = psa;
    v->m_dwElementSize = psa ? psa->cbElements : 0;
    v->m_dwDims = psa ? psa->cDims : 0;
}
} } }

namespace openmfc { namespace detail { namespace colesafearray {
inline void RefreshCache(void* pThis) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    SAFEARRAY* psa = v->var.parray;
    v->m_dwElementSize = psa ? psa->cbElements : 0;
    v->m_dwDims = psa ? psa->cDims : 0;
}
} } }


namespace openmfc { namespace detail { namespace colesafearray {
inline void SetByteArrayEmpty(CByteArray* pByteArray) {
    if (pByteArray) pByteArray->SetSize(0);
}
} } }

// Copy all bytes from a SAFEARRAY into a CByteArray.
// Returns false if the SAFEARRAY cannot be enumerated (including overflow), true otherwise.
namespace openmfc { namespace detail { namespace colesafearray {
inline bool CopySafeArrayBytes(SAFEARRAY* psa, CByteArray* pByteArray) {
    if (!psa || !pByteArray) return false;
    unsigned long long uBytes = 0;
    if (psa->cbElements == 0) {
        SetByteArrayEmpty(pByteArray);
        return true;
    }

    unsigned long long uElemCount = 1;
    for (unsigned long iDim = 1; iDim <= psa->cDims; ++iDim) {
        LONG lBound = 0;
        LONG uBound = 0;
        if (FAILED(SafeArrayGetLBound(psa, iDim, &lBound)) ||
            FAILED(SafeArrayGetUBound(psa, iDim, &uBound))) {
            return false;
        }
        if (uBound < lBound) {
            SetByteArrayEmpty(pByteArray);
            return false;
        }
        unsigned long long dimCount = static_cast<unsigned long long>(uBound - lBound + 1);
        if (dimCount == 0 || uElemCount > (static_cast<unsigned long long>(-1) / dimCount)) {
            SetByteArrayEmpty(pByteArray);
            return false;
        }
        uElemCount *= dimCount;
    }

    if (uElemCount > (static_cast<unsigned long long>(-1) / psa->cbElements)) {
        SetByteArrayEmpty(pByteArray);
        return false;
    }
    uBytes = uElemCount * psa->cbElements;

    void* pData = nullptr;
    if (FAILED(SafeArrayAccessData(psa, &pData)) || !pData) {
        return false;
    }

    pByteArray->SetSize(static_cast<long long>(uBytes));
    if (uBytes != 0) {
        std::memcpy(pByteArray->GetData(), pData, static_cast<size_t>(uBytes));
    }
    SafeArrayUnaccessData(psa);
    return true;
}
} } }


// Release any array this object currently owns before overwriting it, so reusing
// one object across Create()/Attach() doesn't leak. A fresh object has
// vt==VT_EMPTY / parray==null (header ctor), so this is a no-op on first use.
namespace openmfc { namespace detail { namespace colesafearray {
inline void ReleaseArray(void* pThis) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    if (v->var.parray != nullptr) {
        SafeArrayDestroy(v->var.parray);
    }
    v->var.vt = VT_EMPTY;
    v->var.parray = nullptr;
    v->m_dwElementSize = 0;
    v->m_dwDims = 0;
}
} } }
























// Constructor helpers are defined below the overload set but are needed by
// the VARIANT-taking constructors that appear first.
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_AEBVCOleVariant___Z(
    void* pThis, const COleVariant* varSrc);
extern "C" COleSafeArray* MS_ABI impl___4COleSafeArray__QEAAAEAV0_AEBVCOleVariant___Z(
    void* pThis, const COleVariant* varSrc);






//----------------------------------------------------------------------------
// COleSafeArray – ctor/operator=/operator== from COleVariant
//----------------------------------------------------------------------------

// Forward-declare operator= (called by ctor below)
extern "C" COleSafeArray* MS_ABI impl___4COleSafeArray__QEAAAEAV0_AEBVCOleVariant___Z(
    void* pThis, const COleVariant* varSrc);

// _AfxCompareSafeArrays_OleCsafearrayExt helper – same logic as in olecore.cpp.
namespace openmfc { namespace detail { namespace colesafearray {
BOOL _AfxCompareSafeArrays_OleCsafearrayExt(SAFEARRAY* parray1, SAFEARRAY* parray2);
} } }



