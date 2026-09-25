// CMFCToolBarFontSizeComboBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// EVERY RVA IN THIS FILE IS AN mfc140u.dll RVA (14.51.36231).  The bodies
// marked "Retail (RVA 0x..., mfc140u)" were transcribed from the disassembly
// of that entry point.  Entry points come from the export map
// (mfc140u_rva_symbols.json) except RebuildFontSizes (0x169fe0),
// TwipsToPointString (0x16a350) and SetTwipSize (0x16a400), which are not in
// that map and were resolved through the mfc140u export table by ordinal
// (12128, 14045, 13717).  The vftable is at 0x180318050 (mfc140u), stored by
// both ctors and by the dtor; in it slot 53 (+0x1a8) is
// CMFCToolBarComboBoxButton::AddItem (0x1628b0) and slot 55 (+0x1b8) is this
// class's CreateCombo (0x16a640).
//
// Instance layout (atlmfc/include/afxtoolbarfontcombobox.h:129: the class adds
// one `int m_nLogVert` after the CMFCToolBarComboBoxButton base), confirmed by
// the disassembly:
//   +0x000  CMFCToolBarComboBoxButton base (sizeof 0x150)
//   +0x150  int m_nLogVert   (both ctors store 0; RebuildFontSizes stores
//                             GetDeviceCaps(LOGPIXELSY); EnumSizeCallBack reads it)
//   sizeof == 0x158 (CreateObject 0x169ea0: operator new(0x158))
// include/openmfc does not declare this class, so the bodies take `void*
// pThis` and reach the member by offset (FontSizeOwn below).  The base
// members used (m_dwStyle +0xa4, m_pWndCombo +0xd0, m_strEdit +0xd8) are the
// retail offsets documented by featurepack/toolbar/CMFCToolBarComboBoxButton.cpp.
//
// Deviations applied throughout (the conventions of CMFCToolBarFontComboBox.cpp):
//  (1) vptr.  Retail's ctors and dtor store the CMFCToolBarFontSizeComboBox
//      vftable (0x180318050).  OpenMFC has no MSVC-layout vtable for this
//      class; the ctors keep the vptr the exported base ctor installs and
//      record it (g_ownVptr), and the dtor stores that recorded value back in
//      place of retail's store, so that the base dtor thunk's C++
//      virtual-destructor call dispatches on OpenMFC's table rather than on a
//      client-derived class's MSVC table.  Consequence: an object built here
//      reports CMFCToolBarComboBoxButton's runtime class and its virtual
//      CreateCombo resolves to the base's.
//  (2) Virtual calls are devirtualised: AddItem (vslot 0x1a8, slot 53) goes to
//      the base's exported AddItem thunk.  A client override is not reached.
//  (3) The combo's own item list.  Retail reads the base's m_lstItems (+0xe0)
//      and m_lstItemData (+0x118) directly; OpenMFC keeps the combo items in
//      the cbarcore side table (see the header of CMFCToolBarComboBoxButton.cpp),
//      so those reads go through the exported GetCount / GetItem / GetItemData.
//  (4) Exceptions.  The AfxThrow* paths retail can take are not raised from
//      this file; each site says what it does instead.
//  (5) NULL guards.  Every body returns early (NULL / FALSE / nothing, or -2
//      from GetTwipSize) on a NULL `this`, a NULL hidden-return pointer, a
//      NULL `rect` (CreateCombo), a NULL font-name reference
//      (RebuildFontSizes) or a NULL lpv / lpntm (EnumSizeCallBack).  Retail
//      tests none of these and would fault; only the lpntm and GetTwipSize
//      guards are also noted at their sites.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <cstdlib>
#include <new>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named;
// the parameter lists follow those definitions (and the mangled names).
// ---------------------------------------------------------------------------
//   featurepack/toolbar/Thunks.cpp : base ctors / dtor
extern "C" void* MS_ABI impl___0CMFCToolBarComboBoxButton__QEAA_IHKH_Z(void* pThis, unsigned int uiID, int iImage, unsigned long dwStyle, int iWidth);
extern "C" void* MS_ABI impl___0CMFCToolBarComboBoxButton__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarComboBoxButton__UEAA_XZ(void* pThis);
//   featurepack/toolbar/CMFCToolBarComboBoxButton.cpp : base item API
extern "C" INT_PTR MS_ABI impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(
    CMFCToolBarComboBoxButton* pThis, const wchar_t* lpszItem, DWORD_PTR dwData);
