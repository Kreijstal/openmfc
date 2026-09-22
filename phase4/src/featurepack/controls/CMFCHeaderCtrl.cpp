// CMFCHeaderCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include <windows.h>
#include <commctrl.h>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>

#include "detail/CMemDCSupport.h"   // S_Cmemdc / CDC_S views + the CMemDC thunk declarations (windows.h only)

#ifndef MS_ABI
#  if defined(__GNUC__) || defined(__clang__)
#    define MS_ABI __attribute__((ms_abi))
#  else
#    define MS_ABI
#  endif
#endif

struct CPlex;   // opaque here; the node layout lives in detail/CPlexSupport.h

// ===========================================================================
// CMFCHeaderCtrl -- the feature-pack owner-drawn header control
// (afxheaderctrl.h: CMFCHeaderCtrl : CHeaderCtrl : CWnd).
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp).  Each
// body names the entry RVA it was read from; every RVA in this file is an
// mfc140u address (read with `disas.py --u`, resolved through the export
// ordinal table with ures.py), never an mfc140 (ANSI twin) address.  Every
// IAT slot named below was resolved with iatu.py against mfc140u.
//
// How this class is modelled here.  include/openmfc/afxmfc.h:1464 declares
// CMFCHeaderCtrl only as `CWnd + char _pad[16]` (sizeof 248 == 0xf8), which
// is UNDERSIZED: the retail object is 0x140 bytes.  Client (MSVC) code and
// the retail-layout classes that embed a CMFCHeaderCtrl by value
// (CMFCListCtrl::m_wndHeader at +0xe8, CMFCPropertyGridCtrl::m_wndHeader at
// +0x208) allocate the retail size, so every export takes a `void* pThis`
// and reads the members through the file-local HeaderCtrlLayout view
// pinned below.  That view is the only place this layout lives; it is NOT
// a public-header edit (reported in headerRequests).
//
// This translation unit deliberately includes NO OpenMFC class header
// (openmfc/afxwin.h, afxmfc.h, detail/ManualSmallStubImplementationsSupport.h),
// unlike its siblings in featurepack/controls/.  Merely including
// openmfc/afxwin.h emits the CWnd / CWinThread / CWinApp / CCmdTarget
// vtables into every TU and with them nine undefined C++ references
// (AfxGetThread, CWnd::FromHandle, the four classXxx descriptors, sized
// operator delete and two libsupc++ type_info vtables -- the identical set
// build-phase4/obj/.../CMFCShellListCtrl.o carries).  They are all defined
// in the tree, but this file's committed baseline was a bare generated
// fallback, so the campaign's link audit (checkfile.sh, which diffs the
// undefined-symbol set against that baseline) would flag them as new.  The
// self-contained style of detail/CMemDCSupport.h is used instead: every
// object this file touches is read through a retail-layout view whose
// offsets the disassembly itself cites, and every thunk is declared with
// `void*` for the class-typed parameters (C linkage; the ABI is identical).
// A `RECT` passed by value is the same 16-byte aggregate as a `CRect` by
// value -- passed by pointer to a caller-made copy under the MS x64
// convention -- so the CRect-by-value exports are declared with RECT here.
// The three OpenMFC-object offsets this relies on -- CDC::m_hDC == +8,
// CGdiObject::m_hObject == +8, CImageList::m_hImageList == +8 -- were
// verified against include/openmfc/afxwin.h with a scratch static_assert TU
// on 2026-09-22 and coincide with the retail offsets the disassembly reads
// (0x6e626 / 0x6f0c2 / 0x6e511); CWnd::m_hWnd == +0x40 both in OpenMFC and
// in retail (every body below that needs the HWND -- OnPaint, OnMouseMove,
// CommonInit, OnSetFont, the four Redraw sites -- reads 0x40(%rcx); the
// purely map-driven ones such as GetColumnState never touch it).
//
// Retail object layout, read from the constructor (entry RVA 0x6e200) and
// the destructor (entry RVA 0x6e2e0), cross-checked against the shipping
// afxheaderctrl.h on this host (atlmfc/include/afxheaderctrl.h:48-55):
//
//   +0x000  CWnd base, 0xe8 bytes (m_hWnd at +0x40); the ctor calls
//           ??0CWnd@@QEAA@XZ (0x28a700) and stores the CMFCHeaderCtrl
//           vftable 0x1802ea548 at +0.
//   +0x0e8  CMap<int,int,int,int> m_mapColumnsStatus, 0x38 bytes:
//           +0xe8  vfptr            (ctor stores the CMap vftable 0x1802ea3c8)
//           +0xf0  m_pHashTable     (ctor stores 0)
//           +0xf8  m_nHashTableSize (ctor stores 0x11 == 17)
//           +0x100 m_nCount         (ctor stores 0)
//           +0x108 m_pFreeList      (ctor stores 0)
//           +0x110 m_pBlocks        (ctor stores 0)
//           +0x118 m_nBlockSize     (ctor stores 0xa == 10)
//   +0x120  BOOL  m_bIsMousePressed  (ctor stores 0; the 8-byte store at
//                                     0x6e259 zeroes +0x120 and +0x124)
//   +0x124  BOOL  m_bMultipleSort    (ctor stores 0)
//   +0x128  BOOL  m_bAscending       (ctor stores 1; the 8-byte store at
//                                     0x6e270 writes 1 to +0x128 and 0 to +0x12c)
//   +0x12c  BOOL  m_bTracked         (ctor stores 0)
//   +0x130  BOOL  m_bIsDlgControl    (ctor stores 0)
//   +0x134  int   m_nHighlightedItem (ctor stores -1)
//   +0x138  HFONT m_hFont            (ctor stores 0)
//   sizeof == 0x140  (m_nObjectSize of the CRuntimeClass at 0x1802eab48 in
//                     mfc140u, which ?GetRuntimeClass@CMFCHeaderCtrl@@
//                     (entry 0x6e1f0) returns; read with rtcu.py)
//
// The retail destructor (0x6e2e0) reinstalls both vftables, calls the shared
// out-of-line CMap::RemoveAll body (0x1ba40) on m_mapColumnsStatus (the call
// is at 0x6e304, not inlined) and tail-jumps to
// ??1CHeaderCtrl@@UEAA@XZ (0x2963d0).
//
// Retail CMFCHeaderCtrl vftable (0x1802ea548, mfc140u; slots read with
// uvt.py).  The slots the bodies below dispatch through are:
//   slot 94 (+0x2f0)  OnDrawItem(CDC*, int, CRect, BOOL, BOOL)    (0x6e330)
//   slot 95 (+0x2f8)  OnFillBackground(CDC*)                     (0x6eb90)
//   slot 96 (+0x300)  OnDrawSortArrow(CDC*, CRect)               (0x6ecd0)
// Retail CMFCVisualManager vftable (0x18031c128, mfc140u):
//   slot 87 (+0x2b8)  OnFillHeaderCtrlBackground                 (0x188c40)
//   slot 88 (+0x2c0)  OnDrawHeaderCtrlBorder                     (0x188cc0)
//   slot 89 (+0x2c8)  OnDrawHeaderCtrlSortArrow                  (0x188e40)
// Retail CDC vftable (the CMetaFileDC instance at 0x180324ff8 shares the
// CDC slot layout; read with uvt.py):
//   slot 11 (+0x58)   SelectStockObject(int)
//   slot 12 (+0x60)   SelectObject(CFont*)
//   slot 14 (+0x70)   SetTextColor(COLORREF)
//   slot 28 (+0xe0)   DrawTextW(LPCWSTR, int, LPRECT, UINT)
//
// Deviations from retail, applied uniformly and repeated at each site:
//
//  (1) Virtual dispatch on `this` is devirtualized.  OpenMFC models no
//      MSVC-layout vftable for this class, so every retail `this->vslot`
//      call above (OnDrawItem / OnFillBackground / OnDrawSortArrow) is made
//      as a direct call to this file's own thunk; a client class overriding
//      one of those virtuals is not honoured from inside the DLL.  This is
//      the convention throughout featurepack/ (see CMFCShellListCtrl.cpp).
//
//  (2) Virtual dispatch on the CDC is devirtualized the same way: the CDC
//      slots named above are called through the exported CDC thunks
//      (core/gdi/CDC.cpp) and the DLL-internal DrawTextW helper
//      (detail/GdicoreSupport.cpp), as featurepack/controls/CMFCStatusBar.cpp
//      does.  Non-virtual CDC/CWnd inline wrappers that retail expands to a
//      raw USER32/GDI32 call (GetClientRect, RedrawWindow, InvalidateRect,
//      UpdateWindow, GetItemCount/GetItem/GetItemRect/GetImageList/HitTest
//      = SendMessage, GetClipBox) are written as the same Win32 call on
//      m_hWnd / m_hDC, exactly as the retail code does.  SelectClipRgn is
//      NOT one of those: retail calls the out-of-line CDC::SelectClipRgn
//      (0x2a2e20), reproduced here by the DcSelectClipRgn helper below.  The
//      CPaintDC retail builds on OnPaint's stack (ctor 0x2a3d20: zero the
//      CDC, m_hWnd = pWnd->m_hWnd, ::BeginPaint(m_hWnd, &m_ps), CDC::Attach;
//      dtor 0x2a3dd0: ::EndPaint, CDC::Detach, ~CDC) is a retail-layout
//      PaintDcView driven with the same BeginPaint / EndPaint pair (IAT
//      0x1802c7060 / 0x1802c7068); the CMemDC is the S_Cmemdc view driven
//      through its thunks, as featurepack/docking/CPaneDivider.cpp does.
//
//  (3) CMFCVisualManager::GetInstance().  Retail inlines it as the helper
//      at 0x9774: read ?m_pVisManager@CMFCVisualManager@@ (0x3be3c0) and
//      lazily create the default manager when it is NULL.  The creating
//      half exists in this tree only as the C++ static
//      CMFCVisualManager::GetInstance()
//      (featurepack/visualmanager/CMFCVisualManager.cpp),
//      which BRIEFING S1 forbids calling from here, so -- as
//      featurepack/docking/CPaneDivider.cpp::OnPaint does -- the exported
//      pointer is read and the visual-manager call is skipped while it is
//      NULL.  Two of the three manager calls -- OnDrawHeaderCtrlBorder and
//      OnDrawHeaderCtrlSortArrow -- go through the export thunks in
//      featurepack/visualmanager/Thunks.cpp, whose bodies are
//      `pThis->OnDrawHeaderCtrlXxx(...)`, i.e. a C++ virtual call, so a
//      derived OpenMFC manager's override IS honoured there.  The third,
//      OnFillHeaderCtrlBackground, is NOT: its export
//      (featurepack/visualmanager/CMFCVisualManager.cpp:1540) is the base
//      class's concrete body, not a dispatching thunk, so calling it always
//      runs CMFCVisualManager's own fill even when a derived manager is
//      installed.  Retail dispatches all three through vftable slots 87/88/89.
//
//  (4) afxGlobalData.  ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A (0x3c1620) is a
//      zero-filled 720-byte block in OpenMFC whose Initialize thunk is empty.
//      Retail reads clrBtnText at +0x34 and &fontRegular (a CFont at +0x1a8,
//      m_hObject at +0x1b0) after the inlined "if (!m_bInitialized) {
//      Initialize(); m_bInitialized = 1; }" gate (+0).  The gate is
//      reproduced; the colour falls back to ::GetSysColor(COLOR_BTNTEXT)
//      (the value retail's initialiser stores there) and a NULL font handle
//      to the stock DEFAULT_GUI_FONT, the convention of CMFCStatusBar.cpp.
//
//  (5) The CRgn retail builds on the stack in OnPaint (CRgn vftable
//      0x1802e29e0, CreateRectRgnIndirect -> CGdiObject::Attach ->
//      CDC::SelectClipRgn -> ~CRgn) is kept as a raw HRGN: OpenMFC's CRgn
//      ctor is out-of-line C++ with no impl__ thunk.  The clip itself is NOT
//      a plain ::SelectClipRgn(m_hDC, hRgn): the retail CDC::SelectClipRgn
//      body (0x2a2e20) clips m_hDC only when m_hDC != m_hAttribDC and then
//      clips m_hAttribDC when that handle is non-NULL.  DcSelectClipRgn
//      below reproduces exactly that pair of tests.
//
//  (6) The CString retail builds around HDITEM::pszText in OnDrawItem
//      (ctor 0xdcb0, GetLength, release) only serves to pass the text and
//      its length to DrawText; the local buffer and wcslen are passed
//      instead.  No observable difference.
//
//  (7) The ImageList_GetIconSize / ImageList_Draw calls retail makes through
//      its comctl32 delay-load wrappers (0x6f160 / 0x60d64) are made as
//      direct comctl32 calls.
//
// KNOWN GAPS outside this file (all reported as headerRequests):
//   * ??0CMFCHeaderCtrl@@QEAA@XZ / ??1CMFCHeaderCtrl@@UEAA@XZ live in
//     featurepack/controls/CtorDtorPlacement.cpp as placement-new / dtor of
//     the 248-byte header class, so +0xe8..+0x140 (the CMap and the flags)
//     are never initialised by the OpenMFC constructor and the map is never
//     released by the destructor.  OpenMFC_CMFCHeaderCtrl_ConstructRetailMembers
//     / _DestructRetailMembers below are the retail ctor/dtor member stores
//     with C linkage, ready for that file to call.
//   * The class message map (detail/Mfc04MsgmapSupport.cpp:17) is empty, so
//     OpenMFC's message routing never reaches the handlers below.  Retail's
//     map (0x1802ea3f0, msgmap_u.py) has, in order: WM_ERASEBKGND, WM_PAINT,
//     WM_LBUTTONDOWN, WM_LBUTTONUP, WM_MOUSEMOVE, WM_CANCELMODE, WM_CREATE,
//     WM_MOUSELEAVE, WM_SETFONT, base map = CWnd::GetMessageMap.
// ===========================================================================

