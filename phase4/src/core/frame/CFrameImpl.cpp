// CFrameImpl — OpenMFC implementation.
// Sources: cbarcore.cpp
//
// CFrameImpl is the shared implementation object that CFrameWndEx / CMDIFrameWndEx
// hang off themselves.  Behaviour below is decoded from the retail export named in
// each comment (the project's established method -- see the header of
// phase4/src/core/ole/COleControl.cpp), disassembled out of mfc140.dll.
//
// RETAIL MEMBER MAP, transcribed from the constructor ??0CFrameImpl@@QEAA@PEAVCFrameWnd@@@Z
// (RVA 0x61d20) and cross-checked against the accessors named on each line.  OpenMFC's
// CFrameImpl has no such layout: `pThis` here is an opaque token and the only fields
// OpenMFC keeps are the three in FrameImplState (detail/CbarcoreSupport.h).  Every
// member listed here that OpenMFC has no storage for is the reason a body below is
// still a stub.
//
//   +0x08 UINT   first user-toolbar id     ctor stores -1; written by InitUserToolbars 0x626a0
//   +0x0c UINT   last user-toolbar id      ctor stores -1; read by IsUserDefinedToolbar 0x62670
//   +0x14 UINT   hot caption-button id     OnLButtonDown 0x662c0, OnTrackCaptionButtons 0x664d0
//   +0x18 UINT   pressed caption-button id OnLButtonUp 0x66310
//   +0x20 int    gate tested by OnLButtonUp/OnNcMouseMove (purpose not established)
//   +0x24 int    written by OnWindowPosChanging 0x650e0 with the result of the visual
//                manager's vtable slot +0x400, called on the instance returned by the
//                CMFCVisualManager::GetInstance helper (0x97f4) with m_pFrame (+0x118)
//                and the packed WINDOWPOS cx/cy as ARGUMENTS -- it is NOT a virtual on
//                m_pFrame; the same shape appears in OnLButtonUp / OnMouseMove /
//                OnNcMouseMove / OnNcActivate / OnNcPaint (slots +0x3f8/+0x408/+0x418)
//   +0x2c BOOL   m_pFrame->IsKindOf(RUNTIME_CLASS(CMDIChildWnd))  -- the descriptor at
//                0x180339bf0 carries the name "CMDIChildWnd" (read out of the image)
//   +0x30 int    a gate: non-zero makes OnActivateApp / OnNcActivate / OnNcHitTest /
//                OnMouseMove / OnNcMouseMove / OnLButtonUp / OnWindowPosChanging return
//                without doing anything (what sets it was not established)
//   +0x34 int    ctor stores 1
//   +0x38        ctor stores 1 with an 8-byte store; LoadDockState 0x64610 tests its
//                low dword and skips the forward when it is zero
//   +0x40 int    re-entrancy flag set/cleared inside OnWindowPosChanging 0x650e0
//   +0x50..0x5f  CRect, the caption-button repaint rect (RedrawCaptionButton 0x66660)
//   +0x60 CPtrList m_lstUserToolbars    (head +0x68, count +0x78, block size 10)
//                DeleteToolBar 0x63350, GetUserToolBarByIndex 0x62640, CreateNewToolBar 0x62820
//   +0x98 CPtrList tear-off toolbars    (head +0xa0)
//                AddTearOffToolbar 0x62a40, RemoveTearOffToolbar 0x62a50, LoadTearOffMenus 0x62a90
//   +0xd0 CPtrList caption ("system") buttons (head +0xd8)   GetSysButton 0x66480
//   +0x108 CString  user-toolbar profile name; InitUserToolbars 0x626a0 fills it and
//                LoadUserToolbars 0x62490 passes it to CMFCToolBar::LoadState
//   +0x110 CMFCMenuBar*   written by SetMenuBar 0x63480
//   +0x118 CFrameWnd*     m_pFrame -- the constructor argument
//   +0x120 CDockingManager*  LoadUserToolbars 0x62490 passes it to
//                ?DockPane@CDockingManager@@QEAAXPEAVCBasePane@@IPEBUtagRECT@@@Z (0x483c0)
//   +0x178 CRuntimeClass* for newly created user toolbars; ctor default is the descriptor
//                at 0x1803aa5f8, whose name reads "CMFCToolBar"
//   +0x180 CMFCRibbonBar* -- proven by DeactivateMenu 0x646e0 / OnActivateApp 0x65890
//                passing it as `this` to ?DeactivateKeyboardFocus@CMFCRibbonBar@@ (0xe28e0),
//                ?HideKeyTips@CMFCRibbonBar@@ (0xe4050) and ?OnCancelMode@CMFCRibbonBar@@ (0xdcf20)

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
// For the CObList side table: OpenMFC's list wrappers keep their contents in a
// per-object state map (OPENMFC_DECLARE_LIST_WRAPPER, include/openmfc/afx.h), not in
// the object's own bytes.  FindListStorage below is the header-only accessor for it.
#include "detail/FilecoreSupport.h"

// ---------------------------------------------------------------------------
// Thunks for DLL-internal calls (see the briefing's rule 1: most C++ methods in
// this DLL exist only as extern "C" impl__ thunks).
// ---------------------------------------------------------------------------
extern "C" HWND MS_ABI impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(const CWnd* pThis);
extern "C" int MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis,
                                                                        const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();
extern "C" void MS_ABI
impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* ret, CWinAppEx* pThis, const wchar_t* add);
extern "C" int MS_ABI impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(const wchar_t* lpszProfileName);

