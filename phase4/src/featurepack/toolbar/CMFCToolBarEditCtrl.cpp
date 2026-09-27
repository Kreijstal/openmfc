// CMFCToolBarEditCtrl — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp
//
// Every body marked "Retail (RVA 0x..., mfc140u)" below was transcribed from the
// disassembly of that entry point in mfc140u.dll (14.51.36231).  The class is
// the edit child of a toolbar edit-box button (SDK afxtoolbareditboxbutton.h:112,
// `class CMFCToolBarEditCtrl : public CMFCEditBrowseCtrl`, members
// `CMFCToolBarEditBoxButton& m_buttonEdit; BOOL m_bTracked;`).  It is NOT
// declared in include/openmfc, so `this` is a void* and the layout is pinned
// here from the retail constructor ??0CMFCToolBarEditCtrl (RVA 0x168ba0,
// mfc140u), which calls ??0CMFCEditBrowseCtrl@@QEAA@XZ (0x5f270, mfc140u),
// installs the class vftable (0x180317b78, mfc140u), then stores:
//
//   CMFCEditBrowseCtrl base .......................... +0x000 .. +0x150
//   +0x150  CMFCToolBarEditBoxButton& m_buttonEdit (a pointer; the ctor argument)
//   +0x158  BOOL                      m_bTracked   (ctor stores 0)
//   sizeof == 0x160 (the `new` size of retail CreateEdit, as documented in
//   featurepack/toolbar/CMFCToolBarEditBoxButton.cpp)
//
// The base's 0x150 bytes are the layout featurepack/controls/CMFCEditBrowseCtrl.cpp
// pins (its S_CMFCEditBrowseCtrl, "sizeof == 0x150"); the owner button's
// m_uiMenuResID is at +0xac, as featurepack/toolbar/CMFCToolBarEditBoxButton.cpp
// documents from the retail button constructor.
//
// The handlers' mfc140u entries come from the class message map
// (?GetMessageMap@CMFCToolBarEditCtrl@@, 0x168c40, returns the AFX_MSGMAP at
// 0x180317aa0, mfc140u; base map ?GetMessageMap@CMFCEditBrowseCtrl@@):
//   WM_SETFOCUS    (0x007) -> 0x168d80  OnSetFocus
//   WM_KILLFOCUS   (0x008) -> 0x168db0  OnKillFocus
//   WM_MOUSEMOVE   (0x200) -> 0x168de0  OnMouseMove
//   WM_CONTEXTMENU (0x07b) -> 0x168e80  OnContextMenu
//   WM_MOUSELEAVE  (0x2a3) -> 0x168e40  OnMouseLeave
// and PreTranslateMessage is vftable slot 69 (+0x228) of 0x180317b78 -> 0x168c50.
// Every IAT slot named below was resolved with iatu.py against mfc140u.
//
// DEVIATIONS shared by every body in this file:
//  * `this` and m_buttonEdit are null-checked; retail dereferences both
//    unconditionally.  (PreTranslateMessage also returns FALSE for a NULL pMsg.)
//  * OpenMFC has no CMFCToolBarEditCtrl vftable: the object keeps the vptr the
//    exported CMFCEditBrowseCtrl constructor leaves (OpenMFC's CWnd one), so
//    PreTranslateMessage is not reached by virtual dispatch, only through its
//    export.  The in-tree message map for this class
//    (detail/Toolbar21MsgmapSupport.cpp, classCMFCToolBarEditCtrl_msgmap) has
//    no entries, so the five handlers below are likewise reached only through
//    their exports today.
//  * The retail CreateEdit builds this object; OpenMFC's CreateEdit
//    (CMFCToolBarEditBoxButton.cpp) builds a plain CWnd instead, so these bodies
//    run only for an object a client constructs through the exported ctor.

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// ---------------------------------------------------------------------------
// Thunks this file calls (definitions named on each line; parameter lists are
// derived from the mangled names).
// ---------------------------------------------------------------------------
// featurepack/controls/CMFCEditBrowseCtrl.cpp
extern "C" void* MS_ABI impl___0CMFCEditBrowseCtrl__QEAA_XZ(void* pThis);
extern "C" void MS_ABI impl___1CMFCEditBrowseCtrl__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__OnMouseMove_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point);
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCEditBrowseCtrl__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg);
// featurepack/toolbar/CMFCToolBarEditBoxButton.cpp
extern "C" void MS_ABI impl__SetHotEdit_CMFCToolBarEditBoxButton__IEAAXH_Z(CMFCToolBarEditBoxButton* pThis, int bHot);
// core/window/Thunks.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);
extern "C" CFrameWnd* MS_ABI impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const CWnd* pThis);
// core/window/CWnd.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);
// core/runtime/Globals.cpp
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);
// featurepack/menu/CContextMenuManager.cpp
extern "C" CMFCPopupMenu* MS_ABI impl__ShowPopupMenu_CContextMenuManager__UEAAPEAVCMFCPopupMenu__PEAUHMENU____HHPEAVCWnd__HHH_Z(
    CContextMenuManager* pThis, HMENU hmenuPopup, int x, int y, CWnd* pWndOwner,
    int bOwnMessage, int bAutoDestroy, int bRightAlign);
