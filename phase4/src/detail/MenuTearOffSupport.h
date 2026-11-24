#pragma once

#include "openmfc/afxmfc.h"

namespace openmfc::detail {

UINT DecodeTearOffTitle(CString& title);
void SetupActiveTearOffMenus(HMENU menu);
BOOL EnableAppTearOffMenus(void* app, const wchar_t* key, UINT first, UINT last);
void DisableAppTearOffMenus(void* app);
void RetainTearOffID(UINT id, bool retain);
void TrackTearOffToolbar(CFrameWnd& frame, CBasePane& pane, bool add);

}
