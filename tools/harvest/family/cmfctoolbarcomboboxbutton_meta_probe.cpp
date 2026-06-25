// Harvest CMFCToolBarComboBoxButton RTTI metadata (schema, objsize, CreateObject) +
// the headlessly-checkable Compare() ordering, from real mfc140u. Anchors the parity
// golden. Build/run via build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  CRuntimeClass* rc = RUNTIME_CLASS(CMFCToolBarComboBoxButton);
  printf("name=%s objsize=%d schema=0x%X createobj_nonnull=%d\n",
    rc->m_lpszClassName, rc->m_nObjectSize, rc->m_wSchema, rc->m_pfnCreateObject!=NULL);
  // Compare() is a pure string comparison (no GUI) -> headlessly verifiable
  CMFCToolBarComboBoxButton b;
  printf("Compare(a,b)=%d Compare(b,a)=%d Compare(a,a)=%d\n",
    b.Compare(L"a",L"b"), b.Compare(L"b",L"a"), b.Compare(L"a",L"a"));
  return 0;
}
