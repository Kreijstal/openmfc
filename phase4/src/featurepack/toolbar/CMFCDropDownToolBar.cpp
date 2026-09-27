// CMFCDropDownToolBar -- OpenMFC implementation.
//
// The toolbar a CMFCDropDownFrame shows when a CMFCDropDownToolbarButton drops
// down (retail afxdropdowntoolbar.h:32, `class CMFCDropDownToolBar : public
// CMFCToolBar`, DECLARE_SERIAL, no data members of its own: its inline ctor only
// sets m_bLocked = TRUE).
//
// Every body is transcribed from the retail export.  disas.py reads mfc140.dll
// (CreateObject 0x5cb20, OnSendCommand 0x5cbc0, OnUpdateCmdUI 0x5cc80,
// OnMouseMove 0x5ccd0, OnLButtonUp 0x5cfa0 there).  Only CreateObject is in
// mfc140u_rva_symbols.json; the other four were located in mfc140u.dll at the
// same +0x1b0 delta (and each of those RVAs is confirmed by mfc140u.dll's own
// export table, resolved by ordinal), and all five were compared
// instruction-for-instruction with the mfc140.dll bodies (identical apart from
// RIP-relative and call-target displacements).  RVAs below are mfc140u:
//   CreateObject   0x5ccd0     OnSendCommand  0x5cd70     OnUpdateCmdUI 0x5ce30
//   OnMouseMove    0x5ce80     OnLButtonUp    0x5d150
// The retail CMFCDropDownToolBar vftable is mfc140u VA 0x1802e7d68 (the one
// CreateObject installs; mfc140.dll VA 0x1802e5cb8).
//
// OpenMFC's include/openmfc does NOT declare CMFCDropDownToolBar, and this TU
// includes no openmfc header: including openmfc/afxmfc.h emits undefined
// references to C++ symbols (CWnd::classCWnd, CWnd::FromHandle, ...) that the
// per-file link audit rejects.  `this` is therefore void* and members are read
// at the byte offsets below, each of which is the offset the retail body uses
// and the offset include/openmfc/afxmfc.h declares for the CMFCToolBar member
// (class CMFCToolBar, afxmfc.h:641; sizeof 0x1350).
//
// Virtual calls.  An object whose vptr points into THIS image was built by
// OpenMFC (e.g. by CreateObject below) and carries an Itanium-shaped vftable, so
// the retail slot numbers mean nothing for it; such an object is sent to the
// exported impl__ thunk of the retail base implementation (or to the retail
// inline body, where the slot is not exported).  Any other object was built by
// a client, whose compiler emitted the MSVC-shaped vftable locally (the class's
// ctor is inline), and is dispatched through the retail slot.  This is the same
// IsOwnObject test core/frame/CFrameImpl.cpp uses.  DEVIATION for the own-object
// path: it calls the base implementation named at each wrapper directly, so an
// override of that virtual in the object's real OpenMFC class is NOT reached.
// For `this` that loses nothing (OpenMFC has no CMFCDropDownToolBar C++ type,
// and retail's class overrides none of the slots called here); for the frame
// passed to VDestroyWindow it would bypass a DestroyWindow override (retail
// CMFCDropDownFrame / CMiniFrameWnd declare none; CMDIChildWnd does), and
// VIsFrameWnd's CWnd thunk answers by dynamic_cast, so it stays correct.

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <cstring>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

// ---------------------------------------------------------------------------
// Exported thunks used below (BRIEFING S1).  Each definition was read; parameter
// lists follow the mangled names.  Class pointers are void* (identical ABI: one
// pointer register); a CPoint passed by value is an 8-byte aggregate passed in
// one register under the MS ABI and is spelled `long long`.
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                          // detail/MemcoreSupport.cpp
extern "C" void* MS_ABI impl___0CMFCToolBar__QEAA_XZ(void* pThis);                      // featurepack/toolbar/Thunks.cpp
extern "C" void* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
    const void* pThis, int iButton);                                                     // featurepack/toolbar/Thunks.cpp
extern "C" void* MS_ABI impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(
    void* pThis, int iButton);                                                           // featurepack/toolbar/CMFCToolBar.cpp
