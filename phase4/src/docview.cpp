// Document-View Architecture Implementation
//
// Implements CDocument, CView, CDocTemplate and related classes.
// This enables Document-View applications (SDI/MDI).

// Define OPENMFC_APPCORE_IMPL to prevent inline implementations conflicting
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
// Runtime Class Registration
// =============================================================================

IMPLEMENT_DYNCREATE(CDocument, CCmdTarget)
IMPLEMENT_DYNAMIC(CView, CWnd)
IMPLEMENT_DYNCREATE(CScrollView, CView)
IMPLEMENT_DYNCREATE(CFormView, CScrollView)
IMPLEMENT_DYNCREATE(CEditView, CView)
IMPLEMENT_DYNCREATE(CListView, CView)
IMPLEMENT_DYNCREATE(CTreeView, CView)
IMPLEMENT_DYNAMIC(CDocTemplate, CCmdTarget)
IMPLEMENT_DYNCREATE(CSingleDocTemplate, CDocTemplate)
IMPLEMENT_DYNCREATE(CMultiDocTemplate, CDocTemplate)

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
extern "C" void MS_ABI impl___1CDocTemplate__UEAA_XZ(CDocTemplate* pThis);
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_XZ(CSingleDocTemplate* pThis);
extern "C" void MS_ABI impl___1CSingleDocTemplate__UEAA_XZ(CSingleDocTemplate* pThis);
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis);
extern "C" void MS_ABI impl___1CMultiDocTemplate__UEAA_XZ(CMultiDocTemplate* pThis);

// CDocument constructors
CDocument::CDocument() { impl___0CDocument__QEAA_XZ(this); }
CDocument::~CDocument() { impl___1CDocument__UEAA_XZ(this); }

// CView constructors
CView::CView() { impl___0CView__QEAA_XZ(this); }
CView::~CView() { impl___1CView__UEAA_XZ(this); }

// CScrollView constructors
CScrollView::CScrollView() { impl___0CScrollView__QEAA_XZ(this); }
CScrollView::~CScrollView() { impl___1CScrollView__UEAA_XZ(this); }

// CFormView constructors
CFormView::CFormView() { impl___0CFormView__QEAA_XZ(this); }
CFormView::~CFormView() { impl___1CFormView__UEAA_XZ(this); }

// CEditView constructors
CEditView::CEditView() { impl___0CEditView__QEAA_XZ(this); }
CEditView::~CEditView() { impl___1CEditView__UEAA_XZ(this); }

// CListView constructors
CListView::CListView() { impl___0CListView__QEAA_XZ(this); }
CListView::~CListView() { impl___1CListView__UEAA_XZ(this); }

// CTreeView constructors
CTreeView::CTreeView() { impl___0CTreeView__QEAA_XZ(this); }
CTreeView::~CTreeView() { impl___1CTreeView__UEAA_XZ(this); }

// CDocTemplate constructors
CDocTemplate::CDocTemplate(unsigned int nIDResource, CRuntimeClass* pDocClass,
                           CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass) {
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        this, nIDResource, pDocClass, pFrameClass, pViewClass);
}
// Protected default constructor
CDocTemplate::CDocTemplate() {
    m_nIDResource = 0;
    m_pDocClass = nullptr;
    m_pFrameClass = nullptr;
    m_pViewClass = nullptr;
    m_pFirstDoc = nullptr;
    m_pLastDoc = nullptr;
}
CDocTemplate::~CDocTemplate() { impl___1CDocTemplate__UEAA_XZ(this); }

// CSingleDocTemplate constructors
CSingleDocTemplate::CSingleDocTemplate() { impl___0CSingleDocTemplate__QEAA_XZ(this); }
CSingleDocTemplate::~CSingleDocTemplate() { impl___1CSingleDocTemplate__UEAA_XZ(this); }

// CMultiDocTemplate constructors
CMultiDocTemplate::CMultiDocTemplate() { impl___0CMultiDocTemplate__QEAA_XZ(this); }
CMultiDocTemplate::~CMultiDocTemplate() { impl___1CMultiDocTemplate__UEAA_XZ(this); }

// =============================================================================
// Forward declarations of impl_ functions
// =============================================================================
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis);
extern "C" int MS_ABI impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(CDocument* pThis, const wchar_t* lpszPathName);
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(CDocument* pThis, const wchar_t* lpszPathName);
extern "C" void MS_ABI impl__OnCloseDocument_CDocument__UEAAXXZ(CDocument* pThis);
extern "C" void MS_ABI impl__DeleteContents_CDocument__UEAAXXZ(CDocument* pThis);
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
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(CScrollView* pThis, int nMapMode, const SIZE* sizeTotal, const SIZE* sizePage, const SIZE* sizeLine);
extern "C" void MS_ABI impl__OnInitialUpdate_CScrollView__UEAAXXZ(CScrollView* pThis);
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(CScrollView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(CFormView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* pThis);
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(CFormView* pThis, void* pDX);
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(const CEditView* pThis);
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(CEditView* pThis, void* pDC);
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(const CListView* pThis);
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(CListView* pThis, void* pDC);
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis);
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(const CTreeView* pThis);
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
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(CMultiDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible);

