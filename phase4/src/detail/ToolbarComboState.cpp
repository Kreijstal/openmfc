#include "CbarcoreSupport.h"
namespace openmfc { namespace detail { namespace cbarcore {
thread_local std::unordered_map<const CMFCToolBarComboBoxButton*, ComboButtonState> g_comboStates;
thread_local std::unordered_map<UINT, std::vector<CMFCToolBarComboBoxButton*>> g_comboByCmd;
ComboButtonState& EnsureComboState(const CMFCToolBarComboBoxButton* pButton) {
    return g_comboStates[pButton];
}
const ComboButtonState* FindComboState(const CMFCToolBarComboBoxButton* pButton) {
    auto it = g_comboStates.find(pButton);
    return (it != g_comboStates.end()) ? &it->second : nullptr;
}
void RegisterComboButton(CMFCToolBarComboBoxButton* pButton) {
    if (!pButton || pButton->m_nID == 0) return;
    auto& buttons = g_comboByCmd[pButton->m_nID];
    for (CMFCToolBarComboBoxButton* existing : buttons) {
        if (existing == pButton) return;
    }
    buttons.push_back(pButton);
}
void UnregisterComboButton(CMFCToolBarComboBoxButton* pButton) {
    if (!pButton) return;
    g_comboStates.erase(pButton);
    for (auto it = g_comboByCmd.begin(); it != g_comboByCmd.end(); ) {
        auto& buttons = it->second;
        buttons.erase(std::remove(buttons.begin(), buttons.end(), pButton), buttons.end());
        if (buttons.empty()) {
            it = g_comboByCmd.erase(it);
        } else {
            ++it;
        }
    }
}
std::vector<CMFCToolBarComboBoxButton*> GetComboButtonsByCmd(UINT uiCmd) {
    auto it = g_comboByCmd.find(uiCmd);
    if (it == g_comboByCmd.end()) return {};
    return it->second;
}
int CompareComboText(const wchar_t* s1, const wchar_t* s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    return ::CompareStringW(LOCALE_USER_DEFAULT, NORM_IGNORECASE, s1, -1, s2, -1) - 2;
}
} } }
void OpenMFC_CopyComboButtonState(CMFCToolBarComboBoxButton* dst, const CMFCToolBarComboBoxButton* src) {
    if (!dst || !src) return;
    if (const ComboButtonState* state = FindComboState(src)) {
        g_comboStates[dst] = *state;
    }
    RegisterComboButton(dst);
}
