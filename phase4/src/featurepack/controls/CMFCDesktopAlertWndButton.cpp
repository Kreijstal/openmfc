// CMFCDesktopAlertWndButton — OpenMFC implementation.
// Sources: global_cmfcdesktopalertwndbutton.cpp

#include "detail/CMFCDesktopAlertWndButtonSupport.h"

#include <cstddef>

class CMFCVisualManager;
class CMFCDesktopAlertWndButton;
class CDC;

// ABI mirror for CRect's four 32-bit coordinates. A 16-byte CRect argument is
// passed indirectly under MS_ABI, matching the retail visual-manager virtual calls.
class CRect {
public:
    int left, top, right, bottom;
};
static_assert(sizeof(CRect) == 16, "CRect by-value ABI size");
static_assert(offsetof(CRect, left) == 0 && offsetof(CRect, top) == 4 &&
              offsetof(CRect, right) == 8 && offsetof(CRect, bottom) == 12,
              "CRect coordinate offsets");

// The private CMFCVisualManager::GetInstance helper is not exported here; use the
// initialized manager global and dispatch the retail virtual slots directly.
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
extern "C" void MS_ABI
impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState);

namespace {
using ButtonVisualManagerMethod = void (MS_ABI *)(void*, void*, CRect, void*);

void dispatch_button_visual_manager(void* pVisualManager, std::size_t byteOffset,
                                    void* pDC, CRect rectClient, void* pButton)
{
    void** const vtable = *reinterpret_cast<void***>(pVisualManager);
    const auto method = reinterpret_cast<ButtonVisualManagerMethod>(
        vtable[byteOffset / sizeof(void*)]);
    method(pVisualManager, pDC, rectClient, pButton);
}
}

// The mfc140.dll ANSI-twin body at RVA 0x3c640 copies rectClient and dispatches
// CMFCVisualManager vtable slot +0x3e0 with this as pButton. The mfc140u symbol
// map does not resolve this export, so the cited entry address is for mfc140.dll only.
// Deviation: uses the initialized manager global instead of retail's private,
// lazy-creating GetInstance helper; if the global is null, this skips the dispatch.
// The null guards are also additional behavior absent from the retail body.
// Symbol: ?OnFillBackground@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI
impl__OnFillBackground_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEBVCRect___Z(
    void* pThis, void* pDC, const RECT* rectClient)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    void* const pVisualManager = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (pVisualManager == nullptr) return;

    const CRect rectCopy{rectClient->left, rectClient->top, rectClient->right, rectClient->bottom};
    dispatch_button_visual_manager(pVisualManager, 0x3e0, pDC, rectCopy, pThis);
}
// Disassembly of the OnDrawBorder entry at RVA 0x3c850 (mfc140u) copies the
// rect and dispatches CMFCVisualManager vtable slot +0x3e8; uiState is unused.
// Deviation: uses the initialized manager global rather than retail's private,
// lazy-creating GetInstance helper; this skips dispatch when the global is null.
// The null guards are additional behavior absent from retail.
// Symbol: ?OnDrawBorder@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEAVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDrawBorder_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEAVCRect__I_Z(
    void* pThis, void* pDC, RECT* rectClient, unsigned int /*uiState*/)
{
    if (pThis == nullptr || pDC == nullptr || rectClient == nullptr) return;
    void* const pVisualManager = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (pVisualManager == nullptr) return;

    const CRect rectCopy{rectClient->left, rectClient->top, rectClient->right, rectClient->bottom};
    dispatch_button_visual_manager(pVisualManager, 0x3e8, pDC, rectCopy, pThis);
}
// Disassembly of OnDraw at RVA 0x3c8b0 (mfc140u) calls the visual-manager
// button-only virtual at +0x3f0 and returns when it reports handled; otherwise
// it invokes CMFCButton::OnDraw. Uses the initialized manager global instead of
// retail's private, lazy-creating GetInstance helper, so the manager override is
// skipped if the global is null. The null guards likewise differ from retail.
// Symbol: ?OnDraw@CMFCDesktopAlertWndButton@@MEAAXPEAVCDC@@AEBVCRect@@I@Z
extern "C" void MS_ABI
impl__OnDraw_CMFCDesktopAlertWndButton__MEAAXPEAVCDC__AEBVCRect__I_Z(
    void* pThis, void* pDC, const RECT* rect, unsigned int uiState)
{
    if (pThis == nullptr || pDC == nullptr || rect == nullptr) return;

    void* const pVisualManager = impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;
    if (pVisualManager != nullptr) {
        using DrawButtonMethod = int (MS_ABI *)(void*, void*);
        void** const vtable = *reinterpret_cast<void***>(pVisualManager);
        const auto drawButton = reinterpret_cast<DrawButtonMethod>(vtable[0x3f0 / sizeof(void*)]);
        if (drawButton(pVisualManager, pThis) != 0) return;
    }

    impl__OnDraw_CMFCButton__MEAAXPEAVCDC__AEBVCRect__I_Z(pThis, pDC, rect, uiState);
}
