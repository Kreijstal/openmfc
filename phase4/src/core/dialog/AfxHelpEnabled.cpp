#define OPENMFC_APPCORE_IMPL
#include "detail/WindowMessageSupport.h"
namespace {
CCmdTarget* ModuleApp() {
    auto* module=impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    return module ? *reinterpret_cast<CCmdTarget**>(reinterpret_cast<BYTE*>(module)+8) : nullptr;
}
BOOL HasHelpHandler(CCmdTarget* target) {
    AFX_CMDHANDLERINFO handler{};
    using Route=BOOL (MS_ABI*)(CCmdTarget*,UINT,int,void*,AFX_CMDHANDLERINFO*);
    return openmfc::detail::WindowSlot<Route>(target,5)(target,0xe146,CN_COMMAND,nullptr,&handler);
}
}
// Symbol: ?AfxHelpEnabled@@YAHXZ
extern "C" BOOL MS_ABI impl__AfxHelpEnabled__YAHXZ() {
    if(!ModuleApp())return FALSE;
    if(CWinThread* thread=AfxGetThread()) {
        using Main=CWnd* (MS_ABI*)(CWinThread*);
        CWnd* window=openmfc::detail::WindowSlot<Main>(thread,31)(thread);
        if(window && HasHelpHandler(window))return TRUE;
    }
    return HasHelpHandler(ModuleApp());
}
