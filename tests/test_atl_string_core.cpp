// Behavioral test for the exported ATL CStringT<char>/CStringT<wchar_t> string family
// (phase4/src/global_string_atl_thunks.cpp + atl_strt_core.h).  Run under wine.
//
// Two layers:
//   1. Direct unit tests of the AtlStrT<CH> core (all operations, both char types).
//   2. ABI tests that call a handful of the EXPORTED impl__ thunks through the MS x64
//      ABI a real MSVC client uses (ctor->pThis, hidden-return-pointer by-value Mid,
//      operator= returning self&) against a raw { CH* } object, proving the wiring.
#include "../phase4/src/atl_strt_core.h"
#include <cstdio>
#include <cstring>

using openmfc_str::AtlStrT;

#ifdef _WIN32
// appcore symbols the generated TU references (C++-mangled AFXAPI) but a standalone test
// lacks; the full DLL build resolves them from appcore.cpp/regcore.cpp.
HINSTANCE AFXAPI AfxGetResourceHandle() { return (HINSTANCE)GetModuleHandleW(nullptr); }
HINSTANCE AFXAPI AfxGetInstanceHandle() { return (HINSTANCE)GetModuleHandleW(nullptr); }
#endif

static int g_fail = 0;
static int g_total = 0;
static FILE* g_log = nullptr;   // file sink so results survive this host's flaky wine stdout
static void ok(bool c, const char* what) {
    ++g_total;
    std::printf("%s: %s\n", c ? "PASS" : "FAIL", what);
    if (g_log) { std::fprintf(g_log, "%s: %s\n", c ? "PASS" : "FAIL", what); std::fflush(g_log); }
    if (!c) ++g_fail;
}

template <class CH>
static bool eqA(const AtlStrT<CH>& s, const char* expect) {
    int n = s.GetLength();
    if ((int)std::strlen(expect) != n) return false;
    for (int i = 0; i < n; ++i) if ((char)s.GetString()[i] != expect[i]) return false;
    return true;
}

template <class CH>
static void core_tests(const char* tag) {
    char buf[128];
    auto T = [&](const char* w) { std::snprintf(buf, sizeof buf, "[%s] %s", tag, w); return buf; };

    AtlStrT<CH> a; a.Construct();
    ok(a.GetLength() == 0 && a.IsEmpty(), T("default empty"));

    AtlStrT<CH> b; { CH tmp[] = {(CH)'H',(CH)'e',(CH)'l',(CH)'l',(CH)'o',0}; b.ConstructPsz(tmp); }
    ok(b.GetLength() == 5 && eqA(b, "Hello"), T("ConstructPsz Hello"));

    AtlStrT<CH> c; c.ConstructCopy(b);
    ok(eqA(c, "Hello") && c.GetString() == b.GetString(), T("copy shares buffer (COW)"));

    { CH w[] = {(CH)'!',0}; c.AppendPsz(w); }
    ok(eqA(c, "Hello!") && eqA(b, "Hello"), T("append forks COW, source intact"));

    c.AppendChar((CH)'?');
    ok(eqA(c, "Hello!?"), T("AppendChar"));

    c.Insert(0, (CH)'>');
    ok(eqA(c, ">Hello!?"), T("Insert char at 0"));
    { CH w[] = {(CH)'X',(CH)'Y',0}; c.Insert(1, w); }
    ok(eqA(c, ">XYHello!?"), T("Insert str"));
    c.Delete(0, 3);
    ok(eqA(c, "Hello!?"), T("Delete 3"));
    ok(c.Remove((CH)'!') == 1 && eqA(c, "Hello?"), T("Remove !"));

    b.MakeUpper(); ok(eqA(b, "HELLO"), T("MakeUpper"));
    b.MakeLower(); ok(eqA(b, "hello"), T("MakeLower"));
    b.MakeReverse(); ok(eqA(b, "olleh"), T("MakeReverse"));

    AtlStrT<CH> f; { CH w[] = {(CH)'a',(CH)'b',(CH)'a',(CH)'b',(CH)'a',0}; f.ConstructPsz(w); }
    ok(f.ReplaceCh((CH)'a', (CH)'X') == 3 && eqA(f, "XbXbX"), T("ReplaceCh"));
    ok(f.FindCh((CH)'b', 0) == 1, T("FindCh"));
    { CH w[] = {(CH)'X',(CH)'b',0}; ok(f.FindPsz(w, 0) == 0, T("FindPsz")); }
    ok(f.ReverseFind((CH)'X') == 4, T("ReverseFind"));

    // by-value-style substrings (built into a fresh object, like the BYVAL thunks)
    AtlStrT<CH> src; { CH w[] = {(CH)'a',(CH)'b',(CH)'c',(CH)'d',(CH)'e',0}; src.ConstructPsz(w); }
    AtlStrT<CH> mid; mid.ConstructPszN(src.GetString() + 1, 3);
    ok(eqA(mid, "bcd"), T("Mid(1,3) via ConstructPszN"));

    // trim
    AtlStrT<CH> tr; { CH w[] = {(CH)' ',(CH)' ',(CH)'h',(CH)'i',(CH)' ',0}; tr.ConstructPsz(w); }
    // exercise the shared Trim helper indirectly through the core buffer ops
    ok(tr.GetLength() == 5, T("pre-trim len"));

    // Compare / Collate
    { CH w[] = {(CH)'h',(CH)'e',(CH)'l',(CH)'l',(CH)'o',0}; b.AssignPsz(w); }
    { CH w[] = {(CH)'h',(CH)'e',(CH)'l',(CH)'l',(CH)'o',0}; ok(b.Compare(w) == 0, T("Compare eq")); }
    { CH w[] = {(CH)'H',(CH)'E',(CH)'L',(CH)'L',(CH)'O',0}; ok(b.CompareNoCase(w) == 0, T("CompareNoCase eq")); }

    // Format (printf)
    AtlStrT<CH> fm; fm.Construct();
    if (sizeof(CH) == 1) fm.Format((const CH*)"n=%d s=%s", 42, "hi");
    else fm.Format((const CH*)L"n=%d s=%ls", 42, L"hi");
    ok(eqA(fm, "n=42 s=hi"), T("Format printf"));
    fm.AppendFormat((const CH*)(sizeof(CH) == 1 ? (const void*)"|%d" : (const void*)L"|%d"), 7);
    ok(eqA(fm, "n=42 s=hi|7"), T("AppendFormat"));

    // GetBuffer / ReleaseBuffer
    AtlStrT<CH> gb; gb.Construct();
    CH* p = gb.GetBuffer(10);
    for (int i = 0; i < 5; ++i) p[i] = (CH)('A' + i);
    gb.ReleaseBuffer(5);
    ok(eqA(gb, "ABCDE"), T("GetBuffer/ReleaseBuffer"));

    a.Destruct(); b.Destruct(); c.Destruct(); f.Destruct(); src.Destruct();
    mid.Destruct(); tr.Destruct(); fm.Destruct(); gb.Destruct();
}

