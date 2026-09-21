// CMFCMaskedEdit — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <cwctype>
#include <new>

// ===========================================================================
// CMFCMaskedEdit -- the feature-pack masked edit control.
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (the method described in the header of core/ole/COleControl.cpp); each one
// names the RVA of the function ENTRY it was read from, and every Win32 import
// named here was resolved from its IAT slot (SendMessageW, GetKeyState,
// MessageBeep, SetWindowTextW, wcschr, wcscmp, wcslen, memcpy, iswdigit/alpha/
// alnum/space/print, towupper/towlower).  Thirteen of the 30 exports in this
// file are not in the mfc140u RVA map used by disas.py --u (SetValidChars, SetValue,
// SetWindowTextW, OnKeyDown, OnLButtonUp, OnSetFocusR, OnUpdateR, OnCut,
// OnPaste, OnSetText, OnGetText, OnGetTextLength, OnInitControl); their
// entries were recovered from the export table by ordinal (ordrva.py) and
// cross-checked against the class message map at 0x2ed158 (mfc140u), which is
// how OnCut and OnClear turned out to share one body (see ?OnCut below).
//
// Object layout.  CMFCMaskedEdit is not declared in the OpenMFC public headers,
// so the client allocates it with the retail layout and this file is the only
// place that layout lives.  It is CEdit (which adds nothing to CWnd,
// include/openmfc/afxwin.h:2581; sizeof(CWnd) == 0xe8 is pinned there) plus
// the members of the shipping afxmaskededit.h (read on this host at
// atlmfc/include/afxmaskededit.h -- the names and their order below are quoted
// from it), and every offset was read back out of the retail constructor
// ??0CMFCMaskedEdit@@QEAA@XZ (RVA 0x78d70, mfc140u): it calls ??0CWnd@@QEAA@XZ
// (0x28a700), stores its vftable, constructs four CStrings from the string
// manager's nil string at +0xe8/+0xf0/+0xf8/+0x108, and stores
// `movq $1,0x110(%rbx)` / `movq $1,0x118(%rbx)` / `xor %ecx,%ecx; mov
// %rcx,0x120(%rbx)` -- three 8-byte stores, i.e. the six BOOLs
// +0x110..+0x124 = 1,0,1,0,0,0.  +0x100 is the TCHAR EnableMask (0x78f60)
// stores its 4th argument to.  The scalar deleting destructor (??_G, entry
// 0x78e40, not exported) calls ??1 and then hands `mov $0x128,%edx` (the
// instruction at 0x78e6b) to its sized-delete path, which pins sizeof.
//
// Deviations from retail, applied uniformly and marked at each site:
//
//  (1) vtable / IsMaskedChar.  Retail's constructor stores the CMFCMaskedEdit
//      vftable and CheckChar (0x79e10), SetValue (0x79180), OnCharBackspace
//      (0x7a5a0) and OnCharDelete (0x7abf0) call IsMaskedChar VIRTUALLY through
//      slot 0x2d8/8 = 91 of it.  This DLL's constructor runs CWnd's placement
//      constructor (core/window/CtorDtorPlacement.cpp), which leaves OpenMFC's
//      own (mingw-layout) CWnd vtable in the object; slot 91 of that is not
//      IsMaskedChar, so those four bodies call the ?IsMaskedChar thunk in this
//      file DIRECTLY.  A client subclass overriding IsMaskedChar is therefore
//      NOT honoured -- that is the one behavioural gap in this file.
//
//  (2) ATL bounds checks.  Retail's inlined CSimpleStringT::operator[] /
//      SetAt / GetAt throw E_INVALIDARG (AtlThrow -> the shim at 0x333c ->
//      AfxThrowOleException) when an index is out of range.  Those checks are
//      reproduced (me_at / me_SetAt below) and routed to the exported
//      ?AfxThrowOleException@@YAXJ@Z thunk, as core/db/CFieldExchange.cpp does.
//      The ENSURE(psz != NULL) inside CStringT::Compare (the two
//      AtlThrow(E_FAIL) stubs at 0x7b698/0x7b6a3 inside OnUpdateR, entry
//      0x7b540) can never fire on a CString's own m_pszData and is not
//      reproduced.
//
//  (3) CString helpers.  OpenMFC's CString (include/openmfc/afxstr.h) has
//      SetAt (afxstr.h:467, but it silently IGNORES an out-of-range index
//      where ATL throws), Mid/Right/Left and operator+ (afxstr.h:655), and
//      lacks AppendChar and the (ch, nRepeat) constructor.  The me_* helpers
//      below exist so that the ATL bounds throws of deviation (2) and the two
//      missing members are reproduced over GetBuffer/ReleaseBuffer; me_Mid
//      mirrors the retail CStringT::Mid at 0x12a80, whose clamp order was
//      read.  CString::Right and operator+= are used as they are (same clamps
//      as ATL for the arguments that reach them).
// ===========================================================================

// Thunks defined elsewhere in the tree (every one checked to exist as a
// definition, not just a declaration):
extern "C" void* MS_ABI impl___0CWnd__QEAA_XZ(void* pThis);                                  // core/window/CtorDtorPlacement.cpp
extern "C" void  MS_ABI impl___1CEdit__UEAA_XZ(CEdit* pThis);                                // core/controls/RuntimeClasses.cpp
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                          // core/window/Thunks.cpp
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);              // core/window/Thunks.cpp
extern "C" void  MS_ABI impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(CWnd* pThis, const wchar_t* lpszString);                        // core/window/CWnd.cpp
extern "C" int   MS_ABI impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(const CWnd* pThis, wchar_t* lpszStringBuf, int nMaxCount);     // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const CWnd* pThis, CString* rString); // core/window/CWnd.cpp
extern "C" void  MS_ABI impl__AfxThrowOleException__YAXJ_Z(LONG sc);                          // detail/MfcExceptionsSupport.cpp
extern "C" void  MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();                           // detail/MfcExceptionsSupport.cpp
extern "C" int   MS_ABI impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const char* pszUTF8, CString* pStrResult, int cbUTF8); // controls/CMFCControlContainer.cpp
extern "C" void  MS_ABI impl___0CTagManager__QEAA_PEB_W_Z(void* pThis, const wchar_t* lpszBuffer);   // controls/CTagManager.cpp
extern "C" void  MS_ABI impl___1CTagManager__UEAA_XZ(void* pThis);                                  // controls/CTagManager.cpp
extern "C" int   MS_ABI impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(void* pThis, const wchar_t* lpszTag, CString* pStrValue, int bIsCharsList); // controls/CTagManager.cpp
extern "C" int   MS_ABI impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(void* pTagManager, const wchar_t* lpszTag, int* pValue);   // featurepack/CMFC_misc_stubs.cpp

// Thunks defined further down this file that earlier bodies call.
extern "C" void  MS_ABI impl__DisableMask_CMFCMaskedEdit__QEAAXXZ(void* pThis);
extern "C" void  MS_ABI impl__EnableMask_CMFCMaskedEdit__QEAAXPEB_W0_W0_Z(void* pThis, const wchar_t* lpszMask, const wchar_t* lpszInputTemplate, wchar_t chMaskInputTemplate, const wchar_t* lpszValid);
extern "C" void  MS_ABI impl__SetValidChars_CMFCMaskedEdit__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszValid);
extern "C" int   MS_ABI impl__IsMaskedChar_CMFCMaskedEdit__MEBAH_W0_Z(const void* pThis, wchar_t chChar, wchar_t chMaskChar);
extern "C" int   MS_ABI impl__CheckChar_CMFCMaskedEdit__AEAAH_WH_Z(void* pThis, wchar_t chChar, int nPos);
extern "C" int   MS_ABI impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(void* pThis, const wchar_t* lpszString, int bWithDelimiters);
extern "C" CString* MS_ABI impl__GetMaskedValue_CMFCMaskedEdit__IEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const void* pThis, CString* pResult, int bWithSpaces);
extern "C" void  MS_ABI impl__GetGroupBounds_CMFCMaskedEdit__AEAAXAEAH0HH_Z(void* pThis, int* pnBegin, int* pnEnd, int nStartPos, int bForward);
extern "C" int   MS_ABI impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(void* pThis, int bRestoreLastGood, int nBeginOld, int nEndOld);
extern "C" void  MS_ABI impl__OnCharPrintchar_CMFCMaskedEdit__AEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
extern "C" void  MS_ABI impl__OnCharBackspace_CMFCMaskedEdit__AEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
extern "C" void  MS_ABI impl__OnCharDelete_CMFCMaskedEdit__AEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags);
extern "C" void  MS_ABI impl__OnClear_CMFCMaskedEdit__IEAAXXZ(void* pThis);

