"""Validate native object-provider selection and linked export ordinals."""
import os
import json
from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'phase4/scripts'))
import build_msvc


def coff(symbols, big=False):
    """Small COFF fixture with section headers, long names and auxiliary rows."""
    header = bytearray(56 if big else 20)
    offset = len(header) + 80
    if big:
        struct.pack_into('<HHHH', header, 0, 0, 0xffff, 2, 0x8664)
        struct.pack_into('<III', header, 44, 2, offset, len(symbols))
        entry = struct.Struct('<8sIiHBB')
    else:
        struct.pack_into('<HHIIIHH', header, 0, 0x8664, 2, 0, offset, len(symbols), 0, 0)
        entry = struct.Struct('<8sIhHBB')
    strings = bytearray(4)
    rows = []
    for name, section, storage, auxiliary in symbols:
        raw = name.encode('ascii')
        if len(raw) > 8:
            raw = struct.pack('<II', 0, len(strings))
            strings.extend(name.encode('ascii') + b'\0')
        rows.append(entry.pack(raw, 0, section, 0, storage, auxiliary))
    struct.pack_into('<I', strings, 0, len(strings))
    return bytes(header) + bytes(80) + b''.join(rows) + bytes(strings)


class NativeExportTests(unittest.TestCase):
    def read_coff(self, contents):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'input.obj'
            path.write_bytes(contents)
            return build_msvc.defined_symbols(path)

    def test_external_definitions_and_auxiliary_rows(self):
        symbols = [('short', 1, 2, 1), ('ignored', 1, 2, 0),
                   ('?value@Client@@2HA', 2, 2, 0), ('unresolved', 0, 2, 0),
                   ('private', 1, 3, 0), ('weak', 0, 105, 0)]
        for big in (False, True):
            with self.subTest(big=big):
                self.assertEqual({'short', '?value@Client@@2HA'}, self.read_coff(coff(symbols, big)))

    def test_rejects_itanium_providers(self):
        with self.assertRaisesRegex(ValueError, 'Itanium'):
            self.read_coff(coff([('_ZN6Client5valueE', 1, 2, 0)]))

    def test_rejects_wrong_architecture_and_truncated_objects(self):
        obj = bytearray(coff([]))
        struct.pack_into('<H', obj, 0, 0x14c)
        with self.assertRaisesRegex(ValueError, 'x64'):
            self.read_coff(obj)
        with self.assertRaisesRegex(ValueError, 'truncated'):
            self.read_coff(b'bad')

    def test_export_aliases_and_native_data_preserve_ordinals(self):
        entries = [{'symbol': '?value@Client@@2HA', 'ordinal': 900},
                   {'symbol': '?Answer@@YAHXZ', 'ordinal': 300}]
        text, missing = build_msvc.export_definition(entries, {
            '?value@Client@@2HA', '?Answer@@YAHXZ', 'impl__Answer__YAHXZ'})
        self.assertFalse(missing)
        self.assertIn('?Answer@@YAHXZ=impl__Answer__YAHXZ @300\n', text)
        self.assertIn('?value@Client@@2HA @900 DATA\n', text)
        self.assertLess(text.index('@300'), text.index('@900'))

    def test_missing_provider_is_reported_without_a_fallback(self):
        entries = [{'symbol': '?Missing@@YAHXZ', 'ordinal': 40}]
        text, missing = build_msvc.export_definition(entries, {'unrelated'})
        self.assertEqual(entries, missing)
        self.assertNotIn('Missing', text)

    def test_duplicate_or_out_of_range_ordinals_are_errors(self):
        for entries in ([{'symbol': '?A@@YAXXZ', 'ordinal': 1}, {'symbol': '?B@@YAXXZ', 'ordinal': 1}],
                        [{'symbol': '?A@@YAXXZ', 'ordinal': 0}]):
            with self.assertRaises(ValueError):
                build_msvc.export_definition(entries, set())

    def test_repository_ordinal_mapping_is_valid_without_inventing_providers(self):
        entries = json.loads((ROOT / 'mfc_complete_ordinal_mapping.json').read_text())['exports']['mfc140u']
        text, missing = build_msvc.export_definition(entries, set())
        self.assertEqual(len(entries), len(missing))
        self.assertEqual('LIBRARY openmfc\nEXPORTS\n', text)

    def test_cli_rejects_mingw_before_running_a_compiler(self):
        with tempfile.TemporaryDirectory() as directory:
            process = subprocess.run([sys.executable, str(ROOT / 'phase4/scripts/build_msvc.py'),
                                      '--build', directory, '--compiler', 'x86_64-w64-mingw32-g++'],
                                     capture_output=True, text=True)
            self.assertEqual(2, process.returncode)
            self.assertIn('use cl or clang-cl', process.stderr)

    @unittest.skipIf(os.name=='nt','POSIX compatibility launcher')
    def test_compatibility_launcher_also_rejects_mingw(self):
        with tempfile.TemporaryDirectory() as directory:
            env=dict(os.environ,CXX='x86_64-w64-mingw32-g++',BUILD=directory,PYTHON=sys.executable)
            process=subprocess.run(['bash',str(ROOT/'phase4/scripts/build_phase4.sh'),'--compile-only'],
                                   capture_output=True,text=True,env=env)
            self.assertEqual(2,process.returncode)
            self.assertIn('use cl or clang-cl',process.stderr)

    def test_cache_invalidates_for_source_flags_headers_and_environment(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / 'input.cpp'
            source.write_text('int value=1;')
            key = build_msvc.object_key(source, 'cl', ['/MD'], 'headers')
            self.assertEqual(key, build_msvc.object_key(source, 'cl', ['/MD'], 'headers'))
            self.assertNotEqual(key, build_msvc.object_key(source, 'cl', ['/MT'], 'headers'))
            self.assertNotEqual(key, build_msvc.object_key(source, 'cl', ['/MD'], 'other headers'))
            with patch.dict(os.environ, {'INCLUDE': 'another SDK'}):
                self.assertNotEqual(key, build_msvc.object_key(source, 'cl', ['/MD'], 'headers'))
            source.write_text('int value=2;')
            self.assertNotEqual(key, build_msvc.object_key(source, 'cl', ['/MD'], 'headers'))

    def test_real_compiler_objects_and_linked_pe(self):
        compiler = os.environ.get('OPENMFC_TEST_MSVC_COMPILER') or shutil.which('cl' if os.name == 'nt' else 'clang-cl')
        linker = shutil.which('link' if os.name == 'nt' else 'lld-link')
        if not compiler or not linker:
            self.skipTest('requires cl/clang-cl and link/lld-link')
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory)
            source = out / 'providers.cpp'
            source.write_text('struct Client { static int value; };\nint Client::value=42;\n'
                              'extern "C" int impl__Answer__YAHXZ() { return 42; }\n')
            env = dict(os.environ, WINEDEBUG='-all')
            for big in (False, True):
                obj = out / ('big.obj' if big else 'regular.obj')
                subprocess.run([compiler, '/nologo', '/c', *(['/bigobj'] if big else []),
                                str(source), '/Fo' + str(obj)], cwd=out, env=env, check=True,
                               stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
                self.assertTrue({'?value@Client@@2HA', 'impl__Answer__YAHXZ'} <= build_msvc.defined_symbols(obj))
            entries = [{'symbol': '?Answer@@YAHXZ', 'ordinal': 300},
                       {'symbol': '?value@Client@@2HA', 'ordinal': 900}]
            text, missing = build_msvc.export_definition(entries, build_msvc.defined_symbols(obj))
            self.assertFalse(missing)
            definition = out / 'providers.def'
            definition.write_text(text)
            dll = out / 'providers.dll'
            subprocess.run([linker, '/nologo', '/DLL', '/NOENTRY', '/NODEFAULTLIB', str(obj),
                            '/DEF:' + str(definition), '/OUT:' + str(dll)], cwd=out, env=env,
                           check=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            self.assertEqual({entry['symbol']: entry['ordinal'] for entry in entries}, build_msvc.pe_exports(dll))

    def test_real_assembler_writes_the_requested_object_path(self):
        compiler=os.environ.get('OPENMFC_TEST_MSVC_COMPILER') or shutil.which('cl' if os.name=='nt' else 'clang-cl')
        assembler=build_msvc.default_assembler(compiler) if compiler else None
        if not assembler or not shutil.which(assembler):
            self.skipTest('requires an x64 MASM-compatible assembler')
        with tempfile.TemporaryDirectory() as directory:
            out=Path(directory);(out/'src').mkdir();(out/'objects').mkdir()
            source=out/'src/input.asm';obj=out/'objects/selected.obj'
            source.write_text('PUBLIC helper\n.code\nhelper PROC\nret\nhelper ENDP\nEND\n')
            subprocess.run(build_msvc.compile_command(source,obj,assembler,build_msvc.assembler_flags(assembler)),
                           cwd=out,env=dict(os.environ,WINEDEBUG='-all'),check=True,
                           stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            self.assertIn('helper',build_msvc.defined_symbols(obj))
            self.assertFalse((out/'input.obj').exists())


if __name__ == '__main__':
    unittest.main()
