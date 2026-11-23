// CMFCCaptionButton — OpenMFC implementation.

#include "openmfc/afxmfc.h"

#include <new>

#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

extern "C" int impl__m_nButtonMargin_CMFCCaptionButton__2HA;
extern "C" int impl__m_nButtonMarginVert_CMFCCaptionButton__2HA;
extern "C" CSize* MS_ABI impl__Size_CMenuImages__SA_AVCSize__XZ(CSize*);
extern "C" void MS_ABI impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
    CDC*, int, const CRect*, int, const CSize*);

CSize CMFCCaptionButton::GetSize() {
    CSize glyph(9, 9);
    impl__Size_CMenuImages__SA_AVCSize__XZ(&glyph);
    if (glyph.cx <= 0 || glyph.cy <= 0) glyph = CSize(9, 9);
    return CSize(glyph.cx + 2 * impl__m_nButtonMargin_CMFCCaptionButton__2HA,
                 glyph.cy + 2 * impl__m_nButtonMarginVert_CMFCCaptionButton__2HA);
}

CRect CMFCCaptionButton::GetRect() const {
    return CRect(m_ptOrg, m_bHidden ? CSize(0, 0) : GetSize());
}

CMenuImages::IMAGES_IDS CMFCCaptionButton::GetIconID(BOOL horizontal, BOOL maximized) const {
    int image = -1;
    switch (m_nHit) {
    case 8: image = horizontal ? (maximized ? 9 : 1) : (maximized ? 13 : 7); break;
    case 9: image = maximized ? 15 : 16; break;
    case HTCLOSE:
    case HTOBJECT: image = 5; break;
    case 23: image = 27; break;
    case 24: image = 28; break;
    case 25: image = 13; break;
    }
    return static_cast<CMenuImages::IMAGES_IDS>(image);
}

void CMFCCaptionButton::OnDraw(CDC* dc, BOOL, BOOL horizontal, BOOL maximized, BOOL disabled) {
    if (!dc || !dc->GetSafeHdc() || m_bHidden) return;
    const HDC hdc = dc->GetSafeHdc();
    CRect bounds = GetRect();
    const bool inactive = disabled || !m_bEnabled;
    const int image = GetIconID(horizontal, maximized);
    if (image == 5) {
        UINT flags = DFCS_CAPTIONCLOSE;
        if (inactive) flags |= DFCS_INACTIVE;
        if (m_bPushed) flags |= DFCS_PUSHED;
        if (m_bFocused) flags |= DFCS_HOT;
        ::DrawFrameControl(hdc, bounds, DFC_CAPTION, flags);
    } else if (image >= 0) {
        if (m_bFocused || m_bPushed)
            ::DrawEdge(hdc, bounds, m_bPushed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);
        const CSize natural(0, 0);
        impl__Draw_CMenuImages__SAXPEAVCDC__W4IMAGES_IDS_1_AEBVCRect__W4IMAGE_STATE_1_AEBVCSize___Z(
            dc, image, &bounds, inactive ? 1 : 0, &natural);
    }
}

// Symbol: ?GetHit@CMFCCaptionButton@@UEBAIXZ
extern "C" UINT MS_ABI impl__GetHit_CMFCCaptionButton__UEBAIXZ(const CMFCCaptionButton* button) {
    return button ? button->m_nHit : 0;
}

// Symbol: ?GetHit@CMFCCaptionButton@@QEBAIXZ
extern "C" UINT MS_ABI impl__GetHit_CMFCCaptionButton__QEBAIXZ(const CMFCCaptionButton* button) {
    return button ? button->m_nHit : 0;
}

// Symbol: ?GetIconID@CMFCCaptionButton@@UEBA?AW4IMAGES_IDS@CMenuImages@@HH@Z
extern "C" int MS_ABI impl__GetIconID_CMFCCaptionButton__UEBA_AW4IMAGES_IDS_CMenuImages__HH_Z(
    const CMFCCaptionButton* button, BOOL horizontal, BOOL maximized) {
    return button ? button->CMFCCaptionButton::GetIconID(horizontal, maximized) : -1;
}

// Symbol: ?OnDraw@CMFCCaptionButton@@UEAAXPEAVCDC@@HHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCaptionButton__UEAAXPEAVCDC__HHHH_Z(
    CMFCCaptionButton* button, CDC* dc, BOOL active, BOOL horizontal, BOOL maximized, BOOL disabled) {
    if (button) button->CMFCCaptionButton::OnDraw(dc, active, horizontal, maximized, disabled);
}

// Symbol: ??0CMFCCaptionButton@@QEAA@IH@Z
extern "C" void* MS_ABI impl___0CMFCCaptionButton__QEAA_IH_Z(void* storage, UINT hit, BOOL leftAlign) {
    return storage ? ::new (storage) CMFCCaptionButton(hit, leftAlign) : nullptr;
}
