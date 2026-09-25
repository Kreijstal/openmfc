// CDialogTemplate — OpenMFC implementation.
// Sources: collections_strings.cpp
//
// Every body in this file is transcribed from the retail mfc140u.dll export
// (disassembled with the campaign's disas.py --u; IAT slots resolved with
// iatu.py against mfc140u.dll).  RVAs below are mfc140u RVAs unless marked
// otherwise.
//
// Object layout.  include/openmfc does not declare CDialogTemplate, so the
// layout is pinned here.  Retail afxpriv.h (class at line 708, members at
// 734-736) declares exactly
//     HGLOBAL m_hTemplate; DWORD m_dwTemplateSize; BOOL m_bSystemFont;
// and nothing else (no virtuals, no base), so sizeof == 0x10 on x64.  The
// retail bodies agree:
//   * Detach (0x21a0f0) reads/clears the qword at +0x0            (m_hTemplate)
//   * SetTemplate (0x219f70) stores cb as a dword at +0x8          (m_dwTemplateSize)
//     and !(style & DS_SETFONT) as a dword at +0xc                 (m_bSystemFont)
//   * both ctors' NULL-template path store two qwords of 0 at +0x0/+0x8
//     (m_hTemplate, then m_dwTemplateSize and m_bSystemFont together).
//
// CRT invalid-parameter paths.  Where retail inlines Checked::memcpy_s /
// memmove_s (SetTemplate, SetFont), a NULL argument runs memset (memcpy_s
// only), `*_errno() = EINVAL`, _invalid_parameter_noinfo() and only then
// AfxThrowInvalidArgException.  Under the UCRT's default handler
// _invalid_parameter_noinfo terminates the process, so retail normally never
// reaches the throw.  DEVIATION: this file skips the errno store and the
// handler call and throws directly.
//
// History: the previous revision of this file kept the font in a std::map
// side table keyed by `this` (g_dialogTemplateStates) and never wrote the
// object.  That could not coexist with the retail-layout accessors (Detach,
// HasFont, GetSizeIn*, SetFont), and a client that reads m_hTemplate directly
// (it is public in afxpriv.h) saw garbage, so the ctors, dtor, SetTemplate and
// both GetFont overloads were rewritten to the retail layout as well.  The side
// table is no longer touched by this file.  The only deviations from retail
// are the ones marked DEVIATION in this file.

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"

// Sibling thunks, declared with the signatures their mangled names describe.
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);        // core/runtime/Globals.cpp
extern "C" void MS_ABI impl__AfxThrowMemoryException__YAXXZ();       // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();   // detail/MfcExceptionsSupport.cpp

// This file's own thunks, called by siblings below.
extern "C" unsigned char* MS_ABI impl__GetFontSizeField_CDialogTemplate__KAPEAEPEBUDLGTEMPLATE___Z(
    const DLGTEMPLATE* pTemplate);
extern "C" unsigned int MS_ABI impl__GetTemplateSize_CDialogTemplate__KAIPEBUDLGTEMPLATE___Z(
    const DLGTEMPLATE* pTemplate);
extern "C" int MS_ABI impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(
    void* pThis, const DLGTEMPLATE* pTemplate, unsigned int cb);
extern "C" int MS_ABI impl__SetFont_CDialogTemplate__QEAAHPEB_WG_Z(
    void* pThis, const wchar_t* lpFaceName, unsigned short nFontSize);
extern "C" void MS_ABI impl__GetSizeInDialogUnits_CDialogTemplate__QEBAXPEAUtagSIZE___Z(
    const void* pThis, SIZE* pSize);
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    void* self, CString* fontName, unsigned short* fontSize);
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__SAHPEBUDLGTEMPLATE__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize);