// =============================================================================
// CDocument Member Function Implementations (vtable entries)
// =============================================================================
int CDocument::OnNewDocument() { return impl__OnNewDocument_CDocument__UEAAHXZ(this); }
int CDocument::OnOpenDocument(const wchar_t* lpszPathName) { return impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(this, lpszPathName); }
int CDocument::OnSaveDocument(const wchar_t* lpszPathName) { return impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(this, lpszPathName); }
void CDocument::OnCloseDocument() { impl__OnCloseDocument_CDocument__UEAAXXZ(this); }
void CDocument::DeleteContents() { impl__DeleteContents_CDocument__UEAAXXZ(this); }
void CDocument::SetModifiedFlag(int bModified) { impl__SetModifiedFlag_CDocument__UEAAXH_Z(this, bModified); }
int CDocument::IsModified() const { return impl__IsModified_CDocument__UEBAHXZ(this); }
void CDocument::SetTitle(const wchar_t* lpszTitle) { impl__SetTitle_CDocument__UEAAXPEB_W_Z(this, lpszTitle); }
const wchar_t* CDocument::GetTitle() const { return impl__GetTitle_CDocument__UEBAPEB_WXZ(this); }
void CDocument::SetPathName(const wchar_t* lpszPathName, int bAddToMRU) { impl__SetPathName_CDocument__UEAAXPEB_WH_Z(this, lpszPathName, bAddToMRU); }
const wchar_t* CDocument::GetPathName() const { return impl__GetPathName_CDocument__UEBAPEB_WXZ(this); }
void CDocument::AddView(CView* pView) { impl__AddView_CDocument__UEAAXPEAVCView___Z(this, pView); }
void CDocument::RemoveView(CView* pView) { impl__RemoveView_CDocument__UEAAXPEAVCView___Z(this, pView); }
void* CDocument::GetFirstViewPosition() const { return impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(this); }
CView* CDocument::GetNextView(void*& pos) const { return impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(this, pos); }
void CDocument::UpdateAllViews(CView* pSender, unsigned long lHint, CObject* pHint) { impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(this, pSender, lHint, pHint); }
int CDocument::SaveModified() { return impl__SaveModified_CDocument__UEAAHXZ(this); }
int CDocument::DoSave(const wchar_t* lpszPathName, int bReplace) { return impl__DoSave_CDocument__UEAAHPEB_WH_Z(this, lpszPathName, bReplace); }
int CDocument::DoFileSave() { return impl__DoFileSave_CDocument__UEAAHXZ(this); }
void CDocument::Serialize(CArchive& ar) { impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(this, &ar); }

// =============================================================================
// CView Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CView::GetDocument() const { return impl__GetDocument_CView__UEBAPEAVCDocument__XZ(this); }
void CView::OnUpdate(CView* pSender, unsigned long lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
void CView::OnInitialUpdate() { impl__OnInitialUpdate_CView__UEAAXXZ(this); }
int CView::OnPreparePrinting(void* pInfo) { return impl__OnPreparePrinting_CView__UEAAHPEAX_Z(this, pInfo); }
void CView::OnBeginPrinting(void* pDC, void* pInfo) { impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(this, pDC, pInfo); }
void CView::OnEndPrinting(void* pDC, void* pInfo) { impl__OnEndPrinting_CView__UEAAXPEAX0_Z(this, pDC, pInfo); }
void CView::OnActivateView(int bActivate, CView* pActivateView, CView* pDeactiveView) { impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(this, bActivate, pActivateView, pDeactiveView); }
CFrameWnd* CView::GetParentFrame() const { return impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(this); }

// =============================================================================
// CScrollView Member Function Implementations (vtable entries)
// =============================================================================
void CScrollView::SetScrollSizes(int nMapMode, const SIZE& sizeTotal, const SIZE& sizePage, const SIZE& sizeLine) { impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(this, nMapMode, &sizeTotal, &sizePage, &sizeLine); }
void CScrollView::OnInitialUpdate() { impl__OnInitialUpdate_CScrollView__UEAAXXZ(this); }
void CScrollView::OnDraw(void* pDC) { impl__OnDraw_CScrollView__UEAAXPEAX_Z(this, pDC); }
void CScrollView::OnUpdate(CView* pSender, unsigned long lHint, CObject* pHint) { impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(this, pSender, lHint, pHint); }
void CScrollView::GetScrollBarSizes(SIZE& sizeSb) const { sizeSb.cx = 0; sizeSb.cy = 0; }
void CScrollView::GetTrueClientSize(SIZE& size, SIZE& sizeSb) const { size.cx = 0; size.cy = 0; sizeSb.cx = 0; sizeSb.cy = 0; }
void CScrollView::ScrollToPosition(POINT pt) { (void)pt; }
void CScrollView::GetScrollPosition(POINT& pt) const { pt.x = 0; pt.y = 0; }
void CScrollView::FillOutsideRect(void* pDC, void* pBrush) { (void)pDC; (void)pBrush; }
void CScrollView::ResizeParentToFit(int bShrinkOnly) { (void)bShrinkOnly; }

// =============================================================================
// CFormView Member Function Implementations (vtable entries)
// =============================================================================
void CFormView::OnDraw(void* pDC) { impl__OnDraw_CFormView__UEAAXPEAX_Z(this, pDC); }
void CFormView::OnInitialUpdate() { impl__OnInitialUpdate_CFormView__UEAAXXZ(this); }
void CFormView::DoDataExchange(void* pDX) { impl__DoDataExchange_CFormView__UEAAXPEAX_Z(this, pDX); }
int CFormView::Create(const wchar_t*, const wchar_t*, DWORD, const struct tagRECT&, CWnd*, unsigned int, CCreateContext*) { return FALSE; }

// =============================================================================
// CEditView Member Function Implementations (vtable entries)
// =============================================================================
CEdit* CEditView::GetEditCtrl() const { return impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(this); }
void CEditView::OnDraw(void* pDC) { impl__OnDraw_CEditView__UEAAXPEAX_Z(this, pDC); }
void CEditView::Serialize(CArchive&) { }
void CEditView::GetSelectedText(CString&) const { }
int CEditView::FindText(const wchar_t*, int, int) { return -1; }
int CEditView::PrintInsideRect(void*, struct tagRECT&, struct tagRECT&, int) { return 0; }

// =============================================================================
// CListView Member Function Implementations (vtable entries)
// =============================================================================
CListCtrl* CListView::GetListCtrl() const { return impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(this); }
void CListView::OnDraw(void* pDC) { impl__OnDraw_CListView__UEAAXPEAX_Z(this, pDC); }
void CListView::OnInitialUpdate() { impl__OnInitialUpdate_CListView__UEAAXXZ(this); }

// =============================================================================
// CTreeView Member Function Implementations (vtable entries)
// =============================================================================
CTreeCtrl* CTreeView::GetTreeCtrl() const { return impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(this); }
void CTreeView::OnDraw(void* pDC) { impl__OnDraw_CTreeView__UEAAXPEAX_Z(this, pDC); }
void CTreeView::OnInitialUpdate() { impl__OnInitialUpdate_CTreeView__UEAAXXZ(this); }

// =============================================================================
// CDocTemplate Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CDocTemplate::CreateNewDocument() { return impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(this); }
CFrameWnd* CDocTemplate::CreateNewFrame(CDocument* pDoc, CFrameWnd* pOther) { return impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(this, pDoc, pOther); }
int CDocTemplate::CreateAndReplaceFrame(CFrameWnd*, CDocument*) { return FALSE; }
CDocument* CDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CDocTemplate::InitialUpdateFrame(CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible) { impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(this, pFrame, pDoc, bMakeVisible); }
void CDocTemplate::SetDefaultTitle(CDocument* pDoc) { impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
int CDocTemplate::GetDocString(CString& rString, int nID) const { return impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(this, &rString, nID); }
int CDocTemplate::MatchDocType(const wchar_t* lpszPathName, CDocument*& rpDocMatch) { return impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(this, lpszPathName, rpDocMatch); }
void CDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void* CDocTemplate::GetFirstDocPosition() const { return impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(this); }
CDocument* CDocTemplate::GetNextDoc(void*& rPos) const { return impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(this, rPos); }

// =============================================================================
// CSingleDocTemplate Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CSingleDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }
void CSingleDocTemplate::AddDocument(CDocument* pDoc) { impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }
void CSingleDocTemplate::RemoveDocument(CDocument* pDoc) { impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(this, pDoc); }

// =============================================================================
// CMultiDocTemplate Member Function Implementations (vtable entries)
// =============================================================================
CDocument* CMultiDocTemplate::OpenDocumentFile(const wchar_t* lpszPathName, int bMakeVisible) { return impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(this, lpszPathName, bMakeVisible); }

// NOTE: No asm aliases needed here - the .def file handles the MSVC symbol aliasing
// The runtime class symbols are emitted with GCC mangling and the .def file maps them
// to MSVC-mangled exports

// =============================================================================
// CDocument Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CDocument@@QEAA@XZ
extern "C" void MS_ABI impl___0CDocument__QEAA_XZ(CDocument* pThis) {
    if (!pThis) return;
    // Initialize members
    pThis->m_bModified = FALSE;
    pThis->m_bAutoDelete = TRUE;
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;
    pThis->m_pNextDoc = nullptr;
}

// Destructor
// Symbol: ??1CDocument@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocument__UEAA_XZ(CDocument* pThis) {
    if (!pThis) return;
    // Disconnect all views
    CView* pView = pThis->m_pFirstView;
    while (pView) {
        CView* pNext = pView->m_pNextView;
        pView->m_pDocument = nullptr;
        pView = pNext;
    }
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;
}

// OnNewDocument
// Symbol: ?OnNewDocument@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__OnNewDocument_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) return FALSE;
    pThis->DeleteContents();
    pThis->SetModifiedFlag(FALSE);
    return TRUE;
}

