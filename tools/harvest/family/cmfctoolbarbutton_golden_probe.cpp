#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
// m_iImage / m_iUserImage are protected; reach them via a derived accessor.
struct ProbeAccess : public CMFCToolBarButton {
  int rawImage() const { return m_iImage; }
  int rawUserImage() const { return m_iUserImage; }
};
static void dumpB(const char* tag, CMFCToolBarButton& b){
  const ProbeAccess& pa = static_cast<const ProbeAccess&>(b);
  printf("[%s] id=%u style=%u img=%d iImage=%d iUserImage=%d userbtn=%d locked=%d text=%d image=%d hidden=%d visible=%d horz=%d strText=\"%ls\" rect=(%ld,%ld,%ld,%ld)\n",
    tag, b.m_nID, b.m_nStyle, b.GetImage(), pa.rawImage(), pa.rawUserImage(), b.m_bUserButton, b.IsLocked(), b.m_bText, b.m_bImage,
    b.IsHidden(), b.IsVisible(), b.IsHorizontal(), (const wchar_t*)b.m_strText,
    b.Rect().left,b.Rect().top,b.Rect().right,b.Rect().bottom);
}
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  // schema (DECLARE_SERIAL version)
  CRuntimeClass* rc = RUNTIME_CLASS(CMFCToolBarButton);
  printf("schema=0x%X objsize=%d name=%s\n", rc->m_wSchema, rc->m_nObjectSize, rc->m_lpszClassName);
  printf("createobj_nonnull=%d\n", rc->m_pfnCreateObject!=NULL);
  // default ctor
  CMFCToolBarButton def; dumpB("default", def);
  // param ctor
  CMFCToolBarButton p(123, 5, L"Hello", TRUE, FALSE); dumpB("param", p);
  // SetImage
  p.SetImage(9); printf("after SetImage(9): GetImage=%d\n", p.GetImage());
  // CopyFrom + CompareWith
  CMFCToolBarButton c; c.CopyFrom(p); dumpB("copy", c);
  printf("CompareWith(copy,param)=%d\n", c.CompareWith(p));
  c.m_nID=999; printf("CompareWith(after id change)=%d\n", c.CompareWith(p));
  // Serialize round-trip via CMemFile/CArchive
  CMemFile mf;
  { CArchive ar(&mf, CArchive::store); p.Serialize(ar); }
  ULONGLONG len = mf.GetLength();
  printf("serialized_len=%llu\n", len);
  BYTE* buf = mf.Detach();
  printf("serialized_hex=");
  for(ULONGLONG i=0;i<len;i++) printf("%02X", buf[i]);
  printf("\n");
  // re-read
  CMemFile mf2; mf2.Attach(buf,(UINT)len);
  CMFCToolBarButton r;
  { CArchive ar(&mf2, CArchive::load); r.Serialize(ar); }
  dumpB("roundtrip", r);
  printf("RTroundtrip_compare=%d\n", r.CompareWith(p));
  return 0;
}
