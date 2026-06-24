// Verify message-map exports on a built DLL: each class's GetMessageMap /
// GetThisMessageMap return the same non-null AFX_MSGMAP with non-null entries and a
// pfnGetBaseMap that resolves to the base's map. Usage: msgmap_probe <our.dll> [filter-substr]
#include <windows.h>
#include <cstdio>
#include <cstring>
struct ENTRY{unsigned a,b,c,d; unsigned long long e; void* f;};
struct MAP{ const MAP*(__cdecl *pfnGetBaseMap)(); const ENTRY* lpEntries; };
typedef const MAP*(*FS)();            // GetThisMessageMap (static)
typedef const MAP*(*FV)(const void*); // GetMessageMap (virtual, takes this)
#include "msgmap_probe_targets.h"
int main(int argc,char**argv){
  HMODULE h=LoadLibraryA(argv[1]);
  const char* filt = argc>2?argv[2]:0;
  int fail=0,checked=0;
  for(int i=0;i<g_nmt;i++){
    const MT& t=g_mt[i];
    if(filt && !strstr(t.name,filt)) continue;
    checked++;
    FS gtm=(FS)GetProcAddress(h,MAKEINTRESOURCEA(t.gtm));
    FV gm =(FV)GetProcAddress(h,MAKEINTRESOURCEA(t.gm));
    const MAP* pgtm = gtm?gtm():0;
    char dummy[64]={0};
    const MAP* pgm = gm?gm(dummy):0;
    int ok=1; const char* why="";
    if(!pgtm){ ok=0; why="GetThisMessageMap null (stub)"; }
    else if(pgm!=pgtm){ ok=0; why="GetMessageMap != GetThisMessageMap"; }
    else if(!pgtm->lpEntries){ ok=0; why="null lpEntries"; }
    else {
      const MAP* base = pgtm->pfnGetBaseMap ? pgtm->pfnGetBaseMap() : 0;
      if(t.base_gtm==0){ if(base){ ok=0; why="root expected null base"; } }
      else {
        FS bg=(FS)GetProcAddress(h,MAKEINTRESOURCEA(t.base_gtm));
        const MAP* expect = bg?bg():0;
        if(base!=expect){ ok=0; why="base chain mismatch"; }
      }
    }
    if(!ok){ fail++; printf("FAIL %-30s %s (base %s)\n",t.name,why,t.base); }
  }
  printf("\n%s: checked %d, %d failures\n", fail?"FAILED":"ALL PASS", checked, fail);
  return fail?1:0;
}