extern "C" int MS_ABI impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(void* pThis, long long point);          // CMFCToolBar.cpp
extern "C" void MS_ABI impl__SetButtonStyle_CMFCToolBar__UEAAXHI_Z(void* pThis, int nIndex, unsigned int nStyle); // CMFCToolBar.cpp
extern "C" void MS_ABI impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(void* pThis, unsigned int nID);  // CMFCToolBar.cpp
extern "C" void MS_ABI impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(void* pThis, int iHot);                     // CMFCToolBar.cpp
extern "C" void MS_ABI impl__OnLButtonUp_CMFCToolBar__IEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long point);                                  // CMFCToolBar.cpp
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* pTarget, int bDisableIfNoHndler);                                 // CMFCToolBar.cpp
extern "C" void* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(const void* pThis);             // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(const void* pWnd);        // featurepack/CMFC_misc_stubs.cpp
extern "C" void* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);                         // core/window/CWnd.cpp
extern "C" int MS_ABI impl__IsFrameWnd_CWnd__UEBAHXZ(const void* pThis);                                  // core/window/CWnd.cpp
extern "C" int MS_ABI impl__DestroyWindow_CWnd__UEAAHXZ(void* pThis);                                     // core/window/CWnd.cpp
extern "C" void MS_ABI impl__SetDefaultCommand_CMFCDropDownToolbarButton__QEAAXI_Z(void* pThis, unsigned int uiCmd); // CMFCDropDownToolbarButton.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                                       // detail/MfcExceptionsSupport.cpp

// Exported statics (retail mfc140.dll RVAs 0x3b70bc / 0x3b70c0 as the bodies
// read them; storage in featurepack/toolbar/StaticData.cpp and
// core/runtime/StaticData.cpp respectively).
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void*        impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA;

// The linker-provided base of this DLL's own image (IsOwnObject).
extern "C" IMAGE_DOS_HEADER __ImageBase;