namespace {

// A minimal singly linked pointer list.  Retail keeps these as CPtrList members of
// CFrameImpl.  OpenMFC's list WRAPPERS (CObList / CPtrList) cannot be called from this
// translation unit -- their methods are out-of-line C++ symbols in
// detail/FilecoreSupport.cpp and this file's link audit rejects any new C++-mangled
// undefined -- and std::vector / std::list / std::wstring each drag in libstdc++
// helpers the audit rejects for the same reason.  (The header-only FindListStorage
// accessor used by GetFreeCtrlBarID below is the one thing that does get through, but
// it only READS an existing wrapper's state; it cannot stand in for a member list of
// our own.)  So the storage below is hand-rolled and uses nothing but operator
// new/delete.
struct PtrNode {
    void* pData;
    PtrNode* pNext;
};

void PtrListAddTail(PtrNode*& pHead, void* pData) {
    PtrNode* pNode = new PtrNode();
    pNode->pData = pData;
    pNode->pNext = nullptr;
    if (pHead == nullptr) {
        pHead = pNode;
        return;
    }
    PtrNode* pTail = pHead;
    while (pTail->pNext != nullptr) {
        pTail = pTail->pNext;
    }
    pTail->pNext = pNode;
}

void PtrListRemoveAll(PtrNode*& pHead) {
    while (pHead != nullptr) {
        PtrNode* pNext = pHead->pNext;
        delete pHead;
        pHead = pNext;
    }
}

// Per-CFrameImpl state that FrameImplState (detail/CbarcoreSupport.h, not owned by
// this file) has no room for.  It is file-local on purpose: every ?...@CFrameImpl@@
// export lives in this translation unit, so nothing outside can observe it.
struct FrameImplExtra {
    UINT uiUserToolbarFirst = static_cast<UINT>(-1);   // retail +0x08
    UINT uiUserToolbarLast = static_cast<UINT>(-1);    // retail +0x0c
    CString strUserToolbarsProfile;                    // retail +0x108
    PtrNode* pTearOffToolbars = nullptr;               // retail +0x98 (CPtrList)
};

std::mutex g_frameImplExtraMutex;
std::unordered_map<const void*, FrameImplExtra> g_frameImplExtras;

// Retail keeps the frame list in the exported CList below.  OpenMFC's list classes
// keep their contents in a side table rather than in the object's own bytes (see
// OPENMFC_DECLARE_LIST_WRAPPER in include/openmfc/afx.h), so the working storage for
// AddFrame/RemoveFrame is here and the exported object is ABI storage only.
std::mutex g_frameListMutex;
PtrNode* g_pFrameListHead = nullptr;

}  // namespace

// Symbol: ??0CFrameImpl@@QEAA@PEAVCFrameWnd@@@Z
extern "C" void* MS_ABI impl___0CFrameImpl__QEAA_PEAVCFrameWnd___Z(void* pThis, CFrameWnd* pFrame) {
    if (pThis == nullptr) {
        return nullptr;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    g_frameImplStates.insert_or_assign(frameImpl, FrameImplState{pFrame, nullptr, manager != nullptr ? manager->GetRuntimeClass() : nullptr});
    return pThis;
}
// Symbol: ??1CFrameImpl@@UEAA@XZ
extern "C" void MS_ABI impl___1CFrameImpl__UEAA_XZ(void* pThis) {
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        (void)g_frameImplStates.erase(frameImpl);
    }
    // Drop the file-local companion state as well, or it outlives the object.
    std::lock_guard<std::mutex> extraLock(g_frameImplExtraMutex);
    auto it = g_frameImplExtras.find(pThis);
    if (it != g_frameImplExtras.end()) {
        PtrListRemoveAll(it->second.pTearOffToolbars);
        g_frameImplExtras.erase(it);
    }
}
// Retail 0x64130 reads m_pDockManager (+0x120) three times: it gates on
// m_pDockManager->[+0x308]->[+0x8]/[+0xc], then builds a panes menu into a
// ::CreatePopupMenu handle with ?BuildPanesMenu@CDockingManager@@ (0x4df60), and
// finally re-reads +0x120 again.  The rest is CMFCPopupMenu work -- RemoveAllItems
// (0xb7eb0), the popup's vtable slots +0x3a0 and +0x880, IsCustomizePane (0x64fc0)
// and ShowQuickCustomizePane (0x647d0).  It never reads m_pFrame (+0x118).
// OpenMFC's CFrameImpl carries neither the dock-manager pointer nor any CMFCPopupMenu
// item model, so this stays a stub.
// Symbol: ?OnShowPopupMenu@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@PEAVCFrameWnd@@@Z
extern "C" int MS_ABI impl__OnShowPopupMenu_CFrameImpl__IEAAHPEAVCMFCPopupMenu__PEAVCFrameWnd___Z(
    void* pThis, void* pPopupMenu, CFrameWnd* pWnd) {
    (void)pThis;
    (void)pPopupMenu;
    (void)pWnd;
    return FALSE;
}
// Retail 0x64c60 dispatches to one of four virtual slots on m_pFrame (+0x118),
// picked by IsKindOf against four frame runtime classes; the slot numbers are only
// meaningful against retail's vtables, so this stays a stub.
// Symbol: ?OnShowCustomizePane@CFrameImpl@@IEAAHPEAVCMFCPopupMenu@@I@Z
extern "C" int MS_ABI impl__OnShowCustomizePane_CFrameImpl__IEAAHPEAVCMFCPopupMenu__I_Z(
    void* pThis, void* pMenuPopup, UINT uiToolbarID) {
    (void)pThis;
    (void)pMenuPopup;
    (void)uiToolbarID;
    return FALSE;
}
// Symbol: ?OnCloseFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCloseFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates.erase(frameImpl);
}
// Symbol: ?OnLoadFrame@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnLoadFrame_CFrameImpl__IEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    FrameImplState& state = g_frameImplStates[frameImpl];
    CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
    state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
}
// Symbol: ?OnChangeVisualManager@CFrameImpl@@QEAAXXZ
extern "C" void MS_ABI impl__OnChangeVisualManager_CFrameImpl__QEAAXXZ(void* pThis) {
    if (!pThis) {
        return;
    }

    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    CFrameWnd* ownerFrame = nullptr;
    CWnd* menuBarWnd = nullptr;

    {
        std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
        FrameImplState& state = g_frameImplStates[frameImpl];
        ownerFrame = state.ownerFrame;
        menuBarWnd = reinterpret_cast<CWnd*>(state.menuBar);
        CMFCVisualManager* manager = CMFCVisualManager::GetInstance();
        state.visualManagerClass = manager != nullptr ? manager->GetRuntimeClass() : nullptr;
    }

    if (ownerFrame != nullptr && ownerFrame->GetSafeHwnd() != nullptr) {
        ownerFrame->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
    if (menuBarWnd != nullptr && menuBarWnd->GetSafeHwnd() != nullptr) {
        menuBarWnd->RedrawWindow(nullptr, nullptr, RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN);
    }
}
// Symbol: ?SetMenuBar@CFrameImpl@@IEAAXPEAVCMFCMenuBar@@@Z
extern "C" void MS_ABI impl__SetMenuBar_CFrameImpl__IEAAXPEAVCMFCMenuBar___Z(void* pThis, void* pMenuBar) {
    if (!pThis) {
        return;
    }
    auto* frameImpl = reinterpret_cast<CFrameImpl*>(pThis);
    std::lock_guard<std::mutex> lock(g_featurePackStateMutex);
    g_frameImplStates[frameImpl].menuBar = pMenuBar;
}

// Retail's private static frame list: a CList<CFrameWnd*,CFrameWnd*> at RVA 0x3aacc0,
// 56 bytes -- AddFrame/RemoveFrame reach its head at 0x3aacc8, tail at 0x3aacd0,
// count at 0x3aacd8 and free-node head at 0x3aace0, i.e. MFC's usual
// vfptr/head/tail/count/free/blocks/blocksize shape.  OpenMFC exports storage of the
// same size, so anything importing this data export sees an object of the right
// footprint; the live list is the file-local g_pFrameListHead above, exactly
// as OpenMFC's own list classes keep their contents outside the object.
// Symbol: ?m_lstFrames@CFrameImpl@@1V?$CList@PEAVCFrameWnd@@PEAV1@@@A
extern "C" alignas(8) unsigned char impl__m_lstFrames_CFrameImpl__1V__CList_PEAVCFrameWnd__PEAV1___A[56] = {};

// Retail 0x64d40 returns immediately unless ?m_pWndToolBar@CMFCCustomizeMenuButton@@
// (0x3b6f28) is set, then allocates a 0xdb8-byte object and fills the popup from it;
// none of that customization state is modeled here.
// Symbol: ?AddDefaultButtonsToCustomizePane@CFrameImpl@@IEAAXPEAVCMFCPopupMenu@@I@Z
extern "C" void MS_ABI impl__AddDefaultButtonsToCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu__I_Z(void* /*class*/* p0, unsigned int p1) {}

// Transcribed from retail 0x65f40: walk m_lstFrames comparing GetSafeHwnd() of each
// entry against GetSafeHwnd() of the argument (both sides are null-tolerant there --
// retail loads m_hWnd at +0x40 only after testing the pointer), and AddTail only when
// no entry matches.
// Symbol: ?AddFrame@CFrameImpl@@SAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__AddFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(CFrameWnd* pFrame) {
    const HWND hWndNew = impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(pFrame);
    std::lock_guard<std::mutex> lock(g_frameListMutex);
    for (PtrNode* pNode = g_pFrameListHead; pNode != nullptr; pNode = pNode->pNext) {
        if (impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(
                static_cast<const CFrameWnd*>(pNode->pData)) == hWndNew) {
            return;
        }
    }
    PtrListAddTail(g_pFrameListHead, pFrame);
}