extern "C" INT_PTR MS_ABI impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(const CMFCToolBarComboBoxButton* pThis);
extern "C" const wchar_t* MS_ABI impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(const CMFCToolBarComboBoxButton* pThis, int iIndex);
extern "C" DWORD_PTR MS_ABI impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(const CMFCToolBarComboBoxButton* pThis, int iIndex);
extern "C" int MS_ABI impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* lpszItem);
extern "C" void MS_ABI impl__SetText_CMFCToolBarComboBoxButton__QEAAXPEB_W_Z(CMFCToolBarComboBoxButton* pThis, const wchar_t* lpszText);
extern "C" void MS_ABI impl__RemoveAllItems_CMFCToolBarComboBoxButton__QEAAXXZ(CMFCToolBarComboBoxButton* pThis);
//   featurepack/toolbar/CMFCToolBar.cpp : GetCommandButtons
extern "C" int MS_ABI impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(unsigned int uiCmd, CObList* pLstButtons);
//   featurepack/toolbar/RuntimeClasses.cpp : RUNTIME_CLASS(CMFCToolBarComboBoxButton)
//   (retail 0x3b1658, mfc140u -- what ?GetRuntimeClass@CMFCToolBarComboBoxButton@@ 0x8acc0 returns)
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ();
//   core/runtime/CObject.cpp : IsKindOf
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/collections/CObList.cpp : ctor / dtor / FindIndex
extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(void* pThis, long long nBlockSize);
extern "C" void MS_ABI impl___1CObList__UEAA_XZ(void* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
//   featurepack/controls/CMFCFontComboBox.cpp
extern "C" void* MS_ABI impl___0CMFCFontComboBox__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCFontComboBox__UEAA_XZ(void* pThis);
//   detail/CtrlcoreSupport.cpp : CComboBox::Create
extern "C" int MS_ABI impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CComboBox* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);
//   core/window/CWnd.cpp : CWnd::SetWindowTextW
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpsz);
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z / ??3@YAXPEAX@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);

// Exports defined later in this file that earlier bodies call.
extern "C" void* MS_ABI impl___0CMFCToolBarFontSizeComboBox__IEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__InsertSize_CMFCToolBarFontSizeComboBox__IEAAXH_Z(void* pThis, int nSize);
extern "C" CString* MS_ABI impl__TwipsToPointString_CMFCToolBarFontSizeComboBox__IEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* pThis, CString* pRet, int nTwips);
extern "C" int MS_ABI impl__EnumSizeCallBack_CMFCToolBarFontSizeComboBox__KAHPEAUtagLOGFONTW__PEAUtagNEWTEXTMETRICW__HPEAX_Z(
    LOGFONTW* lplf, NEWTEXTMETRICW* lpntm, int FontType, void* lpv);

namespace {

// ---------------------------------------------------------------------------
// Own member (+0x150, see the file header).
// ---------------------------------------------------------------------------
struct FontSizeOwn {
    int m_nLogVert;   // +0x150
};
constexpr std::size_t kOffOwn = 0x150;
static_assert(offsetof(FontSizeOwn, m_nLogVert) == 0x00, "m_nLogVert @+0x150");
static_assert(sizeof(CMFCToolBarComboBoxButton) == kOffOwn, "the base ends at +0x150 (retail sizeof 336)");
constexpr std::size_t kSizeofFontSizeComboBox = 0x158;   // CreateObject 0x169ea0: operator new(0x158)
static_assert((kOffOwn + sizeof(FontSizeOwn) + 7) / 8 * 8 == kSizeofFontSizeComboBox,
              "retail sizeof(CMFCToolBarFontSizeComboBox) is 0x158 (+0x154 is tail padding to the 8-byte alignment)");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24 (CreateCombo / RebuildFontSizes read +0x24)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads the combo HWND at +0x40");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer (ATL layout)");

