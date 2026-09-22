// CMFCFontComboBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

// ===========================================================================
// CMFCFontComboBox -- the feature-pack font picker (a CComboBox whose items
// carry CMFCFontInfo* item data; afxfontcombobox.h).
//
// The bodies below were transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  EVERY
// RVA IN THIS FILE IS AN mfc140u.dll RVA.  Entry points, each confirmed
// either by the export map (mfc140u_rva_symbols.json), by the class vftable
// at 0x1802e8b98 (slot numbers below), or by the class message map at
// 0x1802e8af8 (entries at 0x1802e8b10):
//   ??0 ctor           0x60ef0 (export)    ??1 dtor          0x60f90 (export)
//   scalar del. dtor   0x60f40 (slot 1)    GetMessageMap     0x60fc0 (slot 12)
//   PreTranslateMessage 0x60fd0 (slot 69)  DeleteString      0x61120 (export)
//   CompareItem        0x611b0 (slot 94)   DrawItem          0x612a0 (slot 92)
//   MeasureItem        0x61620 (slot 93)   PreSubclassWindow 0x616d0 (slot 22)
//   OnCreate           0x61700 (msgmap WM_CREATE)
//   Init               0x61730 (export)    CleanUp           0x61780 (export)
//   Setup              0x61840 (callee of Init / OnInitControl with the
//                               (7, DEFAULT_CHARSET, DEFAULT_PITCH) and
//                               (mask, 1, 0) argument shapes)
//   OnDestroy          0x61a20 (msgmap WM_DESTROY)
//   SelectFont(CMFCFontInfo*)     0x61a40   SelectFont(LPCTSTR, BYTE) 0x61b50
//   GetSelFont         0x61c30 (export)
//   OnInitControl      0x61c90 (msgmap message 0x37c, WM_MFC_INITCTRL in afxpriv.h)
// (Setup / the two SelectFont / OnInitControl entries are not in the export
// map; each is also the ANSI twin's export entry + 0x1d0, the constant shift
// every mapped entry of this class shows between mfc140.dll and mfc140u.dll,
// and each body was read at that address.)
//
// Retail object layout, read from the constructor (0x60ef0) and destructor
// (0x60f90), cross-checked against atlmfc/include/afxfontcombobox.h
// (m_Images, m_bToolBarMode after the CComboBox base):
//   +0x000  CComboBox base, 0xe8 bytes (== CWnd, m_hWnd at +0x40; the ctor
//           calls ??0CWnd@@QEAA@XZ (0x28a700) directly -- CComboBox's ctor
//           is inline)
//   +0x0e8  CImageList m_Images  (inline ctor: vfptr = 0x180337b38, the
//           CImageList vftable; m_hImageList (+0xf0) = NULL.  The dtor calls
//           ??1CImageList@@UEAA@XZ (0x296be0) on this+0xe8.)
//   +0x0f8  BOOL m_bToolBarMode  (ctor: movl $0,0xf8)
//   sizeof == 0x100 (the scalar deleting destructor 0x60f40, on its
//                    flags-bit-2 path, loads `mov $0x100,%edx` as the size
//                    argument for the call at 0x60f70 -- whose target 0x27d0
//                    is an ICF-folded bare `ret`; the flags-bit-2-clear
//                    path frees through the CRT `free` import instead.  The
//                    value also equals the member layout: 0xf8 + 4, rounded
//                    up to the 8-byte alignment the vfptr imposes.  No member
//                    access in any body reaches past +0xfb.)
// The class is not declared in OpenMFC's public headers, so this file is the
// only place that layout lives (FontComboLayout below).
//
// CMFCFontInfo (afxtoolbarfontcombobox.h: CObject base, then const CString
// m_strName, const CString m_strScript, const BYTE m_nCharSet, const BYTE
// m_nPitchAndFamily, const int m_nType).  The retail bodies read m_strName at
// +0x08, m_strScript at +0x10, m_nCharSet at +0x18, m_nPitchAndFamily at
// +0x19, m_nType at +0x1c; Setup allocates it with operator new(0x20) and
// installs the vftable 0x1802e91a8.  A CString there is the ATL one: a single
// pointer to the character buffer (CStringData header in front of it).
// FontInfoLayout below reads those pointers as `const wchar_t*` -- which is
// what the retail bodies do (`mov 0x8(%rax),%rdx` then wcscmp).  No
// CMFCFontInfo reaching this file is built by OpenMFC today (Setup is a stub,
// see there), so every object read here was built by client code compiled
// against the shipping headers, i.e. with exactly this layout.
//
// Deviations applied throughout:
//  (1) vptr.  Retail's ctor and dtor store the CMFCFontComboBox vftable
//      (0x1802e8b98).  OpenMFC has no MSVC-layout vtable for this class; the
//      ctor keeps the vptr ??0CWnd@@ installs and records it (g_ownVptr), and
//      the dtor stores that recorded value back in place of retail's store,
//      so that ??1CComboBox@@'s C++ virtual-destructor call dispatches on
//      OpenMFC's table rather than on a client-derived class's MSVC table
//      (the convention of controls/CMFCEditBrowseCtrl.cpp).  No body in this
//      file makes a virtual call on `this`, so nothing else depends on it.
//  (2) The embedded CImageList gets no vptr (same reason; the convention of
//      controls/CMFCEditBrowseCtrl.cpp's m_ImageBrowse).
//  (3) CComboBox / CWnd inline wrappers that retail expands to a raw
//      ::SendMessage on m_hWnd (GetCount, GetItemDataPtr, SetCurSel,
//      GetCurSel, DeleteString, ResetContent, GetDroppedState, ShowDropDown)
//      are written as that same ::SendMessage.  IAT slots, resolved with
//      iatu.py: 0x1802c7120 SendMessageW, 0x1802c7138 IsWindow, 0x1802c72d8
//      GetParent, 0x1802c6ca0 GetKeyState, 0x1802c6ce0 GetNextDlgTabItem,
//      0x1802c6c08 GetWindowRect, 0x1802c7770 wcscmp, 0x1802c7738 wcscoll;
//      0x1802c7b30 is not an import: it is the slot every indirect call in
//      these bodies goes through with the real target loaded in %rax.
//  (4) NULL guards on `this` are added at every entry (retail has none).
//      Other added guards are named at their sites.
//
// OpenMFC's message map for this class (detail/Mfc04MsgmapSupport.cpp,
// classCMFCFontComboBox_msgmap) is EMPTY, so OnCreate / OnDestroy /
// OnInitControl are reached only by callers that name them until that table
// is filled in (reported as a headerRequest).
// ===========================================================================

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cwchar>

