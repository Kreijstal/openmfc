#pragma once
// Shared internals of the former dlgcore.cpp translation unit.
// Definitions live in detail/DlgcoreSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace dlgcore {} } }
using namespace openmfc::detail::dlgcore;
// Dialog Core Implementation
//
// Implements CDialog and dialog-related functionality.
// This enables dialog-based MFC applications.

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting with appcore.cpp
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include <windows.h>
#include <cstring>

// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// CDialog Runtime Class
// =============================================================================


// =============================================================================
// Dialog Procedure
// =============================================================================

// Forward declaration
INT_PTR CALLBACK AfxDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK PropPageDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Map HWND to CDialog* for dialog message routing
#include <map>
namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<HWND, CDialog*> g_dlgMap;
} } }
namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<CDialog*, const DLGTEMPLATE*> g_dlgIndirectTemplates;
} } }
namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<CDialog*, HWND> g_dlgParents;
} } }
namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<CPropertyPage*, PROPSHEETPAGEW> g_propertyPagePspMap;
} } }
namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<HWND, CPropertySheet*> g_propertySheetMap;
} } }
namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<CPropertySheet*, CString> g_propertySheetCaptions;
} } }
namespace openmfc { namespace detail { namespace dlgcore {
extern thread_local CPropertySheet* g_pendingPropertySheet;
} } }

namespace openmfc { namespace detail { namespace dlgcore {
const AFX_MSGMAP* EmptyMessageMap_Dlgcore();
} } }

namespace openmfc { namespace detail { namespace dlgcore {
struct CDialogExAccess : CDialogEx {
    using CDialogEx::m_clrBackground;
    using CDialogEx::m_hBackgroundImage;
};
} } }

namespace openmfc { namespace detail { namespace dlgcore {
struct CPropertyPageAccess : CPropertyPage {
    using CPropertyPage::_propertypage_padding;
};
} } }

namespace openmfc { namespace detail { namespace dlgcore {
struct CPropertySheetAccess : CPropertySheet {
    using CPropertySheet::_propertysheet_padding;
};
} } }

namespace openmfc { namespace detail { namespace dlgcore {
struct CDialogExBackgroundState {
    int location = 0;
    bool ownsBitmap = false;
    HBRUSH brush = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace dlgcore {
extern std::map<CDialogEx*, CDialogExBackgroundState> g_dialogExBackgroundState;
} } }

namespace openmfc { namespace detail { namespace dlgcore {
CDialogExAccess* DialogExAccess(CDialogEx* pThis);
} } }

namespace openmfc { namespace detail { namespace dlgcore {
CPropertyPageAccess* PropertyPageAccess(CPropertyPage* pThis);
} } }

namespace openmfc { namespace detail { namespace dlgcore {
CPropertySheetAccess* PropertySheetAccess(CPropertySheet* pThis);
} } }

extern "C" void MS_ABI impl__CommonConstruct_CDialogEx__IEAAXXZ(CDialogEx* pThis);

// =============================================================================
// CDialog Constructors
// =============================================================================

// Default constructor
// Ordinal: 448
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void* MS_ABI impl___0CDialog__QEAA_XZ(CDialog* pThis);
} } }

// Constructor with resource ID
// Ordinal: 446
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void* MS_ABI impl___0CDialog__QEAA_IPEAVCWnd___Z(
    CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd);
} } }

// Constructor with template name
// Ordinal: 447
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void* MS_ABI impl___0CDialog__QEAA_PEB_WPEAVCWnd___Z(
    CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd);
} } }

// Destructor
// Ordinal: 1089
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void MS_ABI impl___1CDialog__UEAA_XZ(CDialog* pThis);
} } }

// =============================================================================
// CDialog::DoModal - Modal Dialog
// =============================================================================

// Ordinal: 3961
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" intptr_t MS_ABI impl__DoModal_CDialog__UEAA_JXZ(CDialog* pThis);
} } }

// =============================================================================
// CDialog::Create - Modeless Dialog
// =============================================================================

// Ordinal: 3081
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" int MS_ABI impl__Create_CDialog__UEAAHPEB_WPEAVCWnd___Z(
    CDialog* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd);
} } }

