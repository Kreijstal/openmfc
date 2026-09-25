// CMFCDropDownToolbarButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from
// the disassembly of that function in mfc140u.dll (14.51.36231), the method
// described in the header of core/ole/COleControl.cpp.  Two bodies have no
// entry in the export map and were located another way, as their comments
// say: SetDefaultCommand (0x5e120, the target of the calls from
// OnCalculateSize 0x5ed50 and Serialize) and Serialize (0x5e230, slot 2 of the
// class vftable 0x1802e7688, mfc140u).
//
// INSTANCE LAYOUT.  This class is NOT declared in include/openmfc, so this
// file is the only place its layout lives: every export takes a `void*` this
// and a real MSVC client (compiled against afxdropdowntoolbar.h) allocates
// the object with the retail layout.  The SDK header declares, after the
// CMFCToolBarButton base:
//
//   CMFCToolBarButton base ............ +0x00 .. +0x88   (afxmfc.h, sizeof 136)
//   +0x88  CMFCDropDownToolBar* m_pToolBar
//   +0x90  CString              m_strName
//   +0x98  CMFCDropDownFrame*   m_pPopupMenu
//   +0xa0  UINT                 m_uiTimer
//   +0xa4  int                  m_iSelectedImage
//   +0xa8  BOOL                 m_bInternalDraw
//   +0xac  BOOL                 m_bLocalUserButton
//   sizeof == 0xb0 (176: the size ?CreateObject@ (0x5dec0) allocates, the
//   `mov $0xb0,%edx` in the retail deleting destructor, and the value the
//   RTTI descriptor in featurepack/toolbar/RuntimeClasses.cpp carries)
//
// Which offset is which was fixed from the bodies: the default constructor
// (0x5df30) stores the CString nil pointer at +0x90 and zeroes +0x88/+0x98/
// +0xa0/+0xa8; the (LPCTSTR, CMFCDropDownToolBar*) constructor (0x5dff0)
// stores the toolbar argument at +0x88 and SetString's the name into +0x90;
// OnClick (0x5e980) stores SetTimer's result at +0xa0; OnClickUp (0x5eae0)
// clears +0x98 after destroying the frame there; SetDefaultCommand (0x5e120)
// writes the image index at +0xa4 and the source button's m_bUserButton at
// +0xac; OnDraw (0x5e400) raises +0xa8 around its internal base draw.
//
// VFTABLE.  Retail installs &CMFCDropDownToolbarButton::`vftable'
// (0x1802e7688, mfc140u): the 53 CMFCToolBarButton slots, this class's
// overrides at 2 Serialize, 7 CopyFrom, 8 OnDraw, 9 OnCalculateSize,
// 10 OnClick, 11 OnClickUp, 12 OnChangeParentWnd, 13 ExportToMenuButton,
// 24 OnCancelMode, 27 OnContextHelp (inline), 28 OnCustomizeMenu,
// 29 OnDrawOnCustomizeList, 52 IsExtraSize (inline).  OpenMFC has no C++
// class for this type, so the constructors leave the vptr the base
// constructor thunk installs (the mingw-built CMFCToolBarButton one), exactly
// as featurepack/toolbar/CMFCToolBarDateTimeCtrl.cpp does.  Consequences:
//   * a virtual call an MSVC client makes on an object built here goes to
//     whatever entry the mingw CMFCToolBarButton table holds at that MSVC
//     slot index (an Itanium-layout table, so not necessarily the same
//     method), never to this class's override (the exported entry points
//     below are reached only when called by name);
//   * the one virtual call retail makes on `this' -- the timer callback's
//     g_pButtonDown->OnClick(pWnd, FALSE), vslot 0x50 (10) -- is made
//     through this file's own OnClick thunk when the object carries that
//     recorded base vptr, and through the object's MSVC vftable slot 10
//     otherwise (a client-derived object whose own constructor installed its
//     MSVC table after ours ran);
//   * RTTI: slot 0 of the vptr these objects carry is
//     CMFCToolBarButton::GetRuntimeClass, so IsKindOf(RUNTIME_CLASS(
//     CMFCDropDownToolbarButton)) reads FALSE for an object built here.
// The destructor reproduces retail's "store my own vftable first" step with
// the recorded base vptr, as CMFCToolBarDateTimeCtrl.cpp does.
//
// THE TOOLBAR.  OpenMFC keeps a CMFCToolBar's buttons in the mfccore side
// table (featurepack/toolbar/CMFCToolBar.cpp, g_toolBarStates), not in the
// CObList m_Buttons at +0x1188 that retail reads (m_Buttons.m_nCount at
// +0x11a0 -- the inline GetCount(), ?GetCount@CMFCToolBar@@ is
// `mov 0x11a0(%rcx),%eax; ret` -- and m_Buttons.m_pNodeHead at +0x1190), so
// every walk of m_pToolBar's buttons here goes through the exported
// ?GetCount@ / ?GetButton@CMFCToolBar@@ thunks instead; the order is the same
// (GetButton(i) is FindIndex(i) on that list in retail, 0x14fe00).

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <new>

// ---------------------------------------------------------------------------
// Cross-file thunks.  Each was grepped to its definition in the file named;
// the parameter lists are derived from the mangled names.
// ---------------------------------------------------------------------------
//   featurepack/toolbar/Thunks.cpp : the base constructor and destructor
//   (placement-new / C++ destructor of the mingw CMFCToolBarButton)
extern "C" void* MS_ABI impl___0CMFCToolBarButton__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCToolBarButton__UEAA_XZ(void* pThis);
//   featurepack/toolbar/CMFCToolBarButton.cpp : the base-class overrides.
//   CopyFrom, Serialize and SetImage are real bodies (tbb_CopyFrom /
//   tbb_Serialize / tbb_SetImage); OnCalculateSize, OnChangeParentWnd and
//   OnDrawOnCustomizeList are still empty stubs there whose auto-generated
//   parameter lists do not match their mangled names (harmless across
//   translation units for extern "C" symbols -- the same declarations
//   CMFCToolBarDateTimeCtrl.cpp and CMFCToolBarComboBoxButton.cpp use).  The
//   lists below are the retail ones: SIZE is returned through a hidden
//   pointer in RDX (the retail body writes the result to [rdx] and returns
//   rdx in rax).
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(void* pThis, const CMFCToolBarButton* pSrc);
extern "C" void MS_ABI impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar);
extern "C" void MS_ABI impl__SetImage_CMFCToolBarButton__UEAAXH_Z(void* pThis, int iImage);
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarButton* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz);
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(CMFCToolBarButton* pThis, CWnd* pWndParent);
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    CMFCToolBarButton* pThis, CDC* pDC, const RECT* pRect, int bSelected);
//   featurepack/toolbar/Thunks.cpp : CMFCToolBar::GetCount / GetButton (side table)
extern "C" int MS_ABI impl__GetCount_CMFCToolBar__QEBAHXZ(const CMFCToolBar* pThis);
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(const CMFCToolBar* pThis, int nIndex);
//   featurepack/toolbar/CMFCToolBar.cpp : SetHot, GetAllToolbars
extern "C" int MS_ABI impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(CMFCToolBar* pThis, CMFCToolBarButton* pButton);
extern "C" const CObList* MS_ABI impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ();
//   featurepack/toolbar/CMFCToolBarMenuButton.cpp : CreateFromMenu (a C++ virtual call
//   on the mingw CMFCToolBarMenuButton)
extern "C" void MS_ABI impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(CMFCToolBarMenuButton* pThis, HMENU hMenu);
//   featurepack/toolbar/StaticData.cpp : the class statics
extern "C" std::uint32_t impl__m_uiShowBarDelay_CMFCDropDownToolbarButton__2IA;   // 0x3b1ae8 (mfc140u), retail .data 500
extern "C" std::int32_t impl__m_bLargeIcons_CMFCToolBar__1HA;                     // 0x3be368 (mfc140u)
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;                  // 0x3be35c (mfc140u)
//   core/runtime/AFX_GLOBAL_DATA.cpp : Initialize;  featurepack/CMFC_misc_stubs.cpp : the
//   ?afxGlobalData@@ storage (0x1803c1620, mfc140u), a zeroed 720-byte block
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];
//   featurepack/toolbar/RuntimeClasses.cpp / featurepack/menu/RuntimeClasses.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCDropDownToolBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ();
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ();
//   core/runtime/CObject.cpp : IsKindOf
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
//   core/collections/CObList.cpp : FindIndex (the GetAllToolbars walk pattern of
//   featurepack/menu/CMFCMenuBar.cpp / featurepack/visualmanager/CMFCVisualManager.cpp)
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);
//   core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);
//   core/runtime/Thunks.cpp : CArchive::Read / Write
extern "C" unsigned int MS_ABI impl__Read_CArchive__QEAAIPEAXI_Z(CArchive* pThis, void* pBuf, unsigned int nMax);
extern "C" void MS_ABI impl__Write_CArchive__QEAAXPEBXI_Z(CArchive* pThis, const void* pBuf, unsigned int nMax);
//   featurepack/CMFC_misc_stubs.cpp : AfxFindStringResourceHandle
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
//   core/collections/CStringT.cpp : CStringT::LoadString(HINSTANCE, UINT)
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID);
//   detail/MfcExceptionsSupport.cpp : AfxThrowInvalidArgException
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
//   detail/MemcoreSupport.cpp : ??2@YAPEAX_K@Z
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

