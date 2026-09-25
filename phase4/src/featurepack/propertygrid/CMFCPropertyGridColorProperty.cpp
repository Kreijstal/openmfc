// CMFCPropertyGridColorProperty — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp, mfccore.cpp
//
// Layout.  OpenMFC declares no CMFCPropertyGridColorProperty class, and its
// CMFCPropertyGridProperty (include/openmfc/afxmfc.h) is a GCC C++ class whose
// layout is NOT retail's: its state lives in a smaller object plus the
// PropertyGridPropertyState side table.  Clients, however, compile against the
// retail header (afxpropertygridctrl.h:250) and allocate retail's sizeof, so the
// colour property's own members sit at retail offsets past retail's 0x160-byte
// base.  They are pinned here from the retail constructor
// (??0CMFCPropertyGridColorProperty, RVA 0xc4960 (mfc140u)) and destructor
// (RVA 0xc4b10 (mfc140u)); the ctor writes every one of them:
//     +0x160 COLORREF m_Color            +0x164 COLORREF m_ColorOrig
//     +0x168 COLORREF m_ColorAutomatic   +0x170 CString  m_strAutoColor
//     +0x178 CString  m_strOtherColor    +0x180 CMFCColorPopupMenu* m_pPopup
//     +0x188 CArray<COLORREF,COLORREF> m_Colors (0x28 bytes: vfptr, m_pData,
//            m_nSize, m_nMaxSize, m_nGrowBy)
//     +0x1b0 int m_nColumnsNumber        +0x1b4 BOOL m_bStdColorDlg
// Base-class state is reached ONLY through the CMFCPropertyGridProperty impl__
// thunks, never through retail base offsets (m_Rect +0x44, m_pWndInPlace
// +0x100, m_pWndList +0x118, ...), which do not exist in OpenMFC's base.
// The retail bodies below were read from mfc140.dll (the ANSI twin; bodies are
// byte-identical); the RVAs quoted are the mfc140u entry points.

#define OPENMFC_APPCORE_IMPL

#include "detail/ManualSmallStubImplementationsSupport.h"
#include "detail/MfccoreSupport.h"

#include <cstddef>
#include <cstdlib>
#include <new>

namespace {

// Retail CArray<COLORREF,COLORREF> (CObject-derived, INT_PTR counters).  Laid
// out by hand because OpenMFC's CArray template has no vfptr and int counters.
struct RetailColorArray {
    void*     vfptr;       // +0x00
    COLORREF* m_pData;     // +0x08
    INT_PTR   m_nSize;     // +0x10
    INT_PTR   m_nMaxSize;  // +0x18
    INT_PTR   m_nGrowBy;   // +0x20
};
static_assert(sizeof(RetailColorArray) == 0x28, "retail CArray<COLORREF> is 0x28 bytes");

// Members CMFCPropertyGridColorProperty adds past retail's 0x160-byte base.
struct ColorPropertyTail {
    COLORREF         m_Color;           // +0x160
    COLORREF         m_ColorOrig;       // +0x164
    COLORREF         m_ColorAutomatic;  // +0x168
    CString          m_strAutoColor;    // +0x170
    CString          m_strOtherColor;   // +0x178
    void*            m_pPopup;          // +0x180 CMFCColorPopupMenu*
    RetailColorArray m_Colors;          // +0x188
    int              m_nColumnsNumber;  // +0x1b0
    BOOL             m_bStdColorDlg;    // +0x1b4
};
constexpr std::size_t kColorTailBase = 0x160;
static_assert(sizeof(CString) == 8, "retail CString is one pointer");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_Color) == 0x160, "m_Color");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_ColorOrig) == 0x164, "m_ColorOrig");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_ColorAutomatic) == 0x168, "m_ColorAutomatic");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_strAutoColor) == 0x170, "m_strAutoColor");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_strOtherColor) == 0x178, "m_strOtherColor");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_pPopup) == 0x180, "m_pPopup");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_Colors) == 0x188, "m_Colors");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_nColumnsNumber) == 0x1b0, "m_nColumnsNumber");
static_assert(kColorTailBase + offsetof(ColorPropertyTail, m_bStdColorDlg) == 0x1b4, "m_bStdColorDlg");
static_assert(kColorTailBase + sizeof(ColorPropertyTail) == 0x1b8, "retail sizeof(CMFCPropertyGridColorProperty)");
// OpenMFC's base object is placement-constructed at pThis by the base ctor
// thunk; it must end before the retail-offset tail or the two would overlap.
static_assert(sizeof(CMFCPropertyGridProperty) <= kColorTailBase,
              "OpenMFC CMFCPropertyGridProperty must fit inside retail's 0x160-byte base");

