#pragma once
// Shared internals of the former dlgcommon.cpp translation unit.
// Definitions live in detail/DlgcommonSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace dlgcommon {} } }
using namespace openmfc::detail::dlgcommon;
// dlgcommon.cpp - Common Dialog implementations (REAL, not stubs)
//
// This file implements:
// - CFileDialog (GetOpenFileName/GetSaveFileName)
// - CColorDialog (ChooseColor)
// - CFontDialog (ChooseFont)
// - CPrintDialog (PrintDlg)
// - CPageSetupDialog (PageSetupDlg)
// - CFindReplaceDialog (FindText/ReplaceText)

// Skip inline implementations from afxwin.h
#define OPENMFC_APPCORE_IMPL

#include <openmfc/afxmfc.h>
#include <windows.h>
#include <commdlg.h>
#include <cstring>
#include <map>
#include <new>
#include <cstdint>
#include <unordered_map>
#include <vector>

// MS ABI for x64 Windows
#ifdef __GNUC__
#define MS_ABI __attribute__((ms_abi))
#else
#define MS_ABI
#endif

//=============================================================================
// Runtime class implementations
//=============================================================================


#ifdef __GNUC__
asm(".globl \"?classCFileDialog@CFileDialog@@2UCRuntimeClass@@A\"\n"
    ".set \"?classCFileDialog@CFileDialog@@2UCRuntimeClass@@A\", _ZN11CFileDialog16classCFileDialogE\n");
#endif

//=============================================================================
// CFileDialog implementation
//=============================================================================

namespace openmfc { namespace detail { namespace dlgcommon {
struct CFileDialogAccess : CFileDialog {
    using CFileDialog::m_bOpenFileDialog;
    using CFileDialog::m_dwFlags;
    using CFileDialog::m_strDefExt;
    using CFileDialog::m_strFileName;
    using CFileDialog::m_strFileNameOnly;
    using CFileDialog::m_strFilter;
    using CFileDialog::m_strFolderPath;
    using CFileDialog::m_strInitialDir;
    using CFileDialog::m_strPathName;
};
} } }

namespace openmfc { namespace detail { namespace dlgcommon {
struct CFileDialogControlState {
    HWND owner = nullptr;
    std::map<unsigned long, CString> controlLabels;
    std::map<unsigned long, int> checkState;
    std::map<unsigned long, CString> editText;
    std::map<unsigned long, unsigned int> controlStates;
    std::map<unsigned long, unsigned long> selectedItem;

    struct ItemState {
        CString text;
        unsigned int state = 0;
    };
    std::map<unsigned long, std::map<unsigned long, ItemState>> controlItems;

    std::vector<CString> selectedPaths;

    bool openDropDownEnabled = false;
    bool visualGroupActive = false;
    unsigned long activeVisualGroup = 0;
    CString properties;
};
} } }

namespace openmfc { namespace detail { namespace dlgcommon {
extern std::unordered_map<const CFileDialog*, CFileDialogControlState> g_fileDialogStates;
} } }

namespace openmfc { namespace detail { namespace dlgcommon {
CFileDialogControlState* GetFileDialogState(const CFileDialog* pThis, bool createIfMissing);
} } }

namespace openmfc { namespace detail { namespace dlgcommon {
void EraseFileDialogState(const CFileDialog* pThis);
} } }

namespace openmfc { namespace detail { namespace dlgcommon {
void SetTextOrEmpty(CString* dst, const CString* src);
} } }

namespace openmfc { namespace detail { namespace dlgcommon {
OPENFILENAMEW& OpenMfcGetOFNSnapshot(const CFileDialog* pThis);
} } }





namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" intptr_t MS_ABI impl__DoModal_CFileDialog__UEAA_JXZ(CFileDialog* pThis);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetFileName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetFileExt_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetFileTitle_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetFolderPath_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    CString* __ret, const CFileDialog* pThis);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetNextPathName_CFileDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAPEAU__POSITION___Z(
    CString* __ret, const CFileDialog* pThis, void** p0);
} } }



























































//=============================================================================
// CColorDialog implementation
//=============================================================================

// Static custom colors storage (shared across all instances)
namespace openmfc { namespace detail { namespace dlgcommon {
extern COLORREF s_acrCustClr[16];
} } }






//=============================================================================
// CFontDialog implementation
//=============================================================================
















//=============================================================================
// CPrintDialog implementation
//=============================================================================

namespace openmfc { namespace detail { namespace dlgcommon {
struct CPrintDialogAccess : CPrintDialog {
    using CPrintDialog::m_bPrintSetupOnly;
    using CPrintDialog::m_dwFlags;
    using CPrintDialog::m_hDC;
    using CPrintDialog::m_hDevMode;
    using CPrintDialog::m_hDevNames;
    using CPrintDialog::m_nCopies;
    using CPrintDialog::m_nFromPage;
    using CPrintDialog::m_nToPage;
};
} } }










namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetDeviceName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetDriverName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetPortName_CPrintDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPrintDialog* pThis, CString* __ret);
} } }












//=============================================================================
// CPageSetupDialog implementation
//=============================================================================

namespace openmfc { namespace detail { namespace dlgcommon {
struct CPageSetupDialogAccess : CPageSetupDialog {
    using CPageSetupDialog::m_hDevMode;
    using CPageSetupDialog::m_hDevNames;
    using CPageSetupDialog::m_rtMargin;
};
} } }




namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" intptr_t MS_ABI impl__DoModal_CPageSetupDialog__UEAA_JXZ(CPageSetupDialog* pThis);
} } }



namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetDeviceName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetDriverName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret);
} } }


namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" void MS_ABI impl__GetPortName_CPageSetupDialog__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    const CPageSetupDialog* pThis, CString* __ret);
} } }










//=============================================================================
// CFindReplaceDialog implementation
//=============================================================================

// Registered message for find/replace notifications
namespace openmfc { namespace detail { namespace dlgcommon {
extern UINT s_nFindReplaceMsg;
} } }




namespace openmfc { namespace detail { namespace dlgcommon {
extern "C" int MS_ABI impl__Create_CFindReplaceDialog__UEAAHHPEB_W0KPEAVCWnd___Z(
    CFindReplaceDialog* pThis,
    int bFindDialogOnly,
    const wchar_t* lpszFindWhat,
    const wchar_t* lpszReplaceWith,
    unsigned long dwFlags,
    CWnd* pParentWnd);
} } }












//=============================================================================
// CPrintDialogEx
//=============================================================================
















//=============================================================================
// CTaskDialog overloads and wrappers
//=============================================================================






























//=============================================================================
// VS list box wrappers
//=============================================================================











































































