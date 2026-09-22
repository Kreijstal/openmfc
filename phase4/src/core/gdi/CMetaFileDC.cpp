// CMetaFileDC — OpenMFC implementation.
// Sources: gdicore.cpp

#define OPENMFC_APPCORE_IMPL

#include "detail/GdicoreSupport.h"
#include <cstddef>
#include <cwchar>

// ---------------------------------------------------------------------------
// Exported CMetaFileDC overrides (below the C++ members).
//
// Every body is transcribed from the retail mfc140u.dll export; the RVA cited
// at each definition is that function's ENTRY in mfc140u (resolved through the
// export table by ordinal).  Function bodies are byte-identical in the ANSI
// twin mfc140.dll, where e.g. AdjustCP sits at 0x203ad0 instead.
//
// Retail CDC layout used here (pinned below): m_hDC at +0x08 (the output /
// metafile DC), m_hAttribDC at +0x10 (the attribute DC used for queries).
//
// Imports were resolved from the mfc140u IAT (iatu.py), e.g. 0x1802c62d0 is
// GDI32!TextOutW, 0x1802c62e8 GDI32!GetTextAlign, 0x1802c6f08 USER32!DrawTextW.
//
// Like retail, the bodies do not NULL-check `this`.  Where retail calls
// AfxThrowInvalidArgException (RVA 0x227720, mfc140u) the thunk below calls
// the same export.
//
// CPoint / CSize results: retail writes them through the hidden return
// pointer in RDX (`movq $0x0,(%rdx)` ... `mov %rbx,%rax`) and returns that
// pointer in RAX, so those thunks take (this, pRet, ...) and return pRet.
// ---------------------------------------------------------------------------
static_assert(offsetof(CDC, m_hDC) == 0x08, "CDC::m_hDC at +0x08 (retail)");
static_assert(offsetof(CDC, m_hAttribDC) == 0x10, "CDC::m_hAttribDC at +0x10 (retail)");
static_assert(offsetof(TEXTMETRICW, tmOverhang) == 0x20,
              "TEXTMETRICW::tmOverhang at +0x20 (retail reads tm+0x20)");
// The CPoint/CSize thunks write through pRet as POINT/SIZE (retail: one 8-byte
// `movq $0x0,(%rdx)`, then the Win32 call fills it), so the layouts must agree.
static_assert(sizeof(CPoint) == sizeof(POINT) && offsetof(CPoint, y) == offsetof(POINT, y),
              "CPoint must be layout-compatible with POINT");
static_assert(sizeof(CSize) == sizeof(SIZE) && offsetof(CSize, cy) == offsetof(SIZE, cy),
              "CSize must be layout-compatible with SIZE");

extern "C" void MS_ABI impl__AfxThrowInvalidArgException__YAXXZ();      // detail/MfcExceptionsSupport.cpp
extern "C" void MS_ABI impl__ReleaseAttribDC_CDC__UEAAXXZ(CDC* pThis);  // core/gdi/CDC.cpp
extern "C" void MS_ABI impl__AdjustCP_CMetaFileDC__IEAAXH_Z(CMetaFileDC* pThis, int cx);


