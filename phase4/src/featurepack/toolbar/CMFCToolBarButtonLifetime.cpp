#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
IMPLEMENT_DYNAMIC(CMFCToolBarButton,CObject)
IMPLEMENT_DYNAMIC(CMFCToolBarComboBoxButton,CMFCToolBarButton)
void OpenMFC_InitToolBarButton(CMFCToolBarButton* b) {
    b->m_bUserButton = FALSE;
    b->m_bText = FALSE;
    b->m_bImage = TRUE;
    b->m_bWrap = FALSE;
    b->m_bWholeText = TRUE;
    b->m_bTextBelow = FALSE;
    b->m_bDragFromCollection = FALSE;
    b->m_nID = 0;
    b->m_nStyle = 0;
    b->m_dwdItemData = 0;
    b->m_iImage = -1;
    b->m_iUserImage = -1;
    b->m_bLocked = FALSE;
    b->m_bIsHidden = FALSE;
    b->m_bDisableFill = FALSE;
    b->m_bExtraSize = FALSE;
    b->m_bHorz = TRUE;
    b->m_bVisible = TRUE;
    b->m_rect.SetRectEmpty();
    b->m_sizeText.cx = 0;
    b->m_sizeText.cy = 0;
    b->m_pWndParent = nullptr;
}
CMFCToolBarButton::CMFCToolBarButton() {
    OpenMFC_InitToolBarButton(this);
}
CMFCToolBarButton::CMFCToolBarButton(UINT uiCmdID, int iImage, const wchar_t* lpszText, BOOL bUserButton, BOOL bLocked) {
    OpenMFC_InitToolBarButton(this);
    m_nID = uiCmdID;
    if (lpszText) m_strText = lpszText;
    m_bUserButton = bUserButton;
    m_bLocked = bLocked;
    if (bUserButton) m_iUserImage = iImage; else m_iImage = iImage;
}
CMFCToolBarButton::~CMFCToolBarButton() {}