namespace {

static_assert(sizeof(CString) == 8, "CString must be a single m_pszData pointer");
static_assert(sizeof(CWnd) == 0xe8, "CEdit adds nothing to CWnd; the retail tail starts at +0xe8");
static_assert(offsetof(CWnd, m_hWnd) == 0x40, "retail reads m_hWnd at +0x40");

// ---- layout view -----------------------------------------------------------
// Offsets read from ??0CMFCMaskedEdit (0x78d70) / ??1CMFCMaskedEdit (0x78e90)
// / EnableMask (0x78f60); names and order from the shipping afxmaskededit.h.
struct ME_Layout {
    unsigned char _cwnd[0xe8];      // CWnd / CEdit
    CString m_str;                  // 0xe8   the control's value (initial value)
    CString m_strMask;              // 0xf0   the mask string
    CString m_strInputTemplate;     // 0xf8   '_' marks a character entry
    wchar_t m_chMaskInputTemplate;  // 0x100  default char for empty entries
    CString m_strValid;             // 0x108  valid characters (empty = all)
    int     m_bGetMaskedCharsOnly;  // 0x110  ctor: TRUE
    int     m_bSetMaskedCharsOnly;  // 0x114  ctor: FALSE
    int     m_bSelectByGroup;       // 0x118  ctor: TRUE
    int     m_bMaskKeyInProgress;   // 0x11c  ctor: FALSE (no retail body reads or writes it)
    int     m_bPasteProcessing;     // 0x120  ctor: FALSE
    int     m_bSetTextProcessing;   // 0x124  ctor: FALSE
};
static_assert(offsetof(ME_Layout, m_str) == 0xe8, "CMFCMaskedEdit::m_str");
static_assert(offsetof(ME_Layout, m_strMask) == 0xf0, "CMFCMaskedEdit::m_strMask");
static_assert(offsetof(ME_Layout, m_strInputTemplate) == 0xf8, "CMFCMaskedEdit::m_strInputTemplate");
static_assert(offsetof(ME_Layout, m_chMaskInputTemplate) == 0x100, "CMFCMaskedEdit::m_chMaskInputTemplate");
static_assert(offsetof(ME_Layout, m_strValid) == 0x108, "CMFCMaskedEdit::m_strValid");
static_assert(offsetof(ME_Layout, m_bGetMaskedCharsOnly) == 0x110, "CMFCMaskedEdit::m_bGetMaskedCharsOnly");
static_assert(offsetof(ME_Layout, m_bSetMaskedCharsOnly) == 0x114, "CMFCMaskedEdit::m_bSetMaskedCharsOnly");
static_assert(offsetof(ME_Layout, m_bSelectByGroup) == 0x118, "CMFCMaskedEdit::m_bSelectByGroup");
static_assert(offsetof(ME_Layout, m_bMaskKeyInProgress) == 0x11c, "CMFCMaskedEdit::m_bMaskKeyInProgress");
static_assert(offsetof(ME_Layout, m_bPasteProcessing) == 0x120, "CMFCMaskedEdit::m_bPasteProcessing");
static_assert(offsetof(ME_Layout, m_bSetTextProcessing) == 0x124, "CMFCMaskedEdit::m_bSetTextProcessing");
static_assert(sizeof(ME_Layout) == 0x128, "CMFCMaskedEdit size (operator delete size in the ??_G at 0x78e40)");

inline ME_Layout*       ME(void* p)        { return static_cast<ME_Layout*>(p); }
inline const ME_Layout* ME(const void* p)  { return static_cast<const ME_Layout*>(p); }
inline CWnd*            WND(void* p)       { return static_cast<CWnd*>(p); }
inline const CWnd*      WND(const void* p) { return static_cast<const CWnd*>(p); }
inline HWND             HWNDOF(const void* p) { return WND(p)->m_hWnd; }

// ---- ATL string semantics ---------------------------------------------------
// Retail's inlined CSimpleStringT::operator[] allows 0 <= i <= GetLength()
// (the terminator is readable) and its SetAt allows 0 <= i < GetLength();
// both throw E_INVALIDARG otherwise (deviation (2) above).
[[noreturn]] void me_throw_invalidarg() {
    impl__AfxThrowOleException__YAXJ_Z(E_INVALIDARG);
    std::abort();   // the thunk throws; never reached
}
inline wchar_t me_at(const CString& s, int i) {
    if (i < 0 || i > s.GetLength()) me_throw_invalidarg();
    return s.GetString()[i];
}
void me_SetAt(CString& s, int i, wchar_t ch) {
    const int n = s.GetLength();
    if (i < 0 || i >= n) me_throw_invalidarg();
    wchar_t* p = s.GetBuffer(n);
    p[i] = ch;
    s.ReleaseBuffer(n);
}
void me_AppendChar(CString& s, wchar_t ch) {
    const int n = s.GetLength();
    wchar_t* p = s.GetBuffer(n + 1);
    p[n] = ch;
    s.ReleaseBuffer(n + 1);
}
// CStringT(XCHAR ch, int nLength): nLength <= 0 yields the empty string
// (retail ctor 0x7c260: `test r14d; jle` straight to the nil string).
CString me_Repeat(wchar_t ch, int nLength) {
    CString r;
    if (nLength > 0) {
        wchar_t* p = r.GetBuffer(nLength);
        for (int i = 0; i < nLength; ++i) p[i] = ch;
        r.ReleaseBuffer(nLength);
    }
    return r;
}
// CStringT::Mid(iFirst, nCount) with the retail clamp order (0x12a80):
// negatives to 0 first, then clip to the string, then the whole-string case.
CString me_Mid(const CString& s, int iFirst, int nCount) {
    const int len = s.GetLength();
    if (iFirst < 0) iFirst = 0;
    if (nCount < 0) nCount = 0;
    if (iFirst + nCount > len) nCount = len - iFirst;
    if (iFirst > len) nCount = 0;
    if (iFirst == 0 && nCount == len) return s;
    CString r;
    if (nCount > 0) {
        wchar_t* p = r.GetBuffer(nCount);
        std::wmemcpy(p, s.GetString() + iFirst, static_cast<size_t>(nCount));
        r.ReleaseBuffer(nCount);
    }
    return r;
}
// CStringT::Find(XCHAR ch, 0) as retail inlines it everywhere in this class:
// an empty string yields -1, otherwise wcschr.
inline int me_Find(const CString& s, wchar_t ch) {
    if (s.GetLength() <= 0) return -1;
    const wchar_t* p = std::wcschr(s.GetString(), ch);
    return p ? static_cast<int>(p - s.GetString()) : -1;
}

// ---- edit-control plumbing ----------------------------------------------------
// Retail sends every one of these straight to m_hWnd (SendMessageW, IAT
// 0x2c7120), never through CEdit's inline wrappers' m_pCtrlSite branch.
inline void me_GetSel(const void* p, int& nStart, int& nEnd) {
    ::SendMessage(HWNDOF(p), EM_GETSEL, reinterpret_cast<WPARAM>(&nStart), reinterpret_cast<LPARAM>(&nEnd));
}
inline void me_SetSel(const void* p, int nStart, int nEnd) {
    ::SendMessage(HWNDOF(p), EM_SETSEL, static_cast<WPARAM>(static_cast<INT_PTR>(nStart)),
                  static_cast<LPARAM>(static_cast<INT_PTR>(nEnd)));
}
inline void me_ScrollCaret(const void* p) {
    ::SendMessage(HWNDOF(p), EM_SCROLLCARET, 0, 0);
}
inline void me_ReplaceSel(const void* p, const CString& str) {
    ::SendMessage(HWNDOF(p), EM_REPLACESEL, static_cast<WPARAM>(TRUE), reinterpret_cast<LPARAM>(str.GetString()));
}
inline void me_Beep() { ::MessageBeep(static_cast<UINT>(-1)); }
// Retail tests `GetKeyState(vk) & 0x80` (`test %al,%dil` with dil = 0x80),
// not the documented 0x8000 -- reproduced as written.
inline bool me_KeyDown(int vk) { return (::GetKeyState(vk) & 0x80) != 0; }
// CEdit's inherited handlers all reduce to CWnd::Default() (0x28ac80) in retail.
inline __int64 me_Default(void* p) { return impl__Default_CWnd__IEAA_JXZ(WND(p)); }
// Deviation (1): retail dispatches vtable slot 91; this DLL calls the thunk.
inline int me_IsMaskedChar(const void* p, wchar_t ch, wchar_t chMask) {
    return impl__IsMaskedChar_CMFCMaskedEdit__MEBAH_W0_Z(p, ch, chMask);
}
inline void me_GetGroupBounds(void* p, int& nBegin, int& nEnd, int nStartPos, int bForward) {
    impl__GetGroupBounds_CMFCMaskedEdit__AEAAXAEAH0HH_Z(p, &nBegin, &nEnd, nStartPos, bForward);
}
inline int me_min(int a, int b) { return a < b ? a : b; }
inline int me_max(int a, int b) { return a > b ? a : b; }

} // namespace

// Retail (RVA 0x78d70, mfc140u), fully transcribed:
//     CWnd::CWnd();                                   // 0x28a700 (CEdit has no ctor body)
//     vfptr = &CMFCMaskedEdit::`vftable'              // NOT reproduced -- deviation (1)
//     m_str = m_strMask = m_strInputTemplate = m_strValid = nil string
//     m_bGetMaskedCharsOnly = TRUE;  m_bSetMaskedCharsOnly = FALSE;
//     m_bSelectByGroup = TRUE;       m_bMaskKeyInProgress = FALSE;
//     m_bPasteProcessing = FALSE;    m_bSetTextProcessing = FALSE;
// m_chMaskInputTemplate (+0x100) is NOT written by the retail constructor; it
// is left alone here as well.
// Symbol: ??0CMFCMaskedEdit@@QEAA@XZ
extern "C" void* MS_ABI impl___0CMFCMaskedEdit__QEAA_XZ(void* pThis) {
    if (!pThis) return pThis;
    impl___0CWnd__QEAA_XZ(pThis);
    ME_Layout* self = ME(pThis);
    new (&self->m_str) CString();
    new (&self->m_strMask) CString();
    new (&self->m_strInputTemplate) CString();
    new (&self->m_strValid) CString();
    self->m_bGetMaskedCharsOnly = TRUE;
    self->m_bSetMaskedCharsOnly = FALSE;
    self->m_bSelectByGroup = TRUE;
    self->m_bMaskKeyInProgress = FALSE;
    self->m_bPasteProcessing = FALSE;
    self->m_bSetTextProcessing = FALSE;
    return pThis;
}

// Retail (RVA 0x78e90, mfc140u), fully transcribed:
//     vfptr = &CMFCMaskedEdit::`vftable'              // NOT reproduced -- deviation (1)
//     release m_strValid, m_strInputTemplate, m_strMask, m_str   (CStringData refcount)
//     CEdit::~CEdit();                                // tail-jump 0x294370
// The ??1CEdit thunk (core/controls/RuntimeClasses.cpp) runs `pThis->~CEdit()`,
// which is a virtual-destructor dispatch through whatever vptr the object
// carries -- OpenMFC's CWnd vtable for an object this file's constructor built.
// Retail makes that call non-virtually after re-pointing the vptr; the same
// pattern (impl___1CWnd on the base) is what core/frame/CMDIClientAreaWnd.cpp
// and docking/CPaneDivider.cpp already do.
// Symbol: ??1CMFCMaskedEdit@@UEAA@XZ
extern "C" void MS_ABI impl___1CMFCMaskedEdit__UEAA_XZ(void* pThis) {
    if (!pThis) return;
    ME_Layout* self = ME(pThis);
    self->m_strValid.~CString();
    self->m_strInputTemplate.~CString();
    self->m_strMask.~CString();
    self->m_str.~CString();
    impl___1CEdit__UEAA_XZ(static_cast<CEdit*>(pThis));
}

// Retail (RVA 0x78f60, mfc140u), fully transcribed:
//     if (lpszMask == NULL || lpszInputTemplate == NULL) AfxThrowInvalidArgException();   // 0x227720
//     m_strMask = lpszMask;   m_strInputTemplate = lpszInputTemplate;
//     m_chMaskInputTemplate = chMaskInputTemplate;
//     m_str = lpszInputTemplate;
//     if (lpszValid != NULL) m_strValid = lpszValid; else m_strValid.Empty();
// (The four assignments are CSimpleStringT::SetString(psz, wcslen(psz)) at
// 0x2e30; the NULL branch is ?Empty@ at 0x33b0.)  The stores happen in the
// order mask, template, char, m_str, valid -- reproduced.
// Symbol: ?EnableMask@CMFCMaskedEdit@@QEAAXPEB_W0_W0@Z
extern "C" void MS_ABI impl__EnableMask_CMFCMaskedEdit__QEAAXPEB_W0_W0_Z(void* pThis, const wchar_t* lpszMask, const wchar_t* lpszInputTemplate, wchar_t chMaskInputTemplate, const wchar_t* lpszValid) {
    if (!pThis) return;
    if (lpszMask == nullptr || lpszInputTemplate == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return;
    }
    ME_Layout* self = ME(pThis);
    self->m_strMask = lpszMask;
    self->m_strInputTemplate = lpszInputTemplate;
    self->m_chMaskInputTemplate = chMaskInputTemplate;
    self->m_str = lpszInputTemplate;
    if (lpszValid != nullptr) self->m_strValid = lpszValid;
    else self->m_strValid.Empty();
}

// Retail (RVA 0x79030, mfc140u), fully transcribed:
//     m_strMask.Empty();  m_strInputTemplate.Empty();     // 0x33b0 twice
// Symbol: ?DisableMask@CMFCMaskedEdit@@QEAAXXZ
extern "C" void MS_ABI impl__DisableMask_CMFCMaskedEdit__QEAAXXZ(void* pThis) {
    if (!pThis) return;
    ME(pThis)->m_strMask.Empty();
    ME(pThis)->m_strInputTemplate.Empty();
}

// Retail (RVA 0x79060, mfc140u; export-table entry, not in the RVA map), fully transcribed:
//     if (lpszValid != NULL) m_strValid = lpszValid; else m_strValid.Empty();
// Symbol: ?SetValidChars@CMFCMaskedEdit@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetValidChars_CMFCMaskedEdit__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszValid) {
    if (!pThis) return;
    if (lpszValid != nullptr) ME(pThis)->m_strValid = lpszValid;
    else ME(pThis)->m_strValid.Empty();
}

