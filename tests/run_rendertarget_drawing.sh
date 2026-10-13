#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MSVC_DIR="${MSVC_DIR:-/tmp/msvc/unpack}"
BUILD_DIR="${BUILD_DIR:-/tmp/openmfc-rendertarget-drawing}"
MSVC_VER="$(ls -1 "$MSVC_DIR/VC/Tools/MSVC" | sort -V | tail -1)"
SDK_VER="$(ls -1 "$MSVC_DIR/Windows Kits/10/Include" | sort -V | tail -1)"
mkdir -p "$BUILD_DIR"

FLAGS=(/nologo /EHsc /MT /std:c++17 /Gy /Gw /W0
    /DUNICODE /D_UNICODE /DNOMINMAX /D_WIN32_WINNT=0x0601
    -imsvc "$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/include"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/ucrt"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/um"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/shared"
    -I"$ROOT/include" -I"$ROOT/phase4/src")
SOURCES=(tests/test_rendertarget_drawing.cpp
    phase4/src/detail/GdicoreSupport.cpp
    phase4/src/core/d2d/CD2DResource.cpp phase4/src/core/d2d/RuntimeClasses.cpp
    phase4/src/core/d2d/_AFX_D2D_STATE.cpp phase4/src/core/d2d/CDCRenderTarget.cpp
    phase4/src/core/d2d/CHwndRenderTarget.cpp
    phase4/src/core/d2d/CD2DGeometry.cpp phase4/src/core/d2d/CD2DPathGeometry.cpp
    phase4/src/core/d2d/CD2DPointF.cpp phase4/src/core/d2d/CD2DSizeF.cpp
    phase4/src/core/d2d/CD2DRectF.cpp phase4/src/core/d2d/CD2DEllipse.cpp
    phase4/src/core/d2d/CD2DRoundedRect.cpp)
OBJECTS=()
for source in "${SOURCES[@]}"; do
    object="$BUILD_DIR/$(basename "${source%.cpp}").obj"
    clang-cl "${FLAGS[@]}" /c "$ROOT/$source" /Fo"$object"
    OBJECTS+=("$object")
done
# The DLL generator shortens long internal names. Check the actual generator
# aliases, rather than trusting Symbol markers or direct-call tests alone.
python3 - "$ROOT" "$BUILD_DIR/test_rendertarget_drawing.obj" <<'PY'
import re
import subprocess
import sys
from pathlib import Path

root = Path(sys.argv[1])
sys.path.insert(0, str(root / 'tools'))
import gen_weak_stubs
defined = {line.split()[-1] for line in subprocess.check_output(
    ['llvm-nm', '--defined-only', sys.argv[2]], text=True).splitlines() if line.split()}
for name in ('CD2DTextFormat', 'CD2DTextLayout'):
    source = (root / 'phase4/src/core/d2d' / (name + '.cpp')).read_text()
    for symbol in re.findall(r'^// Symbol: (.+)$', source, re.M):
        alias = gen_weak_stubs.generate_stub_name(symbol)
        assert alias in defined, 'Missing compiled export alias: ' + alias
print('Text resource export aliases match the DLL generator')
PY
lld-link /OUT:"$BUILD_DIR/test_rendertarget_drawing.exe" /SUBSYSTEM:CONSOLE /OPT:REF \
    "/LIBPATH:$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64" \
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64" \
    "${OBJECTS[@]}" kernel32.lib user32.lib gdi32.lib ole32.lib d2d1.lib windowscodecs.lib
RUNNER=(wine)
if [[ -z "${DISPLAY:-}" ]] && command -v xvfb-run >/dev/null 2>&1; then
    RUNNER=(xvfb-run -a wine)
fi
WINEDEBUG="${WINEDEBUG:--all}" timeout 45 "${RUNNER[@]}" "$BUILD_DIR/test_rendertarget_drawing.exe"