// Base members (retail offsets, see CMFCToolBarComboBoxButton.cpp's header).
constexpr std::size_t kOffStyle    = 0xa4;   // DWORD      m_dwStyle
constexpr std::size_t kOffWndCombo = 0xd0;   // CComboBox* m_pWndCombo
constexpr std::size_t kOffStrEdit  = 0xd8;   // CString    m_strEdit

template <typename T> T& At(void* p, std::size_t off) { return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off); }
template <typename T> const T& At(const void* p, std::size_t off) { return *reinterpret_cast<const T*>(static_cast<const unsigned char*>(p) + off); }

FontSizeOwn* Own(void* p) { return reinterpret_cast<FontSizeOwn*>(static_cast<unsigned char*>(p) + kOffOwn); }
CMFCToolBarComboBoxButton* Combo(void* p) { return static_cast<CMFCToolBarComboBoxButton*>(p); }
const CMFCToolBarComboBoxButton* Combo(const void* p) { return static_cast<const CMFCToolBarComboBoxButton*>(p); }

// The vptr the exported base ctor installs (deviation (1)).
void* g_ownVptr = nullptr;

// `if (m_pWndCombo != NULL) ::SendMessage(m_pWndCombo->m_hWnd, WM_SETREDRAW, bRedraw, 0)`
// -- retail tests only the CComboBox pointer, not the HWND (IAT 0x1802c7120 =
// SendMessageW).
void SetRedraw(void* pButton, BOOL bRedraw) {
    CComboBox* pWndCombo = At<CComboBox*>(pButton, kOffWndCombo);
    if (pWndCombo != nullptr) {
        ::SendMessage(pWndCombo->m_hWnd, WM_SETREDRAW, static_cast<WPARAM>(bRedraw), 0);
    }
}

// CObList walk (the pattern of CMFCToolBarComboBoxButton.cpp's
// CommandButtonList): a CObList built and torn down through the exported
// ctor/dtor; the nodes a POSITION points at mirror CList<CObject*>::CNode.
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
class CommandButtonList {
public:
    explicit CommandButtonList(unsigned uiCmd) {
        std::memset(m_buf, 0, sizeof(m_buf));
        impl___0CObList__QEAA__J_Z(List(), 10);   // retail: m_nBlockSize = 10 (the xmm store from 0x350150)
        m_count = impl__GetCommandButtons_CMFCToolBar__SAHIAEAVCObList___Z(uiCmd, List());
    }
    ~CommandButtonList() { impl___1CObList__UEAA_XZ(List()); }
    int Count() const { return m_count; }
    ObNode* First() const {
        CObList::POSITION pos = impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(List(), 0);
        ObNode* p = nullptr;
        std::memcpy(&p, &pos, sizeof(p));
        return p;
    }
    CommandButtonList(const CommandButtonList&) = delete;
    CommandButtonList& operator=(const CommandButtonList&) = delete;
private:
    CObList* List() const { return reinterpret_cast<CObList*>(const_cast<unsigned char*>(m_buf)); }
    alignas(8) unsigned char m_buf[sizeof(CObList)];
    int m_count;
};

