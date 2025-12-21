#!/usr/bin/env bash
set -eu

# Phase 4: Real implementations with weak stub fallbacks
#
# CRITICAL: All implementations must be extern "C" with stub names!
# We cannot use C++ classes directly - GCC mangles differently than MSVC.
#
# Strategy:
# 1. Generate weak stubs for ALL 14k exports (ABI safety net)
# 2. Implement specific functions by overriding their impl_xxx names
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

# Symbols to exclude (already implemented) - x64 format
# Exception functions - simple (void())
EXCLUDED_SYMBOLS="?AfxThrowMemoryException@@YAXXZ,?AfxThrowNotSupportedException@@YAXXZ,?AfxThrowResourceException@@YAXXZ,?AfxThrowUserException@@YAXXZ,?AfxThrowInvalidArgException@@YAXXZ,?AfxThrowLastCleanup@@YAXXZ,?AfxAbort@@YAXXZ"
# Exception functions - with parameters (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxThrowFileException@@YAXHJPEB_W@Z,?AfxThrowArchiveException@@YAXHPEB_W@Z,?AfxThrowOleException@@YAXJ@Z,?AfxThrowOleDispatchException@@YAXGII@Z,?AfxThrowOleDispatchException@@YAXGPEB_WI@Z,?AfxThrowInternetException@@YAX_KK@Z,?AfxThrowDBException@@YAXFPEAVCDatabase@@PEAX@Z"
# Version functions
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetDllVersion@@YAKXZ"
# CObject methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetRuntimeClass@CObject@@UEBAPEAUCRuntimeClass@@XZ,?GetThisClass@CObject@@SAPEAUCRuntimeClass@@XZ,?IsKindOf@CObject@@QEBAHPEBUCRuntimeClass@@@Z,?IsSerializable@CObject@@QEBAHXZ,?Serialize@CObject@@UEAAXAEAVCArchive@@@Z"
# CException methods (x64) - needed for RUNTIME_CLASS(CException) to work
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetThisClass@CException@@SAPEAUCRuntimeClass@@XZ"
# CMemoryException methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetThisClass@CMemoryException@@SAPEAUCRuntimeClass@@XZ"
# CFileException methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetThisClass@CFileException@@SAPEAUCRuntimeClass@@XZ"
# CArchiveException methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetThisClass@CArchiveException@@SAPEAUCRuntimeClass@@XZ"
# CRuntimeClass methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?CreateObject@CRuntimeClass@@QEAAPEAVCObject@@XZ,?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEB_W@Z,?CreateObject@CRuntimeClass@@SAPEAVCObject@@PEBD@Z,?FromName@CRuntimeClass@@SAPEAU1@PEB_W@Z,?FromName@CRuntimeClass@@SAPEAU1@PEBD@Z,?IsDerivedFrom@CRuntimeClass@@QEBAHPEBU1@@Z,?Load@CRuntimeClass@@SAPEAU1@AEAVCArchive@@PEAI@Z,?Store@CRuntimeClass@@QEBAXAEAVCArchive@@@Z"
# Helper functions (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxClassInit@@YAXPEAUCRuntimeClass@@@Z,?AfxDynamicDownCast@@YAPEAVCObject@@PEAUCRuntimeClass@@PEAV1@@Z"
# CWinApp/CWinThread constructors and destructors (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CWinApp@@QEAA@PEB_W@Z,??1CWinApp@@UEAA@XZ,??0CWinThread@@QEAA@XZ,??1CWinThread@@UEAA@XZ"
# CWinApp/CWinThread methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Run@CWinThread@@UEAAHXZ,?Run@CWinApp@@UEAAHXZ,?InitInstance@CWinApp@@UEAAHXZ,?InitInstance@CWinThread@@UEAAHXZ,?ExitInstance@CWinThread@@UEAAHXZ,?ExitInstance@CWinApp@@UEAAHXZ"
# AfxWinMain (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxWinMain@@YAHPEAUHINSTANCE__@@0PEA_WH@Z"
# AfxGetThread (x64) - implemented in appcore.cpp
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetThread@@YAPEAVCWinThread@@XZ"
# CFrameWnd constructor/destructor (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CFrameWnd@@QEAA@XZ,??1CFrameWnd@@UEAA@XZ"
# CWnd/CFrameWnd window methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Create@CWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Create@CFrameWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@0KPEAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?ShowWindow@CWnd@@QEAAHH@Z,?UpdateWindow@CWnd@@QEAAXXZ,?DestroyWindow@CWnd@@UEAAHXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?PreCreateWindow@CWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z,?PreCreateWindow@CFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?LoadFrame@CFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z,?WindowProc@CWnd@@MEAA_JI_K_J@Z,?GetSafeHwnd@CWnd@@QEBAPEAUHWND__@@XZ"
# CCmdTarget destructor (x64) and GetThisClass
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??1CCmdTarget@@UEAA@XZ,?GetThisClass@CCmdTarget@@SAPEAUCRuntimeClass@@XZ"

