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

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// COleSafeArray : public tagVARIANT (24) + two cached DWORDs (size 32). The
// SAFEARRAY is held by the variant's `parray`, not inline; vt = VT_ARRAY|<elem>.
struct CSAView {
    VARIANT       var;               // @0  (parray @8, vt @0)
    unsigned long m_dwElementSize;   // @24
    unsigned long m_dwDims;          // @28
};
static_assert(sizeof(CSAView) == 32, "COleSafeArray must be 32 bytes (tagVARIANT + 2 DWORD)");

// The owned SAFEARRAY for this object.
inline SAFEARRAY* SA(void* pThis) { return reinterpret_cast<CSAView*>(pThis)->var.parray; }

// Take ownership of a freshly-created heap SAFEARRAY, caching element size /
// dimension count like real MFC. No inline copy, so any dim count is fine.
inline void StoreArray(void* pThis, SAFEARRAY* psa, unsigned short vt) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    // If creation failed, leave a clean empty VARIANT rather than VT_ARRAY+null.
    v->var.vt = psa ? static_cast<unsigned short>(VT_ARRAY | vt) : static_cast<unsigned short>(VT_EMPTY);
    v->var.parray = psa;
    v->m_dwElementSize = psa ? psa->cbElements : 0;
    v->m_dwDims = psa ? psa->cDims : 0;
}

inline void StoreDescriptor(void* pThis, SAFEARRAY* psa) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    v->var.vt = psa ? static_cast<unsigned short>(VT_ARRAY) : static_cast<unsigned short>(VT_EMPTY);
    v->var.parray = psa;
    v->m_dwElementSize = psa ? psa->cbElements : 0;
    v->m_dwDims = psa ? psa->cDims : 0;
}

inline void RefreshCache(void* pThis) {
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    SAFEARRAY* psa = v->var.parray;
    v->m_dwElementSize = psa ? psa->cbElements : 0;
    v->m_dwDims = psa ? psa->cDims : 0;
}

namespace {

inline void SetByteArrayEmpty(CByteArray* pByteArray) {
    if (pByteArray) pByteArray->SetSize(0);
}

// Copy all bytes from a SAFEARRAY into a CByteArray.
// Returns false if the SAFEARRAY cannot be enumerated (including overflow), true otherwise.
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

} // namespace

// Release any array this object currently owns before overwriting it, so reusing
// one object across Create()/Attach() doesn't leak. A fresh object has
// vt==VT_EMPTY / parray==null (header ctor), so this is a no-op on first use.
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

} // namespace

// ?Create@COleSafeArray@@QEAAXGKPEAK@Z
// void Create(VARTYPE vt, DWORD dwDims, DWORD* rgElements)
// rgElements[i] is the element count of dimension i+1 (rgElements[0] -> dim 1),
// each with a zero lower bound. SafeArrayCreate performs its own internal
// reversal of rgsabound, so the bounds are passed through in the same order.
// Symbol: ?Create@COleSafeArray@@QEAAXGKPEAK@Z
extern "C" void MS_ABI impl__Create_COleSafeArray__QEAAXGKPEAK_Z(
    void* pThis, unsigned short vt, unsigned long dwDims, unsigned long* rgElements)
{
    if (!pThis || !rgElements || dwDims < 1 || dwDims > 64) return;
    ReleaseArray(pThis);  // free any previously-held array on reuse
    SAFEARRAYBOUND bounds[64];
    for (unsigned long i = 0; i < dwDims; ++i) {
        bounds[i].lLbound = 0;
        bounds[i].cElements = rgElements[i];
    }
    SAFEARRAY* psa = SafeArrayCreate(vt, dwDims, bounds);
    StoreArray(pThis, psa, vt);
}

// ?CreateOneDim@COleSafeArray@@QEAAXGKPEBXJ@Z
// void CreateOneDim(VARTYPE vt, DWORD dwElementCount, const void* pvSrcData, long lLbound=0)
// Symbol: ?CreateOneDim@COleSafeArray@@QEAAXGKPEBXJ@Z
extern "C" void MS_ABI impl__CreateOneDim_COleSafeArray__QEAAXGKPEBXJ_Z(
    void* pThis, unsigned short vt, unsigned long dwElementCount,
    const void* pvSrcData, long lLbound)
{
    if (!pThis) return;
    ReleaseArray(pThis);  // free any previously-held array on reuse
    SAFEARRAYBOUND bound;
    bound.lLbound = lLbound;
    bound.cElements = dwElementCount;
    SAFEARRAY* psa = SafeArrayCreate(vt, 1, &bound);
    StoreArray(pThis, psa, vt);
    if (pvSrcData && SA(pThis) && SA(pThis)->pvData && dwElementCount) {
        void* p = nullptr;
        if (SUCCEEDED(SafeArrayAccessData(SA(pThis), &p)) && p) {
            std::memcpy(p, pvSrcData,
                        (size_t)dwElementCount * SA(pThis)->cbElements);
            SafeArrayUnaccessData(SA(pThis));
        }
    }
}

