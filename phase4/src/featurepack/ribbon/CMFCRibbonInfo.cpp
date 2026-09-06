// CMFCRibbonInfo — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"


#include <cstdlib>
#include <cstring>
#include <new>

// ===========================================================================
// CMFCRibbonInfo::X* -- the ribbon XML/state model.
//
// The bodies below were transcribed from the retail mfc140.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); every
// function names the RVA it was read from.  These classes are declared in the
// public header afxribboninfo.h, so client code allocates them with the retail
// member layout; the RI_* layout views below repeat that layout and every
// offset in them was read back out of the retail code (each struct cites the
// function the offsets came from).
//
// Two structural deviations from retail, both applied uniformly:
//
//  (1) vtable pointer.  Every retail constructor and destructor stores a class
//      vtable pointer at +0x00 (e.g. XElementButtonLaunch::XElementButtonLaunch
//      stores 0x180303c60, and each retail vtable is
//      { vector deleting dtor, Read, Write }).  OpenMFC does not model an
//      MSVC-layout vtable for this family -- Read/Write exist only as impl__
//      thunks whose generated signatures drop the XRibbonInfoParser& argument
//      (see featurepack/ribbon/Thunks.cpp and the ?Read@X* blocks below), so
//      their addresses would be wrong as vtable slots -- so the constructors
//      store nullptr there instead, and the destructors leave
//      the slot alone rather than rewriting it to the base-class vtable as
//      retail does.  A virtual call on one of these objects therefore faults
//      deterministically instead of dispatching.  The same applies to the
//      vtable pointer at +0x00 of every embedded CArray (retail stores the
//      vtable of that CArray<T,ARG> instantiation there, not CObject's), which
//      is the convention already used for CArray members elsewhere in this tree
//      (see detail/CMFCRibbonQuickAccessToolBarDefaultStateSupport.h).
//
//  (2) owned array elements.  The retail destructors of XElementButton,
//      XElementGroup, XPanel, XCategory, XCategoryMain, XContext, XRibbonBar
//      and XElementButtonGallery delete each element of their pointer arrays
//      through the element's vtable slot 0 (the vector deleting destructor)
//      and then run the CArray destructor, which frees the buffer.  Neither is
//      reproduced here: deviation (1) leaves no vtable to dispatch through, and
//      every one of those members is a CArray<T*,T*> *template* instantiation
//      (afxribboninfo.h typedefs XArrayElement/XArrayPanel/XArrayCategory/
//      XArrayContext/XArrayGalleryGroup), so its code -- including the
//      allocation of m_pData -- is compiled inline into the client and the
//      buffer belongs to the client's CRT.  (Confirmed in retail: the CArray
//      destructors those bodies call, RVAs 0x108bb4/0x108bd8/0x108bfc/
//      0x108c9c/0x108cc0, are local helpers with no export entry.)  Freeing it
//      from this DLL would be a cross-allocator free, so those elements and
//      that buffer are LEAKED instead.  That is a deliberate
//      leak-instead-of-crash choice, not a transcription of retail.
//      XQAT::m_arItems is the one array this DLL grows itself, in
//      XQAT::AddItem below, and ~XQAT destroys and frees it for real exactly as
//      retail does.  XElementComboBox::m_arItems is skipped for a different
//      reason again -- see the comment on ~XElementComboBox.
//
// CString members are handled with this DLL's own CString (a single m_pszData
// pointer, include/openmfc/afxstr.h): the retail bodies construct them from the
// string manager's nil string and destroy them by the CStringData refcount, and
// the OpenMFC CString constructor and destructor perform those same two
// operations.  Releasing a string whose buffer a client allocated is
// allocator-safe in either direction, because every CStringData carries its own
// IAtlStringMgr and the release dispatches through it.
// KNOWN GAP, not introduced here but widened by these constructors: OpenMFC's
// *nil* CStringData (afxstr.h GetNilStringData) is marked locked with
// nRefs == -1, whereas ATL's CNilStringData uses nRefs == 2
// (atlsimpstr.h:119) precisely so that ATL's inline
// CStringData::Release() -- which decrements unconditionally and calls
// pStringMgr->Free() at <= 0 -- never frees it.  An MSVC client that runs its
// own inline ATL code over an empty CString this file planted therefore calls
// Free() on a static.  Every CString these bodies build starts out nil, so the
// fix belongs in afxstr.h (reported as a headerRequest); nothing in this file
// can work around it.
// ===========================================================================

// Thunks defined further down this file that earlier bodies call, plus the two
// CMFCToolBarImages thunks (featurepack/toolbar/CMFCToolBarImages.cpp) used to
// construct/destroy the CMFCToolBarImages sub-object inside XImage.
extern "C" void* MS_ABI impl___0CMFCToolBarImages__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarImages__UEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XID_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XImage_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(void* pThis, int type);
extern "C" void* MS_ABI impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(void* pThis, int type);
extern "C" void* MS_ABI impl___0XElementButtonGallery_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(void* pThis, int type);
extern "C" void* MS_ABI impl___0XElementEdit_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(void* pThis, int type);
extern "C" void* MS_ABI impl___0XElementComboBox_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(void* pThis, int type);
extern "C" void* MS_ABI impl___0XElementButtonLaunch_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementGroup_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XID_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XImage_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElement_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElementButtonGallery_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElementEdit_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElementComboBox_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElementButtonLaunch_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XElementGroup_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis);

