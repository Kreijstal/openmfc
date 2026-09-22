// CMFCToolBarButtonsListButton — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// ---------------------------------------------------------------------------
// Object model (read before touching any body)
// ---------------------------------------------------------------------------
// OpenMFC has no C++ declaration of CMFCToolBarButtonsListButton.  The retail
// declaration is afxtoolbarbuttonslistbutton.h (MSVC 14.51):
//     class CMFCToolBarButtonsListButton : public CButton
// and the instance layout below was read out of the retail constructor
// ??0CMFCToolBarButtonsListButton@@QEAA@XZ (RVA 0x15ea60, mfc140.dll) instruction
// by instruction; every offset cited here is a store or load in that body:
//
//   +0x000  CButton subobject (== CWnd, 0xe8 bytes; the ctor calls CWnd::CWnd
//           at 0x288b10 on `this` and then stores the class vftable)
//   +0x0e8  CObList m_Buttons        (CObject vfptr +0x0e8, m_pNodeHead +0x0f0,
//                                     m_pNodeTail +0x0f8, m_nCount +0x100,
//                                     m_pNodeFree +0x108, m_pBlocks +0x110,
//                                     m_nBlockSize +0x118 -- the ctor stores 10)
//   +0x120  CMFCToolBarImages* m_pImages
//   +0x128  CSize m_sizeButton       (cx +0x128, cy +0x12c)
//   +0x130  CScrollBar m_wndScrollBar (embedded, 0xe8 bytes; the ctor calls
//                                     CWnd::CWnd on this+0x130 and stores the
//                                     CScrollBar vftable; m_hWnd at +0x170)
//   +0x218  CMFCToolBarButton* m_pSelButton
//   +0x220  int  m_iScrollOffset
//   +0x224  int  m_iScrollTotal
//   +0x228  int  m_iScrollPage
//   +0x22c  BOOL m_bInited
//   +0x230  BOOL m_bEnableDragFromList
//   sizeof  0x238
// The member NAMES are the retail header's, in the retail header's declaration
// order; the OFFSETS are the ones the disassembly uses.  All of it is pinned by
// the static_asserts on ListBtn below.
//
// CMFCToolBarButton members this file reads (declared by OpenMFC at the retail
// offsets, see include/openmfc/afxmfc.h and the static_asserts below):
// m_bUserButton +0x08, m_bDragFromCollection +0x20, m_nStyle +0x28,
// m_iImage +0x48, m_iUserImage +0x4c, m_bLocked +0x50, m_rect +0x68.
// CMFCToolBarImages::m_sizeImage is at +0x68 and m_clrTransparent at +0xd8.
//
// Vtable slots of CMFCToolBarButton named below were derived from the retail
// call sites and cross-checked against the declaration order of the on-host
// MSVC 14.51 afxtoolbarbutton.h (CObject contributes five slots:
// GetRuntimeClass, ~CObject, Serialize, AssertValid, Dump):
//     +0x28 (5)  PrepareDrag        +0x40 (8)  OnDraw
//     +0x48 (9)  OnCalculateSize    +0x60 (12) OnChangeParentWnd
//     +0x70 (14) OnMove             +0x80 (16) GetHwnd
//     +0xa0 (20) OnCtlColor
//
// DEVIATIONS from retail, all of them forced by the OpenMFC object model and
// each one repeated at the function it affects:
//   * OpenMFC's CObList keeps its elements in a side table keyed by the list
//     object (include/openmfc/afx.h, detail/FilecoreSupport.h
//     OPENMFC_DEFINE_LIST_METHODS) and never maintains the retail m_pNodeHead /
//     m_nCount members.  Retail walks those members inline; here the list is
//     walked through the exported ?FindIndex@CObList@@ thunk plus the read-only
//     node mirror below -- the same pattern featurepack/visualmanager/
//     CMFCVisualManager.cpp and docking/CDockingPanesRow.cpp use.  A client that
//     reads m_Buttons back through its own inlined CObList::GetCount() /
//     GetHeadPosition() sees an empty list; that is a property of the CObList
//     model, not of these bodies.
//   * OpenMFC's CMFCToolBarButton declares no virtuals beyond CObject's, so a
//     button's own vtable cannot be dispatched through.  Every button virtual
//     retail calls is issued here as a direct call to the exported BASE-class
//     thunk, so an override in a derived button class is NOT reached.
//   * GetHwnd (slot 16) and OnCtlColor (slot 20) are inline virtuals in
//     afxtoolbarbutton.h -- `return NULL` both -- so they have no export and no
//     thunk.  Where retail branches on them this file uses the base result
//     (NULL) and says so.
//   * Retail guards several list walks with ENSURE(pButton != NULL), which calls
//     AfxThrowInvalidArgException and does not return.  Every such site here
//     returns (or breaks) instead -- the surrounding loop cannot continue with a
//     NULL element either way -- and the same goes for the ENSURE_VALID(m_pImages)
//     in AddButton and the ENSURE on the list tail in RebuildLocations.
//   * The class' AFX_MSGMAP (detail/Toolbar21MsgmapSupport.cpp) is currently the
//     shared EMPTY entry table, so none of the WM_ handlers below is reachable
//     from OpenMFC's message pump yet.  Reported as a header request.
//
// Every RVA quoted in this file is an mfc140.dll (ANSI twin) address, named as
// such at each site.  Function bodies are byte-identical between mfc140.dll and
// mfc140u.dll, so the control flow, member offsets and constants transcribed
// from them are correct for mfc140u; the RVAs themselves are NOT mfc140u RVAs.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstring>

//---------------------------------------------------------------------------
// Thunks for everything this file calls.  A C++ method written directly here
// would compile and fail to link (AGENTS.md / briefing §1).
//---------------------------------------------------------------------------
extern "C" void*    MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                            // core/window/CtorDtorPlacement.cpp
extern "C" void     MS_ABI impl___1CButton__UEAA_XZ(void* pThis);                          // core/controls/CtorDtorPlacement.cpp
extern "C" void     MS_ABI impl___1CScrollBar__UEAA_XZ(CScrollBar* pThis);                 // core/controls/RuntimeClasses.cpp
extern "C" int      MS_ABI impl__Create_CScrollBar__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
    CScrollBar* pThis, DWORD dwStyle, const RECT* pRect, CWnd* pParentWnd, UINT nID);      // detail/CtrlcoreSupport.cpp

extern "C" void*    MS_ABI impl___0CObList__QEAA__J_Z(CObList* pThis, long long nBlockSize);
extern "C" void     MS_ABI impl___1CObList__UEAA_XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(CObList* pThis, CObject* value);
extern "C" CObject* MS_ABI impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(CObList* pThis);
extern "C" CObList::POSITION MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const CObList* pThis, long long nIndex);

extern "C" __int64  MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                      // core/window/Thunks.cpp
extern "C" void*    MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                 // core/window/Thunks.cpp
extern "C" int      MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);            // core/window/Thunks.cpp
extern "C" CWnd*    MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" int      MS_ABI impl__IsWindowEnabled_CWnd__QEBAHXZ(const CWnd* pThis);         // core/window/CWnd.cpp
extern "C" int      MS_ABI impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(CWnd* pThis, LONGLONG* pResult);
extern "C" int      MS_ABI impl__SetScrollPos_CWnd__QEAAHHHH_Z(CWnd* pThis, int nBar, int nPos, int bRedraw);
extern "C" void     MS_ABI impl__SetScrollRange_CWnd__QEAAXHHHH_Z(CWnd* pThis, int nBar, int nMinPos, int nMaxPos, int bRedraw);
extern "C" void     MS_ABI impl__EnableScrollBarCtrl_CWnd__QEAAXHH_Z(CWnd* pThis, int nBar, int bEnable);

extern "C" CDC*     MS_ABI impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(HDC hDC);           // core/gdi/CDC.cpp
extern "C" void     MS_ABI impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(CDC* pThis, const RECT* lpRect, unsigned long clr);
extern "C" CClientDC* MS_ABI impl___0CClientDC__QEAA_PEAVCWnd___Z(CClientDC* pThis, CWnd* pWnd);  // core/gdi/CClientDC.cpp
extern "C" void     MS_ABI impl___1CClientDC__UEAA_XZ(CClientDC* pThis);                   // core/gdi/CClientDC.cpp

extern "C" void     MS_ABI impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
// CSize is passed BY VALUE (8 bytes in a register) -- modelled as a long long,
// exactly as featurepack/controls/CMFCReBar.cpp declares it.
extern "C" int      MS_ABI impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(
    CMFCToolBarImages* pThis, void* pDS, long long sizeImageDest, int bFadeInactive);
extern "C" void     MS_ABI impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(CMFCToolBarImages* pThis, void* pDS);

