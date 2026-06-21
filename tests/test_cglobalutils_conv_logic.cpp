// Behavioral test for CGlobalUtils::CyFromString / DecimalFromString.
// Drives the impl_ thunks against real oleaut32 Variant parsing under Wine.

#include <windows.h>
#include <oleauto.h>
#include <cstdio>
#include <cmath>

#include "../phase4/src/global_cglobalutils_conv.cpp"

static int g_failures = 0;
static void check(bool cond, const char* what) {
    std::printf("%s: %s\n", cond ? "PASS" : "FAIL", what);
    if (!cond) ++g_failures;
}

int main() {
    // The impl faithfully parses using GetThreadLocale() (as MFC does); pin a
    // locale with '.' as the decimal separator so the inputs are deterministic
    // regardless of the host's default locale (Wine defaults to de-DE).
    SetThreadLocale(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));

    int dummy = 0;
    void* self = &dummy;  // unused pThis

    // ----- CyFromString -----
    CY cy; cy.int64 = -1;
    int ok = impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(self, &cy, L"123.45");
    check(ok == 1, "CyFromString(\"123.45\") returns TRUE");
    check(cy.int64 == 1234500, "CY scaled by 10000 (123.45 -> 1234500)");

    CY cyNeg; cyNeg.int64 = 0;
    impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(self, &cyNeg, L"-2.5");
    check(cyNeg.int64 == -25000, "CY negative (-2.5 -> -25000)");

    CY cyBad; cyBad.int64 = 999;
    int bad = impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(self, &cyBad, L"not a number");
    check(bad == 0, "CyFromString of garbage returns FALSE");

    int nullret = impl__CyFromString_CGlobalUtils__QEAAHAEATtagCY__PEB_W_Z(self, &cy, nullptr);
    check(nullret == 0, "CyFromString(nullptr) returns FALSE");

    // ----- DecimalFromString -----
    DECIMAL dec;
    int okd = impl__DecimalFromString_CGlobalUtils__QEAAHAEAUtagDEC__PEB_W_Z(self, &dec, L"123.45");
    check(okd == 1, "DecimalFromString(\"123.45\") returns TRUE");
    double d = 0;
    VarR8FromDec(&dec, &d);
    check(std::fabs(d - 123.45) < 1e-9, "DECIMAL round-trips to 123.45");

    DECIMAL decBig;
    impl__DecimalFromString_CGlobalUtils__QEAAHAEAUtagDEC__PEB_W_Z(self, &decBig, L"1000000.001");
    double db = 0;
    VarR8FromDec(&decBig, &db);
    check(std::fabs(db - 1000000.001) < 1e-6, "DECIMAL preserves precision (1000000.001)");

    DECIMAL decBad;
    int badd = impl__DecimalFromString_CGlobalUtils__QEAAHAEAUtagDEC__PEB_W_Z(self, &decBad, L"xyz");
    check(badd == 0, "DecimalFromString of garbage returns FALSE");

    std::printf("RESULT: %s (%d failures)\n", g_failures == 0 ? "ALL PASS" : "FAILED", g_failures);
    return g_failures == 0 ? 0 : 1;
}