// ?AllocDescriptor@COleSafeArray@@QEAAXK@Z
// void AllocDescriptor(DWORD dwDims) - allocates an empty SAFEARRAY descriptor
// owned by this COleSafeArray. Bounds and element type are filled by the caller
// before AllocData() in the classic OLEAUT32 workflow.
// Symbol: ?AllocDescriptor@COleSafeArray@@QEAAXK@Z
extern "C" void MS_ABI impl__AllocDescriptor_COleSafeArray__QEAAXK_Z(
    void* pThis, unsigned long dwDims)
{
    if (!pThis || dwDims == 0) return;
    ReleaseArray(pThis);
    SAFEARRAY* psa = nullptr;
    if (SUCCEEDED(SafeArrayAllocDescriptor(dwDims, &psa))) {
        StoreDescriptor(pThis, psa);
    }
}

// ?AllocData@COleSafeArray@@QEAAXXZ
// void AllocData() - allocates backing storage for an already-populated
// descriptor.
// Symbol: ?AllocData@COleSafeArray@@QEAAXXZ
extern "C" void MS_ABI impl__AllocData_COleSafeArray__QEAAXXZ(void* pThis)
{
    if (!pThis || !SA(pThis)) return;
    if (SUCCEEDED(SafeArrayAllocData(SA(pThis)))) {
        RefreshCache(pThis);
    }
}

// ?GetLBound@COleSafeArray@@QEAAXKPEAJ@Z
// void GetLBound(DWORD dwDim, long* pLBound)
// Symbol: ?GetLBound@COleSafeArray@@QEAAXKPEAJ@Z
extern "C" void MS_ABI impl__GetLBound_COleSafeArray__QEAAXKPEAJ_Z(
    void* pThis, unsigned long dwDim, long* pLBound)
{
    if (!pThis || !pLBound) return;
    SafeArrayGetLBound(SA(pThis), dwDim, pLBound);
}

// ?GetUBound@COleSafeArray@@QEAAXKPEAJ@Z
// void GetUBound(DWORD dwDim, long* pUBound)
// Symbol: ?GetUBound@COleSafeArray@@QEAAXKPEAJ@Z
extern "C" void MS_ABI impl__GetUBound_COleSafeArray__QEAAXKPEAJ_Z(
    void* pThis, unsigned long dwDim, long* pUBound)
{
    if (!pThis || !pUBound) return;
    SafeArrayGetUBound(SA(pThis), dwDim, pUBound);
}

// ?GetOneDimSize@COleSafeArray@@QEAAKXZ
// DWORD GetOneDimSize() -> element count of the single dimension.
// Symbol: ?GetOneDimSize@COleSafeArray@@QEAAKXZ
extern "C" unsigned long MS_ABI impl__GetOneDimSize_COleSafeArray__QEAAKXZ(void* pThis)
{
    if (!pThis) return 0;
    long lLBound = 0, lUBound = 0;
    if (FAILED(SafeArrayGetLBound(SA(pThis), 1, &lLBound))) return 0;
    if (FAILED(SafeArrayGetUBound(SA(pThis), 1, &lUBound))) return 0;
    return (unsigned long)(lUBound - lLBound + 1);
}

// ?GetElement@COleSafeArray@@QEAAXPEAJPEAX@Z
// void GetElement(long* rgIndices, void* pvData)
// Symbol: ?GetElement@COleSafeArray@@QEAAXPEAJPEAX@Z
extern "C" void MS_ABI impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(
    void* pThis, long* rgIndices, void* pvData)
{
    if (!pThis || !rgIndices || !pvData) return;
    SafeArrayGetElement(SA(pThis), rgIndices, pvData);
}