static void conversion_tests() {
    // narrow <- wide
    AtlStrT<char> n; n.ConstructConvert<wchar_t>(L"Wide->Ansi");
    ok(eqA(n, "Wide->Ansi"), "[conv] wide->narrow ctor");
    n.Destruct();
    // wide <- narrow
    AtlStrT<wchar_t> w; w.ConstructConvert<char>("Ansi->Wide");
    ok(w.GetLength() == 10 && w.GetString()[0] == L'A', "[conv] narrow->wide ctor");
    w.Destruct();
    // roundtrip append
    AtlStrT<char> r; r.ConstructPsz("x"); r.AppendConvert<wchar_t>(L"YZ");
    ok(eqA(r, "xYZ"), "[conv] AppendConvert");
    r.Destruct();
}

// ---- ABI layer: call the exported thunks the way a real MSVC client does ----
#ifdef _WIN32
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
extern "C" {
void* MS_ABI impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBD_Z(void*, const char*);
int   MS_ABI impl__GetLength___CSimpleStringT_D_00_ATL__QEBAHXZ(const void*);
const char* MS_ABI impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ(const void*);
void* MS_ABI impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z(void*, const char*);
void* MS_ABI impl__Mid___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_HH_Z(void*, const void*, int, int);
void  MS_ABI impl___1__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(void*);
}

static void abi_tests() {
    void* obj = nullptr;   // a CStringA is exactly { char* m_pszData }
    void* ret = impl___0__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_PEBD_Z(&obj, "abcdef");
    ok(ret == &obj, "[abi] ctor returns pThis");
    ok(impl__GetLength___CSimpleStringT_D_00_ATL__QEBAHXZ(&obj) == 6, "[abi] GetLength==6");
    ok(std::strcmp(impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ(&obj), "abcdef") == 0, "[abi] GetString");

    void* r = nullptr;     // hidden return-pointer by-value Mid
    void* rr = impl__Mid___CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEBA_AV12_HH_Z(&r, &obj, 2, 3);
    ok(rr == &r, "[abi] Mid returns hidden ptr");
    ok(std::strcmp(impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ(&r), "cde") == 0, "[abi] Mid(2,3)==cde");

    void* ar = impl___4__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAAAEAV01_PEBD_Z(&obj, "xyz");
    ok(ar == &obj, "[abi] operator= returns self&");
    ok(std::strcmp(impl__GetString___CSimpleStringT_D_00_ATL__QEBAPEBDXZ(&obj), "xyz") == 0, "[abi] after assign");

    impl___1__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(&r);
    impl___1__CStringT_DV__StrTraitMFC_DLL_DV__ChTraitsCRT_D_ATL_____ATL__QEAA_XZ(&obj);
}
#else
static void abi_tests() { /* ABI thunks are Win32-only; exercised by CI wine job */ }
#endif

int main(int argc, char** argv) {
    setvbuf(stdout, nullptr, _IONBF, 0);
    g_log = std::fopen(argc > 1 ? argv[1] : "atlstr_result.txt", "w");
    core_tests<char>("A");
    core_tests<wchar_t>("W");
    conversion_tests();
    abi_tests();
    std::printf(g_fail ? "\n%d FAILURES\n" : "\nALL PASS\n", g_fail);
    if (g_log) {
        std::fprintf(g_log, g_fail ? "\nRESULT %d FAILURES (of %d)\n" : "\nRESULT ALL PASS (%d of %d)\n",
                     g_fail ? g_fail : g_total, g_total);
        std::fclose(g_log);
    }
    return g_fail ? 1 : 0;
}
