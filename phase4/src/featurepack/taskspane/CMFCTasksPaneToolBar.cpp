// CMFCTasksPaneToolBar — OpenMFC implementation.
//
// The navigation toolbar embedded in CMFCTasksPane (retail afxtaskspane.h:173,
// `class CMFCTasksPaneToolBar : public CMFCToolBar`, DECLARE_SERIAL).  Every body
// below is transcribed from the retail mfc140u export named in its comment.
// disas.py reads mfc140.dll by default; each body was also disassembled from
// mfc140u.dll and compared instruction-for-instruction (only rip-relative and
// call-target addresses differ), so every RVA quoted here is an mfc140u RVA.
//
// OpenMFC's include/openmfc does NOT declare CMFCTasksPaneToolBar, so `this` is
// taken as void* and every member access goes through the view structs below,
// whose offsets are pinned by static_asserts against the retail disassembly.
// This TU deliberately includes no openmfc header: merely including
// openmfc/afxwin.h emits undefined references to C++ symbols (CCmdTarget
// members, CWnd::classCWnd, ...) that the per-file link audit rejects, so all
// retail types are addressed through byte-offset views and extern "C" thunks.
//
// Objects reached through m_Buttons are the task pane's private button classes
// CTasksPaneNavigateButton / CTasksPaneMenuButton / CTasksPaneHistoryButton.
// They are NOT declared in any public SDK header (grep of atlmfc/include finds
// no `class CTasksPane*`), so a client cannot derive from them, and their
// virtuals resolve to the fixed retail slots noted at each call site.

#include <windows.h>

#include <cstddef>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Exported thunks used below (BRIEFING S1: C++ methods inside this DLL exist
// only as these impl__ thunks).  Each one's definition was read; pointer types
// are spelled void* here because this TU includes no class declarations (the
// ABI is identical: every one of them is a single pointer register).
//   featurepack/toolbar/CMFCToolBar.cpp   AdjustLayout / AdjustLocations /
//                                         UpdateTooltips / OnUserToolTip /
//                                         OnUpdateCmdUI  (CMFCToolBar)
//   featurepack/toolbar/Thunks.cpp        ??0CMFCToolBar@@QEAA@XZ
//   featurepack/toolbar/CMFCToolBarMenuButton.cpp  CreateFromMenu
//   featurepack/taskspane/CMFCTasksPane.cpp  RecalcLayout / GetPreviousPages /
//                                         GetNextPages
//   featurepack/taskspane/RuntimeClasses.cpp  GetThisClass for CMFCTasksPane and
//                                         the three CTasksPane*Button classes
//   core/runtime/CObject.cpp              IsKindOf
//   core/window/CWnd.cpp                  FromHandle
//   core/window/Thunks.cpp                GetStyle
//   core/collections/CObList.cpp / CStringList.cpp  FindIndex
//   core/collections/Thunks.cpp           CSimpleStringT<wchar_t,1>::operator=
//   core/collections/CStringT.cpp         CStringT::LoadStringW(HINSTANCE, UINT)
//   featurepack/CMFC_misc_stubs.cpp       AfxFindStringResourceHandle
//   detail/MfcExceptionsSupport.cpp       AfxThrowInvalidArgException
//   detail/MemcoreSupport.cpp             ??2@YAPEAX_K@Z (MFC's operator new)
// ---------------------------------------------------------------------------
extern "C" void MS_ABI impl__AdjustLayout_CMFCToolBar__UEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__AdjustLocations_CMFCToolBar__MEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(void* pThis);
extern "C" int MS_ABI impl__OnUserToolTip_CMFCToolBar__UEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const void* pThis, void* pButton, void* pStrText);
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* pTarget, int bDisableIfNoHandler);
extern "C" void* MS_ABI impl___0CMFCToolBar__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(void* pThis, HMENU hMenu);
extern "C" void MS_ABI impl__RecalcLayout_CMFCTasksPane__QEAAXH_Z(void* pThis, int bRedraw);
extern "C" void MS_ABI impl__GetPreviousPages_CMFCTasksPane__QEBAXAEAVCStringList___Z(const void* pThis, void* pList);
extern "C" void MS_ABI impl__GetNextPages_CMFCTasksPane__QEBAXAEAVCStringList___Z(const void* pThis, void* pList);
extern "C" void* MS_ABI impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ();
extern "C" void* MS_ABI impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const void* pThis, const void* pClass);
extern "C" void* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const void* pThis);
extern "C" void* MS_ABI impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(const void* pThis, long long nIndex);
extern "C" void* MS_ABI impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(const void* pThis, long long nIndex);
extern "C" void* MS_ABI impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(
    void* pThis, const void* src);
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    void* pThis, HINSTANCE hInst, UINT nID);
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(unsigned int nID);
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);

