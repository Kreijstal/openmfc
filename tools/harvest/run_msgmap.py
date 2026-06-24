#!/usr/bin/env python3
"""Drive one message-map batch: generate -> syntax-check -> resolve test bases ->
ABI build -> safety -> re-probe (the batch's exact classes must flip stub->valid).
Usage: run_msgmap.py <batch_id>
"""
import json, os, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def sh(cmd, cwd=ROOT):
    return subprocess.run(cmd, shell=True, cwd=cwd, text=True, capture_output=True)

def main():
    bid = sys.argv[1]
    sched = json.load(open(f'{ROOT}/tools/harvest/msgmap_schedule.json'))
    info = sched[bid]
    slug, classes = info['slug'], info['classes']
    print(f'== msgmap batch {bid} ({slug}): {len(classes)} classes')
    r = sh(f'python3 tools/harvest/gen_msgmap_batch.py {slug} {" ".join(classes)}')
    print(r.stdout.strip() or r.stderr.strip())
    if r.returncode: sys.exit('gen failed')
    flags = ('-std=c++17 -fpermissive -fms-extensions -Wno-attributes -D_WIN32_WINNT=0x0601 '
             '-DUNICODE -D_UNICODE -DOPENMFC_EXPORTS -Iinclude -Ibuild-phase4/include')
    r = sh(f'x86_64-w64-mingw32-g++ {flags} -fsyntax-only phase4/src/global_{slug}_msgmap.cpp')
    if r.returncode: print(r.stderr[-2000:]); sys.exit('cpp syntax FAILED')
    print('  cpp syntax OK')
    r = sh(f'bash tools/harvest/fix_test_bases.sh {slug} msgmap')
    line = (r.stdout.strip().splitlines() or [''])[-1]
    print('  ' + (line or r.stderr.strip()[-300:]))
    if r.returncode: sys.exit('test link FAILED')
    print('  building ABI DLL ...')
    r = sh('bash phase4/scripts/build_phase4.sh')
    blob = r.stdout + r.stderr
    if 'NO ABI REGRESSIONS' not in blob or '14,109' not in blob:
        print('\n'.join(blob.splitlines()[-25:])); sys.exit('ABI build FAILED')
    print('  ABI build OK (14,109, no regression)')
    sh('cp -f build-phase4/openmfc.dll /tmp/rttiharvest/openmfc_ours.dll')
    r = subprocess.run('WINEDEBUG=-all wine msgmap_probe.exe openmfc_ours.dll',
                       shell=True, cwd='/tmp/rttiharvest', text=True, capture_output=True)
    failed = {ln.split()[1] for ln in r.stdout.splitlines() if ln.startswith('FAIL ')}
    bad = [c for c in classes if c in failed]
    if bad:
        for c in bad:
            for ln in r.stdout.splitlines():
                if ln.startswith(f'FAIL {c} '): print('  ', ln)
        sys.exit(f'probe FAILED: {len(bad)} batch classes still stub/wrong')
    print(f'  PROBE OK: all {len(classes)} classes flip stub->valid map (base chain correct)')
    print('BATCH PASS')

if __name__ == '__main__':
    main()