// detail/RegcoreSupport.cpp, core/runtime/CObject.cpp, core/app/CWinAppEx.cpp
extern "C" CWinApp* MS_ABI impl__AfxGetApp__YAPEAVCWinApp__XZ();
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(const CObject* pThis, const CRuntimeClass* pClass);
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ();

namespace {

// --- this class (file header) ----------------------------------------------
constexpr std::size_t kOffButtonEdit = 0x150;   // CMFCToolBarEditBoxButton& m_buttonEdit
constexpr std::size_t kOffTracked    = 0x158;   // BOOL m_bTracked
constexpr std::size_t kSizeOf        = 0x160;
static_assert(kOffTracked + sizeof(int) <= kSizeOf, "m_bTracked lies inside the 0x160-byte object");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "CWnd::m_hWnd @0x40 (every retail body reads the HWND there)");

// --- the owner button ------------------------------------------------------
constexpr std::size_t kBtnOffMenuResID = 0xac;   // UINT m_uiMenuResID
static_assert(sizeof(CMFCToolBarEditBoxButton) == 0xb0,
              "the header's CMFCToolBarEditBoxButton has the retail size, so +0xac is inside it");

inline unsigned char* Bytes(void* p) { return static_cast<unsigned char*>(p); }
inline CWnd* W(void* p) { return static_cast<CWnd*>(p); }
inline CMFCToolBarEditBoxButton* Button(void* pThis) {
    CMFCToolBarEditBoxButton* p = nullptr;
    std::memcpy(&p, Bytes(pThis) + kOffButtonEdit, sizeof p);
    return p;
}
inline int& Tracked(void* pThis) { return *reinterpret_cast<int*>(Bytes(pThis) + kOffTracked); }
inline UINT BtnMenuResID(CMFCToolBarEditBoxButton* b) {
    UINT v = 0;
    std::memcpy(&v, reinterpret_cast<unsigned char*>(b) + kBtnOffMenuResID, sizeof v);
    return v;
}
// m_buttonEdit.SetHotEdit(bHot); the OpenMFC thunk returns for a NULL button.
inline void SetHotEdit(void* pThis, int bHot) {
    impl__SetHotEdit_CMFCToolBarEditBoxButton__IEAAXH_Z(Button(pThis), bHot);
}
// Retail's inline CWnd::GetParent(): FromHandle(::GetParent(m_hWnd)).
inline CWnd* ParentOf(CWnd* pWnd) {
    return impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(pWnd->m_hWnd));
}

// Retail's afxContextMenuManager (mfc140u .data 0x3be1b0, not exported).  OpenMFC
// publishes no such global; as in featurepack/toolbar/CMFCToolBarComboBoxEdit.cpp
// the closest reachable equivalent is the manager the current CWinAppEx owns,
// read directly (GetContextMenuManager would create one on demand and turn
// retail's "none exists" gate into "always").
struct WinAppExAccess : CWinAppEx {
    using CWinAppEx::m_pContextMenuManager;
};
CContextMenuManager* CurrentContextMenuManager() {
    CWinApp* pApp = impl__AfxGetApp__YAPEAVCWinApp__XZ();
    if (pApp == nullptr ||
        !impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(pApp, impl__GetThisClass_CWinAppEx__SAPEAUCRuntimeClass__XZ())) {
        return nullptr;
    }
    return static_cast<WinAppExAccess*>(static_cast<CWinAppEx*>(pApp))->m_pContextMenuManager;
}

} // namespace

