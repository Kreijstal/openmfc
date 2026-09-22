// AFX_GLOBAL_DATA — OpenMFC implementation.
// Sources: collections_strings.cpp, manual_small_stub_implementations.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/CollectionsStringsSupport.h"
#include "detail/ManualSmallStubImplementationsSupport.h"

#include <uxtheme.h>

// Symbol: ?ExcludeTag@AFX_GLOBAL_DATA@@QEAAHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W0H@Z
extern "C" int MS_ABI impl__ExcludeTag_AFX_GLOBAL_DATA__QEAAHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W0H_Z(
    void*, CString* buffer, const wchar_t* beginTag, const wchar_t* endTag, int) {
    return ExtractTaggedText(buffer, beginTag, endTag) ? TRUE : FALSE;
}
// Symbol: ?RegisterWindowClass@AFX_GLOBAL_DATA@@QEAA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@PEB_W@Z
extern "C" void MS_ABI impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* ret, void*, const wchar_t* requestedClassName) {
    std::wstring className = MakeWindowClassName(requestedClassName);
    HINSTANCE instance = GetModuleHandleW(nullptr);

    WNDCLASSEXW existing = {};
    existing.cbSize = sizeof(existing);
    if (!GetClassInfoExW(instance, className.c_str(), &existing)) {
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(wc);
        wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = DefWindowProcW;
        wc.hInstance = instance;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszClassName = className.c_str();
        RegisterClassExW(&wc);
    }

    new (ret) CString(className.c_str());
}
// Symbol: ?DrawTextOnGlass@AFX_GLOBAL_DATA@@QEAAHPEAXPEAVCDC@@HHV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@VCRect@@KHK@Z
extern "C" int MS_ABI impl__DrawTextOnGlass_AFX_GLOBAL_DATA__QEAAHPEAXPEAVCDC__HHV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__VCRect__KHK_Z(
    void*, void*, CDC* dc, int, int, const CString* text, CRect rect, unsigned long color, int format, unsigned long) {
    if (!dc || !dc->GetSafeHdc()) return FALSE;
    COLORREF oldColor = SetTextColor(dc->GetSafeHdc(), color);
    int oldBkMode = SetBkMode(dc->GetSafeHdc(), TRANSPARENT);
    RECT winRect = { rect.left, rect.top, rect.right, rect.bottom };
    int result = DrawTextW(dc->GetSafeHdc(), CStringText(text), -1, &winRect, static_cast<UINT>(format));
    SetBkMode(dc->GetSafeHdc(), oldBkMode);
    SetTextColor(dc->GetSafeHdc(), oldColor);
    return result;
}
// Symbol: ??0AFX_GLOBAL_DATA@@QEAA@XZ
extern "C" void* MS_ABI impl___0AFX_GLOBAL_DATA__QEAA_XZ(void* pThis) {
    return pThis;
}

// ---------------------------------------------------------------------------
// AFX_GLOBAL_DATA lifecycle / system-state members.
//
// Everything below this line was transcribed from the retail disassembly with
//   python3 <wf>/disas.py '<mangled>'   (image: mfc140.dll, the ANSI twin of the
// mfc140u.dll OpenMFC reimplements -- function bodies are byte-identical, so the
// control flow, member offsets and constants read out of it are correct; only the
// RVAs differ between the two images, and every RVA quoted below is an mfc140 RVA
// and is labelled as such).  Import slots were resolved with iat.py, never guessed.
//
// THE OBJECT IS A RAW BLOB.  ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A is exported by
// featurepack/CMFC_misc_stubs.cpp:3646 as a 720-byte zero-initialised array -- there
// is no AFX_GLOBAL_DATA C++ type in OpenMFC and no constructor runs for it.  These
// thunks therefore work against the `void* pThis` they are handed, through the
// shadow struct `AfxGlobalData` below.  That shadow is a transcription of the retail
// declaration (MSVC 14.51 atlmfc/include/afxglobals.h:66) with every member offset
// pinned by a static_assert, and it agrees with the offsets ~20 other files in this
// tree already hard-code (clrBtnFace 0x28, clrBtnShadow 0x2c, clrBtnHilite 0x30,
// clrBtnText 0x34, fontRegular 0x1a8, fontTooltip 0x1b8, fontBold 0x1c8,
// m_dblRibbonImageScale 0x2b8, m_bIsRibbonImageScale 0x2c0).  Four offsets are
// confirmed a second time by the disassembly itself: GetColor (0x6bba0) reads 19
// distinct COLORREF members whose switch labels are the COLOR_* indices, Initialize
// (0x6a5c0) writes bIsWindows7 at 0x258, and CreateDitherBitmap (0x6b6c0) /
// UpdateSysColors (0x6afd0) reach the object's statics at absolute 0x3ba3a8 /
// 0x3ba3c8 / 0x3ba3e0 / 0x3ba3e4, which are afxGlobalData (mfc140 0x3ba380) + 0x28 /
// 0x48 / 0x60 / 0x64 -- clrBtnFace, clrHilite, clrBarFace, clrBarShadow.
//
// DELIBERATE DEVIATION, applies to every GDI member below.  In retail the brush/pen/
// font members are real CBrush/CPen/CFont objects and are written through
// CGdiObject::Detach + CGdiObject::Attach, which also add/remove the handle in the
// MFC handle map.  Here the blob has no vftable pointers and no handle map entry can
// be meaningful, so ResetGdi() below deletes the old handle and stores the new one
// directly.  The resulting memory image (vfptr left NULL, m_hObject at +8 of each
// 16-byte slot) is what every other file in this tree already reads.
// ---------------------------------------------------------------------------

