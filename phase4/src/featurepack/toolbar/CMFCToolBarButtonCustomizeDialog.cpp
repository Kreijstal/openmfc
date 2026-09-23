// CMFCToolBarButtonCustomizeDialog — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ===========================================================================
// CMFCToolBarButtonCustomizeDialog -- the toolbar-button "Button Appearance"
// dialog (retail afxtoolbarbuttoncustomizedialog.h, class : public CDialog).
//
// Every implemented body below was transcribed from the retail disassembly
// (the method described in the header of core/ole/COleControl.cpp);
// OnAddImage and OnEditImage are stubs whose retail bodies are transcribed
// only in comments.  mfc140u_rva_symbols.json has an entry for only six of
// these exports (ctor, dtor, DoDataExchange, EnableControls, OnAddImage,
// OnDefaultImage), but every one of the sixteen resolves directly through the
// mfc140u.dll export table joined with mfc_complete_ordinal_mapping.json (the
// map annu.py builds), and each such entry is the mfc140.dll entry + 0x19b0.
// Every RVA below is an mfc140u function entry (the scalar deleting
// destructor has no export; it is slot 1 of the class vftable, whose slot was
// read from mfc140u):
//
//   ctor               0x15eae0     OnImageList      0x15f890
//   dtor               0x15ee10     OnImage          0x15f8f0
//   scalar-deleting    0x15edc0     OnImageText      0x15f900
//     (calls the dtor, then `mov $0x968,%edx` before its sized delete)
//   DoDataExchange     0x15ef60     OnText           0x15f920
//   OnAddImage         0x15f040     OnOK             0x15f940
//   OnEditImage        0x15f400     OnInitDialog     0x15fbc0
//   RebuildImageList   0x15ff40     EnableControls   0x160020
//   OnUserImage        0x1601a0     OnDefaultImage   0x1601f0
//   OnPaint            0x160250
//
// Callees are named, not addressed: each call target was resolved through
// mfc140_rva_symbols.json against the mfc140.dll body, and each import slot
// with iat.py (the ANSI image names the *A twin of the USER32/GDI32 imports
// that have one; the calls below use the UNICODE macro form).
//
// The class is NOT declared in OpenMFC's public headers, so this file is the
// only place its layout lives.  S_BtnCustDlg below pins it from the retail
// constructor, whose every store is quoted in the static_asserts; the object
// is 0x968 bytes (scalar deleting destructor).  The member names are those of
// the retail header, whose declaration order matches the offsets.
//
// Structural deviations, applied uniformly and named again where they bite:
//
//  (1) vtable pointers.  Retail stores its own vftable at +0x00 and the
//      CButton / CStatic / CEdit vftables in the plain control members after
//      ??0CWnd@@QEAA@XZ.  OpenMFC has no MSVC-layout vtable for any of these,
//      so +0x00 keeps whatever the CDialog constructor thunk leaves there and
//      the plain controls are built with the exported CWnd constructor alone
//      (exactly as featurepack/customize/CMFCKeyMapDialog.cpp does).  Virtual
//      calls on `this` are therefore made to this file's own thunks, and the
//      virtual calls on other objects (CMFCToolBarButton::SetImage / OnDraw,
//      CWinThread::GetMainWnd, CFrameWnd::GetMessageString) go to the exported
//      base-class thunks; an override in a derived class is NOT reached.
//
//  (2) OpenMFC's CObList keeps its nodes in a side table and never maintains
//      the retail m_nCount (+0x18 of the list, dialog +0x928) that retail's
//      "while (!m_Buttons.IsEmpty())" loops test.  Those loops test the head
//      through the exported ?FindIndex@CObList@@ thunk instead, as
//      CMFCToolBarButtonsListButton.cpp does.
//
//  (3) The exported datum ?afxUserToolsManager@@ (the one retail's
//      constructor reads, 0x3be3b0 mfc140u) is NULL for the whole life of this
//      tree: featurepack/CMFC_misc_stubs.cpp defines it and nothing assigns it.
//      (detail/MfccoreSupport.cpp has a separate, non-NULL C++ global of the
//      same source name that CWinAppEx::GetUserToolsManager returns; it is not
//      the export, has no retail member layout, and its exported FindTool thunk
//      treats the argument as an index rather than a command ID.)  So the
//      inlined afxUserToolsManager->FindTool() in the constructor is not
//      reproduced: m_pUserTool stays NULL, which is exactly what retail
//      computes when the exported pointer is NULL.  Every m_pUserTool branch
//      is still transcribed.
// ===========================================================================

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstdlib>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Thunks this file calls (briefing §1: C++ methods do not link here).  The
// file named on each line holds the definition that was read.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd);   // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis);                                                  // detail/DlgcoreSupport.cpp
extern "C" int   MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);                                       // detail/DlgcoreSupport.cpp
extern "C" void  MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* pThis);                                               // detail/DlgcoreSupport.cpp

extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                       // core/controls/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CStatic__UEAA_XZ(CStatic* pThis);                    // core/controls/RuntimeClasses.cpp
extern "C" void  MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);                        // core/controls/RuntimeClasses.cpp

extern "C" void* MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);        // core/collections/CObList.cpp
extern "C" void  MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);                                 // core/collections/CObList.cpp
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);   // core/collections/CObList.cpp
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);        // core/collections/CObList.cpp
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);   // core/collections/CObList.cpp

extern "C" void* MS_ABI impl___0CMFCToolBarButtonsListButton__QEAA_XZ(void* pThis);                                     // toolbar/CMFCToolBarButtonsListButton.cpp
extern "C" void  MS_ABI impl___1CMFCToolBarButtonsListButton__UEAA_XZ(void* pThis);                                     // toolbar/CMFCToolBarButtonsListButton.cpp
extern "C" void  MS_ABI impl__SetImages_CMFCToolBarButtonsListButton__QEAAXPEAVCMFCToolBarImages___Z(void* pThis, CMFCToolBarImages* pImages);   // toolbar/CMFCToolBarButtonsListButton.cpp
extern "C" void  MS_ABI impl__AddButton_CMFCToolBarButtonsListButton__QEAAXPEAVCMFCToolBarButton___Z(void* pThis, CMFCToolBarButton* pButton);   // toolbar/CMFCToolBarButtonsListButton.cpp
extern "C" void  MS_ABI impl__RemoveButtons_CMFCToolBarButtonsListButton__QEAAXXZ(void* pThis);                         // toolbar/CMFCToolBarButtonsListButton.cpp
extern "C" int   MS_ABI impl__SelectButton_CMFCToolBarButtonsListButton__QEAAHH_Z(void* pThis, int iImage);             // toolbar/CMFCToolBarButtonsListButton.cpp
extern "C" void  MS_ABI impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(void* pThis, CMFCToolBarButton* pButton);   // toolbar/CMFCToolBarButtonsListButton.cpp

extern "C" void* MS_ABI impl___0CMFCToolBarButton__QEAA_XZ(void* pThis);             // toolbar/Thunks.cpp
extern "C" void  MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage);   // toolbar/CMFCToolBarButton.cpp
// The definition in toolbar/CMFCToolBarButton.cpp is still an auto-generated
// empty placeholder whose parameter list has no `this`; this is the list the
// mangled name describes (the one CMFCToolBarButtonsListButton.cpp declares).
extern "C" void  MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarButton* pThis, CDC* pDC, const CRect* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons);

// CMFCToolBarImages.  PrepareDrawImage's definition in toolbar/CMFCToolBarImages.cpp
// is an auto-generated placeholder with no `this` (it returns 0 and touches
// nothing); this is the list the mangled name describes -- CSize by value is an
// 8-byte aggregate, modelled as long long exactly as controls/CMFCReBar.cpp does.
extern "C" int   MS_ABI impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(
    CMFCToolBarImages* pThis, void* pDS, long long sizeImageDest, int bFadeInactive);
extern "C" int   MS_ABI impl__Draw_CMFCToolBarImages__QEAAHPEAVCDC__HHHHHHHHE_Z(
    CMFCToolBarImages* pThis, CDC* pDC, int x, int y, int iImage, int bHilite, int bDisabled,
    int bIndeterminate, int bShadow, int bInactive, unsigned char alphaSrc);                  // toolbar/CMFCToolBarImages.cpp
extern "C" void  MS_ABI impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(CMFCToolBarImages* pThis, void* pDS);   // toolbar/CMFCToolBarImages.cpp

extern "C" void  MS_ABI impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(void* pThis, CDC* pDC, const CRect* pRect);   // customize/CUserTool.cpp

