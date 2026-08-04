// RuntimeClasses — OpenMFC implementation.
// Sources: ctrl_commonctrl_rtti.cpp, global_common_control_rtti.cpp, global_commonctrl2_rtti.cpp, global_docitem_edit_rtti.cpp, global_view_dialog_ctrl_rtti.cpp

#define OPENMFC_APPCORE_IMPL

// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of common
// control wrappers (16 exports, 8 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported
// CRuntimeClass data symbol — only the two getters are exported. The repo has no
// class body for them, so each gets a file-internal CRuntimeClass descriptor
// (schema 0xFFFF = DYNAMIC, no factory) whose m_pBaseClass chains to the real
// base descriptor (classCWnd / classCComboBox / classCEdit), matching the retail
// RUNTIME_CLASS graph that IsKindOf walks. m_nObjectSize is the harvested MSVC
// sizeof (all are 232 — bare CWnd/CComboBox/CEdit subclasses that add no data).
// GetRuntimeClass returns the static descriptor directly (never pThis->GetRuntime
// Class(), which would recurse through this very export in a drop-in client).

#include "openmfc/afxwin.h"   // CWnd, CComboBox, CEdit, CRuntimeClass + DECLARE_DYNAMIC

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID,
    CCreateContext* pContext);

extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
    CWnd* pThis,
    unsigned long dwExStyle,
    const wchar_t* lpszClassName,
    const wchar_t* lpszWindowName,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID,
    void* lpParam);

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CC_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

CC_DESC(CAnimateCtrl,   CWnd,      232);
CC_DESC(CHeaderCtrl,    CWnd,      232);
CC_DESC(CHotKeyCtrl,    CWnd,      232);
CC_DESC(CIPAddressCtrl, CWnd,      232);
CC_DESC(CPagerCtrl,     CWnd,      232);
CC_DESC(CLinkCtrl,      CWnd,      232);
CC_DESC(CComboBoxEx,    CComboBox, 232);
CC_DESC(CNetAddressCtrl, CEdit,    232);
#undef CC_DESC















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
















// ---- Thin CDialog-derived wrappers (defaulted destructors) ----





// ---- Handle/value-owning classes with explicit cleanup ----



// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a second batch of
// common-control / view wrappers (14 exports, 7 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose m_pBaseClass chains to the real base descriptor
// (classCWnd / classCButton / classCView), following the repo's own
// IMPLEMENT_DYNAMIC convention (m_pfnGetBaseClass null, m_pBaseClass set) so
// IsKindOf/IsDerivedFrom walk the correct RUNTIME_CLASS graph. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout:
//   CMonthCalCtrl 232, CReBarCtrl 232, CStatusBarCtrl 232, CToolBarCtrl 232,
//   CRichEditCtrl 232 (all CWnd), CSplitButton 248 (CButton),
//   CTabView 15880 (CView).
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).
//
// Named global_*_rtti.cpp so build_phase4.sh's shard glob compiles it.

#include "openmfc/afxwin.h"   // CWnd / CButton / CView + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define CC2_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

CC2_DESC(CMonthCalCtrl,  CWnd,    232);
CC2_DESC(CReBarCtrl,     CWnd,    232);
CC2_DESC(CStatusBarCtrl, CWnd,    232);
CC2_DESC(CToolBarCtrl,   CWnd,    232);
CC2_DESC(CRichEditCtrl,  CWnd,    232);
CC2_DESC(CSplitButton,   CButton, 248);
CC2_DESC(CTabView,       CView,   15880);
#undef CC2_DESC







// other_mfc shard 03/08 real exports.
//
// Implements the unambiguous RTTI accessors (GetThisClass / GetRuntimeClass)
// for classes whose CRuntimeClass is already defined via IMPLEMENT_DYNAMIC in
// the repo, plus a couple of trivial virtuals with well-defined defaults.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxole.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif








// OpenMFC: RTTI getters (GetThisClass / GetRuntimeClass) for a batch of control,
// view and dialog wrappers (10 exports, 5 classes).
//
// These are MFC DECLARE_DYNAMIC classes with no separately-exported CRuntimeClass
// data symbol — only the two getters are exported. The repo has no class body for
// them, so each gets a file-internal CRuntimeClass descriptor (schema 0xFFFF =
// DYNAMIC, no factory) whose m_pBaseClass chains to the real base descriptor
// (classCWnd / classCButton / classCView / classCDialog / classCFileDialog),
// matching the retail RUNTIME_CLASS graph that IsKindOf walks. m_nObjectSize is
// the real MSVC sizeof harvested with cl.exe /d1reportSingleClassLayout:
//   CDateTimeCtrl 232 (CWnd), CBitmapButton 296 (CButton), CCtrlView 264 (CView),
//   CCommonDialog 304 (CDialog), CFolderPickerDialog 1424 (CFileDialog).
// GetRuntimeClass returns the static descriptor directly (never
// pThis->GetRuntimeClass(), which would recurse through this very export in a
// drop-in client).

