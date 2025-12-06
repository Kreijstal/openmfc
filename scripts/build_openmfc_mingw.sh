#!/usr/bin/env bash
set -euo pipefail
ROOT=$(cd "$(dirname "$0")/.." && pwd)
BUILD=${BUILD:-$ROOT/build}
DB=${DB:-$ROOT/mfc_db_correct.json}
EXC=${EXC:-$ROOT/artifacts/exceptions.json}
LAYOUTS=${LAYOUTS:-$ROOT/artifacts/layouts.json}
CRT_DEF_DIR=${CRT_DEF_DIR:-$ROOT/artifacts}
PYTHON=${PYTHON:-python3}

mkdir -p "$BUILD/src" "$BUILD/include/openmfc"

if [ ! -f "$DB" ]; then
  alt_db=$(find "$ROOT/artifacts" -name mfc_db.json | head -n1 || true)
  if [ -n "$alt_db" ]; then
    DB=$alt_db
  else
    echo "Missing mfc_db.json at $DB" >&2; exit 1;
  fi
fi
if python3 - "$DB" <<'PY'
import json,sys
data=json.load(open(sys.argv[1]))
exports=data.get("exports",{}).get("mfc140u",[])
sys.exit(0 if exports else 1)
PY
then
  :
else
  exp_file=$(find "$ROOT/artifacts" -name mfc.exports | head -n1 || true)
  meta_file=$(find "$ROOT/artifacts" -name metadata.json | head -n1 || true)
  demangled=$(find "$ROOT/artifacts" -name demangled.txt | head -n1 || true)
  if [ -z "$exp_file" ] || [ -z "$meta_file" ]; then
    echo "mfc_db.json is empty and exports/metadata not found to regenerate" >&2
    exit 1
  fi
  regen_args=(--dll mfc140u --exports "$exp_file" --metadata "$meta_file" --out "$DB")
  if [ -n "$demangled" ]; then
    regen_args+=(--demangled "$demangled")
  fi
  echo "Regenerating mfc_db.json from $exp_file"
  "$PYTHON" "$ROOT/tools/parse_exports.py" "${regen_args[@]}"
fi
if [ ! -f "$EXC" ]; then
  alt_exc=$(find "$ROOT/artifacts" -name exceptions.json | head -n1 || true)
  if [ -n "$alt_exc" ]; then
    EXC=$alt_exc
  else
    echo "Missing exceptions.json at $EXC" >&2; exit 1;
  fi
fi

"$PYTHON" "$ROOT/tools/gen_stubs.py" --db "$DB" --out-def "$BUILD/openmfc.def" --out-stubs "$BUILD/stubs.cpp"
"$PYTHON" "$ROOT/tools/gen_rtti.py" --exceptions "$EXC" --out-c "$BUILD/generated_rtti.c" --out-h "$BUILD/include/openmfc/eh_rtti.h"

CXX=${CXX:-x86_64-w64-mingw32-g++}
DLLTOOL=${DLLTOOL:-x86_64-w64-mingw32-dlltool}
CFLAGS=( -std=c++17 -O2 -Wall -Wextra -D_UNICODE -DUNICODE -I"$ROOT/include" -I"$BUILD/include" )
LDFLAGS=( -shared -static-libgcc -static-libstdc++ -Wl,--out-implib,"$BUILD/libopenmfc.a" )

# Build import libs for CRT if DEF files were downloaded
mkdir -p "$BUILD/libs"
if [ -f "$CRT_DEF_DIR/vcruntime140.def" ]; then
  "$DLLTOOL" -d "$CRT_DEF_DIR/vcruntime140.def" -l "$BUILD/libs/libvcruntime140.a"
  LDFLAGS+=( -L"$BUILD/libs" -lvcruntime140 )
fi
if [ -f "$CRT_DEF_DIR/ucrtbase.def" ]; then
  "$DLLTOOL" -d "$CRT_DEF_DIR/ucrtbase.def" -l "$BUILD/libs/libucrtbase.a"
  LDFLAGS+=( -L"$BUILD/libs" -lucrtbase )
fi

"$CXX" "${CFLAGS[@]}" -c "$BUILD/stubs.cpp" -o "$BUILD/stubs.o"
"$CXX" "${CFLAGS[@]}" -c "$BUILD/generated_rtti.c" -o "$BUILD/generated_rtti.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/exceptions.cpp" -o "$BUILD/exceptions.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/afxmem.cpp" -o "$BUILD/afxmem.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/strcore.cpp" -o "$BUILD/strcore.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/appcore.cpp" -o "$BUILD/appcore.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/dlgcore.cpp" -o "$BUILD/dlgcore.o"
"$CXX" "${CFLAGS[@]}" -c "$ROOT/src/mfc/winfrm.cpp" -o "$BUILD/winfrm.o"

"$CXX" "$BUILD/stubs.o" "$BUILD/generated_rtti.o" "$BUILD/exceptions.o" "$BUILD/afxmem.o" "$BUILD/strcore.o" "$BUILD/appcore.o" "$BUILD/dlgcore.o" "$BUILD/winfrm.o" "$BUILD/openmfc.def" "${LDFLAGS[@]}" -o "$BUILD/openmfc.dll"

echo "Built $BUILD/openmfc.dll"
