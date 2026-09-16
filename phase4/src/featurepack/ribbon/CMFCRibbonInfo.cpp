// CMFCRibbonInfo — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"


#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <new>
#include <wchar.h>

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
//      MSVC-layout vtable for this family -- there is no vector deleting
//      destructor to put in slot 0, and every ?Write@X* thunk is still a
//      placeholder (the `return 0` stubs at the end of this file and the
//      RecordRibbonWrite bodies in featurepack/ribbon/Thunks.cpp) -- so the
//      constructors store nullptr there instead, and the destructors leave
//      the slot alone rather than rewriting it to the base-class vtable as
//      retail does.  A virtual call on one of these objects therefore faults
//      deterministically instead of dispatching.  Where a retail body itself
//      makes such a call (pElement->Read(rParser) in CreateFromTag; the
//      m_btnMain / m_MainCategory / m_QAT / m_TabElements Reads in
//      XRibbonBar::Read; m_btnLaunch.Read in XPanel::Read;
//      m_RibbonBar.Read/Write in CMFCRibbonInfo), the bodies below call the
//      target thunk directly or dispatch on XBase::m_ElementType
//      (ri_element_read); each site says so.  The same applies to the
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
//      leak-instead-of-crash choice, not a transcription of retail.  The
//      Read bodies below populate those same arrays themselves (elements
//      through the exported ??2@YAPEAX_K@Z, buffers through
//      ri_array_append_slot), and the destructors leak those too.
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
// Constructors of the element classes created by XBase/XElement::CreateFromType
// (all defined further down this file), the two exported operator new/delete
// (detail/MemcoreSupport.cpp -- retail's CreateFromType and
// XElementButtonGallery::Read call ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z, so these
// bodies do too), the CStringT trim exports (core/collections/CStringT.cpp)
// that retail ReadBool calls, and the XElementProgressBar / XRibbonBar::Write
// thunks that live in featurepack/ribbon/Thunks.cpp.
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);
extern "C" CString* MS_ABI impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis);
extern "C" CString* MS_ABI impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis);
extern "C" void* MS_ABI impl___0XElementButton_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonApplication_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonCheck_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonColor_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonLinkCtrl_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonMainPanel_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementButtonUndo_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementComboBox_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementEdit_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementFontComboBox_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementLabel_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementSeparator_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XElementSlider_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XPanel_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XCategory_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XCategoryMain_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XContext_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XQAT_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XRibbonBar_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void* MS_ABI impl___0XInfoError_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XInfoError_CMFCRibbonInfo__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1XRibbonBar_CMFCRibbonInfo__UEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__Empty_XInfoError_CMFCRibbonInfo__QEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetError_XInfoError_CMFCRibbonInfo__QEAAXW4XError_12_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__JJ_Z(void* pThis, int error, const CString* pDescription, long nLine, long nLinePos);
extern "C" unsigned int MS_ABI impl__ElementTypeFromName_CMFCRibbonInfo__SA_AW4XElementType_1_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CString* pName);
extern "C" void* MS_ABI impl__CreateFromTag_XBase_CMFCRibbonInfo__SAPEAV12_AEAVXRibbonInfoParser_2__Z(void* pParser);
extern "C" void* MS_ABI impl__CreateFromType_XBase_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(int type);
extern "C" void* MS_ABI impl__CreateFromType_XElement_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(int type);
extern "C" long long MS_ABI impl__AddItem_XQAT_CMFCRibbonInfo__QEAA_JAEBVXQATItem_12__Z(void* pThis, const void* pItem);
extern "C" void MS_ABI impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(void* pThis, void* pElement, void* pImages);
extern "C" int MS_ABI impl__Read_XID_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XImage_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElement_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonApplication_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonCheck_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonColor_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonLaunch_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonLinkCtrl_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonMainPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementButtonUndo_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementEdit_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementFontComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementGroup_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementLabel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementProgressBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementSeparator_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XElementSlider_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XQAT_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XQATItem_XQAT_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Read_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);
extern "C" int MS_ABI impl__Write_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser);

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

// Function-pointer shape of every ??0X*@CMFCRibbonInfo@@QEAA@XZ thunk.
typedef void* (MS_ABI* RI_CtorFn)(void*);

// CMFCRibbonInfo itself.  Offsets read out of ??0CMFCRibbonInfo (RVA 0x107b00,
// mfc140u) and ?Read@CMFCRibbonInfo@@MEAAHAEAVXRibbonInfoParser@1@@Z (RVA
// 0x10b280, mfc140u): m_sizeImage[2] at +0x08, m_RibbonBar at +0x18,
// m_dwVersion at +0x4c8, m_Error at +0x4d0.
struct RI_Size { long cx; long cy; };   // CSize, always passed by reference here
static_assert(sizeof(RI_Size) == 8, "CSize is two 32-bit longs");

struct RI_CMFCRibbonInfo {
    void*         vfptr;          // 0x00
    RI_Size       m_sizeImage[2]; // 0x08
    RI_XRibbonBar m_RibbonBar;    // 0x18
    unsigned long m_dwVersion;    // 0x4c8
    int           _pad_4cc;
    RI_XInfoError m_Error;        // 0x4d0
};
static_assert(sizeof(RI_CMFCRibbonInfo) == 0x4e8, "CMFCRibbonInfo size");
static_assert(offsetof(RI_CMFCRibbonInfo, m_RibbonBar) == 0x18, "CMFCRibbonInfo::m_RibbonBar");
static_assert(offsetof(RI_CMFCRibbonInfo, m_dwVersion) == 0x4c8, "CMFCRibbonInfo::m_dwVersion");
static_assert(offsetof(RI_CMFCRibbonInfo, m_Error) == 0x4d0, "CMFCRibbonInfo::m_Error");

// Retail sizes passed to operator new by XBase::CreateFromType (RVA 0xffc40,
// mfc140u) and XElement::CreateFromType (RVA 0xfffb0, mfc140u).  The layout
// views above are checked against every one of them here.
static_assert(sizeof(RI_XElementButtonLinkCtrl)    == 0x90,  "XElementButtonLinkCtrl size");
static_assert(sizeof(RI_XElementButtonApplication) == 0x240, "XElementButtonApplication size");
static_assert(sizeof(RI_XElementFontComboBox)      == 0xf0,  "XElementFontComboBox size");
static_assert(sizeof(RI_XElementButtonColor)       == 0x2b8, "XElementButtonColor size");
static_assert(sizeof(RI_XElementSlider)            == 0x68,  "XElementSlider size");
static_assert(sizeof(RI_XElementSeparator)         == 0x58,  "XElementSeparator size");

// CMFCToolBarImages::m_sizeImage sits at +0x68 of the sub-object (harvested
// layout in include/openmfc/afxmfc.h, "0x068 104"); retail CMFCRibbonInfo::Read
// stores the small size straight into m_RibbonBar.m_Images.m_Image at
// this+0xc8 = 0x18 + 0x28 + 0x20 + 0x68 (the store at 0x10b596 inside 0x10b280).
// The member is protected, so it is reached by offset rather than by name.
RI_Size& ri_image_size(RI_XImage& img) {
    return *reinterpret_cast<RI_Size*>(img.m_Image + 0x68);
}

// ---- the XRibbonInfoParser interface ----------------------------------------
// XRibbonInfoParser and XRibbonInfoParserCollection are abstract classes whose
// concrete objects -- and therefore vtables -- belong to whoever calls Read or
// Write: in retail the DLL-internal MSXML parsers (CXMLParser*, not exported,
// see the note on ?Read@CMFCRibbonInfo@@UEAAHPEAEI@Z below), in a client its
// own subclass.  Every retail Read body dispatches through the vtable at +0x00
// of the parser object, so these helpers do exactly that, by slot index.  Slot
// numbers follow the declaration order in afxribboninfo.h; each was confirmed
// at a retail call site (e.g. XID::Read loads +0x20 for ReadString and +0x48
// for ReadUInt, XImage::Read +0x50 for ReadValue, XElementButton::Read +0x10
// for Read and +0x18 for ReadCollection, CMFCRibbonInfo::Write +0x08 for Add,
// +0x70 for WriteSize and +0x80 for WriteUInt; on a collection
// XElementButton::Read loads +0x08 for GetCount and +0x10 for GetItem).
enum {
    RIP_Delete = 0,        // vector deleting destructor (flag 1 = free the block)
    RIP_Add = 1,
    RIP_Read = 2,
    RIP_ReadCollection = 3,
    RIP_ReadString = 4,
    RIP_ReadColor = 5,
    RIP_ReadBool = 6,
    RIP_ReadSize = 7,
    RIP_ReadInt = 8,
    RIP_ReadUInt = 9,
    RIP_ReadValue = 10,
    RIP_WriteSize = 14,
    RIP_WriteUInt = 16,
    RIC_GetCount = 1,      // XRibbonInfoParserCollection
    RIC_GetItem = 2
};
typedef void     (MS_ABI* RIP_DeleteFn)(void*, unsigned);
typedef int      (MS_ABI* RIP_SubFn)(void*, const CString*, void**);
typedef int      (MS_ABI* RIP_StrFn)(void*, const CString*, CString*);
typedef int      (MS_ABI* RIP_ColorFn)(void*, const CString*, unsigned long*);
typedef int      (MS_ABI* RIP_IntFn)(void*, const CString*, int*);
typedef int      (MS_ABI* RIP_SizeFn)(void*, const CString*, RI_Size*);
typedef int      (MS_ABI* RIP_UIntFn)(void*, const CString*, unsigned*);
typedef int      (MS_ABI* RIP_ValueFn)(void*, CString*);
typedef int      (MS_ABI* RIP_WSizeFn)(void*, const CString*, const RI_Size*, const RI_Size*);
typedef int      (MS_ABI* RIP_WUIntFn)(void*, const CString*, unsigned, unsigned);
typedef unsigned (MS_ABI* RIC_CountFn)(void*);
typedef void*    (MS_ABI* RIC_ItemFn)(void*, unsigned);

void* ri_slot(void* obj, int n) { return (*static_cast<void***>(obj))[n]; }

