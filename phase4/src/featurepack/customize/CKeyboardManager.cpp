// CKeyboardManager — OpenMFC implementation.
// Sources: mfccore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/MfccoreSupport.h"

// Symbol: ??0CKeyboardManager@@QEAA@XZ
extern "C" void* MS_ABI impl___0CKeyboardManager__QEAA_XZ(void* pThis) { return new (pThis) CKeyboardManager(); }
// Symbol: ??1CKeyboardManager@@UEAA@XZ
extern "C" void MS_ABI impl___1CKeyboardManager__UEAA_XZ(CKeyboardManager* pThis) { if (pThis) pThis->~CKeyboardManager(); }
// Symbol: ?IsKeyPrintable@CKeyboardManager@@SAHI@Z
extern "C" int MS_ABI impl__IsKeyPrintable_CKeyboardManager__SAHI_Z(unsigned int ch) { return CKeyboardManager::IsKeyPrintable(ch); }
// Symbol: ?TranslateCharToUpper@CKeyboardManager@@SAII@Z
extern "C" unsigned int MS_ABI impl__TranslateCharToUpper_CKeyboardManager__SAII_Z(unsigned int ch) { return CKeyboardManager::TranslateCharToUpper(ch); }
// Symbol: ?ShowAllAccelerators@CKeyboardManager@@SAXH@Z
extern "C" void MS_ABI impl__ShowAllAccelerators_CKeyboardManager__SAXH_Z(int show) { CKeyboardManager::ShowAllAccelerators(show); }
// Symbol: ?CleanUp@CKeyboardManager@@SAXXZ
extern "C" void MS_ABI impl__CleanUp_CKeyboardManager__SAXXZ() { CKeyboardManager::CleanUp(); }
// Symbol: ?ResetAll@CKeyboardManager@@QEAAXXZ
extern "C" void MS_ABI impl__ResetAll_CKeyboardManager__QEAAXXZ(CKeyboardManager* pThis) { if (pThis) pThis->ResetAll(); }
// Symbol: ?LoadState@CKeyboardManager@@QEAAHPEB_WPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__LoadState_CKeyboardManager__QEAAHPEB_WPEAVCFrameWnd___Z(CKeyboardManager* pThis, const wchar_t* profile, CFrameWnd* frame) { return pThis ? pThis->LoadState(profile, frame) : FALSE; }
// Symbol: ?SaveState@CKeyboardManager@@QEAAHPEB_WPEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__SaveState_CKeyboardManager__QEAAHPEB_WPEAVCFrameWnd___Z(CKeyboardManager* pThis, const wchar_t* profile, CFrameWnd* frame) { return pThis ? pThis->SaveState(profile, frame) : FALSE; }
CKeyboardManager::CKeyboardManager() { memset(_keyboardmanager_padding, 0, sizeof(_keyboardmanager_padding)); }
CKeyboardManager::~CKeyboardManager() {}
BOOL CKeyboardManager::IsKeyPrintable(UINT nChar) { return nChar >= 0x20 && nChar < 0x7f; }
UINT CKeyboardManager::TranslateCharToUpper(UINT nChar) { return static_cast<UINT>(std::towupper(static_cast<wint_t>(nChar))); }
void CKeyboardManager::ShowAllAccelerators(BOOL bShowAll) { g_showAllAccelerators = bShowAll; }
void CKeyboardManager::CleanUp() { g_showAllAccelerators = FALSE; }
void CKeyboardManager::ResetAll() { g_showAllAccelerators = FALSE; }
BOOL CKeyboardManager::LoadState(const wchar_t*, CFrameWnd*) { return TRUE; }
BOOL CKeyboardManager::SaveState(const wchar_t*, CFrameWnd*) { return TRUE; }