// ---- external thunks (definitions verified in the named files) -------------
// Class-typed parameters are declared `void*` / `const void*` (C linkage, no
// mangling; see the header note).  Signatures follow the mangled names.
extern "C" void* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                 // core/window/CWnd.cpp:602
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const void* pThis, const void* pClass);                                                        // core/runtime/CObject.cpp:49
extern "C" void* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ();                    // core/dialog/CDialog.cpp:89
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(void* pThis);                              // core/window/Thunks.cpp:1183
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const void* pThis);                   // core/window/Thunks.cpp:1378
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__UEAAKK_Z(void* pDC, unsigned long clr);    // core/gdi/CDC.cpp:224
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(void* pDC, int nBkMode);                      // core/gdi/CDC.cpp:80
extern "C" void* MS_ABI impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(void* pDC, void* pFont);   // core/gdi/CDC.cpp:444
extern "C" void* MS_ABI impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(void* pDC, int nIndex);// core/gdi/CDC.cpp:473
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    void* pDC, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat);        // detail/GdicoreSupport.cpp:140
extern "C" void* MS_ABI impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(HGDIOBJ hObject);             // core/gdi/CGdiObject.cpp:16
extern "C" void* MS_ABI impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(HIMAGELIST hIL);  // core/gdi/CImageList.cpp:111
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);                     // core/runtime/AFX_GLOBAL_DATA.cpp:98
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                          // featurepack/CMFC_misc_stubs.cpp:3652
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                                // core/runtime/StaticData.cpp:56
extern "C" void MS_ABI impl__OnFillHeaderCtrlBackground_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__VCRect___Z(
    void* pVM, void* pCtrl, void* pDC, RECT rect);                                                 // featurepack/visualmanager/CMFCVisualManager.cpp:1540 (CRect by value)
extern "C" void MS_ABI impl__OnDrawHeaderCtrlBorder_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
    void* pVM, void* pCtrl, void* pDC, RECT* pRect, int bIsPressed, int bIsHighlighted);            // featurepack/visualmanager/Thunks.cpp:1217
extern "C" void MS_ABI impl__OnDrawHeaderCtrlSortArrow_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__H_Z(
    void* pVM, void* pCtrl, void* pDC, RECT* pRect, int bIsAscending);                             // featurepack/visualmanager/Thunks.cpp:1222
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                    // detail/MemcoreSupport.cpp:5
extern "C" void MS_ABI impl___3_YAXPEAX_Z(void* ptr);                                             // detail/MemcoreSupport.cpp:12
extern "C" CPlex* MS_ABI impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(
    CPlex** ppHead, unsigned __int64 nMax, unsigned __int64 cbElement);                            // core/collections/CPlex.cpp:13
extern "C" void MS_ABI impl__FreeDataChain_CPlex__QEAAXXZ(CPlex* self);                           // core/collections/CPlex.cpp:43
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                // detail/MfcExceptionsSupport.cpp:34

// Exports of this class defined below and called by sibling bodies (retail
// reaches OnDrawItem / OnFillBackground / OnDrawSortArrow through vftable
// slots 94 / 95 / 96 -- deviation (1); CommonInit and SelectFont are
// non-virtual direct calls in retail too).
extern "C" void MS_ABI impl__CommonInit_CMFCHeaderCtrl__IEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__SelectFont_CMFCHeaderCtrl__IEAAPEAVCFont__PEAVCDC___Z(void* pThis, void* pDC);
extern "C" void MS_ABI impl__OnDrawItem_CMFCHeaderCtrl__MEAAXPEAVCDC__HVCRect__HH_Z(
    void* pThis, void* pDC, int iItem, RECT rect, int bIsPressed, int bIsHighlighted);
extern "C" void MS_ABI impl__OnDrawSortArrow_CMFCHeaderCtrl__MEAAXPEAVCDC__VCRect___Z(void* pThis, void* pDC, RECT rectArrow);
extern "C" void MS_ABI impl__OnFillBackground_CMFCHeaderCtrl__MEAAXPEAVCDC___Z(void* pThis, void* pDC);

