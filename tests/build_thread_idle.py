#!/usr/bin/env python3
"""Build idle processing with production window dispatch, maps and module/TLS storage."""
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
    return [
        'phase4/src/core/runtime/ModuleStateAccessors.cpp',
        'phase4/src/detail/RegcoreSupport.cpp',
        'phase4/src/core/runtime/AFX_MODULE_THREAD_STATE.cpp',
        'phase4/src/core/collections/CHandleMap.cpp',
        'phase4/src/detail/CHandleMapSupport.cpp',
        'phase4/src/detail/MemcoreSupport.cpp',
        'phase4/src/core/collections/CMapPtrToPtrStorage.cpp',
        'phase4/src/detail/CPtrListSupport.cpp',
        'phase4/src/core/controls/CToolTipCtrlLifetime.cpp',
        'phase4/src/detail/CFixedAllocSupport.cpp',
        'phase4/src/core/collections/CPlex.cpp',
        'phase4/src/core/runtime/CThreadLocalObject.cpp',
        'phase4/src/core/runtime/CThreadSlotData.cpp',
        'phase4/src/detail/CThreadLocalObjectSupport.cpp',
        'phase4/src/detail/CThreadSlotDataSupport.cpp',
        'phase4/src/core/runtime/_AFX_THREAD_STATE.cpp',
        'phase4/src/core/runtime/TemporaryHandleMaps.cpp',
        'phase4/src/core/app/CWinThreadIdle.cpp',
        'phase4/src/core/app/CWinThreadMessageLoop.cpp',
        'phase4/src/core/app/CurrentApplication.cpp',
        'phase4/src/core/app/ThreadWindowExceptions.cpp',
        'phase4/src/core/window/WindowMessageContext.cpp',
        'phase4/src/core/window/CWndDescendants.cpp',
        'phase4/src/core/window/CWndCoordinates.cpp',
        'tests/test_thread_idle.cpp']

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
    link = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_thread_idle.exe')]
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
    for relative in ('core/app/CWinThreadIdle.cpp', 'core/app/ThreadWindowExceptions.cpp',
                     'core/app/CWinThreadMessageLoop.cpp'):
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
    assert count == 12, count
    print('All twelve idle/loop/exception export aliases match the DLL generator', flush=True)
    link.extend(str(obj) for obj in objects)
    link.extend(['kernel32.lib', 'user32.lib'])
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    if os.name == 'nt':
        command = [str(out / 'test_thread_idle.exe')]
    else:
        if args.sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll'):
                for runtime in (args.sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1=n')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') else []) + ['wine', str(out / 'test_thread_idle.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == '__main__':
    main()