namespace {

// CWnd::m_hWnd (include/openmfc/afxwin.h places it at 0x40; every retail body
// in this file reads it as 0x40(%rcx)).
constexpr size_t kHWndOffset = 0x40;
static_assert(offsetof(CWnd, m_hWnd) == kHWndOffset, "CWnd::m_hWnd +0x40");
static_assert(sizeof(CWnd) == 0xe8, "CWnd is 0xe8 bytes (retail CComboBox base)");
static_assert(sizeof(CComboBox) == 0xe8, "CComboBox adds no members to CWnd");

// File-local layout view of the retail CMFCFontComboBox (see the file header).
struct FontComboLayout {
    unsigned char base[0xe8];       // CComboBox (m_hWnd at +0x40)
    unsigned char m_Images[0x10];   // +0x0e8  CImageList {vfptr, m_hImageList}
    BOOL          m_bToolBarMode;   // +0x0f8
    int           pad_fc;           // +0x0fc  alignment tail, never written by retail
};
static_assert(sizeof(FontComboLayout) == 0x100, "CMFCFontComboBox: retail sizeof 0x100 (size argument in scalar deleting dtor 0x60f40)");
static_assert(offsetof(FontComboLayout, m_Images) == 0xe8, "m_Images +0xe8");
static_assert(offsetof(FontComboLayout, m_bToolBarMode) == 0xf8, "m_bToolBarMode +0xf8");
static_assert(sizeof(CImageList) <= 0x10, "OpenMFC's CImageList must fit the retail 0x10-byte member");
static_assert(offsetof(CImageList, m_hImageList) == 8, "CImageList::m_hImageList at +8 == retail +0xf0");

// File-local layout view of a (client-built) CMFCFontInfo (see the file header).
struct FontInfoLayout {
    void*          vfptr;             // +0x00  CObject-derived vftable (retail 0x1802e91a8)
    const wchar_t* m_strName;         // +0x08  ATL CString == pointer to its characters
    const wchar_t* m_strScript;       // +0x10
    BYTE           m_nCharSet;        // +0x18
    BYTE           m_nPitchAndFamily; // +0x19
    int            m_nType;           // +0x1c
};
static_assert(sizeof(FontInfoLayout) == 0x20, "CMFCFontInfo: retail operator new(0x20) in Setup");
static_assert(offsetof(FontInfoLayout, m_strName) == 0x08, "m_strName +0x08");
static_assert(offsetof(FontInfoLayout, m_strScript) == 0x10, "m_strScript +0x10");
static_assert(offsetof(FontInfoLayout, m_nCharSet) == 0x18, "m_nCharSet +0x18");
static_assert(offsetof(FontInfoLayout, m_nPitchAndFamily) == 0x19, "m_nPitchAndFamily +0x19");
static_assert(offsetof(FontInfoLayout, m_nType) == 0x1c, "m_nType +0x1c");

// Win32 structs, pinned at the offsets the retail bodies read.
static_assert(offsetof(MSG, message) == 0x8 && offsetof(MSG, wParam) == 0x10, "MSG layout (PreTranslateMessage)");
static_assert(offsetof(COMPAREITEMSTRUCT, itemID1) == 0x10 && offsetof(COMPAREITEMSTRUCT, itemID2) == 0x20, "COMPAREITEMSTRUCT layout (CompareItem)");
static_assert(offsetof(MEASUREITEMSTRUCT, itemWidth) == 0xc && offsetof(MEASUREITEMSTRUCT, itemHeight) == 0x10, "MEASUREITEMSTRUCT layout (MeasureItem)");

// Message numbers and constants the retail bodies use as immediates.
static_assert(CB_ADDSTRING == 0x143 && CB_DELETESTRING == 0x144 && CB_GETCOUNT == 0x146 &&
              CB_GETCURSEL == 0x147 && CB_RESETCONTENT == 0x14b && CB_SETCURSEL == 0x14e &&
              CB_SHOWDROPDOWN == 0x14f && CB_GETITEMDATA == 0x150 && CB_SETITEMDATA == 0x151 &&
              CB_GETDROPPEDSTATE == 0x157 && CB_FINDSTRINGEXACT == 0x158, "CB_* message numbers");
static_assert((DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE) == 7 && RASTER_FONTTYPE == 1 &&
              DEVICE_FONTTYPE == 2 && TRUETYPE_FONTTYPE == 4, "font-type bits");
static_assert(DEFAULT_CHARSET == 1 && DEFAULT_PITCH == 0, "Setup defaults");
static_assert(E_FAIL == static_cast<HRESULT>(0x80004005), "E_FAIL");

// afxGlobalData (720-byte block, layout pinned in core/runtime/AFX_GLOBAL_DATA.cpp):
//   +0x000 m_bInitialized, the one-time gate every retail reader tests;
//   +0x2a8 m_nTextHeightHorz (afxGlobalData.GetTextHeight()).
constexpr int kGlobalDataInitGate       = 0x000;
constexpr int kGlobalDataTextHeightHorz = 0x2a8;

// Fallback item height floor MeasureItem applies (`mov $0x10,%ecx; cmovl`).
constexpr int kMinItemHeight = 16;

// DrawItem immediates: the font-type glyph strip (bitmap resource 0x4268,
// 16-pixel images) and the 22-pixel text indent (`addl $0x16`).
constexpr unsigned int kFontTypeBitmapId  = 0x4268;
constexpr int          kFontTypeImageSize = 16;
constexpr int          kTextIndent        = 22;

// afxGlobalData fields DrawItem reads (named in core/runtime/AFX_GLOBAL_DATA.cpp):
//   +0x048 clrHilite, +0x04c clrTextHilite, +0x1b0 fontRegular.m_hObject
//   (the CFont at +0x1a8).
constexpr int kGlobalDataClrHilite          = 0x048;
constexpr int kGlobalDataClrTextHilite      = 0x04c;
constexpr int kGlobalDataFontRegularHandle  = 0x1b0;

static_assert(offsetof(DRAWITEMSTRUCT, itemID) == 0x8 && offsetof(DRAWITEMSTRUCT, itemState) == 0x10 &&
              offsetof(DRAWITEMSTRUCT, hDC) == 0x20 && offsetof(DRAWITEMSTRUCT, rcItem) == 0x28 &&
              offsetof(DRAWITEMSTRUCT, itemData) == 0x38, "DRAWITEMSTRUCT layout (DrawItem)");
static_assert(sizeof(LOGFONTW) == 0x5c && offsetof(LOGFONTW, lfCharSet) == 0x17 &&
              offsetof(LOGFONTW, lfFaceName) == 0x1c, "LOGFONTW layout (DrawItem)");
static_assert(ODS_SELECTED == 1 && ODS_FOCUS == 0x10 && TRANSPARENT == 1 && SYMBOL_CHARSET == 2 &&
              (DT_SINGLELINE | DT_VCENTER) == 0x24 && ILD_NORMAL == 0, "DrawItem immediates");

} // namespace

// ---- sibling impl__ exports called by the bodies in this file -------------
// (BRIEFING §1: C++ methods of other classes exist in this DLL only as impl__
// thunks.)  Every declaration matches a definition that exists today; the
// file holding it is named.  Static data is declared with the type its
// defining file uses.
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                  // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CComboBox__UEAA_XZ(void* pThis);                              // core/controls/CtorDtorPlacement.cpp
extern "C" CImageList* MS_ABI impl___0CImageList__QEAA_XZ(CImageList* pThis);               // core/gdi/CImageList.cpp
extern "C" void  MS_ABI impl___1CImageList__UEAA_XZ(CImageList* pThis);                      // core/gdi/CImageList.cpp
extern "C" int   MS_ABI impl__Create_CImageList__QEAAHIHHK_Z(CImageList* pThis, unsigned int nBitmapID, int cx, int nGrow, unsigned long crMask); // core/gdi/CImageList.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);            // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(CWnd* pThis, MSG* pMsg); // core/window/CWnd.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                         // core/window/Thunks.cpp
extern "C" void  MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);                          // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                      // core/window/Thunks.cpp
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis); // core/window/Thunks.cpp
extern "C" void  MS_ABI impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CComboBox* pThis, int nIndex, CString* pText);                                      // core/controls/CComboBox.cpp
extern "C" int   MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass); // core/runtime/CObject.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ();  // featurepack/toolbar/RuntimeClasses.cpp
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ(); // detail/CWinAppSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                          // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);                          // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);              // core/runtime/AFX_GLOBAL_DATA.cpp
extern "C" int   MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const char* pszUTF8, CString* pStrResult, int cbUTF8);                                     // featurepack/controls/CMFCControlContainer.cpp
extern "C" void  MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* pThis, const wchar_t* lpszBuffer); // featurepack/controls/CTagManager.cpp
extern "C" void  MS_ABI impl___1CTagManager__UEAA_XZ(void* pThis);                           // featurepack/controls/CTagManager.cpp
extern "C" int   MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(void* pTagManager, const wchar_t* lpszTag, int* pValue); // featurepack/CMFC_misc_stubs.cpp

extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                     // 0x1803c1620 -- featurepack/CMFC_misc_stubs.cpp
extern "C" void*         impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA;                         // 0x1803be1b8 -- featurepack/CMFC_misc_stubs.cpp
extern "C" std::int32_t  impl__m_bFlat_CMFCToolBarComboBoxButton__1HA;                       // 0x1803b1b74 -- featurepack/toolbar/StaticData.cpp
extern "C" std::int32_t  impl__m_nFontHeight_CMFCToolBarFontComboBox__2HA;                   // 0x1803be380 -- featurepack/toolbar/StaticData.cpp
extern "C" std::int32_t  impl__m_bDrawUsingFont_CMFCFontComboBox__2HA;                       // 0x1803be210 -- featurepack/controls/StaticData.cpp

// Thunks defined further down this file that earlier bodies call.
extern "C" void MS_ABI impl__CleanUp_CMFCFontComboBox__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__Init_CMFCFontComboBox__IEAAXXZ(void* pThis);
extern "C" int  MS_ABI impl__Setup_CMFCFontComboBox__QEAAHHEE_Z(void* pThis, int nFontType, unsigned char nCharSet, unsigned char nPitchAndFamily);

namespace {

void* g_ownVptr = nullptr;   // the vptr ??0CWnd@@QEAA@XZ installs, recorded by the first ctor to run (deviation (1))

inline FontComboLayout* L(void* p) { return static_cast<FontComboLayout*>(p); }
inline const FontComboLayout* L(const void* p) { return static_cast<const FontComboLayout*>(p); }
inline HWND HWndOf(const void* p) {
    return *reinterpret_cast<const HWND*>(static_cast<const unsigned char*>(p) + kHWndOffset);
}
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
inline CImageList* Images(void* p) { return reinterpret_cast<CImageList*>(L(p)->m_Images); }

// ENSURE(::IsWindow(m_hWnd)) as retail expands it: IsWindow (IAT 0x1802c7138),
// and on failure ?AfxThrowInvalidArgException@@YAXXZ (0x227720).  Returns
// false when the (OpenMFC) throw helper returns instead of throwing.
inline bool EnsureIsWindow(const void* pThis) {
    if (::IsWindow(HWndOf(pThis))) return true;
    impl__AfxThrowInvalidArgException__YAXXZ();
    return false;
}

// Deviation (3): the CComboBox inlines as the ::SendMessage retail issues.
inline int ComboGetCount(const void* pThis) {                          // CB_GETCOUNT, result taken as int
    return static_cast<int>(::SendMessage(HWndOf(pThis), CB_GETCOUNT, 0, 0));
}
inline LRESULT ComboGetItemData(const void* pThis, int nIndex) {       // CB_GETITEMDATA, index sign-extended (movslq)
    return ::SendMessage(HWndOf(pThis), CB_GETITEMDATA, static_cast<WPARAM>(static_cast<INT_PTR>(nIndex)), 0);
}
inline void ComboSetCurSel(const void* pThis, int nIndex) {            // CB_SETCURSEL, index sign-extended
    ::SendMessage(HWndOf(pThis), CB_SETCURSEL, static_cast<WPARAM>(static_cast<INT_PTR>(nIndex)), 0);
}

// `delete pFontInfo` as retail compiles it for a CObject-derived object:
//     mov (%rax),%rcx; mov $1,%edx; mov 0x8(%rcx),%rax; call <dispatch>
// i.e. vftable slot 1 (the MSVC scalar deleting destructor) with flags 1.
// The object's vftable is whatever its (client) builder installed.
using ScalarDeletingDtorFn = void* (MS_ABI*)(void* pThis, unsigned int flags);
inline void DeleteFontInfo(void* pInfo) {
    void* const* vtbl = *static_cast<void* const* const*>(pInfo);
    reinterpret_cast<ScalarDeletingDtorFn>(vtbl[1])(pInfo, 1);
}

// CStringT::Compare's ATLENSURE(AtlIsValidString(psz)) as retail inlines it
// in SelectFont(CMFCFontInfo*): a NULL buffer pointer -> AtlThrow(E_FAIL)
// (0x333c, which for anything but E_OUTOFMEMORY calls
// ?AfxThrowOleException@@YAXJ@Z, 0x25f2c0).  A constructed CString never has a
// NULL buffer, so this only fires on a corrupt object.  Returns false when
// the (OpenMFC) throw helper returns instead of throwing.
inline bool EnsureValidString(const wchar_t* psz) {
    if (psz != nullptr) return true;
    impl__AfxThrowOleException__YAXJ_Z(E_FAIL);
    return false;
}

inline int GlobalDataInt(int off) {
    int v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}
inline void* GlobalDataPtr(int off) {
    void* v;
    std::memcpy(&v, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof v);
    return v;
}
// The retail one-time gate: `if (afxGlobalData.<+0> == 0) { Initialize(); <+0> = 1; }`
// (?Initialize@AFX_GLOBAL_DATA@@QEAAXXZ, 0x6a790).
inline void EnsureGlobalDataInitialized() {
    if (GlobalDataInt(kGlobalDataInitGate) == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}

// Field-for-field mirror of detail/CWinAppSupport.h's `struct _AFX_THREAD_STATE`
// (int nTempMapLock; int nWndCreateLock; void* pModuleState; void* pWndInit;
// void* hHookOldCbtFilter;), used only to read pWndInit in PreSubclassWindow --
// the same mirror controls/CMFCListCtrl.cpp uses.  (Retail reads
// _AFX_THREAD_STATE::m_pWndInit at +0x28; OpenMFC's struct keeps it at +0x10.)
struct AfxThreadStateMirror {
    int nTempMapLock;
    int nWndCreateLock;
    void* pModuleState;
    void* pWndInit;
    void* hHookOldCbtFilter;
};
static_assert(sizeof(AfxThreadStateMirror) == 32, "AfxThreadStateMirror: five-field _AFX_THREAD_STATE");
static_assert(offsetof(AfxThreadStateMirror, pWndInit) == 16, "AfxThreadStateMirror::pWndInit");

} // namespace

// ===========================================================================
// Construction / destruction
// ===========================================================================

// Symbol: ??0CMFCFontComboBox@@QEAA@XZ
// Transcribed from retail entry RVA 0x60ef0 (mfc140u):
//     CWnd::CWnd();                                   // 0x28a700 (CComboBox's ctor is inline)
//     vfptr = 0x1802e8b98;                            // deviation (1): OpenMFC's CWnd vptr kept, recorded
//     m_Images: vfptr = 0x180337b38; m_hImageList = NULL;   // deviation (2): no vptr
//     m_bToolBarMode = FALSE;                         // movl $0,0xf8
//     return this;
extern "C" void* MS_ABI impl___0CMFCFontComboBox__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return pThis;
    impl___0CWnd__QEAA_XZ(pThis);
    if (g_ownVptr == nullptr) g_ownVptr = *static_cast<void**>(pThis);
    FontComboLayout* s = L(pThis);
    std::memset(s->m_Images, 0, sizeof(s->m_Images));
    impl___0CImageList__QEAA_XZ(Images(pThis));
    s->m_bToolBarMode = FALSE;
    return pThis;
}
// Symbol: ??1CMFCFontComboBox@@UEAA@XZ
// Transcribed from retail entry RVA 0x60f90 (mfc140u):
//     vfptr = 0x1802e8b98;                            // deviation (1): g_ownVptr stored instead
//     CImageList::~CImageList(&m_Images);             // 0x296be0 on this+0xe8
//     CComboBox::~CComboBox();                        // 0x2941a0 (tail jump)
// The previous definition had no `this` parameter at all; the signature now
// follows the mangled name.
extern "C" void MS_ABI impl___1CMFCFontComboBox__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    if (g_ownVptr != nullptr) *static_cast<void**>(pThis) = g_ownVptr;
    impl___1CImageList__UEAA_XZ(Images(pThis));
    impl___1CComboBox__UEAA_XZ(pThis);
}

