#!/usr/bin/env python3
"""Validate the native data-source cache, embedded COM part and file callbacks."""
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
    """Check the actual production COFF tables against the native 26/12-slot ABI."""
    data = path.read_bytes()
    machine, sections, _, offset, count, optional, _ = struct.unpack_from('<HHIIIHH', data)
    assert machine == 0x8664
    strings = offset + count * 18
    names, tables = {}, {}
    roots = {'??_7COleDataSource@@6B@':26,'??_7XDataObject@COleDataSource@@6B@':12}
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
        expected = ({0:'?GetRuntimeClass@CCmdTarget@@',16:'?GetInterfaceMap@COleDataSource@@',
                     22:'?OnRenderGlobalData@COleDataSource@@',23:'?OnRenderFileData@COleDataSource@@',
                     24:'?OnRenderData@COleDataSource@@',25:'?OnSetData@COleDataSource@@'} if roots[name]==26 else
                    {slot:'?'+method+'@XDataObject@COleDataSource@@' for slot,method in enumerate(
                     ('QueryInterface','AddRef','Release','GetData','GetDataHere','QueryGetData',
                      'GetCanonicalFormatEtc','SetData','EnumFormatEtc','DAdvise','DUnadvise','EnumDAdvise'))})
        if roots[name]==26:
            expected.update({slot:'?'+method+'@CCmdTarget@@' for slot,method in {
                5:'OnCmdMsg',6:'OnFinalRelease',7:'IsInvokeAllowed',8:'GetDispatchIID',
                9:'GetTypeInfoCount',10:'GetTypeLibCache',11:'GetTypeLib',12:'GetMessageMap',
                13:'GetCommandMap',14:'GetDispatchMap',15:'GetConnectionMap',17:'GetEventSinkMap',
                18:'OnCreateAggregates',19:'GetInterfaceHook',20:'GetExtraConnectionPoints',21:'GetConnectionHook'}.items()})
            expected.update({2:'?Serialize@CObject@@',3:'?AssertValid@CObject@@',4:'?Dump@CObject@@',1:('??_ECOleDataSource@@','??_GCOleDataSource@@')})
        for slot, prefix in expected.items():
            assert targets[slot].startswith(prefix), (slot, targets[slot], prefix)
    print('Production data-source vtables have all 26/12 entries in native order', flush=True)


def sources():
    from build_ole_format_enum import sources as enum_sources
    return [p for p in enum_sources() if p != 'tests/test_ole_format_enum.cpp'] + [
        'phase4/src/core/runtime/GlobalLocks.cpp',
        'phase4/src/core/runtime/CProcessLocalObject.cpp',
        'phase4/src/detail/CProcessLocalObjectSupport.cpp',
        'phase4/src/detail/OleProcessStateSupport.cpp',
        'phase4/src/core/ole/COleDropSource.cpp',
        'phase4/src/core/ole/COleDataSource.cpp',
        'phase4/src/core/ole/OleDataSourceMedium.cpp',
        'phase4/src/core/exceptions/OleExceptionProcess.cpp',
        'phase4/src/core/exceptions/CNotSupportedException.cpp',
        'phase4/src/core/file/CFile.cpp',
        'phase4/src/core/file/CMemFile.cpp',
        'phase4/src/core/file/CSharedFile.cpp',
        'phase4/src/detail/CSharedFileSupport.cpp',
        'phase4/src/core/ole/COleStreamFile.cpp',
        'phase4/src/detail/FileStatusSupport.cpp',
        'phase4/src/core/exceptions/CFileOsError.cpp',
        'tests/test_ole_data_source.cpp']

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--compiler', default='cl' if os.name == 'nt' else 'clang-cl')
    parser.add_argument('--sdk', type=Path)
    parser.add_argument('--build', type=Path, required=True)
    parser.add_argument('--optimize', action='store_true')
    parser.add_argument('--jobs', type=int, default=4)
    parser.add_argument('--verify-existing', action='store_true',
                        help='Validate, relink and run the existing objects in --build')
    args = parser.parse_args()
    out = args.build.resolve()
    out.mkdir(parents=True, exist_ok=True)
    flags = ['/nologo', '/EHs', '/MD', '/std:c++17', '/Gy', '/Gw', '/W0',
             '/DUNICODE', '/D_UNICODE', '/DNOMINMAX', '/D_WIN32_WINNT=0x0601',
             '/FIwinsock2.h', '/FIobjbase.h', '/FIcommdlg.h', '/FIshellapi.h',
             '/I' + str(ROOT / 'include'), '/I' + str(ROOT / 'phase4/src')]
    link = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_ole_data_source.exe')]
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

    if args.verify_existing:
        objects = [out / (source.removesuffix('.cpp').replace('/', '_') + '.obj') for source in sources()]
        assert all(obj.is_file() for obj in objects), 'Compile the fixture before --verify-existing'
    else:
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
            objects = list(pool.map(compile_one, sources()))
    sys.path.insert(0, str(ROOT / 'phase4/scripts'))
    import build_msvc
    verify_vtables(out/'phase4_src_core_ole_COleDataSource.obj')
    checked=['phase4/src/core/ole/COleDataSource.cpp','phase4/src/core/exceptions/OleExceptionProcess.cpp']
    defined,names=set(),set()
    for source in checked:
        obj=out / (source.removesuffix('.cpp').replace('/','_')+'.obj')
        defined.update(build_msvc.defined_symbols(obj))
        names.update(re.findall(r'^// Symbol: (.+)$',(ROOT/source).read_text(encoding='utf-8',errors='replace'),re.M))
    import json
    entries=[entry for entry in json.loads((ROOT/'mfc_complete_ordinal_mapping.json').read_text(encoding='utf-8'))['exports']['mfc140u'] if entry['symbol'] in names]
    assert len(entries)==len(names)==30,(len(entries),len(names))
    _,missing=build_msvc.export_definition(entries,defined)
    assert not missing,missing
    owners={name:[] for name in names}
    for path in (ROOT/'phase4/src').rglob('*.cpp'):
        for name in set(re.findall(r'^// Symbol: (.+)$',path.read_text(encoding='utf-8',errors='replace'),re.M))&names:
            owners[name].append(str(path.relative_to(ROOT)))
    assert all(len(paths)==1 for paths in owners.values()),owners
    print('All 30 data-source and exception-process exports have actual providers',flush=True)
    link.extend(str(obj) for obj in objects)
    link.extend(['crypt32.lib', 'odbc32.lib', 'comctl32.lib', 'kernel32.lib', 'user32.lib',
                 'gdi32.lib', 'ole32.lib', 'oleacc.lib', 'oleaut32.lib', 'uuid.lib',
                 'd2d1.lib', 'windowscodecs.lib', 'comdlg32.lib', 'shlwapi.lib'])
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    if os.name == 'nt':
        command = [str(out / 'test_ole_data_source.exe')]
    else:
        if args.sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll', 'msvcp140.dll'):
                for runtime in (args.sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1,msvcp140=n')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') else []) + ['wine', str(out / 'test_ole_data_source.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)


if __name__ == '__main__':
    main()
