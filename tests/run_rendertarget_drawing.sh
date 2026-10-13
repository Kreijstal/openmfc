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
    phase4/src/core/d2d/CD2DPointF.cpp phase4/src/core/d2d/CD2DSizeF.cpp
    phase4/src/core/d2d/CD2DRectF.cpp phase4/src/core/d2d/CD2DEllipse.cpp
    phase4/src/core/d2d/CD2DRoundedRect.cpp)
OBJECTS=()
for source in "${SOURCES[@]}"; do
    object="$BUILD_DIR/$(basename "${source%.cpp}").obj"
    clang-cl "${FLAGS[@]}" /c "$ROOT/$source" /Fo"$object"
    OBJECTS+=("$object")
done
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
