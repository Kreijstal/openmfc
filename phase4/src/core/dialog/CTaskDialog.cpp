// CTaskDialog — OpenMFC implementation.
// Sources: cbarcore.cpp, dlgcommon.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/DlgcommonSupport.h"

// Symbol: ?ClickCommandControl@CTaskDialog@@IEBAXH@Z
extern "C" void MS_ABI impl__ClickCommandControl_CTaskDialog__IEBAXH_Z(CTaskDialog* pThis, int id) {
    if (pThis) pThis->m_nSelectedCommandID = id;
}
// Symbol: ?ClickRadioButton@CTaskDialog@@IEBAXH@Z
extern "C" void MS_ABI impl__ClickRadioButton_CTaskDialog__IEBAXH_Z(CTaskDialog* pThis, int id) {
    if (pThis) pThis->m_nSelectedRadioButtonID = id;
}
// Symbol: ?FillStruct@CTaskDialog@@AEAAXAEAU_TASKDIALOGCONFIG@@@Z
extern "C" void MS_ABI impl__FillStruct_CTaskDialog__AEAAXAEAU_TASKDIALOGCONFIG___Z(CTaskDialog* pThis, TASKDIALOGCONFIG* config) {
    if (config) FillTaskDialogConfig(pThis, *config);
}
// Symbol: ?FreeStruct@CTaskDialog@@AEAAXAEAU_TASKDIALOGCONFIG@@@Z
extern "C" void MS_ABI impl__FreeStruct_CTaskDialog__AEAAXAEAU_TASKDIALOGCONFIG___Z(
    CTaskDialog* pThis, TASKDIALOGCONFIG* config) {
    (void)pThis;
    if (!config) return;
    std::memset(config, 0, sizeof(*config));
}
// Symbol: ?GetButtonData@CTaskDialog@@AEBAPEAU_TASKDIALOG_BUTTON@@AEBV?$CArray@U_CTaskDialogButton@CTaskDialog@@AEBU12@@@@Z
extern "C" TASKDIALOG_BUTTON* MS_ABI impl__GetButtonData_CTaskDialog__AEBAPEAU_TASKDIALOG_BUTTON__AEBV__CArray_U_CTaskDialogButton_CTaskDialog__AEBU12____Z(
    CTaskDialog* pThis, const void*) {
    if (!pThis) return nullptr;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    return FirstTaskDialogButton(g_taskDialogs[pThis].commandControls);
}
// Symbol: ?GetButtonIndex@CTaskDialog@@AEBA_JHAEBV?$CArray@U_CTaskDialogButton@CTaskDialog@@AEBU12@@@@Z
extern "C" intptr_t MS_ABI impl__GetButtonIndex_CTaskDialog__AEBA_JHAEBV__CArray_U_CTaskDialogButton_CTaskDialog__AEBU12____Z(
    CTaskDialog* pThis, int id, const void*) {
    if (!pThis) return -1;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    ptrdiff_t index = FindTaskDialogButtonIndex(g_taskDialogs[pThis].commandControls, id);
    if (index >= 0) return index;
    return FindTaskDialogButtonIndex(g_taskDialogs[pThis].radioButtons, id);
}
// Symbol: ?GetCommonButtonCount@CTaskDialog@@MEBAHXZ
extern "C" int MS_ABI impl__GetCommonButtonCount_CTaskDialog__MEBAHXZ(const CTaskDialog* pThis) {
    if (!pThis) return 0;
    int count = 0;
    for (int flag : { TDCBF_OK_BUTTON, TDCBF_YES_BUTTON, TDCBF_NO_BUTTON,
                      TDCBF_CANCEL_BUTTON, TDCBF_RETRY_BUTTON, TDCBF_CLOSE_BUTTON }) {
        if (pThis->m_nCommonButtons & flag) ++count;
    }
    return count;
}
// Symbol: ?GetCommonButtonFlag@CTaskDialog@@MEBAHH@Z
extern "C" int MS_ABI impl__GetCommonButtonFlag_CTaskDialog__MEBAHH_Z(const CTaskDialog* pThis, int index) {
    return pThis ? CommonButtonFlagByIndex(pThis->m_nCommonButtons, index) : 0;
}
// Symbol: ?GetCommonButtonId@CTaskDialog@@MEBAHH@Z
extern "C" int MS_ABI impl__GetCommonButtonId_CTaskDialog__MEBAHH_Z(const CTaskDialog* pThis, int index) {
    return CommonButtonId(impl__GetCommonButtonFlag_CTaskDialog__MEBAHH_Z(pThis, index));
}
// Symbol: ?GetRuntimeClass@CTaskDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CTaskDialog__UEBAPEAUCRuntimeClass__XZ(const CTaskDialog*) {
    return &g_classCTaskDialog;
}
// Symbol: ?GetThisClass@CTaskDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CTaskDialog__SAPEAUCRuntimeClass__XZ() {
    return &g_classCTaskDialog;
}
// Symbol: ?IsCommandControlEnabled@CTaskDialog@@QEBAHH@Z
extern "C" int MS_ABI impl__IsCommandControlEnabled_CTaskDialog__QEBAHH_Z(const CTaskDialog* pThis, int id) {
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    return g_taskDialogs[const_cast<CTaskDialog*>(pThis)].commandControls.disabled.count(id) == 0;
}
// Symbol: ?IsRadioButtonEnabled@CTaskDialog@@QEBAHH@Z
extern "C" int MS_ABI impl__IsRadioButtonEnabled_CTaskDialog__QEBAHH_Z(const CTaskDialog* pThis, int id) {
    if (!pThis) return FALSE;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    return g_taskDialogs[const_cast<CTaskDialog*>(pThis)].radioButtons.disabled.count(id) == 0;
}
// Symbol: ?LoadCommandControls@CTaskDialog@@QEAAXHH@Z
extern "C" void MS_ABI impl__LoadCommandControls_CTaskDialog__QEAAXHH_Z(CTaskDialog* pThis, int firstId, int count) {
    if (!pThis) return;
    for (int i = 0; i < count; ++i) {
        wchar_t label[32] = {};
        swprintf(label, 32, L"%d", firstId + i);
        pThis->AddCommandControl(firstId + i, label);
    }
}
// Symbol: ?LoadRadioButtons@CTaskDialog@@QEAAXHH@Z
extern "C" void MS_ABI impl__LoadRadioButtons_CTaskDialog__QEAAXHH_Z(CTaskDialog* pThis, int firstId, int count) {
    if (!pThis) return;
    for (int i = 0; i < count; ++i) {
        wchar_t label[32] = {};
        swprintf(label, 32, L"%d", firstId + i);
        pThis->AddRadioButton(firstId + i, label);
    }
}
// Symbol: ?NavigateTo@CTaskDialog@@IEBAXAEAV1@@Z
extern "C" void MS_ABI impl__NavigateTo_CTaskDialog__IEBAXAEAV1__Z(CTaskDialog* pThis, CTaskDialog* other) {
    if (!pThis || !other) return;
    pThis->m_strContent = other->m_strContent;
    pThis->m_strMainInstruction = other->m_strMainInstruction;
    pThis->m_strWindowTitle = other->m_strWindowTitle;
    pThis->m_strFooterText = other->m_strFooterText;
    pThis->m_strVerificationText = other->m_strVerificationText;
    pThis->m_strExpandedInfo = other->m_strExpandedInfo;
    pThis->m_strExpandedControlText = other->m_strExpandedControlText;
    pThis->m_strCollapsedControlText = other->m_strCollapsedControlText;
    pThis->m_nCommonButtons = other->m_nCommonButtons;
    pThis->m_nTaskDialogOptions = other->m_nTaskDialogOptions;
}
// Symbol: ?Notify@CTaskDialog@@AEBAXI_K_J@Z
extern "C" void MS_ABI impl__Notify_CTaskDialog__AEBAXI_K_J_Z(const CTaskDialog*, UINT, uintptr_t, intptr_t) {
}
// Symbol: ?OnCommandControlClick@CTaskDialog@@MEAAJH@Z
extern "C" intptr_t MS_ABI impl__OnCommandControlClick_CTaskDialog__MEAAJH_Z(CTaskDialog* pThis, int id) {
    impl__ClickCommandControl_CTaskDialog__IEBAXH_Z(pThis, id);
    return S_OK;
}
// Symbol: ?OnCreate@CTaskDialog@@MEAAJXZ
extern "C" intptr_t MS_ABI impl__OnCreate_CTaskDialog__MEAAJXZ(CTaskDialog* pThis) {
    if (!pThis) return E_POINTER;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs[pThis];
    return S_OK;
}
// Symbol: ?OnDestroy@CTaskDialog@@MEAAJXZ
extern "C" intptr_t MS_ABI impl__OnDestroy_CTaskDialog__MEAAJXZ(CTaskDialog* pThis) {
    if (pThis) {
        std::lock_guard<std::mutex> lock(g_taskDialogMutex);
        g_taskDialogs.erase(pThis);
    }
    return S_OK;
}
// Symbol: ?OnExpandButtonClick@CTaskDialog@@MEAAJH@Z
extern "C" intptr_t MS_ABI impl__OnExpandButtonClick_CTaskDialog__MEAAJH_Z(CTaskDialog* pThis, int expanded) {
    if (pThis) pThis->m_bExpanded = expanded;
    return S_OK;
}
// Symbol: ?OnHelp@CTaskDialog@@MEAAJXZ
extern "C" intptr_t MS_ABI impl__OnHelp_CTaskDialog__MEAAJXZ(CTaskDialog*) {
    return S_OK;
}
// Symbol: ?OnHyperlinkClick@CTaskDialog@@MEAAJAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" intptr_t MS_ABI impl__OnHyperlinkClick_CTaskDialog__MEAAJAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog*, const CString*) {
    return S_OK;
}
// Symbol: ?OnInit@CTaskDialog@@MEAAJXZ
extern "C" intptr_t MS_ABI impl__OnInit_CTaskDialog__MEAAJXZ(CTaskDialog*) {
    return S_OK;
}
// Symbol: ?OnNavigatePage@CTaskDialog@@MEAAJXZ
extern "C" intptr_t MS_ABI impl__OnNavigatePage_CTaskDialog__MEAAJXZ(CTaskDialog*) {
    return S_OK;
}
// Symbol: ?OnRadioButtonClick@CTaskDialog@@MEAAJH@Z
extern "C" intptr_t MS_ABI impl__OnRadioButtonClick_CTaskDialog__MEAAJH_Z(CTaskDialog* pThis, int id) {
    impl__ClickRadioButton_CTaskDialog__IEBAXH_Z(pThis, id);
    return S_OK;
}
// Symbol: ?OnTimer@CTaskDialog@@MEAAJJ@Z
extern "C" intptr_t MS_ABI impl__OnTimer_CTaskDialog__MEAAJJ_Z(CTaskDialog*, intptr_t) {
    return S_OK;
}
// Symbol: ?OnVerificationCheckboxClick@CTaskDialog@@MEAAJH@Z
extern "C" intptr_t MS_ABI impl__OnVerificationCheckboxClick_CTaskDialog__MEAAJH_Z(CTaskDialog* pThis, int checked) {
    if (pThis) pThis->m_bVerificationChecked = checked;
    return S_OK;
}
// Symbol: ?RemoveAllCommandControls@CTaskDialog@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllCommandControls_CTaskDialog__QEAAXXZ(CTaskDialog* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs[pThis].commandControls = {};
}
// Symbol: ?RemoveAllRadioButtons@CTaskDialog@@QEAAXXZ
extern "C" void MS_ABI impl__RemoveAllRadioButtons_CTaskDialog__QEAAXXZ(CTaskDialog* pThis) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs[pThis].radioButtons = {};
}
// Symbol: ?SetCommandControlOptions@CTaskDialog@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetCommandControlOptions_CTaskDialog__QEAAXHHH_Z(CTaskDialog* pThis, int id, int enabled, int) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    auto& disabled = g_taskDialogs[pThis].commandControls.disabled;
    if (enabled) disabled.erase(id); else disabled.insert(id);
}
// Symbol: ?SetCommonButtonOptions@CTaskDialog@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetCommonButtonOptions_CTaskDialog__QEAAXHH_Z(
    CTaskDialog* pThis, int commonButton, int bEnable) {
    if (!pThis) return;
    if (bEnable) {
        pThis->m_nCommonButtons |= commonButton;
    } else {
        pThis->m_nCommonButtons &= ~commonButton;
    }
}
// Symbol: ?SetCommonButtons@CTaskDialog@@QEAAXHHH@Z
extern "C" void MS_ABI impl__SetCommonButtons_CTaskDialog__QEAAXHHH_Z(CTaskDialog* pThis, int buttons, int, int) {
    if (pThis) pThis->m_nCommonButtons = buttons;
}
// Symbol: ?SetDefaultCommandControl@CTaskDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDefaultCommandControl_CTaskDialog__QEAAXH_Z(CTaskDialog* pThis, int id) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs[pThis].commandControls.defaultButton = id;
}
// Symbol: ?SetDefaultRadioButton@CTaskDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__SetDefaultRadioButton_CTaskDialog__QEAAXH_Z(CTaskDialog* pThis, int id) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs[pThis].radioButtons.defaultButton = id;
}
// Symbol: ?SetExpansionArea@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00@Z
extern "C" void MS_ABI impl__SetExpansionArea_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00_Z(
    CTaskDialog* pThis, const CString* expanded, const CString* expandedControl, const CString* collapsedControl) {
    if (!pThis) return;
    if (expanded) pThis->m_strExpandedInfo = *expanded;
    if (expandedControl) pThis->m_strExpandedControlText = *expandedControl;
    if (collapsedControl) pThis->m_strCollapsedControlText = *collapsedControl;
}
// Symbol: ?SetProgressBarState@CTaskDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__SetProgressBarState_CTaskDialog__QEAAXH_Z(CTaskDialog* pThis, int state) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs[pThis].progressState = state;
}
// Symbol: ?SetRadioButtonOptions@CTaskDialog@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetRadioButtonOptions_CTaskDialog__QEAAXHH_Z(CTaskDialog* pThis, int id, int enabled) {
    if (!pThis) return;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    auto& disabled = g_taskDialogs[pThis].radioButtons.disabled;
    if (enabled) disabled.erase(id); else disabled.insert(id);
}
// Symbol: ?SetVerificationCheckbox@CTaskDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__SetVerificationCheckbox_CTaskDialog__QEAAXH_Z(CTaskDialog* pThis, int checked) {
    if (pThis) pThis->m_bVerificationChecked = checked;
}
CTaskDialog::CTaskDialog(const wchar_t* pszContent, const wchar_t* pszMainInstruction,
                          const wchar_t* pszWindowTitle, int nCommonButtons, int nTaskDialogOptions)
    : m_nCommonButtons(nCommonButtons), m_nTaskDialogOptions(nTaskDialogOptions),
      m_nDialogWidth(0), m_hMainIcon(nullptr), m_hFooterIcon(nullptr),
      m_nProgressMin(0), m_nProgressMax(100), m_nProgressPos(0),
      m_bProgressMarquee(FALSE), m_bVerificationChecked(FALSE),
      m_nSelectedCommandID(0), m_nSelectedRadioButtonID(0), m_bExpanded(FALSE) {
    if (pszContent) m_strContent = pszContent;
    if (pszMainInstruction) m_strMainInstruction = pszMainInstruction;
    if (pszWindowTitle) m_strWindowTitle = pszWindowTitle;
    memset(_taskdialog_padding, 0, sizeof(_taskdialog_padding));
}
CTaskDialog::~CTaskDialog() {
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    g_taskDialogs.erase(this);
}
void CTaskDialog::SetDialogWidth(int nWidth) {
    m_nDialogWidth = nWidth;
}
void CTaskDialog::SetMainIcon(HICON hMainIcon) {
    m_hMainIcon = hMainIcon;
}
void CTaskDialog::SetMainIcon(UINT nMainIconID) {
    m_hMainIcon = ::LoadIconW(nullptr, MAKEINTRESOURCEW(nMainIconID));
}
void CTaskDialog::SetFooterIcon(HICON hFooterIcon) {
    m_hFooterIcon = hFooterIcon;
}
void CTaskDialog::SetFooterText(const wchar_t* pszFooterText) {
    if (pszFooterText) m_strFooterText = pszFooterText;
}
void CTaskDialog::SetVerificationCheckboxText(const wchar_t* pszText) {
    if (pszText) m_strVerificationText = pszText;
}
void CTaskDialog::SetExpandedInformation(const wchar_t* pszText) {
    if (pszText) m_strExpandedInfo = pszText;
}
void CTaskDialog::SetExpandedControlText(const wchar_t* pszText) {
    if (pszText) m_strExpandedControlText = pszText;
}
void CTaskDialog::SetCollapsedControlText(const wchar_t* pszText) {
    if (pszText) m_strCollapsedControlText = pszText;
}
void CTaskDialog::SetProgressBarRange(int nMin, int nMax) {
    m_nProgressMin = nMin;
    m_nProgressMax = nMax;
}
void CTaskDialog::SetProgressBarPosition(int nPos) {
    m_nProgressPos = nPos;
}
void CTaskDialog::SetProgressBarMarquee(BOOL bMarquee, int nSpeed) {
    m_bProgressMarquee = bMarquee;
    (void)nSpeed;
}
HRESULT CTaskDialog::AddCommandControl(int nCommandID, const wchar_t* pszLabel) {
    if (nCommandID <= 0 || !pszLabel) return E_INVALIDARG;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    AddTaskDialogButton(g_taskDialogs[this].commandControls, nCommandID, pszLabel, TRUE);
    return S_OK;
}
HRESULT CTaskDialog::AddRadioButton(int nRadioButtonID, const wchar_t* pszLabel) {
    if (nRadioButtonID <= 0 || !pszLabel) return E_INVALIDARG;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    AddTaskDialogButton(g_taskDialogs[this].radioButtons, nRadioButtonID, pszLabel, TRUE);
    return S_OK;
}
HRESULT CTaskDialog::AddPushButton(int nButtonID, const wchar_t* pszLabel) {
    if (nButtonID <= 0 || !pszLabel) return E_INVALIDARG;
    std::lock_guard<std::mutex> lock(g_taskDialogMutex);
    AddTaskDialogButton(g_taskDialogs[this].commandControls, nButtonID, pszLabel, TRUE);
    return S_OK;
}
int CTaskDialog::DoModal(HWND hWndParent) {
    // Use TaskDialogIndirect if available (Vista+), otherwise fallback to MessageBox
    TASKDIALOGCONFIG tc = {};
    FillTaskDialogConfig(this, tc);
    tc.hwndParent = hWndParent;

    int nButton = 0;
    int nRadio = 0;
    BOOL bVerification = FALSE;
    TaskDialogState state;
    {
        std::lock_guard<std::mutex> lock(g_taskDialogMutex);
        auto it = g_taskDialogs.find(this);
        if (it != g_taskDialogs.end()) state = it->second;
    }

    std::vector<TASKDIALOG_BUTTON> taskDialogButtons = state.commandControls.buttons;
    std::vector<TASKDIALOG_BUTTON> radioButtons = state.radioButtons.buttons;

    if (!taskDialogButtons.empty()) {
        tc.cButtons = static_cast<UINT>(taskDialogButtons.size());
        tc.pButtons = taskDialogButtons.data();
        tc.nDefaultButton = taskDialogButtons.front().nButtonID;
    }
    if (!radioButtons.empty()) {
        tc.cRadioButtons = static_cast<UINT>(radioButtons.size());
        tc.pRadioButtons = radioButtons.data();
        tc.nDefaultRadioButton = radioButtons.front().nButtonID;
        tc.dwFlags |= TDF_ALLOW_DIALOG_CANCELLATION;
    }
    if (!state.commandControls.buttons.empty()) {
        tc.dwFlags |= TDF_USE_COMMAND_LINKS;
    }

    using TaskDialogIndirectFn = HRESULT (WINAPI *)(const TASKDIALOGCONFIG*, int*, int*, BOOL*);
    HMODULE hComctl = ::GetModuleHandleW(L"comctl32.dll");
    if (!hComctl) {
        hComctl = ::LoadLibraryW(L"comctl32.dll");
    }
    TaskDialogIndirectFn pTaskDialogIndirect = hComctl
        ? reinterpret_cast<TaskDialogIndirectFn>(::GetProcAddress(hComctl, "TaskDialogIndirect"))
        : nullptr;

    if (pTaskDialogIndirect) {
        HRESULT hr = pTaskDialogIndirect(&tc, &nButton, &nRadio, &bVerification);
        if (SUCCEEDED(hr)) {
            m_bVerificationChecked = bVerification;
            m_nSelectedRadioButtonID = nRadio;
            m_nSelectedCommandID = nButton;
            return nButton;
        }
    }

    // Fallback to simple MessageBox
    UINT uType = MB_OK;
    if ((m_nCommonButtons & CTaskDialog::TDCBF_YES_BUTTON) && (m_nCommonButtons & CTaskDialog::TDCBF_NO_BUTTON) &&
        (m_nCommonButtons & CTaskDialog::TDCBF_CANCEL_BUTTON)) {
        uType = MB_YESNOCANCEL;
    } else if ((m_nCommonButtons & CTaskDialog::TDCBF_YES_BUTTON) && (m_nCommonButtons & CTaskDialog::TDCBF_NO_BUTTON)) {
        uType = MB_YESNO;
    } else if ((m_nCommonButtons & CTaskDialog::TDCBF_OK_BUTTON) && (m_nCommonButtons & CTaskDialog::TDCBF_CANCEL_BUTTON)) {
        uType = MB_OKCANCEL;
    } else if (m_nCommonButtons & CTaskDialog::TDCBF_RETRY_BUTTON) {
        uType = MB_RETRYCANCEL;
    } else if (m_nCommonButtons & CTaskDialog::TDCBF_YES_BUTTON) {
        uType = MB_YESNO;
    } else if (m_nCommonButtons & CTaskDialog::TDCBF_CANCEL_BUTTON) {
        uType = MB_OKCANCEL;
    } else {
        uType = MB_OK;
    }

    if (!taskDialogButtons.empty()) {
        uType = MB_OK;
    }

    int result = ::MessageBoxW(hWndParent, m_strContent, m_strWindowTitle, uType);
    switch (result) {
        case IDOK:
            if (!taskDialogButtons.empty()) {
                m_nSelectedCommandID = taskDialogButtons.front().nButtonID;
                return m_nSelectedCommandID;
            }
            return IDOK;
        case IDYES: return IDYES;
        case IDNO: return IDNO;
        case IDCANCEL: return IDCANCEL;
        case IDRETRY: return IDRETRY;
        default:
            if (!taskDialogButtons.empty()) {
                m_nSelectedCommandID = taskDialogButtons.front().nButtonID;
                return m_nSelectedCommandID;
            }
            if (!state.radioButtons.buttons.empty()) {
                m_nSelectedRadioButtonID = state.radioButtons.buttons.front().nButtonID;
                return m_nSelectedRadioButtonID;
            }
            return IDOK;
    }

}
// Symbol: ??0CTaskDialog@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00HH0@Z
extern "C" void MS_ABI impl___0CTaskDialog__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HH0_Z(
    CTaskDialog* pThis, const CString* p0, const CString* p1, const CString* p2, int p3, int p4, const CString* p5) {
    new(pThis) CTaskDialog(*p0, *p1, *p2, p3, p4, p5 ? *p5 : CString());
}
// Symbol: ??0CTaskDialog@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00HHHH0@Z
extern "C" void MS_ABI impl___0CTaskDialog__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HHHH0_Z(
    CTaskDialog* pThis, const CString* p0, const CString* p1, const CString* p2, int p3, int p4, int p5, int p6, const CString* p7) {
    new(pThis) CTaskDialog(*p0, *p1, *p2, p3, p4, p5, p6, p7 ? *p7 : CString());
}
// Symbol: ?SetContent@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetContent_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->m_strContent = *p0;
}
// Symbol: ?SetMainInstruction@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetMainInstruction_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->m_strMainInstruction = *p0;
}
// Symbol: ?SetWindowTitle@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetWindowTitle_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->m_strWindowTitle = *p0;
}
// Symbol: ?SetOptions@CTaskDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__SetOptions_CTaskDialog__QEAAXH_Z(CTaskDialog* pThis, int p0) {
    if (pThis) pThis->SetOptions(p0);
}
// Symbol: ?GetOptions@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetOptions_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetOptions() : 0;
}
// Symbol: ?GetVerificationCheckboxState@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetVerificationCheckboxState_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetVerificationCheckboxState() : FALSE;
}
// Symbol: ?GetSelectedCommandControlID@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetSelectedCommandControlID_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetSelectedCommandControlID() : 0;
}
// Symbol: ?GetSelectedRadioButtonID@CTaskDialog@@QEBAHXZ
extern "C" int MS_ABI impl__GetSelectedRadioButtonID_CTaskDialog__QEBAHXZ(const CTaskDialog* pThis) {
    return pThis ? pThis->GetSelectedRadioButtonID() : 0;
}
// Symbol: ?SetFooterText@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetFooterText_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->SetFooterText(*p0);
}
// Symbol: ?SetVerificationCheckboxText@CTaskDialog@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__SetVerificationCheckboxText_CTaskDialog__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CTaskDialog* pThis, const CString* p0) {
    if (pThis && p0) pThis->SetVerificationCheckboxText(*p0);
}
// Symbol: ?SetMainIcon@CTaskDialog@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetMainIcon_CTaskDialog__QEAAXPEB_W_Z(CTaskDialog* pThis, const wchar_t* p0) {
    if (pThis) pThis->SetMainIcon(p0);
}
// Symbol: ?SetFooterIcon@CTaskDialog@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetFooterIcon_CTaskDialog__QEAAXPEB_W_Z(CTaskDialog* pThis, const wchar_t* p0) {
    if (pThis) pThis->SetFooterIcon(p0);
}
// Symbol: ?AddCommandControl@CTaskDialog@@QEAAXHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@HH@Z
extern "C" void MS_ABI impl__AddCommandControl_CTaskDialog__QEAAXHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__HH_Z(
    CTaskDialog* pThis, int p0, const CString* p1, int p2, int p3) {
    if (pThis && p1) pThis->AddCommandControl(p0, *p1, p2, p3);
}
// Symbol: ?AddRadioButton@CTaskDialog@@QEAAXHAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" void MS_ABI impl__AddRadioButton_CTaskDialog__QEAAXHAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CTaskDialog* pThis, int p0, const CString* p1, int p2) {
    if (pThis && p1) pThis->AddRadioButton(p0, *p1, p2);
}
// Symbol: ?IsSupported@CTaskDialog@@SAHXZ
extern "C" int MS_ABI impl__IsSupported_CTaskDialog__SAHXZ() {
    return CTaskDialog::IsSupported();
}
// Symbol: ?ShowDialog@CTaskDialog@@SA_JAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@00HHHH0@Z
extern "C" __int64 MS_ABI impl__ShowDialog_CTaskDialog__SA_JAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__00HHHH0_Z(
    const CString* p0, const CString* p1, const CString* p2, int p3, int p4, int p5, int p6, const CString* p7) {
    CTaskDialog td(*p0, *p1, *p2, p3, p4, p5, p6, p7 ? *p7 : CString());
    return td.DoModal(nullptr);
}
CTaskDialog::CTaskDialog(const CString& strContent, const CString& strMainInstruction,
                         const CString& strWindowTitle, int nCommonButtons,
                         int nTaskDialogOptions, const CString& strFooter)
    : CTaskDialog((const wchar_t*)strContent, (const wchar_t*)strMainInstruction,
                  (const wchar_t*)strWindowTitle, nCommonButtons, nTaskDialogOptions) {
    if (!strFooter.IsEmpty()) {
        m_strFooterText = strFooter;
    }
}
CTaskDialog::CTaskDialog(const CString& strContent, const CString& strMainInstruction,
                         const CString& strWindowTitle, int nCommonButtons,
                         int nTaskDialogOptions, int, int, const CString& strFooter)
    : CTaskDialog(strContent, strMainInstruction, strWindowTitle, nCommonButtons, nTaskDialogOptions, strFooter) {}