# CWinApp/CWinThread constructors and destructors (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CWinApp@@QEAA@PEB_W@Z,??1CWinApp@@UEAA@XZ,??0CWinThread@@QEAA@XZ,??1CWinThread@@UEAA@XZ"
# CWinApp/CWinThread methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Run@CWinThread@@UEAAHXZ,?Run@CWinApp@@UEAAHXZ,?InitInstance@CWinApp@@UEAAHXZ,?InitInstance@CWinThread@@UEAAHXZ,?ExitInstance@CWinThread@@UEAAHXZ,?ExitInstance@CWinApp@@UEAAHXZ"
# CWinApp/CWinThread GetThisClass
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetThisClass@CWinThread@@SAPEAUCRuntimeClass@@XZ,?GetThisClass@CWinApp@@SAPEAUCRuntimeClass@@XZ"

# AfxWinMain (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxWinMain@@YAHPEAUHINSTANCE__@@0PEA_WH@Z"
# AfxGetThread (x64) - implemented in appcore.cpp
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetThread@@YAPEAVCWinThread@@XZ"
# CFrameWnd constructor/destructor (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CFrameWnd@@QEAA@XZ,??1CFrameWnd@@UEAA@XZ"
# CWnd/CFrameWnd window methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Create@CWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAV1@IPEAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?Create@CFrameWnd@@UEAAHPEB_W0KAEBUtagRECT@@PEAVCWnd@@0KPEAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?ShowWindow@CWnd@@QEAAHH@Z,?UpdateWindow@CWnd@@QEAAXXZ,?DestroyWindow@CWnd@@UEAAHXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?PreCreateWindow@CWnd@@UEAAHAEAUtagCREATESTRUCTW@@@Z,?PreCreateWindow@CFrameWnd@@MEAAHAEAUtagCREATESTRUCTW@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?LoadFrame@CFrameWnd@@UEAAHIKPEAVCWnd@@PEAUCCreateContext@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?DefWindowProcW@CWnd@@MEAA_JI_K_J@Z,?WindowProc@CWnd@@MEAA_JI_K_J@Z,?GetSafeHwnd@CWnd@@QEBAPEAUHWND__@@XZ"
# CWnd/CFrameWnd GetThisClass
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetThisClass@CWnd@@SAPEAUCRuntimeClass@@XZ,?GetThisClass@CFrameWnd@@SAPEAUCRuntimeClass@@XZ"
# CDialog methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CDialog@@QEAA@XZ,??0CDialog@@QEAA@IPEAVCWnd@@@Z,??0CDialog@@QEAA@PEB_WPEAVCWnd@@@Z,??1CDialog@@UEAA@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?DoModal@CDialog@@UEAA_JXZ,?Create@CDialog@@UEAAHPEB_WPEAVCWnd@@@Z,?EndDialog@CDialog@@QEAAXH@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?OnInitDialog@CDialog@@UEAAHXZ,?OnOK@CDialog@@MEAAXXZ,?OnCancel@CDialog@@MEAAXXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?CheckAutoCenter@CDialog@@UEAAHXZ,?PreTranslateMessage@CDialog@@UEAAHPEAUtagMSG@@@Z,?Initialize@CDialog@@QEAAXXZ"
# CDialogEx constructors (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CDialogEx@@QEAA@XZ,??0CDialogEx@@QEAA@IPEAVCWnd@@@Z,??0CDialogEx@@QEAA@PEB_WPEAVCWnd@@@Z"
# String functions (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?LoadStringW@?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@QEAAHI@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxExtractSubString@@YAHAEAV?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_WH_W@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxFormatString1@@YAXAEAV?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxFormatString2@@YAXAEAV?\$CStringT@_WV?\$StrTraitMFC_DLL@_WV?\$ChTraitsCRT@_W@ATL@@@@@ATL@@IPEB_W1@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxMessageBox@@YAHPEB_WII@Z,?AfxMessageBox@@YAHIII@Z"