namespace {

// ---------------------------------------------------------------------------
// Retail layout (mfc140u).  Offsets: ctor 0x6e200 / dtor 0x6e2e0 (see header).
// ---------------------------------------------------------------------------

// CMap<int,int,int,int>::CAssoc -- afxtempl.h's
// `CAssoc : CPair { KEY key; VALUE value; } { CAssoc* pNext; UINT nHashValue; }`.
// Read out of the inlined GetAssocAt (0x236e0): key compared at +0
// (`cmp %ebp,(%rax)` at 0x23743), nHashValue at +0x10 (0x2373e), pNext at +8
// (0x23747); Lookup (0x2f610) reads the value at +4 (0x2f62d); the SetAt
// instantiation (entry 0x2346c) hands 0x18 to CPlex::Create as the element
// size (`mov $0x18,%r8d` at 0x234fc inside it).
struct ColAssoc {
    int       key;          // +0x00
    int       value;        // +0x04
    ColAssoc* pNext;        // +0x08
    UINT      nHashValue;   // +0x10
    UINT      pad_14;
};
static_assert(sizeof(ColAssoc) == 0x18, "retail CAssoc<int,int> is 24 bytes");

// The CMap itself (ctor stores at 0x6e218..0x6e24e: vfptr, 0, 17, 0, 0, 0, 10).
struct ColMap {
    void*      vfptr;            // +0x00  retail 0x1802ea3c8 (mfc140u)
    ColAssoc** m_pHashTable;     // +0x08
    UINT       m_nHashTableSize; // +0x10  17
    UINT       pad_14;
    INT_PTR    m_nCount;         // +0x18
    ColAssoc*  m_pFreeList;      // +0x20
    CPlex*     m_pBlocks;        // +0x28
    INT_PTR    m_nBlockSize;     // +0x30  10
};
static_assert(sizeof(ColMap) == 0x38, "retail CMap is 56 bytes");

// File-local layout view of the retail object (offsets cited in the header).
struct HeaderCtrlLayout {
    unsigned char base[0xe8];        // CWnd (m_hWnd at +0x40)
    ColMap        m_mapColumnsStatus;// +0x0e8
    BOOL          m_bIsMousePressed; // +0x120
    BOOL          m_bMultipleSort;   // +0x124
    BOOL          m_bAscending;      // +0x128
    BOOL          m_bTracked;        // +0x12c
    BOOL          m_bIsDlgControl;   // +0x130
    int           m_nHighlightedItem;// +0x134
    HFONT         m_hFont;           // +0x138
};
static_assert(offsetof(HeaderCtrlLayout, m_mapColumnsStatus) == 0xe8, "CMFCHeaderCtrl::m_mapColumnsStatus");
static_assert(offsetof(HeaderCtrlLayout, m_bIsMousePressed) == 0x120, "CMFCHeaderCtrl::m_bIsMousePressed");
static_assert(offsetof(HeaderCtrlLayout, m_bMultipleSort) == 0x124, "CMFCHeaderCtrl::m_bMultipleSort");
static_assert(offsetof(HeaderCtrlLayout, m_bAscending) == 0x128, "CMFCHeaderCtrl::m_bAscending");
static_assert(offsetof(HeaderCtrlLayout, m_bTracked) == 0x12c, "CMFCHeaderCtrl::m_bTracked");
static_assert(offsetof(HeaderCtrlLayout, m_bIsDlgControl) == 0x130, "CMFCHeaderCtrl::m_bIsDlgControl");
static_assert(offsetof(HeaderCtrlLayout, m_nHighlightedItem) == 0x134, "CMFCHeaderCtrl::m_nHighlightedItem");
static_assert(offsetof(HeaderCtrlLayout, m_hFont) == 0x138, "CMFCHeaderCtrl::m_hFont");
static_assert(sizeof(HeaderCtrlLayout) == 0x140, "CMFCHeaderCtrl: retail sizeof 0x140 (CRuntimeClass m_nObjectSize)");

constexpr std::size_t kHwndOffset = 0x40;   // CWnd::m_hWnd (the retail ctor/handlers read 0x40(%rcx))

inline HeaderCtrlLayout* L(void* pThis) { return static_cast<HeaderCtrlLayout*>(pThis); }
inline const HeaderCtrlLayout* L(const void* pThis) { return static_cast<const HeaderCtrlLayout*>(pThis); }
inline HWND HwndOf(const void* pThis) {
    HWND h = nullptr;
    std::memcpy(&h, static_cast<const unsigned char*>(pThis) + kHwndOffset, sizeof h);
    return h;
}

// Retail-layout views of the objects handed in or out through the thunks.
// CGdiObject: vfptr + m_hObject at +8 (OnSetFont reads 0x8(%rdx) of the
// CFont at 0x6f0c2; OnDrawItem reads the CBitmap's as 0x8(%rax) at 0x6e587
// and 0x8(%rdi) at 0x6e61d).
struct GdiObjView { void* vfptr; HGDIOBJ m_hObject; };
static_assert(offsetof(GdiObjView, m_hObject) == 8, "CGdiObject::m_hObject");
// CImageList: vfptr + m_hImageList at +8 (OnDrawItem reads 0x8(%rax) at 0x6e511).
struct ImageListView { void* vfptr; HIMAGELIST m_hImageList; };
static_assert(offsetof(ImageListView, m_hImageList) == 8, "CImageList::m_hImageList");
// CDC: CDC_S from detail/CMemDCSupport.h (vfptr, m_hDC +8, m_hAttribDC +16,
// m_bPrinting +24; OnDrawItem reads pDC->m_hDC as 0x8(%rsi) at 0x6e539/0x6e626).
static_assert(offsetof(CDC_S, m_hDC) == 8 && sizeof(CDC_S) == 32, "CDC layout");
inline HDC DcHandle(const void* pDC) { return pDC != nullptr ? static_cast<const CDC_S*>(pDC)->m_hDC : nullptr; }  // CDC::GetSafeHdc
// CDC::SelectClipRgn(CRgn*), transcribed from retail entry RVA 0x2a2e20
// (mfc140u).  It is NOT a single ::SelectClipRgn on m_hDC:
//     if (m_hDC != m_hAttribDC) ::SelectClipRgn(m_hDC,       pRgn ? pRgn->m_hObject : NULL);
//     if (m_hAttribDC != NULL)  ::SelectClipRgn(m_hAttribDC, pRgn ? pRgn->m_hObject : NULL);
// (IAT 0x1802c6428 = GDI32!SelectClipRgn.)  Deviation (5): the region is a
// raw HRGN here rather than a CRgn, so the handle is passed straight in.
inline void DcSelectClipRgn(void* pDC, HRGN hRgn) {
    if (pDC == nullptr) return;
    CDC_S* p = static_cast<CDC_S*>(pDC);
    if (p->m_hDC != p->m_hAttribDC) ::SelectClipRgn(p->m_hDC, hRgn);
    if (p->m_hAttribDC != nullptr) ::SelectClipRgn(p->m_hAttribDC, hRgn);
}
// CPaintDC as the retail ctor (0x2a3d20) lays it out: CDC, m_hWnd at +0x20,
// PAINTSTRUCT m_ps at +0x28; sizeof 0x70 (the deleting dtor at 0x2a3d80
// frees 0x70 bytes).
struct PaintDcView { CDC_S dc; HWND m_hWnd; PAINTSTRUCT m_ps; };
static_assert(offsetof(PaintDcView, m_hWnd) == 0x20 && offsetof(PaintDcView, m_ps) == 0x28 && sizeof(PaintDcView) == 0x70,
              "retail CPaintDC layout");

// CWnd::RedrawWindow() with its default arguments, as every retail site in
// this class expands it: ::RedrawWindow(m_hWnd, NULL, NULL, 0x105) where
// 0x105 == RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE (IAT 0x1802c7130 =
// USER32!RedrawWindow).  Retail performs no m_hWnd test at those sites
// (EnableMultipleSort is the one exception and tests it itself).
inline void RedrawSelf(void* pThis) {
    ::RedrawWindow(HwndOf(pThis), nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE);
}

// CPoint passed by value is an 8-byte aggregate in a register under MS_ABI.
inline int PtX(long long pt) { return static_cast<int>(static_cast<unsigned long long>(pt) & 0xffffffffull); }
inline int PtY(long long pt) { return static_cast<int>(static_cast<unsigned long long>(pt) >> 32); }

// The afxGlobalData init gate, exactly as every retail reader inlines it
// (e.g. 0x6e924..0x6e939 inside OnPaint):
//     if (afxGlobalData.m_bInitialized == 0) { afxGlobalData.Initialize(); m_bInitialized = 1; }
// m_bInitialized is the first member (afxglobals.h:70).  Deviation (4).
inline void EnsureGlobalDataInitialized() {
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, &one, sizeof one);
    }
}
// afxGlobalData.clrBtnText (+0x34; OnPaint reads it at 0x6e940).  Deviation (4):
// the block is unpopulated in OpenMFC, so the live system colour retail's
// initialiser would have stored there is used.
inline COLORREF GD_clrBtnText() { return ::GetSysColor(COLOR_BTNTEXT); }
// &afxGlobalData.fontRegular (CFont at +0x1a8, m_hObject at +0x1b0;
// SelectFont takes its address at 0x6ec85).  Deviation (4): a NULL handle
// falls back to the stock GUI font.
inline HFONT GD_fontRegular() {
    HFONT h = nullptr;
    std::memcpy(&h, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x1b0, sizeof h);
    if (h == nullptr) h = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    return h;
}

// The visual manager singleton -- deviation (3).
inline void* VisualManager() { return impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA; }

// ---------------------------------------------------------------------------
// m_mapColumnsStatus: the afxtempl.h CMap<int,int,int,int> code retail
// inlines / instantiates, over the ColMap / ColAssoc layout above.
// ---------------------------------------------------------------------------