// m_lstItems.Find(lpsz) != NULL -- CStringList::Find (0x2329e0, mfc140u)
// compares each node with wcscmp (IAT 0x1802c7770), i.e. an exact
// case-sensitive match.  Deviation (3): walked over the side-table items.
bool ItemsContain(const void* pThis, const wchar_t* lpsz) {
    const INT_PTR nCount = impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(Combo(pThis));
    for (INT_PTR i = 0; i < nCount; ++i) {
        const wchar_t* pItem = impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(Combo(pThis), static_cast<int>(i));
        if (pItem != nullptr && std::wcscmp(pItem, lpsz) == 0) return true;
    }
    return false;
}

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail (RVA 0x169f10, mfc140u), fully transcribed:
//     CMFCToolBarComboBoxButton::CMFCToolBarComboBoxButton();     // 0x1614d0
//     m_nLogVert = 0;                                             // +0x150
//     vfptr = 0x180318050;                                        // deviation (1)
// Symbol: ??0CMFCToolBarFontSizeComboBox@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarFontSizeComboBox__IEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CMFCToolBarComboBoxButton__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    Own(pThis)->m_nLogVert = 0;
    return pThis;
}
// Retail (RVA 0x169f90, mfc140u), fully transcribed:
//     CMFCToolBarComboBoxButton::CMFCToolBarComboBoxButton(uiID, iImage, dwStyle, iWidth);   // 0x161620
//     m_nLogVert = 0;                                             // +0x150
//     vfptr = 0x180318050;                                        // deviation (1)
// (all four arguments are forwarded unchanged: RDX / R8 / R9 untouched, the
// stack iWidth copied to the callee's fifth slot).  The previous definition
// had three int parameters and no iWidth; the list now follows the mangled name.
// Symbol: ??0CMFCToolBarFontSizeComboBox@@QEAA@IHKH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarFontSizeComboBox__QEAA_IHKH_Z(
    void* pThis, unsigned int uiID, int iImage, unsigned long dwStyle, int iWidth) {
    if (pThis == nullptr) return pThis;
    impl___0CMFCToolBarComboBoxButton__QEAA_IHKH_Z(pThis, uiID, iImage, dwStyle, iWidth);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    Own(pThis)->m_nLogVert = 0;
    return pThis;
}
// Retail (RVA 0x169fd0, mfc140u), fully transcribed:
//     vfptr = 0x180318050;                                        // deviation (1): g_ownVptr stored instead
//     CMFCToolBarComboBoxButton::~CMFCToolBarComboBoxButton();    // 0x1617c0 (tail jump)
// The previous definition had no `this` parameter; the signature now follows
// the mangled name.
// Symbol: ??1CMFCToolBarFontSizeComboBox@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarFontSizeComboBox__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CMFCToolBarComboBoxButton__UEAA_XZ(pThis);
}

// Retail (RVA 0x169ea0, mfc140u), fully transcribed:
//     void* p = operator new(0x158);                              // 0x27f0
//     return p ? new(p) CMFCToolBarFontSizeComboBox() : NULL;     // tail jump to 0x169f10
// Symbol: ?CreateObject@CMFCToolBarFontSizeComboBox@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarFontSizeComboBox__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeofFontSizeComboBox);
    if (p == nullptr) return nullptr;
    return impl___0CMFCToolBarFontSizeComboBox__IEAA_XZ(p);
}

// ===========================================================================
// Overrides
// ===========================================================================

// Retail (RVA 0x16a640, mfc140u), fully transcribed:
//     CMFCFontComboBox* pWndCombo = new CMFCFontComboBox;         // operator new(0x100), ctor 0x60ef0
//     if (!pWndCombo->Create(m_dwStyle, rect, pWndParent, m_nID)) {   // +0xa4 (no extra style bits); +0x24; vslot 0x2d8 (91)
//         delete pWndCombo;                                       // vslot 0x08 (1), flags 1
//         return NULL;
//     }
//     return pWndCombo;
// (Retail tests the operator-new result only to skip the ctor: on NULL it
// carries on with pWndCombo = NULL and dereferences it at the Create vcall.
// Here a NULL allocation, a NULL `this` or a NULL `rect` returns NULL.)
// Create goes through the
// exported CComboBox::Create -- slot 91 of the CMFCFontComboBox vftable is
// CComboBox::Create, as recorded in CMFCToolBarFontComboBox.cpp's CreateCombo,
// whose shape this follows.  The failure path runs the exported
// ~CMFCFontComboBox and ??3@YAXPEAX@Z (the object came from ??2@YAPEAX_K@Z),
// which is what the scalar deleting dtor amounts to.  The storage is zeroed
// before the ctor runs (an OpenMFC addition; retail does not zero).
// Symbol: ?CreateCombo@CMFCToolBarFontSizeComboBox@@MEAAPEAVCComboBox@@PEAVCWnd@@AEBVCRect@@@Z
extern "C" CComboBox* MS_ABI impl__CreateCombo_CMFCToolBarFontSizeComboBox__MEAAPEAVCComboBox__PEAVCWnd__AEBVCRect___Z(
    void* pThis, CWnd* pWndParent, const RECT* pRect) {
    if (pThis == nullptr || pRect == nullptr) return nullptr;
    constexpr std::size_t kSizeofFontComboBox = 0x100;   // retail operator new(0x100)
    void* pStorage = impl___2_YAPEAX_K_Z(kSizeofFontComboBox);
    if (pStorage == nullptr) return nullptr;
    std::memset(pStorage, 0, kSizeofFontComboBox);
    CComboBox* pWndCombo = static_cast<CComboBox*>(impl___0CMFCFontComboBox__QEAA_XZ(pStorage));
    const DWORD dwStyle = At<DWORD>(pThis, kOffStyle);
    if (!impl__Create_CComboBox__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(pWndCombo, dwStyle, pRect, pWndParent, Combo(pThis)->m_nID)) {
        impl___1CMFCFontComboBox__UEAA_XZ(pWndCombo);
        impl___3_YAXPEAX_Z(pWndCombo);
        return nullptr;
    }
    return pWndCombo;
}