// CMFCToolBarButton base-class virtuals.  The definitions of OnChangeParentWnd,
// PrepareDrag and OnCalculateSize in featurepack/toolbar/CMFCToolBarButton.cpp
// are still auto-generated placeholders whose parameter lists do not match the
// mangled name (they have no `this`); these are the lists the mangled names
// describe, and they are what featurepack/toolbar/CMFCToolBar.cpp and
// CMFCToolBarComboBoxButton.cpp already declare.  Reported as header requests.
extern "C" void     MS_ABI impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(CMFCToolBarButton* pThis, CWnd* pWndParent);
extern "C" int      MS_ABI impl__PrepareDrag_CMFCToolBarButton__UEAAHAEAVCOleDataSource___Z(CMFCToolBarButton* pThis, COleDataSource* pSrcItem);
extern "C" SIZE*    MS_ABI impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
    CMFCToolBarButton* pThis, SIZE* pRet, CDC* pDC, const SIZE* pSizeDefault, int bHorz);
extern "C" void     MS_ABI impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
    CMFCToolBarButton* pThis, CDC* pDC, const RECT* pRect, CMFCToolBarImages* pImages,
    int bHorz, int bCustomizeMode, int bHighlight, int bDrawBorder, int bGrayDisabledButtons);

extern "C" void*    MS_ABI impl___0COleDataSource__QEAA_XZ(void* pThis);
extern "C" void     MS_ABI impl___1COleDataSource__UEAA_XZ(void* pThis);
extern "C" unsigned long MS_ABI impl__DoDragDrop_COleDataSource__QEAAKKPEBUtagRECT__PEAVCOleDropSource___Z(
    COleDataSource* pThis, unsigned long dwEffects, const RECT* lpRectStartDrag, COleDropSource* pDropSource);

extern "C" void*    MS_ABI impl___0CMFCToolBarComboBoxButton__QEAA_XZ(void* pThis);        // toolbar/Thunks.cpp
extern "C" void     MS_ABI impl___1CMFCToolBarComboBoxButton__UEAA_XZ(void* pThis);        // toolbar/Thunks.cpp
extern "C" void     MS_ABI impl__OnDrawComboBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
    CMFCVisualManager* pThis, CDC* pDC, CRect rect, int bDisabled, int bIsDropped, int bIsHighlighted,
    CMFCToolBarComboBoxButton* pButton);                                                   // visualmanager/Thunks.cpp
extern "C" void*    impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                      // core/runtime/StaticData.cpp

extern "C" void     MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                   // featurepack/CMFC_misc_stubs.cpp

//---------------------------------------------------------------------------
// Forward declarations of this file's own exports (they call each other exactly
// as the retail bodies do).
//---------------------------------------------------------------------------
extern "C" void MS_ABI impl__RemoveButtons_CMFCToolBarButtonsListButton__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RebuildLocations_CMFCToolBarButtonsListButton__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__RedrawSelection_CMFCToolBarButtonsListButton__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(
    void* pThis, CMFCToolBarButton* pButton);
extern "C" CMFCToolBarButton* MS_ABI impl__HitTest_CMFCToolBarButtonsListButton__IEBAPEAVCMFCToolBarButton__UtagPOINT___Z(
    const void* pThis, long long ptPacked);


namespace {

//---------------------------------------------------------------------------
// Instance layout (see the header comment).
//---------------------------------------------------------------------------
struct ListBtn {
    char               _base[0xe8];          // 0x000 CButton (== CWnd) subobject
    CObList            m_Buttons;            // 0x0e8
    CMFCToolBarImages* m_pImages;            // 0x120
    CSize              m_sizeButton;         // 0x128
    CScrollBar         m_wndScrollBar;       // 0x130
    CMFCToolBarButton* m_pSelButton;         // 0x218
    int                m_iScrollOffset;      // 0x220
    int                m_iScrollTotal;       // 0x224
    int                m_iScrollPage;        // 0x228
    int                m_bInited;            // 0x22c
    int                m_bEnableDragFromList;// 0x230
};

static_assert(sizeof(CWnd) == 0xe8, "OpenMFC's CWnd is the retail 0xe8-byte CWnd");
static_assert(sizeof(CScrollBar) == sizeof(CWnd), "OpenMFC's CScrollBar adds no members to CWnd");
static_assert(sizeof(CObList) == 0x38, "retail CObList is 56 bytes");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd");
static_assert(offsetof(ListBtn, m_Buttons) == 0x0e8, "m_Buttons (ctor 0x15ea8e stores the CObList vfptr there)");
static_assert(offsetof(ListBtn, m_pImages) == 0x120, "m_pImages (ctor 0x15eaec)");
static_assert(offsetof(ListBtn, m_sizeButton) == 0x128, "m_sizeButton (SetImages 0x15ef9d/0x15efae)");
static_assert(offsetof(ListBtn, m_wndScrollBar) == 0x130, "m_wndScrollBar (ctor 0x15eacc)");
static_assert(offsetof(ListBtn, m_pSelButton) == 0x218, "m_pSelButton (ctor 0x15eae5)");
static_assert(offsetof(ListBtn, m_iScrollOffset) == 0x220, "m_iScrollOffset (ctor 0x15eaf3)");
static_assert(offsetof(ListBtn, m_iScrollTotal) == 0x224, "m_iScrollTotal (RebuildLocations 0x15f7d5)");
static_assert(offsetof(ListBtn, m_iScrollPage) == 0x228, "m_iScrollPage (RebuildLocations 0x15f7dc)");
static_assert(offsetof(ListBtn, m_bInited) == 0x22c, "m_bInited (RebuildLocations 0x15f7f6)");
static_assert(offsetof(ListBtn, m_bEnableDragFromList) == 0x230, "m_bEnableDragFromList (ctor 0x15eb01)");
static_assert(sizeof(ListBtn) == 0x238, "retail CMFCToolBarButtonsListButton is 0x238 bytes");

// CMFCToolBarButton members the retail bodies touch by offset.
static_assert(offsetof(CMFCToolBarButton, m_bUserButton) == 0x08, "m_bUserButton (GetImage, 0x15f39e)");
static_assert(offsetof(CMFCToolBarButton, m_bDragFromCollection) == 0x20, "m_bDragFromCollection (OnLButtonDown 0x15ef3f)");
static_assert(offsetof(CMFCToolBarButton, m_nStyle) == 0x28, "m_nStyle (DrawItem 0x15edb0)");
static_assert(offsetof(CMFCToolBarButton, m_iImage) == 0x48, "m_iImage (GetImage, 0x15f3aa)");
static_assert(offsetof(CMFCToolBarButton, m_iUserImage) == 0x4c, "m_iUserImage (GetImage, 0x15f3aa)");
static_assert(offsetof(CMFCToolBarButton, m_bLocked) == 0x50, "m_bLocked (DrawItem 0x15edb4)");
static_assert(offsetof(CMFCToolBarButton, m_rect) == 0x68, "m_rect (HitTest 0x15f11a, RebuildLocations 0x15f6e7)");
static_assert(sizeof(CMFCToolBarComboBoxButton) == 0x150, "retail CMFCToolBarComboBoxButton is 336 bytes");

// CMFCToolBarImages: m_sizeImage +0x68 (SetImages 0x15ef97) and m_clrTransparent
// +0xd8 (DrawItem 0x15ed25).  Both are protected in include/openmfc/afxmfc.h, so
// they are reached by offset; the header's own layout comment names them at
// exactly these offsets and the class size pins the block.
constexpr int kOffImagesSizeImage     = 0x68;
constexpr int kOffImagesClrTransparent = 0xd8;
static_assert(sizeof(CMFCToolBarImages) == 408, "retail CMFCToolBarImages is 408 bytes");
// Both members are protected, so the assertion is made from a derived probe type
// (it never gets instantiated) rather than trusted from the header comment.
struct ImagesOffsetProbe : public CMFCToolBarImages {
    static void PinOffsets() {
        static_assert(offsetof(ImagesOffsetProbe, m_sizeImage) == kOffImagesSizeImage,
                      "CMFCToolBarImages::m_sizeImage (SetImages reads +0x68)");
        static_assert(offsetof(ImagesOffsetProbe, m_clrTransparent) == kOffImagesClrTransparent,
                      "CMFCToolBarImages::m_clrTransparent (DrawItem reads/writes +0xd8)");
    }
};

// afxGlobalData colour slots, pinned against ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ
// (RVA 0x6afd0, mfc140.dll): +0x28 <- GetSysColor(COLOR_BTNFACE/15) at 0x6b066,
// +0x78 <- GetSysColor(COLOR_WINDOW/5) at 0x6b104.
constexpr int kGlobalDataInitGate   = 0x000;
constexpr int kGlobalDataClrBtnFace = 0x028;
constexpr int kGlobalDataClrWindow  = 0x078;

// The retail one-time gate, exactly as DrawItem (0x15ed00) and OnEraseBkgnd
// (0x15ec12 / 0x15ec3a) inline it (0x6a5c0 is ?Initialize@AFX_GLOBAL_DATA@@):
//     if (afxGlobalData.<+0> == 0) { afxGlobalData.Initialize(); afxGlobalData.<+0> = 1; }
void EnsureGlobalData()
{
    int gate = 0;
    std::memcpy(&gate, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, sizeof gate);
    if (gate == 0) {
        impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A);
        const int one = 1;
        std::memcpy(impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + kGlobalDataInitGate, &one, sizeof one);
    }
}

