#!/usr/bin/env python3
r"""Build the complete Phase 4 DLL with the Windows/MSVC C++ ABI.

    python phase4/scripts/build_msvc.py --build /tmp/openmfc-msvc \
        --compiler /path/to/msvc/bin/x64/cl --sdk /path/to/msvc

On Windows, run from an x64 Visual Studio developer prompt; --sdk is optional.
--compile-only reports compiler failures while the remaining native build port
is in progress. Missing export implementations and duplicate definitions are
errors: this build does not generate default-return stubs or synthetic RTTI.
"""
import argparse
import concurrent.futures
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools'))
from gen_weak_stubs import generate_stub_name, is_data_export


def defined_symbols(path):
    """Read external definitions from standard or bigobj x64 COFF objects."""
    data = Path(path).read_bytes()
    if len(data) < 20:
        raise ValueError(f'{path}: truncated COFF header')
    if data[:4] == b'\0\0\xff\xff':
        if len(data) < 56 or struct.unpack_from('<H', data, 4)[0] < 2:
            raise ValueError(f'{path}: expected a COFF bigobj, not an import object')
        machine = struct.unpack_from('<H', data, 6)[0]
        offset, count = struct.unpack_from('<II', data, 48)
        entry = struct.Struct('<8sIiHBB')
    else:
        machine = struct.unpack_from('<H', data)[0]
        offset, count = struct.unpack_from('<II', data, 8)
        entry = struct.Struct('<8sIhHBB')
    if machine != 0x8664:
        raise ValueError(f'{path}: expected an x64 COFF object, got machine {machine:#x}')
    if not count:
        return set()
    strings = offset + count * entry.size
    if offset < 20 or strings + 4 > len(data):
        raise ValueError(f'{path}: truncated symbol table')
    string_size = struct.unpack_from('<I', data, strings)[0]
    if string_size < 4 or strings + string_size > len(data):
        raise ValueError(f'{path}: truncated string table')
    result = set()
    index = 0
    while index < count:
        raw, value, section, kind, storage, auxiliary = entry.unpack_from(data, offset + index * entry.size)
        if storage == 2 and section > 0:  # External, defined; ignore undefined/weak references.
            if raw[:4] == b'\0' * 4:
                start = struct.unpack_from('<I', raw, 4)[0]
                if start < 4 or start >= string_size:
                    raise ValueError(f'{path}: invalid symbol-name offset')
                end = data.find(b'\0', strings + start, strings + string_size)
                if end < 0:
                    raise ValueError(f'{path}: unterminated symbol name')
                name = data[strings + start:end].decode('ascii')
            else:
                name = raw.rstrip(b'\0').decode('ascii')
            if name.startswith('_Z'):
                raise ValueError(f'{path}: Itanium C++ symbol {name}; use cl or clang-cl')
            result.add(name)
        index += 1 + auxiliary
        if index > count:
            raise ValueError(f'{path}: invalid auxiliary-symbol count')
    return result


def export_definition(entries, symbols):
    """Preserve every mapped ordinal and require a compiled provider for it."""
    lines = ['LIBRARY openmfc', 'EXPORTS']
    missing = []
    names, ordinals = set(), set()
    for item in sorted(entries, key=lambda entry: entry['ordinal']):
        name, ordinal = item['symbol'], item['ordinal']
        if name in names or ordinal in ordinals or not 1 <= ordinal <= 65535:
            raise ValueError(f'duplicate export or invalid ordinal: {name} @{ordinal}')
        names.add(name)
        ordinals.add(ordinal)
        alias = generate_stub_name(name)
        if alias in symbols:
            target = f'{name}={alias}'
        elif name in symbols:
            target = name  # Real MSVC names, including runtime-class data.
        else:
            missing.append(item)
            continue
        lines.append(f'    {target} @{ordinal}' + (' DATA' if is_data_export(name) else ''))
    return '\n'.join(lines) + '\n', missing


def pe_exports(path):
    """Return name -> ordinal from a linked x64 PE export directory."""
    data = Path(path).read_bytes()
    if data[:2] != b'MZ' or len(data) < 64:
        raise ValueError(f'{path}: expected a PE image')
    pe = struct.unpack_from('<I', data, 0x3c)[0]
    if data[pe:pe + 4] != b'PE\0\0':
        raise ValueError(f'{path}: invalid PE signature')
    machine, sections = struct.unpack_from('<HH', data, pe + 4)
    optional_size = struct.unpack_from('<H', data, pe + 20)[0]
    optional = pe + 24
    if machine != 0x8664 or struct.unpack_from('<H', data, optional)[0] != 0x20b:
        raise ValueError(f'{path}: expected an x64 PE32+ image')
    section_table = optional + optional_size

    def file_offset(rva):
        for index in range(sections):
            header = section_table + index * 40
            size, address, raw_size, raw = struct.unpack_from('<IIII', data, header + 8)
            if address <= rva < address + max(size, raw_size):
                relative = rva - address
                if relative >= raw_size or raw + relative >= len(data):
                    break
                return raw + relative
        raise ValueError(f'{path}: unmapped RVA {rva:#x}')

    rva, size = struct.unpack_from('<II', data, optional + 112)
    if not rva or not size:
        return {}
    directory = file_offset(rva)
    base, functions, names, addresses, name_pointers, ordinal_pointers = struct.unpack_from('<IIIIII', data, directory + 16)
    result = {}
    for index in range(names):
        string = file_offset(struct.unpack_from('<I', data, file_offset(name_pointers) + index * 4)[0])
        name = data[string:data.index(b'\0', string)].decode('ascii')
        ordinal = struct.unpack_from('<H', data, file_offset(ordinal_pointers) + index * 2)[0]
        if ordinal >= functions or not struct.unpack_from('<I', data, file_offset(addresses) + ordinal * 4)[0]:
            raise ValueError(f'{path}: missing address for export {name}')
        if name in result:
            raise ValueError(f'{path}: duplicate export {name}')
        result[name] = base + ordinal
    return result


