// CMFCStatusBar — OpenMFC implementation.
// Sources: cbarcore.cpp, mfccore.cpp
//
// Every body below was transcribed from the retail mfc140u.dll disassembly
// (14.51.36231, x64); each thunk cites the entry RVA of the function it
// transcribes.  All RVAs in this file are mfc140u addresses unless a comment
// says otherwise.
//
// ---------------------------------------------------------------------------
// The object this file runs on is NOT the retail object.  Read this first.
// ---------------------------------------------------------------------------
// Retail CMFCStatusBar derives from CPane (sizeof == 0x420, confirmed by the
// scalar deleting destructor at RVA 0x133ce0, which frees 0x420 bytes) and the
// retail constructor (RVA 0x133c70) lays its own members out past the
// 0x3F8-byte CPane sub-object:
//
//   +0x1B8  int     m_nCount            (CPane member: number of panes)
//   +0x1C0  void*   m_pData             (CPane member: CMFCStatusBarPaneInfo[])
//   +0x1C8  int     m_cxLeftBorder = 4  (CPane member, written by the ctor)
//   +0x1CC  int     m_cxRightBorder = 0
//   +0x1D0  int     m_cyTopBorder = 2
//   +0x1D4  int     m_cyBottomBorder = 0
//   +0x1D8  int     m_cxDefaultGap = 2  (CPane member; set by CPane::CPane 0x9f2f0,
//                                        untouched by the CMFCStatusBar ctor;
//                                        RecalcLayout 0x1359d0 reads it)
//   +0x104  DWORD   m_dwStyle           (CBasePane member; PreCreateWindow 0x133dd0,
//                                        SetTipText 0x134d10 and the inlined
//                                        CBasePane::SetPaneStyle at 0x8810 touch it)
//   +0x3F8  HFONT   m_hFont             (GetCurrentFont 0x136080, OnSetFont 0x135520)
//   +0x400  int     m_cxSizeBox         (RecalcLayout 0x1359d0 writes it; OnNcHitTest
//                                        0x135380 / OnSysCommand 0x135460 test it)
//   +0x404  CRect   m_rectSizeBox       (ctor SetRectEmpty; RecalcLayout writes;
//                                        DoPaint 0x134f60 reads)
//   +0x414  BOOL    m_bPaneDoubleClick  (EnablePaneDoubleClick 0x134e30 is
//                                        `mov %edx,0x414(%rcx)`)
//   +0x418  BOOL    m_bDrawExtendedArea (GetExtendedArea 0x136120 tests it)
//
// OpenMFC's CMFCStatusBar (include/openmfc/afxmfc.h:1745) is declared as
//   class CMFCStatusBar : public CStatusBar { ...; char _pad[64]; };
// i.e. a CControlBar-based object of 400 bytes.  It cannot hold the retail
// layout: +0x3F8 is one past its end, and its base class is not CPane.  So this
// file maps the retail fields onto the storage the OpenMFC object does have:
//
//   retail CPane::m_nCount / m_pData / m_cx*Border / m_cy*Border / m_cxDefaultGap
//       -> CControlBar::m_nCount / m_pData / m_cxLeftBorder ... (afxole.h:97-104)
//   retail CBasePane::m_dwStyle  -> CControlBar::m_dwStyle (afxole.h:107)
//   retail own members (+0x3F8..+0x41B, 36 bytes)
//       -> the 64-byte `_pad` blob at offset 336 (struct SbOwn below)
//
// The pane records themselves keep the retail CMFCStatusBarPaneInfo layout
// (0x68 bytes, struct SbPane below) because retail hands CMFCStatusBarPaneInfo*
// to client code (HitTest) and to the visual managers.
//
// What this remap cannot reproduce, and what is therefore deliberately NOT
// done here (each site says so again):
//   * retail calls CPane / CBasePane base handlers and vtable slots (CPane::
//     OnDestroy 0xa1780, CPane::OnLButtonDblClk 0xa1a50, CPane::OnStyleChanged
//     0xa2600, CBasePane::OnSize 0xc2a0, CPane::CalcInsideRect 0xa1950, CPane::
//     AllocElements 0xa18c0, the RecalcLayout/SetPaneText/OnDrawPane virtuals).
//     The object has no CPane vtable, so the pane-specific parts are replaced
//     by their CWnd-level equivalent (Default()) or by a local transcription
//     that reads the CControlBar fields; virtual self-calls go straight to the
//     sibling thunk in this file (non-virtual).
//   * CreateEx (0x133e20) registers the new bar with the parent frame
//     (CFrameWndEx::AddPane / CMDIFrameWndEx::AddPane / CDockingManager::
//     AddPane).  Registering a CControlBar-based object as a CBasePane would
//     make OpenMFC's docking code call CBasePane virtuals through the wrong
//     vtable, so the window is created but not registered.
// Fixing all of that needs the header change listed in headerRequests
// (derive from CPane, declare the retail members, sizeof 0x420).
// ---------------------------------------------------------------------------

#define OPENMFC_APPCORE_IMPL

#include "detail/CbarcoreSupport.h"
#include "detail/MfccoreSupport.h"

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <cwchar>
#include <commctrl.h>

// ---------------------------------------------------------------------------
// Sibling thunks used below.  BRIEFING S1: the C++ methods do not exist inside
// this DLL, only these exports.  Every definition was located before being
// declared here (file:line in the trailing comment).
// ---------------------------------------------------------------------------
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);                      // core/window/Thunks.cpp:1183
extern "C" int MS_ABI impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(
    CWnd* pThis, CREATESTRUCTW& cs);                                                        // core/window/CWnd.cpp:794
extern "C" void MS_ABI impl__OnDestroy_CWnd__IEAAXXZ(CWnd* pThis);                         // core/window/Thunks.cpp:1503
extern "C" int MS_ABI impl__EnableToolTips_CWnd__QEAAHH_Z(CWnd* pThis, int bEnable);       // core/window/Thunks.cpp:1213
extern "C" unsigned long MS_ABI impl__GetStyle_CWnd__QEBAKXZ(const CWnd* pThis);           // core/window/Thunks.cpp:1378
extern "C" unsigned long MS_ABI impl__GetExStyle_CWnd__QEBAKXZ(const CWnd* pThis);         // core/window/Thunks.cpp:1313
extern "C" void MS_ABI impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(
    const CWnd* pThis, RECT* lpRect);                                                       // core/window/Thunks.cpp:1163
extern "C" CWnd* MS_ABI impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd);          // core/window/CWnd.cpp:591
extern "C" CWnd* MS_ABI impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(HWND hWnd); // core/window/CWnd.cpp:915
extern "C" CFrameWnd* MS_ABI impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(
    const CWnd* pThis);                                                                     // core/window/Thunks.cpp:1353
extern "C" CFrameWnd* MS_ABI impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(
    const CWnd* pWnd);                                                                      // featurepack/CMFC_misc_stubs.cpp:711
extern "C" CWnd* MS_ABI impl__AfxGetMainWnd__YAPEAVCWnd__XZ();                             // detail/CWinAppSupport.cpp:123
extern "C" int MS_ABI impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
    CWnd* pThis, const wchar_t* lpszClassName, const wchar_t* lpszWindowName,
    unsigned long dwStyle, const RECT& rect, CWnd* pParentWnd, unsigned int nID,
    CCreateContext* pContext);                                                              // core/window/CWnd.cpp:603
extern "C" void MS_ABI impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
    CString* pRet, void* pThis, const wchar_t* lpszClassNamePrefix);                       // core/runtime/AFX_GLOBAL_DATA.cpp:15
extern "C" unsigned char impl__afxGlobalData__3UAFX_GLOBAL_DATA__A[720];                   // featurepack/CMFC_misc_stubs.cpp:3652
extern "C" unsigned char impl__afxGlobalUtils__3VCGlobalUtils__A[16];                      // featurepack/CMFC_misc_stubs.cpp:3665
extern "C" void* impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA;                         // core/runtime/StaticData.cpp (mirrored by detail/MfccoreSupport.cpp:608)
extern "C" void* impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA;                        // used the same way by featurepack/docking/CDockablePane.cpp:964
extern "C" void MS_ABI impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(
    RECT* lpRect);                                                                          // featurepack/menu/CMFCPopupMenu.cpp (defined there; being edited concurrently, so no line number)
extern "C" int MS_ABI impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
    const CObject* pThis, const CRuntimeClass* pClass);                                     // core/runtime/CObject.cpp:49
extern "C" CRuntimeClass* MS_ABI impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ();    // core/dialog/CDialog.cpp:89
extern "C" void* MS_ABI impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(
    unsigned int nID);                                                                      // featurepack/CMFC_misc_stubs.cpp:1251
extern "C" int MS_ABI impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
    CString* pThis, HINSTANCE hInst, UINT nID);                                             // core/collections/CStringT.cpp:283
extern "C" void MS_ABI impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(
    CWnd* pThis, CCmdTarget* pTarget, int bDisableIfNoHndler);                              // core/window/Thunks.cpp:1743
extern "C" LONGLONG MS_ABI impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CWnd* pThis, CPoint point, TOOLINFOW* pTI);                                       // core/window/CWnd.cpp:1314
extern "C" int MS_ABI impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(const CDC* pThis, RECT* lpRect); // core/gdi/CDC.cpp:419
extern "C" int MS_ABI impl__SetBkMode_CDC__QEAAHH_Z(CDC* pThis, int nBkMode);              // core/gdi/CDC.cpp:80
extern "C" unsigned long MS_ABI impl__SetTextColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long clr); // detail/GdicoreSupport.cpp:101
extern "C" unsigned long MS_ABI impl__SetBkColor_CDC__QEAAKK_Z(CDC* pThis, unsigned long clr);   // detail/GdicoreSupport.cpp:93
extern "C" int MS_ABI impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
    CDC* pThis, const wchar_t* lpszString, int nCount, RECT* lpRect, unsigned int nFormat); // detail/GdicoreSupport.cpp:140
extern "C" unsigned long MS_ABI impl__GetStatusBarPaneTextColor_CMFCVisualManager__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(
    CMFCVisualManager* pThis, CMFCStatusBar* pStatusBar, void* pPane);                      // featurepack/visualmanager/CMFCVisualManager.cpp:279

// Thunks defined further down in THIS file (retail reaches most of them through
// the CMFCStatusBar vtable; here they are called directly, non-virtually).
extern "C" void MS_ABI impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(CMFCStatusBar* pThis);
extern "C" int MS_ABI impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(
    CMFCStatusBar* pThis, int nIndex, const wchar_t* lpszNewText, int bUpdate);
extern "C" void MS_ABI impl__SetTipText_CMFCStatusBar__QEAAXHPEB_W_Z(
    CMFCStatusBar* pThis, int nIndex, const wchar_t* pszTipText);
extern "C" void MS_ABI impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHICON____H_Z(
    CMFCStatusBar* pThis, int nIndex, HICON hIcon, int bUpdate);
extern "C" void MS_ABI impl__SetPaneAnimation_CMFCStatusBar__QEAAXHPEAU_IMAGELIST__IH_Z(
    CMFCStatusBar* pThis, int nIndex, HIMAGELIST hImageList, unsigned int nFrameRate, int bUpdate);
extern "C" void MS_ABI impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(CMFCStatusBar* pThis, int nIndex);
extern "C" void MS_ABI impl__SetPaneStyle_CMFCStatusBar__QEAAXHI_Z(CMFCStatusBar* pThis, int nIndex, unsigned int nStyle);
extern "C" void MS_ABI impl__SetPaneWidth_CMFCStatusBar__QEAAXHH_Z(CMFCStatusBar* pThis, int nIndex, int cx);
extern "C" int MS_ABI impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(const CMFCStatusBar* pThis, unsigned int nIDFind);
extern "C" void* MS_ABI impl__HitTest_CMFCStatusBar__IEBAPEAVCMFCStatusBarPaneInfo__VCPoint___Z(
    const CMFCStatusBar* pThis, long long ptPacked);
extern "C" HFONT MS_ABI impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ(const CMFCStatusBar* pThis);
extern "C" void MS_ABI impl__OnDrawPane_CMFCStatusBar__MEAAXPEAVCDC__PEAVCMFCStatusBarPaneInfo___Z(
    CMFCStatusBar* pThis, CDC* pDC, void* pPane);
extern "C" int MS_ABI impl__CreateEx_CMFCStatusBar__QEAAHPEAVCWnd__KKI_Z(
    CMFCStatusBar* pThis, CWnd* pParentWnd, unsigned long dwCtrlStyle, unsigned long dwStyle, unsigned int nID);