// ===========================================================================
// Item bookkeeping
// ===========================================================================

// Symbol: ?CleanUp@CMFCFontComboBox@@IEAAXXZ
// Transcribed from retail entry RVA 0x61780 (mfc140u):
//     ENSURE(::IsWindow(m_hWnd));                     // else 0x227720 AfxThrowInvalidArgException
//     if (m_bToolBarMode) return;                     // cmpl $0,0xf8; jne -> return
//     for (int i = 0; i < GetCount(); i++) {          // CB_GETCOUNT re-sent every iteration
//         CMFCFontInfo* pDesc = (CMFCFontInfo*)GetItemDataPtr(i);   // CB_GETITEMDATA
//         if (pDesc != NULL) delete pDesc;            // vftable slot 1, flags 1
//     }
//     ResetContent();                                 // CB_RESETCONTENT
// Added guard: an item-data value of CB_ERR ((void*)-1) is not deleted (retail
// would dispatch through it; CB_GETITEMDATA returns CB_ERR only on failure).
extern "C" void MS_ABI impl__CleanUp_CMFCFontComboBox__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    if (!EnsureIsWindow(pThis)) return;
    if (L(pThis)->m_bToolBarMode) return;
    for (int i = 0; i < ComboGetCount(pThis); i++) {
        const LRESULT data = ComboGetItemData(pThis, i);
        if (data != 0 && data != CB_ERR) {
            DeleteFontInfo(reinterpret_cast<void*>(data));
        }
    }
    ::SendMessage(HWndOf(pThis), CB_RESETCONTENT, 0, 0);
}

// Symbol: ?CompareItem@CMFCFontComboBox@@UEAAHPEAUtagCOMPAREITEMSTRUCT@@@Z
// Transcribed from retail entry RVA 0x611b0 (mfc140u):
//     CString strItem1;  GetLBText((WORD)lpCIS->itemID1, strItem1);   // movzwl 0x10; 0x294250
//     CString strItem2;  GetLBText((WORD)lpCIS->itemID2, strItem2);   // movzwl 0x20; 0x294250
//     return strItem1.Collate(strItem2);             // wcscoll(strItem1, strItem2), IAT 0x1802c7738
// The item ids are truncated to 16 bits exactly as retail's movzwl loads them.
// Added guard: a NULL lpCIS returns 0.
extern "C" int MS_ABI impl__CompareItem_CMFCFontComboBox__UEAAHPEAUtagCOMPAREITEMSTRUCT___Z(
    void* pThis, COMPAREITEMSTRUCT* lpCIS) {
    if (pThis == nullptr || lpCIS == nullptr) return 0;
    const CComboBox* pCombo = static_cast<const CComboBox*>(W(pThis));
    CString strItem1;
    impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pCombo, static_cast<int>(static_cast<WORD>(lpCIS->itemID1)), &strItem1);
    CString strItem2;
    impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pCombo, static_cast<int>(static_cast<WORD>(lpCIS->itemID2)), &strItem2);
    return std::wcscoll(strItem1.GetString(), strItem2.GetString());
}

// Symbol: ?DeleteString@CMFCFontComboBox@@QEAAHI@Z
// Transcribed from retail entry RVA 0x61120 (mfc140u):
//     if (GetItemDataPtr(nIndex) != NULL)                         // CB_GETITEMDATA, (int)nIndex sign-extended
//         delete (CMFCFontInfo*)GetItemDataPtr(nIndex);           // sent again; vftable slot 1, flags 1
//     return CComboBox::DeleteString(nIndex);                     // tail-jump: SendMessage(CB_DELETESTRING,
//                                                                 //   nIndex zero-extended, 0), result as int
// Note there is NO m_bToolBarMode test here (unlike CleanUp).  Added guard: an
// item-data value of CB_ERR ((void*)-1, what CB_GETITEMDATA returns for an
// out-of-range index) is not deleted; retail would dispatch through it.
extern "C" int MS_ABI impl__DeleteString_CMFCFontComboBox__QEAAHI_Z(void* pThis, unsigned int nIndex) {
    if (pThis == nullptr) return CB_ERR;
    const int nItem = static_cast<int>(nIndex);
    if (ComboGetItemData(pThis, nItem) != 0) {
        const LRESULT data = ComboGetItemData(pThis, nItem);
        if (data != 0 && data != CB_ERR) {
            DeleteFontInfo(reinterpret_cast<void*>(data));
        }
    }
    return static_cast<int>(::SendMessage(HWndOf(pThis), CB_DELETESTRING, static_cast<WPARAM>(nIndex), 0));
}

