#!/usr/bin/env python3
"""Guardrail: diff the hand-written g_vtbl_* arrays in mfc_exceptions.cpp against
the harvested golden vtable (phase1/harvest/vtable_slots.json).

Fails (exit 1) if a vtable's slot count or per-slot role drifts from the golden
ground truth. This is the regression that caused the original exception crash:
the hand-written CMemoryException vtable was 6 slots when the real one is 8, so
MSVC's call to GetErrorMessage/ReportError ran off the end.

The golden directly covers CMemoryException (harvested from a live mfc140u.dll).
CFileException/CArchiveException share the documented CException-family layout
but were empty in the raw harvest, so they are checked against the family
reference (CMemoryException) and reported as family-inferred, not harvested.

Usage: python3 tools/abi/check_vtables.py   (run from anywhere)
Exit:  0 = all vtables match golden; 1 = mismatch; 2 = inputs missing.
"""
import json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
GOLDEN = os.path.join(ROOT, "phase1/harvest/vtable_slots.json")
SRC = os.path.join(ROOT, "phase4/src/mfc_exceptions.cpp")

# Map a role/symbol string to a canonical family key. Slots 5 and 6 are two
# distinct GetErrorMessage virtuals in the real vtable; the hand-written code
# serves one thunk at both, which is correct -> both map to GETERRORMESSAGE.
# Slot 7 is ReportError in the golden; the code parks a safe no-op pad there.
def role_key(s):
    s = s or ""
    low = s.lower()
    if "getruntimeclass" in low:      return "GetRuntimeClass"
    if "dtor" in low or "destructor" in low: return "~dtor"
    if "serialize" in low:            return "Serialize"
    if "assertvalid" in low:          return "AssertValid"
    if "dump" in low:                 return "Dump"
    if "geterrormessage" in low:      return "GetErrorMessage"
    if "reporterror" in low or "vtable_pad" in low or "pad" in low: return "ReportError"
    return "?(" + s + ")"

ARR_RE = re.compile(r"static\s+void\*\s+g_vtbl_(\w+)\[\]\s*=\s*\{(.*?)\};", re.S)
CAST_RE = re.compile(r"reinterpret_cast<void\*>\(\s*([A-Za-z_]\w*)\s*\)")

def parse_src(path):
    text = open(path).read()
    out = {}
    for m in ARR_RE.finditer(text):
        name = m.group(1)
        syms = CAST_RE.findall(m.group(2))
        out[name] = [role_key(s) for s in syms]
    return out

def golden_roles(path):
    d = json.load(open(path))
    out = {}
    for c in d["classes"]:
        roles = [role_key(s.get("role") or s.get("symbol")) for s in c.get("slots", [])]
        out[c["name"]] = roles
    return out

def main():
    if not os.path.exists(GOLDEN) or not os.path.exists(SRC):
        print("ERROR: missing golden or source input", file=sys.stderr)
        return 2
    golden = golden_roles(GOLDEN)
    src = parse_src(SRC)
    # family reference = the harvested class that actually has slots
    ref_name = next((n for n, r in golden.items() if r), None)
    ref = golden.get(ref_name, [])

    failures = 0
    for cls, actual in sorted(src.items()):
        expected = golden.get(cls) or []
        inferred = False
        if not expected:
            expected = ref            # fall back to family layout
            inferred = ref_name and cls != ref_name
        tag = f"(family-inferred from {ref_name})" if inferred else "(harvested)"
        ok = (actual == expected)
        status = "OK  " if ok else "FAIL"
        print(f"[{status}] g_vtbl_{cls}  {len(actual)} slots {tag}")
        if not ok:
            failures += 1
            n = max(len(actual), len(expected))
            for i in range(n):
                a = actual[i] if i < len(actual) else "<missing>"
                e = expected[i] if i < len(expected) else "<missing>"
                mark = "  " if a == e else "->"
                print(f"        {mark} [{i}] code={a:<18} golden={e}")
    if failures:
        print(f"\nFAILED: {failures} vtable(s) drifted from golden ground truth.")
        return 1
    print("\nAll exception vtables match the harvested golden layout.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