// OnOpenDocument
// Symbol: ?OnOpenDocument@CDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnOpenDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName)
{
    (void)lpszPathName;
    if (!pThis) return FALSE;
    pThis->DeleteContents();
    pThis->SetModifiedFlag(FALSE);
    // Real implementation would open file and serialize
    return TRUE;
}

// OnSaveDocument
// Symbol: ?OnSaveDocument@CDocument@@UEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszPathName)
{
    (void)lpszPathName;
    if (!pThis) return FALSE;
    pThis->SetModifiedFlag(FALSE);
    // Real implementation would serialize to file
    return TRUE;
}

// OnCloseDocument
// Symbol: ?OnCloseDocument@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__OnCloseDocument_CDocument__UEAAXXZ(CDocument* pThis) {
    if (!pThis) return;

    // Destroy all views
    CView* pView = pThis->m_pFirstView;
    while (pView) {
        CView* pNext = pView->m_pNextView;
        if (pView->m_hWnd) {
            ::DestroyWindow(pView->m_hWnd);
        }
        pView->m_pDocument = nullptr;
        pView = pNext;
    }
    pThis->m_pFirstView = nullptr;
    pThis->m_pLastView = nullptr;

    // Auto-delete if enabled
    if (pThis->m_bAutoDelete) {
        delete pThis;
    }
}

// DeleteContents
// Symbol: ?DeleteContents@CDocument@@UEAAXXZ
extern "C" void MS_ABI impl__DeleteContents_CDocument__UEAAXXZ(CDocument* pThis) {
    (void)pThis;
    // Default: nothing to delete
}

// SetModifiedFlag
// Symbol: ?SetModifiedFlag@CDocument@@UEAAXH@Z
extern "C" void MS_ABI impl__SetModifiedFlag_CDocument__UEAAXH_Z(
    CDocument* pThis, int bModified)
{
    if (pThis) {
        pThis->m_bModified = bModified;
    }
}

// IsModified
// Symbol: ?IsModified@CDocument@@UEBAHXZ
extern "C" int MS_ABI impl__IsModified_CDocument__UEBAHXZ(const CDocument* pThis) {
    return pThis ? pThis->m_bModified : FALSE;
}

// SetTitle
// Symbol: ?SetTitle@CDocument@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetTitle_CDocument__UEAAXPEB_W_Z(
    CDocument* pThis, const wchar_t* lpszTitle)
{
    if (pThis) {
        pThis->m_strTitle = lpszTitle ? lpszTitle : L"";
    }
}

// GetTitle
// Symbol: ?GetTitle@CDocument@@UEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetTitle_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis)
{
    return pThis ? (const wchar_t*)pThis->m_strTitle : L"";
}

