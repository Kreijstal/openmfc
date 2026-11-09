#!/usr/bin/env python3
"""Validate native archive storage and independently linked public/SDK clients."""
import argparse, concurrent.futures, json, os, re, shutil, subprocess, sys
from pathlib import Path
import build_string_imports
ROOT=Path(__file__).resolve().parent.parent

def sources():
    return [s for s in build_string_imports.sources() if not s.startswith('tests/')]+[
        'phase4/src/core/runtime/CArchiveStorage.cpp',
        'phase4/src/core/runtime/CArchiveMapping.cpp',
        'phase4/src/core/runtime/NativeMemoryOperators.cpp',
        'phase4/src/core/runtime/CObjectMetadata.cpp',
        'phase4/src/core/runtime/AfxFormatStrings.cpp',
        'phase4/src/core/file/CFile.cpp',
        'phase4/src/core/file/CMemFile.cpp',
        'phase4/src/detail/FileStatusSupport.cpp',
        'phase4/src/core/exceptions/CFileOsError.cpp',
        *['phase4/src/core/exceptions/'+n+'.cpp' for n in ['CFileException','CFileExceptionLifetime','CArchiveException','CArchiveExceptionLifetime']],
        'phase4/src/core/collections/CMapPtrToPtrStorage.cpp',
        'phase4/src/core/collections/CPlex.cpp',
        'phase4/src/core/collections/CPtrArray.cpp',
        'phase4/src/core/collections/CPtrArrayStorage.cpp',
        'tests/archive_services.cpp',
    ]

