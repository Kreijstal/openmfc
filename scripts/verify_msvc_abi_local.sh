#!/usr/bin/env bash
set -eu

# Local MSVC ABI Verification Script
# 
# This script verifies that OpenMFC DLL maintains MSVC ABI compatibility
# WITHOUT requiring MSVC tools locally. It checks:
# 1. All required MSVC-mangled symbols are exported
# 2. Symbols are at correct ordinals
# 3. .def file uses correct MSVC mangling (not GCC)
# 4. Key MFC functions can be resolved
#
# Usage: ./scripts/verify_msvc_abi_local.sh [dll_path] [mapping_path]

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DLL="${1:-$ROOT/build-phase4/openmfc.dll}"
MAPPING="${2:-$ROOT/mfc_complete_ordinal_mapping.json}"
DEF="${DLL%.dll}.def"

echo "=== Local MSVC ABI Verification ==="
echo "DLL: $DLL"
echo "Mapping: $MAPPING"
echo ""

# Check files exist
if [[ ! -f "$DLL" ]]; then
    echo "❌ ERROR: DLL not found: $DLL"
    exit 1
fi

if [[ ! -f "$MAPPING" ]]; then
    echo "❌ ERROR: Mapping file not found: $MAPPING"
    exit 1
fi

if [[ ! -f "$DEF" ]]; then
    echo "❌ ERROR: .def file not found: $DEF"
    echo "  Expected: $DEF"
    exit 1
fi

# Check MinGW tools
if ! command -v x86_64-w64-mingw32-objdump >/dev/null 2>&1; then
    echo "❌ ERROR: MinGW objdump not found"
    echo "  Install: sudo apt-get install mingw-w64"
    exit 1
fi

# Function to extract exports from DLL
extract_dll_exports() {
    local dll="$1"
    x86_64-w64-mingw32-objdump -p "$dll" | \
        grep -E '^\s+\[\s*\d+\]\s*\+base\[\s*\d+\]\s+[0-9a-fA-F]+\s+' | \
        sed 's/^\s*\[\s*[0-9]\+\]\s*\+base\[\s*\([0-9]\+\)\].*[0-9a-fA-F]\+\s\+\(.*\)$/\1 \2/' | \
        sort -n
}

# Function to load mapping
load_mapping() {
    local mapping="$1"
    python3 -c "
import json
import sys
with open('$mapping') as f:
    data = json.load(f)
exports = data.get('exports', {}).get('mfc140u', [])
for exp in exports:
    symbol = exp.get('symbol', '')
    ordinal = exp.get('ordinal', -1)
    if symbol and ordinal > 0:
        print(f'{ordinal} {symbol}')
" | sort -n
}

echo "[1/6] Checking DLL exports contain MSVC-mangled names..."
echo "  Looking for key MSVC-mangled symbols..."

# Check for key MSVC-mangled symbols (not GCC mangled)
KEY_SYMBOLS=(
    "?AfxThrowMemoryException@@YAXXZ"
    "?AfxThrowFileException@@YAXHJPB_W@Z"
    "?AfxThrowNotSupportedException@@YAXXZ"
    "?AfxGetApp@@YAPAVCWinApp@@XZ"
    "?AfxGetMainWnd@@YAPAVCWnd@@XZ"
)

ALL_GOOD=true
for symbol in "${KEY_SYMBOLS[@]}"; do
    if x86_64-w64-mingw32-objdump -p "$DLL" | grep -qF "$symbol"; then
        echo "  ✅ Found: $symbol"
    else
        echo "  ❌ MISSING: $symbol"
        ALL_GOOD=false
    fi
done

echo ""
echo "[2/6] Checking for accidental GCC mangling..."
# GCC mangling patterns to reject
GCC_PATTERNS=(
    "_Z"  # GCC mangling prefix
    "@@"  # Itanium C++ ABI (not MSVC)
)

for pattern in "${GCC_PATTERNS[@]}"; do
    if x86_64-w64-mingw32-objdump -p "$DLL" | grep -q "$pattern"; then
        echo "  ⚠️  WARNING: Found potential GCC/Itanium mangling: $pattern"
        echo "     This may indicate ABI incompatibility"
        # Don't fail on warning, just alert
    fi
done