// Every retail call site builds the tag name as a CString temporary
// (??0CStringT@@QEAA@PEB_W@Z), makes the virtual call, and releases the
// temporary; the helpers below do the same with this DLL's CString.
int rip_sub(void* parser, int slot, const wchar_t* name, void** out) {
    CString n(name);
    return reinterpret_cast<RIP_SubFn>(ri_slot(parser, slot))(parser, &n, out);
}
int rip_read_string(void* parser, const wchar_t* name, CString& val) {
    CString n(name);
    return reinterpret_cast<RIP_StrFn>(ri_slot(parser, RIP_ReadString))(parser, &n, &val);
}
int rip_read_color(void* parser, const wchar_t* name, unsigned long& val) {
    CString n(name);
    return reinterpret_cast<RIP_ColorFn>(ri_slot(parser, RIP_ReadColor))(parser, &n, &val);
}
int rip_read_bool(void* parser, const wchar_t* name, int& val) {
    CString n(name);
    return reinterpret_cast<RIP_IntFn>(ri_slot(parser, RIP_ReadBool))(parser, &n, &val);
}
int rip_read_int(void* parser, const wchar_t* name, int& val) {
    CString n(name);
    return reinterpret_cast<RIP_IntFn>(ri_slot(parser, RIP_ReadInt))(parser, &n, &val);
}
int rip_read_uint(void* parser, const wchar_t* name, unsigned& val) {
    CString n(name);
    return reinterpret_cast<RIP_UIntFn>(ri_slot(parser, RIP_ReadUInt))(parser, &n, &val);
}
int rip_read_size(void* parser, const wchar_t* name, RI_Size& val) {
    CString n(name);
    return reinterpret_cast<RIP_SizeFn>(ri_slot(parser, RIP_ReadSize))(parser, &n, &val);
}
int rip_read_value(void* parser, CString& val) {
    return reinterpret_cast<RIP_ValueFn>(ri_slot(parser, RIP_ReadValue))(parser, &val);
}
int rip_write_uint(void* parser, const wchar_t* name, unsigned val, unsigned valDefault) {
    CString n(name);
    return reinterpret_cast<RIP_WUIntFn>(ri_slot(parser, RIP_WriteUInt))(parser, &n, val, valDefault);
}
int rip_write_size(void* parser, const wchar_t* name, const RI_Size& val, const RI_Size& valDefault) {
    CString n(name);
    return reinterpret_cast<RIP_WSizeFn>(ri_slot(parser, RIP_WriteSize))(parser, &n, &val, &valDefault);
}
// `delete pParser` / `delete pCollection`: retail calls vtable slot 0 with
// edx = 1, i.e. the vector deleting destructor asked to free the block.  The
// null test is retail's too (every site does `if (p) delete p`).
void rip_delete(void* obj) {
    if (obj != nullptr) {
        reinterpret_cast<RIP_DeleteFn>(ri_slot(obj, RIP_Delete))(obj, 1u);
    }
}
unsigned ric_count(void* coll) { return reinterpret_cast<RIC_CountFn>(ri_slot(coll, RIC_GetCount))(coll); }
void*    ric_item(void* coll, unsigned i) { return reinterpret_cast<RIC_ItemFn>(ri_slot(coll, RIC_GetItem))(coll, i); }

// Retail helper at RVA 0xfd604 (mfc140u), called by XID-bearing Read bodies:
//   XRibbonInfoParser* p = NULL; BOOL bRes = TRUE;
//   rParser.Read(CString(name), &p);          // its result is ignored
//   if (p) { bRes = id.Read(*p); delete p; }
//   return bRes;
// Note that a missing element therefore reads as success.
int ri_read_id(const wchar_t* name, RI_XID& id, void* parser) {
    void* sub = nullptr;
    int bRes = 1;
    rip_sub(parser, RIP_Read, name, &sub);
    if (sub != nullptr) {
        bRes = impl__Read_XID_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(&id, sub);
        rip_delete(sub);
    }
    return bRes;
}
// Retail helper at RVA 0xfd6fc (mfc140u): identical shape, XImage::Read instead.
int ri_read_image(const wchar_t* name, RI_XImage& img, void* parser) {
    void* sub = nullptr;
    int bRes = 1;
    rip_sub(parser, RIP_Read, name, &sub);
    if (sub != nullptr) {
        bRes = impl__Read_XImage_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(&img, sub);
        rip_delete(sub);
    }
    return bRes;
}

// The element arrays are CArray<T*,T*>: retail appends with the inlined
//   if (m_nSize < 0) AfxThrowInvalidArgException();
//   SetSize(m_nSize + 1);  m_pData[m_nSize - 1] = p;
// where SetSize (RVA 0x11f48, mfc140u) allocates through ??2@YAPEAX_K@Z and
// frees through the CRT's free().  ri_array_append_slot reproduces SetSize's
// branch structure (see its comment); on allocation failure the element is
// simply not appended, where retail's operator new would have thrown.
bool ri_array_add_ptr(RI_Array& a, void* p) {
    const long long idx = ri_array_append_slot(a, sizeof(void*));
    if (idx < 0) return false;
    static_cast<void**>(a.m_pData)[idx] = p;
    return true;
}

// pElement->Read(rParser) -- retail dispatches through vtable slot 1 of the
// element.  Deviation (1) leaves no vtable to dispatch through, so this file
// dispatches on XBase::m_ElementType instead.  Every object that reaches this
// switch was created by XBase/XElement::CreateFromType below, whose
// constructors store exactly these type codes, so the two dispatches agree for
// everything this DLL builds; a client-derived XElement subclass with an
// overridden Read is the one case they would not.
int ri_element_read(void* elem, void* parser) {
    switch (static_cast<RI_XBase*>(elem)->m_ElementType) {
    case 0:  return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 1:  return impl__Read_XElementButtonCheck_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 2:  return impl__Read_XElementButtonColor_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 3:  return impl__Read_XElementButtonUndo_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 4:  return impl__Read_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 5:  return impl__Read_XElementButtonLinkCtrl_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 6:  return impl__Read_XElementButtonApplication_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 7:  return impl__Read_XElementButtonMainPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 8:  return impl__Read_XElementButtonLaunch_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 9:  return impl__Read_XElementLabel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 10: return impl__Read_XElementEdit_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 11: return impl__Read_XElementComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 12: return impl__Read_XElementFontComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 13: return impl__Read_XElementSlider_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 14: return impl__Read_XElementProgressBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 15: return impl__Read_XElementSeparator_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 16: return impl__Read_XElementGroup_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 17: return impl__Read_XPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 18: return impl__Read_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 19: return impl__Read_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 20: return impl__Read_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 21: return impl__Read_XQAT_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    case 22: return impl__Read_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(elem, parser);
    default: return 0;
    }
}

// The "<OUTER><INNER>..</INNER>..</OUTER>" element-list idiom that retail
// inlines into XElementButton::Read (0x100f00), XElementGroup::Read (0x100b00),
// XPanel::Read (0x1044e0), XCategory::Read (0x104d70), XCategoryMain::Read
// (0x105cd0), XContext::Read (0x105720) and XRibbonBar::Read (0x106ba0), all
// mfc140u, with the same shape every time:
//   XRibbonInfoParser* pSub = NULL;  rParser.Read(CString(outer), &pSub);
//   if (pSub) {
//     XRibbonInfoParserCollection* pColl = NULL;  pSub->ReadCollection(CString(inner), &pColl);
//     if (pColl) {
//       for (UINT i = 0; i < pColl->GetCount(); i++) {       // GetCount re-read each pass
//         XRibbonInfoParser* pItem = pColl->GetItem(i);
//         if (pItem) { XBase* p = XBase::CreateFromTag(*pItem); if (p) arr.Add(p); }
//       }
//       delete pColl;
//     }
//     delete pSub;
//   }
void ri_read_element_array(void* parser, const wchar_t* outer, const wchar_t* inner, RI_Array& arr) {
    void* sub = nullptr;
    rip_sub(parser, RIP_Read, outer, &sub);
    if (sub == nullptr) return;
    void* coll = nullptr;
    rip_sub(sub, RIP_ReadCollection, inner, &coll);
    if (coll != nullptr) {
        for (unsigned i = 0; i < ric_count(coll); ++i) {
            void* item = ric_item(coll, i);
            if (item == nullptr) continue;
            void* elem = impl__CreateFromTag_XBase_CMFCRibbonInfo__SAPEAV12_AEAVXRibbonInfoParser_2__Z(item);
            if (elem != nullptr) {
                ri_array_add_ptr(arr, elem);
            }
        }
        rip_delete(coll);
    }
    rip_delete(sub);
}

// The 23-entry element-name table at 0x33dc90 (mfc140u), indexed by
// XElementType, shared by ElementNameFromType and ElementTypeFromName.
const wchar_t* const ri_element_names[23] = {
    L"Button", L"Button_Check", L"Button_Color", L"Button_Undo", L"Button_Gallery",
    L"Button_LinkCtrl", L"Button_Main", L"Button_Main_Panel", L"Button_Launch",
    L"Label", L"Edit", L"ComboBox", L"ComboBox_Font", L"Slider", L"Progress",
    L"Separator", L"Group", L"Panel", L"Category", L"Context", L"Category_Main",
    L"QAT", L"RibbonBar"
};

} // namespace

// Symbol: ??0CMFCRibbonInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonInfo__QEAA_XZ(void* pThis) {
    // Retail RVA 0x107b00 (mfc140u): the CMFCRibbonInfo vtable into +0x00
    // (deviation 1), m_sizeImage[0..1] zeroed, XRibbonBar::XRibbonBar() on
    // m_RibbonBar (+0x18), XInfoError::XInfoError() on m_Error (+0x4d0),
    // m_dwVersion = 1, then m_sizeImage[0] = (16,16), m_sizeImage[1] = (32,32)
    // and m_RibbonBar.m_Images.m_Image.m_sizeImage (+0xc8) = (16,16).
    RI_CMFCRibbonInfo* p = static_cast<RI_CMFCRibbonInfo*>(pThis);
    p->vfptr = nullptr;
    p->m_sizeImage[0].cx = 0; p->m_sizeImage[0].cy = 0;
    p->m_sizeImage[1].cx = 0; p->m_sizeImage[1].cy = 0;
    impl___0XRibbonBar_CMFCRibbonInfo__QEAA_XZ(&p->m_RibbonBar);
    impl___0XInfoError_CMFCRibbonInfo__QEAA_XZ(&p->m_Error);
    p->m_dwVersion = 1;
    p->m_sizeImage[0].cx = 16; p->m_sizeImage[0].cy = 16;
    p->m_sizeImage[1].cx = 32; p->m_sizeImage[1].cy = 32;
    ri_image_size(p->m_RibbonBar.m_Images).cx = 16;
    ri_image_size(p->m_RibbonBar.m_Images).cy = 16;
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
extern "C" void MS_ABI impl___1CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0x107bc0 (mfc140u): stores the CMFCRibbonInfo vtable
    // (deviation 1), runs ~XInfoError on m_Error (+0x4d0) and tail-calls
    // ~XRibbonBar on m_RibbonBar (+0x18).
    RI_CMFCRibbonInfo* p = static_cast<RI_CMFCRibbonInfo*>(pThis);
    impl___1XInfoError_CMFCRibbonInfo__QEAA_XZ(&p->m_Error);
    impl___1XRibbonBar_CMFCRibbonInfo__UEAA_XZ(&p->m_RibbonBar);
}