// SetPathName
// Symbol: ?SetPathName@CDocument@@UEAAXPEB_WH@Z
extern "C" void MS_ABI impl__SetPathName_CDocument__UEAAXPEB_WH_Z(
    CDocument* pThis, const wchar_t* lpszPathName, int bAddToMRU)
{
    (void)bAddToMRU;
    if (pThis) {
        pThis->m_strPathName = lpszPathName ? lpszPathName : L"";
    }
}

// GetPathName
// Symbol: ?GetPathName@CDocument@@UEBAPEB_WXZ
extern "C" const wchar_t* MS_ABI impl__GetPathName_CDocument__UEBAPEB_WXZ(
    const CDocument* pThis)
{
    return pThis ? (const wchar_t*)pThis->m_strPathName : L"";
}

// AddView
// Symbol: ?AddView@CDocument@@UEAAXPEAVCView@@@Z
extern "C" void MS_ABI impl__AddView_CDocument__UEAAXPEAVCView___Z(
    CDocument* pThis, CView* pView)
{
    if (!pThis || !pView) return;

    // Set view's document
    pView->m_pDocument = pThis;
    pView->m_pNextView = nullptr;

    // Add to end of list
    if (!pThis->m_pFirstView) {
        pThis->m_pFirstView = pView;
        pThis->m_pLastView = pView;
    } else {
        pThis->m_pLastView->m_pNextView = pView;
        pThis->m_pLastView = pView;
    }
}

// RemoveView
// Symbol: ?RemoveView@CDocument@@UEAAXPEAVCView@@@Z
extern "C" void MS_ABI impl__RemoveView_CDocument__UEAAXPEAVCView___Z(
    CDocument* pThis, CView* pView)
{
    if (!pThis || !pView) return;

    // Find and remove from list
    CView* pPrev = nullptr;
    CView* pCur = pThis->m_pFirstView;
    while (pCur) {
        if (pCur == pView) {
            if (pPrev) {
                pPrev->m_pNextView = pCur->m_pNextView;
            } else {
                pThis->m_pFirstView = pCur->m_pNextView;
            }
            if (pThis->m_pLastView == pView) {
                pThis->m_pLastView = pPrev;
            }
            pView->m_pDocument = nullptr;
            pView->m_pNextView = nullptr;
            break;
        }
        pPrev = pCur;
        pCur = pCur->m_pNextView;
    }
}

// GetFirstViewPosition
// Symbol: ?GetFirstViewPosition@CDocument@@UEBAPEAXXZ
extern "C" void* MS_ABI impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(
    const CDocument* pThis)
{
    return pThis ? pThis->m_pFirstView : nullptr;
}

// GetNextView
// Symbol: ?GetNextView@CDocument@@UEBAPEAVCView@@AEAPEAX@Z
extern "C" CView* MS_ABI impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(
    const CDocument* pThis, void*& pos)
{
    if (!pThis || !pos) return nullptr;

    CView* pView = static_cast<CView*>(pos);
    pos = pView->m_pNextView;
    return pView;
}

// UpdateAllViews
// Symbol: ?UpdateAllViews@CDocument@@UEAAXPEAVCView@@KPEAVCObject@@@Z
extern "C" void MS_ABI impl__UpdateAllViews_CDocument__UEAAXPEAVCView__KPEAVCObject___Z(
    CDocument* pThis, CView* pSender, unsigned long lHint, CObject* pHint)
{
    if (!pThis) return;

    CView* pView = pThis->m_pFirstView;
    while (pView) {
        if (pView != pSender) {
            pView->OnUpdate(pSender, lHint, pHint);
        }
        pView = pView->m_pNextView;
    }
}

// SaveModified
// Symbol: ?SaveModified@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__SaveModified_CDocument__UEAAHXZ(CDocument* pThis) {
    (void)pThis;
    // Default: allow close without saving
    return TRUE;
}

// DoSave
// Symbol: ?DoSave@CDocument@@UEAAHPEB_WH@Z
extern "C" int MS_ABI impl__DoSave_CDocument__UEAAHPEB_WH_Z(
    CDocument* pThis, const wchar_t* lpszPathName, int bReplace)
{
    (void)bReplace;
    if (!pThis) return FALSE;
    return impl__OnSaveDocument_CDocument__UEAAHPEB_W_Z(pThis, lpszPathName);
}

// DoFileSave
// Symbol: ?DoFileSave@CDocument@@UEAAHXZ
extern "C" int MS_ABI impl__DoFileSave_CDocument__UEAAHXZ(CDocument* pThis) {
    if (!pThis) return FALSE;
    const wchar_t* path = impl__GetPathName_CDocument__UEBAPEB_WXZ(pThis);
    return impl__DoSave_CDocument__UEAAHPEB_WH_Z(pThis, path, TRUE);
}

// Serialize (CDocument override)
// Symbol: ?Serialize@CDocument@@UEAAXAEAVCArchive@@@Z
extern "C" void MS_ABI impl__Serialize_CDocument__UEAAXAEAVCArchive___Z(
    CDocument* pThis, CArchive* ar)
{
    (void)pThis;
    (void)ar;
    // Default: no serialization
}

// =============================================================================
// CView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CView@@QEAA@XZ
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis) {
    if (!pThis) return;
    pThis->m_hWnd = nullptr;
    pThis->m_pDocument = nullptr;
    pThis->m_pNextView = nullptr;
}

// Destructor
// Symbol: ??1CView@@UEAA@XZ
extern "C" void MS_ABI impl___1CView__UEAA_XZ(CView* pThis) {
    if (!pThis) return;
    if (pThis->m_pDocument) {
        pThis->m_pDocument->RemoveView(pThis);
    }
}

// GetDocument
// Symbol: ?GetDocument@CView@@UEBAPEAVCDocument@@XZ
extern "C" CDocument* MS_ABI impl__GetDocument_CView__UEBAPEAVCDocument__XZ(
    const CView* pThis)
{
    return pThis ? pThis->m_pDocument : nullptr;
}