// Symbol: ?DrawItem@CMFCFontComboBox@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
// Transcribed from retail entry RVA 0x612a0 (mfc140u):
//     if (m_Images.GetSafeHandle() == NULL)
//         m_Images.Create(0x4268, 16, 0, RGB(255, 255, 255));    // ?Create@CImageList@@QEAAHIHHK@Z, 0x296d60
//     CDC* pDC = CDC::FromHandle(lpDIS->hDC);                     // 0x2a2450
//     CRect rc;  ::CopyRect(&rc, &lpDIS->rcItem);                 // IAT 0x1802c7218
//     if (lpDIS->itemState & ODS_FOCUS) pDC->DrawFocusRect(rc);   // ::DrawFocusRect(m_hDC), IAT 0x1802c71d8
//     int nOldDC = pDC->SaveDC();                                 // CDC vslot +0x48 (0x2a2600: ::SaveDC)
//     CBrush br;
//     if (lpDIS->itemState & ODS_SELECTED) {
//         br.Attach(::CreateSolidBrush(afxGlobalData.clrHilite));        // +0x48 (gate first)
//         pDC->SetTextColor(afxGlobalData.clrTextHilite);                // vslot +0x70 (0x2a2960), +0x4c (gate again)
//     } else {
//         br.Attach(::CreateSolidBrush(::GetBkColor(pDC->m_hAttribDC)));  // IAT 0x1802c61b8
//     }
//     pDC->SetBkMode(TRANSPARENT);                                // 0x2a2860, edx = 1
//     ::FillRect(pDC->m_hDC, &rc, br);                            // IAT 0x1802c7208
//     int id = (int)lpDIS->itemID;
//     if (id >= 0) {
//         CFont fontSelected;  CFont* pOldFont = NULL;
//         CMFCFontInfo* pDesc = (CMFCFontInfo*)lpDIS->itemData;
//         if (pDesc != NULL) {
//             if (pDesc->m_nType & (DEVICE_FONTTYPE | TRUETYPE_FONTTYPE)) {     // test $6
//                 CPoint ptImage(rc.left, rc.top + (rc.Height() - 16) / 2);
//                 m_Images.Draw(pDC, (pDesc->m_nType & DEVICE_FONTTYPE) ? 0 : 1, ptImage, ILD_NORMAL);
//                     // 0x60d64: activation-context wrapper that resolves comctl32's
//                     // "ImageList_Draw" (name at 0x18033f350) and calls it
//             }
//             rc.left += 22;                                        // addl $0x16
//             if (m_bDrawUsingFont && pDesc->m_nCharSet != SYMBOL_CHARSET) {   // static 0x1803be210; cmpb $2
//                 LOGFONT lf;  ::GetObjectW(afxGlobalData.fontRegular, sizeof(LOGFONT), &lf);   // HFONT at +0x1b0
//                 ::lstrcpyW(lf.lfFaceName, pDesc->m_strName);
//                 if (pDesc->m_nCharSet != DEFAULT_CHARSET) lf.lfCharSet = pDesc->m_nCharSet;
//                 lf.lfHeight = lf.lfHeight < 0 ? lf.lfHeight - 4 : lf.lfHeight + 4;
//                 fontSelected.Attach(::CreateFontIndirectW(&lf));
//                 pOldFont = pDC->SelectObject(&fontSelected);      // vslot +0x60 (0x2a2730: ::SelectObject)
//             }
//         }
//         CString strText;  GetLBText(id, strText);               // 0x294250
//         pDC->DrawText(strText, strText.GetLength(), &rc, DT_SINGLELINE | DT_VCENTER);   // vslot +0xe0 (0x1e6ce0: ::DrawTextW), 0x24
//         if (pOldFont != NULL) pDC->SelectObject(pOldFont);
//     }                                                           // ~CFont -> CGdiObject::DeleteObject (0x2a3f60)
//     pDC->RestoreDC(nOldDC);                                     // vslot +0x50 (0x2a2650: ::RestoreDC)
//                                                                 // ~CBrush -> CGdiObject::DeleteObject
// Deviations:
//  * The CDC calls are made as the Win32 calls they reach on lpDIS->hDC.  The
//    CDC is the temporary one CDC::FromHandle creates for that HDC, so its
//    m_hDC and m_hAttribDC are both lpDIS->hDC and every virtual slot named
//    above is CDC's own (its body, read at the address given, issues exactly
//    that GDI call on m_hDC / m_hAttribDC).  The CBrush / CFont temporaries are
//    plain HBRUSH / HFONT handles deleted where their destructors run.
//  * ImageList_Draw is called directly, without the activation-context
//    wrapper.
//  * m_Images.Create goes through OpenMFC's CImageList::Create(UINT, ...)
//    (core/gdi/CImageList.cpp), which loads from AfxGetInstanceHandle()
//    only.  Retail's Create (0x296d60) first calls
//    ?AfxFindResourceHandle@@YAPEAUHINSTANCE__@@PEB_W0@Z (0x2aeb50) with
//    (MAKEINTRESOURCE(0x4268), RT_BITMAP), i.e. it searches the module
//    state's resource chain, which can reach the bitmap MFC itself ships.
//    Where the application carries no bitmap 0x4268, m_Images stays empty
//    here and the font-type glyph is not drawn; the item text still is.
//  * lf is zeroed before ::GetObjectW (retail leaves it uninitialised, which
//    only matters when GetObjectW fails).
//  * Added guard: a NULL lpDIS returns; a NULL name buffer in the item's
//    CMFCFontInfo skips the font creation (a constructed CString never has one).
extern "C" void MS_ABI impl__DrawItem_CMFCFontComboBox__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDIS) {
    if (pThis == nullptr || lpDIS == nullptr) return;
    CImageList* pImages = Images(pThis);
    if (pImages->m_hImageList == nullptr) {
        impl__Create_CImageList__QEAAHIHHK_Z(pImages, kFontTypeBitmapId, kFontTypeImageSize, 0, RGB(255, 255, 255));
    }
    HDC hDC = lpDIS->hDC;
    RECT rc;
    ::CopyRect(&rc, &lpDIS->rcItem);
    if (lpDIS->itemState & ODS_FOCUS) {
        ::DrawFocusRect(hDC, &rc);
    }
    const int nOldDC = ::SaveDC(hDC);

    HBRUSH hbr = nullptr;
    if (lpDIS->itemState & ODS_SELECTED) {
        EnsureGlobalDataInitialized();
        hbr = ::CreateSolidBrush(static_cast<COLORREF>(GlobalDataInt(kGlobalDataClrHilite)));
        EnsureGlobalDataInitialized();
        ::SetTextColor(hDC, static_cast<COLORREF>(GlobalDataInt(kGlobalDataClrTextHilite)));
    } else {
        hbr = ::CreateSolidBrush(::GetBkColor(hDC));
    }
    ::SetBkMode(hDC, TRANSPARENT);
    ::FillRect(hDC, &rc, hbr);

    const int id = static_cast<int>(lpDIS->itemID);
    if (id >= 0) {
        HFONT hFontSelected = nullptr;
        HGDIOBJ hOldFont = nullptr;
        const FontInfoLayout* pDesc = reinterpret_cast<const FontInfoLayout*>(lpDIS->itemData);
        if (pDesc != nullptr) {
            if (pDesc->m_nType & (DEVICE_FONTTYPE | TRUETYPE_FONTTYPE)) {
                const int y = rc.top + (rc.bottom - rc.top - kFontTypeImageSize) / 2;
                const int nImage = (pDesc->m_nType & DEVICE_FONTTYPE) ? 0 : 1;
                ::ImageList_Draw(pImages->m_hImageList, nImage, hDC, rc.left, y, ILD_NORMAL);
            }
            rc.left += kTextIndent;
            if (impl__m_bDrawUsingFont_CMFCFontComboBox__2HA != 0 && pDesc->m_nCharSet != SYMBOL_CHARSET &&
                pDesc->m_strName != nullptr) {
                EnsureGlobalDataInitialized();
                LOGFONTW lf;
                std::memset(&lf, 0, sizeof lf);
                ::GetObjectW(static_cast<HGDIOBJ>(GlobalDataPtr(kGlobalDataFontRegularHandle)), sizeof(LOGFONTW), &lf);
                ::lstrcpyW(lf.lfFaceName, pDesc->m_strName);
                if (pDesc->m_nCharSet != DEFAULT_CHARSET) lf.lfCharSet = pDesc->m_nCharSet;
                lf.lfHeight = lf.lfHeight < 0 ? lf.lfHeight - 4 : lf.lfHeight + 4;
                hFontSelected = ::CreateFontIndirectW(&lf);
                hOldFont = ::SelectObject(hDC, hFontSelected);
            }
        }
        CString strText;
        impl__GetLBText_CComboBox__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
            static_cast<const CComboBox*>(W(pThis)), id, &strText);
        ::DrawTextW(hDC, strText.GetString(), strText.GetLength(), &rc, DT_SINGLELINE | DT_VCENTER);
        if (hOldFont != nullptr) ::SelectObject(hDC, hOldFont);
        if (hFontSelected != nullptr) ::DeleteObject(hFontSelected);
    }
    ::RestoreDC(hDC, nOldDC);
    if (hbr != nullptr) ::DeleteObject(hbr);
}

