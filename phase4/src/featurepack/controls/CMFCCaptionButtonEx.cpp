// CMFCCaptionButtonEx — OpenMFC implementation.
// Sources: global_cmfccaptionbuttonex.cpp

#include "detail/CMFCCaptionButtonExSupport.h"

// Symbol: ??0CMFCCaptionButtonEx@@QEAA@I@Z
// CMFCCaptionButtonEx::CMFCCaptionButtonEx(UINT nHit)
//   Chains the base CMFCCaptionButton(nHit) construction: records the hit-test
//   code and installs the framework default state (enabled, not pushed/focused/
//   hidden/dropped, left-aligned off), then clears the derived caption rect.
extern "C" void* MS_ABI impl___0CMFCCaptionButtonEx__QEAA_I_Z(void* pThis, unsigned int nHit) {
    S_Cmfccaptionbuttonex* self = (S_Cmfccaptionbuttonex*)pThis;
    self->vfptr             = (void*)&g_CMFCCaptionButtonEx_vtbl[0];
    self->m_bPushed         = FALSE;
    self->m_bFocused        = FALSE;
    self->m_bHidden         = FALSE;
    self->m_bEnabled        = TRUE;
    self->m_bDroppedDown    = FALSE;
    self->m_bLeftAlign      = FALSE;
    self->m_nHit            = nHit;
    self->m_clrForeground   = (COLORREF)-1;
    self->m_ptOrg_x         = 0;
    self->m_ptOrg_y         = 0;
    self->m_bIsMiniFrameButton = FALSE;
    self->m_pad             = 0;
    self->m_rect.left = self->m_rect.top = self->m_rect.right = self->m_rect.bottom = 0;
    return pThis;
}
// Symbol: ??1CMFCCaptionButtonEx@@UEAA@XZ
// CMFCCaptionButtonEx::~CMFCCaptionButtonEx()
//   The caption button owns no heap/GDI resources (the rect and scalar state
//   are value members), so destruction only keeps the vfptr valid through the
//   base-class destructor chain.
extern "C" void MS_ABI impl___1CMFCCaptionButtonEx__UEAA_XZ(void* pThis) {
    S_Cmfccaptionbuttonex* self = (S_Cmfccaptionbuttonex*)pThis;
    self->vfptr = (void*)&g_CMFCCaptionButtonEx_vtbl[0];
}

// CMFCCaptionButtonEx's MSVC-layout vtable: defined here, next to the
// implementations it points at.
namespace openmfc { namespace detail { namespace cmfccaptionbuttonex {
extern void* const g_CMFCCaptionButtonEx_vtbl[8] = {
    (void*)&vt_GetRuntimeClass_Cmfccaptionbuttonex, // 0 CObject::GetRuntimeClass
    (void*)&vt_vdtor_Cmfccaptionbuttonex,           // 1 CMFCCaptionButtonEx::{dtor}
    (void*)&vt_Serialize_Cmfccaptionbuttonex,       // 2 CObject::Serialize
    (void*)&vt_AssertValid_Cmfccaptionbuttonex,     // 3 CObject::AssertValid
    (void*)&vt_Dump_Cmfccaptionbuttonex,            // 4 CObject::Dump
    (void*)&vt_GetRect,         // 5 CMFCCaptionButtonEx::GetRect
    (void*)&vt_OnDraw,          // 6 CMFCCaptionButton::OnDraw
    (void*)&vt_GetIconID,       // 7 CMFCCaptionButton::GetIconID
};
} } }