// Exports defined later in this file that earlier bodies call (all in this TU).
extern "C" void* MS_ABI impl___0CMFCDropDownToolbarButton__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__SetDefaultCommand_CMFCDropDownToolbarButton__QEAAXI_Z(void* pThis, unsigned int uiCmd);
extern "C" int MS_ABI impl__DropDownToolbar_CMFCDropDownToolbarButton__QEAAHPEAVCWnd___Z(void* pThis, CWnd* pWnd);
extern "C" int MS_ABI impl__OnClick_CMFCDropDownToolbarButton__UEAAHPEAVCWnd__H_Z(void* pThis, CWnd* pWnd, int bDelay);

namespace {

// Retail member offsets (see the file header).
constexpr int kOffToolBar          = 0x88;
constexpr int kOffStrName          = 0x90;
constexpr int kOffPopupMenu        = 0x98;
constexpr int kOffTimer            = 0xa0;
constexpr int kOffSelectedImage    = 0xa4;
constexpr int kOffInternalDraw     = 0xa8;
constexpr int kOffLocalUserButton  = 0xac;
constexpr std::size_t kObjectSize  = 0xb0;
static_assert(kOffLocalUserButton + sizeof(BOOL) == kObjectSize, "m_bLocalUserButton is the last member");

// CMFCDropDownFrame members retail touches through m_pPopupMenu (retail
// layout, afxdropdowntoolbar.h; the frame's constructor 0x5d2a0 (mfc140u)
// stores 1 at +0x1f0 and NULL at +0x1f8, DropDownToolbar stores `this' at +0x1f8).
constexpr int kOffFrameAutoDestroyParent = 0x1f0;   // BOOL m_bAutoDestroyParent

static_assert(sizeof(CMFCToolBarButton) == 0x88, "CMFCDropDownToolbarButton's own members start at +0x88");
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "m_bUserButton @0x08");
static_assert(offsetof(CMFCToolBarButton, m_bText) == 0x0c, "m_bText @0x0c");
static_assert(offsetof(CMFCToolBarButton, m_bImage) == 0x10, "m_bImage @0x10");
static_assert(offsetof(CMFCToolBarButton, m_bDragFromCollection) == 0x20, "m_bDragFromCollection @0x20");
static_assert(offsetof(CMFCToolBarButton, m_nID) == 0x24, "m_nID @0x24");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle @0x28");
static_assert(offsetof(CMFCToolBarButton, m_strText) == 0x38, "m_strText @0x38");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == 0x48, "m_iImage @0x48");
static_assert(offsetof(CMFCToolBarButton, m_iUserImage) == 0x4c, "m_iUserImage @0x4c");
static_assert(offsetof(CMFCToolBarButton, m_bLocked) == 0x50, "m_bLocked @0x50");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect @0x68");
static_assert(offsetof(CMFCToolBarButton, m_pWndParent) == 0x80, "m_pWndParent @0x80");
static_assert(offsetof(CMFCToolBar, m_uiOriginalResID) == 0x1308, "CMFCToolBar::m_uiOriginalResID @0x1308 (Serialize compares it)");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");
static_assert(offsetof(CMenu, m_hMenu) == 0x08, "CMenu::m_hMenu @0x08 (OnCustomizeMenu reads it there)");
static_assert(sizeof(CString) == sizeof(void*), "CString is one pointer (m_strName occupies +0x90..+0x98)");
static_assert(sizeof(CRect) == sizeof(RECT), "CRect is RECT-shaped");

template <class T> inline T& At(void* p, int off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, int off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}

inline CMFCToolBarButton*       Base(void* p)       { return static_cast<CMFCToolBarButton*>(p); }
inline const CMFCToolBarButton* Base(const void* p) { return static_cast<const CMFCToolBarButton*>(p); }
// m_pToolBar is a CMFCDropDownToolBar*; OpenMFC declares no such class (it is
// a CMFCToolBar with no members of its own, afxdropdowntoolbar.h), so it is
// held as the CMFCToolBar base here.
inline CMFCToolBar*& ToolBar(void* p)              { return At<CMFCToolBar*>(p, kOffToolBar); }
inline CMFCToolBar*  ToolBar(const void* p)        { return At<CMFCToolBar*>(p, kOffToolBar); }
inline CString&      StrName(void* p)              { return At<CString>(p, kOffStrName); }
inline const CString& StrName(const void* p)       { return At<CString>(p, kOffStrName); }
inline CWnd*&        PopupMenu(void* p)            { return At<CWnd*>(p, kOffPopupMenu); }
inline UINT&         Timer(void* p)                { return At<UINT>(p, kOffTimer); }
inline int&          SelectedImage(void* p)        { return At<int>(p, kOffSelectedImage); }
inline int           SelectedImage(const void* p)  { return At<int>(p, kOffSelectedImage); }
inline BOOL&         InternalDraw(void* p)         { return At<BOOL>(p, kOffInternalDraw); }
inline BOOL&         LocalUserButton(void* p)      { return At<BOOL>(p, kOffLocalUserButton); }

// OpenMFC's CRect does not derive from RECT; it is RECT-shaped (asserted above).
inline const RECT* RectOf(const CMFCToolBarButton* p) { return reinterpret_cast<const RECT*>(&p->m_rect); }

// `DYNAMIC_DOWNCAST(T, p) != NULL` as retail inlines it: p != NULL && p->IsKindOf(RUNTIME_CLASS(T))
// (?IsKindOf@CObject@@, 0x234cf0 mfc140u).
inline bool IsKindOfClass(const CObject* p, CRuntimeClass* pClass) {
    return p != nullptr && pClass != nullptr &&
           impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, pClass) != 0;
}