// Retail (RVA 0x790b0, mfc140u), fully transcribed.  A switch on chMaskChar:
//     '*': return iswprint(chChar) != 0;
//     '+': return chChar == '+' || chChar == '-' || iswspace(chChar);
//          (`lea -0x2b(%rdi),%eax; test %ax,$0xfffd` == 0  <=>  chChar in {0x2b, 0x2d})
//     'A': return iswalnum(chChar) != 0;      'a': return iswalnum(chChar) || iswspace(chChar);
//     'C': return iswalpha(chChar) != 0;      'c': return iswalpha(chChar) || iswspace(chChar);
//     'D': return iswdigit(chChar) != 0;      'd': return iswdigit(chChar) || iswspace(chChar);
//     default: return FALSE;
// Symbol: ?IsMaskedChar@CMFCMaskedEdit@@MEBAH_W0@Z
extern "C" int MS_ABI impl__IsMaskedChar_CMFCMaskedEdit__MEBAH_W0_Z(const void* pThis, wchar_t chChar, wchar_t chMaskChar) {
    (void)pThis;
    const wint_t ch = static_cast<wint_t>(static_cast<unsigned short>(chChar));
    switch (chMaskChar) {
    case L'*': return std::iswprint(ch) != 0;
    case L'+': return (chChar == L'+' || chChar == L'-') ? TRUE : (std::iswspace(ch) != 0);
    case L'A': return std::iswalnum(ch) != 0;
    case L'a': return (std::iswalnum(ch) != 0) ? TRUE : (std::iswspace(ch) != 0);
    case L'C': return std::iswalpha(ch) != 0;
    case L'c': return (std::iswalpha(ch) != 0) ? TRUE : (std::iswspace(ch) != 0);
    case L'D': return std::iswdigit(ch) != 0;
    case L'd': return (std::iswdigit(ch) != 0) ? TRUE : (std::iswspace(ch) != 0);
    default:   return FALSE;
    }
}

// Retail (RVA 0x79e10, mfc140u), fully transcribed:
//     if (m_strMask.IsEmpty()) {
//         if (m_strValid.IsEmpty()) return TRUE;
//         return m_strValid.Find(chChar) != -1;
//     }
//     if (m_strInputTemplate[nPos] != '_') return FALSE;              // operator[] bounds-checked
//     BOOL b = IsMaskedChar(chChar, m_strMask[nPos]);                  // vtable slot 91 -> deviation (1)
//     if (m_strValid.IsEmpty()) return b;
//     if (!b) return FALSE;
//     return m_strValid.Find(chChar) != -1;
// Symbol: ?CheckChar@CMFCMaskedEdit@@AEAAH_WH@Z
extern "C" int MS_ABI impl__CheckChar_CMFCMaskedEdit__AEAAH_WH_Z(void* pThis, wchar_t chChar, int nPos) {
    if (!pThis) return FALSE;
    ME_Layout* self = ME(pThis);
    if (self->m_strMask.IsEmpty()) {
        if (self->m_strValid.IsEmpty()) return TRUE;
        return me_Find(self->m_strValid, chChar) != -1;
    }
    if (me_at(self->m_strInputTemplate, nPos) != L'_') return FALSE;
    const int b = me_IsMaskedChar(pThis, chChar, me_at(self->m_strMask, nPos));
    if (self->m_strValid.IsEmpty()) return b;
    if (!b) return FALSE;
    return me_Find(self->m_strValid, chChar) != -1;
}

// Retail (RVA 0x79180, mfc140u; export-table entry, not in the RVA map), fully transcribed:
//     if (lpszString == NULL) AfxThrowInvalidArgException();
//     CString strSource(lpszString);
//     if (!m_strMask.IsEmpty()) {
//         if (bWithDelimiters) { if (strSource.GetLength() > m_strMask.GetLength()) return FALSE; }
//         else { int n = number of '_' in m_strInputTemplate; if (strSource.GetLength() > n) return FALSE; }
//     }
//     if (!m_strValid.IsEmpty()) {
//         for (int i = 0; i < strSource.GetLength(); i++) {
//             if (!m_strInputTemplate.IsEmpty()) {
//                 if (m_strInputTemplate[i] != '_') continue;              // bounds-checked
//                 if (strSource[i] == m_chMaskInputTemplate) continue;
//             }
//             if (m_strValid.Find(strSource[i]) == -1) return FALSE;
//         }
//     }
//     if (m_strMask.IsEmpty()) { m_str = strSource; return TRUE; }
//     CString strResult = m_strInputTemplate;
//     for (int i = 0; i < strResult.GetLength(); i++)
//         if (m_strInputTemplate[i] == '_') strResult.SetAt(i, m_chMaskInputTemplate);
//     int iSrc = 0, iDst = 0;
//     while (iSrc < strSource.GetLength() && iDst < m_strInputTemplate.GetLength()) {
//         if (m_strInputTemplate[iDst] == '_') {
//             TCHAR ch = strSource[iSrc];
//             if (ch != m_chMaskInputTemplate && !IsMaskedChar(ch, m_strMask[iDst])) return FALSE;   // slot 91
//             strResult.SetAt(iDst, ch);  iSrc++;  iDst++;
//         } else if (!bWithDelimiters) { iDst++; }
//         else { if (strSource[iSrc] != m_strInputTemplate[iDst]) return FALSE;  iSrc++;  iDst++; }
//     }
//     m_str = strResult;  return TRUE;
// Symbol: ?SetValue@CMFCMaskedEdit@@IEAAHPEB_WH@Z
extern "C" int MS_ABI impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(void* pThis, const wchar_t* lpszString, int bWithDelimiters) {
    if (!pThis) return FALSE;
    ME_Layout* self = ME(pThis);
    if (lpszString == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;
    }
    CString strSource(lpszString);
    if (!self->m_strMask.IsEmpty()) {
        if (bWithDelimiters) {
            if (strSource.GetLength() > self->m_strMask.GetLength()) return FALSE;
        } else {
            int nCount = 0;
            const int nTpl = self->m_strInputTemplate.GetLength();
            for (int i = 0; i < nTpl; ++i) {
                if (self->m_strInputTemplate.GetString()[i] == L'_') ++nCount;
            }
            if (strSource.GetLength() > nCount) return FALSE;
        }
    }
    if (!self->m_strValid.IsEmpty()) {
        for (int i = 0; i < strSource.GetLength(); ++i) {
            if (!self->m_strInputTemplate.IsEmpty()) {
                if (me_at(self->m_strInputTemplate, i) != L'_') continue;
                if (strSource.GetString()[i] == self->m_chMaskInputTemplate) continue;
            }
            if (me_Find(self->m_strValid, strSource.GetString()[i]) == -1) return FALSE;
        }
    }
    if (self->m_strMask.IsEmpty()) {
        self->m_str = strSource;
        return TRUE;
    }
    CString strResult = self->m_strInputTemplate;
    for (int i = 0; i < strResult.GetLength(); ++i) {
        if (me_at(self->m_strInputTemplate, i) == L'_') {
            me_SetAt(strResult, i, self->m_chMaskInputTemplate);
        }
    }
    int iSrc = 0, iDst = 0;
    while (iSrc < strSource.GetLength() && iDst < self->m_strInputTemplate.GetLength()) {
        const wchar_t chTpl = me_at(self->m_strInputTemplate, iDst);
        if (chTpl == L'_') {
            const wchar_t ch = me_at(strSource, iSrc);
            if (ch != self->m_chMaskInputTemplate &&
                !me_IsMaskedChar(pThis, ch, me_at(self->m_strMask, iDst))) {
                return FALSE;
            }
            me_SetAt(strResult, iDst, ch);
            ++iSrc;
            ++iDst;
        } else if (!bWithDelimiters) {
            ++iDst;
        } else {
            if (me_at(strSource, iSrc) != chTpl) return FALSE;
            ++iSrc;
            ++iDst;
        }
    }
    self->m_str = strResult;
    return TRUE;
}

// Retail (RVA 0x79590, mfc140u), fully transcribed.  Returns CString by value:
// this in RCX, the result slot in RDX, and retail returns the slot in RAX
// (`mov %rsi,%rax` at 0x79683), reproduced here.  The (this, slot) argument
// pair is the one this tree's CString-returning thunks already use (e.g.
// core/file/CFile.cpp GetFileName), although that thunk returns void.
//     if (m_strMask.IsEmpty()) return m_str;
//     CString strResult;
//     for (int i = 0; i < m_strInputTemplate.GetLength(); i++) {
//         if (m_strInputTemplate[i] == '_') {                          // bounds-checked
//             TCHAR ch = m_str[i];                                     // bounds-checked
//             if (ch != m_chMaskInputTemplate || bWithSpaces) strResult.AppendChar(ch);   // 0x32b0
//         }
//     }
//     return strResult;
// Symbol: ?GetMaskedValue@CMFCMaskedEdit@@IEBA?BV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" CString* MS_ABI impl__GetMaskedValue_CMFCMaskedEdit__IEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(const void* pThis, CString* pResult, int bWithSpaces) {
    if (!pResult) return pResult;
    if (!pThis) { new (pResult) CString(); return pResult; }
    const ME_Layout* self = ME(pThis);
    if (self->m_strMask.IsEmpty()) {
        new (pResult) CString(self->m_str);
        return pResult;
    }
    CString strResult;
    for (int i = 0; i < self->m_strInputTemplate.GetLength(); ++i) {
        if (me_at(self->m_strInputTemplate, i) == L'_') {
            const wchar_t ch = me_at(self->m_str, i);
            if (ch != self->m_chMaskInputTemplate || bWithSpaces) me_AppendChar(strResult, ch);
        }
    }
    new (pResult) CString(strResult);
    return pResult;
}

// Retail (RVA 0x796a0, mfc140u; export-table entry, not in the RVA map):
//     jmp CWnd::SetWindowText                        // 0x2a9790 (afxwin2.inl body: ::SetWindowTextW(m_hWnd, ..) unless m_pCtrlSite)
// i.e. CEdit::SetWindowText(lpszString), un-hiding the base overload.
// Symbol: ?SetWindowTextW@CMFCMaskedEdit@@QEAAXPEB_W@Z
extern "C" void MS_ABI impl__SetWindowTextW_CMFCMaskedEdit__QEAAXPEB_W_Z(void* pThis, const wchar_t* lpszString) {
    if (!pThis) return;
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), lpszString);
}

// Retail (RVA 0x796b0, mfc140u): jmp ?GetWindowTextW@CWnd@@QEBAHPEA_WH@Z (0x2a9810).
// Symbol: ?GetWindowTextW@CMFCMaskedEdit@@QEBAHPEA_WH@Z
extern "C" int MS_ABI impl__GetWindowTextW_CMFCMaskedEdit__QEBAHPEA_WH_Z(const void* pThis, wchar_t* lpszStringBuf, int nMaxCount) {
    if (!pThis) return 0;
    return impl__GetWindowTextW_CWnd__QEBAHPEA_WH_Z(WND(pThis), lpszStringBuf, nMaxCount);
}

// Retail (RVA 0x796c0, mfc140u): jmp ?GetWindowTextW@CWnd@@QEBAXAEAV?$CStringT@..@@@Z (0x28be00).
// Symbol: ?GetWindowTextW@CMFCMaskedEdit@@QEBAXAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetWindowTextW_CMFCMaskedEdit__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(const void* pThis, CString* rString) {
    if (!pThis) return;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(WND(pThis), rString);
}

// Retail (RVA 0x796d0, mfc140u), fully transcribed:
//     if ((int)CWnd::Default() == -1) return -1;     // 0x28ac80 = CWnd::Default, what the base OnCreate does
//     CWnd::SetWindowText(m_str);                    // 0x2a9790
//     return 0;
// Symbol: ?OnCreate@CMFCMaskedEdit@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCMaskedEdit__IEAAHPEAUtagCREATESTRUCTW___Z(void* pThis, CREATESTRUCTW* lpCreateStruct) {
    (void)lpCreateStruct;
    if (!pThis) return -1;
    if (static_cast<int>(me_Default(pThis)) == -1) return -1;
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), ME(pThis)->m_str.GetString());
    return 0;
}