// ?PutElement@COleSafeArray@@QEAAXPEAJPEAX@Z
// void PutElement(long* rgIndices, void* pvData)
// Symbol: ?PutElement@COleSafeArray@@QEAAXPEAJPEAX@Z
extern "C" void MS_ABI impl__PutElement_COleSafeArray__QEAAXPEAJPEAX_Z(
    void* pThis, long* rgIndices, void* pvData)
{
    if (!pThis || !rgIndices || !pvData) return;
    SafeArrayPutElement(SA(pThis), rgIndices, pvData);
}

// ?PtrOfIndex@COleSafeArray@@QEAAXPEAJPEAPEAX@Z
// void PtrOfIndex(long* rgIndices, void** ppvData)
// Symbol: ?PtrOfIndex@COleSafeArray@@QEAAXPEAJPEAPEAX@Z
extern "C" void MS_ABI impl__PtrOfIndex_COleSafeArray__QEAAXPEAJPEAPEAX_Z(
    void* pThis, long* rgIndices, void** ppvData)
{
    if (!pThis || !rgIndices || !ppvData) return;
    SafeArrayPtrOfIndex(SA(pThis), rgIndices, ppvData);
}

// ?Lock@COleSafeArray@@QEAAXXZ
// Symbol: ?Lock@COleSafeArray@@QEAAXXZ
extern "C" void MS_ABI impl__Lock_COleSafeArray__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    SafeArrayLock(SA(pThis));
}

// ?Unlock@COleSafeArray@@QEAAXXZ
// Symbol: ?Unlock@COleSafeArray@@QEAAXXZ
extern "C" void MS_ABI impl__Unlock_COleSafeArray__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    SafeArrayUnlock(SA(pThis));
}

// ?Redim@COleSafeArray@@QEAAXPEAUtagSAFEARRAYBOUND@@@Z
// void Redim(SAFEARRAYBOUND* psaboundNew) — resizes the least-significant dim.
// Symbol: ?Redim@COleSafeArray@@QEAAXPEAUtagSAFEARRAYBOUND@@@Z
extern "C" void MS_ABI impl__Redim_COleSafeArray__QEAAXPEAUtagSAFEARRAYBOUND___Z(
    void* pThis, SAFEARRAYBOUND* psaboundNew)
{
    if (!pThis || !psaboundNew) return;
    if (SUCCEEDED(SafeArrayRedim(SA(pThis), psaboundNew))) {
        RefreshCache(pThis);
    }
}

// ?ResizeOneDim@COleSafeArray@@QEAAXK@Z
// void ResizeOneDim(DWORD dwElementCount) — preserves the existing lower bound.
// Symbol: ?ResizeOneDim@COleSafeArray@@QEAAXK@Z
extern "C" void MS_ABI impl__ResizeOneDim_COleSafeArray__QEAAXK_Z(
    void* pThis, unsigned long dwElementCount)
{
    if (!pThis || !SA(pThis) || SA(pThis)->cDims != 1) return;
    SAFEARRAYBOUND bound;
    bound.lLbound = SA(pThis)->rgsabound[0].lLbound;
    bound.cElements = dwElementCount;
    if (SUCCEEDED(SafeArrayRedim(SA(pThis), &bound))) {
        RefreshCache(pThis);
    }
}

// ?Copy@COleSafeArray@@QEAAXPEAPEAUtagSAFEARRAY@@@Z
// void Copy(SAFEARRAY** ppsa) — deep-copies *this into a new heap SAFEARRAY.
// Symbol: ?Copy@COleSafeArray@@QEAAXPEAPEAUtagSAFEARRAY@@@Z
extern "C" void MS_ABI impl__Copy_COleSafeArray__QEAAXPEAPEAUtagSAFEARRAY___Z(
    void* pThis, SAFEARRAY** ppsa)
{
    if (!pThis || !ppsa) return;
    *ppsa = nullptr;
    SafeArrayCopy(SA(pThis), ppsa);
}

// ?Attach@COleSafeArray@@QEAAXAEAUtagVARIANT@@@Z
// void Attach(VARIANT& varSrc) — takes over the array held by an array VARIANT.
// Symbol: ?Attach@COleSafeArray@@QEAAXAEAUtagVARIANT@@@Z
extern "C" void MS_ABI impl__Attach_COleSafeArray__QEAAXAEAUtagVARIANT___Z(
    void* pThis, VARIANT* pvarSrc)
{
    if (!pThis || !pvarSrc) return;
    if (!(pvarSrc->vt & VT_ARRAY) || pvarSrc->parray == nullptr) return;
    ReleaseArray(pThis);  // free any previously-held array before adopting
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    v->var.vt = pvarSrc->vt;
    v->var.parray = pvarSrc->parray;
    v->m_dwElementSize = pvarSrc->parray->cbElements;
    v->m_dwDims = pvarSrc->parray->cDims;
    // Transfer ownership: clear the source variant without freeing the array.
    VariantInit(pvarSrc);
}

