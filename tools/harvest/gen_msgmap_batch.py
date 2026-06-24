#!/usr/bin/env python3
"""Generate one message-map batch (phase4/src/global_<slug>_msgmap.cpp + test) from
tools/harvest/msgmap_bases.json. Each stub class C gets a static AFX_MSGMAP whose
pfnGetBaseMap delegates to the base's exported GetThisMessageMap (real-MFC _AFXDLL
mechanism), and an empty terminator entry array (handler entries are real mfc140u
code addresses we cannot replicate). GetMessageMap (virtual-const) and
GetThisMessageMap (static) both return the map. // Symbol: markers auto-exclude the
weak stubs.  Usage: gen_msgmap_batch.py <slug> <Class1> <Class2> ...
"""
import json, re, sys, os
ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

def impl(sym):
    return re.sub(r'[^0-9A-Za-z_]', '_', 'impl_' + re.sub(r'[?@]', '_', sym))
def gm_sym(c):  return f'?GetMessageMap@{c}@@MEBAPEBUAFX_MSGMAP@@XZ'
def gtm_sym(c): return f'?GetThisMessageMap@{c}@@KAPEBUAFX_MSGMAP@@XZ'

def main():
    if len(sys.argv) < 3:
        sys.exit('usage: gen_msgmap_batch.py <slug> <Class>...')
    slug = sys.argv[1]
    classes = sys.argv[2:]
    bases = json.load(open(f'{ROOT}/tools/harvest/msgmap_bases.json'))
    inset = set(classes)
    # distinct out-of-batch bases that export GetThisMessageMap (delegation targets)
    ext_bases = sorted({bases[c] for c in classes
                        if bases[c] not in ('(null)', '??') and bases[c] not in inset})

    L = []
    L.append(f'// OpenMFC: message-map metadata (GetMessageMap / GetThisMessageMap) for')
    L.append(f'// {len(classes)} classes. Each gets a base-chained AFX_MSGMAP with an empty')
    L.append(f'// terminator entry array (real handler entries are mfc140u code addresses we')
    L.append(f'// cannot replicate). pfnGetBaseMap delegates to the base\'s exported')
    L.append(f'// GetThisMessageMap (real-MFC _AFXDLL base resolution); base harvested by')
    L.append(f'// pointer identity from mfc140u.dll. GetMessageMap is virtual-const (takes a')
    L.append(f'// this ptr), GetThisMessageMap is static; both return the static map. The')
    L.append(f'// // Symbol: markers drive build_phase4.sh\'s weak-stub auto-exclude.')
    L.append(f'// Named global_*_msgmap.cpp so the shard glob compiles it.')
    L.append('')
    L.append('#include "openmfc/afxwin.h"   // AFX_MSGMAP, AFX_MSGMAP_ENTRY, AfxSig_end, AFXAPI')
    L.append('')
    L.append('#ifdef __GNUC__')
    L.append('  #define MS_ABI __attribute__((ms_abi))')
    L.append('#else')
    L.append('  #define MS_ABI')
    L.append('#endif')
    L.append('')
    L.append('namespace {')
    L.append('// Shared empty message-map entries (terminator only): the class exposes no')
    L.append('// handlers we implement; routing falls through to the base via pfnGetBaseMap.')
    L.append('const AFX_MSGMAP_ENTRY g_emptyMsgEntries[] = { {0,0,0,0, AfxSig_end, (AFX_PMSG)0} };')
    L.append('} // namespace')
    L.append('')
    if ext_bases:
        L.append('// Base GetThisMessageMap getters (exported elsewhere in this DLL).')
        for b in ext_bases:
            L.append(f'extern "C" const AFX_MSGMAP* MS_ABI {impl(gtm_sym(b))}();')
        L.append('')
    L.append('namespace {')
    L.append('// __cdecl (AFXAPI) base-map thunks matching AFX_MSGMAP::pfnGetBaseMap exactly.')
    for c in classes:
        b = bases[c]
        if b in ('(null)', '??'):
            continue  # root: pfnGetBaseMap = nullptr (handled in DEF_MAP)
        target = f'&class{b}_msgmap' if b in inset else None
        if b in inset:
            # in-batch base: reference its map directly via a forward decl thunk
            L.append(f'const AFX_MSGMAP* AFXAPI gbm_{c}();  // defined after maps (in-batch base {b})')
        else:
            L.append(f'const AFX_MSGMAP* AFXAPI gbm_{c}() {{ return {impl(gtm_sym(b))}(); }}')
    L.append('')
    L.append('// The static maps. m_pfnGetBaseMap delegates to the base (or null at the root).')
    for c in classes:
        b = bases[c]
        gb = 'nullptr' if b in ('(null)', '??') else f'gbm_{c}'
        L.append(f'const AFX_MSGMAP class{c}_msgmap = {{ {gb}, g_emptyMsgEntries }};')
    # in-batch base thunks (defined after maps so they can take the map address)
    for c in classes:
        b = bases[c]
        if b in inset:
            L.append(f'const AFX_MSGMAP* AFXAPI gbm_{c}() {{ return &class{b}_msgmap; }}')
    L.append('} // namespace')
    L.append('')
    L.append('#define DEF_MM(Cls, GMSym, GTMSym) \\')
    L.append('    extern "C" const AFX_MSGMAP* MS_ABI GMSym(const void*) { return &class##Cls##_msgmap; } \\')
    L.append('    extern "C" const AFX_MSGMAP* MS_ABI GTMSym() { return &class##Cls##_msgmap; }')
    L.append('')
    for c in classes:
        L.append(f'// Symbol: {gm_sym(c)}')
        L.append(f'// Symbol: {gtm_sym(c)}')
        L.append(f'DEF_MM({c},')
        L.append(f'       {impl(gm_sym(c))},')
        L.append(f'       {impl(gtm_sym(c))})')
    L.append('#undef DEF_MM')
    L.append('')
    cpp_path = f'{ROOT}/phase4/src/global_{slug}_msgmap.cpp'
    open(cpp_path, 'w', encoding='utf-8').write('\n'.join(L))

    # ---- test: provide stub defs for out-of-batch base getters, assert structure ----
    T = []
    T.append(f'// Logic test for global_{slug}_msgmap.cpp (compiles+links; runs on Windows CI).')
    T.append(f'#include "../phase4/src/global_{slug}_msgmap.cpp"')
    T.append('#include <cstdio>')
    T.append('')
    T.append('// Out-of-batch base GetThisMessageMap getters: define sentinels so the')
    T.append('// standalone TU links (in the DLL these resolve to the real exports).')
    for b in ext_bases:
        T.append(f'static const AFX_MSGMAP s_{b}_map = {{ nullptr, nullptr }};')
        T.append(f'extern "C" const AFX_MSGMAP* MS_ABI {impl(gtm_sym(b))}() {{ return &s_{b}_map; }}')
    T.append('')
    T.append('// Framework CRuntimeClass statics dragged in by afxwin.h\'s class graph (not')
    T.append('// CObject, defined inline). AUTO-RESOLVED region (fix_test_bases.sh):')
    T.append('// >>> BASE-STATICS')
    for b in ['CCmdTarget', 'CWinThread', 'CWinApp', 'CWnd', 'CDialog']:
        T.append(f'CRuntimeClass {b}::class{b}{{}};')
    T.append('// <<< BASE-STATICS')
    T.append('')
    T.append('static int failures = 0;')
    T.append('static void check(bool ok, const char* m){ if(!ok){ printf("FAIL: %s\\n", m); failures++; } }')
    T.append('')
    T.append('int main(){')
    for c in classes:
        b = bases[c]
        T.append(f'    {{')
        T.append(f'        const AFX_MSGMAP* gm = {impl(gm_sym(c))}(nullptr);')
        T.append(f'        const AFX_MSGMAP* gt = {impl(gtm_sym(c))}();')
        T.append(f'        check(gm != nullptr, "{c} GetMessageMap non-null");')
        T.append(f'        check(gm == gt, "{c} GetMessageMap==GetThisMessageMap");')
        T.append(f'        check(gm && gm->lpEntries != nullptr, "{c} lpEntries non-null");')
        if b in ('(null)', '??'):
            T.append(f'        check(gm && gm->pfnGetBaseMap == nullptr, "{c} root has null base");')
        else:
            T.append(f'        check(gm && gm->pfnGetBaseMap != nullptr, "{c} base chain set");')
            # Verify pfnGetBaseMap() resolves to the actual base map, not just non-null.
            base_map = f'&class{b}_msgmap' if b in inset else f'&s_{b}_map'
            T.append(f'        check(gm && gm->pfnGetBaseMap && gm->pfnGetBaseMap() == {base_map},')
            T.append(f'              "{c} pfnGetBaseMap resolves to {b} map");')
        T.append(f'    }}')
    T.append('    printf("%s: %d checks failed\\n", failures?"FAILED":"OK", failures);')
    T.append('    return failures ? 1 : 0;')
    T.append('}')
    test_path = f'{ROOT}/tests/test_global_{slug}_msgmap_logic.cpp'
    open(test_path, 'w', encoding='utf-8').write('\n'.join(T) + '\n')
    print(f'wrote {cpp_path}')
    print(f'wrote {test_path}')

if __name__ == '__main__':
    main()
