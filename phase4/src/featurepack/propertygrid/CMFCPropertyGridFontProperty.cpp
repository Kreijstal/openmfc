// CMFCPropertyGridFontProperty — OpenMFC implementation.
// Sources: manual_small_stub_implementations.cpp

#include "detail/ManualSmallStubImplementationsSupport.h"

#include <cstddef>
#include <cstring>

namespace {
struct FontPropertyTail {
    LOGFONTW m_logFont;
    LOGFONTW m_logFontOriginal;
    COLORREF m_color;
    DWORD m_reserved;
    DWORD m_fontType;
};
static_assert(sizeof(LOGFONTW) == 0x5c, "retail LOGFONTW size");
static_assert(offsetof(FontPropertyTail, m_logFontOriginal) == 0x5c, "original LOGFONTW offset");
static_assert(offsetof(FontPropertyTail, m_color) == 0xb8, "font color offset");
static_assert(offsetof(FontPropertyTail, m_fontType) == 0xc0, "font type offset");
static_assert(0x160 + sizeof(FontPropertyTail) == 0x224, "retail font property tail size");
static_assert(sizeof(CMFCPropertyGridProperty) <= 0x160,
              "OpenMFC property base must fit before the retail font-property tail");

inline FontPropertyTail* FontTail(void* pThis) {
    return reinterpret_cast<FontPropertyTail*>(static_cast<unsigned char*>(pThis) + 0x160);
}
} // namespace

extern "C" void* MS_ABI impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
    CMFCPropertyGridProperty* pThis, const CString* strName, const COleVariant* varValue,
    const wchar_t* lpszDescr, DWORD_PTR dwData, const wchar_t* lpszEditMask,
    const wchar_t* lpszEditTemplate, const wchar_t* lpszValidChars);
extern "C" void MS_ABI impl___1CMFCPropertyGridProperty__UEAA_XZ(void* pThis);
extern "C" void MS_ABI impl__ResetOriginalValue_CMFCPropertyGridProperty__UEAAXXZ(
    CMFCPropertyGridProperty* pThis);

// Symbol: ??0CMFCPropertyGridFontProperty@@QEAA@AEBV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAUtagLOGFONTW@@KPEB_W_KK@Z
// Transcribed from retail constructor RVA 0xc59f0 (mfc140u). The base receives
// the face name as its initial value, the description and data arguments; the
// two LOGFONTW copies, color, and font-type value occupy the retail tail. The
// OpenMFC base constructor initializes its own state; no retail vftable pointer
// or retail base offsets are written into its differently laid-out base object.
extern "C" void* MS_ABI impl___0CMFCPropertyGridFontProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAUtagLOGFONTW__KPEB_W_KK_Z(
    void* pThis, const CString* name, LOGFONTW* lf, unsigned long color,
    const wchar_t* text, unsigned long long data, unsigned long fontType) {
    COleVariant faceName(lf->lfFaceName);
    impl___0CMFCPropertyGridProperty__QEAA_AEBV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEBVCOleVariant__PEB_W_K222_Z(
        static_cast<CMFCPropertyGridProperty*>(pThis), name, &faceName, text,
        static_cast<DWORD_PTR>(data), nullptr, nullptr, nullptr);

    FontPropertyTail* tail = FontTail(pThis);
    std::memcpy(&tail->m_logFont, lf, sizeof(*lf));
    std::memcpy(&tail->m_logFontOriginal, lf, sizeof(*lf));
    tail->m_color = color;
    tail->m_fontType = fontType;
    return pThis;
}

// Symbol: ??1CMFCPropertyGridFontProperty@@UEAA@XZ
// The retail destructor only restores the derived vfptr before its base dtor;
// this implementation has no derived non-trivial tail members and delegates
// cleanup to the OpenMFC base destructor thunk.
extern "C" void MS_ABI impl___1CMFCPropertyGridFontProperty__UEAA_XZ(void* pThis) {
    impl___1CMFCPropertyGridProperty__UEAA_XZ(pThis);
}

// Symbol: ?FormatProperty@CMFCPropertyGridFontProperty@@UEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@XZ
// Stub: retail RVA 0xc5d80 (mfc140u) reads the owner pointer at +0x118 and
// follows a resource/format helper path using ID 0x5a and font data at +0x160
// and +0x17c. The required retail helper path is not implemented here. The
// hidden CString return slot is the second ABI argument, after this.
extern "C" void MS_ABI impl__FormatProperty_CMFCPropertyGridFontProperty__UEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__XZ(
    void* pThis, void* pRet) {
    (void)pThis;
    (void)pRet;
}

// Symbol: ?OnClickButton@CMFCPropertyGridFontProperty@@UEAAXVCPoint@@@Z
// Stub: retail RVA 0xc5ba0 (mfc140u) copies the current LOGFONT, runs a private
// font-selection UI path, then conditionally updates the font and owner grid.
// That dialog/helper object model is not implemented in this unit.
extern "C" void MS_ABI impl__OnClickButton_CMFCPropertyGridFontProperty__UEAAXVCPoint___Z(
    void* pThis, long long point) {
    (void)pThis;
    (void)point;
}

// Symbol: ?ResetOriginalValue@CMFCPropertyGridFontProperty@@MEAAXXZ
// The Unicode export has no resolved RVA. Its constructor establishes the
// LOGFONTW current/original copies at +0x160/+0x1bc (RVA 0xc59f0, mfc140u);
// the ANSI twin's reset restores its LOGFONTA original copy after base reset.
extern "C" void MS_ABI impl__ResetOriginalValue_CMFCPropertyGridFontProperty__MEAAXXZ(void* pThis) {
    impl__ResetOriginalValue_CMFCPropertyGridProperty__UEAAXXZ(
        static_cast<CMFCPropertyGridProperty*>(pThis));
    FontPropertyTail* tail = FontTail(pThis);
    std::memcpy(&tail->m_logFont, &tail->m_logFontOriginal, sizeof(LOGFONTW));
}