COLORREF GlobalDataColor(int off)
{
    EnsureGlobalData();
    COLORREF clr = 0;
    std::memcpy(&clr, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + off, sizeof clr);
    return clr;
}

//---------------------------------------------------------------------------
// Read-only mirror of CList<CObject*,CObject*>::CNode (include/openmfc/afx.h),
// the node a CObList::POSITION points at.  GetHeadPosition / GetNext are inline
// in retail MFC, so they have neither an export nor a thunk; the exported
// FindIndex thunk plus this view is how a CObList is walked from another
// translation unit.
//---------------------------------------------------------------------------
struct ObNode {
    ObNode*  pNext;
    ObNode*  pPrev;
    CObject* data;
};
static_assert(sizeof(CObList::POSITION) == sizeof(void*), "POSITION is one pointer");

ObNode* NodeFromPos(CObList::POSITION pos)
{
    ObNode* p = nullptr;
    std::memcpy(&p, &pos, sizeof p);
    return p;
}

ObNode* ListHead(const CObList* pList)
{
    return NodeFromPos(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(pList, 0));
}

ObNode* ListTail(const CObList* pList)
{
    ObNode* pLast = nullptr;
    for (ObNode* p = ListHead(pList); p != nullptr; p = p->pNext) pLast = p;
    return pLast;
}

// Stands in for the inline CObList::GetCount() retail reads at +0x100.
long long ListCount(const CObList* pList)
{
    long long n = 0;
    for (ObNode* p = ListHead(pList); p != nullptr; p = p->pNext) ++n;
    return n;
}

// OpenMFC's CRect is a standalone class (include/openmfc/afxwin.h), not a RECT
// derivative, so a button's m_rect is copied field by field.
RECT ToRect(const CRect& r)
{
    RECT t;
    t.left = r.left; t.top = r.top; t.right = r.right; t.bottom = r.bottom;
    return t;
}

ListBtn*   Self(void* pThis)        { return static_cast<ListBtn*>(pThis); }
const ListBtn* Self(const void* pThis) { return static_cast<const ListBtn*>(pThis); }
CWnd*      AsWnd(void* pThis)       { return reinterpret_cast<CWnd*>(pThis); }
const CWnd* AsWnd(const void* pThis){ return reinterpret_cast<const CWnd*>(pThis); }
HWND       Hwnd(const void* pThis)  { return AsWnd(pThis)->m_hWnd; }

// CMFCToolBarButton::GetImage(), inlined by retail at 0x15f39e..0x15f3aa as
//     *(int*)((char*)pButton + 0x48 + (m_bUserButton ? 4 : 0))
int ButtonImage(const CMFCToolBarButton* p)
{
    return p->m_bUserButton ? p->m_iUserImage : p->m_iImage;
}

} // namespace

// Symbol: ??0CMFCToolBarButtonsListButton@@QEAA@XZ
// Retail (RVA 0x15ea60, mfc140.dll), fully transcribed:
//     CWnd::CWnd();                            // 0x288b10, on `this`
//     <store class vftable>                    // not reproducible in OpenMFC
//     m_Buttons.CObList(10);                   // the ctor inlines it: vfptr,
//                                              // head/tail/count/free/blocks = 0,
//                                              // m_nBlockSize = 10
//     m_sizeButton = CSize(0, 0);
//     CWnd::CWnd() on &m_wndScrollBar;         // + CScrollBar vftable
//     m_pSelButton = NULL; m_pImages = NULL;
//     m_iScrollOffset = m_iScrollTotal = m_iScrollPage = 0;
//     m_bInited = 0; m_bEnableDragFromList = 0;
// NOTE: this export was not on this file's work list (its previous body just
// returned pThis), but the whole class depends on m_Buttons and m_wndScrollBar
// being constructed: the destructor below destroys both through thunks whose
// explicit `->~T()` calls dispatch through the subobject's vptr, which is only
// valid once the constructor has run -- so it is implemented here.
// DEVIATION: OpenMFC cannot install the retail vftables; CWnd::CWnd leaves the
// OpenMFC CWnd vptr in both subobjects (the same approach
// ribbon/CMFCRibbonPanelMenuBar.cpp takes for its embedded CScrollBar).
extern "C" void* MS_ABI impl___0CMFCToolBarButtonsListButton__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    ListBtn* p = Self(pThis);
    impl___0CWnd__QEAA_XZ(pThis);
    impl___0CObList__QEAA__J_Z(&p->m_Buttons, 10);
    p->m_pImages = nullptr;
    p->m_sizeButton.cx = 0;
    p->m_sizeButton.cy = 0;
    impl___0CWnd__QEAA_XZ(&p->m_wndScrollBar);
    p->m_pSelButton = nullptr;
    p->m_iScrollOffset = 0;
    p->m_iScrollTotal = 0;
    p->m_iScrollPage = 0;
    p->m_bInited = 0;
    p->m_bEnableDragFromList = 0;
    return pThis;
}

// Symbol: ??1CMFCToolBarButtonsListButton@@UEAA@XZ
// Retail (RVA 0x15eb70, mfc140.dll), fully transcribed:
//     <restore class vftable>
//     m_wndScrollBar.~CScrollBar();            // 0x2925b0
//     m_Buttons.~CObList();                    // inlined: CObList vftable +
//                                              // CPtrList::RemoveAll (0x83d0)
//     CButton::~CButton();                     // tail jump to 0x292040
// RemoveAll only frees the list nodes -- retail does NOT delete the buttons, and
// neither does this body.
extern "C" void MS_ABI impl___1CMFCToolBarButtonsListButton__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    impl___1CScrollBar__UEAA_XZ(&p->m_wndScrollBar);
    impl___1CObList__UEAA_XZ(&p->m_Buttons);
    impl___1CButton__UEAA_XZ(pThis);
}

// Symbol: ?SetImages@CMFCToolBarButtonsListButton@@QEAAXPEAVCMFCToolBarImages@@@Z
// Retail (RVA 0x15ef90, mfc140.dll), fully transcribed -- five stores and a tail
// jump, with no NULL test on pImages:
//     m_pImages = pImages;
//     m_sizeButton.cx = pImages->m_sizeImage.cx + 6;   // 0x68
//     m_sizeButton.cy = pImages->m_sizeImage.cy + 7;   // 0x6c
//     RemoveButtons();                                  // jmp 0x15f040
// DEVIATION: pImages is NULL-checked here (retail would fault).
extern "C" void MS_ABI impl__SetImages_CMFCToolBarButtonsListButton__QEAAXPEAVCMFCToolBarImages___Z(
    void* pThis, CMFCToolBarImages* pImages) {
    if (!pThis || !pImages) return;
    ListBtn* p = Self(pThis);
    p->m_pImages = pImages;
    SIZE sizeImage = { 0, 0 };
    std::memcpy(&sizeImage, reinterpret_cast<const char*>(pImages) + kOffImagesSizeImage, sizeof sizeImage);
    p->m_sizeButton.cx = sizeImage.cx + 6;
    p->m_sizeButton.cy = sizeImage.cy + 7;
    impl__RemoveButtons_CMFCToolBarButtonsListButton__QEAAXXZ(pThis);
}

