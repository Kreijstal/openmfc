// OpenMFC True Drop-In Test
//
// This test builds an MSVC-ABI binary that links against mfc140u.dll.
// It uses OpenMFC headers for declarations but imports symbols with
// their real MSVC-mangled names as exported by mfc140u.dll.
// At runtime, we swap mfc140u.dll with openmfc.dll to verify drop-in compatibility.
//
// Build (clang-cl):
//   clang-cl /nologo /EHsc /MD /std:c++17 /D_AFXDLL /D_TEST_DROPIN \
//     -I../../include /I<msvc_include> /I<ucrt_include> /I<um_include> \
//     test_dropin.cpp /link mfc140u.lib user32.lib kernel32.lib \
//     /OUT:test_dropin.exe /SUBSYSTEM:WINDOWS
//
// At runtime:
//   cp openmfc.dll mfc140u.dll
//   wine test_dropin.exe

#include <cstdio>
#include <cstdlib>
#include <windows.h>

// Forward-declare MFC exports we need using their real MSVC-mangled names.
// These must match exactly what mfc140u.dll exports.

// AfxGetApp
extern "C" __declspec(dllimport) void* __stdcall AfxGetApp_import();
// We use GetProcAddress for flexibility
typedef void* (*PFN_AfxGetApp)();

// CObject::classCObject (CRuntimeClass static)
extern "C" __declspec(dllimport) void* classCObject;
extern "C" __declspec(dllimport) void* classCWnd;
extern "C" __declspec(dllimport) void* classCWinApp;
extern "C" __declspec(dllimport) void* classCWinThread;
extern "C" __declspec(dllimport) void* classCFrameWnd;
extern "C" __declspec(dllimport) void* classCException;
extern "C" __declspec(dllimport) void* classCMemoryException;

static int g_tests = 0;
static int g_passed = 0;
static int g_failed = 0;

#define TEST(name, expr) do { \
    g_tests++; \
    if (expr) { \
        g_passed++; \
        printf("  [PASS] %s\n", name); \
    } else { \
        g_failed++; \
        printf("  [FAIL] %s\n", name); \
    } \
} while(0)

// Minimal MFC structures we need (must match binary layout)
struct CRuntimeClass {
    const char* m_lpszClassName;
    int m_nObjectSize;
    unsigned short m_wSchema;
    void* m_pfnConstructObject;
    void* m_pBaseClass;
    void* m_pNextClass;
};

struct AFX_MSGMAP_ENTRY {
    unsigned int nID;
    unsigned int nLastID;
    unsigned int nSig;
    void* pfn;
};

struct AFX_MSGMAP {
    const AFX_MSGMAP* pBaseMap;
    const AFX_MSGMAP_ENTRY* lpEntries;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    (void)hPrevInstance;
    (void)lpCmdLine;

    printf("=== OpenMFC True Drop-In Test ===\n\n");

    HMODULE hMFC = GetModuleHandleA("mfc140u.dll");
    if (!hMFC) {
        hMFC = LoadLibraryA("mfc140u.dll");
    }
    TEST("mfc140u.dll loaded", hMFC != NULL);
    if (!hMFC) {
        printf("FATAL: Cannot find mfc140u.dll\n");
        return 1;
    }

    // --- Test 1: Version ---
    printf("\n--- Version Function ---\n");
    typedef unsigned long (*PFN_AfxGetDllVersion)();
    PFN_AfxGetDllVersion pfnVersion = (PFN_AfxGetDllVersion)GetProcAddress(hMFC, "?AfxGetDllVersion@@YAKXZ");
    TEST("?AfxGetDllVersion@@YAKXZ exported", pfnVersion != NULL);
    if (pfnVersion) {
        unsigned long ver = pfnVersion();
        TEST("AfxGetDllVersion() returns nonzero", ver != 0);
        printf("    Version: 0x%08lX\n", ver);
    }

    // --- Test 2: Global static data (classCObject) ---
    printf("\n--- Global Static Members ---\n");
    void* pClassCObject = GetProcAddress(hMFC, "?classCObject@CObject@@2UCRuntimeClass@@A");
    TEST("classCObject exported", pClassCObject != NULL);
    if (pClassCObject) {
        CRuntimeClass* pRC = (CRuntimeClass*)pClassCObject;
        TEST("classCObject->m_lpszClassName valid", pRC->m_lpszClassName != NULL);
        if (pRC->m_lpszClassName) {
            printf("    Class name: %s\n", pRC->m_lpszClassName);
        }
        TEST("classCObject->m_nObjectSize == 8", pRC->m_nObjectSize == 8);
    }