// HashKey<int> (afxtempl.h:165-173), as compiled into GetAssocAt (0x236e0)
// and inlined into RemoveSortColumn (0x6e7a2..0x6e7cc): ldiv(key, 127773)
// (IAT 0x1802c7868 = api-ms-win-crt-utility!ldiv), rem = 16807*rem -
// 2836*quot, += 2147483647 if negative.
inline UINT ColHashKey(int key) {
    const std::ldiv_t d = std::ldiv(static_cast<long>(key), 127773L);
    long rem = 16807L * d.rem - 2836L * d.quot;
    if (rem < 0) rem += 2147483647L;
    return static_cast<UINT>(rem);
}

// CMap::GetAssocAt (0x236e0): hash, bucket = hash % m_nHashTableSize, then
// walk the bucket chain comparing nHashValue and key.
ColAssoc* ColGetAssocAt(const ColMap& m, int key, UINT& nHash, UINT& nBucket) {
    nHash = ColHashKey(key);
    nBucket = m.m_nHashTableSize ? nHash % m.m_nHashTableSize : 0;   // retail divides unconditionally
    if (m.m_pHashTable == nullptr) return nullptr;
    for (ColAssoc* p = m.m_pHashTable[nBucket]; p != nullptr; p = p->pNext) {
        if (p->nHashValue == nHash && p->key == key) return p;
    }
    return nullptr;
}

// CMap::Lookup (0x2f610): GetAssocAt, then rValue = assoc->value (+4).
bool ColLookup(const ColMap& m, int key, int& rValue) {
    UINT nHash, nBucket;
    ColAssoc* p = ColGetAssocAt(m, key, nHash, nBucket);
    if (p == nullptr) return false;
    rValue = p->value;
    return true;
}

// CMap::RemoveAll (0x1ba40, the body the dtor / EnableMultipleSort /
// SetSortColumn / RemoveSortColumn call): free the bucket array
// (IAT 0x1802c74e8 = free), zero it, zero m_nCount / m_pFreeList, free every
// CPlex block (the inlined CPlex::FreeDataChain at 0x1ba7d..0x1ba8c), zero
// m_pBlocks.  Here the bucket array is released through the exported
// operator delete and the blocks through the exported CPlex::FreeDataChain,
// the deallocators matching the allocators ColSetAt uses.
void ColRemoveAll(ColMap& m) {
    if (m.m_pHashTable != nullptr) {
        impl___3_YAXPEAX_Z(m.m_pHashTable);
        m.m_pHashTable = nullptr;
    }
    m.m_nCount = 0;
    m.m_pFreeList = nullptr;
    if (m.m_pBlocks != nullptr) impl__FreeDataChain_CPlex__QEAAXXZ(m.m_pBlocks);
    m.m_pBlocks = nullptr;
}

// CMap::SetAt as instantiated at entry 0x2346c: GetAssocAt; if missing,
// InitHashTable(m_nHashTableSize) (operator new of size*8 at 0x234bb, memset
// 0) when there is no table yet, NewAssoc (CPlex::Create(m_pBlocks,
// m_nBlockSize, 0x18) at 0x23502, chained into the free list), memset the
// assoc to 0, m_nCount++, key / nHashValue stored, linked at the bucket head
// (0x23567..0x2357d); then assoc->value = value (0x23586).  Both allocation
// failures reach AfxThrowInvalidArgException (0x23597).
void ColSetAt(ColMap& m, int key, int value) {
    UINT nHash, nBucket;
    ColAssoc* p = ColGetAssocAt(m, key, nHash, nBucket);
    if (p == nullptr) {
        if (m.m_pHashTable == nullptr) {
            if (m.m_nHashTableSize == 0) m.m_nHashTableSize = 17;   // guard only; retail assumes the ctor's 17
            const std::size_t bytes = static_cast<std::size_t>(m.m_nHashTableSize) * sizeof(ColAssoc*);
            m.m_pHashTable = static_cast<ColAssoc**>(impl___2_YAPEAX_K_Z(bytes));
            if (m.m_pHashTable == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
            std::memset(m.m_pHashTable, 0, bytes);
            nBucket = nHash % m.m_nHashTableSize;
        }
        if (m.m_pFreeList == nullptr) {
            CPlex* pBlock = impl__Create_CPlex__SAPEAU1_AEAPEAU1__K1_Z(&m.m_pBlocks,
                                                                     static_cast<unsigned __int64>(m.m_nBlockSize),
                                                                     sizeof(ColAssoc));
            if (pBlock == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
            // CPlex::data() == this + 1: the element block follows the 8-byte
            // header (detail/CPlexSupport.h); retail's NewAssoc addresses the
            // last assoc as plex + 8 + 0x18 * (m_nBlockSize - 1) (0x2350b..0x23513).
            ColAssoc* pAssoc = reinterpret_cast<ColAssoc*>(reinterpret_cast<unsigned char*>(pBlock) + 8) + m.m_nBlockSize - 1;
            for (INT_PTR i = m.m_nBlockSize - 1; i >= 0; i--, pAssoc--) {
                pAssoc->pNext = m.m_pFreeList;
                m.m_pFreeList = pAssoc;
            }
        }
        p = m.m_pFreeList;
        if (p == nullptr) { impl__AfxThrowInvalidArgException__YAXXZ(); return; }
        m.m_pFreeList = p->pNext;
        std::memset(static_cast<void*>(p), 0, sizeof(ColAssoc));
        m.m_nCount++;
        p->key = key;
        p->nHashValue = nHash;
        p->pNext = m.m_pHashTable[nBucket];
        m.m_pHashTable[nBucket] = p;
    }
    p->value = value;
}

// CMap::RemoveKey as inlined in RemoveSortColumn (0x6e798..0x6e817): nothing
// to do without a bucket array; otherwise find the assoc in its bucket,
// unlink it, push it on the free list, m_nCount--, and RemoveAll once the map
// is empty.
bool ColRemoveKey(ColMap& m, int key) {
    if (m.m_pHashTable == nullptr) return false;
    const UINT nHash = ColHashKey(key);
    ColAssoc** ppPrev = &m.m_pHashTable[nHash % m.m_nHashTableSize];
    for (ColAssoc* p = *ppPrev; p != nullptr; ppPrev = &p->pNext, p = p->pNext) {
        if (p->nHashValue == nHash && p->key == key) {
            *ppPrev = p->pNext;
            p->pNext = m.m_pFreeList;
            m.m_pFreeList = p;
            m.m_nCount--;
            if (m.m_nCount == 0) ColRemoveAll(m);
            return true;
        }
    }
    return false;
}

} // namespace

// ---------------------------------------------------------------------------
// Retail ctor / dtor member stores with C linkage, for CtorDtorPlacement.cpp
// (the owner of ??0CMFCHeaderCtrl@@ / ??1CMFCHeaderCtrl@@) to call -- see the
// KNOWN GAPS note in the header.  Construct: the stores at 0x6e218..0x6e27b
// (the CWnd part and the vftable at +0 are NOT touched here).  Destruct: the
// dtor's CMap::RemoveAll call at 0x6e304.
// ---------------------------------------------------------------------------
extern "C" void OpenMFC_CMFCHeaderCtrl_ConstructRetailMembers(void* pThis) {
    if (pThis == nullptr) return;
    HeaderCtrlLayout* d = L(pThis);
    d->m_mapColumnsStatus.vfptr            = nullptr;   // retail: CMap vftable 0x1802ea3c8 (not modelled here)
    d->m_mapColumnsStatus.m_pHashTable     = nullptr;
    d->m_mapColumnsStatus.m_nHashTableSize = 17;
    d->m_mapColumnsStatus.pad_14           = 0;
    d->m_mapColumnsStatus.m_nCount         = 0;
    d->m_mapColumnsStatus.m_pFreeList      = nullptr;
    d->m_mapColumnsStatus.m_pBlocks        = nullptr;
    d->m_mapColumnsStatus.m_nBlockSize     = 10;
    d->m_bIsMousePressed  = FALSE;
    d->m_bMultipleSort    = FALSE;
    d->m_bAscending       = TRUE;
    d->m_bTracked         = FALSE;
    d->m_bIsDlgControl    = FALSE;
    d->m_nHighlightedItem = -1;
    d->m_hFont            = nullptr;
}
extern "C" void OpenMFC_CMFCHeaderCtrl_DestructRetailMembers(void* pThis) {
    if (pThis == nullptr) return;
    ColRemoveAll(L(pThis)->m_mapColumnsStatus);
}

// ---------------------------------------------------------------------------
// Attributes
// ---------------------------------------------------------------------------

// Symbol: ?GetSortColumn@CMFCHeaderCtrl@@QEBAHXZ
// Transcribed from retail entry RVA 0x6ed70 (mfc140u):
//     if (m_bMultipleSort) return -1;                          // cmpl $0,0x124(%rcx); jne
//     int nCount = (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT (0x1200), 0, 0);
//     for (int i = 0; i < nCount; i++) {                       // entered only when nCount > 0
//         int nState = 0;
//         if (m_mapColumnsStatus.Lookup(i, nState) && nState != 0) return i;
//     }
//     return -1;
// IAT 0x1802c7120 = USER32!SendMessageW.
extern "C" int MS_ABI impl__GetSortColumn_CMFCHeaderCtrl__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return -1;   // deviation: retail has no NULL check
    const HeaderCtrlLayout* d = L(pThis);
    if (d->m_bMultipleSort != 0) return -1;
    const int nCount = static_cast<int>(::SendMessage(HwndOf(pThis), HDM_GETITEMCOUNT, 0, 0));
    for (int i = 0; i < nCount; i++) {
        int nState = 0;
        if (ColLookup(d->m_mapColumnsStatus, i, nState) && nState != 0) return i;
    }
    return -1;
}

