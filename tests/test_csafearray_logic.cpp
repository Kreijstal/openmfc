// Behavioral test for COleSafeArray extended methods (ole_csafearray_ext.cpp).
//
// Includes the impl .cpp directly and drives the exported impl_ thunks against a
// COleSafeArray-layout object: a SAFEARRAY base plus 24 bytes of trailing
// padding (room for up to 4 dimensions of rgsabound), matching
// include/openmfc/afxole.h. Exercises real oleaut32 SafeArray operations under
// Wine: create, element round-trip, bounds, lock, redim/resize, copy, attach.

#include <windows.h>
#include <oleauto.h>
#include <cstdio>
#include <cstring>

#include "../phase4/src/ole_csafearray_ext.cpp"

// Mirror COleSafeArray's layout: SAFEARRAY (rgsabound[1] inside) + 24 pad bytes.
struct alignas(8) OleSA {
    SAFEARRAY sa;
    char      pad[24];
    OleSA() { std::memset(this, 0, sizeof(*this)); }
};
static_assert(sizeof(OleSA) == sizeof(SAFEARRAY) + 24, "OleSA layout");

static int g_failures = 0;
static void check(bool cond, const char* what) {
    std::printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

int main() {
    // ----- 1. CreateOneDim VT_I4 with seed data -----
    OleSA a;
    long seed[5] = {10, 20, 30, 40, 50};
    impl__CreateOneDim_COleSafeArray__QEAAXGKPEBXJ_Z(&a, VT_I4, 5, seed, 0);
    check(a.sa.cDims == 1, "CreateOneDim: cDims == 1");
    check(a.sa.cbElements == sizeof(long), "CreateOneDim: cbElements == 4");
    check(a.sa.pvData != nullptr, "CreateOneDim: data allocated");

    long lb = -1, ub = -1;
    impl__GetLBound_COleSafeArray__QEAAXKPEAJ_Z(&a, 1, &lb);
    impl__GetUBound_COleSafeArray__QEAAXKPEAJ_Z(&a, 1, &ub);
    check(lb == 0, "GetLBound == 0");
    check(ub == 4, "GetUBound == 4");
    check(impl__GetOneDimSize_COleSafeArray__QEAAKXZ(&a) == 5, "GetOneDimSize == 5");

    long idx = 2, val = 0;
    impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(&a, &idx, &val);
    check(val == 30, "GetElement[2] == 30 (seed copied)");

    long put = 99;
    impl__PutElement_COleSafeArray__QEAAXPEAJPEAX_Z(&a, &idx, &put);
    val = 0;
    impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(&a, &idx, &val);
    check(val == 99, "PutElement[2]=99 then GetElement[2]==99");

    long idx4 = 4;
    void* p = nullptr;
    impl__PtrOfIndex_COleSafeArray__QEAAXPEAJPEAPEAX_Z(&a, &idx4, &p);
    check(p && *reinterpret_cast<long*>(p) == 50, "PtrOfIndex[4] points at 50");

    // ----- 2. Lock / Unlock adjust the lock count -----
    impl__Lock_COleSafeArray__QEAAXXZ(&a);
    check(a.sa.cLocks == 1, "Lock: cLocks == 1");
    impl__Unlock_COleSafeArray__QEAAXXZ(&a);
    check(a.sa.cLocks == 0, "Unlock: cLocks == 0");

    // ----- 3. ResizeOneDim grows; existing elements preserved -----
    impl__ResizeOneDim_COleSafeArray__QEAAXK_Z(&a, 8);
    check(impl__GetOneDimSize_COleSafeArray__QEAAKXZ(&a) == 8, "ResizeOneDim -> size 8");
    long i0 = 0; val = 0;
    impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(&a, &i0, &val);
    check(val == 10, "ResizeOneDim preserves element[0]==10");

    // ----- 4. Redim to a fresh bound -----
    SAFEARRAYBOUND nb; nb.lLbound = 0; nb.cElements = 3;
    impl__Redim_COleSafeArray__QEAAXPEAUtagSAFEARRAYBOUND___Z(&a, &nb);
    check(impl__GetOneDimSize_COleSafeArray__QEAAKXZ(&a) == 3, "Redim -> size 3");

    // ----- 5. Copy makes an independent array -----
    SAFEARRAY* psaCopy = nullptr;
    impl__Copy_COleSafeArray__QEAAXPEAPEAUtagSAFEARRAY___Z(&a, &psaCopy);
    check(psaCopy != nullptr, "Copy produced a new SAFEARRAY");
    if (psaCopy) {
        long cval = -1;
        SafeArrayGetElement(psaCopy, &i0, &cval);
        long mod = 7777;
        impl__PutElement_COleSafeArray__QEAAXPEAJPEAX_Z(&a, &i0, &mod);
        long cval2 = -1;
        SafeArrayGetElement(psaCopy, &i0, &cval2);
        check(cval == cval2, "Copy is independent of the original");
        SafeArrayDestroy(psaCopy);
    }

    // ----- 6. DestroyData frees the data buffer -----
    impl__DestroyData_COleSafeArray__QEAAXXZ(&a);
    check(a.sa.pvData == nullptr, "DestroyData clears pvData");

    // ----- 7. Multi-dim Create (2x3); both bounds must be copied -----
    OleSA m;
    unsigned long dims2[2] = {2, 3};  // logical [2][3]
    impl__Create_COleSafeArray__QEAAXGKPEAK_Z(&m, VT_I4, 2, dims2);
    check(m.sa.cDims == 2, "Create 2-D: cDims == 2");
    long l1 = 0, u1 = 0, l2 = 0, u2 = 0;
    impl__GetLBound_COleSafeArray__QEAAXKPEAJ_Z(&m, 1, &l1);
    impl__GetUBound_COleSafeArray__QEAAXKPEAJ_Z(&m, 1, &u1);
    impl__GetLBound_COleSafeArray__QEAAXKPEAJ_Z(&m, 2, &l2);
    impl__GetUBound_COleSafeArray__QEAAXKPEAJ_Z(&m, 2, &u2);
    long total = (u1 - l1 + 1) * (u2 - l2 + 1);
    check(total == 6, "Create 2-D: 6 total elements (both bounds intact)");
    long mi[2] = {1, 2}, mv = 1234, rv = 0;
    impl__PutElement_COleSafeArray__QEAAXPEAJPEAX_Z(&m, mi, &mv);
    impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(&m, mi, &rv);
    check(rv == 1234, "Create 2-D: element [1][2] round-trips");

    // ----- 8. Attach takes over an array VARIANT -----
    SAFEARRAYBOUND ab; ab.lLbound = 0; ab.cElements = 4;
    SAFEARRAY* src = SafeArrayCreate(VT_R8, 1, &ab);
    double d2 = 3.5; long si = 2;
    SafeArrayPutElement(src, &si, &d2);
    VARIANT var; VariantInit(&var);
    var.vt = VT_ARRAY | VT_R8;
    var.parray = src;
    OleSA at;
    impl__Attach_COleSafeArray__QEAAXAEAUtagVARIANT___Z(&at, &var);
    check(impl__GetOneDimSize_COleSafeArray__QEAAKXZ(&at) == 4, "Attach: size 4 adopted");
    double dr = 0;
    impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(&at, &si, &dr);
    check(dr == 3.5, "Attach: element [2] == 3.5");
    check(var.vt == VT_EMPTY && var.parray == nullptr, "Attach: source variant cleared");

    // ----- reuse: a second Create on the same object must release the first
    // array's data (no leak) and produce a correct new array -----
    OleSA reuse;
    long first[3] = {1, 2, 3};
    impl__CreateOneDim_COleSafeArray__QEAAXGKPEBXJ_Z(&reuse, VT_I4, 3, first, 0);
    long second[5] = {7, 8, 9, 10, 11};
    impl__CreateOneDim_COleSafeArray__QEAAXGKPEBXJ_Z(&reuse, VT_I4, 5, second, 0);
    check(impl__GetOneDimSize_COleSafeArray__QEAAKXZ(&reuse) == 5, "reuse: second Create -> size 5");
    check(reuse.sa.pvData != nullptr, "reuse: new buffer allocated");
    long got = 0; long idx3 = 3;
    impl__GetElement_COleSafeArray__QEAAXPEAJPEAX_Z(&reuse, &idx3, &got);
    check(got == 10, "reuse: second array element [3] == 10");
    impl__DestroyData_COleSafeArray__QEAAXXZ(&reuse);

    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
