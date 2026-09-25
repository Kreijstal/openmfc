// CMFCToolBarsListCheckBox — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

// ===========================================================================
// CMFCToolBarsListCheckBox -- the check list box on the "Toolbars" page of the
// feature-pack Customize dialog (afxtoolbarslistcheckbox.h, derives
// CCheckListBox).  Its one own member is CArray<BOOL,BOOL> m_arCheckData, a
// per-item "check box may be toggled" flag kept parallel to the list items.
//
// Bodies below were transcribed from the retail disassembly (the method in
// the header of core/ole/COleControl.cpp).  RVAs are mfc140u unless marked.
// The ctor, dtor, EnableCheck and IsCheckEnabled resolve directly through
// disas.py --u.  The other eight (the seven message handlers plus the
// OnNewString helper) have no mfc140u entry in the RVA map; they resolve only
// in mfc140.dll (ANSI), and their mfc140u entries below were located at ANSI
// RVA + 0x19e0 (the delta IsCheckEnabled has: 0x179ad0 ANSI -> 0x17b4b0
// mfc140u) and confirmed by comparing each instruction stream (addresses
// masked) against the ANSI body -- all eight are identical.  The seven
// handler entries are independently confirmed by retail's message map for
// this class (AFX_MSGMAP at VA 0x180319940 (mfc140u), returned by
// GetMessageMap 0x17b2b0 (mfc140u)), whose entries point at exactly these
// mfc140u RVAs: WM_LBUTTONDOWN 0x17b2c0, WM_KEYDOWN 0x17b330,
// WM_LBUTTONDBLCLK 0x17b5a0, LB_ADDSTRING 0x17b3a0, LB_INSERTSTRING 0x17b3d0,
// LB_RESETCONTENT 0x17b450, LB_DELETESTRING 0x17b400:
//
//   ctor              0x17b1d0          OnKeyDown         0x17b330 (ANSI 0x179950)
//   dtor              0x17b280          OnLBAddString     0x17b3a0 (ANSI 0x1799c0)
//   EnableCheck       0x17b480          OnLBInsertString  0x17b3d0 (ANSI 0x1799f0)
//   IsCheckEnabled    0x17b4b0          OnLBDeleteString  0x17b400 (ANSI 0x179a20)
//   OnNewString       0x17b4e0 (ANSI 0x179b00)
//   OnLButtonDown     0x17b2c0 (ANSI 0x1798e0)
//   OnLButtonDblClk   0x17b5a0 (ANSI 0x179bc0)
//   OnLBResetContent  0x17b450 (ANSI 0x179a70)
//
// Import slots (resolved with iatu.py against mfc140u.dll):
// 0x1802c7120 = USER32!SendMessageW, 0x1802c7230 = USER32!MessageBeep,
// 0x1802c72d8 = USER32!GetParent, 0x1802c74e8 = CRT free,
// 0x1802c7410 = VCRUNTIME140!memmove.
//
// Layout (retail sizeof 0x118; CMFCToolBarsListPropertyPage.cpp embeds it as
// 0x118 bytes at +0x5e0).  Pinned by the ctor (0x17b1d0):
//   ??0CWnd@@QEAA@XZ (0x28a700) on this; byte 1 -> +0x48; qword 0 -> +0xe8;
//   class vftable -> +0x00; CArray<BOOL,BOOL> vftable -> +0xf0;
//   qword 0 -> +0xf8, +0x110, +0x108, +0x100.
// CCheckListBox's constructor is inlined there: +0x48 is retail CWnd's
// m_bEnableActiveAccessibility (EnableActiveAccessibility()), +0xe8/+0xec are
// retail CCheckListBox's m_cyText / m_nStyle (afxwin.h).  The CArray members
// follow from the offsets CArray<BOOL,BOOL>::SetSize (ANSI 0x15004, no export)
// uses relative to the array: +0x8 m_pData, +0x10 m_nSize, +0x18 m_nMaxSize,
// +0x20 m_nGrowBy.
//
// Structural deviations, applied uniformly:
//
//  (1) vtable pointers.  OpenMFC has no MSVC-layout vftable for this class or
//      for CArray<BOOL,BOOL>, and its CCheckListBox vtable cannot be named
//      here without a C++ symbol reference (the impl__ thunk rule).  The ctor
//      therefore builds the CWnd part with the exported CWnd constructor
//      thunk and leaves its vtable at +0x00 (the pattern
//      CMFCToolBarsListPropertyPage.cpp and
//      featurepack/toolbar/CMFCToolBarButtonCustomizeDialog.cpp use for their
//      embedded controls); the m_arCheckData vptr is left
//      NULL -- nothing in OpenMFC dispatches through it.  Retail's dtor
//      re-stamps its own vftable before tearing down the bases; the dtor here
//      re-stamps the vptr value the CWnd constructor thunk installed (captured
//      in s_cwndVptr) so the base destructor dispatches on OpenMFC's vtable
//      even when an MSVC client-derived destructor has stamped its own.
//
//  (2) m_bEnableActiveAccessibility.  Retail's `movb $1,0x48(%rbx)` is NOT
//      performed: OpenMFC's CWnd keeps m_pfnSuper at +0x48 (afxwin.h), and a
//      byte store there would corrupt it.
//
//  (3) Base-class calls go through the exported thunks (each declaration
//      below names the file that defines it).  OpenMFC's CCheckListBox keeps
//      check / enabled / style state in side tables keyed by the object's
//      address (g_checkStates, g_itemEnabledStates, g_checkStyles, defined in
//      detail/CCheckListBoxSupport.cpp).  A box's whole entry is erased only
//      by the C++ members CCheckListBox::Create and ~CCheckListBox
//      (core/controls/CCheckListBox.cpp; PreDeleteItem erases single items).
//      Neither runs on this object -- it is built on the CWnd constructor and
//      torn down through ??1CListBox, never as an OpenMFC CCheckListBox -- so
//      any entry created for it outlives it.  Reaching those tables from here
//      would need a C++ symbol.
//
//  (4) Message routing.  These handlers are reached in retail through
//      CMFCToolBarsListCheckBox's message map.  In OpenMFC nothing reaches
//      them today, for three separate reasons, and filling in the map alone
//      does NOT fix it:
//        (a) the class map (classCMFCToolBarsListCheckBox_msgmap,
//            detail/Toolbar22MsgmapSupport.cpp) has no entries of its own;
//        (b) this object's vptr is OpenMFC's (g++) CWnd vtable (deviation
//            (1)), so no virtual GetMessageMap dispatched on it can reach
//            this class's map getter;
//        (c) OpenMFC's CWnd::OnWndMsg (core/window/CWnd.cpp) walks no message
//            map at all -- it handles WM_COMMAND and WM_NOTIFY and returns
//            FALSE for everything else.
//      So m_arCheckData is not resized as strings are added, and since
//      EnableCheck / IsCheckEnabled bounds-check exactly as retail does
//      (AfxThrowInvalidArgException), calling them on an index the array has
//      not grown to throws.  HAZARD for whoever wires this up: OpenMFC's
//      CCheckListBox::OnLBAddString / OnLBInsertString (via CheckListBoxSend,
//      detail/CCheckListBoxSupport.cpp) re-send LB_ADDSTRING / LB_INSERTSTRING
//      to the control's own HWND with ::SendMessageW.  Once LB_ADDSTRING is
//      routed to OnLBAddString below, that re-send re-enters it: unbounded
//      recursion.  Those base thunks must reach the default window procedure
//      instead before these entries are routed.
//
//  (5) NULL checks.  Where retail dereferences an unchecked pointer (the
//      CWnd::FromHandle result in OnLButtonDblClk, a NULL allocation in
//      SetSize's first-allocation path), the site says what is done instead.
// ===========================================================================

