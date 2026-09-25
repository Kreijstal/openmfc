// CVSListBoxBase — OpenMFC implementation.
// Sources: dlgcommon.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/DlgcommonSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdint>

// ---------------------------------------------------------------------------
// Retail layout used by the bodies below.
//
// OpenMFC's CVSListBoxBase (include/openmfc/afxmfc.h) declares NO data members,
// so there is no shadow struct for it.  The offsets used here are the ones the
// retail bodies address, cross-checked against the member order in
// atlmfc/include/afxvslistbox.h:43 (CStatic ends at 0xe8 == sizeof(CWnd)):
//   0x0e8 m_lstButtons   CList<CMFCButton*>   (0x38 bytes)
//   0x120 m_lstKeyAccell CList<DWORD>         (0x38 bytes)
//   0x158 m_mapButtonIDs CMap<int,int,UINT,UINT> (0x38 bytes)
//   0x190 m_uiStandardBtns   -- read by OnClickButton (mfc140u 0x1c5cc0)
//   0x194 m_bNewItem         -- written by CreateNewItem / OnEndEditLabel
//   0x198 m_bIsActualDelete  -- written by OnClickButton
//   0x1a0 m_bGrayDisabledButtons -- written by SetGrayDisabledButtons
//   0x1a4 m_bDefaultCaption, 0x1b0 m_rectCaption, 0x1c0 m_font, 0x1d0 m_strCaption
//                            -- read by OnPaint (mfc140u 0x1c56f0)
// Only the scalar BOOLs are touched by the bodies implemented in this file.
// NOTE: OpenMFC's exported ctor (??0CVSListBoxBase, featurepack/controls/
// CtorDtorPlacement.cpp) constructs only the 0xe8-byte CStatic part, so none of
// these members is initialised in an object built through it; see the
// headerRequests of the workflow that wrote this.
//
// Virtual calls use the retail vtable byte offsets read at the call sites:
//   0x2e8 AddItem, 0x2f0 RemoveItem, 0x300 GetSelItem, 0x318 SetItemText,
//   0x330 EditItem, 0x368 OnAfterAddItem, 0x370 OnAfterRenameItem,
//   0x3a0 OnCreateList.
// In the CVSListBox vftable at 0x18031f748 (mfc140u), slots 0x2e8/0x2f0/0x300/
// 0x318/0x330/0x3a0 hold the named CVSListBox exports.  Slots 0x368 and 0x370
// hold 0x27d0 (mfc140u), an identical-code-folded empty `ret` shared by 158
// exports, so the vftable does not name them; their identity comes from the
// virtual declaration order in afxvslistbox.h (EditItem 0x330, OnClickButton
// 0x338, OnKey 0x340, OnEndEditLabel 0x348, OnGetImage 0x350,
// OnSelectionChanged 0x358, OnBeforeRemoveItem 0x360, OnAfterAddItem 0x368,
// OnAfterRenameItem 0x370, ...), which agrees with every named slot around them.
// These helpers assume `this` carries a retail-layout (MSVC) vtable, i.e. the
// thunk was reached through a client class compiled against the real
// afxvslistbox.h.  Init is also reached that way through PreSubclassWindow;
// retail's OnCreate (0x1c5030, mfc140u) calls Init too, but this file's OnCreate
// thunk is still a placeholder that does not.  An object whose vptr is OpenMFC's
// own C++ vtable (built by `new (p) CVSListBox()` in CtorDtorPlacement.cpp)
// does not have these slots; OpenMFC's C++ vtable does not route to these
// thunks, but a client calling one of the public virtuals (OnKey,
// OnEndEditLabel) directly on such an object would dispatch through the wrong
// table.
// ---------------------------------------------------------------------------
namespace {

constexpr std::size_t kOffNewItem            = 0x194;  // m_bNewItem
constexpr std::size_t kOffGrayDisabledButtons = 0x1a0; // m_bGrayDisabledButtons

constexpr std::size_t kSlotAddItem          = 0x2e8 / 8;  // AddItem(const CString&, DWORD_PTR, int)
constexpr std::size_t kSlotRemoveItem       = 0x2f0 / 8;  // RemoveItem(int)
constexpr std::size_t kSlotGetSelItem       = 0x300 / 8;  // GetSelItem() const
constexpr std::size_t kSlotSetItemText      = 0x318 / 8;  // SetItemText(int, const CString&)
constexpr std::size_t kSlotEditItem         = 0x330 / 8;  // EditItem(int)
constexpr std::size_t kSlotOnAfterAddItem   = 0x368 / 8;  // OnAfterAddItem(int)
constexpr std::size_t kSlotOnAfterRenameItem = 0x370 / 8; // OnAfterRenameItem(int)
constexpr std::size_t kSlotOnCreateList     = 0x3a0 / 8;  // OnCreateList()

inline int& IntAt(void* p, std::size_t off) {
    return *reinterpret_cast<int*>(static_cast<unsigned char*>(p) + off);
}
inline void* SlotOf(void* p, std::size_t slot) { return (*reinterpret_cast<void***>(p))[slot]; }

inline int VAddItem(void* p, const CString& str, uintptr_t dwData, int iIndex) {
    using Fn = int (MS_ABI*)(void*, const CString&, uintptr_t, int);
    return reinterpret_cast<Fn>(SlotOf(p, kSlotAddItem))(p, str, dwData, iIndex);
}
inline int VRemoveItem(void* p, int iIndex) {
    using Fn = int (MS_ABI*)(void*, int);
    return reinterpret_cast<Fn>(SlotOf(p, kSlotRemoveItem))(p, iIndex);
}
inline int VGetSelItem(void* p) {
    using Fn = int (MS_ABI*)(void*);
    return reinterpret_cast<Fn>(SlotOf(p, kSlotGetSelItem))(p);
}
inline void VSetItemText(void* p, int iIndex, const CString& str) {
    using Fn = void (MS_ABI*)(void*, int, const CString&);
    reinterpret_cast<Fn>(SlotOf(p, kSlotSetItemText))(p, iIndex, str);
}
inline int VEditItem(void* p, int iIndex) {
    using Fn = int (MS_ABI*)(void*, int);
    return reinterpret_cast<Fn>(SlotOf(p, kSlotEditItem))(p, iIndex);
}
inline void VOnAfterAddItem(void* p, int iItem) {
    using Fn = void (MS_ABI*)(void*, int);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnAfterAddItem))(p, iItem);
}
inline void VOnAfterRenameItem(void* p, int iItem) {
    using Fn = void (MS_ABI*)(void*, int);
    reinterpret_cast<Fn>(SlotOf(p, kSlotOnAfterRenameItem))(p, iItem);
}
inline CWnd* VOnCreateList(void* p) {
    using Fn = CWnd* (MS_ABI*)(void*);
    return reinterpret_cast<Fn>(SlotOf(p, kSlotOnCreateList))(p);
}

