#include "COleCurrencySupport.h"

static_assert(offsetof(COleCurrencyView, m_status) == 8, "m_status @8");
namespace openmfc { namespace detail { namespace colecurrency {
// Symbol: ?Format@COleCurrency@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@KK@Z
extern "C" void MS_ABI impl__Format_COleCurrency__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__KK_Z(
    CString* pRet, const void* pThis, unsigned long dwFormat, unsigned long lcid)
{
    if (!pRet) {
        return;
    }

    if (!pThis) {
        new (pRet) CString();
        return;
    }

    const COleCurrencyView* p = reinterpret_cast<const COleCurrencyView*>(pThis);
    if (p->m_status != CY_VALID) {
        new (pRet) CString();
        return;
    }

    BSTR bstr = nullptr;
    HRESULT hr = VarBstrFromCy(p->m_cur, lcid ? (LCID)lcid : GetThreadLocale(), dwFormat, &bstr);
    if (SUCCEEDED(hr) && bstr) {
        new (pRet) CString(bstr);
        SysFreeString(bstr);
        return;
    }

    if (bstr) {
        SysFreeString(bstr);
    }
    new (pRet) CString();
}
ComparisonStatus CurrencyStatusFromPair(const COleCurrency* lhs, const COleCurrency* rhs) {
    if (!lhs || !rhs) return CMP_INVALID;
    if (lhs->m_status == COleCurrency::CY_NULL || rhs->m_status == COleCurrency::CY_NULL) {
        return CMP_NULL;
    }
    if (lhs->m_status != COleCurrency::CY_VALID || rhs->m_status != COleCurrency::CY_VALID) {
        return CMP_INVALID;
    }
    return CMP_VALID;
}
ComparisonStatus CurrencyStatusSingle(const COleCurrency* value) {
    if (!value) return CMP_INVALID;
    if (value->m_status == COleCurrency::CY_NULL) return CMP_NULL;
    if (value->m_status != COleCurrency::CY_VALID) return CMP_INVALID;
    return CMP_VALID;
}
} } }  // namespace openmfc::detail::colecurrency