extern "C" void* MS_ABI impl__GetCmdMgr__YAPEAVCCommandManager__XZ();                // featurepack/CMFC_misc_stubs.cpp
// customize/CCommandManager.cpp still defines this as an auto-generated empty
// placeholder without `this`; this is the list the mangled name describes (the
// one menu/CMFCPopupMenuBar.cpp declares).
extern "C" void  MS_ABI impl__EnableMenuItemImage_CCommandManager__QEAAXIHH_Z(void* pThis, unsigned int uiCmd, int bEnable, int nUserImage);

extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);   // core/gdi/CPaintDC.cpp
extern "C" void  MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                              // core/gdi/CPaintDC.cpp

extern "C" void  MS_ABI impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp
extern "C" void  MS_ABI impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pDX, int nIDC, void* pv);   // core/runtime/DdxExchange.cpp

extern "C" int   MS_ABI impl__EnableWindow_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);                 // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);            // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__CheckDlgButton_CWnd__QEAAXHI_Z(CWnd* pThis, int nIDButton, unsigned int nCheck);   // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__UpdateData_CWnd__QEAAHH_Z(CWnd* pThis, int bSaveAndValidate);          // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                              // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);    // core/window/Thunks.cpp
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);                   // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CFrameWnd* pThis, unsigned int nID, CString* rMessage);   // core/frame/CFrameWnd.cpp
extern "C" CWinThread* MS_ABI impl__AfxGetThread__YAPEAVCWinThread__XZ();                           // core/app/Globals.cpp
extern "C" CWnd* MS_ABI impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(CWinThread* pThis);           // core/app/CWinThread.cpp

extern "C" int   MS_ABI impl__AfxMessageBox__YAHPEB_WII_Z(const wchar_t* lpszText, unsigned int nType, unsigned int nIDHelp);   // core/collections/Globals.cpp
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);   // featurepack/CMFC_misc_stubs.cpp
extern "C" int   MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);   // core/collections/CStringT.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                 // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowMemoryException__YAXXZ();                   // detail/MfcExceptionsSupport.cpp
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                     // detail/MemcoreSupport.cpp (??2@, malloc)
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);                             // detail/MemcoreSupport.cpp (??3@, free)

// Exported statics.
extern "C" CMFCToolBarImages impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;    // toolbar/CMFCToolBar.cpp
extern "C" std::int32_t impl__m_bDisableLabelsEdit_CMFCToolBar__2HA;                // toolbar/StaticData.cpp
// ?m_DefaultImages@CMFCToolBar@@ -- a CMap<UINT,UINT,int,int>, defined in
// toolbar/CMFCToolBar.cpp as an opaque 56-byte blob in the retail constructor
// state (m_nHashTableSize 17, m_nBlockSize 10, everything else 0).
extern "C" unsigned char impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A[56];

// This file's own exports that earlier bodies call (retail calls them directly;
// none of them is virtual).
extern "C" void MS_ABI impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RebuildImageList_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis);

namespace {

// ---------------------------------------------------------------------------
// The retail object (0x968 bytes).  Every offset is a store in the constructor
// (0x15eae0); the seven control offsets are also the DDX_Control arguments,
// and the two string offsets the DDX_Text arguments, in DoDataExchange.
// ---------------------------------------------------------------------------
struct S_BtnCustDlg {
    alignas(8) unsigned char m_base[0x130];                // +0x000 CDialog (OpenMFC sizeof(CDialog) == 0x130, asserted below)
    alignas(8) unsigned char m_wndDefautImageBtn[0xe8];    // +0x130 CButton (DDX 16530 IDC_AFXBARRES_DEFAULT_IMAGE; retail's spelling)
    alignas(8) unsigned char m_wndUserImageBtn[0xe8];      // +0x218 CButton (DDX 16529 IDC_AFXBARRES_USER_IMAGE)
    alignas(8) unsigned char m_wndDefaultImageArea[0xe8];  // +0x300 CStatic (DDX 16531 IDC_AFXBARRES_DEFAULT_IMAGE_AREA)
    alignas(8) unsigned char m_wndButtonText[0xe8];        // +0x3e8 CEdit   (DDX 16500 IDC_AFXBARRES_BUTTON_TEXT)
    alignas(8) unsigned char m_wndAddImage[0xe8];          // +0x4d0 CButton (DDX 16509 IDC_AFXBARRES_ADD_IMAGE)
    alignas(8) unsigned char m_wndButtonList[0x238];       // +0x5b8 CMFCToolBarButtonsListButton (DDX 16508 IDC_AFXBARRES_IMAGE_LIST)
    alignas(8) unsigned char m_wndEditImage[0xe8];         // +0x7f0 CButton (DDX 16510 IDC_AFXBARRES_EDIT_IMAGE)
    CString            m_strButtonText;                    // +0x8d8 (DDX_Text 16500)
    CString            m_strButtonDescr;                   // +0x8e0 (DDX_Text 16603 IDC_AFXBARRES_BUTTON_DESCR)
    CMFCToolBarButton* m_pButton;                          // +0x8e8
    CMFCToolBarImages* m_pImages;                          // +0x8f0
    int                m_iStartImage;                      // +0x8f8
    int                m_iSelImage;                        // +0x8fc
    BOOL               m_bImage;                           // +0x900
    BOOL               m_bText;                            // +0x904
    BOOL               m_bMenuMode;                        // +0x908
    BOOL               m_bUserButton;                      // +0x90c
    CObList            m_Buttons;                          // +0x910
    CRect              m_rectDefaultImage;                 // +0x948
    CString            m_strAccel;                         // +0x958
    void*              m_pUserTool;                        // +0x960 CUserTool*
};
static_assert(offsetof(S_BtnCustDlg, m_wndDefautImageBtn) == 0x130, "ctor: CWnd ctor on this+0x130");
static_assert(offsetof(S_BtnCustDlg, m_wndUserImageBtn) == 0x218, "ctor: CWnd ctor on this+0x218");
static_assert(offsetof(S_BtnCustDlg, m_wndDefaultImageArea) == 0x300, "ctor: CWnd ctor on this+0x300");
static_assert(offsetof(S_BtnCustDlg, m_wndButtonText) == 0x3e8, "ctor: CWnd ctor on this+0x3e8");
static_assert(offsetof(S_BtnCustDlg, m_wndAddImage) == 0x4d0, "ctor: CWnd ctor on this+0x4d0");
static_assert(offsetof(S_BtnCustDlg, m_wndButtonList) == 0x5b8, "ctor: CMFCToolBarButtonsListButton ctor on this+0x5b8");
static_assert(offsetof(S_BtnCustDlg, m_wndEditImage) == 0x7f0, "ctor: CWnd ctor on this+0x7f0");
static_assert(offsetof(S_BtnCustDlg, m_strButtonText) == 0x8d8, "ctor: nil-string store at +0x8d8");
static_assert(offsetof(S_BtnCustDlg, m_strButtonDescr) == 0x8e0, "ctor: nil-string store at +0x8e0");
static_assert(offsetof(S_BtnCustDlg, m_pButton) == 0x8e8, "ctor: mov %rbp,0x8e8(%rsi)");
static_assert(offsetof(S_BtnCustDlg, m_pImages) == 0x8f0, "ctor: mov %rdi,0x8f0(%rsi)");
static_assert(offsetof(S_BtnCustDlg, m_iStartImage) == 0x8f8, "ctor: iStartImage -> 0x8f8");
static_assert(offsetof(S_BtnCustDlg, m_iSelImage) == 0x8fc, "ctor: pButton->GetImage() -> 0x8fc");
static_assert(offsetof(S_BtnCustDlg, m_bImage) == 0x900, "ctor: pButton->m_bImage -> 0x900");
static_assert(offsetof(S_BtnCustDlg, m_bText) == 0x904, "ctor: -> 0x904");
static_assert(offsetof(S_BtnCustDlg, m_bMenuMode) == 0x908, "ctor: bMenuMode -> 0x908");
static_assert(offsetof(S_BtnCustDlg, m_bUserButton) == 0x90c, "ctor: -> 0x90c");
static_assert(offsetof(S_BtnCustDlg, m_Buttons) == 0x910, "ctor: CObList vftable -> 0x910, m_nBlockSize 10 -> 0x940");
static_assert(offsetof(S_BtnCustDlg, m_rectDefaultImage) == 0x948, "ctor: zero stores 0x948 / 0x950");
static_assert(offsetof(S_BtnCustDlg, m_strAccel) == 0x958, "ctor: nil-string store at +0x958");
static_assert(offsetof(S_BtnCustDlg, m_pUserTool) == 0x960, "ctor: mov %rdi,0x960(%rsi)");
static_assert(sizeof(S_BtnCustDlg) == 0x968, "scalar deleting dtor 0x15edc0: mov $0x968,%edx");
static_assert(sizeof(CDialog) == 0x130, "OpenMFC CDialog fills exactly the retail base block");
static_assert(sizeof(CWnd) == 0xe8 && sizeof(CButton) == 0xe8 && sizeof(CStatic) == 0xe8 && sizeof(CEdit) == 0xe8,
              "plain controls are bare CWnds");
static_assert(sizeof(CObList) == 0x38, "retail CObList is 56 bytes");
static_assert(sizeof(CString) == 8 && sizeof(CRect) == 16, "CString is one pointer, CRect four ints");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "every retail body reads the control HWNDs at member+0x40");
static_assert(offsetof(CRect, left) == 0 && offsetof(CRect, top) == 4 && offsetof(CRect, right) == 8 &&
              offsetof(CRect, bottom) == 12, "CRect has the RECT layout (InvalidateRect / GetClientRect take it)");

