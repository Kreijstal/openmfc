// CMFCBaseToolBar — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp, ole_olectors_exports.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

#include "openmfc/afxole.h"
#include "openmfc/afxmfc.h"

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40");
static_assert(offsetof(CPane, m_pData) == 0x1c0, "CPane::m_pData @0x1c0");
static_assert(offsetof(CPane, m_rectVirtual) == 0x1e0, "CPane::m_rectVirtual @0x1e0");

























// Symbol: ??1CMFCBaseToolBar@@UEAA@XZ
// Retail (RVA 0x1c840, mfc140u) installs the CMFCBaseToolBar vftable, then
// tail-jumps to CPane::~CPane (RVA 0x9f490, mfc140u), which frees m_pData
// before destroying its embedded members and CBasePane. The local CPane
// destructor omits that free, so CMFCBaseToolBar releases the allocation here;
// the compiler performs the remaining base-destructor sequence.
extern "C" void MS_ABI impl___1CMFCBaseToolBar__UEAA_XZ(void* pThis) {
    static_cast<CMFCBaseToolBar*>(pThis)->CMFCBaseToolBar::~CMFCBaseToolBar();
}
// Symbol: ?OnAfterStretch@CMFCBaseToolBar@@MEAAXH@Z
// Retail (RVA 0x1c860, mfc140u) compares GetClientRect's width with
// m_rectVirtual's width; if they differ it calls InvalidateRect(hwnd, NULL,
// TRUE) and UpdateWindow(hwnd). The width argument is not read. The IAT slots
// at mfc140u 0x2c7330, 0x2c7128 and 0x2c7300 resolve to GetClientRect,
// InvalidateRect and UpdateWindow respectively.
extern "C" void MS_ABI impl__OnAfterStretch_CMFCBaseToolBar__MEAAXH_Z(
    void* pThis, int width) {
    (void)width;
    CPane* pPane = static_cast<CPane*>(pThis);
    RECT rectClient{};
    ::GetClientRect(pPane->m_hWnd, &rectClient);
    if (rectClient.right - rectClient.left !=
        pPane->m_rectVirtual.right - pPane->m_rectVirtual.left) {
        ::InvalidateRect(pPane->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pPane->m_hWnd);
    }
}
// Symbol: ?OnAfterChangeParent@CMFCBaseToolBar@@UEAAXPEAVCWnd@@@Z
// disas.py's symbol map omits this export, but ordinal 8631 maps to RVA 0x27d0
// (mfc140u), whose entry is a bare return. Keep this as an honest no-op stub.
extern "C" void MS_ABI impl__OnAfterChangeParent_CMFCBaseToolBar__UEAAXPEAVCWnd___Z(
    void* pThis, void* pWnd) {
    (void)pThis;
    (void)pWnd;
}
// Symbol: ?CreateObject@CMFCBaseToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCBaseToolBar__SAPEAVCObject__XZ() {
    return new (std::nothrow) CMFCBaseToolBarObject();
}
CMFCBaseToolBar::CMFCBaseToolBar() {
    std::lock_guard<std::mutex> lock(g_paneCoreStateMutex_Mfccore);
    PaneCoreState& state = g_paneCoreState_Mfccore[this];
    state.canFloat = TRUE;
    state.canAutoHide = FALSE;
    state.hasGripper = FALSE;
    state.autoHideMode = FALSE;
    state.tabbed = FALSE;
}
CMFCBaseToolBar::~CMFCBaseToolBar() {
    if (m_pData != nullptr) std::free(m_pData);
}
// Symbol: ??0CMFCBaseToolBar@@QEAA@XZ
// CMFCBaseToolBar::CMFCBaseToolBar()
extern "C" void* MS_ABI impl___0CMFCBaseToolBar__QEAA_XZ() {
    return new CMFCBaseToolBar();
}
