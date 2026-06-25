// Harvest CMFCToolBarEditBoxButton RTTI metadata (schema, objsize, CreateObject) from real
// mfc140u. Anchors the parity golden. No pure-headless behavior method exists on this class
// (its public virtuals all touch a real CEdit/CWnd), so metadata only. Build/run via
// build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxtoolbareditboxbutton.h>
#include <cstdio>
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  CRuntimeClass* rc = RUNTIME_CLASS(CMFCToolBarEditBoxButton);
  printf("name=%s objsize=%d schema=0x%X createobj_nonnull=%d\n",
    rc->m_lpszClassName, rc->m_nObjectSize, rc->m_wSchema, rc->m_pfnCreateObject!=NULL);
  return 0;
}
