// CMFCMousePropertyPage — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <new>

// ===========================================================================
// CMFCMousePropertyPage -- the "Mouse" page of the feature-pack Customize
// sheet (afxmousepropertypage.h, derives CPropertyPage).  It lets the user
// bind a double-click command to each view registered with afxMouseManager.
//
// Bodies below were transcribed from the retail disassembly (the method
// described in the header of core/ole/COleControl.cpp).  disas.py reads the
// ANSI twin mfc140.dll; the bodies were read there and each entry below was
// then resolved in mfc140u.dll from the export's ordinal
// (mfc_complete_ordinal_mapping.json) through its export address table:
//
//   ctor                     0x91270   (mfc140u)   OnSelchangeListOfCommands 0x91550   (mfc140u)
//   dtor                     0x913c0   (mfc140u)   OnItemchangedListViews    0x91630   (mfc140u)
//   CreateObject             0x91230   (mfc140u)   OnInitDialog              0x91770   (mfc140u)
//   DoDataExchange           0x91440   (mfc140u)   EnableDblClkControls      0x91c30   (mfc140u)
//   OnNoDblcliick            0x914c0   (mfc140u)   SelectCommand             0x91ca0   (mfc140u)
//   OnUseDblcliick           0x914f0   (mfc140u)
//
// Every USER32 call below goes through an import slot that iat.py resolved in
// the ANSI image to SendMessageA / MessageBeep / GetClientRect / LoadIconW /
// DestroyIcon / CopyRect / GetParent; ::SendMessage is written so UNICODE
// picks SendMessageW, as mfc140u imports it.  The two comctl32 calls of
// OnInitDialog go through retail's activation-context wrappers (mfc140
// 0x60aa8 and 0x1c270, unexported), which look up "ImageList_ReplaceIcon" and
// "ImageList_GetImageInfo" by name (strings at 0x18033c618 / 0x18033b048 in
// mfc140.dll) and call them; OpenMFC calls the comctl32 functions directly.
//
// The class is NOT declared in OpenMFC's public headers, so S_MousePage below
// is the only place its layout lives.  The retail ctor (0x91270 mfc140u)
// builds CPropertyPage at +0, stores the CImageList vftable at +0x160 and
// NULL at +0x168, runs ??0CWnd@@QEAA@XZ on +0x170 / +0x258 / +0x340 (each
// followed by a CListCtrl / CListBox / CStatic vftable store), builds the
// CString at +0x428 and stores -1 at +0x158; CreateObject passes 0x430 to
// operator new, and the harvested descriptor in
// featurepack/customize/RuntimeClasses.cpp agrees (1072).  The member names are
// those of afxmousepropertypage.h (14.51 SDK on this host), whose declaration
// order matches those offsets.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores the class vftable at +0x00 and the
//      CImageList / CListCtrl / CListBox / CStatic vftables in the members.
//      OpenMFC has no MSVC-layout vtable for this class: +0x00 keeps what the
//      CPropertyPage ctor thunk installs, the three controls are built with
//      the exported CWnd constructor alone, and m_ViewsImages is zero-filled
//      and then given the exported CImageList constructor (which sets only
//      m_hImageList).  The CWnd-ctor treatment of the controls is the pattern
//      of CMFCToolBarsKeyboardPropertyPage.cpp (which has no CImageList member,
//      so the image-list part has no precedent there).
//      A virtual OnInitDialog / DoDataExchange call through this object's
//      vtable therefore reaches CPropertyPage's entries, not the thunks below.
//
//  (2) UpdateData.  The retail bodies call CWnd::UpdateData(FALSE), which in
//      retail runs DoDataExchange and so pushes m_strCommandDescription into
//      the IDC_AFXBARRES_COMMAND_DESCRIPTION static through DDX_Text.  The
//      exported CWnd::UpdateData thunk (core/window/Thunks.cpp) reaches
//      OpenMFC's CWnd::UpdateData (core/window/CWnd.cpp), which does NOT call
//      DoDataExchange; the call is made as retail makes it, but the text
//      transfer does not happen today.
//
//  (3) afxMouseManager.  Retail reads the unexported global afxMouseManager
//      (0x1803be268, mfc140u).  OpenMFC keeps it in
//      featurepack/customize/CMouseManager.cpp and exposes it through the
//      C-linkage accessor OpenMfcGetAfxMouseManager() defined there.  Every
//      CMouseManager thunk this file calls also returns its neutral value
//      when handed a NULL manager (that file's own guard), where retail would
//      dereference it in OnNoDblcliick / OnUseDblcliick, which do not ENSURE.
//
//  (4) Virtual calls on other objects.  CFrameWnd::GetMessageString (frame
//      vftable slot 95, +0x2f8) and CMFCToolBarsCustomizeDialog::
//      FillAllCommandsList (dialog vftable slot 97, +0x308 -- slot 97 of the
//      mfc140 vftable 0x180316f28 is ?FillAllCommandsList@, checked with
//      vtdump.py) are devirtualised to their exported OpenMFC thunks, as the
//      sibling customize pages do; an override in a client class is not
//      reached.
//
//  (5) CString.  m_strCommandDescription is OpenMFC's CString (one pointer,
//      the size retail's CString occupies at +0x428), manipulated through
//      OpenMFC's inline CString and the exported thunks that take it.
//
//  (6) Exceptions.  ENSURE failures throw through the exported
//      AfxThrowInvalidArgException thunk, as retail does (0x227720 mfc140u).
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the mangled names; the file that
// defines each is named on its line.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                                  // detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___0CPropertyPage__QEAA_IIK_Z(void* pThis, unsigned int nIDTemplate, unsigned int nIDCaption, unsigned long dwSize);   // core/dialog/Thunks.cpp
extern "C" void  MS_ABI impl___1CPropertyPage__UEAA_XZ(CPropertyPage* pThis);                                  // core/dialog/RuntimeClasses.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                   // detail/DlgcoreSupport.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                                    // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis);                                              // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis);                                            // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CListCtrl__UEAA_XZ(CListCtrl* pThis);                                          // core/controls/RuntimeClasses.cpp
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* pThis);                                   // core/gdi/CImageList.cpp
extern "C" void  MS_ABI impl___1CImageList__UEAA_XZ(CImageList* pThis);                                        // core/gdi/CImageList.cpp
extern "C" int   MS_ABI impl__Create_CImageList__QEAAHHHIHH_Z(CImageList* pThis, int cx, int cy, unsigned int nFlags, int nInitial, int nGrow);   // core/gdi/CImageList.cpp
extern "C" CImageList* MS_ABI impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST hImageList);  // core/gdi/CImageList.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);                        // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                               // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__CheckDlgButton_CWnd__QEAAXHI_Z(CWnd* pThis, int nIDButton, unsigned int nCheck);    // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);                  // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                                  // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CFrameWnd* pThis, unsigned int nID, CString* rMessage);   // core/frame/CFrameWnd.cpp

