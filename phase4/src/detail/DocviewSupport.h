#pragma once
// Shared internals of the former docview.cpp translation unit.
// Definitions live in detail/DocviewSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace docview {} } }
using namespace openmfc::detail::docview;
// Types declared by the internals of other units.
struct AFX_INTERFACEMAP;
// Document-View Architecture Implementation
//
// Implements CDocument, CView, CDocTemplate and related classes.
// This enables Document-View applications (SDI/MDI).

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting
#define OPENMFC_APPCORE_IMPL
#include "openmfc/afxwin.h"
#include "openmfc/afxole.h"
#include <windows.h>
#include <algorithm>
#include <cstring>
#include <deque>
#include <new>
#include <string>
#include <unordered_map>
#include <vector>

class CSplitterWnd;


namespace openmfc { namespace detail { namespace docview {
constexpr int kDocStringCount = 7;
} } }

namespace openmfc { namespace detail { namespace docview {
enum DocStringIndexCompat {
    kWindowTitle = 0,
    kDocName = 1,
    kFileNewName = 2,
    kFilterName = 3,
    kFilterExt = 4,
    kRegFileTypeId = 5,
    kRegFileTypeName = 6,
};
} } }

namespace openmfc { namespace detail { namespace docview {
bool LoadTemplateDocString(UINT resourceId, int index, CString& out);
} } }

namespace openmfc { namespace detail { namespace docview {
std::wstring NormalizePathForCompare(const wchar_t* path);
} } }

namespace openmfc { namespace detail { namespace docview {
bool PathsEqualInsensitive(const wchar_t* lhs, const wchar_t* rhs);
} } }

namespace openmfc { namespace detail { namespace docview {
bool PathHasExtensionInsensitive(const wchar_t* path, const wchar_t* expectedExt);
} } }

namespace openmfc { namespace detail { namespace docview {
struct DocChunkState {
    GUID guid = {};
    DWORD id = 0;
    void* value = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace docview {
struct DocumentExtraState {
    std::vector<DocChunkState> chunks;
    size_t chunkReadIndex = 0;
    bool searchHandler = false;
    HMENU defaultMenu = nullptr;
    HACCEL defaultAccel = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace docview {
struct TemplateExtraState {
    UINT containerId = 0;
    UINT serverId = 0;
    UINT serverEmbeddingId = 0;
    UINT previewId = 0;
    CRuntimeClass* serverDocClass = nullptr;
    CRuntimeClass* serverFrameClass = nullptr;
    CRuntimeClass* previewViewClass = nullptr;
    CRuntimeClass* previewFrameClass = nullptr;
};
} } }

namespace openmfc { namespace detail { namespace docview {
struct EditViewExtraState {
    std::wstring buffer;
    std::wstring findText;
    std::wstring replaceText;
    CFont* printerFont = nullptr;
    const wchar_t* locked = nullptr;
    UINT pageCount = 0;
};
} } }

namespace openmfc { namespace detail { namespace docview {
extern std::unordered_map<const CDocument*, DocumentExtraState> g_documentExtraStates;
} } }
namespace openmfc { namespace detail { namespace docview {
extern std::unordered_map<const CDocTemplate*, TemplateExtraState> g_templateExtraStates;
} } }
namespace openmfc { namespace detail { namespace docview {
extern std::unordered_map<const CEditView*, EditViewExtraState> g_editViewExtraStates;
} } }

namespace openmfc { namespace detail { namespace docview {
std::wstring EditViewText(const CEditView* view);
} } }

namespace openmfc { namespace detail { namespace docview {
void SetEditViewText(CEditView* view, const std::wstring& text);
} } }


// MS ABI calling convention
#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// =============================================================================
// Runtime Class Registration
// =============================================================================


// =============================================================================
// C++ Constructors/Destructors (required for IMPLEMENT_DYNCREATE)
// These call the impl_ functions which contain the actual logic
// =============================================================================

// Forward declarations
extern "C" void MS_ABI impl___0CDocument__QEAA_XZ(CDocument* pThis);
extern "C" void MS_ABI impl___1CDocument__UEAA_XZ(CDocument* pThis);
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis);
extern "C" void MS_ABI impl___1CView__UEAA_XZ(CView* pThis);
extern "C" void MS_ABI impl___0CScrollView__QEAA_XZ(CScrollView* pThis);
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView* pThis);
extern "C" void MS_ABI impl___0CFormView__QEAA_XZ(CFormView* pThis);
extern "C" void MS_ABI impl___0CFormView__IEAA_PEB_W_Z(CFormView* pThis, const wchar_t* lpszTemplateName);
extern "C" void MS_ABI impl___0CFormView__IEAA_I_Z(CFormView* pThis, unsigned int nIDTemplate);
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView* pThis);
extern "C" void MS_ABI impl___0CEditView__QEAA_XZ(CEditView* pThis);
extern "C" void MS_ABI impl___1CEditView__UEAA_XZ(CEditView* pThis);
extern "C" void MS_ABI impl___0CListView__QEAA_XZ(CListView* pThis);
extern "C" void MS_ABI impl___1CListView__UEAA_XZ(CListView* pThis);
extern "C" void MS_ABI impl___0CTreeView__QEAA_XZ(CTreeView* pThis);
extern "C" void MS_ABI impl___1CTreeView__UEAA_XZ(CTreeView* pThis);
extern "C" void MS_ABI impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___0CDocTemplate__IEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___1CDocTemplate__UEAA_XZ(CDocTemplate* pThis);
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_XZ(CSingleDocTemplate* pThis);
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CSingleDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___1CSingleDocTemplate__UEAA_XZ(CSingleDocTemplate* pThis);
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis);
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CMultiDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
extern "C" void MS_ABI impl___1CMultiDocTemplate__UEAA_XZ(CMultiDocTemplate* pThis);

