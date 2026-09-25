// CMFCToolBarsToolsPropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

// ===========================================================================
// CMFCToolBarsToolsPropertyPage -- the "Tools" (user tools) page of the
// feature-pack Customize dialog (afxtoolbarstoolspropertypage.h:50 in the
// 14.51 SDK on this host; derives CPropertyPage).
//
// Bodies below were transcribed from the retail disassembly (the method
// described in the header of core/ole/COleControl.cpp).  Function bodies are
// byte-identical in mfc140.dll and mfc140u.dll; every RVA and absolute address
// quoted in this file is mfc140u.  Entries, resolved from each export's
// ordinal (mfc_complete_ordinal_mapping.json) through mfc140u.dll's export
// address table (urva.py); five of them have no entry in
// mfc140u_rva_symbols.json and were resolved that way only:
//
//   ??0 (ctor)                 0x17fce0     OnUpdateTool              0x180760
//   ??1 (dtor)                 0x17ff10     CreateNewTool             0x180830
//   DoDataExchange             0x180040     OnOK                      0x1808d0
//   OnInitDialog               0x180140     OnKillActive              0x1808e0
//   OnBrowseCommand            0x180670     EnableControls            0x180930
//   OnArgumentsOptions         0x1809c0     OnInitialDirectoryOptions 0x180ab0
//
// The one import slot named below was resolved with annu.py against
// mfc140u: 0x1802c7120 = USER32!SendMessageW.  0x1802c7b30 is not an import;
// it is the indirect-call target every virtual call in these bodies goes
// through.
//
// Layout.  The class is not declared in OpenMFC's public headers, so S_ToolsPage
// below is the only place its layout lives.  Every offset is a store or a
// `lea` in the constructor (0x17fce0), and the dtor (0x17ff10) destroys the
// members at the same offsets in reverse.  The member names are those of
// afxtoolbarstoolspropertypage.h, whose declaration order matches the
// offsets; the dialog-control ids DoDataExchange binds (afxribbonres.h)
// confirm the name-to-offset map (e.g. +0x158 <-> IDD_AFXBARRES_MENU_INITIAL_
// DIRECTORY, +0xca0 <-> IDD_AFXBARRES_MENU_ARGUMENTS).  Every vftable named
// below was identified from its RTTI complete-object locator (vftable[-1]).
// sizeof == 0x1f20: the last member (m_menuInitialDir, a 0x10-byte CMenu) sits
// at +0x1f10, and featurepack/customize/CMFCToolBarsCustomizeDialog.cpp
// allocates kSizeToolsPage = 0x1f20 for this page.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its vftable (0x18031b468) at +0x00,
//      the CEdit (0x1802da628) / CButton (0x1802df1b0) vftables into the
//      plain controls after ??0CWnd@@QEAA@XZ, the CVSToolsListBox vftable
//      (0x18031b7e8) into m_wndToolsList after ??0CVSListBox@@QEAA@XZ and the
//      CMenu vftable (0x1802e2f58) into the two inline-constructed CMenus.
//      OpenMFC has no MSVC-layout vtable for any of them, so +0x00 keeps what
//      the exported CPropertyPage constructor thunk leaves there, the plain
//      controls are built with the exported CWnd constructor alone (as the
//      sibling CMFCToolBarsMenuPropertyPage.cpp does), m_wndToolsList keeps
//      the vptr of the OpenMFC CVSListBox the exported ??0CVSListBox@@ thunk
//      placement-constructs, and the CMenus are raw zeroed blocks (NULL vfptr,
//      NULL m_hMenu), as CMFCToolBarsCustomizeDialog.cpp's StackMenu is.
//      Consequences:
//        * these thunks are not reached through the page's vtable, and the
//          afx_msg handlers are not reached through message dispatch;
//        * the UpdateData calls below go to the exported CWnd::UpdateData
//          thunk, whose OpenMFC body (core/window/CWnd.cpp) does not call
//          DoDataExchange, so the DDX transfers retail performs there do not
//          happen today; and even when DoDataExchange is called, OpenMFC's
//          DDX_Control (core/runtime/DdxExchange.cpp -> openmfc_ddx::Ctrl)
//          ignores the CWnd& and subclasses nothing, so the member controls'
//          m_hWnd stay NULL (EnableWindow / WM_CHAR below then reach no
//          window, and GetSelItem answers LB_ERR);
//        * CVSToolsListBox's overrides (vftable 0x18031b7e8 slots 107..112:
//          OnSelectionChanged 0x180530, OnBeforeRemoveItem 0x180320,
//          OnAfterAddItem 0x180370, OnAfterRenameItem 0x180440,
//          OnAfterMoveItemUp 0x1804d0, OnAfterMoveItemDown 0x180500) are
//          never reached.
//
//  (2) m_wndToolsList.  Retail calls its virtuals through the CVSToolsListBox
//      vftable: slot 96 (+0x300) GetSelItem and slot 100 (+0x320) GetItemData,
//      neither overridden by CVSToolsListBox (vtdump of 0x18031b7e8: they are
//      CVSListBox's 0x1c69c0 / 0x1c6870).  A client cannot change the type
//      of a member, so each call is devirtualised to the exported CVSListBox
//      thunk (featurepack/controls/CVSListBox.cpp).  That OpenMFC CVSListBox
//      is a CListBox, not retail's CVSListBoxBase/CStatic composite; it fits
//      in the retail member (0xe8 <= 0x348 bytes, asserted below).
//
//  (3) Virtual calls on m_pParentSheet (a CMFCToolBarsCustomizeDialog):
//      dispatched through the object's vtable only when that vtable lies
//      outside this image (a client's MSVC-derived dialog, laid out from the
//      SDK header, whose slot numbering is retail's); otherwise the retail
//      base body is used -- the scheme CMFCToolBarsCustomizeDialog.cpp's
//      ToolsPageBlocksClose uses for the same slot.
//
//  (4) `this` / NULL checks.  Retail tests no `this`; the thunks here return
//      early on a NULL `this`, and each other added check says so.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each signature follows the mangled name; the file
// that defines each is named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);          // core/dialog/RuntimeClasses.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                             // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);                           // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                          // core/controls/CtorDtorPlacement.cpp
extern "C" void* MS_ABI impl___0CMFCMenuButton__QEAA_XZ(void* pThis);                   // featurepack/menu/CMFCMenuButton.cpp
extern "C" void  MS_ABI impl___1CMFCMenuButton__UEAA_XZ(void* pThis);                   // featurepack/menu/CMFCMenuButton.cpp
extern "C" void* MS_ABI impl___0CVSListBox__QEAA_XZ(void* pThis);                       // featurepack/controls/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CVSListBox__UEAA_XZ(void* pThis);                       // featurepack/controls/CtorDtorPlacement.cpp
extern "C" int   MS_ABI impl__DestroyMenu_CMenu__QEAAHXZ(CMenu* pThis);                 // core/window/CMenu.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp

extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);    // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);            // core/window/CWnd.cpp

extern "C" int       MS_ABI impl__GetSelItem_CVSListBox__UEBAHXZ(const CVSListBox* pThis);                  // featurepack/controls/CVSListBox.cpp
extern "C" uintptr_t MS_ABI impl__GetItemData_CVSListBox__UEBA_KH_Z(const CVSListBox* pThis, int nIndex);   // featurepack/controls/CVSListBox.cpp
extern "C" void      MS_ABI impl__SetCommand_CUserTool__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCmd);  // featurepack/customize/CUserTool.cpp

extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);   // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                    // detail/MfcExceptionsSupport.cpp

// ?afxUserToolsManager@@3PEAVCUserToolsManager@@EA (featurepack/CMFC_misc_stubs.cpp;
// NULL in this tree -- nothing assigns this exported datum).  Do not confuse it
// with the DLL-internal C++ global `afxUserToolsManager` (detail/MfccoreSupport.cpp),
// which does point at a 0x28-byte g_userToolsManager and is what
// CWinAppEx::GetUserToolsManager returns; retail reads only the exported datum.
extern "C" void* impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;

// The mingw linker's image base symbol (deviation 3).
extern "C" IMAGE_DOS_HEADER __ImageBase;

// This file's own thunks that other bodies here call (retail calls them
// directly, not through the vtable).
extern "C" void MS_ABI impl__EnableControls_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnUpdateTool_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// The retail object (0x1f20 bytes); offsets from the constructor (0x17fce0).
// ---------------------------------------------------------------------------
struct alignas(8) S_ToolsPage {
    unsigned char m_base[0x158];                 // +0x000 CPropertyPage (OpenMFC sizeof == 0x158, asserted below)
    unsigned char m_wndInitialDirBtn[0xb48];     // +0x158 CMFCMenuButton, IDD_AFXBARRES_MENU_INITIAL_DIRECTORY 0x4265
    unsigned char m_wndArgumentsBtn[0xb48];      // +0xca0 CMFCMenuButton, IDD_AFXBARRES_MENU_ARGUMENTS 0x425d
    unsigned char m_wndArgumentsEdit[0xe8];      // +0x17e8 CEdit, IDD_AFXBARRES_ARGUMENTS 0x421c
    unsigned char m_wndInitialDirEdit[0xe8];     // +0x18d0 CEdit, IDD_AFXBARRES_INITIAL_DIRECTORY 0x421d
    unsigned char m_wndCommandEdit[0xe8];        // +0x19b8 CEdit, IDD_AFXBARRES_COMMAND 0x421a
    unsigned char m_wndBrowseBtn[0xe8];          // +0x1aa0 CButton, IDD_AFXBARRES_BROWSE_COMMAND 0x421b
    unsigned char m_wndToolsList[0x348];         // +0x1b88 CVSToolsListBox: its CVSListBox part ...
    void*         m_wndToolsList_pParent;        // +0x1ed0 ... and CVSToolsListBox::m_pParent (ctor: mov %r14,0x348(%rbx))
    unsigned char m_strCommand[8];               // +0x1ed8 CString
    unsigned char m_strArguments[8];             // +0x1ee0 CString
    unsigned char m_strInitialDirectory[8];      // +0x1ee8 CString
    CUserTool*    m_pSelTool;                    // +0x1ef0
    void*         m_pParentSheet;                // +0x1ef8 CMFCToolBarsCustomizeDialog*
    unsigned char m_menuArguments[0x10];         // +0x1f00 CMenu (vfptr, m_hMenu)
    unsigned char m_menuInitialDir[0x10];        // +0x1f10 CMenu
};
static_assert(offsetof(S_ToolsPage, m_wndInitialDirBtn) == 0x158, "ctor: lea 0x158(%r14); ??0CMFCMenuButton");
static_assert(offsetof(S_ToolsPage, m_wndArgumentsBtn) == 0xca0, "ctor: lea 0xca0(%r14); ??0CMFCMenuButton");
static_assert(offsetof(S_ToolsPage, m_wndArgumentsEdit) == 0x17e8, "ctor: lea 0x17e8(%r14); ??0CWnd, CEdit vftable");
static_assert(offsetof(S_ToolsPage, m_wndInitialDirEdit) == 0x18d0, "ctor: lea 0x18d0(%r14)");
static_assert(offsetof(S_ToolsPage, m_wndCommandEdit) == 0x19b8, "ctor: lea 0x19b8(%r14)");
static_assert(offsetof(S_ToolsPage, m_wndBrowseBtn) == 0x1aa0, "ctor: lea 0x1aa0(%r14); ??0CWnd, CButton vftable");
static_assert(offsetof(S_ToolsPage, m_wndToolsList) == 0x1b88, "ctor: lea 0x1b88(%r14); ??0CVSListBox");
static_assert(offsetof(S_ToolsPage, m_wndToolsList_pParent) == 0x1ed0, "ctor: mov %r14,0x348(%rbx) with rbx = this+0x1b88");
static_assert(offsetof(S_ToolsPage, m_strCommand) == 0x1ed8, "ctor: lea 0x1ed8(%r14),%rsi");
static_assert(offsetof(S_ToolsPage, m_strArguments) == 0x1ee0, "ctor: lea 0x1ee0(%r14),%rdi");
static_assert(offsetof(S_ToolsPage, m_strInitialDirectory) == 0x1ee8, "ctor: lea 0x1ee8(%r14),%rbx");
static_assert(offsetof(S_ToolsPage, m_pSelTool) == 0x1ef0, "ctor: movq $0x0,0x1ef0(%r14)");
static_assert(offsetof(S_ToolsPage, m_pParentSheet) == 0x1ef8, "ctor: movq $0x0,0x1ef8(%r14); OnInitDialog stores it");
static_assert(offsetof(S_ToolsPage, m_menuArguments) == 0x1f00, "ctor: CMenu vftable -> 0x1f00, 0 -> 0x1f08");
static_assert(offsetof(S_ToolsPage, m_menuInitialDir) == 0x1f10, "ctor: CMenu vftable -> 0x1f10, 0 -> 0x1f18");
static_assert(sizeof(S_ToolsPage) == 0x1f20, "retail sizeof(CMFCToolBarsToolsPropertyPage) == 0x1f20");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CEdit) == 0xe8 && sizeof(CButton) == 0xe8, "the plain controls are bare CWnds");
static_assert(sizeof(CVSListBox) <= 0x348, "the ??0CVSListBox thunk placement-constructs OpenMFC's CVSListBox in the retail slot (deviation 2)");
static_assert(sizeof(CMenu) == 0x10, "retail CMenu: vfptr + m_hMenu");
static_assert(sizeof(CString) == 8, "one CStringT data pointer");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads the edits' HWNDs at +0x40 (0x1828 = 0x17e8+0x40, 0x1910 = 0x18d0+0x40)");
static_assert(offsetof(CUserTool, m_strArguments) == 0x10, "OnUpdateTool: lea 0x10(%rdi) on the CUserTool");
static_assert(offsetof(CUserTool, m_strInitialDirectory) == 0x18, "OnUpdateTool: lea 0x18(%rdi) on the CUserTool");