// OnUpdate
// Symbol: ?OnUpdate@CView@@UEAAXPEAV1@KPEAVCObject@@@Z
extern "C" void MS_ABI impl__OnUpdate_CView__UEAAXPEAV1_KPEAVCObject___Z(
    CView* pThis, CView* pSender, unsigned long lHint, CObject* pHint)
{
    (void)pSender;
    (void)lHint;
    (void)pHint;

    if (pThis && pThis->m_hWnd) {
        // Invalidate the view to force a redraw
        ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    }
}

// OnInitialUpdate
// Symbol: ?OnInitialUpdate@CView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CView__UEAAXXZ(CView* pThis) {
    if (pThis) {
        pThis->OnUpdate(nullptr, 0, nullptr);
    }
}

// OnPreparePrinting
// Symbol: ?OnPreparePrinting@CView@@UEAAHPEAX@Z
extern "C" int MS_ABI impl__OnPreparePrinting_CView__UEAAHPEAX_Z(
    CView* pThis, void* pInfo)
{
    (void)pThis;
    (void)pInfo;
    return FALSE;  // No printing support by default
}

// OnBeginPrinting
// Symbol: ?OnBeginPrinting@CView@@UEAAXPEAX0@Z
extern "C" void MS_ABI impl__OnBeginPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo)
{
    (void)pThis;
    (void)pDC;
    (void)pInfo;
}

// OnEndPrinting
// Symbol: ?OnEndPrinting@CView@@UEAAXPEAX0@Z
extern "C" void MS_ABI impl__OnEndPrinting_CView__UEAAXPEAX0_Z(
    CView* pThis, void* pDC, void* pInfo)
{
    (void)pThis;
    (void)pDC;
    (void)pInfo;
}

// OnActivateView
// Symbol: ?OnActivateView@CView@@UEAAXHPEAV1@0@Z
extern "C" void MS_ABI impl__OnActivateView_CView__UEAAXHPEAV1_0_Z(
    CView* pThis, int bActivate, CView* pActivateView, CView* pDeactiveView)
{
    (void)pThis;
    (void)bActivate;
    (void)pActivateView;
    (void)pDeactiveView;
}

// GetParentFrame
// Symbol: ?GetParentFrame@CView@@UEBAPEAVCFrameWnd@@XZ
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CView__UEBAPEAVCFrameWnd__XZ(
    const CView* pThis)
{
    if (!pThis || !pThis->m_hWnd) return nullptr;

    HWND hWndParent = ::GetParent(pThis->m_hWnd);
    // In a real implementation, we'd look up the CFrameWnd from HWND
    // For now, return nullptr
    (void)hWndParent;
    return nullptr;
}

// =============================================================================
// CScrollView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CScrollView@@QEAA@XZ
extern "C" void MS_ABI impl___0CScrollView__QEAA_XZ(CScrollView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_totalLog.cx = 0;
    pThis->m_totalLog.cy = 0;
    pThis->m_pageDev.cx = 0;
    pThis->m_pageDev.cy = 0;
    pThis->m_lineDev.cx = 0;
    pThis->m_lineDev.cy = 0;
    pThis->m_nMapMode = MM_TEXT;
}

// Destructor
// Symbol: ??1CScrollView@@UEAA@XZ
extern "C" void MS_ABI impl___1CScrollView__UEAA_XZ(CScrollView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// SetScrollSizes
// Symbol: ?SetScrollSizes@CScrollView@@UEAAXHAEBUtagSIZE@@00@Z
extern "C" void MS_ABI impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(
    CScrollView* pThis, int nMapMode, const SIZE* sizeTotal,
    const SIZE* sizePage, const SIZE* sizeLine)
{
    if (!pThis) return;

    pThis->m_nMapMode = nMapMode;
    pThis->m_totalLog = *sizeTotal;

    if (sizePage && (sizePage->cx > 0 || sizePage->cy > 0)) {
        pThis->m_pageDev = *sizePage;
    } else {
        pThis->m_pageDev.cx = sizeTotal->cx / 10;
        pThis->m_pageDev.cy = sizeTotal->cy / 10;
    }

    if (sizeLine && (sizeLine->cx > 0 || sizeLine->cy > 0)) {
        pThis->m_lineDev = *sizeLine;
    } else {
        pThis->m_lineDev.cx = pThis->m_pageDev.cx / 10;
        pThis->m_lineDev.cy = pThis->m_pageDev.cy / 10;
    }

    if (pThis->m_hWnd) {
        // Update scroll bars
        SCROLLINFO si = { sizeof(si) };
        si.fMask = SIF_PAGE | SIF_RANGE;
        si.nMin = 0;
        si.nMax = sizeTotal->cx;
        si.nPage = pThis->m_pageDev.cx;
        ::SetScrollInfo(pThis->m_hWnd, SB_HORZ, &si, TRUE);

        si.nMax = sizeTotal->cy;
        si.nPage = pThis->m_pageDev.cy;
        ::SetScrollInfo(pThis->m_hWnd, SB_VERT, &si, TRUE);
    }
}

// OnInitialUpdate (CScrollView override)
// Symbol: ?OnInitialUpdate@CScrollView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CScrollView__UEAAXXZ(CScrollView* pThis) {
    if (pThis) {
        // Set default scroll sizes if not set
        if (pThis->m_totalLog.cx == 0 && pThis->m_totalLog.cy == 0) {
            SIZE sizeDefault = { 100, 100 };
            impl__SetScrollSizes_CScrollView__UEAAXHAEBUtagSIZE__00_Z(
                pThis, MM_TEXT, &sizeDefault, nullptr, nullptr);
        }
        impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
    }
}

// OnDraw (CScrollView) - pure virtual in CView, needs implementation
// Symbol: ?OnDraw@CScrollView@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnDraw_CScrollView__UEAAXPEAX_Z(
    CScrollView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Default: nothing to draw
}

// =============================================================================
// CFormView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CFormView@@QEAA@XZ
extern "C" void MS_ABI impl___0CFormView__QEAA_XZ(CFormView* pThis) {
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = nullptr;
    pThis->m_nIDHelp = 0;
}