namespace {

static_assert(sizeof(CString) == 8, "CString must be a single m_pszData pointer");
static_assert(sizeof(CMFCToolBarImages) == 0x198,
              "XImage embeds CMFCToolBarImages at +0x20; retail XImage spans 0x1b8 bytes");

// MFC CArray<T,ARG>: CObject vfptr followed by four INT_PTR-sized fields.
// Offsets read from ??0XElementButton@CMFCRibbonInfo@@QEAA@XZ (RVA 0xfea80),
// which fills m_arSubItems at +0x60..+0x88.
struct RI_Array {
    void*     vfptr;      // 0x00  retail: the CArray instantiation's vtable
    void*     m_pData;    // 0x08
    long long m_nSize;    // 0x10
    long long m_nMaxSize; // 0x18
    long long m_nGrowBy;  // 0x20
};
static_assert(sizeof(RI_Array) == 0x28, "CArray is 0x28 bytes on x64");

// Retail stores the CArray vtable in vfptr and zeroes the rest; see deviation (1).
void ri_array_init(RI_Array& a) {
    a.vfptr     = nullptr;
    a.m_pData   = nullptr;
    a.m_nSize   = 0;
    a.m_nMaxSize = 0;
    a.m_nGrowBy = 0;
}

// Appends one zero-filled slot and returns its index, or -1 when the allocation
// fails.  This reproduces the branch structure of the retail CArray::SetSize
// this family uses (RVA 0xf3f3c, reached from CArray<XQATItem,XQATItem>::Add at
// RVA 0xf3e84), which is MFC's CArray::SetSize(nNewSize, -1):
//   * m_pData == NULL      -> allocate max(nNewSize, m_nGrowBy) elements and
//                             set m_nMaxSize to exactly that.  Retail does NOT
//                             apply the 4/1024 growth heuristic on this path,
//                             so with the default m_nGrowBy == 0 the first
//                             append reserves exactly one element.
//   * nNewSize <= m_nMaxSize -> no allocation, just clear the new tail.
//   * otherwise            -> grow to m_nMaxSize + nGrowBy (nGrowBy defaulting
//                             to m_nSize/8 clamped to [4,1024]), or to
//                             nNewSize if that is larger.
// Same allocator family as arr_SetSize() in
// detail/CMFCRibbonQuickAccessToolBarDefaultStateSupport.cpp, which splits the
// same three branches the same way.
long long ri_array_append_slot(RI_Array& a, std::size_t elem) {
    const long long idx = a.m_nSize;
    const long long need = idx + 1;
    if (a.m_pData == nullptr) {
        const long long newMax = (need > a.m_nGrowBy) ? need : a.m_nGrowBy;
        void* p = std::calloc(static_cast<std::size_t>(newMax), elem);
        if (p == nullptr) return -1;
        a.m_pData    = p;
        a.m_nMaxSize = newMax;
    } else if (need > a.m_nMaxSize) {
        long long grow = a.m_nGrowBy;
        if (grow == 0) {
            grow = a.m_nSize / 8;
            if (grow < 4) grow = 4;
            if (grow > 1024) grow = 1024;
        }
        const long long newMax = (need < a.m_nMaxSize + grow) ? a.m_nMaxSize + grow : need;
        void* p = std::realloc(a.m_pData, static_cast<std::size_t>(newMax) * elem);
        if (p == nullptr) return -1;
        std::memset(static_cast<char*>(p) + idx * static_cast<long long>(elem), 0,
                    static_cast<std::size_t>((newMax - idx) * static_cast<long long>(elem)));
        a.m_pData    = p;
        a.m_nMaxSize = newMax;
    } else {
        std::memset(static_cast<char*>(a.m_pData) + idx * static_cast<long long>(elem), 0, elem);
    }
    a.m_nSize = need;
    return idx;
}

// ---- layout views ---------------------------------------------------------
// Every offset below was read out of the retail function named beside it.

struct RI_XBase {              // ??0XBase 0xfe5c0 / ??1XBase 0xfe610, size 0x10
    void* vfptr;               // 0x00
    int   m_ElementType;       // 0x08
    int   _pad_0c;
};
static_assert(sizeof(RI_XBase) == 0x10, "XBase size");

struct RI_XID {                // ??0XID 0xfe520 / ??1XID 0x2ff0, size 0x10
    CString  m_Name;           // 0x00
    unsigned m_Value;          // 0x08
    int      _pad_0c;
};
static_assert(sizeof(RI_XID) == 0x10, "XID size");

struct RI_XImage {             // ??0XImage 0xfe560 / ??1XImage 0xfe590, size 0x1b8
    RI_XID        m_ID;        // 0x00
    RI_XID        m_ID_HDPI;   // 0x10
    unsigned char m_Image[0x198]; // 0x20  CMFCToolBarImages
};
static_assert(sizeof(RI_XImage) == 0x1b8, "XImage size (XElementGroup puts m_arButtons at +0x208)");

struct RI_XElement : RI_XBase { // ??0XElement 0xfe620 / ??1XElement 0xfe730, size 0x50
    RI_XID  m_ID;              // 0x10
    CString m_strText;         // 0x20
    CString m_strToolTip;      // 0x28
    CString m_strDescription;  // 0x30
    CString m_strKeys;         // 0x38
    CString m_strMenuKeys;     // 0x40
    int     m_bIsOnPaletteTop; // 0x48
    int     m_bIsAlwaysLarge;  // 0x4c
};
static_assert(sizeof(RI_XElement) == 0x50, "XElement size");
static_assert(offsetof(RI_XElement, m_strText) == 0x20, "XElement::m_strText");
static_assert(offsetof(RI_XElement, m_strMenuKeys) == 0x40, "XElement::m_strMenuKeys");

struct RI_XElementSeparator : RI_XElement { // ??0XElementSeparator 0xfe830
    int m_bIsHoriz;            // 0x50
    int _pad_54;
};
static_assert(offsetof(RI_XElementSeparator, m_bIsHoriz) == 0x50, "XElementSeparator::m_bIsHoriz");

struct RI_XElementSlider : RI_XElement {    // ??0XElementSlider 0xff770
    unsigned long m_dwStyle;   // 0x50
    int m_nWidth;              // 0x54
    int m_nMin;                // 0x58
    int m_nMax;                // 0x5c
    int m_nPos;                // 0x60
    int m_bZoomButtons;        // 0x64
};
static_assert(offsetof(RI_XElementSlider, m_nPos) == 0x60, "XElementSlider::m_nPos");

struct RI_XElementGroup : RI_XElement {     // ??0XElementGroup 0xfe8b0, size 0x230
    RI_XImage m_Images;        // 0x50
    RI_Array  m_arButtons;     // 0x208
};
static_assert(sizeof(RI_XElementGroup) == 0x230, "XElementGroup size (operator delete size in ??_G)");
static_assert(offsetof(RI_XElementGroup, m_arButtons) == 0x208, "XElementGroup::m_arButtons");

struct RI_XElementButton : RI_XElement {    // ??0XElementButton 0xfea80, size 0x88
    int m_nSmallImageIndex;         // 0x50
    int m_nLargeImageIndex;         // 0x54
    int m_bIsDefaultCommand;        // 0x58
    int m_bIsAlwaysShowDescription; // 0x5c
    RI_Array m_arSubItems;          // 0x60
};
static_assert(sizeof(RI_XElementButton) == 0x88, "XElementButton size");
static_assert(offsetof(RI_XElementButton, m_arSubItems) == 0x60, "XElementButton::m_arSubItems");

struct RI_XElementButtonLinkCtrl : RI_XElementButton { // ??0XElementButtonLinkCtrl 0xfec60
    CString m_strLink;         // 0x88
};
static_assert(offsetof(RI_XElementButtonLinkCtrl, m_strLink) == 0x88, "XElementButtonLinkCtrl::m_strLink");

struct RI_XElementButtonApplication : RI_XElementButton { // ??0XElementButtonApplication 0xff640
    RI_XImage m_Image;         // 0x88
};
static_assert(offsetof(RI_XElementButtonApplication, m_Image) == 0x88, "XElementButtonApplication::m_Image");

struct RI_XElementEdit : RI_XElementButton { // ??0XElementEdit 0xfed40, size 0xa8
    int m_nWidth;              // 0x88
    int m_nWidthFloaty;        // 0x8c
    int m_bHasSpinButtons;     // 0x90
    int m_nMin;                // 0x94
    int m_nMax;                // 0x98
    int _pad_9c;
    CString m_strValue;        // 0xa0
};
static_assert(sizeof(RI_XElementEdit) == 0xa8, "XElementEdit size");
static_assert(offsetof(RI_XElementEdit, m_strValue) == 0xa0, "XElementEdit::m_strValue");

struct RI_XElementComboBox : RI_XElementEdit { // ??0XElementComboBox 0xfeeb0, size 0xe8
    int m_bHasEditBox;         // 0xa8
    int m_bHasDropDownList;    // 0xac
    int m_bResizeDropDownList; // 0xb0
    int _pad_b4;
    RI_Array m_arItems;        // 0xb8  CStringArray
    CString  m_strValue;       // 0xe0  (shadows XElementEdit::m_strValue)
};
static_assert(sizeof(RI_XElementComboBox) == 0xe8, "XElementComboBox size");
static_assert(offsetof(RI_XElementComboBox, m_arItems) == 0xb8, "XElementComboBox::m_arItems");

struct RI_XElementFontComboBox : RI_XElementComboBox { // ??0XElementFontComboBox 0xff080
    int m_nFontType;             // 0xe8
    unsigned char m_nCharSet;    // 0xec
    unsigned char m_nPitchAndFamily; // 0xed
};
static_assert(offsetof(RI_XElementFontComboBox, m_nCharSet) == 0xec, "XElementFontComboBox::m_nCharSet");

struct RI_XElementButtonGallery : RI_XElementButton { // ??0XElementButtonGallery 0xff200, size 0x280
    int m_bIsButtonMode;       // 0x88
    int m_bEnableMenuResize;   // 0x8c
    int m_bMenuResizeVertical; // 0x90
    int m_nIconsInRow;         // 0x94
    long m_sizeIcon_cx;        // 0x98
    long m_sizeIcon_cy;        // 0x9c
    RI_XImage m_Images;        // 0xa0
    RI_Array  m_arGroups;      // 0x258
};
static_assert(sizeof(RI_XElementButtonGallery) == 0x280, "XElementButtonGallery size");
static_assert(offsetof(RI_XElementButtonGallery, m_arGroups) == 0x258, "XElementButtonGallery::m_arGroups");

// sizeof == 0x10 is the size ~XElementButtonGallery passes to operator delete
// for each group (RVA 0xff2ec).
struct RI_XGalleryGroup {
    CString m_strName;         // 0x00
    int     m_nItems;          // 0x08
    int     _pad_0c;
};
static_assert(sizeof(RI_XGalleryGroup) == 0x10, "XGalleryGroup size");

struct RI_XElementButtonColor : RI_XElementButtonGallery { // ??0XElementButtonColor 0xff340
    unsigned long m_clrColor;             // 0x280
    int m_bSimpleButtonLook;              // 0x284
    CString m_strAutomaticBtnLabel;       // 0x288
    CString m_strAutomaticBtnToolTip;     // 0x290
    unsigned long m_clrAutomaticBtnColor; // 0x298
    int m_bAutomaticBtnOnTop;             // 0x29c
    int m_bAutomaticBtnBorder;            // 0x2a0
    int _pad_2a4;
    CString m_strOtherBtnLabel;           // 0x2a8
    CString m_strOtherBtnToolTip;         // 0x2b0
};
static_assert(offsetof(RI_XElementButtonColor, m_strOtherBtnToolTip) == 0x2b0,
              "XElementButtonColor::m_strOtherBtnToolTip");

struct RI_XPanel : RI_XBase {  // ??0XPanel 0xff890 / ??1XPanel 0xff990, size 0xe0
    CString m_strName;         // 0x10
    CString m_strKeys;         // 0x18
    int m_nImageIndex;         // 0x20
    int m_bJustifyColumns;     // 0x24
    int m_bCenterColumnVert;   // 0x28
    int _pad_2c;
    RI_XElementButton m_btnLaunch; // 0x30  XElementButtonLaunch adds no members
    RI_Array m_arElements;         // 0xb8
};
static_assert(sizeof(RI_XPanel) == 0xe0, "XPanel size");
static_assert(offsetof(RI_XPanel, m_btnLaunch) == 0x30, "XPanel::m_btnLaunch");
static_assert(offsetof(RI_XPanel, m_arElements) == 0xb8, "XPanel::m_arElements");

struct RI_XCategory : RI_XBase { // ??0XCategory 0xffa80 / ??1XCategory 0xffba0, size 0x3e0
    CString   m_strName;       // 0x10
    CString   m_strKeys;       // 0x18
    RI_XImage m_SmallImages;   // 0x20
    RI_XImage m_LargeImages;   // 0x1d8
    RI_Array  m_arPanels;      // 0x390
    RI_Array  m_arElements;    // 0x3b8
};
static_assert(sizeof(RI_XCategory) == 0x3e0, "XCategory size");
static_assert(offsetof(RI_XCategory, m_arPanels) == 0x390, "XCategory::m_arPanels");
static_assert(offsetof(RI_XCategory, m_arElements) == 0x3b8, "XCategory::m_arElements");

struct RI_XCategoryMain : RI_XBase { // ??0XCategoryMain 0xffe50 / ??1XCategoryMain 0xfff50
    CString   m_strName;            // 0x10
    RI_XImage m_SmallImages;        // 0x18
    RI_XImage m_LargeImages;        // 0x1d0
    RI_Array  m_arElements;         // 0x388
    int       m_bRecentListEnable;  // 0x3b0
    int       _pad_3b4;
    CString   m_strRecentListLabel; // 0x3b8
    int       m_nRecentListWidth;   // 0x3c0
    int       _pad_3c4;
};
static_assert(sizeof(RI_XCategoryMain) == 0x3c8, "XCategoryMain size");
static_assert(offsetof(RI_XCategoryMain, m_arElements) == 0x388, "XCategoryMain::m_arElements");
static_assert(offsetof(RI_XCategoryMain, m_nRecentListWidth) == 0x3c0, "XCategoryMain::m_nRecentListWidth");

struct RI_XContext : RI_XBase { // ??0XContext 0xffcf0 / ??1XContext 0xffda0, size 0x58
    RI_XID   m_ID;             // 0x10
    CString  m_strText;        // 0x20
    int      m_Color;          // 0x28  AFX_RibbonCategoryColor
    int      _pad_2c;
    RI_Array m_arCategories;   // 0x30
};
static_assert(sizeof(RI_XContext) == 0x58, "XContext size");
static_assert(offsetof(RI_XContext, m_arCategories) == 0x30, "XContext::m_arCategories");

struct RI_XQATItem {           // ??0XQATItem 0x100040, size 0x18 (AddItem strides by 0x18)
    RI_XID m_ID;               // 0x00
    int    m_bVisible;         // 0x10
    int    _pad_14;
};
static_assert(sizeof(RI_XQATItem) == 0x18, "XQATItem size");

struct RI_XQAT : RI_XBase {    // ??0XQAT 0x100070 / ??1XQAT 0x1000f0, size 0x40
    RI_Array m_arItems;        // 0x10
    int      m_bOnTop;         // 0x38
    int      _pad_3c;
};
static_assert(sizeof(RI_XQAT) == 0x40, "XQAT size");
static_assert(offsetof(RI_XQAT, m_bOnTop) == 0x38, "XQAT::m_bOnTop");

struct RI_XRibbonBar : RI_XBase { // ??0XRibbonBar 0x100180 / ??1XRibbonBar 0x1002d0, size 0x4b0
    int m_bToolTip;            // 0x10
    int m_bToolTipDescr;       // 0x14
    int m_bKeyTips;            // 0x18
    int m_bPrintPreview;       // 0x1c
    int m_bDrawUsingFont;      // 0x20
    int _pad_24;
    RI_XImage m_Images;        // 0x28
    void*     m_MainCategory;  // 0x1e0  XCategoryMain*
    RI_XQAT   m_QAT;           // 0x1e8
    RI_XElementGroup m_TabElements; // 0x228
    RI_Array  m_arCategories;  // 0x458
    RI_Array  m_arContexts;    // 0x480
    void*     m_btnMain;       // 0x4a8  XElementButtonApplication*
};
static_assert(sizeof(RI_XRibbonBar) == 0x4b0, "XRibbonBar size");
static_assert(offsetof(RI_XRibbonBar, m_QAT) == 0x1e8, "XRibbonBar::m_QAT");
static_assert(offsetof(RI_XRibbonBar, m_TabElements) == 0x228, "XRibbonBar::m_TabElements");
static_assert(offsetof(RI_XRibbonBar, m_btnMain) == 0x4a8, "XRibbonBar::m_btnMain");

struct RI_XInfoError {         // ??0XInfoError 0xfe440 / ??1XInfoError 0x31c90, size 0x18
    int     m_Error;           // 0x00
    int     _pad_04;
    CString m_strDescription;  // 0x08
    long    m_nLine;           // 0x10
    long    m_nLinePos;        // 0x14
};
static_assert(sizeof(RI_XInfoError) == 0x18, "XInfoError size");

// The retail bodies build an empty CString by loading the string manager's nil
// CStringData and storing the address just past its 24-byte header; the OpenMFC
// CString default constructor is that same operation.
void ri_str_init(CString& s)    { ::new (static_cast<void*>(&s)) CString(); }
// ... and the retail bodies release one by decrementing CStringData::nRefs and
// calling pStringMgr->Free() when it drops to zero, which is ~CString here.
void ri_str_destroy(CString& s) { s.~CString(); }

} // namespace

