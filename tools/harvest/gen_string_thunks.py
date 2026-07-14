#!/usr/bin/env python3
"""Generate real thunks for the ATL CStringT/CSimpleStringT exported string family.

Each exported method was a weak stub; a drop-in MSVC client imports it by ordinal, so a
stub means e.g. `CStringA s("hi");` runs a no-op ctor -> garbage. We emit self-contained
`extern "C" MS_ABI impl__...` thunks forwarding to the ATL-binary-compatible AtlStrT<CH>
core (phase4/src/atl_strt_core.h). We touch neither the .def nor any public header.

Signatures are keyed by the RAW mangled suffix (never normalized across char types), so a
same-char overload (CStringA(const char*)) is never confused with a converting overload
(CStringW(const char*) = ANSI->UTF16).  A thunk is emitted only for signatures present in
the tables below; every other combo keeps its safe weak stub, so growing the tables never
regresses.

  SAME[(method, tsig)]           -> spec, registered for BOTH narrow ({c}=D) and wide
                                    ({c}=_W); {c} marks the class's own char-type code.
  CROSS[(is_wide, method, rsig)] -> spec, one explicit entry (converting/Win32/by-value).
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import stub_coverage as sc
import gen_weak_stubs as gw

NARROW_SIMPLE = "?$CSimpleStringT@D$00@ATL@@"
NARROW_SIMPLE0 = "?$CSimpleStringT@D$0A@@ATL@@"
NARROW_STR = "?$CStringT@DV?$StrTraitMFC_DLL@DV?$ChTraitsCRT@D@ATL@@@@@ATL@@"
WIDE_SIMPLE = "?$CSimpleStringT@_W$00@ATL@@"
WIDE_SIMPLE0 = "?$CSimpleStringT@_W$0A@@ATL@@"
WIDE_STR = "?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@"
CLASS_STRINGS = [NARROW_SIMPLE, NARROW_SIMPLE0, NARROW_STR,
                 WIDE_SIMPLE, WIDE_SIMPLE0, WIDE_STR]

OUT_FILE = ROOT / "phase4" / "src" / "global_string_atl_thunks.cpp"


def owner_string(sym):
    return bool(re.match(r'^\?[A-Za-z0-9_]+@\?\$C(Simple)?StringT@[D_]', sym) or
                re.match(r'^\?\?[0-9A-Z_]\?\$C(Simple)?StringT@[D_]', sym))


def _impls_defined_elsewhere():
    import glob
    names = set()
    for f in glob.glob(str(ROOT / "phase4" / "src" / "*.cpp")):
        if Path(f).name == OUT_FILE.name:
            continue
        for m in re.finditer(r'\bimpl_[A-Za-z0-9_]+', Path(f).read_text(errors='ignore')):
            names.add(m.group(0))
    return names


def collect():
    m, _ = sc.load_current_manifest()
    syms = [e['symbol'] for e in m['real'] + m['stub'] if owner_string(e['symbol'])]
    elsewhere = _impls_defined_elsewhere()
    return sorted(s for s in syms if gw.generate_stub_name(s) not in elsewhere)


def split(sym):
    """Return (method, is_wide, raw_sig).  raw_sig starts at the calling-convention marker."""
    for c in CLASS_STRINGS:
        idx = sym.find(c)
        if idx != -1:
            head, sig = sym[:idx], sym[idx + len(c):]
            is_wide = "@_W" in c
            method = "op" + head[2:3] if head.startswith("??") else head[1:head.index('@')]
            return method, is_wide, sig
    raise ValueError("no class string: " + sym)


# ----- type placeholders in specs: {CH}=class char, {PCH}={CH}*, {PCCH}=const {CH}* -----
CTOR = 'CTOR'          # returns pThis (constructor)
REFSELF = 'REFSELF'    # returns AEAV01@ / AEAV12@ (self&) -> returns pThis
BYVAL = 'BYVAL'        # returns ?AV12@ (class by value) -> hidden return-pointer first arg

# ---- SAME: same-char overloads, one spec used for narrow (D) and wide (_W) ------------
# tsig uses {c} where the class's own char-type code appears in the mangled suffix.
S_ = '{c}'
SAME = {
    ('op0', 'QEAA@XZ'): (CTOR, [], 'self->Construct();'),
    ('op0', 'QEAA@PEB{c}@Z'): (CTOR, [('{PCCH}', 'psz')], 'self->ConstructPsz(psz);'),
    ('op0', 'QEAA@PEA{c}@Z'): (CTOR, [('{PCH}', 'psz')], 'self->ConstructPsz(psz);'),
    ('op0', 'QEAA@PEB{c}H@Z'): (CTOR, [('{PCCH}', 'psz'), ('int', 'n')], 'self->ConstructPszN(psz, n);'),
    ('op0', 'QEAA@{c}H@Z'): (CTOR, [('{CH}', 'ch'), ('int', 'n')], 'self->ConstructChRep(ch, n);'),
    ('op0', 'QEAA@AEBV01@@Z'): (CTOR, [('const void*', 'src')],
        'self->ConstructCopy(*reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src));'),
    ('op0', 'QEAA@PEAUIAtlStringMgr@1@@Z'): (CTOR, [('IAtlStringMgr*', 'mgr')], 'self->ConstructMgr(mgr);'),
    ('op0', 'QEAA@PEB{c}PEAUIAtlStringMgr@1@@Z'): (CTOR, [('{PCCH}', 'psz'), ('IAtlStringMgr*', 'mgr')],
        '(void)mgr; self->ConstructPsz(psz);'),
    ('op0', 'QEAA@PEB{c}HPEAUIAtlStringMgr@1@@Z'): (CTOR, [('{PCCH}', 'psz'), ('int', 'n'), ('IAtlStringMgr*', 'mgr')],
        '(void)mgr; self->ConstructPszN(psz, n);'),

    ('op1', 'QEAA@XZ'): ('void', [], 'self->Destruct();'),

    ('op4', 'QEAAAEAV01@PEB{c}@Z'): (REFSELF, [('{PCCH}', 'psz')], 'self->AssignPsz(psz);'),
    ('op4', 'QEAAAEAV01@{c}@Z'): (REFSELF, [('{CH}', 'ch')], 'self->AssignChar(ch);'),
    ('op4', 'QEAAAEAV01@AEBV01@@Z'): (REFSELF, [('const void*', 'src')],
        'self->AssignCopy(*reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src));'),

    ('opY', 'QEAAAEAV01@PEB{c}@Z'): (REFSELF, [('{PCCH}', 'psz')], 'self->AppendPsz(psz);'),
    ('opY', 'QEAAAEAV01@{c}@Z'): (REFSELF, [('{CH}', 'ch')], 'self->AppendChar(ch);'),
    ('opY', 'QEAAAEAV01@AEBV01@@Z'): (REFSELF, [('const void*', 'src')],
        'const openmfc_str::AtlStrT<{CH}>* o = reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src); self->AppendPszN(o->GetString(), o->GetLength());'),
    ('opY', 'QEAAAEAV01@AEBV?$CSimpleStringT@{c}$00@1@@Z'): (REFSELF, [('const void*', 'src')],
        'const openmfc_str::AtlStrT<{CH}>* o = reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src); self->AppendPszN(o->GetString(), o->GetLength());'),

    ('opA', 'QEBA{c}H@Z'): ('{CH}', [('int', 'i')], 'return cself->GetAt(i);'),
    ('opB', 'QEBAPEB{c}XZ'): ('{PCCH}', [], 'return cself->GetString();'),
    ('opB', 'QEAAAEAV?$CSimpleStringT@{c}$0A@@1@XZ'): (REFSELF, [], '(void)self;'),

    ('Append', 'QEAAXPEB{c}@Z'): ('void', [('{PCCH}', 'psz')], 'self->AppendPsz(psz);'),
    ('Append', 'QEAAXPEB{c}H@Z'): ('void', [('{PCCH}', 'psz'), ('int', 'n')], 'self->AppendPszN(psz, n);'),
    ('Append', 'QEAAXAEBV12@@Z'): ('void', [('const void*', 'src')],
        'const openmfc_str::AtlStrT<{CH}>* o = reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src); self->AppendPszN(o->GetString(), o->GetLength());'),
    ('AppendChar', 'QEAAX{c}@Z'): ('void', [('{CH}', 'ch')], 'self->AppendChar(ch);'),

    ('GetLength', 'QEBAHXZ'): ('int', [], 'return cself->GetLength();'),
    ('GetAllocLength', 'QEBAHXZ'): ('int', [], 'return cself->GetAllocLength();'),
    ('IsEmpty', 'QEBA_NXZ'): ('bool', [], 'return cself->IsEmpty();'),
    ('GetString', 'QEBAPEB{c}XZ'): ('{PCCH}', [], 'return cself->GetString();'),
    ('GetAt', 'QEBA{c}H@Z'): ('{CH}', [('int', 'i')], 'return cself->GetAt(i);'),
    ('Empty', 'QEAAXXZ'): ('void', [], 'self->Empty();'),
    ('GetManager', 'QEBAPEAUIAtlStringMgr@2@XZ'): ('IAtlStringMgr*', [], 'return cself->GetManager();'),

    ('GetBuffer', 'QEAAPEA{c}XZ'): ('{PCH}', [], 'return self->GetBufferDefault();'),
    ('GetBuffer', 'QEAAPEA{c}H@Z'): ('{PCH}', [('int', 'n')], 'return self->GetBuffer(n);'),
    ('GetBufferSetLength', 'QEAAPEA{c}H@Z'): ('{PCH}', [('int', 'n')], 'return self->GetBufferSetLength(n);'),
    ('ReleaseBuffer', 'QEAAXH@Z'): ('void', [('int', 'n')], 'self->ReleaseBuffer(n);'),
    ('ReleaseBufferSetLength', 'QEAAXH@Z'): ('void', [('int', 'n')], 'self->ReleaseBufferSetLength(n);'),
    ('LockBuffer', 'QEAAPEA{c}XZ'): ('{PCH}', [], 'return self->LockBuffer();'),
    ('UnlockBuffer', 'QEAAXXZ'): ('void', [], 'self->UnlockBuffer();'),
    ('Preallocate', 'QEAAXH@Z'): ('void', [('int', 'n')], 'self->Preallocate(n);'),
    ('FreeExtra', 'QEAAXXZ'): ('void', [], 'self->FreeExtra();'),
    ('Truncate', 'QEAAXH@Z'): ('void', [('int', 'n')], 'self->Truncate(n);'),

    ('SetAt', 'QEAAXH{c}@Z'): ('void', [('int', 'i'), ('{CH}', 'ch')], 'self->SetAt(i, ch);'),
    ('Delete', 'QEAAHHH@Z'): ('int', [('int', 'i'), ('int', 'n')], 'return self->Delete(i, n);'),
    ('Insert', 'QEAAHH{c}@Z'): ('int', [('int', 'i'), ('{CH}', 'ch')], 'return self->Insert(i, ch);'),
    ('Insert', 'QEAAHHPEB{c}@Z'): ('int', [('int', 'i'), ('{PCCH}', 'psz')], 'return self->Insert(i, psz);'),
    ('Remove', 'QEAAH{c}@Z'): ('int', [('{CH}', 'ch')], 'return self->Remove(ch);'),
    ('Replace', 'QEAAH{c}{c}@Z'): ('int', [('{CH}', 'a'), ('{CH}', 'b')], 'return self->ReplaceCh(a, b);'),
    ('MakeUpper', 'QEAAAEAV12@XZ'): (REFSELF, [], 'self->MakeUpper();'),
    ('MakeLower', 'QEAAAEAV12@XZ'): (REFSELF, [], 'self->MakeLower();'),
    ('MakeReverse', 'QEAAAEAV12@XZ'): (REFSELF, [], 'self->MakeReverse();'),
    ('SetString', 'QEAAXPEB{c}@Z'): ('void', [('{PCCH}', 'psz')], 'self->AssignPsz(psz);'),
    ('SetString', 'QEAAXPEB{c}H@Z'): ('void', [('{PCCH}', 'psz'), ('int', 'n')],
        'if (!psz || n <= 0) { self->Empty(); } else { self->Empty(); self->AppendPszN(psz, n); }'),

    ('Trim', 'QEAAAEAV12@XZ'): (REFSELF, [], 'Trim<{CH}>(self, ({CH})0, ({PCCH})nullptr, 3);'),
    ('Trim', 'QEAAAEAV12@{c}@Z'): (REFSELF, [('{CH}', 'ch')], 'Trim<{CH}>(self, ch, ({PCCH})nullptr, 3);'),
    ('Trim', 'QEAAAEAV12@PEB{c}@Z'): (REFSELF, [('{PCCH}', 'set')], 'Trim<{CH}>(self, ({CH})0, set, 3);'),
    ('TrimLeft', 'QEAAAEAV12@XZ'): (REFSELF, [], 'Trim<{CH}>(self, ({CH})0, ({PCCH})nullptr, 1);'),
    ('TrimLeft', 'QEAAAEAV12@{c}@Z'): (REFSELF, [('{CH}', 'ch')], 'Trim<{CH}>(self, ch, ({PCCH})nullptr, 1);'),
    ('TrimLeft', 'QEAAAEAV12@PEB{c}@Z'): (REFSELF, [('{PCCH}', 'set')], 'Trim<{CH}>(self, ({CH})0, set, 1);'),
    ('TrimRight', 'QEAAAEAV12@XZ'): (REFSELF, [], 'Trim<{CH}>(self, ({CH})0, ({PCCH})nullptr, 2);'),
    ('TrimRight', 'QEAAAEAV12@{c}@Z'): (REFSELF, [('{CH}', 'ch')], 'Trim<{CH}>(self, ch, ({PCCH})nullptr, 2);'),
    ('TrimRight', 'QEAAAEAV12@PEB{c}@Z'): (REFSELF, [('{PCCH}', 'set')], 'Trim<{CH}>(self, ({CH})0, set, 2);'),

    ('Find', 'QEBAH{c}H@Z'): ('int', [('{CH}', 'ch'), ('int', 'start')], 'return cself->FindCh(ch, start);'),
    ('Find', 'QEBAHPEB{c}H@Z'): ('int', [('{PCCH}', 'psz'), ('int', 'start')], 'return cself->FindPsz(psz, start);'),
    ('ReverseFind', 'QEBAH{c}@Z'): ('int', [('{CH}', 'ch')], 'return cself->ReverseFind(ch);'),
    ('FindOneOf', 'QEBAHPEB{c}@Z'): ('int', [('{PCCH}', 'set')], 'return cself->FindOneOf(set);'),
    ('Compare', 'QEBAHPEB{c}@Z'): ('int', [('{PCCH}', 'psz')], 'return cself->Compare(psz);'),
    ('CompareNoCase', 'QEBAHPEB{c}@Z'): ('int', [('{PCCH}', 'psz')], 'return cself->CompareNoCase(psz);'),
    ('Collate', 'QEBAHPEB{c}@Z'): ('int', [('{PCCH}', 'psz')], 'return cself->Collate(psz);'),
    ('CollateNoCase', 'QEBAHPEB{c}@Z'): ('int', [('{PCCH}', 'psz')], 'return cself->CollateNoCase(psz);'),
    ('Replace', 'QEAAHPEB{c}0@Z'): ('int', [('{PCCH}', 'a'), ('{PCCH}', 'b')], 'return ReplaceStr(self, a, b);'),

    ('Format', 'QEAAXPEB{c}ZZ'): ('void', [('{PCCH}', 'fmt')], 'VARFMT(Format);', False, True),
    ('AppendFormat', 'QEAAXPEB{c}ZZ'): ('void', [('{PCCH}', 'fmt')], 'VARFMT(AppendFormat);', False, True),
    ('FormatV', 'QEAAXPEB{c}PEAD@Z'): ('void', [('{PCCH}', 'fmt'), ('char*', 'args')],
        'self->FormatV(fmt, (va_list)args);'),
    ('AppendFormatV', 'QEAAXPEB{c}PEAD@Z'): ('void', [('{PCCH}', 'fmt'), ('char*', 'args')],
        'self->AppendFormatV(fmt, (va_list)args);'),

    # by-value returns (hidden return pointer)
    ('Left', 'QEBA?AV12@H@Z'): (BYVAL, [('int', 'n')],
        'int L=cself->GetLength(); if(n<0)n=0; if(n>L)n=L; r->ConstructPszN(cself->GetString(), n);'),
    ('Right', 'QEBA?AV12@H@Z'): (BYVAL, [('int', 'n')],
        'int L=cself->GetLength(); if(n<0)n=0; if(n>L)n=L; r->ConstructPszN(cself->GetString()+L-n, n);'),
    ('Mid', 'QEBA?AV12@H@Z'): (BYVAL, [('int', 'first')],
        'int L=cself->GetLength(); if(first<0)first=0; if(first>L)first=L; r->ConstructPszN(cself->GetString()+first, L-first);'),
    ('Mid', 'QEBA?AV12@HH@Z'): (BYVAL, [('int', 'first'), ('int', 'count')],
        'int L=cself->GetLength(); if(first<0)first=0; if(first>L)first=L; if(count<0)count=0; if(first+count>L)count=L-first; r->ConstructPszN(cself->GetString()+first, count);'),
    ('SpanIncluding', 'QEBA?AV12@PEB{c}@Z'): (BYVAL, [('{PCCH}', 'set')],
        'const {CH}* s=cself->GetString(); int i=0; for(; s[i]; ++i){ bool in=false; for(const {CH}* q=set; q&&*q; ++q) if(*q==s[i]){in=true;break;} if(!in) break; } r->ConstructPszN(s, i);'),
    ('SpanExcluding', 'QEBA?AV12@PEB{c}@Z'): (BYVAL, [('{PCCH}', 'set')],
        'const {CH}* s=cself->GetString(); int i=0; for(; s[i]; ++i){ bool in=false; for(const {CH}* q=set; q&&*q; ++q) if(*q==s[i]){in=true;break;} if(in) break; } r->ConstructPszN(s, i);'),
    ('Tokenize', 'QEBA?AV12@PEB{c}AEAH@Z'): (BYVAL, [('{PCCH}', 'toks'), ('int*', 'piStart')],
        'Tokenize(cself, r, toks, *piStart);'),

    # static helpers
    ('StringLength', 'SAHPEB{c}@Z'): ('int', [('{PCCH}', 'psz')], 'return openmfc_str::AtlStrT<{CH}>::StringLength(psz);', True),
    ('CopyChars', 'SAXPEA{c}PEB{c}H@Z'): ('void', [('{PCH}', 'd'), ('{PCCH}', 's'), ('int', 'n')],
        'openmfc_str::AtlStrT<{CH}>::CopyChars(d, s, n);', True),
    ('CopyCharsOverlapped', 'SAXPEA{c}PEB{c}H@Z'): ('void', [('{PCH}', 'd'), ('{PCCH}', 's'), ('int', 'n')],
        'openmfc_str::AtlStrT<{CH}>::CopyCharsOverlapped(d, s, n);', True),
    ('ThrowMemoryException', 'KAXXZ'): ('void', [], 'RaiseException(0xE06D7363u, 0, 0, nullptr);', True),

    # ATL-internal instance primitives
    ('SetLength', 'AEAAXH@Z'): ('void', [('int', 'n')], 'self->SetLength(n);'),
    ('Fork', 'AEAAXH@Z'): ('void', [('int', 'n')], 'self->Fork(n);'),
    ('Reallocate', 'AEAAXH@Z'): ('void', [('int', 'n')], 'self->Reallocate(n);'),
    ('PrepareWrite2', 'AEAAXH@Z'): ('void', [('int', 'n')], 'self->PrepareWrite2(n);'),
    ('PrepareWrite', 'AEAAPEA{c}H@Z'): ('{PCH}', [('int', 'n')], 'return self->PrepareWrite(n);'),
    ('Attach', 'AEAAXPEAUCStringData@2@@Z'): ('void', [('CStringData*', 'pd')], 'self->Attach(pd);'),
    ('GetData', 'AEBAPEAUCStringData@2@XZ'): ('CStringData*', [], 'return cself->GetData();'),
    ('SetManager', 'QEAAXPEAUIAtlStringMgr@2@@Z'): ('void', [('IAtlStringMgr*', 'mgr')], 'self->SetManagerEmpty(mgr);'),
    ('Construct', 'SAXPEAV12@@Z'): ('void', [('void*', 'p')],
        'openmfc_str::AtlStrT<{CH}>::ConstructStatic(reinterpret_cast<openmfc_str::AtlStrT<{CH}>*>(p));', True),
    ('CloneData', 'CAPEAUCStringData@2@PEAU32@@Z'): ('CStringData*', [('CStringData*', 'pd')],
        'return openmfc_str::AtlStrT<{CH}>::CloneData(pd);', True),
    ('Concatenate', 'KAXAEAV12@PEB{c}H1H@Z'): ('void',
        [('void*', 'dst'), ('{PCCH}', 'p1'), ('int', 'n1'), ('{PCCH}', 'p2'), ('int', 'n2')],
        'openmfc_str::AtlStrT<{CH}>::Concatenate(*reinterpret_cast<openmfc_str::AtlStrT<{CH}>*>(dst), p1, n1, p2, n2);', True),
    ('CheckImplicitLoad', 'AEAA_NPEBX@Z'): ('bool', [('const void*', 'p')], '(void)self; return p != nullptr;'),
}

# ---- CROSS: converting / char-specific overloads, explicit per is_wide ----------------
# Body helpers: `self`, `cself`, `r` (BYVAL return buffer) as in SAME.
def _conv_ctor(src_ctype, src_ch):
    return (CTOR, [(src_ctype, 'psz')], 'self->ConstructConvert<%s>(psz);' % src_ch)

def _conv_ctor_u(src_ctype):  # unsigned-char* narrow -> treat as char*
    return (CTOR, [(src_ctype, 'psz')], 'self->ConstructPsz(reinterpret_cast<const char*>(psz));')

CROSS = {}
# narrow (CStringA) converting ctors from wide, and unsigned char (same-byte) ctors
CROSS[(False, 'op0', 'QEAA@PEB_W@Z')] = _conv_ctor('const wchar_t*', 'wchar_t')
CROSS[(False, 'op0', 'QEAA@PEA_W@Z')] = _conv_ctor('wchar_t*', 'wchar_t')
CROSS[(False, 'op0', 'QEAA@PEB_WPEAUIAtlStringMgr@1@@Z')] = (CTOR,
    [('const wchar_t*', 'psz'), ('IAtlStringMgr*', 'mgr')], '(void)mgr; self->ConstructConvert<wchar_t>(psz);')
CROSS[(False, 'op0', 'QEAA@PEB_WHPEAUIAtlStringMgr@1@@Z')] = (CTOR,
    [('const wchar_t*', 'psz'), ('int', 'n'), ('IAtlStringMgr*', 'mgr')], '(void)mgr; self->ConstructConvertN<wchar_t>(psz, n);')
CROSS[(False, 'op0', 'QEAA@PEBE@Z')] = _conv_ctor_u('const unsigned char*')
CROSS[(False, 'op0', 'QEAA@PEAE@Z')] = _conv_ctor_u('unsigned char*')
CROSS[(False, 'op0', 'QEAA@PEBEPEAUIAtlStringMgr@1@@Z')] = (CTOR,
    [('const unsigned char*', 'psz'), ('IAtlStringMgr*', 'mgr')], '(void)mgr; self->ConstructPsz(reinterpret_cast<const char*>(psz));')
CROSS[(False, 'op4', 'QEAAAEAV01@PEBE@Z')] = (REFSELF, [('const unsigned char*', 'psz')],
    'self->AssignPsz(reinterpret_cast<const char*>(psz));')

# wide (CStringW) converting ctors/assign/append from narrow char
CROSS[(True, 'op0', 'QEAA@PEBD@Z')] = _conv_ctor('const char*', 'char')
CROSS[(True, 'op0', 'QEAA@PEAD@Z')] = _conv_ctor('char*', 'char')
CROSS[(True, 'op0', 'QEAA@PEBDPEAUIAtlStringMgr@1@@Z')] = (CTOR,
    [('const char*', 'psz'), ('IAtlStringMgr*', 'mgr')], '(void)mgr; self->ConstructConvert<char>(psz);')
CROSS[(True, 'op0', 'QEAA@PEBDHPEAUIAtlStringMgr@1@@Z')] = (CTOR,
    [('const char*', 'psz'), ('int', 'n'), ('IAtlStringMgr*', 'mgr')], '(void)mgr; self->ConstructConvertN<char>(psz, n);')
CROSS[(True, 'op0', 'QEAA@PEBE@Z')] = (CTOR, [('const unsigned char*', 'psz')], 'self->ConstructConvert<char>(reinterpret_cast<const char*>(psz));')
CROSS[(True, 'op0', 'QEAA@PEAE@Z')] = (CTOR, [('unsigned char*', 'psz')], 'self->ConstructConvert<char>(reinterpret_cast<const char*>(psz));')
CROSS[(True, 'op0', 'QEAA@PEBEPEAUIAtlStringMgr@1@@Z')] = (CTOR,
    [('const unsigned char*', 'psz'), ('IAtlStringMgr*', 'mgr')], '(void)mgr; self->ConstructConvert<char>(reinterpret_cast<const char*>(psz));')
CROSS[(True, 'op4', 'QEAAAEAV01@PEBE@Z')] = (REFSELF, [('const unsigned char*', 'psz')],
    'self->AssignConvert<char>(reinterpret_cast<const char*>(psz));')

# VARIANT ctors / assign / append (both char types)
for iw in (False, True):
    CROSS[(iw, 'op0', 'QEAA@AEBUtagVARIANT@@@Z')] = (CTOR, [('const void*', 'pv')],
        'self->ConstructVariant(reinterpret_cast<const VARIANT*>(pv));')
    CROSS[(iw, 'op0', 'QEAA@AEBUtagVARIANT@@PEAUIAtlStringMgr@1@@Z')] = (CTOR,
        [('const void*', 'pv'), ('IAtlStringMgr*', 'mgr')],
        '(void)mgr; self->ConstructVariant(reinterpret_cast<const VARIANT*>(pv));')
    CROSS[(iw, 'op4', 'QEAAAEAV01@AEBUtagVARIANT@@@Z')] = (REFSELF, [('const void*', 'pv')],
        'openmfc_str::AtlStrT<{CH}> t; t.ConstructVariant(reinterpret_cast<const VARIANT*>(pv)); self->AssignCopy(t); t.Destruct();')
    CROSS[(iw, 'opY', 'QEAAAEAV01@AEBUtagVARIANT@@@Z')] = (REFSELF, [('const void*', 'pv')],
        'openmfc_str::AtlStrT<{CH}> t; t.ConstructVariant(reinterpret_cast<const VARIANT*>(pv)); self->AppendPszN(t.GetString(), t.GetLength()); t.Destruct();')
    # copy ctor / assign / append from the CSimpleStringT<...,$0A> base subobject (same layout)
    base0 = '?$CSimpleStringT@%s$0A@@1@' % ('_W' if iw else 'D')
    CROSS[(iw, 'op0', 'QEAA@AEBV%s@Z' % base0)] = (CTOR, [('const void*', 'src')],
        'self->ConstructCopy(*reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src));')
    CROSS[(iw, 'op4', 'QEAAAEAV01@AEBV%s@Z' % base0)] = (REFSELF, [('const void*', 'src')],
        'self->AssignCopy(*reinterpret_cast<const openmfc_str::AtlStrT<{CH}>*>(src));')

# converting ctors/assign/append from the OTHER char type, plus other-type StringLength
# and Win32-backed methods, for both instantiations.
for iw in (False, True):
    OT = 'char' if iw else 'wchar_t'          # the other (converted-from) char type
    OC = 'D' if iw else '_W'                   # other char's mangling code
    PCOT = 'const %s*' % OT
    POT = '%s*' % OT
    # ctor(PC-other-string, int nLength) -> convert nLength chars
    CROSS[(iw, 'op0', 'QEAA@PEB%sH@Z' % OC)] = (CTOR, [(PCOT, 'psz'), ('int', 'n')],
        'self->ConstructConvertN<%s>(psz, n);' % OT)
    # ctor(other-char, int nRepeat) -> convert single char then repeat
    CROSS[(iw, 'op0', 'QEAA@%sH@Z' % OC)] = (CTOR, [(OT, 'ch'), ('int', 'n')],
        'openmfc_str::AtlStrT<{CH}> t; t.ConstructConvertN<%s>(&ch, 1); '
        'self->ConstructChRep(t.GetLength() ? t.GetString()[0] : ({CH})0, n); t.Destruct();' % OT)
    # operator=(PC-other) / operator=(other-char)
    CROSS[(iw, 'op4', 'QEAAAEAV01@PEB%s@Z' % OC)] = (REFSELF, [(PCOT, 'psz')],
        'self->AssignConvert<%s>(psz);' % OT)
    CROSS[(iw, 'op4', 'QEAAAEAV01@%s@Z' % OC)] = (REFSELF, [(OT, 'ch')],
        'openmfc_str::AtlStrT<{CH}> t; t.ConstructConvertN<%s>(&ch, 1); self->AssignCopy(t); t.Destruct();' % OT)
    # operator+=(PC-other) / operator+=(other-char) / operator+=(unsigned char)
    CROSS[(iw, 'opY', 'QEAAAEAV01@PEB%s@Z' % OC)] = (REFSELF, [(PCOT, 'psz')],
        'self->AppendConvert<%s>(psz);' % OT)
    CROSS[(iw, 'opY', 'QEAAAEAV01@%s@Z' % OC)] = (REFSELF, [(OT, 'ch')],
        'openmfc_str::AtlStrT<{CH}> t; t.ConstructConvertN<%s>(&ch, 1); self->AppendPszN(t.GetString(), t.GetLength()); t.Destruct();' % OT)
    if iw:  # wide += unsigned char -> convert as ANSI byte
        CROSS[(True, 'opY', 'QEAAAEAV01@E@Z')] = (REFSELF, [('unsigned char', 'ch')],
            'char c = (char)ch; self->AppendConvert<char>(&c);')
    else:   # narrow += unsigned char -> append raw byte
        CROSS[(False, 'opY', 'QEAAAEAV01@E@Z')] = (REFSELF, [('unsigned char', 'ch')],
            'self->AppendChar((char)ch);')
    # StringLength(PC-other) : length of an other-typed string
    CROSS[(iw, 'StringLength', 'SAHPEB%s@Z' % OC)] = ('int', [(PCOT, 'psz')],
        'int n = 0; if (psz) while (psz[n]) ++n; return n;', True)
    # Win32-backed
    CROSS[(iw, 'AllocSysString', 'QEBAPEA_WXZ')] = ('OLECHAR*', [], 'return cself->AllocSysString();')
    CROSS[(iw, 'SetSysString', 'QEBAPEA_WPEAPEA_W@Z')] = ('OLECHAR*', [('OLECHAR**', 'pbstr')],
        'return cself->SetSysString(pbstr);')
    CROSS[(iw, 'GetEnvironmentVariable', 'QEAAHPEB{c}@Z'.replace('{c}', '_W' if iw else 'D'))] = ('int',
        [('{PCCH}', 'name')], 'return self->LoadEnvVar(name);')
    CROSS[(iw, 'GetEnvironmentVariableW', 'QEAAHPEB{c}@Z'.replace('{c}', '_W' if iw else 'D'))] = ('int',
        [('{PCCH}', 'name')], 'return self->LoadEnvVar(name);')
if True:
    CROSS[(False, 'CharToOemA', 'QEAAXXZ')] = ('void', [], 'self->CharToOem();')
    CROSS[(False, 'OemToCharA', 'QEAAXXZ')] = ('void', [], 'self->OemToChar();')

# Resource / message formatting (both instantiations).
for iw in (False, True):
    C = '_W' if iw else 'D'
    # Format(nID, ...) / AppendFormat(nID, ...) : load format from resource then printf
    CROSS[(iw, 'Format', 'QEAAXIZZ')] = ('void', [('unsigned int', 'nID')],
        'openmfc_str::AtlStrT<{CH}> f; f.Construct(); f.LoadStr(GetResHandle(), nID);'
        ' va_list a; va_start(a, nID); self->FormatV(f.GetString(), a); va_end(a); f.Destruct();', False, True)
    CROSS[(iw, 'AppendFormat', 'QEAAXIZZ')] = ('void', [('unsigned int', 'nID')],
        'openmfc_str::AtlStrT<{CH}> f; f.Construct(); f.LoadStr(GetResHandle(), nID);'
        ' va_list a; va_start(a, nID); self->AppendFormatV(f.GetString(), a); va_end(a); f.Destruct();', False, True)
    # FormatMessage(fmt, ...) / FormatMessageW(fmt, ...) : ::FormatMessage inserts
    CROSS[(iw, 'FormatMessage', 'QEAAXPEB%sZZ' % C)] = ('void', [('{PCCH}', 'fmt')],
        'va_list a; va_start(a, fmt); self->FormatMessageStr(fmt, &a); va_end(a);', False, True)
    CROSS[(iw, 'FormatMessageW', 'QEAAXPEB%sZZ' % C)] = ('void', [('{PCCH}', 'fmt')],
        'va_list a; va_start(a, fmt); self->FormatMessageStr(fmt, &a); va_end(a);', False, True)
    # FormatMessage(nID, ...) / FormatMessageW(nID, ...)
    CROSS[(iw, 'FormatMessage', 'QEAAXIZZ')] = ('void', [('unsigned int', 'nID')],
        'openmfc_str::AtlStrT<{CH}> f; f.Construct(); f.LoadStr(GetResHandle(), nID);'
        ' va_list a; va_start(a, nID); self->FormatMessageStr(f.GetString(), &a); va_end(a); f.Destruct();', False, True)
    CROSS[(iw, 'FormatMessageW', 'QEAAXIZZ')] = ('void', [('unsigned int', 'nID')],
        'openmfc_str::AtlStrT<{CH}> f; f.Construct(); f.LoadStr(GetResHandle(), nID);'
        ' va_list a; va_start(a, nID); self->FormatMessageStr(f.GetString(), &a); va_end(a); f.Destruct();', False, True)
    # FormatMessageV(fmt, va_list*)
    CROSS[(iw, 'FormatMessageV', 'QEAAXPEB%sPEAPEAD@Z' % C)] = ('void',
        [('{PCCH}', 'fmt'), ('char**', 'pArgs')], 'self->FormatMessageStr(fmt, (va_list*)pArgs);')

# narrow LoadStringW (the wide variants are owned by strcore.cpp)
CROSS[(False, 'LoadStringW', 'QEAAHI@Z')] = ('int', [('unsigned int', 'nID')],
    'return self->LoadStr(GetResHandle(), nID);')
CROSS[(False, 'LoadStringW', 'QEAAHPEAUHINSTANCE__@@I@Z')] = ('int',
    [('HINSTANCE', 'hInst'), ('unsigned int', 'nID')], 'return self->LoadStr(hInst, nID);')
CROSS[(False, 'LoadStringW', 'QEAAHPEAUHINSTANCE__@@IG@Z')] = ('int',
    [('HINSTANCE', 'hInst'), ('unsigned int', 'nID'), ('unsigned short', 'lang')],
    '(void)lang; return self->LoadStr(hInst, nID);')


PRELUDE_HELPERS = r'''
// Resolve the module whose string resources CStringT::LoadString / Format(nID) use.
HINSTANCE AFXAPI AfxGetResourceHandle();
HINSTANCE AFXAPI AfxGetInstanceHandle();
namespace {
static HINSTANCE GetResHandle() {
    HINSTANCE h = AfxGetResourceHandle();
    if (!h) h = AfxGetInstanceHandle();
    if (!h) h = (HINSTANCE)GetModuleHandleW(nullptr);
    return h;
}
template <class CH>
void Trim(openmfc_str::AtlStrT<CH>* s, CH ch, const CH* set, int mode) {
    // mode bit1=left, bit2=right.  ch!=0 -> trim that char; set!=0 -> trim any in set;
    // else whitespace.
    auto match = [&](CH c) -> bool {
        if (ch) return c == ch;
        if (set) { for (const CH* q = set; *q; ++q) if (*q == c) return true; return false; }
        return c == (CH)' ' || c == (CH)'\t' || c == (CH)'\r' || c == (CH)'\n' ||
               c == (CH)'\v' || c == (CH)'\f';
    };
    int len = s->GetLength();
    if (len == 0) return;
    const CH* p = s->GetString();
    int start = 0, end = len;
    if (mode & 2) { while (end > 0 && match(p[end - 1])) --end; }
    if (mode & 1) { while (start < end && match(p[start])) ++start; }
    if (start == 0 && end == len) return;
    openmfc_str::AtlStrT<CH> tmp; tmp.ConstructPszN(s->GetString() + start, end - start);
    s->AssignCopy(tmp); tmp.Destruct();
}

template <class CH>
int ReplaceStr(openmfc_str::AtlStrT<CH>* s, const CH* oldS, const CH* newS) {
    if (!oldS || !*oldS) return 0;
    if (!newS) { static const CH z = 0; newS = &z; }
    int oldL = 0; while (oldS[oldL]) ++oldL;
    int newL = 0; while (newS[newL]) ++newL;
    // count occurrences
    int cnt = 0; const CH* src = s->GetString();
    for (const CH* p = src; *p; ) {
        const CH* a = p; const CH* b = oldS;
        while (*a && *b && *a == *b) { ++a; ++b; }
        if (!*b) { ++cnt; p += oldL; } else ++p;
    }
    if (cnt == 0) return 0;
    int srcL = s->GetLength();
    int outL = srcL + (newL - oldL) * cnt;
    openmfc_str::AtlStrT<CH> out; out.ConstructChRep((CH)' ', outL);
    CH* w = out.Buffer(); const CH* p = s->GetString();
    while (*p) {
        const CH* a = p; const CH* b = oldS;
        while (*a && *b && *a == *b) { ++a; ++b; }
        if (!*b) { for (int i = 0; i < newL; ++i) *w++ = newS[i]; p += oldL; }
        else *w++ = *p++;
    }
    *w = 0;
    s->AssignCopy(out); out.Destruct();
    return cnt;
}

template <class CH>
void Tokenize(const openmfc_str::AtlStrT<CH>* s, openmfc_str::AtlStrT<CH>* r,
              const CH* toks, int& iStart) {
    auto isTok = [&](CH c) -> bool { for (const CH* q = toks; q && *q; ++q) if (*q == c) return true; return false; };
    int len = s->GetLength();
    const CH* p = s->GetString();
    if (iStart < 0 || iStart > len || !toks) { r->Construct(); iStart = -1; return; }
    int i = iStart;
    while (i < len && isTok(p[i])) ++i;        // skip leading delimiters
    if (i >= len) { r->Construct(); iStart = -1; return; }
    int start = i;
    while (i < len && !isTok(p[i])) ++i;        // scan token
    r->ConstructPszN(p + start, i - start);
    iStart = (i < len) ? i + 1 : len + 1;
}
} // namespace
'''


def cfor(t, ch):
    return t.replace('{PCCH}', 'const %s*' % ch).replace('{PCH}', '%s*' % ch).replace('{CH}', ch)


def build_concrete():
    concrete = {}
    for (method, tsig), spec in SAME.items():
        for iw, code in ((False, 'D'), (True, '_W')):
            concrete[(iw, method, tsig.replace('{c}', code))] = spec
    for key, spec in CROSS.items():
        concrete[key] = spec
    return concrete


CONCRETE = build_concrete()


def emit(sym, method, is_wide, rawsig):
    spec = CONCRETE.get((is_wide, method, rawsig))
    if spec is None:
        return None
    ret = spec[0]; params = spec[1]; body = spec[2]
    is_static = len(spec) > 3 and spec[3]
    is_vararg = len(spec) > 4 and spec[4]
    ch = 'wchar_t' if is_wide else 'char'
    impl = gw.generate_stub_name(sym)
    S = 'openmfc_str::AtlStrT<%s>' % ch

    decl = []
    if ret == BYVAL:
        decl.append('void* pRet')
    if not is_static:
        decl.append('void* pThis')
    for ct, nm in params:
        decl.append('%s %s' % (cfor(ct, ch), nm))
    if is_vararg:
        decl.append('...')

    b = cfor(body, ch)
    if b == 'VARFMT(Format);':
        b = 'va_list a; va_start(a, fmt); self->FormatV(fmt, a); va_end(a);'
    elif b == 'VARFMT(AppendFormat);':
        b = 'va_list a; va_start(a, fmt); self->AppendFormatV(fmt, a); va_end(a);'

    uses_self = re.search(r'\bself\b', b) is not None or ret in (CTOR, REFSELF)
    uses_cself = re.search(r'\bcself\b', b) is not None
    setup = ''
    if ret == BYVAL:
        setup += '    %s* r = reinterpret_cast<%s*>(pRet);\n' % (S, S)
        uses_cself = True
    if not is_static:
        if uses_self:
            setup += '    %s* self = reinterpret_cast<%s*>(pThis);\n' % (S, S)
        if uses_cself:
            setup += '    const %s* cself = reinterpret_cast<const %s*>(pThis);\n' % (S, S)

    if ret in (CTOR, REFSELF):
        cret, tail = 'void*', '\n    return pThis;'
    elif ret == BYVAL:
        cret, tail = 'void*', '\n    return pRet;'
    else:
        cret, tail = cfor(ret, ch), ''

    lines = ['// Symbol: %s' % sym,
             'extern "C" %s MS_ABI %s(%s) {' % (cret, impl, ', '.join(decl))]
    if setup:
        lines.append(setup.rstrip('\n'))
    lines.append('    ' + b + tail)
    lines.append('}')
    return '\n'.join(lines)


PREAMBLE = '''// AUTO-GENERATED by tools/harvest/gen_string_thunks.py -- DO NOT EDIT BY HAND.
// Real implementations of the exported ATL CStringT<char>/CStringT<wchar_t> (and their
// CSimpleStringT bases) methods.  Each forwards to the ATL-binary-compatible AtlStrT<CH>
// core.  See gen_string_thunks.py for the authoritative signature tables.
#include "atl_strt_core.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
''' + PRELUDE_HELPERS


def main():
    syms = collect()
    out = [PREAMBLE]
    n = 0
    for s in syms:
        method, is_wide, rawsig = split(s)
        t = emit(s, method, is_wide, rawsig)
        if t:
            out.append('')
            out.append(t)
            n += 1
    OUT_FILE.write_text('\n'.join(out) + '\n', encoding='ascii')
    print("emitted %d/%d string thunks -> %s" % (n, len(syms), OUT_FILE))


if __name__ == '__main__':
    main()
