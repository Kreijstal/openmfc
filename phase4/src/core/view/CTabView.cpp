// CTabView — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"
#include <cstddef>

namespace {
// Byte shadow: CView occupies 0xf8 bytes; retail CTabView places m_wndTabs
// there, then m_bIsReady and m_nFirstActiveTab at the offsets below.
struct CTabViewRetailLayout {
    unsigned char cView[0xf8];
    unsigned char wndTabs[0x3d68];
    BOOL isReady;
    int firstActiveTab;
};
static_assert(sizeof(CView) == 0xf8, "retail CView base size");
static_assert(offsetof(CTabViewRetailLayout, wndTabs) == 0xf8, "CTabView m_wndTabs offset");
static_assert(offsetof(CTabViewRetailLayout, isReady) == 0x3e60, "CTabView m_bIsReady offset");
static_assert(offsetof(CTabViewRetailLayout, firstActiveTab) == 0x3e64, "CTabView m_nFirstActiveTab offset");
static_assert(sizeof(CTabViewRetailLayout) == 0x3e68, "CTabView retail allocation size");
}

// Retained state-only approximation: unlike the retail constructor/destructor (RVA
// 0x13d100/0x13d1a0, mfc140u), these wrappers do not construct/destroy the CView and
// embedded CMFCTabCtrl subobjects. The methods below likewise do not create native view
// windows or synchronize tab-control items. Keep this object-layout-independent
// approximation; it is not a faithful native CTabView lifecycle.
// Symbol: ??0CTabView@@IEAA@XZ
extern "C" void* MS_ABI impl___0CTabView__IEAA_XZ(void* self) {
    g_tabViewStates[self] = TabViewState();
    return self;
}
// Symbol: ??1CTabView@@MEAA@XZ
extern "C" void MS_ABI impl___1CTabView__MEAA_XZ(void* self) {
    g_tabViewStates.erase(self);
}
// Symbol: ?AddView@CTabView@@QEAAHPEAUCRuntimeClass@@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HPEAUCCreateContext@@@Z
extern "C" int MS_ABI impl__AddView_CTabView__QEAAHPEAUCRuntimeClass__AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HPEAUCCreateContext___Z(
    void* self, CRuntimeClass* runtimeClass, const CString* title, int image, CCreateContext*) {
    if (!self) return FALSE;
    CObject* object = runtimeClass ? runtimeClass->CreateObject() : nullptr;
    CView* view = dynamic_cast<CView*>(object);
    if (object && !view) {
        delete object;
        return FALSE;
    }
    TabViewState& state = g_tabViewStates[self];
    TabViewEntry entry;
    entry.view = view;
    entry.title = CStringText(title);
    entry.image = image;
    state.tabs.push_back(entry);
    if (state.active < 0) state.active = 0;
    return TRUE;
}
// Symbol: ?FindTab@CTabView@@QEBAHPEAUHWND__@@@Z
extern "C" int MS_ABI impl__FindTab_CTabView__QEBAHPEAUHWND_____Z(const void* self, HWND hwnd) {
    auto it = g_tabViewStates.find(const_cast<void*>(self));
    if (it == g_tabViewStates.end()) return -1;
    for (size_t i = 0; i < it->second.tabs.size(); ++i) {
        CView* view = it->second.tabs[i].view;
        if (view && view->GetSafeHwnd() == hwnd) return static_cast<int>(i);
    }
    return -1;
}
// Symbol: ?GetActiveView@CTabView@@QEBAPEAVCView@@XZ
extern "C" CView* MS_ABI impl__GetActiveView_CTabView__QEBAPEAVCView__XZ(const void* self) {
    auto it = g_tabViewStates.find(const_cast<void*>(self));
    if (it == g_tabViewStates.end() || it->second.active < 0 || it->second.active >= static_cast<int>(it->second.tabs.size())) return nullptr;
    return it->second.tabs[it->second.active].view;
}
// Symbol: ?SetActiveView@CTabView@@QEAAHH@Z
extern "C" int MS_ABI impl__SetActiveView_CTabView__QEAAHH_Z(void* self, int index) {
    TabViewState& state = g_tabViewStates[self];
    if (index < 0 || index >= static_cast<int>(state.tabs.size())) return FALSE;
    state.active = index;
    return TRUE;
}
// Symbol: ?RemoveView@CTabView@@QEAAHH@Z
extern "C" int MS_ABI impl__RemoveView_CTabView__QEAAHH_Z(void* self, int index) {
    TabViewState& state = g_tabViewStates[self];
    if (index < 0 || index >= static_cast<int>(state.tabs.size())) return FALSE;
    CView* view = state.tabs[index].view;
    state.tabs.erase(state.tabs.begin() + index);
    if (view) delete view;
    if (state.tabs.empty()) state.active = -1;
    else if (state.active >= static_cast<int>(state.tabs.size())) state.active = static_cast<int>(state.tabs.size()) - 1;
    return TRUE;
}
// Retail RVA 0x13d0b0 (mfc140u) allocates 0x3e68 bytes and invokes the CTabView constructor; this file's constructor only seeds g_tabViewStates, so returning that allocation would not produce a usable native CTabView.
// Symbol: ?CreateObject@CTabView@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CTabView__SAPEAVCObject__XZ() {
    return nullptr;
}

