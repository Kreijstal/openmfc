#!/usr/bin/env python3
"""Drive one RTTI batch locally: generate -> resolve test bases -> ABI build ->
safety check -> re-probe built DLL to confirm the batch's classes flipped
stub->real with values matching mfc140u. Prints a PASS/FAIL summary.

Usage: run_batch.py <batch_id> <slug>
Does NOT touch git; caller handles branch/commit/push/PR/merge after PASS.
"""
import json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def sh(cmd, **kw):
    return subprocess.run(cmd, shell=True, cwd=ROOT, text=True,
                          capture_output=True, **kw)

def main():
    bid, slug = sys.argv[1], sys.argv[2]
    sched = json.load(open(f'{ROOT}/tools/harvest/batch_schedule.json'))
    incs = json.load(open(f'{ROOT}/tools/harvest/batch_includes.json'))
    classes = sched[bid]['classes']
    includes = ','.join(incs[bid])

    print(f'== batch {bid} ({slug}): {len(classes)} classes; includes={includes}')
    r = sh(f'python3 tools/harvest/gen_rtti_batch.py {slug} "{includes}" {" ".join(classes)}')
    print(r.stdout.strip() or r.stderr.strip())
    if r.returncode: sys.exit('gen failed')

    # syntax check the .cpp with build flags
    flags = ('-std=c++17 -fpermissive -fms-extensions -Wno-attributes -D_WIN32_WINNT=0x0601 '
             '-DUNICODE -D_UNICODE -DOPENMFC_EXPORTS -Iinclude -Ibuild-phase4/include')
    r = sh(f'x86_64-w64-mingw32-g++ {flags} -fsyntax-only phase4/src/global_{slug}_rtti.cpp')
    if r.returncode:
        print(r.stderr[-2000:]); sys.exit('cpp syntax FAILED')
    print('  cpp syntax OK')

    r = sh(f'bash tools/harvest/fix_test_bases.sh {slug}')
    print('  ' + (r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr.strip()[-300:]))
    if r.returncode: sys.exit('test link FAILED')

    print('  building ABI DLL ...')
    r = sh('bash phase4/scripts/build_phase4.sh')
    blob = r.stdout + r.stderr
    if 'NO ABI REGRESSIONS' not in blob or '14,109' not in blob:
        tail = '\n'.join(blob.splitlines()[-25:]); print(tail); sys.exit('ABI build FAILED')
    print('  ABI build OK (14,109 exports, no regression)')

    # probe the freshly built DLL
    sh('cp -f build-phase4/openmfc.dll /tmp/rttiharvest/openmfc_ours.dll')
    r = subprocess.run('WINEDEBUG=-all wine rtti_probe.exe mfc140u_real.dll openmfc_ours.dll',
                       shell=True, cwd='/tmp/rttiharvest', text=True, capture_output=True)
    rows = {}
    for l in r.stdout.splitlines():
        l = l.strip()
        if l.startswith('{'):
            o = json.loads(l); rows[o['class']] = o
    bad = [c for c in classes if rows.get(c, {}).get('stub') != 0]
    if bad:
        for c in bad: print('   STILL STUB:', c, rows.get(c))
        sys.exit(f'probe FAILED: {len(bad)} classes still stub')
    print(f'  PROBE OK: all {len(classes)} classes now real (stub->real), values match mfc140u')
    print('BATCH PASS')

if __name__ == '__main__':
    main()
