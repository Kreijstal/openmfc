#!/usr/bin/env python3
"""Link production string owners into a DLL and test independent public/SDK clients."""
import argparse
import concurrent.futures
import json
import os
from pathlib import Path
import shutil
import subprocess
import struct
import sys

ROOT = Path(__file__).resolve().parent.parent


def sources():
    return [
        *['phase4/src/core/exceptions/'+name+'.cpp' for name in (
            'CException','CExceptionVirtuals','CSimpleException','CMemoryException',
            'CNotSupportedException','CInvalidArgException','CResourceException',
            'CUserException','COleException')],
        'phase4/src/detail/CsimpleexceptionRttiSupport.cpp',
        'phase4/src/core/runtime/AfxLoadString.cpp',
        'phase4/src/core/runtime/CAfxStringMgr.cpp',
        *['phase4/src/core/collections/'+name+'.cpp' for name in (
            'CStringA','CStringAStorage','CStringW','CStringVariant','CStringBuffer','CStringResource','CSimpleStringT')],
        'phase4/src/detail/MemcoreSupport.cpp',
        'tests/string_dll_services.cpp',
    ]


def pe_imports(path):
    data=path.read_bytes();pe=struct.unpack_from('<I',data,0x3c)[0]
    sections=struct.unpack_from('<H',data,pe+6)[0];optional=pe+24
    table=optional+struct.unpack_from('<H',data,pe+20)[0]
    def offset(rva):
        for index in range(sections):
            size,address,raw_size,raw=struct.unpack_from('<IIII',data,table+index*40+8)
            if address<=rva<address+max(size,raw_size):return raw+rva-address
        raise ValueError(f'unmapped import RVA {rva:#x}')
    def string(rva):
        at=offset(rva);return data[at:data.index(b'\0',at)].decode('ascii')
    directory=offset(struct.unpack_from('<I',data,optional+120)[0]);result={}
    while True:
        thunk,_,_,name,address=struct.unpack_from('<IIIII',data,directory)
        if not name:break
        names=set();at=offset(thunk or address)
        while True:
            value=struct.unpack_from('<Q',data,at)[0]
            if not value:break
            names.add(value&0xffff if value>>63 else string(value+2))
            at+=8
        result[string(name).lower()]=names;directory+=20
    return result


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',default='cl' if os.name=='nt' else 'clang-cl')
    parser.add_argument('--sdk',type=Path)
    parser.add_argument('--linker',default='link' if os.name=='nt' else 'lld-link')
    parser.add_argument('--mfc-include',type=Path)
    parser.add_argument('--build',type=Path,required=True)
    parser.add_argument('--optimize',action='store_true')
    parser.add_argument('--jobs',type=int,default=4)
    args=parser.parse_args();out=args.build.resolve();out.mkdir(parents=True,exist_ok=True)
    flags=['/nologo','/EHs','/MD','/std:c++17','/Gy','/Gw','/W0','/DUNICODE','/D_UNICODE',
           '/DNOMINMAX','/D_WIN32_WINNT=0x0601']
    if args.optimize:flags.append('/O2')
    include=['/I'+str(ROOT/'include'),'/I'+str(ROOT/'phase4/src')]
    system=[];libraries=[];mfc=args.mfc_include
    if args.sdk:
        latest=lambda p:sorted(p.iterdir(),key=lambda q:[int(n) for n in q.name.split('.')])[-1]
        vc=latest(args.sdk/'VC/Tools/MSVC');sdk=latest(args.sdk/'Windows Kits/10/Include')
        for path in [vc/'include',*[sdk/name for name in ('ucrt','um','shared')]]:
            system.extend(['-imsvc',str(path)] if 'clang-cl' in args.compiler else ['/I'+str(path)])
        libraries=['/LIBPATH:'+str(path) for path in [vc/'lib/x64',*[args.sdk/'Windows Kits/10/Lib'/sdk.name/n/'x64' for n in ('ucrt','um')]]]
        if mfc is None:mfc=vc/'atlmfc/include'
    if mfc is None:
        candidates=[Path(p) for p in os.environ.get('INCLUDE','').split(os.pathsep) if p]
        if os.environ.get('VCToolsInstallDir'):candidates.append(Path(os.environ['VCToolsInstallDir'])/'atlmfc/include')
        mfc=next((p for p in candidates if (p/'afx.h').is_file()),None)
    env={**os.environ,'WINEDEBUG':os.environ.get('WINEDEBUG','-all')}
    def run(command):subprocess.run(command,check=True,cwd=out,env=env)
    def compile_one(source):
        obj=out/(source.replace('/','_').removesuffix('.cpp')+'.obj')
        run([args.compiler,*flags,*include,*system,'/FIwinsock2.h','/FIobjbase.h','/FIcommdlg.h','/FIshellapi.h','/c',str(ROOT/source),'/Fo'+str(obj)])
        return obj
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:objects=list(pool.map(compile_one,sources()))
    sys.path.insert(0,str(ROOT/'phase4/scripts'));import build_msvc
    defined=set().union(*(build_msvc.defined_symbols(p) for p in objects))
    wanted=json.loads((ROOT/'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u']
    exception_classes=('CObject','CException','CSimpleException','CMemoryException','CNotSupportedException','CInvalidArgException','CResourceException','CUserException','COleException')
    def needed(name):
        return ('?$CStringT@' in name or '?$CSimpleStringT@' in name or '@CAfxStringMgr@@' in name
                or name.startswith('?AfxGetStringManager@@') or name.startswith('?AfxThrow')
                or any('@'+cls+'@@' in name for cls in exception_classes))
    entries=[item for item in wanted if needed(item['symbol']) and
             (item['symbol'] in defined or build_msvc.generate_stub_name(item['symbol']) in defined)]
    definition,missing=build_msvc.export_definition(entries,defined);assert not missing
    (out/'strings.def').write_text(definition.replace('LIBRARY openmfc','LIBRARY openmfc_string_types',1))
    resource=out/'strings.res'
    run([('rc' if os.name=='nt' else 'llvm-rc'),'/nologo','/fo',str(resource),str(ROOT/'tests/string_import_fixture.rc')])
    link=args.linker
    def link_response(name,options):
        def argument(value):
            text=str(value)
            if os.name!='nt' and Path(link).name.lower() in ('link','link.exe'):
                # Response-file contents bypass msvc-wine's argument path conversion.
                if text.startswith('/') and not text.startswith(('/OUT:','/IMPLIB:','/DEF:','/LIBPATH:')):
                    if Path(text).is_absolute() and Path(text).exists():text='Z:'+text
                elif ':/' in text:text=text.replace(':/',':Z:/',1)
            return '"'+text+'"'
        rsp=out/(name+'.rsp');rsp.write_text('\n'.join(argument(p) for p in options));run([link,'@'+str(rsp)])
    link_response('dll',['/nologo','/DLL','/OPT:REF','/DEF:'+str(out/'strings.def'),
                         '/OUT:'+str(out/'openmfc_string_types.dll'),'/IMPLIB:'+str(out/'openmfc_string_types.lib'),
                         *libraries,*objects,resource,'kernel32.lib','user32.lib','oleaut32.lib'])
    exports=build_msvc.pe_exports(out/'openmfc_string_types.dll')
    assert exports=={item['symbol']:item['ordinal'] for item in entries}
    print(f'String DLL: {len(entries)} real mapped exports with native ordinals',flush=True)
    if os.name!='nt' and args.sdk:
        for name in ('vcruntime140.dll','vcruntime140_1.dll','msvcp140.dll'):
            for runtime in (args.sdk/'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/'+name):shutil.copy2(runtime,out/name)
        env['WINEDLLOVERRIDES']='vcruntime140,vcruntime140_1,msvcp140=n'
    for native in (False,True):
        if native and (mfc is None or not (mfc/'afx.h').is_file()):
            print('Installed SDK MFC headers unavailable; SDK client skipped',flush=True);continue
        name='sdk_client' if native else 'public_client';obj=out/(name+'.obj')
        client_include=['/I'+str(mfc)] if native else include
        run([args.compiler,*flags,*client_include,*system,'/D_AFXDLL',
             *(['/DOPENMFC_USE_SDK_MFC'] if native else []),'/c',str(ROOT/'tests/test_string_imports.cpp'),'/Fo'+str(obj)])
        link_response(name,['/nologo','/SUBSYSTEM:CONSOLE','/OPT:REF','/NODEFAULTLIB:mfc140u.lib','/NODEFAULTLIB:mfcs140u.lib','/NODEFAULTLIB:atls.lib',
                            '/OUT:'+str(out/(name+'.exe')),*libraries,obj,out/'openmfc_string_types.lib',
                            *([mfc.parent/'lib/x64/atls.lib'] if native else []),'kernel32.lib','oleaut32.lib'])
        command=[str(out/(name+'.exe'))] if os.name=='nt' else (['xvfb-run','-a'] if not env.get('DISPLAY') else [])+['wine',str(out/(name+'.exe'))]
        imports=pe_imports(out/(name+'.exe'))
        assert not any(dll.startswith('mfc') for dll in imports),imports
        raw=imports['openmfc_string_types.dll']
        by_ordinal={ordinal:symbol for symbol,ordinal in exports.items()}
        actual={by_ordinal[item] if isinstance(item,int) else item for item in raw}
        print(f'{name}: {sum(isinstance(item,int) for item in raw)} ordinal imports, {sum(isinstance(item,str) for item in raw)} named imports',flush=True)
        for prefix in ('??0?$CStringT@D','??0?$CStringT@_W','?AfxGetStringManager@@'):
            assert any(symbol.startswith(prefix) for symbol in actual),(name,prefix,actual)
        for method in ('Delete','Insert','Remove','Replace','Trim','TrimLeft','TrimRight','AllocSysString','SetSysString','GetEnvironmentVariable','GetEnvironmentVariableW','CharToOemA','OemToCharA','Construct'):
            for width in ('D','_W'):
                prefix='?'+method+'@?$CStringT@'+width+'V'
                assert any(symbol.startswith(prefix) for symbol in actual),(name,prefix,actual)
        assert not any(symbol.startswith(('??0CString@@','??0CStringA@@')) for symbol in actual),actual
        run(command)
        print(name+': linked and ran independently',flush=True)


if __name__=='__main__':main()