extern "C" uintptr_t MS_ABI impl__GetItemData_CListCtrl__QEBA_KH_Z(const CListCtrl* pThis, int nItem);            // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(CListCtrl* pThis, int nCol, const wchar_t* lpszColumnHeading, int nFormat, int nWidth, int nSubItem);   // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__InsertItem_CListCtrl__QEAAHIHPEB_WIIH_J_Z(CListCtrl* pThis, UINT nMask, int nItem, const wchar_t* lpszItem, UINT nState, UINT nStateMask, int nImage, LPARAM lParam);   // core/controls/CListCtrl.cpp
extern "C" int   MS_ABI impl__SetItem_CListCtrl__QEAAHHHIPEB_WHII_J_Z(CListCtrl* pThis, int nItem, int nSubItem, UINT nMask, const wchar_t* lpszItem, int nImage, UINT nState, UINT nStateMask, LPARAM lParam);   // core/controls/CListCtrl.cpp

extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);   // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ();        // featurepack/customize/RuntimeClasses.cpp
extern "C" void  MS_ABI impl__FillAllCommandsList_CMFCToolBarsCustomizeDialog__UEBAXAEAVCListBox___Z(const void* pThis, CWnd* pWndListOfCommands);   // featurepack/customize/CMFCToolBarsCustomizeDialog.cpp

extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszName, const wchar_t* lpszType);   // core/runtime/Globals.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                              // detail/MfcExceptionsSupport.cpp

extern "C" void* MS_ABI impl___0CStringList__QEAA__J_Z(CStringList* pThis, long long nBlockSize);                 // core/collections/CStringList.cpp
extern "C" void  MS_ABI impl___1CStringList__UEAA_XZ(CStringList* pThis);                                         // core/collections/CStringList.cpp
extern "C" CStringList::POSITION MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const CStringList* pThis, long long nIndex);   // core/collections/CStringList.cpp

// featurepack/customize/CMouseManager.cpp (deviation (3)).
extern "C" void* OpenMfcGetAfxMouseManager();
extern "C" void  MS_ABI impl__GetViewNames_CMouseManager__QEBAXAEAVCStringList___Z(const void* pThis, void* listOfNames);
extern "C" int   MS_ABI impl__GetViewIdByName_CMouseManager__QEBAHPEB_W_Z(const void* pThis, const wchar_t* lpszName);
extern "C" unsigned int MS_ABI impl__GetViewIconId_CMouseManager__QEBAIH_Z(const void* pThis, int iViewId);
extern "C" unsigned int MS_ABI impl__GetViewDblClickCommand_CMouseManager__QEBAIH_Z(const void* pThis, int iId);
extern "C" void  MS_ABI impl__SetCommandForDblClk_CMouseManager__QEAAXHI_Z(void* pThis, int iViewId, unsigned int uiCmd);

// core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];

// This file's own thunks that earlier bodies call.
extern "C" void* MS_ABI impl___0CMFCMousePropertyPage__QEAA_XZ(void* pThis);
extern "C" void  MS_ABI impl__EnableDblClkControls_CMFCMousePropertyPage__IEAAXH_Z(void* pThis, int bEnable);
extern "C" int   MS_ABI impl__SelectCommand_CMFCMousePropertyPage__IEAAHI_Z(void* pThis, unsigned int uiCmd);

