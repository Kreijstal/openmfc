#!/usr/bin/env bash
set -eu
DLL=${DLL:-openmfc.dll}
MIN_EXPORTS=${MIN_EXPORTS:-100}
# Default to AfxThrowArchiveException (wchar_t version) - a symbol that exists in mfc140u.dll
KEY_SYMBOL=${KEY_SYMBOL:-"?AfxThrowArchiveException@@YAXHPB_W@Z"}

if ! command -v x86_64-w64-mingw32-objdump >/dev/null 2>&1; then
  echo "objdump (mingw) required" >&2
  exit 1
fi

if [ ! -f "$DLL" ]; then
  echo "missing $DLL" >&2
  exit 1
fi

# Count exported symbols by looking for [ordinal] pattern in the export section
EXP_COUNT=$(x86_64-w64-mingw32-objdump -p "$DLL" | grep -cE '^\s+\[[0-9]+\]')
if [ "$EXP_COUNT" -lt "$MIN_EXPORTS" ]; then
  echo "export count too low: $EXP_COUNT < $MIN_EXPORTS" >&2
  exit 1
fi

# Search for the symbol - strip leading ? since it can cause grep issues
SEARCH_PATTERN="${KEY_SYMBOL#\?}"
if ! x86_64-w64-mingw32-objdump -p "$DLL" | grep -qF "$SEARCH_PATTERN"; then
  echo "missing key symbol $KEY_SYMBOL" >&2
  exit 1
fi

echo "Smoke test passed: exports=$EXP_COUNT key=$KEY_SYMBOL"
