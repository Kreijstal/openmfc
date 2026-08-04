// CWinAppEx — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ??0CWinAppEx@@QEAA@H@Z
extern "C" void* MS_ABI impl___0CWinAppEx__QEAA_H_Z(void* pThis, int smart) { return new (pThis) CWinAppEx(smart); }
// Symbol: ??1CWinAppEx@@UEAA@XZ
extern "C" void MS_ABI impl___1CWinAppEx__UEAA_XZ(CWinAppEx* pThis) { if (pThis) pThis->~CWinAppEx(); }
// Symbol: ?InitContextMenuManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitContextMenuManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->InitContextMenuManager() : FALSE; }
// Symbol: ?InitKeyboardManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitKeyboardManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->InitKeyboardManager() : FALSE; }
// Symbol: ?InitTooltipManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitTooltipManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->InitTooltipManager() : FALSE; }
// Symbol: ?GetContextMenuManager@CWinAppEx@@QEAAPEAVCContextMenuManager@@XZ
extern "C" CContextMenuManager* MS_ABI impl__GetContextMenuManager_CWinAppEx__QEAAPEAVCContextMenuManager__XZ(CWinAppEx* pThis) { return pThis ? pThis->GetContextMenuManager() : nullptr; }
// Symbol: ?GetKeyboardManager@CWinAppEx@@QEAAPEAVCKeyboardManager@@XZ
extern "C" CKeyboardManager* MS_ABI impl__GetKeyboardManager_CWinAppEx__QEAAPEAVCKeyboardManager__XZ(CWinAppEx* pThis) { return pThis ? pThis->GetKeyboardManager() : nullptr; }
// Symbol: ?GetTooltipManager@CWinAppEx@@QEAAPEAVCTooltipManager@@XZ
extern "C" CTooltipManager* MS_ABI impl__GetTooltipManager_CWinAppEx__QEAAPEAVCTooltipManager__XZ(CWinAppEx* pThis) { return pThis ? pThis->GetTooltipManager() : nullptr; }
// Symbol: ?GetDataVersion@CWinAppEx@@QEBAHXZ
extern "C" int MS_ABI impl__GetDataVersion_CWinAppEx__QEBAHXZ(const CWinAppEx* pThis) { return pThis ? pThis->GetDataVersion() : 0; }
// Symbol: ?SetRegistryBase@CWinAppEx@@QEAAPEB_WPEB_W@Z
extern "C" const wchar_t* MS_ABI impl__SetRegistryBase_CWinAppEx__QEAAPEB_WPEB_W_Z(CWinAppEx* pThis, const wchar_t* section) { return pThis ? pThis->SetRegistryBase(section) : nullptr; }
// Symbol: ?GetRegSectionPath@CWinAppEx@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(CString* ret, CWinAppEx* pThis, const wchar_t* add) { new (ret) CString(pThis ? pThis->GetRegSectionPath(add) : CString()); }
// Symbol: ?GetInt@CWinAppEx@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__GetInt_CWinAppEx__QEAAHPEB_WH_Z(CWinAppEx* pThis, const wchar_t* entry, int defVal) { return pThis ? pThis->GetInt(entry, defVal) : defVal; }
// Symbol: ?WriteInt@CWinAppEx@@QEAAHPEB_WH@Z
extern "C" int MS_ABI impl__WriteInt_CWinAppEx__QEAAHPEB_WH_Z(CWinAppEx* pThis, const wchar_t* entry, int value) { return pThis ? pThis->WriteInt(entry, value) : FALSE; }
// Symbol: ?GetString@CWinAppEx@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0@Z
extern "C" void MS_ABI impl__GetString_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0_Z(CString* ret, CWinAppEx* pThis, const wchar_t* entry, const wchar_t* defVal) { new (ret) CString(pThis ? pThis->GetString(entry, defVal) : CString(defVal ? defVal : L"")); }
// Symbol: ?GetSectionString@CWinAppEx@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W00@Z
extern "C" void MS_ABI impl__GetSectionString_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W00_Z(
    CString* ret, CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* defVal) {
    if (!pThis) {
        new (ret) CString(defVal ? defVal : L"");
        return;
    }
    auto& state = g_winAppExStates[pThis];
    auto it = state.strings.find(AppEntryKey(section, entry));
    new (ret) CString(it == state.strings.end() ? CString(defVal ? defVal : L"") : it->second);
}
// Symbol: ?WriteString@CWinAppEx@@QEAAHPEB_W0@Z
extern "C" int MS_ABI impl__WriteString_CWinAppEx__QEAAHPEB_W0_Z(CWinAppEx* pThis, const wchar_t* entry, const wchar_t* value) { return pThis ? pThis->WriteString(entry, value) : FALSE; }
// Symbol: ?LoadState@CWinAppEx@@UEAAHPEB_WPEAVCFrameImpl@@@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__UEAAHPEB_WPEAVCFrameImpl___Z(CWinAppEx* pThis, const wchar_t* section, void* frame) { return pThis ? pThis->LoadState(section, frame) : FALSE; }
// Symbol: ?SaveState@CWinAppEx@@UEAAHPEB_WPEAVCFrameImpl@@@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__UEAAHPEB_WPEAVCFrameImpl___Z(CWinAppEx* pThis, const wchar_t* section, void* frame) { return pThis ? pThis->SaveState(section, frame) : FALSE; }
// Symbol: ?CleanState@CWinAppEx@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__CleanState_CWinAppEx__UEAAHPEB_W_Z(CWinAppEx* pThis, const wchar_t* section) { return pThis ? pThis->CleanState(section) : FALSE; }
// Symbol: ?IsStateExists@CWinAppEx@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__IsStateExists_CWinAppEx__QEAAHPEB_W_Z(CWinAppEx* pThis, const wchar_t* section) { return pThis ? pThis->IsStateExists(section) : FALSE; }
// Symbol: ?ExitInstance@CWinAppEx@@UEAAHXZ
extern "C" int MS_ABI impl__ExitInstance_CWinAppEx__UEAAHXZ(CWinAppEx* pThis) { return pThis ? pThis->ExitInstance() : 0; }
// Symbol: ?GetRuntimeClass@CWinAppEx@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CWinAppEx__UEBAPEAUCRuntimeClass__XZ(const CWinAppEx*) {
    return &CWinAppEx::classCWinAppEx;
}
// Symbol: ?GetThisClass@CWinAppEx@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ() {
    return &CWinAppEx::classCWinAppEx;
}
// Symbol: ?GetBinary@CWinAppEx@@QEAAHPEB_WPEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetBinary_CWinAppEx__QEAAHPEB_WPEAPEAEPEAI_Z(
    CWinAppEx* pThis, const wchar_t* entry, unsigned char** data, unsigned int* bytes) {
    if (data) *data = nullptr;
    if (bytes) *bytes = 0;
    if (!pThis || !entry || !data || !bytes) return FALSE;
    auto& binaries = g_winAppExStates[pThis].binaries;
    const std::wstring section = CurrentAppExSection(pThis);
    auto it = binaries.find(AppEntryKey(section.c_str(), entry));
    if (it == binaries.end()) return FALSE;
    const std::vector<unsigned char>& blob = it->second;
    unsigned char* copy = new (std::nothrow) unsigned char[blob.size()];
    if (!copy && !blob.empty()) return FALSE;
    if (!blob.empty()) std::memcpy(copy, blob.data(), blob.size());
    *data = copy;
    *bytes = static_cast<unsigned int>(blob.size());
    return TRUE;
}
// Symbol: ?WriteBinary@CWinAppEx@@QEAAHPEB_WPEAEI@Z
extern "C" int MS_ABI impl__WriteBinary_CWinAppEx__QEAAHPEB_WPEAEI_Z(
    CWinAppEx* pThis, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    const std::wstring section = CurrentAppExSection(pThis);
    g_winAppExStates[pThis].binaries[AppEntryKey(section.c_str(), entry)] = std::move(blob);
    return TRUE;
}
// Symbol: ?GetObjectW@CWinAppEx@@QEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__GetObjectW_CWinAppEx__QEAAHPEB_WAEAVCObject___Z(CWinAppEx* pThis, const wchar_t* entry, CObject*) {
    if (!pThis || !entry) return FALSE;
    const std::wstring section = CurrentAppExSection(pThis);
    return g_winAppExStates[pThis].objects.count(AppEntryKey(section.c_str(), entry)) ? TRUE : FALSE;
}
// Symbol: ?WriteObject@CWinAppEx@@QEAAHPEB_WAEAVCObject@@@Z
extern "C" int MS_ABI impl__WriteObject_CWinAppEx__QEAAHPEB_WAEAVCObject___Z(CWinAppEx* pThis, const wchar_t* entry, CObject* object) {
    if (!pThis || !entry || !object) return FALSE;
    const std::wstring section = CurrentAppExSection(pThis);
    g_winAppExStates[pThis].objects.insert(AppEntryKey(section.c_str(), entry));
    return TRUE;
}
// Symbol: ?GetSectionBinary@CWinAppEx@@QEAAHPEB_W0PEAPEAEPEAI@Z
extern "C" int MS_ABI impl__GetSectionBinary_CWinAppEx__QEAAHPEB_W0PEAPEAEPEAI_Z(
    CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, unsigned char** data, unsigned int* bytes) {
    if (data) *data = nullptr;
    if (bytes) *bytes = 0;
    if (!pThis || !section || !entry || !data || !bytes) return FALSE;
    auto& binaries = g_winAppExStates[pThis].binaries;
    auto it = binaries.find(AppEntryKey(section, entry));
    if (it == binaries.end()) return FALSE;
    const std::vector<unsigned char>& blob = it->second;
    unsigned char* copy = new (std::nothrow) unsigned char[blob.size()];
    if (!copy && !blob.empty()) return FALSE;
    if (!blob.empty()) std::memcpy(copy, blob.data(), blob.size());
    *data = copy;
    *bytes = static_cast<unsigned int>(blob.size());
    return TRUE;
}
// Symbol: ?WriteSectionBinary@CWinAppEx@@QEAAHPEB_W0PEAEI@Z
extern "C" int MS_ABI impl__WriteSectionBinary_CWinAppEx__QEAAHPEB_W0PEAEI_Z(
    CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, unsigned char* data, unsigned int bytes) {
    if (!pThis || !section || !entry) return FALSE;
    std::vector<unsigned char> blob;
    if (data && bytes != 0) blob.assign(data, data + bytes);
    g_winAppExStates[pThis].binaries[AppEntryKey(section, entry)] = std::move(blob);
    return TRUE;
}
// Symbol: ?GetSectionInt@CWinAppEx@@QEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__GetSectionInt_CWinAppEx__QEAAHPEB_W0H_Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, int defVal) {
    if (!pThis || !section || !entry) return defVal;
    auto& ints = g_winAppExStates[pThis].ints;
    auto it = ints.find(AppEntryKey(section, entry));
    return it == ints.end() ? defVal : it->second;
}
// Symbol: ?WriteSectionInt@CWinAppEx@@QEAAHPEB_W0H@Z
extern "C" int MS_ABI impl__WriteSectionInt_CWinAppEx__QEAAHPEB_W0H_Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, int value) {
    if (!pThis || !section || !entry) return FALSE;
    g_winAppExStates[pThis].ints[AppEntryKey(section, entry)] = value;
    return TRUE;
}
// Symbol: ?WriteSectionString@CWinAppEx@@QEAAHPEB_W00@Z
extern "C" int MS_ABI impl__WriteSectionString_CWinAppEx__QEAAHPEB_W00_Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, const wchar_t* value) {
    if (!pThis || !section || !entry) return FALSE;
    g_winAppExStates[pThis].strings[AppEntryKey(section, entry)] = CString(value ? value : L"");
    return TRUE;
}
// Symbol: ?GetSectionObject@CWinAppEx@@QEAAHPEB_W0AEAVCObject@@@Z
extern "C" int MS_ABI impl__GetSectionObject_CWinAppEx__QEAAHPEB_W0AEAVCObject___Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, CObject*) {
    if (!pThis || !section || !entry) return FALSE;
    return g_winAppExStates[pThis].objects.count(AppEntryKey(section, entry)) ? TRUE : FALSE;
}
// Symbol: ?WriteSectionObject@CWinAppEx@@QEAAHPEB_W0AEAVCObject@@@Z
extern "C" int MS_ABI impl__WriteSectionObject_CWinAppEx__QEAAHPEB_W0AEAVCObject___Z(CWinAppEx* pThis, const wchar_t* section, const wchar_t* entry, CObject* object) {
    if (!pThis || !section || !entry || !object) return FALSE;
    g_winAppExStates[pThis].objects.insert(AppEntryKey(section, entry));
    return TRUE;
}
// Symbol: ?EnableTearOffMenus@CWinAppEx@@QEAAHPEB_WII@Z
extern "C" int MS_ABI impl__EnableTearOffMenus_CWinAppEx__QEAAHPEB_WII_Z(CWinAppEx* pThis, const wchar_t*, unsigned int, unsigned int) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].tearOffMenus = true;
    return TRUE;
}
// Symbol: ?EnableUserTools@CWinAppEx@@QEAAHIIIPEAUCRuntimeClass@@II@Z
extern "C" int MS_ABI impl__EnableUserTools_CWinAppEx__QEAAHIIIPEAUCRuntimeClass__II_Z(
    CWinAppEx* pThis, unsigned int, unsigned int, unsigned int, CRuntimeClass*, unsigned int, unsigned int) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].userTools = true;
    return TRUE;
}
// Symbol: ?InitMouseManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitMouseManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].mouseManager = true;
    return TRUE;
}
// Symbol: ?GetMouseManager@CWinAppEx@@QEAAPEAVCMouseManager@@XZ
extern "C" CMouseManager* MS_ABI impl__GetMouseManager_CWinAppEx__QEAAPEAVCMouseManager__XZ(CWinAppEx* pThis) {
    if (!pThis) return nullptr;
    impl__InitMouseManager_CWinAppEx__QEAAHXZ(pThis);
    return reinterpret_cast<CMouseManager*>(&g_mouseManagerToken);
}
// Symbol: ?InitShellManager@CWinAppEx@@QEAAHXZ
extern "C" int MS_ABI impl__InitShellManager_CWinAppEx__QEAAHXZ(CWinAppEx* pThis) {
    if (!pThis) return FALSE;
    g_winAppExStates[pThis].shellManager = true;
    return TRUE;
}
// Symbol: ?GetShellManager@CWinAppEx@@QEAAPEAVCShellManager@@XZ
extern "C" CShellManager* MS_ABI impl__GetShellManager_CWinAppEx__QEAAPEAVCShellManager__XZ(CWinAppEx* pThis) {
    if (!pThis) return nullptr;
    impl__InitShellManager_CWinAppEx__QEAAHXZ(pThis);
    return reinterpret_cast<CShellManager*>(&g_shellManagerToken);
}
// Symbol: ?GetUserToolsManager@CWinAppEx@@QEAAPEAVCUserToolsManager@@XZ
extern "C" CUserToolsManager* MS_ABI impl__GetUserToolsManager_CWinAppEx__QEAAPEAVCUserToolsManager__XZ(CWinAppEx* pThis) {
    if (!pThis) return nullptr;
    g_winAppExStates[pThis].userTools = true;
    return afxUserToolsManager;
}
// Symbol: ?LoadWindowPlacement@CWinAppEx@@MEAAHAEAVCRect@@AEAH1@Z
extern "C" int MS_ABI impl__LoadWindowPlacement_CWinAppEx__MEAAHAEAVCRect__AEAH1_Z(CWinAppEx* pThis, CRect* rect, int* showCmd, int* flags) {
    if (!pThis || !rect || !showCmd || !flags) return FALSE;
    WinAppExState& state = g_winAppExStates[pThis];
    *rect = state.windowRect;
    *showCmd = state.windowShowCmd;
    *flags = state.windowFlags;
    return TRUE;
}
// Symbol: ?StoreWindowPlacement@CWinAppEx@@MEAAHAEBVCRect@@HH@Z
extern "C" int MS_ABI impl__StoreWindowPlacement_CWinAppEx__MEAAHAEBVCRect__HH_Z(CWinAppEx* pThis, const CRect* rect, int showCmd, int flags) {
    if (!pThis || !rect) return FALSE;
    WinAppExState& state = g_winAppExStates[pThis];
    state.windowRect = *rect;
    state.windowShowCmd = showCmd;
    state.windowFlags = flags;
    return TRUE;
}
// Symbol: ?LoadState@CWinAppEx@@QEAAHPEAVCFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__QEAAHPEAVCFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->LoadState(section, nullptr) : FALSE;
}
// Symbol: ?SaveState@CWinAppEx@@QEAAHPEAVCFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__QEAAHPEAVCFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->SaveState(section, nullptr) : FALSE;
}
// Symbol: ?LoadState@CWinAppEx@@QEAAHPEAVCMDIFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__QEAAHPEAVCMDIFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CMDIFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->LoadState(section, nullptr) : FALSE;
}
// Symbol: ?SaveState@CWinAppEx@@QEAAHPEAVCMDIFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__QEAAHPEAVCMDIFrameWndEx__PEB_W_Z(CWinAppEx* pThis, CMDIFrameWndEx*, const wchar_t* section) {
    return pThis ? pThis->SaveState(section, nullptr) : FALSE;
}
// Symbol: ?LoadState@CWinAppEx@@QEAAHPEAVCOleIPFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__LoadState_CWinAppEx__QEAAHPEAVCOleIPFrameWndEx__PEB_W_Z(CWinAppEx* pThis, void* frame, const wchar_t* section) {
    return pThis ? pThis->LoadState(section, frame) : FALSE;
}
// Symbol: ?SaveState@CWinAppEx@@QEAAHPEAVCOleIPFrameWndEx@@PEB_W@Z
extern "C" int MS_ABI impl__SaveState_CWinAppEx__QEAAHPEAVCOleIPFrameWndEx__PEB_W_Z(CWinAppEx* pThis, void* frame, const wchar_t* section) {
    return pThis ? pThis->SaveState(section, frame) : FALSE;
}
// Symbol: ?ReloadWindowPlacement@CWinAppEx@@MEAAHPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__ReloadWindowPlacement_CWinAppEx__MEAAHPEAVCFrameWnd___Z(CWinAppEx* pThis, CFrameWnd* frame) {
    if (!pThis || !frame) return FALSE;
    WinAppExState& state = g_winAppExStates[pThis];
    HWND hwnd = frame->GetSafeHwnd();
    if (!hwnd) return TRUE;
    if (!state.windowRect.IsRectNull()) {
        ::MoveWindow(hwnd, state.windowRect.left, state.windowRect.top, state.windowRect.Width(), state.windowRect.Height(), TRUE);
    }
    ::ShowWindow(hwnd, state.windowShowCmd);
    return TRUE;
}
// Symbol: ?OnAppContextHelp@CWinAppEx@@UEAAXPEAVCWnd@@QEBK@Z
extern "C" void MS_ABI impl__OnAppContextHelp_CWinAppEx__UEAAXPEAVCWnd__QEBK_Z(CWinAppEx*, CWnd*, const unsigned long*) {
}
// Symbol: ?OnClosingMainFrame@CWinAppEx@@MEAAXPEAVCFrameImpl@@@Z
extern "C" void MS_ABI impl__OnClosingMainFrame_CWinAppEx__MEAAXPEAVCFrameImpl___Z(CWinAppEx*, void*) {
}
// Symbol: ?OnViewDoubleClick@CWinAppEx@@UEAAHPEAVCWnd@@H@Z
extern "C" int MS_ABI impl__OnViewDoubleClick_CWinAppEx__UEAAHPEAVCWnd__H_Z(CWinAppEx*, CWnd*, int) {
    return FALSE;
}
// Symbol: ?ShowPopupMenu@CWinAppEx@@UEAAHIAEBVCPoint@@PEAVCWnd@@@Z
extern "C" int MS_ABI impl__ShowPopupMenu_CWinAppEx__UEAAHIAEBVCPoint__PEAVCWnd___Z(CWinAppEx*, unsigned int menuId, const CPoint* point, CWnd* owner) {
    HMENU menu = ::LoadMenuW(AfxGetResourceHandle(), MAKEINTRESOURCEW(menuId));
    if (!menu) return FALSE;
    HMENU popup = ::GetSubMenu(menu, 0);
    BOOL shown = FALSE;
    if (popup && point) {
        shown = ::TrackPopupMenu(popup, TPM_LEFTALIGN | TPM_TOPALIGN, point->x, point->y, 0, owner ? owner->GetSafeHwnd() : nullptr, nullptr);
    }
    ::DestroyMenu(menu);
    return shown ? TRUE : FALSE;
}
CWinAppEx::CWinAppEx(BOOL) : CWinApp(nullptr), m_pContextMenuManager(nullptr), m_pKeyboardManager(nullptr), m_pTooltipManager(nullptr), m_nDataVersion(1) { memset(_winappex_padding, 0, sizeof(_winappex_padding)); }
CWinAppEx::~CWinAppEx() { delete m_pContextMenuManager; delete m_pKeyboardManager; delete m_pTooltipManager; g_winAppExStates.erase(this); }
BOOL CWinAppEx::InitContextMenuManager() { if (!m_pContextMenuManager) m_pContextMenuManager = new CContextMenuManager(); return TRUE; }
BOOL CWinAppEx::InitKeyboardManager() { if (!m_pKeyboardManager) m_pKeyboardManager = new CKeyboardManager(); return TRUE; }
BOOL CWinAppEx::InitTooltipManager() { if (!m_pTooltipManager) m_pTooltipManager = new CTooltipManager(); return TRUE; }
CContextMenuManager* CWinAppEx::GetContextMenuManager() { InitContextMenuManager(); return m_pContextMenuManager; }
CKeyboardManager* CWinAppEx::GetKeyboardManager() { InitKeyboardManager(); return m_pKeyboardManager; }
CTooltipManager* CWinAppEx::GetTooltipManager() { InitTooltipManager(); return m_pTooltipManager; }
int CWinAppEx::GetDataVersion() const { return m_nDataVersion; }
const wchar_t* CWinAppEx::SetRegistryBase(const wchar_t* lpszSectionName) { m_strRegSection = lpszSectionName ? lpszSectionName : L""; return static_cast<const wchar_t*>(m_strRegSection); }
CString CWinAppEx::GetRegSectionPath(const wchar_t* lpszSectionAdd) { CString path = m_strRegSection; if (lpszSectionAdd && *lpszSectionAdd) { if (static_cast<const wchar_t*>(path)[0] != 0) path += L"\\"; path += lpszSectionAdd; } return path; }
int CWinAppEx::GetInt(const wchar_t* lpszEntry, int nDefault) { auto& state = g_winAppExStates[this]; auto it = state.ints.find(AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)); return it == state.ints.end() ? nDefault : it->second; }
BOOL CWinAppEx::WriteInt(const wchar_t* lpszEntry, int nValue) { g_winAppExStates[this].ints[AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)] = nValue; return TRUE; }
CString CWinAppEx::GetString(const wchar_t* lpszEntry, const wchar_t* lpszDefault) { auto& state = g_winAppExStates[this]; auto it = state.strings.find(AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)); return it == state.strings.end() ? CString(lpszDefault ? lpszDefault : L"") : it->second; }
BOOL CWinAppEx::WriteString(const wchar_t* lpszEntry, const wchar_t* lpszValue) { g_winAppExStates[this].strings[AppEntryKey(static_cast<const wchar_t*>(m_strRegSection), lpszEntry)] = CString(lpszValue ? lpszValue : L""); return TRUE; }
BOOL CWinAppEx::LoadState(const wchar_t* lpszSectionName, void*) { g_winAppExStates[this].states.insert(WideKey(lpszSectionName)); return TRUE; }
BOOL CWinAppEx::SaveState(const wchar_t* lpszSectionName, void*) { g_winAppExStates[this].states.insert(WideKey(lpszSectionName)); return TRUE; }
BOOL CWinAppEx::CleanState(const wchar_t* lpszSectionName) { g_winAppExStates[this].states.erase(WideKey(lpszSectionName)); return TRUE; }
BOOL CWinAppEx::IsStateExists(const wchar_t* lpszSectionName) { return g_winAppExStates[this].states.count(WideKey(lpszSectionName)) != 0; }
int CWinAppEx::ExitInstance() { delete m_pContextMenuManager; m_pContextMenuManager = nullptr; delete m_pKeyboardManager; m_pKeyboardManager = nullptr; delete m_pTooltipManager; m_pTooltipManager = nullptr; return CWinApp::ExitInstance(); }
