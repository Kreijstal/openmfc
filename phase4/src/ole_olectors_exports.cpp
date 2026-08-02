#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Symbol: ??0COleClientItem@@QEAA@PEAVCOleDocument@@@Z
// COleClientItem::COleClientItem(COleDocument* pContainerDoc)
extern "C" void* MS_ABI impl___0COleClientItem__QEAA_PEAVCOleDocument___Z(COleDocument* pContainerDoc) {
    return new COleClientItem(pContainerDoc);
}

// Symbol: ??0CRichEditCntrItem@@QEAA@PEAU_reobject@@PEAVCRichEditDoc@@@Z
// CRichEditCntrItem::CRichEditCntrItem(REOBJECT* pObject, CRichEditDoc* pContainerDoc)
// NOTE: the OpenMFC constructor currently takes only the container document.
extern "C" void* MS_ABI impl___0CRichEditCntrItem__QEAA_PEAU_reobject__PEAVCRichEditDoc___Z(void* pObject, CRichEditDoc* pContainerDoc) {
    (void)pObject;
    return new CRichEditCntrItem(pContainerDoc);
}

// Symbol: ??0CRichEditDoc@@IEAA@XZ
// CRichEditDoc::CRichEditDoc()
extern "C" void* MS_ABI impl___0CRichEditDoc__IEAA_XZ() {
    return new CRichEditDoc();
}

// Symbol: ??0CWinThread@@QEAA@P6AIPEAX@Z0@Z
// CWinThread::CWinThread()
extern "C" void* MS_ABI impl___0CWinThread__QEAA_P6AIPEAX_Z0_Z(void* p0, void* p1) {
    (void)p0;
    (void)p1;
    return new CWinThread();
}

// Symbol: ??0CView@@IEAA@XZ
// CView::CView()
extern "C" void* MS_ABI impl___0CView__IEAA_XZ() {
    struct CViewFactoryShim : public CView {
        void OnDraw(void* pDC) override { (void)pDC; }
    };
    return new CViewFactoryShim();
}

// Symbol: ??0CMFCBaseToolBar@@QEAA@XZ
// CMFCBaseToolBar::CMFCBaseToolBar()
extern "C" void* MS_ABI impl___0CMFCBaseToolBar__QEAA_XZ() {
    return new CMFCBaseToolBar();
}

// Symbol: ??0CMFCCaptionButton@@QEAA@IH@Z
// CMFCCaptionButton::CMFCCaptionButton()
extern "C" void* MS_ABI impl___0CMFCCaptionButton__QEAA_IH_Z(unsigned int p0, int p1) {
    (void)p0;
    (void)p1;
    return new CMFCCaptionButton();
}

// Symbol: ??0CMFCRibbonContextCaption@@IEAA_XZ
// CMFCRibbonContextCaption::CMFCRibbonContextCaption()
extern "C" void* MS_ABI impl___0CMFCRibbonContextCaption__IEAA_XZ() {
    return new CMFCRibbonContextCaption();
}

// Symbol: ??0CMFCRibbonContextCaption@@IEAA@PEB_WIW4AFX_RibbonCategoryColor@@@Z
// CMFCRibbonContextCaption::CMFCRibbonContextCaption()
// (additional parameters are kept for ABI compatibility)
extern "C" void* MS_ABI impl___0CMFCRibbonContextCaption__IEAA_PEB_WIW4AFX_RibbonCategoryColor___Z(
    const wchar_t* pName, unsigned int nCategory, int nID,
    void* p0, void* p1, void* p2, void* p3, void* p4, void* p5, void* p6,
    void* p7, void* p8, void* p9, void* p10, void* p11, void* p12,
    void* p13, void* p14, void* p15, void* p16, void* p17, void* p18,
    void* p19, void* p20, void* p21, void* p22) {
    (void)pName;
    (void)nCategory;
    (void)nID;
    (void)p0; (void)p1; (void)p2; (void)p3; (void)p4; (void)p5; (void)p6;
    (void)p7; (void)p8; (void)p9; (void)p10; (void)p11; (void)p12;
    (void)p13; (void)p14; (void)p15; (void)p16; (void)p17; (void)p18;
    (void)p19; (void)p20; (void)p21; (void)p22;
    return new CMFCRibbonContextCaption();
}