// CDocument constructors

// CView constructors

// CScrollView constructors

// CFormView constructors
// CFormView constructor is inline in the header; extern "C" thunk below if needed

// CEditView constructors

// CListView constructors

// CTreeView constructors

// CDocTemplate constructors
// Protected default constructor

// CSingleDocTemplate constructors

// CMultiDocTemplate constructors

// =============================================================================
// Forward declarations of impl_ functions
// =============================================================================
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" int MS_ABI impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(CDocument* pThis, const wchar_t* lpszPathName);
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(CDocument* pThis, const wchar_t* lpszPathName);
extern "C" void MS_ABI impl__OnCloseDocument_CDocument__UEAAXXZ(CDocument* pThis);
extern "C" void MS_ABI impl__DeleteContents_CDocument__UEAAXXZ(CDocument* pThis);
extern "C" int MS_ABI impl__Open_CFile__UEAAHPEB_WIPEAVCFileException___Z(CFile* pThis, const wchar_t* lpszFileName, unsigned int nOpenFlags, CFileException* pException);
extern "C" int MS_ABI impl__ReportError_CException__UEAAHII_Z(CException* pThis, unsigned int type, unsigned int);
extern "C" void MS_ABI impl__Enable_CCmdUI__UEAAXH_Z(CCmdUI* pThis, int enable);
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(CDocument* pThis, int bModified);
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__SetTitle_CDocument__UEAAXPEB_W_Z(CDocument* pThis, const wchar_t* lpszTitle);
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__SetPathName_CDocument__UEAAXPEB_WH_Z(CDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU);
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(const CDocument* pThis);
extern "C" void MS_ABI impl__AddView_CDocument__UEAAXPEAVCView___Z(CDocument* pThis, CView* pView);
extern "C" void MS_ABI impl__RemoveView_CDocument__UEAAXPEAVCView___Z(CDocument* pThis, CView* pView);
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(const CDocument* pThis);
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(const CDocument* pThis, void*& pos);
extern "C" void MS_ABI impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(CDocument* pThis, CView* pSender, unsigned long lHint, CObject* pHint);
extern "C" int MS_ABI impl__SaveModified_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" int MS_ABI impl__DoSave_CDocument__UEAAHPEB_WH_Z(CDocument* pThis, const wchar_t* lpszPathName, int bReplace);
extern "C" int MS_ABI impl__DoFileSave_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" void MS_ABI impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(CDocument* pThis, CArchive* ar);
extern "C" CDocument* MS_ABI impl__GetDocument_CView__UEBAPEAVCDocument__XZ(const CView* pThis);
extern "C" void MS_ABI impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(CView* pThis, CView* pSender, unsigned long lHint, CObject* pHint);
extern "C" void MS_ABI impl__OnInitialUpdate_CView__UEAAXXZ(CView* pThis);
extern "C" int MS_ABI impl__OnPreparePrinting_CView__UEAAHPEAX_Z(CView* pThis, void* pInfo);
extern "C" void MS_ABI impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(CView* pThis, void* pDC, void* pInfo);
extern "C" void MS_ABI impl__OnEndPrinting_CView__UEAAXPEAX0_Z(CView* pThis, void* pDC, void* pInfo);
extern "C" void MS_ABI impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView);
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(const CView* pThis);
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z(CScrollView* pThis, int nMapMode, SIZE sizeTotal, const SIZE& sizePage, const SIZE& sizeLine);
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(CScrollView* pThis, int nMapMode, const SIZE* sizeTotal, const SIZE* sizePage, const SIZE* sizeLine);
extern "C" void MS_ABI impl__OnInitialUpdate_CScrollView__UEAAXXZ(CScrollView* pThis);
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(CScrollView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(CFormView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* pThis);
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(CFormView* pThis, void* pDX);
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(const CEditView* pThis);
extern "C" CObject* MS_ABI impl__CreateObject_CEditView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(CEditView* pThis, void* pDC);
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(const CListView* pThis);
extern "C" CObject* MS_ABI impl__CreateObject_CListView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(CListView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis);
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(const CTreeView* pThis);
extern "C" CObject* MS_ABI impl__CreateObject_CTreeView__SAPEAVCObject__XZ();
extern "C" void MS_ABI impl__OnDraw_CTreeView__UEAAXPEAX_Z(CTreeView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CTreeView__UEAAXXZ(CTreeView* pThis);
extern "C" CDocument* MS_ABI impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(CDocTemplate* pThis);
extern "C" CFrameWnd* MS_ABI impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(CDocTemplate* pThis, CDocument* pDoc, CFrameWnd* pOther);
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);
extern "C" void MS_ABI impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(CDocTemplate* pThis, CDocument* pDoc);
extern "C" void MS_ABI impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(CDocTemplate* pThis, CDocument* pDoc);
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(const CDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(const CDocTemplate* pThis, void*& pos);
extern "C" void MS_ABI impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(CDocTemplate* pThis, CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible);
extern "C" void MS_ABI impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(CDocTemplate* pThis, CDocument* pDoc);
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(const CDocTemplate* pThis, CString* rString, int nID);
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch);
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CSingleDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);
extern "C" void MS_ABI impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(CSingleDocTemplate* pThis, CDocument* pDoc);
extern "C" void MS_ABI impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(CSingleDocTemplate* pThis, CDocument* pDoc);
extern "C" void* MS_ABI impl__GetFirstDocPosition_CSingleDocTemplate__UEBAPEAU__POSITION__XZ(const CSingleDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CSingleDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(const CSingleDocTemplate* pThis, void*& pos);
extern "C" void MS_ABI impl__SetDefaultTitle_CSingleDocTemplate__UEAAXPEAVCDocument___Z(CSingleDocTemplate* pThis, CDocument* pDoc);
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CMultiDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);
extern "C" void MS_ABI impl__AddDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(CMultiDocTemplate* pThis, CDocument* pDoc);
extern "C" void MS_ABI impl__RemoveDocument_CMultiDocTemplate__UEAAXPEAVCDocument___Z(CMultiDocTemplate* pThis, CDocument* pDoc);
extern "C" void* MS_ABI impl__GetFirstDocPosition_CMultiDocTemplate__UEBAPEAU__POSITION__XZ(const CMultiDocTemplate* pThis);
extern "C" CDocument* MS_ABI impl__GetNextDoc_CMultiDocTemplate__UEBAPEAVCDocument__AEAPEAU__POSITION___Z(const CMultiDocTemplate* pThis, void*& pos);
extern "C" void MS_ABI impl__SetDefaultTitle_CMultiDocTemplate__UEAAXPEAVCDocument___Z(CMultiDocTemplate* pThis, CDocument* pDoc);

