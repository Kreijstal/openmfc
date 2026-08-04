// COleSafeArray — OpenMFC implementation.
// Sources: ole_csafearray_ext.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleSafeArraySupport.h"
#include "detail/OlecoreSupport.h"

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
// Symbol: ??0COleSafeArray@@QEAA@AEBUtagSAFEARRAY@@G@Z
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_AEBUtagSAFEARRAY__G_Z(
    void* pThis, const SAFEARRAY* p0, unsigned short p1)
{
    if (!pThis) return nullptr;
    COleSafeArray* p = new(pThis) COleSafeArray();
    SAFEARRAY* pCopy = nullptr;
    if (p0 && SUCCEEDED(SafeArrayCopy(const_cast<SAFEARRAY*>(p0), &pCopy))) {
        StoreArray(p, pCopy, p1);
    }
    return p;
}
// Symbol: ??0COleSafeArray@@QEAA@PEBUtagSAFEARRAY@@G@Z
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_PEBUtagSAFEARRAY__G_Z(
    void* pThis, SAFEARRAY* p0, unsigned short p1)
{
    if (!pThis) return nullptr;
    COleSafeArray* p = new(pThis) COleSafeArray();
    SAFEARRAY* pCopy = nullptr;
    if (p0 && SUCCEEDED(SafeArrayCopy(p0, &pCopy))) {
        StoreArray(p, pCopy, p1);
    }
    return p;
}
// Symbol: ??0COleSafeArray@@QEAA@PEBUtagVARIANT@@@Z
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_PEBUtagVARIANT___Z(
    void* pThis, const VARIANT* p0)
{
    if (!pThis) return nullptr;
    COleSafeArray* p = new(pThis) COleSafeArray();
    if (p0) {
        impl___4COleSafeArray__QEAAAEAV0_AEBVCOleVariant___Z(
            pThis, reinterpret_cast<const COleVariant*>(p0));
    }
    return p;
}
// Symbol: ??0COleSafeArray@@QEAA@AEBUtagVARIANT@@@Z
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_AEBUtagVARIANT___Z(
    void* pThis, const VARIANT& p0)
{
    return impl___0COleSafeArray__QEAA_AEBVCOleVariant___Z(
        pThis, reinterpret_cast<const COleVariant*>(&p0));
}
// Symbol: ??0COleSafeArray@@QEAA@AEBV0@@Z
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_AEBV0__Z(
    void* pThis, const COleSafeArray* p0)
{
    if (!pThis) return nullptr;
    COleSafeArray* p = new(pThis) COleSafeArray();
    if (p0 && p0->parray) {
        p->Copy(p0);
    }
    return p;
}
// Symbol: ??0COleSafeArray@@QEAA@AEBVCOleVariant@@@Z
extern "C" COleSafeArray* MS_ABI impl___0COleSafeArray__QEAA_AEBVCOleVariant___Z(
    void* pThis, const COleVariant* varSrc)
{
    if (!pThis) return nullptr;
    // AfxSafeArrayInit: zero the 32-byte COleSafeArray
    memset(pThis, 0, 32);
    if (varSrc) {
        // operator=(const COleVariant&) inline – call directly
        impl___4COleSafeArray__QEAAAEAV0_AEBVCOleVariant___Z(pThis, varSrc);
    }
    RefreshCache(pThis);
    return static_cast<COleSafeArray*>(pThis);
}
// Symbol: ??4COleSafeArray@@QEAAAEAV0@AEBVCOleVariant@@@Z
extern "C" COleSafeArray* MS_ABI impl___4COleSafeArray__QEAAAEAV0_AEBVCOleVariant___Z(
    void* pThis, const COleVariant* varSrc)
{
    if (!pThis || !varSrc) return static_cast<COleSafeArray*>(pThis);
    const VARIANT* pSrc = reinterpret_cast<const VARIANT*>(varSrc);
    if (!(pSrc->vt & VT_ARRAY))
        AfxThrowOleException(E_INVALIDARG);
    ReleaseArray(pThis);
    CSAView* v = reinterpret_cast<CSAView*>(pThis);
    // VariantCopy into the tagVARIANT portion (first 24 bytes)
    VariantCopy(&v->var, const_cast<VARIANT*>(pSrc));
    RefreshCache(pThis);
    return static_cast<COleSafeArray*>(pThis);
}
// Symbol: ??8COleSafeArray@@QEBAHAEBVCOleVariant@@@Z
extern "C" int MS_ABI impl___8COleSafeArray__QEBAHAEBVCOleVariant___Z(
    const void* pThis, const COleVariant* varSrc)
{
    if (!pThis || !varSrc) return FALSE;
    const CSAView* v = reinterpret_cast<const CSAView*>(pThis);
    const VARIANT* pSrc = reinterpret_cast<const VARIANT*>(varSrc);
    if (v->var.vt != pSrc->vt) return FALSE;
    return _AfxCompareSafeArrays_OleCsafearrayExt(v->var.parray, pSrc->parray);
}
void COleSafeArray::Destroy() {
    if (parray) {
        SafeArrayDestroy(parray);
        parray = nullptr;
    }
    vt = VT_EMPTY;
    m_dwElementSize = 0;
    m_dwDims = 0;
}
void COleSafeArray::Clear() {
    VariantClear(this);   // frees parray and resets vt
    m_dwElementSize = 0;
    m_dwDims = 0;
}
void COleSafeArray::Create(VARTYPE vtype, DWORD cDims, SAFEARRAYBOUND* rgsabound) {
    Destroy();
    SAFEARRAY* psa = SafeArrayCreate(vtype, cDims, rgsabound);
    if (psa) {
        vt = VT_ARRAY | vtype;
        parray = psa;
        m_dwElementSize = psa->cbElements;
        m_dwDims = psa->cDims;
    }
}
void COleSafeArray::AccessData(void** ppvData) {
    SafeArrayAccessData(parray, ppvData);
}
void COleSafeArray::UnaccessData() {
    SafeArrayUnaccessData(parray);
}
void COleSafeArray::Attach(const SAFEARRAY& saSrc) {
    Destroy();
    // Deep-copy rather than alias the caller's descriptor: a const& may refer to
    // a stack/temporary SAFEARRAY, and Destroy() would later SafeArrayDestroy it.
    // (Element vartype isn't carried by a bare SAFEARRAY, so record only VT_ARRAY.)
    SAFEARRAY* psaNew = nullptr;
    if (SUCCEEDED(SafeArrayCopy(const_cast<SAFEARRAY*>(&saSrc), &psaNew)) && psaNew) {
        parray = psaNew;
        vt = VT_ARRAY;
        m_dwElementSize = psaNew->cbElements;
        m_dwDims = psaNew->cDims;
    }
}
SAFEARRAY* COleSafeArray::Detach() {
    SAFEARRAY* psa = parray;
    parray = nullptr;
    vt = VT_EMPTY;
    m_dwElementSize = 0;
    m_dwDims = 0;
    return psa;
}
void COleSafeArray::Copy(const COleSafeArray* psaSrc) {
    Destroy();
    if (psaSrc && psaSrc->parray) {
        SAFEARRAY* psaNew = nullptr;
        if (SUCCEEDED(SafeArrayCopy(psaSrc->parray, &psaNew)) && psaNew) {
            parray = psaNew;
            vt = psaSrc->vt;
            m_dwElementSize = psaNew->cbElements;
            m_dwDims = psaNew->cDims;
        }
    }
}
