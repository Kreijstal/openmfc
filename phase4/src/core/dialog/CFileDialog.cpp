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

// ---------------------------------------------------------------------------
// Retail layout note (read from the mfc140u.dll ctor, RVA 0x20a1c0, and the
// Vista-style wrappers below).  The retail CFileDialog keeps, after the CDialog
// base (0x130 bytes):
//   0x130 OPENFILENAMEW* m_pOFN            (malloc'd, dwSize or 0x98)
//   0x138 BOOL  m_bVistaStyle              (GetVersionExW major >= 6 ? bVistaStyle : 0;
//                                            reset to 0 if CoInitializeEx/CoCreateInstance fail)
//   0x13c BOOL  m_bPickFoldersMode
//   0x140 BOOL  m_bPickNonFileSysFoldersMode
//   0x144 DWORD m_dwCookie                  (IFileDialog::Advise cookie)
//   0x148 IFileDialog*          m_pIFileDialog
//   0x150 IFileDialogCustomize* m_pIFileDialogCustomize
//   0x158 BOOL  m_bOpenFileDialog
//   0x160 CString m_strFilter
//   0x168 WCHAR m_szFileTitle[256]
//   0x368 WCHAR m_szFileName[260]
//   0x570 OPENFILENAMEW* m_pofnTemp
//   0x578 BOOL  m_bFileTypesSet
//   0x580 XFileDialogEvents        (vptr; IFileDialogEvents interface part)
//   0x588 XFileDialogControlEvents (vptr; IFileDialogControlEvents part)
//   sizeof == 0x590 (1424)
// The clean-room class in include/openmfc/afxwin.h has the same total size but
// a different member set (m_bOpenFileDialog, m_dwFlags, nine CStrings, m_pFileList,
// 1032 bytes of padding) and NO m_pOFN / m_bVistaStyle / COM pointers / interface
// parts.  Every export below that retail routes through m_pIFileDialog or
// m_pIFileDialogCustomize therefore stays a documented stub until the header
// grows those members; the retail control flow is recorded above each one.
// ---------------------------------------------------------------------------

// Thunks used by the bodies below (definitions seen in core/window/CWnd.cpp:590
// and core/window/Thunks.cpp:1148).
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" void MS_ABI impl__CenterWindow_CWnd__QEAAXPEAV1__Z(CWnd* pThis, void* pAlternateOwner);