namespace {

// --- CWnd / CMFCToolBar members the bodies read (offset == retail == afxmfc.h) ---
constexpr std::size_t kOffHWnd                 = 0x40;    // CWnd::m_hWnd
constexpr std::size_t kOffLocked               = 0x10b8;  // BOOL  m_bLocked               (afxmfc.h:679)
constexpr std::size_t kOffTracked              = 0x10e8;  // BOOL  m_bTracked              (afxmfc.h:691)
constexpr std::size_t kOffRouteCommandsViaFrame= 0x10f4;  // BOOL  m_bRouteCommandsViaFrame(afxmfc.h:694)
constexpr std::size_t kOffButtonCapture        = 0x1134;  // int   m_iButtonCapture        (afxmfc.h:709)
constexpr std::size_t kOffHighlighted          = 0x1138;  // int   m_iHighlighted          (afxmfc.h:710)
constexpr std::size_t kOffPtLastMouse          = 0x12d8;  // CPoint m_ptLastMouse          (afxmfc.h:736)
constexpr std::size_t kSizeofDropDownToolBar   = 0x1350;  // CreateObject: operator new(0x1350) == sizeof(CMFCToolBar)
static_assert(kOffHighlighted == kOffButtonCapture + 4, "m_iButtonCapture / m_iHighlighted are adjacent ints");
static_assert(kOffPtLastMouse + 8 <= kSizeofDropDownToolBar, "m_ptLastMouse lies inside the object");

// --- CMFCToolBarButton members (afxmfc.h, pinned by static_asserts in e.g.
//     featurepack/toolbar/CMFCToolBarDateTimeCtrl.cpp) ---
constexpr std::size_t kOffBtnID    = 0x24;                // UINT m_nID
constexpr std::size_t kOffBtnStyle = 0x28;                // UINT m_nStyle
constexpr unsigned kTbbsSeparator  = 0x00001;             // TBBS_SEPARATOR (testb $0x1)
constexpr unsigned kTbbsPressed    = 0x20000;             // TBBS_PRESSED   (btr $0x11)
constexpr unsigned kTbbsDisabled   = 0x40000;             // TBBS_DISABLED  (testl $0x40000)

// --- CMFCDropDownFrame member (featurepack/controls/CMFCDropDownFrame.cpp header) ---
constexpr std::size_t kOffFrameParentBtn = 0x1f8;         // CMFCDropDownToolbarButton* m_pParentBtn

// --- Retail vftable slots, read out of the CMFCDropDownToolBar vftable
//     (mfc140u VA 0x1802e7d68) -- slot = byte offset / 8 ---
constexpr int kSlotDestroyWindow        = 26;   // +0x0d0 -> 0x28baf0 ?DestroyWindow@CWnd@@UEAAHXZ
constexpr int kSlotIsFrameWnd           = 86;   // +0x2b0 CWnd::IsFrameWnd: `mov $1,%eax` in the CMFCDropDownFrame
                                                //  vftable (mfc140.dll VA 0x1802e5898 -> 0x3ae0), `xor eax,eax`
                                                //  in this one (-> 0x71e0 mfc140u)
constexpr int kSlotSetButtonStyle       = 223;  // +0x6f8 -> 0x14f040 ?SetButtonStyle@CMFCToolBar@@UEAAXHI@Z (by the
                                                //  mfc140u export table; the mfc140.dll twin slot -> 0x14d6b0, same name)
constexpr int kSlotHitTest              = 230;  // +0x730 -> 0x150390 ?HitTest@CMFCToolBar@@UEAAHVCPoint@@@Z
constexpr int kSlotOnChangeHot          = 238;  // +0x770 -> 0x1576f0 ?OnChangeHot@CMFCToolBar@@UEAAXH@Z
constexpr int kSlotGetCommandTarget     = 243;  // +0x798 -> 0x5cbd0 (CMFCToolBar::GetCommandTarget, not exported)
constexpr int kSlotAllowSelectDisabled  = 253;  // +0x7e8 -> 0x71e0 `xor eax,eax; ret`; named from retail
                                                //  afxtoolbar.h, where AllowSelectDisabled() directly follows
                                                //  OnSendCommand (slot 252 -> this class's OnSendCommand, 0x5cd70)
constexpr int kSlotShowCommandMessage   = 264;  // +0x840 -> 0x1566f0 ?ShowCommandMessageString@CMFCToolBar@@MEAAXI@Z
                                                //  (mfc140u export table; mfc140.dll twin slot -> 0x154d50, same name)

constexpr UINT kWmSetMessageString  = 0x0362;   // WM_SETMESSAGESTRING
constexpr UINT kAfxIdsIdleMessage   = 0xE001;   // AFX_IDS_IDLEMESSAGE

template <class T> inline T& At(void* p, std::size_t off) {
    return *reinterpret_cast<T*>(static_cast<char*>(p) + off);
}
template <class T> inline const T& At(const void* p, std::size_t off) {
    return *reinterpret_cast<const T*>(static_cast<const char*>(p) + off);
}
inline HWND HWndOf(const void* pWnd) { return At<HWND>(pWnd, kOffHWnd); }

inline POINT UnpackPoint(long long packed) {
    POINT pt;
    std::memcpy(&pt, &packed, sizeof pt);
    return pt;
}
inline long long PackPoint(const POINT& pt) {
    long long packed;
    std::memcpy(&packed, &pt, sizeof packed);
    return packed;
}

// Retail calls the CRT's labs() on a 32-bit difference (IAT slot
// api-ms-win-crt-utility!labs, and it compares the result signed: `cmp $1; jge`).
// Labs32 returns the two's-complement negation, so INT_MIN maps to INT_MIN
// without signed-overflow UB.  (The Microsoft UCRT labs body was not
// disassembled; INT_MIN is the value a plain negate produces.)
inline int Labs32(int v) {
    return v < 0 ? static_cast<int>(0u - static_cast<unsigned>(v)) : v;
}
inline int Sub32(int a, int b) {
    return static_cast<int>(static_cast<unsigned>(a) - static_cast<unsigned>(b));
}

bool IsOwnObject(const void* pObject) {
    const void* vptr = *reinterpret_cast<const void* const*>(pObject);
    const unsigned char* base = reinterpret_cast<const unsigned char*>(&__ImageBase);
    const IMAGE_NT_HEADERS* nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(
        base + reinterpret_cast<const IMAGE_DOS_HEADER*>(base)->e_lfanew);
    const unsigned char* p = static_cast<const unsigned char*>(vptr);
    return p >= base && p < base + nt->OptionalHeader.SizeOfImage;
}

template <typename Fn>
Fn VSlot(const void* pObject, int nSlot) {
    void* const* vtbl = *reinterpret_cast<void* const* const*>(pObject);
    return reinterpret_cast<Fn>(vtbl[nSlot]);
}

// --- virtual calls (see the file header) ------------------------------------
int VHitTest(void* pThis, long long point) {
    if (IsOwnObject(pThis)) return impl__HitTest_CMFCToolBar__UEAAHVCPoint___Z(pThis, point);
    typedef int (MS_ABI* Fn)(void*, long long);
    return VSlot<Fn>(pThis, kSlotHitTest)(pThis, point);
}

int VAllowSelectDisabled(void* pThis) {
    // Own object: the retail slot for this class is `xor eax,eax; ret`.
    if (IsOwnObject(pThis)) return FALSE;
    typedef int (MS_ABI* Fn)(const void*);
    return VSlot<Fn>(pThis, kSlotAllowSelectDisabled)(pThis);
}

void VSetButtonStyle(void* pThis, int nIndex, unsigned int nStyle) {
    if (IsOwnObject(pThis)) { impl__SetButtonStyle_CMFCToolBar__UEAAXHI_Z(pThis, nIndex, nStyle); return; }
    typedef void (MS_ABI* Fn)(void*, int, unsigned int);
    VSlot<Fn>(pThis, kSlotSetButtonStyle)(pThis, nIndex, nStyle);
}

void VShowCommandMessageString(void* pThis, unsigned int nID) {
    if (IsOwnObject(pThis)) { impl__ShowCommandMessageString_CMFCToolBar__MEAAXI_Z(pThis, nID); return; }
    typedef void (MS_ABI* Fn)(void*, unsigned int);
    VSlot<Fn>(pThis, kSlotShowCommandMessage)(pThis, nID);
}

void VOnChangeHot(void* pThis, int iHot) {
    if (IsOwnObject(pThis)) { impl__OnChangeHot_CMFCToolBar__UEAAXH_Z(pThis, iHot); return; }
    typedef void (MS_ABI* Fn)(void*, int);
    VSlot<Fn>(pThis, kSlotOnChangeHot)(pThis, iHot);
}

int VIsFrameWnd(const void* pWnd) {
    if (IsOwnObject(pWnd)) return impl__IsFrameWnd_CWnd__UEBAHXZ(pWnd);
    typedef int (MS_ABI* Fn)(const void*);
    return VSlot<Fn>(pWnd, kSlotIsFrameWnd)(pWnd);
}

int VDestroyWindow(void* pWnd) {
    if (IsOwnObject(pWnd)) return impl__DestroyWindow_CWnd__UEAAHXZ(pWnd);
    typedef int (MS_ABI* Fn)(void*);
    return VSlot<Fn>(pWnd, kSlotDestroyWindow)(pWnd);
}

// GetOwner() as retail inlines it: `m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd)`
// (CWnd + 0xa0).  DEVIATION, shared with CMFCToolBar.cpp / CMFCPopupMenuBar.cpp:
// OpenMFC's CWnd does not name m_hWndOwner (that range is anonymous padding
// nothing writes), so the ::GetParent branch is taken unconditionally.
inline HWND OwnerHwnd(const void* pThis) {
    return ::GetParent(HWndOf(pThis));
}

// CMFCToolBar::GetCommandTarget() -- vslot 243.  The base body is not exported;
// for an own object it is inlined as retail has it (RVA 0x5cbd0, mfc140u):
//     CWnd* pOwner = CWnd::FromHandle(m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd));
//     if (pOwner != NULL && (!m_bRouteCommandsViaFrame /*+0x10f4*/ || pOwner->IsFrameWnd() /*vslot 86*/))
//         return pOwner;
//     return AFXGetParentFrame(this);                                    // 0x6bd00 (mfc140u)
// (the owner lookup deviates as described at OwnerHwnd).
void* VGetCommandTarget(void* pThis) {
    if (!IsOwnObject(pThis)) {
        typedef void* (MS_ABI* Fn)(const void*);
        return VSlot<Fn>(pThis, kSlotGetCommandTarget)(pThis);
    }
    void* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
    if (pOwner != nullptr) {
        if (At<int>(pThis, kOffRouteCommandsViaFrame) == 0) return pOwner;
        if (VIsFrameWnd(pOwner)) return pOwner;
    }
    return impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pThis);
}

