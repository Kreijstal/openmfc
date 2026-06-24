#!/usr/bin/env bash
# Build & run an MFC probe against the REAL atlmfc headers + real mfc140u.dll under
# wine, to harvest ground-truth class layout / behavior for the "real class family"
# effort. Unlike the #include-the-.cpp logic tests, this links the actual MSVC-ABI
# mfc140u export surface and runs it, so sizeof/offsetof/serialize bytes are authoritative.
#
# Usage: build_family_probe.sh <probe.cpp>   (prints the probe's stdout)
#
# Requires a local msvc-wine toolchain (clang-cl, lld-link, wine) plus the real atlmfc
# headers/libs and the x64 redist DLLs. Set MSVC=/path/to/sdk-root to point at it; if
# unset, common locations are auto-detected. Creates exact-case header/lib symlinks on
# first run (Linux is case-sensitive; MFC #includes are mixed-case).
set -euo pipefail
# Resolve the MSVC SDK root: explicit $MSVC wins, else probe common install dirs.
if [ -z "${MSVC:-}" ]; then
  for cand in "$HOME/msvc" /opt/msvc /tmp/msvc/unpack /tmp/msvc-dl; do
    [ -d "$cand/VC/Tools/MSVC" ] && { MSVC="$cand"; break; }
  done
fi
MSVC="${MSVC:-$HOME/msvc}"
[ -d "$MSVC/VC/Tools/MSVC" ] || { echo "ERROR: MSVC SDK not found under '$MSVC' (set MSVC=/path/to/sdk-root)"; exit 1; }
MSVC_VER="$(ls -1 "$MSVC/VC/Tools/MSVC" | sort -V | tail -1)"
SDK_VER="$(ls -1 "$MSVC/Windows Kits/10/Include" | sort -V | tail -1)"
PROBE="${1:?usage: build_family_probe.sh <probe.cpp> [dll-to-stage]}"
# Optional 2nd arg: a DLL (e.g. our built openmfc.dll) to stage into the run dir and
# pass to the probe as argv[1]. The ctor probe LoadLibrary's it explicitly; without
# this the probe would fall back to whatever "openmfc.dll" resolves to in cwd.
DLL_TO_STAGE="${2:-}"
WORK="${WORK:-$(mktemp -d)}"

INC_MSVC="$MSVC/VC/Tools/MSVC/$MSVC_VER/include"
INC_ATL="$MSVC/VC/Tools/MSVC/$MSVC_VER/atlmfc/include"
INC_UCRT="$MSVC/Windows Kits/10/Include/$SDK_VER/ucrt"
INC_UM="$MSVC/Windows Kits/10/Include/$SDK_VER/um"
INC_SHARED="$MSVC/Windows Kits/10/Include/$SDK_VER/shared"
INC_WINRT="$MSVC/Windows Kits/10/Include/$SDK_VER/winrt"
LIB_MSVC="$MSVC/VC/Tools/MSVC/$MSVC_VER/lib/x64"
LIB_ATL="$MSVC/VC/Tools/MSVC/$MSVC_VER/atlmfc/lib/x64"
LIB_UCRT="$MSVC/Windows Kits/10/Lib/$SDK_VER/ucrt/x64"
LIB_UM="$MSVC/Windows Kits/10/Lib/$SDK_VER/um/x64"

# 1. exact-case header symlinks (mixed-case #includes vs case-sensitive FS)
python3 - "$INC_SHARED" "$INC_UM" "$INC_UCRT" "$INC_WINRT" "$INC_MSVC" "$INC_ATL" <<'PY'
import os, re, sys, glob
roots = [d for d in sys.argv[1:] if os.path.isdir(d)]
inc_re = re.compile(r'#\s*include\s*[<"]([^>"]+)[>"]')
refs = set()
for d in roots:
    for f in glob.glob(os.path.join(d, "*")):
        if not os.path.isfile(f): continue
        try: txt = open(f, encoding='utf-8', errors='ignore').read()
        except Exception: continue
        for m in inc_re.findall(txt):
            refs.add(m.replace('\\','/').split('/')[-1])
for d in roots:
    actual = {}
    for f in os.listdir(d): actual.setdefault(f.lower(), f)
    for ref in refs:
        tgt = os.path.join(d, ref)
        if os.path.exists(tgt): continue
        real = actual.get(ref.lower())
        if real and real != ref:
            try: os.symlink(real, tgt)
            except FileExistsError: pass
PY
# lib case symlinks
for d in "$LIB_ATL" "$LIB_MSVC" "$LIB_UCRT" "$LIB_UM"; do
  ( cd "$d"; for f in *.lib *.Lib; do [ -f "$f" ] || continue
      l="$(echo "$f"|tr '[:upper:]' '[:lower:]')"; [ "$f" != "$l" ] && [ ! -e "$l" ] && ln -sf "$f" "$l"; done ) || true
done

mkdir -p "$WORK"
clang-cl /nologo /EHsc /MD /std:c++17 /D_AFXDLL /DUNICODE /D_UNICODE /D_WIN32_WINNT=0x0A00 /W0 \
  -Wno-invalid-offsetof \
  -imsvc "$INC_ATL" -imsvc "$INC_MSVC" -imsvc "$INC_UCRT" -imsvc "$INC_UM" -imsvc "$INC_SHARED" \
  -c "$PROBE" /Fo"$WORK/probe.obj"
lld-link /nologo /OUT:"$WORK/probe.exe" /SUBSYSTEM:CONSOLE \
  /LIBPATH:"$LIB_ATL" /LIBPATH:"$LIB_MSVC" /LIBPATH:"$LIB_UM" /LIBPATH:"$LIB_UCRT" "$WORK/probe.obj"

# 2. stage real redist DLLs next to the exe
RED="$MSVC/VC/Redist"
mfcdir="$(find "$RED" -type d -path '*x64*Microsoft.VC*.MFC' 2>/dev/null | sort | tail -1)"
crtdir="$(find "$RED" -type d -path '*x64*Microsoft.VC*.CRT' 2>/dev/null | sort | tail -1)"
cp -f "$mfcdir"/*.dll "$WORK"/ 2>/dev/null || true
cp -f "$crtdir"/*.dll "$WORK"/ 2>/dev/null || true

# 3. optionally stage a DLL-under-test (e.g. our openmfc.dll) and pass it to the probe.
PROBE_ARGS=()
if [ -n "$DLL_TO_STAGE" ]; then
  [ -f "$DLL_TO_STAGE" ] || { echo "ERROR: dll-to-stage '$DLL_TO_STAGE' not found"; exit 1; }
  cp -f "$DLL_TO_STAGE" "$WORK"/
  PROBE_ARGS=("$(basename "$DLL_TO_STAGE")")
fi

( cd "$WORK"; WINEDEBUG=-all timeout 60 wine probe.exe "${PROBE_ARGS[@]}" 2>/dev/null )