// Transcribed from retail 0x62a40, which is a two-instruction tail jump:
//   add $0x98,%rcx ; jmp ?AddTail@CPtrList@@QEAAPEAU__POSITION@@PEAX@Z
// i.e. an unconditional AddTail of the pane onto the tear-off toolbar list, with no
// null check and no duplicate check.
// DEVIATION: the list lives in this file's companion state keyed by `this`, so the
// body adds a `pThis == NULL -> no-op` guard that retail (which would just index off
// a null pointer) does not have.  A NULL pPane is still appended, as retail does.
// Symbol: ?AddTearOffToolbar@CFrameImpl@@IEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__AddTearOffToolbar_CFrameImpl__IEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    PtrListAddTail(g_frameImplExtras[pThis].pTearOffToolbars, pPane);
}

// Retail 0x62820, in order: AfxThrowInvalidArgException (0x225b80) when lpszName is
// NULL; then GetFreeCtrlBarID (0x62790) over the id range [+0x08,+0x0c]
// and the user-toolbar list at +0x60 (on 0 it CStringT::Format's string resource 0x3f73
// with (+0x0c - +0x08 + 1), shows it with AfxMessageBox(.., MB_ICONINFORMATION) and
// returns NULL); CRuntimeClass::CreateObject (0x233380) on the runtime class at +0x178;
// the new object's vtable +0x650 called with (m_pFrame, 0x50402808, the free id), i.e.
// Create(pParentWnd, dwStyle, nID); CWnd::SetWindowText with the name argument;
// vtable +0x390 read, OR'd with 0x34 and written back through vtable +0x3d8
// (GetBarStyle/SetBarStyle); vtable +0x3e8 with 0xf000; then ::GetWindowRect on the new
// bar and vtable +0x408 with a rect whose top-left is
// (GetSystemMetrics(SM_CXFULLSCREEN)/2, GetSystemMetrics(SM_CYFULLSCREEN)/2) and whose
// size is unchanged -- NOT the cursor position; the new bar's +0x1dc set to 0x7fff;
// m_pFrame (+0x118)
// vtable +0x300 with 1; and finally CPtrList::AddTail onto the +0x60 list.
// NOTE: this function never reads m_pDockManager (+0x120) -- nothing is docked here.
// None of +0x08/+0x0c/+0x60/+0x118/+0x178 exist here, so this stays a stub.
// Symbol: ?CreateNewToolBar@CFrameImpl@@IEAAPEBVCMFCToolBar@@PEB_W@Z
extern "C" void* MS_ABI impl__CreateNewToolBar_CFrameImpl__IEAAPEBVCMFCToolBar__PEB_W_Z(const wchar_t* p0) {
    return nullptr;
}

// Retail 0x646e0 needs the menu bar's virtual slot 0x6d0 and the ribbon bar at +0x180
// (?DeactivateKeyboardFocus@CMFCRibbonBar@@, 0xe28e0); the slot index is only
// meaningful against retail's vtable, so this stays a stub.
// Symbol: ?DeactivateMenu@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__DeactivateMenu_CFrameImpl__IEAAXXZ() {}

