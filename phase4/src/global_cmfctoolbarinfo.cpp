#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// Layout-faithful mirror of CMFCToolBarInfo (size 32).
// Eight UINT members, each 4 bytes.
struct S {
    UINT m_uiColdResID;          // +0  Toolbar regular images
    UINT m_uiHotResID;           // +4  Toolbar "hot" images
    UINT m_uiDisabledResID;      // +8  Toolbar disabled images
    UINT m_uiLargeColdResID;     // +12 Toolbar large regular images
    UINT m_uiLargeHotResID;      // +16 Toolbar large "hot" images
    UINT m_uiLargeDisabledResID; // +20 Toolbar large disabled images
    UINT m_uiMenuResID;          // +24 Menu images
    UINT m_uiMenuDisabledResID;  // +28 Menu disabled images
};

static_assert(sizeof(S) == 32, "CMFCToolBarInfo size mismatch");
static_assert(offsetof(S, m_uiColdResID) == 0, "m_uiColdResID offset");
static_assert(offsetof(S, m_uiHotResID) == 4, "m_uiHotResID offset");
static_assert(offsetof(S, m_uiDisabledResID) == 8, "m_uiDisabledResID offset");
static_assert(offsetof(S, m_uiMenuDisabledResID) == 28, "m_uiMenuDisabledResID offset");

// Symbol: ??0CMFCToolBarInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCToolBarInfo__QEAA_XZ(void* pThis)
{
    S* p = static_cast<S*>(pThis);
    // Default ctor initializes every resource id to 0 (no images assigned).
    p->m_uiColdResID          = 0;
    p->m_uiHotResID           = 0;
    p->m_uiDisabledResID      = 0;
    p->m_uiLargeColdResID     = 0;
    p->m_uiLargeHotResID      = 0;
    p->m_uiLargeDisabledResID = 0;
    p->m_uiMenuResID          = 0;
    p->m_uiMenuDisabledResID  = 0;
    return pThis;
}