// CMFCMenuButton::m_nMenuResult, +0xb30 in the retail CMFCMenuButton
// (featurepack/menu/CMFCMenuButton.cpp's S_MenuButton pins it from that ctor,
// 0x8e2c0).  Read here as 0xc88 = 0x158 + 0xb30 and 0x17d0 = 0xca0 + 0xb30.
constexpr int kOffMenuButtonMenuResult = 0xb30;

// Resource ids (afxribbonres.h in the 14.51 SDK on this host).
constexpr unsigned int kIddPropPage7              = 0x4220;   // IDD_AFXBARRES_PROPPAGE7
constexpr int          kIdcCommandsList           = 0x4219;   // IDD_AFXBARRES_COMMANDS_LIST
constexpr int          kIdcCommand                = 0x421a;   // IDD_AFXBARRES_COMMAND
constexpr int          kIdcBrowseCommand          = 0x421b;   // IDD_AFXBARRES_BROWSE_COMMAND
constexpr int          kIdcArguments              = 0x421c;   // IDD_AFXBARRES_ARGUMENTS
constexpr int          kIdcInitialDirectory       = 0x421d;   // IDD_AFXBARRES_INITIAL_DIRECTORY
constexpr int          kIdcMenuArguments          = 0x425d;   // IDD_AFXBARRES_MENU_ARGUMENTS
constexpr int          kIdcMenuInitialDirectory   = 0x4265;   // IDD_AFXBARRES_MENU_INITIAL_DIRECTORY

