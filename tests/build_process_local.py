#!/usr/bin/env python3
"""Build native critical-section and process-local lifetime behavior."""
import argparse
import concurrent.futures
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent


def sources():
    return ['phase4/src/core/runtime/GlobalLocks.cpp',
            'phase4/src/core/runtime/CProcessLocalObject.cpp',
            'phase4/src/detail/CProcessLocalObjectSupport.cpp',
            'phase4/src/core/runtime/_AFX_THREAD_STATE.cpp',
            'phase4/src/core/runtime/CThreadLocalObject.cpp',
            'phase4/src/core/runtime/CThreadSlotData.cpp',
            'phase4/src/detail/CThreadSlotDataSupport.cpp',
            'phase4/src/detail/CThreadLocalObjectSupport.cpp',
            'tests/test_process_local.cpp']


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--compiler', default='cl' if os.name == 'nt' else 'clang-cl')
    parser.add_argument('--sdk', type=Path)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--optimize', action='store_true')
    parser.add_argument('--jobs', type=int, default=4)
    args = parser.parse_args()
    out = args.build.resolve()
    out.mkdir(parents=True, exist_ok=True)
    flags = ['/nologo', '/EHs', '/MD', '/std:c++17', '/Gy', '/Gw', '/W0',
             '/DUNICODE', '/D_UNICODE', '/DNOMINMAX', '/D_WIN32_WINNT=0x0601',
             '/FIwinsock2.h', '/FIobjbase.h', '/FIcommdlg.h', '/FIshellapi.h',
             '/I' + str(ROOT / 'include'), '/I' + str(ROOT / 'phase4/src')]
    link = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_process_local.exe')]
    if args.optimize:
        flags.append('/O2')
    if args.sdk:
        versions = lambda path: sorted(path.iterdir(), key=lambda p: [int(n) for n in p.name.split('.')])[-1]
        vc = versions(args.sdk / 'VC/Tools/MSVC')
        sdk = versions(args.sdk / 'Windows Kits/10/Include')
        includes = [vc / 'include'] + [sdk / name for name in ('ucrt', 'um', 'shared')]
        for path in includes:
            flags.extend(['-imsvc', str(path)] if 'clang-cl' in args.compiler else ['/I' + str(path)])
        libraries = [vc / 'lib/x64'] + [args.sdk / 'Windows Kits/10/Lib' / sdk.name / name / 'x64' for name in ('ucrt', 'um')]
        link.extend('/LIBPATH:' + str(path) for path in libraries)
    env = dict(os.environ)
    env.setdefault('WINEDEBUG', '-all')

    def compile_one(relative):
        obj = out / (relative.removesuffix('.cpp').replace('/', '_') + '.obj')
        subprocess.run([args.compiler, *flags, '/c', str(ROOT / relative), '/Fo' + str(obj)],
                       check=True, cwd=out, env=env)
        return obj

    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        objects = list(pool.map(compile_one, sources()))
    sys.path.insert(0, str(ROOT / 'phase4/scripts'))
    import build_msvc
    checked=['phase4/src/core/runtime/GlobalLocks.cpp','phase4/src/core/runtime/CProcessLocalObject.cpp']
    defined,names=set(),set()
    for source in checked:
        obj=out / (source.removesuffix('.cpp').replace('/','_')+'.obj')
        defined.update(build_msvc.defined_symbols(obj))
        names.update(re.findall(r'^// Symbol: (.+)$',(ROOT/source).read_text(),re.M))
    import json
    entries=[entry for entry in json.loads((ROOT/'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u'] if entry['symbol'] in names]
    assert len(entries)==len(names)==6
    _,missing=build_msvc.export_definition(entries,defined)
    assert not missing,missing
    print('All six critical-section and process-local exports have actual providers',flush=True)
    link.extend(str(obj) for obj in objects)
    link.extend(['kernel32.lib', 'user32.lib'])
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    if os.name == 'nt':
        command = [str(out / 'test_process_local.exe')]
    else:
        if args.sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll', 'msvcp140.dll'):
                for runtime in (args.sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1,msvcp140=n')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') else []) + ['wine', str(out / 'test_process_local.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == '__main__':
    main()
