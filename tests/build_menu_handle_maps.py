#!/usr/bin/env python3
"""Build and run focused CMenu handle-map tests with an optional pinned MFC oracle."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parent.parent
SOURCES = [
    'phase4/src/core/window/CMenu.cpp',
    'phase4/src/detail/CMenuSupport.cpp',
    'phase4/src/core/collections/CHandleMap.cpp',
    'phase4/src/detail/CHandleMapSupport.cpp',
    'phase4/src/detail/CFixedAllocSupport.cpp',
    'phase4/src/core/collections/CMapPtrToPtrStorage.cpp',
    'phase4/src/core/collections/CPlex.cpp',
    'tests/test_menu_handle_maps.cpp',
]


def latest(path: Path) -> Path:
    return sorted(path.iterdir(), key=lambda p: [int(part) for part in p.name.split('.')])[-1]


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument('--compiler', default='cl' if os.name == 'nt' else 'clang-cl')
    default_sdk = os.environ.get('MSVC_DIR')
    if default_sdk is None and Path('/home/kreijstal/msvc').is_dir():
        default_sdk = '/home/kreijstal/msvc'
    parser.add_argument('--sdk', type=Path, default=Path(default_sdk) if default_sdk else None)
    parser.add_argument('--build', type=Path, default=Path('/tmp/openmfc-pi-menu'))
    parser.add_argument('--native-dll', type=Path,
                        default=Path('/home/kreijstal/msvc/VC/Redist/MSVC/14.51.36231/x64/Microsoft.VC145.MFC/mfc140u.dll'))
    parser.add_argument('--no-native', action='store_true', help='run source golden checks only')
    args = parser.parse_args()

    out = args.build.resolve()
    out.mkdir(parents=True, exist_ok=True)
    sdk = args.sdk.resolve() if args.sdk else None
    vc = latest(sdk / 'VC/Tools/MSVC') if sdk else None
    kit = latest(sdk / 'Windows Kits/10/Include') if sdk else None
    clang = 'clang-cl' in Path(args.compiler).name.lower()
    flags = ['/nologo', '/EHsc', '/MD', '/std:c++17', '/Gy', '/Gw', '/W0',
             '/DUNICODE', '/D_UNICODE', '/DNOMINMAX', '/DOPENMFC_EXPORTS',
             '/D_WIN32_WINNT=0x0601', '/FIwinsock2.h', '/FIobjbase.h', '/FIcommdlg.h',
             '/FIshellapi.h', '/I' + str(ROOT / 'include'), '/I' + str(ROOT / 'phase4/src')]
    if sdk:
        if clang:
            for include in (vc / 'include', kit / 'ucrt', kit / 'um', kit / 'shared'):
                flags.extend(['-imsvc', str(include)])
        else:
            for include in (vc / 'include', kit / 'ucrt', kit / 'um', kit / 'shared'):
                flags.append('/I' + str(include))

    env = dict(os.environ)
    env.setdefault('WINEDEBUG', '-all')
    objects = []
    for relative in SOURCES:
        obj = out / (relative.removesuffix('.cpp').replace('/', '_') + '.obj')
        subprocess.run([args.compiler, *flags, '/c', str(ROOT / relative), '/Fo' + str(obj)],
                       check=True, cwd=out, env=env)
        objects.append(obj)

    resource = out / 'menu_handle_maps.res'
    rc = shutil.which('rc.exe') or shutil.which('rc') if os.name == 'nt' else shutil.which('llvm-rc')
    if not rc:
        raise RuntimeError('resource compiler (rc.exe or llvm-rc) is required for LoadMenu coverage')
    subprocess.run([rc, '/fo' + str(resource), str(ROOT / 'tests/menu_handle_maps.rc')],
                   check=True, cwd=out, env=env)

    if os.name == 'nt':
        linker = 'link'
        link_flags = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_menu_handle_maps.exe')]
    else:
        linker = 'lld-link'
        link_flags = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_menu_handle_maps.exe')]
    if sdk:
        libs = [vc / 'lib/x64', sdk / 'Windows Kits/10/Lib' / kit.name / 'ucrt/x64',
                sdk / 'Windows Kits/10/Lib' / kit.name / 'um/x64']
        link_flags.extend('/LIBPATH:' + str(path) for path in libs)
    subprocess.run([linker, *link_flags, *(str(obj) for obj in objects), str(resource),
                    'kernel32.lib', 'user32.lib', 'gdi32.lib'], check=True, cwd=out, env=env)

    if os.name != 'nt':
        if sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll'):
                for runtime in (sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1=n')
        if not args.no_native and args.native_dll.is_file():
            native = args.native_dll.resolve()
            env['OPENMFC_NATIVE_MENU_DLL'] = 'Z:' + str(native).replace('/', '\\')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') and shutil.which('xvfb-run') else []) + [
            'wine', str(out / 'test_menu_handle_maps.exe')]
    else:
        if not args.no_native and args.native_dll.is_file():
            env['OPENMFC_NATIVE_MENU_DLL'] = str(args.native_dll.resolve())
        command = [str(out / 'test_menu_handle_maps.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == '__main__':
    main()
