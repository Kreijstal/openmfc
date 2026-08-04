// CPropertySheet — OpenMFC implementation.
// Sources: dlgcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI impl__PreProcessPageTemplate_CPropertyPage__IEAAXAEAU_PROPSHEETPAGEW__H_Z(
    CPropertyPage* pThis, PROPSHEETPAGEW& psp, int bWizard);

// Symbol: ??0CPropertySheet@@QEAA@IPEAVCWnd@@I@Z
// Constructor: CPropertySheet::CPropertySheet(unsigned int, CWnd*, unsigned int)
extern "C" void* MS_ABI impl___0CPropertySheet__QEAA_IPEAVCWnd__I_Z(
    void* pThis, unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    return new(pThis) CPropertySheet(nIDCaption, pParentWnd, iSelectPage);
}
// Symbol: ??0CPropertySheet@@QEAA@IPEAVCWnd@@IPEAUHBITMAP__@@PEAUHPALETTE__@@1@Z
// Constructor: CPropertySheet::CPropertySheet(unsigned int, CWnd*, unsigned int, HBITMAP, HPALETTE, HBITMAP)
extern "C" void* MS_ABI impl___0CPropertySheet__QEAA_IPEAVCWnd__IPEAUHBITMAP____PEAUHPALETTE____1_Z(
    void* pThis, unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage,
    HBITMAP hbmWatermark, HPALETTE hpalWatermark, HBITMAP hbmHeader) {
    (void)hbmWatermark;
    (void)hpalWatermark;
    (void)hbmHeader;
    return new(pThis) CPropertySheet(nIDCaption, pParentWnd, iSelectPage);
}
// Symbol: ??0CPropertySheet@@QEAA@PEB_WPEAVCWnd@@I@Z
// Constructor: CPropertySheet::CPropertySheet(wchar_t const*, CWnd*, unsigned int)
extern "C" void* MS_ABI impl___0CPropertySheet__QEAA_PEB_WPEAVCWnd__I_Z(
    void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage) {
    return new(pThis) CPropertySheet(pszCaption, pParentWnd, iSelectPage);
}
// Symbol: ??0CPropertySheet@@QEAA@PEB_WPEAVCWnd@@IPEAUHBITMAP__@@PEAUHPALETTE__@@2@Z
// Constructor: CPropertySheet::CPropertySheet(wchar_t const*, CWnd*, unsigned int, HBITMAP, HPALETTE, HBITMAP)
extern "C" void* MS_ABI impl___0CPropertySheet__QEAA_PEB_WPEAVCWnd__IPEAUHBITMAP____PEAUHPALETTE____2_Z(
    void* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage,
    HBITMAP hbmWatermark, HPALETTE hpalWatermark, HBITMAP hbmHeader) {
    (void)hbmWatermark;
    (void)hpalWatermark;
    (void)hbmHeader;
    return new(pThis) CPropertySheet(pszCaption, pParentWnd, iSelectPage);
}
// Symbol: ?CommonConstruct@CPropertySheet@@QEAAXPEAVCWnd@@I@Z
extern "C" void MS_ABI impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(
    CPropertySheet* pThis, CWnd* pParentWnd, UINT iSelectPage) {
    if (!pThis) return;
    pThis->m_hWnd = nullptr;
    pThis->m_pszCaption = nullptr;
    pThis->m_pParentWnd = pParentWnd;
    pThis->m_nActivePage = iSelectPage;
    pThis->m_bWizardMode = FALSE;
    pThis->m_nPageCount = 0;
    std::memset(pThis->m_pages, 0, sizeof(pThis->m_pages));
    std::memset(PropertySheetAccess(pThis)->_propertysheet_padding, 0, sizeof(PropertySheetAccess(pThis)->_propertysheet_padding));
}
// Symbol: ?CommonConstruct@CPropertySheet@@QEAAXPEAVCWnd@@IPEAUHBITMAP__@@PEAUHPALETTE__@@1@Z
extern "C" void MS_ABI impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__IPEAUHBITMAP____PEAUHPALETTE____1_Z(
    CPropertySheet* pThis, CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark, HBITMAP hbmHeader) {
    (void)hbmWatermark;
    (void)hpalWatermark;
    (void)hbmHeader;
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(pThis, pParentWnd, iSelectPage);
}
// Symbol: ?Construct@CPropertySheet@@QEAAXIPEAVCWnd@@I@Z
extern "C" void MS_ABI impl__Construct_CPropertySheet__QEAAXIPEAVCWnd__I_Z(
    CPropertySheet* pThis, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage) {
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(pThis, pParentWnd, iSelectPage);
    wchar_t caption[256] = {};
    HINSTANCE instance = AfxGetInstanceHandle();
    if (!instance) instance = ::GetModuleHandleW(nullptr);
    if (pThis && nIDCaption && ::LoadStringW(instance, nIDCaption, caption, 256) > 0) {
        g_propertySheetCaptions[pThis] = caption;
        pThis->m_pszCaption = static_cast<const wchar_t*>(g_propertySheetCaptions[pThis]);
    }
}
// Symbol: ?Construct@CPropertySheet@@QEAAXIPEAVCWnd@@IPEAUHBITMAP__@@PEAUHPALETTE__@@1@Z
extern "C" void MS_ABI impl__Construct_CPropertySheet__QEAAXIPEAVCWnd__IPEAUHBITMAP____PEAUHPALETTE____1_Z(
    CPropertySheet* pThis, UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark, HBITMAP hbmHeader) {
    (void)nIDCaption;
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__IPEAUHBITMAP____PEAUHPALETTE____1_Z(
        pThis, pParentWnd, iSelectPage, hbmWatermark, hpalWatermark, hbmHeader);
}
// Symbol: ?Construct@CPropertySheet@@QEAAXPEB_WPEAVCWnd@@I@Z
extern "C" void MS_ABI impl__Construct_CPropertySheet__QEAAXPEB_WPEAVCWnd__I_Z(
    CPropertySheet* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, UINT iSelectPage) {
    impl__CommonConstruct_CPropertySheet__QEAAXPEAVCWnd__I_Z(pThis, pParentWnd, iSelectPage);
    if (pThis) pThis->m_pszCaption = pszCaption;
}
// Symbol: ?Construct@CPropertySheet@@QEAAXPEB_WPEAVCWnd@@IPEAUHBITMAP__@@PEAUHPALETTE__@@2@Z
extern "C" void MS_ABI impl__Construct_CPropertySheet__QEAAXPEB_WPEAVCWnd__IPEAUHBITMAP____PEAUHPALETTE____2_Z(
    CPropertySheet* pThis, const wchar_t* pszCaption, CWnd* pParentWnd, UINT iSelectPage, HBITMAP hbmWatermark, HPALETTE hpalWatermark, HBITMAP hbmHeader) {
    (void)hbmWatermark;
    (void)hpalWatermark;
    (void)hbmHeader;
    impl__Construct_CPropertySheet__QEAAXPEB_WPEAVCWnd__I_Z(pThis, pszCaption, pParentWnd, iSelectPage);
}
// Symbol: ?BuildPropPageArray@CPropertySheet@@UEAAXXZ
extern "C" void MS_ABI impl__BuildPropPageArray_CPropertySheet__UEAAXXZ(CPropertySheet* pThis) {
    if (!pThis) return;
    for (int i = 0; i < pThis->m_nPageCount; ++i) {
        CPropertyPage* page = pThis->m_pages[i];
        if (!page) continue;
        PROPSHEETPAGEW psp;
        impl__PreProcessPageTemplate_CPropertyPage__IEAAXAEAU_PROPSHEETPAGEW__H_Z(
            page, psp, pThis->m_bWizardMode);
    }
}
// Symbol: ?ContinueModal@CPropertySheet@@UEAAHXZ
extern "C" int MS_ABI impl__ContinueModal_CPropertySheet__UEAAHXZ(CPropertySheet* pThis) {
    return pThis && pThis->m_hWnd && ::IsWindow(pThis->m_hWnd);
}
// Symbol: ?EnableStackedTabs@CPropertySheet@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableStackedTabs_CPropertySheet__QEAAXH_Z(CPropertySheet* pThis, int bStacked) {
    if (!pThis || !pThis->GetSafeHwnd()) return;
    HWND tab = PropSheet_GetTabControl(pThis->GetSafeHwnd());
    if (!tab) return;
    LONG_PTR style = ::GetWindowLongPtrW(tab, GWL_STYLE);
    style = bStacked ? (style | TCS_MULTILINE) : (style & ~static_cast<LONG_PTR>(TCS_MULTILINE));
    ::SetWindowLongPtrW(tab, GWL_STYLE, style);
    ::SetWindowPos(tab, nullptr, 0, 0, 0, 0,
                   SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
}
// Symbol: ?GetMessageMap@CPropertySheet@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CPropertySheet__MEBAPEBUAFX_MSGMAP__XZ(
    const CPropertySheet* pThis) {
    (void)pThis;
    return EmptyMessageMap_Dlgcore();
}
// Symbol: ?GetRuntimeClass@CPropertySheet@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPropertySheet__UEBAPEAUCRuntimeClass__XZ(
    const CPropertySheet* pThis) {
    return CPropertySheet::GetThisClass();
}
// Symbol: ?GetThisClass@CPropertySheet@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPropertySheet__SAPEAUCRuntimeClass__XZ() {
    return CPropertySheet::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CPropertySheet@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPropertySheet__KAPEBUAFX_MSGMAP__XZ() {
    return EmptyMessageMap_Dlgcore();
}
// Symbol: ?HandleInitDialog@CPropertySheet@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__HandleInitDialog_CPropertySheet__IEAA_J_K_J_Z(
    CPropertySheet* pThis, WPARAM wParam, LPARAM lParam) {
    (void)wParam;
    (void)lParam;
    return pThis ? pThis->OnInitDialog() : FALSE;
}
// Symbol: ?OnClose@CPropertySheet@@IEAAXXZ
extern "C" void MS_ABI impl__OnClose_CPropertySheet__IEAAXXZ(CPropertySheet* pThis) {
    if (pThis) pThis->EndDialog(IDCANCEL);
}
// Symbol: ?OnCmdMsg@CPropertySheet@@UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO@@@Z
extern "C" int MS_ABI impl__OnCmdMsg_CPropertySheet__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CPropertySheet* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) {
    (void)pThis;
    (void)nID;
    (void)nCode;
    (void)pExtra;
    (void)pHandlerInfo;
    return FALSE;
}
// Symbol: ?OnCommand@CPropertySheet@@UEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CPropertySheet__UEAAH_K_J_Z(
    CPropertySheet* pThis, WPARAM wParam, LPARAM lParam) {
    (void)lParam;
    UINT id = LOWORD(wParam);
    if (pThis && (id == IDOK || id == IDCANCEL)) {
        pThis->EndDialog(id);
        return TRUE;
    }
    return FALSE;
}
// Symbol: ?OnCommandHelp@CPropertySheet@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnCommandHelp_CPropertySheet__IEAA_J_K_J_Z(
    CPropertySheet* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?OnCtlColor@CPropertySheet@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CPropertySheet__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    CPropertySheet* pThis, CDC* pDC, CWnd* pWnd, UINT nCtlColor) {
    (void)pThis;
    (void)pWnd;
    int colorIndex = nCtlColor == CTLCOLOR_EDIT ? COLOR_WINDOW : COLOR_BTNFACE;
    if (pDC && pDC->GetSafeHdc()) {
        ::SetBkColor(pDC->GetSafeHdc(), ::GetSysColor(colorIndex));
    }
    return ::GetSysColorBrush(colorIndex);
}
// Symbol: ?OnGetMinMaxInfo@CPropertySheet@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CPropertySheet__IEAAXPEAUtagMINMAXINFO___Z(
    CPropertySheet* pThis, MINMAXINFO* lpMMI) {
    (void)pThis;
    (void)lpMMI;
}
// Symbol: ?OnKickIdle@CPropertySheet@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnKickIdle_CPropertySheet__IEAA_J_K_J_Z(
    CPropertySheet* pThis, WPARAM wParam, LPARAM lParam) {
    (void)pThis;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?OnNcCreate@CPropertySheet@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CPropertySheet__IEAAHPEAUtagCREATESTRUCTW___Z(
    CPropertySheet* pThis, CREATESTRUCTW* pCreateStruct) {
    (void)pThis;
    (void)pCreateStruct;
    return TRUE;
}
// Symbol: ?OnSetDefID@CPropertySheet@@IEAA_J_K_J@Z
extern "C" LRESULT MS_ABI impl__OnSetDefID_CPropertySheet__IEAA_J_K_J_Z(
    CPropertySheet* pThis, WPARAM wParam, LPARAM lParam) {
    return pThis && pThis->GetSafeHwnd()
        ? ::DefWindowProcW(pThis->GetSafeHwnd(), DM_SETDEFID, wParam, lParam)
        : 0;
}
// Symbol: ?OnSysCommand@CPropertySheet@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CPropertySheet__IEAAXI_J_Z(
    CPropertySheet* pThis, UINT nID, LPARAM lParam) {
    (void)lParam;
    if (pThis && ((nID & 0xFFF0) == SC_CLOSE)) {
        pThis->EndDialog(IDCANCEL);
    }
}
// Symbol: ?PreTranslateMessage@CPropertySheet@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CPropertySheet__UEAAHPEAUtagMSG___Z(
    CPropertySheet* pThis, MSG* pMsg) {
    if (pThis && pThis->m_hWnd && pMsg) {
        return ::IsDialogMessageW(pThis->m_hWnd, pMsg);
    }
    return FALSE;
}
CPropertySheet::CPropertySheet()
    : CWnd(), m_pszCaption(nullptr), m_pParentWnd(nullptr),
      m_nActivePage(0), m_bWizardMode(FALSE), m_nPageCount(0)
{
    memset(m_pages, 0, sizeof(m_pages));
    memset(_propertysheet_padding, 0, sizeof(_propertysheet_padding));
}
CPropertySheet::CPropertySheet(unsigned int nIDCaption, CWnd* pParentWnd, unsigned int iSelectPage)
    : CWnd(), m_pszCaption(nullptr), m_pParentWnd(pParentWnd),
      m_nActivePage(iSelectPage), m_bWizardMode(FALSE), m_nPageCount(0)
{
    wchar_t caption[256] = {};
    HINSTANCE instance = AfxGetInstanceHandle();
    if (!instance) instance = ::GetModuleHandleW(nullptr);
    if (nIDCaption && ::LoadStringW(instance, nIDCaption, caption, 256) > 0) {
        g_propertySheetCaptions[this] = caption;
        m_pszCaption = static_cast<const wchar_t*>(g_propertySheetCaptions[this]);
    }
    memset(m_pages, 0, sizeof(m_pages));
    memset(_propertysheet_padding, 0, sizeof(_propertysheet_padding));
}
CPropertySheet::CPropertySheet(const wchar_t* pszCaption, CWnd* pParentWnd, unsigned int iSelectPage)
    : CWnd(), m_pszCaption(pszCaption), m_pParentWnd(pParentWnd),
      m_nActivePage(iSelectPage), m_bWizardMode(FALSE), m_nPageCount(0)
{
    memset(m_pages, 0, sizeof(m_pages));
    memset(_propertysheet_padding, 0, sizeof(_propertysheet_padding));
}
int CPropertySheet::GetPageCount() const {
    return m_nPageCount;
}
CPropertyPage* CPropertySheet::GetActivePage() const {
    if (m_hWnd) {
        int nActive = (int)PropSheet_HwndToIndex(m_hWnd, (HWND)PropSheet_GetCurrentPageHwnd(m_hWnd));
        if (nActive >= 0 && nActive < m_nPageCount) {
            return m_pages[nActive];
        }
    }
    if ((int)m_nActivePage < m_nPageCount) {
        return m_pages[m_nActivePage];
    }
    return nullptr;
}
int CPropertySheet::GetActiveIndex() const {
    if (m_hWnd) {
        return (int)PropSheet_HwndToIndex(m_hWnd, (HWND)PropSheet_GetCurrentPageHwnd(m_hWnd));
    }
    return (int)m_nActivePage;
}
CPropertyPage* CPropertySheet::GetPage(int nPage) const {
    if (nPage >= 0 && nPage < m_nPageCount) {
        return m_pages[nPage];
    }
    return nullptr;
}
int CPropertySheet::GetPageIndex(CPropertyPage* pPage) const {
    for (int i = 0; i < m_nPageCount; i++) {
        if (m_pages[i] == pPage) {
            return i;
        }
    }
    return -1;
}
int CPropertySheet::SetActivePage(int nPage) {
    if (m_hWnd) {
        return PropSheet_SetCurSel(m_hWnd, nullptr, nPage);
    }
    if (nPage >= 0 && nPage < m_nPageCount) {
        m_nActivePage = nPage;
        return TRUE;
    }
    return FALSE;
}
int CPropertySheet::SetActivePage(CPropertyPage* pPage) {
    int nIndex = GetPageIndex(pPage);
    if (nIndex >= 0) {
        return SetActivePage(nIndex);
    }
    return FALSE;
}
void CPropertySheet::SetTitle(const wchar_t* lpszText, unsigned int nStyle) {
    m_pszCaption = lpszText;
    if (m_hWnd) {
        PropSheet_SetTitle(m_hWnd, nStyle, lpszText);
    }
}
void CPropertySheet::SetWizardMode() {
    m_bWizardMode = TRUE;
}
void CPropertySheet::SetWizardButtons(unsigned long dwFlags) {
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, PSM_SETWIZBUTTONS, 0, (LPARAM)dwFlags);
    }
}
void CPropertySheet::SetFinishText(const wchar_t* lpszText) {
    if (m_hWnd) {
        ::SendMessageW(m_hWnd, PSM_SETFINISHTEXT, 0, (LPARAM)lpszText);
    }
}
void CPropertySheet::AddPage(CPropertyPage* pPage) {
    if (m_nPageCount < 16 && pPage) {
        m_pages[m_nPageCount++] = pPage;
        // If sheet is already created, add page dynamically
        if (m_hWnd && pPage->m_lpszTemplateName) {
            PROPSHEETPAGEW psp = {};
            psp.dwSize = sizeof(psp);
            psp.dwFlags = PSP_DLGINDIRECT;
            psp.hInstance = AfxGetInstanceHandle();
            psp.pszTemplate = pPage->m_lpszTemplateName;
            psp.pfnDlgProc = PropPageDlgProc;
            psp.lParam = reinterpret_cast<LPARAM>(pPage);
            HPROPSHEETPAGE hPage = CreatePropertySheetPageW(&psp);
            if (hPage) {
                PropSheet_AddPage(m_hWnd, hPage);
            }
        }
    }
}
void CPropertySheet::RemovePage(CPropertyPage* pPage) {
    int nIndex = GetPageIndex(pPage);
    if (nIndex >= 0) {
        RemovePage(nIndex);
    }
}
void CPropertySheet::RemovePage(int nPage) {
    if (nPage >= 0 && nPage < m_nPageCount) {
        if (m_hWnd) {
            PropSheet_RemovePage(m_hWnd, nPage, nullptr);
        }
        // Shift pages down
        for (int i = nPage; i < m_nPageCount - 1; i++) {
            m_pages[i] = m_pages[i + 1];
        }
        m_pages[--m_nPageCount] = nullptr;
    }
}
void CPropertySheet::PressButton(int nButton) {
    if (m_hWnd) {
        ::PostMessageW(m_hWnd, PSM_PRESSBUTTON, (WPARAM)nButton, 0);
    }
}
void CPropertySheet::EndDialog(int nEndID) {
    if (m_hWnd) {
        // For modal property sheets, post a message to close
        if (nEndID == IDOK) {
            ::PostMessageW(m_hWnd, PSM_PRESSBUTTON, (WPARAM)PSBTN_OK, 0);
        } else {
            ::PostMessageW(m_hWnd, PSM_PRESSBUTTON, (WPARAM)PSBTN_CANCEL, 0);
        }
    }
}
intptr_t CPropertySheet::DoModal() {
    if (m_nPageCount == 0) {
        return -1;
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Build array of PROPSHEETPAGE structures
    PROPSHEETPAGEW* pPages = new PROPSHEETPAGEW[m_nPageCount];
    memset(pPages, 0, sizeof(PROPSHEETPAGEW) * m_nPageCount);

    for (int i = 0; i < m_nPageCount; i++) {
        pPages[i].dwSize = sizeof(PROPSHEETPAGEW);
        pPages[i].dwFlags = 0;
        pPages[i].hInstance = hInst;
        pPages[i].pszTemplate = m_pages[i]->m_lpszTemplateName;
        pPages[i].pfnDlgProc = PropPageDlgProc;
        pPages[i].lParam = reinterpret_cast<LPARAM>(m_pages[i]);
    }

    // Build property sheet header
    PROPSHEETHEADERW psh = {};
    psh.dwSize = sizeof(psh);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_USECALLBACK;
    if (m_bWizardMode) {
        psh.dwFlags |= PSH_WIZARD;
    }
    psh.hwndParent = m_pParentWnd ? m_pParentWnd->m_hWnd : nullptr;
    psh.hInstance = hInst;
    psh.pszCaption = m_pszCaption;
    psh.nPages = m_nPageCount;
    psh.nStartPage = m_nActivePage;
    psh.ppsp = pPages;
    psh.pfnCallback = PropSheetCallback;

    // Show the property sheet
    g_pendingPropertySheet = this;
    INT_PTR nResult = PropertySheetW(&psh);
    g_pendingPropertySheet = nullptr;
    for (auto it = g_propertySheetMap.begin(); it != g_propertySheetMap.end();) {
        if (it->second == this) it = g_propertySheetMap.erase(it); else ++it;
    }

    delete[] pPages;

    // Convert result: positive = IDOK, 0 = IDCANCEL, negative = error
    if (nResult > 0) {
        return IDOK;
    } else if (nResult == 0) {
        return IDCANCEL;
    }
    return -1;
}
int CPropertySheet::Create(CWnd* pParentWnd, unsigned long dwStyle, unsigned long dwExStyle) {
    if (m_nPageCount == 0) {
        return FALSE;
    }

    HINSTANCE hInst = AfxGetInstanceHandle();
    if (!hInst) {
        hInst = GetModuleHandle(nullptr);
    }

    // Build array of PROPSHEETPAGE structures
    PROPSHEETPAGEW* pPages = new PROPSHEETPAGEW[m_nPageCount];
    memset(pPages, 0, sizeof(PROPSHEETPAGEW) * m_nPageCount);

    for (int i = 0; i < m_nPageCount; i++) {
        pPages[i].dwSize = sizeof(PROPSHEETPAGEW);
        pPages[i].dwFlags = 0;
        pPages[i].hInstance = hInst;
        pPages[i].pszTemplate = m_pages[i]->m_lpszTemplateName;
        pPages[i].pfnDlgProc = PropPageDlgProc;
        pPages[i].lParam = reinterpret_cast<LPARAM>(m_pages[i]);
    }

    // Build property sheet header for modeless
    PROPSHEETHEADERW psh = {};
    psh.dwSize = sizeof(psh);
    psh.dwFlags = PSH_PROPSHEETPAGE | PSH_MODELESS | PSH_USECALLBACK;
    if (m_bWizardMode) {
        psh.dwFlags |= PSH_WIZARD;
    }
    psh.hwndParent = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    psh.hInstance = hInst;
    psh.pszCaption = m_pszCaption;
    psh.nPages = m_nPageCount;
    psh.nStartPage = m_nActivePage;
    psh.ppsp = pPages;
    psh.pfnCallback = PropSheetCallback;

    // Create the modeless property sheet
    g_pendingPropertySheet = this;
    m_hWnd = (HWND)PropertySheetW(&psh);
    g_pendingPropertySheet = nullptr;
    m_pParentWnd = pParentWnd;

    if (m_hWnd) {
        if (dwStyle != 0xFFFFFFFFUL) ::SetWindowLongPtrW(m_hWnd, GWL_STYLE, static_cast<LONG_PTR>(dwStyle));
        if (dwExStyle != 0) ::SetWindowLongPtrW(m_hWnd, GWL_EXSTYLE, static_cast<LONG_PTR>(dwExStyle));
        if (dwStyle != 0xFFFFFFFFUL || dwExStyle != 0) {
            ::SetWindowPos(m_hWnd, nullptr, 0, 0, 0, 0,
                           SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        }
    }

    delete[] pPages;

    return m_hWnd != nullptr;
}
int CPropertySheet::OnInitDialog() {
    return TRUE;
}
void CPropertySheet::OnPageChanged() {
    // Default: do nothing
}