// CMFCToolBarButtonsListButton::m_pSelButton, read by OnImageList as dialog
// +0x7d0 (== +0x5b8 + 0x218).  That offset is pinned by the static_asserts in
// toolbar/CMFCToolBarButtonsListButton.cpp, which owns the list-button layout.
constexpr std::size_t kOffListSelButton = 0x218;

// CMFCToolBarButton members the retail bodies touch (declared by OpenMFC at the
// retail offsets, include/openmfc/afxmfc.h).
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "+0x08");
static_assert(offsetof(CMFCToolBarButton, m_bText) == 0x0c, "+0x0c");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == 0x10, "+0x10");
static_assert(offsetof(CMFCToolBarButton, m_bTextBelow) == 0x1c, "+0x1c");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "+0x24");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "+0x38");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == 0x48 && offsetof(CMFCToolBarButton, m_iUserImage) == 0x4c,
              "GetImage() inlined as `neg/sbb/and 4; mov 0x48(rax+button)`");
static_assert(offsetof(CMFCToolBarButton, m_bLocked) == 0x50, "+0x50");
static_assert(sizeof(CMFCToolBarButton) == 0x88, "RebuildImageList: operator new(0x88)");

// CMFCToolBarImages members read by offset (protected in include/openmfc/afxmfc.h,
// so reached through a derived probe type that is never instantiated).
struct ImagesProbe : public CMFCToolBarImages {
    static int Count(const CMFCToolBarImages* p) { return static_cast<const ImagesProbe*>(p)->m_iCount; }        // +0x08
    static BOOL ReadOnly(const CMFCToolBarImages* p) { return static_cast<const ImagesProbe*>(p)->m_bReadOnly; } // +0x28
    static SIZE ImageSize(const CMFCToolBarImages* p) {                                                          // +0x68
        const ImagesProbe* q = static_cast<const ImagesProbe*>(p);
        SIZE s; s.cx = q->m_sizeImage.cx; s.cy = q->m_sizeImage.cy; return s;
    }
    static void PinOffsets() {
        static_assert(offsetof(ImagesProbe, m_iCount) == 0x08, "RebuildImageList: mov 0x8(%rax),%esi");
        static_assert(offsetof(ImagesProbe, m_bReadOnly) == 0x28, "EnableControls / OnImageList: cmp 0x28(%rax)");
        static_assert(offsetof(ImagesProbe, m_sizeImage) == 0x68, "OnInitDialog reads CMFCToolBar::m_Images + 0x68");
    }
};

// Resource IDs (afxribbonres.h, 14.51 SDK on this host).
constexpr UINT kIddButtonProps       = 16132;   // IDD_AFXBARRES_BUTTON_PROPS (0x3f04)
constexpr int  kIdcImage             = 16505;   // IDC_AFXBARRES_IMAGE        (0x4079)
constexpr int  kIdcText              = 16506;   // IDC_AFXBARRES_TEXT         (0x407a)
constexpr int  kIdcImageText         = 16507;   // IDC_AFXBARRES_IMAGE_TEXT   (0x407b)
constexpr int  kIdcDefaultImage      = 16530;   // IDC_AFXBARRES_DEFAULT_IMAGE (0x4092)
constexpr int  kIdcUserImage         = 16529;   // IDC_AFXBARRES_USER_IMAGE   (0x4091)
constexpr int  kIdcDefaultImageArea  = 16531;   // IDC_AFXBARRES_DEFAULT_IMAGE_AREA (0x4093)
constexpr int  kIdcButtonText        = 16500;   // IDC_AFXBARRES_BUTTON_TEXT  (0x4074)
constexpr int  kIdcAddImage          = 16509;   // IDC_AFXBARRES_ADD_IMAGE    (0x407d)
constexpr int  kIdcImageList         = 16508;   // IDC_AFXBARRES_IMAGE_LIST   (0x407c)
constexpr int  kIdcEditImage         = 16510;   // IDC_AFXBARRES_EDIT_IMAGE   (0x407e)
constexpr int  kIdcButtonDescr       = 16603;   // IDC_AFXBARRES_BUTTON_DESCR (0x40db)
constexpr UINT kIdpImageIsRequired   = 16001;   // IDP_AFXBARRES_IMAGE_IS_REQUIRED (0x3e81)
constexpr UINT kIdpTextIsRequired    = 16002;   // IDP_AFXBARRES_TEXT_IS_REQUIRED  (0x3e82)

// GetCmdMgr() + 0xb0 is CCommandManager::m_lstCommandsWithoutImages (the
// retail layout featurepack/CMFC_misc_stubs.cpp documents for the singleton).
constexpr std::size_t kOffCmdMgrCommandsWithoutImages = 0xb0;

inline S_BtnCustDlg* D(void* p) { return static_cast<S_BtnCustDlg*>(p); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline HWND HwndOf(const void* pWnd) { return static_cast<const CWnd*>(pWnd)->m_hWnd; }   // member +0x40
inline RECT* AsRect(CRect* p) { return reinterpret_cast<RECT*>(p); }

// ENSURE(x) failure: retail calls ?AfxThrowInvalidArgException@@YAXXZ, which
// does not return.
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

template <typename T>
T ReadAt(const void* base, std::size_t off) {
    T v;
    std::memcpy(&v, static_cast<const unsigned char*>(base) + off, sizeof v);
    return v;
}

// CMFCToolBar::GetDefaultImage(uiID), which every body here inlines as
//     int iImage; if (!m_DefaultImages.Lookup(uiID, iImage)) iImage = -1;
// Lookup is the non-exported CMap<UINT,UINT,int,int>::Lookup instantiation
// (RVA 0x2f610, mfc140u), whose GetAssocAt (RVA 0x236e0, mfc140u) is
// transcribed here:
//     nHashValue = HashKey(key): ldiv((long)key, 127773) (import ldiv);
//                  r = 16807 * rem - 2836 * quot (32-bit); if (r < 0) r += 0x7fffffff;
//     nHashBucket = nHashValue % m_nHashTableSize (+0x10);
//     if (m_pHashTable (+0x08) == NULL) return NULL;
//     for (pAssoc = m_pHashTable[nHashBucket]; pAssoc; pAssoc = pAssoc->pNext (+0x08))
//         if (pAssoc->nHashValue (+0x10) == nHashValue && pAssoc->key (+0x00) == key) return pAssoc;
// and Lookup copies pAssoc->value (+0x04).  DEVIATION: retail divides by
// m_nHashTableSize before any test; a zero size is returned as "not found"
// here instead of faulting (toolbar/CMFCToolBar.cpp initialises it to 17, so
// the guard never fires in this tree).  Nothing in OpenMFC populates the map
// yet (see the note at its definition), so today every lookup misses.
int DefaultImage(UINT uiID) {
    const unsigned char* pMap = impl__m_DefaultImages_CMFCToolBar__1V__CMap_IIHH__A;
    const unsigned char* const* pHashTable = ReadAt<const unsigned char* const*>(pMap, 0x08);
    const UINT nHashTableSize = ReadAt<UINT>(pMap, 0x10);
    const ldiv_t hv = std::ldiv(static_cast<long>(static_cast<int>(uiID)), 127773L);
    std::uint32_t r = static_cast<std::uint32_t>(16807u * static_cast<std::uint32_t>(hv.rem)) -
                      static_cast<std::uint32_t>(2836u * static_cast<std::uint32_t>(hv.quot));
    if (static_cast<std::int32_t>(r) < 0) r += 0x7fffffffu;
    const UINT nHashValue = r;
    if (nHashTableSize == 0 || pHashTable == nullptr) return -1;
    for (const unsigned char* pAssoc = pHashTable[nHashValue % nHashTableSize]; pAssoc != nullptr;
         pAssoc = ReadAt<const unsigned char*>(pAssoc, 0x08)) {
        if (ReadAt<UINT>(pAssoc, 0x10) == nHashValue && ReadAt<UINT>(pAssoc, 0x00) == uiID) {
            return ReadAt<int>(pAssoc, 0x04);
        }
    }
    return -1;
}

// afxCommandManager->IsMenuItemWithoutImage(uiCmd), inlined by retail as the
// non-exported CList<UINT,UINT>::Find walk over GetCmdMgr()->m_lstCommandsWithoutImages:
// head at list+0x08, node pNext at +0x00, node data at +0x10 (the Find body
// also ENSUREs the node's data address, which cannot be NULL for a live node).
// The same walk as menu/CMFCPopupMenuBar.cpp's EnableContextMenuItems.
bool IsMenuItemWithoutImage(UINT uiCmd) {
    const unsigned char* pCmdMgr = static_cast<const unsigned char*>(impl__GetCmdMgr__YAPEAVCCommandManager__XZ());
    if (pCmdMgr == nullptr) return false;
    const unsigned char* pList = pCmdMgr + kOffCmdMgrCommandsWithoutImages;
    for (const unsigned char* pNode = ReadAt<const unsigned char*>(pList, 0x08); pNode != nullptr;
         pNode = ReadAt<const unsigned char*>(pNode, 0x00)) {
        if (ReadAt<UINT>(pNode, 0x10) == uiCmd) return true;
    }
    return false;
}

// Deviation (2): "!m_Buttons.IsEmpty()" through the exported FindIndex thunk.
bool ListHasHead(const CObList* pList) {
    return impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0) != CObList::POSITION(nullptr);
}

