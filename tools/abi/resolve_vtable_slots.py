#!/usr/bin/env python3
"""Resolve a raw harvested vtable dump into a name-resolved golden vtable file.

Input:
  phase1/harvest/raw_vtable_slots.json   -- live in-DLL slot RVAs + export ordinals
  mfc_complete_ordinal_mapping.json      -- ordinal -> mangled symbol (dumpbin /ALL of mfc140u, 14103 exports)

Output (stdout, or --out):
  phase1/harvest/vtable_slots.json        -- golden, one entry per real slot

Resolution rules (kept deliberately conservative / honest):
  * ordinal "@N"        -> exact export hit; symbol is trustworthy.
  * ordinal "@N+0xNN"   -> address is *inside* (or just past) export N; not a real
                           export entry point. The slot is real (it is a vtable
                           entry) but its name cannot be proven from the export
                           table alone -> emitted as rva-only with confidence
                           "rva-only".
  * a slot whose nearest export is MSVC *data* (mangling token @@2 / @@3, i.e.
    a static/global rather than a function) and which sits far from the code
    cluster is treated as a past-the-end over-read and truncates the vtable.
"""
import json, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def load_ord_map(path):
    d = json.load(open(path))
    exp = d["exports"]
    mod = next(iter(exp.values())) if len(exp) == 1 else exp.get("mfc140u", [])
    m = {}
    for e in mod:
        o = e.get("ordinal")
        if o is not None:
            m[int(o)] = e.get("symbol") or e.get("name")
    return m

def is_data_symbol(sym):
    # MSVC mangling: '@@2'/'@@3' = static/global data member; functions use a
    # calling-convention/this token instead. Trailing @A/@B = const/volatile data.
    if not sym:
        return False
    return bool(re.search(r"@@[23]", sym))

ORD_RE = re.compile(r"^@(\d+)(?:\+0x([0-9a-fA-F]+))?$")

# Fixed MFC vtable contract for the CObject -> CException -> CSimpleException
# family (x64 _AFXDLL build). Slots 0/5/6/7 are independently confirmed by exact
# export hits in the harvest; 1-4 are the invariant CObject prologue.
CEXCEPTION_ROLES = {
    0: "GetRuntimeClass",
    1: "~dtor (scalar-deleting destructor)",
    2: "Serialize",
    3: "AssertValid",
    4: "Dump",
    5: "GetErrorMessage (CException, non-const)",
    6: "GetErrorMessage (CSimpleException, const)",
    7: "ReportError",
}
ROLE_TABLE = {
    "CMemoryException": CEXCEPTION_ROLES,
    "CFileException": CEXCEPTION_ROLES,
    "CArchiveException": CEXCEPTION_ROLES,
}

def resolve(raw_path, ordmap_path):
    raw = json.load(open(raw_path))
    omap = load_ord_map(ordmap_path)
    out_classes = []
    for cls in raw["classes"]:
        roles = ROLE_TABLE.get(cls["name"], {})
        rawslots = cls.get("slots", [])
        # ICF detection: an rva shared by >1 slot is a folded shared thunk
        # (trivial no-ops Serialize/AssertValid/Dump fold to one address).
        rva_counts = {}
        for s in rawslots:
            rva_counts[s["rva"]] = rva_counts.get(s["rva"], 0) + 1
        slots = []
        truncated_at = None
        for s in rawslots:
            # harvest emits the resolved token under "symbol"; older hand-made
            # raw dumps used "ordinal". Accept either.
            tok = s.get("symbol") or s.get("ordinal") or ""
            mo = ORD_RE.match(tok)
            ordn = int(mo.group(1)) if mo else None
            off = int(mo.group(2), 16) if (mo and mo.group(2)) else 0
            sym = omap.get(ordn)
            exact = (off == 0 and ordn is not None)
            folded = rva_counts.get(s["rva"], 0) > 1
            # over-read detection: a non-exact hit landing on a data export far
            # from the code cluster is past the vtable end.
            if not exact and is_data_symbol(sym):
                truncated_at = s["index"]
                break
            entry = {"index": s["index"], "rva": s["rva"]}
            if s.get("module"):
                entry["module"] = s["module"]
            if s["index"] in roles:
                entry["role"] = roles[s["index"]]
            if folded:
                # shared/folded slot: the export name at this address is a
                # co-folded symbol, not this slot's semantic identity.
                entry["symbol"] = None
                entry["confidence"] = "folded-icf"
                if exact and sym:
                    entry["cofolded_export"] = sym
                elif "+0x" in tok:
                    entry["address"] = tok
            elif exact and sym:
                entry["symbol"] = sym
                entry["confidence"] = "exact-ordinal"
            elif "+0x" in tok and tok.split("+0x")[0].endswith(".dll"):
                # slot lives in a module we have no ordinal->name map for
                # (CFile/CArchive vtables are in mfc140.dll). Real slot, address
                # known, name not resolvable here.
                entry["symbol"] = None
                entry["confidence"] = "cross-module-rva"
                entry["address"] = tok
            else:
                # real slot, name not provable from exports (dtor not exported)
                entry["symbol"] = None
                entry["confidence"] = "rva-only"
                if sym:
                    entry["nearest_export"] = f"{sym}+0x{off:x}"
            slots.append(entry)
        oc = {"name": cls["name"], "slots": slots}
        if truncated_at is not None:
            oc["note"] = f"vtable ends at slot {truncated_at} (next entry is a data export = past-the-end over-read)"
        if not slots and not cls.get("slots"):
            oc["note"] = "not harvested (empty in raw dump)"
        out_classes.append(oc)
    return {
        "_comment": "Golden MFC exception vtable slot maps, resolved from a live "
                    "MFC harvest. Confidence: 'exact-ordinal' = proven mfc140u.dll "
                    "export hit; 'folded-icf' = ICF-shared no-op thunk; 'rva-only' = "
                    "real slot not individually exported (e.g. destructor); "
                    "'cross-module-rva' = slot in a module with no ordinal map here "
                    "(CFile/CArchive vtables live in mfc140.dll). Per-slot 'role' is "
                    "the fixed CObject/CException ABI contract.",
        "source_raw": os.path.relpath(raw_path, ROOT),
        "source_ordmap": os.path.relpath(ordmap_path, ROOT),
        "classes": out_classes,
    }

if __name__ == "__main__":
    raw = os.path.join(ROOT, "phase1/harvest/raw_vtable_slots.json")
    omap = os.path.join(ROOT, "mfc_complete_ordinal_mapping.json")
    out = resolve(raw, omap)
    txt = json.dumps(out, indent=2)
    if "--out" in sys.argv:
        dst = os.path.join(ROOT, "phase1/harvest/vtable_slots.json")
        open(dst, "w").write(txt + "\n")
        print(f"wrote {dst}")
    else:
        print(txt)