// Constructor with template name
// Symbol: ??0CFormView@@QEAA@PEB_W@Z
extern "C" void MS_ABI impl___0CFormView__QEAA_PEB_W_Z(
    CFormView* pThis, const wchar_t* lpszTemplateName)
{
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = lpszTemplateName;
    pThis->m_nIDHelp = 0;
}

// Constructor with resource ID
// Symbol: ??0CFormView@@QEAA@I@Z
extern "C" void MS_ABI impl___0CFormView__QEAA_I_Z(
    CFormView* pThis, unsigned int nIDTemplate)
{
    if (!pThis) return;
    impl___0CScrollView__QEAA_XZ(pThis);
    pThis->m_lpszTemplateName = MAKEINTRESOURCEW(nIDTemplate);
    pThis->m_nIDHelp = nIDTemplate;
}

// Destructor
// Symbol: ??1CFormView@@UEAA@XZ
extern "C" void MS_ABI impl___1CFormView__UEAA_XZ(CFormView* pThis) {
    impl___1CScrollView__UEAA_XZ(pThis);
}

// OnDraw (CFormView) - form views don't draw, the dialog template does
// Symbol: ?OnDraw@CFormView@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnDraw_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Form views are drawn by their child controls
}

// OnInitialUpdate (CFormView)
// Symbol: ?OnInitialUpdate@CFormView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CFormView__UEAAXXZ(CFormView* pThis) {
    impl__OnInitialUpdate_CScrollView__UEAAXXZ(pThis);
}

// DoDataExchange
// Symbol: ?DoDataExchange@CFormView@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__DoDataExchange_CFormView__UEAAXPEAX_Z(
    CFormView* pThis, void* pDX)
{
    (void)pThis;
    (void)pDX;
    // Override in derived class to transfer data
}

// =============================================================================
// CEditView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CEditView@@QEAA@XZ
extern "C" void MS_ABI impl___0CEditView__QEAA_XZ(CEditView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pEditCtrl = nullptr;
}

// Destructor
// Symbol: ??1CEditView@@UEAA@XZ
extern "C" void MS_ABI impl___1CEditView__UEAA_XZ(CEditView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// GetEditCtrl
// Symbol: ?GetEditCtrl@CEditView@@QEBAPEAVCEdit@@XZ
extern "C" CEdit* MS_ABI impl__GetEditCtrl_CEditView__QEBAPEAVCEdit__XZ(
    const CEditView* pThis)
{
    return pThis ? pThis->m_pEditCtrl : nullptr;
}

// OnDraw (CEditView)
// Symbol: ?OnDraw@CEditView@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnDraw_CEditView__UEAAXPEAX_Z(
    CEditView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Edit control draws itself
}

// =============================================================================
// CListView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CListView@@QEAA@XZ
extern "C" void MS_ABI impl___0CListView__QEAA_XZ(CListView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pListCtrl = nullptr;
}

// Destructor
// Symbol: ??1CListView@@UEAA@XZ
extern "C" void MS_ABI impl___1CListView__UEAA_XZ(CListView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// GetListCtrl
// Symbol: ?GetListCtrl@CListView@@QEBAPEAVCListCtrl@@XZ
extern "C" CListCtrl* MS_ABI impl__GetListCtrl_CListView__QEBAPEAVCListCtrl__XZ(
    const CListView* pThis)
{
    return pThis ? pThis->m_pListCtrl : nullptr;
}

// OnDraw (CListView)
// Symbol: ?OnDraw@CListView@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnDraw_CListView__UEAAXPEAX_Z(
    CListView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // List control draws itself
}

// OnInitialUpdate (CListView)
// Symbol: ?OnInitialUpdate@CListView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CListView__UEAAXXZ(CListView* pThis) {
    impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
}

// =============================================================================
// CTreeView Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CTreeView@@QEAA@XZ
extern "C" void MS_ABI impl___0CTreeView__QEAA_XZ(CTreeView* pThis) {
    if (!pThis) return;
    impl___0CView__QEAA_XZ(pThis);
    pThis->m_pTreeCtrl = nullptr;
}

// Destructor
// Symbol: ??1CTreeView@@UEAA@XZ
extern "C" void MS_ABI impl___1CTreeView__UEAA_XZ(CTreeView* pThis) {
    impl___1CView__UEAA_XZ(pThis);
}

// GetTreeCtrl
// Symbol: ?GetTreeCtrl@CTreeView@@QEBAPEAVCTreeCtrl@@XZ
extern "C" CTreeCtrl* MS_ABI impl__GetTreeCtrl_CTreeView__QEBAPEAVCTreeCtrl__XZ(
    const CTreeView* pThis)
{
    return pThis ? pThis->m_pTreeCtrl : nullptr;
}

// OnDraw (CTreeView)
// Symbol: ?OnDraw@CTreeView@@UEAAXPEAX@Z
extern "C" void MS_ABI impl__OnDraw_CTreeView__UEAAXPEAX_Z(
    CTreeView* pThis, void* pDC)
{
    (void)pThis;
    (void)pDC;
    // Tree control draws itself
}

// OnInitialUpdate (CTreeView)
// Symbol: ?OnInitialUpdate@CTreeView@@UEAAXXZ
extern "C" void MS_ABI impl__OnInitialUpdate_CTreeView__UEAAXXZ(CTreeView* pThis) {
    impl__OnInitialUpdate_CView__UEAAXXZ(pThis);
}

// =============================================================================
// CDocTemplate Implementation
// =============================================================================

// Constructor
// Symbol: ??0CDocTemplate@@QEAA@IPEAUCRuntimeClass@@00@Z
extern "C" void MS_ABI impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    if (!pThis) return;
    pThis->m_nIDResource = nIDResource;
    pThis->m_pDocClass = pDocClass;
    pThis->m_pFrameClass = pFrameClass;
    pThis->m_pViewClass = pViewClass;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
}