inline ColorPropertyTail* ColorTail(void* pThis) {
    return reinterpret_cast<ColorPropertyTail*>(static_cast<unsigned char*>(pThis) + kColorTailBase);
}

} // namespace

// Base-class and sibling entry points (each definition verified in the tree).
// featurepack/propertygrid/CMFCPropertyGridProperty.cpp:
extern "C" void* MS_ABI impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
    CMFCPropertyGridProperty* pThis, const CString* strName, const COleVariant* varValue,
    const wchar_t* lpszDescr, DWORD_PTR dwData, const wchar_t* lpszEditMask,
    const wchar_t* lpszEditTemplate, const wchar_t* lpszValidChars);
extern "C" void MS_ABI impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
    CMFCPropertyGridProperty* pThis, CDC* pDC, CRect rect);
// featurepack/propertygrid/Thunks.cpp:
extern "C" void MS_ABI impl___1CMFCPropertyGridProperty__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__ResetOriginalValue_CMFCPropertyGridProperty__UEAAXXZ(CMFCPropertyGridProperty* pThis);
extern "C" void MS_ABI impl__SetValue_CMFCPropertyGridProperty__UEAAXAEBVCOleVariant___Z(
    CMFCPropertyGridProperty* pThis, const COleVariant* p0);
// core/gdi/CDC.cpp:
extern "C" void MS_ABI impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(
    CDC* pThis, const RECT* lpRect, unsigned long clrTopLeft, unsigned long clrBottomRight);
// featurepack/controls/CMFCColorBar.cpp (static; also declared in
// detail/CMFCColorButtonSupport.h, which this unit does not include):
extern "C" int MS_ABI impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(
    void* pPalette, void* arColors);

