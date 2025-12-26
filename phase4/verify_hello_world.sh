#!/usr/bin/env bash
set -euo pipefail

PHASE4_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd -- "${PHASE4_DIR}/.." && pwd)"

# ------------------------------------------------------------------------------
# Configuration (override via env vars)
# ------------------------------------------------------------------------------
MSVC_DEST="${MSVC_DEST:-/tmp/msvc}"
MINHOOK_DEST="${MINHOOK_DEST:-/tmp/minhook}"
WORK_DIR="${WORK_DIR:-/tmp/openmfc_verify_hello_world}"

OPENMFC_DLL="${OPENMFC_DLL:-}"
OPENMFC_LIB="${OPENMFC_LIB:-}"
OPENMFC_DEF="${OPENMFC_DEF:-}"

need_cmd() {
  command -v "$1" >/dev/null 2>&1 || {
    echo "FATAL: missing required command: $1" >&2
    exit 1
  }
}

pick_existing() {
  for p in "$@"; do
    if [[ -n "$p" && -f "$p" ]]; then
      printf '%s\n' "$p"
      return 0
    fi
  done
  return 1
}

echo "OpenMFC automated hello-world verification"

# ------------------------------------------------------------------------------
# 0. Tooling checks
# ------------------------------------------------------------------------------
need_cmd git
need_cmd python3
need_cmd wine
need_cmd x86_64-w64-mingw32-gcc

need_cmd timeout

# ------------------------------------------------------------------------------
# 1. Locate OpenMFC artifacts (dll/lib/def)
# ------------------------------------------------------------------------------
if [[ -z "$OPENMFC_DLL" ]]; then
  OPENMFC_DLL="$(pick_existing \
    "${PHASE4_DIR}/openmfc.dll" \
    "${REPO_ROOT}/build-phase4/openmfc.dll" \
    "${REPO_ROOT}/build/openmfc.dll" || true)"
fi
if [[ -z "$OPENMFC_LIB" ]]; then
  OPENMFC_LIB="$(pick_existing \
    "${PHASE4_DIR}/openmfc.lib" \
    "${REPO_ROOT}/build-phase4/openmfc.lib" || true)"
fi
if [[ -z "$OPENMFC_DEF" ]]; then
  OPENMFC_DEF="$(pick_existing \
    "${REPO_ROOT}/build-phase4/openmfc.def" \
    "${REPO_ROOT}/build-phase3/openmfc.def" \
    "${REPO_ROOT}/build/openmfc.def" || true)"
fi

if [[ -z "$OPENMFC_DLL" || ! -f "$OPENMFC_DLL" ]]; then
  echo "FATAL: openmfc.dll not found. Set OPENMFC_DLL or build Phase4 first." >&2
  exit 1
fi

# ------------------------------------------------------------------------------
# 2. Setup MSVC SDK (headers & libs) via msvc-wine
# ------------------------------------------------------------------------------
if [[ -d "${MSVC_DEST}/VC/Tools/MSVC" && -d "${MSVC_DEST}/Windows Kits/10/Include" ]]; then
  echo "MSVC SDK found at ${MSVC_DEST}"
else
  echo "Downloading MSVC SDK into ${MSVC_DEST}..."
  tmp_tools="$(mktemp -d /tmp/msvc-wine.XXXXXX)"
  git clone --depth 1 https://github.com/mstorsjo/msvc-wine.git "${tmp_tools}/msvc-wine"
  python3 "${tmp_tools}/msvc-wine/vsdownload.py" --accept-license --dest "${MSVC_DEST}"
  "${tmp_tools}/msvc-wine/install.sh" "${MSVC_DEST}"
  rm -rf "${tmp_tools}"
fi

export PATH="${MSVC_DEST}/bin/x64:${PATH}"

MSVC_VER="$(ls -1 "${MSVC_DEST}/VC/Tools/MSVC" | sort -V | tail -n 1)"
SDK_VER="$(ls -1 "${MSVC_DEST}/Windows Kits/10/Include" | sort -V | tail -n 1)"

INC_MSVC="${MSVC_DEST}/VC/Tools/MSVC/${MSVC_VER}/include"
INC_UCRT="${MSVC_DEST}/Windows Kits/10/Include/${SDK_VER}/ucrt"
INC_UM="${MSVC_DEST}/Windows Kits/10/Include/${SDK_VER}/um"
INC_SHARED="${MSVC_DEST}/Windows Kits/10/Include/${SDK_VER}/shared"
INC_WINRT="${MSVC_DEST}/Windows Kits/10/Include/${SDK_VER}/winrt"

LIB_MSVC="${MSVC_DEST}/VC/Tools/MSVC/${MSVC_VER}/lib/x64"
LIB_UCRT="${MSVC_DEST}/Windows Kits/10/Lib/${SDK_VER}/ucrt/x64"
LIB_UM="${MSVC_DEST}/Windows Kits/10/Lib/${SDK_VER}/um/x64"

# ------------------------------------------------------------------------------
# 3. Setup MinHook (API spying)
# ------------------------------------------------------------------------------
if [[ -d "${MINHOOK_DEST}/include" && -d "${MINHOOK_DEST}/src" ]]; then
  echo "MinHook found at ${MINHOOK_DEST}"
