// CMFCToolBarsListPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>
#include <new>

// ===========================================================================
// CMFCToolBarsListPropertyPage -- the "Toolbars" page of the feature-pack
// Customize dialog (afxtoolbarslistpropertypage.h, derives CPropertyPage).
//
// Bodies below were transcribed from the retail mfc140u.dll disassembly (the
// method described in the header of core/ole/COleControl.cpp).  Every RVA in
// this file is mfc140u unless it says otherwise; each was resolved from the
// export's ordinal (mfc_complete_ordinal_mapping.json) through mfc140u.dll's
// export address table:
//
//   ctor                   0x17bf10      OnSelchangeToolbarList 0x17c410
//   dtor                   0x17c030      OnDblClkToolBarList    0x17c550
//   CreateObject           0x17b690      ShowToolBar            0x17c600
//   DoDataExchange         0x17c0a0      OnResetToolbar         0x17c6a0
//   OnInitDialog           0x17c160      OnResetAllToolbars     0x17c7b0
//   OnDeleteToolbar        0x17c940      OnNewToolbar           0x17cae0
//   OnRenameToolbar        0x17cc70      OnCommand              0x17cea0
//   OnTextLabels           0x17cfa0
//
// Import slots named below were resolved with iatu.py against mfc140u.dll:
// 0x1802c7120 = USER32!SendMessageW, 0x1802c7230 = USER32!MessageBeep,
// 0x1802c72d8 = USER32!GetParent, 0x1802c7130 = USER32!RedrawWindow.
// 0x1802c7b30 is not an import; it is the
// indirect-call target every virtual call in these bodies goes through.
//
// The class is not declared in OpenMFC's public headers with its retail
// layout, so S_ToolbarsListPage below is the only place that layout lives.
// It is pinned by the constructor (0x17bf10), whose stores are listed on the
// struct, and by CreateObject (0x17b690), which passes 0x718 to operator new.
// The member names are afxtoolbarslistpropertypage.h's; the dialog control
// ids DoDataExchange binds (afxribbonres.h) confirm the name-to-offset map.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores the class vftable at +0x00 and the
//      CButton vftable into each button member after ??0CWnd@@QEAA@XZ.
//      OpenMFC has no MSVC-layout vtable for this class, so +0x00 keeps what
//      the CPropertyPage constructor thunk installs and the buttons are built
//      with the exported CWnd constructor alone (the pattern of
//      featurepack/toolbar/CMFCToolBarButtonCustomizeDialog.cpp).  Related
//      consequence: the UpdateData calls below go to the exported
//      CWnd::UpdateData thunk (core/window/Thunks.cpp), whose C++ body
//      (CWnd::UpdateData, core/window/CWnd.cpp) does NOT call DoDataExchange
//      at all -- it only walks the child windows' captions through
//      UpdateDialogControls.  Neither this class's DoDataExchange nor any
//      other runs, so the DDX_Check transfer of m_bTextLabels that retail
//      performs through UpdateData does not happen today, in either
//      direction.
//
//  (2) Virtual calls on a CMFCToolBar.  Retail dispatches through the
//      toolbar's own vftable.  An OpenMFC toolbar carries a mingw vtable (or,
//      for a client-derived one, an MSVC vtable laid out from OpenMFC's
//      header), neither of which has retail's slot numbering, so every such
//      call is devirtualised to the body retail's CMFCToolBar vftable holds
//      in that slot: the exported CMFCToolBar / CBasePane thunk where there
//      is one, the afxtoolbar.h inline body otherwise.  Overrides are
//      therefore not reached -- in particular CMFCMenuBar's CanBeClosed /
//      CanBeRestored / AllowChangeTextLabels / RestoreOriginalState
//      (afxmenubar.h), CMFCColorBar's AllowChangeTextLabels (afxcolorbar.h)
//      and CMFCOutlookBarPane's CanBeRestored / RestoreOriginalState
//      (afxoutlookbarpane.h).
//
//  (3) m_wndToolbarList.  Its constructor and destructor are the sibling
//      thunks in featurepack/customize/CMFCToolBarsListCheckBox.cpp, which
//      are still placeholders that touch nothing.  The page constructor
//      zero-fills the member's 0x118 bytes before calling that constructor so
//      its m_hWnd (+0x40) reads NULL instead of whatever the allocation held;
//      the handlers' ::SendMessage calls on a NULL HWND then fail with 0.
//      Check state goes through the exported CCheckListBox::GetCheck /
//      SetCheck thunks (core/controls/CCheckListBox.cpp), which key it by the
//      member's address in a side table.  Only CCheckListBox's own C++
//      destructor / Create erase that table's entry, and neither runs on this
//      member (its destructor thunk is the placeholder too), so check states
//      recorded by these handlers outlive the page.
//
//  (4) `this` / pointer checks.  Retail dereferences m_pSelectedToolbar and
//      the list-box item data unchecked where it has not tested them; a NULL
//      pointer is skipped here and each site says so.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the mangled names; the file that
// defines each is named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                             // detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);                               // core/dialog/RuntimeClasses.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                                 // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                                              // core/controls/CtorDtorPlacement.cpp
// featurepack/customize/CMFCToolBarsListCheckBox.cpp -- both still placeholders
// (the destructor's definition there even drops `this`; extern "C" linkage makes
// the mismatch harmless, and its body does nothing).  Deviation (3).
extern "C" void* MS_ABI impl___0CMFCToolBarsListCheckBox__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl___1CMFCToolBarsListCheckBox__UEAA_XZ(void* pThis);

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(void* pDX, int nIDC, int* pv);             // core/runtime/DdxExchange.cpp
extern "C" int   MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);            // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                      // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                             // core/window/CWnd.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);                // core/window/Thunks.cpp
extern "C" void  MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString);   // core/window/CWnd.cpp

