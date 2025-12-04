#!/usr/bin/env bash
set -euo pipefail
DLL=${DLL:-openmfc.dll}
MIN_EXPORTS=${MIN_EXPORTS:-100}
KEY_SYMBOL=${KEY_SYMBOL:-"?AfxThrowMemoryException@@YAXXZ"}

if ! command -v x86_64-w64-mingw32-objdump >/dev/null 2>&1; then
  echo "objdump (mingw) required" >&2
  exit 1
fi

if [ ! -f "$DLL" ]; then
  echo "missing $DLL" >&2
  exit 1
fi

EXP_COUNT=$(x86_64-w64-mingw32-objdump -p "$DLL" | grep -c 'Ordinal')
if [ "$EXP_COUNT" -lt "$MIN_EXPORTS" ]; then
  echo "export count too low: $EXP_COUNT < $MIN_EXPORTS" >&2
  exit 1
fi

if ! x86_64-w64-mingw32-objdump -p "$DLL" | grep -q "$KEY_SYMBOL"; then
  echo "missing key symbol $KEY_SYMBOL" >&2
  exit 1
fi

echo "Smoke test passed: exports=$EXP_COUNT key=$KEY_SYMBOL"
