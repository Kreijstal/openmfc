#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"
#include "openmfc/afxinet.h"
#include "openmfc/afxdao.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Destructors for classes whose repo header definitions faithfully model the
// real MFC ABI (thin CWnd/CDialog control wrappers with no extra owning state,
// plus a few value/handle-owning classes with explicit cleanup logic).
//
// Each impl simply invokes the C++ destructor, which performs exactly the work
// the real MFC destructor does (chaining to the base destructor and freeing any
// owned members).  All are null-guarded.
//
// Placeholder stub classes that are only modelled as `char _pad[N]` (uncertain
// real layout) are intentionally left with their weak stubs.

// ---- Thin Win32 control wrappers (CWnd-derived, defaulted destructors) ----

// Symbol: ??1CAnimateCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimateCtrl__UEAA_XZ(CAnimateCtrl* pThis) {
    if (pThis) pThis->~CAnimateCtrl();
}

// Symbol: ??1CComboBoxEx@@UEAA@XZ
extern "C" void MS_ABI impl___1CComboBoxEx__UEAA_XZ(CComboBoxEx* pThis) {
    if (pThis) pThis->~CComboBoxEx();
}

// Symbol: ??1CDateTimeCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CDateTimeCtrl__UEAA_XZ(CDateTimeCtrl* pThis) {
    if (pThis) pThis->~CDateTimeCtrl();
}

// Symbol: ??1CEdit@@UEAA@XZ
extern "C" void MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis) {
    if (pThis) pThis->~CEdit();
}

// Symbol: ??1CStatic@@UEAA@XZ
extern "C" void MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis) {
    if (pThis) pThis->~CStatic();
}

// Symbol: ??1CListBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis) {
    if (pThis) pThis->~CListBox();
}

// Symbol: ??1CHeaderCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CHeaderCtrl__UEAA_XZ(CHeaderCtrl* pThis) {
    if (pThis) pThis->~CHeaderCtrl();
}

// Symbol: ??1CIPAddressCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CIPAddressCtrl__UEAA_XZ(CIPAddressCtrl* pThis) {
    if (pThis) pThis->~CIPAddressCtrl();
}

// Symbol: ??1CListCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CListCtrl__UEAA_XZ(CListCtrl* pThis) {
    if (pThis) pThis->~CListCtrl();
}

// Symbol: ??1CLinkCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CLinkCtrl__UEAA_XZ(CLinkCtrl* pThis) {
    if (pThis) pThis->~CLinkCtrl();
}

// Symbol: ??1CProgressCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CProgressCtrl__UEAA_XZ(CProgressCtrl* pThis) {
    if (pThis) pThis->~CProgressCtrl();
}

// Symbol: ??1CScrollBar@@UEAA@XZ
extern "C" void MS_ABI impl___1CScrollBar__UEAA_XZ(CScrollBar* pThis) {
    if (pThis) pThis->~CScrollBar();
}

// Symbol: ??1CSliderCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CSliderCtrl__UEAA_XZ(CSliderCtrl* pThis) {
    if (pThis) pThis->~CSliderCtrl();
}

// Symbol: ??1CSpinButtonCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CSpinButtonCtrl__UEAA_XZ(CSpinButtonCtrl* pThis) {
    if (pThis) pThis->~CSpinButtonCtrl();
}

// Symbol: ??1CTabCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CTabCtrl__UEAA_XZ(CTabCtrl* pThis) {
    if (pThis) pThis->~CTabCtrl();
}

// ---- Thin CDialog-derived wrappers (defaulted destructors) ----

// Symbol: ??1CFileDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CFileDialog__UEAA_XZ(CFileDialog* pThis) {
    if (pThis) pThis->~CFileDialog();
}

// Symbol: ??1CPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis) {
    if (pThis) pThis->~CPropertyPage();
}

// Symbol: ??1CPropertySheet@@UEAA@XZ
extern "C" void MS_ABI impl___1CPropertySheet__UEAA_XZ(CPropertySheet* pThis) {
    if (pThis) pThis->~CPropertySheet();
}

// ---- Handle/value-owning classes with explicit cleanup ----

// Symbol: ??1CGopherLocator@@UEAA@XZ
extern "C" void MS_ABI impl___1CGopherLocator__UEAA_XZ(CGopherLocator* pThis) {
    if (pThis) pThis->~CGopherLocator();
}

// Symbol: ??1CLongBinary@@UEAA@XZ
extern "C" void MS_ABI impl___1CLongBinary__UEAA_XZ(CLongBinary* pThis) {
    if (pThis) pThis->~CLongBinary();
}

// Symbol: ??1COleControlSiteOrWnd@@QEAA@XZ
extern "C" void MS_ABI impl___1COleControlSiteOrWnd__QEAA_XZ(COleControlSiteOrWnd* pThis) {
    if (pThis) pThis->~COleControlSiteOrWnd();
}