// Symbol: ??0CMFCRibbonButtonsGroup@@QEAA@PEAVCMFCRibbonBaseElement@@@Z
// CMFCRibbonButtonsGroup::CMFCRibbonButtonsGroup()
extern "C" void* MS_ABI impl___0CMFCRibbonButtonsGroup__QEAA_PEAVCMFCRibbonBaseElement___Z(
    void* /*p0*/) {
    return new CMFCRibbonButtonsGroup();
}

// Symbol: ??0CMFCRibbonCaptionButton@@IEAA@IPEAUHWND__@@_Z
// CMFCRibbonCaptionButton::CMFCRibbonCaptionButton()
extern "C" void* MS_ABI impl___0CMFCRibbonCaptionButton__IEAA_IPEAUHWND_____Z(
    unsigned int p0, void* /*struct*/* p1) {
    (void)p0;
    (void)p1;
    return new CMFCRibbonCaptionButton();
}

// Symbol: ??0CMFCRibbonCategory@@IEAA@PEAVCMFCRibbonBar@@PEB_WIIVCSize@@2@Z
// CMFCRibbonCategory::CMFCRibbonCategory()
extern "C" void* MS_ABI impl___0CMFCRibbonCategory__IEAA_PEAVCMFCRibbonBar__PEB_WIIVCSize__2_Z(
    void* /*p0*/, const wchar_t* p1, unsigned int p2, unsigned int p3, void* p4, unsigned int p5) {
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    (void)p5;
    return new CMFCRibbonCategory();
}

// Symbol: ??0CMFCRibbonPanel@@IEAA@PEAVCMFCRibbonGallery@@@Z
// CMFCRibbonPanel::CMFCRibbonPanel()
extern "C" void* MS_ABI impl___0CMFCRibbonPanel__IEAA_PEAVCMFCRibbonGallery___Z(void* /*p0*/) {
    return new CMFCRibbonPanel();
}

// Symbol: ??0CMFCRibbonPanel@@IEAA@PEB_WPEAUHICON__@@@Z
// CMFCRibbonPanel::CMFCRibbonPanel(const wchar_t* lpszName, HICON hIcon)
extern "C" void* MS_ABI impl___0CMFCRibbonPanel__IEAA_PEB_WPEAUHICON_____Z(
    const wchar_t* pName, void* hIcon) {
    return new CMFCRibbonPanel(pName, static_cast<HICON>(hIcon));
}

// Symbol: ??0CMFCPopupMenu@@IEAA@PEAVCMFCToolBarsMenuPropertyPage@@PEB_W@Z
// CMFCPopupMenu::CMFCPopupMenu(CMFCToolBarsMenuPropertyPage*, const wchar_t*)
extern "C" void* MS_ABI impl___0CMFCPopupMenu__IEAA_PEAVCMFCToolBarsMenuPropertyPage__PEB_W_Z(
    void* pPage, const wchar_t* pName) {
    (void)pPage;
    (void)pName;
    return new CMFCPopupMenu();
}

// Symbol: ??0COleControlSiteOrWnd@@QEAA@PEAUHWND__@@H@Z
// COleControlSiteOrWnd::COleControlSiteOrWnd(HWND hWnd, BOOL bNeedFont)
extern "C" void* MS_ABI impl___0COleControlSiteOrWnd__QEAA_PEAUHWND____H_Z(void* hWnd, int bNeedFont) {
    (void)hWnd;
    (void)bNeedFont;
    return new COleControlSiteOrWnd();
}

// Symbol: ??0COleControlSiteOrWnd@@QEAA@PEAVCOleControlSite@@@Z
// COleControlSiteOrWnd::COleControlSiteOrWnd(COleControlSite* pSite)
extern "C" void* MS_ABI impl___0COleControlSiteOrWnd__QEAA_PEAVCOleControlSite___Z(COleControlSite* pSite) {
    auto* pThis = new COleControlSiteOrWnd();
    pThis->m_pSite = pSite;
    pThis->m_pWnd = nullptr;
    return pThis;
}