// Retail, inline in both the destructor and RebuildImageList:
//     while (!m_Buttons.IsEmpty()) delete m_Buttons.RemoveHead();
// (`delete` is the virtual scalar deleting destructor, vslot 1, flag 1, and is
// skipped for a NULL element.)  The only elements are the CMFCToolBarButtons
// RebuildImageList allocated with ??2@ + the exported constructor thunk, so the
// deleting destructor is spelled out as the virtual C++ destructor (the object
// carries OpenMFC's C++ vptr) followed by ??3@, which pairs with that ??2@.
void DeleteButtons(S_BtnCustDlg* d) {
    while (ListHasHead(&d->m_Buttons)) {
        CObject* pObj = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(&d->m_Buttons);
        if (pObj != nullptr) {
            pObj->~CObject();
            impl___3_YAXPEAX_Z(pObj);
        }
    }
}

// CStringT::LoadString(UINT) as retail inlines it (AfxFindStringResourceHandle,
// then the exported LoadString(HINSTANCE, UINT)), under ENSURE.
bool EnsureLoadString(CString& str, UINT nID) {
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
    if (hInst == nullptr ||
        !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&str, hInst, nID)) {
        ThrowInvalidArg();
        return false;
    }
    return true;
}

} // namespace

// Retail (RVA 0x15eae0, mfc140u), fully transcribed:
//     CDialog::CDialog(IDD_AFXBARRES_BUTTON_PROPS /*16132*/, pParent);
//     <store class vftable>
//     m_wndDefautImageBtn, m_wndUserImageBtn: CWnd::CWnd() + CButton vftable
//     m_wndDefaultImageArea: CWnd::CWnd() + CStatic vftable
//     m_wndButtonText: CWnd::CWnd() + CEdit vftable
//     m_wndAddImage: CWnd::CWnd() + CButton vftable
//     m_wndButtonList: CMFCToolBarButtonsListButton::CMFCToolBarButtonsListButton()
//     m_wndEditImage: CWnd::CWnd() + CButton vftable
//     m_strButtonText, m_strButtonDescr: empty (nil string)
//     m_pButton = pButton;  m_pImages = pImages;
//     m_iStartImage = iStartImage;  m_bMenuMode = bMenuMode;
//     m_Buttons: CObList(10), inlined;  m_rectDefaultImage = (0,0,0,0);
//     m_strAccel: empty;  m_pUserTool = NULL;
//     if (afxUserToolsManager != NULL) m_pUserTool = afxUserToolsManager->FindTool(m_pButton->m_nID);   // inlined
//     m_bUserButton = pButton->m_bUserButton ||
//                     (m_pUserTool == NULL && CMFCToolBar::GetDefaultImage(m_pButton->m_nID) < 0);
//     m_iSelImage = pButton->GetImage();
//     m_bImage = pButton->m_bImage;
//     if (m_bMenuMode && afxCommandManager->IsMenuItemWithoutImage(pButton->m_nID)) m_bImage = FALSE;
//     m_bText = (m_bMenuMode || m_pButton->m_bTextBelow) ? TRUE : pButton->m_bText;
//     m_strButtonText = _T("");  m_strButtonDescr = _T("");
// DEVIATIONS: the vftable stores are deviation (1); the FindTool block is
// deviation (3).  Retail dereferences pButton unconditionally, and so does this
// body.  This export was not on this file's work list (its previous body only
// returned pThis), but the destructor and every handler depend on the members
// being constructed, so it is implemented with them.
// Symbol: ??0CMFCToolBarButtonCustomizeDialog@@QEAA@PEAVCMFCToolBarButton@@PEAVCMFCToolBarImages@@PEAVCWnd@@HH@Z
extern "C" void* MS_ABI impl___0CMFCToolBarButtonCustomizeDialog__QEAA_PEAVCMFCToolBarButton__PEAVCMFCToolBarImages__PEAVCWnd__HH_Z(
    void* pThis, CMFCToolBarButton* pButton, CMFCToolBarImages* pImages, CWnd* pParent, int iStartImage, int bMenuMode) {
    if (pThis == nullptr) return nullptr;
    S_BtnCustDlg* d = D(pThis);
    impl___0CDialog__QEAA_IPEAVCWnd___Z(static_cast<CDialog*>(pThis), kIddButtonProps, pParent);
    impl___0CWnd__QEAA_XZ(d->m_wndDefautImageBtn);
    impl___0CWnd__QEAA_XZ(d->m_wndUserImageBtn);
    impl___0CWnd__QEAA_XZ(d->m_wndDefaultImageArea);
    impl___0CWnd__QEAA_XZ(d->m_wndButtonText);
    impl___0CWnd__QEAA_XZ(d->m_wndAddImage);
    impl___0CMFCToolBarButtonsListButton__QEAA_XZ(d->m_wndButtonList);
    impl___0CWnd__QEAA_XZ(d->m_wndEditImage);
    new (&d->m_strButtonText) CString();
    new (&d->m_strButtonDescr) CString();
    d->m_pButton = pButton;
    d->m_pImages = pImages;
    d->m_iStartImage = iStartImage;
    d->m_bMenuMode = bMenuMode;
    impl___0CObList__QEAA__J_Z(&d->m_Buttons, 10);
    d->m_rectDefaultImage.left = 0;
    d->m_rectDefaultImage.top = 0;
    d->m_rectDefaultImage.right = 0;
    d->m_rectDefaultImage.bottom = 0;
    new (&d->m_strAccel) CString();
    d->m_pUserTool = nullptr;               // deviation (3): afxUserToolsManager is NULL

    d->m_bUserButton = (pButton->m_bUserButton != 0 ||
                        (d->m_pUserTool == nullptr && DefaultImage(pButton->m_nID) < 0)) ? TRUE : FALSE;
    d->m_iSelImage = pButton->GetImage();
    d->m_bImage = pButton->m_bImage;
    if (d->m_bMenuMode && IsMenuItemWithoutImage(pButton->m_nID)) {
        d->m_bImage = FALSE;
    }
    d->m_bText = (d->m_bMenuMode || d->m_pButton->m_bTextBelow) ? TRUE : pButton->m_bText;
    d->m_strButtonText = L"";
    d->m_strButtonDescr = L"";
    return pThis;
}