// ---------------------------------------------------------------------------
// Thunks this file calls.  Signatures follow the definitions in the tree
// (file named on each line).
// ---------------------------------------------------------------------------
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                    // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CListBox__UEAA_XZ(CListBox* pThis);                            // core/controls/RuntimeClasses.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                           // core/window/Thunks.cpp
extern "C" void* MS_ABI impl__SetFocus_CWnd__QEAAPEAV1_XZ(CWnd* pThis);                        // core/window/Thunks.cpp
extern "C" int   MS_ABI impl__GetDlgCtrlID_CWnd__QEBAHXZ(const CWnd* pThis);                    // core/window/Thunks.cpp
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);               // core/window/CWnd.cpp

extern "C" int   MS_ABI impl__CheckFromPoint_CCheckListBox__IEAAHVCPoint__AEAH_Z(CCheckListBox* pThis, CPoint pt, int* pbInCheck);        // core/controls/CCheckListBox.cpp
extern "C" void  MS_ABI impl__OnLButtonDown_CCheckListBox__IEAAXIVCPoint___Z(CCheckListBox* pThis, UINT nFlags, CPoint point);          // core/controls/CCheckListBox.cpp
extern "C" void  MS_ABI impl__OnLButtonDblClk_CCheckListBox__IEAAXIVCPoint___Z(CCheckListBox* pThis, UINT nFlags, CPoint point);        // core/controls/CCheckListBox.cpp
extern "C" void  MS_ABI impl__OnKeyDown_CCheckListBox__IEAAXIII_Z(CCheckListBox* pThis, UINT nChar, UINT nRepCnt, UINT nFlags);        // core/controls/CCheckListBox.cpp
extern "C" LRESULT MS_ABI impl__OnLBAddString_CCheckListBox__IEAA_J_K_J_Z(CCheckListBox* pThis, WPARAM wParam, LPARAM lParam);          // core/controls/CCheckListBox.cpp
extern "C" LRESULT MS_ABI impl__OnLBInsertString_CCheckListBox__IEAA_J_K_J_Z(CCheckListBox* pThis, WPARAM wParam, LPARAM lParam);       // core/controls/CCheckListBox.cpp