extern "C" int   MS_ABI impl__GetCheck_CCheckListBox__QEAAHH_Z(CCheckListBox* pThis, int nIndex);                 // core/controls/CCheckListBox.cpp
extern "C" void  MS_ABI impl__SetCheck_CCheckListBox__QEAAXHH_Z(CCheckListBox* pThis, int nIndex, int nCheck);    // core/controls/CCheckListBox.cpp

extern "C" int   MS_ABI impl__CanBeRestored_CMFCToolBar__UEBAHXZ(const CMFCToolBar* pThis);                      // featurepack/toolbar/CMFCToolBar.cpp
extern "C" int   MS_ABI impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(CMFCToolBar* pThis);                     // featurepack/toolbar/CMFCToolBar.cpp
extern "C" int   MS_ABI impl__IsUserDefined_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis);                      // featurepack/toolbar/CMFCToolBar.cpp
extern "C" void  MS_ABI impl__EnableTextLabels_CMFCToolBar__QEAAXH_Z(CMFCToolBar* pThis, int bEnable);          // featurepack/toolbar/CMFCToolBar.cpp
extern "C" void  MS_ABI impl__ShowPane_CBasePane__UEAAXHHH_Z(CBasePane* pThis, int bShow, int bDelay, int bActivate);   // featurepack/docking/CBasePane.cpp

extern "C" void* MS_ABI impl__GetCmdMgr__YAPEAVCCommandManager__XZ();                                          // featurepack/CMFC_misc_stubs.cpp
// featurepack/customize/CCommandManager.cpp -- both still placeholders whose
// definitions drop `this` (and whose bodies do nothing); declared here with the
// signatures the mangled names describe.
extern "C" void  MS_ABI impl__ClearAllCmdImages_CCommandManager__QEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__SetCmdImage_CCommandManager__QEAAXIHH_Z(void* pThis, unsigned int uiCmd, int iImage, int bUserImage);
// ?m_DefaultImages@CMFCToolBar@@1V?$CMap@IIHH@@A, defined in featurepack/toolbar/
// CMFCToolBar.cpp as the retail 56-byte CMap<UINT,UINT,int,int> image; read
// here as bytes, as featurepack/toolbar/CMFCToolBarButtonCustomizeDialog.cpp does.
extern "C" unsigned char impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A[56];

extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);          // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" void  MS_ABI impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(CString* pThis, const wchar_t* pszFormat, ...);   // core/collections/CStringT.cpp
extern "C" int   MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, unsigned int nType, unsigned int nIDHelp);   // core/collections/Globals.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                              // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowOleException__YAXJ_Z(long sc);                                             // detail/MfcExceptionsSupport.cpp

extern "C" unsigned int impl__AFX_WM_DELETETOOLBAR__3IA;                                                        // core/runtime/Globals.cpp

// This file's own thunks that earlier bodies call.
extern "C" void MS_ABI impl__OnSelchangeToolbarList_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// Retail layout (mfc140u).  Every offset is a store in the constructor
// (0x17bf10): CPropertyPage at +0 (??0CPropertyPage@@QEAA@IIK@Z), ??0CWnd@@
// on +0x158/+0x240/+0x328/+0x410/+0x4f8, ??0CMFCToolBarsListCheckBox@@ on
// +0x5e0, then 0 -> +0x708, pParentFrame -> +0x710, 0 -> +0x6f8, 0 -> +0x700.
// ---------------------------------------------------------------------------
struct alignas(8) S_ToolbarsListPage {
    unsigned char base[0x158];                 // CPropertyPage (OpenMFC sizeof == 0x158, asserted below)
    unsigned char m_wndTextLabels[0xe8];       // +0x158 CButton, IDC_AFXBARRES_TEXT_LABELS    16612
    unsigned char m_bntRenameToolbar[0xe8];    // +0x240 CButton, IDC_AFXBARRES_RENAME_TOOLBAR 16616 (retail's spelling)
    unsigned char m_btnNewToolbar[0xe8];       // +0x328 CButton, IDC_AFXBARRES_NEW_TOOLBAR    16615
    unsigned char m_btnDelete[0xe8];           // +0x410 CButton, IDC_AFXBARRES_DELETE_TOOLBAR 16617
    unsigned char m_btnReset[0xe8];            // +0x4f8 CButton, IDC_AFXBARRES_RESET          16613
    unsigned char m_wndToolbarList[0x118];     // +0x5e0 CMFCToolBarsListCheckBox (retail 280 bytes), IDC_AFXBARRES_TOOLBAR_LIST 16611
    int           m_bTextLabels;               // +0x6f8
    int           _pad6fc;                     // +0x6fc
    CMFCToolBar*  m_pSelectedToolbar;          // +0x700
    int           m_bUserDefinedToolbars;      // +0x708
    int           _pad70c;                     // +0x70c
    CFrameWnd*    m_pParentFrame;              // +0x710
};
static_assert(sizeof(CPropertyPage) == 0x158, "CPropertyPage base is 0x158 bytes in retail (first member at +0x158)");
static_assert(sizeof(CButton) == 0xe8, "retail CButton is 0xe8 bytes (members 0xe8 apart)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_wndToolbarList.m_hWnd at +0x5e0+0x40");
static_assert(offsetof(S_ToolbarsListPage, m_wndTextLabels) == 0x158, "m_wndTextLabels");
static_assert(offsetof(S_ToolbarsListPage, m_bntRenameToolbar) == 0x240, "m_bntRenameToolbar");
static_assert(offsetof(S_ToolbarsListPage, m_btnNewToolbar) == 0x328, "m_btnNewToolbar");
static_assert(offsetof(S_ToolbarsListPage, m_btnDelete) == 0x410, "m_btnDelete");
static_assert(offsetof(S_ToolbarsListPage, m_btnReset) == 0x4f8, "m_btnReset");
static_assert(offsetof(S_ToolbarsListPage, m_wndToolbarList) == 0x5e0, "m_wndToolbarList");
static_assert(offsetof(S_ToolbarsListPage, m_bTextLabels) == 0x6f8, "m_bTextLabels");
static_assert(offsetof(S_ToolbarsListPage, m_pSelectedToolbar) == 0x700, "m_pSelectedToolbar");
static_assert(offsetof(S_ToolbarsListPage, m_bUserDefinedToolbars) == 0x708, "m_bUserDefinedToolbars (CMFCToolBarsCustomizeDialog::EnableUserDefinedToolbars writes it)");
static_assert(offsetof(S_ToolbarsListPage, m_pParentFrame) == 0x710, "m_pParentFrame");
static_assert(sizeof(S_ToolbarsListPage) == 0x718, "retail sizeof(CMFCToolBarsListPropertyPage) == 0x718 (CreateObject's operator new)");

