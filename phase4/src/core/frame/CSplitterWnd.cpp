// CSplitterWnd — OpenMFC implementation.
// Sources: cbarcore.cpp (file split); behaviour transcribed from the retail
// mfc140u.dll winsplit.cpp bodies.  Every function below cites the mfc140u RVA
// of the retail function ENTRY it was read from (disas.py --u).
//
// ---------------------------------------------------------------------------
// Layout (retail mfc140u, sizeof(CSplitterWnd) == 0x180, sizeof(CWnd) == 0xe8).
// The ctor at RVA 0x2aa4b0 zero-fills +0xe8..+0x17c (the last 4 bytes are never
// written), then stores 7 at +0xf8, +0xfc, +0x108, +0x10c and 2 at +0x110, +0x114; member order is the real
// afxext.h declaration order (verified against every body below):
//   +0xe8  CRuntimeClass* m_pDynamicViewClass   +0xf0  int m_nMaxRows   +0xf4 int m_nMaxCols
//   +0xf8  m_cxSplitter  +0xfc  m_cySplitter    +0x100 m_cxBorderShare +0x104 m_cyBorderShare
//   +0x108 m_cxSplitterGap +0x10c m_cySplitterGap +0x110 m_cxBorder     +0x114 m_cyBorder
//   +0x118 m_nRows       +0x11c m_nCols          +0x120 m_bHasHScroll   +0x124 m_bHasVScroll
//   +0x128 CRowColInfo* m_pColInfo               +0x130 CRowColInfo* m_pRowInfo
//   +0x138 m_bTracking   +0x13c m_bTracking2     +0x140 CPoint m_ptTrackOffset
//   +0x148 CRect m_rectLimit +0x158 CRect m_rectTracker +0x168 CRect m_rectTracker2
//   +0x178 int m_htTrack (+0x17c pad)
//   CRowColInfo == { int nMinSize; int nIdealSize; int nCurSize; } (12 bytes).
//
// include/openmfc/afxole.h declares the same total size (0x180) but a DIFFERENT
// member order for +0xe8..+0x137 (its m_nRows is at +0xe8, and so on) and has
// no m_pDynamicViewClass, m_cxBorder, m_cyBorder, m_pColInfo or m_pRowInfo at
// all.  That header is not owned by this file, so:
//   * the named header members (m_nRows, m_nCols, m_cxSplitter, m_cySplitter,
//     m_cxBorderShare, m_cyBorderShare, m_cxSplitterGap, m_cySplitterGap,
//     m_nMaxRows, m_nMaxCols, m_bHasHScroll, m_bHasVScroll) are used by name at
//     the OpenMFC offsets, because the exports in core/frame/Thunks.cpp that
//     route through the C++ members (Create, CreateStatic, RecalcLayout, ...)
//     already use them there;
//   * the 72-byte _splitterwnd_padding at +0x138 is exactly retail's tracking
//     block, so that block is kept IN PLACE at the retail offsets
//     (SplitterTrackState below, pinned by static_asserts);
//   * the five members with no home in the header live in a per-instance
//     side node (SplitterExtra below), allocated with malloc so this file adds
//     no C++ heap symbols (the per-file link audit rejects new _Z* undefineds).
// An MSVC-compiled subclass that reads m_nRows/m_cxSplitter through the real
// afxext.h therefore still sees the wrong offsets -- that is a header defect
// (reported as a header request), not something this file can repair.
// ---------------------------------------------------------------------------
// Virtual dispatch: retail reaches its own overridables through the MSVC
// vtable (slot numbers are noted per call: CWnd owns slots 0..90, CSplitterWnd
// adds 91 Create .. 119 StopTracking in afxext.h declaration order).  OpenMFC
// objects may carry either a mingw or an MSVC vtable, so -- following the
// convention used throughout core/ (see the note in CWnd::Create,
// core/window/CWnd.cpp) -- every such call is made directly to the exported
// thunk / C++ member instead.  A client override of one of those virtuals is
// consequently not reached from inside the DLL.

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <mutex>

// ---------------------------------------------------------------------------
// Thunks this file calls.  Each definition was located in the tree (path noted).
// ---------------------------------------------------------------------------
extern "C" CWnd* MS_ABI impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(const CWnd* pThis, int nID);                       // core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                               // core/window/CWnd.cpp
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);              // core/window/Thunks.cpp
extern "C" int MS_ABI impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(                     // core/window/CWnd.cpp
    CWnd* pThis, DWORD dwExStyle, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle,
    int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU nIDorHMenu, void* lpParam);
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(                // core/window/CWnd.cpp
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName, DWORD dwStyle, const RECT& rect,
    CWnd* pParentWnd, UINT nID, CCreateContext* pContext);
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(CWnd* pThis);                                          // core/window/CWnd.cpp
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags);   // core/window/CWnd.cpp
extern "C" int MS_ABI impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags); // core/window/CWnd.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                                           // core/window/Thunks.cpp
extern "C" int MS_ABI impl__OnCommand_CWnd__MEAAH_K_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam);            // core/window/CWnd.cpp
extern "C" int MS_ABI impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(CWnd* pThis, WPARAM wParam, LPARAM lParam, LRESULT* pResult); // core/window/CWnd.cpp
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass); // core/runtime/CObject.cpp
extern "C" CObject* MS_ABI impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(CRuntimeClass* pThis);        // core/runtime/CRuntimeClass.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CView__SAPEAUCRuntimeClass__XZ();                          // core/view/CView.cpp
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CScrollView__SAPEAUCRuntimeClass__XZ();                    // core/view/CScrollView.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();                                                 // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                             // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowResourceException__YAXXZ();                                               // detail/MfcExceptionsSupport.cpp
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);                                // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxLockGlobals__YAXH_Z(int nLockType);                                            // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__AfxUnlockGlobals__YAXH_Z(int nLockType);                                          // featurepack/CMFC_misc_stubs.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(const wchar_t* lpszResource, const wchar_t* lpszType); // core/runtime/Globals.cpp
extern "C" void MS_ABI impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(void* lpLayout, HWND hWnd, const RECT* lpRect); // featurepack/CMFC_misc_stubs.cpp
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight); // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(CDC* pThis, const RECT* lpRect, unsigned long clr); // core/gdi/CDC.cpp
extern "C" CBrush* MS_ABI impl__GetHalftoneBrush_CDC__SAPEAVCBrush__XZ();                                      // core/gdi/CDC.cpp
extern "C" CPaintDC* MS_ABI impl___0CPaintDC__QEAA_PEAVCWnd___Z(CPaintDC* pThis, CWnd* pWnd);                  // core/gdi/CPaintDC.cpp
extern "C" void MS_ABI impl___1CPaintDC__UEAA_XZ(CPaintDC* pThis);                                             // core/gdi/CPaintDC.cpp
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(const CView* pThis, int nBar); // core/view/CView.cpp
extern "C" int MS_ABI impl__OnScroll_CView__UEAAHIIH_Z(CView* pThis, unsigned int nScrollCode, unsigned int nPos, int bDoScroll); // core/view/CView.cpp
extern "C" int MS_ABI impl__OnScrollBy_CView__UEAAHVCSize__H_Z(CView* pThis, CSize sizeScroll, int bDoScroll); // core/view/CView.cpp
extern "C" void MS_ABI impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(CView* pThis, unsigned int nState, CFrameWnd* pFrameWnd); // core/view/CView.cpp
extern "C" int MS_ABI impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(CScrollView* pThis, unsigned int fFlags, short zDelta, CPoint point); // core/view/CScrollView.cpp
extern "C" void MS_ABI impl__SetActiveView_CFrameWnd__QEAAXPEAVCView__H_Z(CFrameWnd* pThis, CView* pViewNew, int bNotify); // core/frame/Thunks.cpp