else
  echo "Cloning MinHook into ${MINHOOK_DEST}..."
  git clone --depth 1 https://github.com/TsudaKageyu/minhook.git "${MINHOOK_DEST}"
fi

# ------------------------------------------------------------------------------
# 4. Generate sources (in /tmp; nothing is written into the repo)
# ------------------------------------------------------------------------------
SRC_DIR="${WORK_DIR}/src"
BIN_DIR="${WORK_DIR}/bin"
mkdir -p "${SRC_DIR}" "${BIN_DIR}"

cat >"${SRC_DIR}/spy.c" <<'EOF'
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "MinHook.h"

static volatile LONG g_CreateCalls = 0;
static volatile LONG g_ShowCalls = 0;

typedef HWND (WINAPI *CREATEWINDOWEXW)(
    DWORD, LPCWSTR, LPCWSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
typedef BOOL (WINAPI *SHOWWINDOW)(HWND, int);

static CREATEWINDOWEXW fpCreateWindowExW = NULL;
static SHOWWINDOW fpShowWindow = NULL;

static HWND WINAPI Detour_CreateWindowExW(
    DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle,
    int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu,
    HINSTANCE hInstance, LPVOID lpParam) {
  InterlockedIncrement(&g_CreateCalls);
  printf("   [SPY] CreateWindowExW\n");
  printf("   [SPY]   ClassName:  %ls\n", lpClassName ? lpClassName : L"(null)");
  printf("   [SPY]   WindowName: %ls\n", lpWindowName ? lpWindowName : L"(null)");
  return fpCreateWindowExW(
      dwExStyle, lpClassName, lpWindowName, dwStyle, x, y, nWidth, nHeight,
      hWndParent, hMenu, hInstance, lpParam);
}

static BOOL WINAPI Detour_ShowWindow(HWND hWnd, int nCmdShow) {
  InterlockedIncrement(&g_ShowCalls);
  printf("   [SPY] ShowWindow (Cmd=%d)\n", nCmdShow);
  return fpShowWindow(hWnd, nCmdShow);
}

__declspec(dllexport) void Spy_Install(void) {
  if (MH_Initialize() != MH_OK) return;
  MH_CreateHookApi(L"user32", "CreateWindowExW", (LPVOID)&Detour_CreateWindowExW,
                   (LPVOID*)&fpCreateWindowExW);
  MH_CreateHookApi(L"user32", "ShowWindow", (LPVOID)&Detour_ShowWindow,
                   (LPVOID*)&fpShowWindow);
  MH_EnableHook(MH_ALL_HOOKS);
  printf("   [SPY] Hooks installed\n");
}

__declspec(dllexport) int Spy_Verify(void) {
  LONG createCalls = g_CreateCalls;
  LONG showCalls = g_ShowCalls;
  if (createCalls > 0 && showCalls > 0) return 1;
  printf("   [SPY] FAILURE: CreateCalls=%ld, ShowCalls=%ld\n", createCalls, showCalls);
  return 0;
}
EOF

cat >"${SRC_DIR}/test_runner.cpp" <<'EOF'
#include <cstdio>
#include <windows.h>
#include "openmfc/afxwin.h"

using PFN_INSTALL = void (*)();
using PFN_VERIFY = int (*)();

static void CALLBACK AutoExitTimer(HWND, UINT, UINT_PTR, DWORD) {
  printf("[APP] Timer expired. Posting WM_QUIT.\n");
  ::PostQuitMessage(0);
}

class CTestApp : public CWinApp {
public:
  CTestApp() : CWinApp(L"TestApp") {}
  BOOL InitInstance() override {
    printf("[APP] InitInstance\n");
    CFrameWnd* pFrame = new CFrameWnd;
    RECT rect = {100, 100, 500, 400};

    if (!pFrame->CFrameWnd::Create(nullptr, L"OpenMFC Regression", WS_OVERLAPPEDWINDOW, rect)) {
      return FALSE;
    }
    ::ShowWindow(pFrame->GetSafeHwnd(), SW_SHOW);
    m_pMainWnd = pFrame;

    ::SetTimer(NULL, 0, 500, &AutoExitTimer);
    return TRUE;
  }
};

CTestApp theApp;

int main() {
  printf("=== OpenMFC Regression Test ===\n");

  HMODULE hSpy = LoadLibraryA("spy.dll");
  if (!hSpy) {
    printf("FATAL: Missing spy.dll\n");
    return 1;
  }

  auto install = reinterpret_cast<PFN_INSTALL>(GetProcAddress(hSpy, "Spy_Install"));
  auto verify = reinterpret_cast<PFN_VERIFY>(GetProcAddress(hSpy, "Spy_Verify"));
  if (install) install();

  if (auto* pApp = AfxGetApp(); pApp && pApp->InitInstance()) {
    pApp->Run();
  }

  printf("=== Verification Results ===\n");
  if (verify && verify()) {
    printf("[PASS] API calls verified.\n");
    return 0;
  }
  printf("[FAIL] API calls missing.\n");
  return 1;
}
EOF

# ------------------------------------------------------------------------------
# 5. Stage artifacts into the temp run dir
# ------------------------------------------------------------------------------
cp -f "${OPENMFC_DLL}" "${BIN_DIR}/openmfc.dll"

if [[ -n "${OPENMFC_LIB}" && -f "${OPENMFC_LIB}" ]]; then
  cp -f "${OPENMFC_LIB}" "${BIN_DIR}/openmfc.lib"
elif [[ -n "${OPENMFC_DEF}" && -f "${OPENMFC_DEF}" && -x "$(command -v lib.exe)" ]]; then
  echo "Generating openmfc.lib from ${OPENMFC_DEF} (MSVC lib.exe)..."
  lib.exe "/def:${OPENMFC_DEF}" "/out:${BIN_DIR}/openmfc.lib" /machine:x64
else
  echo "FATAL: openmfc.lib not found, and cannot generate it." >&2
  echo "  Provide OPENMFC_LIB=... or provide OPENMFC_DEF=... and ensure MSVC lib.exe is available." >&2
  exit 1
fi

# MinGW-built openmfc.dll commonly depends on these runtime DLLs; staging them
# avoids relying on WINEPATH.
MINGW_BIN_DIR="${MINGW_BIN_DIR:-/usr/x86_64-w64-mingw32/bin}"
for dll in libwinpthread-1.dll libgcc_s_seh-1.dll libstdc++-6.dll; do
  if [[ -f "${MINGW_BIN_DIR}/${dll}" ]]; then
    cp -f "${MINGW_BIN_DIR}/${dll}" "${BIN_DIR}/"
  fi
done

# ------------------------------------------------------------------------------
# 6. Build Spy DLL (C + MinGW; no C++/Itanium ABI involved)
# ------------------------------------------------------------------------------
echo "Building spy.dll (MinGW C)..."
x86_64-w64-mingw32-gcc -shared -O2 -s \
  -I"${MINHOOK_DEST}/include" \
  -o "${BIN_DIR}/spy.dll" \
  "${SRC_DIR}/spy.c" \
  "${MINHOOK_DEST}/src/buffer.c" \
  "${MINHOOK_DEST}/src/hook.c" \
  "${MINHOOK_DEST}/src/trampoline.c" \
  "${MINHOOK_DEST}/src/hde/hde64.c" \
  -luser32 -lkernel32 \
  -static-libgcc

# ------------------------------------------------------------------------------
# 7. Build Test Runner (prefer real MSVC; fallback to clang++ in MSVC mode)
# ------------------------------------------------------------------------------
if command -v cl.exe >/dev/null 2>&1; then
  echo "Building test_runner.exe (MSVC cl.exe)..."

  export INCLUDE="${INC_MSVC};${INC_UCRT};${INC_UM};${INC_SHARED};${INC_WINRT};${REPO_ROOT}/include"
  export LIB="${LIB_MSVC};${LIB_UCRT};${LIB_UM};${BIN_DIR}"

  cl.exe /nologo /EHsc /MD /std:c++17 /D_AFXDLL /D_MT /D_DLL \
    "/I${REPO_ROOT}/include" \
    "${SRC_DIR}/test_runner.cpp" \
    /link \
    "/OUT:${BIN_DIR}/test_runner.exe" \
    "${BIN_DIR}/openmfc.lib" user32.lib gdi32.lib \
    /SUBSYSTEM:CONSOLE \
    /NODEFAULTLIB:mfc140u.lib \
    /IGNORE:4217
else
  need_cmd clang++
  need_cmd ld.lld

  echo "Building test_runner.exe (clang++ --target=...msvc)..."
  clang++ \
    --target=x86_64-pc-windows-msvc \
    -fuse-ld=lld \
    -fms-extensions -fms-compatibility -fdelayed-template-parsing \
    -std=c++17 \
    -D_AFXDLL -D_MT -D_DLL \
    -Wno-tautological-undefined-compare \
    -Wno-inconsistent-missing-override \
    -isystem "${INC_MSVC}" \
    -isystem "${INC_UCRT}" \
    -isystem "${INC_UM}" \
    -isystem "${INC_SHARED}" \
    -isystem "${INC_WINRT}" \
    -I"${REPO_ROOT}/include" \
    "${SRC_DIR}/test_runner.cpp" \
    -o "${BIN_DIR}/test_runner.exe" \
    -L"${BIN_DIR}" -L"${LIB_MSVC}" -L"${LIB_UCRT}" -L"${LIB_UM}" \
    -lopenmfc -luser32 -lgdi32 \
    -Wl,-subsystem:console \
    -Wl,-nodefaultlib:mfc140u.lib \
    -Wl,-ignore:4217
fi

# ------------------------------------------------------------------------------
# 8. Run under Wine
# ------------------------------------------------------------------------------
echo "Running under Wine..."
export WINEDEBUG="${WINEDEBUG:--all}"

WINE_TIMEOUT="${WINE_TIMEOUT:-20s}"
(cd "${BIN_DIR}" && timeout "${WINE_TIMEOUT}" wine ./test_runner.exe)
