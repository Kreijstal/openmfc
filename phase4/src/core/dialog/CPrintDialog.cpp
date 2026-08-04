// CPrintDialog — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// Symbol: ??0CPrintDialog@@IEAA@AEAUtagPDW@@@Z
extern "C" CPrintDialog* MS_ABI impl___0CPrintDialog__IEAA_AEAUtagPDW___Z(CPrintDialog* pThis, tagPDW& pd) {
    if (!pThis) return nullptr;

    CPrintDialog* p = new(pThis) CPrintDialog(
        (pd.Flags & PD_PRINTSETUP) ? 1 : 0,
        pd.Flags,
        nullptr
    );
    CPrintDialogAccess* acc = reinterpret_cast<CPrintDialogAccess*>(p);
    acc->m_hDevMode = pd.hDevMode;
    acc->m_hDevNames = pd.hDevNames;
    acc->m_hDC = pd.hDC;
    acc->m_nCopies = pd.nCopies == 0 ? 1 : pd.nCopies;
    acc->m_nFromPage = pd.nFromPage == 0 ? 1 : pd.nFromPage;
    acc->m_nToPage = pd.nToPage;
    return p;
}
// Symbol: ??0CPrintDialog@@QEAA@HKPEAVCWnd@@@Z
extern "C" CPrintDialog* MS_ABI impl___0CPrintDialog__QEAA_HKPEAVCWnd___Z(CPrintDialog* pThis, int bPrintSetupOnly, unsigned long dwFlags, CWnd* pParentWnd) {
    if (!pThis) return nullptr;
    return new(pThis) CPrintDialog(bPrintSetupOnly, dwFlags, pParentWnd);
}
// Symbol: ?AttachOnSetup@CPrintDialog@@MEAAPEAV1@XZ
extern "C" CPrintDialog* MS_ABI impl__AttachOnSetup_CPrintDialog__MEAAPEAV1_XZ(CPrintDialog* pThis) {
    return pThis;
}
// Symbol: ?CreatePrinterDC@CPrintDialog@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__CreatePrinterDC_CPrintDialog__QEAAPEAUHDC____XZ(CPrintDialog* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }

    auto* access = static_cast<CPrintDialogAccess*>(pThis);
    if (access->m_hDC != nullptr) {
        return static_cast<HDC>(access->m_hDC);
    }
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

    access->m_hDC = hDC;
    return hDC;
}
// Symbol: ?GetDefaults@CPrintDialog@@QEAAHXZ
extern "C" int MS_ABI impl__GetDefaults_CPrintDialog__QEAAHXZ(CPrintDialog* pThis) {
    if (pThis == nullptr) {
        return FALSE;
    }

    auto* access = static_cast<CPrintDialogAccess*>(pThis);
    PRINTDLGW pd{};
    pd.lStructSize = sizeof(pd);
    pd.Flags = (access->m_dwFlags & ~(PD_RETURNDC | PD_RETURNIC | PD_PRINTSETUP)) | PD_RETURNDEFAULT;
    pd.hDevMode = static_cast<HGLOBAL>(access->m_hDevMode);
    pd.hDevNames = static_cast<HGLOBAL>(access->m_hDevNames);
    pd.nCopies = static_cast<WORD>(access->m_nCopies);
    pd.nFromPage = static_cast<WORD>(access->m_nFromPage);
    pd.nToPage = static_cast<WORD>(access->m_nToPage);
    pd.nMinPage = 1;
    pd.nMaxPage = 0xFFFF;

    if (!PrintDlgW(&pd)) {
        return FALSE;
    }

    access->m_hDevMode = pd.hDevMode;
    access->m_hDevNames = pd.hDevNames;
    if (pd.hDC != nullptr) {
        if (access->m_hDC != nullptr && access->m_hDC != pd.hDC) {
            DeleteDC(static_cast<HDC>(access->m_hDC));
        }
        access->m_hDC = pd.hDC;
    }
    access->m_nCopies = pd.nCopies;
    access->m_nFromPage = pd.nFromPage;
    access->m_nToPage = pd.nToPage;
    return TRUE;
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
// Symbol: ?GetMessageMap@CPrintDialog@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CPrintDialog__MEBAPEBUAFX_MSGMAP__XZ(
    const CPrintDialog* pThis) {
    (void)pThis;
    return CDialog::GetThisMessageMap();
}
// Symbol: ?GetRuntimeClass@CPrintDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPrintDialog__UEBAPEAUCRuntimeClass__XZ(
    const CPrintDialog* pThis) {
    return CPrintDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CPrintDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPrintDialog__SAPEAUCRuntimeClass__XZ() {
    return CPrintDialog::GetThisClass();
}
// Symbol: ?GetThisMessageMap@CPrintDialog@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CPrintDialog__KAPEBUAFX_MSGMAP__XZ() {
    return CDialog::GetThisMessageMap();
}
// Symbol: ?OnPrintSetup@CPrintDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPrintSetup_CPrintDialog__IEAAXXZ(CPrintDialog* pThis) {
    if (pThis == nullptr) {
        return;
    }

    auto* access = static_cast<CPrintDialogAccess*>(pThis);
    const int oldPrintSetupOnly = access->m_bPrintSetupOnly;
    access->m_bPrintSetupOnly = TRUE;
    pThis->DoModal();
    access->m_bPrintSetupOnly = oldPrintSetupOnly;
}
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