// Symbol: ?GetSelFont@CMFCFontComboBox@@QEBAPEAVCMFCFontInfo@@XZ
// Transcribed from retail entry RVA 0x61c30 (mfc140u):
//     ENSURE(::IsWindow(m_hWnd));                     // else 0x227720 AfxThrowInvalidArgException
//     int iIndex = GetCurSel();                       // CB_GETCURSEL, result taken as int
//     if (iIndex < 0) return NULL;
//     return (CMFCFontInfo*)GetItemDataPtr(iIndex);   // tail-jump SendMessage(CB_GETITEMDATA, sext iIndex, 0)
extern "C" void* MS_ABI impl__GetSelFont_CMFCFontComboBox__QEBAPEAVCMFCFontInfo__XZ(const void* pThis) {
    if (pThis == nullptr) return nullptr;
    if (!EnsureIsWindow(pThis)) return nullptr;
    const int iIndex = static_cast<int>(::SendMessage(HWndOf(pThis), CB_GETCURSEL, 0, 0));
    if (iIndex < 0) return nullptr;
    return reinterpret_cast<void*>(ComboGetItemData(pThis, iIndex));
}

// Symbol: ?Init@CMFCFontComboBox@@IEAAXXZ
// Transcribed from retail entry RVA 0x61730 (mfc140u):
//     m_bToolBarMode = GetParent()->IsKindOf(RUNTIME_CLASS(CMFCToolBar));
//         // CWnd::FromHandle(::GetParent(m_hWnd)) (0x28ad70), CObject::IsKindOf (0x234cf0)
//         // against the static descriptor at 0x1803b15f8 ("CMFCToolBar")
//     if (!m_bToolBarMode)
//         Setup(DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE,   // lea 0x7(%rax),%edx
//               DEFAULT_CHARSET, DEFAULT_PITCH);                         // r8b = 1, r9d = 0; 0x61840
// The descriptor is obtained through OpenMFC's GetThisClass getter.  With no
// parent, retail does not fault: its IsKindOf (0x234cf0) begins with
// ENSURE(this != NULL) (`test %rcx,%rcx; je` -> 0x227720
// AfxThrowInvalidArgException), so retail Init throws CInvalidArgException.
// OpenMFC's IsKindOf thunk (core/runtime/CObject.cpp) returns FALSE for a
// NULL object instead, so here m_bToolBarMode becomes FALSE and Setup runs.
// That difference belongs to the IsKindOf thunk, not to this body.
extern "C" void MS_ABI impl__Init_CMFCFontComboBox__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
    L(pThis)->m_bToolBarMode = impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        pParent, impl__GetThisClass_CMFCToolBar__SAPEAUCRuntimeClass__XZ());
    if (!L(pThis)->m_bToolBarMode) {
        impl__Setup_CMFCFontComboBox__QEAAHHEE_Z(pThis, DEVICE_FONTTYPE | RASTER_FONTTYPE | TRUETYPE_FONTTYPE,
                                                 DEFAULT_CHARSET, DEFAULT_PITCH);
    }
}

// Symbol: ?MeasureItem@CMFCFontComboBox@@UEAAXPEAUtagMEASUREITEMSTRUCT@@@Z
// Transcribed from retail entry RVA 0x61620 (mfc140u):
//     CRect rc;  GetWindowRect(&rc);                  // zero-initialised, then ::GetWindowRect(m_hWnd)
//     lpMIS->itemWidth = rc.Width();                  // right - left -> +0xc
//     int nTextHeight = afxGlobalData.GetTextHeight();         // gate + read of +0x2a8
//     int nHeight = max(nTextHeight, CMFCToolBarFontComboBox::m_nFontHeight);
//                                                     // static at 0x1803be380; signed `jle`
//     lpMIS->itemHeight = max(nHeight, 16);           // signed compare, cmovl -> +0x10
// (The source evidently evaluates GetTextHeight() twice, through a max
// macro: the compiled body keeps a second gate-and-reload of +0x2a8, but it
// is reached only when the gate value held in %ecx is still 0, which cannot
// happen after the first gate -- on every reachable path the first value is
// used.)  Added guard: a NULL lpMIS returns.
extern "C" void MS_ABI impl__MeasureItem_CMFCFontComboBox__UEAAXPEAUtagMEASUREITEMSTRUCT___Z(
    void* pThis, MEASUREITEMSTRUCT* lpMIS) {
    if (pThis == nullptr || lpMIS == nullptr) return;
    RECT rc = {0, 0, 0, 0};
    ::GetWindowRect(HWndOf(pThis), &rc);
    lpMIS->itemWidth = static_cast<UINT>(rc.right - rc.left);
    EnsureGlobalDataInitialized();
    const int nTextHeight = GlobalDataInt(kGlobalDataTextHeightHorz);
    const int nHeight = std::max(nTextHeight, static_cast<int>(impl__m_nFontHeight_CMFCToolBarFontComboBox__2HA));
    lpMIS->itemHeight = static_cast<UINT>(std::max(nHeight, kMinItemHeight));
}

// ===========================================================================
// Message handlers
// ===========================================================================

// Symbol: ?OnCreate@CMFCFontComboBox@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from retail entry RVA 0x61700 (mfc140u):
//     if (CComboBox::OnCreate(lpCreateStruct) == -1)  // inline CWnd::OnCreate == CWnd::Default() (0x28ac80)
//         return -1;                                  // 32-bit compare against -1
//     Init();                                         // 0x61730
//     return 0;
extern "C" int MS_ABI impl__OnCreate_CMFCFontComboBox__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (pThis == nullptr) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(W(pThis))) == -1) return -1;
    impl__Init_CMFCFontComboBox__IEAAXXZ(pThis);
    return 0;
}

// Symbol: ?OnDestroy@CMFCFontComboBox@@IEAAXXZ
// Transcribed from retail entry RVA 0x61a20 (mfc140u):
//     CleanUp();                                      // 0x61780
//     CComboBox::OnDestroy();                         // tail-jump ?OnDestroy@CWnd@@IEAAXXZ (0x28b840)
extern "C" void MS_ABI impl__OnDestroy_CMFCFontComboBox__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    impl__CleanUp_CMFCFontComboBox__IEAAXXZ(pThis);
    impl__OnDestroy_CWnd__IEAAXXZ(W(pThis));
}

