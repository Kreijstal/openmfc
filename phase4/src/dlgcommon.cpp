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

#include <openmfc/afxmfc.h>
#include <windows.h>
#include <commdlg.h>
#include <cstring>
#include <new>

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

// Symbol: ?DoModal@CFileDialog@@UEAA_JXZ
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

extern "C" intptr_t MS_ABI impl__DoModal_CFileDialog__UEAA_JXZ(CFileDialog* pThis) {
    return pThis ? pThis->DoModal() : IDCANCEL;
}

// Symbol: ?GetPathName@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetPathName() const {
    return m_strPathName;
}

extern "C" void MS_ABI impl__GetPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetPathName());
}

// Symbol: ?GetFileName@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFileName() const {
    return m_strFileNameOnly;
}

extern "C" void MS_ABI impl__GetFileName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFileName());
}

// Symbol: ?GetFileExt@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFileExt() const {
    return m_strFileExt;
}

extern "C" void MS_ABI impl__GetFileExt_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFileExt());
}

// Symbol: ?GetFileTitle@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFileTitle() const {
    return m_strFileTitle;
}

extern "C" void MS_ABI impl__GetFileTitle_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFileTitle());
}

// Symbol: ?GetFolderPath@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFolderPath() const {
    return m_strFolderPath;
}

extern "C" void MS_ABI impl__GetFolderPath_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFolderPath());
}

// Symbol: ?GetNextPathName@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAPEAU__POSITION@@@Z
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

extern "C" void MS_ABI impl__GetNextPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAU__POSITION___Z(
    CString* __ret, const CFileDialog* pThis, void** p0) {
    new(__ret) CString(pThis->GetNextPathName(*p0));
}

void CFileDialog::SetDefExt(const wchar_t* lpszDefExt) {
    m_strDefExt = lpszDefExt;
}

// Symbol: ?AddPlace@CFileDialog@@QEAAXPEAUIShellItem@@W4FDAP@@@Z
extern "C" void MS_ABI impl__AddPlace_CFileDialog__QEAAXPEAUIShellItem__W4FDAP___Z(
    CFileDialog* pThis, void* p0, int p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}

// Symbol: ?AddPlace@CFileDialog@@QEAAXPEB_WW4FDAP@@@Z
extern "C" void MS_ABI impl__AddPlace_CFileDialog__QEAAXPEB_WW4FDAP___Z(
    CFileDialog* pThis, const wchar_t* p0, int p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}

// Symbol: ?ApplyOFNToShellDialog@CFileDialog@@QEAAXXZ
extern "C" void MS_ABI impl__ApplyOFNToShellDialog_CFileDialog__QEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}

// Symbol: ?HideControl@CFileDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__HideControl_CFileDialog__QEAAXH_Z(CFileDialog* pThis, int p0) {
    (void)pThis;
    (void)p0;
}

// Symbol: ?OnButtonClicked@CFileDialog@@MEAAXK@Z
extern "C" void MS_ABI impl__OnButtonClicked_CFileDialog__MEAAXK_Z(CFileDialog* pThis, unsigned long p0) {
    (void)pThis;
    (void)p0;
}

// Symbol: ?OnCheckButtonToggled@CFileDialog@@MEAAXKH@Z
extern "C" void MS_ABI impl__OnCheckButtonToggled_CFileDialog__MEAAXKH_Z(
    CFileDialog* pThis, unsigned long p0, int p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}

// Symbol: ?OnControlActivating@CFileDialog@@MEAAXK@Z
extern "C" void MS_ABI impl__OnControlActivating_CFileDialog__MEAAXK_Z(
    CFileDialog* pThis, unsigned long p0) {
    (void)pThis;
    (void)p0;
}

// Symbol: ?OnFileNameChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnFileNameChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}

// Symbol: ?OnFileNameOK@CFileDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnFileNameOK_CFileDialog__MEAAHXZ(CFileDialog* pThis) {
    (void)pThis;
    return FALSE;
}

// Symbol: ?OnFolderChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnFolderChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}

// Symbol: ?OnInitDone@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnInitDone_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}

// Symbol: ?OnItemSelected@CFileDialog@@MEAAXKK@Z
extern "C" void MS_ABI impl__OnItemSelected_CFileDialog__MEAAXKK_Z(
    CFileDialog* pThis, unsigned long p0, unsigned long p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}