// Retail 0x63350 searches the user-toolbar list at +0x60; OpenMFC has no such list
// (nothing populates one -- CreateNewToolBar/LoadUserToolbars are stubs too).
// Symbol: ?DeleteToolBar@CFrameImpl@@IEAAHPEAVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__DeleteToolBar_CFrameImpl__IEAAHPEAVCMFCToolBar___Z(void* /*class*/* p0) {
    return 0;
}

// Retail 0x65ba0 computes the rect from m_pFrame (+0x118) via
// ?GetSystemBorders@CGlobalUtils@@ (0x6de90), ::IsIconic, GetWindowRect and a batch of
// GetSystemMetrics calls.  m_pFrame is not reachable from this opaque `this`.
// Symbol: ?GetCaptionRect@CFrameImpl@@IEAA?AVCRect@@XZ
extern "C" void* MS_ABI impl__GetCaptionRect_CFrameImpl__IEAA_AVCRect__XZ(void* /*class*/* p0) {
    return nullptr;
}

// Transcribed from retail 0x62790, instruction for instruction:
//     if (uiStart == (UINT)-1 || uiEnd == (UINT)-1)                    return 0;
//     if ((INT_PTR)(int)(uiEnd - uiStart + 1) == lst.GetCount())       return 0;
//     if (uiStart > uiEnd)                                             return 0;   // unsigned
//     for (UINT id = uiStart; id <= uiEnd; ++id) {                                 // unsigned
//         BOOL bUsed = FALSE;
//         for (POSITION pos = lst.GetHeadPosition(); pos != NULL; )
//             if ((UINT)((CWnd*)lst.GetNext(pos))->GetDlgCtrlID() == id) { bUsed = TRUE; break; }
//         if (!bUsed) return id;
//     }
//     return 0;
// Retail inlines the whole walk (m_pNodeHead at +0x08, m_nCount at +0x18, node data at
// +0x10, next at +0x00) and the per-element call is ?GetDlgCtrlID@CWnd@@QEBAHXZ
// (RVA 0x2a78b0), reached here through impl__GetDlgCtrlID_CWnd__QEBAHXZ.
// DEVIATION: OpenMFC's CObList keeps its elements in the filecore side table rather
// than in the object, so the walk goes through FindListStorage<CObList,...> instead of
// the inline node chain; a list with no state yet reads as empty (GetCount() == 0).
// It is read without taking openmfc::detail::filecore::g_collectionStateMutex -- that
// symbol is defined in detail/FilecoreSupport.cpp, but naming it from this file adds a
// new C++-mangled undefined that the per-file link audit rejects.  Retail is equally
// unsynchronised here, and every caller is on the UI thread.
// Symbol: ?GetFreeCtrlBarID@CFrameImpl@@KAIIIAEBVCObList@@@Z
extern "C" unsigned int MS_ABI impl__GetFreeCtrlBarID_CFrameImpl__KAIIIAEBVCObList___Z(
    unsigned int uiStart, unsigned int uiEnd, const CObList& lst) {
    if (uiStart == static_cast<unsigned int>(-1) || uiEnd == static_cast<unsigned int>(-1)) {
        return 0;
    }

    typedef CList<CObject*, CObject*> ObListData;
    const ObListData* pData =
        openmfc::detail::filecore::FindListStorage<CObList, CObject*, CObject*>(&lst);
    const INT_PTR nCount = pData != nullptr ? pData->GetCount() : 0;

    if (static_cast<INT_PTR>(static_cast<int>(uiEnd - uiStart + 1)) == nCount) {
        return 0;
    }
    if (uiStart > uiEnd) {
        return 0;
    }

    for (unsigned int uiID = uiStart; uiID <= uiEnd; ++uiID) {
        bool bUsed = false;
        if (pData != nullptr) {
            ObListData::POSITION pos = pData->GetHeadPosition();
            while (pos != ObListData::POSITION(nullptr)) {
                CObject* pObject = pData->GetNext(pos);
                const unsigned int uiBarID = static_cast<unsigned int>(
                    impl__GetDlgCtrlID_CWnd__QEBAHXZ(static_cast<const CWnd*>(pObject)));
                if (uiBarID == uiID) {
                    bUsed = true;
                    break;
                }
            }
        }
        if (!bUsed) {
            return uiID;
        }
    }
    return 0;
}

// Retail 0x66480 walks the caption-button list at +0xd0 comparing each button's id at
// +0x20; OpenMFC keeps no caption-button list (UpdateCaption/OnNcCalcSize, which fill
// it in retail, are stubs here).
// Symbol: ?GetSysButton@CFrameImpl@@IEAAPEAVCMFCCaptionButtonEx@@I@Z
extern "C" void* MS_ABI impl__GetSysButton_CFrameImpl__IEAAPEAVCMFCCaptionButtonEx__I_Z(unsigned int p0) {
    return nullptr;
}

// Retail 0x62640 indexes the user-toolbar list at +0x60 (count at +0x78); OpenMFC has
// no such list and nothing that would fill one.
// Symbol: ?GetUserToolBarByIndex@CFrameImpl@@IEBAPEAVCMFCToolBar@@H@Z
extern "C" void* MS_ABI impl__GetUserToolBarByIndex_CFrameImpl__IEBAPEAVCMFCToolBar__H_Z(int p0) {
    return nullptr;
}

