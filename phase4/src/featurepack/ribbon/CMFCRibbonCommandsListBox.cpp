// CMFCRibbonCommandsListBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis);

// CMFCRibbonCommandsListBox is not declared in include/openmfc, so keep only
// the retail fields this file has verified. The constructor at RVA 0xf7650
// (mfc140u) initializes the fields from +0xe8 onward; list operations read
// m_hWnd at +0x40. These are partial offset views, not complete class definitions.
struct CMFCRibbonCommandsListBoxRetailPrefix {
    unsigned char _baseToHwnd[0x40];
    HWND m_hWnd;
    unsigned char _baseToTextOffset[0xe8 - 0x48];
    int m_nTextOffset;
    int m_bDrawDefaultIcon;
    int m_bCommandsOnly;
    unsigned char _paddingF4[0xf8 - 0xf4];
    void* m_pRibbonBar;
    void* m_pSeparator;
};
static_assert(offsetof(CMFCRibbonCommandsListBoxRetailPrefix, m_hWnd) == 0x40,
              "CMFCRibbonCommandsListBox::m_hWnd @ +0x40");
static_assert(offsetof(CMFCRibbonCommandsListBoxRetailPrefix, m_nTextOffset) == 0xe8,
              "CMFCRibbonCommandsListBox::m_nTextOffset @ +0xe8");
static_assert(offsetof(CMFCRibbonCommandsListBoxRetailPrefix, m_bDrawDefaultIcon) == 0xec,
              "CMFCRibbonCommandsListBox::m_bDrawDefaultIcon @ +0xec");
static_assert(offsetof(CMFCRibbonCommandsListBoxRetailPrefix, m_bCommandsOnly) == 0xf0,
              "CMFCRibbonCommandsListBox::m_bCommandsOnly @ +0xf0");
static_assert(offsetof(CMFCRibbonCommandsListBoxRetailPrefix, m_pRibbonBar) == 0xf8,
              "CMFCRibbonCommandsListBox::m_pRibbonBar @ +0xf8");
static_assert(offsetof(CMFCRibbonCommandsListBoxRetailPrefix, m_pSeparator) == 0x100,
              "CMFCRibbonCommandsListBox::m_pSeparator @ +0x100");

// Retail GetCommandIndex reads m_nID at +0x118, but OpenMFC's abbreviated
// CMFCRibbonBaseElement declaration places m_nID immediately after its 8-byte
// CObject vptr. This local view reads OpenMFC-created elements without relying
// on a retail offset that may be outside their allocation.
struct CMFCRibbonBaseElementOpenMfcPrefix {
    void* m_vftable;
    UINT m_nID;
};
static_assert(offsetof(CMFCRibbonBaseElementOpenMfcPrefix, m_nID) == sizeof(CObject),
              "OpenMFC CMFCRibbonBaseElement::m_nID follows the CObject vptr");

namespace {
UINT RibbonCommandsListBoxElementId(const void* pElement) {
    return reinterpret_cast<const CMFCRibbonBaseElementOpenMfcPrefix*>(pElement)->m_nID;
}

HWND RibbonCommandsListBoxHwnd(void* pThis) {
    return reinterpret_cast<CMFCRibbonCommandsListBoxRetailPrefix*>(pThis)->m_hWnd;
}

void* RibbonCommandsListBoxItemData(void* pThis, int index) {
    return reinterpret_cast<void*>(::SendMessage(
        RibbonCommandsListBoxHwnd(pThis), LB_GETITEMDATA,
        static_cast<WPARAM>(index), 0));
}
} // namespace

// Retail mfc140u RVA 0xf7650 constructs the CWnd base, writes the class vptr,
// stores pRibbonBar at +0xf8, initializes +0xe8/+0xec/+0xf0, and optionally
// allocates a separator at +0x100. OpenMFC has no MSVC vtable/layout for this
// class; use the CWnd constructor and initialize the verified fields, but keep
// the separator null because its retail virtual layout is not modeled.
// Symbol: ??0CMFCRibbonCommandsListBox@@QEAA@PEAVCMFCRibbonBar@@HHH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonCommandsListBox__QEAA_PEAVCMFCRibbonBar__HHH_Z(
    void* pThis, void* pRibbonBar, int bCreateSeparator, int bDrawDefaultIcon, int bCommandsOnly) {
    (void)bCreateSeparator;
    if (pThis == nullptr) {
        return nullptr;
    }

    impl___0CWnd__QEAA_XZ(pThis);
    auto* layout = reinterpret_cast<CMFCRibbonCommandsListBoxRetailPrefix*>(pThis);
    layout->m_nTextOffset = 0;
    layout->m_bDrawDefaultIcon = bDrawDefaultIcon;
    layout->m_bCommandsOnly = bCommandsOnly;
    layout->m_pRibbonBar = pRibbonBar;
    layout->m_pSeparator = nullptr;
    return pThis;
}