namespace {

// ---- retail constants (afxext.h / afxres.h values as they appear in the code)
constexpr UINT  kSBPS_NOBORDERS   = 0x00000100;   // SetIndicators 0x1340e0: `orl $0x8000100` on pane 0
constexpr UINT  kSBPS_POPOUT      = 0x00000200;   // CMFCStatusBarCmdUI::SetCheck 0x135ca0: bit 9
constexpr UINT  kSBPS_DISABLED    = 0x04000000;   // CMFCStatusBarCmdUI::Enable 0x135c30: bit 26
constexpr UINT  kSBPS_STRETCH     = 0x08000000;   // _GetPanePtr 0x133be0: `testl $0x8000000,0x14(%rax)`
constexpr DWORD kSBARS_SIZEGRIP   = 0x00000100;   // CreateEx `bts $0x8,%esi`; RecalcLayout `bt $0x8`
constexpr DWORD kCBRS_TOOLTIPS    = 0x00000010;   // SetTipText 0x134d10: `orl $0x10,0x104(%rsi)`
constexpr DWORD kCBRS_ALL         = 0x0040FFFF;   // CreateEx: `and $0x40ffff,%edx`
constexpr DWORD kCBRS_ALIGN_ANY   = 0x0000F000;   // CBasePane::SetPaneStyle 0x8810: `andl $0xffff0fff`
constexpr DWORD kCBRS_GRIPPER     = 0x00400000;   // CPane::CalcInsideRect 0xa1950
constexpr DWORD kWS_EX_LAYOUTRTL  = 0x00400000;
constexpr int   kSizeBoxBorder    = 4;            // CalcFixedLayout 0x134e40: `add $0x4,%ecx`

// ---- CMFCStatusBarPaneInfo, retail layout (0x68 bytes; CommandToIndex steps
// `add $0x68,%rcx`).  Offsets from SetIndicators (0x1340e0), which zero-fills
// every field, and from the accessors named on each line.
struct SbPane {
    RECT      rect;                 // +0x00  GetItemRect 0x1343e0 (movups (%rax))
    UINT      nID;                  // +0x10  GetItemID 0x1343c0
    UINT      nStyle;               // +0x14  GetPaneStyle 0x134410
    int       cxText;               // +0x18  SetPaneWidth 0x1344d0 writes
    int       cxIcon;               // +0x1c  SetPaneIcon 0x1347d0 writes bmWidth
    int       cyIcon;               // +0x20  SetPaneIcon writes bmHeight
    COLORREF  clrText;              // +0x24  SetPaneTextColor 0x134c10
    COLORREF  clrBackground;        // +0x28  SetPaneBackgroundColor 0x134c50
    int       _pad2c;               // +0x2c  (alignment; retail never touches it)
    LPWSTR    lpszText;             // +0x30  SetPaneText 0x134720
    LPWSTR    lpszToolTip;          // +0x38  SetTipText 0x134d10
    HIMAGELIST hImage;              // +0x40  SetPaneAnimation 0x134a00 / SetPaneIcon
    int       nFrameCount;          // +0x48  SetPaneAnimation: ImageList_GetImageCount
    int       nCurrFrame;           // +0x4c  OnTimer 0x135280 increments it
    LONG      nProgressCurr;        // +0x50  GetPaneProgress 0x136060
    LONG      nProgressTotal;       // +0x54  EnablePaneProgressBar 0x134b40
    BOOL      bProgressText;        // +0x58  EnablePaneProgressBar
    COLORREF  clrProgressBar;       // +0x5c  EnablePaneProgressBar
    COLORREF  clrProgressBarDest;   // +0x60  EnablePaneProgressBar
    COLORREF  clrProgressText;      // +0x64  EnablePaneProgressBar
};
static_assert(sizeof(SbPane) == 0x68, "CMFCStatusBarPaneInfo is 0x68 bytes in retail");
static_assert(offsetof(SbPane, nID) == 0x10, "nID @0x10");
static_assert(offsetof(SbPane, nStyle) == 0x14, "nStyle @0x14");
static_assert(offsetof(SbPane, cxText) == 0x18, "cxText @0x18");
static_assert(offsetof(SbPane, cxIcon) == 0x1c, "cxIcon @0x1c");
static_assert(offsetof(SbPane, cyIcon) == 0x20, "cyIcon @0x20");
static_assert(offsetof(SbPane, clrText) == 0x24, "clrText @0x24");
static_assert(offsetof(SbPane, clrBackground) == 0x28, "clrBackground @0x28");
static_assert(offsetof(SbPane, lpszText) == 0x30, "lpszText @0x30");
static_assert(offsetof(SbPane, lpszToolTip) == 0x38, "lpszToolTip @0x38");
static_assert(offsetof(SbPane, hImage) == 0x40, "hImage @0x40");
static_assert(offsetof(SbPane, nFrameCount) == 0x48, "nFrameCount @0x48");
static_assert(offsetof(SbPane, nCurrFrame) == 0x4c, "nCurrFrame @0x4c");
static_assert(offsetof(SbPane, nProgressCurr) == 0x50, "nProgressCurr @0x50");
static_assert(offsetof(SbPane, nProgressTotal) == 0x54, "nProgressTotal @0x54");
static_assert(offsetof(SbPane, bProgressText) == 0x58, "bProgressText @0x58");
static_assert(offsetof(SbPane, clrProgressBar) == 0x5c, "clrProgressBar @0x5c");
static_assert(offsetof(SbPane, clrProgressBarDest) == 0x60, "clrProgressBarDest @0x60");
static_assert(offsetof(SbPane, clrProgressText) == 0x64, "clrProgressText @0x64");

// ---- the retail own-member block, kept inside OpenMFC's `_pad[64]` blob.
// Retail offsets are +0x3F8.. (see the file header); here the block starts at
// offsetof(CMFCStatusBar, _pad) == 336.  This is the one place where the
// offsets in this file are NOT the retail ones -- see headerRequests.
struct SbOwn {
    HFONT m_hFont;              // retail +0x3F8
    int   m_cxSizeBox;          // retail +0x400
    RECT  m_rectSizeBox;        // retail +0x404
    BOOL  m_bPaneDoubleClick;   // retail +0x414
    BOOL  m_bDrawExtendedArea;  // retail +0x418
};
// Measured with the phase4 mingw flags the blob starts at offset 332 -- the
// Itanium ABI folds it into CStatusBar's four bytes of tail padding -- so the
// block is placed at the first 8-byte boundary inside it (offset 336) to keep
// m_hFont naturally aligned; 336 + 36 <= 332 + 64.
constexpr size_t kPadOffset = offsetof(CMFCStatusBar, _pad);
constexpr size_t kOwnOffset = (kPadOffset + 7) & ~static_cast<size_t>(7);
static_assert(kOwnOffset + sizeof(SbOwn) <= kPadOffset + sizeof(CMFCStatusBar::_pad),
              "own members must fit the padding blob");
static_assert(kOwnOffset == 336, "padding blob moved; recheck SbOwn placement");
static_assert(sizeof(CMFCStatusBar) == 400, "CMFCStatusBar size changed; recheck the remap in the file header");
static_assert(offsetof(CControlBar, m_nCount) == 268 && offsetof(CControlBar, m_pData) == 272,
              "pane array storage moved");

inline SbOwn& Own(CMFCStatusBar* p) {
    return *reinterpret_cast<SbOwn*>(reinterpret_cast<char*>(p) + kOwnOffset);
}
inline const SbOwn& Own(const CMFCStatusBar* p) {
    return *reinterpret_cast<const SbOwn*>(reinterpret_cast<const char*>(p) + kOwnOffset);
}
inline SbPane* Panes(const CMFCStatusBar* p) { return reinterpret_cast<SbPane*>(p->m_pData); }

inline CPoint UnpackPoint(long long v) {
    return CPoint(static_cast<int>(static_cast<LONG>(v & 0xffffffff)),
                  static_cast<int>(static_cast<LONG>(static_cast<unsigned long long>(v) >> 32)));
}

// CMFCStatusBar::_GetPanePtr(int) const -- retail RVA 0x133be0 (not exported;
// reached from every pane accessor).  Transcribed:
//     if (nIndex == 255) {                       // "first stretchy pane"
//         if (m_nCount < 255 && m_nCount > 0)
//             for (i = 0; i < m_nCount; i++) {
//                 p = _GetPanePtr(i); ENSURE(p);   // throws at 0x133c65
//                 if (p->nStyle & SBPS_STRETCH) return p;
//             }
//     } else if (nIndex < 0) return NULL;
//     if (nIndex >= m_nCount || m_pData == NULL) return NULL;
//     return &m_pData[nIndex];
// (Retail reads m_nCount at +0x1B8 and m_pData at +0x1C0; here they are the
// CControlBar members.)
SbPane* GetPanePtr(const CMFCStatusBar* pThis, int nIndex)
{
    if (nIndex == 255) {
        if (pThis->m_nCount < 255 && pThis->m_nCount > 0) {
            for (int i = 0; i < pThis->m_nCount; ++i) {
                SbPane* p = GetPanePtr(pThis, i);
                if (p == nullptr) return nullptr;   // retail: AfxThrowInvalidArgException
                if (p->nStyle & kSBPS_STRETCH) return p;
            }
        }
    } else if (nIndex < 0) {
        return nullptr;
    }
    if (nIndex >= pThis->m_nCount) return nullptr;
    if (pThis->m_pData == nullptr) return nullptr;
    return Panes(pThis) + nIndex;
}

// CPane::AllocElements(int nElements, int cbElement) -- retail RVA 0xa18c0,
// transcribed (retail: m_nCount +0x1B8 / m_pData +0x1C0; here the CControlBar
// members, since OpenMFC's impl__AllocElements_CPane thunk reads the CPane
// offsets, which this object does not have):
//     ENSURE(nElements >= 0 && cbElement >= 0);
//     ENSURE(m_pData != NULL || m_nCount == 0);
//     void* pData = NULL;
//     if (nElements > 0) { ENSURE(cbElement > 0);
//                          if ((pData = calloc(nElements, cbElement)) == NULL) return FALSE; }
//     free(m_pData); m_pData = pData; m_nCount = nElements; return TRUE;
BOOL AllocElements(CMFCStatusBar* pThis, int nElements, int cbElement)
{
    if (nElements < 0 || cbElement < 0) return FALSE;                    // retail throws
    if (pThis->m_pData == nullptr && pThis->m_nCount != 0) return FALSE; // retail throws
    void* pData = nullptr;
    if (nElements > 0) {
        if (cbElement <= 0) return FALSE;                                // retail throws
        pData = std::calloc(static_cast<size_t>(nElements), static_cast<size_t>(cbElement));
        if (pData == nullptr) return FALSE;
    }
    std::free(pThis->m_pData);
    pThis->m_pData = static_cast<UINT*>(pData);
    pThis->m_nCount = nElements;
    return TRUE;
}

// CPane::CalcInsideRect(CRect& rect, BOOL bHorz) const -- retail RVA 0xa1950,
// transcribed:
//     DWORD dwStyle = GetPaneStyle();                       // vslot 114 -> m_dwStyle
//     if (!IsFloating() /*vslot 94*/ && !IsTabbed() /*vslot 93*/) {
//         if (dwStyle & CBRS_BORDER_LEFT)   rect.left++;
//         if (dwStyle & CBRS_BORDER_TOP)    rect.top++;
//         if (dwStyle & CBRS_BORDER_RIGHT)  rect.right--;
//         if (dwStyle & CBRS_BORDER_BOTTOM) rect.bottom--;
//     }
//     if (bHorz) {
//         rect.left += m_cxLeftBorder; rect.top += m_cyTopBorder;
//         rect.right -= m_cxRightBorder; rect.bottom -= m_cyBottomBorder;
//         if ((dwStyle & 0x400001) == CBRS_GRIPPER)
//             { if (GetExStyle() & WS_EX_LAYOUTRTL) rect.right -= 7; else rect.left += 7; }
//     } else {
//         rect.left += m_cyTopBorder; rect.top += m_cxLeftBorder;
//         rect.right -= m_cyBottomBorder; rect.bottom -= m_cxRightBorder;
//         if ((dwStyle & 0x400001) == CBRS_GRIPPER) rect.top += 7;
//     }
// Deviation: IsFloating/IsTabbed are CPane vtable slots this object does not
// have; a status bar is neither, so both are taken as FALSE.
void CalcInsideRect(const CMFCStatusBar* pThis, RECT& rect, BOOL bHorz)
{
    const DWORD dwStyle = pThis->m_dwStyle;
    if (dwStyle & 0x100) rect.left++;
    if (dwStyle & 0x200) rect.top++;
    if (dwStyle & 0x400) rect.right--;
    if (dwStyle & 0x800) rect.bottom--;
    if (bHorz) {
        rect.left   += pThis->m_cxLeftBorder;
        rect.top    += pThis->m_cyTopBorder;
        rect.right  -= pThis->m_cxRightBorder;
        rect.bottom -= pThis->m_cyBottomBorder;
        if ((dwStyle & 0x400001) == kCBRS_GRIPPER) {
            if (impl__GetExStyle_CWnd__QEBAKXZ(pThis) & kWS_EX_LAYOUTRTL) rect.right -= 7;
            else rect.left += 7;
        }
    } else {
        rect.left   += pThis->m_cyTopBorder;
        rect.top    += pThis->m_cxLeftBorder;
        rect.right  -= pThis->m_cyBottomBorder;
        rect.bottom -= pThis->m_cxRightBorder;
        if ((dwStyle & 0x400001) == kCBRS_GRIPPER) rect.top += 7;
    }
}

// afxGlobalData accessors.  Retail reads the fields straight out of
// ?afxGlobalData@@3UAFX_GLOBAL_DATA@@A (RVA 0x3c1620) after an inlined
// "if (!afxGlobalData.<+0>) afxGlobalData.Initialize()" guard.  OpenMFC exports
// that object as a zero-filled 720-byte blob whose Initialize() never runs, so
// the colours are substituted with the ::GetSysColor index the retail
// initialiser stores into each field (the table in visualmanager/
// CMFCVisualManager.cpp:983 is the authority) and a NULL font handle falls back
// to the stock GUI font, as featurepack/controls/CMFCButton.cpp:223 already does.
inline HFONT GD_fontRegular() {                                  // +0x1b0 (CFont fontRegular @+0x1a8, m_hObject @+0x1b0)
    HFONT h = nullptr;
    std::memcpy(&h, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A + 0x1b0, sizeof(h));
    if (h == nullptr) h = static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT));
    return h;
}
inline COLORREF GD_clrBtnFace() { return ::GetSysColor(COLOR_BTNFACE); }   // +0x28
inline COLORREF GD_clrBtnText() { return ::GetSysColor(COLOR_BTNTEXT); }   // +0x34
inline COLORREF GD_clrHilite()  { return ::GetSysColor(COLOR_HIGHLIGHT); } // +0x48

// The visual manager singleton.  Retail's inlined CMFCVisualManager::GetInstance
// (0x9774) first tests ?m_pVisManager@CMFCVisualManager@@1PEAV1@EA (0x3be3c0)
// and only creates the default manager when it is NULL.  The creating half is
// CMFCVisualManager::GetInstance(), a real C++ static in visualmanager/
// CMFCVisualManager.cpp:715 that BRIEFING S1 forbids calling from here (no
// impl__ thunk exists for it), so this reads the exported static only and the
// callers skip the visual-manager drawing while it is still NULL.
inline CMFCVisualManager* VisualManager() {
    return static_cast<CMFCVisualManager*>(impl__m_pVisManager_CMFCVisualManager__1PEAV1_EA);
}

// ---- MSVC-layout CMFCStatusBarCmdUI for OnUpdateCmdUI ----------------------
// OnUpdateCmdUI (0x135d50) builds a CMFCStatusBarCmdUI on its stack and hands
// it to the target's OnCmdMsg, so the object a client's ON_UPDATE_COMMAND_UI
// handler receives must have the retail CCmdUI layout and an MSVC-style vtable
// [Enable, SetCheck, SetRadio, SetText] (retail's devirtualisation guard at
// 0x135e46 compares vtable slot 0 with &CMFCStatusBarCmdUI::Enable).  OpenMFC's
// CCmdUI (afxole.h:68) has a different layout and a virtual destructor, and the
// CMFCStatusBarCmdUI exports in featurepack/controls/CMFCStatusBarCmdUI.cpp cast to it, so
// the three overrides are transcribed here as file-local MS_ABI functions.
struct MsCmdUI {
    const void* const* vfptr;   // +0x00
    UINT     m_nID;             // +0x08
    int      m_nIndex;          // +0x0c
    void*    m_pMenu;           // +0x10
    void*    m_pSubMenu;        // +0x18
    void*    m_pOther;          // +0x20  (the CMFCStatusBar)
    BOOL     m_bEnableChanged;  // +0x28
    BOOL     m_bContinueRouting;// +0x2c
    UINT     m_nIndexMax;       // +0x30
    void*    m_pParentMenu;     // +0x38
};
static_assert(sizeof(MsCmdUI) == 0x40, "retail CCmdUI is 0x40 bytes");
static_assert(offsetof(MsCmdUI, m_pOther) == 0x20 && offsetof(MsCmdUI, m_bEnableChanged) == 0x28 &&
              offsetof(MsCmdUI, m_nIndexMax) == 0x30, "retail CCmdUI offsets");

// CMFCStatusBarCmdUI::Enable(BOOL) -- retail RVA 0x135c30, transcribed:
//     m_bEnableChanged = TRUE;
//     CMFCStatusBar* pBar = (CMFCStatusBar*)m_pOther; ENSURE(pBar);
//     CMFCStatusBarPaneInfo* p = pBar->_GetPanePtr(m_nIndex);
//     UINT nStyle = (p ? p->nStyle : 0) & ~SBPS_DISABLED;
//     if (!bOn) nStyle |= SBPS_DISABLED;
//     pBar->SetPaneStyle(m_nIndex, nStyle);            // tail-jumps to 0x134430
void MS_ABI CmdUI_Enable(MsCmdUI* self, int bOn)
{
    self->m_bEnableChanged = TRUE;
    CMFCStatusBar* pBar = static_cast<CMFCStatusBar*>(self->m_pOther);
    if (pBar == nullptr) return;   // retail: AfxThrowInvalidArgException
    const SbPane* p = GetPanePtr(pBar, self->m_nIndex);
    UINT nStyle = (p ? p->nStyle : 0u) & ~kSBPS_DISABLED;
    if (!bOn) nStyle |= kSBPS_DISABLED;
    impl__SetPaneStyle_CMFCStatusBar__QEAAXHI_Z(pBar, self->m_nIndex, nStyle);
}
// CMFCStatusBarCmdUI::SetCheck(int) -- retail RVA 0x135ca0: same shape with
// SBPS_POPOUT (bit 9), set when nCheck != 0, and no m_bEnableChanged write.
void MS_ABI CmdUI_SetCheck(MsCmdUI* self, int nCheck)
{
    CMFCStatusBar* pBar = static_cast<CMFCStatusBar*>(self->m_pOther);
    if (pBar == nullptr) return;   // retail: AfxThrowInvalidArgException
    const SbPane* p = GetPanePtr(pBar, self->m_nIndex);
    UINT nStyle = (p ? p->nStyle : 0u) & ~kSBPS_POPOUT;
    if (nCheck) nStyle |= kSBPS_POPOUT;
    impl__SetPaneStyle_CMFCStatusBar__QEAAXHI_Z(pBar, self->m_nIndex, nStyle);
}
// CMFCStatusBarCmdUI does not override SetRadio: its vftable slot 2 is
// CCmdUI::SetRadio (retail RVA 0x1dea90, mfc140u), which calls the SetCheck
// virtual (slot 1) with (bOn != 0) and then runs a radio-bitmap path gated on
// (m_pMenu != NULL && m_pSubMenu == NULL) -- m_pMenu is always NULL for the
// status bar's CmdUI, so only the SetCheck forward is transcribed.
void MS_ABI CmdUI_SetRadio(MsCmdUI* self, int bOn) { CmdUI_SetCheck(self, bOn ? 1 : 0); }
// CMFCStatusBarCmdUI::SetText(LPCTSTR) -- retail RVA 0x135d10:
//     ENSURE(m_pOther); ((CMFCStatusBar*)m_pOther)->SetPaneText(m_nIndex, lpszText, TRUE); // vslot 202
void MS_ABI CmdUI_SetText(MsCmdUI* self, const wchar_t* lpszText)
{
    CMFCStatusBar* pBar = static_cast<CMFCStatusBar*>(self->m_pOther);
    if (pBar == nullptr) return;   // retail: AfxThrowInvalidArgException
    impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(pBar, self->m_nIndex, lpszText, TRUE);
}
const void* const g_msCmdUIVtbl[4] = {
    reinterpret_cast<const void*>(&CmdUI_Enable),
    reinterpret_cast<const void*>(&CmdUI_SetCheck),
    reinterpret_cast<const void*>(&CmdUI_SetRadio),
    reinterpret_cast<const void*>(&CmdUI_SetText),
};