// CMFCToolBarsCustomizeDialog vftable slot 101 (+0x328), CheckToolsValidity.
// Retail's base body is `return TRUE` (vftable 0x1803190d8 slot 101 -> 0x3a60;
// CMFCToolBarsCustomizeDialog.cpp documents the same slot).
constexpr int kSlotCheckToolsValidity = 101;
typedef int (MS_ABI *CheckToolsValidityFn)(void* pThis, const void* pListTools);

inline S_ToolsPage* D(void* p) { return static_cast<S_ToolsPage*>(p); }
inline CWnd* Self(void* p) { return static_cast<CWnd*>(p); }
inline CWnd* Wnd(unsigned char* storage) { return reinterpret_cast<CWnd*>(storage); }
inline HWND Hwnd(unsigned char* storage) { return Wnd(storage)->m_hWnd; }
inline CString* Str(unsigned char* storage) { return reinterpret_cast<CString*>(storage); }
inline CMenu* Menu(unsigned char* storage) { return reinterpret_cast<CMenu*>(storage); }
inline const CVSListBox* List(S_ToolsPage* d) { return reinterpret_cast<const CVSListBox*>(d->m_wndToolsList); }
inline int MenuResult(unsigned char* btn) {
    int v = 0;
    std::memcpy(&v, btn + kOffMenuButtonMenuResult, sizeof v);
    return v;
}

bool PointsIntoThisImage(const void* p) {
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* q = static_cast<const unsigned char*>(p);
    return q >= base && q < base + nt->OptionalHeader.SizeOfImage;
}

// The shared body of OnArgumentsOptions / OnInitialDirectoryOptions (retail
// has two copies that differ only in which button/edit they use and in the
// failure path, see each caller):
//     CString str;                                   // nil string
//     HINSTANCE h = AfxFindStringResourceHandle(id); // 0x2aee00
//     <failure: throw or return>  unless h && str.LoadString(h, id)   // 0xdb70
//     for (int i = 0; i < str.GetLength(); i++)
//         ::SendMessage(edit.m_hWnd, WM_CHAR, str[i], 0);             // import 0x1802c7120
// The loop re-reads the length and the edit's m_hWnd every iteration and
// holds the buffer pointer, as retail does.  Retail's str[i] carries the
// CSimpleStringT bounds check (AtlThrow(E_INVALIDARG), 0x80070057, when
// i < 0 || i > length); the loop bounds make it unreachable, so it is not
// reproduced.
void TypeResourceString(UINT nID, unsigned char* edit, bool bThrowOnFailure) {
    CString str;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nID)) {
        if (bThrowOnFailure) impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    const wchar_t* psz = str.GetString();
    for (int i = 0; i < str.GetLength(); ++i) {
        ::SendMessage(Hwnd(edit), WM_CHAR, static_cast<WPARAM>(static_cast<unsigned short>(psz[i])), 0);
    }
}

} // namespace

