// COleVariant — OpenMFC implementation.
// Sources: ole_csafearray_ext.cpp

#include "detail/COleSafeArraySupport.h"

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
void COleVariant::GetByteArrayFromVariantArray(CByteArray& byteArray)
{
    impl__GetByteArrayFromVariantArray_COleVariant__QEAAXAEAVCByteArray___Z(
        this, &byteArray);
}
