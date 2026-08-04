// CMFCToolBarButton — OpenMFC implementation.
// Sources: cbarcore.cpp, global_cmfctoolbarbutton_msvtbl.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/CMFCToolBarButtonSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?SetClipboardFormatName@CMFCToolBarButton@@SAXPEB_W@Z
void CMFCToolBarButton::SetClipboardFormatName(const wchar_t* lpszName) {
    if (lpszName && *lpszName) {
        m_strClipboardFormatName = lpszName;
    } else {
        m_strClipboardFormatName.Empty();
    }
}
CString CMFCToolBarButton::m_strClipboardFormatName;
BOOL CMFCToolBarButton::m_bWrapText = TRUE;
BOOL CMFCToolBarButton::m_bUpdateImages = FALSE;
// ---- named exports: real bodies (flip the weak null stubs to real behavior) ----
// Symbol: ?Serialize@CMFCToolBarButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    tbb_Serialize((CMFCToolBarButton*)pThis, ar);
}
// Symbol: ?CopyFrom@CMFCToolBarButton@@UEAAXAEBV1@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* src) {
    tbb_CopyFrom((CMFCToolBarButton*)pThis, src);
}
// Symbol: ?CompareWith@CMFCToolBarButton@@UEBAHAEBV1@@Z
extern "C" int MS_ABI impl__CompareWith_CMFCToolBarButton__UEBAHAEBV1__Z(const void* pThis, const CMFCToolBarButton* other) {
    return tbb_CompareWith((const CMFCToolBarButton*)pThis, other);
}
// Symbol: ?SetImage@CMFCToolBarButton@@UEAAXH@Z
extern "C" void MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage) {
    tbb_SetImage((CMFCToolBarButton*)pThis, iImage);
}
// Symbol: ?ResetImageToDefault@CMFCToolBarButton@@UEAAXXZ
extern "C" void MS_ABI impl__ResetImageToDefault_CMFCToolBarButton__UEAAXXZ(void*) {
    /* base class: no-op (harvested from real mfc140u) */
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
