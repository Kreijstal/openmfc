// CUserTool — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

// Symbol: ?CopyIconToClipboard@CUserTool@@QEAAHXZ
extern "C" int MS_ABI impl__CopyIconToClipboard_CUserTool__QEAAHXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto it = g_userTools.find(pThis);
    if (it == g_userTools.end() || !it->second.icon || !::OpenClipboard(nullptr)) return FALSE;
    ::EmptyClipboard();
    HICON copy = (HICON)::CopyImage(it->second.icon, IMAGE_ICON, 0, 0, LR_COPYFROMRESOURCE);
    if (!copy) {
        ::CloseClipboard();
        return FALSE;
    }
    BOOL ok = ::SetClipboardData(CF_ENHMETAFILE, copy) != nullptr;
    ::CloseClipboard();
    return ok;
}
// Symbol: ?CreateObject@CUserTool@@SAPEAVCObject@@XZ
extern "C" CObject* MS_ABI impl__CreateObject_CUserTool__SAPEAVCObject__XZ() {
    return reinterpret_cast<CObject*>(new char[sizeof(void*)]);
}
// Symbol: ?DeleteIcon@CUserTool@@MEAAXXZ
extern "C" void MS_ABI impl__DeleteIcon_CUserTool__MEAAXXZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto& state = g_userTools[pThis];
    if (state.icon) {
        ::DestroyIcon(state.icon);
        state.icon = nullptr;
    }
}
// GetRuntimeClass/GetThisClass for CUserTool live in featurepack/customize/RuntimeClasses.cpp
// (classCUserTool from detail/COleControlModuleSupport.h: size 56, schema 0x80000001,
// base CObject). That matches the retail descriptor at 0x1803b17d8 in mfc140u.dll
// (0x1803aa7d8 in mfc140.dll): "CUserTool", 56, 0x80000001, base CObject. The copies
// that used to be here returned g_classCUserTool, sized sizeof(void*) = 8 with schema
// 0xFFFF; 0x80000001 is VERSIONABLE_SCHEMA|1, which only IMPLEMENT_SERIAL emits, so the
// 0xFFFF value would have mis-versioned CUserTool serialization.
// Symbol: ?Invoke@CUserTool@@UEAAHXZ
extern "C" int MS_ABI impl__Invoke_CUserTool__UEAAHXZ(void* pThis) {
    std::wstring command;
    {
        std::lock_guard<std::mutex> lock(g_userToolMutex);
        command = g_userTools[pThis].command;
    }
    if (command.empty()) return FALSE;
    HINSTANCE result = ::ShellExecuteW(nullptr, L"open", command.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    return (UINT_PTR)result > 32;
}
// Symbol: ?LoadDefaultIcon@CUserTool@@MEAAPEAUHICON__@@XZ
extern "C" HICON MS_ABI impl__LoadDefaultIcon_CUserTool__MEAAPEAUHICON____XZ(void* pThis) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    auto& state = g_userTools[pThis];
    if (!state.icon) {
        state.icon = ::LoadIconW(nullptr, MAKEINTRESOURCEW(IDI_APPLICATION));
    }
    return state.icon;
}
// Symbol: ?Serialize@CUserTool@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CUserTool__UEAAXAEAVCArchive___Z(void* pThis, CArchive* ar) {
    if (!ar) return;
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    CString command(g_userTools[pThis].command.c_str());
    if (ar->IsStoring()) {
        *ar << command;
    } else {
        *ar >> command;
        g_userTools[pThis].command = (const wchar_t*)command;
    }
}
// Symbol: ?SetCommand@CUserTool@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetCommand_CUserTool__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszCmd) {
    std::lock_guard<std::mutex> lock(g_userToolMutex);
    g_userTools[pThis].command = lpszCmd ? lpszCmd : L"";
}
// Symbol: ?SetToolIcon@CUserTool@@UEAAPEAUHICON__@@XZ
extern "C" HICON MS_ABI impl__SetToolIcon_CUserTool__UEAAPEAUHICON____XZ(void* pThis) {
    return impl__LoadDefaultIcon_CUserTool__MEAAPEAUHICON____XZ(pThis);
}
// Symbol: ?DrawToolIcon@CUserTool@@QEAAXPEAVCDC@@AEBVCRect@@@Z
extern "C" void MS_ABI impl__DrawToolIcon_CUserTool__QEAAXPEAVCDC__AEBVCRect___Z(
    CUserTool* pThis, CDC* pDC, const CRect* pRect) {
    if (pThis && pRect) pThis->DrawToolIcon(pDC, *pRect);
}
// Symbol: ??0CUserTool@@QEAA@XZ
extern "C" void* MS_ABI impl___0CUserTool__QEAA_XZ(void* pThis) { return new (pThis) CUserTool(); }
// Symbol: ??1CUserTool@@UEAA@XZ
extern "C" void MS_ABI impl___1CUserTool__UEAA_XZ(CUserTool* pThis) { if (pThis) pThis->~CUserTool(); }
CUserTool::CUserTool() : m_uiCmdId(0), m_hIcon(nullptr) {}
CUserTool::~CUserTool() {}
int CUserTool::Invoke() {
    if (m_strCommand.IsEmpty()) return 0;

    HINSTANCE result = ::ShellExecuteW(
        nullptr,
        L"open",
        m_strCommand.GetString(),
        m_strArguments.IsEmpty() ? nullptr : m_strArguments.GetString(),
        m_strInitialDirectory.IsEmpty() ? nullptr : m_strInitialDirectory.GetString(),
        SW_SHOWNORMAL);

    return (UINT_PTR)result > 32;
}
void CUserTool::Serialize(CArchive& ar) {
    if (ar.IsStoring()) {
        ar << m_strLabel << m_strArguments << m_strInitialDirectory;
        ar << m_uiCmdId << m_strCommand;
        ar << static_cast<BOOL>(m_hIcon != nullptr);
    } else {
        ar >> m_strLabel;
        ar >> m_strArguments;
        ar >> m_strInitialDirectory;
        ar >> m_uiCmdId;
        ar >> m_strCommand;
        BOOL hasIcon = FALSE;
        ar >> hasIcon;
        if (!hasIcon && m_hIcon) {
            DestroyIcon(m_hIcon);
            m_hIcon = nullptr;
        }
    }
}
void CUserTool::DrawToolIcon(CDC* pDC, const CRect& rectImage) {
    if (pDC == nullptr || m_hIcon == nullptr) return;

    const int cx = ::GetSystemMetrics(SM_CXSMICON);
    const int cy = ::GetSystemMetrics(SM_CYSMICON);

    int dx = (rectImage.right - rectImage.left - cx) / 2;
    int dy = (rectImage.bottom - rectImage.top - cy) / 2;
    if (dx < 0) dx = 0;
    if (dy < 0) dy = 0;

    ::DrawIconEx(pDC->m_hDC, rectImage.left + dx, rectImage.top + dy,
                 m_hIcon, 0, 0, 0, nullptr, DI_NORMAL);
}