extern "C" void* MS_ABI impl___2_YAPEAX_K_Z(std::size_t size);                                  // detail/MemcoreSupport.cpp
extern "C" void  MS_ABI impl___3_YAXPEAX_Z(void* ptr);                                          // detail/MemcoreSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                              // detail/MfcExceptionsSupport.cpp

// This file's own thunks that other bodies here call.
extern "C" int  MS_ABI impl__IsCheckEnabled_CMFCToolBarsListCheckBox__QEBAHH_Z(const void* pThis, int nIndex);
extern "C" void MS_ABI impl__OnNewString_CMFCToolBarsListCheckBox__IEAAXH_Z(void* pThis, int iIndex);

namespace {

// CArray<BOOL,BOOL> as retail lays it out (CObject vptr + four members).
struct S_BoolArray {
    void*          vptr;        // +0x00  retail: CArray<BOOL,BOOL> vftable; NULL here, deviation (1)
    int*           m_pData;     // +0x08
    std::ptrdiff_t m_nSize;     // +0x10
    std::ptrdiff_t m_nMaxSize;  // +0x18
    std::ptrdiff_t m_nGrowBy;   // +0x20
};
static_assert(sizeof(S_BoolArray) == 0x28, "CArray<BOOL,BOOL> is 0x28 bytes (0xf0 + 0x28 == 0x118)");

struct alignas(8) S_ToolbarsListCheckBox {
    unsigned char base[0xe8];   // CWnd / CListBox part (OpenMFC sizeof(CWnd) == 0xe8, asserted below)
    int           m_cyText;     // +0xe8 retail CCheckListBox::m_cyText
    unsigned int  m_nStyle;     // +0xec retail CCheckListBox::m_nStyle
    S_BoolArray   m_arCheckData;// +0xf0
};
static_assert(sizeof(CWnd) == 0xe8, "retail CCheckListBox's own members start at +0xe8 (ctor's store after ??0CWnd)");
static_assert(sizeof(CCheckListBox) == 0xe8, "OpenMFC CCheckListBox declares no members, so +0xe8.. belongs to this block");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");
static_assert(offsetof(S_ToolbarsListCheckBox, m_cyText) == 0xe8, "m_cyText");
static_assert(offsetof(S_ToolbarsListCheckBox, m_nStyle) == 0xec, "m_nStyle");
static_assert(offsetof(S_ToolbarsListCheckBox, m_arCheckData) == 0xf0, "m_arCheckData");
static_assert(offsetof(S_ToolbarsListCheckBox, m_arCheckData) + offsetof(S_BoolArray, m_pData) == 0xf8, "m_arCheckData.m_pData");
static_assert(offsetof(S_ToolbarsListCheckBox, m_arCheckData) + offsetof(S_BoolArray, m_nSize) == 0x100, "m_arCheckData.m_nSize");
static_assert(offsetof(S_ToolbarsListCheckBox, m_arCheckData) + offsetof(S_BoolArray, m_nMaxSize) == 0x108, "m_arCheckData.m_nMaxSize");
static_assert(offsetof(S_ToolbarsListCheckBox, m_arCheckData) + offsetof(S_BoolArray, m_nGrowBy) == 0x110, "m_arCheckData.m_nGrowBy");
static_assert(sizeof(S_ToolbarsListCheckBox) == 0x118, "retail sizeof(CMFCToolBarsListCheckBox) == 0x118");

constexpr unsigned int kClbnChkChange = 40;   // CLBN_CHKCHANGE, the `or $0x280000` in OnLButtonDblClk

inline S_ToolbarsListCheckBox* D(void* pThis) { return static_cast<S_ToolbarsListCheckBox*>(pThis); }
inline const S_ToolbarsListCheckBox* D(const void* pThis) { return static_cast<const S_ToolbarsListCheckBox*>(pThis); }
inline CWnd* Wnd(void* p) { return static_cast<CWnd*>(p); }
inline CCheckListBox* ChkList(void* p) { return static_cast<CCheckListBox*>(p); }
inline HWND Hwnd(const void* p) { return static_cast<const CWnd*>(p)->m_hWnd; }

// The vptr impl___0CWnd__QEAA_XZ installs; see deviation (1).
void* s_cwndVptr = nullptr;

// CArray<BOOL,BOOL>::SetSize(nNewSize) with nGrowBy == -1 folded in -- the
// out-of-line body retail calls from OnNewString and OnLBResetContent
// (mfc140u 0x15034; transcribed from its ANSI twin 0x15004, no export).
// Retail allocates with ??2 (operator new, mfc140u 0x27f0) and frees by
// calling the CRT free import (0x1802c74e8) directly; impl___2_ / impl___3_
// are this DLL's malloc / free pair.
void BoolArraySetSize(S_BoolArray* a, std::ptrdiff_t nNewSize) {
    if (nNewSize < 0) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    if (nNewSize == 0) {
        // shrink to nothing
        if (a->m_pData != nullptr) {
            impl___3_YAXPEAX_Z(a->m_pData);
            a->m_pData = nullptr;
        }
        a->m_nSize = a->m_nMaxSize = 0;
        return;
    }
    if (a->m_pData == nullptr) {
        // first allocation: max(nNewSize, m_nGrowBy) elements, all zeroed
        std::ptrdiff_t nAlloc = (a->m_nGrowBy > nNewSize) ? a->m_nGrowBy : nNewSize;
        int* p = static_cast<int*>(impl___2_YAPEAX_K_Z(static_cast<std::size_t>(nAlloc) * sizeof(int)));
        if (p == nullptr) {
            // Deviation (5): retail memsets the NULL result.  Throw instead,
            // as retail's later-growth path does for a NULL allocation.
            impl__AfxThrowInvalidArgException__YAXXZ();
            return;
        }
        std::memset(p, 0, static_cast<std::size_t>(nAlloc) * sizeof(int));
        a->m_pData = p;
        a->m_nSize = nNewSize;
        a->m_nMaxSize = nAlloc;
        return;
    }
    if (nNewSize <= a->m_nMaxSize) {
        // fits: zero any newly exposed tail
        if (nNewSize > a->m_nSize)
            std::memset(a->m_pData + a->m_nSize, 0,
                        static_cast<std::size_t>(nNewSize - a->m_nSize) * sizeof(int));
        a->m_nSize = nNewSize;
        return;
    }
    // grow: m_nGrowBy, or m_nSize/8 clamped to [4, 1024] when it is 0
    std::ptrdiff_t nGrowBy = a->m_nGrowBy;
    if (nGrowBy == 0) {
        std::ptrdiff_t n8 = a->m_nSize / 8;
        nGrowBy = (n8 > 1024) ? 1024 : n8;
        if (n8 < 4) nGrowBy = 4;
    }
    std::ptrdiff_t nNewMax = a->m_nMaxSize + nGrowBy;
    if (nNewSize >= nNewMax) {
        nNewMax = nNewSize;
    } else if (nNewMax < a->m_nMaxSize) {   // wrap-around
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    int* pNew = static_cast<int*>(impl___2_YAPEAX_K_Z(static_cast<std::size_t>(nNewMax) * sizeof(int)));
    if (pNew == nullptr) {
        // Retail, when m_nSize != 0: the inlined memcpy_s sees a NULL
        // destination -> errno EINVAL, _invalid_parameter_noinfo, then
        // AfxThrowInvalidArgException.  When m_nSize == 0 retail skips the
        // copy and memsets through the NULL pointer; deviation (5) throws in
        // both cases.
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    std::memcpy(pNew, a->m_pData, static_cast<std::size_t>(a->m_nSize) * sizeof(int));
    std::memset(pNew + a->m_nSize, 0, static_cast<std::size_t>(nNewSize - a->m_nSize) * sizeof(int));
    impl___3_YAXPEAX_Z(a->m_pData);
    a->m_pData = pNew;
    a->m_nSize = nNewSize;
    a->m_nMaxSize = nNewMax;
}

// CArray<BOOL,BOOL>::RemoveAt(nIndex, nCount).  Retail calls mfc140u 0x1b880,
// which is ?RemoveAt@CUIntArray@@QEAAX_J0@Z's body folded in (the ANSI map
// names 0x1b910 as that export); it is transcribed here rather than called
// through OpenMFC's CUIntArray thunk, whose C++ layout this file does not pin.
void BoolArrayRemoveAt(S_BoolArray* a, std::ptrdiff_t nIndex, std::ptrdiff_t nCount) {
    std::ptrdiff_t nUpperBound = nIndex + nCount;
    if (nIndex < 0 || nCount < 0 || nUpperBound > a->m_nSize ||
        nUpperBound < nIndex || nUpperBound < nCount) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    std::ptrdiff_t nMoveCount = a->m_nSize - nUpperBound;
    if (nMoveCount != 0)
        std::memmove(a->m_pData + nIndex, a->m_pData + nUpperBound,
                     static_cast<std::size_t>(nMoveCount) * sizeof(int));
    a->m_nSize -= nCount;
}

// m_arCheckData[nIndex] with CArray::operator[]'s bounds check (the
// `test/js` + `cmp 0x100(...)/jge` -> AfxThrowInvalidArgException pairs).
int* BoolArrayAt(const S_BoolArray* a, std::ptrdiff_t nIndex) {
    if (nIndex < 0 || nIndex >= a->m_nSize) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return nullptr;
    }
    return a->m_pData + nIndex;
}

}  // namespace

// Symbol: ??0CMFCToolBarsListCheckBox@@QEAA@XZ
// Retail 0x17b1d0 (mfc140u):
//   CWnd::CWnd(); m_bEnableActiveAccessibility = true;  (inlined CCheckListBox())
//   m_cyText = m_nStyle = 0;  vptr = CMFCToolBarsListCheckBox vftable;
//   m_arCheckData: vptr = CArray<BOOL,BOOL> vftable, m_pData = NULL,
//                  m_nGrowBy = m_nMaxSize = m_nSize = 0;  return this;
// Deviations (1) and (2).
extern "C" void* MS_ABI impl___0CMFCToolBarsListCheckBox__QEAA_XZ(void* pThis) {
    impl___0CWnd__QEAA_XZ(pThis);
    std::memcpy(&s_cwndVptr, pThis, sizeof s_cwndVptr);
    S_ToolbarsListCheckBox* d = D(pThis);
    d->m_cyText = 0;
    d->m_nStyle = 0;
    d->m_arCheckData.vptr = nullptr;
    d->m_arCheckData.m_pData = nullptr;
    d->m_arCheckData.m_nGrowBy = 0;
    d->m_arCheckData.m_nMaxSize = 0;
    d->m_arCheckData.m_nSize = 0;
    return pThis;
}
// Symbol: ??1CMFCToolBarsListCheckBox@@UEAA@XZ
// Retail 0x17b280 (mfc140u):
//   vptr = CMFCToolBarsListCheckBox vftable;
//   m_arCheckData.~CArray()   (mfc140u 0x14f60, no export: re-stamps the CArray
//                              vftable, then frees m_pData if non-NULL through
//                              the CRT free import 0x1802c74e8);
//   tail-jump ??1CListBox@@UEAA@XZ (0x293f30).
// Deviation (1): the re-stamped vptr is the one the CWnd ctor thunk installed.
extern "C" void MS_ABI impl___1CMFCToolBarsListCheckBox__UEAA_XZ(void* pThis) {
    if (s_cwndVptr != nullptr)
        std::memcpy(pThis, &s_cwndVptr, sizeof s_cwndVptr);
    S_ToolbarsListCheckBox* d = D(pThis);
    if (d->m_arCheckData.m_pData != nullptr)
        impl___3_YAXPEAX_Z(d->m_arCheckData.m_pData);
    impl___1CListBox__UEAA_XZ(static_cast<CListBox*>(Wnd(pThis)));
}

// Symbol: ?EnableCheck@CMFCToolBarsListCheckBox@@QEAAXHH@Z
// Retail 0x17b480 (mfc140u): m_arCheckData[nIndex] = bEnable, with
// operator[]'s bounds check (out of range -> AfxThrowInvalidArgException,
// 0x227720).  See deviation (4) on when the array is sized.
extern "C" void MS_ABI impl__EnableCheck_CMFCToolBarsListCheckBox__QEAAXHH_Z(void* pThis, int nIndex, int bEnable) {
    int* p = BoolArrayAt(&D(pThis)->m_arCheckData, nIndex);
    if (p != nullptr) *p = bEnable;
}

// Symbol: ?IsCheckEnabled@CMFCToolBarsListCheckBox@@QEBAHH@Z
// Retail 0x17b4b0 (mfc140u): return m_arCheckData[nIndex], bounds-checked as
// in EnableCheck.
extern "C" int MS_ABI impl__IsCheckEnabled_CMFCToolBarsListCheckBox__QEBAHH_Z(const void* pThis, int nIndex) {
    const int* p = BoolArrayAt(&D(pThis)->m_arCheckData, nIndex);
    return p != nullptr ? *p : 0;
}

// Symbol: ?OnKeyDown@CMFCToolBarsListCheckBox@@IEAAXIII@Z
// Retail 0x17b330 (mfc140u; ANSI 0x179950):
//   if (nChar == VK_SPACE) {
//       int iIndex = (int)::SendMessage(m_hWnd, LB_GETCARETINDEX, 0, 0);
//       if (iIndex != LB_ERR && !IsCheckEnabled(iIndex)) { ::MessageBeep((UINT)-1); return; }
//   }
//   CCheckListBox::OnKeyDown(nChar, nRepCnt, nFlags);
extern "C" void MS_ABI impl__OnKeyDown_CMFCToolBarsListCheckBox__IEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    if (nChar == VK_SPACE) {
        int iIndex = static_cast<int>(::SendMessage(Hwnd(pThis), LB_GETCARETINDEX, 0, 0));
        if (iIndex != LB_ERR &&
            !impl__IsCheckEnabled_CMFCToolBarsListCheckBox__QEBAHH_Z(pThis, iIndex)) {
            ::MessageBeep(static_cast<UINT>(-1));
            return;
        }
    }
    impl__OnKeyDown_CCheckListBox__IEAAXIII_Z(ChkList(pThis), nChar, nRepCnt, nFlags);
}