// Retail (RVA 0x17fce0, mfc140u), transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE7 /*0x4220*/, 0,
//                                  /*dwSize*/ 0x68);                     // 0x215910
//     vfptr = 0x18031b468;
//     ??0CMFCMenuButton on +0x158 and +0xca0;                           // 0x8e2c0
//     ??0CWnd (0x28a700) then the CEdit vftable on +0x17e8/+0x18d0/+0x19b8,
//         ??0CWnd then the CButton vftable on +0x1aa0;
//     ??0CVSListBox on +0x1b88 (0x1c6490), then the CVSToolsListBox vftable
//         and m_pParent (+0x348) = this;
//     m_strCommand / m_strArguments / m_strInitialDirectory: nil string, then
//         SetString(L"", 0) -- i.e. empty;
//     m_menuArguments / m_menuInitialDir: CMenu vftable, m_hMenu = NULL;
//     m_pSelTool = NULL;  m_pParentSheet = NULL;
// DEVIATION: the vftable stores are deviation (1); the CMenu blocks are
// zero-filled instead (NULL vfptr).
// Symbol: ??0CMFCToolBarsToolsPropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarsToolsPropertyPage__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    S_ToolsPage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage7, 0, 0x68);
    impl___0CMFCMenuButton__QEAA_XZ(d->m_wndInitialDirBtn);
    impl___0CMFCMenuButton__QEAA_XZ(d->m_wndArgumentsBtn);
    for (unsigned char* w : {d->m_wndArgumentsEdit, d->m_wndInitialDirEdit, d->m_wndCommandEdit, d->m_wndBrowseBtn}) {
        impl___0CWnd__QEAA_XZ(w);
    }
    impl___0CVSListBox__QEAA_XZ(d->m_wndToolsList);
    d->m_wndToolsList_pParent = pThis;
    new (d->m_strCommand) CString();
    new (d->m_strArguments) CString();
    new (d->m_strInitialDirectory) CString();
    std::memset(d->m_menuArguments, 0, sizeof d->m_menuArguments);
    std::memset(d->m_menuInitialDir, 0, sizeof d->m_menuInitialDir);
    d->m_pSelTool = nullptr;
    d->m_pParentSheet = nullptr;
    return pThis;
}

// Retail (RVA 0x17ff10, mfc140u), transcribed: re-install the class vftable
// (deviation 1); inline ~CMenu on +0x1f10 then +0x1f00 (CMenu vftable store,
// then ?DestroyMenu@CMenu@@ 0x2a8190); release the three CStrings in reverse
// order (+0x1ee8, +0x1ee0, +0x1ed8); ??1CVSListBox on +0x1b88 (0x1c6540);
// ??1CButton on +0x1aa0; ??1CEdit on +0x19b8/+0x18d0/+0x17e8; the inline
// ~CMFCMenuButton (CMFCMenuButton vftable 0x1802efab8, then ??1CMFCButton
// 0x1cad0) on +0xca0 then +0x158; and a tail jump to ??1CPropertyPage
// (0x215f40).  The exported ??1CMFCMenuButton thunk performs the same
// vptr-restore + ??1CMFCButton pair, so it stands in for the inlined copy.
// Symbol: ??1CMFCToolBarsToolsPropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarsToolsPropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolsPage* d = D(pThis);
    impl__DestroyMenu_CMenu__QEAAHXZ(Menu(d->m_menuInitialDir));
    impl__DestroyMenu_CMenu__QEAAHXZ(Menu(d->m_menuArguments));
    Str(d->m_strInitialDirectory)->~CString();
    Str(d->m_strArguments)->~CString();
    Str(d->m_strCommand)->~CString();
    impl___1CVSListBox__UEAA_XZ(d->m_wndToolsList);
    impl___1CButton__UEAA_XZ(d->m_wndBrowseBtn);
    impl___1CEdit__UEAA_XZ(reinterpret_cast<CEdit*>(d->m_wndCommandEdit));
    impl___1CEdit__UEAA_XZ(reinterpret_cast<CEdit*>(d->m_wndInitialDirEdit));
    impl___1CEdit__UEAA_XZ(reinterpret_cast<CEdit*>(d->m_wndArgumentsEdit));
    impl___1CMFCMenuButton__UEAA_XZ(d->m_wndArgumentsBtn);
    impl___1CMFCMenuButton__UEAA_XZ(d->m_wndInitialDirBtn);
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (RVA 0x180830, mfc140u):
//     if (afxUserToolsManager->m_lstUserTools.GetCount() /*+0x20*/ ==
//         m_uiCmdLast /*+0x48*/ - m_uiCmdFirst /*+0x44*/ + 1) {
//         CString s; s.Format(IDS_AFXBARRES_TOO_MANY_TOOLS_FMT /*0x3e98*/, max);
//         AfxMessageBox(s, 0, 0);  return NULL;
//     }
//     return afxUserToolsManager->CreateNewTool();     // tail jump, 0x183c80
// STUB: the exported ?afxUserToolsManager@@ datum is never assigned in this
// tree (retail would fault on the NULL read; see its declaration above), and
// OpenMFC's CUserToolsManager (0x28 bytes) has none of the retail members read
// here (+0x20, +0x44, +0x48).  Reachable in retail from
// CVSToolsListBox::OnAfterAddItem (0x180370, mfc140u).
// Symbol: ?CreateNewTool@CMFCToolBarsToolsPropertyPage@@IEAAPEAVCUserTool@@XZ
extern "C" void* MS_ABI impl__CreateNewTool_CMFCToolBarsToolsPropertyPage__IEAAPEAVCUserTool__XZ(void* pThis) {
    (void)pThis;
    return nullptr;
}