CMetaFileDC::CMetaFileDC() : CDC() {
}
CMetaFileDC::~CMetaFileDC() {
    // If DC is still open, close it
    if (m_hDC) {
        // Check if it's an enhanced metafile DC
        HENHMETAFILE hMF = CloseEnhMetaFile(m_hDC);
        if (hMF) {
            DeleteEnhMetaFile(hMF);
        }
        m_hDC = nullptr;
        m_hAttribDC = nullptr;
    }
}
int CMetaFileDC::Create(const wchar_t* lpszFilename) {
    // Windows metafiles (WMF) are legacy format
    // CreateMetaFileW returns a DC for recording metafile commands
    m_hDC = ::CreateMetaFileW(lpszFilename);
    m_hAttribDC = m_hDC;
    return m_hDC != nullptr;
}
int CMetaFileDC::CreateEnhanced(void* pDC, const wchar_t* lpszFilename,
                                 const RECT* lpBounds, const wchar_t* lpszDescription)
{
    HDC hRefDC = pDC ? static_cast<CDC*>(pDC)->m_hDC : nullptr;
    m_hDC = ::CreateEnhMetaFileW(hRefDC, lpszFilename, lpBounds, lpszDescription);
    m_hAttribDC = m_hDC;
    return m_hDC != nullptr;
}
void* CMetaFileDC::Close() {
    if (!m_hDC) return nullptr;

    // CloseMetaFile returns an HMETAFILE
    HMETAFILE hMF = ::CloseMetaFile(m_hDC);
    m_hDC = nullptr;
    m_hAttribDC = nullptr;
    return hMF;
}
void* CMetaFileDC::CloseEnhanced() {
    if (!m_hDC) return nullptr;

    // CloseEnhMetaFile returns an HENHMETAFILE
    HENHMETAFILE hMF = ::CloseEnhMetaFile(m_hDC);
    m_hDC = nullptr;
    m_hAttribDC = nullptr;
    return hMF;
}
// CMetaFileDC::AdjustCP(int cx) -- RVA 0x2059d0 (mfc140u).  Moves the current
// position of the attribute DC after text was drawn with TA_UPDATECP:
//   if (m_hAttribDC == NULL) return;
//   nAlign = ::GetTextAlign(m_hAttribDC) & (TA_CENTER|TA_RIGHT);   // & 6
//   if (nAlign == TA_CENTER) return;                               // 6
//   ::GetCurrentPositionEx(m_hAttribDC, &pt);
//   ::MoveToEx(m_hAttribDC, pt.x + (nAlign == TA_RIGHT ? -cx : cx), pt.y, NULL);
// Symbol: ?AdjustCP@CMetaFileDC@@IEAAXH@Z
extern "C" void MS_ABI impl__AdjustCP_CMetaFileDC__IEAAXH_Z(CMetaFileDC* pThis, int cx) {
    if (pThis->m_hAttribDC == nullptr) return;
    const UINT nAlign = ::GetTextAlign(pThis->m_hAttribDC) & (TA_CENTER | TA_RIGHT);
    if (nAlign == TA_CENTER) return;
    POINT pt;
    ::GetCurrentPositionEx(pThis->m_hAttribDC, &pt);
    const int dx = (nAlign == TA_RIGHT) ? -cx : cx;
    ::MoveToEx(pThis->m_hAttribDC, pt.x + dx, pt.y, nullptr);
}

// CMetaFileDC::DrawTextEx -- RVA 0x205b20 (mfc140u).  Same shape as DrawText
// (RVA 0x205a40, below), with lpDTParams forwarded to both ::DrawTextExW calls:
//   if (lpszString == NULL || lpRect == NULL) AfxThrowInvalidArgException();
//   nRet = ::DrawTextExW(m_hDC, lpszString, nCount, lpRect, nFormat, lpDTParams);
//   if (m_hAttribDC != NULL && (::GetTextAlign(m_hAttribDC) & TA_UPDATECP)
//       && !(nFormat & DT_CALCRECT)) {                      // bt $0xa
//       RECT rect = *lpRect;
//       nRet = ::DrawTextExW(m_hAttribDC, lpszString, nCount, &rect,
//                            nFormat | DT_CALCRECT | DT_SINGLELINE, lpDTParams);  // | 0x420
//       AdjustCP(rect.right - rect.left);
//   }
//   return nRet;
// Symbol: ?DrawTextExW@CMetaFileDC@@UEAAHPEA_WHPEAUtagRECT@@IPEAUtagDRAWTEXTPARAMS@@@Z
extern "C" int MS_ABI impl__DrawTextExW_CMetaFileDC__UEAAHPEA_WHPEAUtagRECT__IPEAUtagDRAWTEXTPARAMS___Z(
    CMetaFileDC* pThis, wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat,
    DRAWTEXTPARAMS* lpDTParams) {
    if (lpszString == nullptr || lpRect == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;  // not reached: the export throws
    }
    int nRet = ::DrawTextExW(pThis->m_hDC, lpszString, nCount, lpRect, nFormat, lpDTParams);
    if (pThis->m_hAttribDC != nullptr && (::GetTextAlign(pThis->m_hAttribDC) & TA_UPDATECP) &&
        !(nFormat & DT_CALCRECT)) {
        RECT rect = *lpRect;
        nRet = ::DrawTextExW(pThis->m_hAttribDC, lpszString, nCount, &rect,
                             nFormat | DT_CALCRECT | DT_SINGLELINE, lpDTParams);
        impl__AdjustCP_CMetaFileDC__IEAAXH_Z(pThis, rect.right - rect.left);
    }
    return nRet;
}