// _AFX_THREAD_STATE as OpenMFC lays it out (detail/CWinAppSupport.h:280):
// retail keeps m_pWndInit at +0x28; OpenMFC keeps the same member as pWndInit at
// +0x10.  Same mirror as featurepack/controls/CMFCShellListCtrl.cpp.
struct AfxThreadStateMirror {
    int nTempMapLock;
    int nWndCreateLock;
    void* pModuleState;
    void* pWndInit;
    void* hHookOldCbtFilter;
};
static_assert(offsetof(AfxThreadStateMirror, pWndInit) == 16, "AfxThreadStateMirror::pWndInit");

// SS_USERITEM (winuser.h) -- the style bit retail Init adds.
constexpr unsigned long kStyleUserItem = 0xA;
static_assert(kStyleUserItem == SS_USERITEM, "Init's ModifyStyle immediate");

} // namespace

// ---- sibling impl__ exports called by the bodies in this file -------------
struct _AFX_THREAD_STATE;
extern "C" _AFX_THREAD_STATE* MS_ABI impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ(); // detail/CWinAppSupport.cpp
extern "C" int MS_ABI impl__ModifyStyle_CWnd__QEAAHKKI_Z(CWnd* pThis, unsigned long dwRemove, unsigned long dwAdd, unsigned int nFlags); // core/window/CWnd.cpp

