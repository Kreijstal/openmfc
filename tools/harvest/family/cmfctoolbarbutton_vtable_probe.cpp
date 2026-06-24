// Harvest the real mfc140u CMFCToolBarButton vtable SLOT INDEX of each exported
// virtual method. Construct a real button (its vptr is mfc140u's own vtable), then for
// each exported virtual resolve its code address via GetProcAddress BY ORDINAL (mfc140u
// exports are ordinal-only — no name table, so by-name returns NULL) and find the slot
// holding that exact address. dups==1 means an unambiguous, non-ICF-folded match.
// Result (cmfctoolbarbutton_vtable.json) cross-checks: every exported slot lands exactly
// where SDK-header declaration order predicts (CObject overrides ~dtor/Serialize/
// AssertValid/Dump fold into base slots 1-4; new virtuals start at slot 5). Build/run
// via tools/harvest/family/build_family_probe.sh. cmfctoolbarbutton_virt_ords.h (the
// symbol->ordinal table) is generated from mfc_complete_ordinal_mapping.json.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
#include <cstdint>
#include "cmfctoolbarbutton_virt_ords.h"
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))return 1;
  HMODULE m = GetModuleHandleW(L"mfc140u.dll");
  CMFCToolBarButton btn;
  uintptr_t* vptr = *(uintptr_t**)&btn;
  const int N = 56;
  for(size_t k=0;k<sizeof(VIRT)/sizeof(VIRT[0]);k++){
    uintptr_t a = (uintptr_t)GetProcAddress(m, MAKEINTRESOURCEA(VIRT[k].ord));
    int found=-1,dups=0;
    for(int i=0;i<N;i++) if(vptr[i]==a){ if(found<0)found=i; dups++; }
    printf("slot %2d  dups=%d  ord=%d  %s  addr=%llx\n", found, dups, VIRT[k].ord, VIRT[k].n, (unsigned long long)a);
  }
  return 0;
}