namespace {

// ---------------------------------------------------------------------------
// Retail layout (0x430 bytes); see the file header for the ctor stores.
// ---------------------------------------------------------------------------
struct S_MousePage {
    alignas(8) unsigned char m_base[0x158];                 // +0x000 CPropertyPage
    int                      m_iCurrViewId;                 // +0x158 (ctor: movl $0xffffffff,0x158)
    int                      m_pad15c;                      // +0x15c
    alignas(8) unsigned char m_ViewsImages[0x10];           // +0x160 CImageList { vfptr, m_hImageList (+0x168) }
    alignas(8) unsigned char m_wndListOfViews[0xe8];        // +0x170 CListCtrl (DDX IDC_AFXBARRES_LIST_VIEWS 16631)
    alignas(8) unsigned char m_wndListOfCommands[0xe8];     // +0x258 CListBox  (DDX IDC_AFXBARRES_LIST_OF_COMMANDS 16634)
    alignas(8) unsigned char m_wndCommandDescription[0xe8]; // +0x340 CStatic   (DDX IDC_AFXBARRES_COMMAND_DESCRIPTION 16514)
    CString                  m_strCommandDescription;       // +0x428 (DDX_Text 16514)
};
static_assert(offsetof(S_MousePage, m_iCurrViewId) == 0x158, "ctor: movl $0xffffffff,0x158(%rdi)");
static_assert(offsetof(S_MousePage, m_ViewsImages) == 0x160, "ctor: CImageList vftable at 0x160, NULL at 0x168");
static_assert(offsetof(S_MousePage, m_wndListOfViews) == 0x170, "ctor: ??0CWnd on this+0x170; DDX_Control 16631");
static_assert(offsetof(S_MousePage, m_wndListOfCommands) == 0x258, "ctor: ??0CWnd on this+0x258; DDX_Control 16634");
static_assert(offsetof(S_MousePage, m_wndCommandDescription) == 0x340, "ctor: ??0CWnd on this+0x340; DDX_Control 16514");
static_assert(offsetof(S_MousePage, m_strCommandDescription) == 0x428, "ctor: CString at 0x428; DDX_Text 16514");
static_assert(sizeof(S_MousePage) == 0x430, "CreateObject passes 0x430 to operator new");
static_assert(sizeof(CPropertyPage) == 0x158, "OpenMFC CPropertyPage fills exactly the retail base block");
static_assert(sizeof(CListCtrl) <= 0xe8 && sizeof(CListBox) <= 0xe8 && sizeof(CStatic) <= 0xe8, "controls fit their 0xe8-byte slots");
static_assert(sizeof(CImageList) <= 0x10 && offsetof(CImageList, m_hImageList) == 0x8,
              "retail reads m_ViewsImages.m_hImageList at this+0x168");
static_assert(sizeof(CString) == 8, "retail CString is one pointer");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads each control's HWND at member+0x40 (0x1b0 / 0x298)");
static_assert(offsetof(NMLISTVIEW, uChanged) == 0x28, "OnItemchangedListViews: cmpl $0x8,0x28(%rdx)");
static_assert(offsetof(IMAGEINFO, rcImage) == 0x18, "OnInitDialog: CopyRect from info+0x18");

// Resource ids (afxribbonres.h, 14.51 SDK) as the retail bodies pass them.
constexpr unsigned int kIddPropPage4          = 16135;   // 0x3f07 IDD_AFXBARRES_PROPPAGE4
constexpr int          kIdcListViews          = 16631;   // 0x40f7 IDC_AFXBARRES_LIST_VIEWS
constexpr int          kIdcNoDblClk           = 16632;   // 0x40f8 IDC_AFXBARRES_NO_DBLCLIICK
constexpr int          kIdcUseDblClk          = 16633;   // 0x40f9 IDC_AFXBARRES_USE_DBLCLIICK
constexpr int          kIdcListOfCommands     = 16634;   // 0x40fa IDC_AFXBARRES_LIST_OF_COMMANDS
constexpr int          kIdcCommandDescription = 16514;   // 0x4082 IDC_AFXBARRES_COMMAND_DESCRIPTION

// The retail ctor passes sizeof(PROPSHEETPAGE) = 0x68 as CPropertyPage's dwSize.
constexpr unsigned long kPropSheetPageSize = 0x68;

// AFX_GLOBAL_DATA: the one-time init gate at +0 and m_sizeSmallIcon at
// +0x1a0/+0x1a4 (pinned in core/runtime/AFX_GLOBAL_DATA.cpp).  OnInitDialog
// reads mfc140 0x1803ba520 / 0x1803ba524 = afxGlobalData (0x1803ba380, the
// address its inlined gate tests) + 0x1a0 / +0x1a4.
constexpr std::size_t kGdInitGate        = 0x000;
constexpr std::size_t kGdSizeSmallIconCx = 0x1a0;
constexpr std::size_t kGdSizeSmallIconCy = 0x1a4;

inline S_MousePage* D(void* p) { return static_cast<S_MousePage*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline HWND HwndOf(const void* pWnd) { return static_cast<const CWnd*>(pWnd)->m_hWnd; }   // member +0x40
inline CListCtrl* ListCtrl(S_MousePage* d) { return reinterpret_cast<CListCtrl*>(d->m_wndListOfViews); }
inline CImageList* Images(S_MousePage* d) { return reinterpret_cast<CImageList*>(d->m_ViewsImages); }

// ENSURE(x): retail throws through ?AfxThrowInvalidArgException@@.
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

int GdInt(std::size_t off) {
    int v = 0;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}
// Retail's inlined gate: if (afxGlobalData.<+0> == 0) { Initialize(); <+0> = 1; }
void EnsureGlobalData() {
    if (GdInt(kGdInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGdInitGate, &one, sizeof one);
    }
}

// The command description, as SelectCommand and OnSelchangeListOfCommands
// both inline it:
//     CFrameWnd* pParent = GetParentFrame();
//     if (pParent != NULL && pParent->GetSafeHwnd() != NULL)   // cmpq $0,0x40(%rax)
//         pParent->GetMessageString(uiCmd, m_strCommandDescription);   // vslot 95 (+0x2f8)
//     else
//         m_strCommandDescription.Empty();
// GetMessageString is deviation (4).
void LoadCommandDescription(void* pThis, unsigned int uiCmd) {
    S_MousePage* d = D(pThis);
    CFrameWnd* pParent = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(Wnd(pThis));
    if (pParent != nullptr && pParent->m_hWnd != nullptr) {
        impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pParent, uiCmd, &d->m_strCommandDescription);
    } else {
        d->m_strCommandDescription.Empty();
    }
}

// CListCtrl::GetItemText(int nItem, int nSubItem) as retail's body (RVA
// 0x295610 (mfc140u), the wide-string overload OnInitDialog calls) runs it:
// start from a 128-character length, double it, fetch with LVM_GETITEMTEXTW
// (0x1073) into a buffer of that length, and repeat while the returned length
// reaches nLen - 1.  Written out here rather than called through OpenMFC's
// by-value-CString thunk, whose (pRet, pThis) parameter order is reversed
// relative to the MSVC ABI (retail passes this in RCX, the return slot in RDX).
void ListCtrlItemText(HWND hWnd, int nItem, int nSubItem, CString& str) {
    int nLen = 128;
    int nRes;
    do {
        nLen *= 2;
        LVITEMW lvi = {};
        lvi.iSubItem = nSubItem;
        lvi.cchTextMax = nLen;
        lvi.pszText = str.GetBuffer(nLen);
        nRes = static_cast<int>(::SendMessage(hWnd, LVM_GETITEMTEXT, static_cast<WPARAM>(nItem), reinterpret_cast<LPARAM>(&lvi)));
        str.ReleaseBuffer(-1);
    } while (nRes >= nLen - 1);
}

// OpenMFC's CStringList keeps its nodes in CList<CString>'s node layout
// (include/openmfc/afx.h: CNode { pNext, pPrev, data }), reached through the
// exported FindIndex thunk -- the walk featurepack/toolbar/
// CMFCToolBarComboBoxButton.cpp makes over the same object.
struct StrNode {
    StrNode* pNext;
    StrNode* pPrev;
    CString  data;
};
static_assert(sizeof(CStringList::POSITION) == sizeof(void*), "POSITION is one pointer");
inline StrNode* FirstNode(const CStringList* pList) {
    CStringList::POSITION pos = impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(pList, 0);
    StrNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof p);
    return p;
}