// Retail (RVA 0x7b210, mfc140u), fully transcribed:
//     if (m_strInputTemplate.IsEmpty()) { nBegin = 0; nEnd = m_str.GetLength(); return; }
//     int i = nStartPos;   const int nLen = m_strInputTemplate.GetLength();
//     if (bForward) {
//         // rewind into the group containing i-1.  The retail loop is rotated so
//         // that after the first step it tests T[i] rather than T[i-1]
//         // (0x7b266: rax = i_old, then T[rax-1] with i already decremented);
//         // reproduced literally.  It lands either on 0 or on a non-'_' char,
//         // and the Find below then locates the group, so the result equals
//         // the canonical "while (i > 0 && T[i-1] == '_') i--".
//         if (i > 0 && T[i-1] == '_') { do { i--; } while (i > 0 && T[i] == '_'); }
//         if (i == nLen) goto notfound;
//         if (T[i] != '_') { i = m_strInputTemplate.Find('_', i); if (i == -1) goto notfound; }
//         nBegin = i;
//         while (i < nLen && T[i] == '_') i++;
//         nEnd = i;
//     } else {
//         while (i < m_str.GetLength() && T[i] == '_') i++;    // T[i] bounds-checked against the TEMPLATE
//         while (i > 0 && T[i-1] != '_') i--;
//         if (i == 0) goto notfound;
//         nEnd = i;
//         while (i > 0 && T[i-1] == '_') i--;
//         nBegin = i;
//     }
//     return;
// notfound: nBegin = -1; nEnd = 0;
// Symbol: ?GetGroupBounds@CMFCMaskedEdit@@AEAAXAEAH0HH@Z
extern "C" void MS_ABI impl__GetGroupBounds_CMFCMaskedEdit__AEAAXAEAH0HH_Z(void* pThis, int* pnBegin, int* pnEnd, int nStartPos, int bForward) {
    if (!pThis || !pnBegin || !pnEnd) return;
    ME_Layout* self = ME(pThis);
    const CString& T = self->m_strInputTemplate;
    const int nLen = T.GetLength();
    if (nLen == 0) {
        *pnBegin = 0;
        *pnEnd = self->m_str.GetLength();
        return;
    }
    int i = nStartPos;
    if (bForward) {
        if (i > 0 && me_at(T, i - 1) == L'_') {
            do { --i; } while (i > 0 && me_at(T, i) == L'_');
        }
        if (i == nLen) goto notfound;
        if (me_at(T, i) != L'_') {
            // CStringT::Find('_', i): iStart >= GetLength() -> -1, else wcschr.
            if (i >= nLen) goto notfound;
            const wchar_t* p = std::wcschr(T.GetString() + i, L'_');
            if (p == nullptr) goto notfound;
            i = static_cast<int>(p - T.GetString());
            if (i == -1) goto notfound;
        }
        *pnBegin = i;
        while (i < nLen && T.GetString()[i] == L'_') ++i;
        *pnEnd = i;
        return;
    } else {
        const int nStrLen = self->m_str.GetLength();
        while (i < nStrLen) {
            if (me_at(T, i) != L'_') break;
            ++i;
        }
        while (i > 0 && me_at(T, i - 1) != L'_') --i;
        if (i == 0) goto notfound;
        *pnEnd = i;
        while (i > 0 && me_at(T, i - 1) == L'_') --i;
        *pnBegin = i;
        return;
    }
notfound:
    *pnBegin = -1;
    *pnEnd = 0;
}

// Retail (RVA 0x7b3f0, mfc140u), fully transcribed:
//     if (m_bPasteProcessing) return FALSE;
//     m_bPasteProcessing = TRUE;
//     CString strNew;  CWnd::GetWindowText(strNew);                   // 0x28be00
//     BOOL bRet = SetValue(strNew, TRUE);
//     if (!bRet) {
//         MessageBeep((UINT)-1);
//         if (bRestoreLastGood) {
//             CWnd::SetWindowText(m_str);                              // 0x2a9790
//             if (nBeginOld != -1) { SetSel(nBeginOld, nEndOld); ScrollCaret(); }
//         }
//     }
//     m_bPasteProcessing = FALSE;
//     return bRet;
// Symbol: ?DoUpdate@CMFCMaskedEdit@@AEAAHHHH@Z
extern "C" int MS_ABI impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(void* pThis, int bRestoreLastGood, int nBeginOld, int nEndOld) {
    if (!pThis) return FALSE;
    ME_Layout* self = ME(pThis);
    if (self->m_bPasteProcessing) return FALSE;
    self->m_bPasteProcessing = TRUE;
    CString strNew;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(WND(pThis), &strNew);
    const int bRet = impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(pThis, strNew.GetString(), TRUE);
    if (!bRet) {
        me_Beep();
        if (bRestoreLastGood) {
            CString strOld(self->m_str);
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), strOld.GetString());
            if (nBeginOld != -1) {
                me_SetSel(pThis, nBeginOld, nEndOld);
                me_ScrollCaret(pThis);
            }
        }
    }
    self->m_bPasteProcessing = FALSE;
    return bRet;
}

// Retail (RVA 0x79d70, mfc140u), fully transcribed:
//     if (iswprint((WORD)nChar) && !(GetKeyState(VK_CONTROL) & 0x80)) { OnCharPrintchar(nChar, nRepCnt, nFlags); return; }
//     if ((nChar == VK_DELETE || nChar == VK_BACK) && !m_strMask.IsEmpty()) return;
//     int nBeginOld, nEndOld;  GetSel(nBeginOld, nEndOld);
//     CWnd::Default();                                                 // CEdit::OnChar
//     DoUpdate(TRUE, nBeginOld, nEndOld);
// Symbol: ?OnChar@CMFCMaskedEdit@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnChar_CMFCMaskedEdit__IEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    if (!pThis) return;
    ME_Layout* self = ME(pThis);
    if (std::iswprint(static_cast<wint_t>(static_cast<unsigned short>(nChar))) && !me_KeyDown(VK_CONTROL)) {
        impl__OnCharPrintchar_CMFCMaskedEdit__AEAAXIII_Z(pThis, nChar, nRepCnt, nFlags);
        return;
    }
    if ((nChar == VK_DELETE || nChar == VK_BACK) && !self->m_strMask.IsEmpty()) return;
    int nBeginOld = 0, nEndOld = 0;
    me_GetSel(pThis, nBeginOld, nEndOld);
    me_Default(pThis);
    impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, nBeginOld, nEndOld);
}

// Retail (RVA 0x79f00, mfc140u), fully transcribed:
//     DWORD dwStyle = CWnd::GetStyle();                                // 0x2a9690
//     if (dwStyle & ES_UPPERCASE) nChar = (WORD)towupper((WORD)nChar);
//     else if (dwStyle & ES_LOWERCASE) nChar = (WORD)towlower((WORD)nChar);
//     int nBeginOld, nEndOld;  GetSel(nBeginOld, nEndOld);
//     int nBegin, nEnd;  GetGroupBounds(nBegin, nEnd, nBeginOld, TRUE);
//     // retail tests the m_str length only when nBeginOld is negative (0x79f86: jns skips it)
//     if ((nBeginOld < 0 && nEndOld > m_str.GetLength()) || nBeginOld < nBegin || nBeginOld > nEnd
//         || nEndOld < nBegin || nEndOld > nEnd) {
//         MessageBeep(-1); SetSel(nBegin, nEnd); ScrollCaret(); return;      // 0x7a53d
//     }
//     if (nBeginOld == nEndOld) {                                       // caret, no selection
//         if (m_strMask.IsEmpty()) {                                    // 0x7a1f3
//             if (!CheckChar(nChar, nBeginOld)) { MessageBeep(-1); return; }
//             int b, e; GetSel(b, e); CWnd::Default(); DoUpdate(TRUE, b, e); return;   // 0x7a210
//         }
//         if (nEndOld == nEnd) {                                        // caret at the end of its group
//             if (nEndOld >= m_str.GetLength() - 1) { MessageBeep(-1); return; }
//             GetGroupBounds(nBegin, nEnd, nEndOld + 1, TRUE);          // next group
//             if (nBegin == -1 || nBegin <= nEndOld) { MessageBeep(-1); return; }
//             SetSel(nBegin, nBegin); ScrollCaret();
//             nBeginOld = nEndOld = nBegin;
//         }
//         if (!CheckChar(nChar, nBeginOld)) { MessageBeep(-1); return; }
//         SetSel(nBeginOld, nEndOld + 1); ScrollCaret();
//         ReplaceSel(CString((TCHAR)nChar, 1));                          // EM_REPLACESEL, TRUE
//         m_str.SetAt(nEndOld, (TCHAR)nChar);
//         SetSel(nEndOld + 1, nEndOld + 1); ScrollCaret();
//         GetSel(nBeginOld, nEndOld);
//         if (nEndOld == nEnd) {                                        // 0x7a18d: typed the last char of the group
//             if (nEndOld < m_str.GetLength() - 1) GetGroupBounds(nBegin, nEnd, nEndOld + 1, TRUE);
//             if (nBegin != -1 && nBegin > nEndOld) { SetSel(nBegin, nBegin); ScrollCaret(); }
//         }
//         return;
//     }
//     // selection [nBeginOld, nEndOld)                                  // 0x7a245
//     if (!CheckChar(nChar, nBeginOld)) { MessageBeep(-1); return; }
//     if (m_strInputTemplate.IsEmpty()) { int b, e; GetSel(b, e); CWnd::Default(); DoUpdate(TRUE, b, e); return; }
//     TCHAR chMask = m_strMask[nBeginOld];                               // bounds-checked
//     int nCount = 1;
//     while (nBeginOld + nCount < nEnd && m_strMask[nBeginOld + nCount] == chMask) nCount++;
//     if (nEndOld - nBeginOld > nCount) { MessageBeep(-1); SetSel(nBeginOld, nBeginOld + nCount); ScrollCaret(); return; }
//     CString strText = m_str.Mid(nBeginOld, nCount);                   // 0x12a80
//     if (nCount > 0) {
//         int nSel = nEndOld - nBeginOld;
//         strText = strText.Right(nCount - nSel + 1);                    // 0x3ade0
//         strText += CString(m_chMaskInputTemplate, nSel - 1);           // 0x7c260 + Append 0x2ba0
//         strText.SetAt(0, (TCHAR)nChar);
//     }
//     SetSel(nBeginOld, nBeginOld + nCount); ScrollCaret();
//     ReplaceSel(strText);
//     SetSel(nBeginOld, nBeginOld); ScrollCaret();
//     for (int i = 0; i < strText.GetLength(); i++) m_str.SetAt(nBeginOld + i, strText[i]);
//     SetSel(nBeginOld + 1, nBeginOld + 1); ScrollCaret();
// Symbol: ?OnCharPrintchar@CMFCMaskedEdit@@AEAAXIII@Z
extern "C" void MS_ABI impl__OnCharPrintchar_CMFCMaskedEdit__AEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nRepCnt; (void)nFlags;
    if (!pThis) return;
    ME_Layout* self = ME(pThis);

    const unsigned long dwStyle = impl__GetStyle_CWnd__QEBAKXZ(WND(pThis));
    if (dwStyle & ES_UPPERCASE) {
        nChar = static_cast<unsigned short>(std::towupper(static_cast<wint_t>(static_cast<unsigned short>(nChar))));
    } else if (dwStyle & ES_LOWERCASE) {
        nChar = static_cast<unsigned short>(std::towlower(static_cast<wint_t>(static_cast<unsigned short>(nChar))));
    }
    const wchar_t ch = static_cast<wchar_t>(static_cast<unsigned short>(nChar));

    int nBeginOld = 0, nEndOld = 0;
    me_GetSel(pThis, nBeginOld, nEndOld);
    int nBegin = 0, nEnd = 0;
    me_GetGroupBounds(pThis, nBegin, nEnd, nBeginOld, TRUE);

    if ((nBeginOld < 0 && nEndOld > self->m_str.GetLength()) ||
        nBeginOld < nBegin || nBeginOld > nEnd || nEndOld < nBegin || nEndOld > nEnd) {
        me_Beep();
        me_SetSel(pThis, nBegin, nEnd);
        me_ScrollCaret(pThis);
        return;
    }

    if (nBeginOld == nEndOld) {
        if (self->m_strMask.IsEmpty()) {
            if (!impl__CheckChar_CMFCMaskedEdit__AEAAH_WH_Z(pThis, ch, nBeginOld)) { me_Beep(); return; }
            int b = 0, e = 0;
            me_GetSel(pThis, b, e);
            me_Default(pThis);
            impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, b, e);
            return;
        }
        if (nEndOld == nEnd) {
            if (nEndOld >= self->m_str.GetLength() - 1) { me_Beep(); return; }
            me_GetGroupBounds(pThis, nBegin, nEnd, nEndOld + 1, TRUE);
            if (nBegin == -1 || nBegin <= nEndOld) { me_Beep(); return; }
            me_SetSel(pThis, nBegin, nBegin);
            me_ScrollCaret(pThis);
            nBeginOld = nBegin;
            nEndOld = nBegin;
        }
        if (!impl__CheckChar_CMFCMaskedEdit__AEAAH_WH_Z(pThis, ch, nBeginOld)) { me_Beep(); return; }
        me_SetSel(pThis, nBeginOld, nEndOld + 1);
        me_ScrollCaret(pThis);
        {
            CString strChar = me_Repeat(ch, 1);
            me_ReplaceSel(pThis, strChar);
        }
        me_SetAt(self->m_str, nEndOld, ch);
        me_SetSel(pThis, nEndOld + 1, nEndOld + 1);
        me_ScrollCaret(pThis);
        me_GetSel(pThis, nBeginOld, nEndOld);
        if (nEndOld == nEnd) {
            if (nEndOld < self->m_str.GetLength() - 1) {
                me_GetGroupBounds(pThis, nBegin, nEnd, nEndOld + 1, TRUE);
            }
            if (nBegin != -1 && nBegin > nEndOld) {
                me_SetSel(pThis, nBegin, nBegin);
                me_ScrollCaret(pThis);
            }
        }
        return;
    }

    if (!impl__CheckChar_CMFCMaskedEdit__AEAAH_WH_Z(pThis, ch, nBeginOld)) { me_Beep(); return; }
    if (self->m_strInputTemplate.IsEmpty()) {
        int b = 0, e = 0;
        me_GetSel(pThis, b, e);
        me_Default(pThis);
        impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, b, e);
        return;
    }
    const wchar_t chMask = me_at(self->m_strMask, nBeginOld);
    int nCount = 1;
    while (nBeginOld + nCount < nEnd && me_at(self->m_strMask, nBeginOld + nCount) == chMask) ++nCount;
    if (nEndOld - nBeginOld > nCount) {
        me_Beep();
        me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
        me_ScrollCaret(pThis);
        return;
    }
    CString strText = me_Mid(self->m_str, nBeginOld, nCount);
    if (nCount > 0) {
        const int nSel = nEndOld - nBeginOld;
        strText = strText.Right(nCount - nSel + 1);
        strText += me_Repeat(self->m_chMaskInputTemplate, nSel - 1);
        me_SetAt(strText, 0, ch);
    }
    me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
    me_ScrollCaret(pThis);
    me_ReplaceSel(pThis, strText);
    me_SetSel(pThis, nBeginOld, nBeginOld);
    me_ScrollCaret(pThis);
    for (int i = 0; i < strText.GetLength(); ++i) {
        me_SetAt(self->m_str, nBeginOld + i, me_at(strText, i));
    }
    me_SetSel(pThis, nBeginOld + 1, nBeginOld + 1);
    me_ScrollCaret(pThis);
}