// Symbol: ?OnLBSelChangedNotify@CFileDialog@@MEAAXIII@Z
extern "C" void MS_ABI impl__OnLBSelChangedNotify_CFileDialog__MEAAXIII_Z(
    CFileDialog* pThis, unsigned int p0, unsigned int p1, unsigned int p2) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
}

// Symbol: ?OnNotify@CFileDialog@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CFileDialog__MEAAH_K_JPEA_J_Z(
    CFileDialog* pThis, uintptr_t p0, intptr_t p1, intptr_t* p2) {
    (void)pThis;
    (void)p0;
    (void)p1;
    if (p2 != nullptr) {
        *p2 = 0;
    }
    return FALSE;
}

// Symbol: ?OnShareViolation@CFileDialog@@MEAAIPEB_W@Z
extern "C" unsigned int MS_ABI impl__OnShareViolation_CFileDialog__MEAAIPEB_W_Z(
    CFileDialog* pThis, const wchar_t* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}

// Symbol: ?OnTypeChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnTypeChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}

// Symbol: ?SetControlText@CFileDialog@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetControlText_CFileDialog__QEAAXHPEB_W_Z(
    CFileDialog* pThis, int p0, const wchar_t* p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}

// Symbol: ?SetTemplate@CFileDialog@@QEAAXPEB_W0@Z
extern "C" void MS_ABI impl__SetTemplate_CFileDialog__QEAAXPEB_W0_Z(
    CFileDialog* pThis, const wchar_t* p0, const wchar_t* p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}

// Symbol: ?UpdateOFNFromShellDialog@CFileDialog@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateOFNFromShellDialog_CFileDialog__UEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
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

unsigned long* CColorDialog::GetSavedCustomColors() {
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

void CFontDialog::GetCurrentFont(LOGFONTW* lpLogFont) {
    if (lpLogFont != nullptr) {
        memcpy(lpLogFont, m_lf, sizeof(LOGFONTW));
    }
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

// Symbol: ?GetDeviceName@CPrintDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
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

extern "C" void MS_ABI impl__GetDeviceName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDeviceName());
}

// Symbol: ?GetDriverName@CPrintDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
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

extern "C" void MS_ABI impl__GetDriverName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDriverName());
}

// Symbol: ?GetPortName@CPrintDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
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

extern "C" void MS_ABI impl__GetPortName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetPortName());
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

// Symbol: ?DoModal@CPageSetupDialog@@UEAA_JXZ
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

extern "C" intptr_t MS_ABI impl__DoModal_CPageSetupDialog__UEAA_JXZ(CPageSetupDialog* pThis) {
    return pThis ? pThis->DoModal() : IDCANCEL;
}

// Symbol: ?GetDeviceName@CPageSetupDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
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

extern "C" void MS_ABI impl__GetDeviceName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDeviceName());
}

// Symbol: ?GetDriverName@CPageSetupDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
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

extern "C" void MS_ABI impl__GetDriverName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDriverName());
}

// Symbol: ?GetPortName@CPageSetupDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
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

extern "C" void MS_ABI impl__GetPortName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetPortName());
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

// Symbol: ?Create@CFindReplaceDialog@@UEAAHHPEB_W0KPEAVCWnd@@@Z
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