// Symbol: ??0COleControlContainer@@QEAA@PEAVCWnd@@@Z
// COleControlContainer::COleControlContainer(CWnd* pWnd)
extern "C" void* MS_ABI impl___0COleControlContainer__QEAA_PEAVCWnd___Z(void* p0) {
    return new COleControlContainer(static_cast<CWnd*>(p0));
}

// Symbol: ??0COleControlSite@@QEAA@PEAVCOleControlContainer@@@Z
// COleControlSite::COleControlSite(COleControlContainer* pCtrlCont)
extern "C" void* MS_ABI impl___0COleControlSite__QEAA_PEAVCOleControlContainer___Z(void* p0) {
    return new COleControlSite(static_cast<COleControlContainer*>(p0));
}

// Symbol: ??0COleDocObjectItem@@QEAA@PEAVCOleDocument@@@Z
// COleDocObjectItem::COleDocObjectItem(COleDocument* pContainerDoc)
extern "C" void* MS_ABI impl___0COleDocObjectItem__QEAA_PEAVCOleDocument___Z(COleDocument* pContainerDoc) {
    return new COleDocObjectItem(pContainerDoc);
}

// Symbol: ??0COleFrameHook@@QEAA@PEAVCFrameWnd@@PEAVCOleClientItem@@@Z
// COleFrameHook::COleFrameHook(CFrameWnd* pFrameWnd, COleClientItem* pItem)
extern "C" void* MS_ABI impl___0COleFrameHook__QEAA_PEAVCFrameWnd__PEAVCOleClientItem___Z(
    CFrameWnd* pFrameWnd, COleClientItem* pItem) {
    return new COleFrameHook(pFrameWnd, pItem);
}

// Symbol: ??0COlePropertiesDialog@@QEAA@PEAVCOleClientItem@@IIPEAVCWnd@@@Z
// COlePropertiesDialog::COlePropertiesDialog(COleClientItem* pItem, UINT nScaleMin, UINT nScaleMax, CWnd* pParentWnd)
extern "C" void* MS_ABI impl___0COlePropertiesDialog__QEAA_PEAVCOleClientItem__IIPEAVCWnd___Z(
    COleClientItem* pItem, unsigned int nScaleMin, unsigned int nScaleMax, CWnd* pParentWnd) {
    return new COlePropertiesDialog(pItem, nScaleMin, nScaleMax, pParentWnd);
}

// Symbol: ?OnApplyScale@COlePropertiesDialog@@UEAAHPEAVCOleClientItem@@HH@Z
// COlePropertiesDialog::OnApplyScale
extern "C" int MS_ABI impl__OnApplyScale_COlePropertiesDialog__UEAAHPEAVCOleClientItem__HH_Z(
    COlePropertiesDialog* pThis, COleClientItem* pItem, unsigned int nScaleMin, unsigned int nScaleMax) {
    if (pThis) {
        if (pItem) {
            pThis->m_pItem = pItem;
        }
        return (nScaleMin <= nScaleMax) ? TRUE : FALSE;
    }
    return FALSE;
}

// Symbol: ?OnInitDialog@COlePropertiesDialog@@UEAAHXZ
// COlePropertiesDialog::OnInitDialog
extern "C" int MS_ABI impl__OnInitDialog_COlePropertiesDialog__UEAAHXZ(
    COlePropertiesDialog* pThis) {
    return pThis ? static_cast<CDialog*>(static_cast<COleDialog*>(pThis))->OnInitDialog() : FALSE;
}

// Symbol: ??0COleServerItem@@IEAA@PEAVCOleServerDoc@@H@Z
// COleServerItem::COleServerItem(COleServerDoc* pServerDoc, BOOL bAutoDelete)
extern "C" void* MS_ABI impl___0COleServerItem__IEAA_PEAVCOleServerDoc__H_Z(COleServerDoc* pServerDoc, unsigned int bAutoDelete) {
    return new COleServerItem(pServerDoc, bAutoDelete);
}
