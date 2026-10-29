// CMFCRibbonFontComboBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "openmfc/afxtoolbarfontcombobox.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>

extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(
    const CObList* pThis, long long nIndex);
extern "C" void MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc);

namespace {
// CMFCRibbonFontComboBox is not declared in include/openmfc. Retail's ctor
// initializes derived fields at +0x330/+0x334/+0x335 and its image-list member
// at +0x338/+0x340 (RVA 0xf6d30 (mfc140u)); CreateObject allocates 0x348
// bytes (RVA 0xf6c00 (mfc140u)).
struct RibbonFontComboBoxOwnLayout {
    int fontType;
    BYTE charSet;
    BYTE pitchAndFamily;
    BYTE padding[2];
    void* images[2];
};
constexpr std::size_t kRibbonFontComboBoxOwnOffset = 0x330;
static_assert(offsetof(RibbonFontComboBoxOwnLayout, fontType) == 0x00, "font type @+0x330");
static_assert(offsetof(RibbonFontComboBoxOwnLayout, charSet) == 0x04, "charset @+0x334");
static_assert(offsetof(RibbonFontComboBoxOwnLayout, pitchAndFamily) == 0x05, "pitch/family @+0x335");
static_assert(offsetof(RibbonFontComboBoxOwnLayout, images) == 0x08, "image list @+0x338");
static_assert(kRibbonFontComboBoxOwnOffset + sizeof(RibbonFontComboBoxOwnLayout) == 0x348,
              "retail CMFCRibbonFontComboBox size is 0x348");

// OpenMFC CObList POSITION points at a side-table node with this CList shape.
// This mirrors the node representation used by the CObList implementation.
struct RibbonFontObNode {
    RibbonFontObNode* pNext;
    RibbonFontObNode* pPrev;
    CObject* data;
};
static_assert(offsetof(RibbonFontObNode, data) == 0x10, "CObList node data @+0x10");
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "CObList POSITION is one pointer");

// CMFCFontInfo has a CObject vptr followed by m_strName (retail GetFontsCount
// reads the CString at +0x08); retain a local layout assertion for this access.
struct RibbonFontInfoNameLayout {
    void* vfptr;
    CString name;
};
static_assert(offsetof(RibbonFontInfoNameLayout, name) == 0x08, "CMFCFontInfo::m_strName @+0x08");
static_assert(sizeof(CMFCFontInfo) == 0x20, "CMFCFontInfo size from openmfc/afxtoolbarfontcombobox.h");

RibbonFontObNode* RibbonNodeFromPosition(CObList::POSITION position) {
    RibbonFontObNode* node = nullptr;
    std::memcpy(&node, &position, sizeof(node));
    return node;
}
} // namespace

// Symbol: ??0CMFCRibbonFontComboBox@@IEAA@XZ
// Stub: RVA 0xf6d30 (mfc140u) calls the ribbon-combo base constructor, installs
// the derived vftable, and initializes its image-list member and font filters.
// This thunk still cannot establish that base/MSVC-layout state safely.
extern "C" void* MS_ABI impl___0CMFCRibbonFontComboBox__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0CMFCRibbonFontComboBox@@QEAA@IHEEH@Z
// Stub: RVA 0xf6c40 (mfc140u) constructs the base, initializes the derived
// image-list/filter state, and calls BuildFonts; that base and its native
// object layout are not available for a safe constructor transcription here.
extern "C" void* MS_ABI impl___0CMFCRibbonFontComboBox__QEAA_IHEEH_Z(
    void* pThis, unsigned int arg0, int arg1, unsigned char arg2,
    unsigned char arg3, int arg4) {
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    (void)arg4;
    return pThis;
}
// Symbol: ??1CMFCRibbonFontComboBox@@UEAA@XZ
// Stub: RVA 0xf6d80 (mfc140u) calls ClearData, destroys the image-list member,
// and then the CMFCRibbonComboBox base; ClearData and the base dtor are stubs here.
extern "C" void MS_ABI impl___1CMFCRibbonFontComboBox__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?BuildFonts@CMFCRibbonFontComboBox@@QEAAXHEE@Z
// Stub: RVA 0xf6e40 (mfc140u) updates the filters and calls RebuildFonts
// unless the existing font list and all three filters already match. The
// rebuild requires inherited ribbon item-list/AddItem behavior not implemented
// by the current base combo thunk.
extern "C" void MS_ABI impl__BuildFonts_CMFCRibbonFontComboBox__QEAAXHEE_Z(
    void* pThis, int nFontType, unsigned char nCharSet, unsigned char nPitchAndFamily) {
    (void)pThis;
    (void)nFontType;
    (void)nCharSet;
    (void)nPitchAndFamily;
}

// Symbol: ?ClearData@CMFCRibbonFontComboBox@@MEAAXXZ
// Stub: RVA 0xf7110 (mfc140u) skips cleanup when inherited +0xe8 is non-null;
// otherwise it walks the item-data nodes at +0x2f0 and deletes non-null entries.
// That base list has no usable shadow layout in the current support code.
extern "C" void MS_ABI impl__ClearData_CMFCRibbonFontComboBox__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CopyFrom@CMFCRibbonFontComboBox@@MEAAXAEBVCMFCRibbonBaseElement@@@Z
// Stub: RVA 0xf6dc0 (mfc140u) first calls CMFCRibbonComboBox::CopyFrom
// (RVA 0xf6400 (mfc140u)), which is still an empty thunk in CMFCRibbonComboBox.cpp;
// copying only these derived fields would leave the base element state incomplete.
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonFontComboBox__MEAAXAEBVCMFCRibbonBaseElement___Z(
    void* pThis, const void* pSrc) {
    (void)pThis;
    (void)pSrc;
}