namespace {

// CGdiObject in an x64 MSVC build: vftable pointer, then the HGDIOBJ.  sizeof == 16,
// and CBrush / CPen / CFont / CBitmap add no members.
struct GdiObj {
    void*   vfptr;
    HGDIOBJ m_hObject;
};

// Transcription of `struct AFX_GLOBAL_DATA` (afxglobals.h:66).  Member ORDER and
// hence every offset comes from that header; the offsets marked below are the ones
// independently confirmed by the retail disassembly.
struct AfxGlobalData {
    int      m_bInitialized;                    // 0x000  (Initialize 0x6a5c0 guards on it)
    int      m_bUseSystemFont;                  // 0x004  (UpdateFonts 0x6aaa3)
    int      m_bDontReduceFontHeight;           // 0x008  (UpdateFonts 0x6aa80)
    int      m_bInSettingChange;                // 0x00c  (OnSettingChange 0x6aec2)
    HBRUSH   hbrBtnHilite;                      // 0x010
    HBRUSH   hbrBtnShadow;                      // 0x018
    HBRUSH   hbrWindow;                         // 0x020
    COLORREF clrBtnFace;                        // 0x028  (GetColor case COLOR_BTNFACE)
    COLORREF clrBtnShadow;                      // 0x02c  (GetColor case COLOR_BTNSHADOW)
    COLORREF clrBtnHilite;                      // 0x030  (GetColor case COLOR_BTNHIGHLIGHT)
    COLORREF clrBtnText;                        // 0x034  (GetColor case COLOR_BTNTEXT)
    COLORREF clrWindowFrame;                    // 0x038  (GetColor case COLOR_WINDOWFRAME)
    COLORREF clrBtnDkShadow;                    // 0x03c  (GetColor case COLOR_3DDKSHADOW)
    COLORREF clrBtnLight;                       // 0x040  (GetColor case COLOR_3DLIGHT)
    COLORREF clrGrayedText;                     // 0x044  (GetColor case COLOR_GRAYTEXT)
    COLORREF clrHilite;                         // 0x048  (GetColor case COLOR_HIGHLIGHT)
    COLORREF clrTextHilite;                     // 0x04c  (GetColor case COLOR_HIGHLIGHTTEXT)
    COLORREF clrHotLinkNormalText;              // 0x050
    COLORREF clrHotLinkHoveredText;             // 0x054
    COLORREF clrHotLinkVisitedText;             // 0x058
    COLORREF clrBarWindow;                      // 0x05c
    COLORREF clrBarFace;                        // 0x060
    COLORREF clrBarShadow;                      // 0x064
    COLORREF clrBarHilite;                      // 0x068
    COLORREF clrBarDkShadow;                    // 0x06c
    COLORREF clrBarLight;                       // 0x070
    COLORREF clrBarText;                        // 0x074
    COLORREF clrWindow;                         // 0x078  (GetColor case COLOR_WINDOW)
    COLORREF clrWindowText;                     // 0x07c  (GetColor case COLOR_WINDOWTEXT)
    COLORREF clrCaptionText;                    // 0x080  (GetColor case COLOR_CAPTIONTEXT)
    COLORREF clrMenuText;                       // 0x084  (GetColor case COLOR_MENUTEXT)
    COLORREF clrActiveCaption;                  // 0x088  (GetColor case COLOR_ACTIVECAPTION)
    COLORREF clrInactiveCaption;                // 0x08c  (GetColor case COLOR_INACTIVECAPTION)
    COLORREF clrInactiveCaptionText;            // 0x090  (GetColor case COLOR_INACTIVECAPTIONTEXT)
    COLORREF clrActiveCaptionGradient;          // 0x094
    COLORREF clrInactiveCaptionGradient;        // 0x098
    COLORREF clrActiveBorder;                   // 0x09c  (GetColor case COLOR_ACTIVEBORDER)
    COLORREF clrInactiveBorder;                 // 0x0a0  (GetColor case COLOR_INACTIVEBORDER)
    GdiObj   brBtnFace;                         // 0x0a8
    GdiObj   brHilite;                          // 0x0b8
    GdiObj   brLight;                           // 0x0c8
    GdiObj   brBlack;                           // 0x0d8
    GdiObj   brActiveCaption;                   // 0x0e8
    GdiObj   brInactiveCaption;                 // 0x0f8
    GdiObj   brWindow;                          // 0x108
    GdiObj   brBarFace;                         // 0x118
    GdiObj   penHilite;                         // 0x128
    GdiObj   penBarFace;                        // 0x138
    GdiObj   penBarShadow;                      // 0x148
    HCURSOR  m_hcurStretch;                     // 0x158
    HCURSOR  m_hcurStretchVert;                 // 0x160
    HCURSOR  m_hcurHand;                        // 0x168  (GetHandCursor 0x6c566)
    HCURSOR  m_hcurSizeAll;                     // 0x170
    HCURSOR  m_hcurMoveTab;                     // 0x178
    HCURSOR  m_hcurNoMoveTab;                   // 0x180
    HICON    m_hiconTool;                       // 0x188
    HICON    m_hiconLink;                       // 0x190
    HICON    m_hiconColors;                     // 0x198
    LONG     m_sizeSmallIcon_cx;                // 0x1a0  (OnSettingChange 0x6aed9)
    LONG     m_sizeSmallIcon_cy;                // 0x1a4  (OnSettingChange 0x6aeec)
    GdiObj   fontRegular;                       // 0x1a8
    GdiObj   fontTooltip;                       // 0x1b8
    GdiObj   fontBold;                          // 0x1c8
    GdiObj   fontDefaultGUIBold;                // 0x1d8
    GdiObj   fontUnderline;                     // 0x1e8
    GdiObj   fontDefaultGUIUnderline;           // 0x1f8
    GdiObj   fontVert;                          // 0x208
    GdiObj   fontVertCaption;                   // 0x218
    GdiObj   fontSmall;                         // 0x228
    GdiObj   fontMarlett;                       // 0x238
    RECT     m_rectVirtual;                     // 0x248  (OnSettingChange 0x6aee5)
    int      bIsWindows7;                       // 0x258  (Initialize 0x6a64a)
    int      bIsRemoteSession;                  // 0x25c  (Initialize 0x6a659)
    int      m_bIsBlackHighContrast;            // 0x260  (UpdateSysColors 0x6b00d)
    int      m_bIsWhiteHighContrast;            // 0x264  (UpdateSysColors 0x6b035)
    int      m_bUseBuiltIn32BitIcons;           // 0x268
    int      m_bMenuAnimation;                  // 0x26c  (OnSettingChange 0x6af39)
    int      m_bMenuFadeEffect;                 // 0x270  (OnSettingChange 0x6af3f)
    int      m_bIsRTL;                          // 0x274
    int      m_bEnableAccessibility;            // 0x278  (EnableAccessibilitySupport 0x6bcd0)
    int      m_bUnderlineKeyboardShortcuts;     // 0x27c  (OnSettingChange 0x6afb6)
    int      m_bSysUnderlineKeyboardShortcuts;  // 0x280  (OnSettingChange 0x6af7c)
    int      m_bRefreshAutohideBars;            // 0x284  (OnSettingChange 0x6af90)
    int      m_nBitsPerPixel;                   // 0x288  (UpdateSysColors 0x6b058)
    int      m_nDragFrameThicknessFloat;        // 0x28c
    int      m_nDragFrameThicknessDock;         // 0x290
    int      m_nAutoHideToolBarSpacing;         // 0x294
    int      m_nAutoHideToolBarMargin;          // 0x298
    int      m_nCoveredMainWndClientAreaPercent;// 0x29c
    int      m_nMaxToolTipWidth;                // 0x2a0
    int      m_nShellAutohideBars;              // 0x2a4  (OnSettingChange 0x6af83)
    int      m_nTextHeightHorz;                 // 0x2a8  (UpdateTextMetrics 0x6b628)
    int      m_nTextHeightVert;                 // 0x2ac  (UpdateTextMetrics 0x6b661)
    int      m_nTextWidthHorz;                  // 0x2b0  (UpdateTextMetrics 0x6b631)
    int      m_nTextWidthVert;                  // 0x2b4  (UpdateTextMetrics 0x6b66c)
    double   m_dblRibbonImageScale;             // 0x2b8  (UpdateFonts 0x6a874)
    int      m_bIsRibbonImageScale;             // 0x2c0
    int      m_bBufferedPaintInited;            // 0x2c4  (CleanUp 0x6b8aa)
    HINSTANCE m_hinstDwmapiDLL;                 // 0x2c8  (CleanUp 0x6b917)
};

static_assert(sizeof(AfxGlobalData) == 720, "AFX_GLOBAL_DATA is 720 bytes (CMFC_misc_stubs.cpp:3646)");
static_assert(offsetof(AfxGlobalData, m_bInitialized)          == 0x000, "");
static_assert(offsetof(AfxGlobalData, m_bUseSystemFont)        == 0x004, "");
static_assert(offsetof(AfxGlobalData, m_bDontReduceFontHeight) == 0x008, "");
static_assert(offsetof(AfxGlobalData, m_bInSettingChange)      == 0x00c, "");
static_assert(offsetof(AfxGlobalData, hbrBtnHilite)            == 0x010, "");
static_assert(offsetof(AfxGlobalData, hbrBtnShadow)            == 0x018, "");
static_assert(offsetof(AfxGlobalData, hbrWindow)               == 0x020, "");
static_assert(offsetof(AfxGlobalData, clrBtnFace)              == 0x028, "pinned by CMFCHeaderCtrl.cpp / CMFCStatusBar.cpp");
static_assert(offsetof(AfxGlobalData, clrBtnShadow)            == 0x02c, "pinned by CMFCStatusBar.cpp");
static_assert(offsetof(AfxGlobalData, clrBtnHilite)            == 0x030, "pinned by CMFCStatusBar.cpp");
static_assert(offsetof(AfxGlobalData, clrBtnText)              == 0x034, "pinned by CMFCHeaderCtrl.cpp");
static_assert(offsetof(AfxGlobalData, clrWindowFrame)          == 0x038, "");
static_assert(offsetof(AfxGlobalData, clrGrayedText)           == 0x044, "");
static_assert(offsetof(AfxGlobalData, clrHilite)               == 0x048, "");
static_assert(offsetof(AfxGlobalData, clrBarFace)              == 0x060, "");
static_assert(offsetof(AfxGlobalData, clrBarShadow)            == 0x064, "");
static_assert(offsetof(AfxGlobalData, clrWindow)               == 0x078, "");
static_assert(offsetof(AfxGlobalData, clrInactiveBorder)       == 0x0a0, "");
static_assert(offsetof(AfxGlobalData, brBtnFace)               == 0x0a8, "");
static_assert(offsetof(AfxGlobalData, brLight)                 == 0x0c8, "");
static_assert(offsetof(AfxGlobalData, brBarFace)               == 0x118, "");
static_assert(offsetof(AfxGlobalData, penHilite)               == 0x128, "");
static_assert(offsetof(AfxGlobalData, penBarShadow)            == 0x148, "");
static_assert(offsetof(AfxGlobalData, m_hcurHand)              == 0x168, "");
static_assert(offsetof(AfxGlobalData, m_sizeSmallIcon_cx)      == 0x1a0, "");
static_assert(offsetof(AfxGlobalData, fontRegular)             == 0x1a8, "pinned by CMFCStatusBar.cpp / CMFCHeaderCtrl.cpp");
static_assert(offsetof(AfxGlobalData, fontTooltip)             == 0x1b8, "pinned by CMFCToolTipCtrl.cpp");
static_assert(offsetof(AfxGlobalData, fontBold)                == 0x1c8, "pinned by CMFCToolTipCtrl.cpp");
static_assert(offsetof(AfxGlobalData, fontVert)                == 0x208, "");
static_assert(offsetof(AfxGlobalData, fontMarlett)             == 0x238, "");
static_assert(offsetof(AfxGlobalData, m_rectVirtual)           == 0x248, "");
static_assert(offsetof(AfxGlobalData, bIsWindows7)             == 0x258, "confirmed by Initialize (mfc140 0x6a5c0)");
static_assert(offsetof(AfxGlobalData, bIsRemoteSession)        == 0x25c, "confirmed by Initialize (mfc140 0x6a5c0)");
static_assert(offsetof(AfxGlobalData, m_bEnableAccessibility)  == 0x278, "confirmed by EnableAccessibilitySupport (mfc140 0x6bcd0)");
static_assert(offsetof(AfxGlobalData, m_nBitsPerPixel)         == 0x288, "");
static_assert(offsetof(AfxGlobalData, m_nShellAutohideBars)    == 0x2a4, "");
static_assert(offsetof(AfxGlobalData, m_nTextHeightHorz)       == 0x2a8, "");
static_assert(offsetof(AfxGlobalData, m_dblRibbonImageScale)   == 0x2b8, "pinned by CMFCToolTipCtrl.cpp");
static_assert(offsetof(AfxGlobalData, m_bIsRibbonImageScale)   == 0x2c0, "pinned by CMFCToolTipCtrl.cpp");
static_assert(offsetof(AfxGlobalData, m_bBufferedPaintInited)  == 0x2c4, "");
static_assert(offsetof(AfxGlobalData, m_hinstDwmapiDLL)        == 0x2c8, "");

inline AfxGlobalData* GD(void* pThis) { return static_cast<AfxGlobalData*>(pThis); }

// Retail: CGdiObject::Detach() + ::DeleteObject() + CGdiObject::Attach().  See the
// "DELIBERATE DEVIATION" note above for why the handle map step is dropped.
inline void ResetGdi(GdiObj& o, HGDIOBJ hNew) {
    if (o.m_hObject != nullptr) {
        ::DeleteObject(o.m_hObject);
        o.m_hObject = nullptr;
    }
    o.m_hObject = hNew;
}

inline void DeleteGdi(GdiObj& o) {
    if (o.m_hObject != nullptr) {
        ::DeleteObject(o.m_hObject);
        o.m_hObject = nullptr;
    }
}

// --- IsDwmCompositionEnabled / ResetCheckCompositionFlag file statics ------------
// Retail keeps these in .data at mfc140 0x3bb96c (the "already checked" flag),
// 0x3bcb00 (the cached BOOL) and 0x3bcc18 (the EncodePointer'd DwmIsCompositionEnabled
// address).  They are NOT members of afxGlobalData -- 0x3bcb00 is far past the
// object's 0x3ba380..0x3ba650 extent -- so they are file statics here too.
int    g_bCheckedComposition   = 0;
int    g_bCompositionEnabled   = 0;
void*  g_pfnDwmIsCompEnabledEnc = nullptr;

typedef HRESULT (WINAPI* PFNDwmIsCompositionEnabled)(BOOL*);
typedef HRESULT (WINAPI* PFNBufferedPaintUnInit)(void);
typedef HRESULT (WINAPI* PFNSHCreateItemFromParsingName)(PCWSTR, void*, const GUID&, void**);

// Retail loads system DLLs through an internal helper (entry mfc140 0xd8230).  It
// resolves kernel32!SetDefaultDllDirectories once (GetModuleHandleW(L"kernel32.dll") +
// GetProcAddress, cached EncodePointer'd at mfc140 0x3bcba0) purely as a feature
// probe, then:
//   * if that export exists: LoadLibraryExW(name, NULL, LOAD_LIBRARY_SEARCH_SYSTEM32)
//     (via the wrapper at 0xd80c4) -- and a failure there is final, there is NO
//     retry with a bare name;
//   * otherwise: GetSystemDirectoryW into a MAX_PATH+1 (0x105) buffer, fail if the
//     returned length is 0 or > 0x104, append L"\\" unless the path already ends in
//     one, wcscat_s the name, and LoadLibraryW that FULL path (via 0xd8180).
// Deviation: the two wrappers also activate MFC's module activation context around
// the load (0x1c514 / DeactivateActCtx); OpenMFC has no such context, so the plain
// loader call is made.
HMODULE LoadSystemLibrary(const wchar_t* pszName) {
    static void* s_pfnSetDefaultDllDirectoriesEnc = nullptr;
    void* pfn = nullptr;
    if (s_pfnSetDefaultDllDirectoriesEnc == nullptr) {
        HMODULE hKernel = ::GetModuleHandleW(L"kernel32.dll");
        if (hKernel != nullptr) {
            pfn = reinterpret_cast<void*>(::GetProcAddress(hKernel, "SetDefaultDllDirectories"));
            s_pfnSetDefaultDllDirectoriesEnc = ::EncodePointer(pfn);
        }
    } else {
        pfn = ::DecodePointer(s_pfnSetDefaultDllDirectoriesEnc);
    }
    if (pfn != nullptr) {
        return ::LoadLibraryExW(pszName, nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    }

    wchar_t szPath[MAX_PATH + 1];
    const UINT nLen = ::GetSystemDirectoryW(szPath, MAX_PATH + 1);
    if (nLen - 1 > 0x103) return nullptr;             // 0 or longer than 0x104
    if (szPath[nLen - 1] != L'\\' && wcscat_s(szPath, MAX_PATH + 1, L"\\") != 0) return nullptr;
    if (wcscat_s(szPath, MAX_PATH + 1, pszName) != 0) return nullptr;
    return ::LoadLibraryW(szPath);
}

// UpdateFonts' font-family probe.  Retail's callback is at mfc140 0x6a350 / mfc140u
// 0x6a520: it builds a CString from ENUMLOGFONT::elfLogFont.lfFaceName (+0x1c) and
// returns (name.CollateNoCase((LPCTSTR)lParam) != 0) -- the compare is the CRT
// collation compare, import slot resolved to _wcsicoll in mfc140u (_mbsicoll in the
// ANSI twin), NOT lstrcmpi -- i.e. 0 to STOP once the wanted face is seen, so
// EnumFontFamilies() returning 0 means "the face exists".  Retail raises
// AfxThrowInvalidArgException on a NULL font or NULL lParam; neither can happen here.
int CALLBACK FindFontFamilyProc(const LOGFONTW* plf, const TEXTMETRICW*, DWORD, LPARAM lParam) {
    return ::_wcsicoll(plf->lfFaceName, reinterpret_cast<const wchar_t*>(lParam)) != 0;
}

bool IsFontFamilyAvailable(HDC hDC, const wchar_t* pszFace) {
    return ::EnumFontFamiliesW(hDC, nullptr, reinterpret_cast<FONTENUMPROCW>(FindFontFamilyProc),
                               reinterpret_cast<LPARAM>(pszFace)) == 0;
}

// OnSettingChange's EnumDisplayMonitors callback (retail mfc140 0x6ae20): unions every
// monitor's WORK area into the LPARAM rect.
BOOL CALLBACK VirtualRectEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM lParam) {
    MONITORINFO mi;
    ::ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(MONITORINFO);
    if (::GetMonitorInfoW(hMonitor, &mi)) {
        RECT* pRect = reinterpret_cast<RECT*>(lParam);
        RECT  rcWork = mi.rcWork;
        if (rcWork.left   < pRect->left)   pRect->left   = rcWork.left;
        if (rcWork.right  > pRect->right)  pRect->right  = rcWork.right;
        if (rcWork.top    < pRect->top)    pRect->top    = rcWork.top;
        if (rcWork.bottom > pRect->bottom) pRect->bottom = rcWork.bottom;
    }
    return TRUE;
}

} // namespace

