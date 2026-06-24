// End-to-end verifier for OpenMFC's INSTALLED CMFCToolBarButton vtable. Pure-Win32:
// LoadLibrary our built openmfc.dll explicitly (it exits 53 on implicit load under
// wine), call the exported ctor on a raw buffer, read the vptr the ctor installed, then
// DISPATCH the behavioral slots THROUGH the vtable exactly as a real MSVC client would
// (virtual call = vptr[slot]). This exercises the real symptom (client virtual dispatch
// to MSVC slot indices), not just field layout. Slot indices per
// cmfctoolbarbutton_vtable.json. Behavior expectations per the harvested golden.
//   build: x86_64-w64-mingw32-g++ -O0 -static -static-libgcc -static-libstdc++ \
//            -o ourvtbl.exe cmfctoolbarbutton_ourvtable_probe.cpp
//   run:   cp build-phase4/openmfc.dll . && WINEDEBUG=-all wine ourvtbl.exe openmfc.dll
#include <windows.h>
#include <cstdio>
#include <cstring>

#define O_bUserButton 8
#define O_nID 36
#define O_nStyle 40
#define O_iImage 72
#define O_iUserImage 76
#define O_bExtraSize 92
struct R { long left, top, right, bottom; };

typedef void* (*Ctor0)(void*);
typedef void* (*CtorP)(void*, unsigned, int, const wchar_t*, int, int);
// slot fn-pointer types (default win64 calling conv IS ms_abi under mingw)
typedef void* (*GetRC_fn)(void*);
typedef void  (*SetImage_fn)(void*, int);
typedef void  (*Reset_fn)(void*);
typedef int   (*Compare_fn)(void*, void*);
typedef void  (*CopyFrom_fn)(void*, void*);
typedef void  (*SetStyle_fn)(void*, unsigned);
typedef R     (*GetRect_fn)(void*);
typedef int   (*IsExtra_fn)(void*);

static int  I(void* b, int o){ return *(int*)((char*)b + o); }
static int  fails = 0;
static void check(const char* what, long got, long exp){
  bool ok = (got == exp);
  if(!ok) fails++;
  printf("  [%s] %-34s got=%ld exp=%ld\n", ok?"OK":"FAIL", what, got, exp);
}

int main(int argc, char** argv){
  const char* dll = argc>1?argv[1]:"openmfc.dll";
  HMODULE h = LoadLibraryA(dll);
  if(!h){ printf("LoadLibrary %s failed %lu\n", dll, GetLastError()); return 2; }
  Ctor0 c0 = (Ctor0)GetProcAddress(h, "??0CMFCToolBarButton@@QEAA@XZ");
  CtorP cp = (CtorP)GetProcAddress(h, "??0CMFCToolBarButton@@QEAA@IHPEB_WHH@Z");
  if(!c0||!cp){ printf("missing ctor export\n"); return 3; }

  char buf[160], buf2[160];

  // ---- vptr install + all 53 slots non-null ----
  memset(buf,0xCC,sizeof(buf)); c0(buf);
  void** vptr = *(void***)buf;
  printf("vptr=%p\n", (void*)vptr);
  if(!vptr){ printf("FAIL: null vptr (ctor did not install MSVC vtable)\n"); return 4; }
  int nullslots=0; for(int i=0;i<53;i++) if(!vptr[i]) { nullslots++; printf("  slot %d NULL\n", i); }
  check("all 53 slots non-null", nullslots, 0);

  // ---- slot 0 GetRuntimeClass -> descriptor objsize 136, schema 0x80000001 ----
  void* rc = ((GetRC_fn)vptr[0])(buf);
  // CRuntimeClass: {char* name; int objsize; word schema; ...}; objsize @8, schema @12
  check("slot0 GetRuntimeClass objsize", rc? *(int*)((char*)rc+8):-1, 136);
  check("slot0 GetRuntimeClass schema",  rc? (long)*(unsigned*)((char*)rc+12):-1, (long)0x80000001);

  // ---- user button: slot 50 SetImage sets m_iUserImage; slot 38 Reset = no-op ----
  memset(buf,0xCC,sizeof(buf)); cp(buf,123,5,L"Hello",1,0);   // user button
  ((SetImage_fn)vptr[50])(buf, 9);
  check("slot50 SetImage(user) iUserImage", I(buf,O_iUserImage), 9);
  check("slot50 SetImage(user) iImage unchanged", I(buf,O_iImage), -1);
  ((Reset_fn)vptr[38])(buf);
  check("slot38 ResetImageToDefault no-op", I(buf,O_iUserImage), 9);

  // ---- non-user button: SetImage sets m_iImage ----
  memset(buf,0xCC,sizeof(buf)); cp(buf,77,3,L"Bee",0,0);      // non-user button
  ((SetImage_fn)vptr[50])(buf, 8);
  check("slot50 SetImage(nonuser) iImage", I(buf,O_iImage), 8);
  check("slot50 SetImage(nonuser) iUserImage unchanged", I(buf,O_iUserImage), -1);

  // ---- slot 37 SetStyle ----
  ((SetStyle_fn)vptr[37])(buf, 0x1234);
  check("slot37 SetStyle m_nStyle", I(buf,O_nStyle), 0x1234);

  // ---- slot 52 IsExtraSize ----
  check("slot52 IsExtraSize", ((IsExtra_fn)vptr[52])(buf), I(buf,O_bExtraSize));

  // ---- slot 7 CopyFrom + slot 39 CompareWith ----
  memset(buf,0xCC,sizeof(buf));  cp(buf,200,11,L"Src",1,0);   // src (user)
  memset(buf2,0xCC,sizeof(buf2)); c0(buf2);                   // blank dst
  ((CopyFrom_fn)vptr[7])(buf2, buf);
  check("slot7 CopyFrom nID copied", I(buf2,O_nID), 200);
  check("slot39 CompareWith equal after copy", ((Compare_fn)vptr[39])(buf2, buf), 1);
  *(int*)(buf2+O_nID) = 201;  // change dst nID
  check("slot39 CompareWith differs after nID change", ((Compare_fn)vptr[39])(buf2, buf), 0);

  // ---- slot 36 GetInvalidateRect returns m_rect (default empty) ----
  R r = ((GetRect_fn)vptr[36])(buf2);
  check("slot36 GetInvalidateRect.left", r.left, 0);
  check("slot36 GetInvalidateRect.right", r.right, 0);

  printf("%s (%d failures)\n", fails? "VTABLE PROBE FAILED" : "VTABLE PROBE PASSED", fails);
  return fails ? 1 : 0;
}
