#!/usr/bin/env python3
"""Generate one RTTI-getter batch (phase4/src/global_<cat>_rtti.cpp + test) from
harvested real values. Replicates the established pattern (anonymous-namespace
file-internal CRuntimeClass descriptors, parents-before-children, MS_ABI getters
returning the static descriptor directly, // Symbol: markers for auto-exclude).

Usage:
  gen_rtti_batch.py <cat> <header_includes_csv> <Class1> <Class2> ...
    <cat>     : file slug, e.g. mfcbutton  -> global_mfcbutton_rtti.cpp
    <header_includes_csv> : e.g. "openmfc/afxmfc.h" (comma-separated)
Reads tools/harvest/rtti_layouts.json and mfc_complete_ordinal_mapping.json.
Bases that are in the batch set -> chained in-file (&class<Base>), parents emitted
first; out-of-set bases -> &<Base>::class<Base> (must be a referenceable member
static, the build is the gate). Emits to stdout the .cpp; writes test to tests/.
"""
import json, re, sys, os

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def impl(sym):
    n = 'impl_' + re.sub(r'[?@]', '_', sym)
    return re.sub(r'[^0-9A-Za-z_]', '_', n)

def gt_sym(c):  return f'?GetThisClass@{c}@@SAPEAUCRuntimeClass@@XZ'
def grc_sym(c): return f'?GetRuntimeClass@{c}@@UEBAPEAUCRuntimeClass@@XZ'

def topo(classes, base_of):
    """order so a class's in-set base precedes it"""
    inset = set(classes)
    out, seen = [], set()
    def visit(c):
        if c in seen: return
        b = base_of[c]
        if b in inset and b not in seen: visit(b)
        seen.add(c); out.append(c)
    for c in classes: visit(c)
    return out