// Resource ids (afxribbonres.h) as the retail bodies pass them.
constexpr unsigned int kIddPropPage2           = 16105;   // 0x3ee9 IDD_AFXBARRES_PROPPAGE2
constexpr int          kIdcToolbarList         = 16611;   // 0x40e3
constexpr int          kIdcTextLabels          = 16612;   // 0x40e4
constexpr int          kIdcReset               = 16613;   // 0x40e5
constexpr int          kIdcNewToolbar          = 16615;   // 0x40e7
constexpr int          kIdcRenameToolbar       = 16616;   // 0x40e8
constexpr int          kIdcDeleteToolbar       = 16617;   // 0x40e9
constexpr unsigned int kIdsDeleteToolbarFmt    = 16109;   // 0x3eed IDS_AFXBARRES_DELETE_TOOLBAR_FMT
constexpr unsigned int kIdsResetToolbarFmt     = 16111;   // 0x3eef IDS_AFXBARRES_RESET_TOOLBAR_FMT
constexpr unsigned int kIdsResetAllToolbars    = 16112;   // 0x3ef0 IDS_AFXBARRES_RESET_ALL_TOOLBARS

constexpr unsigned int kClbnChkChange = 40;                // CLBN_CHKCHANGE, the `cmp $0x28,%ax` in OnCommand
constexpr unsigned int kMbYesNoQuestion = MB_YESNO | MB_ICONQUESTION;   // the 0x24 every AfxMessageBox here passes
constexpr long kEFail = static_cast<long>(0x80004005);

inline S_ToolbarsListPage* D(void* pThis) { return static_cast<S_ToolbarsListPage*>(pThis); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline CCheckListBox* CheckList(S_ToolbarsListPage* d) { return reinterpret_cast<CCheckListBox*>(d->m_wndToolbarList); }

// m_wndToolbarList.m_hWnd (+0x5e0 + 0x40, the `0x620(%rcx)` reads).  Read as
// bytes because the member is not a constructed OpenMFC object (deviation 3).
inline HWND ListHwnd(const S_ToolbarsListPage* d) {
    HWND h = nullptr;
    std::memcpy(&h, d->m_wndToolbarList + offsetof(CWnd, m_hWnd), sizeof h);
    return h;
}

// The inline CListBox accessors, as retail spells them (::SendMessageW on m_hWnd).
inline int ListGetCurSel(const S_ToolbarsListPage* d) {
    return static_cast<int>(::SendMessageW(ListHwnd(d), LB_GETCURSEL, 0, 0));
}
inline int ListGetCount(const S_ToolbarsListPage* d) {
    return static_cast<int>(::SendMessageW(ListHwnd(d), LB_GETCOUNT, 0, 0));
}
inline CMFCToolBar* ListGetToolbar(const S_ToolbarsListPage* d, int nIndex) {
    return reinterpret_cast<CMFCToolBar*>(::SendMessageW(ListHwnd(d), LB_GETITEMDATA, static_cast<WPARAM>(nIndex), 0));
}

// CMFCToolBar::CanBeClosed, retail vslot 116 (+0x3a0).  The CMFCToolBar
// vftable holds the unexported mfc140u 0x23a60 there:
//     xor eax,eax ; cmp [rcx+0x10c4],eax ; sete al ; ret
// i.e. afxtoolbar.h's inline `return !m_bPermament;` (deviation 2).
inline BOOL ToolbarCanBeClosed(const CMFCToolBar* p) { return p->m_bPermament == 0 ? TRUE : FALSE; }

// CStringT::Format(UINT nFormatID, ...) as its retail body (RVA 0x66db0) runs
// it: load the format string (AfxFindStringResourceHandle, then
// LoadString(HINSTANCE, UINT)), AtlThrow(E_FAIL) on either failure (the shim
// at 0x333c, which reaches AfxThrowOleException 0x25f2c0 for any hr but
// E_OUTOFMEMORY), then FormatV.  Composed here from the exported LoadString
// and Format(PCXSTR, ...) thunks of core/collections/CStringT.cpp, which take
// OpenMFC's CString.  Every caller in this file formats one string argument.
bool FormatFromResource(CString& str, unsigned int nFormatID, const wchar_t* pszArg) {
    CString strFormat;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nFormatID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&strFormat, hInst, nFormatID)) {
        impl__AfxThrowOleException__YAXJ_Z(kEFail);
        return false;
    }
    impl__Format___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAXPEB_WZZ(&str, strFormat.GetString(), pszArg);
    return true;
}

// Retail's `CString str; ENSURE(str.LoadString(nID))` (AfxFindStringResourceHandle
// + LoadString(HINSTANCE, UINT), AfxThrowInvalidArgException 0x227720 on failure).
bool EnsureLoadString(CString& str, unsigned int nID) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nID)) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return false;
    }
    return true;
}

template <typename T>
T ReadAt(const void* base, std::size_t off) {
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof v);
    return v;
}

} // namespace

