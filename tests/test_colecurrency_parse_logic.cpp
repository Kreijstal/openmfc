// Behavioral test for COleCurrency::ParseCurrency.
// Drives the impl_ thunk against a COleCurrency-layout buffer and verifies the
// parsed CY value and status under Wine (real VarCyFromStr).

#include <windows.h>
#include <oleauto.h>
#include <cstdio>

#include "../phase4/src/ole_colecurrency_parse.cpp"

// COleCurrency layout: CY m_cur @0, int m_status @8 (sizeof 32 with padding).
struct OCC { CY m_cur; int m_status; char pad[20]; };
static_assert(sizeof(OCC) == 32, "COleCurrency is 32 bytes");

static int g_failures = 0;
static void check(bool cond, const char* what) {
    std::printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

int main() {
    // Pin a '.'-decimal locale (Wine defaults to de-DE which uses ',').
    LCID en = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);

    OCC cc; cc.m_cur.int64 = -1; cc.m_status = 99;
    int ok = impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(&cc, L"123.45", 0, en);
    check(ok == 1, "ParseCurrency(\"123.45\") returns TRUE");
    check(cc.m_cur.int64 == 1234500, "CY scaled by 10000 (123.45 -> 1234500)");
    check(cc.m_status == 0, "status == CY_VALID on success");

    OCC neg;
    impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(&neg, L"-2.5", 0, en);
    check(neg.m_cur.int64 == -25000, "CY negative (-2.5 -> -25000)");

    OCC bad; bad.m_cur.int64 = 777; bad.m_status = 0;
    int br = impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(&bad, L"garbage", 0, en);
    check(br == 0, "ParseCurrency of garbage returns FALSE");
    check(bad.m_status == 1, "status == CY_INVALID on failure");
    check(bad.m_cur.int64 == 0, "CY zeroed on failure");

    OCC nul;
    int nr = impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(&nul, nullptr, 0, en);
    check(nr == 0, "ParseCurrency(nullptr) returns FALSE");

    // lcid==0 falls back to the thread locale (set it to en-US to stay deterministic).
    SetThreadLocale(en);
    OCC dl;
    int dr = impl__ParseCurrency_COleCurrency__QEAAHPEB_WKK_Z(&dl, L"10.00", 0, 0);
    check(dr == 1 && dl.m_cur.int64 == 100000, "lcid==0 uses thread locale (10.00 -> 100000)");

    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
