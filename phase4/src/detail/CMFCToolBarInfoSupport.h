#pragma once
// Shared internals of the former global_cmfctoolbarinfo.cpp translation unit.
// Definitions live in detail/CMFCToolBarInfoSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfctoolbarinfo {} } }
using namespace openmfc::detail::cmfctoolbarinfo;
#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout-faithful mirror of CMFCToolBarInfo (size 32).
// Eight UINT members, each 4 bytes.
struct S_Cmfctoolbarinfo {
    UINT m_uiColdResID;          // +0  Toolbar regular images
    UINT m_uiHotResID;           // +4  Toolbar "hot" images
    UINT m_uiDisabledResID;      // +8  Toolbar disabled images
    UINT m_uiLargeColdResID;     // +12 Toolbar large regular images
    UINT m_uiLargeHotResID;      // +16 Toolbar large "hot" images
    UINT m_uiLargeDisabledResID; // +20 Toolbar large disabled images
    UINT m_uiMenuResID;          // +24 Menu images
    UINT m_uiMenuDisabledResID;  // +28 Menu disabled images
};