// Thunks defined further down in this file that are called before their definition.
extern "C" int MS_ABI impl__CreateCommon_CSplitterWnd__IEAAHPEAVCWnd__UtagSIZE__KI_Z(CSplitterWnd* pThis, CWnd* pParentWnd, SIZE sizeMin, unsigned long dwStyle, unsigned int nID);
extern "C" int MS_ABI impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(CSplitterWnd* pThis, unsigned long dwStyle, unsigned int nID);
extern "C" void MS_ABI impl__DeleteColumn_CSplitterWnd__UEAAXH_Z(CSplitterWnd* pThis, int colDelete);
extern "C" void MS_ABI impl__DeleteRow_CSplitterWnd__UEAAXH_Z(CSplitterWnd* pThis, int rowDelete);
extern "C" void MS_ABI impl__DeleteView_CSplitterWnd__UEAAXHH_Z(CSplitterWnd* pThis, int row, int col);
extern "C" void MS_ABI impl__DrawAllSplitBars_CSplitterWnd__MEAAXPEAVCDC__HH_Z(CSplitterWnd* pThis, CDC* pDC, int cxInside, int cyInside);
extern "C" void MS_ABI impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(CSplitterWnd* pThis, int ht, RECT* pRectHit);
extern "C" void MS_ABI impl__GetInsideRect_CSplitterWnd__MEBAXAEAVCRect___Z(const CSplitterWnd* pThis, RECT* pRect);
extern "C" CWnd* MS_ABI impl__GetSizingParent_CSplitterWnd__IEAAPEAVCWnd__XZ(CSplitterWnd* pThis);
extern "C" int MS_ABI impl__HitTest_CSplitterWnd__MEBAHVCPoint___Z(const CSplitterWnd* pThis, long long pt);
extern "C" int MS_ABI impl__IdFromRowCol_CSplitterWnd__QEBAHHH_Z(const CSplitterWnd* pThis, int row, int col);
extern "C" int MS_ABI impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(CSplitterWnd* pThis, CWnd* pWnd, int* pRow, int* pCol);
extern "C" void MS_ABI impl__OnDraw_CSplitterWnd__MEAAXPEAVCDC___Z(CSplitterWnd* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(CSplitterWnd* pThis, CDC* pDC, int nType, const RECT* pRect);
extern "C" void* MS_ABI impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(CSplitterWnd* pThis, int* pRow, int* pCol);
extern "C" void MS_ABI impl__SetScrollStyle_CSplitterWnd__QEAAXK_Z(CSplitterWnd* pThis, unsigned long dwStyle);
extern "C" int MS_ABI impl__SplitColumn_CSplitterWnd__UEAAHH_Z(CSplitterWnd* pThis, int cxBefore);
extern "C" int MS_ABI impl__SplitRow_CSplitterWnd__UEAAHH_Z(CSplitterWnd* pThis, int cyBefore);
extern "C" void MS_ABI impl__StartTracking_CSplitterWnd__MEAAXH_Z(CSplitterWnd* pThis, int ht);
extern "C" void MS_ABI impl__StopTracking_CSplitterWnd__MEAAXH_Z(CSplitterWnd* pThis, int bAccept);
extern "C" void MS_ABI impl__TrackColumnSize_CSplitterWnd__MEAAXHH_Z(CSplitterWnd* pThis, int x, int col);
extern "C" void MS_ABI impl__TrackRowSize_CSplitterWnd__MEAAXHH_Z(CSplitterWnd* pThis, int y, int row);

namespace {

// ---- constants (afxext.h / afxres.h / winsplit.cpp; values as read in the bodies) ----
constexpr int kNoHit                   = 0;
constexpr int kVSplitterBox            = 1;
constexpr int kHSplitterBox            = 2;
constexpr int kBothSplitterBox         = 3;     // keyboard only
constexpr int kVSplitterBar1           = 101;   // ..115
constexpr int kHSplitterBar1           = 201;   // ..215
constexpr int kSplitterIntersection1   = 301;   // ..525 (15 x 15)

constexpr int kSplitBox      = 0;   // ESplitType::splitBox
constexpr int kSplitBar      = 1;   // ESplitType::splitBar
constexpr int kSplitBorder   = 3;   // ESplitType::splitBorder

constexpr unsigned kIdwPaneFirst    = 0xE900;   // AFX_IDW_PANE_FIRST
constexpr unsigned kIdwPaneLast     = 0xE9FF;   // AFX_IDW_PANE_LAST
constexpr unsigned kIdwHScrollFirst = 0xEA00;   // AFX_IDW_HSCROLL_FIRST
constexpr unsigned kIdwVScrollFirst = 0xEA10;   // AFX_IDW_VSCROLL_FIRST
constexpr unsigned kIdwSizeBox      = 0xEA20;   // AFX_IDW_SIZE_BOX
constexpr unsigned kSplsDynamicSplit = 0x0001;  // SPLS_DYNAMIC_SPLIT
constexpr unsigned kWmInitialUpdate  = 0x0364;  // WM_INITIALUPDATE
constexpr unsigned kIdcSmallArrows   = 0x7903;  // AFX_IDC_SMALLARROWS
constexpr unsigned kIdcHSplitBar     = 0x7904;  // AFX_IDC_HSPLITBAR
constexpr unsigned kIdcVSplitBar     = 0x7905;  // AFX_IDC_VSPLITBAR
constexpr int  kCritSplitterWnd      = 9;       // CRIT_SPLITTERWND (AfxLockGlobals argument at 0x2acfc9)
constexpr long kAfxWndMdiFrameReg    = 4;       // AFX_WNDMDIFRAME_REG (AfxEndDeferRegisterClass argument at 0x2aa716)
constexpr int  kCxBorder = 1, kCyBorder = 1;    // CX_BORDER / CY_BORDER
// afxData.cxBorder2 / cyBorder2 (afxData+0x10 / +0x14).  OpenMFC's exported
// afxData is a zero blob (featurepack/CMFC_misc_stubs.cpp); the real AUX_DATA
// ctor sets both to CX_BORDER * 2 == 2, so the constant is used here.
constexpr int  kCxBorder2 = 2, kCyBorder2 = 2;

inline bool IsVBar(int ht)  { return static_cast<unsigned>(ht - kVSplitterBar1) <= 14u; }
inline bool IsHBar(int ht)  { return static_cast<unsigned>(ht - kHSplitterBar1) <= 14u; }
inline bool IsIsect(int ht) { return static_cast<unsigned>(ht - kSplitterIntersection1) <= 224u; }

// Retail CRowColInfo (afxext.h), 12 bytes: the +0x0/+0x4/+0x8 accesses in every body.
struct CRowColInfo {
    int nMinSize;
    int nIdealSize;
    int nCurSize;
};
static_assert(sizeof(CRowColInfo) == 12, "CRowColInfo is 3 ints");

// Retail tracking block, kept in place in the header's 72-byte padding.
struct SplitterTrackState {
    BOOL  m_bTracking;      // +0x138
    BOOL  m_bTracking2;     // +0x13c
    POINT m_ptTrackOffset;  // +0x140
    RECT  m_rectLimit;      // +0x148
    RECT  m_rectTracker;    // +0x158
    RECT  m_rectTracker2;   // +0x168
    int   m_htTrack;        // +0x178
    int   _pad;             // +0x17c
};
constexpr size_t kTrackOffset = 0x138;
static_assert(sizeof(SplitterTrackState) == 72, "tracking block is the 72-byte padding");
static_assert(offsetof(SplitterTrackState, m_bTracking2) == 0x13c - kTrackOffset, "m_bTracking2 @0x13c");
static_assert(offsetof(SplitterTrackState, m_ptTrackOffset) == 0x140 - kTrackOffset, "m_ptTrackOffset @0x140");
static_assert(offsetof(SplitterTrackState, m_rectLimit) == 0x148 - kTrackOffset, "m_rectLimit @0x148");
static_assert(offsetof(SplitterTrackState, m_rectTracker) == 0x158 - kTrackOffset, "m_rectTracker @0x158");
static_assert(offsetof(SplitterTrackState, m_rectTracker2) == 0x168 - kTrackOffset, "m_rectTracker2 @0x168");
static_assert(offsetof(SplitterTrackState, m_htTrack) == 0x178 - kTrackOffset, "m_htTrack @0x178");
// _splitterwnd_padding is protected, so it is located through its neighbours:
static_assert(sizeof(CSplitterWnd) == 0x180, "CSplitterWnd is 0x180 bytes (retail)");
static_assert(offsetof(CSplitterWnd, m_nActiveCol) + sizeof(int) == kTrackOffset, "padding starts at +0x138");
static_assert(sizeof(CWnd) == 0xe8, "CWnd is 0xe8 bytes (retail)");

inline SplitterTrackState& Track(CSplitterWnd* p) {
    return *reinterpret_cast<SplitterTrackState*>(reinterpret_cast<char*>(p) + kTrackOffset);
}
inline const SplitterTrackState& Track(const CSplitterWnd* p) {
    return *reinterpret_cast<const SplitterTrackState*>(reinterpret_cast<const char*>(p) + kTrackOffset);
}

// Retail members with no home in the OpenMFC header (see file header).
struct SplitterExtra {
    SplitterExtra*       next;
    const CSplitterWnd*  owner;
    CRuntimeClass*       pDynamicViewClass;   // retail +0xe8
    int                  cxBorder;            // retail +0x110 (ctor: 2)
    int                  cyBorder;            // retail +0x114 (ctor: 2)
    CRowColInfo*         pColInfo;            // retail +0x128 (m_nMaxCols entries)
    CRowColInfo*         pRowInfo;            // retail +0x130 (m_nMaxRows entries)
};
std::mutex g_extraMutex;
SplitterExtra* g_extraHead = nullptr;
SplitterExtra g_extraFallback = {};   // handed out only if malloc fails; never linked

SplitterExtra& Extra(const CSplitterWnd* p) {
    std::lock_guard<std::mutex> lock(g_extraMutex);
    for (SplitterExtra* n = g_extraHead; n; n = n->next) {
        if (n->owner == p) return *n;
    }
    SplitterExtra* n = static_cast<SplitterExtra*>(std::malloc(sizeof(SplitterExtra)));
    if (!n) {
        g_extraFallback.cxBorder = g_extraFallback.cyBorder = 2;
        return g_extraFallback;
    }
    std::memset(n, 0, sizeof(*n));
    n->owner = p;
    n->cxBorder = n->cyBorder = 2;   // retail ctor: m_cxBorder = m_cyBorder = 2
    n->next = g_extraHead;
    g_extraHead = n;
    return *n;
}
void ReleaseExtra(const CSplitterWnd* p) {
    std::lock_guard<std::mutex> lock(g_extraMutex);
    for (SplitterExtra** pp = &g_extraHead; *pp; pp = &(*pp)->next) {
        if ((*pp)->owner == p) {
            SplitterExtra* n = *pp;
            *pp = n->next;
            std::free(n->pRowInfo);
            std::free(n->pColInfo);
            std::free(n);
            return;
        }
    }
}

// Retail CCreateContext (afxwin.h), 0x28 bytes: the memset(&contextT, 0, 0x28) call at
// 0x2aa95f inside CreateView (0x2aa8e0) and its +0x8/+0x10/+0x18 stores.  OpenMFC only forward-declares it.
struct CCreateContextShadow {
    CRuntimeClass* m_pNewViewClass;      // +0x00
    CDocument*     m_pCurrentDoc;        // +0x08
    CDocTemplate*  m_pNewDocTemplate;    // +0x10
    CView*         m_pLastView;          // +0x18
    CFrameWnd*     m_pCurrentFrame;      // +0x20
};
static_assert(sizeof(CCreateContextShadow) == 0x28, "CCreateContext is 5 pointers");

inline int IdFromRowColImpl(int row, int col) { return static_cast<int>(kIdwPaneFirst) + row * 16 + col; }
inline HWND Hwnd(const CWnd* p) { return p ? p->m_hWnd : nullptr; }
inline DWORD StyleOf(const CWnd* p) { return p && p->m_hWnd ? static_cast<DWORD>(::GetWindowLongPtrW(p->m_hWnd, GWL_STYLE)) : 0; }
inline POINT UnpackPoint(long long v) { POINT pt; pt.x = static_cast<int>(v & 0xffffffffu); pt.y = static_cast<int>(v >> 32); return pt; }
inline CWnd* PaneAt(const CSplitterWnd* p, int row, int col) { return impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(p, IdFromRowColImpl(row, col)); }
inline CRuntimeClass* RtcView() { return impl__GetThisClass_CView__SAPEAUCRuntimeClass__XZ(); }
inline int KindOf(const CWnd* p, const CRuntimeClass* c) { return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(p, c); }
inline void InflateRC(RECT& r, int dx, int dy) { ::InflateRect(&r, dx, dy); }

// static void AFXAPI LayoutRowCol(CSplitterWnd::CRowColInfo* pInfoArray, int nMax, int nSize, int nSizeSplitter)
// Unexported helper at RVA 0x2ac3c8 (mfc140u), called twice from RecalcLayout.  Transcribed:
//     if (nSize < 0) nSize = 0;
//     for (i = 0; i < nMax-1; i++) { if (nIdealSize < nMinSize) nIdealSize = 0; nCurSize = nIdealSize; }
//     pInfo[nMax-1].nCurSize = INT_MAX;                            // last row/col takes the rest
//     for (i = 0; i < nMax; i++) {
//         if (nSize == 0) { nCurSize = 0; continue; }
//         else if (nSize < nMinSize && i != 0) {                   // hide it, give room to previous
//             nCurSize = 0; (pInfo-1)->nCurSize += nSize + afxData.cxBorder2; nSize = 0;
//         } else if (nCurSize == 0) { if (i != 0) nCurSize = 0; } // too small to see
//         else if (nSize < nCurSize) { nCurSize = nSize; nSize = 0; }
//         else nSize -= nCurSize;
//         if (i != nMax-1) {                                       // room for the splitter
//             if (nSize > nSizeSplitter) nSize -= nSizeSplitter;
//             else { nCurSize += nSize;
//                    if (nCurSize > nSizeSplitter - afxData.cxBorder2) nCurSize -= nSizeSplitter - afxData.cyBorder2;
//                    nSize = 0; }
//         }
//     }
void LayoutRowCol(CRowColInfo* pInfoArray, int nMax, int nSize, int nSizeSplitter) {
    if (!pInfoArray || nMax <= 0) return;   // retail has no check (release image); guarded here
    if (nSize < 0) nSize = 0;
    CRowColInfo* pInfo = pInfoArray;
    int i;
    for (i = 0; i < nMax - 1; i++, pInfo++) {
        if (pInfo->nIdealSize < pInfo->nMinSize) pInfo->nIdealSize = 0;
        pInfo->nCurSize = pInfo->nIdealSize;
    }
    pInfo->nCurSize = INT_MAX;
    for (i = 0, pInfo = pInfoArray; i < nMax; i++, pInfo++) {
        if (nSize == 0) {
            pInfo->nCurSize = 0;
            continue;
        } else if (nSize < pInfo->nMinSize && i != 0) {
            pInfo->nCurSize = 0;
            (pInfo - 1)->nCurSize += nSize + kCxBorder2;
            nSize = 0;
        } else {
            if (pInfo->nCurSize == 0) {
                if (i != 0) pInfo->nCurSize = 0;
            } else if (nSize < pInfo->nCurSize) {
                pInfo->nCurSize = nSize;
                nSize = 0;
            } else {
                nSize -= pInfo->nCurSize;
            }
        }
        if (i != nMax - 1) {
            if (nSize > nSizeSplitter) {
                nSize -= nSizeSplitter;
            } else {
                pInfo->nCurSize += nSize;
                if (pInfo->nCurSize > (nSizeSplitter - kCxBorder2))
                    pInfo->nCurSize -= (nSizeSplitter - kCyBorder2);
                nSize = 0;
            }
        }
    }
}

// static void AFXAPI DeferClientPos(AFX_SIZEPARENTPARAMS* lpLayout, CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar)
// Unexported helper at RVA 0x2ac4c4 (mfc140u).  Transcribed:
//     if (bScrollBar) {                          // border only when there is no room for a real bar
//         BOOL bNeedBorder = (cx <= CX_BORDER || cy <= CY_BORDER);
//         pWnd->ModifyStyle(bNeedBorder ? 0 : WS_BORDER, bNeedBorder ? WS_BORDER : 0);
//     }
//     CRect rect(x, y, x+cx, y+cy);
//     if ((pWnd->GetExStyle() & WS_EX_CLIENTEDGE) || pWnd->IsKindOf(RUNTIME_CLASS(CSplitterWnd)))
//         rect.InflateRect(afxData.cxBorder2, afxData.cyBorder2);
//     CRect rectOld; pWnd->GetWindowRect(rectOld); pWnd->GetParent()->ScreenToClient(&rectOld);
//     if (!EqualRect(rect, rectOld)) AfxRepositionWindow(lpLayout, pWnd->m_hWnd, rect);
// Only lpLayout->hDWP (offset 0) is touched, so a bare HDWP* is what is passed around.
// DEVIATION (here, in GetSizingParent and in DoKeyboardSplit): retail's
// CWnd::ScreenToClient(LPRECT) (0x2a32b0) and CWnd::ClientToScreen(LPRECT)
// (0x2a3310) also swap left/right when the window has WS_EX_LAYOUTRTL; the raw
// USER32 calls used here do not (nor does OpenMFC's own CWnd::ScreenToClient).
void DeferClientPos(HDWP* phDWP, CWnd* pWnd, int x, int y, int cx, int cy, BOOL bScrollBar) {
    if (!pWnd || !pWnd->m_hWnd) return;   // retail dereferences pWnd unguarded; a missing pane is skipped here
    if (bScrollBar) {
        const BOOL bNeedBorder = (cx <= kCxBorder || cy <= kCyBorder);
        impl__ModifyStyle_CWnd__QEAAHKKI_Z(pWnd, bNeedBorder ? 0 : WS_BORDER, bNeedBorder ? WS_BORDER : 0, 0);
    }
    RECT rect = { x, y, x + cx, y + cy };
    const DWORD dwExStyle = static_cast<DWORD>(::GetWindowLongPtrW(pWnd->m_hWnd, GWL_EXSTYLE));
    if ((dwExStyle & WS_EX_CLIENTEDGE) || KindOf(pWnd, &CSplitterWnd::classCSplitterWnd)) {
        InflateRC(rect, kCxBorder2, kCyBorder2);
    }
    RECT rectOld = {};
    ::GetWindowRect(pWnd->m_hWnd, &rectOld);
    HWND hParent = ::GetParent(pWnd->m_hWnd);
    ::ScreenToClient(hParent, reinterpret_cast<POINT*>(&rectOld));
    ::ScreenToClient(hParent, reinterpret_cast<POINT*>(&rectOld) + 1);
    if (!::EqualRect(&rect, &rectOld)) {
        impl__AfxRepositionWindow__YAXPEAUAFX_SIZEPARENTPARAMS__PEAUHWND____PEBUtagRECT___Z(phDWP, pWnd->m_hWnd, &rect);
    }
}

// Cursor cache shared by SetSplitCursor (winsplit.cpp file statics _afx_hcurLast,
// _afx_hcurDestroy, _afx_idcPrimaryLast; mfc140u .data 0x3c4100 / 0x3c40f0 / 0x3c40f8).
HCURSOR  g_hcurLast = nullptr;
HCURSOR  g_hcurDestroy = nullptr;
unsigned g_idcPrimaryLast = 0;

} // namespace

// ===========================================================================
// C++ members declared by include/openmfc/afxole.h.  The exports for these live
// in core/frame/Thunks.cpp and route through them.
// ===========================================================================

// ??0CSplitterWnd@@QEAA@XZ, RVA 0x2aa4b0 (mfc140u): CWnd::CWnd; vfptr; zero-fill
// +0xe8..+0x17c (+0x17c is padding and is not written); m_cxSplitter = m_cySplitter = 7; m_cxBorderShare = m_cyBorderShare = 0;
// m_cxSplitterGap = m_cySplitterGap = 7; m_cxBorder = m_cyBorder = 2.
CSplitterWnd::CSplitterWnd()
    : m_nRows(0), m_nCols(0), m_cxSplitter(7), m_cySplitter(7),
      m_cxBorderShare(0), m_cyBorderShare(0),
      m_cxSplitterGap(7), m_cySplitterGap(7),
      m_nMaxRows(0), m_nMaxCols(0),
      m_nId(0), m_bHasHScroll(FALSE), m_bHasVScroll(FALSE),
      m_pActivePane(nullptr), m_nActiveRow(0), m_nActiveCol(0) {
    // m_sizeMin / m_nId / m_pActivePane / m_nActiveRow / m_nActiveCol are OpenMFC
    // header inventions with no retail counterpart; they are zeroed and otherwise
    // only m_sizeMin and m_nId are ever written (by Create).
    m_sizeMin.cx = 0; m_sizeMin.cy = 0;
    memset(_splitterwnd_padding, 0, sizeof(_splitterwnd_padding));   // the tracking block
    ReleaseExtra(this);   // drop a stale node if this address was recycled without a dtor
    SplitterExtra& x = Extra(this);   // creates the node: cxBorder = cyBorder = 2, arrays NULL
    x.pDynamicViewClass = nullptr;
}
// ??1CSplitterWnd@@UEAA@XZ, RVA 0x2aa590 (mfc140u): vfptr; free(m_pRowInfo);
// free(m_pColInfo) (operator delete[] is a jump to free in this image);
// CWnd::~CWnd().  Retail's CWnd dtor destroys a still-attached window; OpenMFC's
// inline CWnd::~CWnd (OPENMFC_APPCORE_IMPL) only nulls m_hWnd, so the destroy is
// done here through the CWnd::DestroyWindow export (which also drops the
// handle-map entry).  The side node is released AFTER the destroy so that any
// handler run during DestroyWindow cannot re-create (and leak) it.
CSplitterWnd::~CSplitterWnd() {
    if (m_hWnd) impl__DestroyWindow_CWnd__UEAAHXZ(this);
    ReleaseExtra(this);
}

// ?Create@CSplitterWnd@@UEAAHPEAVCWnd@@HHUtagSIZE@@PEAUCCreateContext@@KI@Z, RVA 0x2aa5d0 (mfc140u):
//     m_nMaxRows = nMaxRows; m_nMaxCols = nMaxCols; m_nRows = m_nCols = 1;
//     if (!CreateCommon(pParentWnd, sizeMin, dwStyle, nID)) return FALSE;
//     m_pDynamicViewClass = pContext->m_pNewViewClass;                 // pContext dereferenced unguarded
//     if (!CreateView(0, 0, m_pDynamicViewClass, sizeMin, pContext))   // vtable slot 93
//         { DestroyWindow(); return FALSE; }                            // vtable slot 26
//     m_pColInfo[0].nIdealSize = sizeMin.cx; m_pRowInfo[0].nIdealSize = sizeMin.cy;
//     return TRUE;
// DEVIATION: a NULL pContext returns FALSE instead of faulting.
BOOL CSplitterWnd::Create(CWnd* pParentWnd, int nMaxRows, int nMaxCols,
                           SIZE sizeMin, CCreateContext* pContext, DWORD dwStyle, UINT nID) {
    m_nMaxRows = nMaxRows;
    m_nMaxCols = nMaxCols;
    m_nRows = m_nCols = 1;
    m_sizeMin = sizeMin;   // OpenMFC-only member
    m_nId = static_cast<int>(nID);
    if (!impl__CreateCommon_CSplitterWnd__IEAAHPEAVCWnd__UtagSIZE__KI_Z(this, pParentWnd, sizeMin, dwStyle, nID))
        return FALSE;
    if (!pContext) return FALSE;
    CRuntimeClass* pViewClass = reinterpret_cast<CCreateContextShadow*>(pContext)->m_pNewViewClass;
    Extra(this).pDynamicViewClass = pViewClass;
    if (!CreateView(0, 0, pViewClass, sizeMin, pContext)) {
        impl__DestroyWindow_CWnd__UEAAHXZ(this);
        return FALSE;
    }
    SplitterExtra& x = Extra(this);
    if (x.pColInfo) x.pColInfo[0].nIdealSize = sizeMin.cx;
    if (x.pRowInfo) x.pRowInfo[0].nIdealSize = sizeMin.cy;
    return TRUE;
}
// ?CreateStatic@CSplitterWnd@@UEAAHPEAVCWnd@@HHKI@Z, RVA 0x2aa6a0 (mfc140u):
//     m_nMaxRows = m_nRows = nRows; m_nMaxCols = m_nCols = nCols;
//     return CreateCommon(pParentWnd, CSize(0, 0), dwStyle, nID) != 0;
BOOL CSplitterWnd::CreateStatic(CWnd* pParentWnd, int nRows, int nCols,
                                 DWORD dwStyle, UINT nID) {
    m_nMaxRows = m_nRows = nRows;
    m_nMaxCols = m_nCols = nCols;
    m_nId = static_cast<int>(nID);   // OpenMFC-only member
    SIZE sizeMin = { 0, 0 };
    return impl__CreateCommon_CSplitterWnd__IEAAHPEAVCWnd__UtagSIZE__KI_Z(this, pParentWnd, sizeMin, dwStyle, nID) ? TRUE : FALSE;
}
// ?CreateView@CSplitterWnd@@UEAAHHHPEAUCRuntimeClass@@UtagSIZE@@PEAUCCreateContext@@@Z, RVA 0x2aa8e0 (mfc140u):
//     m_pColInfo[col].nIdealSize = sizeInit.cx; m_pRowInfo[row].nIdealSize = sizeInit.cy;
//     CCreateContext contextT; memset(&contextT, 0, sizeof contextT);
//     BOOL bSendInitialUpdate = FALSE;
//     if (pContext == NULL) {                                  // synthesize one from the active pane
//         CWnd* pOldView = GetActivePane(NULL, NULL);         // vtable slot 104
//         if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView))) {
//             contextT.m_pLastView = (CView*)pOldView;
//             contextT.m_pCurrentDoc = pOldView->m_pDocument;            // +0xe8
//             if (contextT.m_pCurrentDoc != NULL)
//                 contextT.m_pNewDocTemplate = m_pCurrentDoc->m_pDocTemplate;   // +0x50
//         }
//         pContext = &contextT; bSendInitialUpdate = TRUE;
//     }
//     CWnd* pWnd = (CWnd*)pViewClass->CreateObject();
//     if (pWnd == NULL) AfxThrowMemoryException();
//     CRect rect(0, 0, sizeInit.cx, sizeInit.cy);
//     if (!pWnd->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, rect, this, IdFromRowCol(row, col), pContext))  // slot 23
//         return FALSE;                                        // (retail leaks pWnd here)
//     if (bSendInitialUpdate) ::SendMessage(pWnd->m_hWnd, WM_INITIALUPDATE, 0, 0);
//     return TRUE;
// DEVIATIONS: OpenMFC's CDocument has no m_pDocTemplate member, so
// contextT.m_pNewDocTemplate stays NULL; pWnd->Create is the CWnd::Create export
// (not the object's own vtable slot); a NULL pViewClass returns FALSE.
BOOL CSplitterWnd::CreateView(int row, int col, CRuntimeClass* pViewClass,
                               SIZE sizeInit, CCreateContext* pContext) {
    SplitterExtra& x = Extra(this);
    if (x.pColInfo && col >= 0 && col < m_nMaxCols) x.pColInfo[col].nIdealSize = sizeInit.cx;
    if (x.pRowInfo && row >= 0 && row < m_nMaxRows) x.pRowInfo[row].nIdealSize = sizeInit.cy;

    CCreateContextShadow contextT;
    std::memset(&contextT, 0, sizeof(contextT));
    BOOL bSendInitialUpdate = FALSE;
    if (pContext == nullptr) {
        CWnd* pOldView = static_cast<CWnd*>(impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(this, nullptr, nullptr));
        if (pOldView != nullptr && KindOf(pOldView, RtcView())) {
            contextT.m_pLastView = static_cast<CView*>(pOldView);
            contextT.m_pCurrentDoc = static_cast<CView*>(pOldView)->m_pDocument;
            // contextT.m_pNewDocTemplate = m_pCurrentDoc->m_pDocTemplate;  -- no such member in OpenMFC
        }
        pContext = reinterpret_cast<CCreateContext*>(&contextT);
        bSendInitialUpdate = TRUE;
    }
    if (!pViewClass) return FALSE;
    CWnd* pWnd = static_cast<CWnd*>(impl__CreateObject_CRuntimeClass__QEAAPEAVCObject__XZ(pViewClass));
    if (pWnd == nullptr) impl__AfxThrowMemoryException__YAXXZ();

    RECT rect = { 0, 0, sizeInit.cx, sizeInit.cy };
    if (!impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
            pWnd, nullptr, nullptr, WS_CHILD | WS_VISIBLE, rect, this,
            static_cast<UINT>(IdFromRowColImpl(row, col)), pContext)) {
        return FALSE;
    }
    if (bSendInitialUpdate) ::SendMessageW(pWnd->m_hWnd, kWmInitialUpdate, 0, 0);
    return TRUE;
}
// ?GetPane@CSplitterWnd@@QEBAPEAVCWnd@@HH@Z, RVA 0x2aab40 (mfc140u):
//     return GetDlgItem(IdFromRowCol(row, col));     (tail jump into CWnd::GetDlgItem)
CWnd* CSplitterWnd::GetPane(int row, int col) const {
    return PaneAt(this, row, col);
}
// ?GetRowInfo@CSplitterWnd@@QEBAXHAEAH0@Z, RVA 0x2aabe0 (mfc140u):
//     cyCur = m_pRowInfo[row].nCurSize; cyMin = m_pRowInfo[row].nMinSize;
// DEVIATION: retail indexes m_pRowInfo unguarded; range and NULL are guarded here.
void CSplitterWnd::GetRowInfo(int row, int& cyCur, int& cyMin) const {
    const SplitterExtra& x = Extra(this);
    if (!x.pRowInfo || row < 0 || row >= m_nMaxRows) { cyCur = 0; cyMin = 0; return; }
    cyCur = x.pRowInfo[row].nCurSize;
    cyMin = x.pRowInfo[row].nMinSize;
}
// ?SetRowInfo@CSplitterWnd@@QEAAXHHH@Z, RVA 0x2aac10 (mfc140u):
//     m_pRowInfo[row].nIdealSize = cyIdeal; m_pRowInfo[row].nMinSize = cyMin;
void CSplitterWnd::SetRowInfo(int row, int cyIdeal, int cyMin) {
    SplitterExtra& x = Extra(this);
    if (!x.pRowInfo || row < 0 || row >= m_nMaxRows) return;   // retail: no check
    x.pRowInfo[row].nIdealSize = cyIdeal;
    x.pRowInfo[row].nMinSize = cyMin;
}
// ?GetColumnInfo@CSplitterWnd@@QEBAXHAEAH0@Z, RVA 0x2aac30 (mfc140u):
//     cxCur = m_pColInfo[col].nCurSize; cxMin = m_pColInfo[col].nMinSize;
void CSplitterWnd::GetColumnInfo(int col, int& cxCur, int& cxMin) const {
    const SplitterExtra& x = Extra(this);
    if (!x.pColInfo || col < 0 || col >= m_nMaxCols) { cxCur = 0; cxMin = 0; return; }
    cxCur = x.pColInfo[col].nCurSize;
    cxMin = x.pColInfo[col].nMinSize;
}
// ?SetColumnInfo@CSplitterWnd@@QEAAXHHH@Z, RVA 0x2aac60 (mfc140u):
//     m_pColInfo[col].nIdealSize = cxIdeal; m_pColInfo[col].nMinSize = cxMin;
void CSplitterWnd::SetColumnInfo(int col, int cxIdeal, int cxMin) {
    SplitterExtra& x = Extra(this);
    if (!x.pColInfo || col < 0 || col >= m_nMaxCols) return;   // retail: no check
    x.pColInfo[col].nIdealSize = cxIdeal;
    x.pColInfo[col].nMinSize = cxMin;
}
// ?RecalcLayout@CSplitterWnd@@UEAAXXZ, RVA 0x2ac6d0 (mfc140u):
//     CRect rectClient; GetClientRect(rectClient); rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
//     CRect rectInside; GetInsideRect(rectInside);                          // vtable slot 112
//     LayoutRowCol(m_pColInfo, m_nCols, rectInside.Width(),  m_cxSplitterGap);
//     LayoutRowCol(m_pRowInfo, m_nRows, rectInside.Height(), m_cySplitterGap);
//     HDWP hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);
//     int cx = rectClient.right - rectInside.right, cy = rectClient.bottom - rectInside.bottom;
//     if (m_bHasHScroll) {
//         if (m_bHasVScroll) {                                                // the size box
//             CWnd* pScrollBar = GetDlgItem(AFX_IDW_SIZE_BOX);
//             CWnd* pSizingParent = GetSizingParent();
//             if (pScrollBar->ModifyStyle(SBS_SIZEBOX|SBS_SIZEGRIP, pSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX, 0))
//                 ::InvalidateRect(pScrollBar->m_hWnd, NULL, TRUE);
//             pScrollBar->EnableWindow(pSizingParent != NULL);
//             DeferClientPos(&hDWP, pScrollBar, rectInside.right, rectInside.bottom, cx, cy, TRUE);
//         }
//         int x = rectClient.left;
//         for (col = 0; col < m_nCols; col++) {                               // horizontal bars
//             int cxCol = m_pColInfo[col].nCurSize;
//             if (col == 0 && m_nCols < m_nMaxCols) { x += m_cxSplitter; cxCol -= m_cxSplitter; }   // room for the split box
//             DeferClientPos(&hDWP, GetDlgItem(AFX_IDW_HSCROLL_FIRST + col), x, rectInside.bottom, cxCol, cy, TRUE);
//             x += cxCol + m_cxSplitterGap;
//         }
//     }
//     if (m_bHasVScroll) {                                                    // vertical bars
//         int y = rectClient.top;
//         for (row = 0; row < m_nRows; row++) {
//             int cyRow = m_pRowInfo[row].nCurSize;
//             if (row == 0 && m_nRows < m_nMaxRows) { y += m_cySplitter; cyRow -= m_cySplitter; }
//             DeferClientPos(&hDWP, GetDlgItem(AFX_IDW_VSCROLL_FIRST + row), rectInside.right, y, cx, cyRow, TRUE);
//             y += cyRow + m_cySplitterGap;
//         }
//     }
//     int x = rectClient.left;                                                // the panes
//     for (col = 0; col < m_nCols; col++) {
//         int cxCol = m_pColInfo[col].nCurSize; int y = rectClient.top;
//         for (row = 0; row < m_nRows; row++) {
//             int cyRow = m_pRowInfo[row].nCurSize;
//             DeferClientPos(&hDWP, GetDlgItem(IdFromRowCol(row, col)), x, y, cxCol, cyRow, FALSE);
//             y += cyRow + m_cySplitterGap;
//         }
//         x += cxCol + m_cxSplitterGap;
//     }
//     if (hDWP != NULL) ::EndDeferWindowPos(hDWP);
//     DrawAllSplitBars(NULL, rectInside.right, rectInside.bottom);           // vtable slot 116 (invalidate)
void CSplitterWnd::RecalcLayout() {
    if (!m_hWnd) return;   // retail calls GetClientRect on m_hWnd unguarded; nothing to lay out without a window
    SplitterExtra& x = Extra(this);
    if (!x.pColInfo || !x.pRowInfo) return;   // arrays exist once CreateCommon has run

    RECT rectClient = {};
    ::GetClientRect(m_hWnd, &rectClient);
    InflateRC(rectClient, -x.cxBorder, -x.cyBorder);
    RECT rectInside = {};
    impl__GetInsideRect_CSplitterWnd__MEBAXAEAVCRect___Z(this, &rectInside);

    LayoutRowCol(x.pColInfo, m_nCols, rectInside.right - rectInside.left, m_cxSplitterGap);
    LayoutRowCol(x.pRowInfo, m_nRows, rectInside.bottom - rectInside.top, m_cySplitterGap);

    HDWP hDWP = ::BeginDeferWindowPos((m_nCols + 1) * (m_nRows + 1) + 1);
    const int cx = rectClient.right - rectInside.right;
    const int cy = rectClient.bottom - rectInside.bottom;
    int row, col;

    if (m_bHasHScroll) {
        if (m_bHasVScroll) {
            CWnd* pScrollBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(this, static_cast<int>(kIdwSizeBox));
            CWnd* pSizingParent = impl__GetSizingParent_CSplitterWnd__IEAAPEAVCWnd__XZ(this);
            if (pScrollBar) {
                if (impl__ModifyStyle_CWnd__QEAAHKKI_Z(pScrollBar, SBS_SIZEBOX | SBS_SIZEGRIP,
                                                        pSizingParent ? SBS_SIZEGRIP : SBS_SIZEBOX, 0)) {
                    ::InvalidateRect(pScrollBar->m_hWnd, nullptr, TRUE);
                }
                ::EnableWindow(pScrollBar->m_hWnd, pSizingParent != nullptr);
                DeferClientPos(&hDWP, pScrollBar, rectInside.right, rectInside.bottom, cx, cy, TRUE);
            }
        }
        int xx = rectClient.left;
        for (col = 0; col < m_nCols; col++) {
            CWnd* pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(this, static_cast<int>(kIdwHScrollFirst) + col);
            int cxCol = x.pColInfo[col].nCurSize;
            if (col == 0 && m_nCols < m_nMaxCols) {
                xx += m_cxSplitter;
                cxCol -= m_cxSplitter;
            }
            DeferClientPos(&hDWP, pBar, xx, rectInside.bottom, cxCol, cy, TRUE);
            xx += cxCol + m_cxSplitterGap;
        }
    }
    if (m_bHasVScroll) {
        int yy = rectClient.top;
        for (row = 0; row < m_nRows; row++) {
            CWnd* pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(this, static_cast<int>(kIdwVScrollFirst) + row);
            int cyRow = x.pRowInfo[row].nCurSize;
            if (row == 0 && m_nRows < m_nMaxRows) {
                yy += m_cySplitter;
                cyRow -= m_cySplitter;
            }
            DeferClientPos(&hDWP, pBar, rectInside.right, yy, cx, cyRow, TRUE);
            yy += cyRow + m_cySplitterGap;
        }
    }
    int xx = rectClient.left;
    for (col = 0; col < m_nCols; col++) {
        const int cxCol = x.pColInfo[col].nCurSize;
        int yy = rectClient.top;
        for (row = 0; row < m_nRows; row++) {
            const int cyRow = x.pRowInfo[row].nCurSize;
            DeferClientPos(&hDWP, PaneAt(this, row, col), xx, yy, cxCol, cyRow, FALSE);
            yy += cyRow + m_cySplitterGap;
        }
        xx += cxCol + m_cxSplitterGap;
    }
    if (hDWP != nullptr) ::EndDeferWindowPos(hDWP);
    impl__DrawAllSplitBars_CSplitterWnd__MEAAXPEAVCDC__HH_Z(this, nullptr, rectInside.right, rectInside.bottom);
}
// ?SetSplitCursor@CSplitterWnd@@MEAAXH@Z, RVA 0x2acfc0 (mfc140u):
//     AfxLockGlobals(CRIT_SPLITTERWND);
//     UINT idcPrimary; LPCTSTR idcSecondary;
//     if (ht == vSplitterBox || vSplitterBar1..15)      { AFX_IDC_VSPLITBAR,   IDC_SIZENS  }
//     else if (ht == hSplitterBox || hSplitterBar1..15) { AFX_IDC_HSPLITBAR,   IDC_SIZEWE  }
//     else if (ht == bothSplitterBox || intersection)   { AFX_IDC_SMALLARROWS, IDC_SIZEALL }
//     else { ::SetCursor(afxData.hcurArrow); goto unlock; }
//     HCURSOR hcurToDestroy = NULL;
//     if (idcPrimary != _afx_idcPrimaryLast) {
//         HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(idcPrimary), RT_GROUP_CURSOR);
//         hcurToDestroy = _afx_hcurDestroy;
//         if ((_afx_hcurDestroy = _afx_hcurLast = ::LoadCursor(hInst, MAKEINTRESOURCE(idcPrimary))) == NULL)
//             _afx_hcurLast = ::LoadCursor(NULL, idcSecondary);       // system alternative
//         _afx_idcPrimaryLast = idcPrimary;
//     }
//     ::SetCursor(_afx_hcurLast);
//     if (hcurToDestroy != NULL) ::DestroyCursor(hcurToDestroy);       // destroy after being set
//     AfxUnlockGlobals(CRIT_SPLITTERWND);
// afxData.hcurArrow is a zero slot in OpenMFC's afxData blob, so the arrow is loaded directly.
void CSplitterWnd::SetSplitCursor(int ht) {
    impl__AfxLockGlobals__YAXH_Z(kCritSplitterWnd);
    unsigned idcPrimary = 0;
    LPCWSTR idcSecondary = nullptr;
    if (ht == kVSplitterBox || IsVBar(ht)) {
        idcPrimary = kIdcVSplitBar;   idcSecondary = IDC_SIZENS;
    } else if (ht == kHSplitterBox || IsHBar(ht)) {
        idcPrimary = kIdcHSplitBar;   idcSecondary = IDC_SIZEWE;
    } else if (ht == kBothSplitterBox || IsIsect(ht)) {
        idcPrimary = kIdcSmallArrows; idcSecondary = IDC_SIZEALL;
    } else {
        ::SetCursor(::LoadCursorW(nullptr, IDC_ARROW));
    }
    if (idcPrimary != 0) {
        HCURSOR hcurToDestroy = nullptr;
        if (idcPrimary != g_idcPrimaryLast) {
            HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
                MAKEINTRESOURCEW(idcPrimary), MAKEINTRESOURCEW(12) /* RT_GROUP_CURSOR */);
            hcurToDestroy = g_hcurDestroy;
            g_hcurDestroy = g_hcurLast = ::LoadCursorW(hInst, MAKEINTRESOURCEW(idcPrimary));
            if (g_hcurLast == nullptr) {
                g_hcurLast = ::LoadCursorW(nullptr, idcSecondary);
            }
            g_idcPrimaryLast = idcPrimary;
        }
        ::SetCursor(g_hcurLast);
        if (hcurToDestroy != nullptr) ::DestroyCursor(hcurToDestroy);
    }
    impl__AfxUnlockGlobals__YAXH_Z(kCritSplitterWnd);
}
// OpenMFC header oddity: retail's GetActivePane(int*, int*) returns CWnd* and is
// exported from this file (see the thunk below).  This int-returning const
// variant exists only in include/openmfc/afxole.h; it reports the pane index
// (row * m_nCols + col) of the retail result, or -1.
int CSplitterWnd::GetActivePane(int* pRow, int* pCol) const {
    int row = -1, col = -1;
    CWnd* pPane = static_cast<CWnd*>(impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(
        const_cast<CSplitterWnd*>(this), &row, &col));
    if (pRow) *pRow = row;
    if (pCol) *pCol = col;
    if (!pPane || row < 0 || col < 0) return -1;
    return row * m_nCols + col;
}
// ?SetActivePane@CSplitterWnd@@UEAAXHHPEAVCWnd@@@Z, RVA 0x2ae040 (mfc140u):
//     if (pWnd == NULL) pWnd = GetDlgItem(IdFromRowCol(row, col));
//     if (pWnd->IsKindOf(RUNTIME_CLASS(CView))) {                     // unguarded deref in retail
//         CFrameWnd* pFrameWnd = GetParentFrame(); ENSURE_VALID(pFrameWnd);   // throws CInvalidArgException
//         pFrameWnd->SetActiveView((CView*)pWnd, TRUE);
//     } else pWnd->SetFocus();
// DEVIATION: a NULL pWnd (no such pane) returns instead of faulting.
void CSplitterWnd::SetActivePane(int row, int col, CWnd* pWnd) {
    if (pWnd == nullptr) pWnd = PaneAt(this, row, col);
    if (pWnd == nullptr) return;
    if (KindOf(pWnd, RtcView())) {
        CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(this);
        if (!pFrameWnd) impl__AfxThrowInvalidArgException__YAXXZ();
        impl__SetActiveView_CFrameWnd__QEAAXPEAVCView__H_Z(pFrameWnd, static_cast<CView*>(pWnd), TRUE);
    } else {
        ::SetFocus(pWnd->m_hWnd);
    }
}
// OpenMFC header-only overload: retail has no argument-less GetActivePane.
CWnd* CSplitterWnd::GetActivePane() {
    return static_cast<CWnd*>(impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(this, nullptr, nullptr));
}
// ?CanActivateNext@CSplitterWnd@@UEAAHH@Z, RVA 0x2adf40 (mfc140u):
//     if (GetActivePane(NULL, NULL) == NULL) return FALSE;             // vtable slot 104
//     return m_nRows > 1 || m_nCols > 1;
BOOL CSplitterWnd::CanActivateNext(BOOL bPrev) {
    (void)bPrev;   // retail ignores it too
    if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(this, nullptr, nullptr) == nullptr) return FALSE;
    return (m_nRows > 1 || m_nCols > 1) ? TRUE : FALSE;
}
// ?ActivateNext@CSplitterWnd@@UEAAXH@Z, RVA 0x2adf80 (mfc140u):
//     int row, col;
//     if (GetActivePane(&row, &col) == NULL) return;                   // vtable slot 104
//     if (bPrev) {                                                     // prev: column first, wrapping
//         if (--col < 0) { col = m_nCols - 1; if (--row < 0) row = m_nRows - 1; }
//     } else {
//         if (++col >= m_nCols) { col = 0; if (++row >= m_nRows) row = 0; }
//     }
//     SetActivePane(row, col, NULL);                                   // vtable slot 105
void CSplitterWnd::ActivateNext(BOOL bPrev) {
    int row = 0, col = 0;
    if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(this, &row, &col) == nullptr) return;
    if (bPrev) {
        if (--col < 0) {
            col = m_nCols - 1;
            if (--row < 0) row = m_nRows - 1;
        }
    } else {
        if (++col >= m_nCols) {
            col = 0;
            if (++row >= m_nRows) row = 0;
        }
    }
    SetActivePane(row, col, nullptr);
}
// ?DoKeyboardSplit@CSplitterWnd@@UEAAHXZ, RVA 0x2ac210 (mfc140u):
//     int ht;
//     if (m_nRows > 1)       ht = (m_nCols > 1) ? splitterIntersection1 : vSplitterBar1;
//     else if (m_nCols > 1)  ht = hSplitterBar1;
//     else if (m_nMaxRows > 1) ht = (m_nMaxCols > 1) ? bothSplitterBox : vSplitterBox;
//     else if (m_nMaxCols > 1) ht = hSplitterBox;
//     else return FALSE;
//     StartTracking(ht);                                                // vtable slot 118
//     CRect rect;                                                       // then centre the cursor in the tracker
//     rect.left = m_rectTracker.Width() / 2;  rect.top = m_rectTracker.Height() / 2;  rect.right = rect.bottom = 0;
//     if (m_ptTrackOffset.y != 0) rect.top  = m_rectTracker.top;
//     if (m_ptTrackOffset.x != 0) rect.left = m_bTracking2 ? m_rectTracker2.left : m_rectTracker.left;
//     rect.OffsetRect(-m_ptTrackOffset.x, -m_ptTrackOffset.y);
//     ClientToScreen(&rect); ::SetCursorPos(rect.left, rect.top);
//     return TRUE;
BOOL CSplitterWnd::DoKeyboardSplit() {
    int ht;
    if (m_nRows > 1)          ht = (m_nCols > 1) ? kSplitterIntersection1 : kVSplitterBar1;
    else if (m_nCols > 1)     ht = kHSplitterBar1;
    else if (m_nMaxRows > 1)  ht = (m_nMaxCols > 1) ? kBothSplitterBox : kVSplitterBox;
    else if (m_nMaxCols > 1)  ht = kHSplitterBox;
    else return FALSE;

    impl__StartTracking_CSplitterWnd__MEAAXH_Z(this, ht);

    const SplitterTrackState& t = Track(this);
    RECT rect = {};
    rect.left = (t.m_rectTracker.right - t.m_rectTracker.left) / 2;
    rect.top  = (t.m_rectTracker.bottom - t.m_rectTracker.top) / 2;
    if (t.m_ptTrackOffset.y != 0) rect.top = t.m_rectTracker.top;
    if (t.m_ptTrackOffset.x != 0) rect.left = t.m_bTracking2 ? t.m_rectTracker2.left : t.m_rectTracker.left;
    ::OffsetRect(&rect, -t.m_ptTrackOffset.x, -t.m_ptTrackOffset.y);
    ::ClientToScreen(m_hWnd, reinterpret_cast<POINT*>(&rect));
    ::ClientToScreen(m_hWnd, reinterpret_cast<POINT*>(&rect) + 1);
    ::SetCursorPos(rect.left, rect.top);
    return TRUE;
}
// C++ face of the export defined below (no virtual dispatch; see file header).
void CSplitterWnd::OnDrawSplitter(CDC* pDC, int nType, const CRect& rect) {
    RECT rc = { rect.left, rect.top, rect.right, rect.bottom };
    impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(this, pDC, nType, &rc);
}
// ?OnInvertTracker@CSplitterWnd@@MEAAXAEBVCRect@@@Z, RVA 0x2ac170 (mfc140u); the export
// in core/frame/Thunks.cpp routes through this member:
//     CDC* pDC = CDC::FromHandle(::GetDC(m_hWnd));
//     CBrush* pBrush = CDC::GetHalftoneBrush();
//     HBRUSH hOldBrush = NULL;
//     if (pBrush != NULL) hOldBrush = (HBRUSH)::SelectObject(pDC->m_hDC, pBrush->m_hObject);
//     ::PatBlt(pDC->m_hDC, rect.left, rect.top, rect.Width(), rect.Height(), PATINVERT);
//     if (hOldBrush != NULL) ::SelectObject(pDC->m_hDC, hOldBrush);
//     ::ReleaseDC(m_hWnd, pDC->m_hDC);
// The temporary CDC wrapper is not needed for this: the HDC is used directly.
// (OpenMFC's CDC::GetHalftoneBrush hands back the DKGRAY stock brush, not the
// 8x8 halftone pattern retail builds -- that difference lives in core/gdi/CDC.cpp.)
void CSplitterWnd::OnInvertTracker(const CRect& rect) {
    if (!m_hWnd) return;
    HDC hDC = ::GetDC(m_hWnd);
    if (!hDC) return;
    CBrush* pBrush = impl__GetHalftoneBrush_CDC__SAPEAVCBrush__XZ();
    HGDIOBJ hOldBrush = nullptr;
    if (pBrush != nullptr) hOldBrush = ::SelectObject(hDC, pBrush->m_hObject);
    ::PatBlt(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, PATINVERT);
    if (hOldBrush != nullptr) ::SelectObject(hDC, hOldBrush);
    ::ReleaseDC(m_hWnd, hDC);
}
// OpenMFC header invention: retail CSplitterWnd has no OnCreateClient (that is a
// CFrameWnd overridable).  Nothing to transcribe; reports whether a window exists.
BOOL CSplitterWnd::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) {
    (void)lpcs;
    (void)pContext;
    return m_hWnd != nullptr;
}