// Symbol: ??0CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0XBase@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XBase_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    // Retail RVA 0xfe5c0: m_ElementType = type (+0x08), then the XBase vtable
    // (0x180303e20) into +0x00 -- see deviation (1) -- and returns this.
    RI_XBase* p = static_cast<RI_XBase*>(pThis);
    p->m_ElementType = type;
    p->vfptr = nullptr;
    return pThis;
}
// Symbol: ??0XCategory@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XCategory_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xffa80: m_ElementType = e_TypeCategory (18) and the XCategory
    // vtable inlined from XBase, m_strName (+0x10) and m_strKeys (+0x18) empty,
    // XImage::XImage() on m_SmallImages (+0x20) and m_LargeImages (+0x1d8), then
    // m_arPanels (+0x390) and m_arElements (+0x3b8) zeroed behind their CArray
    // vtables.
    RI_XCategory* p = static_cast<RI_XCategory*>(pThis);
    p->m_ElementType = 18;
    p->vfptr = nullptr;
    ri_str_init(p->m_strName);
    ri_str_init(p->m_strKeys);
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_SmallImages);
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_LargeImages);
    ri_array_init(p->m_arPanels);
    ri_array_init(p->m_arElements);
    return pThis;
}
// Symbol: ??0XCategoryMain@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XCategoryMain_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xffe50: m_ElementType = e_TypeCategoryMain (20), m_strName
    // (+0x10) empty, XImage::XImage() on m_SmallImages (+0x18) and m_LargeImages
    // (+0x1d0), m_arElements (+0x388) zeroed, m_bRecentListEnable = FALSE,
    // m_strRecentListLabel empty and m_nRecentListWidth = 300.
    RI_XCategoryMain* p = static_cast<RI_XCategoryMain*>(pThis);
    p->m_ElementType = 20;
    p->vfptr = nullptr;
    ri_str_init(p->m_strName);
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_SmallImages);
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_LargeImages);
    ri_array_init(p->m_arElements);
    p->m_bRecentListEnable = 0;
    ri_str_init(p->m_strRecentListLabel);
    p->m_nRecentListWidth = 300;
    return pThis;
}
// Symbol: ??0XContext@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XContext_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xffcf0: m_ElementType = e_TypeContext (19), XID::XID() on m_ID
    // (+0x10), m_strText (+0x20) empty, m_Color = 0 (+0x28) and m_arCategories
    // (+0x30) zeroed behind its CArray vtable.
    RI_XContext* p = static_cast<RI_XContext*>(pThis);
    p->m_ElementType = 19;
    p->vfptr = nullptr;
    impl___0XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
    ri_str_init(p->m_strText);
    p->m_Color = 0;
    ri_array_init(p->m_arCategories);
    return pThis;
}
// Symbol: ??0XElement@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    // Retail RVA 0xfe620: XBase's initialisation is inlined (m_ElementType = type
    // at +0x08, vtable 0x180303e00 at +0x00), then XID::XID() on m_ID (+0x10),
    // the five CStrings at +0x20..+0x40 built empty, and one 8-byte zero store
    // covering m_bIsOnPaletteTop (+0x48) and m_bIsAlwaysLarge (+0x4c).
    RI_XElement* p = static_cast<RI_XElement*>(pThis);
    p->m_ElementType = type;
    p->vfptr = nullptr;
    impl___0XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
    ri_str_init(p->m_strText);
    ri_str_init(p->m_strToolTip);
    ri_str_init(p->m_strDescription);
    ri_str_init(p->m_strKeys);
    ri_str_init(p->m_strMenuKeys);
    p->m_bIsOnPaletteTop = 0;
    p->m_bIsAlwaysLarge  = 0;
    return pThis;
}
// Symbol: ??0XElementButton@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    // Retail RVA 0xfe9e0: XElement(type), vtable 0x180303da0, then
    // m_bIsDefaultCommand = TRUE (+0x58), m_nSmallImageIndex = m_nLargeImageIndex
    // = -1 (+0x50/+0x54) and m_arSubItems zeroed behind its CArray vtable.
    // NOTE: this protected constructor writes only four bytes at +0x58, so
    // m_bIsAlwaysShowDescription (+0x5c) is left uninitialised -- the public
    // XElementButton() below stores both BOOLs with one 8-byte move.  That
    // asymmetry is retail's, and is reproduced here rather than papered over.
    RI_XElementButton* p = static_cast<RI_XElementButton*>(pThis);
    impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, type);
    p->m_bIsDefaultCommand   = 1;
    p->m_nSmallImageIndex    = -1;
    p->m_nLargeImageIndex    = -1;
    ri_array_init(p->m_arSubItems);
    return pThis;
}
// Symbol: ??0XElementButton@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButton_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfea80: XElement(e_TypeButton = 0) -- it calls XElement's
    // constructor directly, not the protected XElementButton(type) -- then the
    // same field initialisation, except that the store at +0x58 is eight bytes
    // wide here, so m_bIsAlwaysShowDescription (+0x5c) is set to FALSE too.
    RI_XElementButton* p = static_cast<RI_XElementButton*>(pThis);
    impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 0);
    p->m_bIsDefaultCommand        = 1;
    p->m_bIsAlwaysShowDescription = 0;
    p->m_nSmallImageIndex         = -1;
    p->m_nLargeImageIndex         = -1;
    ri_array_init(p->m_arSubItems);
    return pThis;
}
// Symbol: ??0XElementButtonApplication@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonApplication_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff640: XElementButton(e_TypeButton_Application = 6), vtable
    // 0x180303c40, then XImage::XImage() on m_Image (+0x88).
    RI_XElementButtonApplication* p = static_cast<RI_XElementButtonApplication*>(pThis);
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 6);
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Image);
    return pThis;
}
// Symbol: ??0XElementButtonCheck@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonCheck_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfebe0: XElementButton(e_TypeButton_Check = 1) and the
    // XElementButtonCheck vtable (0x180303d60); the class adds no members.
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 1);
    return pThis;
}
// Symbol: ??0XElementButtonColor@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonColor_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff340: XElementButtonGallery(e_TypeButton_Color = 2), vtable
    // 0x180303ca0, an 8-byte zero at +0x280 (m_clrColor = 0,
    // m_bSimpleButtonLook = FALSE), the two automatic-button strings empty,
    // m_clrAutomaticBtnColor = 0, an 8-byte store of 1 at +0x29c
    // (m_bAutomaticBtnOnTop = TRUE, m_bAutomaticBtnBorder = FALSE), the two
    // other-button strings empty, and finally it overwrites the gallery
    // defaults: m_sizeIcon = (22,22) and m_nIconsInRow = 5.
    RI_XElementButtonColor* p = static_cast<RI_XElementButtonColor*>(pThis);
    impl___0XElementButtonGallery_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 2);
    p->m_clrColor           = 0;
    p->m_bSimpleButtonLook  = 0;
    ri_str_init(p->m_strAutomaticBtnLabel);
    ri_str_init(p->m_strAutomaticBtnToolTip);
    p->m_clrAutomaticBtnColor = 0;
    p->m_bAutomaticBtnOnTop   = 1;
    p->m_bAutomaticBtnBorder  = 0;
    ri_str_init(p->m_strOtherBtnLabel);
    ri_str_init(p->m_strOtherBtnToolTip);
    p->m_sizeIcon_cx = 22;
    p->m_sizeIcon_cy = 22;
    p->m_nIconsInRow = 5;
    return pThis;
}
// Symbol: ??0XElementButtonGallery@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElementButtonGallery_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    // Retail RVA 0xff120: XElementButton(type), vtable 0x180303cc0, then an
    // 8-byte store at +0x88 (m_bIsButtonMode = TRUE, m_bEnableMenuResize =
    // FALSE), m_bMenuResizeVertical = FALSE, m_nIconsInRow = -1, an 8-byte zero
    // at +0x98 (m_sizeIcon = (0,0)), XImage::XImage() on m_Images (+0xa0) and
    // m_arGroups zeroed behind its CArray vtable.
    RI_XElementButtonGallery* p = static_cast<RI_XElementButtonGallery*>(pThis);
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, type);
    p->m_bIsButtonMode       = 1;
    p->m_bEnableMenuResize   = 0;
    p->m_bMenuResizeVertical = 0;
    p->m_nIconsInRow         = -1;
    p->m_sizeIcon_cx         = 0;
    p->m_sizeIcon_cy         = 0;
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
    ri_array_init(p->m_arGroups);
    return pThis;
}
// Symbol: ??0XElementButtonGallery@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff200: identical body to the protected constructor above but
    // with the type fixed at e_TypeButton_Gallery = 4; it calls XElementButton's
    // constructor directly rather than delegating.
    RI_XElementButtonGallery* p = static_cast<RI_XElementButtonGallery*>(pThis);
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 4);
    p->m_bIsButtonMode       = 1;
    p->m_bEnableMenuResize   = 0;
    p->m_bMenuResizeVertical = 0;
    p->m_nIconsInRow         = -1;
    p->m_sizeIcon_cx         = 0;
    p->m_sizeIcon_cy         = 0;
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
    ri_array_init(p->m_arGroups);
    return pThis;
}
// Symbol: ??0XID@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XID_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfe520: m_Name = the string manager's nil string, m_Value = 0.
    RI_XID* p = static_cast<RI_XID*>(pThis);
    ri_str_init(p->m_Name);
    p->m_Value = 0;
    return pThis;
}
// Symbol: ??0XImage@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XImage_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfe560: XID::XID() on m_ID (+0x00) and m_ID_HDPI (+0x10),
    // then CMFCToolBarImages::CMFCToolBarImages() on m_Image (+0x20).
    // m_Image is constructed through this DLL's CMFCToolBarImages thunk, so the
    // sub-object occupies the same 0x198 bytes but carries this DLL's vtable,
    // not the retail one.
    RI_XImage* p = static_cast<RI_XImage*>(pThis);
    impl___0XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
    impl___0XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID_HDPI);
    impl___0CMFCToolBarImages__QEAA_XZ(p->m_Image);
    return pThis;
}
// Symbol: ??0XInfoError@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XInfoError_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfe440: m_strDescription (+0x08) default-constructed, then the
    // body of Empty() inlined -- m_Error = e_ErrorNone, m_nLine = m_nLinePos = -1
    // and m_strDescription.Empty() (a no-op on the just-built empty string).
    RI_XInfoError* p = static_cast<RI_XInfoError*>(pThis);
    ri_str_init(p->m_strDescription);
    p->m_Error    = 0;
    p->m_nLine    = -1;
    p->m_nLinePos = -1;
    return pThis;
}
// Symbol: ??0XPanel@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XPanel_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff890: m_ElementType = e_TypePanel (17), m_strName (+0x10) and
    // m_strKeys (+0x18) empty, m_nImageIndex = -1, an 8-byte zero at +0x24
    // (m_bJustifyColumns and m_bCenterColumnVert), XElementButtonLaunch's
    // constructor on m_btnLaunch (+0x30) and m_arElements (+0xb8) zeroed.
    RI_XPanel* p = static_cast<RI_XPanel*>(pThis);
    p->m_ElementType = 17;
    p->vfptr = nullptr;
    ri_str_init(p->m_strName);
    ri_str_init(p->m_strKeys);
    p->m_nImageIndex       = -1;
    p->m_bJustifyColumns   = 0;
    p->m_bCenterColumnVert = 0;
    impl___0XElementButtonLaunch_CMFCRibbonInfo__QEAA_XZ(&p->m_btnLaunch);
    ri_array_init(p->m_arElements);
    return pThis;
}
// Symbol: ??0XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x100070: m_ElementType = e_TypeQAT (21) with the XQAT vtable,
    // m_arItems (+0x10) zeroed behind the CArray<XQATItem,XQATItem> vtable, and
    // m_bOnTop = TRUE (+0x38).
    RI_XQAT* p = static_cast<RI_XQAT*>(pThis);
    p->m_ElementType = 21;
    p->vfptr = nullptr;
    ri_array_init(p->m_arItems);
    p->m_bOnTop = 1;
    return pThis;
}
// Symbol: ??0XQATItem@XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x100040: XID::XID() on m_ID (+0x00), m_bVisible = TRUE (+0x10).
    RI_XQATItem* p = static_cast<RI_XQATItem*>(pThis);
    impl___0XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
    p->m_bVisible = 1;
    return pThis;
}
// Symbol: ??0XRibbonBar@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonBar_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x100180: m_ElementType = e_TypeRibbonBar (22); m_bToolTip,
    // m_bToolTipDescr, m_bKeyTips and m_bPrintPreview = TRUE with
    // m_bDrawUsingFont = FALSE (the store at +0x1c is eight bytes wide and
    // covers m_bPrintPreview and m_bDrawUsingFont); XImage::XImage() on m_Images
    // (+0x28); m_MainCategory = NULL; the XQAT constructor inlined into m_QAT
    // (+0x1e8); XElementGroup::XElementGroup() on m_TabElements (+0x228);
    // m_arCategories (+0x458) and m_arContexts (+0x480) zeroed; m_btnMain = NULL.
    RI_XRibbonBar* p = static_cast<RI_XRibbonBar*>(pThis);
    p->m_ElementType = 22;
    p->vfptr = nullptr;
    p->m_bToolTip       = 1;
    p->m_bToolTipDescr  = 1;
    p->m_bKeyTips       = 1;
    p->m_bPrintPreview  = 1;
    p->m_bDrawUsingFont = 0;
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
    p->m_MainCategory = nullptr;
    p->m_QAT.m_ElementType = 21;
    p->m_QAT.vfptr = nullptr;
    ri_array_init(p->m_QAT.m_arItems);
    p->m_QAT.m_bOnTop = 1;
    impl___0XElementGroup_CMFCRibbonInfo__QEAA_XZ(&p->m_TabElements);
    ri_array_init(p->m_arCategories);
    ri_array_init(p->m_arContexts);
    p->m_btnMain = nullptr;
    return pThis;
}
// Symbol: ??0XRibbonInfoParser@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParser_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0XRibbonInfoParserCollection@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParserCollection_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0XRibbonInfoParserRoot@CMFCRibbonInfo@@IEAA@XZ
extern "C" void* MS_ABI impl___0XRibbonInfoParserRoot_CMFCRibbonInfo__IEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ??1CMFCRibbonInfo@@UEAA@XZ
extern "C" void* MS_ABI impl___1CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    return pThis;
}