// Retail (RVA 0x17bf10), fully transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE2 /*0x3ee9*/, 0, 0x68);
//     <store class vftable>
//     m_wndTextLabels, m_bntRenameToolbar, m_btnNewToolbar, m_btnDelete,
//     m_btnReset: CWnd::CWnd() + CButton vftable, in that order
//     CMFCToolBarsListCheckBox::CMFCToolBarsListCheckBox(&m_wndToolbarList);   // 0x17b1d0
//     m_bUserDefinedToolbars = FALSE;  m_pParentFrame = pParentFrame;
//     m_bTextLabels = FALSE;  m_pSelectedToolbar = NULL;
// DEVIATIONS: the vftable stores are deviation (1); the zero-fill of
// m_wndToolbarList before its (placeholder) constructor is deviation (3).
// This export was not on this file's work list (its previous body only
// returned pThis), but the destructor and every handler depend on the members
// being constructed, so it is implemented with them.
// Symbol: ??0CMFCToolBarsListPropertyPage@@QEAA@PEAVCFrameWnd@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarsListPropertyPage__QEAA_PEAVCFrameWnd___Z(void* pThis, void* pFrameWnd) {
    if (pThis == nullptr) return nullptr;
    S_ToolbarsListPage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage2, 0, 0x68);
    impl___0CWnd__QEAA_XZ(d->m_wndTextLabels);
    impl___0CWnd__QEAA_XZ(d->m_bntRenameToolbar);
    impl___0CWnd__QEAA_XZ(d->m_btnNewToolbar);
    impl___0CWnd__QEAA_XZ(d->m_btnDelete);
    impl___0CWnd__QEAA_XZ(d->m_btnReset);
    std::memset(d->m_wndToolbarList, 0, sizeof d->m_wndToolbarList);   // deviation (3)
    impl___0CMFCToolBarsListCheckBox__QEAA_XZ(d->m_wndToolbarList);
    d->m_bUserDefinedToolbars = FALSE;
    d->m_pParentFrame = static_cast<CFrameWnd*>(pFrameWnd);
    d->m_bTextLabels = FALSE;
    d->m_pSelectedToolbar = nullptr;
    return pThis;
}

// Retail (RVA 0x17c030), fully transcribed:
//     <store class vftable>
//     m_wndToolbarList.~CMFCToolBarsListCheckBox();                        // 0x17b280
//     m_btnReset.~CButton();  m_btnDelete.~CButton();  m_btnNewToolbar.~CButton();
//     m_bntRenameToolbar.~CButton();  m_wndTextLabels.~CButton();          // 0x293be0 each
//     CPropertyPage::~CPropertyPage();                                     // tail jump 0x215f40
// The vftable store is deviation (1); the list-box destructor is the
// placeholder of deviation (3), paired with its placeholder constructor.
// Symbol: ??1CMFCToolBarsListPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsListPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    impl___1CMFCToolBarsListCheckBox__UEAA_XZ(d->m_wndToolbarList);
    impl___1CButton__UEAA_XZ(d->m_btnReset);
    impl___1CButton__UEAA_XZ(d->m_btnDelete);
    impl___1CButton__UEAA_XZ(d->m_btnNewToolbar);
    impl___1CButton__UEAA_XZ(d->m_bntRenameToolbar);
    impl___1CButton__UEAA_XZ(d->m_wndTextLabels);
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (RVA 0x17b690), fully transcribed:
//     void* p = operator new(0x718);                                       // 0x27f0
//     return p ? new(p) CMFCToolBarsListPropertyPage(NULL) : NULL;         // 0x17bf10
// Symbol: ?CreateObject@CMFCToolBarsListPropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCToolBarsListPropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_ToolbarsListPage));
    if (p == nullptr) return nullptr;
    return impl___0CMFCToolBarsListPropertyPage__QEAA_PEAVCFrameWnd___Z(p, nullptr);
}

// Retail (RVA 0x17c0a0), fully transcribed:
//     DDX_Control(pDX, IDC_AFXBARRES_TEXT_LABELS,    m_wndTextLabels);     // +0x158
//     DDX_Control(pDX, IDC_AFXBARRES_RENAME_TOOLBAR, m_bntRenameToolbar);  // +0x240
//     DDX_Control(pDX, IDC_AFXBARRES_NEW_TOOLBAR,    m_btnNewToolbar);     // +0x328
//     DDX_Control(pDX, IDC_AFXBARRES_DELETE_TOOLBAR, m_btnDelete);         // +0x410
//     DDX_Control(pDX, IDC_AFXBARRES_RESET,          m_btnReset);          // +0x4f8
//     DDX_Control(pDX, IDC_AFXBARRES_TOOLBAR_LIST,   m_wndToolbarList);    // +0x5e0
//     DDX_Check  (pDX, IDC_AFXBARRES_TEXT_LABELS,    m_bTextLabels);       // +0x6f8, tail jump
// Symbol: ?DoDataExchange@CMFCToolBarsListPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarsListPropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcTextLabels, d->m_wndTextLabels);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcRenameToolbar, d->m_bntRenameToolbar);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcNewToolbar, d->m_btnNewToolbar);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcDeleteToolbar, d->m_btnDelete);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcReset, d->m_btnReset);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcToolbarList, d->m_wndToolbarList);
    impl__DDX_Check__YAXPEAVCDataExchange__HAEAH_Z(pDX, kIdcTextLabels, &d->m_bTextLabels);
}