inline void* ButtonOrThrow(void* pThis, int iButton) {
    void* p = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pThis, iButton);
    if (p == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();   // retail ENSURE -> 0x225b80 (mfc140)
    return p;
}

} // namespace

// CMFCDropDownToolBar::CreateObject() -- transcribed from RVA 0x5ccd0 (mfc140u):
//     p = operator new(0x1350);                                   // ??2@YAPEAX_K@Z
//     if (p) { CMFCToolBar::CMFCToolBar(p);                       // ??0CMFCToolBar@@QEAA@XZ
//              p->vfptr = CMFCDropDownToolBar::`vftable';         // 0x1802e7d68 (mfc140u)
//              p->m_bLocked = TRUE; }                             // +0x10b8 (the inline ctor)
//     return p;
// DEVIATION: OpenMFC has no CMFCDropDownToolBar vftable to install (the class
// is not declared in include/openmfc and no ??_7 vftable is exported), so the
// object keeps the vptr the exported CMFCToolBar constructor installs: it is
// correctly sized and initialised, but GetRuntimeClass reports CMFCToolBar and
// the overrides in this file are reached only through their exports.  Same
// approximation as CMFCTasksPaneToolBar::CreateObject.
// Symbol: ?CreateObject@CMFCDropDownToolBar@@SAPEAVCObject@@XZ
extern "C" void* MS_ABI impl__CreateObject_CMFCDropDownToolBar__SAPEAVCObject__XZ() {
    void* p = impl___2_YAPEAX_K_Z(kSizeofDropDownToolBar);
    if (p == nullptr) return nullptr;
    impl___0CMFCToolBar__QEAA_XZ(p);
    At<int>(p, kOffLocked) = TRUE;
    return p;
}