// CCmdTarget::OnCmdMsg is MSVC vtable slot 5 (retail calls it as `mov
// 0x28(%rax),%rax` at 0x135e01); the target is a client-constructed frame with
// an MSVC vtable, so it is reached by raw slot, the way CMFCPopupMenu::OnCmdMsg
// in featurepack/menu/CMFCPopupMenu.cpp does (its VT_ONCMDMSG_SLOT; no line
// number, that file is being edited concurrently).
constexpr size_t kVtOnCmdMsgSlot = 5;
using OnCmdMsgFn = int (MS_ABI*)(void* pThis, unsigned int nID, int nCode, void* pExtra, void* pHandlerInfo);
constexpr int kCN_UPDATE_COMMAND_UI = -1;
constexpr int kCN_COMMAND = 0;

}  // namespace

// ---------------------------------------------------------------------------
// C++ constructor / destructor (the exported ??0/??1 thunks in featurepack/
// controls/Thunks.cpp placement-new / destroy through these).
// Retail ctor (RVA 0x133c70): CPane::CPane(); m_hFont = NULL; m_cxSizeBox = 0;
// m_bPaneDoubleClick = m_bDrawExtendedArea = 0; m_cxLeftBorder = 4;
// m_cxRightBorder = 0; m_cyTopBorder = 2; m_cyBottomBorder = 0;
// ::SetRectEmpty(&m_rectSizeBox) (import slot resolved: USER32!SetRectEmpty).
// It does NOT touch m_cxDefaultGap (+0x1D8), which therefore keeps the value
// CPane::CPane (RVA 0x9f2f0) stored: `lea 0x2(%rsi),%eax ; mov %eax,0x1d8(%rdi)`
// with rsi == 0, i.e. 2.  OpenMFC's CControlBar ctor (core/controlbar/
// CControlBar.cpp) initialises the same member to 0 and CStatusBar's ctor
// leaves it, so it is set here explicitly -- RecalcLayout below uses it as the
// inter-pane gap and the pane rectangles would otherwise differ from retail.
// Retail dtor (RVA 0x133d50): installs the vftable and tail-jumps to
// CPane::~CPane (0x9f490); it frees nothing itself (pane text/icons are
// released in OnDestroy).  OpenMFC's CStatusBar::~CStatusBar frees m_pData,
// which matches the calloc'd pane array.
// ---------------------------------------------------------------------------
CMFCStatusBar::CMFCStatusBar() {
    memset(_pad, 0, sizeof(_pad));
    m_cxLeftBorder = 4;
    m_cxRightBorder = 0;
    m_cyTopBorder = 2;
    m_cyBottomBorder = 0;
    m_cxDefaultGap = 2;   // inherited from CPane::CPane in retail, see above
    ::SetRectEmpty(&Own(this).m_rectSizeBox);
}
CMFCStatusBar::~CMFCStatusBar() {}

// CMFCStatusBar::SetPaneText(int, LPCTSTR, BOOL) -- retail RVA 0x134720
// (vtable slot 202), transcribed:
//     p = _GetPanePtr(nIndex); if (!p) return FALSE;
//     if (p->lpszText != NULL) {
//         if (lpszNewText != NULL && lstrcmpW(p->lpszText, lpszNewText) == 0) return TRUE;
//         free(p->lpszText);
//     } else if (lpszNewText == NULL || *lpszNewText == 0) return TRUE;
//     BOOL bOK = TRUE;
//     if (lpszNewText != NULL && *lpszNewText != 0) {
//         p->lpszText = _wcsdup(lpszNewText); if (!p->lpszText) bOK = FALSE;
//     } else p->lpszText = NULL;
//     if (bUpdate) InvalidatePaneContent(nIndex);
//     return bOK;
// (Rewritten from an earlier body that forwarded to the CStatusBar common
// control; the pane array is now the retail CMFCStatusBarPaneInfo array.)
// Symbol: ?SetPaneText@CMFCStatusBar@@UEAAHHPEB_WH@Z
extern "C" int MS_ABI impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(
    CMFCStatusBar* pThis, int nIndex, const wchar_t* lpszNewText, int bUpdate) {
    if (pThis == nullptr) return FALSE;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return FALSE;
    if (p->lpszText != nullptr) {
        if (lpszNewText != nullptr && ::lstrcmpW(p->lpszText, lpszNewText) == 0) return TRUE;
        std::free(p->lpszText);
    } else if (lpszNewText == nullptr || *lpszNewText == 0) {
        return TRUE;
    }
    BOOL bOK = TRUE;
    if (lpszNewText != nullptr && *lpszNewText != 0) {
        p->lpszText = ::_wcsdup(lpszNewText);
        if (p->lpszText == nullptr) bOK = FALSE;
    } else {
        p->lpszText = nullptr;
    }
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
    return bOK;
}
// CMFCStatusBar::GetPaneText(int) const -- retail RVA 0x1346a0.  A by-value
// CString return: `this` in RCX, the hidden result slot in RDX, nIndex in R8D
// (`mov %rdx,%rbx ; mov %r8d,%edx ; call _GetPanePtr`).  Transcribed:
//     p = _GetPanePtr(nIndex);
//     return CString(p != NULL && p->lpszText != NULL ? p->lpszText : L"");
// (An earlier body here took the result slot in RCX and `this` in RDX, which
// is the wrong order for a member function; fixed to the retail order.)
// Symbol: ?GetPaneText@CMFCStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" CString* MS_ABI impl__GetPaneText_CMFCStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const CMFCStatusBar* pThis, CString* __ret, int nIndex) {
    if (__ret == nullptr) return __ret;
    const SbPane* p = pThis ? GetPanePtr(pThis, nIndex) : nullptr;
    const wchar_t* psz = (p != nullptr && p->lpszText != nullptr) ? p->lpszText : L"";
    new (__ret) CString(psz);
    return __ret;
}
// CMFCStatusBar::GetPaneText(int, CString&) const -- retail RVA 0x134650:
//     p = _GetPanePtr(nIndex); if (!p) return;        // rString untouched
//     psz = p->lpszText ? p->lpszText : L"";
//     rString.SetString(psz, wcslen(psz));
// Symbol: ?GetPaneText@CMFCStatusBar@@QEBAXHAEAV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@@Z
extern "C" void MS_ABI impl__GetPaneText_CMFCStatusBar__QEBAXHAEAV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL___Z(
    const CMFCStatusBar* pThis, int nIndex, CString* rString) {
    if (pThis == nullptr || rString == nullptr) return;
    const SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    *rString = (p->lpszText != nullptr) ? p->lpszText : L"";
}
// CMFCStatusBar::EnablePaneProgressBar(int, long, BOOL, COLORREF, COLORREF, COLORREF)
// -- retail RVA 0x134b40, transcribed:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     p->bProgressText = bDisplayText;  p->clrProgressBar = clrBar;
//     p->clrProgressBarDest = clrProgressBarDest; p->nProgressTotal = nTotal;
//     p->nProgressCurr = 0; p->clrProgressText = clrProgressText;
//     if (clrProgressBarDest != (COLORREF)-1 && bDisplayText) p->bProgressText = FALSE;
//     InvalidatePaneContent(nIndex);
// (Rewritten from an earlier body that kept the state in a process-wide map
// keyed by nIndex only.)
// Symbol: ?EnablePaneProgressBar@CMFCStatusBar@@QEAAXHJHKKK@Z
extern "C" void MS_ABI impl__EnablePaneProgressBar_CMFCStatusBar__QEAAXHJHKKK_Z(
    CMFCStatusBar* pThis, int nIndex, long nTotal, int bDisplayText,
    unsigned long clrBar, unsigned long clrProgressBarDest, unsigned long clrProgressText) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    p->bProgressText = bDisplayText;
    p->clrProgressBar = clrBar;
    p->clrProgressBarDest = clrProgressBarDest;
    p->nProgressTotal = nTotal;
    p->nProgressCurr = 0;
    p->clrProgressText = clrProgressText;
    if (clrProgressBarDest != 0xFFFFFFFFul && bDisplayText != 0) p->bProgressText = FALSE;
    impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
}
// CMFCStatusBar::GetPaneProgress(int) const -- retail RVA 0x136060:
//     p = _GetPanePtr(nIndex); return p ? p->nProgressCurr : -1;
// Symbol: ?GetPaneProgress@CMFCStatusBar@@QEBAJH@Z
extern "C" long MS_ABI impl__GetPaneProgress_CMFCStatusBar__QEBAJH_Z(
    const CMFCStatusBar* pThis, int nIndex) {
    const SbPane* p = pThis ? GetPanePtr(pThis, nIndex) : nullptr;
    return p ? p->nProgressCurr : -1;
}
// CMFCStatusBar::SetPaneProgress(int, long, BOOL) -- retail RVA 0x134bb0:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     nCurr = max(0, nCurr); nCurr = min(nCurr, p->nProgressTotal);
//     if (p->nProgressCurr == nCurr) return;
//     p->nProgressCurr = nCurr;
//     if (bUpdate) InvalidatePaneContent(nIndex);
// Symbol: ?SetPaneProgress@CMFCStatusBar@@QEAAXHJH@Z
extern "C" void MS_ABI impl__SetPaneProgress_CMFCStatusBar__QEAAXHJH_Z(
    CMFCStatusBar* pThis, int nIndex, long nCurr, int bUpdate) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    if (nCurr < 0) nCurr = 0;
    if (nCurr > p->nProgressTotal) nCurr = p->nProgressTotal;
    if (p->nProgressCurr == nCurr) return;
    p->nProgressCurr = nCurr;
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
}
// CMFCStatusBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz) -- retail RVA
// 0x134e40.  CSize is returned through a hidden slot: `this` in RCX, the slot
// in RDX (`mov %rdx,%rsi`), bStretch in R8D (unused), bHorz in R9D.  Transcribed:
//     CClientDC dc(NULL);
//     HFONT hFont = GetCurrentFont();
//     HGDIOBJ hOld = hFont ? ::SelectObject(dc, hFont) : NULL;
//     TEXTMETRICW tm; ::GetTextMetricsW(dc, &tm);
//     if (hOld) ::SelectObject(dc, hOld);
//     int cyIconMax = 0;
//     for each pane: cyIconMax = max(cyIconMax, pane->cyIcon);
//     CRect rect; rect.SetRectEmpty(); CalcInsideRect(rect, bHorz);
//     size.cx = 32767;
//     size.cy = max(cyIconMax, tm.tmHeight) - rect.bottom + rect.top + 4;
// Symbol: ?CalcFixedLayout@CMFCStatusBar@@UEAA?AVCSize@@HH@Z
extern "C" CSize* MS_ABI impl__CalcFixedLayout_CMFCStatusBar__UEAA_AVCSize__HH_Z(
    CMFCStatusBar* pThis, CSize* pRet, int /*bStretch*/, int bHorz) {
    if (pRet == nullptr) return pRet;
    pRet->cx = 0;
    pRet->cy = 0;
    if (pThis == nullptr) return pRet;

    HDC hdc = ::GetDC(nullptr);
    HFONT hFont = impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ(pThis);
    HGDIOBJ hOld = hFont ? ::SelectObject(hdc, hFont) : nullptr;
    TEXTMETRICW tm;
    std::memset(&tm, 0, sizeof(tm));
    ::GetTextMetricsW(hdc, &tm);
    if (hOld) ::SelectObject(hdc, hOld);
    ::ReleaseDC(nullptr, hdc);

    int cyIconMax = 0;
    const SbPane* panes = Panes(pThis);
    for (int i = 0; i < pThis->m_nCount; ++i) {
        if (panes[i].cyIcon > cyIconMax) cyIconMax = panes[i].cyIcon;
    }

    RECT rect;
    ::SetRectEmpty(&rect);
    CalcInsideRect(pThis, rect, bHorz);

    pRet->cx = 32767;
    pRet->cy = std::max<int>(cyIconMax, tm.tmHeight) - rect.bottom + rect.top + kSizeBoxBorder;
    return pRet;
}

// CMFCStatusBar::CommandToIndex(UINT) const -- retail RVA 0x134390:
//     if (m_nCount <= 0 || m_pData == NULL) return -1;
//     for (i = 0; i < m_nCount; i++) if (m_pData[i].nID == nIDFind) return i;
//     return -1;
// Symbol: ?CommandToIndex@CMFCStatusBar@@QEBAHI@Z
extern "C" int MS_ABI impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(const CMFCStatusBar* pThis, unsigned int nIDFind) {
    if (pThis == nullptr || pThis->m_nCount <= 0 || pThis->m_pData == nullptr) return -1;
    const SbPane* panes = Panes(pThis);
    for (int i = 0; i < pThis->m_nCount; ++i) {
        if (panes[i].nID == nIDFind) return i;
    }
    return -1;
}

// CMFCStatusBar::Create(CWnd*, DWORD, UINT) -- retail RVA 0x133e00:
//     return CreateEx(pParentWnd, 0, dwStyle, nID);
// Symbol: ?Create@CMFCStatusBar@@QEAAHPEAVCWnd@@KI@Z
extern "C" int MS_ABI impl__Create_CMFCStatusBar__QEAAHPEAVCWnd__KI_Z(
    CMFCStatusBar* pThis, CWnd* pParentWnd, unsigned long dwStyle, unsigned int nID) {
    return impl__CreateEx_CMFCStatusBar__QEAAHPEAVCWnd__KKI_Z(pThis, pParentWnd, 0, dwStyle, nID);
}

