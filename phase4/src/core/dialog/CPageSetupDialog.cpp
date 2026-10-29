// CPageSetupDialog — OpenMFC implementation.
// Layout and modal flow cross-checked against retail mfc140u.dll.

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// This file's parameterized-constructor export thunk placement-constructs
// CPageSetupDialog by calling the C++ constructor implemented below.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#include <cstddef>
#include <cstring>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {
// Retail CPageSetupDialog embeds PAGESETUPDLGW at +0x130; constructor RVA
// 0x2189d0 (mfc140u) initializes that 0x80-byte block. The current public
// class declaration's convenience fields do not match these retail offsets.
constexpr size_t kPageSetupDataOffset = 0x130;
static_assert(sizeof(PAGESETUPDLGW) == 0x80, "PAGESETUPDLGW retail size");
static_assert(offsetof(PAGESETUPDLGW, hDevMode) == 0x10, "PAGESETUPDLGW hDevMode offset");
static_assert(offsetof(PAGESETUPDLGW, hDevNames) == 0x18, "PAGESETUPDLGW hDevNames offset");
static_assert(offsetof(PAGESETUPDLGW, Flags) == 0x20, "PAGESETUPDLGW Flags offset");
static_assert(offsetof(PAGESETUPDLGW, rtMinMargin) == 0x2c, "PAGESETUPDLGW rtMinMargin offset");
static_assert(offsetof(PAGESETUPDLGW, rtMargin) == 0x3c, "PAGESETUPDLGW rtMargin offset");

PAGESETUPDLGW* PageSetupData(CPageSetupDialog* pThis) {
    return reinterpret_cast<PAGESETUPDLGW*>(
        reinterpret_cast<unsigned char*>(pThis) + kPageSetupDataOffset);
}

const PAGESETUPDLGW* PageSetupData(const CPageSetupDialog* pThis) {
    return reinterpret_cast<const PAGESETUPDLGW*>(
        reinterpret_cast<const unsigned char*>(pThis) + kPageSetupDataOffset);
}
}

extern "C" HWND MS_ABI impl__PreModal_CDialog__IEAAPEAUHWND____XZ(CDialog* pThis);
extern "C" void MS_ABI impl__PostModal_CDialog__IEAAXXZ(CDialog* pThis);
extern "C" BOOL MS_ABI impl__AfxHelpEnabled__YAHXZ();
extern "C" void* MS_ABI impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(void* hDevNames, void* hDevMode);


