namespace {

// --- CMFCTasksPaneToolBar (retail sizeof 0x1360) ------------------------------
// +0x40    CWnd::m_hWnd            -- read by every body (mov 0x40(%rcx)); OpenMFC's
//                                     CWnd matches (static_assert offsetof(CWnd, m_hWnd)
//                                     == 0x40 in e.g. featurepack/menu/CMFCMenuButton.cpp)
// +0x1188  CMFCToolBar::m_Buttons  -- CObList; retail reads its head at +0x1190;
//                                     include/openmfc/afxmfc.h declares it at 0x1188
// +0x1350  m_pBtnBack, +0x1358 m_pBtnForward -- CreateObject (RVA 0x143a80,
//          mfc140u) allocates 0x1360 bytes and zeroes these two, matching the
//          inline ctor in afxtaskspane.h; sizeof(CMFCToolBar) is 0x1350
//          (static_assert in featurepack/controls/CMFCToolTipCtrl.cpp).
constexpr std::size_t kOffHWnd        = 0x40;
constexpr std::size_t kOffButtons     = 0x1188;
constexpr std::size_t kOffBtnBack     = 0x1350;
constexpr std::size_t kOffBtnForward  = 0x1358;
constexpr std::size_t kSizeofTasksPaneToolBar = 0x1360;
static_assert(kOffBtnForward + sizeof(void*) == kSizeofTasksPaneToolBar,
              "retail sizeof 0x1360 (descriptor m_nObjectSize 4960)");

// --- CMFCToolBarButton, retail layout (sizeof 0x88) ---------------------------
// Offsets as retail reads them in the bodies below, and as the harvested layout
// in include/openmfc/afxmfc.h (class CMFCToolBarButton) declares them.
struct TbButtonView {
    void*     vfptr;                // +0x00
    BOOL      m_bUserButton;        // +0x08  GetImage() selector
    BOOL      m_bText;              // +0x0c
    BOOL      m_bImage;             // +0x10
    BOOL      m_bWrap;              // +0x14
    BOOL      m_bWholeText;         // +0x18
    BOOL      m_bTextBelow;         // +0x1c
    BOOL      m_bDragFromCollection;// +0x20
    UINT      m_nID;                // +0x24
    UINT      m_nStyle;             // +0x28
    DWORD_PTR m_dwdItemData;        // +0x30
    void*     m_strText;            // +0x38  CString { m_pszData }
    void*     m_strTextCustom;      // +0x40
    int       m_iImage;             // +0x48
    int       m_iUserImage;         // +0x4c
    BOOL      m_bLocked;            // +0x50
    BOOL      m_bIsHidden;          // +0x54
    BOOL      m_bDisableFill;       // +0x58
    BOOL      m_bExtraSize;         // +0x5c
    BOOL      m_bHorz;              // +0x60
    BOOL      m_bVisible;           // +0x64
    RECT      m_rect;               // +0x68
    SIZE      m_sizeText;           // +0x78
    void*     m_pWndParent;         // +0x80
    // inline CMFCToolBarButton::GetImage() -- retail inlines it as
    // `neg m_bUserButton; sbb; and $4` selecting +0x4c or +0x48.
    int GetImage() const { return m_bUserButton ? m_iUserImage : m_iImage; }
};
static_assert(offsetof(TbButtonView, m_bUserButton) == 0x08, "m_bUserButton");
static_assert(offsetof(TbButtonView, m_nID) == 0x24, "m_nID");
static_assert(offsetof(TbButtonView, m_nStyle) == 0x28, "m_nStyle");
static_assert(offsetof(TbButtonView, m_strText) == 0x38, "m_strText");
static_assert(offsetof(TbButtonView, m_iImage) == 0x48 && offsetof(TbButtonView, m_iUserImage) == 0x4c,
              "GetImage() reads +0x48 / +0x4c");
static_assert(offsetof(TbButtonView, m_bIsHidden) == 0x54, "m_bIsHidden");
static_assert(offsetof(TbButtonView, m_rect) == 0x68, "m_rect (left +0x68, top +0x6c, right +0x70, bottom +0x74)");
static_assert(sizeof(TbButtonView) == 0x88, "CMFCToolBarButton / CTasksPaneNavigateButton retail sizeof 0x88");

// CTasksPaneHistoryButton (retail-only class, derives CMFCToolBarMenuButton).
// Its CreateObject (RVA 0x143830, mfc140u) allocates 0x168 bytes, zeroes +0x128,
// and constructs a CStringList in place at +0x130 (vptr store at +0x130, list
// fields +0x138..+0x160, m_nBlockSize = 10).  UpdateButtons reads the task-pane
// pointer at +0x128 and fills / walks the list at +0x130 (head at +0x138).
struct HistoryButtonView {
    unsigned char base[0x128];          // CMFCToolBarMenuButton part
    void*         m_pParentTaskPane;    // +0x128  CMFCTasksPane*
    unsigned char m_lstPages[0x38];     // +0x130  CStringList
};
static_assert(offsetof(HistoryButtonView, m_pParentTaskPane) == 0x128, "history button: task pane @0x128");
static_assert(offsetof(HistoryButtonView, m_lstPages) == 0x130, "history button: CStringList @0x130");
static_assert(sizeof(HistoryButtonView) == 0x168, "CTasksPaneHistoryButton retail sizeof 0x168");

// Command IDs UpdateButtons compares the history button's m_nID against: 0x427c
// selects GetPreviousPages (the Back button), 0x427d GetNextPages (Forward).
constexpr UINT kIdHistoryBack    = 0x427c;
constexpr UINT kIdHistoryForward = 0x427d;
// TBBS_SEPARATOR (== TBSTYLE_SEP): AdjustLocations tests `testb $0x1, 0x28(button)`.
constexpr UINT kTbbsSeparator = 0x0001;
// String resource OnUserToolTip loads for a CTasksPaneMenuButton.
constexpr UINT kIdsMenuButtonToolTip = 0x4280;

inline unsigned char* Bytes(void* p) { return static_cast<unsigned char*>(p); }
inline HWND HWndOf(void* pThis) {
    HWND h = nullptr;
    std::memcpy(&h, Bytes(pThis) + kOffHWnd, sizeof h);
    return h;
}

// Mirror of CList<...>::CNode (pNext / pPrev / data) -- what a POSITION points
// at, identical to retail's node layout (retail reads data at +0x10).  OpenMFC's
// lists keep their live contents in a side table keyed by the list's address
// (include/openmfc/afx.h, OPENMFC_DECLARE_LIST_WRAPPER), so the in-object head
// pointer retail reads is NOT the working representation.  The walks below
// therefore get the head node from the exported FindIndex(0) thunk and follow
// pNext, the same pattern featurepack/taskspane/CMFCTasksPane.cpp uses.  For a
// list address the side table does not know, FindIndex returns NULL and the
// walk is empty.
struct ObNode {
    ObNode* pNext;
    ObNode* pPrev;
    void*   data;           // CObject*
};
struct StrNode {
    StrNode*       pNext;
    StrNode*       pPrev;
    const wchar_t* data;    // CString { m_pszData } -- retail passes this word to AppendMenuW
};
static_assert(offsetof(ObNode, data) == 0x10 && offsetof(StrNode, data) == 0x10, "node payload at +0x10");

inline ObNode* ButtonsHead(void* pThis) {
    return static_cast<ObNode*>(impl__FindIndex_CObList__QEBAPEAU__POSITION____J_Z(Bytes(pThis) + kOffButtons, 0));
}
inline StrNode* StringListHead(void* pList) {
    return static_cast<StrNode*>(impl__FindIndex_CStringList__QEBAPEAU__POSITION____J_Z(pList, 0));
}

inline int IsKindOf(const void* pObj, const void* pClass) {
    return impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pObj, pClass);
}

} // namespace