// CMetaFileDC::DrawText -- RVA 0x205a40 (mfc140u):
//   if (lpszString == NULL || lpRect == NULL) AfxThrowInvalidArgException();
//   nRet = ::DrawTextW(m_hDC, lpszString, nCount, lpRect, nFormat);
//   if (m_hAttribDC != NULL && (::GetTextAlign(m_hAttribDC) & TA_UPDATECP)
//       && !(nFormat & DT_CALCRECT)) {                      // bt $0xa
//       RECT rect = *lpRect;
//       nRet = ::DrawTextW(m_hAttribDC, lpszString, nCount, &rect,
//                          nFormat | DT_CALCRECT | DT_SINGLELINE);   // | 0x420
//       AdjustCP(rect.right - rect.left);
//   }
//   return nRet;
// Note the second call's result replaces the first (retail `mov %eax,%ebp`).
// Symbol: ?DrawTextW@CMetaFileDC@@UEAAHPEB_WHPEAUtagRECT@@I@Z
extern "C" int MS_ABI impl__DrawTextW_CMetaFileDC__UEAAHPEB_WHPEAUtagRECT__I_Z(
    CMetaFileDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat) {
    if (lpszString == nullptr || lpRect == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return 0;  // not reached: the export throws
    }
    int nRet = ::DrawTextW(pThis->m_hDC, lpszString, nCount, lpRect, nFormat);
    if (pThis->m_hAttribDC != nullptr && (::GetTextAlign(pThis->m_hAttribDC) & TA_UPDATECP) &&
        !(nFormat & DT_CALCRECT)) {
        RECT rect = *lpRect;
        nRet = ::DrawTextW(pThis->m_hAttribDC, lpszString, nCount, &rect,
                           nFormat | DT_CALCRECT | DT_SINGLELINE);
        impl__AdjustCP_CMetaFileDC__IEAAXH_Z(pThis, rect.right - rect.left);
    }
    return nRet;
}

// CMetaFileDC::Escape -- RVA 0x205c10 (mfc140u).  The escape always goes to
// the metafile DC; it is repeated on the attribute DC (and that result
// returned) only for the escape codes the retail compare tree at
// 0x205c4a..0x205d45 accepts, decoded to exactly this set:
//   3-8, 0x0c-0x0e, 0x10-0x12, 0x14-0x18, 0x1a-0x20, 0x22, 0x23,
//   0x100-0x103, 0x300-0x304, 0x1007, 0x1009, 0x100a.
//   nRet = ::Escape(m_hDC, nEscape, nCount, lpszInData, lpOutData);
//   if (m_hAttribDC == NULL) return nRet;
//   if (nEscape in set) return ::Escape(m_hAttribDC, nEscape, nCount, lpszInData, lpOutData);
//   return nRet;
// Symbol: ?Escape@CMetaFileDC@@UEAAHHHPEBDPEAX@Z
extern "C" int MS_ABI impl__Escape_CMetaFileDC__UEAAHHHPEBDPEAX_Z(
    CMetaFileDC* pThis, int nEscape, int nCount, const char* lpszInData, void* lpOutData) {
    const int nRet = ::Escape(pThis->m_hDC, nEscape, nCount, lpszInData, lpOutData);
    if (pThis->m_hAttribDC == nullptr) return nRet;
    switch (nEscape) {
    case 0x0003: case 0x0004: case 0x0005: case 0x0006: case 0x0007: case 0x0008:
    case 0x000c: case 0x000d: case 0x000e:
    case 0x0010: case 0x0011: case 0x0012:
    case 0x0014: case 0x0015: case 0x0016: case 0x0017: case 0x0018:
    case 0x001a: case 0x001b: case 0x001c: case 0x001d: case 0x001e: case 0x001f: case 0x0020:
    case 0x0022: case 0x0023:
    case 0x0100: case 0x0101: case 0x0102: case 0x0103:
    case 0x0300: case 0x0301: case 0x0302: case 0x0303: case 0x0304:
    case 0x1007: case 0x1009: case 0x100a:
        return ::Escape(pThis->m_hAttribDC, nEscape, nCount, lpszInData, lpOutData);
    default:
        return nRet;
    }
}

