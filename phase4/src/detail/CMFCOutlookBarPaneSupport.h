#pragma once
// Shared internals of the former global_cmfcoutlookbarpane.cpp translation unit.
// Definitions live in detail/CMFCOutlookBarPaneSupport.cpp, so file-scope state keeps a
// single instance across the classes this unit was split into.
namespace openmfc { namespace detail { namespace cmfcoutlookbarpane {} } }
using namespace openmfc::detail::cmfcoutlookbarpane;
// OpenMFC: CMFCOutlookBarPane exports.
//
// All 45 exports are implemented here, transcribed from the retail mfc140u.dll
// (14.51.36231, x64) by decoding the export table (ordinal -> RVA -> objdump
// of the retail body).
//
// The class is not declared in the public headers, so `this` is taken as void*
// and the touched member offsets are mirrored with the file-local layout struct
// below.  CMFCOutlookBarPane derives from CMFCToolBar and is 11112 bytes
// (0x2b68, the size CreateObject allocates).  The stub constructors in
// manual_small_stub_implementations.cpp leave the object uninitialized, so every
// pointer access is NULL-guarded.
//
// Layout offsets harvested from the retail bodies:
//   * CWnd::m_hWnd at +0x40, m_dwStyle +0x104 / m_dwControlBarStyle +0x108.
//   * CMFCToolBar base: m_Buttons +0x1188 / m_OrigButtons +0x11c0 (CObList, 56
//     bytes each; m_Buttons.m_nCount lands at +0x11a0).
//   * Colours: text +0x1350, transparent +0x1354, back +0x1358; the inner
//     CMFCToolBarImages well at +0x1360 (0x198 bytes, ends at +0x14f8) with the
//     loaded image size at +0x13c8/+0x13cc.
//   * Scroll buttons: up child +0x14f8 (m_hWnd +0x1538), down child +0x2018
//     (m_hWnd +0x2058); OnTimer gates +0x160c/+0x1614 (up) and +0x212c/+0x2134
//     (down); the page-vs-line picker at +0x2b64.
//   * m_iSel +0x2b38, scroll offset +0x2b40, range +0x2b44/+0x2b48, the
//     ScrollDown gate +0x2b58, +0x2b5c flag and the +0x2b60 layout picker.
//
// Faithfully transcribed: OnTimer / ScrollUp / ScrollDown / ScrollPageUp /
// ScrollPageDown / SetBackColor / SetTextColor / SetTransparentColor /
// SetBackImage / RemoveAllButtons / RemoveButton / RemoveButtonByIndex /
// RestoreOriginalState / SetDefaultState / CanBeDropped / CreateObject /
// SetImage / CalcFixedLayout / OnEraseBkgnd / OnNcCalcSize / OnSetFocus /
// OnNcDestroy / OnSize / OnSysColorChange / OnCreate / EnableContextMenuItems.
// The vtable-slot indirections (0x428 AdjustLocations, 0x7d8 internal layout),
// the internal CopyButtonsList clone loop, the scroll-button child creation, the
// AFX_GLOBAL_DATA object and the static CMFCToolBarImages image well
// (0x1803c22c0) are not modeled, so those parts are kept conservative with
// `// TODO(clean-room)` markers.
//
// Conventions (see IMPLEMENTATION_GUIDE.md):
//   * every export is `extern "C"` + `__attribute__((ms_abi))`;
//   * the implicit `this` is the first explicit parameter;
//   * NULL-check pThis everywhere;
//   * struct-returning exports take the hidden return slot in the register the
//     retail body uses, as documented at each definition.
//
// Kept in a global_*.cpp file so the build's AUTO_EXCLUDES pass (the scan of
// `// Symbol:` comments) drops the generated weak stubs for these ordinals.

#include "openmfc/afxwin.h"
#include "openmfc/afxmfc.h"

#include <windows.h>

#include <cstddef>
#include <cstdint>
#include <new>