// CMFCTasksPaneToolBar::AdjustLayout() -- transcribed from RVA 0x143f80 (mfc140u):
//   CMFCToolBar::AdjustLayout();                                  // direct call, 0x156ff0
//   CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));        // GetParent() inlined; FromHandle 0x28ad70
//   if (pParent != NULL &&
//       pParent->IsKindOf(RUNTIME_CLASS(CMFCTasksPane)))          // descriptor 0x1803b1568 (mfc140u)
//       ((CMFCTasksPane*)pParent)->RecalcLayout(TRUE);            // 0x14a4b0, %edx = 1
// The pThis NULL guard has no retail counterpart (retail tests nothing on entry).
// Symbol: ?AdjustLayout@CMFCTasksPaneToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CMFCTasksPaneToolBar__MEAAXXZ(void* pThis)
{
    if (pThis == nullptr) return;
    impl__AdjustLayout_CMFCToolBar__UEAAXXZ(pThis);
    void* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
    if (pParent != nullptr &&
        IsKindOf(pParent, impl__GetThisClass_CMFCTasksPane__SAPEAUCRuntimeClass__XZ())) {
        impl__RecalcLayout_CMFCTasksPane__QEAAXH_Z(pParent, TRUE);
    }
}

// CMFCTasksPaneToolBar::AdjustLocations() -- transcribed from RVA 0x143ba0 (mfc140u):
//   if (this == NULL || m_hWnd == NULL || !::IsWindow(m_hWnd)) return;   // GetSafeHwnd() inlined
//   CMFCToolBar::AdjustLocations();                                 // direct call, 0x155990
//   CTasksPaneNavigateButton* pNav = NULL; CTasksPaneMenuButton* pMenu = NULL;
//   for (node = m_Buttons head (+0x1190); node; node = node->pNext) {
//       pButton = node->data;
//       if (pButton->m_nStyle & TBBS_SEPARATOR) continue;            // testb $1, +0x28
//       if (pButton->IsKindOf(CTasksPaneNavigateButton)) {           // 0x1803b1598 (mfc140u)
//           if (pButton->GetImage() == 3)                            // +0x08 ? +0x4c : +0x48
//               pNav = DYNAMIC_DOWNCAST(CTasksPaneNavigateButton, pButton);
//       } else if (pButton->IsKindOf(CTasksPaneMenuButton))          // 0x1803b1508 (mfc140u)
//           pMenu = DYNAMIC_DOWNCAST(CTasksPaneMenuButton, pButton);
//   }
//   CRect rectClient(0,0,0,0); ::GetClientRect(m_hWnd, &rectClient);
//   if (pMenu != NULL) {
//       pMenu->m_rect.right = max(pMenu->m_rect.left + 3 * pMenu->m_rect.Height(),
//                                 rectClient.right - 1);             // signed (cmovg)
//       pMenu->OnMove();                                             // vtable +0x70, slot 14
//       if (pNav != NULL && pNav->m_bIsHidden != 1) {                // +0x54
//           pNav->m_bIsHidden = TRUE;
//           pNav->OnShow(FALSE);                                     // vtable +0x118, slot 35
//       }
//   }
//   UpdateTooltips();                                                // direct call, 0x159ce0
// Note the navigate-button block is nested inside `pMenu != NULL`, and
// UpdateTooltips is skipped only on the early-return path.
//
// DEVIATION (dispatch only, no behaviour lost): the two virtual calls are not
// made.  OpenMFC objects carry mingw (Itanium) vtables, so indexing slot 14 / 35
// of the object's vptr would call into unrelated data (see the note above
// CMFCToolBarButton::IsEditable in featurepack/visualmanager/CMFCVisualManagerOfficeXP.cpp).
// The only objects that can pass these IsKindOf tests are the retail-private
// CTasksPaneMenuButton / CTasksPaneNavigateButton, and in their retail vftables
// (0x180313df8 and 0x180314218, mfc140u, the vptrs their CreateObject exports
// install) slot 14 and slot 35 are both RVA 0x27d0 (mfc140u), a bare `ret`.
// The m_rect / m_bIsHidden stores are kept.
// Symbol: ?AdjustLocations@CMFCTasksPaneToolBar@@MEAAXXZ
extern "C" void MS_ABI impl__AdjustLocations_CMFCTasksPaneToolBar__MEAAXXZ(void* pThis)
{
    if (pThis == nullptr) return;
    HWND hWnd = HWndOf(pThis);
    if (hWnd == nullptr || !::IsWindow(hWnd)) return;

    impl__AdjustLocations_CMFCToolBar__MEAAXXZ(pThis);

    void* pNavClass  = impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ();
    void* pMenuClass = impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ();
    TbButtonView* pNav  = nullptr;
    TbButtonView* pMenu = nullptr;
    for (ObNode* node = ButtonsHead(pThis); node != nullptr; node = node->pNext) {
        TbButtonView* pButton = static_cast<TbButtonView*>(node->data);
        if (pButton->m_nStyle & kTbbsSeparator) continue;
        if (IsKindOf(pButton, pNavClass)) {
            if (pButton->GetImage() == 3)
                pNav = IsKindOf(pButton, pNavClass) ? pButton : nullptr;
        } else if (IsKindOf(pButton, pMenuClass)) {
            pMenu = IsKindOf(pButton, pMenuClass) ? pButton : nullptr;
        }
    }

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(HWndOf(pThis), &rectClient);

    if (pMenu != nullptr) {
        const int nMin = pMenu->m_rect.left + 3 * (pMenu->m_rect.bottom - pMenu->m_rect.top);
        const int nRight = rectClient.right - 1;
        pMenu->m_rect.right = (nMin > nRight) ? nMin : nRight;
        // pMenu->OnMove() (slot 14): retail body is `ret` -- see DEVIATION above.
        if (pNav != nullptr && pNav->m_bIsHidden != TRUE) {
            pNav->m_bIsHidden = TRUE;
            // pNav->OnShow(FALSE) (slot 35): retail body is `ret` -- see DEVIATION above.
        }
    }

    impl__UpdateTooltips_CMFCToolBar__IEAAXXZ(pThis);
}

