// On-host drop-in verifier for CMFCToolBarButton's exported constructors. This is a
// PURE WIN32 probe (no MFC headers/link): it LoadLibrary's our built openmfc.dll
// explicitly and calls the exported ctors on a raw buffer, then reads fields at the
// REAL mfc140u offsets. Explicit LoadLibrary is required because our DLL exits 53 on
// *implicit* load under wine (CRT/MFC startup gap) — same approach as rtti_probe.
// NOTE: do NOT build this through build_family_probe.sh (that one links mfc140u for
// the MFC-using golden/layout probes). Build & run standalone, e.g.:
//   x86_64-w64-mingw32-g++ -O0 -static -static-libgcc -static-libstdc++ \
//       -o ctorprobe.exe cmfctoolbarbutton_ctor_probe.cpp
//   cp build-phase4/openmfc.dll . && WINEDEBUG=-all wine ctorprobe.exe openmfc.dll
// Expected output == real mfc140u golden (cmfctoolbarbutton_golden.json): default
// iImage=-1 iUserImage=-1; user(123,5,"Hello",T) iUserImage=5; nonuser(77,3,"Bee",F,locked) iImage=3.
#include <windows.h>
#include <cstdio>
#include <cstring>
// Field offsets (harvested from real mfc140u).
#define O_bUserButton 8
#define O_bImage 16
#define O_bWholeText 24
#define O_nID 36
#define O_nStyle 40
#define O_strText 56
#define O_iImage 72
#define O_iUserImage 76
#define O_bLocked 80
#define O_bHorz 96
#define O_bVisible 100
typedef void* (*Ctor0)(void*);
typedef void* (*CtorP)(void*, unsigned, int, const wchar_t*, int, int);
static int I(char*b,int o){ return *(int*)(b+o); }
static void dump(const char* tag, char* b){
  int userBtn=I(b,O_bUserButton);
  int img = userBtn? I(b,O_iUserImage): I(b,O_iImage);
  const wchar_t* s = *(const wchar_t**)(b+O_strText);
  printf("%s|nID=%d|nStyle=%d|GetImage=%d|iImage=%d|iUserImage=%d|userBtn=%d|locked=%d|bImage=%d|bWholeText=%d|horz=%d|visible=%d|strText=%ls\n",
    tag, I(b,O_nID),I(b,O_nStyle),img,I(b,O_iImage),I(b,O_iUserImage),userBtn,I(b,O_bLocked),
    I(b,O_bImage),I(b,O_bWholeText),I(b,O_bHorz),I(b,O_bVisible), s?s:L"<null>");
}
int main(int argc, char** argv){
  const char* dll = argc>1?argv[1]:"openmfc.dll";
  HMODULE h = LoadLibraryA(dll);
  if(!h){ printf("LoadLibrary %s failed %lu\n", dll, GetLastError()); return 2; }
  Ctor0 c0 = (Ctor0)GetProcAddress(h, "??0CMFCToolBarButton@@QEAA@XZ");
  CtorP cp = (CtorP)GetProcAddress(h, "??0CMFCToolBarButton@@QEAA@IHPEB_WHH@Z");
  printf("c0=%p cp=%p\n", (void*)c0, (void*)cp);
  if(!c0||!cp){ printf("missing ctor export\n"); return 3; }
  char buf[160];
  memset(buf,0xCC,sizeof(buf)); c0(buf);                         dump("default", buf);
  memset(buf,0xCC,sizeof(buf)); cp(buf,123,5,L"Hello",1,0);      dump("user", buf);
  memset(buf,0xCC,sizeof(buf)); cp(buf,77,3,L"Bee",0,1);         dump("nonuser", buf);
  return 0;
}
