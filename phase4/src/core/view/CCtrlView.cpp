// CCtrlView — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// CCtrlView is not declared in include/openmfc (OpenMFC's CEditView/CListView/
// CTreeView/CRichEditView derive from CView directly), so these thunks take a
// raw pThis and address the two CCtrlView members by their retail offsets,
// pinned below.  Every body here was transcribed from the retail mfc140u.dll
// export (entry RVAs read from the mfc140u export table by ordinal); the
// deviations each body makes are stated in its own comment.

#include "detail/ManualSmallStubImplementationsSupport.h"

// Cross-file thunks (each definition located with grep; parameter lists derived
// from the mangled names / the existing definitions).
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                          // core/window/CtorDtorPlacement.cpp:22
extern "C" void MS_ABI impl___0CView__QEAA_XZ(CView* pThis);                         // detail/DocviewSupport.cpp:119 (internal helper, not an export)
extern "C" void MS_ABI impl___1CView__UEAA_XZ(CView* pThis);                         // core/view/CView.cpp:43
extern "C" int MS_ABI impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(    // core/view/CView.cpp:315
    CView* pThis, CREATESTRUCTW* cs);
extern "C" int MS_ABI impl__AfxEndDeferRegisterClass__YAHJ_Z(long fToRegister);      // featurepack/CMFC_misc_stubs.cpp:1209
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                 // core/window/Thunks.cpp:1183
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(              // core/runtime/CObject.cpp:49
    const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CRichEditView__SAPEAUCRuntimeClass__XZ(); // core/view/RuntimeClasses.cpp:275

namespace {

// Retail layout (mfc140u ctor ??0CCtrlView@@QEAA@PEB_WK@Z, RVA 0x277ec0):
//   CView base occupies [0, 0xf8)   (CView::m_pDocument +0xe8, m_bInitialRedraw +0xf0)
//   +0xf8  CString m_strClass
//   +0x100 DWORD   m_dwDefaultStyle
//   sizeof == 0x108 (264, the size the CCtrlView CRuntimeClass descriptor records
//   in core/view/RuntimeClasses.cpp).
constexpr size_t kCtrlViewStrClass     = 0xf8;
constexpr size_t kCtrlViewDefaultStyle = 0x100;

static_assert(sizeof(CView) == kCtrlViewStrClass, "OpenMFC CView must end where retail CCtrlView::m_strClass starts");
static_assert(offsetof(CView, m_pDocument) == 0xe8, "CView::m_pDocument at retail +0xe8");
static_assert(sizeof(CString) == 8, "CString is one pointer (m_pszData), as ATL CStringW");
static_assert(offsetof(CREATESTRUCTW, style) == 0x30, "CREATESTRUCTW::style");
static_assert(offsetof(CREATESTRUCTW, lpszClass) == 0x40, "CREATESTRUCTW::lpszClass");
// OpenMFC's CRichEditView derives from CView directly and embeds a child
// CRichEditCtrl here, where retail keeps CCtrlView::m_strClass/m_dwDefaultStyle.
static_assert(offsetof(CRichEditView, m_richEdit) == kCtrlViewStrClass,
              "OpenMFC CRichEditView::m_richEdit overlaps retail CCtrlView::m_strClass");

inline CString* CtrlViewStrClass(void* pThis) {
    return reinterpret_cast<CString*>(static_cast<unsigned char*>(pThis) + kCtrlViewStrClass);
}
inline DWORD* CtrlViewDefaultStyle(void* pThis) {
    return reinterpret_cast<DWORD*>(static_cast<unsigned char*>(pThis) + kCtrlViewDefaultStyle);
}

// AFX_WS_DEFAULT_VIEW == WS_CHILD | WS_VISIBLE | WS_BORDER (0x50800000).
constexpr DWORD kAfxWsDefaultView = WS_CHILD | WS_VISIBLE | WS_BORDER;

} // namespace

// CCtrlView::CCtrlView(LPCTSTR lpszClass, DWORD dwStyle)
// Retail RVA 0x277ec0 (mfc140u):
//   CView::CView(this)                       (call 0x277530 = ??0CView@@IEAA@XZ)
//   m_strClass (+0xf8) = nil string, then SetString(lpszClass, lpszClass ? wcslen(lpszClass) : 0)
//   m_dwDefaultStyle (+0x100) = dwStyle;  return this
// Retail's CView::CView (0x277530) is: CWnd::CWnd(this) (call 0x28a700 =
// ??0CWnd@@QEAA@XZ); m_pDocument (+0xe8) = NULL; m_bInitialRedraw (+0xf0) = 0.
// Deviation: OpenMFC's exported ??0CView@@IEAA@XZ thunk (core/view/CView.cpp:348)
// ignores `this` and heap-allocates a new object, so it cannot be used to
// construct a base subobject.  That body is inlined here instead: the CWnd
// placement-constructor thunk, then the internal CView field helper, which
// nulls m_hWnd (already null), m_pDocument (+0xe8) and the 8-byte OpenMFC
// m_pNextView at +0xf0 (a superset of retail's 4-byte m_bInitialRedraw store;
// +0xf4 is padding in retail).  m_strClass is built with OpenMFC's CString,
// which yields the same string value as retail's nil-then-SetString sequence
// for NULL, empty and non-empty lpszClass (neither performs CStringT's implicit
// MAKEINTRESOURCE load); the representation differs -- OpenMFC's empty string
// is its locked static nil, not the string manager's AddRef'd nil.
// Symbol: ??0CCtrlView@@QEAA@PEB_WK@Z
extern "C" void* MS_ABI impl___0CCtrlView__QEAA_PEB_WK_Z(
    void* pThis, const wchar_t* lpszClass, unsigned long dwStyle) {
    impl___0CWnd__QEAA_XZ(pThis);
    impl___0CView__QEAA_XZ(static_cast<CView*>(pThis));
    new (CtrlViewStrClass(pThis)) CString(lpszClass);
    *CtrlViewDefaultStyle(pThis) = dwStyle;
    return pThis;
}

// CCtrlView::~CCtrlView()
// Retail RVA 0x277e80 (mfc140u):
//   release m_strClass's CStringData (+0xf8 points 0x18 past the header):
//     if (InterlockedDecrement(&hdr->nRefs) <= 0) hdr->pStringMgr->Free(hdr)  (mgr vslot 1)
//   then tail-jump to CView::~CView (0x277560 = ??1CView@@UEAA@XZ).
// The release is done through OpenMFC's CString destructor.  Deviations: that
// destructor skips locked data (nRefs < 0), because OpenMFC's nil string is
// locked with nRefs == -1 rather than ATL's refcounted nil, where retail
// decrements unconditionally; and its decrement is a plain --nRefs, not
// retail's `lock xadd`.  The final free is pStringMgr->Free in both.
// OpenMFC's ~CView thunk (core/view/CView.cpp:43) only detaches the view from
// its document; unlike retail ~CView it does not go on to run ~CWnd.
// Symbol: ??1CCtrlView@@UEAA@XZ
extern "C" void MS_ABI impl___1CCtrlView__UEAA_XZ(void* pThis) {
    CtrlViewStrClass(pThis)->~CString();
    impl___1CView__UEAA_XZ(static_cast<CView*>(pThis));
}

// void CCtrlView::OnDraw(CDC*)
// The export's entry (ordinal 9218) is RVA 0x27d0 (mfc140u), a shared
// identical-code-folded body consisting of a single `ret` -- retail's
// ASSERT(FALSE) compiles to nothing in the release DLL.  The same address sits
// in CTreeView's vftable (0x180331e38, mfc140u) at slot 105 (byte offset 0x348),
// the CView::OnDraw slot.  The empty body below is the transcription.
// Symbol: ?OnDraw@CCtrlView@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__OnDraw_CCtrlView__MEAAXPEAVCDC___Z(void* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
}

