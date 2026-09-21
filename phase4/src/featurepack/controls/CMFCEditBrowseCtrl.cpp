// CMFCEditBrowseCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Status of this translation unit (2026-09-20 implementation pass)
// ---------------------------------------------------------------------------
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  Each
// body names the entry RVA it was read from; every RVA in this file is an
// mfc140u address (read with `disas.py --u`, resolved through the export
// ordinal table), never an mfc140 (ANSI twin) address.  Every IAT slot named
// below was resolved with iatu.py against mfc140u.
//
// How this class is modelled here.  OpenMFC declares no `class
// CMFCEditBrowseCtrl` in include/openmfc/ and has no detail/*Support.h shadow
// for it, so this file carries the retail layout itself
// (S_CMFCEditBrowseCtrl below, pinned by static_asserts).  Client (MSVC) code
// compiled against the shipping afxeditbrowsectrl.h allocates the object with
// that layout (the RTTI descriptor in featurepack/controls/RuntimeClasses.cpp
// records the same 336 == 0x150 bytes) and inlines GetMode() as a read of
// m_Mode at +0x13c, so the members live in the object's own bytes at the
// retail offsets.
//
// ---------------------------------------------------------------------------
// Retail CMFCEditBrowseCtrl layout, transcribed from the constructor
// ??0CMFCEditBrowseCtrl@@QEAA@XZ (entry RVA 0x5f270, mfc140u) and
// cross-checked against the destructor (entry RVA 0x5f3d0, mfc140u); member
// names and their order are quoted from the shipping afxeditbrowsectrl.h
// (read on this host at atlmfc/include/afxeditbrowsectrl.h), which declares
// them in exactly this order.
// ---------------------------------------------------------------------------
//   +0x000  CEdit base (== CWnd, 0xe8 bytes; the retail ctor calls
//           ??0CWnd@@QEAA@XZ, 0x28a700 -- CEdit's ctor is inline -- and then
//           stores the CMFCEditBrowseCtrl vftable 0x1802e8798 (mfc140u);
//           OpenMFC keeps the vptr ??0CWnd@@QEAA@XZ installs, see below)
//   +0x040  CWnd::m_hWnd            (every retail body reads the HWND there)
//   +0x0a0  CWnd::m_hWndOwner       (read by OnAfterUpdate's inlined GetOwner())
//   +0x0e8  int   m_nBrowseButtonWidth   ctor stores 20 (0x14)
//   +0x0ec  BOOL  m_bIsButtonPressed     ctor: one 8-byte zero store covers
//   +0x0f0  BOOL  m_bIsButtonHighlighted   +0xec/+0xf0
//   +0x0f4  BOOL  m_bIsButtonCaptured    ctor stores 0
//   +0x0f8  BOOL  m_bDefaultImage        ctor stores 1
//   +0x0fc  CRect m_rectBtn              ctor zeroes it (two 8-byte stores)
//                                        and then calls ::SetRectEmpty on it
//   +0x10c  CSize m_sizeImage            ctor zeroes it (twice)
//   +0x118  CString m_strDefFileExt      ctor: string manager's nil string
//   +0x120  CString m_strFileFilter      (IAtlStringMgr slot 3 GetNilString
//   +0x130  CString m_strBrowseFolderTitle  on the static at 0x3b25e8, + 0x18)
//   +0x128  DWORD m_dwFileDialogFlags    ctor stores 6
//                                        (OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT)
//   +0x138  UINT  m_ulBrowseFolderFlags  NOT written by the retail ctor
//   +0x13c  BrowseMode m_Mode            ctor stores 0 (BrowseMode_None)
//   +0x140  CImageList m_ImageBrowse     ctor stores the CImageList vftable
//                                        0x180337b38 (mfc140u) + NULL handle
//                                        at +0x148 (inlined CImageList ctor)
//   sizeof == 0x150
//
// The retail destructor (0x5f3d0) does exactly: store the class vftable,
// CImageList::~CImageList(&m_ImageBrowse) (0x296be0), release the three
// CStrings in reverse order (+0x130, +0x120, +0x118: decrement
// CStringData::nRefs and call pStringMgr->Free() at zero), then tail-jump to
// ??1CEdit@@UEAA@XZ (0x294370).
//
// vptr.  ??0CWnd@@QEAA@XZ (core/window/CtorDtorPlacement.cpp) placement-news
// OpenMFC's CWnd, so an object built here carries OpenMFC's mingw CWnd vtable
// and there is no MSVC-layout vtable for this class.  The retail bodies make
// five virtual calls on `this`, through the CMFCEditBrowseCtrl vftable at
// 0x1802e8798 (mfc140u, dumped slot by slot):
//   +0x2d8 slot 91  OnBrowse            +0x2e0 slot 92  OnDrawBrowseButton
//   +0x2e8 slot 93  OnChangeLayout      +0x2f0 slot 94  OnAfterUpdate
//   +0x2f8 slot 95  OnIllegalFileName   (+0x228 slot 69 is PreTranslateMessage)
// Those slot numbers mean something else on OpenMFC's mingw table, so each
// site below goes through Dispatch* helpers: when the object still carries
// the vptr this file's constructor recorded (g_ownVptr), the call is made
// directly to this file's own thunk; when the vptr is anything else -- a
// client-derived class compiled against the real headers installs its own
// MSVC vftable after our constructor returns -- the call is made through that
// table's slot exactly as retail does, so a client override IS reached.  The
// destructor stores g_ownVptr back first (retail's "vfptr = &vftable" store),
// so that ??1CEdit@@'s C++ virtual-destructor call dispatches on OpenMFC's
// table and not on the client's.
//
// The visual manager.  OnDrawBrowseButton calls
// CMFCVisualManager::GetInstance()->OnDrawBrowseButton (vslot 113, +0x388)
// and ->IsOffsetPressedButton (vslot 189, +0x5e8).  Retail's GetInstance is
// an unexported helper (entry 0x9774, mfc140u: test
// ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA at 0x3be3c0; when NULL, create
// ?m_pRTIDefault@CMFCVisualManager@@ (0x3be3b8) ->CreateObject() (0x234d60)
// or, when that is NULL too, a 0x108-byte CMFCVisualManager
// (??0CMFCVisualManager@@QEAA@H@Z 0x184030, bTemporary = FALSE); store it,
// set its +0x104 flag to 1 and call its vslot 14).
// OpenMFC's creating half is a C++ static
// (visualmanager/CMFCVisualManager.cpp:715) that BRIEFING §1 forbids calling
// from here, so this file reads the exported static only, as
// featurepack/controls/CMFCStatusBar.cpp and toolbar/CMFCToolBarComboBoxButton.cpp
// do, and a NULL manager is treated as the base manager (whose OpenMFC thunk
// returns TRUE without drawing).  OpenMFC's CMFCVisualManager has no
// OnDrawBrowseButton virtual and IsOffsetPressedButton is inline in the
// shipping header (no export, no thunk), so the two retail vtable slots are
// emulated from the manager's runtime class: the slot-113 / slot-189 entries
// of every retail visual-manager vftable were read (mfc140u):
//   CMFCVisualManager  0x31c128: slot113 = base   (0x18af60)  slot189 = 0x3a60 (`mov eax,1; ret`)
//   OfficeXP           0x31d588: slot113 = XP     (0x1b3010)  slot189 = 0x3a60 (TRUE)
//   Office2003         0x31c770: slot113 = 2003   (0x196cc0)  slot189 = 0x71e0 (`xor eax,eax; ret`)
//   Office2007         0x31ce98: slot113 = 2003              slot189 = 0x71e0 (FALSE)
//   VS2005             0x31dbd0: slot113 = 2003              slot189 = 0x71e0 (FALSE)
//   VS2008             0x31e268: slot113 = 2003              slot189 = 0x71e0 (FALSE)
//   Windows            0x31e908: slot113 = Windows(0x1bb670)  slot189 = 0x3a60 (TRUE)
//   Windows7           0x31ef80: slot113 = Windows            slot189 = 0x3a60 (TRUE)
// VmDrawBrowseButton / VmIsOffsetPressedButton below reproduce that table
// through the exported ?IsKindOf@CObject@@ and the GetThisClass thunks (most
// derived classes tested first, because OpenMFC's visual-manager hierarchy is
// flat -- afxmfc.h derives every manager from CMFCVisualManager directly).
//
// afxGlobalData.  ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A (0x3c1620, mfc140u) is
// a zeroed 720-byte block in OpenMFC whose Initialize thunk is an empty stub.
// Retail reads clrBtnText at +0x34 (UpdateSysColors, entry 0x6b1c0 mfc140u,
// stores GetSysColor(COLOR_BTNTEXT = 18) there at 0x6b2ab) and the
// Is32BitIcons() quartet +0x268 m_bUseBuiltIn32BitIcons / +0x288
// m_nBitsPerPixel / +0x260 m_bIsBlackHighContrast / +0x264
// m_bIsWhiteHighContrast (the inline in afxglobals.h, read on this host).
// The colour falls back to the live GetSysColor value while the block is
// unpopulated (the convention of toolbar/CMFCToolBarDateTimeCtrl.cpp); the
// Is32BitIcons quartet is read as-is, so it reads FALSE here.
//
// CString members are this DLL's own CString (one m_pszData pointer,
// include/openmfc/afxstr.h), constructed from the nil string and released by
// the CStringData refcount -- the two operations the retail ctor/dtor perform
// -- exactly as featurepack/ribbon/CMFCRibbonInfo.cpp does for its layout
// views; the KNOWN GAP about OpenMFC's nil CStringData documented there
// applies to the three strings here as well.
//
// Deviations from retail, each repeated at its site:
//  * the destructor / OnNcPaint / SetBrowseButtonImage / OnBrowse build the
//    retail stack objects (CWindowDC, CRgn, CFileDialog, CTagManager,
//    CMFCToolBarImages) either through the matching impl__ thunks in raw
//    storage or as raw handles (the CRgn: OpenMFC's CRgn ctor is out-of-line
//    C++ with no thunk, so the region is an HRGN);
//  * SetBrowseButtonImage(UINT) loads the bitmap resource itself instead of
//    through CMFCToolBarImages::LoadStr, whose OpenMFC body keeps no image
//    well;
//  * SetInternalImage deletes the HBITMAP it loaded (retail leaves it to a
//    temp-map CGdiObject that is detached, i.e. leaks it);
//  * the constructor zeroes m_ulBrowseFolderFlags and the two alignment
//    holes, which retail leaves uninitialised;
//  * null-`this` guards where retail has only ASSERT_VALID.
// The message map of this class (detail/Mfc04MsgmapSupport.cpp) is still the
// empty CWnd-based table, so the ON_WM_* handlers below are not yet reached by
// the window procedure; see headerRequests.  (Even once that table carries
// the retail entries, OpenMFC's ?OnWndMsg@CWnd@@ -- core/window/CWnd.cpp --
// dispatches only WM_COMMAND / WM_NOTIFY and never walks a message map, and
// OpenMFC's AfxSig enum in include/openmfc/afxwin.h does not use the retail
// signature numbers, so the map alone does not make the handlers reachable.)

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <new>