// CMFCStatusBar::CreateEx(CWnd*, DWORD, DWORD, UINT) -- retail RVA 0x133e20.
// Transcribed (dwCtrlStyle is never read by the retail body):
//     ENSURE(AfxIsExtendedFrameClass(pParentWnd));          // 0x6c840, throws at 0x1340ce
//     SetPaneStyle(dwStyle & CBRS_ALL);                     // vslot 122 = CBasePane::SetPaneStyle (0x8810):
//                                                           //   m_dwStyle = (m_dwStyle & ~CBRS_ALIGN_ANY) | x
//     CRect rect; rect.SetRectEmpty();
//     m_dwControlBarStyle = 0;                              // CBasePane +0x108
//     if (pParentWnd->GetStyle() & WS_THICKFRAME) {         // bit 18
//         BOOL bSizeGrip = TRUE;
//         if (pParentWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWndEx))) {
//             CFrameWnd* pMain = pParentWnd->GetParentFrame();
//             if (pMain && pMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)) &&
//                 (pMain->+0x4574 || pMain->+0x4668)) bSizeGrip = FALSE;   // MDI-tab state
//         }
//         if (bSizeGrip) dwStyle |= SBARS_SIZEGRIP;
//     }
//     CString strClass = GetGlobalData()->RegisterWindowClass(L"Afx:StatusBar");  // 0x6bed0, literal at 0x342018
//     dwStyle |= WS_CLIPSIBLINGS;                           // bts $0x1a
//     if (!CWnd::Create(strClass, NULL, dwStyle, rect, pParentWnd, nID, NULL)) return FALSE;  // 0x28b690, direct
//     if (pParentWnd->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))          ((CFrameWndEx*)pParentWnd)->AddPane(this, TRUE);
//     else if (IsKindOf(CMDIFrameWndEx))                               ((CMDIFrameWndEx*)p)->AddPane(this, TRUE);
//     else if (IsKindOf(COleIPFrameWndEx) || IsKindOf(COleDocIPFrameWndEx)) (p+0x428)->CDockingManager::AddPane(this, TRUE, 0, 0);
//     else if (IsKindOf(CMDIChildWndEx))                               (p+0x3a8)->AddPane(...);
//     else if (IsKindOf(COleCntrFrameWndEx))                           (p+0x1e0)->AddPane(...);
//     else if (IsKindOf(RUNTIME_CLASS(CDialog)) &&
//              pParentWnd->m_hWnd == AfxGetMainWnd()->m_hWnd)         afxGlobalUtils.m_bDialogApp = TRUE;   // 0x3b1d00
//     return TRUE;
// Deviations, each forced by the object/infrastructure (see file header):
//   * the AfxIsExtendedFrameClass gate is not applied: OpenMFC's export is a
//     generated stub that returns 0, so applying it would reject every parent.
//   * m_dwControlBarStyle has no counterpart on the CControlBar-based object.
//   * the MDI-tab exception to SBARS_SIZEGRIP reads two CMDIFrameWndEx members
//     (+0x4574/+0x4668) OpenMFC's class does not declare; the grip is added
//     whenever the parent has WS_THICKFRAME.
//   * the AddPane registration is not done (wrong vtable for the docking code).
//   * OpenMFC's CWnd::Create thunk calls the CWnd PreCreateWindow, not this
//     class's virtual override; the override's only effect (the m_dwStyle
//     border fix-up, see PreCreateWindow below) is applied inline first.
// Symbol: ?CreateEx@CMFCStatusBar@@QEAAHPEAVCWnd@@KKI@Z
extern "C" int MS_ABI impl__CreateEx_CMFCStatusBar__QEAAHPEAVCWnd__KKI_Z(
    CMFCStatusBar* pThis, CWnd* pParentWnd, unsigned long /*dwCtrlStyle*/, unsigned long dwStyle, unsigned int nID) {
    if (pThis == nullptr || pParentWnd == nullptr) return FALSE;

    pThis->m_dwStyle = (pThis->m_dwStyle & ~kCBRS_ALIGN_ANY) | (dwStyle & kCBRS_ALL);

    RECT rect;
    ::SetRectEmpty(&rect);

    if (impl__GetStyle_CWnd__QEBAKXZ(pParentWnd) & WS_THICKFRAME) {
        dwStyle |= kSBARS_SIZEGRIP;
    }

    // PreCreateWindow's m_dwStyle fix-up (retail runs it from inside CWnd::Create).
    if ((pThis->m_dwStyle & 0xff00) == 0x8200) pThis->m_dwStyle &= 0xfffff07f;

    CString strClass;
    impl__RegisterWindowClass_AFX_GLOBAL_DATA__QEAA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__PEB_W_Z(
        &strClass, impl__afxGlobalData__3UAFX_GLOBAL_DATA__A, L"Afx:StatusBar");
    dwStyle |= WS_CLIPSIBLINGS;

    if (!impl__Create_CWnd__UEAAHPEB_W0KAEBUtagRECT__PEAV1_IPEAUCCreateContext___Z(
            pThis, static_cast<const wchar_t*>(strClass), nullptr, dwStyle, rect, pParentWnd, nID, nullptr)) {
        return FALSE;
    }

    // CDialog-hosted bar in the main window: afxGlobalUtils.m_bDialogApp = TRUE
    // (BOOL at +8 of the exported 16-byte CGlobalUtils object).
    if (impl__IsKindOf_CObject__QEBAHPEBUCRuntimeClass___Z(
            pParentWnd, impl__GetThisClass_CDialog__SAPEAUCRuntimeClass__XZ())) {
        CWnd* pMain = impl__AfxGetMainWnd__YAPEAVCWnd__XZ();
        HWND hMain = pMain ? pMain->m_hWnd : nullptr;
        if (pParentWnd->m_hWnd == hMain) {
            const BOOL bTrue = TRUE;
            std::memcpy(impl__afxGlobalUtils__3VCGlobalUtils__A + 8, &bTrue, sizeof(bTrue));
        }
    }
    return TRUE;
}

// CMFCStatusBar::DoPaint(CDC*) -- retail RVA 0x134f60.  Transcribed:
//     CRect rectClip; pDC->GetClipBox(rectClip);           // vslot 22; result unused afterwards
//     CRect rectClient; ::GetClientRect(m_hWnd, rectClient);   // likewise unused
//     CMemDC memDC(*pDC, this); CDC* pDC = &memDC.GetDC();  // 0x69f50 / +0x10,+0x20
//     CBasePane::DoPaint(pDC);                               // 0xb1b0: visual-manager bar background
//     HFONT hFont = GetCurrentFont();
//     HGDIOBJ hOldFont = hFont ? ::SelectObject(pDC->m_hDC, hFont) : NULL;
//     int nOldMode = pDC->SetBkMode(TRANSPARENT);            // 0x2a2860, direct
//     COLORREF clrOldText = pDC->SetTextColor(GetGlobalData()->clrBtnText);  // vslot 14, +0x34
//     COLORREF clrOldBk   = pDC->SetBkColor(GetGlobalData()->clrBtnFace);    // vslot 13, +0x28
//     for (i = 0; i < m_nCount; i++) OnDrawPane(pDC, &m_pData[i]);           // vslot 204
//     if (hOldFont) ::SelectObject(pDC->m_hDC, hOldFont);
//     if (!::IsRectEmpty(&m_rectSizeBox))
//         CMFCVisualManager::GetInstance()->OnDrawStatusBarSizeBox(pDC, this, m_rectSizeBox);  // vslot 28
//     pDC->SetTextColor(clrOldText); pDC->SetBkColor(clrOldBk); pDC->SetBkMode(nOldMode);
// Deviations: no CMemDC (its OpenMFC m_dcMem sub-object carries a NULL vptr,
// detail/CMemDCSupport.h:130, which the visual-manager virtuals below could
// dereference), so the panes are painted straight onto pDC; CBasePane::DoPaint
// is a no-op stub here (featurepack/docking/CBasePane.cpp:490).  Retail's
// CBasePane::DoPaint (0xb1b0) takes its own GetClipBox/GetClientRect and calls
// CMFCVisualManager::OnFillBarBackground(pDC, this, rectClient, rectClip, FALSE)
// (vslot 15); the base body (0x1845c0 mfc140u, 0x182bd0 in the ANSI image)
// ends, for a plain pane, in ::FillRect(pDC->m_hDC, rectClip -- or rectClient
// when the clip box is empty -- , afxGlobalData.brBarFace) where the HBRUSH
// sits at +0x120 of afxGlobalData (+0xb0, brBtnFace, for a dialog-control
// bar).  The colour initialiser (0x6b1c0) creates that brush from clrBarFace
// (+0x60), which it fills from ::GetSysColor(COLOR_BTNFACE) (`mov $0xf,%ebx`
// ... `mov %eax,0x60(%rdi)`), so the stand-in below fills the same rectangle
// with the COLOR_BTNFACE system brush.  OpenMFC's exported afxGlobalData is
// zero-filled, so the retail brush handle itself is unavailable.  The size box
// is only drawn once a visual manager instance exists.
// Symbol: ?DoPaint@CMFCStatusBar@@MEAAXPEAVCDC@@@Z
extern "C" void MS_ABI impl__DoPaint_CMFCStatusBar__MEAAXPEAVCDC___Z(CMFCStatusBar* pThis, CDC* pDC) {
    if (pThis == nullptr || pDC == nullptr || pDC->m_hDC == nullptr) return;

    RECT rectClip;
    ::SetRectEmpty(&rectClip);
    impl__GetClipBox_CDC__UEBAHPEAUtagRECT___Z(pDC, &rectClip);
    RECT rectClient;
    ::SetRectEmpty(&rectClient);
    ::GetClientRect(pThis->m_hWnd, &rectClient);

    // stand-in for CBasePane::DoPaint -> OnFillBarBackground (see above):
    // the clip box unless it is empty, in the bar-face colour.
    ::FillRect(pDC->m_hDC, ::IsRectEmpty(&rectClip) ? &rectClient : &rectClip,
               reinterpret_cast<HBRUSH>(static_cast<INT_PTR>(COLOR_BTNFACE + 1)));

    HFONT hFont = impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ(pThis);
    HGDIOBJ hOldFont = hFont ? ::SelectObject(pDC->m_hDC, hFont) : nullptr;
    const int nOldMode = impl__SetBkMode_CDC__QEAAHH_Z(pDC, TRANSPARENT);
    const COLORREF clrOldText = impl__SetTextColor_CDC__QEAAKK_Z(pDC, GD_clrBtnText());
    const COLORREF clrOldBk = impl__SetBkColor_CDC__QEAAKK_Z(pDC, GD_clrBtnFace());

    SbPane* panes = Panes(pThis);
    for (int i = 0; i < pThis->m_nCount; ++i) {
        impl__OnDrawPane_CMFCStatusBar__MEAAXPEAVCDC__PEAVCMFCStatusBarPaneInfo___Z(pThis, pDC, panes + i);
    }

    if (hOldFont) ::SelectObject(pDC->m_hDC, hOldFont);

    const RECT& rectSizeBox = Own(pThis).m_rectSizeBox;
    if (!::IsRectEmpty(&rectSizeBox)) {
        if (CMFCVisualManager* pVM = VisualManager()) {
            pVM->OnDrawStatusBarSizeBox(pDC, pThis, CRect(rectSizeBox));
        }
    }

    impl__SetTextColor_CDC__QEAAKK_Z(pDC, clrOldText);
    impl__SetBkColor_CDC__QEAAKK_Z(pDC, clrOldBk);
    impl__SetBkMode_CDC__QEAAHH_Z(pDC, nOldMode);
}

// CMFCStatusBar::EnablePaneDoubleClick(BOOL) -- retail RVA 0x134e30:
//     m_bPaneDoubleClick = bEnable;   (`mov %edx,0x414(%rcx)`)
// Symbol: ?EnablePaneDoubleClick@CMFCStatusBar@@QEAAXH@Z
extern "C" void MS_ABI impl__EnablePaneDoubleClick_CMFCStatusBar__QEAAXH_Z(CMFCStatusBar* pThis, int bEnable) {
    if (pThis == nullptr) return;
    Own(pThis).m_bPaneDoubleClick = bEnable;
}

// CMFCStatusBar::GetCurrentFont() const -- retail RVA 0x136080:
//     return m_hFont != NULL ? m_hFont : GetGlobalData()->fontRegular;   // +0x1b0 of afxGlobalData
// Symbol: ?GetCurrentFont@CMFCStatusBar@@IEBAPEAUHFONT__@@XZ
extern "C" HFONT MS_ABI impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ(const CMFCStatusBar* pThis) {
    if (pThis == nullptr) return nullptr;
    HFONT h = Own(pThis).m_hFont;
    return h != nullptr ? h : GD_fontRegular();
}

// CMFCStatusBar::GetExtendedArea(CRect&) const -- retail RVA 0x136120:
//     if (!m_bDrawExtendedArea) return FALSE;
//     CRect rectClient; ::GetClientRect(m_hWnd, rectClient);
//     for (i = m_nCount - 1; i >= 0; i--) {
//         p = _GetPanePtr(i); ENSURE(p);
//         if (p->nStyle & SBPS_STRETCH) { rect = rectClient; rect.left = p->rect.right; return TRUE; }
//     }
//     return FALSE;
// Symbol: ?GetExtendedArea@CMFCStatusBar@@UEBAHAEAVCRect@@@Z
extern "C" int MS_ABI impl__GetExtendedArea_CMFCStatusBar__UEBAHAEAVCRect___Z(const CMFCStatusBar* pThis, CRect* rect) {
    if (pThis == nullptr || rect == nullptr) return FALSE;
    if (!Own(pThis).m_bDrawExtendedArea) return FALSE;
    RECT rectClient;
    ::SetRectEmpty(&rectClient);
    ::GetClientRect(pThis->m_hWnd, &rectClient);
    for (int i = pThis->m_nCount - 1; i >= 0; --i) {
        const SbPane* p = GetPanePtr(pThis, i);
        if (p == nullptr) return FALSE;   // retail: AfxThrowInvalidArgException
        if (p->nStyle & kSBPS_STRETCH) {
            *rect = rectClient;
            rect->left = p->rect.right;
            return TRUE;
        }
    }
    return FALSE;
}

// CMFCStatusBar::GetItemID(int) const -- retail RVA 0x1343c0:
//     p = _GetPanePtr(nIndex); return p ? p->nID : 0;
// (Retail leaves EAX holding the NULL from _GetPanePtr, i.e. 0.)
// Symbol: ?GetItemID@CMFCStatusBar@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetItemID_CMFCStatusBar__QEBAIH_Z(const CMFCStatusBar* pThis, int nIndex) {
    const SbPane* p = pThis ? GetPanePtr(pThis, nIndex) : nullptr;
    return p ? p->nID : 0u;
}

// CMFCStatusBar::GetItemRect(int, LPRECT) const -- retail RVA 0x1343e0:
//     ENSURE(lpRect != NULL);                    // throws at 0x134405
//     p = _GetPanePtr(nIndex); if (p) *lpRect = p->rect;
// Deviation: a NULL lpRect returns instead of throwing.
// Symbol: ?GetItemRect@CMFCStatusBar@@QEBAXHPEAUtagRECT@@@Z
extern "C" void MS_ABI impl__GetItemRect_CMFCStatusBar__QEBAXHPEAUtagRECT___Z(const CMFCStatusBar* pThis, int nIndex, RECT* lpRect) {
    if (pThis == nullptr || lpRect == nullptr) return;
    const SbPane* p = GetPanePtr(pThis, nIndex);
    if (p != nullptr) *lpRect = p->rect;
}

// CMFCStatusBar::GetPaneInfo(int, UINT&, UINT&, int&) const -- retail RVA 0x134590:
//     p = _GetPanePtr(nIndex); if (!p) return;         // outputs untouched
//     nID = p->nID; nStyle = p->nStyle;
//     CRect rect; ::CopyRect(&rect, &p->rect); cxWidth = rect.Width();
// Symbol: ?GetPaneInfo@CMFCStatusBar@@QEBAXHAEAI0AEAH@Z
extern "C" void MS_ABI impl__GetPaneInfo_CMFCStatusBar__QEBAXHAEAI0AEAH_Z(
    const CMFCStatusBar* pThis, int nIndex, unsigned int* pnID, unsigned int* pnStyle, int* pcxWidth) {
    if (pThis == nullptr) return;
    const SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    if (pnID) *pnID = p->nID;
    if (pnStyle) *pnStyle = p->nStyle;
    if (pcxWidth) *pcxWidth = p->rect.right - p->rect.left;
}

// CMFCStatusBar::GetPaneStyle(int) const -- retail RVA 0x134410:
//     p = _GetPanePtr(nIndex); return p ? p->nStyle : 0;
// Symbol: ?GetPaneStyle@CMFCStatusBar@@QEBAIH@Z
extern "C" unsigned int MS_ABI impl__GetPaneStyle_CMFCStatusBar__QEBAIH_Z(const CMFCStatusBar* pThis, int nIndex) {
    const SbPane* p = pThis ? GetPanePtr(pThis, nIndex) : nullptr;
    return p ? p->nStyle : 0u;
}