// Symbol: ?AddButton@CMFCToolBarButtonsListButton@@QEAAXPEAVCMFCToolBarButton@@@Z
// Retail (RVA 0x15efc0, mfc140.dll), fully transcribed:
//     if (m_pImages == NULL) AfxThrowInvalidArgException();   // ENSURE_VALID
//     m_Buttons.AddTail(pButton);                             // 0x230490
//     pButton->OnChangeParentWnd(this);                       // vslot +0x60
//     RebuildLocations();                                     // 0x15f5c0
//     HWND h = pButton->GetHwnd();                            // vslot +0x80
//     if (h != NULL) ::EnableWindow(h, FALSE);
// DEVIATIONS: the ENSURE becomes an early return; OnChangeParentWnd goes to the
// exported base thunk (no derived override); GetHwnd is a non-exported inline
// virtual whose base result is NULL, so the ::EnableWindow call is unreachable
// here and is therefore not written.  Added guard: pButton is NULL-checked here;
// retail has no test and would fault on the OnChangeParentWnd vcall.
extern "C" void MS_ABI impl__AddButton_CMFCToolBarButtonsListButton__QEAAXPEAVCMFCToolBarButton___Z(
    void* pThis, CMFCToolBarButton* pButton) {
    if (!pThis || !pButton) return;
    ListBtn* p = Self(pThis);
    if (p->m_pImages == nullptr) return;
    impl__AddTail_CObList__QEAAPEAU__POSITION__PEAVCObject___Z(&p->m_Buttons, pButton);
    impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(pButton, AsWnd(pThis));
    impl__RebuildLocations_CMFCToolBarButtonsListButton__IEAAXXZ(pThis);
}

// Symbol: ?RemoveButtons@CMFCToolBarButtonsListButton@@QEAAXXZ
// Retail (RVA 0x15f040, mfc140.dll), fully transcribed:
//     SelectButton((CMFCToolBarButton*)NULL);            // 0x15f180
//     while (m_Buttons.GetCount() != 0)                   // +0x100
//         ((CMFCToolBarButton*)m_Buttons.RemoveHead())->OnChangeParentWnd(NULL); // 0x230560, vslot +0x60
//     m_iScrollOffset = 0; m_iScrollTotal = 0;            // one qword store at +0x220
//     m_iScrollPage = 0;
//     EnableScrollBarCtrl(SB_VERT, FALSE);                // 0x28cca0
//     SetScrollRange(SB_VERT, 0, 0, TRUE);                // 0x28cbe0
extern "C" void MS_ABI impl__RemoveButtons_CMFCToolBarButtonsListButton__QEAAXXZ(void* pThis) {
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(pThis, nullptr);
    while (ListCount(&p->m_Buttons) != 0) {
        CObject* pObj = impl__RemoveHead_CObList__QEAAPEAVCObject__XZ(&p->m_Buttons);
        if (pObj == nullptr) break;   // added guard: retail vcalls on it unchecked
        impl__OnChangeParentWnd_CMFCToolBarButton__UEAAXPEAVCWnd___Z(
            static_cast<CMFCToolBarButton*>(pObj), nullptr);
    }
    p->m_iScrollOffset = 0;
    p->m_iScrollTotal = 0;
    p->m_iScrollPage = 0;
    impl__EnableScrollBarCtrl_CWnd__QEAAXHH_Z(AsWnd(pThis), SB_VERT, FALSE);
    impl__SetScrollRange_CWnd__QEAAXHHHH_Z(AsWnd(pThis), SB_VERT, 0, 0, TRUE);
}

// Symbol: ?HitTest@CMFCToolBarButtonsListButton@@IEBAPEAVCMFCToolBarButton@@UtagPOINT@@@Z
// Retail (RVA 0x15f0d0, mfc140.dll), fully transcribed:
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);   // computed, never used
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL;) {
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
//         if (pButton == NULL) AfxThrowInvalidArgException();
//         CRect rect = pButton->m_rect;                          // +0x68
//         ::OffsetRect(&rect, 0, -m_iScrollOffset);
//         if (::PtInRect(&rect, point)) return pButton;
//     }
//     return NULL;
// The retail rectClient is dead (nothing reads it back); it is not computed here.
// POINT is an 8-byte aggregate passed by value in RDX, so it is modelled as a
// long long.
extern "C" CMFCToolBarButton* MS_ABI impl__HitTest_CMFCToolBarButtonsListButton__IEBAPEAVCMFCToolBarButton__UtagPOINT___Z(
    const void* pThis, long long ptPacked) {
    if (!pThis) return nullptr;
    const ListBtn* p = Self(pThis);
    POINT point;
    std::memcpy(&point, &ptPacked, sizeof point);
    for (ObNode* n = ListHead(&p->m_Buttons); n != nullptr; n = n->pNext) {
        CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(n->data);
        if (pButton == nullptr) return nullptr;
        RECT rect = ToRect(pButton->m_rect);
        ::OffsetRect(&rect, 0, -p->m_iScrollOffset);
        if (::PtInRect(&rect, point)) return pButton;
    }
    return nullptr;
}

// Symbol: ?RedrawSelection@CMFCToolBarButtonsListButton@@IEAAXXZ
// Retail (RVA 0x15f9f0, mfc140.dll), fully transcribed:
//     if (m_pSelButton == NULL) return;
//     CRect rect = m_pSelButton->m_rect;
//     ::OffsetRect(&rect, 0, -m_iScrollOffset);
//     ::InflateRect(&rect, 2, 2);
//     ::InvalidateRect(m_hWnd, &rect, TRUE);
//     ::UpdateWindow(m_hWnd);
extern "C" void MS_ABI impl__RedrawSelection_CMFCToolBarButtonsListButton__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    if (p->m_pSelButton == nullptr) return;
    RECT rect = ToRect(p->m_pSelButton->m_rect);
    ::OffsetRect(&rect, 0, -p->m_iScrollOffset);
    ::InflateRect(&rect, 2, 2);
    ::InvalidateRect(Hwnd(pThis), &rect, TRUE);
    ::UpdateWindow(Hwnd(pThis));
}

// Symbol: ?SelectButton@CMFCToolBarButtonsListButton@@IEAAXPEAVCMFCToolBarButton@@@Z
// Retail (RVA 0x15f180, mfc140.dll), fully transcribed:
//     CMFCToolBarButton* pOldSel = m_pSelButton;
//     if (pOldSel == pButton) { RedrawSelection(); return; }   // call 0x15f9f0, then
//                                          // `jmp 0x15f33f` -- the epilogue: NO
//                                          // UpdateWindow and NO WM_COMMAND notify
//     m_pSelButton = pButton;
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     CRect rectSel; ::SetRectEmpty(&rectSel);
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL;) {
//         CMFCToolBarButton* p = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
//         if (p == NULL) AfxThrowInvalidArgException();
//         CRect rect = p->m_rect;
//         ::OffsetRect(&rect, 0, -m_iScrollOffset);
//         if (p == m_pSelButton)      rectSel = rect;
//         else if (p != pOldSel)      continue;
//         ::InflateRect(&rect, 2, 2);
//         CRect rectInter;
//         if (::IntersectRect(&rectInter, &rectClient, &rect))
//             ::InvalidateRect(m_hWnd, &rectInter, TRUE);
//     }
//     if (!::IsRectEmpty(&rectSel) &&
//         (rectSel.top >= rectClient.bottom || rectSel.bottom <= rectClient.top)) {
//         int iPos = rectSel.bottom + (rectClient.top - rectClient.bottom) - m_iScrollOffset;
//         if (iPos > m_iScrollTotal) iPos = m_iScrollTotal;   // cmovge at 0x15f2bf
//         if (iPos < 0) iPos = 0;                             // 0x15f2c2..0x15f2cf
//         SetScrollPos(SB_VERT, iPos, TRUE);                  // 0x28cb40
//         m_iScrollOffset = iPos;
//         ::InvalidateRect(m_hWnd, NULL, TRUE);
//     }
//     ::UpdateWindow(m_hWnd);                                  // 0x15f2f9
//     CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));   // 0x289180
//     if (pParent != NULL)
//         ::SendMessage(pParent->m_hWnd, WM_COMMAND, (WORD)GetDlgCtrlID(), (LPARAM)m_hWnd);
// (The import slot 0x2c5378 reads SendMessageA in this ANSI twin, i.e.
// SendMessageW in mfc140u; ::SendMessage under UNICODE picks the W entry.)
// The scroll arithmetic is transcribed exactly as the instructions compute it:
// rectSel already carries the -m_iScrollOffset offset applied in the loop, and
// 0x15f2b3 subtracts m_iScrollOffset a second time.
extern "C" void MS_ABI impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(
    void* pThis, CMFCToolBarButton* pButton) {
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    CMFCToolBarButton* pOldSel = p->m_pSelButton;
    if (pOldSel == pButton) {
        // Retail 0x15f1b2: RedrawSelection, then straight to the epilogue.
        impl__RedrawSelection_CMFCToolBarButtonsListButton__IEAAXXZ(pThis);
        return;
    }
    {
        p->m_pSelButton = pButton;
        RECT rectClient = { 0, 0, 0, 0 };
        ::GetClientRect(Hwnd(pThis), &rectClient);
        RECT rectSel;
        ::SetRectEmpty(&rectSel);
        for (ObNode* n = ListHead(&p->m_Buttons); n != nullptr; n = n->pNext) {
            CMFCToolBarButton* pBtn = static_cast<CMFCToolBarButton*>(n->data);
            if (pBtn == nullptr) return;
            RECT rect = ToRect(pBtn->m_rect);
            ::OffsetRect(&rect, 0, -p->m_iScrollOffset);
            if (pBtn == p->m_pSelButton) {
                rectSel = rect;
            } else if (pBtn != pOldSel) {
                continue;
            }
            ::InflateRect(&rect, 2, 2);
            RECT rectInter = { 0, 0, 0, 0 };
            if (::IntersectRect(&rectInter, &rectClient, &rect)) {
                ::InvalidateRect(Hwnd(pThis), &rectInter, TRUE);
            }
        }
        if (!::IsRectEmpty(&rectSel) &&
            (rectSel.top >= rectClient.bottom || rectSel.bottom <= rectClient.top)) {
            int iPos = rectSel.bottom + (rectClient.top - rectClient.bottom) - p->m_iScrollOffset;
            if (iPos > p->m_iScrollTotal) iPos = p->m_iScrollTotal;
            if (iPos < 0) iPos = 0;
            impl__SetScrollPos_CWnd__QEAAHHHH_Z(AsWnd(pThis), SB_VERT, iPos, TRUE);
            p->m_iScrollOffset = iPos;
            ::InvalidateRect(Hwnd(pThis), nullptr, TRUE);
        }
    }
    ::UpdateWindow(Hwnd(pThis));
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Hwnd(pThis)));
    if (pParent != nullptr) {
        const WORD wID = static_cast<WORD>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(AsWnd(pThis)));
        ::SendMessage(pParent->m_hWnd, WM_COMMAND, static_cast<WPARAM>(wID),
                      reinterpret_cast<LPARAM>(Hwnd(pThis)));
    }
}