// Symbol: ?OnInitControl@CMFCFontComboBox@@IEAA_J_K_J@Z
// Transcribed from retail entry RVA 0x61c90 (mfc140u; message-map entry for
// message 0x37c).  wParam is the byte count and lParam the UTF-8 init string;
// the four keys are UTF-16 literals in the image (0x18033f360, 0x18033f398,
// 0x18033f3e0, 0x18033f420):
//     CString strDst;  CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                                                              // 0x13d840
//     BOOL bDrawUsingFont = TRUE;
//     if (ReadBoolProp(tagManager, L"MFCComboBox_DrawUsingFont", bDrawUsingFont))                 // 0x1409a0
//         m_bDrawUsingFont = bDrawUsingFont;                                                       // static 0x1803be210
//     BOOL bShowTrueType = TRUE;  ReadBoolProp(tagManager, L"MFCComboBox_ShowTrueTypeFonts", bShowTrueType);
//     BOOL bShowRaster   = TRUE;  ReadBoolProp(tagManager, L"MFCComboBox_ShowRasterTypeFonts", bShowRaster);
//     BOOL bShowDevice   = TRUE;  ReadBoolProp(tagManager, L"MFCComboBox_ShowDeviceTypeFonts", bShowDevice);
//     int nFontType = 0;
//     if (bShowTrueType) nFontType  = TRUETYPE_FONTTYPE;    // 4 (cmovne)
//     if (bShowRaster)   nFontType |= RASTER_FONTTYPE;      // 1
//     if (bShowDevice)   nFontType |= DEVICE_FONTTYPE;      // 2
//     Setup(nFontType, DEFAULT_CHARSET, DEFAULT_PITCH);     // 0x61840, r8b = 1, r9d = 0 -- unconditional
//     return 0;                                             // ~CTagManager (0x13d8f0), ~CString
// CTagManager exists in this DLL only as its impl__ thunks over a side table
// keyed by the object address (controls/CTagManager.cpp), so it is built in
// raw storage and torn down through the matching thunk, as
// controls/CMFCMaskedEdit.cpp does.  (Setup is a stub today -- see there.)
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCFontComboBox__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (pThis == nullptr) return 0;
    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));

    alignas(void*) unsigned char tagStorage[sizeof(CTagManager)] = {};
    void* pTagManager = tagStorage;
    impl___0CTagManager__QEAA_PEB_W_Z(pTagManager, strDst.GetString());

    int bDrawUsingFont = TRUE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCComboBox_DrawUsingFont", &bDrawUsingFont)) {
        impl__m_bDrawUsingFont_CMFCFontComboBox__2HA = bDrawUsingFont;
    }
    int bShowTrueType = TRUE;
    impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCComboBox_ShowTrueTypeFonts", &bShowTrueType);
    int bShowRaster = TRUE;
    impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCComboBox_ShowRasterTypeFonts", &bShowRaster);
    int bShowDevice = TRUE;
    impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCComboBox_ShowDeviceTypeFonts", &bShowDevice);

    int nFontType = 0;
    if (bShowTrueType) nFontType = TRUETYPE_FONTTYPE;
    if (bShowRaster) nFontType |= RASTER_FONTTYPE;
    if (bShowDevice) nFontType |= DEVICE_FONTTYPE;
    impl__Setup_CMFCFontComboBox__QEAAHHEE_Z(pThis, nFontType, DEFAULT_CHARSET, DEFAULT_PITCH);

    impl___1CTagManager__UEAA_XZ(pTagManager);
    return 0;
}

// Symbol: ?PreSubclassWindow@CMFCFontComboBox@@MEAAXXZ
// Transcribed from retail entry RVA 0x616d0 (mfc140u):
//     _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();   // 0x1332a0
//     if (pThreadState->m_pWndInit == NULL) Init();            // +0x28; 0x61730
// (No call to the CWnd/CComboBox PreSubclassWindow survives in the retail
// body.)  OpenMFC keeps the same member as _AFX_THREAD_STATE::pWndInit (read
// through AfxThreadStateMirror above).  Retail dereferences the thread state
// unconditionally; a NULL thread state is treated as "no window being
// created", the choice controls/CMFCListCtrl.cpp makes.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCFontComboBox__MEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    const AfxThreadStateMirror* pThreadState =
        reinterpret_cast<const AfxThreadStateMirror*>(impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ());
    if (pThreadState == nullptr || pThreadState->pWndInit == nullptr) {
        impl__Init_CMFCFontComboBox__IEAAXXZ(pThis);
    }
}

// Symbol: ?PreTranslateMessage@CMFCFontComboBox@@UEAAHPEAUtagMSG@@@Z
// Transcribed from retail entry RVA 0x60fd0 (mfc140u):
//     if (m_bToolBarMode && pMsg->message == WM_KEYDOWN &&
//         !CMFCToolBarComboBoxButton::m_bFlat) {                         // static 0x1803b1b74
//         CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));        // 0x28ad70
//         switch (pMsg->wParam) {
//         case VK_TAB:
//             if (pParent != NULL)
//                 CWnd::FromHandle(::GetNextDlgTabItem(pParent->m_hWnd, m_hWnd, FALSE))->SetFocus();  // 0x2a9b60
//             return TRUE;                              // also when pParent == NULL
//         case VK_ESCAPE:
//             if (AFXGetTopLevelFrame(this) != NULL)    // g_pTopLevelFrame (0x1803be1b8), else
//                 AFXGetTopLevelFrame(this)->SetFocus();//   GetTopLevelFrame() (0x28e490) -- evaluated twice
//             return TRUE;                              // also when there is no frame
//         case VK_UP: case VK_DOWN:                     // (wParam - 0x26) & ~2 == 0
//             if (::GetKeyState(VK_MENU) >= 0 && ::GetKeyState(VK_CONTROL) >= 0 &&
//                 !GetDroppedState()) {                 // CB_GETDROPPEDSTATE
//                 ShowDropDown();                       // CB_SHOWDROPDOWN, TRUE
//                 return TRUE;
//             }
//             break;
//         }
//     }
//     return CComboBox::PreTranslateMessage(pMsg);     // direct call to 0x28bc00, the body in slot 69 of
//                                                      // CWnd's vftable (0x1803371b8, stored by ??0CWnd@@ 0x28a700)
// Added guards: a NULL window from GetNextDlgTabItem, or a NULL frame on the
// second evaluation, skips the SetFocus call (retail would fault); a NULL
// pMsg goes straight to the base.
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCFontComboBox__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr) return FALSE;
    if (pMsg != nullptr && L(pThis)->m_bToolBarMode && pMsg->message == WM_KEYDOWN &&
        impl__m_bFlat_CMFCToolBarComboBoxButton__1HA == 0) {
        CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
        switch (pMsg->wParam) {
        case VK_TAB:
            if (pParent != nullptr) {
                CWnd* pNext = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
                    ::GetNextDlgTabItem(pParent->m_hWnd, HWndOf(pThis), FALSE));
                if (pNext != nullptr) impl__SetFocus_CWnd__QEAAPEAV1_XZ(pNext);
            }
            return TRUE;
        case VK_ESCAPE: {
            CWnd* pFrame = static_cast<CWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
            if (pFrame == nullptr) pFrame = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(W(pThis));
            if (pFrame != nullptr) {
                CWnd* pFocus = static_cast<CWnd*>(impl__g_pTopLevelFrame__3PEAVCFrameWnd__EA);
                if (pFocus == nullptr) pFocus = impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(W(pThis));
                if (pFocus != nullptr) impl__SetFocus_CWnd__QEAAPEAV1_XZ(pFocus);
            }
            return TRUE;
        }
        case VK_UP:
        case VK_DOWN:
            if (::GetKeyState(VK_MENU) >= 0 && ::GetKeyState(VK_CONTROL) >= 0 &&
                static_cast<int>(::SendMessage(HWndOf(pThis), CB_GETDROPPEDSTATE, 0, 0)) == 0) {   // `test %eax,%eax`
                ::SendMessage(HWndOf(pThis), CB_SHOWDROPDOWN, TRUE, 0);
                return TRUE;
            }
            break;
        default:
            break;
        }
    }
    return impl__PreTranslateMessage_CWnd__UEAAHPEAUtagMSG___Z(W(pThis), pMsg);
}