// Menu-command / style numbers the retail bodies use as immediates.
constexpr UINT     kTimerId           = 0xEC11;   // the nIDEvent OnClick passes to ::SetTimer
constexpr unsigned kTbbsSeparator     = 0x1;      // TBBS_SEPARATOR (`testb $0x1,0x28(button)`)
constexpr int      kArrowSize         = 7;        // the 7 / 7.0 OnCalculateSize scales
constexpr UINT     kMfSeparator       = 0x800;    // MF_SEPARATOR
constexpr UINT     kMfString          = 0x0;      // MF_STRING
constexpr UINT     kMfByCommandGrayed = 0x1;      // MF_BYCOMMAND | MF_GRAYED
static_assert(kMfSeparator == MF_SEPARATOR && kMfString == MF_STRING, "AppendMenu flags");
static_assert(kMfByCommandGrayed == (MF_BYCOMMAND | MF_GRAYED), "EnableMenuItem flags");

// ---------------------------------------------------------------------------
// afxGlobalData.  The slots OnCalculateSize reads are those of the inline
// AFX_GLOBAL_DATA::GetRibbonImageScale(): `m_bIsRibbonImageScale ?
// m_dblRibbonImageScale : 1.0`, a BOOL at +0x2c0 and a double at +0x2b8
// (retail reads 0x1803c18e0 / 0x1803c18d8 against the block at 0x1803c1620,
// mfc140u).  DIVERGENCE (global, not specific to this file): retail's
// ??0AFX_GLOBAL_DATA@@QEAA@XZ (RVA 0x6a590, mfc140u) stores 1 at +0x2c0, so by
// default retail answers m_dblRibbonImageScale, which UpdateFonts derives
// from the screen DPI.  OpenMFC runs no constructor on the exported blob and
// nothing in the tree writes +0x2c0, so here the flag stays 0 and the scale
// reads 1.0 on every display, even though Initialize
// (core/runtime/AFX_GLOBAL_DATA.cpp) does fill +0x2b8.
// ---------------------------------------------------------------------------
constexpr int kGlobalDataInitGate         = 0x000;
constexpr int kGlobalDataRibbonImageScale = 0x2b8;   // double
constexpr int kGlobalDataIsRibbonScale    = 0x2c0;   // BOOL
static_assert(kGlobalDataIsRibbonScale + sizeof(BOOL) <= sizeof(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A), "inside the 720-byte block");

inline unsigned char* GlobalDataBytes() { return impl__afxGlobalData__3UAFX_GLOBAL_DATA__A; }
inline int GlobalDataInt(int off) { int v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
inline double GlobalDataDouble(int off) { double v; std::memcpy(&v, GlobalDataBytes() + off, sizeof v); return v; }
// The retail one-time gate every reader does first:
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); <+0> = 1; }    // 0x6a790 (mfc140u)
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(GlobalDataBytes());
        const int one = 1;
        std::memcpy(GlobalDataBytes() + kGlobalDataInitGate, &one, sizeof one);
    }
}
inline double RibbonImageScale() {
    EnsureGlobalDataInitialized();
    return GlobalDataInt(kGlobalDataIsRibbonScale) != 0 ? GlobalDataDouble(kGlobalDataRibbonImageScale) : 1.0;
}

// CArchive scalar / string I/O through the exported Read / Write, in the
// byte layout OpenMFC's own C++ CArchive::operator<< / >> use
// (core/runtime/CArchive.cpp: a 4-byte int; a CString as a 4-byte length
// followed by that many wchar_t) -- the layout the base class's
// tbb_Serialize writes its m_strText in.  Retail inlines the int case as
// direct buffer accesses (FillBuffer 0x1d1cc0 / Flush 0x1d1be0 on a short
// buffer) and calls the outlined CString archive operators (0x1b5e4 >> and
// 0x1b818 <<, mfc140u), whose on-disk string format (MFC's
// AfxWriteStringLength prefix) OpenMFC's C++ CArchive operators << / >> for
// CString do not produce.  The exported ?AfxWriteStringLength@@ /
// ?AfxReadStringLength@@ are transcribed in core/runtime/CArchive.cpp, but
// the base-class part of this record (tbb_Serialize) is already in the
// OpenMFC layout, so the record as a whole is not retail-compatible either
// way; m_strName follows the base class's layout for consistency.
inline void ArWriteInt(CArchive* ar, int v) { impl__Write_CArchive__QEAAXPEBXI_Z(ar, &v, sizeof v); }
inline int ArReadInt(CArchive* ar) { int v = 0; impl__Read_CArchive__QEAAIPEAXI_Z(ar, &v, sizeof v); return v; }
inline void ArWriteString(CArchive* ar, const CString& s) {
    const UINT nLen = static_cast<UINT>(s.GetLength());
    impl__Write_CArchive__QEAAXPEBXI_Z(ar, &nLen, sizeof nLen);
    if (nLen > 0) impl__Write_CArchive__QEAAXPEBXI_Z(ar, s.GetString(), nLen * static_cast<UINT>(sizeof(wchar_t)));
}
inline void ArReadString(CArchive* ar, CString& s) {
    UINT nLen = 0;
    impl__Read_CArchive__QEAAIPEAXI_Z(ar, &nLen, sizeof nLen);
    if (nLen > 0 && nLen < 0x10000000) {
        wchar_t* pBuf = s.GetBuffer(static_cast<int>(nLen) + 1);
        impl__Read_CArchive__QEAAIPEAXI_Z(ar, pBuf, nLen * static_cast<UINT>(sizeof(wchar_t)));
        pBuf[nLen] = L'\0';
        s.ReleaseBuffer(static_cast<int>(nLen));
    } else {
        s.Empty();
    }
}

// Mirror of CList<CObject*, CObject*>::CNode (include/openmfc/afx.h), the node
// a CObList::POSITION points at -- used to walk GetAllToolbars() exactly as
// featurepack/menu/CMFCMenuBar.cpp does.
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");
inline const ObNode* ObNodeFromPos(CObList::POSITION pos) {
    const ObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof(p));
    return p;
}

// The vptr the base constructor thunk installs (the mingw CMFCToolBarButton
// one), recorded by the first constructor to run; the destructor stores it
// back at +0x00 first, which is what retail's `vfptr = &vftable' store does.
void* g_ownVptr = nullptr;
inline bool CarriesOwnVptr(const void* p) {
    return g_ownVptr == nullptr || *static_cast<void* const*>(p) == g_ownVptr;
}

// Retail's file-static `g_pButtonDown' (0x1803c3548, mfc140u; not exported):
// the button whose delayed click the pending timer will complete.
void* g_pButtonDown = nullptr;

// Retail (RVA 0x5e950, mfc140u; not exported -- the TIMERPROC OnClick passes
// to ::SetTimer), fully transcribed:
//     CWnd* pWnd = CWnd::FromHandle(hWnd);                      // 0x28ad70
//     if (g_pButtonDown != NULL) g_pButtonDown->OnClick(pWnd, FALSE);   // vslot 0x50 (10)
// The virtual call is made as described in the file header (VFTABLE).
void CALLBACK ButtonTimerProc(HWND hWnd, UINT, UINT_PTR, DWORD) {
    CWnd* pWnd = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hWnd);
    void* pButton = g_pButtonDown;
    if (pButton == nullptr) return;
    if (CarriesOwnVptr(pButton)) {
        impl__OnClick_CMFCDropDownToolbarButton__UEAAHPEAVCWnd__H_Z(pButton, pWnd, FALSE);
        return;
    }
    using OnClickFn = int (MS_ABI*)(void*, CWnd*, int);
    reinterpret_cast<OnClickFn>((*static_cast<void***>(pButton))[10])(pButton, pWnd, FALSE);
}