// CMFCDropDownToolBar::OnLButtonUp(UINT nFlags, CPoint point) -- transcribed from
// RVA 0x5d150 (mfc140u):
//     CRect rectClient(0, 0, 0, 0);  ::GetClientRect(m_hWnd, &rectClient);
//     if (!CMFCToolBar::m_bCustomizeMode) {
//         if (!::PtInRect(&rectClient, point)) {
//             GetParentFrame()->DestroyWindow();          // 0x28e200 (mfc140u), then vslot 0xd0 (26)
//             return;                                      // base OnLButtonUp NOT called
//         }
//         if (m_iHighlighted >= 0) {                       // +0x1138 (`js`)
//             m_iButtonCapture = m_iHighlighted;           // +0x1134
//             GetButton(m_iHighlighted)->m_nStyle &= ~TBBS_PRESSED;   // 0x14fe00; btrl $0x11, 0x28(btn)
//         }
//     }
//     CMFCToolBar::OnLButtonUp(nFlags, point);             // direct call (not virtual)
// Retail re-reads m_bCustomizeMode before the m_iHighlighted block (only the
// USER32 PtInRect call lies between the two reads); folded into one test here.  DEVIATIONS: retail dereferences the
// GetParentFrame() and GetButton() results unchecked; both are NULL-checked here
// (OpenMFC's side-table GetButton can return NULL).  Note the base
// CMFCToolBar::OnLButtonUp thunk is itself still a stub in CMFCToolBar.cpp.
// Symbol: ?OnLButtonUp@CMFCDropDownToolBar@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCDropDownToolBar__QEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long pointPacked) {
    if (!pThis) return;
    const POINT point = UnpackPoint(pointPacked);

    RECT rectClient = { 0, 0, 0, 0 };
    ::GetClientRect(HWndOf(pThis), &rectClient);

    if (impl__m_bCustomizeMode_CMFCToolBar__1HA == 0) {
        if (!::PtInRect(&rectClient, point)) {
            void* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
            if (pFrame != nullptr) VDestroyWindow(pFrame);
            return;
        }
        const int iHighlighted = At<int>(pThis, kOffHighlighted);
        if (iHighlighted >= 0) {
            At<int>(pThis, kOffButtonCapture) = iHighlighted;
            void* pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pThis, iHighlighted);
            if (pButton != nullptr) At<unsigned>(pButton, kOffBtnStyle) &= ~kTbbsPressed;
        }
    }

    impl__OnLButtonUp_CMFCToolBar__IEAAXIVCPoint___Z(pThis, nFlags, pointPacked);
}