// Symbol: ?AddElementImages@CMFCRibbonInfo@@IEAAXAEAVXElement@1@AEAV?$CArray@PEAVXImage@CMFCRibbonInfo@@PEAV12@@@@Z
extern "C" void MS_ABI impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
    void* pThis, void* pElement, void* pImages) {
    (void)pThis;
    (void)pElement;
    (void)pImages;
}

// Symbol: ?ElementNameFromType@CMFCRibbonInfo@@SAPEB_WW4XElementType@1@@Z
extern "C" const wchar_t* MS_ABI impl__ElementNameFromType_CMFCRibbonInfo__SAPEB_WW4XElementType_1__Z(int type) {
    (void)type;
    return L"";
}

// Symbol: ?ElementTypeFromName@CMFCRibbonInfo@@SA?AW4XElementType@1@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" unsigned int MS_ABI impl__ElementTypeFromName_CMFCRibbonInfo__SA_AW4XElementType_1_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pName) {
    (void)pName;
    return 0;
}

// Symbol: ?GetArrayImages@CMFCRibbonInfo@@QEAAXAEAV?$CArray@PEAVXImage@CMFCRibbonInfo@@PEAV12@@@@Z
extern "C" void MS_ABI impl__GetArrayImages_CMFCRibbonInfo__QEAAXAEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
    void* pThis, void* pArray) {
    (void)pThis;
    (void)pArray;
}

