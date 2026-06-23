#!/usr/bin/env bash
# Iteratively resolve undefined "Class::classClass" framework statics in a
# generated RTTI logic test by appending placeholder definitions until it links.
# Usage: fix_test_bases.sh <cat>
set -u
cd "$(dirname "$0")/../.." || exit 1
CAT="$1"
TEST="tests/test_global_${CAT}_rtti_logic.cpp"
FLAGS="-std=c++17 -fpermissive -fms-extensions -Wno-attributes -D_WIN32_WINNT=0x0601 -DUNICODE -D_UNICODE -Iinclude -Ibuild-phase4/include"
for i in $(seq 1 40); do
  err=$(x86_64-w64-mingw32-g++ $FLAGS -o /tmp/test_${CAT}.exe "$TEST" 2>&1)
  if [ $? -eq 0 ]; then echo "LINK OK after $((i-1)) additions"; exit 0; fi
  # collect undefined "X::classX" symbols
  miss=$(echo "$err" | grep -oE "undefined reference to .[A-Za-z0-9_]+::class[A-Za-z0-9_]+" \
         | sed -E "s/.*to .//; s/::class.*//" | sort -u)
  if [ -z "$miss" ]; then echo "LINK FAILED (non-base-static error):"; echo "$err" | grep -i error | head; exit 1; fi
  add=""
  for c in $miss; do
    grep -q "CRuntimeClass ${c}::class${c}{};" "$TEST" || add="${add}CRuntimeClass ${c}::class${c}{};\n"
  done
  [ -z "$add" ] && { echo "no new bases to add but still failing"; echo "$err"|grep -i error|head; exit 1; }
  # insert before the closing marker
  python3 - "$TEST" "$add" <<'PY'
import sys
p, add = sys.argv[1], sys.argv[2].replace('\\n','\n')
s = open(p).read()
s = s.replace('// <<< BASE-STATICS', add + '// <<< BASE-STATICS', 1)
open(p,'w').write(s)
PY
done
echo "exceeded iterations"; exit 1