#ifdef __GNUC__
  #define MS_ABI __attribute__((ms_abi))
#else
  #define MS_ABI
#endif

//---------------------------------------------------------------------------
// Cross-file impl helpers, plus forward declarations of exports defined
// further down in this file.
//---------------------------------------------------------------------------
extern "C" void MS_ABI impl__OnSetFocus_CWnd__IEAAXPEAV1__Z(CWnd* pThis, void* pOldWnd);
extern "C" void MS_ABI impl__OnNcDestroy_CWnd__IEAAXXZ(CWnd* pThis);
extern "C" int MS_ABI impl__OnCreate_CMFCToolBar__IEAAHPEAUtagCREATESTRUCTW___Z(
    CMFCToolBar* pThis, LPCREATESTRUCTW lpCreateStruct);
extern "C" void MS_ABI impl__OnSize_CMFCToolBar__IEAAXIHH_Z(
    CMFCToolBar* pThis, unsigned int nType, int cx, int cy);
extern "C" void MS_ABI impl__OnSysColorChange_CMFCToolBar__IEAAXXZ(CMFCToolBar* pThis);
extern "C" void* MS_ABI impl___0CMFCOutlookBarPane__QEAA_XZ(void* pThis);
// Customize-mode static (retail 0x1803be35c), defined in
// feature_static_data.cpp.  Used as the gate for the context-menu edits below.
extern "C" std::int32_t impl__m_bCustomizeMode_CMFCToolBar__1HA;
extern "C" void MS_ABI impl__RemoveAllButtons_CMFCToolBar__UEAAXXZ(CMFCToolBar* pThis);
extern "C" CMFCToolBarButton* MS_ABI impl__GetButton_CMFCToolBar__QEBAPEAVCMFCToolBarButton__H_Z(
    const CMFCToolBar* pThis, int p0);
extern "C" int MS_ABI impl__PreTranslateMessage_CMFCToolBar__MEAAHPEAUtagMSG___Z(
    CMFCToolBar* pThis, MSG* pMsg);
extern "C" __int64 MS_ABI impl__Default_CWnd__IEAA_JXZ(CWnd* pThis);
extern "C" int MS_ABI impl__AddImage_CMFCToolBarImages__QEAAHPEAUHBITMAP____H_Z(
    CMFCToolBarImages* pThis, HBITMAP hBmp, int bSet);
extern "C" void MS_ABI impl__Clear_CMFCToolBarImages__QEAAXXZ(CMFCToolBarImages* pThis);
extern "C" HINSTANCE MS_ABI impl__AfxFindResourceHandle__YAPEAUHINSTANCE____PEB_W0_Z(
    const wchar_t* lpszResource, const wchar_t* lpszType);
extern "C" void MS_ABI impl__CopyButtonsList_CMFCOutlookBarPane__IEAAXAEBVCObList__AEAV2__Z(
    void* pThis, const void* pSrcList, void* pDstList);
// Sibling exports defined later in this file (used by OnTimer and
// PreTranslateMessage before their definitions).
extern "C" void MS_ABI impl__ScrollUp_CMFCOutlookBarPane__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__ScrollDown_CMFCOutlookBarPane__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__ScrollPageUp_CMFCOutlookBarPane__IEAAXXZ(void* pThis);
extern "C" void MS_ABI impl__ScrollPageDown_CMFCOutlookBarPane__IEAAXXZ(void* pThis);


//---------------------------------------------------------------------------
// Layout mirrors (offsets harvested from the retail bodies).
//---------------------------------------------------------------------------

