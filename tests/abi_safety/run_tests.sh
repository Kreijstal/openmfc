#!/usr/bin/env bash
set -eu

# ABI Safety Test Runner
# Runs all tests for MSVC ABI safety tools

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TEST_DIR="$ROOT/tests/abi_safety"

echo "=== Running ABI Safety Tests ==="
echo ""

# Run core logic tests
echo "1. Testing core MSVC ABI logic..."
if python3 "$TEST_DIR/test_core_logic.py"; then
    echo "  ✅ PASS"
else
    echo "  ❌ FAIL"
    exit 1
fi

echo ""

# Verify generated fallback sources never count as implementations
echo "2. Testing manifest fallback classification..."
if python3 "$TEST_DIR/test_manifest_classification.py"; then
    echo "  ✅ PASS"
else
    echo "  ❌ FAIL"
    exit 1
fi

echo ""

# Run end-to-end test
echo "3. Running end-to-end test..."
if "$TEST_DIR/test_end_to_end.sh"; then
    echo "  ✅ PASS"
else
    echo "  ❌ FAIL"
    exit 1
fi

echo ""
echo "=== All ABI Safety Tests Complete ==="
echo ""
echo "✅ Test suite validates that:"
echo "   1. Core MSVC vs GCC mangling logic works"
echo "   2. End-to-end safety system functions"
echo "   3. Phase 4 produces MSVC-compatible output"
echo ""
echo "📋 MSVC ABI Safety System includes:"
echo "   - test_msvc_abi_simple.sh: Checks DLL exports"
echo "   - validate_implementation_safety.py: Validates source files"
echo "   - phase4_safety_check.sh: Comprehensive check"
echo ""
echo "💡 For development workflow:"
echo "   1. Write implementation with extern \"C\" + MS_ABI + stub_ prefix"
echo "   2. Build: ./phase4/scripts/build_phase4.sh"
echo "   3. Check: ./scripts/phase4_safety_check.sh"
echo "   4. CI will verify actual MSVC linking"