// Retail (RVA 0x180040, mfc140u), fully transcribed -- no CPropertyPage::
// DoDataExchange call, the DDX calls in this order, the last a tail jump:
//     DDX_Control(pDX, 0x4265 MENU_INITIAL_DIRECTORY, m_wndInitialDirBtn);
//     DDX_Control(pDX, 0x425d MENU_ARGUMENTS,         m_wndArgumentsBtn);
//     DDX_Control(pDX, 0x421c ARGUMENTS,              m_wndArgumentsEdit);
//     DDX_Control(pDX, 0x421d INITIAL_DIRECTORY,      m_wndInitialDirEdit);
//     DDX_Control(pDX, 0x421a COMMAND,                m_wndCommandEdit);
//     DDX_Control(pDX, 0x421b BROWSE_COMMAND,         m_wndBrowseBtn);
//     DDX_Control(pDX, 0x4219 COMMANDS_LIST,          m_wndToolsList);
//     DDX_Text   (pDX, 0x421a COMMAND,                m_strCommand);
//     DDX_Text   (pDX, 0x421c ARGUMENTS,              m_strArguments);
//     DDX_Text   (pDX, 0x421d INITIAL_DIRECTORY,      m_strInitialDirectory);
// (Not reached through UpdateData today -- deviation 1.)
// Symbol: ?DoDataExchange@CMFCToolBarsToolsPropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarsToolsPropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_ToolsPage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMenuInitialDirectory, d->m_wndInitialDirBtn);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcMenuArguments, d->m_wndArgumentsBtn);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcArguments, d->m_wndArgumentsEdit);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcInitialDirectory, d->m_wndInitialDirEdit);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCommand, d->m_wndCommandEdit);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcBrowseCommand, d->m_wndBrowseBtn);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCommandsList, d->m_wndToolsList);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcCommand, d->m_strCommand);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcArguments, d->m_strArguments);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcInitialDirectory, d->m_strInitialDirectory);
}

// Retail (RVA 0x180930, mfc140u), fully transcribed:
//     BOOL bEnable = m_wndToolsList.GetSelItem() >= 0;     // vslot 96 (+0x300), deviation 2
//     m_wndCommandEdit.EnableWindow(bEnable);              // 0x2a9b30, each
//     m_wndArgumentsEdit.EnableWindow(bEnable);
//     m_wndInitialDirEdit.EnableWindow(bEnable);
//     m_wndBrowseBtn.EnableWindow(bEnable);
//     m_wndInitialDirBtn.EnableWindow(bEnable);
//     m_wndArgumentsBtn.EnableWindow(bEnable);             // tail jump
// Symbol: ?EnableControls@CMFCToolBarsToolsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__EnableControls_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolsPage* d = D(pThis);
    const int bEnable = impl__GetSelItem_CVSListBox__UEBAHXZ(List(d)) >= 0 ? TRUE : FALSE;
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndCommandEdit), bEnable);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndArgumentsEdit), bEnable);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndInitialDirEdit), bEnable);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndBrowseBtn), bEnable);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndInitialDirBtn), bEnable);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndArgumentsBtn), bEnable);
}

