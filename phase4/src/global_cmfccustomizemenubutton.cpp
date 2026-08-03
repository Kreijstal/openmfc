// OpenMFC: CMFCCustomizeMenuButton exports.
//
// All 6 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the OpenMFC headers, so `this` is taken as void*
// and the members are reached through raw offsets.  It derives from
// CMFCToolBarMenuButton (296 bytes, declared in afxmfc.h) with its own members
// starting at +0x128; the whole object is 0x148 bytes.  The bodies that need
// the module-global customize maps or the CMFCPopupMenu / visual-manager
// machinery behind InvokeCommand / OnClickMenuItem / OnDraw / OnCalculateSize
// are kept as conservative type-correct versions with a `// TODO(clean-room)`
// marker.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc);

namespace {

struct CZMenuButton : public CMFCToolBarMenuButton {
    UINT    m_nCommandID;      // 0x128
    int     m_b12c;            // 0x12c
    int     m_b130;            // 0x130
    int     m_b134;            // 0x134
    int     m_n138;            // 0x138
    int     m_b13c;            // 0x13c
    void*   m_pToolbar;        // 0x140
};

static_assert(sizeof(CZMenuButton) == 0x148, "CZMenuButton size 0x148");

} // namespace

// CopyFrom(const CMFCToolBarButton& src): base copy plus the seven own ints
// (0x128..0x140).  Transcribed verbatim from retail RVA 0x363c0.
// Symbol: ?CopyFrom@CMFCCustomizeMenuButton@@MEAAXAEBVCMFCToolBarButton@@@Z
extern "C" void MS_ABI impl__CopyFrom_CMFCCustomizeMenuButton__MEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const CMFCToolBarButton& src)
{
    if (pThis == nullptr) return;
    impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
        reinterpret_cast<CMFCToolBarMenuButton*>(pThis),
        reinterpret_cast<const CMFCToolBarButton*>(&src));     // 0x18015c1a0
    CZMenuButton* s = reinterpret_cast<CZMenuButton*>(pThis);
    const CZMenuButton* srcB = reinterpret_cast<const CZMenuButton*>(&src);
    s->m_nCommandID = srcB->m_nCommandID;   // 0x128
    s->m_b12c = srcB->m_b12c;               // 0x12c
    s->m_b130 = srcB->m_b130;               // 0x130
    s->m_b134 = srcB->m_b134;               // 0x134
    s->m_n138 = srcB->m_n138;               // 0x138
    s->m_b13c = srcB->m_b13c;               // 0x13c
    s->m_pToolbar = srcB->m_pToolbar;       // 0x140
}

// CreateObject(): retail (RVA 0x36210) allocates 0x148 bytes and runs the
// constructor (0x180036250 = CMFCToolBarMenuButton base ctor + vftable
// 0x1802e38a0).  The opaque class has no modeled constructor or vftable, so
// nullptr is returned.
// Symbol: ?CreateObject@CMFCCustomizeMenuButton@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CMFCCustomizeMenuButton__SAPEAVCObject__XZ(void)
{
    // TODO(clean-room): partially transcribed -- retail allocates 0x148 bytes
    // and runs the CMFCCustomizeMenuButton constructor (0x180036250, retail
    // vftable 0x1802e38a0); the opaque class has no modeled constructor or
    // vftable, so nullptr (the safe allocation-failure terminal) is returned.
    return nullptr;
}

// DrawCheckBox(CDC* pDC, const CRect& rect, int nCheck): draws the checked
// state of the "customize" popup item.  Retail RVA 0x37160 uses the base
// CMFCToolBarMenuButton::OnDraw (0x18015e300) and the visual-manager checkbox
// helper; not modeled, so the body is conservative.
// Symbol: ?DrawCheckBox@CMFCCustomizeMenuButton@@MEAAXPEAVCDC@@AEBVCRect@@H@Z
extern "C" void MS_ABI impl__DrawCheckBox_CMFCCustomizeMenuButton__MEAAXPEAVCDC__AEBVCRect__H_Z(
    void* pThis, void* /*pDC*/, const CRect* /*rect*/, int /*nCheck*/)
{
    if (pThis == nullptr) return;
    // TODO(clean-room): not transcribed -- depends on the base OnDraw
    // (0x18015e300) and the visual-manager checkbox drawing; not modeled.
}

// IsCommandExist(UINT nID): static -- reports whether the module-global
// CMFCCustomizeMenuButton::m_mapPresentIDs table (0x1803b1c80) contains nID.
// The global map is not modeled, so FALSE is returned.
// Symbol: ?IsCommandExist@CMFCCustomizeMenuButton@@SAHI@Z
extern "C" int MS_ABI impl__IsCommandExist_CMFCCustomizeMenuButton__SAHI_Z(
    unsigned int /*nID*/)
{
    // TODO(clean-room): transcribed partially -- retail looks nID up in the
    // module-global CMap at 0x1803b1c80 (CMap::Lookup 0x18002f610); the global
    // map is not modeled, so the not-found result is returned.
    return 0;
}

// OnCalculateSize(CDC* pDC, CSize sizeButton, BOOL bHorz): returns the button
// size.  Retail RVA 0x36440 delegates to CMFCToolBarButton::OnCalculateSize
// (0x180173560) and folds in the caption-string width.  The hidden return
// slot is the second parameter (after `this`), matching the retail register
// usage.  Conservative: returns {0, 0}.
// Symbol: ?OnCalculateSize@CMFCCustomizeMenuButton@@MEAA?AUtagSIZE@@PEAVCDC@@AEBVCSize@@H@Z
extern "C" SIZE* MS_ABI impl__OnCalculateSize_CMFCCustomizeMenuButton__MEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    void* pThis, SIZE* pRet, void* /*pDC*/, const CSize* /*sizeButton*/, int /*bHorz*/)
{
    if (pThis == nullptr || pRet == nullptr) return pRet;
    // TODO(clean-room): not transcribed -- the retail delegates to the base
    // CMFCToolBarButton::OnCalculateSize (0x180173560) and adds the caption
    // width (CString at +0x38); not modeled.
    pRet->cx = 0;
    pRet->cy = 0;
    return pRet;
}

// OnClickMenuItem(): invoked when the "customize" menu item is clicked -- the
// retail (RVA 0x35850) toggles the command presence in the toolbar's button
// list and refreshes it.  The toolbar internals are not modeled; returns TRUE
// (the retail always returns 1 on the main path).
// Symbol: ?OnClickMenuItem@CMFCCustomizeMenuButton@@MEAAHXZ
extern "C" int MS_ABI impl__OnClickMenuItem_CMFCCustomizeMenuButton__MEAAHXZ(
    void* pThis)
{
    if (pThis == nullptr) return 0;
    // TODO(clean-room): not transcribed -- depends on the toolbar button
    // list at +0x140 (CMFCToolBar::ButtonToIndex / GetButton 0x18014fe00) and
    // the module-global customize state; not modeled.
    return 1;
}