// Create with resource ID
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" int MS_ABI impl__Create_CDialog__UEAAHI_PEAVCWnd___Z(
    CDialog* pThis, UINT nIDTemplate, CWnd* pParentWnd);
} } }

// =============================================================================
// CDialog::EndDialog
// =============================================================================

// Ordinal: 4353
extern "C" void MS_ABI impl__EndDialog_CDialog__QEAAXH_Z(CDialog* pThis, int nResult);

// =============================================================================
// CDialog::OnInitDialog
// =============================================================================

// Ordinal: 10170
extern "C" int MS_ABI impl__OnInitDialog_CDialog__UEAAHXZ(CDialog* pThis);

// =============================================================================
// CDialog::OnOK / OnCancel
// =============================================================================

// Ordinal: 10711
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void MS_ABI impl__OnOK_CDialog__MEAAXXZ(CDialog* pThis);
} } }

// Ordinal: 8738
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void MS_ABI impl__OnCancel_CDialog__MEAAXXZ(CDialog* pThis);
} } }

// =============================================================================
// CDialog::CheckAutoCenter
// =============================================================================


// =============================================================================
// CDialog::PreTranslateMessage
// =============================================================================


// =============================================================================
// CDialog::Initialize
// =============================================================================













// =============================================================================
// Dialog Procedure
// =============================================================================














INT_PTR CALLBACK AfxDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// =============================================================================
// CDialogEx Implementation
// =============================================================================

// CDialogEx default constructor
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_XZ(CDialogEx* pThis);
} } }

// CDialogEx constructor with ID
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_IPEAVCWnd___Z(
    CDialogEx* pThis, UINT nIDTemplate, CWnd* pParentWnd);
} } }

// CDialogEx constructor with template name
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void* MS_ABI impl___0CDialogEx__QEAA_PEB_WPEAVCWnd___Z(
    CDialogEx* pThis, const wchar_t* lpszTemplateName, CWnd* pParentWnd);
} } }


















namespace openmfc { namespace detail { namespace dlgcore {
extern "C" int MS_ABI impl__SetBackgroundImage_CDialogEx__QEAAHIW4BackgroundLocation_1_H_Z(
    CDialogEx* pThis, UINT uiBmpResId, int backgroundLocation, BOOL bRepaint);
} } }


// =============================================================================
// Helper: GetDlgItem wrapper
// =============================================================================

extern CWnd* OpenMfcAttachCWnd(HWND hWnd);



// =============================================================================
// Dialog Data Exchange helpers
// =============================================================================

// Get text from dialog control
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" int MS_ABI impl__GetDlgItemText_CWnd__QEAAHHAEAV_CStringT___Z(
    CWnd* pThis, int nID, CString* rString);
} } }

// Set text on dialog control
namespace openmfc { namespace detail { namespace dlgcore {
extern "C" void MS_ABI impl__SetDlgItemText_CWnd__QEAAXHPEB_W_Z(
    CWnd* pThis, int nID, const wchar_t* lpszString);
} } }

// =============================================================================
// CDialog member functions (for vtable usage)
// These are needed by derived classes (CColorDialog, CFileDialog, etc.)
// =============================================================================













// =============================================================================
// CPropertyPage Implementation
// =============================================================================


// Default constructor

// Constructor with resource ID

// Constructor with template name






































// =============================================================================
// CPropertySheet Implementation
// =============================================================================


// Property sheet callback for modeless sheets
namespace openmfc { namespace detail { namespace dlgcore {
int CALLBACK PropSheetCallback(HWND hDlg, UINT message, LPARAM lParam);
} } }

// Property page dialog proc
INT_PTR CALLBACK PropPageDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// Default constructor

// Constructor with caption ID

// Constructor with caption string

// =============================================================================
// CPropertySheet constructor thunks (ords 910-913)
// These are placement-new wrappers that call the C++ ctors defined above.
// =============================================================================


















































// =============================================================================
// CCommonDialog::OnOK / OnCancel / OnPaint / OnHelpInfo
// =============================================================================