// `m_pPopupMenu->DestroyWindow()` -- retail dispatches vslot 0xd0 (26) on the
// frame.  CMFCDropDownFrame, CMiniFrameWnd and CFrameWnd do not override
// DestroyWindow (only CWnd declares it among them in the SDK afxwin.h /
// afxdropdowntoolbar.h), so the exported ?DestroyWindow@CWnd@@ body is what
// that slot holds for a stock frame; it is called by name here.  An override
// in a client-derived frame is not reached.
inline void DestroyPopupFrame(CWnd* pFrame) {
    impl__DestroyWindow_CWnd__UEAAHXZ(pFrame);
}

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Retail ??0CMFCDropDownToolbarButton@@QEAA@XZ (RVA 0x5df30, mfc140u), fully transcribed:
//     CMFCToolBarButton::CMFCToolBarButton();                  // 0x15bec0
//     vfptr = &CMFCDropDownToolbarButton::`vftable';           // 0x1802e7688
//     m_strName = CString();                                   // the nil string
//     m_pToolBar = NULL;  m_pPopupMenu = NULL;  m_pWndParent = NULL;
//     m_uiTimer = 0;  m_iSelectedImage = 0;                    // one 8-byte store at +0xa0
//     m_bLocked = TRUE;                                        // +0x50
//     m_bInternalDraw = FALSE;  m_bLocalUserButton = FALSE;    // one 8-byte store at +0xa8
// The vfptr is left as the base constructor thunk installs it (file header).
// Symbol: ??0CMFCDropDownToolbarButton@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCDropDownToolbarButton__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    impl___0CMFCToolBarButton__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    new (&StrName(pThis)) CString();
    ToolBar(pThis) = nullptr;
    PopupMenu(pThis) = nullptr;
    Base(pThis)->m_pWndParent = nullptr;
    Timer(pThis) = 0;
    SelectedImage(pThis) = 0;
    Base(pThis)->m_bLocked = TRUE;
    InternalDraw(pThis) = FALSE;
    LocalUserButton(pThis) = FALSE;
    return pThis;
}

// Retail ??0CMFCDropDownToolbarButton@@QEAA@PEB_WPEAVCMFCDropDownToolBar@@@Z
// (RVA 0x5dff0, mfc140u), fully transcribed:
//     CMFCToolBarButton::CMFCToolBarButton();                  // 0x15bec0
//     vfptr = &CMFCDropDownToolbarButton::`vftable';           // 0x1802e7688
//     m_strName = CString();
//     if (lpszName == NULL) AfxThrowInvalidArgException();     // 0x227720
//     m_strName.SetString(lpszName, wcslen(lpszName));
//     m_uiTimer = 0;  m_pPopupMenu = NULL;  m_pWndParent = NULL;
//     m_pToolBar = pToolBar;
//     CMFCToolBarButton* pButton = pToolBar->GetButton(0);     // 0x14fe00
//     if (pButton != NULL) CMFCToolBarButton::CopyFrom(*pButton);   // 0x15c1a0, a direct (non-virtual) call
//     m_iSelectedImage = 0;  m_bLocalUserButton = FALSE;
// m_bInternalDraw (+0xa8) is NOT written by this constructor in retail; it
// is not written here either.  DEVIATIONS: retail calls GetButton(0) on a NULL
// pToolBar (and faults); the call is skipped for NULL here.  On the NULL-name
// throw, a C++ constructor unwinds what it has built (at least the
// CMFCToolBarButton base; retail's unwind funclets were not read); this
// thunk does no cleanup, so the base is left constructed.
// Symbol: ??0CMFCDropDownToolbarButton@@QEAA@PEB_WPEAVCMFCDropDownToolBar@@@Z
extern "C" void* MS_ABI impl___0CMFCDropDownToolbarButton__QEAA_PEB_WPEAVCMFCDropDownToolBar___Z(
    void* pThis, const wchar_t* lpszName, void* pToolBar) {
    if (!pThis) return pThis;
    impl___0CMFCToolBarButton__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    new (&StrName(pThis)) CString();
    if (lpszName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return pThis;
    }
    StrName(pThis) = lpszName;
    Timer(pThis) = 0;
    PopupMenu(pThis) = nullptr;
    Base(pThis)->m_pWndParent = nullptr;
    ToolBar(pThis) = static_cast<CMFCToolBar*>(pToolBar);
    if (CMFCToolBar* pBar = ToolBar(pThis)) {
        if (CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, 0)) {
            impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pThis, pButton);
        }
    }
    SelectedImage(pThis) = 0;
    LocalUserButton(pThis) = FALSE;
    return pThis;
}

// Retail ??1CMFCDropDownToolbarButton@@UEAA@XZ (RVA 0x5e0d0, mfc140u), fully transcribed:
//     vfptr = &CMFCDropDownToolbarButton::`vftable';           // 0x1802e7688
//     m_strName.~CString();                                    // inlined release (lock xadd on +0x10 of the header)
//     CMFCToolBarButton::~CMFCToolBarButton();                 // 0x15c130 (tail jump)
// Nothing else is torn down: the timer, g_pButtonDown and m_pPopupMenu are
// left as they are, exactly as retail leaves them.  The vfptr store is
// reproduced with the recorded base vptr (file header).
// Symbol: ??1CMFCDropDownToolbarButton@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCDropDownToolbarButton__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    StrName(pThis).~CString();
    impl___1CMFCToolBarButton__UEAA_XZ(pThis);
}

// Retail (RVA 0x5dec0, mfc140u), fully transcribed:
//     void* p = operator new(0xb0);
//     return p != NULL ? new (p) CMFCDropDownToolbarButton() : NULL;   // tail jump to 0x5df30
// Symbol: ?CreateObject@CMFCDropDownToolbarButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCDropDownToolbarButton__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kObjectSize);
    if (p == nullptr) return nullptr;
    return static_cast<CObject*>(impl___0CMFCDropDownToolbarButton__QEAA_XZ(p));
}

// Retail (RVA 0x5e1d0, mfc140u), fully transcribed:
//     CMFCToolBarButton::CopyFrom(src);                         // 0x15c1a0
//     m_pToolBar       = src.m_pToolBar;                        // +0x88
//     m_strName        = src.m_strName;                         // +0x90, CString operator=
//     m_iSelectedImage = src.m_iSelectedImage;                  // +0xa4
//     m_bDragFromCollection = FALSE;                            // +0x20
// src is reinterpreted as a CMFCDropDownToolbarButton without a type check,
// as in retail.
// Symbol: ?CopyFrom@CMFCDropDownToolbarButton@@UEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCDropDownToolbarButton__UEAAXAEBVCMFCToolBarButton___Z(void* pThis, const CMFCToolBarButton* pSrc) {
    if (!pThis || !pSrc) return;
    impl__CopyFrom_CMFCToolBarButton__UEAAXAEBV1__Z(pThis, pSrc);
    ToolBar(pThis) = ToolBar(static_cast<const void*>(pSrc));
    StrName(pThis) = StrName(static_cast<const void*>(pSrc));
    SelectedImage(pThis) = SelectedImage(static_cast<const void*>(pSrc));
    Base(pThis)->m_bDragFromCollection = FALSE;
}