// Symbol: ?IsAscending@CMFCHeaderCtrl@@QEBAHXZ
// Transcribed from retail entry RVA 0x6edf0 (mfc140u); same shape as
// GetSortColumn but returns the state's sign:
//     if (m_bMultipleSort) return FALSE;
//     int nCount = (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0);
//     for (int i = 0; i < nCount; i++) {
//         int nState = 0;
//         if (m_mapColumnsStatus.Lookup(i, nState) && nState != 0) return nState > 0;   // setg
//     }
//     return FALSE;
extern "C" int MS_ABI impl__IsAscending_CMFCHeaderCtrl__QEBAHXZ(const void* pThis) {
    if (pThis == nullptr) return FALSE;   // deviation: retail has no NULL check
    const HeaderCtrlLayout* d = L(pThis);
    if (d->m_bMultipleSort != 0) return FALSE;
    const int nCount = static_cast<int>(::SendMessage(HwndOf(pThis), HDM_GETITEMCOUNT, 0, 0));
    for (int i = 0; i < nCount; i++) {
        int nState = 0;
        if (ColLookup(d->m_mapColumnsStatus, i, nState) && nState != 0) return nState > 0 ? TRUE : FALSE;
    }
    return FALSE;
}

// Symbol: ?GetColumnState@CMFCHeaderCtrl@@QEBAHH@Z
// Transcribed from retail entry RVA 0x6ee80 (mfc140u):
//     int nState = 0;
//     m_mapColumnsStatus.Lookup(iColumn, nState);    // 0x2f610; the result is ignored
//     return nState;                                 // 0 / -1 / 1
extern "C" int MS_ABI impl__GetColumnState_CMFCHeaderCtrl__QEBAHH_Z(const void* pThis, int iColumn) {
    if (pThis == nullptr) return 0;   // deviation: retail has no NULL check
    int nState = 0;
    ColLookup(L(pThis)->m_mapColumnsStatus, iColumn, nState);
    return nState;
}

// ---------------------------------------------------------------------------
// Operations
// ---------------------------------------------------------------------------

// Symbol: ?SetSortColumn@CMFCHeaderCtrl@@QEAAXHHH@Z
// Transcribed from retail entry RVA 0x6e700 (mfc140u):
//     if (iColumn < 0) { m_mapColumnsStatus.RemoveAll(); return; }      // 0x6e715: no redraw on this path
//     if (bAdd) bAdd = m_bMultipleSort ? bAdd : FALSE;                   // neg/sbb/and at 0x6e728..0x6e732
//     if (!bAdd) m_mapColumnsStatus.RemoveAll();                         // 0x6e744
//     m_mapColumnsStatus.SetAt(iColumn, bAscending ? 1 : -1);            // 0x6e75a; neg/sbb/and $2/dec
//     ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);
extern "C" void MS_ABI impl__SetSortColumn_CMFCHeaderCtrl__QEAAXHHH_Z(void* pThis, int iColumn, int bAscending, int bAdd) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HeaderCtrlLayout* d = L(pThis);
    if (iColumn < 0) {
        ColRemoveAll(d->m_mapColumnsStatus);
        return;
    }
    if (bAdd != 0 && d->m_bMultipleSort == 0) bAdd = FALSE;
    if (bAdd == 0) ColRemoveAll(d->m_mapColumnsStatus);
    ColSetAt(d->m_mapColumnsStatus, iColumn, bAscending != 0 ? 1 : -1);
    RedrawSelf(pThis);
}

// Symbol: ?RemoveSortColumn@CMFCHeaderCtrl@@QEAAXH@Z
// Transcribed from retail entry RVA 0x6e780 (mfc140u):
//     m_mapColumnsStatus.RemoveKey(iColumn);            // inlined, 0x6e798..0x6e817 (see ColRemoveKey)
//     ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);        // tail jump at 0x6e83a, unconditional
extern "C" void MS_ABI impl__RemoveSortColumn_CMFCHeaderCtrl__QEAAXH_Z(void* pThis, int iColumn) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    ColRemoveKey(L(pThis)->m_mapColumnsStatus, iColumn);
    RedrawSelf(pThis);
}

// Symbol: ?EnableMultipleSort@CMFCHeaderCtrl@@QEAAXH@Z
// Transcribed from retail entry RVA 0x6ed20 (mfc140u):
//     if (m_bMultipleSort == bEnable) return;
//     m_bMultipleSort = bEnable;
//     if (bEnable) return;                                       // test %edx,%edx; jne exit
//     m_mapColumnsStatus.RemoveAll();                            // 0x6ed42
//     if (m_hWnd != NULL) ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);   // the one site that tests m_hWnd
extern "C" void MS_ABI impl__EnableMultipleSort_CMFCHeaderCtrl__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HeaderCtrlLayout* d = L(pThis);
    if (d->m_bMultipleSort == bEnable) return;
    d->m_bMultipleSort = bEnable;
    if (bEnable != 0) return;
    ColRemoveAll(d->m_mapColumnsStatus);
    if (HwndOf(pThis) != nullptr) RedrawSelf(pThis);
}

// ---------------------------------------------------------------------------
// Overrides / implementation
// ---------------------------------------------------------------------------

// Symbol: ?CommonInit@CMFCHeaderCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x6f050 (mfc140u):
//     HWND hWnd = m_hWnd;
//     for (;;) {
//         CWnd* pParent = CWnd::FromHandle(::GetParent(hWnd));      // IAT 0x1802c72d8 = USER32!GetParent; 0x28ad70
//         if (pParent == NULL) break;
//         if (pParent->IsKindOf(RUNTIME_CLASS(CDialog))) {           // 0x234cf0 with the CRuntimeClass at 0x180325ad0 ("CDialog", rtcu.py)
//             m_bIsDlgControl = TRUE;                                // 0x6f092
//             break;
//         }
//         hWnd = pParent->m_hWnd;
//     }
// Nothing else is initialised here.
extern "C" void MS_ABI impl__CommonInit_CMFCHeaderCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HWND hWnd = HwndOf(pThis);
    for (;;) {
        void* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(hWnd));
        if (pParent == nullptr) break;
        if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pParent, impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ())) {
            L(pThis)->m_bIsDlgControl = TRUE;
            break;
        }
        hWnd = HwndOf(pParent);
    }
}

// Symbol: ?PreSubclassWindow@CMFCHeaderCtrl@@MEAAXXZ
// Retail entry RVA 0x6f040 (mfc140u) is a bare `jmp 0x6f050` -- a tail call to
// CommonInit.  The CHeaderCtrl::PreSubclassWindow() the source calls first is
// CWnd::PreSubclassWindow, an empty body that the retail build elided.
extern "C" void MS_ABI impl__PreSubclassWindow_CMFCHeaderCtrl__MEAAXXZ(void* pThis) {
    impl__CommonInit_CMFCHeaderCtrl__IEAAXXZ(pThis);
}

// Symbol: ?OnCreate@CMFCHeaderCtrl@@IEAAHPEAUtagCREATESTRUCTW@@@Z
// Transcribed from retail entry RVA 0x6f010 (mfc140u); lpCreateStruct is never read:
//     if ((int)CWnd::Default() == -1) return -1;     // 0x28ac80 = ?Default@CWnd@@IEAA_JXZ
//     CommonInit();                                  // 0x6f050
//     return 0;
extern "C" int MS_ABI impl__OnCreate_CMFCHeaderCtrl__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* /*lpCreateStruct*/) {
    if (pThis == nullptr) return -1;   // deviation: retail has no NULL check
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis)) == -1) return -1;
    impl__CommonInit_CMFCHeaderCtrl__IEAAXXZ(pThis);
    return 0;
}

// Symbol: ?OnEraseBkgnd@CMFCHeaderCtrl@@IEAAHPEAVCDC@@@Z
// The export resolves (ures.py) to RVA 0x3a60 (mfc140u), the shared
// `mov $1,%eax; ret` body: return TRUE, pDC unused.
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCHeaderCtrl__IEAAHPEAVCDC___Z(void* /*pThis*/, void* /*pDC*/) {
    return TRUE;
}

// Symbol: ?OnFillBackground@CMFCHeaderCtrl@@MEAAXPEAVCDC@@@Z
// Transcribed from retail entry RVA 0x6eb90 (mfc140u):
//     CRect rectClient(0,0,0,0);
//     ::GetClientRect(m_hWnd, &rectClient);                          // IAT 0x1802c7330
//     CMFCVisualManager::GetInstance()                                // 0x9774 (deviation 3)
//         ->OnFillHeaderCtrlBackground(this, pDC, rectClient);        // vslot 87 (+0x2b8); CRect by value
extern "C" void MS_ABI impl__OnFillBackground_CMFCHeaderCtrl__MEAAXPEAVCDC___Z(void* pThis, void* pDC) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(HwndOf(pThis), &rectClient);
    void* pVM = VisualManager();
    if (pVM == nullptr) return;     // deviation (3): retail would create the default manager
    impl__OnFillHeaderCtrlBackground_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__VCRect___Z(
        pVM, pThis, pDC, rectClient);
}