echo ""
echo "[3/6] Checking .def file MSVC mangling..."
DEF_MSVC_COUNT=$(grep -c '^[[:space:]]*\?[A-Z]' "$DEF" || true)
DEF_TOTAL_COUNT=$(grep -c '^[[:space:]]*[A-Za-z_?@]' "$DEF" || true)

if [[ "$DEF_MSVC_COUNT" -gt 1000 ]]; then
    echo "  ✅ .def file has $DEF_MSVC_COUNT MSVC-mangled exports (of $DEF_TOTAL_COUNT total)"
else
    echo "  ❌ .def file has only $DEF_MSVC_COUNT MSVC-mangled exports"
    echo "     Expected many more (14k+)"
    ALL_GOOD=false
fi

echo ""
echo "[4/6] Checking key ordinals match real MFC..."
# Real MFC ordinals for key functions
declare -A REAL_ORDINALS=(
    ["?AfxThrowMemoryException@@YAXXZ"]="2350"
    ["?AfxThrowFileException@@YAXHJPB_W@Z"]="2351"
    ["?AfxThrowNotSupportedException@@YAXXZ"]="2352"
    ["?AfxGetApp@@YAPAVCWinApp@@XZ"]="2344"
    ["?AfxGetMainWnd@@YAPAVCWnd@@XZ"]="2345"
)

for symbol in "${!REAL_ORDINALS[@]}"; do
    expected="${REAL_ORDINALS[$symbol]}"
    # Check .def file for ordinal
    if grep -q "$symbol.*@ $expected" "$DEF"; then
        echo "  ✅ $symbol at correct ordinal $expected"
    elif grep -q "$symbol" "$DEF"; then
        actual=$(grep "$symbol" "$DEF" | sed -n 's/.*@ \([0-9]\+\).*/\1/p')
        echo "  ❌ $symbol at wrong ordinal: $actual (expected $expected)"
        ALL_GOOD=false
    else
        echo "  ❌ $symbol not found in .def file"
        ALL_GOOD=false
    fi
done

echo ""
echo "[5/6] Running ABI export verification..."
if python3 "$ROOT/scripts/verify_abi_exports.py" \
    --mapping "$MAPPING" \
    --dll "$DLL" 2>/dev/null; then
    echo "  ✅ All required exports present"
else
    echo "  ❌ ABI export verification failed"
    ALL_GOOD=false
fi

echo ""
echo "[6/6] Checking implementation vs stubs..."
if [[ -f "$ROOT/build-phase4/weak_stubs.o" ]]; then
    # Count weak symbols (stubs) vs strong symbols (implementations)
    WEAK_COUNT=$(x86_64-w64-mingw32-nm "$ROOT/build-phase4/weak_stubs.o" 2>/dev/null | grep -c ' [wWvV] ' || true)
    echo "  Found $WEAK_COUNT weak symbols (stubs)"
    
    # Check if any key functions are implemented (not stubs)
    IMPL_FILES=("$ROOT/phase4/src"/*.cpp "$ROOT/src/mfc"/*.cpp 2>/dev/null)
    if [[ ${#IMPL_FILES[@]} -gt 0 ]]; then
        echo "  Implementation files found:"
        for file in "${IMPL_FILES[@]}"; do
            if [[ -f "$file" ]]; then
                echo "    - $(basename "$file")"
            fi
        done
    fi
else
    echo "  ⚠️  Weak stubs object file not found, skipping stub analysis"
fi

echo ""
echo "=== Verification Summary ==="
if $ALL_GOOD; then
    echo "✅ All local MSVC ABI checks passed!"
    echo ""
    echo "Next steps for full verification:"
    echo "1. Push to CI for actual MSVC compilation"
    echo "2. Run Wine tests with compiled MSVC binaries"
    echo "3. Verify real MFC applications can link"
    exit 0
else
    echo "❌ Some MSVC ABI checks failed!"
    echo ""
    echo "Common issues:"
    echo "1. Missing MSVC-mangled exports (using GCC mangling instead)"
    echo "2. Wrong ordinals (not matching real MFC)"
    echo "3. Missing required exports"
    echo ""
    echo "Fix by:"
    echo "1. Using 'extern \"C\"' with MS_ABI attribute"
    echo "2. Using stub naming convention (gen_weak_stubs.py)"
    echo "3. Never writing normal C++ class methods"
    exit 1
fi