# Global operator new/delete are used by real MFC clients and by our MSVC hello-world
# test. The auto-stub generator doesn't know signatures and would emit `void` stubs,
# which produces garbage pointers and immediate crashes.
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??2@YAPEAX_K@Z,??3@YAXPEAX@Z,??_U@YAPEAX_K@Z,??_V@YAXPEAX@Z"

# CDC constructor/destructor (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CDC@@QEAA@XZ,??1CDC@@UEAA@XZ"
# CDC methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?CreateCompatibleDC@CDC@@QEAAHPEAV1@@Z,?DeleteDC@CDC@@QEAAHXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?SaveDC@CDC@@QEAAHXZ,?RestoreDC@CDC@@QEAAHH@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?MoveTo@CDC@@QEAA?AVCPoint@@HH@Z,?LineTo@CDC@@QEAAHHH@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?SelectStockObject@CDC@@QEAAHH@Z"
# CClientDC/CPaintDC/CWindowDC (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CClientDC@@QEAA@PEAVCWnd@@@Z,??1CClientDC@@UEAA@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CPaintDC@@QEAA@PEAVCWnd@@@Z,??1CPaintDC@@UEAA@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CWindowDC@@QEAA@PEAVCWnd@@@Z,??1CWindowDC@@UEAA@XZ"
# CPen/CBrush/CFont/CBitmap constructors (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CPen@@QEAA@XZ,??0CPen@@QEAA@HHK@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CBrush@@QEAA@XZ,??0CBrush@@QEAA@K@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CFont@@QEAA@XZ,??0CBitmap@@QEAA@XZ"

# CMenu constructor/destructor (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,??0CMenu@@QEAA@XZ,??1CMenu@@UEAA@XZ"
# CMenu methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?CreateMenu@CMenu@@QEAAHXZ,?CreatePopupMenu@CMenu@@QEAAHXZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?DestroyMenu@CMenu@@QEAAHXZ,?Attach@CMenu@@QEAAHPEAUHMENU__@@@Z,?Detach@CMenu@@QEAAPEAUHMENU__@@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?TrackPopupMenu@CMenu@@QEAAHIHHPEAVCWnd@@PEBUtagRECT@@@Z"
# CWnd menu methods (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?GetMenu@CWnd@@UEBAPEAVCMenu@@XZ,?SetMenu@CWnd@@UEAAHPEAVCMenu@@@Z"

# AfxRegisterClass and AfxRegisterWndClass (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxRegisterClass@@YAHPEAUtagWNDCLASSW@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxRegisterWndClass@@YAPEB_WIPEAUHICON__@@PEAUHBRUSH__@@0@Z"
# Afx global functions (x64)
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetInstanceHandle@@YAPEAUHINSTANCE__@@XZ,?AfxGetResourceHandle@@YAPEAUHINSTANCE__@@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxSetResourceHandle@@YAXPEAUHINSTANCE__@@@Z"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetMainWnd@@YAPEAVCWnd@@XZ,?AfxGetApp@@YAPEAVCWinApp@@XZ"
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?AfxGetModuleState@@YAPEAUAFX_MODULE_STATE@@XZ,?AfxGetStaticModuleState@@YAPEAUAFX_MODULE_STATE@@XZ"