// CMFCStatusBar::GetPaneWidth(int) const -- retail RVA 0x134480:
//     p = _GetPanePtr(nIndex); if (!p) return 0;   // (EAX keeps the NULL)
//     CRect rect; ::CopyRect(&rect, &p->rect); return rect.Width();
// Symbol: ?GetPaneWidth@CMFCStatusBar@@QEBAHH@Z
extern "C" int MS_ABI impl__GetPaneWidth_CMFCStatusBar__QEBAHH_Z(const CMFCStatusBar* pThis, int nIndex) {
    const SbPane* p = pThis ? GetPanePtr(pThis, nIndex) : nullptr;
    return p ? (p->rect.right - p->rect.left) : 0;
}

// CMFCStatusBar::GetTipText(int) const -- retail RVA 0x134c90.  By-value
// CString return, same register order as GetPaneText (this=RCX, slot=RDX,
// nIndex=R8D).  Transcribed:
//     p = _GetPanePtr(nIndex);
//     return CString(p != NULL && p->lpszToolTip != NULL ? p->lpszToolTip : L"");
// Symbol: ?GetTipText@CMFCStatusBar@@QEBA?AV?$CStringT@_WV?$StrTraitMFC_DLL@_WV?$ChTraitsCRT@_W@ATL@@@@@ATL@@H@Z
extern "C" CString* MS_ABI impl__GetTipText_CMFCStatusBar__QEBA_AV__CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__H_Z(
    const CMFCStatusBar* pThis, CString* __ret, int nIndex) {
    if (__ret == nullptr) return __ret;
    const SbPane* p = pThis ? GetPanePtr(pThis, nIndex) : nullptr;
    const wchar_t* psz = (p != nullptr && p->lpszToolTip != nullptr) ? p->lpszToolTip : L"";
    new (__ret) CString(psz);
    return __ret;
}

// CMFCStatusBar::HitTest(CPoint) const -- retail RVA 0x135fd0 (CPoint packed
// in RDX):
//     for (i = 0; i < m_nCount; i++) {
//         p = _GetPanePtr(i); ENSURE(p);
//         CRect rect; ::CopyRect(&rect, &p->rect);
//         if (::PtInRect(&rect, point)) return p;
//     }
//     return NULL;
// Symbol: ?HitTest@CMFCStatusBar@@IEBAPEAVCMFCStatusBarPaneInfo@@VCPoint@@@Z
extern "C" void* MS_ABI impl__HitTest_CMFCStatusBar__IEBAPEAVCMFCStatusBarPaneInfo__VCPoint___Z(
    const CMFCStatusBar* pThis, long long ptPacked) {
    if (pThis == nullptr) return nullptr;
    const CPoint pt = UnpackPoint(ptPacked);
    POINT point = { pt.x, pt.y };
    for (int i = 0; i < pThis->m_nCount; ++i) {
        SbPane* p = GetPanePtr(pThis, i);
        if (p == nullptr) return nullptr;   // retail: AfxThrowInvalidArgException
        RECT rect = p->rect;
        if (::PtInRect(&rect, point)) return p;
    }
    return nullptr;
}

// CMFCStatusBar::InvalidatePaneContent(int) -- retail RVA 0x134d90:
//     if (this == NULL || m_hWnd == NULL) return;
//     p = _GetPanePtr(nIndex); if (!p) return;
//     CRect rect; ::CopyRect(&rect, &p->rect);
//     if (!(p->nStyle & SBPS_NOBORDERS)) ::InflateRect(&rect, -1, -1); else rect.top--;
//     ::InvalidateRect(m_hWnd, &rect, FALSE); ::UpdateWindow(m_hWnd);
// Symbol: ?InvalidatePaneContent@CMFCStatusBar@@QEAAXH@Z
extern "C" void MS_ABI impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(CMFCStatusBar* pThis, int nIndex) {
    if (pThis == nullptr || pThis->m_hWnd == nullptr) return;
    const SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    RECT rect = p->rect;
    if (!(p->nStyle & kSBPS_NOBORDERS)) ::InflateRect(&rect, -1, -1);
    else rect.top--;
    ::InvalidateRect(pThis->m_hWnd, &rect, FALSE);
    ::UpdateWindow(pThis->m_hWnd);
}

// CMFCStatusBar::OnCreate(LPCREATESTRUCT) -- retail RVA 0x1351b0:
//     if (CWnd::Default() == -1) return -1;      // 0x28ac80, called directly: neither CPane nor
//                                                //   CBasePane exports an OnCreate, CWnd's is the inline Default()
//     EnableToolTips();                          // inlined: AfxGetModuleThreadState();
//                                                //   if (!(m_nFlags & WF_TOOLTIPS)) {
//                                                //     m_pModuleState->m_pfnFilterToolTipMessage = 0x2758a0;
//                                                //     m_nFlags |= WF_TOOLTIPS; }
//     return 0;
// Symbol: ?OnCreate@CMFCStatusBar@@IEAAHPEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__OnCreate_CMFCStatusBar__IEAAHPEAUtagCREATESTRUCTW___Z(CMFCStatusBar* pThis, CREATESTRUCTW* /*lpCreateStruct*/) {
    if (pThis == nullptr) return -1;
    if (static_cast<int>(impl__Default_CWnd__IEAA_JXZ(pThis)) == -1) return -1;
    impl__EnableToolTips_CWnd__QEAAHH_Z(pThis, TRUE);
    return 0;
}

// CMFCStatusBar::OnDestroy() -- retail RVA 0x133d60:
//     for (i = 0; i < m_nCount; i++) {
//         SetPaneText(i, NULL, FALSE);          // vslot 202
//         SetTipText(i, NULL);                  // 0x134d10
//         SetPaneIcon(i, (HICON)NULL, FALSE);   // 0x1347d0
//     }
//     CPane::OnDestroy();                        // 0xa1780
// Deviation: CPane::OnDestroy's tabbed-pane detach runs on CPane vtable slots
// this object lacks; the chain ends at CWnd::OnDestroy instead.
// Symbol: ?OnDestroy@CMFCStatusBar@@IEAAXXZ
extern "C" void MS_ABI impl__OnDestroy_CMFCStatusBar__IEAAXXZ(CMFCStatusBar* pThis) {
    if (pThis == nullptr) return;
    for (int i = 0; i < pThis->m_nCount; ++i) {
        impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(pThis, i, nullptr, FALSE);
        impl__SetTipText_CMFCStatusBar__QEAAXHPEB_W_Z(pThis, i, nullptr);
        impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHICON____H_Z(pThis, i, nullptr, FALSE);
    }
    impl__OnDestroy_CWnd__IEAAXXZ(pThis);
}

// CMFCStatusBar::OnDrawPane(CDC*, CMFCStatusBarPaneInfo*) -- retail RVA 0x1356d0.
// Transcribed:
//     ENSURE(pPane != NULL);
//     CRect rectPane; ::CopyRect(&rectPane, &pPane->rect);
//     if (::IsRectEmpty(&rectPane) || !pDC->RectVisible(rectPane)) return;   // vslot 24
//     if (pPane->clrBackground != (COLORREF)-1) {
//         CBrush brush(pPane->clrBackground);                     // 0x2a4060
//         CBrush* pOld = pDC->SelectObject(&brush);               // 0x2a2730
//         ::PatBlt(pDC->m_hDC, rectPane.left, rectPane.top, rectPane.Width(), rectPane.Height(), PATCOPY);
//         pDC->SelectObject(pOld);
//     }
//     CMFCVisualManager::GetInstance()->OnDrawStatusBarPaneBorder(pDC, this, rectPane, pPane->nID, pPane->nStyle);  // vslot 25
//     if (!(pPane->nStyle & SBPS_NOBORDERS)) ::InflateRect(&rectPane, -2, -1);
//     if (pPane->hImage != NULL && pPane->cxIcon > 0) {
//         int y = max(0, (rectPane.Height() - pPane->cyIcon) / 2) + rectPane.top;
//         ::ImageList_DrawEx(pPane->hImage, pPane->nCurrFrame, pDC->m_hDC, rectPane.left, y,
//                            pPane->cxIcon, pPane->cyIcon, CLR_NONE, 0, ILD_NORMAL);   // 0x136210 wrapper
//     }
//     CRect rectText = rectPane; rectText.left += pPane->cxIcon; if (pPane->cxIcon > 0) rectText.left += 4;
//     if (pPane->nProgressTotal > 0) {
//         CRect rectProgress = rectText; ::InflateRect(&rectProgress, -1, -1);
//         COLORREF clrBar = pPane->clrProgressBar;
//         if (clrBar == (COLORREF)-1) clrBar = GetGlobalData()->clrHilite;   // +0x48
//         CMFCVisualManager::GetInstance()->OnDrawStatusBarProgress(pDC, this, rectProgress,
//             pPane->nProgressTotal, pPane->nProgressCurr, clrBar, pPane->clrProgressBarDest,
//             pPane->clrProgressText, pPane->bProgressText);                      // vslot 27
//     } else if (pPane->lpszText != NULL && pPane->cxText > 0) {
//         COLORREF clrText = CMFCVisualManager::GetInstance()->GetStatusBarPaneTextColor(this, pPane);  // vslot 26
//         COLORREF clrOld = pDC->SetTextColor(clrText);                            // vslot 14
//         pDC->DrawText(pPane->lpszText, wcslen(pPane->lpszText), rectText,
//                       DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);                 // 0x824, vslot 28
//         pDC->SetTextColor(clrOld);
//     }
// Deviations: the CBrush wrapper is replaced by the equivalent raw GDI calls;
// RectVisible is ::RectVisible on m_hDC (no CDC thunk exists); the text colour
// comes from the exported base CMFCVisualManager::GetStatusBarPaneTextColor
// (OpenMFC's C++ CMFCVisualManager declares no such virtual, so derived
// managers' overrides are not reached); with no visual manager instance yet
// (see VisualManager()) the border and progress bar are skipped and the text
// colour falls back to the base manager's rule (SBPS_DISABLED ->
// COLOR_GRAYTEXT, explicit clrText, else COLOR_BTNTEXT).
// Symbol: ?OnDrawPane@CMFCStatusBar@@MEAAXPEAVCDC@@PEAVCMFCStatusBarPaneInfo@@@Z
extern "C" void MS_ABI impl__OnDrawPane_CMFCStatusBar__MEAAXPEAVCDC__PEAVCMFCStatusBarPaneInfo___Z(
    CMFCStatusBar* pThis, CDC* pDC, void* pPaneRaw) {
    if (pThis == nullptr || pDC == nullptr || pDC->m_hDC == nullptr) return;
    if (pPaneRaw == nullptr) return;   // retail: AfxThrowInvalidArgException
    SbPane* pPane = static_cast<SbPane*>(pPaneRaw);

    RECT rectPane = pPane->rect;
    if (::IsRectEmpty(&rectPane)) return;
    if (!::RectVisible(pDC->m_hDC, &rectPane)) return;

    if (pPane->clrBackground != 0xFFFFFFFFul) {
        HBRUSH hBrush = ::CreateSolidBrush(pPane->clrBackground);
        if (hBrush != nullptr) {
            HGDIOBJ hOld = ::SelectObject(pDC->m_hDC, hBrush);
            ::PatBlt(pDC->m_hDC, rectPane.left, rectPane.top,
                     rectPane.right - rectPane.left, rectPane.bottom - rectPane.top, PATCOPY);
            ::SelectObject(pDC->m_hDC, hOld);
            ::DeleteObject(hBrush);
        }
    }

    CMFCVisualManager* pVM = VisualManager();
    if (pVM != nullptr) {
        pVM->OnDrawStatusBarPaneBorder(pDC, pThis, CRect(rectPane), pPane->nID, pPane->nStyle);
    }

    if (!(pPane->nStyle & kSBPS_NOBORDERS)) ::InflateRect(&rectPane, -2, -1);

    if (pPane->hImage != nullptr && pPane->cxIcon > 0) {
        int y = ((rectPane.bottom - rectPane.top) - pPane->cyIcon) / 2;
        if (y < 0) y = 0;
        y += rectPane.top;
        ::ImageList_DrawEx(pPane->hImage, pPane->nCurrFrame, pDC->m_hDC, rectPane.left, y,
                           pPane->cxIcon, pPane->cyIcon, CLR_NONE, 0, ILD_NORMAL);
    }

    RECT rectText = rectPane;
    rectText.left += pPane->cxIcon;
    if (pPane->cxIcon > 0) rectText.left += 4;

    if (pPane->nProgressTotal > 0) {
        RECT rectProgress = rectText;
        ::InflateRect(&rectProgress, -1, -1);
        COLORREF clrBar = pPane->clrProgressBar;
        if (clrBar == 0xFFFFFFFFul) clrBar = GD_clrHilite();
        if (pVM != nullptr) {
            pVM->OnDrawStatusBarProgress(pDC, pThis, CRect(rectProgress),
                                         pPane->nProgressTotal, pPane->nProgressCurr, clrBar,
                                         pPane->clrProgressBarDest, pPane->clrProgressText,
                                         pPane->bProgressText);
        }
    } else if (pPane->lpszText != nullptr && pPane->cxText > 0) {
        COLORREF clrText;
        if (pVM != nullptr) {
            clrText = impl__GetStatusBarPaneTextColor_CMFCVisualManager__UEAAKPEAVCMFCStatusBar__PEAVCMFCStatusBarPaneInfo___Z(
                pVM, pThis, pPane);
        } else if (pPane->nStyle & kSBPS_DISABLED) {
            clrText = ::GetSysColor(COLOR_GRAYTEXT);
        } else {
            clrText = (pPane->clrText != 0xFFFFFFFFul) ? pPane->clrText : GD_clrBtnText();
        }
        const COLORREF clrOld = impl__SetTextColor_CDC__QEAAKK_Z(pDC, clrText);
        impl__DrawTextW_CDC__QEAAHPEB_WHPEAUTAGRECT__I_Z(
            pDC, pPane->lpszText, static_cast<int>(::wcslen(pPane->lpszText)), &rectText,
            DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
        impl__SetTextColor_CDC__QEAAKK_Z(pDC, clrOld);
    }
}

// CMFCStatusBar::OnGetFont() -- retail RVA 0x135580: `jmp GetCurrentFont`.
// Symbol: ?OnGetFont@CMFCStatusBar@@IEAAPEAUHFONT__@@XZ
extern "C" HFONT MS_ABI impl__OnGetFont_CMFCStatusBar__IEAAPEAUHFONT____XZ(CMFCStatusBar* pThis) {
    return impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ(pThis);
}

// CMFCStatusBar::OnGetText(int nMaxChars, LPTSTR lpszDest) -- retail RVA 0x1355f0:
//     if (nMaxChars == 0 || lpszDest == NULL) return 0;
//     int nLen = 0;
//     int nIndex = CommandToIndex(0);
//     if (nIndex >= 0) {
//         p = _GetPanePtr(nIndex); if (!p) return 0;
//         nLen = p->lpszText ? wcslen(p->lpszText) : 0;
//         if (nLen > nMaxChars) nLen = nMaxChars - 1;
//         memcpy(lpszDest, p->lpszText, nLen * sizeof(WCHAR));
//     }
//     lpszDest[nLen] = 0;
//     return nLen + 1;
// Retail quirk kept as-is: when nLen == nMaxChars the terminator lands at
// lpszDest[nMaxChars].
// Symbol: ?OnGetText@CMFCStatusBar@@IEAAHHPEA_W@Z
extern "C" int MS_ABI impl__OnGetText_CMFCStatusBar__IEAAHHPEA_W_Z(CMFCStatusBar* pThis, int nMaxChars, wchar_t* lpszDest) {
    if (pThis == nullptr || nMaxChars == 0 || lpszDest == nullptr) return 0;
    int nLen = 0;
    const int nIndex = impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(pThis, 0);
    if (nIndex >= 0) {
        const SbPane* p = GetPanePtr(pThis, nIndex);
        if (p == nullptr) return 0;
        nLen = p->lpszText ? static_cast<int>(::wcslen(p->lpszText)) : 0;
        if (nLen > nMaxChars) nLen = nMaxChars - 1;
        if (nLen > 0) std::memcpy(lpszDest, p->lpszText, static_cast<size_t>(nLen) * sizeof(wchar_t));
    }
    lpszDest[nLen] = 0;
    return nLen + 1;
}

// CMFCStatusBar::OnGetTextLength() -- retail RVA 0x135680:
//     nIndex = CommandToIndex(0); if (nIndex < 0) return 0;
//     p = _GetPanePtr(nIndex); if (!p) return 0;
//     return p->lpszText ? wcslen(p->lpszText) : 0;
// Symbol: ?OnGetTextLength@CMFCStatusBar@@IEAAIXZ
extern "C" unsigned int MS_ABI impl__OnGetTextLength_CMFCStatusBar__IEAAIXZ(CMFCStatusBar* pThis) {
    if (pThis == nullptr) return 0;
    const int nIndex = impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(pThis, 0);
    if (nIndex < 0) return 0;
    const SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return 0;
    return p->lpszText ? static_cast<unsigned int>(::wcslen(p->lpszText)) : 0u;
}

// CMFCStatusBar::OnLButtonDblClk(UINT, CPoint) -- retail RVA 0x135200 (point
// packed in R8):
//     if (m_bPaneDoubleClick) {
//         p = HitTest(point);
//         if (p) {
//             HWND hOwner = m_hWndOwner ? m_hWndOwner : ::GetParent(m_hWnd);   // CWnd +0xa0
//             CWnd* pOwner = CWnd::FromHandle(hOwner);                         // 0x28ad70
//             ::PostMessageW(pOwner->m_hWnd, WM_COMMAND, p->nID, 0);
//         }
//     }
//     CPane::OnLButtonDblClk(nFlags, point);       // tail-jump to 0xa1a50
// CPane::OnLButtonDblClk (0xa1a50) is:
//     OnProcessDblClk();                            // vslot 177 = CPane::OnProcessDblClk 0xa1ac0
//     if (CanFloat()) {                             // vslot 117 = CBasePane::CanFloat 0xc7c0
//         FloatPane(CRect(this+0x2c0), 2, TRUE);    // vslot 129
//         CWnd::Default();                          // 0x28ac80 -- ONLY inside this branch
//     }
// CBasePane::CanFloat (0xc7c0) returns (m_dwControlBarStyle & 1) for a pane
// that is not tabbed, and CreateEx (0x133e20) stores 0 into that member
// (+0x108), so for a status bar the whole branch is skipped and the retail
// handler returns without ever reaching DefWindowProc.
// Deviations: OpenMFC's CWnd has no m_hWndOwner, so the owner is always the
// parent; retail dereferences the FromHandle result unconditionally, here a
// NULL result falls back to the raw HWND; CPane::OnProcessDblClk (sets the
// +0x212 double-click flag and tears down a drag capture, +0x210) works on
// CPane members this object lacks and is not modelled.  Default() is NOT
// called, matching the CanFloat()==FALSE path retail takes for a status bar.
// Symbol: ?OnLButtonDblClk@CMFCStatusBar@@IEAAXIVCPoint@@@Z
extern "C" void MS_ABI impl__OnLButtonDblClk_CMFCStatusBar__IEAAXIVCPoint___Z(CMFCStatusBar* pThis, unsigned int /*nFlags*/, long long ptPacked) {
    if (pThis == nullptr) return;
    if (Own(pThis).m_bPaneDoubleClick) {
        const SbPane* p = static_cast<const SbPane*>(
            impl__HitTest_CMFCStatusBar__IEBAPEAVCMFCStatusBarPaneInfo__VCPoint___Z(pThis, ptPacked));
        if (p != nullptr) {
            HWND hOwner = ::GetParent(pThis->m_hWnd);
            CWnd* pOwner = impl__FromHandle_CWnd__SAPEAV1_PEAUHWND_____Z(hOwner);
            ::PostMessageW(pOwner ? pOwner->m_hWnd : hOwner, WM_COMMAND, p->nID, 0);
        }
    }
    // CPane::OnLButtonDblClk with CanFloat()==FALSE: nothing further (see above).
}

// CMFCStatusBar::OnNcHitTest(CPoint) -- retail RVA 0x135380 (point packed in RDX):
//     DWORD dwExStyle = GetExStyle();
//     if (m_cxSizeBox != 0) {
//         CRect rect; ::GetClientRect(m_hWnd, rect);
//         CalcInsideRect(rect, TRUE);
//         rect.left = rect.right - min(m_cxSizeBox - 1, rect.Height());
//         ClientToScreen(rect);
//         if (::PtInRect(&rect, point))
//             return (dwExStyle & WS_EX_LAYOUTRTL) ? HTBOTTOMLEFT : HTBOTTOMRIGHT;
//     }
//     return CWnd::Default();
// Symbol: ?OnNcHitTest@CMFCStatusBar@@IEAA_JVCPoint@@@Z
extern "C" __int64 MS_ABI impl__OnNcHitTest_CMFCStatusBar__IEAA_JVCPoint___Z(CMFCStatusBar* pThis, long long ptPacked) {
    if (pThis == nullptr) return HTNOWHERE;
    const DWORD dwExStyle = impl__GetExStyle_CWnd__QEBAKXZ(pThis);
    if (Own(pThis).m_cxSizeBox != 0) {
        RECT rect;
        ::SetRectEmpty(&rect);
        ::GetClientRect(pThis->m_hWnd, &rect);
        CalcInsideRect(pThis, rect, TRUE);
        rect.left = rect.right - std::min<int>(Own(pThis).m_cxSizeBox - 1, rect.bottom - rect.top);
        impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pThis, &rect);
        const CPoint pt = UnpackPoint(ptPacked);
        POINT point = { pt.x, pt.y };
        if (::PtInRect(&rect, point)) {
            return (dwExStyle & kWS_EX_LAYOUTRTL) ? HTBOTTOMLEFT : HTBOTTOMRIGHT;
        }
    }
    return impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMFCStatusBar::OnSetFont(CFont*, BOOL) -- retail RVA 0x135520:
//     m_hFont = pFont ? (HFONT)pFont->m_hObject : NULL;
//     RecalcLayout();                                        // vslot 134
//     if (bRedraw) { ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd); }
// Symbol: ?OnSetFont@CMFCStatusBar@@IEAAXPEAVCFont@@H@Z
extern "C" void MS_ABI impl__OnSetFont_CMFCStatusBar__IEAAXPEAVCFont__H_Z(CMFCStatusBar* pThis, CFont* pFont, int bRedraw) {
    if (pThis == nullptr) return;
    Own(pThis).m_hFont = pFont ? static_cast<HFONT>(pFont->m_hObject) : nullptr;
    impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    if (bRedraw) {
        ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pThis->m_hWnd);
    }
}