// Symbol: ?SelectButton@CMFCToolBarButtonsListButton@@QEAAHH@Z
// Retail (RVA 0x15f370, mfc140.dll), fully transcribed:
//     if (iImage < 0) { SelectButton((CMFCToolBarButton*)NULL); return TRUE; }
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL;) {
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
//         if (pButton == NULL) AfxThrowInvalidArgException();
//         if (pButton->GetImage() == iImage) { SelectButton(pButton); return TRUE; }
//     }
//     return FALSE;
extern "C" int MS_ABI impl__SelectButton_CMFCToolBarButtonsListButton__QEAAHH_Z(void* pThis, int iImage) {
    if (!pThis) return FALSE;
    ListBtn* p = Self(pThis);
    if (iImage < 0) {
        impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(pThis, nullptr);
        return TRUE;
    }
    for (ObNode* n = ListHead(&p->m_Buttons); n != nullptr; n = n->pNext) {
        CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(n->data);
        if (pButton == nullptr) return FALSE;
        if (ButtonImage(pButton) == iImage) {
            impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(pThis, pButton);
            return TRUE;
        }
    }
    return FALSE;
}

// Symbol: ?RebuildLocations@CMFCToolBarButtonsListButton@@IEAAXXZ
// Retail (RVA 0x15f5c0, mfc140.dll), fully transcribed:
//     if (this == NULL) return;
//     if (m_hWnd == NULL || m_Buttons.GetCount() == 0) return;
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     CRect rect = rectClient;
//     rect.right += -1 - ::GetSystemMetrics(SM_CXVSCROLL);
//     ::InflateRect(&rect, -4, -5);
//     int x = rect.left;
//     int y = rect.top - m_iScrollOffset;
//     CClientDC dc(this);                                        // 0x2a1a60
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL;) {
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
//         if (pButton == NULL) AfxThrowInvalidArgException();
//         SIZE sz = pButton->OnCalculateSize(&dc, m_sizeButton, TRUE);   // vslot +0x48
//         int cx = sz.cx;
//         if (x + cx > rect.right) {
//             if (x == rect.left) cx = rect.right - rect.left;
//             else { x = rect.left; y += sz.cy + 5; }
//         }
//         pButton->m_rect = CRect(x, y, x + cx, y + m_sizeButton.cy);
//         pButton->OnMove();                                     // vslot +0x70
//         x += 4 + cx;
//     }
//     CMFCToolBarButton* pLast = (CMFCToolBarButton*)m_Buttons.GetTail();
//     if (pLast == NULL) AfxThrowInvalidArgException();
//     int nRowHeight = m_sizeButton.cy + 5;
//     int nPage  = (rect.bottom - rect.top) / nRowHeight;
//     int nTotal = pLast->m_rect.bottom / nRowHeight - nPage;
//     if (nTotal > 0) {
//         if (m_wndScrollBar.m_hWnd == NULL) {
//             CRect rectSB; ::GetClientRect(m_hWnd, &rectSB);
//             ::InflateRect(&rectSB, -1, -1);
//             rectSB.left = rectSB.right - ::GetSystemMetrics(SM_CXVSCROLL) - 1;
//             m_wndScrollBar.Create(SBS_VERT|WS_CHILD|WS_VISIBLE, rectSB, this, 1); // vslot +0x2d8
//         }
//         m_iScrollTotal = nRowHeight * nTotal;
//         m_iScrollPage  = nPage;
//         SetScrollRange(SB_VERT, 0, m_iScrollTotal, TRUE);
//     }
//     m_bInited = TRUE;
// DEVIATION: OnCalculateSize goes to the exported base thunk (no derived
// override reached), and OnMove -- an inline `{}` virtual with no export -- is
// not called; the base body is empty, so nothing is lost for a base button.
extern "C" void MS_ABI impl__RebuildLocations_CMFCToolBarButtonsListButton__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    if (Hwnd(pThis) == nullptr) return;
    if (ListCount(&p->m_Buttons) == 0) return;

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    RECT rect = rectClient;
    rect.right += -1 - ::GetSystemMetrics(SM_CXVSCROLL);
    ::InflateRect(&rect, -4, -5);

    int x = rect.left;
    int y = rect.top - p->m_iScrollOffset;

    alignas(8) unsigned char dcBuf[sizeof(CClientDC)];
    std::memset(dcBuf, 0, sizeof dcBuf);
    CClientDC* pDC = impl___0CClientDC__QEAA_PEAVCWnd___Z(
        reinterpret_cast<CClientDC*>(dcBuf), AsWnd(pThis));

    for (ObNode* n = ListHead(&p->m_Buttons); n != nullptr; n = n->pNext) {
        CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(n->data);
        if (pButton == nullptr) break;
        SIZE sz = { 0, 0 };
        SIZE sizeDefault;
        sizeDefault.cx = p->m_sizeButton.cx;
        sizeDefault.cy = p->m_sizeButton.cy;
        impl__OnCalculateSize_CMFCToolBarButton__UEAA_AUtagSIZE__PEAVCDC__AEBVCSize__H_Z(
            pButton, &sz, pDC, &sizeDefault, TRUE);
        int cx = sz.cx;
        if (x + cx > rect.right) {
            if (x == rect.left) {
                cx = rect.right - rect.left;
            } else {
                x = rect.left;
                y += sz.cy + 5;
            }
        }
        pButton->m_rect.left   = x;
        pButton->m_rect.top    = y;
        pButton->m_rect.right  = x + cx;
        pButton->m_rect.bottom = y + p->m_sizeButton.cy;
        x += 4 + cx;
    }

    ObNode* pTailNode = ListTail(&p->m_Buttons);
    CMFCToolBarButton* pLast = pTailNode ? static_cast<CMFCToolBarButton*>(pTailNode->data) : nullptr;
    if (pLast != nullptr) {
        const int nRowHeight = p->m_sizeButton.cy + 5;
        // Added guard: retail divides by nRowHeight unconditionally (idiv at
        // 0x15f73b / 0x15f745) and would fault if m_sizeButton.cy were -5.
        if (nRowHeight != 0) {
            const int nPage  = (rect.bottom - rect.top) / nRowHeight;
            const int nTotal = pLast->m_rect.bottom / nRowHeight - nPage;
            if (nTotal > 0) {
                if (p->m_wndScrollBar.m_hWnd == nullptr) {
                    RECT rectSB = { 0, 0, 0, 0 };
                    ::GetClientRect(Hwnd(pThis), &rectSB);
                    ::InflateRect(&rectSB, -1, -1);
                    rectSB.left = rectSB.right - ::GetSystemMetrics(SM_CXVSCROLL) - 1;
                    impl__Create_CScrollBar__UEAAHKAEBUtagRECT__PEAVCWnd__I_Z(
                        &p->m_wndScrollBar, SBS_VERT | WS_CHILD | WS_VISIBLE, &rectSB, AsWnd(pThis), 1);
                }
                p->m_iScrollTotal = nRowHeight * nTotal;
                p->m_iScrollPage  = nPage;
                impl__SetScrollRange_CWnd__QEAAXHHHH_Z(AsWnd(pThis), SB_VERT, 0, p->m_iScrollTotal, TRUE);
            }
        }
    }
    p->m_bInited = TRUE;

    impl___1CClientDC__UEAA_XZ(pDC);
}