// Retail RVA 0x13d4d0 (mfc140u) checks m_bIsReady at +0x3e60; before readiness it stores the low 32 bits of wParam at m_nFirstActiveTab (+0x3e64), and afterward it uses the embedded tab control to switch views. This side-state model does not initialize that control.
// Symbol: ?OnChangeActiveTab@CTabView@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnChangeActiveTab_CTabView__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wp, __int64 lp) {
    (void)pThis;
    (void)wp;
    (void)lp;
    return 0;
}

// Retail RVA 0x13d1e0 (mfc140u) performs window setup using the embedded CMFCTabCtrl; the current CTabView constructor does not initialize that control.
// Symbol: ?OnCreate@CTabView@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CTabView__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* createStruct) {
    (void)pThis;
    (void)createStruct;
    // Fail creation rather than claim success without the embedded tab control setup.
    return -1;
}

// No OnDraw@CTabView entry is present in either available RVA symbol map; drawing behavior
// is not established by a transcribable retail body.
// Symbol: ?OnDraw@CTabView@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CTabView__MEAAXPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// The Unicode RVA map omits this export, but the ANSI twin's identical body calls
// CView::OnInitialUpdate, sets m_bIsReady (+0x3e60), and tail-calls OnChangeActiveTab with
// m_nFirstActiveTab (+0x3e64). This implementation does not initialize the native CView /
// CMFCTabCtrl state that the ready path needs, so it remains stubbed.
// Symbol: ?OnInitialUpdate@CTabView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CTabView__UEAAXXZ(void* pThis) {
    (void)pThis;
}

// The Unicode RVA map omits this export, but the ANSI twin's identical body delegates to
// CView::OnMouseActivate when there is no active tab view and otherwise performs tab-view
// activation/focus work using native view and frame state not represented by this side state.
// Symbol: ?OnMouseActivate@CTabView@@IEAAHPEAVCWnd@@II@Z
extern "C" int MS_ABI impl__OnMouseActivate_CTabView__IEAAHPEAVCWnd__II_Z(void* pThis, CWnd* desktopWnd, unsigned int hitTest, unsigned int message) {
    (void)pThis;
    (void)desktopWnd;
    (void)hitTest;
    (void)message;
    return 0;
}

// The Unicode RVA map omits this export, but the ANSI twin's identical body checks flag 0x4
// and, when set, uses the embedded tab control's vtable byte offset 0x5c0 with a rectangle
// obtained from the CWnd handle at +0x40. That native control is not initialized here.
// Symbol: ?OnPrintClient@CTabView@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CTabView__IEAA_JPEAVCDC__I_Z(void* pThis, CDC* pDC, unsigned int flags) {
    (void)pThis;
    (void)pDC;
    (void)flags;
    return 0;
}

// The Unicode RVA map omits this export, but the ANSI twin's identical body computes
// tab-control bounds from cx+1 and cy+3 and dispatches to the embedded control's positioning
// method. The native CMFCTabCtrl is not initialized by this implementation.
// Symbol: ?OnSize@CTabView@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CTabView__IEAAXIHH_Z(void* pThis, unsigned int type, int cx, int cy) {
    (void)pThis;
    (void)type;
    (void)cx;
    (void)cy;
}

