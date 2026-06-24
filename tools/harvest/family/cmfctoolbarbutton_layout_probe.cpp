#include <afxwin.h>
#include <afxcontrolbars.h>
#include <cstddef>
#include <cstdio>
struct P : public CMFCToolBarButton {
  static void dump(){
    printf("sizeof CMFCToolBarButton = %zu\n", sizeof(CMFCToolBarButton));
    #define O(m) printf("  %-22s @ %zu\n", #m, offsetof(P, m))
    O(m_bUserButton); O(m_bText); O(m_bImage); O(m_bWrap); O(m_bWholeText);
    O(m_bTextBelow); O(m_bDragFromCollection); O(m_nID); O(m_nStyle);
    O(m_dwdItemData); O(m_strText); O(m_strTextCustom);
    O(m_iImage); O(m_iUserImage); O(m_bLocked); O(m_bIsHidden);
    O(m_bDisableFill); O(m_bExtraSize); O(m_bHorz); O(m_bVisible);
    O(m_rect); O(m_sizeText); O(m_pWndParent);
    #undef O
  }
};
int main(){ P::dump(); return 0; }