namespace {

struct S_CDialogTemplate {
    HGLOBAL m_hTemplate;        // +0x0
    DWORD   m_dwTemplateSize;   // +0x8
    BOOL    m_bSystemFont;      // +0xc
};
static_assert(offsetof(S_CDialogTemplate, m_hTemplate) == 0x0, "Detach 0x21a0f0: mov (%rcx),%rax");
static_assert(offsetof(S_CDialogTemplate, m_dwTemplateSize) == 0x8, "SetTemplate 0x219f70: mov %r8d,0x8(%rcx)");
static_assert(offsetof(S_CDialogTemplate, m_bSystemFont) == 0xc, "SetTemplate 0x219f70: mov %ecx,0xc(%rdi)");
static_assert(sizeof(S_CDialogTemplate) == 0x10, "ctors' NULL path zero two qwords at +0/+8");

inline S_CDialogTemplate* DT(void* p) { return static_cast<S_CDialogTemplate*>(p); }
inline const S_CDialogTemplate* DT(const void* p) { return static_cast<const S_CDialogTemplate*>(p); }

// Retail tests `cmp $0xffff, 0x2(template)` (DLGTEMPLATEEX::signature).
inline bool IsDialogEx(const void* p) {
    return reinterpret_cast<const WORD*>(p)[1] == 0xFFFF;
}
// DLGTEMPLATE::style is at +0x0, DLGTEMPLATEEX::style at +0xc.
inline DWORD StyleOf(const void* p) {
    const BYTE* pb = static_cast<const BYTE*>(p);
    return *reinterpret_cast<const DWORD*>(IsDialogEx(p) ? pb + 0xc : pb);
}
// The sz_Or_Ord skip retail inlines everywhere: 0xFFFF introduces a 4-byte
// ordinal; otherwise walk WCHARs up to and past the terminating NUL.
inline BYTE* SkipSzOrOrd(BYTE* pb) {
    if (*reinterpret_cast<const WORD*>(pb) == 0xFFFF) return pb + 4;
    WORD ch;
    do {
        ch = *reinterpret_cast<const WORD*>(pb);
        pb += 2;
    } while (ch != 0);
    return pb;
}
inline BYTE* SkipSz(BYTE* pb) {
    WORD ch;
    do {
        ch = *reinterpret_cast<const WORD*>(pb);
        pb += 2;
    } while (ch != 0);
    return pb;
}
inline BYTE* AlignDword(BYTE* pb) {
    return reinterpret_cast<BYTE*>((reinterpret_cast<ULONG_PTR>(pb) + 3) & ~static_cast<ULONG_PTR>(3));
}

} // namespace

