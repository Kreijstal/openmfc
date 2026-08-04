// CFileDialog — OpenMFC implementation.
// Sources: dlgcommon.cpp, global_ctordtor_param.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"

// Real parameterized-constructor export thunks for header-defined, publicly
// constructible MFC classes. Each viewed-as MS_ABI thunk placement-constructs
// the object with the real ctor body (defined in filecore/olecore/dlgcommon/
// inetcore or inline in headers) -- NOT a stub. Completes classes that were one
// export short of full coverage.
#include "openmfc/afx.h"
#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"
#include "openmfc/afxole.h"
#include "openmfc/afxdisp.h"
#include "openmfc/afxdb.h"
#include "openmfc/afxinet.h"
#include <new>
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif


















// Symbol: ?DoModal@CFileDialog@@UEAA_JXZ
intptr_t CFileDialog::DoModal() {
    CFileDialogControlState* state = GetFileDialogState(this, true);
    if (state) {
        state->selectedPaths.clear();
        state->controlStates.clear();
        state->checkState.clear();
        state->editText.clear();
        state->controlLabels.clear();
        state->selectedItem.clear();
        state->controlItems.clear();
        state->visualGroupActive = false;
    }

    // Allocate buffer for multiple file selection
    const size_t nBufferSize = 65536;
    std::vector<wchar_t> buffer(nBufferSize);
    wchar_t* szFile = buffer.data();
    memset(szFile, 0, nBufferSize * sizeof(wchar_t));

    if (!m_strFileName.IsEmpty()) {
        wcsncpy(szFile, (const wchar_t*)m_strFileName, MAX_PATH - 1);
    }

    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = state ? state->owner : nullptr;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = (DWORD)nBufferSize;
    ofn.lpstrFilter = m_strFilter.IsEmpty() ? L"All Files\0*.*\0" : (const wchar_t*)m_strFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrDefExt = m_strDefExt.IsEmpty() ? nullptr : (const wchar_t*)m_strDefExt;
    ofn.lpstrInitialDir = m_strInitialDir.IsEmpty() ? nullptr : (const wchar_t*)m_strInitialDir;
    ofn.Flags = m_dwFlags | OFN_EXPLORER;

    BOOL bResult;
    if (m_bOpenFileDialog) {
        bResult = GetOpenFileNameW(&ofn);
    } else {
        bResult = GetSaveFileNameW(&ofn);
    }

    if (bResult) {
        // Check for multiple file selection (OFN_ALLOWMULTISELECT flag)
        if ((m_dwFlags & OFN_ALLOWMULTISELECT) && szFile[wcslen(szFile) + 1] != L'\0') {
            // Multiple files selected: directory followed by null-separated filenames
            m_strFolderPath = szFile;
            if (state) {
                const wchar_t* pFirstFile = szFile + wcslen(szFile) + 1;
                while (*pFirstFile != L'\0') {
                    const wchar_t* pNextNull = pFirstFile + wcslen(pFirstFile) + 1;
                    CString filePath = m_strFolderPath + L"\\" + CString(pFirstFile);
                    state->selectedPaths.push_back(filePath);
                    pFirstFile = pNextNull;
                }

                if (!state->selectedPaths.empty()) {
                    m_strPathName = state->selectedPaths[0];
                    const wchar_t* pFileName = wcsrchr((const wchar_t*)m_strPathName, L'\\');
                    m_strFileNameOnly = pFileName != nullptr ? CString(pFileName + 1) : m_strPathName;
                }
            }
        } else {
            CString fullPath = szFile;
            m_strPathName = fullPath;
            if (state) {
                state->selectedPaths.clear();
                state->selectedPaths.push_back(fullPath);
            }

            // Extract file name from path
            const wchar_t* pFileName = wcsrchr((const wchar_t*)fullPath, L'\\');
            if (pFileName != nullptr) {
                m_strFileNameOnly = pFileName + 1;
                m_strFolderPath = m_strPathName.Left((int)(pFileName - (const wchar_t*)fullPath));
            } else {
                m_strFileNameOnly = szFile;
                m_strFolderPath = L"";
            }
        }

        // Extract extension
        const wchar_t* pExt = wcsrchr((const wchar_t*)m_strFileNameOnly, L'.');
        if (pExt != nullptr) {
            m_strFileExt = pExt + 1;
            m_strFileTitle = m_strFileNameOnly.Left((int)(pExt - (const wchar_t*)m_strFileNameOnly));
        } else {
            m_strFileExt = L"";
            m_strFileTitle = m_strFileNameOnly;
        }

        return IDOK;
    }

    return IDCANCEL;
}
// Symbol: ?GetPathName@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetPathName() const {
    return m_strPathName;
}
// Symbol: ?GetFileName@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFileName() const {
    return m_strFileNameOnly;
}
// Symbol: ?GetFileExt@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFileExt() const {
    return m_strFileExt;
}
// Symbol: ?GetFileTitle@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFileTitle() const {
    return m_strFileTitle;
}
// Symbol: ?GetFolderPath@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
CString CFileDialog::GetFolderPath() const {
    return m_strFolderPath;
}
// Symbol: ?GetNextPathName@CFileDialog@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAPEAU__POSITION@@@Z
CString CFileDialog::GetNextPathName(void*& pos) const {
    const CFileDialogControlState* state = GetFileDialogState(this, false);
    if (!state) {
        pos = nullptr;
        return CString();
    }

    size_t index = (pos == nullptr) ? 0 : (reinterpret_cast<uintptr_t>(pos) - 1);
    if (index >= state->selectedPaths.size()) {
        pos = nullptr;
        return CString();
    }

    CString result = state->selectedPaths[index];
    pos = (index + 1 < state->selectedPaths.size()) ? reinterpret_cast<void*>(index + 2) : nullptr;
    return result;
}
// Symbol: ?GetInterfaceMap@CFileDialog@@MEBAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetInterfaceMap_CFileDialog__MEBAPEBUAFX_INTERFACEMAP__XZ(
    const CFileDialog* pThis) {
    return pThis ? pThis->GetInterfaceMap() : CWnd::GetThisInterfaceMap();
}
// Symbol: ?GetOFN@CFileDialog@@QEAAAEAUtagOFNW@@XZ
extern "C" OPENFILENAMEW* MS_ABI impl__GetOFN_CFileDialog__QEAAAEAUtagOFNW__XZ(CFileDialog* pThis) {
    return &OpenMfcGetOFNSnapshot(pThis);
}
// Symbol: ?GetOFN@CFileDialog@@QEBAAEBUtagOFNW@@XZ
extern "C" const OPENFILENAMEW* MS_ABI impl__GetOFN_CFileDialog__QEBAAEBUtagOFNW__XZ(const CFileDialog* pThis) {
    return &OpenMfcGetOFNSnapshot(pThis);
}
// Symbol: ?GetRuntimeClass@CFileDialog@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CFileDialog__UEBAPEAUCRuntimeClass__XZ(
    const CFileDialog* pThis) {
    return CFileDialog::GetThisClass();
}
// Symbol: ?GetThisClass@CFileDialog@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CFileDialog__SAPEAUCRuntimeClass__XZ() {
    return CFileDialog::GetThisClass();
}
// Symbol: ?GetThisInterfaceMap@CFileDialog@@KAPEBUAFX_INTERFACEMAP@@XZ
extern "C" const AFX_INTERFACEMAP* MS_ABI impl__GetThisInterfaceMap_CFileDialog__KAPEBUAFX_INTERFACEMAP__XZ() {
    return CWnd::GetThisInterfaceMap();
}
// Symbol: ?AddComboBox@CFileDialog@@QEAAJK@Z
extern "C" HRESULT MS_ABI impl__AddComboBox_CFileDialog__QEAAJK_Z(CFileDialog* pThis, unsigned long p0) {
    if (!pThis) return E_INVALIDARG;

    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) {
        return E_OUTOFMEMORY;
    }
    state->controlLabels.emplace(p0, CString());
    return S_OK;
}
// Symbol: ?AddCheckButton@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" HRESULT MS_ABI impl__AddCheckButton_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* label, int bChecked) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->controlLabels[dwIDCtl], label);
    state->checkState[dwIDCtl] = bChecked ? TRUE : FALSE;
    return S_OK;
}
// Symbol: ?AddControlItem@CFileDialog@@QEAAJKKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__AddControlItem_CFileDialog__QEAAJKKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, unsigned long dwIDItem, const CString* label) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    CString itemLabel;
    SetTextOrEmpty(&itemLabel, label);
    auto& items = state->controlItems[dwIDCtl];
    items[dwIDItem].text = itemLabel;
    return S_OK;
}
// Symbol: ?AddEditBox@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__AddEditBox_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* text) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->editText[dwIDCtl], text);
    return S_OK;
}
// Symbol: ?AddMenu@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__AddMenu_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* label) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->controlLabels[dwIDCtl], label);
    return S_OK;
}
// Symbol: ?AddPushButton@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__AddPushButton_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* label) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->controlLabels[dwIDCtl], label);
    return S_OK;
}
// Symbol: ?AddText@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__AddText_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* text) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->controlLabels[dwIDCtl], text);
    return S_OK;
}
// Symbol: ?AddPlace@CFileDialog@@QEAAXPEAUIShellItem@@W4FDAP@@@Z
extern "C" void MS_ABI impl__AddPlace_CFileDialog__QEAAXPEAUIShellItem__W4FDAP___Z(
    CFileDialog* pThis, void* p0, int p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}