// ---------------------------------------------------------------------------
// Thunks for DLL-internal calls (BRIEFING §1: C++ methods exist only as their
// extern "C" impl__ thunks).  Every one below was grepped to its definition in
// the file named, and the parameter lists are the ones the definitions carry.
// ---------------------------------------------------------------------------
// This file (exports defined further down that earlier bodies call).
extern "C" void MS_ABI impl__OnBrowse_CMFCEditBrowseCtrl__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnDrawBrowseButton_CMFCEditBrowseCtrl__UEAAXPEAVCDC__VCRect__HH_Z(void* pThis, CDC* pDC, CRect rect, int bIsButtonPressed, int bIsButtonHot);
extern "C" void MS_ABI impl__OnChangeLayout_CMFCEditBrowseCtrl__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnAfterUpdate_CMFCEditBrowseCtrl__UEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__OnIllegalFileName_CMFCEditBrowseCtrl__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pThis, CString* pStrFileName);
extern "C" void MS_ABI impl__SetInternalImage_CMFCEditBrowseCtrl__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SetBrowseButtonImage_CMFCEditBrowseCtrl__QEAAXPEAUHBITMAP____H_Z(void* pThis, HBITMAP hBitmap, int bAutoDestroy);
extern "C" void MS_ABI impl__EnableBrowseButton_CMFCEditBrowseCtrl__QEAAXH_Z(void* pThis, int bEnable);
extern "C" void MS_ABI impl__EnableFileBrowseButton_CMFCEditBrowseCtrl__QEAAXPEB_W0K_Z(void* pThis, const wchar_t* lpszDefExt, const wchar_t* lpszFilter, unsigned long dwFlags);
extern "C" void MS_ABI impl__EnableFolderBrowseButton_CMFCEditBrowseCtrl__QEAAXPEB_WI_Z(void* pThis, const wchar_t* lpszBrowseFolderTitle, unsigned int ulBrowseFolderFlags);
// core/window/CtorDtorPlacement.cpp, core/controls/RuntimeClasses.cpp
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);
// core/window/CWnd.cpp, core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" void MS_ABI impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(const CWnd* pThis, RECT* lpRect);
extern "C" int MS_ABI impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(CWnd* pThis, const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, unsigned int nFlags);
extern "C" void MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);
extern "C" void MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" int MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" int MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg);
// core/gdi/CWindowDC.cpp, core/gdi/CDC.cpp, detail/GdicoreSupport.cpp
extern "C" CWindowDC* MS_ABI impl___0CWindowDC__QEAA_PEAVCWnd___Z(CWindowDC* pThis, CWnd* pWnd);
extern "C" void MS_ABI impl___1CWindowDC__UEAA_XZ(CWindowDC* pThis);
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(CDC* pThis, unsigned long crColor);
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);
extern "C" CGdiObject* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(CDC* pThis, int nIndex);
extern "C" CFont* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(CDC* pThis, CFont* pFont);
// (DLL-internal helper with no export of its own -- CDC::DrawText is inline
// in the shipping headers; the same helper toolbar/CMFCToolBarComboBoxButton.cpp uses)
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);
// core/gdi/CImageList.cpp
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* pThis);
extern "C" void MS_ABI impl___1CImageList__UEAA_XZ(CImageList* pThis);
extern "C" int MS_ABI impl__DeleteImageList_CImageList__QEAAHXZ(CImageList* pThis);
extern "C" int MS_ABI impl__Create_CImageList__QEAAHHHIHH_Z(CImageList* pThis, int cx, int cy, unsigned int nFlags, int nInitial, int nGrow);
// core/collections/CStringT.cpp
extern "C" CString* MS_ABI impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis);
extern "C" CString* MS_ABI impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(CString* pThis);
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(CString* pThis, HINSTANCE hInst, UINT nID);
// core/runtime/Globals.cpp, detail/RegcoreSupport.cpp, detail/MfcExceptionsSupport.cpp,
// featurepack/CMFC_misc_stubs.cpp, core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszResource, const wchar_t* lpszType);
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);   // a member function: `this` in RCX (the definition in core/runtime/AFX_GLOBAL_DATA.cpp is an empty stub that declares no parameter)
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];       // 0x3c1620 (mfc140u)
extern "C" void* impl__afxShellManager__3PEAVCShellManager__EA;                // 0x3be2c0 (mfc140u), a CShellManager*
// core/runtime/StaticData.cpp : ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA (0x3be3c0, mfc140u)
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
// core/runtime/CObject.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
// featurepack/visualmanager/*.cpp, featurepack/visualmanager/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerOffice2007__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManager__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_1_AEAK_Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, void* pEdit, int nState, COLORREF* pClrText);
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerOfficeXP* pThis, CDC* pDC, CRect rect, void* pEdit, int state, unsigned long* pclrText);
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerOffice2003* pThis, CDC* pDC, CRect rect, void* pEdit, CMFCVisualManager::AFX_BUTTON_STATE state, unsigned long& clrText);
extern "C" int MS_ABI impl__OnDrawBrowseButton_CMFCVisualManagerWindows__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
    CMFCVisualManagerWindows* pThis, CDC* pDC, CRect rect, void* pEdit, int state, unsigned long* pclrText);
// core/app/CShellManager.cpp.  Retail's parameter order (read out of
// ?BrowseForFolder@CShellManager@@, entry 0x12cae0 mfc140u: the 4th argument
// is stored in this+0x10 for the BFFCALLBACK's initial-folder selection and
// the 5th becomes BROWSEINFO.lpszTitle) is
//   (this, CString& strOutFolder, CWnd* pWndParent, LPCTSTR lpszInitialFolder,
//    LPCTSTR lpszTitle, UINT ulFlags, LPINT piFolderImage);
// the OpenMFC definition names its 4th parameter `title` and its 5th
// `initialFolder` -- the reverse -- so a call in the retail order shows the
// title where the initial folder belongs (headerRequests).  This file calls
// it in the retail order.
extern "C" int MS_ABI impl__BrowseForFolder_CShellManager__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCWnd__PEB_W2IPEAH_Z(
    void* pThis, CString* pStrOutFolder, CWnd* pWndParent, const wchar_t* lpszInitialFolder, const wchar_t* lpszTitle, UINT ulFlags, int* piFolderImage);
// core/dialog/CFileDialog.cpp, core/dialog/RuntimeClasses.cpp, detail/DlgcommonSupport.cpp.
// GetPathName returns a CString by value; the retail call site (0x5ffcd inside
// OnBrowse, 0x5fbc0) passes `this` in RCX and the hidden return slot in RDX,
// but the OpenMFC definition (detail/DlgcommonSupport.cpp:77) takes
// (CString* __ret, const CFileDialog* pThis) -- the reverse.  It is called
// here exactly as it is defined, so the in-DLL call works; the export-ABI
// defect is reported in headerRequests.
extern "C" void* MS_ABI impl___0CFileDialog__QEAA_HPEB_W0K0PEAVCWnd__KH_Z(
    void* p, int bOpenFileDialog, const wchar_t* lpszDefExt, const wchar_t* lpszFileName, unsigned long dwFlags,
    const wchar_t* lpszFilter, CWnd* pParentWnd, unsigned long dwSize, int bVistaStyle);
extern "C" intptr_t MS_ABI impl__DoModal_CFileDialog__UEAA_JXZ(CFileDialog* pThis);
extern "C" void MS_ABI impl__GetPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(CString* __ret, const CFileDialog* pThis);
extern "C" void MS_ABI impl___1CFileDialog__UEAA_XZ(CFileDialog* pThis);
// featurepack/controls/CTagManager.cpp, featurepack/controls/CMFCControlContainer.cpp
extern "C" void MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* self, const wchar_t* buffer);
extern "C" void MS_ABI impl___1CTagManager__UEAA_XZ(void* self);
extern "C" int MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(void* self, const wchar_t* tag, CString* value, int trim);
extern "C" int MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const char* pszUTF8, CString* pStrResult, int cbUTF8);

namespace {

// ---------------------------------------------------------------------------
// Retail layout (see the map in the file header).
// ---------------------------------------------------------------------------
enum BrowseMode { BrowseMode_None = 0, BrowseMode_Default = 1, BrowseMode_File = 2, BrowseMode_Folder = 3 };

struct S_CMFCEditBrowseCtrl {
    unsigned char cedit[0xe8];              // CEdit base (== CWnd, 0xe8 bytes)
    int      m_nBrowseButtonWidth;          // +0xe8
    BOOL     m_bIsButtonPressed;            // +0xec
    BOOL     m_bIsButtonHighlighted;        // +0xf0
    BOOL     m_bIsButtonCaptured;           // +0xf4
    BOOL     m_bDefaultImage;               // +0xf8
    RECT     m_rectBtn;                     // +0xfc   CRect
    SIZE     m_sizeImage;                   // +0x10c  CSize
    int      pad_114;                       // +0x114  alignment hole (not written by retail)
    CString  m_strDefFileExt;               // +0x118
    CString  m_strFileFilter;               // +0x120
    DWORD    m_dwFileDialogFlags;           // +0x128
    int      pad_12c;                       // +0x12c  alignment hole (not written by retail)
    CString  m_strBrowseFolderTitle;        // +0x130
    UINT     m_ulBrowseFolderFlags;         // +0x138
    int      m_Mode;                        // +0x13c  BrowseMode
    unsigned char m_ImageBrowse[0x10];      // +0x140  CImageList {vptr, m_hImageList}
};
static_assert(sizeof(S_CMFCEditBrowseCtrl) == 0x150, "retail sizeof(CMFCEditBrowseCtrl) == 336");
static_assert(sizeof(CWnd) == 0xe8, "OpenMFC's CWnd is 0xe8 bytes, the retail CEdit size");
static_assert(sizeof(CEdit) == sizeof(CWnd), "OpenMFC's CEdit adds no members to CWnd");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_nBrowseButtonWidth) == 0xe8, "m_nBrowseButtonWidth");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_bIsButtonPressed) == 0xec, "m_bIsButtonPressed");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_bIsButtonHighlighted) == 0xf0, "m_bIsButtonHighlighted");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_bIsButtonCaptured) == 0xf4, "m_bIsButtonCaptured");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_bDefaultImage) == 0xf8, "m_bDefaultImage");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_rectBtn) == 0xfc, "m_rectBtn");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_sizeImage) == 0x10c, "m_sizeImage");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_strDefFileExt) == 0x118, "m_strDefFileExt");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_strFileFilter) == 0x120, "m_strFileFilter");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_dwFileDialogFlags) == 0x128, "m_dwFileDialogFlags");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_strBrowseFolderTitle) == 0x130, "m_strBrowseFolderTitle");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_ulBrowseFolderFlags) == 0x138, "m_ulBrowseFolderFlags");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_Mode) == 0x13c, "m_Mode");
static_assert(offsetof(S_CMFCEditBrowseCtrl, m_ImageBrowse) == 0x140, "m_ImageBrowse");
static_assert(sizeof(CString) == sizeof(void*), "CString is one m_pszData pointer");
static_assert(sizeof(CImageList) <= 0x10, "OpenMFC's CImageList must fit the retail 0x10-byte member");
static_assert(offsetof(CImageList, m_hImageList) == 8, "CImageList::m_hImageList at +8 == retail +0x148");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");

constexpr int kOffHWndOwner = 0xa0;   // CWnd::m_hWndOwner (see OwnerOf)

// Retail vftable slots of CMFCEditBrowseCtrl (0x1802e8798, mfc140u).
constexpr int kSlotOnBrowse           = 91;   // +0x2d8
constexpr int kSlotOnDrawBrowseButton = 92;   // +0x2e0
constexpr int kSlotOnChangeLayout     = 93;   // +0x2e8
constexpr int kSlotOnAfterUpdate      = 94;   // +0x2f0
constexpr int kSlotOnIllegalFileName  = 95;   // +0x2f8

// Message / flag constants the retail bodies use as immediates.
constexpr unsigned int kRdwFrameInvalidate = RDW_INVALIDATE | RDW_FRAME;                    // 0x401
constexpr unsigned int kRdwParentRefresh   = RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_FRAME;  // 0x481
constexpr unsigned int kIdpInvalidFileName = 0xF100;    // AFX_IDP_INVALID_FILENAME (afxres.h)
constexpr unsigned int kIdbBrowse          = 17032;     // IDB_AFXBARRES_BROWSE   (afxribbonres.h) == 0x4288
constexpr unsigned int kIdbBrowse32        = 17049;     // IDB_AFXBARRES_BROWSE32 (afxribbonres.h) == 0x4299
constexpr unsigned int kWmMfcInitCtrl      = 0x037C;    // WM_MFC_INITCTRL (afxpriv.h)
static_assert(kRdwFrameInvalidate == 0x401 && kRdwParentRefresh == 0x481, "RDW immediates");
static_assert((OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT) == 6, "the ctor's m_dwFileDialogFlags immediate");
static_assert((DT_CENTER | DT_VCENTER | DT_SINGLELINE) == 0x25, "the DrawText format immediate");

