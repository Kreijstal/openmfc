#!/usr/bin/env bash
set -eu

# Simple MSVC ABI Test
# Tests the MOST CRITICAL aspect: MSVC mangling vs GCC mangling
# 
# This is the test that should run after every build to catch
# accidental GCC mangling before it breaks MSVC compatibility.

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DLL="${1:-$ROOT/build-phase4/openmfc.dll}"

echo "=== MSVC ABI Simple Test ==="
echo "Testing: $DLL"
echo ""

if [[ ! -f "$DLL" ]]; then
    echo "❌ ERROR: DLL not found"
    echo "  Build with: ./phase4/scripts/build_phase4.sh"
    exit 1
fi

if ! command -v x86_64-w64-mingw32-objdump >/dev/null 2>&1; then
    echo "❌ ERROR: MinGW objdump not found"
    echo "  Install: sudo apt-get install mingw-w64"
    exit 1
fi

# Extract exports
# Extract exports
# Handle both formats:
# 1. [   0] +base[ 256] 00001480 symbol_name
# 2. [   0] symbol_name
EXPORTS=$(x86_64-w64-mingw32-objdump -p "$DLL" 2>/dev/null | \
    grep -E '^[[:space:]]+\[[[:space:]]*[0-9]+\][[:space:]]+' | \
    sed -E 's/.*base\[[[:space:]]*[0-9]+\][[:space:]]+[0-9a-fA-F]+[[:space:]]+//' | \
    sed -E 's/^[[:space:]]+\[[[:space:]]*[0-9]+\][[:space:]]+//' | \
    grep -v "RVA exportieren" | grep -v "Export RVA" || true)

if [[ -z "$EXPORTS" ]]; then
    echo "❌ ERROR: Could not extract exports from DLL"
    exit 1
fi

TOTAL=$(echo "$EXPORTS" | wc -l)
echo "Total exports: $TOTAL"

# Check 1: Must have MSVC-mangled names (start with ?)
MSVC_COUNT=$(echo "$EXPORTS" | grep -c '^?' || true)
echo "MSVC-mangled exports: $MSVC_COUNT"

if [[ $MSVC_COUNT -lt 100 ]]; then
    echo "❌ FAIL: Not enough MSVC-mangled exports (need 100+, got $MSVC_COUNT)"
    echo "  This means GCC mangling is being used instead of MSVC mangling"
    echo "  MSVC applications will NOT be able to link against this DLL"
    exit 1
fi

# Check 2: Must NOT have GCC-mangled names (start with _Z)
GCC_COUNT=$(echo "$EXPORTS" | grep -c '^_Z' || true)
echo "GCC-mangled exports: $GCC_COUNT"

if [[ $GCC_COUNT -gt 0 ]]; then
    echo "❌ FAIL: Found $GCC_COUNT GCC-mangled exports"
    echo "  GCC mangling: _ZN7CObject15GetRuntimeClassEv"
    echo "  MSVC mangling: ?GetRuntimeClass@CObject@@UBEPAUCRuntimeClass@@XZ"
    echo "  These are INCOMPATIBLE - MSVC cannot use GCC-mangled names"
    exit 1
fi

# Check 3: Key MSVC symbols must be present
# Using symbols we know should be in mfc_complete_ordinal_mapping.json
KEY_SYMBOLS=(
    "?AfxThrowMemoryException@@YAXXZ"
    "?AfxThrowFileException@@YAXHJPB_W@Z"
    "?AfxThrowNotSupportedException@@YAXXZ"
    "?AfxThrowInvalidArgException@@YAXXZ"
    "?AfxAbort@@YAXXZ"
)

echo ""
echo "Checking key MSVC symbols:"
ALL_KEYS_FOUND=true
for symbol in "${KEY_SYMBOLS[@]}"; do
    if echo "$EXPORTS" | grep -qF "$symbol"; then
        echo "  ✅ $symbol"
    else
        echo "  ❌ $symbol (MISSING)"
        ALL_KEYS_FOUND=false
    fi
done

# Check 4: Actually, MSVC uses @@ in mangling, so this check is wrong
# Itanium would be things like _Z (GCC) or @ (non-MSVC)
# MSVC @@ is correct, so we'll skip this check

echo ""
if $ALL_KEYS_FOUND && [[ $MSVC_COUNT -gt 100 ]] && [[ $GCC_COUNT -eq 0 ]]; then
    echo "✅ PASS: DLL uses correct MSVC mangling"
    echo "  - $MSVC_COUNT MSVC-mangled exports"
    echo "  - 0 GCC-mangled exports"
    echo "  - All key symbols present"
    echo ""
    echo "MSVC applications SHOULD be able to link against this DLL."
    echo "Full verification requires actual MSVC compilation in CI."
    exit 0
else
    echo "❌ FAIL: MSVC ABI compatibility issues detected"
    echo ""
    echo "COMMON CAUSES:"
    echo "1. Writing normal C++ class methods (produces GCC mangling)"
    echo "2. Missing 'extern \"C\"' wrapper"
    echo "3. Missing MS_ABI attribute"
    echo "4. Using wrong stub names"
    echo ""
    echo "CORRECT PATTERN:"
    echo "  extern \"C\" void MS_ABI stub__AfxThrowMemoryException__YAXXZ() {"
    echo "      // implementation"
    echo "  }"
    echo ""
    echo "INCORRECT PATTERN:"
    echo "  void AfxThrowMemoryException() {  // GCC-mangled!"
    echo "      // implementation"
    echo "  }"
    exit 1
fi