// CMFCTasksPaneToolBar::CreateObject() -- transcribed from RVA 0x143a80 (mfc140u):
//   p = operator new(0x1360);                                        // ??2@YAPEAX_K@Z, 0x27f0
//   if (p) { CMFCToolBar::CMFCToolBar(p);                            // 0x14d2b0
//            p->vfptr = CMFCTasksPaneToolBar::`vftable';             // 0x1803143c8 (mfc140u)
//            p->m_pBtnBack = NULL; p->m_pBtnForward = NULL; }        // +0x1350 / +0x1358
//   return p;
// DEVIATION: OpenMFC has no CMFCTasksPaneToolBar vftable to install (the class
// is not declared in include/openmfc and no ??_7 vftable is exported), so the
// object keeps the vptr the exported CMFCToolBar constructor installs: it is
// correctly sized and initialised, but GetRuntimeClass reports CMFCToolBar and
// the overrides in this file are reached only through their exports.  This is
// the same approximation detail/DyncreateFactoriesSupport.h documents for the
// other RTTI-shell classes.
// Symbol: ?CreateObject@CMFCTasksPaneToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCTasksPaneToolBar__SAPEAVCObject__XZ()
{
    void* p = impl___2_YAPEAX_K_Z(kSizeofTasksPaneToolBar);
    if (p == nullptr) return nullptr;
    impl___0CMFCToolBar__QEAA_XZ(p);
    void* const kNull = nullptr;
    std::memcpy(Bytes(p) + kOffBtnBack, &kNull, sizeof kNull);
    std::memcpy(Bytes(p) + kOffBtnForward, &kNull, sizeof kNull);
    return p;
}

// CMFCTasksPaneToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM) -- transcribed
// from RVA 0x143b30 (mfc140u):
//   if (GetStyle() & WS_VISIBLE) {                                   // 0x2a9690; bt $0x1c
//       CWnd* pOwner = m_hWndOwner (+0xa0) != NULL
//                        ? CWnd::FromHandle(m_hWndOwner)
//                        : CWnd::FromHandle(::GetParent(m_hWnd));    // GetOwner() inlined
//       OnUpdateCmdUI((CFrameWnd*)pOwner, (BOOL)wParam);             // vtable +0x498, slot 147
//   }
//   return 0;
// DEVIATIONS:
//  * Owner: OpenMFC's CWnd does not name m_hWndOwner (+0xa0), so the
//    ::GetParent branch is taken unconditionally -- the convention already used
//    by featurepack/menu/CMFCMenuBar.cpp and CMFCPopupMenuBar.cpp (OwnerHwnd).
//  * Slot 147 is called as the CMFCToolBar::OnUpdateCmdUI export instead of
//    through the vptr (OpenMFC objects carry mingw vtables).  That is what the
//    retail CMFCTasksPaneToolBar vftable (0x1803143c8, mfc140u) holds at +0x498:
//    RVA 0x151c80 (mfc140u), which IS the
//    ?OnUpdateCmdUI@CMFCToolBar@@UEAAXPEAVCFrameWnd@@H@Z export (resolved
//    through the mfc140u export table by ordinal).  An override in a
//    client-derived class is not honoured.
//  * The pThis NULL guard has no retail counterpart (retail calls GetStyle on
//    `this` unconditionally).
// Symbol: ?OnIdleUpdateCmdUI@CMFCTasksPaneToolBar@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnIdleUpdateCmdUI_CMFCTasksPaneToolBar__IEAA_J_K_J_Z(
    void* pThis, unsigned __int64 wParam, __int64 lParam)
{
    (void)lParam;   // retail never reads it
    if (pThis == nullptr) return 0;
    if (impl__GetStyle_CWnd__QEBAKXZ(pThis) & WS_VISIBLE) {
        void* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
        impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(pThis, pOwner, static_cast<int>(wParam));
    }
    return 0;
}