// Symbol: ?AddButton@CVSListBoxBase@@QEAAHIPEB_WGEI@Z
extern "C" int MS_ABI impl__AddButton_CVSListBoxBase__QEAAHIPEB_WGEI_Z(
    CVSListBoxBase* pThis, unsigned int nID, const wchar_t* lpszName, unsigned short, unsigned char, unsigned int) {
    (void)pThis;
    (void)lpszName;
    return (int)nID;
}
// Symbol: ?AdjustLayout@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__AdjustLayout_CVSListBoxBase__IEAAXXZ(CVSListBoxBase* pThis) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
}
// Transcribed from retail entry RVA 0x1c5f40 (mfc140u):
//     int iItem = AddItem(CString(L""), 0, -1);   // vslot 0x2e8; the temporary is
//                                                 // built from the empty literal at
//                                                 // 0x18033d19c and released here
//     m_bNewItem = TRUE;                          // +0x194
//     EditItem(iItem);                            // vslot 0x330, tail call
// Retail has no NULL-this guard; one is added.
// Symbol: ?CreateNewItem@CVSListBoxBase@@MEAAXXZ
extern "C" void MS_ABI impl__CreateNewItem_CVSListBoxBase__MEAAXXZ(CVSListBoxBase* pThis) {
    if (pThis == nullptr) return;
    int iItem;
    {
        CString strEmpty(L"");
        iItem = VAddItem(pThis, strEmpty, 0, -1);
    }
    IntAt(pThis, kOffNewItem) = TRUE;
    (void)VEditItem(pThis, iItem);
}
// Symbol: ?EnableBrowseButton@CVSListBoxBase@@QEAAXH@Z
extern "C" void MS_ABI impl__EnableBrowseButton_CVSListBoxBase__QEAAXH_Z(CVSListBoxBase* pThis, int bEnable) {
    if (pThis && pThis->GetSafeHwnd()) ::EnableWindow(pThis->GetSafeHwnd(), bEnable);
}
// Symbol: ?EnableButton@CVSListBoxBase@@QEAAHHH@Z
extern "C" void MS_ABI impl__EnableButton_CVSListBoxBase__QEAAHHH_Z(CVSListBoxBase* pThis, int, int bEnable) {
    if (pThis && pThis->GetSafeHwnd()) ::EnableWindow(pThis->GetSafeHwnd(), bEnable);
}
// Symbol: ?GetButtonID@CVSListBoxBase@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetButtonID_CVSListBoxBase__QEBAIH_Z(const CVSListBoxBase* pThis, int nIndex) {
    (void)pThis;
    return (unsigned int)nIndex;
}
// Symbol: ?GetButtonNum@CVSListBoxBase@@QEBAHI@Z
extern "C" int MS_ABI impl__GetButtonNum_CVSListBoxBase__QEBAHI_Z(const CVSListBoxBase* pThis, unsigned int nID) {
    (void)pThis;
    return (int)nID;
}
// GetMessageMap/GetThisMessageMap for CVSListBoxBase live in featurepack/controls/MessageMaps.cpp
// (classCVSListBoxBase_msgmap, base map CWnd). Retail agrees: ?GetThisMessageMap@
// CVSListBoxBase@@KAPEBUAFX_MSGMAP@@XZ in mfc140.dll returns the AFX_MSGMAP at
// 0x18031d948, whose pfnGetBaseMap is ?GetThisMessageMap@CWnd@@KAPEBUAFX_MSGMAP@@XZ --
// CWnd, even though the class's CRuntimeClass base is CStatic. The copies that used to be
// here returned CStatic's map directly.
// Symbol: ?GetRuntimeClass@CVSListBoxBase@@UEBAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetRuntimeClass_CVSListBoxBase__UEBAPEAUCRuntimeClass__XZ(const CVSListBoxBase* pThis) {
    return pThis ? pThis->GetRuntimeClass() : CVSListBoxBase::GetThisClass();
}
// Symbol: ?GetStdButtonNum@CVSListBoxBase@@IEBAHI@Z
extern "C" int MS_ABI impl__GetStdButtonNum_CVSListBoxBase__IEBAHI_Z(const CVSListBoxBase* pThis, unsigned int nID) {
    (void)pThis;
    return (int)nID;
}
// Symbol: ?GetThisClass@CVSListBoxBase@@SAPEAUCRuntimeClass@@XZ
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CVSListBoxBase__SAPEAUCRuntimeClass__XZ() {
    return CVSListBoxBase::GetThisClass();
}
// Transcribed from retail entry RVA 0x1c5060 (mfc140u):
//     ModifyStyle(0, SS_USERITEM /*0xA*/, 0);     // ?ModifyStyle@CWnd@@QEAAHKKI@Z, 0x2a96f0
//     if (OnCreateList() != NULL)                 // vslot 0x3a0
//         AdjustLayout();                         // 0x1c5990
// AdjustLayout is the thunk defined above in this file; its body is still a
// placeholder (InvalidateRect), not a transcription of retail 0x1c5990.
// Retail has no NULL-this guard; one is added.
// Symbol: ?Init@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__Init_CVSListBoxBase__IEAAXXZ(CVSListBoxBase* pThis) {
    if (pThis == nullptr) return;
    impl__ModifyStyle_CWnd__QEAAHKKI_Z(static_cast<CWnd*>(pThis), 0, kStyleUserItem, 0);
    if (VOnCreateList(pThis) != nullptr) {
        impl__AdjustLayout_CVSListBoxBase__IEAAXXZ(pThis);
    }
}
// STUB.  Retail entry RVA 0x1c5cc0 (mfc140u) returns at once when
// m_uiStandardBtns (+0x190) is 0, otherwise reads GetSelItem (vslot 0x300) and
// maps the button index through GetButtonID (0x1c6290), then dispatches on the
// standard IDs: -11 NEW -> CreateNewItem (vslot 0x390); -12 DELETE -> if the
// selection is >= 0 and OnBeforeRemoveItem(sel) (0x360) returns nonzero,
// RemoveItem(sel) (0x2f0); -13 UP / -14 DOWN -> return unless sel >= 0 and
// (UP) sel != 0 or (DOWN) sel != GetCount()-1 (0x2f8), then a WM_SETREDRAW-
// bracketed (sent to this window) GetItemText/GetItemData/RemoveItem/AddItem(
// sel-1 or sel+1)/SelectItem move (m_bIsActualDelete, +0x198, is set FALSE before
// the RemoveItem and TRUE after it), ::InvalidateRect on the GetListHwnd window,
// then OnAfterMoveItemUp/Down (0x378/0x380).  Other IDs do nothing.  Not transcribed: this file's
// GetButtonID is still a placeholder that returns its argument (retail looks the
// index up in m_lstButtons/m_mapButtonIDs), and those members are never
// constructed by OpenMFC's ctor, so any transcription would dispatch on wrong IDs.
// Symbol: ?OnClickButton@CVSListBoxBase@@UEAAXH@Z
extern "C" void MS_ABI impl__OnClickButton_CVSListBoxBase__UEAAXH_Z(CVSListBoxBase* pThis, int) {
    (void)pThis;
}
// STUB.  Retail entry RVA 0x1c5c00 (mfc140u) walks m_lstButtons (CList head at
// +0xf0; ENSURE on a NULL button) comparing each button's m_hWnd (+0x40) with
// lParam; on a match it
// focuses the list (GetListHwnd vslot 0x398 -> CWnd::FromHandle -> SetFocus),
// calls OnClickButton(index) (vslot 0x338) and returns TRUE; with no match it
// returns CWnd::OnCommand(wParam, lParam) (0x28e040).  Not transcribed: OpenMFC's
// ctor never constructs m_lstButtons (+0xe8), so walking it would read
// uninitialised memory.
// Symbol: ?OnCommand@CVSListBoxBase@@MEAAH_K_J@Z
extern "C" int MS_ABI impl__OnCommand_CVSListBoxBase__MEAAH_K_J_Z(CVSListBoxBase* pThis, unsigned __int64, __int64) {
    (void)pThis;
    return FALSE;
}
// Symbol: ?OnCreate@CVSListBoxBase@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CVSListBoxBase__IEAAHPEAUtagCREATESTRUCTW___Z(CVSListBoxBase* pThis, CREATESTRUCTW*) {
    return pThis ? 0 : -1;
}
// Symbol: ?OnEnable@CVSListBoxBase@@IEAAXH@Z
extern "C" void MS_ABI impl__OnEnable_CVSListBoxBase__IEAAXH_Z(CVSListBoxBase* pThis, int bEnable) {
    if (pThis && pThis->GetSafeHwnd()) ::EnableWindow(pThis->GetSafeHwnd(), bEnable);
}
// Transcribed from retail entry RVA 0x1c6150 (mfc140u):
//     int iSelItem = GetSelItem();                         // vslot 0x300
//     if (iSelItem < 0) return;
//     CString strLabel(lpszLabel == NULL ? L"" : lpszLabel);
//     if (!strLabel.IsEmpty()) {
//         SetItemText(iSelItem, strLabel);                 // vslot 0x318
//         if (m_bNewItem) OnAfterAddItem(iSelItem);        // +0x194; vslot 0x368
//         else            OnAfterRenameItem(iSelItem);     // vslot 0x370
//     } else if (m_bNewItem) {
//         RemoveItem(iSelItem);                            // vslot 0x2f0
//     }
//     m_bNewItem = FALSE;
// (m_bNewItem is cleared on every path that gets past the iSelItem < 0 return.)
// Retail has no NULL-this guard; one is added.
// Symbol: ?OnEndEditLabel@CVSListBoxBase@@UEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnEndEditLabel_CVSListBoxBase__UEAAXPEB_W_Z(CVSListBoxBase* pThis, const wchar_t* lpszLabel) {
    if (pThis == nullptr) return;
    const int iSelItem = VGetSelItem(pThis);
    if (iSelItem < 0) return;
    CString strLabel(lpszLabel == nullptr ? L"" : lpszLabel);
    if (!strLabel.IsEmpty()) {
        VSetItemText(pThis, iSelItem, strLabel);
        if (IntAt(pThis, kOffNewItem) != 0) {
            VOnAfterAddItem(pThis, iSelItem);
        } else {
            VOnAfterRenameItem(pThis, iSelItem);
        }
    } else if (IntAt(pThis, kOffNewItem) != 0) {
        (void)VRemoveItem(pThis, iSelItem);
    }
    IntAt(pThis, kOffNewItem) = FALSE;
}
// Retail: the export's ordinal resolves to RVA 0x3a60 (mfc140u), an identical-
// code-folded `mov $0x1,%eax; ret` shared with 100 other `return TRUE` exports
// (CBasePane::OnEraseBkgnd among them).  The background is left to OnPaint.
// Symbol: ?OnEraseBkgnd@CVSListBoxBase@@IEAAHPEAVCDC@@@Z
extern "C" int MS_ABI impl__OnEraseBkgnd_CVSListBoxBase__IEAAHPEAVCDC___Z(CVSListBoxBase* pThis, CDC* pDC) {
    (void)pThis;
    (void)pDC;
    return TRUE;
}
// Symbol: ?OnGetDlgCode@CVSListBoxBase@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetDlgCode_CVSListBoxBase__IEAAIXZ(CVSListBoxBase*) {
    return DLGC_WANTARROWS | DLGC_WANTCHARS;
}
// Transcribed from retail entry RVA 0x1c5fe0 (mfc140u):
//     int iSelItem = GetSelItem();                                // vslot 0x300
//     WORD ch = (WORD)::MapVirtualKey(wKey, MAPVK_VK_TO_CHAR);    // IAT 0x1802c6d80
//     if (fFlags == 0 && iSelItem >= 0 && (ch == L' ' || wKey == VK_F2))
//         EditItem(iSelItem);                                     // vslot 0x330
// GetSelItem and MapVirtualKey both run before any test.  The IAT slot
// resolves to USER32!MapVirtualKeyW in mfc140u.  Retail has no NULL-this guard;
// one is added.
// Symbol: ?OnKey@CVSListBoxBase@@UEAAXGE@Z
extern "C" void MS_ABI impl__OnKey_CVSListBoxBase__UEAAXGE_Z(CVSListBoxBase* pThis, unsigned short wKey, unsigned char fFlags) {
    if (pThis == nullptr) return;
    const int iSelItem = VGetSelItem(pThis);
    const unsigned short ch = static_cast<unsigned short>(::MapVirtualKeyW(wKey, MAPVK_VK_TO_CHAR));
    if (fFlags == 0 && iSelItem >= 0 && (ch == 0x20 || wKey == VK_F2)) {
        (void)VEditItem(pThis, iSelItem);
    }
}
// Symbol: ?OnSetFocus@CVSListBoxBase@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CVSListBoxBase__IEAAXPEAVCWnd___Z(CVSListBoxBase* pThis, CWnd*) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, FALSE);
}
// Symbol: ?OnSetText@CVSListBoxBase@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CVSListBoxBase__IEAAHPEB_W_Z(CVSListBoxBase* pThis, const wchar_t* lpszText) {
    return pThis && pThis->GetSafeHwnd() ? ::SetWindowTextW(pThis->GetSafeHwnd(), lpszText) : FALSE;
}
// Symbol: ?OnSize@CVSListBoxBase@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CVSListBoxBase__IEAAXIHH_Z(CVSListBoxBase* pThis, unsigned int, int, int) {
    if (pThis && pThis->GetSafeHwnd()) ::InvalidateRect(pThis->GetSafeHwnd(), nullptr, TRUE);
}
// Transcribed from retail entry RVA 0x1c5000 (mfc140u):
//     _AFX_THREAD_STATE* pThreadState = AfxGetThreadState();  // 0x1332a0
//     if (pThreadState->m_pWndInit == NULL)                  // retail +0x28
//         Init();                                            // 0x1c5060
// Retail makes no base-class PreSubclassWindow call (CWnd's is empty).  OpenMFC
// keeps m_pWndInit as _AFX_THREAD_STATE::pWndInit (read through
// AfxThreadStateMirror above).  Retail dereferences the thread state and `this`
// unconditionally; NULL guards are added.
// Symbol: ?PreSubclassWindow@CVSListBoxBase@@MEAAXXZ
extern "C" void MS_ABI impl__PreSubclassWindow_CVSListBoxBase__MEAAXXZ(CVSListBoxBase* pThis) {
    if (pThis == nullptr) return;
    const AfxThreadStateMirror* pThreadState =
        reinterpret_cast<const AfxThreadStateMirror*>(impl__AfxGetThreadState__YAPEAV_AFX_THREAD_STATE__XZ());
    if (pThreadState == nullptr || pThreadState->pWndInit == nullptr) {
        impl__Init_CVSListBoxBase__IEAAXXZ(pThis);
    }
}
// Transcribed from retail entry RVA 0x1c6340 (mfc140u): a single store,
//     m_bGrayDisabledButtons = bOn;               // +0x1a0
// with no redraw.  Retail has no NULL-this guard; one is added.
// Symbol: ?SetGrayDisabledButtons@CVSListBoxBase@@QEAAXH@Z
extern "C" void MS_ABI impl__SetGrayDisabledButtons_CVSListBoxBase__QEAAXH_Z(CVSListBoxBase* pThis, int bOn) {
    if (pThis == nullptr) return;
    IntAt(pThis, kOffGrayDisabledButtons) = bOn;
}
// Symbol: ?SetStandardButtons@CVSListBoxBase@@QEAAHI@Z
extern "C" int MS_ABI impl__SetStandardButtons_CVSListBoxBase__QEAAHI_Z(CVSListBoxBase* pThis, unsigned int nButtons) {
    (void)pThis;
    return (int)nButtons;
}
// Symbol: ?OnGetFont@CVSListBoxBase@@IEAAPEAUHFONT__@@XZ
extern "C" void* MS_ABI impl__OnGetFont_CVSListBoxBase__IEAAPEAUHFONT____XZ(void* pThis) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd->GetSafeHwnd() == nullptr) {
        return nullptr;
    }
    auto* pFontHandle = reinterpret_cast<void*>(
        static_cast<uintptr_t>(pWnd->SendMessageW(WM_GETFONT, 0, 0)));
    return pFontHandle;
}
// STUB.  Retail entry RVA 0x1c56f0 (mfc140u) paints the caption strip with a
// CPaintDC: ::FillRect(m_rectCaption (+0x1b0), the afxGlobalData brush at +0xb0),
// CDC::Draw3dRect, ::InflateRect(-5, 0), SetBkMode(TRANSPARENT), text colour
// chosen by IsWindowEnabled, selects
// m_font (+0x1c0) if its handle is non-NULL, else the parent's WM_GETFONT font
// (skipped when that is NULL; ENSURE -- AfxThrowInvalidArgException -- when the
// SelectObject of the parent font returns NULL), then draws
// either the window text (m_bDefaultCaption, +0x1a4) or m_strCaption (+0x1d0)
// with DT_SINGLELINE|DT_VCENTER.  Not transcribed: m_font and m_strCaption are
// never constructed by OpenMFC's ctor.  CVSListBoxBase's OpenMFC message map
// (featurepack/controls/MessageMaps.cpp) has no WM_PAINT entry, so this thunk is
// not reached by OpenMFC's own dispatch.
// Symbol: ?OnPaint@CVSListBoxBase@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaint_CVSListBoxBase__IEAAXXZ(void* pThis) {
    (void)pThis;
}
// Symbol: ?OnSetFont@CVSListBoxBase@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CVSListBoxBase__IEAAXPEAVCFont__H_Z(
    void* pThis, void* pFont, int redraw) {
    auto* pWnd = static_cast<CWnd*>(pThis);
    if (pWnd == nullptr) {
        return;
    }
    if (pWnd->GetSafeHwnd() == nullptr) {
        return;
    }
    HFONT hFont = nullptr;
    if (pFont != nullptr) {
        hFont = reinterpret_cast<HFONT>(
            static_cast<CFont*>(pFont)->GetSafeHandle());
    }
    pWnd->SendMessageW(WM_SETFONT, reinterpret_cast<uintptr_t>(hFont), redraw);
}