// Symbol: ?AddElementImages@CMFCRibbonInfo@@IEAAXAEAVXElement@1@AEAV?$CArray@PEAVXImage@CMFCRibbonInfo@@PEAV12@@@@Z
extern "C" void MS_ABI impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
    void* pThis, void* pElement, void* pImages) {
    // Retail RVA 0x107bf0 (mfc140u), switching on info.m_ElementType (+0x08):
    //   e_TypeButton_Gallery (4): images.Add(&gallery.m_Images (+0xa0)), then
    //       fall into the sub-item walk below;
    //   e_TypeGroup (16):         images.Add(&group.m_Images (+0x50)) and
    //       AddElementImages(*e, images) for every e in m_arButtons (+0x208);
    //       nothing else;
    //   e_TypeButton (0) and e_TypeButton_Color (2): the sub-item walk --
    //       AddElementImages(*e, images) for every e in m_arSubItems (+0x60);
    //   anything else: nothing.
    // `this` is only passed through to the recursive calls.
    RI_XBase* base = static_cast<RI_XBase*>(pElement);
    RI_Array* images = static_cast<RI_Array*>(pImages);
    const int type = base->m_ElementType;
    if (type == 16) {
        RI_XElementGroup* g = static_cast<RI_XElementGroup*>(pElement);
        ri_array_add_ptr(*images, &g->m_Images);
        void** buttons = static_cast<void**>(g->m_arButtons.m_pData);
        for (long long i = 0; i < g->m_arButtons.m_nSize; ++i) {
            impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
                pThis, buttons[i], pImages);
        }
        return;
    }
    if (type == 4) {
        RI_XElementButtonGallery* gal = static_cast<RI_XElementButtonGallery*>(pElement);
        ri_array_add_ptr(*images, &gal->m_Images);
    } else if (type != 0 && type != 2) {
        return;
    }
    RI_XElementButton* btn = static_cast<RI_XElementButton*>(pElement);
    void** subs = static_cast<void**>(btn->m_arSubItems.m_pData);
    for (long long i = 0; i < btn->m_arSubItems.m_nSize; ++i) {
        impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
            pThis, subs[i], pImages);
    }
}

// Symbol: ?ElementNameFromType@CMFCRibbonInfo@@SAPEB_WW4XElementType@1@@Z
extern "C" const wchar_t* MS_ABI impl__ElementNameFromType_CMFCRibbonInfo__SAPEB_WW4XElementType_1__Z(int type) {
    // Retail RVA 0xfd800 (mfc140u): (unsigned)type <= 22 ? table[type] : NULL.
    if (static_cast<unsigned>(type) > 22u) return nullptr;
    return ri_element_names[type];
}

// Symbol: ?ElementTypeFromName@CMFCRibbonInfo@@SA?AW4XElementType@1@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" unsigned int MS_ABI impl__ElementTypeFromName_CMFCRibbonInfo__SA_AW4XElementType_1_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CString* pName) {
    // Retail RVA 0xfd820 (mfc140u): result = e_TypeUnknown (-1); if the name
    // is not empty, walk the 23-entry table and return the first index whose
    // entry _wcsicmp()s equal to the name (IAT slot 0x2c7788 = _wcsicmp).  A
    // NULL table entry makes retail throw E_FAIL (the call at 0xfd882 goes to
    // MFC's AtlThrow shim at 0x333c, i.e. AfxThrowOleException(E_FAIL)); the
    // table here has none.
    if (pName->GetLength() == 0) return static_cast<unsigned int>(-1);
    for (int i = 0; i < 23; ++i) {
        if (_wcsicmp(pName->GetString(), ri_element_names[i]) == 0) {
            return static_cast<unsigned int>(i);
        }
    }
    return static_cast<unsigned int>(-1);
}

// Symbol: ?GetArrayImages@CMFCRibbonInfo@@QEAAXAEAV?$CArray@PEAVXImage@CMFCRibbonInfo@@PEAV12@@@@Z
extern "C" void MS_ABI impl__GetArrayImages_CMFCRibbonInfo__QEAAXAEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
    void* pThis, void* pArray) {
    // Retail RVA 0x107cf0 (mfc140u):
    //   images.RemoveAll();               inlined: free(m_pData) if non-null,
    //                                     m_pData = NULL, m_nMaxSize = m_nSize = 0
    //   images.SetSize(1); images[0] = &m_RibbonBar.m_Images;         (+0x40)
    //   if (m_RibbonBar.m_btnMain)        images.Add(&m_btnMain->m_Image);  (+0x88)
    //   if (m_RibbonBar.m_MainCategory) { images.Add(&main->m_SmallImages (+0x18));
    //                                     images.Add(&main->m_LargeImages (+0x1d0)); }
    //   for e in m_RibbonBar.m_TabElements.m_arButtons: AddElementImages(*e, images);
    //   CArray<XCategory*,XCategory*> cats;              (local, vtable 0x305ad0)
    //   cats.Append(m_RibbonBar.m_arCategories);         (helper 0x108070 = Append)
    //   for ctx in m_RibbonBar.m_arContexts: cats.Append(ctx->m_arCategories (+0x30));
    //   for cat in cats: images.Add(&cat->m_SmallImages (+0x20));
    //                    images.Add(&cat->m_LargeImages (+0x1d8));
    //                    for panel in cat->m_arPanels: for e in panel->m_arElements:
    //                        AddElementImages(*e, images);
    //   ~cats (helper 0x1080ec: free(m_pData)).
    // The local category array is replaced by walking the two sources in the
    // same order, which visits the same categories in the same sequence.
    // The RemoveAll() free is retail's (IAT slot 0x2c74e8 = free): images is
    // the caller's array, so its buffer came from the caller's CRT -- the same
    // cross-allocator caveat as ~XQAT above, and the same one the caller's
    // inline ~CArray then applies in reverse to the buffer built here.
    RI_CMFCRibbonInfo* p = static_cast<RI_CMFCRibbonInfo*>(pThis);
    RI_Array* images = static_cast<RI_Array*>(pArray);
    if (images->m_pData != nullptr) {
        std::free(images->m_pData);
        images->m_pData = nullptr;
    }
    images->m_nMaxSize = 0;
    images->m_nSize    = 0;
    ri_array_add_ptr(*images, &p->m_RibbonBar.m_Images);
    if (p->m_RibbonBar.m_btnMain != nullptr) {
        ri_array_add_ptr(*images, &static_cast<RI_XElementButtonApplication*>(p->m_RibbonBar.m_btnMain)->m_Image);
    }
    if (p->m_RibbonBar.m_MainCategory != nullptr) {
        RI_XCategoryMain* main = static_cast<RI_XCategoryMain*>(p->m_RibbonBar.m_MainCategory);
        ri_array_add_ptr(*images, &main->m_SmallImages);
        ri_array_add_ptr(*images, &main->m_LargeImages);
    }
    {
        void** tabs = static_cast<void**>(p->m_RibbonBar.m_TabElements.m_arButtons.m_pData);
        for (long long i = 0; i < p->m_RibbonBar.m_TabElements.m_arButtons.m_nSize; ++i) {
            impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
                pThis, tabs[i], pArray);
        }
    }
    // cats = m_arCategories ++ every context's m_arCategories, walked in that
    // order without materialising the local array.
    auto forEachCategory = [&](const RI_Array& arr) {
        void** cats = static_cast<void**>(arr.m_pData);
        for (long long c = 0; c < arr.m_nSize; ++c) {
            RI_XCategory* cat = static_cast<RI_XCategory*>(cats[c]);
            ri_array_add_ptr(*images, &cat->m_SmallImages);
            ri_array_add_ptr(*images, &cat->m_LargeImages);
            void** panels = static_cast<void**>(cat->m_arPanels.m_pData);
            for (long long k = 0; k < cat->m_arPanels.m_nSize; ++k) {
                RI_XPanel* panel = static_cast<RI_XPanel*>(panels[k]);
                void** elems = static_cast<void**>(panel->m_arElements.m_pData);
                for (long long e = 0; e < panel->m_arElements.m_nSize; ++e) {
                    impl__AddElementImages_CMFCRibbonInfo__IEAAXAEAVXElement_1_AEAV__CArray_PEAVXImage_CMFCRibbonInfo__PEAV12____Z(
                        pThis, elems[e], pArray);
                }
            }
        }
    };
    forEachCategory(p->m_RibbonBar.m_arCategories);
    {
        void** ctxs = static_cast<void**>(p->m_RibbonBar.m_arContexts.m_pData);
        for (long long i = 0; i < p->m_RibbonBar.m_arContexts.m_nSize; ++i) {
            forEachCategory(static_cast<RI_XContext*>(ctxs[i])->m_arCategories);
        }
    }
}