// ===========================================================================
// Exports whose bodies live here.
// ===========================================================================

// === Moved from ManualThunks.cpp ===
// ?GetActivePane@CSplitterWnd@@UEAAPEAVCWnd@@PEAH0@Z, RVA 0x2ae0c0 (mfc140u):
//     CFrameWnd* pFrameWnd = GetParentFrame(); ENSURE_VALID(pFrameWnd);   // throws CInvalidArgException
//     CWnd* pView = pFrameWnd->m_pViewActive;                            // inlined GetActiveView(), +0x170
//     if (pView == NULL) { pView = CWnd::FromHandle(::GetFocus()); if (pView == NULL) return NULL; }
//     if (!IsChildPane(pView, pRow, pCol)) pView = NULL;                 // pRow/pCol written only on this path
//     return pView;
// Symbol: ?GetActivePane@CSplitterWnd@@UEAAPEAVCWnd@@PEAH0@Z
extern "C" void* MS_ABI impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(CSplitterWnd* pThis, int* pRow, int* pCol) {
    if (!pThis) return nullptr;
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (!pFrameWnd) impl__AfxThrowInvalidArgException__YAXXZ();
    CWnd* pView = pFrameWnd->m_pViewActive;
    if (pView == nullptr) {
        pView = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus());
        if (pView == nullptr) return nullptr;
    }
    if (!impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(pThis, pView, pRow, pCol)) pView = nullptr;
    return pView;
}