// Symbol: ?GetScrollBarCtrl@CMFCToolBarButtonsListButton@@UEBAPEAVCScrollBar@@H@Z
// Retail (RVA 0x15f530, mfc140.dll), fully transcribed:
//     if (nBar == SB_HORZ) return NULL;
//     return m_wndScrollBar.m_hWnd != NULL ? (CScrollBar*)&m_wndScrollBar : NULL;
// (0x15f53d..0x15f547 is the branchless `m_hWnd ? &m_wndScrollBar : NULL`.)
extern "C" CScrollBar* MS_ABI impl__GetScrollBarCtrl_CMFCToolBarButtonsListButton__UEBAPEAVCScrollBar__H_Z(
    const void* pThis, int nBar) {
    if (!pThis || nBar == SB_HORZ) return nullptr;
    const ListBtn* p = Self(pThis);
    if (p->m_wndScrollBar.m_hWnd == nullptr) return nullptr;
    return const_cast<CScrollBar*>(&p->m_wndScrollBar);
}

// Symbol: ?DrawItem@CMFCToolBarButtonsListButton@@UEAAXPEAUtagDRAWITEMSTRUCT@@@Z
// Retail (RVA 0x15ec90, mfc140.dll), fully transcribed:
//     if (!m_bInited) RebuildLocations();
//     CDC* pDC = CDC::FromHandle(lpDIS->hDC);                    // 0x2a0390
//     CRect rectClient; ::CopyRect(&rectClient, &lpDIS->rcItem);
//     if (m_pImages != NULL) {
//         if (m_pImages->m_clrTransparent != afxGlobalData.clrBtnFace)   // +0xd8 / +0x28
//             m_pImages->m_clrTransparent = afxGlobalData.clrBtnFace;
//         AFX_DRAWSTATE ds;
//         if (m_pImages->PrepareDrawImage(ds, CSize(0,0), FALSE)) {      // 0x16a680
//             for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL;) {
//                 CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
//                 if (pButton == NULL) AfxThrowInvalidArgException();
//                 CRect rect = pButton->m_rect;
//                 ::OffsetRect(&rect, 0, -m_iScrollOffset);
//                 if (rect.top >= rectClient.bottom) break;
//                 if (rect.bottom <= rectClient.top) continue;
//                 UINT nStyleSaved = pButton->m_nStyle;       // +0x28
//                 BOOL bLockedSaved = pButton->m_bLocked;     // +0x50
//                 BOOL bHighlight;
//                 if (!IsWindowEnabled()) {                   // 0x2a7a10
//                     pButton->m_nStyle |= TBBS_DISABLED;     // bts $18
//                     bHighlight = FALSE;
//                 } else {
//                     bHighlight = (pButton == m_pSelButton);
//                 }
//                 pButton->m_bLocked = TRUE;
//                 pButton->OnDraw(pDC, rect, m_pImages, TRUE, FALSE, bHighlight, TRUE, TRUE); // vslot +0x40
//                 pButton->m_nStyle = nStyleSaved;
//                 pButton->m_bLocked = bLockedSaved;
//             }
//             m_pImages->EndDrawImage(ds);                    // 0x16a890
//         } else {
//             return;            // 0x15ed57 jumps straight to the epilogue
//         }
//     }
//     CMFCToolBarComboBoxButton button;                       // 0x15fb20
//     ::InflateRect(&rectClient, 1, 1);
//     CMFCVisualManager::GetInstance()->OnDrawComboBorder(
//         pDC, rectClient, !IsWindowEnabled(), FALSE, TRUE, &button);   // VM vslot +0xf0
// DEVIATIONS: OnDraw goes to the exported base thunk (no derived override);
// GetInstance() is inline in afxvisualmanager.h and has no thunk, so the current
// manager is read from the exported ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA
// static and the lazy creation of a default manager is NOT reproduced (the same
// deviation toolbar/CMFCToolBar.cpp documents at DrawSeparator) -- with no
// manager created yet the border is not drawn.
extern "C" void MS_ABI impl__DrawItem_CMFCToolBarButtonsListButton__UEAAXPEAUtagDRAWITEMSTRUCT___Z(
    void* pThis, DRAWITEMSTRUCT* lpDIS) {
    if (!pThis || !lpDIS) return;
    ListBtn* p = Self(pThis);
    if (!p->m_bInited) {
        impl__RebuildLocations_CMFCToolBarButtonsListButton__IEAAXXZ(pThis);
    }
    CDC* pDC = impl__FromHandle_CDC__SAPEAV1_PEAUHDC_____Z(lpDIS->hDC);
    RECT rectClient;
    ::CopyRect(&rectClient, &lpDIS->rcItem);

    if (p->m_pImages != nullptr) {
        const COLORREF clrBtnFace = GlobalDataColor(kGlobalDataClrBtnFace);
        COLORREF clrTransparent = 0;
        char* pImagesBytes = reinterpret_cast<char*>(p->m_pImages);
        std::memcpy(&clrTransparent, pImagesBytes + kOffImagesClrTransparent, sizeof clrTransparent);
        if (clrTransparent != clrBtnFace) {
            std::memcpy(pImagesBytes + kOffImagesClrTransparent, &clrBtnFace, sizeof clrBtnFace);
        }
        // AFX_DRAWSTATE { HBITMAP hbmMono, hbmMonoOld, hbmOldGlyphs } -- the
        // 32-byte frame slot retail reserves at 0x50(%rsp).
        unsigned char ds[32];
        std::memset(ds, 0, sizeof ds);
        if (!impl__PrepareDrawImage_CMFCToolBarImages__QEAAHAEAUtagAFXDrawState__VCSize__H_Z(
                p->m_pImages, ds, 0LL, FALSE)) {
            return;
        }
        // Retail calls IsWindowEnabled() once per drawn button (0x15edbb);
        // hoisted out of the loop here -- same result for one WM_DRAWITEM.
        const int bEnabled = impl__IsWindowEnabled_CWnd__QEBAHXZ(AsWnd(pThis));
        for (ObNode* n = ListHead(&p->m_Buttons); n != nullptr; n = n->pNext) {
            CMFCToolBarButton* pButton = static_cast<CMFCToolBarButton*>(n->data);
            if (pButton == nullptr) break;
            RECT rect = ToRect(pButton->m_rect);
            ::OffsetRect(&rect, 0, -p->m_iScrollOffset);
            if (rect.top >= rectClient.bottom) break;
            if (rect.bottom <= rectClient.top) continue;
            const UINT nStyleSaved = pButton->m_nStyle;
            const BOOL bLockedSaved = pButton->m_bLocked;
            int bHighlight;
            if (!bEnabled) {
                pButton->m_nStyle |= 0x00040000u;   // TBBS_DISABLED (bts $0x12)
                bHighlight = FALSE;
            } else {
                bHighlight = (pButton == p->m_pSelButton);
            }
            pButton->m_bLocked = TRUE;
            impl__OnDraw_CMFCToolBarButton__UEAAXPEAVCDC__AEBVCRect__PEAVCMFCToolBarImages__HHHHH_Z(
                pButton, pDC, &rect, p->m_pImages, TRUE, FALSE, bHighlight, TRUE, TRUE);
            pButton->m_nStyle = nStyleSaved;
            pButton->m_bLocked = bLockedSaved;
        }
        impl__EndDrawImage_CMFCToolBarImages__QEAAXAEAUtagAFXDrawState___Z(p->m_pImages, ds);
    }

    CMFCVisualManager* pVM =
        static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
    ::InflateRect(&rectClient, 1, 1);
    if (pVM == nullptr) {
        // TODO(clean-room): retail's GetInstance() would create the default
        // manager here; see the deviation note above.
        return;
    }
    alignas(16) unsigned char btnBuf[sizeof(CMFCToolBarComboBoxButton)];
    std::memset(btnBuf, 0, sizeof btnBuf);
    impl___0CMFCToolBarComboBoxButton__QEAA_XZ(btnBuf);
    impl__OnDrawComboBorder_CMFCVisualManager__UEAAXPEAVCDC__VCRect__HHHPEAVCMFCToolBarComboBoxButton___Z(
        pVM, pDC, CRect(rectClient), !impl__IsWindowEnabled_CWnd__QEBAHXZ(AsWnd(pThis)), FALSE, TRUE,
        reinterpret_cast<CMFCToolBarComboBoxButton*>(btnBuf));
    impl___1CMFCToolBarComboBoxButton__UEAA_XZ(btnBuf);
}