// Symbol: ?Read@CMFCRibbonInfo@@MEAAHAEAVXRibbonInfoParser@1@@Z
extern "C" int MS_ABI impl__Read_CMFCRibbonInfo__MEAAHAEAVXRibbonInfoParser_1__Z(void* pThis, void* pParser) {
    (void)pThis;
    (void)pParser;
    return 0;
}

// Symbol: ?Read@CMFCRibbonInfo@@UEAAHPEAEI@Z
extern "C" int MS_ABI impl__Read_CMFCRibbonInfo__UEAAHPEAEI_Z(
    void* pData, unsigned int cb) {
    (void)pData;
    (void)cb;
    return 0;
}

// Symbol: ?Read@CMFCRibbonInfo@@UEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__Read_CMFCRibbonInfo__UEAAHPEAUIStream___Z(void* pThis, void* pStream) {
    (void)pThis;
    (void)pStream;
    return 0;
}

// Symbol: ?Write@CMFCRibbonInfo@@MEAAHAEAVXRibbonInfoParser@1@@Z
extern "C" int MS_ABI impl__Write_CMFCRibbonInfo__MEAAHAEAVXRibbonInfoParser_1__Z(void* pThis, void* pParser) {
    (void)pThis;
    (void)pParser;
    return 0;
}

// Symbol: ?Write@CMFCRibbonInfo@@UEAAHPEAPEAEAEAI@Z
extern "C" int MS_ABI impl__Write_CMFCRibbonInfo__UEAAHPEAPEAEAEAI_Z(
    unsigned char** ppData, unsigned int* pcData) {
    (void)ppData;
    (void)pcData;
    return 0;
}

// Symbol: ?Write@CMFCRibbonInfo@@UEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__Write_CMFCRibbonInfo__UEAAHPEAUIStream___Z(void* pThis, void* pStream) {
    (void)pThis;
    (void)pStream;
    return 0;
}

// Symbol: ??1XRibbonInfoParserCollection@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonInfoParserCollection_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??1XRibbonInfoParserRoot@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonInfoParserRoot_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ??0XElementButtonLaunch@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonLaunch_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff5c0: XElementButton(e_TypeButton_Launch = 8) and the
    // XElementButtonLaunch vtable (0x180303c60); the class adds no members.
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 8);
    return pThis;
}

// Symbol: ??0XElementButtonLinkCtrl@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonLinkCtrl_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfec60: XElementButton(e_TypeButton_LinkCtrl = 5), vtable
    // 0x180303d40, then m_strLink (+0x88) built empty.
    RI_XElementButtonLinkCtrl* p = static_cast<RI_XElementButtonLinkCtrl*>(pThis);
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 5);
    ri_str_init(p->m_strLink);
    return pThis;
}

// Symbol: ??0XElementButtonMainPanel@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonMainPanel_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff6f0: XElementButton(e_TypeButton_MainPanel = 7) and the
    // XElementButtonMainPanel vtable (0x180303c20); the class adds no members.
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 7);
    return pThis;
}

// Symbol: ??0XElementButtonUndo@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementButtonUndo_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff540: XElementButtonGallery(e_TypeButton_Undo = 3) and the
    // XElementButtonUndo vtable (0x180303c80); the class adds no members.
    impl___0XElementButtonGallery_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 3);
    return pThis;
}

// Symbol: ??0XElementComboBox@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElementComboBox_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    // Retail RVA 0xfeeb0: XElementEdit(type), vtable 0x180303d00, then
    // m_bHasEditBox = FALSE, m_bHasDropDownList = TRUE, m_bResizeDropDownList =
    // TRUE, m_arItems (+0xb8) zeroed behind the CStringArray vtable (that
    // vtable pointer is deviation (1); see ~XElementComboBox for why this
    // member is not handed to OpenMFC's CStringArray),
    // XElementComboBox's own m_strValue (+0xe0) built empty, and finally the
    // inherited XElementEdit::m_nWidth (+0x88) overwritten with 108.
    RI_XElementComboBox* p = static_cast<RI_XElementComboBox*>(pThis);
    impl___0XElementEdit_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, type);
    p->m_bHasEditBox         = 0;
    p->m_bHasDropDownList    = 1;
    p->m_bResizeDropDownList = 1;
    ri_array_init(p->m_arItems);
    ri_str_init(p->m_strValue);
    p->m_nWidth = 108;
    return pThis;
}

// Symbol: ??0XElementComboBox@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementComboBox_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfef80: the same body as the protected constructor above with
    // the type fixed at e_TypeComboBox = 11; it calls XElementEdit's constructor
    // directly rather than delegating.
    RI_XElementComboBox* p = static_cast<RI_XElementComboBox*>(pThis);
    impl___0XElementEdit_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 11);
    p->m_bHasEditBox         = 0;
    p->m_bHasDropDownList    = 1;
    p->m_bResizeDropDownList = 1;
    ri_array_init(p->m_arItems);
    ri_str_init(p->m_strValue);
    p->m_nWidth = 108;
    return pThis;
}

// Symbol: ??0XElementEdit@CMFCRibbonInfo@@IEAA@W4XElementType@1@@Z
extern "C" void* MS_ABI impl___0XElementEdit_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(
    void* pThis, int type) {
    // Retail RVA 0xfed40: XElementButton(type), vtable 0x180303d20, an 8-byte
    // zero at +0x88 (m_nWidth and m_nWidthFloaty), m_bHasSpinButtons = FALSE,
    // m_nMin = m_nMax = 0x7fffffff, and m_strValue (+0xa0) built empty.
    RI_XElementEdit* p = static_cast<RI_XElementEdit*>(pThis);
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, type);
    p->m_nWidth          = 0;
    p->m_nWidthFloaty    = 0;
    p->m_bHasSpinButtons = 0;
    p->m_nMin            = 0x7fffffff;
    p->m_nMax            = 0x7fffffff;
    ri_str_init(p->m_strValue);
    return pThis;
}