// Opaque COM interface types for the nested-class event sinks.  Only pointers
// are passed through, so forward declarations suffice (shobjidl.h is not
// included by this TU).
struct IFileDialog;
struct IFileDialogCustomize;
struct IShellItem;

















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
// Retail (RVA 0x20b840, mfc140u): if m_bVistaStyle (+0x138) is zero the call is
// a no-op; otherwise m_pIFileDialog (+0x148)->AddPlace(psi, fdap) (vtable slot
// 0xa8/8 = 21) and a failed HRESULT throws AfxThrowInvalidArgException.
// STUB: the clean-room object has neither m_bVistaStyle nor m_pIFileDialog, so
// only the non-Vista no-op path can be honoured here.
extern "C" void MS_ABI impl__AddPlace_CFileDialog__QEAAXPEAUIShellItem__W4FDAP___Z(
    CFileDialog* pThis, IShellItem* psi, int fdap) {
    (void)pThis;
    (void)psi;
    (void)fdap;
}
// Symbol: ?AddPlace@CFileDialog@@QEAAXPEB_WW4FDAP@@@Z
// Retail (RVA 0x20b7c0, mfc140u): returns immediately when m_bVistaStyle (+0x138)
// is zero.  Otherwise calls the internal helper at 0x1cab08 (not an export: it
// GetProcAddress()es shell32!SHCreateItemFromParsingName and returns E_FAIL when
// that is unavailable) as SHCreateItemFromParsingName(lpszFolder, NULL,
// IID_IShellItem (at 0x2d9958), &psi), throws AfxThrowInvalidArgException on a
// failed HRESULT, then calls the IShellItem* overload (0x20b840) and Release()s
// the item (slot 2) when non-null.
// STUB: no m_bVistaStyle / m_pIFileDialog in the clean-room object; only the
// non-Vista no-op path can be honoured here.
extern "C" void MS_ABI impl__AddPlace_CFileDialog__QEAAXPEB_WW4FDAP___Z(
    CFileDialog* pThis, const wchar_t* lpszFolder, int fdap) {
    (void)pThis;
    (void)lpszFolder;
    (void)fdap;
}
// Symbol: ?ApplyOFNToShellDialog@CFileDialog@@QEAAXXZ
// Retail (RVA 0x20af70, mfc140u; ~0x770 bytes): returns immediately unless
// m_bVistaStyle (+0x138) == 1.  Then pushes m_pOFN (+0x130) into
// m_pIFileDialog (+0x148): lpstrTitle -> SetTitle (slot 0x88/8 = 17),
// lpstrDefExt -> SetDefaultExtension (slot 0xb0/8 = 22); walks the
// double-NUL-terminated lpstrFilter counting name/spec pairs, builds a heap
// COMDLG_FILTERSPEC array (16 bytes/entry; the array and each copied string
// come from the exported operator new, ??2@YAPEAX_K@Z at RVA 0x27f0, not
// new[]), passes it to SetFileTypes (slot 0x20/8 = 4) only while
// m_bFileTypesSet (+0x578) is 0 and then sets that flag, free()s every string
// and the array through the CRT free import, and calls
// SetFileTypeIndex (slot 5) with max(1, nFilterIndex); it then continues with
// lpstrFile (+0x30) and the remaining OFN fields (that tail was not read in
// full).  Failed HRESULTs throw AfxThrowInvalidArgException; a failed
// allocation throws AfxThrowMemoryException.
// STUB: needs m_bVistaStyle, m_pOFN and m_pIFileDialog, none of which exist in
// the clean-room object (the clean-room OFN is a per-call snapshot, see
// OpenMfcGetOFNSnapshot).
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
// Retail (RVA 0x20b780, mfc140u): result = NULL; if m_bVistaStyle (+0x138) == 1,
// m_pIFileDialog (+0x148)->QueryInterface(IID at 0x34c608, &result); the HRESULT
// is ignored; returns result (the caller owns the AddRef).
// STUB: no m_bVistaStyle / m_pIFileDialog in the clean-room object, so this can
// only return the non-Vista NULL.
extern "C" void* MS_ABI impl__GetIFileDialogCustomize_CFileDialog__QEAAPEAUIFileDialogCustomize__XZ(
    CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetIFileOpenDialog@CFileDialog@@QEAAPEAUIFileOpenDialog@@XZ
// Retail (RVA 0x20b700, mfc140u): same shape as GetIFileDialogCustomize with the
// IID at 0x34c5f8 (IID_IFileOpenDialog): NULL unless m_bVistaStyle == 1, else
// m_pIFileDialog->QueryInterface(..., &result), HRESULT ignored.
// STUB: no m_bVistaStyle / m_pIFileDialog in the clean-room object.
extern "C" void* MS_ABI impl__GetIFileOpenDialog_CFileDialog__QEAAPEAUIFileOpenDialog__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetIFileSaveDialog@CFileDialog@@QEAAPEAUIFileSaveDialog@@XZ
// Retail (RVA 0x20b740, mfc140u): same shape as GetIFileDialogCustomize with the
// IID at 0x34c5d8 (IID_IFileSaveDialog): NULL unless m_bVistaStyle == 1, else
// m_pIFileDialog->QueryInterface(..., &result), HRESULT ignored.
// STUB: no m_bVistaStyle / m_pIFileDialog in the clean-room object.
extern "C" void* MS_ABI impl__GetIFileSaveDialog_CFileDialog__QEAAPEAUIFileSaveDialog__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetResult@CFileDialog@@QEAAPEAUIShellItem@@XZ
// Retail (RVA 0x20c220, mfc140u): returns NULL when m_bVistaStyle (+0x138) is 0.
// Otherwise m_pIFileDialog (+0x148)->GetResult(&psi) (slot 0xa0/8 = 20); if that
// fails it falls back to the late-bound shell32!SHCreateItemFromParsingName
// helper at 0x1cab08 with GetPathName() (0x20c350), NULL, IID_IShellItem, &psi
// and throws AfxThrowInvalidArgException if that fails too; returns psi.
// STUB: no m_bVistaStyle / m_pIFileDialog in the clean-room object; only the
// non-Vista NULL can be returned here.
extern "C" void* MS_ABI impl__GetResult_CFileDialog__QEAAPEAUIShellItem__XZ(CFileDialog* pThis) {
    (void)pThis;
    return nullptr;
}
// Symbol: ?GetResults@CFileDialog@@QEAAPEAUIShellItemArray@@XZ
// Retail (RVA 0x20c2c0, mfc140u): returns NULL when m_bVistaStyle (+0x138) is 0.
// Otherwise m_pIFileDialog->QueryInterface(IID at 0x34c5f8 = IID_IFileOpenDialog,
// &pfod), pfod->GetResults(&psia) (slot 0xd8/8 = 27), pfod->Release(); any
// failed HRESULT throws AfxThrowInvalidArgException; returns psia.
// STUB: no m_bVistaStyle / m_pIFileDialog in the clean-room object.
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
// Retail (RVA 0x20e2a0, mfc140u): if m_bVistaStyle (+0x138) is 0 returns
// E_NOTIMPL (0x80004001).  Otherwise p = GetIFileDialogCustomize() (0x20b780),
// hr = p->MakeProminent(dwIDCtl) (slot 0xe0/8 = 28), p->Release(), return hr.
// Note retail does not null-check p on the Vista path; on that path the result
// is whatever IFileDialogCustomize::MakeProminent returned (S_OK on success).
// STUB: the clean-room object never owns an IFileDialogCustomize.  The rest of
// the customization family in this file (AddPushButton, AddCheckButton, ...)
// emulates the Vista-style path through the CFileDialogControlState side table
// and reports S_OK, so this returns S_OK for consistency with those siblings;
// nothing is actually made prominent.  (Retail's non-Vista result would be
// E_NOTIMPL.)
extern "C" HRESULT MS_ABI impl__MakeProminent_CFileDialog__QEAAJK_Z(CFileDialog* pThis, unsigned long dwIDCtl) {
    (void)pThis;
    (void)dwIDCtl;
    return S_OK;
}
// The eight overridables below share one retail body: export RVA 0x27d0 in
// mfc140u (resolved through the export ordinal table; COMDAT-folded onto
// CFrameWndEx::AddDockSite) which is a bare `ret`.  Retail does nothing in the
// base class; only application overrides add behaviour.  Empty by design.
// Symbol: ?OnButtonClicked@CFileDialog@@MEAAXK@Z
extern "C" void MS_ABI impl__OnButtonClicked_CFileDialog__MEAAXK_Z(CFileDialog* pThis, unsigned long dwIDCtl) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
    (void)dwIDCtl;
}
// Symbol: ?OnCheckButtonToggled@CFileDialog@@MEAAXKH@Z
extern "C" void MS_ABI impl__OnCheckButtonToggled_CFileDialog__MEAAXKH_Z(
    CFileDialog* pThis, unsigned long dwIDCtl, int bChecked) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
    (void)dwIDCtl;
    (void)bChecked;
}
// Symbol: ?OnControlActivating@CFileDialog@@MEAAXK@Z
extern "C" void MS_ABI impl__OnControlActivating_CFileDialog__MEAAXK_Z(
    CFileDialog* pThis, unsigned long dwIDCtl) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
    (void)dwIDCtl;
}
// Symbol: ?OnFileNameChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnFileNameChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
}
// Symbol: ?OnFileNameOK@CFileDialog@@MEAAHXZ
// Retail RVA 0x71e0 (mfc140u, COMDAT-folded onto COleUILinkInfo::AddRef):
// `xor eax,eax; ret` -- the base class accepts every file name (FALSE).
extern "C" int MS_ABI impl__OnFileNameOK_CFileDialog__MEAAHXZ(CFileDialog* pThis) {
    (void)pThis;
    return FALSE;
}
// Symbol: ?OnFolderChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnFolderChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
}
// Symbol: ?OnInitDone@CFileDialog@@MEAAXXZ
// Retail (RVA 0x20d540, mfc140u):
//     CWnd::FromHandle(::GetParent(m_hWnd))->CenterWindow(NULL);
// (GetParent via the IAT, FromHandle at 0x28ad70, tail-jump to CenterWindow at
// 0x291180 with rdx = 0).  m_hWnd is the explorer-style hook child; its parent
// is the actual file dialog, which gets centred over its owner.
// Deviations: retail tests neither `this` nor the FromHandle result
// (CHandleMap::FromHandle at 0x2a6080 returns NULL for a NULL handle, and
// CenterWindow would then fault reading m_hWnd); we return early in both cases.
extern "C" void MS_ABI impl__OnInitDone_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    if (!pThis) return;
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pThis->m_hWnd));
    if (pParent) {
        impl__CenterWindow_CWnd__QEAAXPEAV1__Z(pParent, nullptr);
    }
}
// Symbol: ?OnItemSelected@CFileDialog@@MEAAXKK@Z
extern "C" void MS_ABI impl__OnItemSelected_CFileDialog__MEAAXKK_Z(
    CFileDialog* pThis, unsigned long dwIDCtl, unsigned long dwIDItem) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
    (void)dwIDCtl;
    (void)dwIDItem;
}
// Symbol: ?OnLBSelChangedNotify@CFileDialog@@MEAAXIII@Z
extern "C" void MS_ABI impl__OnLBSelChangedNotify_CFileDialog__MEAAXIII_Z(
    CFileDialog* pThis, unsigned int nIDBox, unsigned int iCurSel, unsigned int nCode) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
    (void)pThis;
    (void)nIDBox;
    (void)iCurSel;
    (void)nCode;
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
// Retail RVA 0x71e0 (mfc140u): `xor eax,eax; ret` == OFN_SHAREWARN (0), i.e. let
// the common dialog show its own sharing-violation warning.
extern "C" unsigned int MS_ABI impl__OnShareViolation_CFileDialog__MEAAIPEB_W_Z(
    CFileDialog* pThis, const wchar_t* lpszPathName) {
    (void)pThis;
    (void)lpszPathName;
    return OFN_SHAREWARN;
}
// Symbol: ?OnTypeChange@CFileDialog@@MEAAXXZ
extern "C" void MS_ABI impl__OnTypeChange_CFileDialog__MEAAXXZ(CFileDialog* pThis) {
    // Retail RVA 0x27d0 (mfc140u): `ret`.
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
// Retail (RVA 0x20d140, mfc140u):
//     if (m_bVistaStyle == 1) AfxThrowNotSupportedException();
//     m_pOFN->lpTemplateName = (m_pOFN->Flags & OFN_EXPLORER) ? lpWin4ID : lpWin3ID;
//     m_pOFN->Flags |= OFN_ENABLETEMPLATE;
// (+0x138 m_bVistaStyle, +0x130 m_pOFN, OFN +0x60 Flags, +0x80 lpTemplateName.)
// STUB: the clean-room object has no persistent OPENFILENAMEW (GetOFN() hands
// out a per-call snapshot) and no m_bVistaStyle, so lpTemplateName cannot be
// stored; setting OFN_ENABLETEMPLATE alone would make GetOpenFileName fail, so
// nothing is done.
extern "C" void MS_ABI impl__SetTemplate_CFileDialog__QEAAXPEB_W0_Z(
    CFileDialog* pThis, const wchar_t* lpWin3ID, const wchar_t* lpWin4ID) {
    (void)pThis;
    (void)lpWin3ID;
    (void)lpWin4ID;
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
// Retail (RVA 0x20a680, mfc140u; ~0x900 bytes): returns immediately unless
// m_bVistaStyle (+0x138) == 1; then m_pIFileDialog (+0x148)->GetResult(&psi)
// (slot 0xa0/8 = 20).  On success: if GetIFileSaveDialog() (0x20b740) is
// non-null, GetProperties (slot 0xf0/8 = 30) + ApplyProperties(psi, store,
// m_hWnd, NULL) (slot 0xf8/8 = 31) and both are Release()d; then
// psi->GetDisplayName(SIGDN_FILESYSPATH = 0x80058000, &pszPath) and the path
// is copied back into m_pOFN.  On GetResult FAILURE it is not an error: if
// m_pOFN->Flags (+0x60) has OFN_ALLOWMULTISELECT (0x200) it QIs
// IID_IFileOpenDialog (0x34c5f8), GetResults (slot 0xd8/8 = 27),
// IShellItemArray::EnumItems (slot 0x48/8 = 9) and walks IEnumShellItems::Next
// (slot 3) to build the multi-selection.  (The copy-back tails were not read
// instruction by instruction.)
// STUB: needs m_bVistaStyle, m_pIFileDialog and m_pOFN, none present in the
// clean-room object.
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

// ---------------------------------------------------------------------------
// Nested interface parts: CFileDialog::XFileDialogEvents (IFileDialogEvents) and
// CFileDialog::XFileDialogControlEvents (IFileDialogControlEvents).
//
// In retail these are the two interface-part sub-objects that the ctor
// (RVA 0x20a1c0, mfc140u) installs at +0x580 / +0x588 of CFileDialog and hands
// to IFileDialog::Advise.  Every method starts with METHOD_PROLOGUE:
//     CFileDialog* pThis = (CFileDialog*)((char*)this - 0x580 /* or 0x588 */);
//     AFX_MAINTAIN_STATE2 state(pThis->m_pModuleState /* CCmdTarget +0x38 */);
// and then dispatches to a CFileDialog virtual by vtable slot:
//     0x328/8 = 101 UpdateOFNFromShellDialog   0x330/8 = 102 OnShareViolation
//     0x338/8 = 103 OnFileNameOK               0x350/8 = 106 OnFileNameChange
//     0x358/8 = 107 OnFolderChange             0x360/8 = 108 OnTypeChange
//     0x368/8 = 109 OnItemSelected             0x370/8 = 110 OnButtonClicked
//     0x378/8 = 111 OnCheckButtonToggled       0x380/8 = 112 OnControlActivating
// The clean-room CFileDialog has neither the two sub-objects nor those vtable
// slots (afxwin.h declares none of the overridables as virtual), so there is no
// pointer a caller could legitimately pass as `this` here and no outer object to
// recover from it.  The methods that dispatch to the outer object therefore stay
// stubs (signatures corrected from the mangled names); the three whose retail
// body is only the module-state guard plus `return S_OK` are complete as written.
// ---------------------------------------------------------------------------

// Symbol: ?OnButtonClicked@XFileDialogControlEvents@CFileDialog@@UEAAJPEAUIFileDialogCustomize@@K@Z
// Retail (RVA 0x20bf70, mfc140u): METHOD_PROLOGUE(-0x588); pThis->OnButtonClicked(dwIDCtl)
// via slot 110; return S_OK.  STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnButtonClicked_XFileDialogControlEvents_CFileDialog__UEAAJPEAUIFileDialogCustomize__K_Z(
    void* pInner, IFileDialogCustomize* pfdc, unsigned long dwIDCtl) {
    (void)pInner;
    (void)pfdc;
    (void)dwIDCtl;
    return S_OK;
}

// Symbol: ?OnCheckButtonToggled@XFileDialogControlEvents@CFileDialog@@UEAAJPEAUIFileDialogCustomize@@KH@Z
// Retail (RVA 0x20bfd0, mfc140u): METHOD_PROLOGUE(-0x588);
// pThis->OnCheckButtonToggled(dwIDCtl, bChecked) via slot 111; return S_OK.
// STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnCheckButtonToggled_XFileDialogControlEvents_CFileDialog__UEAAJPEAUIFileDialogCustomize__KH_Z(
    void* pInner, IFileDialogCustomize* pfdc, unsigned long dwIDCtl, int bChecked) {
    (void)pInner;
    (void)pfdc;
    (void)dwIDCtl;
    (void)bChecked;
    return S_OK;
}

