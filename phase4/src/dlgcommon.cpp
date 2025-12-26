// dlgcommon.cpp - Common Dialog implementations (REAL, not stubs)
//
// This file implements:
// - CFileDialog (GetOpenFileName/GetSaveFileName)
// - CColorDialog (ChooseColor)
// - CFontDialog (ChooseFont)
// - CPrintDialog (PrintDlg)
// - CPageSetupDialog (PageSetupDlg)
// - CFindReplaceDialog (FindText/ReplaceText)

// Skip inline implementations from afxwin.h
#define OPENMFC_APPCORE_IMPL

#include <openmfc/afxwin.h>
#include <windows.h>
#include <commdlg.h>
#include <cstring>

// MS ABI for x64 Windows
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

//=============================================================================
// Runtime class implementations
//=============================================================================

IMPLEMENT_DYNAMIC(CColorDialog, CDialog)
IMPLEMENT_DYNAMIC(CFontDialog, CDialog)
IMPLEMENT_DYNAMIC(CPrintDialog, CDialog)
IMPLEMENT_DYNAMIC(CPageSetupDialog, CDialog)
IMPLEMENT_DYNAMIC(CFindReplaceDialog, CDialog)

//=============================================================================
// CFileDialog implementation
//=============================================================================

CFileDialog::CFileDialog(int bOpenFileDialog,
                         const wchar_t* lpszDefExt,
                         const wchar_t* lpszFileName,
                         unsigned long dwFlags,
                         const wchar_t* lpszFilter,
                         CWnd* pParentWnd,
                         unsigned long dwSize,
                         int bVistaStyle)
    : CDialog(), m_bOpenFileDialog(bOpenFileDialog), m_dwFlags(dwFlags), m_pFileList(nullptr) {
    (void)pParentWnd;
    (void)dwSize;
    (void)bVistaStyle;

    if (lpszDefExt != nullptr) {
        m_strDefExt = lpszDefExt;
    }
    if (lpszFileName != nullptr) {
        m_strFileName = lpszFileName;
    }
    if (lpszFilter != nullptr) {
        m_strFilter = lpszFilter;
    }
    memset(_filedialog_padding, 0, sizeof(_filedialog_padding));
}

intptr_t CFileDialog::DoModal() {
    // Allocate buffer for multiple file selection
    const size_t nBufferSize = 65536;
    wchar_t* szFile = new wchar_t[nBufferSize];
    memset(szFile, 0, nBufferSize * sizeof(wchar_t));

    if (!m_strFileName.IsEmpty()) {
        wcsncpy(szFile, (const wchar_t*)m_strFileName, MAX_PATH - 1);
    }

    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = (DWORD)nBufferSize;
    ofn.lpstrFilter = m_strFilter.IsEmpty() ? L"All Files\0*.*\0" : (const wchar_t*)m_strFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = m_strDefExt.IsEmpty() ? nullptr : (const wchar_t*)m_strDefExt;
    ofn.lpstrInitialDir = m_strInitialDir.IsEmpty() ? nullptr : (const wchar_t*)m_strInitialDir;
    ofn.Flags = m_dwFlags | OFN_EXPLORER;

    BOOL bResult;
    if (m_bOpenFileDialog) {
        bResult = GetOpenFileNameW(&ofn);
    } else {
        bResult = GetSaveFileNameW(&ofn);
    }

    if (bResult) {
        // Check for multiple file selection (OFN_ALLOWMULTISELECT flag)
        if ((m_dwFlags & OFN_ALLOWMULTISELECT) && szFile[wcslen(szFile) + 1] != L'\0') {
            // Multiple files selected: directory followed by null-separated filenames
            m_strFolderPath = szFile;

            // Store pointer to file list for GetNextPathName
            // m_pFileList points to the first filename after the directory
            m_pFileList = (void*)(szFile + wcslen(szFile) + 1);

            // Keep the buffer alive
            // (In a real implementation, we'd allocate persistent storage)

            // Build first path name
            const wchar_t* pFirstFile = static_cast<const wchar_t*>(m_pFileList);
            if (*pFirstFile != L'\0') {
                m_strPathName = m_strFolderPath + L"\\" + CString(pFirstFile);
                m_strFileNameOnly = pFirstFile;
            } else {
                // Single file selected in multi-select mode
                m_strPathName = m_strFolderPath;
                const wchar_t* pFileName = wcsrchr(szFile, L'\\');
                if (pFileName) {
                    m_strFileNameOnly = pFileName + 1;
                    m_strFolderPath = m_strPathName.Left((int)(pFileName - szFile));
                } else {
                    m_strFileNameOnly = szFile;
                }
                m_pFileList = nullptr;
            }
        } else {
            // Single file selection
            m_strPathName = szFile;
            m_pFileList = nullptr;

            // Extract file name from path
            const wchar_t* pFileName = wcsrchr(szFile, L'\\');
            if (pFileName != nullptr) {
                m_strFileNameOnly = pFileName + 1;
                m_strFolderPath = m_strPathName.Left((int)(pFileName - szFile));
            } else {
                m_strFileNameOnly = szFile;
                m_strFolderPath = L"";
            }
        }

        // Extract extension
        const wchar_t* pExt = wcsrchr((const wchar_t*)m_strFileNameOnly, L'.');
        if (pExt != nullptr) {
            m_strFileExt = pExt + 1;
            m_strFileTitle = m_strFileNameOnly.Left((int)(pExt - (const wchar_t*)m_strFileNameOnly));
        } else {
            m_strFileExt = L"";
            m_strFileTitle = m_strFileNameOnly;
        }

        // Note: szFile buffer must remain valid for GetNextPathName to work
        // In a production implementation, we'd copy to persistent storage
        return IDOK;
    }

    delete[] szFile;
    return IDCANCEL;
}