// CMFCDropDownToolBar::OnMouseMove(UINT, CPoint point) -- transcribed from
// RVA 0x5ce80 (mfc140u); nFlags is not read:
//     if (m_ptLastMouse != CPoint(-1,-1) &&                              // +0x12d8/+0x12dc
//         labs(m_ptLastMouse.x - point.x) < 1 && labs(m_ptLastMouse.y - point.y) < 1) {
//         m_ptLastMouse = point;  return;
//     }
//     m_ptLastMouse = point;
//     int iPrevHighlighted = m_iHighlighted;                             // +0x1138
//     m_iHighlighted = HitTest(point);                                   // vslot 0x730 (230)
//     CMFCToolBarButton* pButton = (m_iHighlighted == -1) ? NULL : GetButton(m_iHighlighted);
//     if (pButton != NULL && ((pButton->m_nStyle & TBBS_SEPARATOR) ||
//             ((pButton->m_nStyle & TBBS_DISABLED) && !AllowSelectDisabled())))  // vslot 0x7e8 (253)
//         m_iHighlighted = -1;                                           // pButton itself is kept
//     if (!m_bTracked) {                                                 // +0x10e8
//         m_bTracked = TRUE;
//         TRACKMOUSEEVENT t; t.cbSize = 0x18; t.dwFlags = TME_LEAVE; t.hwndTrack = m_hWnd;
//         ::TrackMouseEvent(&t);                                         // IAT, USER32
//     }
//     if (iPrevHighlighted == m_iHighlighted) return;
//     m_iButtonCapture = m_iHighlighted;                                 // +0x1134
//     if (iPrevHighlighted != -1) {
//         pTBB = GetButton(iPrevHighlighted);  ENSURE(pTBB);
//         nNew = pTBB->m_nStyle & ~TBBS_PRESSED;
//         if (nNew != pTBB->m_nStyle) SetButtonStyle(iPrevHighlighted, nNew);   // vslot 0x6f8 (223)
//     }
//     BOOL bNeedUpdate = FALSE;
//     if (m_iButtonCapture != -1) {
//         pTBB = GetButton(m_iButtonCapture);  ENSURE(pTBB);
//         nNew = pTBB->m_nStyle & ~TBBS_PRESSED;
//         if (m_iHighlighted == m_iButtonCapture) nNew |= TBBS_PRESSED;
//         if (nNew != pTBB->m_nStyle) { SetButtonStyle(m_iButtonCapture, nNew); bNeedUpdate = TRUE; }
//     }
//     if ((m_iButtonCapture == -1 || iPrevHighlighted == m_iButtonCapture) && iPrevHighlighted != -1) {
//         InvalidateButton(iPrevHighlighted);  bNeedUpdate = TRUE;      // 0x14fe50 (mfc140u)
//     }
//     if ((m_iButtonCapture == -1 || m_iHighlighted == m_iButtonCapture) && m_iHighlighted != -1) {
//         InvalidateButton(m_iHighlighted);  bNeedUpdate = TRUE;
//     }
//     if (bNeedUpdate) ::UpdateWindow(m_hWnd);
//     if (m_iHighlighted != -1 && (m_iHighlighted == m_iButtonCapture || m_iButtonCapture == -1)) {
//         ENSURE(pButton);  ShowCommandMessageString(pButton->m_nID);   // vslot 0x840 (264)
//     } else if (m_iButtonCapture == -1 && CMFCToolBar::m_hookMouseHelp == NULL) {
//         ::SendMessage(CWnd::FromHandle(GetOwner-hwnd)->m_hWnd, WM_SETMESSAGESTRING, AFX_IDS_IDLEMESSAGE, 0);
//     }
//     OnChangeHot(m_iHighlighted);                                       // vslot 0x770 (238)
// ENSURE failures call AfxThrowInvalidArgException (0x225b80, mfc140).
// DEVIATIONS: retail leaves TRACKMOUSEEVENT::dwHoverTime unwritten (ignored for
// TME_LEAVE); it is zeroed here.  The owner lookup takes the ::GetParent branch
// (see OwnerHwnd) and a NULL FromHandle result is skipped where retail would
// dereference it.
// Symbol: ?OnMouseMove@CMFCDropDownToolBar@@QEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnMouseMove_CMFCDropDownToolBar__QEAAXIVCPoint___Z(
    void* pThis, unsigned int nFlags, long long pointPacked) {
    (void)nFlags;
    if (!pThis) return;
    const POINT point = UnpackPoint(pointPacked);

    POINT& ptLast = At<POINT>(pThis, kOffPtLastMouse);
    if (ptLast.x != -1 || ptLast.y != -1) {
        if (Labs32(Sub32(ptLast.x, point.x)) < 1 && Labs32(Sub32(ptLast.y, point.y)) < 1) {
            ptLast = point;
            return;
        }
    }

    int& iHighlighted  = At<int>(pThis, kOffHighlighted);
    int& iButtonCapture = At<int>(pThis, kOffButtonCapture);

    ptLast = point;
    const int iPrevHighlighted = iHighlighted;
    iHighlighted = VHitTest(pThis, pointPacked);

    void* pButton = nullptr;
    if (iHighlighted != -1) {
        pButton = impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(pThis, iHighlighted);
        if (pButton != nullptr) {
            const unsigned style = At<unsigned>(pButton, kOffBtnStyle);
            if ((style & kTbbsSeparator) != 0 ||
                ((style & kTbbsDisabled) != 0 && !VAllowSelectDisabled(pThis))) {
                iHighlighted = -1;
            }
        }
    }

    if (At<int>(pThis, kOffTracked) == 0) {
        At<int>(pThis, kOffTracked) = TRUE;
        TRACKMOUSEEVENT tme;
        tme.cbSize = sizeof(TRACKMOUSEEVENT);   // 0x18
        tme.dwFlags = TME_LEAVE;                // 2
        tme.hwndTrack = HWndOf(pThis);
        tme.dwHoverTime = 0;                    // unwritten in retail (see DEVIATIONS)
        ::TrackMouseEvent(&tme);
    }

    if (iHighlighted == iPrevHighlighted) return;

    iButtonCapture = iHighlighted;

    if (iPrevHighlighted != -1) {
        void* pTBB = ButtonOrThrow(pThis, iPrevHighlighted);
        const unsigned nNew = At<unsigned>(pTBB, kOffBtnStyle) & ~kTbbsPressed;
        if (nNew != At<unsigned>(pTBB, kOffBtnStyle)) {
            VSetButtonStyle(pThis, iPrevHighlighted, nNew);
        }
    }

    BOOL bNeedUpdate = FALSE;
    if (iButtonCapture != -1) {
        void* pTBB = ButtonOrThrow(pThis, iButtonCapture);
        unsigned nNew = At<unsigned>(pTBB, kOffBtnStyle) & ~kTbbsPressed;
        if (iButtonCapture == iHighlighted) nNew |= kTbbsPressed;
        if (nNew != At<unsigned>(pTBB, kOffBtnStyle)) {
            VSetButtonStyle(pThis, iButtonCapture, nNew);
            bNeedUpdate = TRUE;
        }
    }

    if ((iButtonCapture == -1 || iButtonCapture == iPrevHighlighted) && iPrevHighlighted != -1) {
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iPrevHighlighted);
        bNeedUpdate = TRUE;
    }
    if ((iButtonCapture == -1 || iButtonCapture == iHighlighted) && iHighlighted != -1) {
        impl__InvalidateButton_CMFCToolBar__QEAAPEAVCMFCToolBarButton__H_Z(pThis, iHighlighted);
        bNeedUpdate = TRUE;
    }
    if (bNeedUpdate) ::UpdateWindow(HWndOf(pThis));

    if (iHighlighted != -1 && (iHighlighted == iButtonCapture || iButtonCapture == -1)) {
        if (pButton == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
        VShowCommandMessageString(pThis, At<unsigned>(pButton, kOffBtnID));
    } else if (iButtonCapture == -1 && impl__m_hookMouseHelp_CMFCToolBar__1PEAUHHOOK____EA == nullptr) {
        void* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
        if (pOwner != nullptr) {
            ::SendMessage(HWndOf(pOwner), kWmSetMessageString, kAfxIdsIdleMessage, 0);
        }
    }

    VOnChangeHot(pThis, iHighlighted);
}

