#pragma once

#include "openmfc/afxmfc.h"
#include <memory>
#include <type_traits>

namespace openmfc::detail {

CString EncodeTearOffTitle(UINT id, const CString& title);
UINT DecodeTearOffTitle(CString& title);
void SetupActiveTearOffMenus(HMENU menu);
BOOL EnableAppTearOffMenus(void* app, const wchar_t* key, UINT first, UINT last);
void DisableAppTearOffMenus(void* app);
void RetainTearOffID(UINT id, bool retain);
struct TearOffMenuDeleter {
    void operator()(HMENU menu) const { ::DestroyMenu(menu); }
};
using TearOffMenuHandle = std::unique_ptr<std::remove_pointer_t<HMENU>, TearOffMenuDeleter>;

struct TearOffToolbarProfile {
    UINT id = 0;
    CString title;
    CRect bounds;
    bool floating = false;
    TearOffMenuHandle menu;
};

bool CaptureTearOffToolbar(CFrameWnd& frame, CBasePane& pane, TearOffToolbarProfile& profile);
bool RestoreTearOffToolbar(CFrameWnd& frame, CDockingManager& manager, const TearOffToolbarProfile& profile);
bool IsManagedTearOffToolbar(const CBasePane& pane);
void WriteTearOffToolbar(CArchive& archive, const TearOffToolbarProfile& profile);
bool ReadTearOffToolbar(CArchive& archive, TearOffToolbarProfile& profile);
void* FindTearOffFrameImpl(const CFrameWnd* frame);
bool SaveTearOffProfile(CWinAppEx& app, void* frameImpl, const wchar_t* section, bool frameBarsOnly);
bool LoadTearOffProfile(CWinAppEx& app, void* frameImpl, const wchar_t* section);
bool RemoveTearOffProfile(CWinAppEx& app, const wchar_t* section);
bool HasTearOffProfile(CWinAppEx& app, const wchar_t* section);
void TrackTearOffToolbar(CFrameWnd& frame, CBasePane& pane, bool add);

}