// ?OnDrawSplitter@CSplitterWnd@@MEAAXPEAVCDC@@W4ESplitType@1@AEBVCRect@@@Z, RVA 0x2aaf00 (mfc140u):
//     if (pDC == NULL) { ::RedrawWindow(m_hWnd, &rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN); return; }
//     CRect rect = rectArg;
//     switch (nType) {
//     case splitBorder:
//         pDC->Draw3dRect(rect, afxData.clrBtnShadow, afxData.clrBtnHilite);
//         rect.InflateRect(-CX_BORDER, -CY_BORDER);
//         pDC->Draw3dRect(rect, afxData.clrWindowFrame, afxData.clrBtnFace);
//         return;
//     case splitBox:
//         pDC->Draw3dRect(rect, afxData.clrBtnFace, afxData.clrWindowFrame);
//         rect.InflateRect(-CX_BORDER, -CY_BORDER);
//         pDC->Draw3dRect(rect, afxData.clrBtnHilite, afxData.clrBtnShadow);
//         rect.InflateRect(-CX_BORDER, -CY_BORDER);
//         break;
//     default: break;                       // splitBar / splitIntersection: just the fill
//     }
//     pDC->FillSolidRect(rect, afxData.clrBtnFace);
// afxData.clr* are the cached COLOR_BTNFACE/BTNSHADOW/BTNHIGHLIGHT/WINDOWFRAME
// (afxData+0x30/+0x34/+0x38/+0x40); OpenMFC's afxData blob is zero, so
// ::GetSysColor is read directly.
// Symbol: ?OnDrawSplitter@CSplitterWnd@@MEAAXPEAVCDC@@W4ESplitType@1@AEBVCRect@@@Z
extern "C" void MS_ABI impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(CSplitterWnd* pThis, CDC* pDC, int nType, const RECT* pRect) {
    if (!pThis || !pRect) return;
    if (pDC == nullptr) {
        if (pThis->m_hWnd) ::RedrawWindow(pThis->m_hWnd, pRect, nullptr, RDW_INVALIDATE | RDW_NOCHILDREN);
        return;
    }
    const COLORREF clrBtnFace     = ::GetSysColor(COLOR_BTNFACE);
    const COLORREF clrBtnShadow   = ::GetSysColor(COLOR_BTNSHADOW);
    const COLORREF clrBtnHilite   = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    const COLORREF clrWindowFrame = ::GetSysColor(COLOR_WINDOWFRAME);
    RECT rect = *pRect;
    if (nType == kSplitBorder) {
        impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, clrBtnShadow, clrBtnHilite);
        InflateRC(rect, -kCxBorder, -kCyBorder);
        impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, clrWindowFrame, clrBtnFace);
        return;
    }
    if (nType == kSplitBox) {
        impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, clrBtnFace, clrWindowFrame);
        InflateRC(rect, -kCxBorder, -kCyBorder);
        impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, &rect, clrBtnHilite, clrBtnShadow);
        InflateRC(rect, -kCxBorder, -kCyBorder);
    }
    impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rect, clrBtnFace);
}

// ?CreateCommon@CSplitterWnd@@IEAAHPEAVCWnd@@UtagSIZE@@KI@Z, RVA 0x2aa6f0 (mfc140u):
//     AfxEndDeferRegisterClass(AFX_WNDMDIFRAME_REG);
//     if (!CreateEx(0, _afxWndMDIFrame /* L"AfxMDIFrame140u" */, NULL,        // vtable slot 25
//                   dwStyle & ~(WS_HSCROLL|WS_VSCROLL|WS_BORDER), 0, 0, 0, 0,
//                   pParentWnd->m_hWnd, (HMENU)(UINT_PTR)nID, NULL))
//         return FALSE;
//     m_pColInfo = new CRowColInfo[m_nMaxCols];
//     for (col...) { nMinSize = nIdealSize = sizeMin.cx; nCurSize = -1; }
//     m_pRowInfo = new CRowColInfo[m_nMaxRows];
//     for (row...) { nMinSize = nIdealSize = sizeMin.cy; nCurSize = -1; }
//     SetScrollStyle(dwStyle);
//     return TRUE;
// DEVIATIONS: OpenMFC registers a single window class (AfxEndDeferRegisterClass in
// featurepack/CMFC_misc_stubs.cpp ignores the bit), so the class name is passed as
// NULL and CWnd::CreateEx substitutes it; the arrays are malloc'ed (retail: operator
// new[], freed with free()); arrays from an earlier Create are released first where
// retail overwrites the pointers without freeing them; a NULL pParentWnd returns FALSE.
// Symbol: ?CreateCommon@CSplitterWnd@@IEAAHPEAVCWnd@@UtagSIZE@@KI@Z
extern "C" int MS_ABI impl__CreateCommon_CSplitterWnd__IEAAHPEAVCWnd__UtagSIZE__KI_Z(CSplitterWnd* pThis, CWnd* pParentWnd, SIZE sizeMin, unsigned long dwStyle, unsigned int nID) {
    if (!pThis || !pParentWnd) return FALSE;
    impl__AfxEndDeferRegisterClass__YAHJ_Z(kAfxWndMdiFrameReg);
    if (!impl__CreateEx_CWnd__UEAAHKPEB_W0KHHHHPEAUHWND____PEAUHMENU____PEAX_Z(
            pThis, 0, nullptr, nullptr, dwStyle & ~(WS_HSCROLL | WS_VSCROLL | WS_BORDER),
            0, 0, 0, 0, pParentWnd->m_hWnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)), nullptr)) {
        return FALSE;
    }
    SplitterExtra& x = Extra(pThis);
    std::free(x.pColInfo); x.pColInfo = nullptr;
    std::free(x.pRowInfo); x.pRowInfo = nullptr;
    const int nMaxCols = pThis->m_nMaxCols > 0 ? pThis->m_nMaxCols : 0;
    const int nMaxRows = pThis->m_nMaxRows > 0 ? pThis->m_nMaxRows : 0;
    x.pColInfo = static_cast<CRowColInfo*>(std::malloc(sizeof(CRowColInfo) * (nMaxCols ? nMaxCols : 1)));
    if (!x.pColInfo) impl__AfxThrowMemoryException__YAXXZ();
    for (int col = 0; col < nMaxCols; col++) {
        x.pColInfo[col].nMinSize = x.pColInfo[col].nIdealSize = sizeMin.cx;
        x.pColInfo[col].nCurSize = -1;
    }
    x.pRowInfo = static_cast<CRowColInfo*>(std::malloc(sizeof(CRowColInfo) * (nMaxRows ? nMaxRows : 1)));
    if (!x.pRowInfo) impl__AfxThrowMemoryException__YAXXZ();
    for (int row = 0; row < nMaxRows; row++) {
        x.pRowInfo[row].nMinSize = x.pRowInfo[row].nIdealSize = sizeMin.cy;
        x.pRowInfo[row].nCurSize = -1;
    }
    impl__SetScrollStyle_CSplitterWnd__QEAAXK_Z(pThis, dwStyle);
    return TRUE;
}

// ?CreateScrollBarCtrl@CSplitterWnd@@UEAAHKI@Z, RVA 0x2aaaa0 (mfc140u):
//     HWND hWnd = AfxCtxCreateWindowEx(0, L"SCROLLBAR", NULL, dwStyle | WS_VISIBLE | WS_CHILD,
//                     0, 0, 1, 1, m_hWnd, (HMENU)(UINT_PTR)nID,
//                     AfxGetModuleState()->m_hCurrentInstanceHandle, NULL);
//     return hWnd != NULL;
// (AfxCtxCreateWindowEx is the activation-context wrapper around ::CreateWindowExW;
// the module-state instance handle is AfxGetInstanceHandle() here.)
// Symbol: ?CreateScrollBarCtrl@CSplitterWnd@@UEAAHKI@Z
extern "C" int MS_ABI impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(CSplitterWnd* pThis, unsigned long dwStyle, unsigned int nID) {
    if (!pThis) return FALSE;
    HWND hWnd = ::CreateWindowExW(0, L"SCROLLBAR", nullptr, dwStyle | WS_VISIBLE | WS_CHILD,
                                  0, 0, 1, 1, pThis->m_hWnd, reinterpret_cast<HMENU>(static_cast<UINT_PTR>(nID)),
                                  AfxGetInstanceHandle(), nullptr);
    return hWnd != nullptr ? TRUE : FALSE;
}

// ?DeleteColumn@CSplitterWnd@@UEAAXH@Z, RVA 0x2ab500 (mfc140u):
//     int rowActive, colActive;
//     if (GetActivePane(&rowActive, &colActive) != NULL && colActive == colDelete) {   // slot 104
//         if (++colActive >= m_nCols) colActive = 0;
//         SetActivePane(rowActive, colActive, NULL);                                   // slot 105
//     }
//     CWnd* pScrollDel = m_bHasHScroll ? GetDlgItem(AFX_IDW_HSCROLL_FIRST + colDelete) : NULL;
//     for (row = 0; row < m_nRows; row++) {
//         DeleteView(row, colDelete);                                                  // slot 99
//         for (col = colDelete + 1; col < m_nCols; col++) {
//             GetDlgItem(IdFromRowCol(row, col))->SetDlgCtrlID(IdFromRowCol(row, col - 1));
//             if (m_bHasHScroll && row == m_nRows - 1) {
//                 CWnd* pScroll = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
//                 if (pScroll != NULL) pScroll->SetDlgCtrlID(AFX_IDW_HSCROLL_FIRST + col - 1);
//             }
//         }
//     }
//     m_nCols--;
//     if (pScrollDel != NULL) pScrollDel->DestroyWindow();                              // slot 26
//     RecalcLayout();                                                                  // slot 94
// Symbol: ?DeleteColumn@CSplitterWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__DeleteColumn_CSplitterWnd__UEAAXH_Z(CSplitterWnd* pThis, int colDelete) {
    if (!pThis) return;
    int rowActive = 0, colActive = 0;
    if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, &rowActive, &colActive) != nullptr &&
        colActive == colDelete) {
        if (++colActive >= pThis->m_nCols) colActive = 0;
        pThis->SetActivePane(rowActive, colActive, nullptr);
    }
    CWnd* pScrollDel = pThis->m_bHasHScroll
        ? impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwHScrollFirst) + colDelete) : nullptr;
    for (int row = 0; row < pThis->m_nRows; row++) {
        impl__DeleteView_CSplitterWnd__UEAAXHH_Z(pThis, row, colDelete);
        for (int col = colDelete + 1; col < pThis->m_nCols; col++) {
            CWnd* pPane = PaneAt(pThis, row, col);
            if (pPane && pPane->m_hWnd)   // retail calls SetDlgCtrlID on it unguarded
                ::SetWindowLongPtrW(pPane->m_hWnd, GWLP_ID, IdFromRowColImpl(row, col - 1));
            if (pThis->m_bHasHScroll && row == pThis->m_nRows - 1) {
                CWnd* pScroll = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwHScrollFirst) + col);
                if (pScroll != nullptr && pScroll->m_hWnd)
                    ::SetWindowLongPtrW(pScroll->m_hWnd, GWLP_ID, static_cast<int>(kIdwHScrollFirst) + col - 1);
            }
        }
    }
    pThis->m_nCols--;
    if (pScrollDel != nullptr) impl__DestroyWindow_CWnd__UEAAHXZ(pScrollDel);
    pThis->RecalcLayout();
}

// ?DeleteRow@CSplitterWnd@@UEAAXH@Z, RVA 0x2ab360 (mfc140u): the row-wise twin of
// DeleteColumn above (AFX_IDW_VSCROLL_FIRST, m_bHasVScroll, rows re-numbered with
// IdFromRowCol(row - 1, col), the last column's vertical bar re-numbered).
// Symbol: ?DeleteRow@CSplitterWnd@@UEAAXH@Z
extern "C" void MS_ABI impl__DeleteRow_CSplitterWnd__UEAAXH_Z(CSplitterWnd* pThis, int rowDelete) {
    if (!pThis) return;
    int rowActive = 0, colActive = 0;
    if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, &rowActive, &colActive) != nullptr &&
        rowActive == rowDelete) {
        if (++rowActive >= pThis->m_nRows) rowActive = 0;
        pThis->SetActivePane(rowActive, colActive, nullptr);
    }
    CWnd* pScrollDel = pThis->m_bHasVScroll
        ? impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwVScrollFirst) + rowDelete) : nullptr;
    for (int col = 0; col < pThis->m_nCols; col++) {
        impl__DeleteView_CSplitterWnd__UEAAXHH_Z(pThis, rowDelete, col);
        for (int row = rowDelete + 1; row < pThis->m_nRows; row++) {
            CWnd* pPane = PaneAt(pThis, row, col);
            if (pPane && pPane->m_hWnd)   // retail calls SetDlgCtrlID on it unguarded
                ::SetWindowLongPtrW(pPane->m_hWnd, GWLP_ID, IdFromRowColImpl(row - 1, col));
            if (pThis->m_bHasVScroll && col == pThis->m_nCols - 1) {
                CWnd* pScroll = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwVScrollFirst) + row);
                if (pScroll != nullptr && pScroll->m_hWnd)
                    ::SetWindowLongPtrW(pScroll->m_hWnd, GWLP_ID, static_cast<int>(kIdwVScrollFirst) + row - 1);
            }
        }
    }
    pThis->m_nRows--;
    if (pScrollDel != nullptr) impl__DestroyWindow_CWnd__UEAAHXZ(pScrollDel);
    pThis->RecalcLayout();
}

// ?DeleteView@CSplitterWnd@@UEAAXHH@Z, RVA 0x2aae80 (mfc140u):
//     CWnd* pPane = GetDlgItem(IdFromRowCol(row, col));
//     if (GetActivePane(NULL, NULL) == pPane) ActivateNext(FALSE);   // slots 104 / 107
//     pPane->DestroyWindow();                                        // slot 26 (tail call); the view auto-deletes in PostNcDestroy
// DEVIATION: a missing pane returns instead of faulting.
// Symbol: ?DeleteView@CSplitterWnd@@UEAAXHH@Z
extern "C" void MS_ABI impl__DeleteView_CSplitterWnd__UEAAXHH_Z(CSplitterWnd* pThis, int row, int col) {
    if (!pThis) return;
    CWnd* pPane = PaneAt(pThis, row, col);
    if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, nullptr, nullptr) == pPane) {
        pThis->ActivateNext(FALSE);
    }
    if (pPane) impl__DestroyWindow_CWnd__UEAAHXZ(pPane);
}

// ?DoScroll@CSplitterWnd@@UEAAHPEAVCView@@IH@Z, RVA 0x2adb00 (mfc140u):
//     int rowFrom, colFrom;
//     if (!IsChildPane(pViewFrom, &rowFrom, &colFrom)) return FALSE;
//     int nOldVPos = 0, nOldHPos = 0;
//     CScrollBar* pBarV = pViewFrom->GetScrollBarCtrl(SB_VERT);   // slot 32
//     if (pBarV) nOldVPos = ::GetScrollPos(pBarV->m_hWnd, SB_CTL);
//     CScrollBar* pBarH = pViewFrom->GetScrollBarCtrl(SB_HORZ);
//     if (pBarH) nOldHPos = ::GetScrollPos(pBarH->m_hWnd, SB_CTL);
//     BOOL bResult = pViewFrom->OnScroll(nScrollCode, 0, bDoScroll) != 0;   // CView slot 92
//     if (pBarV != NULL && m_nCols > 0) {                          // other panes of the same row: vertical part
//         for (col = 0; col < m_nCols; col++) {
//             if (col == colFrom) continue;
//             ::SetScrollPos(pBarV->m_hWnd, SB_CTL, nOldVPos, FALSE);
//             CView* pView = (CView*)GetDlgItem(IdFromRowCol(rowFrom, col));
//             if (pView->OnScroll(MAKEWORD(-1, HIBYTE(nScrollCode)), 0, bDoScroll)) bResult = TRUE;
//         }
//     }
//     if (pBarH != NULL) {                                         // other panes of the same column: horizontal part
//         for (row = 0; row < m_nRows; row++) {
//             if (row == rowFrom) continue;
//             ::SetScrollPos(pBarH->m_hWnd, SB_CTL, nOldHPos, FALSE);
//             CView* pView = (CView*)GetDlgItem(IdFromRowCol(row, colFrom));
//             if (pView->OnScroll(MAKEWORD(LOBYTE(nScrollCode), -1), 0, bDoScroll)) bResult = TRUE;
//         }
//     }
//     return bResult;
// NOTE: OpenMFC's CView::GetScrollBarCtrl (core/view/CView.cpp) looks for a child
// 0xE811/0xE812 of the view, not for the splitter's shared bar as retail does, so
// the shared-bar branches are reached only once that export matches retail.
// Symbol: ?DoScroll@CSplitterWnd@@UEAAHPEAVCView@@IH@Z
extern "C" int MS_ABI impl__DoScroll_CSplitterWnd__UEAAHPEAVCView__IH_Z(CSplitterWnd* pThis, CView* pViewFrom, unsigned int nScrollCode, int bDoScroll) {
    if (!pThis || !pViewFrom) return FALSE;
    int rowFrom = 0, colFrom = 0;
    if (!impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(pThis, pViewFrom, &rowFrom, &colFrom)) return FALSE;
    int nOldVPos = 0, nOldHPos = 0;
    CScrollBar* pBarV = impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(pViewFrom, SB_VERT);
    if (pBarV) nOldVPos = ::GetScrollPos(pBarV->m_hWnd, SB_CTL);
    CScrollBar* pBarH = impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(pViewFrom, SB_HORZ);
    if (pBarH) nOldHPos = ::GetScrollPos(pBarH->m_hWnd, SB_CTL);
    BOOL bResult = impl__OnScroll_CView__UEAAHIIH_Z(pViewFrom, nScrollCode, 0, bDoScroll) != 0;
    if (pBarV != nullptr && pThis->m_nCols > 0) {
        for (int col = 0; col < pThis->m_nCols; col++) {
            if (col == colFrom) continue;
            ::SetScrollPos(pBarV->m_hWnd, SB_CTL, nOldVPos, FALSE);
            CView* pView = static_cast<CView*>(PaneAt(pThis, rowFrom, col));
            if (pView && impl__OnScroll_CView__UEAAHIIH_Z(pView, ((nScrollCode >> 8) & 0xff) << 8 | 0xff, 0, bDoScroll))
                bResult = TRUE;
        }
    }
    if (pBarH != nullptr) {
        for (int row = 0; row < pThis->m_nRows; row++) {
            if (row == rowFrom) continue;
            ::SetScrollPos(pBarH->m_hWnd, SB_CTL, nOldHPos, FALSE);
            CView* pView = static_cast<CView*>(PaneAt(pThis, row, colFrom));
            if (pView && impl__OnScroll_CView__UEAAHIIH_Z(pView, (nScrollCode & 0xff) | 0xff00, 0, bDoScroll))
                bResult = TRUE;
        }
    }
    return bResult;
}

