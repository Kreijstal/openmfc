#!/usr/bin/env bash
set -euo pipefail
SO=${SO:-build/lib/libopenmfc.so}

if [ ! -f "$SO" ]; then
  echo "missing $SO" >&2
  exit 1
fi

echo "Checking for AfxThrowMemoryException..."
if ! nm -D "$SO" | grep -q "AfxThrowMemoryException"; then
  echo "missing AfxThrowMemoryException" >&2
  exit 1
fi

echo "Checking for TI_CMemoryException..."
if ! nm -D "$SO" | grep -q "TI_CMemoryException"; then
  echo "missing TI_CMemoryException" >&2
  exit 1
fi

echo "Smoke test passed!"
