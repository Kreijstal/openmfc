#!/usr/bin/env python3
"""Validate the production x64 thunk and 128-bit arithmetic with MSVC."""
import argparse
import os
from pathlib import Path
import random
import shutil
import subprocess
import sys

ROOT=Path(__file__).resolve().parent.parent
sys.path.insert(0,str(ROOT/'phase4/scripts'))
import build_msvc


def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--build',required=True,type=Path)
    parser.add_argument('--compiler',default='cl' if os.name=='nt' else 'clang-cl')
    parser.add_argument('--assembler')
    parser.add_argument('--sdk',type=Path)
    parser.add_argument('--optimize',action='store_true')
    args=parser.parse_args()
    out=args.build.resolve();out.mkdir(parents=True,exist_ok=True)
    sdk=args.sdk.resolve() if args.sdk else None
    flags,libraries=build_msvc.compile_flags(sdk)
    if 'clang-cl' in Path(args.compiler).name.lower():
        flags=[part for flag in flags for part in
               (['-imsvc',flag[2:]] if flag.startswith('/I') and sdk and not flag[2:].startswith(str(ROOT)) else [flag])]
    if args.optimize:flags.append('/O2')
    assembler=args.assembler or build_msvc.default_assembler(args.compiler)
    env=dict(os.environ);env.setdefault('WINEDEBUG','-all')
    obj=out/'test_native_runtime.obj';assembly=out/'DHtmlThunk.obj'
    subprocess.run([args.compiler,*flags,'/c',str(ROOT/'tests/test_native_runtime.cpp'),'/Fo'+str(obj)],cwd=out,env=env,check=True)
    subprocess.run([assembler,*build_msvc.assembler_flags(assembler),'/Fo'+str(assembly),
                    str(ROOT/'phase4/src/core/view/DHtmlThunk.asm')],cwd=out,env=env,check=True)
    assert 'openmfc_DHCS_StdCallThunkHelper' in build_msvc.defined_symbols(assembly)
    exe=out/'test_native_runtime.exe'
    link=['/nologo','/SUBSYSTEM:CONSOLE','/OUT:'+str(exe),str(obj),str(assembly),'oleaut32.lib','kernel32.lib']
    link+=['/LIBPATH:'+str(path) for path in libraries]
    response=out/'link.rsp';response.write_text('\n'.join('"'+part+'"' for part in link))
    subprocess.run(['link' if os.name=='nt' else 'lld-link','@'+str(response)],cwd=out,env=env,check=True)
    if os.name=='nt':
        command=[str(exe)]
    else:
        if sdk:
            for name in ('vcruntime140.dll','vcruntime140_1.dll'):
                for path in (sdk/'VC/Redist/MSVC').glob('*/x64/Microsoft.VC*.CRT/'+name):shutil.copy2(path,out/name)
        env.setdefault('WINEDLLOVERRIDES','vcruntime140,vcruntime140_1=n')
        command=(['xvfb-run','-a'] if not env.get('DISPLAY') else [])+['wine',str(exe)]
    subprocess.run(command,cwd=out,env=env,check=True,timeout=120)
    cases=['','0','0000000000001','9'*200]
    for bit in range(129):
        for delta in (-1,0,1):cases.append(str(max(0,(1<<bit)+delta)))
    cases += [str(10**power-1) for power in range(1,40)]
    rng=random.Random(0x128)
    cases += [''.join(str(rng.randrange(10)) for _ in range(rng.randrange(1,513))) for _ in range(1000)]
    result=subprocess.run([*command,'--numeric'],input='\n'.join(cases)+'\n',text=True,capture_output=True,
                          cwd=out,env=env,check=True,timeout=120)
    expected=[f'{int(value or "0") & ((1<<128)-1):032x} {int(value or "0") & ((1<<128)-1)}' for value in cases]
    actual=result.stdout.splitlines()
    if actual!=expected:
        for index,(found,wanted) in enumerate(zip(actual,expected)):
            if found!=wanted:raise AssertionError(f'case {index}: {found!r} != {wanted!r}')
        raise AssertionError(f'expected {len(expected)} numeric results, got {len(actual)}')
    print(f'All {len(cases)} numeric coefficients match independent Python integer arithmetic',flush=True)


if __name__=='__main__':main()