// CMFCStatusBar::OnSetText(LPCTSTR) -- retail RVA 0x135590:
//     nIndex = CommandToIndex(0); if (nIndex < 0) return -1;
//     return SetPaneText(nIndex, lpszText, TRUE) ? 0 : -1;    // vslot 202
// Symbol: ?OnSetText@CMFCStatusBar@@IEAAHPEB_W@Z
extern "C" int MS_ABI impl__OnSetText_CMFCStatusBar__IEAAHPEB_W_Z(CMFCStatusBar* pThis, const wchar_t* lpszText) {
    if (pThis == nullptr) return -1;
    const int nIndex = impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(pThis, 0);
    if (nIndex < 0) return -1;
    return impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(pThis, nIndex, lpszText, TRUE) ? 0 : -1;
}

// CMFCStatusBar::OnSettingChange(UINT, LPCTSTR) -- retail RVA 0x133d30:
//     RecalcLayout();          // `mov 0x430(%rax),%rax ; jmp` -- vslot 134, nothing else
// Symbol: ?OnSettingChange@CMFCStatusBar@@IEAAXIPEB_W@Z
extern "C" void MS_ABI impl__OnSettingChange_CMFCStatusBar__IEAAXIPEB_W_Z(CMFCStatusBar* pThis, unsigned int /*uFlags*/, const wchar_t* /*lpszSection*/) {
    if (pThis == nullptr) return;
    impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
}

// CMFCStatusBar::OnShowWindow(BOOL, UINT) -- retail RVA 0x1361d0:
//     CWnd::Default();                                            // 0x28ac80
//     if (GetParentFrame() != NULL)
//         ::PostMessageW(GetParentFrame()->m_hWnd, AFX_WM_CHANGEVISUALMANAGER, 0, 0);   // 0x3c2508
// AFX_WM_CHANGEVISUALMANAGER is read from the exported registered-message
// variable, the way the rest of the DLL reaches it.
// Symbol: ?OnShowWindow@CMFCStatusBar@@IEAAXHI@Z
extern "C" unsigned int impl__AFX_WM_CHANGEVISUALMANAGER__3IA;
extern "C" void MS_ABI impl__OnShowWindow_CMFCStatusBar__IEAAXHI_Z(CMFCStatusBar* pThis, int /*bShow*/, unsigned int /*nStatus*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    CFrameWnd* pFrame = impl__GetParentFrame_CWnd__QEBAPEAVCFrameWnd__XZ(pThis);
    if (pFrame != nullptr) {
        ::PostMessageW(pFrame->m_hWnd, impl__AFX_WM_CHANGEVISUALMANAGER__3IA, 0, 0);
    }
}

// CMFCStatusBar::OnSize(UINT, int, int) -- retail RVA 0x1354d0:
//     CBasePane::OnSize(nType, cx, cy);        // 0xc2a0: Default(); m_pDockBarRow ? ->vslot23(this)
//     RecalcLayout();                           // vslot 134
//     ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd);
// Deviation: the CBasePane part is its Default() only (no dock-bar row here).
// Symbol: ?OnSize@CMFCStatusBar@@IEAAXIHH@Z
extern "C" void MS_ABI impl__OnSize_CMFCStatusBar__IEAAXIHH_Z(CMFCStatusBar* pThis, unsigned int /*nType*/, int /*cx*/, int /*cy*/) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
    ::UpdateWindow(pThis->m_hWnd);
}

// CMFCStatusBar::OnStyleChanged(int, LPSTYLESTRUCT) -- retail RVA 0x1360c0:
//     CPane::OnStyleChanged(nStyleType, lpStyleStruct);            // 0xa2600
//     if ((lpStyleStruct->styleNew & SBARS_SIZEGRIP) && !(lpStyleStruct->styleOld & SBARS_SIZEGRIP))
//         RecalcLayout();                                           // vslot 134
// Deviation: CPane::OnStyleChanged is Default() plus an RTL-change notification
// through a CPane virtual; only the Default() part applies to this object.
// Symbol: ?OnStyleChanged@CMFCStatusBar@@IEAAXHPEAUtagSTYLESTRUCT@@@Z
extern "C" void MS_ABI impl__OnStyleChanged_CMFCStatusBar__IEAAXHPEAUtagSTYLESTRUCT___Z(CMFCStatusBar* pThis, int /*nStyleType*/, STYLESTRUCT* lpStyleStruct) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    if (lpStyleStruct == nullptr) return;   // retail dereferences it unconditionally
    if ((lpStyleStruct->styleNew & kSBARS_SIZEGRIP) && !(lpStyleStruct->styleOld & kSBARS_SIZEGRIP)) {
        impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    }
}

// CMFCStatusBar::OnSysCommand(UINT, LPARAM) -- retail RVA 0x135460:
//     if (m_cxSizeBox == 0 && (nID & 0xFFF0) == SC_SIZE) {
//         CFrameWnd* pFrame = AFXGetParentFrame(this);              // 0x6bd00
//         if (pFrame) { ::SendMessageW(pFrame->m_hWnd, WM_SYSCOMMAND, nID, lParam); return; }
//     }
//     CWnd::Default();
// Symbol: ?OnSysCommand@CMFCStatusBar@@IEAAXI_J@Z
extern "C" void MS_ABI impl__OnSysCommand_CMFCStatusBar__IEAAXI_J_Z(CMFCStatusBar* pThis, unsigned int nID, __int64 lParam) {
    if (pThis == nullptr) return;
    if (Own(pThis).m_cxSizeBox == 0 && (nID & 0xFFF0) == SC_SIZE) {
        CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pThis);
        if (pFrame != nullptr) {
            ::SendMessageW(pFrame->m_hWnd, WM_SYSCOMMAND, nID, static_cast<LPARAM>(lParam));
            return;
        }
    }
    impl__Default_CWnd__IEAA_JXZ(pThis);
}

// CMFCStatusBar::OnTimer(UINT_PTR) -- retail RVA 0x135280:
//     CWnd::Default();
//     nIndex = CommandToIndex((UINT)nIDEvent); if (nIndex < 0) return;
//     p = _GetPanePtr(nIndex); if (!p) return;
//     if (++p->nCurrFrame >= p->nFrameCount) p->nCurrFrame = 0;
//     CRect rect; ::CopyRect(&rect, &p->rect);
//     if (!(p->nStyle & SBPS_NOBORDERS)) ::InflateRect(&rect, -1, -1); else rect.top--;
//     rect.right = rect.left + p->cxIcon;
//     ::InvalidateRect(m_hWnd, &rect, FALSE); ::UpdateWindow(m_hWnd);
//     if (CMFCPopupMenu::m_pActivePopupMenu != NULL &&                       // 0x3be288
//         CWnd::FromHandlePermanent(m_pActivePopupMenu->m_hWnd) != NULL) {  // 0x28adc0
//         ClientToScreen(&rect);                                           // 0x2a3310
//         CMFCPopupMenu::UpdateAllShadows(&rect);                          // 0xbacd0
//     }
// Symbol: ?OnTimer@CMFCStatusBar@@IEAAX_K@Z
extern "C" void MS_ABI impl__OnTimer_CMFCStatusBar__IEAAX_K_Z(CMFCStatusBar* pThis, unsigned __int64 nIDEvent) {
    if (pThis == nullptr) return;
    impl__Default_CWnd__IEAA_JXZ(pThis);
    const int nIndex = impl__CommandToIndex_CMFCStatusBar__QEBAHI_Z(pThis, static_cast<unsigned int>(nIDEvent));
    if (nIndex < 0) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    p->nCurrFrame++;
    if (p->nCurrFrame >= p->nFrameCount) p->nCurrFrame = 0;
    RECT rect = p->rect;
    if (!(p->nStyle & kSBPS_NOBORDERS)) ::InflateRect(&rect, -1, -1);
    else rect.top--;
    rect.right = rect.left + p->cxIcon;
    ::InvalidateRect(pThis->m_hWnd, &rect, FALSE);
    ::UpdateWindow(pThis->m_hWnd);

    CWnd* pActivePopup = static_cast<CWnd*>(impl__m_pActivePopupMenu_CMFCPopupMenu__1PEAV1_EA);
    if (pActivePopup != nullptr &&
        impl__FromHandlePermanent_CWnd__SAPEAV1_PEAUHWND_____Z(pActivePopup->m_hWnd) != nullptr) {
        impl__ClientToScreen_CWnd__QEBAXPEAUtagRECT___Z(pThis, &rect);
        impl__UpdateAllShadows_CMFCPopupMenu__SAXPEAUtagRECT___Z(&rect);
    }
}