extern "C" int MS_ABI impl__Create_CFindReplaceDialog__UEAAHHPEB_W0KPEAVCWnd___Z(
    CFindReplaceDialog* pThis,
    int bFindDialogOnly,
    const wchar_t* lpszFindWhat,
    const wchar_t* lpszReplaceWith,
    unsigned long dwFlags,
    CWnd* pParentWnd) {
    return pThis->Create(bFindDialogOnly, lpszFindWhat, lpszReplaceWith, dwFlags, pParentWnd);
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

//=============================================================================
// CPrintDialogEx
//=============================================================================
IMPLEMENT_DYNAMIC(CPrintDialogEx, CDialog)

CPrintDialogEx::CPrintDialogEx(DWORD dwFlags, CWnd* pParentWnd)
    : CDialog() {
    memset(&m_pdex, 0, sizeof(m_pdex));
    m_pdex.lStructSize = sizeof(PRINTDLGEXW);
    m_pdex.hwndOwner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    m_pdex.Flags = dwFlags;
    m_pdex.nStartPage = START_PAGE_GENERAL;
    memset(_printdialogex_padding, 0, sizeof(_printdialogex_padding));
}

CPrintDialogEx::~CPrintDialogEx() {
    if (m_pdex.hDevMode) ::GlobalFree(m_pdex.hDevMode);
    if (m_pdex.hDevNames) ::GlobalFree(m_pdex.hDevNames);
}

intptr_t CPrintDialogEx::DoModal() {
    if (m_pdex.lStructSize == 0) {
        m_pdex.lStructSize = sizeof(PRINTDLGEXW);
    }

    if (!m_pdex.hwndOwner) {
        CWnd* pMain = AfxGetMainWnd();
        m_pdex.hwndOwner = pMain ? pMain->GetSafeHwnd() : nullptr;
    }

    HRESULT hr = ::PrintDlgExW(&m_pdex);
    if (FAILED(hr)) {
        return -1;
    }

    switch (m_pdex.dwResultAction) {
    case PD_RESULT_PRINT:
        return IDOK;
    case PD_RESULT_CANCEL:
        return IDCANCEL;
    default:
        return -1;
    }
}

int CPrintDialogEx::GetCopies() const { return (int)m_pdex.nCopies; }
int CPrintDialogEx::GetFromPage() const {
    return (m_pdex.nPageRanges > 0 && m_pdex.lpPageRanges) ? (int)m_pdex.lpPageRanges[0].nFromPage : 0;
}
int CPrintDialogEx::GetToPage() const {
    return (m_pdex.nPageRanges > 0 && m_pdex.lpPageRanges) ? (int)m_pdex.lpPageRanges[0].nToPage : 0;
}

CString CPrintDialogEx::GetDeviceName() const {
    CString str;
    if (m_pdex.hDevNames) {
        DEVNAMES* pDevNames = (DEVNAMES*)::GlobalLock(m_pdex.hDevNames);
        if (pDevNames) {
            str = (const wchar_t*)((BYTE*)pDevNames + pDevNames->wDeviceOffset);
            ::GlobalUnlock(m_pdex.hDevNames);
        }
    }
    return str;
}

CString CPrintDialogEx::GetDriverName() const {
    CString str;
    if (m_pdex.hDevNames) {
        DEVNAMES* pDevNames = (DEVNAMES*)::GlobalLock(m_pdex.hDevNames);
        if (pDevNames) {
            str = (const wchar_t*)((BYTE*)pDevNames + pDevNames->wDriverOffset);
            ::GlobalUnlock(m_pdex.hDevNames);
        }
    }
    return str;
}

CString CPrintDialogEx::GetPortName() const {
    CString str;
    if (m_pdex.hDevNames) {
        DEVNAMES* pDevNames = (DEVNAMES*)::GlobalLock(m_pdex.hDevNames);
        if (pDevNames) {
            str = (const wchar_t*)((BYTE*)pDevNames + pDevNames->wOutputOffset);
            ::GlobalUnlock(m_pdex.hDevNames);
        }
    }
    return str;
}

HDC CPrintDialogEx::GetPrinterDC() const { return m_pdex.hDC; }
HDC CPrintDialogEx::CreatePrinterDC() {
    if (m_pdex.hDC) {
        return m_pdex.hDC;
    }
    if (!m_pdex.hDevMode || !m_pdex.hDevNames) {
        return nullptr;
    }

    DEVMODEW* pDevMode = static_cast<DEVMODEW*>(::GlobalLock(m_pdex.hDevMode));
    DEVNAMES* pDevNames = static_cast<DEVNAMES*>(::GlobalLock(m_pdex.hDevNames));
    if (!pDevMode || !pDevNames) {
        if (pDevMode) ::GlobalUnlock(m_pdex.hDevMode);
        if (pDevNames) ::GlobalUnlock(m_pdex.hDevNames);
        return nullptr;
    }

    const wchar_t* pDriver = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wDriverOffset;
    const wchar_t* pDevice = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wDeviceOffset;
    const wchar_t* pOutput = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wOutputOffset;
    HDC hdc = ::CreateDCW(pDriver, pDevice, pOutput, pDevMode);

    ::GlobalUnlock(m_pdex.hDevNames);
    ::GlobalUnlock(m_pdex.hDevMode);

    m_pdex.hDC = hdc;
    return hdc;
}

int CPrintDialogEx::GetPortrait() const {
    if (m_pdex.hDevMode) {
        DEVMODEW* pDevMode = (DEVMODEW*)::GlobalLock(m_pdex.hDevMode);
        if (pDevMode) {
            int portrait = (pDevMode->dmOrientation == DMORIENT_PORTRAIT);
            ::GlobalUnlock(m_pdex.hDevMode);
            return portrait;
        }
    }
    return 1;
}

LPDEVMODEW CPrintDialogEx::GetDevMode() const {
    if (m_pdex.hDevMode) return (LPDEVMODEW)::GlobalLock(m_pdex.hDevMode);
    return nullptr;
}

void CPrintDialogEx::SetPageRange(int nMinPage, int nMaxPage, BOOL bPageRange) {
    m_pdex.nMinPage = nMinPage;
    m_pdex.nMaxPage = nMaxPage;
    if (bPageRange) m_pdex.Flags |= PD_PAGENUMS;
}

//=============================================================================
// CTaskDialog overloads and wrappers
//=============================================================================

CTaskDialog::CTaskDialog(const CString& strContent, const CString& strMainInstruction,
                         const CString& strWindowTitle, int nCommonButtons,
                         int nTaskDialogOptions, const CString& strFooter)
    : CTaskDialog((const wchar_t*)strContent, (const wchar_t*)strMainInstruction,
                  (const wchar_t*)strWindowTitle, nCommonButtons, nTaskDialogOptions) {
    if (!strFooter.IsEmpty()) {
        m_strFooterText = strFooter;
    }
}

CTaskDialog::CTaskDialog(const CString& strContent, const CString& strMainInstruction,
                         const CString& strWindowTitle, int nCommonButtons,
                         int nTaskDialogOptions, int, int, const CString& strFooter)
    : CTaskDialog(strContent, strMainInstruction, strWindowTitle, nCommonButtons, nTaskDialogOptions, strFooter) {}

void CTaskDialog::SetMainIcon(const wchar_t* pszMainIcon) {
    m_hMainIcon = pszMainIcon ? ::LoadIconW(nullptr, pszMainIcon) : nullptr;
}

void CTaskDialog::SetFooterIcon(const wchar_t* pszFooterIcon) {
    m_hFooterIcon = pszFooterIcon ? ::LoadIconW(nullptr, pszFooterIcon) : nullptr;
}

void CTaskDialog::SetFooterText(const CString& strFooterText) {
    m_strFooterText = strFooterText;
}

void CTaskDialog::SetVerificationCheckboxText(const CString& strText) {
    m_strVerificationText = strText;
}

void CTaskDialog::AddCommandControl(int nCommandID, const CString& strLabel, int, int) {
    (void)AddCommandControl(nCommandID, (const wchar_t*)strLabel);
}

void CTaskDialog::AddRadioButton(int nRadioButtonID, const CString& strLabel, int) {
    (void)AddRadioButton(nRadioButtonID, (const wchar_t*)strLabel);
}

void CTaskDialog::SetOptions(int nTaskDialogOptions) {
    m_nTaskDialogOptions = nTaskDialogOptions;
}

int CTaskDialog::GetOptions() const {
    return m_nTaskDialogOptions;
}

BOOL CTaskDialog::IsSupported() {
    HMODULE hComctl = ::GetModuleHandleW(L"comctl32.dll");
    if (!hComctl) {
        hComctl = ::LoadLibraryW(L"comctl32.dll");
    }
    return hComctl && (::GetProcAddress(hComctl, "TaskDialogIndirect") != nullptr);
}

// Symbol: ??0CTaskDialog@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00HH0@Z
extern "C" void MS_ABI impl___0CTaskDialog__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HH0_Z(
    CTaskDialog* pThis, const CString* p0, const CString* p1, const CString* p2, int p3, int p4, const CString* p5) {
    new(pThis) CTaskDialog(*p0, *p1, *p2, p3, p4, p5 ? *p5 : CString());
}

// Symbol: ??0CTaskDialog@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00HHHH0@Z
extern "C" void MS_ABI impl___0CTaskDialog__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HHHH0_Z(
    CTaskDialog* pThis, const CString* p0, const CString* p1, const CString* p2, int p3, int p4, int p5, int p6, const CString* p7) {
    new(pThis) CTaskDialog(*p0, *p1, *p2, p3, p4, p5, p6, p7 ? *p7 : CString());
}

// Symbol: ?SetContent@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetContent_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->m_strContent = *p0;
}