// Retail (RVA 0x15ee10, mfc140u), fully transcribed:
//     <store class vftable>
//     while (!m_Buttons.IsEmpty()) delete m_Buttons.RemoveHead();
//     m_strAccel.~CString();
//     m_Buttons.~CObList();                  // inlined: CObList vftable + RemoveAll
//     m_strButtonDescr.~CString();  m_strButtonText.~CString();
//     m_wndEditImage.~CButton();  m_wndButtonList.~CMFCToolBarButtonsListButton();
//     m_wndAddImage.~CButton();  m_wndButtonText.~CEdit();
//     m_wndDefaultImageArea.~CStatic();
//     m_wndUserImageBtn.~CButton();  m_wndDefautImageBtn.~CButton();
//     CDialog::~CDialog();                   // tail jump
// The loop is deviation (2); the vftable store is deviation (1).
// Symbol: ??1CMFCToolBarButtonCustomizeDialog@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarButtonCustomizeDialog__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    DeleteButtons(d);
    d->m_strAccel.~CString();
    impl___1CObList__UEAA_XZ(&d->m_Buttons);
    d->m_strButtonDescr.~CString();
    d->m_strButtonText.~CString();
    impl___1CButton__UEAA_XZ(d->m_wndEditImage);
    impl___1CMFCToolBarButtonsListButton__UEAA_XZ(d->m_wndButtonList);
    impl___1CButton__UEAA_XZ(d->m_wndAddImage);
    impl___1CEdit__UEAA_XZ(reinterpret_cast<CEdit*>(d->m_wndButtonText));
    impl___1CStatic__UEAA_XZ(reinterpret_cast<CStatic*>(d->m_wndDefaultImageArea));
    impl___1CButton__UEAA_XZ(d->m_wndUserImageBtn);
    impl___1CButton__UEAA_XZ(d->m_wndDefautImageBtn);
    impl___1CDialog__UEAA_XZ(static_cast<CDialog*>(pThis));
}

// Retail (RVA 0x15ef60, mfc140u), fully transcribed:
//     DDX_Control(pDX, IDC_AFXBARRES_DEFAULT_IMAGE,      m_wndDefautImageBtn);
//     DDX_Control(pDX, IDC_AFXBARRES_USER_IMAGE,         m_wndUserImageBtn);
//     DDX_Control(pDX, IDC_AFXBARRES_DEFAULT_IMAGE_AREA, m_wndDefaultImageArea);
//     DDX_Control(pDX, IDC_AFXBARRES_BUTTON_TEXT,        m_wndButtonText);
//     DDX_Control(pDX, IDC_AFXBARRES_ADD_IMAGE,          m_wndAddImage);
//     DDX_Control(pDX, IDC_AFXBARRES_IMAGE_LIST,         m_wndButtonList);
//     DDX_Control(pDX, IDC_AFXBARRES_EDIT_IMAGE,         m_wndEditImage);
//     DDX_Text   (pDX, IDC_AFXBARRES_BUTTON_TEXT,        m_strButtonText);
//     DDX_Text   (pDX, IDC_AFXBARRES_BUTTON_DESCR,       m_strButtonDescr);   // tail jump
// Symbol: ?DoDataExchange@CMFCToolBarButtonCustomizeDialog@@MEAAXPEAVCDataExchange@@@Z
extern "C" void MS_ABI impl__DoDataExchange_CMFCToolBarButtonCustomizeDialog__MEAAXPEAVCDataExchange___Z(void* pThis, void* pDX) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcDefaultImage, d->m_wndDefautImageBtn);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcUserImage, d->m_wndUserImageBtn);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcDefaultImageArea, d->m_wndDefaultImageArea);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcButtonText, d->m_wndButtonText);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcAddImage, d->m_wndAddImage);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcImageList, d->m_wndButtonList);
    impl__DDX_Control__YAXPEAVCDataExchange__HAEAVCWnd___Z(pDX, kIdcEditImage, d->m_wndEditImage);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcButtonText, &d->m_strButtonText);
    impl__DDX_Text__YAXPEAVCDataExchange__HAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pDX, kIdcButtonDescr, &d->m_strButtonDescr);
}

// Retail (RVA 0x160020, mfc140u), fully transcribed (m_pButton->m_bLocked is
// read once, on entry):
//     BOOL bLocked = m_pButton->m_bLocked;
//     m_wndButtonText.EnableWindow(m_bText && (!m_bMenuMode || m_pUserTool == NULL) &&
//                                  !CMFCToolBar::m_bDisableLabelsEdit);
//     m_wndButtonList.EnableWindow(m_bImage && m_pImages != NULL && m_bUserButton && !bLocked);
//     m_wndAddImage.EnableWindow(m_bImage && m_pImages != NULL && m_bUserButton &&
//                                !m_pImages->IsReadOnly() && !bLocked);
//     m_wndEditImage.EnableWindow(m_bImage && m_pImages != NULL && m_iSelImage >= 0 &&
//                                 m_bUserButton && !m_pImages->IsReadOnly() && !bLocked);
//     m_wndUserImageBtn.EnableWindow(m_bImage && m_pImages != NULL && !bLocked);
//     m_wndDefautImageBtn.EnableWindow(m_pUserTool != NULL ||
//         (m_bImage && CMFCToolBar::GetDefaultImage(m_pButton->m_nID) >= 0 && !bLocked));
//     ::InvalidateRect(m_hWnd, &m_rectDefaultImage, TRUE);    // tail jump; import slot resolves to InvalidateRect
// Symbol: ?EnableControls@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    const BOOL bLocked = d->m_pButton->m_bLocked;

    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndButtonText),
        (d->m_bText && (!d->m_bMenuMode || d->m_pUserTool == nullptr) &&
         impl__m_bDisableLabelsEdit_CMFCToolBar__2HA == 0) ? TRUE : FALSE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndButtonList),
        (d->m_bImage && d->m_pImages != nullptr && d->m_bUserButton && !bLocked) ? TRUE : FALSE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndAddImage),
        (d->m_bImage && d->m_pImages != nullptr && d->m_bUserButton &&
         !ImagesProbe::ReadOnly(d->m_pImages) && !bLocked) ? TRUE : FALSE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndEditImage),
        (d->m_bImage && d->m_pImages != nullptr && d->m_iSelImage >= 0 && d->m_bUserButton &&
         !ImagesProbe::ReadOnly(d->m_pImages) && !bLocked) ? TRUE : FALSE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndUserImageBtn),
        (d->m_bImage && d->m_pImages != nullptr && !bLocked) ? TRUE : FALSE);
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndDefautImageBtn),
        (d->m_pUserTool != nullptr ||
         (d->m_bImage && DefaultImage(d->m_pButton->m_nID) >= 0 && !bLocked)) ? TRUE : FALSE);
    ::InvalidateRect(Wnd(pThis)->m_hWnd, AsRect(&d->m_rectDefaultImage), TRUE);
}