// Symbol: ??0CMFCPropertyGridColorProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEBKPEAVCPalette@@PEB_W_K@Z
// Transcribed from retail RVA 0xc4960 (mfc140u):
//     CMFCPropertyGridProperty(strName, COleVariant() /*VT_EMPTY*/, lpszDescr, dwData, NULL, NULL, NULL);
//     vfptr = CMFCPropertyGridColorProperty::`vftable';
//     m_Color = color; m_ColorOrig = color;
//     m_strAutoColor, m_strOtherColor default-constructed;
//     m_Colors: vfptr, m_pData = 0, m_nSize = m_nMaxSize = m_nGrowBy = 0;
//     CMFCColorBar::InitColors(pPalette, m_Colors);
//     m_varValue = (LONG)color; m_varValueOrig = (LONG)color;   // base +0x08 / +0x20
//     m_dwFlags = 1;                                             // base +0x40 (AFX_PROP_HAS_LIST)
//     m_pPopup = NULL; m_nColumnsNumber = 5; m_bStdColorDlg = FALSE; m_ColorAutomatic = 0;
// DEVIATIONS: (1) the base is given COleVariant((long)color) (VT_I4) instead of
// VT_EMPTY-then-assign, because OpenMFC's base ctor records the variant it is
// handed as the original value -- the resulting value/original pair is the same
// VT_I4 retail ends with.  (2) OpenMFC has no MSVC-layout vftable for this class
// (nor for CArray<COLORREF>), so the base's vptr is kept and m_Colors.vfptr is
// NULL.  (3) OpenMFC's base has no m_dwFlags, so the value 1 is not stored.
// That is the drop-down-list bit (AFX_PROP_HAS_LIST), not the has-button bit:
// retail AddOption (mfc140u) stores the same 1, while the
// CMFCPropertyGridFileProperty ctors (RVAs 0xc54b0 / 0xc55f0 (mfc140u)) store
// 2.  Retail HasButton tests (m_dwFlags & 3), so both bits show a button.
extern "C" void* MS_ABI impl___0CMFCPropertyGridColorProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBKPEAVCPalette__PEB_W_K_Z(
    void* pThis, const CString* strName, const COLORREF* color, void* pPalette,
    const wchar_t* lpszDescr, unsigned long long dwData) {
    const COLORREF clr = *color;
    COleVariant varValue(static_cast<long>(clr));
    impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
        static_cast<CMFCPropertyGridProperty*>(pThis), strName, &varValue,
        lpszDescr, static_cast<DWORD_PTR>(dwData), nullptr, nullptr, nullptr);

    ColorPropertyTail* t = ColorTail(pThis);
    t->m_Color = clr;                                   // +0x160
    t->m_ColorOrig = clr;                               // +0x164
    new (&t->m_strAutoColor) CString();                 // +0x170
    new (&t->m_strOtherColor) CString();                // +0x178
    t->m_Colors.vfptr = nullptr;                        // +0x188 (see DEVIATION 2)
    t->m_Colors.m_pData = nullptr;
    t->m_Colors.m_nGrowBy = 0;
    t->m_Colors.m_nMaxSize = 0;
    t->m_Colors.m_nSize = 0;
    (void)impl__InitColors_CMFCColorBar__KAHPEAVCPalette__AEAV__CArray_KK___Z(pPalette, &t->m_Colors);
    t->m_pPopup = nullptr;                              // +0x180
    t->m_nColumnsNumber = 5;                            // +0x1b0 (qword store of 5
    t->m_bStdColorDlg = FALSE;                          // +0x1b4  covers both fields)
    t->m_ColorAutomatic = 0;                            // +0x168
    return pThis;
}
// Symbol: ?SetOriginalValue@CMFCPropertyGridColorProperty@@UEAAXAEBVCOleVariant@@@Z
// Transcribed from retail entry RVA 0xc50c0 (mfc140u):
//     CMFCPropertyGridProperty::SetOriginalValue(varValue);            // direct call
//     m_ColorOrig = varValue.lVal;               // +0x164 <- dword at VARIANT+8, no vt check
extern "C" void MS_ABI impl__SetOriginalValue_CMFCPropertyGridColorProperty__UEAAXAEBVCOleVariant___Z(
    CMFCPropertyGridProperty* pThis, const COleVariant* varValue)
{
    impl__SetOriginalValue_CMFCPropertyGridProperty__UEAAXAEBVCOleVariant___Z(pThis, varValue);
    ColorTail(pThis)->m_ColorOrig = static_cast<COLORREF>(varValue->lVal);
}
// Symbol: ??1CMFCPropertyGridColorProperty@@UEAA@XZ
// Transcribed from retail RVA 0xc4b10 (mfc140u):
//     vfptr = CMFCPropertyGridColorProperty::`vftable';
//     m_Colors.~CArray();            // +0x188: free(m_pData) when non-NULL
//     m_strOtherColor.~CString();    // +0x178
//     m_strAutoColor.~CString();     // +0x170
//     CMFCPropertyGridProperty::~CMFCPropertyGridProperty();   // tail jump
// The vfptr store is omitted (no MSVC vftable exists for this class in
// OpenMFC; see the ctor).  Parameter list corrected to the mangled signature
// (this only, void return).
extern "C" void MS_ABI impl___1CMFCPropertyGridColorProperty__UEAA_XZ(void* pThis) {
    ColorPropertyTail* t = ColorTail(pThis);
    if (t->m_Colors.m_pData != nullptr) {
        std::free(t->m_Colors.m_pData);   // retail's CArray dtor calls CRT free
    }
    t->m_strOtherColor.~CString();
    t->m_strAutoColor.~CString();
    impl___1CMFCPropertyGridProperty__UEAA_XZ(pThis);
}

// Symbol: ?AdjustInPlaceEditRect@CMFCPropertyGridColorProperty@@MEAAXAEAVCRect@@0@Z
// STUB: retail (RVA 0xc5020 (mfc140u)) empties rectSpin (::SetRectEmpty), copies
// the base's m_Rect (+0x44) into rectEdit and derives rectEdit.left from grid
// fields of m_pWndList (+0x118 -> +0x55c/+0x590/+0x5ac/+0x5b0/+0x5c0), then calls
// the AdjustButtonRect virtual (vtable +0x100, slot 32) and takes rectEdit.right
// from m_rectButton (+0x54).  None of m_Rect, m_rectButton or m_pWndList exist
// in OpenMFC's base, and no in-place editor is ever created, so there is
// nothing faithful to compute.  Parameter list corrected to the mangled
// signature (this, CRect&, CRect&).
extern "C" void MS_ABI impl__AdjustInPlaceEditRect_CMFCPropertyGridColorProperty__MEAAXAEAVCRect__0_Z(
    void* pThis, CRect* rectEdit, CRect* rectSpin) {
    (void)pThis; (void)rectEdit; (void)rectSpin;
}

