#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif
namespace openmfc { namespace detail { namespace wincore {
// Restore the lifetime entries first. The remaining native base-window
// handlers still require their own bodies before they can be wired here.
extern const AFX_MSGMAP_ENTRY g_cwndMsgEntries[]={
    {WM_DESTROY,0,0,0,AfxSig_vv,static_cast<AFX_PMSG>(&CWnd::OnDestroy)},
    {WM_PAINT,0,0,0,AfxSig_vv,static_cast<AFX_PMSG>(&CWnd::OnPaint)},
    {WM_NCDESTROY,0,0,0,AfxSig_vv,static_cast<AFX_PMSG>(&CWnd::OnNcDestroy)},
    {0,0,0,0,AfxSig_end,nullptr}};
struct RootAccess : CCmdTarget { static const AFX_MSGMAP* Root() { return GetThisMessageMap(); } };
const AFX_MSGMAP* AFXAPI gbm_CWnd_base() { return RootAccess::Root(); }
extern const AFX_MSGMAP g_cwndMessageMap={gbm_CWnd_base,g_cwndMsgEntries};
} } }
const AFX_MSGMAP* CWnd::GetThisMessageMap() { return &openmfc::detail::wincore::g_cwndMessageMap; }
// Symbol: ?GetThisMessageMap@CWnd@@KAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetThisMessageMap_CWnd__KAPEBUAFX_MSGMAP__XZ() { return &openmfc::detail::wincore::g_cwndMessageMap; }
// Symbol: ?GetMessageMap@CWnd@@MEBAPEBUAFX_MSGMAP@@XZ
extern "C" const AFX_MSGMAP* MS_ABI impl__GetMessageMap_CWnd__MEBAPEBUAFX_MSGMAP__XZ(const void*) { return &openmfc::detail::wincore::g_cwndMessageMap; }
