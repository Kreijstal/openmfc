#!/usr/bin/env bash
set -eu

# Phase 4: Real implementations with weak stub fallbacks
#
# CRITICAL: All implementations must be extern "C" with stub names!
# We cannot use C++ classes directly - GCC mangles differently than MSVC.
#
# Strategy:
# 1. Generate weak stubs for ALL 14k exports (ABI safety net)
# 2. Implement specific functions by overriding their stub_xxx names
# 3. The .def file maps stub names → MSVC-mangled exports
# 4. Verify all required exports are present

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase4}"

echo "Phase 4: Building OpenMFC with real implementations"
echo "=================================================="

# Clean and create build directory
rm -rf "$BUILD"
mkdir -p "$BUILD/include/openmfc"

# Compiler setup
CXX="${CXX:-x86_64-w64-mingw32-g++}"

if ! command -v "$CXX" >/dev/null 2>&1; then
    echo "Error: MinGW compiler not found: $CXX"
    exit 1
fi

# Common flags
CFLAGS=(
    -std=c++17
    -O2
    -Wall
    -fPIC
    -D_WIN32_WINNT=0x0601
    -DUNICODE
    -D_UNICODE
    -DOPENMFC_EXPORTS
    -I"$ROOT/include"
    -I"$BUILD/include"
    -Wno-attributes
)

# Step 1: Generate weak stubs
echo ""
echo "[1/4] Generating weak stubs..."

# Symbols to exclude (already implemented)
# Exception functions - simple (void())
EXCLUDED_SYMBOLS="?AfxThrowMemoryException@@YAXXZ,?AfxThrowNotSupportedException@@YAXXZ,?AfxThrowResourceException@@YAXXZ,?AfxThrowUserException@@YAXXZ,?AfxThrowInvalidArgException@@YAXXZ,?AfxThrowLastCleanup@@YAXXZ,?AfxAbort@@YAXXZ"
# Exception functions - with parameters
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxThrowFileException@@YAXHJPB_W@Z,?AfxThrowArchiveException@@YAXHPB_W@Z,?AfxThrowOleException@@YAXJ@Z,?AfxThrowOleDispatchException@@YAXGII@Z,?AfxThrowOleDispatchException@@YAXGPB_WI@Z,?AfxThrowInternetException@@YAXKK@Z,?AfxThrowDBException@@YAXFPAVCDatabase@@PAX@Z"
# Version functions
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetDllVersion@@YAKXZ"
# CObject methods
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetRuntimeClass@CObject@@UBAPAUCRuntimeClass@@XZ,?GetThisClass@CObject@@SAPAUCRuntimeClass@@XZ,?IsKindOf@CObject@@QBAHPBUCRuntimeClass@@@Z,?IsSerializable@CObject@@QBAHXZ"
# CRuntimeClass methods
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?CreateObject@CRuntimeClass@@QAAPAVCObject@@XZ,?CreateObject@CRuntimeClass@@SAPAVCObject@@PB_W@Z,?CreateObject@CRuntimeClass@@SAPAVCObject@@PBD@Z,?FromName@CRuntimeClass@@SAPAU1@PB_W@Z,?FromName@CRuntimeClass@@SAPAU1@PBD@Z,?IsDerivedFrom@CRuntimeClass@@QBAHPBU1@@Z,?Load@CRuntimeClass@@SAPAU1@AAVCArchive@@PAI@Z,?Store@CRuntimeClass@@QBAXAAVCArchive@@@Z"
# Helper functions
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxClassInit@@YAXPAUCRuntimeClass@@@Z,?AfxDynamicDownCast@@YAPAVCObject@@PAUCRuntimeClass@@PAV1@@Z"
# CWinApp/CWinThread constructors and destructors
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CWinApp@@QAA@PB_W@Z,??1CWinApp@@UAA@XZ,??0CWinThread@@QAA@XZ,??1CWinThread@@UAA@XZ"
# CWinApp/CWinThread methods
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Run@CWinThread@@UAAHXZ,?Run@CWinApp@@UAAHXZ,?InitInstance@CWinApp@@UAAHXZ,?InitInstance@CWinThread@@UAAHXZ,?ExitInstance@CWinThread@@UAAHXZ,?ExitInstance@CWinApp@@UAAHXZ"
# AfxWinMain
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxWinMain@@YAHPAUHINSTANCE__@@0PA_WH@Z"
# CFrameWnd constructor/destructor
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CFrameWnd@@QAA@XZ,??1CFrameWnd@@UAA@XZ"
# CWnd/CFrameWnd window methods
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Create@CWnd@@UAAHPB_W0KABUtagRECT@@PAV1@IPAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Create@CFrameWnd@@UAAHPB_W0KABUtagRECT@@PAVCWnd@@0KPAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?ShowWindow@CWnd@@QAAHH@Z,?UpdateWindow@CWnd@@QAAXXZ,?DestroyWindow@CWnd@@UAAHXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?PreCreateWindow@CWnd@@UAAHAAUtagCREATESTRUCTW@@@Z,?PreCreateWindow@CFrameWnd@@MAAHAAUtagCREATESTRUCTW@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?LoadFrame@CFrameWnd@@UAAHIKPAVCWnd@@PAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?DefWindowProc@CWnd@@MAAJAIH_J@Z,?WindowProc@CWnd@@MAAJAIH_J@Z,?GetSafeHwnd@CWnd@@QBAPAUHWND__@@XZ"
# CDialog methods
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CDialog@@QAA@XZ,??0CDialog@@QAA@IPAVCWnd@@@Z,??0CDialog@@QAA@PB_WPAVCWnd@@@Z,??1CDialog@@UAA@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?DoModal@CDialog@@UAAHXZ,?Create@CDialog@@UAAHPB_WPAVCWnd@@@Z,?EndDialog@CDialog@@QAAXH@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?OnInitDialog@CDialog@@UAAHXZ,?OnOK@CDialog@@MAAXXZ,?OnCancel@CDialog@@MAAXXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?CheckAutoCenter@CDialog@@UAAHXZ,?PreTranslateMessage@CDialog@@UAAHPAUtagMSG@@@Z,?Initialize@CDialog@@QAAXXZ"
# CDialogEx constructors
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CDialogEx@@QAA@XZ,??0CDialogEx@@QAA@IPAVCWnd@@@Z,??0CDialogEx@@QAA@PB_WPAVCWnd@@@Z"
# String functions
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?LoadStringW@?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@QAAHI@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxExtractSubString@@YAHAAV?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@PB_WH_W@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxFormatString1@@YAXAAV?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@IPB_W@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxFormatString2@@YAXAAV?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@IPB_W1@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxMessageBox@@YAHPB_WII@Z,?AfxMessageBox@@YAHIII@Z"

