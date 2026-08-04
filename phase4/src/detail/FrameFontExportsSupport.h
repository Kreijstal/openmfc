#pragma once
// Shared internals of the former frame_font_exports.cpp translation unit.
// Definitions live in detail/FrameFontExportsSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace framefontexports {} } }
using namespace openmfc::detail::framefontexports;
// Frame-window and font-dialog exported wrappers for remaining common-control shard symbols.

#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include <windows.h>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" LRESULT MS_ABI impl__DefWindowProcW_CWnd__MEAA_JI_K_J_Z(
    CWnd* pThis, UINT message, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__LoadFrame_CFrameWnd__UEAAHIKPEAVCWnd__PEAUCCreateContext___Z(
    CFrameWnd* pThis, UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext);
extern "C" int MS_ABI impl__OnCmdMsg_CFrameWnd__UEAAHIHPEAXPEAUAFX_CMDHANDLERINFO___Z(
    CFrameWnd* pThis, UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
extern "C" int MS_ABI impl__OnCommand_CFrameWnd__MEAAH_K_J_Z(
    CFrameWnd* pThis, WPARAM wParam, LPARAM lParam);
extern "C" int MS_ABI impl__PreCreateWindow_CFrameWnd__MEAAHAEAUtagCREATESTRUCTW___Z(
    CFrameWnd* pThis, CREATESTRUCTW& cs);
extern "C" int MS_ABI impl__PreTranslateMessage_CFrameWnd__UEAAHPEAUtagMSG___Z(
    CFrameWnd* pThis, MSG* pMsg);

namespace openmfc { namespace detail { namespace framefontexports {
struct CFontDialogAccess : CFontDialog {
    using CFontDialog::m_clrResult;
    using CFontDialog::m_lf;
    using CFontDialog::m_nPointSize;
};
} } }

namespace openmfc { namespace detail { namespace framefontexports {
LOGFONTW* FontLog(CFontDialog* pThis);
} } }

namespace openmfc { namespace detail { namespace framefontexports {
const LOGFONTW* FontLog(const CFontDialog* pThis);
} } }

namespace openmfc { namespace detail { namespace framefontexports {
const AFX_MSGMAP* EmptyMessageMap_FrameFontExports();
} } }































