// Symbol: ?Read@CMFCRibbonInfo@@MEAAHAEAVXRibbonInfoParser@1@@Z
extern "C" int MS_ABI impl__Read_CMFCRibbonInfo__MEAAHAEAVXRibbonInfoParser_1__Z(void* pThis, void* pParser) {
    // Retail RVA 0x10b280 (mfc140u):
    //   m_Error.Empty();
    //   XRibbonInfoParser* pHeader = NULL; rParser.Read(CString(L"HEADER"), &pHeader);
    //   if (!pHeader) { m_Error.SetError(e_ErrorInvalidHeader, CString(), -1, -1); return FALSE; }
    //   UINT ver = m_dwVersion; BOOL bRes = pHeader->ReadUInt(CString(L"VERSION"), ver); m_dwVersion = ver;
    //   if (bRes) {
    //     XRibbonInfoParser* pSizes = NULL; pHeader->Read(CString(L"SIZES"), &pSizes);
    //     if (pSizes) { pSizes->ReadSize(L"IMAGE_SMALL", m_sizeImage[0]);
    //                   pSizes->ReadSize(L"IMAGE_LARGE", m_sizeImage[1]); delete pSizes; }
    //   }
    //   delete pHeader;
    //   if (!bRes) { m_Error.SetError(e_ErrorInvalidHeader, CString(), -1, -1); return FALSE; }
    //   XRibbonInfoParser* pBar = NULL; rParser.Read(CString(L"RIBBON_BAR"), &pBar);
    //   if (!pBar) { m_Error.SetError(e_ErrorInvalidRibbon, CString(), -1, -1); return FALSE; }
    //   bRes = m_RibbonBar.Read(*pBar);    (vtable slot 1 of m_RibbonBar)
    //   delete pBar;
    //   if (!bRes) { m_Error.SetError(e_ErrorInvalidRibbon, CString(), -1, -1); return FALSE; }
    //   -- then the image sizes are pushed down the tree --
    //   m_RibbonBar.m_Images.m_Image.m_sizeImage = m_sizeImage[0];                   (+0xc8)
    //   if (m_RibbonBar.m_MainCategory) { main->m_SmallImages.m_Image.m_sizeImage = small;
    //                                     main->m_LargeImages.m_Image.m_sizeImage = large; }
    //   CArray<XCategory*> cats; cats.Append(m_RibbonBar.m_arCategories);
    //   for ctx in m_RibbonBar.m_arContexts: cats.Append(ctx->m_arCategories);
    //   for cat in cats: cat->m_SmallImages.m_Image.m_sizeImage = small;
    //                    cat->m_LargeImages.m_Image.m_sizeImage = large;
    //                    for panel in cat->m_arPanels: for e in panel->m_arElements:
    //                        if (e->m_ElementType == e_TypeGroup)
    //                            ((XElementGroup*)e)->m_Images.m_Image.m_sizeImage = small;
    //   return TRUE;
    // m_RibbonBar.Read is called directly (deviation 1: no vtable), and the
    // local category array is replaced by walking its two sources in order.
    RI_CMFCRibbonInfo* p = static_cast<RI_CMFCRibbonInfo*>(pThis);
    impl__Empty_XInfoError_CMFCRibbonInfo__QEAAXXZ(&p->m_Error);
    void* header = nullptr;
    rip_sub(pParser, RIP_Read, L"HEADER", &header);
    if (header == nullptr) {
        CString empty;
        impl__SetError_XInfoError_CMFCRibbonInfo__QEAAXW4XError_12_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__JJ_Z(
            &p->m_Error, 3, &empty, -1, -1);
        return 0;
    }
    unsigned ver = static_cast<unsigned>(p->m_dwVersion);
    int bRes = rip_read_uint(header, L"VERSION", ver);
    p->m_dwVersion = ver;
    if (bRes) {
        void* sizes = nullptr;
        rip_sub(header, RIP_Read, L"SIZES", &sizes);
        if (sizes != nullptr) {
            rip_read_size(sizes, L"IMAGE_SMALL", p->m_sizeImage[0]);
            rip_read_size(sizes, L"IMAGE_LARGE", p->m_sizeImage[1]);
            rip_delete(sizes);
        }
    }
    rip_delete(header);
    if (!bRes) {
        CString empty;
        impl__SetError_XInfoError_CMFCRibbonInfo__QEAAXW4XError_12_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__JJ_Z(
            &p->m_Error, 3, &empty, -1, -1);
        return 0;
    }
    void* bar = nullptr;
    rip_sub(pParser, RIP_Read, L"RIBBON_BAR", &bar);
    if (bar != nullptr) {
        bRes = impl__Read_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(&p->m_RibbonBar, bar);
        rip_delete(bar);
    }
    if (bar == nullptr || !bRes) {
        CString empty;
        impl__SetError_XInfoError_CMFCRibbonInfo__QEAAXW4XError_12_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__JJ_Z(
            &p->m_Error, 4, &empty, -1, -1);
        return 0;
    }
    const RI_Size small = p->m_sizeImage[0];
    const RI_Size large = p->m_sizeImage[1];
    ri_image_size(p->m_RibbonBar.m_Images) = small;
    if (p->m_RibbonBar.m_MainCategory != nullptr) {
        RI_XCategoryMain* main = static_cast<RI_XCategoryMain*>(p->m_RibbonBar.m_MainCategory);
        ri_image_size(main->m_SmallImages) = small;
        ri_image_size(main->m_LargeImages) = large;
    }
    auto forEachCategory = [&](const RI_Array& arr) {
        void** cats = static_cast<void**>(arr.m_pData);
        for (long long c = 0; c < arr.m_nSize; ++c) {
            RI_XCategory* cat = static_cast<RI_XCategory*>(cats[c]);
            ri_image_size(cat->m_SmallImages) = small;
            ri_image_size(cat->m_LargeImages) = large;
            void** panels = static_cast<void**>(cat->m_arPanels.m_pData);
            for (long long k = 0; k < cat->m_arPanels.m_nSize; ++k) {
                RI_XPanel* panel = static_cast<RI_XPanel*>(panels[k]);
                void** elems = static_cast<void**>(panel->m_arElements.m_pData);
                for (long long e = 0; e < panel->m_arElements.m_nSize; ++e) {
                    if (static_cast<RI_XBase*>(elems[e])->m_ElementType == 16) {
                        ri_image_size(static_cast<RI_XElementGroup*>(elems[e])->m_Images) = small;
                    }
                }
            }
        }
    };
    forEachCategory(p->m_RibbonBar.m_arCategories);
    {
        void** ctxs = static_cast<void**>(p->m_RibbonBar.m_arContexts.m_pData);
        for (long long i = 0; i < p->m_RibbonBar.m_arContexts.m_nSize; ++i) {
            forEachCategory(static_cast<RI_XContext*>(ctxs[i])->m_arCategories);
        }
    }
    return 1;
}

// Symbol: ?Read@CMFCRibbonInfo@@UEAAHPEAEI@Z
extern "C" int MS_ABI impl__Read_CMFCRibbonInfo__UEAAHPEAEI_Z(
    void* pThis, void* pData, unsigned int cb) {
    // STUB.  Retail RVA 0x10bce0 (mfc140u): m_Error.Empty(); if (!lpBuffer ||
    // !nSize) return FALSE; then it builds a DLL-internal CXMLParserRoot on the
    // stack (constructor at 0x10a2c4, an MSXML IXMLDOMDocument wrapper that is
    // not exported), calls its Load(lpBuffer, nSize) (0x10aa90), on failure
    // SetError(e_ErrorFile, description, line, linepos) from the parser's
    // error fields, otherwise GetRoot(CString(L"AFX_RIBBON"), &pParser)
    // (0x10a370), SetError(e_ErrorInvalidRoot) when that yields nothing, and
    // finally Read(*pParser) through vtable slot 1 followed by delete pParser.
    // OpenMFC has no CXMLParser* implementation (featurepack/ribbon/
    // CXMLParserCollection.cpp is a bare stub), so the XML front end cannot be
    // reproduced.
    (void)pThis;
    (void)pData;
    (void)cb;
    return 0;
}

// Symbol: ?Read@CMFCRibbonInfo@@UEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__Read_CMFCRibbonInfo__UEAAHPEAUIStream___Z(void* pThis, void* pStream) {
    // STUB.  Retail RVA 0x10bb40 (mfc140u): the same shape as the LPBYTE
    // overload above (m_Error.Empty(); NULL stream -> FALSE; CXMLParserRoot
    // loaded from the IStream via 0x1084b0; GetRoot("AFX_RIBBON"); Read(*p)),
    // and blocked by the same missing MSXML parser.
    (void)pThis;
    (void)pStream;
    return 0;
}

// Symbol: ?Write@CMFCRibbonInfo@@MEAAHAEAVXRibbonInfoParser@1@@Z
extern "C" int MS_ABI impl__Write_CMFCRibbonInfo__MEAAHAEAVXRibbonInfoParser_1__Z(void* pThis, void* pParser) {
    // Retail RVA 0x10b7f0 (mfc140u):
    //   m_Error.Empty();
    //   XRibbonInfoParser* pHeader = NULL; rParser.Add(CString(L"HEADER"), &pHeader);
    //   if (!pHeader) return FALSE;
    //   BOOL bRes = pHeader->WriteUInt(CString(L"VERSION"), m_dwVersion, 0);
    //   if (bRes && !(m_sizeImage[0] == (16,16) && m_sizeImage[1] == (32,32))) {
    //     XRibbonInfoParser* pSizes = NULL; pHeader->Add(CString(L"SIZES"), &pSizes);
    //     if (pSizes) { pSizes->WriteSize(L"IMAGE_SMALL", m_sizeImage[0], CSize(16,16));
    //                   pSizes->WriteSize(L"IMAGE_LARGE", m_sizeImage[1], CSize(32,32));
    //                   delete pSizes; }
    //   }
    //   delete pHeader;
    //   if (!bRes) return FALSE;
    //   XRibbonInfoParser* pBar = NULL; rParser.Add(CString(L"RIBBON_BAR"), &pBar);
    //   if (pBar) { bRes = m_RibbonBar.Write(*pBar); delete pBar; }   (vtable slot 2)
    //   return bRes;
    // m_RibbonBar.Write is called directly (deviation 1).  That thunk lives in
    // featurepack/ribbon/Thunks.cpp and is NOT a transcription: its body is
    // RecordRibbonWrite(parser, L"XRibbonBar"), which logs the call to a side
    // table and returns TRUE without emitting anything through the parser.
    // So this function is faithful only down to the HEADER/SIZES block; the
    // RIBBON_BAR element it adds stays empty and the TRUE it returns does not
    // mean a complete document was written.
    RI_CMFCRibbonInfo* p = static_cast<RI_CMFCRibbonInfo*>(pThis);
    impl__Empty_XInfoError_CMFCRibbonInfo__QEAAXXZ(&p->m_Error);
    void* header = nullptr;
    rip_sub(pParser, RIP_Add, L"HEADER", &header);
    if (header == nullptr) return 0;
    int bRes = rip_write_uint(header, L"VERSION", static_cast<unsigned>(p->m_dwVersion), 0u);
    if (bRes) {
        const bool defaults = p->m_sizeImage[0].cx == 16 && p->m_sizeImage[0].cy == 16 &&
                              p->m_sizeImage[1].cx == 32 && p->m_sizeImage[1].cy == 32;
        if (!defaults) {
            void* sizes = nullptr;
            rip_sub(header, RIP_Add, L"SIZES", &sizes);
            if (sizes != nullptr) {
                const RI_Size defSmall = { 16, 16 };
                const RI_Size defLarge = { 32, 32 };
                rip_write_size(sizes, L"IMAGE_SMALL", p->m_sizeImage[0], defSmall);
                rip_write_size(sizes, L"IMAGE_LARGE", p->m_sizeImage[1], defLarge);
                rip_delete(sizes);
            }
        }
    }
    rip_delete(header);
    if (!bRes) return 0;
    void* bar = nullptr;
    rip_sub(pParser, RIP_Add, L"RIBBON_BAR", &bar);
    if (bar != nullptr) {
        bRes = impl__Write_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(&p->m_RibbonBar, bar);
        rip_delete(bar);
    }
    return bRes;
}

// Symbol: ?Write@CMFCRibbonInfo@@UEAAHPEAPEAEAEAI@Z
extern "C" int MS_ABI impl__Write_CMFCRibbonInfo__UEAAHPEAPEAEAEAI_Z(
    void* pThis, unsigned char** ppData, unsigned int* pcData) {
    // STUB.  Retail RVA 0x10bfb0 (mfc140u): m_Error.Empty(); if (!lpBuffer)
    // return FALSE; *lpBuffer = NULL; size = 0; then a stack CXMLParserRoot
    // (0x10a2c4), GetRoot(CString(L"AFX_RIBBON"), &pParser) (0x10a370),
    // Write(*pParser) through vtable slot 4, delete pParser, and -- only when
    // that Write returned TRUE -- the root's Save(lpBuffer, size) (0x10b050)
    // to serialise the MSXML document.  Blocked by the missing CXMLParser*
    // implementation.
    (void)pThis;
    (void)ppData;
    (void)pcData;
    return 0;
}