// Retail mfc140u RVA 0xf7740 resets the class vptr, destroys the optional
// object at +0x100, then calls CListBox::~CListBox (entry RVA 0x293f30,
// mfc140u). Our constructor leaves +0x100 null and cannot install the retail
// class vtable, so perform the modeled base teardown only.
// Symbol: ??1CMFCRibbonCommandsListBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCRibbonCommandsListBox__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        impl___1CListBox__UEAA_XZ(static_cast<CListBox*>(pThis));
    }
}

// Retail mfc140u RVA 0xf7f80 checks for an existing nonzero command ID, computes
// the initial text offset via element slot 71 (vtable byte offset 0x238), calls
// element slot 95 (0x2f8) to add it, and selects the new row only when bSelect is
// true. The required element virtual contracts are not modeled here.
// Symbol: ?AddCommand@CMFCRibbonCommandsListBox@@QEAAHPEAVCMFCRibbonBaseElement@@HH@Z
extern "C" int MS_ABI impl__AddCommand_CMFCRibbonCommandsListBox__QEAAHPEAVCMFCRibbonBaseElement__HH_Z(
    void* pThis, void* pCommand, int bSelect, int bDeep) {
    (void)pThis;
    (void)pCommand;
    (void)bSelect;
    (void)bDeep;
    return 0;
}

// Retail mfc140u RVA 0xf7790 performs owner drawing using item state, item data,
// ribbon/theme state, and several drawing virtuals. Those rendering
// dependencies have no verified shadow here.
// Symbol: ?DrawItem@CMFCRibbonCommandsListBox@@MEAAXPEAUtagDRAWITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__DrawItem_CMFCRibbonCommandsListBox__MEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, void* pDrawItemStruct) {
    (void)pThis;
    (void)pDrawItemStruct;
}

// Retail mfc140u RVA 0xf7c20 resets the list, walks the CArray, optionally
// filters elements, calls element slots 95 and 71, updates the text offset, and
// selects row zero if the final list is nonempty. The filter and virtual-call
// contracts plus shared global-state handling are not modeled here.
// Symbol: ?FillFromArray@CMFCRibbonCommandsListBox@@QEAAXAEBV?$CArray@PEAVCMFCRibbonBaseElement@@PEAV1@@@HH@Z
extern "C" void MS_ABI impl__FillFromArray_CMFCRibbonCommandsListBox__QEAAXAEBV__CArray_PEAVCMFCRibbonBaseElement__PEAV1___HH_Z(
    void* pThis, const void* pArray, int bDeep, int bIgnoreSeparators) {
    (void)pThis;
    (void)pArray;
    (void)bDeep;
    (void)bIgnoreSeparators;
}

// Retail mfc140u RVA 0xf7b40 clears the list; for a non-null category it
// builds a temporary element array, forwards it to FillFromArray with both
// flags true, then conditionally calls AddToListBox on the object at +0x100.
// GetElements is available over modeled panel state (but not hidden elements);
// row insertion still depends on FillFromArray/AddToListBox, which are stubs.
// Symbol: ?FillFromCategory@CMFCRibbonCommandsListBox@@QEAAXPEAVCMFCRibbonCategory@@@Z
extern "C" void MS_ABI impl__FillFromCategory_CMFCRibbonCommandsListBox__QEAAXPEAVCMFCRibbonCategory___Z(
    void* pThis, void* pCategory) {
    (void)pThis;
    (void)pCategory;
}

// Retail mfc140u RVA 0xf7dc0 walks the CList head/node fields (+0x8/+0x10),
// resolves nonzero IDs through the existing CMFCRibbonBar::FindByID thunk and
// uses the separator at +0x100 for ID zero, then forwards the array to
// FillFromArray. Those lookups are available; row insertion remains blocked by
// the FillFromArray/AddToListBox stubs, so this path is left stubbed.
// Symbol: ?FillFromIDs@CMFCRibbonCommandsListBox@@QEAAXAEBV?$CList@II@@H@Z
extern "C" void MS_ABI impl__FillFromIDs_CMFCRibbonCommandsListBox__QEAAXAEBV__CList_II__H_Z(
    void* pThis, const void* pCommands, int bDeep) {
    (void)pThis;
    (void)pCommands;
    (void)bDeep;
}