// CMetaFileDC::ExtTextOut -- RVA 0x2056f0 (mfc140u):
//   if (lpszString == NULL) AfxThrowInvalidArgException();
//   bResult = ::ExtTextOutW(m_hDC, x, y, nOptions, lpRect, lpszString, nCount, lpDxWidths);
//   if (bResult && m_hAttribDC != NULL && (::GetTextAlign(m_hAttribDC) & TA_UPDATECP)) {
//       if (nCount != 0 && lpDxWidths == NULL) AfxThrowInvalidArgException();
//       int nWidth = 0;
//       for (UINT i = 0; i < nCount; i++) nWidth += lpDxWidths[i];
//       AdjustCP(nWidth);
//   }
//   return bResult;
// Symbol: ?ExtTextOutW@CMetaFileDC@@UEAAHHHIPEBUtagRECT@@PEB_WIPEAH@Z
extern "C" int MS_ABI impl__ExtTextOutW_CMetaFileDC__UEAAHHHIPEBUtagRECT__PEB_WIPEAH_Z(
    CMetaFileDC* pThis, int x, int y, unsigned int nOptions, const RECT* lpRect,
    const wchar_t* lpszString, unsigned int nCount, int* lpDxWidths) {
    if (lpszString == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;  // not reached: the export throws
    }
    const BOOL bResult = ::ExtTextOutW(pThis->m_hDC, x, y, nOptions, lpRect, lpszString,
                                       nCount, lpDxWidths);
    if (bResult && pThis->m_hAttribDC != nullptr &&
        (::GetTextAlign(pThis->m_hAttribDC) & TA_UPDATECP)) {
        if (nCount != 0 && lpDxWidths == nullptr) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return FALSE;  // not reached: the export throws
        }
        int nWidth = 0;
        for (unsigned int i = 0; i < nCount; ++i) nWidth += lpDxWidths[i];
        impl__AdjustCP_CMetaFileDC__IEAAXH_Z(pThis, nWidth);
    }
    return bResult;
}

// CMetaFileDC::GetClipBox -- RVA 0x205610 (mfc140u).  Clipping queries go to
// the attribute DC (read unconditionally, even if NULL):
//   if (lpRect == NULL) AfxThrowInvalidArgException();
//   return ::GetClipBox(m_hAttribDC, lpRect);        // tail jump
// Symbol: ?GetClipBox@CMetaFileDC@@UEBAHPEAUtagRECT@@@Z
extern "C" int MS_ABI impl__GetClipBox_CMetaFileDC__UEBAHPEAUtagRECT___Z(
    const CMetaFileDC* pThis, RECT* lpRect) {
    if (lpRect == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return ERROR;  // not reached: the export throws
    }
    return ::GetClipBox(pThis->m_hAttribDC, lpRect);
}