// ===========================================================================
// Size list
// ===========================================================================

// Retail (RVA 0x16a260, mfc140u), fully transcribed:
//     CMFCToolBarFontSizeComboBox* pThis = (CMFCToolBarFontSizeComboBox*)lpv;
//     if (!(FontType & TRUETYPE_FONTTYPE) && (FontType & RASTER_FONTTYPE)) {    // test $4 / test $1
//         int nHeight = lpntm->tmHeight - lpntm->tmInternalLeading;             // +0x00 - +0x0c
//         pThis->InsertSize(::MulDiv(nHeight, 1440, pThis->m_nLogVert));         // IAT 0x1802c6658 = MulDiv; +0x150; 0x16a5c0
//         return TRUE;                                                          // keep enumerating
//     }
//     for (int i = 0; i < 16; i++) {                                            // TrueType or non-raster font: the stock list
//         CString str; str.Format(L"%d", nFontSizes[i]);                        // table at 0x33e150; "%d" at 0x33eefc
//         pThis->AddItem(str, 0);                                               // vslot 0x1a8 (53)
//     }
//     return FALSE;                                                             // stop enumerating
// nFontSizes (read from mfc140u .rdata 0x33e150) is
// { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72 }.
// lplf is not read.  AddItem goes to the base's exported thunk (deviation (2)).
// Symbol: ?EnumSizeCallBack@CMFCToolBarFontSizeComboBox@@KAHPEAUtagLOGFONTW@@PEAUtagNEWTEXTMETRICW@@HPEAX@Z
extern "C" int MS_ABI impl__EnumSizeCallBack_CMFCToolBarFontSizeComboBox__KAHPEAUtagLOGFONTW__PEAUtagNEWTEXTMETRICW__HPEAX_Z(
    LOGFONTW* lplf, NEWTEXTMETRICW* lpntm, int FontType, void* lpv) {
    (void)lplf;
    if (lpv == nullptr) return FALSE;
    if ((FontType & TRUETYPE_FONTTYPE) == 0 && (FontType & RASTER_FONTTYPE) != 0) {
        if (lpntm == nullptr) return TRUE;   // retail dereferences lpntm unconditionally
        const int nHeight = lpntm->tmHeight - lpntm->tmInternalLeading;
        impl__InsertSize_CMFCToolBarFontSizeComboBox__IEAAXH_Z(lpv, ::MulDiv(nHeight, 1440, Own(lpv)->m_nLogVert));
        return TRUE;
    }
    static const int nFontSizes[16] = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72 };
    for (int i = 0; i < 16; i++) {
        CString str;
        str.Format(L"%d", nFontSizes[i]);
        impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(Combo(lpv), static_cast<const wchar_t*>(str), 0);
    }
    return FALSE;
}

// Retail (RVA 0x16a5c0, mfc140u), fully transcribed:
//     AddItem(TwipsToPointString(nSize), (DWORD)nSize);           // 0x16a350; vslot 0x1a8 (53)
// (No FindItem / duplicate check.  The item data is the 32-bit nSize
// zero-extended -- `mov %ebx,%r8d` -- so a negative size is stored as
// 0x00000000FFFFFFxx, not sign-extended.)  AddItem goes to the base's
// exported thunk (deviation (2)).  The previous definition had no `this`
// parameter; the signature now follows the mangled name.
// Symbol: ?InsertSize@CMFCToolBarFontSizeComboBox@@IEAAXH@Z
extern "C" void MS_ABI impl__InsertSize_CMFCToolBarFontSizeComboBox__IEAAXH_Z(void* pThis, int nSize) {
    if (pThis == nullptr) return;
    alignas(CString) unsigned char buf[sizeof(CString)];
    CString* pStr = impl__TwipsToPointString_CMFCToolBarFontSizeComboBox__IEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        pThis, reinterpret_cast<CString*>(buf), nSize);
    impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(
        Combo(pThis), static_cast<const wchar_t*>(*pStr), static_cast<DWORD_PTR>(static_cast<std::uint32_t>(nSize)));
    pStr->~CString();
}