// Retail (RVA 0x17cea0), fully transcribed:
//     if (HIWORD(wParam) == CLBN_CHKCHANGE && LOWORD(wParam) == IDC_AFXBARRES_TOOLBAR_LIST) {
//         int iSel = m_wndToolbarList.GetCurSel();                         // SendMessageW LB_GETCURSEL
//         if (iSel != LB_ERR) {
//             CMFCToolBar* pToolbar = (CMFCToolBar*)m_wndToolbarList.GetItemData(iSel);   // LB_GETITEMDATA
//             if (pToolbar->CanBeClosed())                                 // vslot 116 (+0x3a0)
//                 pToolbar->ShowPane(m_wndToolbarList.GetCheck(iSel), FALSE, TRUE);   // vslot 139 (+0x458), GetCheck 0x2978a0
//             else if (m_wndToolbarList.GetCheck(iSel) == 0) {
//                 m_wndToolbarList.SetCheck(iSel, 1);                      // 0x2977c0
//                 MessageBeep((UINT)-1);
//             }
//         }
//     }
//     return CWnd::OnCommand(wParam, lParam);                              // tail jump
// Slot 139 of the CMFCToolBar vftable is ?ShowPane@CBasePane@@ (0xbf90), so
// the exported CBasePane thunk is the body retail reaches for a CMFCToolBar
// (deviation 2, as is the CanBeClosed read).  Retail dereferences the item
// data unchecked; a NULL item is skipped here (deviation 4).
// Symbol: ?OnCommand@CMFCToolBarsListPropertyPage@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CMFCToolBarsListPropertyPage__MEAAH_K_J_Z(void* pThis, WPARAM wParam, LPARAM lParam) {
    if (pThis == nullptr) return FALSE;
    S_ToolbarsListPage* d = D(pThis);
    if (HIWORD(wParam) == kClbnChkChange && LOWORD(wParam) == static_cast<WORD>(kIdcToolbarList)) {
        const int iSel = ListGetCurSel(d);
        if (iSel != LB_ERR) {
            CMFCToolBar* pToolbar = ListGetToolbar(d, iSel);
            if (pToolbar != nullptr) {   // deviation (4)
                if (ToolbarCanBeClosed(pToolbar)) {
                    impl__ShowPane_CBasePane__UEAAXHHH_Z(pToolbar, impl__GetCheck_CCheckListBox__QEAAHH_Z(CheckList(d), iSel), FALSE, TRUE);
                } else if (impl__GetCheck_CCheckListBox__QEAAHH_Z(CheckList(d), iSel) == 0) {
                    impl__SetCheck_CCheckListBox__QEAAXHH_Z(CheckList(d), iSel, 1);
                    ::MessageBeep(static_cast<UINT>(-1));
                }
            }
        }
    }
    return impl__OnCommand_CWnd__MEAAH_K_J_Z(Wnd(pThis), wParam, lParam);
}

// Retail (RVA 0x17c550), fully transcribed:
//     int iSel = m_wndToolbarList.GetCurSel();
//     if (iSel != LB_ERR) {
//         m_pSelectedToolbar = (CMFCToolBar*)m_wndToolbarList.GetItemData(iSel);
//         if (m_pSelectedToolbar->CanBeClosed())                           // vslot 116
//             m_wndToolbarList.SetCheck(iSel, !m_wndToolbarList.GetCheck(iSel));
//         else
//             MessageBeep((UINT)-1);
//     }
//     OnSelchangeToolbarList();                                            // tail jump 0x17c410, on both paths
// CanBeClosed is deviation (2); a NULL item is not dereferenced (deviation 4:
// the check toggle and beep are skipped for it, OnSelchangeToolbarList still runs).
// Symbol: ?OnDblClkToolBarList@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnDblClkToolBarList_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    const int iSel = ListGetCurSel(d);
    if (iSel != LB_ERR) {
        d->m_pSelectedToolbar = ListGetToolbar(d, iSel);
        if (d->m_pSelectedToolbar != nullptr) {   // deviation (4)
            if (ToolbarCanBeClosed(d->m_pSelectedToolbar)) {
                const int nCheck = impl__GetCheck_CCheckListBox__QEAAHH_Z(CheckList(d), iSel);
                impl__SetCheck_CCheckListBox__QEAAXHH_Z(CheckList(d), iSel, nCheck == 0 ? 1 : 0);
            } else {
                ::MessageBeep(static_cast<UINT>(-1));
            }
        }
    }
    impl__OnSelchangeToolbarList_CMFCToolBarsListPropertyPage__IEAAXXZ(pThis);
}