// CMFCTasksPaneToolBar::OnUserToolTip(CMFCToolBarButton* pButton, CString& strTTText) const
// -- transcribed from RVA 0x143ec0 (mfc140u):
//   if (pButton->IsKindOf(RUNTIME_CLASS(CTasksPaneMenuButton))) {    // 0x1803b1508
//       ENSURE(strTTText.LoadString(0x4280));   // AfxFindStringResourceHandle 0x2aee00,
//                                               // LoadStringW(HINSTANCE, UINT) 0xdb70;
//                                               // NULL handle or FALSE -> AfxThrowInvalidArgException 0x227720
//       return TRUE;
//   }
//   if (pButton != NULL &&
//       (pButton->IsKindOf(CTasksPaneNavigateButton)                 // 0x1803b1598
//        || pButton->IsKindOf(CTasksPaneHistoryButton))) {           // 0x1803b1538
//       strTTText = pButton->m_strText;                              // +0x38, CSimpleStringT::operator= 0xde30
//       return TRUE;
//   }
//   return CMFCToolBar::OnUserToolTip(pButton, strTTText);           // direct call, 0x159370
// NULL pButton: retail's first IsKindOf runs before any NULL test, and retail
// CObject::IsKindOf (RVA 0x234cf0, mfc140u) begins with ENSURE(this != NULL) --
// `test %rcx,%rcx; je` to its call of AfxThrowInvalidArgException -- so a NULL
// button throws CInvalidArgException.  OpenMFC's IsKindOf thunk returns FALSE
// for NULL instead, so that throw is reproduced explicitly below.
// (The strTTText NULL guard has no retail counterpart: it is a reference.)
// Symbol: ?OnUserToolTip@CMFCTasksPaneToolBar@@MEBAHPEAVCMFCToolBarButton@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" int MS_ABI impl__OnUserToolTip_CMFCTasksPaneToolBar__MEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const void* pThis, void* pButtonArg, void* pStrTTText)
{
    if (pStrTTText == nullptr) return FALSE;
    TbButtonView* pButton = static_cast<TbButtonView*>(pButtonArg);
    if (pButton == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();   // retail: ENSURE inside IsKindOf
        return FALSE;   // not reached: the thunk throws
    }
    if (IsKindOf(pButton, impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ())) {
        HINSTANCE hInst = static_cast<HINSTANCE>(
            impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(kIdsMenuButtonToolTip));
        if (hInst == nullptr ||
            !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
                pStrTTText, hInst, kIdsMenuButtonToolTip)) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;   // not reached: the thunk throws
        }
        return TRUE;
    }
    if (pButton != nullptr &&
        (IsKindOf(pButton, impl__GetThisClass_CTasksPaneNavigateButton__SAPEAUCRuntimeClass__XZ()) ||
         IsKindOf(pButton, impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ()))) {
        impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(pStrTTText, &pButton->m_strText);
        return TRUE;
    }
    return impl__OnUserToolTip_CMFCToolBar__UEBAHPEAVCMFCToolBarButton__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
        pThis, pButton, pStrTTText);
}