// Retail (RVA 0x169fe0, mfc140u), fully transcribed:
//     if (strFontName.IsEmpty()) return;                          // length at data-0x10
//     CString strText = m_strEdit;                                // +0xd8
//     if (m_pWndCombo != NULL) m_pWndCombo->SendMessage(WM_SETREDRAW, FALSE);   // +0xd0; HWND +0x40
//     CWindowDC dc(NULL);                                         // 0x2a3c20
//     RemoveAllItems();                                           // 0x162b30
//     m_nLogVert = ::GetDeviceCaps(dc.m_hAttribDC, LOGPIXELSY);   // IAT 0x1802c61a8; +0x150
//     ::EnumFontFamilies(dc.m_hDC, strFontName, EnumSizeCallBack, (LPARAM)this);   // IAT 0x1802c6198; 0x16a260
//     if (!SelectItem(strText)) {                                 // 0x163d40
//         m_strEdit = strText;
//         if (m_pWndCombo != NULL) m_pWndCombo->SetWindowText(m_strEdit);      // 0x2a9790
//     }
//     // Synchronize the list with the other combo buttons of the same command:
//     CObList listButtons;
//     if (CMFCToolBar::GetCommandButtons(m_nID, listButtons) > 0) {             // 0x152990
//         for (POSITION posCombo = listButtons.GetHeadPosition(); posCombo != NULL;) {
//             CObject* pObj = listButtons.GetNext(posCombo);
//             if (pObj != NULL && pObj->IsKindOf(RUNTIME_CLASS(CMFCToolBarComboBoxButton))   // direct call to IsKindOf 0x234cf0, class 0x3b1658
//                 && pObj != this) {
//                 CMFCToolBarComboBoxButton* pCombo = (CMFCToolBarComboBoxButton*)pObj;
//                 if (pCombo->m_pWndCombo != NULL) pCombo->m_pWndCombo->SendMessage(WM_SETREDRAW, FALSE);
//                 pCombo->RemoveAllItems();                                   // 0x162b30
//                 POSITION posData = m_lstItemData.GetHeadPosition();         // +0x120
//                 for (POSITION pos = m_lstItems.GetHeadPosition(); pos != NULL && posData != NULL;)   // +0xe8
//                     pCombo->AddItem(m_lstItems.GetNext(pos), m_lstItemData.GetNext(posData));    // vslot 0x1a8 (53)
//                 if (pCombo->m_pWndCombo != NULL) pCombo->m_pWndCombo->SendMessage(WM_SETREDRAW, TRUE);
//             }
//         }
//     }
//     if (m_pWndCombo != NULL) m_pWndCombo->SendMessage(WM_SETREDRAW, TRUE);
//     // ~listButtons (RemoveAll 0x8350), ~dc (0x2a3ce0), ~strText
// Deviations: the CWindowDC is not built as a CDC object; the body takes the
// same DC it would attach (::GetWindowDC(NULL), the call inside CWindowDC's
// ctor 0x2a3c20 -- m_hDC and m_hAttribDC are both that handle) and releases
// it with ::ReleaseDC(NULL, hDC) as its dtor 0x2a3ce0 does.  Where retail
// raises AfxThrowResourceException on a failed GetWindowDC, this proceeds
// with a NULL DC (GetDeviceCaps then yields 0 and EnumFontFamilies adds
// nothing).  The item copy walks this button's side-table items through
// GetCount / GetItem / GetItemData (deviation (3)); AddItem goes to the base
// thunk (deviation (2)).  The previous definition's parameter list was an
// auto-generated placeholder; it now follows the mangled name.
// Symbol: ?RebuildFontSizes@CMFCToolBarFontSizeComboBox@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__RebuildFontSizes_CMFCToolBarFontSizeComboBox__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const CString* pStrFontName) {
    if (pThis == nullptr || pStrFontName == nullptr) return;
    if (pStrFontName->IsEmpty()) return;

    const CString strText = At<CString>(pThis, kOffStrEdit);
    SetRedraw(pThis, FALSE);

    HDC hDC = ::GetWindowDC(nullptr);
    impl__RemoveAllItems_CMFCToolBarComboBoxButton__QEAAXXZ(Combo(pThis));
    Own(pThis)->m_nLogVert = ::GetDeviceCaps(hDC, LOGPIXELSY);
    ::EnumFontFamiliesW(hDC, static_cast<const wchar_t*>(*pStrFontName),
                        reinterpret_cast<FONTENUMPROCW>(
                            &impl__EnumSizeCallBack_CMFCToolBarFontSizeComboBox__KAHPEAUtagLOGFONTW__PEAUtagNEWTEXTMETRICW__HPEAX_Z),
                        reinterpret_cast<LPARAM>(pThis));

    if (!impl__SelectItem_CMFCToolBarComboBoxButton__QEAAHPEB_W_Z(Combo(pThis), static_cast<const wchar_t*>(strText))) {
        CString& strEdit = At<CString>(pThis, kOffStrEdit);
        strEdit = strText;
        if (CComboBox* pWndCombo = At<CComboBox*>(pThis, kOffWndCombo)) {
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pWndCombo, static_cast<const wchar_t*>(strEdit));
        }
    }

    {
        CommandButtonList listButtons(Combo(pThis)->m_nID);
        if (listButtons.Count() > 0) {
            const CRuntimeClass* pComboClass = impl__GetThisClass_CMFCToolBarComboBoxButton__SAPEAUCRuntimeClass__XZ();
            for (ObNode* n = listButtons.First(); n != nullptr; n = n->pNext) {
                CObject* pObj = n->data;
                if (pObj == nullptr || !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, pComboClass)) continue;
                CMFCToolBarComboBoxButton* pCombo = static_cast<CMFCToolBarComboBoxButton*>(pObj);
                if (static_cast<void*>(pCombo) == pThis) continue;

                SetRedraw(pCombo, FALSE);
                impl__RemoveAllItems_CMFCToolBarComboBoxButton__QEAAXXZ(pCombo);
                const INT_PTR nCount = impl__GetCount_CMFCToolBarComboBoxButton__QEBA_JXZ(Combo(pThis));
                for (INT_PTR i = 0; i < nCount; ++i) {
                    const wchar_t* pItem = impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(Combo(pThis), static_cast<int>(i));
                    const DWORD_PTR dwData = impl__GetItemData_CMFCToolBarComboBoxButton__QEBA_KH_Z(Combo(pThis), static_cast<int>(i));
                    impl__AddItem_CMFCToolBarComboBoxButton__UEAA_JPEB_W_K_Z(pCombo, pItem, dwData);
                }
                SetRedraw(pCombo, TRUE);
            }
        }
    }
    SetRedraw(pThis, TRUE);

    if (hDC != nullptr) ::ReleaseDC(nullptr, hDC);
}