inline S_CMFCEditBrowseCtrl* L(void* p)  { return static_cast<S_CMFCEditBrowseCtrl*>(p); }
inline CWnd*  W(void* p)                 { return static_cast<CWnd*>(p); }
inline HWND   HWndOf(void* p)            { return W(p)->m_hWnd; }
inline CImageList* Images(void* p)       { return reinterpret_cast<CImageList*>(L(p)->m_ImageBrowse); }

// CPoint passed by value is an 8-byte aggregate in a register under MS_ABI.
inline POINT PtFrom(long long v) { POINT pt; std::memcpy(&pt, &v, sizeof pt); return pt; }

// ENSURE(): retail calls ?AfxThrowInvalidArgException@@YAXXZ (0x227720).
inline void ThrowInvalidArg() { impl__AfxThrowInvalidArgException__YAXXZ(); }

// ---------------------------------------------------------------------------
// Virtual dispatch on `this` (file header).
// ---------------------------------------------------------------------------
void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run
inline bool HasOwnVptr(const void* p) {
    return g_ownVptr != nullptr && *reinterpret_cast<void* const*>(p) == g_ownVptr;
}
inline void* SlotOf(void* p, int slot) { return (*reinterpret_cast<void***>(p))[slot]; }

inline void DispatchOnBrowse(void* p) {
    if (HasOwnVptr(p)) { impl__OnBrowse_CMFCEditBrowseCtrl__UEAAXXZ(p); return; }
    using Fn = void (MS_ABI*)(void*);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnBrowse))(p);
}
inline void DispatchOnDrawBrowseButton(void* p, CDC* pDC, CRect rect, int bPressed, int bHot) {
    if (HasOwnVptr(p)) {
        impl__OnDrawBrowseButton_CMFCEditBrowseCtrl__UEAAXPEAVCDC__VCRect__HH_Z(p, pDC, rect, bPressed, bHot);
        return;
    }
    using Fn = void (MS_ABI*)(void*, CDC*, CRect, int, int);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnDrawBrowseButton))(p, pDC, rect, bPressed, bHot);
}
inline void DispatchOnChangeLayout(void* p) {
    if (HasOwnVptr(p)) { impl__OnChangeLayout_CMFCEditBrowseCtrl__UEAAXXZ(p); return; }
    using Fn = void (MS_ABI*)(void*);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnChangeLayout))(p);
}
inline void DispatchOnAfterUpdate(void* p) {
    if (HasOwnVptr(p)) { impl__OnAfterUpdate_CMFCEditBrowseCtrl__UEAAXXZ(p); return; }
    using Fn = void (MS_ABI*)(void*);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnAfterUpdate))(p);
}
inline int DispatchOnIllegalFileName(void* p, CString* pStr) {
    if (HasOwnVptr(p)) {
        return impl__OnIllegalFileName_CMFCEditBrowseCtrl__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(p, pStr);
    }
    using Fn = int (MS_ABI*)(void*, CString*);
    return reinterpret_cast<Fn>(SlotOf(p, kSlotOnIllegalFileName))(p, pStr);
}

// CWnd::GetOwner() as retail inlines it (OnAfterUpdate, 0x60870):
//     m_hWndOwner != NULL ? CWnd::FromHandle(m_hWndOwner) : CWnd::FromHandle(::GetParent(m_hWnd))
// m_hWndOwner (CWnd +0xa0) is not NAMED by OpenMFC's CWnd -- it lies inside
// the zero-filled _cwnd_padding2 -- but MSVC clients inline CWnd::SetOwner and
// store the owner HWND there, so it is read by offset exactly as
// featurepack/controls/CMFCReBar.cpp (RawOwnerHwnd) does.
inline CWnd* OwnerOf(void* p) {
    HWND hOwner = nullptr;
    std::memcpy(&hOwner, static_cast<const char*>(p) + kOffHWndOwner, sizeof hOwner);
    if (hOwner == nullptr) hOwner = ::GetParent(HWndOf(p));
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOwner);
}
// CWnd::SetCapture() as retail inlines it: CWnd::FromHandle(::SetCapture(m_hWnd)).   // 0x28ad70
inline void SetCaptureOn(void* p) {
    impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::SetCapture(HWndOf(p)));
}

// ---------------------------------------------------------------------------
// afxGlobalData (file header).
// ---------------------------------------------------------------------------
constexpr int kGlobalDataInitGate            = 0x000;
constexpr int kGlobalDataClrBtnText          = 0x034;
constexpr int kGlobalDataBrWindowHandle      = 0x110;
constexpr int kGlobalDataIsBlackHighContrast = 0x260;
constexpr int kGlobalDataIsWhiteHighContrast = 0x264;
constexpr int kGlobalDataUseBuiltIn32BitIcons = 0x268;
constexpr int kGlobalDataBitsPerPixel        = 0x288;

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline void* GlobalDataPtr(int off) { void* v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
// The retail one-time gate every reader does first:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }    // 0x6a790
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
// The block is populated once UpdateSysColors has created brWindow (+0x110).
inline bool GlobalDataPopulated() { return GlobalDataPtr(kGlobalDataBrWindowHandle) != nullptr; }
inline COLORREF GlobalColor(int off, int sysColorIndex) {
    EnsureGlobalDataInitialized();
    if (GlobalDataPopulated()) return static_cast<COLORREF>(GlobalDataInt(off));
    return ::GetSysColor(sysColorIndex);
}
// AFX_GLOBAL_DATA::Is32BitIcons() (inline in afxglobals.h), as SetInternalImage
// (0x606f0) inlines it at 0x60736..0x60761.
inline bool GlobalIs32BitIcons() {
    EnsureGlobalDataInitialized();
    return GlobalDataInt(kGlobalDataUseBuiltIn32BitIcons) != 0 &&
           GlobalDataInt(kGlobalDataBitsPerPixel) >= 16 &&
           GlobalDataInt(kGlobalDataIsBlackHighContrast) == 0 &&
           GlobalDataInt(kGlobalDataIsWhiteHighContrast) == 0;
}

// ---------------------------------------------------------------------------
// The visual manager (file header).
// ---------------------------------------------------------------------------
enum VmFamily { kVmBase, kVmOfficeXP, kVmOffice2003, kVmWindows };

inline CMFCVisualManager* VisualManager() {
    return static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
}
inline bool VmIs(CMFCVisualManager* pVM, CRuntimeClass* pClass) {
    return pClass != nullptr && impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pVM, pClass) != 0;
}
// Which retail vftable the manager would carry (the table in the file header).
VmFamily VmFamilyOf(CMFCVisualManager* pVM) {
    if (pVM == nullptr) return kVmBase;
    if (VmIs(pVM, impl__GetThisClass_CMFCVisualManagerWindows7__SAPEAUCRuntimeClass__XZ()) ||
        VmIs(pVM, impl__GetThisClass_CMFCVisualManagerWindows__SAPEAUCRuntimeClass__XZ()))    return kVmWindows;
    if (VmIs(pVM, impl__GetThisClass_CMFCVisualManagerVS2008__SAPEAUCRuntimeClass__XZ()) ||
        VmIs(pVM, impl__GetThisClass_CMFCVisualManagerVS2005__SAPEAUCRuntimeClass__XZ()) ||
        VmIs(pVM, impl__GetThisClass_CMFCVisualManagerOffice2007__SAPEAUCRuntimeClass__XZ()) ||
        VmIs(pVM, impl__GetThisClass_CMFCVisualManagerOffice2003__SAPEAUCRuntimeClass__XZ())) return kVmOffice2003;
    if (VmIs(pVM, impl__GetThisClass_CMFCVisualManagerOfficeXP__SAPEAUCRuntimeClass__XZ()))    return kVmOfficeXP;
    return kVmBase;
}
// CMFCVisualManager::GetInstance()->OnDrawBrowseButton(pDC, rect, pEdit, state, clrText)   // vslot 113 (+0x388)
// A NULL manager is treated as the base manager (file header).
int VmDrawBrowseButton(CMFCVisualManager* pVM, CDC* pDC, CRect rect, void* pEdit, int state, COLORREF* pClrText) {
    switch (VmFamilyOf(pVM)) {
    case kVmWindows:
        return impl__OnDrawBrowseButton_CMFCVisualManagerWindows__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
            reinterpret_cast<CMFCVisualManagerWindows*>(pVM), pDC, rect, pEdit, state, pClrText);
    case kVmOffice2003:
        return impl__OnDrawBrowseButton_CMFCVisualManagerOffice2003__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
            reinterpret_cast<CMFCVisualManagerOffice2003*>(pVM), pDC, rect, pEdit,
            static_cast<CMFCVisualManager::AFX_BUTTON_STATE>(state), *pClrText);
    case kVmOfficeXP:
        return impl__OnDrawBrowseButton_CMFCVisualManagerOfficeXP__MEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_CMFCVisualManager__AEAK_Z(
            reinterpret_cast<CMFCVisualManagerOfficeXP*>(pVM), pDC, rect, pEdit, state, pClrText);
    case kVmBase:
    default:
        if (pVM == nullptr) return TRUE;   // what the base thunk below returns for any non-NULL manager
        return impl__OnDrawBrowseButton_CMFCVisualManager__UEAAHPEAVCDC__VCRect__PEAVCMFCEditBrowseCtrl__W4AFX_BUTTON_STATE_1_AEAK_Z(
            pVM, pDC, rect, pEdit, state, pClrText);
    }
}
// CMFCVisualManager::GetInstance()->IsOffsetPressedButton()   // vslot 189 (+0x5e8)
inline bool VmIsOffsetPressedButton(CMFCVisualManager* pVM) {
    return VmFamilyOf(pVM) != kVmOffice2003;
}