// Retail (RVA 0x17c160) -- NOT IMPLEMENTED, left a stub.  Decoded:
//     CDialog::OnInitDialog();                                             // 0x208d10
//     if (!m_bUserDefinedToolbars) {
//         m_btnNewToolbar.EnableWindow(FALSE);  m_btnNewToolbar.ShowWindow(SW_HIDE);
//         m_btnDelete.ShowWindow(SW_HIDE);  m_bntRenameToolbar.ShowWindow(SW_HIDE);
//     }
//     for (each CMFCToolBar* pToolBar in the private all-toolbars CObList) {   // m_pNodeHead read at 0x3b2098
//         ENSURE(pToolBar != NULL);
//         if (CWnd::FromHandlePermanent(pToolBar->m_hWnd) != NULL &&
//             !pToolBar->IsKindOf(RUNTIME_CLASS(CMFCDropDownToolBar)) &&
//             pToolBar->GetTopLevelFrame() == m_pParentFrame->GetTopLevelFrame() &&   // called in that order
//             pToolBar->AllowShowOnList() /*vslot 254, +0x7f0*/ && !pToolBar->m_bMasked /*+0x10c0*/) {
//             CString strName;  pToolBar->GetWindowText(strName);
//             if (strName.IsEmpty()) ENSURE(strName.LoadString(IDS_AFXBARRES_UNTITLED_TOOLBAR /*16104*/));
//             int i = m_wndToolbarList.AddString(strName);  m_wndToolbarList.SetItemData(i, pToolBar);
//             if (pToolBar->GetStyle() & WS_VISIBLE) m_wndToolbarList.SetCheck(i, 1);
//             m_wndToolbarList.EnableCheck(i, pToolBar->CanBeClosed() /*vslot 116*/);   // 0x17b480
//         }
//     }
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//     ENSURE(pParent && pParent->IsKindOf(RUNTIME_CLASS(CMFCToolBarsCustomizeDialog)));
//     if (!(pParent->m_uiFlags /*+0x284*/ & AFX_CUSTOMIZE_TEXT_LABELS)) m_wndTextLabels.ShowWindow(SW_HIDE);
//     if (m_wndToolbarList.GetCount() > 0) { m_wndToolbarList.SetCurSel(0); OnSelchangeToolbarList(); }
//     return TRUE;
// Left unimplemented because (a) the ENSURE on the parent's runtime class
// would throw for every customize dialog OpenMFC builds: its constructor
// thunk (CMFCToolBarsCustomizeDialog.cpp) installs CPropertySheet's vtable, so
// IsKindOf(RUNTIME_CLASS(CMFCToolBarsCustomizeDialog)) is FALSE there; and
// (b) CMFCToolBarsListCheckBox::EnableCheck is still a placeholder whose
// definition drops `this` (featurepack/customize/CMFCToolBarsListCheckBox.cpp).
// Symbol: ?OnInitDialog@CMFCToolBarsListPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsListPropertyPage__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Retail (RVA 0x17cae0) -- NOT IMPLEMENTED, left a stub.  Decoded:
//     CMFCToolBarNameDialog dlg(this);                                     // on the stack; ctor 0x176470
//     if (dlg.DoModal() == IDOK) {                                         // CDialog::DoModal
//         CString strName = dlg.m_strToolbarName;                          // dlg + 0x218
//         CFrameWnd* pParentFrame = GetParentFrame();
//         if (pParentFrame == NULL) MessageBeep(MB_ICONASTERISK);
//         else if (CMFCToolBar* pNew = (CMFCToolBar*)::SendMessageW(pParentFrame->m_hWnd,
//                      AFX_WM_CREATETOOLBAR, 0, (LPARAM)(LPCTSTR)strName)) {
//             int i = m_wndToolbarList.AddString(strName);
//             m_wndToolbarList.SetItemData(i, (DWORD_PTR)pNew);
//             m_wndToolbarList.SetCheck(i, 1);
//             m_wndToolbarList.SetCurSel(i);  m_wndToolbarList.SetTopIndex(i);
//             OnSelchangeToolbarList();
//         }
//     }
// Left unimplemented because every CMFCToolBarNameDialog export in
// featurepack/toolbar/CMFCToolBarNameDialog.cpp is still a placeholder (its
// constructor builds nothing), so DoModal would run on an unconstructed dialog.
// Symbol: ?OnNewToolbar@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnNewToolbar_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x17cc70) -- NOT IMPLEMENTED, left a stub.  Decoded:
//     if (m_pSelectedToolbar == NULL) return;
//     CMFCToolBarNameDialog dlg(this);                                     // ctor 0x176470
//     m_pSelectedToolbar->GetWindowText(dlg.m_strToolbarName);             // dlg + 0x218
//     if (dlg.DoModal() == IDOK) {
//         CString strName = dlg.m_strToolbarName;
//         m_pSelectedToolbar->SetWindowText(strName);
//         if (m_pSelectedToolbar->IsFloating()) {                          // vslot 94 (+0x2f0): `return m_bFloating` (+0x10d8)
//             CWnd* pMiniFrame = m_pSelectedToolbar->GetParentMiniFrame(FALSE);   // vslot 140 (+0x460)
//             if (pMiniFrame != NULL) {
//                 pMiniFrame->SetWindowText(strName);
//                 ::RedrawWindow(pMiniFrame->m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_FRAME /*0x401*/);
//             }
//         }
//         m_wndToolbarList.DeleteString(m_wndToolbarList.GetCurSel());
//         int i = m_wndToolbarList.AddString(strName);
//         m_wndToolbarList.SetItemData(i, (DWORD_PTR)m_pSelectedToolbar);
//         if (m_pSelectedToolbar->GetStyle() & WS_VISIBLE) m_wndToolbarList.SetCheck(i, 1);
//         m_wndToolbarList.SetCurSel(i);  m_wndToolbarList.SetTopIndex(i);
//         OnSelchangeToolbarList();
//     }
// Left unimplemented for the same reason as OnNewToolbar: CMFCToolBarNameDialog
// is still a placeholder class in this tree.
// Symbol: ?OnRenameToolbar@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnRenameToolbar_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x17c7b0), fully transcribed:
//     CString str;  ENSURE(str.LoadString(IDS_AFXBARRES_RESET_ALL_TOOLBARS));
//     if (AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION, 0) != IDYES) return;   // 0x1cec10
//     afxCommandManager->ClearAllCmdImages();                              // 0x2f010 on 0x3c1470 (= GetCmdMgr())
//     for (POSITION pos = CMFCToolBar::m_DefaultImages.GetStartPosition(); pos != NULL; ) {   // m_nCount at 0x3b1fa8
//         UINT uiCmdId; int iImage;
//         CMFCToolBar::m_DefaultImages.GetNextAssoc(pos, uiCmdId, iImage);  // unexported 0x15bb58
//         afxCommandManager->SetCmdImage(uiCmdId, iImage, FALSE);           // 0x2ef30
//     }
//     for (int i = 0; i < m_wndToolbarList.GetCount(); i++) {              // LB_GETCOUNT re-read each pass
//         CMFCToolBar* pToolBar = (CMFCToolBar*)m_wndToolbarList.GetItemData(i);
//         if (pToolBar->CanBeRestored())                                   // vslot 227 (+0x718)
//             pToolBar->RestoreOriginalState();                            // vslot 228 (+0x720)
//     }
// The GetNextAssoc walk is reproduced over the map's retail bytes, with the
// layout that non-exported instantiation reads (m_pHashTable +0x08,
// m_nHashTableSize +0x10; CAssoc key +0x00, value +0x04, pNext +0x08,
// nHashValue +0x10), visiting buckets in index order and each chain in link
// order, as GetNextAssoc does.  One difference: GetNextAssoc throws
// (AfxThrowInvalidArgException, the call at 0x15bbdf inside 0x15bb58) when
// m_pHashTable is NULL; a map with a nonzero m_nCount always has a table, so
// the walk here simply skips that impossible state instead of throwing.
// CCommandManager::ClearAllCmdImages /
// SetCmdImage are still placeholders (featurepack/customize/
// CCommandManager.cpp), so today those calls do nothing; nothing in this tree
// populates m_DefaultImages either, so the replay loop does not iterate.
// CanBeRestored / RestoreOriginalState go to the exported CMFCToolBar bodies
// (deviation 2); both thunks NULL-check the item data, which retail does not.
// Symbol: ?OnResetAllToolbars@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetAllToolbars_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);

    CString str;
    if (!EnsureLoadString(str, kIdsResetAllToolbars)) return;
    if (impl__AfxMessageBox__YAHPEB_WII_Z(str.GetString(), kMbYesNoQuestion, 0) != IDYES) return;

    void* pCmdMgr = impl__GetCmdMgr__YAPEAVCCommandManager__XZ();
    impl__ClearAllCmdImages_CCommandManager__QEAAXXZ(pCmdMgr);

    const unsigned char* pMap = impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A;
    if (ReadAt<INT_PTR>(pMap, 0x18) != 0) {   // m_nCount: GetStartPosition() != NULL
        const unsigned char* const* pHashTable = ReadAt<const unsigned char* const*>(pMap, 0x08);
        const UINT nHashTableSize = ReadAt<UINT>(pMap, 0x10);
        if (pHashTable != nullptr) {
            for (UINT nBucket = 0; nBucket < nHashTableSize; ++nBucket) {
                for (const unsigned char* pAssoc = pHashTable[nBucket]; pAssoc != nullptr;
                     pAssoc = ReadAt<const unsigned char*>(pAssoc, 0x08)) {
                    impl__SetCmdImage_CCommandManager__QEAAXIHH_Z(pCmdMgr, ReadAt<UINT>(pAssoc, 0x00), ReadAt<int>(pAssoc, 0x04), FALSE);
                }
            }
        }
    }

    for (int i = 0; i < ListGetCount(d); ++i) {
        CMFCToolBar* pToolBar = ListGetToolbar(d, i);
        if (impl__CanBeRestored_CMFCToolBar__UEBAHXZ(pToolBar)) {
            impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(pToolBar);
        }
    }
}