// CMFCOutlookBarPane, sizeof 11112 (0x2b68).  The first 0x40 bytes are the
// CObject/CWnd block (m_hWnd at 0x40); the CMFCToolBar base owns the three
// CObLists at 0x1188/0x11c0/0x11f8 and everything up to 0x1350.  Only the
// fields this batch touches are named.
namespace openmfc { namespace detail { namespace cmfcoutlookbarpane {
struct OutlookBarPane
{
    void*    vfptr;                      // 0x00 CObject vfptr
    char     _pad0[0x40 - 0x08];         // 0x08..0x3f
    HWND     m_hWnd;                     // 0x40 CWnd::m_hWnd
    char     _pad1[0x104 - 0x48];        // 0x48..0x103
    DWORD    m_dwStyle;                  // 0x104
    DWORD    m_dwControlBarStyle;        // 0x108
    char     _pad2[0x1188 - 0x10c];      // 0x10c..0x1187
    CObList  m_Buttons;                  // 0x1188 CMFCToolBar::m_Buttons
    CObList  m_OrigButtons;              // 0x11c0 CMFCToolBar::m_OrigButtons
    char     _pad3[0x1350 - 0x11f8];     // 0x11f8..0x134f (incl. m_OrigResetButtons)
    COLORREF m_textColor;                // 0x1350
    COLORREF m_transparentColor;         // 0x1354
    COLORREF m_color;                    // 0x1358 cached back colour
    char     _pad4[0x1360 - 0x135c];     // 0x135c..0x135f
    CMFCToolBarImages m_imageWell;       // 0x1360 (0x198 bytes, ends 0x14f8)
    char     _pad5[0x1538 - 0x14f8];     // 0x14f8 up scroll button object (opaque)
    HWND     m_hwndScrollUp;             // 0x1538 up scroll button m_hWnd
    char     _pad6[0x160c - 0x1540];     // 0x1540..0x160b
    int      m_pScrollUpGate1;           // 0x160c OnTimer up gate (dword)
    char     _pad6a[0x1614 - 0x1610];    // 0x1610..0x1613 (unknown dword)
    int      m_pScrollUpGate2;           // 0x1614 OnTimer up gate (dword)
    char     _pad7[0x2018 - 0x1618];     // 0x1618..0x2017
    char     _pad8[0x2058 - 0x2018];     // 0x2018 down scroll button object (opaque)
    HWND     m_hwndScrollDown;           // 0x2058 down scroll button m_hWnd
    char     _pad9[0x212c - 0x2060];     // 0x2060..0x212b
    int      m_pScrollDownGate1;         // 0x212c OnTimer down gate (dword)
    char     _pad9a[0x2134 - 0x2130];    // 0x2130..0x2133 (unknown dword)
    int      m_pScrollDownGate2;         // 0x2134 OnTimer down gate (dword)
    char     _pad10[0x2b38 - 0x2138];    // 0x2138..0x2b37
    int      m_iSel;                     // 0x2b38 currently selected image
    char     _pad10a[0x2b40 - 0x2b3c];   // 0x2b3c..0x2b3f
    int      m_nScrollOffset;            // 0x2b40 scroll offset
    int      m_nScrollRangeStart;        // 0x2b44 scroll range start
    int      m_nScrollRangeEnd;          // 0x2b48 scroll range end
    char     _pad11[0x2b50 - 0x2b4c];    // 0x2b4c..0x2b4f
    void*    m_pSelectedPane;            // 0x2b50 selected/parent pane pointer
    BOOL     m_bCanScrollDown;           // 0x2b58 ScrollDown gate
    BOOL     m_bFlag1;                   // 0x2b5c
    BOOL     m_bLayoutFlag;              // 0x2b60 picks vslot 0x428 vs 0x7d8
    int      m_bScrollPageMode;          // 0x2b64 page vs line scroll picker

    // Loaded-image size cache (+0x13c8/+0x13cc) -- aliases the inner image
    // well's m_sizeImage (well @ +0x1360, m_sizeImage at well+0x68).
    // m_sizeImage is protected in the public header, so reach it through raw
    // offsets.
    int& ImageWidth() {
        return *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0x13c8);
    }
    int& ImageHeight() {
        return *reinterpret_cast<int*>(reinterpret_cast<char*>(this) + 0x13cc);
    }
};
} } }


