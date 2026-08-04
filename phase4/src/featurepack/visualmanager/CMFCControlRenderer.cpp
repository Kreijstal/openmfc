// CMFCControlRenderer — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Symbol: ??0CMFCControlRenderer@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCControlRenderer__QEAA_XZ(void* self) { return new (self) CMFCControlRenderer(); }
// Symbol: ??1CMFCControlRenderer@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCControlRenderer__UEAA_XZ(CMFCControlRenderer* self) { if (self) self->~CMFCControlRenderer(); }
// Symbol: ?Create@CMFCControlRenderer@@UEAAHAEBVCMFCControlRendererInfo@@H@Z
extern "C" int MS_ABI impl__Create_CMFCControlRenderer__UEAAHAEBVCMFCControlRendererInfo__H_Z(CMFCControlRenderer* self, const CMFCControlRendererInfo* info, int flip) { return self && info ? self->Create(*info, flip) : FALSE; }
// Symbol: ?CleanUp@CMFCControlRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__CleanUp_CMFCControlRenderer__UEAAXXZ(CMFCControlRenderer* self) { if (self) self->CleanUp(); }
// Symbol: ?Mirror@CMFCControlRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__Mirror_CMFCControlRenderer__UEAAXXZ(CMFCControlRenderer* self) { if (self) self->Mirror(); }
// Symbol: ?OnSysColorChange@CMFCControlRenderer@@UEAAXXZ
extern "C" void MS_ABI impl__OnSysColorChange_CMFCControlRenderer__UEAAXXZ(CMFCControlRenderer* self) { if (self) self->OnSysColorChange(); }
// Symbol: ?SmoothResize@CMFCControlRenderer@@QEAAHN@Z
extern "C" int MS_ABI impl__SmoothResize_CMFCControlRenderer__QEAAHN_Z(CMFCControlRenderer* self, double scale) { return self ? self->SmoothResize(scale) : FALSE; }
// Symbol: ?Draw@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__Draw_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, unsigned int index, unsigned char alpha) { if (self) self->Draw(dc, rect, index, alpha); }
// Symbol: ?DrawFrame@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__DrawFrame_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, unsigned int index, unsigned char alpha) { if (self) self->DrawFrame(dc, rect, index, alpha); }
// Symbol: ?FillInterior@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@IE@Z
extern "C" void MS_ABI impl__FillInterior_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, unsigned int index, unsigned char alpha) { if (self) self->FillInterior(dc, rect, index, alpha); }
// Symbol: ?FillInterior@CMFCControlRenderer@@UEAAXPEAVCDC@@VCRect@@W4ImageAlignHorz@CMFCToolBarImages@@W4ImageAlignVert@5@IE@Z
extern "C" void MS_ABI impl__FillInterior_CMFCControlRenderer__UEAAXPEAVCDC__VCRect__W4ImageAlignHorz_CMFCToolBarImages__W4ImageAlignVert_5_IE_Z(CMFCControlRenderer* self, CDC* dc, CRect rect, int horz, int vert, unsigned int index, unsigned char alpha) { if (self) self->FillInterior(dc, rect, horz, vert, index, alpha); }
CMFCControlRenderer::CMFCControlRenderer() {
    memset(_controlrenderer_padding, 0, sizeof(_controlrenderer_padding));
    g_controlRendererStates[this] = ControlRendererState();
}
CMFCControlRenderer::~CMFCControlRenderer() {
    g_controlRendererStates.erase(this);
}
BOOL CMFCControlRenderer::Create(const CMFCControlRendererInfo& info, BOOL bFlip) {
    ControlRendererState& state = g_controlRendererStates[this];
    state.info = g_controlRendererInfoStates[const_cast<CMFCControlRendererInfo*>(&info)];
    state.valid = !state.info.resourceId.empty() || state.info.hasImage;
    state.mirrored = bFlip != FALSE;
    return state.valid ? TRUE : FALSE;
}
void CMFCControlRenderer::CleanUp() {
    g_controlRendererStates[this] = ControlRendererState();
}
void CMFCControlRenderer::Draw(CDC* pDC, CRect rect, UINT, BYTE) {
    if (pDC && pDC->GetSafeHdc()) ::Rectangle(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom);
}
void CMFCControlRenderer::DrawFrame(CDC* pDC, CRect rect, UINT, BYTE) {
    if (pDC && pDC->GetSafeHdc()) {
        RECT nativeRect = {rect.left, rect.top, rect.right, rect.bottom};
        ::FrameRect(pDC->GetSafeHdc(), &nativeRect, static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH)));
    }
}
void CMFCControlRenderer::FillInterior(CDC* pDC, CRect rect, UINT, BYTE) {
    if (pDC && pDC->GetSafeHdc()) {
        RECT nativeRect = {rect.left, rect.top, rect.right, rect.bottom};
        ::FillRect(pDC->GetSafeHdc(), &nativeRect, static_cast<HBRUSH>(::GetStockObject(LTGRAY_BRUSH)));
    }
}
void CMFCControlRenderer::FillInterior(CDC* pDC, CRect rect, int, int, UINT index, BYTE alphaSrc) {
    FillInterior(pDC, rect, index, alphaSrc);
}
void CMFCControlRenderer::Mirror() {
    g_controlRendererStates[this].mirrored = !g_controlRendererStates[this].mirrored;
}
void CMFCControlRenderer::OnSysColorChange() {
    auto it = g_controlRendererStates.find(this);
    if (it == g_controlRendererStates.end()) return;
    it->second.valid = it->second.info.hasImage || !it->second.info.resourceId.empty();
    it->second.imageScale = 1.0;
}
BOOL CMFCControlRenderer::SmoothResize(double dblImageScale) {
    g_controlRendererStates[this].imageScale = dblImageScale;
    return TRUE;
}
