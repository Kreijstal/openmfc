#!/usr/bin/env python3
"""Generate real thunks for the exported Dialog Data Exchange/Validation helpers
(DDX_/DDP_/DDV_).  Forwards to the faithful Win32-control exchange core in
phase4/src/ddx_core.h.  Covers the control-backed families; recordset-bound (DDX_Field*),
OLE-control (DDX_OC*), date-struct and DHtml variants are left as safe weak stubs.

No .def/header change: each mangled export is already aliased to its impl_ name.
"""
import re, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import stub_coverage as sc
import gen_weak_stubs as gw

OUT = ROOT / "phase4" / "src" / "global_ddx_exchange.cpp"
CSTR = "V?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@"

# value-type single-char code -> (C type, DDX TCode enum, is CString)
NUM = {
    'E': ('unsigned char', 'openmfc_ddx::T_BYTE'),
    'F': ('short', 'openmfc_ddx::T_SHORT'),
    'H': ('int', 'openmfc_ddx::T_INT'),
    'I': ('unsigned int', 'openmfc_ddx::T_UINT'),
    'J': ('long', 'openmfc_ddx::T_LONG'),
    'K': ('unsigned long', 'openmfc_ddx::T_DWORD'),
    'M': ('float', 'openmfc_ddx::T_FLOAT'),
    'N': ('double', 'openmfc_ddx::T_DOUBLE'),
}
NUM2 = {'_J': ('long long', 'openmfc_ddx::T_I64'), '_K': ('unsigned long long', 'openmfc_ddx::T_U64')}


def owner(sym):
    return re.match(r'^\?DD[XPV]_', sym) is not None


def collect():
    m, _ = sc.load_current_manifest()
    import glob
    elsewhere = set()
    for f in glob.glob(str(ROOT / "phase4" / "src" / "*.cpp")):
        if Path(f).name == OUT.name: continue
        for mm in re.finditer(r'\bimpl_[A-Za-z0-9_]+', Path(f).read_text(errors='ignore')):
            elsewhere.add(mm.group(0))
    syms = [e['symbol'] for e in m['real'] + m['stub'] if owner(e['symbol'])]
    return sorted(s for s in syms if gw.generate_stub_name(s) not in elsewhere), elsewhere


def fname(sym):
    return re.match(r'^\?(DD[XPV]_[A-Za-z_]+?)@', sym).group(1)


def gsuffix(sym):
    """global funcs: after '?Name@@YAX' ... return the param encoding (before final @Z)."""
    mm = re.match(r'^\?[A-Za-z_]+@@YAX(.*)@Z$', sym)
    return mm.group(1) if mm else None


def valcode_after_idc(suffix):
    """suffix begins 'PEAVCDataExchange@@H<value...>'.  Return the value-type token."""
    body = suffix[len("PEAVCDataExchange@@H"):]
    return body


def parse_value(body):
    """body like 'AEAH' or 'AEAV?$CStringT...@@' or 'PEAH'. Return ('num',ctype,code)|('str',)|None"""
    m2 = re.match(r'^A?EA?([EFHIJKMN])$', body)  # AEA<code>  (reference)
    if body.startswith('AEA') or body.startswith('PEA'):
        inner = body[3:]
        if inner in NUM:
            return ('num', NUM[inner][0], NUM[inner][1], body.startswith('PEA'))
        if inner[:2] in NUM2:
            return ('num', NUM2[inner[:2]][0], NUM2[inner[:2]][1], body.startswith('PEA'))
        if inner == CSTR:
            return ('str', body.startswith('PEA'))
        if inner == '_W':   # wchar buffer (LPTSTR) - handle as string-to-buffer, skip
            return None
    return None


SPECS = []   # (impl_name, decl_params, body)


def add(sym, params, body):
    SPECS.append((sym, gw.generate_stub_name(sym), params, body))


