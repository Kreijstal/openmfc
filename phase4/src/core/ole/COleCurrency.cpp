// COleCurrency — OpenMFC implementation.
// Sources: ole_colecurrency_parse.cpp, olecore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/COleCurrencySupport.h"
#include "detail/OlecoreSupport.h"

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
// Symbol: ??4COleCurrency@@QEAAAEBV0@AEBUtagVARIANT@@@Z
// COleCurrency::operator=(tagVARIANT const&)
extern "C" const COleCurrency* MS_ABI impl___4COleCurrency__QEAAAEBV0_AEBUtagVARIANT___Z(
    COleCurrency* pThis, const VARIANT* pSrc)
{
    if (!pThis) {
        return nullptr;
    }

    if (!pSrc) {
        pThis->SetStatus(COleCurrency::CY_INVALID);
        return pThis;
    }

    if (pSrc->vt == VT_EMPTY || pSrc->vt == VT_NULL) {
        pThis->SetStatus(COleCurrency::CY_NULL);
        return pThis;
    }

    VARIANT copy;
    VariantInit(&copy);
    if (FAILED(VariantCopy(&copy, const_cast<VARIANT*>(pSrc)))) {
        pThis->SetStatus(COleCurrency::CY_INVALID);
        return pThis;
    }

    if (copy.vt != VT_CY) {
        HRESULT hr = VariantChangeType(&copy, &copy, 0, VT_CY);
        if (FAILED(hr)) {
            VariantClear(&copy);
            pThis->SetStatus(COleCurrency::CY_INVALID);
            return pThis;
        }
    }

    pThis->m_cur = copy.cyVal;
    pThis->SetStatus(COleCurrency::CY_VALID);
    VariantClear(&copy);
    return pThis;
}
// Symbol: ??4COleCurrency@@QEAAAEBV0@AEBV0@@Z
// COleCurrency::operator=(COleCurrency const&)
extern "C" const COleCurrency* MS_ABI impl___4COleCurrency__QEAAAEBV0_AEBV0__Z(
    COleCurrency* pThis, const COleCurrency* pSrc)
{
    if (!pThis) return nullptr;
    if (!pSrc) {
        pThis->SetStatus(COleCurrency::CY_INVALID);
        return pThis;
    }
    pThis->m_cur = pSrc->m_cur;
    pThis->SetStatus(static_cast<COleCurrency::CurrencyStatus>(pSrc->m_status));
    return pThis;
}
// Symbol: ??4COleCurrency@@QEAAAEBV0@TtagCY@@@Z
// COleCurrency::operator=(tagCY)
extern "C" const COleCurrency* MS_ABI impl___4COleCurrency__QEAAAEBV0_TtagCY___Z(
    COleCurrency* pThis, const CY& src)
{
    if (!pThis) return nullptr;
    pThis->m_cur = src;
    pThis->SetStatus(COleCurrency::CY_VALID);
    return pThis;
}
// Symbol: ??DCOleCurrency@@QEBA?AV0@J@Z
// COleCurrency::operator*(long) const
extern "C" void MS_ABI impl___DCOleCurrency__QEBA_AV0_J_Z(
    COleCurrency* pRet, const COleCurrency* pThis, long rhs)
{
    if (!pRet || !pThis) return;
    if (CurrencyStatusSingle(pThis) == CMP_VALID) {
        pRet->m_cur.int64 = pThis->m_cur.int64 * static_cast<long long>(rhs);
        pRet->SetStatus(COleCurrency::CY_VALID);
        return;
    }
    if (CurrencyStatusSingle(pThis) == CMP_NULL) {
        pRet->m_cur.int64 = 0;
        pRet->SetStatus(COleCurrency::CY_NULL);
        return;
    }
    pRet->m_cur.int64 = 0;
    pRet->SetStatus(COleCurrency::CY_INVALID);
}
// Symbol: ??KCOleCurrency@@QEBA?AV0@J@Z
// COleCurrency::operator/(long) const
extern "C" void MS_ABI impl___KCOleCurrency__QEBA_AV0_J_Z(
    COleCurrency* pRet, const COleCurrency* pThis, long rhs)
{
    if (!pRet || !pThis) return;
    if (rhs == 0) {
        pRet->m_cur.int64 = 0;
        pRet->SetStatus(COleCurrency::CY_INVALID);
        return;
    }
    if (CurrencyStatusSingle(pThis) == CMP_VALID) {
        pRet->m_cur.int64 = pThis->m_cur.int64 / static_cast<long long>(rhs);
        pRet->SetStatus(COleCurrency::CY_VALID);
        return;
    }
    if (CurrencyStatusSingle(pThis) == CMP_NULL) {
        pRet->m_cur.int64 = 0;
        pRet->SetStatus(COleCurrency::CY_NULL);
        return;
    }
    pRet->m_cur.int64 = 0;
    pRet->SetStatus(COleCurrency::CY_INVALID);
}
// Symbol: ??GCOleCurrency@@QEBA?AV0@AEBV0@@Z
// COleCurrency::operator-(COleCurrency const&) const
extern "C" void MS_ABI impl___GCOleCurrency__QEBA_AV0_AEBV0__Z(
    COleCurrency* pRet, const COleCurrency* pThis, const COleCurrency* rhs)
{
    if (!pRet || !pThis || !rhs) {
        if (pRet) {
            pRet->m_cur.int64 = 0;
            pRet->SetStatus(COleCurrency::CY_INVALID);
        }
        return;
    }

    ComparisonStatus status = CurrencyStatusFromPair(pThis, rhs);
    if (status == CMP_VALID) {
        pRet->m_cur.int64 = pThis->m_cur.int64 - rhs->m_cur.int64;
        pRet->SetStatus(COleCurrency::CY_VALID);
        return;
    }
    if (status == CMP_NULL) {
        pRet->m_cur.int64 = 0;
        pRet->SetStatus(COleCurrency::CY_NULL);
        return;
    }
    pRet->m_cur.int64 = 0;
    pRet->SetStatus(COleCurrency::CY_INVALID);
}
// Symbol: ??GCOleCurrency@@QEBA?AV0@XZ
// COleCurrency::operator-() const
extern "C" void MS_ABI impl___GCOleCurrency__QEBA_AV0_XZ(
    COleCurrency* pRet, const COleCurrency* pThis)
{
    if (!pRet || !pThis) {
        if (pRet) {
            pRet->m_cur.int64 = 0;
            pRet->SetStatus(COleCurrency::CY_INVALID);
        }
        return;
    }
    if (CurrencyStatusSingle(pThis) == CMP_VALID) {
        pRet->m_cur.int64 = -pThis->m_cur.int64;
        pRet->SetStatus(COleCurrency::CY_VALID);
        return;
    }
    if (CurrencyStatusSingle(pThis) == CMP_NULL) {
        pRet->m_cur.int64 = 0;
        pRet->SetStatus(COleCurrency::CY_NULL);
        return;
    }
    pRet->m_cur.int64 = 0;
    pRet->SetStatus(COleCurrency::CY_INVALID);
}
// Symbol: ??HCOleCurrency@@QEBA?AV0@AEBV0@@Z
// COleCurrency::operator+(COleCurrency const&) const
extern "C" void MS_ABI impl___HCOleCurrency__QEBA_AV0_AEBV0__Z(
    COleCurrency* pRet, const COleCurrency* pThis, const COleCurrency* rhs)
{
    if (!pRet || !pThis || !rhs) {
        if (pRet) {
            pRet->m_cur.int64 = 0;
            pRet->SetStatus(COleCurrency::CY_INVALID);
        }
        return;
    }

    ComparisonStatus status = CurrencyStatusFromPair(pThis, rhs);
    if (status == CMP_VALID) {
        pRet->m_cur.int64 = pThis->m_cur.int64 + rhs->m_cur.int64;
        pRet->SetStatus(COleCurrency::CY_VALID);
        return;
    }
    if (status == CMP_NULL) {
        pRet->m_cur.int64 = 0;
        pRet->SetStatus(COleCurrency::CY_NULL);
        return;
    }
    pRet->m_cur.int64 = 0;
    pRet->SetStatus(COleCurrency::CY_INVALID);
}
// Symbol: ??MCOleCurrency@@QEBAHAEBV0@@Z
// COleCurrency::operator<(COleCurrency const&) const
extern "C" int MS_ABI impl___MCOleCurrency__QEBAHAEBV0__Z(
    const COleCurrency* pThis, const COleCurrency* rhs)
{
    if (!pThis || !rhs) return FALSE;
    if (CurrencyStatusFromPair(pThis, rhs) != CMP_VALID) return FALSE;
    return pThis->m_cur.int64 < rhs->m_cur.int64;
}
// Symbol: ??NCOleCurrency@@QEBAHAEBV0@@Z
// COleCurrency::operator<=(COleCurrency const&) const
extern "C" int MS_ABI impl___NCOleCurrency__QEBAHAEBV0__Z(
    const COleCurrency* pThis, const COleCurrency* rhs)
{
    if (!pThis || !rhs) return FALSE;
    if (CurrencyStatusFromPair(pThis, rhs) != CMP_VALID) return FALSE;
    return pThis->m_cur.int64 <= rhs->m_cur.int64;
}
// Symbol: ??OCOleCurrency@@QEBAHAEBV0@@Z
// COleCurrency::operator>(COleCurrency const&) const
extern "C" int MS_ABI impl___OCOleCurrency__QEBAHAEBV0__Z(
    const COleCurrency* pThis, const COleCurrency* rhs)
{
    if (!pThis || !rhs) return FALSE;
    if (CurrencyStatusFromPair(pThis, rhs) != CMP_VALID) return FALSE;
    return pThis->m_cur.int64 > rhs->m_cur.int64;
}
// Symbol: ??PCOleCurrency@@QEBAHAEBV0@@Z
// COleCurrency::operator>=(COleCurrency const&) const
extern "C" int MS_ABI impl___PCOleCurrency__QEBAHAEBV0__Z(
    const COleCurrency* pThis, const COleCurrency* rhs)
{
    if (!pThis || !rhs) return FALSE;
    if (CurrencyStatusFromPair(pThis, rhs) != CMP_VALID) return FALSE;
    return pThis->m_cur.int64 >= rhs->m_cur.int64;
}
COleCurrency::COleCurrency(long nUnits, long nFractionalUnits) {
    m_cur.int64 = ((long long)nUnits * 10000) + nFractionalUnits;
    m_status = CY_VALID;
}
COleCurrency::COleCurrency(const CY& cySrc) {
    m_cur = cySrc;
    m_status = CY_VALID;
}
COleCurrency::COleCurrency(const COleCurrency& curSrc) {
    m_cur = curSrc.m_cur;
    m_status = curSrc.m_status;
}
COleCurrency& COleCurrency::operator=(const COleCurrency& curSrc) {
    m_cur = curSrc.m_cur;
    m_status = curSrc.m_status;
    return *this;
}
void COleCurrency::SetCurrency(long nUnits, long nFractionalUnits) {
    m_cur.int64 = ((long long)nUnits * 10000) + nFractionalUnits;
    m_status = CY_VALID;
}
