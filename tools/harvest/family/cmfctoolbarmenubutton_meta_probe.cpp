// Harvest CMFCToolBarMenuButton RTTI metadata (schema, objsize, CreateObject) from real
// mfc140u. Anchors the parity golden. No purely-headless behavioral virtual is available
// on this class (CreateMenu/OpenPopupMenu/OnDraw all need a GUI), so this is metadata only.
// Build/run via build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  CRuntimeClass* rc = RUNTIME_CLASS(CMFCToolBarMenuButton);
  printf("name=%s objsize=%d schema=0x%X createobj_nonnull=%d\n",
    rc->m_lpszClassName, rc->m_nObjectSize, rc->m_wSchema, rc->m_pfnCreateObject!=NULL);
  return 0;
}