def build():
    syms, _ = collect()
    covered = 0
    for s in syms:
        fn = fname(s)
        suf = gsuffix(s)
        if suf is None:   # member (DHtml) - skip
            continue
        # ---- DDV first (no nIDC) ----
        if fn.startswith('DDV_MinMax'):
            body_enc = suf[len("PEAVCDataExchange@@"):]
            # three same-type args: by value (HHH) or const-ref for float/double (AEBN NN)
            code = None
            if re.match(r'^([EFHIJK])\1\1$', body_enc):
                ct = {'E':'unsigned char','F':'short','H':'int','I':'unsigned int','J':'long','K':'unsigned long'}[body_enc[0]]
                add(s, [('void*','pDX'),(ct,'v'),(ct,'lo'),(ct,'hi')], 'openmfc_ddx::MinMax(pDX, v, lo, hi);')
                covered += 1; continue
            if re.match(r'^(III)$', body_enc):
                add(s, [('void*','pDX'),('unsigned int','v'),('unsigned int','lo'),('unsigned int','hi')], 'openmfc_ddx::MinMax(pDX, v, lo, hi);'); covered+=1; continue
            if re.match(r'^_J11$', body_enc):
                add(s, [('void*','pDX'),('long long','v'),('long long','lo'),('long long','hi')], 'openmfc_ddx::MinMax(pDX, v, lo, hi);'); covered+=1; continue
            if re.match(r'^_K11$', body_enc):
                add(s, [('void*','pDX'),('unsigned long long','v'),('unsigned long long','lo'),('unsigned long long','hi')], 'openmfc_ddx::MinMax(pDX, v, lo, hi);'); covered+=1; continue
            if body_enc == 'AEBNNN':   # double, min, max (value by const-ref)
                add(s, [('void*','pDX'),('const double*','v'),('double','lo'),('double','hi')], 'openmfc_ddx::MinMax(pDX, *v, lo, hi);'); covered+=1; continue
            if body_enc == 'AEBMMM':
                add(s, [('void*','pDX'),('const float*','v'),('float','lo'),('float','hi')], 'openmfc_ddx::MinMax(pDX, *v, lo, hi);'); covered+=1; continue
            continue   # date variants -> skip
        if fn == 'DDV_MaxChars':
            add(s, [('void*','pDX'),('const void*','pv'),('int','n')],
                'openmfc_ddx::MaxChars(pDX, reinterpret_cast<const CString*>(pv), n);'); covered+=1; continue
        if fn == 'DDV_MinMaxSlider':
            add(s, [('void*','pDX'),('unsigned long','v'),('unsigned long','lo'),('unsigned long','hi')], 'openmfc_ddx::MinMax(pDX, v, lo, hi);'); covered+=1; continue

        body = valcode_after_idc(suf)
        # ---- DDX_ control families ----
        if fn == 'DDX_Text':
            if body == 'PEA_WH':   # LPTSTR buffer + nMaxChars
                add(s, [('void*','pDX'),('int','nIDC'),('wchar_t*','pv'),('int','n')],
                    'openmfc_ddx::TextBuf(pDX, nIDC, pv, n);'); covered+=1; continue
            pv = parse_value(body)
            if pv and pv[0] == 'num':
                add(s, [('void*','pDX'),('int','nIDC'),(pv[1]+'*','pv')], 'openmfc_ddx::TextNum(pDX, nIDC, pv, %s);'%pv[2]); covered+=1
            elif pv and pv[0] == 'str':
                add(s, [('void*','pDX'),('int','nIDC'),('void*','pv')], 'openmfc_ddx::TextStr(pDX, nIDC, reinterpret_cast<CString*>(pv));'); covered+=1
            continue
        # ---- DDX_Field* : same control I/O as DDX, plus a trailing CRecordset* (the
        #      field-null bookkeeping lives in CRecordset; the observable control<->member
        #      move is identical, so we perform it and ignore the recordset here). ----
        if fn.startswith('DDX_Field') and body.endswith('PEAVCRecordset@@'):
            vb = body[:-len('PEAVCRecordset@@')]
            base = fn[len('DDX_Field'):]   # Text / Check / Radio / Scroll / CBIndex / ...
            RS = ('void*', 'pRs')
            if base == 'Text':
                if vb == 'PEA_WH':
                    add(s, [('void*','pDX'),('int','nIDC'),('wchar_t*','pv'),('int','n'),RS],
                        '(void)pRs; openmfc_ddx::TextBuf(pDX, nIDC, pv, n);'); covered+=1; continue
                pv = parse_value(vb)
                if pv and pv[0]=='num':
                    add(s, [('void*','pDX'),('int','nIDC'),(pv[1]+'*','pv'),RS], '(void)pRs; openmfc_ddx::TextNum(pDX, nIDC, pv, %s);'%pv[2]); covered+=1
                elif pv and pv[0]=='str':
                    add(s, [('void*','pDX'),('int','nIDC'),('void*','pv'),RS], '(void)pRs; openmfc_ddx::TextStr(pDX, nIDC, reinterpret_cast<CString*>(pv));'); covered+=1
                continue
            if base == 'Check':
                add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),RS], '(void)pRs; openmfc_ddx::Check(pDX, nIDC, pv);'); covered+=1; continue
            if base == 'Radio':
                add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),RS], '(void)pRs; openmfc_ddx::Radio(pDX, nIDC, pv);'); covered+=1; continue
            if base == 'Scroll':
                add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),RS], '(void)pRs; openmfc_ddx::ScrollPos(pDX, nIDC, pv);'); covered+=1; continue
            if base == 'CBIndex':
                add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),RS], '(void)pRs; openmfc_ddx::CBIndex(pDX, nIDC, pv);'); covered+=1; continue
            if base == 'LBIndex':
                add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),RS], '(void)pRs; openmfc_ddx::LBIndex(pDX, nIDC, pv);'); covered+=1; continue
            if base in ('CBString','CBStringExact'):
                ex='true' if base.endswith('Exact') else 'false'
                add(s, [('void*','pDX'),('int','nIDC'),('void*','pv'),RS], '(void)pRs; openmfc_ddx::CBString(pDX, nIDC, reinterpret_cast<CString*>(pv), %s);'%ex); covered+=1; continue
            if base in ('LBString','LBStringExact'):
                ex='true' if base.endswith('Exact') else 'false'
                add(s, [('void*','pDX'),('int','nIDC'),('void*','pv'),RS], '(void)pRs; openmfc_ddx::LBString(pDX, nIDC, reinterpret_cast<CString*>(pv), %s);'%ex); covered+=1; continue
            continue
        if fn in ('DDX_Check',):
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv')], 'openmfc_ddx::Check(pDX, nIDC, pv);'); covered+=1; continue
        if fn in ('DDX_Radio',):
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv')], 'openmfc_ddx::Radio(pDX, nIDC, pv);'); covered+=1; continue
        if fn in ('DDX_Scroll',):
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv')], 'openmfc_ddx::ScrollPos(pDX, nIDC, pv);'); covered+=1; continue
        if fn in ('DDX_Slider',):
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv')], 'openmfc_ddx::SliderPos(pDX, nIDC, pv);'); covered+=1; continue
        if fn == 'DDX_CBIndex':
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv')], 'openmfc_ddx::CBIndex(pDX, nIDC, pv);'); covered+=1; continue
        if fn == 'DDX_LBIndex':
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv')], 'openmfc_ddx::LBIndex(pDX, nIDC, pv);'); covered+=1; continue
        if fn in ('DDX_CBString','DDX_CBStringExact'):
            ex = 'true' if fn.endswith('Exact') else 'false'
            add(s, [('void*','pDX'),('int','nIDC'),('void*','pv')], 'openmfc_ddx::CBString(pDX, nIDC, reinterpret_cast<CString*>(pv), %s);'%ex); covered+=1; continue
        if fn in ('DDX_LBString','DDX_LBStringExact'):
            ex = 'true' if fn.endswith('Exact') else 'false'
            add(s, [('void*','pDX'),('int','nIDC'),('void*','pv')], 'openmfc_ddx::LBString(pDX, nIDC, reinterpret_cast<CString*>(pv), %s);'%ex); covered+=1; continue
        if fn == 'DDX_IPAddress':
            add(s, [('void*','pDX'),('int','nIDC'),('unsigned long*','pv')], 'openmfc_ddx::IPAddress(pDX, nIDC, pv);'); covered+=1; continue
        if fn == 'DDX_Control':
            add(s, [('void*','pDX'),('int','nIDC'),('void*','pv')], 'openmfc_ddx::Ctrl(pDX, nIDC);'); covered+=1; continue
        # ---- date/time-picker & month-cal controls (CTime / COleDateTime / FILETIME) ----
        if fn in ('DDX_DateTimeCtrl','DDX_MonthCalCtrl'):
            month = 'true' if fn == 'DDX_MonthCalCtrl' else 'false'
            kind = None
            if body == 'AEAVCTime@ATL@@': kind = 'openmfc_ddx::DK_CTIME'
            elif body == 'AEAVCOleDateTime@ATL@@': kind = 'openmfc_ddx::DK_OLEDT'
            elif body == 'AEAU_FILETIME@@': kind = 'openmfc_ddx::DK_FILETIME'
            if kind:
                add(s, [('void*','pDX'),('int','nIDC'),('void*','pv')],
                    'openmfc_ddx::DateExchange(pDX, nIDC, pv, %s, %s);' % (kind, month)); covered+=1
            continue

        # ---- DDP_ property-page families: data-move like DDX; End*/PostProcessing commit ----
        if fn == 'DDP_Text':
            # body = <value>PEB_W  (member& then LPCTSTR prop name)
            vb = body[:-len('PEB_W')] if body.endswith('PEB_W') else body
            pv = parse_value(vb)
            if pv and pv[0] == 'num':
                add(s, [('void*','pDX'),('int','nIDC'),(pv[1]+'*','pv'),('const wchar_t*','name')],
                    '(void)name; openmfc_ddx::TextNum(pDX, nIDC, pv, %s);'%pv[2]); covered+=1
            elif pv and pv[0] == 'str':
                add(s, [('void*','pDX'),('int','nIDC'),('void*','pv'),('const wchar_t*','name')],
                    '(void)name; openmfc_ddx::TextStr(pDX, nIDC, reinterpret_cast<CString*>(pv));'); covered+=1
            continue
        if fn == 'DDP_Check':
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),('const wchar_t*','name')],
                '(void)name; openmfc_ddx::Check(pDX, nIDC, pv);'); covered+=1; continue
        if fn == 'DDP_Radio':
            add(s, [('void*','pDX'),('int','nIDC'),('int*','pv'),('const wchar_t*','name')],
                '(void)name; openmfc_ddx::Radio(pDX, nIDC, pv);'); covered+=1; continue
        if fn in ('DDP_EndText','DDP_EndCheck','DDP_EndRadio'):
            # commit phase: value already moved by DDP_*; the property write is a no-op
            # without a live property set (an empty page), matching faithful behavior.
            add(s, [('void*','pDX'),('int','nIDC'),('void*','pv'),('const wchar_t*','name')],
                '(void)pDX;(void)nIDC;(void)pv;(void)name;'); covered+=1; continue
        if fn == 'DDP_PostProcessing':
            add(s, [('void*','pDX')], 'openmfc_ddx::PostProcessing(pDX);'); covered+=1; continue
    return covered, len(syms)


PREAMBLE = '''// AUTO-GENERATED by tools/harvest/gen_ddx.py -- DO NOT EDIT BY HAND.
// Real implementations of the exported DDX_/DDP_/DDV_ dialog-data-exchange helpers,
// forwarding to the Win32-control exchange core in ddx_core.h.
#include "ddx_core.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif
'''


def emit(sym, name, params, body):
    decl = ', '.join('%s %s' % (t, n) for t, n in params)
    return '// Symbol: %s\nextern "C" void MS_ABI %s(%s) {\n    %s\n}' % (sym, name, decl, body)


def main():
    covered, total = build()
    out = [PREAMBLE]
    for sym, name, params, body in SPECS:
        out.append('')
        out.append(emit(sym, name, params, body))
    OUT.write_text('\n'.join(out) + '\n', encoding='ascii')
    print("emitted %d DDX thunks (of %d family exports) -> %s" % (covered, total, OUT))


if __name__ == '__main__':
    main()