// Retail (RVA 0x5e120, mfc140u -- not in the export map; the target of the
// direct calls from OnCalculateSize 0x5ed50 and Serialize 0x5e230; its
// instructions match those of the mfc140.dll export at 0x5df60, which was
// read in full), fully transcribed:
//     m_nID = uiCmd;
//     m_iSelectedImage = -1;
//     int iImage = 0;
//     for (int i = 0; i < m_pToolBar->GetCount(); i++) {        // inline: m_Buttons.m_nCount (+0x11a0)
//         CMFCToolBarButton* pButton = m_pToolBar->GetButton(i);   // 0x14fe00
//         if (pButton->m_nStyle & TBBS_SEPARATOR) continue;     // separators are not counted
//         if (pButton->m_nID == uiCmd) {
//             m_bLocalUserButton = pButton->m_bUserButton;
//             m_iSelectedImage = m_bLocalUserButton ? pButton->GetImage() : iImage;   // GetImage(): +0x08 ? +0x4c : +0x48
//             break;
//         }
//         iImage++;
//     }
//     if (m_iSelectedImage == -1) m_iSelectedImage = 0;
// DEVIATIONS: retail dereferences a NULL m_pToolBar (and faults); a NULL
// toolbar is treated as empty here.  Retail's GetButton (0x14fe00, mfc140u)
// never returns NULL for an in-range index -- it throws through
// AfxThrowInvalidArgException (the call at 0x14fe3d) on a NULL list entry --
// but OpenMFC's side-table GetButton can return NULL, so a NULL button is
// skipped here.  The count and buttons come from the GetCount / GetButton
// thunks (file header, THE TOOLBAR).
// Symbol: ?SetDefaultCommand@CMFCDropDownToolbarButton@@QEAAXI@Z
extern "C" void MS_ABI impl__SetDefaultCommand_CMFCDropDownToolbarButton__QEAAXI_Z(void* pThis, unsigned int uiCmd) {
    if (!pThis) return;
    Base(pThis)->m_nID = uiCmd;
    SelectedImage(pThis) = -1;
    const CMFCToolBar* pBar = ToolBar(pThis);
    const int nCount = pBar ? impl__GetCount_CMFCToolBar__QEBAHXZ(pBar) : 0;
    int iImage = 0;
    for (int i = 0; i < nCount; i++) {
        const CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (pButton == nullptr) continue;
        if (pButton->m_nStyle & kTbbsSeparator) continue;
        if (pButton->m_nID == uiCmd) {
            LocalUserButton(pThis) = pButton->m_bUserButton;
            SelectedImage(pThis) = LocalUserButton(pThis) ? pButton->GetImage() : iImage;
            break;
        }
        iImage++;
    }
    if (SelectedImage(pThis) == -1) SelectedImage(pThis) = 0;
}

// Retail (RVA 0x5e230, mfc140u -- vftable slot 2 of 0x1802e7688; not in the
// export map; read in full from the mfc140.dll export at 0x5e070, whose
// call sequence it matches), fully transcribed.  The int reads / writes are CArchive's inline buffer accesses,
// the string ones the outlined CString archive operators:
//     CMFCToolBarButton::Serialize(ar);                              // 0x15c240
//     if (ar.IsLoading()) {                                          // ar.m_nMode & 1 (+0x20)
//         m_pToolBar = NULL;
//         int iResID;  ar >> iResID;
//         ar >> m_strName;
//         ar >> m_iSelectedImage;
//         for (node = gAllToolbars.head /* 0x1803b2098 */; node != NULL; node = node->pNext) {
//             CMFCToolBar* pToolBar = node->data;
//             if (pToolBar != NULL && pToolBar->IsKindOf(RUNTIME_CLASS(CMFCDropDownToolBar))  // RTC 0x1803b1238
//                 && CWnd::FromHandlePermanent(pToolBar->m_hWnd) != NULL                     // 0x28adc0
//                 && pToolBar->m_uiOriginalResID == iResID) {                               // +0x1308
//                 m_pToolBar = pToolBar;  break;
//             }
//         }
//         SetDefaultCommand(m_nID);                                  // 0x5e120
//     } else {
//         ar << (m_pToolBar != NULL ? m_pToolBar->m_uiOriginalResID : 0);
//         ar << m_strName;
//         ar << m_iSelectedImage;
//     }
// DEVIATIONS: the scalars and the string use OpenMFC's own archive byte
// layout (ArWriteInt / ArWriteString above); the toolbar walk runs over the
// list the exported ?GetAllToolbars@CMFCToolBar@@ returns (CMFCToolBar.cpp
// rebuilds it from the mfccore side table; retail's list is not exported),
// as featurepack/menu/CMFCMenuBar.cpp does.
// Symbol: ?Serialize@CMFCDropDownToolbarButton@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CMFCDropDownToolbarButton__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    if (!pThis || !ar) return;
    impl__Serialize_CMFCToolBarButton__UEAAXAEAVCArchive___Z(pThis, ar);
    if (ar->IsLoading()) {
        ToolBar(pThis) = nullptr;
        const UINT uiResID = static_cast<UINT>(ArReadInt(ar));
        ArReadString(ar, StrName(pThis));
        SelectedImage(pThis) = ArReadInt(ar);
        CRuntimeClass* pDropDownClass = impl__GetThisClass_CMFCDropDownToolBar__SAPEAUCRuntimeClass__XZ();
        if (const CObList* pList = impl__GetAllToolbars_CMFCToolBar__SAAEBVCObList__XZ()) {
            for (const ObNode* p = ObNodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0)); p != nullptr; p = p->pNext) {
                CMFCToolBar* pToolBar = static_cast<CMFCToolBar*>(p->data);
                if (pToolBar == nullptr) continue;
                if (!IsKindOfClass(pToolBar, pDropDownClass)) continue;
                if (impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pToolBar->m_hWnd) == nullptr) continue;
                if (pToolBar->m_uiOriginalResID == uiResID) {
                    ToolBar(pThis) = pToolBar;
                    break;
                }
            }
        }
        impl__SetDefaultCommand_CMFCDropDownToolbarButton__QEAAXI_Z(pThis, Base(pThis)->m_nID);
    } else {
        const CMFCToolBar* pBar = ToolBar(pThis);
        ArWriteInt(ar, pBar != nullptr ? static_cast<int>(pBar->m_uiOriginalResID) : 0);
        ArWriteString(ar, StrName(pThis));
        ArWriteInt(ar, SelectedImage(pThis));
    }
}

// ===========================================================================
// Drop-down
// ===========================================================================

// Retail (RVA 0x5ec30, mfc140u) creates the drop-down frame:
//     if (m_pToolBar == NULL || m_pPopupMenu != NULL) return FALSE;
//     if (pWnd == NULL && (pWnd = m_pWndParent) == NULL) return FALSE;
//     CPoint pt = (m_pWndParent is a CMFCToolBar (RTC 0x1803b15f8) && !its vslot 0x2d8 (91) ())
//               ? CPoint(m_rect.right + 1, m_rect.top) : CPoint(m_rect.left - 1, m_rect.bottom);
//     ::ClientToScreen(pWnd->m_hWnd, &pt);
//     m_pPopupMenu = new CMFCDropDownFrame;                       // 0x1570 bytes, ctor 0x5d2a0
//     m_pPopupMenu->m_pParentBtn = this;                          // +0x1f8
//     return m_pPopupMenu->Create(pWnd, pt.x, pt.y, m_pToolBar);  // vslot 0x3a0 (116)
// STUB: OpenMFC's CMFCDropDownFrame is entirely unimplemented
// (featurepack/controls/CMFCDropDownFrame.cpp: the constructor returns pThis
// without initialising anything and Create is a stub that returns FALSE),
// so the frame cannot be built.
// Transcribing the body would leak a 0x1570-byte uninitialised object into
// m_pPopupMenu on every call and wedge IsDropDown() at TRUE.  Returns FALSE,
// which is what retail returns when the frame's Create fails.
// Symbol: ?DropDownToolbar@CMFCDropDownToolbarButton@@QEAAHPEAVCWnd@@@Z
extern "C" int MS_ABI impl__DropDownToolbar_CMFCDropDownToolbarButton__QEAAHPEAVCWnd___Z(void* pThis, CWnd* pWnd) {
    (void)pThis;
    (void)pWnd;
    return FALSE;
}

