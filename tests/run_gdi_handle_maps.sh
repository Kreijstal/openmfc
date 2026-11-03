#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
ARGS=(--compiler "${COMPILER:-clang-cl}" --linker "${LINKER:-lld-link}"
    --sdk "${MSVC_DIR:-/home/kreijstal/msvc}"
    --build "${BUILD_DIR:-/tmp/openmfc-pi-gdi}")
if [[ "${OPTIMIZE:-0}" == 1 ]]; then ARGS+=(--optimize); fi
if [[ -n "${NATIVE_DLL:-}" ]]; then ARGS+=(--native-dll "$NATIVE_DLL"); fi
exec python3 "$ROOT/tests/build_gdi_handle_maps.py" "${ARGS[@]}"