// CMFCDropDownToolBar::OnSendCommand(const CMFCToolBarButton* pButton) --
// transcribed from RVA 0x5cd70 (mfc140u):
//     if ((pButton->m_nStyle & TBBS_DISABLED) ||                         // +0x28
//         pButton->m_nID == 0 || pButton->m_nID == (UINT)-1)             // `dec; cmp $0xfffffffd; ja`
//         return FALSE;
//     CMFCDropDownFrame* pParent = (CMFCDropDownFrame*)CWnd::FromHandle(::GetParent(m_hWnd));
//     pParent->m_pParentBtn->SetDefaultCommand(pButton->m_nID);           // +0x1f8; direct call
//     CFrameWnd* pParentFrame = GetParentFrame();                         // 0x28e200 (mfc140u)
//     ::PostMessage(CWnd::FromHandle(GetOwner-hwnd)->m_hWnd, WM_COMMAND, pButton->m_nID, 0);
//     pParentFrame->DestroyWindow();                                      // vslot 0xd0 (26)
//     return TRUE;
// (IAT slots resolve to USER32 GetParent / PostMessageA in mfc140.dll, i.e.
// PostMessageW in mfc140u; ::PostMessage picks it under UNICODE.)
// DEVIATIONS: retail dereferences pButton, the parent CWnd, m_pParentBtn, the
// owner CWnd and pParentFrame unchecked; each is NULL-checked here (a NULL
// pButton returns FALSE; the other checks skip just that step).  The owner
// lookup takes the ::GetParent branch (see OwnerHwnd).  Note: the parent is
// assumed to be a retail-shaped CMFCDropDownFrame exactly as retail assumes it;
// OpenMFC cannot build one yet (see featurepack/controls/CMFCDropDownFrame.cpp).
// Symbol: ?OnSendCommand@CMFCDropDownToolBar@@UEAAHPEBVCMFCToolBarButton@@@Z
extern "C" int MS_ABI impl__OnSendCommand_CMFCDropDownToolBar__UEAAHPEBVCMFCToolBarButton___Z(
    void* pThis, const void* pButton) {
    if (!pThis || !pButton) return FALSE;
    if ((At<unsigned>(pButton, kOffBtnStyle) & kTbbsDisabled) != 0) return FALSE;
    const unsigned nID = At<unsigned>(pButton, kOffBtnID);
    if (nID == 0 || nID == static_cast<unsigned>(-1)) return FALSE;

    void* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(HWndOf(pThis)));
    if (pParent != nullptr) {
        void* pParentBtn = At<void*>(pParent, kOffFrameParentBtn);
        if (pParentBtn != nullptr) {
            impl__SetDefaultCommand_CMFCDropDownToolbarButton__QEAAXI_Z(pParentBtn, nID);
        }
    }

    void* pParentFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);

    void* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(OwnerHwnd(pThis));
    if (pOwner != nullptr) {
        ::PostMessage(HWndOf(pOwner), WM_COMMAND, nID, 0);
    }

    if (pParentFrame != nullptr) VDestroyWindow(pParentFrame);
    return TRUE;
}

// CMFCDropDownToolBar::OnUpdateCmdUI(CFrameWnd*, BOOL bDisableIfNoHndler) --
// transcribed from RVA 0x5ce30 (mfc140u):
//     CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetCommandTarget(), bDisableIfNoHndler);
// GetCommandTarget is vslot 0x798 (243); the base call is a direct tail jump
// (not virtual).  The caller's pTarget is ignored, as in retail.
// Symbol: ?OnUpdateCmdUI@CMFCDropDownToolBar@@UEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCDropDownToolBar__UEAAXPEAVCFrameWnd__H_Z(
    void* pThis, void* pTarget, int bDisableIfNoHndler) {
    (void)pTarget;
    if (!pThis) return;
    void* pCmdTarget = VGetCommandTarget(pThis);
    impl__OnUpdateCmdUI_CMFCToolBar__UEAAXPEAVCFrameWnd__H_Z(pThis, pCmdTarget, bDisableIfNoHndler);
}