# Automatically exclude any symbols that have a real implementation in Phase 4.
# The weak stubs generator can't rely on PE/COFF weak symbols, so duplicates
# must be excluded up-front.
AUTO_EXCLUDES=""
if command -v rg >/dev/null 2>&1; then
    AUTO_EXCLUDES="$(rg -N --no-heading --no-line-number --no-filename '^// Symbol: ' "$ROOT/phase4/src"/*.cpp \
        | sed -E 's%^// Symbol: %%' \
        | tr -d '\r' \
        | paste -sd, - || true)"
else
    AUTO_EXCLUDES="$(grep -hE '^// Symbol: ' "$ROOT/phase4/src"/*.cpp 2>/dev/null \
        | sed -E 's%^// Symbol: %%' \
        | tr -d '\r' \
        | paste -sd, - || true)"
fi

if [[ -n "$AUTO_EXCLUDES" ]]; then
    EXCLUDED_SYMBOLS="${EXCLUDED_SYMBOLS},${AUTO_EXCLUDES}"
fi

# Static Class Members (Data Exports)
# We implement these manually (via IMPLEMENT_DYNAMIC or manual definition) so we must exclude them from stubs
# CObject
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?classCObject@CObject@@2UCRuntimeClass@@A"
# CException and derived
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?classCException@CException@@2UCRuntimeClass@@A,?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A,?classCFileException@CFileException@@2UCRuntimeClass@@A,?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A"
# Application Core
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A,?classCWinThread@CWinThread@@2UCRuntimeClass@@A,?classCWinApp@CWinApp@@2UCRuntimeClass@@A"
# Windowing
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?classCWnd@CWnd@@2UCRuntimeClass@@A,?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A"
# Menus
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?classCMenu@CMenu@@2UCRuntimeClass@@A"
# GDI
EXCLUDED_SYMBOLS="$EXCLUDED_SYMBOLS,?classCGdiObject@CGdiObject@@2UCRuntimeClass@@A,?classCPen@CPen@@2UCRuntimeClass@@A,?classCBrush@CBrush@@2UCRuntimeClass@@A,?classCFont@CFont@@2UCRuntimeClass@@A,?classCBitmap@CBitmap@@2UCRuntimeClass@@A,?classCPalette@CPalette@@2UCRuntimeClass@@A,?classCRgn@CRgn@@2UCRuntimeClass@@A,?classCDC@CDC@@2UCRuntimeClass@@A,?classCClientDC@CClientDC@@2UCRuntimeClass@@A,?classCPaintDC@CPaintDC@@2UCRuntimeClass@@A,?classCWindowDC@CWindowDC@@2UCRuntimeClass@@A"

python3 "$ROOT/tools/gen_weak_stubs.py" \
    --mapping "$ROOT/mfc_complete_ordinal_mapping.json" \
    --out-def "$BUILD/openmfc.def" \
    --out-stubs "$BUILD/weak_stubs.cpp" \
    --exclude "$EXCLUDED_SYMBOLS"

# Remove the "; Total exports:" comment line if present (causes .def syntax errors)
sed -i '/; Total exports:/d' "$BUILD/openmfc.def"

