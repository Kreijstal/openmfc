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
//   - Detach()->VARIANT and GetByteArray(CByteArray&): the repo's vt-less
//     SAFEARRAY model cannot recover the element VARTYPE that a VARIANT result
//     requires, and GetByteArray couples to CByteArray's layout.
//   - AllocData/AllocDescriptor/DestroyDescriptor: these free or allocate the
//     bare descriptor, which is unsafe against an *embedded* (non-heap)
//     descriptor under this layout model.

#include <windows.h>
#include <oleauto.h>
#include <cstring>

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
    SafeArrayRedim(SA(pThis), psaboundNew);
}

// ?ResizeOneDim@COleSafeArray@@QEAAXK@Z
// void ResizeOneDim(DWORD dwElementCount) — preserves the existing lower bound.
// Symbol: ?ResizeOneDim@COleSafeArray@@QEAAXK@Z
extern "C" void MS_ABI impl__ResizeOneDim_COleSafeArray__QEAAXK_Z(
    void* pThis, unsigned long dwElementCount)
{
    if (!pThis) return;
    SAFEARRAYBOUND bound;
    bound.lLbound = SA(pThis)->rgsabound[0].lLbound;
    bound.cElements = dwElementCount;
    SafeArrayRedim(SA(pThis), &bound);
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
    SafeArrayDestroyData(SA(pThis));
}