#include "openmfc/afxwin.h"   // CWnd/CButton/CView/CDialog/CFileDialog + CRuntimeClass

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// m_lpszClassName, m_nObjectSize, m_wSchema, m_pfnCreateObject,
// m_pfnGetBaseClass, m_pBaseClass, m_pNextClass.
#define VDC_DESC(Cls, Base, Size) \
    CRuntimeClass class##Cls = { #Cls, (Size), 0xFFFF, nullptr, nullptr, &Base::class##Base, nullptr }

VDC_DESC(CDateTimeCtrl,       CWnd,        232);
VDC_DESC(CBitmapButton,       CButton,     296);
VDC_DESC(CCtrlView,           CView,       264);
VDC_DESC(CCommonDialog,       CDialog,     304);
VDC_DESC(CFolderPickerDialog, CFileDialog, 1424);
#undef VDC_DESC





// Symbol: ?GetThisClass@CAnimateCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CAnimateCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCAnimateCtrl;
}
// Symbol: ?GetRuntimeClass@CAnimateCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CAnimateCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCAnimateCtrl;
}
// Symbol: ?Create@CAnimateCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CAnimateCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID)
{
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        static_cast<CWnd*>(pThis),
        L"SysAnimate32",
        nullptr,
        dwStyle,
        rect,
        pParentWnd,
        nID,
        nullptr);
}
// Symbol: ?CreateEx@CAnimateCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CAnimateCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis,
    unsigned long dwExStyle,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID)
{
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
        static_cast<CWnd*>(pThis),
        dwExStyle,
        L"SysAnimate32",
        nullptr,
        dwStyle,
        rect,
        pParentWnd,
        nID,
        nullptr);
}
// Symbol: ?GetThisClass@CHeaderCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHeaderCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCHeaderCtrl;
}
// Symbol: ?GetRuntimeClass@CHeaderCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHeaderCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCHeaderCtrl;
}
// Symbol: ?GetThisClass@CHotKeyCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CHotKeyCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCHotKeyCtrl;
}
// Symbol: ?GetRuntimeClass@CHotKeyCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CHotKeyCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCHotKeyCtrl;
}
// Symbol: ?GetThisClass@CIPAddressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CIPAddressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCIPAddressCtrl;
}
// Symbol: ?GetRuntimeClass@CIPAddressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CIPAddressCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCIPAddressCtrl;
}
// Symbol: ?Create@CIPAddressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CIPAddressCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID)
{
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        static_cast<CWnd*>(pThis),
        L"SysIPAddress32",
        nullptr,
        dwStyle,
        rect,
        pParentWnd,
        nID,
        nullptr);
}
// Symbol: ?CreateEx@CIPAddressCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CIPAddressCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis,
    unsigned long dwExStyle,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID)
{
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
        static_cast<CWnd*>(pThis),
        dwExStyle,
        L"SysIPAddress32",
        nullptr,
        dwStyle,
        rect,
        pParentWnd,
        nID,
        nullptr);
}
// Symbol: ?GetAddress@CIPAddressCtrl@@QEBAHAEAE000@Z
extern "C" int MS_ABI impl__GetAddress_CIPAddressCtrl__QEBAHAEAE000_Z(
    void* pThis,
    unsigned char& nField0,
    unsigned char& nField1,
    unsigned char& nField2,
    unsigned char& nField3)
{
    CWnd* pWnd = static_cast<CWnd*>(pThis);
    if (!pWnd || !pWnd->m_hWnd) return 0;
    return (int)::SendMessageW(pWnd->m_hWnd, IPM_GETADDRESS, 0,
        (LPARAM)(LPDWORD)&nField0);
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@CLinkCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CLinkCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCLinkCtrl;
}
// Symbol: ?GetRuntimeClass@CLinkCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CLinkCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCLinkCtrl;
}
// Symbol: ?GetThisClass@CComboBoxEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CComboBoxEx__SAPEAUCRuntimeClass__XZ() {
    return &classCComboBoxEx;
}
// Symbol: ?GetRuntimeClass@CComboBoxEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CComboBoxEx__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCComboBoxEx;
}
// Symbol: ?GetThisClass@CNetAddressCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CNetAddressCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCNetAddressCtrl;
}
// Symbol: ?GetRuntimeClass@CNetAddressCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CNetAddressCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCNetAddressCtrl;
}
// Symbol: ?Create@CNetAddressCtrl@@UEAAHKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__Create_CNetAddressCtrl__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID)
{
    return impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
        static_cast<CWnd*>(pThis),
        L"SysAddressCtrl",
        nullptr,
        dwStyle,
        rect,
        pParentWnd,
        nID,
        nullptr);
}
// Symbol: ?CreateEx@CNetAddressCtrl@@UEAAHKKAEBUtagRECT@@PEAVCWnd@@I@Z
extern "C" int MS_ABI impl__CreateEx_CNetAddressCtrl__UEAAHKKAEBUtagRECT__PEAVCWnd__I_Z(
    void* pThis,
    unsigned long dwExStyle,
    unsigned long dwStyle,
    const RECT& rect,
    CWnd* pParentWnd,
    unsigned int nID)
{
    return impl__CreateEx_CWnd__UEAAHKPEB_W0KAEBUtagRECT__PEAV1_IPEAX_Z(
        static_cast<CWnd*>(pThis),
        dwExStyle,
        L"SysAddressCtrl",
        nullptr,
        dwStyle,
        rect,
        pParentWnd,
        nID,
        nullptr);
}
// Symbol: ??1CAnimateCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CAnimateCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
}
// Symbol: ??1CComboBoxEx@@UEAA@XZ
extern "C" void MS_ABI impl___1CComboBoxEx__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CComboBox*>(pThis)->~CComboBox();
}
// Symbol: ??1CDateTimeCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CDateTimeCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
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
extern "C" void MS_ABI impl___1CHeaderCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
}
// Symbol: ??1CIPAddressCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CIPAddressCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
}
// Symbol: ??1CListCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CListCtrl__UEAA_XZ(CListCtrl* pThis) {
    if (pThis) pThis->~CListCtrl();
}
// Symbol: ??1CLinkCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CLinkCtrl__UEAA_XZ(void* pThis) {
    if (pThis) static_cast<CWnd*>(pThis)->~CWnd();
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
// Symbol: ?GetThisClass@CMonthCalCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMonthCalCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCMonthCalCtrl;
}
// Symbol: ?GetRuntimeClass@CMonthCalCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CMonthCalCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCMonthCalCtrl;
}
// Symbol: ?GetThisClass@CReBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CReBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCReBarCtrl;
}
// Symbol: ?GetRuntimeClass@CReBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CReBarCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCReBarCtrl;
}
// Symbol: ?GetThisClass@CStatusBarCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CStatusBarCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCStatusBarCtrl;
}
// Symbol: ?GetRuntimeClass@CStatusBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CStatusBarCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCStatusBarCtrl;
}
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CToolBarCtrl@@SAPEAUCRuntimeClass@@XZ
// Implemented in cbarcore.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CToolBarCtrl@@UEBAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@CRichEditCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCRichEditCtrl;
}
// Symbol: ?GetRuntimeClass@CRichEditCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CRichEditCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCRichEditCtrl;
}
// Symbol: ?GetThisClass@CSplitButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CSplitButton__SAPEAUCRuntimeClass__XZ() {
    return &classCSplitButton;
}
// Symbol: ?GetRuntimeClass@CSplitButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CSplitButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCSplitButton;
}
// Symbol: ?GetRuntimeClass@CDragListBox@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDragListBox__UEBAPEAUCRuntimeClass__XZ(const CDragListBox*) {
    return CDragListBox::GetThisClass();
}
// Symbol: ?GetThisClass@CDragListBox@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDragListBox__SAPEAUCRuntimeClass__XZ() {
    return CDragListBox::GetThisClass();
}
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetRuntimeClass@CEdit@@UEBAPEAUCRuntimeClass@@XZ
// Implemented in manual_small_stub_implementations.cpp; this unit had a second, unlinked copy.
// Symbol: ?GetThisClass@CEdit@@SAPEAUCRuntimeClass@@XZ
// Symbol: ?GetThisClass@CDateTimeCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDateTimeCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCDateTimeCtrl;
}
// Symbol: ?GetRuntimeClass@CDateTimeCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CDateTimeCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCDateTimeCtrl;
}
// Symbol: ?GetThisClass@CBitmapButton@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CBitmapButton__SAPEAUCRuntimeClass__XZ() {
    return &classCBitmapButton;
}
// Symbol: ?GetRuntimeClass@CBitmapButton@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CBitmapButton__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCBitmapButton;
}
