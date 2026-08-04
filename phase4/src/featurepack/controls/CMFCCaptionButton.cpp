// CMFCCaptionButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, ole_olectors_exports.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

























// Symbol: ?GetHit@CMFCCaptionButton@@UEBAIXZ
extern "C" unsigned int MS_ABI impl__GetHit_CMFCCaptionButton__UEBAIXZ(void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }
    auto& state = EnsureCMFCCaptionButtonState(pThis);
    return state.m_hit;
}
// Symbol: ?GetIconID@CMFCCaptionButton@@UEBA?AW4IMAGES_IDS@CMenuImages@@HH@Z
extern "C" void* MS_ABI impl__GetIconID_CMFCCaptionButton__UEBA_AW4IMAGES_IDS_CMenuImages__HH_Z(
    void* pThis, int imageId, unsigned int width, float scale, unsigned short* extra, unsigned char flags,
    void* args0, int args1, unsigned int args2, char args3, void* args4, void* args5,
    float args6, void* args7, void* args8, void* args9, unsigned int args10,
    void* args11, void* args12, void* args13, void* args14, void* args15, int args16, int args17) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCMFCCaptionButtonState(pThis);
    state.m_lastImageId = static_cast<unsigned int>(imageId);
    state.m_lastX = static_cast<int>(width);
    state.m_lastY = static_cast<int>(flags);
    if (extra != nullptr) {
        // Encode a small amount of caller-provided state into returned ID.
        *extra = static_cast<unsigned short>((imageId ^ flags) & 0xFFFF);
    }
    (void)scale;
    (void)args0;
    (void)args1;
    (void)args2;
    (void)args3;
    (void)args4;
    (void)args5;
    (void)args6;
    (void)args7;
    (void)args8;
    (void)args9;
    (void)args10;
    (void)args11;
    (void)args12;
    (void)args13;
    (void)args14;
    (void)args15;
    (void)args16;
    (void)args17;
    return reinterpret_cast<void*>(static_cast<uintptr_t>(state.m_lastImageId));
}
// Symbol: ?GetHit@CMFCCaptionButton@@QEBAIXZ
extern "C" unsigned int MS_ABI impl__GetHit_CMFCCaptionButton__QEBAIXZ(const void* pThis) {
    if (pThis == nullptr) {
        return 0;
    }
    auto& state = EnsureCMFCCaptionButtonState(const_cast<void*>(pThis));
    return state.m_hit;
}
// Symbol: ?OnDraw@CMFCCaptionButton@@UEAAXPEAVCDC@@HHHH@Z
extern "C" void MS_ABI impl__OnDraw_CMFCCaptionButton__UEAAXPEAVCDC__HHHH_Z(
    void* pThis, void* pDC, int x, int y, int width, int height) {
    auto* pDeviceContext = static_cast<CDC*>(pDC);
    if (pDeviceContext == nullptr || pDeviceContext->GetSafeHdc() == nullptr) {
        return;
    }
    if (pThis != nullptr) {
        auto& state = EnsureCMFCCaptionButtonState(pThis);
        state.m_hit = 0;
        state.m_lastX = x;
        state.m_lastY = y;
    }

    RECT rc{ x, y, x + width, y + height };
    HBRUSH brush = ::CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
    if (brush != nullptr) {
        ::FillRect(pDeviceContext->GetSafeHdc(), &rc, brush);
        ::DeleteObject(brush);
    }
    ::DrawFrameControl(pDeviceContext->GetSafeHdc(), &rc, DFC_BUTTON, DFCS_BUTTONPUSH);
}
// Symbol: ??0CMFCCaptionButton@@QEAA@IH@Z
// CMFCCaptionButton::CMFCCaptionButton()
extern "C" void* MS_ABI impl___0CMFCCaptionButton__QEAA_IH_Z(unsigned int p0, int p1) {
    (void)p0;
    (void)p1;
    return new CMFCCaptionButton();
}