// Symbol: ?EnableAutomaticButton@CMFCPropertyGridColorProperty@@QEAAXPEB_WKH@Z
// Transcribed from retail RVA 0xc52f0 (mfc140u):
//     m_ColorAutomatic = colorAutomatic;                              // +0x168, unconditional
//     m_strAutoColor = (bEnable && lpszLabel != NULL) ? lpszLabel : L"";   // +0x170
// (retail assigns through CSimpleStringT::SetString(psz, wcslen(psz))).
extern "C" void MS_ABI impl__EnableAutomaticButton_CMFCPropertyGridColorProperty__QEAAXPEB_WKH_Z(
    void* pThis, const wchar_t* lpszLabel, unsigned long colorAutomatic, int bEnable) {
    ColorPropertyTail* t = ColorTail(pThis);
    t->m_ColorAutomatic = colorAutomatic;
    t->m_strAutoColor = (bEnable && lpszLabel != nullptr) ? lpszLabel : L"";
}

// Symbol: ?EnableOtherButton@CMFCPropertyGridColorProperty@@QEAAXPEB_WHH@Z
// Transcribed from retail RVA 0xc5340 (mfc140u):
//     m_bStdColorDlg = !bAltColorDlg;                                  // +0x1b4, unconditional
//     m_strOtherColor = (bEnable && lpszLabel != NULL) ? lpszLabel : L"";  // +0x178
extern "C" void MS_ABI impl__EnableOtherButton_CMFCPropertyGridColorProperty__QEAAXPEB_WHH_Z(
    void* pThis, const wchar_t* lpszLabel, int bAltColorDlg, int bEnable) {
    ColorPropertyTail* t = ColorTail(pThis);
    t->m_bStdColorDlg = bAltColorDlg ? FALSE : TRUE;
    t->m_strOtherColor = (bEnable && lpszLabel != nullptr) ? lpszLabel : L"";
}

// Symbol: ?FormatProperty@CMFCPropertyGridColorProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Transcribed from retail RVA 0xc5110 (mfc140u), which takes this in RCX and
// the hidden return slot in RDX and returns the slot in RAX:
//     if (m_Color == (COLORREF)-1) return m_strAutoColor;              // +0x160, +0x170
//     CString str; str.Format(L"%02x%02x%02x", GetRValue(m_Color), GetGValue(m_Color), GetBValue(m_Color));
//     return str;
// (format literal read from mfc140u at VA 0x180340380; the three arguments are
// the bytes at +0x160/+0x161/+0x162.)  Parameter list corrected to that ABI.
extern "C" CString* MS_ABI impl__FormatProperty_CMFCPropertyGridColorProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pThis, CString* pRet) {
    ColorPropertyTail* t = ColorTail(pThis);
    if (t->m_Color == static_cast<COLORREF>(-1)) {
        return new (pRet) CString(t->m_strAutoColor);
    }
    CString* str = new (pRet) CString();
    str->Format(L"%02x%02x%02x",
                static_cast<unsigned int>(GetRValue(t->m_Color)),
                static_cast<unsigned int>(GetGValue(t->m_Color)),
                static_cast<unsigned int>(GetBValue(t->m_Color)));
    return pRet;
}

// Symbol: ?OnClickButton@CMFCPropertyGridColorProperty@@UEAAXVCPoint@@@Z
// STUB: retail (RVA 0xc4c60 (mfc140u)) sets m_bButtonIsDown (+0x64), redraws,
// allocates a 0x2ed8-byte CMFCColorPopupMenu constructed from m_Colors, m_Color,
// m_ColorAutomatic and m_nColumnsNumber, stores it in m_pPopup (+0x180), hands
// it m_pWndList (+0x118) at popup +0x2ec0, enables its Other/Automatic buttons
// on the embedded colour bar (popup +0x19c8), positions it from m_pWndList
// grid fields (+0x590/+0x5c0) and m_rectButton.bottom (+0x60; m_rectButton is
// at +0x54), converts that point with ::ClientToScreen, then calls the popup's
// Create virtual (vtable +0x430). If Create fails it resets m_pPopup to NULL.
// Otherwise it calls SetFocus on the result of vtable +0x3a0.
// OpenMFC has neither the retail CMFCColorPopupMenu layout nor the base's
// m_Rect / m_pWndList / m_bButtonIsDown, so the popup cannot be built.
// Parameter list corrected to the mangled signature (this, CPoint by value).
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridColorProperty__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis; (void)point;
}