// =============================================================================
// CDocument Member Function Implementations (vtable entries)
// =============================================================================

// =============================================================================
// CView Member Function Implementations (vtable entries)
// =============================================================================

// OLE drag/drop virtual defaults.




// =============================================================================
// CScrollView Member Function Implementations (vtable entries)
// =============================================================================









// =============================================================================
// CFormView Member Function Implementations (vtable entries)
// =============================================================================

// =============================================================================
// CEditView Member Function Implementations (vtable entries)
// =============================================================================





// =============================================================================
// CListView Member Function Implementations (vtable entries)
// =============================================================================

// =============================================================================
// CTreeView Member Function Implementations (vtable entries)
// =============================================================================

// =============================================================================
// CDocTemplate Member Function Implementations (vtable entries)
// =============================================================================

// =============================================================================
// CSingleDocTemplate Member Function Implementations (vtable entries)
// =============================================================================

// =============================================================================
// CMultiDocTemplate Member Function Implementations (vtable entries)
// =============================================================================

// NOTE: No asm aliases needed here - the .def file handles the MSVC symbol aliasing
// The runtime class symbols are emitted with GCC mangling and the .def file maps them
// to MSVC-mangled exports

// =============================================================================
// CDocument Implementation
// =============================================================================








// SetModifiedFlag
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(
    CDocument* pThis, int bModified);