// Symbol: ?OnControlActivating@XFileDialogControlEvents@CFileDialog@@UEAAJPEAUIFileDialogCustomize@@K@Z
// Retail (RVA 0x20c040, mfc140u): METHOD_PROLOGUE(-0x588); pThis->OnControlActivating(dwIDCtl)
// via slot 112; return S_OK.  STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnControlActivating_XFileDialogControlEvents_CFileDialog__UEAAJPEAUIFileDialogCustomize__K_Z(
    void* pInner, IFileDialogCustomize* pfdc, unsigned long dwIDCtl) {
    (void)pInner;
    (void)pfdc;
    (void)dwIDCtl;
    return S_OK;
}

// Symbol: ?OnFileOk@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@@Z
// Retail (RVA 0x20b9b0, mfc140u): METHOD_PROLOGUE(-0x580);
// pThis->UpdateOFNFromShellDialog() (slot 101); return pThis->OnFileNameOK()
// (slot 103) ? S_FALSE : S_OK.  STUB: no outer object / slots in the clean-room
// layout; S_OK is what the base-class OnFileNameOK (FALSE) yields.
extern "C" HRESULT MS_ABI impl__OnFileOk_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog___Z(
    void* pInner, IFileDialog* pfd) {
    (void)pInner;
    (void)pfd;
    return S_OK;
}