// Symbol: ?OnLBAddString@CMFCToolBarsListCheckBox@@IEAA_J_K_J@Z
// Retail 0x17b3a0 (mfc140u; ANSI 0x1799c0):
//   LRESULT lRes = CCheckListBox::OnLBAddString(wParam, lParam);
//   OnNewString((int)lRes);  return lRes;
extern "C" __int64 MS_ABI impl__OnLBAddString_CMFCToolBarsListCheckBox__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    LRESULT lRes = impl__OnLBAddString_CCheckListBox__IEAA_J_K_J_Z(ChkList(pThis), wParam, lParam);
    impl__OnNewString_CMFCToolBarsListCheckBox__IEAAXH_Z(pThis, static_cast<int>(lRes));
    return lRes;
}

// Symbol: ?OnLBDeleteString@CMFCToolBarsListCheckBox@@IEAA_J_K_J@Z
// Retail 0x17b400 (mfc140u; ANSI 0x179a20):
//   LRESULT lRes = Default();
//   if (lRes != LB_ERR) m_arCheckData.RemoveAt((INT_PTR)wParam, 1);
//   return lRes;
extern "C" __int64 MS_ABI impl__OnLBDeleteString_CMFCToolBarsListCheckBox__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)lParam;   // retail passes only `this` to Default()
    __int64 lRes = impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
    if (lRes != -1)
        BoolArrayRemoveAt(&D(pThis)->m_arCheckData, static_cast<std::ptrdiff_t>(wParam), 1);
    return lRes;
}

