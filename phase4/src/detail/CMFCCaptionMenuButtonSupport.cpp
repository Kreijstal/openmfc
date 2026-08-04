#include "CMFCCaptionMenuButtonSupport.h"

// Implementations this unit calls that are defined with their own class.
extern "C" void MS_ABI
impl___1CMFCCaptionMenuButton__UEAA_XZ(void* /*pThis*/);

static_assert(sizeof(S_Cmfccaptionmenubutton) == 72, "CMFCCaptionMenuButton must be 72 bytes");
static_assert(offsetof(S_Cmfccaptionmenubutton, vfptr) == 0, "vfptr at 0");
static_assert(offsetof(S_Cmfccaptionmenubutton, m_bPushed) == 8, "m_bPushed at 8");
static_assert(offsetof(S_Cmfccaptionmenubutton, m_bEnabled) == 20, "m_bEnabled at 20");
static_assert(offsetof(S_Cmfccaptionmenubutton, m_clrForeground) == 36, "m_clrForeground at 36");
static_assert(offsetof(S_Cmfccaptionmenubutton, m_nMenuResult) == 56, "m_nMenuResult at 56");
static_assert(offsetof(S_Cmfccaptionmenubutton, m_bOSMenu) == 60, "m_bOSMenu at 60");
namespace openmfc { namespace detail { namespace cmfccaptionmenubutton {
HWND WndHandle(void* pWnd) {
    return pWnd ? *(HWND*)((char*)pWnd + 64) : NULL;
}
HDC DcHandle_Cmfccaptionmenubutton(void* pDC) {
    return pDC ? *(HDC*)((char*)pDC + 8) : NULL;
}
CRuntimeClassLite g_rtc = {
    "CMFCCaptionButton", 60, 0xFFFF, 0, 0, 0, 0
};
void* MS_ABI slot_GetRuntimeClass_Cmfccaptionmenubutton(const void* /*pThis*/) {
    return &g_rtc;
}
void MS_ABI slot_Serialize_Cmfccaptionmenubutton(void* pThis, void* pAr) {
    if (!pThis || !pAr) return;
    static_cast<CObject*>(pThis)->CObject::Serialize(*static_cast<CArchive*>(pAr));
}
void MS_ABI slot_AssertValid_Cmfccaptionmenubutton(const void* pThis) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::AssertValid();
}
void MS_ABI slot_Dump_Cmfccaptionmenubutton(const void* pThis, void* /*dc*/) {
    if (!pThis) return;
    static_cast<const CObject*>(pThis)->CObject::Dump();
}
void* MS_ABI slot_GetRect(void* retptr, const void* pThis) {
    // No stored rect in the base; return an empty rect anchored at m_ptOrg.
    const S_Cmfccaptionmenubutton* s = (const S_Cmfccaptionmenubutton*)pThis;
    RECT r = { s->m_ptOrg.x, s->m_ptOrg.y, s->m_ptOrg.x, s->m_ptOrg.y };
    *(RECT*)retptr = r;
    return retptr;
}
unsigned MS_ABI slot_GetIconID(const void* /*pThis*/, int /*bActive*/,
                                      int /*bHorz*/, int /*bMaximized*/,
                                      int /*bDisabled*/) {
    return 0;
}
void InitDefaults(S_Cmfccaptionmenubutton* self) {
    // Default values verified byte-exact against real mfc140u (differential dump):
    // only m_bEnabled, m_clrForeground(-1) and m_bOSMenu are non-zero.
    self->vfptr            = (void*)&g_CMFCCaptionMenuButton_vtbl[0];
    self->m_bPushed        = FALSE;
    self->m_bFocused       = FALSE;
    self->m_bHidden        = FALSE;
    self->m_bEnabled       = TRUE;
    self->m_bDroppedDown   = FALSE;
    self->m_bLeftAlign     = FALSE;
    self->m_nHit           = 0;
    self->m_clrForeground  = (COLORREF)-1;
    self->m_ptOrg.x = self->m_ptOrg.y = 0;
    self->m_bIsMiniFrameButton = FALSE;
    self->_pad0            = 0;
    self->m_nMenuResult    = 0;
    self->m_bOSMenu        = TRUE;
    self->m_bMenuIsActive  = FALSE;
    self->_pad1            = 0;
}
void* MS_ABI slot_vecDelDtor_Cmfccaptionmenubutton(void* pThis, unsigned flags) {
    impl___1CMFCCaptionMenuButton__UEAA_XZ(pThis);
    if (flags & 1)
        ::operator delete(pThis);
    return pThis;
}
} } }  // namespace openmfc::detail::cmfccaptionmenubutton