// A stack CStringList built and destroyed through the exported thunks.
class ScratchStringList {
public:
    ScratchStringList() {
        std::memset(m_buf, 0, sizeof m_buf);
        impl___0CStringList__QEAA__J_Z(List(), 10);
    }
    ~ScratchStringList() { Destroy(); }
    // Idempotent, so a caller about to throw can release the list first.
    void Destroy() {
        if (m_live) impl___1CStringList__UEAA_XZ(List());
        m_live = false;
    }
    CStringList* List() { return reinterpret_cast<CStringList*>(m_buf); }
    ScratchStringList(const ScratchStringList&) = delete;
    ScratchStringList& operator=(const ScratchStringList&) = delete;
private:
    alignas(8) unsigned char m_buf[sizeof(CStringList)];
    bool m_live = true;
};

} // namespace

// Retail (RVA 0x91270, mfc140u), fully transcribed:
//     CPropertyPage::CPropertyPage(IDD_AFXBARRES_PROPPAGE4 /*0x3f07*/, 0, 0x68);
//     <store class vftable>
//     m_ViewsImages: CImageList vftable at +0x160, m_hImageList (+0x168) = NULL   // inlined ctor
//     m_wndListOfViews, m_wndListOfCommands, m_wndCommandDescription:
//         CWnd::CWnd() + CListCtrl / CListBox / CStatic vftable, in that order
//     m_strCommandDescription: nil string, then SetString(L"", 0)            // the literal is empty
//     m_iCurrViewId = -1;
// DEVIATIONS: the vftable stores are deviation (1); m_ViewsImages is zero-
// filled before the CImageList ctor thunk so its vfptr slot is not left as
// allocation garbage.  Assigning L"" to an empty CString leaves it empty, so
// the CString is simply default-constructed.  This export was not on this
// file's work list (its previous body only returned pThis), but the
// destructor tears down exactly these members and CMFCToolBarsCustomizeDialog
// constructs and destroys the page through this pair, so both are
// implemented together.
// Symbol: ??0CMFCMousePropertyPage@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCMousePropertyPage__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    S_MousePage* d = D(pThis);
    impl___0CPropertyPage__QEAA_IIK_Z(pThis, kIddPropPage4, 0, kPropSheetPageSize);
    std::memset(d->m_ViewsImages, 0, sizeof d->m_ViewsImages);
    impl___0CImageList__QEAA_XZ(Images(d));
    impl___0CWnd__QEAA_XZ(d->m_wndListOfViews);
    impl___0CWnd__QEAA_XZ(d->m_wndListOfCommands);
    impl___0CWnd__QEAA_XZ(d->m_wndCommandDescription);
    new (&d->m_strCommandDescription) CString();
    d->m_iCurrViewId = -1;
    return pThis;
}

// Retail (RVA 0x913c0, mfc140u), fully transcribed:
//     <store class vftable>
//     m_strCommandDescription.~CString();         // inline CStringData release
//     m_wndCommandDescription.~CStatic();         // 0x293b30
//     m_wndListOfCommands.~CListBox();            // 0x293f30
//     m_wndListOfViews.~CListCtrl();              // 0x295210
//     m_ViewsImages.~CImageList();                // 0x296be0
//     CPropertyPage::~CPropertyPage();            // tail jump 0x215f40
// The vftable store is deviation (1).
// Symbol: ??1CMFCMousePropertyPage@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCMousePropertyPage__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MousePage* d = D(pThis);
    d->m_strCommandDescription.~CString();
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndCommandDescription));
    impl___1CListBox__UEAA_XZ(reinterpret_cast<CListBox*>(d->m_wndListOfCommands));
    impl___1CListCtrl__UEAA_XZ(ListCtrl(d));
    impl___1CImageList__UEAA_XZ(Images(d));
    impl___1CPropertyPage__UEAA_XZ(static_cast<CPropertyPage*>(pThis));
}

// Retail (RVA 0x91230, mfc140u), fully transcribed:
//     void* p = operator new(0x430);              // 0x27f0
//     return p ? new (p) CMFCMousePropertyPage : NULL;   // ctor 0x91270
// Symbol: ?CreateObject@CMFCMousePropertyPage@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCMousePropertyPage__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(sizeof(S_MousePage));
    if (p == nullptr) return nullptr;
    return impl___0CMFCMousePropertyPage__QEAA_XZ(p);
}

