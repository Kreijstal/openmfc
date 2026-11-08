#!/usr/bin/env python3
"""Build drop target behavior with the production window, module/TLS and map owners."""
import argparse
import concurrent.futures
import os
from pathlib import Path
import re
import shutil
import subprocess
import struct
import sys

ROOT = Path(__file__).resolve().parent.parent


def verify_vtables(path):
    """Check the actual production COFF tables against the native 29/7-slot ABI."""
    data = path.read_bytes()
    machine, sections, _, offset, count, optional, _ = struct.unpack_from('<HHIIIHH', data)
    assert machine == 0x8664
    strings = offset + count * 18
    names, tables = {}, {}
    roots = {'??_7COleDropTarget@@6B@': 29, '??_7XDropTarget@COleDropTarget@@6B@': 7}
    index = 0
    while index < count:
        at = offset + index * 18
        raw = data[at:at + 8]
        if raw[:4] == b'\0' * 4:
            start = strings + struct.unpack_from('<I', raw, 4)[0]
            name = data[start:data.index(b'\0', start)].decode()
        else:
            name = raw.rstrip(b'\0').decode()
        value, section, _, _, auxiliary = struct.unpack_from('<IhHBB', data, at + 8)
        names[index] = name
        if name in roots:
            tables[name] = value, section
        index += 1 + auxiliary
    assert set(tables) == set(roots), tables
    for name, (value, section) in tables.items():
        assert 0 < section <= sections
        header = 20 + optional + (section - 1) * 40
        size, _, relocations = struct.unpack_from('<III', data, header + 16)
        reloc_count = struct.unpack_from('<H', data, header + 32)[0]
        assert size - value == roots[name] * 8, (name, size, value)
        targets = {}
        for at in range(relocations, relocations + reloc_count * 10, 10):
            address, symbol, _ = struct.unpack_from('<IIH', data, at)
            if address >= value:
                targets[(address - value) // 8] = names[symbol]
        expected = ({0: '?GetRuntimeClass@CCmdTarget@@', 16: '?GetInterfaceMap@COleDropTarget@@',
                     **{slot: '?' + method + '@COleDropTarget@@' for slot, method in enumerate(
                         ('Revoke','OnDragEnter','OnDragOver','OnDrop','OnDropEx','OnDragLeave','OnDragScroll'),22)}}
                    if roots[name] == 29 else
                    {slot: '?' + method + '@XDropTarget@COleDropTarget@@' for slot, method in
                     enumerate(('QueryInterface','AddRef','Release','DragEnter','DragOver','DragLeave','Drop'))})
        for slot, prefix in expected.items():
            assert targets[slot].startswith(prefix), (slot, targets[slot], prefix)
    print('Production drop target and COM vtables have all 29/7 entries in native order', flush=True)


def sources():
    from build_window_handles import sources as window_sources
    return [s for s in window_sources() if s != 'tests/test_window_handles.cpp'] + [
        'phase4/src/core/runtime/AFX_MODULE_THREAD_STATE.cpp',
        'phase4/src/core/controls/CToolTipCtrlLifetime.cpp',
        'phase4/src/core/ole/COleDropTarget.cpp',
        'phase4/src/core/ole/COleDataObjectLifetime.cpp',
        'phase4/src/core/view/CViewRuntimeClass.cpp',
        'phase4/src/core/runtime/GlobalLocks.cpp',
        'tests/test_ole_drop_target.cpp']


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
    link = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_ole_drop_target.exe')]
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
    source = 'phase4/src/core/ole/COleDropTarget.cpp'
    obj = out / (source.removesuffix('.cpp').replace('/', '_') + '.obj')
    defined = build_msvc.defined_symbols(obj)
    verify_vtables(obj)
    names = set(re.findall(r'^// Symbol: (.+)$', (ROOT / source).read_text(), re.M))
    import json
    entries = [entry for entry in json.loads((ROOT / 'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u']
               if entry['symbol'] in names]
    assert len(entries) == len(names) == 19
    definition, missing = build_msvc.export_definition(entries, defined)
    assert not missing, missing
    for name in ('nScrollDelay', 'nScrollInterval', 'nScrollInset'):
        assert '?' + name + '@COleDropTarget@@1' + ('HA' if name == 'nScrollInset' else 'IA') + ' @' in definition
    print('All nineteen drop target exports have actual native providers; all three data exports share the C++ fields', flush=True)
    link.extend(str(obj) for obj in objects)
    link.extend(['crypt32.lib', 'odbc32.lib', 'comctl32.lib', 'kernel32.lib', 'user32.lib',
                 'gdi32.lib', 'ole32.lib', 'oleacc.lib', 'oleaut32.lib', 'uuid.lib',
                 'd2d1.lib', 'windowscodecs.lib'])
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    if os.name == 'nt':
        command = [str(out / 'test_ole_drop_target.exe')]
    else:
        if args.sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll'):
                for runtime in (args.sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1=n')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') else []) + ['wine', str(out / 'test_ole_drop_target.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == '__main__':
    main()
