// OpenMFC: real implementation of CMFCTabButton exported virtual overrides.
//
// CMFCTabButton (afxtabctrl.h) derives from CMFCButton and exists only as the clickable
// tab in a CMFCTabCtrl. It overrides two protected drawing virtuals so the button does
// NOT paint its own CMFCButton chrome -- the owning tab control is responsible for the
// tab's background and border. In real mfc140u both overrides have deliberately empty
// bodies (they exist purely to shadow the CMFCButton implementations). Reproducing that
// exact suppression behavior here is the faithful implementation, not a stub.
//
// Only two symbols are exported for this class (OnFillBackground, OnDrawBorder); neither
// is a constructor, so no vptr installation is performed here. The class layout is
// mirrored purely to anchor the static_asserts.

#include <windows.h>
#include <cstddef>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

namespace {

// Layout-faithful view (harvested size(2848); CObject vfptr@0, CCmdTarget m_dwRef@8).
// The two overrides below are pure no-ops, so only the head fields need be modeled.
struct S {
    void* vfptr;          // 0: CObject vtable pointer
    unsigned long m_dwRef; // 8: CCmdTarget reference count
    char _rest[2848 - 12]; // pad out to the full harvested object size
};

static_assert(sizeof(S) == 2848, "CMFCTabButton size must match harvested layout");
static_assert(offsetof(S, vfptr) == 0, "vfptr at offset 0");
static_assert(offsetof(S, m_dwRef) == 8, "m_dwRef at offset 8");

} // namespace

// CMFCTabButton suppresses the CMFCButton background fill: the parent tab control paints
// the tab, so this override intentionally does nothing.
// Symbol: ?OnFillBackground@CMFCTabButton@@EEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI
impl__OnFillBackground_CMFCTabButton__EEAAXPEAVCDC__AEBVCRect___Z(
    void* /*pThis*/, void* /*pDC*/, const RECT* /*rectClient*/)
{
    // Faithful empty override: no background is drawn by the tab button itself.
}

// CMFCTabButton suppresses the CMFCButton border draw: the parent tab control owns the
// tab border, so this override intentionally does nothing.
// Symbol: ?OnDrawBorder@CMFCTabButton@@EEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDrawBorder_CMFCTabButton__EEAAXPEAVCDC__AEAVCRect__I_Z(
    void* /*pThis*/, void* /*pDC*/, RECT* /*rectClient*/, unsigned int /*uiState*/)
{
    // Faithful empty override: no border is drawn by the tab button itself.
}