// Symbol: ?OnLBInsertString@CMFCToolBarsListCheckBox@@IEAA_J_K_J@Z
// Retail 0x17b3d0 (mfc140u; ANSI 0x1799f0): as OnLBAddString, through
// CCheckListBox::OnLBInsertString.
extern "C" __int64 MS_ABI impl__OnLBInsertString_CMFCToolBarsListCheckBox__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    LRESULT lRes = impl__OnLBInsertString_CCheckListBox__IEAA_J_K_J_Z(ChkList(pThis), wParam, lParam);
    impl__OnNewString_CMFCToolBarsListCheckBox__IEAAXH_Z(pThis, static_cast<int>(lRes));
    return lRes;
}

// Symbol: ?OnLBResetContent@CMFCToolBarsListCheckBox@@IEAA_J_K_J@Z
// Retail 0x17b450 (mfc140u; ANSI 0x179a70):
//   m_arCheckData.SetSize(0);  return Default();   (tail jump)
extern "C" __int64 MS_ABI impl__OnLBResetContent_CMFCToolBarsListCheckBox__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    (void)wParam;
    (void)lParam;
    BoolArraySetSize(&D(pThis)->m_arCheckData, 0);
    return impl__Default_CWnd__IEAA_JXZ(Wnd(pThis));
}

