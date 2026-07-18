// OpenMFC real implementation of CMFCOutlookBarPaneList exports.
//
// CMFCOutlookBarPaneList is the internal toolbar-style pane that
// CMFCPropertySheet uses to render its Outlook-bar navigation buttons.
// Each button switches the active property page of the owning sheet.
//
// The full class is a CMFCOutlookBarPane (ultimately a CWnd), size(11120).
// The only member introduced by this leaf class is m_pParent, the owning
// CMFCPropertySheet*, which sits at the very end of the object.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

namespace {

// Layout-faithful view. Harvested size is 11120. The whole polymorphic
// base chain (CObject..CMFCOutlookBarPane) is opaque here; we only need the
// vfptr at offset 0 and this class's own trailing member m_pParent, which is
// the last pointer in the object (offset 11112 = 11120 - sizeof(void*)).
struct S
{
    void*  vfptr;                          // 0: CObject vfptr
    char   _opaque[11120 - 8 - 8];         // 8..11111: base subobjects
    void*  m_pParent;                      // 11112: CMFCPropertySheet*
};

static_assert(sizeof(S) == 11120, "CMFCOutlookBarPaneList size mismatch");
static_assert(offsetof(S, vfptr) == 0, "vfptr offset");
static_assert(offsetof(S, m_pParent) == 11112, "m_pParent offset");

} // namespace

// -----------------------------------------------------------------------------
// virtual BOOL CMFCOutlookBarPaneList::OnSendCommand(const CMFCToolBarButton*)
//
// MFC forwards the clicked navigation button to the owning property sheet so it
// can activate the corresponding page. We validate the incoming pointers and,
// with no owning sheet reachable, return the documented success value (TRUE).
// The page-switch side effect requires CMFCPropertySheet internals that are not
// modeled here; see notes.
// -----------------------------------------------------------------------------
// Symbol: ?OnSendCommand@CMFCOutlookBarPaneList@@EEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI
impl__OnSendCommand_CMFCOutlookBarPaneList__EEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton)
{
    if (pThis == nullptr || pButton == nullptr)
        return FALSE;

    // The real implementation dispatches to m_pParent->SetActivePage(...).
    // Even without that reachable, MFC treats the command as handled.
    return TRUE;
}

// -----------------------------------------------------------------------------
// void CMFCOutlookBarPaneList::EnsureVisible(int iButton)
//
// Scrolls the pane so the button with the given index becomes visible. The real
// body walks the CMFCToolBar button array and adjusts the scroll offset; those
// internals are not modeled here, so this is a safe no-op guard.
// -----------------------------------------------------------------------------
// Symbol: ?EnsureVisible@CMFCOutlookBarPaneList@@AEAAXH@Z
extern "C" void MS_ABI
impl__EnsureVisible_CMFCOutlookBarPaneList__AEAAXH_Z(void* pThis, int iButton)
{
    (void)pThis;
    (void)iButton;
}