// Symbol: ?AddPlace@CFileDialog@@QEAAXPEB_WW4FDAP@@@Z
extern "C" void MS_ABI impl__AddPlace_CFileDialog__QEAAXPEB_WW4FDAP___Z(
    CFileDialog* pThis, const wchar_t* p0, int p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}
// Symbol: ?ApplyOFNToShellDialog@CFileDialog@@QEAAXXZ
extern "C" void MS_ABI impl__ApplyOFNToShellDialog_CFileDialog__QEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}
// Symbol: ?AddRadioButtonList@CFileDialog@@QEAAJK@Z
extern "C" HRESULT MS_ABI impl__AddRadioButtonList_CFileDialog__QEAAJK_Z(
    CFileDialog* pThis, unsigned long p0) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->controlLabels[p0] = L"";
    state->controlStates[p0] = 0;
    state->selectedItem[p0] = 0;
    return S_OK;
}
// Symbol: ?AddSeparator@CFileDialog@@QEAAJK@Z
extern "C" HRESULT MS_ABI impl__AddSeparator_CFileDialog__QEAAJK_Z(CFileDialog* pThis, unsigned long p0) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->controlLabels[p0] = L"---";
    return S_OK;
}
// Symbol: ?EnableOpenDropDown@CFileDialog@@QEAAJK@Z
extern "C" HRESULT MS_ABI impl__EnableOpenDropDown_CFileDialog__QEAAJK_Z(
    CFileDialog* pThis, unsigned long p0) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->openDropDownEnabled = TRUE;
    state->controlLabels[p0] = L"";
    return S_OK;
}
// Symbol: ?EndVisualGroup@CFileDialog@@QEAAJXZ
extern "C" HRESULT MS_ABI impl__EndVisualGroup_CFileDialog__QEAAJXZ(CFileDialog* pThis) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) return S_FALSE;
    state->visualGroupActive = FALSE;
    state->activeVisualGroup = 0;
    return S_OK;
}
// Symbol: ?GetCheckButtonState@CFileDialog@@QEAAJKAEAH@Z
extern "C" HRESULT MS_ABI impl__GetCheckButtonState_CFileDialog__QEAAJKAEAH_Z(
    CFileDialog* pThis, unsigned long p0, int* p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) {
        if (p1 != nullptr) {
            *p1 = FALSE;
        }
        return S_FALSE;
    }
    if (p1 != nullptr) {
        auto it = state->checkState.find(p0);
        *p1 = (it == state->checkState.end()) ? FALSE : it->second;
    }
    return S_OK;
}
// Symbol: ?GetEditBoxText@CFileDialog@@QEAAJKAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__GetEditBoxText_CFileDialog__QEAAJKAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, CString* text) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) {
        if (text) text->Empty();
        return S_FALSE;
    }
    auto it = state->editText.find(dwIDCtl);
    if (it == state->editText.end()) {
        if (text) text->Empty();
        return S_FALSE;
    }
    if (text) {
        *text = it->second;
    }
    return S_OK;
}
// Symbol: ?GetControlItemState@CFileDialog@@QEAAJKKAEAW4CDCONTROLSTATEF@@@Z
extern "C" HRESULT MS_ABI impl__GetControlItemState_CFileDialog__QEAAJKKAEAW4CDCONTROLSTATEF___Z(
    CFileDialog* pThis, unsigned long p0, unsigned long p1, unsigned int* p2) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) {
        if (p2 != nullptr) {
            *p2 = 0;
        }
        return S_FALSE;
    }
    auto itControl = state->controlItems.find(p0);
    if (itControl != state->controlItems.end()) {
        auto itItem = itControl->second.find(p1);
        if (itItem != itControl->second.end()) {
            if (p2 != nullptr) {
                *p2 = itItem->second.state;
            }
            return S_OK;
        }
    }
    if (p2 != nullptr) {
        *p2 = 0;
    }
    return S_FALSE;
}
// Symbol: ?GetControlState@CFileDialog@@QEAAJKAEAW4CDCONTROLSTATEF@@@Z
extern "C" HRESULT MS_ABI impl__GetControlState_CFileDialog__QEAAJKAEAW4CDCONTROLSTATEF___Z(
    CFileDialog* pThis, unsigned long p0, unsigned int* p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) {
        if (p1 != nullptr) {
            *p1 = 0;
        }
        return S_FALSE;
    }
    auto it = state->controlStates.find(p0);
    if (p1 != nullptr) {
        *p1 = (it == state->controlStates.end()) ? 0 : it->second;
    }
    return (it == state->controlStates.end()) ? S_FALSE : S_OK;
}
// Symbol: ?GetIFileDialogCustomize@CFileDialog@@QEAAPEAUIFileDialogCustomize@@XZ
extern "C" void* MS_ABI impl__GetIFileDialogCustomize_CFileDialog__QEAAPEAUIFileDialogCustomize__XZ(
    CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetIFileOpenDialog@CFileDialog@@QEAAPEAUIFileOpenDialog@@XZ
extern "C" void* MS_ABI impl__GetIFileOpenDialog_CFileDialog__QEAAPEAUIFileOpenDialog__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetIFileSaveDialog@CFileDialog@@QEAAPEAUIFileSaveDialog@@XZ
extern "C" void* MS_ABI impl__GetIFileSaveDialog_CFileDialog__QEAAPEAUIFileSaveDialog__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetResult@CFileDialog@@QEAAPEAUIShellItem@@XZ
extern "C" void* MS_ABI impl__GetResult_CFileDialog__QEAAPEAUIShellItem__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetResults@CFileDialog@@QEAAPEAUIShellItemArray@@XZ
extern "C" void* MS_ABI impl__GetResults_CFileDialog__QEAAPEAUIShellItemArray__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetSelectedControlItem@CFileDialog@@QEAAJKAEAK@Z
extern "C" HRESULT MS_ABI impl__GetSelectedControlItem_CFileDialog__QEAAJKAEAK_Z(
    CFileDialog* pThis, unsigned long p0, unsigned long* p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) {
        if (p1 != nullptr) {
            *p1 = 0;
        }
        return S_FALSE;
    }
    auto it = state->selectedItem.find(p0);
    if (p1 != nullptr) {
        *p1 = (it == state->selectedItem.end()) ? 0 : it->second;
    }
    return (it == state->selectedItem.end()) ? S_FALSE : S_OK;
}
// Symbol: ?HideControl@CFileDialog@@QEAAXH@Z
extern "C" void MS_ABI impl__HideControl_CFileDialog__QEAAXH_Z(CFileDialog* pThis, int p0) {
    if (!pThis || !pThis->GetSafeHwnd()) return;
    HWND hControl = ::GetDlgItem(pThis->GetSafeHwnd(), p0);
    if (hControl) {
        ::ShowWindow(hControl, SW_HIDE);
    }
}
// Symbol: ?MakeProminent@CFileDialog@@QEAAJK@Z
extern "C" HRESULT MS_ABI impl__MakeProminent_CFileDialog__QEAAJK_Z(CFileDialog* pThis, unsigned long p0) {
    (void)pThis;
    (void)p0;
    return S_OK;
}
// Symbol: ?OnButtonClicked@CFileDialog@@MEAAXK@Z
extern "C" void MS_ABI impl__OnButtonClicked_CFileDialog__MEAAXK_Z(CFileDialog* pThis, unsigned long p0) {
    (void)pThis;
    (void)p0;
}
// Symbol: ?OnCheckButtonToggled@CFileDialog@@MEAAXKH@Z
extern "C" void MS_ABI impl__OnCheckButtonToggled_CFileDialog__MEAAXKH_Z(
    CFileDialog* pThis, unsigned long p0, int p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}
