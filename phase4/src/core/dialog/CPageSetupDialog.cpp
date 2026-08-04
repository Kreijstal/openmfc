// CPageSetupDialog — OpenMFC implementation.
// Sources: dlgcommon.cpp, global_ctordtor_param.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















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
// Symbol: ?CreatePrinterDC@CPageSetupDialog@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__CreatePrinterDC_CPageSetupDialog__QEAAPEAUHDC____XZ(CPageSetupDialog* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    auto* access = static_cast<CPageSetupDialogAccess*>(pThis);
    if (access->m_hDevMode == nullptr || access->m_hDevNames == nullptr) {
        return nullptr;
    }

    DEVMODEW* pDevMode = static_cast<DEVMODEW*>(GlobalLock(access->m_hDevMode));
    DEVNAMES* pDevNames = static_cast<DEVNAMES*>(GlobalLock(access->m_hDevNames));
    if (pDevMode == nullptr || pDevNames == nullptr) {
        if (pDevMode != nullptr) {
            GlobalUnlock(access->m_hDevMode);
        }
        if (pDevNames != nullptr) {
            GlobalUnlock(access->m_hDevNames);
        }
        return nullptr;
    }

    const wchar_t* pDriver = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wDriverOffset;
    const wchar_t* pDevice = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wDeviceOffset;
    const wchar_t* pOutput = reinterpret_cast<const wchar_t*>(pDevNames) + pDevNames->wOutputOffset;
    HDC hDC = CreateDCW(pDriver, pDevice, pOutput, pDevMode);

    GlobalUnlock(access->m_hDevNames);
    GlobalUnlock(access->m_hDevMode);
    return hDC;
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
// Symbol: ?GetMargins@CPageSetupDialog@@QEBAXPEAUtagRECT@@0@Z
extern "C" void MS_ABI impl__GetMargins_CPageSetupDialog__QEBAXPEAUtagRECT__0_Z(
    const CPageSetupDialog* pThis, RECT* lpRectMargins, RECT* lpRectMinMargins) {
    RECT margins{};
    if (pThis != nullptr) {
        const auto* access = static_cast<const CPageSetupDialogAccess*>(pThis);
        margins = access->m_rtMargin;
    }
    if (lpRectMargins != nullptr) {
        *lpRectMargins = margins;
    }
    if (lpRectMinMargins != nullptr) {
        memset(lpRectMinMargins, 0, sizeof(*lpRectMinMargins));
    }
}
// Symbol: ?GetRuntimeClass@CPageSetupDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPageSetupDialog__UEBAPEAUCRuntimeClass__XZ(
    const CPageSetupDialog* pThis) {
    return CPageSetupDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CPageSetupDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPageSetupDialog__SAPEAUCRuntimeClass__XZ() {
    return CPageSetupDialog::GetThisClass();
}
// Symbol: ?OnDrawPage@CPageSetupDialog@@UEAAIPEAVCDC@@IPEAUtagRECT@@@Z
extern "C" unsigned int MS_ABI impl__OnDrawPage_CPageSetupDialog__UEAAIPEAVCDC__IPEAUtagRECT___Z(
    CPageSetupDialog* pThis, CDC* pDC, unsigned int nMessage, RECT* lpRect) {
    (void)pThis;
    (void)pDC;
    (void)nMessage;
    (void)lpRect;
    return 0;
}
// Symbol: ?PaintHookProc@CPageSetupDialog@@KAIPEAUHWND__@@I_K_J@Z
extern "C" unsigned int MS_ABI impl__PaintHookProc_CPageSetupDialog__KAIPEAUHWND____I_K_J_Z(
    HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    (void)hWnd;
    (void)message;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?PreDrawPage@CPageSetupDialog@@UEAAIGGPEAUtagPSDW@@@Z
extern "C" unsigned int MS_ABI impl__PreDrawPage_CPageSetupDialog__UEAAIGGPEAUtagPSDW___Z(
    CPageSetupDialog* pThis, unsigned short wPaper, unsigned short wFlags, PAGESETUPDLGW* pPSD) {
    (void)pThis;
    (void)wPaper;
    (void)wFlags;
    (void)pPSD;
    return 0;
}
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
// Symbol: ??0CPageSetupDialog@@QEAA@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CPageSetupDialog__QEAA_KPEAVCWnd___Z(
        void* p, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) CPageSetupDialog(dwFlags, pParentWnd);
}