// CMFCStatusBar::OnToolHitTest(CPoint, TOOLINFO*) const -- retail RVA 0x135ea0
// (point packed in RDX, pTI in R8):
//     INT_PTR nHit = CWnd::OnToolHitTest(point, pTI);          // 0x28bcc0
//     if (nHit != -1) return nHit;
//     p = HitTest(point);
//     if (p != NULL && p->lpszToolTip != NULL) {
//         nHit = p->nID;
//         if (pTI != NULL) {
//             CString str(p->lpszToolTip);
//             pTI->lpszText = (LPWSTR)calloc(str.GetLength() + 1, sizeof(WCHAR));
//             ::lstrcpyW(pTI->lpszText, str);
//             pTI->rect = p->rect; pTI->uId = 0; pTI->hwnd = m_hWnd;
//         }
//     }
//     CToolTipCtrl* pToolTip = AfxGetModuleState()->m_thread->m_pToolTip;      // +0xf8 / +0x108
//     if (pToolTip && pToolTip->m_hWnd)
//         ::SendMessageW(pToolTip->m_hWnd, WM_SETFONT, GetGlobalData()->fontTooltip, 0);   // +0x1c0
//     return nHit;
// Deviation: OpenMFC's AFX_MODULE_THREAD_STATE (detail/RegcoreSupport.h:152)
// has no m_pToolTip, so the tooltip-font part is not modelled.
// Symbol: ?OnToolHitTest@CMFCStatusBar@@MEBA_JVCPoint@@PEAUtagTOOLINFOW@@@Z
extern "C" __int64 MS_ABI impl__OnToolHitTest_CMFCStatusBar__MEBA_JVCPoint__PEAUtagTOOLINFOW___Z(
    const CMFCStatusBar* pThis, long long ptPacked, TOOLINFOW* pTI) {
    if (pThis == nullptr) return -1;
    const CPoint pt = UnpackPoint(ptPacked);
    __int64 nHit = impl__OnToolHitTest_CWnd__UEBA_JVCPoint__PEAUtagTOOLINFOW___Z(pThis, pt, pTI);
    if (nHit != -1) return nHit;
    const SbPane* p = static_cast<const SbPane*>(
        impl__HitTest_CMFCStatusBar__IEBAPEAVCMFCStatusBarPaneInfo__VCPoint___Z(pThis, ptPacked));
    if (p != nullptr && p->lpszToolTip != nullptr) {
        nHit = p->nID;
        if (pTI != nullptr) {
            const size_t nLen = ::wcslen(p->lpszToolTip);
            pTI->lpszText = static_cast<LPWSTR>(std::calloc(nLen + 1, sizeof(wchar_t)));
            if (pTI->lpszText != nullptr) ::lstrcpyW(pTI->lpszText, p->lpszToolTip);
            pTI->rect = p->rect;
            pTI->uId = 0;
            pTI->hwnd = pThis->m_hWnd;
        }
    }
    return nHit;
}

// CMFCStatusBar::OnUpdateCmdUI(CFrameWnd*, BOOL) -- retail RVA 0x135d50:
//     CMFCStatusBarCmdUI state;  state.m_pOther = this;  state.m_nIndexMax = m_nCount;
//     for (state.m_nIndex = 0; state.m_nIndex < state.m_nIndexMax; state.m_nIndex++) {
//         state.m_nID = _GetPanePtr(state.m_nIndex)->nID;        // no NULL check in retail
//         if (state.m_nID == 0 || (WORD)state.m_nID == 0xFFFF) continue;
//         ENSURE(pTarget != NULL);
//         state.m_bEnableChanged = FALSE;
//         pTarget->OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL);   // vslot 5
//         if (bDisableIfNoHndler && !state.m_bEnableChanged) {
//             AFX_CMDHANDLERINFO info; info.pTarget = NULL;
//             BOOL bHandler = pTarget->OnCmdMsg(state.m_nID, CN_COMMAND, &state, &info);
//             state.Enable(bHandler);                                // vslot 0 (devirtualised)
//         }
//     }
//     UpdateDialogControls(pTarget, bDisableIfNoHndler);            // 0x291460
// Symbol: ?OnUpdateCmdUI@CMFCStatusBar@@MEAAXPEAVCFrameWnd@@H@Z
extern "C" void MS_ABI impl__OnUpdateCmdUI_CMFCStatusBar__MEAAXPEAVCFrameWnd__H_Z(CMFCStatusBar* pThis, CFrameWnd* pTarget, int bDisableIfNoHndler) {
    if (pThis == nullptr) return;
    MsCmdUI state;
    std::memset(&state, 0, sizeof(state));
    state.vfptr = g_msCmdUIVtbl;
    state.m_pOther = pThis;
    state.m_nIndexMax = static_cast<UINT>(pThis->m_nCount);
    for (state.m_nIndex = 0; static_cast<UINT>(state.m_nIndex) < state.m_nIndexMax; ++state.m_nIndex) {
        const SbPane* p = GetPanePtr(pThis, state.m_nIndex);
        if (p == nullptr) break;   // retail would fault here
        state.m_nID = p->nID;
        if (state.m_nID == 0 || static_cast<WORD>(state.m_nID) == 0xFFFF) continue;
        if (pTarget == nullptr) return;   // retail: AfxThrowInvalidArgException
        state.m_bEnableChanged = FALSE;
        void** vtbl = *reinterpret_cast<void***>(pTarget);
        OnCmdMsgFn fnOnCmdMsg = reinterpret_cast<OnCmdMsgFn>(vtbl[kVtOnCmdMsgSlot]);
        fnOnCmdMsg(pTarget, state.m_nID, kCN_UPDATE_COMMAND_UI, &state, nullptr);
        if (bDisableIfNoHndler && !state.m_bEnableChanged) {
            void* info[2] = { nullptr, nullptr };   // AFX_CMDHANDLERINFO { pTarget, pmf }
            const int bHandler = fnOnCmdMsg(pTarget, state.m_nID, kCN_COMMAND, &state, info);
            CmdUI_Enable(&state, bHandler);
        }
    }
    impl__UpdateDialogControls_CWnd__QEAAXPEAVCCmdTarget__H_Z(pThis, pTarget, bDisableIfNoHndler);
}

// CMFCStatusBar::PreCreateWindow(CREATESTRUCT&) -- retail RVA 0x133dd0:
//     if ((m_dwStyle & (CBRS_ALIGN_ANY | CBRS_BORDER_ANY)) == CBRS_BOTTOM)   // & 0xff00 == 0x8200
//         m_dwStyle &= ~(CBRS_BORDER_ANY | CBRS_BORDER_3D);                  // & 0xfffff07f
//     return CWnd::PreCreateWindow(cs);                                     // jmp 0x28b640
// (m_dwStyle is CBasePane +0x104 in retail; CControlBar::m_dwStyle here.)
// Symbol: ?PreCreateWindow@CMFCStatusBar@@UEAAHAEAUtagCREATESTRUCTW@@@Z
extern "C" int MS_ABI impl__PreCreateWindow_CMFCStatusBar__UEAAHAEAUtagCREATESTRUCTW___Z(CMFCStatusBar* pThis, CREATESTRUCTW* cs) {
    if (pThis == nullptr || cs == nullptr) return FALSE;
    if ((pThis->m_dwStyle & 0xff00) == 0x8200) pThis->m_dwStyle &= 0xfffff07f;
    return impl__PreCreateWindow_CWnd__UEAAHAEAUtagCREATESTRUCTW___Z(pThis, *cs);
}

// CMFCStatusBar::RecalcLayout() -- retail RVA 0x1359d0 (vslot 134).  Transcribed:
//     ENSURE(this != NULL && m_hWnd != NULL);
//     CRect rect; ::GetClientRect(m_hWnd, rect);
//     CalcInsideRect(rect, TRUE);
//     m_cxSizeBox = min(::GetSystemMetrics(SM_CXVSCROLL) + 1, rect.Height());
//     CFrameWnd* pFrame = AFXGetParentFrame(this);
//     if (pFrame && ::IsZoomed(pFrame->m_hWnd)) m_cxSizeBox = 0;
//     if (!(GetStyle() & SBARS_SIZEGRIP)) m_cxSizeBox = 0;
//     CClientDC dc(NULL);                                    // constructed, never used
//     rect.right -= m_cxSizeBox;
//     int xMax = (m_cxSizeBox == 0) ? rect.right + m_cxRightBorder + 1 : rect.right;
//     if (m_nCount > 0) {
//         int cxExtra = rect.Width() + m_cxDefaultGap;
//         for each pane: cxExtra -= pane->cxText + pane->cxIcon + m_cxDefaultGap + 4;
//                        if (pane->cxText > 0 && pane->cxIcon > 0) cxExtra -= 4;
//         for each pane {
//             if (rect.left >= xMax) { pane->rect = CRect(0,0,0,0); continue; }
//             int cx = pane->cxText + pane->cxIcon; if (both > 0) cx += 4;
//             if ((pane->nStyle & SBPS_STRETCH) && cxExtra > 0) { cx += cxExtra; cxExtra = 0; }
//             rect.right = min(xMax, rect.left + cx + 4);
//             pane->rect = rect;
//             rect.left = rect.right + m_cxDefaultGap;
//         }
//     }
//     if (m_cxSizeBox != 0) {
//         m_rectSizeBox = rect;                    // the running rect
//         m_rectSizeBox.left = rect.right;
//         m_rectSizeBox.right = rect.right + min(m_cxSizeBox, rect.Height() + m_cyTopBorder);
//     } else ::SetRectEmpty(&m_rectSizeBox);
// Symbol: ?RecalcLayout@CMFCStatusBar@@MEAAXXZ
extern "C" void MS_ABI impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(CMFCStatusBar* pThis) {
    if (pThis == nullptr || pThis->m_hWnd == nullptr) return;   // retail: AfxThrowInvalidArgException
    SbOwn& own = Own(pThis);

    RECT rect;
    ::SetRectEmpty(&rect);
    ::GetClientRect(pThis->m_hWnd, &rect);
    CalcInsideRect(pThis, rect, TRUE);

    own.m_cxSizeBox = std::min<int>(::GetSystemMetrics(SM_CXVSCROLL) + 1, rect.bottom - rect.top);
    CFrameWnd* pFrame = impl__AFXGetParentFrame__YAPEAVCFrameWnd__PEBVCWnd___Z(pThis);
    if (pFrame != nullptr && ::IsZoomed(pFrame->m_hWnd)) own.m_cxSizeBox = 0;
    if (!(impl__GetStyle_CWnd__QEBAKXZ(pThis) & kSBARS_SIZEGRIP)) own.m_cxSizeBox = 0;

    rect.right -= own.m_cxSizeBox;
    const int xMax = (own.m_cxSizeBox == 0) ? rect.right + pThis->m_cxRightBorder + 1 : rect.right;

    SbPane* panes = Panes(pThis);
    if (pThis->m_nCount > 0 && panes != nullptr) {
        int cxExtra = (rect.right - rect.left) + pThis->m_cxDefaultGap;
        for (int i = 0; i < pThis->m_nCount; ++i) {
            cxExtra -= panes[i].cxText + panes[i].cxIcon + pThis->m_cxDefaultGap + 4;
            if (panes[i].cxText > 0 && panes[i].cxIcon > 0) cxExtra -= 4;
        }
        for (int i = 0; i < pThis->m_nCount; ++i) {
            SbPane& pane = panes[i];
            if (rect.left >= xMax) {
                ::SetRectEmpty(&pane.rect);
                continue;
            }
            int cx = pane.cxText + pane.cxIcon;
            if (pane.cxText > 0 && pane.cxIcon > 0) cx += 4;
            if ((pane.nStyle & kSBPS_STRETCH) && cxExtra > 0) {
                cx += cxExtra;
                cxExtra = 0;
            }
            rect.right = std::min<int>(xMax, rect.left + cx + 4);
            pane.rect = rect;
            rect.left = rect.right + pThis->m_cxDefaultGap;
        }
    }

    if (own.m_cxSizeBox != 0) {
        own.m_rectSizeBox = rect;
        own.m_rectSizeBox.left = rect.right;
        own.m_rectSizeBox.right = rect.right +
            std::min<int>(own.m_cxSizeBox, (rect.bottom - rect.top) + pThis->m_cyTopBorder);
    } else {
        ::SetRectEmpty(&own.m_rectSizeBox);
    }
}

// CMFCStatusBar::SetIndicators(const UINT*, int) -- retail RVA 0x1340e0.  Transcribed:
//     for (i = 0; i < m_nCount; i++) { SetPaneText(i, NULL, FALSE); SetPaneIcon(i, (HICON)NULL, FALSE); SetTipText(i, NULL); }
//     if (!AllocElements(nIDCount, sizeof(CMFCStatusBarPaneInfo))) return FALSE;   // 0xa18c0
//     BOOL bResult = TRUE;
//     if (lpIDArray != NULL) {
//         HFONT hFont = GetCurrentFont(); ENSURE(hFont != NULL);        // throws at 0x134387
//         CString strText;
//         CClientDC dcScreen(NULL);
//         HGDIOBJ hOldFont = ::SelectObject(dcScreen.m_hDC, hFont);
//         for (i = 0; i < nIDCount; i++) {
//             p = _GetPanePtr(i); if (!p) { <release dc, strText>; return FALSE; }   // 0x134342, no RecalcLayout
//             <zero-fill p; clrText = clrBackground = -1; nProgressTotal = -1;
//              clrProgressBar = clrProgressBarDest = clrProgressText = -1>
//             p->nID = lpIDArray[i];
//             if (p->nID != 0) {
//                 HINSTANCE hInst = AfxFindStringResourceHandle(p->nID);   // 0x2aee00
//                 if (hInst == NULL || !strText.LoadString(hInst, p->nID)) { bResult = FALSE; break; }
//                 SIZE size; ::GetTextExtentPoint32W(dcScreen.m_hDC, strText, strText.GetLength(), &size);
//                 p->cxText = size.cx;
//                 if (!SetPaneText(i, strText, FALSE)) { bResult = FALSE; break; }   // vslot 202
//             } else {
//                 p->cxText = ::GetSystemMetrics(SM_CXSCREEN) / 4;
//                 if (i == 0) p->nStyle |= (SBPS_STRETCH | SBPS_NOBORDERS);
//             }
//         }
//         if (hOldFont) ::SelectObject(dcScreen.m_hDC, hOldFont);
//     }
//     RecalcLayout();                                                    // vslot 134
//     return bResult;
// The string lookup is transcribed as retail does it: the exported
// AfxFindStringResourceHandle (featurepack/CMFC_misc_stubs.cpp:1251, a real
// body that walks resource handle / instance handle / main module) followed by
// CString::LoadString(HINSTANCE, UINT) (core/collections/CStringT.cpp:283).
// (An earlier revision called CString::LoadString(UINT) instead and claimed it
// performed the same search; OpenMFC's LoadString(UINT) only tries
// AfxGetResourceHandle()/AfxGetInstanceHandle(), so that claim was false.)
// Symbol: ?SetIndicators@CMFCStatusBar@@QEAAHPEBIH@Z
extern "C" int MS_ABI impl__SetIndicators_CMFCStatusBar__QEAAHPEBIH_Z(CMFCStatusBar* pThis, const unsigned int* lpIDArray, int nIDCount) {
    if (pThis == nullptr) return FALSE;

    for (int i = 0; i < pThis->m_nCount; ++i) {
        impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(pThis, i, nullptr, FALSE);
        impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHICON____H_Z(pThis, i, nullptr, FALSE);
        impl__SetTipText_CMFCStatusBar__QEAAXHPEB_W_Z(pThis, i, nullptr);
    }

    if (!AllocElements(pThis, nIDCount, static_cast<int>(sizeof(SbPane)))) return FALSE;

    BOOL bResult = TRUE;
    if (lpIDArray != nullptr) {
        HFONT hFont = impl__GetCurrentFont_CMFCStatusBar__IEBAPEAUHFONT____XZ(pThis);
        if (hFont == nullptr) return FALSE;   // retail: AfxThrowInvalidArgException
        CString strText;
        HDC hdcScreen = ::GetDC(nullptr);
        HGDIOBJ hOldFont = ::SelectObject(hdcScreen, hFont);

        for (int i = 0; i < nIDCount; ++i) {
            SbPane* p = GetPanePtr(pThis, i);
            if (p == nullptr) {
                // retail 0x134342: release the DC and the string, return FALSE
                // without the RecalcLayout that the other failure paths run
                if (hOldFont) ::SelectObject(hdcScreen, hOldFont);
                ::ReleaseDC(nullptr, hdcScreen);
                return FALSE;
            }
            std::memset(p, 0, sizeof(*p));
            p->clrText = 0xFFFFFFFFul;
            p->clrBackground = 0xFFFFFFFFul;
            p->nProgressTotal = -1;
            p->clrProgressBar = 0xFFFFFFFFul;
            p->clrProgressBarDest = 0xFFFFFFFFul;
            p->clrProgressText = 0xFFFFFFFFul;
            p->nID = lpIDArray[i];
            if (p->nID != 0) {
                HINSTANCE hInst = static_cast<HINSTANCE>(
                    impl__AfxFindStringResourceHandle__YAPEAUHINSTANCE____I_Z(p->nID));
                if (hInst == nullptr ||
                    !impl__LoadStringW___CStringT__WV__StrTraitMFC_DLL__WV__ChTraitsCRT__W_ATL_____ATL__QEAAHPEAUHINSTANCE____I_Z(
                        &strText, hInst, p->nID)) {
                    bResult = FALSE;
                    break;
                }
                SIZE size = { 0, 0 };
                ::GetTextExtentPoint32W(hdcScreen, static_cast<const wchar_t*>(strText), strText.GetLength(), &size);
                p->cxText = size.cx;
                if (!impl__SetPaneText_CMFCStatusBar__UEAAHHPEB_WH_Z(pThis, i, static_cast<const wchar_t*>(strText), FALSE)) {
                    bResult = FALSE;
                    break;
                }
            } else {
                p->cxText = ::GetSystemMetrics(SM_CXSCREEN) / 4;
                if (i == 0) p->nStyle |= (kSBPS_STRETCH | kSBPS_NOBORDERS);
            }
        }

        if (hOldFont) ::SelectObject(hdcScreen, hOldFont);
        ::ReleaseDC(nullptr, hdcScreen);
    }

    impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    return bResult;
}