// Retail mfc140u RVA 0xf7ee0 tail-jumps to LB_GETITEMDATA on m_hWnd (+0x40).
// The corresponding mfc140.dll IAT slot 0x1802c5378 resolves to SendMessageA;
// the Unicode twin uses SendMessageW, selected by ::SendMessage here.
// Symbol: ?GetCommand@CMFCRibbonCommandsListBox@@QEBAPEAVCMFCRibbonBaseElement@@H@Z
extern "C" void* MS_ABI impl__GetCommand_CMFCRibbonCommandsListBox__QEBAPEAVCMFCRibbonBaseElement__H_Z(
    void* pThis, int nIndex) {
    return RibbonCommandsListBoxItemData(pThis, nIndex);
}

// Retail mfc140u RVA 0xf7f00 scans LB_GETCOUNT/LB_GETITEMDATA and compares
// each element's retail m_nID at +0x118, re-querying the count after each
// nonmatching item. This reads the field at OpenMFC's declared member offset
// rather than the retail offset, which is not valid for abbreviated objects.
// The corresponding mfc140.dll IAT slot 0x1802c5378 resolves to SendMessageA;
// ::SendMessage selects SendMessageW for this build.
// Symbol: ?GetCommandIndex@CMFCRibbonCommandsListBox@@QEBAHI@Z
extern "C" int MS_ABI impl__GetCommandIndex_CMFCRibbonCommandsListBox__QEBAHI_Z(
    void* pThis, unsigned int commandId) {
    int count = static_cast<int>(::SendMessage(
        RibbonCommandsListBoxHwnd(pThis), LB_GETCOUNT, 0, 0));
    for (int index = 0; index < count;) {
        const void* item = RibbonCommandsListBoxItemData(pThis, index);
        if (RibbonCommandsListBoxElementId(item) == commandId) {
            return index;
        }
        ++index;
        count = static_cast<int>(::SendMessage(
            RibbonCommandsListBoxHwnd(pThis), LB_GETCOUNT, 0, 0));
    }
    return -1;
}

// Retail mfc140u RVA 0xf7e90 returns null when LB_GETCURSEL is negative and
// otherwise returns LB_GETITEMDATA for that index.
// Symbol: ?GetSelected@CMFCRibbonCommandsListBox@@QEBAPEAVCMFCRibbonBaseElement@@XZ
extern "C" void* MS_ABI impl__GetSelected_CMFCRibbonCommandsListBox__QEBAPEAVCMFCRibbonBaseElement__XZ(
    void* pThis) {
    const int index = static_cast<int>(::SendMessage(
        RibbonCommandsListBoxHwnd(pThis), LB_GETCURSEL, 0, 0));
    return index < 0 ? nullptr : RibbonCommandsListBoxItemData(pThis, index);
}

// Retail mfc140u RVA 0xf7aa0 creates a client DC, selects DEFAULT_GUI_FONT
// (stock-object index 17), calls GetTextMetricsW, and stores tmHeight + 6.
// Use the equivalent GDI sequence directly rather than relying on unmodeled
// CDC/CFont handle-map wrappers. On a GDI failure, leave itemHeight untouched.
// Symbol: ?MeasureItem@CMFCRibbonCommandsListBox@@MEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
extern "C" void MS_ABI impl__MeasureItem_CMFCRibbonCommandsListBox__MEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    void* pThis, void* pMeasureItemStruct) {
    if (pThis == nullptr || pMeasureItemStruct == nullptr) {
        return;
    }

    const HWND hwnd = RibbonCommandsListBoxHwnd(pThis);
    HDC hdc = ::GetDC(hwnd);
    if (hdc == nullptr) {
        return;
    }

    HGDIOBJ oldFont = ::SelectObject(hdc, ::GetStockObject(DEFAULT_GUI_FONT));
    TEXTMETRICW metrics{};
    if (oldFont != nullptr && oldFont != HGDI_ERROR && ::GetTextMetricsW(hdc, &metrics)) {
        static_cast<MEASUREITEMSTRUCT*>(pMeasureItemStruct)->itemHeight = metrics.tmHeight + 6;
    }

    if (oldFont != nullptr && oldFont != HGDI_ERROR) {
        ::SelectObject(hdc, oldFont);
    }
    ::ReleaseDC(hwnd, hdc);
}