// Symbol: ?Write@CMFCRibbonInfo@@UEAAHPEAUIStream@@@Z
extern "C" int MS_ABI impl__Write_CMFCRibbonInfo__UEAAHPEAUIStream___Z(void* pThis, void* pStream) {
    // STUB.  Retail RVA 0x10be90 (mfc140u): the IStream twin of the LPBYTE*
    // overload above (NULL stream -> FALSE; CXMLParserRoot; GetRoot; Write;
    // save to the stream), blocked by the same missing MSXML parser.
    (void)pThis;
    (void)pStream;
    return 0;
}

// Symbol: ??1XRibbonInfoParserCollection@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonInfoParserCollection_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0x108b70 (mfc140u): the entire body is the store of the
    // XRibbonInfoParserCollection vtable at +0x00.  The class is abstract with
    // no data members and OpenMFC does not model the vtable (deviation 1), so
    // there is nothing left to do.
    (void)pThis;
}

// Symbol: ??1XRibbonInfoParserRoot@CMFCRibbonInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1XRibbonInfoParserRoot_CMFCRibbonInfo__UEAA_XZ(void* pThis) {
    // Retail RVA 0x108b20 (mfc140u): the entire body is the store of the
    // XRibbonInfoParserRoot vtable at +0x00; abstract, no data members,
    // vtable not modelled (deviation 1) -- nothing left to do.
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
    // and the destructor leaves it alone; anything a client put in it leaks,
    // and so do the CStrings XElementComboBox::Read below appends to the raw
    // CArray image.
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
extern "C" void* MS_ABI impl__CreateFromTag_XBase_CMFCRibbonInfo__SAPEAV12_AEAVXRibbonInfoParser_2__Z(void* pParser) {
    // Retail RVA 0xffd90 (mfc140u):
    //   XBase* pResult = NULL;  CString strName;
    //   rParser.ReadString(CString(L"ELEMENT_NAME"), strName);
    //   if (!strName.IsEmpty()) {
    //     pResult = CreateFromType(ElementTypeFromName(strName));
    //     if (pResult) pResult->Read(rParser);        (vtable slot 1)
    //   }
    //   return pResult;
    // The virtual Read goes through ri_element_read (deviation 1).
    void* result = nullptr;
    CString strName;
    rip_read_string(pParser, L"ELEMENT_NAME", strName);
    if (strName.GetLength() != 0) {
        const int type = static_cast<int>(
            impl__ElementTypeFromName_CMFCRibbonInfo__SA_AW4XElementType_1_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(&strName));
        result = impl__CreateFromType_XBase_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(type);
        if (result != nullptr) {
            ri_element_read(result, pParser);
        }
    }
    return result;
}

// Symbol: ?CreateFromType@XBase@CMFCRibbonInfo@@SAPEAV12@W4XElementType@2@@Z
extern "C" void* MS_ABI impl__CreateFromType_XBase_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(int type) {
    // Retail RVA 0xffc40 (mfc140u): for the container types it does
    //   p = operator new(sizeof(T)); if (p) T::T(p); return p;
    // with e_TypePanel (17) -> XPanel 0xe0, e_TypeCategory (18) -> XCategory
    // 0x3e0, e_TypeContext (19) -> XContext 0x58, e_TypeCategoryMain (20) ->
    // XCategoryMain 0x3c8, e_TypeQAT (21) -> XQAT 0x40 (constructor inlined),
    // e_TypeRibbonBar (22) -> XRibbonBar 0x4b0; every other value tail-calls
    // XElement::CreateFromType(type).  operator new is the exported
    // ??2@YAPEAX_K@Z, as in retail.
    std::size_t size = 0;
    RI_CtorFn ctor = nullptr;
    switch (type) {
    case 17: size = sizeof(RI_XPanel);        ctor = impl___0XPanel_CMFCRibbonInfo__QEAA_XZ;        break;
    case 18: size = sizeof(RI_XCategory);     ctor = impl___0XCategory_CMFCRibbonInfo__QEAA_XZ;     break;
    case 19: size = sizeof(RI_XContext);      ctor = impl___0XContext_CMFCRibbonInfo__QEAA_XZ;      break;
    case 20: size = sizeof(RI_XCategoryMain); ctor = impl___0XCategoryMain_CMFCRibbonInfo__QEAA_XZ; break;
    case 21: size = sizeof(RI_XQAT);          ctor = impl___0XQAT_CMFCRibbonInfo__QEAA_XZ;          break;
    case 22: size = sizeof(RI_XRibbonBar);    ctor = impl___0XRibbonBar_CMFCRibbonInfo__QEAA_XZ;    break;
    default:
        return impl__CreateFromType_XElement_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(type);
    }
    void* p = impl___2_YAPEAX_K_Z(size);
    if (p != nullptr) ctor(p);
    return p;
}

// Symbol: ?CreateFromType@XElement@CMFCRibbonInfo@@SAPEAV12@W4XElementType@2@@Z
extern "C" void* MS_ABI impl__CreateFromType_XElement_CMFCRibbonInfo__SAPEAV12_W4XElementType_2__Z(int type) {
    // Retail RVA 0xfffb0 (mfc140u): p = operator new(sizeof(T)); if (p)
    // T::T(p); return p; for
    //   0 Button 0x88, 1 Button_Check 0x88, 2 Button_Color 0x2b8,
    //   3 Button_Undo 0x280, 4 Button_Gallery 0x280, 5 Button_LinkCtrl 0x90,
    //   6 Button_Application 0x240, 7 Button_MainPanel 0x88, 8 Button_Launch
    //   0x88, 9 Label 0x88, 10 Edit 0xa8, 11 ComboBox 0xe8, 12 ComboBox_Font
    //   0xf0, 13 Slider 0x68, 14 Progress 0x68, 15 Separator 0x58, 16 Group
    //   0x230; anything else returns NULL.
    // DEVIATION for e_TypeProgress (14): XElementProgressBar's constructor is
    // not defined in this file -- featurepack/ribbon/Thunks.cpp owns
    // ??0XElementProgressBar@CMFCRibbonInfo@@QEAA@XZ and its body is a no-op
    // that initialises nothing -- so a 0x68-byte block run through it would be
    // an uninitialised element with no type code.  NULL is returned for 14
    // instead until that constructor is real.
    std::size_t size = 0;
    RI_CtorFn ctor = nullptr;
    switch (type) {
    case 0:  size = sizeof(RI_XElementButton);            ctor = impl___0XElementButton_CMFCRibbonInfo__QEAA_XZ;            break;
    case 1:  size = sizeof(RI_XElementButton);            ctor = impl___0XElementButtonCheck_CMFCRibbonInfo__QEAA_XZ;       break;
    case 2:  size = sizeof(RI_XElementButtonColor);       ctor = impl___0XElementButtonColor_CMFCRibbonInfo__QEAA_XZ;       break;
    case 3:  size = sizeof(RI_XElementButtonGallery);     ctor = impl___0XElementButtonUndo_CMFCRibbonInfo__QEAA_XZ;        break;
    case 4:  size = sizeof(RI_XElementButtonGallery);     ctor = impl___0XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ;     break;
    case 5:  size = sizeof(RI_XElementButtonLinkCtrl);    ctor = impl___0XElementButtonLinkCtrl_CMFCRibbonInfo__QEAA_XZ;    break;
    case 6:  size = sizeof(RI_XElementButtonApplication); ctor = impl___0XElementButtonApplication_CMFCRibbonInfo__QEAA_XZ; break;
    case 7:  size = sizeof(RI_XElementButton);            ctor = impl___0XElementButtonMainPanel_CMFCRibbonInfo__QEAA_XZ;   break;
    case 8:  size = sizeof(RI_XElementButton);            ctor = impl___0XElementButtonLaunch_CMFCRibbonInfo__QEAA_XZ;      break;
    case 9:  size = sizeof(RI_XElementButton);            ctor = impl___0XElementLabel_CMFCRibbonInfo__QEAA_XZ;             break;
    case 10: size = sizeof(RI_XElementEdit);              ctor = impl___0XElementEdit_CMFCRibbonInfo__QEAA_XZ;              break;
    case 11: size = sizeof(RI_XElementComboBox);          ctor = impl___0XElementComboBox_CMFCRibbonInfo__QEAA_XZ;          break;
    case 12: size = sizeof(RI_XElementFontComboBox);      ctor = impl___0XElementFontComboBox_CMFCRibbonInfo__QEAA_XZ;      break;
    case 13: size = sizeof(RI_XElementSlider);            ctor = impl___0XElementSlider_CMFCRibbonInfo__QEAA_XZ;            break;
    case 15: size = sizeof(RI_XElementSeparator);         ctor = impl___0XElementSeparator_CMFCRibbonInfo__QEAA_XZ;         break;
    case 16: size = sizeof(RI_XElementGroup);             ctor = impl___0XElementGroup_CMFCRibbonInfo__QEAA_XZ;             break;
    default:
        return nullptr;
    }
    void* p = impl___2_YAPEAX_K_Z(size);
    if (p != nullptr) ctor(p);
    return p;
}

// Symbol: ?Empty@XInfoError@CMFCRibbonInfo@@QEAAXXZ
extern "C" void MS_ABI impl__Empty_XInfoError_CMFCRibbonInfo__QEAAXXZ(void* pThis) {
    // Retail RVA 0xfd8f0 (mfc140u): m_Error = e_ErrorNone (0), m_nLine =
    // m_nLinePos = -1, then a tail call to CSimpleStringT::Empty on
    // m_strDescription (+0x08).
    RI_XInfoError* p = static_cast<RI_XInfoError*>(pThis);
    p->m_Error    = 0;
    p->m_nLine    = -1;
    p->m_nLinePos = -1;
    p->m_strDescription.Empty();
}

// Symbol: ?Read@XCategory@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XCategory_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x104d70 (mfc140u): the PANELS/PANEL element list into
    // m_arPanels (+0x390) and the ELEMENTS/ELEMENT list into m_arElements
    // (+0x3b8) -- see ri_read_element_array -- then ReadString NAME (+0x10)
    // and KEYS (+0x18), the IMAGE_SMALL (+0x20) and IMAGE_LARGE (+0x1d8)
    // sub-elements through the XImage helper, and return TRUE.
    RI_XCategory* p = static_cast<RI_XCategory*>(pThis);
    ri_read_element_array(pParser, L"PANELS", L"PANEL", p->m_arPanels);
    ri_read_element_array(pParser, L"ELEMENTS", L"ELEMENT", p->m_arElements);
    rip_read_string(pParser, L"NAME", p->m_strName);
    rip_read_string(pParser, L"KEYS", p->m_strKeys);
    ri_read_image(L"IMAGE_SMALL", p->m_SmallImages, pParser);
    ri_read_image(L"IMAGE_LARGE", p->m_LargeImages, pParser);
    return 1;
}