// ===========================================================================
// Twips <-> point text
// ===========================================================================

// Retail (RVA 0x16a350, mfc140u), fully transcribed:
//     CString str;                                                // the nil string
//     if (nTwips >= 0) {
//         nTwips = (nTwips + 5) / 10;                             // round to the nearest half point
//         if ((nTwips & 1) == 0) str.Format(L"%ld", nTwips / 2);  // "%ld" at 0x3402f0
//         else str.Format(L"%.1f", (double)((float)nTwips * 0.5f));   // "%.1f" at 0x342d50; 0.5f at 0x34feb0
//     }
//     return str;                                                 // hidden return pointer in RDX
// Symbol: ?TwipsToPointString@CMFCToolBarFontSizeComboBox@@IEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" CString* MS_ABI impl__TwipsToPointString_CMFCToolBarFontSizeComboBox__IEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    void* pThis, CString* pRet, int nTwips) {
    (void)pThis;   // retail does not read `this`
    if (pRet == nullptr) return nullptr;
    CString* str = new (pRet) CString();
    if (nTwips >= 0) {
        // `lea 0x5(%rdi)` wraps at INT_MAX; do the add unsigned so that wrap is
        // reproduced without signed-overflow UB, then divide signed as retail's
        // imul-0x66666667 sequence does.
        const int n = static_cast<int>(static_cast<unsigned>(nTwips) + 5u) / 10;
        if ((n & 1) == 0) {
            str->Format(L"%ld", static_cast<long>(n / 2));
        } else {
            str->Format(L"%.1f", static_cast<double>(static_cast<float>(n) * 0.5f));
        }
    }
    return str;
}

