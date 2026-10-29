// CPreviewViewEx — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, viewrich.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/ViewrichSupport.h"
#include <cstddef>

namespace {
// Observed CPreviewViewEx layout. OpenMFC's runtime-class descriptor records
// 5536 bytes (0x15a0); retail accesses place the toolbar at +0x218, its CWnd::m_hWnd
// at +0x258, cleanup pointers at +0x1568/+0x1570, and cached dimensions at
// +0x1594/+0x1598 (mfc140u destructor RVA 0xc0080; mfc140 ANSI-twin
// SetToolbarSize RVA 0xc0d50).
struct S_CPreviewViewExObservedLayout {
    unsigned char baseThroughField178[0x178];
    void* field_178;
    unsigned char baseGapToToolbar[0x218 - 0x180];
    unsigned char toolbarPrefixToHwnd[0x40];
    HWND toolbarHwnd;
    unsigned char opaqueThroughCleanup[0x1568 - 0x260];
    void* cleanupTarget_1568;
    void* cleanupTarget_1570;
    unsigned char opaqueToCachedDimensions[0x1594 - 0x1578];
    unsigned int cachedDimension_1594;
    unsigned int cachedDimension_1598;
    unsigned char opaqueTail[0x15a0 - 0x159c];
};
static_assert(offsetof(S_CPreviewViewExObservedLayout, field_178) == 0x178,
              "CPreviewViewEx observed pointer field at +0x178");
static_assert(offsetof(S_CPreviewViewExObservedLayout, toolbarPrefixToHwnd) == 0x218,
              "CPreviewViewEx toolbar subobject at +0x218");
static_assert(offsetof(S_CPreviewViewExObservedLayout, toolbarHwnd) == 0x258,
              "toolbar CWnd::m_hWnd at +0x258");
static_assert(offsetof(S_CPreviewViewExObservedLayout, cleanupTarget_1568) == 0x1568,
              "CPreviewViewEx cleanup pointer at +0x1568");
static_assert(offsetof(S_CPreviewViewExObservedLayout, cleanupTarget_1570) == 0x1570,
              "CPreviewViewEx cleanup pointer at +0x1570");
static_assert(offsetof(S_CPreviewViewExObservedLayout, cachedDimension_1594) == 0x1594,
              "CPreviewViewEx cached dimension at +0x1594");
static_assert(offsetof(S_CPreviewViewExObservedLayout, cachedDimension_1598) == 0x1598,
              "CPreviewViewEx cached dimension at +0x1598");
static_assert(sizeof(S_CPreviewViewExObservedLayout) == 0x15a0,
              "CPreviewViewEx runtime-class size is 5536 bytes");
}

// Symbol: ?m_bScaleLargeImages@CPreviewViewEx@@1HA
extern "C" int MS_ABI impl__m_bScaleLargeImages_CPreviewViewEx__1HA = 1;

// Reuse CPreviewView helpers for behavior compatibility where layouts align.
extern "C" int MS_ABI impl__OnCreate_CPreviewView__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct);
extern "C" CObject* MS_ABI impl__CreateObject_CPreviewView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewView__MEAAXII_Z(
    void* pThis, unsigned int nCurrent, unsigned int nLast);
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewView__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC);
extern "C" void MS_ABI impl__OnSize_CPreviewView__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy);

// Symbol: ??0CPreviewViewEx@@IEAA@XZ
extern "C" void* MS_ABI impl___0CPreviewViewEx__IEAA_XZ(void* pThis) {
    return pThis;
}
// Symbol: ?OnUpdatePreviewNumPage@CPreviewViewEx@@IEAAXPEAVCCmdUI@@@Z
extern "C" void MS_ABI impl__OnUpdatePreviewNumPage_CPreviewViewEx__IEAAXPEAVCCmdUI___Z(void*, CCmdUI* ui) {
    if (ui) impl__Enable_CCmdUI__UEAAXH_Z(ui, TRUE);
}
// Symbol: ??1CPreviewViewEx@@MEAA@XZ
extern "C" void MS_ABI impl___1CPreviewViewEx__MEAA_XZ(void* pThis) {
    // Retail mfc140u entry RVA 0xc0080 resets the vptr and decrements a
    // module-global counter. Only when the count reaches zero does it, if
    // non-null, dispatch through the pointer at +0x1568 using vtable byte
    // offset +0x650 (slot 0xca), then conditionally call a helper for +0x1570.
    // It then calls CMFCToolBar::~CMFCToolBar on the subobject at +0x218 and
    // tail-jumps CPreviewView::~CPreviewView (entry RVA 0x280430, mfc140u).
    // The counter and cleanup-target behavior are not modeled here; omitting
    // those conditional steps would make a partial teardown unsafe.
    (void)pThis;
}
// Symbol: ?CreateObject@CPreviewViewEx@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CPreviewViewEx__SAPEAVCObject__XZ()
{
    // The CPreviewViewEx runtime-class descriptor reports 0x15a0 bytes.
    void* p = ::operator new(0x15a0, std::nothrow);
    if (p) {
        impl___0CPreviewViewEx__IEAA_XZ(p);
    }
    return static_cast<CObject*>(p);
}
// Symbol: ?OnCreate@CPreviewViewEx@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CPreviewViewEx__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* lpCreateStruct)
{
    return impl__OnCreate_CPreviewView__IEAAHPEAUtagCREATESTRUCTW___Z(pThis, lpCreateStruct);
}
// Symbol: ?OnDisplayPageNumber@CPreviewViewEx@@MEAAXII@Z
extern "C" void MS_ABI impl__OnDisplayPageNumber_CPreviewViewEx__MEAAXII_Z(
    void* pThis, unsigned int nCurrent, unsigned int nLast)
{
    impl__OnDisplayPageNumber_CPreviewView__MEAAXII_Z(pThis, nCurrent, nLast);
}
// Symbol: ?OnEraseBkgnd@CPreviewViewEx@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CPreviewViewEx__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC)
{
    return impl__OnEraseBkgnd_CPreviewView__IEAAHPEAVCDC___Z(pThis, pDC);
}
// Symbol: ?OnSize@CPreviewViewEx@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CPreviewViewEx__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy)
{
    impl__OnSize_CPreviewView__IEAAXIHH_Z(pThis, nType, cx, cy);
}
// Symbol: ?SetToolbarSize@CPreviewViewEx@@IEAAXXZ
extern "C" void MS_ABI impl__SetToolbarSize_CPreviewViewEx__IEAAXXZ(void* pThis) {
    // mfc140u has no resolved symbol; the ANSI twin mfc140 entry is RVA 0xc0d50.
    // Retail returns when the toolbar HWND at +0x258 is null; otherwise it
    // dispatches toolbar slots +0x4d0 (slot 0x9a) and +0x480 (slot 0x90),
    // gets the parent frame via AFXGetParentFrame, calls GetClientRect and
    // GetWindowRect, converts coordinates with CWnd::ScreenToClient, and calls
    // CWnd::SetWindowPos. It updates the value reached through +0x178 at +0x14c;
    // when the computed dimensions differ from +0x1594/+0x1598, it stores them,
    // dispatches frame slot +0x300 (slot 0x60), and calls UpdateWindow.
    // The toolbar virtual targets at +0x4d0 and +0x480 have no validated
    // project dispatch wrappers here, so leave this stubbed rather than call
    // those slots through an unverified local vtable.
    (void)pThis;
}