// Retail RVA 0x219f00 (mfc140u):
//     if (hTemplate == NULL) { m_hTemplate = NULL; *(QWORD*)&m_dwTemplateSize = 0; }
//     else {
//         DLGTEMPLATE* p = (DLGTEMPLATE*)::GlobalLock(hTemplate);
//         BOOL ok = SetTemplate(p, GetTemplateSize(p));
//         ::GlobalUnlock(hTemplate);
//         if (!ok) AfxThrowMemoryException();
//     }
//     return this;
// Retail does not test the GlobalLock result; neither does this (a NULL lock
// result faults inside GetTemplateSize in both).
// Symbol: ??0CDialogTemplate@@QEAA@PEAX@Z
extern "C" void* MS_ABI impl___0CDialogTemplate__QEAA_PEAX_Z(void* self, void* tmpl) {
    S_CDialogTemplate* t = DT(self);
    if (tmpl == nullptr) {
        t->m_hTemplate = nullptr;
        t->m_dwTemplateSize = 0;
        t->m_bSystemFont = FALSE;
        return self;
    }
    HGLOBAL hTemplate = static_cast<HGLOBAL>(tmpl);
    const DLGTEMPLATE* p = static_cast<const DLGTEMPLATE*>(::GlobalLock(hTemplate));
    const int ok = impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(
        self, p, impl__GetTemplateSize_CDialogTemplate__KAIPEBUDLGTEMPLATE___Z(p));
    ::GlobalUnlock(hTemplate);
    if (!ok) impl__AfxThrowMemoryException__YAXXZ();
    return self;
}
// Retail RVA 0x219eb0 (mfc140u):
//     if (pTemplate == NULL) { m_hTemplate = NULL; *(QWORD*)&m_dwTemplateSize = 0; }
//     else if (!SetTemplate(pTemplate, GetTemplateSize(pTemplate))) AfxThrowMemoryException();
//     return this;
// Symbol: ??0CDialogTemplate@@QEAA@PEBUDLGTEMPLATE@@@Z
extern "C" void* MS_ABI impl___0CDialogTemplate__QEAA_PEBUDLGTEMPLATE___Z(void* self, const DLGTEMPLATE* tmpl) {
    S_CDialogTemplate* t = DT(self);
    if (tmpl == nullptr) {
        t->m_hTemplate = nullptr;
        t->m_dwTemplateSize = 0;
        t->m_bSystemFont = FALSE;
        return self;
    }
    if (!impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(
            self, tmpl, impl__GetTemplateSize_CDialogTemplate__KAIPEBUDLGTEMPLATE___Z(tmpl)))
        impl__AfxThrowMemoryException__YAXXZ();
    return self;
}
// Retail RVA 0x21a040 (mfc140u): if (m_hTemplate) ::GlobalFree(m_hTemplate);
// Symbol: ??1CDialogTemplate@@QEAA@XZ
extern "C" void MS_ABI impl___1CDialogTemplate__QEAA_XZ(void* self) {
    if (DT(self)->m_hTemplate != nullptr) ::GlobalFree(DT(self)->m_hTemplate);
}
// mfc140u_rva_symbols.json has no RVA for this export, but the mfc140u export
// table does: ordinal 13658 -> RVA 0x219f70 (mfc140u), the function both ctors
// and Load call with (this, pTemplate, cb):
//     m_dwTemplateSize = cb;
//     if (cb + LF_FACESIZE*2 < cb) return FALSE;          // 32-bit wrap check
//     m_hTemplate = ::GlobalAlloc(GPTR /*0x40*/, cb + LF_FACESIZE*2);
//     if (!m_hTemplate) return FALSE;
//     DLGTEMPLATE* pNew = (DLGTEMPLATE*)::GlobalLock(m_hTemplate);
//     if (!pNew) return FALSE;
//     if (m_dwTemplateSize) memcpy(pNew, pTemplate, m_dwTemplateSize);   // IAT 0x1802c7420
//             // (inlined memcpy_s: NULL pTemplate -> memset(pNew, 0, cb) [IAT 0x1802c7418],
//             //  *_errno() = EINVAL, _invalid_parameter_noinfo(), AfxThrowInvalidArgException
//             //  (0x227720); the lock is left held.  See the header note on the deviation.)
//     m_bSystemFont = !(style & DS_SETFONT);   // style of DLGTEMPLATE or DLGTEMPLATEEX
//     ::GlobalUnlock(m_hTemplate);
//     return TRUE;
// The previous m_hTemplate is overwritten, not freed -- retail behaviour.
// Symbol: ?SetTemplate@CDialogTemplate@@IEAAHPEBUDLGTEMPLATE@@I@Z
extern "C" int MS_ABI impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(void* self, const DLGTEMPLATE* tmpl, unsigned int cb) {
    S_CDialogTemplate* t = DT(self);
    t->m_dwTemplateSize = cb;
    const unsigned int cbAlloc = cb + LF_FACESIZE * 2;
    if (cbAlloc < cb) return FALSE;
    t->m_hTemplate = ::GlobalAlloc(GPTR, cbAlloc);
    if (t->m_hTemplate == nullptr) return FALSE;
    BYTE* pNew = static_cast<BYTE*>(::GlobalLock(t->m_hTemplate));
    if (pNew == nullptr) return FALSE;
    if (t->m_dwTemplateSize != 0) {
        if (tmpl == nullptr) {
            std::memset(pNew, 0, t->m_dwTemplateSize);
            impl__AfxThrowInvalidArgException__YAXXZ();
        }
        std::memcpy(pNew, tmpl, t->m_dwTemplateSize);
    }
    t->m_bSystemFont = (StyleOf(pNew) & DS_SETFONT) ? FALSE : TRUE;
    ::GlobalUnlock(t->m_hTemplate);
    return TRUE;
}
// Retail RVA 0x21a370 (mfc140u):
//     p = ::GlobalLock(m_hTemplate); r = GetFont(p, strFace, nFontSize) /*static*/;
//     ::GlobalUnlock(m_hTemplate); return r;
// DEVIATION: retail passes a NULL lock result straight to the static GetFont,
// which faults reading p->style; this returns FALSE instead.
// Symbol: ?GetFont@CDialogTemplate@@QEBAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAG@Z
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    void* self, CString* fontName, unsigned short* fontSize) {
    const DLGTEMPLATE* p = static_cast<const DLGTEMPLATE*>(::GlobalLock(DT(self)->m_hTemplate));
    if (p == nullptr) return FALSE;
    const int r = impl__GetFont_CDialogTemplate__SAHPEBUDLGTEMPLATE__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
        p, fontName, fontSize);
    ::GlobalUnlock(DT(self)->m_hTemplate);
    return r;
}
// Retail RVA 0x21a2f0 (mfc140u), complete:
//     if (!(style & DS_SETFONT)) return FALSE;              // style of DLGTEMPLATE[EX]
//     BYTE* pb = GetFontSizeField(pTemplate);               // 0x21a140
//     nFontSize = *(WORD*)pb;
//     WCHAR* pFace = (WCHAR*)(pb + (IsDialogEx ? 6 : 2));
//     strFaceName.SetString(pFace, pFace ? wcslen(pFace) : 0);   // 0x2e30; wcslen IAT 0x1802c7748
//     return TRUE;
// Like retail it walks the header with GetFontSizeField (the title is a plain
// sz, no 0xFFFF test) and does not null-check its arguments.  SetString with
// wcslen's length is the same as assigning the NUL-terminated face.
// Symbol: ?GetFont@CDialogTemplate@@SAHPEBUDLGTEMPLATE@@AEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@AEAG@Z
extern "C" int MS_ABI impl__GetFont_CDialogTemplate__SAHPEBUDLGTEMPLATE__AEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
    const DLGTEMPLATE* tmpl, CString* fontName, unsigned short* fontSize) {
    if (!(StyleOf(tmpl) & DS_SETFONT)) return FALSE;
    BYTE* pb = impl__GetFontSizeField_CDialogTemplate__KAPEAEPEBUDLGTEMPLATE___Z(tmpl);
    *fontSize = *reinterpret_cast<const WORD*>(pb);
    const wchar_t* pFace = reinterpret_cast<const wchar_t*>(pb + (IsDialogEx(tmpl) ? 6 : 2));
    *fontName = pFace;
    return TRUE;
}
// Retail RVA 0x21a0f0 (mfc140u), complete:
//     HGLOBAL h = m_hTemplate; m_hTemplate = NULL; return h;
// Symbol: ?Detach@CDialogTemplate@@QEAAPEAXXZ
extern "C" void* MS_ABI impl__Detach_CDialogTemplate__QEAAPEAXXZ(void* pThis) {
    HGLOBAL h = DT(pThis)->m_hTemplate;
    DT(pThis)->m_hTemplate = nullptr;
    return h;
}

