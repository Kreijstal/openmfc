// Parity scorecard for CMFCToolBarMenuButton: grades OUR built openmfc.dll against the
// harvested real-mfc140u ground truth (cmfctoolbarmenubutton_vtable.json + _meta_probe).
// It is a PROGRESS TRACKER, not a hard gate — it prints done/total and a per-check
// [done]/[TODO], and degrades safely while the class is still a stub (it never dispatches
// through a vtable it hasn't first proven is the installed MSVC one, so it won't crash on
// the current mingw-stub object).
//
// Pure Win32 + explicit LoadLibrary (our DLL exits 53 on implicit load under wine).
//   build: x86_64-w64-mingw32-g++ -O0 -static -static-libgcc -static-libstdc++ \
//            -o menuparity.exe cmfctoolbarmenubutton_parity_probe.cpp
//   run:   cp build-phase4/openmfc.dll . && WINEDEBUG=-all wine menuparity.exe openmfc.dll
//
// GROUND TRUTH (harvested real mfc140u 14.51.36231):
//   sizeof=296, RTTI schema=0x80000001, CreateObject non-null (DECLARE_SERIAL factory),
//   vtable=65 slots, slot64=SetTearOff(UINT), slot63=IsTearOffMenu()->m_uiTearOffBarID!=0.
#include <windows.h>
#include <cstdio>
#include <cstring>

typedef void* (*Ctor0)(void*);
typedef void* (*GetRC_fn)(const void*);
typedef void* (*CreateObj_fn)(void);
typedef const void* (*VtblAddr_fn)(void);
typedef void  (*SetTearOff_fn)(void*, unsigned);
typedef int   (*IsTearOff_fn)(void*);

static int g_done=0, g_total=0;
static void grade(const char* what, int ok, const char* note){
  g_total++; if(ok) g_done++;
  printf("  [%s] %-46s %s\n", ok?"done":"TODO", what, note?note:"");
}

int main(int argc, char** argv){
  const char* dll = argc>1?argv[1]:"openmfc.dll";
  HMODULE h = LoadLibraryA(dll);
  if(!h){ printf("LoadLibrary %s failed %lu\n", dll, GetLastError()); return 2; }

  printf("== CMFCToolBarMenuButton parity vs real mfc140u (harvested golden) ==\n");
  printf("-- Group A: class metadata --\n");

  Ctor0 c0 = (Ctor0)GetProcAddress(h, "??0CMFCToolBarMenuButton@@QEAA@XZ");
  grade("ctor ??0...@XZ exported", c0!=0, 0);

  GetRC_fn getrc = (GetRC_fn)GetProcAddress(h, "?GetRuntimeClass@CMFCToolBarMenuButton@@UEBAPEAUCRuntimeClass@@XZ");
  int rc_obj=-1; unsigned rc_schema=0;
  if(getrc){ void* rc=getrc(0); if(rc){ rc_obj=*(int*)((char*)rc+8); rc_schema=*(unsigned*)((char*)rc+12); } }
  char nb[64];
  snprintf(nb,sizeof nb,"got objsize=%d (exp 296)", rc_obj);
  grade("RTTI objsize == 296", rc_obj==296, nb);
  snprintf(nb,sizeof nb,"got schema=0x%X (exp 0x80000001)", rc_schema);
  grade("RTTI schema == 0x80000001", rc_schema==0x80000001, nb);

  CreateObj_fn co = (CreateObj_fn)GetProcAddress(h, "?CreateObject@CMFCToolBarMenuButton@@SAPEAVCObject@@XZ");
  void* created = co? co() : 0;
  grade("CreateObject exported + non-null (DECLARE_SERIAL)", created!=0, co? (created?"":"returns null (stub)") : "not exported");

  printf("-- Group B: MSVC 65-slot vtable (increment-3) --\n");
  VtblAddr_fn vaddr = (VtblAddr_fn)GetProcAddress(h, "OpenMFC_ToolBarMenuButtonVtableAddr");
  grade("OpenMFC_ToolBarMenuButtonVtableAddr exported", vaddr!=0, vaddr?"":"vtable not installed yet");

  if(vaddr && c0){
    const void* expVtbl = vaddr();
    char buf[512]; memset(buf,0,sizeof buf); c0(buf);
    void** vptr = *(void***)buf;
    grade("ctor installs MSVC vtable (vptr==addr)", (const void*)vptr==expVtbl, 0);
    // Only dispatch through the vtable once we've confirmed it's the installed MSVC one.
    if((const void*)vptr==expVtbl){
      IsTearOff_fn isto = (IsTearOff_fn)vptr[63];   // slot 63 = IsTearOffMenu (declaration-order)
      SetTearOff_fn sto = (SetTearOff_fn)vptr[64];  // slot 64 = SetTearOff (harvested)
      int before = isto(buf);
      sto(buf, 5);
      int after = isto(buf);
      grade("slot64 SetTearOff -> slot63 IsTearOffMenu (0 then 1)", before==0 && after!=0, 0);
    } else {
      grade("slot64 SetTearOff -> slot63 IsTearOffMenu (0 then 1)", 0, "skipped: vtable not installed");
    }
  } else {
    grade("ctor installs MSVC vtable (vptr==addr)", 0, "skipped: no vtable addr export");
    grade("slot64 SetTearOff -> slot63 IsTearOffMenu (0 then 1)", 0, "skipped: no vtable addr export");
  }

  printf("\nPARITY: %d/%d checks (%.0f%%)\n", g_done, g_total, 100.0*g_done/g_total);
  return 0;   // informational progress tracker; never fails the build
}