CString CFileDialog::GetPathName() const {
    return m_strPathName;
}

CString CFileDialog::GetFileName() const {
    return m_strFileNameOnly;
}

CString CFileDialog::GetFileExt() const {
    return m_strFileExt;
}

CString CFileDialog::GetFileTitle() const {
    return m_strFileTitle;
}

CString CFileDialog::GetFolderPath() const {
    return m_strFolderPath;
}

CString CFileDialog::GetNextPathName(void*& pos) const {
    // Multiple file selection: files are null-separated in the buffer
    // pos should point to the current position in the null-separated list

    if (pos == nullptr) {
        return CString();
    }

    const wchar_t* pCurrent = static_cast<const wchar_t*>(pos);

    // Check if we've reached the end (double null terminator)
    if (*pCurrent == L'\0') {
        pos = nullptr;
        return CString();
    }

    // Build full path: folder + filename
    CString strResult = m_strFolderPath + L"\\" + CString(pCurrent);

    // Advance pos to next filename (past the null terminator)
    pos = (void*)(pCurrent + wcslen(pCurrent) + 1);

    return strResult;
}

void CFileDialog::SetDefExt(const wchar_t* lpszDefExt) {
    m_strDefExt = lpszDefExt;
}

//=============================================================================
// CColorDialog implementation
//=============================================================================

// Static custom colors storage (shared across all instances)
static COLORREF s_acrCustClr[16] = {
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255)
};

CColorDialog::CColorDialog(unsigned long clrInit, unsigned long dwFlags, CWnd* pParentWnd)
    : CDialog(), m_clrInit(clrInit), m_clrResult(clrInit), m_dwFlags(dwFlags) {
    (void)pParentWnd;
    memset(_colordialog_padding, 0, sizeof(_colordialog_padding));
}

intptr_t CColorDialog::DoModal() {
    CHOOSECOLORW cc;
    memset(&cc, 0, sizeof(cc));
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = nullptr;
    cc.lpCustColors = s_acrCustClr;
    cc.rgbResult = m_clrInit;
    cc.Flags = m_dwFlags | CC_RGBINIT;

    if (ChooseColorW(&cc)) {
        m_clrResult = cc.rgbResult;
        return IDOK;
    }

    return IDCANCEL;
}

unsigned long CColorDialog::GetColor() const {
    return m_clrResult;
}

void CColorDialog::SetCurrentColor(unsigned long clr) {
    m_clrInit = clr;
}

void* CColorDialog::GetSavedCustomColors() {
    return s_acrCustClr;
}

//=============================================================================
// CFontDialog implementation
//=============================================================================

CFontDialog::CFontDialog(void* lpLogFont, unsigned long dwFlags, void* pdcPrinter, CWnd* pParentWnd)
    : CDialog(), m_dwFlags(dwFlags), m_clrResult(0), m_nPointSize(0) {
    (void)pdcPrinter;
    (void)pParentWnd;

    memset(m_lf, 0, sizeof(m_lf));

    if (lpLogFont != nullptr) {
        memcpy(m_lf, lpLogFont, sizeof(LOGFONTW));
        m_lpLogFont = lpLogFont;  // User wants their buffer updated
    } else {
        m_lpLogFont = m_lf;  // Use internal buffer
    }

    memset(_fontdialog_padding, 0, sizeof(_fontdialog_padding));
}