// Symbol: ?OnLButtonDblClk@CMFCToolBarsListCheckBox@@IEAAXIVCPoint@@@Z
// Retail 0x17b5a0 (mfc140u; ANSI 0x179bc0):
//   BOOL bInCheck;  int iIndex = CheckFromPoint(point, bInCheck);
//   if (bInCheck && iIndex != -1 && !IsCheckEnabled(iIndex)) { ::MessageBeep((UINT)-1); return; }
//   CCheckListBox::OnLButtonDblClk(nFlags, point);
//   CWnd* pParent = CWnd::FromHandle(::GetParent(m_hWnd));
//   ::SendMessage(pParent->m_hWnd, WM_COMMAND,
//                 MAKEWPARAM(GetDlgCtrlID(), CLBN_CHKCHANGE), (LPARAM)m_hWnd);
// Deviation (5): retail dereferences pParent unchecked; a NULL parent skips
// the notification here.
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCToolBarsListCheckBox__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, CPoint point) {
    int bInCheck = FALSE;
    int iIndex = impl__CheckFromPoint_CCheckListBox__IEAAHVCPoint__AEAH_Z(ChkList(pThis), point, &bInCheck);
    if (bInCheck && iIndex != -1 &&
        !impl__IsCheckEnabled_CMFCToolBarsListCheckBox__QEBAHH_Z(pThis, iIndex)) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }
    impl__OnLButtonDblClk_CCheckListBox__IEAAXIVCPoint___Z(ChkList(pThis), nFlags, point);

    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(::GetParent(Hwnd(pThis)));
    HWND hWndSelf = Hwnd(pThis);
    int nID = impl__GetDlgCtrlID_CWnd__QEBAHXZ(Wnd(pThis));
    if (pParent == nullptr) return;
    ::SendMessage(pParent->m_hWnd, WM_COMMAND,
                  MAKEWPARAM(static_cast<WORD>(nID), kClbnChkChange),
                  reinterpret_cast<LPARAM>(hWndSelf));
}