// Symbol: ?OnFolderChange@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@@Z
// Retail (RVA 0x20ba20, mfc140u): METHOD_PROLOGUE(-0x580); pThis->OnFolderChange()
// via slot 107; return S_OK.  STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnFolderChange_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog___Z(
    void* pInner, IFileDialog* pfd) {
    (void)pInner;
    (void)pfd;
    return S_OK;
}

// OnFolderChanging, OnHelp and OnOverwrite share one retail body (RVA 0x20ba70,
// mfc140u, reached through three export ordinals): read the outer object's
// m_pModuleState (rcx - 0x548 == (this - 0x580) + 0x38), construct and destroy
// an AFX_MAINTAIN_STATE2 around nothing, return S_OK.  The guard has no effect
// that outlives the call, so `return S_OK` is the complete behaviour.

// Symbol: ?OnFolderChanging@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@PEAUIShellItem@@@Z
extern "C" HRESULT MS_ABI impl__OnFolderChanging_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog__PEAUIShellItem___Z(
    void* pInner, IFileDialog* pfd, IShellItem* psiFolder) {
    // Retail RVA 0x20ba70 (mfc140u): module-state guard only; S_OK.
    (void)pInner;
    (void)pfd;
    (void)psiFolder;
    return S_OK;
}

// Symbol: ?OnHelp@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@@Z
extern "C" HRESULT MS_ABI impl__OnHelp_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog___Z(
    void* pInner, IFileDialog* pfd) {
    // Retail RVA 0x20ba70 (mfc140u): module-state guard only; S_OK.
    (void)pInner;
    (void)pfd;
    return S_OK;
}