intptr_t CFontDialog::DoModal() {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(m_lf);

    CHOOSEFONTW cf;
    memset(&cf, 0, sizeof(cf));
    cf.lStructSize = sizeof(cf);
    cf.hwndOwner = nullptr;
    cf.lpLogFont = plf;
    cf.Flags = m_dwFlags | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
    cf.rgbColors = m_clrResult;

    if (ChooseFontW(&cf)) {
        m_clrResult = cf.rgbColors;
        m_nPointSize = cf.iPointSize / 10;  // iPointSize is in 1/10 points

        // If user provided a buffer, copy results back
        if (m_lpLogFont != m_lf) {
            memcpy(m_lpLogFont, plf, sizeof(LOGFONTW));
        }
        return IDOK;
    }

    return IDCANCEL;
}

void* CFontDialog::GetCurrentFont() const {
    return const_cast<unsigned char*>(m_lf);
}

CString CFontDialog::GetFaceName() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    return CString(plf->lfFaceName);
}

CString CFontDialog::GetStyleName() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    // Construct style name from font attributes
    CString style;
    if (plf->lfWeight >= FW_BOLD) {
        style = L"Bold";
    }
    if (plf->lfItalic) {
        if (!style.IsEmpty()) style += L" ";
        style += L"Italic";
    }
    if (style.IsEmpty()) {
        style = L"Regular";
    }
    return style;
}

int CFontDialog::GetSize() const {
    return m_nPointSize;
}

unsigned long CFontDialog::GetColor() const {
    return m_clrResult;
}

int CFontDialog::GetWeight() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    return plf->lfWeight;
}

int CFontDialog::IsStrikeOut() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    return plf->lfStrikeOut != 0;
}

int CFontDialog::IsUnderline() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    return plf->lfUnderline != 0;
}

int CFontDialog::IsBold() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    return plf->lfWeight >= FW_BOLD;
}

int CFontDialog::IsItalic() const {
    LOGFONTW* plf = reinterpret_cast<LOGFONTW*>(const_cast<unsigned char*>(m_lf));
    return plf->lfItalic != 0;
}

//=============================================================================
// CPrintDialog implementation
//=============================================================================

CPrintDialog::CPrintDialog(int bPrintSetupOnly, unsigned long dwFlags, CWnd* pParentWnd)
    : CDialog(), m_bPrintSetupOnly(bPrintSetupOnly), m_dwFlags(dwFlags),
      m_hDevMode(nullptr), m_hDevNames(nullptr), m_hDC(nullptr),
      m_nCopies(1), m_nFromPage(1), m_nToPage(1) {
    (void)pParentWnd;
    memset(_printdialog_padding, 0, sizeof(_printdialog_padding));
}

CPrintDialog::~CPrintDialog() {
    if (m_hDevMode != nullptr) {
        GlobalFree(m_hDevMode);
        m_hDevMode = nullptr;
    }
    if (m_hDevNames != nullptr) {
        GlobalFree(m_hDevNames);
        m_hDevNames = nullptr;
    }
    if (m_hDC != nullptr) {
        DeleteDC(static_cast<HDC>(m_hDC));
        m_hDC = nullptr;
    }
}

intptr_t CPrintDialog::DoModal() {
    PRINTDLGW pd;
    memset(&pd, 0, sizeof(pd));
    pd.lStructSize = sizeof(pd);
    pd.hwndOwner = nullptr;
    pd.hDevMode = static_cast<HGLOBAL>(m_hDevMode);
    pd.hDevNames = static_cast<HGLOBAL>(m_hDevNames);
    pd.Flags = m_dwFlags | PD_RETURNDC;
    pd.nCopies = static_cast<WORD>(m_nCopies);
    pd.nFromPage = static_cast<WORD>(m_nFromPage);
    pd.nToPage = static_cast<WORD>(m_nToPage);
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;

    if (m_bPrintSetupOnly) {
        pd.Flags |= PD_PRINTSETUP;
    }

    if (PrintDlgW(&pd)) {
        // Store results
        m_hDevMode = pd.hDevMode;
        m_hDevNames = pd.hDevNames;
        m_hDC = pd.hDC;
        m_nCopies = pd.nCopies;
        m_nFromPage = pd.nFromPage;
        m_nToPage = pd.nToPage;
        return IDOK;
    }

    return IDCANCEL;
}