// ?DoScrollBy@CSplitterWnd@@UEAAHPEAVCView@@VCSize@@H@Z, RVA 0x2add10 (mfc140u): same
// shape as DoScroll with OnScrollBy (CView slot 93): the focus pane gets the whole
// sizeScroll, the same-row panes CSize(0, sizeScroll.cy), the same-column panes
// CSize(sizeScroll.cx, 0), each after restoring the shared bar position.
// Symbol: ?DoScrollBy@CSplitterWnd@@UEAAHPEAVCView@@VCSize@@H@Z
extern "C" int MS_ABI impl__DoScrollBy_CSplitterWnd__UEAAHPEAVCView__VCSize__H_Z(CSplitterWnd* pThis, CView* pViewFrom, long long sizeScrollPacked, int bDoScroll) {
    if (!pThis || !pViewFrom) return FALSE;
    const POINT sz = UnpackPoint(sizeScrollPacked);   // cx, cy
    int rowFrom = 0, colFrom = 0;
    if (!impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(pThis, pViewFrom, &rowFrom, &colFrom)) return FALSE;
    int nOldVPos = 0, nOldHPos = 0;
    CScrollBar* pBarV = impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(pViewFrom, SB_VERT);
    if (pBarV) nOldVPos = ::GetScrollPos(pBarV->m_hWnd, SB_CTL);
    CScrollBar* pBarH = impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(pViewFrom, SB_HORZ);
    if (pBarH) nOldHPos = ::GetScrollPos(pBarH->m_hWnd, SB_CTL);
    BOOL bResult = impl__OnScrollBy_CView__UEAAHVCSize__H_Z(pViewFrom, CSize(sz.x, sz.y), bDoScroll) != 0;
    if (pBarV != nullptr && pThis->m_nCols > 0) {
        for (int col = 0; col < pThis->m_nCols; col++) {
            if (col == colFrom) continue;
            ::SetScrollPos(pBarV->m_hWnd, SB_CTL, nOldVPos, FALSE);
            CView* pView = static_cast<CView*>(PaneAt(pThis, rowFrom, col));
            if (pView && impl__OnScrollBy_CView__UEAAHVCSize__H_Z(pView, CSize(0, sz.y), bDoScroll)) bResult = TRUE;
        }
    }
    if (pBarH != nullptr) {
        for (int row = 0; row < pThis->m_nRows; row++) {
            if (row == rowFrom) continue;
            ::SetScrollPos(pBarH->m_hWnd, SB_CTL, nOldHPos, FALSE);
            CView* pView = static_cast<CView*>(PaneAt(pThis, row, colFrom));
            if (pView && impl__OnScrollBy_CView__UEAAHVCSize__H_Z(pView, CSize(sz.x, 0), bDoScroll)) bResult = TRUE;
        }
    }
    return bResult;
}

// ?DrawAllSplitBars@CSplitterWnd@@MEAAXPEAVCDC@@HH@Z, RVA 0x2acaa0 (mfc140u):
//     CRect rect; GetClientRect(rect);
//     rect.left += m_cxBorder;                                     // column split bars
//     for (col = 0; col < m_nCols - 1; col++) {
//         rect.left += m_pColInfo[col].nCurSize + m_cxBorderShare;
//         rect.right = rect.left + m_cxSplitter;
//         if (rect.left > cxInside) break;                         // stop if not fully visible
//         OnDrawSplitter(pDC, splitBar, rect);                     // slot 95
//         rect.left = rect.right + m_cxBorderShare;
//     }
//     GetClientRect(rect);
//     rect.top += m_cyBorder;                                      // row split bars
//     for (row = 0; row < m_nRows - 1; row++) {
//         rect.top += m_pRowInfo[row].nCurSize + m_cyBorderShare;
//         rect.bottom = rect.top + m_cySplitter;
//         if (rect.top > cyInside) break;
//         OnDrawSplitter(pDC, splitBar, rect);
//         rect.top = rect.bottom + m_cyBorderShare;
//     }
//     GetClientRect(rect);                                         // pane borders
//     int x = rect.left;
//     for (col = 0; col < m_nCols; col++) {
//         int cx = m_pColInfo[col].nCurSize + 2*m_cxBorder;
//         if (col == m_nCols-1 && m_bHasVScroll) cx += afxData.cxVScroll - CX_BORDER;
//         int y = rect.top;
//         for (row = 0; row < m_nRows; row++) {
//             int cy = m_pRowInfo[row].nCurSize + 2*m_cyBorder;
//             if (row == m_nRows-1 && m_bHasHScroll) cy += afxData.cyHScroll - CY_BORDER;
//             OnDrawSplitter(pDC, splitBorder, CRect(x, y, x+cx, y+cy));
//             y += cy - 2*m_cyBorder + m_cySplitterGap;
//         }
//         x += cx - 2*m_cxBorder + m_cxSplitterGap;
//     }
// afxData.cxVScroll / cyHScroll (afxData+0x0/+0x4) are read as ::GetSystemMetrics.
// Symbol: ?DrawAllSplitBars@CSplitterWnd@@MEAAXPEAVCDC@@HH@Z
extern "C" void MS_ABI impl__DrawAllSplitBars_CSplitterWnd__MEAAXPEAVCDC__HH_Z(CSplitterWnd* pThis, CDC* pDC, int cxInside, int cyInside) {
    if (!pThis || !pThis->m_hWnd) return;
    const SplitterExtra& x = Extra(pThis);
    if (!x.pColInfo || !x.pRowInfo) return;
    int row, col;
    RECT rect = {};
    ::GetClientRect(pThis->m_hWnd, &rect);
    rect.left += x.cxBorder;
    for (col = 0; col < pThis->m_nCols - 1; col++) {
        rect.left += x.pColInfo[col].nCurSize + pThis->m_cxBorderShare;
        rect.right = rect.left + pThis->m_cxSplitter;
        if (rect.left > cxInside) break;
        impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(pThis, pDC, kSplitBar, &rect);
        rect.left = rect.right + pThis->m_cxBorderShare;
    }
    ::GetClientRect(pThis->m_hWnd, &rect);
    rect.top += x.cyBorder;
    for (row = 0; row < pThis->m_nRows - 1; row++) {
        rect.top += x.pRowInfo[row].nCurSize + pThis->m_cyBorderShare;
        rect.bottom = rect.top + pThis->m_cySplitter;
        if (rect.top > cyInside) break;
        impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(pThis, pDC, kSplitBar, &rect);
        rect.top = rect.bottom + pThis->m_cyBorderShare;
    }
    ::GetClientRect(pThis->m_hWnd, &rect);
    int xx = rect.left;
    for (col = 0; col < pThis->m_nCols; col++) {
        int cx = x.pColInfo[col].nCurSize + 2 * x.cxBorder;
        if (col == pThis->m_nCols - 1 && pThis->m_bHasVScroll) cx += ::GetSystemMetrics(SM_CXVSCROLL) - kCxBorder;
        int yy = rect.top;
        for (row = 0; row < pThis->m_nRows; row++) {
            int cy = x.pRowInfo[row].nCurSize + 2 * x.cyBorder;
            if (row == pThis->m_nRows - 1 && pThis->m_bHasHScroll) cy += ::GetSystemMetrics(SM_CYHSCROLL) - kCyBorder;
            RECT rc = { xx, yy, xx + cx, yy + cy };
            impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(pThis, pDC, kSplitBorder, &rc);
            yy += cy - 2 * x.cyBorder + pThis->m_cySplitterGap;
        }
        xx += cx - 2 * x.cxBorder + pThis->m_cxSplitterGap;
    }
}

// ?GetHitRect@CSplitterWnd@@MEAAXHAEAVCRect@@@Z, RVA 0x2abd20 (mfc140u):
//     CRect rectClient; GetClientRect(rectClient); rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
//     int cx = rectClient.Width(), cy = rectClient.Height();
//     int x = rectClient.top, y = rectClient.left;     // sic: retail seeds x from .top and y from .left
//     m_ptTrackOffset.x = m_ptTrackOffset.y = 0;       // m_rectLimit is narrowed for the hit
//     if (ht == vSplitterBox) {
//         cy = m_cySplitter - 2*m_cyBorder + 1;  m_ptTrackOffset.y = -(cy/2);  m_rectLimit.bottom -= cy;
//     } else if (ht == hSplitterBox) {
//         cx = m_cxSplitter - 2*m_cxBorder + 1;  m_ptTrackOffset.x = -(cx/2);  m_rectLimit.right -= cx;
//     } else if (vSplitterBar1..15) {
//         cy = m_cySplitter - 2*m_cyBorder + 1;  m_ptTrackOffset.y = -(cy/2);
//         for (row = 0; row < ht - vSplitterBar1; row++) y += m_pRowInfo[row].nCurSize + m_cySplitterGap;
//         m_rectLimit.top = y;
//         y += m_pRowInfo[row].nCurSize + m_cyBorderShare + 1;
//         m_rectLimit.bottom -= cy;
//     } else if (hSplitterBar1..15) {
//         cx = m_cxSplitter - 2*m_cxBorder + 1;  m_ptTrackOffset.x = -(cx/2);
//         for (col = 0; col < ht - hSplitterBar1; col++) x += m_pColInfo[col].nCurSize + m_cxSplitterGap;
//         m_rectLimit.left = x;
//         x += m_pColInfo[col].nCurSize + m_cxBorderShare + 1;
//         m_rectLimit.right -= cx;
//     }                                                  // (any other ht leaves x/y/cx/cy at the client values)
//     rectHit.right = (rectHit.left = x) + cx;  rectHit.bottom = (rectHit.top = y) + cy;
// The x/y seeding really is crossed in the retail body: the register loaded from
// rectClient.top (0x24(%rsp)) is the one advanced by m_pColInfo sizes, stored to
// m_rectLimit.left and written to rectHit.left, and vice versa.  It is harmless
// while m_cxBorder == m_cyBorder (both 2 here) and is kept as retail has it.
// The "+ 1" terms are literal `inc` instructions in the retail image.
// Symbol: ?GetHitRect@CSplitterWnd@@MEAAXHAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(CSplitterWnd* pThis, int ht, RECT* pRectHit) {
    if (!pThis || !pRectHit) return;
    const SplitterExtra& x = Extra(pThis);
    SplitterTrackState& t = Track(pThis);
    RECT rectClient = {};
    if (pThis->m_hWnd) ::GetClientRect(pThis->m_hWnd, &rectClient);
    InflateRC(rectClient, -x.cxBorder, -x.cyBorder);
    int cx = rectClient.right - rectClient.left;
    int cy = rectClient.bottom - rectClient.top;
    int xx = rectClient.top;    // sic (see the note above)
    int yy = rectClient.left;   // sic
    t.m_ptTrackOffset.x = 0;
    t.m_ptTrackOffset.y = 0;
    if (ht == kVSplitterBox) {
        cy = pThis->m_cySplitter - 2 * x.cyBorder + 1;
        t.m_ptTrackOffset.y = -(cy / 2);
        t.m_rectLimit.bottom -= cy;
    } else if (ht == kHSplitterBox) {
        cx = pThis->m_cxSplitter - 2 * x.cxBorder + 1;
        t.m_ptTrackOffset.x = -(cx / 2);
        t.m_rectLimit.right -= cx;
    } else if (IsVBar(ht) && x.pRowInfo) {
        cy = pThis->m_cySplitter - 2 * x.cyBorder + 1;
        t.m_ptTrackOffset.y = -(cy / 2);
        int row;
        for (row = 0; row < ht - kVSplitterBar1; row++) yy += x.pRowInfo[row].nCurSize + pThis->m_cySplitterGap;
        t.m_rectLimit.top = yy;
        yy += x.pRowInfo[row].nCurSize + pThis->m_cyBorderShare + 1;
        t.m_rectLimit.bottom -= cy;
    } else if (IsHBar(ht) && x.pColInfo) {
        cx = pThis->m_cxSplitter - 2 * x.cxBorder + 1;
        t.m_ptTrackOffset.x = -(cx / 2);
        int col;
        for (col = 0; col < ht - kHSplitterBar1; col++) xx += x.pColInfo[col].nCurSize + pThis->m_cxSplitterGap;
        t.m_rectLimit.left = xx;
        xx += x.pColInfo[col].nCurSize + pThis->m_cxBorderShare + 1;
        t.m_rectLimit.right -= cx;
    }
    pRectHit->left = xx;
    pRectHit->right = xx + cx;
    pRectHit->top = yy;
    pRectHit->bottom = yy + cy;
}

// ?GetInsideRect@CSplitterWnd@@MEBAXAEAVCRect@@@Z, RVA 0x2ab690 (mfc140u):
//     GetClientRect(rect);
//     rect.InflateRect(-m_cxBorder, -m_cyBorder);
//     if (m_bHasVScroll) rect.right  -= afxData.cxVScroll - CX_BORDER;   // (adds 1 - cxVScroll)
//     if (m_bHasHScroll) rect.bottom -= afxData.cyHScroll - CY_BORDER;
// afxData.cxVScroll / cyHScroll are read as ::GetSystemMetrics (OpenMFC's afxData is zero).
// Symbol: ?GetInsideRect@CSplitterWnd@@MEBAXAEAVCRect@@@Z
extern "C" void MS_ABI impl__GetInsideRect_CSplitterWnd__MEBAXAEAVCRect___Z(const CSplitterWnd* pThis, RECT* pRect) {
    if (!pThis || !pRect) return;
    const SplitterExtra& x = Extra(pThis);
    RECT rect = {};
    if (pThis->m_hWnd) ::GetClientRect(pThis->m_hWnd, &rect);
    InflateRC(rect, -x.cxBorder, -x.cyBorder);
    if (pThis->m_bHasVScroll) rect.right  -= ::GetSystemMetrics(SM_CXVSCROLL) - kCxBorder;
    if (pThis->m_bHasHScroll) rect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL) - kCyBorder;
    *pRect = rect;
}

// ?GetScrollStyle@CSplitterWnd@@QEBAKXZ, RVA 0x2aac80 (mfc140u):
//     DWORD dwStyle = 0;
//     if (m_bHasHScroll) dwStyle |= WS_HSCROLL;
//     if (m_bHasVScroll) dwStyle |= WS_VSCROLL;
//     return dwStyle;
// Symbol: ?GetScrollStyle@CSplitterWnd@@QEBAKXZ
extern "C" unsigned long MS_ABI impl__GetScrollStyle_CSplitterWnd__QEBAKXZ(const CSplitterWnd* pThis) {
    if (!pThis) return 0;
    unsigned long dwStyle = 0;
    if (pThis->m_bHasHScroll) dwStyle |= WS_HSCROLL;
    if (pThis->m_bHasVScroll) dwStyle |= WS_VSCROLL;
    return dwStyle;
}

// ?GetSizingParent@CSplitterWnd@@IEAAPEAVCWnd@@XZ, RVA 0x2ac5f0 (mfc140u):
//     CRect rectClient; GetClientRect(rectClient);
//     CWnd* pParent = (GetStyle() & WS_THICKFRAME) ? this : GetParent();
//     if ((pParent->GetStyle() & (WS_THICKFRAME|WS_MAXIMIZE)) == WS_THICKFRAME) {
//         CRect rect; pParent->GetClientRect(rect); pParent->ClientToScreen(rect); ScreenToClient(rect);
//         if (rect.right == rectClient.right && rect.bottom == rectClient.bottom) return pParent;
//     }
//     return NULL;
// Symbol: ?GetSizingParent@CSplitterWnd@@IEAAPEAVCWnd@@XZ
extern "C" CWnd* MS_ABI impl__GetSizingParent_CSplitterWnd__IEAAPEAVCWnd__XZ(CSplitterWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) return nullptr;
    RECT rectClient = {};
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    CWnd* pParent = (StyleOf(pThis) & WS_THICKFRAME)
        ? pThis : impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (!pParent) return nullptr;   // retail dereferences it unguarded
    if ((StyleOf(pParent) & (WS_THICKFRAME | WS_MAXIMIZE)) == WS_THICKFRAME) {
        RECT rect = {};
        ::GetClientRect(pParent->m_hWnd, &rect);
        ::ClientToScreen(pParent->m_hWnd, reinterpret_cast<POINT*>(&rect));
        ::ClientToScreen(pParent->m_hWnd, reinterpret_cast<POINT*>(&rect) + 1);
        ::ScreenToClient(pThis->m_hWnd, reinterpret_cast<POINT*>(&rect));
        ::ScreenToClient(pThis->m_hWnd, reinterpret_cast<POINT*>(&rect) + 1);
        if (rect.right == rectClient.right && rect.bottom == rectClient.bottom) return pParent;
    }
    return nullptr;
}

// ?HitTest@CSplitterWnd@@MEBAHVCPoint@@@Z, RVA 0x2abf30 (mfc140u):
//     CRect rectClient; GetClientRect(rectClient); rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
//     CRect rectInside; GetInsideRect(rectInside);                                        // slot 112
//     if (m_bHasVScroll && m_nRows < m_nMaxRows &&
//         CRect(rectInside.right, rectClient.top, rectClient.right, rectClient.top + m_cySplitter - CY_BORDER).PtInRect(pt))
//         return vSplitterBox;
//     if (m_bHasHScroll && m_nCols < m_nMaxCols &&
//         CRect(rectClient.left, rectInside.bottom, rectClient.left + m_cxSplitter - CX_BORDER, rectClient.bottom).PtInRect(pt))
//         return hSplitterBox;
//     CRect rect = rectClient;                          // for hit detect, include the border of the splitters
//     for (col = 0; col < m_nCols - 1; col++) {
//         rect.left += m_pColInfo[col].nCurSize; rect.right = rect.left + m_cxSplitterGap;
//         if (rect.PtInRect(pt)) break;
//         rect.left = rect.right;
//     }
//     rect = rectClient;
//     for (row = 0; row < m_nRows - 1; row++) {
//         rect.top += m_pRowInfo[row].nCurSize; rect.bottom = rect.top + m_cySplitterGap;
//         if (rect.PtInRect(pt)) break;
//         rect.top = rect.bottom;
//     }
//     if (col == m_nCols - 1) return (row == m_nRows - 1) ? noHit : vSplitterBar1 + row;
//     if (row == m_nRows - 1) return hSplitterBar1 + col;
//     return splitterIntersection1 + row * 15 + col;
// Symbol: ?HitTest@CSplitterWnd@@MEBAHVCPoint@@@Z
extern "C" int MS_ABI impl__HitTest_CSplitterWnd__MEBAHVCPoint___Z(const CSplitterWnd* pThis, long long ptPacked) {
    if (!pThis || !pThis->m_hWnd) return kNoHit;
    const SplitterExtra& x = Extra(pThis);
    const POINT pt = UnpackPoint(ptPacked);
    RECT rectClient = {};
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    InflateRC(rectClient, -x.cxBorder, -x.cyBorder);
    RECT rectInside = {};
    impl__GetInsideRect_CSplitterWnd__MEBAXAEAVCRect___Z(pThis, &rectInside);
    if (pThis->m_bHasVScroll && pThis->m_nRows < pThis->m_nMaxRows) {
        RECT rc = { rectInside.right, rectClient.top, rectClient.right, rectClient.top + pThis->m_cySplitter - kCyBorder };
        if (::PtInRect(&rc, pt)) return kVSplitterBox;
    }
    if (pThis->m_bHasHScroll && pThis->m_nCols < pThis->m_nMaxCols) {
        RECT rc = { rectClient.left, rectInside.bottom, rectClient.left + pThis->m_cxSplitter - kCxBorder, rectClient.bottom };
        if (::PtInRect(&rc, pt)) return kHSplitterBox;
    }
    RECT rect = rectClient;
    int col;
    for (col = 0; col < pThis->m_nCols - 1 && x.pColInfo; col++) {
        rect.left += x.pColInfo[col].nCurSize;
        rect.right = rect.left + pThis->m_cxSplitterGap;
        if (::PtInRect(&rect, pt)) break;
        rect.left = rect.right;
    }
    rect = rectClient;
    int row;
    for (row = 0; row < pThis->m_nRows - 1 && x.pRowInfo; row++) {
        rect.top += x.pRowInfo[row].nCurSize;
        rect.bottom = rect.top + pThis->m_cySplitterGap;
        if (::PtInRect(&rect, pt)) break;
        rect.top = rect.bottom;
    }
    if (col == pThis->m_nCols - 1) {
        if (row == pThis->m_nRows - 1) return kNoHit;
        return kVSplitterBar1 + row;
    }
    if (row == pThis->m_nRows - 1) return kHSplitterBar1 + col;
    return kSplitterIntersection1 + row * 15 + col;
}

