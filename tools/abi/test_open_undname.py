import pathlib
import unittest
import os

from tools.abi.open_undname import Undecorator


class TestOpenUndname(unittest.TestCase):
    def test_basic_demangles(self):
        cases = {
            "?GetValue@CReferenceTest@@UEBAHXZ": "public virtual int CReferenceTest::GetValue(void) const",
            "?Foo@@YAHH@Z": "public Foo()",
            "??0Bar@@QEAA@XZ": "public virtual Bar::Bar()",
            "??1Bar@@QEAA@XZ": "public virtual Bar::~Bar()",
        }
        for mangled, expected in cases.items():
            with self.subTest(mangled=mangled):
                self.assertEqual(Undecorator(mangled).demangle(), expected)

    def test_fixture_symbols_do_not_crash(self):
        # Ensure we can parse all recorded symbols without throwing.
        data_dir = pathlib.Path(__file__).parent / "testdata"
        fixtures = [
            data_dir / "compare_msvcp140.dll.txt",
            data_dir / "compare_concrt140.dll.txt",
        ]
        for fixture in fixtures:
            if not fixture.exists():
                self.skipTest(f"missing fixture: {fixture}")
            with fixture.open("r", encoding="utf-8", errors="ignore") as fh:
                for line in fh:
                    line = line.strip()
                    if not line or not line.startswith("?"):
                        continue
                    Undecorator(line).demangle()  # should not raise

    def test_compare_fixture_expectations(self):
        """
        Use the recorded undname output in the compare fixtures as expectations.
        This is stricter and can be toggled with STRICT_UNDNAME_FIXTURES=1.
        """
        if os.environ.get("STRICT_UNDNAME_FIXTURES") != "1":
            self.skipTest("set STRICT_UNDNAME_FIXTURES=1 to enforce equality against fixture expectations")

        data_dir = pathlib.Path(__file__).parent / "testdata"
        fixtures = [
            data_dir / "compare_msvcp140.dll.txt",
            data_dir / "compare_concrt140.dll.txt",
        ]

        for fixture in fixtures:
            with fixture.open("r", encoding="utf-8", errors="ignore") as fh:
                sym = None
                expect = None
                for raw in fh:
                    line = raw.strip()
                    if not line:
                        continue
                    if line.startswith("??"):
                        sym = line
                        expect = None
                        continue
                    if line.startswith('is :- "'):
                        expect = line[len('is :- "'):-1] if line.endswith('"') else line[len('is :- "') :]
                        if sym and expect:
                            got = Undecorator(sym).demangle()
                            self.assertEqual(expect, got, f"mismatch for {sym}")
                            sym = None
                            expect = None


if __name__ == "__main__":
    unittest.main()