// Transcribed from retail 0x626a0 (ANSI twin ?InitUserToolbars@CFrameImpl@@IEAAXPEBDII@Z):
//   if (uiFirst == (UINT)-1 || uiLast == (UINT)-1) return;   // nothing is stored
//   m_uiUserToolbarFirst = uiFirst;  m_uiUserToolbarLast = uiLast;
//   pApp = AfxGetModuleState()->m_pCurrentWinApp;
//   if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx))) pApp = NULL;   // null-safe in retail
//   if (lpszRegEntry)   profile.SetString(lpszRegEntry, strlen(lpszRegEntry));
//   else if (pApp)      profile = pApp->GetRegSectionPath(_T(""));
//   else                profile.SetString(_T(""), 0);
// (the runtime-class descriptor at 0x18031e618 reads "CWinAppEx" in the image.)
// DEVIATION: the id pair and the profile name are kept in this file's companion state
// rather than in the object; the app lookup goes through impl__AfxGetApp (OpenMFC's
// AfxGetApp, phase4/src/detail/RegcoreSupport.cpp:59) and the CWinAppEx thunks; and a
// NULL `pThis` is a no-op here, where retail would store through a null pointer.
// Symbol: ?InitUserToolbars@CFrameImpl@@IEAAXPEB_WII@Z
extern "C" void MS_ABI impl__InitUserToolbars_CFrameImpl__IEAAXPEB_WII_Z(
    void* pThis, const wchar_t* lpszRegEntry, unsigned int uiUserToolbarFirst, unsigned int uiUserToolbarLast) {
    if (!pThis) {
        return;
    }
    if (uiUserToolbarFirst == static_cast<unsigned int>(-1) ||
        uiUserToolbarLast == static_cast<unsigned int>(-1)) {
        return;
    }

    CWinAppEx* pAppEx = nullptr;
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp != nullptr &&
        impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        pAppEx = static_cast<CWinAppEx*>(pApp);
    }

    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    FrameImplExtra& extra = g_frameImplExtras[pThis];
    extra.uiUserToolbarFirst = uiUserToolbarFirst;
    extra.uiUserToolbarLast = uiUserToolbarLast;
    if (lpszRegEntry != nullptr) {
        extra.strUserToolbarsProfile = lpszRegEntry;
    } else if (pAppEx != nullptr) {
        alignas(CString) unsigned char storage[sizeof(CString)];
        CString* pPath = reinterpret_cast<CString*>(storage);
        impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
            pPath, pAppEx, L"");
        extra.strUserToolbarsProfile = *pPath;
        pPath->~CString();
    } else {
        extra.strUserToolbarsProfile = L"";
    }
}

// Retail 0x64fc0 returns FALSE unless ?GetParentPopupMenu@CMFCPopupMenu@@ (0xb7b10)
// yields a parent, then loads string resource 0x427a and compares it with text reached
// through that parent's +0x228; neither the resource load nor CMFCPopupMenu's parent
// chain is modeled here.
// Symbol: ?IsCustomizePane@CFrameImpl@@IEBAHPEBVCMFCPopupMenu@@@Z
extern "C" int MS_ABI impl__IsCustomizePane_CFrameImpl__IEBAHPEBVCMFCPopupMenu___Z(const void* /*class*/* p0) {
    return 0;
}

// No RVA: this export is one of the ~11% with no entry in mfc140_rva_symbols.json,
// so there is nothing to transcribe and no CDockState modeling here to fall back on.
// Symbol: ?IsDockStateValid@CFrameImpl@@IEAAHAEBVCDockState@@@Z
extern "C" int MS_ABI impl__IsDockStateValid_CFrameImpl__IEAAHAEBVCDockState___Z(const void* /*class*/* p0) {
    return 0;
}

// Transcribed from retail 0x64680: F1 with no modifier held and not an auto-repeat.
// The three ::GetKeyState calls are the import slot at 0x2c4d20 (USER32!GetKeyState),
// queried in the order VK_SHIFT, VK_CONTROL, VK_MENU; the auto-repeat test is
// `test $0x4000, 0x1a(%rcx)`, i.e. KF_REPEAT in HIWORD(lParam).
// DEVIATION: retail dereferences lpMsg unconditionally; the null guard is ours.
// Symbol: ?IsHelpKey@CFrameImpl@@KAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__IsHelpKey_CFrameImpl__KAHPEAUtagMSG___Z(MSG* lpMsg) {
    if (lpMsg == nullptr) {
        return FALSE;
    }
    if (lpMsg->message != WM_KEYDOWN) {
        return FALSE;
    }
    if (lpMsg->wParam != VK_F1) {
        return FALSE;
    }
    if ((HIWORD(lpMsg->lParam) & KF_REPEAT) != 0) {
        return FALSE;
    }
    if (::GetKeyState(VK_SHIFT) < 0) {
        return FALSE;
    }
    if (::GetKeyState(VK_CONTROL) < 0) {
        return FALSE;
    }
    if (::GetKeyState(VK_MENU) < 0) {
        return FALSE;
    }
    return TRUE;
}

// Retail 0x66ab0 is `m_pDockManager (+0x120) != NULL && m_pDockManager->[+0x208] != 0`
// -- a BOOL that sits directly in front of the print-preview pane list at +0x210 (the
// list ?HideForPrintPreview@CDockingManager@@ 0x4ce00 fills).  OpenMFC's CFrameImpl
// has no dock-manager pointer and its CDockingManager state has no such flag.
// Symbol: ?IsPrintPreview@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__IsPrintPreview_CFrameImpl__IEAAHXZ() {
    return 0;
}

// Transcribed from retail 0x62670:
//     UINT uiID = (UINT)pToolBar->GetDlgCtrlID();          // ?GetDlgCtrlID@CWnd@@ 0x2a78b0
//     return uiID >= m_uiUserToolbarFirst && uiID <= m_uiUserToolbarLast;   // unsigned
// The two bounds are the pair InitUserToolbars stores; before that call the
// constructor leaves both at (UINT)-1, which makes the range empty for every id but
// (UINT)-1 itself -- retail behaves the same way.
// DEVIATION: retail calls GetDlgCtrlID on the argument without a null check, and it
// reads the bounds straight out of the object; a `pThis` this file has never seen
// (i.e. InitUserToolbars was not called) falls back to the constructor's -1/-1 pair.
// Both NULL `pThis` and NULL `pToolBar` return FALSE here.
// Symbol: ?IsUserDefinedToolbar@CFrameImpl@@IEBAHPEBVCMFCToolBar@@@Z
extern "C" int MS_ABI impl__IsUserDefinedToolbar_CFrameImpl__IEBAHPEBVCMFCToolBar___Z(
    const void* pThis, const void* pToolBar) {
    if (!pThis || !pToolBar) {
        return FALSE;
    }
    const unsigned int uiID =
        static_cast<unsigned int>(impl__GetDlgCtrlID_CWnd__QEBAHXZ(
            static_cast<const CWnd*>(reinterpret_cast<const CMFCToolBar*>(pToolBar))));

    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    auto it = g_frameImplExtras.find(pThis);
    const unsigned int uiFirst =
        it != g_frameImplExtras.end() ? it->second.uiUserToolbarFirst : static_cast<unsigned int>(-1);
    const unsigned int uiLast =
        it != g_frameImplExtras.end() ? it->second.uiUserToolbarLast : static_cast<unsigned int>(-1);
    return (uiID >= uiFirst && uiID <= uiLast) ? TRUE : FALSE;
}