// The ILC_* flag word the three SetBrowseButtonImage bodies and SetInternalImage
// derive from BITMAP.bmBitsPixel (the same five-way switch in each, e.g.
// 0x602fb..0x60337 in the HICON overload): ILC_MASK | ILC_COLORn, ILC_COLOR4 by default.
inline UINT ImageListFlagsFor(WORD bmBitsPixel) {
    switch (bmBitsPixel) {
    case 4:  return ILC_MASK | ILC_COLOR4;    // 0x05
    case 8:  return ILC_MASK | ILC_COLOR8;    // 0x09
    case 16: return ILC_MASK | ILC_COLOR16;   // 0x11
    case 24: return ILC_MASK | ILC_COLOR24;   // 0x19
    case 32: return ILC_MASK | ILC_COLOR32;   // 0x21
    default: return ILC_MASK | ILC_COLOR4;    // 0x05
    }
}
static_assert((ILC_MASK | ILC_COLOR32) == 0x21 && (ILC_MASK | ILC_COLOR4) == 0x5, "ILC immediates");

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail ??0CMFCEditBrowseCtrl@@QEAA@XZ (entry RVA 0x5f270, mfc140u), fully transcribed:
//     CWnd::CWnd();                                         // 0x28a700
//     vfptr = &CMFCEditBrowseCtrl::`vftable';               // 0x1802e8798 (kept as OpenMFC's CWnd vptr here)
//     m_rectBtn = {0,0,0,0}; m_sizeImage = {0,0};
//     m_strDefFileExt = m_strFileFilter = m_strBrowseFolderTitle = <nil string>;
//     m_ImageBrowse: vfptr = &CImageList::`vftable' (0x180337b38), m_hImageList = NULL;
//     ::SetRectEmpty(&m_rectBtn);                            // import 0x2c7348
//     m_bIsButtonPressed = m_bIsButtonHighlighted = FALSE;  // one 8-byte store
//     m_bIsButtonCaptured = FALSE;  m_Mode = BrowseMode_None;
//     m_dwFileDialogFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;   // 6
//     m_sizeImage = {0,0};  m_nBrowseButtonWidth = 20;  m_bDefaultImage = TRUE;
// Deviations: m_ulBrowseFolderFlags (+0x138) and the two alignment holes are
// zeroed here (retail leaves them uninitialised; retail only reads
// m_ulBrowseFolderFlags after EnableFolderBrowseButton has set it).  The
// CImageList sub-object gets no vptr (no MSVC-layout table exists; the
// convention of core/frame/CMDIClientAreaWnd.cpp's m_TabIcons).  The vptr
// ??0CWnd@@ installs is recorded for the dispatch helpers (file header).
// Symbol: ??0CMFCEditBrowseCtrl@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCEditBrowseCtrl__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);

    S_CMFCEditBrowseCtrl* self = L(pThis);
    self->m_rectBtn.left = self->m_rectBtn.top = self->m_rectBtn.right = self->m_rectBtn.bottom = 0;
    self->m_sizeImage.cx = self->m_sizeImage.cy = 0;
    self->pad_114 = 0;
    ::new (static_cast<void*>(&self->m_strDefFileExt)) CString();
    ::new (static_cast<void*>(&self->m_strFileFilter)) CString();
    ::new (static_cast<void*>(&self->m_strBrowseFolderTitle)) CString();
    std::memset(self->m_ImageBrowse, 0, sizeof(self->m_ImageBrowse));
    impl___0CImageList__QEAA_XZ(Images(pThis));
    ::SetRectEmpty(&self->m_rectBtn);
    self->m_bIsButtonPressed = FALSE;
    self->m_bIsButtonHighlighted = FALSE;
    self->m_bIsButtonCaptured = FALSE;
    self->m_Mode = BrowseMode_None;
    self->m_dwFileDialogFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    self->pad_12c = 0;
    self->m_ulBrowseFolderFlags = 0;          // not written by retail (see above)
    self->m_sizeImage.cx = self->m_sizeImage.cy = 0;
    self->m_nBrowseButtonWidth = 20;
    self->m_bDefaultImage = TRUE;
    return pThis;
}

// Retail ??1CMFCEditBrowseCtrl@@UEAA@XZ (entry RVA 0x5f3d0, mfc140u), fully transcribed:
//     vfptr = &CMFCEditBrowseCtrl::`vftable';               // 0x1802e8798 -> g_ownVptr here (file header)
//     CImageList::~CImageList(&m_ImageBrowse);             // 0x296be0
//     release m_strBrowseFolderTitle, m_strFileFilter, m_strDefFileExt;  // CStringData refcount, in that order
//     CEdit::~CEdit();                                      // 0x294370 (tail jump)
// Symbol: ??1CMFCEditBrowseCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCEditBrowseCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    impl___1CImageList__UEAA_XZ(Images(pThis));
    self->m_strBrowseFolderTitle.~CString();
    self->m_strFileFilter.~CString();
    self->m_strDefFileExt.~CString();
    impl___1CEdit__UEAA_XZ(static_cast<CEdit*>(W(pThis)));
}

// ===========================================================================
// Mode / layout
// ===========================================================================

// Retail (entry RVA 0x5faa0, mfc140u), fully transcribed:
//     ENSURE(this != NULL && m_hWnd != NULL);               // else 0x227720 AfxThrowInvalidArgException
//     m_Mode = bEnable ? BrowseMode_Default : BrowseMode_None;   // setne
//     m_ImageBrowse.DeleteImageList();                       // 0x296c80, unconditional
//     m_sizeImage = {0,0};                                    // one 8-byte store
//     OnChangeLayout();                                       // vslot 93 (tail jump)
// Symbol: ?EnableBrowseButton@CMFCEditBrowseCtrl@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableBrowseButton_CMFCEditBrowseCtrl__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr || HWndOf(pThis) == nullptr) { ThrowInvalidArg(); return; }
    S_CMFCEditBrowseCtrl* self = L(pThis);
    self->m_Mode = (bEnable != 0) ? BrowseMode_Default : BrowseMode_None;
    impl__DeleteImageList_CImageList__QEAAHXZ(Images(pThis));
    self->m_sizeImage.cx = self->m_sizeImage.cy = 0;
    DispatchOnChangeLayout(pThis);
}

// Retail (entry RVA 0x60590, mfc140u), fully transcribed:
//     ENSURE(this != NULL && m_hWnd != NULL);               // 0x227720
//     m_Mode = BrowseMode_File;
//     m_strDefFileExt.SetString(lpszDefExt == NULL ? L"" : lpszDefExt, wcslen(...));   // 0x2e30, import 0x2c7748
//     m_strFileFilter.SetString(lpszFilter == NULL ? L"" : lpszFilter, wcslen(...));
//     m_dwFileDialogFlags = dwFlags;
//     SetInternalImage();                                     // 0x606f0
//     OnChangeLayout();                                       // vslot 93 (tail jump)
// The SetString calls are this DLL's inline CString assignment (file header).
// Symbol: ?EnableFileBrowseButton@CMFCEditBrowseCtrl@@QEAAXPEB_W0K@Z
extern "C" void MS_ABI impl__EnableFileBrowseButton_CMFCEditBrowseCtrl__QEAAXPEB_W0K_Z(void* pThis, const wchar_t* lpszDefExt, const wchar_t* lpszFilter, unsigned long dwFlags) {
    if (pThis == nullptr || HWndOf(pThis) == nullptr) { ThrowInvalidArg(); return; }
    S_CMFCEditBrowseCtrl* self = L(pThis);
    self->m_Mode = BrowseMode_File;
    self->m_strDefFileExt = (lpszDefExt != nullptr) ? lpszDefExt : L"";
    self->m_strFileFilter = (lpszFilter != nullptr) ? lpszFilter : L"";
    self->m_dwFileDialogFlags = static_cast<DWORD>(dwFlags);
    impl__SetInternalImage_CMFCEditBrowseCtrl__IEAAXXZ(pThis);
    DispatchOnChangeLayout(pThis);
}

// Retail (entry RVA 0x60650, mfc140u), fully transcribed:
//     ENSURE(this != NULL && m_hWnd != NULL && afxShellManager != NULL);   // 0x3be2c0; else 0x227720
//     m_Mode = BrowseMode_Folder;
//     m_strBrowseFolderTitle.SetString(lpszBrowseFolderTitle == NULL ? L"" : lpszBrowseFolderTitle, wcslen(...));   // 0x2e30
//     m_ulBrowseFolderFlags = ulBrowseFolderFlags;
//     SetInternalImage();                                     // 0x606f0
//     OnChangeLayout();                                       // vslot 93 (tail jump)
// In retail, ?afxShellManager@@ (0x3be2c0, mfc140u) is NULL until the
// application calls CWinAppEx::InitShellManager (entry 0x1c7740, mfc140u:
// news a 0x18-byte CShellManager and stores it there), so an application
// that never called it gets CInvalidArgException here.  In OpenMFC the export
// (featurepack/CMFC_misc_stubs.cpp) is a NULL data export that NOTHING in the
// tree ever writes -- core/app/CWinAppEx.cpp's InitShellManager thunk only
// records a flag in its own state map -- so this function throws for every
// caller and the BrowseMode_Folder branch of OnBrowse is unreachable until
// InitShellManager is made to populate the export (the same gap
// CMFCShellListCtrl.cpp / CMFCShellTreeCtrl.cpp record in their headers).
// Symbol: ?EnableFolderBrowseButton@CMFCEditBrowseCtrl@@QEAAXPEB_WI@Z
extern "C" void MS_ABI impl__EnableFolderBrowseButton_CMFCEditBrowseCtrl__QEAAXPEB_WI_Z(void* pThis, const wchar_t* lpszBrowseFolderTitle, unsigned int ulBrowseFolderFlags) {
    if (pThis == nullptr || HWndOf(pThis) == nullptr || impl__afxShellManager__3PEAVCShellManager__EA == nullptr) {
        ThrowInvalidArg();
        return;
    }
    S_CMFCEditBrowseCtrl* self = L(pThis);
    self->m_Mode = BrowseMode_Folder;
    self->m_strBrowseFolderTitle = (lpszBrowseFolderTitle != nullptr) ? lpszBrowseFolderTitle : L"";
    self->m_ulBrowseFolderFlags = ulBrowseFolderFlags;
    impl__SetInternalImage_CMFCEditBrowseCtrl__IEAAXXZ(pThis);
    DispatchOnChangeLayout(pThis);
}

// Retail (entry RVA 0x5fb00, mfc140u), fully transcribed:
//     ENSURE(this != NULL && m_hWnd != NULL);               // 0x227720
//     m_nBrowseButtonWidth = max(20, m_sizeImage.cx + 8);   // cmovl
//     SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);   // 0x2a9a60, flags 0x27
//     if (m_Mode != BrowseMode_None) {
//         ::GetWindowRect(m_hWnd, &m_rectBtn);              // import 0x2c6c08
//         m_rectBtn.left = m_rectBtn.right - m_nBrowseButtonWidth;
//         ScreenToClient(&m_rectBtn);                        // 0x2a32b0 (CWnd's, RTL-aware)
//     } else {
//         ::SetRectEmpty(&m_rectBtn);                        // import 0x2c7348
//     }
// Symbol: ?OnChangeLayout@CMFCEditBrowseCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnChangeLayout_CMFCEditBrowseCtrl__UEAAXXZ(void* pThis) {
    if (pThis == nullptr || HWndOf(pThis) == nullptr) { ThrowInvalidArg(); return; }
    S_CMFCEditBrowseCtrl* self = L(pThis);
    int nWidth = self->m_sizeImage.cx + 8;
    if (nWidth < 20) nWidth = 20;
    self->m_nBrowseButtonWidth = nWidth;
    impl__SetWindowPos_CWnd__QEAAHPEBV1_HHHHI_Z(W(pThis), nullptr, 0, 0, 0, 0,
                                               SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    if (self->m_Mode != BrowseMode_None) {
        ::GetWindowRect(HWndOf(pThis), &self->m_rectBtn);
        self->m_rectBtn.left = self->m_rectBtn.right - self->m_nBrowseButtonWidth;
        impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(W(pThis), &self->m_rectBtn);
    } else {
        ::SetRectEmpty(&self->m_rectBtn);
    }
}

// ===========================================================================
// Images
// ===========================================================================