// ?DestroyData@COleSafeArray@@QEAAXXZ
// void DestroyData() — frees the data buffer; the descriptor object remains.
// Symbol: ?DestroyData@COleSafeArray@@QEAAXXZ
extern "C" void MS_ABI impl__DestroyData_COleSafeArray__QEAAXXZ(void* pThis)
{
    if (!pThis) return;
    if (SUCCEEDED(SafeArrayDestroyData(SA(pThis)))) {
        RefreshCache(pThis);
    }
}

// ?DestroyDescriptor@COleSafeArray@@QEAAXXZ
// void DestroyDescriptor() - frees only the descriptor and detaches this wrapper.
// This mirrors OLEAUT32's descriptor-only lifecycle; callers that allocated data
// separately should call DestroyData() first.
// Symbol: ?DestroyDescriptor@COleSafeArray@@QEAAXXZ
extern "C" void MS_ABI impl__DestroyDescriptor_COleSafeArray__QEAAXXZ(void* pThis)
{
    if (!pThis || !SA(pThis)) return;
    SAFEARRAY* psa = SA(pThis);
    if (SUCCEEDED(SafeArrayDestroyDescriptor(psa))) {
        CSAView* v = reinterpret_cast<CSAView*>(pThis);
        v->var.vt = VT_EMPTY;
        v->var.parray = nullptr;
        v->m_dwElementSize = 0;
        v->m_dwDims = 0;
    }
}

// ?Detach@COleSafeArray@@QEAA?AUtagVARIANT@@XZ
// VARIANT Detach() — releases ownership of the SAFEARRAY descriptor and returns
// an equivalent VARIANT with VT_ARRAY set.
// Symbol: ?Detach@COleSafeArray@@QEAA?AUtagVARIANT@@XZ
extern "C" VARIANT* MS_ABI impl__Detach_COleSafeArray__QEAA_AUtagVARIANT__XZ(
    VARIANT* pRet, void* pThis)
{
    if (!pRet) return nullptr;
    VariantInit(pRet);
    if (!pThis) return pRet;
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    pRet->vt = v->var.vt;
    pRet->parray = static_cast<SAFEARRAY*>(v->var.parray);
    v->var.vt = VT_EMPTY;
    v->var.parray = nullptr;
    v->m_dwElementSize = 0;
    v->m_dwDims = 0;
    return pRet;
}

// ?GetByteArray@COleSafeArray@@QEAAXAEAVCByteArray@@@Z
// void GetByteArray(CByteArray& ba) — copy the raw byte payload of this SAFEARRAY.
// Symbol: ?GetByteArray@COleSafeArray@@QEAAXAEAVCByteArray@@@Z
extern "C" void MS_ABI impl__GetByteArray_COleSafeArray__QEAAXAEAVCByteArray___Z(
    void* pThis, CByteArray* pByteArray)
{
    if (!pThis || !pByteArray) return;
    SetByteArrayEmpty(pByteArray);
    SAFEARRAY* psa = SA(pThis);
    if (!psa) return;
    CopySafeArrayBytes(psa, pByteArray);
}

// ?GetByteArrayFromVariantArray@COleVariant@@QEAAXAEAVCByteArray@@@Z
// void GetByteArrayFromVariantArray(CByteArray& ba) — convert array VARIANT payload.
// Symbol: ?GetByteArrayFromVariantArray@COleVariant@@QEAAXAEAVCByteArray@@@Z
extern "C" void MS_ABI impl__GetByteArrayFromVariantArray_COleVariant__QEAAXAEAVCByteArray___Z(
    void* pThis, CByteArray* pByteArray)
{
    if (!pThis || !pByteArray) return;
    VARIANT* pVar = static_cast<VARIANT*>(pThis);
    SetByteArrayEmpty(pByteArray);
    if ((pVar->vt & VT_ARRAY) == 0) return;

    SAFEARRAY* psa = (pVar->vt & VT_BYREF)
        ? (pVar->pparray ? *pVar->pparray : nullptr)
        : pVar->parray;
    if (!psa) return;

    CopySafeArrayBytes(psa, pByteArray);
}