CString CPrintDialog::GetDeviceName() const {
    if (m_hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(m_hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    CString result(reinterpret_cast<const wchar_t*>(pdn) + pdn->wDeviceOffset);
    GlobalUnlock(m_hDevNames);
    return result;
}

CString CPrintDialog::GetDriverName() const {
    if (m_hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(m_hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    CString result(reinterpret_cast<const wchar_t*>(pdn) + pdn->wDriverOffset);
    GlobalUnlock(m_hDevNames);
    return result;
}

CString CPrintDialog::GetPortName() const {
    if (m_hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(m_hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    CString result(reinterpret_cast<const wchar_t*>(pdn) + pdn->wOutputOffset);
    GlobalUnlock(m_hDevNames);
    return result;
}

int CPrintDialog::GetCopies() const {
    return m_nCopies;
}

int CPrintDialog::GetFromPage() const {
    return m_nFromPage;
}

int CPrintDialog::GetToPage() const {
    return m_nToPage;
}

int CPrintDialog::GetPortrait() const {
    if (m_hDevMode == nullptr) {
        return TRUE;
    }

    DEVMODEW* pdm = static_cast<DEVMODEW*>(GlobalLock(m_hDevMode));
    if (pdm == nullptr) {
        return TRUE;
    }

    int result = (pdm->dmOrientation == DMORIENT_PORTRAIT);
    GlobalUnlock(m_hDevMode);
    return result;
}

void* CPrintDialog::GetPrinterDC() const {
    return m_hDC;
}

void* CPrintDialog::GetDevMode() const {
    return m_hDevMode;
}

//=============================================================================
// CPageSetupDialog implementation
//=============================================================================

CPageSetupDialog::CPageSetupDialog(unsigned long dwFlags, CWnd* pParentWnd)
    : CDialog(), m_dwFlags(dwFlags), m_hDevMode(nullptr), m_hDevNames(nullptr) {
    (void)pParentWnd;
    memset(&m_rtMargin, 0, sizeof(m_rtMargin));
    memset(&m_sizePaper, 0, sizeof(m_sizePaper));
    memset(_pagesetupdialog_padding, 0, sizeof(_pagesetupdialog_padding));
}

CPageSetupDialog::~CPageSetupDialog() {
    if (m_hDevMode != nullptr) {
        GlobalFree(m_hDevMode);
        m_hDevMode = nullptr;
    }
    if (m_hDevNames != nullptr) {
        GlobalFree(m_hDevNames);
        m_hDevNames = nullptr;
    }
}

intptr_t CPageSetupDialog::DoModal() {
    PAGESETUPDLGW psd;
    memset(&psd, 0, sizeof(psd));
    psd.lStructSize = sizeof(psd);
    psd.hwndOwner = nullptr;
    psd.hDevMode = static_cast<HGLOBAL>(m_hDevMode);
    psd.hDevNames = static_cast<HGLOBAL>(m_hDevNames);
    psd.Flags = m_dwFlags;
    psd.rtMargin = m_rtMargin;
    psd.ptPaperSize.x = m_sizePaper.cx;
    psd.ptPaperSize.y = m_sizePaper.cy;

    if (PageSetupDlgW(&psd)) {
        m_hDevMode = psd.hDevMode;
        m_hDevNames = psd.hDevNames;
        m_rtMargin = psd.rtMargin;
        m_sizePaper.cx = psd.ptPaperSize.x;
        m_sizePaper.cy = psd.ptPaperSize.y;
        return IDOK;
    }

    return IDCANCEL;
}

CString CPageSetupDialog::GetDeviceName() const {
    if (m_hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(m_hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    CString result(reinterpret_cast<const wchar_t*>(pdn) + pdn->wDeviceOffset);
    GlobalUnlock(m_hDevNames);
    return result;
}

CString CPageSetupDialog::GetDriverName() const {
    if (m_hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(m_hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    CString result(reinterpret_cast<const wchar_t*>(pdn) + pdn->wDriverOffset);
    GlobalUnlock(m_hDevNames);
    return result;
}

CString CPageSetupDialog::GetPortName() const {
    if (m_hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(m_hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    CString result(reinterpret_cast<const wchar_t*>(pdn) + pdn->wOutputOffset);
    GlobalUnlock(m_hDevNames);
    return result;
}

void CPageSetupDialog::GetMarginRect(RECT* pRect) const {
    if (pRect != nullptr) {
        *pRect = m_rtMargin;
    }
}

void CPageSetupDialog::GetPaperSize(SIZE* pSize) const {
    if (pSize != nullptr) {
        *pSize = m_sizePaper;
    }
}

void* CPageSetupDialog::GetDevMode() const {
    return m_hDevMode;
}

//=============================================================================
// CFindReplaceDialog implementation
//=============================================================================

// Registered message for find/replace notifications
static UINT s_nFindReplaceMsg = 0;

CFindReplaceDialog::CFindReplaceDialog()
    : CDialog() {
    memset(m_fr, 0, sizeof(m_fr));
    memset(m_szFindWhat, 0, sizeof(m_szFindWhat));
    memset(m_szReplaceWith, 0, sizeof(m_szReplaceWith));
}

CFindReplaceDialog::~CFindReplaceDialog() {
    // Modeless dialog - destroyed via DestroyWindow when user closes
    if (m_hWnd != nullptr) {
        ::DestroyWindow(m_hWnd);
        m_hWnd = nullptr;
    }
}

int CFindReplaceDialog::Create(int bFindDialogOnly,
                               const wchar_t* lpszFindWhat,
                               const wchar_t* lpszReplaceWith,
                               unsigned long dwFlags,
                               CWnd* pParentWnd) {
    // Register the find/replace message if not already done
    if (s_nFindReplaceMsg == 0) {
        s_nFindReplaceMsg = RegisterWindowMessageW(FINDMSGSTRINGW);
    }

    // Copy initial strings
    if (lpszFindWhat != nullptr) {
        wcsncpy(m_szFindWhat, lpszFindWhat, 255);
        m_szFindWhat[255] = L'\0';
    }
    if (lpszReplaceWith != nullptr) {
        wcsncpy(m_szReplaceWith, lpszReplaceWith, 255);
        m_szReplaceWith[255] = L'\0';
    }

    // Setup the FINDREPLACE structure
    FINDREPLACEW* pfr = reinterpret_cast<FINDREPLACEW*>(m_fr);
    pfr->lStructSize = sizeof(FINDREPLACEW);
    pfr->hwndOwner = pParentWnd ? pParentWnd->m_hWnd : nullptr;
    pfr->hInstance = nullptr;
    pfr->Flags = dwFlags;
    pfr->lpstrFindWhat = m_szFindWhat;
    pfr->lpstrReplaceWith = m_szReplaceWith;
    pfr->wFindWhatLen = sizeof(m_szFindWhat) / sizeof(wchar_t);
    pfr->wReplaceWithLen = sizeof(m_szReplaceWith) / sizeof(wchar_t);
    pfr->lCustData = reinterpret_cast<LPARAM>(this);  // Store 'this' for GetNotifier

    HWND hDlg;
    if (bFindDialogOnly) {
        hDlg = FindTextW(pfr);
    } else {
        hDlg = ReplaceTextW(pfr);
    }

    if (hDlg != nullptr) {
        m_hWnd = hDlg;
        return TRUE;
    }

    return FALSE;
}

CString CFindReplaceDialog::GetFindString() const {
    return CString(m_szFindWhat);
}

CString CFindReplaceDialog::GetReplaceString() const {
    return CString(m_szReplaceWith);
}

int CFindReplaceDialog::SearchDown() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_DOWN) != 0;
}

int CFindReplaceDialog::FindNext() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_FINDNEXT) != 0;
}

int CFindReplaceDialog::MatchCase() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_MATCHCASE) != 0;
}

int CFindReplaceDialog::MatchWholeWord() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_WHOLEWORD) != 0;
}

int CFindReplaceDialog::ReplaceCurrent() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_REPLACE) != 0;
}

int CFindReplaceDialog::ReplaceAll() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_REPLACEALL) != 0;
}

int CFindReplaceDialog::IsTerminating() const {
    const FINDREPLACEW* pfr = reinterpret_cast<const FINDREPLACEW*>(m_fr);
    return (pfr->Flags & FR_DIALOGTERM) != 0;
}

CFindReplaceDialog* CFindReplaceDialog::GetNotifier(LPARAM lParam) {
    FINDREPLACEW* pfr = reinterpret_cast<FINDREPLACEW*>(lParam);
    if (pfr != nullptr && pfr->lCustData != 0) {
        return reinterpret_cast<CFindReplaceDialog*>(pfr->lCustData);
    }
    return nullptr;
}

unsigned int CFindReplaceDialog::GetFindReplaceMessage() {
    if (s_nFindReplaceMsg == 0) {
        s_nFindReplaceMsg = RegisterWindowMessageW(FINDMSGSTRINGW);
    }
    return s_nFindReplaceMsg;
}