// CMetaFileDC::OffsetViewportOrg -- RVA 0x205db0 (mfc140u).  ONE call, on the
// attribute DC when there is one, else on the metafile DC; the previous origin
// is written straight into the zero-initialised return object:
//   *pRet = CPoint(0, 0);
//   ::OffsetViewportOrgEx(m_hAttribDC != NULL ? m_hAttribDC : m_hDC, nWidth, nHeight, pRet);
//   return pRet;
// Symbol: ?OffsetViewportOrg@CMetaFileDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint* MS_ABI impl__OffsetViewportOrg_CMetaFileDC__UEAA_AVCPoint__HH_Z(
    CMetaFileDC* pThis, CPoint* pRet, int nWidth, int nHeight) {
    pRet->x = 0;
    pRet->y = 0;
    HDC hDC = pThis->m_hAttribDC != nullptr ? pThis->m_hAttribDC : pThis->m_hDC;
    ::OffsetViewportOrgEx(hDC, nWidth, nHeight, reinterpret_cast<POINT*>(pRet));
    return pRet;
}

// CMetaFileDC::PtVisible -- RVA 0x205630 (mfc140u):
//   return ::PtVisible(m_hAttribDC, x, y);            // tail jump
// Symbol: ?PtVisible@CMetaFileDC@@UEBAHHH@Z
extern "C" int MS_ABI impl__PtVisible_CMetaFileDC__UEBAHHH_Z(const CMetaFileDC* pThis, int x, int y) {
    return ::PtVisible(pThis->m_hAttribDC, x, y);
}

// CMetaFileDC::RectVisible -- the mfc140u export resolves (by ordinal) to RVA
// 0x3a60, an identical-COMDAT-folded body `mov $0x1,%eax; ret` shared with other
// exports.  CMetaFileDC's vftable (0x180324ff8, mfc140u) slot 24 also points at
// 0x3a60.  So retail answers TRUE without touching either DC.
// Symbol: ?RectVisible@CMetaFileDC@@UEBAHPEBUtagRECT@@@Z
extern "C" int MS_ABI impl__RectVisible_CMetaFileDC__UEBAHPEBUtagRECT___Z(
    const CMetaFileDC* /*pThis*/, const RECT* /*lpRect*/) {
    return TRUE;
}

// CMetaFileDC::ReleaseOutputDC -- the mfc140u export resolves (by ordinal) to
// RVA 0x27d0, a folded bare `ret` shared with other exports; CMetaFileDC's
// vftable (0x180324ff8, mfc140u) slot 8 also points at 0x27d0.  Retail does
// nothing here (the real afxext.h annotates this override "Release the Output
// DC -- Not allowed"), so this empty body IS the retail behaviour.
// Symbol: ?ReleaseOutputDC@CMetaFileDC@@MEAAXXZ
extern "C" void MS_ABI impl__ReleaseOutputDC_CMetaFileDC__MEAAXXZ(CMetaFileDC* /*pThis*/) {
}

// CMetaFileDC::ScaleViewportExt -- RVA 0x205e30 (mfc140u).  Same shape as
// SetViewportOrg: one call on m_hAttribDC, or m_hDC when there is none.
//   *pRet = CSize(0, 0);
//   ::ScaleViewportExtEx(m_hAttribDC != NULL ? m_hAttribDC : m_hDC,
//                        xNum, xDenom, yNum, yDenom, pRet);
//   return pRet;
// Symbol: ?ScaleViewportExt@CMetaFileDC@@UEAA?AVCSize@@HHHH@Z
extern "C" CSize* MS_ABI impl__ScaleViewportExt_CMetaFileDC__UEAA_AVCSize__HHHH_Z(
    CMetaFileDC* pThis, CSize* pRet, int xNum, int xDenom, int yNum, int yDenom) {
    pRet->cx = 0;
    pRet->cy = 0;
    HDC hDC = pThis->m_hAttribDC != nullptr ? pThis->m_hAttribDC : pThis->m_hDC;
    ::ScaleViewportExtEx(hDC, xNum, xDenom, yNum, yDenom, reinterpret_cast<SIZE*>(pRet));
    return pRet;
}

