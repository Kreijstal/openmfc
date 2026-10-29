// CMFCRibbonRichEditCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CRichEditCtrl__UEAA_XZ(void* pThis);

namespace {
// This class is not fully declared in include/openmfc. The retail header declares
// CMFCRibbonEdit& followed by three BOOLs and CString; the Unicode constructor
// (RVA 0xfca40 (mfc140u)) stores those members at the offsets asserted below.
// CMFCRibbonEdit::CreateEdit allocates exactly 0x108 bytes (RVA 0xfc1d0 (mfc140u)),
// pinning this shadow's total size.
struct S_CMFCRibbonRichEditCtrl {
    unsigned char m_richEditCtrlBase[0xe8];
    void* m_edit;
    BOOL m_bTracked;
    BOOL m_bIsHighlighted;
    BOOL m_bIsContextMenu;
    alignas(8) unsigned char m_strOldText[sizeof(CString)];
};

static_assert(sizeof(CWnd) == 0xe8, "CWnd base ends at the retail rich-edit owner reference");
static_assert(sizeof(CRichEditCtrl) == 0xe8, "CRichEditCtrl adds no data beyond CWnd");
static_assert(offsetof(S_CMFCRibbonRichEditCtrl, m_edit) == 0xe8, "m_edit +0xe8");
static_assert(offsetof(S_CMFCRibbonRichEditCtrl, m_bTracked) == 0xf0, "m_bTracked +0xf0");
static_assert(offsetof(S_CMFCRibbonRichEditCtrl, m_bIsHighlighted) == 0xf4, "m_bIsHighlighted +0xf4");
static_assert(offsetof(S_CMFCRibbonRichEditCtrl, m_bIsContextMenu) == 0xf8, "m_bIsContextMenu +0xf8");
static_assert(offsetof(S_CMFCRibbonRichEditCtrl, m_strOldText) == 0x100, "m_strOldText +0x100");
static_assert(sizeof(CString) == 0x8, "CString is one data pointer on x64");
static_assert(sizeof(S_CMFCRibbonRichEditCtrl) == 0x108, "retail CMFCRibbonRichEditCtrl size");
} // namespace

// OpenMFC has no C++ declaration/vtable for CMFCRibbonRichEditCtrl, so this
// initializes the CWnd base and fields but cannot install the retail derived
// vtable; the object retains CWnd's vptr and virtual rich-edit dispatch differs.
// Retail's constructor is RVA 0xfca40 (mfc140u).
// Symbol: ??0CMFCRibbonRichEditCtrl@@QEAA@AEAVCMFCRibbonEdit@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonRichEditCtrl__QEAA_AEAVCMFCRibbonEdit___Z(void* pThis, void* pEdit) {
    impl___0CWnd__QEAA_XZ(pThis);
    auto* layout = reinterpret_cast<S_CMFCRibbonRichEditCtrl*>(pThis);
    layout->m_edit = pEdit;
    layout->m_bTracked = FALSE;
    layout->m_bIsHighlighted = FALSE;
    layout->m_bIsContextMenu = FALSE;
    impl___0__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
        layout->m_strOldText);
    return pThis;
}

// Retail destructor entry RVA 0xfcb00 (mfc140u) releases the CString at +0x100
// before tail-calling CRichEditCtrl::~CRichEditCtrl at RVA 0x296b00 (mfc140u).
// Pair the cleanup with the base/member initialization above; the derived-vtable
// limitation noted there remains because this class is not declared in OpenMFC.
// Symbol: ??1CMFCRibbonRichEditCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonRichEditCtrl__UEAA_XZ(void* pThis) {
    auto* layout = reinterpret_cast<S_CMFCRibbonRichEditCtrl*>(pThis);
    impl___1__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAA_XZ(
        layout->m_strOldText);
    impl___1CRichEditCtrl__UEAA_XZ(pThis);
}

// The body at RVA 0xfd150 (mfc140u) reads the control text into a CString,
// assigns the owner's edit string, then enters an internal recursive helper.
// That helper and the owner's matching state/layout are not available here.
// Symbol: ?OnChange@CMFCRibbonRichEditCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnChange_CMFCRibbonRichEditCtrl__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail RVA 0xfd5a0 (mfc140u) gates on a global pointer, gets the top-level
// ribbon bar, sets +0xf8 while calling its vtable byte offset 0x6a0, then clears
// the flag. The global is not modeled, and the existing GetTopLevelRibbonBar
// thunk returns nullptr (CMFCRibbonBaseElement.cpp), so this path cannot be
// faithfully forwarded in this build.
// Symbol: ?OnContextMenu@CMFCRibbonRichEditCtrl@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCRibbonRichEditCtrl__IEAAXPEAVCWnd__VCPoint___Z(
    void* pThis, void* pWnd, long long point) {
    (void)pThis;
    (void)pWnd;
    (void)point;
}