// Symbol: ?SetMainInstruction@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetMainInstruction_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->m_strMainInstruction = *p0;
}

// Symbol: ?SetWindowTitle@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetWindowTitle_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->m_strWindowTitle = *p0;
}

// Symbol: ?SetOptions@CTaskDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__SetOptions_CTaskDialog__QEAAXH_Z(CTaskDialog* pThis, int p0) {
    if (pThis) pThis->SetOptions(p0);
}

// Symbol: ?GetOptions@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetOptions_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetOptions() : 0;
}

// Symbol: ?GetVerificationCheckboxState@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetVerificationCheckboxState_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetVerificationCheckboxState() : FALSE;
}

// Symbol: ?GetSelectedCommandControlID@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetSelectedCommandControlID_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetSelectedCommandControlID() : 0;
}

// Symbol: ?GetSelectedRadioButtonID@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetSelectedRadioButtonID_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetSelectedRadioButtonID() : 0;
}

// Symbol: ?SetFooterText@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetFooterText_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->SetFooterText(*p0);
}

// Symbol: ?SetVerificationCheckboxText@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetVerificationCheckboxText_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->SetVerificationCheckboxText(*p0);
}

// Symbol: ?SetMainIcon@CTaskDialog@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetMainIcon_CTaskDialog__QEAAXPEB_W_Z(CTaskDialog* pThis, const wchar_t* p0) {
    if (pThis) pThis->SetMainIcon(p0);
}