// Symbol: ?Read@XCategoryMain@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x105cd0 (mfc140u): the ELEMENTS/ELEMENT list into
    // m_arElements (+0x388); then
    //   XRibbonInfoParser* pRecent = NULL; rParser.Read(CString(L"RECENT_FILE_LIST"), &pRecent);
    //   if (pRecent) { pRecent->ReadBool(L"ENABLE", m_bRecentListEnable (+0x3b0));
    //                  pRecent->ReadString(L"LABEL", m_strRecentListLabel (+0x3b8));
    //                  pRecent->ReadInt(L"WIDTH", m_nRecentListWidth (+0x3c0)); delete pRecent; }
    // ReadString NAME (+0x10), the IMAGE_SMALL (+0x18) and IMAGE_LARGE (+0x1d0)
    // sub-elements, return TRUE.
    RI_XCategoryMain* p = static_cast<RI_XCategoryMain*>(pThis);
    ri_read_element_array(pParser, L"ELEMENTS", L"ELEMENT", p->m_arElements);
    void* recent = nullptr;
    rip_sub(pParser, RIP_Read, L"RECENT_FILE_LIST", &recent);
    if (recent != nullptr) {
        rip_read_bool(recent, L"ENABLE", p->m_bRecentListEnable);
        rip_read_string(recent, L"LABEL", p->m_strRecentListLabel);
        rip_read_int(recent, L"WIDTH", p->m_nRecentListWidth);
        rip_delete(recent);
    }
    rip_read_string(pParser, L"NAME", p->m_strName);
    ri_read_image(L"IMAGE_SMALL", p->m_SmallImages, pParser);
    ri_read_image(L"IMAGE_LARGE", p->m_LargeImages, pParser);
    return 1;
}

// Symbol: ?Read@XContext@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XContext_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x105720 (mfc140u): the CATEGORIES/CATEGORY list into
    // m_arCategories (+0x30); the ID sub-element into m_ID (+0x10) through the
    // XID helper; ReadString TEXT (+0x20); then int color = 0;
    // rParser.ReadInt(L"COLOR", color); m_Color = color; return TRUE.
    RI_XContext* p = static_cast<RI_XContext*>(pThis);
    ri_read_element_array(pParser, L"CATEGORIES", L"CATEGORY", p->m_arCategories);
    ri_read_id(L"ID", p->m_ID, pParser);
    rip_read_string(pParser, L"TEXT", p->m_strText);
    int color = 0;
    rip_read_int(pParser, L"COLOR", color);
    p->m_Color = color;
    return 1;
}

// Symbol: ?Read@XElement@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElement_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1002c0 (mfc140u): the ID sub-element into m_ID (+0x10)
    // through the XID helper; ReadString TEXT (+0x20), TOOLTIP (+0x28),
    // DESCRIPTION (+0x30), KEYS (+0x38), KEYS_MENU (+0x40); ReadBool
    // PALETTE_TOP (+0x48) and ALWAYS_LARGE (+0x4c); return TRUE.  Every result
    // is ignored.
    RI_XElement* p = static_cast<RI_XElement*>(pThis);
    ri_read_id(L"ID", p->m_ID, pParser);
    rip_read_string(pParser, L"TEXT", p->m_strText);
    rip_read_string(pParser, L"TOOLTIP", p->m_strToolTip);
    rip_read_string(pParser, L"DESCRIPTION", p->m_strDescription);
    rip_read_string(pParser, L"KEYS", p->m_strKeys);
    rip_read_string(pParser, L"KEYS_MENU", p->m_strMenuKeys);
    rip_read_bool(pParser, L"PALETTE_TOP", p->m_bIsOnPaletteTop);
    rip_read_bool(pParser, L"ALWAYS_LARGE", p->m_bIsAlwaysLarge);
    return 1;
}

// Symbol: ?Read@XElementButton@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x100f00 (mfc140u): the ELEMENTS/ELEMENT list into
    // m_arSubItems (+0x60); ReadInt INDEX_SMALL (+0x50) and INDEX_LARGE
    // (+0x54); ReadBool DEFAULT_COMMAND (+0x58) and ALWAYS_DESCRIPTION
    // (+0x5c); then calls XElement::Read (a plain call before the epilogue,
    // not a tail jump) and returns its result.
    RI_XElementButton* p = static_cast<RI_XElementButton*>(pThis);
    ri_read_element_array(pParser, L"ELEMENTS", L"ELEMENT", p->m_arSubItems);
    rip_read_int(pParser, L"INDEX_SMALL", p->m_nSmallImageIndex);
    rip_read_int(pParser, L"INDEX_LARGE", p->m_nLargeImageIndex);
    rip_read_bool(pParser, L"DEFAULT_COMMAND", p->m_bIsDefaultCommand);
    rip_read_bool(pParser, L"ALWAYS_DESCRIPTION", p->m_bIsAlwaysShowDescription);
    return impl__Read_XElement_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonApplication@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonApplication_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x103a30 (mfc140u): the IMAGE sub-element into m_Image
    // (+0x88) through the XImage helper (result ignored), then a tail jump to
    // XElementButton::Read.
    RI_XElementButtonApplication* p = static_cast<RI_XElementButtonApplication*>(pThis);
    ri_read_image(L"IMAGE", p->m_Image, pParser);
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonCheck@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonCheck_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x101590 (mfc140u): a bare jump to XElementButton::Read.
    // (Check, Launch, MainPanel and Label all fold onto this one RVA.)
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonColor@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonColor_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x102fc0 (mfc140u): ReadColor COLOR (+0x280); ReadBool
    // SIMPLE_LOOK (+0x284); then
    //   pSub = NULL; rParser.Read(CString(L"AUTOMATIC_BTN"), &pSub);
    //   if (pSub) { ReadString LABEL (+0x288), TOOLTIP (+0x290); ReadColor COLOR (+0x298);
    //               ReadBool PALETTE_TOP (+0x29c), BORDER (+0x2a0); delete pSub; }
    //   pSub = NULL; rParser.Read(CString(L"OTHER_BTN"), &pSub);
    //   if (pSub) { ReadString LABEL (+0x2a8), TOOLTIP (+0x2b0); delete pSub; }
    // and returns XElementButtonGallery::Read(rParser).
    RI_XElementButtonColor* p = static_cast<RI_XElementButtonColor*>(pThis);
    rip_read_color(pParser, L"COLOR", p->m_clrColor);
    rip_read_bool(pParser, L"SIMPLE_LOOK", p->m_bSimpleButtonLook);
    void* sub = nullptr;
    rip_sub(pParser, RIP_Read, L"AUTOMATIC_BTN", &sub);
    if (sub != nullptr) {
        rip_read_string(sub, L"LABEL", p->m_strAutomaticBtnLabel);
        rip_read_string(sub, L"TOOLTIP", p->m_strAutomaticBtnToolTip);
        rip_read_color(sub, L"COLOR", p->m_clrAutomaticBtnColor);
        rip_read_bool(sub, L"PALETTE_TOP", p->m_bAutomaticBtnOnTop);
        rip_read_bool(sub, L"BORDER", p->m_bAutomaticBtnBorder);
        rip_delete(sub);
    }
    sub = nullptr;
    rip_sub(pParser, RIP_Read, L"OTHER_BTN", &sub);
    if (sub != nullptr) {
        rip_read_string(sub, L"LABEL", p->m_strOtherBtnLabel);
        rip_read_string(sub, L"TOOLTIP", p->m_strOtherBtnToolTip);
        rip_delete(sub);
    }
    return impl__Read_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonGallery@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1027e0 (mfc140u): ReadBool BUTTON_MODE (+0x88),
    // MENU_RESIZE (+0x8c), MENU_RESIZE_VERTICAL (+0x90); ReadInt ICONS_IN_ROW
    // (+0x94); ReadSize SIZE_ICON (+0x98); then
    //   if (readImage(L"IMAGE", m_Images (+0xa0)))      -- TRUE also when absent
    //       m_Images.m_Image.m_sizeImage (+0x128) = m_sizeIcon;
    //   pGroups = NULL; rParser.Read(CString(L"GROUPS"), &pGroups);
    //   if (pGroups) {
    //     pColl = NULL; pGroups->ReadCollection(CString(L"GROUP"), &pColl);
    //     if (pColl) {
    //       for (UINT i = 0; i < pColl->GetCount(); i++) {
    //         pItem = pColl->GetItem(i);
    //         if (pItem) {
    //           XGalleryGroup* g = new XGalleryGroup;     (operator new(0x10), ctor folded onto XID::XID)
    //           if (g) { if (g->Read(*pItem)) m_arGroups.Add(g);
    //                    else { g->~XGalleryGroup(); operator delete(g); } }
    //         }
    //       }
    //       delete pColl;
    //     }
    //     delete pGroups;
    //   }
    // and returns XElementButton::Read(rParser).
    RI_XElementButtonGallery* p = static_cast<RI_XElementButtonGallery*>(pThis);
    rip_read_bool(pParser, L"BUTTON_MODE", p->m_bIsButtonMode);
    rip_read_bool(pParser, L"MENU_RESIZE", p->m_bEnableMenuResize);
    rip_read_bool(pParser, L"MENU_RESIZE_VERTICAL", p->m_bMenuResizeVertical);
    rip_read_int(pParser, L"ICONS_IN_ROW", p->m_nIconsInRow);
    static_assert(offsetof(RI_XElementButtonGallery, m_sizeIcon_cy) ==
                  offsetof(RI_XElementButtonGallery, m_sizeIcon_cx) + 4, "m_sizeIcon is one CSize");
    RI_Size* sizeIcon = reinterpret_cast<RI_Size*>(&p->m_sizeIcon_cx);
    rip_read_size(pParser, L"SIZE_ICON", *sizeIcon);
    if (ri_read_image(L"IMAGE", p->m_Images, pParser)) {
        ri_image_size(p->m_Images) = *sizeIcon;
    }
    void* groups = nullptr;
    rip_sub(pParser, RIP_Read, L"GROUPS", &groups);
    if (groups != nullptr) {
        void* coll = nullptr;
        rip_sub(groups, RIP_ReadCollection, L"GROUP", &coll);
        if (coll != nullptr) {
            for (unsigned i = 0; i < ric_count(coll); ++i) {
                void* item = ric_item(coll, i);
                if (item == nullptr) continue;
                RI_XGalleryGroup* g = static_cast<RI_XGalleryGroup*>(impl___2_YAPEAX_K_Z(sizeof(RI_XGalleryGroup)));
                if (g == nullptr) continue;
                impl___0XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(g);
                if (impl__Read_XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(g, item)) {
                    ri_array_add_ptr(p->m_arGroups, g);
                } else {
                    impl___1XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAA_XZ(g);
                    impl___3_YAXPEAX_Z(g);
                }
            }
            rip_delete(coll);
        }
        rip_delete(groups);
    }
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonLaunch@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonLaunch_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x101590 (mfc140u): a bare jump to XElementButton::Read.
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonLinkCtrl@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonLinkCtrl_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1015b0 (mfc140u): ReadString LINK into m_strLink (+0x88),
    // then a tail jump to XElementButton::Read.
    RI_XElementButtonLinkCtrl* p = static_cast<RI_XElementButtonLinkCtrl*>(pThis);
    rip_read_string(pParser, L"LINK", p->m_strLink);
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonMainPanel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonMainPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x101590 (mfc140u): a bare jump to XElementButton::Read.
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementButtonUndo@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementButtonUndo_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x103a10 (mfc140u): a bare jump to XElementButtonGallery::Read.
    return impl__Read_XElementButtonGallery_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementComboBox@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x101c40 (mfc140u):
    //   pItems = NULL; rParser.Read(CString(L"ITEMS"), &pItems);
    //   if (pItems) {
    //     pColl = NULL; pItems->ReadCollection(CString(L"ITEM"), &pColl);
    //     if (pColl) {
    //       for (UINT i = 0; i < pColl->GetCount(); i++) {
    //         pItem = pColl->GetItem(i);
    //         if (pItem) { CString s; if (pItem->ReadValue(s))
    //                      m_arItems.SetAtGrow(m_arItems.GetSize(), s); }   (CStringArray export, RVA 0x1d5150)
    //       }
    //       delete pColl;
    //     }
    //     delete pItems;
    //   }
    // ReadBool EDIT_BOX (+0xa8), DROPDOWN_LIST (+0xac), DROPDOWN_LIST_RESIZE
    // (+0xb0); ReadString VALUE into XElementComboBox's own m_strValue (+0xe0);
    // return XElementEdit::Read(rParser).
    // m_arItems (+0xb8) is a CStringArray in the retail layout, which
    // OpenMFC's CStringArray does not share (see ~XElementComboBox), so the
    // append is done on the raw CArray image: SetAtGrow(GetSize(), s) on an
    // array of CString is one more zeroed 8-byte slot followed by a
    // copy-construction into it.  The strings placed here are never released
    // by ~XElementComboBox (it leaves m_arItems alone) -- they leak, as that
    // destructor's comment already says of anything a client put there.
    RI_XElementComboBox* p = static_cast<RI_XElementComboBox*>(pThis);
    void* items = nullptr;
    rip_sub(pParser, RIP_Read, L"ITEMS", &items);
    if (items != nullptr) {
        void* coll = nullptr;
        rip_sub(items, RIP_ReadCollection, L"ITEM", &coll);
        if (coll != nullptr) {
            for (unsigned i = 0; i < ric_count(coll); ++i) {
                void* item = ric_item(coll, i);
                if (item == nullptr) continue;
                CString s;
                if (rip_read_value(item, s)) {
                    const long long idx = ri_array_append_slot(p->m_arItems, sizeof(CString));
                    if (idx >= 0) {
                        ::new (static_cast<void*>(static_cast<CString*>(p->m_arItems.m_pData) + idx)) CString(s);
                    }
                }
            }
            rip_delete(coll);
        }
        rip_delete(items);
    }
    rip_read_bool(pParser, L"EDIT_BOX", p->m_bHasEditBox);
    rip_read_bool(pParser, L"DROPDOWN_LIST", p->m_bHasDropDownList);
    rip_read_bool(pParser, L"DROPDOWN_LIST_RESIZE", p->m_bResizeDropDownList);
    rip_read_string(pParser, L"VALUE", p->m_strValue);
    return impl__Read_XElementEdit_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementEdit@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementEdit_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x101720 (mfc140u): ReadInt WIDTH (+0x88) and WIDTH_FLOATY
    // (+0x8c); ReadBool SPIN_BUTTONS (+0x90); if (m_bHasSpinButtons) { ReadInt
    // MIN (+0x94) and MAX (+0x98); } ReadString VALUE (+0xa0); then a tail
    // jump to XElementButton::Read.
    RI_XElementEdit* p = static_cast<RI_XElementEdit*>(pThis);
    rip_read_int(pParser, L"WIDTH", p->m_nWidth);
    rip_read_int(pParser, L"WIDTH_FLOATY", p->m_nWidthFloaty);
    rip_read_bool(pParser, L"SPIN_BUTTONS", p->m_bHasSpinButtons);
    if (p->m_bHasSpinButtons != 0) {
        rip_read_int(pParser, L"MIN", p->m_nMin);
        rip_read_int(pParser, L"MAX", p->m_nMax);
    }
    rip_read_string(pParser, L"VALUE", p->m_strValue);
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementFontComboBox@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementFontComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x102340 (mfc140u): ReadInt FONT_TYPE (+0xe8); then for each
    // of the two BYTE members: int v = member; ReadInt(name, v); member =
    // (BYTE)v -- CHAR_SET (+0xec) and PITCH_AND_FAMILY (+0xed); and returns
    // XElementComboBox::Read(rParser).
    RI_XElementFontComboBox* p = static_cast<RI_XElementFontComboBox*>(pThis);
    rip_read_int(pParser, L"FONT_TYPE", p->m_nFontType);
    int v = p->m_nCharSet;
    rip_read_int(pParser, L"CHAR_SET", v);
    p->m_nCharSet = static_cast<unsigned char>(v);
    v = p->m_nPitchAndFamily;
    rip_read_int(pParser, L"PITCH_AND_FAMILY", v);
    p->m_nPitchAndFamily = static_cast<unsigned char>(v);
    return impl__Read_XElementComboBox_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementGroup@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementGroup_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x100b00 (mfc140u): the ELEMENTS/ELEMENT list into
    // m_arButtons (+0x208); the IMAGE sub-element into m_Images (+0x50)
    // through the XImage helper (result ignored); return m_arButtons.GetSize()
    // > 0.  It does NOT chain to XElement::Read.
    RI_XElementGroup* p = static_cast<RI_XElementGroup*>(pThis);
    ri_read_element_array(pParser, L"ELEMENTS", L"ELEMENT", p->m_arButtons);
    ri_read_image(L"IMAGE", p->m_Images, pParser);
    return p->m_arButtons.m_nSize > 0 ? 1 : 0;
}