// Siblings called from Initialize / SetMenuFont / UpdateFonts / the destructor.  They
// are defined in this same translation unit; the declarations carry the signature the
// mangled name describes (all four are __thiscall, so all four take pThis).
extern "C" void MS_ABI impl__CleanUp_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateSysColors_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateFonts_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__OnSettingChange_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__UpdateTextMetrics_AFX_GLOBAL_DATA__IEAAXXZ(void* pThis);
extern "C" void* MS_ABI impl__CreateDitherBitmap_AFX_GLOBAL_DATA__IEAAPEAUHBITMAP____PEAUHDC_____Z(void* pThis, HDC hDC);

// Cross-file thunks.  Each one is a definition that already exists in this tree:
//   CMFCReBar.cpp:1026, StaticData.cpp (docking/visualmanager), CWinApp.cpp:409/420,
//   Globals.cpp:557, CMFCVisualManagerOffice2007.cpp:359/550, visualmanager/Thunks.cpp:1503.
extern "C" void MS_ABI impl__CleanUp_CMenuImages__SAXXZ();
extern "C" std::int32_t impl__m_bSDParamsModified_CDockingManager__1HA;
extern "C" std::uint32_t impl__m_Style_CMFCVisualManagerOffice2007__1W4Style_1_A;
extern "C" void* impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA;
extern "C" void MS_ABI impl__CleanStyle_CMFCVisualManagerOffice2007__SAXXZ();
extern "C" int MS_ABI impl__SetStyle_CMFCVisualManagerOffice2007__SAHW4Style_1_PEB_W_Z(int style, const wchar_t* lpszPath);
extern "C" void MS_ABI impl__SetDefaultManager_CMFCVisualManager__SAXPEAUCRuntimeClass___Z(void* pRTC);
extern "C" void* MS_ABI impl__GetITaskbarList_CWinApp__QEAAPEAUITaskbarList__XZ(void* pApp);
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);   // core/window/CWnd.cpp
extern "C" void* MS_ABI impl__GetITaskbarList3_CWinApp__QEAAPEAUITaskbarList3__XZ(void* pApp);
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];   // featurepack/CMFC_misc_stubs.cpp

// Symbol: ??1AFX_GLOBAL_DATA@@QEAA@XZ
// Retail mfc140 0x6a6a0.  CleanUp(), then the ten CFont subobjects are destroyed in
// reverse declaration order (0x238 fontMarlett down to 0x1a8 fontRegular), then the
// three CPens (0x148, 0x138, 0x128), then the eight CBrushes (0x118 down to 0xa8) --
// the last of which is the tail jump to ~CGdiObject at 0x1c7a0.  ~CGdiObject stores
// the CGdiObject vftable and calls CGdiObject::DeleteObject, so the observable effect
// per subobject is "delete the handle if there is one".  OpenMFC's blob has no
// vftables to restore, so only the handle half is reproduced.
extern "C" void MS_ABI impl___1AFX_GLOBAL_DATA__QEAA_XZ(void* pThis) {
    if (pThis == nullptr) return;
    AfxGlobalData* p = GD(pThis);

    impl__CleanUp_AFX_GLOBAL_DATA__QEAAXXZ(pThis);

    DeleteGdi(p->fontMarlett);              // 0x238
    DeleteGdi(p->fontSmall);                // 0x228
    DeleteGdi(p->fontVertCaption);          // 0x218
    DeleteGdi(p->fontVert);                 // 0x208
    DeleteGdi(p->fontDefaultGUIUnderline);  // 0x1f8
    DeleteGdi(p->fontUnderline);            // 0x1e8
    DeleteGdi(p->fontDefaultGUIBold);       // 0x1d8
    DeleteGdi(p->fontBold);                 // 0x1c8
    DeleteGdi(p->fontTooltip);              // 0x1b8
    DeleteGdi(p->fontRegular);              // 0x1a8

    DeleteGdi(p->penBarShadow);             // 0x148
    DeleteGdi(p->penBarFace);               // 0x138
    DeleteGdi(p->penHilite);                // 0x128

    DeleteGdi(p->brBarFace);                // 0x118
    DeleteGdi(p->brWindow);                 // 0x108
    DeleteGdi(p->brInactiveCaption);        // 0x0f8
    DeleteGdi(p->brActiveCaption);          // 0x0e8
    DeleteGdi(p->brBlack);                  // 0x0d8
    DeleteGdi(p->brLight);                  // 0x0c8
    DeleteGdi(p->brHilite);                 // 0x0b8
    DeleteGdi(p->brBtnFace);                // 0x0a8
}

// Symbol: ?CleanUp@AFX_GLOBAL_DATA@@QEAAXXZ
// Retail mfc140 0x6b840, transcribed verbatim.  Note that of the eight brushes only
// brLight (+0xc8) is released here, and only six of the ten fonts, in the order
// fontRegular, fontBold, fontUnderline, fontVert, fontVertCaption, fontTooltip.
extern "C" void MS_ABI impl__CleanUp_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AfxGlobalData* p = GD(pThis);

    if (p->brLight.m_hObject != nullptr) {    // 0x6b84d..0x6b85d
        DeleteGdi(p->brLight);
    }

    DeleteGdi(p->fontRegular);      // 0x1a8, 0x6b862
    DeleteGdi(p->fontBold);         // 0x1c8, 0x6b86e
    DeleteGdi(p->fontUnderline);    // 0x1e8, 0x6b87a
    DeleteGdi(p->fontVert);         // 0x208, 0x6b886
    DeleteGdi(p->fontVertCaption);  // 0x218, 0x6b892
    DeleteGdi(p->fontTooltip);      // 0x1b8, 0x6b89e

    if (p->m_bBufferedPaintInited) {                          // 0x6b8aa
        // 0x6b8b3..0x6b90d: a cached, EncodePointer'd uxtheme!BufferedPaintUnInit,
        // resolved with GetModuleHandleW (NOT LoadLibrary -- if uxtheme is not already
        // loaded there is nothing to un-init) + GetProcAddress.
        static void* s_pfnBufferedPaintUnInitEnc = nullptr;
        PFNBufferedPaintUnInit pfn = nullptr;
        if (s_pfnBufferedPaintUnInitEnc == nullptr) {
            HMODULE hUx = ::GetModuleHandleW(L"uxtheme.dll");
            if (hUx != nullptr) {
                pfn = reinterpret_cast<PFNBufferedPaintUnInit>(
                          reinterpret_cast<void*>(::GetProcAddress(hUx, "BufferedPaintUnInit")));
                s_pfnBufferedPaintUnInitEnc = ::EncodePointer(reinterpret_cast<void*>(pfn));
            }
        } else {
            pfn = reinterpret_cast<PFNBufferedPaintUnInit>(::DecodePointer(s_pfnBufferedPaintUnInitEnc));
        }
        if (pfn != nullptr) {
            pfn();
        }
        p->m_bBufferedPaintInited = FALSE;                    // 0x6b90d
    }

    if (p->m_hinstDwmapiDLL != nullptr) {                     // 0x6b917
        ::FreeLibrary(p->m_hinstDwmapiDLL);
        p->m_hinstDwmapiDLL = nullptr;
    }

    p->m_bEnableAccessibility = FALSE;                        // 0x6b934
}