// ?IdFromRowCol@CSplitterWnd@@QEBAHHH@Z, RVA 0x2aab30 (mfc140u):
//     return AFX_IDW_PANE_FIRST + row * 16 + col;
// Symbol: ?IdFromRowCol@CSplitterWnd@@QEBAHHH@Z
extern "C" int MS_ABI impl__IdFromRowCol_CSplitterWnd__QEBAHHH_Z(const CSplitterWnd* pThis, int row, int col) {
    (void)pThis;
    return IdFromRowColImpl(row, col);
}

// ?IsChildPane@CSplitterWnd@@QEAAHPEAVCWnd@@PEAH1@Z, RVA 0x2aab60 (mfc140u):
//     UINT nID = ::GetDlgCtrlID(pWnd->m_hWnd);
//     if (::IsChild(m_hWnd, pWnd->m_hWnd) && nID >= AFX_IDW_PANE_FIRST && nID <= AFX_IDW_PANE_LAST) {
//         if (pRow != NULL) *pRow = (nID - AFX_IDW_PANE_FIRST) / 16;
//         if (pCol != NULL) *pCol = (nID - AFX_IDW_PANE_FIRST) % 16;
//         return TRUE;
//     }
//     if (pRow != NULL) *pRow = -1;
//     if (pCol != NULL) *pCol = -1;
//     return FALSE;
// Symbol: ?IsChildPane@CSplitterWnd@@QEAAHPEAVCWnd@@PEAH1@Z
extern "C" int MS_ABI impl__IsChildPane_CSplitterWnd__QEAAHPEAVCWnd__PEAH1_Z(CSplitterWnd* pThis, CWnd* pWnd, int* pRow, int* pCol) {
    if (pThis && pWnd && pThis->m_hWnd && pWnd->m_hWnd) {   // retail dereferences both unguarded
        const unsigned nID = static_cast<unsigned>(::GetDlgCtrlID(pWnd->m_hWnd));
        if (::IsChild(pThis->m_hWnd, pWnd->m_hWnd) && nID >= kIdwPaneFirst && nID <= kIdwPaneLast) {
            if (pRow != nullptr) *pRow = static_cast<int>((nID - kIdwPaneFirst) / 16);
            if (pCol != nullptr) *pCol = static_cast<int>((nID - kIdwPaneFirst) % 16);
            return TRUE;
        }
    }
    if (pRow != nullptr) *pRow = -1;
    if (pCol != nullptr) *pCol = -1;
    return FALSE;
}

// ?OnCancelMode@CSplitterWnd@@IEAAXXZ, RVA 0x2ad4d0 (mfc140u):
//     StopTracking(FALSE);          // vtable slot 119, tail call
// Symbol: ?OnCancelMode@CSplitterWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnCancelMode_CSplitterWnd__IEAAXXZ(CSplitterWnd* pThis) {
    impl__StopTracking_CSplitterWnd__MEAAXH_Z(pThis, FALSE);
}

// ?OnCommand@CSplitterWnd@@MEAAH_K_J@Z, RVA 0x2ad6c0 (mfc140u):
//     if (CWnd::OnCommand(wParam, lParam)) return TRUE;
//     CFrameWnd* pFrameWnd = GetParentFrame(); ENSURE_VALID(pFrameWnd);     // throws CInvalidArgException
//     return ::SendMessage(pFrameWnd->m_hWnd, WM_COMMAND, wParam, lParam) == 0;   // note: == 0
// Symbol: ?OnCommand@CSplitterWnd@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CSplitterWnd__MEAAH_K_J_Z(CSplitterWnd* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return FALSE;
    if (impl__OnCommand_CWnd__MEAAH_K_J_Z(pThis, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam))) return TRUE;
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (!pFrameWnd) impl__AfxThrowInvalidArgException__YAXXZ();
    return ::SendMessageW(pFrameWnd->m_hWnd, WM_COMMAND, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam)) == 0 ? TRUE : FALSE;
}

// ?OnDisplayChange@CSplitterWnd@@IEAAXXZ, RVA 0x2ac350 (mfc140u):
//     if (!::IsIconic(m_hWnd) && ::IsWindowVisible(m_hWnd)) RecalcLayout();   // vtable slot 94
// Symbol: ?OnDisplayChange@CSplitterWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnDisplayChange_CSplitterWnd__IEAAXXZ(CSplitterWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) return;
    if (!::IsIconic(pThis->m_hWnd) && ::IsWindowVisible(pThis->m_hWnd)) pThis->RecalcLayout();
}

// ?OnDraw@CSplitterWnd@@MEAAXPEAVCDC@@@Z, RVA 0x2acd90 (mfc140u):
//     CRect rectClient; GetClientRect(rectClient); rectClient.InflateRect(-m_cxBorder, -m_cyBorder);
//     CRect rectInside; GetInsideRect(rectInside);                                       // slot 112
//     if (m_bHasVScroll && m_nRows < m_nMaxRows)                                          // the split boxes
//         OnDrawSplitter(pDC, splitBox, CRect(rectInside.right, rectClient.top, rectClient.right, rectClient.top + m_cySplitter));
//     if (m_bHasHScroll && m_nCols < m_nMaxCols)
//         OnDrawSplitter(pDC, splitBox, CRect(rectClient.left, rectInside.bottom, rectClient.left + m_cxSplitter, rectClient.bottom));
//     DrawAllSplitBars(pDC, rectInside.right, rectInside.bottom);                         // slot 116
// Symbol: ?OnDraw@CSplitterWnd@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CSplitterWnd__MEAAXPEAVCDC___Z(CSplitterWnd* pThis, CDC* pDC) {
    if (!pThis || !pThis->m_hWnd) return;
    const SplitterExtra& x = Extra(pThis);
    RECT rectClient = {};
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    InflateRC(rectClient, -x.cxBorder, -x.cyBorder);
    RECT rectInside = {};
    impl__GetInsideRect_CSplitterWnd__MEBAXAEAVCRect___Z(pThis, &rectInside);
    if (pThis->m_bHasVScroll && pThis->m_nRows < pThis->m_nMaxRows) {
        RECT rect = { rectInside.right, rectClient.top, rectClient.right, rectClient.top + pThis->m_cySplitter };
        impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(pThis, pDC, kSplitBox, &rect);
    }
    if (pThis->m_bHasHScroll && pThis->m_nCols < pThis->m_nMaxCols) {
        RECT rect = { rectClient.left, rectInside.bottom, rectClient.left + pThis->m_cxSplitter, rectClient.bottom };
        impl__OnDrawSplitter_CSplitterWnd__MEAAXPEAVCDC__W4ESplitType_1_AEBVCRect___Z(pThis, pDC, kSplitBox, &rect);
    }
    impl__DrawAllSplitBars_CSplitterWnd__MEAAXPEAVCDC__HH_Z(pThis, pDC, rectInside.right, rectInside.bottom);
}

// ?OnHScroll@CSplitterWnd@@IEAAXIIPEAVCScrollBar@@@Z, RVA 0x2ad980 (mfc140u):
//     int col = ::GetDlgCtrlID(pScrollBar->m_hWnd) - AFX_IDW_HSCROLL_FIRST;
//     int nOldPos = ::GetScrollPos(pScrollBar->m_hWnd, SB_CTL);
//     for (row = 0; row < m_nRows; row++) {
//         ::SendMessage(GetDlgItem(IdFromRowCol(row, col))->m_hWnd, WM_HSCROLL, MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
//         if (row < m_nRows - 1) ::SetScrollPos(pScrollBar->m_hWnd, SB_CTL, nOldPos, FALSE);   // restore for the next pane
//     }
// Symbol: ?OnHScroll@CSplitterWnd@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnHScroll_CSplitterWnd__IEAAXIIPEAVCScrollBar___Z(CSplitterWnd* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (!pThis || !pScrollBar || !pScrollBar->m_hWnd) return;   // retail dereferences pScrollBar unguarded
    const int col = ::GetDlgCtrlID(pScrollBar->m_hWnd) - static_cast<int>(kIdwHScrollFirst);
    const int nOldPos = ::GetScrollPos(pScrollBar->m_hWnd, SB_CTL);
    const WPARAM wParam = static_cast<WPARAM>(static_cast<int>(MAKELONG(nSBCode, nPos)));
    for (int row = 0; row < pThis->m_nRows; row++) {
        CWnd* pPane = PaneAt(pThis, row, col);
        if (pPane && pPane->m_hWnd)
            ::SendMessageW(pPane->m_hWnd, WM_HSCROLL, wParam, reinterpret_cast<LPARAM>(pScrollBar->m_hWnd));
        if (row < pThis->m_nRows - 1) ::SetScrollPos(pScrollBar->m_hWnd, SB_CTL, nOldPos, FALSE);
    }
}

// ?OnKeyDown@CSplitterWnd@@IEAAXIII@Z, RVA 0x2ad4f0 (mfc140u):
//     CPoint pt; ::GetCursorPos(&pt);
//     int cz = ::GetKeyState(VK_CONTROL) < 0 ? 1 : 16;
//     int dx = 0, dy = 0;
//     switch (nChar) {
//     case VK_RETURN: StopTracking(TRUE);  return;      // slot 119
//     case VK_ESCAPE: StopTracking(FALSE); return;
//     case VK_LEFT: dx = -1; break;  case VK_RIGHT: dx = 1; break;
//     case VK_UP:   dy = -1; break;  case VK_DOWN:  dy = 1; break;
//     default: Default(); return;
//     }
//     if (m_htTrack == vSplitterBox || vSplitterBar1..15) dx = 0;    // vertical tracker: y only
//     if (m_htTrack == hSplitterBox || hSplitterBar1..15) dy = 0;    // horizontal tracker: x only
//     pt.x += dx * cz; pt.y += dy * cz;
//     ::ScreenToClient(m_hWnd, &pt);
//     if (pt.y < m_rectLimit.top) pt.y = m_rectLimit.top; else if (pt.y > m_rectLimit.bottom) pt.y = m_rectLimit.bottom;
//     if (pt.x < m_rectLimit.left) pt.x = m_rectLimit.left; else if (pt.x > m_rectLimit.right) pt.x = m_rectLimit.right;
//     ::ClientToScreen(m_hWnd, &pt); ::SetCursorPos(pt.x, pt.y);
// Symbol: ?OnKeyDown@CSplitterWnd@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CSplitterWnd__IEAAXIII_Z(CSplitterWnd* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt;
    (void)nFlags;
    if (!pThis) return;
    POINT pt = {};
    ::GetCursorPos(&pt);
    const int cz = ::GetKeyState(VK_CONTROL) < 0 ? 1 : 16;
    int dx = 0, dy = 0;
    switch (nChar) {
    case VK_RETURN: impl__StopTracking_CSplitterWnd__MEAAXH_Z(pThis, TRUE);  return;
    case VK_ESCAPE: impl__StopTracking_CSplitterWnd__MEAAXH_Z(pThis, FALSE); return;
    case VK_LEFT:  dx = -1; break;
    case VK_RIGHT: dx = 1;  break;
    case VK_UP:    dy = -1; break;
    case VK_DOWN:  dy = 1;  break;
    default: impl__Default_CWnd__IEAA_JXZ(pThis); return;
    }
    const SplitterTrackState& t = Track(pThis);
    if (t.m_htTrack == kVSplitterBox || IsVBar(t.m_htTrack)) dx = 0;
    if (t.m_htTrack == kHSplitterBox || IsHBar(t.m_htTrack)) dy = 0;
    pt.x += dx * cz;
    pt.y += dy * cz;
    ::ScreenToClient(pThis->m_hWnd, &pt);
    if (pt.y < t.m_rectLimit.top) pt.y = t.m_rectLimit.top;
    else if (pt.y > t.m_rectLimit.bottom) pt.y = t.m_rectLimit.bottom;
    if (pt.x < t.m_rectLimit.left) pt.x = t.m_rectLimit.left;
    else if (pt.x > t.m_rectLimit.right) pt.x = t.m_rectLimit.right;
    ::ClientToScreen(pThis->m_hWnd, &pt);
    ::SetCursorPos(pt.x, pt.y);
}

// ?OnLButtonDblClk@CSplitterWnd@@IEAAXIVCPoint@@@Z, RVA 0x2ad300 (mfc140u):
//     int ht = HitTest(pt);                                            // slot 111
//     StopTracking(FALSE);                                             // slot 119
//     if (!(GetStyle() & SPLS_DYNAMIC_SPLIT)) return;                  // only for dynamic splitters
//     if (ht == vSplitterBox)      SplitRow(m_pRowInfo[0].nCurSize / 2);          // slot 100
//     else if (ht == hSplitterBox) SplitColumn(m_pColInfo[0].nCurSize / 2);       // slot 101
//     else if (vSplitterBar1..15) {
//         int rowDelete = ht - vSplitterBar1, rowActive;
//         if (GetActivePane(&rowActive, NULL) != NULL && rowDelete == rowActive) rowDelete++;
//         DeleteRow(rowDelete);                                        // slot 102
//     } else if (hSplitterBar1..15) {
//         int colDelete = ht - hSplitterBar1, colActive;
//         if (GetActivePane(NULL, &colActive) != NULL && colDelete == colActive) colDelete++;
//         DeleteColumn(colDelete);                                     // slot 103
//     } else if (splitterIntersection1..225) {
//         int rowDelete = (ht - splitterIntersection1) / 15, colDelete = (ht - splitterIntersection1) % 15;
//         int rowActive, colActive;
//         if (GetActivePane(&rowActive, &colActive) != NULL) {
//             if (colDelete == colActive) colDelete++;
//             if (rowDelete == rowActive) rowDelete++;
//         }
//         DeleteRow(rowDelete); DeleteColumn(colDelete);
//     }
// Symbol: ?OnLButtonDblClk@CSplitterWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CSplitterWnd__IEAAXIVCPoint___Z(CSplitterWnd* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    if (!pThis) return;
    const int ht = impl__HitTest_CSplitterWnd__MEBAHVCPoint___Z(pThis, ptPacked);
    impl__StopTracking_CSplitterWnd__MEAAXH_Z(pThis, FALSE);
    if (!(StyleOf(pThis) & kSplsDynamicSplit)) return;
    const SplitterExtra& x = Extra(pThis);
    if (ht == kVSplitterBox) {
        if (x.pRowInfo) impl__SplitRow_CSplitterWnd__UEAAHH_Z(pThis, x.pRowInfo[0].nCurSize / 2);
    } else if (ht == kHSplitterBox) {
        if (x.pColInfo) impl__SplitColumn_CSplitterWnd__UEAAHH_Z(pThis, x.pColInfo[0].nCurSize / 2);
    } else if (IsVBar(ht)) {
        int rowDelete = ht - kVSplitterBar1, rowActive = 0;
        if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, &rowActive, nullptr) != nullptr && rowDelete == rowActive)
            rowDelete++;
        impl__DeleteRow_CSplitterWnd__UEAAXH_Z(pThis, rowDelete);
    } else if (IsHBar(ht)) {
        int colDelete = ht - kHSplitterBar1, colActive = 0;
        if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, nullptr, &colActive) != nullptr && colDelete == colActive)
            colDelete++;
        impl__DeleteColumn_CSplitterWnd__UEAAXH_Z(pThis, colDelete);
    } else if (IsIsect(ht)) {
        int rowDelete = (ht - kSplitterIntersection1) / 15;
        int colDelete = (ht - kSplitterIntersection1) % 15;
        int rowActive = 0, colActive = 0;
        if (impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, &rowActive, &colActive) != nullptr) {
            if (colDelete == colActive) colDelete++;
            if (rowDelete == rowActive) rowDelete++;
        }
        impl__DeleteRow_CSplitterWnd__UEAAXH_Z(pThis, rowDelete);
        impl__DeleteColumn_CSplitterWnd__UEAAXH_Z(pThis, colDelete);
    }
}

// ?OnLButtonDown@CSplitterWnd@@IEAAXIVCPoint@@@Z, RVA 0x2ad2b0 (mfc140u):
//     if (m_bTracking) return;
//     StartTracking(HitTest(pt));       // slots 111 / 118
// Symbol: ?OnLButtonDown@CSplitterWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CSplitterWnd__IEAAXIVCPoint___Z(CSplitterWnd* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    if (!pThis) return;
    if (Track(pThis).m_bTracking) return;
    impl__StartTracking_CSplitterWnd__MEAAXH_Z(pThis, impl__HitTest_CSplitterWnd__MEBAHVCPoint___Z(pThis, ptPacked));
}

// ?OnLButtonUp@CSplitterWnd@@IEAAXIVCPoint@@@Z, RVA 0x2ad4b0 (mfc140u):
//     StopTracking(TRUE);               // slot 119, tail call
// Symbol: ?OnLButtonUp@CSplitterWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CSplitterWnd__IEAAXIVCPoint___Z(CSplitterWnd* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    (void)ptPacked;
    impl__StopTracking_CSplitterWnd__MEAAXH_Z(pThis, TRUE);
}