// Retail (RVA 0x16a400, mfc140u), fully transcribed:
//     SetText(TwipsToPointString(nSize));                         // 0x16a350; 0x164450
// The previous definition had no `this` parameter; the signature now follows
// the mangled name.
// Symbol: ?SetTwipSize@CMFCToolBarFontSizeComboBox@@QEAAXH@Z
extern "C" void MS_ABI impl__SetTwipSize_CMFCToolBarFontSizeComboBox__QEAAXH_Z(void* pThis, int nSize) {
    if (pThis == nullptr) return;
    alignas(CString) unsigned char buf[sizeof(CString)];
    CString* pStr = impl__TwipsToPointString_CMFCToolBarFontSizeComboBox__IEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        pThis, reinterpret_cast<CString*>(buf), nSize);
    impl__SetText_CMFCToolBarComboBoxButton__QEAAXPEB_W_Z(Combo(pThis), static_cast<const wchar_t*>(*pStr));
    pStr->~CString();
}

// Retail (RVA 0x16a450, mfc140u), fully transcribed:
//     CString str;
//     LPCTSTR lpszItem = GetItem(-1);                             // 0x162a00 (the current selection)
//     if (lpszItem == NULL) str = m_strEdit; else str = lpszItem; // +0xd8; SetString(p, wcslen(p))
//     if (m_lstItems.Find(m_strEdit) == NULL) str = m_strEdit;    // +0xe0; CStringList::Find 0x2329e0
//     LPCTSTR p = str;
//     while (*p == L' ' || *p == L'\t') p++;
//     if (*p == 0) return -1;                                     // empty
//     double d = wcstod(p, &p);                                   // IAT 0x1802c7478 = wcstod
//     while (*p == L' ' || *p == L'\t') p++;
//     if (*p != 0) return -2;                                     // trailing garbage
//     int n = (int)(d * 20.0);                                    // cvttsd2si; 20.0 at 0x350050
//     return (0.0 > d) ? 0 : n;                                   // comisd / cmova
// Retail calls GetItem(-1) a second time to copy the text (a const,
// non-virtual call, so one call here gives the same result).  The
// m_lstItems.Find step walks the side-table items (deviation (3)).  The
// (int) conversion reproduces cvttsd2si's result for an out-of-range or NaN
// product (0x80000000), which a plain C++ cast leaves undefined; a NaN d is
// not "less than 0.0", so it also yields 0x80000000, as retail's cmova does.
// A NULL `this` returns -2 (an OpenMFC guard; retail would fault).
// Symbol: ?GetTwipSize@CMFCToolBarFontSizeComboBox@@QEBAHXZ
extern "C" int MS_ABI impl__GetTwipSize_CMFCToolBarFontSizeComboBox__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return -2;
    const CString& strEdit = At<CString>(pThis, kOffStrEdit);
    CString str;
    const wchar_t* lpszItem = impl__GetItem_CMFCToolBarComboBoxButton__QEBAPEB_WH_Z(Combo(pThis), -1);
    if (lpszItem == nullptr) {
        str = strEdit;
    } else {
        str = lpszItem;
    }
    if (!ItemsContain(pThis, static_cast<const wchar_t*>(strEdit))) {
        str = strEdit;
    }

    const wchar_t* p = static_cast<const wchar_t*>(str);
    while (*p == L' ' || *p == L'\t') p++;
    if (*p == 0) return -1;
    wchar_t* pEnd = nullptr;
    const double d = std::wcstod(p, &pEnd);
    p = pEnd;
    while (*p == L' ' || *p == L'\t') p++;
    if (*p != 0) return -2;
    const double t = d * 20.0;
    int n;
    if (t != t || t >= 2147483648.0 || t <= -2147483649.0) {
        n = static_cast<int>(0x80000000u);
    } else {
        n = static_cast<int>(t);
    }
    return (0.0 > d) ? 0 : n;
}