// Retail (entry RVA 0x606f0, mfc140u), fully transcribed:
//     if (m_ImageBrowse.GetSafeHandle() != NULL) m_ImageBrowse.DeleteImageList();   // 0x296c80
//     <afxGlobalData init gate>                                                       // 0x6a790
//     UINT uiBmpResId = afxGlobalData.Is32BitIcons() ? IDB_AFXBARRES_BROWSE32 : IDB_AFXBARRES_BROWSE;   // 0x4299 : 0x4288
//     HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(uiBmpResId), RT_BITMAP);   // 0x2aeb50
//     HBITMAP hbmp = (HBITMAP)::LoadImage(hInst, MAKEINTRESOURCE(uiBmpResId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);   // import 0x2c71f0
//     if (hbmp == NULL) return;
//     BITMAP bmp; ::GetObject(hbmp, sizeof(BITMAP), &bmp);                            // import 0x2c6290
//     UINT nFlags = ImageListFlagsFor(bmp.bmBitsPixel);
//     m_ImageBrowse.Create(16, 16, nFlags, 0, 0);                                     // 0x296d10
//     ::ImageList_AddMasked(m_ImageBrowse, CGdiObject::FromHandle(hbmp)->m_hObject, RGB(255, 0, 255));   // 0x1c5a8 wrapper; 0x2a3ea0
//     m_sizeImage = CSize(16, 16);  m_bDefaultImage = TRUE;
// Deviations: hbmp is deleted after ImageList_AddMasked has copied it (retail
// hands it to a temp-map CGdiObject that is detached at idle, i.e. never
// deletes it); the CGdiObject::FromHandle round trip is skipped (its
// m_hObject is the handle it was given).  OpenMFC's DLL carries no bitmap
// resources, so unless the application supplies IDB_AFXBARRES_BROWSE the
// LoadImage fails and the control keeps drawing the "..." text.
// Symbol: ?SetInternalImage@CMFCEditBrowseCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__SetInternalImage_CMFCEditBrowseCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    CImageList* pImages = Images(pThis);
    if (pImages->m_hImageList != nullptr) impl__DeleteImageList_CImageList__QEAAHXZ(pImages);

    const UINT uiBmpResId = GlobalIs32BitIcons() ? kIdbBrowse32 : kIdbBrowse;
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(MAKEINTRESOURCEW(uiBmpResId), RT_BITMAP);
    HBITMAP hbmp = static_cast<HBITMAP>(::LoadImageW(hInst, MAKEINTRESOURCEW(uiBmpResId), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
    if (hbmp == nullptr) return;

    BITMAP bmp;
    std::memset(&bmp, 0, sizeof bmp);
    ::GetObjectW(hbmp, sizeof(BITMAP), &bmp);
    const UINT nFlags = ImageListFlagsFor(bmp.bmBitsPixel);
    impl__Create_CImageList__QEAAHHHIHH_Z(pImages, 16, 16, nFlags, 0, 0);
    ::ImageList_AddMasked(pImages->m_hImageList, hbmp, RGB(255, 0, 255));
    ::DeleteObject(hbmp);   // deviation, see above
    self->m_sizeImage.cx = 16;
    self->m_sizeImage.cy = 16;
    self->m_bDefaultImage = TRUE;
}

// Retail (entry RVA 0x60270, mfc140u), fully transcribed:
//     if (m_ImageBrowse.GetSafeHandle() != NULL) m_ImageBrowse.DeleteImageList();   // 0x296c80
//     if (hIcon == NULL) { m_sizeImage = {0,0}; return; }
//     ICONINFO ii; ::GetIconInfo(hIcon, &ii);                                        // import 0x2c7200
//     BITMAP bmp; ::GetObject(ii.hbmColor, sizeof(BITMAP), &bmp);                    // import 0x2c6290
//     m_sizeImage = CSize(bmp.bmWidth, bmp.bmHeight);
//     ::DeleteObject(ii.hbmColor); ::DeleteObject(ii.hbmMask);                       // import 0x2c6278
//     UINT nFlags = ImageListFlagsFor(bmp.bmBitsPixel);
//     m_ImageBrowse.Create(m_sizeImage.cx, m_sizeImage.cy, nFlags, 0, 0);            // 0x296d10
//     ::ImageList_ReplaceIcon(m_ImageBrowse, -1, hIcon);                             // 0x60c78 wrapper (comctl32 "ImageList_ReplaceIcon")
//     m_bDefaultImage = FALSE;
//     if (bAutoDestroy) ::DestroyIcon(hIcon);                                        // import 0x2c7278
// Symbol: ?SetBrowseButtonImage@CMFCEditBrowseCtrl@@QEAAXPEAUHICON__@@H@Z
extern "C" void MS_ABI impl__SetBrowseButtonImage_CMFCEditBrowseCtrl__QEAAXPEAUHICON____H_Z(void* pThis, HICON hIcon, int bAutoDestroy) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    CImageList* pImages = Images(pThis);
    if (pImages->m_hImageList != nullptr) impl__DeleteImageList_CImageList__QEAAHXZ(pImages);
    if (hIcon == nullptr) {
        self->m_sizeImage.cx = self->m_sizeImage.cy = 0;
        return;
    }
    ICONINFO ii;
    std::memset(&ii, 0, sizeof ii);
    ::GetIconInfo(hIcon, &ii);
    BITMAP bmp;
    std::memset(&bmp, 0, sizeof bmp);
    ::GetObjectW(ii.hbmColor, sizeof(BITMAP), &bmp);
    self->m_sizeImage.cx = bmp.bmWidth;
    self->m_sizeImage.cy = bmp.bmHeight;
    ::DeleteObject(ii.hbmColor);
    ::DeleteObject(ii.hbmMask);
    const UINT nFlags = ImageListFlagsFor(bmp.bmBitsPixel);
    impl__Create_CImageList__QEAAHHHIHH_Z(pImages, bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);
    ::ImageList_ReplaceIcon(pImages->m_hImageList, -1, hIcon);
    self->m_bDefaultImage = FALSE;
    if (bAutoDestroy != 0) ::DestroyIcon(hIcon);
}

// Retail (entry RVA 0x60390, mfc140u), fully transcribed:
//     if (m_ImageBrowse.GetSafeHandle() != NULL) m_ImageBrowse.DeleteImageList();   // 0x296c80
//     if (hBitmap == NULL) { m_sizeImage = {0,0}; return; }
//     BITMAP bmp; ::GetObject(hBitmap, sizeof(BITMAP), &bmp);                        // import 0x2c6290
//     m_sizeImage = CSize(bmp.bmWidth, bmp.bmHeight);
//     UINT nFlags = ImageListFlagsFor(bmp.bmBitsPixel);
//     m_ImageBrowse.Create(m_sizeImage.cx, m_sizeImage.cy, nFlags, 0, 0);            // 0x296d10
//     HBITMAP hCopy = (HBITMAP)::CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, 0);          // import 0x2c71f8
//     ::ImageList_AddMasked(m_ImageBrowse, CGdiObject::FromHandle(hCopy)->m_hObject, RGB(192, 192, 192));   // 0x1c5a8; 0x2a3ea0
//     ::DeleteObject(hCopy);                                                         // import 0x2c6278
//     m_bDefaultImage = FALSE;
//     if (bAutoDestroy) ::DeleteObject(hBitmap);
// The CGdiObject::FromHandle round trip is skipped (retail passes NULL to
// ImageList_AddMasked when it returns NULL and the copy's own handle
// otherwise; here the copy's handle is passed, NULL included).
// Symbol: ?SetBrowseButtonImage@CMFCEditBrowseCtrl@@QEAAXPEAUHBITMAP__@@H@Z
extern "C" void MS_ABI impl__SetBrowseButtonImage_CMFCEditBrowseCtrl__QEAAXPEAUHBITMAP____H_Z(void* pThis, HBITMAP hBitmap, int bAutoDestroy) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    CImageList* pImages = Images(pThis);
    if (pImages->m_hImageList != nullptr) impl__DeleteImageList_CImageList__QEAAHXZ(pImages);
    if (hBitmap == nullptr) {
        self->m_sizeImage.cx = self->m_sizeImage.cy = 0;
        return;
    }
    BITMAP bmp;
    std::memset(&bmp, 0, sizeof bmp);
    ::GetObjectW(hBitmap, sizeof(BITMAP), &bmp);
    self->m_sizeImage.cx = bmp.bmWidth;
    self->m_sizeImage.cy = bmp.bmHeight;
    const UINT nFlags = ImageListFlagsFor(bmp.bmBitsPixel);
    impl__Create_CImageList__QEAAHHHIHH_Z(pImages, bmp.bmWidth, bmp.bmHeight, nFlags, 0, 0);
    HBITMAP hCopy = static_cast<HBITMAP>(::CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, 0));
    ::ImageList_AddMasked(pImages->m_hImageList, hCopy, RGB(192, 192, 192));
    ::DeleteObject(hCopy);
    self->m_bDefaultImage = FALSE;
    if (bAutoDestroy != 0) ::DeleteObject(hBitmap);
}