// Retail (RVA 0x91440, mfc140u), fully transcribed:
//     DDX_Control(pDX, IDC_AFXBARRES_LIST_VIEWS,          m_wndListOfViews);        // +0x170
//     DDX_Control(pDX, IDC_AFXBARRES_LIST_OF_COMMANDS,    m_wndListOfCommands);     // +0x258
//     DDX_Control(pDX, IDC_AFXBARRES_COMMAND_DESCRIPTION, m_wndCommandDescription); // +0x340
//     DDX_Text   (pDX, IDC_AFXBARRES_COMMAND_DESCRIPTION, m_strCommandDescription); // +0x428, tail jump
// Symbol: ?DoDataExchange@CMFCMousePropertyPage@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCMousePropertyPage__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_MousePage* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcListViews, d->m_wndListOfViews);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcListOfCommands, d->m_wndListOfCommands);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcCommandDescription, d->m_wndCommandDescription);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pDX, kIdcCommandDescription, &d->m_strCommandDescription);
}

// Retail (RVA 0x91c30, mfc140u), fully transcribed:
//     m_wndListOfCommands.EnableWindow(bEnable);                 // 0x2a9b30
//     m_wndCommandDescription.EnableWindow(bEnable);
//     if (!bEnable) {
//         m_wndListOfCommands.SetCurSel(-1);                      // SendMessage LB_SETCURSEL (0x186), -1
//         m_strCommandDescription.Empty();
//         UpdateData(FALSE);                                      // 0x2910d0 -- deviation (2)
//     }
// Symbol: ?EnableDblClkControls@CMFCMousePropertyPage@@IEAAXH@Z
extern "C" void MS_ABI impl__EnableDblClkControls_CMFCMousePropertyPage__IEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    S_MousePage* d = D(pThis);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndListOfCommands), bEnable);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndCommandDescription), bEnable);
    if (!bEnable) {
        ::SendMessage(HwndOf(d->m_wndListOfCommands), LB_SETCURSEL, static_cast<WPARAM>(-1), 0);
        d->m_strCommandDescription.Empty();
        impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);
    }
}

// Retail (RVA 0x914c0, mfc140u), fully transcribed:
//     afxMouseManager->SetCommandForDblClk(m_iCurrViewId, 0);    // 0x90ce0, no ENSURE
//     EnableDblClkControls(FALSE);                               // tail jump 0x91c30
// afxMouseManager is deviation (3).
// Symbol: ?OnNoDblcliick@CMFCMousePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnNoDblcliick_CMFCMousePropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__SetCommandForDblClk_CMouseManager__QEAAXHI_Z(OpenMfcGetAfxMouseManager(), D(pThis)->m_iCurrViewId, 0);
    impl__EnableDblClkControls_CMFCMousePropertyPage__IEAAXH_Z(pThis, FALSE);
}

// Retail (RVA 0x914f0, mfc140u), fully transcribed:
//     m_wndListOfCommands.EnableWindow(TRUE);                    // EnableDblClkControls(TRUE), inlined:
//     m_wndCommandDescription.EnableWindow(TRUE);                //   the !bEnable arm folds away
//     if (m_iCurrViewId < 0) { MessageBeep((UINT)-1); return; }  // tail jump through the import
//     afxMouseManager->SetCommandForDblClk(m_iCurrViewId, 0);    // tail jump 0x90ce0, no ENSURE
// afxMouseManager is deviation (3).
// Symbol: ?OnUseDblcliick@CMFCMousePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnUseDblcliick_CMFCMousePropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_MousePage* d = D(pThis);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndListOfCommands), TRUE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndCommandDescription), TRUE);
    if (d->m_iCurrViewId < 0) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }
    impl__SetCommandForDblClk_CMouseManager__QEAAXHI_Z(OpenMfcGetAfxMouseManager(), d->m_iCurrViewId, 0);
}

// Retail (RVA 0x91550, mfc140u), fully transcribed:
//     ENSURE(afxMouseManager != NULL);                           // AfxThrowInvalidArgException
//     if (m_iCurrViewId < 0) { MessageBeep((UINT)-1); return; }
//     int iSel = m_wndListOfCommands.GetCurSel();                // LB_GETCURSEL (0x188); not checked for LB_ERR
//     UINT uiCmd = (UINT)m_wndListOfCommands.GetItemData(iSel);  // LB_GETITEMDATA (0x199)
//     <command description, see LoadCommandDescription>          // GetParentFrame 0x28e200
//     afxMouseManager->SetCommandForDblClk(m_iCurrViewId, uiCmd);   // 0x90ce0
//     UpdateData(FALSE);                                         // 0x2910d0 -- deviation (2)
// Symbol: ?OnSelchangeListOfCommands@CMFCMousePropertyPage@@IEAAXXZ
extern "C" void MS_ABI impl__OnSelchangeListOfCommands_CMFCMousePropertyPage__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    void* pMouseManager = OpenMfcGetAfxMouseManager();
    if (pMouseManager == nullptr) { ThrowInvalidArg(); return; }
    S_MousePage* d = D(pThis);
    if (d->m_iCurrViewId < 0) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }
    const HWND hCommands = HwndOf(d->m_wndListOfCommands);
    const int iSel = static_cast<int>(::SendMessage(hCommands, LB_GETCURSEL, 0, 0));
    const unsigned int uiCmd = static_cast<unsigned int>(
        ::SendMessage(hCommands, LB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(iSel)), 0));
    LoadCommandDescription(pThis, uiCmd);
    impl__SetCommandForDblClk_CMouseManager__QEAAXHI_Z(pMouseManager, d->m_iCurrViewId, uiCmd);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);
}

