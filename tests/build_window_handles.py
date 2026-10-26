#!/usr/bin/env python3
"""Build the real window lifetime/map units with the existing virtual siblings."""
import argparse
import concurrent.futures
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent


def sources():
    runner = (ROOT / 'tests/run_rendertarget_drawing.sh').read_text()
    body = re.search(r'SOURCES=\((.*?)\)\nOBJECTS=', runner, re.S).group(1)
    shared = [s for s in shlex.split(body) if not s.startswith('tests/')]
    return shared + ['phase4/src/core/window/CWndHandleMap.cpp',
                     'phase4/src/detail/MemcoreSupport.cpp',
                     'phase4/src/core/d2d/CRenderTarget.cpp',
                     'phase4/src/core/d2d/CD2DTextFormat.cpp',
                     'phase4/src/core/d2d/CD2DTextLayout.cpp',
                     'tests/test_window_handles.cpp']


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
    link = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_window_handles.exe')]
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
    sys.path.insert(0, str(ROOT / 'tools'))
    import gen_weak_stubs
    count = 0
    for relative in ('core/window/CWndConstruction.cpp', 'core/window/CWndHandleMap.cpp',
                     'core/window/CWndLifetime.cpp', 'core/window/CWndD2D.cpp',
                     'core/app/ThreadQuit.cpp'):
        source = 'phase4/src/' + relative
        obj = out / (source.removesuffix('.cpp').replace('/', '_') + '.obj')
        if os.name == 'nt':
            symbols = subprocess.check_output(['dumpbin', '/symbols', str(obj)], text=True, errors='replace')
            defined = {line.rsplit('|', 1)[-1].strip() for line in symbols.splitlines()
                       if 'External' in line and 'UNDEF' not in line and '|' in line}
        else:
            symbols = subprocess.check_output(['llvm-nm', '--defined-only', str(obj)], text=True)
            defined = {line.split()[-1] for line in symbols.splitlines() if line.split()}
        for symbol in re.findall(r'^// Symbol: (.+)$', (ROOT / source).read_text(), re.M):
            assert gen_weak_stubs.generate_stub_name(symbol) in defined, symbol
            count += 1
    assert count == 24, count
    print('All 24 window/quit export aliases match the DLL generator', flush=True)
    link.extend(str(obj) for obj in objects)
    link.extend(['crypt32.lib', 'odbc32.lib', 'comctl32.lib', 'kernel32.lib', 'user32.lib',
                 'gdi32.lib', 'ole32.lib', 'oleacc.lib', 'oleaut32.lib', 'uuid.lib',
                 'd2d1.lib', 'windowscodecs.lib'])
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    if os.name == 'nt':
        command = [str(out / 'test_window_handles.exe')]
    else:
        if args.sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll'):
                for runtime in (args.sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1=n')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') else []) + ['wine', str(out / 'test_window_handles.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == '__main__':
    main()