// Retail (RVA 0x17c6a0), fully transcribed:
//     if (m_pSelectedToolbar == NULL) return;
//     CString strName;  m_pSelectedToolbar->GetWindowText(strName);        // 0x28be00
//     CString strPrompt;  strPrompt.Format(IDS_AFXBARRES_RESET_TOOLBAR_FMT, strName);   // 0x66db0
//     if (AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION, 0) == IDYES)
//         m_pSelectedToolbar->RestoreOriginalState();                      // vslot 228 (+0x720)
// RestoreOriginalState goes to the exported CMFCToolBar body (deviation 2).
// Symbol: ?OnResetToolbar@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnResetToolbar_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    if (d->m_pSelectedToolbar == nullptr) return;

    CString strName;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(d->m_pSelectedToolbar, &strName);
    CString strPrompt;
    if (!FormatFromResource(strPrompt, kIdsResetToolbarFmt, strName.GetString())) return;
    if (impl__AfxMessageBox__YAHPEB_WII_Z(strPrompt.GetString(), kMbYesNoQuestion, 0) == IDYES) {
        impl__RestoreOriginalState_CMFCToolBar__UEAAHXZ(d->m_pSelectedToolbar);
    }
}

// Retail (RVA 0x17c940), fully transcribed:
//     if (m_pSelectedToolbar == NULL) return;
//     CFrameWnd* pParentFrame = GetParentFrame();                          // 0x28e200
//     if (pParentFrame == NULL) { MessageBeep(MB_ICONASTERISK); return; }
//     CString strName;  m_pSelectedToolbar->GetWindowText(strName);
//     CString strPrompt;  strPrompt.Format(IDS_AFXBARRES_DELETE_TOOLBAR_FMT, strName);
//     if (AfxMessageBox(strPrompt, MB_YESNO | MB_ICONQUESTION, 0) != IDYES) return;
//     if (!::SendMessageW(pParentFrame->m_hWnd, AFX_WM_DELETETOOLBAR, 0, (LPARAM)m_pSelectedToolbar)) {
//         MessageBeep(MB_ICONASTERISK);  return;
//     }
//     m_wndToolbarList.DeleteString(m_wndToolbarList.GetCurSel());         // LB_DELETESTRING
//     m_wndToolbarList.SetCurSel(0);                                       // LB_SETCURSEL
//     OnSelchangeToolbarList();                                            // 0x17c410
// Symbol: ?OnDeleteToolbar@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnDeleteToolbar_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    if (d->m_pSelectedToolbar == nullptr) return;

    CFrameWnd* pParentFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(Wnd(pThis));
    if (pParentFrame == nullptr) {
        ::MessageBeep(MB_ICONASTERISK);
        return;
    }

    CString strName;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(d->m_pSelectedToolbar, &strName);
    CString strPrompt;
    if (!FormatFromResource(strPrompt, kIdsDeleteToolbarFmt, strName.GetString())) return;
    if (impl__AfxMessageBox__YAHPEB_WII_Z(strPrompt.GetString(), kMbYesNoQuestion, 0) != IDYES) return;

    if (::SendMessageW(pParentFrame->m_hWnd, impl__AFX_WM_DELETETOOLBAR__3IA, 0,
                       reinterpret_cast<LPARAM>(d->m_pSelectedToolbar)) == 0) {
        ::MessageBeep(MB_ICONASTERISK);
        return;
    }

    const HWND hList = ListHwnd(d);
    // CListBox::DeleteString(UINT): retail zero-extends the index (`mov %eax,%r8d`).
    ::SendMessageW(hList, LB_DELETESTRING, static_cast<WPARAM>(static_cast<UINT>(ListGetCurSel(d))), 0);
    ::SendMessageW(hList, LB_SETCURSEL, 0, 0);
    impl__OnSelchangeToolbarList_CMFCToolBarsListPropertyPage__IEAAXXZ(pThis);
}