// Retail 0x64610 forwards to virtual slot 0xa0 of m_pDockManager (+0x120), gated on
// the INT_PTR at +0x38; neither member exists here.
// Symbol: ?LoadDockState@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__LoadDockState_CFrameImpl__IEAAXPEB_W_Z(const wchar_t* p0) {}

// Transcribed from retail 0x64750 (which ignores `this` entirely):
//     pApp = AfxGetModuleState()->m_pCurrentWinApp;
//     if (!pApp || !pApp->IsKindOf(RUNTIME_CLASS(CWinAppEx))) return FALSE;
//     return CMFCToolBar::LoadLargeIconsState(pApp->GetRegSectionPath(_T("")));
// The callee is ?LoadLargeIconsState@CMFCToolBar@@SAHPEBD@Z (RVA 0x156b80); OpenMFC's
// own CMFCToolBar::LoadLargeIconsState is itself an un-transcribed stub returning 0,
// so this currently returns FALSE for every caller -- the dispatch, not the result,
// is what is implemented here.
// Symbol: ?LoadLargeIconsState@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__LoadLargeIconsState_CFrameImpl__IEAAHXZ(void* pThis) {
    (void)pThis;
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr) {
        return FALSE;
    }
    if (!impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return FALSE;
    }

    alignas(CString) unsigned char storage[sizeof(CString)];
    CString* pPath = reinterpret_cast<CString*>(storage);
    impl__GetRegSectionPath_CWinAppEx__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
        pPath, static_cast<CWinAppEx*>(pApp), L"");
    const int bResult = impl__LoadLargeIconsState_CMFCToolBar__SAHPEB_W_Z(pPath->GetString());
    pPath->~CString();
    return bResult;
}

// Retail 0x62a90 walks the tear-off list at +0x98 (head +0xa0) and calls virtuals on
// each pane; the body runs past 400 instructions and was not transcribed, so having the
// list alone is not enough to write this one.
// Symbol: ?LoadTearOffMenus@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__LoadTearOffMenus_CFrameImpl__IEAAXXZ() {}

// Retail 0x62490 instantiates one toolbar per id in [+0x08,+0x0c] from the runtime
// class at +0x178, calls CMFCToolBar::LoadState with the profile name at +0x108 and
// docks each result through m_pDockManager (+0x120).  Only the id range exists here.
// Symbol: ?LoadUserToolbars@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__LoadUserToolbars_CFrameImpl__IEAAXXZ() {}

// Retail 0x65890 gates on +0x30, calls a visual-manager virtual (the instance comes
// from the CMFCVisualManager::GetInstance helper at 0x97f4) passing m_pFrame (+0x118),
// and on deactivation drives the ribbon bar at +0x180 (?HideKeyTips@CMFCRibbonBar@@
// 0xe4050, ?OnCancelMode@CMFCRibbonBar@@ 0xdcf20) plus AFX_GLOBAL_DATA at 0x3ba380.
// Symbol: ?OnActivateApp@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__OnActivateApp_CFrameImpl__IEAAXH_Z(int p0) {}

// Retail 0x66ad0 re-initializes AFX_GLOBAL_DATA (0x3ba380), pokes the ribbon bar at
// +0x180 and tail-jumps into OnChangeVisualManager; the ribbon pointer is not modeled.
// Symbol: ?OnCompositionChanged@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__OnCompositionChanged_CFrameImpl__IEAAXXZ() {}

// Retail 0x65300 works off the ribbon bar at +0x180, read at 0x180065335 right after
// the null check on the MINMAXINFO argument; the ribbon pointer is not modeled.
// Symbol: ?OnGetMinMaxInfo@CFrameImpl@@IEAAXPEAUtagMINMAXINFO@@@Z
extern "C" void MS_ABI impl__OnGetMinMaxInfo_CFrameImpl__IEAAXPEAUtagMINMAXINFO___Z(void* /*struct*/* p0) {}

// Retail 0x662c0 latches the hot caption button (+0x14) as pressed (+0x18), sets the
// button's own state word and repaints it; the caption-button list at +0xd0 that it
// searches does not exist here.
// Symbol: ?OnLButtonDown@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDown_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Retail 0x66310 turns the pressed caption button (+0x18) into an SC_CLOSE / SC_RESTORE /
// SC_MAXIMIZE / SC_MINIMIZE ::PostMessage(WM_SYSCOMMAND) on m_pFrame (+0x118); neither
// member exists here.
// Symbol: ?OnLButtonUp@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Retail 0x64490 offers the character to the ribbon bar at +0x180 (gated on its
// WS_VISIBLE style) before falling through; the ribbon pointer is not modeled.
// Symbol: ?OnMenuChar@CFrameImpl@@IEAAHI@Z
extern "C" int MS_ABI impl__OnMenuChar_CFrameImpl__IEAAHI_Z(unsigned int p0) {
    return 0;
}

// Retail 0x66410 asks the visual manager (instance helper 0x97f4) whether it owner-
// draws the caption, gates on +0x30, maps the point with ::ClientToScreen on m_pFrame
// (+0x118) and calls OnTrackCaptionButtons, which needs the +0xd0 button list.
// Symbol: ?OnMouseMove@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Retail 0x659d0 repaints the custom caption through m_pFrame (+0x118), the ribbon bar
// (+0x180) and the +0x30 gate; none of those members exist here.
// Symbol: ?OnNcActivate@CFrameImpl@@IEAAHH@Z
extern "C" int MS_ABI impl__OnNcActivate_CFrameImpl__IEAAHH_Z(int p0) {
    return 0;
}