def main():
    cat = sys.argv[1]
    includes = [h for h in sys.argv[2].split(',') if h]
    classes = sys.argv[3:]
    H = json.load(open(os.path.join(ROOT, 'tools/harvest/rtti_layouts.json')))
    inset = set(classes)
    base_of = {c: H[c]['base'] for c in classes}
    ordered = topo(classes, base_of)

    def schema_lit(s):
        return '0xFFFF' if s == 0xFFFF else f'0x{s:08X}'
    def base_ref(c):
        b = H[c]['base']
        return f'&class{b}' if b in inset else f'&{b}::class{b}'

    P = cat.upper().replace('-', '_')
    L = []
    L.append(f'// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for {len(classes)} classes.')
    L.append('// File-internal CRuntimeClass descriptors; real m_nObjectSize / m_wSchema harvested')
    L.append('// from mfc140u.dll (tools/harvest). GetRuntimeClass returns the static descriptor')
    L.append('// directly (no self-dispatch). // Symbol: markers auto-exclude the weak stubs.')
    L.append('// Named global_*_rtti.cpp so build_phase4.sh\'s shard glob compiles it.')
    L.append('')
    for h in includes:
        L.append(f'#include "{h}"')
    L.append('')
    L.append('#ifdef __GNUC__')
    L.append('  #define MS_ABI __attribute__((ms_abi))')
    L.append('#else')
    L.append('  #define MS_ABI')
    L.append('#endif')
    L.append('')
    L.append('namespace {')
    L.append('// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,')
    L.append('// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.')
    L.append(f'#define {P}_DESC(Cls, Size, Schema, BaseDesc) \\')
    L.append('    CRuntimeClass class##Cls = { #Cls, (Size), (Schema), nullptr, nullptr, (BaseDesc), nullptr }')
    for c in ordered:
        L.append(f'{P}_DESC({c}, {H[c]["sizeof"]}, {schema_lit(H[c]["schema"])}, {base_ref(c)});')
    L.append(f'#undef {P}_DESC')
    L.append('} // namespace')
    L.append('')
    L.append(f'#define {P}_GETTERS(Cls, GetThisSym, GetRtSym) \\')
    L.append('    extern "C" CRuntimeClass* MS_ABI GetThisSym() { return &class##Cls; } \\')
    L.append('    extern "C" CRuntimeClass* MS_ABI GetRtSym(const void*) { return &class##Cls; }')
    L.append('')
    for c in ordered:
        L.append(f'// Symbol: {gt_sym(c)}')
        L.append(f'// Symbol: {grc_sym(c)}')
        L.append(f'{P}_GETTERS({c},')
        L.append(f'           {impl(gt_sym(c))},')
        L.append(f'           {impl(grc_sym(c))})')
    L.append(f'#undef {P}_GETTERS')
    L.append('')
    cpp = '\n'.join(L)
    cpp_path = os.path.join(ROOT, f'phase4/src/global_{cat}_rtti.cpp')
    open(cpp_path, 'w').write(cpp)

    # ---- test ----
    T = []
    T.append(f'// Logic test for global_{cat}_rtti.cpp (run under wine/host).')
    T.append(f'#include "../phase4/src/global_{cat}_rtti.cpp"')
    T.append('#include <cstdio>')
    T.append('#include <cstring>')
    T.append('')
    # Framework base statics dragged in by the header class-graph at link time
    # (CObject::classCObject is defined inline by afx.h, so do NOT redeclare it).
    # STD covers the common chain; direct out-of-set bases are added. Any others
    # surfaced by the linker are appended by fix_test_bases.sh between the markers.
    STD = ['CCmdTarget', 'CWinThread', 'CWinApp', 'CWnd', 'CDialog']
    outbases = sorted({H[c]['base'] for c in classes if H[c]['base'] not in inset})
    decls = [b for b in dict.fromkeys(STD + outbases) if b != 'CObject']
    T.append('// out-of-set / framework base descriptors, defined here only so the')
    T.append('// standalone test TU links. AUTO-RESOLVED region (fix_test_bases.sh):')
    T.append('// >>> BASE-STATICS')
    for b in decls:
        T.append(f'CRuntimeClass {b}::class{b}{{}};')
    T.append('// <<< BASE-STATICS')
    T.append('')
    T.append('static int failures = 0;')
    T.append('static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\\n", m); failures++; } }')
    T.append('')
    T.append('struct Case { CRuntimeClass* (MS_ABI *gt)(); CRuntimeClass* (MS_ABI *grc)(const void*);')
    T.append('              const char* name; int size; unsigned schema; CRuntimeClass* base; };')
    T.append('')
    T.append('int main(){')
    T.append('    Case cases[] = {')
    for c in ordered:
        T.append(f'        {{ {impl(gt_sym(c))}, {impl(grc_sym(c))}, "{c}", '
                 f'{H[c]["sizeof"]}, {schema_lit(H[c]["schema"])}, {base_ref(c)} }},')
    T.append('    };')
    T.append('    for (const Case& c : cases) {')
    T.append('        CRuntimeClass* r = c.gt();')
    T.append('        check(r != nullptr, c.name);')
    T.append('        check(r && strcmp(r->m_lpszClassName, c.name)==0, c.name);')
    T.append('        check(r && r->m_nObjectSize == c.size, c.name);')
    T.append('        check(r && r->m_wSchema == c.schema, c.name);')
    T.append('        check(r && r->m_pfnCreateObject == nullptr, c.name);')
    T.append('        check(r && r->m_pfnGetBaseClass == nullptr, c.name);')
    T.append('        check(c.grc(nullptr) == r, c.name);   // no self-dispatch')
    T.append('        check(r && r->m_pBaseClass == c.base, c.name);')
    T.append('    }')
    T.append('    printf("%s: %d checks failed\\n", failures?"FAILED":"OK", failures);')
    T.append('    return failures ? 1 : 0;')
    T.append('}')
    test_path = os.path.join(ROOT, f'tests/test_global_{cat}_rtti_logic.cpp')
    open(test_path, 'w').write('\n'.join(T) + '\n')
    print(f'wrote {cpp_path}')
    print(f'wrote {test_path}')

if __name__ == '__main__':
    main()
