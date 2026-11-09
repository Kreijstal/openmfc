#!/usr/bin/env python3
"""Compare public generic and pointer arrays with installed SDK/native MFC arrays."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import sys
import build_string_imports

ROOT=Path(__file__).resolve().parent.parent

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--sdk',type=Path)
    parser.add_argument('--compiler',default='cl' if os.name=='nt' else 'clang-cl')
    parser.add_argument('--linker',default='link' if os.name=='nt' else 'lld-link')
    parser.add_argument('--build',type=Path,required=True)
    parser.add_argument('--archive-build',type=Path,help='Reuse an already validated production archive DLL')
    parser.add_argument('--optimize',action='store_true')
    args=parser.parse_args();out=args.build.resolve();out.mkdir(parents=True,exist_ok=True)
    archive=args.archive_build.resolve() if args.archive_build else out/'archive'
    if not args.archive_build:
        subprocess.run([sys.executable,str(ROOT/'tests/build_archive.py'),'--compiler',args.compiler,'--linker',args.linker,
                        '--build',str(archive),*(['--sdk',str(args.sdk)] if args.sdk else []),*(['--optimize'] if args.optimize else [])],check=True)
    system=[];libraries=[]
    if args.sdk:
        latest=lambda p:sorted((q for q in p.iterdir() if all(n.isdigit() for n in q.name.split('.'))),key=lambda q:[int(n) for n in q.name.split('.')])[-1]
        vc=latest(args.sdk/'VC/Tools/MSVC');sdk=latest(args.sdk/'Windows Kits/10/Include')
        mfc=vc/'atlmfc/include'
        system=['/I'+str(p) for p in [vc/'include',*[sdk/n for n in ('ucrt','um','shared')]]]
        libraries=['/LIBPATH:'+str(p) for p in [vc/'lib/x64',*[args.sdk/'Windows Kits/10/Lib'/sdk.name/n/'x64' for n in ('ucrt','um')]]]
        redist=latest(args.sdk/'VC/Redist/MSVC')
    else:
        mfc=Path(os.environ['VCToolsInstallDir'])/'atlmfc/include'
        redist=Path(os.environ['VCToolsRedistDir'])
    for pattern in ('x64/Microsoft.VC*.CRT/*.dll','x64/Microsoft.VC*.MFC/*.dll'):
        for runtime in redist.glob(pattern):shutil.copy2(runtime,out/runtime.name)
    shutil.copy2(archive/'openmfc_archive.dll',out/'openmfc_archive.dll')
    env={**os.environ,'WINEDEBUG':os.environ.get('WINEDEBUG','-all')}
    if os.name!='nt':env['WINEDLLOVERRIDES']='vcruntime140,vcruntime140_1,msvcp140,mfc140u=n'
    def run(command):subprocess.run(command,cwd=out,env=env,check=True)
    flags=['/nologo','/EHs','/MD','/std:c++17','/Gy','/Gw','/W0','/DUNICODE','/D_UNICODE','/DNOMINMAX',*system]
    if args.optimize:flags.append('/O2')
    def link(name,options):
        def quote(value):
            text=str(value)
            if os.name!='nt' and Path(args.linker).name.lower() in ('link','link.exe'):
                if text.startswith('/') and Path(text).exists():text='Z:'+text
                elif ':/' in text:text=text.replace(':/',':Z:/',1)
            return '"'+text+'"'
        response=out/(name+'.rsp');response.write_text('\n'.join(quote(p) for p in options));run([args.linker,'@'+str(response)])
    for native in (False,True):
        name='array_native' if native else 'array_source';obj=out/(name+'.obj')
        include=['/I'+str(mfc),'/DOPENMFC_USE_SDK_MFC'] if native else ['/I'+str(ROOT/'include')]
        run([args.compiler,*flags,*include,'/D_AFXDLL','/c',str(ROOT/'tests/array_probe.cpp'),'/Fo'+str(obj)])
        link(name,['/nologo','/DLL','/OPT:REF','/OUT:'+str(out/(name+'.dll')),*libraries,obj,
                   mfc.parent/'lib/x64/mfc140u.lib' if native else archive/'openmfc_archive.lib',
                   'kernel32.lib','user32.lib','oleaut32.lib'])
        imports=build_string_imports.pe_imports(out/(name+'.dll'))
        if native:assert 'mfc140u.dll' in imports and 'openmfc_archive.dll' not in imports,imports
        else:assert 'openmfc_archive.dll' in imports and not any(d.startswith('mfc') for d in imports),imports
        print(name+': independent '+('installed SDK/native MFC' if native else 'public header/production DLL')+' imports verified',flush=True)
    obj=out/'runner.obj'
    run([args.compiler,*flags,'/c',str(ROOT/'tests/test_array_oracle.cpp'),'/Fo'+str(obj)])
    link('runner',['/nologo','/SUBSYSTEM:CONSOLE','/OPT:REF','/OUT:'+str(out/'test_array_oracle.exe'),*libraries,obj,'kernel32.lib'])
    run([str(out/'test_array_oracle.exe')] if os.name=='nt' else ['wine',str(out/'test_array_oracle.exe')])

if __name__=='__main__':main()