// ---------------------------------------------------------------------------
// Message handlers.  The class' AFX_MSGMAP (RVA 0x3144c0, mfc140.dll; entries at
// 0x3144d0) maps: WM_ERASEBKGND->OnEraseBkgnd, WM_LBUTTONDOWN->OnLButtonDown,
// WM_VSCROLL->OnVScroll, WM_ENABLE->OnEnable, WM_SYSCOLORCHANGE->OnSysColorChange,
// WM_SIZE->OnSize, WM_CTLCOLOR->OnCtlColor, WM_KEYDOWN->OnKeyDown,
// WM_GETDLGCODE->OnGetDlgCode.
// ---------------------------------------------------------------------------

// Symbol: ?OnSize@CMFCToolBarButtonsListButton@@IEAAXIHH@Z
// Retail (RVA 0x15f830, mfc140.dll), fully transcribed:
//     CWnd::Default();                 // 0x289090 -- CButton::OnSize is the
//                                      // non-virtual inline CWnd::OnSize { Default(); }
//     RebuildLocations();              // tail jump to 0x15f5c0
extern "C" void MS_ABI impl__OnSize_CMFCToolBarButtonsListButton__IEAAXIHH_Z(
    void* pThis, unsigned int nType, int cx, int cy) {
    (void)nType; (void)cx; (void)cy;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    impl__RebuildLocations_CMFCToolBarButtonsListButton__IEAAXXZ(pThis);
}

// Symbol: ?OnEnable@CMFCToolBarButtonsListButton@@IEAAXH@Z
// Retail (RVA 0x15f550, mfc140.dll), fully transcribed:
//     CWnd::Default();                                     // 0x289090
//     ::RedrawWindow(m_hWnd, NULL, NULL,
//                    RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);   // 0x105, tail jump
extern "C" void MS_ABI impl__OnEnable_CMFCToolBarButtonsListButton__IEAAXH_Z(void* pThis, int bEnable) {
    (void)bEnable;
    if (!pThis) return;
    impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
    ::RedrawWindow(Hwnd(pThis), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// Symbol: ?OnSysColorChange@CMFCToolBarButtonsListButton@@IEAAXXZ
// Retail (RVA 0x15f580, mfc140.dll), fully transcribed:
//     if (m_pImages == NULL) return;
//     m_pImages->OnSysColorChange();                       // 0x16bb20
//     ::RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBarButtonsListButton__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    if (p->m_pImages == nullptr) return;
    impl__OnSysColorChange_CMFCToolBarImages__QEAAXXZ(p->m_pImages);
    ::RedrawWindow(Hwnd(pThis), nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
}

// Symbol: ?OnEraseBkgnd@CMFCToolBarButtonsListButton@@IEAAHPEAVCDC@@@Z
// Retail (RVA 0x15ebd0, mfc140.dll), fully transcribed:
//     CRect rectClient; ::GetClientRect(m_hWnd, &rectClient);
//     COLORREF clr = IsWindowEnabled() ? afxGlobalData.clrWindow    // +0x78
//                                      : afxGlobalData.clrBtnFace;  // +0x28
//     pDC->FillSolidRect(&rectClient, clr);                          // 0x2a39e0
//     return TRUE;
// (The two afxGlobalData slots are pinned by UpdateSysColors, RVA 0x6afd0:
// +0x78 <- GetSysColor(COLOR_WINDOW), +0x28 <- GetSysColor(COLOR_BTNFACE).)
extern "C" int MS_ABI impl__OnEraseBkgnd_CMFCToolBarButtonsListButton__IEAAHPEAVCDC___Z(
    void* pThis, CDC* pDC) {
    if (!pThis) return FALSE;
    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rectClient);
    const COLORREF clr = impl__IsWindowEnabled_CWnd__QEBAHXZ(AsWnd(pThis))
                             ? GlobalDataColor(kGlobalDataClrWindow)
                             : GlobalDataColor(kGlobalDataClrBtnFace);
    impl__FillSolidRect_CDC__QEAAXPEBUtagRECT__K_Z(pDC, &rectClient, clr);
    return TRUE;
}

// Symbol: ?OnGetDlgCode@CMFCToolBarButtonsListButton@@IEAAIXZ
// This export has no resolved RVA in either symbol map, but the class'
// AFX_MSGMAP entry for WM_GETDLGCODE (0x0087) at RVA 0x3145d0 (mfc140.dll)
// points at 0x3ae0, a two-instruction body ICF-folded with
// ?OnEraseBkgnd@CPaneTrackingWnd@@IEAAHPEAVCDC@@@Z:
//     mov $0x1,%eax ; ret       i.e. `return DLGC_WANTARROWS;`
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CMFCToolBarButtonsListButton__IEAAIXZ(void* pThis) {
    (void)pThis;
    return DLGC_WANTARROWS;
}

// Symbol: ?OnVScroll@CMFCToolBarButtonsListButton@@IEAAXIIPEAVCScrollBar@@@Z
// Retail (RVA 0x15f3d0, mfc140.dll), fully transcribed:
//     int iOffset = m_iScrollOffset;
//     int nRowHeight = m_sizeButton.cy + 5;
//     switch (nSBCode) {
//     case SB_LINEUP:        iOffset += -5 - m_sizeButton.cy;      break;
//     case SB_LINEDOWN:      iOffset += nRowHeight;                break;
//     case SB_PAGEUP:        iOffset -= nRowHeight * m_iScrollPage;break;
//     case SB_PAGEDOWN:      iOffset += nRowHeight * m_iScrollPage;break;
//     case SB_THUMBPOSITION: iOffset = ((nPos + nRowHeight/2) / nRowHeight) * nRowHeight; break;
//     case SB_TOP:           iOffset = 0;                          break;
//     case SB_BOTTOM:        iOffset = m_iScrollTotal;             break;
//     default:               return;   // SB_THUMBTRACK (5) and SB_ENDSCROLL fall here
//     }
//     if (iOffset < 0) iOffset = 0;
//     if (iOffset > m_iScrollTotal) iOffset = m_iScrollTotal;
//     if (iOffset == m_iScrollOffset) return;
//     m_iScrollOffset = iOffset;
//     SetScrollPos(SB_VERT, iOffset, TRUE);                        // 0x28cb40
//     CRect rect; ::GetClientRect(m_hWnd, &rect);
//     rect.right += -2 - ::GetSystemMetrics(SM_CXVSCROLL);
//     ::InflateRect(&rect, -1, -1);
//     ::InvalidateRect(m_hWnd, &rect, TRUE);
// (SB_THUMBPOSITION uses an unsigned divide at 0x15f43f, so nPos is treated as
// unsigned there, exactly as written below.)
extern "C" void MS_ABI impl__OnVScroll_CMFCToolBarButtonsListButton__IEAAXIIPEAVCScrollBar___Z(
    void* pThis, unsigned int nSBCode, unsigned int nPos, CScrollBar* pScrollBar) {
    (void)pScrollBar;
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    const int nRowHeight = p->m_sizeButton.cy + 5;
    int iOffset = p->m_iScrollOffset;
    switch (nSBCode) {
    case SB_LINEUP:        iOffset += -5 - p->m_sizeButton.cy;         break;
    case SB_LINEDOWN:      iOffset += nRowHeight;                      break;
    case SB_PAGEUP:        iOffset -= nRowHeight * p->m_iScrollPage;   break;
    case SB_PAGEDOWN:      iOffset += nRowHeight * p->m_iScrollPage;   break;
    case SB_THUMBPOSITION:
        if (nRowHeight == 0) return;   // added guard: retail's div at 0x15f43f would #DE
        iOffset = static_cast<int>((nPos + static_cast<unsigned int>(nRowHeight / 2)) /
                                   static_cast<unsigned int>(nRowHeight)) * nRowHeight;
        break;
    case SB_TOP:           iOffset = 0;                                break;
    case SB_BOTTOM:        iOffset = p->m_iScrollTotal;                break;
    default:               return;
    }
    if (iOffset < 0) iOffset = 0;
    if (iOffset > p->m_iScrollTotal) iOffset = p->m_iScrollTotal;
    if (iOffset == p->m_iScrollOffset) return;
    p->m_iScrollOffset = iOffset;
    impl__SetScrollPos_CWnd__QEAAHHHH_Z(AsWnd(pThis), SB_VERT, iOffset, TRUE);
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(Hwnd(pThis), &rect);
    rect.right += -2 - ::GetSystemMetrics(SM_CXVSCROLL);
    ::InflateRect(&rect, -1, -1);
    ::InvalidateRect(Hwnd(pThis), &rect, TRUE);
}

// Symbol: ?OnLButtonDown@CMFCToolBarButtonsListButton@@IEAAXIVCPoint@@@Z
// Retail (RVA 0x15eef0, mfc140.dll), fully transcribed:
//     SetFocus();                                              // 0x2a7a70
//     CMFCToolBarButton* pButton = HitTest(point);              // 0x15f0d0
//     if (pButton == NULL) return;
//     SelectButton(pButton);                                    // 0x15f180
//     if (!m_bEnableDragFromList) return;
//     COleDataSource srcItem;                                   // 0x252050
//     pButton->m_bDragFromCollection = TRUE;                    // +0x20
//     pButton->PrepareDrag(srcItem);                            // vslot +0x28
//     pButton->m_bDragFromCollection = TRUE;                    // stored again
//     srcItem.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_LINK, NULL, NULL); // 0x259710
//     // ~COleDataSource                                        // 0x252090
// CPoint is an 8-byte aggregate passed by value in R8, so it is modelled as a
// long long.  DEVIATION: PrepareDrag goes to the exported base thunk -- whose
// definition in toolbar/CMFCToolBarButton.cpp is still a `return 0` placeholder
// with a mismatched parameter list -- so no drag data is actually offered.
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBarButtonsListButton__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long ptPacked) {
    (void)nFlags;
    if (!pThis) return;
    ListBtn* p = Self(pThis);
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(AsWnd(pThis));
    CMFCToolBarButton* pButton =
        impl__HitTest_CMFCToolBarButtonsListButton__IEBAPEAVCMFCToolBarButton__UtagPOINT___Z(pThis, ptPacked);
    if (pButton == nullptr) return;
    impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(pThis, pButton);
    if (!p->m_bEnableDragFromList) return;

    alignas(8) unsigned char srcBuf[sizeof(COleDataSource)];
    std::memset(srcBuf, 0, sizeof srcBuf);
    impl___0COleDataSource__QEAA_XZ(srcBuf);
    COleDataSource* pSrc = reinterpret_cast<COleDataSource*>(srcBuf);
    pButton->m_bDragFromCollection = TRUE;
    impl__PrepareDrag_CMFCToolBarButton__UEAAHAEAVCOleDataSource___Z(pButton, pSrc);
    pButton->m_bDragFromCollection = TRUE;
    impl__DoDragDrop_COleDataSource__QEAAKKPEBUtagRECT__PEAVCOleDropSource___Z(
        pSrc, DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK, nullptr, nullptr);
    impl___1COleDataSource__UEAA_XZ(srcBuf);
}