// Symbol: ?CreateDitherBitmap@AFX_GLOBAL_DATA@@IEAAPEAUHBITMAP__@@PEAUHDC__@@@Z
// Retail mfc140 0x6b6c0.  Builds an 8x8 1bpp DIB whose two palette entries are
// clrBtnFace (retail reads the singleton directly at mfc140 0x3ba3a8, which is
// afxGlobalData+0x28; it is read through pThis here -- identical for the singleton) and
// ::GetSysColor(COLOR_BTNHIGHLIGHT), with a checkerboard bit pattern: the vectorised
// loop at 0x6b751..0x6b799 writes bits[i] = (i & 1) ? 0xaaaa5555 : 0x5555aaaa for
// i in 0..7 (the two 16-byte constants at 0x348ea0 / 0x348ec0 are 0x5555aaaa and
// 0xaaaa5555 replicated four times, selected by the parity mask).
extern "C" void* MS_ABI impl__CreateDitherBitmap_AFX_GLOBAL_DATA__IEAAPEAUHBITMAP____PEAUHDC_____Z(void* pThis, HDC hDC) {
    if (pThis == nullptr) return nullptr;
    AfxGlobalData* p = GD(pThis);

    struct {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD          bmiColors[16];
    } bmi;
    ::ZeroMemory(&bmi, sizeof(bmi));            // 0x68 bytes, 0x6b6ed

    bmi.bmiHeader.biSize     = sizeof(BITMAPINFOHEADER);   // 0x28
    bmi.bmiHeader.biWidth    = 8;
    bmi.bmiHeader.biHeight   = 8;
    bmi.bmiHeader.biPlanes   = 1;
    bmi.bmiHeader.biBitCount = 1;

    const COLORREF clrFace = p->clrBtnFace;
    bmi.bmiColors[0].rgbBlue  = GetBValue(clrFace);
    bmi.bmiColors[0].rgbGreen = GetGValue(clrFace);
    bmi.bmiColors[0].rgbRed   = GetRValue(clrFace);

    const COLORREF clrHi = ::GetSysColor(COLOR_BTNHIGHLIGHT);
    bmi.bmiColors[1].rgbBlue  = GetBValue(clrHi);
    bmi.bmiColors[1].rgbGreen = GetGValue(clrHi);
    bmi.bmiColors[1].rgbRed   = GetRValue(clrHi);

    DWORD bits[8];
    for (int i = 0; i < 8; ++i) {
        bits[i] = (i & 1) ? 0xaaaa5555u : 0x5555aaaau;
    }

    return ::CreateDIBitmap(hDC, &bmi.bmiHeader, CBM_INIT, bits,
                            reinterpret_cast<BITMAPINFO*>(&bmi), DIB_RGB_COLORS);
}

// Symbol: ?DrawParentBackground@AFX_GLOBAL_DATA@@QEAAHPEAVCWnd@@PEAVCDC@@PEAUtagRECT@@@Z
// Retail mfc140 0x6b9e0.  Import slots resolved with iat.py:
//   0x2c41a0 GDI32!CreateRectRgnIndirect, 0x2c5300 USER32!GetParent,
//   0x2c53d0 UxTheme!DrawThemeParentBackground, 0x2c5250 USER32!MapWindowPoints,
//   0x2c5378 USER32!SendMessage.
// Deviation: retail clips through CDC::SelectClipRgn (0x2a0d60), which applies the
// region to both m_hDC and m_hAttribDC and needs a live CRgn; OpenMFC's CRgn ctor/dtor
// exist only as impl__ thunks, so the region is handled as a bare HRGN on GetSafeHdc().
// Likewise CDC::OffsetWindowOrg (0x2a0c00) / CDC::SetWindowOrg (0x2a0ba0) apply to
// m_hDC (when it differs from m_hAttribDC) and to m_hAttribDC; here both go to m_hDC
// only.  For an ordinary CDC (m_hDC == m_hAttribDC) the effect is identical.
// Second deviation: retail dereferences pWnd (0x6ba35), the parent CWnd* (0x6bac2) and
// pDC (the clip-region calls and the origin calls) without null checks; all of those
// guards here are ours.  Retail's result is the low 32 bits of SendMessage's LRESULT.  ::SendMessage (not ::SendMessageW
// spelled out) is deliberate -- the slot reads as SendMessageA in the ANSI image and is
// SendMessageW in mfc140u, so UNICODE picks the right one.
extern "C" int MS_ABI impl__DrawParentBackground_AFX_GLOBAL_DATA__QEAAHPEAVCWnd__PEAVCDC__PEAUtagRECT___Z(
    void* pThis, CWnd* pWnd, CDC* pDC, RECT* lpRect) {
    (void)pThis;
    if (pWnd == nullptr) return FALSE;

    HDC  hDC   = (pDC != nullptr) ? pDC->GetSafeHdc() : nullptr;
    HWND hWnd  = pWnd->GetSafeHwnd();

    HRGN hRgn = nullptr;
    if (lpRect != nullptr) {                                  // 0x6ba0f
        hRgn = ::CreateRectRgnIndirect(lpRect);
        if (hDC != nullptr) ::SelectClipRgn(hDC, hRgn);
    }

    HWND  hParent = ::GetParent(hWnd);                        // 0x6ba35
    CWnd* pParent = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hParent);   // 0x6ba42

    BOOL bRes;
    if (::DrawThemeParentBackground(hWnd, hDC, lpRect) == S_OK) {   // 0x6ba5e
        bRes = TRUE;                                          // 0x6bae1
    } else {
        // 0x6ba6f..0x6badf: shift the DC's window origin into the parent's client
        // space, let the parent erase, then restore the origin.
        POINT pt = { 0, 0 };
        ::MapWindowPoints(hWnd, (pParent != nullptr) ? pParent->GetSafeHwnd() : nullptr, &pt, 1);

        POINT ptOldOrg = { 0, 0 };
        if (hDC != nullptr) ::OffsetWindowOrgEx(hDC, pt.x, pt.y, &ptOldOrg);

        bRes = (pParent != nullptr)
                   ? static_cast<BOOL>(::SendMessage(pParent->GetSafeHwnd(), WM_ERASEBKGND,
                                                     reinterpret_cast<WPARAM>(hDC), 0))
                   : FALSE;

        if (hDC != nullptr) ::SetWindowOrgEx(hDC, ptOldOrg.x, ptOldOrg.y, nullptr);
    }

    if (hDC != nullptr) ::SelectClipRgn(hDC, nullptr);        // 0x6bae3
    if (hRgn != nullptr) ::DeleteObject(hRgn);                // ~CRgn at 0x6baf6
    return bRes;
}

// Symbol: ?EnableAccessibilitySupport@AFX_GLOBAL_DATA@@QEAAXH@Z
// Retail mfc140 0x6bcd0 is two instructions: `mov %edx,0x278(%rcx); ret`.
extern "C" void MS_ABI impl__EnableAccessibilitySupport_AFX_GLOBAL_DATA__QEAAXH_Z(void* pThis, int bEnable) {
    if (pThis == nullptr) return;
    GD(pThis)->m_bEnableAccessibility = bEnable;
}

// Symbol: ?GetColor@AFX_GLOBAL_DATA@@QEAAKH@Z
// Retail mfc140 0x6bba0: a chain of compares mapping 19 COLOR_* indices onto
// cached members, with a tail jump to USER32!GetSysColor (slot 0x2c4dc0) for anything
// else.  Every case below is the offset the disassembly loads for that index.
extern "C" unsigned long MS_ABI impl__GetColor_AFX_GLOBAL_DATA__QEAAKH_Z(void* pThis, int nColor) {
    if (pThis == nullptr) return ::GetSysColor(nColor);
    AfxGlobalData* p = GD(pThis);
    switch (nColor) {
    case COLOR_ACTIVECAPTION:        return p->clrActiveCaption;          // 0x088
    case COLOR_INACTIVECAPTION:      return p->clrInactiveCaption;        // 0x08c
    case COLOR_WINDOW:               return p->clrWindow;                 // 0x078
    case COLOR_WINDOWFRAME:          return p->clrWindowFrame;            // 0x038
    case COLOR_MENUTEXT:             return p->clrMenuText;               // 0x084
    case COLOR_WINDOWTEXT:           return p->clrWindowText;             // 0x07c
    case COLOR_CAPTIONTEXT:          return p->clrCaptionText;            // 0x080
    case COLOR_ACTIVEBORDER:         return p->clrActiveBorder;           // 0x09c
    case COLOR_INACTIVEBORDER:       return p->clrInactiveBorder;         // 0x0a0
    case COLOR_HIGHLIGHT:            return p->clrHilite;                 // 0x048
    case COLOR_HIGHLIGHTTEXT:        return p->clrTextHilite;             // 0x04c
    case COLOR_BTNFACE:              return p->clrBtnFace;                // 0x028
    case COLOR_BTNSHADOW:            return p->clrBtnShadow;              // 0x02c
    case COLOR_GRAYTEXT:             return p->clrGrayedText;             // 0x044
    case COLOR_BTNTEXT:              return p->clrBtnText;                // 0x034
    case COLOR_INACTIVECAPTIONTEXT:  return p->clrInactiveCaptionText;    // 0x090
    case COLOR_BTNHIGHLIGHT:         return p->clrBtnHilite;              // 0x030
    case COLOR_3DDKSHADOW:           return p->clrBtnDkShadow;            // 0x03c
    case COLOR_3DLIGHT:              return p->clrBtnLight;               // 0x040
    default:                         return ::GetSysColor(nColor);        // 0x6bc72
    }
}