// Retail (RVA 0x168ba0, mfc140u), fully transcribed:
//     CMFCEditBrowseCtrl::CMFCEditBrowseCtrl();     // 0x5f270 (mfc140u)
//     vptr         = CMFCToolBarEditCtrl::`vftable' (0x180317b78, mfc140u);
//     m_buttonEdit = &edit;                         // +0x150
//     m_bTracked   = FALSE;                         // +0x158
//     return this;
// DEVIATION: no vftable store (file header); the object keeps the vptr the
// exported CMFCEditBrowseCtrl constructor leaves, which that file records for
// its own virtual-dispatch helpers.
// Symbol: ??0CMFCToolBarEditCtrl@@QEAA@AEAVCMFCToolBarEditBoxButton@@@Z
extern "C" void* MS_ABI impl___0CMFCToolBarEditCtrl__QEAA_AEAVCMFCToolBarEditBoxButton___Z(void* pThis, void* pButton) {
    if (pThis == nullptr) return pThis;
    impl___0CMFCEditBrowseCtrl__QEAA_XZ(pThis);
    std::memcpy(Bytes(pThis) + kOffButtonEdit, &pButton, sizeof pButton);
    Tracked(pThis) = FALSE;
    return pThis;
}

// Retail (RVA 0x168c30, mfc140u), fully transcribed:
//     vptr = CMFCToolBarEditCtrl::`vftable' (0x180317b78, mfc140u);
//     jmp CMFCEditBrowseCtrl::~CMFCEditBrowseCtrl   // 0x5f3d0 (mfc140u)
// DEVIATION: no vftable store here; the exported base destructor stores back
// the vptr the base constructor recorded before it runs ~CEdit
// (featurepack/controls/CMFCEditBrowseCtrl.cpp), which serves the same purpose.
// Symbol: ??1CMFCToolBarEditCtrl@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCToolBarEditCtrl__UEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    impl___1CMFCEditBrowseCtrl__UEAA_XZ(pThis);
}

// Retail (RVA 0x168e80, mfc140u), fully transcribed:
//     if (m_buttonEdit.m_uiMenuResID != 0) {                                   // +0xac
//         CWnd* pWndParent = pWnd->GetParent();     // FromHandle(::GetParent(pWnd->m_hWnd)); pWnd not null-checked
//         HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE((WORD)m_uiMenuResID), RT_MENU);
//         if (hInst != NULL) {
//             HMENU hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCE((WORD)m_uiMenuResID));   // IAT 0x2c6be8
//             if (hMenu != NULL) {
//                 HMENU hPopup = ::GetSubMenu(hMenu, 0);                           // IAT 0x2c6bf0
//                 if (hPopup != NULL) {
//                     if (afxContextMenuManager != NULL)                          // 0x3be1b0 (mfc140u)
//                         afxContextMenuManager->ShowPopupMenu(hPopup, point.x, point.y, pWndParent,
//                                                              FALSE, TRUE, FALSE);   // its vslot 5 (+0x28)
//                     else
//                         ::TrackPopupMenu(hPopup, TPM_CENTERALIGN /*4*/, point.x, point.y, 0,
//                                          pWndParent->GetSafeHwnd(), NULL);  // IAT 0x2c71c0
//                     return;
//                 }
//             }
//         }
//     }
//     Default();                                     // 0x28ac80 (CWnd::OnContextMenu is inline Default())
// (hMenu is never destroyed on any path; that is retail's behaviour and is kept.)
// DEVIATIONS: afxContextMenuManager is replaced by CurrentContextMenuManager()
// (see its comment); its ShowPopupMenu goes through the exported thunk, which
// re-dispatches on OpenMFC's own CContextMenuManager vtable, not through retail's
// MSVC slot 5; a null pWnd skips the parent lookup (pWndParent = NULL) where
// retail would fault.
// Symbol: ?OnContextMenu@CMFCToolBarEditCtrl@@IEAAXPEAVCWnd@@VCPoint@@@Z
extern "C" void MS_ABI impl__OnContextMenu_CMFCToolBarEditCtrl__IEAAXPEAVCWnd__VCPoint___Z(void* pThis, CWnd* pWnd, long long point) {
    if (pThis == nullptr) return;
    CMFCToolBarEditBoxButton* pButton = Button(pThis);
    if (pButton != nullptr && BtnMenuResID(pButton) != 0) {
        CWnd* pWndParent = pWnd != nullptr ? ParentOf(pWnd) : nullptr;
        const int x = static_cast<int>(static_cast<unsigned long long>(point) & 0xffffffffu);
        const int y = static_cast<int>(static_cast<unsigned long long>(point) >> 32);
        HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
            MAKEINTRESOURCEW(static_cast<WORD>(BtnMenuResID(pButton))), MAKEINTRESOURCEW(4) /*RT_MENU*/);
        if (hInst != nullptr) {
            HMENU hMenu = ::LoadMenuW(hInst, MAKEINTRESOURCEW(static_cast<WORD>(BtnMenuResID(pButton))));
            if (hMenu != nullptr) {
                HMENU hPopup = ::GetSubMenu(hMenu, 0);
                if (hPopup != nullptr) {
                    if (CContextMenuManager* pManager = CurrentContextMenuManager()) {
                        impl__ShowPopupMenu_CContextMenuManager__UEAAPEAVCMFCPopupMenu__PEAUHMENU____HHPEAVCWnd__HHH_Z(
                            pManager, hPopup, x, y, pWndParent, FALSE, TRUE, FALSE);
                    } else {
                        ::TrackPopupMenu(hPopup, TPM_CENTERALIGN, x, y, 0,
                                         pWndParent != nullptr ? pWndParent->m_hWnd : nullptr, nullptr);
                    }
                    return;
                }
            }
        }
    }
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
}