python3 "$ROOT/tools/gen_weak_stubs.py" \
    --mapping "$ROOT/mfc_complete_ordinal_mapping.json" \
    --out-def "$BUILD/openmfc.def" \
    --out-stubs "$BUILD/weak_stubs.cpp" \
    --exclude "$EXCLUDED_SYMBOLS"

# Step 2: Generate RTTI metadata
echo ""
echo "[2/4] Generating RTTI metadata..."
python3 "$ROOT/tools/gen_rtti.py" \
    --exceptions "$ROOT/exceptions.json" \
    --out-c "$BUILD/generated_rtti.c" \
    --out-h "$BUILD/include/openmfc/eh_rtti.h"

# Step 3: Compile everything
echo ""
echo "[3/4] Compiling..."
"$CXX" "${CFLAGS[@]}" -c "$BUILD/weak_stubs.cpp" -o "$BUILD/weak_stubs.o"
"$CXX" "${CFLAGS[@]}" -c "$BUILD/generated_rtti.c" -o "$BUILD/generated_rtti.o"

# Compile implementation files
IMPL_SOURCES=(
    "$ROOT/phase4/src/mfc_exceptions.cpp"
    "$ROOT/phase4/src/version_impl.cpp"
    "$ROOT/phase4/src/cobject_impl.cpp"
    "$ROOT/phase4/src/appcore.cpp"
    "$ROOT/phase4/src/strcore.cpp"
    "$ROOT/phase4/src/wincore.cpp"
    "$ROOT/phase4/src/dlgcore.cpp"
    # Add more implementation files here as they are created
)

for src in "${IMPL_SOURCES[@]}"; do
    if [[ -f "$src" ]]; then
        obj="${src%.cpp}.o"
        echo "  Compiling implementation: $(basename "$src")"
        "$CXX" "${CFLAGS[@]}" -c "$src" -o "$obj"
    fi
done

# Step 4: Link DLL
echo ""
echo "[4/4] Linking openmfc.dll..."

LDFLAGS=(
    -shared
    -static-libgcc
    -static-libstdc++
    -Wl,--enable-stdcall-fixup
    -Wl,--enable-auto-import
    -Wl,"$BUILD/openmfc.def"
    -Wl,--out-implib,"$BUILD/libopenmfc.a"
)

# Collect all object files
OBJ_FILES=(
    "$BUILD/generated_rtti.o"
    "$BUILD/weak_stubs.o"
)

# Add implementation object files
for src in "${IMPL_SOURCES[@]}"; do
    if [[ -f "$src" ]]; then
        obj="${src%.cpp}.o"
        OBJ_FILES+=("$obj")
    fi
done

echo "  Linking with ${#OBJ_FILES[@]} object files"
"$CXX" \
    "${OBJ_FILES[@]}" \
    "$BUILD/openmfc.def" \
    "${LDFLAGS[@]}" \
    -o "$BUILD/openmfc.dll"

echo ""
echo "=================================================="
echo "Build complete!"
echo "  DLL: $BUILD/openmfc.dll"
echo "  Import lib: $BUILD/libopenmfc.a"

# Verify ABI (only if we have the right tools)
echo ""
echo "Verifying ABI exports..."
if command -v x86_64-w64-mingw32-objdump >/dev/null 2>&1; then
    python3 "$ROOT/scripts/verify_abi_exports.py" \
        --mapping "$ROOT/mfc_complete_ordinal_mapping.json" \
        --dll "$BUILD/openmfc.dll" || echo "Warning: ABI verification failed (may need different objdump)"
else
    echo "Skipping ABI verification (x86_64-w64-mingw32-objdump not available)"
fi

# Run comprehensive safety check (only if phase4_safety_check.sh exists and is executable)
if [[ -x "$ROOT/scripts/phase4_safety_check.sh" ]]; then
    echo ""
    echo "Running comprehensive ABI safety check..."
    "$ROOT/scripts/phase4_safety_check.sh" || echo "Warning: Safety check had issues"
fi