// Symbol: ?OnControlActivating@CFileDialog@@MEAAXK@Z
extern "C" void MS_ABI impl__OnControlActivating_CFileDialog__MEAAXK_Z(
    CFileDialog* pThis, unsigned long p0) {
    (void)pThis;
    (void)p0;
}
// Symbol: ?OnFileNameChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnFileNameChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}
// Symbol: ?OnFileNameOK@CFileDialog@@MEAAHXZ
extern "C" int MS_ABI impl__OnFileNameOK_CFileDialog__MEAAHXZ(CFileDialog* pThis) {
    (void)pThis;
    return FALSE;
}
// Symbol: ?OnFolderChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnFolderChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}
// Symbol: ?OnInitDone@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnInitDone_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}
// Symbol: ?OnItemSelected@CFileDialog@@MEAAXKK@Z
extern "C" void MS_ABI impl__OnItemSelected_CFileDialog__MEAAXKK_Z(
    CFileDialog* pThis, unsigned long p0, unsigned long p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}
// Symbol: ?OnLBSelChangedNotify@CFileDialog@@MEAAXIII@Z
extern "C" void MS_ABI impl__OnLBSelChangedNotify_CFileDialog__MEAAXIII_Z(
    CFileDialog* pThis, unsigned int p0, unsigned int p1, unsigned int p2) {
    (void)pThis;
    (void)p0;
    (void)p1;
    (void)p2;
}
// Symbol: ?OnNotify@CFileDialog@@MEAAH_K_JPEA_J@Z
extern "C" int MS_ABI impl__OnNotify_CFileDialog__MEAAH_K_JPEA_J_Z(
    CFileDialog* pThis, uintptr_t p0, intptr_t p1, intptr_t* p2) {
    (void)pThis;
    (void)p0;
    (void)p1;
    if (p2 != nullptr) {
        *p2 = 0;
    }
    return FALSE;
}
// Symbol: ?OnShareViolation@CFileDialog@@MEAAIPEB_W@Z
extern "C" unsigned int MS_ABI impl__OnShareViolation_CFileDialog__MEAAIPEB_W_Z(
    CFileDialog* pThis, const wchar_t* p0) {
    (void)pThis;
    (void)p0;
    return 0;
}
// Symbol: ?OnTypeChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnTypeChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}
// Symbol: ?RemoveControlItem@CFileDialog@@QEAAJKK@Z
extern "C" HRESULT MS_ABI impl__RemoveControlItem_CFileDialog__QEAAJKK_Z(
    CFileDialog* pThis, unsigned long p0, unsigned long p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, false);
    if (!state) {
        return S_FALSE;
    }
    auto it = state->controlItems.find(p0);
    if (it == state->controlItems.end()) {
        return S_FALSE;
    }
    it->second.erase(p1);
    if (it->second.empty()) {
        state->controlItems.erase(it);
    }
    auto selected = state->selectedItem.find(p0);
    if (selected != state->selectedItem.end() && selected->second == p1) {
        selected->second = 0;
    }
    return S_OK;
}
// Symbol: ?SetCheckButtonState@CFileDialog@@QEAAJKH@Z
extern "C" HRESULT MS_ABI impl__SetCheckButtonState_CFileDialog__QEAAJKH_Z(
    CFileDialog* pThis, unsigned long p0, int p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->checkState[p0] = p1 ? TRUE : FALSE;
    return S_OK;
}
// Symbol: ?SetControlItemText@CFileDialog@@QEAAJKKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__SetControlItemText_CFileDialog__QEAAJKKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, unsigned long dwIDItem, const CString* text) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    CString itemText;
    SetTextOrEmpty(&itemText, text);
    state->controlItems[dwIDCtl][dwIDItem].text = itemText;
    return S_OK;
}
// Symbol: ?SetControlLabel@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__SetControlLabel_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* text) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->controlLabels[dwIDCtl], text);
    return S_OK;
}
// Symbol: ?SetEditBoxText@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__SetEditBoxText_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* text) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    SetTextOrEmpty(&state->editText[dwIDCtl], text);
    return S_OK;
}
// Symbol: ?SetControlItemState@CFileDialog@@QEAAJKKW4CDCONTROLSTATEF@@@Z
extern "C" HRESULT MS_ABI impl__SetControlItemState_CFileDialog__QEAAJKKW4CDCONTROLSTATEF___Z(
    CFileDialog* pThis, unsigned long p0, unsigned long p1, unsigned int p2) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->controlItems[p0][p1].state = p2;
    return S_OK;
}
// Symbol: ?SetControlState@CFileDialog@@QEAAJKW4CDCONTROLSTATEF@@@Z
extern "C" HRESULT MS_ABI impl__SetControlState_CFileDialog__QEAAJKW4CDCONTROLSTATEF___Z(
    CFileDialog* pThis, unsigned long p0, unsigned int p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->controlStates[p0] = p1;
    return S_OK;
}
// Symbol: ?SetControlText@CFileDialog@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetControlText_CFileDialog__QEAAXHPEB_W_Z(
    CFileDialog* pThis, int p0, const wchar_t* p1) {
    if (!pThis) return;
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (state) {
        state->controlLabels[static_cast<unsigned long>(p0)] = p1 ? p1 : L"";
    }
    if (pThis->GetSafeHwnd()) {
        ::SetDlgItemTextW(pThis->GetSafeHwnd(), p0, p1 ? p1 : L"");
    }
}
// Symbol: ?SetProperties@CFileDialog@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__SetProperties_CFileDialog__QEAAHPEB_W_Z(
    CFileDialog* pThis, const wchar_t* p0) {
    if (!pThis) return FALSE;
    if (!p0 || !p0[0]) {
        return FALSE;
    }
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return FALSE;
    state->properties = p0;
    return TRUE;
}
// Symbol: ?SetSelectedControlItem@CFileDialog@@QEAAJKK@Z
extern "C" HRESULT MS_ABI impl__SetSelectedControlItem_CFileDialog__QEAAJKK_Z(
    CFileDialog* pThis, unsigned long p0, unsigned long p1) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->selectedItem[p0] = p1;
    return S_OK;
}
// Symbol: ?SetTemplate@CFileDialog@@QEAAXPEB_W0@Z
extern "C" void MS_ABI impl__SetTemplate_CFileDialog__QEAAXPEB_W0_Z(
    CFileDialog* pThis, const wchar_t* p0, const wchar_t* p1) {
    (void)pThis;
    (void)p0;
    (void)p1;
}
// Symbol: ?StartVisualGroup@CFileDialog@@QEAAJKAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" HRESULT MS_ABI impl__StartVisualGroup_CFileDialog__QEAAJKAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    CFileDialog* pThis, unsigned long dwIDCtl, const CString* label) {
    CFileDialogControlState* state = GetFileDialogState(pThis, true);
    if (!state) return E_OUTOFMEMORY;
    state->visualGroupActive = TRUE;
    state->activeVisualGroup = dwIDCtl;
    SetTextOrEmpty(&state->controlLabels[dwIDCtl], label);
    return S_OK;
}
// Symbol: ?UpdateOFNFromShellDialog@CFileDialog@@UEAAXXZ
extern "C" void MS_ABI impl__UpdateOFNFromShellDialog_CFileDialog__UEAAXXZ(CFileDialog* pThis) {
    (void)pThis;
}
CFileDialog::CFileDialog(int bOpenFileDialog,
                         const wchar_t* lpszDefExt,
                         const wchar_t* lpszFileName,
                         unsigned long dwFlags,
                         const wchar_t* lpszFilter,
                         CWnd* pParentWnd,
                         unsigned long dwSize,
                         int bVistaStyle)
    : CDialog(), m_bOpenFileDialog(bOpenFileDialog), m_dwFlags(dwFlags), m_pFileList(nullptr) {
    (void)dwSize;
    (void)bVistaStyle;

    CFileDialogControlState* state = GetFileDialogState(this, true);
    if (state) {
        state->owner = pParentWnd ? pParentWnd->GetSafeHwnd() : nullptr;
    }

    if (lpszDefExt != nullptr) {
        m_strDefExt = lpszDefExt;
    }
    if (lpszFileName != nullptr) {
        m_strFileName = lpszFileName;
    }
    if (lpszFilter != nullptr) {
        m_strFilter = lpszFilter;
    }
    memset(_filedialog_padding, 0, sizeof(_filedialog_padding));
}
CFileDialog::~CFileDialog() {
    EraseFileDialogState(this);
}
void CFileDialog::SetDefExt(const wchar_t* lpszDefExt) {
    m_strDefExt = lpszDefExt;
}
// Symbol: ??0CFileDialog@@QEAA@HPEB_W0K0PEAVCWnd@@KH@Z
extern "C" void* MS_ABI impl___0CFileDialog__QEAA_HPEB_W0K0PEAVCWnd__KH_Z(
        void* p, int bOpenFileDialog, const wchar_t* lpszDefExt,
        const wchar_t* lpszFileName, unsigned long dwFlags,
        const wchar_t* lpszFilter, CWnd* pParentWnd, unsigned long dwSize,
        int bVistaStyle) {
    return new (p) CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags,
                               lpszFilter, pParentWnd, dwSize, bVistaStyle);
}