// Retail (RVA 0x91630, mfc140u), fully transcribed:
//     ENSURE(afxMouseManager != NULL);  ENSURE(pNMHDR != NULL);  // both -> AfxThrowInvalidArgException
//     *pResult = 0;                                              // pResult not checked
//     if (((NM_LISTVIEW*)pNMHDR)->uChanged != LVIF_STATE) return;   // +0x28 != 8
//     for (int i = 0; i < m_wndListOfViews.GetItemCount(); i++) {  // LVM_GETITEMCOUNT (0x1004), re-read each pass
//         if (m_wndListOfViews.GetItemState(i, LVIF_STATE | LVIS_SELECTED) & LVIS_SELECTED) {   // LVM_GETITEMSTATE (0x102c), mask 0xa, bit 2
//             m_iCurrViewId = (int)m_wndListOfViews.GetItemData(i);   // 0x295790
//             UINT uiCmd = afxMouseManager->GetViewDblClickCommand(m_iCurrViewId);   // 0x90560
//             if (uiCmd == 0) {
//                 CheckDlgButton(IDC_AFXBARRES_USE_DBLCLIICK, 0);    // 0x2a9320
//                 CheckDlgButton(IDC_AFXBARRES_NO_DBLCLIICK, 1);
//                 EnableDblClkControls(FALSE);                     // 0x91c30
//             } else {
//                 CheckDlgButton(IDC_AFXBARRES_USE_DBLCLIICK, 1);
//                 CheckDlgButton(IDC_AFXBARRES_NO_DBLCLIICK, 0);
//                 m_wndListOfCommands.EnableWindow(TRUE);          // EnableDblClkControls(TRUE), inlined
//                 m_wndCommandDescription.EnableWindow(TRUE);
//                 SelectCommand(uiCmd);                            // 0x91ca0, result unused
//             }
//             return;
//         }
//     }
// The inlined EnableDblClkControls(TRUE) is written as the call (it performs
// exactly those two EnableWindow calls when bEnable is TRUE).
// Symbol: ?OnItemchangedListViews@CMFCMousePropertyPage@@IEAAXPEAUtagNMHDR@@PEA_J@Z
extern "C" void MS_ABI impl__OnItemchangedListViews_CMFCMousePropertyPage__IEAAXPEAUtagNMHDR__PEA_J_Z(void* pThis, NMHDR* pNMHDR, LRESULT* pResult) {
    if (pThis == nullptr) return;
    void* pMouseManager = OpenMfcGetAfxMouseManager();
    if (pMouseManager == nullptr || pNMHDR == nullptr) { ThrowInvalidArg(); return; }
    *pResult = 0;
    if (reinterpret_cast<const NMLISTVIEW*>(pNMHDR)->uChanged != LVIF_STATE) return;

    S_MousePage* d = D(pThis);
    const HWND hViews = HwndOf(d->m_wndListOfViews);
    for (int i = 0; i < static_cast<int>(::SendMessage(hViews, LVM_GETITEMCOUNT, 0, 0)); i++) {
        const UINT state = static_cast<UINT>(::SendMessage(hViews, LVM_GETITEMSTATE, static_cast<WPARAM>(static_cast<INT_PTR>(i)),
                                                           LVIF_STATE | LVIS_SELECTED));
        if ((state & LVIS_SELECTED) == 0) continue;

        d->m_iCurrViewId = static_cast<int>(impl__GetItemData_CListCtrl__QEBA_KH_Z(ListCtrl(d), i));
        const unsigned int uiCmd = impl__GetViewDblClickCommand_CMouseManager__QEBAIH_Z(pMouseManager, d->m_iCurrViewId);
        if (uiCmd == 0) {
            impl__CheckDlgButton_CWnd__QEAAXHI_Z(Wnd(pThis), kIdcUseDblClk, 0);
            impl__CheckDlgButton_CWnd__QEAAXHI_Z(Wnd(pThis), kIdcNoDblClk, 1);
            impl__EnableDblClkControls_CMFCMousePropertyPage__IEAAXH_Z(pThis, FALSE);
        } else {
            impl__CheckDlgButton_CWnd__QEAAXHI_Z(Wnd(pThis), kIdcUseDblClk, 1);
            impl__CheckDlgButton_CWnd__QEAAXHI_Z(Wnd(pThis), kIdcNoDblClk, 0);
            impl__EnableDblClkControls_CMFCMousePropertyPage__IEAAXH_Z(pThis, TRUE);
            (void)impl__SelectCommand_CMFCMousePropertyPage__IEAAHI_Z(pThis, uiCmd);
        }
        return;
    }
}

// Retail (RVA 0x91ca0, mfc140u), fully transcribed:
//     <command description, see LoadCommandDescription>          // GetParentFrame 0x28e200
//     UpdateData(FALSE);                                         // 0x2910d0 -- deviation (2)
//     for (int i = 0; i < m_wndListOfCommands.GetCount(); i++) {   // LB_GETCOUNT (0x18b), re-read each pass
//         if (uiCmd == (UINT)m_wndListOfCommands.GetItemData(i)) {   // LB_GETITEMDATA (0x199), 32-bit compare
//             m_wndListOfCommands.SetCurSel(i);                  // LB_SETCURSEL (0x186)
//             m_wndListOfCommands.SetTopIndex(i);                // LB_SETTOPINDEX (0x197)
//             return TRUE;
//         }
//     }
//     return FALSE;
// Symbol: ?SelectCommand@CMFCMousePropertyPage@@IEAAHI@Z
extern "C" int MS_ABI impl__SelectCommand_CMFCMousePropertyPage__IEAAHI_Z(void* pThis, unsigned int uiCmd) {
    if (pThis == nullptr) return FALSE;
    S_MousePage* d = D(pThis);
    LoadCommandDescription(pThis, uiCmd);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), FALSE);

    const HWND hCommands = HwndOf(d->m_wndListOfCommands);
    for (int i = 0; i < static_cast<int>(::SendMessage(hCommands, LB_GETCOUNT, 0, 0)); i++) {
        const WPARAM wIndex = static_cast<WPARAM>(static_cast<INT_PTR>(i));
        if (uiCmd == static_cast<unsigned int>(::SendMessage(hCommands, LB_GETITEMDATA, wIndex, 0))) {
            ::SendMessage(hCommands, LB_SETCURSEL, wIndex, 0);
            ::SendMessage(hCommands, LB_SETTOPINDEX, wIndex, 0);
            return TRUE;
        }
    }
    return FALSE;
}

