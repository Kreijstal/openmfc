#pragma once
// Shared internals of the former global_cmfcribboncmdui.cpp translation unit.
// Definitions live in detail/CMFCRibbonCmdUISupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcribboncmdui {} } }
using namespace openmfc::detail::cmfcribboncmdui;
// OpenMFC real implementation of CMFCRibbonCmdUI (derives from CCmdUI).
//
// Layout (MSVC /d1reportSingleClassLayout, size 72):
//   0  {vfptr}            (CCmdUI is polymorphic)
//   8  m_nID              (CCmdUI::m_nID, UINT)
//  12  m_nIndex           (UINT)
//  16  m_pMenu            (CMenu*)
//  24  m_pSubMenu         (CMenu*)
//  32  m_pParentMenu      (CMenu*)
//  40  m_nIndexMax        (UINT)
//  44  m_bEnableChanged   (BOOL)
//  48  m_bContinueRouting (BOOL)
//  56  m_pOther           (CCmdTarget*)  [4 bytes tail padding at 52]
//  64  m_pUpdated         (CMFCRibbonBaseElement*)  <- CMFCRibbonCmdUI member
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
    void*    m_pParentMenu;     // 32
    unsigned m_nIndexMax;       // 40
    int      m_bEnableChanged;  // 44
    int      m_bContinueRouting;// 48
    char     _pad0[4];          // 52..55 (align m_pOther to 8)
    void*    m_pOther;          // 56
    void*    m_pUpdated;        // 64
};
} } }


extern void* const g_CMFCRibbonCmdUI_vtbl[4];


// The base ribbon element's SetText is a real exported virtual in the same DLL;
// CMFCRibbonCmdUI::SetText forwards to it. Declared here (no header) so this TU
// stays self-contained and only takes a link-time symbol reference.
extern "C" void MS_ABI
impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(void* pElem, const wchar_t* lpszText);






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


