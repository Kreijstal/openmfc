#!/usr/bin/env python3
"""Build and validate native framework exception hierarchy, resources and ownership."""
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


def sources():
    return [
        'phase4/src/core/exceptions/CException.cpp',
        'phase4/src/core/exceptions/CExceptionVirtuals.cpp',
        'phase4/src/core/exceptions/CSimpleException.cpp',
        'phase4/src/core/exceptions/CMemoryException.cpp',
        'phase4/src/core/exceptions/CNotSupportedException.cpp',
        'phase4/src/core/exceptions/CInvalidArgException.cpp',
        'phase4/src/core/exceptions/CResourceException.cpp',
        'phase4/src/core/exceptions/CUserException.cpp',
        'phase4/src/detail/CsimpleexceptionRttiSupport.cpp',
        'phase4/src/core/runtime/AfxLoadString.cpp',
        'phase4/src/core/exceptions/COleException.cpp',
        'phase4/src/core/exceptions/COleDispatchException.cpp',
        'phase4/src/core/exceptions/OleExceptionProcess.cpp',
        'phase4/src/core/ole/OleExceptionTranslation.cpp',
        'phase4/src/core/exceptions/CFileException.cpp',
        'phase4/src/core/exceptions/CFileExceptionLifetime.cpp',
        'phase4/src/core/exceptions/CArchiveException.cpp',
        'phase4/src/core/exceptions/CArchiveExceptionLifetime.cpp',
        'phase4/src/core/exceptions/CFileOsError.cpp',
        'phase4/src/core/collections/CStringBuffer.cpp',
        'phase4/src/core/collections/CStringResource.cpp',
        'phase4/src/core/runtime/AfxFormatStrings.cpp',
        'phase4/src/core/exceptions/CInternetException.cpp',
        'phase4/src/core/window/WindowActivation.cpp',
        'phase4/src/core/exceptions/AFX_EXCEPTION_LINK.cpp',
        'phase4/src/core/runtime/CAfxStringMgr.cpp',
        'phase4/src/core/collections/CStringVariant.cpp',
        'phase4/src/core/collections/CStringW.cpp',
        'phase4/src/core/collections/CStringA.cpp',
        'phase4/src/core/collections/CStringAStorage.cpp',
        'phase4/src/core/collections/CSimpleStringT.cpp',
        'tests/test_ansi_string_ownership.cpp',
        'tests/test_exception_links.cpp',
        'tests/test_internet_exceptions.cpp',
        'tests/test_file_archive_exceptions.cpp',
        'tests/test_ole_exception_rtti_logic.cpp',
        'tests/test_simple_exceptions_logic.cpp',
    ]


def verify_exception_vtable(path,cls):
    data=path.read_bytes()
    _,sections,_,offset,count,optional,_=struct.unpack_from('<HHIIIHH',data)
    strings=offset+count*18;names={};table=None;index=0
    while index<count:
        at=offset+index*18;raw=data[at:at+8]
        if raw[:4]==b'\0'*4:
            start=strings+struct.unpack_from('<I',raw,4)[0];name=data[start:data.index(b'\0',start)].decode()
        else:name=raw.rstrip(b'\0').decode()
        value,section,_,_,aux=struct.unpack_from('<IhHBB',data,at+8);names[index]=name
        if name=='??_7'+cls+'@@6B@':table=(value,section)
        index+=1+aux
    assert table is not None,cls
    value,section=table;header=20+optional+(section-1)*40
    size,_,relocations=struct.unpack_from('<III',data,header+16);total=struct.unpack_from('<H',data,header+32)[0]
    assert size-value==64,(cls,size,value)
    targets={}
    for at in range(relocations,relocations+total*10,10):
        address,symbol,_=struct.unpack_from('<IIH',data,at)
        if address>=value:targets[(address-value)//8]=names[symbol]
    expected={0:'?GetRuntimeClass@'+cls+'@@',2:'?Serialize@CObject@@',3:'?AssertValid@CObject@@',4:'?Dump@CObject@@',5:'?GetErrorMessage@CException@@UEAA',6:'?GetErrorMessage@'+('CSimpleException' if cls in ('CMemoryException','CNotSupportedException','CInvalidArgException','CResourceException','CUserException') else cls)+'@@UEBA',7:'?ReportError@CException@@'}
    for slot,prefix in expected.items():assert targets[slot].startswith(prefix),(cls,slot,targets[slot])
    print(cls+': all eight actual exception slots in native order',flush=True)


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
    link = ['/nologo', '/SUBSYSTEM:CONSOLE', '/OPT:REF', '/OUT:' + str(out / 'test_framework_exceptions.exe')]
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
    checked=[source for source in sources() if not source.startswith('tests/')]
    defined,names=set(),set()
    for source in checked:
        obj=out / (source.removesuffix('.cpp').replace('/','_')+'.obj')
        defined.update(build_msvc.defined_symbols(obj))
        names.update(re.findall(r'^// Symbol: (.+)$',(ROOT/source).read_text(encoding='utf-8', errors='replace'),re.M))
    import json
    entries=[entry for entry in json.loads((ROOT/'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u'] if entry['symbol'] in names]
    assert len(entries)==len(names)==274, (len(entries), len(names))
    owners = {name: [] for name in names}
    for path in (ROOT / 'phase4/src').rglob('*.cpp'):
        for name in re.findall(r'^// Symbol: (.+)$', path.read_text(encoding='utf-8', errors='replace'), re.M):
            if name in owners:
                owners[name].append(path.relative_to(ROOT).as_posix())
    assert all(len(paths) == 1 for paths in owners.values()), owners
    _,missing=build_msvc.export_definition(entries,defined)
    assert not missing,missing
    print('All 274 framework/file/string exports have actual providers',flush=True)
    for relative in checked:
        name=Path(relative).stem
        if name.endswith("Lifetime"):name=name[:-8]
        if ('Lifetime' in relative or name not in ('CFileException','CArchiveException')) and name in ('CMemoryException','CNotSupportedException','CInvalidArgException','CResourceException','CUserException','COleException','COleDispatchException','CFileException','CArchiveException','CInternetException'):
            verify_exception_vtable(out/(relative.removesuffix('.cpp').replace('/','_')+'.obj'),name)
    link.extend(str(obj) for obj in objects)
    link.extend(['kernel32.lib', 'user32.lib', 'comdlg32.lib', 'shlwapi.lib', 'oleaut32.lib', 'wininet.lib'])
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    if os.name == 'nt':
        command = [str(out / 'test_framework_exceptions.exe')]
    else:
        if args.sdk:
            for name in ('vcruntime140.dll', 'vcruntime140_1.dll', 'msvcp140.dll'):
                for runtime in (args.sdk / 'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/' + name):
                    shutil.copy2(runtime, out / name)
        env.setdefault('WINEDLLOVERRIDES', 'vcruntime140,vcruntime140_1,msvcp140=n')
        command = (['xvfb-run', '-a'] if not env.get('DISPLAY') else []) + ['wine', str(out / 'test_framework_exceptions.exe')]
    subprocess.run(command, check=True, cwd=out, env=env, timeout=120)
    # Actual finder/loader integration uses the real module and library owners
    # in build_dynlink_library.py; this fixture isolates exception services.



if __name__ == '__main__':
    main()
