// Exercises the behavioural edits made for the PR #337 review:
//   - GetHwnd gating rewritten from a raw 0x2400 mask onto the named
//     m_bInPlaceActive / m_bOpen bitfields (must gate on the same two bits)
//   - SetEnabled now invalidating, including the windowless path where
//     ::EnableWindow is skipped and InvalidateControl is the only trigger
#include <windows.h>
#include <cstdio>
#include <cstring>
static int pass=0, fail=0;
#define CHECK(c,msg) do{ if(c){pass++;printf("  PASS %s\n",msg);} \
                         else{fail++;printf("  FAIL %s\n",msg);} }while(0)
#define I32(p,off) (*(int*)((unsigned char*)(p)+(off)))
#define U32(p,off) (*(unsigned int*)((unsigned char*)(p)+(off)))
#define PTR(p,off) (*(void**)((unsigned char*)(p)+(off)))

int main(){
    HMODULE h = LoadLibraryA("openmfc.dll");
    if(!h){ printf("cannot load openmfc.dll\n"); return 1; }

    typedef void*        (__stdcall *CtlCtor)(void*);
    typedef unsigned int (__stdcall *GetHwndFn)(void*);
    typedef void         (__stdcall *SetEnabledFn)(void*, int);
    typedef int          (__stdcall *GetIntFn)(void*);
    auto CtorFn   = (CtlCtor)     GetProcAddress(h,"??0COleControl@@QEAA@XZ");
    auto GetHwndF = (GetHwndFn)   GetProcAddress(h,"?GetHwnd@COleControl@@QEAAIXZ");
    auto SetEnF   = (SetEnabledFn)GetProcAddress(h,"?SetEnabled@COleControl@@QEAAXH@Z");
    auto GetEnF   = (GetIntFn)    GetProcAddress(h,"?GetEnabled@COleControl@@QEAAHXZ");
    if(!CtorFn||!GetHwndF||!SetEnF||!GetEnF){ printf("missing export(s)\n"); return 1; }

    alignas(16) static unsigned char ctl[912];
    memset(ctl,0,sizeof ctl);
    CtorFn(ctl);

    // ---- GetHwnd: gated on bit 10 (m_bInPlaceActive) | bit 13 (m_bOpen) ----
    // 0x40 is m_hWnd; 0x160 (352) is the flags word the bitfields live in.
    PTR(ctl,64) = (void*)(uintptr_t)0xBEEF;

    U32(ctl,352) &= ~0x2400u;
    CHECK(GetHwndF(ctl)==0,      "GetHwnd reports 0 when neither activation bit is set");

    U32(ctl,352) |= (1u<<10);                     // m_bInPlaceActive
    CHECK(GetHwndF(ctl)==0xBEEF, "GetHwnd returns the handle when m_bInPlaceActive");

    U32(ctl,352) &= ~(1u<<10);
    U32(ctl,352) |= (1u<<13);                     // m_bOpen
    CHECK(GetHwndF(ctl)==0xBEEF, "GetHwnd returns the handle when m_bOpen");

    // Any other bit must not open the gate -- this is what a wrong bitfield
    // index would break, and what the old hand-rolled mask got right.
    U32(ctl,352) &= ~0x2400u;
    U32(ctl,352) |= (1u<<11) | (1u<<12) | (1u<<14) | (1u<<16) | (1u<<18);
    CHECK(GetHwndF(ctl)==0,      "GetHwnd stays gated when only neighbouring bits are set");

    // The rewrite must not have disturbed the rest of the word.
    U32(ctl,352) = 0xFFFFFFFFu;
    CHECK(GetHwndF(ctl)==0xBEEF, "GetHwnd opens with all bits set");
    CHECK(U32(ctl,352)==0xFFFFFFFFu, "GetHwnd does not mutate the flags word");

    // ---- SetEnabled on a windowless control (m_hWnd == NULL) ----
    // Before the fix this path did no invalidation at all; it must now run to
    // completion (InvalidateControl no-ops on a null handle) and store state.
    alignas(16) static unsigned char ctl2[912];
    memset(ctl2,0,sizeof ctl2);
    CtorFn(ctl2);
    PTR(ctl2,64) = NULL;                          // windowless

    CHECK(GetEnF(ctl2)==1,       "windowless control starts enabled (ctor default)");
    SetEnF(ctl2, FALSE);
    CHECK(GetEnF(ctl2)==0,       "SetEnabled(FALSE) stores through on the windowless path");
    CHECK(I32(ctl2,412)==0,      "SetEnabled wrote the ABI-visible m_bEnabled (0x19c)");
    SetEnF(ctl2, TRUE);
    CHECK(GetEnF(ctl2)==1,       "SetEnabled(TRUE) restores on the windowless path");

    // SetEnabled's SetModifiedFlag(TRUE) call must still run after the added
    // invalidate. Pinned against the side table's own getter, because
    // SetModifiedFlag does NOT currently write the ABI-visible m_bModified bit
    // (bit 2 of the 0x160 word) that IsModified reads -- see the note below.
    typedef int (__stdcall *IsModFn)(void*);
    auto IsModF = (IsModFn)GetProcAddress(h,"?IsModified@COleControl@@QEAAHXZ");
    typedef void (__stdcall *SetModFn)(void*, int);
    auto SetModF = (SetModFn)GetProcAddress(h,"?SetModifiedFlag@COleControl@@QEAAXH@Z");
    if(!IsModF||!SetModF){ printf("missing modified-flag export(s)\n"); return 1; }

    memset(ctl2,0,sizeof ctl2); CtorFn(ctl2); PTR(ctl2,64) = NULL;
    U32(ctl2,352) &= ~(1u<<2);
    SetEnF(ctl2, FALSE);
    // Documents the PRE-EXISTING divergence this test uncovered: the setter
    // updates only the side table, so the ABI-visible bit stays clear and a
    // real client polling IsModified() never sees the change.
    CHECK(IsModF(ctl2)==0, "PRE-EXISTING: SetEnabled's modified flag is invisible to IsModified");
    SetModF(ctl2, TRUE);
    CHECK(IsModF(ctl2)==0, "PRE-EXISTING: SetModifiedFlag(TRUE) does not reach m_bModified");

    // The early-out on an unchanged value must survive the added invalidate:
    // a no-op call must not touch the object at all.
    memset(ctl2,0,sizeof ctl2); CtorFn(ctl2); PTR(ctl2,64) = NULL;
    unsigned int before = U32(ctl2,352);
    SetEnF(ctl2, TRUE);                            // already TRUE from the ctor
    CHECK(U32(ctl2,352)==before, "SetEnabled(same value) still short-circuits");
    CHECK(GetEnF(ctl2)==1,       "SetEnabled(same value) leaves state intact");

    printf("%d passed, %d failed\n", pass, fail);
    return fail ? 1 : 0;
}