// Retail (RVA 0x5e980, mfc140u), fully transcribed:
//     if (m_uiTimer == 0) {
//         if (m_pWndParent != NULL)
//             m_uiTimer = ::SetTimer(m_pWndParent->m_hWnd, 0xEC11, m_uiShowBarDelay, ButtonTimerProc);   // 0x5e950
//         g_pButtonDown = this;
//         return FALSE;
//     }
//     if (m_pWndParent != NULL) ::KillTimer(m_pWndParent->m_hWnd, m_uiTimer);
//     m_uiTimer = 0;
//     g_pButtonDown = NULL;
//     CMFCMenuBar* pMenuBar = DYNAMIC_DOWNCAST(CMFCMenuBar, m_pWndParent);          // RTC 0x1803b1268
//     if (m_pPopupMenu != NULL) {
//         m_pPopupMenu->m_bAutoDestroyParent = FALSE;                                // +0x1f0
//         m_pPopupMenu->DestroyWindow();                                             // vslot 0xd0 (26)
//         m_pPopupMenu = NULL;
//         if (pMenuBar != NULL) pMenuBar->SetHot(NULL);                              // 0x157d80
//     } else {
//         CMFCPopupMenuBar* pParentMenu = DYNAMIC_DOWNCAST(CMFCPopupMenuBar, m_pWndParent);   // RTC 0x1803b1478
//         if (!bDelay || pParentMenu == NULL || CMFCToolBar::m_bCustomizeMode)
//             DropDownToolbar(pWnd);                                                 // 0x5ec30
//         if (pMenuBar != NULL) pMenuBar->SetHot(this);
//     }
//     if (m_pWndParent != NULL) ::InvalidateRect(m_pWndParent->m_hWnd, &m_rect, TRUE);
//     return FALSE;
// The IAT slots resolve to USER32 SetTimer / KillTimer / InvalidateRect.
// The frame is destroyed through DestroyPopupFrame (see its comment);
// DropDownToolbar is this file's stub, so today the else branch drops nothing
// down.  m_uiShowBarDelay is OpenMFC's StaticData.cpp value (0 there, 500 in
// retail .data -- see headerRequests).
// Symbol: ?OnClick@CMFCDropDownToolbarButton@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnClick_CMFCDropDownToolbarButton__UEAAHPEAVCWnd__H_Z(void* pThis, CWnd* pWnd, int bDelay) {
    if (!pThis) return FALSE;
    CMFCToolBarButton* pBase = Base(pThis);
    if (Timer(pThis) == 0) {
        if (pBase->m_pWndParent != nullptr) {
            Timer(pThis) = static_cast<UINT>(::SetTimer(pBase->m_pWndParent->m_hWnd, kTimerId,
                                                        impl__m_uiShowBarDelay_CMFCDropDownToolbarButton__2IA,
                                                        ButtonTimerProc));
        }
        g_pButtonDown = pThis;
        return FALSE;
    }
    if (pBase->m_pWndParent != nullptr) ::KillTimer(pBase->m_pWndParent->m_hWnd, Timer(pThis));
    Timer(pThis) = 0;
    g_pButtonDown = nullptr;

    CWnd* pParent = pBase->m_pWndParent;
    CMFCToolBar* pMenuBar = IsKindOfClass(pParent, impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ())
                                ? static_cast<CMFCToolBar*>(pParent) : nullptr;
    if (CWnd* pFrame = PopupMenu(pThis)) {
        At<BOOL>(pFrame, kOffFrameAutoDestroyParent) = FALSE;
        DestroyPopupFrame(pFrame);
        PopupMenu(pThis) = nullptr;
        if (pMenuBar != nullptr) impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(pMenuBar, nullptr);
    } else {
        const bool bParentIsPopupMenuBar =
            IsKindOfClass(pParent, impl__GetThisClass_CMFCPopupMenuBar__SAPEAUCRuntimeClass__XZ());
        if (!bDelay || !bParentIsPopupMenuBar || impl__m_bCustomizeMode_CMFCToolBar__1HA != 0) {
            impl__DropDownToolbar_CMFCDropDownToolbarButton__QEAAHPEAVCWnd___Z(pThis, pWnd);
        }
        if (pMenuBar != nullptr) impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(pMenuBar, pBase);
    }
    if (pBase->m_pWndParent != nullptr) ::InvalidateRect(pBase->m_pWndParent->m_hWnd, RectOf(pBase), TRUE);
    return FALSE;
}

// Retail (RVA 0x5eae0, mfc140u), fully transcribed:
//     CMFCMenuBar* pMenuBar = DYNAMIC_DOWNCAST(CMFCMenuBar, m_pWndParent);          // RTC 0x1803b1268
//     if (m_uiTimer != 0) {
//         if (m_pWndParent != NULL) ::KillTimer(m_pWndParent->m_hWnd, m_uiTimer);
//         m_uiTimer = 0;
//         g_pButtonDown = NULL;
//         return FALSE;
//     }
//     if (m_pPopupMenu != NULL) {
//         m_pPopupMenu->m_bAutoDestroyParent = FALSE;                                // +0x1f0
//         m_pPopupMenu->DestroyWindow();                                             // vslot 0xd0 (26)
//         m_pPopupMenu = NULL;
//         if (pMenuBar != NULL) pMenuBar->SetHot(NULL);                              // 0x157d80
//     }
//     return TRUE;
// Symbol: ?OnClickUp@CMFCDropDownToolbarButton@@UEAAHXZ
extern "C" int MS_ABI impl__OnClickUp_CMFCDropDownToolbarButton__UEAAHXZ(void* pThis) {
    if (!pThis) return FALSE;
    CMFCToolBarButton* pBase = Base(pThis);
    CWnd* pParent = pBase->m_pWndParent;
    CMFCToolBar* pMenuBar = IsKindOfClass(pParent, impl__GetThisClass_CMFCMenuBar__SAPEAUCRuntimeClass__XZ())
                                ? static_cast<CMFCToolBar*>(pParent) : nullptr;
    if (Timer(pThis) != 0) {
        if (pBase->m_pWndParent != nullptr) ::KillTimer(pBase->m_pWndParent->m_hWnd, Timer(pThis));
        Timer(pThis) = 0;
        g_pButtonDown = nullptr;
        return FALSE;
    }
    if (CWnd* pFrame = PopupMenu(pThis)) {
        At<BOOL>(pFrame, kOffFrameAutoDestroyParent) = FALSE;
        DestroyPopupFrame(pFrame);
        PopupMenu(pThis) = nullptr;
        if (pMenuBar != nullptr) impl__SetHot_CMFCToolBar__QEAAHPEAVCMFCToolBarButton___Z(pMenuBar, nullptr);
    }
    return TRUE;
}

// Retail (RVA 0x5eba0, mfc140u), fully transcribed:
//     CMFCToolBarButton::OnChangeParentWnd(pWndParent);          // 0x15e570
//     m_bText = FALSE;                                           // +0x0c
//     m_strText.Empty();                                         // +0x38
//     m_bUserButton = FALSE;                                     // +0x08
// The base body (which in retail stores m_pWndParent and recomputes
// m_bExtraSize) is still an empty stub in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp); it is called by name as
// retail does, so it takes effect here once it is implemented.
// Symbol: ?OnChangeParentWnd@CMFCDropDownToolbarButton@@UEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnChangeParentWnd_CMFCDropDownToolbarButton__UEAAXPEAVCWnd___Z(void* pThis, CWnd* pWndParent) {
    if (!pThis) return;
    CMFCToolBarButton* pBase = Base(pThis);
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(pBase, pWndParent);
    pBase->m_bText = FALSE;
    pBase->m_strText.Empty();
    pBase->m_bUserButton = FALSE;
}