// Retail (RVA 0x15f040, mfc140u), transcribed (the reason it stays a stub is
// below the transcription):
//     ENSURE(m_pImages != NULL);
//     CSize sizeImage = m_pImages->GetImageSize();                   // +0x68
//     CClientDC dc(&m_wndButtonList);
//     CBitmap bitmap;  CDC memDC;
//     memDC.Attach(::CreateCompatibleDC(dc.GetSafeHdc()));
//     if (!bitmap.Attach(::CreateCompatibleBitmap(dc.GetSafeHdc(), sizeImage.cx, sizeImage.cy))) {
//         AfxMessageBox(IDP_AFXBARRES_CANNT_CREATE_IMAGE /*16003*/, 0, (UINT)-1);  return;
//     }
//     CBitmap* pOldBitmap = CGdiObject::FromHandle(::SelectObject(memDC, bitmap));
//     <afxGlobalData one-time Initialize gate>
//     CRect rect(0, 0, sizeImage.cx, sizeImage.cy);
//     ::FillRect(memDC, &rect, <the HBRUSH at afxGlobalData + 0xb0>);
//     ::SelectObject(memDC, pOldBitmap ? pOldBitmap->m_hObject : NULL);
//     BITMAP bmp;  ::GetObject(m_pImages->GetImageWell() /*+0xa0*/, sizeof(BITMAP), &bmp);
//     if (g_pWndCustomize != NULL) {                                 // the DLL-internal customize-dialog pointer
//         if (!g_pWndCustomize->OnEditToolbarMenuImage(this, bitmap, bmp.bmBitsPixel)) return;   // vslot 105
//     } else {
//         CMFCImageEditorDialog dlg(&bitmap, this, bmp.bmBitsPixel);
//         if (dlg.DoModal() != IDOK) return;
//     }
//     int iImage = m_pImages->AddImage((HBITMAP)bitmap.m_hObject, FALSE);
//     if (iImage < 0) { AfxMessageBox(IDP_AFXBARRES_CANNT_CREATE_IMAGE, 0, (UINT)-1);  return; }
//     RebuildImageList();
//     m_wndButtonList.SelectButton(iImage);
// STUB: the image-editing step cannot run in OpenMFC.  ??0CMFCImageEditorDialog
// (controls/CMFCImageEditorDialog.cpp) is, as of this writing, an
// argument-silencing placeholder
// that constructs nothing, so DoModal cannot be run on it, and
// CMFCToolBarsCustomizeDialog::OnEditToolbarMenuImage (the other branch) is a
// documented stub that returns FALSE for the same reason.  Every path would end
// at "not edited -> return" after creating and freeing GDI objects; the image
// list would never change.
// Symbol: ?OnAddImage@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnAddImage_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x1601f0, mfc140u), fully transcribed:
//     m_iSelImage = m_pButton->GetImage();
//     m_bUserButton = FALSE;
//     m_wndUserImageBtn.SetCheck(0);        // inlined ::SendMessage(hWnd, BM_SETCHECK, 0, 0)
//     EnableControls();                     // tail jump
// Symbol: ?OnDefaultImage@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnDefaultImage_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    d->m_iSelImage = d->m_pButton->GetImage();
    d->m_bUserButton = FALSE;
    ::SendMessage(HwndOf(d->m_wndUserImageBtn), BM_SETCHECK, 0, 0);
    impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x15f400, mfc140u), transcribed (the reason it stays a stub is
// below the transcription):
//     ENSURE(m_pImages != NULL && m_iSelImage >= 0);
//     CSize sizeImage = m_pImages->GetImageSize();
//     CClientDC dc(&m_wndButtonList);  CBitmap bitmap;  CDC memDC;
//     memDC.Attach(::CreateCompatibleDC(dc));
//     if (!bitmap.Attach(::CreateCompatibleBitmap(dc, sizeImage.cx, sizeImage.cy))) return;
//     CBitmap* pOldBitmap = CGdiObject::FromHandle(::SelectObject(memDC, bitmap));
//     COLORREF clrTransparent = m_pImages->m_clrTransparent;                        // +0xd8
//     if (clrTransparent != RGB(192,192,192)) m_pImages->m_clrTransparent = RGB(192,192,192);
//     CRect rect(0, 0, sizeImage.cx, sizeImage.cy);
//     memDC.FillSolidRect(&rect, RGB(192,192,192));   // inlined: ENSURE(memDC.m_hDC), SetBkColor, ExtTextOut(ETO_OPAQUE)
//     CAfxDrawState ds;
//     if (!m_pImages->PrepareDrawImage(ds, CSize(0, 0), FALSE)) return;
//     m_pImages->Draw(&memDC, 0, 0, m_iSelImage, FALSE, FALSE, FALSE, FALSE, FALSE, 0xff);
//     m_pImages->EndDrawImage(ds);
//     if (m_pImages->m_clrTransparent != clrTransparent) m_pImages->m_clrTransparent = clrTransparent;
//     ::SelectObject(memDC, pOldBitmap ? pOldBitmap->m_hObject : NULL);
//     BITMAP bmp;  ::GetObject(m_pImages->GetImageWell(), sizeof(BITMAP), &bmp);
//     (the same g_pWndCustomize->OnEditToolbarMenuImage / CMFCImageEditorDialog
//      branch as OnAddImage; "not edited" returns)
//     m_pImages->UpdateImage(m_iSelImage, (HBITMAP)bitmap.m_hObject);
//     ::InvalidateRect(m_wndButtonList.m_hWnd, NULL, TRUE);
// STUB: the editing step cannot run in OpenMFC, for the reasons given at
// OnAddImage; in addition ?PrepareDrawImage@CMFCToolBarImages@@ (which gates
// the body) and ?UpdateImage@CMFCToolBarImages@@ are still auto-generated
// placeholders in toolbar/CMFCToolBarImages.cpp that take no `this` and
// return 0, so this body would return at the PrepareDrawImage test.
// Symbol: ?OnEditImage@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnEditImage_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Retail (RVA 0x15f8f0, mfc140u), fully transcribed:
//     m_bImage = TRUE;  m_bText = FALSE;    // one 8-byte store of 1 at +0x900
//     EnableControls();                     // tail jump
// Symbol: ?OnImage@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnImage_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    d->m_bImage = TRUE;
    d->m_bText = FALSE;
    impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x15f890, mfc140u), fully transcribed (no EnableControls call):
//     CMFCToolBarButton* pSel = m_wndButtonList.GetSelectedButton();   // inline, list +0x218
//     m_iSelImage = pSel == NULL ? -1 : pSel->GetImage();
//     m_wndEditImage.EnableWindow(m_iSelImage >= 0 && m_pImages != NULL &&
//                                 !m_pImages->IsReadOnly());          // tail jump
// Symbol: ?OnImageList@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnImageList_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    const CMFCToolBarButton* pSel = ReadAt<const CMFCToolBarButton*>(d->m_wndButtonList, kOffListSelButton);
    d->m_iSelImage = pSel == nullptr ? -1 : pSel->GetImage();
    impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndEditImage),
        (d->m_iSelImage >= 0 && d->m_pImages != nullptr && !ImagesProbe::ReadOnly(d->m_pImages)) ? TRUE : FALSE);
}

// Retail (RVA 0x15f900, mfc140u), fully transcribed:
//     m_bImage = TRUE;  m_bText = TRUE;
//     EnableControls();                     // tail jump
// Symbol: ?OnImageText@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnImageText_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    d->m_bImage = TRUE;
    d->m_bText = TRUE;
    impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x15fbc0, mfc140u), fully transcribed:
//     CDialog::OnInitDialog();
//     CWnd* pWndImage = GetDlgItem(IDC_AFXBARRES_IMAGE);          ENSURE(pWndImage != NULL);
//     CWnd* pWndImageText = GetDlgItem(IDC_AFXBARRES_IMAGE_TEXT); ENSURE(pWndImageText != NULL);
//     if (AfxGetMainWnd() != NULL && (AfxGetMainWnd()->GetExStyle() & WS_EX_LAYOUTRTL))
//         ModifyStyleEx(0, WS_EX_LAYOUTRTL);
//         // AfxGetMainWnd inlined: AfxGetModuleThreadState()->m_pCurrentWinThread,
//         // then CWinThread::GetMainWnd (vslot 31); retail evaluates it twice
//     if (m_pImages != NULL) {
//         m_wndButtonList.SetImages(m_pImages);
//         RebuildImageList();
//         m_wndButtonList.SelectButton(m_iSelImage);
//     } else {
//         m_wndButtonList.EnableWindow(FALSE);
//         m_wndUserImageBtn.EnableWindow(FALSE);
//         if (m_iSelImage < 0) { pWndImage->EnableWindow(FALSE); pWndImageText->EnableWindow(FALSE); }
//     }
//     if (m_bUserButton && !m_pButton->m_bLocked) m_wndUserImageBtn.SetCheck(TRUE);   // inlined BM_SETCHECK, 1, 0
//     else                                        m_wndDefautImageBtn.SetCheck(TRUE);
//     if (m_bImage) {
//         CheckDlgButton(m_bText ? IDC_AFXBARRES_IMAGE_TEXT : IDC_AFXBARRES_IMAGE, TRUE);
//     } else {
//         ENSURE(m_bText);
//         CheckDlgButton(IDC_AFXBARRES_TEXT, TRUE);
//         m_bText = TRUE;
//         m_wndButtonList.EnableWindow(FALSE);
//     }
//     int iTabOffset = m_pButton->m_strText.Find(_T('\t'));      // inlined: wcschr, (p - buf) / 2
//     if (iTabOffset >= 0) {
//         m_strButtonText = m_pButton->m_strText.Left(iTabOffset);
//         m_strAccel = m_pButton->m_strText.Mid(iTabOffset + 1);
//     } else {
//         m_strButtonText = m_pButton->m_strText;
//     }
//     CFrameWnd* pParent = GetParentFrame();
//     if (pParent != NULL) pParent->GetMessageString(m_pButton->m_nID, m_strButtonDescr);   // vslot 95
//     if (m_bMenuMode) pWndImage->EnableWindow(FALSE);
//     if (m_pButton->m_bTextBelow) pWndImage->EnableWindow(FALSE);
//     ::GetClientRect(m_wndDefaultImageArea.m_hWnd, &m_rectDefaultImage);
//     ::MapWindowPoints(m_wndDefaultImageArea.m_hWnd, m_hWnd, (LPPOINT)&m_rectDefaultImage, 2);
//     CSize sizeImage = CMFCToolBar::m_Images.GetImageSize();     // m_Images + 0x68
//     m_rectDefaultImage.right = m_rectDefaultImage.left + min(sizeImage.cx, 16);
//     m_rectDefaultImage.bottom = m_rectDefaultImage.top + min(sizeImage.cy, 16);
//     EnableControls();
//     UpdateData(FALSE);
//     return TRUE;
// DEVIATIONS: CWinThread::GetMainWnd and CFrameWnd::GetMessageString go to the
// exported base thunks (deviation (1)), and the main window is fetched once and
// reused (retail evaluates the virtual call twice).  The current thread comes
// from the exported AfxGetThread thunk rather than a direct read of
// m_pCurrentWinThread; OpenMFC's AfxGetThread (detail/CWinAppSupport.cpp)
// falls back to the application object when no thread pointer is recorded,
// where retail's inlined read would see whatever the module thread state
// holds.  The ENSURE throws
// are the thunk call followed by `return FALSE`, which is never reached.
// Symbol: ?OnInitDialog@CMFCToolBarButtonCustomizeDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnInitDialog_CMFCToolBarButtonCustomizeDialog__MEAAHXZ(void* pThis) {
    if (pThis == nullptr) return FALSE;
    S_BtnCustDlg* d = D(pThis);
    CWnd* pSelf = Wnd(pThis);
    impl__OnInitDialog_CDialog__UEAAHXZ(static_cast<CDialog*>(pThis));

    CWnd* pWndImage = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pSelf, kIdcImage);
    if (pWndImage == nullptr) { ThrowInvalidArg(); return FALSE; }
    CWnd* pWndImageText = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pSelf, kIdcImageText);
    if (pWndImageText == nullptr) { ThrowInvalidArg(); return FALSE; }

    CWinThread* pThread = impl__AfxGetThread__YAPEAVCWinThread__XZ();
    CWnd* pMainWnd = pThread != nullptr ? impl__GetMainWnd_CWinThread__UEAAPEAVCWnd__XZ(pThread) : nullptr;
    if (pMainWnd != nullptr && (impl__GetExStyle_CWnd__QEBAKXZ(pMainWnd) & WS_EX_LAYOUTRTL) != 0) {
        impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(pSelf, 0, WS_EX_LAYOUTRTL, 0);
    }

    if (d->m_pImages != nullptr) {
        impl__SetImages_CMFCToolBarButtonsListButton__QEAAXPEAVCMFCToolBarImages___Z(d->m_wndButtonList, d->m_pImages);
        impl__RebuildImageList_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
        impl__SelectButton_CMFCToolBarButtonsListButton__QEAAHH_Z(d->m_wndButtonList, d->m_iSelImage);
    } else {
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndButtonList), FALSE);
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndUserImageBtn), FALSE);
        if (d->m_iSelImage < 0) {
            impl__EnableWindow_CWnd__QEAAHH_Z(pWndImage, FALSE);
            impl__EnableWindow_CWnd__QEAAHH_Z(pWndImageText, FALSE);
        }
    }

    if (d->m_bUserButton && !d->m_pButton->m_bLocked) {
        ::SendMessage(HwndOf(d->m_wndUserImageBtn), BM_SETCHECK, 1, 0);
    } else {
        ::SendMessage(HwndOf(d->m_wndDefautImageBtn), BM_SETCHECK, 1, 0);
    }

    if (d->m_bImage) {
        impl__CheckDlgButton_CWnd__QEAAXHI_Z(pSelf, d->m_bText ? kIdcImageText : kIdcImage, TRUE);
    } else {
        if (!d->m_bText) { ThrowInvalidArg(); return FALSE; }
        impl__CheckDlgButton_CWnd__QEAAXHI_Z(pSelf, kIdcText, TRUE);
        d->m_bText = TRUE;
        impl__EnableWindow_CWnd__QEAAHH_Z(Wnd(d->m_wndButtonList), FALSE);
    }

    const CString& strText = d->m_pButton->m_strText;
    const int iTabOffset = strText.Find(L'\t');
    if (iTabOffset >= 0) {
        d->m_strButtonText = strText.Left(iTabOffset);
        d->m_strAccel = strText.Mid(iTabOffset + 1);
    } else {
        d->m_strButtonText = strText;
    }

    CFrameWnd* pParent = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pSelf);
    if (pParent != nullptr) {
        impl__GetMessageString_CFrameWnd__UEBAXIAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            pParent, d->m_pButton->m_nID, &d->m_strButtonDescr);
    }

    if (d->m_bMenuMode) impl__EnableWindow_CWnd__QEAAHH_Z(pWndImage, FALSE);
    if (d->m_pButton->m_bTextBelow) impl__EnableWindow_CWnd__QEAAHH_Z(pWndImage, FALSE);

    const HWND hArea = HwndOf(d->m_wndDefaultImageArea);
    ::GetClientRect(hArea, AsRect(&d->m_rectDefaultImage));
    ::MapWindowPoints(hArea, pSelf->m_hWnd, reinterpret_cast<LPPOINT>(AsRect(&d->m_rectDefaultImage)), 2);

    const SIZE sizeImage = ImagesProbe::ImageSize(&impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A);
    d->m_rectDefaultImage.right = d->m_rectDefaultImage.left + (sizeImage.cx > 16 ? 16 : sizeImage.cx);
    d->m_rectDefaultImage.bottom = d->m_rectDefaultImage.top + (sizeImage.cy > 16 ? 16 : sizeImage.cy);

    impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(pSelf, FALSE);
    return TRUE;
}

// Retail (RVA 0x15f940, mfc140u), fully transcribed:
//     UpdateData(TRUE);
//     int iImage;
//     if (m_bUserButton)            iImage = m_iSelImage;
//     else if (m_pUserTool != NULL) iImage = 0;
//     else                          iImage = CMFCToolBar::GetDefaultImage(m_pButton->m_nID);
//     if (m_bImage && iImage < 0) {
//         CString str;  ENSURE(str.LoadString(IDP_AFXBARRES_IMAGE_IS_REQUIRED));
//         AfxMessageBox(str, 0, 0);
//         m_wndButtonList.SetFocus();
//         return;
//     }
//     if (m_bText && m_strButtonText.IsEmpty()) {
//         CString str;  ENSURE(str.LoadString(IDP_AFXBARRES_TEXT_IS_REQUIRED));
//         AfxMessageBox(str, 0, 0);
//         m_wndButtonText.SetFocus();
//         return;
//     }
//     if (!m_pButton->m_bTextBelow) m_pButton->m_bText = m_bText;
//     if (m_bMenuMode) afxCommandManager->EnableMenuItemImage(m_pButton->m_nID, m_bImage, iImage);
//     else             m_pButton->m_bImage = m_bImage;
//     m_pButton->m_bUserButton = m_bUserButton;
//     m_pButton->SetImage(iImage);                           // vslot 50
//     m_pButton->m_strText = m_strButtonText;
//     if (!m_strAccel.IsEmpty()) { m_pButton->m_strText += _T('\t'); m_pButton->m_strText += m_strAccel; }
//     CDialog::OnOK();
// DEVIATIONS: SetImage goes to the exported CMFCToolBarButton base thunk
// (deviation (1)).  CCommandManager::EnableMenuItemImage is called with the
// argument list its mangled name describes; its definition is still an
// auto-generated empty placeholder, so in this tree the menu-mode arm records
// nothing.
// Symbol: ?OnOK@CMFCToolBarButtonCustomizeDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnOK_CMFCToolBarButtonCustomizeDialog__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    impl__UpdateData_CWnd__QEAAHH_Z(Wnd(pThis), TRUE);

    int iImage;
    if (d->m_bUserButton) {
        iImage = d->m_iSelImage;
    } else if (d->m_pUserTool != nullptr) {
        iImage = 0;
    } else {
        iImage = DefaultImage(d->m_pButton->m_nID);
    }

    if (d->m_bImage && iImage < 0) {
        CString str;
        if (!EnsureLoadString(str, kIdpImageIsRequired)) return;
        impl__AfxMessageBox__YAHPEB_WII_Z(str.GetString(), 0, 0);
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(d->m_wndButtonList));
        return;
    }
    if (d->m_bText && d->m_strButtonText.IsEmpty()) {
        CString str;
        if (!EnsureLoadString(str, kIdpTextIsRequired)) return;
        impl__AfxMessageBox__YAHPEB_WII_Z(str.GetString(), 0, 0);
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(d->m_wndButtonText));
        return;
    }

    CMFCToolBarButton* pButton = d->m_pButton;
    if (!pButton->m_bTextBelow) pButton->m_bText = d->m_bText;
    if (d->m_bMenuMode) {
        impl__EnableMenuItemImage_CCommandManager__QEAAXIHH_Z(impl__GetCmdMgr__YAPEAVCCommandManager__XZ(),
                                                             pButton->m_nID, d->m_bImage, iImage);
    } else {
        pButton->m_bImage = d->m_bImage;
    }
    pButton->m_bUserButton = d->m_bUserButton;
    impl__SetImage_CMFCToolBarButton__UEAAXH_Z(pButton, iImage);
    pButton->m_strText = d->m_strButtonText;
    if (!d->m_strAccel.IsEmpty()) {
        pButton->m_strText += L'\t';
        pButton->m_strText += d->m_strAccel;
    }
    impl__OnOK_CDialog__MEAAXXZ(static_cast<CDialog*>(pThis));
}