void CTaskDialog::SetMainIcon(const wchar_t* pszMainIcon) {
    m_hMainIcon = pszMainIcon ? ::LoadIconW(nullptr, pszMainIcon) : nullptr;
}
void CTaskDialog::SetFooterIcon(const wchar_t* pszFooterIcon) {
    m_hFooterIcon = pszFooterIcon ? ::LoadIconW(nullptr, pszFooterIcon) : nullptr;
}
void CTaskDialog::SetFooterText(const CString& strFooterText) {
    m_strFooterText = strFooterText;
}
void CTaskDialog::SetVerificationCheckboxText(const CString& strText) {
    m_strVerificationText = strText;
}
void CTaskDialog::AddCommandControl(int nCommandID, const CString& strLabel, int, int) {
    (void)AddCommandControl(nCommandID, (const wchar_t*)strLabel);
}
void CTaskDialog::AddRadioButton(int nRadioButtonID, const CString& strLabel, int) {
    (void)AddRadioButton(nRadioButtonID, (const wchar_t*)strLabel);
}
void CTaskDialog::SetOptions(int nTaskDialogOptions) {
    m_nTaskDialogOptions = nTaskDialogOptions;
}
int CTaskDialog::GetOptions() const {
    return m_nTaskDialogOptions;
}
BOOL CTaskDialog::IsSupported() {
    HMODULE hComctl = ::GetModuleHandleW(L"comctl32.dll");
    if (!hComctl) {
        hComctl = ::LoadLibraryW(L"comctl32.dll");
    }
    return hComctl && (::GetProcAddress(hComctl, "TaskDialogIndirect") != nullptr);
}