// Retail (RVA 0x5ebd0, mfc140u), fully transcribed:
//     if (m_pWndParent != NULL && ::IsWindow(m_pWndParent->m_hWnd)) {
//         ::InvalidateRect(m_pWndParent->m_hWnd, &m_rect, TRUE);
//         ::UpdateWindow(m_pWndParent->m_hWnd);
//     }
// (USER32 IsWindow / InvalidateRect / UpdateWindow, resolved from the IAT.)
// Symbol: ?OnCancelMode@CMFCDropDownToolbarButton@@UEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CMFCDropDownToolbarButton__UEAAXXZ(void* pThis) {
    if (!pThis) return;
    CMFCToolBarButton* pBase = Base(pThis);
    if (pBase->m_pWndParent != nullptr && ::IsWindow(pBase->m_pWndParent->m_hWnd)) {
        ::InvalidateRect(pBase->m_pWndParent->m_hWnd, RectOf(pBase), TRUE);
        ::UpdateWindow(pBase->m_pWndParent->m_hWnd);
    }
}

// ===========================================================================
// Layout / drawing
// ===========================================================================

// Retail (RVA 0x5ed50, mfc140u), fully transcribed:
//     if (m_nID == 0 && m_pToolBar != NULL) {
//         CMFCToolBarButton* pButton = m_pToolBar->GetButton(0);   // 0x14fe00
//         if (pButton != NULL) SetDefaultCommand(pButton->m_nID);   // 0x5e120
//     }
//     m_iImage = m_iSelectedImage;
//     BOOL bImage = m_bImage;  m_bImage = TRUE;
//     CSize size = CMFCToolBarButton::OnCalculateSize(pDC, sizeDefault, bHorz);   // 0x15d680
//     m_bImage = bImage;
//     m_iImage = -1;
//     double dScale = afxGlobalData.GetRibbonImageScale();
//     int nArrow = (dScale != 1.0) ? (int)(dScale * 7.0) : 7;
//     size.cx += CMFCToolBar::m_bLargeIcons ? nArrow + 2 : nArrow / 2 + 1;
//     return size;
// (The scale is read twice, through two inlined initialisation gates; the
// result is the one above.)  The base OnCalculateSize is still an empty stub
// in OpenMFC (featurepack/toolbar/CMFCToolBarButton.cpp) that writes nothing,
// so the base contribution is (0, 0) until it is implemented.
// Symbol: ?OnCalculateSize@CMFCDropDownToolbarButton@@UEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCDropDownToolbarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz) {
    if (!pRet) return pRet;
    pRet->cx = 0; pRet->cy = 0;
    if (!pThis || !pSizeDefault) return pRet;
    CMFCToolBarButton* pBase = Base(pThis);

    if (pBase->m_nID == 0 && ToolBar(pThis) != nullptr) {
        if (const CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(ToolBar(pThis), 0)) {
            impl__SetDefaultCommand_CMFCDropDownToolbarButton__QEAAXI_Z(pThis, pButton->m_nID);
        }
    }
    pBase->m_iImage = SelectedImage(pThis);
    const BOOL bImage = pBase->m_bImage;
    pBase->m_bImage = TRUE;
    SIZE size = { 0, 0 };
    impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(pBase, &size, pDC, pSizeDefault, bHorz);
    pBase->m_bImage = bImage;
    pBase->m_iImage = -1;

    const double dScale = RibbonImageScale();
    const int nArrow = (dScale != 1.0) ? static_cast<int>(dScale * static_cast<double>(kArrowSize)) : kArrowSize;
    size.cx += impl__m_bLargeIcons_CMFCToolBar__1HA != 0 ? nArrow + 2 : nArrow / 2 + 1;
    *pRet = size;
    return pRet;
}

// Retail (RVA 0x5e400, mfc140u) draws the button.  What was read from the
// disassembly (callee names from the mfc140.dll twin, whose body is
// byte-identical; RVAs marked (mfc140.dll) are that image's):
//   * FillInterior(pDC, rect, bHighlight, FALSE) first (the call at 0x5e442
//     into 0x15e300, mfc140u);
//   * only when m_pToolBar != NULL: m_bInternalDraw = TRUE; the image size is
//     m_pToolBar->m_sizeImageLocked / m_sizeCurImageLocked (+0x1160 / +0x1170,
//     picked by +0x10bc) for a locked bar (+0x10b8), else the statics
//     ?m_sizeImage@ / ?m_sizeCurImage@CMFCToolBar@@, scaled by
//     afxGlobalData.GetRibbonImageScale(); the image list is
//     m_pToolBar->m_LargeImagesLocked (+0x8c0, when CMFCToolBar::m_bLargeIcons
//     and the int at its +0x8 is > 0) or m_ImagesLocked (+0x3f8), or
//     ?m_pUserImages@CMFCToolBar@@ for m_bLocalUserButton; the image is drawn by
//     CMFCToolBarButton::OnDraw (0x15af70, mfc140.dll) between
//     CMFCToolBarImages::PrepareDrawImage / EndDrawImage (0x16a680 / 0x16a890,
//     mfc140.dll) with m_iImage = m_iUserImage = m_iSelectedImage and
//     m_bImage / m_bDisableFill (+0x58) raised, all restored afterwards;
//   * the drop-down triangle: a CDC vslot 0x58 call with 8, a
//     ?SelectObject@CDC@@ of the afxGlobalData member at +0xd8, and GDI32
//     ::Polygon with 3 points;
//   * when !bCustomizeMode, this->vslot 0xb8 (23, HaveHotBorder in the
//     harvested CMFCToolBarButton order) returns nonzero and bDrawBorder,
//     CMFCVisualManager::GetInstance()->OnDrawButtonBorder(pDC, this, rect,
//     state 1 or 2 chosen from m_pPopupMenu / m_nStyle / bHighlight / m_nID).
//     GetInstance is the inline accessor, outlined at 0x9774 (mfc140u; 0x97f4
//     in mfc140.dll): it returns ?m_pVisManager@CMFCVisualManager@@
//     (0x3be3c0), creating it first from ?m_pRTIDefault@ (0x3be3b8) through
//     CRuntimeClass::CreateObject or as new CMFCVisualManager(FALSE)
//     (0x108 bytes, ctor 0x184030).  Slot 36 (0x120) of the CMFCVisualManager
//     vftable (0x18031c128, mfc140u) is ?OnDrawButtonBorder@CMFCVisualManager@@.
// STUB: the body depends on CMFCToolBarImages::PrepareDrawImage /
// EndDrawImage and tagAFXDrawState, the locked image lists of the toolbar,
// CMFCToolBar::m_pUserImages and the base OnDraw / FillInterior; the base
// OnDraw and FillInterior are still empty stubs in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp), and so is PrepareDrawImage
// (a placeholder-signature stub returning 0 in
// featurepack/toolbar/CMFCToolBarImages.cpp); a partial transcription would
// draw only the arrow (and the visual manager's hot border) over an
// unpainted button.  The parameter list is the
// one the mangled name describes (the previous placeholder list was not).
// Symbol: ?OnDraw@CMFCDropDownToolbarButton@@UEAAXPEAVCDC@@AEBVCRect@@PEAVCMFCToolBarImages@@HHHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCDropDownToolbarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    void* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons) {
    (void)pThis; (void)pDC; (void)pRect; (void)pImages;
    (void)bHorz; (void)bCustomizeMode; (void)bHighlight; (void)bDrawBorder; (void)bGrayDisabledButtons;
}

