#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
MSVC_DIR="${MSVC_DIR:-/home/kreijstal/msvc}"
BUILD_DIR="${BUILD_DIR:-/tmp/openmfc-cimagelist-handle-map}"
MSVC_VER="$(ls -1 "$MSVC_DIR/VC/Tools/MSVC" | sort -V | tail -1)"
SDK_VER="$(ls -1 "$MSVC_DIR/Windows Kits/10/Include" | sort -V | tail -1)"
mkdir -p "$BUILD_DIR"
FLAGS=(/nologo /EHsc /MD /std:c++17 /Gy /Gw /W0 /DUNICODE /D_UNICODE /DNOMINMAX
    /DOPENMFC_EXPORTS /D_WIN32_WINNT=0x0601 /FIwinsock2.h /FIobjbase.h /FIcommdlg.h /FIshellapi.h
    -imsvc "$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/include"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/ucrt"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/um"
    -imsvc "$MSVC_DIR/Windows Kits/10/Include/$SDK_VER/shared"
    -I"$ROOT/include" -I"$ROOT/phase4/src")
SOURCES=(phase4/src/core/gdi/CImageList.cpp
    phase4/src/core/gdi/CImageListLifetime.cpp
    phase4/src/core/collections/CHandleMap.cpp
    phase4/src/detail/CHandleMapSupport.cpp
    phase4/src/detail/CFixedAllocSupport.cpp
    phase4/src/core/collections/CMapPtrToPtrStorage.cpp
    phase4/src/core/collections/CPlex.cpp
    tests/test_cimagelist_handle_map.cpp)
OBJECTS=()
for source in "${SOURCES[@]}"; do
    object="$BUILD_DIR/$(basename "${source%.cpp}").obj"
    clang-cl "${FLAGS[@]}" /c "$ROOT/$source" "/Fo$object"
    OBJECTS+=("$object")
done
LINK_FLAGS=(/nologo /SUBSYSTEM:CONSOLE /OPT:REF /OUT:"$BUILD_DIR/test_cimagelist_handle_map.exe"
    "/LIBPATH:$MSVC_DIR/VC/Tools/MSVC/$MSVC_VER/lib/x64"
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/ucrt/x64"
    "/LIBPATH:$MSVC_DIR/Windows Kits/10/Lib/$SDK_VER/um/x64")
lld-link "${LINK_FLAGS[@]}" "${OBJECTS[@]}" kernel32.lib user32.lib comctl32.lib
for name in vcruntime140.dll vcruntime140_1.dll; do
    for runtime in "$MSVC_DIR"/VC/Redist/MSVC/*/x64/Microsoft.VC*.CRT/"$name"; do
        [[ -f "$runtime" ]] && cp "$runtime" "$BUILD_DIR/$name"
    done
done
DLL="$MSVC_DIR/VC/Redist/MSVC/14.51.36231/x64/Microsoft.VC145.MFC/mfc140u.dll"
export OPENMFC_NATIVE_IMAGELIST_DLL="Z:${DLL//\//\\}"
export WINEDEBUG="${WINEDEBUG:--all}"
export WINEDLLOVERRIDES="${WINEDLLOVERRIDES:-vcruntime140,vcruntime140_1=n}"
RUNNER=(wine)
if [[ -z "${DISPLAY:-}" ]] && command -v xvfb-run >/dev/null 2>&1; then RUNNER=(xvfb-run -a wine); fi
timeout 120 "${RUNNER[@]}" "$BUILD_DIR/test_cimagelist_handle_map.exe"