// Symbol: ?OnLButtonDown@CMFCToolBarsListCheckBox@@IEAAXIVCPoint@@@Z
// Retail 0x17b2c0 (mfc140u; ANSI 0x1798e0):
//   SetFocus();
//   BOOL bInCheck;  int iIndex = CheckFromPoint(point, bInCheck);
//   if (bInCheck && iIndex != -1 && !IsCheckEnabled(iIndex)) { ::MessageBeep((UINT)-1); return; }
//   CCheckListBox::OnLButtonDown(nFlags, point);
extern "C" void MS_ABI impl__OnLButtonDown_CMFCToolBarsListCheckBox__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, CPoint point) {
    impl__SetFocus_CWnd__QEAAPEAV1_XZ(Wnd(pThis));
    int bInCheck = FALSE;
    int iIndex = impl__CheckFromPoint_CCheckListBox__IEAAHVCPoint__AEAH_Z(ChkList(pThis), point, &bInCheck);
    if (bInCheck && iIndex != -1 &&
        !impl__IsCheckEnabled_CMFCToolBarsListCheckBox__QEBAHH_Z(pThis, iIndex)) {
        ::MessageBeep(static_cast<UINT>(-1));
        return;
    }
    impl__OnLButtonDown_CCheckListBox__IEAAXIVCPoint___Z(ChkList(pThis), nFlags, point);
}

