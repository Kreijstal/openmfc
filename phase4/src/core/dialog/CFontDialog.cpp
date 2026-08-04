// CFontDialog — OpenMFC implementation.
// Sources: dlgcommon.cpp, frame_font_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"
#include "detail/FrameFontExportsSupport.h"

// Export wrappers (placement-new the C++ ctors above; matches .def aliases).
// Symbol: ??0CFontDialog@@QEAA@PEAUtagLOGFONTW@@KPEAVCDC@@PEAVCWnd@@@Z
// Ordinal: 497
// Ctor taking LOGFONTW*
extern "C" CFontDialog* MS_ABI impl___0CFontDialog__QEAA_PEAUtagLOGFONTW__KPEAVCDC__PEAVCWnd___Z(
    CFontDialog* pThis, LOGFONTW* lpLogFont, unsigned long dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) {
    return new(pThis) CFontDialog(lpLogFont, dwFlags, pdcPrinter, pParentWnd);
}
// Symbol: ??0CFontDialog@@QEAA@AEBU_charformatw@@KPEAVCDC@@PEAVCWnd@@@Z
// Ordinal: 496
// Ctor taking const CHARFORMATW&
extern "C" CFontDialog* MS_ABI impl___0CFontDialog__QEAA_AEBU_charformatw__KPEAVCDC__PEAVCWnd___Z(
    CFontDialog* pThis, const CHARFORMATW& cf, unsigned long dwFlags, CDC* pdcPrinter, CWnd* pParentWnd) {
    return new(pThis) CFontDialog(cf, dwFlags, pdcPrinter, pParentWnd);
}
CFontDialog::CFontDialog(LOGFONTW* lpLogFont, unsigned long dwFlags,
                         CDC* pdcPrinter, CWnd* pParentWnd)
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
CFontDialog::CFontDialog(const CHARFORMATW& cf, unsigned long dwFlags,
                         CDC* pdcPrinter, CWnd* pParentWnd)
    : CDialog(), m_dwFlags(dwFlags), m_clrResult(0), m_nPointSize(0) {
    (void)pdcPrinter;
    (void)pParentWnd;

    memset(m_lf, 0, sizeof(m_lf));
    LOGFONTW* lf = reinterpret_cast<LOGFONTW*>(m_lf);

    lf->lfWeight = (cf.dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
    lf->lfItalic      = (BYTE)((cf.dwEffects & CFE_ITALIC) != 0);
    lf->lfUnderline   = (BYTE)((cf.dwEffects & CFE_UNDERLINE) != 0);
    lf->lfStrikeOut   = (BYTE)((cf.dwEffects & CFE_STRIKEOUT) != 0);
    lf->lfCharSet = cf.bCharSet;
    lf->lfPitchAndFamily = cf.bPitchAndFamily;
    if (cf.yHeight > 0) {
        lf->lfHeight = -MulDiv(cf.yHeight, 1, 20);
        m_nPointSize = (int)(cf.yHeight / 20);
    }
    lstrcpynW(lf->lfFaceName, cf.szFaceName, LF_FACESIZE);
    m_clrResult = cf.crTextColor;
    m_lpLogFont = m_lf;

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
// Symbol: ?FillInLogFont@CFontDialog@@QEAAKAEBU_charformatw@@@Z
extern "C" DWORD MS_ABI impl__FillInLogFont_CFontDialog__QEAAKAEBU_charformatw___Z(
    CFontDialog* pThis, const CHARFORMATW* pCharFormat) {
    LOGFONTW* lf = FontLog(pThis);
    if (!lf || !pCharFormat) return 0;

    lf->lfWeight = (pCharFormat->dwEffects & CFE_BOLD) ? FW_BOLD : FW_NORMAL;
    lf->lfItalic = (BYTE)((pCharFormat->dwEffects & CFE_ITALIC) != 0);
    lf->lfUnderline = (BYTE)((pCharFormat->dwEffects & CFE_UNDERLINE) != 0);
    lf->lfStrikeOut = (BYTE)((pCharFormat->dwEffects & CFE_STRIKEOUT) != 0);
    lf->lfCharSet = pCharFormat->bCharSet;
    lf->lfPitchAndFamily = pCharFormat->bPitchAndFamily;
    if (pCharFormat->yHeight > 0) {
        lf->lfHeight = -MulDiv(pCharFormat->yHeight, 1, 20);
        static_cast<CFontDialogAccess*>(pThis)->m_nPointSize = (int)(pCharFormat->yHeight / 20);
    }
    lstrcpynW(lf->lfFaceName, pCharFormat->szFaceName, LF_FACESIZE);
    static_cast<CFontDialogAccess*>(pThis)->m_clrResult = pCharFormat->crTextColor;
    return pCharFormat->dwMask;
}
// Symbol: ?GetCharFormat@CFontDialog@@QEBAXAEAU_charformatw@@@Z
extern "C" void MS_ABI impl__GetCharFormat_CFontDialog__QEBAXAEAU_charformatw___Z(
    const CFontDialog* pThis, CHARFORMATW* pCharFormat) {
    if (!pCharFormat) return;
    const LOGFONTW* lf = FontLog(pThis);
    ZeroMemory(pCharFormat, sizeof(*pCharFormat));
    pCharFormat->cbSize = sizeof(*pCharFormat);
    if (!lf) return;

    pCharFormat->dwMask = CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC |
                          CFM_UNDERLINE | CFM_STRIKEOUT | CFM_COLOR;
    pCharFormat->dwEffects = 0;
    if (lf->lfWeight >= FW_BOLD) pCharFormat->dwEffects |= CFE_BOLD;
    if (lf->lfItalic) pCharFormat->dwEffects |= CFE_ITALIC;
    if (lf->lfUnderline) pCharFormat->dwEffects |= CFE_UNDERLINE;
    if (lf->lfStrikeOut) pCharFormat->dwEffects |= CFE_STRIKEOUT;
    pCharFormat->yHeight = (LONG)(static_cast<const CFontDialogAccess*>(pThis)->m_nPointSize * 20);
    pCharFormat->crTextColor = static_cast<const CFontDialogAccess*>(pThis)->m_clrResult;
    pCharFormat->bCharSet = lf->lfCharSet;
    pCharFormat->bPitchAndFamily = lf->lfPitchAndFamily;
    lstrcpynW(pCharFormat->szFaceName, lf->lfFaceName, LF_FACESIZE);
}
// Symbol: ?GetRuntimeClass@CFontDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFontDialog__UEBAPEAUCRuntimeClass__XZ(
    const CFontDialog* pThis) {
    return CFontDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CFontDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFontDialog__SAPEAUCRuntimeClass__XZ() {
    return CFontDialog::GetThisClass();
}