// Symbol: ?Read@XElementLabel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementLabel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x101590 (mfc140u): a bare jump to XElementButton::Read.
    return impl__Read_XElementButton_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XElementSeparator@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementSeparator_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1009f0 (mfc140u): ReadBool HORIZ into m_bIsHoriz (+0x50)
    // and return TRUE.  It does not chain to XElement::Read.
    RI_XElementSeparator* p = static_cast<RI_XElementSeparator*>(pThis);
    rip_read_bool(pParser, L"HORIZ", p->m_bIsHoriz);
    return 1;
}

// Symbol: ?Read@XElementSlider@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XElementSlider_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x103b30 (mfc140u): int style = m_dwStyle; ReadInt STYLE
    // style; m_dwStyle = style; ReadInt WIDTH (+0x54), MIN (+0x58), MAX
    // (+0x5c), POS (+0x60); then m_nPos = min(max(m_nPos, m_nMin), m_nMax)
    // (cmovle/cmovl at 0x103d04..0x103d16); ReadBool ZOOM_BUTTONS (+0x64);
    // return XElement::Read(rParser).
    RI_XElementSlider* p = static_cast<RI_XElementSlider*>(pThis);
    int style = static_cast<int>(p->m_dwStyle);
    rip_read_int(pParser, L"STYLE", style);
    p->m_dwStyle = static_cast<unsigned long>(style);
    rip_read_int(pParser, L"WIDTH", p->m_nWidth);
    rip_read_int(pParser, L"MIN", p->m_nMin);
    rip_read_int(pParser, L"MAX", p->m_nMax);
    rip_read_int(pParser, L"POS", p->m_nPos);
    int pos = p->m_nPos;
    if (pos < p->m_nMin) pos = p->m_nMin;
    if (pos > p->m_nMax) pos = p->m_nMax;
    p->m_nPos = pos;
    rip_read_bool(pParser, L"ZOOM_BUTTONS", p->m_bZoomButtons);
    return impl__Read_XElement_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(pThis, pParser);
}

// Symbol: ?Read@XGalleryGroup@XElementButtonGallery@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@3@@Z
extern "C" int MS_ABI impl__Read_XGalleryGroup_XElementButtonGallery_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1025e0 (mfc140u): ReadString NAME into m_strName (+0x00),
    // ReadInt ITEMS into m_nItems (+0x08), return TRUE.
    RI_XGalleryGroup* p = static_cast<RI_XGalleryGroup*>(pThis);
    rip_read_string(pParser, L"NAME", p->m_strName);
    rip_read_int(pParser, L"ITEMS", p->m_nItems);
    return 1;
}

// Symbol: ?Read@XID@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XID_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0xff870 (mfc140u):
    //   BOOL bRes = rParser.ReadString(CString(L"NAME"), m_Name);
    //   UINT value = m_Value;
    //   if (rParser.ReadUInt(CString(L"VALUE"), value)) bRes = TRUE;   (both calls always made)
    //   m_Value = value;
    //   return bRes;
    RI_XID* p = static_cast<RI_XID*>(pThis);
    int bRes = rip_read_string(pParser, L"NAME", p->m_Name);
    unsigned value = p->m_Value;
    if (rip_read_uint(pParser, L"VALUE", value)) bRes = 1;
    p->m_Value = value;
    return bRes;
}

// Symbol: ?Read@XImage@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XImage_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0xffaa0 (mfc140u):
    //   if (!readID(L"ID", m_ID, rParser))          -- FALSE only when an ID element
    //       return rParser.ReadValue(m_ID.m_Name);     exists and XID::Read failed
    //   readID(L"ID_HDPI", m_ID_HDPI, rParser);
    //   return TRUE;
    RI_XImage* p = static_cast<RI_XImage*>(pThis);
    if (!ri_read_id(L"ID", p->m_ID, pParser)) {
        return rip_read_value(pParser, p->m_ID.m_Name);
    }
    ri_read_id(L"ID_HDPI", p->m_ID_HDPI, pParser);
    return 1;
}

// Symbol: ?Read@XPanel@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XPanel_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1044e0 (mfc140u): the ELEMENTS/ELEMENT list into
    // m_arElements (+0xb8); then
    //   pLaunch = NULL; rParser.Read(CString(L"BUTTON_LAUNCH"), &pLaunch);
    //   if (pLaunch) { m_btnLaunch.Read(*pLaunch); delete pLaunch; }   (vtable slot 1 of +0x30)
    // ReadBool JUSTIFY_COLUMNS (+0x24) and CENTER_COLUMN_VERT (+0x28); ReadInt
    // INDEX (+0x20); ReadString NAME (+0x10) and KEYS (+0x18); return TRUE.
    // m_btnLaunch's Read is XElementButtonLaunch::Read, called directly
    // (deviation 1).
    RI_XPanel* p = static_cast<RI_XPanel*>(pThis);
    ri_read_element_array(pParser, L"ELEMENTS", L"ELEMENT", p->m_arElements);
    void* launch = nullptr;
    rip_sub(pParser, RIP_Read, L"BUTTON_LAUNCH", &launch);
    if (launch != nullptr) {
        impl__Read_XElementButtonLaunch_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(&p->m_btnLaunch, launch);
        rip_delete(launch);
    }
    rip_read_bool(pParser, L"JUSTIFY_COLUMNS", p->m_bJustifyColumns);
    rip_read_bool(pParser, L"CENTER_COLUMN_VERT", p->m_bCenterColumnVert);
    rip_read_int(pParser, L"INDEX", p->m_nImageIndex);
    rip_read_string(pParser, L"NAME", p->m_strName);
    rip_read_string(pParser, L"KEYS", p->m_strKeys);
    return 1;
}

