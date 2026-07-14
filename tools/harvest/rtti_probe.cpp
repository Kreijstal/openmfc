// Harvest real CRuntimeClass values (sizeof / schema / base) for each target
// class by calling its exported GetThisClass in a loaded mfc140u.dll, and (when
// a second DLL is given) detect whether our build still stubs it.
//
// Usage:  rtti_probe.exe <real_mfc140u.dll> [our_openmfc.dll]
// Emits one JSON object per line: {"class","sizeof","schema","base","stub"}.
//
// Real mfc140u is built _AFXDLL, so CRuntimeClass uses m_pfnGetBaseClass (a
// function pointer) rather than m_pBaseClass. Layout (x64):
//   +0  LPCSTR m_lpszClassName
//   +8  int    m_nObjectSize
//   +12 UINT   m_wSchema
//   +16 void*  m_pfnCreateObject
//   +24 void*  m_pfnGetBaseClass   (_AFXDLL)
//   +32 void*  m_pNextClass
#include <windows.h>
#include <cstdio>
#include <cstring>

struct RTC {
    const char* m_lpszClassName;
    int         m_nObjectSize;
    unsigned    m_wSchema;
    void*       m_pfnCreateObject;
    RTC*      (*m_pfnGetBaseClass)();
    void*       m_pNextClass;
};
typedef RTC* (*GetThisFn)();

#include "rtti_targets.h"

static RTC* call_getthis(HMODULE h, int ord) {
    GetThisFn fn = (GetThisFn)GetProcAddress(h, MAKEINTRESOURCEA(ord));
    if (!fn) return nullptr;
    return fn();
}

int main(int argc, char** argv) {
    if (argc < 2) { fprintf(stderr, "need real mfc140u path\n"); return 2; }
    HMODULE real = LoadLibraryExA(
        argv[1], nullptr,
        LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    if (!real) { fprintf(stderr, "LoadLibrary real failed %lu\n", GetLastError()); return 2; }
    HMODULE ours = (argc >= 3)
        ? LoadLibraryExA(argv[2], nullptr,
                         LOAD_LIBRARY_SEARCH_DLL_LOAD_DIR | LOAD_LIBRARY_SEARCH_DEFAULT_DIRS)
        : nullptr;
    if (argc >= 3 && !ours) {
        fprintf(stderr, "LoadLibrary ours failed %lu\n", GetLastError());
        return 2;
    }

    for (int i = 0; i < g_ntargets; ++i) {
        const Target& t = g_targets[i];
        RTC* r = call_getthis(real, t.ord_gt);
        if (!r) { printf("{\"class\":\"%s\",\"error\":\"no real getthis\"}\n", t.name); continue; }
        if (!r->m_lpszClassName || strcmp(r->m_lpszClassName, t.name) != 0) {
            printf("{\"class\":\"%s\",\"error\":\"descriptor class mismatch\"}\n", t.name);
            continue;
        }
        const char* base = "?";
        RTC* b = r->m_pfnGetBaseClass ? r->m_pfnGetBaseClass() : nullptr;
        if (b && b->m_lpszClassName) base = b->m_lpszClassName;
        else if (!b) base = "(null)";

        // stub detection on our DLL: a real impl returns a descriptor whose
        // class name matches; a stub returns null or a wrong/empty name.
        int stub = -1;
        if (ours) {
            RTC* o = call_getthis(ours, t.ord_gt);
            if (!o || !o->m_lpszClassName) stub = 1;
            else stub = (strcmp(o->m_lpszClassName, t.name) == 0 && o->m_nObjectSize != 0) ? 0 : 1;
        }
        printf("{\"class\":\"%s\",\"sizeof\":%d,\"schema\":%u,\"base\":\"%s\",\"stub\":%d}\n",
               t.name, r->m_nObjectSize, r->m_wSchema, base, stub);
        fflush(stdout);
    }
    return 0;
}