// void CCtrlView::OnPaint()
// The export's entry (ordinal 10726) is RVA 0xda30 (mfc140u), a folded body
// that is exactly `jmp 0x28ac80` = CWnd::Default() (?Default@CWnd@@IEAA_JXZ).
// The CCtrlView message map (0x180332640, mfc140u) routes WM_PAINT (0x000f) to
// the same 0xda30, so the view HWND's own control paints itself instead of
// CView::OnPaint's BeginPaint/OnDraw path.
// Symbol: ?OnPaint@CCtrlView@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CCtrlView__IEAAXXZ(void* pThis) {
    impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// LRESULT CCtrlView::OnPrintClient(CDC* pDC, UINT nFlags)
// The export's entry (ordinal 10837) is the same folded RVA 0xda30 (mfc140u),
// `jmp CWnd::Default` -- pDC and nFlags are ignored and Default()'s LRESULT is
// returned.  The message map routes WM_PRINTCLIENT (0x0318) there as well.
// Symbol: ?OnPrintClient@CCtrlView@@IEAA_JPEAVCDC@@I@Z
extern "C" __int64 MS_ABI impl__OnPrintClient_CCtrlView__IEAA_JPEAVCDC__I_Z(
    void* pThis, CDC* pDC, unsigned int nFlags) {
    (void)pDC;
    (void)nFlags;
    return impl__Default_CWnd__IEAA_JXZ(static_cast<CWnd*>(pThis));
}