// Symbol: ?Read@XQAT@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XQAT_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x1067a0 (mfc140u):
    //   pItems = NULL; rParser.Read(CString(L"ITEMS"), &pItems);
    //   if (pItems) {
    //     pColl = NULL; pItems->ReadCollection(CString(L"ITEM"), &pColl);
    //     if (pColl) {
    //       for (UINT i = 0; i < pColl->GetCount(); i++) {
    //         XQATItem item;                                (ctor inlined: XID(), m_bVisible = TRUE)
    //         if (item.Read(*pColl->GetItem(i))) AddItem(item);   (no NULL test on GetItem's result)
    //       }
    //       delete pColl;
    //     }
    //     delete pItems;
    //   }
    //   ReadBool QAT_TOP (+0x38); return TRUE;
    // AddItem is inlined in retail (CloneData copy + CArray::Add); the
    // exported thunk below does the same.  Retail passes GetItem's result to
    // XQATItem::Read without a NULL check and would fault on one; that is
    // reproduced as-is rather than papered over.
    RI_XQAT* p = static_cast<RI_XQAT*>(pThis);
    void* items = nullptr;
    rip_sub(pParser, RIP_Read, L"ITEMS", &items);
    if (items != nullptr) {
        void* coll = nullptr;
        rip_sub(items, RIP_ReadCollection, L"ITEM", &coll);
        if (coll != nullptr) {
            for (unsigned i = 0; i < ric_count(coll); ++i) {
                // raw storage: the RI_* views are never used as C++ objects
                alignas(8) unsigned char itemBuf[sizeof(RI_XQATItem)];
                RI_XQATItem* item = reinterpret_cast<RI_XQATItem*>(itemBuf);
                impl___0XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(item);
                void* sub = ric_item(coll, i);
                if (impl__Read_XQATItem_XQAT_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(item, sub)) {
                    impl__AddItem_XQAT_CMFCRibbonInfo__QEAA_JAEBVXQATItem_12__Z(pThis, item);
                }
                impl___1XQATItem_XQAT_CMFCRibbonInfo__QEAA_XZ(item);
            }
            rip_delete(coll);
        }
        rip_delete(items);
    }
    rip_read_bool(pParser, L"QAT_TOP", p->m_bOnTop);
    return 1;
}

// Symbol: ?Read@XQATItem@XQAT@CMFCRibbonInfo@@QEAAHAEAVXRibbonInfoParser@3@@Z
extern "C" int MS_ABI impl__Read_XQATItem_XQAT_CMFCRibbonInfo__QEAAHAEAVXRibbonInfoParser_3__Z(void* pThis, void* pParser) {
    // Retail RVA 0x106600 (mfc140u): if (readID(L"ID", m_ID, rParser))
    // rParser.ReadBool(CString(L"VISIBLE"), m_bVisible (+0x10)); return TRUE.
    RI_XQATItem* p = static_cast<RI_XQATItem*>(pThis);
    if (ri_read_id(L"ID", p->m_ID, pParser)) {
        rip_read_bool(pParser, L"VISIBLE", p->m_bVisible);
    }
    return 1;
}

// Symbol: ?Read@XRibbonBar@CMFCRibbonInfo@@UEAAHAEAVXRibbonInfoParser@2@@Z
extern "C" int MS_ABI impl__Read_XRibbonBar_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(void* pThis, void* pParser) {
    // Retail RVA 0x106ba0 (mfc140u):
    //   pSub = NULL; rParser.Read(CString(L"BUTTON_MAIN"), &pSub);
    //   if (pSub) { m_btnMain = new XElementButtonApplication;     (operator new(0x240) + ctor)
    //               m_btnMain->Read(*pSub); delete pSub; }        (no NULL test on m_btnMain)
    //   pSub = NULL; rParser.Read(CString(L"CATEGORY_MAIN"), &pSub);
    //   if (pSub) { m_MainCategory = new XCategoryMain;            (operator new(0x3c8) + ctor)
    //               m_MainCategory->Read(*pSub); delete pSub; }
    //   pSub = NULL; rParser.Read(CString(L"QAT_ELEMENTS"), &pSub);
    //   if (pSub) { m_QAT.Read(*pSub); delete pSub; }             (vtable slot 1 of +0x1e8)
    //   pSub = NULL; rParser.Read(CString(L"TAB_ELEMENTS"), &pSub);
    //   if (pSub) { m_TabElements.Read(*pSub); delete pSub; }     (vtable slot 1 of +0x228)
    //   the CONTEXTS/CONTEXT list into m_arContexts (+0x480);
    //   the CATEGORIES/CATEGORY list into m_arCategories (+0x458);
    //   ReadBool ENABLE_TOOLTIPS (+0x10), ENABLE_TOOLTIPS_DESCRIPTION (+0x14),
    //   ENABLE_KEYS (+0x18), ENABLE_PRINTPREVIEW (+0x1c), ENABLE_DRAWUSINGFONT (+0x20);
    //   the IMAGE sub-element into m_Images (+0x28);
    //   return TRUE;
    // The four virtual Reads are called directly (deviation 1).  Retail does
    // not test the two operator new results before dispatching Read on them;
    // here a failed allocation skips the Read instead of faulting.  An
    // existing m_btnMain / m_MainCategory is overwritten without being freed,
    // exactly as retail does.
    RI_XRibbonBar* p = static_cast<RI_XRibbonBar*>(pThis);
    void* sub = nullptr;
    rip_sub(pParser, RIP_Read, L"BUTTON_MAIN", &sub);
    if (sub != nullptr) {
        void* btn = impl___2_YAPEAX_K_Z(sizeof(RI_XElementButtonApplication));
        if (btn != nullptr) impl___0XElementButtonApplication_CMFCRibbonInfo__QEAA_XZ(btn);
        p->m_btnMain = btn;
        if (btn != nullptr) {
            impl__Read_XElementButtonApplication_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(btn, sub);
        }
        rip_delete(sub);
    }
    sub = nullptr;
    rip_sub(pParser, RIP_Read, L"CATEGORY_MAIN", &sub);
    if (sub != nullptr) {
        void* main = impl___2_YAPEAX_K_Z(sizeof(RI_XCategoryMain));
        if (main != nullptr) impl___0XCategoryMain_CMFCRibbonInfo__QEAA_XZ(main);
        p->m_MainCategory = main;
        if (main != nullptr) {
            impl__Read_XCategoryMain_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(main, sub);
        }
        rip_delete(sub);
    }
    sub = nullptr;
    rip_sub(pParser, RIP_Read, L"QAT_ELEMENTS", &sub);
    if (sub != nullptr) {
        impl__Read_XQAT_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(&p->m_QAT, sub);
        rip_delete(sub);
    }
    sub = nullptr;
    rip_sub(pParser, RIP_Read, L"TAB_ELEMENTS", &sub);
    if (sub != nullptr) {
        impl__Read_XElementGroup_CMFCRibbonInfo__UEAAHAEAVXRibbonInfoParser_2__Z(&p->m_TabElements, sub);
        rip_delete(sub);
    }
    ri_read_element_array(pParser, L"CONTEXTS", L"CONTEXT", p->m_arContexts);
    ri_read_element_array(pParser, L"CATEGORIES", L"CATEGORY", p->m_arCategories);
    rip_read_bool(pParser, L"ENABLE_TOOLTIPS", p->m_bToolTip);
    rip_read_bool(pParser, L"ENABLE_TOOLTIPS_DESCRIPTION", p->m_bToolTipDescr);
    rip_read_bool(pParser, L"ENABLE_KEYS", p->m_bKeyTips);
    rip_read_bool(pParser, L"ENABLE_PRINTPREVIEW", p->m_bPrintPreview);
    rip_read_bool(pParser, L"ENABLE_DRAWUSINGFONT", p->m_bDrawUsingFont);
    ri_read_image(L"IMAGE", p->m_Images, pParser);
    return 1;
}

// Symbol: ?ReadBool@XRibbonInfoParser@CMFCRibbonInfo@@UEAAHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAH@Z
extern "C" int MS_ABI impl__ReadBool_XRibbonInfoParser_CMFCRibbonInfo__UEAAHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAH_Z(
    void* pThis, const CString* pName, int* pValue) {
    // Retail RVA 0x108c80 (mfc140u):
    //   CString str;
    //   if (ReadString(strName, str) && !str.IsEmpty()) {     (virtual slot 4 on this)
    //     str.TrimRight(); str.TrimLeft();                     (the CStringT exports)
    //     val = (wcscmp(str, L"TRUE") == 0);                   (IAT slot 0x2c7770 = wcscmp)
    //     return TRUE;
    //   }
    //   return FALSE;
    // The trim exports here trim ' ', '\t', '\r', '\n' (OpenMFC's inline
    // CString), where retail's CStringT trims everything iswspace() accepts.
    CString str;
    if (reinterpret_cast<RIP_StrFn>(ri_slot(pThis, RIP_ReadString))(pThis, pName, &str) && str.GetLength() != 0) {
        impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&str);
        impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&str);
        *pValue = (std::wcscmp(str.GetString(), L"TRUE") == 0) ? 1 : 0;
        return 1;
    }
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
extern "C" void MS_ABI impl__SetError_XInfoError_CMFCRibbonInfo__QEAAXW4XError_12_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__JJ_Z(
    void* pThis, int error, const CString* pDescription, long nLine, long nLinePos) {
    // Retail RVA 0xfd910 (mfc140u): Empty() inlined (m_Error = 0, m_nLine =
    // m_nLinePos = -1, m_strDescription.Empty()), then m_Error = error,
    // m_strDescription = strDescription (CSimpleStringT::operator=),
    // m_nLinePos = nLinePos (the fifth argument, read from the stack) and
    // m_nLine = nLine.
    RI_XInfoError* p = static_cast<RI_XInfoError*>(pThis);
    p->m_Error    = 0;
    p->m_nLine    = -1;
    p->m_nLinePos = -1;
    p->m_strDescription.Empty();
    p->m_Error = error;
    p->m_strDescription = *pDescription;
    p->m_nLinePos = nLinePos;
    p->m_nLine    = nLine;
}

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