// Retail (RVA 0x5f130, mfc140u), fully transcribed:
//     CString strText = m_strText;
//     m_strText = m_strName;
//     int iResult = CMFCToolBarButton::OnDrawOnCustomizeList(pDC, rect, bSelected);   // 0x15db10
//     m_strText = strText;
//     return iResult;
// The base body is still an empty stub returning 0 in OpenMFC
// (featurepack/toolbar/CMFCToolBarButton.cpp); it is called by name as
// retail does.
// Symbol: ?OnDrawOnCustomizeList@CMFCDropDownToolbarButton@@UEAAHPEAVCDC@@AEBVCRect@@H@Z
extern "C" int MS_ABI impl__OnDrawOnCustomizeList_CMFCDropDownToolbarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(
    void* pThis, CDC* pDC, const RECT* pRect, int bSelected) {
    if (!pThis) return 0;
    CMFCToolBarButton* pBase = Base(pThis);
    const CString strText = pBase->m_strText;
    pBase->m_strText = StrName(pThis);
    const int iResult = impl__OnDrawOnCustomizeList_CMFCToolBarButton__UEAAHPEAVCDC__AEBVCRect__H_Z(pBase, pDC, pRect, bSelected);
    pBase->m_strText = strText;
    return iResult;
}

// ===========================================================================
// Customization
// ===========================================================================

// Retail (RVA 0x5eea0, mfc140u), fully transcribed:
//     if (m_pToolBar == NULL) return FALSE;
//     if (!CMFCToolBarButton::ExportToMenuButton(menuButton)) return FALSE;       // 0x15e140
//     CMenu menu;  menu.Attach(::CreatePopupMenu());
//     for (POSITION pos = m_pToolBar->m_Buttons.GetHeadPosition(); pos != NULL;) {   // +0x1190
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*) m_pToolBar->m_Buttons.GetNext(pos);
//         ENSURE(pButton != NULL);                                                  // AfxThrowInvalidArgException
//         if (pButton->m_nStyle & TBBS_SEPARATOR) {
//             ::AppendMenu(menu.m_hMenu, MF_SEPARATOR, 0, NULL);
//         } else if (pButton->m_nID != 0 && pButton->m_nID != (UINT)-1) {
//             CString strItem = pButton->m_strText;
//             if (strItem.IsEmpty()) {
//                 CString strMessage;  int iOffset;
//                 HINSTANCE h = AfxFindStringResourceHandle(pButton->m_nID);
//                 if (h != NULL && strMessage.LoadString(h, pButton->m_nID) &&
//                     (iOffset = strMessage.Find(_T('\n'))) != -1)
//                     strItem = strMessage.Mid(iOffset + 1);
//             }
//             ::AppendMenu(menu.m_hMenu, MF_STRING, pButton->m_nID, strItem);
//         }
//     }
//     menuButton.m_nID = 0;
//     menuButton.m_strText = m_strName;
//     menuButton.SetImage(-1);                                                      // vslot 0x190 (50)
//     menuButton.m_bImage = FALSE;
//     menuButton.CreateFromMenu(menu.GetSafeHmenu());                               // vslot 0x1b0 (54)
//     menu.DestroyMenu();
//     return TRUE;
// DEVIATIONS: (1) the base call is not made -- the retail base body
// (0x15e140) always returns TRUE and its only side effect is, when this
// button's m_strText is empty and m_nID != 0, to set menuButton.m_strText
// from the string resource -- which this body overwrites with m_strName; OpenMFC's
// base export is still an empty stub returning 0 that would make this body
// always fail.  (2) The buttons come from the GetCount / GetButton thunks
// (file header, THE TOOLBAR).  (3) The menu is a bare HMENU from
// ::CreatePopupMenu / ::DestroyMenu rather than a temporary CMenu (whose
// Attach / DestroyMenu only add the handle-map bookkeeping).  (4) SetImage
// goes to the exported CMFCToolBarButton body (slot 50 of the stock
// CMFCToolBarMenuButton vftable 0x180316338 (mfc140.dll) is
// ?SetImage@CMFCToolBarButton@@), CreateFromMenu to the exported
// CMFCToolBarMenuButton thunk (a mingw virtual call); a client-derived
// override of either is not reached by MSVC slot.
// Symbol: ?ExportToMenuButton@CMFCDropDownToolbarButton@@UEBAHAEAVCMFCToolBarMenuButton@@@Z
extern "C" int MS_ABI impl__ExportToMenuButton_CMFCDropDownToolbarButton__UEBAHAEAVCMFCToolBarMenuButton___Z(
    const void* pThis, CMFCToolBarMenuButton* pMenuButton) {
    if (!pThis || !pMenuButton) return FALSE;
    const CMFCToolBar* pBar = ToolBar(pThis);
    if (pBar == nullptr) return FALSE;

    HMENU hMenu = ::CreatePopupMenu();
    const int nCount = impl__GetCount_CMFCToolBar__QEBAHXZ(pBar);
    for (int i = 0; i < nCount; i++) {
        const CMFCToolBarButton* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pBar, i);
        if (pButton == nullptr) {   // retail: ENSURE(pButton != NULL)
            if (hMenu != nullptr) ::DestroyMenu(hMenu);
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;
        }
        if (pButton->m_nStyle & kTbbsSeparator) {
            ::AppendMenuW(hMenu, kMfSeparator, 0, nullptr);
            continue;
        }
        const UINT nID = pButton->m_nID;
        if (nID == 0 || nID == static_cast<UINT>(-1)) continue;
        CString strItem = pButton->m_strText;
        if (strItem.IsEmpty()) {
            CString strMessage;
            HINSTANCE hInst = static_cast<HINSTANCE>(impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(nID));
            if (hInst != nullptr &&
                impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(&strMessage, hInst, nID)) {
                const int iOffset = strMessage.Find(L'\n');
                if (iOffset != -1) strItem = strMessage.Mid(iOffset + 1);
            }
        }
        ::AppendMenuW(hMenu, kMfString, nID, strItem.GetString());
    }

    CMFCToolBarButton* pMenuBase = pMenuButton;
    pMenuBase->m_nID = 0;
    pMenuBase->m_strText = StrName(pThis);
    impl__SetImage_CMFCToolBarButton__UEAAXH_Z(pMenuBase, -1);
    pMenuBase->m_bImage = FALSE;
    impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(pMenuButton, hMenu);
    if (hMenu != nullptr) ::DestroyMenu(hMenu);
    return TRUE;
}

// Retail (RVA 0x5f1d0, mfc140u), fully transcribed:
//     ::EnableMenuItem(pMenu->m_hMenu, 0x4212, MF_BYCOMMAND | MF_GRAYED);
//     ::EnableMenuItem(pMenu->m_hMenu, 0x4213, MF_BYCOMMAND | MF_GRAYED);
//     ::EnableMenuItem(pMenu->m_hMenu, 0x4214, MF_BYCOMMAND | MF_GRAYED);
//     ::EnableMenuItem(pMenu->m_hMenu, 0x4211, MF_BYCOMMAND | MF_GRAYED);
//     ::EnableMenuItem(pMenu->m_hMenu, 0x420f, MF_BYCOMMAND | MF_GRAYED);
//     return TRUE;
// (USER32 EnableMenuItem, resolved from the IAT.  In the SDK afxribbonres.h
// the IDs are ID_AFXBARRES_TOOLBAR_IMAGE (16914), _TEXT (16915),
// _IMAGE_AND_TEXT (16916), ID_AFXBARRES_TOOLBAR_APPEARANCE (16913) and
// ID_AFXBARRES_COPY_IMAGE (16911), greyed in that order.)  Retail does not
// test pMenu for NULL; this does.
// Symbol: ?OnCustomizeMenu@CMFCDropDownToolbarButton@@UEAAHPEAVCMenu@@@Z
extern "C" int MS_ABI impl__OnCustomizeMenu_CMFCDropDownToolbarButton__UEAAHPEAVCMenu___Z(void* pThis, CMenu* pMenu) {
    (void)pThis;
    if (pMenu == nullptr) return TRUE;
    static const UINT kGrayedIds[] = { 0x4212, 0x4213, 0x4214, 0x4211, 0x420f };
    for (UINT id : kGrayedIds) ::EnableMenuItem(pMenu->m_hMenu, id, kMfByCommandGrayed);
    return TRUE;
}