// CMFCTasksPaneToolBar::UpdateButtons() -- transcribed from RVA 0x143d80 (mfc140u):
//   for (node = m_Buttons head (+0x1190); node; node = node->pNext) {
//       pButton = node->data;
//       if (pButton == NULL || !pButton->IsKindOf(CTasksPaneHistoryButton)) continue;   // 0x1803b1538
//       pTaskPane = pButton->+0x128; if (pTaskPane == NULL) continue;
//       if      (pButton->m_nID == 0x427c) pTaskPane->GetPreviousPages(pButton->+0x130);  // 0x14b930
//       else if (pButton->m_nID == 0x427d) pTaskPane->GetNextPages(pButton->+0x130);      // 0x14b9c0
//       CMenu menu; menu.Attach(::CreatePopupMenu());                // 0x2a8100
//       for (s = (+0x130 list) head (+0x138); s; s = s->pNext)
//           ::AppendMenuW(menu.m_hMenu, MF_STRING /*0*/, pButton->m_nID, s->data);
//       pButton->CreateFromMenu(menu.m_hMenu);                       // vtable +0x1b0, slot 54
//       // ~CMenu, inlined: if (m_hMenu) { remove from the permanent HMENU map
//       //   (AfxGetModuleThreadState 0x133a20, +0x30 map, RemoveKey); ::DestroyMenu(h); }
//   }
// (IAT slots resolved in mfc140u: 0x1802c6cc0 CreatePopupMenu,
//  0x1802c6cb8 AppendMenuW, 0x1802c6be0 DestroyMenu.)
// Slot 54 of the retail CTasksPaneHistoryButton vftable (0x180314008, mfc140u)
// is ?CreateFromMenu@CMFCToolBarMenuButton@@UEAAXPEAUHMENU__@@@Z (RVA 0x173d20,
// mfc140u); the class is retail-private, so that target is fixed and is called
// here through its export.
// DEVIATION: the local CMenu is replaced by a bare HMENU.  CMenu's constructor,
// destructor and vftable are not exported (no ??0/??1/??_7 CMenu entry in
// mfc_complete_ordinal_mapping.json), so a real CMenu cannot be built in this
// TU without a C++ symbol.  What that loses: the handle's entry in the
// permanent HMENU map between Attach and ~CMenu.  Retail CreateFromMenu
// (0x173d20, mfc140u) looks the handle up with CMenu::FromHandle (its call at
// 0x173d8d), which in retail therefore returns this local CMenu; without the
// permanent entry a retail-style FromHandle would build a temporary CMenu
// instead.  (OpenMFC's own CreateFromMenu does not call FromHandle today.)
// The handle is still created, filled, handed to CreateFromMenu and destroyed
// in retail's order.
// The pThis NULL guard has no retail counterpart (retail reads +0x1190 first).
// Symbol: ?UpdateButtons@CMFCTasksPaneToolBar@@QEAAXXZ
extern "C" void MS_ABI impl__UpdateButtons_CMFCTasksPaneToolBar__QEAAXXZ(void* pThis)
{
    if (pThis == nullptr) return;
    void* pHistClass = impl__GetThisClass_CTasksPaneHistoryButton__SAPEAUCRuntimeClass__XZ();
    for (ObNode* node = ButtonsHead(pThis); node != nullptr; node = node->pNext) {
        TbButtonView* pButton = static_cast<TbButtonView*>(node->data);
        if (pButton == nullptr || !IsKindOf(pButton, pHistClass)) continue;
        HistoryButtonView* pHist = reinterpret_cast<HistoryButtonView*>(pButton);
        void* pTaskPane = pHist->m_pParentTaskPane;
        if (pTaskPane == nullptr) continue;
        void* pPages = pHist->m_lstPages;

        if (pButton->m_nID == kIdHistoryBack)
            impl__GetPreviousPages_CMFCTasksPane__QEBAXAEAVCStringList___Z(pTaskPane, pPages);
        else if (pButton->m_nID == kIdHistoryForward)
            impl__GetNextPages_CMFCTasksPane__QEBAXAEAVCStringList___Z(pTaskPane, pPages);

        HMENU hMenu = ::CreatePopupMenu();
        for (StrNode* s = StringListHead(pPages); s != nullptr; s = s->pNext) {
            ::AppendMenuW(hMenu, MF_STRING, pButton->m_nID, s->data);
        }
        impl__CreateFromMenu_CMFCToolBarMenuButton__UEAAXPEAUHMENU_____Z(pButton, hMenu);
        if (hMenu != nullptr) ::DestroyMenu(hMenu);
    }
}