// ?OnMouseMove@CSplitterWnd@@IEAAXIVCPoint@@@Z, RVA 0x2ad0c0 (mfc140u):
//     if (CWnd::FromHandle(::GetCapture()) != this) StopTracking(FALSE);      // slot 119
//     if (m_bTracking) {
//         pt += m_ptTrackOffset;                                              // pt is the tracker's upper-left
//         int y = pt.y < m_rectLimit.top  ? m_rectLimit.top  : min(pt.y, m_rectLimit.bottom);
//         int x = pt.x < m_rectLimit.left ? m_rectLimit.left : min(pt.x, m_rectLimit.right);
//         if (m_htTrack == vSplitterBox || vSplitterBar1..15) {
//             if (m_rectTracker.top != y) { OnInvertTracker(m_rectTracker); m_rectTracker.OffsetRect(0, y - m_rectTracker.top); OnInvertTracker(m_rectTracker); }   // slot 96
//         } else if (m_htTrack == hSplitterBox || hSplitterBar1..15) {
//             if (m_rectTracker.left != x) { OnInvertTracker(m_rectTracker); m_rectTracker.OffsetRect(x - m_rectTracker.left, 0); OnInvertTracker(m_rectTracker); }
//         } else if (m_htTrack == bothSplitterBox || splitterIntersection1..225) {
//             if (m_rectTracker.top != y)   { ...same vertical move of m_rectTracker... }
//             if (m_rectTracker2.left != x) { OnInvertTracker(m_rectTracker2); m_rectTracker2.OffsetRect(x - m_rectTracker2.left, 0); OnInvertTracker(m_rectTracker2); }
//         }
//     } else {
//         SetSplitCursor(HitTest(pt));                                        // slots 111 / 117
//     }
// Symbol: ?OnMouseMove@CSplitterWnd@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CSplitterWnd__IEAAXIVCPoint___Z(CSplitterWnd* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    if (!pThis) return;
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetCapture()) != pThis)
        impl__StopTracking_CSplitterWnd__MEAAXH_Z(pThis, FALSE);
    SplitterTrackState& t = Track(pThis);
    if (t.m_bTracking) {
        POINT pt = UnpackPoint(ptPacked);
        pt.x += t.m_ptTrackOffset.x;
        pt.y += t.m_ptTrackOffset.y;
        const int y = pt.y < t.m_rectLimit.top  ? t.m_rectLimit.top  : (pt.y < t.m_rectLimit.bottom ? pt.y : t.m_rectLimit.bottom);
        const int x = pt.x < t.m_rectLimit.left ? t.m_rectLimit.left : (pt.x < t.m_rectLimit.right  ? pt.x : t.m_rectLimit.right);
        const int ht = t.m_htTrack;
        const bool bVert = (ht == kVSplitterBox || IsVBar(ht));
        const bool bHorz = (ht == kHSplitterBox || IsHBar(ht));
        const bool bBoth = (ht == kBothSplitterBox || IsIsect(ht));
        if ((bVert || bBoth) && t.m_rectTracker.top != y) {
            pThis->OnInvertTracker(CRect(t.m_rectTracker));
            ::OffsetRect(&t.m_rectTracker, 0, y - t.m_rectTracker.top);
            pThis->OnInvertTracker(CRect(t.m_rectTracker));
        }
        if (bHorz && t.m_rectTracker.left != x) {
            pThis->OnInvertTracker(CRect(t.m_rectTracker));
            ::OffsetRect(&t.m_rectTracker, x - t.m_rectTracker.left, 0);
            pThis->OnInvertTracker(CRect(t.m_rectTracker));
        }
        if (bBoth && t.m_rectTracker2.left != x) {
            pThis->OnInvertTracker(CRect(t.m_rectTracker2));
            ::OffsetRect(&t.m_rectTracker2, x - t.m_rectTracker2.left, 0);
            pThis->OnInvertTracker(CRect(t.m_rectTracker2));
        }
    } else {
        pThis->SetSplitCursor(impl__HitTest_CSplitterWnd__MEBAHVCPoint___Z(pThis, ptPacked));
    }
}

// ?OnMouseWheel@CSplitterWnd@@IEAAHIFVCPoint@@@Z, RVA 0x2ad790 (mfc140u):
//     BOOL bVertScroll = FALSE;                     // is there any enabled vertical bar among the panes?
//     for (row...) for (col...) {
//         CWnd* pPane = GetDlgItem(IdFromRowCol(row, col));
//         if (pPane != NULL && pPane->IsKindOf(RUNTIME_CLASS(CScrollView))) {
//             CScrollBar* pBar = pPane->GetScrollBarCtrl(SB_VERT);            // slot 32
//             if (pBar != NULL && pBar->IsWindowEnabled()) { bVertScroll = TRUE; goto scroll; }
//         }
//     }
// scroll:
//     for (row...) for (col...) {
//         CWnd* pPane = GetDlgItem(IdFromRowCol(row, col));
//         if (pPane == NULL || !pPane->IsKindOf(RUNTIME_CLASS(CScrollView))) continue;
//         CScrollBar* pBar = pPane->GetScrollBarCtrl(bVertScroll ? SB_VERT : SB_HORZ);
//         if (pBar == NULL || !pBar->IsWindowEnabled()) continue;
//         int nOldPos = ::GetScrollPos(pBar->m_hWnd, SB_CTL);
//         ((CScrollView*)pPane)->DoMouseWheel(fFlags, zDelta, point);
//         if (bVertScroll ? (col < m_nCols - 1) : (row < m_nRows - 1))
//             ::SetScrollPos(pBar->m_hWnd, SB_CTL, nOldPos, FALSE);            // restore for the next pane
//     }
//     return TRUE;
// Symbol: ?OnMouseWheel@CSplitterWnd@@IEAAHIFVCPoint@@@Z
extern "C" int MS_ABI impl__OnMouseWheel_CSplitterWnd__IEAAHIFVCPoint___Z(CSplitterWnd* pThis, unsigned int fFlags, short zDelta, long long ptPacked) {
    if (!pThis) return FALSE;
    CRuntimeClass* pRtcScrollView = impl__GetThisClass_CScrollView__SAPEAUCRuntimeClass__XZ();
    const POINT pt = UnpackPoint(ptPacked);
    BOOL bVertScroll = FALSE;
    int row, col;
    for (row = 0; row < pThis->m_nRows && !bVertScroll; row++) {
        for (col = 0; col < pThis->m_nCols && !bVertScroll; col++) {
            CWnd* pPane = PaneAt(pThis, row, col);
            if (pPane != nullptr && KindOf(pPane, pRtcScrollView)) {
                CScrollBar* pBar = impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(static_cast<CView*>(pPane), SB_VERT);
                if (pBar != nullptr && pBar->m_hWnd && ::IsWindowEnabled(pBar->m_hWnd)) bVertScroll = TRUE;
            }
        }
    }
    for (row = 0; row < pThis->m_nRows; row++) {
        for (col = 0; col < pThis->m_nCols; col++) {
            CWnd* pPane = PaneAt(pThis, row, col);
            if (pPane == nullptr || !KindOf(pPane, pRtcScrollView)) continue;
            CScrollBar* pBar = impl__GetScrollBarCtrl_CView__UEBAPEAVCScrollBar__H_Z(static_cast<CView*>(pPane), bVertScroll ? SB_VERT : SB_HORZ);
            if (pBar == nullptr || !pBar->m_hWnd || !::IsWindowEnabled(pBar->m_hWnd)) continue;
            const int nOldPos = ::GetScrollPos(pBar->m_hWnd, SB_CTL);
            impl__DoMouseWheel_CScrollView__QEAAHIFVCPoint___Z(static_cast<CScrollView*>(pPane), fFlags, zDelta, CPoint(pt.x, pt.y));
            if (bVertScroll ? (col < pThis->m_nCols - 1) : (row < pThis->m_nRows - 1))
                ::SetScrollPos(pBar->m_hWnd, SB_CTL, nOldPos, FALSE);
        }
    }
    return TRUE;
}

// ?OnNcCreate@CSplitterWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z, RVA 0x2aa890 (mfc140u):
//     if (!CWnd::OnNcCreate(lpcs)) return FALSE;                       // == CWnd::Default()
//     CWnd::FromHandle(::GetParent(m_hWnd))->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);   // the splitter draws the 3D look itself
//     return TRUE;
// Symbol: ?OnNcCreate@CSplitterWnd@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnNcCreate_CSplitterWnd__IEAAHPEAUtagCREATESTRUCTW___Z(CSplitterWnd* pThis, CREATESTRUCTW* lpcs) {
    (void)lpcs;
    if (!pThis) return FALSE;
    if (!impl__Default_CWnd__IEAA_JXZ(pThis)) return FALSE;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent) impl__ModifyStyleEx_CWnd__QEAAHKKI_Z(pParent, WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
    return TRUE;
}

// ?OnNotify@CSplitterWnd@@MEAAH_K_JPEA_J@Z, RVA 0x2ad730 (mfc140u):
//     if (CWnd::OnNotify(wParam, lParam, pResult)) return TRUE;
//     CFrameWnd* pFrameWnd = GetParentFrame(); ENSURE_VALID(pFrameWnd);    // throws CInvalidArgException
//     *pResult = ::SendMessage(pFrameWnd->m_hWnd, WM_NOTIFY, wParam, lParam);   // route to the frame
//     return TRUE;
// Symbol: ?OnNotify@CSplitterWnd@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CSplitterWnd__MEAAH_K_JPEA_J_Z(CSplitterWnd* pThis, unsigned __int64 wParam, __int64 lParam, __int64* pResult) {
    if (!pThis) return FALSE;
    if (impl__OnNotify_CWnd__MEAAH_K_JPEA_J_Z(pThis, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam), reinterpret_cast<LRESULT*>(pResult)))
        return TRUE;
    CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (!pFrameWnd) impl__AfxThrowInvalidArgException__YAXXZ();
    const LRESULT lResult = ::SendMessageW(pFrameWnd->m_hWnd, WM_NOTIFY, static_cast<WPARAM>(wParam), static_cast<LPARAM>(lParam));
    if (pResult) *pResult = lResult;   // retail stores unguarded
    return TRUE;
}

// ?OnPaint@CSplitterWnd@@IEAAXXZ, RVA 0x2acef0 (mfc140u):
//     CPaintDC dc(this);
//     OnDraw(&dc);                       // vtable slot 97
// The CPaintDC is built on raw storage through its ctor/dtor exports (the class has
// no C++ ctor in OpenMFC); nothing on the OnDraw path touches its vfptr.
// Symbol: ?OnPaint@CSplitterWnd@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CSplitterWnd__IEAAXXZ(CSplitterWnd* pThis) {
    if (!pThis || !pThis->m_hWnd) return;
    alignas(CPaintDC) unsigned char storage[sizeof(CPaintDC)];
    std::memset(storage, 0, sizeof(storage));
    CPaintDC* pDC = reinterpret_cast<CPaintDC*>(storage);
    impl___0CPaintDC__QEAA_PEAVCWnd___Z(pDC, pThis);
    impl__OnDraw_CSplitterWnd__MEAAXPEAVCDC___Z(pThis, pDC);
    impl___1CPaintDC__UEAA_XZ(pDC);
}

// ?OnPrintClient@CSplitterWnd@@IEAA_JPEAVCDC@@I@Z, RVA 0x2acf60 (mfc140u):
//     if (pDC != NULL && pDC->m_hDC != NULL) { OnDraw(pDC); return 0; }   // slot 97
//     return -1;
// Symbol: ?OnPrintClient@CSplitterWnd@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CSplitterWnd__IEAA_JPEAVCDC__I_Z(CSplitterWnd* pThis, CDC* pDC, unsigned int nFlags) {
    (void)nFlags;
    if (pDC != nullptr && pDC->m_hDC != nullptr) {
        impl__OnDraw_CSplitterWnd__MEAAXPEAVCDC___Z(pThis, pDC);
        return 0;
    }
    return -1;
}

// ?OnSetCursor@CSplitterWnd@@IEAAHPEAVCWnd@@II@Z, RVA 0x2acf90 (mfc140u):
//     if (nHitTest == HTCLIENT && pWnd == this && !m_bTracking) return TRUE;   // handled in OnMouseMove
//     return CWnd::Default();
// Symbol: ?OnSetCursor@CSplitterWnd@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnSetCursor_CSplitterWnd__IEAAHPEAVCWnd__II_Z(CSplitterWnd* pThis, CWnd* pWnd, unsigned int nHitTest, unsigned int message) {
    (void)message;
    if (!pThis) return FALSE;
    if (nHitTest == HTCLIENT && pWnd == pThis && !Track(pThis).m_bTracking) return TRUE;
    return static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis));
}

// ?OnSize@CSplitterWnd@@IEAAXIHH@Z, RVA 0x2ac390 (mfc140u):
//     if (nType != SIZE_MINIMIZED && cx > 0 && cy > 0) RecalcLayout();   // slot 94
//     CWnd::Default();                                                   // tail call
// Symbol: ?OnSize@CSplitterWnd@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CSplitterWnd__IEAAXIHH_Z(CSplitterWnd* pThis, unsigned int nType, int cx, int cy) {
    if (!pThis) return;
    if (nType != SIZE_MINIMIZED && cx > 0 && cy > 0) pThis->RecalcLayout();
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// ?OnSysCommand@CSplitterWnd@@IEAAXI_J@Z, RVA 0x2ad660 (mfc140u):
//     if ((nID & 0xFFF0) == SC_SIZE) {
//         CWnd* pParent = GetSizingParent();
//         if (pParent != NULL) { ::SendMessage(pParent->m_hWnd, WM_SYSCOMMAND, nID, lParam); return; }
//     }
//     CWnd::Default();
// Symbol: ?OnSysCommand@CSplitterWnd@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CSplitterWnd__IEAAXI_J_Z(CSplitterWnd* pThis, unsigned int nID, __int64 lParam) {
    if (!pThis) return;
    if ((nID & 0xFFF0) == SC_SIZE) {
        CWnd* pParent = impl__GetSizingParent_CSplitterWnd__IEAAPEAVCWnd__XZ(pThis);
        if (pParent != nullptr) {
            ::SendMessageW(pParent->m_hWnd, WM_SYSCOMMAND, nID, static_cast<LPARAM>(lParam));
            return;
        }
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// ?OnVScroll@CSplitterWnd@@IEAAXIIPEAVCScrollBar@@@Z, RVA 0x2ada40 (mfc140u): the
// row-wise twin of OnHScroll (row = ctrl id - AFX_IDW_VSCROLL_FIRST, WM_VSCROLL to
// every pane of that row, bar position restored between panes).
// Symbol: ?OnVScroll@CSplitterWnd@@IEAAXIIPEAVCScrollBar@@@Z
extern "C" void MS_ABI impl__OnVScroll_CSplitterWnd__IEAAXIIPEAVCScrollBar___Z(CSplitterWnd* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    if (!pThis || !pScrollBar || !pScrollBar->m_hWnd) return;   // retail dereferences pScrollBar unguarded
    const int row = ::GetDlgCtrlID(pScrollBar->m_hWnd) - static_cast<int>(kIdwVScrollFirst);
    const int nOldPos = ::GetScrollPos(pScrollBar->m_hWnd, SB_CTL);
    const WPARAM wParam = static_cast<WPARAM>(static_cast<int>(MAKELONG(nSBCode, nPos)));
    for (int col = 0; col < pThis->m_nCols; col++) {
        CWnd* pPane = PaneAt(pThis, row, col);
        if (pPane && pPane->m_hWnd)
            ::SendMessageW(pPane->m_hWnd, WM_VSCROLL, wParam, reinterpret_cast<LPARAM>(pScrollBar->m_hWnd));
        if (col < pThis->m_nCols - 1) ::SetScrollPos(pScrollBar->m_hWnd, SB_CTL, nOldPos, FALSE);
    }
}

// ?SetScrollStyle@CSplitterWnd@@QEAAXK@Z, RVA 0x2aacb0 (mfc140u):
//     dwStyle &= WS_HSCROLL|WS_VSCROLL;
//     if (GetScrollStyle() == dwStyle) return;                              // no change
//     m_bHasHScroll = (dwStyle & WS_HSCROLL) != 0;  m_bHasVScroll = (dwStyle & WS_VSCROLL) != 0;
//     for (col = 0; col < m_nCols; col++) {                                 // horizontal bars
//         CWnd* pBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
//         if (pBar == NULL) {
//             if (!CreateScrollBarCtrl(SBS_HORZ, AFX_IDW_HSCROLL_FIRST + col)) AfxThrowResourceException();   // slot 98
//             pBar = GetDlgItem(AFX_IDW_HSCROLL_FIRST + col);
//         }
//         pBar->ShowWindow(m_bHasHScroll ? SW_SHOW : SW_HIDE);
//     }
//     for (row = 0; row < m_nRows; row++) {                                 // vertical bars (SBS_VERT)
//         ...same with AFX_IDW_VSCROLL_FIRST + row, m_bHasVScroll...
//     }
//     if (m_bHasVScroll && m_bHasHScroll) {                                 // the size box
//         CWnd* pBox = GetDlgItem(AFX_IDW_SIZE_BOX);
//         if (pBox == NULL) {
//             if (!CreateScrollBarCtrl(SBS_SIZEBOX|WS_DISABLED, AFX_IDW_SIZE_BOX)) AfxThrowResourceException();
//             pBox = GetDlgItem(AFX_IDW_SIZE_BOX);
//         }
//         pBox->ShowWindow(SW_SHOW);
//     } else {
//         CWnd* pBox = GetDlgItem(AFX_IDW_SIZE_BOX);
//         if (pBox != NULL) pBox->DestroyWindow();                          // slot 26
//     }
// Symbol: ?SetScrollStyle@CSplitterWnd@@QEAAXK@Z
extern "C" void MS_ABI impl__SetScrollStyle_CSplitterWnd__QEAAXK_Z(CSplitterWnd* pThis, unsigned long dwStyle) {
    if (!pThis) return;
    dwStyle &= (WS_HSCROLL | WS_VSCROLL);
    if (impl__GetScrollStyle_CSplitterWnd__QEBAKXZ(pThis) == dwStyle) return;
    pThis->m_bHasHScroll = (dwStyle & WS_HSCROLL) != 0;
    pThis->m_bHasVScroll = (dwStyle & WS_VSCROLL) != 0;
    for (int col = 0; col < pThis->m_nCols; col++) {
        const int nID = static_cast<int>(kIdwHScrollFirst) + col;
        CWnd* pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, nID);
        if (pBar == nullptr) {
            if (!impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(pThis, SBS_HORZ, static_cast<unsigned>(nID)))
                impl__AfxThrowResourceException__YAXXZ();
            pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, nID);
        }
        if (pBar && pBar->m_hWnd) ::ShowWindow(pBar->m_hWnd, pThis->m_bHasHScroll ? SW_SHOW : SW_HIDE);
    }
    for (int row = 0; row < pThis->m_nRows; row++) {
        const int nID = static_cast<int>(kIdwVScrollFirst) + row;
        CWnd* pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, nID);
        if (pBar == nullptr) {
            if (!impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(pThis, SBS_VERT, static_cast<unsigned>(nID)))
                impl__AfxThrowResourceException__YAXXZ();
            pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, nID);
        }
        if (pBar && pBar->m_hWnd) ::ShowWindow(pBar->m_hWnd, pThis->m_bHasVScroll ? SW_SHOW : SW_HIDE);
    }
    if (pThis->m_bHasVScroll && pThis->m_bHasHScroll) {
        CWnd* pBox = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwSizeBox));
        if (pBox == nullptr) {
            if (!impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(pThis, SBS_SIZEBOX | WS_DISABLED, kIdwSizeBox))
                impl__AfxThrowResourceException__YAXXZ();
            pBox = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwSizeBox));
        }
        if (pBox && pBox->m_hWnd) ::ShowWindow(pBox->m_hWnd, SW_SHOW);
    } else {
        CWnd* pBox = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwSizeBox));
        if (pBox != nullptr) impl__DestroyWindow_CWnd__UEAAHXZ(pBox);
    }
}