def main():
    ap=argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--sdk',type=Path);ap.add_argument('--compiler',default='cl' if os.name=='nt' else 'clang-cl')
    ap.add_argument('--linker',default='link' if os.name=='nt' else 'lld-link')
    ap.add_argument('--build',type=Path,required=True);ap.add_argument('--jobs',type=int,default=4);ap.add_argument('--optimize',action='store_true')
    args=ap.parse_args();out=args.build.resolve();out.mkdir(parents=True,exist_ok=True)
    flags=['/nologo','/EHs','/MD','/std:c++17','/Gy','/Gw','/W0','/DUNICODE','/D_UNICODE','/DNOMINMAX','/D_WIN32_WINNT=0x0601']
    if args.optimize:flags.append('/O2')
    include=['/I'+str(ROOT/'include'),'/I'+str(ROOT/'phase4/src')];system=[];libraries=[];mfc=None
    if args.sdk:
        latest=lambda p:sorted(p.iterdir(),key=lambda q:[int(n) for n in q.name.split('.')])[-1]
        vc=latest(args.sdk/'VC/Tools/MSVC');sdk=latest(args.sdk/'Windows Kits/10/Include')
        system=['/I'+str(p) for p in [vc/'include',*[sdk/n for n in ('ucrt','um','shared')]]]
        libraries=['/LIBPATH:'+str(p) for p in [vc/'lib/x64',*[args.sdk/'Windows Kits/10/Lib'/sdk.name/n/'x64' for n in ('ucrt','um')]]]
        mfc=vc/'atlmfc/include'
    else:
        if os.environ.get('VCToolsInstallDir'):mfc=Path(os.environ['VCToolsInstallDir'])/'atlmfc/include'
        for p in os.environ.get('INCLUDE','').split(os.pathsep):
            if (Path(p)/'afx.h').is_file():mfc=Path(p)
    env={**os.environ,'WINEDEBUG':os.environ.get('WINEDEBUG','-all')}
    def run(command):subprocess.run(command,cwd=out,env=env,check=True)
    def compile_one(source):
        obj=out/(source.replace('/','_').removesuffix('.cpp')+'.obj')
        run([args.compiler,*flags,*include,*system,'/FIwinsock2.h','/FIobjbase.h','/FIcommdlg.h','/FIshellapi.h','/c',str(ROOT/source),'/Fo'+str(obj)])
        return obj
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:objects=list(pool.map(compile_one,sources()))
    sys.path.insert(0,str(ROOT/'phase4/scripts'));import build_msvc
    defined=set();providers=set();names=set()
    for source,obj in zip(sources(),objects):
        symbols=build_msvc.defined_symbols(obj);defined.update(symbols)
        if not source.startswith('tests/'):providers.update(symbols)
        names.update(re.findall(r'^// Symbol: (.+)$',(ROOT/source).read_text(),re.M))
    all_entries=json.loads((ROOT/'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u']
    providers.update(name for name in defined if '?$CSimpleStringT@' in name)
    entries=[e for e in all_entries if e['symbol'] in providers or build_msvc.generate_stub_name(e['symbol']) in providers]
    assert names<={e['symbol'] for e in entries},names-{e['symbol'] for e in entries}
    definition,missing=build_msvc.export_definition(entries,defined);assert not missing,missing
    (out/'archive.def').write_text(definition.replace('LIBRARY openmfc','LIBRARY openmfc_archive',1))
    def link_response(name,options):
        def quote(value):
            text=str(value)
            if os.name!='nt' and Path(args.linker).name.lower() in ('link','link.exe'):
                if text.startswith('/') and Path(text).exists():text='Z:'+text
                elif ':/' in text:text=text.replace(':/',':Z:/',1)
            return '"'+text+'"'
        rsp=out/(name+'.rsp');rsp.write_text('\n'.join(quote(p) for p in options));run([args.linker,'@'+str(rsp)])
    libs=['kernel32.lib','user32.lib','oleaut32.lib','comdlg32.lib','shlwapi.lib']
    link_response('dll',['/nologo','/DLL','/OPT:REF','/DEF:'+str(out/'archive.def'),'/OUT:'+str(out/'openmfc_archive.dll'),'/IMPLIB:'+str(out/'openmfc_archive.lib'),*libraries,*objects,*libs])
    assert build_msvc.pe_exports(out/'openmfc_archive.dll')=={e['symbol']:e['ordinal'] for e in entries}
    print(f'Archive DLL: {len(entries)} actual native exports with exact ordinals',flush=True)
    if os.name!='nt' and args.sdk:
        for name in ('vcruntime140.dll','vcruntime140_1.dll','msvcp140.dll'):
            for source in (args.sdk/'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/'+name):shutil.copy2(source,out/name)
        env.setdefault('WINEDLLOVERRIDES','vcruntime140,vcruntime140_1,msvcp140=n')
    def execute(name):run(([str(out/name)] if os.name=='nt' else (['xvfb-run','-a'] if not env.get('DISPLAY') else [])+['wine',str(out/name)]))
    oracle=compile_one('tests/test_archive_storage.cpp')
    link_response('oracle',['/nologo','/SUBSYSTEM:CONSOLE','/OPT:REF','/OUT:'+str(out/'test_archive_storage.exe'),*libraries,*objects,oracle,*libs])
    execute('test_archive_storage.exe')
    # Independent clients use no implementation headers or test replacement types.
    for native in (False,True):
        if native and (mfc is None or not (mfc/'afx.h').is_file()):raise RuntimeError('Installed MFC SDK headers required for the archive ABI client')
        name='sdk_client' if native else 'public_client';obj=out/(name+'.obj')
        run([args.compiler,*flags,*system,*(['/I'+str(mfc)] if native else include),'/D_AFXDLL',*(['/DOPENMFC_USE_SDK_MFC'] if native else []),'/c',str(ROOT/'tests/test_archive_imports.cpp'),'/Fo'+str(obj)])
        link_response(name,['/nologo','/SUBSYSTEM:CONSOLE','/OPT:REF','/OUT:'+str(out/(name+'.exe')),*libraries,obj,str(out/'openmfc_archive.lib'),*libs])
        imports=build_string_imports.pe_imports(out/(name+'.exe'));assert not any(d.startswith('mfc') for d in imports)
        print(name+': '+str(len(imports['openmfc_archive.dll']))+' production DLL imports',flush=True)
        execute(name+'.exe')

if __name__=='__main__':main()
