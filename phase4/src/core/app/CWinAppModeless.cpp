#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
#include "openmfc/afxole.h"
#include <cstring>

// Symbol: ?DoEnableModeless@CWinApp@@SAXH@Z
extern "C" void MS_ABI impl__DoEnableModeless_CWinApp__SAXH_Z(BOOL enable) {
    auto* thread = AfxGetThread();
    CWnd* main = thread ? thread->GetMainWnd() : nullptr;
    if (!main || !main->IsFrameWnd()) return;

    const auto* frame = static_cast<CFrameWnd*>(main);
    COleFrameHook* hook;
    std::memcpy(&hook, frame->m_retailMiddle.m_pNotifyHook, sizeof(hook));
    if (hook) hook->OnEnableModeless(enable);
}

// Symbol: ?EnableModeless@CWinApp@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableModeless_CWinApp__QEAAXH_Z(CWinApp*, BOOL enable) {
    impl__DoEnableModeless_CWinApp__SAXH_Z(enable);
}
