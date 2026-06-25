// Harvest the real mfc140u CMFCToolBarComboBoxButton vtable SLOT INDEX of each exported
// virtual. Same technique as cmfctoolbarbutton_vtable_probe.cpp: construct a real button
// (its vptr is mfc140u's own vtable), resolve each exported virtual's code address via
// GetProcAddress BY ORDINAL (mfc140u is ordinal-only; by-name returns NULL), and find the
// slot holding that exact address (dups==1 = unambiguous). Derived class: overrides reuse
// the base CMFCToolBarButton slots (0-52); new virtuals declared in CMFCToolBarComboBoxButton
// append after slot 52 in SDK-header (afxtoolbarcomboboxbutton.h) declaration order.
// Build/run via tools/harvest/family/build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
#include <cstdint>
#include "cmfctoolbarcomboboxbutton_virt_ords.h"
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0))return 1;
  HMODULE m = GetModuleHandleW(L"mfc140u.dll");
  CMFCToolBarComboBoxButton btn;
  uintptr_t* vptr = *(uintptr_t**)&btn;
  const int N = 62;   // exact slot count (cmfctoolbarcomboboxbutton_vtable.json); don't over-read
  for(size_t k=0;k<sizeof(VIRT)/sizeof(VIRT[0]);k++){
    uintptr_t a = (uintptr_t)GetProcAddress(m, MAKEINTRESOURCEA(VIRT[k].ord));
    int found=-1,dups=0;
    for(int i=0;i<N;i++) if(vptr[i]==a){ if(found<0)found=i; dups++; }
    printf("slot %2d  dups=%d  ord=%d  %s  addr=%llx\n", found, dups, VIRT[k].ord, VIRT[k].n, (unsigned long long)a);
  }
  return 0;
}
