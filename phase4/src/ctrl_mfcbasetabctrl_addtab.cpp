// OpenMFC: CMFCBaseTabCtrl::AddTab — both retail overloads.
//
// These previously sat in the generated thunks.cpp under markers that used
// 32-bit mangling (PAVCWnd@@, PB_W) where retail exports the 64-bit PEAVCWnd@@
// and PEB_W forms. Neither marker named a real export, so the .def aliased each
// export to a correctly-mangled impl__ name that only the stub generator ever
// defined: the exports returned without doing anything while this code sat
// unreachable.
//
// They live in their own translation unit rather than back in thunks.cpp
// because gen_thunks.py does not emit AddTab at all -- regenerating thunks.cpp
// would silently drop them again.
//
// Retail signatures:
//   ?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@IIH@Z
//       void AddTab(CWnd* pTabWnd, UINT uiResTabLabel, UINT uiImageId, BOOL bDetachable)
//   ?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@PEB_WIH@Z
//       void AddTab(CWnd* pTabWnd, LPCTSTR lpszTabLabel, UINT uiImageId, BOOL bDetachable)
//
// The resource-id overload was also one parameter short of its retail signature
// (it took three explicit params where the mangling declares four), which would
// have misaligned every argument had it ever been reachable.

#include <windows.h>
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Retail takes the tab label as a string-resource id. Resolve it the same way
// MFC does, and fall back to no label if the resource is missing rather than
// passing through an uninitialised buffer.
inline bool LoadTabLabel(UINT uiResTabLabel, wchar_t* buf, int cch)
{
    if (!uiResTabLabel) return false;
    const int n = ::LoadStringW(AfxGetResourceHandle(), uiResTabLabel, buf, cch);
    return n > 0;
}

} // namespace

// Symbol: ?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@IIH@Z
extern "C" void MS_ABI impl__AddTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__IIH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* pTabWnd, unsigned int uiResTabLabel,
    unsigned int uiImageId, int bDetachable)
{
    (void)bDetachable;
    if (!pThis) return;

    wchar_t label[256];
    const bool haveLabel = LoadTabLabel(uiResTabLabel, label, 256);
    pThis->AddTab(pTabWnd, haveLabel ? label : nullptr, uiImageId);
}

// Symbol: ?AddTab@CMFCBaseTabCtrl@@UEAAXPEAVCWnd@@PEB_WIH@Z
extern "C" void MS_ABI impl__AddTab_CMFCBaseTabCtrl__UEAAXPEAVCWnd__PEB_WIH_Z(
    CMFCBaseTabCtrl* pThis, CWnd* pTabWnd, const wchar_t* lpszTabLabel,
    unsigned int uiImageId, int bDetachable)
{
    (void)bDetachable;
    if (!pThis) return;
    pThis->AddTab(pTabWnd, lpszTabLabel, uiImageId);
}
