// CPrintDialogEx — OpenMFC implementation.
// Sources: dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// Symbol: ??0CPrintDialogEx@@QEAA@KPEAVCWnd@@@Z
// CPrintDialogEx::CPrintDialogEx(DWORD dwFlags, CWnd* pParentWnd)
extern "C" void* MS_ABI impl___0CPrintDialogEx__QEAA_KPEAVCWnd___Z(unsigned long dwFlags, CWnd* pParentWnd) {
    return new CPrintDialogEx(dwFlags, pParentWnd);
}
// Symbol: ?GetDeviceName@CPrintDialogEx@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDeviceName_CPrintDialogEx__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CPrintDialogEx* pThis) {
    new(__ret) CString(pThis ? pThis->GetDeviceName() : CString());
}
// Symbol: ?GetDriverName@CPrintDialogEx@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetDriverName_CPrintDialogEx__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CPrintDialogEx* pThis) {
    new(__ret) CString(pThis ? pThis->GetDriverName() : CString());
}
// Symbol: ?GetPortName@CPrintDialogEx@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
extern "C" void MS_ABI impl__GetPortName_CPrintDialogEx__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CPrintDialogEx* pThis) {
    new(__ret) CString(pThis ? pThis->GetPortName() : CString());
}
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
// Symbol: ?AddRef@CPrintDialogEx@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__AddRef_CPrintDialogEx__UEAAKXZ() {
    return 0;
}

// Symbol: ?DefWindowProcW@CPrintDialogEx@@UEAA_JI_K_J@Z
extern "C" __int64 MS_ABI impl__DefWindowProcW_CPrintDialogEx__UEAA_JI_K_J_Z(unsigned int p0, unsigned __int64 p1, __int64 p2) {
    return 0;
}

// Symbol: ?GetDefaults@CPrintDialogEx@@QEAAHXZ
extern "C" int MS_ABI impl__GetDefaults_CPrintDialogEx__QEAAHXZ() {
    return 0;
}

// Symbol: ?GetSite@CPrintDialogEx@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__GetSite_CPrintDialogEx__UEAAJAEBU_GUID__PEAPEAX_Z(const void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?HandleInitDialog@CPrintDialogEx@@QEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__HandleInitDialog_CPrintDialogEx__QEAA_J_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Symbol: ?HandleMessage@CPrintDialogEx@@UEAAJPEAUHWND__@@I_K_JPEA_J@Z
extern "C" long MS_ABI impl__HandleMessage_CPrintDialogEx__UEAAJPEAUHWND____I_K_JPEA_J_Z(void* /*struct*/* p0, unsigned int p1, unsigned __int64 p2, __int64 p3, __int64* p4) {
    return 0;
}

// Symbol: ?InitDone@CPrintDialogEx@@UEAAJXZ
extern "C" long MS_ABI impl__InitDone_CPrintDialogEx__UEAAJXZ() {
    return 0;
}

// Symbol: ?PostModal@CPrintDialogEx@@QEAAXXZ
extern "C" void MS_ABI impl__PostModal_CPrintDialogEx__QEAAXXZ() {}

// Symbol: ?PreModal@CPrintDialogEx@@QEAAPEAUHWND__@@XZ
extern "C" void* MS_ABI impl__PreModal_CPrintDialogEx__QEAAPEAUHWND____XZ() {
    return nullptr;
}

// Symbol: ?QueryInterface@CPrintDialogEx@@UEAAJAEBU_GUID@@PEAPEAX@Z
extern "C" long MS_ABI impl__QueryInterface_CPrintDialogEx__UEAAJAEBU_GUID__PEAPEAX_Z(const void* /*struct*/* p0, void** p1) {
    return 0;
}

// Symbol: ?Release@CPrintDialogEx@@UEAAKXZ
extern "C" unsigned long MS_ABI impl__Release_CPrintDialogEx__UEAAKXZ() {
    return 0;
}

// Symbol: ?SelectionChange@CPrintDialogEx@@UEAAJXZ
extern "C" long MS_ABI impl__SelectionChange_CPrintDialogEx__UEAAJXZ() {
    return 0;
}

// Symbol: ?SetSite@CPrintDialogEx@@UEAAJPEAUIUnknown@@@Z
extern "C" long MS_ABI impl__SetSite_CPrintDialogEx__UEAAJPEAUIUnknown___Z(void* /*struct*/* p0) {
    return 0;
}

