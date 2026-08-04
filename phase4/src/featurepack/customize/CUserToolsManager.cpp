// CUserToolsManager — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"


// Implementations this unit calls that are defined with their own class.
extern "C" int MS_ABI impl__Invoke_CUserTool__UEAAHXZ(void* pThis);

// Symbol: ?CreateNewTool@CUserToolsManager@@QEAAPEAVCUserTool@@XZ
extern "C" void* MS_ABI impl__CreateNewTool_CUserToolsManager__QEAAPEAVCUserTool__XZ(void* pThis) {
    auto* tool = new char[sizeof(void*)];
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    g_userTools[tool] = {};
    g_userToolManagers[pThis].tools.push_back(tool);
    return tool;
}
// Symbol: ?FindTool@CUserToolsManager@@QEBAPEAVCUserTool@@I@Z
extern "C" void* MS_ABI impl__FindTool_CUserToolsManager__QEBAPEAVCUserTool__I_Z(const void* pThis, UINT index) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto& tools = g_userToolManagers[const_cast<void*>(pThis)].tools;
    return index < tools.size() ? tools[index] : nullptr;
}
// Symbol: ?InvokeTool@CUserToolsManager@@QEAAHI@Z
extern "C" int MS_ABI impl__InvokeTool_CUserToolsManager__QEAAHI_Z(void* pThis, UINT index) {
    void* tool = impl__FindTool_CUserToolsManager__QEBAPEAVCUserTool__I_Z(pThis, index);
    return tool ? impl__Invoke_CUserTool__UEAAHXZ(tool) : FALSE;
}
// Symbol: ?LoadState@CUserToolsManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__LoadState_CUserToolsManager__QEAAHPEB_W_Z(void* pThis, const wchar_t*) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    g_userToolManagers[pThis];
    return TRUE;
}
// Symbol: ?MoveToolDown@CUserToolsManager@@QEAAHPEAVCUserTool@@@Z
extern "C" int MS_ABI impl__MoveToolDown_CUserToolsManager__QEAAHPEAVCUserTool___Z(void* pThis, void* tool) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto& tools = g_userToolManagers[pThis].tools;
    auto it = std::find(tools.begin(), tools.end(), tool);
    if (it == tools.end() || std::next(it) == tools.end()) return FALSE;
    std::iter_swap(it, std::next(it));
    return TRUE;
}
// Symbol: ?MoveToolUp@CUserToolsManager@@QEAAHPEAVCUserTool@@@Z
extern "C" int MS_ABI impl__MoveToolUp_CUserToolsManager__QEAAHPEAVCUserTool___Z(void* pThis, void* tool) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto& tools = g_userToolManagers[pThis].tools;
    auto it = std::find(tools.begin(), tools.end(), tool);
    if (it == tools.end() || it == tools.begin()) return FALSE;
    std::iter_swap(it, std::prev(it));
    return TRUE;
}
// Symbol: ?RemoveTool@CUserToolsManager@@QEAAHPEAVCUserTool@@@Z
extern "C" int MS_ABI impl__RemoveTool_CUserToolsManager__QEAAHPEAVCUserTool___Z(void* pThis, void* tool) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto& tools = g_userToolManagers[pThis].tools;
    auto it = std::find(tools.begin(), tools.end(), tool);
    if (it == tools.end()) return FALSE;
    tools.erase(it);
    g_userTools.erase(tool);
    delete[] reinterpret_cast<char*>(tool);
    return TRUE;
}
// Symbol: ?SaveState@CUserToolsManager@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SaveState_CUserToolsManager__QEAAHPEB_W_Z(void*, const wchar_t*) {
    return TRUE;
}
// Symbol: ??0CUserToolsManager@@QEAA@IIIPEAUCRuntimeClass@@II@Z
extern "C" void* MS_ABI impl___0CUserToolsManager__QEAA_IIIPEAUCRuntimeClass__II_Z(
    void* pThis, int p0, int p1, int p2, CRuntimeClass* p3,
    int p4, int p5) {
    return new (pThis) CUserToolsManager(p0, p1, p2, p3, p4, p5);
}
// Symbol: ??0CUserToolsManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CUserToolsManager__QEAA_XZ(void* pThis) { return new (pThis) CUserToolsManager(); }
// Symbol: ??1CUserToolsManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CUserToolsManager__UEAA_XZ(CUserToolsManager* pThis) { if (pThis) pThis->~CUserToolsManager(); }
CUserToolsManager::CUserToolsManager() {
    memset(_usertoolsmanager_padding, 0, sizeof(_usertoolsmanager_padding));
}
CUserToolsManager::CUserToolsManager(int, int, int, CRuntimeClass*, int, int) {
    memset(_usertoolsmanager_padding, 0, sizeof(_usertoolsmanager_padding));
}
CUserToolsManager::~CUserToolsManager() {}