// Retail (RVA 0x91770, mfc140u), transcribed:
//     ENSURE(afxMouseManager != NULL);
//     CPropertyPage::OnInitDialog();              // resolves to CDialog::OnInitDialog (0x208d10); result unused
//     CStringList listOfViewNames;
//     afxMouseManager->GetViewNames(listOfViewNames);            // 0x905a0
//     m_ViewsImages.Create(afxGlobalData.m_sizeSmallIcon.cx, afxGlobalData.m_sizeSmallIcon.cy,
//                          ILC_MASK, (int)listOfViewNames.GetCount(), 1);   // 0x296d10, gate + Initialize 0x6a790
//     m_wndListOfViews.SetImageList(&m_ViewsImages, LVSIL_SMALL);   // LVM_SETIMAGELIST (0x1003), then
//                                                                //   CImageList::FromHandle 0x296cb0 on the result
//     CRect rectList;  m_wndListOfViews.GetClientRect(rectList);   // ::GetClientRect on +0x1b0
//     m_wndListOfViews.InsertColumn(0, _T(""), LVCFMT_LEFT, rectList.Width() - 1, -1);   // 0x295390
//     int iMaxWidth = 0;
//     for (POSITION pos = listOfViewNames.GetHeadPosition(); pos != NULL;) {
//         CString strViewName = listOfViewNames.GetNext(pos);
//         int iImageIndex = -1;
//         UINT uiViewIconId = afxMouseManager->GetViewIconId(afxMouseManager->GetViewIdByName(strViewName));
//         if (uiViewIconId != 0) {
//             HICON hViewIcon = ::LoadIcon(AfxFindResourceHandle(MAKEINTRESOURCE(uiViewIconId), RT_GROUP_ICON),
//                                          MAKEINTRESOURCE(uiViewIconId));   // 0x2aeb50
//             if (hViewIcon != NULL) {
//                 iImageIndex = ImageList_ReplaceIcon(m_ViewsImages.m_hImageList, -1, hViewIcon);   // m_ViewsImages.Add
//                 ::DestroyIcon(hViewIcon);
//             }
//         }
//         int iIndex = m_wndListOfViews.GetItemCount();          // LVM_GETITEMCOUNT
//         for (int i = 0; i < m_wndListOfViews.GetItemCount(); i++) {   // re-read each pass
//             CString strText = m_wndListOfViews.GetItemText(i, 0);   // 0x295610
//             if (strText.Compare(strViewName) > 0) { iIndex = i; break; }   // ENSURE psz != NULL (AtlThrow E_FAIL)
//         }
//         m_wndListOfViews.InsertItem(LVIF_TEXT | LVIF_IMAGE, iIndex, strViewName, 0, 0, iImageIndex, 0);   // 0x295400, result unused
//         m_wndListOfViews.SetItemData(iIndex, afxMouseManager->GetViewIdByName(strViewName));
//                                                                //   inlined as SetItem(iIndex, 0, LVIF_PARAM, NULL, 0, 0, 0, (UINT)id)
//         iMaxWidth = max(iMaxWidth, m_wndListOfViews.GetStringWidth(strViewName));   // LVM_GETSTRINGWIDTHW (0x1057 in
//                                                                //   mfc140u; the ANSI twin sends 0x1011)
//     }
//     IMAGEINFO info;  ImageList_GetImageInfo(m_ViewsImages.m_hImageList, 0, &info);
//     CRect rectImage = info.rcImage;                            // ::CopyRect
//     m_wndListOfViews.SetColumnWidth(0, iMaxWidth + rectImage.Width() + 10);   // LVM_SETCOLUMNWIDTH (0x101e), 16-bit sum
//     CMFCToolBarsCustomizeDialog* pWndParent = DYNAMIC_DOWNCAST(CMFCToolBarsCustomizeDialog, GetParent());
//                                                                //   ::GetParent, FromHandle 0x28ad70, IsKindOf 0x234cf0
//     ENSURE(pWndParent != NULL);
//     pWndParent->FillAllCommandsList(m_wndListOfCommands);      // dialog vslot 97 (+0x308)
//     m_wndListOfViews.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);   // LVM_SETITEMSTATE (0x102b)
//     m_wndListOfViews.EnsureVisible(0, FALSE);                  // LVM_ENSUREVISIBLE (0x1013)
//     return TRUE;                                               // ~CStringList on the way out
// Retail also calls AfxGetModuleState (0x133930) just before
// AfxFindResourceHandle and discards the result; that call is not reproduced.
// DEVIATIONS:
//  * afxMouseManager is deviation (3); GetParent's FillAllCommandsList is
//    deviation (4).  For a customize dialog OpenMFC itself constructed the
//    downcast yields NULL (see CMFCToolBarsKeyboardPropertyPage.cpp
//    deviation (5)), so this ENSURE throws there, as retail's would for a
//    non-customize parent.
//  * The name list is walked over OpenMFC's CStringList nodes (StrNode), and
//    each node's string is used in place rather than copied; the ImageList
//    count is that walk's length (retail reads m_nCount).
//  * The item text is fetched by ListCtrlItemText, a transcription of the
//    retail GetItemText(int, int) body, instead of through OpenMFC's by-value
//    CString thunk.  strViewName is never NULL, so the Compare ENSURE cannot
//    fire and is not reproduced.
//  * IMAGEINFO is zero-initialised; retail leaves it uninitialised when
//    ImageList_GetImageInfo fails (an empty image list), so there the column
//    width is computed from stack contents.
//  * On either ENSURE failure after the list exists, the scratch list is
//    destroyed explicitly before throwing.  The exception is raised through
//    the MSVC runtime's _CxxThrowException (detail/MfcExceptionsSupport.h);
//    whether that unwind runs this mingw-compiled frame's destructors was not
//    verified, so the release is not left to it.  Destroy() is idempotent, so
//    it is safe either way.
// Symbol: ?OnInitDialog@CMFCMousePropertyPage@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCMousePropertyPage__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    void* pMouseManager = OpenMfcGetAfxMouseManager();
    if (pMouseManager == nullptr) { ThrowInvalidArg(); return FALSE; }
    S_MousePage* d = D(pThis);

    (void)impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));

    ScratchStringList names;
    impl__GetViewNames_CMouseManager__QEBAXAEAVCStringList___Z(pMouseManager, names.List());

    int nNames = 0;
    for (StrNode* p = FirstNode(names.List()); p != nullptr; p = p->pNext) ++nNames;

    EnsureGlobalData();
    impl__Create_CImageList__QEAAHHHIHH_Z(Images(d), GdInt(kGdSizeSmallIconCx), GdInt(kGdSizeSmallIconCy), ILC_MASK, nNames, 1);

    const HWND hViews = HwndOf(d->m_wndListOfViews);
    const HIMAGELIST hImages = Images(d)->m_hImageList;
    impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(reinterpret_cast<HIMAGELIST>(
        ::SendMessage(hViews, LVM_SETIMAGELIST, LVSIL_SMALL, reinterpret_cast<LPARAM>(hImages))));

    RECT rectList = {};
    ::GetClientRect(hViews, &rectList);
    impl__InsertColumn_CListCtrl__QEAAHHPEB_WHHH_Z(ListCtrl(d), 0, L"", LVCFMT_LEFT, (rectList.right - rectList.left) - 1, -1);

    int iMaxWidth = 0;
    for (StrNode* pNode = FirstNode(names.List()); pNode != nullptr; pNode = pNode->pNext) {
        const wchar_t* pszViewName = pNode->data.GetString();

        int iImageIndex = -1;
        const unsigned int uiViewIconId = impl__GetViewIconId_CMouseManager__QEBAIH_Z(
            pMouseManager, impl__GetViewIdByName_CMouseManager__QEBAHPEB_W_Z(pMouseManager, pszViewName));
        if (uiViewIconId != 0) {
            const wchar_t* lpszIcon = MAKEINTRESOURCEW(static_cast<WORD>(uiViewIconId));
            HICON hViewIcon = ::LoadIcon(impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszIcon, RT_GROUP_ICON), lpszIcon);
            if (hViewIcon != nullptr) {
                iImageIndex = ::ImageList_ReplaceIcon(Images(d)->m_hImageList, -1, hViewIcon);
                ::DestroyIcon(hViewIcon);
            }
        }

        int iIndex = static_cast<int>(::SendMessage(hViews, LVM_GETITEMCOUNT, 0, 0));
        for (int i = 0; i < static_cast<int>(::SendMessage(hViews, LVM_GETITEMCOUNT, 0, 0)); i++) {
            CString strText;
            ListCtrlItemText(hViews, i, 0, strText);
            if (std::wcscmp(strText.GetString(), pszViewName) > 0) {
                iIndex = i;
                break;
            }
        }

        impl__InsertItem_CListCtrl__QEAAHIHPEB_WIIH_J_Z(ListCtrl(d), LVIF_TEXT | LVIF_IMAGE, iIndex, pszViewName, 0, 0, iImageIndex, 0);
        const int iViewId = impl__GetViewIdByName_CMouseManager__QEBAHPEB_W_Z(pMouseManager, pszViewName);
        impl__SetItem_CListCtrl__QEAAHHHIPEB_WHII_J_Z(ListCtrl(d), iIndex, 0, LVIF_PARAM, nullptr, 0, 0, 0,
                                                     static_cast<LPARAM>(static_cast<unsigned int>(iViewId)));

        const int iWidth = static_cast<int>(::SendMessage(hViews, LVM_GETSTRINGWIDTH, 0, reinterpret_cast<LPARAM>(pszViewName)));
        if (iWidth > iMaxWidth) iMaxWidth = iWidth;
    }

    IMAGEINFO info = {};
    ::ImageList_GetImageInfo(Images(d)->m_hImageList, 0, &info);
    RECT rectImage;
    ::CopyRect(&rectImage, &info.rcImage);
    const WORD wColumnWidth = static_cast<WORD>(iMaxWidth - rectImage.left + 10 + rectImage.right);
    ::SendMessage(hViews, LVM_SETCOLUMNWIDTH, 0, MAKELPARAM(wColumnWidth, 0));

    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Wnd(pThis)->m_hWnd));
    if (pParent == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CMFCToolBarsCustomizeDialog__SAPEAUCRuntimeClass__XZ())) {
        names.Destroy();
        ThrowInvalidArg();
        return TRUE;
    }
    impl__FillAllCommandsList_CMFCToolBarsCustomizeDialog__UEBAXAEAVCListBox___Z(pParent, Wnd(d->m_wndListOfCommands));

    LVITEMW lvi = {};
    lvi.state = LVIS_SELECTED | LVIS_FOCUSED;
    lvi.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
    ::SendMessage(hViews, LVM_SETITEMSTATE, 0, reinterpret_cast<LPARAM>(&lvi));
    ::SendMessage(hViews, LVM_ENSUREVISIBLE, 0, FALSE);

    return TRUE;   // names destroyed here
}
