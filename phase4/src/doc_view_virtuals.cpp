#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// CView base-class default implementations.
//
// These mirror the trivial default behaviour of the MFC CView base class:
//   - OnPrepareDC: the base CView::OnPrepareDC does nothing (only CScrollView et
//     al. set a map mode), so a null-guarded no-op is correct.
//   - OnScroll / OnScrollBy: the base CView does not scroll and returns FALSE.
//   - PreCreateWindow: forwards to the default window-class registration which
//     succeeds; returning TRUE matches the sibling view classes' behaviour.

// Symbol: ?OnPrepareDC@CView@@UEAAXPEAVCDC@@PEAUCPrintInfo@@@Z
extern "C" void MS_ABI impl__OnPrepareDC_CView__UEAAXPEAVCDC__PEAUCPrintInfo___Z(
    CView* /*pThis*/, CDC* /*pDC*/, void* /*pInfo*/) {
    // Base CView performs no DC preparation.
}

// Symbol: ?OnScroll@CView@@UEAAHIIH@Z
extern "C" int MS_ABI impl__OnScroll_CView__UEAAHIIH_Z(
    CView* /*pThis*/, unsigned int /*nScrollCode*/, unsigned int /*nPos*/, int /*bDoScroll*/) {
    // Base CView does not scroll.
    return FALSE;
}

// Symbol: ?OnScrollBy@CView@@UEAAHVCSize@@H@Z
extern "C" int MS_ABI impl__OnScrollBy_CView__UEAAHVCSize__H_Z(
    CView* /*pThis*/, CSize /*sizeScroll*/, int /*bDoScroll*/) {
    // Base CView does not scroll.
    return FALSE;
}

// Symbol: ?PreCreateWindow@CView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(
    CView* /*pThis*/, CREATESTRUCTW* /*cs*/) {
    return TRUE;
}