// Symbol: ?OnDrawValue@CMFCPropertyGridColorProperty@@UEAAXPEAVCDC@@VCRect@@@Z
// Transcribed from retail entry RVA 0xc4b90 (mfc140u). That RVA is missing from
// mfc140u_rva_symbols.json, but mfc140u's own export table names it as this
// export. Its body matches the ANSI twin's export at 0xc5110 (mfc140):
//     CRect rectColor = rect;
//     rect.left += rect.Height();
//     CMFCPropertyGridProperty::OnDrawValue(pDC, rect);                // direct call
//     rectColor.right = rectColor.left + rectColor.Height();
//     ::InflateRect(rectColor, -1, -1);
//     rectColor.top++; rectColor.left++;
//     CBrush br(m_Color == (COLORREF)-1 ? m_ColorAutomatic : m_Color); // +0x160 / +0x168
//     ::FillRect(pDC->m_hDC, &rectColor, br);
//     pDC->Draw3dRect(&rectColor, 0, 0);
// DEVIATIONS: (1) the brush is a raw ::CreateSolidBrush/::DeleteObject pair.
// Retail's CBrush(COLORREF) ctor throws a resource exception if creation
// fails; here FillRect just receives a NULL brush. (2) The base OnDrawValue
// thunk calls FormatProperty virtually through the OpenMFC base vptr (see the
// ctor's DEVIATION 2), so the text it draws comes from the base FormatProperty
// and not from this class's hex formatter.
extern "C" void MS_ABI impl__OnDrawValue_CMFCPropertyGridColorProperty__UEAAXPEAVCDC__VCRect___Z(
    void* pThis, CDC* pDC, CRect rect) {
    CRect rectColor = rect;
    rect.left += rect.Height();
    impl__OnDrawValue_CMFCPropertyGridProperty__UEAAXPEAVCDC__VCRect___Z(
        static_cast<CMFCPropertyGridProperty*>(pThis), pDC, rect);

    rectColor.right = rectColor.left + rectColor.Height();
    ::InflateRect(rectColor, -1, -1);
    rectColor.top++;
    rectColor.left++;

    ColorPropertyTail* t = ColorTail(pThis);
    const COLORREF clr = (t->m_Color == static_cast<COLORREF>(-1)) ? t->m_ColorAutomatic : t->m_Color;
    HBRUSH hbr = ::CreateSolidBrush(clr);
    ::FillRect(pDC->GetSafeHdc(), rectColor, hbr);
    impl__Draw3dRect_CDC__QEAAXPEBUtagRECT__KK_Z(pDC, rectColor, 0, 0);
    if (hbr != nullptr) ::DeleteObject(hbr);
}

// Symbol: ?OnEdit@CMFCPropertyGridColorProperty@@UEAAHPEAUtagPOINT@@@Z
// STUB: retail (RVA 0xc4e70 (mfc140u)) clears m_pWndInPlace (+0x100), gets the
// edit rect from the AdjustInPlaceEditRect virtual (vtable +0x108), creates a
// CMFCMaskedEdit (0x128 bytes; ??0CMFCMaskedEdit@@QEAA@XZ) with a hex valid-char
// set, Create()s it on m_pWndList (+0x118), stores it in m_pWndInPlace, sets its
// text from the FormatProperty virtual (vtable +0x58), sends it WM_SETFONT,
// SetFocus()es it, sets m_bInPlaceEdit (+0x6c) and returns TRUE.  OpenMFC's
// property grid never creates in-place edit windows and its base has none of
// those members, so the TRUE ("edit started") result would be a lie.
// Parameter list corrected to the mangled signature (this, LPPOINT).
extern "C" int MS_ABI impl__OnEdit_CMFCPropertyGridColorProperty__UEAAHPEAUtagPOINT___Z(
    void* pThis, POINT* lptClick) {
    (void)pThis; (void)lptClick;
    return FALSE;
}