// CMFCTasksPaneToolBar::UpdateMenuButtonText(const CString& str) -- transcribed
// from RVA 0x143d20 (mfc140u):
//   for (node = m_Buttons head (+0x1190); node; node = node->pNext) {
//       pButton = node->data;
//       if (pButton != NULL && pButton->IsKindOf(CTasksPaneMenuButton))   // 0x1803b1508
//           pButton->m_strText = str;                                    // +0x38, operator= 0xde30
//   }
// Every matching button is updated (no early exit) and nothing is redrawn.
// The pThis / pStr NULL guards have no retail counterpart (retail reads +0x1190
// on entry, and str is a reference).
// Symbol: ?UpdateMenuButtonText@CMFCTasksPaneToolBar@@QEAAXAEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__UpdateMenuButtonText_CMFCTasksPaneToolBar__QEAAXAEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    void* pThis, const void* pStr)
{
    if (pThis == nullptr || pStr == nullptr) return;
    void* pMenuClass = impl__GetThisClass_CTasksPaneMenuButton__SAPEAUCRuntimeClass__XZ();
    for (ObNode* node = ButtonsHead(pThis); node != nullptr; node = node->pNext) {
        TbButtonView* pButton = static_cast<TbButtonView*>(node->data);
        if (pButton != nullptr && IsKindOf(pButton, pMenuClass))
            impl___4__CSimpleStringT__W_00_ATL__QEAAAEAV01_AEBV__CSimpleStringT__W_0A__1__Z(&pButton->m_strText, pStr);
    }
}