// Retail RVA 0x21a140 (mfc140u), complete.  Returns the byte after the title
// string, which is where the font point-size WORD sits when DS_SETFONT is set
// (it does not test DS_SETFONT itself):
//     pb = (BYTE*)pTemplate + (IsDialogEx ? 0x1a : 0x12);   // past the fixed header
//     pb = skip sz_Or_Ord (menu); pb = skip sz_Or_Ord (class); pb = skip sz (title);
//     return pb;
// Symbol: ?GetFontSizeField@CDialogTemplate@@KAPEAEPEBUDLGTEMPLATE@@@Z
extern "C" unsigned char* MS_ABI impl__GetFontSizeField_CDialogTemplate__KAPEAEPEBUDLGTEMPLATE___Z(const DLGTEMPLATE* pTemplate) {
    BYTE* pb = reinterpret_cast<BYTE*>(const_cast<DLGTEMPLATE*>(pTemplate)) + (IsDialogEx(pTemplate) ? 0x1a : 0x12);
    pb = SkipSzOrOrd(pb);   // menu
    pb = SkipSzOrOrd(pb);   // window class
    pb = SkipSz(pb);        // title (retail's third loop has no 0xFFFF test)
    return pb;
}

// Retail RVA 0x21a660 (mfc140u), complete:
//     p = ::GlobalLock(m_hTemplate);
//     pSize->cx = *(short*)(p + (IsDialogEx ? 0x16 : 0xe));
//     pSize->cy = *(short*)(p + (IsDialogEx ? 0x18 : 0x10));
//     ::GlobalUnlock(m_hTemplate);                 // tail jump
// DEVIATION: retail faults on a NULL lock result; this leaves *pSize untouched.
// Symbol: ?GetSizeInDialogUnits@CDialogTemplate@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__GetSizeInDialogUnits_CDialogTemplate__QEBAXPEAUtagSIZE___Z(const void* pThis, SIZE* pSize) {
    HGLOBAL h = DT(pThis)->m_hTemplate;
    const BYTE* p = static_cast<const BYTE*>(::GlobalLock(h));
    if (p == nullptr) return;
    const bool ex = IsDialogEx(p);
    pSize->cx = *reinterpret_cast<const short*>(p + (ex ? 0x16 : 0xe));
    pSize->cy = *reinterpret_cast<const short*>(p + (ex ? 0x18 : 0x10));
    ::GlobalUnlock(h);
}

