// CMFCColorPopupMenu — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

// Symbol: ??1CMFCColorPopupMenu@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCColorPopupMenu__UEAA_XZ(void* pThis) {
    if (pThis != nullptr) {
        ClearCMFCColorPopupMenuState(pThis);
    }
}
// Symbol: ?CreateTearOffBar@CMFCColorPopupMenu@@UEAAPEAVCPane@@PEAVCFrameWnd@@IPEB_W@Z
extern "C" void* MS_ABI impl__CreateTearOffBar_CMFCColorPopupMenu__UEAAPEAVCPane__PEAVCFrameWnd__IPEB_W_Z(
    void* pThis, void* pFrameWnd, unsigned int commandId, const wchar_t* text) {
    if (pThis == nullptr || pFrameWnd == nullptr) {
        return nullptr;
    }
    auto& state = EnsureCMFCColorPopupMenuState(pThis);
    state.m_commandId = commandId;
    state.m_text = const_cast<wchar_t*>(text);
    if (state.m_tearOffBar == nullptr) {
        state.m_tearOffBar = new (std::nothrow) CPane();
    }
    state.m_created = true;
    return state.m_tearOffBar;
}
// Symbol: ?OnCreate@CMFCColorPopupMenu@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCColorPopupMenu__IEAAHPEAUtagCREATESTRUCTW___Z(
    void* pThis, void* createStruct) {
    if (pThis == nullptr || createStruct == nullptr) {
        return 0;
    }
    auto& state = EnsureCMFCColorPopupMenuState(pThis);
    state.m_created = true;
    return 1;
}