// Destructor
// Symbol: ??1CDocTemplate@@UEAA@XZ
extern "C" void MS_ABI impl___1CDocTemplate__UEAA_XZ(CDocTemplate* pThis) {
    (void)pThis;
    // Note: Documents should be closed before template destruction
}

// CreateNewDocument
// Symbol: ?CreateNewDocument@CDocTemplate@@UEAAPEAVCDocument@@XZ
extern "C" CDocument* MS_ABI impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(
    CDocTemplate* pThis)
{
    if (!pThis || !pThis->m_pDocClass) return nullptr;

    CObject* pObj = pThis->m_pDocClass->CreateObject();
    if (!pObj) return nullptr;

    CDocument* pDoc = static_cast<CDocument*>(pObj);
    pThis->AddDocument(pDoc);
    return pDoc;
}

// CreateNewFrame
// Symbol: ?CreateNewFrame@CDocTemplate@@UEAAPEAVCFrameWnd@@PEAVCDocument@@PEAV2@@Z
extern "C" CFrameWnd* MS_ABI impl__CreateNewFrame_CDocTemplate__UEAAPEAVCFrameWnd__PEAVCDocument__PEAV2__Z(
    CDocTemplate* pThis, CDocument* pDoc, CFrameWnd* pOther)
{
    (void)pOther;
    if (!pThis || !pThis->m_pFrameClass) return nullptr;

    CObject* pObj = pThis->m_pFrameClass->CreateObject();
    if (!pObj) return nullptr;

    CFrameWnd* pFrame = static_cast<CFrameWnd*>(pObj);

    // Create view if we have a view class and document
    if (pThis->m_pViewClass && pDoc) {
        CObject* pViewObj = pThis->m_pViewClass->CreateObject();
        if (pViewObj) {
            CView* pView = static_cast<CView*>(pViewObj);
            pDoc->AddView(pView);
        }
    }

    return pFrame;
}

// OpenDocumentFile
// Symbol: ?OpenDocumentFile@CDocTemplate@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible)
{
    (void)bMakeVisible;
    if (!pThis) return nullptr;

    CDocument* pDoc = impl__CreateNewDocument_CDocTemplate__UEAAPEAVCDocument__XZ(pThis);
    if (!pDoc) return nullptr;

    if (lpszPathName) {
        if (!pDoc->OnOpenDocument(lpszPathName)) {
            pDoc->OnCloseDocument();
            return nullptr;
        }
        pDoc->SetPathName(lpszPathName);
    } else {
        if (!pDoc->OnNewDocument()) {
            pDoc->OnCloseDocument();
            return nullptr;
        }
    }

    return pDoc;
}

// AddDocument
// Symbol: ?AddDocument@CDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    pDoc->m_pNextDoc = nullptr;

    if (!pThis->m_pFirstDoc) {
        pThis->m_pFirstDoc = pDoc;
        pThis->m_pLastDoc = pDoc;
    } else {
        pThis->m_pLastDoc->m_pNextDoc = pDoc;
        pThis->m_pLastDoc = pDoc;
    }
}

// RemoveDocument
// Symbol: ?RemoveDocument@CDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    CDocument* pPrev = nullptr;
    CDocument* pCur = pThis->m_pFirstDoc;
    while (pCur) {
        if (pCur == pDoc) {
            if (pPrev) {
                pPrev->m_pNextDoc = pCur->m_pNextDoc;
            } else {
                pThis->m_pFirstDoc = pCur->m_pNextDoc;
            }
            if (pThis->m_pLastDoc == pDoc) {
                pThis->m_pLastDoc = pPrev;
            }
            pDoc->m_pNextDoc = nullptr;
            break;
        }
        pPrev = pCur;
        pCur = pCur->m_pNextDoc;
    }
}

// GetFirstDocPosition
// Symbol: ?GetFirstDocPosition@CDocTemplate@@UEBAPEAXXZ
extern "C" void* MS_ABI impl__GetFirstDocPosition_CDocTemplate__UEBAPEAXXZ(
    const CDocTemplate* pThis)
{
    return pThis ? pThis->m_pFirstDoc : nullptr;
}

// GetNextDoc
// Symbol: ?GetNextDoc@CDocTemplate@@UEBAPEAVCDocument@@AEAPEAX@Z
extern "C" CDocument* MS_ABI impl__GetNextDoc_CDocTemplate__UEBAPEAVCDocument__AEAPEAX_Z(
    const CDocTemplate* pThis, void*& pos)
{
    if (!pThis || !pos) return nullptr;

    CDocument* pDoc = static_cast<CDocument*>(pos);
    pos = pDoc->m_pNextDoc;
    return pDoc;
}

// InitialUpdateFrame
// Symbol: ?InitialUpdateFrame@CDocTemplate@@UEAAXPEAVCFrameWnd@@PEAVCDocument@@H@Z
extern "C" void MS_ABI impl__InitialUpdateFrame_CDocTemplate__UEAAXPEAVCFrameWnd__PEAVCDocument__H_Z(
    CDocTemplate* pThis, CFrameWnd* pFrame, CDocument* pDoc, int bMakeVisible)
{
    (void)pThis;
    (void)pFrame;

    if (pDoc) {
        // Call OnInitialUpdate on all views
        void* pos = impl__GetFirstViewPosition_CDocument__UEBAPEAXXZ(pDoc);
        while (pos) {
            CView* pView = impl__GetNextView_CDocument__UEBAPEAVCView__AEAPEAX_Z(pDoc, pos);
            if (pView) {
                pView->OnInitialUpdate();
            }
        }
    }

    if (bMakeVisible && pFrame && pFrame->m_hWnd) {
        ::ShowWindow(pFrame->m_hWnd, SW_SHOW);
        ::UpdateWindow(pFrame->m_hWnd);
    }
}