// IsModified
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis);


// GetTitle
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis);


// GetPathName
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis);

// AddView
extern "C" void MS_ABI impl__AddView_CDocument__UEAAXPEAVCView___Z(
    CDocument* pThis, CView* pView);

// RemoveView
extern "C" void MS_ABI impl__RemoveView_CDocument__UEAAXPEAVCView___Z(
    CDocument* pThis, CView* pView);

// GetFirstViewPosition
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(
    const CDocument* pThis);

// GetNextView
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(
    const CDocument* pThis, void*& pos);

// UpdateAllViews
extern "C" void MS_ABI impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(
    CDocument* pThis, CView* pSender, unsigned long lHint, CObject* pHint);




// Serialize (CDocument override)
extern "C" void MS_ABI impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(
    CDocument* pThis, CArchive* ar);

// =============================================================================
// CView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis);


// GetDocument
extern "C" CDocument* MS_ABI impl__GetDocument_CView__UEBAPEAVCDocument__XZ(
    const CView* pThis);

// OnUpdate
extern "C" void MS_ABI impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(
    CView* pThis, CView* pSender, unsigned long lHint, CObject* pHint);


// OnPreparePrinting
extern "C" int MS_ABI impl__OnPreparePrinting_CView__UEAAHPEAX_Z(
    CView* pThis, void* pInfo);


// OnBeginPrinting
extern "C" void MS_ABI impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo);


// OnEndPrinting
extern "C" void MS_ABI impl__OnEndPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo);


// OnActivateView
extern "C" void MS_ABI impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView);


// GetParentFrame
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(
    const CView* pThis);

// =============================================================================
// CScrollView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CScrollView__QEAA_XZ(CScrollView* pThis);