// Symbol: ?OnNewString@CMFCToolBarsListCheckBox@@IEAAXH@Z
// Retail 0x17b4e0 (mfc140u; ANSI 0x179b00):
//   if (iIndex < 0) return;
//   int nCount = (int)::SendMessage(m_hWnd, LB_GETCOUNT, 0, 0);
//   m_arCheckData.SetSize(nCount);
//   for (int i = nCount - 1; i > iIndex; i--)
//       m_arCheckData[i] = m_arCheckData[i - 1];      // both bounds-checked
//   m_arCheckData[iIndex] = TRUE;                     // bounds-checked
extern "C" void MS_ABI impl__OnNewString_CMFCToolBarsListCheckBox__IEAAXH_Z(void* pThis, int iIndex) {
    if (iIndex < 0) return;
    S_BoolArray* a = &D(pThis)->m_arCheckData;
    int nCount = static_cast<int>(::SendMessage(Hwnd(pThis), LB_GETCOUNT, 0, 0));
    BoolArraySetSize(a, nCount);
    for (int i = nCount - 1; i > iIndex; i--) {
        int* pSrc = BoolArrayAt(a, i - 1);
        int* pDst = BoolArrayAt(a, i);
        if (pSrc == nullptr || pDst == nullptr) return;
        *pDst = *pSrc;
    }
    int* p = BoolArrayAt(a, iIndex);
    if (p != nullptr) *p = TRUE;
}