// Symbol: ?OnDrawSortArrow@CMFCHeaderCtrl@@MEAAXPEAVCDC@@VCRect@@@Z
// Transcribed from retail entry RVA 0x6ecd0 (mfc140u):
//     CMFCVisualManager::GetInstance()                                // 0x9774 (deviation 3)
//         ->OnDrawHeaderCtrlSortArrow(this, pDC, rectArrow, m_bAscending);   // vslot 89 (+0x2c8); +0x128
extern "C" void MS_ABI impl__OnDrawSortArrow_CMFCHeaderCtrl__MEAAXPEAVCDC__VCRect___Z(void* pThis, void* pDC, RECT rectArrow) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    void* pVM = VisualManager();
    if (pVM == nullptr) return;     // deviation (3)
    impl__OnDrawHeaderCtrlSortArrow_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__H_Z(
        pVM, pThis, pDC, &rectArrow, L(pThis)->m_bAscending);
}

// Symbol: ?OnDrawItem@CMFCHeaderCtrl@@MEAAXPEAVCDC@@HVCRect@@HH@Z
// Transcribed from retail entry RVA 0x6e330 (mfc140u).  `rect` is the callee's
// own by-value copy; retail hands its address to the visual manager (CRect&)
// and keeps reading the possibly-updated copy afterwards, as here.
//     CMFCVisualManager::GetInstance()->OnDrawHeaderCtrlBorder(this, pDC, rect, bIsPressed, bIsHighlighted);  // vslot 88
//     if (iItem < 0) return;                                                     // js 0x6e6d4
//     int nSortVal = 0;
//     if (m_mapColumnsStatus.Lookup(iItem, nSortVal) && nSortVal != 0) {         // 0x2f610
//         CRect rectArrow = rect;
//         ::InflateRect(&rectArrow, -5, -5);                                      // IAT 0x1802c72e8
//         rectArrow.left = rectArrow.right - (rectArrow.bottom - rectArrow.top);
//         if (bIsPressed) { rectArrow.right++; rectArrow.bottom++; }
//         rect.right = rectArrow.left - 1;
//         int dy = (int)((double)(rectArrow.right - rectArrow.left) * -0.134);   // constant at 0x3500e0 = -0.134
//         ::InflateRect(&rectArrow, 0, dy);                                       // == DeflateRect(0, (int)(.134 * Width()))
//         m_bAscending = nSortVal > 0;                                            // 0x6e44a (+0x128)
//         OnDrawSortArrow(pDC, rectArrow);                                        // vslot 96 (+0x300)
//     }
//     HDITEM hdItem; memset(&hdItem, 0, 0x48);                                    // IAT 0x1802c7418 = memset
//     hdItem.mask = HDI_FORMAT | HDI_BITMAP | HDI_TEXT | HDI_IMAGE (0x36);
//     WCHAR szText[256]; hdItem.pszText = szText; hdItem.cchTextMax = 255;
//     if (!::SendMessage(m_hWnd, HDM_GETITEMW (0x120b), iItem, &hdItem)) return;
//     if ((hdItem.fmt & HDF_IMAGE (0x800)) && hdItem.iImage >= 0) {
//         CImageList* pIL = CImageList::FromHandle((HIMAGELIST)::SendMessage(m_hWnd, HDM_GETIMAGELIST (0x1209), 0, 0));  // 0x296cb0
//         if (pIL != NULL) {
//             int cx = 0, cy = 0;
//             ImageList_GetIconSize(pIL->m_hImageList, &cx, &cy);             // delay-load wrapper 0x6f160; result unchecked
//             ImageList_Draw(pIL->m_hImageList, hdItem.iImage, pDC ? pDC->m_hDC : NULL,
//                            rect.left + 1, (rect.top + rect.bottom - cy) / 2, ILD_NORMAL);   // wrapper 0x60d64
//             rect.left += cx;
//         }
//     }
//     if ((hdItem.fmt & (HDF_BITMAP | HDF_BITMAP_ON_RIGHT) (0x3000)) && hdItem.hbm != NULL) {
//         CBitmap* pBmp = (CBitmap*)CGdiObject::FromHandle(hdItem.hbm);         // 0x2a3ea0
//         BITMAP bmp; ::GetObjectW(pBmp->m_hObject, sizeof(BITMAP) (0x20), &bmp); // IAT 0x1802c6290
//         CRect rectBitmap;                                                      // only left/top are stored
//         if (hdItem.fmt & HDF_BITMAP_ON_RIGHT (0x1000)) {
//             rectBitmap.right = rect.right - 1;
//             rect.right = rectBitmap.left = rectBitmap.right - bmp.bmWidth;
//         } else {
//             rectBitmap.left = rect.left + 1;
//             rect.left = rectBitmap.right = rectBitmap.left + bmp.bmWidth;
//         }
//         int dy = (rect.bottom - rect.top - bmp.bmHeight) / 2;                   // signed division (cltd/sub/sar)
//         rectBitmap.top = rect.top + (dy >= 0 ? dy : 0);                         // cmovns
//         ::DrawStateW(pDC->m_hDC, NULL, NULL, (LPARAM)pBmp->m_hObject, 0,        // IAT 0x1802c6bb8
//                      rectBitmap.left, rectBitmap.top,
//                      rectBitmap.right - rectBitmap.left, bmp.bmHeight, DST_BITMAP (4));
//     }
//     if ((hdItem.fmt & HDF_STRING (0x4000)) && hdItem.pszText != NULL) {
//         CRect rectLabel = rect; ::InflateRect(&rectLabel, -5, 0);
//         CString strLabel(hdItem.pszText);                                       // 0xdcb0 (deviation 6)
//         UINT uiFlags = (hdItem.fmt & HDF_CENTER (2)) ? 0x8825                   // DT_CENTER |
//                      : 0x8824 | ((hdItem.fmt & HDF_RIGHT (1)) << 1);            // DT_RIGHT when HDF_RIGHT
//                      // 0x8824 == DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS
//         pDC->DrawText(strLabel, strLabel.GetLength(), &rectLabel, uiFlags);     // CDC vslot 28 (+0xe0)
//     }
extern "C" void MS_ABI impl__OnDrawItem_CMFCHeaderCtrl__MEAAXPEAVCDC__HVCRect__HH_Z(
    void* pThis, void* pDC, int iItem, RECT rect, int bIsPressed, int bIsHighlighted) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HeaderCtrlLayout* d = L(pThis);

    void* pVM = VisualManager();
    if (pVM != nullptr) {           // deviation (3): retail would create the default manager
        impl__OnDrawHeaderCtrlBorder_CMFCVisualManager__UEAAXPEAVCMFCHeaderCtrl__PEAVCDC__AEAVCRect__HH_Z(
            pVM, pThis, pDC, &rect, bIsPressed, bIsHighlighted);
    }

    if (iItem < 0) return;

    int nSortVal = 0;
    if (ColLookup(d->m_mapColumnsStatus, iItem, nSortVal) && nSortVal != 0) {
        RECT rectArrow = { rect.left, rect.top, rect.right, rect.bottom };
        ::InflateRect(&rectArrow, -5, -5);
        rectArrow.left = rectArrow.right - (rectArrow.bottom - rectArrow.top);
        if (bIsPressed != 0) {
            rectArrow.right++;
            rectArrow.bottom++;
        }
        rect.right = rectArrow.left - 1;
        const int dy = static_cast<int>(static_cast<double>(rectArrow.right - rectArrow.left) * -0.134);
        ::InflateRect(&rectArrow, 0, dy);
        d->m_bAscending = nSortVal > 0 ? TRUE : FALSE;
        impl__OnDrawSortArrow_CMFCHeaderCtrl__MEAAXPEAVCDC__VCRect___Z(pThis, pDC, rectArrow);   // retail: vslot 96 (deviation 1)
    }

    HDITEMW hdItem;
    std::memset(&hdItem, 0, sizeof(hdItem));
    static_assert(sizeof(HDITEMW) == 0x48, "retail memsets 0x48 bytes of HDITEM");
    hdItem.mask = HDI_FORMAT | HDI_BITMAP | HDI_TEXT | HDI_IMAGE;
    wchar_t szText[256];
    hdItem.pszText = szText;
    hdItem.cchTextMax = 255;
    if (!::SendMessage(HwndOf(pThis), HDM_GETITEMW, static_cast<WPARAM>(iItem), reinterpret_cast<LPARAM>(&hdItem))) return;

    if ((hdItem.fmt & HDF_IMAGE) && hdItem.iImage >= 0) {
        HIMAGELIST hIL = reinterpret_cast<HIMAGELIST>(::SendMessage(HwndOf(pThis), HDM_GETIMAGELIST, 0, 0));
        const ImageListView* pImageList = static_cast<const ImageListView*>(impl__FromHandle_CImageList__SAPEAV1_PEAU_IMAGELIST___Z(hIL));
        if (pImageList != nullptr) {
            int cx = 0;
            int cy = 0;
            ::ImageList_GetIconSize(pImageList->m_hImageList, &cx, &cy);    // deviation (7); result unchecked as in retail
            ::ImageList_Draw(pImageList->m_hImageList, hdItem.iImage, DcHandle(pDC),
                             rect.left + 1, (rect.top + rect.bottom - cy) / 2, ILD_NORMAL);
            rect.left += cx;
        }
    }

    if ((hdItem.fmt & (HDF_BITMAP | HDF_BITMAP_ON_RIGHT)) && hdItem.hbm != nullptr) {
        const GdiObjView* pBmp = static_cast<const GdiObjView*>(impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(hdItem.hbm));
        if (pBmp != nullptr) {      // deviation: retail dereferences the FromHandle result without a test
            BITMAP bmp;
            std::memset(&bmp, 0, sizeof(bmp));   // deviation: retail leaves bmp uninitialised
                                                 // and ignores GetObjectW's result
            ::GetObjectW(pBmp->m_hObject, sizeof(BITMAP), &bmp);
            int xLeft, xRight;
            if (hdItem.fmt & HDF_BITMAP_ON_RIGHT) {
                xRight = rect.right - 1;
                xLeft = xRight - bmp.bmWidth;
                rect.right = xLeft;
            } else {
                xLeft = rect.left + 1;
                xRight = xLeft + bmp.bmWidth;
                rect.left = xRight;
            }
            const int dy = (rect.bottom - rect.top - bmp.bmHeight) / 2;
            const int yTop = rect.top + (dy >= 0 ? dy : 0);
            ::DrawStateW(DcHandle(pDC), nullptr, nullptr,
                         reinterpret_cast<LPARAM>(pBmp->m_hObject), 0,
                         xLeft, yTop, xRight - xLeft, bmp.bmHeight, DST_BITMAP);
        }
    }

    if ((hdItem.fmt & HDF_STRING) && hdItem.pszText != nullptr) {
        RECT rectLabel = { rect.left, rect.top, rect.right, rect.bottom };
        ::InflateRect(&rectLabel, -5, 0);
        UINT uiTextFlags = DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX | DT_END_ELLIPSIS;   // 0x8824
        if (hdItem.fmt & HDF_CENTER) {
            uiTextFlags |= DT_CENTER;
        } else if (hdItem.fmt & HDF_RIGHT) {
            uiTextFlags |= DT_RIGHT;
        }
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(pDC, hdItem.pszText,
                                                          static_cast<int>(std::wcslen(hdItem.pszText)),
                                                          &rectLabel, uiTextFlags);   // retail: CDC vslot 28 (deviation 2)
    }
}