// Symbol: ??0XElementEdit@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementEdit_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfedf0: the same body as the protected constructor above with
    // the type fixed at e_TypeEdit = 10; it calls XElementButton's constructor
    // directly rather than delegating.
    RI_XElementEdit* p = static_cast<RI_XElementEdit*>(pThis);
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 10);
    p->m_nWidth          = 0;
    p->m_nWidthFloaty    = 0;
    p->m_bHasSpinButtons = 0;
    p->m_nMin            = 0x7fffffff;
    p->m_nMax            = 0x7fffffff;
    ri_str_init(p->m_strValue);
    return pThis;
}

// Symbol: ??0XElementFontComboBox@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementFontComboBox_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff080: XElementComboBox(e_TypeComboBox_Font = 12), vtable
    // 0x180303ce0, m_nFontType = 7 (DEVICE_FONTTYPE|RASTER_FONTTYPE|
    // TRUETYPE_FONTTYPE), a 2-byte store of 1 at +0xec covering m_nCharSet = 1
    // (DEFAULT_CHARSET) and m_nPitchAndFamily = 0, and finally the inherited
    // m_bHasEditBox (+0xa8) set to TRUE.
    RI_XElementFontComboBox* p = static_cast<RI_XElementFontComboBox*>(pThis);
    impl___0XElementComboBox_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 12);
    p->m_nFontType        = 7;
    p->m_nCharSet         = 1;
    p->m_nPitchAndFamily  = 0;
    p->m_bHasEditBox      = 1;
    return pThis;
}

// Symbol: ??0XElementGroup@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementGroup_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfe8b0: XElement(e_TypeGroup = 16), vtable 0x180303dc0,
    // XImage::XImage() on m_Images (+0x50) and m_arButtons (+0x208) zeroed
    // behind its CArray vtable.
    RI_XElementGroup* p = static_cast<RI_XElementGroup*>(pThis);
    impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 16);
    impl___0XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
    ri_array_init(p->m_arButtons);
    return pThis;
}

// Symbol: ??0XElementLabel@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementLabel_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfeb60: XElementButton(e_TypeLabel = 9) and the XElementLabel
    // vtable (0x180303d80); the class adds no members.
    impl___0XElementButton_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 9);
    return pThis;
}

// Symbol: ??0XElementSeparator@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementSeparator_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfe830: XElement(e_TypeSeparator = 15), vtable 0x180303de0 and
    // m_bIsHoriz = FALSE (+0x50).
    RI_XElementSeparator* p = static_cast<RI_XElementSeparator*>(pThis);
    impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 15);
    p->m_bIsHoriz = 0;
    return pThis;
}

// Symbol: ??0XElementSlider@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XElementSlider_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xff770: XElement(e_TypeSlider = 13), vtable 0x180303c00, then
    // two 8-byte stores of 100 at +0x54 and +0x5c and two 4-byte zeroes: they
    // land as m_dwStyle = 0, m_nWidth = 100, m_nMin = 0, m_nMax = 100,
    // m_nPos = 0 and m_bZoomButtons = FALSE.
    RI_XElementSlider* p = static_cast<RI_XElementSlider*>(pThis);
    impl___0XElement_CMFCRibbonInfo__IEAA_W4XElementType_1__Z(pThis, 13);
    p->m_dwStyle      = 0;
    p->m_nWidth       = 100;
    p->m_nMin         = 0;
    p->m_nMax         = 100;
    p->m_nPos         = 0;
    p->m_bZoomButtons = 0;
    return pThis;
}

// Symbol: ??0XGalleryGroup@XElementButtonGallery@CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // This export has no RVA of its own in the map because the linker folded it
    // onto XID::XID() (RVA 0xfe520) -- both classes are "one CString then one
    // 4-byte value".  XElementButtonGallery::Read (RVA 0x103390) shows the fold
    // at 0x1036c4: operator new(0x10), then a call to 0xfe520 to construct the
    // group, and on the failure path 0x2ff0 followed by operator delete(p,0x10).
    // So: m_strName empty, m_nItems = 0.
    RI_XGalleryGroup* p = static_cast<RI_XGalleryGroup*>(pThis);
    ri_str_init(p->m_strName);
    p->m_nItems = 0;
    return pThis;
}

// Symbol: ??1XBase@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XBase_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfe610: the whole body is the store of the XBase vtable
    // (0x180303e20) at +0x00; m_ElementType is a plain int.  OpenMFC does not
    // model that vtable (deviation 1), so there is nothing left to do.
    (void)pThis;
}

// Symbol: ??1XCategory@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XCategory_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xffba0: stores the XCategory vtable, then the destructor body
    // deletes every non-null element of m_arPanels (m_pData +0x398, m_nSize
    // +0x3a0) and of m_arElements (m_pData +0x3c0, m_nSize +0x3c8) through each
    // element's vtable slot 0; the implicit member teardown then runs the CArray
    // destructors on m_arElements (+0x3b8) and m_arPanels (+0x390), ~XImage on
    // m_LargeImages (+0x1d8) and m_SmallImages (+0x20), releases m_strKeys
    // (+0x18) and m_strName (+0x10), and stores the XBase vtable.
    // The two element loops and the two buffer frees are deviation (2).
    RI_XCategory* p = static_cast<RI_XCategory*>(pThis);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_LargeImages);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_SmallImages);
    ri_str_destroy(p->m_strKeys);
    ri_str_destroy(p->m_strName);
}

// Symbol: ??1XCategoryMain@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XCategoryMain_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfff50: stores the XCategoryMain vtable, deletes every
    // non-null element of m_arElements (m_pData +0x390, m_nSize +0x398) through
    // its vtable slot 0, then releases m_strRecentListLabel (+0x3b8), runs the
    // CArray destructor on m_arElements (+0x388), ~XImage on m_LargeImages
    // (+0x1d0) and m_SmallImages (+0x18), releases m_strName (+0x10) and stores
    // the XBase vtable.  The element loop and the buffer free are deviation (2).
    RI_XCategoryMain* p = static_cast<RI_XCategoryMain*>(pThis);
    ri_str_destroy(p->m_strRecentListLabel);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_LargeImages);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_SmallImages);
    ri_str_destroy(p->m_strName);
}

// Symbol: ??1XContext@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XContext_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xffda0: stores the XContext vtable, deletes every non-null
    // element of m_arCategories (m_pData +0x38, m_nSize +0x40) through its
    // vtable slot 0, runs the CArray destructor on m_arCategories (+0x30),
    // releases m_strText (+0x20), runs ~XID on m_ID (+0x10) and stores the XBase
    // vtable.  The element loop and the buffer free are deviation (2).
    RI_XContext* p = static_cast<RI_XContext*>(pThis);
    ri_str_destroy(p->m_strText);
    impl___1XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
}

// Symbol: ??1XElement@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElement_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfe730: stores the XElement vtable, releases the five CStrings
    // in reverse declaration order (+0x40, +0x38, +0x30, +0x28, +0x20), calls
    // ~XID on m_ID (+0x10) and finally stores the XBase vtable.  Both vtable
    // stores are deviation (1).
    RI_XElement* p = static_cast<RI_XElement*>(pThis);
    ri_str_destroy(p->m_strMenuKeys);
    ri_str_destroy(p->m_strKeys);
    ri_str_destroy(p->m_strDescription);
    ri_str_destroy(p->m_strToolTip);
    ri_str_destroy(p->m_strText);
    impl___1XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
}