// Retail RVA 0x21a6c0 (mfc140u), transcribed:
//     if (m_bSystemFont) {
//         GetSizeInDialogUnits(pSize);
//         DWORD dlu = ::GetDialogBaseUnits();
//         pSize->cx = (pSize->cx * LOWORD(dlu)) / 4;    // signed division
//         pSize->cy = (pSize->cy * HIWORD(dlu)) / 8;
//         return;
//     }
//     CString strFace; WORD wSize = 10;
//     GetFont(strFace, wSize);                       // instance GetFont, result ignored
//     SIZE size; GetSizeInDialogUnits(&size);
//     HDC hDC = ::GetDC(NULL);
//     LOGFONTW lf; memset(&lf, 0, sizeof lf);
//     lf.lfHeight = -::MulDiv(wSize, ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
//     lf.lfWeight = FW_NORMAL; lf.lfCharSet = DEFAULT_CHARSET;
//     wcsncpy_s(lf.lfFaceName, LF_FACESIZE, strFace, _TRUNCATE);
//         // 0/STRUNCATE ok; ENOMEM -> AfxThrowMemoryException; else AfxThrowInvalidArgException
//     int cxAvg, cyAvg;
//     if (HFONT hFont = ::CreateFontIndirectW(&lf)) {
//         HGDIOBJ hOld = ::SelectObject(hDC, hFont);
//         TEXTMETRICW tm; ::GetTextMetricsW(hDC, &tm);
//         cyAvg = tm.tmHeight + tm.tmExternalLeading;
//         SIZE ext; ::GetTextExtentPoint32W(hDC, L"ABC...XYZabc...xyz", 52, &ext);
//         cxAvg = (ext.cx + 26) / 52;
//         ::SelectObject(hDC, hOld); ::DeleteObject(hFont);
//     } else {
//         cxAvg = LOWORD(::GetDialogBaseUnits()); cyAvg = HIWORD(::GetDialogBaseUnits());
//     }
//     ::ReleaseDC(NULL, hDC);
//     pSize->cx = ::MulDiv(size.cx, cxAvg, 4);
//     pSize->cy = ::MulDiv(size.cy, cyAvg, 8);
// IAT slots resolved against mfc140u: GetDialogBaseUnits 0x1802c6f78, GetDC
// 0x1802c6e28, GetDeviceCaps 0x1802c61a8, MulDiv 0x1802c6658, wcsncpy_s
// 0x1802c7818, CreateFontIndirectW 0x1802c61b0, SelectObject 0x1802c64f8,
// GetTextMetricsW 0x1802c62a8, GetTextExtentPoint32W 0x1802c62a0, DeleteObject
// 0x1802c6278, ReleaseDC 0x1802c6e30; the 52-char string is at 0x18034c9a0.
// Symbol: ?GetSizeInPixels@CDialogTemplate@@QEBAXPEAUtagSIZE@@@Z
extern "C" void MS_ABI impl__GetSizeInPixels_CDialogTemplate__QEBAXPEAUtagSIZE___Z(const void* pThis, SIZE* pSize) {
    if (DT(pThis)->m_bSystemFont) {
        impl__GetSizeInDialogUnits_CDialogTemplate__QEBAXPEAUtagSIZE___Z(pThis, pSize);
        const DWORD dlu = static_cast<DWORD>(::GetDialogBaseUnits());
        pSize->cx = (pSize->cx * static_cast<int>(LOWORD(dlu))) / 4;
        pSize->cy = (pSize->cy * static_cast<int>(HIWORD(dlu))) / 8;
        return;
    }

    CString strFace;
    unsigned short wSize = 10;
    impl__GetFont_CDialogTemplate__QEBAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__AEAG_Z(
        const_cast<void*>(pThis), &strFace, &wSize);

    SIZE size = { 0, 0 };
    impl__GetSizeInDialogUnits_CDialogTemplate__QEBAXPEAUtagSIZE___Z(pThis, &size);

    HDC hDC = ::GetDC(nullptr);
    LOGFONTW lf;
    std::memset(&lf, 0, sizeof lf);
    lf.lfHeight = -::MulDiv(wSize, ::GetDeviceCaps(hDC, LOGPIXELSY), 72);
    lf.lfWeight = FW_NORMAL;
    lf.lfCharSet = DEFAULT_CHARSET;
    const errno_t err = wcsncpy_s(lf.lfFaceName, LF_FACESIZE, static_cast<LPCWSTR>(strFace), _TRUNCATE);
    if (err != 0 && err != STRUNCATE) {
        if (err == ENOMEM) impl__AfxThrowMemoryException__YAXXZ();
        impl__AfxThrowInvalidArgException__YAXXZ();
    }

    int cxAvg;
    int cyAvg;
    HFONT hFont = ::CreateFontIndirectW(&lf);
    if (hFont != nullptr) {
        HGDIOBJ hOld = ::SelectObject(hDC, hFont);
        TEXTMETRICW tm;
        ::GetTextMetricsW(hDC, &tm);
        cyAvg = tm.tmHeight + tm.tmExternalLeading;
        SIZE ext;
        ::GetTextExtentPoint32W(hDC, L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52, &ext);
        cxAvg = (ext.cx + 26) / 52;
        ::SelectObject(hDC, hOld);
        ::DeleteObject(hFont);
    } else {
        cxAvg = LOWORD(::GetDialogBaseUnits());
        cyAvg = HIWORD(::GetDialogBaseUnits());
    }
    ::ReleaseDC(nullptr, hDC);

    pSize->cx = ::MulDiv(size.cx, cxAvg, 4);
    pSize->cy = ::MulDiv(size.cy, cyAvg, 8);
}

