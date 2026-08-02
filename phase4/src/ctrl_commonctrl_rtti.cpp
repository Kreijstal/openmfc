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

namespace {
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
} // namespace

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

// Symbol: ?GetThisClass@CPagerCtrl@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CPagerCtrl__SAPEAUCRuntimeClass__XZ() {
    return &classCPagerCtrl;
}
// Symbol: ?GetRuntimeClass@CPagerCtrl@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CPagerCtrl__UEBAPEAUCRuntimeClass__XZ(const void*) {
    return &classCPagerCtrl;
}

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