// Symbol: ?CreateObject@CMFCRibbonFontComboBox@@SAPEAVCObject@@XZ
// Stub: RVA 0xf6c00 (mfc140u) allocates 0x348 bytes and invokes the default
// constructor; that constructor thunk in this file currently only returns pThis.
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonFontComboBox__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?DropDownList@CMFCRibbonFontComboBox@@MEAAXXZ
// Stub: RVA 0xf6e10 (mfc140u) calls BuildFonts and then the base combo
// DropDownList implementation, which remains a stub in CMFCRibbonComboBox.cpp.
extern "C" void MS_ABI impl__DropDownList_CMFCRibbonFontComboBox__MEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?GetFontsCount@CMFCRibbonFontComboBox@@IEAAHPEB_WAEBVCObList@@@Z
// Retail (RVA 0xf70a0 (mfc140u)) counts exact, case-sensitive name matches in
// the supplied list (IAT slot 0x1802c7770 resolves to wcscmp). OpenMFC reaches
// its side-table-backed nodes through FindIndex rather than reading a retail
// list head inline. Retail raises AfxThrowOleException(E_FAIL) for a NULL name
// when the list is nonempty.
extern "C" int MS_ABI impl__GetFontsCount_CMFCRibbonFontComboBox__IEAAHPEB_WAEBVCObList___Z(
    void* pThis, const wchar_t* lpszName, const CObList* pFonts) {
    (void)pThis;
    int count = 0;
    CObList::POSITION position =
        impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pFonts, 0);
    for (RibbonFontObNode* node = RibbonNodeFromPosition(position);
         node != nullptr; node = node->pNext) {
        if (lpszName == nullptr) {
            impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
            return count;
        }
        const auto* font = static_cast<const CMFCFontInfo*>(node->data);
        if (std::wcscmp(font->m_strName.GetString(), lpszName) == 0) {
            ++count;
        }
    }
    return count;
}

// Symbol: ?OnDrawDropListItem@CMFCRibbonFontComboBox@@MEAAHPEAVCDC@@HPEAVCMFCToolBarMenuButton@@H@Z
// Stub: RVA 0xf7390 (mfc140u) performs font-specific rendering through ribbon,
// image-list, and visual-manager state not represented by this file's shadows.
extern "C" int MS_ABI impl__OnDrawDropListItem_CMFCRibbonFontComboBox__MEAAHPEAVCDC__HPEAVCMFCToolBarMenuButton__H_Z(
    void* pThis, void* pDC, int nIndex, void* pItem, int bHighlight) {
    (void)pThis;
    (void)pDC;
    (void)nIndex;
    (void)pItem;
    (void)bHighlight;
    return 0;
}

// Symbol: ?OnGetDropListItemSize@CMFCRibbonFontComboBox@@MEAA?AVCSize@@PEAVCDC@@HPEAVCMFCToolBarMenuButton@@V2@@Z
// Retail (RVA 0xf75f0 (mfc140u)) copies the by-value CSize to its hidden
// result buffer and adds 0x16 to cx. Under the MS ABI, this thunk receives the
// result buffer in RDX and the two 8-byte aggregate values are modeled as
// long long; the returned RAX is the result-buffer pointer.
extern "C" void* MS_ABI impl__OnGetDropListItemSize_CMFCRibbonFontComboBox__MEAA_AVCSize__PEAVCDC__HPEAVCMFCToolBarMenuButton__V2__Z(
    void* pThis, void* pResult, void* pDC, int nIndex, void* pItem, long long sizeDefault) {
    (void)pThis;
    (void)pDC;
    (void)nIndex;
    (void)pItem;
    std::memcpy(pResult, &sizeDefault, sizeof(sizeDefault));
    std::uint32_t cx;
    std::memcpy(&cx, pResult, sizeof(cx));
    cx += 0x16u;
    std::memcpy(pResult, &cx, sizeof(cx));
    return pResult;
}

// Symbol: ?RebuildFonts@CMFCRibbonFontComboBox@@QEAAXXZ
// Stub: RVA 0xf6e90 (mfc140u) constructs a temporary toolbar font combo to
// enumerate fonts, then transfers eligible entries through the ribbon combo's
// inherited item-list virtual. Font enumeration exists in the toolbar subsystem,
// but the inherited ribbon list/AddItem behavior this routine needs is stubbed.
extern "C" void MS_ABI impl__RebuildFonts_CMFCRibbonFontComboBox__QEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?SetFont@CMFCRibbonFontComboBox@@QEAAHPEB_WEH@Z
// Stub: RVA 0xf7240 (mfc140u) lowercases the requested name and scans the
// inherited item-data nodes at +0x2f0, then selects the matching font. The
// ribbon combo item-list and selection behavior needed to reproduce that path
// is not implemented by its current base thunks.
extern "C" int MS_ABI impl__SetFont_CMFCRibbonFontComboBox__QEAAHPEB_WEH_Z(
    void* pThis, const wchar_t* lpszName, unsigned char nCharSet, int bExact) {
    (void)pThis;
    (void)lpszName;
    (void)nCharSet;
    (void)bExact;
    return 0;
}