// CMetaFileDC::SetAttribDC -- RVA 0x2055a0 (mfc140u):
//   if (hDC != m_hDC) m_hAttribDC = hDC;          // CDC::SetAttribDC, inlined
//   if (m_hDC == m_hAttribDC) ReleaseAttribDC();   // virtual: vftable +0x38, slot 7
// Slot 7 of CMetaFileDC's vftable (0x180324ff8, mfc140u) is CDC::ReleaseAttribDC
// (RVA 0x2a2590, mfc140u: `movq $0x0,0x10(%rcx); ret`).
// DEVIATION: retail dispatches ReleaseAttribDC through the vtable.  Objects
// built by this DLL carry the GCC vtable generated from the OpenMFC
// declarations (include/openmfc/afxwin.h): CDC and CMetaFileDC declare no
// virtual except the destructor, so the vtable holds only CObject's virtuals
// plus the two Itanium destructor entries -- there is no ReleaseAttribDC slot,
// and a raw slot-7 call would read past the vtable.  We call the CDC::ReleaseAttribDC export
// directly; a client subclass that overrides ReleaseAttribDC is therefore not
// reached from here.
// Symbol: ?SetAttribDC@CMetaFileDC@@UEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetAttribDC_CMetaFileDC__UEAAXPEAUHDC_____Z(CMetaFileDC* pThis, HDC hDC) {
    if (hDC != pThis->m_hDC) pThis->m_hAttribDC = hDC;
    if (pThis->m_hDC == pThis->m_hAttribDC) impl__ReleaseAttribDC_CDC__UEAAXXZ(pThis);
}

// CMetaFileDC::SetOutputDC -- the mfc140u export resolves (by ordinal) to RVA
// 0x27d0, the same folded bare `ret` as ReleaseOutputDC; vftable
// (0x180324ff8, mfc140u) slot 6 points there too.  Retail ignores the call
// (the real afxext.h annotates this override "Set the Output DC -- Not
// allowed"), so this empty body IS the retail behaviour.
// Symbol: ?SetOutputDC@CMetaFileDC@@MEAAXPEAUHDC__@@@Z
extern "C" void MS_ABI impl__SetOutputDC_CMetaFileDC__MEAAXPEAUHDC_____Z(CMetaFileDC* /*pThis*/, HDC /*hDC*/) {
}

// CMetaFileDC::SetViewportExt -- RVA 0x205df0 (mfc140u):
//   *pRet = CSize(0, 0);
//   ::SetViewportExtEx(m_hAttribDC != NULL ? m_hAttribDC : m_hDC, x, y, pRet);
//   return pRet;
// Symbol: ?SetViewportExt@CMetaFileDC@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__SetViewportExt_CMetaFileDC__UEAA_AVCSize__HH_Z(
    CMetaFileDC* pThis, CSize* pRet, int x, int y) {
    pRet->cx = 0;
    pRet->cy = 0;
    HDC hDC = pThis->m_hAttribDC != nullptr ? pThis->m_hAttribDC : pThis->m_hDC;
    ::SetViewportExtEx(hDC, x, y, reinterpret_cast<SIZE*>(pRet));
    return pRet;
}

// CMetaFileDC::SetViewportOrg -- RVA 0x205d70 (mfc140u):
//   *pRet = CPoint(0, 0);
//   ::SetViewportOrgEx(m_hAttribDC != NULL ? m_hAttribDC : m_hDC, x, y, pRet);
//   return pRet;
// Symbol: ?SetViewportOrg@CMetaFileDC@@UEAA?AVCPoint@@HH@Z
extern "C" CPoint* MS_ABI impl__SetViewportOrg_CMetaFileDC__UEAA_AVCPoint__HH_Z(
    CMetaFileDC* pThis, CPoint* pRet, int x, int y) {
    pRet->x = 0;
    pRet->y = 0;
    HDC hDC = pThis->m_hAttribDC != nullptr ? pThis->m_hAttribDC : pThis->m_hDC;
    ::SetViewportOrgEx(hDC, x, y, reinterpret_cast<POINT*>(pRet));
    return pRet;
}