// Symbol: ?GetHandCursor@AFX_GLOBAL_DATA@@QEAAPEAUHICON__@@XZ
// Retail mfc140 0x6c560: lazily loads IDC_HAND (0x7f89) into m_hcurHand (+0x168) and
// returns it.  Slot 0x2c4ed0 is USER32!LoadCursorW.
extern "C" void* MS_ABI impl__GetHandCursor_AFX_GLOBAL_DATA__QEAAPEAUHICON____XZ(void* pThis) {
    if (pThis == nullptr) return nullptr;
    AfxGlobalData* p = GD(pThis);
    if (p->m_hcurHand == nullptr) {
        p->m_hcurHand = ::LoadCursorW(nullptr, IDC_HAND);
    }
    return p->m_hcurHand;
}

// Symbol: ?GetITaskbarList@AFX_GLOBAL_DATA@@QEAAPEAUITaskbarList@@XZ
// Retail mfc140 0x6b7e0: AfxGetModuleState() (0x1345b0), then the pointer at
// state+8 -- which is handed on as `this` to CWinApp::GetITaskbarList (0x1ce2f0), so it
// is m_pCurrentWinApp -- and NULL is returned when it is null.
// OpenMFC's module state is a plain 3-pointer struct with no vftable
// (detail/ManualSmallStubImplementationsSupport.h:632 / detail/RegcoreSupport.h:124),
// so m_pCurrentWinApp sits at +0x10 there; it is reached by name, not by offset.
extern "C" void* MS_ABI impl__GetITaskbarList_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList__XZ(void* pThis) {
    (void)pThis;
    LocalAFX_MODULE_STATE* pState = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if (pState == nullptr || pState->m_pCurrentWinApp == nullptr) return nullptr;
    return impl__GetITaskbarList_CWinApp__QEAAPEAUITaskbarList__XZ(pState->m_pCurrentWinApp);
}

// Symbol: ?GetITaskbarList3@AFX_GLOBAL_DATA@@QEAAPEAUITaskbarList3@@XZ
// Retail mfc140 0x6b810 -- identical to GetITaskbarList except for the tail jump,
// which goes to CWinApp::GetITaskbarList3 (0x1ce370).
extern "C" void* MS_ABI impl__GetITaskbarList3_AFX_GLOBAL_DATA__QEAAPEAUITaskbarList3__XZ(void* pThis) {
    (void)pThis;
    LocalAFX_MODULE_STATE* pState = impl__AfxGetModuleState__YAPEAVAFX_MODULE_STATE__XZ();
    if (pState == nullptr || pState->m_pCurrentWinApp == nullptr) return nullptr;
    return impl__GetITaskbarList3_CWinApp__QEAAPEAUITaskbarList3__XZ(pState->m_pCurrentWinApp);
}

// Symbol: ?GetNonClientMetrics@AFX_GLOBAL_DATA@@QEAAHAEAUtagNONCLIENTMETRICSW@@@Z
// Retail: mfc140 0x6c5e0 (ANSI, writes 0x154) / mfc140u 0x6c810 (Unicode, writes
// 0x1f4) -- this is one of the rare places where the two images differ in a constant,
// both being `sizeof(NONCLIENTMETRICS) - sizeof(info.iPaddedBorderWidth)`.  The caller's
// cbSize is left alone on Windows 7+; only on older systems is it forced down to the
// pre-Vista size.  The tail jump is USER32!SystemParametersInfo (import slot 0x2c4cc8
// in mfc140 = SystemParametersInfoA; slot 0x2c6c50 in mfc140u = SystemParametersInfoW).
// The pThis null guard is ours.
extern "C" int MS_ABI impl__GetNonClientMetrics_AFX_GLOBAL_DATA__QEAAHAEAUtagNONCLIENTMETRICSW___Z(
    void* pThis, NONCLIENTMETRICSW& info) {
    if (pThis != nullptr && !GD(pThis)->bIsWindows7) {
        info.cbSize = static_cast<UINT>(sizeof(NONCLIENTMETRICSW) - sizeof(info.iPaddedBorderWidth));
    }
    return ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, info.cbSize, &info, 0);
}

// Symbol: ?Initialize@AFX_GLOBAL_DATA@@QEAAXXZ
// Retail mfc140 0x6a5c0.  Guarded on m_bInitialized (+0).  Initialize itself never
// sets that flag: the inline GetGlobalData() in afxglobals.h:346 sets it to TRUE after
// Initialize returns.  So the guard makes a call AFTER a completed GetGlobalData() a
// no-op; it does NOT protect against re-entry while Initialize is still running.
// Slots: 0x2c4688 KERNEL32!VerSetConditionMask, 0x2c4690 KERNEL32!VerifyVersionInfo,
// 0x2c4c50 USER32!GetSystemMetrics.
//
// DELIBERATE DEVIATION -- `this` is ignored and the exported singleton is used.
// Retail works on `this`.  But five in-tree callers still declare this thunk with an
// EMPTY parameter list and call it with no argument, which leaves RCX undefined:
// core/frame/CFrameImpl.cpp:145/310, core/frame/CMDIFrameWndEx.cpp:241/492,
// core/frame/CMDIChildWndEx.cpp:159/256, core/frame/CMDIClientAreaWnd.cpp:167/694,
// featurepack/propertygrid/CMFCPropertyGridCtrl.cpp:130/324.  With a body that writes
// ~700 bytes through `this`, a stray RCX would corrupt whatever it points at.  The
// only AFX_GLOBAL_DATA instance this DLL supports is the exported blob (the exported
// constructor above does nothing), so every well-formed caller passes exactly that
// address and redirecting to it changes nothing for them.  Restore `GD(pThis)` once
// those callers pass the pointer.
extern "C" void MS_ABI impl__Initialize_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis) {
    (void)pThis;
    pThis = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    AfxGlobalData* p = GD(pThis);
    if (p->m_bInitialized) return;                            // 0x6a5db

    OSVERSIONINFOEXW osvi;
    ::ZeroMemory(&osvi, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);
    osvi.dwMajorVersion = 6;                                  // 0x6a5f7
    osvi.dwMinorVersion = 1;                                  // 0x6a604  -> Windows 7

    ULONGLONG mask = ::VerSetConditionMask(0, VER_MAJORVERSION, VER_GREATER_EQUAL);
    mask = ::VerSetConditionMask(mask, VER_MINORVERSION, VER_GREATER_EQUAL);
    p->bIsWindows7 = ::VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION, mask);

    p->bIsRemoteSession = ::GetSystemMetrics(SM_REMOTESESSION);   // 0x6a645, 0x1000

    impl__UpdateSysColors_AFX_GLOBAL_DATA__QEAAXXZ(pThis);    // 0x6a65f
    impl__UpdateFonts_AFX_GLOBAL_DATA__QEAAXXZ(pThis);        // 0x6a667
    impl__OnSettingChange_AFX_GLOBAL_DATA__QEAAXXZ(pThis);    // 0x6a66f

    p->m_bEnableAccessibility = TRUE;                         // 0x6a674 (+0x278)
}

// Symbol: ?IsDwmCompositionEnabled@AFX_GLOBAL_DATA@@QEAAHXZ
// Retail mfc140 0x6c260.  Slots: 0x2c45e8 KERNEL32!GetProcAddress, 0x2c46f0
// KERNEL32!EncodePointer, 0x2c46e8 KERNEL32!DecodePointer; 0x2c5bd0 is the CFG
// dispatcher (__guard_dispatch_icall_fptr), not an import.  Note the two distinct
// failure paths retail has: if dwmapi.dll cannot be loaded at all the cached answer
// is set to FALSE and the "already checked" flag IS set (0x6c2d5 -> 0x6c2df), but if
// DwmIsCompositionEnabled itself returns a failure HRESULT the flag is left clear
// (0x6c2d1 jumps straight to the return) so the next call retries.
extern "C" int MS_ABI impl__IsDwmCompositionEnabled_AFX_GLOBAL_DATA__QEAAHXZ(void* pThis) {
    (void)pThis;
    if (!g_bCheckedComposition) {                             // 0x6c266
        PFNDwmIsCompositionEnabled pfn = nullptr;
        if (g_pfnDwmIsCompEnabledEnc == nullptr) {            // 0x6c26f
            HMODULE hDwm = LoadSystemLibrary(L"dwmapi.dll");  // 0x6c282
            if (hDwm == nullptr) {
                g_bCompositionEnabled = FALSE;                // 0x6c2d5
                g_bCheckedComposition = 1;                    // 0x6c2df
                return g_bCompositionEnabled;
            }
            pfn = reinterpret_cast<PFNDwmIsCompositionEnabled>(
                      reinterpret_cast<void*>(::GetProcAddress(hDwm, "DwmIsCompositionEnabled")));
            g_pfnDwmIsCompEnabledEnc = ::EncodePointer(reinterpret_cast<void*>(pfn));
        } else {
            pfn = reinterpret_cast<PFNDwmIsCompositionEnabled>(::DecodePointer(g_pfnDwmIsCompEnabledEnc));
        }

        if (pfn == nullptr) {                                 // 0x6c2ba
            g_bCompositionEnabled = FALSE;                    // 0x6c2d5
            g_bCheckedComposition = 1;                        // 0x6c2df
        } else {
            // Retail passes the address of the cache itself (lea 0x3bcb00,%rcx), so a
            // failing call can still have written into it; that is reproduced here.
            if (pfn(reinterpret_cast<BOOL*>(&g_bCompositionEnabled)) != S_OK) {   // 0x6c2c9 / 0x6c2d1
                return g_bCompositionEnabled;                 // flag deliberately left clear: retry next time
            }
            g_bCheckedComposition = 1;                        // 0x6c2df
        }
    }
    return g_bCompositionEnabled;                             // 0x6c2e9
}