// ?SplitRow@CSplitterWnd@@UEAAHH@Z, RVA 0x2ab020 (mfc140u):
//     int rowNew = m_nRows;
//     cyBefore -= m_cyBorder;
//     if (cyBefore < m_pRowInfo[rowNew-1].nMinSize) return FALSE;
//     int cyAfter = m_pRowInfo[rowNew-1].nCurSize - m_cySplitter - cyBefore;
//     if (cyAfter < m_pRowInfo[rowNew].nMinSize || cyAfter == -1) return FALSE;    // split too small
//     if (m_bHasVScroll && !CreateScrollBarCtrl(SBS_VERT, AFX_IDW_VSCROLL_FIRST + rowNew)) return FALSE;   // slot 98
//     m_nRows++;                                                    // bump count during view creation
//     for (col = 0; col < m_nCols; col++) {
//         CSize size(m_pColInfo[col].nCurSize, cyAfter);
//         if (!CreateView(rowNew, col, m_pDynamicViewClass, size, NULL)) {         // slot 93
//             while (col > 0) DeleteView(rowNew, --col);            // slot 99: undo the ones that worked
//             if (m_bHasVScroll) GetDlgItem(AFX_IDW_VSCROLL_FIRST + rowNew)->DestroyWindow();   // slot 26
//             m_nRows--;
//             return FALSE;
//         }
//     }
//     m_pRowInfo[rowNew-1].nIdealSize = cyBefore;                  // new parts created - resize to original size
//     m_pRowInfo[rowNew].nIdealSize = cyAfter;
//     RecalcLayout();                                               // slot 94
//     return TRUE;
// Symbol: ?SplitRow@CSplitterWnd@@UEAAHH@Z
extern "C" int MS_ABI impl__SplitRow_CSplitterWnd__UEAAHH_Z(CSplitterWnd* pThis, int cyBefore) {
    if (!pThis) return FALSE;
    SplitterExtra& x = Extra(pThis);
    const int rowNew = pThis->m_nRows;
    if (!x.pRowInfo || !x.pColInfo || rowNew < 1 || rowNew >= pThis->m_nMaxRows) return FALSE;   // retail indexes m_pRowInfo[m_nRows] unguarded
    cyBefore -= x.cyBorder;
    if (cyBefore < x.pRowInfo[rowNew - 1].nMinSize) return FALSE;
    const int cyAfter = x.pRowInfo[rowNew - 1].nCurSize - pThis->m_cySplitter - cyBefore;
    if (cyAfter < x.pRowInfo[rowNew].nMinSize || cyAfter == -1) return FALSE;
    if (pThis->m_bHasVScroll &&
        !impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(pThis, SBS_VERT, kIdwVScrollFirst + static_cast<unsigned>(rowNew)))
        return FALSE;
    pThis->m_nRows++;
    for (int col = 0; col < pThis->m_nCols; col++) {
        SIZE size = { x.pColInfo[col].nCurSize, cyAfter };
        if (!pThis->CreateView(rowNew, col, x.pDynamicViewClass, size, nullptr)) {
            while (col > 0) {
                col--;
                impl__DeleteView_CSplitterWnd__UEAAXHH_Z(pThis, rowNew, col);
            }
            if (pThis->m_bHasVScroll) {
                CWnd* pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwVScrollFirst) + rowNew);
                if (pBar) impl__DestroyWindow_CWnd__UEAAHXZ(pBar);
            }
            pThis->m_nRows--;
            return FALSE;
        }
    }
    x.pRowInfo[rowNew - 1].nIdealSize = cyBefore;
    x.pRowInfo[rowNew].nIdealSize = cyAfter;
    pThis->RecalcLayout();
    return TRUE;
}

// ?SplitColumn@CSplitterWnd@@UEAAHH@Z, RVA 0x2ab1c0 (mfc140u): the column-wise twin
// of SplitRow (m_cxBorder, m_cxSplitter, SBS_HORZ + AFX_IDW_HSCROLL_FIRST + colNew,
// CSize(cxAfter, m_pRowInfo[row].nCurSize), CreateView(row, colNew, ...)).
// Symbol: ?SplitColumn@CSplitterWnd@@UEAAHH@Z
extern "C" int MS_ABI impl__SplitColumn_CSplitterWnd__UEAAHH_Z(CSplitterWnd* pThis, int cxBefore) {
    if (!pThis) return FALSE;
    SplitterExtra& x = Extra(pThis);
    const int colNew = pThis->m_nCols;
    if (!x.pRowInfo || !x.pColInfo || colNew < 1 || colNew >= pThis->m_nMaxCols) return FALSE;   // retail indexes m_pColInfo[m_nCols] unguarded
    cxBefore -= x.cxBorder;
    if (cxBefore < x.pColInfo[colNew - 1].nMinSize) return FALSE;
    const int cxAfter = x.pColInfo[colNew - 1].nCurSize - pThis->m_cxSplitter - cxBefore;
    if (cxAfter < x.pColInfo[colNew].nMinSize || cxAfter == -1) return FALSE;
    if (pThis->m_bHasHScroll &&
        !impl__CreateScrollBarCtrl_CSplitterWnd__UEAAHKI_Z(pThis, SBS_HORZ, kIdwHScrollFirst + static_cast<unsigned>(colNew)))
        return FALSE;
    pThis->m_nCols++;
    for (int row = 0; row < pThis->m_nRows; row++) {
        SIZE size = { cxAfter, x.pRowInfo[row].nCurSize };
        if (!pThis->CreateView(row, colNew, x.pDynamicViewClass, size, nullptr)) {
            while (row > 0) {
                row--;
                impl__DeleteView_CSplitterWnd__UEAAXHH_Z(pThis, row, colNew);
            }
            if (pThis->m_bHasHScroll) {
                CWnd* pBar = impl__GetDlgItem_CWnd__QEBAPEAV1_H_Z(pThis, static_cast<int>(kIdwHScrollFirst) + colNew);
                if (pBar) impl__DestroyWindow_CWnd__UEAAHXZ(pBar);
            }
            pThis->m_nCols--;
            return FALSE;
        }
    }
    x.pColInfo[colNew - 1].nIdealSize = cxBefore;
    x.pColInfo[colNew].nIdealSize = cxAfter;
    pThis->RecalcLayout();
    return TRUE;
}

// ?StartTracking@CSplitterWnd@@MEAAXH@Z, RVA 0x2ab700 (mfc140u):
//     if (ht == noHit) return;
//     GetInsideRect(m_rectLimit);                                   // slot 112; GetHitRect narrows it
//     if (splitterIntersection1..225) {                             // two tracking rectangles
//         int row = (ht - splitterIntersection1) / 15, col = (ht - splitterIntersection1) % 15;
//         GetHitRect(vSplitterBar1 + row, m_rectTracker);           // slot 113
//         int yTrackOffset = m_ptTrackOffset.y;
//         m_bTracking2 = TRUE;
//         GetHitRect(hSplitterBar1 + col, m_rectTracker2);
//         m_ptTrackOffset.y = yTrackOffset;
//     } else if (ht == bothSplitterBox) {                           // keyboard: both split boxes
//         GetHitRect(vSplitterBox, m_rectTracker);
//         int yTrackOffset = m_ptTrackOffset.y;
//         m_bTracking2 = TRUE;
//         GetHitRect(hSplitterBox, m_rectTracker2);
//         m_ptTrackOffset.y = yTrackOffset;
//         m_rectTracker.OffsetRect(0, m_rectLimit.Height() / 2);   // centre them
//         m_rectTracker2.OffsetRect(m_rectLimit.Width() / 2, 0);
//     } else {
//         GetHitRect(ht, m_rectTracker);                            // only one bar
//     }
//     CWnd* pWnd = GetActivePane(NULL, NULL);                       // slot 104
//     if (pWnd != NULL && pWnd->IsKindOf(RUNTIME_CLASS(CView))) {
//         CFrameWnd* pFrameWnd = GetParentFrame(); ENSURE_VALID(pFrameWnd);
//         ((CView*)pWnd)->OnActivateFrame(WA_INACTIVE, pFrameWnd);   // CView slot 103
//     }
//     CWnd::FromHandle(::SetCapture(m_hWnd));  SetFocus();          // steal focus and capture
//     ::RedrawWindow(m_hWnd, NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);   // no pending updates
//     m_bTracking = TRUE;
//     OnInvertTracker(m_rectTracker);                               // slot 96
//     if (m_bTracking2) OnInvertTracker(m_rectTracker2);
//     m_htTrack = ht;
//     SetSplitCursor(ht);                                           // slot 117
// Symbol: ?StartTracking@CSplitterWnd@@MEAAXH@Z
extern "C" void MS_ABI impl__StartTracking_CSplitterWnd__MEAAXH_Z(CSplitterWnd* pThis, int ht) {
    if (!pThis || ht == kNoHit) return;
    SplitterTrackState& t = Track(pThis);
    impl__GetInsideRect_CSplitterWnd__MEBAXAEAVCRect___Z(pThis, &t.m_rectLimit);
    if (IsIsect(ht)) {
        const int row = (ht - kSplitterIntersection1) / 15;
        const int col = (ht - kSplitterIntersection1) % 15;
        impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(pThis, kVSplitterBar1 + row, &t.m_rectTracker);
        const int yTrackOffset = t.m_ptTrackOffset.y;
        t.m_bTracking2 = TRUE;
        impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(pThis, kHSplitterBar1 + col, &t.m_rectTracker2);
        t.m_ptTrackOffset.y = yTrackOffset;
    } else if (ht == kBothSplitterBox) {
        impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(pThis, kVSplitterBox, &t.m_rectTracker);
        const int yTrackOffset = t.m_ptTrackOffset.y;
        t.m_bTracking2 = TRUE;
        impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(pThis, kHSplitterBox, &t.m_rectTracker2);
        t.m_ptTrackOffset.y = yTrackOffset;
        ::OffsetRect(&t.m_rectTracker, 0, (t.m_rectLimit.bottom - t.m_rectLimit.top) / 2);
        ::OffsetRect(&t.m_rectTracker2, (t.m_rectLimit.right - t.m_rectLimit.left) / 2, 0);
    } else {
        impl__GetHitRect_CSplitterWnd__MEAAXHAEAVCRect___Z(pThis, ht, &t.m_rectTracker);
    }
    CWnd* pWnd = static_cast<CWnd*>(impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, nullptr, nullptr));
    if (pWnd != nullptr && KindOf(pWnd, RtcView())) {
        CFrameWnd* pFrameWnd = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
        if (!pFrameWnd) impl__AfxThrowInvalidArgException__YAXXZ();
        impl__OnActivateFrame_CView__MEAAXIPEAVCFrameWnd___Z(static_cast<CView*>(pWnd), WA_INACTIVE, pFrameWnd);
    }
    ::SetCapture(pThis->m_hWnd);
    ::SetFocus(pThis->m_hWnd);
    ::RedrawWindow(pThis->m_hWnd, nullptr, nullptr, RDW_ALLCHILDREN | RDW_UPDATENOW);
    t.m_bTracking = TRUE;
    pThis->OnInvertTracker(CRect(t.m_rectTracker));
    if (t.m_bTracking2) pThis->OnInvertTracker(CRect(t.m_rectTracker2));
    t.m_htTrack = ht;
    pThis->SetSplitCursor(ht);
}

// ?StopTracking@CSplitterWnd@@MEAAXH@Z, RVA 0x2abaf0 (mfc140u):
//     if (!m_bTracking) return;
//     ::ReleaseCapture();
//     OnInvertTracker(m_rectTracker);                               // erase the tracker(s); slot 96
//     if (m_bTracking2) OnInvertTracker(m_rectTracker2);
//     m_bTracking = m_bTracking2 = FALSE;
//     CWnd* pOldActiveView = GetActivePane(NULL, NULL);            // slot 104
//     m_rectTracker.OffsetRect(-CX_BORDER, -CY_BORDER);            // new splitter position, without border
//     m_rectTracker2.OffsetRect(-CX_BORDER, -CY_BORDER);
//     if (bAccept) {
//         if (m_htTrack == vSplitterBox)            SplitRow(m_rectTracker.top);                          // slot 100
//         else if (vSplitterBar1..15)             { TrackRowSize(m_rectTracker.top, m_htTrack - vSplitterBar1); RecalcLayout(); }   // slots 114 / 94
//         else if (m_htTrack == hSplitterBox)       SplitColumn(m_rectTracker.left);                      // slot 101
//         else if (hSplitterBar1..15)             { TrackColumnSize(m_rectTracker.left, m_htTrack - hSplitterBar1); RecalcLayout(); }   // slot 115
//         else if (splitterIntersection1..225)    { int row = (ht - 301) / 15, col = (ht - 301) % 15;
//                                                   TrackRowSize(m_rectTracker.top, row); TrackColumnSize(m_rectTracker2.left, col); RecalcLayout(); }
//         else if (m_htTrack == bothSplitterBox)  { SplitRow(m_rectTracker.top); SplitColumn(m_rectTracker2.left); }
//     }
//     if (pOldActiveView == GetActivePane(NULL, NULL) && pOldActiveView != NULL) {   // put the focus back in the pane
//         SetActivePane(-1, -1, pOldActiveView);                    // slot 105
//         pOldActiveView->SetFocus();
//     }
// Symbol: ?StopTracking@CSplitterWnd@@MEAAXH@Z
extern "C" void MS_ABI impl__StopTracking_CSplitterWnd__MEAAXH_Z(CSplitterWnd* pThis, int bAccept) {
    if (!pThis) return;
    SplitterTrackState& t = Track(pThis);
    if (!t.m_bTracking) return;
    ::ReleaseCapture();
    pThis->OnInvertTracker(CRect(t.m_rectTracker));
    if (t.m_bTracking2) pThis->OnInvertTracker(CRect(t.m_rectTracker2));
    t.m_bTracking = t.m_bTracking2 = FALSE;
    CWnd* pOldActiveView = static_cast<CWnd*>(impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, nullptr, nullptr));
    ::OffsetRect(&t.m_rectTracker, -kCxBorder, -kCyBorder);
    ::OffsetRect(&t.m_rectTracker2, -kCxBorder, -kCyBorder);
    if (bAccept) {
        const int ht = t.m_htTrack;
        if (ht == kVSplitterBox) {
            impl__SplitRow_CSplitterWnd__UEAAHH_Z(pThis, t.m_rectTracker.top);
        } else if (IsVBar(ht)) {
            impl__TrackRowSize_CSplitterWnd__MEAAXHH_Z(pThis, t.m_rectTracker.top, ht - kVSplitterBar1);
            pThis->RecalcLayout();
        } else if (ht == kHSplitterBox) {
            impl__SplitColumn_CSplitterWnd__UEAAHH_Z(pThis, t.m_rectTracker.left);
        } else if (IsHBar(ht)) {
            impl__TrackColumnSize_CSplitterWnd__MEAAXHH_Z(pThis, t.m_rectTracker.left, ht - kHSplitterBar1);
            pThis->RecalcLayout();
        } else if (IsIsect(ht)) {
            const int row = (ht - kSplitterIntersection1) / 15;
            const int col = (ht - kSplitterIntersection1) % 15;
            impl__TrackRowSize_CSplitterWnd__MEAAXHH_Z(pThis, t.m_rectTracker.top, row);
            impl__TrackColumnSize_CSplitterWnd__MEAAXHH_Z(pThis, t.m_rectTracker2.left, col);
            pThis->RecalcLayout();
        } else if (ht == kBothSplitterBox) {
            impl__SplitRow_CSplitterWnd__UEAAHH_Z(pThis, t.m_rectTracker.top);
            impl__SplitColumn_CSplitterWnd__UEAAHH_Z(pThis, t.m_rectTracker2.left);
        }
    }
    if (pOldActiveView == impl__GetActivePane_CSplitterWnd__UEAAPEAVCWnd__PEAH0_Z(pThis, nullptr, nullptr) &&
        pOldActiveView != nullptr) {
        pThis->SetActivePane(-1, -1, pOldActiveView);
        ::SetFocus(pOldActiveView->m_hWnd);
    }
}

// ?TrackColumnSize@CSplitterWnd@@MEAAXHH@Z, RVA 0x2aba20 (mfc140u):
//     CPoint pt(x, 0);
//     ClientToScreen(&pt);
//     GetDlgItem(IdFromRowCol(0, col))->ScreenToClient(&pt);        // relative to the column's first pane
//     m_pColInfo[col].nIdealSize = pt.x;                            // new size
//     if (pt.x < m_pColInfo[col].nMinSize) {                        // resized too small
//         m_pColInfo[col].nIdealSize = 0;                           // make it go away
//         if (GetStyle() & SPLS_DYNAMIC_SPLIT) DeleteColumn(col);   // slot 103
//     } else if (m_pColInfo[col].nCurSize + m_pColInfo[col+1].nCurSize < pt.x + m_pColInfo[col+1].nMinSize) {
//         if (GetStyle() & SPLS_DYNAMIC_SPLIT) DeleteColumn(col + 1);   // not enough room for the next pane
//     }
// Symbol: ?TrackColumnSize@CSplitterWnd@@MEAAXHH@Z
extern "C" void MS_ABI impl__TrackColumnSize_CSplitterWnd__MEAAXHH_Z(CSplitterWnd* pThis, int x, int col) {
    if (!pThis) return;
    SplitterExtra& ex = Extra(pThis);
    if (!ex.pColInfo || col < 0 || col + 1 >= pThis->m_nMaxCols) return;   // retail indexes m_pColInfo[col + 1] unguarded
    POINT pt = { x, 0 };
    ::ClientToScreen(pThis->m_hWnd, &pt);
    CWnd* pPane = PaneAt(pThis, 0, col);
    if (!pPane || !pPane->m_hWnd) return;   // retail dereferences it unguarded
    ::ScreenToClient(pPane->m_hWnd, &pt);
    ex.pColInfo[col].nIdealSize = pt.x;
    if (pt.x < ex.pColInfo[col].nMinSize) {
        ex.pColInfo[col].nIdealSize = 0;
        if (StyleOf(pThis) & kSplsDynamicSplit) impl__DeleteColumn_CSplitterWnd__UEAAXH_Z(pThis, col);
    } else if (ex.pColInfo[col].nCurSize + ex.pColInfo[col + 1].nCurSize < pt.x + ex.pColInfo[col + 1].nMinSize) {
        if (StyleOf(pThis) & kSplsDynamicSplit) impl__DeleteColumn_CSplitterWnd__UEAAXH_Z(pThis, col + 1);
    }
}

// ?TrackRowSize@CSplitterWnd@@MEAAXHH@Z, RVA 0x2ab950 (mfc140u): the row-wise twin
// of TrackColumnSize (CPoint(0, y), pane (row, 0), m_pRowInfo, DeleteRow).
// Symbol: ?TrackRowSize@CSplitterWnd@@MEAAXHH@Z
extern "C" void MS_ABI impl__TrackRowSize_CSplitterWnd__MEAAXHH_Z(CSplitterWnd* pThis, int y, int row) {
    if (!pThis) return;
    SplitterExtra& ex = Extra(pThis);
    if (!ex.pRowInfo || row < 0 || row + 1 >= pThis->m_nMaxRows) return;   // retail indexes m_pRowInfo[row + 1] unguarded
    POINT pt = { 0, y };
    ::ClientToScreen(pThis->m_hWnd, &pt);
    CWnd* pPane = PaneAt(pThis, row, 0);
    if (!pPane || !pPane->m_hWnd) return;   // retail dereferences it unguarded
    ::ScreenToClient(pPane->m_hWnd, &pt);
    ex.pRowInfo[row].nIdealSize = pt.y;
    if (pt.y < ex.pRowInfo[row].nMinSize) {
        ex.pRowInfo[row].nIdealSize = 0;
        if (StyleOf(pThis) & kSplsDynamicSplit) impl__DeleteRow_CSplitterWnd__UEAAXH_Z(pThis, row);
    } else if (ex.pRowInfo[row].nCurSize + ex.pRowInfo[row + 1].nCurSize < pt.y + ex.pRowInfo[row + 1].nMinSize) {
        if (StyleOf(pThis) & kSplsDynamicSplit) impl__DeleteRow_CSplitterWnd__UEAAXH_Z(pThis, row + 1);
    }
}