// CMFCStatusBar::SetPaneAnimation(int, HIMAGELIST, UINT, BOOL) -- retail RVA 0x134a00:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     if (p->nFrameCount > 0) ::KillTimer(m_hWnd, p->nID);
//     if (p->hImage) { ::ImageList_Destroy(p->hImage); p->hImage = NULL; }
//     p->nFrameCount = p->nCurrFrame = 0;
//     if (hImageList == NULL || (p->nFrameCount = ::ImageList_GetImageCount(hImageList)) == 0) {
//         if (bUpdate) InvalidatePaneContent(nIndex);
//         return;
//     }
//     ::ImageList_GetIconSize(hImageList, &p->cxIcon, &p->cyIcon);
//     p->hImage = ::ImageList_Create(p->cxIcon, p->cyIcon, ILC_COLORDDB | ILC_MASK /*0xff*/, 1, 1);
//     for (i = 0; i < p->nFrameCount; i++) {
//         HICON hIcon = ::ImageList_GetIcon(hImageList, i, ILD_TRANSPARENT);
//         ::ImageList_ReplaceIcon(p->hImage, -1, hIcon);
//         ::DestroyIcon(hIcon);
//     }
//     RecalcLayout();                                        // vslot 134
//     if (bUpdate) InvalidatePaneContent(nIndex);
//     ::SetTimer(m_hWnd, p->nID, nFrameRate, NULL);
// (The comctl32 entry points are delay-load wrappers in retail, resolved by
// their import-name strings at 0x342060/0x342078/0x33f630/0x33f658/0x33f618/0x33f338.)
// Symbol: ?SetPaneAnimation@CMFCStatusBar@@QEAAXHPEAU_IMAGELIST@@IH@Z
extern "C" void MS_ABI impl__SetPaneAnimation_CMFCStatusBar__QEAAXHPEAU_IMAGELIST__IH_Z(
    CMFCStatusBar* pThis, int nIndex, HIMAGELIST hImageList, unsigned int nFrameRate, int bUpdate) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    if (p->nFrameCount > 0) ::KillTimer(pThis->m_hWnd, p->nID);
    if (p->hImage != nullptr) {
        ::ImageList_Destroy(p->hImage);
        p->hImage = nullptr;
    }
    p->nFrameCount = 0;
    p->nCurrFrame = 0;
    if (hImageList == nullptr || (p->nFrameCount = ::ImageList_GetImageCount(hImageList)) == 0) {
        if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
        return;
    }
    ::ImageList_GetIconSize(hImageList, &p->cxIcon, &p->cyIcon);
    p->hImage = ::ImageList_Create(p->cxIcon, p->cyIcon, ILC_COLORDDB | ILC_MASK, 1, 1);
    for (int i = 0; i < p->nFrameCount; ++i) {
        HICON hIcon = ::ImageList_GetIcon(hImageList, i, ILD_TRANSPARENT);
        ::ImageList_ReplaceIcon(p->hImage, -1, hIcon);
        ::DestroyIcon(hIcon);
    }
    impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
    ::SetTimer(pThis->m_hWnd, p->nID, nFrameRate, nullptr);
}

// CMFCStatusBar::SetPaneBackgroundColor(int, COLORREF, BOOL) -- retail RVA 0x134c50:
//     p = _GetPanePtr(nIndex); if (!p || p->clrBackground == clr) return;
//     p->clrBackground = clr; if (bUpdate) InvalidatePaneContent(nIndex);
// Symbol: ?SetPaneBackgroundColor@CMFCStatusBar@@QEAAXHKH@Z
extern "C" void MS_ABI impl__SetPaneBackgroundColor_CMFCStatusBar__QEAAXHKH_Z(CMFCStatusBar* pThis, int nIndex, unsigned long clrBackground, int bUpdate) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr || p->clrBackground == clrBackground) return;
    p->clrBackground = clrBackground;
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
}

// CMFCStatusBar::SetPaneIcon(int, HBITMAP, COLORREF, BOOL) -- retail RVA 0x1348f0:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     SetPaneAnimation(nIndex, NULL, 0, FALSE);
//     if (hBmp == NULL) {
//         if (p->hImage) { ::ImageList_Destroy(p->hImage); }  p->hImage = NULL;
//         if (bUpdate) InvalidatePaneContent(nIndex);
//         return;
//     }
//     BITMAP bmp; ::GetObjectW(hBmp, sizeof(BITMAP), &bmp);
//     if (p->hImage == NULL) {
//         p->cxIcon = bmp.bmWidth; p->cyIcon = bmp.bmHeight;
//         p->hImage = ::ImageList_Create(p->cxIcon, p->cyIcon, ILC_COLORDDB | ILC_MASK, 1, 0);
//         RecalcLayout();                                              // vslot 134
//     } else ::ImageList_Remove(p->hImage, 0);      // wrapper 0x83704: `xor %edx,%edx`, index 0 hard-coded
//     HBITMAP hCopy = (HBITMAP)::CopyImage(hBmp, IMAGE_BITMAP, 0, 0, 0);
//     ::ImageList_AddMasked(p->hImage, hCopy, clrTransparent);
//     ::DeleteObject(hCopy);
//     if (bUpdate) InvalidatePaneContent(nIndex);
// Symbol: ?SetPaneIcon@CMFCStatusBar@@QEAAXHPEAUHBITMAP__@@KH@Z
extern "C" void MS_ABI impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHBITMAP____KH_Z(
    CMFCStatusBar* pThis, int nIndex, HBITMAP hBmp, unsigned long clrTransparent, int bUpdate) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    impl__SetPaneAnimation_CMFCStatusBar__QEAAXHPEAU_IMAGELIST__IH_Z(pThis, nIndex, nullptr, 0, FALSE);
    if (hBmp == nullptr) {
        if (p->hImage != nullptr) ::ImageList_Destroy(p->hImage);
        p->hImage = nullptr;
        if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
        return;
    }
    BITMAP bmp;
    std::memset(&bmp, 0, sizeof(bmp));
    ::GetObjectW(hBmp, sizeof(BITMAP), &bmp);
    if (p->hImage == nullptr) {
        p->cxIcon = bmp.bmWidth;
        p->cyIcon = bmp.bmHeight;
        p->hImage = ::ImageList_Create(p->cxIcon, p->cyIcon, ILC_COLORDDB | ILC_MASK, 1, 0);
        impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    } else {
        ::ImageList_Remove(p->hImage, 0);   // retail removes image 0, not -1 (the list holds one image)
    }
    HBITMAP hCopy = static_cast<HBITMAP>(::CopyImage(hBmp, IMAGE_BITMAP, 0, 0, 0));
    ::ImageList_AddMasked(p->hImage, hCopy, clrTransparent);
    ::DeleteObject(hCopy);
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
}

// CMFCStatusBar::SetPaneIcon(int, HICON, BOOL) -- retail RVA 0x1347d0:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     SetPaneAnimation(nIndex, NULL, 0, FALSE);
//     if (hIcon == NULL) {
//         if (p->hImage) ::ImageList_Destroy(p->hImage);  p->hImage = NULL;
//         if (bUpdate) InvalidatePaneContent(nIndex);
//         return;
//     }
//     ICONINFO ii; ::GetIconInfo(hIcon, &ii);
//     BITMAP bmp; ::GetObjectW(ii.hbmColor, sizeof(BITMAP), &bmp);
//     ::DeleteObject(ii.hbmColor); ::DeleteObject(ii.hbmMask);
//     if (p->hImage == NULL) {
//         p->cxIcon = bmp.bmWidth; p->cyIcon = bmp.bmHeight;
//         p->hImage = ::ImageList_Create(p->cxIcon, p->cyIcon, ILC_COLORDDB | ILC_MASK, 1, 0);
//         ::ImageList_ReplaceIcon(p->hImage, -1, hIcon);
//         RecalcLayout();                                              // vslot 134
//     } else ::ImageList_ReplaceIcon(p->hImage, 0, hIcon);
//     if (bUpdate) InvalidatePaneContent(nIndex);
// Symbol: ?SetPaneIcon@CMFCStatusBar@@QEAAXHPEAUHICON__@@H@Z
extern "C" void MS_ABI impl__SetPaneIcon_CMFCStatusBar__QEAAXHPEAUHICON____H_Z(
    CMFCStatusBar* pThis, int nIndex, HICON hIcon, int bUpdate) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    impl__SetPaneAnimation_CMFCStatusBar__QEAAXHPEAU_IMAGELIST__IH_Z(pThis, nIndex, nullptr, 0, FALSE);
    if (hIcon == nullptr) {
        if (p->hImage != nullptr) ::ImageList_Destroy(p->hImage);
        p->hImage = nullptr;
        if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
        return;
    }
    ICONINFO ii;
    std::memset(&ii, 0, sizeof(ii));
    ::GetIconInfo(hIcon, &ii);
    BITMAP bmp;
    std::memset(&bmp, 0, sizeof(bmp));
    ::GetObjectW(ii.hbmColor, sizeof(BITMAP), &bmp);
    ::DeleteObject(ii.hbmColor);
    ::DeleteObject(ii.hbmMask);
    if (p->hImage == nullptr) {
        p->cxIcon = bmp.bmWidth;
        p->cyIcon = bmp.bmHeight;
        p->hImage = ::ImageList_Create(p->cxIcon, p->cyIcon, ILC_COLORDDB | ILC_MASK, 1, 0);
        ::ImageList_ReplaceIcon(p->hImage, -1, hIcon);
        impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
    } else {
        ::ImageList_ReplaceIcon(p->hImage, 0, hIcon);
    }
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
}

// CMFCStatusBar::SetPaneInfo(int, UINT, UINT, int) -- retail RVA 0x134600:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     p->nID = nID; SetPaneStyle(nIndex, nStyle); SetPaneWidth(nIndex, cxWidth);
// Symbol: ?SetPaneInfo@CMFCStatusBar@@QEAAXHIIH@Z
extern "C" void MS_ABI impl__SetPaneInfo_CMFCStatusBar__QEAAXHIIH_Z(CMFCStatusBar* pThis, int nIndex, unsigned int nID, unsigned int nStyle, int cxWidth) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    p->nID = nID;
    impl__SetPaneStyle_CMFCStatusBar__QEAAXHI_Z(pThis, nIndex, nStyle);
    impl__SetPaneWidth_CMFCStatusBar__QEAAXHH_Z(pThis, nIndex, cxWidth);
}

// CMFCStatusBar::SetPaneStyle(int, UINT) -- retail RVA 0x134430:
//     p = _GetPanePtr(nIndex); if (!p || p->nStyle == nStyle) return;
//     p->nStyle = nStyle;
//     ::InvalidateRect(m_hWnd, &p->rect, FALSE); ::UpdateWindow(m_hWnd);
// Symbol: ?SetPaneStyle@CMFCStatusBar@@QEAAXHI@Z
extern "C" void MS_ABI impl__SetPaneStyle_CMFCStatusBar__QEAAXHI_Z(CMFCStatusBar* pThis, int nIndex, unsigned int nStyle) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr || p->nStyle == nStyle) return;
    p->nStyle = nStyle;
    ::InvalidateRect(pThis->m_hWnd, &p->rect, FALSE);
    ::UpdateWindow(pThis->m_hWnd);
}

// CMFCStatusBar::SetPaneTextColor(int, COLORREF, BOOL) -- retail RVA 0x134c10:
//     p = _GetPanePtr(nIndex); if (!p || p->clrText == clr) return;
//     p->clrText = clr; if (bUpdate) InvalidatePaneContent(nIndex);
// Symbol: ?SetPaneTextColor@CMFCStatusBar@@QEAAXHKH@Z
extern "C" void MS_ABI impl__SetPaneTextColor_CMFCStatusBar__QEAAXHKH_Z(CMFCStatusBar* pThis, int nIndex, unsigned long clrText, int bUpdate) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr || p->clrText == clrText) return;
    p->clrText = clrText;
    if (bUpdate) impl__InvalidatePaneContent_CMFCStatusBar__QEAAXH_Z(pThis, nIndex);
}

// CMFCStatusBar::SetPaneWidth(int, int) -- retail RVA 0x1344d0:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     CRect rect; ::CopyRect(&rect, &p->rect);
//     int cxOld = rect.Width() - 4;
//     int cxText = cx - p->cxIcon; if (p->cxIcon > 0) cxText -= 4;
//     p->cxText = max(0, cxText);
//     if (cx != cxOld) {
//         RecalcLayout();                                   // vslot 134
//         ::InvalidateRect(m_hWnd, NULL, TRUE); ::UpdateWindow(m_hWnd);
//     }
// Symbol: ?SetPaneWidth@CMFCStatusBar@@QEAAXHH@Z
extern "C" void MS_ABI impl__SetPaneWidth_CMFCStatusBar__QEAAXHH_Z(CMFCStatusBar* pThis, int nIndex, int cx) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    const int cxOld = (p->rect.right - p->rect.left) - 4;
    int cxText = cx - p->cxIcon;
    if (p->cxIcon > 0) cxText -= 4;
    p->cxText = (cxText < 0) ? 0 : cxText;
    if (cx != cxOld) {
        impl__RecalcLayout_CMFCStatusBar__MEAAXXZ(pThis);
        ::InvalidateRect(pThis->m_hWnd, nullptr, TRUE);
        ::UpdateWindow(pThis->m_hWnd);
    }
}

// CMFCStatusBar::SetTipText(int, LPCTSTR) -- retail RVA 0x134d10:
//     p = _GetPanePtr(nIndex); if (!p) return;
//     if (p->lpszToolTip != NULL) {
//         if (pszTipText != NULL && lstrcmpW(p->lpszToolTip, pszTipText) == 0) return;
//         free(p->lpszToolTip);
//     } else if (pszTipText == NULL || *pszTipText == 0) return;
//     p->lpszToolTip = (pszTipText != NULL && *pszTipText != 0) ? _wcsdup(pszTipText) : NULL;
//     m_dwStyle |= CBRS_TOOLTIPS;                              // CBasePane +0x104
// Symbol: ?SetTipText@CMFCStatusBar@@QEAAXHPEB_W@Z
extern "C" void MS_ABI impl__SetTipText_CMFCStatusBar__QEAAXHPEB_W_Z(CMFCStatusBar* pThis, int nIndex, const wchar_t* pszTipText) {
    if (pThis == nullptr) return;
    SbPane* p = GetPanePtr(pThis, nIndex);
    if (p == nullptr) return;
    if (p->lpszToolTip != nullptr) {
        if (pszTipText != nullptr && ::lstrcmpW(p->lpszToolTip, pszTipText) == 0) return;
        std::free(p->lpszToolTip);
    } else if (pszTipText == nullptr || *pszTipText == 0) {
        return;
    }
    p->lpszToolTip = (pszTipText != nullptr && *pszTipText != 0) ? ::_wcsdup(pszTipText) : nullptr;
    pThis->m_dwStyle |= kCBRS_TOOLTIPS;
}