// Symbol: ?OnSettingChange@AFX_GLOBAL_DATA@@QEAAXXZ
// Retail mfc140 0x6aeb0, transcribed verbatim.  Slots: 0x2c4c50 USER32!GetSystemMetrics,
// 0x2c5368 USER32!SetRectEmpty, 0x2c4db8 USER32!EnumDisplayMonitors, 0x2c4cc8
// USER32!SystemParametersInfo.  It does NOT call UpdateFonts or UpdateSysColors.
extern "C" void MS_ABI impl__OnSettingChange_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AfxGlobalData* p = GD(pThis);

    p->m_bInSettingChange = TRUE;                             // 0x6aec2 (+0xc)

    p->m_sizeSmallIcon_cx = ::GetSystemMetrics(SM_CXSMICON);  // 0x6aec9 (0x31)
    p->m_sizeSmallIcon_cy = ::GetSystemMetrics(SM_CYSMICON);  // 0x6aed4 (0x32)

    ::SetRectEmpty(&p->m_rectVirtual);                        // 0x6aef5
    if (!::EnumDisplayMonitors(nullptr, nullptr, VirtualRectEnumProc,
                               reinterpret_cast<LPARAM>(&p->m_rectVirtual))) {   // 0x6af09
        ::SystemParametersInfoW(SPI_GETWORKAREA, 0, &p->m_rectVirtual, 0);       // 0x6af1e (0x30)
    }

    p->m_bMenuAnimation   = FALSE;                            // 0x6af39 (+0x26c)
    p->m_bMenuFadeEffect  = FALSE;                            // 0x6af3f (+0x270)
    g_bCheckedComposition = 0;                                // 0x6af45 -- same static
                                                              //   ResetCheckCompositionFlag clears

    if (!p->bIsRemoteSession) {                               // 0x6af24 / 0x6af4f
        ::SystemParametersInfoW(SPI_GETMENUANIMATION, 0, &p->m_bMenuAnimation, 0);   // 0x1002
        if (p->m_bMenuAnimation) {
            ::SystemParametersInfoW(SPI_GETMENUFADE, 0, &p->m_bMenuFadeEffect, 0);   // 0x1012
        }
    }

    p->m_nShellAutohideBars   = 0;                            // 0x6af83 (+0x2a4)
    p->m_bRefreshAutohideBars = TRUE;                         // 0x6af90 (+0x284)

    ::SystemParametersInfoW(SPI_GETKEYBOARDCUES, 0, &p->m_bSysUnderlineKeyboardShortcuts, 0); // 0x100a
    p->m_bUnderlineKeyboardShortcuts = p->m_bSysUnderlineKeyboardShortcuts;   // 0x6afb6

    p->m_bInSettingChange = FALSE;                            // 0x6afbc
}

// Symbol: ?ResetCheckCompositionFlag@AFX_GLOBAL_DATA@@QEAAXXZ
// Retail mfc140 0x6c6f0 is a single store of 0 into the same static
// IsDwmCompositionEnabled tests (mfc140 0x3bb96c), followed by ret.
extern "C" void MS_ABI impl__ResetCheckCompositionFlag_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis) {
    (void)pThis;
    g_bCheckedComposition = 0;
}

// Symbol: ?Resume@AFX_GLOBAL_DATA@@QEAAHXZ
// Retail mfc140 0x6c590.  Reads CMFCVisualManagerOffice2007::m_Style (mfc140 0x3b7130)
// BEFORE calling CMFCVisualManagerOffice2007::CleanStyle (0x199190), then -- only if
// CMFCVisualManager::m_pRTIDefault (0x3b7118) is non-null -- re-applies that style and
// re-installs the default manager.  Always returns TRUE.  Both statics are exported
// data this tree already defines (visualmanager/StaticData.cpp:34, core/runtime/
// StaticData.cpp:54).
//
// CALLER MISMATCH: the mangled name is __thiscall so this thunk takes pThis, but
// core/frame/CMDIFrameWndEx.cpp:242 forward declares it with an EMPTY parameter list
// and calls it with no argument at line 1945, which leaves RCX undefined.  A null
// check cannot catch a non-null garbage RCX, so -- exactly as in Initialize above and
// for the same reason -- `this` is ignored and the exported singleton is used.  Restore
// `GD(pThis)` once that caller passes the pointer.
extern "C" int MS_ABI impl__Resume_AFX_GLOBAL_DATA__QEAAHXZ(void* pThis) {
    (void)pThis;
    pThis = impl__afxGlobalData__3UAFX_GLOBAL_DATA__A;
    if (GD(pThis)->m_bEnableAccessibility) {
        GD(pThis)->m_bEnableAccessibility = TRUE;             // 0x6c596: normalises to 1
    }

    const int nStyle = static_cast<int>(impl__m_Style_CMFCVisualManagerOffice2007__1W4Style_1_A);
    impl__CleanStyle_CMFCVisualManagerOffice2007__SAXXZ();    // 0x6c5af

    if (impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA != nullptr) {   // 0x6c5b4
        impl__SetStyle_CMFCVisualManagerOffice2007__SAHW4Style_1_PEB_W_Z(nStyle, nullptr);
        impl__SetDefaultManager_CMFCVisualManager__SAXPEAUCRuntimeClass___Z(
            impl__m_pRTIDefault_CMFCVisualManager__1PEAUCRuntimeClass__EA);
    }
    return TRUE;                                              // 0x6c5d3
}

// Symbol: ?SetLayeredAttrib@AFX_GLOBAL_DATA@@QEAAHPEAUHWND__@@KEK@Z
// Retail mfc140 0x6bcb0 is a pure argument shuffle followed by a tail jump to import
// slot 0x2c4ce0, which iat.py resolves to USER32!SetLayeredWindowAttributes.
extern "C" int MS_ABI impl__SetLayeredAttrib_AFX_GLOBAL_DATA__QEAAHPEAUHWND____KEK_Z(
    void* pThis, HWND hwnd, unsigned long crKey, unsigned char bAlpha, unsigned long dwFlags) {
    (void)pThis;
    return ::SetLayeredWindowAttributes(hwnd, crKey, bAlpha, dwFlags);
}

// Symbol: ?SetMenuFont@AFX_GLOBAL_DATA@@QEAAHPEAUtagLOGFONTW@@H@Z
// Retail mfc140 0x6b4c0.  Slot 0x2c4170 is GDI32!CreateFontIndirect.  A NULL lpLogFont
// reaches AfxThrowInvalidArgException (0x225b80); here it just returns FALSE.
// Note that the horizontal path MUTATES the caller's LOGFONT (lfUnderline 1 then 0,
// lfWeight forced to FW_BOLD (700) then restored) -- that is retail behaviour, not ours.
// The FALSE returns test CGdiObject::Attach's result, i.e. "CreateFontIndirect gave
// NULL".  The pThis null guard is ours.
extern "C" int MS_ABI impl__SetMenuFont_AFX_GLOBAL_DATA__QEAAHPEAUtagLOGFONTW__H_Z(
    void* pThis, LOGFONTW* lpLogFont, int bHorz) {
    if (pThis == nullptr || lpLogFont == nullptr) return FALSE;   // 0x6b4cf
    AfxGlobalData* p = GD(pThis);

    if (!bHorz) {                                             // 0x6b4d8 -> 0x6b581
        ResetGdi(p->fontVert, ::CreateFontIndirectW(lpLogFont));
        if (p->fontVert.m_hObject == nullptr) return FALSE;   // 0x6b567
    } else {
        ResetGdi(p->fontRegular, ::CreateFontIndirectW(lpLogFont));      // 0x6b4e1
        if (p->fontRegular.m_hObject == nullptr) return FALSE;           // 0x6b504

        lpLogFont->lfUnderline = 1;                                      // 0x6b513
        ResetGdi(p->fontUnderline, ::CreateFontIndirectW(lpLogFont));    // 0x6b50c

        const LONG lfWeightSaved = lpLogFont->lfWeight;                  // 0x6b533
        lpLogFont->lfUnderline = 0;                                      // 0x6b540
        lpLogFont->lfWeight    = 700;                                    // 0x6b544
        ResetGdi(p->fontBold, ::CreateFontIndirectW(lpLogFont));         // 0x6b536
        lpLogFont->lfWeight    = lfWeightSaved;                          // 0x6b564

        if (p->fontBold.m_hObject == nullptr) return FALSE;              // 0x6b567
    }

    impl__UpdateTextMetrics_AFX_GLOBAL_DATA__IEAAXXZ(pThis);  // 0x6b56e
    return TRUE;                                              // 0x6b573
}

// Symbol: ?ShellCreateItemFromParsingName@AFX_GLOBAL_DATA@@QEAAJPEB_WPEAUIBindCtx@@AEBU_GUID@@PEAPEAX@Z
// Retail mfc140 0x6c6d0 drops `this` and tail-jumps to the internal forwarder at
// 0x1c90e8 (mfc140), which caches an EncodePointer'd shell32!SHCreateItemFromParsingName
// (GetModuleHandleW(L"shell32.dll") + GetProcAddress, cache at mfc140 0x3bcc68) and
// calls it.  It uses GetModuleHandleW ONLY -- no LoadLibrary: mfc140u does not import
// shell32, so this succeeds only when the application already has shell32 loaded, and
// if it is not loaded nothing is cached, so the next call probes again.  When the
// module or the entry point is missing the forwarder returns E_FAIL (0x80004005, the
// `mov $0x80004005,%eax` at 0x1c9167).  Reproduced exactly.
extern "C" long MS_ABI impl__ShellCreateItemFromParsingName_AFX_GLOBAL_DATA__QEAAJPEB_WPEAUIBindCtx__AEBU_GUID__PEAPEAX_Z(
    void* pThis, const wchar_t* pszPath, void* pbc, const GUID& riid, void** ppv) {
    (void)pThis;
    static void* s_pfnEnc = nullptr;
    PFNSHCreateItemFromParsingName pfn = nullptr;

    if (s_pfnEnc == nullptr) {
        HMODULE hShell = ::GetModuleHandleW(L"shell32.dll");
        if (hShell == nullptr) return E_FAIL;
        pfn = reinterpret_cast<PFNSHCreateItemFromParsingName>(
                  reinterpret_cast<void*>(::GetProcAddress(hShell, "SHCreateItemFromParsingName")));
        s_pfnEnc = ::EncodePointer(reinterpret_cast<void*>(pfn));
    } else {
        pfn = reinterpret_cast<PFNSHCreateItemFromParsingName>(::DecodePointer(s_pfnEnc));
    }

    if (pfn == nullptr) return E_FAIL;
    return pfn(pszPath, pbc, riid, ppv);
}