def latest(path):
    return sorted(path.iterdir(), key=lambda item: [int(part) for part in item.name.split('.')])[-1]


def compile_flags(sdk):
    flags = ['/nologo', '/EHs', '/MD', '/std:c++17', '/Gy', '/Gw', '/W0',
             '/DUNICODE', '/D_UNICODE', '/DNOMINMAX', '/D_WIN32_WINNT=0x0601',
             '/FIwinsock2.h', '/FIobjbase.h', '/FIcommdlg.h', '/FIshellapi.h',
             '/I' + str(ROOT / 'include'), '/I' + str(ROOT / 'phase4/src')]
    libraries = []
    if sdk:
        vc = latest(sdk / 'VC/Tools/MSVC')
        kit = latest(sdk / 'Windows Kits/10/Include')
        flags += ['/I' + str(path) for path in [vc / 'include'] + [kit / name for name in ('ucrt', 'um', 'shared')]]
        libraries = [vc / 'lib/x64'] + [sdk / 'Windows Kits/10/Lib' / kit.name / name / 'x64' for name in ('ucrt', 'um')]
    flags.append('/DOPENMFC_EXPORTS')
    return flags, libraries


def header_digest():
    digest = hashlib.sha256()
    for folder in (ROOT / 'include', ROOT / 'phase4/src'):
        for path in sorted(folder.rglob('*')):
            if path.suffix.lower() in {'.h', '.hpp', '.inl'}:
                digest.update(str(path.relative_to(ROOT)).encode())
                digest.update(path.read_bytes())
    return digest.hexdigest()


def object_key(source, compiler, flags, headers):
    executable = shutil.which(compiler)
    stamp = Path(executable).stat().st_mtime_ns if executable else None
    environment = {name: os.environ.get(name) for name in ('INCLUDE', 'CL', '_CL_')}
    payload = json.dumps([compiler, executable, stamp, flags, headers, environment]).encode() + source.read_bytes()
    return hashlib.sha256(payload).hexdigest()


def default_assembler(compiler):
    executable = shutil.which(compiler)
    if executable:
        sibling = Path(executable).with_name('ml64.exe' if os.name == 'nt' else 'ml64')
        if sibling.is_file():
            return str(sibling)
    return 'ml64' if os.name == 'nt' else 'llvm-ml'


def assembler_flags(assembler):
    return (['--m64'] if Path(assembler).name.lower().startswith('llvm-ml') else []) + ['/nologo', '/c']


