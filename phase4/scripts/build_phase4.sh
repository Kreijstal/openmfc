#!/usr/bin/env bash
set -eu

# Compatibility entry point for the native Windows/MSVC build. The Python
# driver rejects non-MSVC C++ ABIs and never generates default-return stubs.
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD="${BUILD:-$ROOT/build-phase4}"
args=(--build "$BUILD")
if [[ -n "${CXX:-}" ]]; then
    args+=(--compiler "$CXX")
fi
if [[ -n "${MSVC_DIR:-}" ]]; then
    args+=(--sdk "$MSVC_DIR")
fi
exec "${PYTHON:-python3}" "$ROOT/phase4/scripts/build_msvc.py" "${args[@]}" "$@"