//=============================================================================
// CMFCOutlookBarPane static data exports
//=============================================================================


//=============================================================================
// CMFCOutlookBarPane
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddBitmapImage(HBITMAP hBitmap)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(const wchar_t* lpszIcon, const wchar_t*
//                                    lpszLabel, UINT nID, int iInsertIndex)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(UINT uiImageResId, UINT uiLabelResId,
//                                    UINT nID, int iInsertIndex)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(UINT uiImageResId, const wchar_t*
//                                    lpszLabel, UINT nID, int iInsertIndex)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(HBITMAP hBitmap, const wchar_t* lpszLabel,
//                                    UINT nID, int iInsertIndex)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::AddButton(HICON hIcon, const wchar_t* lpszLabel,
//                                    UINT nID, int iInsertIndex, BOOL bAutoDestroy)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::AdjustLocations()
//=============================================================================

//=============================================================================
// CSize CMFCOutlookBarPane::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::ClearAll()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::CopyButtonsList(const CObList& src, CObList& dst)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::Create(CWnd* pParentWnd, DWORD dwStyle, UINT nID,
//                                 DWORD dwControlBarStyle)
//=============================================================================

//=============================================================================
// CMFCToolBarButton* CMFCOutlookBarPane::CreateDroppedButton(
//                                    COleDataObject* pDataObject)
//=============================================================================

//=============================================================================
// CObject* CMFCOutlookBarPane::CreateObject()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::DoPaint(CDC* pDC)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::Dock(CBasePane* pTargetBar, const RECT* lpRect,
//                               AFX_DOCK_METHOD dockMethod)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::EnableContextMenuItems(CMFCToolBarButton* pButton,
//                                                 CMenu* pMenu)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::InternalAddButton(int iImage, const wchar_t*
//                                            lpszLabel, UINT nID,
//                                            int iInsertIndex)
//=============================================================================

//=============================================================================
// AFX_CS_STATUS CMFCOutlookBarPane::IsChangeState(int nSrollOffset,
//                                                 CBasePane** ppTargetBar) const
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::OnBeforeFloat(CRect& rectFloat, AFX_DOCK_METHOD
//                                        dockMethod)
//=============================================================================

//=============================================================================
// int CMFCOutlookBarPane::OnCreate(LPCREATESTRUCTW lpCreateStruct)
//=============================================================================

//=============================================================================
// DROPEFFECT CMFCOutlookBarPane::OnDragOver(COleDataObject* pDataObject,
//                                           DWORD dwEffect, CPoint point)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::OnEraseBkgnd(CDC* pDC)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnEraseWorkArea(CDC* pDC, CRect rectWorkArea)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnLButtonUp(UINT nFlags, CPoint point)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnNcCalcSize(BOOL bCalcValidRects,
//                                       NCCALCSIZE_PARAMS* lpncsp)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnNcDestroy()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnSetFocus(CWnd* pOldWnd)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnSize(UINT nType, int cx, int cy)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnSysColorChange()
//=============================================================================

//=============================================================================
// CMFCOutlookBarPane
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::OnTimer(UINT_PTR nIDEvent)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::PreTranslateMessage(MSG* pMsg)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::RemoveAllButtons()
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::RemoveButton(UINT nID)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::RemoveButtonByIndex(int index)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::RestoreOriginalState()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::ScrollDown()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::ScrollUp()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::ScrollPageUp()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::ScrollPageDown()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::SetBackColor(COLORREF clr)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::SetBackImage(UINT iImage)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::SetDefaultState()
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::SetTextColor(COLORREF clrText, COLORREF clrTextHighlight)
//=============================================================================

//=============================================================================
// void CMFCOutlookBarPane::SetTransparentColor(COLORREF clr)
//=============================================================================

//=============================================================================
// BOOL CMFCOutlookBarPane::SmartUpdate(const CObList& lstButtons)
//=============================================================================
