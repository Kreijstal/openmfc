// CTabView — OpenMFC implementation.
// Sources: collections_strings.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

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