// Symbol: ??1XElementButton@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfeae0: stores the XElementButton vtable, walks m_arSubItems
    // (m_pData +0x68, m_nSize +0x70) deleting every non-null element through its
    // own vtable slot 0 with the "delete" flag, runs the CArray destructor on
    // m_arSubItems (+0x60) -- which frees the buffer -- and tail-calls
    // ~XElement.  The element loop and the buffer free are deviation (2): they
    // are skipped here and leak.
    impl___1XElement_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonApplication@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonApplication_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff6c0: stores the XElementButtonApplication vtable, runs
    // ~XImage on m_Image (+0x88) and tail-calls ~XElementButton.
    RI_XElementButtonApplication* p = static_cast<RI_XElementButtonApplication*>(pThis);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Image);
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonCheck@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonCheck_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfec50: stores the XElementButtonCheck vtable (deviation 1)
    // and tail-calls ~XElementButton; the class adds no members.
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonColor@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonColor_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff470: stores the XElementButtonColor vtable, releases the
    // four CStrings in reverse declaration order (+0x2b0, +0x2a8, +0x290,
    // +0x288) and tail-calls ~XElementButtonGallery.
    RI_XElementButtonColor* p = static_cast<RI_XElementButtonColor*>(pThis);
    ri_str_destroy(p->m_strOtherBtnToolTip);
    ri_str_destroy(p->m_strOtherBtnLabel);
    ri_str_destroy(p->m_strAutomaticBtnToolTip);
    ri_str_destroy(p->m_strAutomaticBtnLabel);
    impl___1XElementButtonGallery_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonGallery@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonGallery_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff2a0: stores the XElementButtonGallery vtable, walks
    // m_arGroups (m_pData +0x260, m_nSize +0x268) calling ~XGalleryGroup and
    // operator delete(p, 0x10) on every non-null group, runs the CArray
    // destructor on m_arGroups (+0x258), runs ~XImage on m_Images (+0xa0) and
    // tail-calls ~XElementButton.
    // XGalleryGroup has no virtual functions, so unlike the other owned-pointer
    // arrays in this file its elements CAN be destroyed without a vtable, and
    // they are.  What is skipped is the operator delete of each group block and
    // the CArray buffer free: which allocator owns either is not determinable
    // from here, so both are leaked -- deviation (2).
    RI_XElementButtonGallery* p = static_cast<RI_XElementButtonGallery*>(pThis);
    RI_XGalleryGroup** groups = static_cast<RI_XGalleryGroup**>(p->m_arGroups.m_pData);
    for (long long i = 0; groups != nullptr && i < p->m_arGroups.m_nSize; ++i) {
        if (groups[i] != nullptr) {
            impl___1XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(groups[i]);
        }
    }
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonLaunch@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonLaunch_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff630: stores the XElementButtonLaunch vtable (deviation 1)
    // and tail-calls ~XElementButton; the class adds no members.
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonLinkCtrl@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonLinkCtrl_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfecf0: stores the XElementButtonLinkCtrl vtable, releases
    // m_strLink (+0x88) and tail-calls ~XElementButton.
    RI_XElementButtonLinkCtrl* p = static_cast<RI_XElementButtonLinkCtrl*>(pThis);
    ri_str_destroy(p->m_strLink);
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonMainPanel@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonMainPanel_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff760: stores the XElementButtonMainPanel vtable
    // (deviation 1) and tail-calls ~XElementButton; the class adds no members.
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementButtonUndo@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementButtonUndo_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff5b0: stores the XElementButtonUndo vtable (deviation 1) and
    // tail-calls ~XElementButtonGallery; the class adds no members.
    impl___1XElementButtonGallery_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementComboBox@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementComboBox_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff020: stores the XElementComboBox vtable, releases its own
    // m_strValue (+0xe0), calls ??1CStringArray@@UEAA@XZ (RVA 0x1d2d40) on
    // m_arItems (+0xb8) -- which destructs every CString in the buffer and then
    // frees it -- and tail-calls ~XElementEdit.
    // The m_arItems teardown is skipped here, and NOT for the reason the other
    // arrays are (deviation (2)): CStringArray is a fully exported non-template
    // class, so in retail that buffer is allocated inside the DLL and freeing
    // it here would be legitimate.  The blocker is that OpenMFC's CStringArray
    // (include/openmfc/afx.h + core/collections/CStringArray.cpp) does not use
    // the retail layout at all -- it declares no data members and keeps its
    // elements in a side table keyed by `this` -- so this DLL has no code that
    // can interpret, let alone free, the retail 0x28-byte CArray image sitting
    // at +0xb8.  impl___1CStringArray__UEAA_XZ is not a way out either: its
    // body is `pThis->~CStringArray()`, a virtual destructor call, so on the
    // nullptr vfptr deviation (1) leaves there it would fault outright, and
    // even paired with impl___0CStringArray__QEAA_XZ it would only add and
    // remove a side-table entry while planting a mingw/Itanium-layout vptr in a
    // member MSVC clients dispatch through -- silent wrong-slot dispatch
    // instead of the deterministic fault deviation (1) gives.  So the
    // constructor zero-fills the slot (as for every other embedded array here)
    // and the destructor leaves it alone; anything a client put in it leaks.
    RI_XElementComboBox* p = static_cast<RI_XElementComboBox*>(pThis);
    ri_str_destroy(p->m_strValue);
    impl___1XElementEdit_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementEdit@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementEdit_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfee60: stores the XElementEdit vtable, releases m_strValue
    // (+0xa0) and tail-calls ~XElementButton.
    RI_XElementEdit* p = static_cast<RI_XElementEdit*>(pThis);
    ri_str_destroy(p->m_strValue);
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementFontComboBox@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementFontComboBox_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff110: stores the XElementFontComboBox vtable (deviation 1)
    // and tail-calls ~XElementComboBox; its own members are plain integers.
    impl___1XElementComboBox_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementGroup@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementGroup_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfe950: stores the XElementGroup vtable, walks m_arButtons
    // (m_pData +0x210, m_nSize +0x218) deleting every non-null element through
    // its vtable slot 0, runs the CArray destructor on m_arButtons (+0x208),
    // runs ~XImage on m_Images (+0x50) and tail-calls ~XElement.  The element
    // loop and the buffer free are deviation (2) and are skipped.
    RI_XElementGroup* p = static_cast<RI_XElementGroup*>(pThis);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
    impl___1XElement_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementLabel@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementLabel_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfebd0: stores the XElementLabel vtable (deviation 1) and
    // tail-calls ~XElementButton; the class adds no members.
    impl___1XElementButton_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementSeparator@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementSeparator_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xfe8a0: stores the XElementSeparator vtable (deviation 1) and
    // tail-calls ~XElement; m_bIsHoriz is a plain BOOL.
    impl___1XElement_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XElementSlider@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XElementSlider_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff7f0: stores the XElementSlider vtable (deviation 1) and
    // tail-calls ~XElement; all of its own members are plain integers.
    impl___1XElement_CMFCRibbonInfo__UEAA_XZ(pThis);
}

// Symbol: ??1XGalleryGroup@XElementButtonGallery@CMFCRibbonInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Folded onto ~XID (RVA 0x2ff0) for the same reason the constructor is:
    // ~XElementButtonGallery (RVA 0xff2a0) calls 0x2ff0 at 0xff2e7 on each
    // XGalleryGroup* before operator delete(p, 0x10).  So: release m_strName.
    RI_XGalleryGroup* p = static_cast<RI_XGalleryGroup*>(pThis);
    ri_str_destroy(p->m_strName);
}

// Symbol: ??1XID@CMFCRibbonInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1XID_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x2ff0: releases m_Name (+0x00) through the CStringData
    // refcount, freeing it via its own string manager when it hits zero.
    RI_XID* p = static_cast<RI_XID*>(pThis);
    ri_str_destroy(p->m_Name);
}

// Symbol: ??1XImage@CMFCRibbonInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1XImage_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0xfe590: members destroyed in reverse order --
    // ~CMFCToolBarImages on m_Image (+0x20), ~XID on m_ID_HDPI (+0x10), then a
    // tail call to ~XID on m_ID (+0x00).
    RI_XImage* p = static_cast<RI_XImage*>(pThis);
    impl___1CMFCToolBarImages__UEAA_XZ(p->m_Image);
    impl___1XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID_HDPI);
    impl___1XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
}

// Symbol: ??1XInfoError@CMFCRibbonInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1XInfoError_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x31c90: releases m_strDescription (+0x08); the other three
    // members are plain integers.
    RI_XInfoError* p = static_cast<RI_XInfoError*>(pThis);
    ri_str_destroy(p->m_strDescription);
}

// Symbol: ??1XPanel@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XPanel_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0xff990: stores the XPanel vtable, walks m_arElements (m_pData
    // +0xc0, m_nSize +0xc8) deleting every non-null element through its vtable
    // slot 0, runs the CArray destructor on m_arElements (+0xb8), destroys
    // m_btnLaunch (+0x30) by storing the XElementButtonLaunch vtable and calling
    // ~XElementButton, releases m_strKeys (+0x18) and m_strName (+0x10), and
    // stores the XBase vtable.  The element loop and the buffer free are
    // deviation (2) and are skipped.
    RI_XPanel* p = static_cast<RI_XPanel*>(pThis);
    impl___1XElementButtonLaunch_CMFCRibbonInfo__UEAA_XZ(&p->m_btnLaunch);
    ri_str_destroy(p->m_strKeys);
    ri_str_destroy(p->m_strName);
}

