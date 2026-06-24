// Harvest CMFCToolBarButton::CompareWith field sensitivity + CopyFrom copied-fields,
// from real mfc140u. CompareWith: toggle one field at a time off an otherwise-equal
// pair and see which flips the verdict. CopyFrom: copy a fully-populated src into a
// blank dst and dump dst's fields. Build/run via build_family_probe.sh.
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstdio>
struct PA : public CMFCToolBarButton { int iImg()const{return m_iImage;} int iUImg()const{return m_iUserImage;} };
static int cmp(const CMFCToolBarButton& a, const CMFCToolBarButton& b){ return a.CompareWith(b); }
int main(){
  if(!AfxWinInit(::GetModuleHandle(NULL),NULL,::GetCommandLine(),0)){printf("init fail\n");return 1;}
  #define BASE() CMFCToolBarButton(100, 7, L"Txt", FALSE, FALSE)
  { CMFCToolBarButton a=BASE(), b=BASE(); printf("equal pair                 -> %d\n", cmp(a,b)); }
  { CMFCToolBarButton a=BASE(), b=BASE(); b.m_nID=101;      printf("diff m_nID                 -> %d\n", cmp(a,b)); }
  { CMFCToolBarButton a=BASE(), b=BASE(); b.m_nStyle=4;     printf("diff m_nStyle              -> %d\n", cmp(a,b)); }
  { CMFCToolBarButton a=BASE(), b=BASE(); b.SetImage(9);    printf("diff image                 -> %d\n", cmp(a,b)); }
  { CMFCToolBarButton a=BASE(), b=BASE(); b.m_strText=L"X"; printf("diff m_strText             -> %d\n", cmp(a,b)); }
  { CMFCToolBarButton a=BASE(), b=BASE(); b.m_bText=TRUE;   printf("diff m_bText               -> %d\n", cmp(a,b)); }
  { CMFCToolBarButton a=BASE(), b=BASE(); b.m_bImage=FALSE; printf("diff m_bImage              -> %d\n", cmp(a,b)); }
  // CopyFrom: populate src, copy into blank dst, dump key fields
  CMFCToolBarButton src(200, 11, L"Hello", TRUE, FALSE);
  src.m_nStyle=6; src.m_bText=TRUE; src.m_strTextCustom=L"Cust"; src.m_dwdItemData=0x99;
  CMFCToolBarButton dst; dst.CopyFrom(src);
  const PA& p = static_cast<const PA&>(dst);
  printf("CopyFrom dst: nID=%u nStyle=%u iImage=%d iUserImage=%d userBtn=%d bText=%d strText=%ls strCustom=%ls itemData=%llu\n",
    dst.m_nID, dst.m_nStyle, p.iImg(), p.iUImg(), dst.m_bUserButton, dst.m_bText,
    (const wchar_t*)dst.m_strText, (const wchar_t*)dst.m_strTextCustom, (unsigned long long)dst.m_dwdItemData);
  printf("CopyFrom CompareWith(src) -> %d\n", dst.CompareWith(src));
  return 0;
}