// Retail 0x655f0 reserves the caption strip using ?GetSystemBorders@CGlobalUtils@@
// (0x6de90) on m_pFrame (+0x118) and the ribbon bar at +0x180.
// Symbol: ?OnNcCalcSize@CFrameImpl@@IEAAHHPEAUtagNCCALCSIZE_PARAMS@@@Z
extern "C" int MS_ABI impl__OnNcCalcSize_CFrameImpl__IEAAHHPEAUtagNCCALCSIZE_PARAMS___Z(int p0, void* /*struct*/* p1) {
    return 0;
}

// Retail 0x66040 first offers WM_NCHITTEST to the ribbon bar (+0x180, gated on its
// +0x468 and +0x448 flags), then hit-tests the caption-button list (+0xd0) and the
// caption rect; none of that state exists here.
// Symbol: ?OnNcHitTest@CFrameImpl@@IEAAIVCPoint@@@Z
extern "C" unsigned int MS_ABI impl__OnNcHitTest_CFrameImpl__IEAAIVCPoint___Z(void* /*class*/ p0) {
    return 0;
}

// Retail 0x66270 gates on +0x30 and +0x20 and then calls OnTrackCaptionButtons, which
// needs the caption-button list at +0xd0.
// Symbol: ?OnNcMouseMove@CFrameImpl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnNcMouseMove_CFrameImpl__IEAAXIVCPoint___Z(unsigned int p0, void* /*class*/ p1) {}

// Retail 0x651d0 decides whether to draw the custom caption from the ribbon bar
// (+0x180, its +0x468 flag), ?IsDwmCompositionEnabled@AFX_GLOBAL_DATA@@ (0x6c260) and
// the cached caption rect at +0x50.
// Symbol: ?OnNcPaint@CFrameImpl@@IEAAHXZ
extern "C" int MS_ABI impl__OnNcPaint_CFrameImpl__IEAAHXZ() {
    return 0;
}

// Retail 0x65980 repaints the ribbon bar at +0x180 (::RedrawWindow with
// RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW) when it is visible and its +0x468 flag is
// set, so the new caption text reaches the ribbon; the ribbon pointer is not modeled.
// Symbol: ?OnSetText@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__OnSetText_CFrameImpl__IEAAXPEB_W_Z(const wchar_t* p0) {}

// Retail 0x664d0 moves the hot-button id (+0x14) between caption buttons found in the
// list at +0xd0 and captures/releases the mouse accordingly.
// Symbol: ?OnTrackCaptionButtons@CFrameImpl@@IEAAXVCPoint@@@Z
extern "C" void MS_ABI impl__OnTrackCaptionButtons_CFrameImpl__IEAAXVCPoint___Z(void* /*class*/ p0) {}

// Retail 0x650e0 caches, at +0x24, the result of a visual-manager virtual (instance
// helper 0x97f4) called with m_pFrame (+0x118) and the WINDOWPOS cx/cy, under the +0x40
// re-entrancy flag; neither member exists here.
// Symbol: ?OnWindowPosChanging@CFrameImpl@@IEAAXPEAUtagWINDOWPOS@@@Z
extern "C" void MS_ABI impl__OnWindowPosChanging_CFrameImpl__IEAAXPEAUtagWINDOWPOS___Z(void* /*struct*/* p0) {}

// Retail 0x63540 routes the key through ?GetSafeActivePopupMenu@CMFCPopupMenu@@
// (0xbc470), the docking-pane list at 0x3ab098 and the ribbon bar at +0x180; the
// popup/pane infrastructure it walks is not modeled.
// Symbol: ?ProcessKeyboard@CFrameImpl@@IEAAHHPEAH@Z
extern "C" int MS_ABI impl__ProcessKeyboard_CFrameImpl__IEAAHHPEAH_Z(int p0, int* p1) {
    return 0;
}

// Retail 0x63840 hands the point to the ribbon bar at +0x180
// (?DeactivateKeyboardFocus@CMFCRibbonBar@@ 0xe28e0) and then walks the active popup
// menu chain; neither is modeled.
// Symbol: ?ProcessMouseClick@CFrameImpl@@IEAAHIUtagPOINT@@PEAUHWND__@@@Z
extern "C" int MS_ABI impl__ProcessMouseClick_CFrameImpl__IEAAHIUtagPOINT__PEAUHWND_____Z(unsigned int p0, void* /*struct*/ p1, void* /*struct*/* p2) {
    return 0;
}

// Retail 0x63f40 dereferences no CFrameImpl member at all (`this` is never used): it
// works from ?m_bCustomizeMode@CMFCToolBar@@ (0x3b70bc), ?m_pActivePopupMenu@
// CMFCPopupMenu@@ (0x3b6fe8), ?FindMenuWithConnectedFloaty@CMFCPopupMenu@@ (0xbbf50)
// and ?GetSafeActivePopupMenu@CMFCPopupMenu@@ (0xbc470), ::GetWindowRect/::PtInRect on
// the menu at +0x19c0 and on the popup's own m_hWnd, and finally the popup's vtable
// +0x3a0 feeding ?FindDestintationToolBar@CMFCPopupMenuBar@@ (0xbf1b0).  It becomes
// transcribable as soon as that popup-menu infrastructure exists; today none of it is
// modeled.
// Symbol: ?ProcessMouseMove@CFrameImpl@@IEAAHUtagPOINT@@@Z
extern "C" int MS_ABI impl__ProcessMouseMove_CFrameImpl__IEAAHUtagPOINT___Z(void* /*struct*/ p0) {
    return 0;
}

// Retail 0x64040 sends WM_MOUSEWHEEL to the active CMFCPopupMenu when there is one and
// otherwise to the ribbon bar at +0x180; neither is modeled.
// Symbol: ?ProcessMouseWheel@CFrameImpl@@IEAAH_K_J@Z
extern "C" int MS_ABI impl__ProcessMouseWheel_CFrameImpl__IEAAH_K_J_Z(unsigned __int64 p0, __int64 p1) {
    return 0;
}

