import pathlib
import unittest

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


if __name__ == "__main__":
    unittest.main()