// Symbol: ??1XQAT@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XQAT_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0x1000f0: stores the XQAT vtable, calls
    // CArray<XQATItem,XQATItem>::SetSize(0) on m_arItems (+0x10) -- which
    // destroys every XQATItem and frees the buffer -- then runs the CArray
    // destructor and stores the XBase vtable.
    // m_arItems holds XQATItems by value and their destructor is not virtual, so
    // this teardown needs no vtable; and this is the one array in the family
    // that this DLL allocates itself (XQAT::AddItem below, calloc/realloc), so
    // it is destroyed and freed for real here.  Caveat, stated because it is a
    // real hazard: if a client grew m_arItems with its own inline CArray code
    // the buffer came from the client's CRT and this free is a cross-allocator
    // free.  The same caveat applies to the realloc in AddItem.
    RI_XQAT* p = static_cast<RI_XQAT*>(pThis);
    RI_XQATItem* items = static_cast<RI_XQATItem*>(p->m_arItems.m_pData);
    for (long long i = 0; items != nullptr && i < p->m_arItems.m_nSize; ++i) {
        impl___1XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(&items[i]);
    }
    std::free(p->m_arItems.m_pData);
    p->m_arItems.m_pData   = nullptr;
    p->m_arItems.m_nSize   = 0;
    p->m_arItems.m_nMaxSize = 0;
}

// Symbol: ??1XQATItem@XQAT@CMFCRibbonInfo@@QEAA@XZ
extern "C" void MS_ABI impl___1XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x100060: a bare tail call to ~XID on m_ID (+0x00);
    // m_bVisible is a plain BOOL.
    RI_XQATItem* p = static_cast<RI_XQATItem*>(pThis);
    impl___1XID_CMFCRibbonInfo__QEAA_XZ(&p->m_ID);
}

// Symbol: ??1XRibbonBar@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonBar_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0x1002d0: stores the XRibbonBar vtable, deletes m_btnMain
    // (+0x4a8) and m_MainCategory (+0x1e0) through their vtable slot 0, deletes
    // every non-null element of m_arCategories (m_pData +0x460, m_nSize +0x468)
    // and m_arContexts (m_pData +0x488, m_nSize +0x490) the same way, then runs
    // the CArray destructors on m_arContexts (+0x480) and m_arCategories
    // (+0x458), ~XElementGroup on m_TabElements (+0x228), ~XQAT on m_QAT
    // (+0x1e8), ~XImage on m_Images (+0x28) and stores the XBase vtable.
    // The four delete paths and the two buffer frees are deviation (2).
    RI_XRibbonBar* p = static_cast<RI_XRibbonBar*>(pThis);
    impl___1XElementGroup_CMFCRibbonInfo__UEAA_XZ(&p->m_TabElements);
    impl___1XQAT_CMFCRibbonInfo__UEAA_XZ(&p->m_QAT);
    impl___1XImage_CMFCRibbonInfo__QEAA_XZ(&p->m_Images);
}

// Symbol: ??1XRibbonInfoParser@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonInfoParser_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0x109770: the entire body is the store of the
    // XRibbonInfoParser vtable (0x180303fa8) at +0x00.  The class is abstract
    // and holds no data members, and OpenMFC does not model the vtable
    // (deviation 1), so nothing is left to do.
    (void)pThis;
}

// Symbol: ?AddItem@XQAT@CMFCRibbonInfo@@QEAA_JAEBVXQATItem@12@@Z
extern "C" long long MS_ABI impl__AddItem_XQAT_CMFCRibbonInfo__QEAA_JAEBVXQATItem_12__Z(
    void* pThis, const void* pItem) {
    // Retail RVA 0x100130: copy-constructs rItem into a temporary (the CString
    // via CSimpleStringT::CloneData, then the two 4-byte fields) and passes it
    // by value to CArray<XQATItem,XQATItem>::Add (RVA 0xf3e84), which grows the
    // array by one with SetSize(m_nSize + 1), copy-constructs the element into
    // the new slot and returns the index the element went to.
    // The growth here reproduces retail's SetSize branch structure over
    // calloc/realloc -- see ri_array_append_slot above -- and uses the same
    // allocator family as the CArray helpers in
    // detail/CMFCRibbonQuickAccessToolBarDefaultStateSupport.cpp; see the
    // cross-allocator caveat on ~XQAT.  The element copy uses this DLL's CString
    // copy constructor, which shares the CStringData and bumps its refcount.
    // CloneData (RVA 0xde00) takes that same share-and-AddRef path only when
    // the source is unlocked AND pStringMgr->Clone() returns that same manager;
    // for a locked buffer -- which includes every empty/nil string -- or a
    // foreign manager it instead allocates a private copy.  Both outcomes are
    // an equal string, so the difference is one of sharing, not of value.
    RI_XQAT* p = static_cast<RI_XQAT*>(pThis);
    const RI_XQATItem* src = static_cast<const RI_XQATItem*>(pItem);
    const long long idx = ri_array_append_slot(p->m_arItems, sizeof(RI_XQATItem));
    if (idx < 0) {
        return -1;   // allocation failed; retail would have thrown CMemoryException
    }
    RI_XQATItem* dst = static_cast<RI_XQATItem*>(p->m_arItems.m_pData) + idx;
    ::new (static_cast<void*>(&dst->m_ID.m_Name)) CString(src->m_ID.m_Name);
    dst->m_ID.m_Value = src->m_ID.m_Value;
    dst->m_bVisible   = src->m_bVisible;
    return idx;
}

// Symbol: ?CreateFromTag@XBase@CMFCRibbonInfo@@SAPEAV12@AEAVXRibbonInfoParser@2@@Z
extern "C" void* MS_ABI impl__CreateFromTag_XBase_CMFCRibbonInfo__SAPEAV12_AEAVXRibbonInfoParser_2__Z() {
    return nullptr;
}

// Symbol: ?CreateFromType@XBase@CMFCRibbonInfo@@SAPEAV12@W4XElementType@2@@Z
extern "C" void* MS_ABI impl__CreateFromType_XBase_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z() {
    return nullptr;
}

// Symbol: ?CreateFromType@XElement@CMFCRibbonInfo@@SAPEAV12@W4XElementType@2@@Z
extern "C" void* MS_ABI impl__CreateFromType_XElement_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z() {
    return nullptr;
}

// Symbol: ?Empty@XInfoError@CMFCRibbonInfo@@QEAAXXZ
extern "C" void MS_ABI impl__Empty_XInfoError_CMFCRibbonInfo__QEAAXXZ() {}

// Symbol: ?Read@XCategory@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XCategoryMain@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XContext@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElement@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElement_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButton@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonApplication@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonApplication_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonCheck@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonCheck_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonColor@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonColor_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonGallery@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonLaunch@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonLaunch_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonLinkCtrl@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonLinkCtrl_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonMainPanel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonMainPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementButtonUndo@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonUndo_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementComboBox@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementEdit@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementEdit_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementFontComboBox@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementFontComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementGroup@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementGroup_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementLabel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementLabel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementSeparator@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementSeparator_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XElementSlider@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementSlider_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XGalleryGroup@XElementButtonGallery@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@3@@Z
extern "C" int MS_ABI impl__Read_XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XID@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XID_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XImage@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XImage_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XPanel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XQAT@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XQAT_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XQATItem@XQAT@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@3@@Z
extern "C" int MS_ABI impl__Read_XQATItem_XQAT_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Read@XRibbonBar@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?ReadBool@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadBool_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(const void* /*class*/* p0, void** p1, void* p2, int* p3) {
    return 0;
}

// Symbol: ?ReadColor@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAK@Z
extern "C" int MS_ABI impl__ReadColor_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAK_Z(const void* /*class*/* p0, void** p1, void* p2, unsigned long* p3) {
    return 0;
}

// Symbol: ?ReadInt@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadInt_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(const void* /*class*/* p0, void** p1, void* p2, int* p3) {
    return 0;
}

// Symbol: ?ReadSize@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAVCSize@@@Z
extern "C" int MS_ABI impl__ReadSize_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAVCSize___Z(const void* /*class*/* p0, void** p1, void* p2, void* /*class*/* p3) {
    return 0;
}

// Symbol: ?ReadUInt@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAI@Z
extern "C" int MS_ABI impl__ReadUInt_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAI_Z(const void* /*class*/* p0, void** p1, void* p2, unsigned int* p3) {
    return 0;
}

// Symbol: ?SetError@XInfoError@CMFCRibbonInfo@@QEAAXW4XError@12@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@JJ@Z
extern "C" void MS_ABI impl__SetError_XInfoError_CMFCRibbonInfo__QEAAXW4XError_12_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__JJ_Z(int /*enum*/ p0, unsigned char p1, void* p2, void* p3, void* p4, void* p5, unsigned char p6, void* p7, const void* /*class*/* p8, void** p9, void* p10, long p11, long p12) {}

// Symbol: ?Write@XBase@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XBase_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XCategory@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XCategoryMain@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XContext@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElement@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElement_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButton@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonApplication@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonApplication_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonCheck@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonCheck_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonColor@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonColor_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonGallery@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonLaunch@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonLaunch_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonLinkCtrl@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonLinkCtrl_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonMainPanel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonMainPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementButtonUndo@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementButtonUndo_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}

// Symbol: ?Write@XElementComboBox@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Write_XElementComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* /*class*/* p0) {
    return 0;
}