// Symbol: ?OnCtlColor@CMFCToolBarButtonsListButton@@IEAAPEAUHBRUSH__@@PEAVCDC@@PEAVCWnd@@I@Z
// Retail (RVA 0x15f850, mfc140.dll), fully transcribed:
//     LRESULT lResult;
//     HBRUSH hbr = pWnd->SendChildNotifyLastMsg(&lResult) ? (HBRUSH)lResult   // 0x28d400
//                                                         : (HBRUSH)Default(); // 0x289090
//     for (POSITION pos = m_Buttons.GetHeadPosition(); pos != NULL;) {
//         CMFCToolBarButton* pButton = (CMFCToolBarButton*)m_Buttons.GetNext(pos);
//         HWND h = pButton->GetHwnd();                                // vslot +0x80
//         if (h != NULL && (h == pWnd->GetSafeHwnd() || ::IsChild(h, pWnd->GetSafeHwnd()))) {
//             HBRUSH h2 = pButton->OnCtlColor(pDC, nCtlColor);        // vslot +0xa0
//             return h2 != NULL ? h2 : hbr;                           // cmove at 0x15f90b
//         }
//     }
//     return hbr;
// DEVIATION: GetHwnd and OnCtlColor are inline virtuals in afxtoolbarbutton.h
// (`return NULL` both) with no export and no thunk, so OpenMFC cannot dispatch
// them; with the base results the retail loop can never match a button and
// always falls through to `return hbr`.  Only that reachable behaviour is
// written here -- the loop itself is omitted because it would be dead code.
extern "C" HBRUSH MS_ABI impl__OnCtlColor_CMFCToolBarButtonsListButton__IEAAPEAUHBRUSH____PEAVCDC__PEAVCWnd__I_Z(
    void* pThis, CDC* pDC, CWnd* pWnd, unsigned int nCtlColor) {
    (void)pDC; (void)nCtlColor;
    if (!pThis) return nullptr;
    LONGLONG lResult = 0;
    // Added guard: retail calls pWnd->SendChildNotifyLastMsg unconditionally
    // (0x15f871) and would fault on a NULL pWnd.
    if (pWnd != nullptr && impl__SendChildNotifyLastMsg_CWnd__QEAAHPEA_J_Z(pWnd, &lResult)) {
        return reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(lResult));
    }
    return reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis))));
}

// Symbol: ?OnKeyDown@CMFCToolBarButtonsListButton@@IEAAXIII@Z
// Retail (RVA 0x15f920, mfc140.dll), fully transcribed.  The dispatch is a chain
// of `sub`/`je` on nChar starting at 0x15f924 (VK_END 0x23, VK_HOME 0x24,
// VK_LEFT 0x25, VK_UP 0x26, VK_RIGHT 0x27, VK_DOWN 0x28):
//     case VK_RIGHT: case VK_DOWN:        // 0x15f94f
//         if (m_pSelButton == NULL) goto home;
//         find the node whose data == m_pSelButton; if not found, return;
//         if (node->pNext == NULL) return;
//         SelectButton((CMFCToolBarButton*)node->pNext->data);
//         break;
//     case VK_LEFT: case VK_UP:           // 0x15f999
//         if (m_pSelButton == NULL) goto home;
//         find the node whose data == m_pSelButton; if not found, return;
//         if (node->pPrev == NULL) return;
//         SelectButton((CMFCToolBarButton*)node->pPrev->data);
//         break;
//     case VK_HOME: home:                 // 0x15f986
//         if (m_Buttons.GetCount() == 0) return;
//         SelectButton((CMFCToolBarButton*)m_Buttons.GetHead());
//         break;
//     case VK_END:                        // 0x15f9cc
//         if (m_Buttons.GetCount() != 0) return;        // bytes 75 10 = jne
//         SelectButton((CMFCToolBarButton*)m_Buttons.GetTail());
//         break;
//     default:
//         CWnd::Default();                // tail jump to 0x289090
//     }
// DEVIATION on VK_END: retail's gate is inverted (`jne` -- it takes the tail
// only when the list is EMPTY) and then dereferences m_pNodeTail, which is NULL
// on exactly that path; that is a null dereference in retail.  This body keeps
// retail's gate but returns when the tail is NULL, so VK_END is a no-op -- which
// is retail's behaviour on every path that does not fault.
extern "C" void MS_ABI impl__OnKeyDown_CMFCToolBarButtonsListButton__IEAAXIII_Z(
    void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt; (void)nFlags;
    if (!pThis) return;
    ListBtn* p = Self(pThis);

    bool bHome = false;
    switch (nChar) {
    case VK_RIGHT:
    case VK_DOWN:
    case VK_LEFT:
    case VK_UP: {
        if (p->m_pSelButton == nullptr) {
            bHome = true;
            break;
        }
        ObNode* n = ListHead(&p->m_Buttons);
        while (n != nullptr && n->data != static_cast<CObject*>(p->m_pSelButton)) n = n->pNext;
        if (n == nullptr) return;                       // not found: retail returns
        ObNode* nStep = (nChar == VK_RIGHT || nChar == VK_DOWN) ? n->pNext : n->pPrev;
        if (nStep == nullptr) return;
        impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(
            pThis, static_cast<CMFCToolBarButton*>(nStep->data));
        return;
    }
    case VK_HOME:
        bHome = true;
        break;
    case VK_END: {
        if (ListCount(&p->m_Buttons) != 0) return;      // retail's inverted gate
        ObNode* n = ListTail(&p->m_Buttons);
        if (n == nullptr) return;                       // retail faults here instead
        impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(
            pThis, static_cast<CMFCToolBarButton*>(n->data));
        return;
    }
    default:
        impl__Default_CWnd__IEAA_JXZ(AsWnd(pThis));
        return;
    }

    if (bHome) {
        if (ListCount(&p->m_Buttons) == 0) return;
        ObNode* n = ListHead(&p->m_Buttons);
        if (n == nullptr) return;
        impl__SelectButton_CMFCToolBarButtonsListButton__IEAAXPEAVCMFCToolBarButton___Z(
            pThis, static_cast<CMFCToolBarButton*>(n->data));
    }
}