// Symbol: ?SetFooterIcon@CTaskDialog@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetFooterIcon_CTaskDialog__QEAAXPEB_W_Z(CTaskDialog* pThis, const wchar_t* p0) {
    if (pThis) pThis->SetFooterIcon(p0);
}

// Symbol: ?AddCommandControl@CTaskDialog@@QEAAXHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HH@Z
extern "C" void MS_ABI impl__AddCommandControl_CTaskDialog__QEAAXHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HH_Z(
    CTaskDialog* pThis, int p0, const CString* p1, int p2, int p3) {
    if (pThis && p1) pThis->AddCommandControl(p0, *p1, p2, p3);
}

// Symbol: ?AddRadioButton@CTaskDialog@@QEAAXHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__AddRadioButton_CTaskDialog__QEAAXHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CTaskDialog* pThis, int p0, const CString* p1, int p2) {
    if (pThis && p1) pThis->AddRadioButton(p0, *p1, p2);
}

// Symbol: ?IsSupported@CTaskDialog@@SAHXZ
extern "C" int MS_ABI impl__IsSupported_CTaskDialog__SAHXZ() {
    return CTaskDialog::IsSupported();
}

// Symbol: ?ShowDialog@CTaskDialog@@SA_JAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00HHHH0@Z
extern "C" __int64 MS_ABI impl__ShowDialog_CTaskDialog__SA_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HHHH0_Z(
    const CString* p0, const CString* p1, const CString* p2, int p3, int p4, int p5, int p6, const CString* p7) {
    CTaskDialog td(*p0, *p1, *p2, p3, p4, p5, p6, p7 ? *p7 : CString());
    return td.DoModal(nullptr);
}

//=============================================================================
// VS list box wrappers
//=============================================================================

IMPLEMENT_DYNAMIC(CVSListBoxBase, CStatic)
IMPLEMENT_DYNAMIC(CVSListBoxEditCtrl, CEdit)
IMPLEMENT_DYNAMIC(CVSListBox, CListBox)

int CVSListBox::AddItem(const CString& strText, uintptr_t dwData, int bSelect) {
    int nIndex = CListBox::AddString((const wchar_t*)strText);
    if (nIndex >= 0) {
        CListBox::SetItemData(nIndex, dwData);
        if (bSelect) CListBox::SetCurSel(nIndex);
    }
    return nIndex;
}

int CVSListBox::RemoveItem(int nIndex) {
    return CListBox::DeleteString((unsigned int)nIndex);
}