// Retail (RVA 0x7a5a0, mfc140u), fully transcribed (nChar/nRepCnt/nFlags unused):
//     int nBeginOld, nEndOld;  GetSel(nBeginOld, nEndOld);
//     int nBegin, nEnd;  GetGroupBounds(nBegin, nEnd, nBeginOld, TRUE);
//     if ((nBeginOld < 0 && nEndOld > m_str.GetLength()) || nBeginOld < nBegin || nBeginOld > nEnd
//         || nEndOld < nBegin || nEndOld > nEnd) { MessageBeep(-1); SetSel(nBegin, nEnd); ScrollCaret(); return; }
//     if (nBeginOld == nEndOld) {                                       // caret
//         if (m_strMask.IsEmpty()) { int b, e; GetSel(b, e); CWnd::Default(); DoUpdate(TRUE, b, e); return; }
//         if (nEndOld == nBegin) {                                      // caret at the start of its group
//             if (nEndOld > 1) GetGroupBounds(nBegin, nEnd, nEndOld - 1, FALSE);   // previous group
//             if (nBegin == -1 || nEnd >= nEndOld) { MessageBeep(-1); return; }
//             SetSel(nEnd, nEnd); ScrollCaret(); return;
//         }
//         int nPos = nBeginOld - 1;
//         TCHAR chMask = m_strMask[nPos];                                // bounds-checked
//         int nCount = 1;
//         while (nPos + nCount < nEnd && m_strMask[nPos + nCount] == chMask) nCount++;
//         for (int i = nPos; i + nCount < nEnd; i++) {                   // the rest of the group must be valid
//             TCHAR c = m_str[i];                                        // bounds-checked
//             if (c != m_chMaskInputTemplate && !IsMaskedChar(c, m_strMask[i])) { MessageBeep(-1); return; }   // slot 91
//         }
//         CString strText = m_str.Mid(nPos, nCount);
//         if (nCount > 0) { strText = strText.Right(nCount - 1); strText.AppendChar(m_chMaskInputTemplate); }
//         SetSel(nPos, nPos + nCount); ScrollCaret();
//         ReplaceSel(strText);
//         SetSel(nPos, nPos); ScrollCaret();
//         for (int i = 0; i < strText.GetLength(); i++) m_str.SetAt(nPos + i, strText[i]);
//         return;
//     }
//     // selection                                                       // 0x7a924
//     if (m_strInputTemplate.IsEmpty()) { int b, e; GetSel(b, e); CWnd::Default(); DoUpdate(TRUE, b, e); return; }
//     TCHAR chMask = m_strMask[nBeginOld];
//     int nCount = 1;
//     while (nBeginOld + nCount < nEnd && m_strMask[nBeginOld + nCount] == chMask) nCount++;
//     if (nEndOld - nBeginOld > nCount) { MessageBeep(-1); SetSel(nBeginOld, nBeginOld + nCount); ScrollCaret(); return; }
//     CString strText = m_str.Mid(nBeginOld, nCount);
//     if (nCount > 0) { int nSel = nEndOld - nBeginOld; strText = strText.Right(nCount - nSel); strText += CString(m_chMaskInputTemplate, nSel); }
//     SetSel(nBeginOld, nBeginOld + nCount); ScrollCaret();
//     ReplaceSel(strText);
//     SetSel(nBeginOld, nBeginOld); ScrollCaret();
//     for (int i = 0; i < strText.GetLength(); i++) m_str.SetAt(nBeginOld + i, strText[i]);
// Symbol: ?OnCharBackspace@CMFCMaskedEdit@@AEAAXIII@Z
extern "C" void MS_ABI impl__OnCharBackspace_CMFCMaskedEdit__AEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nChar; (void)nRepCnt; (void)nFlags;
    if (!pThis) return;
    ME_Layout* self = ME(pThis);

    int nBeginOld = 0, nEndOld = 0;
    me_GetSel(pThis, nBeginOld, nEndOld);
    int nBegin = 0, nEnd = 0;
    me_GetGroupBounds(pThis, nBegin, nEnd, nBeginOld, TRUE);

    if ((nBeginOld < 0 && nEndOld > self->m_str.GetLength()) ||
        nBeginOld < nBegin || nBeginOld > nEnd || nEndOld < nBegin || nEndOld > nEnd) {
        me_Beep();
        me_SetSel(pThis, nBegin, nEnd);
        me_ScrollCaret(pThis);
        return;
    }

    if (nBeginOld == nEndOld) {
        if (self->m_strMask.IsEmpty()) {
            int b = 0, e = 0;
            me_GetSel(pThis, b, e);
            me_Default(pThis);
            impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, b, e);
            return;
        }
        if (nEndOld == nBegin) {
            if (nEndOld > 1) me_GetGroupBounds(pThis, nBegin, nEnd, nEndOld - 1, FALSE);
            if (nBegin == -1 || nEnd >= nEndOld) { me_Beep(); return; }
            me_SetSel(pThis, nEnd, nEnd);
            me_ScrollCaret(pThis);
            return;
        }
        const int nPos = nBeginOld - 1;
        const wchar_t chMask = me_at(self->m_strMask, nPos);
        int nCount = 1;
        while (nPos + nCount < nEnd && me_at(self->m_strMask, nPos + nCount) == chMask) ++nCount;
        for (int i = nPos; i + nCount < nEnd; ++i) {
            const wchar_t c = me_at(self->m_str, i);
            if (c != self->m_chMaskInputTemplate &&
                !me_IsMaskedChar(pThis, c, me_at(self->m_strMask, i))) {
                me_Beep();
                return;
            }
        }
        CString strText = me_Mid(self->m_str, nPos, nCount);
        if (nCount > 0) {
            strText = strText.Right(nCount - 1);
            me_AppendChar(strText, self->m_chMaskInputTemplate);
        }
        me_SetSel(pThis, nPos, nPos + nCount);
        me_ScrollCaret(pThis);
        me_ReplaceSel(pThis, strText);
        me_SetSel(pThis, nPos, nPos);
        me_ScrollCaret(pThis);
        for (int i = 0; i < strText.GetLength(); ++i) {
            me_SetAt(self->m_str, nPos + i, me_at(strText, i));
        }
        return;
    }

    if (self->m_strInputTemplate.IsEmpty()) {
        int b = 0, e = 0;
        me_GetSel(pThis, b, e);
        me_Default(pThis);
        impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, b, e);
        return;
    }
    const wchar_t chMask = me_at(self->m_strMask, nBeginOld);
    int nCount = 1;
    while (nBeginOld + nCount < nEnd && me_at(self->m_strMask, nBeginOld + nCount) == chMask) ++nCount;
    if (nEndOld - nBeginOld > nCount) {
        me_Beep();
        me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
        me_ScrollCaret(pThis);
        return;
    }
    CString strText = me_Mid(self->m_str, nBeginOld, nCount);
    if (nCount > 0) {
        const int nSel = nEndOld - nBeginOld;
        strText = strText.Right(nCount - nSel);
        strText += me_Repeat(self->m_chMaskInputTemplate, nSel);
    }
    me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
    me_ScrollCaret(pThis);
    me_ReplaceSel(pThis, strText);
    me_SetSel(pThis, nBeginOld, nBeginOld);
    me_ScrollCaret(pThis);
    for (int i = 0; i < strText.GetLength(); ++i) {
        me_SetAt(self->m_str, nBeginOld + i, me_at(strText, i));
    }
}