def sources():
    # OpenMfcExports is an application helper. ManualThunks is an obsolete
    # generated default-return file, never a production implementation.
    return sorted(path for path in (ROOT / 'phase4/src').rglob('*') if path.suffix in {'.cpp', '.asm'}
                  if path.relative_to(ROOT / 'phase4/src').as_posix() not in {
                      'core/runtime/OpenMfcExports.cpp', 'core/runtime/ManualThunks.cpp'})


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--build', required=True, type=Path)
    parser.add_argument('--compiler', default='cl' if os.name == 'nt' else 'clang-cl')
    parser.add_argument('--assembler', help='x64 MASM-compatible assembler (ml64 or llvm-ml)')
    parser.add_argument('--sdk', type=Path)
    parser.add_argument('--jobs', type=int, default=4)
    parser.add_argument('--compile-only', action='store_true')
    args = parser.parse_args()
    if args.jobs < 1:
        parser.error('--jobs must be positive')
    if not re.fullmatch(r'(cl|clang-cl(?:-\d+)?)(\.exe)?', Path(args.compiler).name, re.I):
        parser.error('use cl or clang-cl for the Windows/MSVC C++ ABI')
    assembler = args.assembler or default_assembler(args.compiler)
    if not re.fullmatch(r'(ml64|llvm-ml(?:-\d+)?)(\.exe)?', Path(assembler).name, re.I):
        parser.error('use ml64 or llvm-ml for x64 MASM sources')
    out = args.build.resolve()
    out.mkdir(parents=True, exist_ok=True)
    flags, libraries = compile_flags(args.sdk.resolve() if args.sdk else None)
    if 'clang-cl' in Path(args.compiler).name.lower():
        # SDK headers use the Windows filesystem's case-insensitive includes.
        flags = [part for flag in flags for part in (['-imsvc', flag[2:]] if flag.startswith('/I') and args.sdk and not flag[2:].startswith(str(ROOT)) else [flag])]
    headers = header_digest()
    env = dict(os.environ)
    env.setdefault('WINEDEBUG', '-all')

    def compile_one(source):
        relative = source.relative_to(ROOT)
        obj = out / 'obj' / relative.with_suffix('.obj')
        obj.parent.mkdir(parents=True, exist_ok=True)
        metadata = obj.with_suffix('.build.json')
        tool = assembler if source.suffix == '.asm' else args.compiler
        tool_flags = assembler_flags(assembler) if source.suffix == '.asm' else flags
        key = object_key(source, tool, tool_flags, headers)
        result = {'source': relative.as_posix(), 'object': str(obj), 'log': str(obj.with_suffix('.log'))}
        if obj.is_file() and metadata.is_file():
            try:
                cached_key = json.loads(metadata.read_text()).get('key')
            except (ValueError, OSError):
                cached_key = None
            if cached_key == key:
                result.update(exit_code=0, cached=True)
                return result
        metadata.unlink(missing_ok=True)
        with Path(result['log']).open('w') as log:
            process = subprocess.run([tool, *tool_flags, str(source), '/Fo' + str(obj)]
                                     if source.suffix == '.asm' else
                                     [tool, *tool_flags, '/c', str(source), '/Fo' + str(obj)],
                                     cwd=out, env=env, stdout=log, stderr=subprocess.STDOUT)
        result.update(exit_code=process.returncode, cached=False)
        if process.returncode == 0:
            defined_symbols(obj)  # Reject non-MSVC ABI/architecture objects before caching.
            temporary = metadata.with_suffix('.tmp')
            temporary.write_text(json.dumps({'key': key}))
            temporary.replace(metadata)
        return result

    source_files = sources()
    results = []
    print(f'Compiling {len(source_files)} production sources with {args.jobs} jobs', flush=True)
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as pool:
        tasks = [pool.submit(compile_one, source) for source in source_files]
        for task in concurrent.futures.as_completed(tasks):
            result = task.result()
            results.append(result)
            if result['exit_code']:
                print(f'FAILED: {result["source"]}; see {result["log"]}', flush=True)
            if len(results) % 25 == 0 or len(results) == len(source_files):
                print(f'{len(results)}/{len(source_files)} completed', flush=True)
    (out / 'compile-report.json').write_text(json.dumps(results, indent=2))
    failed = [result for result in results if result['exit_code']]
    if failed:
        print(f'{len(failed)} compiler failures; full report: {out / "compile-report.json"}', file=sys.stderr)
        return 1
    if args.compile_only:
        print('All production sources compiled with the Windows/MSVC ABI', flush=True)
        return 0

    objects = [Path(result['object']) for result in results]
    symbols = set().union(*(defined_symbols(obj) for obj in objects))
    entries = json.loads((ROOT / 'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u']
    definition, missing = export_definition(entries, symbols)
    (out / 'missing-exports.json').write_text(json.dumps(missing, indent=2))
    if missing:
        print(f'{len(missing)} exports have no compiled provider; see {out / "missing-exports.json"}', file=sys.stderr)
        return 1
    (out / 'openmfc.def').write_text(definition)
    link = ['/nologo', '/DLL', '/MACHINE:X64', '/OPT:REF', '/OUT:' + str(out / 'openmfc.dll'),
            '/IMPLIB:' + str(out / 'openmfc.lib'), '/DEF:' + str(out / 'openmfc.def')]
    link += ['/LIBPATH:' + str(path) for path in libraries]
    link += [str(obj) for obj in sorted(objects)]
    link += [name + '.lib' for name in ('kernel32', 'gdi32', 'user32', 'comctl32', 'comdlg32',
             'wininet', 'ws2_32', 'advapi32', 'ole32', 'oleaut32', 'oleacc', 'uuid', 'oledlg',
             'crypt32', 'odbc32', 'uxtheme', 'gdiplus', 'd2d1', 'dwrite', 'windowscodecs')]
    response = out / 'link.rsp'
    response.write_text('\n'.join('"' + part + '"' for part in link))
    subprocess.run(['link' if os.name == 'nt' else 'lld-link', '@' + str(response)], check=True, cwd=out, env=env)
    exported = pe_exports(out / 'openmfc.dll')
    for entry in entries:
        if exported.get(entry['symbol']) != entry['ordinal']:
            raise ValueError(f'linked export has the wrong ordinal: {entry}')
    print(f'Built openmfc.dll/openmfc.lib; verified all {len(entries)} mapped export ordinals', flush=True)
    return 0


if __name__ == '__main__':
    try:
        raise SystemExit(main())
    except (ValueError, OSError, subprocess.CalledProcessError) as error:
        raise SystemExit(str(error))
