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

namespace {

struct S {
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

static_assert(sizeof(S) == 72, "CMFCRibbonCmdUI size must be 72");
static_assert(offsetof(S, m_nID) == 8, "m_nID @8");
static_assert(offsetof(S, m_bEnableChanged) == 44, "m_bEnableChanged @44");
static_assert(offsetof(S, m_pUpdated) == 64, "m_pUpdated @64");

extern void* const g_CMFCRibbonCmdUI_vtbl[4];

} // namespace

// The base ribbon element's SetText is a real exported virtual in the same DLL;
// CMFCRibbonCmdUI::SetText forwards to it. Declared here (no header) so this TU
// stays self-contained and only takes a link-time symbol reference.
extern "C" void MS_ABI
impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(void* pElem, const wchar_t* lpszText);

// Symbol: ?Enable@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn)
{
    S* p = static_cast<S*>(pThis);
    // CCmdUI::Enable core behavior: record that an explicit enable state was set,
    // so the update machinery knows the handler took control of the item.
    p->m_bEnableChanged = TRUE;
    // The ribbon element itself is enabled/disabled by the ribbon update pass;
    // there is no reachable per-element enable mutator to forward to here.
    (void)bOn;
}

// Symbol: ?SetCheck@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int nCheck)
{
    S* p = static_cast<S*>(pThis);
    // nCheck: 0 => off, 1 => on, 2 => indeterminate. Real MFC pushes the checked
    // state onto the associated ribbon element; that element mutator is not part
    // of the exported surface, so we validate and guard the target here.
    if (p->m_pUpdated != nullptr) {
        // no reachable element check-state setter to forward to
        (void)nCheck;
    }
}

// Symbol: ?SetRadio@CMFCRibbonCmdUI@@UEAAXH@Z
extern "C" void MS_ABI impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(void* pThis, int bOn)
{
    // CCmdUI::SetRadio simply forwards to SetCheck with the boolean state.
    impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(pThis, bOn ? 1 : 0);
}

// Symbol: ?SetText@CMFCRibbonCmdUI@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(void* pThis, const wchar_t* lpszText)
{
    S* p = static_cast<S*>(pThis);
    // Forward the new text to the associated ribbon element, which owns the
    // displayed caption (real MFC calls m_pUpdated->SetText).
    if (p->m_pUpdated != nullptr) {
        impl__SetText_CMFCRibbonBaseElement__UEAAXPEB_W_Z(p->m_pUpdated, lpszText);
    }
}

namespace {

// ---- vtable slot wrappers (MS_ABI virtual dispatch order) ----
void MS_ABI vt_Enable(void* p, int b)  { impl__Enable_CMFCRibbonCmdUI__UEAAXH_Z(p, b); }
void MS_ABI vt_SetCheck(void* p, int c){ impl__SetCheck_CMFCRibbonCmdUI__UEAAXH_Z(p, c); }
void MS_ABI vt_SetRadio(void* p, int b){ impl__SetRadio_CMFCRibbonCmdUI__UEAAXH_Z(p, b); }
void MS_ABI vt_SetText(void* p, const wchar_t* t){ impl__SetText_CMFCRibbonCmdUI__UEAAXPEB_W_Z(p, t); }

void* const g_CMFCRibbonCmdUI_vtbl[4] = {
    reinterpret_cast<void*>(&vt_Enable),
    reinterpret_cast<void*>(&vt_SetCheck),
    reinterpret_cast<void*>(&vt_SetRadio),
    reinterpret_cast<void*>(&vt_SetText),
};

} // namespace

// Symbol: ??0CMFCRibbonCmdUI@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonCmdUI__QEAA_XZ(void* pThis)
{
    S* p = static_cast<S*>(pThis);
    p->vfptr             = reinterpret_cast<void*>(const_cast<void**>(&g_CMFCRibbonCmdUI_vtbl[0]));
    p->m_nID             = 0;
    p->m_nIndex          = 0;
    p->m_pMenu           = nullptr;
    p->m_pSubMenu        = nullptr;
    p->m_pParentMenu     = nullptr;
    p->m_nIndexMax       = 0;
    p->m_bEnableChanged  = FALSE;
    p->m_bContinueRouting= FALSE;
    p->m_pOther          = nullptr;
    p->m_pUpdated        = nullptr;   // CMFCRibbonCmdUI() { m_pUpdated = NULL; }
    return pThis;
}