// Retail (RVA 0x1809c0, mfc140u), fully transcribed:
//     if (m_wndArgumentsBtn.m_nMenuResult /*+0x17d0*/ == 0) return;
//     <TypeResourceString(m_nMenuResult, m_wndArgumentsEdit)> where both a
//     NULL AfxFindStringResourceHandle result and a failed LoadString jump to
//     AfxThrowInvalidArgException (0x227720) -- the ENSURE'd CString::LoadString.
// Symbol: ?OnArgumentsOptions@CMFCToolBarsToolsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnArgumentsOptions_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolsPage* d = D(pThis);
    const int nID = MenuResult(d->m_wndArgumentsBtn);
    if (nID == 0) return;
    TypeResourceString(static_cast<UINT>(nID), d->m_wndArgumentsEdit, true);
}

// Retail (RVA 0x180670, mfc140u):
//     CFileDialog dlg(TRUE, afxUserToolsManager->m_strDefExt /*+0x60*/, NULL, 0,
//                     afxUserToolsManager->m_strFilter /*+0x58*/, this, 0, TRUE);   // 0x20a1c0
//     if (dlg.DoModal() == IDOK) {                                                  // 0x20c0a0
//         m_strCommand = dlg.GetPathName();                                        // 0x20c350
//         UpdateData(FALSE);  OnUpdateTool();
//     }
// STUB: the exported ?afxUserToolsManager@@ datum is never assigned in this
// tree (retail would fault on the NULL read), and OpenMFC's CUserToolsManager
// (0x28 bytes) has neither m_strFilter (+0x58) nor m_strDefExt (+0x60).
// Symbol: ?OnBrowseCommand@CMFCToolBarsToolsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnBrowseCommand_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x180140, mfc140u): CDialog::OnInitDialog(); ENSURE(afxUserToolsManager);
// m_pParentSheet = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent()),
// ENSURE'd; m_wndToolsList.SetStandardButtons(0xf); m_pParentSheet->OnInitToolsPage()
// (vslot 98); one AddItem(pTool->m_strLabel, pTool) per tool in
// afxUserToolsManager->m_lstUserTools (+0x10 head); then, per non-zero
// m_uiInitialDirMenuID (+0x6c) / m_uiArgumentsMenuID (+0x68): show the menu
// button, LoadMenu into m_menuInitialDir / m_menuArguments, and hand its
// first submenu to the button's m_hMenu; m_bRightArrow = TRUE on both;
// EnableControls(); return TRUE.
// STUB: every step past CDialog::OnInitDialog needs a non-NULL exported
// ?afxUserToolsManager@@ with the retail member layout; in this tree that datum
// is never assigned (retail would throw from the ENSURE), and OpenMFC's
// CUserToolsManager (0x28 bytes) has no m_lstUserTools head at +0x10 and
// nothing at +0x68/+0x6c.  (CMFCToolBarsCustomizeDialog.cpp adds this page to
// the sheet only when that datum is non-NULL, so the page is never shown.)
// Symbol: ?OnInitDialog@CMFCToolBarsToolsPropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarsToolsPropertyPage__MEAAHXZ(void* pThis) {
    (void)pThis;
    return 0;
}

// Retail (RVA 0x180ab0, mfc140u), fully transcribed:
//     if (m_wndInitialDirBtn.m_nMenuResult /*+0xc88*/ == 0) return;
//     <TypeResourceString(m_nMenuResult, m_wndInitialDirEdit)> where, unlike
//     OnArgumentsOptions, a NULL AfxFindStringResourceHandle result or a
//     failed LoadString simply releases the string and returns (both jump to
//     0x180b53) -- no exception.
// Symbol: ?OnInitialDirectoryOptions@CMFCToolBarsToolsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnInitialDirectoryOptions_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolsPage* d = D(pThis);
    const int nID = MenuResult(d->m_wndInitialDirBtn);
    if (nID == 0) return;
    TypeResourceString(static_cast<UINT>(nID), d->m_wndInitialDirEdit, false);
}