// Retail (RVA 0x7abf0, mfc140u), fully transcribed (nChar/nRepCnt/nFlags unused).
// Identical to OnCharBackspace except for the caret case, which works on the
// character AT the caret (nPos = nBeginOld) and beeps when the caret sits at the
// END of its group; there is no previous-group step.  Retail reads the
// selection twice (the second copy is the GetGroupBounds start position).
//     int nBeginOld, nEndOld;  GetSel(nBeginOld, nEndOld);
//     int nBegin, nEnd;        GetSel(nBegin, nEnd);
//     GetGroupBounds(nBegin, nEnd, nBegin, TRUE);
//     if ((nBeginOld < 0 && nEndOld > m_str.GetLength()) || nBeginOld < nBegin || nBeginOld > nEnd
//         || nEndOld < nBegin || nEndOld > nEnd) { MessageBeep(-1); SetSel(nBegin, nEnd); ScrollCaret(); return; }
//     if (nBeginOld == nEndOld) {
//         if (m_strMask.IsEmpty()) { int b, e; GetSel(b, e); CWnd::Default(); DoUpdate(TRUE, b, e); return; }
//         if (nEndOld == nEnd) { MessageBeep(-1); return; }
//         TCHAR chMask = m_strMask[nBeginOld];  int nCount = 1;
//         while (nBeginOld + nCount < nEnd && m_strMask[nBeginOld + nCount] == chMask) nCount++;
//         for (int i = nBeginOld; i + nCount < nEnd; i++) {
//             TCHAR c = m_str[i];
//             if (c != m_chMaskInputTemplate && !IsMaskedChar(c, m_strMask[i])) { MessageBeep(-1); return; }   // slot 91
//         }
//         CString strText = m_str.Mid(nBeginOld, nCount);
//         if (nCount > 0) { strText = strText.Right(nCount - 1); strText.AppendChar(m_chMaskInputTemplate); }
//         SetSel(nBeginOld, nBeginOld + nCount); ScrollCaret(); ReplaceSel(strText);
//         SetSel(nBeginOld, nBeginOld); ScrollCaret();
//         for (int i = 0; i < strText.GetLength(); i++) m_str.SetAt(nBeginOld + i, strText[i]);
//         return;
//     }
//     (selection case, from 0x7af3a: the same logic as OnCharBackspace's selection
//      case -- same calls, same order, same clamps; the machine code differs only
//      in register/stack-slot allocation)
// Symbol: ?OnCharDelete@CMFCMaskedEdit@@AEAAXIII@Z
extern "C" void MS_ABI impl__OnCharDelete_CMFCMaskedEdit__AEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    (void)nChar; (void)nRepCnt; (void)nFlags;
    if (!pThis) return;
    ME_Layout* self = ME(pThis);

    int nBeginOld = 0, nEndOld = 0;
    me_GetSel(pThis, nBeginOld, nEndOld);
    int nBegin = 0, nEnd = 0;
    me_GetSel(pThis, nBegin, nEnd);
    me_GetGroupBounds(pThis, nBegin, nEnd, nBegin, TRUE);

    if ((nBeginOld < 0 && nEndOld > self->m_str.GetLength()) ||
        nBeginOld < nBegin || nBeginOld > nEnd || nEndOld < nBegin || nEndOld > nEnd) {
        me_Beep();
        me_SetSel(pThis, nBegin, nEnd);
        me_ScrollCaret(pThis);
        return;
    }

    if (nBeginOld == nEndOld) {
        if (self->m_strMask.IsEmpty()) {
            int b = 0, e = 0;
            me_GetSel(pThis, b, e);
            me_Default(pThis);
            impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, b, e);
            return;
        }
        if (nEndOld == nEnd) { me_Beep(); return; }
        const wchar_t chMask = me_at(self->m_strMask, nBeginOld);
        int nCount = 1;
        while (nBeginOld + nCount < nEnd && me_at(self->m_strMask, nBeginOld + nCount) == chMask) ++nCount;
        for (int i = nBeginOld; i + nCount < nEnd; ++i) {
            const wchar_t c = me_at(self->m_str, i);
            if (c != self->m_chMaskInputTemplate &&
                !me_IsMaskedChar(pThis, c, me_at(self->m_strMask, i))) {
                me_Beep();
                return;
            }
        }
        CString strText = me_Mid(self->m_str, nBeginOld, nCount);
        if (nCount > 0) {
            strText = strText.Right(nCount - 1);
            me_AppendChar(strText, self->m_chMaskInputTemplate);
        }
        me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
        me_ScrollCaret(pThis);
        me_ReplaceSel(pThis, strText);
        me_SetSel(pThis, nBeginOld, nBeginOld);
        me_ScrollCaret(pThis);
        for (int i = 0; i < strText.GetLength(); ++i) {
            me_SetAt(self->m_str, nBeginOld + i, me_at(strText, i));
        }
        return;
    }

    if (self->m_strInputTemplate.IsEmpty()) {
        int b = 0, e = 0;
        me_GetSel(pThis, b, e);
        me_Default(pThis);
        impl__DoUpdate_CMFCMaskedEdit__AEAAHHHH_Z(pThis, TRUE, b, e);
        return;
    }
    const wchar_t chMask = me_at(self->m_strMask, nBeginOld);
    int nCount = 1;
    while (nBeginOld + nCount < nEnd && me_at(self->m_strMask, nBeginOld + nCount) == chMask) ++nCount;
    if (nEndOld - nBeginOld > nCount) {
        me_Beep();
        me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
        me_ScrollCaret(pThis);
        return;
    }
    CString strText = me_Mid(self->m_str, nBeginOld, nCount);
    if (nCount > 0) {
        const int nSel = nEndOld - nBeginOld;
        strText = strText.Right(nCount - nSel);
        strText += me_Repeat(self->m_chMaskInputTemplate, nSel);
    }
    me_SetSel(pThis, nBeginOld, nBeginOld + nCount);
    me_ScrollCaret(pThis);
    me_ReplaceSel(pThis, strText);
    me_SetSel(pThis, nBeginOld, nBeginOld);
    me_ScrollCaret(pThis);
    for (int i = 0; i < strText.GetLength(); ++i) {
        me_SetAt(self->m_str, nBeginOld + i, me_at(strText, i));
    }
}

// Retail (RVA 0x79700, mfc140u; export-table entry, WM_KEYDOWN in the message
// map), fully transcribed:
//     int nBegin, nEnd;  GetGroupBounds(nBegin, nEnd, 0, TRUE);
//     if (nBegin == -1) { MessageBeep(-1); return; }
//     switch (nChar) {
//     case VK_BACK:   OnCharBackspace(nChar, nRepCnt, nFlags); return;
//     case VK_DELETE: if (!(GetKeyState(VK_SHIFT) & 0x80)) { OnCharDelete(nChar, nRepCnt, nFlags); return; }  break;
//     case VK_INSERT: if ((GetKeyState(VK_CONTROL) & 0x80) || (GetKeyState(VK_SHIFT) & 0x80)) break;
//                     if (!m_strMask.IsEmpty()) return;   break;      // no overwrite-mode toggle under a mask
//     case VK_RIGHT: case VK_DOWN: {
//         int s, e;  GetSel(s, e);
//         GetGroupBounds(nBegin, nEnd, e, TRUE);
//         if (nBegin == -1) GetGroupBounds(nBegin, nEnd, m_str.GetLength(), FALSE);
//         if (GetKeyState(VK_SHIFT) & 0x80) {
//             GetSel(s, e);  int n = e + 1;
//             if (m_bSelectByGroup) n = max(nBegin, min(n, nEnd));
//             SetSel(s, n); ScrollCaret(); return;
//         }
//         if (GetKeyState(VK_CONTROL) & 0x80) {
//             GetSel(s, e);
//             if (e < m_str.GetLength() - 1) GetGroupBounds(nBegin, nEnd, e + 1, TRUE);
//             if (nBegin == -1 || (nBegin == s && nEnd == e)) { MessageBeep(-1); return; }
//             SetSel(nBegin, nEnd); ScrollCaret(); return;
//         }
//         GetSel(s, e);
//         if (s == e && e == nEnd) {                                    // caret at the end of its group
//             if (e < m_str.GetLength() - 1) GetGroupBounds(nBegin, nEnd, s + 1, TRUE);
//             if (nBegin == -1 || nBegin <= e) { MessageBeep(-1); return; }
//             SetSel(nBegin, nBegin); ScrollCaret(); return;
//         }
//         int n = max(nBegin, min(e + 1, nEnd));  SetSel(n, n); ScrollCaret(); return;
//     }
//     case VK_LEFT: case VK_UP: {
//         int s, e;  GetSel(s, e);
//         GetGroupBounds(nBegin, nEnd, s, FALSE);
//         if (nBegin == -1) GetGroupBounds(nBegin, nEnd, 0, TRUE);
//         if (GetKeyState(VK_SHIFT) & 0x80) {
//             GetSel(s, e);  int n = s - 1;
//             if (m_bSelectByGroup) n = min(nEnd, max(n, nBegin));
//             SetSel(n, e); ScrollCaret(); return;
//         }
//         if (GetKeyState(VK_CONTROL) & 0x80) {
//             GetSel(s, e);
//             if (s > 1) GetGroupBounds(nBegin, nEnd, s - 1, FALSE);
//             if (nBegin == -1 || (nBegin == s && nEnd == e)) { MessageBeep(-1); return; }
//             SetSel(nBegin, nEnd); ScrollCaret(); return;
//         }
//         GetSel(s, e);
//         if (s == e && s == nBegin) {                                  // caret at the start of its group
//             if (s > 1) GetGroupBounds(nBegin, nEnd, s - 1, FALSE);
//             if (nBegin == -1 || nEnd >= s) { MessageBeep(-1); return; }
//             SetSel(nEnd, nEnd); ScrollCaret(); return;
//         }
//         int n = min(nEnd, max(s - 1, nBegin));  SetSel(n, n); ScrollCaret(); return;
//     }
//     case VK_HOME: {                                                   // modifiers ignored
//         int s, e;  GetSel(s, e);
//         GetGroupBounds(nBegin, nEnd, s, FALSE);
//         if (nBegin == -1) GetGroupBounds(nBegin, nEnd, 0, TRUE);
//         SetSel(nBegin, nBegin); ScrollCaret(); return;
//     }
//     case VK_END: {                                                    // modifiers ignored
//         int s, e;  GetSel(s, e);
//         GetGroupBounds(nBegin, nEnd, e, TRUE);
//         if (nBegin == -1) GetGroupBounds(nBegin, nEnd, m_str.GetLength(), FALSE);
//         SetSel(nEnd, nEnd); ScrollCaret(); return;
//     }
//     }
//     CWnd::Default();                                                  // CEdit::OnKeyDown
// Symbol: ?OnKeyDown@CMFCMaskedEdit@@IEAAXIII@Z
extern "C" void MS_ABI impl__OnKeyDown_CMFCMaskedEdit__IEAAXIII_Z(void* pThis, unsigned int nChar, unsigned int nRepCnt, unsigned int nFlags) {
    if (!pThis) return;
    ME_Layout* self = ME(pThis);

    int nBegin = 0, nEnd = 0;
    me_GetGroupBounds(pThis, nBegin, nEnd, 0, TRUE);
    if (nBegin == -1) { me_Beep(); return; }

    switch (nChar) {
    case VK_BACK:
        impl__OnCharBackspace_CMFCMaskedEdit__AEAAXIII_Z(pThis, nChar, nRepCnt, nFlags);
        return;
    case VK_DELETE:
        if (!me_KeyDown(VK_SHIFT)) {
            impl__OnCharDelete_CMFCMaskedEdit__AEAAXIII_Z(pThis, nChar, nRepCnt, nFlags);
            return;
        }
        break;
    case VK_INSERT:
        if (me_KeyDown(VK_CONTROL) || me_KeyDown(VK_SHIFT)) break;
        if (!self->m_strMask.IsEmpty()) return;
        break;
    case VK_RIGHT:
    case VK_DOWN: {
        int s = 0, e = 0;
        me_GetSel(pThis, s, e);
        me_GetGroupBounds(pThis, nBegin, nEnd, e, TRUE);
        if (nBegin == -1) me_GetGroupBounds(pThis, nBegin, nEnd, self->m_str.GetLength(), FALSE);
        if (me_KeyDown(VK_SHIFT)) {
            me_GetSel(pThis, s, e);
            int n = e + 1;
            if (self->m_bSelectByGroup) n = me_max(nBegin, me_min(n, nEnd));
            me_SetSel(pThis, s, n);
            me_ScrollCaret(pThis);
            return;
        }
        if (me_KeyDown(VK_CONTROL)) {
            me_GetSel(pThis, s, e);
            if (e < self->m_str.GetLength() - 1) me_GetGroupBounds(pThis, nBegin, nEnd, e + 1, TRUE);
            if (nBegin == -1 || (nBegin == s && nEnd == e)) { me_Beep(); return; }
            me_SetSel(pThis, nBegin, nEnd);
            me_ScrollCaret(pThis);
            return;
        }
        me_GetSel(pThis, s, e);
        if (s == e && e == nEnd) {
            if (e < self->m_str.GetLength() - 1) me_GetGroupBounds(pThis, nBegin, nEnd, s + 1, TRUE);
            if (nBegin == -1 || nBegin <= e) { me_Beep(); return; }
            me_SetSel(pThis, nBegin, nBegin);
            me_ScrollCaret(pThis);
            return;
        }
        {
            const int n = me_max(nBegin, me_min(e + 1, nEnd));
            me_SetSel(pThis, n, n);
            me_ScrollCaret(pThis);
        }
        return;
    }
    case VK_LEFT:
    case VK_UP: {
        int s = 0, e = 0;
        me_GetSel(pThis, s, e);
        me_GetGroupBounds(pThis, nBegin, nEnd, s, FALSE);
        if (nBegin == -1) me_GetGroupBounds(pThis, nBegin, nEnd, 0, TRUE);
        if (me_KeyDown(VK_SHIFT)) {
            me_GetSel(pThis, s, e);
            int n = s - 1;
            if (self->m_bSelectByGroup) n = me_min(nEnd, me_max(n, nBegin));
            me_SetSel(pThis, n, e);
            me_ScrollCaret(pThis);
            return;
        }
        if (me_KeyDown(VK_CONTROL)) {
            me_GetSel(pThis, s, e);
            if (s > 1) me_GetGroupBounds(pThis, nBegin, nEnd, s - 1, FALSE);
            if (nBegin == -1 || (nBegin == s && nEnd == e)) { me_Beep(); return; }
            me_SetSel(pThis, nBegin, nEnd);
            me_ScrollCaret(pThis);
            return;
        }
        me_GetSel(pThis, s, e);
        if (s == e && s == nBegin) {
            if (s > 1) me_GetGroupBounds(pThis, nBegin, nEnd, s - 1, FALSE);
            if (nBegin == -1 || nEnd >= s) { me_Beep(); return; }
            me_SetSel(pThis, nEnd, nEnd);
            me_ScrollCaret(pThis);
            return;
        }
        {
            const int n = me_min(nEnd, me_max(s - 1, nBegin));
            me_SetSel(pThis, n, n);
            me_ScrollCaret(pThis);
        }
        return;
    }
    case VK_HOME: {
        int s = 0, e = 0;
        me_GetSel(pThis, s, e);
        me_GetGroupBounds(pThis, nBegin, nEnd, s, FALSE);
        if (nBegin == -1) me_GetGroupBounds(pThis, nBegin, nEnd, 0, TRUE);
        me_SetSel(pThis, nBegin, nBegin);
        me_ScrollCaret(pThis);
        return;
    }
    case VK_END: {
        int s = 0, e = 0;
        me_GetSel(pThis, s, e);
        me_GetGroupBounds(pThis, nBegin, nEnd, e, TRUE);
        if (nBegin == -1) me_GetGroupBounds(pThis, nBegin, nEnd, self->m_str.GetLength(), FALSE);
        me_SetSel(pThis, nEnd, nEnd);
        me_ScrollCaret(pThis);
        return;
    }
    default:
        break;
    }
    me_Default(pThis);
}

