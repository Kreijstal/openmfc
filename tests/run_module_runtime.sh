#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MSVC_DIR="${MSVC_DIR:-/tmp/msvc/unpack}"
BUILD_DIR="${BUILD_DIR:-/tmp/openmfc-module-runtime-validation}"
MSVC_VER="$(ls -1 "$MSVC_DIR/VC/Tools/MSVC" | sort -V | tail -1)"
SDK_VER="$(ls -1 "$MSVC_DIR/Windows Kits/10/Include" | sort -V | tail -1)"
mkdir -p "$BUILD_DIR"
FLAGS=(/nologo /EHs /MD /std:c++17 /Gy /Gw /W0 /DUNICODE /D_UNICODE /DNOMINMAX
    /DOPENMFC_EXPORTS /D_WIN32_WINNT=0x0601 /FIwinsock2.h /FIobjbase.h /FIcommdlg.h /FIshellapi.h
    -imsvc "$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/include"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/ucrt"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/um"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/shared"
    -I"$ROOT/include" -I"$ROOT/phase4/src")
if [[ "${OPTIMIZE:-0}" == 1 ]]; then FLAGS+=(/O2); fi
SOURCES=(phase4/src/core/runtime/AFX_MODULE_THREAD_STATE.cpp
    phase4/src/core/collections/CHandleMap.cpp phase4/src/detail/CHandleMapSupport.cpp
    phase4/src/detail/MemcoreSupport.cpp phase4/src/core/collections/CMapPtrToPtrStorage.cpp
    phase4/src/detail/CPtrListSupport.cpp phase4/src/core/controls/CToolTipCtrlLifetime.cpp
    phase4/src/detail/CFixedAllocSupport.cpp phase4/src/core/collections/CPlex.cpp
    phase4/src/core/runtime/CThreadLocalObject.cpp phase4/src/core/runtime/CThreadSlotData.cpp
    phase4/src/detail/CThreadLocalObjectSupport.cpp phase4/src/detail/CThreadSlotDataSupport.cpp
    phase4/src/core/runtime/_AFX_THREAD_STATE.cpp)
OBJECTS=()
for source in "${SOURCES[@]}"; do
    object="$BUILD_DIR/$(basename "${source%.cpp}").obj"
    clang-cl "${FLAGS[@]}" /c "$ROOT/$source" /Fo"$object"
    OBJECTS+=("$object")
done
python3 - "$ROOT" "$BUILD_DIR" <<'PY'
import re, subprocess, sys
from pathlib import Path
root=Path(sys.argv[1]);out=Path(sys.argv[2]);sys.path.insert(0,str(root/'tools'))
import gen_weak_stubs
count=0
for relative in ('core/runtime/AFX_MODULE_THREAD_STATE','core/collections/CHandleMap','core/controls/CToolTipCtrlLifetime','detail/MemcoreSupport'):
    path=root/'phase4/src'/(relative+'.cpp');name=Path(relative).name
    defined={line.split()[-1] for line in subprocess.check_output(['llvm-nm','--defined-only',str(out/(name+'.obj'))],text=True).splitlines() if line.split()}
    for symbol in re.findall(r'^// Symbol: (.+)$',path.read_text(),re.M):
        assert gen_weak_stubs.generate_stub_name(symbol) in defined,'Missing module runtime alias: '+symbol
        count+=1
assert count==11,count
print('All eleven module runtime aliases match the DLL generator')
PY
LINK_FLAGS=(/SUBSYSTEM:CONSOLE /OPT:REF /errorlimit:0
    "/LIBPATH:$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64"
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64"
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64")
for test in test_module_runtime_native test_chandlemap_logic; do
    clang-cl "${FLAGS[@]}" /c "$ROOT/tests/$test.cpp" /Fo"$BUILD_DIR/$test.obj"
    extra=()
    if [[ "$test" == test_chandlemap_logic ]]; then
        clang-cl "${FLAGS[@]}" /c "$ROOT/tests/thread_state_test_runtime.cpp" /Fo"$BUILD_DIR/thread_state_test_runtime.obj"
        extra=("$BUILD_DIR/thread_state_test_runtime.obj")
    fi
    lld-link /OUT:"$BUILD_DIR/$test.exe" "${LINK_FLAGS[@]}" "${OBJECTS[@]}" "$BUILD_DIR/$test.obj" "${extra[@]}" kernel32.lib user32.lib
done
for name in vcruntime140.dll vcruntime140_1.dll; do
    for runtime in "$MSVC_DIR"/VC/Redist/MSVC/*/x64/Microsoft.VC*.CRT/"$name"; do
        if [[ -f "$runtime" ]]; then cp "$runtime" "$BUILD_DIR/$name"; fi
    done
done
RUNNER=(wine)
if [[ -z "${DISPLAY:-}" ]] && command -v xvfb-run >/dev/null 2>&1; then RUNNER=(xvfb-run -a wine); fi
for test in test_module_runtime_native test_chandlemap_logic; do
    WINEDEBUG="${WINEDEBUG:--all}" WINEDLLOVERRIDES="${WINEDLLOVERRIDES:-vcruntime140,vcruntime140_1=n}" timeout 120 "${RUNNER[@]}" "$BUILD_DIR/$test.exe"
done