// Retail (RVA 0x1808e0, mfc140u), fully transcribed -- no
// CPropertyPage::OnKillActive call:
//     if (!m_pParentSheet->CheckToolsValidity(afxUserToolsManager->m_lstUserTools))
//         return FALSE;                                   // vslot 101 (+0x328)
//     return UpdateData(TRUE) != 0;                       // 0x2910d0
// The list argument is `afxUserToolsManager + 8` (an `add $0x8`, no load), so
// it is formed here the same way without dereferencing the pointer.
// DEVIATIONS: CheckToolsValidity is dispatched per deviation (3) -- only a
// client vtable is called; OpenMFC's own dialog stands for the retail base
// body, `return TRUE`.  m_pParentSheet is set only by OnInitDialog (a stub
// here), so it is NULL in practice; retail would fault on it, and here a NULL
// sheet is treated as "nothing to validate" (deviation 4).  Likewise a NULL
// exported ?afxUserToolsManager@@ is treated as "nothing to validate" rather
// than handing a client override a CObList& at address 8: in retail a set
// m_pParentSheet implies OnInitDialog already ENSURE'd the manager non-NULL,
// and CMFCToolBarsCustomizeDialog.cpp's ToolsPageBlocksClose applies the same
// guard before the same slot.
// Symbol: ?OnKillActive@CMFCToolBarsToolsPropertyPage@@UEAAHXZ
extern "C" int MS_ABI impl__OnKillActive_CMFCToolBarsToolsPropertyPage__UEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_ToolsPage* d = D(pThis);
    int bValid = TRUE;
    void* pSheet = d->m_pParentSheet;
    void* pMgr = impl__afxUserToolsManager__3PEAVCUserToolsManager__EA;
    if (pSheet != nullptr && pMgr != nullptr) {
        const void* vptr = *static_cast<void* const*>(pSheet);
        if (vptr != nullptr && !PointsIntoThisImage(vptr)) {
            const void* pListTools = static_cast<const unsigned char*>(pMgr) + 8;
            bValid = reinterpret_cast<CheckToolsValidityFn const*>(vptr)[kSlotCheckToolsValidity](pSheet, pListTools);
        }
    }
    if (!bValid) return FALSE;
    return impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE) != 0 ? TRUE : FALSE;
}

// Retail (RVA 0x1808d0, mfc140u), fully transcribed: `jmp OnUpdateTool`
// (0x180760) -- no CPropertyPage::OnOK call.
// Symbol: ?OnOK@CMFCToolBarsToolsPropertyPage@@UEAAXXZ
extern "C" void MS_ABI impl__OnOK_CMFCToolBarsToolsPropertyPage__UEAAXXZ(void* pThis) {
    impl__OnUpdateTool_CMFCToolBarsToolsPropertyPage__IEAAXXZ(pThis);
}

// Retail (RVA 0x180760, mfc140u), fully transcribed:
//     UpdateData(TRUE);                                            // 0x2910d0
//     int iSel = m_wndToolsList.GetSelItem();                      // vslot 96, deviation 2
//     CUserTool* pTool = iSel >= 0 ? (CUserTool*)m_wndToolsList.GetItemData(iSel)   // vslot 100
//                                  : NULL;
//     if (pTool == NULL) {
//         m_strCommand.Empty(); m_strArguments.Empty(); m_strInitialDirectory.Empty();   // 0x33b0
//         UpdateData(FALSE);
//     } else {
//         pTool->SetCommand(m_strCommand);                         // 0x183390
//         pTool->m_strArguments = m_strArguments;                  // +0x10, 0xde30
//         pTool->m_strInitialDirectory = m_strInitialDirectory;    // +0x18, 0xde30
//     }
//     EnableControls();                                            // tail jump, 0x180930
// Retail does not touch m_pSelTool here.  The item data is whatever
// CUserTool* was stored with the list item; OpenMFC's CUserTool has the
// retail member layout (asserted above).  NOTE: OpenMFC's
// CUserToolsManager::CreateNewTool (featurepack/customize/CUserToolsManager.cpp)
// hands out 8-byte placeholder blocks, not CUserTool objects; none of them
// reaches this list in this tree, because the only retail paths that add items
// (OnInitDialog, CVSToolsListBox::OnAfterAddItem) are not reproduced.
// Symbol: ?OnUpdateTool@CMFCToolBarsToolsPropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnUpdateTool_CMFCToolBarsToolsPropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_ToolsPage* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), TRUE);
    const int iSel = impl__GetSelItem_CVSListBox__UEBAHXZ(List(d));
    CUserTool* pTool = iSel >= 0
        ? reinterpret_cast<CUserTool*>(impl__GetItemData_CVSListBox__UEBA_KH_Z(List(d), iSel))
        : nullptr;
    if (pTool == nullptr) {
        Str(d->m_strCommand)->Empty();
        Str(d->m_strArguments)->Empty();
        Str(d->m_strInitialDirectory)->Empty();
        impl__UpdateData_CWnd__QEAAHH_Z(Self(pThis), FALSE);
    } else {
        impl__SetCommand_CUserTool__QEAAXPEB_W_Z(pTool, Str(d->m_strCommand)->GetString());
        pTool->m_strArguments = *Str(d->m_strArguments);
        pTool->m_strInitialDirectory = *Str(d->m_strInitialDirectory);
    }
    impl__EnableControls_CMFCToolBarsToolsPropertyPage__IEAAXXZ(pThis);
}