// Retail (entry RVA 0x604c0, mfc140u), fully transcribed:
//     if (m_ImageBrowse.GetSafeHandle() != NULL) m_ImageBrowse.DeleteImageList();   // 0x296c80
//     if (uiBmpResId == 0) { m_sizeImage = {0,0}; return; }
//     CMFCToolBarImages images;                                                       // 0x16b0f0
//     if (images.LoadStr(MAKEINTRESOURCE((WORD)uiBmpResId), NULL, FALSE)) {          // 0x16b6c0
//         HBITMAP hbmp = (HBITMAP)::CopyImage(images.m_hbmImageWell /* +0xa0 */, IMAGE_BITMAP, 0, 0, 0);   // import 0x2c71f8
//         SetBrowseButtonImage(hbmp, TRUE);                                           // 0x60390 (direct call)
//         m_bDefaultImage = FALSE;
//     }
//     images.~CMFCToolBarImages();                                                    // 0x16b550
// DEVIATION: OpenMFC's CMFCToolBarImages::LoadStr (featurepack/toolbar/
// CMFCToolBarImages.cpp) loads the bitmap, deletes it and records only a
// count -- it keeps no image well to copy from.  So the bitmap resource is
// loaded here directly the way retail LoadStr (0x16b6c0) starts:
// AfxFindResourceHandle(name, RT_BITMAP) + LoadImage(IMAGE_BITMAP, 0, 0,
// LR_CREATEDIBSECTION), and handed to the HBITMAP overload with bAutoDestroy
// = TRUE (the CopyImage of the well is skipped, the loaded bitmap itself
// plays the copy's role).  What is lost is LoadStr's post-processing of the
// well (read at 0x16b7df..0x16b8dc inside LoadStr, 0x16b6c0): the
// LR_LOADMAP3DCOLORS load flag when m_bMapTo3DColors (+0x38) is set and
// afxGlobalData.m_bIsBlackHighContrast is clear; a premultiply-alpha pass
// (the unexported helper at 0x16fef0) for >= 32bpp bitmaps; and
// MapBmpTo3dColors (0x16fc10) for 9..31bpp bitmaps when m_bMapTo3DColors is
// set, or for any <= 31bpp bitmap when the display is black-high-contrast
// -- so a bitmap loaded through this overload keeps its resource colours.
// Symbol: ?SetBrowseButtonImage@CMFCEditBrowseCtrl@@QEAAXI@Z
extern "C" void MS_ABI impl__SetBrowseButtonImage_CMFCEditBrowseCtrl__QEAAXI_Z(void* pThis, unsigned int uiBmpResId) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    CImageList* pImages = Images(pThis);
    if (pImages->m_hImageList != nullptr) impl__DeleteImageList_CImageList__QEAAHXZ(pImages);
    if (uiBmpResId == 0) {
        self->m_sizeImage.cx = self->m_sizeImage.cy = 0;
        return;
    }
    const wchar_t* lpszName = MAKEINTRESOURCEW(static_cast<WORD>(uiBmpResId));
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpszName, RT_BITMAP);
    HBITMAP hbmp = static_cast<HBITMAP>(::LoadImageW(hInst, lpszName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
    if (hbmp != nullptr) {
        impl__SetBrowseButtonImage_CMFCEditBrowseCtrl__QEAAXPEAUHBITMAP____H_Z(pThis, hbmp, TRUE);
        self->m_bDefaultImage = FALSE;
    }
}

// ===========================================================================
// Drawing
// ===========================================================================

// Retail (entry RVA 0x5f800, mfc140u), fully transcribed:
//     AFX_BUTTON_STATE state = bIsButtonPressed ? ButtonsIsPressed : (bIsButtonHot ? ButtonsIsHighlighted : ButtonsIsRegular);
//     <afxGlobalData init gate>                                          // 0x6a790
//     COLORREF clrText = afxGlobalData.clrBtnText;                       // +0x34
//     if (!CMFCVisualManager::GetInstance()->OnDrawBrowseButton(pDC, rect, this, state, clrText))   // 0x9774; vslot 113
//         return;
//     if (m_ImageBrowse.GetSafeHandle() != NULL) {
//         int nImage = (m_bDefaultImage && m_Mode == BrowseMode_File) ? 1 : 0;
//         int x = (rect.left + rect.right) / 2 - m_sizeImage.cx / 2;     // signed halves (cdq/sub/sar)
//         int y = (rect.top + rect.bottom) / 2 - m_sizeImage.cy / 2;
//         if (bIsButtonPressed && CMFCVisualManager::GetInstance()->IsOffsetPressedButton()) { x++; y++; }   // vslot 189
//         ::ImageList_Draw(m_ImageBrowse, nImage, pDC->GetSafeHdc(), x, y, ILD_NORMAL);   // 0x60d64 wrapper (comctl32 "ImageList_Draw")
//     } else {
//         COLORREF clrOld = pDC->SetTextColor(clrText);                  // vslot 14
//         int nOldMode = pDC->SetBkMode(TRANSPARENT);                    // 0x2a2860
//         CGdiObject* pOldFont = pDC->SelectStockObject(DEFAULT_GUI_FONT);   // vslot 11
//         CRect rc = rect; rc.InflateRect(-1, -2); rc.OffsetRect(0, -2);    // imports 0x2c72e8, 0x2c72f0
//         if (bIsButtonPressed) rc.OffsetRect(1, 1);
//         CString str(L"...");                                           // 0xdcb0
//         pDC->DrawText(str, str.GetLength(), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);   // vslot 28
//         pDC->SetTextColor(clrOld);  pDC->SetBkMode(nOldMode);  pDC->SelectObject((CFont*)pOldFont);   // vslots 14, -, 12
//     }
// The CDC virtuals are reached through the exported thunks of the same
// functions (CDC::DrawText is inline in the shipping headers; the in-tree
// helper impl__DrawTextW_CDC__ stands in), so a CDC subclass's override
// (CPreviewDC, CMetaFileDC) is not dispatched to.  The visual-manager calls
// go through VmDrawBrowseButton / VmIsOffsetPressedButton (file header).
// Symbol: ?OnDrawBrowseButton@CMFCEditBrowseCtrl@@UEAAXPEAVCDC@@VCRect@@HH@Z
extern "C" void MS_ABI impl__OnDrawBrowseButton_CMFCEditBrowseCtrl__UEAAXPEAVCDC__VCRect__HH_Z(void* pThis, CDC* pDC, CRect rect, int bIsButtonPressed, int bIsButtonHot) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    int state = CMFCVisualManager::ButtonsIsRegular;
    if (bIsButtonPressed != 0)   state = CMFCVisualManager::ButtonsIsPressed;
    else if (bIsButtonHot != 0)  state = CMFCVisualManager::ButtonsIsHighlighted;

    COLORREF clrText = GlobalColor(kGlobalDataClrBtnText, COLOR_BTNTEXT);
    CMFCVisualManager* pVM = VisualManager();
    if (!VmDrawBrowseButton(pVM, pDC, rect, pThis, state, &clrText)) return;

    CImageList* pImages = Images(pThis);
    if (pImages->m_hImageList != nullptr) {
        const int nImage = (self->m_bDefaultImage != 0 && self->m_Mode == BrowseMode_File) ? 1 : 0;
        int x = (rect.left + rect.right) / 2 - self->m_sizeImage.cx / 2;
        int y = (rect.top + rect.bottom) / 2 - self->m_sizeImage.cy / 2;
        if (bIsButtonPressed != 0 && VmIsOffsetPressedButton(pVM)) {
            ++x;
            ++y;
        }
        ::ImageList_Draw(pImages->m_hImageList, nImage, pDC != nullptr ? pDC->m_hDC : nullptr, x, y, ILD_NORMAL);
        return;
    }

    const COLORREF clrOld = impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrText);
    const int nOldMode = impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);
    CGdiObject* pOldFont = impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(pDC, DEFAULT_GUI_FONT);
    RECT rc = { rect.left, rect.top, rect.right, rect.bottom };
    ::InflateRect(&rc, -1, -2);
    ::OffsetRect(&rc, 0, -2);
    if (bIsButtonPressed != 0) ::OffsetRect(&rc, 1, 1);
    CString str(L"...");
    impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, str.GetString(), str.GetLength(), &rc,
                                                   DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, clrOld);
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, nOldMode);
    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, static_cast<CFont*>(pOldFont));
}

// Retail (entry RVA 0x5f600, mfc140u; the WM_NCPAINT entry), fully transcribed:
//     Default();                                                          // 0x28ac80
//     if (m_Mode == BrowseMode_None) return;
//     CWindowDC dc(this);                                                 // 0x2a3c20
//     CRect rectWindow; ::GetWindowRect(m_hWnd, &rectWindow);             // import 0x2c6c08
//     m_rectBtn = rectWindow;  m_rectBtn.left = m_rectBtn.right - m_nBrowseButtonWidth;
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);             // import 0x2c7330
//     ClientToScreen(&rectClient);                                        // 0x2a3310
//     m_rectBtn.OffsetRect(m_nBrowseButtonWidth - (rectWindow.right - rectClient.right), 0);   // import 0x2c72f0
//     m_rectBtn.top    += rectClient.top    - rectWindow.top;
//     m_rectBtn.bottom += rectClient.bottom - rectWindow.bottom;
//     CRect rectBtn = m_rectBtn;  rectBtn.OffsetRect(-rectWindow.left, -rectWindow.top);
//     CRgn rgnClip;  rgnClip.CreateRectRgnIndirect(&rectBtn);             // import 0x2c61e0 + CGdiObject::Attach 0x2a3ed0
//     dc.SelectClipRgn(&rgnClip);                                         // 0x2a2e20
//     OnDrawBrowseButton(&dc, rectBtn, m_bIsButtonPressed, m_bIsButtonHighlighted);   // vslot 92
//     dc.SelectClipRgn(NULL);                                             // 0x2a2e20
//     ScreenToClient(&m_rectBtn);                                         // 0x2a32b0
//     rgnClip.~CRgn();  dc.~CWindowDC();                                  // 0x1c6f0 (the shared CGdiObject dtor body: vftable store + DeleteObject 0x2a3f60), 0x2a3ce0
// The CRgn is a raw HRGN here (OpenMFC's CRgn ctor is out-of-line C++ with
// no thunk).  Retail's CDC::SelectClipRgn(CRgn*) (0x2a2e20) selects the region
// into m_hDC when it differs from m_hAttribDC and then into m_hAttribDC; the
// CWindowDC ctor thunk sets both to the same handle, so the single
// ::SelectClipRgn(m_hDC, ...) below is that body's effect for this DC.
// Symbol: ?OnNcPaint@CMFCEditBrowseCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnNcPaint_CMFCEditBrowseCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    if (self->m_Mode == BrowseMode_None) return;

    // CWindowDC exists in this DLL only as its impl__ thunks, so the object is
    // built in raw storage and torn down through the matching thunk.
    alignas(void*) unsigned char dcStorage[sizeof(CWindowDC)] = {};
    CWindowDC* pDC = reinterpret_cast<CWindowDC*>(dcStorage);
    impl___0CWindowDC__QEAA_PEAVCWnd___Z(pDC, W(pThis));

    RECT rectWindow;
    ::SetRectEmpty(&rectWindow);
    ::GetWindowRect(HWndOf(pThis), &rectWindow);
    self->m_rectBtn = rectWindow;
    self->m_rectBtn.left = self->m_rectBtn.right - self->m_nBrowseButtonWidth;

    RECT rectClient;
    ::SetRectEmpty(&rectClient);
    ::GetClientRect(HWndOf(pThis), &rectClient);
    impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(W(pThis), &rectClient);

    ::OffsetRect(&self->m_rectBtn, self->m_nBrowseButtonWidth - (rectWindow.right - rectClient.right), 0);
    self->m_rectBtn.top    += rectClient.top    - rectWindow.top;
    self->m_rectBtn.bottom += rectClient.bottom - rectWindow.bottom;

    RECT rectBtn = self->m_rectBtn;
    ::OffsetRect(&rectBtn, -rectWindow.left, -rectWindow.top);

    HRGN hRgnClip = ::CreateRectRgnIndirect(&rectBtn);
    ::SelectClipRgn(pDC->m_hDC, hRgnClip);
    DispatchOnDrawBrowseButton(pThis, pDC, CRect(rectBtn), self->m_bIsButtonPressed, self->m_bIsButtonHighlighted);
    ::SelectClipRgn(pDC->m_hDC, nullptr);
    impl__ScreenToClient_CWnd__QEBAXPEAUtagRECT___Z(W(pThis), &self->m_rectBtn);
    if (hRgnClip != nullptr) ::DeleteObject(hRgnClip);
    impl___1CWindowDC__UEAA_XZ(pDC);
}

// ===========================================================================
// Browsing
// ===========================================================================