# Add OpenMFC-specific exports (static class members with MSVC-mangled names)
# These use the .def alias syntax (MSVC_name=GCC_name) to export MSVC-compatible
# names from GCC-mangled internal symbols. This is required because multiple
# translation units may instantiate the inline static members, and the linker
# may pick a version without the asm alias.
# IMPORTANT: Entries must be indented to be part of the EXPORTS section!
cat >> "$BUILD/openmfc.def" << 'EOF_OPENMFC_EXPORTS'
    ; OpenMFC-specific static class member exports (CRuntimeClass statics)
    ; These are required for MSVC code that uses MFC RTTI macros
    ; Using aliasing: external_name=internal_name
    ; Note: DATA keyword omitted - lib.exe may not handle aliased DATA exports correctly
    ?classCObject@CObject@@2UCRuntimeClass@@A=_ZN7CObject12classCObjectE
    ?classCCmdTarget@CCmdTarget@@2UCRuntimeClass@@A=_ZN10CCmdTarget15classCCmdTargetE
    ?classCWnd@CWnd@@2UCRuntimeClass@@A=_ZN4CWnd9classCWndE
    ?classCWinThread@CWinThread@@2UCRuntimeClass@@A=_ZN10CWinThread15classCWinThreadE
    ?classCWinApp@CWinApp@@2UCRuntimeClass@@A=_ZN7CWinApp12classCWinAppE
    ?classCException@CException@@2UCRuntimeClass@@A=_ZN10CException15classCExceptionE
    ?classCFileException@CFileException@@2UCRuntimeClass@@A=_ZN14CFileException19classCFileExceptionE
    ?classCMemoryException@CMemoryException@@2UCRuntimeClass@@A=_ZN16CMemoryException21classCMemoryExceptionE
    ?classCArchiveException@CArchiveException@@2UCRuntimeClass@@A=_ZN17CArchiveException22classCArchiveExceptionE
    ; GDI class runtime classes
    ?classCGdiObject@CGdiObject@@2UCRuntimeClass@@A=_ZN10CGdiObject15classCGdiObjectE
    ?classCPen@CPen@@2UCRuntimeClass@@A=_ZN4CPen9classCPenE
    ?classCBrush@CBrush@@2UCRuntimeClass@@A=_ZN6CBrush11classCBrushE
    ?classCFont@CFont@@2UCRuntimeClass@@A=_ZN5CFont10classCFontE
    ?classCBitmap@CBitmap@@2UCRuntimeClass@@A=_ZN7CBitmap12classCBitmapE
    ?classCPalette@CPalette@@2UCRuntimeClass@@A=_ZN8CPalette13classCPaletteE
    ?classCRgn@CRgn@@2UCRuntimeClass@@A=_ZN4CRgn9classCRgnE
    ?classCDC@CDC@@2UCRuntimeClass@@A=_ZN3CDC8classCDCE
    ?classCClientDC@CClientDC@@2UCRuntimeClass@@A=_ZN9CClientDC14classCClientDCE
    ?classCPaintDC@CPaintDC@@2UCRuntimeClass@@A=_ZN8CPaintDC13classCPaintDCE
    ?classCWindowDC@CWindowDC@@2UCRuntimeClass@@A=_ZN9CWindowDC14classCWindowDCE
    ; CMenu runtime class
    ?classCMenu@CMenu@@2UCRuntimeClass@@A=_ZN5CMenu10classCMenuE
    ; CFrameWnd runtime class
    ?classCFrameWnd@CFrameWnd@@2UCRuntimeClass@@A=_ZN9CFrameWnd14classCFrameWndE
EOF_OPENMFC_EXPORTS
echo "Added OpenMFC-specific static class member exports to .def file"

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
    "$ROOT/phase4/src/memcore.cpp"
    "$ROOT/phase4/src/strcore.cpp"
    "$ROOT/phase4/src/wincore.cpp"
    "$ROOT/phase4/src/dlgcore.cpp"
    "$ROOT/phase4/src/gdicore.cpp"
    "$ROOT/phase4/src/menucore.cpp"
    "$ROOT/phase4/src/regcore.cpp"
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

# Win32 import libs required by current implementations.
# Keep this list minimal and add as implementations grow.
LDLIBS=(
    -lgdi32
    -luser32
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
    "${LDFLAGS[@]}" \
    "${LDLIBS[@]}" \
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