// BOOL CCtrlView::PreCreateWindow(CREATESTRUCT& cs)
// Retail RVA 0x277f40 (mfc140u):
//   cs.lpszClass (+0x40) = m_strClass (+0xf8 data pointer);
//   AfxEndDeferRegisterClass(0x10);        (call 0x2918f0, AFX_WNDCOMMCTLS_REG, result ignored)
//   AfxEndDeferRegisterClass(0xfc000);     (call 0x2918f0, result ignored)
//   if ((cs.style | WS_BORDER) == AFX_WS_DEFAULT_VIEW)
//       cs.style = m_dwDefaultStyle (+0x100) & (cs.style | ~WS_BORDER);
//   tail-jump CView::PreCreateWindow(cs)   (0x277610 = ?PreCreateWindow@CView@@MEAAHAEAUtagCREATESTRUCTW@@@Z)
// (mfc140.dll's ANSI twin passes 0x3c000 as the second mask; the Unicode image
// passes 0xfc000, transcribed here.)  OpenMFC's AfxEndDeferRegisterClass
// registers its single window class regardless of the mask.
// Deviation (OpenMFC-only guard, no retail counterpart): the one in-tree
// caller, CRichEditView::PreCreateWindow (core/view/CRichEditView.cpp), passes
// an OpenMFC CRichEditView, which is NOT CCtrlView-shaped -- +0xf8 is the
// embedded m_richEdit (asserted above), so reading it as m_strClass would put
// a vftable pointer into cs.lpszClass and CreateWindowEx would fail.  Every
// CRichEditView in OpenMFC has that layout (its ctor thunk, core/view/
// Thunks.cpp:1128, placement-constructs the header class), so for those
// objects the two member-dependent steps are skipped.  Objects built by the
// ??0CCtrlView export above (a retail-header client's CTreeView/CListView,
// whose inline ctors call it) take the full retail path.
// Symbol: ?PreCreateWindow@CCtrlView@@MEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CCtrlView__MEAAHAEAUtagCREATESTRUCTW___Z(
    void* pThis, CREATESTRUCTW* cs) {
    const bool bCtrlViewLayout = !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
        static_cast<const CObject*>(static_cast<CView*>(pThis)),
        impl__GetThisClass_CRichEditView__SAPEAUCRuntimeClass__XZ());
    if (bCtrlViewLayout) {
        cs->lpszClass = CtrlViewStrClass(pThis)->GetString();
    }
    impl__AfxEndDeferRegisterClass__YAHJ_Z(0x10);
    impl__AfxEndDeferRegisterClass__YAHJ_Z(0xfc000);
    if (bCtrlViewLayout && (cs->style | WS_BORDER) == kAfxWsDefaultView) {
        cs->style = *CtrlViewDefaultStyle(pThis) & (cs->style | ~static_cast<DWORD>(WS_BORDER));
    }
    return impl__PreCreateWindow_CView__MEAAHAEAUtagCREATESTRUCTW___Z(static_cast<CView*>(pThis), cs);
}
