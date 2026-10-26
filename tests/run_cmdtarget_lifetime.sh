#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MSVC_DIR="${MSVC_DIR:-/tmp/msvc/unpack}"
BUILD_DIR="${BUILD_DIR:-/tmp/openmfc-cmdtarget-lifetime}"
MSVC_VER="$(ls -1 "$MSVC_DIR/VC/Tools/MSVC" | sort -V | tail -1)"
SDK_VER="$(ls -1 "$MSVC_DIR/Windows Kits/10/Include" | sort -V | tail -1)"
mkdir -p "$BUILD_DIR"
FLAGS=(/nologo /EHs /MD /std:c++17 /Gy /Gw /W0 /DUNICODE /D_UNICODE /DNOMINMAX
    /D_WIN32_WINNT=0x0601 /FIwinsock2.h /FIobjbase.h /FIcommdlg.h /FIshellapi.h
    -imsvc "$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/include"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/ucrt"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/um"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/shared"
    -I"$ROOT/include" -I"$ROOT/phase4/src")
if [[ "${OPTIMIZE:-0}" == 1 ]]; then FLAGS+=(/O2); fi
clang-cl "${FLAGS[@]}" /c "$ROOT/phase4/src/core/runtime/CCmdTargetLifetime.cpp" /Fo"$BUILD_DIR/CCmdTargetLifetime.obj"
clang-cl "${FLAGS[@]}" /c "$ROOT/tests/test_cmdtarget_lifetime.cpp" /Fo"$BUILD_DIR/test_cmdtarget_lifetime.obj"
python3 - "$ROOT" "$BUILD_DIR" <<'PY'
import re, subprocess, sys
from pathlib import Path
root=Path(sys.argv[1]);out=Path(sys.argv[2]);sys.path.insert(0,str(root/'tools'))
import gen_weak_stubs
source=root/'phase4/src/core/runtime/CCmdTargetLifetime.cpp'
defined={line.split()[-1] for line in subprocess.check_output(['llvm-nm','--defined-only',str(out/'CCmdTargetLifetime.obj')],text=True).splitlines() if line.split()}
symbols=re.findall(r'^// Symbol: (.+)$',source.read_text(),re.M)
assert len(symbols)==2
for symbol in symbols:
    assert gen_weak_stubs.generate_stub_name(symbol) in defined,symbol
print('Both command-target lifetime aliases match the DLL generator')
PY
lld-link /OUT:"$BUILD_DIR/test_cmdtarget_lifetime.exe" /SUBSYSTEM:CONSOLE /OPT:REF \
    "/LIBPATH:$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64" \
    "$BUILD_DIR/CCmdTargetLifetime.obj" "$BUILD_DIR/test_cmdtarget_lifetime.obj" kernel32.lib
for name in vcruntime140.dll vcruntime140_1.dll; do
    for runtime in "$MSVC_DIR"/VC/Redist/MSVC/*/x64/Microsoft.VC*.CRT/"$name"; do
        if [[ -f "$runtime" ]]; then cp "$runtime" "$BUILD_DIR/$name"; fi
    done
done
RUNNER=(wine)
if [[ -z "${DISPLAY:-}" ]] && command -v xvfb-run >/dev/null 2>&1; then RUNNER=(xvfb-run -a wine); fi
WINEDEBUG="${WINEDEBUG:--all}" WINEDLLOVERRIDES="${WINEDLLOVERRIDES:-vcruntime140,vcruntime140_1=n}" \
    timeout 120 "${RUNNER[@]}" "$BUILD_DIR/test_cmdtarget_lifetime.exe"
