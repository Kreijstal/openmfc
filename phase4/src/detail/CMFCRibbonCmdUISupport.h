#pragma once
// Shared internals of the former global_cmfcribboncmdui.cpp translation unit.
// Definitions live in detail/CMFCRibbonCmdUISupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcribboncmdui {} } }
using namespace openmfc::detail::cmfcribboncmdui;
// OpenMFC real implementation of CMFCRibbonCmdUI (derives from CCmdUI).
//
// Native CCmdUI prefix: ID@8, index@12, menu@16, submenu@24,
// other window@32, enable-changed@40, continuation@44, maximum index@48,
// parent menu@56. The updated ribbon element follows at64.
//
// vtable order (harvested): Enable, SetCheck, SetRadio, SetText.
// CCmdUI has no virtual destructor, so there is no deleting-dtor slot.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif


namespace openmfc { namespace detail { namespace cmfcribboncmdui {
struct S_Cmfcribboncmdui {
    void*    vfptr;              // 0
    unsigned m_nID;             // 8
    unsigned m_nIndex;          // 12
    void*    m_pMenu;           // 16
    void*    m_pSubMenu;        // 24
    void*    m_pOther;          // 32
    int      m_bEnableChanged;  // 40
    int      m_bContinueRouting;// 44
    unsigned m_nIndexMax;       // 48
    unsigned _pad0;            // 52
    void*    m_pParentMenu;     // 56
    void*    m_pUpdated;        // 64
};
} } }


extern void* const g_CMFCRibbonCmdUI_vtbl[4];


// ---- vtable slot wrappers (MS_ABI virtual dispatch order) ----
namespace openmfc { namespace detail { namespace cmfcribboncmdui {
void MS_ABI vt_Enable_Cmfcribboncmdui(void* p, int b);
} } }
namespace openmfc { namespace detail { namespace cmfcribboncmdui {
void MS_ABI vt_SetCheck_Cmfcribboncmdui(void* p, int c);
} } }
namespace openmfc { namespace detail { namespace cmfcribboncmdui {
void MS_ABI vt_SetRadio_Cmfcribboncmdui(void* p, int b);
} } }
namespace openmfc { namespace detail { namespace cmfcribboncmdui {
void MS_ABI vt_SetText_Cmfcribboncmdui(void* p, const wchar_t* t);
} } }

extern void* const g_CMFCRibbonCmdUI_vtbl[4];


