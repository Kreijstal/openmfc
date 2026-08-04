#pragma once
// Shared internals of the former global_other-22_impl.cpp translation unit.
// Definitions live in detail/CPreviewViewSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cpreviewview {} } }
using namespace openmfc::detail::cpreviewview;
// CGlobalUtils + CPreviewView exports, wave 22 ("other-22").
//
// 2 CGlobalUtils numeric conversions + 28 CPreviewView methods transcribed
// from the retail mfc140u export (ordinal -> RVA -> objdump).
//
// CPreviewView is NOT declared in the clean-room headers (see
// global_controlbar_keyframe_rtti.cpp: the class is registered with a 536
// = 0x218 byte layout, and the message-map base chains to CScrollView).  All
// CPreviewView methods therefore take `void* pThis` and touch state through
// the member offsets harvested from retail disassembly:
//     0x1DC  int          page-state latch (DoZoom fit mode / OnNumPageChange)
//     0x1E0  int          zoom state (0 = fit page, 1 = zoom, 2 = two-up)
//     0x1E4  int          page count
//     0x1E8  unsigned int current page (1-based)
//     0x1EC  int          two-up / page-change mode flag
//     0x1F0  int          page display width (CalcPageDisplaySize)
//     0x130  void*        CCreateContext::m_pCurrentFrame stored in OnCreate
//     0x138  CView*       m_pPrintView (SetPrintView)
//     0x180  void*        per-page state array (stride 40, unmodeled)
//     0x210  void*        m_pPDC (print-dialog wrapper, unmodeled)
// CWnd::m_hWnd lives at +0x40 (matches the rest of this codebase).
//
// Where the retail body depends on unmodeled state (the page-state array at
// 0x180, the PDC object at 0x210, CScrollView scroll machinery, font/DC
// helpers) the implementation keeps the observable member writes and marks the
// gaps with `// TODO(clean-room)`.

#include "openmfc/afxmfc.h"
#include <oleauto.h>

#include <cstddef>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CWnd / CView / CScrollView forward-declared exports used as bases below.
// CPreviewView::OnCreate (0x180280500) calls CView::OnCreate directly.
extern "C" int MS_ABI impl__OnCreate_CView__IEAAHPEAUtagCREATESTRUCTW___Z(
    CView* pThis, CREATESTRUCTW* pCreateStruct);

// CPreviewView ctor (0x1802802d0): defined in
// manual_small_stub_implementations.cpp; used by CreateObject.
extern "C" void* MS_ABI impl___0CPreviewView__QEAA_XZ(void* pThis);

// Same-file helper (defined below): DoZoom drives the page transition after a
// zoom-state change; OnZoomIn/OnZoomOut/SetZoomState call it.
extern "C" void MS_ABI impl__DoZoom_CPreviewView__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nPage, CPoint pt);

// Field access helpers for the not-yet-declared CPreviewView layout.  All
// offsets come from retail disassembly (see file header).
namespace openmfc { namespace detail { namespace cpreviewview {
int& PV_INT(void* p, size_t off);
} } }
namespace openmfc { namespace detail { namespace cpreviewview {
unsigned int& PV_UINT(void* p, size_t off);
} } }
namespace openmfc { namespace detail { namespace cpreviewview {
void*& PV_PTR(void* p, size_t off);
} } }
namespace openmfc { namespace detail { namespace cpreviewview {
HWND PV_HWND(void* p);
} } }

//=============================================================================
// CGlobalUtils::StringFromCy
//=============================================================================

//=============================================================================
// CGlobalUtils::StringFromDecimal
//=============================================================================

//=============================================================================
// CPreviewView::CalcScaleRatio
//=============================================================================

//=============================================================================
// CPreviewView::CalcPageDisplaySize
//=============================================================================

//=============================================================================
// CPreviewView::SetCurrentPage
//=============================================================================

//=============================================================================
// CPreviewView::OnNextPage
//=============================================================================

//=============================================================================
// CPreviewView::OnPrevPage
//=============================================================================

//=============================================================================
// CPreviewView::OnNumPageChange
//=============================================================================

//=============================================================================
// CPreviewView::OnZoomIn
//=============================================================================

//=============================================================================
// CPreviewView::OnZoomOut
//=============================================================================

//=============================================================================
// CPreviewView::SetZoomState
//=============================================================================

//=============================================================================
// CPreviewView::DoZoom
//=============================================================================

//=============================================================================
// CPreviewView::OnCreate
//=============================================================================

//=============================================================================
// CPreviewView::CreateObject (static)
//=============================================================================

//=============================================================================
// CPreviewView::SetPrintView
//=============================================================================

//=============================================================================
// CPreviewView::OnHScroll
//=============================================================================

//=============================================================================
// CPreviewView::OnSetCursor
//=============================================================================

//=============================================================================
// CPreviewView::OnSize
//=============================================================================

//=============================================================================
// CPreviewView::OnActivateView
//=============================================================================

//=============================================================================
// CPreviewView::OnPreviewClose
//=============================================================================

//=============================================================================
// CPreviewView::OnPreviewPrint
//=============================================================================

//=============================================================================
// CPreviewView::OnDraw
//=============================================================================

//=============================================================================
// CPreviewView::OnEraseBkgnd
//=============================================================================

//=============================================================================
// CPreviewView::OnPrepareDC
//=============================================================================

//=============================================================================
// CPreviewView::OnDisplayPageNumber
//=============================================================================

//=============================================================================
// CPreviewView::FindPageRect
//=============================================================================

//=============================================================================
// CPreviewView::OnLButtonDown
//=============================================================================

//=============================================================================
// CPreviewView::OnVScroll
//=============================================================================

//=============================================================================
// CPreviewView::PositionPage
//=============================================================================

//=============================================================================
// CPreviewView::SetScaledSize
//=============================================================================