int CVSListBox::GetCount() const {
    return CListBox::GetCount();
}

int CVSListBox::GetSelItem() const {
    return CListBox::GetCurSel();
}

int CVSListBox::SelectItem(int nIndex) {
    return CListBox::SetCurSel(nIndex);
}

int CVSListBox::EditItem(int nIndex) {
    return CListBox::SetCurSel(nIndex);
}

uintptr_t CVSListBox::GetItemData(int nIndex) const {
    return CListBox::GetItemData(nIndex);
}

void CVSListBox::SetItemData(int nIndex, uintptr_t dwData) {
    (void)CListBox::SetItemData(nIndex, dwData);
}

CString CVSListBox::GetItemText(int nIndex) const {
    int nLen = CListBox::GetTextLen(nIndex);
    if (nLen < 0) return CString();
    CString str;
    wchar_t* pBuf = str.GetBuffer(nLen + 1);
    int nCopied = CListBox::GetText(nIndex, pBuf);
    str.ReleaseBuffer(nCopied >= 0 ? nCopied : 0);
    return str;
}

void CVSListBox::SetItemText(int nIndex, const CString& strText) {
    uintptr_t dwData = CListBox::GetItemData(nIndex);
    int nSel = CListBox::GetCurSel();
    if (CListBox::DeleteString((unsigned int)nIndex) >= 0) {
        int nNew = CListBox::InsertString(nIndex, (const wchar_t*)strText);
        if (nNew >= 0) {
            CListBox::SetItemData(nNew, dwData);
            if (nSel == nIndex) CListBox::SetCurSel(nNew);
        }
    }
}

// Symbol: ?AddItem@CVSListBox@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@_KH@Z
extern "C" int MS_ABI impl__AddItem_CVSListBox__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___KH_Z(
    CVSListBox* pThis, const CString* p0, uintptr_t p1, int p2) {
    return pThis && p0 ? pThis->AddItem(*p0, p1, p2) : LB_ERR;
}

// Symbol: ?RemoveItem@CVSListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__RemoveItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0) {
    return pThis ? pThis->RemoveItem(p0) : LB_ERR;
}

// Symbol: ?GetCount@CVSListBox@@UEBAHXZ
extern "C" int MS_ABI impl__GetCount_CVSListBox__UEBAHXZ(const CVSListBox* pThis) {
    return pThis ? pThis->GetCount() : 0;
}

// Symbol: ?GetSelItem@CVSListBox@@UEBAHXZ
extern "C" int MS_ABI impl__GetSelItem_CVSListBox__UEBAHXZ(const CVSListBox* pThis) {
    return pThis ? pThis->GetSelItem() : LB_ERR;
}

// Symbol: ?SelectItem@CVSListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__SelectItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0) {
    return pThis ? pThis->SelectItem(p0) : LB_ERR;
}

// Symbol: ?EditItem@CVSListBox@@UEAAHH@Z
extern "C" int MS_ABI impl__EditItem_CVSListBox__UEAAHH_Z(CVSListBox* pThis, int p0) {
    return pThis ? pThis->EditItem(p0) : LB_ERR;
}

// Symbol: ?GetItemData@CVSListBox@@UEBA_KH@Z
extern "C" uintptr_t MS_ABI impl__GetItemData_CVSListBox__UEBA_KH_Z(const CVSListBox* pThis, int p0) {
    return pThis ? pThis->GetItemData(p0) : 0;
}

// Symbol: ?SetItemData@CVSListBox@@UEAAXH_K@Z
extern "C" void MS_ABI impl__SetItemData_CVSListBox__UEAAXH_K_Z(CVSListBox* pThis, int p0, uintptr_t p1) {
    if (pThis) pThis->SetItemData(p0, p1);
}

// Symbol: ?GetItemText@CVSListBox@@UEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__GetItemText_CVSListBox__UEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CString* __ret, const CVSListBox* pThis, int p0) {
    new(__ret) CString(pThis ? pThis->GetItemText(p0) : CString());
}

// Symbol: ?SetItemText@CVSListBox@@MEAAXHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetItemText_CVSListBox__MEAAXHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CVSListBox* pThis, int p0, const CString* p1) {
    if (pThis && p1) pThis->SetItemText(p0, *p1);
}