// Symbol: ?SelectFont@CMFCHeaderCtrl@@IEAAPEAVCFont@@PEAVCDC@@@Z
// Transcribed from retail entry RVA 0x6ec20 (mfc140u):
//     if (m_hFont != NULL)
//         return pDC->SelectObject(CFont::FromHandle(m_hFont));        // 0x2a3ea0 = CGdiObject::FromHandle; CDC vslot 12
//     if (m_bIsDlgControl)
//         return (CFont*)pDC->SelectStockObject(DEFAULT_GUI_FONT (0x11)); // CDC vslot 11
//     <afxGlobalData init gate>                                          // 0x6ec66..0x6ec7b (deviation 4)
//     return pDC->SelectObject(&afxGlobalData.fontRegular);              // +0x1a8 of 0x3c1620; CDC vslot 12
extern "C" void* MS_ABI impl__SelectFont_CMFCHeaderCtrl__IEAAPEAVCFont__PEAVCDC___Z(void* pThis, void* pDC) {
    if (pThis == nullptr) return nullptr;   // deviation: retail has no NULL check
    const HeaderCtrlLayout* d = L(pThis);
    if (d->m_hFont != nullptr) {
        void* pFont = impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(d->m_hFont);
        return impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pFont);
    }
    if (d->m_bIsDlgControl != 0) {
        return impl__SelectStockObject_CDC__UEAAPEAVCGdiObject__H_Z(pDC, DEFAULT_GUI_FONT);
    }
    EnsureGlobalDataInitialized();
    // Deviation (4): retail passes the CFont object embedded in afxGlobalData;
    // its handle is read here (falling back to the stock GUI font while the
    // block is unpopulated) and wrapped through CGdiObject::FromHandle.
    void* pRegular = impl__FromHandle_CGdiObject__SAPEAV1_PEAX_Z(GD_fontRegular());
    return impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pRegular);
}

// Symbol: ?OnPaint@CMFCHeaderCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x6e850 (mfc140u):
//     if (GetStyle() & HDS_FILTERBAR (0x100)) { CWnd::Default(); return; }   // 0x2a9690; bt $8
//     CPaintDC dc(this);                                                     // 0x2a3d20
//     CMemDC memDC(dc, this);                                                // 0x69f50
//     CDC* pDC = &memDC.GetDC();                                             // inline: m_bMemDC ? &m_dcMem : m_dc (0x6e8b6..0x6e8c1)
//     CRect rectClip; ::GetClipBox(dc.m_hDC, &rectClip);                     // IAT 0x1802c62e0; never read afterwards
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);                // IAT 0x1802c7330 (first copy, 0x6e8ea)
//     OnFillBackground(pDC);                                                 // vslot 95 (+0x2f8)
//     CFont* pOldFont = SelectFont(pDC);                                     // 0x6ec20
//     <afxGlobalData init gate>                                              // 0x6e924..0x6e939
//     pDC->SetTextColor(afxGlobalData.clrBtnText);                           // CDC vslot 14; +0x34; result discarded
//     pDC->SetBkMode(TRANSPARENT);                                           // 0x2a2860
//     CRect rect; ::GetClientRect(m_hWnd, &rect);                            // second copy, 0x6e96d
//     int nCount = (int)::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0);
//     int xMax = 0; CRect rectItem(0,0,0,0);
//     for (int i = 0; i < nCount; i++) {
//         POINT pt = {0,0}; ::GetCursorPos(&pt); ::ScreenToClient(m_hWnd, &pt);     // IAT 0x1802c7320 / 0x1802c7318
//         HDHITTESTINFO hti; hti.pt = pt;
//         int iHit = (int)::SendMessage(m_hWnd, HDM_HITTEST (0x1206), 0, &hti);
//         BOOL bHit = (iHit == i) && (hti.flags & HHT_ONHEADER (2));
//         BOOL bIsPressed = m_bIsMousePressed && bHit;                             // 0x6ea03..0x6ea10
//         ::SendMessage(m_hWnd, HDM_GETITEMRECT (0x1207), i, &rectItem);
//         CRgn rgnClip; rgnClip.Attach(::CreateRectRgnIndirect(&rectItem));        // IAT 0x1802c61e0; 0x2a3ed0 (deviation 5)
//         pDC->SelectClipRgn(&rgnClip);                                            // 0x2a2e20
//         OnDrawItem(pDC, i, rectItem, bIsPressed, m_nHighlightedItem == i);       // vslot 94 (+0x2f0); sete at 0x6ea77
//         pDC->SelectClipRgn(NULL);
//         xMax = max(xMax, rectItem.right);                                        // cmovg at 0x6eaae
//         rgnClip.~CRgn();       // the CRgn vptr store is inlined at 0x6eab3; the
//                                // call at 0x6eabd is ~CGdiObject (0x1c6f0), which
//                                // reinstalls vftable 0x1802dddd8 and tail-calls
//                                // CGdiObject::DeleteObject (0x2a3f60) -> ::DeleteObject
//     }
//     if (nCount == 0) { rectItem = rect; rectItem.right = rect.right + 1; }       // 0x6eb64..0x6eb81
//     else             { rectItem.left = xMax; rectItem.right = rect.right + 1; }  // 0x6ead9..0x6eae2
//     OnDrawItem(pDC, -1, rectItem, FALSE, FALSE);                                 // vslot 94
//     pDC->SelectObject(pOldFont);                                                 // CDC vslot 12
//     memDC.~CMemDC(); dc.~CPaintDC();                                             // 0x6a380 / 0x2a3dd0
extern "C" void MS_ABI impl__OnPaint_CMFCHeaderCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HeaderCtrlLayout* d = L(pThis);
    const HWND hWnd = HwndOf(pThis);

    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & HDS_FILTERBAR) {
        impl__Default_CWnd__IEAA_JXZ(pThis);
        return;
    }

    // CPaintDC dc(this) -- retail ctor 0x2a3d20 (deviation 2): zero the CDC,
    // m_hWnd = this->m_hWnd, m_hDC = m_hAttribDC = ::BeginPaint(m_hWnd, &m_ps).
    // (Retail throws AfxThrowResourceException when BeginPaint fails; here
    // the NULL HDC just makes every draw below a no-op.)
    PaintDcView paintDC;
    std::memset(&paintDC, 0, sizeof(paintDC));
    paintDC.m_hWnd = hWnd;
    paintDC.dc.m_hDC = ::BeginPaint(hWnd, &paintDC.m_ps);
    paintDC.dc.m_hAttribDC = paintDC.dc.m_hDC;

    alignas(void*) unsigned char memStorage[sizeof(S_Cmemdc)] = {};
    S_Cmemdc* pMemDC = reinterpret_cast<S_Cmemdc*>(memStorage);
    impl___0CMemDC__QEAA_AEAVCDC__PEAVCWnd___Z(pMemDC, &paintDC.dc, pThis);
    void* pDC = pMemDC->m_bMemDC ? static_cast<void*>(&pMemDC->m_dcMem) : pMemDC->m_dc;   // CMemDC::GetDC()

    RECT rectClip = {0, 0, 0, 0};
    ::GetClipBox(paintDC.dc.m_hDC, &rectClip);
    RECT rectClient = {0, 0, 0, 0};
    ::GetClientRect(hWnd, &rectClient);

    impl__OnFillBackground_CMFCHeaderCtrl__MEAAXPEAVCDC___Z(pThis, pDC);          // retail: vslot 95 (deviation 1)
    void* pOldFont = impl__SelectFont_CMFCHeaderCtrl__IEAAPEAVCFont__PEAVCDC___Z(pThis, pDC);
    EnsureGlobalDataInitialized();
    impl__SetTextColor_CDC__UEAAKK_Z(pDC, GD_clrBtnText());                        // retail: CDC vslot 14 (deviation 2)
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);

    RECT rect = {0, 0, 0, 0};
    ::GetClientRect(hWnd, &rect);
    const int nCount = static_cast<int>(::SendMessage(hWnd, HDM_GETITEMCOUNT, 0, 0));
    int xMax = 0;
    RECT rectItem = {0, 0, 0, 0};
    for (int i = 0; i < nCount; i++) {
        POINT ptCursor = {0, 0};
        ::GetCursorPos(&ptCursor);
        ::ScreenToClient(hWnd, &ptCursor);
        HDHITTESTINFO hti;
        std::memset(&hti, 0, sizeof(hti));   // retail leaves flags/iItem uninitialised before the hit test
        hti.pt = ptCursor;
        const int iHit = static_cast<int>(::SendMessage(hWnd, HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hti)));
        const BOOL bHit = (iHit == i && (hti.flags & HHT_ONHEADER)) ? TRUE : FALSE;
        const BOOL bIsPressed = (d->m_bIsMousePressed != 0 && bHit) ? TRUE : FALSE;

        ::SendMessage(hWnd, HDM_GETITEMRECT, static_cast<WPARAM>(i), reinterpret_cast<LPARAM>(&rectItem));

        HRGN hRgnClip = ::CreateRectRgnIndirect(&rectItem);                      // deviation (5): raw HRGN
        DcSelectClipRgn(pDC, hRgnClip);                                          // retail: CDC::SelectClipRgn (0x2a2e20)
        impl__OnDrawItem_CMFCHeaderCtrl__MEAAXPEAVCDC__HVCRect__HH_Z(
            pThis, pDC, i, rectItem, bIsPressed, d->m_nHighlightedItem == i ? TRUE : FALSE);   // retail: vslot 94
        DcSelectClipRgn(pDC, nullptr);                                           // retail: CDC::SelectClipRgn(NULL)
        if (rectItem.right > xMax) xMax = rectItem.right;
        if (hRgnClip != nullptr) ::DeleteObject(hRgnClip);
    }

    if (nCount == 0) {
        rectItem = rect;
        rectItem.right = rect.right + 1;
    } else {
        rectItem.left = xMax;
        rectItem.right = rect.right + 1;
    }
    impl__OnDrawItem_CMFCHeaderCtrl__MEAAXPEAVCDC__HVCRect__HH_Z(pThis, pDC, -1, rectItem, FALSE, FALSE);   // retail: vslot 94

    impl__SelectObject_CDC__UEAAPEAVCFont__PEAV2__Z(pDC, pOldFont);               // retail: CDC vslot 12

    impl___1CMemDC__UEAA_XZ(pMemDC);
    // ~CPaintDC -- retail dtor 0x2a3dd0: ::EndPaint(m_hWnd, &m_ps), CDC::Detach, ~CDC.
    ::EndPaint(hWnd, &paintDC.m_ps);
    paintDC.dc.m_hDC = nullptr;
    paintDC.dc.m_hAttribDC = nullptr;
}