// SetScrollSizes
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(
    CScrollView* pThis, int nMapMode, const SIZE* sizeTotal,
    const SIZE* sizePage, const SIZE* sizeLine);

// Missing ABI variant with by-value first size argument and matching const refs
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__QEAAXHUtagSIZE__AEBU2_1_Z(
    CScrollView* pThis, int nMapMode, SIZE sizeTotal, const SIZE& sizePage, const SIZE& sizeLine);

// OnInitialUpdate (CScrollView override)
extern "C" void MS_ABI impl__OnInitialUpdate_CScrollView__UEAAXXZ(CScrollView* pThis);

// OnDraw (CScrollView) - pure virtual in CView, needs implementation
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(
    CScrollView* pThis, void* pDC);

// =============================================================================
// CFormView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CFormView__QEAA_XZ(CFormView* pThis);

// Constructor with template name
namespace openmfc { namespace detail { namespace docview {
extern "C" void MS_ABI impl___0CFormView__QEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName);
} } }

// Constructor with resource ID
namespace openmfc { namespace detail { namespace docview {
extern "C" void MS_ABI impl___0CFormView__QEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate);
} } }




// OnDraw (CFormView) - form views don't draw, the dialog template does
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDC);


// DoDataExchange
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDX);

// =============================================================================
// CEditView Implementation
// =============================================================================



// GetEditCtrl
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(
    const CEditView* pThis);


// OnDraw (CEditView)
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(
    CEditView* pThis, void* pDC);

// =============================================================================
// CListView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CListView__QEAA_XZ(CListView* pThis);

// Destructor
extern "C" void MS_ABI impl___1CListView__UEAA_XZ(CListView* pThis);

// GetListCtrl
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(
    const CListView* pThis);


// OnDraw (CListView)
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(
    CListView* pThis, void* pDC);

// OnInitialUpdate (CListView)
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis);

// =============================================================================
// CTreeView Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CTreeView__QEAA_XZ(CTreeView* pThis);

// Destructor
extern "C" void MS_ABI impl___1CTreeView__UEAA_XZ(CTreeView* pThis);

// GetTreeCtrl
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(
    const CTreeView* pThis);


// OnDraw (CTreeView)
extern "C" void MS_ABI impl__OnDraw_CTreeView__UEAAXPEAX_Z(
    CTreeView* pThis, void* pDC);

// OnInitialUpdate (CTreeView)
extern "C" void MS_ABI impl__OnInitialUpdate_CTreeView__UEAAXXZ(CTreeView* pThis);

// =============================================================================
// CDocTemplate Implementation
// =============================================================================

// Constructor
extern "C" void MS_ABI impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);





// OpenDocumentFile
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);



// GetFirstDocPosition
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(
    const CDocTemplate* pThis);

// GetNextDoc
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(
    const CDocTemplate* pThis, void*& pos);


// SetDefaultTitle
extern "C" void MS_ABI impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc);

// GetDocString
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(
    const CDocTemplate* pThis, CString* rString, int nID);


// MatchDocType
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch);


// =============================================================================
// CSingleDocTemplate Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_XZ(CSingleDocTemplate* pThis);










// =============================================================================
// CMultiDocTemplate Implementation
// =============================================================================

// Default Constructor
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis);










namespace openmfc { namespace detail { namespace docview {
extern const AFX_MSGMAP_ENTRY g_docviewEmptyEntries[];
} } }
namespace openmfc { namespace detail { namespace docview {
extern const AFX_MSGMAP g_docviewEmptyMap;
} } }
namespace openmfc { namespace detail { namespace docview {
constexpr const AFX_INTERFACEMAP* g_docviewEmptyInterfaceMap = nullptr;
} } }














namespace openmfc { namespace detail { namespace docview {
extern "C" int MS_ABI impl__GetThumbnail_CDocument__UEAAHIPEAPEAUHBITMAP____PEAW4WTS_ALPHATYPE___Z(CDocument* pThis, unsigned int size, HBITMAP* bitmap, int* alphaType);
} } }