// Retail (RVA 0x17c410), fully transcribed:
//     int iSel = m_wndToolbarList.GetCurSel();
//     if (iSel == LB_ERR) {
//         m_pSelectedToolbar = NULL;
//         m_btnReset.EnableWindow(FALSE);  m_btnDelete.EnableWindow(FALSE);
//         m_bntRenameToolbar.EnableWindow(FALSE);  m_wndTextLabels.EnableWindow(FALSE);
//         return;
//     }
//     m_pSelectedToolbar = (CMFCToolBar*)m_wndToolbarList.GetItemData(iSel);
//     m_btnReset.EnableWindow(m_pSelectedToolbar->CanBeRestored());         // vslot 227 (+0x718)
//     m_btnDelete.EnableWindow(m_pSelectedToolbar->IsUserDefined());        // 0x1596a0
//     m_bntRenameToolbar.EnableWindow(m_pSelectedToolbar->IsUserDefined());
//     m_wndTextLabels.EnableWindow(m_pSelectedToolbar->AllowChangeTextLabels());   // vslot 224 (+0x700)
//     m_bTextLabels = m_pSelectedToolbar->m_bTextLabels;                    // +0x10c8 (AreTextLabels)
//     UpdateData(FALSE);                                                   // 0x2910d0
// Slot 224 of the CMFCToolBar vftable is the shared `mov $1,%eax; ret`
// (0x3a60), afxtoolbar.h's inline AllowChangeTextLabels, so it is TRUE here;
// CanBeRestored is the exported CMFCToolBar body (both deviation 2).  Retail
// dereferences the item data unchecked; a NULL item is treated like the
// LB_ERR path here (deviation 4).  The closing UpdateData(FALSE) runs no
// DoDataExchange in OpenMFC (deviation 1), so the check box is not set from
// m_bTextLabels.
// Symbol: ?OnSelchangeToolbarList@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeToolbarList_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    const int iSel = ListGetCurSel(d);
    CMFCToolBar* pToolbar = (iSel == LB_ERR) ? nullptr : ListGetToolbar(d, iSel);
    if (pToolbar == nullptr) {   // iSel == LB_ERR in retail; a NULL item too (deviation 4)
        d->m_pSelectedToolbar = nullptr;
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_btnReset), FALSE);
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_btnDelete), FALSE);
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_bntRenameToolbar), FALSE);
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndTextLabels), FALSE);
        return;
    }
    d->m_pSelectedToolbar = pToolbar;
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_btnReset), impl__CanBeRestored_CMFCToolBar__UEBAHXZ(pToolbar));
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_btnDelete), impl__IsUserDefined_CMFCToolBar__QEBAHXZ(pToolbar));
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_bntRenameToolbar), impl__IsUserDefined_CMFCToolBar__QEBAHXZ(pToolbar));
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndTextLabels), TRUE);   // AllowChangeTextLabels()
    d->m_bTextLabels = pToolbar->m_bTextLabels;
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);
}

// Retail (RVA 0x17cfa0), fully transcribed:
//     UpdateData(TRUE);                                                    // 0x2910d0
//     m_pSelectedToolbar->EnableTextLabels(m_bTextLabels);                  // inlined:
//         // if (!m_bMenuMode /*+0x10ec*/) { m_bTextLabels /*+0x10c8*/ = b; AdjustLayout(); /*vslot 133, +0x428*/ }
// The inlined body is exactly the exported ?EnableTextLabels@CMFCToolBar@@
// (featurepack/toolbar/CMFCToolBar.cpp), which is called here; it NULL-checks
// the bar, which retail does not.  OpenMFC's CWnd::UpdateData runs no
// DoDataExchange at all (deviation 1), so m_bTextLabels is not refreshed from
// the check box first: the bar receives whatever value the member last held.
// Symbol: ?OnTextLabels@CMFCToolBarsListPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnTextLabels_CMFCToolBarsListPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), TRUE);
    impl__EnableTextLabels_CMFCToolBar__QEAAXH_Z(d->m_pSelectedToolbar, d->m_bTextLabels);
}

// Retail (RVA 0x17c600), fully transcribed:
//     if (m_wndToolbarList.GetSafeHwnd() == NULL) return;                   // &member != NULL && m_hWnd != NULL
//     for (int i = 0; i < m_wndToolbarList.GetCount(); i++)                 // LB_GETCOUNT re-read each pass
//         if ((CMFCToolBar*)m_wndToolbarList.GetItemData(i) == pToolBar) {
//             m_wndToolbarList.SetCheck(i, bShow);                         // 0x2977c0
//             return;
//         }
// Symbol: ?ShowToolBar@CMFCToolBarsListPropertyPage@@QEAAXPEAVCMFCToolBar@@H@Z
extern "C" void MS_ABI impl__ShowToolBar_CMFCToolBarsListPropertyPage__QEAAXPEAVCMFCToolBar__H_Z(void* pThis, CMFCToolBar* pToolBar, int bShow) {
    if (pThis == nullptr) return;
    S_ToolbarsListPage* d = D(pThis);
    if (ListHwnd(d) == nullptr) return;
    for (int i = 0; i < ListGetCount(d); ++i) {
        if (ListGetToolbar(d, i) == pToolBar) {
            impl__SetCheck_CCheckListBox__QEAAXHH_Z(CheckList(d), i, bShow);
            return;
        }
    }
}
