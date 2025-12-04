#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/.." && pwd)
BUILD=${BUILD:-$ROOT/build}
DB=${DB:-$ROOT/artifacts/mfc_db.json}
EXC=${EXC:-$ROOT/artifacts/exceptions.json}
LAYOUTS=${LAYOUTS:-$ROOT/artifacts/layouts.json}

mkdir -p "$BUILD/src" "$BUILD/include/openmfc"

if [ ! -f "$DB" ]; then
  echo "Missing mfc_db.json at $DB" >&2; exit 1;
fi
if [ ! -f "$EXC" ]; then
  echo "Missing exceptions.json at $EXC" >&2; exit 1;
fi

python "$ROOT/tools/gen_stubs.py" --db "$DB" --out-def "$BUILD/openmfc.def" --out-stubs "$BUILD/stubs.cpp"
python "$ROOT/tools/gen_rtti.py" --exceptions "$EXC" --out-c "$BUILD/generated_rtti.c" --out-h "$BUILD/include/openmfc/eh_rtti.h"

CXX=${CXX:-x86_64-w64-mingw32-g++}
CFLAGS=( -std=c++17 -O2 -Wall -Wextra -D_UNICODE -DUNICODE -I"$ROOT/include" -I"$BUILD/include" )
LDFLAGS=( -shared -static-libgcc -static-libstdc++ -Wl,--out-implib,"$BUILD/libopenmfc.a" )

"$CXX" "${CFLAGS[@]}" -c "$BUILD/stubs.cpp" -o "$BUILD/stubs.o"
"$CXX" "${CFLAGS[@]}" -c "$BUILD/generated_rtti.c" -o "$BUILD/generated_rtti.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/exceptions.c" -o "$BUILD/exceptions.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/afxmem.cpp" -o "$BUILD/afxmem.o"

"$CXX" "$BUILD/stubs.o" "$BUILD/generated_rtti.o" "$BUILD/exceptions.o" "$BUILD/afxmem.o" "${LDFLAGS[@]}" -o "$BUILD/openmfc.dll"

echo "Built $BUILD/openmfc.dll"