// Retail RVA 0x21a1a0 (mfc140u), transcribed:
//     BOOL ex = IsDialogEx(pTemplate);
//     BYTE* pb = GetFontSizeField(pTemplate);
//     if (style & DS_SETFONT) {
//         pb += ex ? 6 : 2;                              // WORD size [+ WORD weight, BYTE italic, BYTE charset]
//         pb += (wcslen((WCHAR*)pb) + 1) * 2;            // face name
//     }
//     WORD nCtrl = ex ? DLGTEMPLATEEX::cDlgItems (+0x10) : DLGTEMPLATE::cdit (+0x8);
//     while (nCtrl--) {
//         pb = AlignDword(pb) + (ex ? 0x18 : 0x12);      // DLGITEMTEMPLATE[EX] fixed part
//         pb = skip sz_Or_Ord (class); pb = skip sz_Or_Ord (title);
//         WORD cbExtra = *(WORD*)pb;
//         pb += 2 + (ex ? cbExtra : (cbExtra ? (WORD)(cbExtra - 2) : 0));
//     }
//     return (UINT)(pb - (BYTE*)pTemplate);
// The non-EX creation-data rule (the count WORD includes itself) is what the
// neg/sbb/and sequence at the end of retail's non-EX loop computes.
// Symbol: ?GetTemplateSize@CDialogTemplate@@KAIPEBUDLGTEMPLATE@@@Z
extern "C" unsigned int MS_ABI impl__GetTemplateSize_CDialogTemplate__KAIPEBUDLGTEMPLATE___Z(const DLGTEMPLATE* pTemplate) {
    const bool ex = IsDialogEx(pTemplate);
    BYTE* base = reinterpret_cast<BYTE*>(const_cast<DLGTEMPLATE*>(pTemplate));
    BYTE* pb = impl__GetFontSizeField_CDialogTemplate__KAPEAEPEBUDLGTEMPLATE___Z(pTemplate);
    if (StyleOf(pTemplate) & DS_SETFONT) {
        BYTE* pFace = pb + (ex ? 6 : 2);
        const size_t len = std::wcslen(reinterpret_cast<const wchar_t*>(pFace));
        pb = pFace + static_cast<int>(len * 2 + 2);
    }
    WORD nCtrl = *reinterpret_cast<const WORD*>(base + (ex ? 0x10 : 0x8));
    while (nCtrl != 0) {
        pb = AlignDword(pb) + (ex ? 0x18 : 0x12);
        pb = SkipSzOrOrd(pb);   // class
        pb = SkipSzOrOrd(pb);   // title
        const WORD cbExtra = *reinterpret_cast<const WORD*>(pb);
        WORD skip;
        if (ex) skip = cbExtra;
        else    skip = cbExtra ? static_cast<WORD>(cbExtra - 2) : static_cast<WORD>(0);
        pb += 2 + static_cast<size_t>(skip);
        --nCtrl;
    }
    return static_cast<unsigned int>(pb - base);
}

// Retail RVA 0x21a100 (mfc140u), complete:
//     p = ::GlobalLock(m_hTemplate); DWORD style = IsDialogEx ? p->ex.style : p->style;
//     ::GlobalUnlock(m_hTemplate); return style & DS_SETFONT;   // 0x40, not 1
// DEVIATION: retail faults on a NULL lock result; this returns FALSE.
// Symbol: ?HasFont@CDialogTemplate@@QEBAHXZ
extern "C" int MS_ABI impl__HasFont_CDialogTemplate__QEBAHXZ(const void* pThis) {
    HGLOBAL h = DT(pThis)->m_hTemplate;
    const void* p = ::GlobalLock(h);
    if (p == nullptr) return FALSE;
    const DWORD style = StyleOf(p);
    ::GlobalUnlock(h);
    return static_cast<int>(style & DS_SETFONT);
}

// Retail RVA 0x21a060 (mfc140u), complete:
//     HINSTANCE hInst = AfxFindResourceHandle(lpDialogTemplateID, RT_DIALOG);
//     if (!hInst) return FALSE;
//     HRSRC hRsrc = ::FindResourceW(hInst, lpDialogTemplateID, RT_DIALOG);   // IAT 0x1802c6588
//     if (!hRsrc) return FALSE;
//     HGLOBAL hRes = ::LoadResource(hInst, hRsrc);                          // IAT 0x1802c6580
//     if (!hRes) return FALSE;
//     void* p = ::LockResource(hRes);                                       // IAT 0x1802c6550
//     return SetTemplate((DLGTEMPLATE*)p, ::SizeofResource(hInst, hRsrc));  // IAT 0x1802c69b8
// Like retail, an existing m_hTemplate is not freed first.
// Symbol: ?Load@CDialogTemplate@@QEAAHPEB_W@Z
extern "C" int MS_ABI impl__Load_CDialogTemplate__QEAAHPEB_W_Z(void* pThis, const wchar_t* lpDialogTemplateID) {
    HINSTANCE hInst = impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(lpDialogTemplateID, RT_DIALOG);
    if (hInst == nullptr) return FALSE;
    HRSRC hRsrc = ::FindResourceW(hInst, lpDialogTemplateID, RT_DIALOG);
    if (hRsrc == nullptr) return FALSE;
    HGLOBAL hRes = ::LoadResource(hInst, hRsrc);
    if (hRes == nullptr) return FALSE;
    const DLGTEMPLATE* p = static_cast<const DLGTEMPLATE*>(::LockResource(hRes));
    return impl__SetTemplate_CDialogTemplate__IEAAHPEBUDLGTEMPLATE__I_Z(
        pThis, p, static_cast<unsigned int>(::SizeofResource(hInst, hRsrc)));
}