// Retail (entry RVA 0x5fbc0, mfc140u), fully transcribed:
//     ENSURE(this != NULL && m_hWnd != NULL);                              // 0x227720
//     switch (m_Mode) {
//     case BrowseMode_Folder:                                              // 3
//         if (afxShellManager != NULL) {                                   // 0x3be2c0
//             CString strFolder;  GetWindowText(strFolder);                // 0x28be00
//             CString strResult;
//             if (afxShellManager->BrowseForFolder(strResult, this, strFolder,
//                     m_strBrowseFolderTitle.IsEmpty() ? NULL : m_strBrowseFolderTitle,
//                     m_ulBrowseFolderFlags, NULL)) {                      // 0x12cae0
//                 if (wcscmp(strResult, strFolder) != 0) {                 // import 0x2c7770
//                     SetWindowText(strResult);                            // 0x2a9790
//                     SendMessage(EM_SETMODIFY, TRUE, 0);                  // import 0x2c7120
//                     OnAfterUpdate();                                     // vslot 94
//                 }
//             }
//         }
//         break;
//     case BrowseMode_File: {                                              // 2
//         CString strFile;  GetWindowText(strFile);                        // 0x28be00
//         if (!strFile.IsEmpty()) {
//             TCHAR fname[_MAX_FNAME];
//             _wsplitpath_s(strFile, NULL, 0, NULL, 0, fname, _MAX_FNAME, NULL, 0);   // import 0x2c74a8
//             CString strFileName(fname);                                  // 0xdcb0
//             strFileName.TrimLeft();  strFileName.TrimRight();            // 0x127f0, 0x12750
//             if (strFileName.IsEmpty()) strFile.Empty();                  // 0x33b0
//             CString strInvalidChars(L"*?<>|");
//             if (strFile.FindOneOf(strInvalidChars) >= 0) {               // inlined: wcspbrk (import 0x2c7790)
//                 if (!OnIllegalFileName(strFile)) {                       // vslot 95
//                     SetFocus();                                          // 0x2a9b60
//                     return;
//                 }
//                 // a TRUE return continues with strFile as the override left it
//             }
//         }
//         CFileDialog dlg(TRUE, m_strDefFileExt.IsEmpty() ? NULL : m_strDefFileExt, strFile,
//                         m_dwFileDialogFlags, m_strFileFilter.IsEmpty() ? NULL : m_strFileFilter,
//                         NULL, 0, TRUE);                                  // 0x20a1c0
//         BOOL bChanged = dlg.DoModal() == IDOK && wcscmp(strFile, dlg.GetPathName()) != 0;   // 0x20c0a0, 0x20c350
//         if (bChanged) {
//             SetWindowText(dlg.GetPathName());                            // 0x20c350 again, 0x2a9790
//             SendMessage(EM_SETMODIFY, TRUE, 0);                          // import 0x2c7120
//             OnAfterUpdate();                                             // vslot 94
//         }
//         if (GetParent() != NULL)                                         // import 0x2c72d8 + 0x28ad70
//             GetParent()->RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_FRAME);   // 0x481, import 0x2c7130
//         dlg.~CFileDialog();                                              // 0x20a5b0
//         break; }
//     }
//     SetFocus();                                                          // 0x2a9b60
// The two AtlThrow(E_FAIL) guards on CString data pointers (0x6011b, 0x6012c)
// cannot fire -- a CString's m_pszData is never NULL -- and are not
// reproduced.  The CFileDialog is OpenMFC's, built by its ctor thunk in raw
// storage; its DoModal (core/dialog/CFileDialog.cpp:85, reached through the
// ?DoModal@CFileDialog@@ thunk in detail/DlgcommonSupport.cpp) runs
// ::GetOpenFileNameW / ::GetSaveFileNameW and returns IDOK with m_strPathName
// set on success, so the file branch does change the text.  (Retail's
// DoModal additionally honours bVistaStyle with IFileDialog; OpenMFC's does
// not.)  The BrowseForFolder / GetPathName argument orders are discussed at
// their declarations.
// Symbol: ?OnBrowse@CMFCEditBrowseCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnBrowse_CMFCEditBrowseCtrl__UEAAXXZ(void* pThis) {
    if (pThis == nullptr || HWndOf(pThis) == nullptr) { ThrowInvalidArg(); return; }
    S_CMFCEditBrowseCtrl* self = L(pThis);
    CWnd* pWnd = W(pThis);
    const HWND hWnd = HWndOf(pThis);

    switch (self->m_Mode) {
    case BrowseMode_Folder: {
        if (impl__afxShellManager__3PEAVCShellManager__EA != nullptr) {
            CString strFolder;
            impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pWnd, &strFolder);
            CString strResult;
            const wchar_t* lpszTitle = self->m_strBrowseFolderTitle.IsEmpty() ? nullptr : self->m_strBrowseFolderTitle.GetString();
            if (impl__BrowseForFolder_CShellManager__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEAVCWnd__PEB_W2IPEAH_Z(
                    impl__afxShellManager__3PEAVCShellManager__EA, &strResult, pWnd, strFolder.GetString(), lpszTitle,
                    self->m_ulBrowseFolderFlags, nullptr)) {
                if (std::wcscmp(strResult.GetString(), strFolder.GetString()) != 0) {
                    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pWnd, strResult.GetString());
                    ::SendMessage(hWnd, EM_SETMODIFY, TRUE, 0);
                    DispatchOnAfterUpdate(pThis);
                }
            }
        }
        break;
    }
    case BrowseMode_File: {
        CString strFile;
        impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(pWnd, &strFile);
        if (!strFile.IsEmpty()) {
            wchar_t fname[_MAX_FNAME];
            std::memset(fname, 0, sizeof fname);
            _wsplitpath_s(strFile.GetString(), nullptr, 0, nullptr, 0, fname, _MAX_FNAME, nullptr, 0);
            CString strFileName(fname);
            impl__TrimLeft___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&strFileName);
            impl__TrimRight___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAAEAV12_XZ(&strFileName);
            if (strFileName.IsEmpty()) strFile.Empty();
            CString strInvalidChars(L"*?<>|");
            const wchar_t* pInvalid = std::wcspbrk(strFile.GetString(), strInvalidChars.GetString());
            if (pInvalid != nullptr && static_cast<int>(pInvalid - strFile.GetString()) >= 0) {
                if (!DispatchOnIllegalFileName(pThis, &strFile)) {
                    impl__SetFocus_CWnd__QEAAPEAV1_XZ(pWnd);
                    return;
                }
            }
        }

        // CFileDialog exists in this DLL only through its impl__ thunks, so the
        // object is built in raw storage and torn down through the dtor thunk.
        alignas(CFileDialog) unsigned char dlgStorage[sizeof(CFileDialog)];
        std::memset(dlgStorage, 0, sizeof dlgStorage);
        CFileDialog* pDlg = reinterpret_cast<CFileDialog*>(dlgStorage);
        impl___0CFileDialog__QEAA_HPEB_W0K0PEAVCWnd__KH_Z(
            pDlg, TRUE,
            self->m_strDefFileExt.IsEmpty() ? nullptr : self->m_strDefFileExt.GetString(),
            strFile.GetString(), self->m_dwFileDialogFlags,
            self->m_strFileFilter.IsEmpty() ? nullptr : self->m_strFileFilter.GetString(),
            nullptr, 0, TRUE);

        bool bChanged = false;
        if (impl__DoModal_CFileDialog__UEAA_JXZ(pDlg) == IDOK) {
            CString strPath;
            impl__GetPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(&strPath, pDlg);
            bChanged = std::wcscmp(strFile.GetString(), strPath.GetString()) != 0;
        }
        if (bChanged) {
            CString strPath;
            impl__GetPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(&strPath, pDlg);
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(pWnd, strPath.GetString());
            ::SendMessage(hWnd, EM_SETMODIFY, TRUE, 0);
            DispatchOnAfterUpdate(pThis);
        }
        if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd)) != nullptr) {
            CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd));
            if (pParent != nullptr) ::RedrawWindow(pParent->m_hWnd, nullptr, nullptr, kRdwParentRefresh);
        }
        impl___1CFileDialog__UEAA_XZ(pDlg);
        break;
    }
    default:
        break;
    }
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(pWnd);
}

// Retail (entry RVA 0x60140, mfc140u), fully transcribed:
//     CString strMessage;
//     HINSTANCE hInst = AfxFindStringResourceHandle(AFX_IDP_INVALID_FILENAME);   // 0x2aee00, id 0xf100
//     ENSURE(hInst != NULL);                                                     // 0x227720
//     ENSURE(strMessage.LoadString(hInst, AFX_IDP_INVALID_FILENAME));            // 0xdb70; 0x227720
//     CString strError;  strError.Format(L"%Ts\r\n%Ts", strFileName, strMessage);   // 0xda00, string at 0x33f2f8
//     ::MessageBoxW(GetSafeHwnd(), strError, AfxGetModuleState()->m_lpszCurrentAppName, MB_OK | MB_ICONEXCLAMATION);   // 0x133930 (+0x20); import 0x2c6ea8
//     return FALSE;
// The Format is built with this DLL's CString concatenation (same text).
// AFX_MODULE_STATE::m_lpszCurrentAppName is not modelled by OpenMFC's module
// state; CWinApp::SetCurrentHandles copies CWinApp::m_pszAppName into it in
// retail, so that member is read instead (the convention of
// core/ole/COleControl.cpp DisplayError).  Retail calls ::MessageBoxW
// directly, not AfxMessageBox.  OpenMFC's DLL carries no string table, so
// unless the application supplies AFX_IDP_INVALID_FILENAME the first ENSURE
// throws CInvalidArgException -- which is what retail does with a missing
// resource as well.
// Symbol: ?OnIllegalFileName@CMFCEditBrowseCtrl@@UEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnIllegalFileName_CMFCEditBrowseCtrl__UEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(void* pThis, CString* pStrFileName) {
    CString strMessage;
    HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(kIdpInvalidFileName));
    if (hInst == nullptr) { ThrowInvalidArg(); return FALSE; }
    if (!impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&strMessage, hInst, kIdpInvalidFileName)) {
        ThrowInvalidArg();
        return FALSE;
    }
    CString strError(pStrFileName != nullptr ? pStrFileName->GetString() : L"");
    strError += L"\r\n";
    strError += strMessage;
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    const wchar_t* lpszCaption = (pApp != nullptr) ? pApp->m_pszAppName : nullptr;
    ::MessageBoxW(pThis != nullptr ? HWndOf(pThis) : nullptr, strError.GetString(), lpszCaption, MB_OK | MB_ICONEXCLAMATION);
    return FALSE;
}

// Retail (entry RVA 0x60870, mfc140u), fully transcribed:
//     if (GetOwner() != NULL) {                                            // inlined: +0xa0 ? : ::GetParent(m_hWnd); 0x28ad70
//         GetOwner()->PostMessage(EN_CHANGE, GetDlgCtrlID(), (LPARAM)m_hWnd);   // 0x2a99a0; import 0x2c72b0, message 0x300
//         GetOwner()->PostMessage(EN_UPDATE, GetDlgCtrlID(), (LPARAM)m_hWnd);   // message 0x400
//     }
// (Retail evaluates GetOwner() three times and passes the EN_* notification
// codes as the message ids themselves; GetDlgCtrlID() is sign-extended into
// WPARAM.)  A NULL guard on the re-evaluated owner is added.
// Symbol: ?OnAfterUpdate@CMFCEditBrowseCtrl@@UEAAXXZ
extern "C" void MS_ABI impl__OnAfterUpdate_CMFCEditBrowseCtrl__UEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (OwnerOf(pThis) == nullptr) return;
    const HWND hWnd = HWndOf(pThis);
    CWnd* pOwner = OwnerOf(pThis);
    if (pOwner != nullptr) {
        ::PostMessage(pOwner->m_hWnd, EN_CHANGE, static_cast<WPARAM>(static_cast<LONG_PTR>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(W(pThis)))),
                      reinterpret_cast<LPARAM>(hWnd));
    }
    pOwner = OwnerOf(pThis);
    if (pOwner != nullptr) {
        ::PostMessage(pOwner->m_hWnd, EN_UPDATE, static_cast<WPARAM>(static_cast<LONG_PTR>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(W(pThis)))),
                      reinterpret_cast<LPARAM>(hWnd));
    }
}

// Retail (entry RVA 0x60ad0, mfc140u), fully transcribed:
//     if (pMsg->message == WM_SYSKEYDOWN && m_Mode != BrowseMode_None &&
//         (pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_DOWN)) {         // (wParam - 0x27) <= 1, unsigned
//         OnBrowse();                                                      // vslot 91
//         return TRUE;
//     }
//     return CWnd::PreTranslateMessage(pMsg);                              // 0x28bc00 (tail jump; CEdit has no override)
// Symbol: ?PreTranslateMessage@CMFCEditBrowseCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCEditBrowseCtrl__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    if (pMsg->message == WM_SYSKEYDOWN && L(pThis)->m_Mode != BrowseMode_None &&
        (pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_DOWN)) {
        DispatchOnBrowse(pThis);
        return TRUE;
    }
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(W(pThis), pMsg);
}

// Retail (entry RVA 0x60b20, mfc140u; the WM_MFC_INITCTRL 0x37c entry), fully transcribed:
//     CString strDst;
//     CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                                          // 0x13d840
//     CString strMode;
//     if (tagManager.ExcludeTag(L"MFCEditBrowse_Mode", strMode, FALSE) && !strMode.IsEmpty()) {   // 0x13df50, string 0x33f310
//         switch (_wtoi(strMode)) {                                            // import 0x2c7470
//         case 0: EnableBrowseButton(FALSE); break;                            // 0x5faa0
//         case 1: EnableFileBrowseButton(NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT); break;   // 0x60590
//         case 2: EnableFolderBrowseButton(NULL, BIF_RETURNONLYFSDIRS); break; // 0x60650, flags 1
//         case 3: EnableBrowseButton(TRUE); break;                             // 0x5faa0
//         }
//     }
//     return 0;                                                                // (~strMode, ~tagManager, ~strDst)
// The CTagManager thunks (controls/CTagManager.cpp) key their state by the
// object address and never touch its storage, so an opaque local buffer
// stands in for the retail object (the convention of CMFCShellListCtrl.cpp).
// The three Enable* calls are direct in retail (non-virtual) and here.
// Symbol: ?OnInitControl@CMFCEditBrowseCtrl@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCEditBrowseCtrl__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));
    alignas(16) unsigned char tagManagerStorage[64];
    std::memset(tagManagerStorage, 0, sizeof tagManagerStorage);
    impl___0CTagManager__QEAA_PEB_W_Z(tagManagerStorage, strDst.GetString());
    {
        CString strMode;
        if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
                tagManagerStorage, L"MFCEditBrowse_Mode", &strMode, FALSE) &&
            !strMode.IsEmpty()) {
            switch (_wtoi(strMode.GetString())) {
            case 0: impl__EnableBrowseButton_CMFCEditBrowseCtrl__QEAAXH_Z(pThis, FALSE); break;
            case 1: impl__EnableFileBrowseButton_CMFCEditBrowseCtrl__QEAAXPEB_W0K_Z(pThis, nullptr, nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT); break;
            case 2: impl__EnableFolderBrowseButton_CMFCEditBrowseCtrl__QEAAXPEB_WI_Z(pThis, nullptr, 1 /* BIF_RETURNONLYFSDIRS */); break;
            case 3: impl__EnableBrowseButton_CMFCEditBrowseCtrl__QEAAXH_Z(pThis, TRUE); break;
            default: break;
            }
        }
    }
    impl___1CTagManager__UEAA_XZ(tagManagerStorage);
    return 0;
}

