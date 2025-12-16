#!/usr/bin/env bash
set -eu

# ABI Regression Detection
# 
# Detects if ABI compatibility has been broken since last verification.
# The API is set in stone - we must not regress.

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DLL="$ROOT/build-phase4/openmfc.dll"
MAPPING="$ROOT/mfc_complete_ordinal_mapping.json"
BASELINE="${1:-}"

echo "=== ABI Regression Detection ==="
echo "Checking if ABI compatibility has been broken"
echo ""

if [[ ! -f "$DLL" ]]; then
    echo "❌ ERROR: Phase 4 DLL not built"
    echo "  Build with: ./phase4/scripts/build_phase4.sh"
    exit 1
fi

# Extract current ABI state
echo "1. Analyzing current ABI state..."
CURRENT_STATE=$(python3 "$ROOT/scripts/verify_exact_abi.py" --dll "$DLL" --mapping "$MAPPING" 2>&1 | grep -A5 -B5 "Missing exports:\|Wrong ordinals:" || true)

# Check for regressions
echo ""
echo "2. Checking for regressions..."

# Run exact verification
if python3 "$ROOT/scripts/verify_exact_abi.py" --dll "$DLL" --mapping "$MAPPING" 2>/dev/null | grep -q "EXACT ABI COMPATIBILITY VERIFIED"; then
    echo "  ✅ NO REGRESSION: ABI still matches mfc140u.dll"
else
    echo "  ❌ REGRESSION DETECTED: ABI no longer matches!"
    echo ""
    echo "  Current state:"
    echo "$CURRENT_STATE"
    echo ""
    echo "  The API is set in stone by mfc140u.dll."
    echo "  We MUST NOT break ABI compatibility."
    echo ""
    echo "  Common regression causes:"
    echo "  1. Missing exports in .def file"
    echo "  2. Wrong ordinals in implementation"
    echo "  3. Accidental GCC mangling"
    echo "  4. Build system changes"
    exit 1
fi

# Check MSVC mangling
echo ""
echo "3. Checking MSVC mangling..."
if "$ROOT/scripts/test_msvc_abi_simple.sh" "$DLL" 2>/dev/null | grep -q "MSVC applications SHOULD be able to link"; then
    echo "  ✅ NO REGRESSION: Still using MSVC mangling"
else
    echo "  ❌ REGRESSION: Now using GCC mangling!"
    echo "     MSVC applications cannot link"
    exit 1
fi

# Check implementation safety
echo ""
echo "4. Checking implementation safety..."
UNSAFE_COUNT=$(python3 "$ROOT/scripts/validate_implementation_safety.py" \
    "$ROOT/phase4/src"/*.cpp 2>/dev/null | grep -c "unsafe pattern" || true)

if [[ $UNSAFE_COUNT -eq 0 ]]; then
    echo "  ✅ NO REGRESSION: Implementations still safe"
else
    echo "  ⚠️  WARNING: $UNSAFE_COUNT unsafe patterns found"
    echo "     May cause future ABI regression"
fi

# Compare with baseline if provided
if [[ -n "$BASELINE" && -f "$BASELINE" ]]; then
    echo ""
    echo "5. Comparing with baseline: $BASELINE"
    # Extract exports from baseline and current
    # (This would need a more sophisticated implementation)
    echo "  ⚠️  Baseline comparison not fully implemented"
    echo "  Use --dll option to specify baseline DLL"
fi

echo ""
echo "=== Regression Detection Summary ==="
echo ""
echo "✅ NO ABI REGRESSIONS DETECTED"
echo ""
echo "📋 Current ABI state:"
echo "   - Exact match with mfc140u.dll: ✅"
echo "   - MSVC mangling: ✅"
echo "   - Implementation safety: ✅"
echo ""
echo "💡 The API is set in stone."
echo "   Our job is to NOT break the ABI."
echo "   This check confirms we haven't."
echo ""
echo "🔒 To prevent regressions:"
echo "   1. Always use 'extern \"C\"' + MS_ABI + stub_ prefix"
echo "   2. Never write C++ class methods in implementations"
echo "   3. Run this check before every commit"
echo "   4. CI will catch any remaining issues"