// Symbol: ?OnItemSelected@XFileDialogControlEvents@CFileDialog@@UEAAJPEAUIFileDialogCustomize@@KK@Z
// Retail (RVA 0x20bf00, mfc140u): METHOD_PROLOGUE(-0x588);
// pThis->OnItemSelected(dwIDCtl, dwIDItem) via slot 109; return S_OK.
// STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnItemSelected_XFileDialogControlEvents_CFileDialog__UEAAJPEAUIFileDialogCustomize__KK_Z(
    void* pInner, IFileDialogCustomize* pfdc, unsigned long dwIDCtl, unsigned long dwIDItem) {
    (void)pInner;
    (void)pfdc;
    (void)dwIDCtl;
    (void)dwIDItem;
    return S_OK;
}

// Symbol: ?OnOverwrite@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@PEAUIShellItem@@PEAW4FDE_OVERWRITE_RESPONSE@@@Z
extern "C" HRESULT MS_ABI impl__OnOverwrite_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog__PEAUIShellItem__PEAW4FDE_OVERWRITE_RESPONSE___Z(
    void* pInner, IFileDialog* pfd, IShellItem* psi, int* pResponse) {
    // Retail RVA 0x20ba70 (mfc140u): module-state guard only; *pResponse is
    // never written (no store through r9 anywhere in the body); S_OK.
    (void)pInner;
    (void)pfd;
    (void)psi;
    (void)pResponse;
    return S_OK;
}