// Retail (RVA 0x79c50, mfc140u; export-table entry, WM_LBUTTONUP in the message
// map), fully transcribed (nFlags / point unused):
//     if (m_bSelectByGroup) {
//         int nBegin, nEnd;  GetSel(nBegin, nEnd);
//         GetGroupBounds(nBegin, nEnd, nBegin, TRUE);
//         if (nBegin == -1) { GetSel(nBegin, nEnd); GetGroupBounds(nBegin, nEnd, nBegin, FALSE); }
//         int s, e;  GetSel(s, e);
//         int nNewBegin = min(nEnd, max(nBegin, s));
//         int nNewEnd   = max(nBegin, min(nEnd, e));
//         if (nNewEnd != e || nNewBegin != s) { SetSel(nNewBegin, nNewEnd); ScrollCaret(); }
//     }
//     CWnd::Default();                                                  // tail-jump 0x28ac80
// Symbol: ?OnLButtonUp@CMFCMaskedEdit@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonUp_CMFCMaskedEdit__IEAAXIVCPoint___Z(void* pThis, unsigned int nFlags, long long point) {
    (void)nFlags; (void)point;
    if (!pThis) return;
    ME_Layout* self = ME(pThis);
    if (self->m_bSelectByGroup) {
        int nBegin = 0, nEnd = 0;
        me_GetSel(pThis, nBegin, nEnd);
        me_GetGroupBounds(pThis, nBegin, nEnd, nBegin, TRUE);
        if (nBegin == -1) {
            me_GetSel(pThis, nBegin, nEnd);
            me_GetGroupBounds(pThis, nBegin, nEnd, nBegin, FALSE);
        }
        int s = 0, e = 0;
        me_GetSel(pThis, s, e);
        const int nNewBegin = me_min(nEnd, me_max(nBegin, s));
        const int nNewEnd = me_max(nBegin, me_min(nEnd, e));
        if (nNewEnd != e || nNewBegin != s) {
            me_SetSel(pThis, nNewBegin, nNewEnd);
            me_ScrollCaret(pThis);
        }
    }
    me_Default(pThis);
}

// Retail (RVA 0x7b6b0, mfc140u; export-table entry, EN_SETFOCUS reflected in the
// message map), fully transcribed:
//     if (m_bSelectByGroup) { int nBegin, nEnd; GetGroupBounds(nBegin, nEnd, 0, TRUE); SetSel(nBegin, nEnd); }
//     else SetSel(0, -1);
//     ScrollCaret();                                                    // tail-jump SendMessageW
// Symbol: ?OnSetFocusR@CMFCMaskedEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnSetFocusR_CMFCMaskedEdit__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    if (ME(pThis)->m_bSelectByGroup) {
        int nBegin = 0, nEnd = 0;
        me_GetGroupBounds(pThis, nBegin, nEnd, 0, TRUE);
        me_SetSel(pThis, nBegin, nEnd);
    } else {
        me_SetSel(pThis, 0, -1);
    }
    me_ScrollCaret(pThis);
}

// Retail (RVA 0x7b540, mfc140u; export-table entry -- NOT referenced by the
// class message map, so retail never reaches it through EN_UPDATE), fully transcribed:
//     CString strNew;  CWnd::GetWindowText(strNew);
//     if (wcscmp(m_str, strNew) != 0 && !m_bPasteProcessing) {
//         if (!SetValue(strNew, TRUE)) { MessageBeep(-1); CWnd::SetWindowText(m_str); }
//         else if (wcscmp(strNew, m_str) != 0) {
//             CWnd::SetWindowText(m_str);
//             if (m_bSelectByGroup) { int nBegin, nEnd; GetGroupBounds(nBegin, nEnd, 0, TRUE); SetSel(nBegin, nEnd); }
//             else SetSel(0, -1);
//             ScrollCaret();
//         }
//     }
//     return FALSE;
// Symbol: ?OnUpdateR@CMFCMaskedEdit@@IEAAHXZ
extern "C" int MS_ABI impl__OnUpdateR_CMFCMaskedEdit__IEAAHXZ(void* pThis) {
    if (!pThis) return FALSE;
    ME_Layout* self = ME(pThis);
    CString strNew;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(WND(pThis), &strNew);
    if (std::wcscmp(self->m_str.GetString(), strNew.GetString()) != 0 && !self->m_bPasteProcessing) {
        if (!impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(pThis, strNew.GetString(), TRUE)) {
            me_Beep();
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), self->m_str.GetString());
        } else if (std::wcscmp(strNew.GetString(), self->m_str.GetString()) != 0) {
            impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), self->m_str.GetString());
            if (self->m_bSelectByGroup) {
                int nBegin = 0, nEnd = 0;
                me_GetGroupBounds(pThis, nBegin, nEnd, 0, TRUE);
                me_SetSel(pThis, nBegin, nEnd);
            } else {
                me_SetSel(pThis, 0, -1);
            }
            me_ScrollCaret(pThis);
        }
    }
    return FALSE;
}

// Retail (RVA 0x7b720, mfc140u; both the WM_CUT and the WM_CLEAR entries of the
// message map point here, and the export table resolves ?OnCut and ?OnClear to
// this same entry -- the two bodies were identical-COMDAT-folded), fully transcribed:
//     m_bPasteProcessing = TRUE;
//     int nBeginOld, nEndOld;  GetSel(nBeginOld, nEndOld);
//     CWnd::Default();                                                  // CEdit::OnClear / OnCut
//     CString strNew;  CWnd::GetWindowText(strNew);
//     if (!SetValue(strNew, TRUE)) MessageBeep(-1);
//     CWnd::SetWindowText(m_str);
//     if (m_bSelectByGroup) GetGroupBounds(nBeginOld, nEndOld, nBeginOld, TRUE);
//     SetSel(nBeginOld, nBeginOld); ScrollCaret();
//     m_bPasteProcessing = FALSE;
// Symbol: ?OnClear@CMFCMaskedEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnClear_CMFCMaskedEdit__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    ME_Layout* self = ME(pThis);
    self->m_bPasteProcessing = TRUE;
    int nBeginOld = 0, nEndOld = 0;
    me_GetSel(pThis, nBeginOld, nEndOld);
    me_Default(pThis);
    CString strNew;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(WND(pThis), &strNew);
    if (!impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(pThis, strNew.GetString(), TRUE)) me_Beep();
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), self->m_str.GetString());
    if (self->m_bSelectByGroup) me_GetGroupBounds(pThis, nBeginOld, nEndOld, nBeginOld, TRUE);
    me_SetSel(pThis, nBeginOld, nBeginOld);
    me_ScrollCaret(pThis);
    self->m_bPasteProcessing = FALSE;
}

// Retail: the export table maps ?OnCut to RVA 0x7b720 (mfc140u), the very same
// entry as ?OnClear above (ICF), so this thunk is that body.
// Symbol: ?OnCut@CMFCMaskedEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnCut_CMFCMaskedEdit__IEAAXXZ(void* pThis) {
    impl__OnClear_CMFCMaskedEdit__IEAAXXZ(pThis);
}

// Retail (RVA 0x7b840, mfc140u; export-table entry, WM_PASTE in the message map),
// fully transcribed:
//     m_bPasteProcessing = TRUE;
//     int nBeginOld, nEndOld;  GetSel(nBeginOld, nEndOld);
//     CWnd::Default();                                                  // CEdit::OnPaste
//     int nBeginNew, nEndNew;  GetSel(nBeginNew, nEndNew);
//     nEndNew = max(nEndNew, nBeginNew);
//     CString strText;  CWnd::GetWindowText(strText);
//     CString strPaste = strText.Mid(nBeginOld, nEndNew - nBeginOld);
//     CString strOld;
//     int nPos = nBeginOld;
//     if (m_bSetMaskedCharsOnly) {
//         strOld = GetMaskedValue(TRUE);
//         if (!m_strMask.IsEmpty())                                     // delimiters before the caret don't count
//             for (int i = 0; i < m_strInputTemplate.GetLength() && i < nBeginOld; i++)
//                 if (m_strInputTemplate[i] != '_') nPos--;
//     } else strOld = m_str;
//     CString strNew = strOld.Left(nPos) + strPaste;                    // 0x128f0, operator+ 0x78c90
//     int nTail = (m_strMask.IsEmpty() ? 0 : strPaste.GetLength()) + nPos;
//     if (nTail < strOld.GetLength()) strNew += strOld.Mid(nTail, strOld.GetLength() - nTail);
//     if (!SetValue(strNew, m_bSetMaskedCharsOnly == FALSE)) MessageBeep(-1);
//     CWnd::SetWindowText(m_str);
//     if (m_bSelectByGroup) GetGroupBounds(nBeginOld, nEndOld, nBeginOld, TRUE);
//     SetSel(nBeginOld, nBeginOld); ScrollCaret();
//     m_bPasteProcessing = FALSE;
// Symbol: ?OnPaste@CMFCMaskedEdit@@IEAAXXZ
extern "C" void MS_ABI impl__OnPaste_CMFCMaskedEdit__IEAAXXZ(void* pThis) {
    if (!pThis) return;
    ME_Layout* self = ME(pThis);
    self->m_bPasteProcessing = TRUE;
    int nBeginOld = 0, nEndOld = 0;
    me_GetSel(pThis, nBeginOld, nEndOld);
    me_Default(pThis);
    int nBeginNew = 0, nEndNew = 0;
    me_GetSel(pThis, nBeginNew, nEndNew);
    nEndNew = me_max(nEndNew, nBeginNew);
    CString strText;
    impl__GetWindowTextW_CWnd__QEBAXAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(WND(pThis), &strText);
    CString strPaste = me_Mid(strText, nBeginOld, nEndNew - nBeginOld);
    CString strOld;
    int nPos = nBeginOld;
    if (self->m_bSetMaskedCharsOnly) {
        {
            alignas(CString) unsigned char slot[sizeof(CString)];
            CString* pMasked = impl__GetMaskedValue_CMFCMaskedEdit__IEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(pThis, reinterpret_cast<CString*>(slot), TRUE);
            strOld = *pMasked;
            pMasked->~CString();
        }
        if (!self->m_strMask.IsEmpty()) {
            const int nTpl = self->m_strInputTemplate.GetLength();
            for (int i = 0; i < nTpl && i < nBeginOld; ++i) {
                if (self->m_strInputTemplate.GetString()[i] != L'_') --nPos;
            }
        }
    } else {
        strOld = self->m_str;
    }
    CString strNew = strOld.Left(nPos);
    strNew += strPaste;
    const int nTail = (self->m_strMask.IsEmpty() ? 0 : strPaste.GetLength()) + nPos;
    if (nTail < strOld.GetLength()) {
        strNew += me_Mid(strOld, nTail, strOld.GetLength() - nTail);
    }
    if (!impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(pThis, strNew.GetString(), self->m_bSetMaskedCharsOnly == 0)) me_Beep();
    impl__SetWindowTextW_CWnd__QEAAXPEB_W_Z(WND(pThis), self->m_str.GetString());
    if (self->m_bSelectByGroup) me_GetGroupBounds(pThis, nBeginOld, nEndOld, nBeginOld, TRUE);
    me_SetSel(pThis, nBeginOld, nBeginOld);
    me_ScrollCaret(pThis);
    self->m_bPasteProcessing = FALSE;
}

