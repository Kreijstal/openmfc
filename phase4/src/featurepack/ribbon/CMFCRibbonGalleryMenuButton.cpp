// CMFCRibbonGalleryMenuButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>

namespace {
// Retail mfc140u accesses a CMFCRibbonGallery subobject at +0x128 in the
// destructor (RVA 0x114ed0 (mfc140u)) and CopyFrom (RVA 0x115160 (mfc140u)).
// CreateObject (RVA 0x114d10 (mfc140u)) allocates 0x620 bytes. The OpenMFC
// derived class is not declared, so this opaque shadow pins only those observed
// layout boundaries.
struct CMFCRibbonGalleryMenuButtonLayout {
    unsigned char base[0x128];
    unsigned char paletteButton[0x4f8];
};
static_assert(offsetof(CMFCRibbonGalleryMenuButtonLayout, paletteButton) == 0x128,
              "retail palette subobject begins at +0x128");
static_assert(sizeof(CMFCRibbonGalleryMenuButtonLayout) == 0x620,
              "retail CMFCRibbonGalleryMenuButton allocation is 0x620 bytes");
} // namespace

// Symbol: ??0CMFCRibbonGalleryMenuButton@@IEAA@XZ
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryMenuButton__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ??0CMFCRibbonGalleryMenuButton@@QEAA@IHPEB_WAEAVCMFCToolBarImages@@@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryMenuButton__QEAA_IHPEB_WAEAVCMFCToolBarImages___Z(
    void* pThis, unsigned int style, int flag, const wchar_t* text,
    CMFCToolBarImages& images) {
    (void)style;
    (void)flag;
    (void)text;
    (void)images;
    return pThis;
}
// Symbol: ??0CMFCRibbonGalleryMenuButton@@QEAA@IHPEB_WIH@Z
extern "C" void* MS_ABI impl___0CMFCRibbonGalleryMenuButton__QEAA_IHPEB_WIH_Z(
    void* pThis, unsigned int style, int flag, const wchar_t* text,
    unsigned int p4, int p5) {
    (void)style;
    (void)flag;
    (void)text;
    (void)p4;
    (void)p5;
    return pThis;
}
// Symbol: ??1CMFCRibbonGalleryMenuButton@@UEAA@XZ
// Stub: RVA 0x114ed0 (mfc140u) sets the derived vftable, destroys the +0x128
// gallery subobject, then tail-jumps to the base-destructor path. OpenMFC has no
// concrete derived class, and its constructors do not establish the gallery
// subobject lifetime; calling either subobject destructor on that storage is unsafe.
extern "C" void MS_ABI impl___1CMFCRibbonGalleryMenuButton__UEAA_XZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CommonInit@CMFCRibbonGalleryMenuButton@@IEAAXXZ
// Stub: RVA 0x114f00 (mfc140u) branches through parent/global state and may call
// palette vtable slot +0x278; the required class and palette state is unmodeled.
extern "C" void MS_ABI impl__CommonInit_CMFCRibbonGalleryMenuButton__IEAAXXZ(void* pThis) {
    (void)pThis;
}

// Symbol: ?CopyFrom@CMFCRibbonGalleryMenuButton@@UEAAXAEBVCMFCToolBarButton@@@Z
// RVA 0x115160 (mfc140u) first copies the menu-button base, then dispatches the
// embedded gallery's vtable slot +0x2e0. Preserve the implemented base copy;
// OpenMFC's gallery CopyFrom thunk is a stub, so the palette state is omitted.
extern "C" void MS_ABI impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    CMFCToolBarMenuButton* pThis, const CMFCToolBarButton* pSrc);
extern "C" void MS_ABI impl__CopyFrom_CMFCRibbonGalleryMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
    void* pThis, const void* pSrc) {
    impl__CopyFrom_CMFCToolBarMenuButton__UEAAXAEBVCMFCToolBarButton___Z(
        static_cast<CMFCToolBarMenuButton*>(pThis),
        static_cast<const CMFCToolBarButton*>(pSrc));
}

// Symbol: ?CreateObject@CMFCRibbonGalleryMenuButton@@SAPEAVCObject@@XZ
// Stub: RVA 0x114d10 (mfc140u) allocates 0x620 bytes and calls the default
// constructor at RVA 0x114d50 (mfc140u). That constructor's thunk in this file
// only returns pThis; the public constructor thunks here are placeholders too,
// and the absent derived-class layout/vftable prevents constructing this safely.
extern "C" void* MS_ABI impl__CreateObject_CMFCRibbonGalleryMenuButton__SAPEAVCObject__XZ() {
    return nullptr;
}

// Symbol: ?CreatePopupMenu@CMFCRibbonGalleryMenuButton@@UEAAPEAVCMFCPopupMenu@@XZ
// Stub: RVA 0x114fd0 (mfc140u) reads derived-object offsets including +0x378,
// +0x380 and +0x5f0, and makes multiple palette virtual calls; the derived
// object/palette layout is unavailable in OpenMFC, so a partial menu is unsafe.
extern "C" void* MS_ABI impl__CreatePopupMenu_CMFCRibbonGalleryMenuButton__UEAAPEAVCMFCPopupMenu__XZ(
    void* pThis) {
    (void)pThis;
    return nullptr;
}