// mfc140u_rva_symbols.json has no RVA for this export; the mfc140u export
// table gives ordinal 13204 -> RVA 0x21a3c0 (mfc140u).  SetSystemFont (below)
// calls it last with (this, face, size), and it has the same shape as the ANSI
// export
// ?SetFont@CDialogTemplate@@QEAAHPEBDG@Z (RVA 0x218870, mfc140) except that the
// ANSI build measures the face with strlen and converts it into a stack buffer
// with MultiByteToWideChar before copying.  Transcribed:
//     if (m_dwTemplateSize == 0) return FALSE;
//     DLGTEMPLATE* p = ::GlobalLock(m_hTemplate); if (!p) return FALSE;
//     BOOL ex = IsDialogEx(p);
//     DWORD oldStyle = style;                              // read BEFORE the OR
//     int cbFontAttr = ex ? 6 : 2;
//     style |= DS_SETFONT;                                 // written into the template now
//     int len = lpFaceName ? wcslen(lpFaceName) : 0;
//     if (len >= LF_FACESIZE) return FALSE;                // NB: lock not released
//     int cbName = (len + 1) * 2, cbNew = cbFontAttr + cbName;
//     if (cbNew < cbFontAttr) return FALSE;                // NB: lock not released
//     BYTE* pb = GetFontSizeField(p);
//     int cbOld = (oldStyle & DS_SETFONT) ? cbFontAttr + 2 + 2*wcslen(pb + cbFontAttr) : 0;
//     BYTE* pNewControls = AlignDword(pb + cbNew);
//     BYTE* pOldControls = AlignDword(pb + cbOld);
//     WORD nCtrl = ex ? cDlgItems : cdit;
//     if (cbNew != cbOld && nCtrl > 0) {
//         size_t cb = m_dwTemplateSize - (pOldControls - (BYTE*)p);
//         if (cb > m_dwTemplateSize) return FALSE;         // NB: lock not released
//         if (cb) memmove(pNewControls, pOldControls, cb); // IAT 0x1802c7410
//     }
//     *(WORD*)pb = nFontSize;
//     memmove(pb + cbFontAttr, lpFaceName, cbName);        // inlined memmove_s: NULL face ->
//                                                          // _errno/_invalid_parameter_noinfo,
//                                                          // then AfxThrowInvalidArgException
//     m_dwTemplateSize += (DWORD)(pNewControls - pOldControls);
//     ::GlobalUnlock(m_hTemplate);
//     m_bSystemFont = FALSE;
//     return TRUE;
// All three early FALSE returns after the lock leave the lock held, and the
// face-name length failure leaves DS_SETFONT already set -- both are retail
// behaviour and are kept.  A NULL face gets as far as writing the point size
// before the throw, as in retail (see the header note on the invalid-parameter
// deviation).  The room for growth comes from the LF_FACESIZE*2
// slack SetTemplate allocates.
// Symbol: ?SetFont@CDialogTemplate@@QEAAHPEB_WG@Z
extern "C" int MS_ABI impl__SetFont_CDialogTemplate__QEAAHPEB_WG_Z(void* pThis, const wchar_t* lpFaceName, unsigned short nFontSize) {
    S_CDialogTemplate* t = DT(pThis);
    if (t->m_dwTemplateSize == 0) return FALSE;
    BYTE* p = static_cast<BYTE*>(::GlobalLock(t->m_hTemplate));
    if (p == nullptr) return FALSE;

    const bool ex = IsDialogEx(p);
    DWORD* pStyle = reinterpret_cast<DWORD*>(ex ? p + 0xc : p);
    const DWORD oldStyle = *pStyle;
    const int cbFontAttr = ex ? 6 : 2;
    *pStyle |= DS_SETFONT;

    int len = 0;
    if (lpFaceName != nullptr) {
        len = static_cast<int>(std::wcslen(lpFaceName));
        if (len >= LF_FACESIZE) return FALSE;
    }
    const int cbName = len * 2 + 2;
    const int cbNew = cbName + cbFontAttr;
    if (cbNew < cbFontAttr) return FALSE;

    BYTE* pb = impl__GetFontSizeField_CDialogTemplate__KAPEAEPEBUDLGTEMPLATE___Z(reinterpret_cast<const DLGTEMPLATE*>(p));
    int cbOld = 0;
    if (oldStyle & DS_SETFONT)
        cbOld = cbFontAttr + 2 + 2 * static_cast<int>(std::wcslen(reinterpret_cast<const wchar_t*>(pb + cbFontAttr)));

    BYTE* pNewControls = AlignDword(pb + cbNew);
    BYTE* pOldControls = AlignDword(pb + cbOld);
    const WORD nCtrl = *reinterpret_cast<const WORD*>(p + (ex ? 0x10 : 0x8));
    if (cbNew != cbOld && nCtrl != 0) {
        const size_t cbTemplate = t->m_dwTemplateSize;
        const size_t cbMove = cbTemplate - static_cast<size_t>(pOldControls - p);
        if (cbMove > cbTemplate) return FALSE;
        if (cbMove != 0) std::memmove(pNewControls, pOldControls, cbMove);
    }

    *reinterpret_cast<WORD*>(pb) = nFontSize;
    if (lpFaceName == nullptr) impl__AfxThrowInvalidArgException__YAXXZ();
    std::memmove(pb + cbFontAttr, lpFaceName, static_cast<size_t>(cbName));

    t->m_dwTemplateSize += static_cast<DWORD>(pNewControls - pOldControls);
    ::GlobalUnlock(t->m_hTemplate);
    t->m_bSystemFont = FALSE;
    return TRUE;
}