// Retail (RVA 0x168db0, mfc140u; the WM_KILLFOCUS entry), fully transcribed:
//     Default();                                     // 0x28ac80 (CWnd::OnKillFocus is inline Default())
//     m_buttonEdit.SetHotEdit(FALSE);                // 0x168950 (mfc140u), tail jump
// (pNewWnd is not read.)
// Symbol: ?OnKillFocus@CMFCToolBarEditCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnKillFocus_CMFCToolBarEditCtrl__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pNewWnd) {
    (void)pNewWnd;
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(W(pThis));
    SetHotEdit(pThis, FALSE);
}

// Retail (RVA 0x168e40, mfc140u; the WM_MOUSELEAVE entry), fully transcribed:
//     m_bTracked = FALSE;                            // +0x158
//     if (CWnd::FromHandle(::GetFocus()) != this)    // IAT 0x2c71b0; FromHandle 0x28ad70
//         m_buttonEdit.SetHotEdit(FALSE);            // 0x168950 (mfc140u)
// (No Default() call.)
// Symbol: ?OnMouseLeave@CMFCToolBarEditCtrl@@IEAAXXZ
extern "C" void MS_ABI impl__OnMouseLeave_CMFCToolBarEditCtrl__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    Tracked(pThis) = FALSE;
    if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus()) != W(pThis)) {
        SetHotEdit(pThis, FALSE);
    }
}

// Retail (RVA 0x168de0, mfc140u; the WM_MOUSEMOVE entry), fully transcribed:
//     CMFCEditBrowseCtrl::OnMouseMove(nFlags, point);   // 0x5f520 (mfc140u), arguments passed through
//     m_buttonEdit.SetHotEdit(TRUE);                    // 0x168950 (mfc140u)
//     if (!m_bTracked) {                                // +0x158
//         m_bTracked = TRUE;
//         TRACKMOUSEEVENT tme;  tme.cbSize = sizeof(tme) /*0x18*/;  tme.dwFlags = TME_LEAVE /*2*/;
//         tme.hwndTrack = m_hWnd;  ::TrackMouseEvent(&tme);     // IAT 0x2c7310 (dwHoverTime left unset)
//     }
// The TRACKMOUSEEVENT is zero-initialised here, so dwHoverTime is 0 instead of
// stack garbage; TME_LEAVE ignores it.
// Symbol: ?OnMouseMove@CMFCToolBarEditCtrl@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCToolBarEditCtrl__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    if (pThis == nullptr) return;
    impl__OnMouseMove_CMFCEditBrowseCtrl__IEAAXIVCPoint___Z(pThis, nFlags, point);
    SetHotEdit(pThis, TRUE);
    if (Tracked(pThis) == 0) {
        TRACKMOUSEEVENT tme = {};
        Tracked(pThis) = TRUE;
        tme.cbSize = sizeof(tme);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = W(pThis)->m_hWnd;
        ::TrackMouseEvent(&tme);
    }
}
static_assert(sizeof(TRACKMOUSEEVENT) == 0x18, "retail stores cbSize = 0x18");

// Retail (RVA 0x168d80, mfc140u; the WM_SETFOCUS entry), fully transcribed:
//     CWnd::OnSetFocus(pOldWnd);                     // direct call (0x28f2a0, mfc140u), pOldWnd passed through
//     m_buttonEdit.SetHotEdit(TRUE);                 // 0x168950 (mfc140u), tail jump
// Symbol: ?OnSetFocus@CMFCToolBarEditCtrl@@IEAAXPEAVCWnd@@@Z
extern "C" void MS_ABI impl__OnSetFocus_CMFCToolBarEditCtrl__IEAAXPEAVCWnd___Z(void* pThis, CWnd* pOldWnd) {
    if (pThis == nullptr) return;
    impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(W(pThis), pOldWnd);
    SetHotEdit(pThis, TRUE);
}