// SetDefaultTitle
// Symbol: ?SetDefaultTitle@CDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__SetDefaultTitle_CDocTemplate__UEAAXPEAVCDocument___Z(
    CDocTemplate* pThis, CDocument* pDoc)
{
    (void)pThis;
    if (pDoc) {
        pDoc->SetTitle(L"Untitled");
    }
}

// GetDocString
// Symbol: ?GetDocString@CDocTemplate@@UEBAHAEAVCString@@H@Z
extern "C" int MS_ABI impl__GetDocString_CDocTemplate__UEBAHAEAVCString__H_Z(
    const CDocTemplate* pThis, CString* rString, int nID)
{
    (void)pThis;
    (void)nID;
    if (rString) {
        rString->Empty();
    }
    return FALSE;  // String not found
}

// MatchDocType
// Symbol: ?MatchDocType@CDocTemplate@@UEAAHPEB_WAEAPEAVCDocument@@@Z
extern "C" int MS_ABI impl__MatchDocType_CDocTemplate__UEAAHPEB_WAEAPEAVCDocument___Z(
    CDocTemplate* pThis, const wchar_t* lpszPathName, CDocument*& rpDocMatch)
{
    (void)pThis;
    (void)lpszPathName;
    rpDocMatch = nullptr;
    return 0;  // No match
}

// =============================================================================
// CSingleDocTemplate Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CSingleDocTemplate@@QEAA@XZ
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_XZ(CSingleDocTemplate* pThis) {
    if (!pThis) return;
    pThis->m_nIDResource = 0;
    pThis->m_pDocClass = nullptr;
    pThis->m_pFrameClass = nullptr;
    pThis->m_pViewClass = nullptr;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
    pThis->m_pOnlyDoc = nullptr;
}

// Constructor with parameters
// Symbol: ??0CSingleDocTemplate@@QEAA@IPEAUCRuntimeClass@@00@Z
extern "C" void MS_ABI impl___0CSingleDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CSingleDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        pThis, nIDResource, pDocClass, pFrameClass, pViewClass);
    pThis->m_pOnlyDoc = nullptr;
}

// Destructor
// Symbol: ??1CSingleDocTemplate@@UEAA@XZ
extern "C" void MS_ABI impl___1CSingleDocTemplate__UEAA_XZ(CSingleDocTemplate* pThis) {
    impl___1CDocTemplate__UEAA_XZ(pThis);
}

// OpenDocumentFile (CSingleDocTemplate override)
// Symbol: ?OpenDocumentFile@CSingleDocTemplate@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CSingleDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CSingleDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible)
{
    if (!pThis) return nullptr;

    // In SDI, we reuse the existing document
    if (pThis->m_pOnlyDoc) {
        if (!pThis->m_pOnlyDoc->SaveModified()) {
            return nullptr;
        }
        pThis->m_pOnlyDoc->DeleteContents();

        if (lpszPathName) {
            if (!pThis->m_pOnlyDoc->OnOpenDocument(lpszPathName)) {
                return nullptr;
            }
            pThis->m_pOnlyDoc->SetPathName(lpszPathName);
        } else {
            if (!pThis->m_pOnlyDoc->OnNewDocument()) {
                return nullptr;
            }
        }

        return pThis->m_pOnlyDoc;
    }

    // Create new document
    return impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
        pThis, lpszPathName, bMakeVisible);
}

// AddDocument (CSingleDocTemplate override)
// Symbol: ?AddDocument@CSingleDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__AddDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(
    CSingleDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    pThis->m_pOnlyDoc = pDoc;
    impl__AddDocument_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}

// RemoveDocument (CSingleDocTemplate override)
// Symbol: ?RemoveDocument@CSingleDocTemplate@@UEAAXPEAVCDocument@@@Z
extern "C" void MS_ABI impl__RemoveDocument_CSingleDocTemplate__UEAAXPEAVCDocument___Z(
    CSingleDocTemplate* pThis, CDocument* pDoc)
{
    if (!pThis || !pDoc) return;

    if (pThis->m_pOnlyDoc == pDoc) {
        pThis->m_pOnlyDoc = nullptr;
    }
    impl__RemoveDocument_CDocTemplate__UEAAXPEAVCDocument___Z(pThis, pDoc);
}

// =============================================================================
// CMultiDocTemplate Implementation
// =============================================================================

// Default Constructor
// Symbol: ??0CMultiDocTemplate@@QEAA@XZ
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_XZ(CMultiDocTemplate* pThis) {
    if (!pThis) return;
    pThis->m_nIDResource = 0;
    pThis->m_pDocClass = nullptr;
    pThis->m_pFrameClass = nullptr;
    pThis->m_pViewClass = nullptr;
    pThis->m_pFirstDoc = nullptr;
    pThis->m_pLastDoc = nullptr;
}

// Constructor with parameters
// Symbol: ??0CMultiDocTemplate@@QEAA@IPEAUCRuntimeClass@@00@Z
extern "C" void MS_ABI impl___0CMultiDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
    CMultiDocTemplate* pThis, unsigned int nIDResource,
    CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
{
    impl___0CDocTemplate__QEAA_IPEAUCRuntimeClass__00_Z(
        pThis, nIDResource, pDocClass, pFrameClass, pViewClass);
}

// Destructor
// Symbol: ??1CMultiDocTemplate@@UEAA@XZ
extern "C" void MS_ABI impl___1CMultiDocTemplate__UEAA_XZ(CMultiDocTemplate* pThis) {
    impl___1CDocTemplate__UEAA_XZ(pThis);
}

// OpenDocumentFile (CMultiDocTemplate override)
// Symbol: ?OpenDocumentFile@CMultiDocTemplate@@UEAAPEAVCDocument@@PEB_WH@Z
extern "C" CDocument* MS_ABI impl__OpenDocumentFile_CMultiDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
    CMultiDocTemplate* pThis, const wchar_t* lpszPathName, int bMakeVisible)
{
    // MDI always creates a new document
    return impl__OpenDocumentFile_CDocTemplate__UEAAPEAVCDocument__PEB_WH_Z(
        pThis, lpszPathName, bMakeVisible);
}
