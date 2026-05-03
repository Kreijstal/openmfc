#!/usr/bin/env bash
set -euo pipefail

# OpenMFC True Drop-In Test (simplified)
# Builds a minimal C binary that loads mfc140u.dll and tests
# the exported symbols via GetProcAddress.
#
# The binary imports __declspec(dllimport) symbols from mfc140u.dll
# using MSVC-mangled names. At runtime we swap openmfc.dll -> mfc140u.dll.
#
# Usage:
#   ./phase4/tests/build_and_test_dropin.sh

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PHASE4_DIR="${REPO_ROOT}/phase4"
BUILD_DIR="${REPO_ROOT}/build-phase4"
TEST_DIR="${PHASE4_DIR}/tests"
WORK_DIR="${WORK_DIR:-/tmp/openmfc_dropin_test}"

echo "=== OpenMFC True Drop-In Test ==="

# -----------------------------------------------------------------------
# 1. Ensure DLL is built
# -----------------------------------------------------------------------
OPENMFC_DLL="${BUILD_DIR}/openmfc.dll"
OPENMFC_DEF="${BUILD_DIR}/openmfc.def"

if [[ ! -f "${OPENMFC_DLL}" ]]; then
    echo "Building OpenMFC DLL..."
    bash "${PHASE4_DIR}/scripts/build_phase4.sh"
fi

if [[ ! -f "${OPENMFC_DLL}" ]]; then
    echo "FATAL: openmfc.dll not found"
    exit 1
fi
echo "DLL: ${OPENMFC_DLL}"

# -----------------------------------------------------------------------
# 2. Generate mfc140u.lib from our .def file
#    This .lib will have MSVC-mangled import names, so the EXE
#    will look for mfc140u.dll with the real MSVC export names.
# -----------------------------------------------------------------------
rm -rf "${WORK_DIR}"
mkdir -p "${WORK_DIR}"

echo "Generating mfc140u.lib from openmfc.def..."
llvm-lib "/def:${OPENMFC_DEF}" "/out:${WORK_DIR}/mfc140u.lib" /machine:x64

# -----------------------------------------------------------------------
# 3. Write a minimal C test that only uses Win32 API + GetProcAddress
#    No MFC C++ headers needed — tests the DLL at the ABI level.
# -----------------------------------------------------------------------
cat > "${WORK_DIR}/test_dropin.c" << 'CEOF'
#include <windows.h>
#include <stdio.h>

