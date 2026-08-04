#pragma once
// Shared internals of the former global_ctestcmdui.cpp translation unit.
// Definitions live in detail/CTestCmdUISupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace ctestcmdui {} } }
using namespace openmfc::detail::ctestcmdui;
// OpenMFC implementation of CTestCmdUI.
//
// CTestCmdUI is a CCmdUI-like helper used by test harness code. The behavior here
// mirrors CCmdUI's exported virtuals and keeps layout aligned with the existing
// shim representation used in menucore.cpp.

#include <cstddef>
#include <cstring>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

#include "openmfc/afxole.h"


namespace openmfc { namespace detail { namespace ctestcmdui {
struct CCmdUIShim_Ctestcmdui {
    void* const* vfptr;
    unsigned m_nID;
    CCmdUI* m_pOther;
    int m_nIndex;
    CMenu* m_pMenu;
    CMenu* m_pSubMenu;
    char _padding[8];
};
} } }


extern void* const g_CTestCmdUI_vtbl[4];








namespace openmfc { namespace detail { namespace ctestcmdui {
void MS_ABI vt_Enable_Ctestcmdui(void* pThis, int bOn);
} } }

namespace openmfc { namespace detail { namespace ctestcmdui {
void MS_ABI vt_SetCheck_Ctestcmdui(void* pThis, int nCheck);
} } }

namespace openmfc { namespace detail { namespace ctestcmdui {
void MS_ABI vt_SetRadio_Ctestcmdui(void* pThis, int bOn);
} } }

namespace openmfc { namespace detail { namespace ctestcmdui {
void MS_ABI vt_SetText_Ctestcmdui(void* pThis, const wchar_t* text);
} } }

extern void* const g_CTestCmdUI_vtbl[4];

