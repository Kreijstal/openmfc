#include "DlgcommonSupport.h"

IMPLEMENT_DYNAMIC(CColorDialog, CDialog)
IMPLEMENT_DYNAMIC(CFontDialog, CDialog)
IMPLEMENT_DYNAMIC(CPrintDialog, CDialog)
IMPLEMENT_DYNAMIC(CPageSetupDialog, CDialog)
IMPLEMENT_DYNAMIC(CFindReplaceDialog, CDialog)
IMPLEMENT_DYNAMIC(CFileDialog, CDialog)
IMPLEMENT_DYNAMIC(CPrintDialogEx, CDialog)
IMPLEMENT_DYNAMIC(CVSListBoxBase, CStatic)
IMPLEMENT_DYNAMIC(CVSListBoxEditCtrl, CEdit)
IMPLEMENT_DYNAMIC(CVSListBox, CListBox)
namespace openmfc { namespace detail { namespace dlgcommon {
std::unordered_map<const CFileDialog*, CFileDialogControlState> g_fileDialogStates;
CFileDialogControlState* GetFileDialogState(const CFileDialog* pThis, bool createIfMissing) {
    if (!pThis) {
        return nullptr;
    }

    auto it = g_fileDialogStates.find(pThis);
    if (it == g_fileDialogStates.end()) {
        if (!createIfMissing) {
            return nullptr;
        }
        it = g_fileDialogStates.emplace(pThis, CFileDialogControlState()).first;
    }

    return &it->second;
}
void EraseFileDialogState(const CFileDialog* pThis) {
    g_fileDialogStates.erase(pThis);
}
void SetTextOrEmpty(CString* dst, const CString* src) {
    if (!dst) {
        return;
    }
    if (src) {
        *dst = *src;
    } else {
        dst->Empty();
    }
}
OPENFILENAMEW& OpenMfcGetOFNSnapshot(const CFileDialog* pThis) {
    thread_local OPENFILENAMEW ofn;
    thread_local wchar_t fileBuffer[65536];

    memset(&ofn, 0, sizeof(ofn));
    memset(fileBuffer, 0, sizeof(fileBuffer));

    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = fileBuffer;
    ofn.nMaxFile = static_cast<DWORD>(sizeof(fileBuffer) / sizeof(fileBuffer[0]));
    ofn.nFilterIndex = 1;

    if (pThis == nullptr) {
        ofn.lpstrFilter = L"All Files\0*.*\0";
        return ofn;
    }

    const auto* access = static_cast<const CFileDialogAccess*>(pThis);
    const CString& selectedPath = access->m_strPathName.IsEmpty() ? access->m_strFileName : access->m_strPathName;
    if (!selectedPath.IsEmpty()) {
        wcsncpy(fileBuffer, static_cast<const wchar_t*>(selectedPath), ofn.nMaxFile - 1);
    }

    ofn.lpstrFilter = access->m_strFilter.IsEmpty() ? L"All Files\0*.*\0" : static_cast<const wchar_t*>(access->m_strFilter);
    ofn.lpstrDefExt = access->m_strDefExt.IsEmpty() ? nullptr : static_cast<const wchar_t*>(access->m_strDefExt);
    ofn.lpstrInitialDir =
        access->m_strInitialDir.IsEmpty() ? nullptr : static_cast<const wchar_t*>(access->m_strInitialDir);
    ofn.Flags = access->m_dwFlags | OFN_EXPLORER;

    return ofn;
}
extern "C" intptr_t MS_ABI impl__DoModal_CFileDialog__UEAA_JXZ(CFileDialog* pThis) {
    return pThis ? pThis->DoModal() : IDCANCEL;
}
extern "C" void MS_ABI impl__GetPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetPathName());
}
extern "C" void MS_ABI impl__GetFileName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFileName());
}
extern "C" void MS_ABI impl__GetFileExt_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFileExt());
}
extern "C" void MS_ABI impl__GetFileTitle_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFileTitle());
}
extern "C" void MS_ABI impl__GetFolderPath_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis) {
    new(__ret) CString(pThis->GetFolderPath());
}
extern "C" void MS_ABI impl__GetNextPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAU__POSITION___Z(
    CString* __ret, const CFileDialog* pThis, void** p0) {
    new(__ret) CString(pThis->GetNextPathName(*p0));
}
COLORREF s_acrCustClr[16] = {
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
    RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255)
};
extern "C" void MS_ABI impl__GetDeviceName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDeviceName());
}
extern "C" void MS_ABI impl__GetDriverName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDriverName());
}
extern "C" void MS_ABI impl__GetPortName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetPortName());
}
extern "C" intptr_t MS_ABI impl__DoModal_CPageSetupDialog__UEAA_JXZ(CPageSetupDialog* pThis) {
    return pThis ? pThis->DoModal() : IDCANCEL;
}
extern "C" void MS_ABI impl__GetDeviceName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDeviceName());
}
extern "C" void MS_ABI impl__GetDriverName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetDriverName());
}
extern "C" void MS_ABI impl__GetPortName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret) {
    new(__ret) CString(pThis->GetPortName());
}
UINT s_nFindReplaceMsg = 0;
extern "C" int MS_ABI impl__Create_CFindReplaceDialog__UEAAHHPEB_W0KPEAVCWnd___Z(
    CFindReplaceDialog* pThis,
    int bFindDialogOnly,
    const wchar_t* lpszFindWhat,
    const wchar_t* lpszReplaceWith,
    unsigned long dwFlags,
    CWnd* pParentWnd) {
    return pThis->Create(bFindDialogOnly, lpszFindWhat, lpszReplaceWith, dwFlags, pParentWnd);
}
} } }  // namespace openmfc::detail::dlgcommon