// mfc140u_rva_symbols.json has no RVA for this export; the mfc140u export
// table gives ordinal 13630 -> RVA 0x21a570 (mfc140u), the Unicode twin of the ANSI export at RVA 0x218a70 (mfc140) --
// same control flow and constants, with GetObjectW asked for 0x5c bytes
// (LOGFONTW) where the ANSI build asks GetObjectA for 0x3c (LOGFONTA).  Its
// final call targets 0x21a3c0 (SetFont, confirmed by the export table).  Transcribed:
//     LPCWSTR pszFace = L"System";  WORD wDefSize = 10;     // string at 0x18034ca10
//     HGDIOBJ hFont = ::GetStockObject(DEFAULT_GUI_FONT);   // 17
//     if (!hFont) hFont = ::GetStockObject(SYSTEM_FONT);    // 13
//     LOGFONTW lf;
//     if (hFont && ::GetObjectW(hFont, sizeof(LOGFONTW), &lf)) {
//         pszFace = lf.lfFaceName;
//         HDC hDC = ::GetDC(NULL);
//         if (lf.lfHeight < 0) lf.lfHeight = -lf.lfHeight;
//         wDefSize = (WORD)::MulDiv(lf.lfHeight, 72, ::GetDeviceCaps(hDC, LOGPIXELSY));
//         ::ReleaseDC(NULL, hDC);
//     }
//     if (nFontSize == 0) nFontSize = wDefSize;
//     return SetFont(pszFace, nFontSize);
// IAT slots (mfc140u): GetStockObject 0x1802c6250, GetObjectW 0x1802c6290,
// GetDC 0x1802c6e28, GetDeviceCaps 0x1802c61a8, MulDiv 0x1802c6658,
// ReleaseDC 0x1802c6e30.
// Symbol: ?SetSystemFont@CDialogTemplate@@QEAAHG@Z
extern "C" int MS_ABI impl__SetSystemFont_CDialogTemplate__QEAAHG_Z(void* pThis, unsigned short nFontSize) {
    LPCWSTR pszFace = L"System";
    unsigned short wDefSize = 10;
    HGDIOBJ hFont = ::GetStockObject(DEFAULT_GUI_FONT);
    if (hFont == nullptr) hFont = ::GetStockObject(SYSTEM_FONT);
    LOGFONTW lf;
    if (hFont != nullptr && ::GetObjectW(hFont, sizeof(LOGFONTW), &lf) != 0) {
        pszFace = lf.lfFaceName;
        HDC hDC = ::GetDC(nullptr);
        if (lf.lfHeight < 0) lf.lfHeight = -lf.lfHeight;
        wDefSize = static_cast<unsigned short>(::MulDiv(lf.lfHeight, 72, ::GetDeviceCaps(hDC, LOGPIXELSY)));
        ::ReleaseDC(nullptr, hDC);
    }
    if (nFontSize == 0) nFontSize = wDefSize;
    return impl__SetFont_CDialogTemplate__QEAAHPEB_WG_Z(pThis, pszFace, nFontSize);
}
