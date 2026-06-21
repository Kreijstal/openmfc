// OpenMFC: COleCurrency::ParseCurrency (oleaut32-backed).
//
// COleCurrency holds a CURRENCY (CY) value plus a status enum. Layout (verified
// at runtime): CY m_cur @0, CurrencyStatus m_status @8 (CY_VALID=0, CY_INVALID=1,
// CY_NULL=2), sizeof 32. ParseCurrency parses a localized currency string into
// m_cur via VarCyFromStr, mirroring MFC: on success m_status becomes CY_VALID,
// on failure m_cur is zeroed and m_status becomes CY_INVALID.
//
// COleCurrency::SetCurrency is already implemented in olecore.cpp; Format (a
// CStringT-by-value return) is left for a string-coupled tranche.

#include <windows.h>
#include <oleauto.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// Layout-faithful view of COleCurrency (only the two leading members).
struct COleCurrencyView {
    CY  m_cur;       // @0
    int m_status;    // @8  (0=valid, 1=invalid, 2=null)
};
static_assert(offsetof(COleCurrencyView, m_status) == 8, "m_status @8");
enum { CY_VALID = 0, CY_INVALID = 1 };
} // namespace

// ?ParseCurrency@COleCurrency@@QEAAHPEB_WKK@Z
// int ParseCurrency(const wchar_t* lpszCurrency, DWORD dwFlags, LCID lcid)
// Symbol: ?ParseCurrency@COleCurrency@@QEAAHPEB_WKK@Z
extern "C" int MS_ABI impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(
    void* pThis, const wchar_t* lpszCurrency, unsigned long dwFlags, unsigned long lcid)
{
    if (!pThis) return 0;
    COleCurrencyView* p = reinterpret_cast<COleCurrencyView*>(pThis);
    if (!lpszCurrency) {
        p->m_cur.int64 = 0;
        p->m_status = CY_INVALID;
        return 0;
    }
    LCID useLcid = lcid ? (LCID)lcid : GetThreadLocale();
    HRESULT hr = VarCyFromStr(const_cast<wchar_t*>(lpszCurrency), useLcid, dwFlags, &p->m_cur);
    if (SUCCEEDED(hr)) {
        p->m_status = CY_VALID;
        return 1;
    }
    p->m_cur.int64 = 0;
    p->m_status = CY_INVALID;
    return 0;
}