    void* pClassCWnd = GetProcAddress(hMFC, "?classCWnd@CWnd@@2UCRuntimeClass@@A");
    TEST("classCWnd exported", pClassCWnd != NULL);
    if (pClassCWnd) {
        CRuntimeClass* pRC = (CRuntimeClass*)pClassCWnd;
        TEST("classCWnd->m_lpszClassName valid", pRC->m_lpszClassName != NULL);
        if (pRC->m_lpszClassName) {
            printf("    Class name: %s\n", pRC->m_lpszClassName);
        }
    }

    void* pClassCWinApp = GetProcAddress(hMFC, "?classCWinApp@CWinApp@@2UCRuntimeClass@@A");
    TEST("classCWinApp exported", pClassCWinApp != NULL);

    void* pClassCFrameWnd = GetProcAddress(hMFC, "?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A");
    TEST("classCFrameWnd exported", pClassCFrameWnd != NULL);

    void* pClassCException = GetProcAddress(hMFC, "?classCException@CException@@2UCRuntimeClass@@A");
    TEST("classCException exported", pClassCException != NULL);

    void* pClassCMemoryException = GetProcAddress(hMFC, "?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A");
    TEST("classCMemoryException exported", pClassCMemoryException != NULL);

    // --- Test 3: Exception throw/catch ---
    printf("\n--- Exception Throw/Catch ---\n");
    typedef void (*PFN_AfxThrowMemoryException)();
    PFN_AfxThrowMemoryException pfnThrowMem = (PFN_AfxThrowMemoryException)GetProcAddress(hMFC, "?AfxThrowMemoryException@@YAXXZ");
    TEST("AfxThrowMemoryException exported", pfnThrowMem != NULL);

    typedef void (*PFN_AfxThrowFileException)(int, long, const wchar_t*);
    PFN_AfxThrowFileException pfnThrowFile = (PFN_AfxThrowFileException)GetProcAddress(hMFC, "?AfxThrowFileException@@YAXHJPEB_W@Z");
    TEST("AfxThrowFileException exported", pfnThrowFile != NULL);

    // --- Test 4: Dynamic creation ---
    printf("\n--- Dynamic Object Creation ---\n");
    typedef void* (*PFN_CreateObject)(void*); // CRuntimeClass::CreateObject
    PFN_CreateObject pfnCreateObject = (PFN_CreateObject)GetProcAddress(hMFC, "?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ");
    TEST("CRuntimeClass::CreateObject exported", pfnCreateObject != NULL);
    if (pfnCreateObject && pClassCObject) {
        void* pObj = pfnCreateObject(pClassCObject);
        TEST("CreateObject(CObject) returns non-null", pObj != NULL);
    }

    // --- Test 5: IsDerivedFrom ---
    printf("\n--- RTTI ---\n");
    typedef int (*PFN_IsDerivedFrom)(void*, const void*);
    PFN_IsDerivedFrom pfnIsDerivedFrom = (PFN_IsDerivedFrom)GetProcAddress(hMFC, "?IsDerivedFrom@CRuntimeClass@@QEBAHPEBU1@@Z");
    TEST("CRuntimeClass::IsDerivedFrom exported", pfnIsDerivedFrom != NULL);
    if (pfnIsDerivedFrom && pClassCWnd && pClassCObject) {
        int derived = pfnIsDerivedFrom(pClassCWnd, pClassCObject);
        TEST("CWnd IsDerivedFrom CObject", derived != 0);
    }

    // --- Test 6: CString test ---
    printf("\n--- CString ---\n");
    typedef void (*PFN_AfxFormatString1)(void*, unsigned int, const wchar_t*);
    PFN_AfxFormatString1 pfnFormatString1 = (PFN_AfxFormatString1)GetProcAddress(hMFC, "?AfxFormatString1@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL"
      "@@@@@ATL@@IPEB_W@Z");
    TEST("AfxFormatString1 exported", pfnFormatString1 != NULL);

    // --- Test 7: Message box ---
    printf("\n--- UI Functions ---\n");
    typedef int (*PFN_AfxMessageBox)(unsigned int, unsigned int, unsigned int);
    PFN_AfxMessageBox pfnMessageBox = (PFN_AfxMessageBox)GetProcAddress(hMFC, "?AfxMessageBox@@YAHIII@Z");
    TEST("AfxMessageBox exported", pfnMessageBox != NULL);

    // --- Summary ---
    printf("\n========================================\n");
    printf("Results: %d/%d passed, %d failed\n", g_passed, g_tests, g_failed);
    printf("========================================\n");
    fflush(stdout);

    if (g_failed > 0) {
        return 1;
    }

    printf("\n[SUCCESS] OpenMFC drop-in test PASSED!\n");
    return 0;
}