// CMetaFileDC::TabbedTextOut -- RVA 0x205790 (mfc140u).  Emulates tab
// expansion: each tab-free run is drawn with ::TextOutW on the metafile DC and
// measured with ::GetTextExtentPoint32W on the attribute DC.
//   if (m_hDC == NULL || m_hAttribDC == NULL || lpszString == NULL)
//       AfxThrowInvalidArgException();
//   if (nCount == -1) { nCount = wcslen(lpszString); if >= 0x7fffffff: throw same; }
//   *pRet = CSize(0, 0);  xStart = x;
//   nTabWidth = LOWORD(::GetTabbedTextExtentW(m_hAttribDC, L"\t", 1, 0, NULL));
//   nTabStop  = lpnTabStopPositions == NULL ? nTabWidth
//             : nTabPositions == 1 ? (lpnTabStopPositions[0] ? lpnTabStopPositions[0] : 1)
//             : 0;                                     // 0 = use the explicit stop table
//   while (nCount != 0) {
//       scan lpszTab from lpszString while nCount && *lpszTab != '\t' && *lpszTab != 0
//           (each step lpszTab++, nCount--);
//       nChars = lpszTab - lpszString;
//       ::TextOutW(m_hDC, x, y, lpszString, nChars);
//       ::GetTextExtentPoint32W(m_hAttribDC, lpszString, nChars, &size);  x += size.cx;
//       if (nCount == 0) break;
//       if (*lpszTab == 0) continue;           // lpszString NOT advanced: the je at
//                                              // 0x1802058f4 (mfc140u) re-enters the scan at 0x180205885
//       nCount--;  lpszString = lpszTab + 1;
//       if (nTabStop == 0) {
//           for (i = 0; i < nTabPositions; i++)
//               if (x < lpnTabStopPositions[i] + nTabOrigin)
//                   { x = lpnTabStopPositions[i] + nTabOrigin; goto next; }
//           if (i != nTabPositions) goto next;  // only possible for nTabPositions < 0
//           nTabStop = nTabWidth;               // table exhausted: uniform from now on
//           if (nTabStop == 0) goto next;
//       }
//       x = x - nTabOrigin;  x = x - x % nTabStop + nTabStop + nTabOrigin;
//     next: ;
//   }
//   if (m_hAttribDC != NULL && (::GetTextAlign(m_hAttribDC) & TA_UPDATECP)) {
//       ::GetTextMetricsW(m_hAttribDC, &tm);
//       AdjustCP(x - tm.tmOverhang - xStart);
//   }
//   pRet->cx = x - xStart;                     // cy stays 0 (only cx is stored)
//   return pRet;
// DEVIATION: when the run stops at an embedded NUL, retail loops back without
// advancing lpszString and re-draws the same run until nCount is used up.  If
// that run is EMPTY (the NUL is the first character) nCount never decreases and
// retail spins forever; we break out of the loop in exactly that case instead.
// Every other path matches retail, including the repeated re-draw.
// Symbol: ?TabbedTextOutW@CMetaFileDC@@UEAA?AVCSize@@HHPEB_WHHPEAHH@Z
extern "C" CSize* MS_ABI impl__TabbedTextOutW_CMetaFileDC__UEAA_AVCSize__HHPEB_WHHPEAHH_Z(
    CMetaFileDC* pThis, CSize* pRet, int x, int y, const wchar_t* lpszString, int nCount,
    int nTabPositions, int* lpnTabStopPositions, int nTabOrigin) {
    if (pThis->m_hDC == nullptr || pThis->m_hAttribDC == nullptr || lpszString == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return pRet;  // not reached: the export throws
    }
    if (nCount == -1) {
        const size_t len = ::wcslen(lpszString);
        if (len >= 0x7fffffff) {
            impl__AfxThrowInvalidArgException__YAXXZ();
            return pRet;  // not reached: the export throws
        }
        nCount = static_cast<int>(len);
    }
    pRet->cx = 0;
    pRet->cy = 0;
    const int xStart = x;
    const int nTabWidth = LOWORD(::GetTabbedTextExtentW(pThis->m_hAttribDC, L"\t", 1, 0, nullptr));
    int nTabStop = 0;
    if (lpnTabStopPositions == nullptr) {
        nTabStop = nTabWidth;
    } else if (nTabPositions == 1) {
        nTabStop = lpnTabStopPositions[0];
        if (nTabStop == 0) nTabStop = 1;
    }

    while (nCount != 0) {
        const wchar_t* lpszTab = lpszString;
        while (nCount != 0 && *lpszTab != L'\t' && *lpszTab != L'\0') {
            ++lpszTab;
            --nCount;
        }
        const int nChars = static_cast<int>(lpszTab - lpszString);
        ::TextOutW(pThis->m_hDC, x, y, lpszString, nChars);
        SIZE size = { 0, 0 };
        ::GetTextExtentPoint32W(pThis->m_hAttribDC, lpszString, nChars, &size);
        x += size.cx;
        if (nCount == 0) break;
        if (*lpszTab == L'\0') {
            if (nChars == 0) break;  // DEVIATION (see above): retail never terminates here
            continue;                // retail: re-scan from the same lpszString
        }
        --nCount;
        lpszString = lpszTab + 1;
        if (nTabStop == 0) {
            int i = 0;
            bool bPlaced = false;
            for (; i < nTabPositions; ++i) {
                const int nStop = lpnTabStopPositions[i] + nTabOrigin;
                if (x < nStop) {
                    x = nStop;
                    bPlaced = true;
                    break;
                }
            }
            if (bPlaced) continue;
            if (i != nTabPositions) continue;
            nTabStop = nTabWidth;
            if (nTabStop == 0) continue;
        }
        x -= nTabOrigin;
        x = x - x % nTabStop + nTabStop + nTabOrigin;
    }

    if (pThis->m_hAttribDC != nullptr && (::GetTextAlign(pThis->m_hAttribDC) & TA_UPDATECP)) {
        TEXTMETRICW tm;
        ::GetTextMetricsW(pThis->m_hAttribDC, &tm);
        impl__AdjustCP_CMetaFileDC__IEAAXH_Z(pThis, x - tm.tmOverhang - xStart);
    }
    pRet->cx = x - xStart;
    return pRet;
}