// ---------------------------------------------------------------------------
// Message handlers
// ---------------------------------------------------------------------------

// Symbol: ?OnLButtonDown@CMFCHeaderCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x6ecb0 (mfc140u); nFlags / point are never read:
//     m_bIsMousePressed = TRUE;      // +0x120
//     CWnd::Default();               // tail jump to 0x28ac80
extern "C" void MS_ABI impl__OnLButtonDown_CMFCHeaderCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int /*nFlags*/, long long /*point*/) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    L(pThis)->m_bIsMousePressed = TRUE;
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnLButtonUp@CMFCHeaderCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x6ecc0 (mfc140u); nFlags / point are never read:
//     m_bIsMousePressed = FALSE;     // +0x120
//     CWnd::Default();               // tail jump to 0x28ac80
extern "C" void MS_ABI impl__OnLButtonUp_CMFCHeaderCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int /*nFlags*/, long long /*point*/) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    L(pThis)->m_bIsMousePressed = FALSE;
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnMouseMove@CMFCHeaderCtrl@@IEAAXIVCPoint@@@Z
// Transcribed from retail entry RVA 0x6eeb0 (mfc140u):
//     if ((nFlags & MK_LBUTTON) == 0) {                                   // test $1,%dl
//         int nPrev = m_nHighlightedItem;
//         HDHITTESTINFO hti; hti.pt = point;
//         m_nHighlightedItem = (int)::SendMessage(m_hWnd, HDM_HITTEST (0x1206), 0, &hti);
//         if ((hti.flags & HHT_ONHEADER (2)) == 0) m_nHighlightedItem = -1;
//         if (!m_bTracked) {                                              // +0x12c
//             m_bTracked = TRUE;
//             TRACKMOUSEEVENT tme; tme.cbSize = 0x18; tme.dwFlags = TME_LEAVE (2); tme.hwndTrack = m_hWnd;
//             ::TrackMouseEvent(&tme);                                    // IAT 0x1802c7310; dwHoverTime left unset
//         }
//         if (nPrev != m_nHighlightedItem) ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);
//     }
//     CWnd::Default();                                                    // 0x28ac80, always
extern "C" void MS_ABI impl__OnMouseMove_CMFCHeaderCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HeaderCtrlLayout* d = L(pThis);
    if ((nFlags & MK_LBUTTON) == 0) {
        const int nPrevHighlightedItem = d->m_nHighlightedItem;
        HDHITTESTINFO hti;
        std::memset(&hti, 0, sizeof(hti));   // retail leaves flags/iItem uninitialised before the hit test
        hti.pt.x = PtX(point);
        hti.pt.y = PtY(point);
        d->m_nHighlightedItem = static_cast<int>(::SendMessage(HwndOf(pThis), HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hti)));
        if ((hti.flags & HHT_ONHEADER) == 0) d->m_nHighlightedItem = -1;
        if (d->m_bTracked == 0) {
            d->m_bTracked = TRUE;
            TRACKMOUSEEVENT tme;
            tme.cbSize = sizeof(TRACKMOUSEEVENT);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = HwndOf(pThis);
            tme.dwHoverTime = 0;   // retail never writes this field; unused with TME_LEAVE
            ::TrackMouseEvent(&tme);
        }
        if (nPrevHighlightedItem != d->m_nHighlightedItem) RedrawSelf(pThis);
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// Symbol: ?OnMouseLeave@CMFCHeaderCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x6ef90 (mfc140u); no Default() call:
//     m_bTracked = FALSE;                                               // +0x12c
//     if (m_nHighlightedItem >= 0) {                                    // +0x134
//         m_nHighlightedItem = -1;
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);
//     }
extern "C" void MS_ABI impl__OnMouseLeave_CMFCHeaderCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    HeaderCtrlLayout* d = L(pThis);
    d->m_bTracked = FALSE;
    if (d->m_nHighlightedItem >= 0) {
        d->m_nHighlightedItem = -1;
        RedrawSelf(pThis);
    }
}

// Symbol: ?OnCancelMode@CMFCHeaderCtrl@@IEAAXXZ
// Transcribed from retail entry RVA 0x6efd0 (mfc140u):
//     CWnd::Default();                                                  // 0x28ac80, first
//     if (m_nHighlightedItem >= 0) {                                    // +0x134
//         m_nHighlightedItem = -1;
//         ::RedrawWindow(m_hWnd, NULL, NULL, 0x105);
//     }
extern "C" void MS_ABI impl__OnCancelMode_CMFCHeaderCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    impl__Default_CWnd__IEAA_JXZ(pThis);
    HeaderCtrlLayout* d = L(pThis);
    if (d->m_nHighlightedItem >= 0) {
        d->m_nHighlightedItem = -1;
        RedrawSelf(pThis);
    }
}

// Symbol: ?OnSetFont@CMFCHeaderCtrl@@IEAAXPEAVCFont@@H@Z
// Transcribed from retail entry RVA 0x6f0b0 (mfc140u); no Default() call, so the
// underlying header window never receives the WM_SETFONT:
//     m_hFont = pFont != NULL ? (HFONT)pFont->m_hObject : NULL;        // +0x138; +8 of the CFont
//     if (bRedraw) {
//         ::InvalidateRect(m_hWnd, NULL, TRUE);                         // IAT 0x1802c7128
//         ::UpdateWindow(m_hWnd);                                       // IAT 0x1802c7300
//     }
extern "C" void MS_ABI impl__OnSetFont_CMFCHeaderCtrl__IEAAXPEAVCFont__H_Z(void* pThis, void* pFont, int bRedraw) {
    if (pThis == nullptr) return;   // deviation: retail has no NULL check
    L(pThis)->m_hFont = pFont != nullptr ? static_cast<HFONT>(static_cast<const GdiObjView*>(pFont)->m_hObject) : nullptr;
    if (bRedraw != 0) {
        ::InvalidateRect(HwndOf(pThis), nullptr, TRUE);
        ::UpdateWindow(HwndOf(pThis));
    }
}