// Symbol: ?DoModal@CPageSetupDialog@@UEAA_JXZ
// Matches the retail pre/post-modal bracket (mfc140u RVA 0x218a70). OpenMFC
// lacks retail's private page-setup wrapper, so this calls PageSetupDlgW directly.
intptr_t CPageSetupDialog::DoModal() {
    PAGESETUPDLGW* psd = PageSetupData(this);
    psd->hwndOwner = impl__PreModal_CDialog__IEAAPEAUHWND____XZ(this);
    const BOOL result = PageSetupDlgW(psd);
    impl__PostModal_CDialog__IEAAXXZ(this);
    return result ? IDOK : IDCANCEL;
}
// Symbol: ?CreatePrinterDC@CPageSetupDialog@@QEAAPEAUHDC__@@XZ
extern "C" HDC MS_ABI impl__CreatePrinterDC_CPageSetupDialog__QEAAPEAUHDC____XZ(CPageSetupDialog* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    const PAGESETUPDLGW* psd = PageSetupData(pThis);
    return static_cast<HDC>(impl__AfxCreateDC__YAPEAUHDC____PEAX0_Z(
        psd->hDevNames, psd->hDevMode));
}
// Retail's three name accessors GlobalLock DEVNAMES and do not GlobalUnlock
// after copying the string (mfc140u RVAs 0x218b70, 0x218ae0, and 0x218c00).
// Symbol: ?GetDeviceName@CPageSetupDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CPageSetupDialog::GetDeviceName() const {
    HGLOBAL hDevNames = PageSetupData(this)->hDevNames;
    if (hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    return CString(reinterpret_cast<const wchar_t*>(pdn) + pdn->wDeviceOffset);
}
// Symbol: ?GetDriverName@CPageSetupDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CPageSetupDialog::GetDriverName() const {
    HGLOBAL hDevNames = PageSetupData(this)->hDevNames;
    if (hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    return CString(reinterpret_cast<const wchar_t*>(pdn) + pdn->wDriverOffset);
}
// Symbol: ?GetPortName@CPageSetupDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CPageSetupDialog::GetPortName() const {
    HGLOBAL hDevNames = PageSetupData(this)->hDevNames;
    if (hDevNames == nullptr) {
        return CString();
    }

    DEVNAMES* pdn = static_cast<DEVNAMES*>(GlobalLock(hDevNames));
    if (pdn == nullptr) {
        return CString();
    }

    return CString(reinterpret_cast<const wchar_t*>(pdn) + pdn->wOutputOffset);
}
// Symbol: ?GetMargins@CPageSetupDialog@@QEBAXPEAUtagRECT@@0@Z
extern "C" void MS_ABI impl__GetMargins_CPageSetupDialog__QEBAXPEAUtagRECT__0_Z(
    const CPageSetupDialog* pThis, RECT* lpRectMargins, RECT* lpRectMinMargins) {
    const PAGESETUPDLGW* psd = pThis != nullptr ? PageSetupData(pThis) : nullptr;
    if (lpRectMargins != nullptr) {
        *lpRectMargins = psd != nullptr ? psd->rtMargin : RECT{};
    }
    if (lpRectMinMargins != nullptr) {
        *lpRectMinMargins = psd != nullptr ? psd->rtMinMargin : RECT{};
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
// Both retail symbol lookups are unresolved; the header gives the override signature but not its base behavior.
extern "C" unsigned int MS_ABI impl__OnDrawPage_CPageSetupDialog__UEAAIPEAVCDC__IPEAUtagRECT___Z(
    CPageSetupDialog* pThis, CDC* pDC, unsigned int nMessage, RECT* lpRect) {
    (void)pThis;
    (void)pDC;
    (void)nMessage;
    (void)lpRect;
    return 0;
}
// Symbol: ?PaintHookProc@CPageSetupDialog@@KAIPEAUHWND__@@I_K_J@Z
// mfc140.dll RVA 0x2171c0 dispatches through slots 101/102; the current OpenMFC class omits those virtuals, so its vtable cannot safely support this dispatch. The --u lookup has no mapped RVA.
extern "C" unsigned int MS_ABI impl__PaintHookProc_CPageSetupDialog__KAIPEAUHWND____I_K_J_Z(
    HWND hWnd, unsigned int message, unsigned __int64 wParam, __int64 lParam) {
    (void)hWnd;
    (void)message;
    (void)wParam;
    (void)lParam;
    return 0;
}
// Symbol: ?PreDrawPage@CPageSetupDialog@@UEAAIGGPEAUtagPSDW@@@Z
// Both retail symbol lookups are unresolved; the header gives the override signature but not its base behavior.
extern "C" unsigned int MS_ABI impl__PreDrawPage_CPageSetupDialog__UEAAIGGPEAUtagPSDW___Z(
    CPageSetupDialog* pThis, unsigned short wPaper, unsigned short wFlags, PAGESETUPDLGW* pPSD) {
    (void)pThis;
    (void)wPaper;
    (void)wFlags;
    (void)pPSD;
    return 0;
}
CPageSetupDialog::CPageSetupDialog(unsigned long dwFlags, CWnd* pParentWnd)
    : CDialog() {
    (void)pParentWnd;
    auto* psd = PageSetupData(this);
    memset(psd, 0, sizeof(*psd));
    psd->lStructSize = sizeof(*psd);
    // Retail installs private MFC setup/paint hooks and enables both flags.
    // The OpenMFC class has no corresponding virtual slots or setup-hook body,
    // so clear those flags instead of passing an invalid or mis-dispatched hook.
    psd->Flags = dwFlags & ~0x42000UL;
    if (impl__AfxHelpEnabled__YAHXZ()) {
        psd->Flags |= PSD_SHOWHELP;
    }
}
CPageSetupDialog::~CPageSetupDialog() {
    auto* psd = PageSetupData(this);
    if (psd->hDevMode != nullptr) {
        GlobalFree(psd->hDevMode);
        psd->hDevMode = nullptr;
    }
    if (psd->hDevNames != nullptr) {
        GlobalFree(psd->hDevNames);
        psd->hDevNames = nullptr;
    }
}
void CPageSetupDialog::GetMarginRect(RECT* pRect) const {
    if (pRect != nullptr) {
        *pRect = PageSetupData(this)->rtMargin;
    }
}
void CPageSetupDialog::GetPaperSize(SIZE* pSize) const {
    if (pSize != nullptr) {
        const POINT paperSize = PageSetupData(this)->ptPaperSize;
        pSize->cx = paperSize.x;
        pSize->cy = paperSize.y;
    }
}
// Retail GetDevMode returns GlobalLock(hDevMode), not the HGLOBAL handle
// (mfc140u RVA 0x218ac0).
void* CPageSetupDialog::GetDevMode() const {
    HGLOBAL hDevMode = PageSetupData(this)->hDevMode;
    return hDevMode != nullptr ? GlobalLock(hDevMode) : nullptr;
}
// Symbol: ??0CPageSetupDialog@@QEAA@KPEAVCWnd@@@Z
extern "C" void* MS_ABI impl___0CPageSetupDialog__QEAA_KPEAVCWnd___Z(
        void* p, unsigned long dwFlags, CWnd* pParentWnd) {
    return new (p) CPageSetupDialog(dwFlags, pParentWnd);
}