// Symbol: ?OnSelectionChange@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@@Z
// Retail (RVA 0x20baa0, mfc140u): METHOD_PROLOGUE(-0x580); pThis->OnFileNameChange()
// via slot 106; return S_OK.  STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnSelectionChange_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog___Z(
    void* pInner, IFileDialog* pfd) {
    (void)pInner;
    (void)pfd;
    return S_OK;
}

// Symbol: ?OnShareViolation@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@PEAUIShellItem@@PEAW4FDE_SHAREVIOLATION_RESPONSE@@@Z
// Retail (RVA 0x20bb70, mfc140u): METHOD_PROLOGUE(-0x580); if psi == NULL or
// psi->GetDisplayName(SIGDN_FILESYSPATH = 0x80058000, &pszPath) fails ->
// AfxThrowInvalidArgException; CString strPath(pszPath) (the inlined CString
// ctor, including its MAKEINTRESOURCE branch), CoTaskMemFree(pszPath);
// r = pThis->OnShareViolation(strPath) via slot 102; if pResponse == NULL ->
// AfxThrowInvalidArgException; if r is 0, 1 or 2 (FDESVR_DEFAULT / ACCEPT /
// REFUSE) *pResponse = r, any other value leaves *pResponse untouched; S_OK.
// STUB: no outer object / slot in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnShareViolation_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog__PEAUIShellItem__PEAW4FDE_SHAREVIOLATION_RESPONSE___Z(
    void* pInner, IFileDialog* pfd, IShellItem* psi, int* pResponse) {
    (void)pInner;
    (void)pfd;
    (void)psi;
    (void)pResponse;
    return S_OK;
}

// Symbol: ?OnTypeChange@XFileDialogEvents@CFileDialog@@UEAAJPEAUIFileDialog@@@Z
// Retail (RVA 0x20baf0, mfc140u): METHOD_PROLOGUE(-0x580);
// pThis->m_pIFileDialog->GetFileTypeIndex(&idx) (IFileDialog slot 6, idx
// preset to 0, HRESULT ignored); pThis->m_pOFN->nFilterIndex (+0x2c) = idx;
// pThis->OnTypeChange() via slot 108; return S_OK.
// STUB: needs the outer object, m_pIFileDialog and m_pOFN, none of which exist
// in the clean-room layout.
extern "C" HRESULT MS_ABI impl__OnTypeChange_XFileDialogEvents_CFileDialog__UEAAJPEAUIFileDialog___Z(
    void* pInner, IFileDialog* pfd) {
    (void)pInner;
    (void)pfd;
    return S_OK;
}
