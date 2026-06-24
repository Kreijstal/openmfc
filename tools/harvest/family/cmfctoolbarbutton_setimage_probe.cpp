// Harvest the EXACT field semantics of CMFCToolBarButton::SetImage and
// ResetImageToDefault from real mfc140u (so the OpenMFC vtable slots reproduce them,
// not guess). Reads both m_iImage and m_iUserImage before/after each call, for a
// user button and a non-user button. Build/run via build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
struct ProbeAccess : public CMFCToolBarButton {
  int rawImage() const { return m_iImage; }
  int rawUserImage() const { return m_iUserImage; }
};
static void show(const char* tag, CMFCToolBarButton& b){
  const ProbeAccess& pa = static_cast<const ProbeAccess&>(b);
  printf("%-28s userBtn=%d iImage=%d iUserImage=%d GetImage=%d\n",
    tag, b.m_bUserButton, pa.rawImage(), pa.rawUserImage(), b.GetImage());
}
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  // --- user button ---
  CMFCToolBarButton u(123, 5, L"U", TRUE, FALSE);
  show("user ctor(img=5)", u);
  u.SetImage(9);                 show("user SetImage(9)", u);
  u.ResetImageToDefault();       show("user ResetImageToDefault", u);
  // --- non-user button ---
  CMFCToolBarButton n(77, 3, L"N", FALSE, FALSE);
  show("nonuser ctor(img=3)", n);
  n.SetImage(8);                 show("nonuser SetImage(8)", n);
  n.ResetImageToDefault();       show("nonuser ResetImageToDefault", n);
  return 0;
}