// Symbol: ?OnUpdateValue@CMFCPropertyGridColorProperty@@UEAAHXZ
// Retail entry RVA 0xc53a0 (mfc140u):
//     if (m_pWndInPlace == NULL) return FALSE;                          // +0x100
//     CString str; m_pWndInPlace->GetWindowText(str);
//     if (!str.IsEmpty()) {
//         COLORREF colorCurr = m_Color; int nR = 0, nG = 0, nB = 0;
//         _stscanf_s(str, L"%2x%2x%2x", &nR, &nG, &nB);
//         m_Color = RGB(nR, nG, nB);
//         if (colorCurr != m_Color) m_pWndList->OnPropertyChanged(this);   // grid vtable +0x2f8
//     }
//     return TRUE;
// STUB: OpenMFC never creates an in-place edit window (the same convention
// CMFCPropertyGridCtrl.cpp applies to every `m_pWndInPlace == NULL` guard), so
// only retail's leading early return is reachable.  Parameter list corrected
// to the mangled signature (this).
extern "C" int MS_ABI impl__OnUpdateValue_CMFCPropertyGridColorProperty__UEAAHXZ(void* pThis) {
    (void)pThis;
    return FALSE;
}

// Symbol: ?ResetOriginalValue@CMFCPropertyGridColorProperty@@MEAAXXZ
// Transcribed from retail entry RVA 0xc50f0 (mfc140u):
//     CMFCPropertyGridProperty::ResetOriginalValue();                  // direct call
//     m_Color = m_ColorOrig;                                           // +0x160 <- +0x164
extern "C" void MS_ABI impl__ResetOriginalValue_CMFCPropertyGridColorProperty__MEAAXXZ(void* pThis) {
    impl__ResetOriginalValue_CMFCPropertyGridProperty__UEAAXXZ(static_cast<CMFCPropertyGridProperty*>(pThis));
    ColorTail(pThis)->m_Color = ColorTail(pThis)->m_ColorOrig;
}

// Symbol: ?SetColor@CMFCPropertyGridColorProperty@@QEAAXK@Z
// Transcribed from retail entry RVA 0xc51e0 (mfc140u):
//     m_Color = color;                                                  // +0x160
//     m_varValue = (LONG)color;                                         // base +0x08, VT_I4
//     if (::IsWindow(m_pWndList->GetSafeHwnd())) {                      // +0x118
//         CRect rect = m_Rect; rect.InflateRect(0, -1);                 // +0x44
//         ::InvalidateRect(m_pWndList->m_hWnd, rect, TRUE);
//         ::UpdateWindow(m_pWndList->m_hWnd);
//     }
//     if (m_pWndInPlace != NULL) m_pWndInPlace->SetWindowText(FormatProperty());  // +0x100, vtable +0x58
// DEVIATIONS: in OpenMFC's base, m_varValue is a protected member of a
// different layout and cannot be reached from this free function, so the
// value is stored through the base SetValue thunk; that thunk also refreshes
// the owning grid (TouchPropertyGridCtrl), which stands in for retail's
// InvalidateRect/UpdateWindow pair (OpenMFC does not model m_Rect), and
// recomputes the base's modified flag, which retail's SetColor does not touch.
// The in-place branch is absent because OpenMFC never creates m_pWndInPlace.
extern "C" void MS_ABI impl__SetColor_CMFCPropertyGridColorProperty__QEAAXK_Z(void* pThis, unsigned long color) {
    ColorTail(pThis)->m_Color = color;
    COleVariant varValue(static_cast<long>(color));
    impl__SetValue_CMFCPropertyGridProperty__UEAAXAEBVCOleVariant___Z(
        static_cast<CMFCPropertyGridProperty*>(pThis), &varValue);
}

// Symbol: ?SetColumnsNumber@CMFCPropertyGridColorProperty@@QEAAXH@Z
// Transcribed from retail entry RVA 0xc52e0 (mfc140u): m_nColumnsNumber = nColumnsNumber (+0x1b0).
extern "C" void MS_ABI impl__SetColumnsNumber_CMFCPropertyGridColorProperty__QEAAXH_Z(void* pThis, int nColumnsNumber) {
    ColorTail(pThis)->m_nColumnsNumber = nColumnsNumber;
}