// Symbol: ?UpdateFonts@AFX_GLOBAL_DATA@@QEAAXXZ
// Retail mfc140 0x6a810.  Slots: 0x2c4168 GDI32!GetDeviceCaps, 0x2c4238 GDI32!DeleteObject,
// 0x2c4cc8 USER32!SystemParametersInfo, 0x2c5468 VCRUNTIME!memset, 0x2c4160
// GDI32!GetTextCharsetInfo, 0x2c4628 KERNEL32!lstrcpy, 0x2c4158 GDI32!EnumFontFamilies,
// 0x2c4170 GDI32!CreateFontIndirect, 0x2c5908 CRT!labs, 0x2c4210 GDI32!GetStockObject,
// 0x2c4250 GDI32!GetObject.  Face-name literals read out of .rdata: 0x33c738 "Segoe UI",
// 0x33c744 "Tahoma", 0x33c750 "MS Sans Serif", 0x33c760 "Marlett", 0x33c768 "Arial".
// Double constants: 0x348bb0 = 1.0, 0x348bc0 = 1.1, 0x348c10 = 3.0, 0x348d08 = 96.0f.
//
// NOT TRANSCRIBED: the loop at 0x6ad95..0x6adce is afxglobals.cpp's "notify toolbars
// about font changing" walk over the unexported CObList afxAllToolBars (its
// m_pNodeHead is read from mfc140 0x3ab098, between CMFCToolBar::m_lstUnpermittedCommands
// at 0x3ab058 and CMFCToolBarButton::m_lstProtectedCommands at 0x3ab0c8): for every
// toolbar with CWnd::FromHandlePermanent(m_hWnd) != NULL (call to 0x2891d0) it calls
// vftable slot 236 (byte 0x760), which CMFCToolBar.cpp identifies as
// CMFCToolBar::OnGlobalFontsChanged.  OpenMFC keeps no afxAllToolBars (the exported
// CMFCToolBar::GetAllToolbars rebuilds a list from a side table) and cannot dispatch
// that virtual; the only reachable body, the exported
// ?OnGlobalFontsChanged@CMFCToolBar@@ in featurepack/toolbar/CMFCToolBar.cpp, currently
// walks the buttons without calling anything, so the broadcast would have no
// observable effect and is skipped.  Everything else in this function is reproduced.
// The callback-compare and deviation notes are on FindFontFamilyProc above.
extern "C" void MS_ABI impl__UpdateFonts_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AfxGlobalData* p = GD(pThis);

    // CWindowDC dc(NULL) -> ::GetWindowDC(NULL), the screen DC.  Retail reads
    // GetDeviceCaps from m_hAttribDC and GetTextCharsetInfo from m_hDC; for a
    // CWindowDC both are the same handle.
    HDC hDC = ::GetWindowDC(nullptr);

    const int nLogPixelsY = (hDC != nullptr) ? ::GetDeviceCaps(hDC, LOGPIXELSY) : 96;
    p->m_dblRibbonImageScale = static_cast<double>(static_cast<float>(nLogPixelsY) / 96.0f);   // 0x6a874
    if (p->m_dblRibbonImageScale > 1.0 && 1.1 > p->m_dblRibbonImageScale) {                    // 0x6a884
        p->m_dblRibbonImageScale = 1.0;                                                        // 0x6a898
    }

    DeleteGdi(p->fontRegular);              // 0x6a8a9
    DeleteGdi(p->fontTooltip);              // 0x6a8cf
    DeleteGdi(p->fontBold);                 // 0x6a8f2
    DeleteGdi(p->fontDefaultGUIBold);       // 0x6a915
    DeleteGdi(p->fontUnderline);            // 0x6a938
    DeleteGdi(p->fontDefaultGUIUnderline);  // 0x6a95b
    DeleteGdi(p->fontVert);                 // 0x6a97f
    DeleteGdi(p->fontVertCaption);          // 0x6a9a2
    DeleteGdi(p->fontMarlett);              // 0x6a9c6
    DeleteGdi(p->fontSmall);                // 0x6a9ea

    NONCLIENTMETRICSW ncm;
    ::ZeroMemory(&ncm, sizeof(ncm));
    ncm.cbSize = static_cast<UINT>(p->bIsWindows7 ? sizeof(NONCLIENTMETRICSW)
                                                  : sizeof(NONCLIENTMETRICSW) - sizeof(ncm.iPaddedBorderWidth));
    ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);    // 0x6aa2e

    LOGFONTW lf;
    ::ZeroMemory(&lf, sizeof(lf));                                            // 0x6aa3f
    lf.lfCharSet = static_cast<BYTE>(::GetTextCharsetInfo(hDC, nullptr, 0));  // 0x6aa4f

    // 0x6aa59..0x6aa90: shrink the menu font by one point (unless it is already tiny,
    // in which case a flat 11 is used, or unless m_bDontReduceFontHeight is set),
    // preserving the sign convention of lfHeight.
    {
        const LONG lfHeightMenu = ncm.lfMenuFont.lfHeight;
        lf.lfWeight = ncm.lfMenuFont.lfWeight;
        lf.lfItalic = ncm.lfMenuFont.lfItalic;

        LONG nAbs = (lfHeightMenu < 0) ? -lfHeightMenu : lfHeightMenu;
        LONG nNew;
        if (nAbs <= 12) {
            nNew = 11;
        } else {
            nNew = p->m_bDontReduceFontHeight ? nAbs : (nAbs - 1);
        }
        lf.lfHeight = (lfHeightMenu < 0) ? -nNew : nNew;
    }

    ::lstrcpyW(lf.lfFaceName, ncm.lfMenuFont.lfFaceName);                     // 0x6aa9d

    if (!p->m_bUseSystemFont && ncm.lfMenuFont.lfCharSet <= SYMBOL_CHARSET) { // 0x6aaa3 / 0x6aaa9
        if (IsFontFamilyAvailable(hDC, L"Segoe UI")) {                        // 0x6aac4
            ::lstrcpyW(lf.lfFaceName, L"Segoe UI");
            lf.lfQuality = CLEARTYPE_QUALITY;                                 // 0x6aae0 (lf+0x1a = 5)
        } else if (IsFontFamilyAvailable(hDC, L"Tahoma")) {                   // 0x6aafc
            ::lstrcpyW(lf.lfFaceName, L"Tahoma");
        } else {
            ::lstrcpyW(lf.lfFaceName, L"MS Sans Serif");                      // 0x6ab12
        }
    }

    ResetGdi(p->fontRegular, ::CreateFontIndirectW(&lf));                     // 0x6ab24

    // 0x6ab35..0x6ab80: fontSmall == two thirds of the regular height, sign preserved.
    {
        const LONG lfHeightRegular = lf.lfHeight;
        const long nAbs = ::labs(lfHeightRegular);
        const int  nSmall = static_cast<int>((static_cast<double>(nAbs) + 1.0) * 2 / 3.0);
        lf.lfHeight = (lfHeightRegular < 0) ? -nSmall : nSmall;
        ResetGdi(p->fontSmall, ::CreateFontIndirectW(&lf));
        lf.lfHeight = lfHeightRegular;                                        // 0x6ab85
    }

    // 0x6ab89..0x6abdc: the tooltip font takes the STATUS font's weight/italic.
    {
        NONCLIENTMETRICSW ncm2;
        ::ZeroMemory(&ncm2, sizeof(ncm2));
        ncm2.cbSize = ncm.cbSize;
        ::SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm2.cbSize, &ncm2, 0);
        lf.lfItalic = ncm2.lfStatusFont.lfItalic;
        lf.lfWeight = ncm2.lfStatusFont.lfWeight;
        ResetGdi(p->fontTooltip, ::CreateFontIndirectW(&lf));
    }

    lf.lfItalic    = ncm.lfMenuFont.lfItalic;                                 // 0x6abe1
    lf.lfWeight    = ncm.lfMenuFont.lfWeight;                                 // 0x6abe8
    lf.lfUnderline = 1;                                                       // 0x6abef
    ResetGdi(p->fontUnderline, ::CreateFontIndirectW(&lf));                   // 0x6ac02

    lf.lfUnderline = 0;                                                       // 0x6ac0e
    lf.lfWeight    = 700;                                                     // 0x6ac13
    ResetGdi(p->fontBold, ::CreateFontIndirectW(&lf));                        // 0x6ac29

    // 0x6ac35..0x6ac83: Marlett, the Windows menu-symbol font.
    {
        const BYTE bCharSetSaved = lf.lfCharSet;
        lf.lfCharSet = SYMBOL_CHARSET;
        lf.lfWeight  = 0;
        lf.lfHeight  = ::GetSystemMetrics(SM_CYMENUCHECK) - 1;                // 0x48
        ::lstrcpyW(lf.lfFaceName, L"Marlett");
        ResetGdi(p->fontMarlett, ::CreateFontIndirectW(&lf));
        lf.lfCharSet = bCharSetSaved;
    }

    // 0x6ac87..0x6ad86: everything from here is derived from the stock DEFAULT_GUI_FONT
    // and is skipped entirely if GetStockObject or GetObject fails.
    HFONT hDefaultGui = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));   // 0x6ac98
    if (hDefaultGui != nullptr && ::GetObjectW(hDefaultGui, sizeof(LOGFONTW), &lf) != 0) {
        lf.lfOrientation = 900;                                               // 0x6acd0
        lf.lfEscapement  = 2700;                                              // 0x6acd5
        lf.lfHeight      = ncm.lfMenuFont.lfHeight;                           // 0x6acdd
        lf.lfWeight      = ncm.lfMenuFont.lfWeight;                           // 0x6ace4
        lf.lfItalic      = ncm.lfMenuFont.lfItalic;                           // 0x6aceb
        ::lstrcpyW(lf.lfFaceName, L"Arial");                                  // 0x6acfe
        ResetGdi(p->fontVert, ::CreateFontIndirectW(&lf));                    // 0x6ad12

        lf.lfEscapement = 900;                                                // 0x6ad1a
        ResetGdi(p->fontVertCaption, ::CreateFontIndirectW(&lf));             // 0x6ad2d

        ::GetObjectW(hDefaultGui, sizeof(LOGFONTW), &lf);                     // 0x6ad42, reload
        lf.lfUnderline = 1;                                                   // 0x6ad48
        ResetGdi(p->fontDefaultGUIUnderline, ::CreateFontIndirectW(&lf));     // 0x6ad5b

        lf.lfUnderline = 0;                                                   // 0x6ad63
        lf.lfWeight    = 700;                                                 // 0x6ad68
        ResetGdi(p->fontDefaultGUIBold, ::CreateFontIndirectW(&lf));          // 0x6ad7e
        // Retail then runs ~CFont over its local CFont holding the STOCK handle
        // (0x6add5); ::DeleteObject on a stock object is a no-op, so nothing is done.
    }

    impl__UpdateTextMetrics_AFX_GLOBAL_DATA__IEAAXXZ(pThis);                  // 0x6ad90
    // (the 0x6ad95 relayout broadcast is skipped -- see the header comment)

    if (hDC != nullptr) ::ReleaseDC(nullptr, hDC);                            // ~CWindowDC, 0x6ade5
}