// CMetaFileDC::TextOut -- RVA 0x205640 (mfc140u):
//   if (lpszString == NULL) AfxThrowInvalidArgException();
//   bResult = ::TextOutW(m_hDC, x, y, lpszString, nCount);
//   if (bResult && m_hAttribDC != NULL && (::GetTextAlign(m_hAttribDC) & TA_UPDATECP)) {
//       ::GetTextExtentPoint32W(m_hAttribDC, lpszString, nCount, &size);
//       ::GetTextMetricsW(m_hAttribDC, &tm);
//       AdjustCP(size.cx - tm.tmOverhang);
//   }
//   return bResult;
// (nCount == -1 is NOT expanded here; retail passes it straight to ::TextOutW.)
// Symbol: ?TextOutW@CMetaFileDC@@UEAAHHHPEB_WH@Z
extern "C" int MS_ABI impl__TextOutW_CMetaFileDC__UEAAHHHPEB_WH_Z(
    CMetaFileDC* pThis, int x, int y, const wchar_t* lpszString, int nCount) {
    if (lpszString == nullptr) {
        impl__AfxThrowInvalidArgException__YAXXZ();
        return FALSE;  // not reached: the export throws
    }
    const BOOL bResult = ::TextOutW(pThis->m_hDC, x, y, lpszString, nCount);
    if (bResult && pThis->m_hAttribDC != nullptr &&
        (::GetTextAlign(pThis->m_hAttribDC) & TA_UPDATECP)) {
        SIZE size;
        ::GetTextExtentPoint32W(pThis->m_hAttribDC, lpszString, nCount, &size);
        TEXTMETRICW tm;
        ::GetTextMetricsW(pThis->m_hAttribDC, &tm);
        impl__AdjustCP_CMetaFileDC__IEAAXH_Z(pThis, size.cx - tm.tmOverhang);
    }
    return bResult;
}