// Retail (RVA 0x7bbe0, mfc140u; export-table entry, WM_SETTEXT in the message map),
// fully transcribed:
//     if (m_bSetTextProcessing || m_bPasteProcessing || lpszText == NULL) return (int)CWnd::Default();
//     m_bSetTextProcessing = TRUE;
//     if (!SetValue(lpszText, m_bSetMaskedCharsOnly == FALSE)) { m_bSetTextProcessing = FALSE; return FALSE; }
//     int res;
//     if (wcscmp(m_str, lpszText) != 0) res = ::SetWindowTextW(m_hWnd, m_str);   // IAT 0x2c6d90, NOT CWnd::SetWindowText
//     else res = (int)CWnd::Default();
//     m_bSetTextProcessing = FALSE;
//     return res;
// Symbol: ?OnSetText@CMFCMaskedEdit@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMFCMaskedEdit__IEAAHPEB_W_Z(void* pThis, const wchar_t* lpszText) {
    if (!pThis) return 0;
    ME_Layout* self = ME(pThis);
    if (self->m_bSetTextProcessing || self->m_bPasteProcessing || lpszText == nullptr) {
        return static_cast<int>(me_Default(pThis));
    }
    self->m_bSetTextProcessing = TRUE;
    if (!impl__SetValue_CMFCMaskedEdit__IEAAHPEB_WH_Z(pThis, lpszText, self->m_bSetMaskedCharsOnly == 0)) {
        self->m_bSetTextProcessing = FALSE;
        return FALSE;
    }
    int res;
    {
        CString strOld(self->m_str);
        if (std::wcscmp(strOld.GetString(), lpszText) != 0) {
            res = ::SetWindowTextW(HWNDOF(pThis), strOld.GetString());
        } else {
            res = static_cast<int>(me_Default(pThis));
        }
    }
    self->m_bSetTextProcessing = FALSE;
    return res;
}

// Retail (RVA 0x7bcd0, mfc140u; export-table entry, WM_GETTEXT in the message map),
// fully transcribed:
//     if (m_bPasteProcessing) return (int)CWnd::Default();
//     if (nMaxChars == 0 || lpszText == NULL) return 0;
//     CString strText = m_bGetMaskedCharsOnly ? GetMaskedValue(TRUE) : m_str;
//     int nLen = min(strText.GetLength(), nMaxChars);                  // signed compare
//     memcpy(lpszText, strText, nLen * sizeof(WCHAR));                 // IAT 0x2c7420
//     if (nMaxChars > nLen) lpszText[nLen] = 0;
//     return nLen * 2;                                                  // sic: `lea (%rdi,%rdi,1),%eax` -- the BYTE count
// The doubled return value is what retail ships; reproduced as is.
// Symbol: ?OnGetText@CMFCMaskedEdit@@IEAAHHPEA_W@Z
extern "C" int MS_ABI impl__OnGetText_CMFCMaskedEdit__IEAAHHPEA_W_Z(void* pThis, int nMaxChars, wchar_t* lpszText) {
    if (!pThis) return 0;
    ME_Layout* self = ME(pThis);
    if (self->m_bPasteProcessing) return static_cast<int>(me_Default(pThis));
    if (nMaxChars == 0 || lpszText == nullptr) return 0;
    CString strText;
    if (self->m_bGetMaskedCharsOnly) {
        alignas(CString) unsigned char slot[sizeof(CString)];
        CString* pMasked = impl__GetMaskedValue_CMFCMaskedEdit__IEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(pThis, reinterpret_cast<CString*>(slot), TRUE);
        strText = *pMasked;
        pMasked->~CString();
    } else {
        strText = self->m_str;
    }
    const int nLen = me_min(strText.GetLength(), nMaxChars);
    std::memcpy(lpszText, strText.GetString(), static_cast<size_t>(nLen) * sizeof(wchar_t));
    if (nMaxChars > nLen) lpszText[nLen] = L'\0';
    return nLen * 2;
}

// Retail (RVA 0x7be40, mfc140u; export-table entry, WM_GETTEXTLENGTH in the message
// map), fully transcribed:
//     if (m_bPasteProcessing) return (UINT)CWnd::Default();
//     CString strText = m_bGetMaskedCharsOnly ? GetMaskedValue(TRUE) : m_str;
//     return strText.GetLength();
// Symbol: ?OnGetTextLength@CMFCMaskedEdit@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetTextLength_CMFCMaskedEdit__IEAAIXZ(void* pThis) {
    if (!pThis) return 0;
    ME_Layout* self = ME(pThis);
    if (self->m_bPasteProcessing) return static_cast<unsigned int>(me_Default(pThis));
    if (self->m_bGetMaskedCharsOnly) {
        alignas(CString) unsigned char slot[sizeof(CString)];
        CString* pMasked = impl__GetMaskedValue_CMFCMaskedEdit__IEBA_BV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(pThis, reinterpret_cast<CString*>(slot), TRUE);
        const unsigned int n = static_cast<unsigned int>(pMasked->GetLength());
        pMasked->~CString();
        return n;
    }
    return static_cast<unsigned int>(self->m_str.GetLength());
}

// Retail (RVA 0x7bf50, mfc140u; export-table entry, message 0x37c in the message
// map), fully transcribed.  wParam is the byte count and lParam the UTF-8
// initialisation string; every key is a literal in the image (0x33f990..0x33fa60):
//     CString strDst;  CMFCControlContainer::UTF8ToString((LPCSTR)lParam, strDst, (int)wParam);   // 0x34080
//     CTagManager tagManager(strDst);                                                              // 0x13d840
//     BOOL bSelectByGroup = TRUE;
//     if (ReadBoolProp(tagManager, L"MFCMaskedEdit_SelectByGroup", bSelectByGroup)) m_bSelectByGroup = bSelectByGroup;   // 0x1409a0
//     BOOL bEnableMask = FALSE;
//     CString strMask;
//     if (tagManager.ExcludeTag(L"MFCMaskedEdit_Mask", strMask, TRUE)) bEnableMask = !strMask.IsEmpty();               // 0x13df50
//     CString strInputTemplate;
//     if (tagManager.ExcludeTag(L"MFCMaskedEdit_InputTemplate", strInputTemplate, TRUE)
//         && strInputTemplate.GetLength() != strMask.GetLength()) bEnableMask = FALSE;
//     CString strDefaultChar;
//     TCHAR chMaskInputTemplate = '_';
//     if (tagManager.ExcludeTag(L"MFCMaskedEdit_DefaultChar", strDefaultChar, TRUE)
//         && strDefaultChar.GetLength() >= 2) chMaskInputTemplate = strDefaultChar[0];      // sic: `cmpl $2, len; jl`
//     if (bEnableMask) EnableMask(strMask, strInputTemplate, chMaskInputTemplate, NULL);   // 0x78f60
//     else DisableMask();                                                                  // 0x79030
//     CString strValidChars;
//     if (tagManager.ExcludeTag(L"MFCMaskedEdit_ValidChars", strValidChars, TRUE)) SetValidChars(strValidChars);   // 0x79060
//     return 0;
// CTagManager exists in this DLL only as its impl__ thunks over a side table
// keyed by the object address (controls/CTagManager.cpp), so it is built in raw
// storage and torn down through the matching thunk, as
// propertygrid/CMFCPropertyGridCtrl.cpp does.  The 4th ExcludeTag argument is
// retail's bIsCharsList (TRUE at every call here); the OpenMFC thunk names it
// `trim`.
// Symbol: ?OnInitControl@CMFCMaskedEdit@@IEAA_J_K_J@Z
extern "C" __int64 MS_ABI impl__OnInitControl_CMFCMaskedEdit__IEAA_J_K_J_Z(void* pThis, unsigned __int64 wParam, __int64 lParam) {
    if (!pThis) return 0;
    ME_Layout* self = ME(pThis);
    CString strDst;
    impl__UTF8ToString_CMFCControlContainer__SAHPEBDAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
        reinterpret_cast<const char*>(lParam), &strDst, static_cast<int>(wParam));

    alignas(void*) unsigned char tagStorage[sizeof(CTagManager)] = {};
    void* pTagManager = tagStorage;
    impl___0CTagManager__QEAA_PEB_W_Z(pTagManager, strDst.GetString());

    int bSelectByGroup = TRUE;
    if (impl__ReadBoolProp__YAHAEAVCTagManager__PEB_WAEAH_Z(pTagManager, L"MFCMaskedEdit_SelectByGroup", &bSelectByGroup)) {
        self->m_bSelectByGroup = bSelectByGroup;
    }

    int bEnableMask = FALSE;
    CString strMask;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pTagManager, L"MFCMaskedEdit_Mask", &strMask, TRUE)) {
        bEnableMask = !strMask.IsEmpty();
    }
    CString strInputTemplate;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pTagManager, L"MFCMaskedEdit_InputTemplate", &strInputTemplate, TRUE)) {
        if (strInputTemplate.GetLength() != strMask.GetLength()) bEnableMask = FALSE;
    }
    CString strDefaultChar;
    wchar_t chMaskInputTemplate = L'_';
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pTagManager, L"MFCMaskedEdit_DefaultChar", &strDefaultChar, TRUE) &&
        strDefaultChar.GetLength() >= 2) {
        chMaskInputTemplate = strDefaultChar.GetString()[0];
    }
    if (bEnableMask) {
        impl__EnableMask_CMFCMaskedEdit__QEAAXPEB_W0_W0_Z(pThis, strMask.GetString(), strInputTemplate.GetString(), chMaskInputTemplate, nullptr);
    } else {
        impl__DisableMask_CMFCMaskedEdit__QEAAXXZ(pThis);
    }
    CString strValidChars;
    if (impl__ExcludeTag_CTagManager__QEAAHPEB_WAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
            pTagManager, L"MFCMaskedEdit_ValidChars", &strValidChars, TRUE)) {
        impl__SetValidChars_CMFCMaskedEdit__QEAAXPEB_W_Z(pThis, strValidChars.GetString());
    }
    impl___1CTagManager__UEAA_XZ(pTagManager);
    return 0;
}