// Retail 0x66660 returns at once on a NULL button; otherwise it takes the button's rect
// from vtable slot 5, copies those 16 bytes to +0x50, ::SendMessage(m_pFrame->m_hWnd,
// WM_NCPAINT, 0, 0), ::SetRectEmpty(&this->+0x50) and ::UpdateWindow(m_pFrame->m_hWnd).
// Neither +0x50 nor m_pFrame (+0x118) is reachable from this opaque `this`.
// Symbol: ?RedrawCaptionButton@CFrameImpl@@IEAAXPEAVCMFCCaptionButtonEx@@@Z
extern "C" void MS_ABI impl__RedrawCaptionButton_CFrameImpl__IEAAXPEAVCMFCCaptionButtonEx___Z(void* /*class*/* p0) {}

// Transcribed from retail 0x65f90: find the first entry whose GetSafeHwnd() matches the
// argument's and unlink it; a miss leaves the list untouched.  Retail additionally calls
// ?RemoveAll@CPtrList@@ (0x83d0) once the count reaches zero, which only releases the
// list's node blocks -- the node-per-entry storage here frees as it unlinks instead.
// Symbol: ?RemoveFrame@CFrameImpl@@SAXPEAVCFrameWnd@@@Z
extern "C" void MS_ABI impl__RemoveFrame_CFrameImpl__SAXPEAVCFrameWnd___Z(CFrameWnd* pFrame) {
    const HWND hWndGone = impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(pFrame);
    std::lock_guard<std::mutex> lock(g_frameListMutex);
    PtrNode* pPrev = nullptr;
    for (PtrNode* pNode = g_pFrameListHead; pNode != nullptr; pPrev = pNode, pNode = pNode->pNext) {
        if (impl__GetSafeHwnd_CWnd__QEBAPEAUHWND____XZ(
                static_cast<const CFrameWnd*>(pNode->pData)) != hWndGone) {
            continue;
        }
        if (pPrev == nullptr) {
            g_pFrameListHead = pNode->pNext;
        } else {
            pPrev->pNext = pNode->pNext;
        }
        delete pNode;
        return;
    }
}

// Transcribed from retail 0x62a50: walk the tear-off toolbar list at +0x98 for a node
// whose element is the argument and, if one is found, RemoveAt it (first match only;
// a miss leaves the list untouched).
// DEVIATION: as for AddTearOffToolbar, the body adds a `pThis == NULL -> no-op` guard
// because the list is reached through this file's companion table rather than +0x98.
// Symbol: ?RemoveTearOffToolbar@CFrameImpl@@IEAAXPEAVCBasePane@@@Z
extern "C" void MS_ABI impl__RemoveTearOffToolbar_CFrameImpl__IEAAXPEAVCBasePane___Z(void* pThis, void* pPane) {
    if (!pThis) {
        return;
    }
    std::lock_guard<std::mutex> lock(g_frameImplExtraMutex);
    auto it = g_frameImplExtras.find(pThis);
    if (it == g_frameImplExtras.end()) {
        return;
    }
    PtrNode* pPrev = nullptr;
    for (PtrNode* pNode = it->second.pTearOffToolbars; pNode != nullptr;
         pPrev = pNode, pNode = pNode->pNext) {
        if (pNode->pData != pPane) {
            continue;
        }
        if (pPrev == nullptr) {
            it->second.pTearOffToolbars = pNode->pNext;
        } else {
            pPrev->pNext = pNode->pNext;
        }
        delete pNode;
        return;
    }
}

// Symbol: ?RestorePosition@CFrameImpl@@IEAAXAEAUtagCREATESTRUCTW@@@Z
extern "C" void MS_ABI impl__RestorePosition_CFrameImpl__IEAAXAEAUtagCREATESTRUCTW___Z(void* /*struct*/* p0) {}

// Symbol: ?SaveDockState@CFrameImpl@@IEAAXPEB_W@Z
extern "C" void MS_ABI impl__SaveDockState_CFrameImpl__IEAAXPEB_W_Z(const wchar_t* p0) {}

// Symbol: ?SaveTearOffMenus@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveTearOffMenus_CFrameImpl__IEAAXH_Z(int p0) {}

// Symbol: ?SaveUserToolbars@CFrameImpl@@IEAAXH@Z
extern "C" void MS_ABI impl__SaveUserToolbars_CFrameImpl__IEAAXH_Z(int p0) {}

// Symbol: ?SetDockState@CFrameImpl@@IEAAXAEBVCDockState@@@Z
extern "C" void MS_ABI impl__SetDockState_CFrameImpl__IEAAXAEBVCDockState___Z(const void* /*class*/* p0) {}

// Symbol: ?SetHighlightedSysButton@CFrameImpl@@IEAAXI@Z
extern "C" void MS_ABI impl__SetHighlightedSysButton_CFrameImpl__IEAAXI_Z(unsigned int p0) {}

// Symbol: ?SetNewUserToolBarRTC@CFrameImpl@@QEAAXPEAUCRuntimeClass@@@Z
extern "C" void MS_ABI impl__SetNewUserToolBarRTC_CFrameImpl__QEAAXPEAUCRuntimeClass___Z(void* /*struct*/* p0) {}

// Symbol: ?SetupToolbarMenu@CFrameImpl@@IEAAXAEAVCMenu@@II@Z
extern "C" void MS_ABI impl__SetupToolbarMenu_CFrameImpl__IEAAXAEAVCMenu__II_Z(void* /*class*/* p0, unsigned int p1, unsigned int p2) {}

// Symbol: ?ShowQuickCustomizePane@CFrameImpl@@IEAAXPEAVCMFCPopupMenu@@@Z
extern "C" void MS_ABI impl__ShowQuickCustomizePane_CFrameImpl__IEAAXPEAVCMFCPopupMenu___Z(void* /*class*/* p0) {}

// Symbol: ?StopCaptionButtonsTracking@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__StopCaptionButtonsTracking_CFrameImpl__IEAAXXZ() {}

// Symbol: ?StoreWindowPlacement@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__StoreWindowPlacement_CFrameImpl__IEAAXXZ() {}

// Symbol: ?UpdateCaption@CFrameImpl@@IEAAXXZ
extern "C" void MS_ABI impl__UpdateCaption_CFrameImpl__IEAAXXZ() {}