// Retail (RVA 0x168c50, mfc140u; vftable slot 69), fully transcribed:
//     if (pMsg->message == WM_KEYDOWN) {
//         if (pMsg->wParam == VK_TAB) {
//             if (GetParent() != NULL) {              // FromHandle(::GetParent(m_hWnd)), IAT 0x2c72d8
//                 GetParent()->GetNextDlgTabItem(this)->SetFocus();   // ::GetNextDlgTabItem(parent, m_hWnd, FALSE)
//                 return TRUE;                        //   IAT 0x2c6ce0; SetFocus 0x2a9b60 (mfc140u)
//             }
//         } else if (pMsg->wParam == VK_ESCAPE) {
//             if (GetTopLevelFrame() != NULL) {       // 0x28e490 (mfc140u), called twice
//                 GetTopLevelFrame()->SetFocus();
//                 return TRUE;
//             }
//         }
//         // VK_TAB / VK_ESCAPE whose target is NULL, and every other key, arrive here:
//         if (CWnd::FromHandle(::GetFocus()) == this && ::GetKeyState(VK_CONTROL) < 0) {   // IAT 0x2c71b0, 0x2c6ca0
//             UINT msg;
//             switch (pMsg->wParam) {
//             case 'Z':       msg = EM_UNDO;  break;  // 0x5a + 0x6d == 0xc7
//             case 'X':       msg = WM_CUT;   break;  // 0x300
//             case 'C':       msg = WM_COPY;  break;  // 0x301
//             case 'V':       msg = WM_PASTE; break;  // 0x302
//             case VK_DELETE: msg = WM_CLEAR; break;  // 0x303
//             default:        return CMFCEditBrowseCtrl::PreTranslateMessage(pMsg);
//             }
//             ::SendMessage(m_hWnd, msg, 0, 0);       // IAT 0x2c7120
//             return TRUE;
//         }
//     }
//     return CMFCEditBrowseCtrl::PreTranslateMessage(pMsg);   // 0x60ad0 (mfc140u), direct call
// DEVIATION: on VK_TAB a NULL next tab item skips SetFocus (retail calls
// CWnd::SetFocus on the NULL pointer) and still returns TRUE.
// Symbol: ?PreTranslateMessage@CMFCToolBarEditCtrl@@UEAAHPEAUtagMSG@@@Z
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBarEditCtrl__UEAAHPEAUtagMSG___Z(void* pThis, MSG* pMsg) {
    if (pThis == nullptr || pMsg == nullptr) return FALSE;
    CWnd* pWnd = W(pThis);
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_TAB) {
            if (ParentOf(pWnd) != nullptr) {
                CWnd* pParent = ParentOf(pWnd);
                CWnd* pNext = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(
                    ::GetNextDlgTabItem(pParent->m_hWnd, pWnd->m_hWnd, FALSE));
                if (pNext != nullptr) impl__SetFocus_CWnd__QEAAPEAV1_XZ(pNext);
                return TRUE;
            }
        } else if (pMsg->wParam == VK_ESCAPE) {
            if (impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pWnd) != nullptr) {
                impl__SetFocus_CWnd__QEAAPEAV1_XZ(
                    reinterpret_cast<CWnd*>(impl__GetTopLevelFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pWnd)));
                return TRUE;
            }
        }
        if (impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetFocus()) == pWnd && ::GetKeyState(VK_CONTROL) < 0) {
            UINT msg = 0;
            switch (pMsg->wParam) {
            case 'Z':       msg = EM_UNDO;  break;
            case 'X':       msg = WM_CUT;   break;
            case 'C':       msg = WM_COPY;  break;
            case 'V':       msg = WM_PASTE; break;
            case VK_DELETE: msg = WM_CLEAR; break;
            default:        break;
            }
            if (msg != 0) {
                ::SendMessage(pWnd->m_hWnd, msg, 0, 0);
                return TRUE;
            }
        }
    }
    return impl__PreTranslateMessage_CMFCEditBrowseCtrl__UEAAHPEAUtagMSG___Z(pThis, pMsg);
}
static_assert(EM_UNDO == 0xc7 && WM_CUT == 0x300 && WM_COPY == 0x301 && WM_PASTE == 0x302 && WM_CLEAR == 0x303,
              "message immediates in retail PreTranslateMessage");
