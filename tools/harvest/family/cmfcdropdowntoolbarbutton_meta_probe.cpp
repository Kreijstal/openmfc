// Harvest CMFCDropDownToolbarButton RTTI metadata (schema, objsize, CreateObject) from real
// mfc140u. Anchors the parity golden. Build/run via build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxdropdowntoolbar.h>
#include <cstdio>
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  CRuntimeClass* rc = RUNTIME_CLASS(CMFCDropDownToolbarButton);
  printf("name=%s objsize=%d schema=0x%X createobj_nonnull=%d\n",
    rc->m_lpszClassName, rc->m_nObjectSize, rc->m_wSchema, rc->m_pfnCreateObject!=NULL);
  // IsDropDown() is a pure (no-GUI) attribute getter -> headlessly verifiable on a default button.
  CMFCDropDownToolbarButton b;
  printf("IsDropDown(default)=%d\n", b.IsDropDown());
  return 0;
}
