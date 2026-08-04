// CCommandLineInfo — OpenMFC implementation.
// Sources: appcore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CWinAppSupport.h"

// Symbol: ??0CCommandLineInfo@@QEAA@XZ
extern "C" void* MS_ABI impl___0CCommandLineInfo__QEAA_XZ(void* pThis) {
    if (pThis) g_commandLineInfoStates[reinterpret_cast<CCommandLineInfo*>(pThis)] = CommandLineInfoState{};
    return pThis;
}
// Symbol: ??1CCommandLineInfo@@UEAA@XZ
extern "C" void MS_ABI impl___1CCommandLineInfo__UEAA_XZ(CCommandLineInfo* pThis) {
    if (pThis) g_commandLineInfoStates.erase(pThis);
}
// Symbol: ?ParseParam@CCommandLineInfo@@UEAAXPEB_WHH@Z
extern "C" void MS_ABI impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(
    CCommandLineInfo* pThis, const wchar_t* pszParam, int bFlag, int bLast) {
    if (!pThis) return;
    CommandLineInfoState& state = g_commandLineInfoStates[pThis];
    if (bFlag) ParseCommandFlag(state, pszParam);
    else ParseNonFlagToken(state, pszParam);
    if (bLast && state.shellCommand == kShellCommandFileNothing && !state.hasFileName) {
        state.shellCommand = kShellCommandFileNew;
    }
}
// Symbol: ?ParseParam@CCommandLineInfo@@UEAAXPEBDHH@Z
extern "C" void MS_ABI impl__ParseParam_CCommandLineInfo__UEAAXPEBDHH_Z(
    CCommandLineInfo* pThis, const char* pszParam, int bFlag, int bLast) {
    if (pszParam == nullptr) {
        impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, nullptr, bFlag, bLast);
        return;
    }
    const int needed = ::MultiByteToWideChar(CP_ACP, 0, pszParam, -1, nullptr, 0);
    if (needed > 0) {
        std::wstring wide(static_cast<size_t>(needed - 1), L'\0');
        ::MultiByteToWideChar(CP_ACP, 0, pszParam, -1, wide.data(), needed);
        impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, wide.c_str(), bFlag, bLast);
        return;
    }
    std::wstring fallback;
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(pszParam); *p; ++p) {
        fallback.push_back(static_cast<wchar_t>(*p));
    }
    impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, fallback.c_str(), bFlag, bLast);
}
// Symbol: ?ParseParamFlag@CCommandLineInfo@@IEAAXPEBD@Z
extern "C" void MS_ABI impl__ParseParamFlag_CCommandLineInfo__IEAAXPEBD_Z(CCommandLineInfo* pThis, const char* pszParam) {
    impl__ParseParam_CCommandLineInfo__UEAAXPEBDHH_Z(pThis, pszParam, TRUE, FALSE);
}
// Symbol: ?ParseParamNotFlag@CCommandLineInfo@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__ParseParamNotFlag_CCommandLineInfo__IEAAXPEB_W_Z(CCommandLineInfo* pThis, const wchar_t* pszParam) {
    impl__ParseParam_CCommandLineInfo__UEAAXPEB_WHH_Z(pThis, pszParam, FALSE, FALSE);
}
// Symbol: ?ParseParamNotFlag@CCommandLineInfo@@IEAAXPEBD@Z
extern "C" void MS_ABI impl__ParseParamNotFlag_CCommandLineInfo__IEAAXPEBD_Z(CCommandLineInfo* pThis, const char* pszParam) {
    impl__ParseParam_CCommandLineInfo__UEAAXPEBDHH_Z(pThis, pszParam, FALSE, FALSE);
}
// Symbol: ?ParseLast@CCommandLineInfo@@IEAAXH@Z
extern "C" void MS_ABI impl__ParseLast_CCommandLineInfo__IEAAXH_Z(CCommandLineInfo* pThis, int bLast) {
    if (!pThis || !bLast) return;
    CommandLineInfoState& state = g_commandLineInfoStates[pThis];
    if (state.shellCommand == kShellCommandFileNothing && !state.hasFileName) {
        state.shellCommand = kShellCommandFileNew;
    }
}