// Retail (RVA 0x160250, mfc140u), fully transcribed:
//     CPaintDC dc(this);
//     if (m_pUserTool != NULL) {
//         m_pUserTool->DrawToolIcon(&dc, m_rectDefaultImage);
//     } else if (m_pButton->m_bLocked) {
//         BOOL bText = m_pButton->m_bText, bImage = m_pButton->m_bImage;
//         m_pButton->m_bText = FALSE;  m_pButton->m_bImage = TRUE;
//         m_pButton->OnDraw(&dc, m_rectDefaultImage, NULL, TRUE, FALSE, FALSE, FALSE, FALSE);   // vslot 8
//         m_pButton->m_bText = bText;  m_pButton->m_bImage = bImage;
//     } else {
//         int iImage = CMFCToolBar::GetDefaultImage(m_pButton->m_nID);
//         if (iImage >= 0 && m_bImage) {
//             CAfxDrawState ds;
//             CMFCToolBar::m_Images.PrepareDrawImage(ds, m_rectDefaultImage.Size(), FALSE);
//             CMFCToolBar::m_Images.Draw(&dc, m_rectDefaultImage.left, m_rectDefaultImage.top, iImage,
//                                        FALSE, FALSE, FALSE, FALSE, FALSE, 0xff);
//             CMFCToolBar::m_Images.EndDrawImage(ds);
//         }
//     }
// DEVIATIONS: OnDraw goes to the exported CMFCToolBarButton base thunk
// (deviation (1)); that definition, and PrepareDrawImage's, are still empty
// auto-generated placeholders, and OpenMFC's CMFCToolBarImages::Draw does not
// model the retail draw pipeline -- so today only the calls are made.  The
// CAfxDrawState is the three-handle block controls/CMFCReBar.cpp models (retail
// reserves 0x18 bytes for it on this frame).
// Symbol: ?OnPaint@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);

    alignas(void*) unsigned char paintStorage[sizeof(CPaintDC)] = {};
    CPaintDC* pPaintDC = reinterpret_cast<CPaintDC*>(paintStorage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pPaintDC, Wnd(pThis));
    CDC* pDC = reinterpret_cast<CDC*>(pPaintDC);

    if (d->m_pUserTool != nullptr) {
        impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(d->m_pUserTool, pDC, &d->m_rectDefaultImage);
    } else if (d->m_pButton->m_bLocked) {
        CMFCToolBarButton* pButton = d->m_pButton;
        const BOOL bText = pButton->m_bText;
        const BOOL bImage = pButton->m_bImage;
        pButton->m_bText = FALSE;
        pButton->m_bImage = TRUE;
        impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
            pButton, pDC, &d->m_rectDefaultImage, nullptr, TRUE, FALSE, FALSE, FALSE, FALSE);
        pButton->m_bText = bText;
        pButton->m_bImage = bImage;
    } else {
        const int iImage = DefaultImage(d->m_pButton->m_nID);
        if (iImage >= 0 && d->m_bImage) {
            CMFCToolBarImages* pImages = &impl__m_Images_CMFCToolBar__1VCMFCToolBarImages__A;
            HBITMAP ds[3] = { nullptr, nullptr, nullptr };
            SIZE sizeDest;
            sizeDest.cx = d->m_rectDefaultImage.right - d->m_rectDefaultImage.left;
            sizeDest.cy = d->m_rectDefaultImage.bottom - d->m_rectDefaultImage.top;
            long long sizePacked;
            static_assert(sizeof(sizePacked) == sizeof(sizeDest), "CSize by value is one 8-byte register");
            std::memcpy(&sizePacked, &sizeDest, sizeof sizePacked);
            impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(pImages, ds, sizePacked, FALSE);
            impl__Draw_CMFCToolBarImages__QEAAHPEAVCDC__HHHHHHHHE_Z(pImages, pDC,
                d->m_rectDefaultImage.left, d->m_rectDefaultImage.top, iImage,
                FALSE, FALSE, FALSE, FALSE, FALSE, 0xff);
            impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(pImages, ds);
        }
    }

    impl___1CPaintDC__UEAA_XZ(pPaintDC);
}

// Retail (RVA 0x15f920, mfc140u), fully transcribed:
//     m_bImage = FALSE;  m_bText = TRUE;
//     EnableControls();                     // tail jump
// Symbol: ?OnText@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnText_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    d->m_bImage = FALSE;
    d->m_bText = TRUE;
    impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
}

// Retail (RVA 0x1601a0, mfc140u), fully transcribed:
//     m_iSelImage = -1;
//     m_bUserButton = TRUE;
//     m_wndDefautImageBtn.SetCheck(0);      // inlined ::SendMessage(hWnd, BM_SETCHECK, 0, 0)
//     EnableControls();
//     m_wndButtonList.SelectButton((CMFCToolBarButton*)NULL);   // tail jump
// Symbol: ?OnUserImage@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__OnUserImage_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    d->m_iSelImage = -1;
    d->m_bUserButton = TRUE;
    ::SendMessage(HwndOf(d->m_wndDefautImageBtn), BM_SETCHECK, 0, 0);
    impl__EnableControls_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(pThis);
    impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(d->m_wndButtonList, nullptr);
}

// Retail (RVA 0x15ff40, mfc140u), fully transcribed:
//     m_wndButtonList.RemoveButtons();
//     while (!m_Buttons.IsEmpty()) delete m_Buttons.RemoveHead();
//     for (int iImage = m_iStartImage; iImage < m_pImages->GetCount(); iImage++) {   // m_iCount, +0x08
//         CMFCToolBarButton* pButton = new CMFCToolBarButton;                     // operator new(0x88)
//         pButton->SetImage(iImage);                                               // vslot 50
//         m_wndButtonList.AddButton(pButton);
//         m_Buttons.AddTail(pButton);
//     }
//     ::InvalidateRect(m_wndButtonList.m_hWnd, NULL, TRUE);                      // tail jump
// (Retail loads m_pImages->m_iCount once, before the loop, and dereferences
// m_pImages unchecked; OnInitDialog and OnAddImage only call it with
// m_pImages != NULL.)  The object is allocated with the DLL's own ??2@ thunk
// and the exported constructor thunk, and DeleteButtons releases it with the
// matching ??3@.  Retail's ??2@ (RVA 0x27f0, mfc140u) retries malloc through
// the new handler stored at AFX_MODULE_THREAD_STATE + 0x50 and returns NULL
// only when there is no handler or it gives up; retail's constructor call is
// skipped for NULL, and the SetImage vcall that follows then dereferences NULL.
// OpenMFC's ??2@ is a bare malloc with no handler, so DEVIATION: a NULL result
// raises CMemoryException here (AfxThrowMemoryException does not return)
// instead of faulting.  SetImage is
// called through the base thunk, which is exact here: the object IS a plain
// CMFCToolBarButton.  The list loop is deviation (2).
// Symbol: ?RebuildImageList@CMFCToolBarButtonCustomizeDialog@@IEAAXXZ
extern "C" void MS_ABI impl__RebuildImageList_CMFCToolBarButtonCustomizeDialog__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_BtnCustDlg* d = D(pThis);
    impl__RemoveButtons_CMFCToolBarButtonsListButton__QEAAXXZ(d->m_wndButtonList);
    DeleteButtons(d);

    const int iLast = ImagesProbe::Count(d->m_pImages) - 1;
    for (int iImage = d->m_iStartImage; iImage <= iLast; ++iImage) {
        void* pMem = impl___2_YAPEAX_K_Z(sizeof(CMFCToolBarButton));
        if (pMem == nullptr) { impl__AfxThrowMemoryException__YAXXZ(); return; }
        CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(impl___0CMFCToolBarButton__QEAA_XZ(pMem));
        impl__SetImage_CMFCToolBarButton__UEAAXH_Z(pButton, iImage);
        impl__AddButton_CMFCToolBarButtonsListButton__QEAAXPEAVCMFCToolBarButton___Z(d->m_wndButtonList, pButton);
        impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(&d->m_Buttons, pButton);
    }
    ::InvalidateRect(HwndOf(d->m_wndButtonList), nullptr, TRUE);
}