// ===========================================================================
// Operations
// ===========================================================================

// Symbol: ?SelectFont@CMFCFontComboBox@@QEAAHPEAVCMFCFontInfo@@@Z
// Transcribed from retail entry RVA 0x61a40 (mfc140u):
//     ENSURE(::IsWindow(m_hWnd));                     // else 0x227720 AfxThrowInvalidArgException
//     for (int i = 0; i < GetCount(); i++) {          // CB_GETCOUNT re-sent every iteration
//         CMFCFontInfo* pFontInfo = (CMFCFontInfo*)GetItemDataPtr(i);
//         if (*pDesc == *pFontInfo) {                 // CMFCFontInfo::operator== inlined:
//             //   pDesc->m_strName   == pFontInfo->m_strName    (ATLENSURE item +0x08 != NULL, wcscmp)
//             //   pDesc->m_strScript == pFontInfo->m_strScript  (ATLENSURE item +0x10 != NULL, wcscmp)
//             //   m_nCharSet (+0x18), m_nPitchAndFamily (+0x19), m_nType (+0x1c) equal
//             SetCurSel(i);                           // CB_SETCURSEL
//             return TRUE;
//         }
//     }
//     return FALSE;
// Added guards: a NULL pDesc returns FALSE and an item whose data is NULL or
// CB_ERR is skipped (retail dereferences both unconditionally).
extern "C" int MS_ABI impl__SelectFont_CMFCFontComboBox__QEAAHPEAVCMFCFontInfo___Z(void* pThis, void* pDescArg) {
    if (pThis == nullptr) return FALSE;
    if (!EnsureIsWindow(pThis)) return FALSE;
    const FontInfoLayout* pDesc = static_cast<const FontInfoLayout*>(pDescArg);
    if (pDesc == nullptr) return FALSE;
    for (int i = 0; i < ComboGetCount(pThis); i++) {
        const LRESULT data = ComboGetItemData(pThis, i);
        if (data == 0 || data == CB_ERR) continue;
        const FontInfoLayout* pFontInfo = reinterpret_cast<const FontInfoLayout*>(data);
        if (!EnsureValidString(pFontInfo->m_strName)) return FALSE;
        if (std::wcscmp(pDesc->m_strName, pFontInfo->m_strName) != 0) continue;
        if (!EnsureValidString(pFontInfo->m_strScript)) return FALSE;
        if (std::wcscmp(pDesc->m_strScript, pFontInfo->m_strScript) != 0) continue;
        if (pDesc->m_nCharSet != pFontInfo->m_nCharSet) continue;
        if (pDesc->m_nPitchAndFamily != pFontInfo->m_nPitchAndFamily) continue;
        if (pDesc->m_nType != pFontInfo->m_nType) continue;
        ComboSetCurSel(pThis, i);
        return TRUE;
    }
    return FALSE;
}

// Symbol: ?SelectFont@CMFCFontComboBox@@QEAAHPEB_WE@Z
// Transcribed from retail entry RVA 0x61b50 (mfc140u):
//     ENSURE(::IsWindow(m_hWnd) && lpszName != NULL); // else 0x227720 AfxThrowInvalidArgException
//     for (int i = 0; i < GetCount(); i++) {          // CB_GETCOUNT re-sent every iteration
//         CMFCFontInfo* pFontInfo = (CMFCFontInfo*)GetItemDataPtr(i);
//         if (pFontInfo->m_strName == lpszName &&     // wcscmp(item +0x08, lpszName) == 0
//             (nCharSet == DEFAULT_CHARSET || pFontInfo->m_nCharSet == nCharSet)) {   // +0x18
//             SetCurSel(i);                           // CB_SETCURSEL
//             return TRUE;
//         }
//     }
//     return FALSE;
// Added guard: an item whose data is NULL or CB_ERR, or whose name buffer is
// NULL, is skipped (retail dereferences them unconditionally).
extern "C" int MS_ABI impl__SelectFont_CMFCFontComboBox__QEAAHPEB_WE_Z(
    void* pThis, const wchar_t* lpszName, unsigned char nCharSet) {
    if (pThis == nullptr) return FALSE;
    if (!::IsWindow(HWndOf(pThis)) || lpszName == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    for (int i = 0; i < ComboGetCount(pThis); i++) {
        const LRESULT data = ComboGetItemData(pThis, i);
        if (data == 0 || data == CB_ERR) continue;
        const FontInfoLayout* pFontInfo = reinterpret_cast<const FontInfoLayout*>(data);
        if (pFontInfo->m_strName == nullptr) continue;
        if (std::wcscmp(pFontInfo->m_strName, lpszName) != 0) continue;
        if (nCharSet == DEFAULT_CHARSET || pFontInfo->m_nCharSet == nCharSet) {
            ComboSetCurSel(pThis, i);
            return TRUE;
        }
    }
    return FALSE;
}

// Symbol: ?Setup@CMFCFontComboBox@@QEAAHHEE@Z
// STUB -- returns FALSE without touching the control.  Retail (entry RVA
// 0x61840, mfc140u) does:
//     ENSURE(::IsWindow(m_hWnd));                     // else 0x227720
//     if (m_bToolBarMode) return FALSE;
//     CleanUp();                                      // 0x61780
//     CMFCToolBarFontComboBox combo(0, -1, nFontType, nCharSet, 2, 0, nPitchAndFamily);
//         // on the stack; ??0CMFCToolBarFontComboBox@@QEAA@IHHEKHE@Z (0x1690c0),
//         // the constructor that enumerates the installed fonts
//     for (int i = 0; i < <INT_PTR count at +0xf8 of that object>; i++) {
//         CString strFont = combo.GetItem(i);         // ?GetItem@CMFCToolBarComboBoxButton (0x162a00)
//         CMFCFontInfo* pDesc = (CMFCFontInfo*)combo.GetItemData(i);   // 0x162a80
//         if (SendMessage(CB_FINDSTRINGEXACT, -1, strFont) <= 0) {     // sic: `jg` skips only results > 0
//             CMFCFontInfo* pCopy = new CMFCFontInfo(*pDesc);          // operator new(0x20), vftable 0x1802e91a8
//             SetItemDataPtr(AddString(strFont), pCopy);               // CB_ADDSTRING, CB_SETITEMDATA
//         }
//     }
//     return TRUE;                                    // ~CMFCToolBarFontComboBox (0x1691b0)
// Not reproduced because OpenMFC's ??0CMFCToolBarFontComboBox@@QEAA@IHHEKHE@Z
// (featurepack/toolbar/CMFCToolBarFontComboBox.cpp) returns pThis without
// constructing or enumerating anything, and that file's enumeration exports
// (RebuildFonts, AddFont, the EnumFam*CallBackEx callbacks) are empty stubs,
// so there is no font list to copy from; and OpenMFC has no MSVC-layout
// CMFCFontInfo vftable to install in the copies, which clients would later
// delete through slot 1 (see DeleteFontInfo).  Doing only the ENSURE /
// CleanUp prefix would still return a different result than retail, so the
// body stays empty.  Init and OnInitControl call it as retail does.
extern "C" int MS_ABI impl__Setup_CMFCFontComboBox__QEAAHHEE_Z(
    void* pThis, int nFontType, unsigned char nCharSet, unsigned char nPitchAndFamily) {
    (void)pThis;
    (void)nFontType;
    (void)nCharSet;
    (void)nPitchAndFamily;
    return FALSE;
}