// Symbol: ?UpdateSysColors@AFX_GLOBAL_DATA@@QEAAXXZ
// Retail mfc140 0x6afd0.  Slots: 0x2c4dc0 USER32!GetSysColor, 0x2c52f0
// USER32!GetSysColorBrush, 0x2c4258 GDI32!CreateSolidBrush, 0x2c4150 GDI32!CreatePen,
// 0x2c4148 GDI32!CreatePatternBrush, 0x2c4168 GDI32!GetDeviceCaps.
// Deviation: retail raises AfxThrowInvalidArgException (0x225b80) when
// GetSysColorBrush or CreateDitherBitmap returns NULL; OpenMFC has no exception plumbing
// on this path, so those failures are stored as NULL handles and execution continues.
extern "C" void MS_ABI impl__UpdateSysColors_AFX_GLOBAL_DATA__QEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AfxGlobalData* p = GD(pThis);

    // 0x6afdd..0x6b035: high-contrast detection from two sys colours each.
    p->m_bIsBlackHighContrast = (::GetSysColor(COLOR_3DLIGHT)   == RGB(255, 255, 255) &&
                                 ::GetSysColor(COLOR_BTNFACE)   == 0) ? 1 : 0;
    p->m_bIsWhiteHighContrast = (::GetSysColor(COLOR_3DDKSHADOW) == 0 &&
                                 ::GetSysColor(COLOR_BTNFACE)   == RGB(255, 255, 255)) ? 1 : 0;

    HDC hDC = ::GetWindowDC(nullptr);                          // CWindowDC dc(NULL), 0x6b042
    p->m_nBitsPerPixel = (hDC != nullptr) ? ::GetDeviceCaps(hDC, BITSPIXEL) : 0;   // 0x6b052

    p->clrBtnFace      = p->clrBarFace     = ::GetSysColor(COLOR_BTNFACE);        // 0x6b060
    p->clrBtnShadow    = p->clrBarShadow   = ::GetSysColor(COLOR_BTNSHADOW);      // 0x6b071
    p->clrBtnDkShadow  = p->clrBarDkShadow = ::GetSysColor(COLOR_3DDKSHADOW);     // 0x6b082
    p->clrBtnLight     = p->clrBarLight    = ::GetSysColor(COLOR_3DLIGHT);        // 0x6b093
    p->clrBtnHilite    = p->clrBarHilite   = ::GetSysColor(COLOR_BTNHIGHLIGHT);   // 0x6b0a4
    p->clrBtnText      = p->clrBarText     = ::GetSysColor(COLOR_BTNTEXT);        // 0x6b0b5
    p->clrGrayedText   = ::GetSysColor(COLOR_GRAYTEXT);                           // 0x6b0c6
    p->clrWindowFrame  = ::GetSysColor(COLOR_WINDOWFRAME);                        // 0x6b0d4
    p->clrHilite       = ::GetSysColor(COLOR_HIGHLIGHT);                          // 0x6b0e2
    p->clrTextHilite   = ::GetSysColor(COLOR_HIGHLIGHTTEXT);                      // 0x6b0f0
    p->clrWindow       = p->clrBarWindow   = ::GetSysColor(COLOR_WINDOW);         // 0x6b0fe
    p->clrWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);                         // 0x6b10f
    p->clrCaptionText  = ::GetSysColor(COLOR_CAPTIONTEXT);                        // 0x6b11d
    p->clrMenuText     = ::GetSysColor(COLOR_MENUTEXT);                           // 0x6b12e
    p->clrActiveCaption   = ::GetSysColor(COLOR_ACTIVECAPTION);                   // 0x6b13f
    p->clrInactiveCaption = ::GetSysColor(COLOR_INACTIVECAPTION);                 // 0x6b150
    p->clrActiveCaptionGradient   = ::GetSysColor(COLOR_GRADIENTACTIVECAPTION);   // 0x6b161
    p->clrInactiveCaptionGradient = ::GetSysColor(COLOR_GRADIENTINACTIVECAPTION); // 0x6b172
    p->clrActiveBorder    = ::GetSysColor(COLOR_ACTIVEBORDER);                    // 0x6b183
    p->clrInactiveBorder  = ::GetSysColor(COLOR_INACTIVEBORDER);                  // 0x6b194
    p->clrInactiveCaptionText = ::GetSysColor(COLOR_INACTIVECAPTIONTEXT);         // 0x6b1a5

    if (p->m_bIsBlackHighContrast) {                                              // 0x6b1b1
        p->clrHotLinkNormalText  = p->clrWindowText;
        p->clrHotLinkHoveredText = p->clrWindowText;
        p->clrHotLinkVisitedText = p->clrWindowText;
    } else {
        p->clrHotLinkNormalText  = ::GetSysColor(COLOR_HOTLIGHT);                 // 0x6b1d2 (0x1a)
        p->clrHotLinkHoveredText = 0x00ff0000;                                    // 0x6b1c8, blue
        p->clrHotLinkVisitedText = 0x00800080;                                    // 0x6b1c3, purple
    }

    p->hbrBtnShadow = ::GetSysColorBrush(COLOR_BTNSHADOW);                        // 0x6b1e1
    p->hbrBtnHilite = ::GetSysColorBrush(COLOR_BTNHIGHLIGHT);                     // 0x6b1f9
    p->hbrWindow    = ::GetSysColorBrush(COLOR_WINDOW);                           // 0x6b211

    ResetGdi(p->brBtnFace,         ::CreateSolidBrush(p->clrBtnFace));            // 0x6b229
    ResetGdi(p->brBarFace,         ::CreateSolidBrush(p->clrBarFace));            // 0x6b24c
    ResetGdi(p->brActiveCaption,   ::CreateSolidBrush(p->clrActiveCaption));      // 0x6b26f
    ResetGdi(p->brInactiveCaption, ::CreateSolidBrush(p->clrInactiveCaption));    // 0x6b295
    ResetGdi(p->brHilite,          ::CreateSolidBrush(p->clrHilite));             // 0x6b2bb
    ResetGdi(p->brBlack,           ::CreateSolidBrush(p->clrBtnDkShadow));        // 0x6b2de
    ResetGdi(p->brWindow,          ::CreateSolidBrush(p->clrWindow));             // 0x6b301
    ResetGdi(p->penHilite,     ::CreatePen(PS_SOLID, 1, p->clrHilite));           // 0x6b324
    ResetGdi(p->penBarFace,    ::CreatePen(PS_SOLID, 1, p->clrBarFace));          // 0x6b350
    ResetGdi(p->penBarShadow,  ::CreatePen(PS_SOLID, 1, p->clrBarShadow));        // 0x6b37c

    DeleteGdi(p->brLight);                                                        // 0x6b3a8
    if (p->m_nBitsPerPixel > 8) {                                                 // 0x6b3c3
        // 0x6b3cc..0x6b424: the exact midpoint between clrBtnFace and clrBtnHilite,
        // computed per channel as face + (hilite - face) / 2 with C truncation.
        const int rF = GetRValue(p->clrBtnFace),  gF = GetGValue(p->clrBtnFace),  bF = GetBValue(p->clrBtnFace);
        const int rH = GetRValue(p->clrBtnHilite), gH = GetGValue(p->clrBtnHilite), bH = GetBValue(p->clrBtnHilite);
        const COLORREF clrMid = RGB(static_cast<BYTE>(rF + (rH - rF) / 2),
                                    static_cast<BYTE>(gF + (gH - gF) / 2),
                                    static_cast<BYTE>(bF + (bH - bF) / 2));
        ResetGdi(p->brLight, ::CreateSolidBrush(clrMid));
    } else {
        // 0x6b42b..0x6b480: 8bpp and below get a dithered pattern brush instead.
        HBITMAP hbmDither = static_cast<HBITMAP>(
            impl__CreateDitherBitmap_AFX_GLOBAL_DATA__IEAAPEAUHBITMAP____PEAUHDC_____Z(pThis, hDC));
        if (hbmDither != nullptr) {
            ResetGdi(p->brLight, ::CreatePatternBrush(hbmDither));
            ::DeleteObject(hbmDither);                                            // ~CBitmap, 0x6b480
        }
    }

    impl__CleanUp_CMenuImages__SAXXZ();                                           // 0x6b485
    impl__m_bSDParamsModified_CDockingManager__1HA = 1;                           // 0x6b48a

    if (hDC != nullptr) ::ReleaseDC(nullptr, hDC);                                // ~CWindowDC, 0x6b496
}

// Symbol: ?UpdateTextMetrics@AFX_GLOBAL_DATA@@IEAAXXZ
// Retail mfc140 0x6b5b0.  Slot 0x2c4268 is GDI32!GetTextMetrics.  Everything happens
// inside `if (dc.SelectObject(&fontRegular) != NULL)` -- when the select fails, retail
// takes the branch to the AfxThrowInvalidArgException call at 0x6b6ac (inside this
// function; the callee's entry is 0x225b80) and throws, so no metric is written; here
// it simply returns.  The fontVert select is not checked in retail either.
// The "+2 for small fonts, +5 otherwise" constant pair is 0x6b611/0x6b618 (5 and 5-3).
extern "C" void MS_ABI impl__UpdateTextMetrics_AFX_GLOBAL_DATA__IEAAXXZ(void* pThis) {
    if (pThis == nullptr) return;
    AfxGlobalData* p = GD(pThis);

    HDC hDC = ::GetWindowDC(nullptr);                          // CWindowDC dc(NULL)
    if (hDC == nullptr) return;

    HGDIOBJ hOldFont = ::SelectObject(hDC, p->fontRegular.m_hObject);   // 0x6b5f2
    if (hOldFont != nullptr) {
        TEXTMETRICW tm;
        ::GetTextMetricsW(hDC, &tm);                           // 0x6b60b
        int nPad = (tm.tmHeight < 15) ? 2 : 5;                 // 0x6b61f
        p->m_nTextHeightHorz = tm.tmHeight       + nPad;       // 0x6b628
        p->m_nTextWidthHorz  = tm.tmMaxCharWidth + nPad;       // 0x6b631

        ::SelectObject(hDC, p->fontVert.m_hObject);            // 0x6b642
        ::GetTextMetricsW(hDC, &tm);                           // 0x6b64f
        nPad = (tm.tmHeight < 15) ? 2 : 5;                     // 0x6b658
        p->m_nTextHeightVert = tm.tmHeight       + nPad;       // 0x6b661
        p->m_nTextWidthVert  = tm.tmMaxCharWidth + nPad;       // 0x6b66c

        ::SelectObject(hDC, hOldFont);                         // 0x6b679
    }

    ::ReleaseDC(nullptr, hDC);                                 // ~CWindowDC, 0x6b683
}