// ===========================================================================
// Non-client area
// ===========================================================================

// Retail (entry RVA 0x5f5c0, mfc140u; the WM_NCCALCSIZE entry), fully transcribed:
//     Default();                                                           // 0x28ac80
//     if (m_Mode != BrowseMode_None) lpncsp->rgrc[0].right -= m_nBrowseButtonWidth;
// (bCalcValidRects is not read.)
// Symbol: ?OnNcCalcSize@CMFCEditBrowseCtrl@@IEAAXHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" void MS_ABI impl__OnNcCalcSize_CMFCEditBrowseCtrl__IEAAXHPEAUtagNCCALCSIZE_PARAMS___Z(void* pThis, int bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp) {
    (void)bCalcValidRects;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    if (L(pThis)->m_Mode != BrowseMode_None && lpncsp != nullptr) {
        lpncsp->rgrc[0].right -= L(pThis)->m_nBrowseButtonWidth;
    }
}

// Retail (entry RVA 0x5f7a0, mfc140u; the WM_NCHITTEST entry), fully transcribed:
//     CPoint ptClient = point;  ::ScreenToClient(m_hWnd, &ptClient);      // import 0x2c7318 (the raw import, not CWnd's)
//     if (m_Mode != BrowseMode_None && ::PtInRect(&m_rectBtn, ptClient))  // import 0x2c72f8
//         return HTCAPTION;                                                // 2
//     return Default();                                                    // 0x28ac80
// Symbol: ?OnNcHitTest@CMFCEditBrowseCtrl@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCEditBrowseCtrl__IEAA_JVCPoint___Z(void* pThis, long long point) {
    if (pThis == nullptr) return 0;
    POINT pt = PtFrom(point);
    ::ScreenToClient(HWndOf(pThis), &pt);
    if (L(pThis)->m_Mode != BrowseMode_None && ::PtInRect(&L(pThis)->m_rectBtn, pt)) {
        return HTCAPTION;
    }
    return impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Retail (entry RVA 0x27d0, mfc140u), fully transcribed: the body is the
// single instruction `ret 0` (bytes `C2 00 00` between int3 padding).
// Two independent readings agree on that address:
//  * the mfc140u export table maps this export's ordinal (10617) straight
//    to RVA 0x27d0 -- together with 157 other exports whose bodies are
//    likewise empty and which the linker's identical-COMDAT folding merged
//    onto one copy.  mfc140u_rva_symbols.json keeps one name per RVA and
//    happens to keep ?AddDockSite@CFrameWndEx@@QEAAXXZ for it, which is why
//    `disas.py --u` reports this symbol as NOT FOUND;
//  * ?GetMessageMap@CMFCEditBrowseCtrl@@ (entry RVA 0x5f480, mfc140u)
//    returns the AFX_MSGMAP at 0x1802e85e0 (mfc140u), whose WM_NCLBUTTONDBLCLK
//    entry (nMessage 0x00a3, nSig 54 == AfxSig_v_u_p, void (UINT, CPoint) --
//    the same signature the WM_LBUTTONUP / WM_MOUSEMOVE / WM_NCMOUSEMOVE
//    entries carry) points at 0x27d0 as well.
// (0x27d0 is the only bare-`ret` body inside the MFC code of .text; the two
// other `ret 0` islands, 0x2c4f90 and 0x2c50e0, lie in the CRT tail past
// the last MFC export.)  So the retail handler does nothing -- in
// particular it does NOT call Default(), which swallows the double click on
// the whole non-client area (retail's OnNcHitTest reports HTCAPTION over
// the button, and a default WM_NCLBUTTONDBLCLK on HTCAPTION would otherwise
// go to DefWindowProc).  The empty body IS the transcription; there is
// nothing further to implement.
// Symbol: ?OnNcLButtonDblClk@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcLButtonDblClk_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nHitTest, long long point) {
    (void)pThis;
    (void)nHitTest;
    (void)point;
}

// Retail (entry RVA 0x60940, mfc140u; the WM_NCMOUSEMOVE entry), fully transcribed:
//     if (!m_bIsButtonCaptured) {
//         CPoint ptClient = point;  ::ScreenToClient(m_hWnd, &ptClient);  // import 0x2c7318
//         if (::PtInRect(&m_rectBtn, ptClient)) {                          // import 0x2c72f8
//             SetCapture();                                                // import 0x2c7250 + 0x28ad70
//             m_bIsButtonHighlighted = TRUE;
//             RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);        // import 0x2c7130, 0x401
//         }
//     }
//     Default();                                                           // 0x28ac80 (tail jump)
// Symbol: ?OnNcMouseMove@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nHitTest, long long point) {
    (void)nHitTest;
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    if (self->m_bIsButtonCaptured == 0) {
        POINT pt = PtFrom(point);
        ::ScreenToClient(HWndOf(pThis), &pt);
        if (::PtInRect(&self->m_rectBtn, pt)) {
            SetCaptureOn(pThis);
            self->m_bIsButtonHighlighted = TRUE;
            ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwFrameInvalidate);
        }
    }
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// ===========================================================================
// Mouse / capture
// ===========================================================================

// Retail (entry RVA 0x609c0, mfc140u; the WM_CANCELMODE entry), fully transcribed:
//     Default();                                                           // 0x28ac80
//     if (IsWindowEnabled()) ::ReleaseCapture();                           // 0x2a9b00; import 0x2c7258
//     m_bIsButtonPressed = m_bIsButtonHighlighted = FALSE;                 // one 8-byte store
//     m_bIsButtonCaptured = FALSE;
//     RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);                // import 0x2c7130 (tail jump), 0x401
// Symbol: ?OnCancelMode@CMFCEditBrowseCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCEditBrowseCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    if (impl__IsWindowEnabled_CWnd__QEBAHXZ(W(pThis))) ::ReleaseCapture();
    self->m_bIsButtonPressed = FALSE;
    self->m_bIsButtonHighlighted = FALSE;
    self->m_bIsButtonCaptured = FALSE;
    ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwFrameInvalidate);
}

// Retail (entry RVA 0x60a10, mfc140u; the WM_LBUTTONDOWN entry), fully transcribed:
//     if (m_Mode != BrowseMode_None && ::PtInRect(&m_rectBtn, point)) {   // import 0x2c72f8
//         SetFocus();                                                      // 0x2a9b60
//         m_bIsButtonPressed = TRUE;  m_bIsButtonCaptured = TRUE;
//         SetCapture();                                                    // import 0x2c7250 + 0x28ad70
//         RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);            // import 0x2c7130 (tail jump), 0x401
//         return;
//     }
//     Default();                                                           // 0x28ac80 (tail jump)
// (nFlags is not read.)
// Symbol: ?OnLButtonDown@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    const POINT pt = PtFrom(point);
    if (self->m_Mode != BrowseMode_None && ::PtInRect(&self->m_rectBtn, pt)) {
        impl__SetFocus_CWnd__QEAAPEAV1_XZ(W(pThis));
        self->m_bIsButtonPressed = TRUE;
        self->m_bIsButtonCaptured = TRUE;
        SetCaptureOn(pThis);
        ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwFrameInvalidate);
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Retail (entry RVA 0x5f490, mfc140u; the WM_LBUTTONUP entry), fully transcribed:
//     if (m_bIsButtonCaptured) {
//         ::ReleaseCapture();                                              // import 0x2c7258
//         m_bIsButtonPressed = m_bIsButtonHighlighted = FALSE;             // one 8-byte store
//         m_bIsButtonCaptured = FALSE;
//         RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);            // import 0x2c7130, 0x401
//         if (::PtInRect(&m_rectBtn, point)) OnBrowse();                   // import 0x2c72f8; vslot 91
//     } else {
//         Default();                                                       // 0x28ac80
//     }
// (nFlags is not read.)
// Symbol: ?OnLButtonUp@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    if (self->m_bIsButtonCaptured != 0) {
        ::ReleaseCapture();
        self->m_bIsButtonPressed = FALSE;
        self->m_bIsButtonHighlighted = FALSE;
        self->m_bIsButtonCaptured = FALSE;
        ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwFrameInvalidate);
        const POINT pt = PtFrom(point);
        if (::PtInRect(&self->m_rectBtn, pt)) DispatchOnBrowse(pThis);
        return;
    }
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Retail (entry RVA 0x5f520, mfc140u; the WM_MOUSEMOVE entry), fully transcribed:
//     if (m_bIsButtonCaptured) {
//         BOOL bIsPressed = ::PtInRect(&m_rectBtn, point);                 // import 0x2c72f8
//         if (bIsPressed != m_bIsButtonPressed) {
//             m_bIsButtonPressed = bIsPressed;
//             RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);        // import 0x2c7130 (tail jump), 0x401
//         }
//         return;                                                          // no Default() on this path
//     }
//     if (m_bIsButtonHighlighted && !::PtInRect(&m_rectBtn, point)) {
//         m_bIsButtonHighlighted = FALSE;
//         ::ReleaseCapture();                                              // import 0x2c7258
//         RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME);            // import 0x2c7130
//     }
//     Default();                                                           // 0x28ac80
// (nFlags is not read.)
// Symbol: ?OnMouseMove@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    S_CMFCEditBrowseCtrl* self = L(pThis);
    const POINT pt = PtFrom(point);
    if (self->m_bIsButtonCaptured != 0) {
        const BOOL bIsPressed = ::PtInRect(&self->m_rectBtn, pt);
        if (bIsPressed != self->m_bIsButtonPressed) {
            self->m_bIsButtonPressed = bIsPressed;
            ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwFrameInvalidate);
        }
        return;
    }
    if (self->m_bIsButtonHighlighted != 0 && !::PtInRect(&self->m_rectBtn, pt)) {
        self->m_bIsButtonHighlighted = FALSE;
        ::ReleaseCapture();
        ::RedrawWindow(HWndOf(pThis), nullptr, nullptr, kRdwFrameInvalidate);
    }
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Retail: WM_RBUTTONDOWN and WM_RBUTTONUP share one body (entry RVA 0x60a90,
// mfc140u, folded by the linker), fully transcribed:
//     if (m_Mode != BrowseMode_None && ::PtInRect(&m_rectBtn, point)) return;   // import 0x2c72f8
//     Default();                                                                 // 0x28ac80
// (nFlags is not read.)
// Symbol: ?OnRButtonDown@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonDown_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    const POINT pt = PtFrom(point);
    if (L(pThis)->m_Mode != BrowseMode_None && ::PtInRect(&L(pThis)->m_rectBtn, pt)) return;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Same retail body as OnRButtonDown (entry RVA 0x60a90, mfc140u), see above.
// Symbol: ?OnRButtonUp@CMFCEditBrowseCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnRButtonUp_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags;
    if (pThis == nullptr) return;
    const POINT pt = PtFrom(point);
    if (L(pThis)->m_Mode != BrowseMode_None && ::PtInRect(&L(pThis)->m_rectBtn, pt)) return;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}