// Inspected the ANSI twin at RVA 0xfde20 (mfc140.dll); the Unicode symbol map
// has no resolved RVA. This body changes CMFCRibbonEdit state, redraws the
// control, copies CString text, and calls further owner methods. OpenMFC's
// CMFCRibbonEdit declaration is a placeholder rather than that retail layout.
// Symbol: ?OnKillFocus@CMFCRibbonRichEditCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCRibbonRichEditCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, void* pNewWnd) {
    (void)pThis;
    (void)pNewWnd;
}

// Inspected the ANSI twin at RVA 0xfe0c0 (mfc140.dll); the Unicode symbol map
// has no resolved RVA. It calls a CMFCRibbonEdit vtable entry and dispatches
// WM_MOUSELEAVE / redraw behavior using the highlighted/tracked state. The
// owner's retail vtable is not represented by OpenMFC's placeholder class.
// Symbol: ?OnMouseLeave@CMFCRibbonRichEditCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCRibbonRichEditCtrl__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Inspected the ANSI twin at RVA 0xfdf00 (mfc140.dll); the Unicode symbol map
// has no resolved RVA. Painting calls multiple owner virtuals and depends on
// ribbon state not represented by this file's available layout.
// Symbol: ?OnPaint@CMFCRibbonRichEditCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCRibbonRichEditCtrl__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Inspected the ANSI twin at RVA 0xfddb0 (mfc140.dll); the Unicode symbol map
// has no resolved RVA. Although the CWnd base call is available, this body also
// updates owner fields at +0x290/+0x294 and calls owner vtable slots +0x458/+0x380;
// those entries are not modeled by OpenMFC's CMFCRibbonEdit placeholder.
// Symbol: ?OnSetFocus@CMFCRibbonRichEditCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCRibbonRichEditCtrl__IEAAXPEAVCWnd___Z(
    void* pThis, void* pOldWnd) {
    (void)pThis;
    (void)pOldWnd;
}

// Inspected the ANSI twin at RVA 0xfd710 (mfc140.dll); the Unicode symbol map
// has no resolved RVA. Its message-routing body depends on owner virtuals and
// ribbon/input state not represented by the available class layout.
// Symbol: ?PreTranslateMessage@CMFCRibbonRichEditCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCRibbonRichEditCtrl__UEAAHPEAUtagMSG___Z(
    void* pThis, MSG* pMsg) {
    (void)pThis;
    (void)pMsg;
    return 0;
}

// Inspected the ANSI twin at RVA 0xfdc60 (mfc140.dll); --u has no resolved
// symbol entry. The body checks Ctrl/Shift with GetAsyncKeyState, maps the
// standard accelerators to WM_COPY/WM_PASTE/WM_CUT, sends that message to this
// CWnd, and returns whether it handled the key. Its IAT slots resolve to
// GetAsyncKeyState and SendMessageA in mfc140.dll; the Unicode build uses
// ::SendMessage through the UNICODE mapping. The two DLLs' function bodies are
// byte-identical according to the wf2 image comparison.
// Symbol: ?ProcessClipboardAccelerators@CMFCRibbonRichEditCtrl@@IEAAHI@Z
extern "C" int MS_ABI impl__ProcessClipboardAccelerators_CMFCRibbonRichEditCtrl__IEAAHI_Z(
    void* pThis, unsigned int nChar) {
    const bool controlDown = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    const bool shiftDown = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

    UINT message = 0;
    if (controlDown && (nChar == 'C' || nChar == VK_INSERT)) {
        message = WM_COPY;
    } else if (controlDown && nChar == 'V') {
        message = WM_PASTE;
    } else if (shiftDown && nChar == VK_INSERT) {
        message = WM_PASTE;
    } else if (controlDown && nChar == 'X') {
        message = WM_CUT;
    } else if (shiftDown && nChar == VK_DELETE) {
        message = WM_CUT;
    }

    if (message == 0) {
        return FALSE;
    }
    ::SendMessage(reinterpret_cast<CWnd*>(pThis)->m_hWnd, message, 0, 0);
    return TRUE;
}