static int g_tests = 0, g_passed = 0, g_failed = 0;
#define TEST(name, expr) do { \
    g_tests++; if (expr) { g_passed++; printf("  [PASS] %s\n", name); } \
    else { g_failed++; printf("  [FAIL] %s\n", name); } } while(0)

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int nShow)
{
    (void)hInst; (void)hPrev; (void)cmd; (void)nShow;
    printf("=== OpenMFC True Drop-In Test ===\n\n");

    HMODULE hMFC = LoadLibraryA("mfc140u.dll");
    TEST("mfc140u.dll loaded", hMFC != NULL);
    if (!hMFC) { printf("FATAL\n"); return 1; }

    /* ---- Version ---- */
    printf("\n--- Version ---\n");
    typedef unsigned long (*PFN_Ver)(void);
    PFN_Ver fnVer = (PFN_Ver)GetProcAddress(hMFC, "?AfxGetDllVersion@@YAKXZ");
    TEST("AfxGetDllVersion exported", fnVer != NULL);
    if (fnVer) {
        unsigned long v = fnVer();
        TEST("AfxGetDllVersion nonzero", v != 0);
        printf("    Version: 0x%08lX\n", v);
    }

    /* ---- Static CRuntimeClass data ---- */
    printf("\n--- Static CRuntimeClass ---\n");
    void* pCO = GetProcAddress(hMFC, "?classCObject@CObject@@2UCRuntimeClass@@A");
    TEST("classCObject exported", pCO != NULL);
    if (pCO) {
        const char** name = (const char**)pCO;
        TEST("CObject class name valid", *name != NULL);
        if (*name) printf("    CObject: %s\n", *name);
    }

    void* pCW = GetProcAddress(hMFC, "?classCWnd@CWnd@@2UCRuntimeClass@@A");
    TEST("classCWnd exported", pCW != NULL);

    void* pCApp = GetProcAddress(hMFC, "?classCWinApp@CWinApp@@2UCRuntimeClass@@A");
    TEST("classCWinApp exported", pCApp != NULL);

    void* pCFW = GetProcAddress(hMFC, "?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A");
    TEST("classCFrameWnd exported", pCFW != NULL);

    void* pCExc = GetProcAddress(hMFC, "?classCException@CException@@2UCRuntimeClass@@A");
    TEST("classCException exported", pCExc != NULL);

    void* pCMem = GetProcAddress(hMFC, "?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A");
    TEST("classCMemoryException exported", pCMem != NULL);

    /* ---- RTTI: CreateObject (only for DYNCREATE classes) ---- */
    printf("\n--- RTTI: CreateObject ---\n");
    typedef void* (*PFN_CreateObj)(void*);
    PFN_CreateObj fnCreate = (PFN_CreateObj)GetProcAddress(hMFC, "?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ");
    TEST("CRuntimeClass::CreateObject exported", fnCreate != NULL);
    /* CWnd uses IMPLEMENT_DYNAMIC (not DYNCREATE), so CreateObject returns null */
    if (fnCreate && pCW) {
        void* obj = fnCreate(pCW);
        printf("    CWnd::CreateObject = %p (expected null: IMPLEMENT_DYNAMIC)\n", obj);
    }
    /* CFrameWnd uses IMPLEMENT_DYNCREATE, so CreateObject should work */
    if (fnCreate && pCFW) {
        void* obj = fnCreate(pCFW);
        TEST("CFrameWnd::CreateObject non-null", obj != NULL);
    }

    /* ---- RTTI: IsDerivedFrom ---- */
    printf("\n--- RTTI: IsDerivedFrom ---\n");
    typedef int (*PFN_IsDerived)(void*, const void*);
    PFN_IsDerived fnIsDerived = (PFN_IsDerived)GetProcAddress(hMFC, "?IsDerivedFrom@CRuntimeClass@@QEBAHPEBU1@@Z");
    TEST("IsDerivedFrom exported", fnIsDerived != NULL);
    if (fnIsDerived && pCW && pCO) {
        TEST("CWnd derived from CObject", fnIsDerived(pCW, pCO) != 0);
    }

    /* ---- Exception throw/catch ---- */
    printf("\n--- Exception Functions ---\n");
    typedef void (*PFN_ThrowMem)(void);
    PFN_ThrowMem fnThrowMem = (PFN_ThrowMem)GetProcAddress(hMFC, "?AfxThrowMemoryException@@YAXXZ");
    TEST("AfxThrowMemoryException exported", fnThrowMem != NULL);

    typedef void (*PFN_ThrowFile)(int, long, const wchar_t*);
    PFN_ThrowFile fnThrowFile = (PFN_ThrowFile)GetProcAddress(hMFC, "?AfxThrowFileException@@YAXHJPEB_W@Z");
    TEST("AfxThrowFileException exported", fnThrowFile != NULL);

    /* ---- AfxMessageBox ---- */
    printf("\n--- UI: AfxMessageBox ---\n");
    typedef int (*PFN_MsgBox)(unsigned int, unsigned int, unsigned int);
    PFN_MsgBox fnMsgBox = (PFN_MsgBox)GetProcAddress(hMFC, "?AfxMessageBox@@YAHIII@Z");
    TEST("AfxMessageBox exported", fnMsgBox != NULL);

    /* ---- CString helpers ---- */
    printf("\n--- String Functions ---\n");
    typedef void (*PFN_FmtStr1)(void*, unsigned int, const wchar_t*);
    PFN_FmtStr1 fnFmt1 = (PFN_FmtStr1)GetProcAddress(hMFC,
      "?AfxFormatString1@@YAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W@Z");
    TEST("AfxFormatString1 exported", fnFmt1 != NULL);

    /* ---- Operator new/delete (ABI critical) ---- */
    printf("\n--- Global operators ---\n");
    typedef void* (*PFN_New)(size_t);
    PFN_New fnNew = (PFN_New)GetProcAddress(hMFC, "??2@YAPEAX_K@Z");
    TEST("operator new exported", fnNew != NULL);
    if (fnNew) {
        void* p = fnNew(64);
        TEST("operator new works", p != NULL);
    }

    typedef void (*PFN_Delete)(void*);
    PFN_Delete fnDel = (PFN_Delete)GetProcAddress(hMFC, "??3@YAXPEAX@Z");
    TEST("operator delete exported", fnDel != NULL);

    /* ---- CWnd methods ---- */
    printf("\n--- CWnd Methods ---\n");
    /* GetSafeHwnd is inline in MFC, not exported */

    PFN_ThrowMem fnShowWindow = (PFN_ThrowMem)GetProcAddress(hMFC, "?ShowWindow@CWnd@@QEAAHH@Z");
    TEST("CWnd::ShowWindow exported", fnShowWindow != NULL);

    /* ---- Summary ---- */
    printf("\n========================================\n");
    printf("  %d/%d passed, %d failed\n", g_passed, g_tests, g_failed);
    printf("========================================\n");
    return g_failed > 0 ? 1 : 0;
}
CEOF

# -----------------------------------------------------------------------
# 4. Build test binary with x86_64-w64-mingw32-gcc
#    Links against mfc140u.lib so it imports from mfc140u.dll
# -----------------------------------------------------------------------
echo "Building test_dropin.exe (MinGW)..."

x86_64-w64-mingw32-gcc \
    -O2 -s \
    -mwindows \
    "${WORK_DIR}/test_dropin.c" \
    "${WORK_DIR}/mfc140u.lib" \
    -o "${WORK_DIR}/test_dropin.exe"

echo "Build OK: ${WORK_DIR}/test_dropin.exe"

# -----------------------------------------------------------------------
# 5. Stage DLL as mfc140u.dll + runtime deps
# -----------------------------------------------------------------------
cp "${OPENMFC_DLL}" "${WORK_DIR}/mfc140u.dll"

MINGW_BIN="${MINGW_BIN_DIR:-/usr/x86_64-w64-mingw32/bin}"
for dll in libwinpthread-1.dll libgcc_s_seh-1.dll libstdc++-6.dll; do
    if [[ -f "${MINGW_BIN}/${dll}" ]]; then
        cp -f "${MINGW_BIN}/${dll}" "${WORK_DIR}/"
    fi
done

# -----------------------------------------------------------------------
# 6. Run under Wine
# -----------------------------------------------------------------------
echo ""
echo "========================================="
echo "Running under Wine..."
echo "========================================="

export WINEDEBUG="${WINEDEBUG:--all}"

(cd "${WORK_DIR}" && timeout 20s wine ./test_dropin.exe)
EXIT_CODE=$?

echo ""
if [[ ${EXIT_CODE} -eq 0 ]]; then
    echo "========================================="
    echo "SUCCESS: Drop-in test PASSED!"
    echo "========================================="
else
    echo "========================================="
    echo "FAILURE: Drop-in test returned ${EXIT_CODE}"
    echo "========================================="
    exit ${EXIT_CODE}
fi
